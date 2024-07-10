/*****************************************************************************
** File Name:      mmibrowser_wintable.c                                     *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#include "window_parse.h"
//#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
//#include "guilabel.h"
#include "mmidisplay_data.h"
#include "brw_control_api.h"
#include "mmibrowser_wintable.h"
#include "mmibrowser_id.h"
#include "browser_image.h"
#include "browser_text.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "guirichtext.h"
#include "guilistbox.h"
#include "mmibrowser_setting.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "mmiconnection_export.h"
#include "cfl_wchar.h"
#include "mmi_default.h"
#include "mmiidle_export.h"
#include "mmipb_export.h"
#include "mmicom_time.h"
#include "mmk_timer.h"
#include "cfl_char.h"
#include "mmipb_export.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "browser_anim.h"
//#include "guiform.h"
#include "guiownerdraw.h"
#include "cafcontrol.h"
#include "mmistk_export.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
//#include "mmiebook_file.h"//MS00187401
#include "mmisd_export.h"
#include "mmicc_export.h"
#include "mmibrowser_menutable.h"
#include "mmi_resource.h"
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmiacc_text.h"
#include "mmibrowser_bookmark.h"
#include "mmifmm_image.h"
#include "mime_type.h"
//#include "mmipb_text.h"
#include "mmibrowser_func.h"
#include "mmiudisk_export.h"
#include "mmibrowser_nv.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "mmi_data.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif

#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mmk_tp.h"
#include "mmisms_export.h"
#include "mmibrowser_manager_wintable.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define NUM_STATE_MAX   10
#define NUM_STATE_AND_TEXT_MAX   255

#ifdef MMIDORADO_MY_NAV_SUPPORT
#define MMIBRW_ICO_ROOT_PATH  (s_dorado_ico_root_path) //"browser\ico"
#define MMIBRW_ICO_ROOT_PATH_LEN  11 
LOCAL const wchar s_dorado_ico_root_path[]   = {'B' , 'r', 'o', 'w', 's', 'e', 'r', '\\', 'I','c','o',0 };
LOCAL const wchar s_dorado_ico_file_suffix[]   = {'i', 'c', 'o', 0 };
LOCAL BOOLEAN   s_is_ico_need_update = FALSE;
#endif
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
#ifdef MMIDORADO_FIX_NAV_SUPPORT
const MMIBRW_FIX_NAV_ITEM_T  g_fix_nav_default_info[MMIBRW_FIX_NAV_MAX_ITEM] = 
{
    {TXT_VIDEO, IMAGE_BROWSER_FIX_NAV_ICON1, "http://wap.51wre.com/bookmark/?i=65"},
    {TXT_BROWSER_WEIBO,IMAGE_BROWSER_FIX_NAV_ICON2, "http://wap.51wre.com/bookmark/?i=66"},
    {TXT_BROWSER_SHOPPING,IMAGE_BROWSER_FIX_NAV_ICON3, "http://wap.51wre.com/bookmark/?i=67"},
    {TXT_COMMON_MUSIC, IMAGE_BROWSER_FIX_NAV_ICON4, "http://wap.51wre.com/bookmark/?i=68"},
    {TXT_ENTERTAIMENT_GAME, IMAGE_BROWSER_FIX_NAV_ICON5, "http://wap.51wre.com/bookmark/?i=69"},
    {TXT_BROWSER_SOHU, IMAGE_BROWSER_FIX_NAV_ICON6, "http://wap.51wre.com/bookmark/?i=70"},
    {TXT_BROWSER_MOVIE, IMAGE_BROWSER_FIX_NAV_ICON7, "http://wap.51wre.com/bookmark/?i=71"},
    {TXT_MAINMENU_ICON_BAIDU, IMAGE_BROWSER_FIX_NAV_ICON8, "http://wap.51wre.com/bookmark/?i=72"}
};
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const MMIBROWSER_PROCESS_T g_dorado_proc = {
            MMIDORADO_InitModule,
            MMIDORADO_InitApp, 
            MMIDORADO_UninitApp,
            MMIDORADO_Entry,
            MMIDORADO_Close,
            MMIDORADO_HandleBrwControlMsg,
            MMIDORADO_GetVersion,
            PNULL,
            MMIDORADO_SDCardPlugOut,
            MMIDORADO_HandleStorageFormat,
            MMIDORADO_SetNetSettingIndex,
            MMIDORADO_GetNetSettingIndex,
            MMIDORADO_IsRunning,
            MMIDORADO_IsActive,
            MMIDORADO_ResetFactorySetting,
            MMIDORADO_CleanUserInfo,
            PNULL,//MMIDORADO_RegMimeFunc,
            MMIDORADO_CleanCache,
            PNULL,
            PNULL,
//begin add new
            MMIDORADO_SetDefaultBookmark,
//end:add new
//begin:for AT TEST
            MMIDORADO_AtestInsertURL,
            MMIDORADO_AtestBookmarkTitleInput,
            MMIDORADO_AtestBookmarkURLInput
//end:for AT TEST
            };

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
#ifdef JS_SUPPORT
extern void BrwSetWatchDogPtr(void *SwdgPtr);
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : snapshot open file callback
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
LOCAL void BrwSnapshotOpenFileCallback(char *url_ptr, BOOLEAN is_succ);

/**--------------------------------------------------------------------------*
 **                         WINDOW DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description : enter browser :main menu, homepage, url........
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_Enter(void)
{
    MMIWWW_ENTRY_TYPE_E type = MMIBROWSER_ENTRY_TYPE_MAX;
    MN_DUAL_SYS_E   avtive_sim = MMIBROWSER_GetSim();
#ifdef MMI_ENVSET_MINI_SUPPORT
    MMIAPIENVSET_CloseActModeKeyRingSet();
#endif
    if (!MMIDORADO_IsRunning())
    {
#ifdef MMI_STREAMING_PLAYER_SUPPORT//NEWMS00164417
        if(MMK_IsOpenWin( MMIAPIVP_GetVPNetWinID())) 
        {
            MMK_CloseWin(MMIAPIVP_GetVPNetWinID());
        }
#endif

#ifdef MET_MEX_SUPPORT	
        if (MMIMEX_ConflictPromt())
        {
            // mex kernel is running and may result in mem conflict
            return ;
        }	
#endif
        
        if (BRW_IsNetLinked() 
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifndef WIN32
            || MMIPDP_IsDifferdualsysActivedPdpLinkExist(avtive_sim)
#endif
#endif
            )//MS00223556:之前的GPRS链接仍未断开或者其他卡存在PDP链接
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_Enter: Try later"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_195_112_2_18_2_5_45_48,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_SYS_WAITING_AND_TRY_LATER,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            return ;
        }

        //弹出等待窗口，提前促使释放窗口内存
        MMIPUB_OpenTempWaitWin();

        if(!MMIBROWSER_Init())
        {
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_Init error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_206_112_2_18_2_5_45_49,(uint8*)"");
            return;
        }
        
        if (!MMIBROWSER_GetIsUseWifi())//for sim plmn log
        {
            MN_PLMN_T select_plmn = {0, 0, 2};

            //SCI_TRACE_LOW:"[BRW]MMIBROWSER_Enter:avtive_sim:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_215_112_2_18_2_5_45_50,(uint8*)"d", avtive_sim);
            if (MMIAPIPHONE_IsPSAvail(avtive_sim))/*PS OK后再根据plmn status遍历到对应的UA*/
            {
                MMIAPIPHONE_GetSimHPLMN(&select_plmn, avtive_sim);
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_Enter sim mcc:%d, mnc:%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_219_112_2_18_2_5_45_51,(uint8*)"dd", select_plmn.mcc, select_plmn.mnc);
            }
        }
    }
    
    type = MMIBROWSER_GetEntryType();
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_Enter type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_224_112_2_18_2_5_45_52,(uint8*)"d", type);
    switch (type)
    {
    case MMIBROWSER_HOMEPAGE:
        {
            MMIBROWSER_UpdateEntryUrl((char *)(MMIBROWSER_GetHomepageUrl()));
            MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)(MMIBROWSER_GetEntryUrl()), PNULL);
            MMIBROWSER_FreeEntryUrl();
        }
        break;
        
    case MMIBROWSER_ACCESS_URL:
    case MMIBROWSER_ACCESS_DOWNLOAD_BY_URL:
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_URL, (uint8*)(MMIBROWSER_GetEntryUrl()), PNULL);
        MMIBROWSER_FreeEntryUrl();
        break;

    case MMIBROWSER_ACCESS_MAINMENU:
#if defined(BROWSER_START_PAGE_DEFAULT) || defined(BROWSER_START_PAGE_THEME1)
        MMIBROWSER_OpenStartPageWin();
        ActiveNetwork(BRW_INVALID_INSTANCE);
#else//without startpage window
        MMIBROWSER_StartWeb(MMIBROWSER_WEB_ENTRY_NONE, PNULL, PNULL);
