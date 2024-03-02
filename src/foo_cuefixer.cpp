#include <SDK/foobar2000.h>

DECLARE_COMPONENT_VERSION("CUE fixer", "1.3", "CUE Fixer by RevenantX");
VALIDATE_COMPONENT_FILENAME("foo_cuefixer.dll");

class playlist_cuefixer : public playlist_callback_static
{
	unsigned get_flags() override { return flag_on_items_added; }
	
	void on_items_added(t_size p_playlist, t_size p_start, const pfc::list_base_const_t<metadb_handle_ptr> & p_data, const bit_array & p_selection) override
	{
		auto playlistManager = playlist_manager::get();
		auto* entriesToRemove = new metadb_handle_list();
		metadb_info_container::ptr infoRef;
		t_size addedItemsCount = p_data.get_size();
		t_size playlistItemsCount = playlistManager->playlist_get_item_count(p_playlist);
		t_size removeCount = 0;
		
		for (t_size i = 0; i < playlistItemsCount; i++)
		{
			metadb_handle_ptr itemHandle;
			
			if (!playlistManager->playlist_get_item_handle(itemHandle, p_playlist, i))
				continue;
			
			if (!itemHandle->get_info_ref(infoRef))
			{
				if (strcmp(pfc::io::path::getFileExtension(itemHandle->get_path()).c_str(), ".cue") == 0)
				{
					entriesToRemove->add_item(itemHandle);
					removeCount++;
				}
				continue;
			}
			
			const char* refField = infoRef->info().info_get("referenced_file");
			if (refField == nullptr)
				continue;

			pfc::string fileDir(pfc::io::path::getParent(itemHandle->get_path()));
			pfc::string referencedFullPath(pfc::io::path::combine(fileDir, refField));
			bool must_remove = false;
			try
			{
				if (!filesystem::g_exists(referencedFullPath.subString(7).c_str(), fb2k::noAbort))
				{
					must_remove = true;
				}
			}
			catch (exception_io &)
			{
				//probably malformed path from the cue file
				must_remove = true;
			}
			if (must_remove)
			{
				entriesToRemove->add_item(itemHandle);
				removeCount++;
				continue;
			}

			//check against added items
			for (t_size j = 0; j < addedItemsCount; j++)
			{
				if (metadb::path_compare(p_data[j]->get_path(), referencedFullPath.c_str()) == 0)
				{
					entriesToRemove->add_item(p_data[j]);
					removeCount++;
				}
			}
		}
		
		if (removeCount == 0)
		{
			delete entriesToRemove;
			return;
		}

		fb2k::inMainThread([=]
		{
			auto lock_filter_mask = playlistManager->playlist_lock_get_filter_mask(p_playlist);
			if (lock_filter_mask & playlist_lock::filter_remove)
			{
				delete entriesToRemove;
				console::print("CUEFIXER: playlist locked");
				return;
			}
			pfc::bit_array_bittable table(playlistManager->playlist_get_item_count(p_playlist));
			for (t_size i = 0; i < removeCount; i++)
			{
				t_size idx;
				if (playlistManager->playlist_find_item(p_playlist, (*entriesToRemove)[i], idx))
					table.set(idx, true);
			}
			delete entriesToRemove;
			playlistManager->playlist_remove_items(p_playlist, table);
		});
	}

	void on_items_reordered(t_size p_playlist, const t_size * p_order, t_size p_count) override {}
	void on_items_removing(t_size p_playlist, const bit_array & p_mask, t_size p_old_count, t_size p_new_count) override {}
	void on_items_removed(t_size p_playlist, const bit_array & p_mask, t_size p_old_count, t_size p_new_count) override {}
	void on_items_selection_change(t_size p_playlist, const bit_array & p_affected, const bit_array & p_state) override {}
	void on_item_focus_change(t_size p_playlist, t_size p_from, t_size p_to) override {}
	void on_items_modified(t_size p_playlist, const bit_array & p_mask) override {}
	void on_items_modified_fromplayback(t_size p_playlist, const bit_array & p_mask, play_control::t_display_level p_level) override {}
	void on_items_replaced(t_size p_playlist, const bit_array & p_mask, const pfc::list_base_const_t<t_on_items_replaced_entry> & p_data) override {}
	void on_item_ensure_visible(t_size p_playlist, t_size p_idx) override {}
	void on_playlist_activate(t_size p_old, t_size p_new) override {}
	void on_playlist_created(t_size p_index, const char * p_name, t_size p_name_len) override {}
	void on_playlists_reorder(const t_size * p_order, t_size p_count) override {}
	void on_playlists_removing(const bit_array & p_mask, t_size p_old_count, t_size p_new_count) override {}
	void on_playlists_removed(const bit_array & p_mask, t_size p_old_count, t_size p_new_count) override {}
	void on_playlist_renamed(t_size p_index, const char * p_new_name, t_size p_new_name_len) override {}
	void on_default_format_changed() override {}
	void on_playback_order_changed(t_size p_new_index) override {}
	void on_playlist_locked(t_size p_playlist, bool p_locked) override {}
};

static service_factory_single_t<playlist_cuefixer> g_cuefixer_playlist_callback;