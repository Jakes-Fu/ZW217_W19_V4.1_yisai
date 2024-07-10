#include "mmi_app_accessory_trc.h"
#ifdef MMI_ALARM_GESTURE_CLOCK_SUPPORT
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
#include "mmialarm_export.h"
#include "graphics_draw.h"
#include "Mmk_tp.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_320X480
#define CLOCK_AM_GAP    15
#define TIME_BG_GAP     68
#define SOFTKEY_HEIGHT     50
#define CLOCK_R         136//以时针为半径的圆的半径  
#define CLOCK_TIME_GAR  (0)
#define MMIALARM_CLOCK_TIME_OFFSET 20
#define MMIALARM_CLOCK_TIME_FONT 32
#elif defined MAINLCD_SIZE_240X400
#define CLOCK_AM_GAP    7
#define TIME_BG_GAP     60
#define SOFTKEY_HEIGHT     45
#define CLOCK_R         97//以时针为半径的圆的半径 
#define CLOCK_TIME_GAR  (5)
#define MMIALARM_CLOCK_TIME_OFFSET 10
#define MMIALARM_CLOCK_TIME_FONT 24
#else
#define CLOCK_AM_GAP    7
#define TIME_BG_GAP     50
#define SOFTKEY_HEIGHT     45
#define CLOCK_R         97//以时针为半径的圆的半径  
#define CLOCK_TIME_GAR  (5)
#define MMIALARM_CLOCK_TIME_OFFSET 10
#define MMIALARM_CLOCK_TIME_FONT 24
#endif 

#define MMIALARM_CLOCK_MOVE_TIME               10

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

static const int tan_0_90_table[90] = 
{
  0,
 17, 34, 52, 69, 87,105,122,140,158,176,
194,212,230,249,267,286,305,324,344,364,
383,404,424,445,466,487,509,531,554,577,
600,624,649,674,700,726,753,781,809,839,
869,900,932,965,1000,1035,1072,1110,1150,1191,
1234,1279,1327,1376,1428,1482,1539,1600,1664,1732,
1804,1880,1962,2050,2144,2246,2355,2475,2605,2747,
2904,3077,3270,3487,3732,4010,4331,4704,5144,5671,
6313,7115,8144,9514,11430,14300,19081,28636,57290,
};


static GUI_POINT_T       s_clk_center_point = {0}; 
//static GUI_RECT_T        s_clk_bg_rect= {0};
static GUI_RECT_T        s_clk_am_rect= {0};
LOCAL uint8   s_clk_time_id = 0;    //动画时钟 
LOCAL SCI_TIME_T  s_alm_time ={0};
LOCAL SCI_TIME_T  s_alm_pre_time ={0};
//LOCAL SCI_TIME_T  s_alm_last_second_time ={0};
LOCAL BOOLEAN    s_is_move_min =TRUE;//是否移动时针
LOCAL BOOLEAN    s_is_clk_am = TRUE;
LOCAL BOOLEAN    s_is_clk_move_start = FALSE;
LOCAL alarmClkSaveTimeFunc s_clk_savetime_func =PNULL;

/*****************************************************************************/
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  isPressHourHandle(MMI_WIN_ID_T win_id,SCI_TIME_T sys_time, GUI_POINT_T point);

/*****************************************************************************/
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  isPressMinHandle(MMI_WIN_ID_T win_id,SCI_TIME_T sys_time, GUI_POINT_T point);

/*****************************************************************************/
//  Description : math function: b*b.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 pow(int32 b);
/*****************************************************************************/
//  Description : is in am rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isPressAm(GUI_POINT_T   point);
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isPressClock(GUI_POINT_T   point);
/*****************************************************************************/
//  Description : GetTime
//  Global resource dependence : 
//  Author: 
//  Note:从角度，返回小时或者分钟的时间
/*****************************************************************************/
LOCAL int GetTime(int angel, BOOLEAN ishour);

