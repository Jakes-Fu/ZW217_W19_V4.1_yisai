/*****************************************************************************
** File Name:      mmi_mainmenu_item_func.h                                  *
** Author:                                                                   *
** Date:           10/10/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_ITEM_FUNC_H_
#define _MMI_MAINMENU_ITEM_FUNC_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre.h"
#endif
#ifdef MRAPP_SUPPORT
#include "mmimrapp.h"
#endif
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
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
} BROWSER_URL_TYPE_E;
#endif


/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DECLARE                                                                                 */
/*---------------------------------------------------------------------------*/
#ifdef BROWSER_SUPPORT
PUBLIC void MMIMAINMENU_StartCommunicationBrowser (uint32 param1, uint32 param2);
#endif


#ifdef OPERA_MINI_SUPPORT
PUBLIC void MMIMAINMENU_StartOperaMinBrowser (uint32 param1, uint32 param2);
#endif


#ifdef BROWSER_SUPPORT_NETFRONT
PUBLIC void MMIMAINMENU_StartNetFrontBrowser (uint32 param1, uint32 param2);
#endif


#ifdef BROWSER_SUPPORT_DORADO
PUBLIC void MMIMAINMENU_StartDoradoBrowser (uint32 param1, uint32 param2);
#endif


PUBLIC void MMIMAINMENU_StartSTK (uint32 param1, uint32 param2);



#ifdef EBOOK_SUPPORT
PUBLIC void MMIMAINMENU_StartEBOOK (uint32 param1, uint32 param2);

#endif


#ifdef CALENDAR_SUPPORT
PUBLIC void MMIMAINMENU_StartCalendar (uint32 param1, uint32 param2);
#endif

PUBLIC void MMIMAINMENU_StartFavorite (uint32 param1, uint32 param2);



