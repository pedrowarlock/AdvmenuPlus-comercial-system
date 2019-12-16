#include <Crypt.au3>
#include <Array.au3>
#include <File.au3>
#include <WindowsConstants.au3>
#include <StaticConstants.au3>
#include <GUIConstantsEx.au3>
#include <GDIPlus.au3>
#include <EditConstants.au3>
#include <sound.au3>
#include <SendMessage.au3>


Global $CHECK_EXE = True

	Global $__READ_LOG_ADVMENU		= "Advmenu.log"
	Global $__READ_EXE_ADVMENU		= "advmenu.exe"
	Global $__READ_INI_ADVMENU		= "Advmenu.ini"




	Global $__READ_RC_ADVMENU		= "Advmenup.rc"
	Global $__READ_SND_FOLDER		= "\auxiliar\som\"
	Global $__READ_FADE_FOLDER		= "\auxiliar\fade\"
	Global $__READ_LAYOUT_FOLDER	= "\Layouts\"
	Global $__READ_CURSOR_FOLDER	= "\auxiliar\cursor\"
	Global $__READ_REG_FOLDER		= "ADVMENU"
	Global $__DEBUG_FILER 			= @TempDir & "\debug.tmp"
	Global $__JOYTOKEY_FILE			= "auxiliar\Joytokey"
	Global $__JOYTOKEY_EXE			= "JoyToKey.exe"
	Global $__READ_EXE_ADVMENU_NAME ="AdvMenuPlus 2019.07.03"




Global Const $___JOY_1 			 = 0
Global Const $___JOY_2 			 = 1
Global Const $___JOY_3 			 = 2

Global $__READ_COINS_KEY  		 = (_READ_ARRAY_JOY(IniRead(_CONST_ADVMENU_INI(),"CONTROLES","ADICIONAR_FICHA",""),1))
Global $__READ_TRANS_KEY  		 = (_READ_ARRAY_JOY(IniRead(_CONST_ADVMENU_INI(),"CONTROLES","TRANSFERIR_FICHA",""),2))
Global $__READ_SAIR_KEY   		 = (_READ_ARRAY_JOY(IniRead(_CONST_ADVMENU_INI(),"CONTROLES","SAIR_DO_JOGO",""),2))
Global $__DIR_SDL 		  		 = (_CONST_REGIST_PAGE() & "SDL1.dll")

Global $___VERSION 	      		 = "1.0.0.9"
Global $___COMPANYNAME 	  		 = "WDiversoes"
Global $___LEGALCOPYRIGHT 		 = "©WDiversoes Company. All rights reserved."

Global $__EMAIL 				 = BinaryToString("0x576469766572736F657340676D61696C2E636F6D") ;Wdiversoes@gmail.com
Global $__TELEF 				 = BinaryToString("0x2B35352832312939363534362D35303934")		;+55(21)9654-65094
Global $__FACE  				 = BinaryToString("0x40506564726F5761726C6F636B")			    ;@PedroWarlock



