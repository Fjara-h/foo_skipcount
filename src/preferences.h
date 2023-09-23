#pragma once
#include "stdafx.h"

#ifdef _WIN32
#include "resource.h"
#include <helpers/atl-misc.h>
#include <helpers/DarkMode.h>
#endif // _WIN32

// Conditions
enum logConditionEnum {
	TIME = 0,
	PERCENT = 1,
	TIMEANDPERCENT = 2,
};

// Skip times logging
enum logTypeEnum {
	NOLOG = 0,
	LOGLAST = 1,
	LOGALL = 2,
};

namespace foo_skipcount {

	// GUIDs for settings and their storage within the component's configuration file.
	// {2DCFE159-0683-4AD5-9F10-D612577BA137}
	static constexpr GUID guid_cfg_countNext = { 0x2dcfe159, 0x0683, 0x4ad5, { 0x9f, 0x10, 0xd6, 0x12, 0x57, 0x7b, 0xa1, 0x37 } };
	// {30D15AC1-C56B-40DB-88B2-9B0C0C1C8B17}
	static constexpr GUID guid_cfg_countRandom = { 0x30d15ac1, 0xc56b, 0x40db, { 0x88, 0xb2, 0x9b, 0x0c, 0x0c, 0x1c, 0x8b, 0x17 } };
	// {1AA63FCC-223A-4732-B778-1F94A0C0B91F}
	static constexpr GUID guid_cfg_countPrevious = { 0x1aa63fcc, 0x223a, 0x4732, { 0xb7, 0x78, 0x1f, 0x94, 0xa0, 0xc0, 0xb9, 0x1f } };
	// {74ACC430-FF12-4BC8-846E-2882916FFF90}
	static constexpr GUID guid_cfg_condition = { 0x74acc430, 0xff12, 0x4bc8, { 0x84, 0x6e, 0x28, 0x82, 0x91, 0x6f, 0xff, 0x90 } };
	// {27D73FD1-D9B6-403A-B705-0B00D6952229}
	static constexpr GUID guid_cfg_percent = { 0x27d73fd1, 0xd9b6, 0x403a, { 0xb7, 0x05, 0x0b, 0x00, 0xd6, 0x95, 0x22, 0x29 } };
	// {F6AA9C8A-616F-410C-AE18-F848C9F7DF87}
	static constexpr GUID guid_cfg_time = { 0xf6aa9c8a, 0x616f, 0x410c, { 0xae, 0x18, 0xf8, 0x48, 0xc9, 0xf7, 0xdf, 0x87 } };
	// {C2A71EB9-E63D-433B-84CF-0BE30DFD47B0}
	static constexpr GUID guid_cfg_countFromPause = { 0xc2a71eb9, 0xe63d, 0x433b, { 0x84, 0xcf, 0x0b, 0xe3, 0x0d, 0xfd, 0x47, 0xb0 } };
	// {9C746A7C-24DF-450A-BEBF-F63F14C6BF57}
	static constexpr GUID guid_cfg_countFromStop = { 0x9c746a7c, 0x24df, 0x450a, { 0xbe, 0xbf, 0xf6, 0x3f, 0x14, 0xc6, 0xbf, 0x57 } };
	// {99F52995-6A9D-48C8-8C10-EFC0DCCA0C09}
	static constexpr GUID guid_cfg_logSkipTimes = { 0x99f52995, 0x6a9d, 0x48c8, { 0x8c, 0x10, 0xef, 0xc0, 0xdc, 0xca, 0x0c, 0x09 } };
	// {D74AF3CE-94BD-4964-8386-A11EA00DA16E}
	static constexpr GUID guid_cfg_skipProtectionNext = { 0xd74af3ce, 0x94bd, 0x4964, { 0x83, 0x86, 0xa1, 0x1e, 0xa0, 0x0d, 0xa1, 0x6e } };
	// {60CC830E-704C-498B-A2FC-77DEADCE3E21}
	static constexpr GUID guid_cfg_skipProtectionNextTime = { 0x60cc830e, 0x704c, 0x498b, { 0xa2, 0xfc, 0x77, 0xde, 0xad, 0xce, 0x3e, 0x21 } };
	// {50CEEC36-2BA7-4349-B3B4-E8A96485E4AC}
	static constexpr GUID guid_cfg_skipProtectionRandom = { 0x50ceec36, 0x2ba7, 0x4349, { 0xb3, 0xb4, 0xe8, 0xa9, 0x64, 0x85, 0xe4, 0xac } };
	// {06652612-AF1C-4667-B7ED-4A914FECCAA2}
	static constexpr GUID guid_cfg_skipProtectionRandomTime = { 0x06652612, 0xaf1c, 0x4667, { 0xb7, 0xed, 0x4a, 0x91, 0x4f, 0xec, 0xca, 0xa2 } };
	// {E958AC28-1626-422F-B0F0-356223DC7EDB}
	static constexpr GUID guid_cfg_skipProtectionPrevious = { 0xe958ac28, 0x1626, 0x422f, { 0xb0, 0xf0, 0x35, 0x62, 0x23, 0xdc, 0x7e, 0xdb } };
	// {8B16D08D-9169-4F1C-9102-E058DA09185C}
	static constexpr GUID guid_cfg_skipProtectionPreviousTime = { 0x8b16d08d, 0x9169, 0x4f1c, { 0x91, 0x02, 0xe0, 0x58, 0xda, 0x09, 0x18, 0x5c } };

