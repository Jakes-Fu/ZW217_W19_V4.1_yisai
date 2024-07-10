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
File Name:    atc_stk_adapt.c from old name:phonesever_stk_adapt.c

Description:  This is the C file for the func to be used by phonesever
***************************************************************************/
//#include "scttypes_rtos.h"
#include "atc_common.h"
#include "MN.h"
#include "sim_type.h"
#include "simat_api.h"
#include "sci_types.h"
#include "sim_macro_switch.h"
#include "simat_data_object.h"
#include "atc_stk_enum.h"
#include "atc_stk_func.h"
#include "atc_stk_adapt.h"
#include "atc_stk_common_func.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*lint -save -e752 */
extern MN_DUAL_SYS_E        g_current_card_id;// lint !e752

//the array that mapping the detail proactive command tag to the proactive command
STATIC const uint8 s_simat_pc_tag_array[SIMAT_PC_NUM] =
{
    //---------------------------------------------------------
    //  PC tag                      PC note
    //---------------------------------------------------------
    0x01,               //REFRESH
    0x02,               //MORE TIME
    0x03,               //POLLING INTERVAL
    0x04,               //POLLING OFF
    0x05,               //SETUP EVENT LIST
    0x10,               //SET UP CALL
    0x11,               //SEND SS
    0x12,               //SEND USSD
    0x13,               //SEND SMS
    0x14,               //SEND DTMF
    0x15,               //LAUNCH BROWSER
    0x20,               //PLAY TONE
    0x21,               //DISPLAY TEXT
    0x22,               //GET INKEY
    0x23,               //GET INPUT
    0x24,               //SELECT ITEM
    0x25,               //SETUP MENU
    0x26,               //PROVIDE LOCAL INFO
    0x27,               //TIME MANAGEMENT
    0x28,               //SETUP IDLE MODE TEXT
    0x30,               //CARD APDU
    0x31,               //POWER ON CARD
    0x32,               //POWER OFF CARD
    0x33,               //GET CARD STATUS
    0x34,               //RUN AT COMMAND
    0x35,                //LANG NOTIFICATION
    /* bin.li add usat */
    0x40,               /* OPEN CHANNEL */
    0x41,               /* CLOSE CHANNEL */
    0x42,               /* RECEIVE DATA */
    0x43,               /* SEND DATA */
    0x44,               /* GET CHANNEL STATUS */
    0x60,               /* RETRIEVE_MUITIMEDIA_MSG */
    0x61,               /* SUBBMIT_MUITIMEDIA_MSG */
    0x62                /* DISPLAY_MUITIMEDIA_MSG */

};

