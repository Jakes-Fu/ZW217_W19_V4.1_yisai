/*****************************************************************************
** File Name:      mainmenu_win.c                                            *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2009       Jassmine           Creat
******************************************************************************/

#define MAINMENU_WIN_C

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_menu_trc.h"
#include "window_parse.h"
#include "guitext.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
#include "mmk_tp.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiset_mainmenu_export.h"
#include "mmistk_export.h"
#include "mmisms_export.h"
#include "mmicl_export.h"
#include "mmidc_export.h"
#include "mmi_mainmenu_export.h"
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#ifdef GAME_SUPPORT
//#include "mmigame.h"
#include "mmigame_export.h"
#endif
//#include "mmiebook.h"
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#endif
#include "mmicalendar_export.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
//#include "mmiset_wintab.h"
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
#include "mmi_timer_export.h"//xiyuan edit for pclint
#ifndef MMI_PDA_SUPPORT
#include "mmicountedtime_export.h"
#include "mmistopwatch_export.h"
#endif
//#include "mmiphonesearch_export.h"
#ifdef GAME_SUPPORT
#include "mmigame_menutable.h"
//#include "mmigame_link_internal.h"
#include "mmigame_link_export.h"
#ifdef GAME_BOXMAN_SUPPORT
//#include "mmigame_boxman_internal.h"
#include "mmigame_boxman_export.h"
#endif
#endif
//#include "mmidyna_export.h"
#include "mmi_appmsg.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "mmitheme_special_func.h"
#include "mmicalc_export.h"
#include "mmiwclk_export.h"
#include "mmiacc_id.h"
#include "mmipub.h"
#include "mmisd_export.h"
//#include "mmieng.h"
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

#ifdef PUSH_EMAIL_SUPPORT   //Added by yuanl  2010.5.10
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

//#ifdef MMI_PDA_SUPPORT
#include "mmiset_display.h"
//#endif
#include "guictrl_api.h"
//#include "mmiset_lock_effect.h"  //xiyuan edit for  pclint
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

#include "mmitheme_mainmenu.h"
#ifdef OD_GAME_SUPPORT
#include "od_game_res_id_def.h"
#endif

#include "watch_common_list.h"
#ifdef ADULT_WATCH_SUPPORT
#include "mmisms_export.h"
#include "watch_sms_main.h"
#include "watch_pb_view.h"
#include "watch_stopwatch.h"
#include "watch_gallery_export.h"
#include "watch_charge_win.h"
#include "mmirecord_main.h"
#include "watch_launcher_editclockwin.h"
#include "mmipub.h"
#include "mmi_timer_export.h"
#include "watch_commonwin_export.h"
#include "watch_dial_cl_contact_internal.h"
#include "watch_sms_message.h"
#include "watch_flashlight.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#include "watch_heartrate.h"
#endif
#include "watch_alarm_main.h"
#endif

#ifdef ALIIOT_SUPPORT
#include "mmialiiot_export.h"
#endif

#ifdef APP_STORE_SUPPORT
#include "appstore_home_win_view.h"
#include "appstore_control.h"
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#include "watchface_set_flow.h"
#endif

#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
#include "jee.h"
#endif

#include "guilistbox.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MSG_MAINMENU_UPDATE (MSG_USER_START + 0x01)//异步打开应用消息
#define DYNAMIC_MAINMENU_DEFAULT_LIST_INDEX 0xffff
#define ICON_FULL_PATH_MAX_LEN (200)
#define WSTR_MAX_LEN (100)
#define MAINMENU_LIST_ITEM_CONTENT_0 (0)
#define MAINMENU_LIST_ITEM_CONTENT_1 (1)
#ifdef APP_STORE_SUPPORT
LOCAL APPSTORE_APP_LIST_T* s_p_installed_app_list = PNULL;//已安装app list
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef HERO_ENGINE_SUPPORT  
#include "heroEngineExport.h"
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#ifdef MMI_GPRS_SUPPORT
typedef enum
{
    BROWSER_URL_TYPE_DIANPING,   //大众点评网
    BROWSER_URL_TYPE_KAIXIN,     //开心网
    BROWSER_URL_TYPE_CTRIP,      //携程网  //
    BROWSER_URL_TYPE_BAIDU,      //百度         
    BROWSER_URL_TYPE_SINA_BOLG,  //新浪微博
    BROWSER_URL_TYPE_TAOBAO,     //淘宝
    BROWSER_URL_TYPE_YOUKU,      //优酷    //
    BROWSER_URL_TYPE_RENREN,     //人人网
    BROWSER_URL_TYPE_QIDIAN,     //起点中文网
    BROWSER_URL_TYPE_SINA,       //新浪网  //
    BROWSER_URL_TYPE_SOSO,       //SOSO搜搜网
    BROWSER_URL_TYPE_MOBILESOHU, //手机搜狐网
	BROWSER_URL_TYPE_COOLSTATION,//酷站导航
	BROWSER_URL_TYPE_AMUSEBOX,   //娱乐百宝箱 
    BROWSER_URL_TYPE_COLORWORLD, //彩世界
	BROWSER_URL_TYPE_TUDOU,      //土豆网
	BROWSER_URL_TYPE_PALMREAD,   //掌阅书城
	BROWSER_URL_TYPE_ZHIFUBAO,   //支付宝
    BROWSER_URL_TYPE_MAX
}MAINMENU_BROWSER_URL_TYPE_E;

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
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : main menu handle window message
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuWinMsg(
                                        MMI_WIN_ID_T         win_id, 
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM               param
                                        );

/*****************************************************************************/
// Description : icon main second menu handle window message
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainSecondMenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );

/*****************************************************************************/
// Description : 处理二级菜单静态菜单消息
// Global resource dependence : 
// Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecMenuStaticMsg(
    MMI_WIN_ID_T         win_id, 
    MMI_MESSAGE_ID_E     msg_id, 
    MMI_MENU_ID_T        menu_id
);

#ifndef MMI_MAINMENU_MINI_SUPPORT
/*****************************************************************************/
// Description : 二级菜单list控件处理函数
// Global resource dependence : 
// Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainSecondMenuListWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              );
#endif

#ifdef MMITHEME_MAINMENU_ARRANGE

/*****************************************************************************/
// Description : to handle the SMS menu option window message
// Global resource dependence : 
// Author:liming.deng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuOptWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            );

/*****************************************************************************/
// Description : to handle the SMS menu option window message
// Global resource dependence : 
// Author:liming.deng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuMoveOptWinMsg(
                                                MMI_WIN_ID_T        win_id, 
                                                MMI_MESSAGE_ID_E    msg_id, 
                                                DPARAM                param
                                                );

/*****************************************************************************/
// FUNCTION:     HandleMainMenuHelpWinMsg
// Description :   handle help win msg
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuHelpWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
#endif

#ifdef MMI_GPRS_SUPPORT                                            
/*****************************************************************************/
// FUNCTION:     HandleBrowerUrlMenu
// Description :   处理主菜单中通过browser 访问url的菜单项
// Global resource dependence : 
// Author: fen.xie
// Note:   
/*****************************************************************************/
LOCAL void HandleBrowerUrlMenu(MAINMENU_BROWSER_URL_TYPE_E browser_url_type);
#endif

#ifndef MMI_MAINMENU_MINI_SUPPORT
/*****************************************************************************/
// Description : 创建list
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_Create(MMI_MENU_GROUP_ID_T group_id);

/*****************************************************************************/
// Description : 给list控件传入资源
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_SetItem(
    MMI_CTRL_ID_T ctrl_id,         //in:control id
    uint16 item_index,     //in:item索引
    GUIMENU_GROUP_T *sc_menu_group_ptr
);
/*****************************************************************************/
// Description : 获取list控件被选中index
// Global resource dependence : 
// Author:jinwang
// Note:返回list控件被选中的list index
/*****************************************************************************/
LOCAL uint32 GetSecondMenuListIndex(MMI_MESSAGE_ID_E msg_id);

#if defined(DYNAMIC_MAINMENU_SUPPORT)
/*****************************************************************************/
// Description : 给list控件传入图片信息
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_SetDyPic(
    uint16 item_index,  //控件索引
    uint16 item_content_index,//内容索引
    MMI_CTRL_ID_T ctrl_id,//in:control id
    GUIMENU_GROUP_T *sc_menu_group_ptr,
    MMI_WIN_ID_T win_id
);
#endif
#endif

/*****************************************************************************/
// FUNCTION:     HandleMainMenuListWindow
// Description : handle mainMenu list win msg
// Global resource dependence :
// Author: fei.li
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuListWindow(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM            param
                                       );

#ifdef ADULT_WATCH_SUPPORT

LOCAL void MainMenu_List_App_Enter(void)
{
    MMI_STRING_T txt_msg = {0};
    MMI_GetLabelTextByLang(TXT_MAINMENU_APP_UNINSTALLED, &txt_msg);
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(MAIN_MAINMENU_TOAST_WIN_ID, &txt_msg, 0, PNULL);
}

LOCAL void MainMenu_List_Alarm_Enter(void)
{
    MMIAPI_OpenTimerWin (ID_CLOCK);
}

//jee umusic
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
LOCAL void launcher_umusic_app();
LOCAL void launcher_umusic_app()
{
    jee_start_app("com.unisoc.umusic",NULL);
}
LOCAL void launcher_jswatch_app();
LOCAL void launcher_jswatch_app()
{
    jee_start_app("com.unisoc.watch", NULL);
}
#endif

LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_call          = { TXT_MAINMENU_LIST_CALL,         IMAGE_MAINMENU_ICON_RES_CALL };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_sms           = { TXT_MAINMENU_LIST_SMS,          IMAGE_MAINMENU_ICON_RES_SMS };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_settings      = { TXT_MAINMENU_LIST_SETTINGS,     IMAGE_MAINMENU_ICON_RES_SETTINGS };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_camera        = { TXT_MAINMENU_LIST_CAMERA,       IMAGE_MAINMENU_ICON_RES_CAMERA };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_gallery       = { TXT_MAINMENU_LIST_GALLERY,      IMAGE_MAINMENU_ICON_RES_GALLERY };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_timer         = { TXT_MAINMENU_LIST_TIMER,        IMAGE_MAINMENU_ICON_RES_TIMER };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_alarm         = { TXT_MAINMENU_LIST_ALARM,        IMAGE_MAINMENU_ICON_RES_ALARM };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_soundrecord   = { TXT_MAINMENU_LIST_SOUNDRECORD,  IMAGE_MAINMENU_ICON_RES_SOUNDRECORD };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_flashlight    = { TXT_MAINMENU_LIST_FLASHLIGHT,   IMAGE_MAINMENU_ICON_RES_FLASHLIGHT };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_pay           = { TXT_MAINMENU_LIST_PAY,          IMAGE_MAINMENU_ICON_RES_PAY };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_heart_rate    = { TXT_MAINMENU_LIST_HEART_RATE,   IMAGE_MAINMENU_ICON_RES_HEART_RATE };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_sport         = { TXT_MAINMENU_LIST_SPORT,        IMAGE_MAINMENU_ICON_RES_SPORT };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_moving_object = { TXT_MAINMENU_LIST_MOVING_OBJECT,IMAGE_MAINMENU_ICON_RES_MOVE_OBJECT };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_find_phone    = { TXT_MAINMENU_LIST_FIND_PHONE,   IMAGE_MAINMENU_ICON_RES_FIND_PHONE };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_wallet        = { TXT_MAINMENU_LIST_WALLET,       IMAGE_MAINMENU_ICON_RES_WALLET };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_weather       = { TXT_MAINMENU_LIST_WHETHER,      IMAGE_MAINMENU_ICON_RES_WEATHER };

#ifdef APP_STORE_SUPPORT
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_appstore      = { TXT_MAINMENU_LIST_APPSTORE,     IMAGE_MAINMENU_ICON_RES_APPSTORE };
#endif

LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_steps         = { TXT_MAINMENU_LIST_STEPS,        IMAGE_MAINMENU_ICON_RES_STEPS };
#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_watch_set     = { TXT_MAINMENU_LIST_WATCH_SET,    IMAGE_MAINMENU_ICON_RES_WATCH_SET };
#endif
#endif

//jee umusic
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_umusic      = { TXT_MAINMENU_LIST_UMUSIC,     IMAGE_MAINMENU_ICON_RES_SETTINGS };
LOCAL ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST   list_item_jswatch     = { TXT_MAINMENU_LIST_JSWATCH,    IMAGE_MAINMENU_ICON_RES_SETTINGS };
#endif

LOCAL WATCHCOM_LIST_ITEM__ST s_mainmenu_list_data[] =
{
//jee umusic
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_umusic,           launcher_umusic_app },
//  { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_jswatch,          launcher_jswatch_app },//bug 1985878
#endif
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_call,           Watch_Dial_Cl_Contact_Win_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_sms,            WatchSMS_MessageWin_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_settings,       WatchSET_MainWin_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_camera,         WATCH_DC_OpenPhotoWin },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_gallery,        WatchGallery_MainWin_Enter },
#ifdef STOPWATCH_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_timer,          WatchStopWatch_MainWin_Enter },
#endif
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_alarm,          WatchAlarm_MainWin_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_soundrecord,    WATCHRECORD_OpenMainWin },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_flashlight,     AdultWatchFlashLight_MainWin_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_steps,		    AdultWatchSteps_MainWin_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_heart_rate,		AdultWatchHeartRate_MainWin_Enter },
#ifdef ALIIOT_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_pay,            WatchMainMenu_Alipay_Enter },
#else
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_pay,            MainMenu_List_App_Enter },
#endif
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_sport,			MainMenu_List_App_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_moving_object,  MainMenu_List_App_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_find_phone,		MainMenu_List_App_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_wallet,			MainMenu_List_App_Enter },
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_weather,		MainMenu_List_App_Enter },

#ifdef APP_STORE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_appstore,       APPSTORE_Enter },
#endif

#ifdef DYNAMIC_WATCHFACE_SUPPORT
#ifdef WATCHFACE_MANAGE_SUPPORT
    { WatchCOM_List_Item_Visible_Default, GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH, &list_item_watch_set,      WATCHFACE_Set_Enter },
#endif
#endif
};
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

#ifdef MATRIX_MAIN_MENU_SUPPORT
//icon main menu window 
WINDOW_TABLE(MAINMENU_ICON_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_WIN_ID),
	WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
#if defined MAINLCD_SIZE_128X64 
    
#else        
    //WIN_STATUSBAR,
    WIN_HIDE_STATUS,
#endif 

    CREATE_MAINMENU_CTRL(MMIMAINMENU_MATRIX_E, MAINMENUWIN_MENU_CTRL_ID), 
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),

    END_WIN
};
#endif

