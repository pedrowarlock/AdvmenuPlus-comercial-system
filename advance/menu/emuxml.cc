/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2008 Andrea Mazzoleni
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

#include "emulator.h"
#include "game.h"

#include <expat.h>

#include "advance.h"

#include <string>
#include <iostream>

using namespace std;

/****************************************************************************/
/* Parser */

/**
 * Max depth checked.
 */
#define DEPTH_MAX 5

enum token_t {
	token_open,
	token_close,
	token_data
};

struct state_t;

typedef void (process_t)(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes);

/**
 * State for every depth level.
 */
struct level_t {
	const char* tag; /**< Tag name. */
	char* data; /**< Accumulative data. */
	unsigned len; /**< Length of the data. */
	process_t* process; /**< Processing function. */
};

/**
 * Global parsing state.
 */
struct state_t {
	XML_Parser parser; /**< Parser. */
	int depth; /**< Current depth. */
	struct level_t level[DEPTH_MAX]; /**< Level state. */
	int error; /**< Error flag. */
	emulator* e; /**< Current parent emulator. */
	game* g; /**< Curren game in the loading process. */
	game_set* a; /**< Game archive. */
	machinedevice* m; /**< Machine device. */
	unsigned rom_size; /**< Size of the current rom. */
	bool rom_merge; /**< Merge of the current rom. */

	string sn; // nombre del softwarelist (hash) que se esta leyendo
	softwarelist* s; // system(name, status, filter), software(sn, sharedfeatname, sharedfeatvalue)
};

/****************************************************************************/
/* Processing */

static void process_error(struct state_t* state, const char* tag, const char* msg)
{
	int line = XML_GetCurrentLineNumber(state->parser);
	if (tag)
		target_err("Error reading at line %d for element/attribute `%s' for %s.\n", line, tag, msg);
	else
		target_err("Error reading at line %d for %s.\n", line, msg);
	state->error = 1;
}

static void process_game(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		state->g = new game;
		state->g->emulator_set(state->e);
	} else if (t == token_close) {
		state->a->insert(*state->g);
		delete state->g;
		state->g = 0;
	}
}

static void process_softwarelistnameloaded(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		state->sn = string(s, len);
	}
}

static void process_software(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_open) {
		state->g = new game;
		state->g->emulator_set(state->e);
		state->g->software_set(true);
	} else if (t == token_close) {
		state->a->insert(*state->g);
		delete state->g;
		state->g = 0;
	}
}

static void process_softwarename(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->name_set(state->e->user_name_get() + "/" + state->sn + "/" + string(s, len));
	}
}

static void process_softwarecloneof(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->cloneof_set(state->e->user_name_get() + "/" + state->sn + "/" + string(s, len));
	}
}

static void process_game_generic(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		state->g = new game;
		state->g->emulator_set(state->e);
	} else if (t == token_close) {
		if (state->g->name_without_emulator_get() != "") {
			game_set::const_iterator i = state->a->find(game(state->g->name_get()));
			if (i!=state->a->end()) {
				if (state->g->description_get().length())
					i->auto_description_set(state->g->description_get());
				if (state->g->manufacturer_get().length())
					(const_cast<game&>(*i)).manufacturer_set(state->g->manufacturer_get());
				if (state->g->year_get().length())
					(const_cast<game&>(*i)).year_set(state->g->year_get());
				if (state->g->cloneof_get().length())
					(const_cast<game&>(*i)).cloneof_set(state->g->cloneof_get());
			} else {
				state->g->size_set(1); //para indentificar los juegos como missing deben tener algun tamaño
				state->a->insert(*state->g);
			}
		}
		delete state->g;
		state->g = 0;
	}
}

static void process_runnable(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		string v = string(s, len);
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (v == "no")
			state->g->flag_set(true, emulator::flag_derived_resource);
	}
}

static void process_isbios(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		string v = string(s, len);
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (v == "yes")
			state->g->flag_set(true, emulator::flag_derived_resource);
	}
}

static void process_isdevice(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		string v = string(s, len);
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (v == "yes")
			state->g->flag_set(true, emulator::flag_derived_resource);
	}
}

