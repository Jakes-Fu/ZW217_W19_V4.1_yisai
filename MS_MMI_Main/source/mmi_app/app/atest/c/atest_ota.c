/***************************************************************************************
** FileName    :  atest_ota.c                                                         **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  01/06/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The implementaion of the step and action in the ota testing.        **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  01/06/2010     Mingxuan.Zhu         Created                                       **
**  08/08/2010     Mingxuan.Zhu         Updated                                       **
**  29/09/2010     Mingxuan.Zhu         Updated ( update the structure of the code )  **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#include "mmi_app_atest_trc.h"
#include "mmisms_id.h"
#include "mmisms_app.h"
#include "atest_ota.h"
#include "atest_signal.h"
#include "os_api.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"

/**-----------------------------------------------------------------------------------*
 **                               Constants and  Variables                            *
 **-----------------------------------------------------------------------------------*/
/* Local Variable Initialization */
// struct variable
LOCAL MN_DUAL_SYS_E        s_ota_dual_sys = MN_DUAL_SYS_1;
LOCAL OTA_SETTING_DETAIL_T s_pdu_config_info[OTA_CONFIG_INFO_MAX_NUM]={0};
LOCAL char                 s_ota_config_info[ATEST_STRING_MAX_LEN]={0};
// numerical variable
LOCAL BOOLEAN s_is_same_ota = FALSE;
LOCAL uint16 s_ota_no = 0;
LOCAL uint16 s_ota_number = 0;
LOCAL uint16 s_tobe_msg_total_number = 0;
LOCAL uint16 s_inbox_msg_total_number = 0;
LOCAL uint16 s_internet_account_msg_total_number = 0;
LOCAL uint16 s_idle_switch = RETURN_IDLE_SWITCH_DEFAULT;
LOCAL uint16 s_config_switch = OTA_FLOW_SWITCH_QUIT_DIALOG;
LOCAL uint16 s_no_msg_switch = EMPTY_IN_INBOX;
LOCAL uint16 s_press_key_index = 0;
STATIC SCI_TMID s_time_id_of_press_key_delay;

/* the press key step for every scenario of the OTA config */
LOCAL const uint32 s_press_key_sequence_by_enter_idle[PRESS_KEY_NUM_OF_ENTER_IDLE] = { SCI_VK_MENU_SELECT,SCI_VK_POWER,\
                                                                                       SCI_VK_POWER,SCI_VK_POWER };

LOCAL const uint32 s_press_key_sequence_by_receive_ota[PRESS_KEY_NUM_OF_RECEIVE_OTA] = { SCI_VK_MENU_SELECT,SCI_VK_POWER };

LOCAL const uint32 s_press_key_sequence_by_config_ota[PRESS_KEY_NUM_OF_CONFIG_OTA] = { SCI_VK_MENU_SELECT,SCI_VK_WEB,SCI_VK_MENU_SELECT,SCI_VK_DOWN,\
                                                                                       SCI_VK_WEB,SCI_VK_POWER };

LOCAL const uint32 s_press_key_sequence_by_delete_ia[PRESS_KEY_NUM_OF_DELETE_IA] = { SCI_VK_MENU_SELECT,SCI_VK_WEB,SCI_VK_DOWN,SCI_VK_RIGHT,\
                                                                                     SCI_VK_WEB,SCI_VK_UP,SCI_VK_WEB,SCI_VK_DOWN,\
                                                                                     SCI_VK_DOWN,SCI_VK_WEB,SCI_VK_DOWN,SCI_VK_DOWN,\
                                                                                     SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_MENU_SELECT,\
                                                                                     SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_MENU_SELECT,\
                                                                                     SCI_VK_MENU_SELECT,SCI_VK_POWER };

LOCAL const uint32 s_press_key_sequence_by_delete_ota[PRESS_KEY_NUM_OF_DELETE_OTA] = { SCI_VK_MENU_SELECT,SCI_VK_WEB,SCI_VK_WEB,SCI_VK_DOWN,\
                                                                                       SCI_VK_WEB,SCI_VK_MENU_SELECT,SCI_VK_DOWN,SCI_VK_MENU_SELECT,\
                                                                                       SCI_VK_MENU_SELECT,SCI_VK_POWER,SCI_VK_POWER };

LOCAL const uint32 s_press_key_sequence_by_get_ota_config[PRESS_KEY_NUM_OF_GET_OTA_INFO] = { SCI_VK_MENU_SELECT,SCI_VK_WEB,SCI_VK_DOWN,SCI_VK_RIGHT,\
                                                                                             SCI_VK_WEB,SCI_VK_UP,SCI_VK_WEB,SCI_VK_DOWN,\
                                                                                             SCI_VK_DOWN,SCI_VK_WEB,SCI_VK_DOWN,SCI_VK_DOWN,\
                                                                                             SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_WEB,\
                                                                                             SCI_VK_MENU_CANCEL,SCI_VK_MENU_SELECT,SCI_VK_MENU_SELECT,SCI_VK_DOWN,\
                                                                                             SCI_VK_DOWN,SCI_VK_DOWN,SCI_VK_MENU_SELECT,SCI_VK_MENU_SELECT,SCI_VK_POWER };

/**-----------------------------------------------------------------------------------*
 **                                Function Prototypes                                *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : send the message which notify the control panel that the ota testing can been started.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_StartOtaTestToCPMsg(void);

/**************************************************************************************/
// Description      : send the OTA config information to control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendOtaCfgToCPMsg(ATEST_MSG_NUMBER_IN_INBOX_E msgNumber);

/**************************************************************************************/
// Description      : enter the idle interface from others interface.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByEnterIdle();

/**************************************************************************************/
// Description      : return the idle interface after preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByReturnIdle();

/**************************************************************************************/
// Description      : delete the messages of the Inbox in the preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByPreprocessInbox();

/**************************************************************************************/
// Description      : delete the ota config information of the Internet Account in the preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByPreprocessInternetAccount();

/**************************************************************************************/
// Description      : delete the no OTA message and config the OTA message.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByDelNoOta();

/**************************************************************************************/
// Description      : config OTA messages in the Inbox.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByConfigOta();

/**************************************************************************************/
// Description      : get OTA config information from the Internet Account.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByGetOtaConfig();

/**************************************************************************************/
// Description      : delete OTA messages which has been configured in the Inbox.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByDeleteOta();

/**************************************************************************************/
// Description      : save ota messages into the temporary file.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SaveOtaCfgInfoToGlobalArray(void);

/**************************************************************************************/
// Description      : .
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SetTimerForDelayingPressKey(uint32 triggerState);

/**-----------------------------------------------------------------------------------*
 **                                Function Implementation                            *
 **-----------------------------------------------------------------------------------*/

/**************************************************************************************/
// Description      : send the message which notify the control panel that the ota testing can been started.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_StartOtaTestToCPMsg(void)
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 

    // create signal
    SCI_CREATE_SIGNAL( signal_ptr,
        CPANEL_ATEST_REQ,
        sizeof(ATEST_APP_SIG_T),
        SCI_IDENTIFY_THREAD() );
    
    // set the sig_ptr's param
    signal_ptr->dual_sys = MN_DUAL_SYS_1;
    signal_ptr->signal_id = ATEST_SEND_FIRST_OTA_REQ;
    
    // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
    //SCI_TRACE_LOW:"ATEST_TRACE====START OTA TESTING!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATEST_OTA_232_112_2_18_2_3_0_2,(uint8*)"");
}

/**************************************************************************************/
// Description      : send the ota config information to control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SendOtaCfgToCPMsg(ATEST_MSG_NUMBER_IN_INBOX_E msgNumber)
{
    ATEST_APP_SIG_T *signal_ptr = PNULL; 
    
    // create signal
    SCI_CREATE_SIGNAL( signal_ptr,
        CPANEL_ATEST_REQ,
        sizeof(ATEST_APP_SIG_T),
        SCI_IDENTIFY_THREAD() );
    
    // set the sig_ptr's param
    signal_ptr->dual_sys = MN_DUAL_SYS_1;
    signal_ptr->signal_id = ATEST_SEND_NEXT_OTA_REQ;
    signal_ptr->status_code = msgNumber;
    if(NO_EMPTY_IN_INBOX == msgNumber)
    {
        SCI_MEMCPY(signal_ptr->record_data,s_ota_config_info,ATEST_STRING_MAX_LEN);
    }
    
    // send signal to control panel.
    SCI_SEND_SIGNAL((xSignalHeaderRec*)signal_ptr, P_CPANEL);
}

/**************************************************************************************/
// Description      : enter the idle interface from others interface.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByEnterIdle()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_ENTER_IDLE)
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // process the press key message of the action which enter idle interface.
            ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_enter_idle[s_press_key_index]);
            ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
            s_press_key_index++;
        }
    }
    else
    {   // enter the preprocessing flow of the Inbox after entering idle interface.
        if(!s_is_same_ota)
        {
            s_press_key_index = 0;
            s_config_switch = OTA_FLOW_SWITCH_PRE_PROCESS_INBOX;
            MMIAPIENVSET_SetActiveModeForIdle(MMIENVSET_SILENT_MODE); // set silent mode
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
        else
        {
            s_press_key_index = 0;
            s_is_same_ota = FALSE;
            s_config_switch = OTA_FLOW_SWITCH_DELETE_OTA;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
    }
}

/**************************************************************************************/
// Description      : return the idle interface after preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByReturnIdle()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_ENTER_IDLE )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // process the press key message of the action which return idle interface.
            ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_enter_idle[s_press_key_index]);
            ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
            s_press_key_index++;
        }
    }
    else
    {
        if(RETURN_IDLE_SWITCH_ENTER_IA == s_idle_switch)  
        {   // enter the preprocessing flow of the Internet Account after processing the Inbox.
            s_press_key_index = 0;
            s_config_switch = OTA_FLOW_SWITCH_PRE_PROCESS_IA;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
        else if(RETURN_IDLE_SWITCH_START_OTA == s_idle_switch)
        {   // reset the configSwitch and start the ota testing.
            s_press_key_index = 0;
            s_config_switch = OTA_FLOW_SWITCH_QUIT_DIALOG;
            s_idle_switch = RETURN_IDLE_SWITCH_DEFAULT;
            ATEST_StartOtaTestToCPMsg();
        }
    }
}

/**************************************************************************************/
// Description      : delete the messages of the Inbox in the preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByPreprocessInbox()
{
    if( 0 != s_inbox_msg_total_number )
    {
        if(s_press_key_index < PRESS_KEY_NUM_OF_DELETE_OTA )
        {
            if(0 == s_press_key_index)
            {
                ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
                s_press_key_index++;
            }
            else
            {   // delete the messages in the Inbox.
                ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ota[s_press_key_index]);
                ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                s_press_key_index++;               
            }   
        }
        else
        {   // process next message in the Inbox.
            s_press_key_index = 0;    
            s_config_switch = OTA_FLOW_SWITCH_PRE_PROCESS_INBOX;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
    }
    else
    {   // return the idle interface after processing the Inbox.
        s_press_key_index = 0;    
        s_config_switch = OTA_FLOW_SWITCH_RETURN_IDLE;
        s_idle_switch = RETURN_IDLE_SWITCH_ENTER_IA;
        ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
    }    
}

