#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.0.0
 Author:         AdmiralAlkex

 Script Function:
	SDL UDF


 Random Info:
	Remember to eat one cup of ******* per day or you will ******** ***.


 Version history
		v1		(626 lines over 69 functions)
	Initial version (many functions missing or broken, very few give the expected return values).

		v2		(466 lines over 56 functions)
	Preparation for public release.

		v3		(467 lines)
	Sent to testers.

		v4		(469 lines)
	Initial release to forum.

		v5		(1372 lines over 160-3 functions)
	Changed initialization routines.
	Merged _SDL_Startup with _SDL_Init.
	Merged _SDL_Shutdown with _SDL_Quit.
	Macros moved.
	Implemented alot of graphics funcs + SDL_gfx.dll.
	Started working on SDL_draw, SDL_prim & ffmpeg.dll.

		v6		(1379 lines)
	Second release to forum.

		v7		(1260 lines over 116 (about 5 of them non-working) functions)
	Implemented _SDL_WM_GetCaption() posted by ProgAndy
	Split include into one file for each dll and rearranged the regions.
	Changed initialization routines.
	Implemented version.h, cpuinfo.h, error.h and timer.h
	Done some experiments with events

		v8		(1263 lines)
	Third release to the forum.

		v9		(1412 lines over 126 funcs)
	Implemented mutexes
	Implemented SDL_image.dll

		v10		(1413 lines)
	Fourth release to the forum.

		v11		(1387 lines over 125 funcs)
	Fixed SDL_WarpMouse()
	Implemented SDL_sprig
	Updated SDL_gfx
	Added new _SDL_WM_GetCaption() from Progandy
	Fixed _SDL_SaveBMP_RW()
	Implemented SDL_sge

		v12		(1390 lines)
	Fifth release to the forum.

		v13		(??? lines)
	Doesn't exist.
	I decided on a new way of versioning things, that should mean more compatibility with AutoIt's UDF rules, and SVN if I decide to go that way in the future.
	v13 is nothing more than a dumping ground for everything old.

		v14		(??? lines)
	SDL_mixer implemented

		After v14	(1531 lines over 143 funcs)
	No idea, I didn't comment very much. Someone compare the files or something.
	Reimplementing SDL_PollEvent (still far from  original, but whatever....)
	Implementing SDL_PollEventEasy. (Invented it myself. For lazy people, by lazy people)
	Title for GUI now automatically set to @ScriptName to prevent memory-error on _SDL_WM_GetCaption() if no title set yet.
	_SDL_SetTimer() finally fixed!!


 Missing/Todo
	There are stuff missing from:
	SDL_keyboard.h
	SDL_events.h
	SDL_loadso.h			(Not useful at all but could be an interesting challenge)
	SDL_mouse.h
	SDL_mutex.h
	SDL_opengl.h			(lol)
	SDL_rwops.h				(Read & load from files & memory, this would be really interesting to have but I am not sure it's struct can be converted to AutoIt)
	SDL_stdinc.h			(Not useful at all but could be an interesting challenge)
	SDL_syswm.h
	SDL_thread.h
	SDL_video.h
	+ Lots of other libraries.

#ce ----------------------------------------------------------------------------

#include-once

#Region UDF stuff
Global $__SDL_DLL = -1
Global $__SDL_DLL_ffmpeg = -1
Global $__SDL_DLL_draw = -1
Global $__SDL_Callback_Funcs
#EndRegion

#Region Globals (Initialization)
Global Const $_SDL_INIT_TIMER						= 0x00000001
Global Const $_SDL_INIT_AUDIO						= 0x00000010
Global Const $_SDL_INIT_VIDEO						= 0x00000020
Global Const $_SDL_INIT_CDROM						= 0x00000100
Global Const $_SDL_INIT_JOYSTICK					= 0x00000200
Global Const $_SDL_INIT_EVERYTHING					= 0x0000FFFF	;All of the above
Global Const $_SDL_INIT_NOPARACHUTE					= 0x00100000	;Don't catch fatal signals
Global Const $_SDL_INIT_EVENTTHREAD					= 0x01000000	;Not supported on all OS's

Global Enum $_SDL_ENOMEM, $_SDL_EFREAD, $_SDL_EFWRITE, $_SDL_EFSEEK, $_SDL_UNSUPPORTED, $_SDL_LASTERROR

;For convenience
Global Const $_SDL_ErrorCode[6]=[$_SDL_ENOMEM, $_SDL_EFREAD, $_SDL_EFWRITE, $_SDL_EFSEEK, $_SDL_UNSUPPORTED, $_SDL_LASTERROR]
#EndRegion

#Region Globals (Video)
Global Const $_SDL_ALPHA_OPAQUE						= 255
Global Const $_SDL_ALPHA_TRANSPARENT				= 0

Global Const $tagSDL_Rect = "short x;short y;ushort w;ushort h"

;Monoceres skrev flag istället för flags :P
Global Const $tagSDL_SURFACE="uint flags;ptr format;int w;int h;ushort pitch;ptr pixels;int offset;ptr hwdata;" & _
							"ptr cliprect;uint unused1;uint locked;ptr map;uint formatversion;int refcount;"

;Valik and wus helped with this one
Global Const $tagSDL_VideoInfo="uint;uint;ptr;int;int"

Global Const $tagSDL_PixelFormat="ptr palette;ubyte BitsPerPixel;ubyte BytesPerPixel;ubyte Rloss;ubyte Gloss;ubyte Bloss;" & _
								"ubyte Aloss;ubyte Rshift;ubyte Gshift;ubyte Bshift;ubyte Ashift;uint Rmask;uint Gmask;" & _
								"uint Bmask;uint Amask;uint colorkey;ubyte alpha"

;/* These are the currently supported flags for the SDL_surface */
;/* Available for SDL_CreateRGBSurface() or SDL_SetVideoMode() */
Global Const $_SDL_SWSURFACE						= 0x00000000	;/* Surface is in system memory */
Global Const $_SDL_HWSURFACE						= 0x00000001	;/* Surface is in video memory */
Global Const $_SDL_ASYNCBLIT						= 0x00000004	;/* Use asynchronous blits if possible */
;/* Available for SDL_SetVideoMode() */
Global Const $_SDL_ANYFORMAT						= 0x10000000	;/* Allow any video depth/pixel-format */
Global Const $_SDL_HWPALETTE						= 0x20000000	;/* Surface has exclusive palette */
Global Const $_SDL_DOUBLEBUF						= 0x40000000	;/* Set up double-buffered video mode */
Global Const $_SDL_FULLSCREEN						= 0x80000000	;/* Surface is a full screen display */
Global Const $_SDL_OPENGL    						= 0x00000002	;/* Create an OpenGL rendering context */
Global Const $_SDL_OPENGLBLIT						= 0x0000000A	;/* Create an OpenGL rendering context and use it for blitting */
Global Const $_SDL_RESIZABLE						= 0x00000010	;/* This video mode may be resized */
Global Const $_SDL_NOFRAME							= 0x00000020	;/* No window caption or edge frame */
;/* Used internally (read-only) */
Global Const $_SDL_HWACCEL							= 0x00000100	;/* Blit uses hardware acceleration */
Global Const $_SDL_SRCCOLORKEY						= 0x00001000	;/* Blit uses a source color key */
Global Const $_SDL_RLEACCELOK						= 0x00002000	;/* Private flag */
Global Const $_SDL_RLEACCEL							= 0x00004000	;/* Surface is RLE encoded */
Global Const $_SDL_SRCALPHA							= 0x00010000	;/* Blit uses source alpha blending */
Global Const $_SDL_PREALLOC							= 0x01000000	;/* Surface uses preallocated memory */

Global Enum $_SDL_GRAB_QUERY = -1, $_SDL_GRAB_OFF, $_SDL_GRAB_ON, $_SDL_GRAB_FULLSCREEN

;Used with SDL_GetVideoInfo
Global Const $_SDL_HwAvailable						= 0x00000001	;/* Can you create hardware surfaces? */
Global Const $_SDL_WmAvailable						= 0x00000002	;/* Can you talk to a window manager? */
Global Const $_SDL_BlitHw							= 0x00000400	;/* Accelerated blits HW --> HW */
Global Const $_SDL_BlitHwCC							= 0x00000800	;/* Accelerated blits with Colorkey */
Global Const $_SDL_BlitHwA							= 0x00001000	;/* Accelerated blits with Alpha */
Global Const $_SDL_BlitSw							= 0x00002000	;/* Accelerated blits SW --> HW */
Global Const $_SDL_BlitSwCC							= 0x00004000	;/* Accelerated blits with Colorkey */
Global Const $_SDL_BlitSwA							= 0x00008000	;/* Accelerated blits with Alpha */
Global Const $_SDL_BlitFill							= 0x00010000	;/* Accelerated color fill */

;For convenience
Global Const $_SDL_IsAvailable[9]=[$_SDL_HwAvailable, $_SDL_WmAvailable, $_SDL_BlitHw, $_SDL_BlitHwCC, $_SDL_BlitHwA, $_SDL_BlitSw, $_SDL_BlitSwCC, $_SDL_BlitSwA, $_SDL_BlitFill]
#EndRegion

#Region Globals (Event)
Global Const $_SDL_BUTTON_LEFT						= 1
Global Const $_SDL_BUTTON_MIDDLE					= 2
Global Const $_SDL_BUTTON_RIGHT						= 3
Global Const $_SDL_BUTTON_WHEELUP					= 4
Global Const $_SDL_BUTTON_WHEELDOWN					= 5
Global Const $_SDL_BUTTON_X1						= 6
Global Const $_SDL_BUTTON_X2						= 7

Global Const $_SDL_QUERY							= -1
Global Const $_SDL_IGNORE							= 0
Global Const $_SDL_DISABLE							= 0		;Found in header file, I have no idea if it is ever used
Global Const $_SDL_ENABLE							= 1

Global Const $_SDL_DEFAULT_REPEAT_DELAY				= 500
Global Const $_SDL_DEFAULT_REPEAT_INTERVAL			= 30

;/* The available application states */
Global Const $_SDL_APPMOUSEFOCUS					= 0x01		;/* The app has mouse coverage */
Global Const $_SDL_APPINPUTFOCUS					= 0x02		;/* The app has input focus */
Global Const $_SDL_APPACTIVE						= 0x04		;/* The application is active */

;/* General keyboard/mouse state definitions */
Global Const $_SDL_RELEASED							= 0
Global Const $_SDL_PRESSED							= 1

;/* Event enumerations */
;/* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
;/* This last event is only for bounding internal arrays. It is the number of bits in the event mask datatype -- Uint32 */
Global Enum $_SDL_NOEVENT, $_SDL_ACTIVEEVENT, $_SDL_KEYDOWN, $_SDL_KEYUP, $_SDL_MOUSEMOTION, $_SDL_MOUSEBUTTONDOWN, $_SDL_MOUSEBUTTONUP, $_SDL_JOYAXISMOTION, $_SDL_JOYBALLMOTION, $_SDL_JOYHATMOTION, $_SDL_JOYBUTTONDOWN, $_SDL_JOYBUTTONUP, $_SDL_QUIT, $_SDL_SYSWMEVENT, $_SDL_EVENT_RESERVEDA, $_SDL_EVENT_RESERVEDB, $_SDL_VIDEORESIZE, $_SDL_VIDEOEXPOSE, $_SDL_EVENT_RESERVED2, $_SDL_EVENT_RESERVED3, $_SDL_EVENT_RESERVED4, $_SDL_EVENT_RESERVED5, $_SDL_EVENT_RESERVED6, $_SDL_EVENT_RESERVED7, $_SDL_USEREVENT, $_SDL_NUMEVENTS = 32

;/* Application visibility event structure */
Global Const $tagSDL_ActiveEvent="ubyte type;ubyte gain;ubyte state"

;/* Application visibility event structure */
;Not finished/Maybe doesn't work/Untested
Global Const $tagSDL_KeyboardEvent="ubyte type;ubyte which;ubyte state;ptr keysym"

;/* Mouse motion event structure */
Global Const $tagSDL_MouseMotionEvent="ubyte type;ubyte which;ubyte state;ushort x;ushort y;short xrel;short yrel"

;/* Mouse button event structure */
Global Const $tagSDL_SDL_MouseButtonEvent ="ubyte type;ubyte which;ubyte button;ubyte state;ushort x;ushort y"

;/* Joystick axis motion event structure */
Global Const $tagSDL_JoyAxisEvent ="ubyte type;ubyte which;ubyte axis;short value"

;/* Joystick trackball motion event structure */
Global Const $tagSDL_JoyBallEvent ="ubyte type;ubyte which;ubyte ball;short xrel;short yrel"

;/* Joystick hat position change event structure */
Global Const $tagSDL_JoyHatEvent ="ubyte type;ubyte which;ubyte hat;ubyte value"

;/* Joystick button event structure */
Global Const $tagSDL_JoyButtonEvent="ubyte type;ubyte which;ubyte button;ubyte state"

;/* The "window resized" event
;   When you get this event, you are responsible for setting a new video
;   mode with the new width and height.
; */
Global Const $tagSDL_ResizeEvent="ubyte type;int w;int h"

;/* The "screen redraw" event */
Global Const $tagSDL_ExposeEvent="ubyte type"

;/* The "quit requested" event */
Global Const $tagSDL_QuitEvent="ubyte type"

;/* A user-defined event type */
Global Const $tagSDL_UserEvent="ubyte type;int code;ptr data1;ptr data2"

;/* If you want to use this event, you should include SDL_syswm.h */
Global Const $tagSDL_SysWMEvent="ubyte type;ptr msg"

;/* General event structure */
;I failed to find out how this struct really is built so I made it insanely large just in case.
Global Const $tagSDL_Event="ubyte type;int;int;int;int;int;int;int;int;int"

;For convenience
Global Const $_SDL_EventToStruct[16][2]=[[$_SDL_NOEVENT, 0], _
	[$_SDL_ACTIVEEVENT, $tagSDL_ActiveEvent], _
	[$_SDL_KEYDOWN, $tagSDL_KeyboardEvent], _
	[$_SDL_KEYUP, $tagSDL_KeyboardEvent], _
	[$_SDL_MOUSEMOTION, $tagSDL_MouseMotionEvent], _
	[$_SDL_MOUSEBUTTONDOWN, $tagSDL_SDL_MouseButtonEvent], _
	[$_SDL_MOUSEBUTTONUP, $tagSDL_SDL_MouseButtonEvent], _
	[$_SDL_JOYAXISMOTION, $tagSDL_JoyAxisEvent], _
	[$_SDL_JOYBALLMOTION, $tagSDL_JoyBallEvent], _
	[$_SDL_JOYHATMOTION, $tagSDL_JoyHatEvent], _
	[$_SDL_JOYBUTTONDOWN, $tagSDL_JoyButtonEvent], _
	[$_SDL_JOYBUTTONUP, $tagSDL_JoyButtonEvent], _
	[$_SDL_QUIT, $tagSDL_QuitEvent], _
	[$_SDL_SYSWMEVENT, $tagSDL_SysWMEvent], _
	[$_SDL_VIDEORESIZE, $tagSDL_ResizeEvent], _
	[$_SDL_VIDEOEXPOSE, $tagSDL_ExposeEvent]]
#EndRegion

#Region Globals (Joystick)
Global Const $_SDL_HAT_CENTERED						= 0x00
Global Const $_SDL_HAT_UP							= 0x01
Global Const $_SDL_HAT_RIGHT						= 0x02
Global Const $_SDL_HAT_DOWN							= 0x04
Global Const $_SDL_HAT_LEFT							= 0x08
Global Const $_SDL_HAT_RIGHTUP						= BitOR($_SDL_HAT_RIGHT, $_SDL_HAT_UP)
Global Const $_SDL_HAT_RIGHTDOWN					= BitOR($_SDL_HAT_RIGHT, $_SDL_HAT_DOWN)
Global Const $_SDL_HAT_LEFTUP						= BitOR($_SDL_HAT_LEFT, $_SDL_HAT_UP)
Global Const $_SDL_HAT_LEFTDOWN						= BitOR($_SDL_HAT_LEFT, $_SDL_HAT_DOWN)
#EndRegion

#Region Globals (Audio)
Global Const $tagSDL_AudioSpec="int freq;ushort format;ubyte channels;ubyte silence;ushort samples;" & _
								"ushort padding;uint size;ptr callback;ptr userdata"

Global Const $_SDL_AUDIO_U8								= 0x0008 ;/* Unsigned 8-bit samples */
Global Const $_SDL_AUDIO_S8								= 0x8008 ;/* Signed 8-bit samples */
Global Const $_SDL_AUDIO_U16LSB							= 0x0010 ;/* Unsigned 16-bit samples */
Global Const $_SDL_AUDIO_S16LSB							= 0x8010 ;/* Signed 16-bit samples */
Global Const $_SDL_AUDIO_U16MSB							= 0x1010 ;/* As above, but big-endian byte order */
Global Const $_SDL_AUDIO_S16MSB							= 0x9010 ;/* As above, but big-endian byte order */
Global Const $_SDL_AUDIO_U16							= $_SDL_AUDIO_U16LSB
Global Const $_SDL_AUDIO_S16							= $_SDL_AUDIO_S16MSB

Global Const $_SDL_AUDIO_U16SYS							= $_SDL_AUDIO_U16LSB	;I cheated because AutoIt will always run on LSB
Global Const $_SDL_AUDIO_S16SYS							= $_SDL_AUDIO_S16LSB	;I cheated because AutoIt will always run on LSB

Global Enum $_SDL_AUDIO_STOPPED, $_SDL_AUDIO_PLAYING, $_SDL_AUDIO_PAUSED

Global Const $_SDL_MIX_MAXVOLUME						= 128
#EndRegion

#Region Globals (CDROM)
Global Const $_SDL_MAX_TRACKS						= 99

Global Const $_SDL_AUDIO_TRACK						= 0x00
Global Const $_SDL_DATA_TRACK						= 0x04

Global Enum $_SDL_CD_TRAYEMPTY, $_SDL_CD_STOPPED, $_SDL_CD_PLAYING, $_SDL_CD_PAUSED, $_SDL_CD_ERROR = -1

Global Const $_SDL_CD_FPS							= 75
#EndRegion

#Region Globals (Misc)
Global Const $_SDL_MAJOR_VERSION					= 1
Global Const $_SDL_MINOR_VERSION					= 2
Global Const $_SDL_PATCHLEVEL						= 8

Global Const $tagSDL_version="ubyte;ubyte;ubyte"
#EndRegion

#Region Globals (ffmpeg)
Global Enum $_SDL_ffmpegUninitialized, $_SDL_ffmpegOutputStream, $_SDL_ffmpegInputStream

;Probably faulty
Global Const $tagSDL_ffmpegStream="ptr ffmpeg;ptr decodeFrame;ptr encodeFrame;int encodeFrameBufferSize;" & _
								"ubyte encodeFrameBuffer;int encodeAudioInputSize;uint64 frameCount;byte sampleBuffer;" & _
								"int sampleBufferSize;int sampleBufferOffset;int64 sampleBufferTime;ptr buffer;" & _
								"ptr mutex;int id;int64 lastTimeStamp;ptr next"

Global Const $tagSDL_ffmpegFile="int type;ptr ffmpeg;ptr vs;ptr as;ptr streamMutex;uint videoStreams;" & _
								"uint audioStreams;ptr videoStream;ptr audioStream;int64 minimalTimestamp"

#cs
typedef struct SDL_ffmpegStream {

    /** Pointer to ffmpeg data, internal use only! */
    struct AVStream *_ffmpeg;
    /** Intermediate frame which will be used when decoding */
    struct AVFrame *decodeFrame;
    /** Intermediate frame which will be used when encoding */
    struct AVFrame *encodeFrame;

    int encodeFrameBufferSize;
    uint8_t *encodeFrameBuffer;

    int encodeAudioInputSize;
    uint64_t frameCount;

    /** buffer for decoded audio data */
    int8_t *sampleBuffer;
    /** amount of data in samplebuffer */
    int sampleBufferSize;
    /** position of data in samplebuffer */
    int sampleBufferOffset;
    /** timestamp which fits the data in samplebuffer */
    int64_t sampleBufferTime;

    /** packet buffer */
    SDL_ffmpegPacket *buffer;
    /** mutex for multi threaded acces to buffer */
    SDL_mutex *mutex;

    /** Id of the stream */
    int id;
    /** This holds the lastTimeStamp calculated, usefull when frames don't provide
        a usefull dts/pts, also used for determining at what point we are in the file */
    int64_t lastTimeStamp;

    /** pointer to the next stream, or NULL if current stream is the last one */
    struct SDL_ffmpegStream *next;

} SDL_ffmpegStream;
#ce
#EndRegion

#Region	RECT stuff
Func _SDL_Rect_Create($iX, $iY, $iWidth, $iHeight)
	Local $pRect = DllStructCreate("short;short;ushort;ushort")
	DllStructSetData($pRect, 1,$iX)
	DllStructSetData($pRect, 2,$iY)
	DllStructSetData($pRect, 3,$iWidth)
	DllStructSetData($pRect, 4,$iHeight)
	Return $pRect
EndFunc
#EndRegion

#Region Start and Shutdown commands (for other dlls)
#cs ;Don't touch. Seriously.
Func _SDL_Shutdown_ffmpeg()
	DllClose($__SDL_DLL_ffmpeg)
EndFunc

Func _SDL_Init_ffmpeg()
	$__SDL_DLL_ffmpeg = DllOpen("SDL_ffmpeg.dll")
EndFunc

Func _SDL_Shutdown_draw()
	DllClose($__SDL_DLL_draw)
EndFunc

Func _SDL_Init_draw($sDir = "")
	$__SDL_DLL_draw = DllOpen($sDir & "SDL_draw.dll")
EndFunc
#ce
#EndRegion

#Region Initialization

Func _SDL_Init($iFlags, $sDir = "SDL.dll")
	$__SDL_DLL = DllOpen($sDir)
	If $__SDL_DLL = -1 Then
		Return SetError(1)
	EndIf

;~ 	Local $__SDL_Version = _SDL_Linked_Version()
	;~ 	ConsoleWrite("Loaded SDL version: " & $__SDL_Version[0] & "." & $__SDL_Version[1] & "." & $__SDL_Version[2] & ".*" & @CRLF)
;~ 	If $__SDL_Version[0] <> $_SDL_MAJOR_VERSION Or $__SDL_Version[1] <> $_SDL_MINOR_VERSION Or $__SDL_Version[2] <> $_SDL_PATCHLEVEL Then Exit 0*ConsoleWrite("This script was built for SDL 1.2.13.* Using a dll of another version is not supported" & @CRLF)

	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_Init", "uint", $iFlags)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_InitSubSystem($iFlags)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_InitSubSystem", "uint", $iFlags)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
Func _SDL_QuitSubSystem($iFlags)
	Local $iTemp = DllCall($__SDL_DLL, "none:cdecl", "SDL_QuitSubSystem", "uint", $iFlags)
	Return $iTemp
EndFunc

Func _SDL_Quit()
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_Quit")
	DllClose($__SDL_DLL)
	Return $iTemp
EndFunc

;Untested
;Description: "flags is a bitwise OR'd combination of the subsystems you wish to check"
;Return Value: "SDL_WasInit returns a bitwised OR'd combination of the initialized subsystems"
;Notes: Can use $_SDL_INIT_EVERYTHING to get every susbystem
Func _SDL_WasInit($iFlags)
	Local $iTemp = DllCall($__SDL_DLL, "uint:cdecl", "SDL_WasInit", "uint", $iFlags)
	Return $iTemp[0]
EndFunc

Func _SDL_SetError($sStr)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_SetError", "str", $sStr)
EndFunc

;Return Value: "SDL_GetError returns a string containing the last error."
Func _SDL_GetError()
	Local $sTemp = DllCall($__SDL_DLL, "str:cdecl", "SDL_GetError")
	Return $sTemp[0]
EndFunc

Func _SDL_ClearError()
	DllCall($__SDL_DLL, "none:cdecl", "SDL_ClearError")
EndFunc

;Used internally by SDL.dll, but implemented here for you to play with ;)
Func _SDL_OutOfMemory()
	_SDL_Error($_SDL_ENOMEM)
EndFunc

;Used internally by SDL.dll, but implemented here for you to play with ;)
Func _SDL_Unsupported()
	_SDL_Error($_SDL_UNSUPPORTED)
EndFunc

;Used internally by SDL.dll, but implemented here for you to play with ;)
Func _SDL_Error($iCode)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_Error", "int", $iCode)
EndFunc
#EndRegion

#Region Video
;Untested
Func _SDL_GetVideoSurface()
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_GetVideoSurface")
	Return $pTemp[0]
EndFunc

;It returns an array containing the data.
Func _SDL_GetVideoInfo()
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_GetVideoInfo")
	Local $pStruct = DllStructCreate($tagSDL_VideoInfo, $pTemp[0])

	Local $aTemp[5]
	For $X = 1 To 5
		$aTemp[$X-1] = DllStructGetData($pStruct, $X)
	Next

	Return $aTemp
EndFunc

Func _SDL_VideoDriverName()
	Local $sNamebuf, $iMaxlen = 10	;This will actually never be more than 7, but who cares?
	Local $sTemp = DllCall($__SDL_DLL, "str:cdecl", "SDL_VideoDriverName", "str", $sNamebuf, "int", $iMaxlen)
	$sNamebuf = $sTemp[1]

	Return $sNamebuf
EndFunc

Func _SDL_ListModes($pFormat, $iFlags)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_ListModes", "ptr", $pFormat, "uint", $iFlags)
	Return $pTemp[0]
EndFunc

Func _SDL_VideoModeOK($iWidth, $iHeight, $iBpp, $iFlags)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_VideoModeOK", "int", $iWidth, "int", $iHeight, "int", $iBpp, "uint", $iFlags)
	Return $iTemp[0]
EndFunc

Func _SDL_SetVideoMode($iWidth, $iHeight, $iBpp, $iFlags)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_SetVideoMode", "int", $iWidth, "int", $iHeight, "int", $iBpp, "uint", $iFlags)
	_SDL_WM_SetCaption(@ScriptName, @ScriptName)
	Return $pTemp[0]
EndFunc

;For convenience
Func _SDL_GuiCreate($sTitle, $iWidth, $iHeight, $iBpp, $iFlags)
	Local $pVideo = _SDL_SetVideoMode($iWidth, $iHeight, $iBpp, $iFlags)
	_SDL_WM_SetCaption($sTitle, $sTitle)
	Return $pVideo
EndFunc

Func _SDL_UpdateRect($iScreen, $iX, $iY, $iW, $iH)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_UpdateRect", "int", $iScreen, "int", $iX, "int", $iY, "int", $iW, "int", $iH)
EndFunc

Func _SDL_Flip($iScreen)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_Flip", "int", $iScreen)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Doesn't work
Func _SDL_GetGammaRamp(ByRef $iRedtable, ByRef $iGreentable, ByRef $iBluetable)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_GetGammaRamp", "ushort", $iRedtable, "ushort", $iGreentable, "ushort", $iBluetable)
	$iRedtable = $iTemp[1]
	$iGreentable = $iTemp[2]
	$iBluetable = $iTemp[3]
	Return $iTemp[0] + 1
EndFunc

Func _SDL_MapRGB($pSurface, $iR, $iG, $iB)
	Local $pStruct1 = DllStructCreate($tagSDL_SURFACE, $pSurface)
	Local $pStruct2 = DllStructGetData($pStruct1, "format")

	Local $iTemp = DllCall($__SDL_DLL, "uint:cdecl", "SDL_MapRGB", "ptr", $pStruct2, "ubyte", $iR, "ubyte", $iG, "ubyte", $iB)
	Return $iTemp[0]
EndFunc

Func _SDL_MapRGBA($pSurface, $iR, $iG, $iB, $iA)
	Local $pStruct1 = DllStructCreate($tagSDL_SURFACE, $pSurface)
	Local $pStruct2 = DllStructGetData($pStruct1, "format")

	Local $iTemp = DllCall($__SDL_DLL, "uint:cdecl", "SDL_MapRGBA", "ptr", $pStruct2, "ubyte", $iR, "ubyte", $iG, "ubyte", $iB, "ubyte", $iA)
	Return $iTemp[0]
EndFunc

Func _SDL_CreateRGBSurface($iFlags, $iWidth, $iHeight, $iDepth, $iRmask, $iGmask, $iBmask, $iAmask)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_CreateRGBSurface", "uint", $iFlags, "int", $iWidth, "int", $iHeight, "int", $iDepth, "uint", $iRmask, "uint", $iGmask, "uint", $iBmask, "uint", $iAmask)
	Return $pTemp[0]
EndFunc

Func _SDL_FreeSurface($pSurface)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_FreeSurface", "ptr", $pSurface)
EndFunc

;Untested
Func _SDL_LockSurface($pSurface)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_LockSurface", "ptr", $pSurface)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
Func _SDL_UnlockSurface($pSurface)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_UnlockSurface", "ptr", $pSurface)
EndFunc

Func _SDL_LoadBMP($sFile)
	Return _SDL_LoadBMP_RW(_SDL_RWFromFile($sFile, "rb"), 1)
EndFunc

Func _SDL_LoadBMP_RW($pSrc, $iFreesrc)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_LoadBMP_RW", "ptr", $pSrc, "int", $iFreesrc)
	Return $pTemp[0]
EndFunc

Func _SDL_SaveBMP($pSurface, $sFile)
	Return _SDL_SaveBMP_RW($pSurface, _SDL_RWFromFile($sFile, "wb"), 1)
EndFunc

Func _SDL_SaveBMP_RW($pSurface, $pDst, $iFreeDst)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_SaveBMP_RW", "ptr", $pSurface, "ptr", $pDst, "int", $iFreeDst)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_SetColorKey($pSurface, $iFlag, $iKey)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_SetColorKey", "ptr", $pSurface, "uint", $iFlag, "uint", $ikey)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_SetAlpha($pSurface, $iFlag, $iAlpha)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_SetAlpha", "ptr", $pSurface, "uint", $iFlag, "ubyte", $iAlpha)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_SetClipRect($pSurface, $pRect)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_SetClipRect", "ptr", $pSurface, "ptr", DllStructGetPtr($pRect))
EndFunc

Func _SDL_BlitSurface($pSrcSurface, $pSrcRect, $pDstSurface, $pDstRect)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_UpperBlit", "ptr", $pSrcSurface, "ptr", DllStructGetPtr($pSrcRect), "ptr", $pDstSurface, "ptr", DllStructGetPtr($pDstRect))
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_FillRect($pSurface, $pRect, $iColor)
	Local $pStruct2 = DllStructGetPtr($pRect)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_FillRect", "int", $pSurface, "int", $pStruct2, "uint", $iColor)
	Return $iTemp[0]
EndFunc

Func _SDL_DisplayFormat($pSurface)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_DisplayFormat", "ptr", $pSurface)
	Return $pTemp[0]
EndFunc

;Untested
Func _SDL_DisplayFormatAlpha($pSurface)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_DisplayFormatAlpha", "ptr", $pSurface)
	Return $pTemp[0]
EndFunc

Func _SDL_WarpMouse($iX, $iY)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_WarpMouse", "ushort", $iX, "ushort", $iY)
EndFunc

Func _SDL_ShowCursor($iToggle)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_ShowCursor", "int", $iToggle)
	Return $iTemp[0]
EndFunc

;This is only an experiment.
;Please use some other "draw a pixel func" instead.
;Only tested with bpp = 4
;Based on example in helpfile
;Needs to be recoded
;Created by AdmiralAlkex
Func _SDL_GetPixel($pSurface, $iX, $iY)
	Local $Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
	;~ 	ConsoleWrite(DllStructGetSize($Struct) & @CRLF)
	Local $PtrStruct = DllStructGetData($Struct, "format")
	;~ 	ConsoleWrite(@error & "/" & VarGetType($PtrStruct) & "/" & $PtrStruct & "/" & @CRLF)

	Local $Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
	;~ 	ConsoleWrite(DllStructGetSize($Struct2) & @CRLF)

	Local $Bpp = DllStructGetData($Struct2, "BytesPerPixel")
	;~ 	ConsoleWrite(@error & "/" & VarGetType($Bpp) & "/" & $Bpp & "/" & @CRLF)

	Local $Pixels = DllStructGetData($Struct, "pixels")
	;~ 	ConsoleWrite(@error & "/" & VarGetType($Pixels) & "/" & $Pixels & "/" & @CRLF)
	Local $Pitch = DllStructGetData($Struct, "pitch")
	;~ 	ConsoleWrite(@error & "/" & VarGetType($Pitch) & "/" & $Pitch & "/" & @CRLF)

	Local $P = $Pixels + $iY * $Pitch + $iX * $Bpp
	;~ 	ConsoleWrite(VarGetType($P) & "/" & $P & "/" & @CRLF)

	Switch $Bpp
		Case 1
			Return $P
		Case 2
			Return $P
		Case 3					;I'm not sure how to convert this one. Should really be: return p[0] | p[1] << 8 | p[2] << 16;
;~ 			Local $P2
;~ 			$P2 BitShift($P, -

;~ 			Return $P2

			Return 0
		Case 4
			Return $P
		Case Else		;Shouldn't happen
			Return 0
	EndSwitch
EndFunc

;This is only an experiment.
;Please use some other "draw a pixel func" instead.
;Only tested with bpp = 4 but should work on all.
;Based on example in helpfile
;Created by AdmiralAlkex
Func _SDL_PutPixel($pSurface, $iX, $iY, $iPixel)
	Local $Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
	Local $PtrStruct = DllStructGetData($Struct, "format")

	Local $Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
	Local $Bpp = DllStructGetData($Struct2, "BytesPerPixel")

	Local $Pixels = DllStructGetData($Struct, "pixels")
	Local $Pitch = DllStructGetData($Struct, "pitch")

	Local $P = $Pixels + $iY * $Pitch + $iX * $Bpp

	Switch $Bpp
		Case 1
			$Struct = DllStructCreate("byte", $P)
			DllStructSetData($Struct, 1, $iPixel)
			$Struct = 0
			Return $P
		Case 2
			$Struct = DllStructCreate("short", $P)
			DllStructSetData($Struct, 1, $iPixel)
			$Struct = 0
			Return $P
		Case 3
			$Struct = DllStructCreate("byte;byte;byte", $P)
			DllStructSetData($Struct, 1, BitAnd($iPixel,0xff))
			DllStructSetData($Struct, 2, BitAnd(BitShift($iPixel, 8),0xff))
			DllStructSetData($Struct, 3, BitAnd(BitShift($iPixel, 16), 0xff))
			$Struct = 0
			Return $P
		Case 4
			$Struct = DllStructCreate("uint", $P)
			DllStructSetData($Struct, 1, $iPixel)
			$Struct = 0
			Return $P
		Case Else		;Shouldn't happen
			Return 0
	EndSwitch
EndFunc

;This is only an experiment.
;Please use some other "draw a square func" instead.
;Only tested with bpp = 4 but should work on all.
;Created by AdmiralAlkex
Func _SDL_PutSquare($pSurface, $iX, $iY, $iWidth, $iHeight, $iPixel)
	For $X = 1 To $iWidth
		For $Y = 1 To $iHeight
			_SDL_PutPixel($pSurface, $iX+$X-1, $iY+$Y-1, $iPixel)
		Next
	Next
EndFunc

;This is only an experiment. But it's fast like hell compared to _SDL_PutSquare().
;Please use some other "draw a square func" instead.
;Only tested with bpp = 4 but should work on all.
;Created by AdmiralAlkex
Func _SDL_PutSquareOptimized($pSurface, $iX, $iY, $iWidth, $iHeight, $iPixel)
	Local $Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
	Local $PtrStruct = DllStructGetData($Struct, "format")
	Local $Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
	Local $Bpp = DllStructGetData($Struct2, "BytesPerPixel")

	Local $String, $Type
	Switch $Bpp
		Case 1							;Untested
			$Type = "byte;"
		Case 2							;Untested
			$Type = "short;"
		Case 3							;Untested
			For $X = 1 To $iWidth
				For $Y = 1 To $iHeight
					$Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
					$PtrStruct = DllStructGetData($Struct, "format")
					$Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
					$Bpp = DllStructGetData($Struct2, "BytesPerPixel")
					Local $Pixels = DllStructGetData($Struct, "pixels")
					Local $Pitch = DllStructGetData($Struct, "pitch")
					Local $P = $Pixels + ($iY+$Y-1) * $Pitch + ($iX+$X-1) * $Bpp

					$Struct = DllStructCreate("byte;byte;byte", $P)
					DllStructSetData($Struct, 1, BitAnd($iPixel,0xff))
					DllStructSetData($Struct, 2, BitAnd(BitShift($iPixel, 8),0xff))
					DllStructSetData($Struct, 3, BitAnd(BitShift($iPixel, 16), 0xff))
					$Struct = 0
				Next
			Next
			Return 0
		Case 4
			$Type = "uint;"
		Case Else		;Shouldn't happen
			Return SetError(1)
	EndSwitch

	For $X2 = 1 To $iWidth
		$String &= $Type
	Next

	For $Y = 1 To $iHeight
		$Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
		$PtrStruct = DllStructGetData($Struct, "format")
		$Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
		$Bpp = DllStructGetData($Struct2, "BytesPerPixel")
		$Pixels = DllStructGetData($Struct, "pixels")
		$Pitch = DllStructGetData($Struct, "pitch")
		$P = $Pixels + ($iY+$Y-1) * $Pitch + $iX * $Bpp

		$Struct = DllStructCreate($String, $P)
		For $Z = 1 To $iWidth
			DllStructSetData($Struct, $Z, $iPixel)
		Next
		$Struct = 0
	Next
EndFunc

;This is only an experiment.
;Please use some other "draw a square func" instead.
;Not finished
;Created by AdmiralAlkex
Func _SDL_PutBackOptimized($pSurface, $iPixel)

	Local $Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
	Local $PtrStruct = DllStructGetData($Struct, "format")
	Local $Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
	Local $Bpp = DllStructGetData($Struct2, "BytesPerPixel")

	Switch $Bpp
		Case 1							;Untested
			;~ 	ConsoleWrite("1" & @CRLF)

			$Struct = DllStructCreate($tagSDL_SURFACE, $pSurface)
			$PtrStruct = DllStructGetData($Struct, "format")
			$Struct2 = DllStructCreate($tagSDL_PixelFormat, $PtrStruct)
			$Bpp = DllStructGetData($Struct2, "BytesPerPixel")
			Local $Pixels = DllStructGetData($Struct, "pixels")
			Local $Pitch = DllStructGetData($Struct, "pitch")
			Local $P = $Pixels + 1 * $Pitch + 1 * $Bpp
			;~ 	ConsoleWrite("1.1" & @CRLF)

			Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_memset", "ptr", $P, "int", $iPixel, "str", 1)
			;~ 	ConsoleWrite("1.2" & "/" & @error & "/" & VarGetType($pTemp) & "/" & $pTemp & "/" & @CRLF)
			Return $pTemp[0]
		Case 2							;Untested
			;~ 	ConsoleWrite("2" & @CRLF)
		Case 3							;Untested
			;~ 	ConsoleWrite("3" & @CRLF)
		Case 4
			;~ 	ConsoleWrite("4" & @CRLF)
		Case Else		;Shouldn't happen
			Return SetError(1)
	EndSwitch
EndFunc

#cs
;What should I do with OpenGL? No point in implementing more than these four I think. Maybe for next version.
extern DECLSPEC int SDLCALL SDL_GL_LoadLibrary(const char *path);
extern DECLSPEC int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);
extern DECLSPEC int SDLCALL SDL_GL_GetAttribute(SDL_GLattr attr, int* value);
extern DECLSPEC void SDLCALL SDL_GL_SwapBuffers(void);
#ce
#EndRegion

#Region Window Manager
;Written by ProgAndy
Func _SDL_WM_GetCaption()
    Local $aResult = DllCall($__SDL_DLL, "none:cdecl", "SDL_WM_GetCaption", "str*", 0, "str*", 0)
    Local $aTemp[2]=[($aResult[1]), ($aResult[2])]
    Return $aTemp
EndFunc

;Anyone know where the second parameter shows up on Windows?
Func _SDL_WM_SetCaption($sTitle, $sIcon)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_WM_SetCaption", "str", $sTitle, "str", $sIcon)
EndFunc

Func _SDL_WM_SetIcon($pSurface, $iMask)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_WM_SetIcon", "ptr", $pSurface, "ubyte", $iMask)
EndFunc

Func _SDL_WM_IconifyWindow()
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_WM_IconifyWindow")
	Return $iTemp[0]
EndFunc

;Works, bit is one of the most worthless function ever written in AutoIt.
Func _SDL_WM_ToggleFullScreen($pSurface)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_WM_ToggleFullScreen", "ptr", $pSurface)
	Return $iTemp[0]
EndFunc

;Works great
Func _SDL_WM_GrabInput($pMode)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_WM_GrabInput", "ptr", $pMode)
	Return $iTemp[0]
EndFunc
#EndRegion

#Region Event Handling
;Untested
Func _SDL_PumpEvents()
	DllCall($__SDL_DLL, "none:cdecl", "SDL_PumpEvents")
EndFunc

;Untested or Doesn't work
Func _SDL_PeepEvents($pEvents, $iNumEvents, $iAction, $iMask)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_PeepEvents", "ptr", $pEvents, "int", $iNumEvents, "int", $iAction, "uint", $iMask)
	Return $iTemp[0]
EndFunc

;Works, but doesn't have any parameters like the original
Func _SDL_PollEvent()
	Local $A1 = DllStructCreate($tagSDL_Event)
	DllStructSetData($A1, 1, 1)

	Local $A2 = DllCall($__SDL_DLL, "int:cdecl", "SDL_PollEvent", "ptr", DllStructGetPtr($A1))
	If $A2[0] <> 0 Then
		Return $A2[0]
	Else
		Return 0
	EndIf
EndFunc

;Like _SDL_PollEvent but easy(ish)
Func _SDL_PollEventEasy()
	Local $A1 = DllStructCreate($tagSDL_Event)
	DllStructSetData($A1, 1, 1)

	Local $A2 = DllCall($__SDL_DLL, "int:cdecl", "SDL_PollEvent", "ptr", DllStructGetPtr($A1))
	If $A2[0] = 0 Then Return 0

	Local $A3 = DllStructGetData($A1, "type")
	For $X = 1 To UBound($_SDL_EventToStruct) -1
		If $A3 = $_SDL_EventToStruct[$X][0] Then
			Local $B1 = DllStructCreate($_SDL_EventToStruct[$X][1], DllStructGetPtr($A1))
			Return _LumpDump($B1, $_SDL_EventToStruct[$X][1])
		EndIf
	Next
EndFunc

;Guess.
Func _SDL_WaitEvent()
EndFunc

;Untested
Func _SDL_GetEventFilter()
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_GetEventFilter")
	Return $pTemp[0]
EndFunc

;Untested
Func _SDL_EventState($iType, $iState)
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_EventState", "ubyte", $iType, "int", $iState)
	Return $iTemp[0]
EndFunc

;Untested
Func _SDL_EnableUNICODE($iEnable)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_EnableUNICODE", "int", $iEnable)
	Return $iTemp[0]
EndFunc

;Untested
Func _SDL_EnableKeyRepeat($iDelay, $iInterval)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_EnableKeyRepeat", "int", $iDelay, "int", $iInterval)
	Return $iTemp[0]
EndFunc

Func _SDL_GetMouseState(ByRef $iX, ByRef $iY)
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_GetMouseState", "int*", $iX, "int*", $iY)
	$iX = $iTemp[1]
	$iY = $iTemp[2]
	Return $iTemp[0]
EndFunc

Func _SDL_GetRelativeMouseState(ByRef $iX, ByRef $iY)
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_GetRelativeMouseState", "int*", $iX, "int*", $iY)
	$iX = $iTemp[1]
	$iY = $iTemp[2]
	Return $iTemp[0]
EndFunc

Func _SDL_GetAppState()
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_GetAppState")
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickEventState($iState)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickEventState", "int", $iState)
	Return $iTemp[0]
EndFunc
#EndRegion

#Region Joystick
Func _SDL_NumJoysticks()
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_NumJoysticks")
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickName($iIndex)
	Local $iTemp = DllCall($__SDL_DLL, "str:cdecl", "SDL_JoystickName", "int", $iIndex)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickOpen($iIndex)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_JoystickOpen", "int", $iIndex)
	Return $pTemp[0]
EndFunc

Func _SDL_JoystickOpened($iIndex)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickOpened", "int", $iIndex)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickIndex($pJOYSTICK)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickIndex", "int", $pJOYSTICK)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickNumAxes($pJOYSTICK)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickNumAxes", "int", $pJOYSTICK)
	Return $iTemp[0]
EndFunc

;Seem to work, but since no one have any balls it hard to know.
Func _SDL_JoystickNumBalls($pJOYSTICK)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickNumBalls", "int", $pJOYSTICK)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickNumHats($pJOYSTICK)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickNumHats", "int", $pJOYSTICK)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickNumButtons($pJOYSTICK)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickNumButtons", "int", $pJOYSTICK)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickUpdate($pJOYSTICK)
	if $pJOYSTICK = "False" Then Return
	DllCall($__SDL_DLL, "none:cdecl", "SDL_JoystickUpdate")
EndFunc

Func _SDL_JoystickGetAxis($pJOYSTICK, $iAxis)
	Local $iTemp = DllCall($__SDL_DLL, "short:cdecl", "SDL_JoystickGetAxis", "int", $pJOYSTICK, "int", $iAxis)
;~ 	;~ 	ConsoleWrite(VarGetType($iTemp[0]) & @CRLF)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickGetHat($pJOYSTICK, $iHat)
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_JoystickGetHat", "int", $pJOYSTICK, "int", $iHat)
	Return $iTemp[0]
EndFunc

Func _SDL_JoystickGetButton($pJOYSTICK, $iButton)
	Local $iTemp = DllCall($__SDL_DLL, "ubyte:cdecl", "SDL_JoystickGetButton", "int", $pJOYSTICK, "int", $iButton)
	Return $iTemp[0]
EndFunc

;Probably works, but since no one have any balls it hard to know.
Func _SDL_JoystickGetBall($pJOYSTICK, $iBall, ByRef $iDx, ByRef $iDy)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_JoystickGetBall", "int", $pJOYSTICK, "int", $iBall, "int*", $iDx, "Int*", $iDy)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_JoystickClose($pJOYSTICK)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_JoystickClose", "int", $pJOYSTICK)
EndFunc
#EndRegion

#Region Audio
#cs
This doesn't work and is unfinished. Please use the SDL_mixer library instead, it's better.
#ce

;Untested
Func _SDL_AudioInit($sDriver_Name)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_AudioInit", "str", $sDriver_Name)
	Return $iTemp[0]
EndFunc

;Untested
Func _SDL_AudioQuit()
	DllCall($__SDL_DLL, "none:cdecl", "SDL_AudioQuit")
EndFunc

Func _SDL_AudioDriverName()
	Local $sNamebuf, $iMaxlen = 10
	Local $sTemp = DllCall($__SDL_DLL, "str:cdecl", "SDL_AudioDriverName", "str", $sNamebuf, "int", $iMaxlen)

	Local $sNamebuf = $sTemp[1]
	Return $sNamebuf
EndFunc

Func _SDL_OpenAudio($pDesired)
	Local $0 = 0
	Local $iTemp = _SDL_OpenAudioEx($pDesired, $0)
	Return $iTemp
EndFunc

Func _SDL_PauseAudio($iPause_On)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_PauseAudio", "int", $iPause_On)
EndFunc

;Doesn't work as intended
Func _SDL_OpenAudioEx($pDesired, ByRef $pObtained)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_OpenAudio", "ptr", DllStructGetPtr($pDesired), "ptr", DllStructGetPtr($pObtained))
	$pObtained = $iTemp[2]
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
Func _SDL_MixAudio($pDst, $pSrc, $iLen, $iVolume)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_MixAudio", "ptr", $pDst, "ptr", $pSrc, "uint", $iLen, "int", $iVolume)
EndFunc

;Untested
Func _SDL_CloseAudio()
	DllCall($__SDL_DLL, "none:cdecl", "SDL_CloseAudio")
EndFunc

#cs
extern DECLSPEC int SDLCALL SDL_AudioInit(const char *driver_name);
extern DECLSPEC void SDLCALL SDL_AudioQuit(void);
extern DECLSPEC char * SDLCALL SDL_AudioDriverName(char *namebuf, int maxlen);
extern DECLSPEC int SDLCALL SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained);
extern DECLSPEC void SDLCALL SDL_PauseAudio(int pause_on);

extern DECLSPEC void SDLCALL SDL_MixAudio(Uint8 *dst, const Uint8 *src, Uint32 len, int volume);
extern DECLSPEC void SDLCALL SDL_CloseAudio(void);
#ce
#EndRegion

#Region CDROM
;A macro I found, can be useful
;Return Value: Returns 1 if CD in drive
Func _SDL_CD_INDRIVE($pCDROM)
	If Hex(_SDL_CDStatus($pCDROM)) > 0 Then Return 1
EndFunc

;Another macro
Func _SDL_FRAMES_TO_MSF($iFrames, ByRef $iM, ByRef $iS, ByRef $iF)
	 $iM = Floor($iFrames/$_SDL_CD_FPS/60)
	 $iFrames -= $iM*$_SDL_CD_FPS*60

	 $iS = Floor($iFrames/$_SDL_CD_FPS)
	 $iFrames -= $iS*$_SDL_CD_FPS

	 $iF = $iFrames
EndFunc

;Another macro
Func _SDL_MSF_TO_FRAMES($iM, $iS, $iF)
	Return $iM*60*$_SDL_CD_FPS+$iS*$_SDL_CD_FPS+$iF
EndFunc

Func _SDL_CDNumDrives()
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDNumDrives")
	Return $iTemp[0]
EndFunc

Func _SDL_CDName($iDrive)
	Local $sTemp = DllCall($__SDL_DLL, "str*:cdecl", "SDL_CDName", "int", $iDrive)
	Return $sTemp[0]
EndFunc

;Thx to monoceres for helping with this
Func _SDL_CDOpen($iDrive)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_CDOpen", "int", $iDrive)
	Return $pTemp[0]
EndFunc

Func _SDL_CDStatus($pCDROM)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_CDStatus", "int", $pCDROM)
	Return $pTemp[0]
EndFunc

Func _SDL_CDPlay($pCDROM, $iStart, $iLength)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDPlay", "int", $pCDROM, "int", $iStart, "int", $iLength)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;OBS!!! ZERO BASED!!!
Func _SDL_CDPlayTracks($pCDROM, $iStart_Track, $iStart_Frame, $iNtracks, $iNframes)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDPlayTracks", "int", $pCDROM, "int", $iStart_Track, "int", $iStart_Frame, "int", $iNtracks, "int", $iNframes)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_CDPause($pCDROM)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDPause", "int", $pCDROM)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_CDResume($pCDROM)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDResume", "int", $pCDROM)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_CDStop($pCDROM)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDStop", "int", $pCDROM)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_CDEject($pCDROM)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_CDEject", "int", $pCDROM)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

Func _SDL_CDClose($pCDROM)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_CDClose", "int", $pCDROM)
EndFunc
#EndRegion

#Region Threads
;Doesn't work
Func _SDL_CreateThread($pFunction, $pData)
	Local $Callback = DllCallbackRegister($pFunction, "none", "ptr")
;~ 	;~ 	ConsoleWrite(@error & "/" & VarGetType($Callback) & "/" & $Callback & "/" & @CRLF)

	Local $Pointer = DllCallbackGetPtr($Callback)
;~ 	;~ 	ConsoleWrite(@error & "/" & VarGetType($Pointer) & "/" & $Pointer & "/" & @CRLF)

	Local $pTemp = DllCall($__SDL_DLL, "ptr*:cdecl", "SDL_ThreadID", "ptr", $Pointer, "ptr", $pData)
;~ 	;~ 	ConsoleWrite(@error & "/" & VarGetType($pTemp) & "/" & $pTemp & "/" & @CRLF)
	Return $pTemp[0]
EndFunc

Func _SDL_ThreadID()
	Local $iTemp = DllCall($__SDL_DLL, "uint*:cdecl", "SDL_ThreadID")
	Return $iTemp[0]
EndFunc

;Untested
;/* Create a mutex, initialized unlocked */
Func _SDL_CreateMutex()
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_CreateMutex")
	Return $pTemp[0]
EndFunc

;Untested
Func _SDL_LockMutex($pMutex)
	Local $iTemp = _SDL_mutexP($pMutex)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
;/* Lock the mutex  (Returns 0, or -1 on error) */
Func _SDL_mutexP($pMutex)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_mutexP", "ptr", $pMutex)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
Func _SDL_UnlockMutex($pMutex)
	Local $iTemp = _SDL_mutexV($pMutex)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
;/* Unlock the mutex  (Returns 0, or -1 on error) */
Func _SDL_mutexV($pMutex)
	Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_mutexV", "ptr", $pMutex)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

;Untested
;/* Destroy a mutex */
Func _SDL_DestroyMutex($pMutex)
	DllCall($__SDL_DLL, "none:cdecl", "SDL_DestroyMutex", "ptr", $pMutex)
EndFunc

#cs
SDL_Thread *SDL_CreateThread(int (*fn)(void *), void *data);
extern DECLSPEC SDL_mutex * SDLCALL SDL_CreateMutex(void);

/* Lock the mutex  (Returns 0, or -1 on error) */
#define SDL_LockMutex(m)	SDL_mutexP(m)
extern DECLSPEC int SDLCALL SDL_mutexP(SDL_mutex *mutex);

/* Unlock the mutex  (Returns 0, or -1 on error)
   It is an error to unlock a mutex that has not been locked by
   the current thread, and doing so results in undefined behavior.
 */
#define SDL_UnlockMutex(m)	SDL_mutexV(m)
extern DECLSPEC int SDLCALL SDL_mutexV(SDL_mutex *mutex);

/* Destroy a mutex */
extern DECLSPEC void SDLCALL SDL_DestroyMutex(SDL_mutex *mutex);
#ce
#EndRegion

#Region Timers
Func _SDL_GetTicks()
	Local $iTemp = DllCall($__SDL_DLL, "uint*:cdecl", "SDL_GetTicks")
	Return $iTemp[0]
EndFunc

;Thx to monoceres for helping with this
Func _SDL_Delay($iMS)
	DllCall($__SDL_DLL, "ptr*:cdecl", "SDL_Delay", "uint", $iMS)
EndFunc

;Obsolete (kinda). _SDL_AddTimer() and _SDL_RemoveTimer() does the same, but with multiple timers.
Func _SDL_SetTimer($iInterval, $sFunction)
	If $iInterval = 0 And $sFunction = 0 Then
		Local $iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_SetTimer", "uint", 0, "ptr", 0)
		Return SetError($iTemp[0], 0, ($iTemp[0]=0))
	Else
		Local $hCallback = DLLCallbackRegister($sFunction, "uint:cdecl", "uint")
		$iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_SetTimer", "uint", $iInterval, "ptr", DllCallbackGetPtr($hCallback))
		Return SetError($iTemp[0], 0, ($iTemp[0]=0))
	EndIf
