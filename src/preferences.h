#ifndef SKIPCOUNT_PREFERENCES_H
#define SKIPCOUNT_PREFERENCES_H

#include <array>

#include "globals.h"

#ifdef _WIN32
#include "resource.h"
#include <helpers/atl-misc.h>
#include <helpers/DarkMode.h>
#endif // _WIN32

namespace foo_skipcount {
	// Skip logging conditions
	enum logConditionEnum {
		LOGCONDITION_TIME = 0,
		LOGCONDITION_PERCENT = 1,
		LOGCONDITION_TIMEANDPERCENT = 2,
		LOGCONDITION_TOTAL
	};
	static const TCHAR* logConditionStrings[] = {
		TEXT("Time"),
		TEXT("Percent"),
		TEXT("Time and Percent")
	};

	// Skip logging type
	enum logTypeEnum {
		LOGTYPE_NONE = 0,
		LOGTYPE_LATEST = 1,
		LOGTYPE_ALL = 2,
		LOGTYPE_TOTAL
	};
	static const TCHAR* logTypeStrings[] = {
		TEXT("None"),
		TEXT("Latest"),
		TEXT("All")
	};

	// Copy/Paste skip collection type
	enum pasteSkipSelectionEnum {
		PASTESKIP_ALL = 0,
		PASTESKIP_NONE = 1,
		PASTESKIP_LATEST = 2,
		PASTESKIP_OLDEST = 3,
		PASTESKIP_ALLBUTOLDEST = 4,
		PASTESKIP_ALLBUTLATEST = 5,
		PASTESKIP_LATESTANDOLDEST = 6,
		PASTESKIP_TOTAL
	};
	static const TCHAR* pasteSkipSelectionStrings[] = {
		TEXT("All"),
		TEXT("None"),
		TEXT("Latest"),
		TEXT("Oldest"),
		TEXT("All but oldest"),
		TEXT("All but latest"),
		TEXT("Latest and oldest")
	};
	enum PasteFlags {
		PASTE_NEXTCOUNT = 1 << 0,
		PASTE_RANDOMCOUNT = 1 << 1,
		PASTE_PREVIOUSCOUNT = 1 << 2,
		PASTE_ALLSKIPS = 1 << 3,
		PASTE_NOSKIPS = 1 << 4,
		PASTE_LATESTSKIP = 1 << 5,
		PASTE_OLDESTSKIP = 1 << 6,
		PASTE_ALLBUTOLDESTSKIP = 1 << 7,
		PASTE_ALLBUTLATESTSKIP = 1 << 8,
		PASTE_LATESTANDOLDESTSKIP = 1 << 9
	};

	// Tag timestamp delimiter
	enum tagTimestampDelimiterEnum {
		TAG_TIMESTAMP_SEMICOLON = 0,
		TAG_TIMESTAMP_COMMA = 1,
		TAG_TIMESTAMP_FORWARDSLASH = 2,
		TAG_TIMESTAMP_SPACE = 3,
		TAG_TIMESTAMP_COLON = 4,
		TAG_TIMESTAMP_PIPE = 5,
		TAG_TIMESTAMP_AMPERSAND = 6,
		TAG_TIMESTAMP_TOTAL
	};
	static const TCHAR* tagTimestampDelimiterLabels[] = {
		TEXT("Semicolon \"; \""),
		TEXT("Comma \", \""),
		TEXT("Forward slash \" / \""),
		TEXT("Space \" \""),
		TEXT("Colon \":\""),
		TEXT("Pipe \"|\""),
		TEXT("Ampersand \"&\"")
	};
	static const std::string tagTimestampDelimiters[] = {
		"; ",
		", ",
		" / ",
		" ",
		":",
		"|",
		"&"
	};
	static const std::string AllDelimiterCharacters = " ;,/:|&";

	enum TagFlags {
		TAG_NEXTCOUNT = 1 << 0,
		TAG_RANDOMCOUNT = 1 << 1,
		TAG_PREVIOUSCOUNT = 1 << 2,
		TAG_CURRENTTIME = 1 << 3,
		TAG_ALLSKIPS = 1 << 4,
		TAG_DELIMIT_SEMICOLON = 1 << 5,
		TAG_DELIMIT_COMMA = 1 << 6,
		TAG_DELIMIT_FORWARDSLASH = 1 << 7,
		TAG_DELIMIT_SPACE = 1 << 8,
		TAG_DELIMIT_COLON = 1 << 9,
		TAG_DELIMIT_PIPE = 1 << 10,
		TAG_DELIMIT_AMPERSAND = 1 << 11,
	};

	static const std::unordered_map<std::string, std::string> tagMetaFieldString = {
		{ "next", "SKIP_COUNT_NEXT" },
		{ "random", "SKIP_COUNT_RANDOM" },
		{ "previous", "SKIP_COUNT_PREVIOUS" },
		{ "current", "SKIP_COUNT_TAG_TIMESTAMP" },
		{ "times", "SKIP_TIMESTAMPS_RAW" }
	};

	// XML
	enum ExportFlags {
		EXPORT_JSTIMESTAMP = 1 << 0,
		EXPORT_DATETIME = 1 << 1,
		EXPORT_TOTALSKIPCOUNT = 1 << 2,
		EXPORT_NEXTCOUNT = 1 << 3,
		EXPORT_RANDOMCOUNT = 1 << 4,
		EXPORT_PREVIOUSCOUNT = 1 << 5,
		EXPORT_LATESTSKIP = 1 << 6,
		EXPORT_OLDESTSKIP = 1 << 7,
		EXPORT_TIMESTAMPCOUNT = 1 << 8,
		EXPORT_ALLSKIPS = 1 << 9,
	};

	static const std::unordered_map<t_uint, std::array<std::wstring, 2>> ToolTipStrings = {
		{ IDC_COUNT_NEXT, { L"Next Song command increments skip count", L"Using a next song button or shortcut before the conditions are met will count as a skip." } },
		{ IDC_COUNT_RANDOM, { L"Random Song command increments skip count", L"Using a random song button or shortcut before the conditions are met will count as a skip." } },
		{ IDC_COUNT_PREVIOUS, { L"Previous Song command increments skip count", L"Using a previous song button or shortcut before the conditions are met will count as a skip." } },
		{ IDC_COUNT_FROM_STOP, { L"Count skips when using commands after stopping playback", L"Commands used after the user has manually stopped playback can increment skips." } },
		{ IDC_COUNT_FROM_PAUSE, { L"Count skips when using commands after pausing playback", L"Commands used after the user has manually paused playback can increment skips." } },
		{ IDC_CONDITION, { L"Condition when skips are no longer counted", L"'Time' counts skips before the seconds set. 'Percentage' counts skips before the percentage of the track has played. 'Time and Percent' requires both conditions to be met before skips are no longer tracked." } },
		{ IDC_TIME, { L"Duration of track to play, in seconds, before not counting skips", L"A number from 0 to 1 billion, exclusive. Default = 5" } },
		{ IDC_PERCENT, { L"Percentage of track to play before not counting skips", L"A number from 0 to 100. Default = 5" } },
		{ IDC_LOG_SKIP_TIMES, { L"Timestamps logged on a skip", L"'None' logs no skips. 'Latest' only updates the latest skip: %last_skip% or %latest_skip%. 'All' logs every skip: % skip_times_raw% and %skip_times_js%." } },
		{ IDC_CONFIRM_CONTEXT_CLEAR, { L"Display a confimation dialog for clearing skip timestamps", L"Only applies to context command menu, disabled by default." } },
		{ IDC_PROTECTION_NEXT, { L"Offset when skips can increment with next song commands", L"Using a Next Song command before the defined time will not increase the skip count. The duration into the track where a skip counts will be shifted by the defined time." } },
		{ IDC_PROTECTION_RANDOM, { L"Offset when skips can increment with random song commands", L"Using a Random Song command before the defined time will not increase the skip count. The duration into the track where a skip counts will be shifted by the defined time." } },
		{ IDC_PROTECTION_PREVIOUS, { L"Offset when skips can increment with previous song commands", L"Using a Previous Song command before the defined time will not increase the skip count. The duration into the track where a skip counts will be shifted by the defined time." } },
		{ IDC_PASTE_DISABLE_DIALOG, { L"Disable the paste setting dialog pop up when pasting skip statistics", L"When disabled the settings in the preferences page are automatically used." } },
		{ IDC_PASTE_NEXT_COUNT, { L"Paste the copied 'Next' skip count", L"" } },
		{ IDC_PASTE_RANDOM_COUNT, { L"Paste the copied 'Random' skip count", L"" } },
		{ IDC_PASTE_PREVIOUS_COUNT, { L"Paste the copied 'Previous' skip count", L"" } },
		{ IDC_PASTE_SKIP_SELECTION, { L"Selection of skip timestamps to paste", L"'All', 'None', only the 'Latest', only the 'Oldest', 'All but oldest', 'All but latest', only the 'Latest and Oldest'." } },
		{ IDC_XML_DISABLE_DIALOG, { L"Disable the xml setting dialog pop up when exporting to xml", L"When disabled the settings in the preferences page are automatically used." } },
		{ IDC_XML_EXPORT_JS_TIMESTAMP, { L"Export javascript-style timestamps", L"By default, timestamps are exported as Unix epoch timestamps." } },
		{ IDC_XML_EXPORT_DATE_TIME, { L"Export timestamps as dates and times: YYYY-MM-DD HH:MM:SS", L"By default, timestamps are exported as Unix epoch timestamps. This only applies to exporting all skip timestamps." } },
		{ IDC_XML_EXPORT_NEXT_COUNT, { L"Export the 'Next' skip count as an attribute", L"" } },
		{ IDC_XML_EXPORT_RANDOM_COUNT, { L"Export the 'Random' skip count as an attribute", L"" } },
		{ IDC_XML_EXPORT_PREVIOUS_COUNT, { L"Export the 'Previous' skip count as an attribute", L"" } },
		{ IDC_XML_EXPORT_TOTAL_COUNT, { L"Export the sum of all 3 skip counts as an attribute", L"" } },
		{ IDC_XML_EXPORT_LATEST_SKIP, { L"Export the most recent skip timestamp as an attribute", L"" } },
		{ IDC_XML_EXPORT_OLDEST_SKIP, { L"Export the oldest skip timestamp as an attribute", L"" } },
		{ IDC_XML_EXPORT_TIMESTAMP_COUNT, { L"Export the amount of skip timestamps as an attribute", L"" } },
		{ IDC_XML_EXPORT_ALL_SKIPS, { L"Export every skip timestamp", L"Date-time setting is prioritized over JS timestamps." } },
		{ IDC_TAG_DISABLE_DIALOG, { L"Disable the tag setting dialog pop up when writing or importing skip statistics", L"When disabled the settings in the preferences page are automatically used." } },
		{ IDC_TAG_NEXT_COUNT, { L"Write the 'Next' skip count as a tag", L"" } },
		{ IDC_TAG_RANDOM_COUNT, { L"Write the 'Random' skip count as a tag", L"" } },
		{ IDC_TAG_PREVIOUS_COUNT, { L"Write the 'Previous' skip count as a tag", L"" } },
		{ IDC_TAG_CURRENT_TIMESTAMP, { L"Write the current time timestamp as a tag", L"Used to track when the statistics were written." } },
		{ IDC_TAG_ALL_TIMESTAMPS_RAW, { L"Write all skip timestamps as a tag", L"WARNING: This could increase filesize and be very resource intensive." } },
		{ IDC_TAG_TIMESTAMP_DELIMITER, { L"Choose the delimiter between timestamps in the tag", L"The characters between the quotation marks, including spaces, is used." } },
	};