static void process_isarcade(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		string v = string(s, len);
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (v == "yes")
			state->g->flag_set(true, emulator::flag_derived_romarcade);
	}
}

static void process_name(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->name_set(state->e->user_name_get() + "/" + emu_tolower(string(s, len))); //?????
	}
}

static void process_description(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->auto_description_set(string(s, len));

		string v = string(s, len);
				
		if(v.find("Casino")!= string::npos || v.find("casino")!= string::npos || v.find("Poker")!= string::npos 
		|| v.find("poker")!= string::npos || v.find("Card")!= string::npos || v.find("Fruit Bonus")!= string::npos
		|| v.find("Fruit Cocktail")!= string::npos || v.find("Brasil 8")!= string::npos || v.find("Bra$il")!= string::npos
		|| v.find("Brasil 9")!= string::npos || v.find("Ruleta")!= string::npos || v.find("Jockey")!= string::npos
		|| v.find("Lucky 2")!= string::npos || v.find("Lucky 7")!= string::npos || v.find("Lucky Roulette")!= string::npos
		|| v.find("Cherry Bonu")!= string::npos || v.find("Cherry Gold")!= string::npos || v.find("Cherry Master")!= string::npos
		|| v.find("Ruleta")!= string::npos || v.find("Buena Suerte")!= string::npos || v.find("Player's Edge")!= string::npos
		|| v.find("Touchmaster")!= string::npos)
		{
			string a = v;
			state->g->flag_set(v == a, emulator::flag_derived_pokercasino);
		}	
		
		if(v.find("Beatmania")!= string::npos || v.find("beatmania")!= string::npos || v.find("Dance Dance Revo")!= string::npos
		|| v.find("Dance Maniax")!= string::npos || v.find("Dance Freaks")!= string::npos || v.find("Dancing S")!= string::npos)
		{
			string b = v;
			state->g->flag_set(v == b, emulator::flag_derived_beatmania);
		}		

		if(v.find("Quiz")!= string::npos || v.find("Trivia")!= string::npos || v.find("Funcube")!= string::npos)
		{
			string b = v;
			state->g->flag_set(v == b, emulator::flag_derived_quiztrivial);
		}	

		if(v.find("Golf")!= string::npos || v.find("Bowling")!= string::npos || v.find("Darts")!= string::npos || v.find("Fisherman")!= string::npos)
		{
			string b = v;
			state->g->flag_set(v == b, emulator::flag_derived_golfdarfish);
		}
		
	}
}

static void process_manufacturer(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->manufacturer_set(string(s, len));
	}
}

static void process_year(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->year_set(string(s, len));
	}
}

static void process_cloneof(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (string(s, len).length())
			state->g->cloneof_set(state->e->user_name_get() + "/" + emu_tolower(string(s, len)));
	}
}

static void process_romof(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->romof_set(state->e->user_name_get() + "/" + string(s, len));
	}
}

static void process_refresh(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->refresh_set(string(s, len));
	}
}

static void process_driverstatus(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		if (v == "preliminary")
			state->g->play_set(play_preliminary);
		if (v == "imperfect" && state->g->play_get() < play_imperfect)
			state->g->play_set(play_imperfect);
	}
}

static void process_disk(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->flag_set(true, emulator::flag_derived_chd);		
	}
}

static void process_rom(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		state->rom_merge = false;
		state->rom_size = 0;
	} else if (t == token_close) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (!state->rom_merge)
			state->g->size_set(state->g->size_get() + state->rom_size);
	}
}

static void process_diskarea(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_open) {
		state->rom_merge = false;
		state->rom_size = 1;
	} else if (t == token_close) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		if (!state->rom_merge)
			state->g->size_set(state->rom_size);
	}
}

static void process_romsize(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		string v = string(s, len);
		state->rom_size = atoi(v.c_str());
	}
}

static void process_rommerge(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		state->rom_merge = 1;
	}
}

static void process_inputcoins(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->flag_set(true, emulator::flag_derived_romarcade);
	}
}

