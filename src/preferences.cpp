#include <string>

#include "preferences.h"
#include "SkipCount.h"

#include <SDK/cfg_var.h>

// check todos
namespace foo_skipcount {
	// Config
	cfg_bool cfg_countNext(guid_cfg_countNext, default_cfg_countNext),
		cfg_countRandom(guid_cfg_countRandom, default_cfg_countRandom),
		cfg_countPrevious(guid_cfg_countPrevious, default_cfg_countPrevious),
		cfg_countFromStop(guid_cfg_countFromStop, default_cfg_countFromStop),
		cfg_countFromPause(guid_cfg_countFromPause, default_cfg_countFromPause),
		cfg_confirmContextClear(guid_cfg_confirmContextClear, default_cfg_confirmContextClear),
		cfg_skipProtectionNext(guid_cfg_skipProtectionNext, default_cfg_skipProtectionNext),
		cfg_skipProtectionRandom(guid_cfg_skipProtectionRandom, default_cfg_skipProtectionRandom),
		cfg_skipProtectionPrevious(guid_cfg_skipProtectionPrevious, default_cfg_skipProtectionPrevious),
		cfg_pasteDisableDialog(guid_cfg_pasteDisableDialog, default_cfg_pasteDisableDialog),
		cfg_pasteNextCount(guid_cfg_pasteNextCount, default_cfg_pasteNextCount),
		cfg_pasteRandomCount(guid_cfg_pasteRandomCount, default_cfg_pasteRandomCount),
		cfg_pastePreviousCount(guid_cfg_pastePreviousCount, default_cfg_pastePreviousCount),
		cfg_xmlDisableDialog(guid_cfg_xmlDisableDialog, default_cfg_xmlDisableDialog),
		cfg_xmlExportJSTimestamp(guid_cfg_xmlExportJSTimestamp, default_cfg_xmlExportJSTimestamp),
		cfg_xmlExportDateTime(guid_cfg_xmlExportDateTime, default_cfg_xmlExportDateTime),
		cfg_xmlExportNextCount(guid_cfg_xmlExportNextCount, default_cfg_xmlExportNextCount),
		cfg_xmlExportRandomCount(guid_cfg_xmlExportRandomCount, default_cfg_xmlExportRandomCount),
		cfg_xmlExportPreviousCount(guid_cfg_xmlExportPreviousCount, default_cfg_xmlExportPreviousCount),
		cfg_xmlExportTotalCount(guid_cfg_xmlExportTotalCount, default_cfg_xmlExportTotalCount),
		cfg_xmlExportLatestSkip(guid_cfg_xmlExportLatestSkip, default_cfg_xmlExportLatestSkip),
		cfg_xmlExportOldestSkip(guid_cfg_xmlExportOldestSkip, default_cfg_xmlExportOldestSkip),
		cfg_xmlExportTimestampCount(guid_cfg_xmlExportTimestampCount, default_cfg_xmlExportTimestampCount),
		cfg_xmlExportAllTimestamps(guid_cfg_xmlExportAllTimestamps, default_cfg_xmlExportAllTimestamps),
		cfg_tagDisableDialog(guid_cfg_tagDisableDialog, default_cfg_tagDisableDialog),
		cfg_tagNextCount(guid_cfg_tagNextCount, default_cfg_tagNextCount),
		cfg_tagRandomCount(guid_cfg_tagRandomCount, default_cfg_tagRandomCount),
		cfg_tagPreviousCount(guid_cfg_tagPreviousCount, default_cfg_tagPreviousCount),
		cfg_tagCurrentTimestamp(guid_cfg_tagCurrentTimestamp, default_cfg_tagCurrentTimestamp),
		cfg_tagAllTimestampsRaw(guid_cfg_tagAllTimestampsRaw, default_cfg_tagAllTimestampsRaw);

	cfg_uint cfg_condition(guid_cfg_condition, default_cfg_condition),
		cfg_time(guid_cfg_time, default_cfg_time),
		cfg_percent(guid_cfg_percent, default_cfg_percent),
		cfg_logSkipTimes(guid_cfg_logSkipTimes, default_cfg_logSkipTimes),
		cfg_skipProtectionNextTime(guid_cfg_skipProtectionNextTime, default_cfg_skipProtectionNextTime),
		cfg_skipProtectionRandomTime(guid_cfg_skipProtectionRandomTime, default_cfg_skipProtectionRandomTime),
		cfg_skipProtectionPreviousTime(guid_cfg_skipProtectionPreviousTime, default_cfg_skipProtectionPreviousTime),
		cfg_pasteSkipSelection(guid_cfg_pasteSkipSelection, default_cfg_pasteSkipSelection),
		cfg_tagTimestampDelimiter(guid_cfg_tagTimestampDelimiter, default_cfg_tagTimestampDelimiter);

#ifdef _WIN32
	// Strictly enforces number bounds, ES_NUMBER only limits to valid number characters
	t_uint32 GetIntConformedToBounds(HWND hWnd, t_int32 ctrlID) {
		t_uint val = GetDlgItemInt(hWnd, ctrlID, NULL, FALSE);
		if(val < EditControlBoundsDefaults.at(ctrlID)[0]) {
			val = EditControlBoundsDefaults.at(ctrlID)[1];
		}
		else if(val > EditControlBoundsDefaults.at(ctrlID)[2]) {
			val = EditControlBoundsDefaults.at(ctrlID)[3];
		}
		return val;
	}

