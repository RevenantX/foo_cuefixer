#include "stdafx.h"

DECLARE_COMPONENT_VERSION("CUE fixer", "1.0", "about message goes here");
VALIDATE_COMPONENT_FILENAME("foo_cuefixer.dll");

class playlist_cuefixer : public playlist_callback_static
{
	unsigned get_flags() override { return flag_all; }
	void on_items_added(t_size p_playlist, t_size p_start, const pfc::list_base_const_t<metadb_handle_ptr> & p_data, const bit_array & p_selection) override
	{
		auto playlistManager = playlist_manager::get();
		if (playlistManager->playlist_lock_is_present(p_playlist))
		{
			console::print("CUEFIXER: playlist locked");
			return;
		}

		static_api_ptr_t<playlist_manager> list;
		std::set<pfc::string> referencedFiles;
		std::shared_ptr<metadb_handle_list> entriesToRemove(new metadb_handle_list());
		file_info_const_impl info;
		const t_size itemsCount = p_data.get_size();
		t_size itemsToRemoveCount = 0;

		for (t_size i = 0; i < itemsCount; i++)
		{
			const metadb_handle_ptr playlistItem = p_data[i];
			playlistItem->get_info(info);
			const pfc::string fileDir(pfc::io::path::getParent(playlistItem->get_path()));
			for (t_size idx = 0; idx < info.info_get_count(); idx++)
			{
				if (strcmp(info.info_enum_name(idx), "referenced_file") == 0)
					referencedFiles.insert(pfc::io::path::combine(fileDir, info.info_enum_value(idx)));
			}
		}

		for (t_size i = 0; i < itemsCount; i++)
		{
			for (const auto &ref : referencedFiles)
			{
				if (metadb::path_compare(p_data[i]->get_path(), ref.c_str()) == 0)
				{
					entriesToRemove->add_item(p_data[i]);
					itemsToRemoveCount++;
					break;
				}
			}
		}

		fb2k::inMainThread([=]
		{
			pfc::bit_array_bittable table(playlistManager->playlist_get_item_count(p_playlist));
			for(t_size i = 0; i < itemsToRemoveCount; i++)
			{
				t_size idx;
				if(playlistManager->playlist_find_item(p_playlist, (*entriesToRemove)[i], idx))
				{
					table.set(idx, true);
				}
			}
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