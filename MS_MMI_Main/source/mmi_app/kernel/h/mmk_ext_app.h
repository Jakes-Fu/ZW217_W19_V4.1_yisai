/****************************************************************************
** File Name:      mmk_timer.h                                             *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct and       *
**                  function of timer                                      *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#ifndef  _MMK_EXTERN_APP_H_    
#define _MMK_EXTERN_APP_H_   

/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

    extern MMI_APPLICATION_T    g_cc_app;
    extern MMI_APPLICATION_T    g_mmiphone_app;  
    extern MMI_APPLICATION_T    g_mmiset_app;
    extern MMI_APPLICATION_T    g_main_app;
    extern MMI_APPLICATION_T    g_mmisms_app;
    extern MMI_APPLICATION_T    g_mmipb_app;
    // add by allen  2004.08.18  begin
    extern MMI_APPLICATION_T	g_mmistk_app;
    extern MMI_APPLICATION_T	g_mmieng_app;
    //add by figo.feng 2004.09.24

#ifdef QBTHEME_SUPPORT
	   extern MMI_APPLICATION_T g_mmiqbtheme_app;
#endif
    
#ifdef MMI_ENABLE_DCD
    extern MMI_APPLICATION_T    g_mmidcd_app;
#endif
#ifdef CMCC_UI_STYLE
    extern MMI_APPLICATION_T    g_mmics_app;
#endif

#ifdef BROWSER_SUPPORT_DORADO
    extern MMI_APPLICATION_T	g_mmibrowser_app;
#endif
#ifdef PIM_SUPPORT 
extern MMI_APPLICATION_T g_mmipim_app;  
#endif

#ifdef DL_SUPPORT
    extern MMI_APPLICATION_T	g_download_app;
#endif

#ifdef BLUETOOTH_SUPPORT
	//add by wenming 2005.12.28
    extern MMI_APPLICATION_T    g_bluetooth_app;
#endif
#ifdef MMI_OCR_ENABLE
	extern MMI_APPLICATION_T    g_ocr_app;
#endif
#ifdef CMMB_SUPPORT
    extern MMI_APPLICATION_T    g_mtv_app;
#endif
#ifdef ATV_SUPPORT
    extern MMI_APPLICATION_T    g_atv_app;
#endif
#ifdef WRE_SUPPORT
    extern MMI_APPLICATION_T    g_wre_asyn_app;
    extern MMI_APPLICATION_T    g_wre_videoplayer_app;
    #ifdef QBTHEME_SUPPORT
    extern MMI_APPLICATION_T    g_wre_qbtheme_app;
    #endif
#endif
#ifdef MMI_GPRS_SUPPORT
    extern MMI_APPLICATION_T	g_pdp_app;
#endif
#ifdef MMS_SUPPORT_DORADO
    extern MMI_APPLICATION_T	g_mmimms_app;
#endif
    extern MMI_APPLICATION_T	g_caf_asyn_app;
#ifdef FDN_SUPPORT
    extern MMI_APPLICATION_T g_mmifdn_app;
#endif
#ifdef BROWSER_SUPPORT_NETFRONT
    extern MMI_APPLICATION_T g_mmiwww_app;
#endif
#ifdef MMI_WIFI_SUPPORT
    extern MMI_APPLICATION_T	g_mmiwifi_app;
#endif
#ifdef VT_SUPPORT
    extern MMI_APPLICATION_T g_mmivt_app;
#endif
#ifdef VIDEO_PLAYER_SUPPORT
    extern MMI_APPLICATION_T    g_mmivp_app;  
#endif

#ifdef PUSH_EMAIL_SUPPORT 
    extern MMI_APPLICATION_T g_mmimail_app;
#endif /* PUSH_EMAIL_SUPPORT  */

#ifdef MOBILE_VIDEO_SUPPORT
     extern MMI_APPLICATION_T    g_mv_app;
#endif 

#ifdef WEATHER_SUPPORT
     extern MMI_APPLICATION_T   g_weather_app;
#endif 
#ifdef MMIEMAIL_SUPPORT
     extern MMI_APPLICATION_T    g_mmiemail_app;  
#endif

#ifdef WRE_SUPPORT
     extern MMI_APPLICATION_T    g_wre_app;
#endif 
#ifdef MENU_CRYSTALCUBE_SUPPORT
     extern MMI_APPLICATION_T    g_mmicrystalcube_app;
#endif
#ifdef MENU_CYCLONE_SUPPORT
     extern MMI_APPLICATION_T    g_mmicyclone_app;
