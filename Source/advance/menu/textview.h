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

#ifndef __TEXTVIEW_H
#define __TEXTVIEW_H

#include <string>
#include <vector>

typedef std::vector<std::string> textview_container;

class textview : public textview_container {
	int line_cut;
	int border;
	int indent;
	std::string fileread(const std::string& file);
	std::string infohistory_import(const std::string& file, const std::string& hash, const std::string& rom);
	void draw(const std::string& title, int x, int y, int dx, int base, int rel, int rows);
public:
	textview();
	~textview();
	
	void insert_line(const std::string& line);
	void insert_text(const std::string& text);
	void insert_file(const std::string& file);
	void insert_file(const std::string& file, const std::string& type, const std::string& hash, const std::string& rom);
	
	int run(const std::string& title);
};

int textview_key(int key, int& pos_base, int& pos_rel, int pos_rel_max, int pos_max);

#endif
