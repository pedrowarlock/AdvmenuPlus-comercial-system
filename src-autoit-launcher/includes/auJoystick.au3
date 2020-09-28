#include "auSdl.au3"
; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GET_BUTTON
; Description ...:
; Syntax ........: _JOY_GET_BUTTON($iJoyComand, $iButton)
; Parameters ....: $iJoyComand          - an integer value.
;                  $iButton             - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GET_BUTTON($iJoyComand,$iButton)
	if $iButton = -1 then Return False
	Local $iJoyGet = _JOY_GET_NUMBER()
	if $iJoyGet <= 0 then Return False

	Local $iBT = _SDL_JoystickNumButtons($iJoyComand)
	if $iBT < $iButton Then Return False

	If _SDL_JoystickGetButton($iJoyComand, $iButton) > 0 Then Return 1
	Return False
EndFunc


; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GET_NUMBER_BUTTONS
; Description ...:
; Syntax ........: _JOY_GET_NUMBER_BUTTONS($ijoy)
; Parameters ....: $ijoy                - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GET_NUMBER_BUTTONS($ijoy)
	Return _SDL_JoystickNumButtons($ijoy)
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GET_NAME
; Description ...:
; Syntax ........: _JOY_GET_NAME($i_index)
; Parameters ....: $i_index             - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GET_NAME($i_index)
	Return _SDL_JoystickName($i_index)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GET_AXIS_BUTTONS
; Description ...:
; Syntax ........: _JOY_GET_AXIS_BUTTONS($ijoy, $iaxisN)
; Parameters ....: $ijoy                - an integer value.
;                  $iaxisN              - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GET_AXIS_BUTTONS($ijoy,$iaxisN)
	Return _SDL_JoystickGetAxis($ijoy,$iaxisN)
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GET_NUMBER
; Description ...:
; Syntax ........: _JOY_GET_NUMBER()
; Parameters ....: None
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GET_NUMBER()
	Return _SDL_NumJoysticks()
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_RESET_TICK
; Description ...:
; Syntax ........: _JOY_RESET_TICK($ia)
; Parameters ....: $ia                  - an integer value.
; Return values .: None
; Author ........: Your Name
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_RESET_TICK($ia)
	Local $iTick = DllCall("kernel32.dll", "int", "GetTickCount")
	if @error then Return

	if _IsPressed("66") or _IsPressed("67") or _IsPressed("68") or  _IsPressed("69") or _IsPressed("25") or _IsPressed("26") or _IsPressed("27") or _IsPressed("28")  then
		Global $RETTICK_OCIOSO = $iTick[0]
		Return
	EndIf

	if not IsArray($ia) then Return False
		For $i=0 to UBound($ia)-1
			if _JOY_GETRETTICK($ia[$i]) then
				Global $RETTICK_OCIOSO = $iTick[0]
				Return
			EndIf
		Next
		Return
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_START
; Description ...:
; Syntax ........: _JOY_START()
; Parameters ....:
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_START($i="")
	Return _SDL_Init($_SDL_INIT_JOYSTICK,$i)
EndFunc

Func _JOY_QUIT()
	Return _SDL_Quit()
EndFunc

; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_OPEN
; Description ...:
; Syntax ........: _JOY_OPEN()
; Parameters ....:
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_OPEN($iP)
		Local $iJ = _JOY_GET_NUMBER()
		if $iJ <= 0 then Return False
		Local $Joystick = _SDL_JoystickOpen($iP)
		if $Joystick = 0 Then Return False
	Return $Joystick
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_GETRETTICK
; Description ...:
; Syntax ........: _JOY_GETRETTICK($iJoy)
; Parameters ....: $iJoy                - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_GETRETTICK($iJo)
	if Not $iJo then return False
	Local $iJoyGet = _JOY_GET_NUMBER()
	if $iJoyGet <= 0 then Return SetError(1)

			Local $iBT = _SDL_JoystickNumButtons($iJo)
			Local $Axes = _SDL_JoystickNumAxes($iJo)

			For $i=0 to $iBT
				If _SDL_JoystickGetButton($iJo, $i) > 0 Then Return True
			Next

			For $i=0 to $Axes
				Local $bAxes = _SDL_JoystickGetAxis($iJo, $i)
				If  $bAxes > 1000 or $bAxes < -1000 then return True
			Next
		Return False
EndFunc
; #FUNCTION# ====================================================================================================================
; Name ..........: _JOY_UPDATE
; Description ...:
; Syntax ........: _JOY_UPDATE($iJoy)
; Parameters ....: $iJoy                - an integer value.
; Return values .: None
; Author ........: PedroWarlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func _JOY_UPDATE($iKJ1)
	if not IsArray($iKJ1) then Return False
	Local $iJ = _JOY_GET_NUMBER()
	if $iJ <= 0 then Return SetError(1)

	for $i=0 To $iJ-1
		_SDL_JoystickUpdate($iKJ1[$i])
	Next
	Return True
EndFunc
