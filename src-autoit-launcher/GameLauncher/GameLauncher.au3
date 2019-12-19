#NoTrayIcon
#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Icon=icon.ico
#AutoIt3Wrapper_Outfile=..\..\GameLauncher.Exe
#AutoIt3Wrapper_Res_Description=GameLauncher2019
#AutoIt3Wrapper_Res_Fileversion=5.0.0.853
#AutoIt3Wrapper_Res_Fileversion_AutoIncrement=y
#AutoIt3Wrapper_Res_ProductName=GameLauncher
#AutoIt3Wrapper_Res_ProductVersion=5.0.0.1
#AutoIt3Wrapper_Res_CompanyName=Wdiversoes
#AutoIt3Wrapper_Res_LegalCopyright=@Wdiversoes
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****
ProcessSetPriority(@AutoItPID, 5)

#include <Misc.au3>
#include <WinAPIProc.au3>
#include <Crypt.au3>
#include <array.au3>
#include <WinAPIRes.au3>
#include <WindowsConstants.au3>
#include <StaticConstants.au3>

#include "..\includes\auJOYSTICK.au3"
#include "..\includes\AuConstantes.au3"
#include "..\includes\gui_suport.au3"


Opt("MustDeclareVars", 1)       ;Obriga todas variaveis ter declarações



If _Singleton("LauncherBox", 1) = 0 Then
	_CALL_WRITE_LOG("!(ERRO) - Programa já em execução")
	Exit (0)
EndIf

_WinAPI_ShowCursor(False)
MouseMove(@DesktopWidth + 1, (@DesktopHeight + 1) / 2, 0)

if not @Compiled Then
	Local $iJogo = 3
	Local $CMDLINE[5]
	Local $iPasta=  _CONST_REGIST_PAGE() & "\"
	Local $iEMU[][] =[ _
					[$iPasta &  "\emuladores\mame\mameui32.exe",$iPasta & "\JOGOS\neogeo\kf2k2mp2.zip", "NEOGEO", "The King 2002 MP2"],		_
					[$iPasta &  "\emuladores\retroarch32\retroarch.exe", $iPasta & "\JOGOS\snes\Super Mario World (USA).sfc", "SuperNintendo","Super Mario World (USA)"],  _
					["GameLauncher.exe", $iPasta & "\JOGOS\taito\Tetris The Grand Master 3 Terror Instinct.lnk", "TAITOTYPEX","Tetris The Grand Master 3 Terror Instinct"],	_
					["GameLauncher.exe", $iPasta & "\JOGOS\Pcgames\Turtle.lnk", "JogosDeComputador","Turtle"] 	_
					]
	$CMDLINE[1] = $iEMU[$iJogo][0]
	$CMDLINE[2] = $iEMU[$iJogo][1]
	$CMDLINE[3] = $iEMU[$iJogo][2]
	$CMDLINE[4] = $iEMU[$iJogo][3]
EndIf
;-----------------------------------------------------------------------------------
;Imagens
Global $READ_IMG3 = @TempDir & "\~aza"
Global $READ_IMG4 = @TempDir & "\~azb"
Global $READ_IMG5 = @TempDir & "\~azc"
Global $READ_IMG6 = @TempDir & "\~azd"
Global $READ_IMG7 = @TempDir & "\~aze"

;Loading IMAGENS
FileInstall("..\img\progress_a.png", @TempDir & "\~aza", 1)
FileInstall("..\img\progress_b.png", @TempDir & "\~azb", 1)
FileInstall("..\img\progress_c.png", @TempDir & "\~azc", 1)
FileInstall("..\img\progress_d.png", @TempDir & "\~azd", 1)
FileInstall("..\img\progress_e.png", @TempDir & "\~aze", 1)

;-----------------------------------------------------------------------------------



;-----------------------------------------------------------------------------------
;Inicia os Joysticks
;-----------------------------------------------------------------------------------
_JOY_START($__DIR_SDL)
If @error Or Not FileExists($__DIR_SDL) Then
	_CALL_WRITE_LOG("!SDL1.dll não foi encontrada no diretorio do sistema")
	Exit (0)
EndIf

;-----------------------------------------------------------------------------------
; Mensagens
;-----------------------------------------------------------------------------------
Global $_Alerta_MSG1 = "Por favor registre o sistema executando o 'instaler.exe' ou entre em contato com o fornecedor do produto." & @LF & _
		"Telefone: " & $__TELEF & @LF & _
		"E-Mail: " & $__EMAIL

Global $_Alerta_MSG2 = "" & @LF & _
		"Processo não encontrado, o jogo selecionado não funcionou" & @LF & _
		"ou o executavel do emulador não foi encontrado"
Global $_Alerta_MSG3 = "" & @LF & _
		"O jogo foi finalizado pelo usuario" & @LF & _
		"ou o executavel do jogo não funcionou" & @LF & _
		"Saindo..."
Global $_Alerta_MSG4 = "" & @LF & _
		"Classe do executavel não existe ou não foi" & @LF & _
		"cadastrada no arquivo LauncherBox.ini"
;-----------------------------------------------------------------------------------
;LEITURA
;-----------------------------------------------------------------------------------
;sons
Global $READ_SND_AVISO 	 = _CONST_SOUND_FOLDER() & "voz_aviso_envio.wav"
Global $READ_SND_NO_COIN = _CONST_SOUND_FOLDER() & "Voz_semficha.wav"
Global $READ_SND_COIN 	 = _CONST_SOUND_FOLDER() & "coin.wav"
Global $READ_SND_SAIR 	 = _CONST_SOUND_FOLDER() & "Voz_sair10sec.wav"
Global $READ_SND_TRANS 	 = _CONST_SOUND_FOLDER() & "Voz_transferencia.wav"
Global $READ_SND_ALERTA  = _CONST_SOUND_FOLDER() & "voz_falta1min.wav"
Global $SND_AVISO 		 = _SOUND_OPEN($READ_SND_AVISO)
Global $SND_SAIR 		 = _SOUND_OPEN($READ_SND_SAIR)
Global $SND_NO_COIN 	 = _SOUND_OPEN($READ_SND_NO_COIN)
Global $SND_COIN 		 = _SOUND_OPEN($READ_SND_COIN)
Global $SND_TRANS 		 = _SOUND_OPEN($READ_SND_TRANS)
Global $SND_ALERTA 		 = _SOUND_OPEN($READ_SND_ALERTA)

;------
;Ini
;-----------------------------------------------------------------------------------
;Leitura de fichas e
Global $READ_COINS_CASE = _GET_COINS_ALL()
Global $READ_COIN_VALOR = _GET_VALUE_COIN()
Global $SKIP_INTRO 		= True             ;Envia tecla insert para o Mame
Global $PID_EXE 		= False

;Botões dos joysticks para Transferir fichas/Sair do jogo/Adicionar fichas
;Carrega os controles
Global $INIT_JOY1 = _JOY_OPEN($___JOY_1)
Global $INIT_JOY2 = _JOY_OPEN($___JOY_2)
Global $INIT_JOY3 = _JOY_OPEN($___JOY_3)
Global $JOYSTK[3] = [$INIT_JOY1, $INIT_JOY2, $INIT_JOY3]

;-----------------------------------------------------------------------------------
;TICKS_TIME
;-----------------------------------------------------------------------------------
Global $RETTICK_OCIOSO 				= _TICK_UPDATE()
Global $RETTICK_RESET_KEY_SAIR 		= _TICK_UPDATE()
Global $RETTICK_RESET_KEY_TRANS 	= _TICK_UPDATE()
Global $RETTICK_EXE 				= _TICK_UPDATE()
Global $RETTICK_CONTADOR_GUI 		= _TICK_UPDATE()
Global $RETTICK_CLASS 				= _TICK_UPDATE()
Global $RETTICK_CALL_EXIT			= _TICK_UPDATE()
Global $RETTICK_TIME_SAVE			= _TICK_UPDATE()
Global $RETTICK_AUTO_ENVIO			= _TICK_UPDATE()
Global $RETTICK_AUTO_ENVIO_DELAY 	= _TICK_UPDATE()
Global $RETTICK_DELAY_INICIAR_EMU 	= _TICK_UPDATE()
Global $RETTICK_DELAY_LOG			= _TICK_UPDATE()

Global $RETTICK_TIME_PLAY = Number(_GET_COINS_ALL())
;~ If ($RETTICK_TIME_PLAY < 60000) Then $RETTICK_TIME_PLAY = 60000 ; dá alguns segundos caso não haja tempo
$RETTICK_TIME_PLAY += Number(_TICK_UPDATE())

;-----------------------------------------------------------------------------------
;Internal func
Global $LOOP 			= 1                    ; Loop Principal
Global $PRESSED_COIN 	= False         ; Evita repetir teclas se o botão for segurado
Global $PRESSED 		= False                    ; Evita repetir teclas se o botão for segurado
Global $RUN_GAME 		= False                   ; Vinculada a execução do jogo
Global $CALL_EXIT 		= False                 ; Vinculada função de encerramento
Global $CALL_EXIT_TRY 	= 0
Global $INICIA_COBRANCA = False
;-----------------------------------------------------------------------------------
Global $TIME_TRANSFERIR  = 5000      ; 5sec     ||   Segurando o botão transferir ficha
Global $TIME_SAIR		 = 10000                  ; 10 sec   ||   Segurando o botão sair
Global $GAME_PCTAITO 	 = False          ; False = Jogo de PC/TAITO || TRUE = JOGOS DE EMULADORES
Global $MODO_FICHA 		 = False                 ; 0 = TIME ||  1 = COIN
Global $ALERTA_NO_TIME 	 = False       ; Alerta se falta 1 minuto
Global $CONTADOR_ATIVADO = False
Global $SEND_EMU_COIN	 = ""             ;Tecla que será enviada para adicionar ficha no emulador

Global $___START1_KEYBOARD   = 68       ;Tecla 8 numerico (START 1)
Global $___START2_KEYBOARD   = 69        ;Tecla 9 numerico (START 2)
Global $___COIN___KEYBOARD   = 67        ;Tecla 7 numerico (FICHA)
Global $___EXIT1__KEYBOARD 	 = 68       ;Tecla 8 numerico (START 1 - Sair 1)
Global $___EXIT2__KEYBOARD 	 = 69           ;Tecla 9 numerico (START 2 - Sair 2)
Global $___CALLEXIT_KEYBOARD = 66 ;Tecla 6 numerico (Sair direto)

Global $DELAY_TIMER_EVENT_OCIOSO
Global $DELAY_TIMER_EVENT_TIMER
Global $RETRO_MULT_CORE
Global $RETRO_CORE, $RETRO_CONF, $RETRO_CONFIG_MULT, $GET_PATH_GAME, $READ_EMU_PATH, $READ_GAME_PATH, $EMULE_NAME
Global $READ_EXE_PATH, $READ_EXE,  $RETROARCH_MOD, $READ_COMAND1, $READ_COMAND2,$READ_JOYTOKEY
Global $TIME_SAIR_OCIOSO, $PAY_TO_PLAY, $AUTO_ENVIO_FICHA,$iGUI_SPLASH
Global $READ_LOADER, $READ_CONTADOR = 0, $CONTADOR_LABEL

Global $COMMAND_LINE = ""
Global $GetPathTemp  = ""
Global $SISTEM_DIR   = @ScriptDir
If Not @Compiled Then
	$SISTEM_DIR      = "F:\ADVMENU\ADVMENU\"
	$GetPathTemp     = $SISTEM_DIR
EndIf

Global $iDLE_AVISO 	 = False
Global $iGUI_IDLE, $iGUI_TRANS


;-----------------------------------------------------------------------------------
;Inicia leitura dos emuladores
Local $iRun_sys = _Start_ComandLine($CMDLINE)
If @error Then
	MsgBox(262192, "Erro no GameLauncher.exe", $iRun_sys, 5)
	_CALL_WRITE_LOG("!(ERRO) - " & $iRun_sys)
	Exit (0)
EndIf

;---------------------------------------------------------------------------------------
;INICIA A TELA DE CARREGAMENTO
Local $iFadeFile 		= (UBound($CMDLINE) - 1 >= 3) ? _CONST_FADE_FOLDER() & "\" & $CMDLINE[3] & "\" : ""
Local $iDescription 	= (UBound($CMDLINE) - 1 >= 4) ? $CMDLINE[4] : ""
Global $SPLASH_DELAY	= IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","INTRO_DEMO",0)
;SPLASH INICIAR
Global $iGUI_SPLASH 	= ($SPLASH_DELAY <= 0)? 0 : _SPLASH_LOAD($iFadeFile, _             ;Background da tela de carregamento
							_CONST_FADE_FOLDER() & "\" & "_Default" & "\", _               ;Posição X da barra de carregamento
							(@DesktopWidth / 2) - (558 / 2), _                            ;Posição X da barra de carregamento
							(@DesktopHeight / 2 + 95), _                                   ;Posição Y da barra de carregamento
							90, _                                                        ;Altura da barra de carregamento
							558, _                                                        ;Largura da barra de carregametno
							$READ_IMG3, _                                                ;Pontinho esquerdo (|
							$READ_IMG4, _                                                ;Pontinho direito  |)
							$READ_IMG5, _                                                ;Barra de carregamento =======
							$READ_IMG6, _                                                ;Background da barra de carregamento
							$READ_IMG7, _                                                ;Background da descr
							$iDescription, _
							$MODO_FICHA)
;-----------------------------------------------------------------------------------
_SPLASH_SET_PROGRESS($iGUI_SPLASH, 40, "DEMONSTRAÇÃO...")
_Clean_debug()        ;Limpa o arquivo de debug
_Clear_hotkeys()    ;Limpa as teclas
Global $iGUI_CRONO, $iGUI_IDLE, $iGUI_WSAIR,$iGUI_TRANS

	if $READ_CONTADOR Then
		Global $iGUI_CRONO  = _POPUP_WINDOW_CREATE("arial", 0x00FF00, 0x000000)
		Global $iGUI_IDLE 	= _POPUP_WINDOW_CREATE()
		Global $iGUI_WSAIR  = _POPUP_WINDOW_CREATE()
		Global $iGUI_TRANS  = _POPUP_WINDOW_CREATE()
	EndIf

	if $RETROARCH_MOD and Not $READ_CONTADOR Then
		$READ_CONTADOR = True
	EndIf




OnAutoItExitRegister("_EXIT")     ;Registra o evento de saida

