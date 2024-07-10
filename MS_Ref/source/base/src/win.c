/******************************************************************************
 ** File Name:      lcd.c                                                     *
 ** Author:         Richard Yang                                              *
 ** DATE:           21/11/2001                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 21/11/2001     Richard.Yang     Create.                                   *
 ** 13/03/2002     Lin.liu          Added dc_draw_bmp                         * 
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include <string.h>
#include "os_api.h"
#include "win.h"
#include "dal_lcd.h"
#include "lcd_api.h"   /* LCD_BACKLIGHT_ON  */
#include "tasks_id.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif









































































LOCAL wnd_list_s  * _wnd_list;
LOCAL void win_insert(wnd_list_s * wnd_list);

PUBLIC void wnd_init(void)
{
    _wnd_list = SCI_NULL;
}


PUBLIC SCI_HWND wnd_create(
               char * win_name, 
			   uint32 ui_style, 
			   int x, 
			   int y, 
			   uint32 ui_width,
			   uint32 ui_height,
			   SCI_HWND wnd_parent_ptr,
			   BLOCK_ID task_ID)
{
	wnd_list_s * m_wnd_list;

	m_wnd_list = SCI_ALLOC_APP(sizeof(wnd_list_s));

	strncpy(m_wnd_list->wnd.win_name, win_name, SCI_MAX_NAME_SIZE);
	m_wnd_list->wnd.style          = ui_style;
	m_wnd_list->wnd.task_ID        = task_ID;
	m_wnd_list->wnd.wnd_parent_ptr = wnd_parent_ptr;
	m_wnd_list->wnd.x              = x;
	m_wnd_list->wnd.y              = y;
	m_wnd_list->wnd.ui_width       = ui_width;
	m_wnd_list->wnd.ui_height      = ui_height;

	win_insert(m_wnd_list);

	return ((SCI_HWND) m_wnd_list);
}

LOCAL void win_insert(wnd_list_s * wnd_list)
{
    SCI_DisableIRQ();

	if (_wnd_list == SCI_NULL)
	{
		wnd_list->pre = wnd_list;
		wnd_list->suc = wnd_list;

		_wnd_list = wnd_list;
	}
	else
	{
		wnd_list->pre       = _wnd_list->pre;
		_wnd_list->pre->suc = wnd_list;
		_wnd_list->pre      = wnd_list;
		wnd_list->suc       = _wnd_list;
	}

    SCI_RestoreIRQ();
}

PUBLIC SCI_HDC dc_create(SCI_HWND wnd_ptr)
{
    dc_s * dc;

	SCI_ASSERT(wnd_ptr != SCI_NULL);/*assert verified*/

	dc = SCI_ALLOC_APP(sizeof(dc_s));

    dc->attrib_win = wnd_ptr; 
	font_select_default(&dc->font);
	dc->view_point.x  = 0;
	dc->view_point.y  = 0;
	dc->start_point.x = 0;
	dc->start_point.y = 0;
    dc->display_flag  = 0; // set the dispaly mode




	return ((SCI_HDC) dc);
}

// set the current display mode
PUBLIC void dc_set_display_flag( SCI_HDC dc_ptr, uint32 display_flag )
{
    ((dc_s *)dc_ptr)->display_flag = display_flag;
}

// remove the display mode
PUBLIC void dc_remove_display_flag( SCI_HDC dc_ptr, uint32 display_flag )
{
    ((dc_s *)dc_ptr)->display_flag &= ~(display_flag);
}



















PUBLIC font_s  * dc_get_current_font(SCI_HDC dc_ptr)
{
	return &(((dc_s *) dc_ptr)->font);
}

PUBLIC SCI_HWND dc_get_window(SCI_HDC dc_ptr)
{
	return ((dc_s *) dc_ptr)->attrib_win;
}


























































































































































































































































