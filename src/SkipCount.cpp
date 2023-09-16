#include "stdafx.h"
#include "preferences.h"
#include "SkipCount.h"
#include <mutex>
#include <vector>

using namespace pfc;

namespace foo_skipcount {

	// Pattern used to pin the data in config
	// Songs evaluating to the same string are considered the same, thus sharing skip counts.
	static const char strPinTo[] = "%artist% - $year($if2(%date%,%original release date%)) - %album% $if2(%discnumber%,1)-%tracknumber% %title%";

	std::atomic<bool> quitting = false;
	void foobarQuitting() {
		quitting = true;
		// Cleanly kill g_cachedAPI before reaching static object destructors or else
		g_cachedAPI.release();
	}

	void refreshSingle(GUID guid_foo_skipcount_index, metadb_index_hash hash) {
		g_cachedAPI->dispatch_refresh(guid_foo_skipcount_index, hash);
	}

	void refreshGlobal() {
		g_cachedAPI->dispatch_global_refresh();
	}

	// Called from init_stage_callback to hook into metadb
	// This is done properly, early, to prevent dispatch_global_refresh() with newly added fields from hammering playlists, etc.
	void addMetadbIndexes() {
		auto api = metadb_index_manager::get();
		g_cachedAPI = api;
		// Important, handle the exceptions here!
		// This will fail if the files holding component data are somehow corrupted.
		try {
			api->add(clientByGUID(guid_foo_skipcount_index), guid_foo_skipcount_index, retentionPeriod);
		} catch(std::exception const& e) {
			api->remove(guid_foo_skipcount_index);
			FB2K_console_formatter() << COMPONENT_NAME ": Critical initialization failure: " << e;
			return;
		}
		api->dispatch_global_refresh();
	}

	class my_metadb_io_edit_callback : public metadb_io_edit_callback {
	public:
		typedef const pfc::list_base_const_t<const file_info*>& t_infosref;
		void on_edited(metadb_handle_list_cref items, t_infosref before, t_infosref after) {
			for(size_t t = 0; t < items.get_count(); t++) {
				metadb_index_hash hashOld, hashNew;
				static hasher_md5::ptr hasher = hasher_md5::get();

				clientByGUID(guid_foo_skipcount_index)->hashHandle(items[t], hashOld);

				auto playable_location = make_playable_location(items[t]->get_path(), items[t]->get_subsong_index());
				hashNew = clientByGUID(guid_foo_skipcount_index)->transform(*after[t], playable_location);
				if(hashOld != hashNew) {
					record_t oldRecord = getRecord(hashOld);
					if(oldRecord.skipCountNext > 0 || oldRecord.skipCountRandom > 0 || oldRecord.skipCountPrevious > 0) {
						record_t newRecord = getRecord(hashNew);
						if(newRecord.skipCountNext + newRecord.skipCountPrevious + newRecord.skipCountRandom <= oldRecord.skipCountNext + oldRecord.skipCountRandom + oldRecord.skipCountPrevious) {
							setRecord(hashNew, oldRecord);
						}
					}
				}
			}
		}
	};
	static service_factory_single_t<my_metadb_io_edit_callback> g_my_metadb_io;

	void on_item_skipped(metadb_handle_ptr p_item, int control) {
		metadb_index_hash hash;
		clientByGUID(guid_foo_skipcount_index)->hashHandle(p_item, hash);

		record_t record = getRecord(hash);
		bool didIncrement = false;
		if(cfg_countNext && control == playback_control::t_track_command::track_command_next) {
			record.skipCountNext += 1;
			didIncrement = true;
		}
		else if(cfg_countRandom && control == playback_control::t_track_command::track_command_rand) {
			record.skipCountRandom += 1;
			didIncrement = true;
		}
		else if(cfg_countPrevious && control == playback_control::t_track_command::track_command_prev) {
			record.skipCountPrevious += 1;
			didIncrement = true;
		}
		if(didIncrement) {
			t_filetimestamp time = filetimestamp_from_system_timer();
			if(cfg_lastSkip) {
				record.lastSkip = time;
			}
			if(cfg_skipTimes) {
				record.skipTimesCounter++;
				record.skipTimes.push_back(time);
			}			
			setRecord(hash, record);
			refreshSingle(guid_foo_skipcount_index, hash);
		}
	}

	class my_play_callback : public play_callback_static {
	public:
		void on_playback_new_track(metadb_handle_ptr metadb) {
			if(!currentTrack.is_empty()) {
				previousTrack = currentTrack;
			}
			currentTrack = metadb;
			currentTrackTotalDuration = metadb->get_length();

			if(!previousTrack.is_empty() && shouldCountSkipControl > 0 && shouldCountSkipCondition) {
				on_item_skipped(previousTrack, shouldCountSkipControl);
			}

			if(cfg_skipProtectionPrevious && shouldCountSkipControl == playback_control::track_command_prev) {
				shouldCountSkipCondition = false;
			}
			else {
				shouldCountSkipCondition = true;
			}
			shouldCountSkipControl = 0;
			usedSkipProtection = false;
		}

