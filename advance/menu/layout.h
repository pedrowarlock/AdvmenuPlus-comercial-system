/*
 * This file is part of the AdvMenuPLUS project.
 * 
 * Copyright (C) daesdae 2014 <daesdae@hotmail.com>
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

#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <list>
#include <string>

#include "file.h"

class layout {
	std::string name;
	
	layout();
protected:

	std::string custom_background_path;
	std::string custom_help_path;
	std::string custom_start_path;
	std::string custom_font_path;
	std::string custom_list_pos_size;
	std::string custom_scroll_pos_size;
	std::string custom_list_cols;
	std::string custom_list_rows;
	std::string custom_list_diagonal;
	std::string custom_list_selected_pos;
	std::string custom_list_align;
	std::string custom_color_font;
	std::string custom_color_font_select;
	std::string custom_color_scroll;
	std::string custom_font_size;
	std::string custom_win_snaps;
	std::string custom_win_flyers;
	std::string custom_win_cabinets;
	std::string custom_win_icons;
	std::string custom_win_marquees;
	std::string custom_win_titles;
	std::string custom_win_color;
	std::string custom_bar_info_1;
	std::string custom_bar_info_1_text;
	std::string custom_bar_info_1_color;
	std::string custom_bar_info_1_font;
	std::string custom_bar_info_1_align;
	std::string custom_bar_info_2;
	std::string custom_bar_info_2_text;
	std::string custom_bar_info_2_color;
	std::string custom_bar_info_2_font;
	std::string custom_bar_info_2_align;
	std::string custom_bar_info_3;
	std::string custom_bar_info_3_text;
	std::string custom_bar_info_3_color;
	std::string custom_bar_info_3_font;
	std::string custom_bar_info_3_align;
	std::string custom_bar_info_4;
	std::string custom_bar_info_4_text;
	std::string custom_bar_info_4_color;
	std::string custom_bar_info_4_font;
	std::string custom_bar_info_4_align;
	std::string custom_bar_info_5;
	std::string custom_bar_info_5_text;
	std::string custom_bar_info_5_color; 
	std::string custom_bar_info_5_colorx; //warlock
	std::string custom_bar_info_5_font;
	std::string custom_bar_info_5_align;
	std::string custom_background_color;
	std::string custom_menu_font_path;
	std::string custom_menu_font_size;
	std::string custom_menu_font_color;
	std::string custom_menu_font_select_color;
	std::string custom_menu_title_color;
	std::string custom_video_orientation;
	
public:
	layout(const std::string& Aname);
	~layout();

	const std::string& name_get() const { return name; }
	
	void custom_background_path_set(const std::string& A);// { custom_background_path = A; }
	const std::string& custom_background_path_get() const { return custom_background_path; }
	void custom_help_path_set(const std::string& A);// { custom_help_path = A; }
	const std::string& custom_help_path_get() const { return custom_help_path; }
	void custom_start_path_set(const std::string& A);// { custom_start_path = A; }
	const std::string& custom_start_path_get() const { return custom_start_path; }
	void custom_font_path_set(const std::string& A);// { custom_font_path = A; }
	const std::string& custom_font_path_get() const { return custom_font_path; }
	void custom_list_pos_size_set(const std::string& A) { custom_list_pos_size = A; }
	const std::string& custom_list_pos_size_get() const { return custom_list_pos_size; }
	void custom_scroll_pos_size_set(const std::string& A) { custom_scroll_pos_size = A; }
	const std::string& custom_scroll_pos_size_get() const { return custom_scroll_pos_size; }
	void custom_list_cols_set(const std::string& A) { custom_list_cols = A; }
	const std::string& custom_list_cols_get() const { return custom_list_cols; }
	void custom_list_rows_set(const std::string& A) { custom_list_rows = A; }
	const std::string& custom_list_rows_get() const { return custom_list_rows; }
	void custom_list_diagonal_set(const std::string& A) { custom_list_diagonal = A; }
	const std::string& custom_list_diagonal_get() const { return custom_list_diagonal; }
	void custom_list_selected_pos_set(const std::string& A) { custom_list_selected_pos = A; }
	const std::string& custom_list_selected_pos_get() const { return custom_list_selected_pos; }
	void custom_list_align_set(const std::string& A) { custom_list_align = A; }
	const std::string& custom_list_align_get() const { return custom_list_align; }
	void custom_color_font_set(const std::string& A) { custom_color_font = A; }
	const std::string& custom_color_font_get() const { return custom_color_font; }
	void custom_color_font_select_set(const std::string& A) { custom_color_font_select = A; }
	const std::string& custom_color_font_select_get() const { return custom_color_font_select; }
	void custom_color_scroll_set(const std::string& A) { custom_color_scroll = A; }
	const std::string& custom_color_scroll_get() const { return custom_color_scroll; }
	void custom_font_size_set(const std::string& A) { custom_font_size = A; }
	const std::string& custom_font_size_get() const { return custom_font_size; }
	void custom_win_snaps_set(const std::string& A) { custom_win_snaps = A; }
	const std::string& custom_win_snaps_get() const { return custom_win_snaps; }
	void custom_win_flyers_set(const std::string& A) { custom_win_flyers = A; }
	const std::string& custom_win_flyers_get() const { return custom_win_flyers; }
	void custom_win_cabinets_set(const std::string& A) { custom_win_cabinets = A; }
	const std::string& custom_win_cabinets_get() const { return custom_win_cabinets; }
	void custom_win_icons_set(const std::string& A) { custom_win_icons = A; }
	const std::string& custom_win_icons_get() const { return custom_win_icons; }
	void custom_win_marquees_set(const std::string& A) { custom_win_marquees = A; }
	const std::string& custom_win_marquees_get() const { return custom_win_marquees; }
	void custom_win_titles_set(const std::string& A) { custom_win_titles = A; }
	const std::string& custom_win_titles_get() const { return custom_win_titles; }
	void custom_win_color_set(const std::string& A) { custom_win_color = A; }
	const std::string& custom_win_color_get() const { return custom_win_color; }
	void custom_bar_info_1_set(const std::string& A) { custom_bar_info_1 = A; }
	const std::string& custom_bar_info_1_get() const { return custom_bar_info_1; }
	void custom_bar_info_1_text_set(const std::string& A) { custom_bar_info_1_text = A; }
	const std::string& custom_bar_info_1_text_get() const { return custom_bar_info_1_text; }
	void custom_bar_info_1_color_set(const std::string& A) { custom_bar_info_1_color = A; }
	const std::string& custom_bar_info_1_color_get() const { return custom_bar_info_1_color; }
	void custom_bar_info_1_font_set(const std::string& A);// { custom_bar_info_1_font = A; }
	const std::string& custom_bar_info_1_font_get() const { return custom_bar_info_1_font; }
	void custom_bar_info_1_align_set(const std::string& A) { custom_bar_info_1_align = A; }
	const std::string& custom_bar_info_1_align_get() const { return custom_bar_info_1_align; }
	void custom_bar_info_2_set(const std::string& A) { custom_bar_info_2 = A; }
	const std::string& custom_bar_info_2_get() const { return custom_bar_info_2; }
	void custom_bar_info_2_text_set(const std::string& A) { custom_bar_info_2_text = A; }
	const std::string& custom_bar_info_2_text_get() const { return custom_bar_info_2_text; }
	void custom_bar_info_2_color_set(const std::string& A) { custom_bar_info_2_color = A; }
	const std::string& custom_bar_info_2_color_get() const { return custom_bar_info_2_color; }
	void custom_bar_info_2_font_set(const std::string& A);// { custom_bar_info_2_font = A; }
	const std::string& custom_bar_info_2_font_get() const { return custom_bar_info_2_font; }
	void custom_bar_info_2_align_set(const std::string& A) { custom_bar_info_2_align = A; }
	const std::string& custom_bar_info_2_align_get() const { return custom_bar_info_2_align; }
	void custom_bar_info_3_set(const std::string& A) { custom_bar_info_3 = A; }
	const std::string& custom_bar_info_3_get() const { return custom_bar_info_3; }
	void custom_bar_info_3_text_set(const std::string& A) { custom_bar_info_3_text = A; }
	const std::string& custom_bar_info_3_text_get() const { return custom_bar_info_3_text; }
	void custom_bar_info_3_color_set(const std::string& A) { custom_bar_info_3_color = A; }
	const std::string& custom_bar_info_3_color_get() const { return custom_bar_info_3_color; }
	void custom_bar_info_3_font_set(const std::string& A);// { custom_bar_info_3_font = A; }
	const std::string& custom_bar_info_3_font_get() const { return custom_bar_info_3_font; }
	void custom_bar_info_3_align_set(const std::string& A) { custom_bar_info_3_align = A; }
	const std::string& custom_bar_info_3_align_get() const { return custom_bar_info_3_align; }
	void custom_bar_info_4_set(const std::string& A) { custom_bar_info_4 = A; }
	const std::string& custom_bar_info_4_get() const { return custom_bar_info_4; }
	void custom_bar_info_4_text_set(const std::string& A) { custom_bar_info_4_text = A; }
	const std::string& custom_bar_info_4_text_get() const { return custom_bar_info_4_text; }
	void custom_bar_info_4_color_set(const std::string& A) { custom_bar_info_4_color = A; }
	const std::string& custom_bar_info_4_color_get() const { return custom_bar_info_4_color; }
	void custom_bar_info_4_font_set(const std::string& A);// { custom_bar_info_4_font = A; }
	const std::string& custom_bar_info_4_font_get() const { return custom_bar_info_4_font; }
	void custom_bar_info_4_align_set(const std::string& A) { custom_bar_info_4_align = A; }
	const std::string& custom_bar_info_4_align_get() const { return custom_bar_info_4_align; }
	void custom_bar_info_5_set(const std::string& A) { custom_bar_info_5 = A; }
	const std::string& custom_bar_info_5_get() const { return custom_bar_info_5; }
	void custom_bar_info_5_text_set(const std::string& A) { custom_bar_info_5_text = A; }
	const std::string& custom_bar_info_5_text_get() const { return custom_bar_info_5_text; }
	
	void custom_bar_info_5_color_set(const std::string& A) { custom_bar_info_5_color = A; }
	const std::string& custom_bar_info_5_color_get() const { return custom_bar_info_5_color; }
	
	void custom_bar_info_5_colorx_set(const std::string& A) { custom_bar_info_5_colorx = A; } //warlock
	const std::string& custom_bar_info_5_colorx_get() const { return custom_bar_info_5_colorx; }
	
	void custom_bar_info_5_font_set(const std::string& A);// { custom_bar_info_5_font = A; }
	const std::string& custom_bar_info_5_font_get() const { return custom_bar_info_5_font; }
	void custom_bar_info_5_align_set(const std::string& A) { custom_bar_info_5_align = A; }
	const std::string& custom_bar_info_5_align_get() const { return custom_bar_info_5_align; }
	void custom_background_color_set(const std::string& A) { custom_background_color = A; }
	const std::string& custom_background_color_get() const { return custom_background_color; }
	void custom_menu_font_path_set(const std::string& A);// { custom_menu_font_path = A; }
	const std::string& custom_menu_font_path_get() const { return custom_menu_font_path; }
	void custom_menu_font_size_set(const std::string& A) { custom_menu_font_size = A; }
	const std::string& custom_menu_font_size_get() const { return custom_menu_font_size; }
	void custom_menu_title_color_set(const std::string& A) { custom_menu_title_color = A; }
	const std::string& custom_menu_title_color_get() const { return custom_menu_title_color; }
	void custom_menu_font_color_set(const std::string& A) { custom_menu_font_color = A; }
	const std::string& custom_menu_font_color_get() const { return custom_menu_font_color; }
	void custom_menu_font_select_color_set(const std::string& A) { custom_menu_font_select_color = A; }
	const std::string& custom_menu_font_select_color_get() const { return custom_menu_font_select_color; }
	void custom_video_orientation_set(const std::string& A) { custom_video_orientation = A; }
	const std::string& custom_video_orientation_get() const { return custom_video_orientation; }
	
};

typedef std::list<layout*> playout_container;

static struct layout_option {
	const char* option;
	const char* def_value;
	void (layout::*set)(const std::string& A);
} OPTION_LAYOUT[] = {
	{ "background_image_path", "none", &layout::custom_background_path_set },
	{ "help_image_path", "none", &layout::custom_help_path_set  },
	{ "start_image_path", "none", &layout::custom_start_path_set },
	{ "list_font_path", "auto", &layout::custom_font_path_set },
	{ "list_pos_size", "", &layout::custom_list_pos_size_set },
	{ "scroll_pos_size", "", &layout::custom_scroll_pos_size_set },
	{ "list_cols", "1 0", &layout::custom_list_cols_set },
	{ "list_rows", "auto", &layout::custom_list_rows_set },
	{ "list_diagonal", "0", &layout::custom_list_diagonal_set },
	{ "list_selected_pos", "none", &layout::custom_list_selected_pos_set },
	{ "list_align", "left", &layout::custom_list_align_set },
	{ "list_font_color", "", &layout::custom_color_font_set },
	{ "list_font_select_color", "", &layout::custom_color_font_select_set },
	{ "scroll_color", "", &layout::custom_color_scroll_set },
	{ "list_font_size", "", &layout::custom_font_size_set },
	{ "win_snaps", "none", &layout::custom_win_snaps_set },
	{ "win_flyers", "none", &layout::custom_win_flyers_set },
	{ "win_cabinets", "none", &layout::custom_win_cabinets_set },
	{ "win_icons", "none", &layout::custom_win_icons_set },
	{ "win_marquees", "none", &layout::custom_win_marquees_set },
	{ "win_titles", "none", &layout::custom_win_titles_set },
	{ "win_color", "none", &layout::custom_win_color_set },
	{ "bar_info_1", "none", &layout::custom_bar_info_1_set },
	{ "bar_info_1_text", "", &layout::custom_bar_info_1_text_set },
	{ "bar_info_1_color", "", &layout::custom_bar_info_1_color_set },
	{ "bar_info_1_font", "auto", &layout::custom_bar_info_1_font_set },
	{ "bar_info_1_align", "left", &layout::custom_bar_info_1_align_set },
	{ "bar_info_2", "none", &layout::custom_bar_info_2_set },
	{ "bar_info_2_text", "", &layout::custom_bar_info_2_text_set },
	{ "bar_info_2_color", "", &layout::custom_bar_info_2_color_set },
	{ "bar_info_2_font", "auto", &layout::custom_bar_info_2_font_set },
	{ "bar_info_2_align", "left", &layout::custom_bar_info_2_align_set },
	{ "bar_info_3", "none", &layout::custom_bar_info_3_set },
	{ "bar_info_3_text", "", &layout::custom_bar_info_3_text_set },
	{ "bar_info_3_color", "", &layout::custom_bar_info_3_color_set },
	{ "bar_info_3_font", "auto", &layout::custom_bar_info_3_font_set },
	{ "bar_info_3_align", "left", &layout::custom_bar_info_3_align_set },
	{ "bar_info_4", "none", &layout::custom_bar_info_4_set },
	{ "bar_info_4_text", "", &layout::custom_bar_info_4_text_set },
	{ "bar_info_4_color", "", &layout::custom_bar_info_4_color_set },
	{ "bar_info_4_font", "auto", &layout::custom_bar_info_4_font_set },
	{ "bar_info_4_align", "left", &layout::custom_bar_info_4_align_set },
	{ "bar_info_Coin", "none", &layout::custom_bar_info_5_set },
	{ "bar_info_Coin_text", "", &layout::custom_bar_info_5_text_set },
	{ "bar_info_Coin_color", "", &layout::custom_bar_info_5_color_set },
	{ "bar_info_Coin_NoCoin", "",&layout::custom_bar_info_5_colorx_set }, //warlock
	{ "bar_info_Coin_font", "auto", &layout::custom_bar_info_5_font_set },
	{ "bar_info_Coin_align", "left", &layout::custom_bar_info_5_align_set },
	{ "background_color", "", &layout::custom_background_color_set },
	{ "menu_font_path", "auto", &layout::custom_menu_font_path_set },
	{ "menu_font_size", "", &layout::custom_menu_font_size_set },
	{ "menu_title_color", "", &layout::custom_menu_title_color_set },
	{ "menu_font_color", "", &layout::custom_menu_font_color_set },
	{ "menu_font_select_color", "", &layout::custom_menu_font_select_color_set },
	{ "orientation", "", &layout::custom_video_orientation_set },
	{ 0, 0, &layout::custom_help_path_set } // OJO: DEBE SER LA ULTIMA
};

#endif
