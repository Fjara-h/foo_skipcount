#include <string>

#include "fields.h"
#include "SkipCount.h"

#include <helpers/filetimetools.h>

#include "record.h"

namespace foo_skipcount {
	void my_metadb_display_field_provider::get_field_name(t_uint32 index, pfc::string_base& out) {
		PFC_ASSERT(index >= 0 && index < FIELDS_TOTAL);
		switch(index) {
			case FIELD_SKIP_COUNT:
				out = "skip_count";
				break;
			case FIELD_LAST_SKIP:
				out = "last_skip";
				break;
			case FIELD_LATEST_SKIP:
				out = "latest_skip";
				break;
			case FIELD_FIRST_SKIP:
				out = "first_skip";
				break;
			case FIELD_OLDEST_SKIP:
				out = "oldest_skip";
				break;
			case FIELD_SKIP_TIMES:
				out = "skip_times";
				break;
			case FIELD_SKIP_TIMES_JS:
				out = "skip_times_js";
				break;
			case FIELD_SKIP_TIMES_RAW:
				out = "skip_times_raw";
				break;
		}
	}

	bool my_metadb_display_field_provider::process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) {
		PFC_ASSERT(index >= 0 && index < FIELDS_TOTAL);
		metadb_index_hash hash;
		if(!clientByGUID(guid_foo_skipcount_index)->hashHandle(handle, hash)) {
			return false;
		}
		record_t record = getRecord(hash);

		switch(index) {
			case FIELD_SKIP_COUNT: {
				out->write(titleformat_inputtypes::meta, std::to_string(record.skipCountNext + record.skipCountRandom + record.skipCountPrevious + record.skipCountDoubleClick).c_str());
				break;
			}
			case FIELD_LAST_SKIP:
			case FIELD_LATEST_SKIP:
			case FIELD_FIRST_SKIP:
			case FIELD_OLDEST_SKIP: {
				t_filetimestamp skip = (index == FIELD_LAST_SKIP || index == FIELD_LATEST_SKIP) ? getLatestSkip(record) : getOldestSkip(record);
				if(skip == 0) {
					out->write(titleformat_inputtypes::meta, "Never");
				}
				else {
					out->write(titleformat_inputtypes::meta, foobar2000_io::format_filetimestamp(skip));
				}
				break;
			}
			case FIELD_SKIP_TIMES:
			case FIELD_SKIP_TIMES_JS: 
			case FIELD_SKIP_TIMES_RAW: {
				if(record.skipTimes.empty()) {
					out->write(titleformat_inputtypes::meta, "[]");
				}
				else {
					out->write(titleformat_inputtypes::meta, getSkipTimesStr(record.skipTimes, (index == FIELD_SKIP_TIMES_JS), (index == FIELD_SKIP_TIMES)).c_str());
				}
				break;
			}
		}
		return true;
	}

	void my_track_property_provider::workThisIndex(GUID const& whichID, double priorityBase, metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
		service_ptr_t<my_metadb_index_client> client = clientByGUID(whichID);
		pfc::avltree_t<metadb_index_hash> hashes;
		for(metadb_handle_ptr trackptr : p_tracks) {
			metadb_index_hash hash;
			if(client->hashHandle(trackptr, hash)) {
				hashes += hash;
			}
		}

		metadb_index_hash firstHash;
		if(!hashes.get_first(firstHash)) {
			return;
		}

		t_uint skipCount = 0, nextCount = 0, randomCount = 0, previousCount = 0, doubleClickCount = 0;
		pfc::stringLite skipTimesStr = "", skipTimesRawStr = "", skipTimesJSStr = "", latestSkipStr = "", oldestSkipStr = "";
		t_filetimestamp latestSkip = filetimestamp_invalid, oldestSkip = UINTPTR_MAX;

		record_t firstRecord;
		for(metadb_index_hash hash : hashes) {
			record_t record = getRecord(hash);
			if(hash == firstHash) {
				firstRecord = record;
			}

			skipCount += record.skipCountNext + record.skipCountRandom + record.skipCountPrevious + record.skipCountDoubleClick;
			nextCount += record.skipCountNext;
			randomCount += record.skipCountRandom;
			previousCount += record.skipCountPrevious;
			doubleClickCount += record.skipCountDoubleClick;
			if(!record.skipTimes.empty()) {
				if(oldestSkip > record.skipTimes.front()) {
					oldestSkip = record.skipTimes.front();
				}
				if(latestSkip < record.skipTimes.back()) {
					latestSkip = record.skipTimes.back();
				}
			}
		}

		if(hashes.get_count() == 1 && !firstRecord.skipTimes.empty()) {
			skipTimesStr = getSkipTimesStr(firstRecord.skipTimes, false, true, ", ", true).c_str();
#define MAX_PROPERTY_LENGTH 500
			if(skipTimesStr.get_length() > MAX_PROPERTY_LENGTH) {
				skipTimesStr.truncate(MAX_PROPERTY_LENGTH);
				skipTimesStr += "...";
			}
			skipTimesRawStr = getSkipTimesStr(firstRecord.skipTimes, false, false, ", ", true).c_str();
			if(skipTimesRawStr.get_length() > MAX_PROPERTY_LENGTH) {
				skipTimesRawStr.truncate(MAX_PROPERTY_LENGTH);
				skipTimesRawStr += "...";
			}
			skipTimesJSStr = getSkipTimesStr(firstRecord.skipTimes, true, false, ", ", true).c_str();
			if(skipTimesJSStr.get_length() > MAX_PROPERTY_LENGTH) {
				skipTimesJSStr.truncate(MAX_PROPERTY_LENGTH);
				skipTimesJSStr += "...";
			}
		}

		oldestSkipStr = (oldestSkip == UINTPTR_MAX) ? "Never" : foobar2000_io::format_filetimestamp(oldestSkip);
		latestSkipStr = (latestSkip == filetimestamp_invalid) ? "Never" : foobar2000_io::format_filetimestamp(latestSkip);

		p_out.set_property(strPropertiesGroup, priorityBase + 1, PFC_string_formatter() << "Skip count", PFC_string_formatter() << skipCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 2, PFC_string_formatter() << "Next skipped", PFC_string_formatter() << nextCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 3, PFC_string_formatter() << "Random skipped", PFC_string_formatter() << randomCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 4, PFC_string_formatter() << "Previous skipped", PFC_string_formatter() << previousCount << " times");
		p_out.set_property(strPropertiesGroup, priorityBase + 5, PFC_string_formatter() << "Double Click skipped", PFC_string_formatter() << doubleClickCount << " times");
		if(!skipTimesStr.isEmpty()) {
			p_out.set_property(strPropertiesGroup, priorityBase + 6, PFC_string_formatter() << "Latest skip", PFC_string_formatter() << latestSkipStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 7, PFC_string_formatter() << "Oldest skip", PFC_string_formatter() << oldestSkipStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 8, PFC_string_formatter() << "Skip times", PFC_string_formatter() << skipTimesStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 9, PFC_string_formatter() << "Skip times raw", PFC_string_formatter() << skipTimesRawStr);
			p_out.set_property(strPropertiesGroup, priorityBase + 10, PFC_string_formatter() << "Skip times JS", PFC_string_formatter() << skipTimesJSStr);
		}
	}
} // namespace foo_skipcount
