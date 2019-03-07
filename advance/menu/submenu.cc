/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999, 2000, 2001, 2002, 2003 Andrea Mazzoleni
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

#include "submenu.h"
#include "text.h"
#include "play.h"

#include "advance.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windows.h> //warlock
using namespace std;

#define MSG_CHOICE_DX 30*int_font_dx_get()
#define MSG_CHOICE_X (int_dx_get()-MSG_CHOICE_DX)/2
#define MSG_CHOICE_Y int_dy_get()/2

string menu_name(config_state& rs, const string& s, unsigned event)
{
	if (!rs.menu_key)
		return s;

	string name = event_name(event);
	if (!name.length())
		return s;

	return s + "^" + name;
}

// ------------------------------------------------------------------------
// Sort menu

#define SORT_CHOICE_DX 15*int_font_dx_get()

int run_sort(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Parent", sort_by_root_name));
	ch.insert(ch.end(), choice("Emulator", sort_by_emulator));
	ch.insert(ch.end(), choice("Name", sort_by_name));
	ch.insert(ch.end(), choice("Time played", sort_by_time));
	ch.insert(ch.end(), choice("Play times", sort_by_session));
	ch.insert(ch.end(), choice("Time per play", sort_by_timepersession));
	ch.insert(ch.end(), choice("Type", sort_by_type));
	ch.insert(ch.end(), choice("Manufacturer", sort_by_manufacturer));
	ch.insert(ch.end(), choice("Year", sort_by_year));
	ch.insert(ch.end(), choice("Size", sort_by_size));
	ch.insert(ch.end(), choice("Resolution", sort_by_res));
	ch.insert(ch.end(), choice("Info", sort_by_info));

	choice_bag::iterator i = ch.find_by_value(rs.sort_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Select Sort Mode", THIRD_CHOICE_X, THIRD_CHOICE_Y, SORT_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.sort_set((listsort_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Command menu

#define COMMAND_CHOICE_DX 33*int_font_dx_get()

int run_command(config_state& rs)
{
	choice_bag ch;
	
	for(script_container::iterator i=rs.script_bag.begin();i!=rs.script_bag.end();++i) {
		if (i->text.find("%s") != string::npos) {
			if (!rs.current_game)
				continue;
		}
		if (i->text.find("%p") != string::npos || i->text.find("%f") != string::npos) {
			if (!rs.current_game)
				continue;
			if (rs.current_game->rom_zip_set_get().size() == 0)
				continue;
		}
		ch.insert(ch.end(), choice(i->name, &*i));
	}
 
	ch.insert(ch.end(), choice("configuracoes e leitura", 3));
	ch.insert(ch.end(), choice("testar controle 1", 4));
	ch.insert(ch.end(), choice("testar controle 2", 5));
	ch.insert(ch.end(), choice("testar controle 3", 6));
	ch.insert(ch.end(), choice("remover emuladores", 7));
	ch.insert(ch.end(), choice("mouse normal", 8));
	ch.insert(ch.end(), choice("mouse invisivel", 9));
	ch.insert(ch.end(), choice("iniciar arcade com o windows", 10));
	ch.insert(ch.end(), choice("remover arcade da inicializacao", 11));
	ch.insert(ch.end(), choice("reiniciar maquina", 12));
	ch.insert(ch.end(), choice("desligar maquina", 13));
	ch.insert(ch.end(), choice("mudar resolucao pc 640x480", 14));
	ch.insert(ch.end(), choice("mudar resolucao pc 800x600", 15));
	ch.insert(ch.end(), choice("mudar resolucao pc 1024x768", 16));
	ch.insert(ch.end(), choice("zerar creditos", 0));
	ch.insert(ch.end(), choice("zerar contador geral", 1));
	ch.insert(ch.end(), choice("sair", 2));

	if (ch.begin() == ch.end())
		ch.insert(ch.end(), choice("No commands available", -1));

	choice_bag::iterator i = ch.begin();

	int key = ch.run(string(" ") + rs.script_menu, (int_dx_get() - COMMAND_CHOICE_DX) / 2, int_dy_get() /5 , COMMAND_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		int r;
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			
			case 0 : 
				WritePrivateProfileStringA("FICHEIRO","FICHAS","0",".\\advmenu.ini");		
				sndPlaySoundA( ".\\auxiliar\\som\\zeracredito.wav", SND_ASYNC || SND_NODEFAULT );		
				r = 0;
				break;
			case 1 :
				WritePrivateProfileStringA("FICHEIRO","CONTADOR","0",".\\advmenu.ini");		
				sndPlaySoundA( ".\\auxiliar\\som\\salvo_opçao.wav", SND_ASYNC || SND_NODEFAULT );		
				r = 0;
				break;
			case 2 :
				r = 0;
				break;
			case 3 :
				ShellExecuteA(0, "open", "arcade.exe", " configuracao", ".\\", SW_SHOW);
				r = 0;
				break;
			case 4 :
				ShellExecuteA(0, "open", "arcade.exe", " joystickone", ".\\", SW_SHOW);
				r = 0;
				break;
			case 5 :
				ShellExecuteA(0, "open", "arcade.exe", " joysticktwo", ".\\", SW_SHOW);
				r = 0;
				break;
			case 6 :
				ShellExecuteA(0, "open", "arcade.exe", " joystickthree", ".\\", SW_SHOW);
				r = 0;
				break;
			case 7 :
				ShellExecuteA(0, "open", "arcade.exe", " remover_emulador", ".\\", SW_SHOW);
				r = 0;
				break;
			case 8 :
				ShellExecuteA(0, "open", "arcade.exe", " cursor_normal", ".\\", SW_SHOW);
				r = 0;
				break;
			case 9 :
				ShellExecuteA(0, "open", "arcade.exe", " cursor_inv", ".\\", SW_SHOW);
				r = 0;
				break;
			case 10 :
				ShellExecuteA(0, "open", "arcade.exe", " iniciar_arcade_on", ".\\", SW_SHOW);
				r = 0;
				break;
			case 11 :
				ShellExecuteA(0, "open", "arcade.exe", " iniciar_arcade_off", ".\\", SW_SHOW);
				r = 0;
				break;
			case 12 :
				ShellExecuteA(0, "open", "arcade.exe", " reiniciar_maquina", ".\\", SW_SHOW);
				r = 0;
				break;
			case 13 :
				ShellExecuteA(0, "open", "arcade.exe", " desligar_maquina", ".\\", SW_SHOW);
				r = 0;
				break;
			case 14 :
				ShellExecuteA(0, "open", "arcade.exe", " change_reso640x480", ".\\", SW_SHOW);
				r = 0;
				break;
			case 15 :
				ShellExecuteA(0, "open", "arcade.exe", " change_reso800x600", ".\\", SW_SHOW);
				r = 0;
				break;
			case 16 :
				ShellExecuteA(0, "open", "arcade.exe", " change_reso1024x768", ".\\", SW_SHOW);
				r = 0;
				break; 
			case -1 :
				r = 0;
				break;
			default:
				r = -1;
				break;
			}
		} else {
			script* s = static_cast<script*>(i->ptr_get());

			string text = s->text;

			if (rs.current_game) {
				text = subs(text, "%s", rs.current_game->name_without_emulator_get());
				if (rs.current_game->rom_zip_set_get().size()) {
					string path = *rs.current_game->rom_zip_set_get().begin();
					text = subs(text, "%p", path_export(path));
					text = subs(text, "%f", path_export(file_file(path)));
				}
			}

			int_unplug();

			r = target_script(text.c_str());

			int_plug();
		}

		if (r != 0) {
			choice_bag ch;
			ch.insert(ch.end(), choice(rs.script_error, 0));
			choice_bag::iterator i = ch.begin();
			ch.run(" Error", MSG_CHOICE_X, MSG_CHOICE_Y, MSG_CHOICE_DX, i);
		}
	}

	return key;
}

