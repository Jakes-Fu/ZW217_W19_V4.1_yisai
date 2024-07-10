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
//#include "std_header.h"
#include "mmi_app_widget_trc.h"
#ifdef MMI_WIDGET_ACLOCK
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
#include "mmiwidget_text.h"
#include "mmiwidget_export.h"
#include "mmiwidget_internal.h"
#include "guigraph.h"
#ifdef QBTHEME_SUPPORT
#include "mmiqbtheme_idlewin.h"
#endif

#ifdef MMIWIDGET_ACLOCK_SE
#include "mmiwidget_special_effect.h"
#include "mmiwidget_aclock_se.h"
//#include "mmiset_text.h"
#include "mmiwidget_nv.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIWIDGET_START_DAYTIME_HOUR 6  //按键背光夜间模式关闭背关灯的结束时间
#define MMIWIDGET_END_DAYTIME_HOUR 17  //按键背光夜间模式关闭背关灯的结束时间

#ifdef MAINLCD_SIZE_240X320
#define MMIWIDGET_ACLOCK3_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK3_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT           58
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP            99
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)

#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT           (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT + 5)
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP            MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1 )
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT- 1 )

#define MMIWIDGET_ACLOCK2_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK2_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT           120
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP            75
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT+ MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)

#define MMIWIDGET_ACLOCK_CENTRE_Y                        134

#elif defined MAINLCD_SIZE_240X400
#define MMIWIDGET_ACLOCK3_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK3_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT           58
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP            99
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)

#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT           (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT + 5)
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP            MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1 )
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT- 1 )

#define MMIWIDGET_ACLOCK2_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK2_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT           120
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP            75
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT+ MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)
#elif defined MAINLCD_SIZE_320X480
#define MMIWIDGET_ACLOCK3_NUM_WIDTH                    25    
#define MMIWIDGET_ACLOCK3_NUM_HEIGHT                   25    
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT           130
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP            123
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)

#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT           (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT + 7)
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP            MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1 )
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT- 1 )

#define MMIWIDGET_ACLOCK2_NUM_WIDTH                    25    
#define MMIWIDGET_ACLOCK2_NUM_HEIGHT                   25   
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT           189
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP            83
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT+ MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)
#else
#define MMIWIDGET_ACLOCK3_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK3_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT           58
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP            99
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)

#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT           (MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT + 5)
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP            MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT + MMIWIDGET_ACLOCK3_NUM_WIDTH - 1 )
#define MMIWIDGET_ACLOCK3_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT- 1 )

#define MMIWIDGET_ACLOCK2_NUM_WIDTH                    19    
#define MMIWIDGET_ACLOCK2_NUM_HEIGHT                   19    
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT           121
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP            75
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_RIGHT          (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT+ MMIWIDGET_ACLOCK3_NUM_WIDTH - 1)
#define MMIWIDGET_ACLOCK2_DAY_NUM_RECT_BOTTOM         (MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP + MMIWIDGET_ACLOCK3_NUM_HEIGHT - 1)
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
#define POW(x)  ((x)*(x))

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8	s_sectimer_id[3] = {0};    /*!<按秒钟刷新的timer_id */ 
#ifdef MMIWIDGET_ACLOCK_SE
typedef enum
{
    MMIWIDGET_ACLOCK_SE_OBJ_HOUR,
    MMIWIDGET_ACLOCK_SE_OBJ_MIN,
    MMIWIDGET_ACLOCK_SE_OBJ_SECOND,
    MMIWIDGET_ACLOCK_SE_OBJ_SURFACE_UP,
    MMIWIDGET_ACLOCK_SE_OBJ_SURFACE_DOWN,
    MMIWIDGET_ACLOCK_SE_SHADOW_OBJ_HOUR,
    MMIWIDGET_ACLOCK_SE_SHADOW_OBJ_MIN,
    MMIWIDGET_ACLOCK_SE_SHADOW_OBJ_SENCOND,
    MMIWIDGET_ACLOCK_SE_SHADOW_OBJ_CAP,
#if 0
    MMIWIDGET_ACLOCK_SE_OBJ_ARROW_UP,
    MMIWIDGET_ACLOCK_SE_OBJ_ARROW_DOWN,
#endif
    MMIWIDGET_ACLOCK_SE_MAX        
}MMIWIDGET_ACLOCK_SE_OBJ_E;