	// GUIDs for settings and their storage within the component's configuration file.
	// {2DCFE159-0683-4AD5-9F10-D612577BA137}
	static constexpr GUID guid_cfg_countNext = { 0x2dcfe159, 0x0683, 0x4ad5, { 0x9f, 0x10, 0xd6, 0x12, 0x57, 0x7b, 0xa1, 0x37 } };
	// {30D15AC1-C56B-40DB-88B2-9B0C0C1C8B17}
	static constexpr GUID guid_cfg_countRandom = { 0x30d15ac1, 0xc56b, 0x40db, { 0x88, 0xb2, 0x9b, 0x0c, 0x0c, 0x1c, 0x8b, 0x17 } };
	// {1AA63FCC-223A-4732-B778-1F94A0C0B91F}
	static constexpr GUID guid_cfg_countPrevious = { 0x1aa63fcc, 0x223a, 0x4732, { 0xb7, 0x78, 0x1f, 0x94, 0xa0, 0xc0, 0xb9, 0x1f } };
	// {9C746A7C-24DF-450A-BEBF-F63F14C6BF57}
	static constexpr GUID guid_cfg_countFromStop = { 0x9c746a7c, 0x24df, 0x450a, { 0xbe, 0xbf, 0xf6, 0x3f, 0x14, 0xc6, 0xbf, 0x57 } };
	// {C2A71EB9-E63D-433B-84CF-0BE30DFD47B0}
	static constexpr GUID guid_cfg_countFromPause = { 0xc2a71eb9, 0xe63d, 0x433b, { 0x84, 0xcf, 0x0b, 0xe3, 0x0d, 0xfd, 0x47, 0xb0 } };
	// {74ACC430-FF12-4BC8-846E-2882916FFF90}
	static constexpr GUID guid_cfg_condition = { 0x74acc430, 0xff12, 0x4bc8, { 0x84, 0x6e, 0x28, 0x82, 0x91, 0x6f, 0xff, 0x90 } };
	// {F6AA9C8A-616F-410C-AE18-F848C9F7DF87}
	static constexpr GUID guid_cfg_time = { 0xf6aa9c8a, 0x616f, 0x410c, { 0xae, 0x18, 0xf8, 0x48, 0xc9, 0xf7, 0xdf, 0x87 } };
	// {27D73FD1-D9B6-403A-B705-0B00D6952229}
	static constexpr GUID guid_cfg_percent = { 0x27d73fd1, 0xd9b6, 0x403a, { 0xb7, 0x05, 0x0b, 0x00, 0xd6, 0x95, 0x22, 0x29 } };
	// {4563859D-0FEF-44DA-85FC-5E67CD43944E}
	static constexpr GUID guid_cfg_percent_slider = { 0x4563859d, 0x0fef, 0x44da, { 0x85, 0xfc, 0x5e, 0x67, 0xcd, 0x43, 0x94, 0x4e } };
	// {99F52995-6A9D-48C8-8C10-EFC0DCCA0C09}
	static constexpr GUID guid_cfg_logSkipTimes = { 0x99f52995, 0x6a9d, 0x48c8, { 0x8c, 0x10, 0xef, 0xc0, 0xdc, 0xca, 0x0c, 0x09 } };
	// {418208D6-B7C6-4E66-B703-07C238830217}
	static constexpr GUID guid_cfg_confirmContextClear = { 0x418208d6, 0xb7c6, 0x4e66, { 0xb7, 0x03, 0x07, 0xc2, 0x38, 0x83, 0x02, 0x17 } };
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
	// {49485545-EA16-4376-915F-4FED4C1500AA}
	static constexpr GUID guid_cfg_pasteDisableDialog = { 0x49485545, 0xea16, 0x4376, { 0x91, 0x5f, 0x4f, 0xed, 0x4c, 0x15, 0x00, 0xaa } };
	// {B7D7E16A-4CA2-428E-9D45-85159276532E}
	static constexpr GUID guid_cfg_pasteNextCount = { 0xb7d7e16a, 0x4ca2, 0x428e, { 0x9d, 0x45, 0x85, 0x15, 0x92, 0x76, 0x53, 0x2e } };
	// {522C912E-A61E-4E7B-82D2-DE6D1154709B}
	static constexpr GUID guid_cfg_pasteRandomCount = { 0x522c912e, 0xa61e, 0x4e7b, { 0x82, 0xd2, 0xde, 0x6d, 0x11, 0x54, 0x70, 0x9b } };
	// {6227173B-BEE7-4A59-A4B0-DD38ED47EB0D}
	static constexpr GUID guid_cfg_pastePreviousCount = { 0x6227173b, 0xbee7, 0x4a59, { 0xa4, 0xb0, 0xdd, 0x38, 0xed, 0x47, 0xeb, 0x0d } };
	// {ADA6BE04-0406-4895-B0C3-83E46A1A71EC}
	static constexpr GUID guid_cfg_pasteSkipSelection = { 0xada6be04, 0x0406, 0x4895, { 0xb0, 0xc3, 0x83, 0xe4, 0x6a, 0x1a, 0x71, 0xec } };
	// {08B179BC-9E34-4689-B568-DD94DC41F16C}
	static constexpr GUID guid_cfg_xmlDisableDialog = { 0x08b179bc, 0x9e34, 0x4689, { 0xb5, 0x68, 0xdd, 0x94, 0xdc, 0x41, 0xf1, 0x6c } };
	// {86F2807C-FB88-450E-A283-BE31F4183F73}
	static constexpr GUID guid_cfg_xmlExportJSTimestamp = { 0x86f2807c, 0xfb88, 0x450e, { 0xa2, 0x83, 0xbe, 0x31, 0xf4, 0x18, 0x3f, 0x73 } };
	// {D632E6B5-2191-451F-9619-4F0E3EABA10A}
	static constexpr GUID guid_cfg_xmlExportDateTime = { 0xd632e6B5, 0x2191, 0x451f, { 0x96, 0x19, 0x4f, 0x0e, 0x3e, 0xab, 0xa1, 0x0a } };
	// {31307F07-75BA-410C-A6E6-CFEED49D8459}
	static constexpr GUID guid_cfg_xmlExportNextCount = { 0x31307f07, 0x75ba, 0x410c, { 0xa6, 0xe6, 0xcf, 0xee, 0xd4, 0x9d, 0x84, 0x59 } };
	// {83E37962-3944-4E86-A7B1-9F8693FABA8B}
	static constexpr GUID guid_cfg_xmlExportRandomCount = { 0x83e37962, 0x3944, 0x4e86, { 0xa7, 0xb1, 0x9f, 0x86, 0x93, 0xfa, 0xba, 0x8b } };
	// {275A0A28-E11C-41E5-BC1A-E3C472656590}
	static constexpr GUID guid_cfg_xmlExportPreviousCount = { 0x275a0a28, 0xe11c, 0x41e5, { 0xbc, 0x1a, 0xe3, 0xc4, 0x72, 0x65, 0x65, 0x90 } };
	// {C9F94630-A34C-4EA9-86F6-D4B992D7CD6D}
	static constexpr GUID guid_cfg_xmlExportTotalCount = { 0xc9f94630, 0xa34c, 0x4ea9, { 0x86, 0xf6, 0xd4, 0xb9, 0x92, 0xd7, 0xcd, 0x6d } };
	// {2BEA4ACF-87E5-48F1-879F-A3619A59722A}
	static constexpr GUID guid_cfg_xmlExportLatestSkip = { 0x2bea4acf, 0x87e5, 0x48f1, { 0x87, 0x9f, 0xa3, 0x61, 0x9a, 0x59, 0x72, 0x2a } };
	// {BA8213E5-C37F-44C4-BFC3-20A82F4E70A7}
	static constexpr GUID guid_cfg_xmlExportOldestSkip = { 0xba8213e5, 0xc37f, 0x44c4, { 0xbf, 0xc3, 0x20, 0xa8, 0x2f, 0x4e, 0x70, 0xa7 } };
	// {340CEB5B-C9E1-4F58-B026-934979475961}
	static constexpr GUID guid_cfg_xmlExportTimestampCount = { 0x340ceb5b, 0xc9e1, 0x4f58, { 0xb0, 0x26, 0x93, 0x49, 0x79, 0x47, 0x59, 0x61 } };
	// {7209DDE1-6FB9-4CAC-9613-83B940B6627D}
	static constexpr GUID guid_cfg_xmlExportAllTimestamps = { 0x7209dde1, 0x6fb9, 0x4cac, { 0x96, 0x13, 0x83, 0xb9, 0x40, 0xb6, 0x62, 0x7d } };
	// {2E621E4D-9C2F-44A5-A913-BDE2EACBE028}
	static constexpr GUID guid_cfg_tagDisableDialog = { 0x2e621e4d, 0x9c2f, 0x44a5, { 0xa9, 0x13, 0xbd, 0xe2, 0xea, 0xcb, 0xe0, 0x28 } };
	// {D9000548-FDC6-4D73-92FF-B3A5464104F4}
	static constexpr GUID guid_cfg_tagNextCount = { 0xd9000548, 0xfdc6, 0x4d73, { 0x92, 0xff, 0xb3, 0xa5, 0x46, 0x41, 0x04, 0xf4 } };
	// {399592AC-FDF6-48D9-8120-00F98AFCA33B}
	static constexpr GUID guid_cfg_tagRandomCount = { 0x399592ac, 0xfdf6, 0x48d9, { 0x81, 0x20, 0x00, 0xf9, 0x8a, 0xfc, 0xa3, 0x3b } };
	// {4BA40669-96E4-4BE6-9455-B9B957825E4F}
	static constexpr GUID guid_cfg_tagPreviousCount = { 0x4ba40669, 0x96e4, 0x4be6, { 0x94, 0x55, 0xb9, 0xb9, 0x57, 0x82, 0x5e, 0x4f } };
	// {09C4DD56-4FC2-4543-883D-016467A114CB}
	static constexpr GUID guid_cfg_tagCurrentTimestamp = { 0x09c4dd56, 0x4fc2, 0x4543, { 0x88, 0x3d, 0x01, 0x64, 0x67, 0xa1, 0x14, 0xcb } };
	// {DF44C5ED-4502-47E9-B854-CD9097895546}
	static constexpr GUID guid_cfg_tagAllTimestampsRaw = { 0xdf44c5ed, 0x4502, 0x47e9, { 0xb8, 0x54, 0xcd, 0x90, 0x97, 0x89, 0x55, 0x46 } };
	// {08B82162-8D64-468A-A5A0-FD65A9414AB1}
	static constexpr GUID guid_cfg_tagTimestampDelimiter = { 0x08b82162, 0x8d64, 0x468a, { 0xa5, 0xa0, 0xfd, 0x65, 0xa9, 0x41, 0x4a, 0xb1 } };

