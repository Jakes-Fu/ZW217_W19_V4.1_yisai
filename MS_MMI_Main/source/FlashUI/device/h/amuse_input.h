/*
* amuse_input.h
* Copyright (C) 2011 Microrapid, Inc.
* Authors: Tim <tim@Microrapid.com>
* This file contains input interface.
*/

#ifndef _AMUSE_INPUT_H_
#define _AMUSE_INPUT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* amuse input type */
enum{
	AMUSE_NOINPUT = 0,
	AMUSE_KEYDOWN,
	AMUSE_KEYUP,
	AMUSE_MOUSEMOTION,
	AMUSE_MOUSEDOWN,
	AMUSE_MOUSEUP,
	AMUSE_QUIT,
	AMUSE_USERINPUT = 11,
	AMUSE_INPUTNUM = 16
};

struct amuse_input_key{
	short type;
	short code;
	int mod;
};
typedef struct amuse_input_key amuse_input_key;

/* mouse button */
enum{
	AMUSE_MOUSE_BUTTON_LEFT = 1,
	AMUSE_MOUSE_BUTTON_MIDDLE,
	AMUSE_MOUSE_BUTTON_RIGHT,
	AMUSE_MOUSE_BUTTON_WHEELUP = 4,
	AMUSE_MOUSE_BUTTON_WHEELDOWN = 5
};

struct amuse_input_mouse{
	short type;
	short button;
	int x;
	int y;
};
typedef struct amuse_input_mouse amuse_input_mouse;

union amuse_input_event{
	short type;
	amuse_input_key key;
	amuse_input_mouse mouse;
};
typedef union amuse_input_event amuse_input_event;

/*
//////////////////////sync with SDL event//////////////////////
typedef unsigned char UI8;
typedef unsigned short UI16;
typedef signed short  SI16;
*/

/* Application visibility event structure */
typedef struct Sys_ActiveEvent {
	unsigned char type;	/* SDL_ACTIVEEVENT */
	unsigned char gain;	/* Whether given states were gained or lost (1/0) */
	unsigned char state;	/* A mask of the focus states */
} Sys_ActiveEvent;

/* Keyboard event structure */
typedef struct Sys_KeyboardEvent {
	unsigned char type;	/* SDL_KEYDOWN or SDL_KEYUP */
	unsigned char which;	/* The keyboard device index */
	unsigned char state;	/* SDL_PRESSED or SDL_RELEASED */
	unsigned char mod;
	unsigned short code;/*SDL_keysym keysym;*/
} Sys_KeyboardEvent;

/* Mouse motion event structure */
typedef struct Sys_MouseMotionEvent {
	unsigned char type;	/* SDL_MOUSEMOTION */
	unsigned char which;	/* The mouse device index */
	unsigned char state;	/* The current button state */
	unsigned short x, y;	/* The X/Y coordinates of the mouse */
	signed short xrel;	/* The relative motion in the X direction */
	signed short yrel;	/* The relative motion in the Y direction */
} Sys_MouseMotionEvent;

/* Mouse button event structure */
typedef struct Sys_MouseButtonEvent {
	unsigned char type;	/* SDL_MOUSEBUTTONDOWN or SDL_MOUSEBUTTONUP */
	unsigned char which;	/* The mouse device index */
	unsigned char button;	/* The mouse button index */
	unsigned char state;	/* SDL_PRESSED or SDL_RELEASED */
	unsigned short x, y;	/* The X/Y coordinates of the mouse at press time */
} Sys_MouseButtonEvent;

/* Joystick axis motion event structure */
typedef struct Sys_JoyAxisEvent {
	unsigned char type;	/* SDL_JOYAXISMOTION */
	unsigned char which;	/* The joystick device index */
	unsigned char axis;	/* The joystick axis index */
	signed short  value;	/* The axis value (range: -32768 to 32767) */
} Sys_JoyAxisEvent;

/* Joystick trackball motion event structure */
typedef struct Sys_JoyBallEvent {
	unsigned char type;	/* SDL_JOYBALLMOTION */
	unsigned char which;	/* The joystick device index */
	unsigned char ball;	/* The joystick trackball index */
	signed short  xrel;	/* The relative motion in the X direction */
	signed short yrel;	/* The relative motion in the Y direction */
} Sys_JoyBallEvent;

/* Joystick hat position change event structure */
typedef struct Sys_JoyHatEvent {
	unsigned char type;	/* SDL_JOYHATMOTION */
	unsigned char which;	/* The joystick device index */
	unsigned char hat;	/* The joystick hat index */
	unsigned char value;	/* The hat position value:
					SDL_HAT_LEFTUP   SDL_HAT_UP       SDL_HAT_RIGHTUP
					SDL_HAT_LEFT     SDL_HAT_CENTERED SDL_HAT_RIGHT
					SDL_HAT_LEFTDOWN SDL_HAT_DOWN     SDL_HAT_RIGHTDOWN
					Note that zero means the POV is centered.
					*/
} Sys_JoyHatEvent;

/* Joystick button event structure */
typedef struct Sys_JoyButtonEvent {
	unsigned char type;	/* SDL_JOYBUTTONDOWN or SDL_JOYBUTTONUP */
	unsigned char which;	/* The joystick device index */
	unsigned char button;	/* The joystick button index */
	unsigned char state;	/* SDL_PRESSED or SDL_RELEASED */
} Sys_JoyButtonEvent;

/* The "window resized" event
When you get this event, you are responsible for setting a new video
mode with the new width and height.
*/
typedef struct Sys_ResizeEvent {
	unsigned char type;	/* SDL_VIDEORESIZE */
	int w;		/* New width */
	int h;		/* New height */
} Sys_ResizeEvent;

/* The "screen redraw" event */
typedef struct Sys_ExposeEvent {
	unsigned char type;	/* SDL_VIDEOEXPOSE */
} Sys_ExposeEvent;

/* The "quit requested" event */
typedef struct Sys_QuitEvent {
	unsigned char type;	/* SDL_QUIT */
} Sys_QuitEvent;

/* A user-defined event type */
typedef struct Sys_UserEvent {
	unsigned char type;	/* SDL_USEREVENT through SDL_NUMEVENTS-1 */
	int code;	/* User defined event code */
	void *data1;	/* User defined data pointer */
	void *data2;	/* User defined data pointer */
} Sys_UserEvent;

/* General event structure */
typedef union Amuse_System_Event {
	unsigned char type;
	Sys_ActiveEvent active;
	Sys_KeyboardEvent key;
	Sys_MouseMotionEvent motion;
	Sys_MouseButtonEvent button;
	Sys_JoyAxisEvent jaxis;
	Sys_JoyBallEvent jball;
	Sys_JoyHatEvent jhat;
	Sys_JoyButtonEvent jbutton;
	Sys_ResizeEvent resize;
	Sys_ExposeEvent expose;
	Sys_QuitEvent quit;
	Sys_UserEvent user;
} Amuse_System_Event;

struct key_map;

/**
* This function get key and mouse event from system
 *
* Parameters: none
* Returns: none
*/
void handle_key_mouse_event(struct key_map* keymap);

/** 
* This function add key and mouse event to player event queue
 *
* Parameters: event queue ,event
* Returns: status
 */
int amuse_add_system_event(amuse_input_event* event);

#ifdef __cplusplus
}
#endif

#endif
