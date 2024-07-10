/****************************************************************************
** File Name:      mmk_tp.c                                               *
** Author:                                                                 *
** Date:           12/09/2006                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 12/2006       wancan.you         Create
** 
****************************************************************************/
#ifdef TOUCH_PANEL_SUPPORT

#define MMK_TP_C
/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmi_kernel_trc.h"
#include "mmk_msg.h"
#include "mmk_msg_internal.h"
#include "mmi_osbridge.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "tb_dal.h"
#include "mmi_port.h"
#include "mmk_kbd_internal.h"
#include "mmi_default.h"
#include "mmi_theme.h"
#include "mmk_window_internal.h"
#include "mmk_tp.h"
#include "guilcd.h"
#include "tp_srv.h"
#include "mmi_autotest.h"
#include "mmk_app_internal.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_MAX_WAITING_TP           3    // 允许六个TP消息，即三组

#ifdef TOUCH_GESTURE_ENABLE
#define MMI_MAX_TP_MOVE				 6    // 允许6个TP MOVE或GESTURE消息
#else
#define MMI_MAX_TP_MOVE				 3
#endif

#ifdef TOUCH_GESTURE_ENABLE
#define MMI_MAX_TP_GESTURE          3
#endif

//#define MMI_TP_DOWN_INTERVAL		 50

#define MMI_MOVE_SPACING             3    // when two tp point is larger than this value, then tp move


#ifdef TOUCH_GESTURE_ENABLE
#define MMI_TP_DELAY_MOVE_SPACING    6
#else
// 防止电阻屏烂屏抖动
#if defined MAINLCD_SIZE_320X480 
#define MMI_TP_DELAY_MOVE_SPACING    24
#else
#define MMI_TP_DELAY_MOVE_SPACING    15
#endif

#endif

#define TP_CLOSE_WIN_MIN_SLIDE_LEN   20

#define MMI_TP_DBLCLK_MOVE			 18

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
typedef struct
{
    BOOLEAN is_hw_icon_enable[MMI_MAX_HARDWARE_ICON];
    MMI_HW_ICON_T *hardware_icon_ptr[MMI_MAX_HARDWARE_ICON];
    uint16 hw_icon_num;
}MMK_TP_HW_SHORTCUT_ICON_T;
#endif
// typedef enum
// {
// 	MMK_TPSTAT_NONE,
// 	MMK_TPSTAT_DOWN,
// 	MMK_TPSTAT_LONG		//长按状态
// }MMK_TPSTAT_E;

typedef enum
{
	TP_DBLCLK_NONE,
	TP_DBLCLK_PRE,
	TP_DBLCLK_DONE,
	
	TP_DBLCLK_MAX
}MMK_TP_DBLCLK_STATE_E;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
LOCAL BOOLEAN   s_is_hw_icon_enable = FALSE;
LOCAL MMK_TP_HW_SHORTCUT_ICON_T s_hardware_icon_ptr = {0};
#endif
LOCAL BOOLEAN   s_is_coordinating_tp = FALSE;

LOCAL GUI_POINT_T    s_tp_down_point = {0};

//for move, 判断tp move消息的参数是否和上一次的相同, 相同则不派发
LOCAL uint32    s_tp_point_x = 0;    
LOCAL uint32    s_tp_point_y = 0;
LOCAL uint32    s_physical_tp_x = 0;
LOCAL uint32    s_physical_tp_y = 0;

LOCAL uint8     s_tp_num = 0;           //消息队列中的TP Down/Up Num 
LOCAL uint16    s_tp_prev_code = MMI_TP_NONE;
LOCAL uint16    s_tp_move_num = 0;		//消息队列中的TP Move Num 
LOCAL uint8     s_tp_gesture_num = 0;   //消息队列中的TP Gesture NUM

LOCAL MMI_HANDLE_T s_tpdown_ctrl_handle = 0; //tp down消息点中的控件, tp up消息通常情况下也点中同一个控件
LOCAL MMI_HANDLE_T s_tpdown_win_handle = 0;  //tp down消息点中的窗口, tp up消息通常情况下也点中同一个窗口

LOCAL MMI_TP_STATUS_E    s_tp_status = MMI_TP_NONE; //TP当前状态，TP NONE/TP Down/TP Long
LOCAL BOOLEAN s_is_tp_move = FALSE;
LOCAL BOOLEAN s_is_delay_tp_move = FALSE;
LOCAL MMK_TP_DBLCLK_STATE_E s_tp_dblclk_state = TP_DBLCLK_NONE;
LOCAL MMI_HANDLE_T s_tp_down_win = 0;
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
uint16      g_tp_x = 0, g_tp_y = 0;//记录tp down时的坐标
BOOLEAN     g_tp_short_timer_active = FALSE;
BOOLEAN     g_tp_long_timer_active = FALSE;
BOOLEAN		g_tp_dblclk_timer_active = FALSE;
/**--------------------------------------------------------------------------*
 **                         External FUNCTION                                *
 **--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get multi key param
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void GetMultiKeyParam(MMI_MULTI_KEY_TP_MSG_PARAM_T *key_tp_msg_ptr);

#ifdef TOUCH_GESTURE_ENABLE
/*******************************************************************/
//  Interface:		ReduceHandleTPGestureNum
//  Description : 	ReduceHandleTPGestureNum
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ReduceHandleTPGestureNum(void);

/*****************************************************************************/
//     Description : 手势消息适配
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_GESTURE_TYPE_E ConvertGesureType(
                                           uint16 gesture_type
                                           );

/*****************************************************************************/
//     Description : 发送手势消息
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpGesure(
                               uint16 gesture_type,
                               uint16 x, 
                               uint16 y
                               );
#endif

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpDown(
                             uint16 x,
                             uint16 y
                             );

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpUp(
                          uint16 x,
                          uint16 y
                          );
                                        
/*****************************************************************************/
// Description : dispatch the touch panel message after translate the signal of RTOS to
//        MMI Message
// Global resource dependence : 
// Author:Robert Lu
// Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpMove(
                             uint16     pre_point_x,
                             uint16     pre_point_y,
                             uint16     cur_point_x,
                             uint16     cur_point_y
                             );  

/*****************************************************************************/
//     Description : default process the touch pannel message
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DefaultProcessTPMsg(
                                       MMI_HANDLE_T        win_handle,
                                       MMI_MESSAGE_ID_E    msg_id,   // the message id
                                       DPARAM              param     // the param
                                       );

/*****************************************************************************/
//     Description : default process the window touch pannel message
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DefaultProcessWinTPMsg(
                                          MMI_HANDLE_T        win_handle, // the window pointer
                                          MMI_MESSAGE_ID_E    msg_id,   // the message id
                                          DPARAM              param     // the param
                                          );

/*****************************************************************************/
//  Description : check if handle tp
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckHandleTp(
                            uint16  state   //tp down or up
                            );

/*****************************************************************************/
// 	Description : reduce the handle TP num 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void ReduceHandleTPNum(void);

/*****************************************************************************/
// 	Description : reduce the handle TP num 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void ReduceHandleTPMoveNum(void);

/*****************************************************************************/
// 	Description : set TP down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void SetTPDownCtrl(
                         MMI_HANDLE_T  ctrl_handle
                         );

/*****************************************************************************/
// 	Description : get TP down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetTPDownCtrl(void);

/*****************************************************************************/
// 	Description : check tp pair
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:默认情况下，tp必须配对，若应用设置handle为0，可不配对
/*****************************************************************************/
LOCAL BOOLEAN CheckTPPair( 
                          MMI_HANDLE_T  win_handle
                          );