#ifndef MMI_MAINMENU_MINI_SUPPORT
//others main menu window
WINDOW_TABLE(MAINMENU_OTHERS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_OTHERS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_OTHERS, MAINMENUWIN_SECOND_MENU_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//others main menu window
WINDOW_TABLE(MAINMENU_SLIDE_OTHERS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_OTHERS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(SLIDE_MENU_OTHERS, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MATRIX_MAIN_MENU_SUPPORT
// icon main menu window 
WINDOW_TABLE(MAINMENU_TITLE_ICON_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleMainMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif
    CREATE_MAINMENU_CTRL(MMIMAINMENU_MATRIX_TITLE_E, MAINMENUWIN_MENU_CTRL_ID),
// WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef QBTHEME_SUPPORT
// 千变主题menu窗口
WINDOW_TABLE(MAINMENU_QBTHEME_PAGE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    WIN_STATUSBAR,
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    CREATE_MAINMENU_CTRL(MMIMAINMENU_QBTHEME_E, MAINMENUWIN_MENU_CTRL_ID), 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MMI_SAM_THEME
// 千变主题menu窗口
WINDOW_TABLE(MAINMENU_SAM_PAGE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    WIN_STATUSBAR,
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_0),
    CREATE_MAINMENU_CTRL(MMIMAINMENU_QBTHEME_E, MAINMENUWIN_MENU_CTRL_ID), 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

// icon main menu window with bar
// WINDOW_TABLE(MAINMENU_ISTYLE_WIN_TAB) =
// {
//     WIN_FUNC((uint32)HandleMainMenuWinMsg),
//     WIN_ID(MAIN_MAINMENU_WIN_ID),
//     WIN_STATUSBAR,
//     CREATE_MENU_CTRL(MENU_MAINMENU_ISTYLE, MAINMENUWIN_MENU_CTRL_ID),
//     WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
//     END_WIN
// };

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
// GO main menu window 
WINDOW_TABLE(MAINMENU_GO_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    //WIN_STATUSBAR,
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    CREATE_MAINMENU_CTRL(MMIMAINMENU_GO_SLIDE_E, MAINMENUWIN_MENU_CTRL_ID), 
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

#ifdef MENU_SWEETS_SUPPORT
WINDOW_TABLE(MAINMENU_SWEETS_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMainMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_WIN_ID),
    WIN_SUPPORT_ANGLE(WIN_SUPPORT_ANGLE_INIT),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    CREATE_MAINMENU_CTRL(MMIMAINMENU_SWEETS_E, MAINMENUWIN_MENU_CTRL_ID),
    END_WIN
};
#endif

//icon player menu window 
WINDOW_TABLE(MAINMENU_ICON_PLAYER_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_PLAYER_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_PLAYER, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};


//icon tools main menu window 
WINDOW_TABLE(MAINMENU_ICON_TOOLS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_TOOLS_WIN_ID),
#ifdef MAINLCD_SIZE_128X64    

#else
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
#endif    
    CREATE_MENU_CTRL(MENU_ICON_TOOLS, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TIPS,
    END_WIN
};

#ifdef CMCC_UI_STYLE
/*移动服务*/
WINDOW_TABLE(MAINMENU_ICON_SERVICE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_CMCC_SERVICE_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_SERVICE, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TIPS,
    END_WIN
};

/*移动梦网*/
WINDOW_TABLE(MAINMENU_ICON_MONERNET_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_CMCC_MONERNET_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    //CREATE_MENU_CTRL(MENU_ICON_MONERNET, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TIPS,
    END_WIN
};

/*我的收藏*/
WINDOW_TABLE(MAINMENU_ICON_MYFAVORITE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_MY_FAVORITE_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_MY_FAVORITE, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TIPS,
    END_WIN
};

/* 应用*/
WINDOW_TABLE(MAINMENU_ICON_APPS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),    
    WIN_ID(MAIN_MAINMENU_APP_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(MENU_ICON_APPS, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TIPS,
    END_WIN
};
#endif //CMCC_UI_STYLE

#ifndef MMI_MAINMENU_MINI_SUPPORT
//icon pastime main menu window 
WINDOW_TABLE(MAINMENU_ICON_PASTIME_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuListWinMsg),    
    WIN_ID(MAIN_MAINMENU_PASTIME_WIN_ID),
	WIN_STATUSBAR,
    WIN_TITLE(TXT_MAINMENU_ICON_PASTIME),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MAINMENUWIN_SECOND_LIST_CTRL_ID),
    //CREATE_MENU_CTRL(MENU_ICON_PASTIME, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif


//icon settings main menu window 
WINDOW_TABLE(MAINMENU_ICON_SETTINGS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_SETTINGS_WIN_ID),
#ifdef MAINLCD_SIZE_128X64    

#else
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
	WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_NULL),
#endif    
    CREATE_MENU_CTRL(MENU_SETTINGS_ICON, MAINMENUWIN_SECOND_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#ifndef MMI_MAINMENU_MINI_SUPPORT
//icon settings main menu window 
WINDOW_TABLE(MAINMENU_SLIDE_ICON_SETTINGS_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMainSecondMenuWinMsg),
    WIN_ID(MAIN_MAINMENU_SETTINGS_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_NULL),
    CREATE_MENU_CTRL(SLIDE_MENU_SETTINGS_ICON, MAINMENUWIN_SECOND_MENU_CTRL_ID),
 WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#ifdef MMITHEME_MAINMENU_ARRANGE
WINDOW_TABLE( MAINMENU_OPTMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMainMenuOptWinMsg),
    WIN_ID(MAIN_MAINMENU_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAINMENU_OPT,MAIN_MAINMENU_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MAINMENU_MOVEOPTMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMainMenuMoveOptWinMsg),
    WIN_ID(MAIN_MAINMENU_MOVEOPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAINMENU_MOVE_OPT,MAIN_MAINMENU_MOVEOPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

WINDOW_TABLE( MAINMENU_HELP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMainMenuHelpWinMsg ),    
    WIN_ID( MAIN_MAINMENU_HELP_WIN_ID ),
    WIN_TITLE(TXT_HELP),
    CREATE_TEXT_CTRL(MAIN_MAINMENU_HELP_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    END_WIN
}; 
#endif

#ifdef ADULT_WATCH_SUPPORT
WINDOW_TABLE(MAINMENU_LIST_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleMainMenuListWindow),
    WIN_ID(MAIN_MAINMENU_LIST_WIN_ID),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_MAINMENU_LIST_MAINMENU),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MAINMENUWIN_LIST_MENU_CTRL_ID),
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// Description : creat main menu
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIMENU_CreatMainMenu(void)
{
    MMI_HANDLE_T win_handle = 0;
    MMIMAINMENU_TYPE_E    menu_style = MMIAPISET_GetMainmenuStyle();
    
#ifdef QBTHEME_SUPPORT
    MMISET_IDLE_STYLE_E idle_style = MMISET_IDLE_STYLE_MAX; 
    
    idle_style = MMIAPISET_GetIdleStyle();
    
    if (MMISET_IDLE_QBTHEME == idle_style)
    {
        MMI_HANDLE_T qbwin_handle = NULL;
        qbwin_handle = MMK_CreateWin((uint32*)MAINMENU_QBTHEME_PAGE_WIN_TAB, PNULL);
        GUIWIN_SetStbState(qbwin_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        return qbwin_handle;
    }    
#endif
#ifdef MMI_SAM_THEME
    if (MMIAPISET_GetIdleSubStyle() == MMISET_IDLE_SUBSTYLE_SAM)
    {
        win_handle = MMK_CreateWin((uint32*)MAINMENU_SAM_PAGE_WIN_TAB, PNULL);
        GUIWIN_SetStbState(win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        return win_handle;
    }
#endif
    
    //SCI_ASSERT(menu_style <= MMIMAINMENU_TYPE_MAX);
    
    switch (menu_style)
    {
#ifdef ADULT_WATCH_SUPPORT
    case MMIMAINMENU_LIST_E:
        win_handle = MMK_CreateWin((uint32 *)MAINMENU_LIST_WIN_TAB, PNULL);
        break;
#endif
#ifdef MATRIX_MAIN_MENU_SUPPORT    
    case MMIMAINMENU_MATRIX_E:
        //set special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_MAINMENU);    
        
        //creat
        win_handle = MMK_CreateWin((uint32*)MAINMENU_ICON_WIN_TAB, PNULL);
        break;  

    case MMIMAINMENU_MATRIX_TITLE_E:
        win_handle = MMK_CreateWin((uint32*)MAINMENU_TITLE_ICON_WIN_TAB, PNULL);
        break;
#endif

#ifdef PDA_UI_SUPPORT_MANIMENU_GO
    case MMIMAINMENU_GO_SLIDE_E:    
        win_handle = MMK_CreateWin((uint32*)MAINMENU_GO_WIN_TAB, PNULL);
        break;
#endif  

#ifdef MENU_SWEETS_SUPPORT
    case MMIMAINMENU_SWEETS_E:
        win_handle = MMK_CreateWin((uint32*)MAINMENU_SWEETS_WIN_TAB, PNULL);
        break;
#endif

    default:
#ifdef MATRIX_MAIN_MENU_SUPPORT    
        MMIAPISET_SetMainmenuStyle(MMIMAINMENU_MATRIX_E);
        //set special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_MAINMENU);    
        //creat
        win_handle = MMK_CreateWin((uint32*)MAINMENU_ICON_WIN_TAB, PNULL);
#endif        
        break;
    }

	return win_handle;
}

/*****************************************************************************/
// Description : close main menu
// Global resource dependence : 
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMENU_UpdateMainMenu(void)
{
    if (MMK_IsFocusWin(MAIN_MAINMENU_WIN_ID))
    {
        MMK_CloseWin(MAIN_MAINMENU_WIN_ID);
        MMIAPIMENU_CreatMainMenu();
    }
}

#if (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT) || ( defined MMI_AUTOTEST_SUPPORT )
/*****************************************************************************/
// 	Description : handle main menu id, called by idle.
//	Global resource dependence : 
//  Author: hongbo.lan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMENU_HandleIdleMenuItem(MMI_MENU_GROUP_ID_T  group_id, MMI_MENU_ID_T menu_id)
{
    CTRLMAINMENU_ITEM_T item = {0};

    MMIMAINMENU_GetItemData(menu_id, group_id, &item);
	   
    if (
#ifdef DYNAMIC_MAINMENU_SUPPORT
    !MMIMENU_DyRunItem(group_id,menu_id) && 
#endif
    PNULL != item.call_back_start)
    {
       item.call_back_start(item.param_data1, item.param_data2);
    }
}  
#endif

#if defined MMI_ISTYLE_SUPPORT || defined MMI_GRID_IDLE_SUPPORT || defined MMI_SMART_IDLE_SUPPORT
/*****************************************************************************/
// 	Description : handle main menu win msg
//	Global resource dependence : 
//  Author: James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIMENU_HandleMainMenuWinMsg(
                                                    MMI_WIN_ID_T         win_id, 
                                                    MMI_MESSAGE_ID_E     msg_id, 
                                                    DPARAM               param
                                                    )
{
    return HandleMainMenuWinMsg( win_id, msg_id, param );
}  
#endif

#ifdef MMIMAINMENU_SUPPORT_SET_BGD
/*****************************************************************************/
//  Description : set init wallpaper parameter
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMAINMENU_SetInitWallpaper(
                                           BOOLEAN         is_save_bitmap, //only for wallpaper
                                           MMI_WIN_ID_T    win_id,
                                           MMI_CTRL_ID_T   ctrl_id
                                           )
{
    BOOLEAN                         result              = FALSE;
    BOOLEAN                         is_gif              = FALSE;
    GUIANIM_RESULT_E                anim_result         = GUIANIM_RESULT_FAIL;
    GUIANIM_CTRL_INFO_T             control_info        = {0};
    GUIANIM_DATA_INFO_T             data_info           = {0};
    GUIANIM_FILE_INFO_T             file_info           = {0};
    GUIANIM_DISPLAY_INFO_T          display_info        = {0};
    MMISET_WALLPAPER_SET_T          wallpaper_info      = {0};
    MMIMULTIM_AS_WALLPAPER_INFO     wallpaper_file_ptr  = {0};
    MMI_HANDLE_T ctrl_handle = 0;
    IGUICTRL_T *ctrl_ptr = 0;
    //CAF_GUID_T guid = 0;
    
    //set parameter
    control_info.is_wallpaper_ctrl = TRUE;
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;

	display_info.is_syn_decode = (BOOLEAN)(!is_save_bitmap);
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_save_wallpaper = is_save_bitmap;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_BLACK_COLOR;

    //get wallpaper info
    MMIAPISET_GetWallpaperSetInfo(&wallpaper_info);
	SCI_TRACE_LOW("MMIMAINMENU_SetInitWallpaper GetWallpaperSetInfo  type = %d, id = %d", 
	    wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);
	
    // 当U盘运行时
    if (MMIAPIUDISK_UdiskIsRun() && (MMISET_WALLPAPER_DOWNLOAD == wallpaper_info.wallpaper_type ))
    {
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;        
    }

    ctrl_ptr = MMK_GetCtrlPtrByWin(win_id, ctrl_id);
    /* if wallpaper ctrl did not exist, we should create it */
    if(PNULL == ctrl_ptr)
    {
        MMIAPIIDLE_CreatAnimCtrl(win_id, ctrl_id, FALSE);
    }
    
	SCI_TRACE_LOW("MMIMAINMENU_SetInitWallpaper SetWallpaperSetInfo type = %d, id = %d", 
	    wallpaper_info.wallpaper_type, wallpaper_info.wallpaper_id);

    SCI_TRACE_LOW("MMIMAINMENU_SetInitWallpaper:wallpaper_type %d!",wallpaper_info.wallpaper_type);

    switch(wallpaper_info.wallpaper_type)
    {
    case MMISET_WALLPAPER_FIXED_PIC:
        //get image data buffer
        if (wallpaper_info.wallpaper_id >= MMISET_WALLPAPER_DEFAULT_ID) /*lint !e685 !e568*/
        {
            data_info.img_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);
            anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            if ((GUIANIM_RESULT_SUCC == anim_result) ||
                (GUIANIM_RESULT_WAIT == anim_result))
            {
                result = TRUE;
            }
        }
        break;

    case MMISET_WALLPAPER_ALBUM:
    case MMISET_WALLPAPER_DOWNLOAD:
        if (MMIAPIIDLE_IsExistWallpaperFile(&is_gif))
        {
            if ((!is_gif) &&
                (MMIAPIIDLE_GetWallpaperBitmap(&data_info.bitmap_data)))
            {
                //anim will free wallpaper
                data_info.is_bitmap = TRUE;
                data_info.is_free_bitmap = TRUE;

                if (MMITHEME_IsSupportGsensor())
                {
                    //get file name info
                    MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);
                    file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                    file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                }
                anim_result = GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
            else
            {
                //free bitmap
                SCI_FREE(data_info.bitmap_data.bit_ptr);

                //get file name info
                MMIAPIFMM_GetWallPaperInfo(&wallpaper_file_ptr);

                file_info.full_path_wstr_ptr = wallpaper_file_ptr.full_path_name;
                file_info.full_path_wstr_len = wallpaper_file_ptr.full_path_len;
                anim_result = GUIANIM_SetParam(&control_info,PNULL,&file_info,&display_info);
                if ((GUIANIM_RESULT_SUCC == anim_result) ||
                    (GUIANIM_RESULT_WAIT == anim_result))
                {
                    result = TRUE;
                }
            }
        }
        else
        {
            wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
            wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
            data_info.img_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);
            anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
            if ((GUIANIM_RESULT_SUCC == anim_result) || (GUIANIM_RESULT_WAIT == anim_result))
            {
                result = TRUE;
            }
        }
        break;

    default:
        wallpaper_info.wallpaper_id = MMIAPISET_GetDefaultWallpaperIDByStyle();
        wallpaper_info.wallpaper_type = MMISET_WALLPAPER_FIXED_PIC;
        data_info.img_id = MMIAPISET_GetWallpaperByIndex(wallpaper_info.wallpaper_id);
        anim_result = GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
        if ((GUIANIM_RESULT_SUCC == anim_result) || (GUIANIM_RESULT_WAIT == anim_result))
        {
            result = TRUE;
        }
        break;
    }

    return (result);
}
#endif

#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
// Description : 创建list
// Global resource dependence :
// Author:miao.liu2
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
 LOCAL MainMenuList_Create( MMI_CTRL_ID_T ctrl_id)
{
    uint16 item_cnt = 0;  //输入总数目
    uint16 static_list_count = 0;  //静态数目
    uint32 dy_list_count = 0;  //动态数目
    uint16 idx = 0;
    uint16 curSelection   =   0;

    //读取静态菜单数据个数
    static_list_count = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    item_cnt = static_list_count;
    curSelection = GUILIST_GetCurItemIndex(ctrl_id);

#ifdef APP_STORE_SUPPORT
    APPSTORE_FullInstalledAppList(&s_p_installed_app_list);

    if(PNULL == s_p_installed_app_list)
    {
        SCI_TRACE_LOW("[MainMenuList_Create]:s_p_installed_app_list is error");
    }
    else
    {
        dy_list_count = s_p_installed_app_list->app_num;
    }

    if (dy_list_count > 0)//有动态菜单数据
    {
        item_cnt += (uint16)dy_list_count;
    }

#endif

    if (0 == item_cnt)
    {
        SCI_TRACE_LOW("[MainMenuList_Create]:error");
        return FALSE;
    }

    SCI_TRACE_LOW("[MAINMENU_WIN]: item_cnt=%d",item_cnt);
    GUILIST_RemoveAllItems(ctrl_id);
    GUILIST_SetMaxItem( ctrl_id, item_cnt, TRUE);

    for(idx = 0; idx < item_cnt; idx++)
    {
        GUILIST_ITEM_T item_info = {0};
        if(static_list_count > idx)
        {
            item_info.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH;
        }
        else
        {
            SCI_TRACE_LOW("[MAINMENU_WIN]: Dyna enter!");
            item_info.item_style = GUIITEM_STYLE_1ICON_1STR_DYNA;
        }

        GUILIST_AppendItem(ctrl_id, &item_info);
    }

    //不画分割线
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    //不画高亮条
    GUILIST_SetListState( ctrl_id, GUILIST_STATE_NEED_HIGHTBAR, FALSE);
    //set selected item
    GUILIST_SetSelectedItem(ctrl_id, curSelection, TRUE);
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id,curSelection);
    return TRUE;
}

/*****************************************************************************/
// Description : 给list控件传入资源
// Global resource dependence :
// Author:miao.liu2
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN MainMenuListFillData(MMI_CTRL_ID_T ctrl_id, WATCHCOM_LIST_ITEM__ST* p_list_content,GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content)
{
    uint16       static_item_cnt = 0;
    uint32       user_data   = 0;
    uint16       dy_item_index = 0;
    MMI_STRING_T         str1 = {0};
    GUILIST_ITEM_T       item_t    = {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
#ifdef APP_STORE_SUPPORT
    APPSTORE_APP_NODE_T* p_app_node = PNULL;
#endif
    if(PNULL == p_list_content)
    {
        SCI_TRACE_LOW("[MainMenuListFillData] p_list_content is PNULL");
        return FALSE;
    }
    if(PNULL == p_need_item_content)
    {
        SCI_TRACE_LOW("[MainMenuListFillData] p_need_item_content is PNULL");
        return FALSE;
    }
    SCI_TRACE_LOW("[MainMenuListFillData] item_index :%d",p_need_item_content->item_index);

    static_item_cnt = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    if(  p_need_item_content->item_index < static_item_cnt)
    {
        item_t.item_style = GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH;
        item_t.item_data_ptr = &item_data;

        //图片
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = ((ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST*)(p_list_content[p_need_item_content->item_index].data_ptr))->image_id;
        //文本
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id = ((ADULTWATCHCOM_LIST_ITEM_STYLE_1STR_1ICON_ST*)(p_list_content[p_need_item_content->item_index].data_ptr))->text_id;

        GUILIST_SetItemData( ctrl_id, &item_data, p_need_item_content->item_index );
        user_data = (uint32)(p_list_content[p_need_item_content->item_index].item_callback);
        GUILIST_SetItemUserData(ctrl_id, p_need_item_content->item_index, &user_data);
    }
#ifdef APP_STORE_SUPPORT
    else
    {
        dy_item_index = p_need_item_content->item_index - static_item_cnt;

        APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);
        if(PNULL == p_app_node)
        {
            SCI_TRACE_LOW("[MainMenuListFillData] p_app_node is PNULL");
            return FALSE;
        }

        if((PNULL != p_app_node->p_app_icon_full_name) && (PNULL != p_app_node->p_app_name))
        {
            wchar icon[ICON_FULL_PATH_MAX_LEN] = {0};
            wchar string[WSTR_MAX_LEN] = {0};
            GUIANIM_FILE_INFO_T  icon_info = {0};
            GUI_UTF8ToWstr(icon,SCI_STRLEN(p_app_node->p_app_icon_full_name),p_app_node->p_app_icon_full_name,SCI_STRLEN(p_app_node->p_app_icon_full_name));
            icon_info.full_path_wstr_ptr = icon;
            icon_info.full_path_wstr_len = SCI_STRLEN(p_app_node->p_app_icon_full_name);
            GUI_UTF8ToWstr(string,SCI_STRLEN(p_app_node->p_app_name),p_app_node->p_app_name,SCI_STRLEN(p_app_node->p_app_name));
            str1.wstr_ptr = string;
            str1.wstr_len = SCI_STRLEN(p_app_node->p_app_name);

            WatchCOM_ListItem_Draw_1Icon_1Str_Dyna(MAINMENUWIN_LIST_MENU_CTRL_ID,icon_info,str1,p_need_item_content->item_index);
        }
    }
#endif
    return TRUE;
}

#ifdef APP_STORE_SUPPORT
/*****************************************************************************/
// Description : 给list控件传入图片信息
// Global resource dependence :
// Author:miao.liu2
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN MainMenuListFillContent(MMI_CTRL_ID_T ctrl_id, GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content)
{
    uint16 static_item_cnt = 0;
    uint16 dy_item_index = 0;
    APPSTORE_APP_NODE_T* p_app_node = PNULL;

    if(PNULL == p_need_item_content)
    {
        SCI_TRACE_LOW("[MainMenuListFillContent] p_need_item_content is PNULL");
        return FALSE;
    }

    static_item_cnt = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    if (p_need_item_content->item_index < static_item_cnt)
    {
        SCI_TRACE_LOW("[MainMenuListFillContent] error");
        return FALSE;
    }

    //计算出动态菜单的index
    dy_item_index = p_need_item_content->item_index - static_item_cnt;
    SCI_TRACE_LOW("[MainMenuListFillContent] dy_item_index:%d",dy_item_index);

    APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);

    if (0 == p_need_item_content->item_content_index)   //添加缩略图   //coverity bug1941697
    {
        wchar icon[ICON_FULL_PATH_MAX_LEN] = {0};
        GUILIST_ITEM_DATA_T  item_data = {0};
        GUIANIM_FILE_INFO_T  icon_info = {0};
        GUI_UTF8ToWstr(icon,SCI_STRLEN(p_app_node->p_app_icon_full_name),p_app_node->p_app_icon_full_name,SCI_STRLEN(p_app_node->p_app_icon_full_name));
        icon_info.full_path_wstr_ptr = icon;
        icon_info.full_path_wstr_len = SCI_STRLEN(p_app_node->p_app_icon_full_name);
        //图片数据
        item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_PATH;
        item_data.item_content[0].item_data.anim_data_ptr = &icon_info;
        GUILIST_SetItemContent(ctrl_id, &item_data, p_need_item_content->item_index, p_need_item_content->item_content_index);
    }

    return TRUE;
}

/*****************************************************************************/
// Description : 处理按下APP后的操作
// Global resource dependence :
// Author: mingwei.jia
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN MainMenuItemContentClick( uint16 item_index,uint16 item_content_index )
{
    BOOLEAN ret = FALSE;

    SCI_TRACE_LOW("[MainMenuItemContentClick] item_content_index:%d",item_content_index);
    switch(item_content_index)
    {
        case MAINMENU_LIST_ITEM_CONTENT_0:
        case MAINMENU_LIST_ITEM_CONTENT_1:
        {
            uint16 static_item_cnt = 0;
            static_item_cnt = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
            if(item_index >= static_item_cnt)
            {
                APPSTORE_APP_NODE_T* p_app_node = PNULL;
                uint16 dy_item_index = 0;
                dy_item_index = item_index - static_item_cnt;

                APPSTORE_GetAppNodeFromAppList(dy_item_index, *s_p_installed_app_list, &p_app_node);
                if(PNULL != p_app_node)
                {
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
                   // jsart_runapp(p_app_node->p_app_id,NULL);
                   jee_start_app(p_app_node->p_app_id,NULL);

#endif
                    ret = TRUE;
                }
                SCI_TRACE_LOW("[MainMenuItemContentClick] get server app node is null");
            }
            break;
        }
        default:
        {
            SCI_TRACE_LOW("[MainMenuItemContentClick]:parameter error！item_content_index:%d",item_content_index);
            break;
        }
    }
    return ret;
}

#endif 

LOCAL void MainMenu_List_APP_OK( void )
{
    WatchCOM_List_Item_CallBack( MAINMENUWIN_LIST_MENU_CTRL_ID );
}

LOCAL void MainMenu_List_APP_CANCEL( void )
{
    MMK_CloseWin( MAIN_MAINMENU_LIST_WIN_ID );
}

LOCAL void MainMenu_List_OPEN_WINDOW( void)
{
    uint32 listNum = 0;

    listNum = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
    WatchCOM_TextList_Create(s_mainmenu_list_data, listNum, MAINMENUWIN_LIST_MENU_CTRL_ID);
    MMK_SetAtvCtrl(MAIN_MAINMENU_LIST_WIN_ID, MAINMENUWIN_LIST_MENU_CTRL_ID);
}
#ifdef APP_STORE_SUPPORT
LOCAL void AppSoreMainMenuCallback(MSG_APPSTORE_E msg_id)
{
    SCI_TRACE_LOW("[APPSTORE mainmenu callback]: msg_id:%d", msg_id);

    switch(msg_id)
    {
        case MSG_APPSTORE_UPDATE_APP:
        {
            if( MMK_IsFocusWin(MAIN_MAINMENU_LIST_WIN_ID))
            {
                SCI_TRACE_LOW("[APPSTORE mainmenu callback]: send full pain to main menu");
                MMK_SendMsg(MAIN_MAINMENU_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }
        default:
        {
            break;
        }
    }
}
#endif

/*****************************************************************************/
//  Description : to handle mainmenu list message
//  Global resource dependence :
//  Author:fei.li
//  Note:
/*****************************************************************************/
uint32 s_main_menu_appstore_hanlde = 0;
LOCAL MMI_RESULT_E  HandleMainMenuListWindow(
                                         MMI_WIN_ID_T    win_id,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E recode =  MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = MAINMENUWIN_LIST_MENU_CTRL_ID;
    GUILIST_NEED_ITEM_CONTENT_T* p_need_item_content = PNULL;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
        {
            GUILIST_SetSyncDecodeAnim(ctrl_id,TRUE);//bug1941497
            GUILIST_SetDefaultIconEx(ctrl_id,NULL,NULL);
            MMK_SetAtvCtrl(win_id,MAINMENUWIN_LIST_MENU_CTRL_ID);
#ifdef APP_STORE_SUPPORT
            if(0 == s_main_menu_appstore_hanlde)
            {
                //register appstore control first
                s_main_menu_appstore_hanlde = APPSTORE_AddClient(AppSoreMainMenuCallback);
                if(0 == s_main_menu_appstore_hanlde)
                {
                    SCI_TRACE_LOW("[MAINMENU_WIN]:add appstore client error");
                }
            }
#endif
            break;
        }
        case MSG_FULL_PAINT:
        {
            //uint16 static_list_count = sizeof(s_mainmenu_list_data)/sizeof(WATCHCOM_LIST_ITEM__ST);
            MainMenuList_Create(ctrl_id);
            break;
        }
        case MSG_CTL_OK:
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT
        case MSG_CTL_PENOK:
#endif
        {
            MainMenu_List_APP_OK();
            break;
        }
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
#ifdef APP_STORE_SUPPORT
            APPSTORE_FreeAppList(s_p_installed_app_list);
            s_p_installed_app_list = PNULL;
#endif
            MainMenu_List_APP_CANCEL();
            break;
        }
        case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            p_need_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            MainMenuListFillData(MAINMENUWIN_LIST_MENU_CTRL_ID,s_mainmenu_list_data, p_need_item_content);
            break;
        }

        case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        {
#ifdef APP_STORE_SUPPORT
            p_need_item_content = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            MainMenuListFillContent(MAINMENUWIN_LIST_MENU_CTRL_ID, p_need_item_content);
#endif
            break;
        }

        case MSG_CTL_LIST_ITEM_CONTENT_CLICK:
        {
            GUILIST_ITEM_CONTENT_CLICK_T *p_item_content = (GUILIST_ITEM_CONTENT_CLICK_T*)param;
            if(PNULL == p_item_content)
            {
                recode = MMI_RESULT_FALSE;
                SCI_TRACE_LOW("[MAINMENU_WIN]:param error");
            }
#ifdef APP_STORE_SUPPORT
            else
            {
                MainMenuItemContentClick( p_item_content->item_index,p_item_content->item_content_index);
            }
#endif
            break;
        }
        case MSG_KEYDOWN_USER_FUNC:
        {
            AdultWatchSteps_MainWin_Enter();
            break;
        }
        case MSG_CLOSE_WINDOW:
        {
            if(0 != s_main_menu_appstore_hanlde)
            {
                APPSTORE_DeleteClient(s_main_menu_appstore_hanlde);
                s_main_menu_appstore_hanlde = 0;
            }
        }
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}
#endif

/*****************************************************************************/
// Description : main menu handle window message
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuWinMsg(
                                        MMI_WIN_ID_T         win_id, 
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM               param
                                        )
{
    BOOLEAN                             is_menu_gray = TRUE;
    GUI_BG_T                            softkey_bg = {0};
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MAINMENUWIN_MENU_CTRL_ID;
    MMI_MENU_ID_T                       menu_id = 0;
    MMI_MENU_GROUP_ID_T                 group_id = 0;
    uint16                              stk_sim_id = 0;
    uint32                              stk_sim_num = 0;
    MMI_STRING_T                        stk_str = {0};
    BOOLEAN                             is_exist_stk_name = FALSE;
#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
#ifdef MMI_VIDEOWALLPAPER_SUPPORT  
    MMISET_IDLE_STYLE_E                 idle_style  = MMISET_IDLE_STYLE_COMMON;
    MMIMAINMENU_TYPE_E                  mainmenu_style  =   MMIMAINMENU_MATRIX_E;
#endif
#endif
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMIMAINMENU_SUPPORT_SET_BGD
        MMK_SetWinDisplayStyleState(win_id, WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG, TRUE);
        MMIMAINMENU_SetInitWallpaper(TRUE, win_id, MMIIDLE_WALLPAPER_ANIM_CTRL_ID);
#endif
        MMITHEME_InitMenuItemCache();
#ifdef LOW_MEMORY_SUPPORT
#endif        
        //set status bar
        MAIN_SetAllStbAppCallbackValid(TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
        GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_EVENT, TRUE);
		GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
#ifndef MMI_PDA_SUPPORT	
        GUIWIN_SetStbBgIsDesplay(win_id,FALSE);
#endif
        softkey_bg.bg_type = GUI_BG_IMG;
        softkey_bg.img_id  = IMAGE_THEME_BG;
        GUIWIN_SetSoftkeyCommonBg(win_id,&softkey_bg);
        break;

	case MSG_NOTIFY_STATUSBAR_PAINT:
		MAIN_SetStbTimeVisible(TRUE);
		break;

    case MSG_FULL_PAINT:
        if (MMIAPISET_GetMultiSysSettingNum(PNULL, 0) > 1)
        {
            is_menu_gray = FALSE;
        }
        else
        {
            is_menu_gray = TRUE;
        }
#ifndef MMI_RES_SLIM_SUPPORT        
        MMIMAINMENU_SetItemDisable(ID_CONNECTION_GPRS, is_menu_gray);
#endif
        stk_sim_num = MMIAPISTK_GetStkCardNum(&stk_sim_id, 1);
        if (stk_sim_num > 0)
        {
            is_menu_gray = FALSE;
        }
        else
        {
            is_menu_gray = TRUE;
        }
        if (1 == stk_sim_num)
        {
            is_exist_stk_name = MMIAPISTK_GetStkName((MN_DUAL_SYS_E)stk_sim_id, &stk_str);
        }

        if (!is_exist_stk_name)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &stk_str);
        }

        { //STK Menu
#ifndef MMI_RES_SLIM_SUPPORT
            MMIMAINMENU_SetItemText(ID_COMMUNICATION_STK, &stk_str);
#endif

#ifndef MMI_RES_SLIM_SUPPORT          
            MMIMAINMENU_SetItemDisable(ID_COMMUNICATION_STK, is_menu_gray);
#endif

            GUIMAINMENU_GetId(ctrl_id, &group_id, &menu_id);
            MMIMAINMENU_SetItemText(ID_COMMUNICATION_STK, &stk_str);
            
            MMIMAINMENU_SetItemDisable(ID_COMMUNICATION_STK, is_menu_gray);
        }
        break;

    case MSG_GET_FOCUS:
        MMITHEME_InitMenuItemCache();
        MAIN_SetAllStbAppCallbackValid(TRUE);
        break;