static void process_device(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		state->m = new machinedevice;
	} else if (t == token_close) {
		if (!state->g || !state->m) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->machinedevice_bag_get().insert(state->g->machinedevice_bag_get().end(), *state->m);
		delete state->m;
		state->m = 0;
	}
}

static void process_deviceextensionname(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->m) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->m->ext_bag.insert(state->m->ext_bag.end(), v);
	}
}

static void process_deviceinstancebriefname(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->m) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->m->brief = v;
	}
}

static void process_devicename(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->m) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->m->name = v;
	}
}

// softwarelist
static void process_softwarelist(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_open) {
		state->s = new softwarelist;
	} else if (t == token_close) {
		if (!state->g || !state->s) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->g->flag_set(true, emulator::flag_derived_softwarelist);
		state->g->softwarelist_bag_get().insert(state->g->softwarelist_bag_get().end(), *state->s);
		delete state->s;
		state->s = 0;
	}
}

// sharedfeat
static void process_sharedfeat(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_open) {
		state->s = new softwarelist;
	} else if (t == token_close) {
		if (!state->g || !state->s) {
			process_error(state, 0, "invalid state");
			return;
		}
		state->s->name = state->sn;
		state->g->softwarelist_bag_get().insert(state->g->softwarelist_bag_get().end(), *state->s);
		delete state->s;
		state->s = 0;
	}
}

static void process_softwarelistname(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		if (!state->s) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->s->name = v;
	}
}

static void process_softwareliststatus(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		if (!state->s) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->s->status = v;
	}
}

static void process_softwarelistfilter(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes) //
{
	if (t == token_data) {
		if (!state->s) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->s->filter = v;
	}
}

static void process_videoscreen(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->flag_set(v ==  "vector", emulator::flag_derived_vector);
	}
}

static void process_videoorientation(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->flag_set(v == "90" || v == "270" || v == "vertical", emulator::flag_derived_vertical);
	}
}

static void process_mechanical(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->flag_set(v == "yes", emulator::flag_derived_mechanical);
	}
}

static void process_mahjong(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		if (v == "mahjong")
			state->g->flag_set(v == "mahjong", emulator::flag_derived_mahjong);
	}
}

static void process_videowidth(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->sizex_set(atoi(v.c_str()));
	}
}

static void process_videoheight(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->sizey_set(atoi(v.c_str()));
	}
}

static void process_videoaspectx(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->aspectx_set(atoi(v.c_str()));
	}
}

static void process_videoaspecty(struct state_t* state, enum token_t t, const char* s, unsigned len, const char** attributes)
{
	if (t == token_data) {
		if (!state->g) {
			process_error(state, 0, "invalid state");
			return;
		}
		string v = string(s, len);
		state->g->aspecty_set(atoi(v.c_str()));
	}
}

static const char* match_mamemessraine = "mame|mess|raine";
static const char* match_gamemachine = "emu|game|machine|software";
static const char* match_generic = "menu";
static const char* match_softwarelist = "softwarelist";

/**
 * Conversion table.
 * Any element/attribute not in this table is ignored.
 */
struct conversion_t {
	unsigned depth;
	const char* name[DEPTH_MAX];
	process_t* process;
};

static struct conversion_t CONV1[] = {
	{ 1, { match_mamemessraine, match_gamemachine, 0, 0, 0 }, process_game },
	{ 1, { match_generic, match_gamemachine, 0, 0, 0 }, process_game_generic },
	{ 1, { match_softwarelist, "name", 0, 0, 0 }, process_softwarelistnameloaded },
	{ 1, { match_softwarelist, match_gamemachine, 0, 0, 0 }, process_software },
	{ 0, { 0, 0, 0, 0, 0 }, 0 }
};