/*****************************************************************************/
// Description : 处理应图标快捷菜单
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
// MMI_MESSAGE_ID_E msg_id: 消息id
// DPARAM param: 参数
// GUI_POINT_T point: pressed point
// BOOLEAN is_deal_msg:是否处理msg_id
/*****************************************************************************/
LOCAL MMI_RESULT_E ResponseShortCutBar(
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param, 
                                       GUI_POINT_T point,
                                       BOOLEAN is_deal_msg
                                       );

/*****************************************************************************/
// 	Description : check whe this is a valuable tp move point
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
 LOCAL BOOLEAN IsTpMove(
                        MmiTPPress  *tppress_ptr   // [in]
                        );


/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : 
//  Global resource dependence : 
//  Author:Cheng.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMK_ReInitDblClkState(BOOLEAN is_reset)
{
	s_tp_dblclk_state = TP_DBLCLK_NONE;
	
	//SCI_TRACE_LOW("@DBLCLK, MMK_ReInitDblClkState.\n");
	if (g_tp_dblclk_timer_active)
	{
		MMK_StopTimer( MMI_TP_DBLCLK_TIMER_ID );
		g_tp_dblclk_timer_active = FALSE;
	}
	if (is_reset
        && MMK_GetWinDisplayStyleState(MMK_GerFocusMainWinHandle(), WS_NEED_DBLCLK))
	{
		g_tp_dblclk_timer_active = MMK_StartWinTimer(
			MMK_GetFirstAppletHandle(),
			MMI_TP_DBLCLK_TIMER_ID,
			MILLISECONDS_TO_TICKS(MMI_TP_DBLCLK_PRESS_VALUE),
			FALSE);
	}
}

/*****************************************************************************/
// Description : dispatch the touch panel message after translate the signal of RTOS to
//                MMI Message
//  Global resource dependence : 
//  Author:Robert Lu
//  Note:
/*****************************************************************************/
void MMK_DispatchMSGTp(
                       MmiTPPress   *signal_ptr // the pointer of message which need dispatch
                       )
{
    //uint32          para  = 0;
    MmiTPPress      *tppress_ptr = PNULL;
    
    //SCI_ASSERT( PNULL != signal_ptr );
    if ( PNULL == signal_ptr )
    {
        return;
    }
    
    //cr237201,MMI task
    //tp down，tp task callback函数调用MMK_CheckKeySingle()发现没有key消息,因此创建了single,排入mmi对列。
    //red key down, key task callback函数调用MMK_CheckTPSignale()发现没有消息,因此创建了single,排入mmi对列。
    //MMI收到red key up消息，进行处理，一键灭屏，进入deep sleep状态。
    //tp task发现deep sleep状态就不会再发送任何消息了,其中也包括那个tp up消息！
    //MMI task，对key，tp是否能够处理再判断一次
    if (!CheckHandleTp(signal_ptr->SignalCode))
    {
        return;
    }

    tppress_ptr = (MmiTPPress*)signal_ptr;
    //para = tppress_ptr->x | (tppress_ptr->y << 16);  

    //SCI_TRACE_LOW:"MMK_DispatchMSGTp: x = %d, y = %d, gesture_type = %d, sig_code = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_330_112_2_18_3_22_46_25,(uint8*)"dddd",tppress_ptr->x, tppress_ptr->y, tppress_ptr->gesture_type, signal_ptr->SignalCode );
    
    //convert tp point
    GUILCD_ConvertTpPoint(&tppress_ptr->x,&tppress_ptr->y,GUI_MAIN_LCD_ID);
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
    GUILCD_ConvertTpPointEx(&tppress_ptr->x,&tppress_ptr->y,GUI_MAIN_LCD_ID);
#endif
    g_tp_x = (uint16)tppress_ptr->x;
    g_tp_y = (uint16)tppress_ptr->y;

    //停止repeat按键timer
    MMK_StopKeyRepeatPressTimer();

    MMI_TraceAutoTestOfTpPress(tppress_ptr->x, tppress_ptr->y, tppress_ptr->gesture_type, signal_ptr->SignalCode);

    switch (signal_ptr->SignalCode)
    {
    //熄屏状态，TP点亮屏幕
    case MMI_TP_GESTURE_CLICK:
        MMK_DefaultMSGKtp(MMI_TP_GESTURE_CLICK);
        break;

    case MMI_TP_DOWN:
        //防止烂屏抖动
        MMK_DelayTpMove( TRUE );

        MMK_AddVelocityItemExt( tppress_ptr->x, tppress_ptr->y, MMI_TP_DOWN, tppress_ptr->tick_count); // 增加速度的点
        if (!MMK_DefaultMSGKtp(MSG_TP_PRESS_DOWN))
        {
            break;
        }

        //start tp short timer
        g_tp_short_timer_active = MMK_StartWinTimer(MMK_GetFirstAppletHandle(),
                                        MMI_TP_SHORT_TIMER_ID,
                                        MILLISECONDS_TO_TICKS(MMI_TP_SHORT_PRESS_VALUE),
                                        FALSE);

        //start tp long timer
        g_tp_long_timer_active = MMK_StartWinTimer(
                                    MMK_GetFirstAppletHandle(),
                                    MMI_TP_LONG_TIMER_ID,
                                    MILLISECONDS_TO_TICKS(MMI_TP_LONG_PRESS_VALUE),
                                    FALSE);

        //SCI_TRACE_LOW:"MMK:MMK_DispatchMSGKTp tp down timer state %d, %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_368_112_2_18_3_22_46_26,(uint8*)"dd",g_tp_short_timer_active,g_tp_long_timer_active);

        DispatchMSGTpDown((uint16)tppress_ptr->x,(uint16)tppress_ptr->y);

        //set down point
        s_tp_point_x = tppress_ptr->x;
        s_tp_point_y = tppress_ptr->y;

        s_tp_down_point.x= tppress_ptr->x;
        s_tp_down_point.y= tppress_ptr->y;

        break;

    case MMI_TP_UP:
        MMK_AddVelocityItemExt( tppress_ptr->x, tppress_ptr->y, MMI_TP_UP, tppress_ptr->tick_count ); // 增加速度的点
        ReduceHandleTPNum();

        if (!MMK_DefaultMSGKtp(MSG_TP_PRESS_UP))
        {
            break;
        }

        //stop short timer
        if (g_tp_short_timer_active)
        {
            MMK_StopTimer(MMI_TP_SHORT_TIMER_ID);
            g_tp_short_timer_active = FALSE;
        }

        //stop long timer
        if (g_tp_long_timer_active)
        {
            MMK_StopTimer( MMI_TP_LONG_TIMER_ID );
            g_tp_long_timer_active = FALSE;
        }

        DispatchMSGTpUp((uint16)tppress_ptr->x, (uint16)tppress_ptr->y);
        break;

    case MMI_TP_MOVE:
        MMK_AddVelocityItemExt( tppress_ptr->x, tppress_ptr->y, MMI_TP_MOVE , tppress_ptr->tick_count); // 增加速度的点
		ReduceHandleTPMoveNum();

        if (!MMK_DefaultMSGKtp(MSG_TP_PRESS_MOVE))
        {
            break;
        }

        // check whether it is a tp move point
        if (IsTpMove(tppress_ptr))
        {
			//防抖
			if (abs(tppress_ptr->x - s_tp_point_x) > MMI_TP_DBLCLK_MOVE
				|| abs(tppress_ptr->y - s_tp_point_y) > MMI_TP_DBLCLK_MOVE)
			{
				MMK_ReInitDblClkState(FALSE);
			}			

            //restart the tp short timer
            if (g_tp_short_timer_active)
            {
                MMK_StopTimer(MMI_TP_SHORT_TIMER_ID);
                g_tp_short_timer_active = FALSE;
            }

            //restart the tp long timer
            if (g_tp_long_timer_active)
            {
                MMK_StopTimer( MMI_TP_LONG_TIMER_ID );
                g_tp_long_timer_active = FALSE;

                // 拖动之后不需要发长按消息，不过可能会有误差，如手指抖动，
                ////start tp long timer
                //g_tp_long_timer_active = MMK_StartWinTimer(
                //                            MMK_GetFirstAppletHandle(),
                //                            MMI_TP_LONG_TIMER_ID,
                //                            MILLISECONDS_TO_TICKS(MMI_TP_LONG_PRESS_VALUE),
                //                            FALSE);
            }

            DispatchMSGTpMove((uint16)s_tp_point_x,
                (uint16)s_tp_point_y,
                (uint16)tppress_ptr->x,
                (uint16)tppress_ptr->y);

            s_tp_point_x = tppress_ptr->x;
            s_tp_point_y = tppress_ptr->y;
        }
        break;  

#ifdef TOUCH_GESTURE_ENABLE
    case MMI_TP_GESTURE:
        ReduceHandleTPGestureNum();

        if (!MMK_DefaultMSGKtp(MSG_TP_GESTURE))
        {
            break;
        }

		MMK_ReInitDblClkState(FALSE);

        //restart the tp short timer
        if (g_tp_short_timer_active)
        {
            MMK_StopTimer(MMI_TP_SHORT_TIMER_ID);
            g_tp_short_timer_active = FALSE;
        }

        //restart the tp long timer
        if (g_tp_long_timer_active)
        {
            MMK_StopTimer( MMI_TP_LONG_TIMER_ID );
            g_tp_long_timer_active = FALSE;
        }
		
        DispatchMSGTpGesure( tppress_ptr->gesture_type, (uint16)tppress_ptr->x, (uint16)tppress_ptr->y );
        break;
#endif

    default:
		//SCI_PASSERT(FALSE,("MMK_DispatchMSGTp: signalCode %d is error!",signal_ptr->signalCode));
        break;
    }
}

/*****************************************************************************/
//     Description : tp init
//    Global resource dependence : 
//  Author:Cheng.luo
//    Note:
/*****************************************************************************/
PUBLIC void MMK_InitTp(void)
{
#if defined TOUCH_GESTURE_ENABLE && !defined WIN32
	uint16 speed_mode = MMI_MOVE_SPACING;
	TP_Ioctl(TP_IOCTL_SET_THRES_ZOOM, (uint16 *)&speed_mode);
#endif
}

#ifdef TOUCH_GESTURE_ENABLE
/*******************************************************************/
//  Interface:		ReduceHandleTPGestureNum
//  Description : 	ReduceHandleTPGestureNum
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void ReduceHandleTPGestureNum(void)
{
    if (0 < s_tp_gesture_num)
    {
        s_tp_gesture_num--;
    }
}

/*****************************************************************************/
//     Description : 手势消息适配
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_GESTURE_TYPE_E ConvertGesureType(
                                           uint16 gesture_type
                                           )
{
    MMI_GESTURE_TYPE_E type = MMI_GESTURE_NONE;

    switch( gesture_type )
    {
    case TG_ZOOM_IN:
        type = MMI_GESTURE_ZOOM_IN;
    	break;
    case TG_ZOOM_OUT:
        type = MMI_GESTURE_ZOOM_OUT;
        break;

    case TG_ROTATE_LEFT:
        type = MMI_GESTURE_ROTATE_LEFT;
        break;
        
    case TG_ROTATE_RIGHT:
        type = MMI_GESTURE_ROTATE_RIGHT;
        break;

    default:
        break;
    }
    
    return type;
}

/*****************************************************************************/
//     Description : 发送手势消息
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpGesure(
                               uint16 gesture_type,
                               uint16 x, 
                               uint16 y
                               )
{
    MMI_RESULT_E               tp_handle_result    = MMI_RESULT_FALSE;
    MMI_GESTURE_TYPE_E         type                = ConvertGesureType( gesture_type );
    MMI_HANDLE_T               ctrl_handle         = GetTPDownCtrl();
    MMI_HANDLE_T               win_handle          = ( 0 == ctrl_handle ) 
        ? MMK_GetFocusWinHandle() : MMK_GetWinHandleByCtrl(ctrl_handle );
    MMI_HANDLE_T               old_handle          = MMK_GetProcMsgHandle();
    MMI_MULTI_KEY_TP_MSG_PARAM_T   para               = {0};
    MMI_HANDLE_T               parent_handle=0;

    para.cur_tp_point.x = x;
    para.cur_tp_point.y = y;
    para.gesture_type = type;
    
    GetMultiKeyParam( &para );
    
    if ( !CheckTPPair( win_handle ) )
    {
        return;
    }

    MMK_SetProcMsgHandle( win_handle );
    
    //default dispatch to ctrl
    tp_handle_result = DefaultProcessTPMsg( win_handle, MSG_TP_GESTURE, (DPARAM)&para );
    
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        //dispatch the window 
        tp_handle_result = MMK_RunWinProc( win_handle, MSG_TP_GESTURE, (DPARAM)&para);
        
        //dispatch to parent window
        if ( MMI_RESULT_FALSE == tp_handle_result 
            && 0 != ( parent_handle = MMK_GetParentWinHandle( win_handle ) ) )
        {
            tp_handle_result = MMK_RunWinProc( parent_handle, MSG_TP_GESTURE, (DPARAM)&para);
        }
    }

    MMK_SetProcMsgHandle( old_handle );
}
#endif