// ------------------------------------------------------------------------
// Mode menu

#define MODE_CHOICE_DX 15*int_font_dx_get()

int run_mode(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Full", mode_full));
	ch.insert(ch.end(), choice("Full Mixed", mode_full_mixed));
	ch.insert(ch.end(), choice("Text", mode_text));
	ch.insert(ch.end(), choice("List", mode_list));
	ch.insert(ch.end(), choice("List Mixed", mode_list_mixed));
	ch.insert(ch.end(), choice("Tile Tiny", mode_tile_tiny));
	ch.insert(ch.end(), choice("Tile Small", mode_tile_small));
	ch.insert(ch.end(), choice("Tile Normal", mode_tile_normal));
	ch.insert(ch.end(), choice("Tile Big", mode_tile_big));
	ch.insert(ch.end(), choice("Tile Enormous", mode_tile_enormous));
	ch.insert(ch.end(), choice("Tile Giant", mode_tile_giant));
	ch.insert(ch.end(), choice("Tile Icon", mode_tile_icon));
	ch.insert(ch.end(), choice("Tile Marquee", mode_tile_marquee));
	ch.insert(ch.end(), choice("Custom", mode_custom));

	choice_bag::iterator i = ch.find_by_value(rs.mode_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Select List Mode", THIRD_CHOICE_X, THIRD_CHOICE_Y, MODE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.mode_set((listmode_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Preview menu

#define PREVIEW_CHOICE_DX 15*int_font_dx_get()

int run_preview(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Snap", preview_snap));
	ch.insert(ch.end(), choice("Title", preview_title));
	ch.insert(ch.end(), choice("Flyer", preview_flyer));
	ch.insert(ch.end(), choice("Cabinet", preview_cabinet));
	ch.insert(ch.end(), choice("Icon", preview_icon));
	ch.insert(ch.end(), choice("Marquee", preview_marquee));

	choice_bag::iterator i = ch.find_by_value(rs.preview_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Select Preview Mode", THIRD_CHOICE_X, THIRD_CHOICE_Y, PREVIEW_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.preview_set((listpreview_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Favorites menu

#define FAVORITES_CHOICE_DX 20*int_font_dx_get()

int run_favorites(config_state& rs)
{
	choice_bag ch;

	for(pfavorites_container::const_iterator j = rs.favorites.begin();j!=rs.favorites.end();++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.include_favorites_get());
	if (i==ch.end())
		i = ch.begin();

	int key = ch.run(" Load List", THIRD_CHOICE_X, THIRD_CHOICE_Y, FAVORITES_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		string f = i->desc_get();
		rs.include_favorites_set(f);
	}

	return key;
}

void run_favorites_next(config_state& rs)
{
	bool with_or_without_games = false; // indica si carga la siguiente lista tenga o no juegos
	
	// si Menu Systems activado -> carga la lista siguiente tenga o no juegos
	// si Menu Systems desactivado -> carga la lista siguiente con juegos
	with_or_without_games = rs.menu_systems_activated;
	
	string next_select = "";
	bool pred_in = false;

	for(pfavorites_container::const_iterator j=rs.favorites.begin();j!=rs.favorites.end();++j) {
		if (pred_in) {
			for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
				if (with_or_without_games || (*j)->has_emu(*k)) {
					next_select = (*j)->name_get();
					pred_in = false;
					break;
				}
			}
		}
		if ((*j)->name_get() == rs.include_favorites_get())
			pred_in = true;
	}
	
	if (next_select == "" && rs.favorites.begin() != rs.favorites.end())
		next_select = (*rs.favorites.begin())->name_get(); // lista "All Games"

	string f;

	if (next_select != "")
		f = next_select;

	rs.include_favorites_set(f);
}

int run_favorites_move(config_state& rs)
{
	string game = rs.current_game->name_get();

	choice_bag ch;

	for(pfavorites_container::const_iterator j=rs.favorites.begin();j!=rs.favorites.end();++j) {
		bool tag = false;
		if ((*j)->name_get() != "All Games") {
			if((*j)->has_game(game))
				tag = true;
			
			ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
		}
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.favorites_run(" Add/Remove Game Lists", THIRD_CHOICE_X, THIRD_CHOICE_Y, FAVORITES_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			for(pfavorites_container::const_iterator k=rs.favorites.begin();k!=rs.favorites.end();++k) {
				if((*k)->name_get() == j->desc_get()) {
					(*k)->game_move(game, j->bistate_get());
					break;
				}
			}
		}
	}

	return key;
}

// ------------------------------------------------------------------------
// Emu menu

#define EMU_CHOICE_DX 20*int_font_dx_get()

int run_emu(config_state& rs)
{
	choice_bag ch;

	for(pemulator_container::const_iterator j = rs.emu_active.begin();j!=rs.emu_active.end();++j) {
		bool tag = false;
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				tag = true;
				break;
			}
		}
		ch.insert(ch.end(), choice((*j)->user_name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(" Include Emulators", SECOND_CHOICE_X, SECOND_CHOICE_Y, EMU_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		emulator_container c;
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			if (j->bistate_get())
				c.insert(c.end(), j->desc_get());
		}
		rs.include_emu_set(c);
	}

	return key;
}

emulator* run_emu_select(config_state& rs)
{
	choice_bag ch;

	for(emulator_container::const_iterator j = rs.include_emu_get().begin();j!=rs.include_emu_get().end();++j) {
		ch.insert(ch.end(), choice(*j, 0));
	}

	string emu;

	if (ch.size() > 1) {
		choice_bag::iterator i = ch.begin();
		int key = ch.run(" Select Emulator", THIRD_CHOICE_X, THIRD_CHOICE_Y, EMU_CHOICE_DX, i);

		if (key != EVENT_ENTER)
			return 0;

		emu = i->desc_get();
	} else {
		emu = ch.begin()->desc_get();
	}

	if (rs.menu_systems->user_name_get() == emu)
		return rs.menu_systems;
	
	for(pemulator_container::const_iterator j = rs.emu_active.begin();j!=rs.emu_active.end();++j) {
		if ((*j)->user_name_get() == emu) {
			return *j;
		}
	}

	return 0;
}

void run_emu_next(config_state& rs)
{
	string next_select = "";
	bool pred_in = false;

	// Obtiene la lista de favoritos actual
	pfavorites_container::const_iterator i;
	for(i=rs.favorites.begin();i!=rs.favorites.end();++i)
		if((*i)->name_get() == rs.include_favorites_get())
			break;

	// Obtiene la coleccion de emuladores validos (con juegos para la lista de favoritos actual)
	pemulator_container emu_valid;
	for(pemulator_container::const_iterator j=rs.emu_active.begin();j!=rs.emu_active.end();++j)
		if((*i)->has_emu((*j)->user_name_get()))
			emu_valid.insert(emu_valid.end(), *j);

	// Recorre los emuladores validos para obtener el emulador siguiente
	for(pemulator_container::const_iterator j=emu_valid.begin();j!=emu_valid.end();++j) {
		if (pred_in)
			next_select = (*j)->user_name_get();
		pred_in = false;
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				pred_in = true;
				break;
			}
		}
	}
	if (next_select.length() == 0 && emu_valid.begin() != emu_valid.end())
		next_select = (*emu_valid.begin())->user_name_get();

	emulator_container c;

	if (next_select.length() != 0)
		c.insert(c.end(), next_select);

	rs.include_emu_set(c);	
}

void run_emu_pre(config_state& rs)
{
	string pre_select = "";
	string pre_emu = "";

	// Obtiene la lista de favoritos actual
	pfavorites_container::const_iterator i;
	for(i=rs.favorites.begin();i!=rs.favorites.end();++i)
		if((*i)->name_get() == rs.include_favorites_get())
			break;

	// Obtiene la coleccion de emuladores validos (con juegos para la lista de favoritos actual
	pemulator_container emu_valid;
	for(pemulator_container::const_iterator j=rs.emu_active.begin();j!=rs.emu_active.end();++j)
		if((*i)->has_emu((*j)->user_name_get()))
			emu_valid.insert(emu_valid.end(), *j);

	// Recorre los emuladores validos para obtener el emulador previo
	for(pemulator_container::const_iterator j=emu_valid.begin();j!=emu_valid.end();++j) {
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				pre_select = pre_emu;
				break;
			}
		}
		pre_emu = (*j)->user_name_get();
	}
	if (pre_select.length() == 0 && emu_valid.begin() != emu_valid.end())
		pre_select = pre_emu;

	emulator_container c;

	if (pre_select.length() != 0)
		c.insert(c.end(), pre_select);

	rs.include_emu_set(c);
}

