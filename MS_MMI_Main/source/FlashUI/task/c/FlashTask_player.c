/*
 * FileName: FlashTask_player.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */
 
/* =======================================================================
 * Include headers
 * ======================================================================= */ 
#include "amuse.h"
#include "amuse_input.h"
#include "amuse_key_device.h"
#include "amuse_sound_device.h"
#include "amuse_config_device.h"
#include "amuse_time_device.h"
#include "amuse_sound_device.h"
#include "amuse_display.h"
#include "Block_mem.h"
#include <setjmp.h>
#include "mmi_default.h"
#include "FlashTask_main.h"
#include "FlashTask_player.h"
#include "sci_api.h"
#include "amuse_memory_device.h"


/* =======================================================================
 *  Macro defines
 * ======================================================================= */
#define AMUSE_KEYNUM 0x100 

/* =======================================================================
 *  External variables
 * ======================================================================= */

/* =======================================================================
 *  Global variables
 * ======================================================================= */
static struct key_map* s_amuse_keymap = 0;

//!!!s_amuse_memory小于6M在当前的swf中可能出现异常!!!
static unsigned char s_amuse_memory[(6)*(1024)*(1024)] = {0};
static int s_amuse_init_ok = 0;




//=================================================================
// Exported Functions
//=================================================================
//初始化Flash引擎，栈空间、配置选项等
int Flash_playfile_init(char* file_name, int width, int height)
{
	s_amuse_init_ok = 0;	
	
	amuse_stack_init(32*1024-4*1024);
	amuse_config_setDefault(width, height);
	//amuse_jpeg_init	();
	
	amuse_set_font_path(FLASHTASK_FONT_FILE_PATH);
	s_amuse_keymap = amuse_key_createMap(AMUSE_KEYNUM);
	amuse_sound_open();
	if(amuse_player_create(s_amuse_memory, AMUSE_MEMORY_SIZE, s_amuse_keymap))
	{
		return AMUSE_INIT_FAILED_PLAYER_CREATE;
	}
	amuse_player_load_mainmovie_from_url(file_name);
	s_amuse_init_ok = 1;
	return AMUSE_INIT_OK;
}

int Flash_playfile_decode(void)
{
	int err_code;
	unsigned int tm=0,frame_time;
	unsigned short frame;
	
	jmp_buf *jmp = amuse_getJmp();

	if (!s_amuse_init_ok)
	{
		return AMUSE_FRAME_EXIT;
	}
	
	err_code = setjmp(*jmp);
	if (err_code != 0)
	{
		if (err_code < 0)
		{
			return AMUSE_FRAME_FAILED;	// decode error
		}
		else
		{
			return AMUSE_FRAME_EXIT;		// quit by user or AS
		}
	}
	
	tm = amuse_getTickCount();
	/*lint -e746*/
	amuse_play_frame();		
	amuse_sound_play();
	tm = amuse_getTickCount() - tm;
	
	frame = amuse_GetFrameRate();/*lint !e746*/

	if(frame > 0 )
	{//当走帧太快时，将task停一段时间以便节能
		frame_time = 1000/frame;
		if( tm < frame_time )
		{
			SCI_Sleep(frame_time-tm);
		}
	}

	return AMUSE_FRAME_NEXT;
}

//对Flash资源进行释放
int Flash_playfile_destroy(int success)
{
	if (!s_amuse_init_ok)
	{
		return AMUSE_EXIT_OK;
	}
	s_amuse_init_ok = 0;

	//kill the user input first for fix the crash . by Leonard 2011.05.05
	amuse_key_destroyMap(s_amuse_keymap); 

	amuse_player_destory(success);
	amuse_display_destory();
	amuse_sound_close();
	//amuse_jpeg_destroy();	
	//amuse_free_memory();	
	FlashTask_RestoreFreq();
	return AMUSE_EXIT_OK;
}


