/*************************************************************************
** File Name:      mmi_nv.h                                             *
** Author:         Panda luo                                            *
** Date:           03/14/2003                                           *
** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 03/14/2003     Panda luo        Create.                              *
** 09/16/2003     Steven.niu       cr4775                               *
*************************************************************************/

#ifndef _MMI_NV_H_
#define _MMI_NV_H_

#include "mn_type.h"
#include "mmi_module.h"

#include "nv_item_id.h"
//#include "mmisrv_nv.h"

/*
#ifdef CMCC_UI_STYLE
#define MAX_MMI_NV_USER_ITEM_NUM     3800
#else
#define MAX_MMI_NV_USER_ITEM_NUM     2800
#endif

#ifdef MMI_DUALMODE_ENABLE
#define MMI_NV_USER_BASE     MN_NV_TD_USER_BASE//800  @$discuss 需要根据实际大小修改
#else
#define MMI_NV_USER_BASE     MN_NV_USER_BASE
#endif
  */


// 录音机主留言最大名字长度
#define MMILEAVEWORD_MAX_OWNNAME_LEN          30


typedef struct
{
    uint8      cur_index;
    uint16     ownword[ MMILEAVEWORD_MAX_OWNNAME_LEN ];
}MMINV_LEAVEWORD_ITEM_INFO_T;   //来电留言信息

//The special NV item
typedef enum
{
        //imsi
	MMINV_PHONE_IMSI= MMI_MODULE_COMMON << 16, 
	MMINV_NETWORK_NAME,
#ifdef MMS_SUPPORT
	MMINV_MMS_SETTING,
#endif
//	MMINV_MMS_IS_DELETE_DONE,
//	MMINV_BROWSER_BOOKMARK_INFO,
//	MMINV_BROWSER_SETTING_INFO,
//	MMINV_BROWSER_DOWNLOAD_RING_INFO,
//	MMINV_BROWSER_DOWNLOAD_PICTURE_INFO,
#ifdef BROWSER_SUPPORT
	MMINV_BROWSER_UA,
#endif
#ifdef MMI_GPRS_SUPPORT
        MMINV_CONNECTION_GPRS_SETTINGS,//GPRS Detach
        MMINV_PDP_GPRS_FLOW_RATE,
        MMINV_CONNECTION_SETTING_INFO,
#ifdef ACCOUNT_AUTO_ADAPT_SUPPORT
        MMINV_CONNECTION_CONNECTION_PLMN,//Connection PLMN
        MMINV_CARRIER_PLMN, // record mcc&mnc
        MMINV_SET_APN_TYPE_INDEX,
#endif
    //added by feng.xiao
    #ifdef DATA_ROAMING_SUPPORT	
	    MMINV_CONNECTION_DATA_ROAMING,//Data romaing
    #endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
        MMINV_CONNECTION_GPRS_SWITCH,
#endif        

#endif
#if defined(MMI_MSD_SUPPORT)
	MMINV_MSD_SETTING_INFO,
#endif
	MMINV_DIRECTIONKEY_INFO,    //方向键设置
	MMINV_LEAVEWORD_ITEM_INFO,   //关闭，内置留言一，内置留言二，机主留言
#if 0
	MMINV_MPEG4_PLAY_PARAM,  //Mpeg4播放参数
	MMINV_MPEG4_VOLUME,			// mpeg4 的音量
    MMINV_MPEG4_BRIGHT,          //mpeg4的亮度
#endif
	MMINV_WALLPAPER_BITMAP_INFO, 
    
    // begin main menu nv data 
#ifdef MATRIX_MAIN_MENU_SUPPORT
    // 宫格
    MMINV_ICONMENU_ITEM_NUM,   // 宫格主菜单项数
    MMINV_ICONMENU_ITEM_INFO,  // 宫格主菜单数据
#endif
#ifdef OPTION_PAGE_MAIN_MENU_SUPPORT
    // 选项卡
    MMINV_OPTMENU_1_ITEM_NUM,  // 选项卡第1页项数
    MMINV_OPTMENU_1_ITEM_INFO, // 选项卡第一页数据

    MMINV_OPTMENU_2_ITEM_NUM,  // 选项卡第2页项数
    MMINV_OPTMENU_2_ITEM_INFO, // 选项卡第二页数据

    MMINV_OPTMENU_3_ITEM_NUM,  // 选项卡第3页项数
    MMINV_OPTMENU_3_ITEM_INFO, // 选项卡第三页数据

    MMINV_OPTMENU_4_ITEM_NUM,  // 选项卡第4页项数
    MMINV_OPTMENU_4_ITEM_INFO, // 选项卡第四页数据
#endif

    // 滑动页面
#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)
    MMINV_SLIDEMENU_CUR_PAGEINDEX,
    MMINV_SLIDEMENU_VALID_PAGENUM,
#endif
#if defined(MMI_PDA_SUPPORT) && defined(MMI_ISTYLE_SUPPORT)
    // istyle
    MMINV_MAINMENU_ISTYLE_PAGES,
    MMINV_MAINMENU_ISTYLE_BAR,
    MMINV_MAINMENU_ISTYLE_FOLDERS,
#endif

#ifdef QBTHEME_SUPPORT
    MMINV_QBTHEME_DOCKBAR_INFO,
#endif

// end main menu nv data, 请不要在这之间加入其他数据
#ifdef MMI_AZAN_SUPPORT
	MMINV_AZAN_ALARM,
	MMINV_AZAN_COUNTRY_ID,    // Country id
	MMINV_AZAN_CITY_ID,       // City id
	MMINV_AZAN_CAL_METHOD_ID, // Calculation method
	MMINV_AZAN_STATUS_ID,     // azan status:on/off
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
        MMINV_HPLMN_INFO,
#endif
#ifdef MMI_RESET_PHONE_AUTO_PIN
        MMINV_SIM_PIN_VALUE,
#endif
#ifdef BT_DIALER_SUPPORT
        MMINV_BT_ADDRESS_INFO,    //connected device address
        MMINV_BT_IS_BTMODE,       //is btmode on
#endif

        MMINV_SET_VOLTE,
#ifdef BLUETOOTH_SUPPORT
        WACHT_BT_ONOFF,//BT 总开关状态(包括classic和ble)
#ifdef BBM_ROLE_SUPPORT
        WATCH_BOUND_DEVICE_INFO,//绑定远端设备信息，包括名称，ble地址，classic地址
        WATCHBBM_MODE,//BBM mode,auto/slaver/ble/dialer/phone
#endif

#ifdef ADULT_WATCH_SUPPORT
        WATCH_BT_LOCAL_NAME,//BLE local name
#endif
#endif

	MMI_NV_MAX_ITEM_NUM,
//	MMI_NV_MAX_NUM = MMI_NV_USER_BASE + MAX_MMI_NV_USER_ITEM_NUM

}MMI_NV_ITEM_E;

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
			 MMI_WriteNVItem(_item_id, _addr)

/*****************************************************************************/
// 	Description : register common module nv len and  max item
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMI_RegCommonNv(void);

#endif