#if (defined MMI_GRID_IDLE_SUPPORT) || (defined QBTHEME_SUPPORT)
    case MSG_NOTIFY_GRID_SHORTCUT2IDLE:
    case MSG_NOTIFY_QBTHEME_SHORTCUT2IDLE:
        {
            MMIMAINMENU_ITEM_INFO_T item_info = {0};
            GUI_POINT_T  current_point = {0};
            LCD_ANGLE_E  current_angle = LCD_ANGLE_0;
            LCD_ANGLE_E  idlewin_angle = LCD_ANGLE_0;
            int16 temp = 0;
            uint16  lcd_width = 0; 
            uint16  lcd_height= 0;                    

            current_point = *(GUI_POINT_T*)param;
            MMK_GetWinAngle(MAIN_IDLE_WIN_ID,&idlewin_angle);
            MMK_GetWinAngle(MAIN_MAINMENU_WIN_ID,&current_angle);
            GUILCD_GetWidthHeightByAngle(MAIN_LCD_ID,idlewin_angle,&lcd_width,&lcd_height);

            switch ((uint16)current_angle - (uint16)idlewin_angle)/* lint !e656 */
            {
            case LCD_ANGLE_90:
                temp  = current_point.x;
                current_point.x = (int16)(lcd_width - current_point.y);
                current_point.y =  (int16)temp;
                break;
                
            case LCD_ANGLE_180:
                current_point.x = (int16)(lcd_width - current_point.x);
                current_point.y = (int16)(lcd_height - current_point.y);
                break;
                
            case LCD_ANGLE_270:
                temp  = current_point.y;
                current_point.y = (int16)(lcd_height - current_point.x);
                current_point.x = (int16)(temp);
                break;
                
            default:
                break;
            }

            GUIMAINMENU_GetId(ctrl_id, &item_info.group_id, &item_info.menu_id);
            
#if defined MMI_GRID_IDLE_SUPPORT
            if(MMISET_IDLE_STYLE_GRID == MMIAPISET_GetIdleStyle())
            {
                MMIAPIGRID_AddMenu2Idle(item_info, current_point);
            }
#endif

#if defined QBTHEME_SUPPORT
            if(MMISET_IDLE_QBTHEME == MMIAPISET_GetIdleStyle())
            {
                MMIAPIQBIDLE_AddMenu2Idle(item_info, current_point);
            }
#endif
#ifdef TOUCH_PANEL_SUPPORT     
            //清除tp状态，使ilde可以处理tp up
            MMK_SetTPDownWin((MMI_HANDLE_T)0);
#endif
            //MMK_ReturnIdleWin();
            MMK_CloseWin(win_id);
            result = MMI_RESULT_TRUE;
        }
        break;
