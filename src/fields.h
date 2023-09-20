#pragma once
#include "stdafx.h"
#include "globals.h"

namespace foo_skipcount {

	enum provided_fields {
		SKIP_COUNT = 0,
		LAST_SKIP = 1,
		SKIP_TIMES_JS = 2,
		SKIP_TIMES_RAW = 3,
		MAX_NUM_FIELDS
	};

	class my_metadb_display_field_provider : public metadb_display_field_provider {
	public:
		t_uint32 get_field_count() {
			return MAX_NUM_FIELDS;
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

		bool is_our_tech_info(const char* p_name) {
			// No tech info read from files
			return false;
		}
	};

	static service_factory_single_t<my_track_property_provider> g_my_track_property_provider;

} // namespace foo_skipcount