#endif
        break;
        
#ifdef BROWSER_SEARCH_SUPPORT
    case MMIBROWSER_ACCESS_SEARCH:
        MMIBROWSER_OpenSearchWin();
        ActiveNetwork(BRW_INVALID_INSTANCE);
        break;
#endif
        
    default:
        break;
    }

	MMIBM_CloseBrowserSelectWin();
}

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_Init(void)
{
    SCI_TRACE_LOW("[BRW]MMIBROWSER_Init:begin");

#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_BROWSER);//for browser to get large memory
#endif

    MMIBROWSER_SetKernelInit(FALSE);
    if(!BRW_InitMem())
    {
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_BROWSER);
#endif
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_Init:InitMem error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_271_112_2_18_2_5_45_53,(uint8*)"");
        return FALSE;
    }
    
    {
        BRW_PUBLIC_PROFILE_T    public_profile = {0};
        
        public_profile = MMIBROWSER_SetPublicProfile();
        BrwSetPublicProfile(&public_profile);
        MMIBROWSER_FreePublicProfile(public_profile);
    }

#ifdef MMIDORADO_FIX_HMTL_SUPPORT
    {//init the fixed instance
        BRW_PROFILE_T   profile={0};
      
        MMIBROWSER_InitFixInfo();
        profile = MMIBROWSER_SetFixHtmlProfile(MMIBROWSER_FIX_HTML_WIN_ID);
        MMIBROWSER_SetFixedInstance(BRW_InitInstance(&profile, TRUE));
        
}
#endif

#ifndef MMI_BROWSER_MINI_SUPPORT
    MMIBRW_BookmarkInit();
#endif

#ifndef MMIDORADO_BOOKMARK_BY_NV
    MMIBRW_BookmarkInitAfterStartup();
#endif

    Brw_HttpInit();
    BRW_VisitHisInit();
    BRW_SnapshotInit();
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
    BRW_InputHisInit();
#endif
    BRW_ParseInit();
    
    MMIBROWSER_SetKernelInit(TRUE);
    
    SCI_TRACE_LOW("[BRW]MMIBROWSER_Init:end");
    return TRUE;
}

/*****************************************************************************/
//  Description : the exit of browser 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_Exit(void)
{
    SCI_TRACE_LOW("[BRW]MMIBROWSER_Exit:begin");

    DeactiveNetwork(BRW_INVALID_INSTANCE);
    if (MMIBROWSER_GetKernelInit())
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_Exit"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_313_112_2_18_2_5_45_54,(uint8*)"");
        MMIBROWSER_SetKernelInit(FALSE);//set exit flag
       
        //begin:close all other windows
        MMIBROWSER_StopDownload();//MS00237623:不支持下载时的重入,因此先终止下载
        MMIBROWSER_TidyExistedWin();//MS00192555:保证所有已打开的窗口先关闭,除了main窗口
        
        MMK_CloseWin(MMIBROWSER_MAIN_WIN_ID);

        //end:close all other windows
        BRW_DestroyInstance(MMIBROWSER_GetFixedInstance());
        MMIBROWSER_SetFixedInstance(0);
        BrwDestoryPublicProfile();
        BRW_DestructInstanceList();
        
        //BRW_DestroyInstanceList();//destory all instance
        Brw_HttpExit();
        BRW_VisitHisExit();
        BRW_SnapshotExit();
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
        BRW_InputHisExit();
#endif
        BRW_DestroyHash();//protect    
        BRW_DestroyParse();//protect
        MMIBROWSER_ResetStartInfo();
        BRW_DeleteMem();

        //external,especially for memory
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_BROWSER);
#endif

        //MMI_Enable3DMMI(TRUE);

        /*目前dorado浏览器支持背景播放,若以后有些项目或平台有限制,可增加开关重新打开MMIAUDIO_ResumeBgPlay*/
        //MS00192609:MMIAUDIO_ResumeBgPlay(MMIBGPLAY_MODULE_WAP_MAINMENU);
#ifdef JAVA_SUPPORT_SUN
        MMIAPIJAVA_JavaCallClose(TRUE);
#endif
    	
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);//MS00189926
        MMIAPISTK_BrowserTermination(MMIBROWSER_GetSim());
#ifdef BROWSER_START_PAGE_THEME1
        MMIBROWSER_ReleaseSlide();
#endif

#ifdef MMI_BROWSER_MINI_SUPPORT
        MMIBRW_FreeBookmark();
#endif
        MMK_CloseWin(MMIBROWSER_START_PAGE_WIN_ID);
		
    	SCI_TRACE_LOW("[BRW]MMIBROWSER_Exit:end");
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_Exit has done or doing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_354_112_2_18_2_5_45_55,(uint8*)"");
    }
#ifdef MMI_ENVSET_MINI_SUPPORT
    MMIAPIENVSET_ResetActModeKeyRingSet();
#endif
}

/*****************************************************************************/
//  Description : start the web window(access the url or open one snapshot, etc.)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_StartWeb(
                MMIBROWSER_WEB_ENTRY_TYPE_E entry_type,
                uint8 *url_ptr,
                wchar *snapshot_file_name_ptr
                )
{
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_StartWeb:is_browser_inited = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_368_112_2_18_2_5_45_56,(uint8*)"d", MMIBROWSER_GetKernelInit());

    if (!MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
    {
        MMIBROWSER_ResetContext();//init
        MMIBROWSER_SetWebEntryType(entry_type);
        switch (entry_type)
        {
        case MMIBROWSER_WEB_ENTRY_URL:
            MMIBROWSER_UpdateUrl((char *)url_ptr);
            break;
            
        case MMIBROWSER_WEB_ENTRY_SNAPSHOT:
            MMIBROWSER_UpdateSnapshotFilename(snapshot_file_name_ptr);
            break;
            
        default:
            break;
        }   
        
        MMIBROWSER_OpenWebWin();
    }
    else
    {
        MMIBROWSER_SetWebEntryType(entry_type);
        switch (entry_type)
        {
        case MMIBROWSER_WEB_ENTRY_URL:
            MMIBROWSER_EnterNewUrl(url_ptr, FALSE);
            break;
            
        case MMIBROWSER_WEB_ENTRY_SNAPSHOT:
            MMIBROWSER_StopLoading();
            MMIBROWSER_UpdateSnapshotFilename(snapshot_file_name_ptr);
            MMK_PostMsg(MMIBROWSER_MAIN_WIN_ID, MSG_BRW_ENTER_SNAPSHOT_FILENAME, 
                        snapshot_file_name_ptr, 
                        sizeof(wchar)* (MMIAPICOM_Wstrlen(snapshot_file_name_ptr) + 1));
            break;
            
        default:
            MMIBROWSER_StopLoading();
            break;
        }   
        MMK_WinGrabFocus(MMIBROWSER_MAIN_WIN_ID);
    }

    //after the web window is open,open the waiting window for open one snapshot
    if (MMIBROWSER_WEB_ENTRY_SNAPSHOT == entry_type)
    {
        MMIBROWSER_OpenSnapshotWaitingWin();    
    }
    
}

/*****************************************************************************/
//  Description : exit the web (window)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ExitWeb(void)
{
    BRW_INSTANCE    brw_instance = 0;

    if (MMIBROWSER_GetIntanceInit())
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_ExitWeb"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_431_112_2_18_2_5_45_57,(uint8*)"");

        brw_instance = MMIBROWSER_GetInstance();
        BRW_DestroyInstance(brw_instance);
        MMIBROWSER_ResetContext();
        MMIBROWSER_SetIntanceInit(FALSE);

        //external module to do begin:
#ifdef JAVA_SUPPORT_IA
        MMIAPIJAVA_StopOTAInstall(); //通知java 
#endif

        if (!MMK_IsOpenWin(MMIBROWSER_START_PAGE_WIN_ID))//start page isn't open,need to exit
        {
            MMIBROWSER_Exit();
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_ExitWeb has done"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_450_112_2_18_2_5_45_58,(uint8*)"");
    }
    
}
/*****************************************************************************/
//  Description : the entry of browser is permit?
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsPermitEntry(void)
{
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : enter to add a bookmark 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_EnterAddBookmark(const char *url_ptr, const char *url_title_ptr)
{
    if (PNULL != url_ptr)
    {
        MMIBROWSER_BOOKMARK_EDIT_T bookmark = {0};
        wchar url_wchar[MMIBROWSER_MAX_URL_LEN + 1] = {0};
        wchar   title_wchar[MMIBRW_BOOKMARK_MAX_TITLE_LENGTH + 1] = {0};
        uint32  title_len = 0;
#ifdef MMI_BROWSER_MINI_SUPPORT
        if (!MMIBROWSER_GetIsBookmarkLoaded())
        {
            MMIBRW_BookmarkInit();
        }
#endif
#ifndef MMIDORADO_BOOKMARK_BY_NV
        MMIBRW_BookmarkInitAfterStartup();//protect
#endif

        if (MMIBRW_BookmarkGetCount() >= MMIBRW_BOOKMARK_MAX_ITEM)
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMON_BOOKMARK_FULL);

            return MMIBROWSER_FAIL;
        }

        //fenxie_to_do:重入PUB
        MMK_CloseMiddleWin(MMIBROWSER_BOOKMARK_START_WIN_ID, MMIBROWSER_BOOKMARK_END_WIN_ID);

        //convert url
        CFLWCHAR_StrNCpyWithAsciiStr(url_wchar, url_ptr, (MMIBROWSER_MAX_URL_LEN+1));
        bookmark.url_ptr = url_wchar;
        bookmark.url_len = CFLWCHAR_StrLen(url_wchar);
        
        //convert title
        if (PNULL != url_title_ptr)
        {
            title_len = strlen(url_title_ptr);

            bookmark.title_len = GUI_UTF8ToWstr(title_wchar, MMIBRW_BOOKMARK_MAX_TITLE_LENGTH,  (const uint8*)url_title_ptr, title_len);
            bookmark.title_ptr = title_wchar;
        }

        MMIBROWSER_FreeBookmarkEdit();
        MMIBROWSER_UpdateBookmarkEdit(&bookmark);
        MMIBROWSER_OpenBookmarkEditWin(MMIBRW_BM_CHANGE_ADD_URL);
        return MMIBROWSER_SUCC;
    }
    else 
    {
        return MMIBROWSER_ERROR_INPUT; 
    }
}

