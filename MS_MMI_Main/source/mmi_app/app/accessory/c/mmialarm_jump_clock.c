#include "mmi_app_accessory_trc.h"
#ifdef MMI_ALARM_JUMP_CLOCK_SUPPORT
/*****************************************************************************
** File Name:      mmialarm_clock.c                                        *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                 Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"

#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
//#include "mmicc_text.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmi_common.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiwidget_position.h"
#include "graphics_draw.h"
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "guigraph.h"
#include  "mmialarm_image.h"
#include  "mmialarm_id.h"
#include  "mmialarm_text.h"
//#include "mmialarm_clock.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

#define CLK_ROTATE_PIC_NUM 2


#ifdef MAINLCD_SIZE_320X480
#define CLK_CENTER_X  171
#define CLK_TIME_X   100
//#define CLK_TIME_Y   30
#define CLK_DATE_X   117
//#define CLK_DATE_Y   77 
#define CLK_ALARM_HEIGHT 100
#elif defined MAINLCD_SIZE_240X400
#define CLK_CENTER_X  137
#define CLK_TIME_X   60
//#define CLK_TIME_Y   30
#define CLK_DATE_X   77
//#define CLK_DATE_Y   77 
#define CLK_ALARM_HEIGHT 100
#else
#define CLK_CENTER_X  136
#define CLK_TIME_X   60
//#define CLK_TIME_Y   30
#define CLK_DATE_X   77
//#define CLK_DATE_Y   77
#define CLK_ALARM_HEIGHT 0
#endif 



typedef struct {
    int32 angel;
    GUI_POINT_T  rotate_point;
    GUI_POINT_T  center_point;
}CLK_ROTATE_INFO_T_;


LOCAL CLK_ROTATE_INFO_T_  s_rotate_info_arr[CLK_ROTATE_PIC_NUM]=
{
    {
        359,
        {0,300},
        {154, 271}
    },
    {
        1,
        {250,300},
        {154, 271}
    }
};

LOCAL int s_pic_index = 0;
LOCAL uint8 s_clk_timer_id = 0;

/*****************************************************************************/
//  Description : math function: b*b.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 pow(int32 b);

/*****************************************************************************/
//  Description : Alpha blend two images
//  Global resource dependence : 
//  Author:
//  Note: the output image is in the background image buffer, overwritting the orignial data.
//		  the type of the output image data is the same as that of the background image
/*****************************************************************************/
LOCAL BOOLEAN Convert888To888Or565(CLOCK_ALPHA_IMG_T *bk_img_ptr, CLOCK_ALPHA_IMG_T *fore_img_ptr);

/*****************************************************************************/
//  Description : Handle WidgetClock Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandleAlarmJumpClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );*/

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
//LOCAL BOOLEAN DisplayAlmTime(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect);



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*WINDOW_TABLE( MMIALM_JUMP_CLOCK_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID( MMIALM_JUMP_CLOCK_WIN_ID ),
  //  WIN_TITLE( TXT_ALARM_CLOCK ),
    WIN_FUNC((uint32)HandleAlarmJumpClockWinMsg ),  

    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_CANCEL ),
     END_WIN
};*/
/*****************************************************************************/
//  Description : Handle WidgetClock Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandleAlarmJumpClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint16      width = 0; 
    uint16      height = 0;  
    GUI_POINT_T point;
    GUI_RECT_T   win_rect ={0};
    SCI_TIME_T  sys_time ={0};
    SCI_DATE_T  sys_date = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:       
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);//½ûÖ¹ºáÆÁ
        s_clk_timer_id = MMK_CreateWinTimer(win_id, 100, TRUE);
        break;
    case MSG_GET_FOCUS:
        break;

    case MSG_BACKLIGHT_TURN_ON:
        break;
        
    case MSG_TIMER:
        if (s_clk_timer_id == *((uint8 *) param))
        {
            s_pic_index =(s_pic_index+1)%CLK_ROTATE_PIC_NUM;
            
        }
        MMK_UpdateScreen();
        break;

        
    case MSG_LOSE_FOCUS:
        break;
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if (PNULL != param)
        {
            uint32 src_id = ((MMI_NOTIFY_T*)param)->src_id;
            
            switch (src_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_OK,PNULL);
                break;
            case MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID:
                break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                MMK_SendMsg(win_id, MSG_APP_CANCEL,PNULL);
                break;           
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:

        break;
        
    case MSG_FULL_PAINT:
        {
            GUI_POINT_T point={0};
            MMK_GetWinRect(win_id, &win_rect);

            TM_GetSysTime(&sys_time);

            TM_GetSysDate(&sys_date);
   
            GUIRES_DisplayImg(PNULL,
		        &win_rect,
		        &win_rect,
		        win_id,
		        IMG_JUMP_WIN_BG,
		        &lcd_dev_info);
            MMIAPIALARM_DisplayAlmClock(win_id, sys_time, win_rect, s_rotate_info_arr[s_pic_index].angel);
            point.x = CLK_TIME_X;
            point.y = CLK_TIME_Y;
            MMIAPIALARM_DrawTime(sys_time, point);
            point.x = CLK_DATE_X;
            point.y = CLK_DATE_Y;
            MMIAPIALARM_DrawDate(sys_date, point); 
        }
        break;    
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;     
        		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}*/

