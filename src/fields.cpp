#include <string>

#include "fields.h"

#include <helpers/filetimetools.h>

#include "preferences.h"
#include "SkipCount.h"

namespace foo_skipcount {
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
		size_t index = 0;
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
		if(!record.skipTimes.empty()) {
			return record.skipTimes;
		}
		else {
			std::vector<t_filetimestamp> skipTimes;
			return skipTimes;
		}
	}

	static t_filetimestamp getLastSkip(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		if(!record.skipTimes.empty()) {
			return record.skipTimes.back();
		}
		else {
			t_filetimestamp lastSkip = 0;
			return lastSkip;
		}
	}

	static t_uint getSkipCount(record_t rec) {
		t_uint count = 0;
		if(cfg_countNext) {
			count += rec.skipCountNext;
		}
		if(cfg_countRandom) {
			count += rec.skipCountRandom;
		}
		if(cfg_countPrevious) {
			count += rec.skipCountPrevious;
		}
		return count;
	}

	inline static std::string getSkipCountStr(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		return std::to_string(getSkipCount(record));
	}

	void my_metadb_display_field_provider::get_field_name(t_uint32 index, pfc::string_base& out) {
		PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
		switch(index) {
			case SKIP_COUNT:
				out = "skip_count";
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

	bool my_metadb_display_field_provider::process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) {
		PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
		metadb_index_hash hash;

		if(!clientByGUID(guid_foo_skipcount_index)->hashHandle(handle, hash)) {
			return false;
		}

		switch(index) {
			case SKIP_COUNT: {
				out->write(titleformat_inputtypes::meta, getSkipCountStr(hash).c_str());
				break;
			}
			case LAST_SKIP:
			{
				t_filetimestamp lastSkip = getLastSkip(hash);
				if(lastSkip > 0) {
					out->write(titleformat_inputtypes::meta, foobar2000_io::format_filetimestamp(lastSkip));
				}
				else {
					out->write(titleformat_inputtypes::meta, "Never");
				}
				break;
			}
			case SKIP_TIMES_JS:
			case SKIP_TIMES_RAW: {
				std::vector<t_filetimestamp> skipTimes = getSkipTimes(hash);
				if(skipTimes.empty()) {
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
		}

		return true;
	}

	void my_track_property_provider::workThisIndex(GUID const& whichID, double priorityBase, metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
		auto client = clientByGUID(whichID);
		pfc::avltree_t<metadb_index_hash> hashes;
		for(size_t trackIndex = 0; trackIndex < p_tracks.get_count(); trackIndex++) {
			metadb_index_hash hash;
			if(client->hashHandle(p_tracks[trackIndex], hash)) {
				hashes += hash;
			}
		}

		t_uint skipCount = 0, nextCount = 0, randomCount = 0, previousCount = 0;
		pfc::string8 skipTimesStr = "", skipTimesRawStr = "", skipTimesJSStr = "";
		t_filetimestamp latestSkip = 0;
		{
			t_uint recCount = 0;
			record_t firstRecord;
			for(auto i = hashes.first(); i.is_valid(); i++) {
				record_t rec = getRecord(*i);
				if(i == hashes.first()) {
					firstRecord = rec;
				}

				skipCount += getSkipCount(rec);
				nextCount += rec.skipCountNext;
				randomCount += rec.skipCountRandom;
				previousCount += rec.skipCountPrevious;
				if(!rec.skipTimes.empty() && latestSkip < rec.skipTimes.back()) {
					latestSkip = rec.skipTimes.back();
				}

				recCount++;
			}

			if(recCount == 1 && firstRecord.skipTimesCounter > 0) {
				skipTimesStr = getSkipTimesStr(firstRecord.skipTimes, true, false, true).c_str();
#define MAX_PROPERTY_LENGTH 500
				if(skipTimesStr.get_length() > MAX_PROPERTY_LENGTH) {
					skipTimesStr.truncate(MAX_PROPERTY_LENGTH);
					skipTimesStr += "...";
				}
				skipTimesRawStr = getSkipTimesStr(firstRecord.skipTimes, false, false, true).c_str();
				if(skipTimesRawStr.get_length() > MAX_PROPERTY_LENGTH) {
					skipTimesRawStr.truncate(MAX_PROPERTY_LENGTH);
					skipTimesRawStr += "...";
				}
				skipTimesJSStr = getSkipTimesStr(firstRecord.skipTimes, false, true, true).c_str();
				if(skipTimesJSStr.get_length() > MAX_PROPERTY_LENGTH) {
					skipTimesJSStr.truncate(MAX_PROPERTY_LENGTH);
					skipTimesJSStr += "...";
				}
			}
			else {
				skipTimesStr = "";
			}
		}

		p_out.set_property(strPropertiesGroup, priorityBase + 1, PFC_string_formatter() << "Skipped", PFC_string_formatter() << skipCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 2, PFC_string_formatter() << "Next skipped", PFC_string_formatter() << nextCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 3, PFC_string_formatter() << "Random skipped", PFC_string_formatter() << randomCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 4, PFC_string_formatter() << "Previous skipped", PFC_string_formatter() << previousCount << " times");
		if(skipTimesStr.length() > 0) {
			p_out.set_property(strPropertiesGroup, priorityBase + 5, PFC_string_formatter() << "Last skip", PFC_string_formatter() << foobar2000_io::format_filetimestamp(latestSkip));
			p_out.set_property(strPropertiesGroup, priorityBase + 6, PFC_string_formatter() << "Skip times", PFC_string_formatter() << skipTimesStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 7, PFC_string_formatter() << "Skip times raw", PFC_string_formatter() << skipTimesRawStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 8, PFC_string_formatter() << "Skip times JS", PFC_string_formatter() << skipTimesJSStr);
		}
	}
} // namespace foo_skipcount
