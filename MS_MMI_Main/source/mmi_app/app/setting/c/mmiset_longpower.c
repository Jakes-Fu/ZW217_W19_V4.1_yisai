/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004      maryxiao       Create
******************************************************************************/

#define _MMISET_LONGPOWER_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_setting_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "guilistbox.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmiphone_export.h"
#include "mmiset_id.h"
#include "mmiset_menutable.h"
#include "mmienvset_export.h"
#include "mmipub.h"
#include "guistatusbar.h"
#include "mmimms_export.h"
#include "mmiset_func.h"
#include "mmiset_wintab.h"

#ifdef MCARE_V31_SUPPORT
#include "Mcare_Interface.h"
#endif
#ifndef WIN32
#ifdef SXH_APPLE_SUPPORT
#include "OEMOS.h"
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set fly mode
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyMode(void);

//保存设置静音前的模式
LOCAL uint16 s_mmienvset_premode  = MMIENVSET_STANDARD_MODE;

/*****************************************************************************/
//  Description : to handle the long_power window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLongPowerWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : to handle the longpower option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandleLongPowerMenuMsg(
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 );


WINDOW_TABLE( MMISET_LONGPOWER_WIN_TAB ) = 
{
    //WIN_STATUSBAR,
    WIN_FUNC( (uint32)HandleLongPowerWinMsg),
    WIN_ID(MMISET_LONGPOWER_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_LONGPOWER_OPTION,MMISET_LONGPOWER_CTRL_ID),
    //WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};



/*****************************************************************************/
//  Description : open longpower menu  
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISET_LongpowerCreatmenu(void)
{
	if (MMK_IsExistWin(MMK_GetAppletHandleByWin(MMISET_LONGPOWER_WIN_ID), MMISET_LONGPOWER_WIN_ID))
	{
	    if (!MMK_IsFocusWin(MMISET_LONGPOWER_WIN_ID))
	    {
                MMK_WinGrabFocus(MMISET_LONGPOWER_WIN_ID);
	    }
	}
	else
	{
		MMK_CreateWin((uint32*)MMISET_LONGPOWER_WIN_TAB,PNULL);
	}
}


/*****************************************************************************/
//  Description : to handle the long_power window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLongPowerWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMISET_LONGPOWER_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = MENU_LONGPOWER_OPTION;
    MMI_MENU_ID_T menu_id = 0;
    uint16  mode = MMIAPIENVSET_GetActiveModeId();
    BOOLEAN    is_fly_mode_on  = MMIAPISET_GetFlyMode();
    MMI_HANDLE_T  win_handle = 0;
    //SCI_TRACE_LOW:"HandleBlacklistOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISET_LONGPOWER_114_112_2_18_2_51_19_234,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //xiyuan edit for CR NEWMS00162977
        win_handle = MMK_ConvertIdToHandle(win_id);
        GUISTATUSBAR_SetBgIsDesplay(win_handle, FALSE);

        if (MMIENVSET_SILENT_MODE == mode)
        {
            GUIMENU_SetItemVisible(ctrl_id, group_id, ID_ENVSET_SILENTON,   FALSE);
        }
        else
        {
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
            if((MMIENVSET_EARPHONE_MODE == mode|| MMIENVSET_BLUETOOTH_MODE == mode)
                && MMIAPIENVSET_GetMuteState())
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, ID_ENVSET_SILENTON,   FALSE);
            }
            else
