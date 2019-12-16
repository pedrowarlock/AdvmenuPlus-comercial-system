/*
 * This file is part of the AdvMenuPlus project. Copyright (C) 2016 daesdae
 *
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

#include "textview.h"
#include "text.h"
#include "common.h"

using namespace std;

textview::textview() {
	
	int char_width = int_put_width("a");
	line_cut = 80 * char_width;

	int margen = char_width; // margen pantalla - ventana
	border = char_width / 2; // borde interno de la ventana
	indent = char_width; // tabulado de las lineas

	int scroll_dx = border/2 + (2*indent)/3;
	
	int dx = line_cut + indent + 2 * border + border/2; // ancho de la ventana con scroll

	// si la caja con scroll no cabe recalcula line_cut
	if (dx + 2 * margen > int_dx_get()) {
		line_cut = int_dx_get() - 2*margen - 2*border - indent - scroll_dx;
	}
}

textview::~textview() {
	clear();
}

void scroll_draw(int x, int y, int dx, int pos, int delta, int max)
{
	// pos -> posicion del primer item mostrado relativa al total de items. (pos_base)
	// delta -> numero de items que se muestran (pos_rel_max)
	// max -> numero total de items (pos_max)
	
	int dy = delta * int_font_dy_get();
	
	if (max <= 1)
		return;

	if (pos >= max)
		pos = max - 1;
	if (pos + delta > max)
		delta = max - pos;

	int y0 = pos * (dy-dx-1) / (max-1);
	int y1 = dx + (pos+delta-1) * (dy-dx-1) / (max-1);

	// pinta el fondo
	int_clear_alpha(x, y, dx, dy, COLOR_CHOICE_HIDDEN_SELECT.foreground);
	// pinta el scrollbox o thumb o cursor
	int_clear_alpha(x+1, y+y0+1, dx-2, y1-y0+1-2, COLOR_CHOICE_HIDDEN_SELECT.background);
}

void textview::draw(const string& title, int x, int y, int dx, int pos_base, int pos_rel, int rows)
{
	// pinta el titulo
	int_put_filled(x, y, dx, title, COLOR_CHOICE_TITLE);

	y += int_font_dy_get();

	// subraya el titulo
	int_clear(x, y - 1, int_font_dx_get(title), 1, COLOR_CHOICE_TITLE.foreground);
	
	if (rows < size()) {
		scroll_draw(x + indent + line_cut + border/2, y, (2*indent)/3, pos_base, rows, size());
	}
	
	for(unsigned j=0;j<rows;++j) {

		string desc = "";
		if (pos_base + j  < size()) {
			textview_container::iterator i = begin() + pos_base + j;
			desc = *i;
		}
		
		int_color color = COLOR_CHOICE_NORMAL;
		int_color colorf = COLOR_CHOICE_TITLE;
		/*// color seleccionado
		if (j==pos_rel) {
			color = COLOR_CHOICE_SELECT;
			colorf = COLOR_CHOICE_SELECT;
		}*/

		bool in = false;

		// pinta la linea
		int_put_special(in, x + indent, y, line_cut, desc, colorf, color, color);
		
		y += int_font_dy_get();
	}
}

int textview::run(const string& title)
{
	if (size() == 0)
		return EVENT_ESC;
	
	bool has_scroll = false;
	int key = EVENT_ESC;
	int done = 0;
	
	// ----------------- POS --------------------------------------------------------
	int pos_base = 0; // posicion de la primera linea mostrada en relacion con el total de lineas.
	int pos_rel = 0; // posicion de la linea seleccionada en relacion con las lineas mostradas.
	int pos_max = size(); //numero total de lineas
	int pos_rel_max = (int_dy_get() - 2 * int_font_dy_get()) / int_font_dy_get(); //numero de lineas a mostrar

	if (pos_rel_max > pos_max)
		pos_rel_max = pos_max;
	else if (pos_rel_max < pos_max)
		has_scroll = true;

	// ----------------- MEDIDAS  --------------------------------------
	// ancho linea (dx) ajustado al maximo ancho de las lineas
	int line_width = int_put_width(title);
	for(textview_container::iterator i = begin(); i < end(); ++i) {
		const string& desc = *i;
		int line_width_aux = int_put_width(desc);
		if (line_width_aux > line_width)
			line_width = line_width_aux;
	}
	line_cut = line_width;

	int dx = line_cut + indent + 2 * border;
	if (has_scroll)
		dx += border/2 + (2*indent)/3;
	int x = (int_dx_get() - dx) / 2;
	int dy = (pos_rel_max + 1) * int_font_dy_get() + 2 * border;
	int y = (int_dy_get() - dy) / 2;
	
	// OJO A ESTA COMPROBACION
	if (x < 0) x = 0;//(int_dx_get() - dx - border * 2) / 2;
	if (y < 0) y = 0;//(int_dy_get() - (size() + 1) * int_font_dy_get() - border * 2) / 2;

	// PINTA LA VENTANA
	int_box(x, y, dx, dy, 1, COLOR_CHOICE_NORMAL.foreground);
	int_clear(x + 1, y + 1, dx - 2, dy - 2, COLOR_CHOICE_NORMAL.background);

	while (!done) {
		// pinta el textview
		draw(title, x + border, y + border, dx - 2 * border, pos_base, pos_rel, pos_rel_max);

		// Controlador general de eventos
		key = int_event_get();
		// Controlador de eventos de desplazamiento
		key = textview_key(key, pos_base, pos_rel, pos_rel_max, pos_max);
		// Controlador de eventos de cierre
		switch (key) {
			case EVENT_MINFO :
			case EVENT_ENTER :
			case EVENT_ESC :
			case EVENT_MENU :
				done = 1;
				break;
		}
	}

	return key;
}

int textview_key(int key, int& pos_base, int& pos_rel, int pos_rel_max, int pos_max)
{
	switch (key) {
		case EVENT_HOME : 
			pos_rel = 0;
			pos_base = 0;
			break;
		case EVENT_END : 
			pos_rel = pos_rel_max - 1;
			pos_base = pos_max - pos_rel -1;
			break;
		case EVENT_LEFT : 
		case EVENT_PGUP : 
			pos_rel = 0;
			if (pos_base - pos_rel_max - 1 > 0)
				pos_base -= pos_rel_max;
			else
				pos_base = 0;
			break;
		case EVENT_RIGHT : 
		case EVENT_PGDN : 
			pos_rel = pos_rel_max -1;
			if (pos_base + pos_rel + pos_rel_max + 1 < pos_max)
				pos_base += pos_rel_max;
			else
				pos_base = pos_max - pos_rel_max;
			break;
		case EVENT_UP : 
			pos_rel = 0;
			if (pos_base > 0)
				pos_base--;
			break;
		case EVENT_DOWN : 
			pos_rel = pos_rel_max - 1;
			if (pos_base + pos_rel + 1 < pos_max)
				pos_base++;
			break;
		default:
			return key;
	}

	return EVENT_NONE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Insercion del texto

string textview::fileread(const string& file)
{
	struct stat st;

	const char* path = file_config_file_home(file.c_str());

	if (stat(path, &st)!=0) {
		insert_line("Error opening the file: " + file);
		return string("");
	}

	FILE* f = fopen(path, "rb");
	if (!f) {
		insert_line("Error opening the file: " + file);
		return string("");
	}

	char* ssc = (char*)operator new(st.st_size + 1);

	if (fread(ssc, st.st_size, 1, f)!=1) {
		operator delete(ssc);
		fclose(f);
		insert_line("Error reading the file: " + file);
		return string("");
	}

	fclose(f);
	ssc[st.st_size] = 0;

	string ss = ssc;
	operator delete(ssc);

	return ss;
}

// Devuelve la posicion del ultimo caracter que entra en la linea.
int pos_cut_get(const string& s, const int width) {
	
	string ss = s;
	int pos = 0;

	while (int_put_width(ss) > width) {
				
		pos = ss.rfind(" ");

		// Si la linea no tiene espacio
		if (pos < 0) {
			int size = 0;
			for(pos=0;pos<ss.length();++pos) {
				size += int_put_width(ss[pos]);
				if (size > width)
					return pos;
			}
			return pos;
		}

		ss = ss.substr(0, pos);
	}

	return pos - 1;
}

// Inserta una linea en el textview
void textview::insert_line(const string& line) {
	
	string s = line;

	// Limpia el final de linea
	size_t found = s.find_last_not_of(" \t\f\v\n\r");
	if (found != string::npos)
		s.erase(found + 1);
	else
		s.clear();

	// Sustituye los '\t' del inicio de la linea por 4 espacios
	int i = 0;
	while (i < s.length() && s[i] == '\t') {
		s.replace(i, 1, "    ");
		i += 4;
	}

	// Corta las lineas segun el ancho maximo (line_cut)
	while (int_put_width(s) > line_cut) {

		int pos_cut = pos_cut_get(s, line_cut);
		
		string line_aux = s.substr(0, pos_cut+1);
		push_back(line_aux);
		
		if (s[pos_cut+1] == ' ')
			pos_cut++;

		s = s.erase(0, pos_cut+1);
	}

	push_back(s);
}

// Inserta el texto en el textview
void textview::insert_text(const string& text) {

	int i = 0;

	while (i < text.length()) {
		string line = "";

		// Obtiene las lineas del texto
		int end_line = text.find("\n", i);
		line = string(text, i, end_line - i);
		i += line.length() + 1;

		insert_line(line);
	}
}

// Inserta todo el texto del archivo en el textview
void textview::insert_file(const string& file)
{
	string text = fileread(file);// file_read(file);

	if (text.empty())
		text = "-------- No info --------";
	
	insert_text(text);
}

// Devuelve el texto segun el tipo (mamehistory o mameinfo)
string textview::infohistory_import(const string& file, const string& hash, const string& rom)
{
	string text = "";
	
	if (rom.empty())
		return text;

	string ss = fileread(file);
	if (ss.empty())
		return text;

	//busca '$' en el texto del archivo
	int pos_info = ss.find('$');
	while (pos_info != string::npos) {
		//obtiene la linea "$hash_info=rom_info"
		string line = token_get(ss, pos_info, "\r\n");
		token_skip(ss, pos_info, "\r\n");

		//busca el '=' en la linea
		int pos_eq = line.find('=');
		if (pos_eq != string::npos) {
			//obtiene la parte hash_info y la parte rom_info de la linea
			string hash_info = string(line, 1, pos_eq - 1);
			string rom_info = string(line, pos_eq + 1, line.length() - pos_eq);

			if (hash_info != "" && rom_info != "") {
				//comprueba si el hash del juego esta en la parte hash_info
				bool found_hash = false;
				unsigned i = 0;
				while (i<hash_info.length() && !found_hash) {
					int end = hash_info.find(',', i);
					if (end == string::npos)
						end = hash_info.length();
					string hash_temp = string(hash_info, i, end - i);
					if (hash_temp == hash)
						found_hash = true;
					i = end + 1;
				}

				if (found_hash) {
					//comprueba si la rom del juego esta en la parte rom_info
					bool found_rom = false;
					unsigned j = 0;
					while (j<rom_info.length() && !found_rom) {
						int end = rom_info.find(',', j);
						if (end == string::npos)
							end = rom_info.length();
						string rom_temp = string(rom_info, j, end - j);
						if (rom_temp == rom)
							found_rom = true;
						j = end + 1;
					}

					if (found_rom) {
						//obtiene el texto
						int pos_end = ss.find("$end", pos_info);
						if (pos_end != string::npos) {
							text = string(ss, pos_info, pos_end - pos_info);
							break;
						}
					}
				}
			}
		}
		
		//busca el siguiente '$'
		pos_info = ss.find('$', pos_info);
	}

	// elimina las lineas "bio" o "mame" y a partir de "staff" o "contribute"	
	string bio = "$bio\r\n";
	string mame = "$mame\r\n";

	int pos_ini = 0;
	int pos_bio = text.find(bio);
	int pos_mame = text.find(mame);
	
	if (pos_bio != string::npos)
		pos_ini = pos_bio + bio.length();
	else if (pos_mame != string::npos)
		pos_ini = pos_mame + mame.length();

	int pos_fin = text.length() - 1;
	int pos_staff = text.find("- STAFF -");
	int pos_contribute = text.find("- CONTRIBUTE -");
	
	if (pos_staff != string::npos)
		pos_fin = pos_staff;
	else if (pos_contribute != string::npos)
		pos_fin = pos_contribute;
	
	text = text.substr(pos_ini, pos_fin - pos_ini);
	
	if (text.empty())
		return "-------- No info --------";
	
	return text;
}

// Inserta el texto segun el tipo de archivo en el textview
void textview::insert_file(const string& file, const string& type, const string& hash, const string& rom)
{
	string text = "";
	
	if (type == "mamehistory" || type == "mameinfo")
		text = infohistory_import(file, hash, rom);
	else
		text = "ERROR: Type '" + type + "' es incorrecto.";

	insert_text(text);
}