	// Config
	extern cfg_bool cfg_countNext,
		cfg_countRandom,
		cfg_countPrevious,
		cfg_countFromStop,
		cfg_countFromPause,
		cfg_confirmContextClear,
		cfg_skipProtectionNext,
		cfg_skipProtectionRandom,
		cfg_skipProtectionPrevious,
		cfg_pasteDisableDialog,
		cfg_pasteNextCount,
		cfg_pasteRandomCount,
		cfg_pastePreviousCount,
		cfg_xmlDisableDialog,
		cfg_xmlExportJSTimestamp,
		cfg_xmlExportDateTime,
		cfg_xmlExportNextCount,
		cfg_xmlExportRandomCount,
		cfg_xmlExportPreviousCount,
		cfg_xmlExportTotalCount,
		cfg_xmlExportLatestSkip,
		cfg_xmlExportOldestSkip,
		cfg_xmlExportTimestampCount,
		cfg_xmlExportAllTimestamps,
		cfg_tagDisableDialog,
		cfg_tagNextCount,
		cfg_tagRandomCount,
		cfg_tagPreviousCount,
		cfg_tagCurrentTimestamp,
		cfg_tagAllTimestampsRaw;

	extern cfg_uint	cfg_condition,
		cfg_time,
		cfg_percent,
		cfg_logSkipTimes,
		cfg_skipProtectionNextTime,
		cfg_skipProtectionRandomTime,
		cfg_skipProtectionPreviousTime,
		cfg_pasteSkipSelection,
		cfg_tagTimestampDelimiter;