#Region =======================================================================>
;Inicia o Loop principal
#EndRegion =======================================================================>
While $LOOP
	_JOY_UPDATE($JOYSTK)    	;Atualiza os Joysticks
	_CALL_CHECK_CLASS_EXE() 	;Checa a classe e a execução dos emuladores
	_JOY_RESET_TICK($JOYSTK)	;RESETA TICK SE APERTAR 1 BOTÃO
	_OCIOSO_SYSTEM_CONST()		;GERENCIAR O IDLE, SAIR OU EXIBIR MENSAGEM NA TELA


	Select
		Case _IsPressed($___COIN___KEYBOARD) Or _JOY_GET_BUTTON($JOYSTK[$__READ_COINS_KEY[0][0]], $__READ_COINS_KEY[0][1]) ;///// -> FICHA
			If Not $PRESSED_COIN Then
				_CALL_COIN()
				_CALL_WRITE_LOG("+DINHEIRO ADICIONA AO SISTEMA!")
			EndIf
			$PRESSED_COIN = True
		Case (_JOY_GET_BUTTON($JOYSTK[$__READ_SAIR_KEY[0][0]], $__READ_SAIR_KEY[0][1]) And _JOY_GET_BUTTON($JOYSTK[$__READ_SAIR_KEY[1][0]], $__READ_SAIR_KEY[1][1])) _  ;///// -> SAIR
				Or (_IsPressed($___EXIT1__KEYBOARD) And _IsPressed($___EXIT2__KEYBOARD))                                                                                ;///// -> SAIR

			_write_debug_sair(Int(($TIME_SAIR - (_TICK_UPDATE() - $RETTICK_RESET_KEY_SAIR)) / 1000))
			_write_debug_trans()
			_TICK_RESET($RETTICK_RESET_KEY_TRANS)

			Local $iT = (_TICK_UPDATE() - $RETTICK_RESET_KEY_SAIR)

			If 800 <= $iT Then
				If Not $PRESSED Then
					_SOUND_PLAYSOUND($SND_SAIR)
				EndIf
				If $iT >= $TIME_SAIR Then
					$CALL_EXIT = true ;_CALL_EXIT()
					_CALL_WRITE_LOG("!O emulador foi fechado pelo usuário! Voltando para o menu...")
;~ 					$RETTICK_RESET_KEY_SAIR = _TICK_UPDATE()
					_TICK_RESET($RETTICK_RESET_KEY_SAIR)
					_POPUP_WINDOW_SHOW($iGUI_WSAIR, False)
				Else
					If $READ_CONTADOR Then
						_POPUP_WINDOW_SHOW($iGUI_TRANS, False)
						_POPUP_WINDOW_SET_MSG($iGUI_WSAIR, _
								"Saindo em %s segundo(s)", _
								StringFormat("%02d", Int(Int(($TIME_SAIR - (_TICK_UPDATE() - $RETTICK_RESET_KEY_SAIR)) / 1000))), _
								(@DesktopHeight / 36), _                         		;FONT SIZE
								(@DesktopWidth  / 2) - ((@DesktopWidth  /  2) / 2), _ 	;POSIÇÃO X
								(@DesktopHeight / 2) - ((@DesktopHeight / 17) / 2), _ 	;PISIÇÃO Y
								(@DesktopWidth  / 2), _                           		;LARGURA
								(@DesktopHeight / 20), _                         		;ALTURA
								1)                                                		;QUANTIDADE DE LINHAS

					EndIf
				EndIf

				$PRESSED = True
			EndIf
		Case ($MODO_FICHA) And ($AUTO_ENVIO_FICHA <= 0) _
				And ((_JOY_GET_BUTTON($JOYSTK[$__READ_TRANS_KEY[0][0]], $__READ_TRANS_KEY[0][1]) Or _JOY_GET_BUTTON($JOYSTK[$__READ_TRANS_KEY[1][0]], $__READ_TRANS_KEY[1][1])) _ ;///// -> Controles
				Or (_IsPressed($___START1_KEYBOARD) Or _IsPressed($___START2_KEYBOARD)))                                                                                    ;///// -> Teclado

				_write_debug_sair()
				_TICK_RESET($RETTICK_RESET_KEY_SAIR)
				Local $iT = (_TICK_UPDATE() - $RETTICK_RESET_KEY_TRANS)

				If $iT >= 1000 Then
					If (Int(_GET_COINS_ALL()) >= Int(_GET_VALUE_COIN())) Or Not $PAY_TO_PLAY Then
						_write_debug_trans(Int(($TIME_TRANSFERIR - (_TICK_UPDATE() - $RETTICK_RESET_KEY_TRANS)) / 1000))
						_write_debug_sair()
					EndIf

					If Not $PRESSED Then
						If (Int(_GET_COINS_ALL()) >= Int(_GET_VALUE_COIN())) Or Not $PAY_TO_PLAY Then
							_SOUND_PLAYSOUND($SND_TRANS)
						Else
							_SOUND_PLAYSOUND($SND_NO_COIN)
						EndIf
					EndIf

					If $iT >= $TIME_TRANSFERIR Then
						_CALL_TRANS_COIN()
						_CALL_WRITE_LOG("+Ficha Transferida para o emulador!")
						_TICK_RESET($RETTICK_RESET_KEY_TRANS)
	;~ 					$RETTICK_RESET_KEY_TRANS = _TICK_UPDATE()     ;- 2000
						_write_debug_trans()
						_POPUP_WINDOW_SHOW($iGUI_TRANS, False)
					Else
						If $READ_CONTADOR And (Int(_GET_COINS_ALL()) >= Int(_GET_VALUE_COIN())) Then
							_POPUP_WINDOW_SHOW($iGUI_WSAIR, False)
							_POPUP_WINDOW_SET_MSG($iGUI_TRANS, _
									"Transferindo ficha em %s segundo(s)", _
									StringFormat("%02d", Int(($TIME_TRANSFERIR - (_TICK_UPDATE() - $RETTICK_RESET_KEY_TRANS)) / 1000)), _
									(@DesktopHeight / 36), _                             ;FONT SIZE
									(@DesktopWidth / 2) - ((@DesktopWidth / 2) / 2), _  ;POSIÇÃO X
									(@DesktopHeight / 2) - ((@DesktopHeight / 17) / 2), _ ;PISIÇÃO Y
									(@DesktopWidth / 2), _                               ;LARGURA
									(@DesktopHeight / 20), _                             ;ALTURA
									1)                                                    ;QUANTIDADE DE LINHAS

						EndIf
					EndIf
					$PRESSED = True
				EndIf
		Case _IsPressed($___CALLEXIT_KEYBOARD)
			$CALL_EXIT = true ;	_CALL_EXIT()
			_CALL_WRITE_LOG("!O emulador foi fechado pelo usuário (TECLADO)! Voltando para o menu...")
		Case Else
			_POPUP_WINDOW_SHOW($iGUI_TRANS, False)
			_POPUP_WINDOW_SHOW($iGUI_WSAIR, False)
			_write_debug_sair()
			_write_debug_trans()
			$PRESSED_COIN = False
			$PRESSED = False
			_TICK_RESET($RETTICK_RESET_KEY_SAIR)
			_TICK_RESET($RETTICK_RESET_KEY_TRANS)
	EndSelect

	Switch $MODO_FICHA
		Case True
			Local $iget = (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO)
			If ($iget < $AUTO_ENVIO_FICHA) And ($iget >= 0) Then _write_debug_autoenvio(Int(($AUTO_ENVIO_FICHA - (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO)) / 1000))
			If Not $CALL_EXIT And ($PAY_TO_PLAY) And $AUTO_ENVIO_FICHA > 0 And (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO) >= $AUTO_ENVIO_FICHA  Then
				If _Get_emule_maximized($PID_EXE) Then Send("{INS UP}")
				If (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO_DELAY >= 500) Then
					_CALL_TRANS_COIN()
					If Not @error Then _CALL_WRITE_LOG(">Ficha auto enviada para o emulador!")
					_TICK_RESET($RETTICK_AUTO_ENVIO_DELAY)
					_write_debug_autoenvio()
				EndIf
			EndIf
		Case False
			;------------------------------------------------------------------------
			;Salva o tempo a cada 25 segundos
			If ($INICIA_COBRANCA) And ($PAY_TO_PLAY) And (_TICK_UPDATE() - $RETTICK_TIME_SAVE) >= 25000 Then
				_CALL_SAVE_TIME($RETTICK_TIME_PLAY, _TICK_UPDATE())
				_TICK_RESET($RETTICK_TIME_SAVE)
			EndIf
			;------------------------------------------------------------------------
			;Sair se o tempo acabar
			If ($INICIA_COBRANCA) And Number($RETTICK_TIME_PLAY) <= Number(_TICK_UPDATE()) And $PAY_TO_PLAY Then
				$CALL_EXIT = true ;_CALL_EXIT()
				_CALL_WRITE_LOG("!O tempo acabou, emulador fechado!")
				$INICIA_COBRANCA = False
			EndIf
			;------------------------------------------------------------------------
			;Alerta faltando 1 min
			If ($INICIA_COBRANCA) And ($PAY_TO_PLAY) And (($RETTICK_TIME_PLAY - _TICK_UPDATE()) <= 60000) And (Not $ALERTA_NO_TIME) Then
				$ALERTA_NO_TIME = True
				_SOUND_PLAYSOUND($SND_ALERTA)
			EndIf
			;------------------------------------------------------------------------
			;CRONOMETRO NA TELA 00:00:00
			If ($INICIA_COBRANCA) And ($READ_CONTADOR) And ($PAY_TO_PLAY) Then
				Local $iTg = Int($RETTICK_TIME_PLAY - _TICK_UPDATE())
				Local $Calculado = _CALCULA_TIMER($iTg)
				If Int($DELAY_TIMER_EVENT_TIMER) <> Int($iTg / 1000) Then
					_POPUP_WINDOW_SET_MSG($iGUI_CRONO, _
							"TEMPO:%s", _
							$Calculado, _
							(@DesktopHeight / 36), _                                                                     ;FONT SIZE
							(@DesktopWidth / 2 - (((@DesktopWidth / 5) + (@DesktopWidth / 31)) / 2)), _                    ;POSIÇÃO X
							(@DesktopHeight - (@DesktopHeight / 17)), _                                                    ;PISIÇÃO Y
							((@DesktopWidth / 5) + (@DesktopWidth / 25)), _                                               ;LARGURA
							(@DesktopHeight / 20), _                                                                         ;ALTURA
							1)                                                                                              ;QUANTIDADE DE LINHAS
					_write_debug_timer(_CALCULA_TIMER($iTg))
					$DELAY_TIMER_EVENT_TIMER = Int($iTg / 1000)
				EndIf
			EndIf
	EndSwitch

	;-----------------------------------------------------------------------------
	;RESETAR O EXIT CASO O EMULADOR NÃO FECHE
	If ($CALL_EXIT) And ((_ProcessExistEX()) Or (_TICK_UPDATE() - $RETTICK_CALL_EXIT >= 500)) Then