	// Config
	extern cfg_bool cfg_countNext,
		cfg_countRandom,
		cfg_countPrevious,
		cfg_countFromPause,
		cfg_countFromStop,
		cfg_skipProtectionNext,
		cfg_skipProtectionRandom,
		cfg_skipProtectionPrevious;

	extern cfg_uint cfg_time,
		cfg_percent,
		cfg_condition,
		cfg_logSkipTimes,
		cfg_skipProtectionNextTime,
		cfg_skipProtectionRandomTime,
		cfg_skipProtectionPreviousTime;

	// Defaults
	static const bool default_cfg_countNext = true,
		default_cfg_countRandom = true,
		default_cfg_countPrevious = false,
		default_cfg_countFromPause = true,
		default_cfg_countFromStop = false,
		default_cfg_skipProtectionNext = false,
		default_cfg_skipProtectionRandom = false,
		default_cfg_skipProtectionPrevious = true;

	static const t_uint default_cfg_percent = 5,
		default_cfg_time = 5,
		default_cfg_condition = 0,
		default_cfg_logSkipTimes = 0,
		default_cfg_skipProtectionNextTime = 1,
		default_cfg_skipProtectionRandomTime = 1,
		default_cfg_skipProtectionPreviousTime = 1;

#ifdef _WIN32
	class my_preferences : public CDialogImpl<my_preferences>, public preferences_page_instance {
	public:
		// Constructor
		my_preferences(preferences_page_callback::ptr callback) : m_callback(callback) { }

		// No Destructor: Host ensures that the dialog is destroyed first then the last reference to this object is released, thus deleted.

		static int const IDD = IDD_PREFERENCES;

		// preferences_page_instance methods
		t_uint32 get_state() override;
		void apply() override;
		void reset() override;

		// GUIDs for settings and their storage within the component's configuration file.
		static GUID guid_cfg_countNext;
		static GUID guid_cfg_countRandom;
		static GUID guid_cfg_countPrevious;
		static GUID guid_cfg_condition;
		static GUID guid_cfg_time;
		static GUID guid_cfg_percent;
		static GUID guid_cfg_countFromPause;
		static GUID guid_cfg_countFromStop;
		static GUID guid_cfg_logSkipTimes;
		static GUID guid_cfg_skipProtectionNext;
		static GUID guid_cfg_skipProtectionNextTime;
		static GUID guid_cfg_skipProtectionRandom;
		static GUID guid_cfg_skipProtectionRandomTime;
		static GUID guid_cfg_skipProtectionPrevious;
		static GUID guid_cfg_skipProtectionPreviousTime;

		// WTL message map
		BEGIN_MSG_MAP_EX(preferences)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_COUNT_NEXT, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_RANDOM, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_PREVIOUS, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_CONDITION, CBN_SELCHANGE, OnSelectionChange)
			COMMAND_HANDLER_EX(IDC_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_PERCENT, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_PAUSE, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_STOP, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_LOG_SKIP_TIMES, CBN_SELCHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_NEXT, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_NEXT_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_RANDOM, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_RANDOM_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_PREVIOUS, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_PREVIOUS_TIME, EN_CHANGE, OnEditChange)
		END_MSG_MAP()

	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void my_preferences::CreateTooltip(CToolTipCtrl, CWindow, int, LPCTSTR, LPCTSTR);
		void my_preferences::OnEditChange(UINT, int, CWindow);
		void my_preferences::OnSelectionChange(UINT, int, CWindow);
		bool my_preferences::HasChanged();
		t_uint32 my_preferences::GetIntConformedToBounds(int, t_int32, t_int32, t_int32, t_int32);
		void my_preferences::OnChanged();

		const preferences_page_callback::ptr m_callback;
		CToolTipCtrl tooltips[12];

		// Dark mode hooks object, must be a member of dialog class.
		fb2k::CDarkModeHooks m_dark;
	};

	// preferences_page_impl<> helper deals with instantiation of dialog window; inherits from preferences_page_v3.
	class my_preferences_page : public preferences_page_impl<my_preferences> {

	public:
		inline const char* get_name() override;

		// {B6F5B09F-4926-449E-A30B-121DF1D7E9E4}
		inline GUID get_guid() override {
			return GUID{ 0xb6f5b09f, 0x4926, 0x449e, { 0xa3, 0x0b, 0x12, 0x1d, 0xf1, 0xd7, 0xe9, 0xe4 } };
		}

		inline GUID get_parent_guid() override {
			return guid_tools;
		}
	};

	static preferences_page_factory_t<my_preferences_page> g_my_preferences_page_factory;
#endif // _WIN32

} // namespace foo_skipcount