/*****************************************************************************/
//  Description : alpha blending RGB565 data quickly
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL __inline uint16 BlendRGB565(uint16 bk_color, uint16 fore_color, uint8 alpha)
{
    uint32 blend_color = 0;
    uint32 temp_back_color = 0;
    uint32 temp_fore_color = 0;
    
    alpha = (alpha + 1) >> 3;

    temp_fore_color = ( fore_color | (fore_color << 16)) & BLEND_MASK;
    temp_back_color = ( bk_color | (bk_color << 16)) & BLEND_MASK;
    
    blend_color = ((((temp_fore_color - temp_back_color) * alpha ) >> 5 ) + temp_back_color) & BLEND_MASK;
    blend_color = ((blend_color & 0xFFFF) | (blend_color >> 16));
    
    return (uint16)blend_color;
}

/*****************************************************************************/
//  Description : Alpha blend two images
//  Global resource dependence : 
//  Author:
//  Note: the output image is in the background image buffer, overwritting the orignial data.
//		  the type of the output image data is the same as that of the background image
/*****************************************************************************/
LOCAL BOOLEAN ProcessAlphaBlending(CLOCK_ALPHA_IMG_T *bk_img_ptr, CLOCK_ALPHA_IMG_T *fore_img_ptr)
{
    uint32 fore_width  = fore_img_ptr->width;
    uint32 fore_height = fore_img_ptr->height;
    uint32 bk_width = bk_img_ptr->width;
    uint32 bk_height = bk_img_ptr->height;
    int32 start_x = fore_img_ptr->start_x;
    int32 start_y = fore_img_ptr->start_y;
    uint32 blend_width = fore_width;
    uint32 blend_height = fore_height;
    uint32 *fore_ptr = (uint32 *)fore_img_ptr->data_ptr; 
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T bk_rect = {0};
    GUI_RECT_T for_rect = {0};    

    bk_rect.left  = bk_img_ptr->start_x;
    bk_rect.top   = bk_img_ptr->start_y;
    bk_rect.right = bk_img_ptr->start_x + bk_img_ptr->width;
    bk_rect.bottom  = bk_img_ptr->start_y + bk_img_ptr->height;    

    for_rect.left  = fore_img_ptr->start_x;
    for_rect.top   = fore_img_ptr->start_y;
    for_rect.right = fore_img_ptr->start_x + fore_img_ptr->width;
    for_rect.bottom  = fore_img_ptr->start_y + fore_img_ptr->height;    

    if (!GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        return FALSE;
    }


    if (start_y < 0)
    {
        start_y = -start_y;
        blend_height -= start_y;        /*lint !e737*/
        fore_ptr += (start_y * fore_width);     /*lint !e737*/
        start_y = 0;
    }

    if (start_x < 0)
    {
        start_x = -start_x;
        blend_width -= start_x;     /*lint !e737*/
        fore_ptr += start_x;
        start_x = 0;
    }

    //SCI_PASSERT((int32)bk_width >= start_x, ("bk_width=%d, start_x=%d", bk_width, start_x));
    //SCI_PASSERT((int32)bk_height >= start_y, ("bk_height=%d, start_y=%d", bk_height, start_y));

    if ((int32)bk_width < start_x || (int32)bk_height < start_y)
    {
        //SCI_TRACE_LOW:"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_JUMP_CLOCK_331_112_2_18_2_1_18_61,(uint8*)"");
        return FALSE;
    }
    
    blend_width = (start_x + blend_width) > bk_width        /*lint !e737*/
                    ? (bk_width - start_x) : blend_width;       /*lint !e737*/
    blend_height = (start_y + blend_height) > bk_height         /*lint !e737*/
                    ? (bk_height - start_y) : blend_height;     /*lint !e737*/

    if (IMGREF_FORMAT_RGB565 == bk_img_ptr->data_type)     //output RGB565
    {        
        uint32 i = 0, j = 0;
        uint16 *bk_ptr = (uint16 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);    
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }
    else                                //output ARGB888
    {
        uint32 i = 0, j = 0;
        uint32 *bk_ptr = (uint32 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                int32 alpha, bk_color, fore_color;
                uint32 fore_value = *(fore_ptr + j);    
                uint32 bk_value = *(bk_ptr + j);
                uint32 result_value = (bk_value & 0xff000000);

#if 1
                alpha = ARGB_GET_A(bk_value);

                if (0 == alpha) 
                {
                    *(bk_ptr + j) = *(fore_ptr + j);
                    continue;
                }                

                alpha = ARGB_GET_A(fore_value);
                
                //r
                bk_color = ARGB_GET_R(bk_value);
                fore_color = ARGB_GET_R(fore_value);              
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_R(result_value, bk_color);
                
                //g
                bk_color = ARGB_GET_G(bk_value);
                fore_color = ARGB_GET_G(fore_value);              
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_G(result_value, bk_color);

                //b
                bk_color = ARGB_GET_B(bk_value);
                fore_color = ARGB_GET_B(fore_value);              
                bk_color = bk_color + (((fore_color - bk_color) * alpha) >> 8);
                result_value = ARGB_SET_B(result_value, bk_color);
#else
                result_value = GUI_CompoundARGB888(bk_value, fore_value);
#endif
                *(bk_ptr + j) = result_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }  

    return TRUE;
}

/*****************************************************************************/
//  Description : Alpha blend two images
//  Global resource dependence : 
//  Author:
//  Note: the output image is in the background image buffer, overwritting the orignial data.
//		  the type of the output image data is the same as that of the background image
/*****************************************************************************/
LOCAL BOOLEAN Convert888To888Or565(CLOCK_ALPHA_IMG_T *bk_img_ptr, CLOCK_ALPHA_IMG_T *fore_img_ptr)
{
    uint32 fore_width  = fore_img_ptr->width;
    uint32 fore_height = fore_img_ptr->height;
    uint32 bk_width = bk_img_ptr->width;
    uint32 bk_height = bk_img_ptr->height;
    int32 start_x = fore_img_ptr->start_x;
    int32 start_y = fore_img_ptr->start_y;
    uint32 blend_width = fore_width;
    uint32 blend_height = fore_height;
    uint32 *fore_ptr = (uint32 *)fore_img_ptr->data_ptr;
    GUI_RECT_T dst_rect = {0};
    GUI_RECT_T bk_rect = {0};
    GUI_RECT_T for_rect = {0};    

    bk_rect.left  = bk_img_ptr->start_x;
    bk_rect.top   = bk_img_ptr->start_y;
    bk_rect.right = bk_img_ptr->start_x + bk_img_ptr->width;
    bk_rect.bottom  = bk_img_ptr->start_y + bk_img_ptr->height;    

    for_rect.left  = fore_img_ptr->start_x;
    for_rect.top   = fore_img_ptr->start_y;
    for_rect.right = fore_img_ptr->start_x + fore_img_ptr->width;
    for_rect.bottom  = fore_img_ptr->start_y + fore_img_ptr->height;    

    if (!GUI_IntersectRect(&dst_rect, bk_rect, for_rect))
    {
        return FALSE;
    }

    if (start_y < 0)
    {
        start_y = -start_y;
        blend_height -= start_y;        /*lint !e737*/
        fore_ptr += (start_y * fore_width);     /*lint !e737*/
        start_y = 0;
    }

    if (start_x < 0)
    {
        start_x = -start_x;
        blend_width -= start_x;     /*lint !e737*/
        fore_ptr += start_x;
        start_x = 0;
    }

    //SCI_PASSERT((int32)bk_width >= start_x, ("bk_width=%d, start_x=%d", bk_width, start_x));
    //SCI_PASSERT((int32)bk_height >= start_y, ("bk_height=%d, start_y=%d", bk_height, start_y));

    if ((int32)bk_width < start_x || (int32)bk_height < start_y)
    {
        //SCI_TRACE_LOW:"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_JUMP_CLOCK_483_112_2_18_2_1_18_62,(uint8*)"");
        return FALSE;
    }
    
    blend_width = (start_x + blend_width) > bk_width        /*lint !e737*/
                    ? (bk_width - start_x) : blend_width;   /*lint !e737*/
    blend_height = (start_y + blend_height) > bk_height     /*lint !e737*/
                    ? (bk_height - start_y) : blend_height; /*lint !e737*/

    if (IMGREF_FORMAT_RGB565 == bk_img_ptr->data_type)     //output RGB565
    {        
        uint32 i = 0, j = 0;
        uint16 *bk_ptr = (uint16 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);   /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                uint32 alpha, r, g, b;
                uint32 fore_value = *(fore_ptr + j);    
                uint16 bk_value = *(bk_ptr + j);

                alpha = ARGB_GET_A(fore_value);
                r = ARGB_GET_R(fore_value);
                g = ARGB_GET_G(fore_value);
                b = ARGB_GET_B(fore_value);

                if (0 == alpha)
                {
                    continue;
                }

                fore_value = RGB888_TO_RGB565(r, g, b);
                bk_value = BlendRGB565(bk_value, (uint16)fore_value, alpha);
                *(bk_ptr + j) = bk_value;
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }
    else                                //output ARGB888
    {
        uint32 i = 0, j = 0;
        uint32 *bk_ptr = (uint32 *)bk_img_ptr->data_ptr;

        bk_ptr += (start_y * bk_width + start_x);   /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                *(bk_ptr + j) = *(fore_ptr + j);    
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }   

    return TRUE;
}

/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DrawTime(SCI_TIME_T sys_time, GUI_POINT_T point)
{
    uint8               time_str[MMISET_TIME_STR_12HOURS_LEN + 1]  = {0};
    wchar               time_wstr[MMISET_TIME_STR_12HOURS_LEN + 1]    = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T string = {0};
    GUI_RECT_T rect={0};
    uint16 logic_lcd_width=0;
    
    str_style.align = ALIGN_HVMIDDLE;
    str_style.font  = SONG_FONT_50;
    str_style.font_color = MMI_WHITE_COLOR;
   
    
    //sprintf((char *)time_str," %02d:%02d ",sys_time.hour,sys_time.min);
    MMIAPISET_FormatTimeStrByTime(sys_time.hour,sys_time.min,(uint8*)time_str,MMISET_TIME_STR_12HOURS_LEN + 1);
    MMIAPICOM_StrToWstr(time_str, time_wstr); 
	string.wstr_ptr = time_wstr;
	string.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
    
    //GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &string, &str_style, state);
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, PNULL);
    rect.top=point.y;
    rect.left=0;
    rect.right=logic_lcd_width-1;
    rect.bottom=rect.top+GUIFONT_GetHeight(str_style.font);
    GUISTR_DrawTextToLCDInRect( 
            &lcd_dev_info,
            &rect,      
            &rect,       
            (const MMI_STRING_T    *)&string,
            &str_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
}

/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_DrawDate(SCI_DATE_T sys_date, GUI_POINT_T point)
{
    uint8               date_str[MMISET_DATE_STR_LEN]  = {0};
    wchar               date_wstr[MMISET_DATE_STR_LEN]    = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    MMI_STRING_T string = {0};
    GUI_RECT_T rect={0};
    uint16 logic_lcd_width=0;

    str_style.align = ALIGN_HVMIDDLE;
    str_style.font  = SONG_FONT_18;
    str_style.font_color = MMI_WHITE_COLOR;
   
    
    //sprintf((char *)date_str," %04d-%02d-%2d",sys_date.year, sys_date.mon, sys_date.mday);
    MMIAPISET_FormatDateStrByDateStyle(sys_date.year, sys_date.mon, sys_date.mday,'.',date_str,MMISET_DATE_STR_LEN);
    MMIAPICOM_StrToWstr(date_str, date_wstr); 
	string.wstr_ptr = date_wstr;
	string.wstr_len = MMIAPICOM_Wstrlen(date_wstr);
    
    //GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &string, &str_style, state);
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &logic_lcd_width, PNULL);
    rect.top=point.y;
    rect.left=0;
    rect.right=logic_lcd_width-1;
    rect.bottom=rect.top+GUIFONT_GetHeight(str_style.font);
    GUISTR_DrawTextToLCDInRect( 
            &lcd_dev_info,
            &rect,      
            &rect,       
            &string,
            &str_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
}

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIALARM_DisplayAlmClock(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect, int32 angel)
{
    //GUI_RECT_T        lcd_rect = {0};
    GUI_RECT_T        clock_icon_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_RECT_T        img_rect = {0};
    GUI_POINT_T       point = {0};  
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    CLOCK_ALPHA_IMG_T bk_img = {0};
    CLOCK_ALPHA_IMG_T fore_img = {0};
    UILAYER_INFO_T    layer_info = {0};
    uint16            width  = 0;
    uint16            height = 0;
    //uint32            block_id = GUI_BLOCK_MAIN;

    ROTATE_ARBITRARY_IN_T	rot_in		= {0};
    ROTATE_ARBITRARY_OUT_T	rot_out		= {0};
    uint8*              rotated_buf_ptr         = PNULL;
    uint32              rotated_buf_size       = 0;
    //uint8*              img_ptr         = PNULL;
    //uint32              img_size       = 0;

	//aclock dial.
	aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
	GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMG_JUMP_CLK_BG, win_id);
	aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_JUMP_CLK_BG,win_id,&(aclock_input_param.clock_dial.data_size));
	aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
	aclock_input_param.clock_dial.center_y = CLK_CENTER_X; 
    
    win_rect.left = win_rect.left + (win_rect.right - win_rect.left - aclock_input_param.clock_dial.width)/2;//0;
    win_rect.top = CLK_ALARM_HEIGHT;//win_rect.top + (win_rect.bottom - win_rect.top - aclock_input_param.clock_dial.height)/2;
    win_rect.right = win_rect.right-win_rect.left;
    win_rect.bottom = win_rect.top + aclock_input_param.clock_dial.height; 
	
//aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, IMG_JUMP_CLK_HOUR_HANDLE, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_JUMP_CLK_HOUR_HANDLE,win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, IMG_JUMP_CLK_MIN_HANDLE, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_JUMP_CLK_MIN_HANDLE,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 



    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        return FALSE;
    }

    //get system time
    //TM_GetSysTime(&sys_time);  

    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;

#if defined(ANALOG_CLK_SUPPORT)
    GRAPH_DrawClock(&aclock_input_param, &aclock_output_param);
#endif
    rotated_buf_size = pow((aclock_input_param.clock_dial.width + aclock_input_param.clock_dial.height))* sizeof(uint32);/*lint !e737*/
    rotated_buf_ptr = SCI_ALLOCA((rotated_buf_size) * sizeof(uint8));
    if(PNULL == rotated_buf_ptr)
    {
        SCI_FREE(aclock_input_param.target_buf_ptr);
        return FALSE;
    }    
    rot_in.angle = angel;
    rot_in.src_format = IMGREF_FORMAT_ARGB888;
    rot_in.src_size.w = aclock_input_param.clock_dial.width;
    rot_in.src_size.h = aclock_input_param.clock_dial.height;
    rot_in.src_center.x = s_rotate_info_arr[s_pic_index].rotate_point.x;//aclock_input_param.clock_dial.center_x;
    rot_in.src_center.y = s_rotate_info_arr[s_pic_index].rotate_point.y;//aclock_input_param.clock_dial.center_y;
    rot_in.src_chn.chn0.ptr = (void *)aclock_output_param.output_ptr; 
    rot_in.src_chn.chn0.size = aclock_input_param.target_buf_size;

    rot_in.target_format = IMGREF_FORMAT_ARGB888;
    rot_in.target_size.w = aclock_input_param.clock_dial.width + aclock_input_param.clock_dial.height;
    rot_in.target_size.h = aclock_input_param.clock_dial.width + aclock_input_param.clock_dial.height;
    rot_in.target_chn.chn0.ptr = (void *)rotated_buf_ptr;
    rot_in.target_chn.chn0.size = rotated_buf_size;
    rot_in.target_center.x = s_rotate_info_arr[s_pic_index].rotate_point.x;//aclock_input_param.clock_dial.center_x;//(aclock_input_param.clock_dial.width + aclock_input_param.clock_dial.height)/2;
    rot_in.target_center.y = s_rotate_info_arr[s_pic_index].rotate_point.y;//aclock_input_param.clock_dial.center_y;//(aclock_input_param.clock_dial.width + aclock_input_param.clock_dial.height)/2;
    
    rot_in.mode = ROTATE_ARBITRARY_NO_CROP;

    SCI_MEMSET(rotated_buf_ptr, 0, rotated_buf_size);
    GRAPH_RotateArbitrary(&rot_in, &rot_out);




    img_rect.left = 0;//(win_rect.right - aclock_input_param.clock_dial.width)/2;//0;
    img_rect.top = 0;//(win_rect.bottom - aclock_input_param.clock_dial.height)/2;
    img_rect.right = win_rect.right+1-win_rect.left;
    img_rect.bottom = win_rect.bottom+1-win_rect.top;       
    point.x = win_rect.left;
    point.y = win_rect.top;

    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    //blending handle image
    if (DATA_TYPE_ARGB888 == UILAYER_GetLayerColorType(&lcd_dev_info))
    {
        bk_img.data_type = IMGREF_FORMAT_ARGB888;
    }
    else
    {
        bk_img.data_type = IMGREF_FORMAT_RGB565;
    }


    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);

    if (PNULL == bk_img.data_ptr)
    {
        SCI_FREE(rotated_buf_ptr);  
        SCI_FREE(aclock_input_param.target_buf_ptr);     
        return FALSE;
    }
    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;

   /* GUIRES_GetImgWidthHeight(&width, &height, IMG_JUMP_WIN_BG, win_id); 
    bk_img.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_JUMP_WIN_BG,win_id,&img_size);
    bk_img.data_type = IMGREF_FORMAT_RGB565;
    bk_img.width = width;
    bk_img.height = height;*/

    
   /* fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;*/

    fore_img.data_ptr = rot_out.output_chn.chn0.ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = rot_out.output_size.w;
    fore_img.height = rot_out.output_size.h;
    fore_img.start_x = rot_out.output_offset.x;
    fore_img.start_y = rot_out.output_offset.y;


    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, &lcd_dev_info, &win_rect, &clock_icon_rect);
    fore_img.start_x = clock_icon_rect.left;   
    fore_img.start_y = clock_icon_rect.top; 

    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        SCI_FREE(rotated_buf_ptr);  
        SCI_FREE(aclock_input_param.target_buf_ptr); 
        return FALSE;             
    }

    
    //in the end, display the center point.
  /*  GUIRES_GetImgWidthHeight(&width, &height, IMG_JUMP_CLK_CENTER, win_id);    
    point.x = s_rotate_info_arr[s_pic_index].center_point.x;//win_rect.left + (aclock_input_param.clock_dial.width - width)/2+1;
    point.y = s_rotate_info_arr[s_pic_index].center_point.y;//win_rect.top + CLK_CENTER_X-(height/2);// (aclock_input_param.clock_dial.height - height)/2;
    
    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMG_JUMP_CLK_CENTER,
                      &lcd_dev_info);*/

    //MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);
        
    SCI_FREE(aclock_input_param.target_buf_ptr);
	
    if(PNULL != rotated_buf_ptr) /*lint !e774*/
    {
        SCI_FREE(rotated_buf_ptr);
        rotated_buf_ptr = PNULL;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : math function: sqrt.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL double Sqrt(double b)    
{    
    double x = 1;
    int step = 0;    

    while((x*x-b<-0.000000000000001 || x*x-b>0.000000000000001) && step<50)
    {
        x = (b/x + x)/2.0;
        step++;
    }
    
    return x;    
}

/*****************************************************************************/
//  Description : math function: b*b.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 pow(int32 b)
{
    return (b*b);
}



/*****************************************************************************/
//  Description : create clock window.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
/*PUBLIC void MMIALARM_OpenAlmJumpClockWin(SCI_TIME_T time, alarmClkSaveTimeFunc savetimefunc)
{

    MMK_CreateWin((uint32*)MMIALM_JUMP_CLOCK_TAB, PNULL);
}*/

#endif
#endif											   


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