EndFunc

;Thx to monoceres for helping with this
Func _SDL_AddTimer($iInterval, $sFunction, $iParam)
	Local $hCallback = DLLCallbackRegister($sFunction, "uint:cdecl", "uint;ptr")
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_AddTimer", "uint", $iInterval, "ptr", DllCallbackGetPtr($hCallback), "int", $iParam)
	Return $iTemp[0]
EndFunc

Func _SDL_RemoveTimer($iTimerID)
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_RemoveTimer", "int", $iTimerID)
	Return $iTemp[0]
EndFunc
#EndRegion

#Region Misc

#Region CpuInfo
;/* This function returns true if the CPU has the RDTSC features */
Func _SDL_HasRDTSC()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasRDTSC")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the MMX features */
Func _SDL_HasMMX()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasMMX")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the MMX Ext. features */
Func _SDL_HasMMXExt()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasMMXExt")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the 3DNow features */
Func _SDL_Has3DNow()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_Has3DNow")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the 3DNow Ext. features */
Func _SDL_Has3DNowExt()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_Has3DNowExt")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the SSE features */
Func _SDL_HasSSE()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasSSE")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the SSE2 features */
Func _SDL_HasSSE2()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasSSE2")
	Return $iTemp[0]
EndFunc

;/* This function returns true if the CPU has the AltiVec features */
Func _SDL_HasAltiVec()
	Local $iTemp = DllCall($__SDL_DLL, "int*:cdecl", "SDL_HasAltiVec")
	Return $iTemp[0]
EndFunc
#EndRegion

#Region RWops
Func _SDL_RWFromFile($sFile, $sMode)
	Local $pTemp = DllCall($__SDL_DLL, "ptr:cdecl", "SDL_RWFromFile", "str", $sFile, "str", $sMode)
	Return $pTemp[0]
EndFunc

#cs
extern DECLSPEC SDL_RWops * SDLCALL SDL_RWFromFile(const char *file, const char *mode);
#ce
#EndRegion

#Region stdinc
#cs
Use AutoIt's own EnvSet() and EnvGet() instead.
#ce

;~ Func _SDL_getenv($sName)
;~ 	$sTemp = DllCall($__SDL_DLL, "str:cdecl", "SDL_getenv", "str", $sName)
;~ 	Return $sTemp[0]
;~ EndFunc

;~ Func _SDL_putenv($sVariable)
;~ 	$iTemp = DllCall($__SDL_DLL, "int:cdecl", "SDL_putenv", "str", $sVariable)
;~ 	Return $iTemp[0]
;~ EndFunc

#cs
extern DECLSPEC char * SDLCALL SDL_getenv(const char *name);
extern DECLSPEC int SDLCALL SDL_putenv(const char *variable);
#ce
#EndRegion

#Region Version
Func _SDL_Linked_Version()
	Local $pTemp = DllCall($__SDL_DLL, "ptr*:cdecl", "SDL_Linked_Version")
	Local $Struct = DllStructCreate("ubyte;ubyte;ubyte", $pTemp[0])
	Local $aTemp[3]=[DllStructGetData($Struct, 1), DllStructGetData($Struct, 2), DllStructGetData($Struct, 3)]
	Return $aTemp
EndFunc

#cs
const SDL_version * SDLCALL SDL_Linked_Version(void);
#ce
#EndRegion

#EndRegion

#Region Libraries

#Region SDL_ffmpeg.dll
;/* SDL_ffmpegFile create / destroy */
Func _SDL_ffmpegOpen($sFilename)
	Local $pTemp = DllCall($__SDL_DLL_ffmpeg, "ptr:cdecl", "SDL_ffmpegOpen", "str", $sFilename)
	_CW($pTemp)
	Return $pTemp[0]
EndFunc

Func _SDL_ffmpegFree($pFile)
	DllCall($__SDL_DLL_ffmpeg, "none:cdecl", "SDL_ffmpegFree", "ptr", $pFile)
EndFunc

;Should work
Func _SDL_ffmpegGetAudioStream($pFile, $iAudioID)
	Local $pTemp = DllCall($__SDL_DLL_ffmpeg, "ptr:cdecl", "SDL_ffmpegGetAudioStream", "ptr", $pFile, "uint", $iAudioID)
	Return $pTemp[0]
EndFunc

;Untested
Func _SDL_ffmpegSelectAudioStream($pFile, $iAudioID)
	Local $iTemp = DllCall($__SDL_DLL_ffmpeg, "int:cdecl", "SDL_ffmpegSelectAudioStream", "ptr", $pFile, "int", $iAudioID)
	Return SetError($iTemp[0], 0, ($iTemp[0]=0))
EndFunc

#cs
/* SDL_ffmpegFile create / destroy */
EXPORT SDL_ffmpegFile* SDL_ffmpegOpen( const char* filename );

EXPORT void SDL_ffmpegFree( SDL_ffmpegFile* file );

EXPORT SDL_ffmpegStream* SDL_ffmpegGetAudioStream( SDL_ffmpegFile *file, uint32_t audioID);

EXPORT int SDL_ffmpegSelectAudioStream( SDL_ffmpegFile* file, int audioID);
#ce
#EndRegion

#Region SDL_draw.dll
;Doesn't work
Func _SDL_Draw_Pixel($pSuper, $iX, $iY, $iColor)
	DllCall($__SDL_DLL_draw, "none", "Draw_Pixel", "ptr", $pSuper, "short", $iX, "short", $iY, "uint", $iColor)
;~ 	;~ 	ConsoleWrite(@error & @CRLF)
EndFunc

;Doens't work
Func _SDL_Draw_Line($pSuper, $iX, $iY, $iX2, $iY2, $iColor)
	DllCall($__SDL_DLL_draw, "none:cdecl", "Draw_Line", "ptr", $pSuper, "short", $iX, "short", $iY, "short", $iX2, "short", $iY2, "uint", $iColor)
;~ 	;~ 	ConsoleWrite(@error & @CRLF)
EndFunc

#cs
void Draw_Pixel(SDL_Surface *super, Sint16 x, Sint16 y, Uint32 color);
void (*Draw_Line)(SDL_Surface *super, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
#ce
#EndRegion

#EndRegion

#Region Internal functions
Func __Joe_Zimmerman()
	_SDL_Quit()
	Exit
EndFunc

Func _CW($Whatever, $Whatever2 = @ScriptLineNumber, $Whatever3 = @error)		;Another variant of the mighty _CW used in many of my scripts.
	 	ConsoleWrite("Line: " & $Whatever2 & " /" & $Whatever3 & "/" & VarGetType($Whatever) & "/" & $Whatever & "/" & @CRLF)
EndFunc

;~ Func _StrutDump($pStruct, $iLoop)
;~ 	For $X = 1 To $iLoop
;~ 		Local $Data = DllStructGetData($pStruct, $X)
		;~ 	ConsoleWrite($Data & @CRLF)
;~ 	Next
;~ EndFunc

;~ Func _SkjutDump($pStruct, $iLoop)
;~ 	Local $Data
;~ 	For $X = 1 To $iLoop
;~ 		$Data &= DllStructGetData($pStruct, $X)
;~ 		$Data &= "/"
;~ 	Next
	;~ 	ConsoleWrite($Data & @CRLF)
;~ EndFunc

Func _BuntDump($pStruct, $iLoop)
	Local $aData[$iLoop]
	For $X = 0 To $iLoop -1
		$aData[$X] = DllStructGetData($pStruct, $X +1)
	Next
	Return $aData
EndFunc

Func _LumpDump($pStruct, $sEventStruct)
	StringReplace($sEventStruct, ";", ";")
	Local $iLoop = @extended
	Local $aData[$iLoop +1]
	For $X = 0 To $iLoop
		$aData[$X] = DllStructGetData($pStruct, $X +1)
	Next
	Return $aData
EndFunc
#EndRegion