/*****************************************************************************/
//  Description : select network type(sim or wifi)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SelectNetworkType(MN_DUAL_SYS_E dual_sys, BOOLEAN is_use_wifi)
{   
    BOOLEAN result = TRUE;
    uint16  sim_ok = 0;
    uint32  sim_num = 0;
    
    if (MMIAPISET_GetFlyMode())//NEWMS00111359
    {
        MMIAPISET_AlertFlyMode();
        return FALSE;
    }
    
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_SelectNetworkType:sim_num=%d,dual_sys=%d,is_use_wifi=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_536_112_2_18_2_5_45_59,(uint8*)"ddd", sim_num, dual_sys, is_use_wifi);

#ifdef MMI_WIFI_SUPPORT
    if ((0 == sim_num) || is_use_wifi)
    {
        MMIBROWSER_SetIsUseWifi(TRUE);
        MMIBROWSER_Enter();
    }
    else if ((MMI_DUAL_SYS_MAX > dual_sys) && MMIAPIPHONE_IsSimAvailable(dual_sys))//valid customized dual_sys
    {
        MMIBROWSER_SetSim(dual_sys);
        MMIBROWSER_Enter();
    }
    else
    {
        MMIBROWSER_OpenSimSelectWin();
    }
#else
    if (0 == sim_num)
    {
        MMIBROWSER_SetSim(MMI_DUAL_SYS_MAX);
        MMIBROWSER_Enter();
    }
    else if (1 == sim_num)//only one valid sim,use it
    {
        MMIBROWSER_SetSim((MN_DUAL_SYS_E)sim_ok);
        MMIBROWSER_Enter();
    }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else if ((MMI_DUAL_SYS_MAX > dual_sys) && MMIAPIPHONE_IsSimAvailable(dual_sys))//valid customized dual_sys
    {
        MMIBROWSER_SetSim(dual_sys);
        MMIBROWSER_Enter();
    }
    else
    {
        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

        major_sim_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_WAP);

        if ((major_sim_type < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
            && MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1))/*lint !e656*/ 
            )
        {
            MMIBROWSER_SetSim(MN_DUAL_SYS_1+(major_sim_type-MMISET_MAJOR_SIM_SEL_SIM1));/*lint !e656*/ 
            MMIBROWSER_Enter();
        }
        else
        {
#ifdef MMI_GPRS_DATA_SIM_SET
            MN_DUAL_SYS_E sys = MN_DUAL_SYS_1;
            MMISET_GPRS_MODE_TYPE_E mode = DATA_CARD_MODE_1;

            mode = MMIAPISET_GetGPRSModeType();
    
            if (DATA_CARD_MANUAL_SELECT != mode)
            {
                sys = MN_DUAL_SYS_1 + (mode - DATA_CARD_MODE_1);/*lint !e656*/

                MMIBROWSER_SetSim(sys);

                MMIBROWSER_Enter();
            }
            else
#endif
            {   
                MMIBROWSER_OpenSimSelectWin();
            }
        }
    }
#endif
#endif

    return result;
}
const char* default_url = "http://116.228.149.59/WAP/Download/index_download.html";
/*****************************************************************************/
//  Description : get the url of default homepage for browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetDefaultHomepageUrl(void)
{
    uint8   *homepage_url_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T  *setting_item_ptr = PNULL;
    uint32  index = 0;
    MN_DUAL_SYS_E   active_sim = MMIBROWSER_GetSim();
                
    if (MMIBROWSER_GetIsUseWifi() || (active_sim >= MMI_DUAL_SYS_MAX))//use custom in wifi
    {
        homepage_url_ptr = MMIBROWSER_GetHomepageSetting();
    }
    else
    {
        index = MMIDORADO_GetNetSettingIndex(active_sim);
        setting_item_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(active_sim, index);
        if (PNULL != setting_item_ptr)
        {
            homepage_url_ptr = setting_item_ptr->homepage;
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_Enter MMIAPICONNECTION_GetLinkSettingItemByIndex error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_618_112_2_18_2_5_46_60,(uint8*)"");
        }
    }
	// TODO:temp modify,only for test 
    return default_url;
}

/*****************************************************************************/
//  Description : get the url of homepage for browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetHomepageUrl(void)
{
    MMIBROWSER_HOMEPAGE_MODE_E  homepage_mode = MMIBROWSER_HOMEPAGE_MODE_DEFAULT;
    uint8   *homepage_url_ptr = PNULL;

    homepage_mode = MMIBROWSER_GetHomepageModeSetting();
    switch (homepage_mode)
    {
    case MMIBROWSER_HOMEPAGE_MODE_CUSTOM:
        homepage_url_ptr = MMIBROWSER_GetHomepageSetting();
        break;
        
    default:
        homepage_url_ptr = MMIBROWSER_GetDefaultHomepageUrl();
        break;
    }
    
    return homepage_url_ptr;
}

/*****************************************************************************/
//  Description :set homepage and write to NV
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageUrl(const MMI_STRING_T *string_ptr)
{
    BOOLEAN result = FALSE;

    MMIBROWSER_SetHomepageModeSetting(MMIBROWSER_HOMEPAGE_MODE_CUSTOM);
    result = MMIBROWSER_SetHomepageSetting(string_ptr);
    MMIBROWSER_UpdateNVSetting();
    //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_SetHomepage result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_660_112_2_18_2_5_46_61,(uint8*)"d", result);

#if 0 //homepage don't from connection setting module       
    uint8                               index = 0;
    index = MMIDORADO_GetNetSettingIndex(MMIBROWSER_GetSim());
    result = MMIAPICONNECTION_SetHomepage(MMIBROWSER_GetSim(),index,&string_ptr);
#endif    

    return result;
}

