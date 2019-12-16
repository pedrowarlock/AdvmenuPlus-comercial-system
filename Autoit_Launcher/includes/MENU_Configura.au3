; #FUNCTION# ====================================================================================================================
; Name ..........: ChamaConfig
; Description ...:
; Syntax ........: ChamaConfig()
; Parameters ....: None
; Return values .: None
; Author ........: Pedro Warlock
; Modified ......:
; Remarks .......:
; Related .......:
; Link ..........:
; Example .......: No
; ===============================================================================================================================
Func ChamaConfig()

	Global $__BOTAO_ENTER = $__READ_SAIR_KEY[0][1]
	_JOY_START($__DIR_SDL)
	if @error Or Not FileExists($__DIR_SDL) then
		_CALL_WRITE_LOG("(PROGRAMA TEST JOYSTICK) SDL.dll não foi encontrada no diretorio do sistema")
		Return
	EndIf


Global $JOYSTK[3] = [_JOY_OPEN($___JOY_1), _JOY_OPEN($___JOY_2), _JOY_OPEN($___JOY_3)]


Local $PRETO 	 = 0x000000
Global $VERMELHO = 0xff0000
Local $BRANCO 	 = 0xffffff
Local $VERDE 	 = 0x00ff00
Local $FONTE 	 = "arial"
Local $KEY_UP 	 = 26
Local $KEY_DOWN  = 28
Local $KEY_LEFT  = 25
Local $KEY_RIGHT = 27


FileInstall("..\img\conf_menu\bkgui.png",       @TempDir & "/~tag2a",1)
FileInstall("..\img\conf_menu\bkselect.png",    @TempDir & "/~tag2b",1)
FileInstall("..\img\conf_menu\bkselect2.png",   @TempDir & "/~tag2c",1)
FileInstall("..\img\conf_menu\bkselect3.png",   @TempDir & "/~tag2d",1)
FileInstall("..\img\conf_menu\bkselect4.png",   @TempDir & "/~tag2e",1)
FileInstall("..\img\conf_menu\bkselect5.png",   @TempDir & "/~tag2f",1)
FileInstall("..\img\conf_menu\bkselect8.png",   @TempDir & "/~tag2g",1)
FileInstall("..\img\conf_menu\bkselect9.png",   @TempDir & "/~tag2h",1)

;Imagens
Global $BKGround = @TempDir & "/~tag2a"
Global $BRSELECT1= @TempDir & "/~tag2b"
Global $BRSELECT2= @TempDir & "/~tag2c"
Global $BRSELECT3= @TempDir & "/~tag2d"
Global $BRSELECT4= @TempDir & "/~tag2e"
Global $BRSELECT5= @TempDir & "/~tag2f"
Global $BRSELECT8= @TempDir & "/~tag2g"
Global $BRSELECT9= @TempDir & "/~tag2h"


Global Const $ON  ="ON"
Global Const $OFF ="OFF"
Const $RECUO = 35
Const $MIN =1  ;itens
Const $MAX =12 ;itens
Const $RECUO_TOP = 45
Local $POS_SETA = 83
Local $KEY_ENTER = "0D"


Global $loopingY=1
Global $LB1, $LB2, $LB3, $LB4, $LB5, $LB6, $LB7, $LB8, $LB9, $LB10, $LB11, $LB12,$LB13, $LB14,$LB15,$LB16, $LB17
Global $LB18, $LB19, $LB20, $LB21, $LB22,$LB25,$LB26,$LB27,$LB28,$LB29,$LB30,$LB31,$LB32,$LB33,$LB34,$LB35,$LB36

;joystick
Local $RETJOY1,$RETJOY2,$RETJOY3, $JOY_STATUS[0][1],$JOY_STATUS[1][1],$JOY_STATUS[2][1], $x1, $y1,$x2, $y2,$x3, $y3

;----

Local $ESTADO, $RET, $RET2, $FREE_PLAY, $CREDITOPF, $TEMPOPF, $BLOQUEADO, $SAIR_OCIOSO,$PicFTema
Local $ADICIONAR_CREDITOS, $ENVIO_MAME,$ENVIO_TAITO,$CONTADOR_UI, $C_ATUAL, $C_GERAL
Local $MSG, $SETA, $CONTA
Local $BARRA_A,$BARRA_B,$BARRA_C,$BARRA_D,$BARRA_E,$BARRA_F

; inicia as variaveis
Local $ITEM=1
Local $FLAG_U=False
Local $FLAG_D=False
Local $FLAG_L=False
Local $FLAG_R=False
Local $FLAG_SAIR=False
Local $FLAG_START=False
Local $AUMENTA=False
Local $DIMINUE=False
Global $DEL_CONF=False
Global $DEL_CONG=False

;Teclas Teclado
Local $TECLA_UP, $TECLA_LEFT, $TECLA_RIGHT,$TECLA_DOWN,$TECLA_START


;===========================>
;LÊ VALORES ARQUIVO .ini
;===========================>

;-------------------------------------------------------------------------------------
;FREE TO PLAY
	$RET=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","BLOQUEADO","0")
		If $RET="0" Then
			$FREE_PLAY="Desbloqueado"
		Elseif $RET=1 Then
			$FREE_PLAY="Bloqueado"
		EndIf