#ifdef CMCC_UI_STYLE
PUBLIC void MMIMAINMENU_EnterCmccService(uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_EnterApps(uint32 param1, uint32 param2);

#ifdef BROWSER_SUPPORT
PUBLIC void MMIMAINMENU_EnterMonternetWin (uint32 param1, uint32 param2);
#endif

#ifdef PIC_VIEWER_SUPPORT
PUBLIC void MMIMAINMENU_StartPic (uint32 param1, uint32 param2);

#endif
#endif //CMCC_UI_STYLE


PUBLIC void MMIMAINMENU_StartCreateWin (uint32 *win_tab_ptr);



#ifndef MMI_PDA_SUPPORT
#ifdef COUNTEDTIME_SUPPORT
PUBLIC void MMIMAINMENU_StartCoutDown (uint32 param1, uint32 param2);
#endif
#endif //MMI_PDA_SUPPORT


#ifdef PUSH_EMAIL_SUPPORT
PUBLIC void MMIMAINMENU_StartPushMail (uint32 param1, uint32 param2);
#endif

#ifdef WRE_SUPPORT
PUBLIC void MMIMAINMENU_StartWreApp (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartWreFixApp (uint32 param1, uint32 param2);
#endif


#ifdef SXH_APPLE_SUPPORT
PUBLIC void MMIMAINMENU_StartSXHBook (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHUmvchat (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHTV1 (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHTV2 (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHTV3 (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHTV (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHRadio (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHGames (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHUmmusic (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHVideodl (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHUmplayer (uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_StartSXHWeather (uint32 param1, uint32 param2);
#endif


#ifdef MMI_GPRS_SUPPORT
PUBLIC void MMIMAINMENU_StartBrowerURL (uint32 param1, uint32 param2);
#endif


#ifdef MMI_PDA_SUPPORT
#if defined(MMI_ANDROID_SUPPORT) && (defined MMI_ISTYLE_SUPPORT || defined MMI_GRID_IDLE_SUPPORT || defined MMI_SMART_IDLE_SUPPORT)
PUBLIC void MMIMAINMENU_StartIdleStyle (uint32 param1, uint32 param2);
#endif
#endif


PUBLIC void MMIMAINMENU_StartDial (uint32 param1, uint32 param2);


PUBLIC void MMIMAINMENU_StartMessage (uint32 param1, uint32 param2);


#ifdef MMIEMAIL_SUPPORT
PUBLIC void MMIMAINMENU_StartIEmail (uint32 param1, uint32 param2);
#endif


PUBLIC void MMIMAINMENU_StartCL (uint32 param1, uint32 param2);


PUBLIC void MMIMAINMENU_StartPB (uint32 param1, uint32 param2);

#ifdef WORLD_CLOCK_SUPPORT
PUBLIC void MMIMAINMENU_StartWordTime (uint32 param1, uint32 param2);
#endif

#ifdef  CAMERA_SUPPORT
PUBLIC void MMIMAINMENU_StartDCPhoto (uint32 param1, uint32 param2);
PUBLIC void MMIMAINMENU_StartDCVideo (uint32 param1, uint32 param2);

#if defined(MMIDC_F_U_CAMERA)
PUBLIC void MMIMAINMENU_StartDCUCameraPhoto (uint32 param1, uint32 param2);
#endif
#endif


#ifdef MMI_KING_MOVIE_SUPPORT
PUBLIC void MMIMAINMENU_StartKM (uint32 param1, uint32 param2);
#endif

#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
PUBLIC void MMIMAINMENU_StartMp3Player (uint32 param1, uint32 param2);
#endif


#ifdef PIC_VIEWER_SUPPORT
PUBLIC void MMIMAINMENU_StartPicViewer (uint32 param1, uint32 param2);
#endif

#ifdef GAME_SUPPORT
PUBLIC void MMIMAINMENU_StartGame (uint32 param1, uint32 param2);
#endif


#ifdef JAVA_SUPPORT
PUBLIC void MMIMAINMENU_StartJava (uint32 param1, uint32 param2);
#endif

PUBLIC void MMIMAINMENU_StartALM (uint32 param1, uint32 param2);


#ifdef ASP_SUPPORT
PUBLIC void MMIMAINMENU_StartASP (uint32 param1, uint32 param2);
#endif

#ifdef  DCD_SUPPORT
PUBLIC void MMIMAINMENU_StartDCD (uint32 param1, uint32 param2);
#endif



#ifdef MMI_RECORD_SUPPORT
PUBLIC void MMIMAINMENU_StartRecord (uint32 param1, uint32 param2);
#endif


#ifdef PIM_SUPPORT
PUBLIC void MMIMAINMENU_StartPIM (uint32 param1, uint32 param2);
#endif



#ifdef SEARCH_SUPPORT
PUBLIC void MMIMAINMENU_StartSearch (uint32 param1, uint32 param2);
#endif



#ifdef CALCULATOR_SUPPORT
PUBLIC void MMIMAINMENU_StartPicalc (uint32 param1, uint32 param2);
#endif


#ifdef MMI_UNITCONVERSION_SUPPORT
PUBLIC void MMIMAINMENU_StartUnitConv (uint32 param1, uint32 param2);
#endif


PUBLIC void MMIMAINMENU_StartCallSettings (uint32 param1, uint32 param2);


PUBLIC void MMIMAINMENU_StartPhoneSetting (uint32 param1, uint32 param2);

#if defined(MMI_INDIAN_SOS_SUPPORT) && defined(MMI_PDA_SUPPORT)
PUBLIC void MMIMAINMENU_StartSOSSetting (uint32 param1, uint32 param2);
#endif

PUBLIC void MMIMAINMENU_StartDisplaySetting (uint32 param1, uint32 param2);


PUBLIC void MMIMAINMENU_StartMainmenuStyle (uint32 param1, uint32 param2);


#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
PUBLIC void MMIMAINMENU_StartLockStyle (uint32 param1, uint32 param2);
#endif
#endif


PUBLIC void MMIMAINMENU_StartSecuritySetting (uint32 param1, uint32 param2);


PUBLIC void MMIMAINMENU_StartEnvSetting (uint32 param1, uint32 param2);


#if defined (MMI_TVOUT_ENABLE) || defined (MMI_GPRS_SUPPORT) || defined (BLUETOOTH_SUPPORT)
PUBLIC void MMIMAINMENU_StartConnection (uint32 param1, uint32 param2);
#endif


#ifdef SXH_APPLE_SUPPORT
PUBLIC void MMIMAINMENU_StartSXHApple (uint32 param1, uint32 param2);
#endif


#ifdef MMIUZONE_SUPPORT
PUBLIC void MMIMAINMENU_StartUzone (uint32 param1, uint32 param2);
#endif


#ifdef MMI_AUDIO_PLAYER_SUPPORT
PUBLIC void MMIMAINMENU_StartMp3 (uint32 param1, uint32 param2);
#endif


#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
PUBLIC void MMIMAINMENU_StartIVP (uint32 param1, uint32 param2);
#endif
#endif


#ifdef BLUETOOTH_SUPPORT
PUBLIC void MMIMAINMENU_StartBT (uint32 param1, uint32 param2);
#endif



#ifdef MMI_WIFI_SUPPORT
PUBLIC void MMIMAINMENU_StartWIFI (uint32 param1, uint32 param2);
#endif



#ifdef FM_SUPPORT
PUBLIC void MMIMAINMENU_StartFM (uint32 param1, uint32 param2);
#endif



#ifdef ATV_SUPPORT
PUBLIC void MMIMAINMENU_StartATV (uint32 param1, uint32 param2);
#endif


#ifdef CMMB_SUPPORT
PUBLIC void MMIMAINMENU_StartMTV (uint32 param1, uint32 param2);
#endif



#if (!defined MMI_ALARM_MINI_SUPPORT) && (!defined MMI_WORLDCLOCK_MINI_SUPPORT) && (!defined MMI_COUNTEDTIME_MINI_SUPPORT)
PUBLIC void MMIMAINMENU_StartTimer (uint32 param1, uint32 param2);
#endif


#ifdef HOME_LOCATION_INQUIRE_SUPPORT
PUBLIC void MMIMAINMENU_StartPhoneSearch (uint32 param1, uint32 param2);
#endif


#ifdef IKEYBACKUP_SUPPORT
PUBLIC void MMIMAINMENU_StartKB (uint32 param1, uint32 param2);
#endif


#ifdef MMI_AZAN_SUPPORT
PUBLIC void MMIMAINMENU_StartMuSiLinPray (uint32 param1, uint32 param2);
#endif


#ifdef MV_SUPPORT
PUBLIC void MMIMAINMENU_StartMagicVoice (uint32 param1, uint32 param2);
#endif


#ifdef MMI_TASK_MANAGER
PUBLIC void MMIMAINMENU_StartAppletManager (uint32 param1, uint32 param2);
#endif


#ifdef MMI_MEMO_SUPPORT
PUBLIC void MMIMAINMENU_StartMemo (uint32 param1, uint32 param2);
#endif


#ifdef WEATHER_SUPPORT
PUBLIC void MMIMAINMENU_StartWeather (uint32 param1, uint32 param2);
#endif


#ifdef MCARE_V31_SUPPORT
PUBLIC void MMIMAINMENU_StartMCareV31 (uint32 param1, uint32 param2);
PUBLIC void MMIMAINMENU_StartMCareV31QQBrowser (uint32 param1, uint32 param2);
#endif


#ifdef QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartQQ (uint32 param1, uint32 param2);
#endif


#ifdef MOBILE_VIDEO_SUPPORT
PUBLIC void MMIMAINMENU_StartMV (uint32 param1, uint32 param2);
#endif

#ifdef MRAPP_SUPPORT
PUBLIC void MMIMAINMENU_StartMRAPP (uint32 param1, uint32 param2);
#endif

#ifdef MET_MEX_MERIG_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMex (uint32 param1, uint32 param2);
#endif // MET_MEX_MERIG_SUPPORT


#ifdef MET_MEX_QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexQQ (uint32 param1, uint32 param2);
#endif // MET_MEX_QQ_SUPPORT


#ifdef MET_MEX_ZT_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexZT (uint32 param1, uint32 param2);
#endif // MET_MEX_ZT_SUPPORT


#ifdef MET_MEX_FRUITNINJA_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexFruitNinja (uint32 param1, uint32 param2);
#endif // MET_MEX_FRUITNINJA_SUPPORT


#ifdef MET_MEX_WEIBO_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWeibo (uint32 param1, uint32 param2);
#endif // MET_MEX_WEIBO_SUPPORT


#ifdef MET_MEX_WEATHER_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWeather (uint32 param1, uint32 param2);
#endif  // MET_MEX_WEATHER_SUPPORT


#ifdef MET_MEX_WOMIVIDEO_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexWomiVideo (uint32 param1, uint32 param2);
#endif  //MET_MEX_WOMIVIDEO_SUPPORT


#ifdef MET_MEX_TB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexTB (uint32 param1, uint32 param2);
#endif // MET_MEX_TB_SUPPORT


#ifdef MET_MEX_AQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexAQ (uint32 param1, uint32 param2);
#endif // MET_MEX_AQ_SUPPORT


#ifdef MET_MEX_DOUBLE_QQ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexQQ1 (uint32 param1, uint32 param2);
PUBLIC void MMIMAINMENU_StartMetMexQQ2 (uint32 param1, uint32 param2);
#endif // MET_MEX_DOUBLE_QQ_SUPPORT


#ifdef MET_MEX_MSN_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexMSN (uint32 param1, uint32 param2);
#endif


#ifdef MET_MEX_FETION_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexFetion (uint32 param1, uint32 param2);
#endif


#ifdef MET_MEX_AB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexAB (uint32 param1, uint32 param2);
#endif // MET_MEX_AQ_SUPPORT


#ifdef MET_MEX_CQZ_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexCQZ (uint32 param1, uint32 param2);
#endif


#ifdef MET_MEX_JSCB_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexJSCB (uint32 param1, uint32 param2);
#endif


#ifdef MET_MEX_TOMCAT_SUPPORT
PUBLIC void MMIMAINMENU_StartMetMexTomCat (uint32 param1, uint32 param2);
#endif // MET_MEX_AQ_SUPPORT


#ifdef WRE_SUPPORT
PUBLIC void MMIMAINMENU_StartWRE (uint32 param1, uint32 param2);
#endif


#ifdef GO_MENU_ONE_ICON_CHANGE_STYLE
PUBLIC void MMIMAINMENU_GoMenuChangeStyle (uint32 param1, uint32 param2);
#endif

#ifdef QBTHEME_SUPPORT
PUBLIC void MMIMAINMENU_OpenMainWin(uint32 param1, uint32 param2);

PUBLIC void MMIMANMENU_OpenDownLoadUrl(uint32 param1, uint32 param2);
#endif

PUBLIC void MMIMAINMENU_EnterDialWin(uint32 param1, uint32 param2);

PUBLIC void MMIMAINMENU_OpenListAllGroupWin(uint32 param1, uint32 param2);

#ifdef DL_SUPPORT
PUBLIC void MMIMANMENU_DownloadManager(uint32 param1, uint32 param2);
#endif

PUBLIC void MMIMANMENU_EnterPlayer(uint32 param1, uint32 param2);

PUBLIC void MMIMANMENU_EnterTools(uint32 param1, uint32 param2);

PUBLIC void MMIMANMENU_OpenAlarmMainWin(uint32 param1, uint32 param2);

PUBLIC void MMIMANMENU_EnterSetting(uint32 param1, uint32 param2);

#ifdef BT_DIALER_SUPPORT
PUBLIC void MMIMAINMENU_OpenBtPhoneWin(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_ITEM_FUNC_H_

/*@}*/
