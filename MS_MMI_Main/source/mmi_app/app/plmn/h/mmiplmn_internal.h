/*****************************************************************************
** File Name:      mmiplmn_internal.h                                                *
** Author:                                                                   *
** Date:           09/2011                                                   *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe plmn internal use               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       wancan.you       Create
******************************************************************************/
#ifndef _MMIPLMN_INTERNAL_H_
#define _MMIPLMN_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "mmiplmn_export.h"

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

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Is Same Network Plmn
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:cmp mcc and mnc
/*****************************************************************************/
PUBLIC BOOLEAN MMIPLMN_IsSamePlmn(const MN_PLMN_T *plmn1, const MN_PLMN_T *plmn2);

/*****************************************************************************/
//  Description : Load Local Default Config
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPLMN_LoadLocalDefaultConfig(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Bookmark
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_BOOKMARK_ID_T MMIPLMN_GetLocalBookmark(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Network Account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_NETWORK_ACC_ID_T MMIPLMN_GetLocalNetworkAccount(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Emergency Call Number
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system; MAX NUM:MMIPLMN_EMERGENCY_CALL_MAX
/*****************************************************************************/
PUBLIC const MMIPLMN_EMERGENCY_CALL_T *MMIPLMN_GetLocalEmergency(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Keyboard Shortcuts
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_KEYBOARD_SHORTCUTS_T *MMIPLMN_GetLocalKeyboardShortcuts(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local Language settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_LANGUAGE_T *MMIPLMN_GetLocalLanguageSettings(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local SMS settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_SMS_T *MMIPLMN_GetLocalSMSSettings(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local PDP Time out
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:ms)
/*****************************************************************************/
PUBLIC const MMIPLMN_PDP_TIMEOUT_T MMIPLMN_GetLocalPDPTimeout(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local MMS Limit
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_MMS_LIMIT_T MMIPLMN_GetLocalMMSLimit(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get Local entertainment shortcut menus
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_SHORTCUTS_T *MMIPLMN_GetLocalShortcuts(MN_DUAL_SYS_E dual_sys);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif


