/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2009 Andrea Mazzoleni
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
#include "mconfig.h"
#include "menu.h"
#include "submenu.h"
#include "text.h"
#include "play.h"
#include "advance.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windows.h> 
#include <string>  
#include <iostream> 
#include <fstream> 

using namespace std;

int ValorDaFicha=0; 
int CreditoPorFicha=0;
int bloqueado=0; 
string telefone;

string MensagemErro;
std::string KeyRegis = "TTWCCWEROOOAADCCEIJIEVEIWEPYREYWUEIYFHDFHKMEBBBEYYVVIEPWERTY";

// --------------------------------------------------------------------------
// Run
int run_sub(config_state& rs, bool silent)
{

telefone.clear(); 
telefone  = "T", telefone += "e", telefone += "l: ",	telefone += "(", telefone += "2", telefone += "1", telefone += ")",	telefone += " ";
telefone += "9", telefone += "6", telefone += "5", telefone += "4",	telefone += "-", telefone += "6", telefone += "5";
telefone += "0", telefone += "9", telefone += "4";

MensagemErro.clear(); 
MensagemErro = "Erro! Abra o Menu apartir do programa Arcade.exe\n";
MensagemErro += "Qualquer duvida, entre em contato com o fornecedor:\n";
MensagemErro += telefone;
	
//-----------------------------------------
//VERIFICA SE REGISTRO EXISTE
HKEY keyB;
if (RegOpenKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Wdiversoes\\ADVMENU"), &keyB) != ERROR_SUCCESS){
	target_err(MensagemErro.c_str());
	return EVENT_ESC;
}

if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Identities\\{33222444-AC4A-5555-AAQA-33322-AAP43}"), &keyB) != ERROR_SUCCESS){
	target_err(MensagemErro.c_str());
	return EVENT_ESC;
}
//-----------------------------------------
//Pegar o caminho completo do programa
char pBuf[256]; 
size_t len = sizeof(pBuf);
int bytes = GetModuleFileName(NULL, pBuf, len);

std::string teste = pBuf;
string newstring = teste.substr(0, 3);
newstring += "qrcdriver.d";