/*****************************************************************************/
//  Description : snapshot open file callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwSnapshotOpenFileCallback(char *url_ptr, BOOLEAN is_succ)
{
    if (is_succ)
    {
        BRW_SnapshotLayoutOfflinePage();
        
        MMIBROWSER_UpdateAddrLable(MMIBROWSER_GetSnapshotFilename(), FALSE);
        MMIBROWSER_UpdateUrl(url_ptr);
        MMIBROWSER_SetIsSnapshot(TRUE);
        MMIBROWSER_UpdateSnapshotUrl(url_ptr);
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_BROWSER_SNAPSHOT_OPEN_FILE_FAIL);
    }
    
    //MMIBROWSER_FreeSnapshotFilename();//the operation is end, free the data

    MMIPUB_CloseWaitWin(MMIBROWSER_OPEN_SNAPSHOT_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Description : open one snapshot file
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_OpenSnapshot(wchar *filename_ptr, BRW_INSTANCE brw_instance, BOOLEAN is_fixed)
{
#ifdef MMIDORADO_FIX_HMTL_SUPPORT
    if (is_fixed && (PNULL == filename_ptr))
    {
        MMIAUD_RING_DATA_INFO_T file_info = {0};/*lint !e64*/

		MMI_GetDataInfo(R_DEFAULT_HTML, &file_info);

        BRW_SnapshotParseOfflinePageEx(file_info.data_ptr, file_info.data_len, brw_instance, BrwSnapshotOpenFileCallback, is_fixed);
        BRW_SnapshotLayoutOfflinePage();
        return;
    }
#endif

    if ((PNULL != filename_ptr) && (MMIAPICOM_Wstrlen(filename_ptr) > 0) && (brw_instance != BRW_INVALID_INSTANCE))
    {
        if (BRW_SnapshotParseOfflinePage(filename_ptr, brw_instance, BrwSnapshotOpenFileCallback))
        {
            return;//success
        }
    }
    
    //OPEN FAIL
    BrwSnapshotOpenFileCallback(PNULL, FALSE);
}

/*****************************************************************************/
//  Description : set number state to string 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void BrwSetNumState2String(
                              MMI_STRING_T *string_ptr,//OUT:string_ptr->wstr_ptr need free outside
                              uint16 total_num,//[IN]
                              uint16 max_num//[IN]
                              )
{
    uint8 num_state[NUM_STATE_MAX + 1] = {0};
    int32    num_len = 0;

    if (string_ptr != PNULL)
    {
        SCI_MEMSET(string_ptr,0x00, sizeof(MMI_STRING_T));
        
        num_len = CFL_Snprintf((char *)num_state, NUM_STATE_MAX, "(%d/%d)", total_num, max_num);
        if (num_len > 0)
        {
            string_ptr->wstr_len = num_len;
            string_ptr->wstr_ptr = SCI_ALLOCA(sizeof(wchar) * (string_ptr->wstr_len + 1));
            if (string_ptr->wstr_ptr != PNULL)
            {
                SCI_MEMSET(string_ptr->wstr_ptr, 0x00, (sizeof(wchar) * (string_ptr->wstr_len + 1)));
                MMI_STRNTOWSTR(string_ptr->wstr_ptr, string_ptr->wstr_len, 
                    (const uint8 *)num_state, NUM_STATE_MAX, 
                    string_ptr->wstr_len);
            }
            else
            {
                string_ptr->wstr_len = 0;
                //SCI_TRACE_LOW:"[BRW]W SetNumState2String alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_759_112_2_18_2_5_46_62,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW]W SetNumState2String ERROR"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_764_112_2_18_2_5_46_63,(uint8*)"");
        }
    }

}

/*****************************************************************************/
//  Description : merge the text id and num state("bookmark(m/n)".etc)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_MergeTextAndNum(
                MMI_STRING_T *string_ptr,//OUT:string_ptr->wstr_ptr need free outside
                MMI_TEXT_ID_T label_id,//[IN]
                uint16 total_num,//[IN]
                uint16 max_num//[IN]
                )
{
    MMI_STRING_T    text_num_str = {0};
    MMI_STRING_T    text_title_str = {0};
    const uint16   max_string_len = NUM_STATE_AND_TEXT_MAX;

    if (PNULL == string_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_MergeTextAndNum error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_787_112_2_18_2_5_46_64,(uint8*)"");
        return;
    }
    
    BrwSetNumState2String(&text_num_str, total_num, MMIBRW_BOOKMARK_MAX_ITEM);
    MMI_GetLabelTextByLang(label_id, &text_title_str);
    
    string_ptr->wstr_len = text_num_str.wstr_len + text_title_str.wstr_len;
    
    if (string_ptr->wstr_len > max_string_len)//protect
    {
        string_ptr->wstr_len = max_string_len;
    }
    
    string_ptr->wstr_ptr = SCI_ALLOCA(sizeof(wchar) * (string_ptr->wstr_len + 1));
    if (string_ptr->wstr_ptr != PNULL)
    {
        SCI_MEMSET(string_ptr->wstr_ptr, 0x00, (sizeof(wchar) * (string_ptr->wstr_len + 1)));
        MMIAPICOM_MergeTwoString(
                &text_title_str,
                &text_num_str,
                string_ptr,
                string_ptr->wstr_len
                );
    }
    else
    {
        string_ptr->wstr_len = 0;
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_MergeTextAndNum alloc fail--text_str.wstr_len =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_815_112_2_18_2_5_46_65,(uint8*)"d", string_ptr->wstr_len);
    }

    if (text_num_str.wstr_ptr != PNULL)
    {
        SCI_FREE(text_num_str.wstr_ptr);
    }

}

/*****************************************************************************/
//  Description : split pre pathname to device, dir
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SplitPrePath(      //支持多级目录
                              const wchar      *pre_path_ptr,     //in
                              uint16           pre_path_len,      //in, 双字节为单位
                              wchar            *device_ptr,        //out, one of device_ptr, dir_ptr and file_name_ptr should be NOT NULL
                              uint16           *device_len_ptr,    //in, 双字节为单位
                              wchar            *dir_ptr,           //out
                              uint16           *dir_len_ptr       //in, 双字节为单位
                              )
{
    wchar   temp_pre_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16  temp_pre_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN result = FALSE;
    
    if ((PNULL == pre_path_ptr) || (0 == pre_path_len) || ((MMIFILE_FULL_PATH_MAX_LEN - 1) < pre_path_len))
    {
        return FALSE;
    }

    MMIAPICOM_Wstrcpy(temp_pre_path, pre_path_ptr);
    *(temp_pre_path + pre_path_len) = MMIFILE_SLASH;
   
    temp_pre_path_len = MMIAPICOM_Wstrlen(temp_pre_path);
    
    result = MMIAPIFMM_SplitFullPath(temp_pre_path, temp_pre_path_len, 
                    device_ptr, device_len_ptr, 
                    dir_ptr, dir_len_ptr,
                    PNULL, PNULL
                    );
                    
    if (result)
    {
        if ((PNULL != dir_ptr) && (PNULL != dir_len_ptr)
            && (MMIFILE_SLASH == *dir_ptr) && (1 == *dir_len_ptr)) //根目录
        {
            *dir_ptr = 0X00;
            *dir_len_ptr = 0;
        }
    }
                    
    return result;
}
/*****************************************************************************/
//  Description : convert the fmm path to display format(for example,"D:"->"phone:")
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_FmmPathToDisplay(
                                      const MMI_STRING_T *path_str_ptr,//[IN]
                                      MMI_STRING_T *display_path_str_ptr//[IN/OUT]
                                      )
{
    MMI_STRING_T    device_display_str = {0};
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    wchar   device_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  device_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar   dir_name[MMIFILE_DIR_NAME_MAX_LEN + 1] = {0};
    uint16  dir_name_len = MMIFILE_DIR_NAME_MAX_LEN;

    if ((PNULL == path_str_ptr) || (PNULL == path_str_ptr->wstr_ptr) 
        || (PNULL == display_path_str_ptr) || (PNULL == display_path_str_ptr->wstr_ptr) )
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_FmmPathToDisplay param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_889_112_2_18_2_5_46_66,(uint8*)"");
        return MMI_DEVICE_NUM;
    }

    MMIBROWSER_SplitPrePath(path_str_ptr->wstr_ptr, path_str_ptr->wstr_len, 
        device_name, &device_name_len, 
        dir_name, &dir_name_len
        );
        
    dev = MMIAPIFMM_GetDeviceTypeByPath(device_name, device_name_len);
    if (dev < MMI_DEVICE_NUM)
    {
        MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(dev), &device_display_str);
        MMIAPIFMM_CombineFullPath(device_display_str.wstr_ptr, device_display_str.wstr_len,
                dir_name, dir_name_len,
                PNULL, PNULL,
                display_path_str_ptr->wstr_ptr, &(display_path_str_ptr->wstr_len));
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_FmmPathToDisplay err dev"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_909_112_2_18_2_5_46_67,(uint8*)"");
    }

    return dev;
}

/*****************************************************************************/
//  Description : convert the display text to fmm path(for example,"phone:"-> "D:")
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_DisplayToFmmPath(
                                      const MMI_STRING_T *display_path_str_ptr,//[IN]
                                      MMI_STRING_T *path_str_ptr//[IN/OUT]
                                      )
{
    MMIFILE_DEVICE_E device_type = MMIBROWSER_GetSaveDeviceType();
    wchar   dir_name[MMIFILE_DIR_NAME_MAX_LEN + 1] = {0};
    uint16  dir_name_len = MMIFILE_DIR_NAME_MAX_LEN;

    if ((PNULL == path_str_ptr) || (PNULL == path_str_ptr->wstr_ptr) 
        || (PNULL == display_path_str_ptr) || (PNULL == display_path_str_ptr->wstr_ptr) )
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_DisplayToFmmPath param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_931_112_2_18_2_5_46_68,(uint8*)"");
        return;
    }
    
    MMIBROWSER_SplitPrePath(display_path_str_ptr->wstr_ptr, display_path_str_ptr->wstr_len, 
            //device_name, &device_name_len, 
            PNULL, PNULL,
            dir_name, &dir_name_len
            );

    if(device_type < MMI_DEVICE_NUM)//normal
    {
        MMIAPIFMM_CombineFullPath(MMIAPIFMM_GetDevicePath(device_type), MMIAPIFMM_GetDevicePathLen(device_type),
                dir_name, dir_name_len,
                PNULL, PNULL,
                path_str_ptr->wstr_ptr, &(path_str_ptr->wstr_len));
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_DisplayToFmmPath err dev"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_950_112_2_18_2_5_46_69,(uint8*)"");
    }
}


