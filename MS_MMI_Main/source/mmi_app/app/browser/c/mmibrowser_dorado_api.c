/*****************************************************************************
** File Name:      mmibrowser_export.c                                          *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser api             *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#include "mmk_app.h"
#include "brw_control_api.h"
#include "brw_core_api.h"
#include "mmibrowser_export.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_menutable.h"
#include "mmibrowser_nv.h"
#include "mmibrowser_bookmark.h"
#include "mmipub.h"
#include "mmi_image.h"
#include "browser_text.h"
#include "cfl_wchar.h"
#include "mmiconnection_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"//MS00211656
#include "mmibrowser_id.h"   //add by shuyan.hu
#include "mmibrowser_wintable.h"   //add by shuyan.hu
#include "mmicl_export.h"
#include "mmibrowser_func.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmibrowser_app = {0};
    
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Browser
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyBrowserFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif
/*****************************************************************************/
//  Description : browser application's message handler
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBROWSER_HandleMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : browser application's message handler
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIBROWSER_HandleMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E ret=MMI_RESULT_TRUE;
    
    if (BRW_ProcMsg(msg_id,param))
    {
        ret = MMI_RESULT_TRUE;
    }
    else
    {
        ret = MMI_RESULT_FALSE;
    }
        
    return ret;
}

/*****************************************************************************/
//  Description : init browser module
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_InitModule(void)
{
    g_mmibrowser_app.ProcessMsg=MMIBROWSER_HandleMsg;
    MMIBROWSER_RegMenuGroup();
    MMIBROWSER_RegNv();
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify Browser
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyBrowserFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMIBROWSER]: HandleSIMPlugNotifyBrowserFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_DORADO_API_95_112_2_18_2_5_43_44,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        //no care
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        if ((dual_sys == MMIBROWSER_GetSim())
            && !MMIBROWSER_GetIsUseWifi()
            )
        {
            MMK_CloseMiddleWin(MMI_BRW_BEGIN_WIN_ID, MMIBROWSER_WINDOW_ID_NUMBER);
        }
       
#if defined(MMI_WIFI_SUPPORT) || !defined (MMI_MULTI_SIM_SYS_SINGLE)
        MMK_CloseWin(MMIBROWSER_COM_SELECTSIM_WIN_ID);
#endif
        break;

    default:
        break;
    }
}
#endif

/*****************************************************************************/
//  Description : init browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_InitApp(void)
{
    MMICONNECTION_NOTIFY_INFO_T notify_info = {0};   
#ifdef SIM_PLUG_IN_SUPPORT    
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
#endif

    MMIBROWSER_InitSetting();
    
    notify_info.module_id = MMI_MODULE_MMIBROWSER;
    notify_info.notify_func = MMIBROWSER_Setting_ConnectionCallback;
    MMIAPICONNECTION_RegisterNotifyInfo(&notify_info);
    
#ifndef MMI_BROWSER_MINI_SUPPORT
    MMIBRW_BookmarkInit();
#endif
    //whether record the url which we have clicked.
    BrwSetRecordClickedUrl(MMIAPIENG_GetCMCCTestFlag());
#ifdef SIM_PLUG_IN_SUPPORT 
    sim_plug_notify_info.module_id = MMI_MODULE_MMIBROWSER;
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyBrowserFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif    
}

