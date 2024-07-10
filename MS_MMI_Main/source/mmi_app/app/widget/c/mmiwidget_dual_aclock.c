/*****************************************************************************
** File Name:      mmiwidget_aclock.c                                        *
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
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_DUAL_ACLOCK
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
#include "mmi_nv.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwidget_position.h"
#include "graphics_draw.h"
//#include "mmiacc_text.h"
#include "mmiwclk_export.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "guigraph.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef uint32 MMIWIDGET_DUAL_ACLOCK_T;
#define MMIWIDGET_DUAL_ACLOCK_LEFT    0
#define MMIWIDGET_DUAL_ACLOCK_RIGHT   1

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
LOCAL uint8	s_sectimer_id = 0;    /*!<°´ÃëÖÓË¢ÐÂµÄtimer_id */ 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

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
LOCAL MMI_RESULT_E  HandleWidgetDualAClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayDualAClock(
                       MMI_WIN_ID_T win_id
                       );

/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_DualAClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayAClock(MMI_WIN_ID_T win_id, MMIWIDGET_DUAL_ACLOCK_T aclock);

/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTimer(DPARAM param,MMI_WIN_ID_T win_id);
LOCAL void StartSecTimer(MMI_WIN_ID_T win_id);
LOCAL void StopSecTimer(void);

/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetDualAclockWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img);

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/
PUBLIC const MMIWIDGET_ITEM_INFO_T g_dual_aclock_widget =
{
    MMIWIDGET_DUAL_ACLOCK_ID, 
    HandleWidgetDualAClockWinMsg, 
    PNULL, 
    PNULL,
    GetDualAclockWidgetImage,
    WIDGET_DUAL_ACLOCK_WIN_ID, 
    IMAGE_WIDGET_DUAL_ACLOCK_SHORTCUT, 
    IMAGE_WIDGET_DUAL_ACLOCK_BG,
    TXT_WIDGET_DUAL_ACLOCK,
    50,//    75, 
    50,//    75
    FALSE
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get dual aclock widget image.
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/ 
LOCAL void GetDualAclockWidgetImage(MMI_IMAGE_ID_T *bg_img, MMI_IMAGE_ID_T *sc_img)
{
    if (PNULL != bg_img)
    {
        *bg_img = IMAGE_WIDGET_DUAL_ACLOCK_BG;
    }

    if (PNULL != sc_img)    
    {
        *sc_img = IMAGE_WIDGET_DUAL_ACLOCK_SHORTCUT;
    }
    
#ifdef MMI_SAM_THEME	
    if ((MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM) && (PNULL != bg_img))
    {
        *bg_img = IMAGE_WIDGET_SAM_DUAL_ACLOCK_BG;
    }
#endif    
}


/*****************************************************************************/
//  Description : Handle WidgetClock Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetDualAClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
	
    switch (msg_id)
    {
    case MSG_BACKLIGHT_TURN_ON:        
    case MSG_GET_FOCUS:
        StartSecTimer(win_id);
        break;
    
    case MSG_TIMER:
        HandleTimer(param,win_id);
        break;
        
    case MSG_LOSE_FOCUS:
    case MSG_BACKLIGHT_TURN_OFF:
        StopSecTimer();
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
        recode= MMIWIDGET_DualAClock_HandleTpPressUp(win_id,param);
        break;

    case MSG_APP_WEB:
        MMIAPIWCLK_OpenMainWin();
        break;
        
    case MSG_FULL_PAINT:
        DisplayDualAClock(win_id);
        break;
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;        
}

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
        //SCI_TRACE_LOW:"dual aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DUAL_ACLOCK_288_112_2_18_3_8_7_196,(uint8*)"");
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
        uint32 alpha, r, g, b;
        uint32 fore_value = 0;    
        uint16 bk_value = 0;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                fore_value = *(fore_ptr + j);    
                bk_value = *(bk_ptr + j);

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
        uint32 fore_value = 0;    
        uint32 bk_value = 0;
        uint32 result_value = 0;

        bk_ptr += (start_y * bk_width + start_x);       /*lint !e737*/

        for (i=0; i<blend_height; i++)
        {
            for (j=0; j<blend_width; j++)
            {
                fore_value = *(fore_ptr + j);    
                bk_value = *(bk_ptr + j);
                result_value = (bk_value & 0xff000000);
                result_value = GUI_CompoundARGB888(bk_value, fore_value);
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
        //SCI_TRACE_LOW:"dual aclock Convert888To888Or565 (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DUAL_ACLOCK_409_112_2_18_3_8_8_197,(uint8*)"");
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
                int32 alpha, bk_color, fore_color;
                uint32 fore_value = *(fore_ptr + j);    
                uint32 bk_value = *(bk_ptr + j);
                uint32 result_value = (bk_value & 0xff000000);

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
                
                *(bk_ptr + j) = result_value; //*(fore_ptr + j);    
            }

            bk_ptr += bk_width;
            fore_ptr += fore_width;
        }
    }   

    return TRUE;
}

