/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2008 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "portable.h"

#include "mconfig.h"
#include "text.h"
#include "play.h"

#include "advance.h"
#include <windows.h> //warlock
#include <sstream>

using namespace std;
extern string EmuladorAtivo; //warlock
vector<string> ListaEmuladores;
// --------------------------------------------------------------------------
// Configuration init

static adv_conf_enum_int OPTION_SORT[] = {
{ "name", sort_by_name },
{ "parent", sort_by_root_name },
{ "time", sort_by_time },
{ "play", sort_by_session },
{ "year", sort_by_year },
{ "manufacturer", sort_by_manufacturer },
{ "type", sort_by_type },
{ "size", sort_by_size },
{ "resolution", sort_by_res },
{ "info", sort_by_info },
{ "timeperplay", sort_by_timepersession },
{ "emulator", sort_by_emulator }
};

static adv_conf_enum_int OPTION_MODE[] = {
{ "list", mode_list },
{ "list_mixed", mode_list_mixed },
{ "layout", mode_custom },
{ "tile_tiny", mode_tile_tiny },
{ "tile_small", mode_tile_small },
{ "tile_normal", mode_tile_normal },
{ "tile_big", mode_tile_big },
{ "tile_enormous", mode_tile_enormous },
{ "tile_giant", mode_tile_giant },
{ "full", mode_full },
{ "full_mixed", mode_full_mixed },
{ "tile_icon", mode_tile_icon },
{ "tile_marquee", mode_tile_marquee },
{ "text", mode_text }
};

static adv_conf_enum_int OPTION_PREVIEW[] = {
{ "snap", preview_snap },
{ "flyers", preview_flyer },
{ "cabinets", preview_cabinet },
{ "titles", preview_title }
};

static adv_conf_enum_int OPTION_RESTORE[] = {
{ "save_at_exit", restore_none },
{ "restore_at_exit", restore_exit },
{ "restore_at_idle", restore_idle }
};

static adv_conf_enum_int OPTION_EXIT[] = {
{ "none", exit_none },
{ "normal", exit_normal },
{ "shutdown", exit_shutdown },
{ "all", exit_all }
};

static adv_conf_enum_int OPTION_SAVER[] = {
{ "snap", saver_snap },
{ "play", saver_play },
{ "flyers", saver_flyer },
{ "cabinets", saver_cabinet },
{ "titles", saver_title },
{ "none", saver_off }
};

static adv_conf_enum_int OPTION_IDLE_EXIT[] = {
{ "normal", idle_exit_normal },
{ "shutdown", idle_exit_shutdown },
};

static adv_conf_enum_int OPTION_GAMESAVER[] = {
{ "snap", saver_snap },
{ "flyers", saver_flyer },
{ "cabinets", saver_cabinet },
{ "titles", saver_title },
{ "none", saver_off }
};

static adv_conf_enum_int OPTION_DIFFICULTY[] = {
{ "none", difficulty_none },
{ "easiest", difficulty_easiest },
{ "easy", difficulty_easy },
{ "normal", difficulty_medium },
{ "hard", difficulty_hard },
{ "hardest", difficulty_hardest }
};

static adv_conf_enum_int OPTION_EVENTMODE[] = {
{ "fast", 1 },
{ "wait", 0 }
};

static adv_conf_enum_int OPTION_MERGE[] = {
{ "non-merged", merge_no },
{ "split", merge_differential },
{ "merged", merge_parent },
{ "any", merge_any },
{ "disable", merge_disable }
};

static adv_conf_enum_int OPTION_CLIPMODE[] = {
{ "none", clip_none },
{ "single", clip_single },
{ "singleloop", clip_singleloop },
{ "multi", clip_multi },
{ "multiloop", clip_multiloop },
{ "multiloopall", clip_multiloopall }
};

static void config_error_la(const string& line, const string& arg)
{
	target_err("Invalid argument '%s' at line '%s'.\n", arg.c_str(), line.c_str());
}

static void config_error_oa(const string& opt, const string& arg)
{
	target_err("Invalid argument '%s' at option '%s'.\n", arg.c_str(), opt.c_str());
}

static void config_error_a(const string& arg)
{
	target_err("Invalid argument '%s'.\n", arg.c_str());
}

static bool config_path_import(const string& s, string& a0)
{
	if (!arg_split(s, a0)) {
		config_error_a(s);
		return false;
	}

	if (a0 == "none" || a0 == "default")
		return true;

	a0 = path_import(file_config_file_home(a0.c_str()));

	return true;
}

static bool config_path(const string& s, string& a0)
{
	if (!arg_split(s, a0)) {
		config_error_a(s);
		return false;
	}
	
	if (a0 == "none" || a0 == "default" || a0 == "auto")
		return true;

	a0 = file_config_file_home(a0.c_str());

	return true;
}

static bool config_split(const string& s, string& a0)
{
	if (!arg_split(s, a0)) {
		config_error_a(s);
		return false;
	}
	return true;
}

static bool config_split(const string& s, string& a0, string& a1)
{
	if (!arg_split(s, a0, a1)) {
		config_error_a(s);
		return false;
	}
	return true;
}

static bool config_split(const string& s, string& a0, string& a1, string& a2)
{
	if (!arg_split(s, a0, a1, a2)) {
		config_error_a(s);
		return false;
	}
	return true;
}

static bool config_split(const string& s, string& a0, string& a1, string& a2, string& a3)
{
	if (!arg_split(s, a0, a1, a2, a3)) {
		config_error_a(s);
		return false;
	}
	return true;
}

static bool config_split_system(string& a1, string& as)
{
	string replace = subs(a1, ":", " ");

	if (!arg_split(replace, a1, as)) {
		config_error_a(a1);
		return false;
	}
	    
	return true;
}

static string config_out(const string& a)
{
	return "\"" + a + "\"";
}

static string config_normalize(const string& a)
{
	string r;
	for(unsigned i=0;i<a.length();++i)
		if (isspace(a[i]) || a[i] == '/')
			r += '_';
		else
			r += tolower(a[i]);
	return r;
}

bool config_split_custom(const string& s, string& a0)
{
	return config_split(s, a0);
}
bool config_split_custom(const string& s, string& a0, string& a1)
{
	return config_split(s, a0, a1);
}
bool config_split_custom(const string& s, string& a0, string& a1, string& a2)
{
	return config_split(s, a0, a1, a2);
}
bool config_split_custom(const string& s, string& a0, string& a1, string& a2, string& a3)
{
	return config_split(s, a0, a1, a2, a3);
}

static string borrar_comillas(const string& s) {
	string r = s;
	if (r[0] == '"' && r[r.length()-1] == '"')
		r = s.substr(1, s.length()-2);
	else {
		if (r[0] == '"') 
			r = s.substr(1, s.length()-1);
		else if (r[r.length()-1] == '"') 
			r = s.substr(0, s.length()-1);
	}
	return r;
}

void config_state::conf_register_custom(adv_conf* config_context)
{
	unsigned i;
	for(i=0; OPTION_LAYOUT[i].option; ++i)
	{
		conf_string_register_default(config_context, OPTION_LAYOUT[i].option, OPTION_LAYOUT[i].def_value);
	}
}