	// Defaults
	static const bool default_cfg_countNext = false,
		default_cfg_countRandom = false,
		default_cfg_countPrevious = true,
		default_cfg_countFromStop = false,
		default_cfg_countFromPause = true,
		default_cfg_confirmContextClear = false,
		default_cfg_skipProtectionNext = false,
		default_cfg_skipProtectionRandom = false,
		default_cfg_skipProtectionPrevious = true,
		default_cfg_pasteDisableDialog = false,
		default_cfg_pasteNextCount = true,
		default_cfg_pasteRandomCount = true,
		default_cfg_pastePreviousCount = true,
		default_cfg_xmlDisableDialog = false,
		default_cfg_xmlExportJSTimestamp = false,
		default_cfg_xmlExportDateTime = false,
		default_cfg_xmlExportNextCount = true,
		default_cfg_xmlExportRandomCount = true,
		default_cfg_xmlExportPreviousCount = true,
		default_cfg_xmlExportTotalCount = true,
		default_cfg_xmlExportLatestSkip = true,
		default_cfg_xmlExportOldestSkip = true,
		default_cfg_xmlExportTimestampCount = true,
		default_cfg_xmlExportAllTimestamps = true,
		default_cfg_tagDisableDialog = false,
		default_cfg_tagNextCount = true,
		default_cfg_tagRandomCount = true,
		default_cfg_tagPreviousCount = true,
		default_cfg_tagCurrentTimestamp = true,
		default_cfg_tagAllTimestampsRaw = false;

