/*****************************************************************************
** File Name:      mmi_mainmenu_item_func.c                                       *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"

#include "mmi_app_menu_trc.h"
#include "window_parse.h"
#include "guitext.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
#include "mmk_tp.h"
#include "mmiset_export.h"
#include "mmistk_export.h"
#include "mmisms_export.h"
#include "mmicl_export.h"
#include "mmidc_export.h"
#include "mmi_mainmenu_export.h"
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#ifdef GAME_SUPPORT
#include "mmigame_export.h"
#endif
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmicalendar_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmienvset_export.h"
#include "mmiconnection_export.h"
#include "mmifm_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef CMMB_SUPPORT
#include "mmimtv_export.h"
#endif
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "mmi_timer_export.h"
#ifndef MMI_PDA_SUPPORT
#include "mmicountedtime_export.h"
#include "mmistopwatch_export.h"
#endif
#ifdef GAME_SUPPORT
#include "mmigame_menutable.h"
#include "mmigame_link_export.h"
#include "mmigame_boxman_export.h"
#endif
#include "mmi_appmsg.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "mmitheme_special_func.h"
#include "mmicalc_export.h"
#include "mmiwclk_export.h"
#include "mmiacc_id.h"
#include "mmipub.h"
#include "mmisd_export.h"
#include "mmieng_export.h"
#include "mmiudisk_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif
#include "mmiphone_export.h"
#ifdef MMI_ENABLE_DCD
#include "mmidcd_export.h"
#endif
#ifdef CMCC_UI_STYLE
#include "mmics_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef PIM_SUPPORT
#include "mmipim_export.h"
#endif
#include "mmiunitconversion_export.h"

#ifdef QQ_SUPPORT
#include "mmiqq_export.h"
#endif
#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif
#ifdef SEARCH_SUPPORT
#include "mmisearch_export.h"
#endif

#include "mmiconnection_menutable.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_export.h"
#endif /* PUSH_EMAIL_SUPPORT  */

#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

#ifdef  CMCC_UI_STYLE
//ONLY FOR uplmn SETTING
#include "mmiengtd_win.h"
#endif

#include "guistatusbar.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif

#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
#if defined(DYNAMIC_MAINMENU_SUPPORT)
#include "mmi_mainmenu_synchronize.h"
#endif
#ifdef WEATHER_SUPPORT
#include "mmiweather_export.h"
#endif
#ifdef MCARE_V31_SUPPORT
#include "McfInterface.h"
#include "Mcare_Interface.h"
#endif
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
#include "mmiset_videowallpaper.h"
#endif

#ifdef MMI_TASK_MANAGER
#include "mmi_applet_manager.h"
#endif

#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#include "mmiwre.h"
#endif

#ifdef MMI_MEMO_SUPPORT
#include "mmimemo_export.h"
#endif

#ifdef MMI_KING_MOVIE_SUPPORT
#include "mmikm_export.h"
#endif
#ifdef SXH_APPLE_SUPPORT
#include "Apple_export.h"
#endif

#if (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT)
#include "Mmiwidget_export.h"
#endif

#include "mmiset_display.h"
#include "guictrl_api.h"
#include "mmiset_lock_effect_export.h"
#if (defined QBTHEME_SUPPORT)
#include "mmiqbtheme_export.h"
#include "mmiqbtheme_idleexport.h"
#endif

#ifdef IKEYBACKUP_SUPPORT
#include "mmiikeybackup_api.h"
#endif
#include "mmi_menutable.h"
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#endif

#ifdef MMI_CALL_MINIMIZE_SUPPORT
#include "mmicc_export.h"
#endif

#ifdef MMIUZONE_SUPPORT
#include "mmiuzone_export.h"
#endif

#ifdef SXH_APPLE_SUPPORT
#include "apple_export.h"
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#include "mmiset_lock_effect_export.h"
#endif

#include "mmitheme_mainmenu.h"

#ifdef SXH_APPLE_SUPPORT
#include "OEMOS.h"
#endif

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
#include "mmiphonesearch_export.h"
#endif

#ifdef MMIMAINMENU_SUPPORT_SET_BGD
#include "mmi_wallpaper_export.h"
#endif

#include "mmi_mainmenu_item_func.h"
#include "mmi_mainmenu_u_se.h"
#include "mmi_mainmenu_qbtheme.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
#ifdef BROWSER_SUPPORT
LOCAL const char* const s_mainmenu_browser_url[BROWSER_URL_TYPE_MAX] =
{
    "http://wap.dianping.com",
    "http://wap.kaixin001.com",
    "http://m.ctrip.com",
    "http://m.baidu.com/?from=1214a",//the url is from baidu
    "http://3g.sina.com.cn/3g/pro/index.php?sa=t254d1293v446&wm=9709",//sina wei bo
    "http://wap.taobao.com",
    "http://3g.youku.com",
    "http://wap.renren.com/",
    "http://wap.qidian.com/r/U7nYRr",
    "http://3g.sina.com.cn/?wm=9815",//the url is from sina
    "http://fwd.wap.soso.com:8080/forward.jsp?bid=421&cd_f=70373",//the url is from soso
    "http://wap.sohu.com/?v=3&fr=zhanxun_zm1_0923",
    "http://wap.51wre.com",
    "http://wap.sohu.com/ent/?fr=zx_ylzm_0923",
    "http://zx.m.caishijie.com/wap/jsp/index.jsp?sid=12074000000",
    "http://m.tudou.com/index.do?cp=1074",
    "http://kx.z3.cn/yx/zqqd.php?c=zx2001",
    "http://wap.alipay.com"
};
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef BROWSER_SUPPORT
PUBLIC void MMIMAINMENU_StartCommunicationBrowser (uint32 param1, uint32 param2)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;

    MMIAPIBROWSER_Entry (&entry_param);

    return;
}
#endif


#ifdef OPERA_MINI_SUPPORT
PUBLIC void MMIMAINMENU_StartOperaMinBrowser (uint32 param1, uint32 param2)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_OPERA_MINI;

    MMIAPIBROWSER_Entry (&entry_param);

    return;
}
#endif


#ifdef BROWSER_SUPPORT_NETFRONT
PUBLIC void MMIMAINMENU_StartNetFrontBrowser (uint32 param1, uint32 param2)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_NF;

    MMIAPIBROWSER_Entry (&entry_param);

    return;
}
#endif


#ifdef BROWSER_SUPPORT_DORADO
PUBLIC void MMIMAINMENU_StartDoradoBrowser (uint32 param1, uint32 param2)
{
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_DORADO;

    MMIAPIBROWSER_Entry (&entry_param);

    return;
}
#endif


PUBLIC void MMIMAINMENU_StartSTK (uint32 param1, uint32 param2)
{
    uint16 sim_sys = MN_DUAL_SYS_1;
    uint32 sim_num = 0;

    if (0 == MMIAPISTK_GetStkCardNum (PNULL, 0))
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }

    if (MMIAPISET_GetFlyMode())
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }

    sim_num = MMIAPISTK_GetStkCardNum (&sim_sys, 1);

    if (sim_num == 0)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (sim_num == 1)
    {
        MMIAPISTK_EnterToplevelMenu ( (MN_DUAL_SYS_E) sim_sys);
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    else
    {
        MMIAPISTK_SelectSIMFunc();//Sam.hua	use SelectSimFunc
    }
#endif

    return;
}


#ifdef EBOOK_SUPPORT
PUBLIC void MMIMAINMENU_StartEBOOK (uint32 param1, uint32 param2)
{
    if (MMIAPISET_IsOpenPrivacyProtect (MMISET_PROTECT_EBOOK))
    {
        MMIAPISET_ValidatePrivacyAppEntry (MMISET_PROTECT_EBOOK, MMIAPIEBOOK_ValidatePrivacyAppEntry);
    }
    else
    {
        MMIAPIEBOOK_ManagerWinMsg (TRUE);
    }

    return;
}
#endif


#ifdef CALENDAR_SUPPORT
PUBLIC void MMIMAINMENU_StartCalendar (uint32 param1, uint32 param2)
{
    MMIAPISET_ValidatePrivacyAppEntry (MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);

    return;
}
#endif

PUBLIC void MMIMAINMENU_StartFavorite (uint32 param1, uint32 param2)
{
#ifdef CMCC_UI_STYLE
    MMIAPIMENU_EnterMyFavorite();
#else
    MMIAPIFMM_OpenExplorer();
#endif

    return;
}

PUBLIC void MMIMAINMENU_EnterDialWin(uint32 param1, uint32 param2)
{
    MMIAPIIDLE_EnterDialWin();
}

PUBLIC void MMIMAINMENU_OpenListAllGroupWin(uint32 param1, uint32 param2)
{
    MMIAPIPB_OpenListAllGroupWin();
}

#ifdef CMCC_UI_STYLE
PUBLIC void MMIMAINMENU_EnterCmccService(uint32 param1, uint32 param2)
{
    MMIAPIMENU_EnterCmccService();
}

PUBLIC void MMIMAINMENU_EnterApps(uint32 param1, uint32 param2)
{
    MMIAPIMENU_EnterApps();
}

#ifdef BROWSER_SUPPORT
PUBLIC void MMIMAINMENU_EnterMonternetWin (uint32 param1, uint32 param2)
{
    MMIAPISET_EnterMonternetWin();
}
#endif

#ifdef PIC_VIEWER_SUPPORT
PUBLIC void MMIMAINMENU_StartPic (uint32 param1, uint32 param2)
{
#ifdef CMCC_UI_STYLE
    MMIAPIPICVIEW_OpenPicViewerExt (MMIPICVIEW_DOWNLOAD_PIC);
#else
    MMIAPIPICVIEW_OpenPicViewer();
#endif

    return;
}
#endif
#endif //CMCC_UI_STYLE


PUBLIC void MMIMAINMENU_StartCreateWin (uint32 *win_tab_ptr)
{
    MMK_CreateWin (win_tab_ptr, PNULL);
    return;
}


#ifndef MMI_PDA_SUPPORT
#ifdef COUNTEDTIME_SUPPORT
PUBLIC void MMIMAINMENU_StartCoutDown (uint32 param1, uint32 param2)
{
    if (MMIAPICT_IsShowIdleCoutime())
    {
        MMIAPICT_OpenCoutimeRunningWin();
    }
    else
    {
        MMIAPICT_OpenCoutimeWin();
    }

    return;
}
#endif
#endif //MMI_PDA_SUPPORT


#ifdef PUSH_EMAIL_SUPPORT
PUBLIC void MMIMAINMENU_StartPushMail (uint32 param1, uint32 param2)
{
    MMIAPIMAIL_Enter (MAIL_ENTER_MAINMENU, PNULL);

    return;
}
#endif

#ifdef WRE_SUPPORT
PUBLIC void MMIMAINMENU_StartWreApp (uint32 param1, uint32 param2)
{
    MMIWRE_StartAppEx ((WRE_APP_ID_E)param1, NULL);

    return;
}

PUBLIC void MMIMAINMENU_StartWreFixApp (uint32 param1, uint32 param2)
{
    MMIWRE_StartApp (param1);

    return;
}
#endif

#ifdef SXH_APPLE_SUPPORT
PUBLIC void MMIMAINMENU_StartSXHBook (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86) /* lint !e718 !e746 */ /*lint !e628*/
    {
        APPLE_StartSMDApp (SXH_BOOK);
    }
    else
    {
        extern void APPLE_StartBook (void);
        APPLE_StartBook();
    }

    return;
}

PUBLIC void MMIMAINMENU_StartSXHUmvchat (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_UMVCHAT);
    }
    else
    {
        extern void APPLE_StartUMvchat (void);
        APPLE_StartUMvchat();
    }

    return;
}


PUBLIC void MMIMAINMENU_StartSXHTV1 (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        SMDMenuTv1();
    }
    else
    {
        extern void APPLE_StartTv1 (void);
        APPLE_StartTv1();
    }

    return;
}


PUBLIC void MMIMAINMENU_StartSXHTV2 (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        SMDMenuTv2();
    }
    else
    {
        extern void APPLE_StartTv2 (void);
        APPLE_StartTv2();
    }

    return;
}

PUBLIC void MMIMAINMENU_StartSXHTV3 (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        SMDMenuTv3();
    }
    else
    {
        extern void APPLE_StartTv3 (void);
        APPLE_StartTv3();
    }

    return;
}