/*****************************************************************************/
//  Description : Appoint export save path text
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppointSaveMediaFilePath(wchar *path_ptr, uint32 path_len, MMIBRW_OpenFileCallBack callback)
{
    FILEARRAY ret_array = PNULL;
    MMIFMM_FILTER_T filter = {0};
    MMIFMM_OPENWIN_RET_E ret = FMM_OPENWIN_RET_NULL;

    ret_array = MMIAPIFILEARRAY_Create();

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));

    ret = MMIAPIFMM_OpenFileWinByLayer(path_ptr,
                                    (uint16)path_len,
                                    &filter,
                                    FUNC_FIND_FOLDER,
                                    FALSE,
                                    NULL,
                                    ret_array,
                                    callback,
                                    FALSE);

    if (FMM_OPENWIN_SUCCESS != ret)
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_CombineDownloadFile(
                                             MMI_STRING_T *path_ptr,
                                             MMI_STRING_T *name_ptr,
                                             wchar      *full_path_ptr,     //out
                                             uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                             BOOLEAN is_combine_tmp_file
                                             )
{
    wchar dir_mark[] = {'\\', 0};
    uint16 dir_mark_len = 1;
    uint16 tmp_ext_len = 0;

    if (PNULL == path_ptr || PNULL == name_ptr || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1007_112_2_18_2_5_46_70,(uint8*)"ddd", name_ptr->wstr_len, path_ptr->wstr_len,is_combine_tmp_file);

    if (is_combine_tmp_file)
    {
        tmp_ext_len = strlen(MMIBROWSER_TMPFILE_EXT);
    }

    if (((name_ptr->wstr_len + path_ptr->wstr_len + dir_mark_len + tmp_ext_len)) < MMIFILE_FULL_PATH_MAX_LEN)
    {
        MMI_WSTRNCPY(full_path_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN,
                                path_ptr->wstr_ptr,
                                path_ptr->wstr_len,
                                path_ptr->wstr_len);

        *full_path_len_ptr = path_ptr->wstr_len;

        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                dir_mark,
                                dir_mark_len,
                                dir_mark_len);

        *full_path_len_ptr += dir_mark_len;

        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                name_ptr->wstr_ptr,
                                name_ptr->wstr_len,
                                name_ptr->wstr_len);

        *full_path_len_ptr += name_ptr->wstr_len;

        if (is_combine_tmp_file)
        {
            MMI_STRNTOWSTR(full_path_ptr + *full_path_len_ptr, 
                                    MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                    (uint8 *)MMIBROWSER_TMPFILE_EXT,
                                    tmp_ext_len,
                                    tmp_ext_len);

            *full_path_len_ptr += tmp_ext_len;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : get default media dir for the file to be downloaded
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void BrowserDlGetDefaultMediaDir(
                uint16  **dir_path_pptr,//[IN/OUT]
                uint16 *dir_path_len_ptr,//[IN/OUT]
                BRW_MIME_TYPE_E mime_type
                )
{
    if ((PNULL == dir_path_pptr) || (PNULL == dir_path_len_ptr))
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER BrowserDlGetDefaultMediaDir:path_ptr=0x%x,dir_path_len_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1071_112_2_18_2_5_47_71,(uint8*)"dd",dir_path_pptr,dir_path_len_ptr);
        return;
    }

    if ((BRW_MIME_IMG_BMP <= mime_type) && (BRW_MIME_IMG_UNKNOWN >= mime_type))
    {
        *dir_path_pptr = (uint16*)MMIMULTIM_DIR_PICTURE;
    }
    else if ((BRW_MIME_AUDIO_AMR <= mime_type) && (BRW_MIME_AUDIO_UNKNOWN >= mime_type))
    {
        *dir_path_pptr = (uint16*)MMIMULTIM_DIR_MUSIC;
    }
    else if ((BRW_MIME_VIDEO_3GP <= mime_type) && (BRW_MIME_VIDEO_UNKNOWN >= mime_type))
    {
        *dir_path_pptr = (uint16*)MMIMULTIM_DIR_MOVIE;
    }
#ifdef EBOOK_SUPPORT//MS00187401 
    else if (BRW_MIME_PLAINTEXT == mime_type)
    {
        *dir_path_pptr = (uint16*)MMIMULTIM_DIR_EBOOK;
    }
#endif
#ifdef DRM_SUPPORT
    else if ((BRW_MIME_OMA_DRM_MESSAGE <= mime_type) && (BRW_MIME_OMA_DRM_RIGHTS_WBXML >= mime_type))
    {
        *dir_path_pptr = (uint16*)MMIDRMFILE_SPTMP_DIR;
    }
#endif
    else
    {
        *dir_path_pptr = (uint16*)MMIMULTIM_OTHER_DEFAULT_DIR;
    }

    if (*dir_path_pptr != PNULL)
    {
        *dir_path_len_ptr = MMIAPICOM_Wstrlen(*dir_path_pptr);
    }
    
}

/*****************************************************************************/
//  Description : get the first valid device name for download
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E BrowserDlGetDeviceName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr, 
                BRW_MIME_TYPE_E mime_type)
{
    MMIFILE_DEVICE_E file_type = MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
    
    if (file_type < MMI_DEVICE_NUM)
    {
        if (PNULL != device_name_pptr)
        {
            *device_name_pptr = MMIAPIFMM_GetDevicePath((file_type));
        }
        
        if (PNULL != device_name_len_ptr)
        {
            *device_name_len_ptr = MMIAPIFMM_GetDevicePathLen((file_type));
        }        
    }
    else
    {
    }
    
    //SCI_TRACE_LOW:"[BRW]BrowserDlGetDeviceName file_type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1147_112_2_18_2_5_47_72,(uint8*)"d", file_type);

    return file_type;
}

/*****************************************************************************/
//  Description : geth default media path for download(for example"D:/audio")
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetDefaultMediaFilePath(
                    wchar      *full_path_ptr,     //out
                    uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                    BRW_MIME_TYPE_E mime_type
                    )
{
    wchar   *device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    const wchar *dir_ptr = PNULL;
    uint16 dir_len = 0;
    MMIFILE_DEVICE_E file_type = MMI_DEVICE_NUM;

    if ((PNULL == full_path_ptr) || (PNULL == full_path_len_ptr))
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetDefaultMediaFilePath full_path_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1170_112_2_18_2_5_47_73,(uint8*)"d", full_path_ptr);
        return MMI_DEVICE_NUM;
    }    
    
    file_type = BrowserDlGetDeviceName(&device_name_ptr, &device_name_len, mime_type);
    
    if ((PNULL == device_name_ptr) || (0 == device_name_len))
    {
        return MMI_DEVICE_NUM;
    }
    
    BrowserDlGetDefaultMediaDir((uint16 **)&dir_ptr, &dir_len, mime_type);

    if (MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
                dir_ptr, dir_len,
                PNULL, PNULL,
                full_path_ptr, full_path_len_ptr))
    {
        return file_type;
    }
    else
    {
        return MMI_DEVICE_NUM;//Combine error
    }
    
}

/*****************************************************************************/
//  Description : when save one media file, whether only choose the storage for user by mime type?
//  Global resource dependence : 
//  Author:fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsOnlyChooseStorage(BRW_MIME_TYPE_E mime_type)
{
    BOOLEAN is_only_storage = FALSE;

    //path or storage
    if (((BRW_MIME_OMA_DRM_MESSAGE <= mime_type)&& (BRW_MIME_OMA_END > mime_type)))
    {
        is_only_storage = TRUE;
    }

    return is_only_storage;
}