static struct conversion_t CONV2[] = {
	{ 2, { match_mamemessraine, match_gamemachine, "runnable", 0, 0 }, process_runnable },
	{ 2, { match_mamemessraine, match_gamemachine, "isbios", 0, 0 }, process_isbios },
	{ 2, { match_mamemessraine, match_gamemachine, "isdevice", 0, 0 }, process_isdevice },
	{ 2, { match_mamemessraine, match_gamemachine, "isarcade", 0, 0 }, process_isarcade },
	{ 2, { match_mamemessraine, match_gamemachine, "name", 0, 0 }, process_name },
	{ 2, { match_mamemessraine, match_gamemachine, "description", 0, 0 }, process_description },
	{ 2, { match_mamemessraine, match_gamemachine, "manufacturer", 0, 0 }, process_manufacturer },
	{ 2, { match_mamemessraine, match_gamemachine, "year", 0, 0 }, process_year },
	{ 2, { match_mamemessraine, match_gamemachine, "cloneof", 0, 0 }, process_cloneof },
	{ 2, { match_mamemessraine, match_gamemachine, "romof", 0, 0 }, process_romof },
	{ 2, { match_mamemessraine, match_gamemachine, "rom", 0, 0 }, process_rom },
	{ 2, { match_mamemessraine, match_gamemachine, "disk", 0, 0 }, process_disk },
	{ 2, { match_mamemessraine, match_gamemachine, "device", 0, 0 }, process_device },
	{ 2, { match_mamemessraine, match_gamemachine, "ismechanical", 0, 0 }, process_mechanical },
	{ 2, { match_mamemessraine, match_gamemachine, "softwarelist", 0, 0 }, process_softwarelist },
	{ 2, { match_generic, match_gamemachine, "name", 0, 0 }, process_name },
	{ 2, { match_generic, match_gamemachine, "description", 0, 0 }, process_description },
	{ 2, { match_generic, match_gamemachine, "cloneof", 0, 0 }, process_cloneof },
	{ 2, { match_generic, match_gamemachine, "manufacturer", 0, 0 }, process_manufacturer },
	{ 2, { match_generic, match_gamemachine, "year", 0, 0 }, process_year },
	{ 2, { match_softwarelist, match_gamemachine, "name", 0, 0 }, process_softwarename },
	{ 2, { match_softwarelist, match_gamemachine, "cloneof", 0, 0 }, process_softwarecloneof },
	{ 2, { match_softwarelist, match_gamemachine, "description", 0, 0 }, process_description },
	{ 2, { match_softwarelist, match_gamemachine, "year", 0, 0 }, process_year },
	{ 2, { match_softwarelist, match_gamemachine, "publisher", 0, 0 }, process_manufacturer },
	{ 2, { match_softwarelist, match_gamemachine, "sharedfeat", 0, 0 }, process_sharedfeat },
	{ 2, { match_softwarelist, match_gamemachine, "part", 0, 0 }, process_device },
	{ 0, { 0, 0, 0, 0, 0 }, 0 }
};

static struct conversion_t CONV3[] = {
	{ 3, { match_mamemessraine, match_gamemachine, "rom", "merge", 0 }, process_rommerge },
	{ 3, { match_mamemessraine, match_gamemachine, "rom", "size", 0 }, process_romsize },
	{ 3, { match_mamemessraine, match_gamemachine, "device", "name", 0 }, process_devicename },
	{ 3, { match_mamemessraine, match_gamemachine, "device", "interface", 0 }, process_devicename },
	{ 3, { match_mamemessraine, match_gamemachine, "driver", "status", 0 }, process_driverstatus },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "screen", 0 }, process_videoscreen },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "orientation", 0 }, process_videoorientation },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "width", 0 }, process_videowidth },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "height", 0 }, process_videoheight },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "aspectx", 0 }, process_videoaspectx },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "aspecty", 0 }, process_videoaspecty },
	{ 3, { match_mamemessraine, match_gamemachine, "display", "rotate", 0 }, process_videoorientation },
	{ 3, { match_mamemessraine, match_gamemachine, "display", "type", 0 }, process_videoscreen },
    { 3, { match_mamemessraine, match_gamemachine, "display", "width", 0 }, process_videowidth },
    { 3, { match_mamemessraine, match_gamemachine, "display", "height", 0 }, process_videoheight },
	{ 3, { match_mamemessraine, match_gamemachine, "video", "refresh", 0 }, process_refresh },
	{ 3, { match_mamemessraine, match_gamemachine, "display", "refresh", 0 }, process_refresh },
	{ 3, { match_mamemessraine, match_gamemachine, "input", "coins", 0 }, process_inputcoins },
	{ 3, { match_mamemessraine, match_gamemachine, "softwarelist", "name", 0 }, process_softwarelistname },
	{ 3, { match_mamemessraine, match_gamemachine, "softwarelist", "status", 0 }, process_softwareliststatus },
	{ 3, { match_mamemessraine, match_gamemachine, "softwarelist", "filter", 0 }, process_softwarelistfilter },
	{ 3, { match_softwarelist, match_gamemachine, "sharedfeat", "name", 0 }, process_softwareliststatus },
	{ 3, { match_softwarelist, match_gamemachine, "sharedfeat", "value", 0 }, process_softwarelistfilter },
	{ 3, { match_softwarelist, match_gamemachine, "part", "interface", 0 }, process_devicename },
	{ 3, { match_softwarelist, match_gamemachine, "part", "dataarea", 0 }, process_rom },
	{ 3, { match_softwarelist, match_gamemachine, "part", "diskarea", 0 }, process_diskarea },
	{ 0, { 0, 0, 0, 0, 0 }, 0 }
};

