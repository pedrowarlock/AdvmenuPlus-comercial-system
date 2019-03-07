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

#ifndef __FAVLIST_H
#define __FAVLIST_H

#include "common.h"

#include <list>
#include <string>

typedef std::list<std::string> favorites_container;

class favorite {
	std::string name;
	mutable bool has_changed;

public :
	favorite();
	favorite(const std::string& Aname);
	~favorite();

	mutable favorites_container gar_fav;
	
	void name_set(const std::string& A);
	const std::string& name_get() const { return name; }

	void game_insert(const std::string& A);
	void game_remove(const std::string& A);
	void game_move(const std::string& A, bool bistate);

	bool has_emu(const std::string& emu);
	bool has_game(const std::string& game);

	bool save() const;
	bool import() const;
};

typedef std::list<favorite*> pfavorites_container;

#endif // __FAVLIST_H
