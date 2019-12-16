; #FUNCTION# ====================================================================================================================
; Name ..........: Remover_emulador
; Description ...:
; Syntax ........: Remover_emulador()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func Remover_emulador()

Global Const $ADICIONAR ="Adicionado"
Global Const $REMOVER   ="Removido"

Global $PRETO 	 = 0x000000
Global $VERMELHO = 0xff0000
Global $BRANCO 	 = 0xffffff
Global $VERDE 	 = 0x00ff00
Global $FONTE 	 = "arial"
Global $KEY_UP 	 = 26
Global $KEY_DOWN = 28
Global $KEY_LEFT = 25
Global $KEY_RIGHT= 27
Global $KEY_ENTER= "0D"
Global $LOOPING  = True

;Imagens
FileInstall("..\img\conf_menu\bkgui2.png",   @TempDir & "/~tagx2a",1)
FileInstall("..\img\conf_menu\bkselect6.png",@TempDir & "/~tagx2b",1)

Global $BKGround = @TempDir & "/~tagx2a"
Global $BRSELECT2= @TempDir & "/~tagx2b"

Global $i_Label_NAME[23], $i_Label_INFO[23]
Global $LEFT_SETA 				= 5
Global $LEFT_SETA_SV 			= $LEFT_SETA
Global $TOP_SETA   				= 83
Global $TOP_SETA_SV				= $TOP_SETA
Global $ENTER_PRESSED			= False
Global $ii__INI_GET_ALL_EMULE 	= IniReadSection(_CONST_ADVMENU_INI(),"REMOVER_EMULADOR")
Global $i_LBL_LEFT				= 45
Global $i_LBL_TOP 				= 88
Global $i_LBL_TOP_SV 			= $i_LBL_TOP
Global $ITEM_SELECTED 			= 1
Global $ITEM_COLUNA_SELECTED	= 1
Global $MAX_ITEM				= UBound($ii__INI_GET_ALL_EMULE)-1
Global $FLAG[4] 				= [False,False,False,False]
Global $LSALVAR,$LSAIR
Global $W_SETA = 300
Global $H_SETA = 30
Global $estado

if $MAX_ITEM > 20 Then $MAX_ITEM = 20
If $MAX_ITEM <= 0 Then
	MostraMensa(@LF & "Nenhum emulador foi encontrado e configigurado no sistema", 10000)
	Return
EndIf
;===========================>
;cria a janela principal
;===========================>
Local $GUI_PRINCIPAL=GUICreate("",  640, 568, -1, -1, BitOR($WS_DISABLED,$WS_POPUP),BitOR($WS_EX_COMPOSITED,$WS_EX_TOPMOST))
If @error Then
	MsgBox(0,"Erro","Não foi possivel cria a interface",10)
	Return
EndIf
GUISetIcon(@ScriptFullPath,99)

;===============================>
;cria as opões a serem mostradas
;===============================>
;Telefone
GUICtrlCreateLabel ("E-Mail: " & $__EMAIL  & "/    Tel: " & $__TELEF,  110, 63, 550)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor ( -1, $VERMELHO)
GUICtrlSetFont(-1,11,700,Default,$FONTE)