PUBLIC void MMIMAINMENU_StartSXHTV (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {

        APPLE_StartSMDApp (SXH_TV);
    }
    else
    {
        void APPLE_StartTv1 (void);
        APPLE_StartTv1();
    }

    return;
}

PUBLIC void MMIMAINMENU_StartSXHRadio (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_RADIO);
    }
    else
    {
        extern void APPLE_StartRadio (void);
        APPLE_StartRadio();
    }

    return;
}

PUBLIC void MMIMAINMENU_StartSXHGames (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_ONLINEGAME);
    }
    else
    {
        extern void APPLE_StartOnlineGame (void);
        APPLE_StartOnlineGame();
    }

    return;
}


PUBLIC void MMIMAINMENU_StartSXHUmmusic (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_MUSIC);
    }
    else
    {
        extern void APPLE_StartMusic (void);
        APPLE_StartMusic();
    }

    return;
}

PUBLIC void MMIMAINMENU_StartSXHVideodl (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_VIDELDL);
    }
    else
    {
        extern void APPLE_StartVideoDL (void);
        APPLE_StartVideoDL();
    }

    return;

}

PUBLIC void MMIMAINMENU_StartSXHUmplayer (uint32 param1, uint32 param2)
{
	extern void APPLE_StartUMPlayer(void);
    APPLE_StartUMPlayer();

    return;

}

PUBLIC void MMIMAINMENU_StartSXHWeather (uint32 param1, uint32 param2)
{
    if (OEMOS_GetLangId() == 86)
    {
        APPLE_StartSMDApp (SXH_WEATHER);
    }
    else
    {
        extern void APPLE_StartWeather (void);
        APPLE_StartWeather();
    }

    return;
}
#endif


#ifdef MMI_GPRS_SUPPORT
PUBLIC void MMIMAINMENU_StartBrowerURL (uint32 param1, uint32 param2)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    if (param1 >= BROWSER_URL_TYPE_MAX)
    {
        return;
    }

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.user_agent_ptr = PNULL;

    entry_param.url_ptr = (char *) s_mainmenu_browser_url[param1];
    MMIAPIBROWSER_Entry (&entry_param);
#endif

    return;
}
#endif


#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ANDROID_SUPPORT) && (defined MMI_ISTYLE_SUPPORT || defined MMI_GRID_IDLE_SUPPORT || defined MMI_SMART_IDLE_SUPPORT)
PUBLIC void MMIMAINMENU_StartIdleStyle (uint32 param1, uint32 param2)
{
#ifndef QBTHEME_SUPPORT
#if defined MMI_ISTYLE_SUPPORT 
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        MMISET_SetIdleStyle (MMISET_IDLE_STYLE_GRID);
    }
    else
#endif		
    {
        GUI_BOTH_RECT_T both_rect = {0};

#if defined MMI_ISTYLE_SUPPORT 
        MMISET_SetIdleStyle (MMISET_IDLE_ISTYLE);
#endif
        both_rect  = MMITHEME_GetFullScreenBothRect();
        GUIAPICTRL_SetRect (MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &both_rect.v_rect);
    }

    MMK_ReturnIdleWin();

    MMIPUB_OpenAlertWaitingWin (TXT_SWITCHING_SYSTEM);
#else
    MMIAPIMENU_SetQbThemeIdleStyle();
#endif//QBTHEME_SUPPORT

    return;
}
#endif
#endif

PUBLIC void MMIMAINMENU_StartDial (uint32 param1, uint32 param2)
{
    MMIAPIIDLE_EnterDialWin();

    return;
}

PUBLIC void MMIMAINMENU_StartMessage (uint32 param1, uint32 param2)
{
    MMIAPISMS_OpenMessageMainMenuWin();

    return;
}

#ifdef MMIEMAIL_SUPPORT
PUBLIC void MMIMAINMENU_StartIEmail (uint32 param1, uint32 param2)
{
    MMIAPIEMAIL_Entry();

    return;
}
#endif

PUBLIC void MMIMAINMENU_StartCL (uint32 param1, uint32 param2)
{
    MMIAPICL_OpenMainMenuWindow();

    return;
}

