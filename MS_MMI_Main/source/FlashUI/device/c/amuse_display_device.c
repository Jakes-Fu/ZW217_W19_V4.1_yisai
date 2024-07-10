/*
* amuse_display.c
* Copyright (C) 2011 Microrapid Inc
* Author: Black <black@Microrapid.com>
* 
* This file implementes display interface 
*   for win32 platform.
*/
#ifndef FLASHUI_WIN32_SIM_DLL
#include "dal_lcd.h"
#include "guilcd.h"
#include "guiblock.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_position.h"
#include "guiref_scale.h"
#include "flashtask_msg.h"
#include "mmi_appmsg.h"
#include "mmiflash_export.h"
#endif
#include "amuse.h"

//#include "amuse_config_device.h"
#include "amuse_display.h"
#include "amuse_file_device.h"

static amuse_canvas s_canvas_buf = (amuse_canvas)0;
static GUI_RECT_T	s_flash_update_rect = {0};


#define GUI_LCD_FLASH	LCD_ID_0
#define GUI_BLOCK_FLASH	GUI_BLOCK_0
#define USE_MAIN_BLOCK		0

static amuse_canvas s_canvas_ping = NULL;

static short s_screen_width,s_screen_height;

void amuse_set_screen_size(short w, short h)
{
	s_screen_width = w;
	s_screen_height = h;
}

static __inline GUI_LCD_DEV_INFO GetFlashDisplayBlockInfo(void)
{
	GUI_LCD_DEV_INFO lcd_dev = {GUI_LCD_FLASH, GUI_BLOCK_FLASH};	
	return lcd_dev;
}

amuse_canvas
amuse_display_init(int width, int height,int color_depth)
{
	s_canvas_buf = NULL;
	s_canvas_buf = (amuse_canvas)MMIAPIFlash_GetFlashLayerBufPtr();	
	s_canvas_ping = (amuse_canvas)SCI_ALLOC_APP(s_screen_width*s_screen_height*2);
	SCI_ASSERT(s_canvas_ping != NULL);/*assert verified*/
	return s_canvas_ping;
}

int amuse_display_destory(void)
{
	GUI_LCD_DEV_INFO lcd_dev_info = GetFlashDisplayBlockInfo();

	if(s_canvas_ping != NULL)
	{
		SCI_FREE(s_canvas_ping);
		s_canvas_ping = NULL;
	}
	if (GUIBLOCK_Destroy(&lcd_dev_info))
	{
		return 0;
	}

	return -1;
}


void amuse_hangup_canvas(void)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = GetFlashDisplayBlockInfo();
	GUIBLOCK_Disable(&lcd_dev_info);
}

int FlashTask_is_canvas_enable(void)
{
	GUI_LCD_DEV_INFO	lcd_dev_info = GetFlashDisplayBlockInfo();
	return GUIBLOCK_IsEnable(&lcd_dev_info);
}

void amuse_restore_canvas(void)
{

}

int amuse_display_lock()
{
	return 0;
}

int amuse_display_unlock()
{
	return 0;
}

//check if need to update
int amuse_lcd_update()
{
	return 0;
}

void  amuse_get_flash_update_rect(GUI_RECT_T *pRect)
{
	if( pRect == NULL ) return;
	memcpy(pRect, &s_flash_update_rect, sizeof(GUI_RECT_T) );
}

int amuse_display_update_rect(int x, int y, int width, int height)
{
    static BOOLEAN s_is_first_frame = TRUE;
#ifndef WIN32
	/*lint -e746 -e718*/
	if(!MMIAPIFlash_FlashIsVisiable() || !amuse_get_player_state())
		return 0;
#endif
    if(s_is_first_frame)
    {
        s_is_first_frame = FALSE;
        return 0;
    }

	s_flash_update_rect.left = x;
	s_flash_update_rect.top =  y;
	s_flash_update_rect.right = x+width;
	s_flash_update_rect.bottom = y+height;

	if(s_flash_update_rect.right >= s_screen_width)
		s_flash_update_rect.right = s_screen_width - 1;
	if(s_flash_update_rect.bottom >= s_screen_height) 
		s_flash_update_rect.bottom = s_screen_height - 1;

	if(s_flash_update_rect.left < s_flash_update_rect.right && s_flash_update_rect.top < s_flash_update_rect.bottom)
	{
		amuse_printf("@@@@@amuse_display_update_rect ,screen need update");
        if(PNULL!=s_canvas_buf)
        {
    		memcpy(s_canvas_buf, s_canvas_ping, s_screen_width*s_screen_height*2);
            FlashTask_SendSignalToAPP(MSG_FLASH_UPDATE_WINDOW, 0);
    		return 1;
        }
        else
        {
            return 0;
        }
	}
	else
	{
		return 0;
	}
}


void amuse_show_loading_picture()
{
}
void Flash_display_loading_progress(int pro)
{
}

static int shoud_show_progressbar=0;
int need_show_progressbar(void)
{   
	return shoud_show_progressbar;
}
void set_show_progressbar(int value)
{    
	shoud_show_progressbar=value;
}


void amuse_set_frame_rate(uint16 frame_rate)
{
}