	void InitPasteControls(HWND hWnd) {
		uButton_SetCheck(hWnd, IDC_PASTE_DISABLE_DIALOG, cfg_pasteDisableDialog);
		uButton_SetCheck(hWnd, IDC_PASTE_NEXT_COUNT, cfg_pasteNextCount);
		uButton_SetCheck(hWnd, IDC_PASTE_RANDOM_COUNT, cfg_pasteRandomCount);
		uButton_SetCheck(hWnd, IDC_PASTE_PREVIOUS_COUNT, cfg_pastePreviousCount);
		CComboBox pasteSkipTypesDropList = (CComboBox)GetDlgItem(hWnd, IDC_PASTE_SKIP_SELECTION);
		for(int i = 0; i < PASTESKIP_TOTAL; ++i) {
			pasteSkipTypesDropList.InsertString(i, pasteSkipSelectionStrings[i]);
		}
		pasteSkipTypesDropList.SetCurSel(cfg_pasteSkipSelection);
	}

	void InitXmlControls(HWND hWnd) {
		uButton_SetCheck(hWnd, IDC_XML_DISABLE_DIALOG, cfg_xmlDisableDialog);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_JS_TIMESTAMP, cfg_xmlExportJSTimestamp);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_DATE_TIME, cfg_xmlExportDateTime);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_NEXT_COUNT, cfg_xmlExportNextCount);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_RANDOM_COUNT, cfg_xmlExportRandomCount);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT, cfg_xmlExportPreviousCount);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_TOTAL_COUNT, cfg_xmlExportTotalCount);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_LATEST_SKIP, cfg_xmlExportLatestSkip);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_OLDEST_SKIP, cfg_xmlExportOldestSkip);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT, cfg_xmlExportTimestampCount);
		uButton_SetCheck(hWnd, IDC_XML_EXPORT_ALL_SKIPS, cfg_xmlExportAllTimestamps);
	}

	void InitTagControls(HWND hWnd) {
		uButton_SetCheck(hWnd, IDC_TAG_DISABLE_DIALOG, cfg_tagDisableDialog);
		uButton_SetCheck(hWnd, IDC_TAG_NEXT_COUNT, cfg_tagNextCount);
		uButton_SetCheck(hWnd, IDC_TAG_RANDOM_COUNT, cfg_tagRandomCount);
		uButton_SetCheck(hWnd, IDC_TAG_PREVIOUS_COUNT, cfg_tagPreviousCount);
		uButton_SetCheck(hWnd, IDC_TAG_CURRENT_TIMESTAMP, cfg_tagCurrentTimestamp);
		uButton_SetCheck(hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW, cfg_tagAllTimestampsRaw);
		CComboBox tagDelimiterDropList = (CComboBox)GetDlgItem(hWnd, IDC_TAG_TIMESTAMP_DELIMITER);
		for(int i = 0; i < PASTESKIP_TOTAL; ++i) {
			tagDelimiterDropList.InsertString(i, tagTimestampDelimiterLabels[i]);
		}
		tagDelimiterDropList.SetCurSel(cfg_tagTimestampDelimiter);
	}

	void CreateTooltip(CToolTipCtrl tooltip, CWindow hWnd, int parent) {
		if(tooltip.Create(hWnd, nullptr, nullptr, TTS_NOPREFIX | TTS_BALLOON)) {
			CToolInfo toolInfo(TTF_IDISHWND | TTF_SUBCLASS | TTF_CENTERTIP, GetDlgItem(hWnd, parent), 0, nullptr, nullptr);
			HICON icon = 0;
			tooltip.AddTool(&toolInfo);

			tooltip.SetTitle(icon, ToolTipStrings.at(parent)[0].c_str());
			tooltip.UpdateTipText(ToolTipStrings.at(parent)[1].c_str(), GetDlgItem(hWnd, parent));
			tooltip.SetMaxTipWidth(300);
			tooltip.SetDelayTime(TTDT_INITIAL, 300);
			tooltip.SetDelayTime(TTDT_AUTOPOP, 30000);
			tooltip.Activate(TRUE);
		}
	}

	t_uint32 my_preferences::get_state() {
		// IMPORTANT: Always return dark_mode_supported
		t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;
		if(HasChanged()) {
			state |= preferences_state::changed;
		}
		return state;
	}

	void my_preferences::apply() {
		cfg_countNext = uButton_GetCheck(m_hWnd, IDC_COUNT_NEXT);
		cfg_countRandom = uButton_GetCheck(m_hWnd, IDC_COUNT_RANDOM);
		cfg_countPrevious = uButton_GetCheck(m_hWnd, IDC_COUNT_PREVIOUS);
		cfg_countFromStop = uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_STOP);
		cfg_countFromPause = uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE);
		cfg_condition = (t_int32)uSendDlgItemMessage(IDC_CONDITION, CB_GETCURSEL);
		cfg_time = GetIntConformedToBounds(m_hWnd, IDC_TIME);
		cfg_percent = GetIntConformedToBounds(m_hWnd, IDC_PERCENT);
		cfg_logSkipTimes = (t_int32)uSendDlgItemMessage(IDC_LOG_SKIP_TIMES, CB_GETCURSEL);
		cfg_confirmContextClear = uButton_GetCheck(m_hWnd, IDC_CONFIRM_CONTEXT_CLEAR);
		cfg_skipProtectionNext = uButton_GetCheck(m_hWnd, IDC_PROTECTION_NEXT);
		cfg_skipProtectionNextTime = GetIntConformedToBounds(m_hWnd, IDC_PROTECTION_NEXT_TIME);
		cfg_skipProtectionRandom = uButton_GetCheck(m_hWnd, IDC_PROTECTION_RANDOM);
		cfg_skipProtectionRandomTime = GetIntConformedToBounds(m_hWnd, IDC_PROTECTION_RANDOM_TIME);
		cfg_skipProtectionPrevious = uButton_GetCheck(m_hWnd, IDC_PROTECTION_PREVIOUS);
		cfg_skipProtectionPreviousTime = GetIntConformedToBounds(m_hWnd, IDC_PROTECTION_PREVIOUS_TIME);
		cfg_pasteDisableDialog = uButton_GetCheck(m_hWnd, IDC_PASTE_DISABLE_DIALOG);
		cfg_pasteNextCount = uButton_GetCheck(m_hWnd, IDC_PASTE_NEXT_COUNT);
		cfg_pasteRandomCount = uButton_GetCheck(m_hWnd, IDC_PASTE_RANDOM_COUNT);
		cfg_pastePreviousCount = uButton_GetCheck(m_hWnd, IDC_PASTE_PREVIOUS_COUNT);
		cfg_pasteSkipSelection = (t_int32)uSendDlgItemMessage(IDC_PASTE_SKIP_SELECTION, CB_GETCURSEL);
		cfg_xmlDisableDialog = uButton_GetCheck(m_hWnd, IDC_XML_DISABLE_DIALOG);
		cfg_xmlExportJSTimestamp = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP);
		cfg_xmlExportDateTime = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_DATE_TIME);
		cfg_xmlExportNextCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_NEXT_COUNT);
		cfg_xmlExportRandomCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT);
		cfg_xmlExportPreviousCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT);
		cfg_xmlExportTotalCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT);
		cfg_xmlExportLatestSkip = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_LATEST_SKIP);
		cfg_xmlExportOldestSkip = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP);
		cfg_xmlExportTimestampCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT);
		cfg_xmlExportAllTimestamps = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_ALL_SKIPS);
		cfg_tagDisableDialog = uButton_GetCheck(m_hWnd, IDC_TAG_DISABLE_DIALOG);
		cfg_tagNextCount = uButton_GetCheck(m_hWnd, IDC_TAG_NEXT_COUNT);
		cfg_tagRandomCount = uButton_GetCheck(m_hWnd, IDC_TAG_RANDOM_COUNT);
		cfg_tagPreviousCount = uButton_GetCheck(m_hWnd, IDC_TAG_PREVIOUS_COUNT);
		cfg_tagCurrentTimestamp = uButton_GetCheck(m_hWnd, IDC_TAG_CURRENT_TIMESTAMP);
		cfg_tagAllTimestampsRaw = uButton_GetCheck(m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW);
		cfg_tagTimestampDelimiter = (t_int32)uSendDlgItemMessage(IDC_TAG_TIMESTAMP_DELIMITER, CB_GETCURSEL);

		theAPI()->dispatch_global_refresh();
		OnChanged(); // Dialog content has not changed but the flags have - shown values now match the settings so the apply button can be disabled
	}

	void my_preferences::reset() {
		uButton_SetCheck(m_hWnd, IDC_COUNT_NEXT, default_cfg_countNext);
		uButton_SetCheck(m_hWnd, IDC_COUNT_RANDOM, default_cfg_countRandom);
		uButton_SetCheck(m_hWnd, IDC_COUNT_PREVIOUS, default_cfg_countPrevious);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_STOP, default_cfg_countFromStop);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE, default_cfg_countFromPause);
		uSendDlgItemMessage(IDC_CONDITION, CB_SETCURSEL, default_cfg_condition, NULL);
		uSetDlgItemInt(IDC_TIME, default_cfg_time, TRUE);
		uSetDlgItemInt(IDC_PERCENT, default_cfg_percent, TRUE);
		uSendDlgItemMessage(IDC_LOG_SKIP_TIMES, CB_SETCURSEL, default_cfg_logSkipTimes, NULL);
		uButton_SetCheck(m_hWnd, IDC_CONFIRM_CONTEXT_CLEAR, default_cfg_confirmContextClear);
		uButton_SetCheck(m_hWnd, IDC_PROTECTION_NEXT, default_cfg_skipProtectionNext);
		uSetDlgItemInt(IDC_PROTECTION_NEXT_TIME, default_cfg_skipProtectionNextTime, TRUE);
		uButton_SetCheck(m_hWnd, IDC_PROTECTION_RANDOM, default_cfg_skipProtectionRandom);
		uSetDlgItemInt(IDC_PROTECTION_RANDOM_TIME, default_cfg_skipProtectionRandomTime, TRUE);
		uButton_SetCheck(m_hWnd, IDC_PROTECTION_PREVIOUS, default_cfg_skipProtectionPrevious);
		uSetDlgItemInt(IDC_PROTECTION_PREVIOUS_TIME, default_cfg_skipProtectionPreviousTime, TRUE);
		uButton_SetCheck(m_hWnd, IDC_PASTE_DISABLE_DIALOG, default_cfg_pasteDisableDialog);
		uButton_SetCheck(m_hWnd, IDC_PASTE_NEXT_COUNT, default_cfg_pasteNextCount);
		uButton_SetCheck(m_hWnd, IDC_PASTE_RANDOM_COUNT, default_cfg_pasteRandomCount);
		uButton_SetCheck(m_hWnd, IDC_PASTE_PREVIOUS_COUNT, default_cfg_pastePreviousCount);
		uSendDlgItemMessage(IDC_PASTE_SKIP_SELECTION, CB_SETCURSEL, default_cfg_pasteSkipSelection, NULL);
		uButton_SetCheck(m_hWnd, IDC_XML_DISABLE_DIALOG, default_cfg_xmlDisableDialog);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP, default_cfg_xmlExportJSTimestamp);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_DATE_TIME, default_cfg_xmlExportDateTime);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_NEXT_COUNT, default_cfg_xmlExportNextCount);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT, default_cfg_xmlExportRandomCount);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT, default_cfg_xmlExportPreviousCount);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT, default_cfg_xmlExportTotalCount);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_LATEST_SKIP, default_cfg_xmlExportLatestSkip);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP, default_cfg_xmlExportOldestSkip);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT, default_cfg_xmlExportTimestampCount);
		uButton_SetCheck(m_hWnd, IDC_XML_EXPORT_ALL_SKIPS, default_cfg_xmlExportAllTimestamps);
		uButton_SetCheck(m_hWnd, IDC_TAG_DISABLE_DIALOG, default_cfg_tagDisableDialog);
		uButton_SetCheck(m_hWnd, IDC_TAG_NEXT_COUNT, default_cfg_tagNextCount);
		uButton_SetCheck(m_hWnd, IDC_TAG_RANDOM_COUNT, default_cfg_tagRandomCount);
		uButton_SetCheck(m_hWnd, IDC_TAG_PREVIOUS_COUNT, default_cfg_tagPreviousCount);
		uButton_SetCheck(m_hWnd, IDC_TAG_CURRENT_TIMESTAMP, default_cfg_tagCurrentTimestamp);
		uButton_SetCheck(m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW, default_cfg_tagAllTimestampsRaw);
		uSendDlgItemMessage(IDC_TAG_TIMESTAMP_DELIMITER, CB_SETCURSEL, default_cfg_tagTimestampDelimiter, NULL);

		OnChanged();
	}

	BOOL my_preferences::OnInitDialog(CWindow, LPARAM) {
		// Enable dark mode
		m_dark.AddDialogWithControls(m_hWnd);

		uButton_SetCheck(m_hWnd, IDC_COUNT_NEXT, cfg_countNext);
		uButton_SetCheck(m_hWnd, IDC_COUNT_RANDOM, cfg_countRandom);
		uButton_SetCheck(m_hWnd, IDC_COUNT_PREVIOUS, cfg_countPrevious);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_STOP, cfg_countFromStop);
		uButton_SetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE, cfg_countFromPause);
		CComboBox conditionDropList = (CComboBox)uGetDlgItem(IDC_CONDITION);
		for(int i = 0; i < LOGCONDITION_TOTAL; ++i) {
			conditionDropList.InsertString(i, logConditionStrings[i]);
		}
		conditionDropList.SetCurSel(cfg_condition);
		uSetDlgItemInt(IDC_TIME, cfg_time, FALSE);
		SendDlgItemMessage(IDC_TIME, EM_SETLIMITTEXT, 9, NULL);
		uSetDlgItemInt(IDC_PERCENT, cfg_percent, FALSE);
		SendDlgItemMessage(IDC_PERCENT, EM_SETLIMITTEXT, 3, NULL);
		SendDlgItemMessage(IDC_PERCENT_SLIDER, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendDlgItemMessage(IDC_PERCENT_SLIDER, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)cfg_percent);

		CComboBox logSkipTimesDropList = (CComboBox)uGetDlgItem(IDC_LOG_SKIP_TIMES);
		for(int i = 0; i < LOGTYPE_TOTAL; ++i) {
			logSkipTimesDropList.InsertString(i, logTypeStrings[i]);
		}
		logSkipTimesDropList.SetCurSel(cfg_logSkipTimes);
		uButton_SetCheck(m_hWnd, IDC_CONFIRM_CONTEXT_CLEAR, cfg_confirmContextClear);

		uButton_SetCheck(m_hWnd, IDC_PROTECTION_NEXT, cfg_skipProtectionNext);
		uSetDlgItemInt(IDC_PROTECTION_NEXT_TIME, cfg_skipProtectionNextTime, FALSE);
		SendDlgItemMessage(IDC_PERCENT, EM_SETLIMITTEXT, 3, NULL);
		uButton_SetCheck(m_hWnd, IDC_PROTECTION_RANDOM, cfg_skipProtectionRandom);
		uSetDlgItemInt(IDC_PROTECTION_RANDOM_TIME, cfg_skipProtectionRandomTime, FALSE);
		uSendDlgItemMessage(IDC_PERCENT, EM_SETLIMITTEXT, 3, NULL);
		uButton_SetCheck(m_hWnd, IDC_PROTECTION_PREVIOUS, cfg_skipProtectionPrevious);
		uSetDlgItemInt(IDC_PROTECTION_PREVIOUS_TIME, cfg_skipProtectionPreviousTime, FALSE);
		uSendDlgItemMessage(IDC_PROTECTION_PREVIOUS_TIME, EM_SETLIMITTEXT, 3, NULL);
		InitPasteControls(m_hWnd);
		InitXmlControls(m_hWnd);
		InitTagControls(m_hWnd);

		CreateTooltip(tooltips[0], m_hWnd, IDC_COUNT_NEXT);
		CreateTooltip(tooltips[1], m_hWnd, IDC_COUNT_RANDOM);
		CreateTooltip(tooltips[2], m_hWnd, IDC_COUNT_PREVIOUS);
		CreateTooltip(tooltips[3], m_hWnd, IDC_COUNT_FROM_STOP);
		CreateTooltip(tooltips[4], m_hWnd, IDC_COUNT_FROM_PAUSE);
		CreateTooltip(tooltips[5], m_hWnd, IDC_CONDITION);
		CreateTooltip(tooltips[6], m_hWnd, IDC_TIME);
		CreateTooltip(tooltips[7], m_hWnd, IDC_PERCENT);
		CreateTooltip(tooltips[8], m_hWnd, IDC_LOG_SKIP_TIMES);
		CreateTooltip(tooltips[9], m_hWnd, IDC_CONFIRM_CONTEXT_CLEAR);
		CreateTooltip(tooltips[10], m_hWnd, IDC_PROTECTION_NEXT);
		CreateTooltip(tooltips[11], m_hWnd, IDC_PROTECTION_RANDOM);
		CreateTooltip(tooltips[12], m_hWnd, IDC_PROTECTION_PREVIOUS);
		CreateTooltip(tooltips[13], m_hWnd, IDC_PASTE_DISABLE_DIALOG);
		CreateTooltip(tooltips[14], m_hWnd, IDC_PASTE_NEXT_COUNT);
		CreateTooltip(tooltips[15], m_hWnd, IDC_PASTE_RANDOM_COUNT);
		CreateTooltip(tooltips[16], m_hWnd, IDC_PASTE_PREVIOUS_COUNT);
		CreateTooltip(tooltips[17], m_hWnd, IDC_PASTE_SKIP_SELECTION);
		CreateTooltip(tooltips[18], m_hWnd, IDC_XML_DISABLE_DIALOG);
		CreateTooltip(tooltips[19], m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP);
		CreateTooltip(tooltips[20], m_hWnd, IDC_XML_EXPORT_DATE_TIME);
		CreateTooltip(tooltips[21], m_hWnd, IDC_XML_EXPORT_NEXT_COUNT);
		CreateTooltip(tooltips[22], m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT);
		CreateTooltip(tooltips[23], m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT);
		CreateTooltip(tooltips[24], m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT);
		CreateTooltip(tooltips[25], m_hWnd, IDC_XML_EXPORT_LATEST_SKIP);
		CreateTooltip(tooltips[26], m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP);
		CreateTooltip(tooltips[27], m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT);
		CreateTooltip(tooltips[28], m_hWnd, IDC_XML_EXPORT_ALL_SKIPS);
		CreateTooltip(tooltips[29], m_hWnd, IDC_TAG_DISABLE_DIALOG);
		CreateTooltip(tooltips[30], m_hWnd, IDC_TAG_NEXT_COUNT);
		CreateTooltip(tooltips[31], m_hWnd, IDC_TAG_RANDOM_COUNT);
		CreateTooltip(tooltips[32], m_hWnd, IDC_TAG_PREVIOUS_COUNT);
		CreateTooltip(tooltips[33], m_hWnd, IDC_TAG_CURRENT_TIMESTAMP);
		CreateTooltip(tooltips[34], m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW);
		CreateTooltip(tooltips[35], m_hWnd, IDC_TAG_TIMESTAMP_DELIMITER);

		return FALSE;
	}

	void my_preferences::OnButtonChange(UINT, int, CWindow) {
		OnChanged();
	}

	void my_preferences::OnSelectionChange(UINT, int, CWindow) {
		OnChanged();
	}

	void my_preferences::OnEditChange(UINT, int ctrlID, CWindow wnd) {
		if(ctrlID == IDC_PERCENT) {
			t_uint32 txt = GetIntConformedToBounds(m_hWnd, IDC_PERCENT);
			t_uint32 val = static_cast<int>(uSendDlgItemMessage(IDC_PERCENT_SLIDER, TBM_GETPOS, NULL, NULL));
			if(val != txt) {
				uSendDlgItemMessage(IDC_PERCENT_SLIDER, TBM_SETPOS, TRUE, txt);
			}
		}
		OnChanged();
	}

	BOOL my_preferences::OnFocusLost(UINT, int ctrlID, CWindow) {
		CString characterCounter;
		GetDlgItemText(ctrlID, characterCounter);
		if(characterCounter.GetLength() == 0) {
			t_uint32 newVal = 0;
			if(ctrlID == IDC_TIME) {
				newVal = cfg_time;
			}
			else if(ctrlID == IDC_PERCENT) {
				newVal = cfg_percent;
			}
			else if(ctrlID == IDC_PROTECTION_NEXT_TIME) {
				newVal = cfg_skipProtectionNextTime;
			}
			else if(ctrlID == IDC_PROTECTION_RANDOM_TIME) {
				newVal = cfg_skipProtectionRandomTime;
			}
			else if(ctrlID == IDC_PROTECTION_PREVIOUS_TIME) {
				newVal = cfg_skipProtectionPreviousTime;
			}
			uSetDlgItemInt(ctrlID, newVal, FALSE);
		}
		else {
			uSetDlgItemInt(ctrlID, GetIntConformedToBounds(m_hWnd, ctrlID), FALSE);
		}
		OnChanged();
		return true;
	}

	void my_preferences::OnHScrollChange(UINT nSBCode, LPARAM nPos, CScrollBar pScrollBar) {
		if(nSBCode == TB_THUMBPOSITION || nSBCode == TB_ENDTRACK) {
			return;
		}
		if(pScrollBar.m_hWnd == uGetDlgItem(IDC_PERCENT_SLIDER)) {
			t_uint32 txt = GetIntConformedToBounds(m_hWnd, IDC_PERCENT);
			t_uint32 val = -1;
			if(nSBCode == TB_THUMBTRACK) {
				val = static_cast<int>(nPos);
			}
			else {
				val = static_cast<int>(uSendDlgItemMessage(IDC_PERCENT_SLIDER, TBM_GETPOS, NULL, NULL));
			}

			if(val != txt) {
				uSetDlgItemInt(IDC_PERCENT, val, FALSE);
			}
		}
		OnChanged();
	}

	bool my_preferences::HasChanged() {
		// Returns whether the settings are different from the currently saved configuration - (De)activating the apply button
		return uButton_GetCheck(m_hWnd, IDC_COUNT_NEXT) != cfg_countNext ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_RANDOM) != cfg_countRandom ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_PREVIOUS) != cfg_countPrevious ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_STOP) != cfg_countFromStop ||
			uButton_GetCheck(m_hWnd, IDC_COUNT_FROM_PAUSE) != cfg_countFromPause ||
			(t_int32)uSendDlgItemMessage(IDC_CONDITION, CB_GETCURSEL) != cfg_condition ||
			uGetDlgItemInt(IDC_TIME, NULL, FALSE) != cfg_time ||
			uGetDlgItemInt(IDC_PERCENT, NULL, FALSE) != cfg_percent ||
			(t_int32)uSendDlgItemMessage(IDC_LOG_SKIP_TIMES, CB_GETCURSEL) != cfg_logSkipTimes ||
			uButton_GetCheck(m_hWnd, IDC_CONFIRM_CONTEXT_CLEAR) != cfg_confirmContextClear ||
			uButton_GetCheck(m_hWnd, IDC_PROTECTION_NEXT) != cfg_skipProtectionNext ||
			uGetDlgItemInt(IDC_PROTECTION_NEXT_TIME, NULL, FALSE) != cfg_skipProtectionNextTime ||
			uButton_GetCheck(m_hWnd, IDC_PROTECTION_RANDOM) != cfg_skipProtectionRandom ||
			uGetDlgItemInt(IDC_PROTECTION_RANDOM_TIME, NULL, FALSE) != cfg_skipProtectionRandomTime ||
			uButton_GetCheck(m_hWnd, IDC_PROTECTION_PREVIOUS) != cfg_skipProtectionPrevious ||
			uGetDlgItemInt(IDC_PROTECTION_PREVIOUS_TIME, NULL, FALSE) != cfg_skipProtectionPreviousTime ||
			uButton_GetCheck(m_hWnd, IDC_PASTE_DISABLE_DIALOG) != cfg_pasteDisableDialog ||
			uButton_GetCheck(m_hWnd, IDC_PASTE_NEXT_COUNT) != cfg_pasteNextCount ||
			uButton_GetCheck(m_hWnd, IDC_PASTE_RANDOM_COUNT) != cfg_pasteRandomCount ||
			uButton_GetCheck(m_hWnd, IDC_PASTE_PREVIOUS_COUNT) != cfg_pastePreviousCount ||
			(t_int32)uSendDlgItemMessage(IDC_PASTE_SKIP_SELECTION, CB_GETCURSEL, NULL, NULL) != cfg_pasteSkipSelection ||
			uButton_GetCheck(m_hWnd, IDC_XML_DISABLE_DIALOG) != cfg_xmlDisableDialog ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP) != cfg_xmlExportJSTimestamp ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_DATE_TIME) != cfg_xmlExportDateTime ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_NEXT_COUNT) != cfg_xmlExportNextCount ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT) != cfg_xmlExportRandomCount ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT) != cfg_xmlExportPreviousCount ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT) != cfg_xmlExportTotalCount ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_LATEST_SKIP) != cfg_xmlExportLatestSkip ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP) != cfg_xmlExportOldestSkip ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT) != cfg_xmlExportTimestampCount ||
			uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_ALL_SKIPS) != cfg_xmlExportAllTimestamps ||
			uButton_GetCheck(m_hWnd, IDC_TAG_DISABLE_DIALOG) != cfg_tagDisableDialog ||
			uButton_GetCheck(m_hWnd, IDC_TAG_NEXT_COUNT) != cfg_tagNextCount ||
			uButton_GetCheck(m_hWnd, IDC_TAG_RANDOM_COUNT) != cfg_tagRandomCount ||
			uButton_GetCheck(m_hWnd, IDC_TAG_PREVIOUS_COUNT) != cfg_tagPreviousCount ||
			uButton_GetCheck(m_hWnd, IDC_TAG_CURRENT_TIMESTAMP) != cfg_tagCurrentTimestamp ||
			uButton_GetCheck(m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW) != cfg_tagAllTimestampsRaw ||
			(t_int32)uSendDlgItemMessage(IDC_TAG_TIMESTAMP_DELIMITER, CB_GETCURSEL, NULL, NULL) != cfg_tagTimestampDelimiter;
	}

	void my_preferences::OnChanged() {
		// Update state to 'changed' to enable/disable the apply button appropriately.
		m_callback->on_state_changed();
	}

	/* Paste Dialog */
	paste_dialog::~paste_dialog() {
		if(m_hWnd) {
			delete[] m_hWnd;
		}
	}

	BOOL paste_dialog::OnInitDialog(CWindow, LPARAM) {
		m_dark.AddDialogWithControls(m_hWnd);

		InitPasteControls(m_hWnd);

		CreateTooltip(tooltips[0], m_hWnd, IDC_PASTE_DISABLE_DIALOG);
		CreateTooltip(tooltips[1], m_hWnd, IDC_PASTE_NEXT_COUNT);
		CreateTooltip(tooltips[2], m_hWnd, IDC_PASTE_RANDOM_COUNT);
		CreateTooltip(tooltips[3], m_hWnd, IDC_PASTE_PREVIOUS_COUNT);
		CreateTooltip(tooltips[4], m_hWnd, IDC_PASTE_SKIP_SELECTION);

		return TRUE;
	}

	void paste_dialog::OnSave(UINT, int, CWindow) {
		cfg_pasteDisableDialog = uButton_GetCheck(m_hWnd, IDC_PASTE_DISABLE_DIALOG);
		cfg_pasteNextCount = uButton_GetCheck(m_hWnd, IDC_PASTE_NEXT_COUNT);
		cfg_pasteRandomCount = uButton_GetCheck(m_hWnd, IDC_PASTE_RANDOM_COUNT);
		cfg_pastePreviousCount = uButton_GetCheck(m_hWnd, IDC_PASTE_PREVIOUS_COUNT);
		cfg_pasteSkipSelection = (t_int32)uSendDlgItemMessage(IDC_PASTE_SKIP_SELECTION, CB_GETCURSEL);
	}

	void paste_dialog::OnClose(UINT, int ctrlID, CWindow) {
		if(ctrlID == IDC_CANCEL) {
			EndDialog(-1);
		}
		else if(ctrlID == IDC_OK) {
			cfg_pasteDisableDialog = uButton_GetCheck(m_hWnd, IDC_PASTE_DISABLE_DIALOG);
			t_uint32 r = 0;
			r |= uButton_GetCheck(m_hWnd, IDC_PASTE_NEXT_COUNT) ? PASTE_NEXTCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_PASTE_RANDOM_COUNT) ? PASTE_RANDOMCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_PASTE_PREVIOUS_COUNT) ? PASTE_PREVIOUSCOUNT : 0;

			switch((t_int32)uSendDlgItemMessage(IDC_PASTE_SKIP_SELECTION, CB_GETCURSEL)) {
				case PASTESKIP_ALL:
					r |= PASTE_ALLSKIPS;
					break;
				case PASTESKIP_NONE:
					r |= PASTE_NOSKIPS;
					break;
				case PASTESKIP_LATEST:
					r |= PASTE_LATESTSKIP;
					break;
				case PASTESKIP_OLDEST:
					r |= PASTE_OLDESTSKIP;
					break;
				case PASTESKIP_ALLBUTOLDEST:
					r |= PASTE_ALLBUTOLDESTSKIP;
					break;
				case PASTESKIP_ALLBUTLATEST:
					r |= PASTE_ALLBUTLATESTSKIP;
					break;
				case PASTESKIP_LATESTANDOLDEST:
					r |= PASTE_LATESTANDOLDESTSKIP;
					break;
				default:
					break;
			}
			EndDialog(r);
		}
	}

	void paste_dialog::OnClose() {
		EndDialog(-1);
	}

	/* XML Dialog */
	xml_dialog::~xml_dialog() {
		if(m_hWnd) {
			delete[] m_hWnd;
		}
	}

	BOOL xml_dialog::OnInitDialog(CWindow, LPARAM) {
		m_dark.AddDialogWithControls(m_hWnd);
		
		InitXmlControls(m_hWnd);

		CreateTooltip(tooltips[0], m_hWnd, IDC_XML_DISABLE_DIALOG);
		CreateTooltip(tooltips[1], m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP);
		CreateTooltip(tooltips[2], m_hWnd, IDC_XML_EXPORT_DATE_TIME);
		CreateTooltip(tooltips[3], m_hWnd, IDC_XML_EXPORT_NEXT_COUNT);
		CreateTooltip(tooltips[4], m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT);
		CreateTooltip(tooltips[5], m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT);
		CreateTooltip(tooltips[6], m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT);
		CreateTooltip(tooltips[7], m_hWnd, IDC_XML_EXPORT_LATEST_SKIP);
		CreateTooltip(tooltips[8], m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP);
		CreateTooltip(tooltips[9], m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT);
		CreateTooltip(tooltips[10], m_hWnd, IDC_XML_EXPORT_ALL_SKIPS);

		return TRUE;
	}

	void xml_dialog::OnSave(UINT, int, CWindow) {
		cfg_xmlDisableDialog = uButton_GetCheck(m_hWnd, IDC_XML_DISABLE_DIALOG);
		cfg_xmlExportJSTimestamp = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP);
		cfg_xmlExportDateTime = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_DATE_TIME);
		cfg_xmlExportNextCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_NEXT_COUNT);
		cfg_xmlExportRandomCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT);
		cfg_xmlExportPreviousCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT);
		cfg_xmlExportTotalCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT);
		cfg_xmlExportLatestSkip = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_LATEST_SKIP);
		cfg_xmlExportOldestSkip = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP);
		cfg_xmlExportTimestampCount = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT);
		cfg_xmlExportAllTimestamps = uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_ALL_SKIPS);
	}

	void xml_dialog::OnClose(UINT, int ctrlID, CWindow) {
		if(ctrlID == IDC_CANCEL) {
			EndDialog(-1);
		}
		else if (ctrlID == IDC_OK) {
			cfg_xmlDisableDialog = uButton_GetCheck(m_hWnd, IDC_XML_DISABLE_DIALOG);
			t_uint32 r = 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_JS_TIMESTAMP) ? EXPORT_JSTIMESTAMP : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_DATE_TIME) ? EXPORT_DATETIME : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TOTAL_COUNT) ? EXPORT_TOTALSKIPCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_NEXT_COUNT) ? EXPORT_NEXTCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_RANDOM_COUNT) ? EXPORT_RANDOMCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_PREVIOUS_COUNT) ? EXPORT_PREVIOUSCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_LATEST_SKIP) ? EXPORT_LATESTSKIP : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_OLDEST_SKIP) ? EXPORT_OLDESTSKIP : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_TIMESTAMP_COUNT) ? EXPORT_TIMESTAMPCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_XML_EXPORT_ALL_SKIPS) ? EXPORT_ALLSKIPS : 0;
			EndDialog(r);
		}
	}

	void xml_dialog::OnClose() {
		EndDialog(-1);
	}

	/* Tag Dialog */
	tag_dialog::~tag_dialog() {
		if(m_hWnd) {
			delete[] m_hWnd;
		}
	}

	BOOL tag_dialog::OnInitDialog(CWindow, LPARAM) {
		m_dark.AddDialogWithControls(m_hWnd);

		InitTagControls(m_hWnd);

		CreateTooltip(tooltips[0], m_hWnd, IDC_TAG_DISABLE_DIALOG);
		CreateTooltip(tooltips[1], m_hWnd, IDC_TAG_NEXT_COUNT);
		CreateTooltip(tooltips[2], m_hWnd, IDC_TAG_RANDOM_COUNT);
		CreateTooltip(tooltips[3], m_hWnd, IDC_TAG_PREVIOUS_COUNT);
		CreateTooltip(tooltips[4], m_hWnd, IDC_TAG_CURRENT_TIMESTAMP);
		CreateTooltip(tooltips[5], m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW);
		CreateTooltip(tooltips[6], m_hWnd, IDC_TAG_TIMESTAMP_DELIMITER);

		return TRUE;
	}

	void tag_dialog::OnSave(UINT, int, CWindow) {
		cfg_tagDisableDialog = uButton_GetCheck(m_hWnd, IDC_TAG_DISABLE_DIALOG);
		cfg_tagNextCount = uButton_GetCheck(m_hWnd, IDC_TAG_NEXT_COUNT);
		cfg_tagRandomCount = uButton_GetCheck(m_hWnd, IDC_TAG_RANDOM_COUNT);
		cfg_tagPreviousCount = uButton_GetCheck(m_hWnd, IDC_TAG_PREVIOUS_COUNT);
		cfg_tagCurrentTimestamp = uButton_GetCheck(m_hWnd, IDC_TAG_CURRENT_TIMESTAMP);
		cfg_tagAllTimestampsRaw = uButton_GetCheck(m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW);
		cfg_tagTimestampDelimiter = (t_int32)uSendDlgItemMessage(IDC_TAG_TIMESTAMP_DELIMITER, CB_GETCURSEL);

	}

	void tag_dialog::OnClose(UINT, int ctrlID, CWindow) {
		if(ctrlID == IDC_CANCEL) {
			EndDialog(-1);
		}
		else if(ctrlID == IDC_OK) {
			cfg_tagDisableDialog = uButton_GetCheck(m_hWnd, IDC_TAG_DISABLE_DIALOG);
			t_int32 r = 0;
			r |= uButton_GetCheck(m_hWnd, IDC_TAG_NEXT_COUNT) ? TAG_NEXTCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_TAG_RANDOM_COUNT) ? TAG_RANDOMCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_TAG_PREVIOUS_COUNT) ? TAG_PREVIOUSCOUNT : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_TAG_CURRENT_TIMESTAMP) ? TAG_CURRENTTIME : 0;
			r |= uButton_GetCheck(m_hWnd, IDC_TAG_ALL_TIMESTAMPS_RAW) ? TAG_ALLSKIPS : 0;
			switch((t_int32)uSendDlgItemMessage(IDC_TAG_TIMESTAMP_DELIMITER, CB_GETCURSEL)) {
				case TAG_TIMESTAMP_SEMICOLON:
					r |= TAG_DELIMIT_SEMICOLON;
					break;
				case TAG_TIMESTAMP_COMMA:
					r |= TAG_DELIMIT_COMMA;
					break;
				case TAG_TIMESTAMP_FORWARDSLASH:
					r |= TAG_DELIMIT_FORWARDSLASH;
					break;
				case TAG_TIMESTAMP_SPACE:
					r |= TAG_DELIMIT_SPACE;
					break;
				case TAG_TIMESTAMP_COLON:
					r |= TAG_DELIMIT_COLON;
					break;
				case TAG_TIMESTAMP_PIPE:
					r |= TAG_DELIMIT_PIPE;
					break;
				case TAG_TIMESTAMP_AMPERSAND:
					r |= TAG_DELIMIT_AMPERSAND;
					break;
				default:
					break;
			}
			EndDialog(r);
		}
	}

	void tag_dialog::OnClose() {
		EndDialog(-1);
	}

	const char* my_preferences_page::get_name() {
		return "Skip Count";
	}
#endif // _WIN32
} // namespace foo_skipcount
