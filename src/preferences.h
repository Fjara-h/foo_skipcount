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
	extern cfg_bool cfg_countNext, cfg_countRandom, cfg_countPrevious, cfg_countFromPause, cfg_countFromStop;
	extern cfg_uint cfg_condition, cfg_percent, cfg_time;

	// Defaults
	static const bool default_cfg_countNext = true, default_cfg_countRandom = true, default_cfg_countPrevious = false, default_cfg_countFromPause = true, default_cfg_countFromStop = false;
	static const int default_cfg_condition = 0, default_cfg_percent = 5, default_cfg_time = 5;

#ifdef _WIN32
	class preferences : public CDialogImpl<preferences>, public preferences_page_instance {
	public:
		// Constructor
		preferences(preferences_page_callback::ptr callback) : m_callback(callback) {}

		// No Destructor: Host ensures that this dialog is destroyed first, then the last reference to this preferences_page_instance object is released, causing this object to be deleted.

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
		END_MSG_MAP()

	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void preferences::CreateTooltip(CToolTipCtrl, CWindow, int, LPCTSTR, LPCTSTR);
		void preferences::OnEditChange(UINT, int, CWindow);
		void preferences::OnSelectionChange(UINT, int, CWindow);
		bool preferences::HasChanged();
		void preferences::ConformToBounds(cfg_uint&, unsigned int, unsigned int, unsigned int, unsigned int, int);
		void preferences::OnChanged();
		
		const preferences_page_callback::ptr m_callback;
		CToolTipCtrl tooltips[8];

		// Dark mode hooks object, must be a member of dialog class.
		fb2k::CDarkModeHooks m_dark;
	};

	class preferences_page_myimpl : public preferences_page_impl<preferences> {
		// preferences_page_impl<> helper deals with instantiation of our dialog; inherits from preferences_page_v3.
	public:
		const char* get_name() override;
		// {B6F5B09F-4926-449E-A30B-121DF1D7E9E4}
		GUID get_guid() override { return GUID{ 0xb6f5b09f, 0x4926, 0x449e, { 0xa3, 0x0b, 0x12, 0x1d, 0xf1, 0xd7, 0xe9, 0xe4 } }; }
		GUID get_parent_guid() override { return guid_tools; }
	};

	static preferences_page_factory_t<preferences_page_myimpl> g_preferences_page_myimpl_factory;
#endif // _WIN32

}