typedef struct
{
    BMP_OBJ image_obj[MMIWIDGET_ACLOCK_SE_MAX];
    uint32  surface_buffer_size;
    uint8  *surface_up_buf_ptr;
    BOOLEAN is_swing;
}MMIWIDGET_ACLOCK_SE_T;

LOCAL MMIWIDGET_SPECIAL_EFFECT_T s_aclock_spe_eff_t = {0};
LOCAL MMIWIDGET_ACLOCK_SE_T      s_aclock_t  = {0};
/*****************************************************************************/
//  Description : write watch angle nv
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void  Aclock_WriteAngleNV(int32 *watch_angle_ptr);

/*****************************************************************************/
//  Description : read watch angle nv
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 Aclock_ReadAngleNV(void);

#endif
//LOCAL uint8	s_second_num[3] = {0};    /*!<记录秒针值*/ 
//LOCAL BOOLEAN s_is_slide = FALSE;
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
LOCAL MMI_RESULT_E  HandleWidgetAClockWinMsg(
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
LOCAL BOOLEAN DisplayAClock( MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_AClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      DPARAM            param        //IN:
                                      );

/*****************************************************************************/
//  Description : aclock to handle slide message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
// LOCAL void MMIWIDGET_AClock_HandleSlide(
// 										MMI_WIN_ID_T win_id,
// 										DPARAM param
// 										);

/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
#ifdef MMIWIDGET_ACLOCK_SE
LOCAL int32 BarPhone_CreateAclockBuffer(
                                        MMI_WIN_ID_T win_id,
                                        MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                        );
LOCAL int32 BarPhone_DestroyAclockBuffer(
                                        MMI_WIN_ID_T win_id,
                                        MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                        );

/*****************************************************************************/
//  Description : Destroy aclock buffer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 BarPhone_DisplayAclock(
                                    MMI_WIN_ID_T  win_id, 
                                    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                    );
#else
LOCAL BOOLEAN HandleTimer(DPARAM param,MMI_WIN_ID_T win_id);
LOCAL void StartSecTimer(MMI_WIN_ID_T win_id);
LOCAL void StopSecTimer(MMI_WIN_ID_T win_id);
LOCAL void DisplayMonthNum(MMI_WIN_ID_T win_id);
LOCAL void DisplayDayNum(MMI_WIN_ID_T win_id);
#endif

/**--------------------------------------------------------------------------*
**                         CONSTANT VARIABLES                               *
**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_aclock_widget =
{
    MMIWIDGET_ACLOCK_ID, 
    HandleWidgetAClockWinMsg, 
    PNULL, 
    PNULL,
    PNULL,
    WIDGET_ACLOCK_WIN_ID, 
    IMAGE_WIDGET_ACLOCK_SHORTCUT,
    IMAGE_WIDGET_ACLOCK, 
    TXT_WIDGET_ACLOCK,
    50,//    75, 
    50,//    75
    FALSE
};

#ifndef MMIWIDGET_ACLOCK_SE
PUBLIC const MMIWIDGET_ITEM_INFO_T g_aclock_widget2 =
{
    MMIWIDGET_ACLOCK_ID2, 
    HandleWidgetAClockWinMsg, 
    PNULL, 
    PNULL,
    PNULL,
    WIDGET_ACLOCK_WIN_ID2, 
    IMAGE_WIDGET_ACLOCK_SHORTCUT,
    IMAGE_WIDGET_ACLOCK2, 
    TXT_WIDGET_ACLOCK2,
    50,//    75, 
    50,//    75
    FALSE
};

PUBLIC const MMIWIDGET_ITEM_INFO_T g_aclock_widget3 =
{
    MMIWIDGET_ACLOCK_ID3, 
    HandleWidgetAClockWinMsg, 
    PNULL, 
    PNULL,
    PNULL,
    WIDGET_ACLOCK_WIN_ID3, 
    IMAGE_WIDGET_ACLOCK_SHORTCUT,
    IMAGE_WIDGET_ACLOCK3, 
    TXT_WIDGET_ACLOCK3,
    50,//    75, 
    50,//    75
    FALSE
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Handle WidgetClock Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetAClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T   win_rect = {0};
    GUI_RECT_T  scr_rect    = MMITHEME_GetFullScreenRect();
    static int32       watch_angle = 0;
    SCI_TRACE_LOW("HandleWidgetAClockWinMsg msg_id = %x",msg_id);

        
    MMK_GetWinRect(win_id, &win_rect);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if defined  MMIWIDGET_ACLOCK_SE
        s_aclock_spe_eff_t.CreateResourceBuffer = BarPhone_CreateAclockBuffer;  /*lint !e64 */
        s_aclock_spe_eff_t.DestroyResourceBuffer = BarPhone_DestroyAclockBuffer;    /*lint !e64 */     
        s_aclock_spe_eff_t.Display = BarPhone_DisplayAclock;         /*lint !e64 */
        watch_angle = Aclock_ReadAngleNV();
        *MMIWIDGET_Aclock_GetWatchAnglePtr() = watch_angle;
        s_aclock_t.is_swing = TRUE;
