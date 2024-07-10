/*****************************************************************************
** File Name:      mmidcd_export.c                                           *
** Author:                                                                   *
** Date:           2008/10/21                                                *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/10/21     aoke.hu          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_dcd_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"

#ifdef MMI_ENABLE_DCD
#include "mmidcd_main.h"
#include "mmidcd_export.h"
#include "mmidcd_nv.h"
#include "mmidcd_http.h"
#include "mmidcd_file.h"
#include "mmidcd_wintab.h"
#include "mmidcd_id.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "guitab.h"
#include "mmiidle_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

MMI_APPLICATION_T g_mmidcd_app={0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 预处理DCD的更新.....
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void PretreatDCDUpdate(MMIDCD_REQ_TYPE_E req_type);
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_InitModule(void)
{
    MMIDCD_RegMenuGroup();
    MMIDCD_RegNv();
    MMIDCD_RegWinIdNameArr();  
    MMIDCD_RegAppletInfo();
}
/*****************************************************************************/
//  Description :  open dcd main window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_OpenMainWin(void)
{
    MMIDCD_OpenMainWin(TRUE);
}
/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 2009/10/9
/*****************************************************************************/
PUBLIC void MMIAPIDCD_ExternalOpenWin(BOOLEAN openMainmenu)
{
    MMIDCD_OpenMainWinFromIdle(openMainmenu);
}

/*****************************************************************************/
//  Description : whether dcd is open
//  Global resource dependence : none
//  Author:byte.guo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_DcdIsOpen(void)
{
    return MMIAPIDCD_GeSettingInfo()->is_dcd_open;
}