	static const t_uint default_cfg_condition = 0,
		default_cfg_time = 5,
		default_cfg_percent = 5,
		default_cfg_logSkipTimes = 0,
		default_cfg_skipProtectionNextTime = 1,
		default_cfg_skipProtectionRandomTime = 1,
		default_cfg_skipProtectionPreviousTime = 1,
		default_cfg_pasteSkipSelection = 0,
		default_cfg_tagTimestampDelimiter = 0;

	// Control: Minimum accepted value, value to set when below the minimum, maximum accepted value, value to set when above the maximum
	static const std::unordered_map<t_uint, std::array<t_uint, 4>> EditControlBoundsDefaults = {
		{ IDC_TIME, { 0, default_cfg_time, 999999999, default_cfg_time } },
		{ IDC_PERCENT, { 0, default_cfg_percent, 100, 100 } },
		{ IDC_PROTECTION_NEXT_TIME, { 0, default_cfg_skipProtectionNextTime, 999999999, default_cfg_skipProtectionNextTime } },
		{ IDC_PROTECTION_RANDOM_TIME, { 0, default_cfg_skipProtectionRandomTime, 999999999, default_cfg_skipProtectionRandomTime } },
		{ IDC_PROTECTION_PREVIOUS_TIME, { 0, default_cfg_skipProtectionPreviousTime, 999999999, default_cfg_skipProtectionPreviousTime } },
	};

#ifdef _WIN32
	t_uint32 GetIntConformedToBounds(HWND, t_int32);
	void InitPasteControls(HWND);
	void InitXmlControls(HWND);
	void InitTagControls(HWND);
	void CreateTooltip(CToolTipCtrl, CWindow, int);

