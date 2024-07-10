/*****************************************************************************
** File Name:      mmiwidget_calendar.c                                        *
** Author:                                                                   *
** Date:           07/01/2010                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 
******************************************************************************/

#define _MMIWIDGET_NETWORK_NAME_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_NETWORK
#include "window_parse.h"
#include "mmi_modu_main.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget.h"
#include "mmiwidget_id.h"
#include "mmiwidget_image.h"
#include "guires.h"
#include "mmiset_text.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmiwidget_position.h"
#include "mmiidle_export.h"
#if defined QBTHEME_SUPPORT
#include "Mmiqbtheme_idlewin.h"
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
#include "mmiwidget_internal.h"
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIWIDGET_NETWORK_NAME_MARGIN            2

#ifdef MMI_GRID_IDLE_SUPPORT
#if defined (MAINLCD_SIZE_240X400)
#define MMIWIDGET_NETWORK_NUM_MARGIN_X            10
#define MMIWIDGET_NETWORK_NUM_MARGIN_Y            19
#define MMIWIDGET_NETWORK_STR_MARGIN_X            10
#define MMIWIDGET_NETWORK_STR_MARGIN_Y            10
#elif defined(MAINLCD_SIZE_240X320)
#define MMIWIDGET_NETWORK_NUM_MARGIN_X            10
#define MMIWIDGET_NETWORK_NUM_MARGIN_Y            19
#define MMIWIDGET_NETWORK_STR_MARGIN_X            10
#define MMIWIDGET_NETWORK_STR_MARGIN_Y            10
#elif defined(MAINLCD_SIZE_320X480)
#define MMIWIDGET_NETWORK_NUM_MARGIN_X            14
#define MMIWIDGET_NETWORK_NUM_MARGIN_Y            27
#define MMIWIDGET_NETWORK_STR_MARGIN_X            14
#define MMIWIDGET_NETWORK_STR_MARGIN_Y            20
#else
#define MMIWIDGET_NETWORK_NUM_MARGIN_X            14
#define MMIWIDGET_NETWORK_NUM_MARGIN_Y            27
#define MMIWIDGET_NETWORK_STR_MARGIN_X            14
#define MMIWIDGET_NETWORK_STR_MARGIN_Y            10
#endif
#endif

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetNetWorkWinMsg(
											  MMI_WIN_ID_T        win_id,        //IN:
											  MMI_MESSAGE_ID_E    msg_id,        //IN:
											  DPARAM            param        //IN:
											  );
/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Network_DrawNetWorkWidget( MMI_WIN_ID_T  win_id );