static struct conversion_t CONV4[] = {
	{ 4, { match_mamemessraine, match_gamemachine, "device", "instance", "briefname" }, process_deviceinstancebriefname },
	{ 4, { match_mamemessraine, match_gamemachine, "device", "extension", "name" }, process_deviceextensionname },
	{ 4, { match_mamemessraine, match_gamemachine, "input", "control", "type" }, process_mahjong },
	{ 4, { match_softwarelist, match_gamemachine, "part", "dataarea", "size" }, process_romsize },
	{ 0, { 0, 0, 0, 0, 0 }, 0 }
};

/**
 * Identify the specified element/attribute.
 */
static struct conversion_t* identify(unsigned depth, const struct level_t* level)
{
	struct conversion_t* conv;

	switch (depth) {
	case 1 : conv = CONV1; break;
	case 2 : conv = CONV2; break;
	case 3 : conv = CONV3; break;
	case 4 : conv = CONV4; break;
	default:
		return 0;
	}

	for(unsigned i=0;conv[i].name[0];++i) {
		bool equal = true;

		// check all the item, backward
		for(int j=depth;equal && j>=0;--j) {
			if (conv[i].name[j] == match_mamemessraine) {
				if (strcmp(level[j].tag, "mame") != 0 && strcmp(level[j].tag, "mess") != 0 && strcmp(level[j].tag, "raine") != 0)
					equal = false;
			} else if (conv[i].name[j] == match_softwarelist) {
				if (strcmp(level[j].tag, "softwarelist") != 0)
					equal = false;
			} else if (conv[i].name[j] == match_generic) {
				if (strcmp(level[j].tag, "menu") != 0)
					equal = false;
			} else if (conv[i].name[j] == match_gamemachine) {
				if (strcmp(level[j].tag, "emu") != 0 && strcmp(level[j].tag, "game") != 0 && strcmp(level[j].tag, "machine") != 0 && strcmp(level[j].tag, "software") != 0)
					equal = false;
			} else {
				if (strcmp(level[j].tag, conv[i].name[j]) != 0)
					equal = false;
			}
		}

		// if match return
		if (equal)
			return &conv[i];
	}

	return 0;
}

/**
 * End Handler for the Expat parser.
 */
static void end_handler(void* data, const XML_Char* name)
{
	struct state_t* state = (struct state_t*)data;

	if (state->depth < DEPTH_MAX) {
		if (state->error == 0) {
			if (state->level[state->depth].process) {
				if (state->level[state->depth].data != 0) {
					state->level[state->depth].process(state, token_data, state->level[state->depth].data, state->level[state->depth].len, 0);
				} else {
					state->level[state->depth].process(state, token_data, "", 0, 0);
				}
				state->level[state->depth].process(state, token_close, 0, 0, 0);
			}
		}
		free(state->level[state->depth].data);
	}

	--state->depth;
}

/**
 * Data Handler for the Expat parser.
 */
void data_handler(void* data, const XML_Char* s, int len)
{
	struct state_t* state = (struct state_t*)data;

	if (state->depth < DEPTH_MAX) {
		if (state->error == 0) {
			/* accumulate the data */
			unsigned new_len = state->level[state->depth].len + len;
			state->level[state->depth].data = (char*)realloc(state->level[state->depth].data, new_len);
			if (!state->level[state->depth].data) {
				process_error(state, state->level[state->depth].tag, "low memory");
				return;
			}
			memcpy(state->level[state->depth].data + state->level[state->depth].len, s, len);
			state->level[state->depth].len += len;
		}
	}
}

/**
 * Start Handler for the Expat parser.
 */
static void start_handler(void* data, const XML_Char* name, const XML_Char** attributes)
{
	struct state_t* state = (struct state_t*)data;
	struct conversion_t* c;
	unsigned i;

	++state->depth;

	if (state->depth < DEPTH_MAX) {
		state->level[state->depth].tag = name;
		state->level[state->depth].data = 0;
		state->level[state->depth].len = 0;

		if (state->error == 0) {
			c = identify(state->depth, state->level);
			if (c) {
				state->level[state->depth].process = c->process;
				state->level[state->depth].process(state, token_open, 0, 0, attributes);
			} else {
				state->level[state->depth].process = 0;
			}

			for(i=0;attributes[i];i+=2) {
				const char* null_atts[1] = { 0 };
				start_handler(data, attributes[i], null_atts);
				data_handler(data, attributes[i+1], strlen(attributes[i+1]));
				end_handler(data, attributes[i]);
			}
		} else {
			state->level[state->depth].process = 0;
		}
	}
}

bool mame_info::load_xml(istream& is, game_set& gar)
{
	struct state_t state;
	char buf[16384];

	state.parser = XML_ParserCreate(NULL);
	if (!state.parser) {
		return false;
	}

	state.depth = -1;
	state.error = 0;
	state.e = this;
	state.g = 0;
	state.a = &gar;
	state.m = 0;
	state.s = 0;
	state.sn = string();

	XML_SetUserData(state.parser, &state);
	XML_SetElementHandler(state.parser, start_handler, end_handler);
	XML_SetCharacterDataHandler(state.parser, data_handler);

	while (1) {
		bool done;
		int len;

		is.read(buf, sizeof(buf));
		if (is.bad()) {
			process_error(&state, "", "read error");
			break;
		}

		len = is.gcount();

		done = is.eof();

		if (XML_Parse(state.parser, buf, len, done) == XML_STATUS_ERROR) {
			process_error(&state, "", XML_ErrorString(XML_GetErrorCode(state.parser)));
			break;
		}

		if (done)
			break;
	}

	XML_ParserFree(state.parser);

	if (state.error) {
		return false;
	}

	return true;
}

bool generic::load_xml(istream& is, game_set& gar)
{
	struct state_t state;
	char buf[16384];

	state.parser = XML_ParserCreate(NULL);
	if (!state.parser) {
		return false;
	}

	state.depth = -1;
	state.error = 0;
	state.e = this;
	state.g = 0;
	state.a = &gar;
	state.m = 0;
	state.s = 0;
	state.sn = string();

	XML_SetUserData(state.parser, &state);
	XML_SetElementHandler(state.parser, start_handler, end_handler);
	XML_SetCharacterDataHandler(state.parser, data_handler);

	while (1) {
		bool done;
		int len;

		is.read(buf, sizeof(buf));
		if (is.bad()) {
			process_error(&state, "", "read error");
			break;
		}

		len = is.gcount();

		done = is.eof();

		if (XML_Parse(state.parser, buf, len, done) == XML_STATUS_ERROR) {
			process_error(&state, "", XML_ErrorString(XML_GetErrorCode(state.parser)));
			break;
		}

		if (done)
			break;
	}

	XML_ParserFree(state.parser);

	if (state.error) {
		return false;
	}

	return true;
}
