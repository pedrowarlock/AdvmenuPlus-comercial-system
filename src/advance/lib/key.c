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

#include "key.h"

struct key_entry {
	const char* name;
	unsigned code;
};

static struct key_entry KEY[] = {
{ "a", KEYB_A },
{ "b", KEYB_B },
{ "c", KEYB_C },
{ "d", KEYB_D },
{ "e", KEYB_E },
{ "f", KEYB_F },
{ "g", KEYB_G },
{ "h", KEYB_H },
{ "i", KEYB_I },
{ "j", KEYB_J },
{ "k", KEYB_K },
{ "l", KEYB_L },
{ "m", KEYB_M },
{ "n", KEYB_N },
{ "o", KEYB_O },
{ "p", KEYB_P },
{ "q", KEYB_Q },
{ "r", KEYB_R },
{ "s", KEYB_S },
{ "t", KEYB_T },
{ "u", KEYB_U },
{ "v", KEYB_V },
{ "w", KEYB_W },
{ "x", KEYB_X },
{ "y", KEYB_Y },
{ "z", KEYB_Z },
{ "0", KEYB_0 },
{ "1", KEYB_1 },
{ "2", KEYB_2 },
{ "3", KEYB_3 },
{ "4", KEYB_4 },
{ "5", KEYB_5 },
{ "6", KEYB_6 },
{ "7", KEYB_7 },
{ "8", KEYB_8 },
{ "9", KEYB_9 },
{ "0_pad", KEYB_0_PAD },
{ "1_pad", KEYB_1_PAD },
{ "2_pad", KEYB_2_PAD },
{ "3_pad", KEYB_3_PAD },
{ "4_pad", KEYB_4_PAD },
{ "5_pad", KEYB_5_PAD },
{ "6_pad", KEYB_6_PAD },
{ "7_pad", KEYB_7_PAD },
{ "8_pad", KEYB_8_PAD },
{ "9_pad", KEYB_9_PAD },
{ "f1", KEYB_F1 },
{ "f2", KEYB_F2 },
{ "f3", KEYB_F3 },
{ "f4", KEYB_F4 },
{ "f5", KEYB_F5 },
{ "f6", KEYB_F6 },
{ "f7", KEYB_F7 },
{ "f8", KEYB_F8 },
{ "f9", KEYB_F9 },
{ "f10", KEYB_F10 },
{ "f11", KEYB_F11 },
{ "f12", KEYB_F12 },
{ "esc", KEYB_ESC },
{ "backquote", KEYB_BACKQUOTE },
{ "minus", KEYB_MINUS },
{ "equals", KEYB_EQUALS },
{ "backspace", KEYB_BACKSPACE },
{ "tab", KEYB_TAB },
{ "openbrace", KEYB_OPENBRACE },
{ "closebrace", KEYB_CLOSEBRACE },
{ "enter", KEYB_ENTER },
{ "semicolon", KEYB_SEMICOLON },
{ "quote", KEYB_QUOTE },
{ "backslash", KEYB_BACKSLASH },
{ "less", KEYB_LESS },
{ "comma", KEYB_COMMA },
{ "period", KEYB_PERIOD },
{ "slash", KEYB_SLASH },
{ "space", KEYB_SPACE },
{ "insert", KEYB_INSERT },
{ "del", KEYB_DEL },
{ "home", KEYB_HOME },
{ "end", KEYB_END },
{ "pgup", KEYB_PGUP },
{ "pgdn", KEYB_PGDN },
{ "left", KEYB_LEFT },
{ "right", KEYB_RIGHT },
{ "up", KEYB_UP },
{ "down", KEYB_DOWN },
{ "slash_pad", KEYB_SLASH_PAD },
{ "asterisk_pad", KEYB_ASTERISK },
{ "minus_pad", KEYB_MINUS_PAD },
{ "plus_pad", KEYB_PLUS_PAD },
{ "period_pad", KEYB_PERIOD_PAD },
{ "enter_pad", KEYB_ENTER_PAD },
{ "prtscr", KEYB_PRTSCR },
{ "pause", KEYB_PAUSE },
{ "lshift", KEYB_LSHIFT },
{ "rshift", KEYB_RSHIFT },
{ "lcontrol", KEYB_LCONTROL },
{ "rcontrol", KEYB_RCONTROL },
{ "lalt", KEYB_ALT },
{ "ralt", KEYB_ALTGR },
{ "lwin", KEYB_LWIN },
{ "rwin", KEYB_RWIN },
{ "menu", KEYB_MENU },
{ "scrlock", KEYB_SCRLOCK },
{ "numlock", KEYB_NUMLOCK },
{ "capslock", KEYB_CAPSLOCK },
{ "stop", KEYB_STOP },
{ "again", KEYB_AGAIN },
{ "props", KEYB_PROPS },
{ "undo", KEYB_UNDO },
{ "front", KEYB_FRONT },
{ "copy", KEYB_COPY },
{ "open", KEYB_OPEN },
{ "paste", KEYB_PASTE },
{ "find", KEYB_FIND },
{ "cut", KEYB_CUT },
{ "help", KEYB_HELP },
{ "menu", KEYB_MENU },
{ "calc", KEYB_CALC },
{ "setup", KEYB_SETUP },
{ "sleep", KEYB_SLEEP },
{ "wakeup", KEYB_WAKEUP },
{ "file", KEYB_FILE },
{ "sendfile", KEYB_SENDFILE },
{ "deletefile", KEYB_DELETEFILE },
{ "xfer", KEYB_XFER },
{ "prog1", KEYB_PROG1 },
{ "prog2", KEYB_PROG2 },
{ "www", KEYB_WWW },
{ "msdos", KEYB_MSDOS },
{ "coffee", KEYB_COFFEE },
{ "direction", KEYB_DIRECTION },
{ "cyclewindows", KEYB_CYCLEWINDOWS },
{ "mail", KEYB_MAIL },
{ "bookmarks", KEYB_BOOKMARKS },
{ "computer", KEYB_COMPUTER },
{ "back", KEYB_BACK },
{ "forward", KEYB_FORWARD },
{ "closecd", KEYB_CLOSECD },
{ "ejectcd", KEYB_EJECTCD },
{ "ejectclosecd", KEYB_EJECTCLOSECD },
{ "nextsong", KEYB_NEXTSONG },
{ "playpause", KEYB_PLAYPAUSE },
{ "previoussong", KEYB_PREVIOUSSONG },
{ "stopcd", KEYB_STOPCD },
{ "record", KEYB_RECORD },
{ "rewind", KEYB_REWIND },
{ "phone", KEYB_PHONE },
{ "iso", KEYB_ISO },
{ "config", KEYB_CONFIG },
{ "homepage", KEYB_HOMEPAGE },
{ "refresh", KEYB_REFRESH },
{ "exit", KEYB_EXIT },
{ "move", KEYB_MOVE },
{ "edit", KEYB_EDIT },
{ "scrollup", KEYB_SCROLLUP },
{ "scrolldown", KEYB_SCROLLDOWN },
{ "leftparen_pad", KEYB_KPLEFTPAREN },
{ "rightparen_pad", KEYB_KPRIGHTPAREN },
{ "intl1", KEYB_INTL1 },
{ "intl2", KEYB_INTL2 },
{ "intl3", KEYB_INTL3 },
{ "intl4", KEYB_INTL4 },
{ "intl5", KEYB_INTL5 },
{ "intl6", KEYB_INTL6 },
{ "intl7", KEYB_INTL7 },
{ "intl8", KEYB_INTL8 },
{ "intl9", KEYB_INTL9 },
{ "lang1", KEYB_LANG1 },
{ "lang2", KEYB_LANG2 },
{ "lang3", KEYB_LANG3 },
{ "lang4", KEYB_LANG4 },
{ "lang5", KEYB_LANG5 },
{ "lang6", KEYB_LANG6 },
{ "lang7", KEYB_LANG7 },
{ "lang8", KEYB_LANG8 },
{ "lang9", KEYB_LANG9 },
{ "playcd", KEYB_PLAYCD },
{ "pausecd", KEYB_PAUSECD },
{ "prog3", KEYB_PROG3 },
{ "prog4", KEYB_PROG4 },
{ "suspend", KEYB_SUSPEND },
{ "close", KEYB_CLOSE },
{ "brightnessdown", KEYB_BRIGHTNESSDOWN },
{ "brightnessup", KEYB_BRIGHTNESSUP },
{ "macro", KEYB_MACRO },
{ "mute", KEYB_MUTE },
{ "volumedown", KEYB_VOLUMEDOWN },
{ "volumeup", KEYB_VOLUMEUP },
{ "power", KEYB_POWER },
{ "compose", KEYB_COMPOSE },
{ "f13", KEYB_F13 },
{ "f14", KEYB_F14 },
{ "f15", KEYB_F15 },
{ "f16", KEYB_F16 },
{ "f17", KEYB_F17 },
{ "f18", KEYB_F18 },
{ "f19", KEYB_F19 },
{ "f20", KEYB_F20 },
{ "f21", KEYB_F21 },
{ "f22", KEYB_F22 },
{ "f23", KEYB_F23 },
{ "f24", KEYB_F24 },
/* joystick 1 */
{ "1joy_1", JOYB_1_1},
{ "1joy_2", JOYB_1_2 },
{ "1joy_3", JOYB_1_3 },
{ "1joy_4", JOYB_1_4 },
{ "1joy_5", JOYB_1_5 },
{ "1joy_6", JOYB_1_6 },
{ "1joy_7", JOYB_1_7 },
{ "1joy_8", JOYB_1_8 },
{ "1joy_9", JOYB_1_9 },
{ "1joy_10", JOYB_1_10 },
{ "1joy_11", JOYB_1_11 },
{ "1joy_12", JOYB_1_12 },
{ "1joy_13", JOYB_1_13 },
{ "1joy_14", JOYB_1_14 },
{ "1joy_15", JOYB_1_15 },
{ "1joy_16", JOYB_1_16 },
{ "1joy_up", JOYB_1_UP },
{ "1joy_down", JOYB_1_DOWN },
{ "1joy_left", JOYB_1_LEFT },
{ "1joy_right", JOYB_1_RIGHT },
/* joystick 2 */
{ "2joy_1", JOYB_2_1},
{ "2joy_2", JOYB_2_2 },
{ "2joy_3", JOYB_2_3 },
{ "2joy_4", JOYB_2_4 },
{ "2joy_5", JOYB_2_5 },
{ "2joy_6", JOYB_2_6 },
{ "2joy_7", JOYB_2_7 },
{ "2joy_8", JOYB_2_8 },
{ "2joy_9", JOYB_2_9 },
{ "2joy_10", JOYB_2_10 },
{ "2joy_11", JOYB_2_11 },
{ "2joy_12", JOYB_2_12 },
{ "2joy_13", JOYB_2_13 },
{ "2joy_14", JOYB_2_14 },
{ "2joy_15", JOYB_2_15 },
{ "2joy_16", JOYB_2_16 },
{ "2joy_up", JOYB_2_UP },
{ "2joy_down", JOYB_2_DOWN },
{ "2joy_left", JOYB_2_LEFT },
{ "2joy_right", JOYB_2_RIGHT },
/* joystick 3 */
{ "3joy_1", JOYB_3_1},
{ "3joy_2", JOYB_3_2 },
{ "3joy_3", JOYB_3_3 },
{ "3joy_4", JOYB_3_4 },
{ "3joy_5", JOYB_3_5 },
{ "3joy_6", JOYB_3_6 },
{ "3joy_7", JOYB_3_7 },
{ "3joy_8", JOYB_3_8 },
{ "3joy_9", JOYB_3_9 },
{ "3joy_10", JOYB_3_10 },
{ "3joy_11", JOYB_3_11 },
{ "3joy_12", JOYB_3_12 },
{ "3joy_13", JOYB_3_13 },
{ "3joy_14", JOYB_3_14 },
{ "3joy_15", JOYB_3_15 },
{ "3joy_16", JOYB_3_16 },
{ "3joy_up", JOYB_3_UP },
{ "3joy_down", JOYB_3_DOWN },
{ "3joy_left", JOYB_3_LEFT },
{ "3joy_right", JOYB_3_RIGHT },
/* joystick 4 */
{ "4joy_1", JOYB_4_1},
{ "4joy_2", JOYB_4_2 },
{ "4joy_3", JOYB_4_3 },
{ "4joy_4", JOYB_4_4 },
{ "4joy_5", JOYB_4_5 },
{ "4joy_6", JOYB_4_6 },
{ "4joy_7", JOYB_4_7 },
{ "4joy_8", JOYB_4_8 },
{ "4joy_9", JOYB_4_9 },
{ "4joy_10", JOYB_4_10 },
{ "4joy_11", JOYB_4_11 },
{ "4joy_12", JOYB_4_12 },
{ "4joy_13", JOYB_4_13 },
{ "4joy_14", JOYB_4_14 },
{ "4joy_15", JOYB_4_15 },
{ "4joy_16", JOYB_4_16 },
{ "4joy_up", JOYB_4_UP },
{ "4joy_down", JOYB_4_DOWN },
{ "4joy_left", JOYB_4_LEFT },
{ "4joy_right", JOYB_4_RIGHT },
{ 0, 0 }
};

