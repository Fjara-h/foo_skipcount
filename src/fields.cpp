#include "preferences.h"
#include "SkipCount.h"
#include <string>

namespace foo_skipcount {

	enum provided_fields {
		SKIP_COUNT,
		MAX_NUM_FIELDS
	};

	static std::string skipCount_get(metadb_index_hash hash) {
		record_t record = getRecord(hash);
		unsigned int count = 0;
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

	class metadb_display_field_provider_impl : public metadb_display_field_provider {
	public:
		t_uint32 get_field_count() {
			return MAX_NUM_FIELDS;
		}
		void get_field_name(t_uint32 index, pfc::string_base& out) {
			PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
			switch(index) {
				case SKIP_COUNT:
					out = "skip_count";
					break;
			}
		}
		bool process_field(t_uint32 index, metadb_handle* handle, titleformat_text_out* out) {
			PFC_ASSERT(index >= 0 && index < MAX_NUM_FIELDS);
			metadb_index_hash hash;

			if(!clientByGUID(guid_foo_skipcount_index)->hashHandle(handle, hash)) {
				return false;
			}

			switch(index) {
				case SKIP_COUNT:
					out->write(titleformat_inputtypes::meta, skipCount_get(hash).c_str());
					break;
			}

			return true;
		}
	};

	static service_factory_single_t<metadb_display_field_provider_impl> g_metadb_display_field_provider_impl;

	static const char strPropertiesGroup[] = "Skip Count Statistics";

	// This class provides our information for the properties dialog
	class track_property_provider_impl : public track_property_provider_v2 {
	public:
		void workThisIndex(GUID const& whichID, double priorityBase, metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
			auto client = clientByGUID(whichID);
			pfc::avltree_t<metadb_index_hash> hashes;
			const size_t trackCount = p_tracks.get_count();
			for(size_t trackWalk = 0; trackWalk < trackCount; trackWalk++) {
				metadb_index_hash hash;
				if(client->hashHandle(p_tracks[trackWalk], hash)) {
					hashes += hash;
				}
			}

			unsigned int count = 0;
			{
				for (auto i = hashes.first(); i.is_valid(); i++) {
					record_t rec = getRecord(*i);
					if(cfg_countNext) {
						count += rec.skipCountNext;
					}
					if(cfg_countRandom) {
						count += rec.skipCountRandom;
					}
					if(cfg_countPrevious) {
						count += rec.skipCountPrevious;
					}
				}
			}

			p_out.set_property(strPropertiesGroup, priorityBase + 0, PFC_string_formatter() << "Skipped", PFC_string_formatter() << count << " times");
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

	static service_factory_single_t<track_property_provider_impl> g_track_property_provider_impl;
}