PUBLIC void MMIMAINMENU_StartPB (uint32 param1, uint32 param2)
{
    MMIAPIPB_OpenListAllGroupWin();

    return;
}

#ifdef WORLD_CLOCK_SUPPORT
PUBLIC void MMIMAINMENU_StartWordTime (uint32 param1, uint32 param2)
{
    MMIAPIWCLK_OpenMainWin();

    return;
}
#endif

#ifdef  CAMERA_SUPPORT
PUBLIC void MMIMAINMENU_StartDCPhoto (uint32 param1, uint32 param2)
{
    MMIAPIDC_OpenPhotoWin();

    return;
}

PUBLIC void MMIMAINMENU_StartDCVideo (uint32 param1, uint32 param2)
{
    MMIAPIDC_OpenVideoWin();

    return;
}

#if defined(MMIDC_F_U_CAMERA)
PUBLIC void MMIMAINMENU_StartDCUCameraPhoto (uint32 param1, uint32 param2)
{
    MMIAPIDC_OpenUCameraPhotoWin();

    return;
}
#endif
#endif

#ifdef MMI_KING_MOVIE_SUPPORT
PUBLIC void MMIMAINMENU_StartKM (uint32 param1, uint32 param2)
{
    MMIAPIKM_MainEntry();

    return;
}
#endif

#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
PUBLIC void MMIMAINMENU_StartMp3Player (uint32 param1, uint32 param2)
{
    MMIAPIMP3_OpenPlayerMenuWin();

    return;
}
#endif


#ifdef PIC_VIEWER_SUPPORT
PUBLIC void MMIMAINMENU_StartPicViewer (uint32 param1, uint32 param2)
{
    MMIAPIPICVIEW_OpenPicViewer();

    return;
}
#endif

#ifdef GAME_SUPPORT
PUBLIC void MMIMAINMENU_StartGame (uint32 param1, uint32 param2)
{
    MMIAPIGAME_OpenMainMenuWindow();

    return;
}
#endif


#ifdef JAVA_SUPPORT
PUBLIC void MMIMAINMENU_StartJava (uint32 param1, uint32 param2)
{
    MMIAPIJAVA_OpenMainMenu();

    return;
}
#endif
PUBLIC void MMIMAINMENU_StartALM (uint32 param1, uint32 param2)
{
    MMIAPIALM_OpenMainWin();

    return;
}

#ifdef ASP_SUPPORT
PUBLIC void MMIMAINMENU_StartASP (uint32 param1, uint32 param2)
{
    MMIAPIASP_EnterAsp();

    return;
}
#endif

#ifdef  DCD_SUPPORT
PUBLIC void MMIMAINMENU_StartDCD (uint32 param1, uint32 param2)
{
    MMIAPIDCD_OpenMainWin();

    return;
}
#endif



#ifdef MMI_RECORD_SUPPORT
PUBLIC void MMIMAINMENU_StartRecord (uint32 param1, uint32 param2)
{
    MMIAPIRECORD_OpenMainWin(PNULL);

    return;
}
#endif


#ifdef PIM_SUPPORT
PUBLIC void MMIMAINMENU_StartPIM (uint32 param1, uint32 param2)
{
    MMIAPIPIM_Main (param1);

    return;
}
#endif



#ifdef SEARCH_SUPPORT
PUBLIC void MMIMAINMENU_StartSearch (uint32 param1, uint32 param2)
{
    MMIAPISEARCH_Entry();

    return;
}
#endif



#ifdef CALCULATOR_SUPPORT
PUBLIC void MMIMAINMENU_StartPicalc (uint32 param1, uint32 param2)
{
    MMIAPICALC_OpenMainWin();

    return;
}
#endif


#ifdef MMI_UNITCONVERSION_SUPPORT
PUBLIC void MMIMAINMENU_StartUnitConv (uint32 param1, uint32 param2)
{
    MMIAPIUNITCONV_OpenMainWin();

    return;
}
#endif


PUBLIC void MMIMAINMENU_StartCallSettings (uint32 param1, uint32 param2)
{
    MMIAPISET_OpenCallSettingsWindow();

    return;
}

PUBLIC void MMIMAINMENU_StartPhoneSetting (uint32 param1, uint32 param2)
{
    MMIAPISET_EnterPhoneSettingWin();

    return;
}

#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
PUBLIC void MMIMAINMENU_StartSOSSetting (uint32 param1, uint32 param2)
{
    MMIAPISET_EnterSOSSettingWin();
    
    return;
}
#endif


PUBLIC void MMIMAINMENU_StartDisplaySetting (uint32 param1, uint32 param2)
{
    MMIAPISET_EnterDisplaySettingWin();

    return;
}


PUBLIC void MMIMAINMENU_StartMainmenuStyle (uint32 param1, uint32 param2)
{
    MMIAPISET_CreateMainmenuStyleWin();

    return;
}


#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
PUBLIC void MMIMAINMENU_StartLockStyle (uint32 param1, uint32 param2)
{
    MMIAPISET_CreateLockStyleWin();

    return;
}
#endif
#endif


PUBLIC void MMIMAINMENU_StartSecuritySetting (uint32 param1, uint32 param2)
{
    MMIAPISET_EnterSecuritySettingWin();

    return;
}


PUBLIC void MMIMAINMENU_StartEnvSetting (uint32 param1, uint32 param2)
{
    MMIENVSET_OpenMainMenuWindow();

    return;
}



#if defined (MMI_TVOUT_ENABLE) || defined (MMI_GPRS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
PUBLIC void MMIMAINMENU_StartConnection (uint32 param1, uint32 param2)
{
    MMIAPICONNECTION_OpenMenuWin();

    return;
}
#endif


#ifdef SXH_APPLE_SUPPORT
PUBLIC void MMIMAINMENU_StartSXHApple (uint32 param1, uint32 param2)
{
    APPLE_StartApple();

    return;
}
#endif


#ifdef MMIUZONE_SUPPORT
PUBLIC void MMIMAINMENU_StartUzone (uint32 param1, uint32 param2)
{
    MMIAPIUZONE_Entry();

    return;
}
#endif


#ifdef MMI_AUDIO_PLAYER_SUPPORT
PUBLIC void MMIMAINMENU_StartMp3 (uint32 param1, uint32 param2)
{
    MMIAPIMP3_OpenMp3Player();

    return;
}
#endif


#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
PUBLIC void MMIMAINMENU_StartIVP (uint32 param1, uint32 param2)
{
    MMIAPIVP_MainEntry();

    return;
}
#endif
#endif


#ifdef BLUETOOTH_SUPPORT
PUBLIC void MMIMAINMENU_StartBT (uint32 param1, uint32 param2)
{
    MMIAPIBT_OpenMainMenuWin();

    return;
}
#endif



#ifdef MMI_WIFI_SUPPORT
PUBLIC void MMIMAINMENU_StartWIFI (uint32 param1, uint32 param2)
{
    MMIAPIWIFI_OpenMainWin();

    return;
}
#endif



#ifdef FM_SUPPORT
PUBLIC void MMIMAINMENU_StartFM (uint32 param1, uint32 param2)
{
    MMIAPIFM_OpenMainWin();

    return;
}
#endif



#ifdef ATV_SUPPORT
PUBLIC void MMIMAINMENU_StartATV (uint32 param1, uint32 param2)
{
    MMIAPIATV_OpenATV();

    return;
}
#endif


#ifdef CMMB_SUPPORT
PUBLIC void MMIMAINMENU_StartMTV (uint32 param1, uint32 param2)
{
    MMIAPIMTV_OpenMTVPlayer();

    return;
}
#endif



#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
PUBLIC void MMIMAINMENU_StartTimer (uint32 param1, uint32 param2)
{
    MMIAPI_OpenTimerWin (param1);

    return;
}
#endif


#ifdef HOME_LOCATION_INQUIRE_SUPPORT
PUBLIC void MMIMAINMENU_StartPhoneSearch (uint32 param1, uint32 param2)
{
    MMIAPIPHONESEARCH_OpenWnd();

    return;
}
#endif


