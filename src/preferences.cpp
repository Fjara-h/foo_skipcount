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
	// {800D9D04-CB2E-4D30-9094-522CCF54F5FE}
	static constexpr GUID guid_cfg_lastSkip = { 0x800d9d04, 0xcb2e, 0x4d30, { 0x90, 0x94, 0x52, 0x2c, 0xcf, 0x54, 0xf5, 0xfe } };
	// {99F52995-6A9D-48C8-8C10-EFC0DCCA0C09}
	static constexpr GUID guid_cfg_skipTimes = { 0x99f52995, 0x6a9d, 0x48c8, { 0x8c, 0x10, 0xef, 0xc0, 0xdc, 0xca, 0x0c, 0x09 } };
	// {FC15F7CD-2DB0-415B-8E8C-3C15366BE028}
	static constexpr GUID guid_cfg_skipFieldPattern = { 0xfc15f7cd, 0x2db0, 0x415b, { 0x8e, 0x8c, 0x3c, 0x15, 0x36, 0x6b, 0xe0, 0x28 } };
	// {E958AC28-1626-422F-B0F0-356223DC7EDB}
	static constexpr GUID guid_cfg_skipProtectionPrevious = { 0xe958ac28, 0x1626, 0x422f, { 0xb0, 0xf0, 0x35, 0x62, 0x23, 0xdc, 0x7e, 0xdb } };

	// Config
	cfg_bool cfg_countNext(guid_cfg_countNext, default_cfg_countNext),
		cfg_countRandom(guid_cfg_countRandom, default_cfg_countRandom),
		cfg_countPrevious(guid_cfg_countPrevious, default_cfg_countPrevious),
		cfg_countFromPause(guid_cfg_countFromPause, default_cfg_countFromPause),
		cfg_countFromStop(guid_cfg_countFromStop, default_cfg_countFromStop),
		cfg_skipTimes(guid_cfg_skipTimes, default_cfg_skipTimes),
		cfg_lastSkip(guid_cfg_lastSkip, default_cfg_lastSkip),
		cfg_skipProtectionPrevious(guid_cfg_skipProtectionPrevious, default_cfg_skipProtectionPrevious);

	cfg_uint cfg_condition(guid_cfg_condition, default_cfg_condition),
		cfg_percent(guid_cfg_percent, default_cfg_percent),
		cfg_time(guid_cfg_time, default_cfg_time);

	cfg_string cfg_skipFieldPattern(guid_cfg_skipFieldPattern, default_cfg_skipFieldPattern);