void config_state::conf_register(adv_conf* config_context)
{
	// MENU SYSTEMS
	conf_bool_register_default(config_context, "menu_systems", 0);
	conf_string_register_multi(config_context, "menu_systems_attrib");
	conf_string_register_default(config_context, "menu_systems_altss", "");
	conf_string_register_default(config_context, "menu_systems_flyers", "");
	conf_string_register_default(config_context, "menu_systems_cabinets", "");
	conf_string_register_default(config_context, "menu_systems_icons", "");
	conf_string_register_default(config_context, "menu_systems_marquees", "");
	conf_string_register_default(config_context, "menu_systems_titles", "");
	conf_string_register_default(config_context, "menu_systems_default_snap", "none");
	conf_string_register_default(config_context, "menu_systems_default_marquee", "none");
	conf_string_register_default(config_context, "menu_systems_layout", "");
	conf_string_register_default(config_context, "menu_systems_background", "");
	conf_string_register_default(config_context, "menu_systems_help", "");
	conf_string_register_default(config_context, "menu_systems_start", "");
	conf_string_register_default(config_context, "menu_systems_font", "");
	conf_string_register_default(config_context, "menu_systems_font_size", "");
	conf_string_register_default(config_context, "menu_systems_font_color", "");
	conf_string_register_default(config_context, "menu_systems_font_color_select", "");
	
	conf_string_register_multi(config_context, "emulator");
	conf_string_register_multi(config_context, "emulator_roms");
	conf_string_register_multi(config_context, "emulator_roms_filter");
	conf_string_register_multi(config_context, "emulator_altss");
	conf_string_register_multi(config_context, "emulator_flyers");
	conf_string_register_multi(config_context, "emulator_cabinets");
	conf_string_register_multi(config_context, "emulator_icons");
	conf_string_register_multi(config_context, "emulator_marquees");
	conf_string_register_multi(config_context, "emulator_titles");
	conf_string_register_multi(config_context, "emulator_include");
	conf_string_register_multi(config_context, "emulator_attrib");
	conf_string_register_multi(config_context, "emulator_layout");
	conf_string_register_multi(config_context, "emulator_background");
	conf_string_register_multi(config_context, "emulator_default_snap");
	conf_string_register_multi(config_context, "emulator_default_marquee");
	conf_string_register_multi(config_context, "emulator_help");
	conf_string_register_multi(config_context, "emulator_start");
	conf_string_register_multi(config_context, "emulator_font");
	conf_string_register_multi(config_context, "emulator_font_size");
	conf_string_register_multi(config_context, "emulator_font_color");
	conf_string_register_multi(config_context, "emulator_font_color_select");
	conf_string_register_multi(config_context, "favorites");
	conf_string_register_default(config_context, "favorites_include", "\"All Games\"");
	conf_string_register_multi(config_context, "type");
	conf_string_register_multi(config_context, "type_include");
	conf_string_register_multi(config_context, "type_import");
	conf_string_register_multi(config_context, "desc_import");
	conf_string_register_multi(config_context, "info_import");
	conf_string_register_multi(config_context, "game");
	conf_int_register_enum_default(config_context, "sort", conf_enum(OPTION_SORT), sort_by_root_name);
	conf_bool_register_default(config_context, "lock", 0);
	conf_int_register_enum_default(config_context, "config", conf_enum(OPTION_RESTORE), restore_none);
	conf_int_register_enum_default(config_context, "mode", conf_enum(OPTION_MODE), mode_list);
	conf_string_register_default(config_context, "mode_skip", "");
	conf_int_register_enum_default(config_context, "misc_exit", conf_enum(OPTION_EXIT), exit_normal);
	conf_bool_register_default(config_context, "ui_console", 0);
	conf_bool_register_default(config_context, "ui_menukey", 1);
	conf_string_register_multi(config_context, "event_assign");
	conf_string_register_multi(config_context, "ui_color");
	conf_string_register_default(config_context, "idle_start", "0 0");
	conf_string_register_default(config_context, "idle_screensaver", "60 10");
	conf_int_register_enum_default(config_context, "idle_screensaver_preview", conf_enum(OPTION_SAVER), saver_snap);
	conf_int_register_default(config_context, "idle_exit_time", 0);
	conf_int_register_enum_default(config_context, "idle_exit_type", conf_enum(OPTION_IDLE_EXIT), idle_exit_normal);
	
	// REM SELECTED
	conf_string_register_default(config_context, "menu_pos", "0 0");
	// opciones obsoletas del rc
	conf_int_register_default(config_context, "menu_base", 0);
	conf_int_register_default(config_context, "menu_rel", 0);

	conf_string_register_default(config_context, "event_repeat", "500 50");
	conf_bool_register_default(config_context, "input_hotkey", 1);
	conf_string_register_default(config_context, "ui_gamemsg", "\"Loading\"");
	conf_int_register_enum_default(config_context, "ui_game", conf_enum(OPTION_GAMESAVER), saver_snap);
	conf_int_register_enum_default(config_context, "difficulty", conf_enum(OPTION_DIFFICULTY), difficulty_none);
	conf_int_register_enum_default(config_context, "preview", conf_enum(OPTION_PREVIEW), preview_snap);
	conf_float_register_limit_default(config_context, "preview_expand", 1.0, 6.0, 1.15);
	conf_string_register_default(config_context, "preview_default", "none");
	conf_string_register_default(config_context, "preview_default_snap", "none");
	conf_string_register_default(config_context, "preview_default_flyer", "none");
	conf_string_register_default(config_context, "preview_default_cabinet", "none");
	conf_string_register_default(config_context, "preview_default_icon", "none");
	conf_string_register_default(config_context, "preview_default_marquee", "none");
	conf_string_register_default(config_context, "preview_default_title", "none");
	conf_int_register_enum_default(config_context, "event_mode", conf_enum(OPTION_EVENTMODE), 1);
	conf_bool_register_default(config_context, "event_alpha", 1);
	conf_int_register_enum_default(config_context, "ui_clip", conf_enum(OPTION_CLIPMODE), clip_single);
	conf_int_register_enum_default(config_context, "romset_type", conf_enum(OPTION_MERGE), merge_no);
	conf_int_register_limit_default(config_context, "icon_space", 10, 500, 43);
	conf_string_register_default(config_context, "sound_foreground_nocoin", "none");
	conf_string_register_default(config_context, "sound_foreground_notime", "none");
	conf_string_register_default(config_context, "background_config_menu", "none");
	conf_string_register_default(config_context, "sound_foreground_coin", "none");
	conf_string_register_default(config_context, "sound_foreground_zgeral", "default");
	conf_string_register_default(config_context, "sound_foreground_zerado", "default");
	conf_string_register_default(config_context, "sound_foreground_begin", "default");
	conf_string_register_default(config_context, "sound_foreground_end", "default");
	conf_string_register_default(config_context, "sound_foreground_key", "default");
	conf_string_register_default(config_context, "sound_foreground_start", "default");
	conf_string_register_default(config_context, "sound_foreground_stop", "default");
	conf_string_register_default(config_context, "sound_background_loop", "default");
	conf_string_register_default(config_context, "sound_background_begin", "none");
	conf_string_register_default(config_context, "sound_background_end", "none");
	conf_string_register_default(config_context, "sound_background_start", "none");
	conf_string_register_default(config_context, "sound_background_stop", "none");
	conf_string_register_default(config_context, "sound_background_loop_dir", "\"mp3\"");
	conf_string_register_default(config_context, "display_size", "1024");
	conf_string_register_default(config_context, "ui_font", "auto");
	conf_string_register_default(config_context, "ui_fontsize", "auto");
	conf_string_register_default(config_context, "display_orientation", "");
	conf_float_register_limit_default(config_context, "display_gamma", 0.2, 5, 1);
	conf_float_register_limit_default(config_context, "display_brightness", 0.2, 5, 1);
	conf_bool_register_default(config_context, "display_restoreatgame", 1);
	conf_bool_register_default(config_context, "display_restoreatexit", 1);
	conf_bool_register_default(config_context, "misc_quiet", 0);
	conf_float_register_limit_default(config_context, "ui_translucency", 0, 1, 0.6);
	conf_string_register_default(config_context, "ui_background", "none");
	conf_string_register_default(config_context, "ui_help", "none");
	conf_string_register_default(config_context, "ui_exit", "none");
	conf_string_register_default(config_context, "ui_startup", "none");
	conf_int_register_limit_default(config_context, "ui_skipbottom", 0, 1000, 0);
	conf_int_register_limit_default(config_context, "ui_skiptop", 0, 1000, 0);
	conf_int_register_limit_default(config_context, "ui_skipleft", 0, 1000, 0);
	conf_int_register_limit_default(config_context, "ui_skipright", 0, 1000, 0);
	conf_bool_register_default(config_context, "ui_bottombar", 1);
	conf_bool_register_default(config_context, "ui_topbar", 1);
	conf_string_register_default(config_context, "ui_command_menu", "Command...");
	conf_string_register_default(config_context, "ui_command_error", "Error running the command");
	conf_string_register_multi(config_context, "ui_command");
	conf_bool_register_default(config_context, "rem_selected", 1);
	conf_bool_register_default(config_context, "favorites_filtertype", 0);
	conf_bool_register_default(config_context, "security_exit", 1);
	conf_string_register_default(config_context, "mame_history", "none");
	conf_string_register_default(config_context, "mame_info", "none");
	conf_string_register_default(config_context, "xml_dir", "default");
	conf_string_register_default(config_context, "favorites_dir", "favlist");
}

// -------------------------------------------------------------------------
// Configuration load

static bool config_is_emulator(const pemulator_container& ec, const string& emulator)
{
	for(pemulator_container::const_iterator i=ec.begin();i!=ec.end();++i) {
		if ((*i)->user_name_get() == emulator)
			return true;
	}
	return false;
}

static bool config_load_background_dir(const string& dir, path_container& c)
{
	bool almost_one = false;

	log_std(("menu: load background music dir %s\n", dir.c_str()));

	DIR* d = opendir(cpath_export(slash_remove(dir)));
	if (!d)
		return almost_one;

	struct dirent* dd;
	while ((dd = readdir(d))!=0) {
		string file = file_import(dd->d_name);
		if (file_ext(file) == ".mp3" || file_ext(file) == ".wav") {
			string path = slash_add(dir) + file;

			log_std(("menu: load background music file %s\n", path.c_str()));

			c.insert(c.end(), path);
		}
	}

	closedir(d);
	return almost_one;
}

static bool config_load_background_list(const string& list, path_container& c)
{
	bool almost_one = false;
	int i = 0;

	while (i<list.length()) {
		char sep[2];
		sep[0] = file_dir_separator();
		sep[1] = 0;

		string arg = token_get(list, i, sep);

		token_skip(list, i, sep);

		string dir;

		if (config_path_import(arg, dir)) {
			if (config_load_background_dir(dir, c))
				almost_one = true;
		}
	}

	return almost_one;
}

bool config_state::load_game(const string& name, const string& type_name, const string& time, const string& session, const string& desc)
{
	game_set::const_iterator i = gar.find(game(name));
	if (i==gar.end())
		return false;

	i->user_type_set(type.insert(type_name));

	if (desc.length()!=0)
		i->user_description_set(desc);

	if (time.length()!=0 && isdigit(time[0]))
		i->time_set(atoi(time.c_str()));

	if (session.length()!=0 && isdigit(session[0]))
		i->session_set(atoi(session.c_str()));

	return true;
}

static bool config_emulator_load(const string& name, pemulator_container& emu, void (emulator::*set)(const string& s), const string& value)
{
	pemulator_container::iterator i = emu.begin();
	while (i!=emu.end() && name!=(*i)->user_name_get())
		++i;
	if (i!=emu.end()) {
		((*i)->*set)(value);
		return true;
	} else {
		return false;
	}
}

static bool config_emulator_attrib_load(const string& name, pemulator_container& emu, const string& value0, const string& value1)
{
	pemulator_container::iterator i = emu.begin();
	while (i!=emu.end() && name!=(*i)->user_name_get())
		++i;
	if (i!=emu.end()) {
		if (!(*i)->attrib_set(value0, value1))
			return false;
		return true;
	} else {
		return false;
	}
}