#endif       
        break;

#ifdef MMIWIDGET_ACLOCK_SE
    case MSG_GET_FOCUS:
        if(!MMK_IsFocusWin(win_id))
        {
            s_aclock_t.is_swing= TRUE;
        }
        break;

#else
    case MSG_BACKLIGHT_TURN_ON:
    case MSG_GET_FOCUS:
        StartSecTimer(win_id);
        break;
#endif 
       
    case MSG_TIMER:
#ifndef MMIWIDGET_ACLOCK_SE
        HandleTimer(param,win_id);
#endif
        break;

#ifdef MMIWIDGET_ACLOCK_SE
    case MSG_APP_UP:
        {
            s_aclock_t.is_swing = FALSE;
            watch_angle += 150;
            if(watch_angle >= 450)
            {
                watch_angle = 450;
            }
            *MMIWIDGET_Aclock_GetWatchAnglePtr() = watch_angle;
        }
        break;
        
    case MSG_APP_DOWN:
        {
            s_aclock_t.is_swing = FALSE;
            watch_angle -= 150;
            if(watch_angle <= 0)
            {
                watch_angle = 0;
            }
            *MMIWIDGET_Aclock_GetWatchAnglePtr() = watch_angle;
        }        
        break;
#endif
        
    case MSG_LOSE_FOCUS:
    case MSG_BACKLIGHT_TURN_OFF:
#ifdef MMIWIDGET_ACLOCK_SE
        Aclock_WriteAngleNV(&watch_angle);
#else
        StopSecTimer(win_id);
#endif
        break;

    case MSG_APP_WEB:
        MMIAPISET_SetTime();
        break;
        
    case MSG_WIDGET_TP_PRESS_UP:
        recode= MMIWIDGET_AClock_HandleTpPressUp(win_id,param);
        break;
        
    case MSG_WIDGET_TP_PRESS_LONG_UP:
#ifndef MMIWIDGET_ACLOCK_SE
        StartSecTimer(win_id);
#endif
        break;
        
    case MSG_FULL_PAINT:
#ifndef MMIWIDGET_ACLOCK_SE
        DisplayAClock(win_id);  
#endif
        break;

#ifdef BAR_PHONE_WIDGET 
    case MSG_WIDGET_UPDATE_SOFTKEY:
        MMIWIDGET_SoftkeySetTextId(win_id, TXT_TIME, FALSE);
        break;
#endif        
        
    case MSG_WIDGET_MODIFY_RECT:
        //MMIWIDGET_AClock_HandleSlide(win_id,param);
		break;

    case MSG_WIDGET_PAGE_SLIDE_END: 
#ifdef MMIWIDGET_ACLOCK_SE
        if(GUI_RectIsCovered(scr_rect, win_rect))
        {
            s_aclock_t.is_swing = TRUE;
        }
		else
		{
			s_aclock_t.is_swing = FALSE;
		}
#endif
        break;

    case MSG_WIDGET_PAGE_SLIDE_START:
        break;
		
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
#if defined  MMIWIDGET_ACLOCK_SE
    recode = MMIWIDGET_HandleWidgetSpecialEffectWinMsg(&s_aclock_spe_eff_t, win_id, msg_id, param);
#endif

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
        //SCI_TRACE_LOW:"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_387_112_2_18_3_7_49_166,(uint8*)"");
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_539_112_2_18_3_7_49_167,(uint8*)"");
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
#ifndef MMIWIDGET_ACLOCK_SE
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAClock(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T        win_rect = {0};
    GUI_RECT_T        lcd_rect = {0};
    GUI_RECT_T        clock_icon_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    GUI_RECT_T        img_rect = {0};
    SCI_TIME_T        sys_time        = {0};
    GUI_POINT_T       point = {0};  
    CLOCK_DRAW_IN_T   aclock_input_param  = {0};
    CLOCK_DRAW_OUT_T  aclock_output_param = {0};
    CLOCK_ALPHA_IMG_T bk_img = {0};
    CLOCK_ALPHA_IMG_T fore_img = {0};
    UILAYER_INFO_T    layer_info = {0};
    uint16            width  = 0;
    uint16            height = 0;
    int32             index = win_id - WIDGET_ACLOCK_WIN_ID;
    MMI_IMAGE_ID_T    img_table[3][5] = {
        IMAGE_WIDGET_ACLOCK,
        IMAGE_WIDGET_ACLOCK_HOUR,
        IMAGE_WIDGET_ACLOCK_MIN,
        IMAGE_WIDGET_ACLOCK_SEC_HAND,
        IMAGE_WIDGET_ACLOCK_CENTER,
        
        IMAGE_WIDGET_ACLOCK2,
        IMAGE_WIDGET_ACLOCK_HOUR2,
        IMAGE_WIDGET_ACLOCK_MIN2,
        IMAGE_WIDGET_ACLOCK_SEC_HAND2,
        IMAGE_WIDGET_ACLOCK_CENTER2,
        
        IMAGE_WIDGET_ACLOCK3,
        IMAGE_WIDGET_ACLOCK_HOUR3,
        IMAGE_WIDGET_ACLOCK_MIN3,
        IMAGE_WIDGET_ACLOCK_SEC_HAND3,
        IMAGE_WIDGET_ACLOCK_CENTER3
    };
    
    MMK_GetWinRect(win_id, &win_rect);
    //SCI_TRACE_LOW:"MMIWIDGET_ACLOCK DisplayAClock rect = %d,%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_647_112_2_18_3_7_50_168,(uint8*)"dddd",win_rect.left, win_rect.top, win_rect.right , win_rect.bottom);

    lcd_rect = MMITHEME_GetFullScreenRect();

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //SCI_TRACE_LOW:"MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_652_112_2_18_3_7_50_169,(uint8*)"d", lcd_dev_info.lcd_id);

#if 0
	if (sys_time.hour >= MMIWIDGET_START_DAYTIME_HOUR && sys_time.hour <= MMIWIDGET_END_DAYTIME_HOUR)
	{
		//aclock dial.
		aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
		GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMAGE_WIDGET_ACLOCK_DAY, win_id);
		aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WIDGET_ACLOCK_DAY,win_id,&(aclock_input_param.clock_dial.data_size));
		aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
		aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2; 
	}
    else
	{
		//aclock dial.
		aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
		GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMAGE_WIDGET_ACLOCK_NIGHT, win_id);
		aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMAGE_WIDGET_ACLOCK_NIGHT,win_id,&(aclock_input_param.clock_dial.data_size));
		aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
		aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2; 
	}
#endif

    //aclock dial.
    aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, img_table[index][0], win_id);
    aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(img_table[index][0],win_id,&(aclock_input_param.clock_dial.data_size));
    aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
    aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2; 
    
	//aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, img_table[index][1], win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(img_table[index][1],win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, img_table[index][2], win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(img_table[index][2],win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height/2; 

	//aclock sec hand
    aclock_input_param.sec_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.sec_handle.width, &aclock_input_param.sec_handle.height, img_table[index][3], win_id);        
    aclock_input_param.sec_handle.data_ptr = (uint8 *)MMI_GetLabelImage(img_table[index][3],win_id,&(aclock_input_param.sec_handle.data_size));
    aclock_input_param.sec_handle.center_x = aclock_input_param.sec_handle.width/2;
    aclock_input_param.sec_handle.center_y = aclock_input_param.sec_handle.height/2; 
    
    aclock_input_param.target_buf_size = aclock_input_param.clock_dial.width * aclock_input_param.clock_dial.height * 4;
    aclock_input_param.target_buf_ptr = (void*)SCI_ALLOCA(aclock_input_param.target_buf_size);

    if (PNULL == aclock_input_param.target_buf_ptr)
    {
        //SCI_TRACE_LOW:"MMIWIDGET_ACLOCK DisplayAClock SCI_ALLOCA error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_708_112_2_18_3_7_50_170,(uint8*)"");
        return FALSE;
    }
    
    //get system time
    TM_GetSysTime(&sys_time);  

    aclock_input_param.time.hour = sys_time.hour;
    aclock_input_param.time.minute  = sys_time.min;
    aclock_input_param.time.second = sys_time.sec;