#ifdef TOUCH_GESTURE_ENABLE
/*****************************************************************************/
//  Description : handle zoom msg
//  Global resource dependence : 
//  Author:fen.xie
//  Note:  retrun->is_zoom
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_HandleZoom(MMI_GESTURE_TYPE_E gesture_type)
{
    BRW_PROFILE_T   profile = {0};
    MMIBROWSER_FONT_E   font_type = MMIBROWSER_FONT_MAX;
    BOOLEAN is_need_zoom = FALSE;
    
    font_type = MMIBROWSER_GetFontSetting();

    if (MMI_GESTURE_ZOOM_OUT == gesture_type) 
    {
        if (font_type > MMIBROWSER_FONT_MINI)
        {
            font_type--;
            is_need_zoom = TRUE;
        }
        else
        {
            //do nothing
        }
    }
    else if (MMI_GESTURE_ZOOM_IN == gesture_type)
    {
        if (font_type < MMIBROWSER_FONT_BIG)
        {
            font_type++;
            is_need_zoom = TRUE;
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        //do nothing       
    }
    
    is_need_zoom = (font_type < MMIBROWSER_FONT_MAX)? is_need_zoom: FALSE;
    
    if (is_need_zoom)
    {
        MMIBROWSER_SetFontSetting(font_type);
        profile = MMIBROWSER_SetProfile();
        BRW_SetParam(MMIBROWSER_GetInstance(),&profile,BRW_FONT_PROFILE);
    }
    
    SCI_TRACE_LOW("[BRW]MMIBROWSER_HandleZoom:gesture_type = %d, is_need_zoom=%d", gesture_type, is_need_zoom);
    return is_need_zoom;
}
#endif

/*****************************************************************************/
//  Description : get sys dev name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetBrwSysDevName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    if (device_name_pptr != PNULL)
    {
        *device_name_pptr = BRW_GetSysDevName(device_name_len_ptr);
        dev = MMIAPIFMM_GetDeviceTypeByPath(*device_name_pptr, *device_name_len_ptr);
    }

    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_GetBrwSysDevName dev =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1289_112_2_18_2_5_47_75,(uint8*)"d", dev);
	return dev;
}
/*****************************************************************************/
//  Description : get cookie dev name
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetBrwCookieDevName(
                wchar **device_name_pptr, 
                uint16 *device_name_len_ptr)
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    if (device_name_pptr != PNULL)
    {
        *device_name_pptr = BRW_GetCookieDevName(device_name_len_ptr);
        dev = MMIAPIFMM_GetDeviceTypeByPath(*device_name_pptr, *device_name_len_ptr);
    }
    SCI_TRACE_LOW("[BRW]MMIBROWSER_GetBrwCookieDevName dev = %d\n", dev);
	return dev;
}

/*****************************************************************************/
//  Description : get cur page's url info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 与MMIBROWSER_FreeCurPageUrlInfo配对使用
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetCurPageUrlInfo(
                uint8 **url_pptr, //[IN/OUT]:need to free *url_pptr outside
                wchar **wurl_pptr//[IN/OUT]:need to free *wurl_pptr outside
                )
{
    uint8   *url_buf_ptr = PNULL;
    wchar   *wurl_buf_ptr = PNULL;
    BOOLEAN is_snapshot = FALSE;
    BOOLEAN is_blank_page = FALSE;
    BRW_INSTANCE instance = MMIBROWSER_GetInstance();
    uint16  url_buf_max_size = (BRW_MAX_URL_LEN + 1);
    uint16  url_len = 0;
    BOOLEAN result = TRUE;

    if ((PNULL == url_pptr) && (PNULL == wurl_pptr))
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetCurPageUrlInfo error param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1313_112_2_18_2_5_47_76,(uint8*)"");
        return FALSE;        
    }

    url_buf_ptr = SCI_ALLOCA(url_buf_max_size);
    if (PNULL == url_buf_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetCurPageUrlInfo url_ptr alloc fails"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1320_112_2_18_2_5_47_77,(uint8*)"");
        return FALSE;        
    }
    SCI_MEMSET(url_buf_ptr, 0x00, url_buf_max_size);
    
    if (BRW_GetPageUrl(instance, url_buf_ptr, (url_buf_max_size - 1), &is_snapshot, &is_blank_page))
    {
        if (is_snapshot)
        {
            SCI_MEMSET(url_buf_ptr, 0x00, url_buf_max_size);
            url_len = SCI_STRLEN(MMIBROWSER_GetSnapshotUrl());
            strncpy((char*)url_buf_ptr, MMIBROWSER_GetSnapshotUrl(), MIN(url_len, BRW_MAX_URL_LEN));//temp
        }
        else if (is_blank_page)
        {
            char    *url_ptr = MMIBROWSER_GetUrl();
            
            //is_snapshot need to get the snapshot url info
            SCI_MEMSET(url_buf_ptr, 0x00, url_buf_max_size);
#ifdef MMI_BROWSER_MINI_SUPPORT
            if (PNULL == url_ptr)
            {
                result =  FALSE;
            }
            else
#endif
            {
                url_len = SCI_STRLEN(url_ptr);
                strncpy((char*)url_buf_ptr, url_ptr, MIN(url_len, BRW_MAX_URL_LEN));//temp
            }
        }
        else
        {
            //normal page's url info
        }
        
        url_len = SCI_STRLEN((char*)url_buf_ptr);
        
        if ((wurl_pptr != PNULL) && (url_len > 0))
        {
            wurl_buf_ptr = SCI_ALLOCA((url_len + 1) * sizeof(wchar));
            if (PNULL != wurl_buf_ptr)
            {
                SCI_MEMSET(wurl_buf_ptr, 0x00, ((url_len + 1) * sizeof(wchar)));
                MMI_STRNTOWSTR(wurl_buf_ptr, url_len, url_buf_ptr, url_len, url_len);
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetCurPageUrlInfo wurl_buf_ptr alloc fails"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1357_112_2_18_2_5_47_78,(uint8*)"");
                result =  FALSE;
            }
        }
    }
    else
    {
        result =  FALSE;
    }

    if (!result)//error
    {
        SCI_FREE(url_buf_ptr);
        
        if (wurl_buf_ptr != PNULL)
        {
            SCI_FREE(wurl_buf_ptr);
        }
    }
    else
    {
        if (url_pptr != PNULL)
        {
            *url_pptr = url_buf_ptr;
        }

        if (wurl_pptr != PNULL)
        {
            *wurl_pptr = wurl_buf_ptr;
        }
    }
    
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_GetCurPageUrlInfo result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1389_112_2_18_2_5_47_79,(uint8*)"d", result);

    return result;
}

/*****************************************************************************/
//  Description : free memory for get cur page's url info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeCurPageUrlInfo(
                uint8 **url_pptr, //[IN/OUT]
                wchar **wurl_pptr//[IN/OUT]
                )
{
    if (url_pptr != PNULL)
    {
        SCI_FREE(*url_pptr);
    }
    
    if (wurl_pptr != PNULL)
    {
        SCI_FREE(*wurl_pptr);
    }
    
}

/*****************************************************************************/
//  Description : get cur address label info
//  Global resource dependence : 
//  Author: fen.xie
//  Note:  与MMIBROWSER_FreeCurAddrLabelInfo配对使用
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetCurAddrLabelInfo(wchar **label_info_pptr)//[IN/OUT]:need to free *label_info_pptr outside
{
    uint8   *url_ptr = PNULL;
    BOOLEAN is_snapshot = FALSE;
    BOOLEAN is_blank_page = FALSE;
    BRW_INSTANCE instance = MMIBROWSER_GetInstance();
    BOOLEAN is_has_title = FALSE;
    wchar   *label_info_ptr = PNULL;
    uint16  label_info_max_size = (BRW_MAX_URL_LEN + 1) * sizeof(wchar);
    uint16  label_info_len = 0;

    if (PNULL == label_info_pptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_pptr null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1432_112_2_18_2_5_47_80,(uint8*)"");
        return FALSE;        
    }

    label_info_ptr = SCI_ALLOCA(label_info_max_size);
    if (PNULL == label_info_ptr)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetCurAddrLabelInfo label_info_ptr alloc fails"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1439_112_2_18_2_5_47_81,(uint8*)"");
        return FALSE;        
    }
    SCI_MEMSET(label_info_ptr, 0x00, label_info_max_size);

    BRW_GetPageUrl(instance, PNULL, 0, &is_snapshot, &is_blank_page);    
    
    if (is_snapshot)//get the file nam of snapshot
    {
        label_info_len = MMIAPICOM_Wstrlen(MMIBROWSER_GetSnapshotFilename());
        if (label_info_len > 0)
        {
            MMI_WSTRNCPY(label_info_ptr, BRW_MAX_URL_LEN, 
                MMIBROWSER_GetSnapshotFilename(), label_info_len,
                MIN(label_info_len, BRW_MAX_URL_LEN));
        }
    }
    else if (is_blank_page)
    {
        url_ptr = (uint8*)MMIBROWSER_GetUrl();

        label_info_len = SCI_STRLEN((char*)url_ptr);
        if (label_info_len > 0)
        {
            MMI_STRNTOWSTR(label_info_ptr, BRW_MAX_URL_LEN, 
                url_ptr, label_info_len,
                MIN(label_info_len, BRW_MAX_URL_LEN));
        }
    }
    else
    {
        uint16  title_max_len = MIN(MMIBROWSER_MAX_TITLE_LEN, BRW_MAX_URL_LEN);
        is_has_title = BRW_GetPageTitle(MMIBROWSER_GetInstance(), label_info_ptr, 
                        &label_info_len, 
                        title_max_len);
        if (is_has_title && (label_info_len> 0))
        {
            //get the title
        }
        else//get current page's url
        {
            MMIBROWSER_GetCurPageUrlInfo(&url_ptr, PNULL);
            if ((PNULL != url_ptr) && ((label_info_len = SCI_STRLEN((char*)url_ptr)) > 0))
            {
                MMI_STRNTOWSTR(label_info_ptr, BRW_MAX_URL_LEN, 
                    url_ptr, label_info_len,
                    MIN(label_info_len, BRW_MAX_URL_LEN));
            }
            MMIBROWSER_FreeCurPageUrlInfo(&url_ptr, PNULL);
        }
    }

    *label_info_pptr = label_info_ptr;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : free cur address label info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeCurAddrLabelInfo(wchar **label_info_pptr)//[IN/OUT]
{
    if (label_info_pptr != PNULL)
    {
        SCI_FREE(*label_info_pptr);
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendTwoTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *string_1, MMI_STRING_T *string_2, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    if (PNULL != string_1)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_1->wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = string_1->wstr_len;
    }

    if (PNULL != string_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = string_2->wstr_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = string_2->wstr_len;
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_ICON;
    }
 
    if (IMAGE_NULL != image_id)
    {
        item_data.item_content[1].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.text_id = image_id;
    }

    GUILIST_AppendItem(ctrl_id, &item_t);
}