/**************************************************************************************/
// Description      : delete the ota config information of the Internet Account in the preprocessing flow.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByPreprocessInternetAccount()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_DELETE_IA )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {
            if(ENTER_IA_GUI_INDEX == s_press_key_index) // enter the Internet Account interface
            {
                if( 0 != (s_internet_account_msg_total_number - ORIGINAL_MSG_NUMBER_IN_IA) ) // check whether the ota config is existed or not.
                {   // delete the ota config information of the Internet Account
                    ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ia[s_press_key_index]);
                    ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                    s_press_key_index++;   
                }
                else
                {   // return the idle interface after processing the Internet Account. 
                    s_press_key_index = 0;
                    s_config_switch = OTA_FLOW_SWITCH_RETURN_IDLE;
                    s_idle_switch = RETURN_IDLE_SWITCH_START_OTA;  // the switch which start the ota testing 
                    ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
                }
            }
            else
            {   // the process before entering the Interner Account interface
                ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ia[s_press_key_index]);
                ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                s_press_key_index++;  
            } 
        }   
    }
    else
    {   // process next ota config information in the Interner Account.
        s_press_key_index = 0;    
        s_config_switch = OTA_FLOW_SWITCH_PRE_PROCESS_IA;
        ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
    }
}

/**************************************************************************************/
// Description      : delete the no ota messages and config the ota messages.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByDelNoOta()
{
    uint16  selected_index = 0;
    s_tobe_msg_total_number = MMIAPISMS_GetTobeReadMessageCount(s_ota_dual_sys); // tobe message total number

    if(0 != s_tobe_msg_total_number || (0 != s_press_key_index && s_press_key_index < PRESS_KEY_NUM_OF_DELETE_OTA))
    {
        s_no_msg_switch = NO_EMPTY_IN_INBOX;
        if(s_press_key_index < PRESS_KEY_NUM_OF_DELETE_OTA )
        {
            if(0 == s_press_key_index)
            {
                ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
                s_press_key_index++;
            }
            else
            {
                if(ENTER_INBOX_GUI_INDEX == s_press_key_index) // enter the Inbox
                {
                    selected_index = GUILIST_GetCurItemIndex(MMISMS_MTBOX_LISTBOX_CTRL_ID);
                    MMISMS_SetCurOperationOrderId(MMISMS_BOX_MT, selected_index);
                    if(MMISMS_GetMsgType()== MMISMS_TYPE_MMS_OTA || MMISMS_GetMsgType()==MMISMS_TYPE_WAP_OTA)
                    {   // config the ota messages.
                        s_config_switch = OTA_FLOW_SWITCH_CONFIG_OTA;
                        s_press_key_index = 0;
                        ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
                    }
                    else
                    {   // delete the no ota messages.
                        ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ota[s_press_key_index]);
                        ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                        s_press_key_index++;               
                    }
                }
                else
                {   // the process before entering the Inbox.
                    ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ota[s_press_key_index]);
                    ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                    s_press_key_index++;
                }
                
            }
        }
        else
        {   // process next the message in the Inbox.
            s_press_key_index = 0;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
    }
    else
    {
        if(NO_EMPTY_IN_INBOX == s_no_msg_switch)
        {   // enter the getting ota config information after configging the ota messages
            s_press_key_index = 0;
            s_tobe_msg_total_number = 0;
            s_no_msg_switch = EMPTY_IN_INBOX;
            s_config_switch = OTA_FLOW_SWITCH_GET_OTA_CFG;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
        else
        {   // the process when the Inbox is empty.
            // reset the config switch and send the ota config information to control panel.
            s_press_key_index = 0;
            s_tobe_msg_total_number = 0;
            s_config_switch = OTA_FLOW_SWITCH_QUIT_DIALOG;
            ATEST_SendOtaCfgToCPMsg(EMPTY_IN_INBOX);
        }
    }
}