/*****************************************************************************/
//  Description : convert clock time.
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL SCI_TIME_T ConvertSysTime(MCC_WT_CITY_INFO_T *wlk_info)
{
    uint8 hour_char[3]  = {0};
    uint8 min_char[3]   = {0};
    uint8 sec_char[3]   = {0};
    SCI_TIME_T sys_time = {0};

    MMI_MEMCPY(&hour_char, 2, wlk_info->time,    2, 2);
    MMI_MEMCPY(&min_char, 2, wlk_info->time + 3, 2, 2); 
    MMI_MEMCPY(&sec_char, 2, wlk_info->time + 6, 2, 2);

    sys_time.hour = atoi((char*) hour_char);  
    sys_time.min  = atoi((char*) min_char ); 
    sys_time.sec  = atoi((char*) sec_char ); 

    return sys_time;
}


/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayAClock(MMI_WIN_ID_T win_id, MMIWIDGET_DUAL_ACLOCK_T aclock)
{
    GUI_RECT_T        win_rect = {0};
    GUI_RECT_T        clock_icon_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    SCI_TIME_T        sys_time        = {0};
    GUI_POINT_T       point = {0};  
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    CLOCK_ALPHA_IMG_T bk_img = {0};
    CLOCK_ALPHA_IMG_T fore_img = {0};
    UILAYER_INFO_T    layer_info = {0};
    uint16            width  = 0;
    uint16            height = 0;

    MMI_IMAGE_ID_T    aclock_dial   = 0;
    MMI_IMAGE_ID_T    aclock_hour   = 0;
    MMI_IMAGE_ID_T    aclock_min    = 0;
    MMI_IMAGE_ID_T    aclock_sec    = 0;    
    MMI_IMAGE_ID_T    aclock_center = 0;
    MCC_WT_CITY_INFO_T *wlk_city_info = PNULL;
    GUI_POINT_T       left_aclock_pt  = {0};
    GUI_POINT_T       right_aclock_pt = {0};

    if (aclock > MMIWIDGET_DUAL_ACLOCK_RIGHT)
    {
        return;
    }


    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    MMK_GetWinRect(win_id, &win_rect);
    bk_img.data_ptr = UILAYER_GetLayerBufferPtr(&lcd_dev_info);

    if (PNULL == bk_img.data_ptr)
    {
        return;
    }

    if (MMIWIDGET_DUAL_ACLOCK_LEFT == aclock)
    {        
        wlk_city_info = MMIAPIACC_GetLocalTimeZoneInfo();  
        if(0 == wlk_city_info)
        {
            return;
        }
    }
    else if (MMIWIDGET_DUAL_ACLOCK_RIGHT == aclock)
    {
        wlk_city_info = MMIAPIACC_GetOtherTimeZoneInfo(); 
        if(0 == wlk_city_info)
        {
            wlk_city_info = MMIAPIACC_GetLocalTimeZoneInfo();  
        }
        if(0 == wlk_city_info)
        {
            return;
        }
    }

    sys_time = ConvertSysTime(wlk_city_info);  

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        aclock_dial   = IMAGE_WIDGET_SAM_DUAL_ACLOCK1;
        aclock_hour   = IMAGE_WIDGET_SAM_DUAL_ACLOCK_HOUR1;
        aclock_min    = IMAGE_WIDGET_SAM_DUAL_ACLOCK_MIN1;
        aclock_sec    = IMAGE_WIDGET_SAM_DUAL_ACLOCK_SEC1;
        aclock_center = IMAGE_WIDGET_SAM_DUAL_ACLOCK_CENTER1;    
    }
    else
#endif    
    if (6 <= sys_time.hour && 18 > sys_time.hour)
    {
        aclock_dial   = IMAGE_WIDGET_DUAL_ACLOCK1;
        aclock_hour   = IMAGE_WIDGET_DUAL_ACLOCK_HOUR1;
        aclock_min    = IMAGE_WIDGET_DUAL_ACLOCK_MIN1;
        aclock_sec    = IMAGE_WIDGET_DUAL_ACLOCK_SEC1;
        aclock_center = IMAGE_WIDGET_DUAL_ACLOCK_CENTER1;
    }
    else
    {
        aclock_dial   = IMAGE_WIDGET_DUAL_ACLOCK2;
        aclock_hour   = IMAGE_WIDGET_DUAL_ACLOCK_HOUR2;
        aclock_min    = IMAGE_WIDGET_DUAL_ACLOCK_MIN2;
        aclock_sec    = IMAGE_WIDGET_DUAL_ACLOCK_SEC2;        
        aclock_center = IMAGE_WIDGET_DUAL_ACLOCK_CENTER2;
    }
    
    //aclock dial.
    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, aclock_dial, win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(aclock_dial,win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2;   

    //aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, aclock_hour, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(aclock_hour,win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, aclock_min, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(aclock_min,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

    //aclock sec hand
    aclock_input_param.sec_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.sec_handle.width, &aclock_input_param.sec_handle.height, aclock_sec, win_id);        
    aclock_input_param.sec_handle.data_ptr = (uint8 *)MMI_GetLabelImage(aclock_sec,win_id,&(aclock_input_param.sec_handle.data_size));
    aclock_input_param.sec_handle.center_x = aclock_input_param.sec_handle.width/2;
    aclock_input_param.sec_handle.center_y = aclock_input_param.sec_handle.height/2; 

    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        return;
    }
   
    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;

#if defined(ANALOG_CLK_SUPPORT)
    SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLOCK DisplayAClock GRAPH_DrawClock start == %d", SCI_GetTickCount());
    if(0 != GRAPH_DrawClock(&aclock_input_param, &aclock_output_param))
    {
        //SCI_TRACE_LOW:"MMIWIDGET_DUAL_ACLOCK DisplayAClock GRAPH_DrawClock error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DUAL_ACLOCK_639_112_2_18_3_8_8_198,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);     
        return;
    }
    SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLOCK DisplayAClock GRAPH_DrawClock end == %d", SCI_GetTickCount());
#else
        return;
#endif

#if 0

        {
             FILE *fp = fopen("d:\\aclock.raw", "w+b");

			 fwrite(aclock_output_param.output_ptr, 1, aclock_input_param.target_buf_size, fp);

			 fclose(fp);
		}
#endif

   
    UILAYER_GetLayerInfo(&lcd_dev_info, &layer_info);
    
    //blending handle image
    
    if (DATA_TYPE_ARGB888 == layer_info.data_type)
    {
        bk_img.data_type = IMGREF_FORMAT_ARGB888;
    }
    else
    {
        bk_img.data_type = IMGREF_FORMAT_RGB565;
    }
    bk_img.width = layer_info.layer_width;
    bk_img.height = layer_info.layer_height;
    
    fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;


        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);

        LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, &lcd_dev_info, &win_rect, &clock_icon_rect); 

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        left_aclock_pt.x  = MMIWIDGET_SAM_LACLOCK_RECT_LEFT;
        left_aclock_pt.y  = MMIWIDGET_SAM_LACLOCK_RECT_TOP;        
        right_aclock_pt.x = MMIWIDGET_SAM_RACLOCK_RECT_LEFT;
        right_aclock_pt.y = MMIWIDGET_SAM_RACLOCK_RECT_TOP;
    }
    else
#endif
    {
        left_aclock_pt.x  = MMIWIDGET_LACLOCK_RECT_LEFT;
        left_aclock_pt.y  = MMIWIDGET_LACLOCK_RECT_TOP;        
        right_aclock_pt.x = MMIWIDGET_RACLOCK_RECT_LEFT;
        right_aclock_pt.y = MMIWIDGET_RACLOCK_RECT_TOP;    
    }
    
    if (MMIWIDGET_DUAL_ACLOCK_LEFT == aclock)        
    {
        fore_img.start_x = clock_icon_rect.left + left_aclock_pt.x;   
        fore_img.start_y = clock_icon_rect.top + left_aclock_pt.y;         
    }
    else
    if (MMIWIDGET_DUAL_ACLOCK_RIGHT == aclock)        
    {
        fore_img.start_x = clock_icon_rect.left + right_aclock_pt.x;   
        fore_img.start_y = clock_icon_rect.top + right_aclock_pt.y;         
    }

    SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLOCK DisplayAClock ProcessAlphaBlending start == %d", SCI_GetTickCount());
    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        SCI_FREE(aclock_input_param.target_buf_ptr); 
        SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLOCK DisplayAClock ProcessAlphaBlending error");
        return;             
    }
    SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLOCK DisplayAClock ProcessAlphaBlending end == %d", SCI_GetTickCount());
    
    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, aclock_center, win_id);

    if (MMIWIDGET_DUAL_ACLOCK_LEFT == aclock)        
    {
        point.x = win_rect.left + (aclock_input_param.clock_dial.width - width)/2 + left_aclock_pt.x;
        point.y = win_rect.top + (aclock_input_param.clock_dial.height - height)/2 + left_aclock_pt.y;           
    }
    else
    if (MMIWIDGET_DUAL_ACLOCK_RIGHT == aclock)        
    {
        point.x = win_rect.left + (aclock_input_param.clock_dial.width - width)/2 + right_aclock_pt.x;
        point.y = win_rect.top + (aclock_input_param.clock_dial.height - height)/2 + right_aclock_pt.y;         
    }     
 
    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      aclock_center,
                      &lcd_dev_info);

    SCI_FREE(aclock_input_param.target_buf_ptr);

}

