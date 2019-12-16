
; #FUNCTION# ====================================================================================================================
; Name ..........: _INTARNAL_TEST_JOYSTICK
; Description ...:
; Syntax ........: _INTARNAL_TEST_JOYSTICK()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _INTARNAL_TEST_JOYSTICK($indexJoy)

;Inicia os Joysticks
_JOY_START($__DIR_SDL)
if @error Or Not FileExists($__DIR_SDL) then
	_CALL_WRITE_LOG("SDL1.dll não foi encontrada no diretorio do sistema")
	Return
EndIf


Global $JOYSTK[3] 	  = [_JOY_OPEN($___JOY_1), _JOY_OPEN($___JOY_2), _JOY_OPEN($___JOY_3)]
Global $looping	  	  = 1
Global $font	  	  = "Arial"
Global $JOY_ATIVO 	  = 0
Global $o__JOY_NUMBER = _JOY_GET_NUMBER_BUTTONS($JOYSTK[0])


Local $iJOY_NUM 	  = _JOY_GET_NUMBER()
if $iJOY_NUM  <= 0 Then
	MostraMensa(@LF & "Nenhum joystick conectado ao sistema", 10000)
	Return
EndIf

if $iJOY_NUM < $indexJoy Then
	if $indexJoy = 2 Then
		MostraMensa(@LF & "O Joystick 2 não foi encontrado", 10000)
		Return
	ElseIf $indexJoy = 3 Then
		MostraMensa(@LF & "O Joystick 3 não foi encontrado", 10000)
		Return
	EndIf
EndIf


If $indexJoy = 1 and _JOY_GET_NUMBER() >= 1 then $JOY_ATIVO = 0
If $indexJoy = 2 and _JOY_GET_NUMBER() >= 2 then $JOY_ATIVO = 1
If $indexJoy = 3 and _JOY_GET_NUMBER() >= 3 then $JOY_ATIVO = 2

Local $PRESSED_BUTTON1[29]= [True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True,True]


FileInstall("..\img\test_joysticks\red_normal.png",   @TempDir & "/~tag1a",1)
FileInstall("..\img\test_joysticks\green_normal.png", @TempDir & "/~tag1b",1)
FileInstall("..\img\test_joysticks\disable.png",      @TempDir & "/~tag1c",1)


FileInstall("..\img\test_joysticks\dir_joy.png",    @TempDir & "/~tag1d",1)
FileInstall("..\img\test_joysticks\background.png", @TempDir & "/~tag1e",1)

FileInstall("..\img\test_joysticks\alpha.png",        @TempDir & "/~tag1f",1)


Const $imagem_botao_vermelho		= @TempDir & "/~tag1a"
Const $imagem_botao_verde			= @TempDir & "/~tag1b"
Const $imagem_botao_disable			= @TempDir & "/~tag1c"


Const $imagem_background_Direc		= @TempDir & "/~tag1d"
Const $imagem_background_gui		= @TempDir & "/~tag1e"
Const $imagem_botao_alpha		= @TempDir & "/~tag1f"


;=========================================>
;GUI PRINCIPAL
;=========================================>
Local $GUI_PRINCIPAL=GUICreate("", 739,414, -1, -1, $WS_POPUP,BitOR($WS_EX_COMPOSITED,$WS_EX_TOPMOST,$WS_EX_TOOLWINDOW))
If @error Then
	MostraMensa(@LF & "Não foi possivel cria a interface", 10000)
	Return
EndIf
;--------------------------------------------------------------------------------------------------------------------------
;INFO TELEFONE
GUICtrlCreateLabel($__FACE,15,10,300,20)
GUICtrlSetFont(-1,13)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor(-1,0xF0F0E1)
GUICtrlCreateLabel(" Tel: " & $__TELEF,15,30,300,20)
GUICtrlSetFont(-1,13)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor(-1,0xF0F0E1)
;--------------------------------------------------------------------------------------------------------------------------
;INFO SAIR
Global $__BOTAO_SAIR_1 = $__READ_SAIR_KEY[0][1]
Global $__BOTAO_SAIR_2 = $__READ_SAIR_KEY[1][1]