/**************************************************************************************/
// Description      : config ota messages in the Inbox.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByConfigOta()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_CONFIG_OTA )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // config ota messages in the Inbox.
            ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_config_ota[s_press_key_index]);
            ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
            s_press_key_index++;
        }
    }
    else
    {   // return the inbox interface and process next message after configging ota information.
        s_press_key_index = 0;
        s_config_switch = OTA_FLOW_SWITCH_FILTER_NO_OTA;
        ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
    }
}

/**************************************************************************************/
// Description      : get ota config information from the Internet Account.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByGetOtaConfig()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_GET_OTA_INFO )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // get ota config information.
            if(ENTER_IA_GUI_INDEX == s_press_key_index && ORIGINAL_MSG_NUMBER_IN_IA == s_internet_account_msg_total_number)
            {
                // the process when the OTA is the same with ota config info in the Internet Account. 
                // reset the config switch and send the ota config information to control panel.
                s_press_key_index = 0;
                s_internet_account_msg_total_number = 0;
                s_is_same_ota = TRUE;
                s_config_switch = OTA_FLOW_SWITCH_ENTER_IDLE;
                ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            }
            else
            {
                ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_get_ota_config[s_press_key_index]);
                ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
                s_press_key_index++;
            }
            
        }
    }
    else
    {
        if(0 != (s_internet_account_msg_total_number - (ORIGINAL_MSG_NUMBER_IN_IA+1)))  // -5-1( current deleted OTA config )
        {   // get next ota config information.
            s_press_key_index = 0;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
        else
        {   // enter the Inbox interface and delete ota messages after getting ota config information.
            s_press_key_index = 0;
            s_internet_account_msg_total_number = 0;
            s_config_switch = OTA_FLOW_SWITCH_DELETE_OTA;
            ATEST_SaveOtaCfgInfoToGlobalArray(); // save ota config information into temporary file. 
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
    }
}

/**************************************************************************************/
// Description      : delete ota messages which has been configured in the Inbox.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_ProcessPressKeySerialByDeleteOta()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_DELETE_OTA )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // delete ota messages.
            ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_delete_ota[s_press_key_index]);
            ATEST_SetTimerForDelayingPressKey(TRIGGER_STATE);
            s_press_key_index++;
        }
    }
    else
    {
        if(0 != s_inbox_msg_total_number)
        {   // process next ota message.
            s_press_key_index = 0;
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
        }
        else
        {   // reset the config switch and send the ota config information to control panel. 
            s_press_key_index = 0;
            s_config_switch = OTA_FLOW_SWITCH_QUIT_DIALOG;
            ATEST_SendOtaCfgToCPMsg(NO_EMPTY_IN_INBOX);
        }
    }
}

/**************************************************************************************/
// Description      : save OTA messages into the global array.
//--------------------------------------------------------------------------------------
// Input Parameter  : NONE
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SaveOtaCfgInfoToGlobalArray(void)
{
    char tempNo[PORT_MAX_LEN]="";
    int32 otaNoPerPDU = 0;

    SCI_MEMSET(&s_ota_config_info,0,sizeof(char)*ATEST_STRING_MAX_LEN);

    for(otaNoPerPDU=0; otaNoPerPDU < s_inbox_msg_total_number; otaNoPerPDU++)
    {
        if(0 != otaNoPerPDU)
        {
            strcat(s_ota_config_info,"\r");
        }
        strcat(s_ota_config_info,"[Change Name]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].alias);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Set Homepage]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].homepage);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Gateway]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].gateway);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Port]:");
        strcat(s_ota_config_info,itoa(s_pdu_config_info[otaNoPerPDU].port,tempNo,10));
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[User Name]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].username);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Password]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].password);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Access Points]:");
        strcat(s_ota_config_info,(char*)s_pdu_config_info[otaNoPerPDU].apn);
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Security Connect]:");
        if(0 == s_pdu_config_info[otaNoPerPDU].auth_type)
        {
            strcat(s_ota_config_info,"Forbidden");
        }
        else
        {
            strcat(s_ota_config_info,"Enable");
        }
        strcat(s_ota_config_info,"\r");
        strcat(s_ota_config_info,"[Net Type]:");
        if (0 == s_pdu_config_info[otaNoPerPDU].access_type)
        {
            strcat(s_ota_config_info,"WAP");
        } 
        else
        {
            strcat(s_ota_config_info,"HTTP");
        }
        strcat(s_ota_config_info,"\0\n");
    }
    // reset pduConfigInfo array
    SCI_MEMSET(&s_pdu_config_info,0,sizeof(OTA_SETTING_DETAIL_T)*OTA_CONFIG_INFO_MAX_NUM);
    s_ota_no = 0;
}

