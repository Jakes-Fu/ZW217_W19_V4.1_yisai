/*************************************************************************
 ** File Name:      mmi_nv.c                                             *
 ** Author:         Great.Tian                                           *
 ** Date:           01/28/2004                                           *
 ** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 01/28/2004     Panda luo        Create.                              *
*************************************************************************/
#define _MMI_NV_C

//#include "std_header.h"
#include "mmi_nv.h"
#include "mmimms_export.h"
#include "mmiconnection_export.h"
#if defined(MMI_MSD_SUPPORT)
#include "mmimsd_export.h"
#endif
#include "mmi_modu_main.h"
#include "mmi_module.h"
//#include "mmiidle_screensaver.h"
#include "mmi_wallpaper_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmipdp_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef MMI_AZAN_SUPPORT
#include "mmiazan_export.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
#include "mmi_mainmenu_istyle.h"
#endif

#ifdef BT_DIALER_SUPPORT
#include "mmibt_export.h"
#endif
#ifdef BLUETOOTH_SUPPORT
#ifdef BBM_ROLE_SUPPORT
#include "bbm_export.h"
#endif
#include "ual_bt.h"
#endif

#ifdef ADULT_WATCH_SUPPORT
#include "adultwatch_bt_name_win.h"
#endif

//#define MMI_NVITEM_NUM  (MMI_NV_MAX_NUM - MMI_UINV_USER_BASE)   // the number of the mmi nv item