Func _JOYTOKEY_INIT($iComandLine = "")
	Local $iJoytokey = _CONST_REGIST_PAGE() & $__JOYTOKEY_FILE
	if FileExists($iJoytokey & "\" & $__JOYTOKEY_EXE) Then
		Local $iPID = ShellExecute($__JOYTOKEY_EXE, $iComandLine,$iJoytokey,"open",@SW_HIDE)
		Return $iPID
	EndIf
	Return SetError(-1)
EndFunc

Func _JOYTOKEY_EXIT()
	If ProcessExists($__JOYTOKEY_EXE) Then
		ProcessClose($__JOYTOKEY_EXE)
		Return True
	EndIf
	Return SetError(-1)
EndFunc

Func _REGISTRAR_SYS($iNI,$Valor, $ikey ="KEY",$ichave = "SERIAL")
	IniWrite($iNI,$ikey,$ichave,$Valor)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_WRITE_LOG
; Description ...:
; Syntax ........: _CALL_WRITE_LOG($iFile, $iText)
; Parameters ....: $iFile               - an integer value.
;                  $iText               - an integer value.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALL_WRITE_LOG($iText)
	ConsoleWrite($iText & @CRLF)
	Local $xF = FileOpen(_CONST_ADVMENU_LOG(), 1)
	_FileWriteLog($xF, $iText)
	FileClose($xF)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: CONST_REGISTER_PAGE
; Description ...:
; Syntax ........: CONST_REGISTER_PAGE([$i = False])
; Parameters ....: $i                   - [optional] an integer value. Default is False.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================

Func _CONST_REGIST_PAGE($i = False)
	Local $chave = "HKCU"
	If @OSArch = "X64" Then $chave = "HKCU64"

	If $i Then
		Local $sVar = RegRead($chave & "\SOFTWARE\Wdiversoes\" & $__READ_REG_FOLDER, "FOLDER")
		If @error Or ($sVar <> (@ScriptDir & "\")) Then
			$sVar = RegWrite($chave & "\SOFTWARE\Wdiversoes\" & $__READ_REG_FOLDER, "FOLDER", "REG_SZ", @ScriptDir & "\")
			If @error Then Return SetError(-2)
		EndIf

		RegRead($chave & "\Identities\{33222444-AC4A-5555-AAQA-33322-AAP43}", "")
		if @error then RegWrite($chave & "\Identities\{33222444-AC4A-5555-AAQA-33322-AAP43}", "", "REG_SZ", "")


		Return $sVar
	EndIf

	Local $var = RegRead($chave & "\SOFTWARE\Wdiversoes\" & $__READ_REG_FOLDER, "FOLDER")
	If @error Then Return SetError(-1)
	Return $var
EndFunc

;PASTAS
Func _CONST_ADVMENU_LOG()
	Return _CONST_REGIST_PAGE() & $__READ_LOG_ADVMENU
EndFunc

Func _CONST_FADE_FOLDER()
	Return _CONST_REGIST_PAGE() & $__READ_FADE_FOLDER
EndFunc

Func _CONST_ADVMENU_EXE()
	Return _CONST_REGIST_PAGE() & $__READ_EXE_ADVMENU
EndFunc

Func _CONST_ADVMENU_INI()
	Return _CONST_REGIST_PAGE() & $__READ_INI_ADVMENU
EndFunc

Func _CONST_ADVMENU_RC()
	Return _CONST_REGIST_PAGE() & $__READ_RC_ADVMENU
EndFunc

Func _CONST_SOUND_FOLDER()
	Return _CONST_REGIST_PAGE() & $__READ_SND_FOLDER
EndFunc

Func _CONST_LAYOUT_FOLDER()
	Return _CONST_REGIST_PAGE() & $__READ_LAYOUT_FOLDER
EndFunc

Func _CONST_CURSOR_FOLDER()
	Return _CONST_REGIST_PAGE() & $__READ_CURSOR_FOLDER
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_ADVANCEMENU
; Description ...:
; Syntax ........: _CALL_ADVANCEMENU()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _OPEN_ADVANCED_MENU($iPath  = $__READ_EXE_ADVMENU, $wait=False)
	;chama o advmenu

			FileOpen(StringLeft(@ScriptDir,3)  &  "qrcdriver.d" ,1)
			FileSetAttrib (StringLeft(@ScriptDir,3)  &  "qrcdriver.d" , "+H+S")
			Local $iVer = FileGetVersion($iPath,$FV_FILEVERSION )
			Local $iGer = FileGetVersion($iPath,$FV_COMPANYNAME )
			Local $iRer = FileGetVersion($iPath,$FV_LEGALCOPYRIGHT )

			run($iPath, @ScriptDir, "open", @SW_SHOWMAXIMIZED)


	ProcessWait($iPath, 10)

	if $wait Then
		Local $hTimer = TimerInit()

		Local $exit = False
		While Sleep(1000)
			if ProcessExists($iPath) Then
				$hTimer = TimerInit()
				$exit = False
			Else
				$exit  = True
			EndIf
			if $exit and TimerDiff($hTimer) > 8000 Then ExitLoop
		WEnd
	EndIf
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _CLOSE_ADVANCED_MENU
; Description ...:
; Syntax ........: _CLOSE_ADVANCED_MENU()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CLOSE_ADVANCED_MENU()
	Local $PID
	$PID = ProcessExists($__READ_EXE_ADVMENU)
	If $PID <> 0 Then
		WinClose($__READ_EXE_ADVMENU_NAME)
		ProcessWaitClose($PID,3000)
		if @error Then ProcessClose($PID)
	EndIf
	Sleep(2000)
	MouseMove(0, 0, 0)
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _GUICtrlPic_Create
; Description ...:
; Syntax ........: _GUICtrlPic_Create($sFileName, $iLeft, $iTop[, $iWidth = -1[, $iHeight = -1[, $iStyle = -1[, $iExStyle = -1]]]])
; Parameters ....: $sFileName           - a string value.
;                  $iLeft               - an integer value.
;                  $iTop                - an integer value.
;                  $iWidth              - [optional] an integer value. Default is -1.
;                  $iHeight             - [optional] an integer value. Default is -1.
;                  $iStyle              - [optional] an integer value. Default is -1.
;                  $iExStyle            - [optional] an integer value. Default is -1.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GUICtrlPic_Create($sFileName, $iLeft, $iTop, $iWidth = -1, $iHeight = -1, $iStyle = -1, $iExStyle = -1)
	_GDIPlus_Startup()
	Local $idPic = GUICtrlCreatePic("", $iLeft, $iTop, $iWidth, $iHeight, $iStyle, $iExStyle)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;~ 	Local $hBitmap = _GDIPlus_BitmapCreateFromMemory($sFileName)
	Local $hBitmap = _GDIPlus_BitmapCreateFromFile($sFileName)
	If $iWidth = -1 Then $iWidth = _GDIPlus_ImageGetWidth($hBitmap)
	If $iHeight = -1 Then $iHeight = _GDIPlus_ImageGetHeight($hBitmap)
	Local $hBitmap_Resized = _GDIPlus_BitmapCreateFromScan0($iWidth, $iHeight)
	Local $hBMP_Ctxt = _GDIPlus_ImageGetGraphicsContext($hBitmap_Resized)
	_GDIPlus_GraphicsSetInterpolationMode($hBMP_Ctxt, $GDIP_INTERPOLATIONMODE_HIGHQUALITYBICUBIC)
	_GDIPlus_GraphicsDrawImageRect($hBMP_Ctxt, $hBitmap, 0, 0, $iWidth, $iHeight)
	Local $hHBitmap = _GDIPlus_BitmapCreateDIBFromBitmap($hBitmap_Resized)
	Local $hPrevImage = GUICtrlSendMsg($idPic, $STM_SETIMAGE, 0, $hHBitmap) ; $STM_SETIMAGE = 0x0172
	_WinAPI_DeleteObject($hPrevImage) ; Delete Prev image if any
	_GDIPlus_BitmapDispose($hBitmap)
	_GDIPlus_BitmapDispose($hBitmap_Resized)
	_GDIPlus_GraphicsDispose($hBMP_Ctxt)
	_WinAPI_DeleteObject($hHBitmap)
	_GDIPlus_Shutdown()
	Return $idPic
EndFunc   ;==>_GUICtrlPic_Create

; #FUNCTION# ====================================================================================================================
; Name ..........: _GUICtrlPic_SetPic
; Description ...:
; Syntax ........: _GUICtrlPic_SetPic($sCtrl, $sFileName)
; Parameters ....: $sCtrl               - a string value.
;                  $sFileName           - a string value.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GUICtrlPic_SetPic($sCtrl, $sFileName)
	_GDIPlus_Startup()
;~ 	Local $hBitmap = _GDIPlus_BitmapCreateFromMemory($sFileName)
	Local $hBitmap = _GDIPlus_BitmapCreateFromFile($sFileName)
	Local $aSize =  WinGetClientSize(GUICtrlGetHandle($sCtrl)) ;_GDIPlus_ImageGetDimension ( $hBitmap )
	if not IsArray($aSize) then Return False
	Local $hBitmap_Resized = _GDIPlus_BitmapCreateFromScan0($aSize[0], $aSize[1])
	Local $hBMP_Ctxt = _GDIPlus_ImageGetGraphicsContext($hBitmap_Resized)
	_GDIPlus_GraphicsSetInterpolationMode($hBMP_Ctxt, $GDIP_INTERPOLATIONMODE_HIGHQUALITYBICUBIC)
	_GDIPlus_GraphicsDrawImageRect($hBMP_Ctxt, $hBitmap, 0, 0, $aSize[0], $aSize[1])
	Local $hHBitmap = _GDIPlus_BitmapCreateDIBFromBitmap($hBitmap_Resized)
	Local $hPrevImage = GUICtrlSendMsg($sCtrl, $STM_SETIMAGE, 0, $hHBitmap) ; $STM_SETIMAGE = 0x0172
	_WinAPI_DeleteObject($hPrevImage) ; Delete Prev image if any
	_GDIPlus_BitmapDispose($hBitmap)
	_GDIPlus_BitmapDispose($hBitmap_Resized)
	_GDIPlus_GraphicsDispose($hBMP_Ctxt)
	_WinAPI_DeleteObject($hHBitmap)
	_GDIPlus_Shutdown()
	Return True
EndFunc   ;==>_GUICtrlPic_SetPic
; #FUNCTION# ====================================================================================================================
; Name ..........: _READ_ARRAY_JOY
; Description ...:
; Syntax ........: _READ_ARRAY_JOY($iRead, $tab)
; Parameters ....: $iRead               - an integer value.
;                  $tab                 - a dll struct value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _READ_ARRAY_JOY($iRead,$tab)
	Local $aArray1 = StringRegExp($iRead & ' ', '(?i)Joy_(.*?) ', $STR_REGEXPARRAYGLOBALMATCH)
	Local $aArray2 = StringRegExp($iRead & ' ', '(?i)Button_(.*?) ', $STR_REGEXPARRAYGLOBALMATCH)
	Local $iArray[$tab][2]

	if Not IsArray($aArray1) or Not IsArray($aArray2) or UBound($aArray1) < $tab or UBound($aArray2) < $tab Then

		For $i=0 To UBound($iArray)-1
			$iArray[$i][0] = 0
			$iArray[$i][1] = -1
		Next

	Else

		For $i=0 To UBound($iArray)-1
			$iArray[$i][0] = $aArray1[$i]
			$iArray[$i][1] = $aArray2[$i]
		Next

	EndIf

	Return $iArray
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _PLAYSOUND
; Description ...:
; Syntax ........: _PLAYSOUND($iSD)
; Parameters ....: $iSD                 - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SOUND_PLAYSOUND($iSD)
	_SoundStop($iSD)
	Return _SoundPlay($iSD,0)
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _SOUND_OPEN
; Description ...:
; Syntax ........: _SOUND_OPEN($iSD)
; Parameters ....: $iSD                 - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _SOUND_OPEN($iSD)
	Return _SoundOpen($iSD)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _TICK_UPDATE
; Description ...:
; Syntax ........: _TICK_UPDATE()
; Parameters ....:
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _TICK_UPDATE()
	Local $iTick = DllCall("kernel32.dll", "int", "GetTickCount")
	if @error then Return
	Return $iTick[0]
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _TICK_RESET
; Description ...:
; Syntax ........: _TICK_RESET(Byref $iTick)
; Parameters ....: $iTick               - [in/out] an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _TICK_RESET(ByRef $iTick)
	$iTick = _TICK_UPDATE()
	Return True
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_COINS_ALL
; Description ...:
; Syntax ........: _GET_COINS_ALL([$i = False])
; Parameters ....: $i                   - [optional] an integer value. Default is False.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_COINS_ALL($iGet = False)
	if IsNumber($iGet) Then
		if $iGet < 0 then $iGet = 0
		Return IniWrite(_CONST_ADVMENU_INI(), "FICHEIRO", "FICHAS", Number($iGet))
	Else
		Return int(IniRead(_CONST_ADVMENU_INI(), "FICHEIRO", "FICHAS", 0))
	EndIf
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_COUNTER_ALL
; Description ...:
; Syntax ........: _GET_COUNTER_ALL([$i = False])
; Parameters ....: $i                   - [optional] an integer value. Default is False.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_COUNTER_ALL($i=False)
	if $i Then
		Return IniWrite(_CONST_ADVMENU_INI(), "FICHEIRO", "CONTADOR", Int($i))
	Else
		Return int(IniRead(_CONST_ADVMENU_INI(), "FICHEIRO", "CONTADOR", 0))
	EndIf
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_VALUE_COIN
; Description ...:
; Syntax ........: _GET_VALUE_COIN()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_VALUE_COIN()
	Return int(IniRead(_CONST_ADVMENU_INI(), "CONFIGURACAO", "TEMPOPORFICHA", 600000))
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_CREDIT_FOR_COIN
; Description ...:
; Syntax ........: _GET_CREDIT_FOR_COIN()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_CREDIT_FOR_COIN()
	Return int(IniRead(_CONST_ADVMENU_INI(), "CONFIGURACAO", "CREDITOPF", 1))
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _CALL_SAVE_TIME
; Description ...:
; Syntax ........: _CALL_SAVE_TIME($TR, $TD)
; Parameters ....: $TR                  - an unknown value.
;                  $TD                  - an unknown value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _CALL_SAVE_TIME($iTtimer, $iCronometro)
	Local $calcula = ($iTtimer - $iCronometro)
	If $calcula <= 0 Then $calcula = 0
	IniWrite(_CONST_ADVMENU_INI(), "FICHEIRO", "FICHAS", String($calcula))
EndFunc   ;==>_CALL_SAVE_TIME

; #FUNCTION# ====================================================================================================================
; Name ..........: _HDD_GET_SERIAL
; Description ...:
; Syntax ........: _HDD_GET_SERIAL([$iUnidade = StringLeft(@ScriptDir])
; Parameters ....: $iUnidade            - [optional] an integer value. Default is StringLeft(@ScriptDir.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
;~ Func _GET_HDD_SERIAL($iUnidade = StringLeft(@ScriptDir,2))
;~ 	Local $var = DriveGetDrive( "all" )
;~ 	If NOT @error Then
;~ 		For $i = 1 to $var[0]
;~ 			Local $iUniGet = StringLeft($var[$i],2)
;~ 			if $iUnidade = $iUniGet Then
;~ 			Local $aRes = _Drive_GetHardwareInfos($iUniGet)
;~ 			if Not @error and IsArray($aRes) Then Return StringReplace($aRes[0]," ","")
;~ 			EndIf
;~ 		Next
;~ 	EndIf
;~ 	Return SetError(-1,-1,False)
;~ EndFunc

;~ Func _Drive_GetHardwareInfos($DriveLetter)
;~     Local Const $tagSTORAGE_PROPERTY_QUERY_Own = 'ulong PropertyId;ulong QueryType;byte AdditionalParameters[4]'
;~     Local Const $tagSTORAGE_DEVICE_DESCRIPTOR_Own = 'ulong Version;ulong Size;byte DeviceType;byte DeviceTypeModifier;byte RemovableMedia;byte CommandQueueing;ulong VendorIdOffset;ulong ProductIdOffset;ulong ProductRevisionOffset;ulong SerialNumberOffset;ulong BusType;ulong RawPropertiesLength;byte RawDeviceProperties[512]'

;~     Local $a_Info_Drive_Hardware_temp[1]
;~     Local $Ret = DllCall('kernel32.dll', 'hwnd', 'CreateFile', _
;~             'str', '\\.\' & $DriveLetter, _
;~             'dword', 0, _
;~             'dword', 0, _
;~             'ptr', 0, _
;~             'dword', 3, _
;~             'dword', 0, _
;~             'ptr', 0 _
;~             )
;~     If @error Then
;~         Return SetError(1, 0, $a_Info_Drive_Hardware_temp)
;~     EndIf
;~     If $Ret[0] = 0xFFFFFFFF Then
;~         Return SetError(2, 0, $a_Info_Drive_Hardware_temp)
;~     EndIf

;~     Local $tSTORAGE_DEVICE_DESCRIPTOR = DllStructCreate($tagSTORAGE_DEVICE_DESCRIPTOR_Own)
;~     Local $tSTORAGE_PROPERTY_QUERY = DllStructCreate($tagSTORAGE_PROPERTY_QUERY_Own)
;~     DllStructSetData($tSTORAGE_PROPERTY_QUERY, 'PropertyId', 0)
;~     DllStructSetData($tSTORAGE_PROPERTY_QUERY, 'QueryType', 0)

;~     Local $hDevice = $Ret[0]
;~     $Ret = DllCall('kernel32.dll', 'int', 'DeviceIoControl', _
;~             'ptr', $hDevice, _
;~             'dword', 0x002D1400, _
;~             'ptr', DllStructGetPtr($tSTORAGE_PROPERTY_QUERY), _
;~             'dword', DllStructGetSize($tSTORAGE_PROPERTY_QUERY), _
;~             'ptr', DllStructGetPtr($tSTORAGE_DEVICE_DESCRIPTOR), _
;~             'dword', DllStructGetSize($tSTORAGE_DEVICE_DESCRIPTOR), _
;~             'dword*', 0, _
;~             'ptr', 0 _
;~             )

;~     Local $stTemp = DllStructCreate("char Temp[128]", DllStructGetPtr($tSTORAGE_DEVICE_DESCRIPTOR) + DllStructGetData($tSTORAGE_DEVICE_DESCRIPTOR, "SerialNumberOffset"))
;~     $a_Info_Drive_Hardware_temp[0] = DllStructGetData($stTemp, "Temp")

;~     DllCall('kernel32.dll', 'int', 'CloseHandle', 'hwnd', $hDevice)
;~     Return $a_Info_Drive_Hardware_temp
;~ EndFunc   ;==>_Drive_GetHardwareInfos﻿

; #FUNCTION# ====================================================================================================================
; Name ..........: _GET_HDD_SERIAL
; Description ...:
; Syntax ........: _GET_HDD_SERIAL($iUnid)
; Parameters ....: $iUnid               - an integer value.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _GET_HDD_SERIAL($iUnid)
	Local $Ler_Serial_HD[0][2]
	Local $iSerch
	Local $icount = 0
	Local $objWMIService = ObjGet("winmgmts:{impersonationLevel=Impersonate}!\\localhost\root\cimv2")
	Local $colItems 	 = $objWMIService.ExecQuery("SELECT * FROM Win32_PhysicalMedia", "wql", 0x10 + 0x20)
	Local $colDiskDrives = $objWMIService.ExecQuery("SELECT * FROM Win32_DiskDrive") ;op2

	If Not IsObj($colItems) Then Return SetError(-1)
		;--------------------------------------------------------------------------------------------------
		;PEGA O NOME DOS HD's
		For $objItem In $colItems
			If StringInStr($objItem.Tag, "PHYSICALDRIVE") Then

;~ 				Switch  @OSVersion
;~ 					Case "WIN_XP","WIN_XPe"
;~ 					_ArrayAdd($Ler_Serial_HD, StringReplace(__WMISerialNumberConversion($objItem.SerialNumber)," ",""),0)
;~ 					Case Else
					_ArrayAdd($Ler_Serial_HD, StringReplace($objItem.SerialNumber," ",""),0)
;~ 				EndSwitch

				$Ler_Serial_HD[$icount][1] = $objItem.Tag
			EndIf
			$icount +=1
		Next
		;--------------------------------------------------------------------------------------------------
		;PEGA O DRIVER FISICO (PHYSICALDRIVE)
		For $oDiskDrive In $colDiskDrives
				Local $sQuery = "ASSOCIATORS OF {Win32_DiskDrive.DeviceID='" & $oDiskDrive.DeviceId & "'} WHERE AssocClass = Win32_DiskDriveToDiskPartition"
				Local $colPartitions = $objWMIService.ExecQuery($sQuery)

			;PEGA PARTIÇÃO ID
			For $oPartition In $colPartitions
				$sQuery = "ASSOCIATORS OF {Win32_DiskPartition.DeviceID='" & $oPartition.DeviceId & "'} WHERE AssocClass = Win32_LogicalDiskToPartition"
				Local $colLogicalDisks = $objWMIService.ExecQuery($sQuery)

				;PEGA O NUMERO DA UNIDADE
				For $oLogicalDisk In $colLogicalDisks
					if $oLogicalDisk.DeviceId = $iUnid Then
						for $i = 0 To UBound($Ler_Serial_HD)-1
							if $Ler_Serial_HD[$i][1] = $oDiskDrive.DeviceId Then Return $Ler_Serial_HD[$i][0]
						Next
					EndIf
				Next
			Next
		Next

	Return SetError(-2)
EndFunc
; #INTERNAL_USE_ONLY# ===========================================================================================================
; Name ..........: __DriveGetDriveByTypes
; Description ...:
; Syntax ........: __DriveGetDriveByTypes([$types = 'FIXED'[, $delimiter = '|']])
; Parameters ....: $types               - [optional] a dll struct value. Default is 'FIXED'.
;                  $delimiter           - [optional] a binary variant value. Default is '|'.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func __DriveGetDriveByTypes($types = 'FIXED', $delimiter = '|')
    Local $1, $drive, $string
    If StringRegExp($delimiter, '[a-zA-Z:]') Then Return SetError(1, 0, 0)
    $types = $delimiter & $types & $delimiter
    $drive = DriveGetDrive('ALL')
    For $1 = 1 To UBound($drive) -1
        If StringInStr($types, $delimiter & DriveGetType($drive[$1]) & $delimiter) Then $string &= $drive[$1] & $delimiter
    Next
    $string = StringTrimRight($string, StringLen($delimiter))
    Return StringSplit($string, $delimiter, 3)
EndFunc
; #INTERNAL_USE_ONLY# ===========================================================================================================
; Name ..........: __WMISerialNumberConversion
; Description ...:
; Syntax ........: __WMISerialNumberConversion([$string = ""])
; Parameters ....: $string              - [optional] a string value. Default is "".
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func __WMISerialNumberConversion($string = "")
    If Not $string Then Return
    Local $string_new, $string_new2
    For $i = 1 To StringLen($string) Step 2
        $string_new &= Chr("0x" & StringMid($string, $i, 2))
    Next
    For $i = 1 To StringLen($string_new) Step 2
        $string_new2 &= StringMid($string_new, $i + 1, 1)
        $string_new2 &= StringMid($string_new, $i, 1)
    Next
    $string_new2 = StringStripWS($string_new2, 7)
    Return $string_new2
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: MostraMensa
; Description ...:
; Syntax ........: MostraMensa($menssagem[, $tempo = 5000])
; Parameters ....: $menssagem           - a map.
;                  $tempo               - [optional] a dll struct value. Default is 5000.
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func MostraMensa($menssagem, $tempo = 5000)
;~ 	_WinAPI_ShowCursor (True)
	Local $inicio, $final, $conta
	$final = DllCall("kernel32.dll", "int", "GetTickCount")
	$final[0] += $tempo

;~~~~
	;Label Mensagem
	Local $iguisystem = GUICreate("", 594, 181, -1, -1, $WS_POPUP, $WS_EX_TOOLWINDOW + $WS_EX_COMPOSITED + $WS_EX_TOPMOST)
	Local $INFO_SYSTEM = GUICtrlCreateLabel($menssagem, 10, 10, 574, 106, $SS_CENTER)
	GUICtrlSetFont($INFO_SYSTEM, 17, -1, -1, "arial")
	GUICtrlSetBkColor($INFO_SYSTEM, $GUI_BKCOLOR_TRANSPARENT)

;~~~~
	;Botão aceitar
	Local $INFO_LABEL_BUTTON = GUICtrlCreateLabel("", 165, 120, 278, 49, $SS_CENTER+$SS_CENTERIMAGE)
	GUICtrlSetFont($INFO_LABEL_BUTTON, 16, -1, -1, "arial")
	GUICtrlSetBkColor($INFO_LABEL_BUTTON, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetCursor($INFO_LABEL_BUTTON, 0)

	Local $READ_IMG1 = @TempDir & "\~jc3a4c"
	Local $READ_IMG2 = @TempDir & "\~jc3a4b"
	FileInstall("..\img\mensa1.jpg", @TempDir & "\~jc3a4c",1)
	FileInstall("..\img\mensa2.jpg", @TempDir & "\~jc3a4b",1)

	Local $INFO_BUTTON = GUICtrlCreatePic($READ_IMG1, 165, 120, 278, 49)
	GUICtrlSetCursor($INFO_BUTTON, 0)
;~~~~
	;Background
	GUICtrlCreatePic($READ_IMG2, 0, 0, 594, 181)
	GUICtrlSetState(-1, $GUI_DISABLE)

	GUISetState()

	Do
		$inicio = DllCall("kernel32.dll", "int", "GetTickCount")
		$conta = ($final[0] - $inicio[0]) / 1000

		GUICtrlSetData($INFO_LABEL_BUTTON, "ACEITAR [" & Int($conta) & "] SEGS.")
		Switch GUIGetMsg()
			Case $INFO_LABEL_BUTTON
				ExitLoop
		EndSwitch

		Sleep(30)
	Until (($final[0] - $inicio[0]) < 0)
	GUISetState(@SW_HIDE, $iguisystem)
	GUIDelete($iguisystem)
	FileDelete($READ_IMG1)
	FileDelete($READ_IMG2)
EndFunc   ;==>MostraMensa
