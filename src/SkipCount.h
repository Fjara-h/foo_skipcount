#pragma once
#include "globals.h"

namespace foo_skipcount {

	void foobarQuitting(void);
	void addMetadbIndexes(void);

	static metadb_index_manager::ptr g_cachedAPI;

	// Retain pinned data for four weeks if there are no matching items in library
	static const t_filetimestamp retentionPeriod = system_time_periods::week * 4;

	struct record_t {
		t_uint version = CURRENT_RECORD_VERSION;
		t_uint skipCountNext = 0;
		t_uint skipCountRandom = 0;
		t_uint skipCountPrevious = 0;
		t_uint skipTimesCounter = 0;
		t_filetimestamp lastSkip = 0;
		std::vector<t_filetimestamp> skipTimes;
	};

	void refreshSingle(GUID, metadb_index_hash);
	void refreshGlobal();

	record_t getRecord(metadb_index_hash hash, const GUID index_guid = guid_foo_skipcount_index);
	void setRecord(metadb_index_hash hash, record_t record, const GUID index_guid = guid_foo_skipcount_index);

	void clearRecords(metadb_handle_list_cref items);
	void clearTimestamps(metadb_handle_list_cref items);

	metadb_index_manager::ptr theAPI();

	// Turns metadata and location into hashes that the backend pins data to
	class my_metadb_index_client : public metadb_index_client {
	public:
		my_metadb_index_client(const char* pinTo, bool toLower);
		metadb_index_hash transform(const file_info& info, const playable_location& location);

	private:
		bool forceLowercase;
		titleformat_object::ptr m_keyObj;
	};

	service_ptr_t<my_metadb_index_client> clientByGUID(const GUID& guid);
}