/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author:Jiaoyou.wu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendSplitListItem(MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id)
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/

    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;

    item_t.item_data_ptr = &item_data;
    item_t.item_state |=GUIITEM_STATE_SPLIT;

    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id  = text_id;

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreateDynamicPopupRadioMenuByString(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, GUIMENU_BUTTON_STYLE_E style, MMI_TEXT_ID_T title_id, uint16 node_num, MMI_STRING_T *string_array, uint32 *node_array, uint32 select_index)
{
    uint16 index = 0;
    MMI_STRING_T string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUIMENU_BUTTON_INFO_T button_info = {0};
    GUIMENU_POP_SEL_INFO_T  pop_item_info  = {0};

    if (PNULL == string_array)
    {
        return;
    }

    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP_RADIO);

    if (TXT_NULL != title_id)
    {
        MMI_GetLabelTextByLang(title_id, &string);
    }

    GUIMENU_SetMenuTitle(&string, ctrl_id);

    for (index = 0; index < node_num; index ++)
    {
        node_item.item_text_ptr = &string_array[index];
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, node_array[index], 0, &node_item, ctrl_id);
    }

    button_info.is_static   = FALSE;
    button_info.button_style = style;
    GUIMENU_SetButtonStyle(ctrl_id, &button_info);

    pop_item_info.ctrl_id = ctrl_id;
    pop_item_info.node_id = node_array[select_index];

    GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreateRadioListByTextId(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id,  uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array, uint32 select_index)
{
    GUILIST_ITEM_T          item_t = {0};
    GUILIST_ITEM_DATA_T     item_data = {0};
    uint16                  index = {0};

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;

    GUILIST_SetMaxItem(ctrl_id, node_num, FALSE);

    for (index = 0; index < node_num; index++)
    {
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = text_id_array[index];
        item_t.user_data                            = node_array[index];

        GUILIST_AppendItem(ctrl_id, &item_t);
    }

    GUILIST_SetSelectedItem(ctrl_id, select_index, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id, select_index);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_AppendOneTextOneImageItem(MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *text_1, MMI_IMAGE_ID_T image_id, uint32 user_data)
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};

    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;

    if (IMAGE_NULL != image_id)
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;

        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.text_id = image_id;

        if(PNULL != text_1)
        {
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr = text_1->wstr_ptr;
            item_data.item_content[1].item_data.text_buffer.wstr_len = text_1->wstr_len;
        }
    }
    else
    {
        item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT;

        if(PNULL != text_1)
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = text_1->wstr_ptr;
            item_data.item_content[0].item_data.text_buffer.wstr_len = text_1->wstr_len;
        }
    }
    GUILIST_AppendItem(ctrl_id, &item_t);
}

/****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/****************************************************************************/
PUBLIC void MMIBROWSER_CreatePopupAutoMenu(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_STRING_T *title_str, uint16 node_num, MMI_TEXT_ID_T *text_id_array, uint32 *node_array)
{
    uint16 index = 0;
    MMI_STRING_T string = {0};
    GUIMENU_DYNA_ITEM_T node_item = {0};
    GUIMENU_BUTTON_INFO_T button_info = {0};

    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP_AUTO);

    if (PNULL != title_str)
    {
        GUIMENU_SetMenuTitle(title_str, ctrl_id);
    }

    for (index = 0; index < node_num; index ++)
    {
        MMI_GetLabelTextByLang(text_id_array[index], &string);
        node_item.item_text_ptr = &string;
        node_item.is_grayed = FALSE;
        GUIMENU_InsertNode(index, node_array[index], 0, &node_item, ctrl_id);
    }

    button_info.is_static   = FALSE;
    button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
    GUIMENU_SetButtonStyle(ctrl_id, &button_info);
}

/*****************************************************************************/
// 	Description : set watch dog pointer for dorado's js
//	Global resource dependence : 
//  Author:fen.xie
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDORADO_SetWatchDogPtr(void *SwdgPtr)
{
#ifdef JS_SUPPORT
    BrwSetWatchDogPtr((void *)SwdgPtr);
#endif
}