/*****************************************************************************/
//     Description : 获得多点信息
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetTpPoints( 
                               MMI_TP_POINTS_T* points_ptr
                               )
{
    BOOLEAN result = FALSE;

    if ( PNULL != points_ptr )
    {
#if defined TOUCH_GESTURE_ENABLE && !defined WIN32
        TPDSVR_SIG_T tp_sig = {0};

        SCI_MEMSET( points_ptr, 0, sizeof(MMI_TP_POINTS_T) );

        if ( SCI_SUCCESS == TP_Read( &tp_sig ) )
        {
            int32 i = 0;

            points_ptr->num = MIN( tp_sig.num_of_point, MMI_TP_POINTS_MAX );

            points_ptr->points[0].x = tp_sig.x_key;
            points_ptr->points[0].y = tp_sig.y_key;
            for ( i = 1; i < points_ptr->num; i++ )
            {
                points_ptr->points[i].x = tp_sig.xy_position[i-1].x_position;
                points_ptr->points[i].y = tp_sig.xy_position[i-1].y_position;
            }
            result = TRUE;
        }
#else
        SCI_MEMSET( points_ptr, 0, sizeof(MMI_TP_POINTS_T) );

        if ( MMI_TP_DOWN != s_tp_status )
        {
            points_ptr->points->x = g_tp_x;
            points_ptr->points->y = g_tp_y;
            points_ptr->num = 1;
            
            result = TRUE;
        }
#endif
    }

    return result;
}

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpDown(
                             uint16 x,
                             uint16 y
                             )
{
    MMI_HANDLE_T                   win_handle         = 0;
    MMI_HANDLE_T                   root_handle        = 0;
    MMI_HANDLE_T                   ctrl_handle        = 0;
    GUI_POINT_T                    point              = {0};
    MMI_MULTI_KEY_TP_MSG_PARAM_T   para               = {0};
    MMI_RESULT_E                   tp_handle_result = MMI_RESULT_FALSE; 
	MMI_MESSAGE_ID_E               msg_id			= MSG_TP_PRESS_DOWN;
	MMI_HANDLE_T				   focus_win		= MMK_GetFocusWinHandle();
    MMI_HANDLE_T                   old_handle       = MMK_GetProcMsgHandle();

    s_tp_status = MMI_TP_DOWN;

    point.x = x;
    point.y = y;

    para.cur_tp_point.x = x;
	para.cur_tp_point.y = y;

	GetMultiKeyParam( &para );

    if( !MMK_GetHandleByPos( point, TRUE, &win_handle, &ctrl_handle, 0 ) )
    {
		s_tp_dblclk_state = TP_DBLCLK_NONE;
        return;
    }

    MMK_SetTPDownWin( win_handle );

    if (MMK_GetWinDisplayStyleState(MMK_GerFocusMainWinHandle(), WS_NEED_DBLCLK))
    {
        // start dblclk timer
        if(TP_DBLCLK_NONE == s_tp_dblclk_state)
        {
            MMK_ReInitDblClkState(TRUE);
        }
        
        // for dblclk
#if !defined RELEASE_INFO
        {
            static uint32 s_tp_down_tick = 0;
            uint32 cur_tp_down_tick = SCI_GetTickCount();
            
            //SCI_TRACE_LOW:"@DBLCLK, tick = %u."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_690_112_2_18_3_22_47_27,(uint8*)"u", cur_tp_down_tick - s_tp_down_tick);
            s_tp_down_tick = cur_tp_down_tick;
        }
#endif
        //SCI_TRACE_LOW:"@DBLCLK, s_tp_dblclk_state = %u, focus_win = 0x%x, s_tp_down_win = 0x%x, offset_x = %u, offset_y = %u n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_695_112_2_18_3_22_47_28,(uint8*)"udduu", s_tp_dblclk_state, focus_win, s_tp_down_win, abs(x - s_tp_point_x), abs(y - s_tp_point_y));
        
        if(TP_DBLCLK_NONE == s_tp_dblclk_state)
        {
            s_tp_dblclk_state = TP_DBLCLK_PRE;
            msg_id			  = MSG_TP_PRESS_DOWN;
            //SCI_TRACE_LOW:"@DBLCLK, MSG_TP_PRESS_DOWN"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_701_112_2_18_3_22_47_29,(uint8*)"");
        }
        else if(TP_DBLCLK_PRE == s_tp_dblclk_state
            && focus_win == s_tp_down_win
            && abs(x - s_tp_point_x) <= MMI_TP_DBLCLK_MOVE
            && abs(y - s_tp_point_y) <= MMI_TP_DBLCLK_MOVE)
        {
            MMK_ReInitDblClkState(FALSE);
            s_tp_dblclk_state = TP_DBLCLK_DONE;
            msg_id			  = MSG_TP_PRESS_DOUBLE;
            //SCI_TRACE_LOW:"@DBLCLK, MSG_TP_PRESS_DOUBLE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_711_112_2_18_3_22_47_30,(uint8*)"");
        }
        else
        {
            MMK_ReInitDblClkState(TRUE);
            s_tp_dblclk_state = TP_DBLCLK_PRE;
            msg_id			  = MSG_TP_PRESS_DOWN;
            //SCI_TRACE_LOW:"@DBLCLK, MSG_TP_PRESS_DOWN"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_718_112_2_18_3_22_47_31,(uint8*)"");
        }
        s_tp_down_win = focus_win;
    }
		
	//set tp mode
	MMITHEME_SetTiggerMode(MMITHEME_TRIGGER_MODE_TP);

    MMK_SetProcMsgHandle( win_handle );

    // check whether user hook handler
    tp_handle_result = MMK_RunWinHookProc(win_handle, msg_id, (DPARAM)&para);
    if (MMI_RESULT_FALSE != tp_handle_result)
    {
        SetTPDownCtrl( 0 );
    }
    else    // check form handler
    {
        //get root form control handle
        root_handle = MMK_GetRootFormCtrlHandle(win_handle);
        if(!(MMK_PosIsCtrl(root_handle, point) && MMK_IsResponseToClick(root_handle)))
        {
            root_handle = 0;
        }

        if (0 != root_handle)
        {
            SetTPDownCtrl(root_handle);

            tp_handle_result = MMK_RunCtrlProc(root_handle, msg_id, (DPARAM)&para);
        }
    }

    // check ctrl handler
    if (MMI_RESULT_FALSE == tp_handle_result)
    {
        if ( 0 != ctrl_handle )
        {        
            SetTPDownCtrl(ctrl_handle);
        
            MMK_SetAtvCtrl(win_handle, ctrl_handle);
        
            tp_handle_result = MMK_RunCtrlProc(ctrl_handle, msg_id, (DPARAM)&para);        
        }
        else
        {
            SetTPDownCtrl( 0 );
            //call the default proces function to 
            tp_handle_result = DefaultProcessWinTPMsg( win_handle, msg_id, (DPARAM)&para );
        }
    }

    // check window callback handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        //dispatch the window 
        tp_handle_result = MMK_RunWinProc( win_handle, msg_id, (DPARAM)&para);
    }

    MMK_SetProcMsgHandle( old_handle );
}