/*****************************************************************************/
//  Description : get angel
//  Global resource dependence : 
//  Author: 
//  Note:point1:原点, point2:手指所在位置
/*****************************************************************************/
LOCAL int GetAngel(GUI_POINT_T point1, GUI_POINT_T point2);

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
LOCAL MMI_RESULT_E  HandleAlarmClockWinMsg(
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
LOCAL BOOLEAN DisplayAlmClock(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect);
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAlmTime(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect);


/*****************************************************************************/
// Description : StartMoveTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartMoveTimer(uint8* time_id,MMI_HANDLE_T handle);

/*****************************************************************************/
// Description : StopMoveTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopMoveTimer( uint8* time_id);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
WINDOW_TABLE( MMIALM_CLOCK_TAB ) = 
{
    //CLEAR_LCD,
//    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_ID( MMIALM_CLOCK_WIN_ID ),
    WIN_TITLE( TXT_ALARM_CLOCK ),
    WIN_FUNC((uint32)HandleAlarmClockWinMsg ),  

    WIN_STATUSBAR,
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL ),
     END_WIN
};
/*****************************************************************************/
//  Description : Handle WidgetClock Win Msg
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAlarmClockWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM            param        //IN:
                                      )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_RECT_T   time_rect ={0};
    GUI_RECT_T   clock_rect ={0};
    uint16      width = 0; 
    uint16      height = 0;  
    GUI_POINT_T point;
    static GUI_POINT_T pre_point={0};
  //  SCI_TIME_T  alm_time ={0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:       
        MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
        MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);//禁止横屏
        break;
    case MSG_GET_FOCUS:
        break;

    case MSG_BACKLIGHT_TURN_ON:
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        StopMoveTimer(&s_clk_time_id);
        if(isPressClock(point))
        {

            if(s_is_clk_move_start)
            {
                if(!s_is_move_min)
                {
                    s_alm_time.hour =    GetTime(GetAngel(s_clk_center_point, point), TRUE); 
                }
                else
                {
                
                    s_alm_time.min = GetTime(GetAngel(s_clk_center_point, point), FALSE);
                    if(
                       (s_alm_time.min<=30)//当前分针在中的右半部
                       &&(s_alm_pre_time.min<=59)&& (s_alm_pre_time.min>30)//上一个点分针在钟的左半部
                       //&&(s_alm_last_second_time.min<=s_alm_pre_time.min)//倒数第二个点的分钟数小于，上一个点的分针数（顺时针）
                        &&(point.y<s_clk_center_point.y)
                        )
                    {
                        s_alm_time.hour= (s_alm_time.hour+1);
                        if(s_alm_time.hour > 12)
                        {
                            s_alm_time.hour = s_alm_time.hour -12;
                        }
                    }
                   // else if((s_alm_pre_time.min<15) && (s_alm_time.min>45))
                    else if(
                           (s_alm_time.min>30)&&(s_alm_time.min<=59)
                           &&(s_alm_pre_time.min<=30)
                           //&&(s_alm_last_second_time.min>=s_alm_pre_time.min)
                           &&(point.y<s_clk_center_point.y)
                           )
                    {
                        if(s_alm_time.hour == 0)
                        {
                            s_alm_time.hour= 12;
                        }
                        s_alm_time.hour= (s_alm_time.hour-1);
                        if(s_alm_time.hour == 0)
                        {
                            s_alm_time.hour = 12;
                        }

                    }
                    //s_alm_last_second_time.min = s_alm_pre_time.min;
                    s_alm_pre_time.min = s_alm_time.min;                    
                }
                MMK_UpdateScreen();

            }
        }
        s_is_clk_move_start = FALSE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    //case MSG_TP_PRESS_MOVE:
    case MSG_TIMER:
        if (s_clk_time_id == *(uint8*)param)
        {
            MMI_TP_STATUS_E state= MMI_TP_NONE;
            MMK_GetLogicTPMsg(&state, &point);
            
            StopMoveTimer(&s_clk_time_id);
            if(isPressClock(point))
            {
                if(s_is_clk_move_start)
                {
                    if(point.x==pre_point.x&&point.y==pre_point.y)
                    {
                        StartMoveTimer(&s_clk_time_id,win_id);
                        break; 
                    }
                    pre_point=point;
                
                    if(!s_is_move_min)
                    {
                        s_alm_time.hour =    GetTime(GetAngel(s_clk_center_point, point), TRUE);
                    }
                    else
                    {
                    
                        s_alm_time.min = GetTime(GetAngel(s_clk_center_point, point), FALSE);
                        if(
                           (s_alm_time.min<=30)//当前分针在中的右半部
                           &&(s_alm_pre_time.min<=59)&& (s_alm_pre_time.min>30)//上一个点分针在钟的左半部
                           //&&(s_alm_last_second_time.min<=s_alm_pre_time.min)//倒数第二个点的分钟数小于，上一个点的分针数（顺时针）
                           &&(point.y<s_clk_center_point.y)
                            )
                        {
                            
                            s_alm_time.hour= (s_alm_time.hour+1);
                            if(s_alm_time.hour > 12)
                            {
                                s_alm_time.hour = s_alm_time.hour -12;
                            }
                        }
                        else if(
                               (s_alm_time.min>30)&&(s_alm_time.min<=59)
                               && (s_alm_pre_time.min<=30)
                               //&&(s_alm_last_second_time.min>=s_alm_pre_time.min)
                               &&(point.y<s_clk_center_point.y)
                               )
                        {
                            if(s_alm_time.hour == 0)
                            {
                                s_alm_time.hour= 12;
                            }
                            s_alm_time.hour= (s_alm_time.hour-1);
                            if(s_alm_time.hour == 0)
                            {
                                s_alm_time.hour= 12;
                            }

                        }
                        else
                        {
                            
                        }
                        //s_alm_last_second_time.min = s_alm_pre_time.min;
                        s_alm_pre_time.min = s_alm_time.min;
                    }
                    MMK_UpdateScreen();
                    StartMoveTimer(&s_clk_time_id,win_id);
                }
            }
            else
            {
                s_is_clk_move_start =FALSE;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {
            //GUI_POINT_T point;
            GUI_POINT_T clock_point;
            uint16  clockwidth = 0;
            uint16  clockheight = 0;

            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            pre_point.x=point.x;
            pre_point.y=point.y;
            if(isPressClock(point))
            {
                GUIRES_GetImgWidthHeight(&clockwidth, &clockheight, IMG_CLOCK_DAY, win_id);
                clock_point.x = point.x - s_clk_center_point.x + (clockwidth/2);
                clock_point.y = point.y - s_clk_center_point.y + (clockheight/2);

                if(isPressMinHandle(win_id, s_alm_time , clock_point))
                {
                    
                    s_alm_pre_time.min = s_alm_time.min;//GetTime(GetAngel(s_clk_center_point, point), FALSE);
                    s_alm_time.min=GetTime(GetAngel(s_clk_center_point, point), FALSE);
                    if(
                           (s_alm_time.min<=30)//当前分针在中的右半部
                           &&(s_alm_pre_time.min<=59)&& (s_alm_pre_time.min>30)//上一个点分针在钟的左半部
                           //&&(s_alm_last_second_time.min<=s_alm_pre_time.min)//倒数第二个点的分钟数小于，上一个点的分针数（顺时针）
                           &&(point.y<s_clk_center_point.y)
                            )
                    {
                        
                        s_alm_time.hour= (s_alm_time.hour+1);
                        if(s_alm_time.hour > 12)
                        {
                            s_alm_time.hour = s_alm_time.hour -12;
                        }
                    }
                    else if(
                           (s_alm_time.min>30)&&(s_alm_time.min<=59)
                           && (s_alm_pre_time.min<=30)
                           //&&(s_alm_last_second_time.min>=s_alm_pre_time.min)
                           &&(point.y<s_clk_center_point.y)
                           )
                    {
                        if(s_alm_time.hour == 0)
                        {
                            s_alm_time.hour= 12;
                        }
                        s_alm_time.hour= (s_alm_time.hour-1);
                        if(s_alm_time.hour == 0)
                        {
                            s_alm_time.hour= 12;
                        }

                    }
                    s_alm_pre_time.min = s_alm_time.min;
                    //s_alm_last_second_time.min = s_alm_pre_time.min;
                    s_is_clk_move_start = TRUE;
                    s_is_move_min = TRUE;
                    StartMoveTimer(&s_clk_time_id,win_id);

                }
                else if(isPressHourHandle(win_id, s_alm_time , clock_point))
                {
                    s_alm_pre_time.min = GetTime(GetAngel(s_clk_center_point, point), FALSE);
                    //s_alm_last_second_time.min = s_alm_pre_time.min;
                    s_is_clk_move_start = TRUE;
                    s_is_move_min = FALSE;
                    StartMoveTimer(&s_clk_time_id,win_id);
                }
                MMK_UpdateScreen();
            }
            else if(isPressAm(point))
            {
                if(s_is_clk_am == TRUE)
                {
                    s_is_clk_am =FALSE;
                }
                else
                {
                    s_is_clk_am =TRUE;
                }
                MMK_UpdateScreen();
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
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
        if(s_clk_savetime_func())
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;     
        
        
    case MSG_FULL_PAINT:
        {
            GUIRES_GetImgWidthHeight(&width, &height, IMG_CLOCK_TIME_BG, win_id);
            MMK_GetWinRect(win_id, &time_rect);
            GUIRES_DisplayImg(PNULL,
		        &time_rect,
		        &time_rect,
		        win_id,
		        IMAGE_COMMON_BG,
		        &lcd_dev_info);
            time_rect.top =time_rect.top+TIME_BG_GAP;
            time_rect.bottom = time_rect.top + height;
            DisplayAlmTime(win_id, s_alm_time, time_rect);
            
            MMK_GetWinRect(win_id, &clock_rect);
            clock_rect.top = time_rect.bottom;
            clock_rect.bottom = clock_rect.bottom - SOFTKEY_HEIGHT;
            s_clk_center_point.y = clock_rect.top + (clock_rect.bottom - clock_rect.top)/2;
            s_clk_center_point.x = clock_rect.left + (clock_rect.right -clock_rect.left)/2;
            DisplayAlmClock(win_id, s_alm_time, clock_rect);       
        }
        break;

    case MSG_CLOSE_WINDOW:
        //s_alm_last_second_time.min = 0;
        //s_alm_last_second_time.hour = 0;
        s_alm_pre_time.min = 0;
        s_alm_pre_time.hour = 0;
        s_alm_time.min = 0;
        s_alm_time.hour = 0;
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
        //SCI_TRACE_LOW:"aclock ProcessAlphaBlending (int32)bk_width < start_x || (int32)bk_height < start_y"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_CLOCK_568_112_2_18_2_1_15_57,(uint8*)"");
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_CLOCK_720_112_2_18_2_1_15_58,(uint8*)"");
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
//  Description : AM OR PM
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsAmTime()
{
    return s_is_clk_am;
}

/*****************************************************************************/
//  Description : AM OR PM
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/

LOCAL void DrawTime(SCI_TIME_T sys_time, GUI_POINT_T point)
{
    uint8               time_str[10]  = {0};
    wchar               time_wstr[10]    = {0};
    GUISTR_STYLE_T      str_style = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUISTR_STATE_T state = {0};/*lint !e64 */
    MMI_STRING_T string = {0};

    str_style.align = ALIGN_LVMIDDLE;
    str_style.font  = MMIALARM_CLOCK_TIME_FONT;

    str_style.font_color = MMITHEME_GetCurThemeFontColor(MMI_THEME_TEXT_FONT);
    state = GUISTR_STATE_BOLD | GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT;
   
    
    sprintf((char *)time_str," %02d:%02d ",sys_time.hour,sys_time.min);
    MMIAPICOM_StrToWstr(time_str, time_wstr); 
	string.wstr_ptr = time_wstr;
	string.wstr_len = MMIAPICOM_Wstrlen(time_wstr);
    
    GUISTR_DrawTextToLCDSingleLine(&lcd_dev_info, &point, &string, &str_style, state);
}
/*****************************************************************************/
//  Description : get angel
//  Global resource dependence : 
//  Author: 
//  Note:point1:原点, point2:手指所在位置
/*****************************************************************************/
LOCAL int GetAngel(GUI_POINT_T point1, GUI_POINT_T point2)
{
    int i=0;
    int value = 0;    
    
    if((-5<(point2.y- point1.y)) &&((point2.y- point1.y)<5))//
    {
        if(point2.x > point1.x)
        {
            return 90;
        }
        else
        {
            return 270;
        }
    }
    if((-5<(point2.x-point1.x)) && ((point2.x-point1.x)<5))//
    {
        if(point2.y > point1.y)
            return 180;
        else
            return 0;
    }

   if((point2.x > point1.x) && (point2.y < point1.y))//右上角
   {
       value = ((point2.x - point1.x)*1000)/(point1.y-point2.y);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
               return i; 
            }
       }
       
   }
   else if((point2.x > point1.x) && (point2.y > point1.y))//右下角
   {
       value = ((point2.y - point1.y)*1000)/(point2.x-point1.x);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
               return 90+i; 
            }
       }

   }
   else if((point2.x < point1.x) && (point2.y > point1.y))//左下角
   {
       value = ((point1.x - point2.x)*1000)/(point2.y-point1.y);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
               return 180+i; 
            }
       }

   }
   else if((point2.x < point1.x) && (point2.y < point1.y))//左上角
   {
       value = ((point1.y - point2.y)*1000)/(point1.x-point2.x);
       for(i=0;i<90; i++)
       {
            if(value< tan_0_90_table[i])
            {
               return 270+i; 
            }
       }

   }

   return 0;
}