FILE *pFile;
pFile = fopen(newstring.c_str(), "r");
if (pFile==NULL)
{
  fclose (pFile);
	target_err(MensagemErro.c_str());
	return EVENT_ESC;
}
//-----------------------------------------

	log_std(("menu: int_enable call\n"));

	if (!int_enable(-1, -1, "none", rs.video_orientation_effective)) {
		return EVENT_ESC;
	}

	bool done = false;
	bool is_run = false;
	int key = 0;

	log_std(("menu: menu start\n"));

	while (!done) {
		emulator* emu;
			
		key = run_menu(rs, (rs.video_orientation_effective & ADV_ORIENTATION_FLIP_XY) != 0, silent);

		// don't replay the sound and clip
		silent = true;

		if (rs.console_mode && key == EVENT_ESC)
			key = EVENT_MENU;

		if (!rs.lock_effective) {
			if (key == EVENT_MENU) {
				silent = true;
				key = run_submenu(rs);
			}
		}

		switch (key) {
			case EVENT_HELP :
				silent = false;
				run_help(rs);
				break;
			case EVENT_MINFO :
				silent = false;
				run_subInfohistory(rs);
				break;
			case EVENT_FAVORITES_NEXT :
				silent = false;
				run_favorites_next(rs);
				break;
			case EVENT_EMU_NEXT :
				silent = false;
				run_emu_next(rs);
				break;
			case EVENT_EMU_PRE :
				silent = false;
				run_emu_pre(rs);
				break;
			case EVENT_TYPE :
				silent = false;
				run_type_next(rs);
				break;
			case EVENT_ATTRIB :
				silent = false;
				emu = run_emu_select(rs);
				if (emu)
					emu->attrib_run(SECOND_CHOICE_X, SECOND_CHOICE_Y);
				break;
			case EVENT_COMMAND :
				silent = false;
				run_command(rs);
				break;
			case EVENT_SORT :
				silent = false;
				run_sort(rs);
				break;
			case EVENT_SETFAVORITES :
				silent = false;
				run_favorites_move(rs);
				break;
			case EVENT_SETTYPE :
				silent = false;
				run_type_move(rs);
				break;

			case EVENT_ESCEMULE:
			
				silent = false;
				// MENU SYSTEMS: Al salir de un emulador carga el Menu Systems (si esta activado)
				if (rs.menu_systems_activated && !rs.menu_systems->state_get()) {
					emulator_container c;
					c.insert(c.end(), rs.menu_systems->user_name_get());
					rs.include_emu_set(c);
					key = EVENT_NONE;
					break;
				}

					break;
				
			case EVENT_ESC :
			case EVENT_OFF :
				silent = false;
				// MENU SYSTEMS: Al salir de un emulador carga el Menu Systems (si esta activado)
				/*if (rs.menu_systems_activated && !rs.menu_systems->state_get()) {
					emulator_container c;
					c.insert(c.end(), rs.menu_systems->user_name_get());
					rs.include_emu_set(c);
					key = EVENT_NONE;
					break;
				} */
				// Muestra ventana de confirmacion de salida (Security Exit)
				if(rs.security_exit) {
					key = run_exit(rs);
					if (key == EVENT_NONE)
						break;
				}
			case EVENT_ESC_FORCE :
			case EVENT_OFF_FORCE :
				disable_fonts();
			case EVENT_ROTATE :
				done = true;
				break;
			case EVENT_LOCK :
				rs.lock_effective = !rs.lock_effective;
				break;
			case EVENT_IDLE_0 :
				if (rs.current_game) {
					rs.current_clone = &rs.current_game->clone_best_get();
					done = true;
					is_run = true;
				}
				break;
			case EVENT_NONE :
				silent = false;
				//done = true;
				break;
			case EVENT_SETCOIN : //warlock Set coin
					silent = false;
					{
					int RetFichas=0,Fichas=0, Contador=0;
					char bufferFicha[100];
					char bufferContador[100];
					char PegaCreditoPF[100];
					char PegaFicha[100];
					char PegaBloqueado[100];
					
					//MessageBox(NULL,rs.sound_foreground_coin.c_str(),"",MB_OK|MB_SYSTEMMODAL);
										
					//PEGA O VALOR DA FICHA
					GetPrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","0",PegaFicha, 100, ".\\advmenu.ini");
					ValorDaFicha=atoi(PegaFicha);
					
					//PEGA O CREDITO POR FICHA
					GetPrivateProfileStringA("CONFIGURACAO","CREDITOPF", "1", PegaCreditoPF, 100, ".\\advmenu.ini");
					CreditoPorFicha=atoi(PegaCreditoPF);
					
					//PEGA CONTADOR GERAL
					GetPrivateProfileStringA("FICHEIRO",    "CONTADOR" ,"0", bufferContador, 100, ".\\advmenu.ini");
					Contador = atoi(bufferContador);
					
					//PEGA A QUANTIDADE DE FICHAS
					RetFichas = GetPrivateProfileStringA("FICHEIRO","FICHAS","0", bufferFicha, 100, ".\\advmenu.ini");
					Fichas   = atoi(bufferFicha);
					
					//VERIFICA SE TA BLOQUEADO
					GetPrivateProfileStringA("CONFIGURACAO","BLOQUEADO", "0", PegaBloqueado, 100, ".\\advmenu.ini");
					bloqueado=atoi(PegaBloqueado);
		
					if ( RetFichas && bloqueado >=1 ){
						Contador+=1;
						Fichas+=ValorDaFicha*CreditoPorFicha;    
						sprintf(bufferFicha,"%d",Fichas);

						if(Fichas !=0 ){ 
								WritePrivateProfileStringA("FICHEIRO","FICHAS", bufferFicha,".\\advmenu.ini");						
							}else{ 
								WritePrivateProfileStringA("FICHEIRO","FICHAS", "0", ".\\advmenu.ini");
							}
								sprintf(bufferContador, "%d", Contador);
								WritePrivateProfileStringA("FICHEIRO","CONTADOR", bufferContador, ".\\advmenu.ini");
								
								
								play_foreground_fliper_key(rs.sound_foreground_coin);
								//MessageBox(NULL,rs.sound_foreground_coin.c_str(),"",MB_OK|MB_SYSTEMMODAL);
						}
					}
					break;	
			case EVENT_ENTER :
				silent = false;
				
				if (rs.current_game) { 
			
				int RetFichas=0, Fichas=0; 
				char buffer1[100];
				char buffer2[100];
				extern int TipoEmu;
				RetFichas = GetPrivateProfileStringA("FICHEIRO","FICHAS","0", buffer1, 100, ".\\advmenu.ini");
				Fichas = atoi(buffer1);
				
				
		
			if (rs.menu_systems->state_get()) { // MENU SYSTEMS - Passa Direto no Menu Systems
				for(pemulator_container::const_iterator i=rs.emu_active.begin();i!=rs.emu_active.end();++i) {
					if(emu_tolower((*i)->user_name_get()) == rs.current_game->name_without_emulator_get()) {
					emulator_container c;
					c.insert(c.end(), (*i)->user_name_get());
					rs.include_emu_set(c);
					break;
					}
				}
			break;	
			} 
				
				
			if ( bloqueado != 0 ){ // BLOQUEADO - Se estiver bloqueado, então verificar se tem fichas
				if ( RetFichas ){	
					if (Fichas >= 60000 && !TipoEmu) { //Da enter no game MODO tempo mesmo até com 1 minuto de tempo.
						if (rs.current_game) {
							done = true;
							is_run = true;
						}
					break;
					} 
				
					if (Fichas >= ValorDaFicha) {									
						if (rs.current_game) {
							done = true;
							is_run = true;
						}
						break;
					}else{
						if ( TipoEmu ){
							play_foreground_fliper_key(rs.sound_foreground_nocoin);
						}else{
							play_foreground_fliper_key(rs.sound_foreground_notime);
						}
					}	
				break;	
				}
			}else{
					if (rs.current_game) {
						done = true;
						is_run = true;
					}
				break; 
			}
		}
		break; 		

		}
	}

	if (is_run) {
		assert(rs.current_game);
		if (!rs.current_clone) {
			if (rs.current_game->emulator_get()->tree_get())
				rs.current_clone = &rs.current_game->clone_best_get();
			else
				rs.current_clone = rs.current_game;
		}
		if (!rs.resetgame_get(rs.current_clone))
			run_runinfo(rs);
	}

	log_std(("menu: menu stop\n"));

	log_std(("menu: int_disable call\n"));
	int_disable();

	return key;
}

