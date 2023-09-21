#pragma once
#include "stdafx.h"
#include "globals.h"

namespace foo_skipcount {

	// Skip times logging
	enum ContextClearEnum {
		cmd_clear_skipcount = 0,
		cmd_clear_lastskip = 1,
		cmd_clear_allbutlastskip = 2,
		cmd_clear_skiptimes = 3,
		cmd_total
	};

	// Context menu group.
	// {3AEB1668-9FD0-450F-856F-EB6C1C240187}
	static const GUID context_menu_guid = { 0x3aeb1668, 0x9fd0, 0x450f, { 0x85, 0x6f, 0xeb, 0x6c, 0x1c, 0x24, 0x01, 0x87 } };
	// Switch to contextmenu_group_factory to embed commands in the root menu but separated from other commands.
	static contextmenu_group_popup_factory g_my_group(context_menu_guid, contextmenu_groups::root, "Skip Count", 0);

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