#endif        
      

    case MSG_NOTIFY_MENU_SET_STATUS:
		//fixed by hongjun.jia for cr152406
		if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
		{
			MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
		}

		else
		{
			if (*(GUIMENU_MOVE_STATE_T*)param == GUIMENU_STATE_MOVE_SET)
			{
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
				MMIAPISETVWP_Pause();
#endif
				GUIMAINMENU_InitMoveState(MAINMENUWIN_MENU_CTRL_ID);
			}
			else if (*(GUIMENU_MOVE_STATE_T*)param == GUIMENU_STATE_MOVE_CANCEL)
			{
				GUIMAINMENU_CancelMoveIcon(MAINMENUWIN_MENU_CTRL_ID);
			}
			else
			{
				result = MMI_RESULT_FALSE;
			}
		}
        break;

    case MSG_LOSE_FOCUS:
        MMITHEME_DestroyMenuItemCache();
        MAIN_SetAllStbAppCallbackValid(FALSE);
        break;
        
    case MSG_CTL_OK:
#ifdef MMITHEME_MAINMENU_ARRANGE
        if (GUIMAINMENU_IsMoveState(MAINMENUWIN_MENU_CTRL_ID))
        {
            //SCI_TRACE_LOW:"Create MAINMENU_MOVEOPTMENU_WIN_TAB!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINMENU_WIN_1130_112_2_18_2_32_50_0,(uint8*)"");
            MMK_CreateWin((uint32*)MAINMENU_MOVEOPTMENU_WIN_TAB, PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"Create MAINMENU_OPTMENU_WIN_TAB!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINMENU_WIN_1135_112_2_18_2_32_50_1,(uint8*)"");
            MMK_CreateWin((uint32*)MAINMENU_OPTMENU_WIN_TAB, PNULL);
        }
        break;
#endif
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_GRID_PEN_MENU_ITEM:
        if (MSG_GRID_PEN_MENU_ITEM == msg_id)
        {
            group_id = ((MMIMAINMENU_ITEM_INFO_T*)param)->group_id;
            menu_id  = ((MMIMAINMENU_ITEM_INFO_T*)param)->menu_id;
        }
        else
        {
            GUIMAINMENU_GetId(ctrl_id, &group_id, &menu_id);
        }

#if 0//defined(DYNAMIC_MAINMENU_SUPPORT)
        if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == group_id)
        {
			if(MMIAPIUDISK_UdiskIsRun())
			{
				//u disk runing
				MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
			}
			else 
			{
				GUIMAINMENU_DY_ITEM_T item_info = {0};
				BOOLEAN is_info_get = FALSE;
				is_info_get = MMIMENU_DySlideGetRunInfo(menu_id, &item_info);
				
				if (is_info_get
					&& PNULL != item_info.ori_data_ptr
					&& MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
				{
					(item_info.ori_data_ptr->dynamic_menu_info.link_function_ptr)(
						item_info.ori_data_ptr->dynamic_menu_info.param1ptr, 
						item_info.ori_data_ptr->dynamic_menu_info.param2ptr
						);
					
					MMIMENU_DyFreeParmInfo(&item_info);
				}
                else
			    {
			        //t card not exist
				    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NO_EXIST);
			    }
			}
            break;
        }
#endif

#if defined(MMI_PDA_SUPPORT) && defined(QBTHEME_SUPPORT)
        if (ID_MAINMENU_RETURNIDLE == menu_id)
        {
            MMK_ReturnIdleWin();
        }
#endif
    break;