static bool config_load_iterator(adv_conf* config_context, const string& tag, bool (*func)(const string& s))
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0 = conf_iterator_string_get(&i);
		if (!func(a0)) {
			config_error_a(a0);
			return false;
		}
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_layout_set(adv_conf* config_context, const string& tag, layout* lay, void (layout::*set)(const string& s))
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		const string arg = borrar_comillas(conf_iterator_string_get(&i));

		(lay->*set)(arg);

		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_emu_path_set(adv_conf* config_context, const string& tag, pemulator_container& emu, void (emulator::*set)(const string& s))
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s, a0, a1;
		s = conf_iterator_string_get(&i);
		if (!config_split(s, a0, a1))
			return false;

		if (a1 != "none" && a1 != "default") {
			a1 = file_config_file_home(a1.c_str());
		}
		
		//Impedir de deletar info do jogo que foi removido warlock 
		if (!config_emulator_load(a0, emu, set, a1)) {
			//config_error_a(s);
			//return false;
			
		}
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_emu_set(adv_conf* config_context, const string& tag, pemulator_container& emu, void (emulator::*set)(const string& s))
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s, a0, a1;
		s = conf_iterator_string_get(&i);
		if (!config_split(s, a0, a1))
			return false;
		//Impedir de deletar info do jogo que foi removido warlock 
		if (!config_emulator_load(a0, emu, set, a1)) {
			//config_error_a(s);
			//return false;
		}
		
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_emu_attrib(adv_conf* config_context, const string& tag, pemulator_container& emu, bool quiet)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s, a0, a1, a2;
		s = conf_iterator_string_get(&i);
		if (!config_split(s, a0, a1, a2))
			return false;
		if (!config_emulator_attrib_load(a0, emu, a1, a2)) {
			//if(!quiet)
				//config_error_a(s);
			//return false;
		}
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_skip(adv_conf* config_context, unsigned& mask)
{
	string s;
	int i;

	s = conf_string_get_default(config_context, "mode_skip");
	i = 0;
	mask = 0;
	while (i<s.length()) {
		unsigned j;
		string a0;
		a0 = arg_get(s, i);
		for(j=0;j<conf_size(OPTION_MODE);++j)
			if (a0 == OPTION_MODE[j].value)
				break;
		if (j == conf_size(OPTION_MODE)) {
			config_error_la("mode_skip " + s, a0);
			return false;
		}
		mask |= OPTION_MODE[j].map;
	}

	return true;
}

// MENU SYSTEMS
static bool config_load_menu_systems(adv_conf* config_context, const string& tag, pmenu_systems& menusystems)
{
	menusystems = new systems("MENU SYSTEMS", "", "");
	
	return conf_bool_get_default(config_context, tag.c_str());
}
//
static bool config_load_iterator_menu_systems_attrib(adv_conf* config_context, const string& tag, pmenu_systems& menusystems)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s, a0, a1;
		s = conf_iterator_string_get(&i);
		if (!config_split(s, a0, a1))
			return false;
		if (!menusystems->attrib_set(a0, a1)) {
			config_error_a(s);
			return false;
		}
		conf_iterator_next(&i);
	}
	return true;
}
//
static bool config_load_menu_systems_set(adv_conf* config_context, const string& tag, pmenu_systems& menusystems, void (emulator::*set)(const string& s))
{
	string s, a0;
	s = conf_string_get_default(config_context, tag.c_str());

	if (!config_split(s, a0))
		return false;

	((menusystems)->*set)(a0);
	
	return true;
}
//
static bool config_load_menu_systems_path_set(adv_conf* config_context, const string& tag, pmenu_systems& menusystems, void (emulator::*set)(const string& s))
{
	string s, a0;
	s = conf_string_get_default(config_context, tag.c_str());

	if (!config_split(s, a0))
		return false;

	if (a0 != "" && a0 != "none" && a0 != "default") {
		a0 = file_config_file_home(a0.c_str());
	}

	((menusystems)->*set)(a0);
	
	return true;
}

static bool config_load_iterator_emu(adv_conf* config_context, const string& tag, pemulator_container& emu)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0, a1, a2, a3;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0, a1, a2, a3))
			return false;
		if (a0.length()==0 || a1.length()==0) {
			config_error_a(s);
			return false;
		}
		// separa a1 en tipo de emulador y sistema
		string as; //system
		if (!config_split_system(a1, as))
			return false;
		
		emulator* e;
		if (a1 == "mame") {
			if (as.length()==0)
				e = new wmame(a0, a2, a3);
			else
				e = new wmess(a0, a1, as, a2, a3);
		} else if (a1 == "sdlmame") {
			if (as.length() == 0)
				e = new sdlmame(a0, a2, a3);
			else
				e = new sdlmess(a0, a1, as, a2, a3);
		} else if (a1 == "sdlmess") {
				e = new sdlmess(a0, a1, as, a2, a3);
		} else if (a1 == "mess") {
				e = new wmess(a0, a1, as, a2, a3);
		} else if (a1 == "dmame") {
			e = new dmame(a0, a2, a3);
		} else if (a1 == "dmess") {
			e = new dmess(a0, a2, a3);
		} else if (a1 == "draine") {
			e = new draine(a0, a2, a3);
		} else if (a1 == "generic") {
			e = new generic(a0, a2, a3);
		} else if (a1 == "advmame") {
			e = new advmame(a0, a2, a3);
		} else if (a1 == "advmess") {
			e = new advmess(a0, a2, a3);
		} else if (a1 == "advpac") {
			e = new advpac(a0, a2, a3);
		} else {
			config_error_la(tag + " " + s, a1);
			return false;
		}

		//Cria uma lista com todos emuladores adicionado no sistema Warlock
		ListaEmuladores.push_back(a0);
		
		//Verifica se o emulador está removido ou adicionado ao sistema
		int Ret=0;
		char buffer1[100];
		
		string move1 = ".\\ini\\";
		move1 += a0.c_str();
		move1 += ".ini";
		Ret = GetPrivateProfileStringA("configuracao","OCULTAR","0", buffer1, 100, move1.c_str()); 
		//Ret = GetPrivateProfileStringA("OCULTAR",a0.c_str(),"0", buffer1, 100, move1.c_str()); 
		if (Ret){ //warlock
			if (atoi(buffer1) <= 0) {
				emu.insert(emu.end(), e);
			} else{
				
				conf_remove(config_context, "", "emulator_include"); //warlock Move o menu para o priemiro emulador
			}
		}
		conf_iterator_next(&i);

	}

	return true;
}

static bool config_load_iterator_favorites(adv_conf* config_context, const string& tag, pfavorites_container& fav)
{
	if (fav.size() == 0) {
		favorite* f;
		f = new favorite("All Games");
		fav.insert(fav.end(), f);
	}
	

		
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0))
			return false;
		if (a0.length() == 0) {
			config_error_a(s);
			return false;
		}

		if(a0 != "All Games") {
			favorite* f;
			f = new favorite(a0);
			fav.insert(fav.end(), f);
		}
		
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_pcategory(adv_conf* config_context, const string& tag, pcategory_container& cat)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0))
			return false;
		cat.insert(a0);
		conf_iterator_next(&i);
	}
	return true;
}

bool config_state::load_iterator_import(adv_conf* config_context, const string& tag, void (config_state::*set)(const game&, const string&), bool opt_verbose)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0, a1, a2, a3;
		string s = conf_iterator_string_get(&i);

		if (!config_split(s, a0, a1, a2, a3))
			return false;

		if (a0 != "nms" && a0 != "ini" && a0 != "mac" && a0 != "catver" && a0 != "catlist") {
			config_error_oa(tag, a0);
			return false;
		}
		
		
		if (!config_is_emulator(emu, a1)) {
			config_error_oa(tag, a1);
			return false;
		}

		if (a2.length() == 0) {
			config_error_oa(tag, a2);
			return false;
		}

		a2 = path_import(a2);

		config_import j(a0, a1, a2, a3);

		if (opt_verbose)
			target_nfo("log: importing from %s\n", a2.c_str());

		j.import(gar, *this, set);

		conf_iterator_next(&i);
	}

	return true;
}

static bool config_load_iterator_category(adv_conf* config_context, const string& tag, category_container& cat)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0))
			return false;
		cat.insert(cat.end(), a0);
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_category_section(adv_conf* config_context, const string& section, const string& tag, category_container& cat)
{
	adv_conf_iterator i;
	conf_iterator_section_begin(&i, config_context, section.c_str(), tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0))
			return false;
		cat.insert(cat.end(), a0);
		conf_iterator_next(&i);
	}
	return true;
}

static bool config_load_iterator_emu_include(adv_conf* config_context, const string& tag, emulator_container& emu)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string a0;
		string s = conf_iterator_string_get(&i);
		if (!config_split(s, a0))
			return false;
		emu.insert(emu.end(), a0);
		conf_iterator_next(&i);
	}
	return true;
}

bool config_state::load_iterator_game(adv_conf* config_context, const string& tag)
{
	unsigned ignored_count = 0;

	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s = conf_iterator_string_get(&i);
		int j = 0;
		string game = arg_get(s, j);
		
		string sfavorites = arg_get(s, j); //OJO: en el futuro grupos
		
		string type = arg_get(s, j);
		string time = arg_get(s, j);
		string session = arg_get(s, j);
		string desc = arg_get(s, j);

		if (j != s.length()) {
			config_error_a(s);
			return false;
		}

		if (game.length()==0) {
			config_error_a(s);
			return false;
		}

		if (!load_game(game, type, time, session, desc)) {
			if (!quiet) {
				++ignored_count;
				/*
				if (ignored_count < 10)
					//target_err("Ignoring info for game '%s'.\n", game.c_str()); //Mensagem de erro se o emulador não existir e tiver jogo no .rc WARLOCK
				else if (ignored_count == 10)
					//target_err("And also for other games.\n"); //warlock
			*/
			}
		}

		conf_iterator_next(&i);
	}

	//if (ignored_count > 0)
		//target_err("You may lose the game sessions and time information.\n"); //warlock

	return true;
}