// ------------------------------------------------------------------------
// Type menu

#define TYPE_CHOICE_DX 30*int_font_dx_get()

int run_type(config_state& rs)
{
	choice_bag ch;

	for(pcategory_container::const_iterator j = rs.type.begin();j!=rs.type.end();++j) {
		bool tag = rs.include_type_get().find((*j)->name_get()) != rs.include_type_get().end();
		ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(" Include Types", THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		category_container c;
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			if (j->bistate_get())
				c.insert(j->desc_get());
		}
		rs.include_type_set(c);
	}

	return key;
}

void run_type_next(config_state& rs)
{
	category* next_select = 0;
	bool all_select = true;

	bool pred_in = false;
	for(pcategory_container::const_iterator j=rs.type.begin();j!=rs.type.end();++j) {
		if (pred_in)
			next_select = *j;
		pred_in = false;
		for(category_container::const_iterator k = rs.include_type_get().begin();k!=rs.include_type_get().end();++k) {
			if ((*j)->name_get() == *k) {
				pred_in = true;
				break;
			}
		}
		if (!pred_in)
			all_select = false;
	}

	category_container c;
	if (!all_select && next_select == 0) {
		// insert all
		for(pcategory_container::const_iterator j=rs.type.begin();j!=rs.type.end();++j) {
			c.insert((*j)->name_get());
		}
	} else {
		if ((all_select || next_select == 0) && rs.type.begin() != rs.type.end())
			next_select = *rs.type.begin();
		if (next_select != 0) {
			// insert the next
			c.insert(next_select->name_get());
		}
	}
	rs.include_type_set(c);
}