/*
        switch (menu_id)
        {
        case ID_COMMUNICATION_DIAL:
            MMIAPIIDLE_EnterDialWin();
            break;
            
        case ID_COMMUNICATION_MESSAGE:
            MMIAPISMS_OpenMessageMainMenuWin();
            break;

        case ID_COMMUNICATION_BROWSER:
#ifdef BROWSER_SUPPORT
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};
            
            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            
            MMIAPIBROWSER_Entry(&entry_param);
        }
#endif
            break;

#ifdef OPERA_MINI_SUPPORT
        case ID_MAINMENU_OPERAMINI:
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};
            
            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.is_browser_type_customized = TRUE;
            entry_param.browser_type = MMIBROWSER_TYPE_OPERA_MINI;
            
            MMIAPIBROWSER_Entry(&entry_param);
        }
        break;
#endif    
#ifdef BROWSER_SUPPORT_NETFRONT
        case ID_MAINMENU_NF:
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};
            
            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.is_browser_type_customized = TRUE;
            entry_param.browser_type = MMIBROWSER_TYPE_NF;
            
            MMIAPIBROWSER_Entry(&entry_param);
        }
        break;
#endif    
#ifdef BROWSER_SUPPORT_DORADO    
        case ID_MAINMENU_DORADO:
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};
            
            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.is_browser_type_customized = TRUE;
            entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
            
            MMIAPIBROWSER_Entry(&entry_param);
        }
        break;
#endif    

#ifdef MMIEMAIL_SUPPORT
        case ID_COMMUNICATION_EMAIL:
           MMIAPIEMAIL_Entry();
            break;
#endif

        case ID_COMMUNICATION_CL:
            
            MMIAPICL_OpenMainMenuWindow();
            
            break;
            
        case ID_COMMUNICATION_PB:
            MMIAPIPB_OpenListAllGroupWin();
            break;
            
        case ID_COMMUNICATION_STK:            
            {
                uint16 sim_sys = MN_DUAL_SYS_1;
                uint32 sim_num = 0;
				
				if (0 == MMIAPISTK_GetStkCardNum( PNULL, 0 ))
				{
					break;
				}
				
                if (MMIAPISET_GetFlyMode())
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                    break;
                }

                sim_num = MMIAPISTK_GetStkCardNum(&sim_sys, 1);
				
                if (sim_num == 0)
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (sim_num == 1)
                {
                    MMIAPISTK_EnterToplevelMenu((MN_DUAL_SYS_E)sim_sys);
                }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                else
                {
                    MMIAPISTK_SelectSIMFunc();//Sam.hua	use SelectSimFunc 
                }
#endif
            }
            break;
            
#ifdef  CAMERA_SUPPORT
        case ID_ENTERTAIMENT_CAMERA:
            MMIAPIDC_OpenPhotoWin(); 
            break;
            
        case ID_ENTERTAIMENT_DV:
            MMIAPIDC_OpenVideoWin(); 
            break;
            
#if defined(MMIDC_F_U_CAMERA)
        case ID_ENTERTAIMENT_UCAMERA:
            MMIAPIDC_OpenUCameraPhotoWin();
            break;
#endif            
#endif

#ifdef WRE_SUPPORT
#ifdef WRE_FACEBOOK_SUPPORT
       case ID_MAINMENU_FACEBOOK:
	   MMIWRE_StartAppEx(WRE_FIX_ID_FACEBOOK, NULL);
       break;	   	
#endif
#ifdef WRE_TWITTER_SUPPORT
       case ID_MAINMENU_TWITTER:
	  MMIWRE_StartAppEx(WRE_FIX_ID_TWITTER, NULL);
       break;	   	
#endif
#ifdef WRE_WEIBO_SUPPORT
       case ID_MAINMENU_SINA_BOLG:
	  MMIWRE_StartAppEx(WRE_FIX_ID_SINA, NULL);	
       break;	   	
#endif
#endif

			
#ifdef MMI_KING_MOVIE_SUPPORT	// 电影王
		case ID_MENU_KING_MOVIE:
			MMIAPIKM_MainEntry();
			break;
#endif

#ifdef PIC_VIEWER_SUPPORT            
        case ID_PIC_VIEWER:
            MMIAPIPICVIEW_OpenPicViewer();
            break;
#endif
            
#ifdef GAME_SUPPORT
        case ID_ENTERTAIMENT_GAME:
            MMIAPIGAME_OpenMainMenuWindow();
            break;
#endif
            
        case ID_EBOOK:
#ifdef EBOOK_SUPPORT
            if ( MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_EBOOK) )
            {
                MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_EBOOK,MMIAPIEBOOK_ValidatePrivacyAppEntry);
            }
            else
            {
                MMIAPIEBOOK_ManagerWinMsg(TRUE);
            }
#endif 
            break;
            
#ifdef JAVA_SUPPORT            
        case ID_ENTERTAIMENT_GOLDHOUSE:
            MMIAPIJAVA_OpenMainMenu();
            break;
#endif       
        case ID_TOOLS_ALARM:
            MMIAPIALM_OpenMainWin();
            break;
   
#ifdef ASP_SUPPORT
      case ID_TOOLS_ASP:
     MMIAPIASP_EnterAsp();
        break;
#endif
            
#ifdef  DCD_SUPPORT
        case ID_TOOLS_DCD:
            MMIAPIDCD_OpenMainWin();
            break;
#endif //DCD_SUPPORT
#ifdef CALENDAR_SUPPORT
        case ID_TOOLS_CALENDAR:
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
            break;
#endif            
#ifdef MMI_RECORD_SUPPORT            
        case ID_TOOLS_RECORD:
            MMIAPIRECORD_OpenMainWin(PNULL);
            break;
#endif            

#ifdef PIM_SUPPORT
        case ID_MAINMENU_PIM:
            MMIAPIPIM_Main(0);
            break;
#endif
            
#ifdef SEARCH_SUPPORT
        case ID_MAINMENU_SEARCH:
            MMIAPISEARCH_Entry();
            break;
#endif

#ifdef CALCULATOR_SUPPORT            
        case ID_TOOLS_CALCULATOR:
            MMIAPICALC_OpenMainWin();
            break;
#endif

#ifdef MMI_UNITCONVERSION_SUPPORT            
        case ID_TOOLS_UNIT_CONVERSION:
            MMIAPIUNITCONV_OpenMainWin();
            break;
#endif
            
        case ID_ENTERTAIMENT_FAVORITE:
#ifdef CMCC_UI_STYLE    
            MMK_CreateWin((uint32 *)MAINMENU_ICON_MYFAVORITE_WIN_TAB,PNULL);
#else
            MMIAPIFMM_OpenExplorer();
#endif
            break;
         
#ifndef MMI_MAINMENU_MINI_SUPPORT
#if(defined(WORLD_CLOCK_SUPPORT) || defined(COUNTEDTIME_SUPPORT) || defined(HOME_LOCATION_INQUIRE_SUPPORT) || defined(DYNAMIC_MODULE_SUPPORT))
        case ID_TOOLS_OTHERS:
            MMK_CreateWin((uint32 *)MAINMENU_OTHERS_WIN_TAB,PNULL);
            break;
#endif

        case ID_MAINMENU_SLIDE_TOOLS_OTHERS:
            MMK_CreateWin((uint32 *)MAINMENU_SLIDE_OTHERS_WIN_TAB,PNULL);
            break;

        case ID_MAINMENU_SLIDE_SETTINGS:
            MMK_CreateWin((uint32 *)MAINMENU_SLIDE_ICON_SETTINGS_WIN_TAB,PNULL);
            break;
#endif            
        case ID_SETTINGS_CALL:
            MMIAPISET_OpenCallSettingsWindow();
            break;
            
        case ID_SETTINGS_PHONE:
            MMIAPISET_EnterPhoneSettingWin();
            break;
            
        case ID_SETTINGS_DISPLAY:
            MMIAPISET_EnterDisplaySettingWin();
            break;
//#ifdef MMI_MULTI_MAINMENU_SUPPORT
        case ID_MAINMENU_STYLE_SET:
            MMIAPISET_CreateMainmenuStyleWin();
            break;
//#endif            
#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
        case ID_GOMENU_CHANGE_STYLE:
#ifdef UI_SUPPORT_U_MAINMENU
            if (MMIMAINMENU_ISTYLE_E == GUIMAINMENU_GetStyle(ctrl_id))
            {
                MMIMENU_U_ChangeMenuStyle(win_id);
            }
            else
#endif
            {
                MMIAPIMENU_ChangeGOMenuStyle(win_id);
            }
            break;
#endif

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
        case ID_MAINMENU_LOCK_SET:
            MMIAPISET_CreateLockStyleWin();
            break;
#endif // #ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#endif
        case ID_SETTINGS_SECURITY:
            MMIAPISET_EnterSecuritySettingWin();
            break;
            
        case ID_SETTINGS_ENVIONMENT:
            MMIENVSET_OpenMainMenuWindow();
            break;

#if defined (MMI_TVOUT_ENABLE) || defined (MMI_GPRS_SUPPORT) || defined (BLUETOOTH_SUPPORT)            
        case ID_SETTINGS_LINK:
            MMIAPICONNECTION_OpenMenuWin();
            break;
#endif
                     
        case ID_MAINMENU_PLAYER:
            MMK_CreateWin((uint32 *)MAINMENU_ICON_PLAYER_WIN_TAB,PNULL);
            break;
			           
#ifdef CMMB_SUPPORT
        case ID_ENTERTAIMENT_MTV:
            MMIAPIMTV_OpenMTVPlayer();
            break;
#endif
#ifdef ATV_SUPPORT
        case ID_ENTERTAIMENT_ATV:
            MMIAPIATV_OpenATV();
            break;
#endif

#ifdef FM_SUPPORT
        case ID_ENTERTAIMENT_FM:
            MMIAPIFM_OpenMainWin();
            break;

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
        case ID_ENTERTAIMENT_FM_TIMER_RECORD:
            MMIAPIFM_CreateFMTimerRecordInfoWin();    
            break; 
#endif  
#endif

            
#ifdef CMCC_UI_STYLE
        case ID_MAINMENU_CMCC_MONERNET:     
            MMIAPISET_EnterMonternetWin();
            break;
            
        case ID_MAINMENU_CMCC_SERVICE:
            MMK_CreateWin((uint32 *)MAINMENU_ICON_SERVICE_WIN_TAB,PNULL);     
            
            break;
        case ID_CUSTMER_SERVICE:
            MMIAPICS_OpenMainWin();
            break;
        case ID_MAINMENU_APPS:   
            MMK_CreateWin((uint32 *)MAINMENU_ICON_APPS_WIN_TAB,PNULL);
            break;     
        case ID_NET_FETION:
            MMIAPISET_EnterFetionWin();
            break;
            
        case ID_NET_MUSIC:
            MMIAPISET_EnterMusicWin();
            break;

#ifdef PIC_VIEWER_SUPPORT
        case ID_ENTERTAIMENT_PIC:
#ifdef CMCC_UI_STYLE
            MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_DOWNLOAD_PIC);
#else
            MMIAPIPICVIEW_OpenPicViewer();
#endif
            break;
#endif

#ifdef BLUETOOTH_SUPPORT
        case ID_APPS_BT:
            MMIAPIBT_OpenMainMenuWin(); 
            break;
#endif
            
#ifdef JAVA_SUPPORT
        case ID_JAVA_APP:
            MMIAPIJAVA_OpenMainMenu();
            break;
#endif

#ifdef CMMB_SUPPORT
        case ID_APPS_MBBMS:
            MMIAPIMTV_OpenMTVPlayer();
            break;
#endif
        case ID_CHINAMOBILE_SECURITIES:
            MMIAPISET_EnterStockWin();
            break;
        case ID_CHINAMOBILE_MAP:
            MMIAPISET_EnterMapWin();
            break;
            
        case ID_SETTINGS_VERSION:
            MMIAPIENG_CreateShowVersionWin();
            break;
#endif //CMCC_UI_STYLE
        case ID_MAINMENU_TOOLS:
            MMK_CreateWin((uint32 *)MAINMENU_ICON_TOOLS_WIN_TAB,PNULL);
            break;

#ifndef MMI_MAINMENU_MINI_SUPPORT           
        case ID_MAINMENU_PASTIME:
            MMK_CreateWin((uint32 *)MAINMENU_ICON_PASTIME_WIN_TAB,PNULL);
            break;
#endif
            
        case ID_MAINMENU_SETTINGS:
            MMK_CreateWin((uint32 *)MAINMENU_ICON_SETTINGS_WIN_TAB,PNULL);
            break;
            
#ifdef QQ_SUPPORT
        case ID_SPREADTRUM_QQ_SUPPORT:
            MMIAPIQQ_OpenMainWin();
            break;
#endif
#ifdef MMI_AUDIO_PLAYER_SUPPORT 
        case ID_ENTERTAIMENT_MP3:
            MMIAPIMP3_OpenMp3Player();
            break;
#endif
#ifdef MMIUZONE_SUPPORT
        case ID_ENTERTAIMENT_UZONE:
            MMIAPIUZONE_Entry();
            break;
#endif
            
        case ID_ENTERTAIMENT_VIDEO:
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
            MMIAPIVP_MainEntry();
#endif
#endif
            break;      

        case ID_ENTERTAIMENT_MOBILE_VIDEO:
#ifdef MOBILE_VIDEO_SUPPORT
            MMIAPIMV_Entry();
#endif
            break;


#ifdef BLUETOOTH_SUPPORT
        case ID_MAINMENU_BT:
            MMIAPIBT_OpenMainMenuWin();   
            break;
#endif
            
#ifdef MMI_PDA_SUPPORT
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
#ifdef COUNTEDTIME_SUPPORT
        case ID_COUNTDOWN:
            MMIAPI_OpenTimerWin(ID_COUNTDOWN);   
            break;
#endif            
#endif
#else   //MMI_PDA_SUPPORT
#ifdef COUNTEDTIME_SUPPORT
        case ID_COUNTDOWN:
            if (MMIAPICT_IsShowIdleCoutime())
            {
                MMIAPICT_OpenCoutimeRunningWin();
            }
            else
            {
                MMIAPICT_OpenCoutimeWin();
            }
            break;
#endif
#ifdef STOPWATCH_SUPPORT           
        case ID_STOPWATCH:  //yaye.jiang 2011.7.21
            MMIAPIST_OpenStopWatchWin();
            break;  
#endif
#endif//MMI_PDA_SUPPORT        

#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
        case ID_CLOCK:
            MMIAPI_OpenTimerWin(ID_CLOCK);   
        break;
#endif

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
        case ID_PHONESEARCH:
            MMIAPIPHONESEARCH_OpenWnd();
            break;
#endif

#ifdef WIFI_SUPPORT
        case ID_CONNECTION_WIFI:
            MMIAPIWIFI_OpenMainWin();
            break;
#endif

#ifdef MMI_GPRS_SUPPORT
        case ID_CONNECTION_NET_USERID:  
            MMIAPICONNECTION_OpenSettingListWin();
            break;

        case ID_CONNECTION_GPRS:
            MMIAPICONNECTION_OpenGprsWin();
            break;
#endif

#ifdef WORLD_CLOCK_SUPPORT
        case ID_WORLDTIME:
            MMIAPIWCLK_OpenMainWin();
            break;
#endif
            
#ifdef PUSH_EMAIL_SUPPORT   //Added by yuanl  2010.5.7
        case ID_MAINMENU_PUSHMAIL:
            MMIAPIMAIL_Enter(MAIL_ENTER_MAINMENU, PNULL);
            break;
#endif // PUSH_EMAIL_SUPPORT 
#ifdef MMI_GPRS_SUPPORT

        case ID_MAINMENU_DIANPING:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_DIANPING);
            break;
            
        case ID_MAINMENU_KAIXIN:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_KAIXIN);
            break;
            
        case ID_MAINMENU_CTRIP:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_CTRIP);
            break;
            
        case ID_MAINMENU_BAIDU:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_BAIDU);
            break;
            
        case ID_MAINMENU_TAOBAO:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_TAOBAO);
            break;
            
        case ID_MAINMENU_YOUKU:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_YOUKU);
            break;
            
        case ID_MAINMENU_RENREN:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_RENREN);
            break;
            
        case ID_MAINMENU_QIDIAN:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_QIDIAN);
            break;
#ifdef SXH_APPLE_SUPPORT
    	case ID_SXH_APPLE:
		{
			APPLE_StartApple();
		}
		break;
        case ID_SXH_BOOK:
		    {
				if(OEMOS_GetLangId() == 86)//
				{
					APPLE_StartSMDApp(SXH_BOOK);
				}
				else
				{
					extern void APPLE_StartBook(void);
					APPLE_StartBook();
				}
			}
		break;
		case ID_SXH_UMVCHAT:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_UMVCHAT);	
				}
				else
				{
					extern void APPLE_StartUMvchat(void);
					APPLE_StartUMvchat();
				}
			}
		break;
		case ID_SXH_TV1:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv1();
				}
				else
				{
					extern void APPLE_StartTv1(void);
					APPLE_StartTv1();
				}
			}
		break;
		case ID_SXH_TV2:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv2();	
				}
				else
				{
					extern void APPLE_StartTv2(void);
					APPLE_StartTv2();
				}
			}
		break;
		case ID_SXH_TV3:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv3();	
				}
				else
				{
					extern void APPLE_StartTv3(void);
					APPLE_StartTv3();
				}
			}
		break;
		case ID_SXH_TV:		// switch (menu_id)下
			{	
				if(OEMOS_GetLangId() == 86)
				{	

					APPLE_StartSMDApp(SXH_TV);	
				}
				else
				{
					void APPLE_StartTv1(void);	
					APPLE_StartTv1();
				}
				
			}
		break;
		case ID_SXH_RADIO:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_RADIO);	
				}
				else
				{
					extern void APPLE_StartRadio(void);
					APPLE_StartRadio();
				}
			}
	break;
		case ID_SXH_Games:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_ONLINEGAME);	
				}
				else
				{
					extern void APPLE_StartOnlineGame(void);
					APPLE_StartOnlineGame();
				}
			}
			break;
		case ID_SXH_UMMUSIC:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_MUSIC);	
				}
				else
				{
					extern void APPLE_StartMusic(void);
					APPLE_StartMusic();
				}
			}
			break;
		case ID_SXH_VIDEODL:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_VIDELDL);	
				}
				else
				{
					extern void APPLE_StartVideoDL(void);
					APPLE_StartVideoDL();
				}
			}
			break;
		case ID_SXH_UMPLAYER:
			{
				extern void APPLE_StartUMPlayer(void);
				APPLE_StartUMPlayer();
			}
			break;	
		case ID_SXH_WEATHER:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_WEATHER);	
				}
				else
				{
					extern void APPLE_StartWeather(void);
					APPLE_StartWeather();
				}
			}
			break;
			
#ifdef SXH_APPLE_3GVEDIO_SUPPORT	
	case ID_SXH_UMVCHATTHIN:
		{
			APPLE_StartUMvchatthin();
		}
	break;
#endif // SXH_APPLE_3GVEDIO_SUPPORT	
#ifdef SXH_APPLE_UMVCHAT_DEMO
       case ID_SXH_UMVCHATDEMO:
	   	{
			APPLE_StartUMvchatDemo();
       	}
	   break;
#endif
#endif
			
        case ID_MAINMENU_SINA:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_SINA);
            break;
            
        case ID_MAINMENU_SOSO:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_SOSO);
            break;

//new xiyuan
		case ID_MAINMENU_PALMREAD:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_PALMREAD);
            break;
			
		case ID_MAINMENU_AMUSEBOX:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_AMUSEBOX);
            break;
			
		case ID_MAINMENU_MOBILESOHU:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_MOBILESOHU);
            break;
			
		case ID_MAINMENU_COOLSTATION:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_COOLSTATION);
            break;
			
		case ID_MAINMENU_COLORWORLD:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_COLORWORLD);
            break;
			
		case ID_MAINMENU_TUDOU:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_TUDOU);
            break;
			
		case ID_MAINMENU_ZHIFUBAO:
            HandleBrowerUrlMenu(BROWSER_URL_TYPE_ZHIFUBAO);
            break;
//new end
#endif

#ifdef QQ_SUPPORT
        case ID_MAINMENU_QQ:
            MMIAPIQQ_OpenMainWin();
            break;
#endif

//============== MET MEX Start ===================
#ifdef MET_MEX_MERIG_SUPPORT
        case ID_MAINMENU_MET_MEX_ITEM_ID:
            {
                MetMexOpenWin();
            }
            break;
#endif // MET_MEX_MERIG_SUPPORT


#ifdef MET_MEX_QQ_SUPPORT
        case ID_MAINMENU_MET_MEX_QQ_ITEM_ID:
            {
                MetMexQQOpenWin();
            }
            break;
#endif // MET_MEX_QQ_SUPPORT
#ifdef MET_MEX_ZT_SUPPORT
		case ID_MAINMENU_MET_MEX_ZT_ITEM_ID:
			{
				MetMexZTOpenWin();
			}
			break;
#endif // MET_MEX_ZT_SUPPORT
#ifdef MET_MEX_FRUITNINJA_SUPPORT
		case ID_MAINMENU_MET_MEX_FRUITNINJA_ITEM_ID:
			{
				MetMexFruitNinjaOpenWin();
			}
			break;
#endif // MET_MEX_FRUITNINJA_SUPPORT
#ifdef MET_MEX_WEIBO_SUPPORT
        case ID_MAINMENU_MET_MEX_WEIBO_ITEM_ID:
            {
                MetMexWeiboOpenWin();
            }
			break;
#endif // MET_MEX_WEIBO_SUPPORT
#ifdef MET_MEX_WEATHER_SUPPORT
        case ID_MAINMENU_MET_MEX_WEATHER_ITEM_ID:
            {
                MetMexWeatherOpenWin();
            }
			break;
#endif  // MET_MEX_WEATHER_SUPPORT
#ifdef MET_MEX_WOMIVIDEO_SUPPORT
        case ID_MAINMENU_MET_MEX_WOMIVIDEO_ITEM_ID:
            {
                MetMexWomiVideoOpenWin();
            }
			break;
#endif  //MET_MEX_WOMIVIDEO_SUPPORT
#ifdef MET_MEX_TB_SUPPORT
		case ID_MAINMENU_MET_MEX_TB_ITEM_ID:
			{
                MetMexTBOpenWin();
			}
			break;
#endif // MET_MEX_TB_SUPPORT


#ifdef MET_MEX_AQ_SUPPORT
		case ID_MAINMENU_MET_MEX_AQ_ITEM_ID:
			{
                MetMexAQOpenWin();
			}
			break;
#endif // MET_MEX_AQ_SUPPORT
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
		case ID_MAINMENU_MET_MEX_QQ1_ITEM_ID:
			{
				MetMexQQ1OpenWin();
			}
			break;
		case ID_MAINMENU_MET_MEX_QQ2_ITEM_ID:
			{
				MetMexQQ2OpenWin();
			}
			break;
#endif // MET_MEX_DOUBLE_QQ_SUPPORT

#ifdef MET_MEX_MSN_SUPPORT
		case ID_MAINMENU_MET_MEX_MSN_ITEM_ID:
			{
				MetMexMSNOpenWin();
			}
			break;
#endif 
#ifdef MET_MEX_FETION_SUPPORT
		case ID_MAINMENU_MET_MEX_FETION_ITEM_ID:
			{
				MetMexFETIONOpenWin();
			}
			break;
#endif
#ifdef MET_MEX_AB_SUPPORT
		case ID_MAINMENU_MET_MEX_AB_ITEM_ID:
			{
				MetMexABOpenWin();
			}
			break;
#endif // MET_MEX_AQ_SUPPORT
#ifdef MET_MEX_CQZ_SUPPORT
    case ID_MAINMENU_MET_MEX_CQZ_ITEM_ID:
		{
			MetMexCQZOpenWin();
		}
	break;
#endif

#ifdef MET_MEX_JSCB_SUPPORT
    case ID_MAINMENU_MET_MEX_JSCB_ITEM_ID:
		{
			MetMexJSCBOpenWin();
		}
	break;
#endif
#ifdef MET_MEX_TOMCAT_SUPPORT
		case ID_MAINMENU_MET_MEX_TOMCAT_ITEM_ID:
			{
				MetMexTomCatOpenWin();
			}
			break;
#endif // MET_MEX_AQ_SUPPORT
//============== MET MEX end ===================


#ifdef WEATHER_SUPPORT
        case ID_MAINMENU_WEATHER:
            MMIAPIWEATHER_OpenMainWin();
            break;
#endif

#ifdef IKEYBACKUP_SUPPORT
        case ID_MAINMENU_IKEYBACKUP:
            MMIAPIIKB_OpenMainWin();
            break;
#endif


            
#ifdef MV_SUPPORT
        case ID_MAINMENU_MAGIC_VOICE:
            MMIAPISET_OpenMagicVoiceWin();
            break;
#endif
#ifdef MMI_AZAN_SUPPORT
        case ID_AZAN:
            MMIAPIAZAN_OpenMuSiLinPrayWin();
            break;
#endif  
#ifdef WRE_SUPPORT
		case ID_TOOLS_WRE:
            {
		  MMIWRE_Entry();
            }	
            break;
#ifdef WRE_CONTRA_SUPPORT  // 魂斗罗
	case ID_WRE_CONTRA:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_CONTRA, NULL);
            }	
            break;
#endif
#ifdef WRE_SUPERMARIO_SUPPORT  // 超级玛丽
	case ID_WRE_SUPERMARIO:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_SUPERMARIO, NULL);
            }	
            break;
#endif
#ifdef WRE_TSLAND_SUPPORT  // 冒险岛
	case ID_WRE_TSLAND:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_TSLAND, NULL);
            }	
            break;
#endif
#endif
	                    
#ifdef MCARE_V31_SUPPORT
        case ID_TMC_APP_SUPPORT:
            MCareV31_Main_Entry();
            break;
        case ID_BRW_APP_SUPPORT:
            MCareV31_QQBrowser_Shortcut();
            break;
#endif

#ifdef MMI_TASK_MANAGER
        case ID_APP_MANAGER:
            MMI_OpenAppletManager();
            break;
#endif

#ifdef MMI_MEMO_SUPPORT
        case ID_APP_MEMO:
            MMIMEMO_OpenMain();
            break;
#endif

#ifdef MRAPP_SUPPORT
#ifdef MRAPP_COOKIE_OPEN
    case ID_MRAPP_COOKIE:
        MMIMRAPP_AppEntry(MRAPP_COOKIE);
        break;
#endif

#ifdef MRAPP_MRPSTORE_OPEN
    case ID_MRAPP_MRPSTORE:
        MMIMRAPP_AppEntry(MRAPP_MRPSTORE);
        break;
#endif

#ifdef MRAPP_SKYBUDDY_OPEN
		case ID_MRAPP_SKYBUDDY:
            MMIMRAPP_AppEntry(MRAPP_SKYBUDDY);
            break;
#endif

#ifdef MRAPP_SDKQQ_OPEN
    case ID_MRAPP_QQ:
        MMIMRAPP_AppEntry(MRAPP_SDKQQ);
        break;
#endif

#ifdef MRAPP_MSN_OPEN
    case ID_MRAPP_MSN:
        MMIMRAPP_AppEntry(MRAPP_MSN);
        break;
#endif

#ifdef MRAPP_EBOOK_OPEN
	case ID_MRAPP_BOOKS:
		MMIMRAPP_AppEntry(MRAPP_EBOOK);
		break;		
#endif

#ifdef MRAPP_NETGAME_OPEN
   case ID_MRAPP_NETGAME:
        MMIMRAPP_AppEntry(MRAPP_NETGAME);
        break;
#endif

#ifdef MRAPP_SIMUGAME_OPEN
   case ID_MRAPP_NESGAME:
        MMIMRAPP_AppEntry(MRAPP_NESGAME);
        break;
#endif

#ifdef MRAPP_TOOL_OPEN
    case ID_MRAPP_TOOL:
        MMIMRAPP_AppEntry(MRAPP_TOOL);
        break;
#endif

#ifdef MRAPP_ENDICT_OPEN
    case ID_MRAPP_ENDICT:
        MMIMRAPP_AppEntry(MRAPP_ENDICT);
        break;
#endif

#ifdef MRAPP_CHDICT_OPEN
    case ID_MRAPP_CHDICT:
        MMIMRAPP_AppEntry(MRAPP_CHDICT);
        break;
#endif

#ifdef MRAPP_TIANQI_OPEN
    case ID_MRAPP_TIANQI:
        MMIMRAPP_AppEntry(MRAPP_TIANQI);
        break;
#endif

#ifdef MRAPP_FETION_OPEN
    case ID_MRAPP_FETION:
        MMIMRAPP_AppEntry(MRAPP_FETION);
        break;
#endif

#ifdef MRAPP_XLWB_OPEN
    case ID_MRAPP_XLWB:
        MMIMRAPP_AppEntry(MRAPP_XLWB);
        break;
#endif

#ifdef MRAPP_KAIXIN_OPEN
   case ID_MRAPP_KAIXIN:
        MMIMRAPP_AppEntry(MRAPP_KAIXIN);
        break;
#endif

#ifdef MRAPP_FARM_OPEN
   case ID_MRAPP_FARM:
        MMIMRAPP_AppEntry(MRAPP_FARM);
        break;
#endif

#ifdef MRAPP_CLOCK_OPEN
    case ID_MRAPP_CLOCK:
        MMIMRAPP_AppEntry(MRAPP_CLOCK);
        break;
#endif

#ifdef MRAPP_FACEBOOK_OPEN
    case ID_MRAPP_FACEBOOK:
        MMIMRAPP_AppEntry(MRAPP_FACEBOOK);
        break;
#endif

#ifdef MRAPP_YAHOO_OPEN
    case ID_MRAPP_YAHOO:
        MMIMRAPP_AppEntry(MRAPP_YAHOO);
        break;
#endif

#ifdef MRAPP_SKYPE_OPEN
    case ID_MRAPP_SKYPE:
        MMIMRAPP_AppEntry(MRAPP_SKYPE);
        break;
#endif

#ifdef MRAPP_TWITTER_OPEN
   case ID_MRAPP_TWITTER:
        MMIMRAPP_AppEntry(MRAPP_TWITTER);
        break;
#endif
#endif
                  
#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ANDROID_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
        case ID_MAINMENU_IDLE_STYLE:
            
#ifndef QBTHEME_SUPPORT
            if ( MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle() )
            {
                MMISET_SetIdleStyle( MMISET_IDLE_STYLE_GRID );
            }
            else
            {
                GUI_BOTH_RECT_T both_rect = {0};
                
                MMISET_SetIdleStyle( MMISET_IDLE_ISTYLE );
                
                both_rect  = MMITHEME_GetFullScreenBothRect();
                GUIAPICTRL_SetRect(MMIIDLE_WALLPAPER_ANIM_CTRL_ID, &both_rect.v_rect);                 
            }
            MMK_ReturnIdleWin();
            
            MMIPUB_OpenAlertWaitingWin(TXT_SWITCHING_SYSTEM);
#else
            MMIAPIMENU_SetQbThemeIdleStyle();
#endif//QBTHEME_SUPPORT

            break;
#endif

#ifdef QBTHEME_SUPPORT
        case ID_MAINMENU_RETURNIDLE:
            MMK_ReturnIdleWin();
        break;

        case ID_MAINMENU_QBTHEMEAPP:
            MMIAPIQBTHEME_OpenMainWin();
        break;

        case ID_MAINMENU_QBTHEMEMORE:
#ifdef BROWSER_SUPPORT   
            MMIAPIQBTHEME_OpenDownLoadUrl();
#endif
        break;        
#endif
#endif
        default:
            break;
        }

        break;
        */
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef CMMB_SUPPORT
        case MSG_KEYDOWN_TV:
      MMIAPIMTV_OpenMTVPlayer();
   break;
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

