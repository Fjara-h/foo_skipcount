#include "SkipCount.h"

#include <algorithm>

#include "preferences.h"
#include "record.h"

namespace foo_skipcount {
	std::atomic<bool> quitting = false;
	void foobarQuitting() {
		quitting = true;
		// Cleanly kill g_cachedAPI before reaching static object destructors or else
		g_cachedAPI.release();
	}

	// Called from init_stage_callback to hook into metadb early on to prevent dispatch_global_refresh() with newly added fields from hammering playlists, etc.
	void addMetadbIndexes() {
		metadb_index_manager::ptr api = metadb_index_manager::get();
		g_cachedAPI = api;
		// This fails if the files holding component data are somehow corrupted.
		try {
			api->add(clientByGUID(guid_foo_skipcount_index), guid_foo_skipcount_index, retentionPeriod);
		}
		catch(std::exception const& e) {
			api->remove(guid_foo_skipcount_index);
			FB2K_console_formatter() << COMPONENT_NAME ": Critical initialization failure: " << e;
			return;
		}
		api->dispatch_global_refresh();
	}

	// Static cached ptr to metadb_index_manager for efficient use on a per-track basis
	// Obtaining the pointer from core is reasonably efficient
	metadb_index_manager::ptr theAPI() {
		metadb_index_manager::ptr ret = g_cachedAPI;
		if(ret.is_empty()) {
			ret = metadb_index_manager::get(); // Since fb2k SDK v1.4, core API interfaces have a static get() method
		}
		return ret;
	}

	my_metadb_index_client::my_metadb_index_client(const char* pinTo, bool toLower = false) {
		titleformat_compiler::get()->compile_force(m_keyObj, pinTo);
		forceLowercase = toLower;
	}

	void mb_to_lower(const char* src, pfc::string_base& dest) {
		t_uint c1;
		t_size d1;
		while((d1 = pfc::utf8_decode_char(src, c1)) != 0) {
			dest.add_char(pfc::charLower(c1));
			src += d1;
		}
	}

	metadb_index_hash my_metadb_index_client::transform(const file_info& info, const playable_location& location) {
		pfc::string_formatter str, str_lower;
		pfc::string_formatter* strPtr = &str;
		m_keyObj->run_simple(location, &info, str);
		if(forceLowercase) {
			mb_to_lower(str, str_lower);
			strPtr = &str_lower;
		}
		// Make MD5 hash of the string, then reduce it to 64-bit metadb_index_hash
		return hasher_md5::get()->process_single_string(*strPtr).xorHalve();
	}

	service_ptr_t<my_metadb_index_client> clientByGUID(const GUID& guid) {
		service_ptr_t<my_metadb_index_client> g_clientIndex = new service_impl_t<my_metadb_index_client>(strPinTo, true);
		PFC_ASSERT(guid == guid_foo_skipcount_index);
		return g_clientIndex;
	}

	class my_metadb_io_edit_callback : public metadb_io_edit_callback {
	public:
		typedef const pfc::list_base_const_t<const file_info*>& t_infosref;
		void my_metadb_io_edit_callback::on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) {
			service_ptr_t<my_metadb_index_client> client = clientByGUID(guid_foo_skipcount_index);
			for(size_t t = 0; t < items.get_count(); ++t) {
				metadb_index_hash hashOld;
				if(!client->hashHandle(items[t], hashOld)) {
					continue;
				}

				// If files collide when renamed, unique join, because they are presumably the same track
				metadb_index_hash hashNew = client->transform(*after[t], make_playable_location(items[t]->get_path(), items[t]->get_subsong_index()));
				if(hashOld != hashNew) {
					record_t oldRecord = getRecord(hashOld);
					uint32_t oldSkipCount = oldRecord.skipCountNext + oldRecord.skipCountRandom + oldRecord.skipCountPrevious + oldRecord.skipCountDoubleClick;
					if(oldSkipCount > 0 || !oldRecord.skipTimes.empty()) {
						record_t newRecord = getRecord(hashNew);
						newRecord.skipCountNext += oldRecord.skipCountNext;
						newRecord.skipCountRandom += oldRecord.skipCountRandom;
						newRecord.skipCountPrevious += oldRecord.skipCountPrevious;
						newRecord.skipCountDoubleClick += oldRecord.skipCountDoubleClick;

						if(newRecord.skipTimes.size() == 0) {
							newRecord.skipTimes = oldRecord.skipTimes;
						}
						else {
							std::sort(newRecord.skipTimes.begin(), newRecord.skipTimes.end());
							std::sort(oldRecord.skipTimes.begin(), oldRecord.skipTimes.end());
							std::vector<t_filetimestamp> skipTimes(newRecord.skipTimes.size() + oldRecord.skipTimes.size());
							std::vector<t_filetimestamp>::iterator iter;
							iter = std::set_union(oldRecord.skipTimes.begin(), oldRecord.skipTimes.end(), newRecord.skipTimes.begin(), newRecord.skipTimes.end(), skipTimes.begin());
							skipTimes.resize(iter - skipTimes.begin());
							newRecord.skipTimes = skipTimes;
						}
						setRecord(hashNew, oldRecord);
					}
				}
			}
		}
	};
	static service_factory_single_t<my_metadb_io_edit_callback> g_my_metadb_io;

	class my_play_callback : public play_callback_static {
	public:
		void my_play_callback::on_item_skipped(metadb_handle_ptr p_item, int control) {
			metadb_index_hash hash;
			if(!clientByGUID(guid_foo_skipcount_index)->hashHandle(p_item, hash)) {
				return;
			}

			record_t record = getRecord(hash);
			record.skipCountNext += (control == playback_control::t_track_command::track_command_next);
			record.skipCountRandom += (control == playback_control::t_track_command::track_command_rand);
			record.skipCountPrevious += (control == playback_control::t_track_command::track_command_prev);
			record.skipCountDoubleClick += (control == playback_control::t_track_command::track_command_settrack);
			t_filetimestamp time = filetimestamp_from_system_timer();
			switch(cfg_logSkipTimes) {
				case LOGTYPE_NONE:
					break;
				case LOGTYPE_LATEST:
					if(!record.skipTimes.empty()) {
						// Only overwrite the latest skip
						record.skipTimes.back() = time;
						break;
					}
					[[fallthrough]]; // skiptimes is empty, so these use the same functionality
				case LOGTYPE_ALL:
					record.skipTimes.push_back(time);
					break;
				default:
					uBugCheck(); // Failsafe
					break;
			}
			setRecord(hash, record);
			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hash);
		}
		
		void my_play_callback::on_playback_new_track(metadb_handle_ptr metadb) {
			if(!currentTrack.is_empty()) {
				previousTrack = currentTrack;
			}

			if(!previousTrack.is_empty() && lastSkipCommand > 0 && !skipProtected && !hasPassedCondition) {
				on_item_skipped(previousTrack, lastSkipCommand);
			}
			currentTrack = metadb;
			currentTrackTotalDuration = metadb->get_length();

			skipProtected = true;
			hasPassedCondition = false;
			if(cfg_skipProtectionNext && lastSkipCommand == playback_control::track_command_next) {
				protectionOffsetTime = cfg_skipProtectionNextTime;
			}
			else if(cfg_skipProtectionRandom && lastSkipCommand == playback_control::track_command_rand) {
				protectionOffsetTime = cfg_skipProtectionRandomTime;
			}
			else if(cfg_skipProtectionPrevious && lastSkipCommand == playback_control::track_command_prev) {
				protectionOffsetTime = cfg_skipProtectionPreviousTime;
			}
			else if(cfg_skipProtectionDoubleClick && lastSkipCommand == playback_control::track_command_settrack) {
				protectionOffsetTime = cfg_skipProtectionDoubleClickTime;
			}
			else {
				skipProtected = false;
				protectionOffsetTime = 0;
			}
		}

		void my_play_callback::on_playback_starting(play_control::t_track_command p_command, bool) {
			if((cfg_countNext && p_command == playback_control::track_command_next) ||
			   (cfg_countRandom && p_command == playback_control::track_command_rand) ||
			   (cfg_countPrevious && p_command == playback_control::track_command_prev) ||
			   (cfg_countDoubleClick && p_command == playback_control::track_command_settrack))  {
				lastSkipCommand = p_command;
			}
			else {
				lastSkipCommand = 0;
			}
		}

		void my_play_callback::on_playback_stop(play_control::t_stop_reason p_reason) {
			// Do not reset when naturally starting new song playback
			// Do not reset if the user wants to count a skip when coming out of stopped playback and they stopped playback themselves.
			if(p_reason != play_control::stop_reason_starting_another && !(cfg_countFromStop && p_reason == play_control::stop_reason_user)) {
				previousTrack = nullptr;
				currentTrack = nullptr;
				protectionOffsetTime = 0;
				lastSkipCommand = 0;
			}
		}

		void on_playback_seek(double) {};

		void my_play_callback::on_playback_pause(bool p_state) {
			// Do not reset if the player wants to count skips immediately from paused playback and it is currently being paused.
			if(p_state && !cfg_countFromPause) {
				previousTrack = nullptr;
				currentTrack = nullptr;
				protectionOffsetTime = 0;
				lastSkipCommand = 0;
			}
		}

		void on_playback_edited(metadb_handle_ptr) {};
		void on_playback_dynamic_info(const file_info&) {};
		void on_playback_dynamic_info_track(const file_info&) {};

		void my_play_callback::on_playback_time(double p_time) {
			if(skipProtected) {
				if(p_time < protectionOffsetTime) {
					return;
				}
				else {
					skipProtected = false;
				}
			}

			// Time and/or percent required is shifted by adding protection time
			//  If skips should count after 5 seconds, and skip protection is 1 second, count after 6
			p_time += protectionOffsetTime;

			// While skips can be logged, check for one of the conditions to switch it off
			if(!hasPassedCondition && (
				(cfg_condition == LOGCONDITION_TIME && p_time >= double(cfg_time)) ||
				(cfg_condition == LOGCONDITION_PERCENT && (p_time / currentTrackTotalDuration) >= double(cfg_percent / 100)) ||
				(cfg_condition == LOGCONDITION_TIMEANDPERCENT && (p_time / currentTrackTotalDuration) >= double(cfg_percent / 100) && p_time >= double(cfg_time)))) {
				hasPassedCondition = true;
			}
		}

		void on_volume_change(float) {};

		/* The play_callback_manager enumerates play_callback_static services and registers them automatically. Provide flags to indicating requested callbacks. */
		inline virtual unsigned get_flags() {
			return flag_on_playback_new_track | flag_on_playback_starting | flag_on_playback_stop | flag_on_playback_pause | flag_on_playback_time;
		}
	private:
		metadb_handle_ptr currentTrack, previousTrack;
		bool skipProtected = false, hasPassedCondition = false;
		t_uint lastSkipCommand = 0, protectionOffsetTime = 0;
		double currentTrackTotalDuration = 0;
	};
	static play_callback_static_factory_t<my_play_callback> g_my_play_callback;
} // namespace foo_skipcount