/*****************************************************************************/
//     Description : default process the touch pannel message
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DefaultProcessTPMsg(
                                       MMI_HANDLE_T        win_handle,
                                       MMI_MESSAGE_ID_E    msg_id,   // the message id
                                       DPARAM              param     // the param
                                       )
{
    MMI_RESULT_E result         = MMI_RESULT_FALSE; 
    MMI_HANDLE_T tp_down_handle = 0;

    if ( (0 != ( tp_down_handle = GetTPDownCtrl() )) )//发送给获得TPdown的控件处理
    {	
        if ( MMI_HANDLE_CONTROL == MMK_GetHandleType( tp_down_handle ) )
        {
            if ( MMK_IsResponseToClick(tp_down_handle) )
            {
                //按住tp切换窗口, 再松开tp时, 丢弃该消息, 避免花屏.
                if( !MMK_IsFocusWin( MMK_GetWinHandleByCtrl( tp_down_handle ) ) )
                {
                    SetTPDownCtrl( 0 );

        //            result = MMI_RESULT_TRUE;
                    result = MMI_RESULT_FALSE;
                    
                    //SCI_TRACE_LOW:"DefaultProcessTPMsg, down ctrl is not focus!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_807_112_2_18_3_22_47_32,(uint8*)"");
                }
                else
                {
                    result = MMK_RunCtrlProc( tp_down_handle, msg_id, param );
                }
            }
        }
        //控件已经不存在
        else
        {
            SetTPDownCtrl( 0 );

            result = MMI_RESULT_TRUE;

            //SCI_TRACE_LOW:"DefaultProcessTPMsg, down ctrl is not exist!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_822_112_2_18_3_22_47_33,(uint8*)"");
        }
    }
    else
    {
        result = DefaultProcessWinTPMsg( win_handle, msg_id, param );
    }

    return result;
}
/*****************************************************************************/
//  Description : compute tp fling direction
//  Global resource dependence :
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL MMI_TP_FLING_DIRECTION_E ComputeTPFlingDirection(
                                                    GUI_POINT_T   down_point ,
                                                    GUI_POINT_T   cur_point
                                                    )
{

    float               x_fling_velocity = 0;
    float               y_fling_velocity = 0;
    VELOCITY_TRACKER_T  velocity_tracker = {0};
    BOOLEAN             is_move          = FALSE;
    BOOLEAN             is_valid_move    = FALSE;
    //判断是否达到一定速度
    MMK_GetCurrentVelocityTracker(&velocity_tracker);//1367534
    MMK_ComputeCurrentVelocityByTracker(&velocity_tracker,&x_fling_velocity,&y_fling_velocity,FLING_MAX_VELOCITY,FLING_MAX_VELOCITY);
    if((abs(y_fling_velocity) < FLING_MIN_VELOCITY)&&(abs(x_fling_velocity) < FLING_MIN_VELOCITY))
    {
         return TP_FLING_NONE;
    }
    //获取滑动方向
    if(abs(y_fling_velocity)>abs(x_fling_velocity))
    {
          //判断是否有效纵向移动
          is_move = abs(cur_point.y-down_point.y) > TP_CLOSE_WIN_MIN_SLIDE_LEN;
          is_valid_move = abs(cur_point.y-down_point.y) > abs(cur_point.x-down_point.x);
          if((FALSE == is_move)||(is_valid_move == FALSE))
          {
               return TP_FLING_NONE;
          }
          //返回方向
          return y_fling_velocity < 0 ? TP_FLING_UP:TP_FLING_DOWN;
    }
    else
    {
          //判断是否有效横向移动
          is_move = abs(cur_point.x - down_point.x) > TP_CLOSE_WIN_MIN_SLIDE_LEN;
          is_valid_move = abs(cur_point.x - down_point.x) > abs(cur_point.y - down_point.y);
          if((FALSE == is_move)||(is_valid_move == FALSE))
          {
               return TP_FLING_NONE;
          }
          //返回方向
          return x_fling_velocity < 0 ? TP_FLING_LEFT:TP_FLING_RIGHT;
    }
}
/*****************************************************************************/
//  Description : dispatch the touch pannel fing message
//  Global resource dependence :
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchMSGTpFling(
                                       MMI_HANDLE_T              win_handle,
                                       MMI_TP_FLING_DIRECTION_E  fling_direction,
                                       GUI_POINT_T               point
                                       )
{
    MMI_RESULT_E                 tp_handle_result    = MMI_RESULT_FALSE; 
    MMI_TP_MSG_PARAM_T           para                = {0};
    MMI_HANDLE_T                 old_handle          = MMK_GetProcMsgHandle();
    para.up_tp_point.x = point.x;
    para.up_tp_point.y = point.y;
    para.down_tp_point.x = s_tp_down_point.x;
    para.down_tp_point.y = s_tp_down_point.y;
    para.tp_fling_direction = fling_direction;
    MMK_SetProcMsgHandle( win_handle );

    // check whether user hook handler
    tp_handle_result = MMK_RunWinHookProc(win_handle, MSG_TP_PRESS_FLING, (DPARAM)&para);

    // check ctrl handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        tp_handle_result = DefaultProcessTPMsg( win_handle, MSG_TP_PRESS_FLING, (DPARAM)&para );
    }

    // check window callback handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        //dispatch the window
        tp_handle_result = MMK_RunWinProc( win_handle, MSG_TP_PRESS_FLING, (DPARAM)&para );
        if (MMI_RESULT_FALSE == tp_handle_result)
        {
            tp_handle_result = DefaultProcessWinTPMsg( win_handle, MSG_TP_PRESS_FLING, (DPARAM)&para );
        }
    }
    MMK_SetProcMsgHandle( old_handle );
    return tp_handle_result;
}
/*****************************************************************************/
//  Description : default process tp fling MSG
//  Global resource dependence :
//  Author:longwei.qiao
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DefaultProcessTPFlingMSG(
                                       MMI_HANDLE_T             win_handle,
                                       MMI_TP_FLING_DIRECTION_E fling_direction
                                       )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    switch(fling_direction)
    {
           /*系统默认从左往右滑屏退出当前窗口：
             1.横向滑动距离大于纵向滑动距离;
             2.滑动方向是左到右;
             3.滑动速度大于最小初始速度;
             同时满足上述3个条件，认为是一次有效操作.
             注意，对一些特殊窗口(WS_DISABLE_RETURN_WIN)不起作用.*/
        case TP_FLING_RIGHT:
            if(FALSE == MMK_GetWinDisplayStyleState(win_handle , WS_DISABLE_RETURN_WIN )
                && FALSE == MMK_GetWinDisplayStyleState(win_handle , WS_DISABLE_FLING_CLOSE_WIN ))
            {
                MMK_CloseWin(win_handle);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
            break;
        case TP_FLING_LEFT:
        case TP_FLING_UP:
        case TP_FLING_DOWN:
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpUp(
                          uint16 x,
                          uint16 y
                          )
{
    MMI_HANDLE_T                    win_handle       = 0;
    GUI_POINT_T                     point            = {0};
    MMI_RESULT_E                    tp_handle_result = MMI_RESULT_FALSE;
    MMI_RESULT_E                    tp_fling_handle_result = MMI_RESULT_FALSE;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    para             = {0};
    MMI_MESSAGE_ID_E                msg_id           = MSG_TP_PRESS_UP;
    MMI_HANDLE_T                    old_handle       = MMK_GetProcMsgHandle();
    MMI_TP_FLING_DIRECTION_E        fling_direction = TP_FLING_NONE;
    s_tp_status = MMI_TP_NONE;

    s_is_tp_move = FALSE;
    s_is_delay_tp_move = FALSE;

    point.x = x;
    point.y = y;

    para.cur_tp_point.x = x;
    para.cur_tp_point.y = y;

	GetMultiKeyParam( &para );

    if( !MMK_GetHandleByPos( point, TRUE, &win_handle, PNULL, 0 ) )
    {
		s_tp_dblclk_state = TP_DBLCLK_NONE;
        return;
    }

    if ( !CheckTPPair( win_handle ) )
    {
        return;
    }
    // check whether user fling msg
    fling_direction = ComputeTPFlingDirection( s_tp_down_point, point);
    if(TP_FLING_NONE != fling_direction)
    {
         tp_fling_handle_result = DispatchMSGTpFling(win_handle, fling_direction, point);

         if(MMI_RESULT_FALSE == tp_fling_handle_result)
         {
             tp_fling_handle_result = DefaultProcessTPFlingMSG(win_handle, fling_direction);
             if(MMI_RESULT_TRUE == tp_fling_handle_result)
             {
                 SCI_TRACE_LOW("DispatchMSGTpFling default process:close cur win");
                 return;
             }
         }
    }
    para.tp_fling_direction = fling_direction;
    // dblclk
    if (MMK_GetWinDisplayStyleState(MMK_GerFocusMainWinHandle(), WS_NEED_DBLCLK))
    {
        if (TP_DBLCLK_DONE == s_tp_dblclk_state)
        {
            msg_id = MSG_TP_PRESS_DBLCLK_UP;
            s_tp_dblclk_state = TP_DBLCLK_NONE;
            //SCI_TRACE_LOW:"@DBLCLK, MSG_TP_PRESS_DBLCLK_UP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_881_112_2_18_3_22_47_34,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"@DBLCLK, MSG_TP_PRESS_UP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_885_112_2_18_3_22_47_35,(uint8*)"");
        }
    }

    MMK_SetProcMsgHandle( win_handle );

    // check whether user hook handler
    tp_handle_result = MMK_RunWinHookProc(win_handle, msg_id, (DPARAM)&para);

    // check ctrl handler
    if (MMI_RESULT_FALSE == tp_handle_result)
    {
        tp_handle_result = DefaultProcessTPMsg( win_handle, msg_id, (DPARAM)&para );
    }

    SetTPDownCtrl(0);
    MMK_SetTPDownWin( 0 );

    // check window callback handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        //dispatch the window 
        tp_handle_result = MMK_RunWinProc( win_handle, msg_id, (DPARAM)&para);                                                    
        if (MMI_RESULT_FALSE == tp_handle_result)
        {
            tp_handle_result = DefaultProcessWinTPMsg( win_handle, msg_id, (DPARAM)&para);
        }
    }

    MMK_SetProcMsgHandle( old_handle );
}

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
LOCAL void DispatchMSGTpMove(
                             uint16     pre_point_x,
                             uint16     pre_point_y,
                             uint16     cur_point_x,
                             uint16     cur_point_y
                             )
{
    MMI_HANDLE_T                 win_handle          = 0;
    GUI_POINT_T                  point               = {0};
    MMI_RESULT_E                 tp_handle_result    = MMI_RESULT_FALSE; 
    MMI_MULTI_KEY_TP_MSG_PARAM_T para                = {0};
    MMI_HANDLE_T                 old_handle          = MMK_GetProcMsgHandle();
    
    s_is_tp_move = TRUE;

    //set point
    point.x = cur_point_x;
    point.y = cur_point_y;

    //set parameter
    para.cur_tp_point.x = cur_point_x;
	para.cur_tp_point.y = cur_point_y;
    para.pre_tp_point.x = pre_point_x;
    para.pre_tp_point.y = pre_point_y;
	GetMultiKeyParam(&para);
	
    if( !MMK_GetHandleByPos( point, TRUE, &win_handle, PNULL, 0 ) )
    {
        return;
    }

    if ( !CheckTPPair( win_handle ) )
    {
        return;
    }

    MMK_SetProcMsgHandle( win_handle );
    
    // check whether user hook handler
    tp_handle_result = MMK_RunWinHookProc(win_handle, MSG_TP_PRESS_MOVE, (DPARAM)&para);

    // check ctrl handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        tp_handle_result = DefaultProcessTPMsg( win_handle, MSG_TP_PRESS_MOVE, (DPARAM)&para );
    }

    // check window callback handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        tp_handle_result = MMK_RunWinProc( win_handle, MSG_TP_PRESS_MOVE, (DPARAM)&para);
    }

    MMK_SetProcMsgHandle( old_handle );
}


