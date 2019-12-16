/*
 * This file is part of the AdvMenuPLUS project.
 * 
 * Copyright (C) daesdae 2016 <daesdae@hotmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "portable.h"

#include "emulator.h"
#include "game.h"

#include <sstream>

using namespace std;

static const string without_emu(const string& emu, const string& name)
{
	int i = name.find(emu + "/");
	if (i == 0)
		return name.substr(emu.length() + 1);
	else
		return name;
}

static const string xmlnormalize(const string& s)
{
	string xmltext = "";
	int i = 0;
	int l = s.length();
		
	while (i<l) {
		if (s[i] == '&')
			xmltext += "&amp;";
		else if (s[i] == '<')
			xmltext += "&lt;";
		else if (s[i] == '>')
			xmltext += "&gt;";
		else
			xmltext += s[i];
		++i;
	}
	
	return xmltext;
}

static const string cleanxml_get(const string& emu_name, const game_set& gar)
{
	string s = "<?xml version=\"1.0\"?>";
	s += "\r\n<mame>";

	for(game_set::const_iterator i=gar.begin();i!=gar.end();++i) {
				
		if (i->emulator_get()->user_name_get() == emu_name)
		{
			s += "\r\n\t<game";

			if (i->flag_get(emulator::flag_derived_resource))
				s += " runnable=\"no\"";
			
			s += " name=\"" + without_emu(emu_name, i->name_get()) + "\"";

			if (i->flag_get(emulator::flag_derived_mechanical))
				s += " ismechanical=\"yes\"";

			if (i->flag_get(emulator::flag_derived_romarcade))
				s += " isarcade=\"yes\"";
			
			if (i->cloneof_get() != "")
				s += " cloneof=\"" + without_emu(emu_name, i->cloneof_get()) + "\"";
			
			if (i->romof_get() != "")
				s += " romof=\"" + without_emu(emu_name, i->romof_get()) + "\"";

			s += ">";

			if (i->description_get() != "")
				s += "\r\n\t\t<description>" + xmlnormalize(i->description_get()) + "</description>";

			if (i->year_get() != "")
				s += "\r\n\t\t<year>" + i->year_get() + "</year>";

			if (i->manufacturer_get() != "")
				s += "\r\n\t\t<manufacturer>" + xmlnormalize(i->manufacturer_get()) + "</manufacturer>";

			if (i->size_get() != 0)
			{
				ostringstream os;
				os << "\r\n\t\t<rom size=\"" << i->size_get() << "\" />";
				s += os.str();
			}

			if (i->flag_get(emulator::flag_derived_chd))
				s += "\r\n\t\t<disk />";

			if (i->flag_get(emulator::flag_derived_vector) || i->flag_get(emulator::flag_derived_vertical) ||
			    i->sizex_get() != 0 || i->sizey_get() != 0 || i->aspectx_get() != 0 || i->aspecty_get() != 0)
			{
				ostringstream os;
				os << "\r\n\t\t<video";
				if (i->flag_get(emulator::flag_derived_vector))
				    os << " screen=\"vector\"";
				if (i->flag_get(emulator::flag_derived_vertical))
					os << " orientation=\"vertical\"";
				if (i->sizex_get() != 0)
					os << " width=\"" << i->sizex_get() << "\"";
				if (i->sizey_get() != 0)
					os << " height=\"" << i->sizey_get() << "\"";
				if (i->aspectx_get() != 0)
					os << " aspectx=\"" << i->aspectx_get() << "\"";
				if (i->aspecty_get() != 0)
					os << " aspecty=\"" << i->aspecty_get() << "\"";
				os << " />";

				s += os.str();
			}

			if (i->flag_get(emulator::flag_derived_mahjong)) {
				s += "\r\n\t\t<input>";
				s += "\r\n\t\t\t<control type=\"mahjong\" />";
				s += "\r\n\t\t</input>";
			}

			if (i->flag_get(emulator::flag_derived_softwarelist))
				s += "\r\n\t\t<softwarelist />";

			if (i->play_get() == play_preliminary)
				s += "\r\n\t\t<driver status=\"preliminary\" />";
			if (i->play_get() == play_imperfect)
				s += "\r\n\t\t<driver status=\"imperfect\" />";

			s += "\r\n\t</game>";
		}
	}

	s += "\r\n</mame>";

	return s;
}

bool mame_mame::clean_xml(const string& xml_dir, const game_set& gar)
{
	string emu_name = user_name_get();

	string xml_text = cleanxml_get(emu_name, gar);

	string xml_file = path_abs(path_import(file_config_file_home((slash_add(xml_dir) + emu_name + ".xml").c_str())), dir_cwd());

	if (!file_write(xml_file, xml_text))
		return false;

	return true;
}