// ------------------------------------------------------------------------
// Move menu

int run_type_move(config_state& rs)
{
	choice_bag ch;

	if (!rs.current_game)
		return EVENT_ENTER;

	for(pcategory_container::const_iterator j = rs.type.begin();j!=rs.type.end();++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.current_game->type_get()->name_get());
	if (i==ch.end())
		i = ch.begin();

	int key = ch.run(" Select Game Type", THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.current_game->user_type_set(rs.type.insert(i->desc_get()));
	}

	return key;
}

// ------------------------------------------------------------------------
// Clone menu

#define CLONE_CHOICE_X int_dx_get()/10
#define CLONE_CHOICE_Y int_dy_get()/5
#define CLONE_CHOICE_DX int_dx_get()*4/5

void run_clone(config_state& rs)
{
	choice_bag ch;
	choice_bag::iterator i;
	const game* base;

	rs.current_clone = 0;
	if (!rs.current_game)
		return;

	if (rs.current_game->software_get()) {
		base = &rs.current_game->root_get();
	} else {
		base = rs.current_game;
	}

	ostringstream s;
	s << base->description_get() << ", " << base->manufacturer_get() << ", " << base->year_get();
	ch.insert(ch.end(), choice(s.str(), (void*)base));

	for(pgame_container::const_iterator j = base->clone_bag_get().begin();j!=base->clone_bag_get().end();++j) {
		if (!(*j)->software_get()) {
			ostringstream s;

			s << (*j)->description_get() << ", " << (*j)->manufacturer_get() << ", " << (*j)->year_get();

			switch ((*j)->play_get()) {
			case play_imperfect : s << " [imperfect]"; break;
			case play_preliminary : s << " [preliminary]"; break;
			default: break;
			}

			ch.insert(ch.end(), choice(s.str(), (void*)&**j, (*j)->present_tree_get()));
		}
	}

	i = ch.begin();
	int key = ch.run(" Select game clone", CLONE_CHOICE_X, CLONE_CHOICE_Y, CLONE_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		rs.current_clone = (game*)i->ptr_get();
	}
}

// ------------------------------------------------------------------------
// Calib menu

#define CALIB_CHOICE_DX 30*int_font_dx_get()
#define CALIB_CHOICE_X (int_dx_get()-CALIB_CHOICE_DX)/2
#define CALIB_CHOICE_Y int_dy_get()/2

