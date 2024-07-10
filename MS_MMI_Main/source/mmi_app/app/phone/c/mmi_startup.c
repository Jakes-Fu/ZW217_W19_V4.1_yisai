/*****************************************************************************
** File Name:   mmiphone_startup.c                                           *
** Author:      liheng.wei                                                   *
** Date:        2020.03                                                      *
** Copyright:                                                                *
** Description: 关于启机流程的控制实现                                       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE              NAME                    DESCRIPTION                   *
**   2020.03           liheng.wei              Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "mmi_app_phone_trc.h"
//#include "mmiphone_onoff.h"
//#include "mmiphone_internal.h"
#include "window_parse.h"
#include "guiblock.h"
#include "mmiidle_subwintab.h"
#include "mmi_id.h"
#include "mmienvset_export.h"
#include "mmieng_export.h"
//#include "mmipub.h"
#include "mmisd_export.h"


#include "mmicom_trace.h"
#include "mmiphone_export.h"
#include "mmiidle_export.h"
#include "mmiset_export.h"
#ifdef TOUCH_PANEL_SUPPORT
#include "tp_export.h"
#endif


/**--------------------------------------------------------------------------*
**                         TYPE DEFINITION                                   *
**---------------------------------------------------------------------------*/

#if defined(W217_UWS6121EG_NOGNSS_PATCH_20231019)
 // move to other file
#else
//启机过程中的状态
typedef enum
{
    MMI_STARTUP_STATE_NORMAL_INIT,  //启机后进入MMI的初始状态

#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
    MMI_STARTUP_STATE_COORDINATE,   //第一次启机，坐标校准窗口，是否要保留???
#endif
    MMI_STARTUP_STATE_BOOTPWD,      //启机密码
    MMI_STARTUP_STATE_FLYMODE,      //飞行模式设置
    MMI_STARTUP_STATE_ANIMATION,    //开机动画
    MMI_STARTUP_STATE_PINPUK,       //Pin or Puk
    MMI_STARTUP_STATE_END,          //启机完成的最后状态，进入Idle或者Launcher
    MMI_STARTUP_STATE_MAX
}MMI_STARTUP_STATE_E;

#endif
typedef BOOLEAN (*API_STATE_Enter)(void);
typedef BOOLEAN (*API_WIN_Enter)(void);

typedef struct
{
    API_STATE_Enter     state_enter;
    API_WIN_Enter       enter_win;
    MMI_STARTUP_STATE_E e_state;
    MMI_STARTUP_EVENT_E e_event;
}MMI_STARTUP_STATE_ITEM_ST;


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
extern MMISET_PWD_OPERATE_TYPE_E g_mmiset_pwd_type;

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
//启机过程中的状态
LOCAL MMI_STARTUP_STATE_E g_startup_state = MMI_STARTUP_STATE_NORMAL_INIT;

/*---------------------------------------------------------------------------*
**                         LOCAL FUNCTION                                    *
**---------------------------------------------------------------------------*/

LOCAL BOOLEAN StartupCtrl_StateEnter(void)
{
    return TRUE;
}
LOCAL BOOLEAN StartupCtrl_StateSkip(void)
{
    return FALSE;
}

LOCAL BOOLEAN StartupCtrl_NeedCoordinate(void)
{
    if (MMIAPITP_IsPowerOnFirst() && !POWER_IsAutoTestMode())
    {
        return TRUE;
    }
    return FALSE;
}

LOCAL BOOLEAN StartupCtrl_NeedAnimation(void)
{
#if defined(W217_UWS6121EG_NOGNSS_PATCH_20231019)
    if(RESTART_BY_ASSERT == POWER_GetRestartCondition())
    {
        return FALSE; 
    }
    else
    {
        return TRUE; // Bug 1321510,1334016
    }
#else
    return TRUE; // Bug 1321510,1334016
#endif
}

LOCAL BOOLEAN StartupCtrl_NeedBootPWD(void)
{
    if (MMIAPISET_GetPowerOnPwdStatus())//开机密码开启
    {
        g_mmiset_pwd_type = MMISET_PWD_NORMAL;
        return TRUE;
    }
    return FALSE;
}

LOCAL void StartupCtrl_CoordinateEnter(void)
{
    MMIAPITP_COORDINATE_OpenWin(TRUE);
}
///////////////////////////////////////////////////////////
// Each state needs to follow this logic:
//
//        |
//        |_____________
//   Enter|        Skip |      (1)
//        |             |
//   ____(2)____        |
//  |           |       |
//  |  State(3) |       | 
//  |           |       |
//  |____(5)____|       |
//        |Event(4)     |
//        |_____________|
//        |
//   Next state
//
// (1)API to judge enter or skip this state
// (2)API to enter window of this state
// (3)This state name
// (4)The finish event which is complete current status
// (5)To call MMIAPIPHONE_StartupCtrl, when finish
///////////////////////////////////////////////////////////