bool config_state::load_iterator_script(adv_conf* config_context, const string& tag)
{
	adv_conf_iterator i;
	conf_iterator_begin(&i, config_context, tag.c_str());
	while (!conf_iterator_is_end(&i)) {
		string s = conf_iterator_string_get(&i);

		int j = 0;
		string name = arg_get(s, j);
		if (name.length() == 0) {
			config_error_a(s);
			return false;
		}

		string text = string(s, j);

		script_bag.insert(script_bag.end(), script(name, text));

		conf_iterator_next(&i);
	}

	return true;
}

static bool config_load_orientation(adv_conf* config_context, unsigned& mask)
{
	string s;
	int i;

	s = conf_string_get_default(config_context, "display_orientation");
	i = 0;
	mask = 0;
	while (i<s.length()) {
		string arg = arg_get(s, i);
		if (arg == "flip_xy")
			mask ^= ADV_ORIENTATION_FLIP_XY;
		else if (arg == "mirror_x")
			mask ^= ADV_ORIENTATION_FLIP_X;
		else if (arg == "mirror_y")
			mask ^= ADV_ORIENTATION_FLIP_Y;
		else {
			config_error_la("display_orientation " + s, arg);
			return false;
		}
	}

	return true;
}

bool config_state::load_custom(adv_conf* config_context, const string& path_archivo_custom)
{
	//mira si ya hay un layout con el mismo nombre (la ruta al *.amp)
	for(playout_container::iterator j = lay_cont.begin();j!=lay_cont.end();j++) {
		if((*j)->name_get() == path_archivo_custom) {
			return true;
		}
	}

	//crea un nuevo layout con el nombre de la ruta al archivo amp
	layout* lay = new layout(path_archivo_custom);

	//rellena los datos del layout
	unsigned i;
	for(i=0; OPTION_LAYOUT[i].option; ++i)
	{
		if (!config_load_iterator_layout_set( config_context, OPTION_LAYOUT[i].option, lay, OPTION_LAYOUT[i].set))
			return false;
	}
	
	//inserta el nuevo layout en la lista de layouts (playout_container)
	lay_cont.insert(lay_cont.end(), lay);
	
	return true;
}

