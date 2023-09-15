#include "preferences.h"
#include "SkipCount.h"

#include <helpers/filetimetools.h>

#include <string>

namespace foo_skipcount {

	enum provided_fields {
		SKIP_COUNT = 0,
		LAST_SKIP = 1,
		SKIP_TIMES_JS = 2,
		SKIP_TIMES_RAW = 3,
		MAX_NUM_FIELDS
	};

	std::string t_uint64_to_string(t_uint64 value, bool jsTimestamp) {
		if(jsTimestamp) {
			return std::to_string(pfc::fileTimeWtoU(value) * 1000); // convert to unix timestamp, then add milliseconds for JS
		}
		else {
			return std::to_string(value);
		}
	}

	static std::string getSkipTimesStr(std::vector<t_filetimestamp> skipTimes, bool convertTimeStamp, bool jsTimeStamp, bool noArrayChars = false) {
		std::string str;
		unsigned int index = 0;
		if(!noArrayChars) {
			str += "[";
		}
		for(std::vector<t_filetimestamp>::iterator iter = skipTimes.begin(); iter != skipTimes.end(); iter++, index++) {
			if(*iter != 0) {
				if(convertTimeStamp) {
					if(!noArrayChars) {
						str += "\"";
					}
					str.append(foobar2000_io::format_filetimestamp(*iter));
					if(!noArrayChars) {
						str += "\"";
					}
				}
				else {
					str += t_uint64_to_string(*iter, jsTimeStamp);
				}
				if(index + 1 < skipTimes.size()) {
					str.append(", ");
				}
			}
		}
		if(!noArrayChars) {
			str += "]";
		}
		return str;
	}

	static std::vector<t_filetimestamp> getSkipTimes(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		if(record.skipTimes.empty()) {
			std::vector<t_filetimestamp> skipTimes;
			return skipTimes;
		}
		else {
			return record.skipTimes;
		}
	}

	static t_filetimestamp getLastSkip(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		return record.lastSkip;
	}

	static std::string getSkipCount(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		t_uint count = 0;
		if(cfg_countNext) {
			count += record.skipCountNext;
		}
		if(cfg_countRandom) {
			count += record.skipCountRandom;
		}
		if(cfg_countPrevious) {
			count += record.skipCountPrevious;
		}
		return std::to_string(count);
	}

	class foo_metadb_display_field_provider : public metadb_display_field_provider {
	public:
		t_uint32 get_field_count() {
			return MAX_NUM_FIELDS;
		}
		void get_field_name(t_uint32 index, pfc::string_base& out) {
			PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
			switch(index) {
				case SKIP_COUNT:
					out = cfg_skipFieldPattern;
					break;
				case LAST_SKIP:
					out = "last_skip";
					break;
				case SKIP_TIMES_JS:
					out = "skip_times_js";
					break;
				case SKIP_TIMES_RAW:
					out = "skip_times_raw";
					break;
			}
		}

		bool process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) {
			PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
			metadb_index_hash hash;

			if(!clientByGUID(guid_foo_skipcount_index)->hashHandle(handle, hash)) {
				return false;
			}

			t_filetimestamp lastSkip;
			std::vector<t_filetimestamp> skipTimes;
			switch(index) {
				case SKIP_COUNT:
					out->write(titleformat_inputtypes::meta, getSkipCount(hash).c_str());
					break;
				case LAST_SKIP:
					lastSkip = getLastSkip(hash);
					if(lastSkip > 0) {
						out->write(titleformat_inputtypes::meta, foobar2000_io::format_filetimestamp(lastSkip));
					}
					else {
						out->write(titleformat_inputtypes::meta, "N/A");
					}
					break;
				case SKIP_TIMES_JS:
				case SKIP_TIMES_RAW:
					skipTimes = getSkipTimes(hash);
					if(!skipTimes.size()) {
						out->write(titleformat_inputtypes::meta, "[]");
					}
					else {
						if(index == SKIP_TIMES_JS) {
							out->write(titleformat_inputtypes::meta, getSkipTimesStr(skipTimes, false, true).c_str());
						}
						else {
							out->write(titleformat_inputtypes::meta, getSkipTimesStr(skipTimes, false, false).c_str());
						}
					}
					break;
			}

			return true;
		}
	};

	static service_factory_single_t<foo_metadb_display_field_provider> g_foo_metadb_display_field_provider;

	static const char strPropertiesGroup[] = "Skip Count Statistics";

	// This class provides config information for the properties dialog
	class foo_track_property_provider : public track_property_provider_v2 {
	public:
		void workThisIndex(GUID const& whichID, double priorityBase, metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
			auto client = clientByGUID(whichID);
			pfc::avltree_t<metadb_index_hash> hashes;
			for(size_t trackIndex = 0; trackIndex < p_tracks.get_count(); trackIndex++) {
				metadb_index_hash hash;
				if(client->hashHandle(p_tracks[trackIndex], hash)) {
					hashes += hash;
				}
			}

			unsigned int skipCount = 0;
			pfc::string8 lastSkip = "", skipTimes = "";
			{
				unsigned int recCount = 0;
				record_t firstRecord;
				t_filetimestamp lastestSkip = 0;
				for(auto i = hashes.first(); i.is_valid(); i++) {
					record_t rec = getRecord(*i);
					if(recCount == 0) {
						firstRecord = rec;
					}

					if(cfg_countNext) {
						skipCount += rec.skipCountNext;
					}
					if(cfg_countRandom) {
						skipCount += rec.skipCountRandom;
					}
					if(cfg_countPrevious) {
						skipCount += rec.skipCountPrevious;
					}
					if(rec.lastSkip > lastestSkip) {
						lastestSkip = rec.lastSkip;
					}
					recCount++;
				}

				if(recCount == 1) {
					skipTimes = getSkipTimesStr(firstRecord.skipTimes, true, false, true).c_str();
#define MAX_PROPERTY_LENGTH 500
					if(skipTimes.get_length() > MAX_PROPERTY_LENGTH) {
						skipTimes.truncate(MAX_PROPERTY_LENGTH);
						skipTimes += "...";
					}
				}
				else {
					skipTimes = "N/A";
				}
				if(lastestSkip != 0) {
					lastSkip = foobar2000_io::format_filetimestamp(lastestSkip);
				}
			}

			p_out.set_property(strPropertiesGroup, priorityBase + 0, PFC_string_formatter() << "Skipped", PFC_string_formatter() << skipCount << " times");
			p_out.set_property(strPropertiesGroup, priorityBase + 0, PFC_string_formatter() << "Skip times", PFC_string_formatter() << skipTimes << " times");
			p_out.set_property(strPropertiesGroup, priorityBase + 0, PFC_string_formatter() << "Last skipped", PFC_string_formatter() << lastSkip);
		}

		void enumerate_properties(metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
			workThisIndex(guid_foo_skipcount_index, 0, p_tracks, p_out);
		}

		void enumerate_properties_v2(metadb_handle_list_cref p_tracks, track_property_callback_v2& p_out) {
			if(p_out.is_group_wanted(strPropertiesGroup)) {
				enumerate_properties(p_tracks, p_out);
			}
		}

		bool is_our_tech_info(const char* p_name) {
			// No tech info read from files
			return false;
		}
	};

	static service_factory_single_t<foo_track_property_provider> g_foo_track_property_provider;
}