Global $i_ALERTA = GUICtrlCreateLabel("Adicione pelo menos um emulado ao sistema!",0,470,640,20,$SS_CENTER)
GUICtrlSetState(-1,$GUI_HIDE)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor ( -1, $VERMELHO)
GUICtrlSetFont(-1,11,700,Default,$FONTE)
;-------------------------------------------------------------------------------------
;CRIA O LABEL
For $i=1 To UBound($ii__INI_GET_ALL_EMULE)-1
	;Label Nome
	$i_Label_NAME[$i]=GUICtrlCreateLabel ($ii__INI_GET_ALL_EMULE[$i][0],  $i_LBL_LEFT, $i_LBL_TOP, 300)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( -1, $BRANCO)
	GUICtrlSetFont(-1,14,700,Default,$FONTE)
	GUICtrlSetColor(-1,0)
	;Label Add/Remov
	Local $iRead = IniRead(_CONST_ADVMENU_INI(),"REMOVER_EMULADOR",$ii__INI_GET_ALL_EMULE[$i][0],0)
	Local $RET = $REMOVER
	If $iRead = 1 Then $RET = $ADICIONAR

	$i_Label_INFO[$i]=GUICtrlCreateLabel ($RET,  $i_LBL_LEFT+170, $i_LBL_TOP, 85,20,$SS_CENTER)
	GUICtrlSetFont(-1,11,700,Default,$FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	;Imagem <===>
	_GUICtrlPic_Create($BRSELECT2,$i_LBL_LEFT+170,$i_LBL_TOP-3,85 ,25)
	;Cor do Add/Removido
	If $iRead = 1 Then
		GUICtrlSetColor ($i_Label_INFO[$i], $VERDE)
	Else
		GUICtrlSetColor ($i_Label_INFO[$i], $VERMELHO)
	EndIf
	;Reposicionamento dos itens
	If $i = 13 Then
		$i_LBL_TOP = $i_LBL_TOP_SV
		$i_LBL_LEFT+= 300
	Else
		$i_LBL_TOP += 30
	EndIf
Next

;-------------------------------------------------------------------------------------
;item22 Salvar e sair
$LSALVAR=GUICtrlCreateLabel ("SALVAR e SAIR",  0, 485, 640,35,$SS_CENTER+$SS_CENTERIMAGE)
		 GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
		 GUIctrlSetFont (-1,18, 800, -1, $FONTE)
;-------------------------------------------------------------------------------------
;item23 Salvar e sair
$LSAIR=GUICtrlCreateLabel ("SAIR",  0,525, 640,35,$SS_CENTER+$SS_CENTERIMAGE)
	   GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	   GUIctrlSetFont (-1,18, 800, -1, $FONTE)
;-------------------------------------------------------------------------------------
;SETA
Local $SETA=GUICtrlCreateLabel("",5, $TOP_SETA,$W_SETA,$H_SETA)
GUICtrlSetBkColor(-1,0x0059B2)
GUICtrlSetState(-1,$GUI_DISABLE )
;-------------------------------------------------------------------------------------
;BACKGROUND
_GUICtrlPic_Create($BKGround,0,0,640 ,568)
;-------------------------------------------------------------------------------------
GUISetState  ()
;========================>
;Inicia o Loop principal
;========================>

While $LOOPING
	Switch GUIGetMsg()
		Case $GUI_EVENT_CLOSE
			ExitLoop

	EndSwitch
	;up
	If (_IsPressed($KEY_UP)) And (($ITEM_SELECTED > 1 and $ITEM_COLUNA_SELECTED < 2) or ($ITEM_SELECTED > 14 And $ITEM_COLUNA_SELECTED > 1)) And ($FLAG[0]=False) Then
			$FLAG[0]=True
		if ($ITEM_SELECTED <= 13 and $ITEM_COLUNA_SELECTED < 2)  or ($ITEM_SELECTED <= $MAX_ITEM And $ITEM_COLUNA_SELECTED > 1) Then
			$ITEM_SELECTED-=1
			$TOP_SETA-=30
			$W_SETA = 300
			$H_SETA = 30
		Else
			if $ITEM_SELECTED = 27 Then
					if $MAX_ITEM < 13 Then
						$ITEM_SELECTED = $MAX_ITEM
						$TOP_SETA= ($MAX_ITEM -1) * 30 +$TOP_SETA_SV
					Else
						$ITEM_SELECTED = 13
						$TOP_SETA=12 * 30 +$TOP_SETA_SV
					EndIf
					$LEFT_SETA = $LEFT_SETA_SV

				if $ITEM_COLUNA_SELECTED < 2 Then
					if $MAX_ITEM < 13 Then
						$ITEM_SELECTED = $MAX_ITEM
					Else
						$ITEM_SELECTED = 13
					EndIf
				Else
					$TOP_SETA = $TOP_SETA_SV + ($MAX_ITEM-14)*30
					$LEFT_SETA+=300
					$ITEM_SELECTED = $MAX_ITEM
				EndIf
					$W_SETA = 300
					$H_SETA = 30
			ElseIf $ITEM_SELECTED = 28 Then
				$TOP_SETA=485
				$LEFT_SETA = $LEFT_SETA_SV
				$ITEM_SELECTED = 27
				$W_SETA = 630
				$H_SETA = 40
			EndIf
		EndIf
		GUICtrlSetPos($seta, $LEFT_SETA, $TOP_SETA,$W_SETA,$H_SETA)
	EndIf
	If Not (_IsPressed($KEY_UP)) And ($FLAG[0]=True) Then $FLAG[0]=False
 	;down
	If (_IsPressed($KEY_DOWN)) And ($FLAG[1]=False) Then
 		$FLAG[1]=True
		if ($ITEM_SELECTED < $MAX_ITEM and $ITEM_COLUNA_SELECTED < 2) And ($ITEM_SELECTED < 13 and $ITEM_COLUNA_SELECTED < 2)  or ($ITEM_SELECTED < $MAX_ITEM And $ITEM_COLUNA_SELECTED > 1) Then
			$ITEM_SELECTED+=1
			$TOP_SETA+=30
			$W_SETA = 300
			$H_SETA = 30
		Else
			if $ITEM_SELECTED < 27 Then
				$TOP_SETA=485
				$LEFT_SETA = $LEFT_SETA_SV
				$ITEM_SELECTED = 27
				$W_SETA = 630
				$H_SETA = 40
			Elseif $ITEM_SELECTED = 27 Then
				$TOP_SETA=523
				$LEFT_SETA = $LEFT_SETA_SV
				$ITEM_SELECTED = 28
				$W_SETA = 630
				$H_SETA = 40
			EndIf
		EndIf
		GUICtrlSetPos($seta, $LEFT_SETA, $TOP_SETA,$W_SETA,$H_SETA)
	EndIf
	If Not(_IsPressed($KEY_DOWN)) And ($FLAG[1]=True) Then $FLAG[1]=False
	;left
	If (_IsPressed($KEY_LEFT)) And $ITEM_COLUNA_SELECTED > 1 And $MAX_ITEM > 13  And $ITEM_SELECTED < 27 And($FLAG[2]=False) Then
 		$FLAG[2]=True
		$LEFT_SETA-=300
		$ITEM_COLUNA_SELECTED -=1
		GUICtrlSetPos($seta, $LEFT_SETA, $TOP_SETA)
		$ITEM_SELECTED-=13
	EndIf
	If Not(_IsPressed($KEY_LEFT)) And ($FLAG[2]=True) Then $FLAG[2]=False

	;right
	If (_IsPressed($KEY_RIGHT)) And $ITEM_COLUNA_SELECTED < 2  And $ITEM_SELECTED < 27 And $MAX_ITEM > 13 And ($FLAG[3]=False) Then
 		$FLAG[3]=True
		$ITEM_COLUNA_SELECTED +=1
		if $ITEM_SELECTED + 13 > $MAX_ITEM Then
			$LEFT_SETA+=300
			$ITEM_SELECTED = $MAX_ITEM
			$TOP_SETA = $TOP_SETA_SV + ($MAX_ITEM-14)*30

		Else
			$LEFT_SETA+=300
			$ITEM_SELECTED+=13

		EndIf
		GUICtrlSetPos($seta, $LEFT_SETA, $TOP_SETA)
	EndIf
	If Not(_IsPressed($KEY_RIGHT)) And ($FLAG[3]=True) Then $FLAG[3]=False



	If _IsPressed($KEY_ENTER) And ($ENTER_PRESSED=False) Then
		$ENTER_PRESSED=True
		If $ITEM_SELECTED <= 20 Then
			$estado=GUICtrlRead($i_Label_INFO[$ITEM_SELECTED])
			If $estado <> $REMOVER Then
				GUICtrlSetData($i_Label_INFO[$ITEM_SELECTED],$REMOVER)
				GUICtrlSetColor ($i_Label_INFO[$ITEM_SELECTED], $VERMELHO)
				$ii__INI_GET_ALL_EMULE[$ITEM_SELECTED][1] = 0
			Else
				GUICtrlSetData($i_Label_INFO[$ITEM_SELECTED],$ADICIONAR)
				GUICtrlSetColor ($i_Label_INFO[$ITEM_SELECTED], $VERDE)
				$ii__INI_GET_ALL_EMULE[$ITEM_SELECTED][1] = 1
			EndIf
			ElseIf $ITEM_SELECTED = 27 Then
			SalvaDados_Lista()
		ElseIf $ITEM_SELECTED = 28 Then
			ExitLoop
		EndIf
	EndIf
	If Not(_IsPressed($KEY_ENTER)) And ($ENTER_PRESSED=True) Then $ENTER_PRESSED=False

Wend
GUIDelete($GUI_PRINCIPAL)
EndFunc

Func SalvaDados_Lista()
	Local $EMU="ARCADE"
	Local $Linha = 1, $Chars = "", $Search
	Local $File = FileOpen(_CONST_ADVMENU_RC(), 0)
	Local $iContagem = 0

	For $i=1 To $MAX_ITEM
		if $ii__INI_GET_ALL_EMULE[$i][1] = 1 Then $iContagem += 1
	Next
	If $iContagem <= 0 Then
		GUICtrlSetState($i_ALERTA,$GUI_SHOW)
		Return
	EndIf
	GUICtrlSetState($i_ALERTA,$GUI_HIDE)
	Local $iContagem = 0
	For $i=1 To $MAX_ITEM
		if $ii__INI_GET_ALL_EMULE[$i][1] = 1 Then
			$EMU = $ii__INI_GET_ALL_EMULE[$i][0]
			ExitLoop
		Else
			$iContagem += 1
		EndIf
	Next


	For $i = 1 To $MAX_ITEM
		Local $estado=GUICtrlRead($i_Label_INFO[$i])
		If $estado = $ADICIONAR Then
			IniWrite(_CONST_ADVMENU_INI(),"REMOVER_EMULADOR",$ii__INI_GET_ALL_EMULE[$i][0],1)
		Else
			IniWrite(_CONST_ADVMENU_INI(),"REMOVER_EMULADOR",$ii__INI_GET_ALL_EMULE[$i][0],0)
		EndIf
	Next

;muda o emulador padrão


If $File <> -1 Then
	While 1
		$Chars = FileReadLine($File, $Linha)
		If @error = -1 or $EMU="" Then ExitLoop
		$Search = StringInStr($Chars, "emulator_include") ; linha a ser procurada no arquivo
		If $Search <> 0 Then
			_FileWriteToLine(_CONST_ADVMENU_RC(), $Linha, 'emulator_include "' & $EMU & '"', True)
			ExitLoop
		EndIf
		$Linha += 1
	WEnd
EndIf
	FileClose($File)
	SoundPlay(_CONST_SOUND_FOLDER() & "salvadados.wav", 1)
	$LOOPING = False
EndFunc