/*****************************************************************************/
//  Description : display dual aclock background
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDualAClockBg(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_RECT_T img_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    MMI_IMAGE_ID_T    bg_img = IMAGE_WIDGET_DUAL_ACLOCK_BG;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)   
    {
        bg_img = IMAGE_WIDGET_SAM_DUAL_ACLOCK_BG;
    }
#endif
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    img_rect.left = 0;
    img_rect.top = 0;
    img_rect.right = win_rect.right+1-win_rect.left;
    img_rect.bottom = win_rect.bottom+1-win_rect.top;        

    GUIRES_DisplayImg(
        PNULL,
        &win_rect,
        &img_rect,
        win_id,
        bg_img,
        &lcd_dev_info
        );
}

/*****************************************************************************/
//  Description : get dual aclock text style.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDualAClockWidgetTextStyle(GUISTR_STYLE_T *city_style_ptr)
{
    MMITHEME_WIDGET_CLOCK_T clock_widget_theme = {0};

    MMITHEME_GetADualClockWidgetTheme(&clock_widget_theme);    

    //city
    if (PNULL != city_style_ptr)
    {
        city_style_ptr->effect = FONT_EFFECT_NONE; 
        city_style_ptr->angle = ANGLE_0;
        city_style_ptr->align = ALIGN_HVMIDDLE;
        city_style_ptr->font = clock_widget_theme.city_font;
        city_style_ptr->font_color = clock_widget_theme.city_font_color;
    }
}

/*****************************************************************************/
//  Description : display dual aclock background
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDualAClockCity(MMI_WIN_ID_T win_id)
{
    GUISTR_INFO_T	  str_info={0};
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    // GUISTR_STYLE_T    time_text_style = {0};
    GUISTR_STYLE_T    city_text_style = {0};
    MMI_STRING_T      city_str1 = {0};
    MMI_STRING_T      city_str2 = {0};    
    GUI_RECT_T        city1_rect = MMIWIDGET_LACLOCK_CITY_RECT;
    GUI_RECT_T        city2_rect = MMIWIDGET_RACLOCK_CITY_RECT; 
    GUISTR_STATE_T    text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    MMI_TEXT_ID_T     text_id = 0;

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        city1_rect.left   = MMIWIDGET_SAM_LACLOCK_CITY_RECT_LEFT;
        city1_rect.top    = MMIWIDGET_SAM_LACLOCK_CITY_RECT_TOP;
        city1_rect.right  = MMIWIDGET_SAM_LACLOCK_CITY_RECT_RIGHT;
        city1_rect.bottom = MMIWIDGET_SAM_LACLOCK_CITY_RECT_BOTTOM;
        
        city2_rect.left   = MMIWIDGET_SAM_RACLOCK_CITY_RECT_LEFT;
        city2_rect.top    = MMIWIDGET_SAM_RACLOCK_CITY_RECT_TOP;
        city2_rect.right  = MMIWIDGET_SAM_RACLOCK_CITY_RECT_RIGHT;
        city2_rect.bottom = MMIWIDGET_SAM_RACLOCK_CITY_RECT_BOTTOM;
    }