static char key_name_buffer[64];

/**
 * Return a short name for the specified key code.
 */
const char* key_name(unsigned code)
{
	struct key_entry* i;

	for(i=KEY;i->name;++i)
		if (i->code == code)
			return i->name;

	snprintf(key_name_buffer, sizeof(key_name_buffer), "scan%d", code);

	return key_name_buffer;
}

/**
 * Convert a short name to the relative key code.
 * If the short name is unknown the KEYB_MAX value is returned.
 */
unsigned key_code(const char* name)
{
	struct key_entry* i;

	for(i=KEY;i->name;++i)
		if (strcmp(name, i->name)==0)
			return i->code;

	if (strlen(name)>=5 && strncmp(name, "scan", 4) == 0 && strspn(name+4, "0123456789") == strlen(name+4)) {
		int v;

		v = atoi(name + 4);

		if (v >= 0 && v < KEYB_MAX)
			return v;
	}

	return KEYB_MAX;
}

/**
 * Check if a key code is a defined code.
 * Not all the codes from 0 to KEYB_MAX are defined.
 */
adv_bool key_is_defined(unsigned code)
{
	struct key_entry* i;

	if (code >= KEYB_MAX)
		return 0;

	for(i=KEY;i->name;++i)
		if (i->code == code)
			return 1;

	return 0;
}