bool config_state::load(adv_conf* config_context, bool opt_verbose)
{
	string a0, a1;

	preview_mask = 0;
	current_game = 0;
	current_clone = 0;
	fast = "";
	current_backdrop = resource();
	current_sound = resource();

	default_sort_orig = (listsort_t)conf_int_get_default(config_context, "sort");
	default_mode_orig = (listmode_t)conf_int_get_default(config_context, "mode");
	default_preview_orig = (listpreview_t)conf_int_get_default(config_context, "preview");
	
	double d = conf_float_get_default(config_context, "ui_translucency");
	ui_translucency = static_cast<int>(d * 255);
	if (ui_translucency > 255)
		ui_translucency = 255;
	if (!config_path(conf_string_get_default(config_context, "ui_background"), ui_back))
		return false;
	if (!config_path(conf_string_get_default(config_context, "ui_help"), ui_help))
		return false;
	if (!config_path(conf_string_get_default(config_context, "ui_exit"), ui_exit))
		return false;
	if (!config_path(conf_string_get_default(config_context, "ui_startup"), ui_startup))
		return false;
	ui_left = conf_int_get_default(config_context, "ui_skipleft");
	ui_right = conf_int_get_default(config_context, "ui_skipright");
	ui_top = conf_int_get_default(config_context, "ui_skiptop");
	ui_bottom = conf_int_get_default(config_context, "ui_skipbottom");
	ui_top_bar = conf_bool_get_default(config_context, "ui_topbar");
	ui_bottom_bar = conf_bool_get_default(config_context, "ui_bottombar");
	script_menu = conf_string_get_default(config_context, "ui_command_menu");
	script_error = conf_string_get_default(config_context, "ui_command_error");
	if (!load_iterator_script(config_context, "ui_command"))
		return false;
	lock_orig = (bool)conf_bool_get_default(config_context, "lock");
	restore = (restore_t)conf_int_get_default(config_context, "config");
	if (!config_load_skip(config_context, mode_skip_mask))
		return false;
	exit_mode = (exit_t)conf_int_get_default(config_context, "misc_exit");
	console_mode = conf_bool_get_default(config_context, "ui_console");
	menu_key = conf_bool_get_default(config_context, "ui_menukey");
	if (!config_load_iterator(config_context, "event_assign", event_in))
		return false;
	if (!config_load_iterator(config_context, "ui_color", color_in))
		return false;
	if (!config_split(conf_string_get_default(config_context, "idle_start"), a0, a1))
		return false;
	idle_start_first = atoi(a0.c_str());
	idle_start_rep = atoi(a1.c_str());
	// REM SELECTED
	rem_selected = conf_bool_get_default(config_context, "rem_selected");
	// carga la posicion del seleccionado para "rem_selected no"
	if (!config_split(conf_string_get_default(config_context, "menu_pos"), a0, a1))
		return false;
	menu_base_orig = atoi(a0.c_str());
	menu_rel_orig = atoi(a1.c_str());
	
	if (!config_split(conf_string_get_default(config_context, "idle_screensaver"), a0, a1))
		return false;
	idle_saver_first = atoi(a0.c_str());
	idle_saver_rep = atoi(a1.c_str());
	idle_exit_time = conf_int_get_default(config_context, "idle_exit_time");
	idle_exit_type = (idle_exit_t)conf_int_get_default(config_context, "idle_exit_type");
	if (!config_split(conf_string_get_default(config_context, "event_repeat"), a0, a1))
		return false;
	repeat = atoi(a0.c_str());
	repeat_rep = atoi(a1.c_str());
	disable_special = !conf_bool_get_default(config_context, "input_hotkey");
	if (!config_split(conf_string_get_default(config_context, "display_size"), a0, a1))
		return false;
	video_sizex = atoi(a0.c_str());
	video_sizey = atoi(a1.c_str());
	if (video_sizey == 0)
		video_sizey = video_sizex*3/4;
	if (video_sizex < 160 || video_sizex > 2048 || video_sizey < 90 || video_sizey > 1638) {
		target_err("Invalid argument '%s' for option 'display_size'\nValid arguments are size_x from 160 to 2048 and size_y from 90 to 1638\n", a0.c_str());
		return false;
	}
	
	if (!config_path(conf_string_get_default(config_context, "ui_font"), video_font_path))
		return false;
	if (!config_split(conf_string_get_default(config_context, "ui_fontsize"), a0, a1))
		return false;
	video_fonty = atoi(a0.c_str());
	video_fontx = atoi(a1.c_str());
	if (!config_load_orientation(config_context, video_orientation_orig))
		return false;
	video_gamma = conf_float_get_default(config_context, "display_gamma");
	video_brightness = conf_float_get_default(config_context, "display_brightness");
	resetexit = conf_bool_get_default(config_context, "display_restoreatexit");
	default_resetgame_unique = conf_bool_get_default(config_context, "display_restoreatgame");
	quiet = conf_bool_get_default(config_context, "misc_quiet");
	if (!config_split(conf_string_get_default(config_context, "ui_gamemsg"), ui_gamemsg))
		return false;
	ui_gamesaver = (saver_t)conf_int_get_default(config_context, "ui_game");
	difficulty_orig = (difficulty_t)conf_int_get_default(config_context, "difficulty");
	idle_saver_type = (saver_t)conf_int_get_default(config_context, "idle_screensaver_preview");
	preview_expand = conf_float_get_default(config_context, "preview_expand");
	if (!config_path_import(conf_string_get_default(config_context, "preview_default"), preview_default))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_snap"), preview_default_snap))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_flyer"), preview_default_flyer))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_cabinet"), preview_default_cabinet))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_icon"), preview_default_icon))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_marquee"), preview_default_marquee))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "preview_default_title"), preview_default_title))
		return false;

	if (!config_path_import(conf_string_get_default(config_context, "mame_history"), mame_history))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "mame_info"), mame_info))
		return false;
	
	if (!config_split(conf_string_get_default(config_context, "xml_dir"), xml_dir))
		return false;
	
	preview_fast = (bool)conf_int_get_default(config_context, "event_mode");
	alpha_mode = (bool)conf_bool_get_default(config_context, "event_alpha");
	clip_mode = (clip_mode_t)conf_int_get_default(config_context, "ui_clip");
	merge = (merge_t)conf_int_get_default(config_context, "romset_type");
	icon_space = conf_int_get_default(config_context, "icon_space");

	security_exit = conf_bool_get_default(config_context, "security_exit");
	
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_zgeral"), sound_foreground_zgeral))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_zerado"), sound_foreground_zerado))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_begin"), sound_foreground_begin))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_notime"), sound_foreground_notime))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_nocoin"), sound_foreground_nocoin))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "background_config_menu"), background_config_menu))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_coin"), sound_foreground_coin))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_end"), sound_foreground_end))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_key"), sound_foreground_key))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_start"), sound_foreground_start))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_foreground_stop"), sound_foreground_stop))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_background_begin"), sound_background_begin))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_background_end"), sound_background_end))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_background_start"), sound_background_start))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_background_stop"), sound_background_stop))
		return false;
	if (!config_path_import(conf_string_get_default(config_context, "sound_background_loop"), sound_background_loop))
		return false;
	if (!config_split(conf_string_get_default(config_context, "sound_background_loop_dir"), sound_background_loop_dir))
		return false;

	// MENU SYSTEMS
	menu_systems_activated = config_load_menu_systems(config_context, "menu_systems", menu_systems);
	if (!config_load_iterator_menu_systems_attrib(config_context, "menu_systems_attrib", menu_systems))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_altss", menu_systems, &systems::user_alts_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_flyers", menu_systems, &systems::user_flyer_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_cabinets", menu_systems, &systems::user_cabinet_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_icons", menu_systems, &systems::user_icon_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_marquees", menu_systems, &systems::user_marquee_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_titles", menu_systems, &systems::user_title_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_default_snap", menu_systems, &systems::user_default_snap_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_default_marquee", menu_systems, &systems::user_default_marquee_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_layout", menu_systems, &systems::custom_file_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_background", menu_systems, &systems::nocustom_background_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_help", menu_systems, &systems::nocustom_help_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_start", menu_systems, &systems::nocustom_start_path_set))
		return false;
	if (!config_load_menu_systems_path_set(config_context, "menu_systems_font", menu_systems, &systems::nocustom_font_path_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_font_size", menu_systems, &systems::nocustom_font_size_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_font_color", menu_systems, &systems::nocustom_font_color_set))
		return false;
	if (!config_load_menu_systems_set(config_context, "menu_systems_font_color_select", menu_systems, &systems::nocustom_font_color_select_set))
		return false;
	
	if (!config_load_iterator_emu(config_context, "emulator", emu))
		return false;
	if (!config_load_iterator_emu_attrib(config_context, "emulator_attrib", emu, quiet))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_roms", emu, &emulator::user_rom_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_roms_filter", emu, &emulator::user_rom_filter_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_altss", emu, &emulator::user_alts_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_flyers", emu, &emulator::user_flyer_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_cabinets", emu, &emulator::user_cabinet_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_icons", emu, &emulator::user_icon_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_marquees", emu, &emulator::user_marquee_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_titles", emu, &emulator::user_title_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_default_snap", emu, &emulator::user_default_snap_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_default_marquee", emu, &emulator::user_default_marquee_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_layout", emu, &emulator::custom_file_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_background", emu, &emulator::nocustom_background_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_help", emu, &emulator::nocustom_help_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_start", emu, &emulator::nocustom_start_path_set))
		return false;
	if (!config_load_iterator_emu_path_set(config_context, "emulator_font", emu, &emulator::nocustom_font_path_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_font_size", emu, &emulator::nocustom_font_size_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_font_color", emu, &emulator::nocustom_font_color_set))
		return false;
	if (!config_load_iterator_emu_set(config_context, "emulator_font_color_select", emu, &emulator::nocustom_font_color_select_set))
		return false;

	// carga las opciones de los emuladores del tipo (emulador/opcion valor)
	if (!menu_systems->config_get().load(config_context, config_normalize(menu_systems->user_name_get())))
		return false;
	for(pemulator_container::iterator i=emu.begin();i!=emu.end();++i) {
		if (!(*i)->config_get().load(config_context, config_normalize((*i)->user_name_get())))
			return false;
	}

	// print the copyright message before other messages
	if (!quiet) {
		target_nfo(ADV_COPY);
	}

	// select the active emulators
	for(pemulator_container::iterator i=emu.begin();i!=emu.end();++i) {
		
		if ((*i)->is_present()) {
			emu_active.insert(emu_active.end(), *i);
		} else {
			if (!quiet)
				target_err("Emulador '%s' não encontrado, ignorado do carregamento.\n", (*i)->user_exe_path_get().c_str());
		}
	}

	if (emu_active.size() == 0) {
		target_err("Nenhum emulador encontrado no sistema, caso tenha removido todos pelo menu de configuracao, então re-adicione indo no advmenu.ini na chave Remover_emuladores e mude o valor para 0 Zero.\n");
		return false;
	}

	// Comprueba si el directorio xml_path existe, si no: lo crea, si falla la creacion: xml_dir por defecto
	if (xml_dir == "default")
		xml_dir = "";
	
	if (xml_dir != "") {
		string dir = file_config_file_home((slash_add(xml_dir)).c_str());
		if (!dir.empty() && access(dir.c_str(), F_OK) != 0) {
			if (target_mkdir(dir.c_str()) != 0) {
				xml_dir = "";
				if (!quiet)
					target_err("Error creating dir %s\n", dir.c_str());
			}
		}
	}
	
	// load the game definitions
	for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();) {
		if (opt_verbose)
			target_nfo("log: load game for %s\n", (*i)->user_name_get().c_str());
		if (!(*i)->load_game(xml_dir, gar, quiet)) {
			if (!quiet)
				target_err("Emulator '%s' without game information, ignoring it.\n", (*i)->user_exe_path_get().c_str());
			pemulator_container::iterator j = i;
			++i;
			emu_active.erase(j);
		} else {
			++i;
		}
	}

	// load the emulator configurations
	for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();) {
		if (opt_verbose)
			target_nfo("log: load cfg for %s\n", (*i)->user_name_get().c_str());
		if (!(*i)->load_cfg(gar, quiet)) {
			if (!quiet)
				target_err("Emulator '%s' without configuration, ignoring it.\n", (*i)->user_exe_path_get().c_str());
			pemulator_container::iterator j = i;
			++i;
			emu_active.erase(j);
		} else {
			++i;
		}
	}

	// remove emulator without roms
	for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();) {
		if ((*i)->is_empty()) {
			if (!quiet)
				target_err("Emulator '%s' without rom files, ignoring it.\n", (*i)->user_exe_path_get().c_str());
			pemulator_container::iterator j = i;
			++i;
			emu_active.erase(j);
		} else {
			++i;
		}
	}

	// load the software definitions
	for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();) {
		if (opt_verbose)
			target_nfo("log: load software for %s\n", (*i)->user_name_get().c_str());
		if (!(*i)->load_software(gar, quiet)) {
			if (!quiet)
				target_err("Emulator '%s' without software information, ignoring it.\n", (*i)->user_exe_path_get().c_str());
			pemulator_container::iterator j = i;
			++i;
			emu_active.erase(j);
		} else {
			++i;
		}
	}

	if (emu_active.size() == 0) {
		target_err("No working emulator found. Adjust the `emulator' options in your configuration file. These options are documented in the `advmenu.txt' file.\n");
		return false;
	}

	// MENU SYSTEMS
	// establece los emuladores del Menu Systems como juegos
	for(pemulator_container::iterator j=emu_active.begin();j!=emu_active.end();++j) {
		string name = menu_systems->user_name_get() + "/" + emu_tolower((*j)->user_name_get());
		game g;
		g.name_set(name);
		g.auto_description_set((*j)->user_name_get());
		g.emulator_set(menu_systems);
		gar.insert(g);
	}
	// carga la configuracion, path de las imagenes, ...
	if (!menu_systems->load_cfg(gar, quiet)) {
		if (!quiet)
			target_err("Menu Systems without configuration.\n");
	}
	// carga la informacion de los emuladores LST/XML 
	if (!menu_systems->load_game(xml_dir, gar, quiet)) {
		if (!quiet)
			target_err("Menu Systems without emu information.\n");
	}
	// carga las previews del Menu Systems
	menu_systems->preview_set(gar);
	
	// cache some values and relations
	if (opt_verbose)
		target_nfo("log: cache\n");
	gar.cache(merge);

	// set the previews
	for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();++i) {
		if (opt_verbose)
			target_nfo("log: load preview for %s\n", (*i)->user_name_get().c_str());
		(*i)->preview_set(gar);
	}

	if (opt_verbose)
		target_nfo("log: load game types\n");

	if (!config_load_iterator_pcategory(config_context, "type", type))
		return false;
	if (!config_load_iterator_category(config_context, "type_include", default_include_type_orig))
		return false;
	if (default_include_type_orig.size() == 0) {
		for(pcategory_container::iterator i=type.begin();i!=type.end();++i)
			default_include_type_orig.insert((*i)->name_get());
	}

	if (opt_verbose)
		target_nfo("log: load games info\n");

	if (!load_iterator_game(config_context, "game"))
		return false;

	// set the type of all the remainig games
	for(game_set::const_iterator i=gar.begin();i!=gar.end();++i) {
		i->auto_type_set(type.undefined_get());
	}

	// load the emulators include
	if (menu_systems_activated) {
		include_emu_orig.clear();
		include_emu_orig.insert(include_emu_orig.end(), menu_systems->user_name_get());
	} else if (config_load_iterator_emu_include(config_context, "emulator_include", include_emu_orig)) {
		for(emulator_container::iterator i=include_emu_orig.begin();i!=include_emu_orig.end();) {
			if (!config_is_emulator(emu_active, *i)) {
				emulator_container::iterator j = i;
				++i;
				include_emu_orig.erase(j);
			} else {
				++i;
			}
		}
	} else {
		return false;
	}
	
	// if the set is empty add the first emulator
	if (include_emu_orig.size() == 0) {
		for(pemulator_container::iterator i=emu_active.begin();i!=emu_active.end();++i) {
			include_emu_orig.insert(include_emu_orig.end(), (*i)->user_name_get());
			break;
		}
	}

	if (!load_iterator_import(config_context, "desc_import", &config_state::import_desc, opt_verbose))
		return false;
	if (!load_iterator_import(config_context, "info_import", &config_state::import_info, opt_verbose))
		return false;
	if (!load_iterator_import(config_context, "type_import", &config_state::import_type, opt_verbose))
		return false;

	if (opt_verbose)
		target_nfo("log: load background music list\n");

	config_load_background_list(sound_background_loop_dir, sound_background);

	// ------------------------------------------- LOAD FAVORITES LISTS ---------------------------------------------
	if (opt_verbose)
		target_nfo("log: load favorites lists\n");

	if (!config_load_iterator_favorites(config_context, "favorites", favorites))
		return false;

	if (!config_split(conf_string_get_default(config_context, "favorites_include"), default_include_favorites_orig))
		return false;

	bool include_favorites_valid = false;
	for(pfavorites_container::const_iterator i=favorites.begin();i!=favorites.end();++i) {
		if ((*i)->name_get() == default_include_favorites_orig) {
			include_favorites_valid = true;
			break;
		}
	}
	
	if (!include_favorites_valid || default_include_favorites_orig == "") {
		default_include_favorites_orig = "All Games";
	}

	favorites_filtertype = conf_bool_get_default(config_context, "favorites_filtertype");
	
	if (!config_split(conf_string_get_default(config_context, "favorites_dir"), favorites_dir))
		return false;
	for(pfavorites_container::const_iterator i=favorites.begin();i!=favorites.end();++i) {
		if(!(*i)->import(favorites_dir))
			target_err("Failed to import data from the favorites list '%s'.\n", (*i)->name_get().c_str());
	}
	
	// ------------------------------------ LOAD REM SELECTED ---------------------------------------------
	// posicion del Menu Systems
	string name = config_normalize(menu_systems->user_name_get()); //config_normalize((*i)->name_get()) + "/" + config_normalize(menu_systems->user_name_get());
	pos_selected* ps = new pos_selected(name);
	const char* c;
	if (conf_string_section_get(config_context, ps->name.c_str(), "menu_pos", &c) == 0) {
		string a0, a1;
		if (config_split(c, a0, a1)) {
			ps->base = atoi(a0.c_str());
			ps->rel = atoi(a1.c_str());
		}
	}
	rem_pos.insert(rem_pos.end(), ps);
	// posicion de todos los pares fav/emu
	for(pfavorites_container::const_iterator i=favorites.begin();i!=favorites.end();++i) {
		for(pemulator_container::const_iterator j=emu.begin();j!=emu.end();++j) {
			string name = config_normalize((*i)->name_get()) + "/" + config_normalize((*j)->user_name_get());
			pos_selected* ps = new pos_selected(name);
			const char* c;
			if (conf_string_section_get(config_context, ps->name.c_str(), "menu_pos", &c) == 0) {
				string a0, a1;
				if (config_split(c, a0, a1)) {
					ps->base = atoi(a0.c_str());
					ps->rel = atoi(a1.c_str());
				}
			}
			rem_pos.insert(rem_pos.end(), ps);
		}
	}
	
	if (opt_verbose)
		target_nfo("log: start\n");

	return true;
}