#ifdef IKEYBACKUP_SUPPORT
PUBLIC void MMIMAINMENU_StartKB (uint32 param1, uint32 param2)
{
    MMIAPIIKB_OpenMainWin();

    return;
}
#endif


#ifdef MMI_AZAN_SUPPORT
PUBLIC void MMIMAINMENU_StartMuSiLinPray (uint32 param1, uint32 param2)
{
    MMIAPIAZAN_OpenMuSiLinPrayWin();

    return;
}
#endif


#ifdef MV_SUPPORT
PUBLIC void MMIMAINMENU_StartMagicVoice (uint32 param1, uint32 param2)
{
    MMIAPISET_OpenMagicVoiceWin();

    return;
}
#endif


#ifdef MMI_TASK_MANAGER
PUBLIC void MMIMAINMENU_StartAppletManager (uint32 param1, uint32 param2)
{
    MMI_OpenAppletManager();

    return;
}
#endif


#ifdef MMI_MEMO_SUPPORT
PUBLIC void MMIMAINMENU_StartMemo (uint32 param1, uint32 param2)
{
    MMIMEMO_OpenMain();

    return;
}
#endif


#ifdef WEATHER_SUPPORT
PUBLIC void MMIMAINMENU_StartWeather (uint32 param1, uint32 param2)
{
    MMIAPIWEATHER_OpenMainWin();

    return;
}
#endif


#ifdef MCARE_V31_SUPPORT
PUBLIC void MMIMAINMENU_StartMCareV31 (uint32 param1, uint32 param2)
{
    MCareV31_Main_Entry();

    return;
}

PUBLIC void MMIMAINMENU_StartMCareV31QQBrowser (uint32 param1, uint32 param2)
{
    MCareV31_QQBrowser_Shortcut();

    return;
}
#endif


#ifdef QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartQQ (uint32 param1, uint32 param2)
{
    MMIAPIQQ_OpenMainWin();

    return;
}
#endif


#ifdef MOBILE_VIDEO_SUPPORT
PUBLIC void MMIMAINMENU_StartMV (uint32 param1, uint32 param2)
{
    MMIAPIMV_Entry();

    return;
}
#endif

#ifdef MRAPP_SUPPORT
PUBLIC void MMIMAINMENU_StartMRAPP (uint32 param1, uint32 param2)
{
    MMIMRAPP_AppEntry (param1);

    return;
}
#endif


#ifdef MET_MEX_MERIG_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMex (uint32 param1, uint32 param2)
{
    MetMexOpenWin();

    return;
}
#endif // MET_MEX_MERIG_SUPPORT


#ifdef MET_MEX_QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexQQ (uint32 param1, uint32 param2)
{
    MetMexQQOpenWin();

    return;
}
#endif // MET_MEX_QQ_SUPPORT


#ifdef MET_MEX_ZT_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexZT (uint32 param1, uint32 param2)
{
    MetMexZTOpenWin();

    return;
}
#endif // MET_MEX_ZT_SUPPORT


#ifdef MET_MEX_FRUITNINJA_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexFruitNinja (uint32 param1, uint32 param2)
{
    MetMexFruitNinjaOpenWin();

    return;
}
#endif // MET_MEX_FRUITNINJA_SUPPORT


#ifdef MET_MEX_WEIBO_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWeibo (uint32 param1, uint32 param2)
{
    MetMexWeiboOpenWin();

    return;
}
#endif // MET_MEX_WEIBO_SUPPORT


#ifdef MET_MEX_WEATHER_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWeather (uint32 param1, uint32 param2)
{
    MetMexWeatherOpenWin();

    return;
}
#endif  // MET_MEX_WEATHER_SUPPORT


#ifdef MET_MEX_WOMIVIDEO_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWomiVideo (uint32 param1, uint32 param2)
{
    MetMexWomiVideoOpenWin();

    return;
}
#endif  //MET_MEX_WOMIVIDEO_SUPPORT


#ifdef MET_MEX_TB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexTB (uint32 param1, uint32 param2)
{
    MetMexTBOpenWin();

    return;
}
#endif // MET_MEX_TB_SUPPORT


#ifdef MET_MEX_AQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexAQ (uint32 param1, uint32 param2)
{
    MetMexAQOpenWin();

    return;
}
#endif // MET_MEX_AQ_SUPPORT


