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

#include "submenu.h"
#include "text.h"
#include "play.h"

#include "advance.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windows.h> //warlock
#include <string>  
#include <iostream> 
#include <fstream> 

#include <vector>
#include <iostream>
#include <string>
#include <fstream>


using namespace std;
extern string KeyRegis;
extern vector<string> ListaEmuladores;
extern int ValorDaFicha;
extern bool NumerarLista; //warlock
int ItemSelecionado=0;


	
#define MSG_CHOICE_DX 30*int_font_dx_get()
#define MSG_CHOICE_X (int_dx_get()-MSG_CHOICE_DX)/2
#define MSG_CHOICE_Y int_dy_get()/2

string menu_name(config_state& rs, const string& s, unsigned event)
{
	if (!rs.menu_key)
		return s;

	string name = event_name(event);
	if (!name.length())
		return s;

	return s + "^" + name;
}

// ------------------------------------------------------------------------
// Sort menu

#define SORT_CHOICE_DX 40*int_font_dx_get()

int run_sort(config_state& rs)
{
	choice_bag ch;
	
	ch.insert(ch.end(), choice("Nome (Padrao)", sort_by_name));
	ch.insert(ch.end(), choice("Parente", sort_by_root_name));
	ch.insert(ch.end(), choice("Emulador", sort_by_emulator));
	ch.insert(ch.end(), choice("Tempo jogado", sort_by_time));
	ch.insert(ch.end(), choice("Vezes jogado", sort_by_session));
	ch.insert(ch.end(), choice("Tempo jogando", sort_by_timepersession));
	ch.insert(ch.end(), choice("Tipo", sort_by_type));
	ch.insert(ch.end(), choice("Fabricante", sort_by_manufacturer));
	ch.insert(ch.end(), choice("Ano", sort_by_year));
	ch.insert(ch.end(), choice("Tamanho", sort_by_size));
	ch.insert(ch.end(), choice("Resolucao", sort_by_res));
	ch.insert(ch.end(), choice("Informacao", sort_by_info));

	choice_bag::iterator i = ch.find_by_value(rs.sort_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Selecione o modo de ordenar", THIRD_CHOICE_X, THIRD_CHOICE_Y, SORT_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.sort_set((listsort_t)i->value_get());
	}

	return key;
}

//AUTO ENVIO DE FICHAS
#define AUTOENVIO_CHOICE_DX 65*int_font_dx_get()

int run_autoenvio(config_state& rs)
{
	if (!rs.current_game) 
		return false;
	
	choice_bag ch;
	ostringstream game_name,rom_name;
	string SetValor="",GetSessao="",rom="", GetTitle="";
	string GetEmule = rs.current_game->emulator_get()->user_name_get().c_str();
	char Buffer[500], Buffer2[500];
	const game* base;
	base = rs.current_game;

	
	game_name << base->description_get();
	if (rs.current_game)
		rom = rs.current_game->name_without_emulator_get();
		rom_name << rom;
	
	//Verifica se o sistema é de jogos de PC ou TAITO
	string GetValor= ".\\ini\\" + GetEmule + ".ini";
	GetPrivateProfileStringA("CONFIGURACAO","PCTAITO_GAMES","0", Buffer2, 500, GetValor.c_str()); 
	//Caso seja PC/TAITO - Buscar informação no Games.ini - Se não, buscar na pasta .//ini//nome_do_emulador.ini
	if (atoi(Buffer2) >= 1){
			GetTitle= "JOGO (" + game_name.str() + ")";
			SetValor= ".\\ini\\Games.ini";
			GetSessao = rom_name.str();
			GetPrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","0", Buffer, 500, SetValor.c_str()); 
	}else{
			GetTitle = "EMULADOR (" + GetEmule + ")";
			SetValor= ".\\ini\\" + GetEmule + ".ini";
			GetSessao = "CONFIGURACAO";
			GetPrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","0", Buffer, 500, SetValor.c_str()); 
	}
	

	int valor = atoi(Buffer);
		
		if (valor <= 0){
			ch.insert(ch.end(), choice(">> DESATIVADO" ,true, 0));
		}else{
			ch.insert(ch.end(), choice("DESATIVADO" ,false, 0));
		}
		
		if (valor == 10000){
			ch.insert(ch.end(), choice(">> 10 SEGUNDOS" ,true, 1));
		}else{
			ch.insert(ch.end(), choice("10 SEGUNDOS" ,false, 1));
		}
		if (valor == 15000){
			ch.insert(ch.end(), choice(">> 15 SEGUNDOS" ,true, 2));
		}else{
			ch.insert(ch.end(), choice("15 SEGUNDOS" ,false, 2));
		}
		if (valor == 20000){
			ch.insert(ch.end(), choice(">> 20 SEGUNDOS" ,true, 3));
		}else{
			ch.insert(ch.end(), choice("20 SEGUNDOS" ,false, 3));
		}
		if (valor == 25000){
			ch.insert(ch.end(), choice(">> 25 SEGUNDOS" ,true, 4));
		}else{
			ch.insert(ch.end(), choice("25 SEGUNDOS" ,false, 4));
		}
		if (valor == 30000){
			ch.insert(ch.end(), choice(">> 30 SEGUNDOS" ,true, 5));
		}else{
			ch.insert(ch.end(), choice("30 SEGUNDOS" ,false, 5));
		}
		if (valor == 35000){
			ch.insert(ch.end(), choice(">> 35 SEGUNDOS" ,true, 6));
		}else{
			ch.insert(ch.end(), choice("35 SEGUNDOS" ,false, 6));
		}
	

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run(" AUTO ENVIO DE FCHAS PARA O " + GetTitle, SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, AUTOENVIO_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","0",SetValor.c_str());
				break;
			case 1 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","10000",SetValor.c_str());
				break;
			case 2 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","15000",SetValor.c_str());
				break;
			case 3 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","20000",SetValor.c_str());
				break;
			case 4 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","25000",SetValor.c_str());
				break;
			case 5 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","30000",SetValor.c_str());
				break;
			case 6 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"FICHA_AUTOENVIO","35000",SetValor.c_str());
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}

//SAIR OCIOSO
#define OCIOSO_CHOICE_DX 45*int_font_dx_get()

int run_sairocioso(config_state& rs)
{
	choice_bag ch;
	
	char Buffer[100];
	GetPrivateProfileStringA("CONFIGURACAO","OCIOSO","0", Buffer, 100, ".\\advmenu.ini"); 
	int valor = atoi(Buffer);
		
		if (valor <= 0){
			ch.insert(ch.end(), choice(">> DESATIVADO" ,true, 0));
		}else{
			ch.insert(ch.end(), choice("DESATIVADO" ,false, 0));
		}
		
		if (valor == 60000){
			ch.insert(ch.end(), choice(">> 1 MINUTO" ,true, 1));
		}else{
			ch.insert(ch.end(), choice("1 MINUTO" ,false, 1));
		}
		if (valor == 120000){
			ch.insert(ch.end(), choice(">> 2 MINUTOS" ,true, 2));
		}else{
			ch.insert(ch.end(), choice("2 MINUTOS" ,false, 2));
		}
		if (valor == 180000){
			ch.insert(ch.end(), choice(">> 3 MINUTOS" ,true, 3));
		}else{
			ch.insert(ch.end(), choice("3 MINUTOS" ,false, 3));
		}
		if (valor == 300000){
			ch.insert(ch.end(), choice(">> 5 MINUTOS" ,true, 4));
		}else{
			ch.insert(ch.end(), choice("5 MINUTOS" ,false, 4));
		}
		if (valor == 600000){
			ch.insert(ch.end(), choice(">> 10 MINUTOS" ,true, 5));
		}else{
			ch.insert(ch.end(), choice("10 MINUTOS" ,false, 5));
		}
		if (valor == 900000){
			ch.insert(ch.end(), choice(">> 15 MINUTOS" ,true, 6));
		}else{
			ch.insert(ch.end(), choice("15 MINUTOS" ,false, 6));
		}
	

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run(" SAIR OCIOSO", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, OCIOSO_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","0",".\\advmenu.ini");
				break;
			case 1 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","60000",".\\advmenu.ini");
				break;
			case 2 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","120000",".\\advmenu.ini");
				break;
			case 3 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","180000",".\\advmenu.ini");
				break;
			case 4 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","300000",".\\advmenu.ini");
				break;
			case 5 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","600000",".\\advmenu.ini");
				break;
			case 6 : 
				WritePrivateProfileStringA("CONFIGURACAO","OCIOSO","900000",".\\advmenu.ini");
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}
//REINICIALIZAÇÃO
#define REBOOT_CHOICE_DX 45*int_font_dx_get()

int run_reboot(config_state& rs)
{
	choice_bag ch;
	
		ch.insert(ch.end(), choice("Iniciar Sistema arcade com o Computador",false, 0));
		ch.insert(ch.end(), choice("Iniciar Windows Explorer com o Computador",false, 1));;
		ch.insert(ch.end(), choice("Reiniciar Programa",false, 2));
		ch.insert(ch.end(), choice("Reiniciar Maquina completa",false, 3));
		ch.insert(ch.end(), choice("Desligar Maquina",false, 4));
	
			

	choice_bag::iterator i = ch.begin();
	string sp;
	
	int key = ch.run(" Reinicializacao/Desligamento", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, REBOOT_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				ShellExecute(0, "open", "ARCADE.exe", " iniciar_arcade_on", 0, SW_HIDE);
				break;	
			case 1 : 
				ShellExecute(0, "open", "ARCADE.exe", " iniciar_arcade_off", 0, SW_HIDE);
				break;	
			case 2 : 
				sp = "/c taskkill /f /im advmenu.exe && TIMEOUT 2 && CALL advmenu.exe ";
				sp += "-";
				sp += KeyRegis;
				ShellExecute(0, "open", "cmd.exe", sp.c_str(), 0, SW_HIDE);
				break;	
			case 3 :
				ShellExecute(0, "open", "cmd.exe", "/c shutdown /r /t 0", 0, SW_HIDE);
				break;
			case 4 :
				ShellExecute(0, "open", "cmd.exe", "/c shutdown /p", 0, SW_HIDE);
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}
//REMOVER EMULADOR
#define REMOVEMULE_CHOICE_DX 45*int_font_dx_get()

int remover_emu(config_state& rs)
{
	choice_bag ch;
	char Buffer[100];
	
	bool done = false;
	
		for (size_t i = 0; i < ListaEmuladores.size(); ++i) {
			string path =  ".\\ini\\" + ListaEmuladores[i] + ".ini";
			GetPrivateProfileStringA("configuracao","OCULTAR","1", Buffer, 100, path.c_str()); 
			if (atoi(Buffer) >= 1){
				if (ItemSelecionado == i){
					ch.insert(ch.end(), choice(" - " + ListaEmuladores[i] + " (REMOVIDO)", true, i));
				}else{
					ch.insert(ch.end(), choice(" - " + ListaEmuladores[i] + " (REMOVIDO)", false, i));
				}
			}else{
				if (ItemSelecionado == i){
					ch.insert(ch.end(), choice(" + " + ListaEmuladores[i], true, i));
				}else{
					ch.insert(ch.end(), choice(" + " + ListaEmuladores[i], false, i));
				}
				
			}
		}
	



	choice_bag::iterator i = ch.begin();
	do {
	void* save = int_save();
	int key = ch.run(" Remover Emuladores (NECESSITA REINICIALIZACAO)", SECOND_CHOICE_X+240, SECOND_CHOICE_Y, REMOVEMULE_CHOICE_DX, i);
	
	
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
				string path =  ".\\ini\\" + ListaEmuladores[i->value_get()] + ".ini";
				GetPrivateProfileStringA("configuracao","OCULTAR","0", Buffer, 100, path.c_str()); 
				if (atoi(Buffer) >= 1){
					WritePrivateProfileStringA("configuracao","OCULTAR","0",path.c_str());
				}else{
					WritePrivateProfileStringA("configuracao","OCULTAR","1",path.c_str());
				}
				ItemSelecionado = i->value_get();
				done = true;
			
		}
	}
		if (key == EVENT_ESC || key == EVENT_ESCEMULE ) {
			done =true;
			return true;
		}
		int_restore(save);
		
	} while (!done);
	return false;
}



