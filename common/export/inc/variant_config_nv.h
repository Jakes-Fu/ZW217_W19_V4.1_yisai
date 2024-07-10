/*************************************************************************
** File Name:      variant_config_nv.h                                   *
** Author:         Mingwen Ge                                            *
** Date:           05/11/2015                                            *
** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about nv              *
**************************************************************************
**************************************************************************
**                        Edit History                                   *
** ----------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                           *
** 05/11/2015     Mingwen.Ge        Create.                              *
**                                                                       *
*************************************************************************/

#ifndef _CONFIG_NV_H_
#define _CONFIG_NV_H_

//#include "mn_type.h"
//#include "mmi_module.h"

//#include "nv_item_id.h"

/*NV_VARIANT_CONFIG_TELE nv items*/

//config nv base id
#define VARIANT_NV_CONFIG_USER_BASE 1900

//The config NV item id
typedef enum
{
    //TELE module
    TELE_NV_DIAL2_PROMPT= VARIANT_NV_CONFIG_USER_BASE,
    TELE_NV_LIFE_TIMER,
    //TELE_NV_SHOW_CALL_TIME,
    TELE_NV_CALL_CFG,
    TELE_NV_CLI_NUMBER,
    TELE_NV_ENABLE_SPEED_DIAL,
    TELE_NV_FAST_DORMANCY,
    TELE_NV_OPERATOR_SHOW_ORDER,
    //TELE_NV_MNVO_SHOW_PNN,
    //TELE_NV_LARGE_DIALING_FONT,
    TELE_NV_SET_CPHS_ON,
    //TELE_NV_TELE_CFG,
    TELE_NV_CELL_BROADCAST,
    TELE_NV_CELL_BROADCAST_CHANNEL,
    //TELE_NV_CALL_SUMMARY_ENABLE,
    TEL_NV_L2_FILL_BIT_VAMOS,
    TEL_NV_OPERATOR_PAY_SET,
    TEL_NV_NETWORK_TYPE_SELECT,
    TEL_NV_CELL_INFO_DISPLAY_ENABLE,
    TELE_NV_SMSCB_MCC_SET,
    TELE_NV_RESERVED_ITEM,
    //MESSAGES module
    //MESSAGES_NV_SMSC_CONFIG_BY_USER = 1920,//NV module first item id value
    MESSAGES_NV_SMS_DELIVERY_REPORT= 1920,//NV module first item id value
    MESSAGES_NV_SMS_CFG,
    MESSAGES_NV_MMS_CONFIG,
    MESSAGES_NV_TIME_STAMP_TYPE,
    MESSAGES_NV_MADVERTISE_ON,
    //MESSAGES_NV_FULL_SMS_CHAR,
    //MESSAGES_NV_LARGE_MSG_FONT,
    MESSAGES_NV_MMS_DELIVERY_REPORT,
    MESSAGES_NV_SMSC_EDITABLE,
    MESSAGES_NV_RESERVED_ITEM,
    //APP module
    APP_NV_BT_CONFIG = 1940,//NV module first item id value
    APP_NV_CAMERA_SHOT_SOUND,
    APP_NV_CAMERA_RECORD_INDICATOR,
    APP_NV_EMERGENCY_CALL_UI,
    APP_NV_FM_RECORD_CFG,
    APP_NV_FM_TUNNING_STEP,
    APP_NV_DEFAULT_BROWSER_APP,
    APP_NV_DISABLE_SHARE_VIA_OPTION,
    APP_NV_LEFT_SOFTKEY_APP,
    APP_NV_RIGHT_SOFTKEY_APP,
    APP_NV_DEFAULT_THEME_COLOR,
    APP_NV_BT_PROFILE_LIST,
    APP_NV_SHOW_UNINSTALL_PRELOAD_GAME,
    APP_NV_RESERVED_NV_ITEM,
    //SETTINGS module
    SETTINGS_NV_INPUT_T9_ON = 1960,//NV module first item id value
    SETTINGS_NV_TIME_FORMAT_TYPE,
    SETTINGS_NV_TIME_NITZ_ON,
    SETTINGS_NV_NDT_VERSION,
    SETTINGS_NV_SHOW_INDIA_SAR,
    SETTINGS_NV_DEFAULT_SIM_ECC,
    SETTINGS_NV_DEFAULT_NOSIM_ECC,
    SETTINGS_NV_DEFAULT_VOICEMAIL,
    SETTINGS_NV_DEFAULT_HOMEPAGE,
    SETTINGS_NV_DEFAULT_BOOKMARK,
    SETTINGS_NV_DEFAULT_LANGUAGE,  
    SETTINGS_NV_CIPHERING_A5,
    SETTINGS_NV_CIPHERING_GEA,
    SETTINGS_NV_IP_PORTOCOL_SELECTION,
    SETTINGS_NV_ENABLE_DOMESTIC_ROAMING,
    SETTINGS_NV_PHONEBOOK_SORT_BY_SURNAME,
    SETTINGS_NV_STATUSBAR_SHOW_3DOT5G_ICON,
    SETTINGS_NV_NAS_FEATURE_FLAG,
    SETTINGS_NV_DEFAULT_FAKE_ECC,
    SETTINGS_NV_RESERVED_ITEM,
    OTHER_NV_MENU_TREE_LIST = 1980,//NV module first item id value
    OTHER_NV_PRODUCT_TYPE,
    OTHER_NV_OPERATOR_LOGO,//operator logo
    OTHER_NV_SOFTWARE_VERSION,
    OTHER_NV_4G_ICON_DISPLAY_STYLE,
    OTHER_NV_VOLTE_ICON_DISPLAY_STYLE,
    OTHER_NV_VOLTE_ICON_DISPLAY_SET,
    OTHER_NV_VOLTE_MENU_DISPLAY_SET ,
    OTHER_NV_NETWORK_MENU_DISPLAY_STYLE ,
    SETTINGS_NV_INDIAN_SOS_SUPPORT,
    OTHER_NV_ENABLE_MOBILE_DATA,
    OTHER_NV_NETWORK_OPN_UPDATE,
    OTHER_NV_RESERVED_ITEM,

	CONFIG_NV_MAX_ITEM_NUM,
}NDT_NV_ITEM_E;


#endif