#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexQQ1 (uint32 param1, uint32 param2)
{
    MetMexQQ1OpenWin();

    return;
}

PUBLIC void MMIMAINMENU_StartMetMexQQ2 (uint32 param1, uint32 param2)
{
    MetMexQQ2OpenWin();

    return;
}
#endif // MET_MEX_DOUBLE_QQ_SUPPORT


#ifdef MET_MEX_MSN_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexMSN (uint32 param1, uint32 param2)
{
    MetMexMSNOpenWin();

    return;
}
#endif


#ifdef MET_MEX_FETION_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexFetion (uint32 param1, uint32 param2)
{
    MetMexFETIONOpenWin();

    return;
}
#endif


#ifdef MET_MEX_AB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexAB (uint32 param1, uint32 param2)
{
    MetMexABOpenWin();

    return;
}
#endif // MET_MEX_AQ_SUPPORT


#ifdef MET_MEX_CQZ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexCQZ (uint32 param1, uint32 param2)
{
    MetMexCQZOpenWin();

    return;
}
#endif


#ifdef MET_MEX_JSCB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexJSCB (uint32 param1, uint32 param2)
{
    MetMexJSCBOpenWin();

    return;
}
#endif


#ifdef MET_MEX_TOMCAT_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexTomCat (uint32 param1, uint32 param2)
{
    MetMexTomCatOpenWin();

    return;
}
#endif // MET_MEX_AQ_SUPPORT


#ifdef WRE_SUPPORT
PUBLIC void MMIMAINMENU_StartWRE (uint32 param1, uint32 param2)
{
    MMIWRE_Entry();

    return;
}
#endif

#ifdef QBTHEME_SUPPORT
PUBLIC void MMIMAINMENU_OpenMainWin(uint32 param1, uint32 param2)
{
    MMIAPIQBTHEME_OpenMainWin();
}

PUBLIC void MMIMANMENU_OpenDownLoadUrl(uint32 param1, uint32 param2)
{
    MMIAPIQBTHEME_OpenDownLoadUrl();
}
#endif

#ifdef DL_SUPPORT
PUBLIC void MMIMANMENU_DownloadManager(uint32 param1, uint32 param2)
{
    MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
}
#endif

PUBLIC void MMIMANMENU_EnterPlayer(uint32 param1, uint32 param2)
{
    MMIAPIMENU_EnterPlayer();
}

PUBLIC void MMIMANMENU_EnterTools(uint32 param1, uint32 param2)
{
    MMIAPIMENU_EnterTools();
}

PUBLIC void MMIMANMENU_OpenAlarmMainWin(uint32 param1, uint32 param2)
{
    MMIAPIALM_OpenMainWin();
}

PUBLIC void MMIMANMENU_EnterSetting(uint32 param1, uint32 param2)
{
    MMIAPIMENU_EnterSetting();
}

#ifdef BT_DIALER_SUPPORT
PUBLIC void MMIMAINMENU_OpenBtPhoneWin(void)
{
    MMIAPIBT_OpenBtPhoneWin();
}
#endif
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
//extern void MMIAPIMENU_ChangeGOMenuStyle(MMI_WIN_ID_T win_id);
PUBLIC void MMIMAINMENU_GoMenuChangeStyle (uint32 param1, uint32 param2)
{
    //uint32 ctrl_id = MAINMENUWIN_MENU_CTRL_ID;
    uint32 win_id = MAIN_MAINMENU_WIN_ID;

#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
#ifdef UI_SUPPORT_U_MAINMENU
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        MMIISTYLE_U_ChangeMenuStyle(MAIN_IDLE_WIN_ID);
    }
    else
#endif
#endif
#ifdef QBTHEME_SUPPORT
    if (MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle()
#ifdef MMI_SAM_THEME
        || MMISET_IDLE_SUBSTYLE_SAM == MMIAPISET_GetIdleSubStyle()
#endif
        )
    {
        MMIAPIMENU_ChangeQbThemeGOStyle(win_id);
    }
    else
#endif
    {
        MMIAPIMENU_ChangeGOMenuStyle(win_id);
    }

    return;
}
#endif