int run_main(config_state& rs, bool is_first, bool silent)
{
	log_std(("menu: int_set call\n"));

	if (!int_set(rs.video_gamma, rs.video_brightness, rs.idle_start_first, rs.idle_start_rep, rs.idle_saver_first,
	             rs.idle_saver_rep, rs.idle_exit_time, rs.preview_fast, rs.ui_translucency, rs.disable_special))
	{
		return EVENT_ESC;
	}

	log_std(("menu: play_init call\n"));
	if (!play_init()) {
		int_unset(true);
		target_err("Error initializing the sound mixer.\n");
		target_err("Try with the option '-device_sound none'.\n");
		return EVENT_ESC;
	}

	// play start background sounds
	if (is_first) {
		play_background_effect(rs.sound_background_begin, PLAY_PRIORITY_EVENT, false);
	} else {
		play_background_effect(rs.sound_background_stop, PLAY_PRIORITY_EVENT, false);
	}

	// play start foreground sounds
	if (is_first) {
		play_foreground_effect_begin(rs.sound_foreground_begin);
	} else {
		play_foreground_effect_stop(rs.sound_foreground_stop);
	}

	// fill the player buffer
	log_std(("menu: play_fill call\n"));
	play_fill();

	if (is_first) {
		if (rs.ui_startup != "none") {
			if (int_enable(-1, -1, "none", rs.video_orientation_effective)) {
				int_clip(rs.ui_startup, false);
				int_disable();
			}
		}
	}

	bool done = false;
	bool is_terminate = false;
	bool is_run = false;
	int key = 0;

	log_std(("menu: menu start\n"));

	while (!done) {
		key = run_sub(rs, silent);

		// don't replay the sound and clip
		silent = true;

		if (!rs.lock_effective)
			switch (key) {
				case EVENT_ROTATE : {
					unsigned mirror = rs.video_orientation_effective & (ADV_ORIENTATION_FLIP_X | ADV_ORIENTATION_FLIP_Y);
					unsigned flip = rs.video_orientation_effective & ADV_ORIENTATION_FLIP_XY;
					if (mirror == 0) {
						mirror = ADV_ORIENTATION_FLIP_Y;
					} else if (mirror == ADV_ORIENTATION_FLIP_Y) {
						mirror = ADV_ORIENTATION_FLIP_X | ADV_ORIENTATION_FLIP_Y;
					} else if (mirror == (ADV_ORIENTATION_FLIP_X | ADV_ORIENTATION_FLIP_Y)) {
						mirror = ADV_ORIENTATION_FLIP_X;
					} else {
						mirror = 0;
					}
					flip ^= ADV_ORIENTATION_FLIP_XY;
					rs.video_orientation_effective = flip | mirror;
				}
				break;
			case EVENT_ESC :
			case EVENT_OFF :
				done = true;
				is_terminate = true;
				break;
		}
		switch (key) {
		case EVENT_ESC_FORCE :
		case EVENT_OFF_FORCE :
			done = true;
			is_terminate = true;
			break;
		case EVENT_IDLE_0 :
		case EVENT_ENTER :
		case EVENT_SETCOIN :
		case EVENT_ESCEMULE :
		case EVENT_CLONE :
			if (rs.current_game && rs.current_clone) {
				done = true;
				is_run = true;
			}
			break;
		}
	}

	log_std(("menu: menu stop\n"));

	if (is_terminate) {
		play_foreground_effect_end(rs.sound_foreground_end);
		play_background_effect(rs.sound_background_end, PLAY_PRIORITY_END, false);
	}
	if (is_run) {
		play_foreground_effect_start(rs.sound_foreground_start);
		play_background_effect(rs.sound_background_start, PLAY_PRIORITY_END, false);
	}

	// fill the player buffer
	log_std(("menu: play_fill call\n"));
	play_fill();
	bool wait = true;

	if (is_terminate) {
		if (rs.ui_exit != "none") {
			if (int_enable(-1, -1, "none",  rs.video_orientation_effective)) {
				wait = int_clip(rs.ui_exit, false);
				int_disable();
			}
		}
	}

	// wait for the sound end
	if (!wait)
		play_foreground_stop();
	log_std(("menu: wait foreground stop\n"));
	play_foreground_wait();
	log_std(("menu: background stop\n"));
	play_background_stop(PLAY_PRIORITY_EVENT);
	log_std(("menu: wait background stop\n"));
	play_background_wait();

	log_std(("menu: play_done call\n"));
	play_done();

	log_std(("menu: int_unset call\n"));
	int_unset((is_terminate && rs.resetexit_get()) || (!is_terminate && rs.resetgame_get(rs.current_clone)));

	return key;
}