GUICtrlCreateLabel("BOTÃO " & $__BOTAO_SAIR_1+1 & " + BOTÃO " & $__BOTAO_SAIR_2+1 & " = SAIR",250,385,420,20)
GUICtrlSetFont(-1,15)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor(-1,0xFFFF00)
;--------------------------------------------------------------------------------------------------------------------------
;DUMMY
GUICtrlCreateButton("", 1, 1, 1, 1,BitOR($GUI_HIDE,$GUI_DISABLE))
;--------------------------------------------------------------------------------------------------------------------------
;COMBO BOX
Local $Combo_box = GUICtrlCreateCombo("", 8, 54, 230, 25,BitOR($CBS_DISABLENOSCROLL ,$CBS_DROPDOWN  ,$CBS_SIMPLE))
For $i = 0 To _JOY_GET_NUMBER()-1
	GUICtrlSetData($Combo_box,$i & " - " & StringReplace(_JOY_GET_NAME($i),"  "," "),$JOY_ATIVO & " - " & StringReplace(_JOY_GET_NAME($JOY_ATIVO),"  "," "))
Next
;--------------------------------------------------------------------------------------------------------------------------
;LABELS / BOTÕES
Global $PIC_IMG[20], $LABEL_NUMB[20]
Local $iLeft    = 309
Local $iLeft_SV = $iLeft
Local $itop     = 40

For $i=0 to UBound($PIC_IMG)-1
	$LABEL_NUMB[$i] = GUICtrlCreateLabel($i+1,$iLeft,$itop,66,66,$SS_CENTER+$SS_CENTERIMAGE)
	GUICtrlSetFont(-1,25)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor(-1,0xF0F0E1)
	GUICtrlSetState(-1,$GUI_HIDE)
	$PIC_IMG[$i]= _GUICtrlPic_Create($imagem_botao_disable,$iLeft,$itop, 66,66)
	GUICtrlSetState(-1,$GUI_HIDE)
	if $i = 4 or $i = 9 or $i = 14 Then
		$iLeft = $iLeft_SV
		$itop  += 79
	Else
		$iLeft += 79
	EndIf
Next
;ESCONDE/MOSTRA - Label dependendo da quantidade de botões no Joy
_SHOW_HIDE_BUTTONS($o__JOY_NUMBER)
;-------------------------------------------------------------------------------------
;SETAS_MANETE
Local $iLeft_manete   = 90
Local $iTop_manete    = 138

Local $i_MANETE = _GUICtrlPic_Create($imagem_botao_vermelho,$iLeft_manete,$iTop_manete, 92,92)
_GUICtrlPic_Create($imagem_botao_alpha,$iLeft_manete,$iTop_manete, 92,92)
;BK_MANETE
_GUICtrlPic_Create($imagem_background_Direc,    52,100, 166,166)
GUICtrlSetState (-1, $GUI_DISABLE)
;BACKGROUND
_GUICtrlPic_Create($imagem_background_gui,0,0,739,414)
GUICtrlSetState (-1, $GUI_DISABLE)
;-------------------------------------------------------------------------------------
;mostra a GUI
GUISetState (@SW_SHOW,$GUI_PRINCIPAL)
Local $aPos = WinGetPos($GUI_PRINCIPAL)
Global $g_iWidth = $aPos[2]
Global $g_iHeight = $aPos[3]
Local $hRgn = _WinAPI_CreateRoundRectRgn(0, 0, $g_iWidth, $g_iHeight, 78, 78)
        _WinAPI_SetWindowRgn($GUI_PRINCIPAL, $hRgn)
