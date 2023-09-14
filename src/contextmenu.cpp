#include "stdafx.h"
#include "SkipCount.h"

// Context menu group.
// {3AEB1668-9FD0-450F-856F-EB6C1C240187}
static const GUID context_menu_guid = { 0x3aeb1668, 0x9fd0, 0x450f, { 0x85, 0x6f, 0xeb, 0x6c, 0x1c, 0x24, 0x01, 0x87 } };

// Switch to contextmenu_group_factory to embed commands in the root menu but separated from other commands.
static contextmenu_group_popup_factory g_my_group(context_menu_guid, contextmenu_groups::root, "Skip Count", 0);

namespace foo_skipcount {

	// Simple context menu item class.
	class my_item : public contextmenu_item_simple {
	public:
		enum {
			cmd_clear_skipcount = 0,
			cmd_clear_skiptimes = 1,
			cmd_total
		};

		GUID get_parent() {
			return context_menu_guid;
		}

		unsigned get_num_items() {
			return cmd_total;
		}

		void get_item_name(unsigned p_index, pfc::string_base& p_out) {
			switch(p_index) {
				case cmd_clear_skipcount:
					p_out = "Clear saved skip count";
					break;
				case cmd_clear_skiptimes:
					p_out = "Clear skip timestamps";
					break;
				default:
					uBugCheck(); // Failsafe
			}
		}

		bool context_get_display(unsigned p_index, metadb_handle_list_cref p_data, pfc::string_base& p_out, unsigned& p_displayflags, const GUID& p_caller) {
			PFC_ASSERT(p_index >= 0 && p_index < get_num_items());
			get_item_name(p_index, p_out);
			return true;
		}

		void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) {
			switch(p_index) {
				case cmd_clear_skipcount:
					clearRecords(p_data);
					break;
				case cmd_clear_skiptimes:
					clearTimestamps(p_data);
					break;
				default:
					uBugCheck();
			}
		}

		GUID get_item_guid(unsigned p_index) {
			// GUIDs identifying the context menu items.
			// {204B4CB0-C0BA-4732-8826-888EA7CD3CE8}
			static const GUID guid_clear_skipcount = { 0x204b4cb0, 0xc0ba, 0x4732, { 0x88, 0x26, 0x88, 0x8e, 0xa7, 0xcd, 0x3c, 0xe8 } };
			// {485F7FE9-43F8-4D35-AE41-4CA98377E1CF}
			static const GUID guid_clear_skiptimes = { 0x485f7fe9, 0x43f8, 0x4d35, { 0xae, 0x41, 0x4c, 0xa9, 0x83, 0x77, 0xe1, 0xcf } };

			switch(p_index) {
				case cmd_clear_skipcount:
					return guid_clear_skipcount;
					break;
				case cmd_clear_skiptimes:
					return guid_clear_skiptimes;
					break;
				default:
					uBugCheck(); // Failsafe
			}

		}

		bool get_item_description(unsigned p_index, pfc::string_base& p_out) {
			switch(p_index) {
				case cmd_clear_skipcount:
					p_out = "Clears all recorded skips for selected tracks.";
					return true;
				case cmd_clear_skiptimes:
					p_out = "Clears all timestamps for skips on seelcted tracks.";
					return true;
				default:
					uBugCheck(); // Failsafe
			}
		}
	};

	static contextmenu_item_factory_t<my_item> g_my_item_factory;
}