;~ 	If ($CALL_EXIT) And ((_ProcessExistEX()) Or (_TICK_UPDATE() - $RETTICK_CALL_EXIT >= 3500) or ($CALL_EXIT_TRY <= 1)) Then
		If Not _ProcessExistEX() Then
			ExitLoop
		Else
			_CALL_EXIT()
		EndIf
	EndIf
	;------------------------------------------------------------------------
	;LOG
	if _TICK_UPDATE() - $RETTICK_DELAY_LOG >= 800 Then
		_LOG_SYSTEM()
		_TICK_RESET($RETTICK_DELAY_LOG)
	EndIf
WEnd
_EXIT()

Func _ProcessExistEX()
	if ProcessExists($READ_EXE) Or ProcessExists($READ_LOADER) Then Return True
	Return False
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _EXIT
; Description ...:
; Syntax ........: _EXIT()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _EXIT()
	_JOY_QUIT()
	_JOYTOKEY_EXIT()
	FileDelete($READ_IMG3)
	FileDelete($READ_IMG4)
	FileDelete($READ_IMG5)
	FileDelete($READ_IMG6)
	FileDelete($__DEBUG_FILER)

	_Clear_hotkeys()

	_WINDOW_DELETE($iGUI_CRONO)
	_WINDOW_DELETE($iGUI_WSAIR)
	_WINDOW_DELETE($iGUI_TRANS)
	_WINDOW_DELETE($iGUI_IDLE)
	_SPLASH_CLOSE($iGUI_SPLASH)
	Exit (0)
EndFunc   ;==>_EXIT
; #FUNCTION# ====================================================================================================================
; Name ..........: _OCIOSO_SYSTEM_CONST
; Description ...:
; Syntax ........: _OCIOSO_SYSTEM_CONST()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _OCIOSO_SYSTEM_CONST()
	Local $is = Int(($TIME_SAIR_OCIOSO - (_TICK_UPDATE() - $RETTICK_OCIOSO)) / 1000)

	If ($DELAY_TIMER_EVENT_OCIOSO <> $is) And ($TIME_SAIR_OCIOSO > 0) Then
		;CONTADOR DE IDLE - OCIOSO
		_write_debug_ocioso($is)
		;SE TIVER MENOR QUE 30 SEGUNDOS - AVISAR
		If ($TIME_SAIR_OCIOSO > 0) And $READ_CONTADOR Then
			;avisa no ini do mame

			If (Int(($TIME_SAIR_OCIOSO - (_TICK_UPDATE() - $RETTICK_OCIOSO)) / 1000) < 30) Then
				If $READ_CONTADOR Then _POPUP_WINDOW_SET_MSG($iGUI_IDLE, _
						"O jogo será fechado por inatividade/n %s  segundo(s)", _
						Int(($TIME_SAIR_OCIOSO - (_TICK_UPDATE() - $RETTICK_OCIOSO)) / 1000), _
						(@DesktopHeight / 36), _                                                                         ;FONT SIZE
						(@DesktopWidth / 2) - ((@DesktopWidth / 2) / 2), _                                              ;POSIÇÃO X
						(@DesktopHeight / 2) - ((@DesktopHeight / 17) / 2), _                                             ;PISIÇÃO Y
						(@DesktopWidth / 2), _                                                                           ;LARGURA
						(@DesktopHeight / 20), _                                                                         ;ALTURA
						2)                                                                                                ;QUANTIDADE DE LINHAS

			Else
				_POPUP_WINDOW_SHOW($iGUI_IDLE, False)
			EndIf
		EndIf
		;------------------------------------------------------------------------
		;SAIR OCIOSO - FECHAR AO CHEGAR A ZERO
		If (_TIME_OCIOSO_EXIT(Number(_TICK_UPDATE()), Number($RETTICK_OCIOSO), Number($TIME_SAIR_OCIOSO))) Then
			$CALL_EXIT = True ;_CALL_EXIT()
			_CALL_WRITE_LOG("!O emulador foi fechado por ocioso")
		EndIf
		$DELAY_TIMER_EVENT_OCIOSO = $is
	EndIf
EndFunc   ;==>_OCIOSO_SYSTEM_CONST

; #FUNCTION# ====================================================================================================================
; Name ..........: _RUN
; Description ...:
; Syntax ........: _RUN($iExe, $iCmd, $path)
; Parameters ....: $iExe                - an integer value.
;                  $iCmd                - an integer value.
;                  $path                - a pointer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _RUN($iExe, $iCmd, $path)
	Local $pid = ShellExecute($iExe, $iCmd, $path, "", @SW_SHOW) ;Run($iExe & " " & $iCmd, $path, @SW_SHOW)

	If $RETROARCH_MOD Then
		_CALL_WRITE_LOG(">Emulador Retroarch chamado (" & $EMULE_NAME & ")")
	ElseIf $GAME_PCTAITO Then
		_CALL_WRITE_LOG(">Jogos de PC/TAITO chamado (" & $EMULE_NAME & ")")
	Else
		_CALL_WRITE_LOG(">Emulador normal chamado (" & $EMULE_NAME & ")")
	EndIf

	Return $pid
