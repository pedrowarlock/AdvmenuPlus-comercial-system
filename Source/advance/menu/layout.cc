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

#include "layout.h"

using namespace std;

layout::layout(const string& Aname) : name(Aname)
{
}

void layout::custom_background_path_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());
	
	custom_background_path = A_path;
}

void layout::custom_help_path_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_help_path = A_path;
}

void layout::custom_start_path_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_start_path = A_path;
}


void layout::custom_font_path_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_font_path = A_path;
}

void layout::custom_bar_info_1_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_1_font = A_path;
}

void layout::custom_bar_info_2_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_2_font = A_path;
}

void layout::custom_bar_info_3_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_3_font = A_path;
}

void layout::custom_bar_info_4_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_4_font = A_path;
}

void layout::custom_bar_info_5_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_5_font = A_path;
}

void layout::custom_bar_info_6_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_6_font = A_path;
}

void layout::custom_bar_info_7_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_7_font = A_path;
}

void layout::custom_bar_info_8_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_8_font = A_path;
}

void layout::custom_bar_info_9_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_9_font = A_path;
}
 
void layout::custom_bar_info_10_font_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_bar_info_10_font = A_path;
}


void layout::custom_menu_font_path_set(const string& A)
{
	string A_path = A;
	if (A_path != ""  && A_path != "none" && A_path != "default" && A_path != "auto")
		A_path = file_config_file_custom(A.c_str());

	custom_menu_font_path = A_path;
}