//---------------------------------------------------------------------------
// run_all

int run_all(adv_conf* config_context, config_state& rs)
{
	bool done = false;
	bool is_first = true;
	bool silent = false;
	int key = 0;

	rs.current_game = 0;
	rs.current_clone = 0;
	rs.fast = "";

	while (!done) {
		key = run_main(rs, is_first, silent);

		// the next
		is_first = false;

		// replay the sound and clip
		silent = false;

		switch (key) {
		case EVENT_ESC :
		case EVENT_OFF :
		case EVENT_ESC_FORCE :
		case EVENT_OFF_FORCE :
			done = true;
			break; 
		case EVENT_IDLE_0 :
		case EVENT_SETCOIN :
		case EVENT_ESCEMULE :
		case EVENT_ENTER :
		case EVENT_CLONE :
			if (key == EVENT_IDLE_0) {
				// don't replay the sound and clip
				silent = true;
			}

			if (!rs.current_clone)
				rs.current_clone = rs.current_game;

			if (rs.current_clone) {
				// save before
				rs.save(config_context);
				// save the favorites lists
				rs.save_favorites();

				// run the game
				if (rs.current_game->software_get()) {
					const game* bios;
					if (rs.current_clone->software_get())
						bios = &rs.current_clone->bios_get();
					else
						bios = rs.current_clone;
					rs.current_game->emulator_get()->run(*rs.current_game, bios, rs.video_orientation_effective, true, rs.difficulty_effective, rs.console_mode, play_attenuation_get(), key == EVENT_IDLE_0);
				} else {
					rs.current_clone->emulator_get()->run(*rs.current_clone, 0, rs.video_orientation_effective, true, rs.difficulty_effective, rs.console_mode, play_attenuation_get(), key == EVENT_IDLE_0);
				}

				// update the game info
				rs.current_clone->emulator_get()->update(*rs.current_clone);

				// save after
				rs.save(config_context);

				// print the messages
				target_flush();
			}
			break;
		}
	}

	return key;
}

//---------------------------------------------------------------------------
// Version

static void version(void)
{
	char report_buffer[128];
	target_out("AdvMenuPLUS %s\n", ADV_VERSION);
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__) /* OSDEF Detect compiler version */
#define COMPILER_RESOLVE(a) #a
#define COMPILER(a, b, c) COMPILER_RESOLVE(a) "." COMPILER_RESOLVE(b) "." COMPILER_RESOLVE(c)
	target_out("Compiled %s with gcc-%s\n", __DATE__, COMPILER(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__));
#endif
	target_out("\n");

	target_out("Drivers (in priority order):\n");
	video_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("  Video:%s\n", report_buffer);
	soundb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("  Sound:%s\n", report_buffer);
	keyb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("  Keyboard:%s\n", report_buffer);
	joystickb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("  Joystick:%s\n", report_buffer);
	mouseb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("  Mouse:%s\n", report_buffer);
	target_out("\n");

	target_out("Directories:\n");
#ifdef ADV_DATADIR
	target_out("  Data: %s\n", ADV_DATADIR);