EndFunc   ;==>_RUN
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_CHECK_CLASS_EXE
; Description ...:
; Syntax ........: _CALL_CHECK_CLASS_EXE()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALL_CHECK_CLASS_EXE()
	If $CALL_EXIT Then Return
	If (_TICK_UPDATE() - $RETTICK_DELAY_INICIAR_EMU) < $SPLASH_DELAY Then Return
	If Not $RUN_GAME Then
		_SPLASH_SET_PROGRESS($iGUI_SPLASH, 60, "INICIANDO EMULADOR...")
		$PID_EXE = _RUN($READ_EXE, $COMMAND_LINE, $READ_EXE_PATH)
;~ 		ProcessSetPriority($PID_EXE,$PROCESS_HIGH )
		$RUN_GAME = True
		_TICK_RESET($RETTICK_EXE)
;~ 		$RETTICK_EXE = _TICK_UPDATE()

	Else

		If (_TICK_UPDATE() - $RETTICK_EXE) >= 15000 Then
			If (Not ProcessExists($READ_EXE)) And Not $READ_LOADER Then
				$CALL_EXIT = True ;_CALL_EXIT()
				_CALL_WRITE_LOG("!(ERRO) - Processo não existe, voltou para o menu!")
			ElseIf $READ_LOADER And (Not ProcessExists($READ_LOADER)) Then
				$CALL_EXIT = True ;_CALL_EXIT()
				_CALL_WRITE_LOG("!(ERRO) - Loader (TAITO) não existe, voltou para o menu!")
			EndIf

			If Not ($INICIA_COBRANCA) Then
				$INICIA_COBRANCA = True
				$RETTICK_TIME_PLAY = Number(_GET_COINS_ALL())
				$RETTICK_TIME_PLAY += Number(_TICK_UPDATE())
			EndIf
		EndIf

		Local $iGet_EXE = (_TICK_UPDATE() - $RETTICK_EXE)
		If $iGet_EXE <= 14000 Then
			_SPLASH_SET_PROGRESS($iGUI_SPLASH, 100, "CARREGANDO JOGO...")
			If $iGet_EXE >= 12000 Then _SPLASH_CLOSE($iGUI_SPLASH)
		EndIf

;~ 		if $READ_CONTADOR >= 1 And Not $CONTADOR_ATIVADO Then
;~ 				if _Get_emule_maximized($PID_EXE) Then
;~ 					if (_TICK_UPDATE() - $RETTICK_CONTADOR_GUI) >= 3000 Then

;~ 						if  not $RETROARCH_MOD Then

;~ 							if $MODO_FICHA Then
;~ 								$iGUI_TRANS = _POPUP_WINDOW_CREATE()
;~ 							Else
;~ 								$iGUI_CRONO = _CRONO_WINDOW_CREATE()
;~ 							EndIf
;~ 						EndIf
;~ 							$CONTADOR_ATIVADO =True
;~ 					EndIf
;~ 				Else
;~ 					$RETTICK_CONTADOR_GUI = _TICK_UPDATE()
;~ 				EndIf
;~ 		EndIf

;~ 			EndIf
;~ 			EndIf

		If (_TICK_UPDATE() - $RETTICK_EXE) >= 3000 And $SKIP_INTRO And $MODO_FICHA Then
			If $AUTO_ENVIO_FICHA > 0 And $PAY_TO_PLAY And _Get_emule_maximized($PID_EXE) Then Send("{INS DOWN}")
			_TICK_RESET($RETTICK_AUTO_ENVIO)
;~ 			$RETTICK_AUTO_ENVIO = _TICK_UPDATE()
			$SKIP_INTRO = False
		EndIf

	EndIf
EndFunc   ;==>_CALL_CHECK_CLASS_EXE
; #FUNCTION# ====================================================================================================================
; Name ..........: _Get_emule_maximized
; Description ...:
; Syntax ........: _Get_emule_maximized($Process)
; Parameters ....: $Process             - an unknown value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Get_emule_maximized($Process)
	If $GAME_PCTAITO Then Return True
	Local $wArray = _WinAPI_EnumProcessWindows(ProcessExists($Process), True)
	If @error Then
		Return 0
	Else
		For $i = 0 To UBound($wArray) - 1
			If WinGetState($wArray[$i][0]) = $WIN_STATE_EXISTS + $WIN_STATE_VISIBLE + $WIN_STATE_ENABLED + $WIN_STATE_ACTIVE Then Return True
		Next
		Return True
	EndIf
EndFunc   ;==>_Get_emule_maximized
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_COIN
; Description ...:
; Syntax ........: _CALL_COIN()
; Parameters ....: None
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALL_COIN()
	If (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO) < $AUTO_ENVIO_FICHA Or Not $MODO_FICHA Or $AUTO_ENVIO_FICHA <= 0 Then _SOUND_PLAYSOUND($SND_COIN)
	If Not $MODO_FICHA Then
		$RETTICK_TIME_PLAY += (_GET_VALUE_COIN() * _GET_CREDIT_FOR_COIN())
		$ALERTA_NO_TIME = False
		GUICtrlSetColor($CONTADOR_LABEL, 0x00FF00)
	EndIf

	Local $creditopf = _GET_CREDIT_FOR_COIN()
	Local $atual 	 = _GET_COINS_ALL()
	Local $contador  = _GET_COUNTER_ALL()
	Local $valor 	 = _GET_VALUE_COIN()
	Local $adiciona  = (Number($contador) + 1)
	_GET_COUNTER_ALL(Number($adiciona))
	Local $calcula   = Number(Number($atual) + Number($valor) * Number($creditopf))
	_GET_COINS_ALL($calcula)
	If ($PAY_TO_PLAY and $AUTO_ENVIO_FICHA <= 0) Then _write_debug_fichas(_GET_COINS_ALL() / _GET_VALUE_COIN())

EndFunc   ;==>_CALL_COIN
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_TRANS_COIN
; Description ...:
; Syntax ........: _CALL_TRANS_COIN()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALL_TRANS_COIN()
	If $PAY_TO_PLAY Then
		Local $atual = _GET_COINS_ALL()
		Local $valor = _GET_VALUE_COIN()

		If (Number($atual) < Number($valor)) Then Return SetError(-1)
		Local $calcula = (Number($atual) / Number($valor)) - 1
		Local $resultado = (Number($calcula) * Number($valor))
		_GET_COINS_ALL($resultado)
		If ($PAY_TO_PLAY and $AUTO_ENVIO_FICHA <= 0) Then _write_debug_fichas(_GET_COINS_ALL() / _GET_VALUE_COIN())
	EndIf

	Local $get_key = _Get_exit_key($SEND_EMU_COIN)
	If Not IsArray($get_key) Then Return
	If _Get_emule_maximized($PID_EXE) or $GAME_PCTAITO Then
		Send($get_key[0])
		Sleep(100)
		Send($get_key[1])
	EndIf
EndFunc   ;==>_CALL_TRANS_COIN
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_EXIT
; Description ...:
; Syntax ........: _CALL_EXIT()
; Parameters ....:
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _ProcessCloseEX()
	ProcessClose($READ_EXE)
	ProcessClose($READ_LOADER)
EndFunc


Func _CALL_EXIT()
	_WINDOW_DELETE($iGUI_CRONO)
	_WINDOW_DELETE($iGUI_WSAIR)
	_WINDOW_DELETE($iGUI_TRANS)
	_WINDOW_DELETE($iGUI_IDLE)
	_SPLASH_CLOSE($iGUI_SPLASH)
	$CALL_EXIT = True
		If $CALL_EXIT_TRY < 5 Then
			_Close_Pid($PID_EXE)
		Else
			if _ProcessExistEX() Then _ProcessCloseEX()
