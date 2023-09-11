#pragma once
#include "globals.h"

namespace foo_skipcount {
	void foobarQuitting(void);
	void addMetadbIndexes(void);
	static metadb_index_manager::ptr g_cachedAPI;
	// Retain pinned data for four weeks if there are no matching items in library
	static const t_filetimestamp retentionPeriod = system_time_periods::week * 4;

#define kCurrRecordVersion 1
	struct record_t {
		unsigned int version = kCurrRecordVersion;
		unsigned int skipCountNext = 0;
		unsigned int skipCountRandom = 0;
		unsigned int skipCountPrevious = 0;
	};

	void RefreshSingle(GUID, metadb_index_hash);
	void RefreshGlobal();
	record_t getRecord(metadb_index_hash hash, const GUID index_guid = guid_foo_skipcount_index);
	void setRecord(metadb_index_hash hash, record_t record, const GUID index_guid = guid_foo_skipcount_index);

	struct hash_record {
		metadb_index_hash hash;
		metadb_handle_ptr mdb_handle;
		record_t record;
		hash_record(metadb_handle_ptr mdb_ptr) : mdb_handle(mdb_ptr) {}
	};

	void ClearSkipCountRecords(metadb_handle_list_cref items);
	metadb_index_manager::ptr theAPI();

	// Turns metadata and location into hashes which are pinned by the backend
	class metadb_index_client_impl : public metadb_index_client {
	public:
		metadb_index_client_impl(const char* pinTo, bool toLower);
		metadb_index_hash transform(const file_info& info, const playable_location& location);
		
	private:
		bool forceLowercase;
		titleformat_object::ptr m_keyObj;
	};

	service_ptr_t<metadb_index_client_impl> clientByGUID(const GUID& guid);
}