#endif 
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    GetDualAClockWidgetTextStyle(&city_text_style);
    
    //display city one.
    text_id = MMIAPIACC_GetLocalCityStringID();
    MMI_GetLabelTextByLang(text_id, &city_str1); 
    GUISTR_GetStringInfo(&city_text_style, &city_str1, text_state,&str_info);	
    city1_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, city1_rect);

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif    
    {  
        GUIRES_DisplayImg(
            PNULL,
            &city1_rect,
            PNULL,
            win_id,
            IMAGE_WIDGET_DUAL_ACLOCK_TIME_BG,
            &lcd_dev_info
            );
    }

    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&city1_rect,
		&city1_rect,
        (const MMI_STRING_T     *)&city_str1,
        &city_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);

    //display city second.    
    text_id = MMIAPIACC_GetOtherCityStringID();
    if(text_id == TXT_NULL)
    {
        text_id = MMIAPIACC_GetLocalCityStringID();
    }
    MMI_GetLabelTextByLang(text_id, &city_str2); 
    GUISTR_GetStringInfo(&city_text_style, &city_str2, text_state, &str_info);	
    city2_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, city2_rect);

#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() != MMISET_IDLE_SUBSTYLE_SAM)
#endif 
    {
        GUIRES_DisplayImg(
            PNULL,
            &city2_rect,
            PNULL,
            win_id,
            IMAGE_WIDGET_DUAL_ACLOCK_TIME_BG,
            &lcd_dev_info
            ); 
    }
    
    GUISTR_DrawTextToLCDInRect( 
		(const GUI_LCD_DEV_INFO *)&lcd_dev_info,
		&city2_rect,
		&city2_rect,
        (const MMI_STRING_T     *)&city_str2,
        &city_text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO);        
}

/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayDualAClock(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T win_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};    

    
    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if(UILAYER_HANDLE_MULTI == UILAYER_GetHandleType(&lcd_dev_info))
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }

    DisplayDualAClockBg(win_id);     
    DisplayAClock(win_id, MMIWIDGET_DUAL_ACLOCK_LEFT);
    DisplayAClock(win_id, MMIWIDGET_DUAL_ACLOCK_RIGHT); 
    DisplayDualAClockCity(win_id); 

    return 1;
}

/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_DualAClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,      //IN:
                                      DPARAM              param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T win_rect = {0};

     point.x = MMK_GET_TP_X(param);
     point.y = MMK_GET_TP_Y(param);

     MMK_GetWinRect(win_id, &win_rect);

    if (GUI_PointIsInRect(point, win_rect))
    {
         MMIAPIWCLK_OpenMainWin();
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle timer message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleTimer(
                          DPARAM param,
                          MMI_WIN_ID_T win_id
                          )
{
    MMI_WIN_ID_T      parent_win_id = MMK_GetParentWinHandle(win_id);
    GUI_RECT_T        win_rect = {0};
    GUI_RECT_T        lcd_rect = MMITHEME_GetFullScreenRect();
    
    if((*(uint8*)param) != s_sectimer_id )
    {
        return FALSE;
    }
        
    if(!MMK_IsFocusWin(parent_win_id))
    {
        StopSecTimer();
        return FALSE;
    }
            MMK_GetWinRect(win_id, &win_rect);
    	    if (!GUI_IntersectRect(&win_rect, win_rect, lcd_rect))			
			{
				StopSecTimer();
				return FALSE;
			}        
            
#if defined MMI_GRID_IDLE_SUPPORT
            if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
            {
                if(MMIGRID_IsForbidPaint(MMIWIDGET_DUAL_ACLOCK_ID))
                {
                    return FALSE;
                }
            }
#endif
            
#if defined QBTHEME_SUPPORT
            if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
            {
                if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_DUAL_ACLOCK_ID))
                {
                    return FALSE;
                }
            }
#endif
    DisplayDualAClock(win_id);                  
    GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,win_rect, 0 );
    SCI_TRACE_LOW("MMIWIDGET_DUAL_ACLCOK HandleTimer %d",SCI_GetTickCount());
    
    return TRUE;
}
/*****************************************************************************/
//  Description : start timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StartSecTimer(MMI_WIN_ID_T win_id)
{
    //StopSecTimer();

    if (0 == s_sectimer_id )
    {
	    s_sectimer_id  = MMK_CreateWinTimer(win_id, 500, TRUE);  
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StartTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DUAL_ACLOCK_1053_112_2_18_3_8_9_199,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopSecTimer(void)
{
    if (0 != s_sectimer_id )
    {
        MMK_StopTimer(s_sectimer_id );
        s_sectimer_id  = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StopTimer: the timer has stopped!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_DUAL_ACLOCK_1069_112_2_18_3_8_9_200,(uint8*)"");
    }
}
#endif
