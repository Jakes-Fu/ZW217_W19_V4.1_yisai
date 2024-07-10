/***************************************************************************
** File Name:      mmidl_internal.h                                                                               *
** Author:                                                                                                                    *
** Date:           2012/02/03                                                                                          *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.                             *
** Description:    This file is used to connection internal define                                      *
****************************************************************************
**                         Important Edit History                                                                       *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                  *
** 02/2012        dave.ruan		   Create
****************************************************************************/
#ifndef  _MMICONNECTION_INTERNAL_H_    
#define  _MMICONNECTION_INTERNAL_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
#ifdef MMI_GPRS_SUPPORT
    MMI_CONNECTION_MENU_LIST_GPRS_NET_USERID,   //ID_CONNECTION_NET_USERID
#endif    
#ifdef MMI_GPRS_SWITCH_SUPPORT
    MMI_CONNECTION_MENU_LIST_GPRS_SWITCH,       //ID_CONNECTION_GRPS_SWITCH
#endif
#ifdef DATA_ROAMING_SUPPORT
    MMI_CONNECTION_MENU_LIST_GPRS_ROAMING,      //ID_CONNECTION_DATA_ROAMING
#endif
#ifdef MMI_GPRS_SUPPORT
    MMI_CONNECTION_MENU_LIST_GPRS_MODESET,       //ID_CONNECTION_GPRS
#endif                                                
#ifdef  MMI_DUALMODE_ENABLE
    MMI_CONNECTION_MENU_LIST_NETSETTING,        //ID_CONNECTION_SET_NETSETTING
#else
    MMI_CONNECTION_MENU_LIST_NETWORK,           //ID_CONNECTION_SET_NETWORK
#endif   
    MMI_CONNECTION_MENU_LIST_NETWORK_TYPE,      //ID_CONNECTION_SET_NETWORK_TYPE
#ifdef MMI_PREFER_PLMN_SUPPORT
    MMI_CONNECTION_MENU_LIST_GPRS_PREFER_NETWORK,   //ID_CONNECTION_SET_PREFER_NETWORK
#endif

#ifdef MMI_GPRS_DATA_SIM_SET
    MMI_CONNECTION_MENU_LIST_GPRS_MODE,
#endif
#ifdef MMI_VOLTE_SUPPORT
    MMI_CONNECTION_MENU_LIST_VOLTE,           //ID_CONNECTION_SET_VOLTE
#endif
    MMI_CONNECTION_MENU_LIST_TYPE_MAX
}MMI_CONNECTION_MAIN_LIST_TYPE_E;
 


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : init connection
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_Init(void);

/*****************************************************************************/
// 	Description : 连接列表重置
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_ResetSettingList(BOOLEAN is_clean_user_data);

/*****************************************************************************/
// 	Description : 连接列表重置
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_ResetSettingListEx(MN_DUAL_SYS_E dual_sys, BOOLEAN is_clean_user_data);

#ifdef MMI_GPRS_SUPPORT
/*****************************************************************************/
//  Description : Auto Adapting Network account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_AutoAdapting(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Auto Adapting specified account
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_AdaptingAccount(MN_DUAL_SYS_E dual_sys, MMICONNECTION_ACCOUNT_TYPE_E account_type);

/*****************************************************************************/
// 	Description : create Linklist Win for mms,browser,dcd,java
//	Global resource dependence : none
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMICONNECTION_CreateLinklistWin(ADD_DATA param, MMICONNECTION_CALLBACK_FUNC callback_func);

/*****************************************************************************/
//  Description : Get Chinese Net Name Text Id
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:CMCC and CUCC
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMICONNECTION_GetChineseNetNameTextId(const wchar *resource_name, uint16 name_len);

#ifdef DERECT_LINK_FIRST_SUPPORT
/*****************************************************************************/
//  Description : Set Derect Link Index First
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:change the derect link detail as first account if not
/*****************************************************************************/
PUBLIC BOOLEAN MMICONNECTION_SetDerectLinkToFirstIndex(MMICONNECTION_SETTING_T *connection_setting);
#endif
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
#if defined(BROWSER_SUPPORT)
/*******************************************************************/
//  Interface:	MMICONNECTION_OpenNetAccountDownLoadUrl
//  Description : 	mmi connection Open Net account  DownLoad Url
//  Global resource dependence : 
//  Author:dave.ruan
//  Note:
/*******************************************************************/
PUBLIC void MMICONNECTION_OpenNetAccountDownLoadUrl(void);

/*******************************************************************/
//  Description : 	MMICONNECTION_RegBrowserCB
//  Global resource dependence : 
//  Author: dave.ruan
//  Note:
/*******************************************************************/
PUBLIC void MMICONNECTION_RegBrowserCB(void);
#endif

/*******************************************************************/
//  Description : 	MMICONNECTION_RefreshSettingList
//  Global resource dependence : 
//  Author: dave.ruan
//  Note: refresh  setting list after load list
/*******************************************************************/
PUBLIC void MMICONNECTION_RefreshSettingList(void);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif/*_MMICONNECTION_INTERNAL_H_*/