#if 0  //#ifndef PDA_VIDEOPLAYER_WALLPAPER_SUPPORT_LOW_MEMORY
#ifdef MMI_VIDEOWALLPAPER_SUPPORT 
    idle_style = MMIAPISET_GetIdleStyle();
    mainmenu_style = MMIAPISET_GetMainmenuStyle();
    if(MMISET_IDLE_ISTYLE != idle_style)
    {
        switch (mainmenu_style)
        {
        case MMIMAINMENU_ISTYLE_E:
            break;
        default:
            result = MMIAPISETVWP_HandleVideoWallPaper(win_id, msg_id, param);
            break;
        }
    }
#endif
#endif

    return (result);
}

/*****************************************************************************/
// Description : icon main second menu handle window message
// Global resource dependence : 
// Author:jassmine.meng
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainSecondMenuWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_RESULT_E                        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id = MAINMENUWIN_SECOND_MENU_CTRL_ID;
    MMI_MENU_ID_T                     menu_id = 0;
    MMI_MENU_GROUP_ID_T                 group_id = 0;
    uint16 stk_sim_id = 0;
    uint32 stk_sim_num = 0;
    MMI_STRING_T stk_str = {0};
    BOOLEAN is_exist_stk_name = FALSE;
    BOOLEAN is_menu_visible = TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        stk_sim_num = MMIAPISTK_GetStkCardNum(&stk_sim_id, 1);

        if (stk_sim_num > 0)
        {
            is_menu_visible = TRUE;
        }
        else
        {
            is_menu_visible = FALSE;
        }

        if (1 == stk_sim_num)
        {
            is_exist_stk_name = MMIAPISTK_GetStkName((MN_DUAL_SYS_E)stk_sim_id, &stk_str);
        }

        if (!is_exist_stk_name)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_MAIN_STK, &stk_str);
        }

#ifdef CMCC_UI_STYLE
        GUIMENU_SetStaticItem(MAINMENUWIN_SECOND_MENU_CTRL_ID,
                                                MENU_ICON_SERVICE,
                                                ID_COMMUNICATION_STK,
                                                &stk_str,
                                                IMAGE_NULL
                                                );
#else
        GUIMENU_SetStaticItem(MAINMENUWIN_SECOND_MENU_CTRL_ID,
                                                MENU_ICON_TOOLS,
                                                ID_COMMUNICATION_STK,
                                                &stk_str,
                                                IMAGE_NULL
                                                );
#endif

#ifdef CMCC_UI_STYLE
        GUIMENU_SetItemVisible(MAINMENUWIN_SECOND_MENU_CTRL_ID, MENU_ICON_SERVICE,ID_COMMUNICATION_STK,is_menu_visible);
#else
        GUIMENU_SetItemVisible(MAINMENUWIN_SECOND_MENU_CTRL_ID,MENU_ICON_TOOLS,ID_COMMUNICATION_STK,is_menu_visible);
#endif
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        HandleSecMenuStaticMsg(win_id, msg_id, menu_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
/*****************************************************************************/
// Description : 处理二级菜单静态菜单消息
// Global resource dependence : 
// Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSecMenuStaticMsg(
    MMI_WIN_ID_T         win_id, 
    MMI_MESSAGE_ID_E     msg_id, 
    MMI_MENU_ID_T        menu_id
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(menu_id)
    {
#ifdef  CAMERA_SUPPORT
    case ID_ENTERTAIMENT_CAMERA:
        MMIAPIDC_OpenPhotoWin();
        break;
    case ID_ENTERTAIMENT_DV:
        MMIAPIDC_OpenVideoWin(); 
        break;
#if defined(MMIDC_F_U_CAMERA)
        case ID_ENTERTAIMENT_UCAMERA:
            MMIAPIDC_OpenUCameraPhotoWin();
            break;
#endif            
#endif

#ifdef MMI_KING_MOVIE_SUPPORT	// 电影王
	case ID_MENU_KING_MOVIE:
		MMIAPIKM_MainEntry();
		break;
#endif

    case ID_ENTERTAIMENT_FAVORITE:
#ifdef CMCC_UI_STYLE    
		MMK_CreateWin((uint32 *)MAINMENU_ICON_MYFAVORITE_WIN_TAB,PNULL);
#else
		MMIAPIFMM_OpenExplorer();
#endif
		break;

#ifdef BROWSER_SUPPORT
    case ID_COMMUNICATION_BROWSER:
        {
            MMIBROWSER_ENTRY_PARAM entry_param = {0};

            entry_param.type = MMIBROWSER_ACCESS_MAINMENU;
            entry_param.dual_sys = MN_DUAL_SYS_MAX;
            entry_param.url_ptr = PNULL;
            entry_param.user_agent_ptr = PNULL;
            
            MMIAPIBROWSER_Entry(&entry_param);
        }
        break;
#endif

#ifdef MMI_AUDIO_PLAYER_SUPPORT 
    case ID_ENTERTAIMENT_MP3:
        MMIAPIMP3_OpenMp3Player();
        break;
#endif
#ifdef MMIUZONE_SUPPORT
    case ID_ENTERTAIMENT_UZONE:
        MMIAPIUZONE_Entry();
        break;
#endif

    case ID_ENTERTAIMENT_VIDEO:
#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
        MMIAPIVP_MainEntry();
#endif
#endif
        break;

    case ID_ENTERTAIMENT_MOBILE_VIDEO:
#ifdef MOBILE_VIDEO_SUPPORT
        MMIAPIMV_Entry();
#endif
        break;

#ifdef SEARCH_SUPPORT
    case ID_MAINMENU_SEARCH:
         MMIAPISEARCH_Entry();
         break;
#endif

#ifdef FM_SUPPORT
    case ID_ENTERTAIMENT_FM:
        MMIAPIFM_OpenMainWin();
        break;

#ifdef MMI_FM_TIMER_RECORD_SUPPORT
    case ID_ENTERTAIMENT_FM_TIMER_RECORD:
        MMIAPIFM_CreateFMTimerRecordInfoWin();    
        break; 
#endif
#endif

#ifdef BT_DIALER_SUPPORT
	case ID_TOOLS_BTPHONE:
        MMIAPIBT_OpenBtPhoneWin();
        break;
#endif
#ifdef STREAMING_SUPPORT_NEXTREAMING
    case ID_ENTERTAIMENT_ST:
        MMIAPIST_SetFromNet(FALSE);
  MMIAPIST_OpenPlayWin();
        break;
#endif

#ifdef KURO_SUPPORT
    case ID_ENTERTAIMENT_KUR:
        MMIAPIKUR_OpenKurPlayer();
        break;
#endif

#ifdef CMMB_SUPPORT
    case ID_ENTERTAIMENT_MTV:
        MMIAPIMTV_OpenMTVPlayer();
        break;
#endif

#ifdef ATV_SUPPORT
    case ID_ENTERTAIMENT_ATV:
        MMIAPIATV_OpenATV();
        break;
#endif

#ifdef WORLD_CLOCK_SUPPORT        
    case ID_WORLDTIME:
        MMIAPIWCLK_OpenMainWin();
        break;
#endif

#ifdef MMI_PDA_SUPPORT
#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
#ifdef COUNTEDTIME_SUPPORT
    case ID_COUNTDOWN:
        MMIAPI_OpenTimerWin(ID_COUNTDOWN);   
        break;
#endif
#endif
#else
#ifdef COUNTEDTIME_SUPPORT
    case ID_COUNTDOWN:
       if (MMIAPICT_IsShowIdleCoutime())
       {
            MMIAPICT_OpenCoutimeRunningWin();
       }
       else       
       {
            MMIAPICT_OpenCoutimeWin();
       }
       break;
#endif
#if defined(STOPWATCH_SUPPORT) && defined(OLD_STOPWATCH_SUPPORT)
    case ID_STOPWATCH:
        MMIAPIST_OpenStopWatchWin();
        break;
#endif            
#endif//  MMI_PDA_SUPPORT 

#ifdef HOME_LOCATION_INQUIRE_SUPPORT
    case ID_PHONESEARCH:
        MMIAPIPHONESEARCH_OpenWnd();
        break;
#endif

#ifdef DYNAMIC_MODULE_SUPPORT
    case ID_DYNAPP:
        MMIAPIDYNA_OpenMainListWindow();
        break;
#endif

    case ID_COMMUNICATION_CL:
        MMIAPICL_OpenMainMenuWindow();
        break;
  
#ifdef ASP_SUPPORT
    case ID_TOOLS_ASP:
     MMIAPIASP_EnterAsp();
    break;
#endif
#ifdef MMIEMAIL_SUPPORT
    case ID_COMMUNICATION_EMAIL:
        MMIAPIEMAIL_Entry();
        break;
#endif
    case ID_TOOLS_ALARM:
        MMIAPIALM_OpenMainWin();
        break;

#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
    case ID_CLOCK:
        MMIAPI_OpenTimerWin(ID_CLOCK);   
    break;
#endif
#ifdef CALENDAR_SUPPORT
    case ID_TOOLS_CALENDAR:
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_CALENDAR, MMIAPICALEND_OpenMainWin);
        break;
#endif
#ifdef CALCULATOR_SUPPORT
    case ID_TOOLS_CALCULATOR:
        MMIAPICALC_OpenMainWin();
        break;
#endif

#ifdef MMI_UNITCONVERSION_SUPPORT        
case ID_TOOLS_UNIT_CONVERSION:
        MMIAPIUNITCONV_OpenMainWin();
        break;
#endif

#ifdef PIM_SUPPORT
   case ID_MAINMENU_PIM:
     MMIAPIPIM_Main(0);
    break;
#endif
    
#ifdef MMI_RECORD_SUPPORT    
    case ID_TOOLS_RECORD:
        MMIAPIRECORD_OpenMainWin(PNULL);
        break;
#endif

    case ID_COMMUNICATION_STK:
        {
            uint16 sim_sys = MN_DUAL_SYS_1;
            uint32 sim_num = 0;

            if (MMIAPISET_GetFlyMode())
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
                break;
            }

            sim_num = MMIAPISTK_GetStkCardNum(&sim_sys, 1);
            if (sim_num == 0)
            {
                MMIAPIPHONE_AlertSimNorOKStatus();
            }
            else if (sim_num == 1)
            {
                MMIAPISTK_EnterToplevelMenu((MN_DUAL_SYS_E)sim_sys);
            }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            else
            {
                MMIAPISTK_SelectSIMFunc();
            }
#endif
        }
        break;

    case ID_DOWNLOAD_MANAGER:
#ifdef DL_SUPPORT
        MMIAPIDL_StartDownload(MMIDL_OPEN_DOWNLOAD_MANAGER, PNULL);
#endif
        break;
#ifdef PIC_VIEWER_SUPPORT
    case ID_PIC_VIEWER:
        MMIAPIPICVIEW_OpenPicViewer();
        break;
#endif
#ifdef JAVA_SUPPORT
    case ID_ENTERTAIMENT_GOLDHOUSE:
        MMIAPIJAVA_OpenMainMenu();
        break;
#endif
    case ID_EBOOK:
#ifdef EBOOK_SUPPORT
        if ( MMIAPISET_IsOpenPrivacyProtect(MMISET_PROTECT_EBOOK) )
        {
            MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_EBOOK,MMIAPIEBOOK_ValidatePrivacyAppEntry);
        }
        else
        {
            MMIAPIEBOOK_ManagerWinMsg(TRUE);
        }
#endif
        break;
#ifdef MMI_ENABLE_DCD            
   case ID_DCD_OPEN:     
        {
            MMIAPIDCD_OpenMainWin();
        }   
     break;
#endif    

#ifdef SXH_APPLE_SUPPORT
    	case ID_SXH_APPLE:
		{
			extern void APPLE_StartApple(void);
			APPLE_StartApple();
		}
		break;
        case ID_SXH_BOOK:
		    {
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_BOOK);
				}
				else
				{
					extern void APPLE_StartBook(void);
					APPLE_StartBook();
				}
			}
		break;
		case ID_SXH_UMVCHAT:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_UMVCHAT);	
				}
				else
				{
					extern void APPLE_StartUMvchat(void);
					APPLE_StartUMvchat();
				}
			}
		break;
		case ID_SXH_TV1:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv1();
				}
				else
				{
					extern void APPLE_StartTv1(void);
					APPLE_StartTv1();
				}
			}
		break;
		case ID_SXH_TV2:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv2();	
				}
				else
				{
					extern void APPLE_StartTv2(void);
					APPLE_StartTv2();
				}
			}
		break;
		case ID_SXH_TV3:
			{
				if(OEMOS_GetLangId() == 86)
				{
					SMDMenuTv3();	
				}
				else
				{
					extern void APPLE_StartTv3(void);
					APPLE_StartTv3();
				}
			}
		break;
		case ID_SXH_TV:		// switch (menu_id)下
			{
#ifdef APPLE_LOG    
			OEMOS_dbgprintf("ID_SXH_TV is ok OEMOS_GetLangId is %d" ,OEMOS_GetLangId());
#endif		

				if(OEMOS_GetLangId() == 86)
				{	

					APPLE_StartSMDApp(SXH_TV);	
				}
				else
				{
					//MMK_CreateWin((uint32*)MAINMENU_ICON_APPLETV_WIN_TAB,NULL);
					extern void APPLE_StartTv1(void);
					APPLE_StartTv1();
				}
				
			}
		break;
		case ID_SXH_RADIO:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_RADIO);	
				}
				else
				{
					extern void APPLE_StartRadio(void);
					APPLE_StartRadio();
				}
			}
	break;
		case ID_SXH_Games:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_ONLINEGAME);	
				}
				else
				{
					extern void APPLE_StartOnlineGame(void);
					APPLE_StartOnlineGame();
				}
			}
			break;
		case ID_SXH_UMMUSIC:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_MUSIC);	
				}
				else
				{
					extern void APPLE_StartMusic(void);
					APPLE_StartMusic();
				}
			}
			break;
		case ID_SXH_VIDEODL:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_VIDELDL);	
				}
				else
				{
					extern void APPLE_StartVideoDL(void);
					APPLE_StartVideoDL();
				}
			}
			break;
		case ID_SXH_UMPLAYER:
			{
				extern void APPLE_StartUMPlayer(void);
				APPLE_StartUMPlayer();
			}
			break;	
		case ID_SXH_WEATHER:
			{
				if(OEMOS_GetLangId() == 86)
				{
					APPLE_StartSMDApp(SXH_WEATHER);	
				}
				else
				{
					extern void APPLE_StartWeather(void);
					APPLE_StartWeather();
				}
			}
			break;
			
