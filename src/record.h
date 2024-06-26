#ifndef SKIPCOUNT_RECORD_H
#define SKIPCOUNT_RECORD_H

#include <vector>
#include <string>

#include <helpers/filetimetools.h>

#include "preferences.h"

namespace foo_skipcount {
	struct record_t {
		t_uint version = CURRENT_RECORD_VERSION;
		t_uint skipCountNext = 0;
		t_uint skipCountRandom = 0;
		t_uint skipCountPrevious = 0;
		t_uint skipCountDoubleClick = 0;
		t_uint skipTimesCounter = 0; // Used ONLY for size in getRecord. Use skiptimes.size() instead.
		std::vector<t_filetimestamp> skipTimes;
	};

	record_t getRecord(metadb_index_hash, const GUID index_guid = guid_foo_skipcount_index);
	void setRecord(metadb_index_hash, record_t, const GUID index_guid = guid_foo_skipcount_index);

	// Convert Windows timestamp to local system date-time format
	inline static t_uint64 getLocalTimestamp(t_filetimestamp timestamp, bool JS) {
		return JS ? pfc::fileTimeWtoU(timestamp) * 1000 : timestamp;
	}

	inline static t_filetimestamp getLatestSkip(record_t record) {
		return record.skipTimes.empty() ? 0 : record.skipTimes.back();
	}

	inline static t_filetimestamp getOldestSkip(record_t record) {
		return record.skipTimes.empty() ? 0 : record.skipTimes.front();
	}

	static std::string getSkipTimesStr(std::vector<t_filetimestamp> skipTimes, bool JS, bool dateTime, bool useArrayChars = true, std::string delimiter = ", ") {
		if(skipTimes.empty()) {
			return useArrayChars ? "[]" : "";
		}
		std::string str = useArrayChars ? "[" : "";
		for(t_filetimestamp& time : skipTimes) {
			if(dateTime) {
				str.append(useArrayChars ? "\"" : "");
				str.append(foobar2000_io::format_filetimestamp(time));
				str.append(useArrayChars ? "\"" : "");
			}
			else {
				str.append(std::to_string(getLocalTimestamp(time, JS)));
			}

			if(&time != &skipTimes.back()) {
				str.append(delimiter);
			}
		}
		str.append(useArrayChars ? "]" : "");
		return str;
	}

	static t_int64 parseInt(const char* str) {
		char* out = nullptr;
		t_int64 num = strtoll(str, &out, 10);
		if(!(*out) && strlen(str) > 0) {
			return num;
		}
		else return -1;
	}

	static std::vector<t_filetimestamp> getRawSkipTimesFromString(const char* ch, std::string delimiter = AllDelimiterCharacters) {
		std::vector<t_filetimestamp> skipTimes;
		std::string str(ch);
		char* token = NULL;
		char* nextTokenPtr = NULL;
		token = strtok_s(str.data(), delimiter.c_str(), &nextTokenPtr);
		while(token != NULL) {
			skipTimes.push_back(parseInt(token));
			token = strtok_s(NULL, delimiter.c_str(), &nextTokenPtr);
		}
		return skipTimes;
	}
}
#endif //SKIPCOUNT_RECORD_H
