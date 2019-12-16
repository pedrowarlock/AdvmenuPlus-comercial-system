

Global $i___GIF_FILES_SAVE,$i___GIF_FILES = 0,$i___GIF_CONTROL,$iFADE_FOLDER,$iFADE_DEFAULT_FOLDER
Global $iProgressPic, $iProgressGet = 0,$iProgressPic2,$iGetPercent,$iGetMSG

Global $iGUIS_AZUL    	= 0x0000FF
Global $iGUIS_VERDE   	= 0x00FF00
Global $iGUIS_AMARELO 	= 0xFFFF00
Global $iGUIS_BRANCO 	= 0xFFFFFF
Global $iGUIS_PRETO  	= 0x000000
Global $iGUIS_VERRMELHO = 0xFF0000
; #FUNCTION# ====================================================================================================================
; Name ..........: _POPUP_WINDOW_CREATE
; Description ...:
; Syntax ........: _POPUP_WINDOW_CREATE([$iFont = "Arial"[, $font_color = $iGUIS_VERRMELHO[, $background = $iGUIS_AZUL]]])
; Parameters ....: $iFont               - [optional] an integer value. Default is "Arial".
;                  $font_color          - [optional] a floating point value. Default is $iGUIS_VERRMELHO.
;                  $background          - [optional] a boolean value. Default is $iGUIS_AZUL.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _POPUP_WINDOW_CREATE($iFont = "Arial",$font_color = $iGUIS_VERRMELHO, $background = $iGUIS_PRETO)


		Local $hGUI = GUICreate("", 0, 0, 0,  0, BitOR($WS_POPUP, $WS_DISABLED), BitOR($WS_EX_TOPMOST, $WS_EX_TOOLWINDOW, $WS_EX_COMPOSITED), 0)
		GUISetBkColor($background , $hGUI)

		Local $iLabel = GUICtrlCreateLabel("", 0, 0, 0, 0,$SS_CENTER)
		GUICtrlSetBkColor(-1,$background)
		GUICtrlSetFont(-1,0, $FW_BOLD , -1, $iFont,  $CLEARTYPE_QUALITY)
		GUICtrlSetColor(-1, $font_color)

		WinSetTrans($hGUI,"", 190)
		GUISetState(@SW_HIDE, $hGUI)

		Local $iRetorno[] = [$hGUI, $iLabel, $iFont]
		Return $iRetorno
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _IDLE_WINDOW_REFRESH
; Description ...:
; Syntax ........: _IDLE_WINDOW_REFRESH($iHandle)
; Parameters ....: $iHandle             - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _POPUP_WINDOW_REFRESH($iHandle)
	if not IsArray($iHandle) then Return
	if not IsHWnd($iHandle[0]) then Return
	if not WinActive($iHandle[0]) Then

		WinSetOnTop($iHandle[0], "", 1)
	EndIf
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _IDLE_WINDOW_SHOW
; Description ...:
; Syntax ........: _IDLE_WINDOW_SHOW($iHandle[, $ishow = False])
; Parameters ....: $iHandle             - an integer value.
;                  $ishow               - [optional] an integer value. Default is False.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _POPUP_WINDOW_SHOW($iHandle,$ishow=False)
	if not IsArray($iHandle) then Return
	if not IsHWnd($iHandle[0]) then Return
	if $ishow Then
		GUISetState(@SW_SHOWNOACTIVATE, $iHandle[0])
	Else
		GUISetState(@SW_HIDE, $iHandle[0])
	EndIf
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _IDLE_WINDOW_SET_MSG
; Description ...:
; Syntax ........: _IDLE_WINDOW_SET_MSG($iHandle, $sTick)
; Parameters ....: $iHandle             - an integer value.
;                  $sTick               - a string value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _POPUP_WINDOW_SET_MSG($iHandle, $iMensagem = "", $sTick = 0, $_F_size = 0,$_PosX = 0, $_PosY = 0, $_largura = 0, $_altura = 0, $Linhas = 1)
	if not IsArray($iHandle) then Return
	if not $iHandle[0] or $sTick < 0 then Return

		_POPUP_WINDOW_SHOW($iHandle,True)
		_POPUP_WINDOW_REFRESH($iHandle)

		Local $SetTick  = StringReplace($iMensagem, "%s", $sTick,1)
		Local $JumpLine = StringReplace($SetTick, "/n", @CRLF,1)
		if $JumpLine <> GUICtrlRead($iHandle[1]) Then

			Local $iGet = WinGetPos($iHandle[0],"")
			if Not @error Then

				If int($iGet[0]) <> int($_PosX) or int($iGet[1]) <> int($_PosY) Then
					WinMove($iHandle[0],"",int($_PosX), int($_PosY), int($_largura), int($_altura*$Linhas))
					ControlMove($iHandle[0],"",$iHandle[1],0,0,int($_largura),int($_altura*$Linhas))
					GUICtrlSetFont($iHandle[1],$_F_size, $FW_BOLD , -1, $iHandle[2],  $CLEARTYPE_QUALITY)
				EndIf
			EndIf

			GUICtrlSetData($iHandle[1], $JumpLine)
		EndIf
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _WINDOW_DELETE
; Description ...:
; Syntax ........: _WINDOW_DELETE($iHandle)
; Parameters ....: $iHandle             - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _WINDOW_DELETE($iHandle)
	if not IsArray($iHandle) then Return
	if not IsHWnd($iHandle[0]) then Return
	_POPUP_WINDOW_SHOW($iHandle[0],False)
	GUIDelete($iHandle[0])