void config_state::import_desc(const game& g, const string& text)
{
	g.auto_description_set(text);
}

void config_state::import_info(const game& g, const string& text)
{
	g.auto_info_set(text);
}

void config_state::import_type(const game& g, const string& text)
{
	g.auto_type_set(type.insert(text));
}

void config_state::conf_default(adv_conf* config_context)
{
	adv_conf_iterator i;

	conf_iterator_begin(&i, config_context, "emulator");
	if (conf_iterator_is_end(&i)) {
		char path[FILE_MAXPATH];
#if defined(__MSDOS__) || defined(__WIN32__)
		if (target_search(path, FILE_MAXPATH, "advmame.exe") == 0) {
			target_out("Adding emulator `advmame'...\n");
			conf_set(config_context, "", "emulator", "\"advmame\" advmame \"advmame.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "advmess.exe") == 0) {
			target_out("Adding emulator `advmess'...\n");
			conf_set(config_context, "", "emulator", "\"advmess\" advmess \"advmess.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "dmame.exe") == 0) {
			target_out("Adding emulator `dmame'...\n");
			conf_set(config_context, "", "emulator", "\"dmame\" dmame \"dmame.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "mame.exe") == 0) {
			target_out("Adding emulator `mame'...\n");
			conf_set(config_context, "", "emulator", "\"mame\" mame \"mame.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "mamepp.exe") == 0) {
			target_out("Adding emulator `mamepp'...\n");
			conf_set(config_context, "", "emulator", "\"mame\" mame \"mamepp.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "sdlmame.exe") == 0) {
			target_out("Adding emulator `sdlmam'...\n");
			conf_set(config_context, "", "emulator", "\"sdlmame\" sdlmame \"sdlmame.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "raine.exe") == 0) {
			target_out("Adding emulator `draine'...\n");
			conf_set(config_context, "", "emulator", "\"draine\" draine \"raine.exe\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "snes9x.exe") == 0) {
			target_out("Adding emulator `snes9x'...\n");
			conf_set(config_context, "", "emulator", "\"snes9x\" generic \"snes9x.exe\" \"%f\"");
			conf_set(config_context, "", "emulator_roms", "\"snes9x\" \"roms\"");
		}
		if (target_search(path, FILE_MAXPATH, "zsnes.exe") == 0) {
			target_out("Adding emulator `zsnes'...\n");
			conf_set(config_context, "", "emulator", "\"zsnes\" generic \"zsnes.exe\" \"-e -m roms\\%f\"");
			conf_set(config_context, "", "emulator_roms", "\"zsnes\" \"roms\"");
		}
#else
		if (target_search(path, FILE_MAXPATH, "advmame") == 0) {
			target_out("Adding emulator `advmame'...\n");
			conf_set(config_context, "", "emulator", "\"advmame\" advmame \"advmame\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "advmess") == 0) {
			target_out("Adding emulator `advmess'...\n");
			conf_set(config_context, "", "emulator", "\"advmess\" advmess \"advmess\" \"\"");
		}
		if (target_search(path, FILE_MAXPATH, "mame") == 0) {
			target_out("Adding emulator `sdlmame'...\n");
			conf_set(config_context, "", "emulator", "\"sdlmame\" sdlmame \"mame\" \"\"");
		}
#endif
	}

	conf_iterator_begin(&i, config_context, "favorites");
	if (conf_iterator_is_end(&i)) {
		conf_set(config_context, "", "favorites", "\"Favorites\"");
	}

	conf_iterator_begin(&i, config_context, "type");
	if (conf_iterator_is_end(&i)) {
		conf_set(config_context, "", "type", "\"Computer\"");
		conf_set(config_context, "", "type", "\"Console\"");
		conf_set(config_context, "", "type", "\"Application\"");
		conf_set(config_context, "", "type", "\"Arcade\"");
		conf_set(config_context, "", "type", "\"Shot 'em Up\"");
		conf_set(config_context, "", "type", "\"Bet 'em Up\"");
		conf_set(config_context, "", "type", "\"Fight\"");
		conf_set(config_context, "", "type", "\"Gun\"");
		conf_set(config_context, "", "type", "\"Puzzle\"");
		conf_set(config_context, "", "type", "\"RPG\"");
		conf_set(config_context, "", "type", "\"Sport\"");
		conf_set(config_context, "", "type", "\"Breakout\"");
		conf_set(config_context, "", "type", "\"Filler\"");
		conf_set(config_context, "", "type", "\"Racing\"");
		conf_set(config_context, "", "type", "\"Flipper\"");
		conf_set(config_context, "", "type", "\"-LISTA COMPLETA-\"");
	}

	conf_iterator_begin(&i, config_context, "ui_color");
	if (conf_iterator_is_end(&i)) {
		color_out(config_context, "ui_color");
	}

	conf_iterator_begin(&i, config_context, "event_assign");
	if (conf_iterator_is_end(&i)) {
		event_out(config_context, "event_assign");
	}
}

// -------------------------------------------------------------------------
// Configuration save

bool config_state::save_favorites() const
{
	for(pfavorites_container::const_iterator i=favorites.begin();i!=favorites.end();++i) {
		if(!(*i)->save(favorites_dir))
			return false;			
	}
	return true;
}

bool config_state::save(adv_conf* config_context) const
{
	conf_int_set(config_context, "", "mode", default_mode_orig);
	conf_int_set(config_context, "", "sort", default_sort_orig);
	conf_int_set(config_context, "", "preview", default_preview_orig);
	
	conf_remove(config_context, "", "favorites_include");
	conf_string_set(config_context, "", "favorites_include", config_out(default_include_favorites_orig).c_str());
	
	//Salva o volume
	conf_int_set(config_context, "", "sound_volume", play_attenuation_get());

	conf_remove(config_context, "", "type_include");
	for(category_container::const_iterator i=default_include_type_orig.begin();i!=default_include_type_orig.end();++i) {
		conf_string_set(config_context, "", "type_include", config_out(*i).c_str());
	}

	// MENU SYSTEMS
	conf_bool_set(config_context, "", "menu_systems", menu_systems_activated);
	conf_remove(config_context, "", "menu_systems_attrib");
	menu_systems->attrib_get(config_context, "", "menu_systems_attrib");
	
	// REM SELECTED ---------------------------------------------------------------------------

	// guarda la posicion del seleccionado para el "rem_selected no"
	ostringstream fnorem;
	fnorem << menu_base_orig << " " << menu_rel_orig;
	conf_string_set(config_context, "", "menu_pos", fnorem.str().c_str());

	// guarda las posiciones de los pares fav/emu
	if(rem_selected) {
		for(ppos_selected_container::const_iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
			if((*i)->base == 0 && (*i)->rel == 0) {
				conf_remove(config_context, (*i)->name.c_str(), "menu_pos");
			} else {
				ostringstream frem;
				frem << (*i)->base << " " << (*i)->rel;
				conf_string_set(config_context, (*i)->name.c_str(), "menu_pos", frem.str().c_str());
			}
		}
	}

	// borra las opciones obsoletas del rc
	conf_remove(config_context, "", "menu_base");
	conf_remove(config_context, "", "menu_rel");

	// ---------------------------------------------------------------------------------------------------------
	
	conf_int_set(config_context, "", "difficulty", difficulty_orig);

	conf_bool_set(config_context, "", "favorites_filtertype", favorites_filtertype);

	conf_bool_set(config_context, "", "security_exit", security_exit);

	// guarda las opciones de los emuladores del tipo (emulador/opcion valor) del rc
	menu_systems->config_get().save(config_context, config_normalize(menu_systems->user_name_get()));
	for(pemulator_container::const_iterator i=emu.begin();i!=emu.end();++i) {
		(*i)->config_get().save(config_context, config_normalize((*i)->user_name_get()));
	}

	conf_remove(config_context, "", "emulator_include");
	for(emulator_container::const_iterator i=include_emu_orig.begin();i!=include_emu_orig.end();++i) {
		conf_string_set(config_context, "", "emulator_include", config_out(*i).c_str());
	}

	conf_remove(config_context, "", "type");
	for(pcategory_container::const_iterator i=type.begin();i!=type.end();++i) {
		conf_string_set(config_context, "", "type", config_out((*i)->name_get()).c_str());
	}

	conf_remove(config_context, "", "emulator_attrib");
	for(pemulator_container::const_iterator i=emu.begin();i!=emu.end();++i) {
		(*i)->attrib_get(config_context, "", "emulator_attrib");
	}

	string s;
	if ((video_orientation_orig & ADV_ORIENTATION_FLIP_XY) != 0) {
		if (s.length()) s += " ";
		s += "flip_xy";
	}
	if ((video_orientation_orig & ADV_ORIENTATION_FLIP_X) != 0) {
		if (s.length()) s += " ";
		s += "mirror_x";
	}
	if ((video_orientation_orig & ADV_ORIENTATION_FLIP_Y) != 0) {
		if (s.length()) s += " ";
		s += "mirror_y";
	}

	conf_string_set(config_context, "", "display_orientation", s.c_str());

	conf_remove(config_context, "", "game");
	for(game_set::const_iterator i=gar.begin();i!=gar.end();++i) {
		if (0
			|| i->is_user_type_set()
			|| i->is_time_set()
			|| i->is_session_set()
			|| (i->is_user_description_set() && i->description_get().length()!=0)
		) {
			ostringstream f;
			f << "\"" << i->name_get() << "\"";

			f << " \"\""; //apartir de ahora los datos de los favoritos no se guarda en el RC

			f << " \"";
			if (i->is_user_type_set())
				f << i->type_get()->name_get();
			f << "\"";

			f << " " << i->time_get();

			f << " " << i->session_get();

			f << " \"";
			if (i->is_user_description_set()) {
				 f << i->description_get();
			}
			f << "\"";

			conf_string_set(config_context, "", "game", f.str().c_str());
		}
	}

	// don't print the error message (error_callback==0)
	if (conf_save(config_context, 1, 0, 0, 0) != 0)
		return false;

	// prevent data lost if crashing
	target_sync();

	return true;
}

// ------------------------------------------------------------------------
// Configuration restore

void config_state::restore_load()
{
	default_sort_effective = default_sort_orig;
	default_mode_effective = default_mode_orig;
	default_preview_effective = default_preview_orig;
	default_include_favorites_effective = default_include_favorites_orig;
	default_include_type_effective = default_include_type_orig;
	difficulty_effective = difficulty_orig;
	include_emu_set(include_emu_orig);
	menu_base_effective = menu_base_orig;
	menu_rel_effective = menu_rel_orig;
	lock_effective = lock_orig;
	video_orientation_effective = video_orientation_orig;
	// Menu Systems
	menu_systems->config_get().restore_load();
	menu_systems->attrib_load();
	for(pemulator_container::const_iterator i=emu.begin();i!=emu.end();++i) {
		(*i)->config_get().restore_load();
		(*i)->attrib_load();
	}
}

void config_state::restore_save_default()
{
	default_sort_orig = default_sort_effective;
	default_mode_orig = default_mode_effective;
	default_preview_orig = default_preview_effective;
	default_include_favorites_orig = default_include_favorites_effective;
	default_include_type_orig = default_include_type_effective;
	difficulty_orig = difficulty_effective;
	include_emu_orig = include_emu_effective;
	menu_base_orig = menu_base_effective;
	menu_rel_orig = menu_rel_effective;
	lock_orig = lock_effective;
	video_orientation_orig = video_orientation_effective;
	// Menu Systems
	menu_systems->attrib_save();
	for(pemulator_container::const_iterator i=emu.begin();i!=emu.end();++i) {
		(*i)->attrib_save();
	}
}

void config_state::restore_save()
{
	restore_save_default();

	menu_systems->config_get().restore_save();
	for(pemulator_container::const_iterator i=emu.begin();i!=emu.end();++i) {
		(*i)->config_get().restore_save();
	}
}

// ------------------------------------------------------------------------
// Configuration state

config_state::config_state()
	: sub_emu(0)
{
}

config_state::~config_state()
{
	// delete the favorites
	for(pfavorites_container::iterator i=favorites.begin();i!=favorites.end();++i) {
		delete *i;
	}

	// delete the Menu Systems
	menu_systems = 0;
	delete menu_systems;

	// delete the emulators
	for(pemulator_container::iterator i=emu.begin();i!=emu.end();++i) {
		delete *i;
	}
	
	// delete the types
	for(pcategory_container::iterator i=type.begin();i!=type.end();++i) {
		delete *i;
	}

	// delete the position selected
	for(ppos_selected_container::iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
		delete *i;
	}
}

void config_state::sub_disable()
{
	sub_emu = 0;
}

void config_state::sub_enable()
{
	if (include_emu_effective.size() == 1) {
		sub_emu = 0;
		if (menu_systems->user_name_get() == *include_emu_effective.begin()) {
			sub_emu = menu_systems;
		} else {
			for(pemulator_container::iterator i=emu.begin();i!=emu.end();++i) {
				if ((*i)->user_name_get() == *include_emu_effective.begin()) {
					sub_emu = *i;
					break;
				}
			}
		}
	} else if (include_emu_effective.size() == 0 && emu_active.size() == 1) {
		sub_emu = *emu_active.begin();
	} else {
		sub_emu = 0;
	}
}

void config_state::include_emu_set(const emulator_container& A)
{
	include_emu_effective = A;

	sub_enable();
}

const emulator_container& config_state::include_emu_get()
{
	return include_emu_effective;
}

listsort_t config_state::sort_get()
{
	if (sub_has() && sub_get().sort_has())
		return sub_get().sort_get();
	else
		return default_sort_effective;
}

listmode_t config_state::mode_get()
{
	if (sub_has() && sub_get().mode_has())
		return sub_get().mode_get();
	else
		return default_mode_effective;
}

// REM SELECTED
string config_state::namepos_get() 
{
	string list_name = config_normalize(include_favorites_get());
	string emu_name = config_normalize(*include_emu_get().begin());
	string menu_systems_name = config_normalize(menu_systems->user_name_get());
	return (emu_name == menu_systems_name) ? emu_name : list_name + "/" + emu_name;
}
//get base
int config_state::menu_base_get()
{
	string namepos = namepos_get();

	for(ppos_selected_container::const_iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
		if((*i)->name == namepos)
			return (*i)->base;
	}
	return 0;
}
//get rel
int config_state::menu_rel_get()
{
	string namepos = namepos_get();
	
	for(ppos_selected_container::const_iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
		if((*i)->name == namepos)
			return (*i)->rel;
	}
	return 0;
}
//get pos
void config_state::menu_pos_get(int& base, int& rel)
{
	string namepos = namepos_get();

	for(ppos_selected_container::const_iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
		if((*i)->name == namepos) {
			base = (*i)->base;
			rel = (*i)->rel;
			break;
		}
	}
}
//set pos
void config_state::menu_pos_set(int& base, int& rel)
{
	string namepos = namepos_get();

	for(ppos_selected_container::const_iterator i=rem_pos.begin();i!=rem_pos.end();++i) {
		if((*i)->name == namepos) {
			(*i)->base = base;
			(*i)->rel = rel;
			break;
		}
	}
}

listpreview_t config_state::preview_get()
{
	if (sub_has() && sub_get().preview_has())
		return sub_get().preview_get();
	else
		return default_preview_effective;
}

const category_container& config_state::include_type_get()
{
	if (sub_has() && sub_get().include_type_has())
		return sub_get().include_type_get();
	else
		return default_include_type_effective;
}

bool config_state::resetgame_get(const game* g)
{
	// In multiple emulator listing, returns always the specific
	// emulator value depending on the game selected.
	if (!g)
		return default_resetgame_unique;

	if (!g->emulator_get()->config_get().resetgame_has())
		return default_resetgame_unique;

	return g->emulator_get()->config_get().resetgame_get();
}

bool config_state::resetexit_get()
{
	return resetexit;
}

merge_t config_state::merge_get()
{
	if (sub_has() && sub_get().merge_has())
		return sub_get().merge_get();
	else
		return merge;
}

void config_state::sort_set(listsort_t A)
{
	if (sub_has())
		sub_get().sort_set(A);
	else
		default_sort_effective = A;
}

void config_state::mode_set(listmode_t A)
{
	if (sub_has())
		sub_get().mode_set(A);
	else
		default_mode_effective = A;
}

void config_state::preview_set(listpreview_t A)
{
	if (sub_has())
		sub_get().preview_set(A);
	else
		default_preview_effective = A;
}

void config_state::include_type_set(const category_container& A)
{
	if (sub_has())
		sub_get().include_type_set(A);
	else
		default_include_type_effective = A;
}

void config_state::merge_set(merge_t A)
{
	if (sub_has())
		sub_get().merge_set(A);
}

// ------------------------------------------------------------------------
// config_import

config_import::config_import(const std::string Atype, const std::string Aemulator, const std::string Afile, const std::string Asection)
	: type(Atype), emulator(Aemulator), file(Afile), section(Asection) {
}

void config_import::import_catver(game_set& gar, config_state& config, void (config_state::*set)(const game&, const std::string&))
{
	int j = 0;

	string ss = file_read(file);

	bool in = false;
	while (j < ss.length()) {
		string s = token_get(ss, j, "\r\n");
		token_skip(ss, j, "\r\n");

		int i = 0;
		token_skip(s, i, " \t");

		if (i<s.length() && s[i]=='[') {
			token_skip(s, i, "[");
			string cmd = token_get(s, i, "]");
			in = cmd == section;
		} else if (in && i<s.length() && isalnum(s[i])) {
			string tag = token_get(s, i, " =");
			token_skip(s, i, " =");
			string text = token_get(s, i, "");
			if (text.length()) {
				string name = emulator + "/" + tag;
				game_set::const_iterator k = gar.find(game(name));
				if (k!=gar.end()) {
					(config.*set)(*k, text);
				}
			}
		}
	}
}

void config_import::import_catlist(game_set& gar, config_state& config, void (config_state::*set)(const game&, const std::string&))
{
	int j = 0;

	string ss = file_read(file);
	string cat;

	while (j < ss.length()) {
		string s = token_get(ss, j, "\r\n");
		token_skip(ss, j, "\r\n");

		int i = 0;
		token_skip(s, i, " \t");

		if (i<s.length() && s[i]=='[') {
			token_skip(s, i, "[");
			cat = token_get(s, i, "]");
		} else if (cat.length() && i<s.length() && isalnum(s[i])) {
			string tag = token_get(s, i, " \t");
			token_skip(s, i, " \t");
			if (i==s.length() && tag.length() != 0) {
				string name = emulator + "/" + tag;
				game_set::const_iterator k = gar.find(game(name));
				if (k!=gar.end()) {
					(config.*set)(*k, cat);
				}
			}
		}
	}
}

void config_import::import_mac(game_set& gar, config_state& config, void (config_state::*set)(const game&, const std::string&))
{
	int j = 0;

	string ss = file_read(file);

	string main_text;
	while (j < ss.length()) {

		string s = token_get(ss, j, "\r\n");
		token_skip(ss, j, "\r\n");

		int i = 0;
		token_skip(s, i, " \t");

		if (i<s.length() && isalnum(s[i])) {
			string tag = token_get(s, i, " \t");
			token_skip(s, i, " \t");
			string text = token_get(s, i, "");
			if (text.length()) {
				main_text = text;
				// remove special chars
				if (main_text.length() && main_text[main_text.length()-1]=='-')
					main_text.erase(main_text.length()-1, 1);
				if (main_text.length() && main_text[0] == '¥')
					main_text.erase(0, 1);
			}
			if (main_text.length()) {
				string name = emulator + "/" + tag;
				game_set::const_iterator k = gar.find(game(name));
				if (k!=gar.end()) {
					(config.*set)(*k, main_text);
				}
			}
		}
	}
}

void config_import::import_nms(game_set& gar, config_state& config, void (config_state::*set)(const game&, const std::string&))
{
	int j = 0;

	string ss = file_read(file);

	while (j < ss.length()) {
		string s = token_get(ss, j, "\r\n");
		token_skip(ss, j, "\r\n");

		int i = 0;
		token_skip(s, i, " \t");

		string text = token_get(s, i, '|');
		token_skip(s, i, "|");
		string tag = token_get(s, i, "");

		if (text.length() && tag.length()) {
			string name = emulator + "/" + tag;
			game_set::iterator k = gar.find(game(name));
			if (k!=gar.end()) {
				(config.*set)(*k, text);
			}
		}
	}
}

void config_import::import(game_set& gar, config_state& config, void (config_state::*set)(const game&, const std::string&))
{
	if (type == "ini" || type == "catver")
		import_catver(gar, config, set);
	else if (type == "catlist")
		import_catlist(gar, config, set);
	else if (type == "mac")
		import_mac(gar, config, set);
	else if (type == "nms")
		import_nms(gar, config, set);
}

bool config_emulator_state::load(adv_conf* config_context, const string& section)
{
	int i;
	adv_bool b;
	const char* c;

	if (conf_int_section_get(config_context, section.c_str(), "sort", &i) == 0) {
		sort_set_orig = true;
		sort_orig = (listsort_t)i;
	} else {
		sort_set_orig = false;
	}

	if (conf_int_section_get(config_context, section.c_str(), "mode", &i) == 0) {
		mode_set_orig = true;
		mode_orig = (listmode_t)i;
	} else {
		mode_set_orig = false;
	}

	if (conf_int_section_get(config_context, section.c_str(), "preview", &i) == 0) {
		preview_set_orig = true;
		preview_orig = (listpreview_t)i;
	} else {
		preview_set_orig = false;
	}

	if (!config_load_iterator_category_section(config_context, section, "type_include", include_type_orig))
		return false;
	if (include_type_orig.size() != 0) {
		include_type_set_orig = true;
	} else {
		include_type_set_orig = false;
	}

	if (conf_bool_section_get(config_context, section.c_str(), "display_restoreatgame", &b) == 0) {
		resetgame_set_unique = true;
		resetgame_unique = b;
	} else {
		resetgame_set_unique = false;
	}

	if (conf_int_section_get(config_context, section.c_str(), "romset_type", &i) == 0) {
		merge_set_orig = true;
		merge_orig = (merge_t)i;
		merge_set_effective = true;
		merge_effective = (merge_t)i;
	} else {
		merge_set_orig = false;
		merge_set_effective = false;
	}

	return true;
}

void config_emulator_state::save(adv_conf* config_context, const string& section)
{
	if (mode_set_orig) {
		conf_int_set(config_context, section.c_str(), "mode", mode_orig);
	} else {
		conf_remove(config_context, section.c_str(), "mode");
	}


	// REM SELECTED
	// borra las opciones obsoletas del rc
	conf_remove(config_context, section.c_str(), "menu_base");
	conf_remove(config_context, section.c_str(), "menu_rel");
	
	if (sort_set_orig) {
		conf_int_set(config_context, section.c_str(), "sort", sort_orig);
	} else {
		conf_remove(config_context, section.c_str(), "sort");
	}

	if (preview_set_orig) {
		conf_int_set(config_context, section.c_str(), "preview", preview_orig);
	} else {
		conf_remove(config_context, section.c_str(), "preview");
	}

	conf_remove(config_context, section.c_str(), "type_include");
	if (include_type_set_orig) {
		for(category_container::const_iterator i=include_type_orig.begin();i!=include_type_orig.end();++i) {
			conf_string_set(config_context, section.c_str(), "type_include", config_out(*i).c_str());
		}
	}

	if (merge_set_orig) {
		conf_int_set(config_context, section.c_str(), "romset_type", merge_orig);
	} else {
		conf_remove(config_context, section.c_str(), "romset_type");
	}
	
}

void config_emulator_state::restore_load()
{
	sort_effective = sort_orig;
	mode_effective = mode_orig;
	preview_effective = preview_orig;
	include_type_effective = include_type_orig;
	merge_effective = merge_orig;
	sort_set_effective = sort_set_orig;
	mode_set_effective = mode_set_orig;
	preview_set_effective = preview_set_orig;
	include_type_set_effective = include_type_set_orig;
	merge_set_effective = merge_set_orig;
}

void config_emulator_state::restore_save()
{
	sort_orig = sort_effective;
	mode_orig = mode_effective;
	preview_orig = preview_effective;
	include_type_orig = include_type_effective;
	merge_orig = merge_effective;
	sort_set_orig = sort_set_effective;
	mode_set_orig = mode_set_effective;
	preview_set_orig = preview_set_effective;
	include_type_set_orig = include_type_set_effective;
	merge_set_orig = merge_set_effective;
}

bool config_emulator_state::sort_has()
{
	return sort_set_effective;
}

bool config_emulator_state::mode_has()
{
	return mode_set_effective;
}

bool config_emulator_state::preview_has()
{
	return preview_set_effective;
}

bool config_emulator_state::include_type_has()
{
	return include_type_set_effective;
}

bool config_emulator_state::resetgame_has()
{
	return resetgame_set_unique;
}

listsort_t config_emulator_state::sort_get()
{
	return sort_effective;
}

listmode_t config_emulator_state::mode_get()
{
	return mode_effective;
}

listpreview_t config_emulator_state::preview_get()
{
	return preview_effective;
}

const category_container& config_emulator_state::include_type_get()
{
	return include_type_effective;
}

bool config_emulator_state::resetgame_get()
{
	return resetgame_unique;
}

void config_emulator_state::sort_set(listsort_t A)
{
	sort_set_effective = true;
	sort_effective = A;
}

void config_emulator_state::mode_set(listmode_t A)
{
	mode_set_effective = true;
	mode_effective = A;
}

void config_emulator_state::preview_set(listpreview_t A)
{
	preview_set_effective = true;
	preview_effective = A;
}

void config_emulator_state::include_type_set(const category_container& A)
{
	include_type_set_effective = true;
	include_type_effective = A;
}

void config_emulator_state::sort_unset()
{
	sort_set_effective = false;
}

void config_emulator_state::mode_unset()
{
	mode_set_effective = false;
}

void config_emulator_state::preview_unset()
{
	preview_set_effective = false;
}

void config_emulator_state::include_type_unset()
{
	include_type_set_effective = false;
}

// romset_type
bool config_emulator_state::merge_has()
{
	return merge_set_effective;
}
//
merge_t config_emulator_state::merge_get()
{
	return merge_effective;
}
//
void config_emulator_state::merge_set(merge_t A)
{
	merge_set_effective = true;
	merge_effective = A;
}
//
void config_emulator_state::merge_unset()
{
	merge_set_effective = false;
}

void invertir_colores(string& s, string& color) {
	string c0, c1;
	if(config_split(color, c0, c1))
		s = c1 + " " + c0;
}