/*****************************************************************************/
//  Description : GetTime
//  Global resource dependence : 
//  Author: 
//  Note:从角度，返回小时或者分钟的时间
/*****************************************************************************/
LOCAL int GetTime(int angel, BOOLEAN ishour)
{
    int time=0;
    if(ishour)
    {
        time=angel/30;
        if(0==time)
        {
           time=12; 
        }
    }
    else
    {
        time=angel/6;
    }

    return time;
}

/*****************************************************************************/
//  Description : is in am rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isPressAm(GUI_POINT_T   point)
{
    
    if(
        (s_clk_am_rect.top<=point.y)
        &&(point.y<=s_clk_am_rect.bottom)
        && (s_clk_am_rect.left<=point.x)
        &&(point.x<=s_clk_am_rect.right)
       )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN isPressClock(GUI_POINT_T   point)
{
    int32 pow_x = pow(point.x-s_clk_center_point.x);
    int32 pow_y = pow(point.y-s_clk_center_point.y);


   if((pow_x+ pow_y)<pow(CLOCK_R))
       return TRUE;
   else
       return FALSE;
        
    /*if(
        (s_clk_bg_rect.top<=point.y)
        &&(point.y<=s_clk_bg_rect.bottom)
        && (s_clk_bg_rect.left<=point.x)
        &&(point.x<=s_clk_bg_rect.right)
       )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }*/
}

/*****************************************************************************/
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  isPressHourHandle(MMI_WIN_ID_T win_id,SCI_TIME_T sys_time, GUI_POINT_T point)
{
    int32 ret = SCI_SUCCESS;
	CLOCK_POINT_TEST_IN_T	in_param = {0};
	CLOCK_POINT_TEST_OUT_T	out_param = {0};
    uint16  clockwidth = 0;
    uint16  clockheight = 0;
    uint16  handlewidth = 0;
    uint16  handleheight = 0;

    GUIRES_GetImgWidthHeight(&clockwidth, &clockheight, IMG_CLOCK_DAY, win_id);

	in_param.clock_dial.width = clockwidth;
	in_param.clock_dial.height = clockheight;
	in_param.clock_dial.center_x = clockwidth/2;
	in_param.clock_dial.center_y = clockheight/2;

     GUIRES_GetImgWidthHeight(&handlewidth, &handleheight, IMG_CLOCK_HOUR_HAND, win_id); 
	in_param.handle.width = handlewidth;
	in_param.handle.height = handleheight;
	in_param.handle.center_x = handlewidth/2;
	in_param.handle.center_y = handleheight;//handleheight/2;

	in_param.handle_type = CLOCK_HOUR_HANDLE;		//对秒针做检测
	in_param.time.hour = sys_time.hour;
	in_param.time.minute = sys_time.min;
	in_param.time.second = sys_time.sec;

	in_param.point.x = point.x;
	in_param.point.y = point.y ;
#if defined(ANALOG_CLK_SUPPORT)
	ret = GRAPH_ClockPointTest(&in_param, &out_param);
	if (ret == SCI_SUCCESS)
	{
		return 	out_param.result;
	}
#endif
	return FALSE;
}


/*****************************************************************************/
//  Description : is in CLOCK rect
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL  BOOLEAN  isPressMinHandle(MMI_WIN_ID_T win_id,SCI_TIME_T sys_time, GUI_POINT_T point)
{
    int32 ret = SCI_SUCCESS;
	CLOCK_POINT_TEST_IN_T	in_param = {0};
	CLOCK_POINT_TEST_OUT_T	out_param = {0};
    uint16  clockwidth = 0;
    uint16  clockheight = 0;
    uint16  handlewidth = 0;
    uint16  handleheight = 0;

    GUIRES_GetImgWidthHeight(&clockwidth, &clockheight, IMG_CLOCK_DAY, win_id);

	in_param.clock_dial.width = clockwidth;
	in_param.clock_dial.height = clockheight;
	in_param.clock_dial.center_x = clockwidth/2;
	in_param.clock_dial.center_y = clockheight/2;

     GUIRES_GetImgWidthHeight(&handlewidth, &handleheight, IMG_CLOCK_MIN_HAND, win_id); 
	in_param.handle.width = handlewidth;
	in_param.handle.height = handleheight;
	in_param.handle.center_x = handlewidth/2;
	in_param.handle.center_y =handleheight;//handleheight/2;

	in_param.handle_type = CLOCK_MINUTE_HANDLE;		//对秒针做检测
	in_param.time.hour = sys_time.hour;
	in_param.time.minute = sys_time.min;
	in_param.time.second = sys_time.sec;

	in_param.point.x = point.x;
	in_param.point.y = point.y ;
#if defined(ANALOG_CLK_SUPPORT)
	ret = GRAPH_ClockPointTest(&in_param, &out_param);
	if (ret == SCI_SUCCESS)
	{
		return 	out_param.result;
	}
#endif
	return FALSE;
}
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAlmTime(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect)
{
    uint16 timebg_width = 0;
    uint16 timebg_height = 0;
    uint16 timeam_width = 0;
    uint16 timeam_height = 0;
    GUI_POINT_T       point = {0}; 
    MMI_IMAGE_ID_T    img_id =0;
    GUI_LCD_DEV_INFO  lcd_dev_info = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
    GUI_POINT_T         time_point = {0};

    GUIRES_GetImgWidthHeight(&timebg_width, &timebg_height, IMG_CLOCK_TIME_BG, win_id);
    GUIRES_GetImgWidthHeight(&timeam_width, &timeam_height, IMG_CLOCK_AM, win_id);
    

    point.x= (win_rect.right - timebg_width)/2;
    point.y= win_rect.bottom - timebg_height;
    time_point.x = point.x+MMIALARM_CLOCK_TIME_OFFSET;
    time_point.y = point.y+(timebg_height-GUIFONT_GetHeight(MMIALARM_CLOCK_TIME_FONT))/2;
    
    GUIRES_DisplayImg(&point,
		PNULL,
		PNULL,
		win_id,
		IMG_CLOCK_TIME_BG,
		&lcd_dev_info);

    if(IsAmTime())
    {
        img_id = IMG_CLOCK_AM;
    }
    else
    {
        img_id = IMG_CLOCK_PM;
    }

    point.x = point.x + timebg_width - timeam_width - CLOCK_AM_GAP;
    point.y = point.y + (timebg_height - timeam_height)/2;
    
   
    //time_point.y = point.y-CLOCK_TIME_GAR;
    //time_point.y = point.y+(timebg_height-GUIFONT_GetHeight(MMIALARM_CLOCK_TIME_FONT))/2;
    GUIRES_DisplayImg(&point,
		PNULL,
		PNULL,
		win_id,
		img_id,
		&lcd_dev_info);
    s_clk_am_rect.left = point.x;
    s_clk_am_rect.top = point.y;
    s_clk_am_rect.right = s_clk_am_rect.left +timeam_width;
    s_clk_am_rect.bottom = s_clk_am_rect.top+ timeam_height;



    DrawTime(sys_time, time_point);
    return TRUE;
}
/*****************************************************************************/
//  Description : display clock
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DisplayAlmClock(MMI_WIN_ID_T win_id, SCI_TIME_T sys_time,GUI_RECT_T win_rect)
{
    GUI_RECT_T        lcd_rect = {0};
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

  


    //SCI_TRACE_LOW:"DisplayAlmClock rect = %d,%d,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_CLOCK_1150_112_2_18_2_1_16_59,(uint8*)"dddd",win_rect.left, win_rect.top, win_rect.right , win_rect.bottom);

    lcd_rect = MMITHEME_GetFullScreenRect();

    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    //SCI_TRACE_LOW:"MMIWIDGET_ACLOCK DisplayAClock lcd_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIALARM_CLOCK_1155_112_2_18_2_1_16_60,(uint8*)"d", lcd_dev_info.lcd_id);


	//TM_GetSysTime(&sys_time);  
	if (IsAmTime())
	{
		//aclock dial.
		aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
		GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMG_CLOCK_DAY, win_id);
		aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_CLOCK_DAY,win_id,&(aclock_input_param.clock_dial.data_size));
		aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
		aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2; 
	}
    else
	{
		//aclock dial.
		aclock_input_param.clock_dial.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
		GUIRES_GetImgWidthHeight(&aclock_input_param.clock_dial.width, &aclock_input_param.clock_dial.height, IMG_CLOCK_NIGHT, win_id);
		aclock_input_param.clock_dial.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_CLOCK_NIGHT,win_id,&(aclock_input_param.clock_dial.data_size));
		aclock_input_param.clock_dial.center_x = aclock_input_param.clock_dial.width/2;
		aclock_input_param.clock_dial.center_y = aclock_input_param.clock_dial.height/2; 
	}
    
    win_rect.left = win_rect.left + (win_rect.right - win_rect.left - aclock_input_param.clock_dial.width)/2;//0;
    win_rect.top = win_rect.top + (win_rect.bottom - win_rect.top - aclock_input_param.clock_dial.height)/2;
    win_rect.right = win_rect.right-win_rect.left;
    win_rect.bottom = win_rect.top + aclock_input_param.clock_dial.height; 
    //s_clk_bg_rect = win_rect;
	