#ifdef MMIDORADO_MY_NAV_SUPPORT
/*****************************************************************************/
//  Description : get the default of my navigation
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
LOCAL void GetMyNavDefaultInfo(MMIBRW_MY_NAV_CONTEXT_T *nv_info_ptr)
{
    if (nv_info_ptr != PNULL)
    {
#if 0//customize
        uint32 i = 0;
        MMI_STRING_T title_str = {0};
        uint16  url_len = 0;
        MMIBRW_MY_NAV_ITEM_T  *item_info_ptr = PNULL;
        uint32  default_max_len = 0;
        const MMIBRW_MY_NAV_DEFAULT_ITEM_T  default_info[] = 
        {
            {TXT_MAINMENU_ICON_BAIDU, "http://m.baidu.com/?from=1214a", IMAGE_MAINMENU_ICON_BAIDU},
            {TXT_MAINMENU_ICON_MOBILESOHU, "http://wap.sohu.com/?v=3&fr=zhanxun_zm1_0923", IMAGE_MAINMENU_ICON_MOBILESOHU},
        };//demo

        SCI_MEMSET(nv_info_ptr, 0x00, sizeof(MMIBRW_MY_NAV_CONTEXT_T));
        
        default_max_len = sizeof(default_info)/sizeof(default_info[0]);
        for (i = 0; i < MIN(MMIBRW_MY_NAV_MAX_ITEM, default_max_len); i++)
        {
            MMI_GetLabelTextByLang(default_info[i].title_text_id, &title_str);
            
            if ((default_info[i].url_ptr != PNULL) && ((url_len = SCI_STRLEN(default_info[i].url_ptr)) > 0)
                && (title_str.wstr_ptr != PNULL) && (title_str.wstr_len > 0))//valid
            {
                item_info_ptr = &(nv_info_ptr->item_info[i]);

                item_info_ptr->item_type = MMIBRW_MY_NAV_ITEM_TYPE_FIXED;
                if (default_info[i].img_id != 0)
                {
                    item_info_ptr->img_id = default_info[i].img_id;
                }
                item_info_ptr->title_info_u.title_text_id = default_info[i].title_text_id;
                
                url_len = SCI_STRLEN(default_info[i].url_ptr);
                SCI_MEMCPY(item_info_ptr->url_arr, default_info[i].url_ptr, MIN(url_len, MMIBRW_MY_NAV_MAX_URL_LENGTH));
                
                //nv_info_ptr->item_count++;
            }
        }
#else
	SCI_MEMSET(nv_info_ptr, 0x00, sizeof(MMIBRW_MY_NAV_CONTEXT_T));
#endif		
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]GetMyNavDefaultInfo:PARAM null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1883_112_2_18_2_5_48_82,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : get my navigation info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetMyNavInfo(MMIBRW_MY_NAV_CONTEXT_T *nv_info_ptr)
{
    MN_RETURN_RESULT_E result = MN_RETURN_FAILURE;

    if (nv_info_ptr != PNULL)
    {
        MMINV_READ(MMINV_BROWSER_MY_NAV_INFO, nv_info_ptr, result);
        if(result != MN_RETURN_SUCCESS)
        {
            GetMyNavDefaultInfo(nv_info_ptr);
            MMINV_WRITE(MMINV_BROWSER_MY_NAV_INFO, nv_info_ptr);
        }
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : set my navigation info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetMyNavInfo(MMIBRW_MY_NAV_CONTEXT_T *nv_info_ptr)
{
    if (nv_info_ptr != PNULL)
    {
        MMINV_WRITE(MMINV_BROWSER_MY_NAV_INFO, nv_info_ptr);
    }
}

/*****************************************************************************/
//  Description : reset the my navigation information
//  Global resource dependence : MMINV_BROWSER_MY_NAV_INFO
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetMyNavInfo(void)
{
    MMIBRW_MY_NAV_CONTEXT_T   *nv_info_ptr = PNULL;

    nv_info_ptr = SCI_ALLOCAZ(sizeof(MMIBRW_MY_NAV_CONTEXT_T));
    if (nv_info_ptr != PNULL)
    {
        GetMyNavDefaultInfo(nv_info_ptr);
        MMINV_WRITE(MMINV_BROWSER_MY_NAV_INFO, nv_info_ptr);
        SCI_FREE(nv_info_ptr);
    }
    else
    {
        SCI_TRACE_LOW("[BRW]W MMIBROWSER_ResetMyNavInfo:alloc fail");
    }
    
    MMIBROWSER_DeleteIcoDir();
}

/*****************************************************************************/
//  Description : get the full path of the ico
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIcoPathInfo(
                    wchar   *path_ptr,     //in/out
                    uint16  *path_len_ptr,  //in/out, 双字节为单位
                    MMIFILE_DEVICE_E    *dev_ptr   //in/out,may be null
                    )
{
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    BOOLEAN result = FALSE;
    const wchar    *path_name_ptr = (wchar*)MMIBRW_ICO_ROOT_PATH;
    uint16  path_name_len = MMIBRW_ICO_ROOT_PATH_LEN;                                     

#ifdef NANDBOOT_SUPPORT
    dev = MMI_DEVICE_UDISK;
#else
    dev = MMIAPIFMM_GetFirstValidSD();
#endif
    {
        if (MMI_DEVICE_NUM > dev)
        {
            wchar   *dst_path_name_ptr = PNULL;
            uint16  dst_path_name_len = 0;
            wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\'};
            uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN+1);

            dst_path_name_len = (path_name_len + syspath_name_len);
            dst_path_name_ptr = (wchar *)SCI_ALLOCAZ(sizeof(wchar) * (dst_path_name_len+1));
            if (dst_path_name_ptr != PNULL)
            {
                MMI_WSTRNCPY(dst_path_name_ptr, dst_path_name_len, syspath_name_ptr, syspath_name_len, syspath_name_len);
                MMI_WSTRNCPY(dst_path_name_ptr+syspath_name_len, path_name_len, path_name_ptr, path_name_len, path_name_len);

                result = MMIAPIFMM_CombineFullPath(
                    MMIAPIFMM_GetDevicePath(dev), 
                    MMIAPIFMM_GetDevicePathLen(dev),
                    dst_path_name_ptr, 
                    dst_path_name_len,
                    PNULL, 0,
                    path_ptr, path_len_ptr
                    );   
                
                SCI_FREE(dst_path_name_ptr);    
            }
            else
            {
                //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_GetIcoPath:dst_path_name_ptr alloc fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_1989_112_2_18_2_5_49_83,(uint8*)"");
            }
        }
    }    

    if (result)
    {
        if (PNULL != dev_ptr)
        {
            *dev_ptr = dev;
        }
    }
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_GetIcoPath result =%d, dev =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2002_112_2_18_2_5_49_84,(uint8*)"dd", result, dev);
    return result;
}

/*****************************************************************************/
//  Description : get the full path file name of the ico by filename
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIcoFullFileName(
                wchar  *name_ptr,      //[in]may PNULL,don't include suffix
                uint16 name_len,  //[in]may PNULL,name_ptr不为空时,此处必须填max len
                wchar  *full_path_ptr, //[in/out]
                uint16 *full_path_len_ptr  //[in/out]the len of full_path_ptr
                )
{
    wchar   path[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};
    uint16  path_len = MMIFILE_DIR_NAME_MAX_LEN;

    if (MMIBROWSER_GetIcoPathInfo(path, &path_len, PNULL))
    {
        //combin new file full path name
        if (MMIBRW_BookmarkCombinePath(path, path_len, 
                        name_ptr, name_len, 
                        s_dorado_ico_file_suffix, MMIAPICOM_Wstrlen(s_dorado_ico_file_suffix), 
                        full_path_ptr, full_path_len_ptr))
        {
            return TRUE;
        }
    }
    
    //SCI_TRACE_LOW:"[BRW W]MMIBROWSER_GetIcoFullFileName:FAIL"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2032_112_2_18_2_5_49_85,(uint8*)"");
    return FALSE;
}

/*****************************************************************************/
//  Description : get the full path file name of the ico by index
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIcoFullFileNameByIdex(
                uint16  index,
                wchar  *full_path_ptr, //[in/out]
                uint16 *full_path_len_ptr  //[in/out]the len of full_path_ptr
                )
{
    char   name[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};
    uint16  name_len = MMIFILE_DIR_NAME_MAX_LEN;
    wchar   wname[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};

    sprintf(name, "%d", index);
    name_len = SCI_STRLEN(name);
    MMI_STRNTOWSTR(wname, MMIFILE_DIR_NAME_MAX_LEN, (uint8*)name, MMIFILE_DIR_NAME_MAX_LEN, name_len);

    return MMIBROWSER_GetIcoFullFileName(wname,name_len,full_path_ptr,full_path_len_ptr);
}

/*****************************************************************************/
//  Description : delete the ico directory(with the files of ico)
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_DeleteIcoDir(void)
{
    wchar   path[MMIFILE_DIR_NAME_MAX_LEN+1] = {0};
    uint16  path_len = MMIFILE_DIR_NAME_MAX_LEN;
    MMIFILE_ERROR_E sfs_err = SFS_ERROR_NONE;
    BOOLEAN result = FALSE;
            
    if (MMIBROWSER_GetIcoPathInfo(path, &path_len, PNULL))
    {
        sfs_err = MMIAPIFMM_DeleteDirectory(path);
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_DeleteIcoDir:sfs_err=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2071_112_2_18_2_5_49_86,(uint8*)"d", sfs_err);
        if (SFS_ERROR_NONE == sfs_err)
        {
            result = TRUE;
        }
    }
    
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_DeleteIcoDir:result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2078_112_2_18_2_5_49_87,(uint8*)"d", result);
    
    return result;
}

/*****************************************************************************/
//  Description : delete the ico file by the index
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_DeleteIco(uint16  index)
{
    wchar   file_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16  file_name_len = MMIFILE_FULL_PATH_MAX_LEN;
    MMIFILE_ERROR_E sfs_err = SFS_NO_ERROR;
    BOOLEAN result = FALSE;

    if (MMIBROWSER_GetIcoFullFileNameByIdex(index, file_name, &file_name_len))
    {
        sfs_err = MMIAPIFMM_DeleteFile(file_name, PNULL);
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_DeleteIco sfs_err=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2097_112_2_18_2_5_49_88,(uint8*)"d", sfs_err);
        if (SFS_ERROR_NONE == sfs_err)
        {
            result = TRUE;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"[BRW]MMIBROWSER_DeleteIco:get name fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_FUNC_2105_112_2_18_2_5_49_89,(uint8*)"");
    }
    
    return result;
}

/*****************************************************************************/
//  Description : get s_is_ico_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsIcoUpdate(void)
{
    return s_is_ico_need_update;
}

/*****************************************************************************/
//  Description : set s_is_ico_need_update
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsIcoUpdate(BOOLEAN is_need_update)
{
    s_is_ico_need_update = is_need_update;
}
#endif

/*****************************************************************************/
//  Description : send the url and title info by sms
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SendUrlContentBySms(
            const wchar   *title_ptr,
            uint16  title_len,
            const wchar   *url_ptr,
            uint16  url_len
            )
{
    MMI_STRING_T    message_str = {0};
    wchar   combine_char[4] ={L':',0x0D,0x0A,0x00};//:
    uint16  combine_char_len = 0;
    BOOLEAN result = FALSE;
    
    if ((PNULL == url_ptr) || (0 == url_len))//title_ptr maybe NULL, title_len maybe 0
    {
        SCI_TRACE_LOW("[BRW] MMIBROWSER_CombineSendUrlContent:param error");
        return  FALSE;
    }
    
    if ((title_ptr != PNULL) && (title_len > 0))
    {
        combine_char_len = MMIAPICOM_Wstrlen(combine_char);
        message_str.wstr_len += title_len;
        message_str.wstr_len += combine_char_len;
    }
    else
    {
        title_len = 0;//protect to reset
    }
                
    message_str.wstr_len += url_len;

    message_str.wstr_ptr = SCI_ALLOCAZ((message_str.wstr_len + 1) * sizeof(uint16));
    if(PNULL != message_str.wstr_ptr)
    {
        if(title_len > 0)
        {
            MMIAPICOM_Wstrncpy(message_str.wstr_ptr,(const wchar*)title_ptr,title_len);
            MMIAPICOM_Wstrncat(message_str.wstr_ptr,combine_char,combine_char_len);//cat "enter"
        }
        MMIAPICOM_Wstrncat(message_str.wstr_ptr,url_ptr,url_len);
        
        message_str.wstr_len = MMIAPICOM_Wstrlen(message_str.wstr_ptr);
        MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, &message_str, PNULL, 0);
        result = TRUE;
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
    
    if(PNULL != message_str.wstr_ptr)
    {
        SCI_FREE(message_str.wstr_ptr);
    }

    return result;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