#if defined(ANALOG_CLK_SUPPORT)
    if(0 != GRAPH_DrawClock(&aclock_input_param, &aclock_output_param))
    {
        //SCI_TRACE_LOW:"MMIWIDGET_ACLOCK DisplayAClock GRAPH_DrawClock error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_722_112_2_18_3_7_50_171,(uint8*)"");
        SCI_FREE(aclock_input_param.target_buf_ptr);     
        return FALSE;
    }
#else
        return FALSE;
#endif

#if 0
    {
        FILE *fp = fopen("d:\\aclock.raw", "w+b");
        
        fwrite(aclock_output_param.output_ptr, 1, aclock_input_param.target_buf_size, fp);
        
        fclose(fp);
    }
#endif

    img_rect.left = 0;
    img_rect.top = 0;
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
        SCI_FREE(aclock_input_param.target_buf_ptr);     
        return FALSE;
    }
    
    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;
    
    fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;

   // if (lcd_dev_info.block_id == block_id)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
        LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, &lcd_dev_info, &win_rect, &clock_icon_rect);
        fore_img.start_x = clock_icon_rect.left;   
        fore_img.start_y = clock_icon_rect.top; 

        if (!ProcessAlphaBlending(&bk_img, &fore_img))
        {
            SCI_FREE(aclock_input_param.target_buf_ptr); 
            return FALSE;             
        }
    }
   /*
    else
    {
        point.x = win_rect.left;  
        point.y = win_rect.top;  
        LCD_ConvertDispPoint(GUILCD_CONVERT_POS_LCD2DISP, &lcd_dev_info, &point, &point);
        fore_img.start_x = point.x;  
        fore_img.start_y = point.y;

        if (!Convert888To888Or565(&bk_img, &fore_img))
        {
            SCI_FREE(aclock_input_param.target_buf_ptr); 
            return FALSE; 
        }
    }
    */
    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, img_table[index][4], win_id);    
    point.x = win_rect.left + (aclock_input_param.clock_dial.width - width)/2;
    point.y = win_rect.top + (aclock_input_param.clock_dial.height - height)/2;
    
    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      img_table[index][4],
                      &lcd_dev_info);

    MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);
        
    SCI_FREE(aclock_input_param.target_buf_ptr);	
    
    DisplayMonthNum(win_id);
    DisplayDayNum(win_id);
    return TRUE;
}

#endif
/*****************************************************************************/
//  Description : aclock to handle tp up message.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIWIDGET_AClock_HandleTpPressUp(
                                      MMI_WIN_ID_T        win_id,      //IN:
                                      DPARAM              param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};
    GUI_RECT_T button_time_rect = MMIWIDGET_ACLOCK_RECT;

     point.x = MMK_GET_TP_X(param);
     point.y = MMK_GET_TP_Y(param);
     point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

	 //取point到圆心的距离和半径比较，判断点是否在圆内。
	 button_time_rect.right = button_time_rect.right - button_time_rect.left;
     point.x = point.x - button_time_rect.left;

    if ((POW(point.x - button_time_rect.right/2)+ POW(point.y - button_time_rect.bottom/2)) < POW(button_time_rect.right/2))
    {
         MMIAPISET_SetTime();
    }

    return recode;
}
#ifndef MMIWIDGET_ACLOCK_SE
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
    SCI_TIME_T        sys_time        = {0};
    MMI_WIN_ID_T      parent_win_id = MMK_GetParentWinHandle(win_id);
    GUI_RECT_T        win_rect = {0};
    GUI_RECT_T        lcd_rect = MMITHEME_GetFullScreenRect();
    int32  index = 0;
    index = win_id - WIDGET_ACLOCK_WIN_ID;
    
    if((*(uint8*)param) != s_sectimer_id[index] )
    {
        return FALSE;
    }
    if(!MMK_IsFocusWin(parent_win_id))
    {
        StopSecTimer(win_id);
        return FALSE;
    }
    
    MMK_GetWinRect(win_id, &win_rect);
    if (!GUI_IntersectRect(&win_rect, win_rect, lcd_rect))
    {
        StopSecTimer(win_id);
        return FALSE;
    }
    