		void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {
			if(cfg_countNext && p_command == playback_control::track_command_next) {
				shouldCountSkipControl = playback_control::get()->track_command_next;
			}
			else if(cfg_countRandom && p_command == playback_control::track_command_rand) {
				shouldCountSkipControl = playback_control::get()->track_command_rand;
			}
			else if(cfg_countPrevious && p_command == playback_control::track_command_prev) {
				shouldCountSkipControl = playback_control::get()->track_command_prev;
			}
		}

		void on_playback_stop(play_control::t_stop_reason p_reason) {
			// Do not reset when naturally starting new song playback
			// Do not reset if the player wants to count a skip when coming out of stopped playback and they stopped playback themselves.
			if(p_reason != play_control::get()->stop_reason_starting_another && !(cfg_countFromStop && p_reason == play_control::get()->stop_reason_user)) {
				previousTrack = nullptr;
				currentTrack = nullptr;
				shouldCountSkipControl = 0;
				shouldCountSkipCondition = true;
			}
		}
		void on_playback_seek(double p_time) {}
		void on_playback_pause(bool p_state) {
			// Do not reset if the player wants to count skips immediately from paused playback and it is currently being paused.
			if(p_state && !cfg_countFromPause) {
				previousTrack = nullptr;
				currentTrack = nullptr;
				shouldCountSkipControl = 0;
				shouldCountSkipCondition = true;
			}
		}
		void on_playback_edited(metadb_handle_ptr p_track) {}
		void on_playback_dynamic_info(const file_info& p_info) {}
		void on_playback_dynamic_info_track(const file_info& p_info) {}
		void on_playback_time(double p_time) {
			if(cfg_skipProtectionPrevious && !usedSkipProtection && p_time >= 1) {
				shouldCountSkipCondition = true;
				usedSkipProtection = true;
			}
			// If user has cfg_time set to 1, it will never be incremented, so just -1 for that specific scenario
			if(cfg_skipProtectionPrevious && usedSkipProtection && p_time >= 1) {
				p_time--;
			}

			if(shouldCountSkipCondition) {
				if(cfg_condition == TIME && p_time >= double(cfg_time)) {
					shouldCountSkipCondition = false;
				}
				else if(cfg_condition == PERCENT && (p_time / currentTrackTotalDuration) >= double(cfg_percent / 100)) {
					shouldCountSkipCondition = false;
				}
				else if(cfg_condition == TIMEANDPERCENT && (p_time / currentTrackTotalDuration) >= double(cfg_percent / 100) && p_time >= double(cfg_time)) {
					shouldCountSkipCondition = false;
				}
			}
		}
		void on_volume_change(float p_new_val) {}

		/* The play_callback_manager enumerates play_callback_static services and registers them automatically. Provide flags to indicating requested callbacks. */
		virtual unsigned get_flags() {
			return flag_on_playback_new_track | flag_on_playback_starting | flag_on_playback_stop | flag_on_playback_pause | flag_on_playback_time;
		}
	private:
		metadb_handle_ptr currentTrack, previousTrack;
		bool shouldCountSkipCondition = false, usedSkipProtection = false;
		unsigned int shouldCountSkipControl = 0;
		double currentTrackTotalDuration = 0;
	};

	static service_factory_single_t<my_play_callback> g_my_play_callback;

	void copyTimestampsToVector(std::vector<t_filetimestamp>& dest, unsigned int* src, unsigned int elementCount) {
		dest.insert(dest.begin(), src, src + elementCount);
	}

	//this is crashing
	record_t getRecord(metadb_index_hash hash, const GUID index_guid) {
		unsigned int buf[10004];
		record_t record;
		size_t size = 0;
		size = theAPI()->get_user_data_here(index_guid, hash, &buf, sizeof(buf));
		if(size == 0) {
			return record;
		}
		
		if(buf[0] == 1) {
			record.version = buf[0];
			record.skipCountNext = buf[1];
			record.skipCountRandom = buf[2];
			record.skipCountPrevious = buf[3];
		}
		else if(buf[0] == 2) {
			record.version = buf[0];
			record.skipCountNext = buf[1];
			record.skipCountRandom = buf[2];
			record.skipCountPrevious = buf[3];
			record.skipTimesCounter = buf[4];
			memcpy(&record.lastSkip, &buf[5], sizeof(t_filetimestamp));
			if(record.skipTimesCounter > 0) {
				//memcpy(&record.skipTimes, &buf[5], record.skipTimesCounter * sizeof(t_filetimestamp))
				record.skipTimes.insert(record.skipTimes.begin(), buf[5] + sizeof(t_filetimestamp)/sizeof(int), buf[5] + sizeof(t_filetimestamp) / sizeof(int) + record.skipTimesCounter);
				//copyTimestampsToVector(record.skipTimes, &buf[5] + (*t_filetimestamp), record.skipTimesCounter);
			}
		}
		return record;
	}