;-------------------------------------------------------------------------------------
;Credito por fichas
	$CREDITOPF=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","CREDITOPF","0")
;-------------------------------------------------------------------------------------
;TEMPO POR FICHAS
	$RET=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","TEMPOPORFICHA","600000")
	$CONTA=(Number($RET) / 1000) /60
	$TEMPOPF=String($CONTA)
;-------------------------------------------------------------------------------------
;Sair Ocioso
	$RET=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","OCIOSO",0)
	$CONTA=(Number($RET) / 1000) /60
		if $RET >= 0 Then
			$SAIR_OCIOSO=Number($CONTA)
		Else
			$SAIR_OCIOSO=0
		EndIf
;-------------------------------------------------------------------------------------
;Tema
	Global $TEMA=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","TEMA",1)
	Global $TEMA_SAVE=$TEMA
	Global $_FileListSKIN =	_FileListToArray(_CONST_LAYOUT_FOLDER(), "*",   $FLTA_FOLDERS )

;-------------------------------------------------------------------------------------
;Liga o contador geral
	$RET =IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","LIGAR_CONTADOR","0")
		if $RET=1 then
			$CONTADOR_UI=$ON
		Else
			$CONTADOR_UI=$OFF
		EndIf
;-------------------------------------------------------------------------------------
;CONTADOR GERAL
	$RET =IniRead(_CONST_ADVMENU_INI(),"FICHEIRO","FICHAS","0")
	$RET2=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","TEMPOPORFICHA","0")
	$CONTA=(Number($RET) / Number($RET2))
	$C_ATUAL=$CONTA
	$C_GERAL=IniRead(_CONST_ADVMENU_INI(),"FICHEIRO","CONTADOR","0")
;-------------------------------------------------------------------------------------
;Envia a Ficha em MAME e Taito Segundos

$ENVIO_MAME=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","FICHA_MAME","20000")
;~ $ENVIO_TAITO=IniRead(_CONST_ADVMENU_INI(),"CONFIGURACAO","FICHA_TAITO","20000")


;----------------------------FIM LEITURA DE .INI--------------------------------------


;===================================================>
;GUI PRINCIPAL
;===================================================>
Local $GUI_PRINCIPAL=GUICreate("Arcade Configuração", 640, 568, -1, -1, BitOR($WS_DISABLED,$WS_POPUP),BitOR($WS_EX_COMPOSITED,$WS_EX_TOPMOST,$WS_EX_TOOLWINDOW))
If @error Then
	MsgBox(0,"Erro","Não foi possivel cria a interface",10)
	Return
EndIf

;===========================>
;cria as opões a serem mostradas
;===========================>

GUICtrlCreateLabel ("E-Mail: " & $__EMAIL  & "/    Tel: " & $__TELEF,  110, 63, 550)
GUIctrlSetFont(-1,11, 800, -1, $FONTE)
GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
GUICtrlSetColor ( -1, $VERMELHO)