/**************************************************************************************/
// Description      : set delay time after key is pressed.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint32 triggerState
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
LOCAL void ATEST_SetTimerForDelayingPressKey(uint32 triggerState)
{
    // check whether s_time_id_of_press_key_delay is null or not.
    if(PNULL == s_time_id_of_press_key_delay)
    {
        s_time_id_of_press_key_delay = SCI_CreateTimer( "Press Key Delay Timer",
            ATESTKEYSTEP_TriggerPressKeySerialMsg,
            triggerState,
            DELAY_TIME,
            SCI_NO_ACTIVATE );
    }

    if (SCI_IsTimerActive(s_time_id_of_press_key_delay))
    {
        SCI_DeactiveTimer(s_time_id_of_press_key_delay);
    }
    
    SCI_ChangeTimer(s_time_id_of_press_key_delay,ATESTKEYSTEP_TriggerPressKeySerialMsg,DELAY_TIME);
    SCI_ActiveTimer(s_time_id_of_press_key_delay);
}

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
PUBLIC void ATESTOTA_GetTotalMsgNumberInInbox(uint16 totalNubmerOfIndexMsg)
{
    s_inbox_msg_total_number = totalNubmerOfIndexMsg;
}

/**************************************************************************************/
// Description      : get the total number of all ota config information in Internet Account.
//--------------------------------------------------------------------------------------
// Input Parameter  : uint8 totalNubmerOfIA -- Mmibrowser_wintab.c / HandleSettingListWinMsg
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC void ATESTOTA_GetTotalMsgNumberInInternetAccount(uint8 totalNubmerOfIA)
{
    s_internet_account_msg_total_number = totalNubmerOfIA;
}

