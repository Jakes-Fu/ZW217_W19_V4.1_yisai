/*
* amuse_key.h
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains key interface,  including key code,
* key mod definitions and key mapping interface.
*/
#ifndef _AMUSE_KEY_INTERFACE_H_
#define _AMUSE_KEY_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef  IMPORT_C
#if WIN32
#define IMPORT_C	__declspec(dllimport)
#define EXPORT_C   __declspec(dllexport)
#else
#define IMPORT_C extern
#define EXPORT_C
#endif
#endif

/* Key code definitions */
enum
{
	AMUSE_KEY_INVALID = 0,

	AMUSE_KEY_MOUSE_LEFT = 1,
	AMUSE_KEY_MOUSE_RIGHT = 2,

	AMUSE_KEY_BACKSPACE = 8,
	AMUSE_KEY_TAB = 9,

	AMUSE_KEY_CLEAR = 12,
	AMUSE_KEY_ENTER = 13,

	AMUSE_KEY_SHIFT = 16,
	AMUSE_KEY_CONTROL = 17,
	AMUSE_KEY_ALT = 18,/*//? not described in help doc*/
	AMUSE_KEY_PAUSE = 19,
	AMUSE_KEY_CAPSLOCK = 20,

	AMUSE_KEY_ESCAPE = 27,

	AMUSE_KEY_SPACE = 32,
	AMUSE_KEY_PGUP,
	AMUSE_KEY_PGDN,
	AMUSE_KEY_END,
	AMUSE_KEY_HOME,

	AMUSE_KEY_LEFT = 37,
	AMUSE_KEY_UP,
	AMUSE_KEY_RIGHT,
	AMUSE_KEY_DOWN = 40,

	AMUSE_KEY_INSERT = 45,
	AMUSE_KEY_DELETE = 46,
	AMUSE_KEY_HELP = 47,/*?*/

	AMUSE_KEY_0 = 48,
	AMUSE_KEY_1,
	AMUSE_KEY_2,
	AMUSE_KEY_3,
	AMUSE_KEY_4,
	AMUSE_KEY_5,
	AMUSE_KEY_6,
	AMUSE_KEY_7,
	AMUSE_KEY_8,
	AMUSE_KEY_9 = 57,

	/*a~z and A~Z(not distinguish upper and lower)*/
	AMUSE_KEY_A = 65,
	AMUSE_KEY_B,
	AMUSE_KEY_C,
	AMUSE_KEY_D,
	AMUSE_KEY_E,
	AMUSE_KEY_F,
	AMUSE_KEY_G,
	AMUSE_KEY_H,
	AMUSE_KEY_I,
	AMUSE_KEY_J,
	AMUSE_KEY_K,
	AMUSE_KEY_L,
	AMUSE_KEY_M,
	AMUSE_KEY_N,
	AMUSE_KEY_O,
	AMUSE_KEY_P,
	AMUSE_KEY_Q,
	AMUSE_KEY_R,
	AMUSE_KEY_S,
	AMUSE_KEY_T,
	AMUSE_KEY_U,
	AMUSE_KEY_V,
	AMUSE_KEY_W,
	AMUSE_KEY_X,
	AMUSE_KEY_Y,
	AMUSE_KEY_Z = 90,

	/*key pad*/
	AMUSE_KEY_KP_0 = 96,
	AMUSE_KEY_KP_1,
	AMUSE_KEY_KP_2,
	AMUSE_KEY_KP_3,
	AMUSE_KEY_KP_4,
	AMUSE_KEY_KP_5,
	AMUSE_KEY_KP_6,
	AMUSE_KEY_KP_7,
	AMUSE_KEY_KP_8,
	AMUSE_KEY_KP_9,
	AMUSE_KEY_KP_MULTIPLY,
	AMUSE_KEY_KP_PLUS = 107,
	AMUSE_KEY_KP_ENTER = 13,
	AMUSE_KEY_KP_MINUS = 109,
	AMUSE_KEY_KP_PERIOD,
	AMUSE_KEY_KP_DIVIDE = 111,

	/*function key*/
	AMUSE_KEY_F1 = 112,
	AMUSE_KEY_F2,
	AMUSE_KEY_F3,
	AMUSE_KEY_F4,
	AMUSE_KEY_F5,
	AMUSE_KEY_F6,
	AMUSE_KEY_F7,
	AMUSE_KEY_F8,
	AMUSE_KEY_F9,
	AMUSE_KEY_F10,/*F10: not used in AS2*/
	AMUSE_KEY_F11,
	AMUSE_KEY_F12,
	AMUSE_KEY_F13,
	AMUSE_KEY_F14,
	AMUSE_KEY_F15 = 126,

