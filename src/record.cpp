#include <vector>
#include <mutex>

#include "SkipCount.h"
#include "record.h"
#include "preferences.h"

namespace foo_skipcount {
	// Dynamic allocation is not possible
	record_t getRecord(metadb_index_hash hash, const GUID index_guid) {
		t_uint buf[10005];
		record_t record;
		size_t size = 0;
		size = theAPI()->get_user_data_here(index_guid, hash, &buf, sizeof(buf));
		if(size == 0) {
			return record;
		}

		if(buf[0] == 1) {
			record.version = buf[0];
			record.skipCountNext = buf[1];
			record.skipCountRandom = buf[2];
			record.skipCountPrevious = buf[3];
		}
		else if(buf[0] == 2) {
			record.version = buf[0];
			record.skipCountNext = buf[1];
			record.skipCountRandom = buf[2];
			record.skipCountPrevious = buf[3];
			record.skipTimesCounter = buf[4];
			if(record.skipTimesCounter > 0) {
				std::vector<t_filetimestamp>::iterator iter = record.skipTimes.begin();
				record.skipTimes.insert(iter, (t_filetimestamp*)&buf[5], (t_filetimestamp*)&buf[5] + record.skipTimesCounter);
			}
		}
		return record;
	}

	std::mutex set_record_mutex;
	void setRecord(metadb_index_hash hash, record_t record, const GUID index_guid) {
		t_uint* buf = new t_uint[5 + record.skipTimes.size() * sizeof(t_filetimestamp) / sizeof(t_uint)];
		record.skipTimesCounter = static_cast<t_uint>(record.skipTimes.size());

		// Update record version before new set
		record.version = CURRENT_RECORD_VERSION;
		// Copy over the first five (non-vector, non-timestamp) settings
		memcpy(buf, &record, 5 * sizeof(t_uint));
		size_t size = 5;

		// Copy over the vector of skip timestamps
		if(!record.skipTimes.empty()) {
			memcpy(buf + size, &record.skipTimes[0], record.skipTimes.size() * sizeof(t_filetimestamp));
			size += record.skipTimes.size() * sizeof(t_filetimestamp) / sizeof(t_uint);
		}

		std::lock_guard<std::mutex> guard(set_record_mutex);
		theAPI()->set_user_data(index_guid, hash, buf, size * sizeof(t_uint));
		delete[] buf;
	}
}