//EMULE_TIPO
#define TIPOEMU_CHOICE_DX 40*int_font_dx_get()

int run_topoemu(config_state& rs)
{
	if (!rs.current_game) 
		return false;
	
	string GetEmule = "",path="";
	GetEmule = rs.current_game->emulator_get()->user_name_get();
	path = ".\\ini\\" + GetEmule + ".ini";
		
	choice_bag ch;
	if (!rs.current_game){
		GetEmule = "";
		ch.insert(ch.end(), choice("Selecione pelo menos um jogo", -1));
	}else{
	
		
		if (GetEmule == "MENU SYSTEMS"){
			GetEmule = rs.current_game->name_without_emulator_get();
		}
			char Buffer[100];
			
			GetPrivateProfileStringA("configuracao","MODOFICHAS","0", Buffer, 100, path.c_str() );
			//GetPrivateProfileStringA("MODOFICHAS",GetEmule.c_str(),"0", Buffer, 100, ".\\advmenu.ini");  
			
			if (atoi(Buffer) >= 1){
				ch.insert(ch.end(), choice(">> FICHA",true, 0));
				ch.insert(ch.end(), choice("   TEMPO",false, 1));
			}else{
				ch.insert(ch.end(), choice("   FICHA",false,0));
				ch.insert(ch.end(), choice(">> TEMPO",true, 1)); 
			}
			
	}

		choice_bag::iterator i = ch.begin();


		int key = ch.run("Tipo de emulador - " + GetEmule, SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, TIPOEMU_CHOICE_DX, i);
		if (key == EVENT_ENTER) {
			int r;
			if (i->value_get()>=-1 && i->value_get()<=256) {
				switch (i->value_get()) {
				case 0 : 
					//WritePrivateProfileStringA("MODOFICHAS",GetEmule.c_str(),"1",".\\advmenu.ini");
					WritePrivateProfileStringA("configuracao","MODOFICHAS","1",path.c_str());
					r = 0;
					break;	
				case 1 :
					WritePrivateProfileStringA("configuracao","MODOFICHAS","0",path.c_str());
					r = 0;
					break;
				default:
					r = -1;
					break;
				}
			} 

			return true;
		} 

		return false;
}

//EMULE_TIPO
#define CREDPFICHA_CHOICE_DX 40*int_font_dx_get()