#else
	target_out("  Data: . (current directory)\n");
#endif
	target_out("\n");

	target_out("Configuration (in priority order):\n");
	if (file_config_file_host("advmenup.rc") != 0)
		target_out("  Host configuration file (R): %s\n", file_config_file_host("advmenup.rc"));
	target_out("  Command line (R)\n");
	target_out("  Home configuration file (RW): %s\n", file_config_file_home("advmenup.rc"));
	if (file_config_file_data("advmenup.rc") != 0)
		target_out("  Data configuration file (R): %s\n", file_config_file_data("advmenup.rc"));
}

static void help(void)
{
#if !defined(__MSDOS__) && !defined(__WIN32__)
	const char* slash = "--";
#else
	const char* slash = "-";
#endif
	target_out(ADV_COPY);
	target_out("\n");
	target_out("Usage: advmenup [options]\n\n");
	target_out("Options:\n");
	target_out("%sdefault  add all the default options at the configuration file\n", slash);
	target_out("%sremove   remove all the default option from the configuration file\n", slash);
	target_out("%slog      create a log of operations\n", slash);
	target_out("%sversion  print the version\n", slash);
	target_out("\n");
#if !defined(__MSDOS__) && !defined(__WIN32__)
	target_out("To get an extensive help type 'man advmenup'\n");
	target_out("\n");
#endif
}

//---------------------------------------------------------------------------
// Configuration

static void error_callback(void* context, enum conf_callback_error error, const char* file, const char* tag, const char* valid, const char* desc, ...)
{
	va_list arg;
	va_start(arg, desc);
	target_err_va(desc, arg);
	target_err("\n");
	if (valid)
		target_err("%s\n", valid);
	va_end(arg);
}

static adv_conf_conv STANDARD[] = {
#ifdef __MSDOS__
{ "", "allegro_*", "*", "%s", "%s", "%s", ADV_CONF_CONV_AUTOREG_MULTI }, /* auto registration of the Allegro options */
#endif
{ "*", "type_inport", "*", "%s", "type_import", "%s", 0 }, /* 1.16.0 */
{ "*", "preview_aspect", "fit", "%s", "preview_expand", "3.0", 0 }, /* 1.17.4 */
{ "*", "preview_aspect", "correct", "%s", "preview_expand", "1.15", 0 }, /* 1.17.4 */
/* 2.1.0 */
{ "*", "msg_run", "*", "%s", "run_msg", "%s", 0 }, /* rename */
{ "*", "select_neogeo", "*", "", "", "", 0 }, /* remove */
{ "*", "select_neogeo", "*", "", "", "", 0 }, /* remove */
{ "*", "select_deco", "*", "", "", "", 0 }, /* remove */
{ "*", "select_playchoice", "*", "", "", "", 0 }, /* remove */
{ "*", "select_clone", "*", "", "", "", 0 }, /* remove */
{ "*", "select_bad", "*", "", "", "", 0 }, /* remove */
{ "*", "select_missing", "*", "", "", "", 0 }, /* remove */
{ "*", "select_vector", "*", "", "", "", 0 }, /* remove */
{ "*", "select_vertical", "*", "", "", "", 0 }, /* remove */
{ "*", "type_import", "none", "", "", "", 0 }, /* remove */
{ "*", "desc_import", "none", "", "", "", 0 }, /* remove */
{ "*", "device_joystick", "standard", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "dual", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "4button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "6button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "8button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "fspro", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "wingex", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sidewinder", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sidewinderag", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "gamepadpro", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "grip", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "grip4", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lp2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lp3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segaisa", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segapci", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segapcifast", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "wingwarrior", "%s", "%s", "allegro/%s", 0 }, /* rename */
/* 2.2.0 */
{ "*", "device_svgaline_divide_clock", "*", "%s", "device_svgaline_divideclock", "%s", 0 }, /* rename */
/* 2.2.2 */
{ "*", "video_depth", "*", "", "", "", 0 }, /* remove */
{ "*", "device_sdl_fullscreen", "yes", "%s", "device_video_output", "fullscreen", 0 }, /* rename */
{ "*", "device_sdl_fullscreen", "no", "%s", "device_video_output", "window", 0 }, /* rename */
{ "*", "device_video_8bit", "*", "%s", "device_color_palette8", "%s", 0 }, /* rename */
{ "*", "device_video_15bit", "*", "%s", "device_color_bgr15", "%s", 0 }, /* rename */
{ "*", "device_video_16bit", "*", "%s", "device_color_bgr16", "%s", 0 }, /* rename */
{ "*", "device_video_24bit", "*", "%s", "device_color_bgr24", "%s", 0 }, /* rename */
{ "*", "device_video_32bit", "*", "%s", "device_color_bgr32", "%s", 0 }, /* rename */
/* 2.2.12 */
{ "*", "event_exit_press", "*", "", "", "", 0 }, /* remove */
{ "*", "color", "*", "%s", "ui_color", "%s", 0 }, /* rename */
/* 2.2.13 */
{ "*", "misc_console", "*", "%s", "ui_console", "%s", 0 }, /* rename */
/* 2.2.14 */
{ "*", "loop", "yes", "%s", "ui_clip", "singleloop", 0 }, /* rename */
{ "*", "loop", "no", "%s", "ui_clip", "single", 0 }, /* rename */
/* 2.2.15 */
{ "*", "device_video_output", "zoom", "%s", "%s", "overlay", 0 }, /* rename */
{ "*", "device_video_zoom", "*", "%s", "device_video_overlay", "%s", 0 }, /* rename */
/* 2.2.17 */
{ "*", "device_video_overlay", "*", "%s", "device_video_overlaysize", "%s", 0 }, /* rename */
{ "*", "sort", "coin", "%s", "%s", "play", 0 }, /* rename */
{ "*", "sort", "timepercoin", "%s", "%s", "timeperplay", 0 }, /* rename */
/* 2.3.2 */
{ "*", "video_size", "*", "%s", "display_size", "%s", 0 }, /* rename */
{ "*", "video_font", "*", "%s", "ui_font", "%s", 0 }, /* rename */
{ "*", "video_orientation", "*", "%s", "display_orientation", "%s", 0 }, /* rename */
{ "*", "video_gamma", "*", "%s", "display_gamma", "%s", 0 }, /* rename */
{ "*", "video_brightness", "*", "%s", "display_brightness", "%s", 0 }, /* rename */
{ "*", "video_restore", "*", "%s", "display_restoreatgame", "%s", 0 }, /* rename */
{ "*", "run_msg", "*", "%s", "ui_gamemsg", "%s", 0 }, /* rename */
{ "*", "run_preview", "*", "%s", "ui_game", "%s", 0 }, /* rename */
/* 2.4.0 */
{ "*", "ui_game", "play", "%s", "%s", "snap", 0 }, /* rename */
/* plus */
{ "*", "merge", "none", "%s", "romset_type", "non-merged", 0 },
{ "*", "merge", "differential", "%s", "romset_type", "split", 0 },
{ "*", "merge", "parent", "%s", "romset_type", "merged", 0 },
{ "*", "merge", "any", "%s", "romset_type", "any", 0 },
{ "*", "merge", "disable", "%s", "romset_type", "disable", 0 },
{ "*", "emulator_file_custom", "*", "%s", "emulator_layout", "%s", 0 },
{ "*", "mode", "custom", "%s", "mode", "layout", 0 }
};