;-------------------------------------------------------------------------------------
;item1 Modo Livre (Free To Play)

	$LB1=GUICtrlCreateLabel ("Modo Livre (Free To Play)",  $RECUO, $RECUO_TOP+45, 300,30)
	GUIctrlSetFont ($LB1,18, 800, -1, $FONTE)
	GUICtrlSetColor ( $LB1, $BRANCO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

	$LB2=GUICtrlCreateLabel ($FREE_PLAY,  460, $RECUO_TOP+48, 120,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	if $FREE_PLAY = "Desbloqueado" Then
	GUICtrlSetColor ( $LB2, $VERDE)
	Else
	GUICtrlSetColor ( $LB2, $VERMELHO)
	EndIf
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item2 livre

	$LB3=GUICtrlCreateLabel ("Crédito Por Ficha",  $RECUO, $RECUO_TOP+85, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB3, $PRETO)

	$LB4=GUICtrlCreateLabel (StringFormat("%02d",$CREDITOPF),  495,$RECUO_TOP+88, 40,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetStyle($LB4,$SS_Center)
	GUICtrlSetColor ( $LB4, $BRANCO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

	$LB31=GUICtrlCreateLabel ("Fichas",  535, $RECUO_TOP+88, 100)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( -1, $PRETO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

;-------------------------------------------------------------------------------------
;item3 Tempo Por Ficha (MINUTOS)
	$LB5=GUICtrlCreateLabel ("Tempo Por Ficha",  $RECUO, $RECUO_TOP+125, 300,30) ;	$LB5=GUICtrlCreateLabel ("MODO HABILIDADE",  $RECUO, 100, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB5, $PRETO)

	$LB6=GUICtrlCreateLabel (StringFormat("%02d",$TEMPOPF),  495, $RECUO_TOP+128, 40,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetStyle($LB6,$SS_Center)
	GUICtrlSetColor ( $LB6, $BRANCO)

	$LB32=GUICtrlCreateLabel ("Minutos",  535, $RECUO_TOP+128, 100)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( -1, $PRETO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item4 OCIOSO
	$LB11=GUICtrlCreateLabel ("Sair Ocioso",  $RECUO, $RECUO_TOP+165, 300,30) ;	$LB11=GUICtrlCreateLabel ("1 FICHA VALE [     ] CRÉDITOS",  $RECUO, 220, 500)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB11, $PRETO)

	$LB12=GUICtrlCreateLabel (StringFormat("%02d",$SAIR_OCIOSO), 495, $RECUO_TOP+168,  40,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetStyle($LB12,$SS_Center)
	GUICtrlSetColor ( $LB12, $BRANCO)
	$LB33=GUICtrlCreateLabel ("Minutos", 535, $RECUO_TOP+168, 100)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( -1, $PRETO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item5 Thema
	$LB13=GUICtrlCreateLabel ("Tema personalizado",  $RECUO, $RECUO_TOP+205, 420,30) ;	$LB13=GUICtrlCreateLabel ("TEMPO POR FICHA [       ] MINUTOS",  $RECUO, 250, 500)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
    GUICtrlSetColor ( $LB13, $PRETO)
	$LB14=GUICtrlCreateLabel ($TEMA,  450, $RECUO_TOP+208,  140,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetStyle($LB14,$SS_Center)
	GUICtrlSetColor ( $LB14, $BRANCO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item6 desabilitar contador geral
	$LB15=GUICtrlCreateLabel ("Mostrar Contador Geral",  $RECUO, $RECUO_TOP+245, 270,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB15, $PRETO)
	$LB16=GUICtrlCreateLabel ($CONTADOR_UI,  465, $RECUO_TOP+248, 100,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	if $CONTADOR_UI = $ON Then
	GUICtrlSetColor ( $LB16, $VERDE)
	Else
	GUICtrlSetColor ( $LB16, $VERMELHO)
	EndIf
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item7 zerar fichas
	$LB17=GUICtrlCreateLabel ("Zerar Fichas",  $RECUO, $RECUO_TOP+285, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB17, $PRETO)

	$LB18=GUICtrlCreateLabel ($OFF,  465, $RECUO_TOP+288, 100,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( $LB18, $VERMELHO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	$LB22=GUICtrlCreateLabel (StringFormat("%06d", number($C_ATUAL)),  565, 332, 400)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( $LB22, $VERMELHO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

;-------------------------------------------------------------------------------------
;item8 zerar contador geral
	$LB9=GUICtrlCreateLabel ("Zerar Contador Geral",  $RECUO, $RECUO_TOP+325, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB9, $PRETO)

	$LB10=GUICtrlCreateLabel ($OFF,  465, $RECUO_TOP+328, 100,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( $LB10, $VERMELHO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

	$LB21=GUICtrlCreateLabel (StringFormat("%06d", Number($C_GERAL)),  565, 372, 400)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( $LB21, $VERMELHO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item9 Enviar Ficha MAME em:
	$LB20=GUICtrlCreateLabel ("Enviar Ficha MAME em",  $RECUO, $RECUO_TOP+365, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB20, $PRETO)


	Local $iSendCoin = StringFormat("%02d",$ENVIO_MAME/1000)
	If $ENVIO_MAME <= 0 Then $iSendCoin = "OFF"
	$LB25=GUICtrlCreateLabel ($iSendCoin, 495, $RECUO_TOP+368,  40,20,$SS_CENTER)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
	GUICtrlSetColor ( $LB25, $BRANCO)
	GUICtrlSetStyle($LB25,$SS_Center)
	$LB35=GUICtrlCreateLabel ("Segundos",  535, $RECUO_TOP+368, 100)
	GUIctrlSetFont (-1,12, 800, -1, $FONTE)
	GUICtrlSetColor ( -1, $PRETO)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)
;-------------------------------------------------------------------------------------
;item11 Salvar e sair
	$LB28=GUICtrlCreateLabel ("SALVAR e SAIR",  $RECUO+200, $RECUO_TOP+445, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)

;-------------------------------------------------------------------------------------
;item12 Salvar e sair
	$LB29=GUICtrlCreateLabel ("SAIR",  $RECUO+250, $RECUO_TOP+485, 300,30)
	GUIctrlSetFont (-1,18, 800, -1, $FONTE)
	GUICtrlSetBkColor(-1, $GUI_BKCOLOR_TRANSPARENT)


;===================================>
;Imagens - SLIDE,BARRA DE SELEÇÂO
;===================================>

; Imagens  <III>
	_GUICtrlPic_Create($BRSELECT9,437,89,167 ,27)
	_GUICtrlPic_Create($BRSELECT1,474,289,83 ,25)
	_GUICtrlPic_Create($BRSELECT5,474,329,83 ,25)
	_GUICtrlPic_Create($BRSELECT5,474,369,83 ,25)
	_GUICtrlPic_Create($BRSELECT8,455,250,130 ,23)
; Imagens  III
	_GUICtrlPic_Create($BRSELECT2,498,129,33 ,25)
	_GUICtrlPic_Create($BRSELECT2,498,169,33 ,25)
	_GUICtrlPic_Create($BRSELECT2,498,209,33 ,25)
	_GUICtrlPic_Create($BRSELECT2,498,409,33 ,25)

; Barras de slide (BOLINHA)
Local $limitate1=(138/8)*$CREDITOPF
Local $limitate2=(138/59)*$TEMPOPF
Local $limitate3=(138/99)*$SAIR_OCIOSO
Local $limitate4=0,$limitate5=(138/55)*$ENVIO_MAME/1000
Local $limitate6=(138/55)*$ENVIO_TAITO/1000

$BARRA_A=_GUICtrlPic_Create($BRSELECT3,303+(138/8)*$CREDITOPF,137,12 ,12)
$BARRA_B=_GUICtrlPic_Create($BRSELECT3,318+(138/59)*$TEMPOPF,177,12 ,12)
$BARRA_C=_GUICtrlPic_Create($BRSELECT3,320+(138/99)*$SAIR_OCIOSO,217,12 ,12)
$BARRA_E=_GUICtrlPic_Create($BRSELECT3,318+(138/60)*($ENVIO_MAME/1000),417,12 ,12)

; Barras de slide (LINHA)
_GUICtrlPic_Create($BRSELECT4,320,140,150 ,5)
_GUICtrlPic_Create($BRSELECT4,320,180,150 ,5)
_GUICtrlPic_Create($BRSELECT4,320,220,150 ,5)
_GUICtrlPic_Create($BRSELECT4,320,420,150 ,5)

;IMAGEM SETA e BACKGROUND
$SETA=GUICtrlCreateLabel("",5, $POS_SETA,630,40)
GUICtrlSetBkColor(-1,0x0059B2)
GUICtrlSetState(-1,$GUI_DISABLE )
_GUICtrlPic_Create($BKGround,0,0,640 ,568)

;inicia a GUI
GUISetState (@SW_SHOW,$GUI_PRINCIPAL)

Local $Handle=WinGetPos($GUI_PRINCIPAL)

Local $GUITEMA=GUICreate("", 435,315, $Handle[0]+20,$Handle[1]+125, BitOR($WS_DISABLED,$WS_POPUP), BitOR($WS_EX_TOOLWINDOW,$WS_EX_TOPMOST),$GUI_PRINCIPAL)
If @error Then
	MsgBox(0,"Erro","Não foi possivel cria a interface",10)
	Return
EndIf
GUISetBkColor(0xff00ff)
Local $_Label_INFO = GUICtrlCreateLabel("",0,0,425,20,$SS_CENTER+$SS_CENTERIMAGE)
GUICtrlSetFont(-1,15)
GUICtrlSetColor(-1,0xffffff)
Local $PicTema=GUICtrlCreatePic($PicFTema, 5, 20, 425,290)
GUISetState (@SW_HIDE,$GUITEMA)
;=================================================>
;Inicia o WHILE principal
;=================================================>
While $loopingY
	if GUIGetMsg() = $GUI_EVENT_CLOSE then ExitLoop
	_JOY_UPDATE($JOYSTK)

;------------------------------------------------------
	;up
	If (_IsPressed($KEY_UP)) or _JOY_GET_AXIS_BUTTONS($JOYSTK[0], 1) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 1) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 1) < (-5000) And ($ITEM > $MIN) And ($FLAG_U=False) Then
		$FLAG_u=True
		$POS_SETA-=40
		if ($ITEM = 11)	Then
			$POS_SETA-=40
			$ITEM-=1
			GUICtrlSetColor ( $LB26, $BRANCO)
			GUICtrlSetColor ( $LB28, $PRETO)
			GUICtrlSetColor ( $LB36, $BRANCO)
		EndIf
			If ($ITEM = 2) Then
			GUICtrlSetColor ( $LB1, $BRANCO)
			GUICtrlSetColor ( $LB3, $PRETO)
			GUICtrlSetColor ( $LB31, $PRETO)
			ElseIf ($ITEM = 3) Then
			GUICtrlSetColor ( $LB3, $BRANCO)
			GUICtrlSetColor ( $LB5, $PRETO)
			GUICtrlSetColor ( $LB31, $BRANCO)
			GUICtrlSetColor ( $LB32, $PRETO)
			ElseIf ($ITEM = 4) Then
			GUICtrlSetColor ( $LB5,  $BRANCO)
			GUICtrlSetColor ( $LB11, $PRETO)
			GUICtrlSetColor ( $LB33, $PRETO)
			GUICtrlSetColor ( $LB32, $BRANCO)

			ElseIf ($ITEM = 5) Then
			GUICtrlSetColor ( $LB11, $BRANCO)
			GUICtrlSetColor ( $LB13, $PRETO)
			GUICtrlSetColor ( $LB33, $BRANCO)
			GUICtrlSetColor ( $LB34, $PRETO)
			GUISetState (@SW_HIDE,$GUITEMA)
			ElseIf ($ITEM = 6) Then
			GUICtrlSetColor ( $LB13, $BRANCO)
			GUICtrlSetColor ( $LB15, $PRETO)
			GUICtrlSetColor ( $LB34, $BRANCO)
			ElseIf ($ITEM = 7) Then
			GUICtrlSetColor ( $LB15, $BRANCO)
			GUICtrlSetColor ( $LB17, $PRETO)
			ElseIf ($ITEM = 8) Then
			GUICtrlSetColor ( $LB17, $BRANCO)
			GUICtrlSetColor ( $LB9, $PRETO)
			ElseIf ($ITEM = 9) Then
			GUICtrlSetColor ( $LB9, $BRANCO)
			GUICtrlSetColor ( $LB20,  $PRETO)
			GUICtrlSetColor ( $LB35, $PRETO)
			ElseIf ($ITEM = 10) Then
			GUICtrlSetColor ( $LB20, $BRANCO)
			GUICtrlSetColor ( $LB26, $PRETO)
			GUICtrlSetColor ( $LB36, $PRETO)
			GUICtrlSetColor ( $LB35, $BRANCO)
			ElseIf ($ITEM = 11) Then
			GUICtrlSetColor ( $LB26, $BRANCO)
			GUICtrlSetColor ( $LB28, $PRETO)
			GUICtrlSetColor ( $LB36, $BRANCO)
			ElseIf ($ITEM = 12) Then
			GUICtrlSetColor ( $LB28, $BRANCO)
			GUICtrlSetColor ( $LB29, $PRETO)
			EndIf

		$ITEM-=1
		GUICtrlSetPos($SETA, 5, $POS_SETA)
	EndIf
	If Not((_IsPressed($KEY_UP)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 1) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 1) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 1) < (-5000)) And ($FLAG_U=True)) Then
		$FLAG_u=False
	EndIf

;--------------------------------------------------------

;down
	If (_IsPressed($KEY_DOWN)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 1) > (5000)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[1], 1) > (5000)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[2], 1) > (5000))  And ($ITEM < $MAX) And ($FLAG_D=False) Then
		$FLAG_D=True
		$POS_SETA+=40
		If ($ITEM = 9) Then
			$POS_SETA+=40
			$ITEM+=1
			GUICtrlSetColor ( $LB20, $PRETO)
			GUICtrlSetColor ( $LB26, $BRANCO)
			GUICtrlSetColor ( $LB35, $PRETO)
			GUICtrlSetColor ( $LB36, $BRANCO)
		EndIf

		If ($ITEM = 1) Then
			GUICtrlSetColor ( $LB1, $PRETO)
			GUICtrlSetColor ( $LB3, $BRANCO)
			GUICtrlSetColor ( $LB31, $BRANCO)
			ElseIf ($ITEM = 2) Then
			GUICtrlSetColor ( $LB3, $PRETO)
			GUICtrlSetColor ( $LB5, $BRANCO)
			GUICtrlSetColor ( $LB31, $PRETO)
			GUICtrlSetColor ( $LB32, $BRANCO)
			ElseIf ($ITEM = 3) Then
			GUICtrlSetColor ( $LB5,  $PRETO)
			GUICtrlSetColor ( $LB11, $BRANCO)
			GUICtrlSetColor ( $LB32, $PRETO)
			GUICtrlSetColor ( $LB33, $BRANCO)
			ElseIf ($ITEM = 4) Then
			GUICtrlSetColor ( $LB11, $PRETO)
			GUICtrlSetColor ( $LB13, $BRANCO)
			GUICtrlSetColor ( $LB33, $PRETO)
			GUICtrlSetColor ( $LB34, $BRANCO)
			ElseIf ($ITEM = 5) Then
			GUICtrlSetColor ( $LB13, $PRETO)
			GUICtrlSetColor ( $LB15, $BRANCO)
			GUICtrlSetColor ( $LB34, $PRETO)
			GUISetState (@SW_HIDE,$GUITEMA)
			ElseIf ($ITEM = 6) Then
			GUICtrlSetColor ( $LB15, $PRETO)
			GUICtrlSetColor ( $LB17, $BRANCO)
			ElseIf ($ITEM = 7) Then
			GUICtrlSetColor ( $LB17, $PRETO)
			GUICtrlSetColor ( $LB9, $BRANCO)
			ElseIf ($ITEM = 8) Then
			GUICtrlSetColor ( $LB9, $PRETO)
			GUICtrlSetColor ( $LB20,  $BRANCO)
			GUICtrlSetColor ( $LB35, $BRANCO)
			ElseIf ($ITEM = 9) Then
			GUICtrlSetColor ( $LB20, $PRETO)
			GUICtrlSetColor ( $LB26, $BRANCO)
			GUICtrlSetColor ( $LB35, $PRETO)
			GUICtrlSetColor ( $LB36, $BRANCO)
			ElseIf ($ITEM = 10) Then
			GUICtrlSetColor ( $LB26, $PRETO)
			GUICtrlSetColor ( $LB28, $BRANCO)
			GUICtrlSetColor ( $LB36, $PRETO)
			ElseIf ($ITEM = 11) Then
			GUICtrlSetColor ( $LB28, $PRETO)
			GUICtrlSetColor ( $LB29, $BRANCO)
			EndIf

		$ITEM+=1
		GUICtrlSetPos($SETA, 5, $POS_SETA)
	EndIf
	If Not((_IsPressed($KEY_DOWN)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 1) > (5000)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[1], 1) > (5000)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[2], 1) > (5000))  And ($ITEM < $MAX) And ($FLAG_D=True)) Then
		$FLAG_D=False
	EndIf


;~ ;=========================>
;~ ;Botões ESQUERDA e DIREITA
;~ ;=========================>

;--------------------------------------------------------
;left

	if (_IsPressed($KEY_LEFT)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 0) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 0) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 0) < (-5000)) And ($FLAG_L=False) Then
		$FLAG_L=True
		$DIMINUE=True

	ElseIf Not((_IsPressed($KEY_LEFT)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 0) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 0) < (-5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 0) < (-5000)) And ($FLAG_L=True)) Then
		$FLAG_L=False
	EndIf