/*****************************************************************************/
//  Description : dispatch the tp short long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchMSGTpShort(
                                   uint16   x,
                                   uint16   y
                                   )
{
    GUI_POINT_T                     tp_point = {0};
    MMI_HANDLE_T                    old_handle = MMK_GetProcMsgHandle();
    MMI_HANDLE_T                    win_handle = {0};
    MMI_RESULT_E                    result = MMI_RESULT_FALSE;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    param = {0};

    //set tp point
    tp_point.x = x;
    tp_point.y = y;

    //get handle
    if (MMK_GetHandleByPos(tp_point,TRUE,&win_handle,PNULL,0))
    {
        if ( !CheckTPPair( win_handle ) )
        {
            return;
        }

        //set process handle
        MMK_SetProcMsgHandle(win_handle);

        //get multi param
        param.cur_tp_point.x = x;
        param.cur_tp_point.y = y;
        GetMultiKeyParam(&param);

        // check whether user hook handler
        result = MMK_RunWinHookProc(win_handle,MSG_TP_PRESS_SHORT,(DPARAM)&param);

        // check ctrl handler
        if (MMI_RESULT_FALSE == result)
        {
            result = DefaultProcessTPMsg(win_handle,MSG_TP_PRESS_SHORT,(DPARAM)&param);
        }

        // check window callback handler
        if (MMI_RESULT_FALSE == result)
        {
            //dispatch to win
            result = MMK_RunWinProc(win_handle,MSG_TP_PRESS_SHORT,(DPARAM)&param);
            if (MMI_RESULT_FALSE == result)
            {
                result = DefaultProcessWinTPMsg(win_handle,MSG_TP_PRESS_SHORT,(DPARAM)&param);
            }
        }

        //reset process handle
        MMK_SetProcMsgHandle(old_handle);
    }
}

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchMSGTpLong(
                                  uint16 x,
                                  uint16 y
                                  )
{
    MMI_HANDLE_T                 win_handle          = 0;
    GUI_POINT_T                  point               = {0};
    MMI_RESULT_E                 tp_handle_result    = MMI_RESULT_FALSE; 
    MMI_MULTI_KEY_TP_MSG_PARAM_T para                = {0};
    MMI_HANDLE_T                  old_handle         = MMK_GetProcMsgHandle();

    point.x = x;
    point.y = y;

    para.cur_tp_point.x = x;
	para.cur_tp_point.y = y;
	GetMultiKeyParam(&para); 

    if( !MMK_GetHandleByPos( point, TRUE, &win_handle, PNULL, 0 ) )
    {
        return;
    }

    if ( !CheckTPPair( win_handle ) )
    {
        return;
    }
    
    MMK_SetProcMsgHandle( win_handle );
    
    // check whether user hook handler
    tp_handle_result = MMK_RunWinHookProc(win_handle, MSG_TP_PRESS_LONG, (DPARAM)&para);

    // check ctrl handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        tp_handle_result = DefaultProcessTPMsg( win_handle, MSG_TP_PRESS_LONG, (DPARAM)&para );
    }

    // check window callback handler
    if( MMI_RESULT_FALSE == tp_handle_result )
    {
        //dispatch the window 
        tp_handle_result = MMK_RunWinProc( win_handle, MSG_TP_PRESS_LONG, (DPARAM)&para );
        if (MMI_RESULT_FALSE == tp_handle_result)
        {
            tp_handle_result = DefaultProcessWinTPMsg( win_handle, MSG_TP_PRESS_LONG, (DPARAM)&para );
        }
    }
    
    MMK_SetProcMsgHandle( old_handle );
}

/*****************************************************************************/
// Description : 获取硬图标快捷菜单数目
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC uint16 MMK_GetShortcutIconNum(void)
{
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    return s_hardware_icon_ptr.hw_icon_num;
#else
    return 0;
#endif
}

/*****************************************************************************/
// Description : point是否在有效的硬图标区
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsPointInShortcutIconRect(GUI_POINT_T point, uint32 icon_index)
{
    BOOLEAN result = FALSE;
    
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    if ( s_is_hw_icon_enable
        //&& MMK_IsResponseHWIcon()
        )
    {
        if (icon_index < s_hardware_icon_ptr.hw_icon_num)
        {
            if (s_hardware_icon_ptr.is_hw_icon_enable[icon_index])
            {
                //SCI_ASSERT(PNULL != s_hardware_icon_ptr.hardware_icon_ptr[i]);
                
                if ( PNULL != s_hardware_icon_ptr.hardware_icon_ptr[icon_index] )
                {
                    if ( GUI_PointIsInRect(point, s_hardware_icon_ptr.hardware_icon_ptr[icon_index]->rect ) )
                    {
                        result = TRUE;
                    }
                }
            }
        }
    }  
#endif
    
    return result;
}

/*****************************************************************************/
// Description : 获取当前硬图标要虚拟的物理键值
// Global resource dependence : 
// Author:xin.li
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetShortcutIconVirtualKeycode(uint32 icon_index,     //in
                                                 uint32 *keycode_ptr    //out
                                                 )
{
        BOOLEAN result = FALSE;
    
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    if ( s_is_hw_icon_enable
        //&& MMK_IsResponseHWIcon()
        )
    {
        if (icon_index < s_hardware_icon_ptr.hw_icon_num)
        {
            if (s_hardware_icon_ptr.is_hw_icon_enable[icon_index])
            {
                //SCI_ASSERT(PNULL != s_hardware_icon_ptr.hardware_icon_ptr[i]);
                
                if ( PNULL != s_hardware_icon_ptr.hardware_icon_ptr[icon_index] )
                {
                    *keycode_ptr = s_hardware_icon_ptr.hardware_icon_ptr[icon_index]->virtual_key_code;
                    
                    result = TRUE;
                }
            }
        }
    }  
#endif
    
    return result;
}