void run_calib(config_state& rs)
{
	const char* message = 0;
	bool at_least_one = false;

	joystickb_calib_start();

	while (1) {
		const char* ope = joystickb_calib_next();

		if (!ope) {
			if (!at_least_one)
				message = "Calibration not needed";
			break;
		}

		choice_bag ch;
		ch.insert(ch.end(), choice(ope, 0));

		choice_bag::iterator i = ch.begin();
		int key = ch.run(" Joystick Calibration", CALIB_CHOICE_X, CALIB_CHOICE_Y, CALIB_CHOICE_DX, i);

		if (key != EVENT_ENTER) {
			message = 0;
			break;
		}

		at_least_one = true;
	}

	if (message) {
		choice_bag ch;
		ch.insert(ch.end(), choice(message, 0));
		choice_bag::iterator i = ch.begin();
		ch.run(" Joystick Calibration", CALIB_CHOICE_X, CALIB_CHOICE_Y, CALIB_CHOICE_DX, i);
	}
}

// ------------------------------------------------------------------------
// Volume

#define VOLUME_CHOICE_DX 10*int_font_dx_get()

int run_volume(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Full", 0));
	ch.insert(ch.end(), choice("-2 db", -2));
	ch.insert(ch.end(), choice("-4 db", -4));
	ch.insert(ch.end(), choice("-6 db", -6));
	ch.insert(ch.end(), choice("-8 db", -8));
	ch.insert(ch.end(), choice("-10 db", -10));
	ch.insert(ch.end(), choice("-12 db", -12));
	ch.insert(ch.end(), choice("-14 db", -14));
	ch.insert(ch.end(), choice("-16 db", -16));
	ch.insert(ch.end(), choice("-18 db", -18));
	ch.insert(ch.end(), choice("-20 db", -20));
	ch.insert(ch.end(), choice("Silence", -40));

	choice_bag::iterator i = ch.find_by_value(play_attenuation_get());
	if (i == ch.end())
		i = ch.find_by_value(play_attenuation_get() - 1); // if the value is odd
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Volume", SECOND_CHOICE_X, SECOND_CHOICE_Y, VOLUME_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		play_attenuation_set(i->value_get());
		play_mute_set(false);
	}

	return key;
}

// ------------------------------------------------------------------------
// Difficulty

#define DIFFICULTY_CHOICE_DX 10*int_font_dx_get()

