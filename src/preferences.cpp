#include "preferences.h"

#include <string>

#include <SDK/cfg_var.h>
#include <pfc/win-objects.cpp>
#include <helpers/dropdown_helper.cpp>

#include "globals.h"
#include "SkipCount.h"

namespace foo_skipcount {

	// Config
	cfg_bool cfg_countNext(guid_cfg_countNext, default_cfg_countNext),
		cfg_countRandom(guid_cfg_countRandom, default_cfg_countRandom),
		cfg_countPrevious(guid_cfg_countPrevious, default_cfg_countPrevious),
		cfg_countFromPause(guid_cfg_countFromPause, default_cfg_countFromPause),
		cfg_countFromStop(guid_cfg_countFromStop, default_cfg_countFromStop),
		cfg_skipProtectionNext(guid_cfg_skipProtectionNext, default_cfg_skipProtectionNext),
		cfg_skipProtectionRandom(guid_cfg_skipProtectionRandom, default_cfg_skipProtectionRandom),
		cfg_skipProtectionPrevious(guid_cfg_skipProtectionPrevious, default_cfg_skipProtectionPrevious);

	cfg_uint cfg_time(guid_cfg_time, default_cfg_time),
		cfg_percent(guid_cfg_percent, default_cfg_percent),
		cfg_condition(guid_cfg_condition, default_cfg_condition), 
		cfg_logSkipTimes(guid_cfg_logSkipTimes, default_cfg_logSkipTimes),
		cfg_skipProtectionNextTime(guid_cfg_skipProtectionNextTime, default_cfg_skipProtectionNextTime),
		cfg_skipProtectionRandomTime(guid_cfg_skipProtectionRandomTime, default_cfg_skipProtectionRandomTime),
		cfg_skipProtectionPreviousTime(guid_cfg_skipProtectionPreviousTime, default_cfg_skipProtectionPreviousTime);

#ifdef _WIN32
	BOOL my_preferences::OnInitDialog(CWindow, LPARAM) {
		// Enable dark mode
		m_dark.AddDialogWithControls(m_hWnd);

		uButton_SetCheck(m_hWnd, IDC_COUNT_NEXT, cfg_countNext);

		uButton_SetCheck(m_hWnd, IDC_COUNT_RANDOM, cfg_countRandom);
		uButton_SetCheck(m_hWnd, IDC_COUNT_PREVIOUS, cfg_countPrevious);

		CComboBox conditionDropList = (CComboBox)GetDlgItem(IDC_CONDITION);
		::uSendMessageText(conditionDropList, CB_ADDSTRING, 0, "Time");
		::uSendMessageText(conditionDropList, CB_ADDSTRING, 0, "Percent");
		::uSendMessageText(conditionDropList, CB_ADDSTRING, 0, "Time and Percent");
		::SendMessage(conditionDropList, CB_SETCURSEL, (WPARAM)cfg_condition, NULL);

		SetDlgItemInt(IDC_TIME, cfg_time, FALSE);
		SetDlgItemInt(IDC_PERCENT, cfg_percent, FALSE);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE, cfg_countFromPause);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_STOP, cfg_countFromStop);

		CComboBox logSkipTimesDropList = (CComboBox)GetDlgItem(IDC_LOG_SKIP_TIMES);
		::uSendMessageText(logSkipTimesDropList, CB_ADDSTRING, 0, "None");
		::uSendMessageText(logSkipTimesDropList, CB_ADDSTRING, 0, "Last");
		::uSendMessageText(logSkipTimesDropList, CB_ADDSTRING, 0, "All");
		::SendMessage(logSkipTimesDropList, CB_SETCURSEL, (WPARAM)cfg_logSkipTimes, NULL);

		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_NEXT, cfg_skipProtectionNext);
		SetDlgItemInt(IDC_SKIP_PROTECTION_NEXT_TIME, cfg_skipProtectionNextTime, FALSE);
		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_RANDOM, cfg_skipProtectionRandom);
		SetDlgItemInt(IDC_SKIP_PROTECTION_RANDOM_TIME, cfg_skipProtectionRandomTime, FALSE);
		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS, cfg_skipProtectionPrevious);
		SetDlgItemInt(IDC_SKIP_PROTECTION_PREVIOUS_TIME, cfg_skipProtectionPreviousTime, FALSE);

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
		CreateTooltip(tooltips[4], m_hWnd, IDC_TIME,
			L"Duration of song to count skip",
			L"\nEnter a number from 0 to 2 billion. After that duration in the song, skips no longer count."
		);
		CreateTooltip(tooltips[5], m_hWnd, IDC_PERCENT,
			L"Percentage of song to count skip",
			L"\nEnter a number from 0 to 100. After that percentage in the song, skips no longer count."
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
		CreateTooltip(tooltips[8], m_hWnd, IDC_LOG_SKIP_TIMES,
			L"Log timestamps of each skip",
			L"\n'None' will not log any skips. 'Last' only tracks the most recent skip with %last_skip%. "
			L"'All' logs every skip which and can be displayed with %skip_times_raw% and %skip_times_js%."
		);
		CreateTooltip(tooltips[9], m_hWnd, IDC_SKIP_PROTECTION_NEXT,
			L"Disable skip counting for a user-defined time after using Next Song",
			L"\nWhen enabled, using a control during the condition that would normally count as a skip "
			L"after pressing Next Song will not count as a skip. The duration to count skips is still"
			L"the same as the setting above, just shifted."
		);
		CreateTooltip(tooltips[10], m_hWnd, IDC_SKIP_PROTECTION_RANDOM,
			L"Disable skip counting for a user-defined time second after using Random Song",
			L"\nWhen enabled, using a control during the condition that would normally count as a skip "
			L"after pressing Previous Song will not count as a skip. The duration to count skips is still"
			L"the same as the setting above, just shifted."
		);
		CreateTooltip(tooltips[11], m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS,
			L"Disable skip counting for a user-defined time second after using Previous Song",
			L"\nWhen enabled, using a control during the condition that would normally count as a skip "
			L"after pressing Previous Song will not count as a skip. The duration to count skips is still"
			L"the same as the setting above, just shifted."
		);

		return FALSE;
	}

	void my_preferences::CreateTooltip(CToolTipCtrl tooltip, CWindow hWnd, int parent, LPCTSTR title, LPCTSTR body) {
		if(tooltip.Create(hWnd, nullptr, nullptr, TTS_NOPREFIX | TTS_BALLOON)) {
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
		if(HasChanged()) { state |= preferences_state::changed; }
		return state;
	}

	void my_preferences::reset() {
		uButton_SetCheck(m_hWnd, IDC_COUNT_NEXT, default_cfg_countNext);
		uButton_SetCheck(m_hWnd, IDC_COUNT_RANDOM, default_cfg_countRandom);
		uButton_SetCheck(m_hWnd, IDC_COUNT_PREVIOUS,default_cfg_countPrevious);
		GetDlgItem(IDC_CONDITION).SendMessage(CB_SETCURSEL, (WPARAM)default_cfg_condition, NULL);
		SetDlgItemInt(IDC_TIME, default_cfg_time, FALSE);
		SetDlgItemInt(IDC_PERCENT, default_cfg_percent, FALSE);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE, default_cfg_countFromPause);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_STOP, default_cfg_countFromStop);
		GetDlgItem(IDC_LOG_SKIP_TIMES).SendMessage(CB_SETCURSEL, (WPARAM)default_cfg_logSkipTimes, NULL);
		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_NEXT, default_cfg_skipProtectionNext);
		SetDlgItemInt(IDC_SKIP_PROTECTION_NEXT_TIME, default_cfg_skipProtectionNextTime, FALSE);
		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_RANDOM, default_cfg_skipProtectionRandom);
		SetDlgItemInt(IDC_SKIP_PROTECTION_RANDOM_TIME, default_cfg_skipProtectionRandomTime, FALSE);
		uButton_SetCheck(m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS, default_cfg_skipProtectionPrevious);
		SetDlgItemInt(IDC_SKIP_PROTECTION_PREVIOUS_TIME, default_cfg_skipProtectionPreviousTime, FALSE);
		OnChanged();
	}

	void my_preferences::apply() {
		cfg_countNext = uButton_GetCheck(m_hWnd, IDC_COUNT_NEXT);
		cfg_countRandom = uButton_GetCheck(m_hWnd, IDC_COUNT_RANDOM);
		cfg_countPrevious = uButton_GetCheck(m_hWnd, IDC_COUNT_PREVIOUS);
		cfg_condition = (t_int32)SendDlgItemMessage(IDC_CONDITION, CB_GETCURSEL);
		cfg_percent = GetIntConformedToBounds(IDC_PERCENT, 0, default_cfg_percent, 100, 100);
		cfg_time = GetIntConformedToBounds(IDC_TIME, 0, default_cfg_time, 1000000000, 1000);
		cfg_countFromPause = uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE);
		cfg_countFromStop = uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_STOP);
		cfg_logSkipTimes = (t_int32)SendDlgItemMessage(IDC_LOG_SKIP_TIMES, CB_GETCURSEL);
		cfg_skipProtectionNext = uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_NEXT);
		cfg_skipProtectionNextTime = GetIntConformedToBounds(IDC_SKIP_PROTECTION_NEXT_TIME, 0, default_cfg_skipProtectionNextTime, 1000000000, 1000);
		cfg_skipProtectionRandom = uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_RANDOM);
		cfg_skipProtectionRandomTime = GetIntConformedToBounds(IDC_SKIP_PROTECTION_RANDOM_TIME, 0, default_cfg_skipProtectionRandomTime, 1000000000, 1000);
		cfg_skipProtectionPrevious = uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS);
		cfg_skipProtectionPreviousTime = GetIntConformedToBounds(IDC_SKIP_PROTECTION_PREVIOUS_TIME, 0, default_cfg_skipProtectionPreviousTime, 1000000000, 1000);

		refreshGlobal();
		OnChanged(); // Dialog content has not changed but the flags have - shown values now match the settings so the apply button can be disabled
	}

	// ES_NUMBER only allows valid number characters to be typed or pasted.
	// This enforces number bounds.
	t_uint32 my_preferences::GetIntConformedToBounds(int ID, t_int32 min, t_int32 minDefault, t_int32 max, t_int32 maxDefault) {
		t_int32 userInput = (t_int32)GetDlgItemInt(ID, NULL, FALSE);
		if(userInput < min) {
			SetDlgItemInt(ID, minDefault, FALSE);
			return minDefault;
		}
		else if(userInput > max) {
			SetDlgItemInt(ID, maxDefault, FALSE);
			return maxDefault;
		}
		else {
			return userInput;
		}
	}

	// Is there a less horrendous way to do this?
	bool my_preferences::HasChanged() {
	
		// Returns whether the settings are different from the current configuration - (De)activating the apply button
		//uButton_GetCheck
		return uButton_GetCheck(m_hWnd, IDC_COUNT_NEXT) != cfg_countNext ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_RANDOM) != cfg_countRandom ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_PREVIOUS) != cfg_countPrevious ||
			(t_int32)SendDlgItemMessage(IDC_CONDITION, CB_GETCURSEL) != cfg_condition || 
			GetDlgItemInt(IDC_TIME, NULL, FALSE) != cfg_time ||
			GetDlgItemInt(IDC_PERCENT, NULL, FALSE) != cfg_percent ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE) != cfg_countFromPause ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_STOP) != cfg_countFromStop ||
			(t_int32)SendDlgItemMessage(IDC_LOG_SKIP_TIMES, CB_GETCURSEL) != cfg_logSkipTimes ||
			uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_NEXT) != cfg_skipProtectionNext ||
			GetDlgItemInt(IDC_SKIP_PROTECTION_NEXT_TIME, NULL, FALSE) != cfg_skipProtectionNextTime ||
			uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_RANDOM) != cfg_skipProtectionRandom ||
			GetDlgItemInt(IDC_SKIP_PROTECTION_RANDOM_TIME, NULL, FALSE) != cfg_skipProtectionRandomTime ||
			uButton_GetCheck(m_hWnd, IDC_SKIP_PROTECTION_PREVIOUS) != cfg_skipProtectionPrevious ||
			GetDlgItemInt(IDC_SKIP_PROTECTION_PREVIOUS_TIME, NULL, FALSE) != cfg_skipProtectionPreviousTime;
	}

	void my_preferences::OnChanged() {
		// Update state to 'changed' to enable/disable the apply button appropriately.
		m_callback->on_state_changed();
	}

	const char* my_preferences_page::get_name() {
		return "Skip Count";
	}
#endif // _WIN32
} // namespace foo_skipcount
