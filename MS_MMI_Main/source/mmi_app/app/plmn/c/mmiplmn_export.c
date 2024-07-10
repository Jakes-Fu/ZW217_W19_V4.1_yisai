/*****************************************************************************
** File Name:      mmiplmn_export.c                                                *
** Author:                                                                   *
** Date:           09/2011                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe PLMN API module                *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2011       wancan.you       Create
******************************************************************************/

#define _MMIPLMN_EXPORT_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiplmn_internal.h"
#include "mmiplmn_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Is Same plmn
//  Global resource dependence : none
//  Author: wancan.you
//  Note:cmp mcc and network name
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPLMN_IsSamePlmn(const MN_PLMN_T *plmn1, const MN_PLMN_T *plmn2)
{
    return MMIPLMN_IsSamePlmn(plmn1, plmn2);
}

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
//  Description : Load Local Default Config
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPLMN_LoadLocalDefaultConfig(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_LoadLocalDefaultConfig(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local Bookmark
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_BOOKMARK_ID_T MMIAPIPLMN_GetLocalBookmark(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalBookmark(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local Network Account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_NETWORK_ACC_ID_T MMIAPIPLMN_GetLocalNetworkAccount(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalNetworkAccount(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local Emergency Call Number
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system; MAX NUM:MMIPLMN_EMERGENCY_CALL_MAX
/*****************************************************************************/
PUBLIC const MMIPLMN_EMERGENCY_CALL_T *MMIAPIPLMN_GetLocalEmergency(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalEmergency(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local Keyboard Shortcuts
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_KEYBOARD_SHORTCUTS_T *MMIAPIPLMN_GetLocalKeyboardShortcuts(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalKeyboardShortcuts(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local Language settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_LANGUAGE_T *MMIAPIPLMN_GetLocalLanguageSettings(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalLanguageSettings(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local SMS settings
//  Global resource dependence : none
//  Author: wancan.you
//  Note:PNULL:Use default by system;
/*****************************************************************************/
PUBLIC const MMIPLMN_SMS_T *MMIAPIPLMN_GetLocalSMSSettings(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalSMSSettings(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local PDP Time out
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:ms)
/*****************************************************************************/
PUBLIC const MMIPLMN_PDP_TIMEOUT_T MMIAPIPLMN_GetLocalPDPTimeout(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalPDPTimeout(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local MMS Limit
//  Global resource dependence : none
//  Author: wancan.you
//  Note:NULL:Use default by system;(uint:Bytes)
/*****************************************************************************/
PUBLIC const MMIPLMN_MMS_LIMIT_T MMIAPIPLMN_GetLocalMMSLimit(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalMMSLimit(dual_sys);
}

/*****************************************************************************/
//  Description : Get Local entermainment shortcuts 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:PNULL:default donot show this menu;(uint:points)
/*****************************************************************************/
PUBLIC const MMIPLMN_SHORTCUTS_T *MMIAPIPLMN_GetLocalShortcuts(MN_DUAL_SYS_E dual_sys)
{
    return MMIPLMN_GetLocalShortcuts(dual_sys);
}
#endif