extern SIMAT_GENERAL_PC_RESULT_E DecodeMinimalObject(
                                SIMAT_B_DIRECTION_TAG_E object_type,    //the object type of the simple object
                                uint8                   *pdata,         //in:the data buf that including the simple TLV data object
                                uint8                   *offset,        //out:the simple TLV data object length in the data buf
                                void                    *pobject,       //out:the simple TLV data object
                                uint16                  data_len        //the total length of the data buf
                                );

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN EncodePCCommand
(
    uint8 com_num,
    uint8 com_type,
    uint8 com_qulifier,
    uint8 *data,
    uint16 *data_len
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_MenuSel_Env
(
    uint8 *data,
    uint16 data_len,
    SIMAT_MENU_SELECTION_IND_T *menu_sel_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Event_DownLoad_Env
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_UserActivity(MN_DUAL_SYS_E   dual_sys);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_IdleScreenAvi(MN_DUAL_SYS_E   dual_sys);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_BrowserTermination
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_LanSel
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Huawei.zhang
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_DataAvailable
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Huawei.zhang
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_ChannelStatus
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_DisplayText
(
    uint8 *data,
    uint16 data_len,
    SIMAT_DISPLAY_TEXT_CNF_T *display_text_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetInkey
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_INKEY_CNF_T  *get_inkey_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetInput
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_INPUT_CNF_T *get_input_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupMenu
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_MENU_CNF_T *setup_menu_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupCall
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_CALL_CNF_T *setup_call_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SelectItem
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SELECT_ITEM_CNF_T *select_item_cnf_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendDtmf
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SEND_DTMF_CNF_T *send_dtmf_cnf_ptr
);

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendData(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in, the length of response data
    SIMAT_SEND_DATA_CNF_T *send_data_cnf_ptr    //out, response info of send data
);
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_ReceiveData(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_RECEIVE_DATA_CNF_T *receive_data_cnf_ptr    //out,response info of receive data
);
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_LaunchBrowser(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_LAUNCH_BROWSER_CNF_T *launch_browser_cnf_ptr    //out,response info of launch browser
);
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_OpenChannel(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_OPEN_CHANNEL_CNF_T *open_channel_cnf_ptr //out,response info of open channel
);
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_CloseChannel(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_CLOSE_CHANNEL_CNF_T *close_channel_cnf_ptr    //out,response info of close channel
);

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupEventList(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_SETUP_EVENT_LIST_CNF_T *setup_event_list_cnf_ptr    //out,response info of setup event list
);

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetChannelStatus(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_GET_CHANNEL_STATUS_CNF_T *get_channel_status_cnf_ptr    //out,response info of get channel status
);

#if 0
/********************************************************************
//    DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendSms
(
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//    DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendSs
(
    uint8 *data,
    uint16 data_len
);
/********************************************************************
//    DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendUssd
(
    uint8 *data,
    uint16 data_len
);
#endif

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_PlayTone
(
    uint8 *data,
    uint16 data_len,
    SIMAT_PLAY_TONE_CNF_T *play_tone_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_Refresh
(
    uint8 *data,
    uint16 data_len,
    SIMAT_REFRESH_CNF_T  *refresh_cnf_ptr
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_ProLocInfo
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_LANG_CNF_T *lang_cnf,
    SIMAT_GET_DT_TZ_CNF_T *dt_tz_cnf
);
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupIdleText
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_IDLE_TEXT_CNF_T *setup_idle_text_cnf_ptr
);


#if 0
/********************************************************************
//    DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendDtmf
(
    uint8 *data,
    uint16 data_len
);
#endif
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
)
{
    uint32 loop = 0;

    //searh in the proactive command type tag array
    for(loop = 0; loop < SIMAT_PC_NUM; loop++)
    {
        if(s_simat_pc_tag_array[loop] == pc_tag)
        {
            return (SIMAT_PC_NAME_E)loop;
        }
    }

    ATC_TRACE_LOW("SIMAT::ATC_GetPCName PC tag 0x%x is out of range. \n", pc_tag);

    return SIMAT_PC_NUM;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN EncodePCCommand
(
    uint8 com_num,
    uint8 com_type,
    uint8 com_qulifier,
    uint8 *data,
    uint16 *data_len
)
{
    if((NULL == data) || (NULL == data_len))
    {
        return FALSE;
    }

    *data = 0x81;
    *(data + 1) = 0x3;
    *(data + 2) = com_num;
    *(data + 3) = com_type;
    *(data + 4) = com_qulifier;

    *data_len += 5;

    return TRUE;

}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_UserActivity(MN_DUAL_SYS_E   dual_sys)
{
    SIMAT_IndUserActEx(dual_sys);
    /* bin.li at+sted should add here */
    /*begin  */

    /*end   */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_IdleScreenAvi(MN_DUAL_SYS_E   dual_sys)
{
    /* bin.li at+sted should add here */
    /*begin  */

    SIMAT_IndIdleScrAvaEx(dual_sys);
    /*end   */
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_BrowserTermination
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
)
{
    SIMAT_BROWSER_TERMINATION_IND_T browertermination;
    uint8  offset                   = 0;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;

    SCI_TRACE_LOW("[phoneserver_stk_adapt]:enter Process_BrowserTermination!");

    if ((NULL == data) || (data_len<3))
    {
        return FALSE;
    }

    SCI_MEMSET(&browertermination, 0x0 ,sizeof(SIMAT_BROWSER_TERMINATION_IND_T));

    gen_result = DecodeMinimalObject(SIMAT_BTAG_BROWSER_TERMINAT_CAUSE,data,&offset,&browertermination.termination_cause,data_len);

    if (SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    SIMAT_IndBrowserTerminationEx(dual_sys, (SIMAT_BROWSER_TERMINATION_IND_T *)&browertermination);

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_LanSel
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
)
{
    SIMAT_DO_LANG_T language;
    //uint8* ptemp                = data;
    //uint8  com_len              = 0;
    uint8  offset                   = 0;
    SIMAT_GENERAL_PC_RESULT_E gen_result  = SIMAT_TR_SUCCESS;

    if((NULL == data) || (data_len < 4))
    {
        return FALSE;
    }

    SCI_MEMSET(&language, 0x0 , sizeof(SIMAT_DO_LANG_T));

    gen_result = DecodeMinimalObject(SIMAT_BTAG_LANGUAGE, data, &offset, &language, data_len);

    if(SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    /* bin.li at+sted should add here use language */
    /*begin  */
    SIMAT_IndLangSelEx(dual_sys, (SIMAT_LANG_SELECTION_IND_T *)&language);
    /*end   */
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:huawei.zhang
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_DataAvailable(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len)
{
    SIMAT_DATA_AVAILABLE_IND_T datavailble;
    uint8  offset                   = 0;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;

    SCI_TRACE_LOW("[phoneserver_stk_adapt]:enter Process_DataAvailable!");

    if ((NULL == data) || (data_len<7))
    {
        return FALSE;
    }

    SCI_MEMSET(&datavailble, 0x0 ,sizeof(SIMAT_DATA_AVAILABLE_IND_T));

    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_STATUS,data,&offset,&datavailble.channel_status,data_len);

    if (SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_DATA_LEN,data,&offset,&datavailble.data_len,data_len);

    if (SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    SIMAT_IndDataAvailableEx(dual_sys, (SIMAT_DATA_AVAILABLE_IND_T *)&datavailble);

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:huawei.zhang
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Process_ChannelStatus(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len)
{
    SIMAT_CHANNEL_STATUS_IND_T channel;
    uint8  offset                   = 0;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;

    SCI_TRACE_LOW("[phoneserver_stk_adapt]:Process_ChannelStatus!");

    if ((NULL == data) || (data_len<4))
    {
        return FALSE;
    }

    SCI_MEMSET(&channel, 0x0 ,sizeof(SIMAT_CHANNEL_STATUS_IND_T));

    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_STATUS, data, &offset, &channel.channel_status, data_len);

    if (SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }
    
    SIMAT_IndChannelStatusEx(dual_sys, (SIMAT_CHANNEL_STATUS_IND_T *)&channel);

    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_MenuSel_Env
(
    uint8 *data,
    uint16 data_len,
    SIMAT_MENU_SELECTION_IND_T *menu_sel_ptr
)
{
    uint8 *ptemp                = data;
    uint8  com_len              = 0;
    uint8  offset                   = 0;
    // SIMAT_MENU_SELECTION_IND_T menu_sel;
    SIMAT_GENERAL_PC_RESULT_E gen_result  = SIMAT_TR_SUCCESS;
    SIMAT_DO_DEVICE_ID_T device_id;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != menu_sel_ptr);/*assert verified: check null pointer*/

    // SCI_MEMSET(&menu_sel, 0x0, sizeof(SIMAT_MENU_SELECTION_IND_T));

    /*+CR174286 Additional Moidification*/
    if((((uint8)SIMAT_DTAG_MENU_SELECTION) != *ptemp) || (data_len < SIMAT_MIN_PC_LEN - 2))
    {
        return FALSE;
    }

    ptemp += 1;
    data_len -= 1;

    //get the left length of the command,if the length error,ignore this proactive command
    if(!SIMAT_GetTLVLength(ptemp, &offset, &com_len))
    {
        return FALSE;
    }

    ptemp += offset;
    data_len -= offset;

    gen_result = DecodeMinimalObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id, data_len);

    if(SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    if((device_id.source != SIMAT_DE_KEYPAD)  || (device_id.destination != SIMAT_DE_SIM))
    {
        return FALSE;
    }

    ptemp += offset;
    data_len -= offset;

    gen_result = DecodeMinimalObject(SIMAT_BTAG_ITEM_ID, ptemp, &offset, &menu_sel_ptr->menu_id, data_len);

    if(SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    ptemp += offset;
    data_len -= offset;

    if(data_len > 0)
    {
        gen_result = DecodeMinimalObject(SIMAT_BTAG_HELP_REQ, ptemp, &offset, &menu_sel_ptr->is_help_IND, data_len);

        if(SIMAT_TR_SUCCESS != gen_result)
        {
            return FALSE;
        }

        menu_sel_ptr->is_help_IND = TRUE;
    }

    /*binli menu sel stru to at+stgr should do here ..... */
    /*begin  */

    /*end   */

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Event_DownLoad_Env
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len
)
{
    BOOLEAN ret = FALSE;
    uint8 *ptemp                = data;
    uint8  com_len              = 0;
    uint8  offset                   = 0;

    SIMAT_DO_EVENT_LIST_T event_list;
    SIMAT_GENERAL_PC_RESULT_E gen_result  = SIMAT_TR_SUCCESS;
    SIMAT_DO_DEVICE_ID_T device_id;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/

    ATC_TRACE_LOW("SIMAT_Event_DownLoad_Env:enter");

    SCI_MEMSET(&event_list, 0x0, sizeof(SIMAT_DO_EVENT_LIST_T));
    SCI_MEMSET(&device_id, 0x0, sizeof(SIMAT_DO_DEVICE_ID_T));


    if((((uint8)SIMAT_DTAG_EVENT_DOWNLOAD) != *ptemp) || (data_len < SIMAT_MIN_PC_LEN))
    {
        return FALSE;
    }

    ptemp += 1;
    data_len -= 1;

    //get the left length of the command,if the length error,ignore this proactive command
    if(!SIMAT_GetTLVLength(ptemp, &offset, &com_len))
    {
        return FALSE;
    }

    ptemp += offset;
    data_len -= offset;

    ATC_TRACE_LOW("SIMAT_Event_DownLoad_Env:SIMAT_BTAG_EVENT_LIST");

    gen_result = DecodeMinimalObject(SIMAT_BTAG_EVENT_LIST, ptemp, &offset, &event_list, data_len);

    if(SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    ptemp += offset;
    data_len -= offset;

    ATC_TRACE_LOW("SIMAT_Event_DownLoad_Env:SIMAT_BTAG_DEVICE_ID");
    gen_result = DecodeMinimalObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id, data_len);

    if(SIMAT_TR_SUCCESS != gen_result)
    {
        return FALSE;
    }

    ATC_TRACE_LOW("SIMAT_Event_DownLoad_Env:des/source");

    switch(event_list.events_arr[0])
    {
        case SIMAT_EVENT_USER_ACTIVITY:
        case SIMAT_EVENT_LAN_SEL:
        case SIMAT_EVENT_BROESWER_TERMINATION:
        case SIMAT_EVENT_DATA_AVAILABLE:
        case SIMAT_EVENT_CHANNEL_STATUS:

            if((device_id.source != SIMAT_DE_ME)  || (device_id.destination != SIMAT_DE_SIM))
            {
                return FALSE;
            }

            break;
        case SIMAT_EVENT_IDLE_SCREEN_AVIALBLE:

            if((device_id.source != SIMAT_DE_DISPLAY)  || (device_id.destination != SIMAT_DE_SIM))
            {
                return FALSE;
            }

            break;
        default:
            break;

    }

    ptemp += offset;
    data_len -= offset;

    ATC_TRACE_LOW("SIMAT_Event_DownLoad_Env Event is %d", event_list.events_arr[0]);

    switch(event_list.events_arr[0])
    {
        case SIMAT_EVENT_USER_ACTIVITY:
            ret = SIMAT_Process_UserActivity(dual_sys);
            break;
        case SIMAT_EVENT_IDLE_SCREEN_AVIALBLE:
            ret = SIMAT_Process_IdleScreenAvi(dual_sys);
            break;
        case SIMAT_EVENT_LAN_SEL:
            ret  = SIMAT_Process_LanSel(dual_sys, ptemp, data_len);
            break;
        case SIMAT_EVENT_BROESWER_TERMINATION:
            ret = SIMAT_Process_BrowserTermination(dual_sys, ptemp, data_len);
            break;
        case SIMAT_EVENT_DATA_AVAILABLE:
            ret = SIMAT_Process_DataAvailable(dual_sys, ptemp, data_len);
            break;
        case SIMAT_EVENT_CHANNEL_STATUS:
            ret = SIMAT_Process_ChannelStatus(dual_sys, ptemp, data_len);
            break;            
        default:
            return FALSE;
    }

    return ret;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
BOOLEAN PhoneSever_STK_Envelop_Handle
(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *data,
    uint16 data_len,
    STK_ENVELOP_RESULT_T *stk_envelop_result_ptr
)
{
    SIMAT_ME_SIM_TAG_E envelop_tag = SIMAT_DTAG_SMS_PP;
    BOOLEAN ret = FALSE;


    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != stk_envelop_result_ptr);/*assert verified: check null pointer*/

    envelop_tag = (SIMAT_ME_SIM_TAG_E)(*data);

    stk_envelop_result_ptr->event_tag = envelop_tag;

    ATC_TRACE_LOW("PhoneSever_STK_Envelop_Handle envelop_tag is %d\n", envelop_tag);

    switch(envelop_tag)
    {

        case SIMAT_DTAG_MENU_SELECTION:
            ret = SIMAT_MenuSel_Env(data,  data_len , &(stk_envelop_result_ptr->item.menu_sel_ind));

            break;

        case SIMAT_DTAG_EVENT_DOWNLOAD:
            ret = SIMAT_Event_DownLoad_Env(dual_sys, data,  data_len);
            break;

        default:
            break;
    }

    ATC_TRACE_LOW("PhoneSever_STK_Envelop_Handle ret is %d\n", ret);

    return ret;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_DisplayText
(
    uint8 *data,
    uint16 data_len,
    SIMAT_DISPLAY_TEXT_CNF_T *display_text_cnf_ptr
)
{
    //SIMAT_DISPLAY_TEXT_CNF_T display_text_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != display_text_cnf_ptr);/*assert verified: check null pointer*/


    //  SCI_MEMSET(&display_text_cnf, 0x0, sizeof(SIMAT_DISPLAY_TEXT_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    display_text_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        display_text_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetInkey
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_INKEY_CNF_T  *get_inkey_cnf_ptr
)
{
    //SIMAT_GET_INKEY_CNF_T get_inkey_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != get_inkey_cnf_ptr);/*assert verified: check null pointer*/

    //  SCI_MEMSET(&get_inkey_cnf, 0x0, sizeof(SIMAT_GET_INKEY_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    get_inkey_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        get_inkey_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    if(data_len > 0)
    {
        gen_result = DecodeMinimalObject(SIMAT_BTAG_TEXT_STRING, data, &offset, &(get_inkey_cnf_ptr->text_str), data_len);

        if(SIMAT_TR_ME_UNABLE <= gen_result)
        {
            return FALSE;
        }
    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetInput
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_INPUT_CNF_T *get_input_cnf_ptr
)
{
    //SIMAT_GET_INPUT_CNF_T get_input_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != get_input_cnf_ptr);/*assert verified: check null pointer*/

    //SCI_MEMSET(&get_input_cnf, 0x0, sizeof(SIMAT_GET_INPUT_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    get_input_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        get_input_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    if(data_len > 0)
    {
        gen_result = DecodeMinimalObject(SIMAT_BTAG_TEXT_STRING, data, &offset, &(get_input_cnf_ptr->text_str), data_len);

        if(SIMAT_TR_ME_UNABLE <= gen_result)
        {
            return FALSE;
        }
    }

    // data += offset;
    //data_len-= offset;
    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupMenu
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_MENU_CNF_T *setup_menu_cnf_ptr
)
{
    // SIMAT_SETUP_MENU_CNF_T setup_menu_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != setup_menu_cnf_ptr);/*assert verified: check null pointer*/

    //  SCI_MEMSET(&setup_menu_cnf, 0x0, sizeof(SIMAT_SETUP_MENU_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    // data += offset;
    // data_len-= offset;

    setup_menu_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        setup_menu_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupCall
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_CALL_CNF_T *setup_call_cnf_ptr
)
{
    // SIMAT_SETUP_CALL_CNF_T setup_call_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    //SCI_MEMSET(&setup_call_cnf, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    // data += offset;
    //    data_len-= offset;
    setup_call_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        if(SIMAT_TR_ME_UNABLE == result.gen_result)
        {
            setup_call_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
        }
        else if(SIMAT_TR_NE_UNABLE == result.gen_result)
        {
            setup_call_cnf_ptr->ne_info = (SIMAT_NE_PROBLEM_E)result.add_ptr[0];
        }


    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SelectItem
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SELECT_ITEM_CNF_T *select_item_cnf_ptr
)
{
    //SIMAT_SELECT_ITEM_CNF_T select_item_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != select_item_cnf_ptr);/*assert verified: check null pointer*/

    // SCI_MEMSET(&select_item_cnf, 0x0, sizeof(SIMAT_SELECT_ITEM_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    select_item_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        select_item_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    if(data_len > 0)
    {
        gen_result = DecodeMinimalObject(SIMAT_BTAG_ITEM_ID, data, &offset, &select_item_cnf_ptr->item_id, data_len);

        if(SIMAT_TR_ME_UNABLE <= gen_result)
        {
            return FALSE;
        }
    }

    //  data += offset;
    // data_len-= offset;

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendSms
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SEND_SMS_CNF_T *send_sms_cnf_ptr
)
{
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != send_sms_cnf_ptr);/*assert verified: check null pointer*/

    // SCI_MEMSET(&send_sms_cnf, 0x0, sizeof(SIMAT_SEND_SMS_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    send_sms_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        if(SIMAT_TR_ME_UNABLE == result.gen_result)
        {

            send_sms_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

        }
        else if(SIMAT_TR_SMS_ERROR == result.gen_result)
        {
            send_sms_cnf_ptr->sms_info = result.add_ptr[0];
        }
        else if(SIMAT_TR_NE_UNABLE == result.gen_result)
        {
            send_sms_cnf_ptr->ne_info = result.add_ptr[0];
        }
    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendSs
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SEND_SS_CNF_T *send_ss_cnf_ptr
)
{

    /*未完成,需要仔细考虑*/
    // SIMAT_SEND_SS_CNF_T send_ss_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != send_ss_cnf_ptr);/*assert verified: check null pointer*/

    // SCI_MEMSET(&send_ss_cnf, 0x0, sizeof(SIMAT_SEND_SS_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    send_ss_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        if(SIMAT_TR_ME_UNABLE == result.gen_result)
        {

            send_ss_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

        }
        else if(SIMAT_TR_SS_ERROR == result.gen_result)
        {
            send_ss_cnf_ptr->ss_info = result.add_ptr[0];
        }
        else if(SIMAT_TR_NE_UNABLE == result.gen_result)
        {
            send_ss_cnf_ptr->ne_info = result.add_ptr[0];
        }
    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendUssd
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SEND_USSD_CNF_T *send_ussd_cnf_ptr
)
{
    /*未完成,需要仔细考虑*/
    //SIMAT_SEND_USSD_CNF_T send_ussd_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != send_ussd_cnf_ptr);/*assert verified: check null pointer*/

    //  SCI_MEMSET(&send_ussd_cnf, 0x0, sizeof(SIMAT_SEND_SS_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    send_ussd_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        if(SIMAT_TR_ME_UNABLE == result.gen_result)
        {

            send_ussd_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

        }
        else if(SIMAT_TR_USSD_ERROR == result.gen_result)
        {
            send_ussd_cnf_ptr->ussd_info = result.add_ptr[0];
        }
        else if(SIMAT_TR_NE_UNABLE == result.gen_result)
        {
            send_ussd_cnf_ptr->ne_info = result.add_ptr[0];
        }
    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_PlayTone
(
    uint8 *data,
    uint16 data_len,
    SIMAT_PLAY_TONE_CNF_T *play_tone_cnf_ptr
)
{
    // SIMAT_PLAY_TONE_CNF_T play_tone_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != play_tone_cnf_ptr);/*assert verified: check null pointer*/
    // SCI_MEMSET(&play_tone_cnf, 0x0, sizeof(SIMAT_PLAY_TONE_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    play_tone_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        play_tone_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_Refresh
(
    uint8 *data,
    uint16 data_len,
    SIMAT_REFRESH_CNF_T  *refresh_cnf_ptr
)
{
    // SIMAT_REFRESH_CNF_T refresh_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != refresh_cnf_ptr);/*assert verified: check null pointer*/

    // SCI_MEMSET(&refresh_cnf, 0x0, sizeof(SIMAT_DISPLAY_TEXT_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    refresh_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        refresh_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_ProLocInfo
(
    uint8 *data,
    uint16 data_len,
    SIMAT_GET_LANG_CNF_T *lang_cnf,
    SIMAT_GET_DT_TZ_CNF_T *dt_tz_cnf
)
{
    SIMAT_LOCAL_INFO_TYPE_E local_info_type = SIMAT_LI_RESERVED;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_MEMSET(lang_cnf, 0x0, sizeof(SIMAT_GET_LANG_CNF_T));
    SCI_MEMSET(dt_tz_cnf, 0x0, sizeof(SIMAT_GET_DT_TZ_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    local_info_type = (SIMAT_LOCAL_INFO_TYPE_E)(*(data + SIMAT_COMMAND_DETAIL_QULIFIER_POS));

    data += SIMAT_MIN_PC_LEN;
    data_len-= SIMAT_MIN_PC_LEN;

    if(local_info_type != SIMAT_LI_LANG &&  local_info_type != SIMAT_LI_DT_TZ)
    {
        return FALSE;
    }

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT,data,&offset,&result,data_len);
    if (SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }
    data_len -= offset;
    data+= offset;

    if (SIMAT_LI_LANG == local_info_type)
    {
        lang_cnf->result = result.gen_result;
        if (result.len>0)
        {
            lang_cnf->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
        }

        if (data_len > 0)
       {
            gen_result = DecodeMinimalObject(SIMAT_BTAG_LANGUAGE,data,&offset,&lang_cnf->lang,data_len);
            if (SIMAT_TR_ME_UNABLE <= gen_result)
            {
                return FALSE;
            }
        }
    }
    else
    {
        dt_tz_cnf->result = result.gen_result;
        if (result.len>0)
        {
            dt_tz_cnf->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
        }

        if (data_len > 0)
        {
            gen_result = DecodeMinimalObject(SIMAT_BTAG_DT_AND_TZ,data,&offset,&dt_tz_cnf->dt_tz,data_len);
            if (SIMAT_TR_ME_UNABLE <= gen_result)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupIdleText
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SETUP_IDLE_TEXT_CNF_T *setup_idle_text_cnf_ptr
)
{
    // SIMAT_SETUP_IDLE_TEXT_CNF_T setup_idle_text_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != setup_idle_text_cnf_ptr);/*assert verified: check null pointer*/

    //  SCI_MEMSET(&setup_idle_text_cnf, 0x0, sizeof(SIMAT_SETUP_IDLE_TEXT_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    setup_idle_text_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {

        setup_idle_text_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];

    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}


/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author:Bin.Li
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendDtmf
(
    uint8 *data,
    uint16 data_len,
    SIMAT_SEND_DTMF_CNF_T *send_dtmf_cnf_ptr
)
{
    //  SIMAT_SEND_DTMF_CNF_T send_dtmf_cnf;
    SIMAT_DO_RESULT_T result ;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != send_dtmf_cnf_ptr);/*assert verified: check null pointer*/

    //SCI_MEMSET(&send_dtmf_cnf, 0x0, sizeof(SIMAT_SEND_DTMF_CNF_T));
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode the decvice id
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    send_dtmf_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        send_dtmf_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    /* binli should do at+stgr here use the struct */
    /* begin */

    /* end */
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SendData(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in, the length of response data
    SIMAT_SEND_DATA_CNF_T *send_data_cnf_ptr    //out, response info of send data
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != send_data_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    send_data_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        send_data_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    if(data_len > 0)
    {
        if(result.gen_result <= SIMAT_TR_WITH_MODIFICATION)
        {
            //command performed successfully,should include channel data length
            //decode Channel data length
            gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_DATA_LEN, data, &offset, &(send_data_cnf_ptr->channel_data_len), data_len);

            if(SIMAT_TR_ME_UNABLE <= gen_result)
            {
                return FALSE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        //command performed successfully,should include channel data length
        if(result.gen_result <= SIMAT_TR_WITH_MODIFICATION)
        {
            return FALSE;
        }
    }

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_ReceiveData(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_RECEIVE_DATA_CNF_T *receive_data_cnf_ptr    //out,response info of receive data
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != receive_data_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    receive_data_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        receive_data_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    //decode Channel data
    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_DATA, data, &offset,
                                     &receive_data_cnf_ptr->channel_data, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        ATC_TRACE_LOW("11,%d", gen_result);
        return FALSE;
    }

    data += offset;
    data_len -= offset;
    //decode Channel data length
    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_DATA_LEN, data, &offset,
                                     &receive_data_cnf_ptr->channel_data_len, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        ATC_TRACE_LOW("22,%d", gen_result);
        return FALSE;
    }

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_LaunchBrowser(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_LAUNCH_BROWSER_CNF_T *launch_browser_cnf_ptr    //out,response info of launch browser
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != launch_browser_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    launch_browser_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        launch_browser_cnf_ptr->mp_info = (SIMAT_LAUNCH_BROWSER_PROBLEM_E)result.add_ptr[0];
    }


    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_OpenChannel(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_OPEN_CHANNEL_CNF_T *open_channel_cnf_ptr //out,response info of open channel
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != open_channel_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    open_channel_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        open_channel_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    if(result.gen_result <= SIMAT_TR_WITH_MODIFICATION)
    {
        //decode Channel status
        gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_STATUS, data, &offset,
                                         &open_channel_cnf_ptr->channel_status, data_len);

        if(SIMAT_TR_ME_UNABLE <= gen_result)
        {
            return FALSE;
        }

        open_channel_cnf_ptr->is_channel_status = TRUE;
        data += offset;
        data_len -= offset;
    }
    else
    {
        if((SIMAT_BTAG_CHANNEL_STATUS) != GET_TAG_VALUE(*data))
        {
            open_channel_cnf_ptr->is_channel_status = FALSE;
        }
        else
        {
            ATC_TRACE_LOW("ATC: open channel failed,should not include channel status!");
            return FALSE;
        }
    }
    
    //decode Bearer description
    gen_result = DecodeMinimalObject(SIMAT_BTAG_BEAR_DESC, data, &offset,
                                     &open_channel_cnf_ptr->bear_desc, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    open_channel_cnf_ptr->is_bear_desc = TRUE;
    data += offset;
    data_len -= offset;
    //decode Buffer size
    gen_result = DecodeMinimalObject(SIMAT_BTAG_BUFF_SIZE, data, &offset,
                                     &open_channel_cnf_ptr->buf_size, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    open_channel_cnf_ptr->is_buf_size = TRUE;
    data += offset;
    data_len -= offset;
    if(data_len > 0)    
    {
        //decode Other address
        gen_result = DecodeMinimalObject(SIMAT_BTAG_OTHER_ADDR, data, &offset,
                            &open_channel_cnf_ptr->other_address, data_len);

        if(SIMAT_TR_ME_UNABLE <= gen_result)
        {
            return FALSE;
        }

        open_channel_cnf_ptr->is_other_address = TRUE;
    }

    return TRUE;
}
/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_CloseChannel(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_CLOSE_CHANNEL_CNF_T *close_channel_cnf_ptr    //out,response info of close channel
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != close_channel_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    close_channel_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        close_channel_cnf_ptr->mp_info = (SIMAT_BEARER_INDEPENDENT_PROBLEM_E)result.add_ptr[0];
    }

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_SetupEventList(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_SETUP_EVENT_LIST_CNF_T *setup_event_list_cnf_ptr    //out,response info of setup event list
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != setup_event_list_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    setup_event_list_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        setup_event_list_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the response data from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
static  BOOLEAN SIMAT_Terminal_GetChannelStatus(
    uint8 *data,            //in,the response command data buf from APP
    uint16 data_len,        //in,the length of response data
    SIMAT_GET_CHANNEL_STATUS_CNF_T *get_channel_status_cnf_ptr    //out,response info of get channel status
)
{
    SIMAT_DO_RESULT_T result;
    SIMAT_GENERAL_PC_RESULT_E gen_result = SIMAT_TR_SUCCESS;
    uint8 offset = 0;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != get_channel_status_cnf_ptr);/*assert verified: check null pointer*/
    SCI_MEMSET(&result, 0x0, sizeof(SIMAT_DO_RESULT_T));

    //offset(do not decode) data of command details and device identities
    data += SIMAT_MIN_PC_LEN;
    data_len -= SIMAT_MIN_PC_LEN;

    //decode Result
    gen_result = DecodeMinimalObject(SIMAT_BTAG_RESULT, data, &offset, &result, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    get_channel_status_cnf_ptr->result = result.gen_result;

    if(result.len > 0)
    {
        get_channel_status_cnf_ptr->mp_info = (SIMAT_ME_PROBLEM_E)result.add_ptr[0];
    }

    //decode Channel status
    gen_result = DecodeMinimalObject(SIMAT_BTAG_CHANNEL_STATUS, data, &offset,
                                     &get_channel_status_cnf_ptr->channel_status, data_len);

    if(SIMAT_TR_ME_UNABLE <= gen_result)
    {
        return FALSE;
    }

    data += offset;
    data_len -= offset;

    return TRUE;
}

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
)
{
    uint8 *ptemp                = NULL;
    SIMAT_PC_NAME_E com_name  = SIMAT_PC_NUM;
    BOOLEAN ret = FALSE;

    SCI_ASSERT(PNULL != data);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != stk_terminal_result);/*assert verified: check null pointer*/

    ptemp = data;

    if(data_len < SIMAT_MIN_PC_LEN)
    {
        return FALSE;
    }

    com_name = ATC_GetPCName(*(ptemp + 3));

    stk_terminal_result->pc_cmd_type = com_name;

    switch(com_name)
    {
        case SIMAT_PC_REFRESH:
            ret = SIMAT_Terminal_Refresh(data, data_len, &(stk_terminal_result->item.refresh_cnf));
            break;

        case SIMAT_PC_SETUP_EVENT_LIST:
            ret = SIMAT_Terminal_SetupEventList(data, data_len, &(stk_terminal_result->item.setup_eventlist_cnf));
            break;

        case SIMAT_PC_SETUP_CALL:
            ret = SIMAT_Terminal_SetupCall(data, data_len, &(stk_terminal_result->item.setup_call_cnf));
            break;

        case SIMAT_PC_SEND_SS:
            ret = SIMAT_Terminal_SendSs(data, data_len, &(stk_terminal_result->item.send_ss_cnf));
            break;

        case SIMAT_PC_SEND_USSD:
            ret = SIMAT_Terminal_SendUssd(data, data_len, &(stk_terminal_result->item.send_ussd_cnf));
            break;

        case SIMAT_PC_SMS:
            ret = SIMAT_Terminal_SendSms(data, data_len, &(stk_terminal_result->item.send_sms_cnf));
            break;

        case SIMAT_PC_SEND_DTMF:
            ret = SIMAT_Terminal_SendDtmf(data, data_len, &(stk_terminal_result->item.send_dtmf_cnf));
            break;

        case SIMAT_PC_PLAY_TONE:
            ret = SIMAT_Terminal_PlayTone(data, data_len, &(stk_terminal_result->item.play_tone_cnf));
            break;

        case SIMAT_PC_DISPLAY_TEXT:
            ret = SIMAT_Terminal_DisplayText(data, data_len, &(stk_terminal_result->item.display_text_cnf));
            break;

        case SIMAT_PC_GET_INKEY:
            ret = SIMAT_Terminal_GetInkey(data, data_len, &(stk_terminal_result->item.get_inkey_cnf));
            break;

        case SIMAT_PC_GET_INPUT:
            ret = SIMAT_Terminal_GetInput(data, data_len, &(stk_terminal_result->item.get_input_cnf));
            break;

        case SIMAT_PC_SELECT_ITEM:
            ret = SIMAT_Terminal_SelectItem(data, data_len, &(stk_terminal_result->item.select_item_cnf));
            break;

        case SIMAT_PC_SETUP_MENU:
            ret = SIMAT_Terminal_SetupMenu(data, data_len, &(stk_terminal_result->item.setup_menu_cnf));
            break;

        case SIMAT_PC_PROVIDE_LOCATION_INFO:
            ret = SIMAT_Terminal_ProLocInfo( data, data_len,&(stk_terminal_result->item.get_lang_cnf),&(stk_terminal_result->item.get_dttz_cnf));
            break;

        case SIMAT_PC_SETUP_IDLE_MODE_TEXT:
            ret = SIMAT_Terminal_SetupIdleText(data, data_len, &(stk_terminal_result->item.idle_text_cnf));
            break;

        case SIMAT_PC_SEND_DATA:
            ret = SIMAT_Terminal_SendData(data, data_len, &(stk_terminal_result->item.send_data_cnf));
            break;

        case SIMAT_PC_RECEIVE_DATA:
            ret = SIMAT_Terminal_ReceiveData(data, data_len, &(stk_terminal_result->item.receive_data_cnf));
            break;

        case SIMAT_PC_LAUNCH_BROWSER:
            ret = SIMAT_Terminal_LaunchBrowser(data, data_len, &(stk_terminal_result->item.launch_browser_cnf));
            break;

        case SIMAT_PC_OPEN_CHANNEL:
            ret = SIMAT_Terminal_OpenChannel(data, data_len, &(stk_terminal_result->item.open_channel_cnf));
            break;

        case SIMAT_PC_CLOSE_CHANNEL:
            ret = SIMAT_Terminal_CloseChannel(data, data_len, &(stk_terminal_result->item.close_channel_cnf));
            break;

        case SIMAT_PC_GET_CHANNEL_STATUS:
            ret = SIMAT_Terminal_GetChannelStatus(data, data_len, &(stk_terminal_result->item.get_channel_status_cnf));
            break;

        default :
            ATC_TRACE_LOW("PhoneSever_STK_TerminalRsp_Handle pc name is %d ", com_name);
            break;
    }

    return ret;
}

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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(display_text_ptr->high_priority)
    {
        com_qualier |= 0x1;
    }

    if(display_text_ptr->user_clear)
    {
        com_qualier |= 0x80;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_DISPLAY_TEXT], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &display_text_ptr->text_str))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(display_text_ptr->is_icon_exist)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &display_text_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(display_text_ptr->is_ir)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_IMMEDIATE_RESPONSE, ptemp, &offset, &display_text_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(get_inkey_ptr->help_info)
    {
        com_qualier |= 0x80;
    }

    switch(get_inkey_ptr->inkey_type)
    {
        case SIMAT_DIGIT_ONLY:
            com_qualier |= 0x1;
            break;

        case SIMAT_SMS_ALPHABET:
            com_qualier |= 0x2;
            break;

        case SIMAT_YES_NO:
            com_qualier |= 0x4;
            break;

        case SIMAT_UCS2_ALPHABET:
            com_qualier |= 0x8;
            break;

        default:
            break;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_GET_INKEY], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &get_inkey_ptr->text_str))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(get_inkey_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &get_inkey_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(get_input_ptr->help_info)
    {
        com_qualier |= 0x80;
    }

    switch(get_input_ptr->input_type)
    {
        case SIMAT_INPUT_DIGIT_ONLY:
            com_qualier |= 0x0;
            break;

        case SIMAT_INPUT_SMS_ALPHABET:
            com_qualier |= 0x1;
            break;

        case SIMAT_INPUT_UCS2_ALPHABET:
            com_qualier |= 0x3;
            break;

        case SIMAT_INPUT_DUAL_DIGIT:
            com_qualier |= 0x2;
            break;

        default:
            break;
    }

    if(get_input_ptr->is_packed)
    {
        com_qualier |= 0x8;
    }

    if(!get_input_ptr->input_echo)
    {
        com_qualier |= 0x4;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_GET_INPUT], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &get_input_ptr->text_str))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_RESPONSE_LEN, ptemp, &offset, &get_input_ptr->response_len))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(get_input_ptr->is_default_text)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &get_input_ptr->default_text))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(get_input_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &get_input_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint32 loop = 0;
    SIMAT_ITEMTEMPLATE_T item_template;
    uint8 menuitem_offset = 0;
    uint8 offset = 0;

    SCI_MEMSET(&item_template, 0x0, sizeof(SIMAT_ITEMTEMPLATE_T));
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(setup_menu_ptr->help_info)
    {
        com_qualier |= 0x80;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SETUP_MENU], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &setup_menu_ptr->alpha_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    for(loop = 0; loop < setup_menu_ptr->item.item_data_object_num; loop++)
    {
        item_template.id = setup_menu_ptr->item.item_data_object_info[loop][0];
        item_template.len = setup_menu_ptr->item.item_data_object_info[loop][1];
        menuitem_offset = setup_menu_ptr->item.item_data_object_info[loop][2];
        SCI_MEMCPY(item_template.item_content_entry_buf,
                   setup_menu_ptr->item.item_data_object_buf + menuitem_offset,
                   item_template.len);

        if(!ATC_EncodeDataObject(SIMAT_BTAG_ITEM, ptemp, &offset, &item_template))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_menu_ptr->is_action_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ITEM_NEXT_ACTION_INDICATOR, ptemp, &offset, &setup_menu_ptr->next_action))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_menu_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &setup_menu_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_menu_ptr->is_action_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID_LIST, ptemp, &offset, &setup_menu_ptr->icon_list))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    com_qualier = (uint8)setup_call_ptr->call_parameter.call_type;

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SETUP_CALL], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(setup_call_ptr->is_uc_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &setup_call_ptr->uc_alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(!ATC_EncodeDataObject(SIMAT_BTAG_ADDRESS, ptemp, &offset, &setup_call_ptr->call_parameter.address))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(setup_call_ptr->call_parameter.is_ccp)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_CCP, ptemp, &offset, &setup_call_ptr->call_parameter.ccp))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_call_ptr->call_parameter.is_cps)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_CPS, ptemp, &offset, &setup_call_ptr->call_parameter.cps))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_call_ptr->call_parameter.is_duration)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_DURATION, ptemp, &offset, &setup_call_ptr->call_parameter.duration))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_call_ptr->is_uc_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &setup_call_ptr->uc_icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }


    if(setup_call_ptr->is_cs_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &setup_call_ptr->cs_alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(setup_call_ptr->is_cs_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &setup_call_ptr->cs_icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint32 loop = 0;
    SIMAT_ITEMTEMPLATE_T item_template;
    uint8 menuitem_offset = 0;
    uint8 offset = 0;

    SCI_MEMSET(&item_template, 0x0, sizeof(SIMAT_ITEMTEMPLATE_T));
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(select_item_ptr->speci_type)
    {
        com_qualier |= 0x1;
    }

    if(select_item_ptr->data_option)
    {
        com_qualier |= 0x2;
    }

    if(select_item_ptr->help_info)
    {
        com_qualier |= 0x80;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SELECT_ITEM], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(select_item_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &select_item_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    for(loop = 0; loop < select_item_ptr->item.item_data_object_num; loop++)
    {
        item_template.id = select_item_ptr->item.item_data_object_info[loop][0];
        item_template.len = select_item_ptr->item.item_data_object_info[loop][1];
        menuitem_offset = select_item_ptr->item.item_data_object_info[loop][2];
        SCI_MEMCPY(item_template.item_content_entry_buf,
                   select_item_ptr->item.item_data_object_buf + menuitem_offset,
                   item_template.len);

        if(!ATC_EncodeDataObject(SIMAT_BTAG_ITEM, ptemp, &offset, &item_template))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(select_item_ptr->is_action_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ITEM_NEXT_ACTION_INDICATOR, ptemp, &offset, &select_item_ptr->next_action))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(select_item_ptr->is_item_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ITEM_ID, ptemp, &offset, &select_item_ptr->item_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(select_item_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &select_item_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(select_item_ptr->is_icon_list)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID_LIST, ptemp, &offset, &select_item_ptr->icon_list))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}


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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(send_sms_ptr->is_packet_IND)
    {
        com_qualier |= 0x1;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SMS], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_NETWORK;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(send_sms_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &send_sms_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(send_sms_ptr->is_address)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ADDRESS, ptemp, &offset, &send_sms_ptr->address))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(!ATC_EncodeDataObject(SIMAT_BTAG_SMS_TPDU, ptemp, &offset, &send_sms_ptr->tpdu))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(send_sms_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &send_sms_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SEND_SS], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_NETWORK;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(send_ss_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &send_ss_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }


    if(!ATC_EncodeDataObject(SIMAT_BTAG_SS_STRING, ptemp, &offset, &send_ss_ptr->ss))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(send_ss_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &send_ss_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SEND_USSD], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_NETWORK;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(send_ussd_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &send_ussd_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(!ATC_EncodeDataObject(SIMAT_BTAG_USSD_STRING, ptemp, &offset, &send_ussd_ptr->ussd))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(send_ussd_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &send_ussd_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}

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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;

    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_PLAY_TONE], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(play_tone_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &play_tone_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(play_tone_ptr->is_tone_exist)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_TONE, ptemp, &offset, &play_tone_ptr->tone))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(play_tone_ptr->is_duration_exist)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_DURATION, ptemp, &offset, &play_tone_ptr->duration))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}

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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SETUP_EVENT_LIST], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_EVENT_LIST, ptemp, &offset, &setup_event_list->event_list))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;
    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }

    //encode command details
    com_qualier = (uint8)refresh_ptr->refresh_type;

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_REFRESH], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode file list
    if(refresh_ptr->is_files_exist)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_FILE_LIST, ptemp, &offset, &refresh_ptr->file_list))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }


    com_qualier = (uint8)(*local_info_type_ptr);

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_PROVIDE_LOCATION_INFO], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;
    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }


    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SETUP_IDLE_MODE_TEXT], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &setup_idle_text_ptr->text_string))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(setup_idle_text_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &setup_idle_text_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}


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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if((NULL == data)  || (NULL == data_len_ptr))
    {
        return FALSE;
    }


    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SEND_DTMF], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;

    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_NETWORK;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    if(send_dtmf_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &send_dtmf_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DTMF_STRING, ptemp, &offset, &send_dtmf_ptr->dtmf))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    if(send_dtmf_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &send_dtmf_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc  tag D0*/
    //*temp = SIMAT_UTAG_PC;
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}


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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if(NULL == openchannel_ptr || NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    if(SIMAT_CHANNEL_LINK_ESTABLISH_IMMEDIATE == openchannel_ptr->link_establish_type)
    {
        com_qualier |= 0x1;
    }

    if(SIMAT_CHANNEL_RECONNECT_AUTOMATIC == openchannel_ptr->reconnect_type)
    {
        com_qualier |= (0x1 << 1);
    }

    if(SIMAT_CHANNEL_LINK_ESTABLISH_IMMEDIATE_BACKGROUND == openchannel_ptr->background_mode)
    {
        com_qualier |= (0x1 << 2);
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_OPEN_CHANNEL], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode alpha identifier
    if(openchannel_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &openchannel_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode icon identifier
    if(openchannel_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &openchannel_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Bearer description
    if(!ATC_EncodeDataObject(SIMAT_BTAG_BEAR_DESC, ptemp, &offset, &openchannel_ptr->bear_desc))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode buffer size
    if(!ATC_EncodeDataObject(SIMAT_BTAG_BUFF_SIZE, ptemp, &offset, &openchannel_ptr->buf_size))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode Network Access Name
    if(openchannel_ptr->is_net_access_name)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_NETWORK_ACCESS_NAME, ptemp, &offset, &openchannel_ptr->net_access_name))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Other address (local address)
    if(openchannel_ptr->is_other_address)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_OTHER_ADDR, ptemp, &offset, &openchannel_ptr->other_address))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Text String (User login)
    if(openchannel_ptr->is_text_str_login)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &openchannel_ptr->text_str_login))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Text String (User password)
    if(openchannel_ptr->is_text_str_pwd)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &openchannel_ptr->text_str_pwd))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode UICC/terminal interface transport level
    if(openchannel_ptr->is_trans_level)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_INTERFACE_TRANS_LEVEL, ptemp, &offset, &openchannel_ptr->trans_level))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Data destination address
    if(openchannel_ptr->is_data_dest_addr)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_OTHER_ADDR, ptemp, &offset, &openchannel_ptr->data_dest_address))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);

    return TRUE;
}

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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);

    ptemp = temp_data;

    if(NULL == closechannel_ptr || NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    // note: com_qualier == 0x0 is RFU
    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_CLOSE_CHANNEL], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = (SIMAT_DEVICE_ID_E)(closechannel_ptr->channel_id - 1 + SIMAT_DE_CHANNEL1);   //0x21 - 0x27

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode alpha identifier
    if(closechannel_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &closechannel_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode icon identifier
    if(closechannel_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &closechannel_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }


    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;
    uint32  i;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);
    ptemp = temp_data;

    if(NULL == lauchbrowser_ptr || NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    com_qualier = (uint8)lauchbrowser_ptr->launch_browser_type;

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_LAUNCH_BROWSER], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode browser identity
    if(lauchbrowser_ptr->is_browser_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_BROWSER_ID, ptemp, &offset, &lauchbrowser_ptr->browser_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode URL
    if(!ATC_EncodeDataObject(SIMAT_BTAG_URL, ptemp, &offset, &lauchbrowser_ptr->url))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode bearer
    if(lauchbrowser_ptr->is_bear)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_BEARER, ptemp, &offset, &lauchbrowser_ptr->bear))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Provisioning File Reference
    for(i = 0; i < lauchbrowser_ptr->file_path_num; i++)
    {
        SCI_ASSERT(i < MN_SIMAT_FILE_PATH_NUM);/*assert verified: check valid value*/

        if(!ATC_EncodeDataObject(SIMAT_BTAG_PROVISION_PRE, ptemp, &offset, &lauchbrowser_ptr->provision_file_path[i]))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode text string
    if(lauchbrowser_ptr->is_gate_way)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_TEXT_STRING, ptemp, &offset, &lauchbrowser_ptr->gate_way))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Alpha identifier (user confirmation phase)
    if(lauchbrowser_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &lauchbrowser_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode Icon identifier (user confirmation phase)
    if(lauchbrowser_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &lauchbrowser_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }


    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);
    ptemp = temp_data;

    if(NULL == senddata_ptr || NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    if(senddata_ptr->is_send_immediate)
    {
        com_qualier |= 0x1;
    }

    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_SEND_DATA], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = (SIMAT_DEVICE_ID_E)(senddata_ptr->channel_id - 1 + SIMAT_DE_CHANNEL1);   //0x21 - 0x27

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode alpha identifier
    if(senddata_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &senddata_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode icon identifier
    if(senddata_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &senddata_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode channel data
    if(!ATC_EncodeDataObject(SIMAT_BTAG_CHANNEL_DATA, ptemp, &offset, &senddata_ptr->channel_data))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}
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
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);
    ptemp = temp_data;

    if(NULL == receivedata_ptr || NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    // note: com_qualier == 0x0 is RFU
    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_RECEIVE_DATA], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = (SIMAT_DEVICE_ID_E)(receivedata_ptr->channle_id - 1 + SIMAT_DE_CHANNEL1);   //0x21 - 0x27

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    //encode alpha identifier
    if(receivedata_ptr->is_alpha_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ALPHA_ID, ptemp, &offset, &receivedata_ptr->alpha_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode icon identifier
    if(receivedata_ptr->is_icon_id)
    {
        if(!ATC_EncodeDataObject(SIMAT_BTAG_ICON_ID, ptemp, &offset, &receivedata_ptr->icon_id))
        {
            return FALSE;
        }

        MAKE_COMPREHESION(*ptemp);
        ptemp += offset;
        data_len += offset;
    }

    //encode channel data
    if(!ATC_EncodeDataObject(SIMAT_BTAG_CHANNEL_DATA_LEN, ptemp, &offset, &receivedata_ptr->channel_data_len))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;


    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}