	class my_preferences : public CDialogImpl<my_preferences>, public preferences_page_instance {
	public:
		// Constructor
		my_preferences(preferences_page_callback::ptr callback) : m_callback(callback) {}
		// No Destructor: Host ensures that the dialog is destroyed first then the last reference to this object is released, thus deleted.
		static int const IDD = IDD_PREFERENCES;

		// preferences_page_instance methods
		t_uint32 get_state() override;
		void apply() override;
		void reset() override;

		// WTL message map
		BEGIN_MSG_MAP(my_preferences)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_COUNT_NEXT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_COUNT_RANDOM, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_COUNT_PREVIOUS, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_STOP, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_COUNT_FROM_PAUSE, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_CONDITION, CBN_SELCHANGE, OnSelectionChange)
			COMMAND_HANDLER_EX(IDC_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_TIME, EN_KILLFOCUS, OnFocusLost)
			COMMAND_HANDLER_EX(IDC_PERCENT, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_PERCENT, EN_KILLFOCUS, OnFocusLost)
			MSG_WM_HSCROLL(OnHScrollChange)
			COMMAND_HANDLER_EX(IDC_LOG_SKIP_TIMES, CBN_SELCHANGE, OnSelectionChange)
			COMMAND_HANDLER_EX(IDC_CONFIRM_CONTEXT_CLEAR, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_NEXT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_NEXT_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_NEXT_TIME, EN_KILLFOCUS, OnFocusLost)
			COMMAND_HANDLER_EX(IDC_PROTECTION_RANDOM, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_RANDOM_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_RANDOM_TIME, EN_KILLFOCUS, OnFocusLost)
			COMMAND_HANDLER_EX(IDC_PROTECTION_PREVIOUS, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_PREVIOUS_TIME, EN_CHANGE, OnEditChange)
			COMMAND_HANDLER_EX(IDC_PROTECTION_PREVIOUS_TIME, EN_KILLFOCUS, OnFocusLost)
			COMMAND_HANDLER_EX(IDC_PASTE_DISABLE_DIALOG, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PASTE_NEXT_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PASTE_RANDOM_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PASTE_PREVIOUS_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_PASTE_SKIP_SELECTION, CBN_SELCHANGE, OnSelectionChange)
			COMMAND_HANDLER_EX(IDC_XML_DISABLE_DIALOG, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_JS_TIMESTAMP, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_DATE_TIME, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_NEXT_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_RANDOM_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_PREVIOUS_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_TOTAL_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_LATEST_SKIP, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_OLDEST_SKIP, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_TIMESTAMP_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_XML_EXPORT_ALL_SKIPS, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_DISABLE_DIALOG, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_NEXT_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_RANDOM_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_PREVIOUS_COUNT, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_CURRENT_TIMESTAMP, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_ALL_TIMESTAMPS_RAW, BN_CLICKED, OnButtonChange)
			COMMAND_HANDLER_EX(IDC_TAG_TIMESTAMP_DELIMITER, CBN_SELCHANGE, OnSelectionChange)
		END_MSG_MAP()

	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void OnButtonChange(UINT, int, CWindow);
		void OnSelectionChange(UINT, int, CWindow);
		void OnEditChange(UINT, int, CWindow);
		BOOL OnFocusLost(UINT, int, CWindow);
		void OnHScrollChange(UINT, LPARAM, CScrollBar);
		bool HasChanged();
		void OnChanged();

