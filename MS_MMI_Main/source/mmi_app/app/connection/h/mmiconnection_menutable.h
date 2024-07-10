/*****************************************************************************
** File Name:      mmiconnection_menutable.h                                 *
** Author:         ryan.xu													 *
** Date:           2007/1/31                                                 *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe connection menu id          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/1/31      ryan.xu	       Create                                    *
******************************************************************************/

#ifndef _MMICONNECTION_MENUTABLE_H_
#define _MMICONNECTION_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
    ID_CONNECTION_MENU_START = (MMI_MODULE_CONNECTION << 16),
    ID_CONNECTION_BLUETOOTH,
    ID_CONNECTION_OPEN_TV_OUT,
    ID_CONNECTION_TV_OUT_SETTING,
    ID_CONNECTION_NET_USERID,
#ifdef DATA_ROAMING_SUPPORT
    ID_CONNECTION_DATA_ROAMING,//added by feng.xiao
#endif
#ifdef MMI_GPRS_SWITCH_SUPPORT
    ID_CONNECTION_GRPS_SWITCH,
#endif    
    ID_CONNECTION_GPRS,
    ID_CONNECTION_SET_NETWORK,
    ID_CONNECTION_SET_NETSETTING,
    ID_CONNECTION_SET_NETWORK_TYPE,
#ifdef MMI_VOLTE_SUPPORT
    ID_CONNECTION_SET_VOLTE,
#endif
#ifdef MMI_PREFER_PLMN_SUPPORT
    ID_CONNECTION_SET_PREFER_NETWORK,
#endif
    ID_CONNECTION_SETTING_SETGBUILD,
#if 0  //bug185707
    ID_CONNECTION_SETTING_ADDLINKSETTING,
#endif
    ID_CONNECTION_SETTING_REASERCH,
    ID_CONNECTION_SETTING_DELETE,
    ID_CONNECTION_SETTING_EDIT,
    ID_CONNECTION_SETTING_GPRS,

    ID_CONNECTION_SETTING_SECRET,
    ID_CONNECTION_SETTING_USERNAME,
    ID_CONNECTION_SETTING_PASSWORD,
    ID_CONNECTION_SETTING_APN,
    ID_CONNECTION_SETTING_GATEWAY,
    ID_CONNECTION_SETTING_PORT,
    ID_CONNECTION_SETTING_CHANGENAME,
    ID_CONNECTION_SETTING_SETHOMEPAGE,
    ID_CONNECTION_SETTING_ADVANCE,

    ID_CONNECTION_SETTING_DNS,
    ID_CONNECTION_SETTING_NETTYPE,
#ifdef IPVERSION_SUPPORT_V4_V6
    ID_CONNECTION_SETTING_IPTYPE,
#endif
    
#ifdef MMI_WIFI_SUPPORT
    ID_CONNECTION_WIFI,
#endif

	ID_CONNECTION_SETTING_GPRS_DETACH,
	ID_CONNECTION_SETTING_GPRS_ALWAYS,
    ID_CONNECTION_SETTING_DOWNLOAD_RES,
	
    
	ID_CONNECTION_MENU_MAX
}MMICONNECTION_ID_MENU_E;
		
		
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMICONNECTION_MENU_LABEL_START = (MMI_MODULE_CONNECTION << 16),

    #include "mmiconnection_menutable.def"

    MMICONNECTION_MENUTABLE_MAX
} MMICONNECTION_MENU_LABEL_E;

#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMISMS_MENUTABLE_H_