	/*others*/
	AMUSE_KEY_NUM_LOCK = 144,
	AMUSE_KEY_SCROLL_LOCK = 145,
	/*rich add */
	AMUSE_KEY_PRINT = 146,

	AMUSE_KEY_SEMICOLON = 186,	/* ;:*/
	AMUSE_KEY_EQUALS = 187,/* =+*/
	AMUSE_KEY_COMMA = 188,/* ,<*/
	AMUSE_KEY_MINUS = 189,/* -_*/
	AMUSE_KEY_PERIOD = 190,/* .>*/
	AMUSE_KEY_SLASH = 191,/* /?*/
	AMUSE_KEY_BACKTICK = 192,/* `~*/
	AMUSE_KEY_LEFT_BRACKET = 219,/* [{*/
	AMUSE_KEY_BACKSLASH = 220,/* \|*/
	AMUSE_KEY_RIGHT_BRACKET = 221,/* ]}*/
	AMUSE_KEY_QUOTE = 222,/* "'*/

	/* Add by Leonard  */
	AMUSE_KEY_STAR = 224,/* \* */
	AMUSE_KEY_POUND = 225,/* # */
	AMUSE_KEY_GREEN = 226,/*call*/
	AMUSE_KEY_RED = 227,/*hang up */

	AMUSE_EXTENEDEDKEY_KEY1 = 228,
	AMUSE_EXTENEDEDKEY_KEY2 = 229,
	AMUSE_EXTENEDEDKEY_KEY3 = 230,
	AMUSE_EXTENEDEDKEY_KEY4 = 231,
	AMUSE_EXTENEDEDKEY_KEY5 = 232,
	AMUSE_EXTENEDEDKEY_KEY6 = 233,
	AMUSE_EXTENEDEDKEY_KEY7 = 234,
	AMUSE_EXTENEDEDKEY_KEY8 = 235,
	AMUSE_EXTENEDEDKEY_KEY9 = 236,
	AMUSE_EXTENEDEDKEY_KEY10 = 237,

	/*John add for mouse move key simulate*/
	AMUSE_KEY_MOUSE_MOVE_LEFT,
	AMUSE_KEY_MOUSE_MOVE_RIGHT,
	AMUSE_KEY_MOUSE_MOVE_UP,
	AMUSE_KEY_MOUSE_MOVE_DOWN,

	AMUSE_KEY_LAST,
};

/* key mod definition */
enum{
	AMUSE_KMOD_NONE  	= 0x0000,
	AMUSE_KMOD_LSHIFT	= 0x0001,
	AMUSE_KMOD_RSHIFT	= 0x0002,
	AMUSE_KMOD_LCTRL 	= 0x0040,
	AMUSE_KMOD_RCTRL 	= 0x0080,
	AMUSE_KMOD_LALT  	= 0x0100,
	AMUSE_KMOD_RALT  	= 0x0200,
	AMUSE_KMOD_LMETA 	= 0x0400,
	AMUSE_KMOD_RMETA 	= 0x0800,
	AMUSE_KMOD_NUM   	= 0x1000,
	AMUSE_KMOD_CAPS  	= 0x2000,
	AMUSE_KMOD_MODE  	= 0x4000,
	AMUSE_KMOD_RESERVED = 0x8000
};

#define AMUSE_KMOD_CTRL		(AMUSE_KMOD_LCTRL|AMUSE_KMOD_RCTRL)
#define AMUSE_KMOD_SHIFT		(AMUSE_KMOD_LSHIFT|AMUSE_KMOD_RSHIFT)
#define AMUSE_KMOD_ALT		(AMUSE_KMOD_LALT|AMUSE_KMOD_RALT)
#define AMUSE_KMOD_META		(AMUSE_KMOD_LMETA|AMUSE_KMOD_RMETA)

/* key mapping definitions */
struct key_map_item{
	int soft_key;
	int default_soft_key;
	char* key_name;
};
typedef struct key_map_item key_map_item;

struct key_map{
	key_map_item* keys;
	int keynum;
};
typedef struct key_map key_map;

/** 
* This function gets corresponding amuse key code from custom key code.
*
* Parameters: 
* keymap: key map from custom key to amuse key
* key: custom key code
* 
* Returns: amuse key code
*/
EXPORT_C int amuse_key_getAmuseKeyCode(key_map* keymap, int hardkey);

/** 
* This function loads key mapping manager flash GUI, to change key map settings.
*
* Parameters: 
* path: location of the manager flash
* key: whether to load it from memory
* 
* Returns: none
*/
EXPORT_C void amuse_key_loadMapManager(char* path, int fromMem);

#ifdef __cplusplus
}
#endif

#endif