;~ 			if ProcessExists($READ_LOADER) Then ProcessClose($READ_LOADER)
			_CALL_WRITE_LOG("!Saída forçada! Voltando para o menu...")
		EndIf
		_TICK_RESET($RETTICK_CALL_EXIT)
		$CALL_EXIT_TRY += 1
 		;$RETTICK_CALL_EXIT = _TICK_UPDATE()
	;Salva o tempo
	If Not $MODO_FICHA Then _CALL_SAVE_TIME($RETTICK_TIME_PLAY, _TICK_UPDATE())
EndFunc   ;==>_CALL_EXIT

; ===============================================================================================================================
;FECHA O EMULADOR ENVIANDO COMANDO DE FECHAR
; ===============================================================================================================================
Func _Close_Pid($iPid1)
	if ProcessExists($iPid1) Then
		Local $wArray = _WinAPI_EnumProcessWindows($iPid1, True)
		If Not @error Then
			For $i = 1 To UBound($wArray) - 1
				WinClose($wArray[$i][0])
			Next
		EndIf
	EndIf
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _Get_exit_key
; Description ...:
; Syntax ........:
; Parameters ....: $ikey
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Get_exit_key($ikey)
	If $ikey = "" Then Return SetError(-1)
	Local $iKey_replace = StringReplace($ikey, " ", "")
	Local $CLOSE_EM[37][3] = [["ESC", "{ESC DOWN}", "{ESC UP}"], _
			["ALT+F4", "!+{F4}", ""], _
			["a", "{a DOWN}", "{a UP}"], _
			["b", "{b DOWN}", "{b UP}"], _
			["c", "{c DOWN}", "{c UP}"], _
			["d", "{d DOWN}", "{d UP}"], _
			["e", "{e DOWN}", "{e UP}"], _
			["f", "{f DOWN}", "{f UP}"], _
			["g", "{g DOWN}", "{g UP}"], _
			["h", "{h DOWN}", "{h UP}"], _
			["i", "{i DOWN}", "{i UP}"], _
			["j", "{j DOWN}", "{j UP}"], _
			["k", "{k DOWN}", "{k UP}"], _
			["l", "{l DOWN}", "{l UP}"], _
			["m", "{m DOWN}", "{m UP}"], _
			["n", "{n DOWN}", "{n UP}"], _
			["o", "{o DOWN}", "{o UP}"], _
			["p", "{p DOWN}", "{p UP}"], _
			["q", "{q DOWN}", "{q UP}"], _
			["r", "{r DOWN}", "{r UP}"], _
			["s", "{s DOWN}", "{s UP}"], _
			["t", "{t DOWN}", "{t UP}"], _
			["u", "{u DOWN}", "{u UP}"], _
			["v", "{v DOWN}", "{v UP}"], _
			["x", "{x DOWN}", "{x UP}"], _
			["y", "{y DOWN}", "{y UP}"], _
			["z", "{z DOWN}", "{z UP}"], _
			["0", "{0 DOWN}", "{0 UP}"], _
			["1", "{1 DOWN}", "{1 UP}"], _
			["2", "{2 DOWN}", "{2 UP}"], _
			["3", "{3 DOWN}", "{3 UP}"], _
			["4", "{4 DOWN}", "{4 UP}"], _
			["5", "{5 DOWN}", "{5 UP}"], _
			["6", "{6 DOWN}", "{6 UP}"], _
			["7", "{7 DOWN}", "{7 UP}"], _
			["8", "{8 DOWN}", "{8 UP}"], _
			["9", "{9 DOWN}", "{9 UP}"]]
	Local $get_key = _ArraySearch($CLOSE_EM, $iKey_replace, 0, 0, 0, 1, 1, 0)
	If @error Then Return SetError(-2)
	Local $array = [$CLOSE_EM[$get_key][1], $CLOSE_EM[$get_key][2]]

	Return $array
EndFunc   ;==>_Get_exit_key
; #FUNCTION# ====================================================================================================================
; Name ..........: _TIME_OCIOSO_EXIT
; Description ...:
; Syntax ........: _TIME_OCIOSO_EXIT($iTick, $iRVar, $iSair)
; Parameters ....: $iTick               - an integer value.
;                  $iRVar               - an integer value.
;                  $iSair               - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _TIME_OCIOSO_EXIT($iTick, $iRVar, $iSair)
	If ($iTick - $iRVar) >= $iSair Then Return True
	Return False
