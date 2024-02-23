#ifndef SKIPCOUNT_FIELDS_H
#define SKIPCOUNT_FIELDS_H

#include "globals.h"

namespace foo_skipcount {
	enum provided_fields {
		FIELD_SKIP_COUNT = 0,
		FIELD_LATEST_SKIP = 1,
		FIELD_LAST_SKIP = 2,
		FIELD_FIRST_SKIP = 3,
		FIELD_OLDEST_SKIP = 4,
		FIELD_SKIP_TIMES = 5,
		FIELD_SKIP_TIMES_JS = 6,
		FIELD_SKIP_TIMES_RAW = 7,
		FIELDS_TOTAL
	};

	class my_metadb_display_field_provider : public metadb_display_field_provider {
	public:
		inline t_uint32 get_field_count() {
			return FIELDS_TOTAL;
		}

		void get_field_name(t_uint32, pfc::string_base&);
		bool process_field(t_uint32, metadb_handle*, titleformat_text_out*);
	};
	static service_factory_single_t<my_metadb_display_field_provider> g_my_metadb_display_field_provider;

	static const char strPropertiesGroup[] = "Skip Count Statistics";
	// This class provides config information for the properties dialog
	class my_track_property_provider : public track_property_provider_v2 {
	public:
		void workThisIndex(GUID const&, double, metadb_handle_list_cref, track_property_callback&);

		void enumerate_properties(metadb_handle_list_cref p_tracks, track_property_callback& p_out) {
			workThisIndex(guid_foo_skipcount_index, 0, p_tracks, p_out);
		}

		void enumerate_properties_v2(metadb_handle_list_cref p_tracks, track_property_callback_v2& p_out) {
			if(p_out.is_group_wanted(strPropertiesGroup)) {
				enumerate_properties(p_tracks, p_out);
			}
		}

		inline bool is_our_tech_info(const char*) {
			// No tech info read from files
			return false;
		}
	};
	static service_factory_single_t<my_track_property_provider> g_my_track_property_provider;
} // namespace foo_skipcount
#endif // SKIPCOUNT_FIELDS_H