//aclock hour hand
    aclock_input_param.hour_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.hour_handle.width, &aclock_input_param.hour_handle.height, IMG_CLOCK_HOUR_HAND, win_id);    
    aclock_input_param.hour_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_CLOCK_HOUR_HAND,win_id,&(aclock_input_param.hour_handle.data_size));
    aclock_input_param.hour_handle.center_x = aclock_input_param.hour_handle.width/2;
    aclock_input_param.hour_handle.center_y = aclock_input_param.hour_handle.height;//aclock_input_param.hour_handle.height/2;    

    //aclock min hand
    aclock_input_param.min_handle.data_type = IMGREF_FORMAT_COMPRESSED_ARGB;
    GUIRES_GetImgWidthHeight(&aclock_input_param.min_handle.width, &aclock_input_param.min_handle.height, IMG_CLOCK_MIN_HAND, win_id);        
    aclock_input_param.min_handle.data_ptr = (uint8 *)MMI_GetLabelImage(IMG_CLOCK_MIN_HAND,win_id,&(aclock_input_param.min_handle.data_size));
    aclock_input_param.min_handle.center_x = aclock_input_param.min_handle.width/2;
    aclock_input_param.min_handle.center_y = aclock_input_param.min_handle.height;//aclock_input_param.min_handle.height/2; 



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
        SCI_FREE(aclock_input_param.target_buf_ptr);     
        return FALSE;
    }
    
    bk_img.width = layer_info.mem_width;
    bk_img.height = layer_info.layer_height;
    
    fore_img.data_ptr = aclock_output_param.output_ptr;
    fore_img.data_type = IMGREF_FORMAT_ARGB888;
    fore_img.width = aclock_input_param.clock_dial.width;
    fore_img.height = aclock_input_param.clock_dial.height;


    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
    LCD_ConvertDispRect(GUILCD_CONVERT_POS_LCD2DISP, &lcd_dev_info, &win_rect, &clock_icon_rect);
    fore_img.start_x = clock_icon_rect.left;   
    fore_img.start_y = clock_icon_rect.top; 

    if (!ProcessAlphaBlending(&bk_img, &fore_img))
    {
        SCI_FREE(aclock_input_param.target_buf_ptr); 
        return FALSE;             
    }

    
    //in the end, display the center point.
    GUIRES_GetImgWidthHeight(&width, &height, IMG_CLOCK_CENTER, win_id);    
    point.x = win_rect.left + (aclock_input_param.clock_dial.width - width)/2;
    point.y = win_rect.top + (aclock_input_param.clock_dial.height - height)/2;
    
    GUIRES_DisplayImg(&point,
                      PNULL,
                      PNULL,
                      win_id,
                      IMG_CLOCK_CENTER,
                      &lcd_dev_info);

    //MMITHEME_StoreUpdateRect(&lcd_dev_info,win_rect);
        
    SCI_FREE(aclock_input_param.target_buf_ptr);
	

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
//  Description : get clock time.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIALARM_GetTime(uint8* hour, uint8* min)
{
    *hour =  s_alm_time.hour;
    *min = s_alm_time.min;
    if(s_is_clk_am)
    {
        if(*hour == 12)
        {
            *hour =0;
        }
    }
    
    else
    {
        if(*hour<12)
        {
        
            *hour = (*hour+12);
        }
    }

}
/*****************************************************************************/
//  Description : create clock window.
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIALARM_OpenAlmClockWin(SCI_TIME_T time, alarmClkSaveTimeFunc savetimefunc)
{
    s_alm_time =time;
    if(s_alm_time.hour <12)
    {
        if(s_alm_time.hour == 0)
        {
            s_alm_time.hour = 12;
        }
        s_is_clk_am = TRUE;
        
    }
    else if((s_alm_time.hour>=12) && (s_alm_time.hour < 13))
    {
        s_is_clk_am =FALSE;
    }
    else
    {
        s_is_clk_am =FALSE;
        s_alm_time.hour = s_alm_time.hour -12;
    }
    s_clk_savetime_func = savetimefunc;
    MMK_CreateWin((uint32*)MMIALM_CLOCK_TAB, PNULL);
}