adv_error include_load(adv_conf* context, int priority, const char* include_spec, adv_bool ignore_unknown, adv_bool multi_line, const adv_conf_conv* conv_map, unsigned conv_mac, conf_error_callback* error, void* error_context)
{
	char separator[2];
	char* s;
	int i;

	separator[0] = file_dir_separator();
	separator[1] = 0;

	i = 0;
	s = strdup(include_spec);

	sskip(&i, s, " \t");
	while (s[i]) {
		char c;
		const char* file;
		const char* include_file;

		file = stoken(&c, &i, s, separator, " \t");
		sskip(&i, s, " \t");

		if (file[0] == 0 || (c != 0 && s[i] == 0)) {
			error_callback(error_context, conf_error_failure, file, 0, 0, "Error in the include file specification.");
			free(s);
			return -1;
		}

		include_file = file_config_file_home(file);

		if (access(include_file, R_OK)!=0) {
			error_callback(error_context, conf_error_failure, include_file, 0, 0, "Missing configuration include file '%s'.", include_file);
			free(s);
			return -1;
		}

		if (conf_input_file_load_adv(context, priority, include_file, 0, ignore_unknown, multi_line, conv_map, conv_mac, error_callback, error_context) != 0) {
			free(s);
			return -1;
		}
	}

	free(s);

	return 0;
}

//---------------------------------------------------------------------------
// Log

extern "C" void adv_svgalib_log_va(const char *text, va_list arg)
{
	log_va(text, arg);
}

//---------------------------------------------------------------------------
// Signal

void os_signal(int signum, void* info, void* context)
{
	os_default_signal(signum, info, context);
}

//---------------------------------------------------------------------------
// Lectura de archivos Layouts

