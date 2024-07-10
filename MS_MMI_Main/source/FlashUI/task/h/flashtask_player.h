/*
 * FileName: FlashTask_player.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */

#ifndef FLASHTASK_PLAYER_H
#define FLASHTASK_PLAYER_H

#include "amuse_input.h"


enum
{
	AMUSE_INIT_OK = 0,
	AMUSE_INIT_FAILED,
	AMUSE_INIT_FAILED_PLAYER_CREATE,
};

enum
{
	AMUSE_FRAME_NEXT = 0,
	AMUSE_FRAME_EXIT,
	AMUSE_FRAME_FAILED
};

enum
{
	AMUSE_EXIT_OK = 0,
	AMUSE_EXIT_FAILED
};


/****************************************************************
 * Function:       Flash_playfile_init
 * Description:   Open the player to play the FLASH file
 * Params:
 			@file_name: the path of the FLASH file to play
 			@width: the output width
 			@height: the output height
 * Return:        
 			AMUSE_INIT_OK: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int Flash_playfile_init(char* file_name, int width, int height);


/****************************************************************
 * Function:       Flash_playfile_decode
 * Description:   Run the player routine to decode FLASH file
  * Params:	None
 * Return:        
 			AMUSE_FRAME_NEXT: succeed
 			AMUSE_FRAME_EXIT: player quits by user or AS
 			AMUSE_FRAME_FAILED: player quits of error
 * Others:         
 ****************************************************************/
int Flash_playfile_decode(void);


/****************************************************************
 * Function:       Flash_playfile_destroy
 * Description:   Destroy the player
  * Params:	None
 * Return:        
 			AMUSE_EXIT_OK: succeed
 			AMUSE_EXIT_FAILED: errors occured when destroying the player
 * Others:         
 ****************************************************************/
int Flash_playfile_destroy(int success);

void amuse_player_pause(void);
void amuse_player_resume(void);
int amuse_sound_play(void);
int amuse_process_input(amuse_input_event* input);
int amuse_display_update_rect(int x,int y, int width,int height);
int amuse_get_player_state(void);

#endif	// FLASHTASK_PLAYER_H