	std::mutex set_record_mutex;
	static void setRecord(metadb_index_hash hash, record_t record, const GUID index_guid) {
		unsigned int* buf = new unsigned int[10006];

		// Update record version before new set
		record.version = CURRENT_RECORD_VERSION;
		// Copy over the first five (non-vector, non-timestamp) settings
		memcpy(buf, &record, 5 * sizeof(int));
		size_t size = 5;

		// Copy lastSkip timestamp
		memcpy(buf + size, &record.lastSkip, sizeof(t_filetimestamp));

		size += sizeof(t_filetimestamp) / sizeof(int);
		// Copy over the vector of skip timestamps
		if(record.skipTimesCounter > 0) {
			memcpy(buf + size, &record.skipTimes[0], record.skipTimes.size() * sizeof(t_filetimestamp));
			size += record.skipTimes.size() * sizeof(t_filetimestamp) / sizeof(int);
		}

		std::lock_guard<std::mutex> guard(set_record_mutex);
		theAPI()->set_user_data(index_guid, hash, buf, size * sizeof(int));
		delete[] buf;
	}

	// Context Menu functions
	void clearSkipCount(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;

			for(size_t t = 0; t < items.get_count(); t++) {
				metadb_index_hash hash;
				clientByGUID(guid_foo_skipcount_index)->hashHandle(items[t], hash);

				record_t record = getRecord(hash);
				record.skipCountNext = 0;
				record.skipCountRandom = 0;
				record.skipCountPrevious = 0;
				setRecord(hash, record);
				tmp += hash;
			}

			pfc::list_t<metadb_index_hash> hashes;
			for(auto iter = tmp.first(); iter.is_valid(); iter++) {
				const metadb_index_hash hash = *iter;
				hashes += hash;
			}

			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		} catch(std::exception const& e) {
			popup_message::g_complain("Could not clear skip information", e);
		}
	}

	void clearLastSkip(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;

			for(size_t t = 0; t < items.get_count(); t++) {
				metadb_index_hash hash;
				clientByGUID(guid_foo_skipcount_index)->hashHandle(items[t], hash);

				record_t record = getRecord(hash);
				record.lastSkip = 0;
				setRecord(hash, record);
				tmp += hash;
			}

			pfc::list_t<metadb_index_hash> hashes;
			for(auto iter = tmp.first(); iter.is_valid(); iter++) {
				const metadb_index_hash hash = *iter;
				hashes += hash;
			}

			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		} catch(std::exception const& e) {
			popup_message::g_complain("Could not clear skip information", e);
		}
	}

	void clearSkipTimestamp(metadb_handle_list_cref items) {
		try {
			if(items.get_count() == 0) {
				throw pfc::exception_invalid_params();
			}

			pfc::avltree_t<metadb_index_hash> tmp;

			for(size_t t = 0; t < items.get_count(); t++) {
				metadb_index_hash hash;
				clientByGUID(guid_foo_skipcount_index)->hashHandle(items[t], hash);

				record_t record = getRecord(hash);
				record.skipTimesCounter = 0;
				record.skipTimes.clear();
				setRecord(hash, record);
				tmp += hash;
			}

			pfc::list_t<metadb_index_hash> hashes;
			for(auto iter = tmp.first(); iter.is_valid(); iter++) {
				const metadb_index_hash hash = *iter;
				hashes += hash;
			}

			theAPI()->dispatch_refresh(guid_foo_skipcount_index, hashes);
		} catch(std::exception const& e) {
			popup_message::g_complain("Could not clear skip information", e);
		}
	}
	// END: Context Menu functions

	// Static cached ptr to metadb_index_manager
	// Cached because there are many calls on a per-track basis, avoid passing always to low level functions
	// Obtaining the pointer from core is reasonably efficient - log(n) to the number of known service classes, but not good enough for something potentially called hundreds of times
	static metadb_index_manager::ptr theAPI() {
		auto ret = g_cachedAPI;
		if(ret.is_empty()) {
			ret = metadb_index_manager::get(); // since fb2k SDK v1.4, core API interfaces have a static get() method
		}
		return ret;
	}

	my_metadb_index_client::my_metadb_index_client(const char* pinTo, bool toLower = false) {
		static_api_ptr_t<titleformat_compiler>()->compile_force(m_keyObj, pinTo);
		forceLowercase = toLower;
	}

	void mb_to_lower(const char* src, pfc::string_base& dest) {
		for(;;) {
			unsigned c1;
			t_size d1;
			d1 = utf8_decode_char(src, c1);
			if(d1 == 0) {
				break;
			}
			else {
				dest.add_char(charLower(c1));
			}
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
		return static_api_ptr_t<hasher_md5>()->process_single_string(*strPtr).xorHalve();
	}

	service_ptr_t<my_metadb_index_client> clientByGUID(const GUID& guid) {
		service_ptr_t<my_metadb_index_client> g_clientIndex = new service_impl_t<my_metadb_index_client>(strPinTo, true);
		PFC_ASSERT(guid == guid_foo_skipcount_index);
		return g_clientIndex;
	}
}