int run_creditfichas(config_state& rs)
{
	choice_bag ch;
	/*
	string GetEmule = rs.current_game->emulator_get()->user_name_get();
	if (GetEmule == "MENU SYSTEMS"){
		GetEmule = rs.current_game->name_without_emulator_get();
	}*/
		char Buffer[100];
		GetPrivateProfileStringA("CONFIGURACAO","CREDITOPF","1", Buffer, 100, ".\\advmenu.ini"); 
		
		if (atoi(Buffer) == 1){
			ch.insert(ch.end(), choice(">> 01 FICHA",true, 0));
		}else{
			ch.insert(ch.end(), choice("01 FICHA",false, 0));
		}
		if (atoi(Buffer) == 2){
			ch.insert(ch.end(), choice(">> 02 FICHAS",true, 1));
		}else{
			ch.insert(ch.end(), choice("02 FICHAS",false, 1));
		}
		if (atoi(Buffer) == 3){
			ch.insert(ch.end(), choice(">> 03 FICHAS",true, 2));
		}else{
			ch.insert(ch.end(), choice("03 FICHAS",false, 2));
		}
		if (atoi(Buffer) == 4){
			ch.insert(ch.end(), choice(">> 04 FICHAS",true, 3));
		}else{
			ch.insert(ch.end(), choice("04 FICHAS",false, 3));
		}
		if (atoi(Buffer) == 5){
			ch.insert(ch.end(), choice(">> 05 FICHAS",true, 4));
		}else{
			ch.insert(ch.end(), choice("05 FICHAS",false, 4));
		}
		if (atoi(Buffer) == 6){
			ch.insert(ch.end(), choice(">> 06 FICHAS",true, 5));
		}else{
			ch.insert(ch.end(), choice("06 FICHAS",false, 5));
		}
		if (atoi(Buffer) == 7){
			ch.insert(ch.end(), choice(">> 07 FICHAS",true, 6));
		}else{
			ch.insert(ch.end(), choice("07 FICHAS",false, 6));
		}
		if (atoi(Buffer) == 8){
			ch.insert(ch.end(), choice(">> 08 FICHAS",true, 7));
		}else{
			ch.insert(ch.end(), choice("08 FICHAS",false, 7));
		}
		if (atoi(Buffer) == 9){		
			ch.insert(ch.end(), choice(">> 09 FICHAS",true, 8));
		}else{
			ch.insert(ch.end(), choice("09 FICHAS",false, 8));
		}
		if (atoi(Buffer) == 10){		
			ch.insert(ch.end(), choice(">> 10 FICHAS",true, 9));
		}else{
			ch.insert(ch.end(), choice("10 FICHAS",false, 9));
		}

	choice_bag::iterator i = ch.begin();
	
	
	int key = ch.run(" Credito por fichas", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, CREDPFICHA_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","1",".\\advmenu.ini");
				break;	
			case 1 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","2",".\\advmenu.ini");
				break;
			case 2 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","3",".\\advmenu.ini");
				break;
			case 3 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","4",".\\advmenu.ini");
				break;
			case 4 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","5",".\\advmenu.ini");
				break;
			case 5 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","6",".\\advmenu.ini");
				break;
			case 6 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","7",".\\advmenu.ini");
				break;
			case 7 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","8",".\\advmenu.ini");
				break;
			case 8 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","9",".\\advmenu.ini");
				break;
			case 9 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","10",".\\advmenu.ini");
				break;
			case 10 :
				WritePrivateProfileStringA("CONFIGURACAO","CREDITOPF","11",".\\advmenu.ini");
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}
//Delay de inicialização
#define DELAY_CHOICE_DX 50*int_font_dx_get()

int run_delayintro(config_state& rs)
{
	choice_bag ch;
	bool Demo=false;
	char Buffer[100];
	GetPrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","0", Buffer, 100, ".\\advmenu.ini"); 
		
		
		ch.insert(ch.end(), choice("Desativar",(atoi(Buffer) > 0)? false : true, 0));
		ch.insert(ch.end(), choice("3 SEGUNDOS",(atoi(Buffer) != 3000)? false : true, 1));
		ch.insert(ch.end(), choice("5 SEGUNDOS",(atoi(Buffer) != 5000)? false : true, 2));
		ch.insert(ch.end(), choice("8 SEGUNDOS",(atoi(Buffer) != 8000)? false : true, 3));
		ch.insert(ch.end(), choice("10 SEGUNDOS",(atoi(Buffer) != 10000)? false : true, 4));
	choice_bag::iterator i = ch.begin();
	
	string sp;
	int key = ch.run(" Tempo de demonstracao do Fade (Tela de carregando...)", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, DELAY_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","0",".\\advmenu.ini");
				break;	
			case 1 :
				WritePrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","3000",".\\advmenu.ini");
				break;
			case 2 :
				WritePrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","5000",".\\advmenu.ini");
				break;
			case 3 :
				WritePrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","8000",".\\advmenu.ini");
				break;
			case 4 :
				WritePrivateProfileStringA("CONFIGURACAO","INTRO_DEMO","10000",".\\advmenu.ini");
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}
//TESTAR JOYSTICKS
#define JOYSTICKTEST_CHOICE_DX 40*int_font_dx_get()

int run_testjoy(config_state& rs)
{
	choice_bag ch;
		ch.insert(ch.end(), choice("Testar Controle 1",true, 0));
		ch.insert(ch.end(), choice("Testar Controle 2",false, 1));
		ch.insert(ch.end(), choice("Testar Controle 3",false, 2));
		ch.insert(ch.end(), choice("Configurador de Teclas",false, 3));

	choice_bag::iterator i = ch.begin();
	
	string sp;
	int key = ch.run(" Testar / Configurar Controles", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, JOYSTICKTEST_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				ShellExecuteA(0, "open", "arcade.exe", " joystickone", ".\\", SW_SHOW);
				break;	
			case 1 :
				ShellExecuteA(0, "open", "arcade.exe", " joysticktwo", ".\\", SW_SHOW);
				break;
			case 2 :
				ShellExecuteA(0, "open", "arcade.exe", " joystickthree", ".\\", SW_SHOW);
				break;
			case 3 :
				sp = "/c taskkill /f /im advmenu.exe && call ConfigurarControle.Exe && call advmenu.exe ";
				sp += "-";
				sp += KeyRegis;
				ShellExecute(0, "open", "cmd.exe", sp.c_str(), 0, SW_HIDE);
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}
//CONTADOR NA TELA
#define CONTADOR_CHOICE_DX 70*int_font_dx_get()
int run_contador_no_jogo(config_state& rs)
{
	choice_bag ch;
	ostringstream game_name,rom_name;
	string SetValor="",GetSessao="",rom="", GetTitle="",GetEmule;
	if (!rs.current_game) 
		return false;
		
	GetEmule = rs.current_game->emulator_get()->user_name_get().c_str();
	char Buffer[500], Buffer2[500];
	const game* base;
	base = rs.current_game;
	game_name << base->description_get();
	
	//if (rs.current_game)
		rom = rs.current_game->name_without_emulator_get();
		rom_name << rom;
	
	//Verifica se o sistema é de jogos de PC ou TAITO
	string GetValor= ".\\ini\\" + GetEmule + ".ini";
	GetPrivateProfileStringA("CONFIGURACAO","PCTAITO_GAMES","0", Buffer2, 500, GetValor.c_str()); 
	//Caso seja PC/TAITO - Buscar informação no Games.ini - Se não, buscar na pasta .//ini//nome_do_emulador.ini
	if (atoi(Buffer2) >= 1){
			GetTitle= "JOGO (" + game_name.str() + ")";
			SetValor= ".\\ini\\Games.ini";
			GetSessao = rom_name.str();
			GetPrivateProfileStringA(GetSessao.c_str(),"CONTADOR","0", Buffer, 500, SetValor.c_str()); 
	}else{
			GetTitle = "EMULADOR (" + GetEmule + ")";
			SetValor= ".\\ini\\" + GetEmule + ".ini";
			GetSessao = "CONFIGURACAO";
			GetPrivateProfileStringA(GetSessao.c_str(),"CONTADOR","0", Buffer, 500, SetValor.c_str()); 
	}
	

	int valor = atoi(Buffer);
			
			if (valor <= 0) {	
				ch.insert(ch.end(), choice("  MOSTRAR CONTADOR",false, 0));
				ch.insert(ch.end(), choice(">>OCULTAR CONTADOR",true, 1));
			}else{
				ch.insert(ch.end(), choice(">>MOSTRAR CONTADOR",true, 0));
				ch.insert(ch.end(), choice("  OCULTAR CONTADOR",false, 1));
			}
			

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run(" Nao funciona em todos emuladores - Desative no retroarch e Mame", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, CONTADOR_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA(GetSessao.c_str(),"CONTADOR","1",SetValor.c_str());
				break;	
			case 1 :
				WritePrivateProfileStringA(GetSessao.c_str(),"CONTADOR","0",SetValor.c_str());
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}

//MODO BLOQUEADO
#define COMERCIAL_CHOICE_DX 40*int_font_dx_get()

int run_modocomercial(config_state& rs)
{
	choice_bag ch;

		char PegaBloqueado[100];
		GetPrivateProfileStringA("CONFIGURACAO","BLOQUEADO", "0", PegaBloqueado, 100, ".\\advmenu.ini");
		int bloqueado=atoi(PegaBloqueado);
			
			if (bloqueado == 1) {	
				ch.insert(ch.end(), choice(">>SISTEMA PAGO",true, 0));
				ch.insert(ch.end(), choice("  SISTEMA GATUITO",false, 1));
			}else{
				ch.insert(ch.end(), choice("  SISTEMA PAGO",false, 0));
				ch.insert(ch.end(), choice(">>SISTEMA GATUITO",true, 1));
			}
			

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run(" MODO GRATIS/PAGO", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, COMERCIAL_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","BLOQUEADO","1",".\\advmenu.ini");
				break;	
			case 1 :
				WritePrivateProfileStringA("CONFIGURACAO","BLOQUEADO","0",".\\advmenu.ini");
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}

#define TEMA_CHOICE_DX 40*int_font_dx_get()

int run_selectheme(config_state& rs)
{
	choice_bag ch;
	
	//Posição do tema
	char buffertemaini[100];
	GetPrivateProfileStringA("CONFIGURACAO","TEMA","0", buffertemaini, 100, ".\\Temas.ini");	
			
	for (int i = 1; i < 256; i++){
		char infobuffer[100];
		ostringstream os,ss;
		string strc = "";
		bool TEMA_SELECIONADO = false;
		if (atoi(buffertemaini) == i){
			TEMA_SELECIONADO = true;
			strc = ">>";
		}	
		
		ss << "TEMA_" << i;
		string s = ss.str();
		GetPrivateProfileStringA("TEMAS_NOMES", s.c_str(), "-1", infobuffer, 100, ".\\Temas.ini");
		if (atoi(infobuffer) != -1){
			os << infobuffer;
			ch.insert(ch.end(), choice(strc + os.str(),TEMA_SELECIONADO, i));
		}

	}
	
	choice_bag::iterator i = ch.begin();
	
	int key = ch.run( "Temas", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, TEMA_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {

			if (i->value_get() > 0) {
				char folderbuffer[100];
				ostringstream ss1,ss2;
				ss1 << i->value_get();
				string s1 = ss1.str();
				
				ss2 << "TEMA_" << i->value_get();
				string s2 = ss2.str();
			
				GetPrivateProfileStringA("TEMAS_PASTA",s2.c_str() ,s1.c_str(), folderbuffer,   100, ".\\Temas.ini");
				
				key = run_settema(rs,folderbuffer);
				
				
				WritePrivateProfileStringA("CONFIGURACAO","TEMA", s1.c_str() ,".\\Temas.ini");
			}
		} 
		return true;
	} 

	return false;
}
//JOYTOKEY

#define JOYTOKEY_CHOICE_DX 70*int_font_dx_get()
int run_joytokey(config_state& rs)
{
	if (!rs.current_game) 
		return false;
	choice_bag ch;
	string GetEmule = "",path="";
	GetEmule = rs.current_game->emulator_get()->user_name_get();
	path = ".\\ini\\" + GetEmule + ".ini";
	
	
	std::ofstream outfile ((".\\auxiliar\\Joytokey\\" +GetEmule + ".cfg").c_str());
	string SetConf =  	"[General]\n"
						"FileVersion=51\n"
						"NumberOfJoysticks=2\n"
						"DisplayMode=2\n"
						"UseDiagonalInput=0\n"
						"UsePOV8Way=0\n"
						"Threshold=20\n"
						"Threshold2=20\n"
						"KeySendMode=0\n"
						"[Joystick 1]\n"
						"Axis1n=0\n"
						"Axis1p=0\n"
						"Axis2n=0\n"
						"Axis2p=0\n"
						"Axis3n=0\n"
						"Axis3p=0\n"
						"Axis4n=0\n"
						"Axis4p=0\n"
						"Axis5n=0\n"
						"Axis5p=0\n"
						"Axis6n=0\n"
						"Axis6p=0\n"
						"Axis7n=0\n"
						"Axis7p=0\n"
						"Axis8n=0\n"
						"Axis8p=0\n"
						"POV1-1=0\n"
						"POV1-2=0\n"
						"POV1-3=0\n"
						"POV1-4=0\n"
						"POV1-5=0\n"
						"POV1-6=0\n"
						"POV1-7=0\n"
						"POV1-8=0\n"
						"POV2-1=0\n"
						"POV2-2=0\n"
						"POV2-3=0\n"
						"POV2-4=0\n"
						"POV2-5=0\n"
						"POV2-6=0\n"
						"POV2-7=0\n"
						"POV2-8=0\n"
						"Up-Right=0\n"
						"Up- Left=0\n"
						"Dn- Left=0\n"
						"Dn-Right=0\n"
						"Up-Right2=0\n"
						"Up- Left2=0\n"
						"Dn- Left2=0\n"
						"Dn-Right2=0\n"
						"Button01=0\n"
						"Button02=0\n"
						"Button03=0\n"
						"Button04=0\n"
						"Button05=0\n"
						"Button06=0\n"
						"Button07=0\n"
						"Button08=0\n"
						"Button09=0\n"
						"Button10=0\n"
						"Button11=0\n"
						"Button12=0\n"
						"Button13=0\n"
						"Button14=0\n"
						"Button15=0\n"
						"Button16=0\n"
						"Button17=0\n"
						"Button18=0\n"
						"Button19=0\n"
						"Button20=0\n"
						"Button21=0\n"
						"Button22=0\n"
						"Button23=0\n"
						"Button24=0\n"
						"Button25=0\n"
						"Button26=0\n"
						"Button27=0\n"
						"Button28=0\n"
						"Button29=0\n"
						"Button30=0\n"
						"Button31=0\n"
						"Button32=0\n"
						"[Joystick 2]\n"
						"Axis1n=0\n"
						"Axis1p=0\n"
						"Axis2n=0\n"
						"Axis2p=0\n"
						"Axis3n=0\n"
						"Axis3p=0\n"
						"Axis4n=0\n"
						"Axis4p=0\n"
						"Axis5n=0\n"
						"Axis5p=0\n"
						"Axis6n=0\n"
						"Axis6p=0\n"
						"Axis7n=0\n"
						"Axis7p=0\n"
						"Axis8n=0\n"
						"Axis8p=0\n"
						"POV1-1=0\n"
						"POV1-2=0\n"
						"POV1-3=0\n"
						"POV1-4=0\n"
						"POV1-5=0\n"
						"POV1-6=0\n"
						"POV1-7=0\n"
						"POV1-8=0\n"
						"POV2-1=0\n"
						"POV2-2=0\n"
						"POV2-3=0\n"
						"POV2-4=0\n"
						"POV2-5=0\n"
						"POV2-6=0\n"
						"POV2-7=0\n"
						"POV2-8=0\n"
						"Up-Right=0\n"
						"Up- Left=0\n"
						"Dn- Left=0\n"
						"Dn-Right=0\n"
						"Up-Right2=0\n"
						"Up- Left2=0\n"
						"Dn- Left2=0\n"
						"Dn-Right2=0\n"
						"Button01=0\n"
						"Button02=0\n"
						"Button03=0\n"
						"Button04=0\n"
						"Button05=0\n"
						"Button06=0\n"
						"Button07=0\n"
						"Button08=0\n"
						"Button09=0\n"
						"Button10=0\n"
						"Button11=0\n"
						"Button12=0\n"
						"Button13=0\n"
						"Button14=0\n"
						"Button15=0\n"
						"Button16=0\n"
						"Button17=0\n"
						"Button18=0\n"
						"Button19=0\n"
						"Button20=0\n"
						"Button21=0\n"
						"Button22=0\n"
						"Button23=0\n"
						"Button24=0\n"
						"Button25=0\n"
						"Button26=0\n"
						"Button27=0\n"
						"Button28=0\n"
						"Button29=0\n"
						"Button30=0\n"
						"Button31=0\n"
						"Button32=0\n";

		char PegaContador[100];
		GetPrivateProfileStringA("configuracao","JOYTOKEY","0", PegaContador, 100, path.c_str() );
		int ContadorNaTela=atoi(PegaContador);
			
			if (ContadorNaTela == 1) {	
				ch.insert(ch.end(), choice(">>INICIAR JOYTOKEY COM O JOGO/SISTEMA SELECIONADO",true, 0));
				ch.insert(ch.end(), choice("  DESATIVAR JOYTOKEY COM O JOGO/SISTEMA SELECIONADO",false, 1));
			}else{
				ch.insert(ch.end(), choice("  INICIAR JOYTOKEY COM O JOGO/SISTEMA SELECIONADO",false, 0));
				ch.insert(ch.end(), choice(">>DESATIVAR JOYTOKEY COM O JOGO/SISTEMA SELECIONADO",true, 1));
			}
			

	choice_bag::iterator i = ch.begin();
	
	int key = ch.run(" Iniciar Joytokey ao executar o emulador ou o jogo selecionado", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, JOYTOKEY_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","JOYTOKEY","1",path.c_str());
				outfile << SetConf.c_str() << std::endl;
				outfile.close();
				break;	
			case 1 :
				WritePrivateProfileStringA("CONFIGURACAO","JOYTOKEY","0",path.c_str());
				break;
			default:
				break;
			}
		} 
		outfile.close();
		return true;
	} 
	outfile.close();
	return false;
}


//TEMA 1 ARQUIVO 
int run_settema(config_state& rs, const std::string& sr)
{
	string s;
	string p;
	string sp;

	
	for (size_t i = 0; i < ListaEmuladores.size(); ++i) {

					p = ListaEmuladores[i];
					s = "/c COPY ";
					s += sr; 
					
					string FileExist = sr + "\\" + p + ".amp";
					ifstream f(FileExist.c_str());
					if(f.good()){
						s += "\\" + p + ".amp";
					}else{
						s += "\\skin.amp";
					}
					
					s += " .\\Layouts\\";
					s += p ;
					s += ".amp /y";
					
					
					//MessageBox (NULL,s.c_str() ,"", 1);
					ShellExecute(0, "open", "cmd.exe", s.c_str(), 0, SW_HIDE);
	
				}
				rs.restore_save();
				
				sp =  "/c taskkill /f /im advmenu.exe && TIMEOUT 2 && CALL advmenu.exe ";
				sp += "-";
				sp += KeyRegis;
				
				ShellExecute(0, "open", "cmd.exe", sp.c_str(), 0, SW_HIDE);
				
				
return false;
}

//FICHAS
#define TEMPFICHA_CHOICE_DX 40*int_font_dx_get()

int run_tempofichas(config_state& rs)
{
	choice_bag ch;
	
		char Buffer[100];
		GetPrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","0", Buffer, 100, ".\\advmenu.ini"); 
		
		if (atoi(Buffer) == 300000){
			ch.insert(ch.end(), choice(">> 05 MINUTOS",true, 0));
		}else{
			ch.insert(ch.end(), choice("05 MINUTOS",false, 0));
		}
		if (atoi(Buffer) == 600000){
			ch.insert(ch.end(), choice(">> 10 MINUTOS",true, 1));
		}else{
			ch.insert(ch.end(), choice("10 MINUTOS",false, 1));
		}
		if (atoi(Buffer) == 900000){
			ch.insert(ch.end(), choice(">> 15 MINUTOS",true, 2));
		}else{
			ch.insert(ch.end(), choice("15 MINUTOS",false, 2));
		}
		if (atoi(Buffer) == 1200000){
			ch.insert(ch.end(), choice(">> 20 MINUTOS",true, 3));
		}else{
			ch.insert(ch.end(), choice("20 MINUTOSS",false, 3));
		}
		if (atoi(Buffer) == 1500000){
			ch.insert(ch.end(), choice(">> 25 MINUTOS",true, 4));
		}else{
			ch.insert(ch.end(), choice("25 MINUTOS",false, 4));
		}
		if (atoi(Buffer) == 1800000){
			ch.insert(ch.end(), choice(">> 30 MINUTOS",true, 5));
		}else{
			ch.insert(ch.end(), choice("30 MINUTOS",false, 5));
		}
		if (atoi(Buffer) == 2400000){
			ch.insert(ch.end(), choice(">> 40 MINUTOS",true, 6));
		}else{
			ch.insert(ch.end(), choice("40 MINUTOS",false, 6));
		}
		if (atoi(Buffer) == 3600000){
			ch.insert(ch.end(), choice(">> 01 HORA",true, 7));
		}else{
			ch.insert(ch.end(), choice("01 HORA",false, 7));
		}
		if (atoi(Buffer) == 7200000){		
			ch.insert(ch.end(), choice(">> 02 HORAS",true, 8));
		}else{
			ch.insert(ch.end(), choice("02 HORAS",false, 8));
		}
		if (atoi(Buffer) == 10800000){		
			ch.insert(ch.end(), choice(">> 03 HORAS",true, 9));
		}else{
			ch.insert(ch.end(), choice("03 HORAS",false, 9));
		}
			

	choice_bag::iterator i = ch.begin();
	
	
	int key = ch.run(" Tempo por fichas", SECOND_CHOICE_X + 240, SECOND_CHOICE_Y, TEMPFICHA_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		if (i->value_get()>=-1 && i->value_get()<=256) {
			switch (i->value_get()) {
			case 0 : 
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","300000",".\\advmenu.ini");
				ValorDaFicha=300000;
				break;	
			case 1 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","600000",".\\advmenu.ini");
				ValorDaFicha=600000;
				break;
			case 2 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","900000",".\\advmenu.ini");
				ValorDaFicha=900000;
				break;
			case 3 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","1200000",".\\advmenu.ini");
				ValorDaFicha=1200000;
				break;
			case 4 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","1500000",".\\advmenu.ini");
				ValorDaFicha=1500000;
				break;
			case 5 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","1800000",".\\advmenu.ini");
				ValorDaFicha=1800000;
				break;
			case 6 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","2400000",".\\advmenu.ini");
				ValorDaFicha=2400000;
				break;
			case 7 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","3600000",".\\advmenu.ini");
				ValorDaFicha=3600000;
				break;
			case 8 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","7200000",".\\advmenu.ini");
				ValorDaFicha=7200000;
				break;
			case 9 :
				WritePrivateProfileStringA("CONFIGURACAO","TEMPOPORFICHA","10800000",".\\advmenu.ini");
				ValorDaFicha=10800000;
				break;
			default:
				break;
			}
		} 

		return true;
	} 

	return false;
}


// ------------------------------------------------------------------------
// Command menu

#define COMMAND_CHOICE_DX 40*int_font_dx_get()

int run_command(config_state& rs)
{
	choice_bag ch;
	
	//MessageBox(NULL,"1","",MB_OK|MB_SYSTEMMODAL);
	run_background(rs);

	for(script_container::iterator i=rs.script_bag.begin();i!=rs.script_bag.end();++i) {
		if (i->text.find("%s") != string::npos) {
			if (!rs.current_game)
				continue;
		}
		if (i->text.find("%p") != string::npos || i->text.find("%f") != string::npos) {
			if (!rs.current_game)
				continue;
			if (rs.current_game->rom_zip_set_get().size() == 0)
				continue;
		}
		ch.insert(ch.end(), choice(i->name, &*i));
	}

	char Buffer[100];
	GetPrivateProfileStringA("CONFIGURACAO","NUMERAR_LISTA","0", Buffer, 100, ".\\advmenu.ini"); 
	NumerarLista = atoi(Buffer);
	
  
	
	ch.insert(ch.end(), choice("Controles (Configurar/Testar)...", 1));
	ch.insert(ch.end(), choice("Modo GRATIS/PAGO...", 2));
	ch.insert(ch.end(), choice("Tipo de emulador FICHA/TEMPO...", 3));
	ch.insert(ch.end(), choice("Remover Emuladores...", 4));
	ch.insert(ch.end(), choice("Creditos por fichas...", 5));
	ch.insert(ch.end(), choice("Tempo por fichas...", 6));
	ch.insert(ch.end(), choice("Auto envio de fichas...", 7));
	ch.insert(ch.end(), choice("Contador na tela do jogo...", 16));
	ch.insert(ch.end(), choice("SAIR OCIOSO...", 8));
	ch.insert(ch.end(), choice("TEMA...", 9));
	ch.insert(ch.end(), choice("Tela de demonstracao (FADE)...", 18));
	ch.insert(ch.end(), choice("INICIAR JOYTOKEY COM O SISTEMA...", 17));
	ch.insert(ch.end(), choice("REINICIALIZACAO...", 10));
	ch.insert(ch.end(), choice("VOLUME...", 15));
	ch.insert(ch.end(), choice("Zerar creditos", 11));
	ch.insert(ch.end(), choice("Zerar contador geral", 12));
	ch.insert(ch.end(), choice((rs.menu_systems_activated ? "Desativar Menu de Sistemas" : "Ativar Menu de Sistemas"), 13));
	
	if (NumerarLista == 0){
		ch.insert(ch.end(), choice("Numerar Lista", 14));
	}else{
		ch.insert(ch.end(), choice("Remover Numeracao", 14));
	}

	ch.insert(ch.end(), choice("Sair", -1));



	if (ch.begin() == ch.end())
		ch.insert(ch.end(), choice("Nenhum Comando adicionado", -1));

	choice_bag::iterator i = ch.begin();
 
	bool done = false; 
	bool leave = false;
	int key;
	
	void* save = int_save();
	int_restore(save);
	
	do {
		void* save = int_save();
		//key = ch.run(string(" Arcade 2019 feito por PedroWarlock") + rs.script_menu, SECOND_CHOICE_X, SECOND_CHOICE_Y, COMMAND_CHOICE_DX, i);
		key = ch.run(string(" Arcade 2019 feito por PedroWarlock"), SECOND_CHOICE_X, SECOND_CHOICE_Y, COMMAND_CHOICE_DX, i);
	
		if (key == EVENT_ENTER) {
			int r;
			if (i->value_get()>=-1 && i->value_get()<=256) {
				switch (i->value_get()) {
				
				case 1 :
					r = 0;
					done = run_testjoy(rs);
					break;
				
				case 2 :
					r = 0;
					done = run_modocomercial(rs);
					break;
					
				case 3 :
					done = run_topoemu(rs);
					r = 0;
					break;	
					
				case 4 :
					r = 0;
					do {
						leave = remover_emu(rs);
						} while (!leave);
					ItemSelecionado = 0;
					break;	
					
				case 5 :
					r = 0;
					done = run_creditfichas(rs);
					break;	
				case 6 :
					r = 0;
					done = run_tempofichas(rs);
					break;	
					
				case 7 :
					r = 0;
					done = run_autoenvio(rs);
					break;
					
				case 8 :
					r = 0;
					done = run_sairocioso(rs);
					break;
					
				case 9 :
					r = 0;
					done = run_selectheme(rs);
					break;
				
				case 10 :
					r = 0;
					run_reboot(rs);
					break;
						
				case 11 : 
					WritePrivateProfileStringA("FICHEIRO","FICHAS","0",".\\advmenu.ini");	
					play_foreground_fliper_key(rs.sound_foreground_zerado);	
					done = true;
					r = 0;
					break;	
					
				case 12 :
					WritePrivateProfileStringA("FICHEIRO","CONTADOR","0",".\\advmenu.ini");		
					play_foreground_fliper_key(rs.sound_foreground_zgeral);
					done = true;
					r = 0;
					break;
				
				case 13 : 
						rs.menu_systems_activated = !rs.menu_systems_activated;
						// Desactivar MenuSystems: mostrado y emu seleccionado
						if(!rs.menu_systems_activated && rs.menu_systems->state_get() && rs.current_game != 0) {
							emulator_container c;
							for(pemulator_container::const_iterator i=rs.emu_active.begin();i!=rs.emu_active.end();) {
								if(emu_tolower((*i)->user_name_get()) == rs.current_game->name_without_emulator_get()) {
									c.insert(c.end(), (*i)->user_name_get());
									done = true;
									r = 0;
									break;
								} else if(++i == rs.emu_active.end()) {
									// emu seleccionado no esta entre los activos -> carga el primero
									c.insert(c.end(), (*rs.emu_active.begin())->user_name_get());
								}
							}
							rs.include_emu_set(c);
						}
						// Activar MenuSystems: no mostrado
						if(rs.menu_systems_activated && !rs.menu_systems->state_get()) {
							emulator_container c;
							c.insert(c.end(), rs.menu_systems->user_name_get());
							rs.include_emu_set(c);
						}
					done = true;
					r = 0;
					break;
					
				case 14 :
					if (NumerarLista == 0){
						WritePrivateProfileStringA("CONFIGURACAO","NUMERAR_LISTA","1",".\\advmenu.ini");
					}else{
						WritePrivateProfileStringA("CONFIGURACAO","NUMERAR_LISTA","0",".\\advmenu.ini");
					}
					done = true;
					r = 0;
					break;	
					
				case 15 :
					r = 0;
					run_volume(rs);
					break;
				case 16 :
					r = 0;
					done = run_contador_no_jogo(rs);
					break;
				case 17 :
					r = 0;
					done = run_joytokey(rs);
					break;
				case 18 :
					r = 0;
					done = run_delayintro(rs);
					break;
					
				case -1 :
					r = 0;
					done = true;
					break;
				default:
					r = -1;
					//done = true;
					break;
				}
			} else {
				script* s = static_cast<script*>(i->ptr_get());

				string text = s->text;

				if (rs.current_game) {
					text = subs(text, "%s", rs.current_game->name_without_emulator_get());
					if (rs.current_game->rom_zip_set_get().size()) {
						string path = *rs.current_game->rom_zip_set_get().begin();
						text = subs(text, "%p", path_export(path));
						text = subs(text, "%f", path_export(file_file(path)));
					}
				}

				int_unplug();

				r = target_script(text.c_str());

				int_plug();
			}

			if (r != 0) {
				choice_bag ch;
				ch.insert(ch.end(), choice(rs.script_error, 0));
				choice_bag::iterator i = ch.begin();
				ch.run(" Erro", MSG_CHOICE_X, MSG_CHOICE_Y, MSG_CHOICE_DX, i);
			}
		
		} else if (key == EVENT_ESC) {
				done = true;
		} else if (key == EVENT_ESCEMULE) {
				done = true;
		}
		
			int_restore(save);
		
	} while (!done);

										
	return key;
}

// ------------------------------------------------------------------------
// Mode menu

#define MODE_CHOICE_DX 25*int_font_dx_get()

int run_mode(config_state& rs)
{
	choice_bag ch;
	ch.insert(ch.end(), choice("Tema Personalizado", mode_custom));
	ch.insert(ch.end(), choice("Completo", mode_full));
	ch.insert(ch.end(), choice("Misto completo", mode_full_mixed));
	ch.insert(ch.end(), choice("Texto", mode_text));
	ch.insert(ch.end(), choice("Lista", mode_list));
	ch.insert(ch.end(), choice("Lista misturada", mode_list_mixed));
	ch.insert(ch.end(), choice("Modo muito pequeno", mode_tile_tiny));
	ch.insert(ch.end(), choice("Modo pequeno", mode_tile_small));
	ch.insert(ch.end(), choice("Modo normal", mode_tile_normal));
	ch.insert(ch.end(), choice("Modo grande", mode_tile_big));
	ch.insert(ch.end(), choice("Modo enorme", mode_tile_enormous));
	ch.insert(ch.end(), choice("Modo gigante", mode_tile_giant));
	ch.insert(ch.end(), choice("Icones", mode_tile_icon));
	ch.insert(ch.end(), choice("Marquee", mode_tile_marquee));


	choice_bag::iterator i = ch.find_by_value(rs.mode_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Selecione o modo de lista", THIRD_CHOICE_X, THIRD_CHOICE_Y, MODE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.mode_set((listmode_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Preview menu

#define PREVIEW_CHOICE_DX 33*int_font_dx_get()

int run_preview(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Snap", preview_snap));
	ch.insert(ch.end(), choice("Titulo", preview_title));
	ch.insert(ch.end(), choice("Folheto", preview_flyer));
	ch.insert(ch.end(), choice("Gabinete", preview_cabinet));
	ch.insert(ch.end(), choice("Icone", preview_icon));
	ch.insert(ch.end(), choice("Marquee", preview_marquee));

	choice_bag::iterator i = ch.find_by_value(rs.preview_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Selecione o modo de visualizacao", THIRD_CHOICE_X, THIRD_CHOICE_Y, PREVIEW_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.preview_set((listpreview_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Favorites menu

#define FAVORITES_CHOICE_DX 20*int_font_dx_get()

int run_favorites(config_state& rs)
{
	choice_bag ch;

	for(pfavorites_container::const_iterator j = rs.favorites.begin();j!=rs.favorites.end();++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.include_favorites_get());
	if (i==ch.end())
		i = ch.begin();

	int key = ch.run(" Carregar Lista", THIRD_CHOICE_X, THIRD_CHOICE_Y, FAVORITES_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		string f = i->desc_get();
		rs.include_favorites_set(f);
	}

	return key;
}

void run_favorites_next(config_state& rs)
{
	bool with_or_without_games = false; // indica si carga la siguiente lista tenga o no juegos
	
	// si Menu Systems activado -> carga la lista siguiente tenga o no juegos
	// si Menu Systems desactivado -> carga la lista siguiente con juegos
	with_or_without_games = rs.menu_systems_activated;
	
	string next_select = "";
	bool pred_in = false;

	for(pfavorites_container::const_iterator j=rs.favorites.begin();j!=rs.favorites.end();++j) {
		if (pred_in) {
			for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
				if (with_or_without_games || (*j)->has_emu(*k)) {
					next_select = (*j)->name_get();
					pred_in = false;
					break;
				}
			}
		}
		if ((*j)->name_get() == rs.include_favorites_get())
			pred_in = true;
	}
	
	if (next_select == "" && rs.favorites.begin() != rs.favorites.end())
		next_select = (*rs.favorites.begin())->name_get(); // lista "All Games"

	string f;

	if (next_select != "")
		f = next_select;

	rs.include_favorites_set(f);
}

int run_favorites_move(config_state& rs)
{
	string game = rs.current_game->name_get();

	choice_bag ch;

	for(pfavorites_container::const_iterator j=rs.favorites.begin();j!=rs.favorites.end();++j) {
		bool tag = false;
		if ((*j)->name_get() != "All Games") {
			if((*j)->has_game(game))
				tag = true;
			
			ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
		}
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.favorites_run(" Add/Remover Jogo a lista", THIRD_CHOICE_X, THIRD_CHOICE_Y, FAVORITES_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			for(pfavorites_container::const_iterator k=rs.favorites.begin();k!=rs.favorites.end();++k) {
				if((*k)->name_get() == j->desc_get()) {
					(*k)->game_move(game, j->bistate_get());
					break;
				}
			}
		}
	}

	return key;
}

// ------------------------------------------------------------------------
// Emu menu

#define EMU_CHOICE_DX 20*int_font_dx_get()

int run_emu(config_state& rs)
{
	choice_bag ch;

	for(pemulator_container::const_iterator j = rs.emu_active.begin();j!=rs.emu_active.end();++j) {
		bool tag = false;
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				tag = true;
				break;
			}
		}
		ch.insert(ch.end(), choice((*j)->user_name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(" Emuladores incluidos", SECOND_CHOICE_X, SECOND_CHOICE_Y, EMU_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		emulator_container c;
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			if (j->bistate_get())
				c.insert(c.end(), j->desc_get());
		}
		rs.include_emu_set(c);
	}

	return key;
}

emulator* run_emu_select(config_state& rs)
{
	choice_bag ch;

	for(emulator_container::const_iterator j = rs.include_emu_get().begin();j!=rs.include_emu_get().end();++j) {
		ch.insert(ch.end(), choice(*j, 0));
	}

	string emu;

	if (ch.size() > 1) {
		choice_bag::iterator i = ch.begin();
		int key = ch.run(" Selecionar Emulador", THIRD_CHOICE_X, THIRD_CHOICE_Y, EMU_CHOICE_DX, i);

		if (key != EVENT_ENTER)
			return 0;

		emu = i->desc_get();
	} else {
		emu = ch.begin()->desc_get();
	}

	if (rs.menu_systems->user_name_get() == emu)
		return rs.menu_systems;
	
	for(pemulator_container::const_iterator j = rs.emu_active.begin();j!=rs.emu_active.end();++j) {
		if ((*j)->user_name_get() == emu) {
			return *j;
		}
	}

	return 0;
}

void run_emu_next(config_state& rs)
{
	string next_select = "";
	bool pred_in = false;

	// Obtiene la lista de favoritos actual
	pfavorites_container::const_iterator i;
	for(i=rs.favorites.begin();i!=rs.favorites.end();++i)
		if((*i)->name_get() == rs.include_favorites_get())
			break;

	// Obtiene la coleccion de emuladores validos (con juegos para la lista de favoritos actual)
	pemulator_container emu_valid;
	for(pemulator_container::const_iterator j=rs.emu_active.begin();j!=rs.emu_active.end();++j)
		if((*i)->has_emu((*j)->user_name_get()))
			emu_valid.insert(emu_valid.end(), *j);

	// Recorre los emuladores validos para obtener el emulador siguiente
	for(pemulator_container::const_iterator j=emu_valid.begin();j!=emu_valid.end();++j) {
		if (pred_in)
			next_select = (*j)->user_name_get();
		pred_in = false;
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				pred_in = true;
				break;
			}
		}
	}
	if (next_select.length() == 0 && emu_valid.begin() != emu_valid.end())
		next_select = (*emu_valid.begin())->user_name_get();

	emulator_container c;

	if (next_select.length() != 0)
		c.insert(c.end(), next_select);

	rs.include_emu_set(c);	
}

void run_emu_pre(config_state& rs)
{
	string pre_select = "";
	string pre_emu = "";

	// Obtiene la lista de favoritos actual
	pfavorites_container::const_iterator i;
	for(i=rs.favorites.begin();i!=rs.favorites.end();++i)
		if((*i)->name_get() == rs.include_favorites_get())
			break;

	// Obtiene la coleccion de emuladores validos (con juegos para la lista de favoritos actual
	pemulator_container emu_valid;
	for(pemulator_container::const_iterator j=rs.emu_active.begin();j!=rs.emu_active.end();++j)
		if((*i)->has_emu((*j)->user_name_get()))
			emu_valid.insert(emu_valid.end(), *j);

	// Recorre los emuladores validos para obtener el emulador previo
	for(pemulator_container::const_iterator j=emu_valid.begin();j!=emu_valid.end();++j) {
		for(emulator_container::const_iterator k = rs.include_emu_get().begin();k!=rs.include_emu_get().end();++k) {
			if ((*j)->user_name_get() == *k) {
				pre_select = pre_emu;
				break;
			}
		}
		pre_emu = (*j)->user_name_get();
	}
	if (pre_select.length() == 0 && emu_valid.begin() != emu_valid.end())
		pre_select = pre_emu;

	emulator_container c;

	if (pre_select.length() != 0)
		c.insert(c.end(), pre_select);

	rs.include_emu_set(c);
}

// ------------------------------------------------------------------------
// Type menu

#define TYPE_CHOICE_DX 30*int_font_dx_get()

int run_type(config_state& rs)
{
	choice_bag ch;

	for(pcategory_container::const_iterator j = rs.type.begin();j!=rs.type.end();++j) {
		bool tag = rs.include_type_get().find((*j)->name_get()) != rs.include_type_get().end();
		ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(" Incluir Tipos", THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		category_container c;
		for(choice_bag::const_iterator j=ch.begin();j!=ch.end();++j) {
			if (j->bistate_get())
				c.insert(j->desc_get());
		}
		rs.include_type_set(c);
	}

	return key;
}

void run_type_next(config_state& rs)
{
	category* next_select = 0;
	bool all_select = true;

	bool pred_in = false;
	for(pcategory_container::const_iterator j=rs.type.begin();j!=rs.type.end();++j) {
		if (pred_in)
			next_select = *j;
		pred_in = false;
		for(category_container::const_iterator k = rs.include_type_get().begin();k!=rs.include_type_get().end();++k) {
			if ((*j)->name_get() == *k) {
				pred_in = true;
				break;
			}
		}
		if (!pred_in)
			all_select = false;
	}

	category_container c;
	if (!all_select && next_select == 0) {
		// insert all
		for(pcategory_container::const_iterator j=rs.type.begin();j!=rs.type.end();++j) {
			c.insert((*j)->name_get());
		}
	} else {
		if ((all_select || next_select == 0) && rs.type.begin() != rs.type.end())
			next_select = *rs.type.begin();
		if (next_select != 0) {
			// insert the next
			c.insert(next_select->name_get());
		}
	}
	rs.include_type_set(c);
}


// ------------------------------------------------------------------------
// Move menu

int run_type_move(config_state& rs)
{
	choice_bag ch;

	if (!rs.current_game)
		return EVENT_ENTER;

	for(pcategory_container::const_iterator j = rs.type.begin();j!=rs.type.end();++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.current_game->type_get()->name_get());
	if (i==ch.end())
		i = ch.begin();

	int key = ch.run(" Selecione o tipo de jogo", THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.current_game->user_type_set(rs.type.insert(i->desc_get()));
	}

	return key;
}

// ------------------------------------------------------------------------
// Clone menu

#define CLONE_CHOICE_X int_dx_get()/10
#define CLONE_CHOICE_Y int_dy_get()/5
#define CLONE_CHOICE_DX int_dx_get()*5/5

void run_clone(config_state& rs)
{
	choice_bag ch;
	choice_bag::iterator i;
	const game* base;

	rs.current_clone = 0;
	if (!rs.current_game)
		return;

	if (rs.current_game->software_get()) {
		base = &rs.current_game->root_get();
	} else {
		base = rs.current_game;
	}

	ostringstream s;
	s << base->description_get() << ", " << base->manufacturer_get() << ", " << base->year_get();
	ch.insert(ch.end(), choice(s.str(), (void*)base));

	for(pgame_container::const_iterator j = base->clone_bag_get().begin();j!=base->clone_bag_get().end();++j) {
		if (!(*j)->software_get() || (*j)->emulator_get()->type_get() == "mess_mess") {
			ostringstream s;

			s << (*j)->description_get() << ", " << (*j)->manufacturer_get() << ", " << (*j)->year_get();

			switch ((*j)->play_get()) {
			case play_imperfect : s << " [imperfect]"; break;
			case play_preliminary : s << " [preliminary]"; break;
			default: break;
			}

			ch.insert(ch.end(), choice(s.str(), (void*)&**j, (*j)->present_tree_get()));
		}
	}

	i = ch.begin();
	int key = ch.run(" Selecione o clone do jogo", CLONE_CHOICE_X, CLONE_CHOICE_Y, CLONE_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		rs.current_clone = (game*)i->ptr_get();
	}
}

// ------------------------------------------------------------------------
// Calib menu

#define CALIB_CHOICE_DX 30*int_font_dx_get()
#define CALIB_CHOICE_X (int_dx_get()-CALIB_CHOICE_DX)/2
#define CALIB_CHOICE_Y int_dy_get()/2

void run_calib(config_state& rs)
{
	const char* message = 0;
	bool at_least_one = false;

	joystickb_calib_start();

	while (1) {
		const char* ope = joystickb_calib_next();

		if (!ope) {
			if (!at_least_one)
				message = "Calibracao nao necessaria";
			break;
		}

		choice_bag ch;
		ch.insert(ch.end(), choice(ope, 0));

		choice_bag::iterator i = ch.begin();
		int key = ch.run(" Calibracao de controle", CALIB_CHOICE_X, CALIB_CHOICE_Y, CALIB_CHOICE_DX, i);

		if (key != EVENT_ENTER) {
			message = 0;
			break;
		}

		at_least_one = true;
	}

	if (message) {
		choice_bag ch;
		ch.insert(ch.end(), choice(message, 0));
		choice_bag::iterator i = ch.begin();
		ch.run(" Calibracao de controle", CALIB_CHOICE_X, CALIB_CHOICE_Y, CALIB_CHOICE_DX, i);
	}
}

// ------------------------------------------------------------------------
// Volume

#define VOLUME_CHOICE_DX 20*int_font_dx_get()

int run_volume(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Maximo", 0));
	ch.insert(ch.end(), choice("90%", -2));
	ch.insert(ch.end(), choice("80%", -4));
	ch.insert(ch.end(), choice("70%", -6));
	ch.insert(ch.end(), choice("60%", -8));
	ch.insert(ch.end(), choice("50%", -10));
	ch.insert(ch.end(), choice("40%", -12));
	ch.insert(ch.end(), choice("30%", -14));
	ch.insert(ch.end(), choice("20%", -16));
	ch.insert(ch.end(), choice("10%", -20));
	ch.insert(ch.end(), choice("Mudo", -40));

	choice_bag::iterator i = ch.find_by_value(play_attenuation_get());
	if (i == ch.end())
		i = ch.find_by_value(play_attenuation_get() - 1); // if the value is odd
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Volume", SECOND_CHOICE_X+ 240, SECOND_CHOICE_Y, VOLUME_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.restore_save();
		play_attenuation_set(i->value_get());
		play_mute_set(false);
	}

	return key;
}

// ------------------------------------------------------------------------
// Difficulty

#define DIFFICULTY_CHOICE_DX 13*int_font_dx_get()

int run_difficulty(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Padrao", difficulty_none));
	ch.insert(ch.end(), choice("Muito Facil", difficulty_easiest));
	ch.insert(ch.end(), choice("Facil", difficulty_easy));
	ch.insert(ch.end(), choice("Normal", difficulty_medium));
	ch.insert(ch.end(), choice("Dificil", difficulty_hard));
	ch.insert(ch.end(), choice("Muito Dificil", difficulty_hardest));

	choice_bag::iterator i = ch.find_by_value(rs.difficulty_effective);
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Dificuldade", SECOND_CHOICE_X, SECOND_CHOICE_Y, DIFFICULTY_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.difficulty_effective = static_cast<difficulty_t>(i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// RomSet Type

#define MERGE_CHOICE_DX 15*int_font_dx_get()

int run_merge(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Nao mesclado", merge_no));
	ch.insert(ch.end(), choice("Split", merge_differential));
	ch.insert(ch.end(), choice("Mesclado", merge_parent));
	ch.insert(ch.end(), choice("Todos", merge_any));
	ch.insert(ch.end(), choice("Desativado", merge_disable));
	
	choice_bag::iterator i = ch.find_by_value(rs.merge_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Tipo de Roms", SECOND_CHOICE_X, SECOND_CHOICE_Y, MERGE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.merge_set((merge_t)i->value_get());
		
		for(game_set::const_iterator i=rs.gar.begin();i!=rs.gar.end();++i) {
			if (i->emulator_get()->state_get())
				i->flag_set(rs.gar.is_tree_rom_of_present(i->name_get(), rs.merge_get()), game::flag_tree_present);
		}
	}

	return key;
}

// ------------------------------------------------------------------------
// XML MAME...

#define XMLMAME_CHOICE_DX 20*int_font_dx_get()

int run_xmlmame(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Limpar XML Original", 0));
	ch.insert(ch.end(), choice("Carregar XML Original", 1));
	
	choice_bag::iterator i = ch.begin();

	int key = ch.run(" Gerenciar XML M.A.M.E.", THIRD_CHOICE_X, THIRD_CHOICE_Y, XMLMAME_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		
		emulator* emu = 0;
		
		if (rs.menu_systems->state_get()) {
			for(pemulator_container::iterator e = rs.emu.begin();e!=rs.emu.end();e++) {
				if (emu_tolower((*e)->user_name_get()) == rs.current_game->name_without_emulator_get()) {
					emu = *e;
					break;
				}
			}
		} else {
			for(pemulator_container::iterator e = rs.emu.begin();e!=rs.emu.end();e++) {
				if ((*e)->state_get()) {
					emu = *e;
					break;
				}
			}
		}

		if (emu) {
			switch (i->value_get()) {
				case 0 :
					emu->clean_xml(rs.xml_dir, rs.gar);
					break;
				case 1 :
					emu->update_xml(rs.xml_dir);
					target_flush();
					break;
			}
		}

		emu = 0;
	}

	return key;
}

// ------------------------------------------------------------------------
// Sub Menu

#define MENU_CHOICE_DX 30*int_font_dx_get()

int run_suballmenu(config_state& rs)
{
	choice_bag ch;

	rs.sub_disable(); // force the use of the default config
	if (rs.type.size() > 1)
		ch.insert(ch.end(), choice(menu_name(rs, "Tipo de jogo...", EVENT_SETTYPE), 8, rs.current_game != 0));

	// MENU SYSTEMS
	ch.insert(ch.end(), choice((rs.menu_systems_activated ? "Desativar Menu de Sistemas" : "Ativar Menu de Sistemas"), 14));
	
	ch.insert(ch.end(), choice("Calibracao...", 9));
	ch.insert(ch.end(), choice("Salvar todas configuracoes", 6));
	ch.insert(ch.end(), choice("Restaurar configuracoes", 20));
	ch.insert(ch.end(), choice(menu_name(rs, "Travar configuracoes", EVENT_LOCK), 11));
	ch.insert(ch.end(), choice(rs.security_exit ? "Desativar alerta de saida" : "Ativar alerta de saida", 12));
	ch.insert(ch.end(), choice("Gerenciar XML M.A.M.E....", 13));

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(" Configuracoes", SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_sort(rs);
				break;
			case 1 :
				key = run_mode(rs);
				break;
			case 2 :
				key = run_preview(rs);
				break;
			case 3 :
				key = run_type(rs);
				break;
			case 4 :
				key = run_favorites(rs);
				break;
			case 6 :
				rs.restore_save();
				break;
			case 20 :
				rs.restore_load();
				break;
			case 8 :
				key = run_type_move(rs);
				break;
			case 9 :
				run_calib(rs);
				break;
			case 11 :
				rs.lock_effective = !rs.lock_effective;
				break;
			case 12 :
				rs.security_exit = !rs.security_exit;
				break;
			case 13 :
				key = run_xmlmame(rs);
				break;
			case 14 : // MENU SYSTEMS
				rs.menu_systems_activated = !rs.menu_systems_activated;
				// Desactivar MenuSystems: mostrado y emu seleccionado
				if(!rs.menu_systems_activated && rs.menu_systems->state_get() && rs.current_game != 0) {
					emulator_container c;
					for(pemulator_container::const_iterator i=rs.emu_active.begin();i!=rs.emu_active.end();) {
						if(emu_tolower((*i)->user_name_get()) == rs.current_game->name_without_emulator_get()) {
							c.insert(c.end(), (*i)->user_name_get());
							break;
						} else if(++i == rs.emu_active.end()) {
							// emu seleccionado no esta entre los activos -> carga el primero
							c.insert(c.end(), (*rs.emu_active.begin())->user_name_get());
						}
					}
					rs.include_emu_set(c);
				}
				// Activar MenuSystems: no mostrado
				if(rs.menu_systems_activated && !rs.menu_systems->state_get()) {
					emulator_container c;
					c.insert(c.end(), rs.menu_systems->user_name_get());
					rs.include_emu_set(c);
				}
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	rs.sub_enable(); // restore the use of the normal config

	return key;
}

int run_subthismenu(config_state& rs)
{
	bool is_favorites_allGames = (rs.include_favorites_get() == "All Games");
	bool enable_filtertype = (rs.favorites_filtertype || is_favorites_allGames);

	choice_bag ch;

	ch.insert(ch.end(), choice(menu_name(rs, "Ordem...", EVENT_SORT), 0));
	ch.insert(ch.end(), choice(menu_name(rs, "Modo...", EVENT_MODE), 1));
	ch.insert(ch.end(), choice(menu_name(rs, "Visualizar...", EVENT_PREVIEW), 2));
	if (enable_filtertype)	{
		ch.insert(ch.end(), choice(menu_name(rs, "Tipos...", EVENT_TYPE), 3));
		ch.insert(ch.end(), choice(menu_name(rs, "Filtros...", EVENT_ATTRIB), 11, rs.include_emu_get().size() != 0));
	}
	
	string title;
	if (rs.sub_has()) {
		title = " Listando Emuladores";
	} else {
		title = " Listando Multiplos";
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(title, SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			emulator* emu;
			switch (i->value_get()) {
			case 0 :
				key = run_sort(rs);
				break;
			case 1 :
				key = run_mode(rs);
				break;
			case 2 :
				key = run_preview(rs);
				break;
			case 3 :
				key = run_type(rs);
				break;
			case 5 :
				rs.sub_get().restore_save();
				break;
			case 6 :
				rs.sub_get().sort_unset();
				break;
			case 7 :
				rs.sub_get().mode_unset();
				break;
			case 8 :
				rs.sub_get().preview_unset();
				break;
			case 9 :
				rs.sub_get().include_type_unset();
				break;
			case 10 :
				break;
			case 11 :
				emu = run_emu_select(rs);
				if (emu) {
					key = emu->attrib_run(FOURTH_CHOICE_X, FOURTH_CHOICE_Y);
				} else {
					key = EVENT_ESC;
				}
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	return key;
}

int run_subfavoritesmenu(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice(menu_name(rs, "Carregar Lista...", EVENT_FAVORITES_NEXT), 0));
	ch.insert(ch.end(), choice(menu_name(rs, "Add/Rem Jogos...", EVENT_SETFAVORITES), 1, rs.current_game != 0));

	string text_filtertype = rs.favorites_filtertype ? "Desativar Filtros/Tipos" : "Ativar Filtros/Tipos"; 
	ch.insert(ch.end(), choice(text_filtertype, 2));
	
	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(" Lista de Favoritos", SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_favorites(rs);
				break;
			case 1 :
				key = run_favorites_move(rs);
				break;
			case 2 :
				rs.favorites_filtertype = !rs.favorites_filtertype;
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	return key;
}

int run_submenu(config_state& rs)
{
	choice_bag ch;

	if (!rs.console_mode) {
		ch.insert(ch.end(), choice("Listando...", 1));
		ch.insert(ch.end(), choice("Configuracoes...", 0));
		ch.insert(ch.end(), choice("Lista de Favoritos...", 2));
		if (rs.emu.size() > 1)
			ch.insert(ch.end(), choice(menu_name(rs, "Emuladores...", EVENT_EMU_NEXT), 7));
		if (!rs.menu_systems->state_get())
			ch.insert(ch.end(), choice("Tipos de Roms...", 3));
		ch.insert(ch.end(), choice("Volume...", 16));
		ch.insert(ch.end(), choice("Dificuldade...", 17));
		ch.insert(ch.end(), choice(menu_name(rs, "Menu de Comandos", EVENT_COMMAND), 8));
		ch.insert(ch.end(), choice(menu_name(rs, "Clone...", EVENT_CLONE), 15));
		ch.insert(ch.end(), choice(menu_name(rs, "Ajuda", EVENT_HELP), 10));
		ch.insert(ch.end(), choice(menu_name(rs, "MameInfo", EVENT_MINFO), 11));
		ch.insert(ch.end(), choice("Estatisticas", 18));
	} else {
		ch.insert(ch.end(), choice(menu_name(rs, "Ajuda", EVENT_HELP), 10));
		if (rs.emu.size() > 1)
			ch.insert(ch.end(), choice(menu_name(rs, "Emuladores...", EVENT_EMU_NEXT), 7));
		ch.insert(ch.end(), choice("Volume...", 16));
		ch.insert(ch.end(), choice("Dificuldade...", 17));
		if (rs.script_bag.size()!=0)
			ch.insert(ch.end(), choice(menu_name(rs, "Menu de Comandos", EVENT_COMMAND), 8));
	}

	if (rs.exit_mode == exit_normal || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice(menu_name(rs, "Sair", EVENT_ESC), 19));
	}
	if (rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice(menu_name(rs, "Desligar", EVENT_OFF), 20));
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;
	int ret = EVENT_NONE;

	do {
		void* save = int_save();

		key = ch.run(" Menu", FIRST_CHOICE_X, FIRST_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0 :
				key = run_suballmenu(rs);
				break;
			case 1 :
				key = run_subthismenu(rs);
				break;
			case 2 :
				key = run_subfavoritesmenu(rs);
				break;
			case 3 :
				key = run_merge(rs);
				break;
			case 7 :
				key = run_emu(rs);
				break;
			case 8 :
				key = run_command(rs);
				break;
			case 10 :
				run_help(rs);
				break;
			case 11 :
				run_subInfohistory(rs);
				break;
			case 15 :
				done = true;
				ret = EVENT_CLONE;
				break;
			case 16 :
				key = run_volume(rs);
				break;
			case 17 :
				key = run_difficulty(rs);
				break;
			case 18 :
				run_stat(rs);
				break;
			case 19 :
				done = true;
				ret = EVENT_ESC;
				break;
			case 20 :
				done = true;
				ret = EVENT_OFF;
				break;
			}
		} else if (key == EVENT_ESC) {
			done = true;
		}

		
		int_restore(save);
		

		if (key == EVENT_MENU || key == EVENT_ENTER)
			done = true;

	} while (!done);
	
	return ret;
}

void run_background(config_state& rs)
{
	string img_help = "";
	
	
	img_help = rs.background_config_menu;
	if (img_help != "" && img_help != "none") {
		int_clear(COLOR_HELP_NORMAL.background);
		int_clip(img_help, true);
	}
}
// ------------------------------------------------------------------------
// Help menu

void run_help(config_state& rs)
{
	string img_help = "";
	emulator* emu_actual = 0;
	
	if (rs.menu_systems->state_get()) {
		emu_actual = rs.menu_systems;
	} else {
		for(pemulator_container::iterator k = rs.emu.begin();k!=rs.emu.end();k++) {
			if ((*k)->state_get() == 1) {
				emu_actual = *k;
				break;
			}
		}
	}

	if (rs.mode_get() == mode_custom) {
		string lay_path = emu_actual->custom_file_path_get();
		for(playout_container::iterator j = rs.lay_cont.begin();j!=rs.lay_cont.end();j++) {
			if((*j)->name_get() == lay_path) {
				img_help = (*j)->custom_help_path_get();
				break;
			}
		}
	} else {
		img_help = emu_actual->nocustom_help_path_get();
		if (img_help == "" || img_help == "default")
			img_help = rs.ui_help;
	}
	
	bool wait = true;

	int_clear(COLOR_HELP_NORMAL.background);

	if (img_help != "" && img_help != "none") {
		wait = int_clip(img_help, true);
	} else {
		if (rs.ui_back != "none") {
			unsigned x, y;
			int_image(rs.ui_back, x, y);
		}

		int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

		int y = rs.ui_top;
		int xt = rs.ui_left + 2*int_font_dx_get();
		int xd = rs.ui_left + (2+12)*int_font_dx_get();
		
		//-----------------------------------------------------------------------------------------------------------------------------------------

		ostringstream os;
		os << video_size_x() << "x" << video_size_y();
		if (video_vclock()) os << "@" << video_vclock() << "Hz";
		os << "  (" << rs.video_sizex << "x" << rs.video_sizey << ")";
		
		y += int_font_dy_get();
		int_put_alpha(xt, y, "VIDEO SIZE:", COLOR_HELP_TAG);
		int_put_alpha(xd, y, os.str(), COLOR_HELP_NORMAL);
		y += int_font_dy_get();

		//-----------------------------------------------------------------------------------------------------------------------------------------

		y += int_font_dy_get();
		int_put_alpha(xt, y, "In the game menu:", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Main menu", COLOR_HELP_NORMAL);
		} else {
			int_put_alpha(xt, y, event_name(EVENT_MENU), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Main menu", COLOR_HELP_NORMAL);
		}
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Run the current game/On menu accept the choice", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next preview mode/On menu change the option", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_MODE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next menu mode", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (rs.exit_mode == exit_normal || rs.exit_mode == exit_all) {
			if (!rs.console_mode) {
				int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
				int_put_alpha(xd, y, "Exit", COLOR_HELP_NORMAL);
				y += int_font_dy_get();
			}
		}
		if (rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all) {
			int_put_alpha(xt, y, event_name(EVENT_OFF), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Shutdown", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.favorites.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_FAVORITES_NEXT), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Next game list", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.type.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_TYPE), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Next game type", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (!rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_ATTRIB), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Include/Exclude games by attribute", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, event_name(EVENT_SORT), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Select the game sort method", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		int_put_alpha(xt, y, event_name(EVENT_EMU_PRE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Previous emulator", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_EMU_NEXT), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Next emulator", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		if (!rs.console_mode) {
			int_put_alpha(xt, y, event_name(EVENT_COMMAND), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Commands", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.favorites.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_SETFAVORITES), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Change the current game lists", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		if (rs.type.size() > 1) {
			int_put_alpha(xt, y, event_name(EVENT_SETTYPE), COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Change the current game type", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
		int_put_alpha(xt, y, event_name(EVENT_CLONE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Run a clone", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ROTATE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Rotate the screen", COLOR_HELP_NORMAL);
		y += int_font_dy_get();

		y += int_font_dy_get();
		int_put_alpha(xt, y, "In the submenus:", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Accept", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_DEL), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Unselect all", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_INS), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Select all", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Toggle (+ include, - exclude, * required)", COLOR_HELP_NORMAL);
		y += int_font_dy_get();
		int_put_alpha(xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
		int_put_alpha(xd, y, "Cancel", COLOR_HELP_NORMAL);
		y += int_font_dy_get();

		if (rs.console_mode) {
			y += int_font_dy_get();
			int_put_alpha(xt, y, "In the emulators:", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "F1", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Help", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "5 6 7 8", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Insert Coins", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "1 2 3 4", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Start Player 1, 2, 3, 4", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "CTRL ALT SPACE", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Buttons Player 1", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "ARROWS", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Move Player 1", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
			int_put_alpha(xt, y, "ESC", COLOR_HELP_TAG);
			int_put_alpha(xd, y, "Return to the menu", COLOR_HELP_NORMAL);
			y += int_font_dy_get();
		}
	}

	if (wait)
		int_event_get();
}

// ------------------------------------------------------------------------
// MAME HISTORY - INFO

#define INFOMAME_CHOICE_DX 15*int_font_dx_get()

void run_Infohistory(const string& file, const string& type, const string& hash, const string& rom)
{
	textview tview;
	
	tview.insert_file(file, type, hash, rom);

	string title = (type == "mamehistory") ? "MAME HISTORY: " + rom : "MAME INFO: " + rom;
	
	tview.run(title);
}

void run_subInfohistory(config_state& rs)
{
	if (!rs.current_game)
		return;
	
	string mamehistory_file = file_file(rs.mame_history); //nombre del archivo (sin path)
	string mameinfo_file = file_file(rs.mame_info); //nombre del archivo (sin path)
	string emu_type = rs.current_game->emulator_get()->type_get(); //tipo de emulador
	string rom = rs.current_game->name_without_emulator_get(); //nombre del juego
	string hash = rs.current_game->name_hash_get(); //nombre del hash o carpeta del juego

	if (hash == "")
		hash = "info";
		
	choice_bag ch;

	//mameHistory
	if ((emu_type == "advmame" || emu_type == "mame" || emu_type == "sdlmame" || emu_type == "mess_mess") &&
	    (mamehistory_file != "" && mamehistory_file != "none"))
	{
		ch.insert(ch.end(), choice("Mame History", 0));
	}

	//mameInfo
	if ((emu_type == "advmame" || emu_type == "mame" || emu_type == "sdlmame") &&
	    (mameinfo_file != "" && mameinfo_file != "none"))
	{
		ch.insert(ch.end(), choice("Mame Info", 1));
	}
	
	choice_bag::iterator i = ch.begin();

	// Si no hay ningun archivo configurado en el rc
	if (ch.size() == 0)
		return;

	// Si solo hay uno de los archivos configurado en el rc
	if (ch.size() == 1) {
		if (i->desc_get() == "Mame History")
			run_Infohistory(rs.mame_history, "mamehistory", hash, rom);
		else
			run_Infohistory(rs.mame_info, "mameinfo", hash, rom);
		return;
	}

	// Si los dos archivos estan configurados en el rc
	bool done = false;

	do {
		int key = ch.run("Info/History", SECOND_CHOICE_X, SECOND_CHOICE_Y, INFOMAME_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
				case 0 :
					run_Infohistory(rs.mame_history, "mamehistory", hash, rom);
					done = true;
					break;
				case 1 :
					run_Infohistory(rs.mame_info, "mameinfo", hash, rom);
					done = true;
					break;
			}
		}

		if (key == EVENT_ESC || key == EVENT_MENU)
			done = true;

	} while (!done);
}

// ------------------------------------------------------------------------
// Stat

#define STAT_MAX 10

void stat_insert(const game* (&map)[STAT_MAX], unsigned (&val)[STAT_MAX], const game* g, unsigned v)
{
	for(unsigned i=0;i<STAT_MAX;++i) {
		if (!map[i] || val[i] < v) {
			for(unsigned j=STAT_MAX-1;j>i;--j) {
				map[j] = map[j-1];
				val[j] = val[j-1];
			}
			map[i] = g;
			val[i] = v;
			return;
		}
	}
}

string stat_time(unsigned v)
{
	ostringstream os;
	os << (v/3600) << ":" << setw(2) << setfill('0') << ((v/60)%60);
	return os.str();
}

string stat_int(unsigned v)
{
	ostringstream os;
	os << v;
	return os.str();
}

string stat_perc(unsigned v, unsigned t)
{
	ostringstream os;
	if (t)
		os << stat_int(v * 100 / t) << "%";
	else
		os << stat_int(0) << "%";
	return os.str();
}

void run_stat(config_state& rs)
{
	unsigned total_count = 0;
	unsigned total_session = 0;
	unsigned total_time = 0;
	unsigned select_count = 0;
	unsigned select_session = 0;
	unsigned select_time = 0;
	const game* most_session_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_time_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_timepersession_map[STAT_MAX] = { 0, 0, 0 };
	unsigned most_session_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_time_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_timepersession_val[STAT_MAX] = { 0, 0, 0 };
	unsigned n;

	int y = rs.ui_top;
	int xn = rs.ui_left + 2*int_font_dx_get();
	int xs = rs.ui_left + (2+1*8)*int_font_dx_get();
	int xt = rs.ui_left + (2+2*8)*int_font_dx_get();
	int xp = rs.ui_left + (2+3*8)*int_font_dx_get();
	int xe = rs.ui_left + (2+4*8)*int_font_dx_get();

	n = ((int_dy_get() - rs.ui_top - rs.ui_bottom) / int_font_dy_get() - 12) / 3;
	if (n > STAT_MAX)
		n = STAT_MAX;

	int_clear(COLOR_HELP_NORMAL.background);

	if (rs.ui_back != "none") {
		unsigned x, y;
		int_image(rs.ui_back, x, y);
	}

	int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

	// select and sort
	for(game_set::const_iterator i=rs.gar.begin();i!=rs.gar.end();++i) {
		unsigned session;
		unsigned time;
		unsigned timepersession;

		if (i->emulator_get()->tree_get())
			session = i->session_tree_get();
		else
			session = i->session_get();
		if (i->emulator_get()->tree_get())
			time = i->time_tree_get();
		else
			time = i->time_get();
		if (session)
			timepersession = time / session;
		else
			timepersession = 0;

		total_count += 1;
		total_session += session;
		total_time += time;

		// emulator
		if (!i->emulator_get()->state_get())
			continue;

		// type
		if (!i->type_derived_get()->state_get())
			continue;

		// filter
		if (!i->emulator_get()->filter(*i))
			continue;

		select_count += 1;
		select_session += session;
		select_time += time;

		stat_insert(most_session_map, most_session_val, &*i, session);
		stat_insert(most_time_map, most_time_val, &*i, time);
		stat_insert(most_timepersession_map, most_timepersession_val, &*i, timepersession);
	}

	y += int_font_dy_get();
	int_put_right_alpha(xs, y, xt-xs, "Listed", COLOR_HELP_TAG);
	int_put_right_alpha(xt, y, xp-xt, "Total", COLOR_HELP_TAG);
	int_put_right_alpha(xp, y, xe-xp, "Perc", COLOR_HELP_TAG);

	{

		y += int_font_dy_get();
		int_put_alpha(xn, y, "Games", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_int(select_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_int(total_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_count, total_count), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Play", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_int(select_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_int(total_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_session, total_session), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Time", COLOR_HELP_TAG);
		int_put_right_alpha(xs, y, xt-xs, stat_time(select_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(xt, y, xp-xt, stat_time(total_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(xp, y, xe-xp, stat_perc(select_time, total_time), COLOR_HELP_NORMAL);
	}

	xs = (1+7)*int_font_dx_get();
	xe = (1+7+5)*int_font_dx_get();
	xt = (1+7+5+2)*int_font_dx_get();

	if (n>0 && most_time_map[0]) {
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most time", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_time_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_time(most_time_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(xs, y, xe-xs, stat_perc(most_time_val[i], select_time), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_time_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n>0 && most_session_map[0]) {
		ostringstream os;
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most play", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_session_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_int(most_session_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(xs, y, xe-xs, stat_perc(most_session_val[i], select_session), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_session_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n>0 && most_timepersession_map[0]) {
		ostringstream os;
		y += int_font_dy_get();
		y += int_font_dy_get();
		int_put_alpha(xn, y, "Most time per play", COLOR_HELP_TAG);
		for(unsigned i=0;i<n && most_timepersession_map[i];++i) {
			y += int_font_dy_get();
			int_put_right_alpha(xn, y, xs-xn, stat_time(most_timepersession_val[i]), COLOR_HELP_NORMAL);
			int_put_alpha(xt, y, most_timepersession_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	int_event_get();
}

// ------------------------------------------------------------------------
// Security Exit

int run_exit(config_state& rs)
{
	int dx = int_font_dx_get() * 10;
	int x = (int_dx_get() - dx) / 2;
	int y = int_dy_get() / 3;
	
	choice_bag ch;

	ch.insert(ch.end(), choice("Continue", 0));

	if (rs.exit_mode == exit_normal || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice("    Sair", 1));
	}
	if (rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all) {
		ch.insert(ch.end(), choice("Poweroff", 2));
	}

	choice_bag::iterator i = ch.begin();

	int ret = EVENT_NONE;

	//int key = ch.run(" Security Exit", x, y, dx, i);
	int key = ch.run("", x, y, dx+10, i);
	
	if (key == EVENT_ENTER) {
		switch (i->value_get()) {
		case 0 :
			break;
		case 1 :
			ret = EVENT_ESC;
			break;
		case 2 :
			ret = EVENT_OFF;
			break;
		}
	}

	return ret;
}