		CToolTipCtrl tooltips[36];
		const preferences_page_callback::ptr m_callback;
		// Dark mode hooks object, must be a member of dialog class.
		fb2k::CDarkModeHooks m_dark;
	};

	class paste_dialog : public CDialogImpl<paste_dialog> {
	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void OnSave(UINT, int, CWindow);
		void OnClose(UINT, int, CWindow);
		void OnClose();
	public:
		paste_dialog() : m_bMsgHandled(FALSE) {}
		~paste_dialog();

		static int const IDD = IDD_PASTEFORM;

		// WTL message map
		BEGIN_MSG_MAP_EX(paste_dialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_SAVE, BN_CLICKED, OnSave)
			COMMAND_HANDLER_EX(IDC_OK, BN_CLICKED, OnClose)
			COMMAND_HANDLER_EX(IDC_CANCEL, BN_CLICKED, OnClose)
			MSG_WM_CLOSE(OnClose)
		END_MSG_MAP()
	private:
		CToolTipCtrl tooltips[5];
		// Dark mode hooks object, must be a member of dialog class.
		fb2k::CDarkModeHooks m_dark;
	};

	class xml_dialog : public CDialogImpl<xml_dialog> {
	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void OnSave(UINT, int, CWindow);
		void OnClose(UINT, int, CWindow);
		void OnClose();
	public:
		xml_dialog() : m_bMsgHandled(FALSE) {}
		~xml_dialog();

		static int const IDD = IDD_XMLFORM;

		// WTL message map
		BEGIN_MSG_MAP_EX(xml_dialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_SAVE, BN_CLICKED, OnSave)
			COMMAND_HANDLER_EX(IDC_OK, BN_CLICKED, OnClose)
			COMMAND_HANDLER_EX(IDC_CANCEL, BN_CLICKED, OnClose)
			MSG_WM_CLOSE(OnClose)
		END_MSG_MAP()

	private:
		CToolTipCtrl tooltips[11];
		// Dark mode hooks object, must be a member of dialog class.
		fb2k::CDarkModeHooks m_dark;
	};

	class tag_dialog : public CDialogImpl<tag_dialog> {
	private:
		BOOL OnInitDialog(CWindow, LPARAM);
		void OnSave(UINT, int, CWindow);
		void OnClose(UINT, int, CWindow);
		void OnClose();
	public:
		tag_dialog() : m_bMsgHandled(FALSE) {}
		~tag_dialog();

		static int const IDD = IDD_TAGFORM;

		BEGIN_MSG_MAP_EX(tag_dialog)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_HANDLER_EX(IDC_SAVE, BN_CLICKED, OnSave)
			COMMAND_HANDLER_EX(IDC_OK, BN_CLICKED, OnClose)
			COMMAND_HANDLER_EX(IDC_CANCEL, BN_CLICKED, OnClose)
			MSG_WM_CLOSE(OnClose)
		END_MSG_MAP()
	private:
		CToolTipCtrl tooltips[7];
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
#endif // SKIPCOUNT_PREFERENCES_H