#endif
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, ID_ENVSET_SILENTOFF,   FALSE);
            }

        }
        if (is_fly_mode_on)
        {
            GUIMENU_SetItemVisible(ctrl_id, group_id, ID_ENVSET_FLYON,   FALSE);
        }
        else
        {
            GUIMENU_SetItemVisible(ctrl_id, group_id, ID_ENVSET_FLYOFF,   FALSE);
        }
        MMK_SetAtvCtrl(win_id, MMISET_LONGPOWER_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
#ifdef MCARE_V31_SUPPORT
        if(ID_ENVSET_FLYON == menu_id)
        {
            if(MCareV31_Exit_Confirm())
            {
                break;		
            }
        }
#endif
#ifndef WIN32
#ifdef SXH_APPLE_SUPPORT
      if(ID_ENVSET_FLYON == menu_id)
        {
            if(AppleIsRun())
            {         
               OEMOS_ToExitCore();
            }
        }
#endif
#endif
        HandleLongPowerMenuMsg(menu_id,ctrl_id);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:    
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//  Description : to handle the longpower option window
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
LOCAL void HandleLongPowerMenuMsg(
                                 MMI_MENU_ID_T menu_id,    //IN: 被选中的菜单项
                                 MMI_CTRL_ID_T ctrl_id
                                 )
{
    uint16  mode = MMIAPIENVSET_GetActiveModeId();
    switch (menu_id)
    {
    case ID_ENVSET_SILENTON:
    case ID_ENVSET_SILENTOFF:   
        if (MMIENVSET_SILENT_MODE != mode)
        {
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
            if(MMIENVSET_EARPHONE_MODE == mode
                || MMIENVSET_BLUETOOTH_MODE == mode)
            {
                s_mmienvset_premode = MMIAPIENVSET_GetPreActiveMode();
                if (MMIAPIENVSET_GetMuteState())
                {
                    MMIENVSET_SetPreActiveMode(MMIENVSET_STANDARD_MODE);
                    MMIAPIENVSET_SetMuteState(FALSE);
                }
                else
                {
                    MMIENVSET_SetPreActiveMode(MMIENVSET_SILENT_MODE);
                    MMIAPIENVSET_SetMuteState(TRUE);
                }
            }
            else
#endif
            {
                s_mmienvset_premode = mode;
                MMIAPIENVSET_ActiveMode(MMIENVSET_SILENT_MODE);
                MMIENVSET_SetPreActiveMode(MMIENVSET_SILENT_MODE);
                MMIAPIENVSET_SetActiveModeForIdle(MMIENVSET_SILENT_MODE);
            }
        }
        else
        {
            MMIAPIENVSET_ActiveMode(s_mmienvset_premode);
            MMIENVSET_SetPreActiveMode(s_mmienvset_premode);
            MMIAPIENVSET_SetActiveModeForIdle(s_mmienvset_premode);
        }
        break;
    case ID_ENVSET_FLYON:
    case ID_ENVSET_FLYOFF:
        SetFlyMode();
        break;
    case ID_POWEROFF:
        MMIAPIPHONE_PowerOff(); //开机
        break;
    default:
        break;
    }
}


/*****************************************************************************/
//  Description :输入语言设置的窗口处理函数
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetFlyMode(void)
{
	BOOLEAN    is_fly_mode_on  = MMIAPISET_GetFlyMode();
	uint16     i               = 0;
	{
		if(is_fly_mode_on)
		{
			BOOLEAN is_ps_deactive = TRUE;
			
			for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
			{
				if(!MMIAPIPHONE_GetIsPsDeactiveCnf((MN_DUAL_SYS_E)i))
				{
					is_ps_deactive = FALSE;
					break;
				}
			}
			if (is_ps_deactive)
			{
				is_fly_mode_on = !is_fly_mode_on;
				
				MMIAPISET_SetFlyMode(is_fly_mode_on);
				
				MMIAPISET_SetIsQueryCfu(FALSE,MMI_DUAL_SYS_MAX);
				
				// 激活协议栈
				MMIAPIPHONE_StartUpPsAccordingToSetting(FALSE, TRUE);
				MMIAPIMMS_RestartAutodownMMS();
			}
			else
			{
				MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
				
				//MMK_CloseWin(win_id);
				return;
			}
		}
		else
		{
			uint32 dual_sys =MN_DUAL_SYS_1;
			BOOLEAN is_ps_ready = TRUE;
			MMISET_DUALSYS_TYPE_E e_dualsys_setting = MMIAPISET_GetMultiSysSetting();
			
			for (dual_sys = MN_DUAL_SYS_1; dual_sys < MMI_DUAL_SYS_MAX; dual_sys++)
			{
				if ((e_dualsys_setting & (0x01 << dual_sys))
					&& MMIAPIPHONE_GetSimExistedStatus(dual_sys))
				{
					is_ps_ready = MMIAPIPHONE_GetIsPsReady(dual_sys);
					
					if (!is_ps_ready)
					{
						return;
					}
				}
			}
            
			if (is_ps_ready)
			{
				if (MMISET_GetIsQueryOpenFlyMode())
				{
					MMIPUB_OpenQueryWinByTextId(TXT_FLY_MODE_PS_ALERT,IMAGE_PUBWIN_QUERY,PNULL,MMISET_HandleFlightmodeWinMsg);
					return;
				}
				else
				{
					is_fly_mode_on = !is_fly_mode_on;
					
					MMIAPISET_SetFlyMode(is_fly_mode_on);
					
					MMIAPIPHONE_DeactivePsAccordingToSetting();
				}
			}
			else
			{
				MMIPUB_OpenAlertWarningWin( TXT_SYS_WAITING_AND_TRY_LATER);
				
				//MMK_CloseWin(win_id);
				return;
			}
		}
	}
}

#endif



/*Edit by script, ignore 1 case. Fri Apr 27 09:38:54 2012*/ //IGNORE9527
