#include "contextmenu.h"
#include "SkipCount.h"

#include <helpers/tag_write_callback_impl.h>
#include <SDK/messageBox.h>

#include "include/tinyxml2.h"

#include "preferences.h"
#include "record.h"

namespace foo_skipcount {
	/* Context Menu: Clear */
	void my_contextmenu_clear_cmds::get_item_name(unsigned int p_index, pfc::string_base &p_out) {
		switch(p_index) {
			case CMD_CLEAR_SKIPCOUNT:
				p_out = "Clear skip count";
				break;
			case CMD_CLEAR_LATESTSKIP:
				p_out = "Clear latest skip timestamp";
				break;
			case CMD_CLEAR_ALLBUTLATESTSKIP:
				p_out = "Clear all but latest skip timstamp";
				break;
			case CMD_CLEAR_OLDESTSKIP:
				p_out = "Clear the oldest skip timstamp";
				break;
			case CMD_CLEAR_ALLSKIPS:
				p_out = "Clear skip timestamps";
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	bool my_contextmenu_clear_cmds::context_get_display(unsigned int p_index, metadb_handle_list_cref, pfc::string_base& p_out, unsigned int& p_displayflags, const GUID&) {
		PFC_ASSERT(p_index >= 0 && p_index < get_num_items());
		p_displayflags = 0;
		get_item_name(p_index, p_out);
		return true;
	}

	void my_contextmenu_clear_cmds::context_command(unsigned int p_index, metadb_handle_list_cref p_data, const GUID&) {
		switch(p_index) {
			case CMD_CLEAR_SKIPCOUNT:
			case CMD_CLEAR_LATESTSKIP:
			case CMD_CLEAR_ALLBUTLATESTSKIP:
			case CMD_CLEAR_OLDESTSKIP:
			case CMD_CLEAR_ALLSKIPS:
				contextClear(p_data, p_index);
				break;
			default:
				uBugCheck();
				break;
		}
	}

	GUID my_contextmenu_clear_cmds::get_item_guid(unsigned int p_index) {
		switch(p_index) {
			case CMD_CLEAR_SKIPCOUNT:
				return guid_clear_skipcount;
				break;
			case CMD_CLEAR_LATESTSKIP:
				return guid_clear_latestskip;
				break;
			case CMD_CLEAR_ALLBUTLATESTSKIP:
				return guid_clear_allbutlatestskip;
				break;
			case CMD_CLEAR_OLDESTSKIP:
				return guid_clear_oldestskip;
				break;
			case CMD_CLEAR_ALLSKIPS:
				return guid_clear_skiptimes;
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	bool my_contextmenu_clear_cmds::get_item_description(unsigned p_index, pfc::string_base& p_out) {
		switch(p_index) {
			case CMD_DATA_COPY:
				p_out = "Copies skip statistics of the selected tracks to the clipboard.";
				return true;
				break;
			case CMD_DATA_PASTE:
				p_out = "Overwrites skips statistics of the selected tracks with data stored in the clipboard.";
				return true;
				break;
			case CMD_TAG_WRITE:
				p_out = "Write skip statistics to file tags on the selected tracks.";
				return true;
				break;
			case CMD_TAG_IMPORT:
				p_out = "Import skip statistics from file tags on the selected tracks.";
				return true;
				break;
			case CMD_TAG_EXPORT:
				p_out = "Export skip statistics of the selected tracks to an XML file.";
				return true;
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	void my_contextmenu_clear_cmds::contextClear(metadb_handle_list_cref items, int contextClearEnum) {
		if(cfg_confirmContextClear) {
			popup_message_v3::query_t query = {};
			query.title = "Confirm skip clear";
			query.msg = "Continue with context menu action?";
			query.buttons = popup_message_v3::buttonYes | popup_message_v3::buttonNo;
			query.defButton = popup_message_v3::buttonYes;
			query.icon = popup_message_v3::iconQuestion;
			query.wndParent = core_api::get_main_window();
			switch(popup_message_v3::get()->show_query_modal(query)) {
				case popup_message_v3::buttonYes:
					break;
				case popup_message_v3::buttonNo:
					return;
				default:
					return;
			}
		}
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;
			service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
			for(metadb_handle_ptr item : items) {
				metadb_index_hash hash;
				if(!client->hashHandle(item, hash)) {
					continue;
				}
				record_t record = getRecord(hash);

				switch(contextClearEnum) {
					case CMD_CLEAR_SKIPCOUNT: {
						if(record.skipCountNext == 0 && record.skipCountRandom == 0 && record.skipCountPrevious == 0 && record.skipCountDoubleClick == 0) {
							break;
						}
						record.skipCountNext = 0;
						record.skipCountRandom = 0;
						record.skipCountPrevious = 0;
						record.skipCountDoubleClick = 0;
						setRecord(hash, record);
						tmp += hash;
						break;
					}
					case CMD_CLEAR_LATESTSKIP: {
						if(record.skipTimes.size() < 2) {
							break;
						}
						record.skipTimes.pop_back();
						setRecord(hash, record);
						tmp += hash;
						break;
					}
					case CMD_CLEAR_OLDESTSKIP: {
						if(record.skipTimes.size() < 2) {
							break;
						}
						record.skipTimes.erase(record.skipTimes.begin());
						setRecord(hash, record);
						tmp += hash;
						break;
					}
					case CMD_CLEAR_ALLBUTLATESTSKIP: {
						if(record.skipTimes.size() < 2) {
							break;
						}
						record.skipTimes = { record.skipTimes.back() };
						setRecord(hash, record);
						tmp += hash;
						break;
					}
					case CMD_CLEAR_ALLSKIPS: {
						if(record.skipTimes.size() == 0) {
							break;
						}
						record.skipTimes.clear();
						setRecord(hash, record);
						tmp += hash;
						break;
					}
					default:
						uBugCheck(); // Failsafe
						break;
				}
			}

			pfc::list_t<metadb_index_hash> hashes;
			for(metadb_index_hash hash : tmp) {
				hashes += hash;
			}
			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		}
		catch(std::exception const& e) {
			popup_message::g_complain("Could not clear skip information", e);
		}
	}
	/* END Context Menu: Clear */

	/* Context Menu: Data */
	bool my_contextmenu_data_cmds::context_get_display(unsigned int p_index, metadb_handle_list_cref, pfc::string_base& p_out, unsigned int& p_displayflags, const GUID&) {
		PFC_ASSERT(p_index >= 0 && p_index < get_num_items());
		switch(p_index) {
			case CMD_DATA_PASTE: {
				pfc::com_ptr_t<IDataObject> data;
				DWORD dummy = DROPEFFECT_COPY;
				bool b_native;
				dropped_files_data_impl parsedData;
				metadb_handle_list handles;
				if(SUCCEEDED(OleGetClipboard(data.receive_ptr())) &&
				   SUCCEEDED(ole_interaction::get()->check_dataobject(data, dummy, b_native)) &&
				   SUCCEEDED(ole_interaction::get()->parse_dataobject(data, parsedData)) &&
				   parsedData.to_handles(handles, false, core_api::get_main_window()) &&
				   clipboardData.g_equals(clipboardData, handles)) {
					p_displayflags = 0;
				}
				else {
					p_displayflags = FLAG_DISABLED_GRAYED;
					clipboardData = metadb_handle_list();
				}
				break;
			}
			default:
				p_displayflags = 0;
				break;
		}
		get_item_name(p_index, p_out);
		return true;
	}

	void my_contextmenu_data_cmds::get_item_name(unsigned int p_index, pfc::string_base& p_out) {
		switch(p_index) {
			case CMD_DATA_COPY:
				p_out = "Copy skip count";
				break;
			case CMD_DATA_PASTE:
				p_out = "Paste skip count";
				break;
			case CMD_TAG_WRITE:
				p_out = "Write skip count to file tags";
				break;
			case CMD_TAG_IMPORT:
				p_out = "Import skip count from file tags";
				break;
			case CMD_TAG_EXPORT:
				p_out = "Export statistic to XML...";
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	void my_contextmenu_data_cmds::context_command(unsigned int p_index, metadb_handle_list_cref p_data, const GUID&) {
		switch(p_index) {
			case CMD_DATA_COPY:
				contextCopySkipData(p_data);
				break;
			case CMD_DATA_PASTE:
				contextPasteSkipData(p_data);
				break;
			case CMD_TAG_WRITE:
				contextWriteSkipTag(p_data);
				break;
			case CMD_TAG_IMPORT:
				contextImportSkipTag(p_data);
				break;
			case CMD_TAG_EXPORT:
				contextExportXMLSkipData(p_data);
				break;
			default:
				uBugCheck();
				break;
		}
	}

	GUID my_contextmenu_data_cmds::get_item_guid(unsigned int p_index) {
		switch(p_index) {
			case CMD_DATA_COPY:
				return guid_data_copy;
				break;
			case CMD_DATA_PASTE:
				return guid_data_paste;
				break;
			case CMD_TAG_WRITE:
				return guid_tag_write;
				break;
			case CMD_TAG_IMPORT:
				return guid_tag_import;
				break;
			case CMD_TAG_EXPORT:
				return guid_data_export;
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	bool my_contextmenu_data_cmds::get_item_description(unsigned p_index, pfc::string_base& p_out) {
		switch(p_index) {
			case CMD_DATA_COPY:
				p_out = "Copies skip statistics of the selected tracks to the clipboard.";
				return true;
				break;
			case CMD_DATA_PASTE: 
				p_out = "Overwrites skips statistics of the selected tracks with data stored in the clipboard.";
				return true;
				break;
			case CMD_TAG_WRITE:
				p_out = "Write skip statistics to file tags on the selected tracks.";
				return true;
				break;
			case CMD_TAG_IMPORT:
				p_out = "Import skip statistics from file tags on the selected tracks.";
				return true;
				break;
			case CMD_TAG_EXPORT:
				p_out = "Export skip statistics of the selected tracks to an XML file.";
				return true;
				break;
			default:
				uBugCheck(); // Failsafe
				break;
		}
	}

	void my_contextmenu_data_cmds::contextCopySkipData(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}
			pfc::com_ptr_t<IDataObject> data = ole_interaction::get()->create_dataobject(items);
			if(data.is_valid() && SUCCEEDED(OleSetClipboard(data.get_ptr()))) {
				this->clipboardData = items;
			}
			else {
				popup_message::g_complain("Invalid skip statistics to clipboard.");
			}
		}
		catch(const exception_service_not_found& e) {
			popup_message::g_complain("Could not copy skip statistics to clipboard.", e);
		}
	}

	void my_contextmenu_data_cmds::contextPasteSkipData(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;
			pfc::com_ptr_t<IDataObject> data;
			DWORD dummy = DROPEFFECT_COPY;
			bool b_native;
			dropped_files_data_impl parsedData;
			metadb_handle_list dataItems;
			if(SUCCEEDED(OleGetClipboard(data.receive_ptr())) &&
			   SUCCEEDED(ole_interaction::get()->check_dataobject(data, dummy, b_native)) &&
			   SUCCEEDED(ole_interaction::get()->parse_dataobject(data, parsedData)) &&
			   parsedData.to_handles(dataItems, b_native, NULL)) {

				if(items.get_count() != dataItems.get_count()) {
					std::string text = "Clipboard contains skip statistics for " + std::to_string(dataItems.get_count()) + " tracks. Select the same number of tracks to paste.";
					fb2k::messageBox(core_api::get_main_window(), text.c_str(), "Skip Statistics", MB_OK);
					return;
				}

				bool pasteNext = cfg_pasteNextCount;
				bool pasteRandom = cfg_pasteRandomCount;
				bool pastePrevious = cfg_pastePreviousCount;
				bool pasteDoubleClick = cfg_pastePreviousCount;
				uint32_t pasteSkips = cfg_pasteSkipSelection;
						
				if(!cfg_pasteDisableDialog) {
					paste_dialog paste_dlg;
					INT_PTR options = paste_dlg.DoModal(core_api::get_main_window());
					if(options == -1) {
						return;
					}
					pasteNext = (options & PASTE_NEXTCOUNT);
					pasteRandom = (options & PASTE_RANDOMCOUNT);
					pastePrevious = (options & PASTE_PREVIOUSCOUNT);
					pasteDoubleClick = (options & PASTE_DOUBLECLICKCOUNT);
					if(options & PASTE_ALLSKIPS) {
						pasteSkips = PASTESKIP_ALL;
					}
					else if(options & PASTE_NOSKIPS) {
						pasteSkips = PASTESKIP_NONE;
					}
					else if(options & PASTE_LATESTSKIP) {
						pasteSkips = PASTESKIP_LATEST;
					}
					else if(options & PASTE_OLDESTSKIP) {
						pasteSkips = PASTESKIP_OLDEST;
					}
					else if(options & PASTE_ALLBUTOLDESTSKIP) {
						pasteSkips = PASTESKIP_ALLBUTOLDEST;
					}
					else if(options & PASTE_ALLBUTLATESTSKIP) {
						pasteSkips = PASTESKIP_ALLBUTLATEST;
					}
					else if(options & PASTE_LATESTANDOLDESTSKIP) {
						pasteSkips = PASTESKIP_LATESTANDOLDEST;
					}
				}

				if(!pasteNext && !pasteRandom && !pastePrevious && !pasteDoubleClick && pasteSkips == PASTESKIP_NONE) {
					return;
				}

				popup_message_v3::query_t query = {};
				query.title = "Paste skip statistics";
				std::string msg = "The following data is about to be imported, overwriting your current skip statistics:";
				std::string timestamps = "";
#define MAX_DISPLAY_LENGTH 200
				query.buttons = popup_message_v3::buttonOK | popup_message_v3::buttonCancel;
				query.defButton = popup_message_v3::buttonOK;
				query.icon = popup_message_v3::iconQuestion;
				query.wndParent = core_api::get_main_window();
				
				//metadb_io_v2::get()->load_info_async(items, metadb_io_v2::load_info_default, core_api::get_main_window(), metadb_io_v2::op_flag_delay_ui, NULL);

				service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
				std::vector<std::pair<metadb_index_hash, record_t>> hashRecordsToWrite;
#define MAX_FILE_LISTING 6
				for(size_t t = 0; t < dataItems.get_count(); ++t) {
					metadb_index_hash hashCopy;
					if(!client->hashHandle(dataItems[t], hashCopy)) {
						continue;
					}
					record_t recordCopy = getRecord(hashCopy);
					metadb_index_hash hashWrite;
					if(!client->hashHandle(items[t], hashWrite)) {
						continue;
					}
					record_t recordWrite = getRecord(hashWrite);

					BOOL writeMsg = false;
					if(!writeMsg && t <= MAX_FILE_LISTING) {
						writeMsg = true;
					}
					if(writeMsg) {
						file_info_impl recordWriteFileInfo = items[t]->get_info_ref()->info();
						if(recordWriteFileInfo.meta_exists("title")) {
							msg += ("\n\nItem: " + std::string(recordWriteFileInfo.meta_get("title", 0)));
						}
						else {
							std::string titlePath(dataItems[t]->get_path());
							titlePath = titlePath.substr(titlePath.find_last_of("/\\") + 1);
							
							msg += ("\n\nItem: " + titlePath);
						}
					}
					if(pasteNext || pasteRandom || pastePrevious) {
						msg += writeMsg ? "\n" : "";
					}
					if(pasteNext) {
						recordWrite.skipCountNext = recordCopy.skipCountNext;
						msg += writeMsg ? ("Next skips: " + std::to_string(recordWrite.skipCountNext) + ((pasteRandom || pastePrevious) ? ", " : "")) : "";
					}
					if(pasteRandom) {
						recordWrite.skipCountRandom = recordCopy.skipCountRandom;
						msg += writeMsg ? (" Random skips: " + std::to_string(recordWrite.skipCountRandom) + (pastePrevious ? ", " : "")) : "";
					}
					if(pastePrevious) {
						recordWrite.skipCountPrevious = recordCopy.skipCountPrevious;
						msg += writeMsg ? (" Previous skips: " + std::to_string(recordWrite.skipCountPrevious)) : "";
					}
					if(pasteDoubleClick) {
						recordWrite.skipCountDoubleClick = recordCopy.skipCountDoubleClick;
						msg += writeMsg ? (" Double Click skips: " + std::to_string(recordWrite.skipCountDoubleClick)) : "";
					}
					if(recordCopy.version > 1) {
						if(pasteSkips == PASTESKIP_ALL) {
							recordWrite.skipTimes = recordCopy.skipTimes;
						}
						else if(pasteSkips == PASTESKIP_NONE) {}
						else if(pasteSkips == PASTESKIP_LATEST) {
							if((recordCopy.skipTimes.size() == 0)) {
								recordWrite.skipTimes.clear();
							}
							else {
								recordWrite.skipTimes = { recordCopy.skipTimes.back() };
							}
						}
						else if(pasteSkips == PASTESKIP_OLDEST) {
							if(recordCopy.skipTimes.size() == 0) {
								recordWrite.skipTimes.clear();
							}
							else {
								recordWrite.skipTimes = { recordCopy.skipTimes.front() };
							}
						}
						else if(pasteSkips == PASTESKIP_ALLBUTOLDEST) {
							if(recordCopy.skipTimes.size() < 2) {
								recordWrite.skipTimes.clear();
							}
							else {
								recordCopy.skipTimes.erase(recordCopy.skipTimes.begin());
								recordWrite.skipTimes = recordCopy.skipTimes;
							}
						}
						else if(pasteSkips == PASTESKIP_ALLBUTLATEST) {
							if(recordCopy.skipTimes.size() < 2) {
								recordWrite.skipTimes.clear();
							}
							else {
								recordCopy.skipTimes.pop_back();
								recordWrite.skipTimes = recordCopy.skipTimes;
							}
						}
						else if(pasteSkips == PASTESKIP_LATESTANDOLDEST) {
							if(recordCopy.skipTimes.size() < 3) {
								recordWrite.skipTimes.clear();
							}
							else {
								recordWrite.skipTimes = std::vector<t_filetimestamp>{ recordCopy.skipTimes.front(), recordCopy.skipTimes.back() };
							}
						}
						if(writeMsg) {
							pfc::stringLite skipTimes = getSkipTimesStr(recordWrite.skipTimes, false, true, false).c_str();
							if(skipTimes.get_length() > MAX_DISPLAY_LENGTH) {
								skipTimes.truncate(MAX_DISPLAY_LENGTH);
								skipTimes += "...";
							}
							if(skipTimes.get_length() > 0) {
								msg += "\n" + std::to_string(recordWrite.skipTimes.size()) + " Skips: ";
								msg += PFC_string_formatter() << skipTimes;
							}
							else {
								msg += "\nSkips: Empty";
							}
						}
					}
					hashRecordsToWrite.push_back(std::make_pair(hashWrite, recordWrite));
				}
				if(dataItems.get_count() > MAX_FILE_LISTING) {
					msg += "\n\n(" + std::to_string(dataItems.get_count() - MAX_FILE_LISTING) + " more...)";
				}

				query.msg = msg.c_str();
				switch(popup_message_v3::get()->show_query_modal(query)) {
					case popup_message_v3::buttonOK:
						for(std::pair pairHashRecord : hashRecordsToWrite) {
							setRecord(pairHashRecord.first, pairHashRecord.second);
							tmp += pairHashRecord.first;
						}
						break;
					case popup_message_v3::buttonCancel:
						return;
					default:
						return;
				}
			}
			else {
				popup_message::g_complain("Could not get valid skip statistics from clipboard.");
			}
			pfc::list_t<metadb_index_hash> hashes;
			for(metadb_index_hash hash : tmp) {
				hashes += hash;
			}

			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		}
		catch(const exception_service_not_found& e) {
			popup_message::g_complain("Could not paste skip statistics from clipboard.", e);
		}
	}

	void my_contextmenu_data_cmds::contextWriteSkipTag(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			bool tagNext = cfg_tagNextCount;
			bool tagRandom = cfg_tagRandomCount;
			bool tagPrevious = cfg_tagPreviousCount;
			bool tagDoubleClick = cfg_tagDoubleClickCount;
			bool tagCurrentTimestamp = cfg_tagCurrentTimestamp;
			bool tagAllTimestampsRaw = cfg_tagAllTimestampsRaw;
			uint32_t tagDelimiter = cfg_tagTimestampDelimiter;

			if(!cfg_tagDisableDialog) {
				tag_dialog tag_dlg;
				INT_PTR options = tag_dlg.DoModal(core_api::get_main_window());
				if(options == -1) {
					return;
				}
				tagNext = (options & TAG_NEXTCOUNT);
				tagRandom = (options & TAG_RANDOMCOUNT);
				tagPrevious = (options & TAG_PREVIOUSCOUNT);
				tagDoubleClick = (options & TAG_DOUBLECLICKCOUNT);
				tagCurrentTimestamp = (options & TAG_CURRENTTIME);
				tagAllTimestampsRaw = (options & TAG_ALLSKIPS);
				if(options & TAG_DELIMIT_SEMICOLON) {
					tagDelimiter = TAG_TIMESTAMP_SEMICOLON;
				}
				else if(options & TAG_DELIMIT_COMMA) {
					tagDelimiter = TAG_TIMESTAMP_COMMA;
				}
				else if(options & TAG_DELIMIT_FORWARDSLASH) {
					tagDelimiter = TAG_TIMESTAMP_FORWARDSLASH;
				}
				else if(options & TAG_DELIMIT_SPACE) {
					tagDelimiter = TAG_TIMESTAMP_SPACE;
				}
				else if(options & TAG_DELIMIT_COLON) {
					tagDelimiter = TAG_TIMESTAMP_COLON;
				}
				else if(options & TAG_DELIMIT_PIPE) {
					tagDelimiter = TAG_TIMESTAMP_PIPE;
				}
				else if(options & TAG_DELIMIT_AMPERSAND) {
					tagDelimiter = TAG_TIMESTAMP_AMPERSAND;
				}
			}

			pfc::avltree_t<metadb_index_hash> tmp;
			service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
			//metadb_io_v2::get()->load_info_async(items, metadb_io_v2::load_info_default, core_api::get_main_window(), metadb_io_v2::op_flag_delay_ui, NULL);
			t_size count = items.get_count();
			std::vector<file_info_impl> info(count);
			for(size_t i = 0; i < count; ++i) {
				metadb_index_hash hash;
				if(!client->hashHandle(items[i], hash)) {
					return;
				}
				record_t record = getRecord(hash);
				info[i] = items[i]->get_info_ref()->info();

				if(tagNext) {
					info[i].meta_set(tagMetaFieldString.at("next").c_str(), std::to_string(record.skipCountNext).c_str());
				}
				if(tagRandom) {
					info[i].meta_set(tagMetaFieldString.at("random").c_str(), std::to_string(record.skipCountRandom).c_str());
				}
				if(tagPrevious) {
					info[i].meta_set(tagMetaFieldString.at("previous").c_str(), std::to_string(record.skipCountPrevious).c_str());
				}
				if(tagDoubleClick) {
					info[i].meta_set(tagMetaFieldString.at("doubleclick").c_str(), std::to_string(record.skipCountDoubleClick).c_str());
				}
				if(tagCurrentTimestamp) {
					info[i].meta_set(tagMetaFieldString.at("current").c_str(), std::to_string(getLocalTimestamp(filetimestamp_from_system_timer(), false)).c_str());
				}
				if(tagAllTimestampsRaw) {
					info[i].meta_set(tagMetaFieldString.at("times").c_str(), getSkipTimesStr(record.skipTimes, false, false, false, tagTimestampDelimiters[tagDelimiter]).c_str());
				}
				tmp += hash;
			}

			metadb_io_v2::get()->update_info_async_simple(items, pfc::ptr_list_const_array_t<const file_info, file_info_impl*>(info.data(), info.size()), core_api::get_main_window(), metadb_io_v2::op_flag_delay_ui, nullptr);
			pfc::list_t<metadb_index_hash> hashes;
			for(metadb_index_hash hash : tmp) {
				hashes += hash;
			}
			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		}
		catch(const exception_service_not_found& e) {
			popup_message::g_complain("Could not write skip count statistics to file tags.", e);
		}
	}

	void my_contextmenu_data_cmds::contextImportSkipTag(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;
			//win32_event
			//metadb_io_v2::get()->load_info_async(items, metadb_io_v2::load_info_default, core_api::get_main_window(), metadb_io_v2::op_flag_delay_ui, NULL);
			service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
			for(metadb_handle_ptr item : items) {
				metadb_index_hash hash;
				if(!client->hashHandle(item, hash)) {
					return;
				}
				record_t record = getRecord(hash);
				bool hasRecordChanged = false;
				file_info_impl fileInfo = item->get_info_ref()->info();

				if(fileInfo.meta_exists(tagMetaFieldString.at("next").c_str())) {
					t_int num = static_cast<int>(parseInt(fileInfo.meta_get(tagMetaFieldString.at("next").c_str(), 0)));
					if(num > 0) {
						record.skipCountNext = num;
						hasRecordChanged = true;
					}
				}
				if(fileInfo.meta_exists(tagMetaFieldString.at("random").c_str())) {
					t_int num = static_cast<int>(parseInt(fileInfo.meta_get(tagMetaFieldString.at("random").c_str(), 0)));
					if(num > 0) {
						record.skipCountRandom = num;
						hasRecordChanged = true;
					}
				}
				if(fileInfo.meta_exists(tagMetaFieldString.at("previous").c_str())) {
					t_int num = static_cast<int>(parseInt(fileInfo.meta_get(tagMetaFieldString.at("previous").c_str(), 0)));
					if(num > 0) {
						record.skipCountPrevious = num;
						hasRecordChanged = true;
					}
				}
				if(fileInfo.meta_exists(tagMetaFieldString.at("doubleclick").c_str())) {
					t_int num = static_cast<int>(parseInt(fileInfo.meta_get(tagMetaFieldString.at("doubleclick").c_str(), 0)));
					if(num > 0) {
						record.skipCountDoubleClick = num;
						hasRecordChanged = true;
					}
				}
				if(fileInfo.meta_exists(tagMetaFieldString.at("times").c_str())) {
					auto a = getRawSkipTimesFromString(fileInfo.meta_get(tagMetaFieldString.at("times").c_str(), 0));
					record.skipTimes = getRawSkipTimesFromString(fileInfo.meta_get(tagMetaFieldString.at("times").c_str(), 0));
					hasRecordChanged = true;
				}
				
				if(hasRecordChanged) {
					setRecord(hash, record);
					tmp += hash;
				}
			}
			pfc::list_t<metadb_index_hash> hashes;
			for(metadb_index_hash hash : tmp) {
				hashes += hash;
			}
			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		}
		catch(const exception_service_not_found& e) {
			popup_message::g_complain("Could not write skip count statistics to file tags.", e);
		}
	}

	void my_contextmenu_data_cmds::contextExportXMLSkipData(metadb_handle_list_cref items) {
		bool xmlJS = cfg_xmlExportJSTimestamp;
		bool xmlDateTime = cfg_xmlExportDateTime;
		bool xmlTotalCount = cfg_xmlExportTotalCount;
		bool xmlNextCount = cfg_xmlExportNextCount;
		bool xmlRandomCount = cfg_xmlExportRandomCount;
		bool xmlPreviousCount = cfg_xmlExportPreviousCount;
		bool xmlLatest = cfg_xmlExportLatestSkip;
		bool xmlOldest = cfg_xmlExportOldestSkip;
		bool xmlTimestampCount = cfg_xmlExportTimestampCount;
		bool xmlAllTimestamps = cfg_xmlExportAllTimestamps;

		if(!cfg_xmlDisableDialog) {
			xml_dialog xml_dlg;
			INT_PTR options = xml_dlg.DoModal(core_api::get_main_window());
			if(options == -1) {
				return;
			}
			xmlJS = (options & EXPORT_JSTIMESTAMP);
			xmlDateTime = (options & EXPORT_DATETIME);
			xmlTotalCount = (options & EXPORT_TOTALSKIPCOUNT);
			xmlNextCount = (options & EXPORT_NEXTCOUNT);
			xmlRandomCount = (options & EXPORT_RANDOMCOUNT);
			xmlPreviousCount = (options & EXPORT_PREVIOUSCOUNT);
			xmlLatest = (options & EXPORT_LATESTSKIP);
			xmlOldest = (options & EXPORT_OLDESTSKIP);
			xmlTimestampCount = (options & EXPORT_TIMESTAMPCOUNT);
			xmlAllTimestamps = (options & EXPORT_ALLSKIPS);
		}

		pfc::string8 FilePath, DirectoryPath;
		if(::uGetOpenFileName(core_api::get_main_window(), "XML files|*.xml", 1, "xml", "Save XML file save location", DirectoryPath, FilePath, true) == 0) {
			return;
		}

		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}
			tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
			tinyxml2::XMLElement* root = doc->NewElement(COMPONENT_NAME);
			root->SetAttribute("Version", COMPONENT_VERSION);
			root->SetAttribute("Mapping", pfc::print_guid(guid_foo_skipcount_index).c_str());
			
			service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
			for(metadb_handle_ptr item : items) {
				metadb_index_hash hash;
				if(!client->hashHandle(item, hash)) {
					return;
				}

				record_t record = getRecord(hash);
				tinyxml2::XMLElement* entry = doc->NewElement("Entry");
				entry->SetAttribute("ID", hash);
				if(xmlNextCount) {
					entry->SetAttribute("NextSkips", record.skipCountNext);
				}
				if(xmlRandomCount) {
					entry->SetAttribute("RandomSkips", record.skipCountRandom);
				}
				if(xmlPreviousCount) {
					entry->SetAttribute("PreviousSkips", record.skipCountPrevious);
				}
				if(xmlTotalCount) {
					entry->SetAttribute("TotalSkips", (record.skipCountNext + record.skipCountRandom + record.skipCountPrevious));
				}
				if(xmlLatest) {
					t_filetimestamp latestTimestamp = getLatestSkip(record);
					entry->SetAttribute("LatestSkip", latestTimestamp != 0 ? foobar2000_io::format_filetimestamp(latestTimestamp) : "Never");
					entry->SetAttribute("LatestSkipTimestamp", latestTimestamp != 0 ? std::to_string(getLocalTimestamp(latestTimestamp, xmlJS)).c_str() : "Never");
				}
				if(xmlOldest) {
					t_filetimestamp oldestTimestamp = getOldestSkip(record);
					entry->SetAttribute("OldestSkip", oldestTimestamp != 0 ? foobar2000_io::format_filetimestamp(oldestTimestamp) : "Never");
					entry->SetAttribute("OldestSkipTimestamp", oldestTimestamp != 0 ? std::to_string(getLocalTimestamp(oldestTimestamp, xmlJS)).c_str() : "Never");
				}
				if(xmlTimestampCount) {
					entry->SetAttribute("TimestampCount", record.skipTimes.size());
				}

				tinyxml2::XMLElement* list = doc->NewElement("Item");
				std::string itemPath(item->get_path());
				if(itemPath.rfind("file://", 0) == 0) {
					itemPath.erase(0, strlen("file://"));
				}
				list->SetAttribute("Path", itemPath.c_str());
				if(xmlAllTimestamps) {
					for(t_filetimestamp ts : record.skipTimes) {
						tinyxml2::XMLElement* listitem = doc->NewElement("Timestamp");
						listitem->SetText(xmlDateTime ? foobar2000_io::format_filetimestamp(ts).c_str() : std::to_string(getLocalTimestamp(ts, xmlJS)).c_str());
						list->InsertEndChild(listitem);
					}
				}
				entry->InsertEndChild(list);
				root->InsertEndChild(entry);
			}
			try {
				doc->InsertEndChild(root);
				doc->SaveFile(FilePath);
			}
			catch(std::exception const& e) {
				popup_message::g_complain("Could not save XML file", e);
			}
		}
		catch(std::exception const& e) {
			popup_message::g_complain("Could not clear skip information", e);
		}
	}
	/* END Context Menu: Data */
} // namespace foo_skipcount