/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Network_GetNetWorkTextStyle(GUISTR_STYLE_T * style_ptr );
/*****************************************************************************/
// Description :
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_network_widget =
{
        MMIWIDGET_NETWORK_NAME_ID, 
		HandleWidgetNetWorkWinMsg, 
		PNULL,
		PNULL,
		PNULL,		
		WIDGET_NETWORK_WIN_ID, 
		IMAGE_WIDGET_NETWORK_NAME_SHORTCUT, 

		IMAGE_WIDGET_NETWORK_BG,

        TXT_WIDGET_NETWORK,
		50,//    50, 
		50,//    50
		FALSE,
};


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetNetWorkWinMsg(
											  MMI_WIN_ID_T        win_id,        //IN:
											  MMI_MESSAGE_ID_E    msg_id,        //IN:
											  DPARAM            param        //IN:
											  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;
        
    case MSG_IDLE_UPDATE_NETWORK:
    {
        BOOLEAN is_forbidden_paint = FALSE;
#if defined QBTHEME_SUPPORT
        if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
        {
            is_forbidden_paint = MMIQBIDLE_IsForbidPaint(MMIWIDGET_MP3_ID);                     
        }
#endif
        
#ifdef MMI_GRID_IDLE_SUPPORT
        if (MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle()) 
        {
            is_forbidden_paint = MMIGRID_IsForbidPaint(MMIWIDGET_MP3_ID);
        }
#endif
        
        if(is_forbidden_paint)
        {
            break;
        }  
    }
		MMIWIDGET_Network_DrawNetWorkWidget(win_id);
        break;

    case MSG_FULL_PAINT:
		MMIWIDGET_Network_DrawNetWorkWidget(win_id);
        break;

    case MSG_APP_WEB:
#ifdef MMI_MULTI_SIM_SYS_SINGLE
        MMIAPIIDLE_OpenSelNetWorkWin(MN_DUAL_SYS_1);
#else
        MMIAPISET_OpenMultiSimSettingsWindow(); 
#endif
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
       recode = HandleTpPressUp(win_id, param);        
        break;		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

/*****************************************************************************/
//     Description : get network text  diasplay style 
//    Global resource dependence : 
//  Author:
//    Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Network_GetNetWorkTextStyle(GUISTR_STYLE_T * style_ptr )
{
	
	//SCI_ASSERT(PNULL != style_ptr);

	if (PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_Network_GetNetWorkTextStyle PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_NETWORK_NAME_205_112_2_18_3_8_21_219,(uint8*)"");
        return;
    }
	
	style_ptr->effect 		= FONT_EFFECT_REVERSE_EDGE; 
	style_ptr->angle 		= ANGLE_0;
	style_ptr->align 		= ALIGN_HVMIDDLE;
	style_ptr->font 		= SONG_FONT_16;

#ifdef MMIWIDGET_SUPPORT
	style_ptr->font_color 	= MMITHEME_GetDefaultWidgetThemeColor();
#else
	style_ptr->font_color 	= MMI_BLACK_COLOR;
#endif
	
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
LOCAL void MMIWIDGET_Network_DrawNetWorkWidget(
											   MMI_WIN_ID_T  win_id
											   )
{
    GUI_RECT_T 			    win_rect = {0};
    GUI_RECT_T 			    layer_rect = {0};
    //UILAYER_HANDLE_T 	layer_handle = 0;
    MMI_STRING_T         network_str = {0};
    GUI_POINT_T 			   point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    BOOLEAN           is_fly_mode_on = FALSE;
    uint32                         i = 0;
    GUISTR_STYLE_T 		  text_style = {0};/*lint !e64*/
    GUISTR_STATE_T        text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_NO_HALF_LINE;
    wchar 					opn_spn_name[MMIPHONE_MAX_OPER_NAME_LEN] = {0};
    GUISTR_INFO_T	    tmp_str_info ={0};
    wchar       *sim_deactivated_str = PNULL;
    //GUI_RECT_T  fullscreen_rect = MMITHEME_GetFullScreenRect();
    //GUI_RECT_T 	rect = {0};
	GUI_RECT_T SIM_rect[4] = {
		MMIWIDGET_NETWORK_WIDGET_SIM1_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM2_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM3_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM4_RECT
	};
    MMI_IMAGE_ID_T number_Image_index[2] = {
		IMAGE_WIDGET_NETWORK_YELLOW,
			IMAGE_WIDGET_NETWORK_GRAY
	};

	MMI_IMAGE_ID_T number_Image_index_num[4] = {
		IMAGE_WIDGET_NETWORK_NUM_1,
			IMAGE_WIDGET_NETWORK_NUM_2,
			IMAGE_WIDGET_NETWORK_NUM_3,
			IMAGE_WIDGET_NETWORK_NUM_4
	};
	uint16 image_width =0;
    uint16 image_height =0;	
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

	if(!UILAYER_IsLayerActive(&lcd_dev_info))
    {
        return;
    }
    layer_rect = UILAYER_GetLayerRect(&lcd_dev_info);
    if(!GUI_IntersectRect(&win_rect, layer_rect, win_rect))
    {
        return;
    }

        MMIWIDGET_Network_GetNetWorkTextStyle(&text_style);
        // 读取设置模式的值
        is_fly_mode_on = MMIAPISET_GetFlyMode();
     
        win_rect.left += MMIWIDGET_NETWORK_NAME_MARGIN;
        win_rect.right -= MMIWIDGET_NETWORK_NAME_MARGIN;
        //win_rect.top += MMIWIDGET_NETWORK_TOP_SPACE;

        
        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(is_fly_mode_on)
            {
                MMI_GetLabelTextByLang(TXT_SET_FLY_MODE, &network_str);
            }
            else if (MMIAPIPHONE_GetSimExistedStatus(i))
            {
                if (MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting((MN_DUAL_SYS_E)i))
                {
                    SCI_MEMSET(&network_str, 0x00, sizeof(network_str));
                    network_str.wstr_ptr = (wchar *)opn_spn_name;
                    SCI_MEMSET(network_str.wstr_ptr, 0x00, sizeof(opn_spn_name));
                    
                    MMIAPIPHONE_GetSimAndServiceString((MN_DUAL_SYS_E)i, &network_str);
                }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                else
                {
                    MMISET_SIM_NAME_T sim_name = {0};
                    MMI_STRING_T sim_deactivated = {0};

                    sim_name = MMIAPISET_GetSimName(i);

                    MMI_GetLabelTextByLang(TXT_SIM_DEACTIVATED, &sim_deactivated);

                    //此处不判断是否需要加入空格，统一都多分配一个空格的存储空间(2bytes)    
                    sim_deactivated_str = (wchar *)SCI_ALLOC_APP(((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                    SCI_MEMSET(sim_deactivated_str, 0x00, ((sim_deactivated.wstr_len + sim_name.wstr_len + 2)*sizeof(wchar)));
                                        
                    MMI_WSTRNCPY(sim_deactivated_str,
                                                (sim_deactivated.wstr_len + sim_name.wstr_len + 1),
                                                sim_name.wstr_arr,
                                                sim_name.wstr_len,
                                                sim_name.wstr_len);
                    
                    // ASCII 码数据，两个字符串中间追加一个空格。
                    if (MMIAPICOM_IsASCIIString(sim_name.wstr_arr,sim_name.wstr_len)
                        && MMIAPICOM_IsASCIIString(sim_deactivated.wstr_ptr ,sim_name.wstr_len))
                    {
                        MMIAPICOM_Wstrcat(sim_deactivated_str, L" "); 
                        MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len + 1,
                                                (sim_deactivated.wstr_len + 1),
                                                sim_deactivated.wstr_ptr,
                                                sim_deactivated.wstr_len,
                                                sim_deactivated.wstr_len);
                        
                        network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                        network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len + 1 ;
                    }
                    else
                    {
                        MMI_WSTRNCPY(sim_deactivated_str + sim_name.wstr_len ,
                                                (sim_deactivated.wstr_len + 1),
                                                sim_deactivated.wstr_ptr,
                                                sim_deactivated.wstr_len,
                                                sim_deactivated.wstr_len);
                        network_str.wstr_ptr = (wchar *)sim_deactivated_str;
                        network_str.wstr_len = sim_deactivated.wstr_len + sim_name.wstr_len ;
                    }
                }
#endif
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_INSERT_SIM, &network_str);
            }

            GUISTR_GetStringInfo(&text_style, &network_str, text_state,&tmp_str_info);     
            
            
         win_rect = SIM_rect[i];
         win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, SIM_rect[i]);

			point.x = win_rect.left;
			point.y = win_rect.top;
			if (MMIAPIPHONE_GetSimExistedStatus(i))
			{			
				GUIRES_DisplayImg(
					&point,
					PNULL,
					PNULL,
					win_id,
					number_Image_index[0],
					&lcd_dev_info
					);
			}
			else
			{
				GUIRES_DisplayImg(
					&point,
					PNULL,
					PNULL,
					win_id,
					number_Image_index[1],
					&lcd_dev_info
					);
			}


			
			point.x += MMIWIDGET_NETWORK_NUM_MARGIN_X ;
			point.y += MMIWIDGET_NETWORK_NUM_MARGIN_Y ;
			
			GUIRES_DisplayImg(
				&point,
				PNULL,
				PNULL,
				win_id,
				number_Image_index_num[i],
				&lcd_dev_info
				);

			GUIRES_GetImgWidthHeight(&image_width, &image_height, number_Image_index_num[0],  win_id);
			win_rect.left += MMIWIDGET_NETWORK_NUM_MARGIN_X + image_width;
			win_rect.right -= MMIWIDGET_NETWORK_STR_MARGIN_X;
			win_rect.top += MMIWIDGET_NETWORK_STR_MARGIN_Y;
			win_rect.bottom -= MMIWIDGET_NETWORK_STR_MARGIN_Y;

            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                &win_rect,
                &win_rect,
                &network_str,
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO
                );

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (PNULL != sim_deactivated_str)
            {
                SCI_FREE(sim_deactivated_str);
                sim_deactivated_str = PNULL;
            }
#endif
        }   
}

LOCAL MMI_RESULT_E  HandleTpPressUp(
									MMI_WIN_ID_T        win_id,        //IN:
									DPARAM            param        //IN:
									)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    return recode;
#else
    int i = 0;
    BOOLEAN is_hit = FALSE;
    GUI_POINT_T point = {0};	
	GUI_RECT_T SIM_rect[4] = {
		MMIWIDGET_NETWORK_WIDGET_SIM1_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM2_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM3_RECT,
			MMIWIDGET_NETWORK_WIDGET_SIM4_RECT
			};
	GUI_RECT_T win_rect ={0};	
    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        win_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2LCD, win_id, SIM_rect[i]);
        if (GUI_PointIsInRect(point, win_rect))
        {
            is_hit = TRUE;
            break;
        }
    }
    
    if(is_hit)
    {
        // how to deal with when the single sim card case ??
             MMIAPISET_OpenMultiSimSettingsWindow(); 
    	    //MMIAPISET_OpenDualSimSetWin();
    }
    else
    {
    	recode = MMI_RESULT_FALSE;
    }
    return recode;
#endif
}




#endif
#endif