/*****************************************************************************/
//  Description : init dcd app
//  Global resource dependence : none
//  Author:violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_AppInit(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]: MMIAPIDCD_AppInit !!!! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_EXPORT_98_112_2_18_2_12_39_0,(uint8*)"");
    
    g_mmidcd_app.ProcessMsg     = HandleDCDProcessMsg;
    g_mmidcd_app.component_type = CT_APPLICATION;
    
    MMIDCD_InitSetting();

    /* init dcd xml file ,if not exsit, create it!*/
    MMIDCDFILE_CreateDCDDir();  
    
    /*when dcd init ,parse xml file*/
    DCD_InitStart();       

    MMIDCD_InitNotifyEvent();
    
}
/*****************************************************************************/
//  Description :get current mcc
//  Global resource dependence :
//  Author: zhuo.wang
//  Note: app interface
/*****************************************************************************/
//PUBLIC void MMIAPIDCD_SetCurMCC(uint32 mcc, uint32 mnc)
//{
//  MMIDCD_SetCurMCC(mcc, mnc);
//}
/*****************************************************************************/
//  Description : 是否可显示
//  Global resource dependence : 
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_DisplayIsValid(void)
{
    BOOLEAN ret = FALSE;
    if(MMIAPIDCD_DcdIsOpen())
    {
        ret = TRUE;  
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : 是否可IDLE下显示
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsIdleDisplay(void)
{
    BOOLEAN ret = FALSE;
    if(MMIAPIDCD_DisplayIsValid() && !MMITHEME_IsMainScreenLandscape())
    {
        ret = TRUE;  
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : 设置更新LOG开关
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetUpdateLogOnOff(BOOLEAN is_on)
{
    if(is_on)
    {
        MMIAPIDCD_GeSettingInfo()->is_dcd_update_log_on = TRUE;
    }
    else
    {
        MMIAPIDCD_GeSettingInfo()->is_dcd_update_log_on = FALSE;
        MMIDCDFILE_DeleteUpdateInfo();
    }
    
    MMINV_WRITE(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo());   
}

/*****************************************************************************/
//  Description : 获取更新LOG开关状态
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_GetUpdateLogOnOff(void)
{
    MMI_ReadNVItem(MMINV_DCD_SETTING, MMIAPIDCD_GeSettingInfo());
    return MMIAPIDCD_GeSettingInfo()->is_dcd_update_log_on; 
}

/*****************************************************************************/
//  Description : restore factory setting
//  Global resource dependence : 
//  Author:byte.guo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_RestoreFactorySetting(void)
{
    // for 连续两次恢复出厂设置后，DCD没发起更新,特起用applet机制来解决
    // 此函数专门供设置模块的恢复出厂设置调用, 也可把DCD 模块的恢复
    // 出厂设置调用此函数，只是之前DCD模块的恢复出厂设置已做好，就没
    // 改动...
    MMIDCD_ResetDCDStart();
}

/*****************************************************************************/
//  Description : when stop u disk, need update idle win
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIDCD_StopUdiskUpdateIdleWin( void )
{
     MMIDCD_StopUdiskUpdateIdleWin();
}

/*****************************************************************************/
//  Description : 获取IDLE下显示区域
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIAPIDCD_GetFocusCtrlRect(void)
{
    return MMIDCD_GetFocusCtrlRect();
}

/*****************************************************************************/
//  Description : dcd wap push msg is arrived, svr update!!
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_WappushUpdate(void)
{
    MMIDCD_SetWapPushUpdate(TRUE);
}
/*****************************************************************************/
//  Description :handle dcd flymode
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_HandFlyModeSetChange(void)
{
    //SCI_TRACE_LOW:"[MMIDCD]: MMIAPIDCD_HandFlyModeSetChange enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_EXPORT_234_112_2_18_2_12_40_1,(uint8*)"");
    if(!MMIDCD_IsIdle())
    {
        //SCI_TRACE_LOW:"[MMIDCD]: DCD in updating but phone is set in fly mode now!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDCD_EXPORT_237_112_2_18_2_12_40_2,(uint8*)"");
        DCD_Sendsig(DCD_SIG_DL_CANCLE_REQ,HTTP_REQ_TYPE_NULL);
    }
}

/*****************************************************************************/
//  Description :dcd is download data now?
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsDownloadingData(void)
{
    BOOLEAN result=FALSE;
    if(!MMIDCD_IsIdle())
    {
        result= TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : the process message function of the main application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIDCD_HandleSysMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    static BOOLEAN is_start_update=FALSE;
    
    switch(msg_id)
    {
        case MISC_MSG_RTC_SEC:
            break;
        
        case MISC_MSG_RTC_MIN:  
            if(MMIAPIDCD_DcdIsOpen())
            {            
                if(!is_start_update)
                {
                    PretreatDCDUpdate(HTTP_REQ_TYPE_START);
                    is_start_update = TRUE;       
                }
                if(MMIAPIDCD_GeSettingInfo()->is_auto_update)
                {
                    if(PNULL != MMIAPIDCD_GetXmlDoc_Ptr())
                    {   
                        if(MMIDCD_IsWapPushUpdate())
                        {
                            PretreatDCDUpdate(HTTP_REQ_TYPE_SVR);
                            MMIDCD_SetWapPushUpdate(FALSE);                         
                        }
                        /*
                        else if(MMIDCD_IsSynchronize(MMIAPIDCD_GetXmlDoc_Ptr()))
                        {
                            PretreatDCDUpdate(HTTP_REQ_TYPE_TTL);
                        }
                        else if(MMIDCD_IsRetryUpdate())
                        {
                            PretreatDCDUpdate(HTTP_REQ_TYPE_RETRY);
                        }*/
                    }
                    else
                    {
                        PretreatDCDUpdate(HTTP_REQ_TYPE_TTL);                 
                    }
                }       
            }
            else
            {
                /*关闭时唤醒功能*/
                if(MMIDCD_IsWapPushUpdate())
                {
                    MMIDCD_Wakeup();
                    MMIDCD_SetWapPushUpdate(FALSE); // for cr177200
                }
            }
            break;
        
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}
/*****************************************************************************/
//  Description : 预处理DCD的更新.....
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void PretreatDCDUpdate(MMIDCD_REQ_TYPE_E req_type)
{
    if(MMIDCD_NetWorkIsValid())
    {
        MMIDCD_UpdateStart(req_type);
    }
    else
    {
        MMIDCD_SetLatestUpdateState(DCD_ERROR_INVALID_NETWORK);
        MMIDCD_SetLatestReqType(req_type);
    }
}
/*****************************************************************************/
//  Description : 设置idle上的dcd tab控件的状态
//  Global resource dependence : none
//  Author: jun.hu
//  Note: 2009/9/6
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetIdleTabColorAndActive(BOOLEAN active_flag)
{
     MMK_SetActiveCtrl(MMK_ConvertIdToHandle(MMIDCD_IDLEWIN_TAB_CTRL_ID), active_flag);
     DCD_SetPageFocusBgImage(active_flag);
}

/*****************************************************************************/
//  Description : handle idle win message
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_HandleIdleWinMessage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    if(!MMIAPIDCD_IsIdleDisplay()
#ifdef MMIWIDGET_SUPPORT 
       || MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()
#endif
        )
    {
        if(NULL != MMK_GetCtrlPtr(MMIDCD_IDLEWIN_TAB_CTRL_ID))
        {
#if defined MMI_SMART_IDLE_SUPPORT
            MMIAPIIDLE_DCDFocusToShortcut();
#endif
            GUITAB_DeleteAllItems(MMIDCD_IDLEWIN_TAB_CTRL_ID);
            MMK_DestroyControl(MMIDCD_IDLEWIN_TAB_CTRL_ID);
            
            MMK_CloseWin(MMIDCD_IDLE_CHILD_WIN_ID);
            
        }
        return FALSE;
    }
    else
    {
        return MMIDCD_HandleIdleWinMessage(win_id,msg_id,param);
    }
	
}
/*****************************************************************************/
//  Description : 设置DCD开关状态
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetDCDEnable(BOOLEAN enable)
{
    MMIAPIDCD_GeSettingInfo()->is_dcd_open = enable;
    MMIAPIDCD_WriteSettingToNv(); 
}
/*****************************************************************************/
//  Description : open link list window
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_OpenLinkListWin(void)
{   
    MMIDCD_OpenLinkListWin();
}

/*****************************************************************************/
//  Description : get current page index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIDCD_GetDcdCurrentPageIndex()
{
	return MMIDCD_GetDcdCurrentPageIndex();
}
/*****************************************************************************/
//  Description : set dcd current page index
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetDcdCurrentPageIndex(uint16 current_page_index,uint16 current_real_index_in_page)
{
		MMIDCD_SetDcdCurrentPageIndex(current_page_index,current_real_index_in_page);
}

/*****************************************************************************/
//  Description : Set DCD net setting index  
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_Setting_SetNetSettingIndex(
                                                 MN_DUAL_SYS_E dual_sys,
                                                 uint8         index
                                                 )
{
    MMIDCD_Setting_SetNetSettingIndex(dual_sys, index);
}

#else   //!宏定义MMI_ENABLE_DCD

/*****************************************************************************/
//  Description : creat wallpaper anim control and set parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_CreateIdleCtrl(MMI_WIN_ID_T win_id )
{
   return TRUE;
}

/*****************************************************************************/
//  Description : 设置更新LOG开关
//  Global resource dependence : 
//  Author:zhuo.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetUpdateLogOnOff(BOOLEAN is_on)
{
    return;    
}

/*****************************************************************************/
//  Description : 是否可显示
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_DisplayIsValid(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : 是否可IDLE下显示
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsIdleDisplay(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description :handle dcd rx change
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_HandFlyModeSetChange(void)
{
   return;
}
/*****************************************************************************/
//  Description :handle dcd rx change
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_IsDownloadingData(void)
{
   return FALSE;
}

/*****************************************************************************/
//  Description : when stop u disk, need update idle win
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:  
/*****************************************************************************/
PUBLIC void MMIAPIDCD_StopUdiskUpdateIdleWin( void )
{
    return;
}

/*****************************************************************************/
//  Description :handle dcd rx change
//  Global resource dependence : 
//  Author: zhuo.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDCD_HandFlyModeSetChange(uint8 rx_level)
{
    return;
}
/*****************************************************************************/
//  Description : the process message function of the main application
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 2003-12-09
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIDCD_HandleSysMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    return MMI_RESULT_FALSE;
}
/*****************************************************************************/
//  Description : handle idle win message
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCD_HandleIdleWinMessage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : 设置DCD开关状态
//  Global resource dependence : 
//  Author: bin.ji
//  Date: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDCD_SetDCDEnable(BOOLEAN enable)
{

}
/*****************************************************************************/
//  Description : open link list window
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDCD_OpenLinkListWin(void)
{   

}
#endif  //!宏定义end MMI_ENABLE_DCD

