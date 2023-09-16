#pragma once
#include "stdafx.h"

#ifdef _WIN32
#include "resource.h"
#include <helpers/atl-misc.h>
#include <helpers/DarkMode.h>
#endif // _WIN32

enum {
	TIME = 0,
	PERCENT = 1,
	TIMEANDPERCENT = 2,
};

namespace foo_skipcount {

	// Config
	extern cfg_bool cfg_countNext,
		cfg_countRandom,
		cfg_countPrevious,
		cfg_countFromPause,
		cfg_countFromStop,
		cfg_lastSkip,
		cfg_skipTimes,
		cfg_skipProtectionPrevious;

	extern cfg_uint cfg_condition,
		cfg_percent,
		cfg_time;

	// Defaults
	static const bool default_cfg_countNext = true,
		default_cfg_countRandom = true,
		default_cfg_countPrevious = false,
		default_cfg_countFromPause = true,
		default_cfg_countFromStop = false,
		default_cfg_lastSkip = false,
		default_cfg_skipTimes = false,
		default_cfg_skipProtectionPrevious = true;

	static const t_uint default_cfg_condition = 0,
		default_cfg_percent = 5,
		default_cfg_time = 5;


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
		static GUID guid_cfg_percent;
		static GUID guid_cfg_time;
		static GUID guid_cfg_countFromPause;
		static GUID guid_cfg_countFromStop;
		static GUID guid_cfg_lastSkip;
		static GUID guid_cfg_skipTimes;
		static GUID guid_cfg_skipProtectionPrevious;

		// WTL message map
		BEGIN_MSG_MAP_EX(preferences)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_COUNT_NEXT, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_RANDOM, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_PREVIOUS, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_CONDITION, CBN_SELCHANGE, OnSelectionChange)
			COMMAND_HANDLER_EX(IDC_PERCENT, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_PAUSE, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_STOP, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_LAST_SKIPPED, BN_CLICKED, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_TIMES, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_SKIP_PROTECTION_PREVIOUS, BN_CLICKED, OnEditChange)
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
		const char* get_name() override;

		// {B6F5B09F-4926-449E-A30B-121DF1D7E9E4}
		GUID get_guid() override {
			return GUID{ 0xb6f5b09f, 0x4926, 0x449e, { 0xa3, 0x0b, 0x12, 0x1d, 0xf1, 0xd7, 0xe9, 0xe4 } };
		}

		GUID get_parent_guid() override {
			return guid_tools;
		}
	};

	static preferences_page_factory_t<my_preferences_page> g_my_preferences_page_factory;
#endif // _WIN32

}