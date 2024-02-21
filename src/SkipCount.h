#ifndef SKIPCOUNT_SKIPCOUNT_H
#define SKIPCOUNT_SKIPCOUNT_H

#include "globals.h"

namespace foo_skipcount {
	// Pattern used to pin data in the config - Tracks evaluating to the same string are considered to the same, thus sharing skip statistics
	static const char strPinTo[] = "%artist% - $year($if2(%date%,%original release date%)) - %album% $if2(%discnumber%,1)-%tracknumber% %title%";

	// Retain pinned data for four weeks if there are no matching items in library
	static const t_filetimestamp retentionPeriod = system_time_periods::week * 4;

	void foobarQuitting(void);
	void addMetadbIndexes(void);

	static metadb_index_manager::ptr g_cachedAPI;
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
} // namespace foo_skipcount
#endif //SKIPCOUNT_SKIPCOUNT_H