PUBLIC uint32 dc_draw_bmp(SCI_HDC dc_ptr, int x, int y, uint8 *bmp_ptr)
{

	lcd_bmp_info_s  bmp;
	 bmp.dc = dc_ptr;
	 bmp.x  = x;
	 bmp.y =  y;
	 bmp.bmp_data_ptr = bmp_ptr;
 
	 return  _lcd_draw_bmp(&bmp);
	
}































PUBLIC uint32 dc_text_out(SCI_HDC dc_ptr, int x, int y, uint8 * str, int size)
{
	extern uint32 _lcd_display_text(lcd_text_info_s * text_info);
	lcd_text_info_s     textInfo;

	textInfo.dc        = (dc_s *) dc_ptr;
	textInfo.x         = x;
	textInfo.y         = y;
	textInfo.text      = SCI_ALLOC_APP(size);

	memcpy(textInfo.text, str, size);

	textInfo.text_size = size;
	textInfo.size      = 0;             // Reserved now.
	textInfo.width_ptr = SCI_NULL;

    _lcd_display_text(&textInfo);

	return SCI_SUCCESS;

	
}

PUBLIC point_s * dc_move_to(SCI_HDC dc_ptr, int x, int y)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/

	((dc_s *) dc_ptr)->start_point.x = x;
	((dc_s *) dc_ptr)->start_point.y = y;
    
	return &((dc_s *) dc_ptr)->start_point;
}

PUBLIC uint32 dc_line_to(SCI_HDC dc_ptr, int x, int y, uint8 lineMode)
{

  return _lcd_line_to(dc_ptr,  x, y, lineMode);
	
}

PUBLIC void dc_lp_to_dp(SCI_HDC dc_ptr, point_s * point)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/
	SCI_ASSERT(point != SCI_NULL);/*assert verified*/

	point->x = point->x + ((dc_s *) dc_ptr)->view_point.x + ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.x;
	point->y = point->y + ((dc_s *) dc_ptr)->view_point.y + ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.y;
}

PUBLIC void dc_dp_to_lp(SCI_HDC dc_ptr, point_s * point)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/
	SCI_ASSERT(point != SCI_NULL);/*assert verified*/

	point->x = point->x - ((dc_s *) dc_ptr)->view_point.x - ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.x;
	point->y = point->y - ((dc_s *) dc_ptr)->view_point.y - ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.y;
}

PUBLIC uint32	dc_get_rect(SCI_HDC dc_ptr, rect_s * rect)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/

	rect->left_top.x = 0;
	rect->left_top.y = 0;

	rect->right_bottom.x = ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.ui_width;
	rect->right_bottom.y = ((wnd_list_s *) ((dc_s *) dc_ptr)->attrib_win)->wnd.ui_height;

	return SCI_SUCCESS;
}

PUBLIC uint32 font_select_default(font_s * font)
{
	font->font_type = SYS_DEFAULT;
    font->width     = 8;
	font->height    = 16;

	return SCI_SUCCESS;
}

PUBLIC uint32 font_select_font(
    SCI_HDC          hdc, 
    font_e          font_type,
    uint8           width,
    uint8           height)
{

    // ignore font type and width

    ((dc_s *)hdc)->font.font_type =  font_type;
   
    // We only support 8X8, 8X16(Chinese is 16X16)
    if( height < 12 )
    {
        height = 8;  width = 8;
    }
    else
    {
        height = 16; width = 8;
    }
    ((dc_s *)hdc)->font.height = height;
    ((dc_s *)hdc)->font.width = width;

    return SCI_SUCCESS;
}

PUBLIC uint32 font_get_size(
    SCI_HDC            dc_ptr,
    uint8           * width_ptr,
    uint8           * height_ptr)
{
    SCI_ASSERT( SCI_NULL != dc_ptr );/*assert verified*/

    if( SCI_NULL != width_ptr )
    {
        *width_ptr = ((dc_s *)dc_ptr)->font.width;
    }

    if( SCI_NULL != height_ptr )
    {
        *height_ptr  =  ((dc_s *)dc_ptr)->font.height;
    }

    return SCI_SUCCESS;
}


extern void LCD_Clr(void);