/********************************************************************
//  DESCRIPTION
//      process the envelop  data bug get from the APP
//  Global resource dependence :
//  Author: minqian.qian
//  Note:
***********************************************************************/
BOOLEAN SIMAT_Proactive_GetChannelStatusInd(
    uint8   *data,
    uint16  *data_len_ptr
)
{
    uint8 *ptemp = NULL;
    uint16 data_len = 0;
    uint8 com_qualier = 0;
    uint8 temp_data[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    SIMAT_DO_DEVICE_ID_T device_id;
    uint8 offset = 0;

    SCI_MEMSET(temp_data, 0x0, MN_MAX_SIMAT_DATA_OBJECT_LEN);
    ptemp = temp_data;

    if(NULL == data || NULL == data_len_ptr)
    {
        return FALSE;
    }

    //encode command details
    // note: com_qualier == 0x0 is RFU
    if(!EncodePCCommand(0x1, s_simat_pc_tag_array[SIMAT_PC_GET_CHANNEL_STATUS], com_qualier, ptemp, &data_len))
    {
        return FALSE;
    }

    ptemp += data_len;
    //encode device identities
    device_id.source = SIMAT_DE_SIM;
    device_id.destination = SIMAT_DE_ME;

    if(!ATC_EncodeDataObject(SIMAT_BTAG_DEVICE_ID, ptemp, &offset, &device_id))
    {
        return FALSE;
    }

    MAKE_COMPREHESION(*ptemp);
    ptemp += offset;
    data_len += offset;

    *data_len_ptr = data_len;

    /* encode pc tag D0*/
    *data = (uint8)SIMAT_UTAG_PC;
    data += 1;
    *data_len_ptr += 1;
    SIMAT_SetTLVLength(data, &offset, data_len);
    data += offset;
    *data_len_ptr += offset;

    SCI_MEMCPY(data, temp_data, data_len);
    return TRUE;
}
#ifdef __cplusplus
}
#endif