;--------------------------------------------------------
;right
	If (_IsPressed($KEY_RIGHT)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 0) > (5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 0) > (5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 0) > (5000)) And ($FLAG_R=False) Then
		$FLAG_R=True
		$AUMENTA=True
	ElseIf Not((_IsPressed($KEY_RIGHT)) or (_JOY_GET_AXIS_BUTTONS($JOYSTK[0], 0) > (5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[1], 0) > (5000) or _JOY_GET_AXIS_BUTTONS($JOYSTK[2], 0) > (5000)) And ($FLAG_R=True)) Then
		$FLAG_R=False
	EndIf


Select
	;salvar e sair
	case(_JOY_GET_BUTTON($JOYSTK[0],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[1],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[2],$__BOTAO_ENTER)) or (_IsPressed($KEY_ENTER)) And ($FLAG_START=False) And $ITEM=11
		$FLAG_START=True
	Case Not((_JOY_GET_BUTTON($JOYSTK[0],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[1],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[2],$__BOTAO_ENTER)) or (_IsPressed($KEY_ENTER))) And ($FLAG_START=True) And $ITEM=11
		$FLAG_START=False
		SalvaDados()
		Sleep(100)
		ExitLoop

	;sair
	Case (_JOY_GET_BUTTON($JOYSTK[0],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[1],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[2],$__BOTAO_ENTER)) or (_IsPressed($KEY_ENTER)) And ($FLAG_SAIR=False) And $ITEM=12
		$FLAG_SAIR=True

	Case Not((_JOY_GET_BUTTON($JOYSTK[0],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[1],$__BOTAO_ENTER) or _JOY_GET_BUTTON($JOYSTK[2],$__BOTAO_ENTER)) or (_IsPressed($KEY_ENTER))) And  ($FLAG_SAIR=True) And $ITEM=12
		$FLAG_SAIR=False
		ExitLoop
	;------------------------------------
	Case ($ITEM=1) And ($DIMINUE=True)
		$DIMINUE=False
		$ESTADO=GUICtrlRead($LB2)

		If ($ESTADO = "Bloqueado") Then
			GUICtrlSetData($LB2,"Desbloqueado")
			GUICtrlSetColor($LB2, $VERDE)
		EndIf

	Case ($ITEM=1) And ($AUMENTA=True)
		$AUMENTA=False
		$ESTADO=GUICtrlRead($LB2)
		GUICtrlSetColor($LB2, $VERMELHO)
		If ($ESTADO = "Desbloqueado") Then GUICtrlSetData($LB2,"Bloqueado")

;------------------------------------------------------------------------------------------------------
;item2 - Credito por fichas
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=2) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB4)
			If (Number($ESTADO) > 1) Then
				GUICtrlSetData($LB4,StringFormat("%02d",String(Number($ESTADO)-1)))
				$limitate1-=138/8
				ControlMove($GUI_PRINCIPAL, "", $BARRA_A, 303+$limitate1, 137)
			EndIf

		Case ($ITEM=2) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB4)
			If (Number($ESTADO) < 9) Then
				GUICtrlSetData($LB4,StringFormat("%02d",String(Number($ESTADO)+1)))
				$limitate1+=138/8
				ControlMove($GUI_PRINCIPAL, "", $BARRA_A, 303+$limitate1, 137)
		EndIf
;------------------------------------------------------------------------------------------------------
;item 3 Tempo por ficha
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=3) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB6)
			If (Number($ESTADO) > 1) Then
				GUICtrlSetData($LB6,StringFormat("%02d",String(Number($ESTADO)-1)))
				$limitate2-=138/59
				ControlMove($GUI_PRINCIPAL, "", $BARRA_B, 318+$limitate2, 177)
			EndIf
			Case ($ITEM=3) And ($AUMENTA=True)
				$AUMENTA=False
			$ESTADO=GUICtrlRead($LB6)
			If (Number($ESTADO) < 60) Then
				GUICtrlSetData($LB6,StringFormat("%02d",String(Number($ESTADO)+1)))
				$limitate2+=138/59
				ControlMove($GUI_PRINCIPAL, "", $BARRA_B, 318+$limitate2, 177)
			EndIf
;------------------------------------------------------------------------------------------------------
;item4 - Sair Ocioso
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=4) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB12)
			If (Number($ESTADO) > 0) Then
				GUICtrlSetData($LB12,StringFormat("%02d",String(Number($ESTADO)-1)))
				$limitate3-=138/99
				ControlMove($GUI_PRINCIPAL, "", $BARRA_C, 320+$limitate3, 217)
			EndIf

		Case ($ITEM=4) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB12)
			If (Number($ESTADO) < 99) Then
				GUICtrlSetData($LB12,StringFormat("%02d",String(Number($ESTADO)+1)))
				$limitate3+=138/99
				ControlMove($GUI_PRINCIPAL, "", $BARRA_C, 320+$limitate3, 217)
			EndIf
;------------------------------------------------------------------------------------------------------
;item5 - Tema
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=5) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB14)
			GUICtrlSetData($LB18,$OFF)
			GUICtrlSetColor ( $LB18, $VERMELHO)

			If (Number($ESTADO) > 1) Then
				GUICtrlSetData($LB14,$ESTADO-1)
				$TEMA = $ESTADO-1
				Local $i_file = _CONST_LAYOUT_FOLDER() & $_FileListSKIN[$TEMA] & "\demo.jpg"
				GUICtrlSetData($_Label_INFO,$_FileListSKIN[$TEMA])
					if Not FileExists($i_file) Then
						GUICtrlSetImage($PicTema, _CONST_LAYOUT_FOLDER() & "\nopic.jpg")
					Else
						GUICtrlSetImage($PicTema,$i_file)
					EndIf
			EndIf
			GUISetState (@SW_SHOWNOACTIVATE,$GUITEMA)

		Case ($ITEM=5) And ($AUMENTA=True)
		    $AUMENTA=False
			GUICtrlSetData($LB18,$OFF)
			$ESTADO=GUICtrlRead($LB14)
			GUICtrlSetColor ( $LB18, $VERMELHO)

			If (Number($ESTADO) < UBound($_FileListSKIN)-1) Then
				GUICtrlSetData($LB14,$ESTADO+1)
				$TEMA = $ESTADO+1
				Local $i_file = _CONST_LAYOUT_FOLDER() & $_FileListSKIN[$TEMA] & "\demo.jpg"
				GUICtrlSetData($_Label_INFO,$_FileListSKIN[$TEMA])
			if Not FileExists($i_file) Then
				GUICtrlSetImage($PicTema, _CONST_LAYOUT_FOLDER() & "\nopic.jpg")
			Else
				GUICtrlSetImage($PicTema,$i_file)
			EndIf
			EndIf

			GUISetState (@SW_SHOWNOACTIVATE,$GUITEMA)

