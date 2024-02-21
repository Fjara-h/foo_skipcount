#ifndef SKIPCOUNT_CONTEXTMENU_H
#define SKIPCOUNT_CONTEXTMENU_H

#include "globals.h"

namespace foo_skipcount {
	enum ContextMenuClearEnum {
		CMD_CLEAR_SKIPCOUNT = 0,
		CMD_CLEAR_LATESTSKIP = 1,
		CMD_CLEAR_ALLBUTLATESTSKIP = 2,
		CMD_CLEAR_OLDESTSKIP = 3,
		CMD_CLEAR_ALLSKIPS = 4,
		CMD_CLEAR_TOTAL
	};
	enum ContextMenuDataEnum {
		CMD_DATA_COPY = 0,
		CMD_DATA_PASTE = 1,
		CMD_TAG_WRITE = 2,
		CMD_TAG_IMPORT = 3,
		CMD_TAG_EXPORT = 4,
		CMD_DATA_TOTAL
	};

	// Context menu groups
	// {3AEB1668-9FD0-450F-856F-EB6C1C240187}
	static const GUID guid_context_menu = { 0x3aeb1668, 0x9fd0, 0x450f, { 0x85, 0x6f, 0xeb, 0x6c, 0x1c, 0x24, 0x01, 0x87 } };
	// {E7985AA6-47F7-477A-A9F8-31DFDC0210F4}
	static const GUID guid_context_clear = { 0xe7985aa6, 0x47f7, 0x477a, { 0xa9, 0xf8, 0x31, 0xdf, 0xdc, 0x02, 0x10, 0xf4 } };
	// {B66C66E0-554C-4804-B022-910F4EE0CDDC}
	static const GUID guid_context_data = { 0xb66c66e0, 0x554c, 0x4804, { 0xb0, 0x22, 0x91, 0x0f, 0x4e, 0xe0, 0xcd, 0xdc } };

	// Context menu clear commands
	// {204B4CB0-C0BA-4732-8826-888EA7CD3CE8}
	static const GUID guid_clear_skipcount = { 0x204b4cb0, 0xc0ba, 0x4732, { 0x88, 0x26, 0x88, 0x8e, 0xa7, 0xcd, 0x3c, 0xe8 } };
	// {829B7BEF-3DA3-42F8-883F-C3C32E78D980}
	static const GUID guid_clear_latestskip = { 0x829b7bef, 0x3da3, 0x42f8, { 0x88, 0x3f, 0xc3, 0xc3, 0x2e, 0x78, 0xd9, 0x80 } };
	// {4FEFE5E1-C64C-45E2-B0A5-2F9B5084CE4D}
	static const GUID guid_clear_allbutlatestskip = { 0x4fefe5e1, 0xc64c, 0x45e2, { 0xb0, 0xa5, 0x2f, 0x9b, 0x50, 0x84, 0xce, 0x4d } };
	// {1510F998-F11B-48F7-9459-0618E7E9F247}
	static const GUID guid_clear_oldestskip = { 0x1510f998, 0xf11b, 0x48f7, { 0x94, 0x59, 0x06, 0x18, 0xe7, 0xe9, 0xf2, 0x47 } };
	// {485F7FE9-43F8-4D35-AE41-4CA98377E1CF}
	static const GUID guid_clear_skiptimes = { 0x485f7fe9, 0x43f8, 0x4d35, { 0xae, 0x41, 0x4c, 0xa9, 0x83, 0x77, 0xe1, 0xcf } };

	// Context menu data commands
	// {7EA54CA7-D697-4420-9806-CDCA0F9F487A}
	static const GUID guid_data_copy = { 0x7EA54CA7, 0xD697, 0x4420, { 0x98, 0x06, 0xCD, 0xCA, 0x0F, 0x9F, 0x48, 0x7A } };
	// {51367C22-AA7E-422F-9381-EC98D577CF90}
	static const GUID guid_data_paste = { 0x51367C22, 0xAA7E, 0x422F, { 0x93, 0x81, 0xEC, 0x98, 0xD5, 0x77, 0xCF, 0x90 } };
	// {39859899-DBDF-42B9-9E6C-FC0FBCE42C28}
	static const GUID guid_tag_write = { 0x39859899, 0xDBDF, 0x42B9, { 0x9E, 0x6C, 0xFC, 0x0F, 0xBC, 0xE4, 0x2C, 0x28 } };
	// {25FAAD5E-3B9C-4135-B147-94EFF93908A3}
	static const GUID guid_tag_import = { 0x25FAAD5E, 0x3B9C, 0x4135, { 0xB1, 0x47, 0x94, 0xEF, 0xF9, 0x39, 0x08, 0xA3 } };
	// {B41A8273-9B21-4211-BD1D-A5211B739A70}
	static const GUID guid_data_export = { 0xB41A8273, 0x9B21, 0x4211, {0xBD, 0x1D, 0xA5, 0x21, 0x1B, 0x73, 0x9A, 0x70 } };

	static contextmenu_group_popup_factory g_my_group(guid_context_menu, contextmenu_groups::root, "Skip Count", 0);
	static contextmenu_group_factory g_my_clear_group(guid_context_clear, guid_context_menu, 1);
	static contextmenu_group_factory g_my_data_group(guid_context_data, guid_context_menu, 2);

	// Clear commands
	class my_contextmenu_clear_cmds : public contextmenu_item_simple {
	public:
		inline GUID get_parent() {
			return guid_context_clear;
		}

		inline unsigned get_num_items() {
			return CMD_CLEAR_TOTAL;
		}
		void get_item_name(unsigned int, pfc::string_base&);
		bool context_get_display(unsigned int, metadb_handle_list_cref, pfc::string_base&, unsigned int&, const GUID&);
		void context_command(unsigned int, metadb_handle_list_cref, const GUID&);
		GUID get_item_guid(unsigned int);
		bool get_item_description(unsigned, pfc::string_base&);

		void contextClear(metadb_handle_list_cref, int);
	};
	static contextmenu_item_factory_t<my_contextmenu_clear_cmds> g_my_contextmenu_clearcmds_factory;

	// Data commands
	class my_contextmenu_data_cmds : public contextmenu_item_simple {
	public:
		inline GUID get_parent() {
			return guid_context_data;
		}

		inline unsigned get_num_items() {
			return CMD_DATA_TOTAL;
		}
		void get_item_name(unsigned int, pfc::string_base&);
		bool context_get_display(unsigned int, metadb_handle_list_cref, pfc::string_base&, unsigned int&, const GUID&);
		void context_command(unsigned int, metadb_handle_list_cref, const GUID&);
		GUID get_item_guid(unsigned int);
		bool get_item_description(unsigned, pfc::string_base&);

		void contextCopySkipData(metadb_handle_list_cref);
		void contextPasteSkipData(metadb_handle_list_cref);
		void contextWriteSkipTag(metadb_handle_list_cref);
		void contextImportSkipTag(metadb_handle_list_cref);
		void contextExportXMLSkipData(metadb_handle_list_cref);
	private:
		metadb_handle_list clipboardData;
	};
	static contextmenu_item_factory_t<my_contextmenu_data_cmds> g_my_contextmenu_datacmds_factory;
} // namespace foo_skipcount
#endif //SKIPCOUNT_CONTEXTMENU_H
