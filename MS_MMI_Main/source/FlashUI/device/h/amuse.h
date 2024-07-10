/*
* amuse.h
* Copyright (C) 2011 Microrapid Inc
* Author: Tim <tim@Microrapid.com>
* 
* This file contains basic API of Amuse Player interface.
*/
#ifndef _AMUSE_H_
#define _AMUSE_H_

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
	
#include "amuse_key.h"

#ifdef FLASHUI_WIN32_SIM_DLL
	#include "sci_types.h"
	#include "flashUI_Device_Fun_WIN32.h"
#endif

typedef enum
{
	AMUSE_MODE_INVALID,
	AMUSE_MODE_UI,
	AMUSE_MODE_PLAYER,
}AMUSE_MODE;
/** 
 * This function creates and initializes player.
 *
 * Parameters:
 * file_name: name of the swf file to be played
 * membase: start address of heap used internally by player
 * memsize: total size of heap
 * keymap: key map to handle mapping between custom key and amuse key.
 * 
 * Returns: 0, success; -1, errors.
 */
EXPORT_C int amuse_player_create(void* membase, unsigned int memsize, key_map* keymap);


EXPORT_C void amuse_player_load_mainmovie_from_memory(unsigned char *data, int length, char *ident);
EXPORT_C void amuse_player_load_mainmovie_from_url(char *url);
/** 
 * This function destroys player.
 *
 * Parameters: none
 * 
 * Returns: 0, success; -1, errors.
 */
EXPORT_C int amuse_player_destory(int success);

/** 
 * This function runs a frame of flash content.
 *
 * Parameters: none
 * 
 * Returns: none
 */
EXPORT_C void amuse_play_frame(void);

EXPORT_C unsigned short amuse_GetFrameRate(void);
/**
 * This function sets total stack size that amuse can use,
 * to avoid stack overflow.
 * 
 * Parameters:
 * stackSizeAvailable: size of stack that could be used for Amuse, should be smaller than total stack size
 *
 * Returns: none
 */
EXPORT_C void amuse_stack_init(unsigned int stackSizeAvailable);

/*
*  This function fills audio data to the buffer.
*
*  Parameters: 
*  buffer: the audio buffer address
*  bufferLen: the size of the buffer
*
*  Returns: the length of data filled in the buffer 
*/
EXPORT_C int amuse_sound_fill_data(unsigned char* buffer, int bufferLen);

/*
 * This function computes the size of the audio data in the sound buffer.
 * 
 * Parameters: None
 *
 * Returns: the size of data.if 0,means no sound to play now.
 */
EXPORT_C int amuse_sound_get_buffer_data_size(void);

/*
 * This function hides the cursor.
 * 
 * Parameters: None
 *
 * Returns: None.
 */
EXPORT_C void amuse_cursor_hide(void);

/*
 * This function displays the cursor.
 * 
 * Parameters: None
 *
 * Returns: None.
 */
EXPORT_C void amuse_cursor_display(void);

/*
 * Function prototype, to load a swf file from memory directly.
 * 
 * Parameters:
 * filename: the swf name to be loaded
 * pswf_size: the size of the swf readed
 *
 * Returns: the swf buffer address
*/
typedef unsigned char* (*load_swf_mem_func)(char* filename, int* pswf_size);

/*
*  This function registers a function which loads swf file from memory directly.
*
*  Parameters: 
*  func: the function to be registered
*
*  Returns: None 
*/
EXPORT_C void register_load_mem_func(load_swf_mem_func func);

/*
*  This function sets the media file name to amuse, file type is flv or mp3.
*
*  Parameters: 
*  name: full path of the file
*
*  Returns: None 
*/
EXPORT_C void Player_SetMediaFileName(char* name);

/*
 * This function sets path of the font resource file.
 *
 * Parameters: 
 * path: full path of the font file
 *
 * Returns: None
 */
EXPORT_C void amuse_set_font_path(const char* path);

/*
 * This function decides whether to control flash frame rate by Amuse.
 *
 * Parameters: 
 * control: 1, framerate is controlled by Amuse; 0, not controlled by Amuse
 *
 * Returns: None
 */
EXPORT_C void amuse_control_framerate(int control);

/*
* This function gets the mouse position in screen.
*
* Parameters: 
* x_value: x position in screen
* y_value: y position in screen
*
* Returns: None
*/
EXPORT_C void get_mouse_position(int *x_value,int*y_value);


/*
* This function process input event (keyboard and mouse)
*
* Parameters: 
* keymap: key map
*
* Returns: 0, ok; -1, quit
*/
EXPORT_C int amuse_event_process(key_map* keymap);
/*
* This function set swf url 
*
* Parameters: url or swf path
*
*
* Returns: none
*/
EXPORT_C void Player_SetSWFURL(char *swfurl);
/*
* This function for load var (online version)
*
* Parameters: vars
* 
*
* Returns: none
*/
EXPORT_C void PlayerSetFlashVars(char *playerVars);


EXPORT_C void
Amuse_SetMode(AMUSE_MODE mod);

EXPORT_C AMUSE_MODE
Amuse_GetMode();

EXPORT_C void amuse_EnableHiMemStat(unsigned char enable);

EXPORT_C void amuse_PrintMemoryStat(void);
#ifdef __cplusplus
}
#endif

#endif