const uint16 mmi_nv_len[] =
{
    //imsi
    sizeof(MN_IMSI_T)*MMI_DUAL_SYS_MAX,              //MMINV_PHONE_IMSI
    sizeof(MMIPHONE_NV_NETWORK_NAME_T)*MMI_DUAL_SYS_MAX,              //MMINV_NETWORK_NAME
#ifdef MMS_SUPPORT
    //mms
    sizeof(MMIMMS_NV_SETTING_T),              //MMINV_MMS_SETTING
#endif

//    sizeof(BOOLEAN),                        //MMINV_MMS_IS_DELETE_DONE, (boolean)


    //browser nv item start add by figo.feng 2004.09.14  
//    sizeof(MMICMSBRW_BOOKMARK_T),
//    sizeof(MMICMSBRW_SETTING_T),
//    1,
//    1,
#ifdef BROWSER_SUPPORT
    sizeof(MMIBROWSER_UA_T),
#endif
#ifdef MMI_GPRS_SUPPORT
    sizeof(MMICONNECTION_GPRS_SETTINGS_E),
    MMI_DUAL_SYS_MAX * sizeof(MMIPDP_FLOWRATE_T),
    //browser nv item end
    
    //connection start
    sizeof(MMICONNECTION_SETTING_T),
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
    sizeof(PHONE_PLMN_NAME_E)*MMI_DUAL_SYS_MAX,//MMINV_CONNECTION_CONNECTION_PLMN
    sizeof(MN_PLMN_T)*MMI_DUAL_SYS_MAX, //MMINV_CARRIER_PLMN replace MMINV_CONNECTION_CONNECTION_PLMN
    sizeof(MMICONNECTION_APNTYPEINDEX_T), // MMINV_SET_APN_TYPE_INDEX
#endif
    //connection end
    //added by feng.xiao
    #ifdef DATA_ROAMING_SUPPORT
        sizeof(MMICONNECTION_SETTING_DATA_ROAMING_E),//MMINV_CONNECTION_DATA_ROAMING
    #endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
        sizeof(MMICONNECTION_SETTING_GPRS_SWITCH_E),
#endif

#endif
#if defined(MMI_MSD_SUPPORT)
    //MSD START
    sizeof(MMIMSD_NV_INFO_T),
    //MSD END   
#endif
    sizeof(MMISET_DIRECTIONKEY_SHORTCUT_T),  //方向键设置
    sizeof(MMINV_LEAVEWORD_ITEM_INFO_T),    //来电留言信息
    
#if 0
    1,  //MMINV_MPEG4_PLAY_PARAM
    1,  //MMINV_MPEG4_VOLUME
    1,  //MMINV_MPEG4_BRIGHT
#endif
    
    sizeof(MMIIDLE_WALLPAPER_WIDTH_HEIGHT_T),//MMINV_WALLPAPER_BITMAP_INFO

    // begin main menu nv data 

#ifdef MATRIX_MAIN_MENU_SUPPORT
    // 宫格主菜单
    2,                                                    // MMINV_ICONMENU_ITEM_NUM    宫格主菜单项数
    MMIMENU_MAIN_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_ICONMENU_ITEM_INFO   宫格主菜单数据
#endif
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
    // 选项卡主菜单
    2,                                                    // MMINV_OPTMENU_1_ITEM_NUM   选项卡第1页项数
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_1_ITEM_INFO  选项卡第一页数据
    2,                                                    // MMINV_OPTMENU_2_ITEM_NUM   选项卡第2页项数
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_2_ITEM_INFO  选项卡第二页数据
    2,                                                    // MMINV_OPTMENU_3_ITEM_NUM   选项卡第3页项数
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_3_ITEM_INFO  选项卡第三页数据
    2,                                                    // MMINV_OPTMENU_4_ITEM_NUM   选项卡第4页项数
    MMITHEME_MENUMAIN_SLIDE_ITEM_MAX_NUM*sizeof(MMIMAINMENU_ITEM_INFO_T), // MMINV_OPTMENU_4_ITEM_INFO  选项卡第四页数据
#endif

    // 滑动页面主菜单
#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
    2,                                                                      // MMINV_SLIDEMENU_CUR_PAGEINDEX,当前页
    2,                                                                      // MMINV_SLIDEMENU_VALID_PAGENUM
#endif

#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
//size
    //istyle
    sizeof(ISTYLE_PAGES_INFO_T),//MMINV_MAINMENU_ISTYLE_PAGES
    sizeof(ISTYLE_BAR_INFO_T),//MMINV_MAINMENU_ISTYLE_BAR
    sizeof(ISTYLE_FOLDERS_INFO_T),//MMINV_MAINMENU_ISTYLE_FOLDERS
#endif

#ifdef QBTHEME_SUPPORT
    sizeof(MMIMAINMENU_PAGE_ITEM_INFO_T),               // MMINV_QBTHEME_DOCKBAR_INFO
#endif
    // end main menu nv data, 请不要在这之间加入其他数据
#ifdef MMI_AZAN_SUPPORT
	sizeof(AZAN_ALARM_T), //MMINV_AZAN_ALARM
	sizeof(uint16),       //MMINV_AZAN_COUNTRY_ID,    // Country id
	sizeof(uint16),       //MMINV_AZAN_CITY_ID,       // City id
	sizeof(uint16),        //MMINV_AZAN_CAL_METHOD_ID, // Calculation method
	sizeof(BOOLEAN),      //MMINV_AZAN_STATUS_ID
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    sizeof(MN_PLMN_T)*MMI_DUAL_SYS_MAX, //MMINV_HPLMN_INFO
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
    sizeof(MN_BLOCK_T) * MMI_DUAL_SYS_MAX, // MMINV_SIM_PIN_VALUE,
#endif
#ifdef BT_DIALER_SUPPORT
    sizeof(BT_ADDRESS),        //MMINV_BT_ADDRESS_INFO,    //connected device address
    sizeof(BOOLEAN),           //MMINV_BT_IS_BTMODE,       //is btmode on
#endif
    sizeof(uint8),           //MMINV_SET_VOLTE
    // PHONEBOOK
#ifdef BLUETOOTH_SUPPORT
    sizeof(BOOLEAN), //WACHT_BT_ONOFF,
#ifdef BBM_ROLE_SUPPORT
    sizeof(BBM_BOUND_REMOTE_DEV_INFO_T),//WATCH_BOUND_DEVICE_INFO,
    sizeof(BBM_MODE_E),//WATCHBBM_MODE,
#endif
#ifdef ADULT_WATCH_SUPPORT
    sizeof(wchar) * BT_LOCAL_NAME_LEN,//WATCH_BT_LOCAL_NAME
#endif
#endif

};

/*****************************************************************************/
//  Description : register common module nv len and  max item
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMI_RegCommonNv(void)
{    
    MMI_RegModuleNv(MMI_MODULE_COMMON, mmi_nv_len, sizeof(mmi_nv_len)/sizeof(uint16));
}