#ifdef SXH_APPLE_3GVEDIO_SUPPORT	
	case ID_SXH_UMVCHATTHIN:
		{
			APPLE_StartUMvchatthin();
		}
	break;
#endif // SXH_APPLE_3GVEDIO_SUPPORT	
#ifdef SXH_APPLE_UMVCHAT_DEMO
       case ID_SXH_UMVCHATDEMO:
	   	{
			APPLE_StartUMvchatDemo();
       	}
	   break;
#endif
#endif


#ifdef CMCC_UI_STYLE
    case ID_EXPLORE:
  MMIAPIFMM_OpenExplorer();
  break;

#ifdef BLUETOOTH_SUPPORT
    case ID_BLUETOOTH:
  MMIAPIBT_OpenMainMenuWin();   
  break;
#endif

     case ID_CUSTMER_SERVICE:
  MMIAPICS_OpenMainWin();    
  break;

     case ID_NET_MUSIC:
   MMIAPISET_EnterMusicWin();
   break;
     case ID_NET_FETION:
   MMIAPISET_EnterFetionWin();
   break;
     case  ID_CHINAMOBILE_MAP:
   MMIAPISET_EnterMapWin();
   break;

#ifdef GAME_SUPPORT
    case ID_ENTERTAIMENT_GAME:
        MMIAPIGAME_OpenMainMenuWindow();
        break;
#endif

     case ID_JAVA_STOCK:
               MMIAPISET_EnterStockWin();    
   break;
#endif 
#ifdef GAME_LINK_SUPPORT
    case IDGAME_MENU_LINK_ITEM_ID:
        MMIAPIGMLINK_OpenGameWin();
        break;
#endif

#ifdef GAME_BOXMAN_SUPPORT
    case IDGAME_MENU_BOXMAN_ITEM_ID:
        MMIAPIGMBOXMAN_OpenGameWin();
        break;
#endif

#ifdef GAME_SNAKE_SUPPORT
    case IDGAME_MENU_SNAKE_ITEM_ID:
        MMIGMSNAKE_OpenGameWin();
        break;
#endif

#ifdef GAME_TETRIS_SUPPORT
    case IDGAME_MENU_SQUARE_ITEM_ID:
        MMIGMSQU_OpenGameWin();
        break;
#endif

    case ID_SETTINGS_CALL:
        MMIAPISET_OpenCallSettingsWindow();
        break;

    case ID_SETTINGS_PHONE:
        MMIAPISET_EnterPhoneSettingWin();
        break;

#ifdef MMI_INDIAN_SOS_SUPPORT
        case ID_SETTINGS_SOS:
            MMIAPISET_EnterSOSSettingWin();
            break;
#endif

    case ID_SETTINGS_DISPLAY:
        MMIAPISET_EnterDisplaySettingWin();
        break;

    case ID_SETTINGS_SECURITY:
        MMIAPISET_EnterSecuritySettingWin();
        break;

    case ID_SETTINGS_ENVIONMENT:
        MMIENVSET_OpenMainMenuWindow();
        break;

#if defined (MMI_TVOUT_ENABLE) || defined (MMI_GPRS_SUPPORT) || defined (BLUETOOTH_SUPPORT) 
    case ID_SETTINGS_LINK:
        MMIAPICONNECTION_OpenMenuWin();
        break;
#else
    case ID_SETTINGS_LINK:
        MMIAPICONNECTION_OpenMenuWin();
        break;
#endif
        
#ifdef  CMCC_UI_STYLE 
    case ID_SETTINGS_UPLMN:
         MMIENGTD_OpenUPLMNSettingWin();
         break;
#endif

#ifdef CMCC_UI_STYLE
#ifdef JAVA_SUPPORT
    case ID_FAVORITE_GOLDENHOUSE:
        MMIAPIJAVA_OpenMainMenu();
        break;
#endif
    case ID_FAVORITE_RING:
        MMIAPIFMM_OpenMusicFileList();
        break;
    case ID_FAVORITE_PIC:
        MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_DOWNLOAD_PIC);
        break;
    case ID_FAVORITE_PHOTO:
        MMIAPIPICVIEW_OpenPicViewer();
        break;
     case ID_FAVORITE_VIDEO://xuefangjiang add 20101102
        MMIAPIFMM_OpenMovieFileList();
        break;
     case ID_SETTINGS_VERSION:
        MMIAPIENG_CreateShowVersionWin();
        break;
#endif

#ifdef QQ_SUPPORT
    case ID_SPREADTRUM_QQ_SUPPORT:
        MMIAPIQQ_OpenMainWin();
        break;
#endif

#ifdef PUSH_EMAIL_SUPPORT   //Added by yuanl  2010.5.7
    case ID_MAINMENU_PUSHMAIL:
        MMIAPIMAIL_Enter(MAIL_ENTER_MAINMENU, PNULL);
        break;
#endif /* PUSH_EMAIL_SUPPORT  */
 /*added by zack@20090915 for mrapp start*/
#ifdef MRAPP_SUPPORT
#ifdef MRAPP_COOKIE_OPEN
    case ID_MRAPP_COOKIE:
        MMIMRAPP_AppEntry(MRAPP_COOKIE);
        break;
#endif

#ifdef MRAPP_MRPSTORE_OPEN
    case ID_MRAPP_MRPSTORE:
        MMIMRAPP_AppEntry(MRAPP_MRPSTORE);
        break;
#endif

#ifdef MRAPP_SKYBUDDY_OPEN
		case ID_MRAPP_SKYBUDDY:
            MMIMRAPP_AppEntry(MRAPP_SKYBUDDY);
            break;
#endif

#ifdef MRAPP_SDKQQ_OPEN
    case ID_MRAPP_QQ:
        MMIMRAPP_AppEntry(MRAPP_SDKQQ);
        break;
#endif

#ifdef MRAPP_MSN_OPEN
    case ID_MRAPP_MSN:
        MMIMRAPP_AppEntry(MRAPP_MSN);
        break;
#endif

#ifdef MRAPP_EBOOK_OPEN
	case ID_MRAPP_BOOKS:
		MMIMRAPP_AppEntry(MRAPP_EBOOK);
		break;		
#endif

#ifdef MRAPP_NETGAME_OPEN
   case ID_MRAPP_NETGAME:
        MMIMRAPP_AppEntry(MRAPP_NETGAME);
        break;
#endif

#ifdef MRAPP_TOOL_OPEN
    case ID_MRAPP_TOOL:
        MMIMRAPP_AppEntry(MRAPP_TOOL);
        break;
#endif

#ifdef MRAPP_ENDICT_OPEN
    case ID_MRAPP_ENDICT:
        MMIMRAPP_AppEntry(MRAPP_ENDICT);
        break;
#endif

#ifdef MRAPP_CHDICT_OPEN
    case ID_MRAPP_CHDICT:
        MMIMRAPP_AppEntry(MRAPP_CHDICT);
        break;
#endif

#ifdef MRAPP_TIANQI_OPEN
    case ID_MRAPP_TIANQI:
        MMIMRAPP_AppEntry(MRAPP_TIANQI);
        break;
#endif

#ifdef MRAPP_FETION_OPEN
    case ID_MRAPP_FETION:
        MMIMRAPP_AppEntry(MRAPP_FETION);
        break;
#endif

#ifdef MRAPP_XLWB_OPEN
    case ID_MRAPP_XLWB:
        MMIMRAPP_AppEntry(MRAPP_XLWB);
        break;
#endif

#ifdef MRAPP_KAIXIN_OPEN
   case ID_MRAPP_KAIXIN:
        MMIMRAPP_AppEntry(MRAPP_KAIXIN);
        break;
#endif

#ifdef MRAPP_FARM_OPEN
   case ID_MRAPP_FARM:
        MMIMRAPP_AppEntry(MRAPP_FARM);
        break;
#endif

#ifdef MRAPP_CLOCK_OPEN
    case ID_MRAPP_CLOCK:
        MMIMRAPP_AppEntry(MRAPP_CLOCK);
        break;
#endif

#ifdef MRAPP_FACEBOOK_OPEN
    case ID_MRAPP_FACEBOOK:
        MMIMRAPP_AppEntry(MRAPP_FACEBOOK);
        break;
#endif

#ifdef MRAPP_YAHOO_OPEN
    case ID_MRAPP_YAHOO:
        MMIMRAPP_AppEntry(MRAPP_YAHOO);
        break;
#endif

#ifdef MRAPP_SKYPE_OPEN
    case ID_MRAPP_SKYPE:
        MMIMRAPP_AppEntry(MRAPP_SKYPE);
        break;
#endif

#ifdef MRAPP_TWITTER_OPEN
   case ID_MRAPP_TWITTER:
        MMIMRAPP_AppEntry(MRAPP_TWITTER);
        break;
#endif
#endif
	/*added by zack@20090915 for mrapp end*/
#ifdef MMI_AZAN_SUPPORT
     case ID_AZAN:
      MMIAPIAZAN_OpenMuSiLinPrayWin();
      break;
#endif

#ifdef MMI_MEMO_SUPPORT
   case ID_APP_MEMO:
      MMIMEMO_OpenMain();
      break;
#endif
#ifdef MCARE_V31_SUPPORT
        case ID_TMC_APP_SUPPORT:
            MCareV31_Main_Entry();
            break;
        case ID_BRW_APP_SUPPORT:
            MCareV31_QQBrowser_Shortcut();
            break;
#endif

#ifdef WEATHER_SUPPORT
    case ID_MAINMENU_WEATHER:
        MMIAPIWEATHER_OpenMainWin();
        break;
#endif

#ifdef IKEYBACKUP_SUPPORT
    case ID_MAINMENU_IKEYBACKUP:
        MMIAPIIKB_OpenMainWin();
        break;
#endif

#ifdef WRE_SUPPORT
	case ID_TOOLS_WRE:
		{
			MMIWRE_Entry();
		}	
		break;
#ifdef WRE_CONTRA_SUPPORT  // 魂斗罗
	case ID_WRE_CONTRA:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_CONTRA, NULL);
            }	
            break;
#endif
#ifdef WRE_SUPERMARIO_SUPPORT  // 超级玛丽
	case ID_WRE_SUPERMARIO:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_SUPERMARIO, NULL);
            }	
            break;
#endif
#ifdef WRE_TSLAND_SUPPORT  // 冒险岛
	case ID_WRE_TSLAND:
            {
		  MMIWRE_StartAppEx(WRE_FIX_ID_TSLAND, NULL);
            }	
            break;
#endif
#endif
//NEWMS00173119
#ifdef BLUETOOTH_SUPPORT
    case ID_APPS_BT:
        MMIAPIBT_OpenMainMenuWin();
        break;
#endif
#ifdef HERO_ENGINE_SUPPORT
	#ifdef HERO_APP_WSTORE_OPEN 
		case ID_HERO_APP_WSTORE:
			MMIHEROAPP_AppEntry(HERO_APP_WSTORE);
			break;
		case ID_HERO_APP_WSTORE_MANAGE:
			MMIHEROAPP_AppEntry(HERO_APP_WSTORE_MANAGE);
			break;
	#endif	
#endif
#ifdef OD_GAME_SUPPORT
#ifdef OD_DOODLE_JUMP_SUPPORT
        case MENU_ID_OD_GAME_DOODLE_JUMP:
#endif
#ifdef OD_CROSSY_ROAD_SUPPORT
        case MENU_ID_OD_GAME_CROSSY_ROAD:
#endif
#ifdef OD_STICK_HERO_SUPPORT
        case MENU_ID_OD_GAME_STICK_HERO:   	
#endif
#ifdef OD_BUBBLE_PARTY_MIX_SUPPORT
        case MENU_ID_OD_GAME_BUBBLE_PARTY_MIX:  
#endif
#ifdef OD_SUPER_PET_SUPPORT
        case MENU_ID_OD_GAME_SUPER_PET:
#endif
#ifdef OD_RUNNER_SUPPORT
        case MENU_ID_OD_GAME_RUNNER: 
#endif
    {
        extern BOOLEAN od_game_handle_menu(MMI_MENU_ID_T menu_id);
        if(od_game_handle_menu(menu_id))
        {
            break;
        }
        break;
    }
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }  
    return result;
}

#ifndef MMI_MAINMENU_MINI_SUPPORT
/*****************************************************************************/
// Description : 二级菜单list控件处理函数
// Global resource dependence : 
// Author:jin.wang
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainSecondMenuListWinMsg(
                                              MMI_WIN_ID_T         win_id, 
                                              MMI_MESSAGE_ID_E     msg_id, 
                                              DPARAM               param
                                              )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    GUIMENU_GROUP_T *sc_menu_group_ptr = PNULL; //current menu group info
    MMI_MENU_GROUP_ID_T group_id = MENU_ICON_PASTIME;
    uint16 list_index = 0;
    GUILIST_NEED_ITEM_CONTENT_T * need_item_content_ptr = PNULL;
#if defined(DYNAMIC_MAINMENU_SUPPORT)	
    uint16 static_item_cnt = 0;
#endif
    
    //SCI_TRACE_LOW:"HandleMainSecondMenuListWinMsg, msg_id = 0x%02x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINMENU_WIN_3134_112_2_18_2_32_54_2,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {    
    case MSG_OPEN_WINDOW:    

#if defined(DYNAMIC_MAINMENU_SUPPORT)
        MMIMENU_DyGetAllItemData();
#endif

        // 添加list菜单项
        if (SecMenuList_Create(group_id))
        {        
            MMK_SetAtvCtrl(win_id,MAINMENUWIN_SECOND_LIST_CTRL_ID);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);      
        }
        else
        {
            MMK_CloseWin(win_id);
        }          
        break;
        
    case MSG_CTL_MIDSK:  
    case MSG_KEYDOWN_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:        
    case MSG_CTL_OK:
        sc_menu_group_ptr = MMI_GetMenuGroup(group_id);

#if defined(DYNAMIC_MAINMENU_SUPPORT)
        if (PNULL != sc_menu_group_ptr)
        {
            static_item_cnt = sc_menu_group_ptr->item_count;
        }
#endif
        
        list_index = GUILIST_GetCurItemIndex(MAINMENUWIN_SECOND_LIST_CTRL_ID); 
        