int run_difficulty(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Default", difficulty_none));
	ch.insert(ch.end(), choice("Easiest", difficulty_easiest));
	ch.insert(ch.end(), choice("Easy", difficulty_easy));
	ch.insert(ch.end(), choice("Normal", difficulty_medium));
	ch.insert(ch.end(), choice("Hard", difficulty_hard));
	ch.insert(ch.end(), choice("Hardest", difficulty_hardest));

	choice_bag::iterator i = ch.find_by_value(rs.difficulty_effective);
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Difficulty", SECOND_CHOICE_X, SECOND_CHOICE_Y, DIFFICULTY_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.difficulty_effective = static_cast<difficulty_t>(i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Sub Menu

#define MENU_CHOICE_DX 28*int_font_dx_get()

int run_suballmenu(config_state& rs)
{
	choice_bag ch;

	rs.sub_disable(); // force the use of the default config
	if (rs.type.size() > 1)
		ch.insert(ch.end(), choice(menu_name(rs, "Game Type...", EVENT_SETTYPE), 8, rs.current_game != 0));

	// MENU SYSTEMS
	ch.insert(ch.end(), choice((rs.menu_systems_activated ? "Desativar Menu de Sistemas" : "Ativar Menu de Sistemas"), 14));
	
	ch.insert(ch.end(), choice("Calibration...", 9));
	ch.insert(ch.end(), choice("Save all settings", 6));
	ch.insert(ch.end(), choice("Restore all settings", 20));
	ch.insert(ch.end(), choice(menu_name(rs, "Lock settings", EVENT_LOCK), 11));
	ch.insert(ch.end(), choice(rs.security_exit ? "Disable Security Exit" : "Enable Security Exit", 12));

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(" Settings", SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_sort(rs);
				break;
			case 1 :
				key = run_mode(rs);
				break;
			case 2 :
				key = run_preview(rs);
				break;
			case 3 :
				key = run_type(rs);
				break;
			case 4 :
				key = run_favorites(rs);
				break;
			case 6 :
				rs.restore_save();
				break;
			case 20 :
				rs.restore_load();
				break;
			case 8 :
				key = run_type_move(rs);
				break;
			case 9 :
				run_calib(rs);
				break;
			case 11 :
				rs.lock_effective = !rs.lock_effective;
				break;
			case 12 :
				rs.security_exit = !rs.security_exit;
				break;
			case 14 : // MENU SYSTEMS
				rs.menu_systems_activated = !rs.menu_systems_activated;
				// Desactivar MenuSystems: mostrado y emu seleccionado
				if(!rs.menu_systems_activated && rs.menu_systems->state_get() && rs.current_game != 0) {
					emulator_container c;
					for(pemulator_container::const_iterator i=rs.emu_active.begin();i!=rs.emu_active.end();) {
						if(emu_tolower((*i)->user_name_get()) == rs.current_game->name_without_emulator_get()) {
							c.insert(c.end(), (*i)->user_name_get());
							break;
						} else if(++i == rs.emu_active.end()) {
							// emu seleccionado no esta entre los activos -> carga el primero
							c.insert(c.end(), (*rs.emu_active.begin())->user_name_get());
						}
					}
					rs.include_emu_set(c);
				}
				// Activar MenuSystems: no mostrado
				if(rs.menu_systems_activated && !rs.menu_systems->state_get()) {
					emulator_container c;
					c.insert(c.end(), rs.menu_systems->user_name_get());
					rs.include_emu_set(c);
				}
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	rs.sub_enable(); // restore the use of the normal config

	return key;
}

int run_subthismenu(config_state& rs)
{
	bool is_favorites_allGames = (rs.include_favorites_get() == "All Games");
	bool enable_filtertype = (rs.favorites_filtertype || is_favorites_allGames);

	choice_bag ch;

	ch.insert(ch.end(), choice(menu_name(rs, "Sort...", EVENT_SORT), 0));
	ch.insert(ch.end(), choice(menu_name(rs, "Mode...", EVENT_MODE), 1));
	ch.insert(ch.end(), choice(menu_name(rs, "Preview...", EVENT_PREVIEW), 2));
	if (enable_filtertype)	{
		ch.insert(ch.end(), choice(menu_name(rs, "Types...", EVENT_TYPE), 3));
		ch.insert(ch.end(), choice(menu_name(rs, "Filters...", EVENT_ATTRIB), 11, rs.include_emu_get().size() != 0));
	}
	
	string title;
	if (rs.sub_has()) {
		title = " Listing Emulator";
	} else {
		title = " Listing Multiple";
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(title, SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			emulator* emu;
			switch (i->value_get()) {
			case 0 :
				key = run_sort(rs);
				break;
			case 1 :
				key = run_mode(rs);
				break;
			case 2 :
				key = run_preview(rs);
				break;
			case 3 :
				key = run_type(rs);
				break;
			case 5 :
				rs.sub_get().restore_save();
				break;
			case 6 :
				rs.sub_get().sort_unset();
				break;
			case 7 :
				rs.sub_get().mode_unset();
				break;
			case 8 :
				rs.sub_get().preview_unset();
				break;
			case 9 :
				rs.sub_get().include_type_unset();
				break;
			case 10 :
				break;
			case 11 :
				emu = run_emu_select(rs);
				if (emu) {
					key = emu->attrib_run(FOURTH_CHOICE_X, FOURTH_CHOICE_Y);
				} else {
					key = EVENT_ESC;
				}
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	return key;
}

int run_subfavoritesmenu(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice(menu_name(rs, "Load List...", EVENT_FAVORITES_NEXT), 0));
	ch.insert(ch.end(), choice(menu_name(rs, "Add/Rem Game...", EVENT_SETFAVORITES), 1, rs.current_game != 0));

	string text_filtertype = rs.favorites_filtertype ? "Disable Filters/Types" : "Enable Filters/Types"; 
	ch.insert(ch.end(), choice(text_filtertype, 2));
	
	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(" Favorites Lists", SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_favorites(rs);
				break;
			case 1 :
				key = run_favorites_move(rs);
				break;
			case 2 :
				rs.favorites_filtertype = !rs.favorites_filtertype;
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	return key;
}

int run_submenu(config_state& rs)
{
	choice_bag ch;

	if (!rs.console_mode) {
		ch.insert(ch.end(), choice("Listing...", 1));
		ch.insert(ch.end(), choice("Settings...", 0));
		ch.insert(ch.end(), choice("Favorites Lists...", 2));
		if (rs.emu.size() > 1)
			ch.insert(ch.end(), choice(menu_name(rs, "Emulators...", EVENT_EMU_NEXT), 7));
		ch.insert(ch.end(), choice("Volume...", 16));
		ch.insert(ch.end(), choice("Difficulty...", 17));
		ch.insert(ch.end(), choice(menu_name(rs, rs.script_menu, EVENT_COMMAND), 8));
		ch.insert(ch.end(), choice(menu_name(rs, "Clone...", EVENT_CLONE), 15));
		ch.insert(ch.end(), choice(menu_name(rs, "Help", EVENT_HELP), 10));
		ch.insert(ch.end(), choice("Statistics", 18));
	} else {
		ch.insert(ch.end(), choice(menu_name(rs, "Help", EVENT_HELP), 10));
		if (rs.emu.size() > 1)
			ch.insert(ch.end(), choice(menu_name(rs, "Emulators...", EVENT_EMU_NEXT), 7));
		ch.insert(ch.end(), choice("Volume...", 16));
		ch.insert(ch.end(), choice("Difficulty...", 17));
		if (rs.script_bag.size()!=0)
			ch.insert(ch.end(), choice(menu_name(rs, rs.script_menu, EVENT_COMMAND), 8));
	}

	if (rs.exit_mode == exit_normal || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice(menu_name(rs, "Exit", EVENT_ESC), 19));
	}
	if (rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice(menu_name(rs, "Poweroff", EVENT_OFF), 20));
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;
	int ret = EVENT_NONE;

	do {
		void* save = int_save();

		key = ch.run(" Menu", FIRST_CHOICE_X, FIRST_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_suballmenu(rs);
				break;
			case 1 :
				key = run_subthismenu(rs);
				break;
			case 2 :
				key = run_subfavoritesmenu(rs);
				break;
			case 7 :
				key = run_emu(rs);
				break;
			case 8 :
				key = run_command(rs);
				break;
			case 10 :
				run_help(rs);
				break;
			case 15 :
				done = true;
				ret = EVENT_CLONE;
				break;
			case 16 :
				key = run_volume(rs);
				break;
			case 17 :
				key = run_difficulty(rs);
				break;
			case 18 :
				run_stat(rs);
				break;
			case 19 :
				done = true;
				ret = EVENT_ESC;
				break;
			case 20 :
				done = true;
				ret = EVENT_OFF;
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_MENU || key == EVENT_ENTER)
			done = true;

	} while (!done);

	return ret;
}

// ------------------------------------------------------------------------
// Help menu

void run_help(config_state& rs)
{
	string img_help = "";
	emulator* emu_actual = 0;
	
	if (rs.menu_systems->state_get()) {
		emu_actual = rs.menu_systems;
	} else {
		for(pemulator_container::iterator k = rs.emu.begin();k!=rs.emu.end();k++) {
			if ((*k)->state_get() == 1) {
				emu_actual = *k;
				break;
			}
		}
	}

	if (rs.mode_get() == mode_custom) {
		string lay_path = emu_actual->custom_file_path_get();
		for(playout_container::iterator j = rs.lay_cont.begin();j!=rs.lay_cont.end();j++) {
			if((*j)->name_get() == lay_path) {
				img_help = (*j)->custom_help_path_get();
				break;
			}
		}
	} else {
		img_help = emu_actual->nocustom_help_path_get();
		if (img_help == "" || img_help == "default")
			img_help = rs.ui_help;
	}
	
	bool wait = true;

	int_clear(COLOR_HELP_NORMAL.background);

	if (img_help != "" && img_help != "none") {
		wait = int_clip(img_help, true);
	} else {
		if (rs.ui_back != "none") {
			unsigned x, y;
			int_image(rs.ui_back, x, y);
		}

		int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

		int y = rs.ui_top;
		int xt = rs.ui_left + 2*int_font_dx_get();
		int xd = rs.ui_left + (2+12)*int_font_dx_get();

		y += int_font_dy_get();
		int_put_alpha(xt, y, "In the game menu:", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Main menu", COLOR_HELP_NORMAL);
		} else {
			int_put_alpha(xt, y, event_name(EVENT_MENU), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Main menu", COLOR_HELP_NORMAL);
		}
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Run the current game/On menu accept the choice", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next preview mode/On menu change the option", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_MODE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next menu mode", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (rs.exit_mode == exit_normal || rs.exit_mode == exit_all) {
			if (!rs.console_mode) {
				int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
				int_put_alpha(xd, y, "Exit", COLOR_HELP_NORMAL);
				y += int_font_dy_get();
			}
		}
		if (rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all) {
			int_put_alpha(xt, y, event_name(EVENT_OFF), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Shutdown", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.favorites.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_FAVORITES_NEXT), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Next game list", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.type.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_TYPE), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Next game type", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (!rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_ATTRIB), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Include/Exclude games by attribute", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, event_name(EVENT_SORT), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Select the game sort method", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		int_put_alpha(xt, y, event_name(EVENT_EMU_PRE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Previous emulator", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_EMU_NEXT), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next emulator", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (!rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_COMMAND), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Commands", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.favorites.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_SETFAVORITES), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Change the current game lists", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.type.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_SETTYPE), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Change the current game type", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		int_put_alpha(xt, y, event_name(EVENT_CLONE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Run a clone", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ROTATE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Rotate the screen", COLOR_HELP_NORMAL);
		y += int_font_dy_get();

		y += int_font_dy_get();
		int_put_alpha(xt, y, "In the submenus:", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Accept", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_DEL), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Unselect all", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_INS), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Select all", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Toggle (+ include, - exclude, * required)", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Cancel", COLOR_HELP_NORMAL);
		y += int_font_dy_get();

		if (rs.console_mode) {
			y += int_font_dy_get();
			int_put_alpha(xt, y, "In the emulators:", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "F1", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Help", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "5 6 7 8", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Insert Coins", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "1 2 3 4", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Start Player 1, 2, 3, 4", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "CTRL ALT SPACE", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Buttons Player 1", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "ARROWS", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Move Player 1", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "ESC", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Return to the menu", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
	}

	if (wait)
		int_event_get();
}

// ------------------------------------------------------------------------
// Stat

#define STAT_MAX 10

void stat_insert(const game* (&map)[STAT_MAX], unsigned (&val)[STAT_MAX], const game* g, unsigned v)
{
	for(unsigned i=0;i<STAT_MAX;++i) {
		if (!map[i] || val[i] < v) {
			for(unsigned j=STAT_MAX-1;j>i;--j) {
				map[j] = map[j-1];
				val[j] = val[j-1];
			}
			map[i] = g;
			val[i] = v;
			return;
		}
	}
}

string stat_time(unsigned v)
{
	ostringstream os;
	os << (v/3600) << ":" << setw(2) << setfill('0') << ((v/60)%60);
	return os.str();
}

string stat_int(unsigned v)
{
	ostringstream os;
	os << v;
	return os.str();
}

string stat_perc(unsigned v, unsigned t)
{
	ostringstream os;
	if (t)
		os << stat_int(v * 100 / t) << "%";
	else
		os << stat_int(0) << "%";
	return os.str();
}

void run_stat(config_state& rs)
{
	unsigned total_count = 0;
	unsigned total_session = 0;
	unsigned total_time = 0;
	unsigned select_count = 0;
	unsigned select_session = 0;
	unsigned select_time = 0;
	const game* most_session_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_time_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_timepersession_map[STAT_MAX] = { 0, 0, 0 };
	unsigned most_session_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_time_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_timepersession_val[STAT_MAX] = { 0, 0, 0 };
	unsigned n;

	int y = rs.ui_top;
	int xn = rs.ui_left + 2*int_font_dx_get();
	int xs = rs.ui_left + (2+1*8)*int_font_dx_get();
	int xt = rs.ui_left + (2+2*8)*int_font_dx_get();
	int xp = rs.ui_left + (2+3*8)*int_font_dx_get();
	int xe = rs.ui_left + (2+4*8)*int_font_dx_get();

	n = ((int_dy_get() - rs.ui_top - rs.ui_bottom) / int_font_dy_get() - 12) / 3;
	if (n > STAT_MAX)
		n = STAT_MAX;

	int_clear(COLOR_HELP_NORMAL.background);

	if (rs.ui_back != "none") {
		unsigned x, y;
		int_image(rs.ui_back, x, y);
	}

	int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

	// select and sort
	for(game_set::const_iterator i=rs.gar.begin();i!=rs.gar.end();++i) {
		unsigned session;
		unsigned time;
		unsigned timepersession;

		if (i->emulator_get()->tree_get())
			session = i->session_tree_get();
		else
			session = i->session_get();
		if (i->emulator_get()->tree_get())
			time = i->time_tree_get();
		else
			time = i->time_get();
		if (session)
			timepersession = time / session;
		else
			timepersession = 0;

		total_count += 1;
		total_session += session;
		total_time += time;

		// emulator
		if (!i->emulator_get()->state_get())
			continue;

		// type
		if (!i->type_derived_get()->state_get())
			continue;

		// filter
		if (!i->emulator_get()->filter(*i))
			continue;

		select_count += 1;
		select_session += session;
		select_time += time;

		stat_insert(most_session_map, most_session_val, &*i, session);
		stat_insert(most_time_map, most_time_val, &*i, time);
		stat_insert(most_timepersession_map, most_timepersession_val, &*i, timepersession);
	}

	y += int_font_dy_get();
	int_put_right_alpha(xs, y, xt-xs, "Listed", COLOR_HELP_TAG);
	int_put_right_alpha(xt, y, xp-xt, "Total", COLOR_HELP_TAG);
	int_put_right_alpha(xp, y, xe-xp, "Perc", COLOR_HELP_TAG);

	{

		y += int_font_dy_get();
		int_put_alpha(xn, y, "Games", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_int(select_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_int(total_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_count, total_count), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Play", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_int(select_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_int(total_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_session, total_session), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Time", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_time(select_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_time(total_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_time, total_time), COLOR_HELP_NORMAL);
	}

	xs = (1+7)*int_font_dx_get();
	xe = (1+7+5)*int_font_dx_get();
	xt = (1+7+5+2)*int_font_dx_get();

	if (n>0 && most_time_map[0]) {
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most time", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_time_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_time(most_time_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(xs, y, xe-xs, stat_perc(most_time_val[i], select_time), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_time_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n>0 && most_session_map[0]) {
		ostringstream os;
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most play", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_session_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_int(most_session_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(xs, y, xe-xs, stat_perc(most_session_val[i], select_session), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_session_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n>0 && most_timepersession_map[0]) {
		ostringstream os;
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most time per play", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_timepersession_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_time(most_timepersession_val[i]), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_timepersession_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	int_event_get();
}

// ------------------------------------------------------------------------
// Security Exit

int run_exit(config_state& rs)
{
	int dx = int_font_dx_get() * 24;
	int x = (int_dx_get() - dx) / 2;
	int y = int_dy_get() / 3;
	
	choice_bag ch;
	ostringstream os1;
	ostringstream os2;
	os1 <<setw(26) <<"continuar";
	os2 <<setw(29) <<"sair";
	
	ch.insert(ch.end(), choice(os1.str(), 0));
	ch.insert(ch.end(), choice(os2.str(), 1));

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run("     Deseja sair do sistema?", x, y, dx, i);

	if (key == EVENT_ENTER) {
		return i->value_get();
	}

	return 0;
}