/*****************************************************************************/
// Description : StartMoveTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartMoveTimer(uint8* time_id,MMI_HANDLE_T handle)
{
    if (PNULL != time_id)
    {
        if(0 != *time_id)
        {
            MMK_StopTimer(*time_id);
        }
        *time_id = MMK_CreateWinTimer(
            handle, 
            MMIALARM_CLOCK_MOVE_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopMoveTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopMoveTimer( uint8* time_id)
{
    if (0 != *time_id)
    {
        MMK_StopTimer(*time_id);
        *time_id= 0;
    }
}

#ifdef ATEST_SUPPORT
/*****************************************************************************/
// Description : set alarm time
// Global resource dependence : 
// Author:lin.chen
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ATESTALM_SetAlarmTime(SCI_TIME_T    time_val)
{
	MMI_WIN_ID_T win_id = MMK_GetFocusWinId();

	if ((time_val.hour < 24) && (time_val.min < 60) && (time_val.sec < 60))
    {
        s_alm_time.sec = time_val.sec;
		s_alm_time.min = time_val.min;
		s_alm_time.hour = time_val.hour;
		MMK_SendMsg(win_id,MSG_FULL_PAINT,PNULL);
		return TRUE;	
    }

	return FALSE;	
}
#endif	//ATEST_SUPPORT

#endif
#endif											   


/*Edit by script, ignore 2 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 2 case. Fri Apr 27 09:38:47 2012*/ //IGNORE9527
