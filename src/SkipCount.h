#pragma once
#include "globals.h"

namespace foo_skipcount {

	void foobarQuitting(void);
	void addMetadbIndexes(void);

	static metadb_index_manager::ptr g_cachedAPI;

	// Retain pinned data for four weeks if there are no matching items in library
	static const t_filetimestamp retentionPeriod = system_time_periods::week * 4;

	struct record_t {
		unsigned int version = CURRENT_RECORD_VERSION;
		unsigned int skipCountNext = 0;
		unsigned int skipCountRandom = 0;
		unsigned int skipCountPrevious = 0;
		unsigned int skipTimesCounter = 0;
		t_filetimestamp lastSkip = 0;
		std::vector<t_filetimestamp> skipTimes;
	};

	void refreshSingle(GUID, metadb_index_hash);
	void refreshGlobal();

	record_t getRecord(metadb_index_hash, const GUID index_guid = guid_foo_skipcount_index);
	void setRecord(metadb_index_hash, record_t, const GUID index_guid = guid_foo_skipcount_index);

	void clearSkipCount(metadb_handle_list_cref);
	void clearLastSkip(metadb_handle_list_cref);
	void clearSkipTimestamp(metadb_handle_list_cref);

	metadb_index_manager::ptr theAPI();

	// Turns metadata and location into hashes that the backend pins data to
	class my_metadb_index_client : public metadb_index_client {
	public:
		my_metadb_index_client(const char*, bool);
		metadb_index_hash transform(const file_info&, const playable_location&);

	private:
		bool forceLowercase;
		titleformat_object::ptr m_keyObj;
	};

	service_ptr_t<my_metadb_index_client> clientByGUID(const GUID&);
}