;------------------------------------------------------------------------------------------------------
;item6  Mostrar contador geral
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=6) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB16)
			If $ESTADO <> $OFF Then GUICtrlSetData($LB16,$OFF)
			GUICtrlSetColor ( $LB16, $VERMELHO)

		Case ($ITEM=6) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB16)
			If $ESTADO <> $ON Then GUICtrlSetData($LB16,$ON)
			GUICtrlSetColor ( $LB16, $VERDE)
;------------------------------------------------------------------------------------------------------
;item7 zerar créditos
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=7) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB18)

			If $ESTADO <> $OFF Then GUICtrlSetData($LB18,$OFF)
			GUICtrlSetColor ( $LB18, $VERMELHO)
		Case ($ITEM=7) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB18)
			;GUICtrlSetData($LB14,StringFormat("%02d",0))
			;ControlMove($GUI_PRINCIPAL, "", $BARRA_D, 320, 257)
			$limitate4=0
			If $ESTADO <> $ON Then
				GUICtrlSetData($LB18,$ON)
				GUICtrlSetColor ( $LB18, $VERDE)
				GUICtrlSetData($LB22,"000000")
				$DEL_CONG=True
				Sleep(200)
				GUICtrlSetData($LB18,$OFF)
				GUICtrlSetColor ( $LB18, $VERMELHO)

			EndIf
;------------------------------------------------------------------------------------------------------
;item8 zerar contador geral
;------------------------------------------------------------------------------------------------------
		Case ($ITEM=8) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB10)

			If $ESTADO <> $OFF Then GUICtrlSetData($LB10,$OFF)
			GUICtrlSetColor ( $LB10, $VERMELHO)
		Case ($ITEM=8) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB10)
			If $ESTADO <> $ON Then
				GUICtrlSetData($LB10,$ON)
				GUICtrlSetColor ( $LB10, $VERDE)
				GUICtrlSetData($LB21,"000000")
				$DEL_CONF=True
				Sleep(200)
				GUICtrlSetData($LB10,$OFF)
				GUICtrlSetColor ( $LB10, $VERMELHO)
			EndIf
;------------------------------------------------------------------------------------------------------
;item9 - Enviar mame ficha MAME em
;------------------------------------------------------------------------------------------------------
			Case ($ITEM=9) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB25)
			If (Number($ESTADO) > 0) or $ESTADO <> "OFF" Then
				if $ESTADO-1 = 0 Then
					GUICtrlSetData($LB25,"OFF")
				Else
					GUICtrlSetData($LB25,StringFormat("%02d",String(Number($ESTADO)-1)))
				EndIf
				$limitate5-=138/60
				ControlMove($GUI_PRINCIPAL, "", $BARRA_E, 318+$limitate5, 417)
			EndIf
		Case ($ITEM=9) And ($AUMENTA=True)
					$AUMENTA=False
			$ESTADO=GUICtrlRead($LB25)
			If (Number($ESTADO) < 60) Then
				GUICtrlSetData($LB25,StringFormat("%02d",String(Number($ESTADO)+1)))
				$limitate5+=138/60
				ControlMove($GUI_PRINCIPAL, "", $BARRA_E, 318+$limitate5, 417)
			EndIf
;------------------------------------------------------------------------------------------------------
;item10 - Enviar taito ficha TAITO em
;------------------------------------------------------------------------------------------------------
			Case ($ITEM=10) And ($DIMINUE=True)
			$DIMINUE=False
			$ESTADO=GUICtrlRead($LB27)
			If (Number($ESTADO) > 5) Then
				GUICtrlSetData($LB27,StringFormat("%02d",String(Number($ESTADO)-1)))
			$limitate6-=138/55
				ControlMove($GUI_PRINCIPAL, "", $BARRA_F, 308+$limitate6, 457)
			EndIf
			Case ($ITEM=10) And ($AUMENTA=True)
			$AUMENTA=False
			$ESTADO=GUICtrlRead($LB27)
			If (Number($ESTADO) < 60) Then
				GUICtrlSetData($LB27,StringFormat("%02d",String(Number($ESTADO)+1)))
				$limitate6+=138/55
				ControlMove($GUI_PRINCIPAL, "", $BARRA_F, 308+$limitate6, 457)
			EndIf
