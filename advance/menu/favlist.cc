/*
 * This file is part of the AdvMenuPLUS project.
 *
 * Copyright (C) 2015 Daesdae <daesdae@hotmail.com>
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

#include "favlist.h"

using namespace std;

favorite::favorite(const string& Aname) : name(Aname)
{
	has_changed = false;
}

favorite::~favorite()
{
}

string name_emu_get(const string& name_game)
{
	int i = name_game.find('/');
	if (i == string::npos)
		return name_game;
	else
		return name_game.substr(0, i);
}

string name_without_emu_get(const string& name_game)
{
	int i = name_game.rfind('/');
	if (i == string::npos)
		return name_game;
	else
		return name_game.substr(i + 1);
}

void favorite::game_insert(const string& name_game)
{
	gar_fav.insert(gar_fav.end(), name_game);
}

void favorite::game_remove(const string& name_game)
{
	gar_fav.remove(name_game);
}

void favorite::game_move(const std::string& name_game, bool bistate)
{
	bool exists_game = has_game(name_game);
	
	if (bistate && !exists_game) {
		game_insert(name_game);
		has_changed = true;
	} else if (!bistate && exists_game) {
		game_remove(name_game);
		has_changed = true;
	}
}

bool favorite::has_emu(const string& emu)
{
	if(this->name_get() == "All Games")
	   return true;
	   
	for(favorites_container::const_iterator i=gar_fav.begin();i!=gar_fav.end();++i) {
		string emu_game = name_emu_get((*i));
		if(emu_game == emu)
		   return true;
	}
	
	return false;
}

bool favorite::has_game(const string& name_game)
{
	if(this->name_get() == "All Games")
	   return true;
	   
	for(favorites_container::const_iterator i=gar_fav.begin();i!=gar_fav.end();++i) {
		if((*i) == name_game)
		   return true;
	}
	
	return false;
}

bool favorite::import() const
{	
	string emu = "";
	string file = "favlist/" + name_get() + ".fav";
	string path = file_config_file_home(file.c_str());

	if(file_exists(path)) {
	
		int j = 0;

		string ss = file_read(file);

		while (j < ss.length()) {
			string s = token_get(ss, j, "\r\n");
			token_skip(ss, j, "\r\n");

			int i = 0;

			if (i<s.length() && s[i]=='[') {
				token_skip(s, i, "[");
				emu = token_get(s, i, "]");
			} else if (i<s.length() && isalnum(s[i])) {
				string rom = s;
				if (rom.length()) {
					string name = emu + "/" + rom;
					gar_fav.insert(gar_fav.end(), name);
				}
			}
		}
	}
	
	return true;
}

bool favorite::save() const
{
	if(!has_changed)
		return true;
	
	string file = "favlist/" + name_get() + ".fav";
	string path = file_config_file_home(file.c_str());

	string s = "";
	string emu = "";
		
	gar_fav.sort(); // ordena la lista de juegos
	for(favorites_container::const_iterator j=gar_fav.begin();j!=gar_fav.end();++j) {
		string emu_tmp = name_emu_get(*j);
		string rom = name_without_emu_get(*j);
		if(emu != emu_tmp) {
			emu = emu_tmp;
			s += "\r\n[" + emu + "]\r\n";
			s += rom + "\r\n";
		} else {
			s += rom + "\r\n";
		}
	}
	s += "\r\n";

	if(!file_write(path, s))
		return false;

	has_changed = false;

	return true;
}
