#ifndef _ATC_STK_ADAPT_H_
#define _ATC_STK_ADAPT_H_
/**************************************************************************

Copyright (C) 2001 SPREADTRUM COMMUNICATION CO.
All rights reserved.

This document contains proprietary information blonging to
SPREADTRUM. Passing on and copying this document, use and
communication of its contents is not permitted without prior
writed authorization.

***************************************************************************/

/***************************************************************************

Reversion information:
File Name:    atc_stk_adapt.h
Version:      0.1
Author:       Bin.Li
Date:         2009.4
Description:  This is the header file have the phoneserver stk command
            relative data and macro define
***************************************************************************/
/***************************************************************************

            Edit Histroy
Date          Name            Description
--------------------------------------------------------------------------
2009.4        Bin.Li    Create
***************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#include "atc_stk_enum.h"


typedef struct
{
    SIMAT_PC_NAME_E    pc_cmd_type;

    union
    {
        SIMAT_REFRESH_CNF_T         refresh_cnf;    // data about refresh
        SIMAT_SETUP_CALL_CNF_T      setup_call_cnf;  // terminal data about setup call
        SIMAT_SEND_SS_CNF_T         send_ss_cnf;
        SIMAT_SEND_USSD_CNF_T       send_ussd_cnf;
        SIMAT_SEND_SMS_CNF_T        send_sms_cnf;
        SIMAT_SEND_DTMF_CNF_T       send_dtmf_cnf;

        SIMAT_PLAY_TONE_CNF_T       play_tone_cnf;
        SIMAT_DISPLAY_TEXT_CNF_T    display_text_cnf;
        SIMAT_GET_INKEY_CNF_T       get_inkey_cnf;
        SIMAT_GET_INPUT_CNF_T       get_input_cnf;
        SIMAT_SELECT_ITEM_CNF_T     select_item_cnf;

        SIMAT_SETUP_MENU_CNF_T      setup_menu_cnf;

        SIMAT_SETUP_IDLE_TEXT_CNF_T     idle_text_cnf;
        SIMAT_GET_LANG_CNF_T            get_lang_cnf;
        SIMAT_SETUP_EVENT_LIST_CNF_T setup_eventlist_cnf;
        SIMAT_SEND_DATA_CNF_T       send_data_cnf;
        SIMAT_RECEIVE_DATA_CNF_T    receive_data_cnf;
        SIMAT_LAUNCH_BROWSER_CNF_T  launch_browser_cnf;
        SIMAT_OPEN_CHANNEL_CNF_T    open_channel_cnf;
        SIMAT_CLOSE_CHANNEL_CNF_T   close_channel_cnf;
        SIMAT_GET_CHANNEL_STATUS_CNF_T  get_channel_status_cnf;
        SIMAT_GET_DT_TZ_CNF_T           get_dttz_cnf;
    } item;
} STK_TERMINAL_RESULT_T;


typedef struct
{
    SIMAT_ME_SIM_TAG_E event_tag;
    union
    {
        SIMAT_MENU_SELECTION_IND_T menu_sel_ind;
        SIMAT_DO_EVENT_LIST_T      event_list;

    } item;

} STK_ENVELOP_RESULT_T;


/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN  PhoneSever_STK_Envelop_Handle
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,    //in:the proactive command data buf from SIM
    uint16 data_len, //the length of the proactive data command buf
    STK_ENVELOP_RESULT_T *stk_envelop_result_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN  PhoneSever_STK_TerminalRsp_Handle
(
    uint8 *data,    //in:the proactive command data buf from SIM
    uint16 data_len, //the length of the proactive data command buf
    STK_TERMINAL_RESULT_T *stk_terminal_result
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_DisplayText
(
    SIMAT_DISPLAY_TEXT_T *display_text_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_GetInkey
(
    SIMAT_GET_INKEY_T *get_inkey_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_GetInput
(
    SIMAT_GET_INPUT_T *get_input_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SetupMenu
(
    SIMAT_SETUP_MENU_T *setup_menu_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SetupCall
(
    SIMAT_SETUP_CALL_T *setup_call_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SelectItem
(
    SIMAT_SELECT_ITEM_T *select_item_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);


/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SendSms
(
    SIMAT_SEND_SM_T *send_sms_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SendSs
(
    SIMAT_SEND_SS_T *send_ss_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SendUssd
(
    SIMAT_SEND_USSD_T *send_ussd_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_PlayTone
(
    SIMAT_PLAY_TONE_T *play_tone_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SetupEventList
(
    SIMAT_SETUP_EVENT_LIST_T *setup_event_list,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_Refresh
(
    SIMAT_REFRESH_T *refresh_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_ProLocInfo
(
    SIMAT_LOCAL_INFO_TYPE_E *local_info_type_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SetupIdleText
(
    SIMAT_SETUP_IDLE_TEXT_T *setup_idle_text_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SendDtmf
(
    SIMAT_SEND_DTMF_T *send_dtmf_ptr,
    uint8 *data,
    uint16 *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note: only support open channel related gprs
***********************************************************************/
BOOLEAN SIMAT_Proactive_OpenChannel(
    SIMAT_OPEN_CHANNEL_RELATED_GPRS_T *openchannel_ptr,
    uint8   *data,
    uint16  *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_CloseChannel(
    SIMAT_CLOSE_CHANNEL_T *closechannel_ptr,
    uint8   *data,
    uint16  *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_LaunchBrowser(
    SIMAT_LAUNCH_BROWSER_T *lauchbrowser_ptr,
    uint8   *data,
    uint16  *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_SendData(
    SIMAT_SEND_DATA_T *senddata_ptr,
    uint8   *data,
    uint16  *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_ReceiveData(
    SIMAT_RECEIVE_DATA_T *receivedata_ptr,
    uint8   *data,
    uint16  *data_len_ptr
);

BOOLEAN SIMAT_Proactive_GetChannelStatusInd(
    uint8   *data,
    uint16  *data_len_ptr
);

/********************************************************************
//  DESCRIPTION
//      Get the proactive command name by the command type tag
//  Global resource dependence :
//  Author:Bin.Li
//  Note:ref to GSM 11.14 section 13.4
***********************************************************************/
SIMAT_PC_NAME_E ATC_GetPCName //return value:the proactive command name
(
    uint8 pc_tag    //the proactive command
);
#ifdef __cplusplus
}
#endif

#endif  //_ATC_STK_ADAPT_H_