bool layout_read(config_state& rs, const string& custom_path)
{	
	if (custom_path == "")
		return true;
		
	string path_custom_completo = file_config_file_home(custom_path.c_str());
	string dir_custom = file_dir_custom(path_custom_completo); 
		
	load_current_dir_custom(dir_custom.c_str());

	if (access(path_custom_completo.c_str(), F_OK)!=0) {
		target_err("Error opening file '%s'\n", path_custom_completo.c_str());
		return true;
	}
	
	adv_conf* custom_context;
	const char* section_map_custom[1];
	custom_context = conf_init();
	config_state::conf_register_custom(custom_context);
	section_map_custom[0] = (char*)"";
	conf_section_set(custom_context, section_map_custom, 1);

	if (conf_input_file_load_adv(custom_context, 3, path_custom_completo.c_str(), 0, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
		conf_done(custom_context);
		return false;
	}

	if (!rs.load_custom(custom_context, custom_path)) {
		conf_done(custom_context);
		return false;
	}

	conf_done(custom_context);

	return true;
}

bool layouts_load(config_state& rs)
{
	if (!layout_read(rs, rs.menu_systems->custom_file_path_get()))
		return false;
	
	for(pemulator_container::iterator j = rs.emu.begin(); j!=rs.emu.end(); j++)
		if (!layout_read(rs, (*j)->custom_file_path_get()))
			return false;
	
	return true;
}

//---------------------------------------------------------------------------
// Main

int os_main(int argc, char* argv[])
{
	config_state rs;
	adv_conf* config_context;
	bool opt_verbose;
	bool opt_log;
	bool opt_default;
	bool opt_remove;
	bool opt_logsync;
	bool opt_version;
	bool opt_help;
	const char* opt_cfg;
	int key = 0;
	const char* section_map[1];
	char cfg_buffer[512];
	
	char PegaFicha[100]; //warlock
	char PegaBloqueado[100];
	char PegaCreditoPF[100];
	int RetFicha; 
	int RetBlock; 
	int RetCreditPf; 
	
	srand(time(0));

	config_context = conf_init();

	if (os_init(config_context)!=0) {
		target_err("Error initializing the OS support.\n");
		goto err_conf;
	}
	
	/*Pega o valor de cada ficha e atribui a variavel global*/ //warlock
	RetFicha=GetPrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","0",PegaFicha, 100, ".\\advmenu.ini");
	if (RetFicha){
		ValorDaFicha=atoi(PegaFicha);
	}else
	{
		target_err("Erro ao ler o advmenu.ini.\n");
		ValorDaFicha=600000;
	}
	/*Verifica se é pra usar o bloqueio de controle caso não tenha ficha*/ //warlock
	RetBlock=GetPrivateProfileStringA("CONFIGURACAO","BLOQUEADO", "0", PegaBloqueado, 100, ".\\advmenu.ini");
	if (RetBlock){
		bloqueado=atoi(PegaBloqueado);
	}else
	{
		target_err("Erro ao ler o advmenu.ini.\n");
		bloqueado=0;
	}
	
	RetCreditPf=GetPrivateProfileStringA("CONFIGURACAO","CREDITOPF", "1", PegaCreditoPF, 100, ".\\advmenu.ini");
	if (RetCreditPf){
		CreditoPorFicha=atoi(PegaCreditoPF);
	}else
	{
		target_err("Erro ao ler o advmenu.ini.\n");
		CreditoPorFicha=1;
	}
	

	/* include file */
	conf_string_register_default(config_context, "include", "");

	config_state::conf_register(config_context);
	int_reg(config_context);
	play_reg(config_context);
	os_desktopsplash_reg(config_context);

	if (conf_input_args_load(config_context, 3, "", &argc, argv, error_callback, 0) != 0)
		goto err_init;

	opt_verbose = false;
	opt_log = false;
	opt_logsync = false;
	opt_remove = false;
	opt_default = false;
	opt_version = false;
	opt_help = false;
	opt_cfg = 0;
	
	telefone.clear(); 
	telefone  = "T", telefone += "e", telefone += "l: ",	telefone += "(", telefone += "2", telefone += "1", telefone += ")",	telefone += " ";
	telefone += "9", telefone += "6", telefone += "5", telefone += "4",	telefone += "-", telefone += "6", telefone += "5";
	telefone += "0", telefone += "9", telefone += "4";
	
	MensagemErro = "Erro! Abra o Menu apartir do programa Arcade.exe\n";
	MensagemErro += "Qualquer duvida, entre em contato com o fornecedor:\n";
	MensagemErro += telefone;
	

	
	for(int i=1;i<argc;++i) {
		if (target_option_compare(argv[i], KeyRegis.c_str())) {	//Warlock   Adicionado essa linha para impedir do advmenu abrir sem linha de comando!
			opt_cfg = argv[i+1];
			++i;
			} else {
				target_err(MensagemErro.c_str());
			goto err_init;
		} 
	}

	if (argv[2]) {      //Warlock   Adicionado essa linha para impedir do advmenu abrir sem linha de comando!
			target_err(MensagemErro.c_str());
		goto err_init; 
	}
	
	
	
	if (opt_cfg) {
		sncpy(cfg_buffer, sizeof(cfg_buffer), file_config_file_home(opt_cfg));
	} else {
		sncpy(cfg_buffer, sizeof(cfg_buffer), file_config_file_home("advmenup.rc"));
	}

	if (opt_version) {
		version();
		goto done_init;
	}

	if (opt_help) {
		help();
		goto done_init;
	}

	if (opt_log || opt_logsync) {
		remove("advmenup.log");
		if (log_init("advmenup.log", opt_logsync) != 0) {
			target_err("Error opening the log file 'advmenup.log'.\n");
			goto err_init;
		}
	}

	log_std(("menu: %s %s\n", __DATE__, __TIME__));

	if (file_config_file_host("advmenup.rc") != 0) {
		if (conf_input_file_load_adv(config_context, 4, file_config_file_host("advmenup.rc"), 0, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
			goto err_init;
		}
	}

	if (file_config_file_data("advmenup.rc") != 0) {
		if (conf_input_file_load_adv(config_context, 0, file_config_file_data("advmenup.rc"), 0, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
			goto err_init;
		}
	}

	if (conf_input_file_load_adv(config_context, 1, cfg_buffer, cfg_buffer, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0)
		goto err_init;

	if (access(cfg_buffer, F_OK)!=0) {
		target_out("Creating a standard configuration file...\n");
		config_state::conf_default(config_context);
		conf_setdefault_all_if_missing(config_context, "");
		conf_sort(config_context);
		if (conf_save(config_context, 1, 0, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file `%s' created with all the default options.\n", cfg_buffer);
		goto done_init;
	}

	if (opt_default) {
		config_state::conf_default(config_context);
		conf_setdefault_all_if_missing(config_context, "");
		if (conf_save(config_context, 1, 0, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file `%s' updated with all the default options.\n", cfg_buffer);
		goto done_init;
	}

	if (opt_remove) {
		conf_remove_all_if_default(config_context, "");
		if (conf_save(config_context, 1, 0, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file `%s' updated with all the default options removed.\n", cfg_buffer);
		goto done_init;
	}

	/* set the used section */
	section_map[0] = (char*)"";
	conf_section_set(config_context, section_map, 1);

	/* setup the include configuration file */
	/* it must be after the final conf_section_set() call */
	if (include_load(config_context, 2, conf_string_get_default(config_context, "include"), 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
		goto err_init;
	}

	if (os_desktopsplash_load(config_context) != 0) {
		target_err("Error hiding the desktop or initializing the Splash.\n");
		goto err_init;
	}
	
	log_std(("menu: *_load()\n"));

	if (!rs.load(config_context, opt_verbose)) {
		goto err_init;
	}

	if(!layouts_load(rs)) {
		goto err_init;
	}
	
	if (!int_load(config_context)) {
		goto err_init;
	}
	if (!play_load(config_context)) {
		goto err_init;
	}

	if (os_inner_init("AdvMenuPlus " ADV_VERSION) != 0) {
		target_err("Error initializing the inner OS support.\n");
		goto err_init;
	}

	event_setup(rs.sound_foreground_key, rs.repeat, rs.repeat_rep, rs.alpha_mode);

	if (!int_init(rs.video_sizex, rs.video_sizey)) {
		goto err_inner_init;
	}

	// print the messages after setting the video
	target_flush();

	// set the modifiable data
	rs.restore_load();

	key = run_all(config_context, rs);

	// restore or set the changed data
	if (rs.restore == restore_none) {
		rs.restore_save();
	}

	// print the messages before restoring the video
	target_flush();

	int_done();

	// save all the data
	rs.save(config_context);
	// save the favorites lists
	rs.save_favorites();

	if (opt_log || opt_logsync) {
		log_done();
	}

	os_inner_done();
	
done_init:
	int_unreg();
	os_done();

	if (key == EVENT_OFF || key == EVENT_OFF_FORCE)
		target_apm_shutdown();

	conf_done(config_context);

	return EXIT_SUCCESS;

err_inner_init:
	os_inner_done();

err_init:
	int_unreg();
	os_done();

err_conf:
	conf_done(config_context);
	return EXIT_FAILURE;

}