/**************************************************************************************/
// Description      : get the OTA config information from the internet account.
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
PUBLIC void ATESTOTA_GetOtaConfigInfoFromLinkSetting(const MMICONNECTION_LINKSETTING_DETAIL_T pduConfigDetail)
{
    uint8 ota_alias[MMICONNECTION_MAX_LINKMODENAME_LEN+1]={0};
    GUI_WstrToGB(ota_alias,pduConfigDetail.name,pduConfigDetail.name_len);
    
    s_pdu_config_info[s_ota_no].auth_type = pduConfigDetail.auth_type;
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].alias,ota_alias,pduConfigDetail.name_len);
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].username,pduConfigDetail.username,pduConfigDetail.username_len);
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].password,pduConfigDetail.password,pduConfigDetail.password_len);
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].apn,pduConfigDetail.apn,pduConfigDetail.apn_len);
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].gateway,pduConfigDetail.gateway,pduConfigDetail.gateway_len);
    s_pdu_config_info[s_ota_no].port = pduConfigDetail.port;
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].homepage,pduConfigDetail.homepage,pduConfigDetail.homepage_len);
    s_pdu_config_info[s_ota_no].access_type = pduConfigDetail.access_type;
    SCI_MEMCPY(s_pdu_config_info[s_ota_no].dns,pduConfigDetail.dns,(MMICONNECTION_MAX_DNS_LEN+1));
    s_ota_no++;
}

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
PUBLIC void ATESTOTA_TestOta(uint16 ota_signal_id)
{
    switch(ota_signal_id)
    {
    case ATEST_ENTER_IDLE_IND: // reset the idle interface of the simulator.
        {
            //SCI_TRACE_LOW:"ATEST_TRACE====Enter OTA MODULE!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATEST_OTA_829_112_2_18_2_3_2_3,(uint8*)"");
            s_config_switch = OTA_FLOW_SWITCH_ENTER_IDLE;
            ATEST_ProcessPressKeySerialByEnterIdle();   
        }
        break;
    case ATEST_SEND_OTA_IND:  // process the OTA message from the control panel.
        {
            s_config_switch = OTA_FLOW_SWITCH_FILTER_NO_OTA;
            ATEST_ProcessPressKeySerialByDelNoOta();
        }
        break;
    case ATEST_END_OTA_CNF: // notify the simulator that the OTA auto testing is ended normally.
        {
            s_config_switch = OTA_FLOW_SWITCH_QUIT_DIALOG;   // reset the config switch flag
            SCI_DeleteTimer(s_time_id_of_press_key_delay);   // delete the timer of press key delay
            s_time_id_of_press_key_delay = PNULL;
            //SCI_TRACE_LOW:"ATEST_TRACE====NORMAL EXIT OTA TESTING!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATEST_OTA_845_112_2_18_2_3_2_4,(uint8*)"");
        }
        break;
    default:
        break;
    }
}

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
PUBLIC BOOLEAN ATESTOTA_DispatchPressKeySerialMsg(void)
{
    switch (s_config_switch)
    {
    case OTA_FLOW_SWITCH_ENTER_IDLE:      // enter idle interface.
        ATEST_ProcessPressKeySerialByEnterIdle();
        break;
    case OTA_FLOW_SWITCH_FILTER_NO_OTA:   // delete no ota message and configure ota message in Inbox.
        ATEST_ProcessPressKeySerialByDelNoOta();
        break;
    case OTA_FLOW_SWITCH_CONFIG_OTA:      // configure ota mesage.
        ATEST_ProcessPressKeySerialByConfigOta();
        break;
    case OTA_FLOW_SWITCH_GET_OTA_CFG:     // get ota config from Internet Account
        ATEST_ProcessPressKeySerialByGetOtaConfig();
        break;
    case OTA_FLOW_SWITCH_DELETE_OTA:      // delete ota message in Inbox
        ATEST_ProcessPressKeySerialByDeleteOta();
        break;
    case OTA_FLOW_SWITCH_PRE_PROCESS_INBOX:   // preprocess Inbox
        ATEST_ProcessPressKeySerialByPreprocessInbox();
        break;
    case OTA_FLOW_SWITCH_PRE_PROCESS_IA:      // preprocess Internet Account
        ATEST_ProcessPressKeySerialByPreprocessInternetAccount();
        break;
    case OTA_FLOW_SWITCH_RETURN_IDLE:         // return the idle interface after preprocessing is over.
        ATEST_ProcessPressKeySerialByReturnIdle();
        break;
    case OTA_FLOW_SWITCH_QUIT_DIALOG:         // return idle interface from pop dialog
        ATESTOTA_ProcessPressKeySerialByReceiveOta();
        break;
    default:
        break;
    }
    return TRUE;
}

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
PUBLIC void ATESTOTA_ProcessPressKeySerialByReceiveOta()
{
    if(s_press_key_index < PRESS_KEY_NUM_OF_RECEIVE_OTA )
    {
        if(0 == s_press_key_index)
        {
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
        else
        {   // return the idle interface from pop dialog of the OTA and SMS.
            ATESTKEYSTEP_ProcessPressKey(s_press_key_sequence_by_receive_ota[s_press_key_index]);
            ATESTKEYSTEP_TriggerPressKeySerialMsg(TRIGGER_STATE);
            s_press_key_index++;
        }
    }
    else
    {
        s_press_key_index=0;
    }
}


