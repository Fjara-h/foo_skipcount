#include "preferences.h"
#include "globals.h"
#include "SkipCount.h"

#include <SDK/cfg_var.h>
#include <pfc/win-objects.cpp>
#include <string>


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

	// Config
	cfg_bool cfg_countNext(guid_cfg_countNext, default_cfg_countNext),
		cfg_countRandom(guid_cfg_countRandom, default_cfg_countRandom),
		cfg_countPrevious(guid_cfg_countPrevious, default_cfg_countPrevious),
		cfg_countFromPause(guid_cfg_countFromPause, default_cfg_countFromPause),
		cfg_countFromStop(guid_cfg_countFromStop, default_cfg_countFromStop);

	cfg_uint cfg_condition(guid_cfg_condition, default_cfg_condition),
		cfg_percent(guid_cfg_percent, default_cfg_percent),
		cfg_time(guid_cfg_time, default_cfg_time);

#ifdef _WIN32
	BOOL preferences::OnInitDialog(CWindow, LPARAM) {
		// Enable dark mode
		m_dark.AddDialogWithControls(*this);

		SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_SETCHECK, cfg_countNext, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_SETCHECK, cfg_countRandom, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_SETCHECK, cfg_countPrevious, NULL);
		{
			static const WCHAR* CondtionType[] = {
				L"Time",
				L"Percent",
				L"Time and Percent",
			};

			{
				auto conditionDropList = (CComboBox)GetDlgItem(IDC_CONDITION);

				for(size_t i = 0; i < _countof(CondtionType); i++) {
					conditionDropList.AddString(CondtionType[i]);
				}

				conditionDropList.SetCurSel(cfg_condition);
			}
		}
		SetDlgItemInt(IDC_PERCENT, (UINT)cfg_percent, FALSE);
		SetDlgItemInt(IDC_TIME, (UINT)cfg_time, FALSE);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_SETCHECK, cfg_countFromPause, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_SETCHECK, cfg_countFromStop, NULL);

		CreateTooltip(tooltips[0], m_hWnd, IDC_COUNT_NEXT,
			L"Add to skip count after pressing next song",
			L"\nWhen enabled, the component will increment the song's skip count statistic "
			L"when pressing the next song button, shortcut, or using the next song media key "
			L"if the song has not reach the selected time and/or percentage of song played."
		);
		CreateTooltip(tooltips[1], m_hWnd, IDC_COUNT_RANDOM,
			L"Add to skip count after pressing random song",
			L"\nWhen enabled, the component will increment the song's skip count statistic "
			L"when pressing the random song button, shortcut, or using the random song media key "
			L"if the song has not reach the selected time and/or percentage of song played."
		);
		CreateTooltip(tooltips[2], m_hWnd, IDC_COUNT_PREVIOUS,
			L"Add to skip count after pressing previous song",
			L"\nWhen enabled, the component will increment the song's skip count statistic "
			L"when pressing the previous song button, shortcut, or using the previous song media key "
			L"if the song has not reach the selected time and/or percentage of song played."
		);
		CreateTooltip(tooltips[3], m_hWnd, IDC_CONDITION,
			L"Allow the skip to count until the defined condition",
			L"\nSelecting 'Time' will cause any skip to only count before that duration of the "
			L" song has played. 'Percentage' only counts skips before that percent of the song "
			L"has played. 'Time and Percent' means both conditions above must be met for a skip "
			L" to no longer count."
		);
		CreateTooltip(tooltips[4], m_hWnd, IDC_PERCENT,
			L"Percentage of song to count skip",
			L"\nEnter a number from 0 to 100. After that percentage in the song, skips no longer count."
		);
		CreateTooltip(tooltips[5], m_hWnd, IDC_TIME,
			L"Duration of song to count skip",
			L"\nEnter a number from 0 to 2 billion. After that duration in the song, skips no longer count."
		);
		CreateTooltip(tooltips[5], m_hWnd, IDC_COUNT_FROM_PAUSE,
			L"Count a skip after a pause",
			L"\nWhen enabled and playback is paused, skips coming out of the pause can still count towards "
			L" the statistic if the conditions are met."
		);
		CreateTooltip(tooltips[5], m_hWnd, IDC_COUNT_FROM_STOP,
			L"Count a skip after a stop",
			L"\nWhen enabled and the user has stopped playback, skip-actions coming out of stopped playback "
			L"can still count towards the statistic if the conditions are met."
		);

		return FALSE;
	}

	void preferences::CreateTooltip(CToolTipCtrl tooltip, CWindow hWnd, int parent, LPCTSTR title, LPCTSTR body) {
		if (tooltip.Create(hWnd, nullptr, nullptr, TTS_NOPREFIX | TTS_BALLOON)) {
			CToolInfo toolInfo(TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP,	GetDlgItem(parent), 0, nullptr, nullptr);
			HICON icon = 0;
			tooltip.AddTool(&toolInfo);

			tooltip.SetTitle(icon, title);
			tooltip.UpdateTipText(body, GetDlgItem(parent));
			tooltip.SetMaxTipWidth(350);
			tooltip.SetDelayTime(TTDT_INITIAL, 350);
			tooltip.SetDelayTime(TTDT_AUTOPOP, 32000);
			tooltip.Activate(TRUE);
		}
	}


	void preferences::OnEditChange(UINT, int, CWindow) {
		OnChanged();
	}

	void preferences::OnSelectionChange(UINT, int, CWindow) {
		OnChanged();
	}

	t_uint32 preferences::get_state() {
		// IMPORTANT: Always return dark_mode_supported
		t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;
		if (HasChanged()) state |= preferences_state::changed;
		return state;
	}

	void preferences::reset() {
		SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_SETCHECK, default_cfg_countNext, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_SETCHECK, default_cfg_countRandom, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_SETCHECK, default_cfg_countPrevious, NULL);
		SendMessageW(GetDlgItem(IDC_CONDITION), CB_SETCURSEL, default_cfg_condition, NULL);
		SetDlgItemInt(IDC_PERCENT, default_cfg_percent, FALSE);
		SetDlgItemInt(IDC_TIME, default_cfg_time, FALSE);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_SETCHECK, default_cfg_countFromPause, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_SETCHECK, default_cfg_countFromStop, NULL);
		OnChanged();
	}

	// During the applies, check that percent and time are within the correct bounds. 0-100 and 0 - 2000000000
	void preferences::apply() {
		cfg_countNext = SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_countRandom = SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_countPrevious = SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_condition = SendMessageW(GetDlgItem(IDC_CONDITION), CB_GETCURSEL, NULL, NULL);
		cfg_percent = GetDlgItemInt(IDC_PERCENT, NULL, FALSE);
		ConformToBounds(cfg_percent, 0, default_cfg_percent, 100, 100, IDC_PERCENT);
		cfg_time = GetDlgItemInt(IDC_TIME, NULL, FALSE);
		ConformToBounds(cfg_time, 0, default_cfg_time, 2000000000, 1000, IDC_TIME); // Approx half UINT_MAX
		cfg_countFromPause = SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_countFromStop = SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_GETCHECK, NULL, NULL) == BST_CHECKED;

		RefreshGlobal();
		OnChanged(); // Dialog content has not changed but the flags have - shown values now match the settings so the apply button can be disabled
	}

	// ES_NUMBER only allows valid number characters to be typed or pasted.
	// This enforces number bounds.
	void preferences::ConformToBounds(cfg_uint &input, unsigned int min, unsigned int minDefault, unsigned int max, unsigned int maxDefault, int ID) {
		if(input.get_value() < min) {
			input = minDefault;
			SetDlgItemInt(ID, input, FALSE);
		}
		else if(input.get_value() > max) {
			input = maxDefault;
			SetDlgItemInt(ID, input, FALSE);
		}
	}

	bool preferences::HasChanged() {
		// Returns whether the settings are different from the current configuration - (De)activating the apply button
		return (SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countNext ||
			(SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countRandom ||
			(SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countPrevious ||
			SendMessageW(GetDlgItem(IDC_CONDITION), CB_GETCURSEL, NULL, NULL) != cfg_condition ||
			GetDlgItemInt(IDC_PERCENT, NULL, FALSE) != cfg_percent ||
			GetDlgItemInt(IDC_TIME, NULL, FALSE) != cfg_time ||
			(SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countFromPause || 
			(SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countFromStop;
	}
	void preferences::OnChanged() {
		// Update state to 'changed' to enable/disable the apply button appropriately.
		m_callback->on_state_changed();
	}

	const char* preferences_page_myimpl::get_name() { return "Skip Count"; }
#endif // _WIN32
}