#if defined MMI_GRID_IDLE_SUPPORT
    if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
    {
        if(MMIGRID_IsForbidPaint(MMIWIDGET_ACLOCK_ID))
        {
            return FALSE;
        }
        
    }
#endif
    
#if defined QBTHEME_SUPPORT
    if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
    {
        if(MMIQBIDLE_IsForbidPaint(MMIWIDGET_ACLOCK_ID))
        {
            return FALSE;
        }
    }
#endif

    TM_GetSysTime(&sys_time);
    
//    if(s_second_num[index]  == sys_time.sec)
//    {
//        return FALSE;
//     }
    
    if (DisplayAClock(win_id))
    {  
        GUILCD_InvalidateRect(GUI_MAIN_LCD_ID,win_rect, 0 );
        //s_second_num[index] = sys_time.sec;
    }

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
    int32               time_interval = 0;
    int32             index = 0;
#ifdef MMIWIDGET_SUPPORT
    time_interval = 1000*60;
#endif
#ifdef MMI_GRID_IDLE_SUPPORT
    time_interval = 500;
#endif
    index = win_id - WIDGET_ACLOCK_WIN_ID;
    if (0 == s_sectimer_id[index] )
    {
	    s_sectimer_id[index]  = MMK_CreateWinTimer(win_id, time_interval, TRUE);  
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StartTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_952_112_2_18_3_7_50_172,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop timer.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void StopSecTimer(MMI_WIN_ID_T win_id)
{
    int32  index = win_id - WIDGET_ACLOCK_WIN_ID;
    if (0 != s_sectimer_id[index] )
    {
        MMK_StopTimer(s_sectimer_id[index] );
        s_sectimer_id[index]  = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIWIDGET]: StopTimer: the timer has stopped!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_ACLOCK_970_112_2_18_3_7_50_173,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : get grid alarm text style.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetAlarmTextStyle(GUISTR_STYLE_T *style_ptr,
							 GUI_FONT_T     font,
							 GUI_COLOR_T    font_color
							)
{	
    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_HVMIDDLE;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
	style_ptr->line_space = 0;    
}

/*****************************************************************************/
//  Description : display month.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayMonthNum(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    int32  index = win_id - WIDGET_ACLOCK_WIN_ID;
    GUI_RECT_T        disp_rect = {0};
	GUISTR_STYLE_T text_style = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_BOLD;	
	MMI_STRING_T    disp_str= {0};
	wchar           temp_wstr[3] = {0};
    char            temp_str[3] = {0};
    SCI_DATE_T      date      = {0};    
    
 
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    UILAYER_ClearRect(&lcd_dev_info,  disp_rect);

    TM_GetSysDate(&date); 
    disp_str.wstr_len = 2;
    sprintf(temp_str, "%2ld", date.mon);
    MMI_STRNTOWSTR(temp_wstr, disp_str.wstr_len, (uint8*)temp_str, disp_str.wstr_len, disp_str.wstr_len);
    disp_str.wstr_ptr = temp_wstr;

    switch (index)
    {
        case 2:
        disp_rect.left = MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_LEFT;        
        disp_rect.top = MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_TOP;        
        disp_rect.right = MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_RIGHT;
        disp_rect.bottom = MMIWIDGET_ACLOCK3_MONTH_NUM_RECT_BOTTOM;
        break;
        
        default:
        return;
    }
    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
    GetAlarmTextStyle(&text_style, SONG_FONT_16, MMI_BLACK_COLOR);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &disp_rect,
        &disp_rect,
        &disp_str,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  Description : display day.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayDayNum(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T        win_rect = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = {0, 0};
    int32  index = win_id - WIDGET_ACLOCK_WIN_ID;
    GUI_RECT_T        disp_rect = {0};
	GUISTR_STYLE_T text_style = {0};
	GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_BOLD;	
	MMI_STRING_T    disp_str= {0};
	wchar           temp_wstr[3] = {0};
    char            temp_str[3] = {0};
    SCI_DATE_T      date      = {0};    
    
 
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    UILAYER_ClearRect(&lcd_dev_info,  disp_rect);

    TM_GetSysDate(&date); 
    disp_str.wstr_len = 2;
    sprintf(temp_str, "%2ld", date.mday);
    MMI_STRNTOWSTR(temp_wstr, disp_str.wstr_len, (uint8*)temp_str, disp_str.wstr_len, disp_str.wstr_len);
    disp_str.wstr_ptr = temp_wstr;

    switch (index)
    {    
        case 1:
            disp_rect.left = MMIWIDGET_ACLOCK2_DAY_NUM_RECT_LEFT;        
            disp_rect.top = MMIWIDGET_ACLOCK2_DAY_NUM_RECT_TOP;        
            disp_rect.right = MMIWIDGET_ACLOCK2_DAY_NUM_RECT_RIGHT;
            disp_rect.bottom = MMIWIDGET_ACLOCK2_DAY_NUM_RECT_BOTTOM;
        break;
        
        case 2:
            disp_rect.left = MMIWIDGET_ACLOCK3_DAY_NUM_RECT_LEFT;        
            disp_rect.top = MMIWIDGET_ACLOCK3_DAY_NUM_RECT_TOP;        
            disp_rect.right = MMIWIDGET_ACLOCK3_DAY_NUM_RECT_RIGHT;
            disp_rect.bottom = MMIWIDGET_ACLOCK3_DAY_NUM_RECT_BOTTOM;
        break;
        
        default:
        return;
    }
    disp_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, disp_rect);
    GetAlarmTextStyle(&text_style, SONG_FONT_16, MMI_BLACK_COLOR);
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &disp_rect,
        &disp_rect,
        &disp_str,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
}
#else
/*****************************************************************************/
//  Description : display aclock special effect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 BarPhone_DisplayAclock(
                                    MMI_WIN_ID_T win_id,
                                    MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                    )
{
    WATCH_F_PARAM        aclock_se_param = {0};
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    GUI_RECT_T           win_rect        = {0};
    uint16               screen_width    =  0;
    uint16               screen_height   =  0;
    SCI_TIME_T           cur_time        = {0};

    TM_GetSysTime(&cur_time);

    MMK_GetWinRect(win_id, &win_rect);

    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &screen_width, &screen_height);
    UILAYER_SetPreMultiplyAlpha(&lcd_dev_info,  TRUE,  FALSE);
    
    aclock_se_param.out_size.w  = win_rect.right - win_rect.left + 1;                    
    aclock_se_param.out_size.h  = win_rect.bottom - win_rect.top + 1; 

    aclock_se_param.watch_centre.x = screen_width/2;
    aclock_se_param.watch_centre.y = MMIWIDGET_ACLOCK_CENTRE_Y;//(screen_height)/2;
    aclock_se_param.watch_centre.z=(WATCH_SUR_TWO_Z+WATCH_SUR_ONE_Z)/2;

    aclock_se_param.Is_swing = s_aclock_t.is_swing;
    SCI_MEMCPY(s_aclock_t.image_obj[MMIWIDGET_ACLOCK_SE_OBJ_SURFACE_UP].image_buffer,
            s_aclock_t.surface_up_buf_ptr,s_aclock_t.surface_buffer_size);

    aclock_se_param.resource_collection = s_aclock_t.image_obj;

    aclock_se_param.total_frame = 48;

    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_DOUBLE_COPY);
    UILAYER_SwitchDoubleMem( &lcd_dev_info, TRUE );
    UILAYER_SetDirectDraw(TRUE);   
    if(-1 == p_widget_se->current_frame)
    {  
        aclock_se_param.cur_frame = 0;      
        p_widget_se->current_frame = 0;
    }
    else if(p_widget_se->current_frame < aclock_se_param.total_frame)
    {
        aclock_se_param.cur_frame = p_widget_se->current_frame; 
        p_widget_se->current_frame ++;
    }
    else
    {
        aclock_se_param.cur_frame = p_widget_se->current_frame; 
    }
    if(0 != aclock_se_param.cur_frame)
    {
        aclock_se_param.Hour   = cur_time.hour;
        aclock_se_param.Min    = cur_time.min;
        aclock_se_param.Second = cur_time.sec;
    }

    UILAYER_Clear(&lcd_dev_info);		
    aclock_se_param.out_buffer = UILAYER_GetLayerBufferPtr(&lcd_dev_info);
    if(aclock_se_param.Is_swing)
    {
        MMIWIDGET_Creat_Watch(&aclock_se_param);
        if(aclock_se_param.cur_frame >= aclock_se_param.total_frame)
        {
            s_aclock_t.is_swing= FALSE;
            aclock_se_param.Is_swing = FALSE;
    		MMIWIDGET_Creat_Watch(&aclock_se_param);                
        }
    }
    else
    {
		MMIWIDGET_Creat_Watch(&aclock_se_param);
    }
    return 0;
}
/*****************************************************************************/
//  Description : create aclock buffer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 BarPhone_CreateAclockBuffer(
                                        MMI_WIN_ID_T win_id,
                                        MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                        )
{
    uint8              i                  = 0;
    uint16            image_width         = 0;
    uint16           image_height         = 0;
    uint32*               p_buffer         = 0;
    MMI_IMAGE_ID_T image_id_array[]       = {
                                            MMIWIDGET_ACLOCK_SE_HOUR,
                                            MMIWIDGET_ACLOCK_SE_MIN,
                                            MMIWIDGET_ACLOCK_SE_SECOND,
                                            MMIWIDGET_ACLOCK_SE_SURFACE_UP,
                                            MMIWIDGET_ACLOCK_SE_SURFACE_DOWN,
                                            MMIWIDGET_ACLOCK_SE_SHADOW_HOUR,
                                            MMIWIDGET_ACLOCK_SE_SHADOW_MIN,
                                            MMIWIDGET_ACLOCK_SE_SHADOW_SENCOND,
                                            MMIWIDGET_ACLOCK_SE_CAP,
                                            #if 0
                                            MMIWIDGET_ACLOCK_SE_ARROW_UP,
                                            MMIWIDGET_ACLOCK_SE_ARROW_DOWN
                                            #endif
                                            };

    for(i = 0; i < MMIWIDGET_ACLOCK_SE_MAX; i++)
    { 
        GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id_array[i],win_id);    
        p_buffer = MMIWIDGET_CreatePictureResource(win_id, image_id_array[i]);
 
        if(MMIWIDGET_ACLOCK_SE_OBJ_SURFACE_UP == i)
        {   
            if(PNULL == s_aclock_t.surface_up_buf_ptr)
            {
                s_aclock_t.surface_buffer_size = image_width*image_height*4;
                s_aclock_t.surface_up_buf_ptr = SCI_ALLOC_APP(s_aclock_t.surface_buffer_size);
            }
            SCI_MEMCPY(s_aclock_t.surface_up_buf_ptr,p_buffer,s_aclock_t.surface_buffer_size);
        }
        //SET OBJ
        s_aclock_t.image_obj[i].image_buffer = (uint32*)p_buffer;
        s_aclock_t.image_obj[i].image_size.w = image_width;
        s_aclock_t.image_obj[i].image_size.h = image_height;
    }
    return 0;
}
/*****************************************************************************/
//  Description : Destroy aclock buffer
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 BarPhone_DestroyAclockBuffer(
                                        MMI_WIN_ID_T    win_id, 
                                        MMIWIDGET_SPECIAL_EFFECT_T *p_widget_se
                                        )
{    
    int32 i = 0;
    GUI_LCD_DEV_INFO     lcd_dev_info    = {0};
    
    for(i = 0; i < MMIWIDGET_ACLOCK_SE_MAX; i++)
    {
        SCI_FREE(s_aclock_t.image_obj[i].image_buffer);
        s_aclock_t.image_obj[i].image_buffer = PNULL;
    }
    if(PNULL != s_aclock_t.surface_up_buf_ptr)
    {
        SCI_FREE(s_aclock_t.surface_up_buf_ptr);
        s_aclock_t.surface_up_buf_ptr = PNULL;
    }
    s_aclock_t.surface_buffer_size = 0;

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    UILAYER_SetLayerMemFormat(&lcd_dev_info, UILAYER_MEM_IMMUTABLE);
    
    return 0;
}
/*****************************************************************************/
//  Description : write watch angle nv
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void Aclock_WriteAngleNV(int32 *watch_angle_ptr)
{
    MMINV_WRITE(MMI_WIDGET_WATCH_ANGLE, watch_angle_ptr);
}
/*****************************************************************************/
//  Description : read watch angle nv
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 Aclock_ReadAngleNV(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    int32              *watch_angle_ptr = MMIWIDGET_Aclock_GetWatchAnglePtr();
    
    MMINV_READ(MMI_WIDGET_WATCH_ANGLE, watch_angle_ptr, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        *watch_angle_ptr = 450;
        MMINV_WRITE(MMI_WIDGET_WATCH_ANGLE, watch_angle_ptr);
    }
    return *watch_angle_ptr;
}

#endif
#endif											   