/*****************************************************************************/
// Description : 处理应图标快捷菜单
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
// MMI_MESSAGE_ID_E msg_id: 消息id
// DPARAM param: 参数
// GUI_POINT_T point: pressed point
// BOOLEAN is_deal_msg:是否处理msg_id
/*****************************************************************************/
LOCAL MMI_RESULT_E ResponseShortCutBar(
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param, 
                                       GUI_POINT_T point,
                                       BOOLEAN is_deal_msg
                                       )
{
    MMI_RESULT_E     result = MMI_RESULT_FALSE;

#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    uint32      i= 0;

    // hardware short cut bar
    //改成TP UP时响应操作
    if (s_is_hw_icon_enable && GUI_PointIsInRect( point, MMITHEME_GetHardIconRect()))
    {
        if (is_deal_msg 
            //某些窗口不允许响应硬图标, 是临时解决方案, 平台大量使用applet机制后此处可以删除
            && MMK_IsResponseHWIcon())
        {
            for (i = 0;i < s_hardware_icon_ptr.hw_icon_num; i++)
            {
                if (s_hardware_icon_ptr.is_hw_icon_enable[i])
                {
                    //SCI_ASSERT(PNULL != s_hardware_icon_ptr.hardware_icon_ptr[i]);
                    
                    if ( PNULL != s_hardware_icon_ptr.hardware_icon_ptr[i] )
                    {
                        if ( GUI_PointIsInRect( point, s_hardware_icon_ptr.hardware_icon_ptr[i]->rect ) )
                        {
                            if (PNULL != s_hardware_icon_ptr.hardware_icon_ptr[i]->ProcessMsg)
                            {
                                s_hardware_icon_ptr.hardware_icon_ptr[i]->ProcessMsg(msg_id, param);
                                break;
                                
                                
                            }
                        }
                    }
                }
            }
        }
        result  = MMI_RESULT_TRUE;
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }
    // end
#else
    result = MMI_RESULT_FALSE;
#endif

    return result;
}

/*****************************************************************************/
//     Description : default process the window touch pannel message
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DefaultProcessWinTPMsg(
                                          MMI_HANDLE_T        win_handle,
                                          MMI_MESSAGE_ID_E    msg_id,   // the message id
                                          DPARAM              param     // the param
                                          )
{
    uint16              x                  = (MMK_GET_TP_X(param));
    uint16              y                  = (MMK_GET_TP_Y(param));
    GUI_POINT_T         point              = {0};
    MMI_RESULT_E        result             =  MMI_RESULT_FALSE;
    
    point.x = x;
    point.y = y;
    
	// hardware short cut bar
	switch ( msg_id )
	{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:
		// 首先处理应图标
        if(MMI_RESULT_TRUE == ResponseShortCutBar(msg_id, param, point, TRUE))
        {
            result = MMI_RESULT_TRUE;
        }
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:
		// 首先处理应图标
        if(MMI_RESULT_TRUE == ResponseShortCutBar(msg_id, param, point, FALSE))
        {
            result = MMI_RESULT_TRUE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

	default:
		break;
	}

    return result;
}


/******************************************************************************/
/*! \brief check the TP single because of the system queue full,
 *			call by TPCallBackFunc
 *  \param[in]
 *  \param[in]
 *  \author  liqing.peng
 *  \return 
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_CheckTPSignale(
                                  uint16 state,  //[IN] TP_DOWN or TP_UP
                                  uint32 x,
                                  uint32 y
                                  )
{
    BOOLEAN             recode = FALSE;
    BOOLEAN             is_handle = FALSE;
    
    //is handle tp
    is_handle = CheckHandleTp(state);

    if (is_handle)
    {
        SCI_TRACE_LOW("MMK_CheckTPSignale enter: state = %d, s_tp_prev_code = %d, s_tp_num = %d, s_tp_move_num = %d, s_tp_gesture_num = %d");

        if (MMI_MAX_WAITING_TP > s_tp_num)
        {
            // TP没有FULL掉
            if(MMI_TP_NONE == s_tp_prev_code)
            {
                //只接受对应的Down消息和息屏的消息
                if(MMI_TP_DOWN == state)
                {
                    s_tp_prev_code = MMI_TP_DOWN;
                    recode = TRUE;
                }
                else if(MMI_TP_GESTURE_CLICK == state)
                {
                    // 息屏状态下的 CLICK消息,用来唤醒屏幕
                    s_tp_prev_code = MMI_TP_NONE;
                    recode = TRUE;
                    SCI_TRACE_LOW("MMK_CheckTPSignale state = MMI_TP_GESTURE_CLICK");
                }
                else
                {
                    recode = FALSE;
                }
            }
            else if(MMI_TP_DOWN == s_tp_prev_code)
            {
                //已经有了Down消息，只接受对应的Move和Up消息
                if( MMI_TP_MOVE == state)
                {
				    if(MMI_MAX_TP_MOVE > s_tp_move_num)
				    {
					    s_tp_move_num++;
					    recode = TRUE;
				    }
				    else
				    {
					    recode = FALSE;
				    }
                }
#ifdef TOUCH_GESTURE_ENABLE
                else if (MMI_TP_GESTURE == state)
                {
                    if (MMI_MAX_TP_GESTURE > s_tp_gesture_num)
                    {
                        s_tp_gesture_num++;
                        recode = TRUE;
                    }
                    else
                    {
                        recode = FALSE;
                    }
                }
#endif
                else if(MMI_TP_UP == state)
                {
				    s_tp_num++;
                    s_tp_prev_code = MMI_TP_NONE;
                    recode = TRUE;
                }
                else
                {
                    recode = FALSE;
                }
            }
            else
            {
                recode = FALSE;
            }
        }
        else
        {
            //TP消息FULLL掉了
            recode = FALSE;
        }
    }
    
    //if ( recode )
    {
        s_physical_tp_x = x;
        s_physical_tp_y = y;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : check if handle tp
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckHandleTp(
                            uint16  state   //tp down or up
                            )
{
    BOOLEAN             result = TRUE;
    /*BOOLEAN             result = FALSE;
    int32               cur_key_num = 0;
    MMI_MESSAGE_ID_E	multi_key_msg[MMI_MAX_MULTI_KEYS] = {0};

    //有key down消息时,只屏蔽tp down消息
    cur_key_num = MMK_GetMultiKeyMsg(multi_key_msg, KEY_NONE);
    if (0 == cur_key_num )
    {
        result = TRUE;
    }
    else
    {
        if (MMI_TP_DOWN == state)
        {
            //SCI_TRACE_LOW:"CheckHandleTp: has key down,not handle tp down msg!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_1410_112_2_18_3_22_48_37,(uint8*)"");
        }
        else
        {
            result = TRUE;
        }
    }*/
    //ref 有漏报key up的情况，把kbd按下组织tp消息去掉

    return (result);
}

/******************************************************************************/
/*! \brief			quick tp, tp is nearly full
 *  \author			James.Zhang
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsQuickTp(void)
{
    if ( s_tp_num > 0)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : reduce the handle TP num 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void ReduceHandleTPNum(void)
{
    if (0 < s_tp_num)
    {
        s_tp_num--;
//        SCI_TRACE_LOW("ReduceHandleTPNum: s_tp_num-- = %d", s_tp_num);
    }
}

/*****************************************************************************/
// 	Description : reduce the handle TP num 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void ReduceHandleTPMoveNum(void)
{
    if (0 < s_tp_move_num)
    {
        s_tp_move_num--;
    }
}

/*****************************************************************************/
// 	Description : set TP down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL void SetTPDownCtrl(
                         MMI_HANDLE_T  ctrl_handle
                         )
{
    s_tpdown_ctrl_handle = ctrl_handle;
}

/*****************************************************************************/
// 	Description : get TP down ctrl
//	Global resource dependence : 
//  Author: liqing.peng
//	Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetTPDownCtrl( void )
{
    return s_tpdown_ctrl_handle;
}

/*****************************************************************************/
// 	Description : check tp pair
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:默认情况下，tp必须配对，若应用设置handle为0，可不配对
/*****************************************************************************/
LOCAL BOOLEAN CheckTPPair( 
                          MMI_HANDLE_T  win_handle
                          )
{
    if ( 0 == s_tpdown_win_handle 
        || s_tpdown_win_handle == win_handle
        || s_tpdown_win_handle == MMK_GetParentWinHandle( win_handle ) )
    {
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"CheckTPPair failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TP_1494_112_2_18_3_22_48_38,(uint8*)"");
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set TP down win
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPDownWin(
                             MMI_HANDLE_T  win_handle
                             )
{
    MMI_HANDLE_T parent_handle = 0;
    
    if ( 0 != ( parent_handle = MMK_GetParentWinHandle( win_handle ) ) )
    {
        s_tpdown_win_handle = parent_handle;
    }
    else
    {
        s_tpdown_win_handle = win_handle;
    }
    //clear ctrl tp state when clearing win tp state
    if(0 == win_handle)
    {
        SetTPDownCtrl(0);
    }
}

/*****************************************************************************/
// 	Description : get TP down win
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetTPDownWin( void )
{
    return s_tpdown_win_handle;
}

/******************************************************************************/
/*! \brief get multi tp message
 *  \param[in]		tp_status_ptr	pointer to touch panel status
 *  \param[in]		point_ptr		pointer to touch panel point
 *  \author  bruce.chi
 *  \return 
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC int MMK_GetMultiTPMsg(MMI_TP_STATUS_E *tp_status_ptr, GUI_POINT_T *point_ptr)
{
	//SCI_ASSERT(tp_status_ptr != PNULL && point_ptr != PNULL);
    if ( !(tp_status_ptr != PNULL && point_ptr != PNULL) )
    {
        return 0;
    }

    *tp_status_ptr = s_tp_status;

	point_ptr->x = g_tp_x;
	point_ptr->y = g_tp_y;
    
	return 1;
}

/******************************************************************************/
/*! \brief get multi tp message
 *  \param[in]		tp_status_ptr	pointer to touch panel status
 *  \param[in]		point_ptr		pointer to touch panel point
 *  \author  bruce.chi
 *  \return 
 *		- <i><c>message number</c></i>
 */
/******************************************************************************/
PUBLIC void MMK_GetLogicTPMsg(MMI_TP_STATUS_E *tp_status_ptr, GUI_POINT_T *point_ptr)
{
    uint32 logic_x = 0;
    uint32 logic_y = 0;

	//SCI_ASSERT(tp_status_ptr != PNULL && point_ptr != PNULL);
    if (!(tp_status_ptr != PNULL && point_ptr != PNULL))
    {
        return;
    }

    *tp_status_ptr = s_tp_status;
    
    //move过后才取点
    if ( s_is_tp_move )
    {
        logic_x = s_physical_tp_x;
        logic_y = s_physical_tp_y;
        
        GUILCD_ConvertTpPoint(&logic_x, &logic_y, GUI_MAIN_LCD_ID);
#ifdef WATCH_LEFT_RIGHT_WEAR_SUPPORT
        GUILCD_ConvertTpPointEx(&logic_x, &logic_y, GUI_MAIN_LCD_ID);
#endif
        point_ptr->x = logic_x;
        point_ptr->y = logic_y;
    }
    else
    {
        point_ptr->x = g_tp_x;
        point_ptr->y = g_tp_y;
    }

	return;
}

/*****************************************************************************/
// 	Description : get multi key param
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
LOCAL void GetMultiKeyParam(MMI_MULTI_KEY_TP_MSG_PARAM_T *key_tp_msg_ptr)
{
	int					cur_key_num = 0;
	MMI_MESSAGE_ID_E	multi_key_msg[MMI_MAX_MULTI_KEYS] = {0};
	
	SCI_ASSERT(key_tp_msg_ptr != PNULL);/*assert verified*/

	cur_key_num = MMK_GetMultiKeyMsg(multi_key_msg, KEY_NONE);
	
	key_tp_msg_ptr->multi_key_num = cur_key_num;
	SCI_MEMCPY(key_tp_msg_ptr->multi_key_msg, multi_key_msg, sizeof(multi_key_msg));
}

/*****************************************************************************/
// 	Description : Set Hardware Icon Is Enable
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPHardwareIconIsEnable(
                                          BOOLEAN is_hw_icon_enable,
                                          uint16 max_hw_icon_count
                                          )
{
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    //SCI_ASSERT(max_hw_icon_count <= MMI_MAX_HARDWARE_ICON);
    if ( max_hw_icon_count > MMI_MAX_HARDWARE_ICON )
    {
        return;
    }

    s_is_hw_icon_enable = is_hw_icon_enable;
    s_hardware_icon_ptr.hw_icon_num = max_hw_icon_count;
#endif
}

/*****************************************************************************/
// 	Description : Set Hardware Icon Button For TP
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPHardwareIcon(
                                  BOOLEAN is_cur_hw_icon_enable,
                                  MMI_HW_ICON_T *hardware_icon_ptr,
                                  uint16 hw_icon_index
                                  )
{
#ifndef TOUCH_PANEL_HWICON_SUPPORT_NONE
    //SCI_ASSERT(hw_icon_index < MMI_MAX_HARDWARE_ICON);
    if ( hw_icon_index >= MMI_MAX_HARDWARE_ICON )
    {
        return;
    }

    if (is_cur_hw_icon_enable)
    {
        s_hardware_icon_ptr.is_hw_icon_enable[hw_icon_index] = TRUE;
        s_hardware_icon_ptr.hardware_icon_ptr[hw_icon_index] = hardware_icon_ptr;
    }
    else
    {
        s_hardware_icon_ptr.is_hw_icon_enable[hw_icon_index] = FALSE;
    }
#endif
}

/*****************************************************************************/
// 	Description : check whe this is a valuable tp move point
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpMove(
                       MmiTPPress  *tppress_ptr   // [in]
                       )
{
    BOOLEAN is_tp_move = FALSE;
    int32  move_spacing = ( s_is_delay_tp_move ) ? MMI_TP_DELAY_MOVE_SPACING : MMI_MOVE_SPACING;

    //SCI_ASSERT(PNULL != tppress_ptr);
    if (PNULL == tppress_ptr)
    {
        return FALSE;
    }

    if (s_is_tp_move
        || abs((int32)tppress_ptr->x - (int32)s_tp_point_x) > move_spacing
        || abs((int32)tppress_ptr->y - (int32)s_tp_point_y) > move_spacing)
    {
        is_tp_move = TRUE;
        s_is_delay_tp_move = FALSE;
    }

 	return is_tp_move;
}

/*****************************************************************************/
// 	Description : is exist tp move
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsExistTpMove( void )
{
    return s_is_tp_move;
}

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
PUBLIC void MMK_RedispatchMSGTpDown(
                                    uint16 x,
                                    uint16 y
                                    )
{
    DispatchMSGTpDown(x, y);
}
/*****************************************************************************/
// Description : 是否正在校准触摸屏
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsCoordinatingTp(void)
{
    return s_is_coordinating_tp;
}

/*****************************************************************************/
// Description : 设置正在校准触摸屏标志
// Global resource dependence : 
// Author:xin.li
// Note:
/*****************************************************************************/
PUBLIC void MMK_SetCoordinateTpFlag(BOOLEAN is_coordinating)
{
    s_is_coordinating_tp = is_coordinating;
}
/*****************************************************************************/
//     Description : delay tp move
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMK_DelayTpMove( 
                            BOOLEAN is_delay
                            )
{
    s_is_delay_tp_move = is_delay;
}

/*****************************************************************************/
//  Description : reset tp
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMK_ResetTp(void)
{
    if (MMI_TP_NONE != s_tp_status)
    {
        s_tp_prev_code = MMI_TP_NONE;

        s_tp_status = MMI_TP_NONE;

        s_is_tp_move = FALSE;

        s_is_delay_tp_move = FALSE;

        ReduceHandleTPNum();
    }
}
#endif

/*Edit by script, ignore 2 case. Thu Apr 26 19:01:15 2012*/ //IGNORE9527