EndFunc   ;==>_TIME_OCIOSO_EXIT
; #FUNCTION# ====================================================================================================================
; Name ..........: _Start_ComandLine
; Description ...:
; Syntax ........: _Start_ComandLine($iCMDLINE)
; Parameters ....: $iCMDLINE            - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Start_ComandLine($iCMDLINE)
	If (UBound($iCMDLINE) - 1) < 3 Then Return SetError(-1,0,"Nenhum parametro foi informado pelo usuário!")

	Local $RETRO_MULT_CORE = False, $RETRO_EXTENS
	Local $sDrive = "", $sDir = "", $sFileName = "", $sExtension = "", $GET_PATH_EMU
	;=========================================================================================================
	;PEGA AS LINHAS DE COMANDO
	$READ_EMU_PATH 		= $iCMDLINE[1]
	$READ_GAME_PATH 	= $iCMDLINE[2]
	$EMULE_NAME 		= $iCMDLINE[3]
	;=========================================================================================================
	;PEGA O PATH DO EMULADOR E SEPARA
	$GET_PATH_EMU 		= _PathSplit(StringReplace($READ_EMU_PATH,"$PROGDIR",@ScriptDir), $sDrive, $sDir, $sFileName, $sExtension)
	If @error Then Return SetError(-2,0,"Não foi encontrado o local do emulador, pasta invalida!")
	$READ_EXE_PATH 		= FileGetShortName(_PathFull($GET_PATH_EMU[1] & $GET_PATH_EMU[2]))
	$READ_EXE 			= $GET_PATH_EMU[3] & $GET_PATH_EMU[4]
	;=========================================================================================================
	;Leitura de INI

	Local $SISTEMA_INI = $SISTEM_DIR & "\ini\" & $EMULE_NAME & ".ini"
	If Not FileExists($SISTEMA_INI) Then Return SetError(-3,0,'Arquivo "ini" referente ao emulador selecionado "' &  $EMULE_NAME &  ', não foi encontrado na pasta "ini"')
	;Ocioso
	$TIME_SAIR_OCIOSO   = Number(IniRead(_CONST_ADVMENU_INI(), "CONFIGURACAO", "OCIOSO", 300000))         ;     ||   Sair ocioso
	;Free To Play
	$PAY_TO_PLAY 		= (Int(IniRead(_CONST_ADVMENU_INI(), "CONFIGURACAO", "BLOQUEADO", 1)) > 0) ? True : False            ; False = Gratis  ||  True = Pago
	;AUTO ENVIO
	$AUTO_ENVIO_FICHA   = Int(IniRead($SISTEMA_INI, "configuracao", "FICHA_AUTOENVIO", 0))
	$RETROARCH_MOD 		= (Int(IniRead($SISTEMA_INI, "RETROARCH", "RETROARCH", 0)) > 0) ? True : False
	$GAME_PCTAITO 		= (Int(IniRead($SISTEMA_INI, "configuracao", "PCTAITO_GAMES", 0)) > 0) ? True : False
	If $RETROARCH_MOD Then $GAME_PCTAITO = False
	If $GAME_PCTAITO  Then $RETROARCH_MOD = False
	;=========================================================================================================
	;LEITURA (COMAND LINE DO EMULADOR)
	$READ_COMAND1 		= IniRead($SISTEMA_INI, "configuracao", "COMANDO1", "")
	$READ_COMAND2 		= IniRead($SISTEMA_INI, "configuracao", "COMANDO2", "")
	$READ_JOYTOKEY		=(IniRead($SISTEMA_INI, "configuracao", "JOYTOKEY", "0") <= 0)? False : _JOYTOKEY_INIT($EMULE_NAME)
	;=========================================================================================================
	;LEITURA (CONFIGURACAO DO EMULADOR)
	;$KEYBOARD_EMU_SAIR = IniRead($SISTEMA_INI, "configuracao", "SAIR_EMULADOR", "")
	$SEND_EMU_COIN 		= IniRead($SISTEMA_INI, "configuracao", "FICHA_BT", "5")
	$READ_CONTADOR 		= (Int(IniRead($SISTEMA_INI, "configuracao", "CONTADOR", 0)) > 0) ? True : False

	;MODO FICHA
	Global $MODO_FICHA	= (Int(IniRead($SISTEMA_INI, "configuracao", "MODOFICHAS", 0)) > 0) ? True : False
	If Not $MODO_FICHA Then $SEND_EMU_COIN = False         ;Desativa o envio de ficha, caso esteja em modo tempo


	;=========================================================================================================
	;MODO EMULADOR NORMAL
	;=========================================================================================================
	Local $iExtra = ($READ_EXE = "xebra.exe")? "'": "'"
	$READ_COMAND1 = ($READ_COMAND1 <> "")? $READ_COMAND1 & Chr(32) : ""
	$READ_COMAND2 = ($READ_COMAND2 <> "")? Chr(32) & $READ_COMAND2 : ""
	$COMMAND_LINE =	$READ_COMAND1 & _
			Chr(34) 		& _
			$READ_GAME_PATH & _
			Chr(34) 		& _
			$READ_COMAND2

	;=========================================================================================================
	;MODO RETROARCH
	;=========================================================================================================
	Local $GET_PATH_GAME = _PathSplit($READ_GAME_PATH, $sDrive, $sDir, $sFileName, $sExtension)
	If @error Then Return SetError(-3,0,"Não foi encontrado o local do emulador, pasta invalida!")

	If $RETROARCH_MOD Then
		$RETRO_MULT_CORE = (Int(IniRead($SISTEMA_INI, "RETROARCH", "MULT_CORES", 0)) > 0) ? True : False
		$RETRO_EXTENS 	 = ($GET_PATH_GAME[4] = "") ? "" : StringTrimLeft($GET_PATH_GAME[4], 1)

		If $RETRO_MULT_CORE Then
			$RETRO_CORE = IniRead($SISTEMA_INI, "MULTI_CORES",  $RETRO_EXTENS, "")
			$RETRO_CONF = IniRead($SISTEMA_INI, "MULTI_CONFIG", $RETRO_EXTENS, "retroarch.default.cfg")
		Else
			$RETRO_CORE = IniRead($SISTEMA_INI, "RETROARCH", "SINGLE_CORE", "")
			$RETRO_CONF = IniRead($SISTEMA_INI, "RETROARCH", "SINGLE_CONF", "retroarch.default.cfg")
		EndIf

		;RETROARCH
		$COMMAND_LINE = $READ_COMAND1 		& _
						Chr(32) & Chr(34) 	& _
						$READ_GAME_PATH 	& _
						Chr(34) & Chr(32) 	& _
						" -L cores\" 		& _
						$RETRO_CORE 		& _
						Chr(32) 			& _
						"-c " & $RETRO_CONF & _
						Chr(32) 			& _
						$READ_COMAND2
	EndIf

	;=========================================================================================================
	;MODO PC / TAITO
	;=========================================================================================================
	If $GAME_PCTAITO Then
		Local $GAME_PCTAITO_INI = $SISTEM_DIR & "\ini\Games.ini"
		Local $READ_GAME_NAME 	= $GET_PATH_GAME[3]
		Local $READ_GAME_LNK	= FileGetShortcut($READ_GAME_PATH)

		If @error or Not IsArray($READ_GAME_LNK) Then Return SetError(-3,0,"Atalho selecionado não existe")

		Local $GET_PATH_LNK = _PathSplit($READ_GAME_LNK[0], $sDrive, $sDir, $sFileName, $sExtension)
		If @error or Not IsArray($GET_PATH_LNK) Then Return SetError(-3,0,"Não foi encontrado o caminho no atalho selecionado")

		$READ_EXE 				= $GET_PATH_LNK[3] & $GET_PATH_LNK[4]  ;IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME, "exe", "")
		$GET_PATH_GAME 			= $GET_PATH_LNK[1] & $GET_PATH_LNK[2]  ;String(IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME, "PASTA", ""))
		$READ_LOADER 			= ($READ_GAME_LNK[2] = "")? 0 : $READ_GAME_LNK[2]
		$SEND_EMU_COIN 			= IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME, 		"FICHA_BT", "5")
		$READ_COMAND1 			= IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME, 		"COMANDO1", "")
		$READ_COMAND2 			= IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME, 		"COMANDO2", "")
		$READ_JOYTOKEY			= (int(IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME,  "JOYTOKEY", "0")) <= 0)? False : _JOYTOKEY_INIT($READ_GAME_NAME)
		$READ_CONTADOR 			= (Int(IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME,  "CONTADOR", 0)) > 0) ? True : False
		$AUTO_ENVIO_FICHA 		= Int(IniRead($GAME_PCTAITO_INI, $READ_GAME_NAME,   "FICHA_AUTOENVIO", 0))
		If $READ_EXE 			= "" Then Return SetError(-4,0,"O executavel do jogo não foi informado pelo atalho, por favor, crie um atalho novo para o jogo selecionado")                                              ;Sai se o executavel não for informado
		If $GET_PATH_GAME 		= "" Then Return SetError(-5,0,"O executavel informado não existe!")                                        ;Sai se a pasta do jogo não for informada

		Local $GetFolderPath 	= FileGetShortName(_PathFull($GET_PATH_GAME))
		If Not FileExists($GetFolderPath) Then Return SetError(-6,0,"A pasta do executavel do jogo selecionado não existe! Por favor, recrie o atalho do jogo selecionado")         ;Sai se o executavel não existir
		$READ_EXE_PATH 			= $GetFolderPath   ;Pega a pasta do jogo Ex: "C:/jogo/cuphead/"
		if not FileExists($READ_EXE_PATH & $READ_EXE) Then Return SetError(-4,0,"O executavel do jogo não existe, verifique o atalho do jogo selecionado")

		;Verifica se o loader existe
		If Not FileExists($READ_EXE_PATH & "\" & $READ_LOADER) Then
			;PC/TAITO (SEM LOARDER)
			$COMMAND_LINE = $READ_COMAND1 	  & _
							Chr(32) & Chr(34) & _
							$READ_LOADER 	  & _
							Chr(34) & Chr(32) & _
							$READ_COMAND2

			$READ_LOADER = False
		Else
			;PC/TAITO (COM LOARDER)
			$COMMAND_LINE = $READ_COMAND1 	  & _
							Chr(32) & Chr(34) & _
							$READ_LOADER 	  & _
							Chr(34) & Chr(32) & _
							$READ_COMAND2

		EndIf
	EndIf
	;DESATIVADO - DEU UM BUG DE MULT-EXECUÇÃO DE AUDIO. Aparentemente não se pode executar esse audio junto com  outro de ficha
;~ 	if ($MODO_FICHA and $AUTO_ENVIO_FICHA > 0) Then _SOUND_PLAYSOUND($SND_AVISO) ;INFORMA QUE A FICHA SERÁ AUTO ENVIADA
	Return 1
EndFunc   ;==>_Start_ComandLine

Func _LOG_SYSTEM()
		Local $GET_TIPO_SYS   = ($MODO_FICHA > 0) ? "FICHA" : "TEMPO"
		Local $INFO_TAG_EMULE = False
		If Not $GAME_PCTAITO And Not $RETROARCH_MOD Then $INFO_TAG_EMULE = True

		Local $iGET0 = Int(Round($AUTO_ENVIO_FICHA / 1000))
		Local $iGET1 = Int(Round(($AUTO_ENVIO_FICHA - (_TICK_UPDATE() - $RETTICK_AUTO_ENVIO)) / 1000))
		Local $iGET2 = ($SKIP_INTRO = True or $iGET1 < 0) ?  $iGET0 : $iGET1
		Local $iGET_AUTO_ENVIO = StringFormat("%02d", $iGET2)  & "/" & StringFormat("%02d", Int($iGET0))  & " Segundos"
		if $iGET0 <= 0 Then $iGET_AUTO_ENVIO = "Desativado"


		Local $iTg = Int($RETTICK_TIME_PLAY - _TICK_UPDATE())
		Local $Calculado = _CALCULA_TIMER($iTg)
		if $iTg < 0 or $MODO_FICHA or Not $INICIA_COBRANCA Then $Calculado = "--:--:--"

		Local $GET6 = _GET_COINS_ALL() / _GET_VALUE_COIN()

		Local $GET_OCIOSO_STATUS = Int(Round(($TIME_SAIR_OCIOSO - (_TICK_UPDATE() - $RETTICK_OCIOSO)) / 60000))
		Local $GET_OCIOSO_TOTAL  = Int(Round(Int(Number($TIME_SAIR_OCIOSO / 60000) )))
		Local $GET_OCIOSO_RESULT = StringFormat("%02d", $GET_OCIOSO_STATUS) & "/" & StringFormat("%02d",$GET_OCIOSO_TOTAL) & " Minutos"
		if $GET_OCIOSO_TOTAL <= 0 Then $GET_OCIOSO_RESULT = "Desativado"
		ToolTip( _
				"-------------------------------------" & @CRLF & _
				"MODO FICHA:		" & $GET_TIPO_SYS & @CRLF & _
				"PAY TO PLAY:		" & $PAY_TO_PLAY & @CRLF & _
				"AUTO ENVIO FICHA:	" & $iGET_AUTO_ENVIO & @CRLF & _
				"SAIR OCIOSO:		" & $GET_OCIOSO_RESULT & @CRLF & _
				"-------------------------------------" & @CRLF & _
				"SISTEMA			" & $EMULE_NAME & @CRLF & _
				"Jogo:			" & $READ_GAME_PATH & @CRLF & _
				"Emulador:		" & $READ_EXE & @CRLF & _
				"Emulador Pasta:		" & $READ_EXE_PATH & @CRLF & _
				"COMANDO1:		" & $READ_COMAND1 & @CRLF & _
				"COMANDO2:		" & $READ_COMAND2 & @CRLF & _
				"Linha de comando:	" & $COMMAND_LINE & @CRLF & _
				"-------------------------------------" & @CRLF & _
				"Mult-Core:		" & $RETRO_MULT_CORE & @CRLF & _
				"CORE:			" & $RETRO_CORE & @CRLF & _
				"CONF:			" & $RETRO_CONF & @CRLF & _
				"-------------------------------------" & @CRLF & _
				"JOGOS DE PC/TAITO	" & $GAME_PCTAITO & @CRLF & _
				"RETROARCH		" & $RETROARCH_MOD & @CRLF & _
				"EMULADOR NORMAL	" & $INFO_TAG_EMULE & @CRLF & _
				"-------------------------------------" & @CRLF & _
				"BOTÃO ENVIAR FICHA:	" & "Botão " & $SEND_EMU_COIN & " do teclado" & @CRLF & _
				"FICHAS:			" & StringFormat("%02d", Int($GET6)) & @CRLF & _
				"TEMPO:			" & $Calculado & @CRLF & _
				"", -1500, 100, "LOG MENU", 1)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALCULA_TIMER
; Description ...:
; Syntax ........: _CALCULA_TIMER($timer)
; Parameters ....: $timer               - a dll struct value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALCULA_TIMER($timer)
	Local $hour = Int(Mod($timer, 86400000) / 3600000)
	Local $min = Int(Mod(Mod($timer, 86400000), 3600000) / 60000)
	Local $sec = Int(Mod(Mod(Mod($timer, 86400000), 3600000), 60000) / 1000)
	Local $mostra_texto = StringFormat("%02d:%02d:%02d", $hour, $min, $sec)
	Return $mostra_texto
EndFunc   ;==>_CALCULA_TIMER

; #FUNCTION# ====================================================================================================================
; Name ..........: _Clear_hotkeys
; Description ...:
; Syntax ........: _Clear_hotkeys()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Clear_hotkeys()
	Send("{INS UP}")
	Send("{NUMPAD9 UP}")
	Send("{NUMPAD8 UP}")
	Send("{NUMPAD7 UP}")
	Send("{NUMPAD6 UP}")

;~ 	Send("{LCTRL}")
;~ 	Send("{RCTRL}")
;~ 	Send("{LALT}")
;~ 	Send("{RALT}")
;~ 	Send("{LSHIFT}")
;~ 	Send("{RSHIFT}")

EndFunc   ;==>_Clear_hotkeys



; #FUNCTION# ====================================================================================================================
; Name ..........: _Clean_debug
; Description ...:
; Syntax ........: _Clean_debug()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Clean_debug()
	_write_debug_fichas()
	If ($PAY_TO_PLAY and $AUTO_ENVIO_FICHA <= 0) Then _write_debug_fichas(_GET_COINS_ALL() / _GET_VALUE_COIN())
	_write_debug_sair()
	_write_debug_trans()
	_write_debug_autoenvio()
	_write_debug_timer()
	_write_debug_ocioso()
EndFunc   ;==>_Clean_debug

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_sair
; Description ...:
; Syntax ........: _write_debug_sair([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_sair($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"
	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_01", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_01", StringFormat("%02d", Int($iRet)))
EndFunc   ;==>_write_debug_sair

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_trans
; Description ...:
; Syntax ........: _write_debug_trans([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_trans($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"
	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_02", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_02", StringFormat("%02d", Int($iRet)))
EndFunc   ;==>_write_debug_trans

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_autoenvio
; Description ...:
; Syntax ........: _write_debug_autoenvio([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_autoenvio($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"
	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_03", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_03", StringFormat("%02d", Int($iRet)))
EndFunc   ;==>_write_debug_autoenvio

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_fichas
; Description ...:
; Syntax ........: _write_debug_fichas([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_fichas($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"

	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_04", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_04", StringFormat("%02d", Int($iRet)))
EndFunc   ;==>_write_debug_fichas

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_timer
; Description ...:
; Syntax ........: _write_debug_timer([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_timer($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"
	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_06", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_06", $iRet)
EndFunc   ;==>_write_debug_timer

; #FUNCTION# ====================================================================================================================
; Name ..........: _write_debug_ocioso
; Description ...:
; Syntax ........: _write_debug_ocioso([$iTime = "-1"])
; Parameters ....: $iTime               - [optional] an integer value. Default is "-1".
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _write_debug_ocioso($iTime = "-1")
	Local $iRet = $iTime
	If $iRet = "-1" Then $iRet = "-1"
	If IniRead($__DEBUG_FILER, "DEBUG", "DEBUG_05", "-1") <> $iTime Then IniWrite($__DEBUG_FILER, "DEBUG", "DEBUG_05", StringFormat("%02d", Int($iRet)))
EndFunc   ;==>_write_debug_ocioso