#ifdef _WIN32
	BOOL my_preferences::OnInitDialog(CWindow, LPARAM) {
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
		uSetDlgItemText(m_hWnd, IDC_SKIP_FIELD_PATTERN, cfg_skipFieldPattern);
		SendMessageW(GetDlgItem(IDC_LAST_SKIPPED), BM_SETCHECK, cfg_lastSkip, NULL);
		SendMessageW(GetDlgItem(IDC_SKIP_TIMES), BM_SETCHECK, cfg_skipTimes, NULL);
		SendMessageW(GetDlgItem(IDC_SKIP_PROTECTION_PREVIOUS), BM_SETCHECK, cfg_skipProtectionPrevious, NULL);

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
		CreateTooltip(tooltips[6], m_hWnd, IDC_COUNT_FROM_PAUSE,
			L"Count a skip after a pause",
			L"\nWhen enabled and playback is paused, skips coming out of the pause can still count towards "
			L" the statistic if the conditions are met."
		);
		CreateTooltip(tooltips[7], m_hWnd, IDC_COUNT_FROM_STOP,
			L"Count a skip after a stop",
			L"\nWhen enabled and the user has stopped playback, skip-actions coming out of stopped playback "
			L"can still count towards the statistic if the conditions are met."
		);
		CreateTooltip(tooltips[8], m_hWnd, IDC_SKIP_FIELD_PATTERN,
			L"Set the custom column pattern",
			L"\n The default is skip_count to be used as %skip_count%."
		);
		CreateTooltip(tooltips[9], m_hWnd, IDC_LAST_SKIPPED,
			L"Count a skip after a stop",
			L"\nWhen enabled, logs the most recent skip date as 'YYYY-MM-DD HH:MM:SS'"
		);
		CreateTooltip(tooltips[10], m_hWnd, IDC_SKIP_TIMES,
			L"Log timestamps of each skip",
			L"\nWhen enabled, raw and JS timestamps will be logged for each skip, visible with "
			L"%skip_times_raw% and %skip_times_js%"
		);
		CreateTooltip(tooltips[11], m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS,
			L"Disable skip counting for 1 second after using Previous Song",
			L"\nWhen enabled, using a control during the condition that would normally count as a skip "
			L"after pressing Previous Song will not count as a skip. This is for accidental pressing or "
			L" if checking which song previously played without counting as a real skip."
		);

		return FALSE;
	}

	void my_preferences::CreateTooltip(CToolTipCtrl tooltip, CWindow hWnd, int parent, LPCTSTR title, LPCTSTR body) {
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

	void my_preferences::OnEditChange(UINT, int, CWindow) {
		OnChanged();
	}

	void my_preferences::OnSelectionChange(UINT, int, CWindow) {
		OnChanged();
	}

	t_uint32 my_preferences::get_state() {
		// IMPORTANT: Always return dark_mode_supported
		t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;
		if (HasChanged()) state |= preferences_state::changed;
		return state;
	}

	void my_preferences::reset() {
		SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_SETCHECK, default_cfg_countNext, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_SETCHECK, default_cfg_countRandom, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_SETCHECK, default_cfg_countPrevious, NULL);
		SendMessageW(GetDlgItem(IDC_CONDITION), CB_SETCURSEL, default_cfg_condition, NULL);
		SetDlgItemInt(IDC_PERCENT, default_cfg_percent, FALSE);
		SetDlgItemInt(IDC_TIME, default_cfg_time, FALSE);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_SETCHECK, default_cfg_countFromPause, NULL);
		SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_SETCHECK, default_cfg_countFromStop, NULL);
		uSetDlgItemText(m_hWnd, IDC_SKIP_FIELD_PATTERN, default_cfg_skipFieldPattern);
		SendMessageW(GetDlgItem(IDC_LAST_SKIPPED), BM_SETCHECK, default_cfg_lastSkip, NULL);
		SendMessageW(GetDlgItem(IDC_SKIP_TIMES), BM_SETCHECK, default_cfg_skipTimes, NULL);
		SendMessageW(GetDlgItem(IDC_SKIP_PROTECTION_PREVIOUS), BM_SETCHECK, default_cfg_skipProtectionPrevious, NULL);
		OnChanged();
	}

	// During the applies, check that percent and time are within the correct bounds. 0-100 and 0 - 2000000000
	void my_preferences::apply() {
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
		pfc::string8_fast text;
		uGetDlgItemText(m_hWnd, IDC_SKIP_FIELD_PATTERN, text);
		if(text.get_length() == 0) {
			uSetDlgItemText(m_hWnd, IDC_SKIP_FIELD_PATTERN, default_cfg_skipFieldPattern);
		}
		cfg_skipFieldPattern = text.get_ptr();
		cfg_lastSkip = SendMessageW(GetDlgItem(IDC_LAST_SKIPPED), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_skipTimes = SendMessageW(GetDlgItem(IDC_SKIP_TIMES), BM_GETCHECK, NULL, NULL) == BST_CHECKED;
		cfg_skipProtectionPrevious = SendMessageW(GetDlgItem(IDC_SKIP_PROTECTION_PREVIOUS), BM_GETCHECK, NULL, NULL) == BST_CHECKED;

		refreshGlobal();
		OnChanged(); // Dialog content has not changed but the flags have - shown values now match the settings so the apply button can be disabled
	}

	// ES_NUMBER only allows valid number characters to be typed or pasted.
	// This enforces number bounds.
	void my_preferences::ConformToBounds(cfg_uint &input, t_uint min, t_uint minDefault, t_uint max, t_uint maxDefault, int ID) {
		if(input.get_value() < min) {
			input = minDefault;
			SetDlgItemInt(ID, input, FALSE);
		}
		else if(input.get_value() > max) {
			input = maxDefault;
			SetDlgItemInt(ID, input, FALSE);
		}
	}

	// Is there a less horrendous way to do this?
	bool my_preferences::HasChanged() {
		pfc::string8 tempPattern;
		uGetDlgItemText(GetDlgItem(IDC_SKIP_FIELD_PATTERN), IDC_SKIP_FIELD_PATTERN, tempPattern);
		// Returns whether the settings are different from the current configuration - (De)activating the apply button
		return (SendMessageW(GetDlgItem(IDC_COUNT_NEXT), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countNext ||
			(SendMessageW(GetDlgItem(IDC_COUNT_RANDOM), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countRandom ||
			(SendMessageW(GetDlgItem(IDC_COUNT_PREVIOUS), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countPrevious ||
			SendMessageW(GetDlgItem(IDC_CONDITION), CB_GETCURSEL, NULL, NULL) != cfg_condition ||
			GetDlgItemInt(IDC_PERCENT, NULL, FALSE) != cfg_percent ||
			GetDlgItemInt(IDC_TIME, NULL, FALSE) != cfg_time ||
			(SendMessageW(GetDlgItem(IDC_COUNT_FROM_PAUSE), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countFromPause ||
			(SendMessageW(GetDlgItem(IDC_COUNT_FROM_STOP), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_countFromStop ||
			tempPattern != cfg_skipFieldPattern ||
			(SendMessageW(GetDlgItem(IDC_SKIP_TIMES), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_skipTimes ||
			(SendMessageW(GetDlgItem(IDC_LAST_SKIPPED), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_lastSkip ||
			(SendMessageW(GetDlgItem(IDC_SKIP_PROTECTION_PREVIOUS), BM_GETCHECK, NULL, NULL) == BST_CHECKED) != cfg_skipProtectionPrevious;
	}

	void my_preferences::OnChanged() {
		// Update state to 'changed' to enable/disable the apply button appropriately.
		m_callback->on_state_changed();
	}

	const char* my_preferences_page::get_name() {
		return "Skip Count";
	}
#endif // _WIN32
}