#if defined(DYNAMIC_MAINMENU_SUPPORT)

        if (list_index >= static_item_cnt)
        {
            GUIMAINMENU_DY_ITEM_T item_info = {0};
            BOOLEAN is_get_info = FALSE;
            
            list_index -= static_item_cnt;

            is_get_info = MMIMENU_DyListGetRunInfo(list_index, &item_info);
            
            if (is_get_info
                && PNULL != item_info.ori_data_ptr
                && MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
            {
                (item_info.ori_data_ptr->dynamic_menu_info.link_function_ptr)(
                    item_info.ori_data_ptr->dynamic_menu_info.param1ptr, 
                    item_info.ori_data_ptr->dynamic_menu_info.param2ptr);

                MMIMENU_DyFreeParmInfo(&item_info);
            }

            break;
        }
#endif
        if (PNULL != sc_menu_group_ptr)
        {
            HandleSecMenuStaticMsg(win_id,
                                   msg_id,
                                   sc_menu_group_ptr->item_ptr[list_index].menu_id);
        }
        
        break;
        
    case MSG_KEYDOWN_0:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        list_index = GetSecondMenuListIndex(msg_id);
        
        if (list_index != (uint16)DYNAMIC_MAINMENU_DEFAULT_LIST_INDEX)
        {
            GUILIST_SetCurItemIndex(MAINMENUWIN_SECOND_LIST_CTRL_ID, list_index);
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            MMK_PostMsg(win_id, MSG_MAINMENU_UPDATE, &list_index, sizeof(uint16));
        }
        break;


    case MSG_MAINMENU_UPDATE:
        sc_menu_group_ptr = MMI_GetMenuGroup(group_id);
        
#if defined(DYNAMIC_MAINMENU_SUPPORT)
        if (PNULL != sc_menu_group_ptr)
        {
            static_item_cnt = sc_menu_group_ptr->item_count;
        }
#endif
        
        list_index = *(uint16*)param;
        
#if defined(DYNAMIC_MAINMENU_SUPPORT)
        if (list_index >= static_item_cnt)
        {
            GUIMAINMENU_DY_ITEM_T item_info = {0};
            BOOLEAN is_get_info = FALSE;
            
            list_index -= static_item_cnt;
            is_get_info = MMIMENU_DyListGetRunInfo(list_index, &item_info);
            
            if (is_get_info
                && PNULL != item_info.ori_data_ptr
                && MMIMENU_DyIsRunInfoValid(&(item_info.ori_data_ptr->dynamic_menu_info)))
            {
                (item_info.ori_data_ptr->dynamic_menu_info.link_function_ptr)(
                    item_info.ori_data_ptr->dynamic_menu_info.param1ptr, 
                    item_info.ori_data_ptr->dynamic_menu_info.param2ptr);
                
                MMIMENU_DyFreeParmInfo(&item_info);
            }

            break;
        }
#endif
        if (PNULL != sc_menu_group_ptr)
        {
            HandleSecMenuStaticMsg(win_id,
                                   msg_id,
                                   sc_menu_group_ptr->item_ptr[list_index].menu_id);
        }

        break;
          
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:     
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        sc_menu_group_ptr = MMI_GetMenuGroup(group_id);

        //add text into list item
        need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        
        if (PNULL != need_item_content_ptr)
        {
            SecMenuList_SetItem(MAINMENUWIN_SECOND_LIST_CTRL_ID,
                need_item_content_ptr->item_index, 
                sc_menu_group_ptr);
        }
        
        break;

#if defined(DYNAMIC_MAINMENU_SUPPORT)
    case MSG_CTL_LIST_NEED_ITEM_CONTENT:
        sc_menu_group_ptr = MMI_GetMenuGroup(group_id);
        
        need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
        
        if (PNULL != need_item_content_ptr)
        {
            SecMenuList_SetDyPic(need_item_content_ptr->item_index,
                need_item_content_ptr->item_content_index,
                MAINMENUWIN_SECOND_LIST_CTRL_ID,
                sc_menu_group_ptr,
                win_id);
        }
        break;
#endif
        
    case MSG_GET_FOCUS:

#if defined(DYNAMIC_MAINMENU_SUPPORT)
        MMIMENU_DyGetAllItemData();
#endif

        list_index = GUILIST_GetCurItemIndex(MAINMENUWIN_SECOND_LIST_CTRL_ID); 

        // 添加list菜单项数
        if (SecMenuList_Create(group_id))
        {        
            MMK_SetAtvCtrl(win_id,MAINMENUWIN_SECOND_LIST_CTRL_ID);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);      
            GUILIST_SetCurItemIndex(MAINMENUWIN_SECOND_LIST_CTRL_ID, list_index);
        }

        break;
        
    case MSG_CLOSE_WINDOW:
#if defined(DYNAMIC_MAINMENU_SUPPORT)
        MMIMENU_DyFreeData();
#endif 
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}
#endif

#ifdef MMITHEME_MAINMENU_ARRANGE

/*****************************************************************************/
// Description : HandleMainMenuOptWinMsg
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMainMenuOptWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM                param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    GUIMENU_MOVE_STATE_T move_state = GUIMENU_STATE_MOVE_NONE;

    //SCI_TRACE_LOW:"HandleMainMenuOptWinMsg msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINMENU_WIN_3356_112_2_18_2_32_54_3,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MAIN_MAINMENU_OPT_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MAIN_MAINMENU_OPT_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_MAINMENU_OPT_ARRANGE:
            move_state = GUIMENU_STATE_MOVE_SET;
            MMK_PostMsg(MAIN_MAINMENU_WIN_ID, MSG_NOTIFY_MENU_SET_STATUS, &move_state, sizeof(GUIMENU_MOVE_STATE_T));
            break;

        case ID_MAINMENU_OPT_HELP:
            {
                MMI_STRING_T    string = {0};
                MMI_CTRL_ID_T   ctrl_id = MAIN_MAINMENU_HELP_CTRL_ID;

                MMK_CreateWin((uint32*)MAINMENU_HELP_WIN_TAB, PNULL);
                MMI_GetLabelTextByLang(TXT_MAINMENU_ARRANGE_HELP, &string);
                GUITEXT_SetString(ctrl_id,string.wstr_ptr,string.wstr_len,FALSE);
            }
            break;

        default:
            // 不可能有其他的菜单选项，除非又新加了，那得再加case
            SCI_PASSERT(FALSE,("HandleMainMenuOptWinMsg menu_id = %d", menu_id));   /*assert to do*/
        }
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
// Description : HandleMainMenuMoveOptWinMsg
// Global resource dependence : 
// Author:xiaoqing.lu
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuMoveOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM                param
                                               )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T menu_id = 0;
    GUIMENU_MOVE_STATE_T move_state = GUIMENU_STATE_MOVE_NONE;

    //SCI_TRACE_LOW:"HandleMainMenuMoveOptWinMsg msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MAINMENU_WIN_3422_112_2_18_2_32_55_4,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MAIN_MAINMENU_MOVEOPT_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MAIN_MAINMENU_MOVEOPT_CTRL_ID,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_MAINMENU_OPT_HELP:
            {
                MMI_STRING_T    string = {0};
                MMI_CTRL_ID_T   ctrl_id = MAIN_MAINMENU_HELP_CTRL_ID;

                MMK_CreateWin((uint32*)MAINMENU_HELP_WIN_TAB, PNULL);
                MMI_GetLabelTextByLang(TXT_MAINMENU_HELP, &string);
                GUITEXT_SetString(ctrl_id,string.wstr_ptr,string.wstr_len,FALSE);
            }
            break;

        case ID_MAINMENU_OPT_EXIT:
            move_state = GUIMENU_STATE_MOVE_CANCEL;
            MMK_PostMsg(MAIN_MAINMENU_WIN_ID, MSG_NOTIFY_MENU_SET_STATUS, &move_state, sizeof(GUIMENU_MOVE_STATE_T));
            // GUIMAINMENU_CancelMoveIcon(MAIN_MAINMENU_MOVEOPT_CTRL_ID);
            break;

        default:
            // 不可能有其他的菜单选项，除非又新加了，那得再加case
            SCI_PASSERT(FALSE,("HandleMainMenuMoveOptWinMsg menu_id = %d", menu_id)); /*assert to do*/
        }
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
// FUNCTION:     HandleMainMenuHelpWinMsg
// Description :   handle help win msg
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMainMenuHelpWinMsg( 
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    // MMI_STRING_T    string = {0};
    MMI_CTRL_ID_T   ctrl_id = MAIN_MAINMENU_HELP_CTRL_ID;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
// MMI_GetLabelTextByLang(TXT_MAINMENU_HELP, &string);
// GUITEXT_SetString(ctrl_id,string.wstr_ptr,string.wstr_len,FALSE);
// 
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
  
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#endif

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
// FUNCTION:     HandleBrowerUrlMenu
// Description :   处理主菜单中通过browser 访问url的菜单项
// Global resource dependence : 
// Author: fen.xie
// Note:   
/*****************************************************************************/
LOCAL void HandleBrowerUrlMenu(MAINMENU_BROWSER_URL_TYPE_E browser_url_type)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    if (browser_url_type >= BROWSER_URL_TYPE_MAX)
    {
        return;
    }

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.user_agent_ptr = PNULL;
    
    entry_param.url_ptr = (char *)s_mainmenu_browser_url[browser_url_type];
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}
#endif


#ifndef MMI_MAINMENU_MINI_SUPPORT
/*****************************************************************************/
// Description : 创建list
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_Create(MMI_MENU_GROUP_ID_T group_id)
{
    uint16 item_cnt = 0;
    uint16 list_index = 0;
    GUIMENU_GROUP_T *sc_menu_group_ptr = PNULL; //current menu group info
#if defined(DYNAMIC_MAINMENU_SUPPORT)
    uint32 dy_menu_count = 0;
#endif

    //读取静态菜单数据个数
    sc_menu_group_ptr = MMI_GetMenuGroup(group_id);
    
    if (PNULL != sc_menu_group_ptr)
    {
        item_cnt = sc_menu_group_ptr->item_count;
    }

#if defined(DYNAMIC_MAINMENU_SUPPORT)
    dy_menu_count = MMIMENU_DyGetItemCount();

    if (dy_menu_count > 0)//有动态菜单数据
    {
        item_cnt += (uint16)dy_menu_count;
    }
#endif

    if (0 == item_cnt)
    {
        return FALSE;
    }

    GUILIST_SetSyncDecodeAnim(MAINMENUWIN_SECOND_LIST_CTRL_ID, TRUE); 

    GUILIST_SetMaxItem(MAINMENUWIN_SECOND_LIST_CTRL_ID, item_cnt, TRUE);
    
#ifdef JAVA_SUPPORT
    GUILIST_SetDefaultIcon(MAINMENUWIN_SECOND_LIST_CTRL_ID, IMAGE_SECMENU_ICON_DYNAMIC_DEFAULT, IMAGE_SECMENU_ICON_DYNAMIC_DEFAULT);
#endif

    for (list_index = 0; list_index < item_cnt; list_index++)
    {
        GUILIST_ITEM_T listbox_item  = {0};
        GUILIST_ITEM_DATA_T item_data  = {0};

        //图标 + 文本文字
        listbox_item.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TEXT; 
         
        listbox_item.item_data_ptr = &item_data;
        GUILIST_AppendItem (MAINMENUWIN_SECOND_LIST_CTRL_ID, &listbox_item); 
    }

    // Update listbox
    GUILIST_SetCurItemIndex(MAINMENUWIN_SECOND_LIST_CTRL_ID, 0);
    GUILIST_SetSelectedItem(MAINMENUWIN_SECOND_LIST_CTRL_ID, 0, TRUE);
    
    return TRUE;
}

/*****************************************************************************/
// Description : 给list控件传入资源
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_SetItem(
    MMI_CTRL_ID_T ctrl_id,         //in:control id
    uint16 item_index,     //in:item索引
    GUIMENU_GROUP_T *sc_menu_group_ptr
)
{
    BOOLEAN result = FALSE;
    GUILIST_ITEM_DATA_T item_data = {0};
    uint16 static_item_cnt = 0;
    BOOLEAN has_get_item = FALSE;
    
    if (PNULL != sc_menu_group_ptr)
    {
        static_item_cnt = sc_menu_group_ptr->item_count;
    }
    
#if defined(DYNAMIC_MAINMENU_SUPPORT)
    //动态菜单
    if (item_index >= static_item_cnt)
    {
        uint16 dy_item_index = 0;
        GUIMAINMENU_DY_ITEM_T item_info ={0};
        BOOLEAN is_get_info = FALSE;
        
        //计算出动态菜单的index
        dy_item_index = item_index - static_item_cnt;

        is_get_info = MMIMENU_DyListGetDispInfoEx(dy_item_index, &item_info);

        if (is_get_info && PNULL != item_info.ori_data_ptr)
        {
            //图片
            if (item_info.ori_data_ptr->dynamic_menu_info.has_icon)
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
            }
            else
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
                item_data.item_content[0].item_data.image_id = IMAGE_SECMENU_ICON_DYNAMIC_DEFAULT;
            }
            
            //文本
            item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            item_data.item_content[1].item_data.text_buffer.wstr_ptr 
                = item_info.ori_data_ptr->dynamic_menu_info.text;
            item_data.item_content[1].item_data.text_buffer.wstr_len
                = (uint16)MIN(GUIMENU_DYMAINMENU_STR_MAX_NUM, 
            item_info.ori_data_ptr->dynamic_menu_info.text_len);

            has_get_item = TRUE;
        }
    }
    else
#endif
    if (static_item_cnt > 0 && PNULL != sc_menu_group_ptr)
    {
        //静态菜单
        //图片
        item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = sc_menu_group_ptr->item_ptr[item_index].select_icon_id;
        //文本
        item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[1].item_data.text_id
            = sc_menu_group_ptr->item_ptr[item_index].text_str_id;

        has_get_item = TRUE;
    }
    
    // 设置字符信息
    if (has_get_item)
    {
        result = GUILIST_SetItemData(ctrl_id,&item_data,item_index);
    }
    
    return result;
}

/*****************************************************************************/
// Description : 获取list控件被选中index
// Global resource dependence : 
// Author:jinwang
// Note:返回list控件被选中的list index
/*****************************************************************************/
LOCAL uint32 GetSecondMenuListIndex(MMI_MESSAGE_ID_E msg_id)
{
    uint32 list_index = 0;
    uint16 list_num = 0;
    
    list_num = GUILIST_GetTotalItemNum(MAINMENUWIN_SECOND_LIST_CTRL_ID);//list控件item总个数
    list_index = (uint32)(msg_id - MSG_APP_1);
    
    if (list_index >= list_num)
    {
        list_index = DYNAMIC_MAINMENU_DEFAULT_LIST_INDEX;
    }
    
    return list_index;
}

#if defined(DYNAMIC_MAINMENU_SUPPORT)
/*****************************************************************************/
// Description : 给list控件传入动态菜单图片信息
// Global resource dependence : 
// Author:jinwang
// Note:成功返回TRUE，失败返回FALSE
/*****************************************************************************/
LOCAL BOOLEAN SecMenuList_SetDyPic(
    uint16 item_index,  //控件索引
    uint16 item_content_index,//内容索引
    MMI_CTRL_ID_T ctrl_id,
    GUIMENU_GROUP_T *sc_menu_group_ptr,
    MMI_WIN_ID_T win_id
)
{
    uint16 static_item_cnt = 0;
    GUILIST_ITEM_DATA_T item_data = {0}; 
    GUIANIM_DATA_INFO_T anim_data = {0};
    BOOLEAN result = FALSE;
    uint16 dy_item_index = 0;
    GUIMAINMENU_DY_ITEM_T item_info ={0};
    BOOLEAN is_get_info = FALSE;

    if (PNULL != sc_menu_group_ptr)
    {
        static_item_cnt = sc_menu_group_ptr->item_count;
    }

    if (item_index < static_item_cnt)
    {
        return FALSE;
    }
    
    //计算出动态菜单的index
    dy_item_index = item_index - static_item_cnt;
    
    is_get_info = MMIMENU_DyListGetDispInfo(dy_item_index, &item_info);
           
    if (is_get_info
        && item_info.ori_data_ptr->dynamic_menu_info.has_icon
        && PNULL != item_info.ori_data_ptr
        && PNULL != item_info.ori_data_ptr->dynamic_menu_info.icon_ptr)
    {

        //图片数据
        item_data.item_content[0].item_data_type = GUIITEM_DATA_ANIM_DATA;
        item_data.item_content[0].item_data.anim_data_ptr = &anim_data;
        
        anim_data.is_bitmap = FALSE;
        anim_data.is_save_data = TRUE;
        anim_data.data_ptr = item_info.ori_data_ptr->dynamic_menu_info.icon_ptr;
        anim_data.data_size = item_info.ori_data_ptr->dynamic_menu_info.icon_datasize;
        
        GUILIST_SetItemContent(ctrl_id, 
                               &item_data.item_content[item_content_index], 
                               item_index, 
                               item_content_index);

        SCI_FREE(item_info.ori_data_ptr->dynamic_menu_info.icon_ptr);

        result = TRUE;
    }   

    return result;
}
#endif
#endif

#ifdef FLASH_SUPPORT
PUBLIC void MMIAPIMENU_EnterEntertainment(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_PASTIME_WIN_TAB,PNULL);
}
#endif

PUBLIC void MMIAPIMENU_EnterSetting(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_SETTINGS_WIN_TAB,PNULL);
}

PUBLIC void MMIAPIMENU_EnterTools(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_TOOLS_WIN_TAB,PNULL);
}

PUBLIC void MMIAPIMENU_EnterPlayer(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_PLAYER_WIN_TAB,PNULL);
}

#ifdef CMCC_UI_STYLE
PUBLIC void MMIAPIMENU_EnterMyFavorite(void)
{
    MMK_CreateWin ( (uint32 *) MAINMENU_ICON_MYFAVORITE_WIN_TAB, PNULL);
}

PUBLIC void MMIAPIMENU_EnterCmccService(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_SERVICE_WIN_TAB,PNULL);
}

PUBLIC void MMIAPIMENU_EnterApps(void)
{
    MMK_CreateWin((uint32 *)MAINMENU_ICON_APPS_WIN_TAB,PNULL);
}
#endif

/*Edit by script, ignore 5 case. Fri Apr 27 09:38:51 2012*/ //IGNORE9527