LOCAL MMI_STARTUP_STATE_ITEM_ST g_startup_normal_sequence[] = {
/*              (1)                                       (2)                                       (3)                                      (4) */
/*NormalInit*/{NULL,                            NULL,                           MMI_STARTUP_STATE_NORMAL_INIT,  MMI_STARTUP_NORMAL_INIT_ENTER},
/* TODO: Simlock */
#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
/*Coordinate*/{StartupCtrl_NeedCoordinate,      StartupCtrl_CoordinateEnter,    MMI_STARTUP_STATE_COORDINATE,   MMI_STARTUP_EVENT_COORDINATE_FINISH},
#endif
/*BootPwd   */{StartupCtrl_NeedBootPWD,           MMIAPISET_OpenPowerOnPwdWin,    MMI_STARTUP_STATE_BOOTPWD,      MMI_STARTUP_EVENT_BOOTPWD_FINISH},
/*FlyMode   */{MMIAPISET_GetFlyModeSetPowerOn,  MMIAPISET_SetFlyModeOpenPhone,  MMI_STARTUP_STATE_FLYMODE,      MMI_STARTUP_EVENT_FLYMODE_FINISH},
#ifndef WIN32
/*Animation */{StartupCtrl_NeedAnimation,       MMIAPIPHONE_NormalInitAfterPwd, MMI_STARTUP_STATE_ANIMATION,    MMI_STARTUP_EVENT_ANIMATION_FINISH},
/*PinPuk    */{MMIPHONE_IsNeedPinPuk,           MMIPHONE_OpenPinPukWin,         MMI_STARTUP_STATE_PINPUK ,      MMI_STARTUP_EVENT_PINPUK_FINISH},
#endif
/*End       */{StartupCtrl_StateEnter,          MMIAPIIDLE_OpenIdleWin,         MMI_STARTUP_STATE_END,          NULL}
};
/*****************************************************************************/
//  Description : 启机流程的控制函数
//  Parameter: [In]     event: 当前完成的事件
//             [Out]    None
//             [Return] None
//  Author: liheng.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI_StartupCtrl(MMI_STARTUP_EVENT_E event)
{
    uint32 i = 0;
    BOOLEAN bRet = FALSE;

    // 1. Print current state
    TRACE_APP_STARTUP("Startup,Current State is %d,Current event is %d", g_startup_state, event);

    // 2. Judge whether this event will change state
    if(event == g_startup_normal_sequence[g_startup_state].e_event)
    {
        // change current state
        g_startup_state++;

        // 3. Jump to next state
        for(i=g_startup_state; i<MMI_STARTUP_STATE_END; i++)
        {
            if (PNULL != g_startup_normal_sequence[i].state_enter)
            {
                // 4. Judege whether enter
                if(TRUE == g_startup_normal_sequence[i].state_enter())
                {
                    // enter
                    TRACE_APP_STARTUP("Enter this state(%d), call enter win!", i);
                    g_startup_normal_sequence[i].enter_win();

                    // Set current state
                    g_startup_state = i;

                    break;
                }
                else
                {
                    // skip
                    TRACE_APP_STARTUP("The skip this state(%d) and jump to next!", i);
                    continue;
                }
            }
            else
            {
                TRACE_APP_STARTUP("The enter_win is PNULL, and skip this state(%d)", i);
            }
        }

        // Enter end of state
        if(MMI_STARTUP_STATE_END <= i)
        {
            TRACE_APP_STARTUP("The END state, and enter idle win!");
            g_startup_normal_sequence[MMI_STARTUP_STATE_END].enter_win();

            // Set current state
            g_startup_state = MMI_STARTUP_STATE_END;
        }
    }
    else
    {
        // 3. State unchanged
        TRACE_APP_STARTUP("This event(%d) can NOT be processed in current State(%d)", event, g_startup_state);
    }

}
#if defined(W217_UWS6121EG_NOGNSS_PATCH_20231019)
/*****************************************************************************/
//  Description : set current state
//  Parameter: [In]     event:current state
//             [Out]    None
//             [Return] None
//  Author: xiuyun.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI_SetStartUpState(MMI_STARTUP_STATE_E start_state)
{
    TRACE_APP_STARTUP("set startup State(%d)", start_state);
    if((MMI_STARTUP_STATE_NORMAL_INIT <= start_state)&&(MMI_STARTUP_STATE_MAX >= start_state))
    {
        g_startup_state = start_state;
    }
}
#endif