EndSelect

;------------------------------------------------------------------------------------------------------
Sleep(80)
Wend
GUIDelete($GUI_PRINCIPAL)
EndFunc

;======================>
;Inicio das funções
;======================>
Func SalvaDados()
	Local $ESTADO,$calcula, $calcula2, $CONTAdor
;-----------------------------------------------------------------------------------
;Modo Livre (Free To Play)
;-----------------------------------------------------------------------------------
$ESTADO=GUICtrlRead($LB2)
	If $ESTADO = "Desbloqueado" Then
		IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","BLOQUEADO","0")
	Else
		IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","BLOQUEADO","1")
	EndIf
;-----------------------------------------------------------------------------------
;Credito Por fichas
;-----------------------------------------------------------------------------------
$ESTADO=GUICtrlRead($LB4)
	IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","CREDITOPF",$ESTADO)
;-----------------------------------------------------------------------------------
;Tempo por fichas
;-----------------------------------------------------------------------------------
$ESTADO=GUICtrlRead($LB6)
 	$calcula=(Number($ESTADO) * 1000 ) * 60
	IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","TEMPOPORFICHA",String($calcula))
;-----------------------------------------------------------------------------------
;SAIR OCIOSO
;-----------------------------------------------------------------------------------
	$ESTADO=GUICtrlRead($LB12)
	$calcula=(Number($ESTADO) * 1000 ) * 60
	IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","OCIOSO",$calcula)


;-----------------------------------------------------------------------------------
;ZERAR FICHAS (SALVAR)
;-----------------------------------------------------------------------------------
	$ESTADO=GUICtrlRead($LB18)
	If $ESTADO = $OFF and $DEL_CONF=True Then
		IniWrite(_CONST_ADVMENU_INI(),"FICHEIRO","FICHAS","0")
		GUICtrlSetData($LB18,$OFF)
		GUICtrlSetColor ($LB18, $VERMELHO)
		GUICtrlSetData($LB21, "000000")
	EndIf
;-----------------------------------------------------------------------------------
;ZERAR CONTADOR GERAL
;-----------------------------------------------------------------------------------
	$ESTADO=GUICtrlRead($LB10)
	If $ESTADO = $OFF And $DEL_CONG=True Then
		IniWrite(_CONST_ADVMENU_INI(),"FICHEIRO","CONTADOR","0")
		GUICtrlSetData($LB10,$OFF)
		GUICtrlSetColor ($LB10, $VERMELHO)
		GUICtrlSetData($LB21, "000000")
	EndIf
;-----------------------------------------------------------------------------------
;MOSTRAR CONTADOR GERAL NA TELA (SALVAR)
;-----------------------------------------------------------------------------------
$ESTADO=GUICtrlRead($LB16)
	If $ESTADO = $ON Then
		IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","LIGAR_CONTADOR","1")
	Else
		IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","LIGAR_CONTADOR","0")
	EndIf
;-----------------------------------------------------------------------------------
;ENVIAR FICHAS MAME EM?
;-----------------------------------------------------------------------------------
$ESTADO=GUICtrlRead($LB25)
	$calcula=(Number($ESTADO) * 1000 )
	IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","FICHA_MAME",$calcula)

;------------------------------------------------------------------------------------
;Mensagem de salvamento
;------------------------------------------------------------------------------------

_MUDAR_SKIN()

	SoundPlay(_CONST_SOUND_FOLDER() & "salvadados.wav", 1)
	$loopingY=0
EndFunc


Func _MUDAR_SKIN()
	if $TEMA_SAVE = $TEMA Then Return
		Local $_FileListAMP   = _FileListToArray(_CONST_REGIST_PAGE() & "\Layouts\", "*.amp",   $FLTA_FILES)
		Local $_FileListPASTA = _FileListToArray(_CONST_REGIST_PAGE() & "\Layouts\" & $_FileListSKIN[$TEMA], "*.amp",   $FLTA_FILES)

	For $i=1 to UBound($_FileListAMP)-1
		Local $iFile = _CONST_LAYOUT_FOLDER() & $_FileListSKIN[$TEMA]& "\" & $_FileListAMP[$i]

		if Not FileExists($iFile) Then
;~
			if UBound($_FileListPASTA)-1 >= 1 Then FileCopy(_CONST_LAYOUT_FOLDER() & $_FileListSKIN[$TEMA]& "\" & $_FileListPASTA[1],_CONST_REGIST_PAGE() & "\Layouts\" & $_FileListAMP[$i],1)
		Else
			FileCopy(_CONST_LAYOUT_FOLDER() & $_FileListSKIN[$TEMA]& "\" & $_FileListAMP[$i],_CONST_REGIST_PAGE() & "\Layouts\" & $_FileListAMP[$i],1)
		EndIf
	Next
	IniWrite(_CONST_ADVMENU_INI(),"CONFIGURACAO","TEMA",$TEMA)
EndFunc