;------------------------------------------------------------------------------------
While sleep(50)
	_JOY_UPDATE($JOYSTK)

        Switch GUIGetMsg()
            Case $GUI_EVENT_CLOSE
                ExitLoop
			Case $Combo_box
				Local $combo=GUICtrlRead($Combo_box)
				$JOY_ATIVO = Int(StringLeft($combo,1))
				Global $o__JOY_NUMBER = _JOY_GET_NUMBER_BUTTONS($JOYSTK[$JOY_ATIVO])
				_SHOW_HIDE_BUTTONS($o__JOY_NUMBER)
		EndSwitch

		;BOTÕES 1/20
		For $i=0 To UBound($PIC_IMG)-1
			If _JOY_GET_BUTTON($JOYSTK[$JOY_ATIVO],$i) Then
				if $PRESSED_BUTTON1[$i]=True Then
					if $i = $__BOTAO_SAIR_1 or $i = $__BOTAO_SAIR_2 Then
						_GUICtrlPic_SetPic($PIC_IMG[$i], $imagem_botao_vermelho)
					Else
						_GUICtrlPic_SetPic($PIC_IMG[$i], $imagem_botao_verde)
					EndIf
				EndIf
					$PRESSED_BUTTON1[$i]=False
			Elseif $PRESSED_BUTTON1[$i]=False Then
				_GUICtrlPic_SetPic($PIC_IMG[$i], $imagem_botao_disable)
				$PRESSED_BUTTON1[$i]=True
			EndIf
		Next

		Select
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) < (-5000) And Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) > (5000)) and Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) < (-5000))
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete,$iTop_manete-25)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) > (5000) And Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) > (5000)) and Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) < (-5000))
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete,$iTop_manete+25)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) < (-5000) and not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) < (-5000)) and Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) > (5000))
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete-25,$iTop_manete)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) > (5000) and not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) < (-5000)) and Not (_JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) > (5000))
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete+25,$iTop_manete)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) > (5000) and _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) > (5000)
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete+25,$iTop_manete+25)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) < (-5000) And _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) > (5000)
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete+25,$iTop_manete-25)
			Case _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) < (-5000) And _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) < (-5000)
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete-25,$iTop_manete-25)
			Case  _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 0) < (-5000) and _JOY_GET_AXIS_BUTTONS($JOYSTK[$JOY_ATIVO], 1) > (5000)
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete-25,$iTop_manete+25)
			Case _JOY_GET_BUTTON($JOYSTK[$JOY_ATIVO],$__BOTAO_SAIR_1) and _JOY_GET_BUTTON($JOYSTK[$JOY_ATIVO],$__BOTAO_SAIR_2)
				While _JOY_GET_BUTTON($JOYSTK[$JOY_ATIVO],$__BOTAO_SAIR_1) And _JOY_GET_BUTTON($JOYSTK[$JOY_ATIVO],$__BOTAO_SAIR_2)
				_JOY_UPDATE($JOYSTK)
				WEnd
				ExitLoop
			case Else
				ControlMove($GUI_PRINCIPAL,"",$i_MANETE,$iLeft_manete,$iTop_manete)
		EndSelect
Wend
GUIDelete($GUI_PRINCIPAL)
Return
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _SHOW_HIDE_BUTTONS
; Description ...:
; Syntax ........: _SHOW_HIDE_BUTTONS($iNumber)
; Parameters ....: $iNumber             - an integer value.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SHOW_HIDE_BUTTONS($iNumber)
	For $i=0 To UBound($PIC_IMG)-1
		if $i < $iNumber Then
			GUICtrlSetState($LABEL_NUMB[$i],$GUI_SHOW)
			GUICtrlSetState($PIC_IMG[$i],$GUI_SHOW)
		Else
			GUICtrlSetState($LABEL_NUMB[$i],$GUI_HIDE)
			GUICtrlSetState($PIC_IMG[$i],$GUI_HIDE)
		EndIf
	Next

EndFunc