/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_UninitApp(void)
{
    return;
}

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIDORADO_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr)
{
    MMIENG_UA_T ua = {0};
    MMIBROWSER_ERROR_E  result = MMIBROWSER_SUCC;
    char   *url_ptr = PNULL;
    
    if (PNULL == entry_param_ptr)
    {
        return MMIBROWSER_ERROR_INPUT;
    }
    
    if (!MMIBROWSER_IsPermitEntry())
    {
        return MMIBROWSER_FAIL;
    }

    switch (entry_param_ptr->type)
    {
    case MMIBROWSER_ADD_BOOKMARK:
        result = MMIBROWSER_EnterAddBookmark(entry_param_ptr->url_ptr, entry_param_ptr->url_title_ptr);
        return result;//end
        
    case MMIBROWSER_ACCESS_FLOWRATE:
        MMIAPICL_OpenGprsFlowRateWin();
        return result;//end
        
    default:
        break;
    }
    
    //NOTES:ready to entry browser, if fail it needs to FREE s_browser_context.* ......

    //get ENG test UA. if it isn't NULL, use it for browser temp.
    MMIAPIENG_GetUa(&ua);
    if (0 != SCI_STRLEN((char*)(ua.uaprofile)))
    {
        MMIBROWSER_UpdateUaProfile((char*)(ua.uaprofile));
    }

    if (PNULL != entry_param_ptr->user_agent_ptr)
    {
        MMIBROWSER_UpdateUserAgent(entry_param_ptr->user_agent_ptr);
    }
    else
    {
        if (0 != SCI_STRLEN((char*)(ua.user_agent)))
        {
            MMIBROWSER_UpdateUserAgent((char*)(ua.user_agent));
        }
    }
    
    MMIBROWSER_StopDownload();//MS00237623:不支持下载时的重入,因此先终止下载
    MMIBROWSER_TidyExistedWin();//MS00192555:保证所有已打开的窗口先关闭,除了main窗口

    //begin 重入处理
    switch (entry_param_ptr->type)
    {
    case MMIBROWSER_ACCESS_URL:
    case MMIBROWSER_ACCESS_DOWNLOAD_BY_URL:
        if (PNULL != entry_param_ptr->url_ptr)
        {
            url_ptr = (entry_param_ptr->url_ptr);
        }
        if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
           if (MMIBROWSER_MAIN_WIN_ID != MMK_GetFocusWinId())//MS00192555
            {
                MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
            }
            MMIBROWSER_EnterNewUrl((uint8*)url_ptr, TRUE);
            return MMIBROWSER_SUCC;
        }
        break;
        
    case MMIBROWSER_HOMEPAGE:
        if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
        {
            if (MMIBROWSER_MAIN_WIN_ID != MMK_GetFocusWinId())//MS00192555
            {
                MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
            }
            MMIBROWSER_EnterNewUrl(MMIBROWSER_GetHomepageUrl(), TRUE);
            return MMIBROWSER_SUCC;
        }
        break;

    case MMIBROWSER_ACCESS_MAINMENU:
        if (MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))
        {
            MMK_CloseWin(MMIBROWSER_MAIN_WIN_ID);
            if (MMIBROWSER_START_PAGE_WIN_ID != MMK_GetFocusWinId())//MS00192555
            {
                MMK_WinGrabFocus(MMIBROWSER_START_PAGE_WIN_ID);
            }
            return MMIBROWSER_SUCC;
        }
       break;
    
    default:
        break;
    }
    //end 重入处理
    
    if ((MMIBROWSER_ACCESS_URL == entry_param_ptr->type)
        || (MMIBROWSER_ACCESS_DOWNLOAD_BY_URL == entry_param_ptr->type))
    {
        MMIBROWSER_UpdateEntryUrl(url_ptr);
    }

    MMIBROWSER_SetIsUseWifi(FALSE);//INIT
    {
        MMIDORADO_CUSTOMIZED_CONNECTION_T   customized_connection = {0};
        
        customized_connection.dual_sys = entry_param_ptr->dual_sys;
        customized_connection.is_connection_customized = entry_param_ptr->is_connection_customized;
        customized_connection.connection_index = entry_param_ptr->connection_index;
        MMIDORADO_SetConnectionSettings(customized_connection);
    }
    
    //SCI_TRACE_LOW:"[BRW]MMIDORADO_Entry dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_DORADO_API_280_112_2_18_2_5_44_45,(uint8*)"d", entry_param_ptr->dual_sys);

    MMIBROWSER_SetEntryType(entry_param_ptr->type);

    if (MMIBROWSER_SelectNetworkType(entry_param_ptr->dual_sys, entry_param_ptr->is_use_wifi))
    {
        result = MMIBROWSER_SUCC;
    }
    else
    {
        //Destory
        MMIBROWSER_ResetStartInfo();

        result = MMIBROWSER_FAIL;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : Clean cahe
//  Global resource dependence : 
//  Author: minghu.mao
//  Note: 清除浏览器和http的cache
/*****************************************************************************/
PUBLIC void  MMIDORADO_CleanCache(uint8 * url_ptr, uint16 url_len)
{
    BRW_ClearAllCache();
}

/*****************************************************************************/
//  Description : Get browser Version 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDORADO_GetVersion(void)
{
    return PNULL;
}

/*****************************************************************************/
//  Description : set net setting index
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SetNetSettingIndex(MN_DUAL_SYS_E dual_sys, uint8 index)
{
    MMIBROWSER_SetNetIndexSetting(dual_sys, index);
    MMIBROWSER_UpdateNVSetting();
}

/*****************************************************************************/
//  Description : handle plug out sd card
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SDCardPlugOut(void)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBROWSER_BKHandleSDCardPlugOut();
#endif
}

/*****************************************************************************/
//  Description : handle browser when format the storage 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_HandleStorageFormat(void)
{
#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBROWSER_BKHandleStorageFormat();
#endif
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function                                                                */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : handle url input from at+sytin
//  Global resource dependence : none
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestInsertURL(
                wchar * url_ptr,                 //point to input url buffer 
                uint32 len                          //input url length 
                )
{   
    BOOLEAN result = FALSE;
    //check the url input interface is opend		    
    if(MMK_IsOpenWin(MMIBROWSER_ADDRESS_WIN_ID) && PNULL != url_ptr)
    {
        //insert the string to url editbox
        result =GUIEDIT_InsertString(MMIBROWSER_EDIT_ADDRESS_CTRL_ID,  url_ptr, len);
        //refresh the interface and show the information
        MMK_SendMsg(MMIBROWSER_ADDRESS_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    return result;	

}


/*****************************************************************************/
//  Description : Input bookmark  Title from at+ftin
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestBookmarkTitleInput(
                wchar * wstr_ptr,          //point to input information buffer
                uint16  ilen                    //length of information buffer
                )                
{
      BOOLEAN res = FALSE;
       //check the the bookmark interface is opend
      if(MMK_IsOpenWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID) && (PNULL != wstr_ptr))
      {
	     //insert string into bookmakr title editbox
	     res = GUIEDIT_InsertString(MMIBROWSER_BOOKMARK_EDIT_TITLE_CTRL_ID, wstr_ptr, ilen);
      }
      //refresh the interface and show the message
      MMK_SendMsg(MMIBROWSER_BOOKMARK_EDIT_WIN_ID, MSG_FULL_PAINT, PNULL);
      //SCI_TRACE_LOW:"[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkTitleInput  res = %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_DORADO_API_393_112_2_18_2_5_44_46,(uint8*)"d", res);
	
      return res;
}

/*****************************************************************************/
//  Description : Input bookmark  URL from at+ftin 
//  Global resource dependence : 
//  Author: shuyan.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDORADO_AtestBookmarkURLInput(
                wchar * wstr_ptr,                //point to input information buffer
                uint16  ilen                         //length fo information buffer
                )
{
      BOOLEAN res = FALSE;
      //check the the bookmark interface is opend
      if(MMK_IsOpenWin(MMIBROWSER_BOOKMARK_EDIT_WIN_ID) && (PNULL != wstr_ptr))
      {    
	     //insert string into bookmakr url editbox
	     res = GUIEDIT_InsertString(MMIBROWSER_BOOKMARK_EDIT_URL_CTRL_ID, wstr_ptr, ilen);
      }
      //refresh the interface and show the message
      MMK_SendMsg(MMIBROWSER_BOOKMARK_EDIT_WIN_ID, MSG_FULL_PAINT, PNULL);
      //SCI_TRACE_LOW:"[ATC:][FT:][BOOKMARK:]Atest_BROWSERBookmarkURLInput  res=%d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_DORADO_API_416_112_2_18_2_5_44_47,(uint8*)"d", res);
	
      return res;
}

/*****************************************************************************/
//  Description : Dorado Set Default Bookmark
//  Global resource dependence : 
//  Author: dave.raun
//  Note:
/*****************************************************************************/
PUBLIC void MMIDORADO_SetDefaultBookmark(void)
{
    MMIBRW_SetDefaultBookmark();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    end of Atest  Function                                                                 */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