EndFunc




; #FUNCTION# ====================================================================================================================
; Name ..........: _SPLASH_SET_PROGRESS
; Description ...:
; Syntax ........: _SPLASH_SET_PROGRESS($iHandle, $progress, $imensagem)
; Parameters ....: $iHandle             - an integer value.
;                  $progress            - a pointer value.
;                  $imensagem           - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SPLASH_SET_PROGRESS($iHandle,$progress,$imensagem)
	if not IsHWnd($iHandle) Then Return

	Local $CPos = ControlGetPos($iHandle,"",$iProgressPic)
	if @error or GUICtrlRead($iGetPercent) = $progress &"%" then Return

	GUICtrlSetData($iGetMSG,$imensagem)
	Local $calculo = Int(($iProgressGet * $progress)/100)
	ControlMove($iHandle,"",$iProgressPic, $CPos[0], $CPos[1], $calculo , $CPos[3])
	ControlMove($iHandle,"",$iProgressPic2,$calculo+$CPos[0], $CPos[1]) ;Pontinha
	ControlMove($iHandle,"",$iGetPercent,$calculo+$CPos[0]-45,$CPos[1]-30)
	GUICtrlSetData($iGetPercent,$progress & "%")
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _SPLASH_LOAD
; Description ...:
; Syntax ........: _SPLASH_LOAD($iFade_folder, $iFade_default, $progress_x, $progress_y, $progress_w, $progress_h, $iIMG_3,
;                  $iIMG_4, $iIMG_5, $iIMG_6, $iIMG_7, $iGame, $iMode)
; Parameters ....: $iFade_folder        - an integer value.
;                  $iFade_default       - an integer value.
;                  $progress_x          - a pointer value.
;                  $progress_y          - a pointer value.
;                  $progress_w          - a pointer value.
;                  $progress_h          - a pointer value.
;                  $iIMG_3              - an integer value.
;                  $iIMG_4              - an integer value.
;                  $iIMG_5              - an integer value.
;                  $iIMG_6              - an integer value.
;                  $iIMG_7              - an integer value.
;                  $iGame               - an integer value.
;                  $iMode               - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SPLASH_LOAD(	$iFade_folder, _ 								;Background IMG
					$iFade_default, _ 								;Background IMG
					$progress_x, _	;Posição X  da barra de progresso
					$progress_y, _	;Posição Y  da barra de progresso
					$progress_w, _	;Largura da barra de progresso
					$progress_h, _
					$iIMG_3, _
					$iIMG_4, _
					$iIMG_5, _
					$iIMG_6, _
					$iIMG_7, _
					$iGame, _
					$iMode)

	$iFADE_FOLDER = $iFade_folder
	$iFADE_DEFAULT_FOLDER = $iFade_default


	Global $iProgressGet = $progress_h -34
	Local $iGuiSplash = GUICreate("",@DesktopWidth,@DesktopHeight,0,0,BitOR($WS_POPUP, $WS_DISABLED), BitOR($WS_EX_TOOLWINDOW,$WS_EX_COMPOSITED), 0)
	GUISetBkColor(0,$iGuiSplash)
	_WinAPI_ShowCursor (False)

	;====================================== Barra de progresso =================================>
	;Barra de progresso
	Local $barra_x = $progress_x+17
	Local $barra_y = $progress_y+36
	Local $barra_h = 19

	;Descrição da porcentagem Ex: Carregando jogo...
	Global $iGetMSG = GUICtrlCreateLabel("",$progress_x,$progress_y+60,$progress_h -10,30,$SS_RIGHT)
	GUICtrlSetFont(-1,18,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0x7D7D7D)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )

	;Porcentagem -99%-
	Global $iGetPercent = GUICtrlCreateLabel("0%", $progress_x+10  ,$progress_y+4,60,25)
	GUICtrlSetFont(-1,18,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0x7D7D7D)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )

	;pontinho esquerdo <= - Esquerdo
	_GUICtrlPic_Create($iIMG_3, $barra_x - 7 , $barra_y, -1, -1)

	;pontinho esquerdo => - Direito
	Global $iProgressPic2 = _GUICtrlPic_Create($iIMG_5, $barra_x, $barra_y, -1, -1)

	;Barra do meio =====
	Global $iProgressPic  = _GUICtrlPic_Create($iIMG_4, $barra_x, $barra_y, $iProgressGet, -1)
	GUICtrlSetState(-1,@SW_DISABLE)

	;----------------------------------------------------------------------------
	;Backgroundo do Progresso
	_GUICtrlPic_Create($iIMG_6, $progress_x, $progress_y, $progress_h, $progress_w)
	GUICtrlSetState(-1,@SW_DISABLE)
	;====================================== Barra de progresso =================================>

	;----------------------------------------------------------------------------
	Local $iBarX = 0
	Local $iBarY = (@DesktopHeight/2)+200
	Local $iBarW = @DesktopWidth
	Local $iBarH = 130
	Local $iExtraBarTop    = 10
	Local $iExtraBarButton = 23

	;----------------------------------------------------------------------------
	;GIF # GIF #
	$i___GIF_FILES = ($iFADE_FOLDER & "\layer 4*")
	Local $ifile = _get_file_sequence($i___GIF_FILES)
	if @error Then
		$i___GIF_FILES = ($iFADE_DEFAULT_FOLDER & "\layer 4*")
		$ifile = _get_file_sequence($i___GIF_FILES)
	EndIf

	if Not @error Then
		Local $ihSize = _Image_getSize($ifile)
		$i___GIF_CONTROL = _GUICtrlPic_Create($ifile, $iBarW-$ihSize[0]-($iBarW*5/100),($iBarY + ($iExtraBarTop + $iBarH - $iExtraBarButton)/2) - ($ihSize[1]/2) ,-1 , -1)
		AdlibRegister("__gifConst",100)
	EndIf

	;----------------------------------------------------------------------------
	;descrição nome do jogo
	Local $iDesc1_TAM 	 = 22
	Local $iDesc1_font_s = @DesktopHeight/$iDesc1_TAM

	Local $iDesc1_X 	 =  (@DesktopWidth*3)/100
	Local $iDesc1_Y 	 =  $iBarY + 10
	Local $iDesc1_H 	 =  @DesktopWidth -($iDesc1_X)
	Local $iDesc1_W 	 =  @DesktopHeight/($iDesc1_TAM-4)

	;~ Label
	GUICtrlCreateLabel($iGame,$iDesc1_X , $iDesc1_Y, $iDesc1_H, $iDesc1_W, 	$SS_CENTERIMAGE)
	GUICtrlSetFont(-1,$iDesc1_font_s,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0x00468C)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )
	;~ Label sombra
	GUICtrlCreateLabel($iGame,$iDesc1_X+4 , $iDesc1_Y+4, $iDesc1_H, $iDesc1_W, 	$SS_CENTERIMAGE)
	GUICtrlSetFont(-1,$iDesc1_font_s,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )
	;----------------------------------------------------------------------------
	;Descrição 2 (Modo de jogo)
	Local $mode = ($iMode = False)? "Modo: Tempo":"Modo: Ficha"
	Local $iDesc2_TAM = 34
	Local $iDesc2_font_s = @DesktopHeight/$iDesc2_TAM

	Local $iDesc2_X = (@DesktopWidth*3)/100
	Local $iDesc2_Y = $iBarY+10 + @DesktopHeight/($iDesc1_TAM-4)
	Local $iDesc2_H = @DesktopWidth -($iDesc2_X)
	Local $iDesc2_W = @DesktopHeight/($iDesc2_TAM-12)

	;~ Label
	GUICtrlCreateLabel($mode, $iDesc2_X, $iDesc2_Y, $iDesc2_H, $iDesc2_W, $SS_CENTERIMAGE)
	GUICtrlSetFont(-1,$iDesc2_font_s,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0x7D7D7D)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )
	;~ Label sombra
	GUICtrlCreateLabel($mode, $iDesc2_X+4, $iDesc2_Y+4, $iDesc2_H, $iDesc2_W, $SS_CENTERIMAGE)
	GUICtrlSetFont(-1,$iDesc2_font_s,400,0, "impact",  $CLEARTYPE_QUALITY)
	GUICtrlSetColor(-1,0)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT )
	;~
	;----------------------------------------------------------------------------
	;LAYER de descrição (Tarja)
	_GUICtrlPic_Create($iIMG_7, $iBarX,$iBarY ,$iBarW , $iBarH)

	;----------------------------------------------------------------------------
	;Background
	;----------------------------------------------------------------------------
	Local $layer = _GET_FIRST_FILE($iFADE_FOLDER & "\" & "Layer 3*")
	if @error Then  $layer = _GET_FIRST_FILE($iFade_default & "\" & "Layer 3*")
	if Not @error Then
		_GUICtrlPic_Create($layer, (@DesktopWidth/2)-512, (@DesktopHeight/2)-62, 1024, 124)
		GUICtrlSetState(-1,@SW_DISABLE)
	EndIf


	Local $layer = _GET_FIRST_FILE($iFADE_FOLDER & "\" & "Extra Layer 1 - Console*")
	if @error Then  $layer = _GET_FIRST_FILE($iFade_default & "\" & "Extra Layer 1 - Console*")
	if Not @error Then
		_GUICtrlPic_Create($layer, 0, 0, @DesktopWidth+1, @DesktopHeight+1)
		GUICtrlSetState(-1,@SW_DISABLE)
	EndIf

	Local $layer = _GET_FIRST_FILE($iFADE_FOLDER & "\" & "Layer 1*")
	if @error Then  $layer = _GET_FIRST_FILE($iFade_default & "\" & "Layer 1*")
	if Not @error Then
		_GUICtrlPic_Create($layer, 0, 0, @DesktopWidth+1, @DesktopHeight+1)
		GUICtrlSetState(-1,@SW_DISABLE)
	EndIf

	;----------------------------------------------------------------------------
	;=======================================
	GUISetState(@SW_SHOW,$iGuiSplash)
;~ 	WinSetOnTop($iGuiSplash,"",$WINDOWS_ONTOP )
	ControlMove($iGuiSplash,"",$iProgressPic,  $barra_x , $barra_y, 1 , $barra_h)
	Return $iGuiSplash
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_FIRST_FILE
; Description ...:
; Syntax ........: _GET_FIRST_FILE($iFile)
; Parameters ....: $iFile               - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_FIRST_FILE($iFile)
	Local $iGet =  FileFindFirstFile($iFile)
	if @error or $iGet < 0 Then Return SetError(-1)

	Local $sDrive = "", $sDir = "", $sFileName = "", $sExtension = ""
	Local $aPathSplit = _PathSplit($iFile, $sDrive, $sDir, $sFileName, $sExtension)
	If @error then Return SetError(-2)

	 Return $aPathSplit[1] & $aPathSplit[2] & FileFindNextFile($iGet)
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _SPLASH_CLOSE
; Description ...:
; Syntax ........: _SPLASH_CLOSE($iHandle)
; Parameters ....: $iHandle             - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SPLASH_CLOSE(ByRef $iHandle)
	if Not $iHandle then Return
	AdlibUnRegister("__gifConst")
	GUISetState(@SW_HIDE,$iHandle)
	GUIDelete($iHandle)
	$iHandle = 0
EndFunc

; #INTERNAL_USE_ONLY# ===========================================================================================================
; Name ..........: __gifConst
; Description ...:
; Syntax ........: __gifConst()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func __gifConst()
	Local $ifile = _get_file_sequence($i___GIF_FILES)
	_GUICtrlPic_SetPic($i___GIF_CONTROL,$ifile)
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _get_file_sequence
; Description ...:
; Syntax ........: _get_file_sequence($hfile)
; Parameters ....: $hfile               - a handle value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _get_file_sequence($hfile)
	Local $sDrive = "", $sDir = "", $sFileName = "", $sExtension = ""
	Local $aPathSplit = _PathSplit($hfile, $sDrive, $sDir, $sFileName, $sExtension)
	if @error then Return SetError(-3)
	Local $Path = $aPathSplit[1] & $aPathSplit[2]
	Local $iRet=FileFindNextFile($i___GIF_FILES_SAVE,1)
	if @error Then
		$i___GIF_FILES_SAVE = FileFindFirstFile($hfile)
		if @error or $i___GIF_FILES_SAVE < 0 Then Return SetError(-1,0,False)
		Return $Path & FileFindNextFile($i___GIF_FILES_SAVE)
	EndIf
	Return $Path & $iRet
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _Image_getSize
; Description ...:
; Syntax ........: _Image_getSize($himage)
; Parameters ....: $himage              - a handle value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _Image_getSize($himage)
	_GDIPlus_Startup()
	Local $hBitmap = _GDIPlus_BitmapCreateFromFile($himage)
	if @error Then Return SetError(-1)
	Local $iWidth = _GDIPlus_ImageGetWidth($hBitmap)
	Local $iHeight = _GDIPlus_ImageGetHeight($hBitmap)
	Local $iArray[2] = [$iWidth,$iHeight]
	_GDIPlus_BitmapDispose($hBitmap)
	_GDIPlus_Shutdown()
	Return $iArray
EndFunc

#cs
; #FUNCTION# ====================================================================================================================
; Name ..........: _MSG_SHOW_MSG
; Description ...:
; Syntax ........: _MSG_SHOW_MSG($iHandle, $menssagem, $iTime)
; Parameters ....: $iHandle             - an integer value.
;                  $menssagem           - a map.
;                  $iTime               - an integer value.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _MSG_SHOW_MSG($iHandle, $menssagem,$iTime,$iRettick)
;~ 	_WinAPI_ShowCursor (False)
;~~~~
	WinGetHandle($iHandle,"")
	if @error Then
		;Label Mensagem
		Local $iguisystem = GUICreate($iHandle, 594, 181, -1, -1, $WS_POPUP, $WS_EX_TOOLWINDOW + $WS_EX_COMPOSITED + $WS_EX_TOPMOST)
		Local $INFO_SYSTEM = GUICtrlCreateLabel($menssagem, 10, 10, 574, 106, $SS_CENTER)
		GUICtrlSetFont($INFO_SYSTEM, 17, -1, -1, "arial")
		GUICtrlSetBkColor($INFO_SYSTEM, $GUI_BKCOLOR_TRANSPARENT)

	;~~~~
		;Botão aceitar
		Local $INFO_LABEL_BUTTON = GUICtrlCreateLabel("", 208, 132, 200, 29, $SS_CENTER)
		GUICtrlSetFont($INFO_LABEL_BUTTON, 16, -1, -1, "arial")
		GUICtrlSetBkColor($INFO_LABEL_BUTTON, $GUI_BKCOLOR_TRANSPARENT)
		GUICtrlSetCursor($INFO_LABEL_BUTTON, 0)

		Local $INFO_BUTTON = GUICtrlCreatePic($READ_IMG1, 165, 120, 278, 49)
		GUICtrlSetCursor($INFO_BUTTON, 0)

		;Background
		Local $IBK = GUICtrlCreatePic($READ_IMG2, 0, 0, 594, 181)
		GUICtrlSetState(-1, $GUI_DISABLE)

		GUISetState()
		Return $iRettick
	Else
		Local $iTick = DllCall("kernel32.dll", "int", "GetTickCount")
		if @error then Return $iRettick
			Local $iGet = StringFormat("%02d",Int(($iTime-($iTick[0]-$RETTICK_MSG_ERRO))/1000))
			if $iGet < 0 Then
				GUISetState(@SW_HIDE,$iHandle)
				GUIDelete($iHandle)
				_CALL_EXIT()
				Return $iTick[0]
			Else
				if ControlGetText($iHandle,"","Static2") <> ("SAIR [" & $iGet & "]") Then ControlSetText($iHandle, "", "Static2", "SAIR [" & $iGet & "]")
				Return $iRettick
			EndIf
	EndIf
EndFunc   ;==>_MSG_SHOW_MSG
#ce