#endif

#ifdef DYNAMIC_MODULE_SUPPORT  
extern MMI_APPLICATION_T    g_caf_tapi_app;
#endif

#ifdef OPERA_MOBILE_SUPPORT_V11
     extern MMI_APPLICATION_T    g_opera_mobile_app;
#endif 

#if defined(OPERA_MINI_SUPPORT_VER42) || defined(OPERA_MINI_SUPPORT_VER6)
     extern MMI_APPLICATION_T    g_opera_mini_app;
#endif 

#ifdef BROWSER_SUPPORT
    extern MMI_APPLICATION_T g_mmibrowser_manager_app;
#endif

#ifdef SXH_APPLE_SUPPORT
    extern MMI_APPLICATION_T   g_apple_app;         
#endif

#ifdef SALES_SUPPORT
    extern MMI_APPLICATION_T   g_sales_app;
#endif

#ifdef MMI_REFACTOR_SAMPLE
    extern MMI_APPLICATION_T    g_appsample_app;
#endif

#ifdef MMISRVAP_SUPPORT
extern MMI_APPLICATION_T    g_mmisrv_ap;
#endif

#ifdef MMI_GPS_SUPPORT
//extern MMI_APPLICATION_T g_mmigps_app;
#endif

extern MMI_APPLICATION_T g_mmiGprsTest_app;

#ifdef SFR_SUPPORT_CTCC
extern MMI_APPLICATION_T g_sfr_ctcc_app;
#endif

#ifdef SFR_SUPPORT
extern MMI_APPLICATION_T   g_need_sfr_app;
#endif

#ifdef BAIDU_DRIVE_SUPPORT
extern MMI_APPLICATION_T g_baidu_drv;
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef DOWNLOAD_WATCHFACE_SUPPORT  //for 217 ram cutdown: unsupport ble,so move download watchface from mobile phone
extern MMI_APPLICATION_T g_watchhttp_app;
#endif
#ifdef THEME_UNZIP_SUPPORT
extern MMI_APPLICATION_T g_watchgzip_app;
#endif
extern MMI_APPLICATION_T g_watchdebug_app;
#endif
#ifdef APP_STORE_SUPPORT
#ifdef THEME_UNZIP_SUPPORT
extern MMI_APPLICATION_T g_appstore_unzip;
#endif
extern MMI_APPLICATION_T g_appstore_debug_mode;
#endif

#ifdef UAL_AUDIOPLAYER_SUPPORT
extern MMI_APPLICATION_T s_audiosource_init;
#ifndef WIN32
#ifdef XYSDK_SUPPORT
    extern MMI_APPLICATION_T   g_xysdk_app;
#endif
#endif 

#ifdef AUDIOPLAYER_IT_SUPPORT
extern MMI_APPLICATION_T audioplayer_test;
#endif
#endif

#ifdef JS_COMPENT_HTTP  //for 217 ram cutdown: unsupport http component
extern MMI_APPLICATION_T g_http_app;
#endif

#ifdef UAL_RF_AM_SUPPORT
extern MMI_APPLICATION_T g_rf_am_func;
extern MMI_APPLICATION_T g_rf_am_network_func;
#endif


#ifdef HERO_ENGINE_TTS_SUPPORT
extern MMI_APPLICATION_T  gHeroTtsApp;
#endif



#ifdef WLDT_APP_SUPPORT
   extern MMI_APPLICATION_T    g_wldt_app;
#endif

#ifdef ZDT_ZFB_SUPPORT
   extern MMI_APPLICATION_T    g_zfb_app;
#endif
#ifdef ZDT_APP_SUPPORT
   extern MMI_APPLICATION_T    g_zdt_app;
#endif

#ifdef ZDT_HTTP_APP_SUPPORT
   extern MMI_APPLICATION_T    g_zdthttp_app;
#endif
#ifdef TULING_AI_SUPPORT
   extern MMI_APPLICATION_T    mmi_ai_app;
#endif
#ifdef BAIRUI_VIDEOCHAT_SUPPORT
   extern MMI_APPLICATION_T    mmi_vct_app;
#endif

#ifdef LEBAO_MUSIC_SUPPORT
//extern MMI_APPLICATION_T	g_mmi_lebao_app;
#endif


#ifdef BEACON_APP_SUPPORT
   extern MMI_APPLICATION_T    g_beacon_app;
#endif


/**-------------------------------------------------------------------------*
 **                         Compile Flag                                   *
 **------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif
