/***************************************************************************************
** FileName    :  atest_ota.h                                                         **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  14/05/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The step and action of the ota testing is defined in this file.     **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  14/05/2010     Mingxuan.Zhu         Created                                       **
**  08/08/2010     Mingxuan.Zhu         Updated                                       **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifndef _ATEST_OTA_H
#define _ATEST_OTA_H

/**-----------------------------------------------------------------------------------*
 **                                   Dependencies                                    *
 **-----------------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mn_type.h"
#include "mmisms_export.h"
#include "mmiconnection_export.h"
#include "atest_key_step.h"

/**-----------------------------------------------------------------------------------*
 **                                Macro Declarations                                 *
 **-----------------------------------------------------------------------------------*/
#define PRESS_KEY_NUM_OF_ENTER_IDLE          4          // the size of the press key step for entering idle interface
#define PRESS_KEY_NUM_OF_RECEIVE_OTA         2          // the size of the press key step for quitting pop dialog
#define PRESS_KEY_NUM_OF_CONFIG_OTA          6          // the size of the press key step for config ota information
#define PRESS_KEY_NUM_OF_DELETE_IA           22         // the size of the press key step for deleting ota config from Internet account
#define PRESS_KEY_NUM_OF_DELETE_OTA          11         // the size of the press key step for deleting ota config from Inbox
#define PRESS_KEY_NUM_OF_GET_OTA_INFO        25         // the size of the press key step for entering idle interface
#define OTA_CONFIG_INFO_MAX_NUM              20         // the max number of ota config 
#define PORT_MAX_LEN                         6          // the max length of the URL port 
#define ORIGINAL_MSG_NUMBER_IN_IA            5          // the original message number in the internet account 
#define TRIGGER_STATE                        1          // trigger the timer
#define DELAY_TIME                           500        // the delay time of press key       

/**-----------------------------------------------------------------------------------*
 **                                   struct definitions                              *
 **-----------------------------------------------------------------------------------*/
typedef struct
{
    //uint8					need_security_connect;                        
    //MN_PCO_AUTH_TYPE_E    auth_type;                                    //鉴定类型0:PAP，1:CHAP:
    uint32                  auth_type;                                    //鉴定类型0:PAP，1:CHAP:
    uint8                   alias[MMICONNECTION_MAX_LINKMODENAME_LEN+1];  // alias of ota config
    uint8					username[MMICONNECTION_MAX_USERNAME_LEN + 1];  
    uint8					password[MMICONNECTION_MAX_PASSWORD_LEN + 1];
    uint8					apn[MMICONNECTION_MAX_APN_LEN + 1];
    uint8					gateway[MMICONNECTION_MAX_GATEWAY_LEN + 1];
    uint16					port;
    uint8					access_type;  //0:wap1.2; 1:wap2.0
    uint8					homepage[MMICONNECTION_HOMEPAGE_MAX_URL_LEN+1];
    uint8                   dns[MMICONNECTION_MAX_DNS_LEN+1];
}OTA_SETTING_DETAIL_T;  // the struct of the ota config information

typedef enum
{
    OTA_FLOW_SWITCH_QUIT_DIALOG = 0,
    OTA_FLOW_SWITCH_ENTER_IDLE,
    OTA_FLOW_SWITCH_FILTER_NO_OTA,
    OTA_FLOW_SWITCH_CONFIG_OTA,
    OTA_FLOW_SWITCH_GET_OTA_CFG,
    OTA_FLOW_SWITCH_DELETE_OTA,
    OTA_FLOW_SWITCH_PRE_PROCESS_INBOX,
    OTA_FLOW_SWITCH_PRE_PROCESS_IA,    // Internet Account
    OTA_FLOW_SWITCH_RETURN_IDLE,
    OTA_FLOW_SWITCH_MAX
}ATEST_OTA_FLOW_SWITCH_E;   // the type of the config switch

typedef enum
{
    RETURN_IDLE_SWITCH_DEFAULT = 0,
    RETURN_IDLE_SWITCH_ENTER_IA, 
    RETURN_IDLE_SWITCH_START_OTA
}ATEST_RETURN_IDLE_SWITCH_E;  // the type of the process after returning idle.

typedef enum
{
    ENTER_INBOX_GUI_INDEX = 5,
    ENTER_IA_GUI_INDEX = 10
}ATEST_ENTER_GUI_INDEX_E;     // the index of the GUI which is entered.

typedef enum
{
    EMPTY_IN_INBOX = 0,
    NO_EMPTY_IN_INBOX
}ATEST_MSG_NUMBER_IN_INBOX_E;  // the message number of the Inbox.

/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : get the OTA config information from the Internet account.
//--------------------------------------------------------------------------------------
// Input Parameter  : const MMICMSBRW_LINKSETTING_DETAIL_T *pduConfigDetail
//                    -- Mmibrowser_wintab.c / HandleSettingLinkMenuWinMsg
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_GetOtaConfigInfoFromLinkSetting(const MMICONNECTION_LINKSETTING_DETAIL_T pduConfigDetail);

/**************************************************************************************/
// Description      : get the total number of all messages in Inbox.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint16 totalNubmerOfIndexMsg  -- mmismsapp_ order.c / CalculateTotalNum
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_GetTotalMsgNumberInInbox(uint16 totalNubmerOfIndexMsg);

/**************************************************************************************/
// Description      : get the total number of all OTA config information in Internet Account.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint8 totalNubmerOfIA -- Mmibrowser_wintab.c / HandleSettingListWinMsg
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_GetTotalMsgNumberInInternetAccount(uint8 totalNubmerOfIA);

/**************************************************************************************/
// Description      : process the signal from control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_TestOta(uint16 ota_signal_id);

/**************************************************************************************/
// Description      : process operation flow by different config switch for every scenario.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN ATESTOTA_DispatchPressKeySerialMsg(void);

/**************************************************************************************/
// Description      : quit the pop dialog of the OTA and SMS.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_ProcessPressKeySerialByReceiveOta();

#endif//_ATEST_OTA_H