#pragma once
#include "stdafx.h"
#include "globals.h"

// Skip times logging
enum {
	cmd_clear_skipcount = 0,
	cmd_clear_lastskip = 1,
	cmd_clear_allbutlastskip = 2,
	cmd_clear_skiptimes = 3,
	cmd_total
};

namespace foo_skipcount {

	// Simple context menu item class.
	class my_contextmenu_item : public contextmenu_item_simple {
	public:
		GUID get_parent() {
			return context_menu_guid;
		}
		
		unsigned get_num_items() {
			return cmd_total;
		}

		void get_item_name(unsigned int, pfc::string_base&);
		bool context_get_display(unsigned int, metadb_handle_list_cref, pfc::string_base&, unsigned int&, const GUID&);
		void context_command(unsigned int, metadb_handle_list_cref, const GUID&);
		GUID get_item_guid(unsigned int);
		bool get_item_description(unsigned, pfc::string_base&);
	};

	static contextmenu_item_factory_t<my_contextmenu_item> g_my_contextmenu_item_factory;

} // namespace foo_skipcount