PUBLIC uint32 dc_clr_window(SCI_HDC dc_ptr)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/

	LCD_Clr(); 

	return SCI_SUCCESS;
}

PUBLIC uint32 dc_get_cursor_pos(SCI_HDC dc_ptr, point_s * pos)
{
	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/

	pos->x = ((dc_s *) dc_ptr)->cursor_point.x;
	pos->y = ((dc_s *) dc_ptr)->cursor_point.y;

	return SCI_SUCCESS;
}

PUBLIC uint32 dc_get_txt_length(SCI_HDC dc_ptr, char * text_ptr)
{
	uint32 len;
	font_s * fnt_ptr;

	SCI_ASSERT(dc_ptr != SCI_NULL);/*assert verified*/

    fnt_ptr = dc_get_current_font(dc_ptr);

	len = strlen(text_ptr) * fnt_ptr->width;

	return len;
}

void LCD_SetBacklightMode(uint8 mode);
PUBLIC void dc_set_backlight_mode( uint32  mode )
{
    if( mode == LCD_BACKLIGHT_ON )
    {
        LCD_SetBacklightMode( LCD_BACKLIGHT_ON );
    }    
    else
    {
        LCD_SetBacklightMode( LCD_BACKLIGHT_OFF );
    }
}


PUBLIC uint32 dc_fill_rect( SCI_HDC dc_ptr, rect_s *rect_ptr, lcd_color_e  color)
{
    lcd_fill_rect_info_s   rect_info;
	//PUBLIC uint32 _lcd_fill_rect( lcd_fill_rect_info_s *info_ptr)
    rect_info.color  = color;
    if( NULL == rect_ptr )
    {
        rect_info.rect_is_valid  = 0;
    }
    else
    {
        rect_info.rect           = *rect_ptr;
        rect_info.rect_is_valid  = 1;
    }

    // Added by lin.liu : convert lp to dp
    dc_lp_to_dp(dc_ptr, &rect_info.rect.left_top);
    dc_lp_to_dp(dc_ptr, &rect_info.rect.right_bottom);
    // End Added
	_lcd_fill_rect( &rect_info);
	return SCI_SUCCESS;

}

PUBLIC uint32 dc_invert_rect( SCI_HDC dc_ptr, rect_s *rect_ptr, lcd_color_e  color)
{
//PUBLIC uint32 _lcd_invert_rect(rect_s *rect_ptr);	
	

   // Added by lin.liu : convert lp to dp
    dc_lp_to_dp(dc_ptr, &rect_ptr->left_top);
    dc_lp_to_dp(dc_ptr, &rect_ptr->right_bottom);
    // End Added
	_lcd_invert_rect(rect_ptr);
	

	return SCI_SUCCESS;
}

PUBLIC uint32 dc_draw_cursor(SCI_HDC dc_ptr, int x, int y)
{
    uint32 height;
	// PUBLIC uint32 _lcd_start_cursor(int x, int y, uint32 height)
    
	point_s         temp_point;  // Added by lin.liu : LP ==> DP

    temp_point.x = x; 
    temp_point.y = y;
    dc_lp_to_dp(dc_ptr, &temp_point);
    
    x = temp_point.x;
    y = temp_point.y;
   
    
    height   = ((dc_s *) dc_ptr)->font.height;

	_lcd_start_cursor(x, y, height);

	return SCI_SUCCESS;
}

PUBLIC uint32 dc_clear_cursor(SCI_HDC dc_ptr)
{

	_lcd_clear_cursor();
	return SCI_SUCCESS;
}


PUBLIC uint32 dc_draw_compact_bmp(SCI_HDC dc_ptr, int x, int y, uint8 *bmp_ptr)
{
     lcd_bmp_info_s  bmp;
	 bmp.dc = dc_ptr;
	 bmp.x  = x;
	 bmp.y =  y;
	 bmp.bmp_data_ptr = bmp_ptr;
 
	 return  _lcd_draw_compact_bmp(&bmp);


	return SCI_SUCCESS;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
