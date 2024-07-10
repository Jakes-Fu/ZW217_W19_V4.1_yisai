/*****************************************************************************
** File Name:      mmiplmn_export.h                                                *
** Author:                                                                   *
** Date:           09/2011                                                   *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe plmn api               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       wancan.you       Create
******************************************************************************/
#ifndef _MMIPLMN_EXPORT_H_
#define _MMIPLMN_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmiset_export.h"
#include "mmisms_app.h"
#include "mmisms_export.h"
#include "mmisms_nv.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIPLMN_EMERGENCY_CALL_MAX  10
#define MMIPLMN_KEYBOARD_SHORTCUTS_MAX  2

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef uint32 MMIPLMN_BOOKMARK_ID_T;   //bookmark

typedef uint32 MMIPLMN_NETWORK_ACC_ID_T;    //network account

typedef struct _MMIPLMN_EMERGENCY_CALL_T_
{
    const char *emergency_call_ptr;
}MMIPLMN_EMERGENCY_CALL_T;//Emergency call info

typedef void (*MMIPLMN_KEYBOARD_SHORTCUTS_FUNC)(MMI_MESSAGE_ID_E msg_id);

typedef struct _MMIPLMN_KEYBOARD_SHORTCUTS_T_
{
    MMI_MESSAGE_ID_E msg_id[MMIPLMN_KEYBOARD_SHORTCUTS_MAX];//Normally msg_id[0]:voice mail;
    MMIPLMN_KEYBOARD_SHORTCUTS_FUNC keyboard_shorcut_func;
}MMIPLMN_KEYBOARD_SHORTCUTS_T;//Keyboard shorcuts

typedef struct _MMIPLMN_LANGUAGE_T_
{
    MMISET_LANGUAGE_TYPE_E language_id;//language id
}MMIPLMN_LANGUAGE_T;//language setting

typedef struct _MMIPLMN_SMS_T_
{
    char *smsc_number;//smsc number. if PNULL will use default setting
    MMINV_SMS_VALID_PERIOD_T validity_period;//validity period.default to:MMISMS_VALID_PERIOD_DEFAULT
    MMISMS_PREFERRED_PATH_E preferred_bearer;//preferred bearer.default to:MMISMS_PREFERRED_PATH_GSM_PREFERRED
    MMISMS_ALPHABET_MANAGEMENT_E character_encoding;//character encoding. default to:MMISMS_ALPHABET_MANAGEMENT_GSM_ALPHABET
}MMIPLMN_SMS_T;//sms setting

typedef uint32 MMIPLMN_PDP_TIMEOUT_T;//pdp time out(unit:ms)

typedef uint32 MMIPLMN_MMS_LIMIT_T;//mms limit(unit:Bytes)

typedef struct _MMIPLMN_SHORTCUT_DETAIL_T_
{
    char *shortcut_name;
    char *shortcut_url;
}MMIPLMN_SHORTCUT_DETAIL_T;//shortcut format is name - url

typedef struct _MMIPLMN_SHORTCUTS_T_
{
    MMIPLMN_SHORTCUT_DETAIL_T *picture_detail;//picture download addr. if PNULL will not show this menu
    MMIPLMN_SHORTCUT_DETAIL_T *sound_detail; //sound download addr. if PNULL will not show this menu
    MMIPLMN_SHORTCUT_DETAIL_T *video_detail; //video download addr. if PNULL will not show this menu
    MMIPLMN_SHORTCUT_DETAIL_T *game_detail; //game download addr. if PNULL will not show this menu
    MMIPLMN_SHORTCUT_DETAIL_T *ringtone_detail; //ringtone download addr. if PNULL will not show this menu
    MMIPLMN_SHORTCUT_DETAIL_T *app_detail; //app store download addr. if PNULL will not show this menu
}MMIPLMN_SHORTCUTS_T;//Lists shortcuts that appear in different menus on the device. The used format is location / name - URL

typedef struct _MMIPLMN_CONFIG_INFO_T_
{
    const MN_PLMN_T plmn;
    const MMIPLMN_NETWORK_ACC_ID_T network_account;   //define in resource, if NULL will use default setting
    const MMIPLMN_BOOKMARK_ID_T bookmark; //define in resource. if NULL will use default setting
    const MMIPLMN_EMERGENCY_CALL_T *emergency_call_list; //define emergency call by local setting, Max:MMIPLMN_EMERGENCY_CALL_MAX. if PNULL will use default setting
    const MMIPLMN_KEYBOARD_SHORTCUTS_T *keyboard_shortcuts_list;//define keyboard shortcuts by local setting. if PNULL will use default setting
    const MMIPLMN_LANGUAGE_T *language_settings;//define language settings by local setting. if PNULL will use default setting
    const MMIPLMN_SMS_T *sms_settings;//define sms setting. if PNULL will use default setting
    const MMIPLMN_PDP_TIMEOUT_T pdp_timeout;//define pdp timeout. if NULL will use default setting(unit:ms)
    const MMIPLMN_MMS_LIMIT_T mms_limit;//define mms limit. if NULL will use default setting(unit:Bytes)
    const MMIPLMN_SHORTCUTS_T* entertainment_shortcuts;
}MMIPLMN_CONFIG_INFO_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Is Same plmn
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cmp mcc and network name
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPLMN_IsSamePlmn(const MN_PLMN_T *plmn1, const MN_PLMN_T *plmn2);

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//  Description : Load Local Default Config
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPLMN_LoadLocalDefaultConfig(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Bookmark
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_BOOKMARK_ID_T MMIAPIPLMN_GetLocalBookmark(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Network Account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_NETWORK_ACC_ID_T MMIAPIPLMN_GetLocalNetworkAccount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Emergency Call Number
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system; MAX NUM:MMIPLMN_EMERGENCY_CALL_MAX
/*****************************************************************************/
PUBLIC const MMIPLMN_EMERGENCY_CALL_T *MMIAPIPLMN_GetLocalEmergency(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Keyboard Shortcuts
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_KEYBOARD_SHORTCUTS_T *MMIAPIPLMN_GetLocalKeyboardShortcuts(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Language settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_LANGUAGE_T *MMIAPIPLMN_GetLocalLanguageSettings(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local SMS settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_SMS_T *MMIAPIPLMN_GetLocalSMSSettings(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local PDP Time out
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:ms)
/*****************************************************************************/
PUBLIC const MMIPLMN_PDP_TIMEOUT_T MMIAPIPLMN_GetLocalPDPTimeout(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local MMS Limit
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_MMS_LIMIT_T MMIAPIPLMN_GetLocalMMSLimit(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local entermainment shortcuts 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:PNULL:default donot show this menu;(uint:points)
/*****************************************************************************/
PUBLIC const MMIPLMN_SHORTCUTS_T *MMIAPIPLMN_GetLocalShortcuts(MN_DUAL_SYS_E dual_sys);

#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif

