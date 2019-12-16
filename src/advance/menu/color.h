/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 2004 Andrea Mazzoleni
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

#ifndef __COLOR_H
#define __COLOR_H

#include "conf.h"
#include "rgb.h"

#include <string>

struct int_color {
	adv_color_rgb foreground;
	adv_color_rgb background;
	adv_pixel alpha[256];
	adv_pixel opaque[256];
};

extern int_color DCOLOR_HELP_NORMAL;
extern int_color DCOLOR_HELP_TAG;
extern int_color DCOLOR_CHOICE_TITLE;
extern int_color DCOLOR_CHOICE_NORMAL;
extern int_color DCOLOR_CHOICE_HIDDEN;
extern int_color DCOLOR_CHOICE_SELECT;
extern int_color DCOLOR_CHOICE_HIDDEN_SELECT;
extern int_color DCOLOR_MENU_NORMAL;
extern int_color DCOLOR_MENU_HIDDEN;
extern int_color DCOLOR_MENU_TAG;
extern int_color DCOLOR_MENU_SELECT;
extern int_color DCOLOR_MENU_HIDDEN_SELECT;
extern int_color DCOLOR_MENU_TAG_SELECT;
extern int_color DCOLOR_MENU_BAR;
extern int_color DCOLOR_MENU_BAR_TAG;
extern int_color DCOLOR_MENU_BAR_HIDDEN;
extern int_color DCOLOR_MENU_GRID;
extern int_color DCOLOR_MENU_BACKDROP;
extern int_color DCOLOR_MENU_ICON;
extern int_color DCOLOR_MENU_CURSOR;
extern int_color DCOLOR_BACKGROUND;
extern int_color RCOLOR_HELP_NORMAL;
extern int_color RCOLOR_HELP_TAG;
extern int_color RCOLOR_CHOICE_TITLE;
extern int_color RCOLOR_CHOICE_NORMAL;
extern int_color RCOLOR_CHOICE_HIDDEN;
extern int_color RCOLOR_CHOICE_SELECT;
extern int_color RCOLOR_CHOICE_HIDDEN_SELECT;
extern int_color RCOLOR_MENU_NORMAL;
extern int_color RCOLOR_MENU_FICHA;
extern int_color RCOLOR_MENU_NFICHA;
extern int_color RCOLOR_MENU_HIDDEN;
extern int_color RCOLOR_MENU_TAG;
extern int_color RCOLOR_MENU_SELECT;
extern int_color RCOLOR_MENU_HIDDEN_SELECT;
extern int_color RCOLOR_MENU_TAG_SELECT;
extern int_color RCOLOR_MENU_BAR;
extern int_color RCOLOR_MENU_BAR_TAG;
extern int_color RCOLOR_MENU_BAR_HIDDEN;
extern int_color RCOLOR_MENU_GRID;
extern int_color RCOLOR_MENU_BACKDROP;
extern int_color RCOLOR_MENU_ICON;
extern int_color RCOLOR_MENU_CURSOR;
extern int_color COLOR_HELP_NORMAL;
extern int_color COLOR_HELP_TAG;
extern int_color COLOR_CHOICE_TITLE;
extern int_color COLOR_CHOICE_NORMAL;
extern int_color COLOR_CHOICE_HIDDEN;
extern int_color COLOR_CHOICE_SELECT;
extern int_color COLOR_CHOICE_HIDDEN_SELECT;
extern int_color COLOR_MENU_NORMAL;
extern int_color COLOR_MENU_HIDDEN;
extern int_color COLOR_MENU_TAG;
extern int_color COLOR_MENU_SELECT;
extern int_color COLOR_MENU_HIDDEN_SELECT;
extern int_color COLOR_MENU_TAG_SELECT;
extern int_color COLOR_MENU_BAR;
extern int_color COLOR_MENU_BAR_TAG;
extern int_color COLOR_MENU_BAR_HIDDEN;
extern int_color COLOR_MENU_GRID;
extern int_color COLOR_MENU_BACKDROP;
extern int_color COLOR_MENU_ICON;
extern int_color COLOR_MENU_CURSOR;
extern int_color LCOLOR_FONT_LIST;
extern int_color LCOLOR_FONT_LIST_SELECT;
extern int_color LCOLOR_GRID;
extern int_color LCOLOR_WIN;
extern int_color LCOLOR_BAR_INFO_A;
extern int_color LCOLOR_BAR_INFO_B;
extern int_color LCOLOR_BAR_INFO_C;
extern int_color LCOLOR_BAR_INFO_D;
extern int_color LCOLOR_BAR_INFO_E;
extern int_color LCOLOR_BAR_INFO_F;
extern int_color LCOLOR_BAR_INFO_G;
extern int_color LCOLOR_BAR_INFO_H;
extern int_color LCOLOR_BAR_INFO_I;
extern int_color LCOLOR_BAR_INFO_J;

extern int_color LCOLOR_BACKGROUND;
extern int_color LCOLOR_FONT_MENU;
extern int_color LCOLOR_FONT_MENU_SELECT;
extern int_color LCOLOR_FONT_MENU_TITLE;

bool color_in(const std::string& s);
void color_out(adv_conf* config_context, const char* tag);
void color_setup(adv_color_def opaque_def, adv_color_def alpha_def, unsigned translucency);
bool color_nocustom(int_color& color, const std::string& s, bool tag_is_selected, adv_color_def opaque_def, unsigned translucency);
bool color_custom(int_color& color, const std::string& s, bool& flag_special, adv_color_def opaque_def);

#endif
