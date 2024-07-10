/******************************************************************************
 ** File Name:      atc_gsm_stk.c                                             *
 ** Author:         Liuhan                                                    *
 ** DATE:           12/08/2003                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the functions that handle the AT       *
 **                 command regarding STK(SIM TOOLKIT)                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/08/2003     Liuhan           Create.                                   *
 ******************************************************************************/
 
#if 0
#include "base_trc.h"
#include "atc_gsm_stk.h"
#include "atc_common.h"
#include "atc_gsm_ss.h"

#ifndef _ULTRA_LOW_CODE_
#include "sig_code.h"
#endif

#include "env_atc_signal.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
 #ifndef _ULTRA_LOW_CODE_
 
static STK_SINAL_T          s_stk_signal[MN_SYS_NUMBER] = {0};

#ifdef _ATC_ONLY
static uint8                s_cmd_stag[MN_SYS_NUMBER] = {0};
#endif

#ifndef _MUX_ENABLE_
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

static uint8                s_mainmenu_selitem[MN_SYS_NUMBER] = {0};

static BOOLEAN              s_is_setup_menu_cnf[MN_SYS_NUMBER]= {0};
static uint32               s_cmd_type[MN_SYS_NUMBER] = {0};
static uint32               s_result[MN_SYS_NUMBER] ={0};
static uint32               s_first_stk_cmd[MN_SYS_NUMBER] ={0};
uint16                      s_setup_menu_len[MN_DUAL_SYS_MAX];
int16                       s_setup_menu_request_cnt[MN_DUAL_SYS_MAX]={0};
uint8                       s_setup_menu_data[MN_DUAL_SYS_MAX][SIMAT_MAX_RPDU_DATA_LEN];

static const char*       s_stk_prompt_str[AT_STK_STR_MAX] =
{
    "Send Short Message",
    "Send SS",
    "Send USSD"
};
typedef struct
{
    BOOLEAN    is_packet_IND;    //true:sms packeting by ME required,false:packeting not required
    BOOLEAN    is_address;          //is the address object exist in the command
    MN_CALLED_NUMBER_T    address;    //the address object
    MN_L3_MSG_UNIT_T        tpdu;          //the TPDU object
    uint16         cmd_id;
}ATC_STK_SEND_SM_T;

static ATC_STK_SEND_SM_T s_stk_send_sms[MN_DUAL_SYS_MAX] = {0};
#define ATC_STK_MIN_SMS_LEN     7

extern const ATC_INFO_T   g_atc_info_table[];
extern RSP_RESULT_STR     g_rsp_str;
extern MN_CALLED_NUMBER_T g_sc_addr[MN_SYS_NUMBER];
extern ATC_CALLS_STATE_T  g_calls_state[MN_SYS_NUMBER];


BOOLEAN                   g_is_stk_sendsms[MN_SYS_NUMBER] = {0};
BOOLEAN                   g_is_stk_call[MN_SYS_NUMBER] = {0};
char *                    g_stk_terminal_profile[AT_STK_TRP_NUM_MAX] =
{
    "160060C01F",
    "5FFFFFFF7F"
};

static SCI_TMID                 T_ATC_STSF_EXP_IND[MN_SYS_NUMBER] = {PNULL};
static BOOLEAN           s_stk_refresh_protect[MN_DUAL_SYS_MAX] = {0};
#define                  ATC_STK_MAX_FILE_LIST_LEN    245

/*****************************************************************************/
//  Description : process stk pdu mode sms and send it
//  Global resource dependence : s_stk_send_sms[]
//  Author: Elvis Xu
//  Note: 
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandleStkPduModeSms(
             ATC_CONFIG_T    *atc_config_ptr,
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             );
#else
LOCAL BOOLEAN HandleStkPduModeSms(
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             );
#endif/*_MUX_ENABLE_*/

/*****************************************************************************/
//     Description : This function initializes the static variables of phonebook
//    Global resource dependence : 
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
void ATC_InitStkVar(
                    ATC_CONFIG_T        *atc_config_ptr 
                    )
{
    uint8 i = 0;

    for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
#ifdef _ATC_ONLY
        s_cmd_stag[i]                     = ATC_CMD_INIT;
#endif

        s_mainmenu_selitem[i]      = 0;

        s_is_setup_menu_cnf[i]     = FALSE;
        s_cmd_type[i]                     = 0;
        s_result[i]                             = 0;
        g_is_stk_sendsms[i]           = FALSE;
        g_is_stk_call[i]                    = FALSE;
        s_first_stk_cmd[i]               = 0;
        
        SCI_MEMSET(&s_stk_signal[i],0,sizeof(s_stk_signal[i]));
    }

}

#if 0
/*****************************************************************************/
//     Description : process when received the setup menu signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSetupMenu(
                                ATC_CONFIG_T *atc_config_ptr
                                )
{
    uint32  i         = 0;
    uint8   ucs2_code = 0;
    uint8   start_pos   = 0;
    uint8   menu_len    = 0;
    uint8   menu_str[MN_MAX_SIMAT_DATA_OBJECT_LEN];
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    SIMAT_SETUP_MENU_T *mainmenu_ptr = &(s_stk_signal[dual_sys].setup_menu);

    // the title of main menu
    menu_len = mainmenu_ptr->alpha_id.len;
    if (0 < menu_len)
    {
        ucs2_code = mainmenu_ptr->alpha_id.id_ptr[0];
        
        if (ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //Change to ascii format.
            ATC_TranUint8IntoChar(mainmenu_ptr->alpha_id.id_ptr,
                                  menu_len, menu_str);
        }
        else
        {
            SCI_MEMCPY(menu_str, mainmenu_ptr->alpha_id.id_ptr, menu_len);
        }
        sprintf((char*)g_rsp_str,"%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr, 
                menu_str);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }

    //Show the item of main menu
    for (i=0; i<mainmenu_ptr->item.item_data_object_num; i++)
    {
        start_pos = mainmenu_ptr->item.item_data_object_info[i][2];
        menu_len  = mainmenu_ptr->item.item_data_object_info[i][1];
        ucs2_code = mainmenu_ptr->item.item_data_object_buf[start_pos];
        if (ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //change UCS2 code to ascii code.            
            ATC_TranUint8IntoChar(&(mainmenu_ptr->item.item_data_object_buf[start_pos]),
                                  menu_len, menu_str);
        }
        else
        {
            SCI_MEMCPY(menu_str, &(mainmenu_ptr->item.item_data_object_buf[start_pos]), menu_len);

        }
        sprintf((char*)g_rsp_str, "%s: %ld,%d,\"%s\",%d",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                i+1, mainmenu_ptr->item.item_data_object_num, 
                menu_str,
                mainmenu_ptr->help_info);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }    

}

/*****************************************************************************/
//     Description : process when received the display text signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessDisplayText(
                                  ATC_CONFIG_T *atc_config_ptr
                                  )
{
    BOOLEAN  user_clear;     //wait for the user to clear the message or clear the message
                            //after a while
    BOOLEAN  is_ir;          //is this command a immediate response
    uint16   str_len = 0;
    uint8    * str_ptr = PNULL;
    SIMAT_DISPLAY_TEXT_CNF_T display_text;    
    uint16   data_length = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    user_clear = s_stk_signal[dual_sys].item.display_text.user_clear;
    is_ir = s_stk_signal[dual_sys].item.display_text.is_ir; 
    //If command is a immediate response,handle it at once.
    if(is_ir)
    {
        display_text.result = SIMAT_TR_SUCCESS;        
        SIMAT_CnfDisplayTextEx(dual_sys, &display_text);  
    }
    //Get the text from s_stk_signal[dual_sys]
    str_len = s_stk_signal[dual_sys].item.display_text.text_str.len;
    if(str_len < 1)
    {
        str_len = 1;
    }
    if(str_len > STK_MAX_STR_LEN)
    {
        str_len = STK_MAX_STR_LEN;
    }
    //Copy the text according to its dcs
    switch(s_stk_signal[dual_sys].item.display_text.text_str.dcs)
    {
    case STK_DEFAULT_ALPHABET:
    case STK_UNPACKED_FORMAT:
        //SCI_TRACE_LOW("ATC:ATC_ProcessDisplayText:STK_DEFAULT_ALPHABET");
        sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.display_text.high_priority,
            (char*)s_stk_signal[dual_sys].item.display_text.text_str.text_ptr
            ,user_clear);
        break;

    case STK_PACKED_FORMAT:
        //SCI_TRACE_LOW("ATC:ATC_ProcessDisplayText:STK_PACKED_FORMAT");
        //Allocate the memory
        str_ptr = SCI_ALLOC_BASE(STK_MAX_STR_LEN);
        data_length = SIMAT_UnpackSmsAlphabet(s_stk_signal[dual_sys].item.display_text.text_str.text_ptr, 
            str_len, (uint8 *)str_ptr, STK_MAX_STR_LEN);
        if(data_length > STK_MAX_STR_LEN)
        {
            //SCI_TRACE_LOW:"ATC: error length in simat."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_268_112_2_17_22_17_26_692,(uint8*)"");
        }
        
        sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.display_text.high_priority,
            (char*)str_ptr,user_clear);
        SCI_FREE(str_ptr);
        break;

    case STK_UCS2_ALPHABET:
        //SCI_TRACE_LOW("ATC:ATC_ProcessDisplayText:STK_UCS2_ALPHABET");
        //allocate the memory
        str_ptr = SCI_ALLOC_BASEZ((str_len+1)*2);
        
        ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.display_text.text_str.text_ptr,
            str_len,str_ptr);
        sprintf((char*)g_rsp_str, "%s: %d,\"80%s\",%d",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.display_text.high_priority,str_ptr,user_clear);
        SCI_FREE(str_ptr);
        break;

    default:

        break;
    }
    //Show the text
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    //SCI_TRACE_LOW("ATC:ATC_ProcessDisplayText,%d,%d",
}

/*****************************************************************************/
//     Description : process when received the get inkey signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessGetInkey(
                               ATC_CONFIG_T *atc_config_ptr
                               )
{
    uint16   str_len;
    uint8*   str_ptr = NULL;
    uint16   data_length = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //check the length of string
    str_len = s_stk_signal[dual_sys].item.get_inkey.text_str.len;
    if(str_len < 1)
    {
        str_len = 1;
    }
    str_ptr = (uint8*)SCI_ALLOC_BASEZ((str_len+1)*2);
    
    //Copy the text according to its dcs
    switch(s_stk_signal[dual_sys].item.get_inkey.text_str.dcs)
    {
    case STK_DEFAULT_ALPHABET:
    case STK_UNPACKED_FORMAT:
        //SCI_TRACE_LOW("ATC:ATC_ProcessGetInkey:STK_DEFAULT_ALPHABET");
        sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.get_inkey.inkey_type,s_stk_signal[dual_sys].item.get_inkey.help_info,
            (char*)s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr);
        break;

    case STK_PACKED_FORMAT:        
        data_length = SIMAT_UnpackSmsAlphabet(s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr, 
            str_len, str_ptr, STK_MAX_STR_LEN);
        if(data_length > STK_MAX_STR_LEN)
        {
            //SCI_TRACE_LOW:"ATC: error data length in simat"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_338_112_2_17_22_17_26_693,(uint8*)"");
        }
        
        sprintf((char*)g_rsp_str, "%s: %d,%d,\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.get_inkey.inkey_type,s_stk_signal[dual_sys].item.get_inkey.help_info,
            (char*)str_ptr);
        break;

    case STK_UCS2_ALPHABET:
        //SCI_TRACE_LOW("ATC:ATC_ProcessGetInkey:STK_UCS2_ALPHABET");
        //change the uint8 to ASCII format
        ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr,
            str_len,str_ptr);
        sprintf((char*)g_rsp_str, "%s: %d,%d,\"80%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.get_inkey.inkey_type,
            s_stk_signal[dual_sys].item.get_inkey.help_info,
            str_ptr);
        break;

    default:

        break;
    }
    
    SCI_FREE(str_ptr);
    
    //Show the text
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    //SCI_TRACE_LOW:"ATC:ATC_ProcessGetInkey,%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_369_112_2_17_22_17_26_694,(uint8*)"dd",s_stk_signal[dual_sys].item.get_inkey.text_str.dcs,str_len);
}

/*****************************************************************************/
//     Description : process when received the get input signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessGetInput(
                               ATC_CONFIG_T *atc_config_ptr
                               )
{    
    uint16   str_len = 0;
    uint8*   str_ptr = NULL;
    uint8    dcs;
    uint8*   text_ptr;
    uint16   data_length = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(s_first_stk_cmd[dual_sys] == APP_MN_SIMAT_GET_INPUT_IND)
    {
        dcs = s_stk_signal[dual_sys].item.get_input.default_text.dcs;
        text_ptr = s_stk_signal[dual_sys].item.get_input.default_text.text_ptr;
        str_len = s_stk_signal[dual_sys].item.get_input.default_text.len;
    }
    else
    {
        dcs = s_stk_signal[dual_sys].item.get_input.text_str.dcs;
        text_ptr = s_stk_signal[dual_sys].item.get_input.text_str.text_ptr;
        str_len = s_stk_signal[dual_sys].item.get_input.text_str.len;
    }
    //SCI_TRACE_LOW("ATC: process get input dcs = %d, str_len = %d",dcs,str_len);
    
    str_ptr = (uint8*)SCI_ALLOC_BASEZ((str_len+1)*2);
    
    //Check the length of string
    if(str_len < 1)
    {
        str_len = 1;
    }
        
    //Copy the text according to its dcs
    switch(dcs)
    {
    case STK_DEFAULT_ALPHABET:
    case STK_UNPACKED_FORMAT:
        {
            //SCI_TRACE_LOW("ATC:ATC_ProcessGetInput:STK_DEFAULT_ALPHABET");
            ConvertBinToHex(text_ptr, str_len, str_ptr);

            sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d,%d,\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,s_stk_signal[dual_sys].item.get_input.input_type,
                s_stk_signal[dual_sys].item.get_input.input_echo,s_stk_signal[dual_sys].item.get_input.response_len.min_len,
                s_stk_signal[dual_sys].item.get_input.response_len.max_len,s_stk_signal[dual_sys].item.get_input.help_info,
                (char*)str_ptr);
        }
        break;

    case STK_PACKED_FORMAT:
        //Unpack the prompting string        
        data_length = SIMAT_UnpackSmsAlphabet(text_ptr, str_len, (uint8 *)str_ptr, STK_MAX_STR_LEN);
        if(data_length > STK_MAX_STR_LEN)
        {
            //SCI_TRACE_LOW:"ATC: error length in simat."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_431_112_2_17_22_17_26_695,(uint8*)"");
        }
        
        sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d,%d,\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,s_stk_signal[dual_sys].item.get_input.input_type,
            s_stk_signal[dual_sys].item.get_input.input_echo,s_stk_signal[dual_sys].item.get_input.response_len.min_len,
            s_stk_signal[dual_sys].item.get_input.response_len.max_len,s_stk_signal[dual_sys].item.get_input.help_info,
            (char*)str_ptr);
        break;

    case STK_UCS2_ALPHABET:
        //SCI_TRACE_LOW("ATC:ATC_ProcessGetInput:STK_UCS2_ALPHABET");
        //change the uint8 to ASCII format
        ATC_TranUint8IntoChar(text_ptr, str_len, str_ptr);
        sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%d,%d,\"80%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.get_input.input_type,
            s_stk_signal[dual_sys].item.get_input.input_echo,s_stk_signal[dual_sys].item.get_input.response_len.min_len,
            s_stk_signal[dual_sys].item.get_input.response_len.max_len,s_stk_signal[dual_sys].item.get_input.help_info,str_ptr);
        break;

    default:

        break;
    }
    
    SCI_FREE(str_ptr);
    
    //Show the text
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}

/*****************************************************************************/
//     Description : process when received the setup call signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSetupCall(
                                ATC_CONFIG_T *atc_config_ptr
                                )
{
    uint8   tel_str[MN_MAX_ADDR_BCD_LEN*2+1] = {0};
    uint8   tel_sub_addr[MN_MAX_ADDR_BCD_LEN*2+1] = {0};
    uint8   tel_num_len;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW("ATC: enter ATC_ProcessSetupCall");

    //Check the length of telphone number.
    if(s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len > MN_MAX_ADDR_BCD_LEN)
    {
        tel_num_len = MN_MAX_ADDR_BCD_LEN;
    }
    else
    {
        tel_num_len = s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len;
    }
    //Change the BCD code to sring.
    TransformBcdToStr(s_stk_signal[dual_sys].item.setup_call.call_parameter.address.party_num,
        tel_num_len,tel_str);
    //There is sub address in call number.
    if(s_stk_signal[dual_sys].item.setup_call.call_parameter.is_cps)
    {
        //Check the length of telphone number.
        if(s_stk_signal[dual_sys].item.setup_call.call_parameter.cps.addr_len > MN_MAX_ADDR_BCD_LEN)
        {
            tel_num_len = MN_MAX_ADDR_BCD_LEN;
        }
        else
        {
            tel_num_len = s_stk_signal[dual_sys].item.setup_call.call_parameter.cps.addr_len;
        }
        //Change the BCD code to sring.
        TransformBcdToStr(s_stk_signal[dual_sys].item.setup_call.call_parameter.cps.sub_num,
            tel_num_len,tel_sub_addr);
        sprintf((char*)g_rsp_str,"%s: %d,\"%s\",\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.setup_call.call_parameter.call_type,tel_str,tel_sub_addr);
    }
    //No sub address.
    else
    {
        sprintf((char*)g_rsp_str,"%s: %d,\"%s\"",
            g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
            s_stk_signal[dual_sys].item.setup_call.call_parameter.call_type,tel_str);
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}

/*****************************************************************************/
//     Description : process when received the select item signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSelectItem(
                                 ATC_CONFIG_T *atc_config_ptr
                                 )
{
    uint8* str_ptr = NULL;
    uint8  last_item_length;
    uint8  last_item_offset;
        
    SIMAT_DO_ITEM_T  item_ptr;
    uint16 string_length       = 0;
    uint8  i                   = 0;
    uint8  ucs2_code           = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    s_stk_signal[dual_sys].menu_item_num = 0;
    SCI_MEMSET(s_stk_signal[dual_sys].menu_item_id,0,sizeof(s_stk_signal[dual_sys].menu_item_id));

    //Show the title
    if(s_stk_signal[dual_sys].item.select_item.is_alpha_id)
    {
        //SCI_TRACE_LOW("ATC:select_item.is_alpha_id==TRUE");
        string_length = s_stk_signal[dual_sys].item.select_item.alpha_id.len;
        if(string_length < 1)
        {
            string_length = 1;
        }
        
        str_ptr = (uint8*)SCI_ALLOC_BASEZ((string_length+1)*2);

        ucs2_code = s_stk_signal[dual_sys].item.select_item.alpha_id.id_ptr[0];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //SCI_TRACE_LOW("ATC:select_item.is_alpha_id.id_ptr[0]==ATC_UCS2_CODE");
            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.select_item.alpha_id.id_ptr,
                string_length,str_ptr);
            sprintf((char*)g_rsp_str,"%s: %d,\"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_mainmenu_selitem[dual_sys],str_ptr);
        }
        else
        {
            //SCI_TRACE_LOW("ATC:select_item.is_alpha_id.id_ptr[0]!=ATC_UCS2_CODE");
            sprintf((char*)g_rsp_str,"%s: %d,\"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_mainmenu_selitem[dual_sys],s_stk_signal[dual_sys].item.select_item.alpha_id.id_ptr);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        
        SCI_FREE(str_ptr);
    }

    item_ptr = s_stk_signal[dual_sys].item.select_item.item;

    //show the item
    //for(i = 1;i < item_ptr.item_data_object_num;i++)
    for(i = 0;i < item_ptr.item_data_object_num;i++)
    {    
        string_length = item_ptr.item_data_object_info[i][1];
        if(i==0)
        {
          last_item_length=0;
          last_item_offset=0;
        }
        else
        {
          last_item_length = item_ptr.item_data_object_info[i-1][1];
          last_item_offset = item_ptr.item_data_object_info[i-1][2];
        }
        
        s_stk_signal[dual_sys].menu_item_id[i] = item_ptr.item_data_object_info[i][0];
        if(string_length < 1)
        {
            string_length = 1;
        }
        
        str_ptr = (uint8*)SCI_ALLOC_BASEZ((string_length+1)*2);

        ucs2_code = item_ptr.item_data_object_buf[last_item_offset + last_item_length];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(&item_ptr.item_data_object_buf[last_item_offset + last_item_length],
                string_length,str_ptr);
            sprintf((char*)g_rsp_str,"%s: %d,%d,\"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                i+1,item_ptr.item_data_object_num,str_ptr);
        }
        else
        {
            SCI_MEMCPY(str_ptr,&item_ptr.item_data_object_buf[last_item_offset + last_item_length],string_length);
            *(str_ptr+string_length)='\0';
            sprintf((char*)g_rsp_str,"%s: %d,%d,\"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                i+1,item_ptr.item_data_object_num,str_ptr);
            //SCI_TRACE_LOW("ATC:ATC_UCS2_CODE != *(item_ptr.item_data_object_buf[last_item_offset + last_item_length])");
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        SCI_FREE(str_ptr);
    }
    
    s_stk_signal[dual_sys].menu_item_num = item_ptr.item_data_object_num;
}

/*****************************************************************************/
//     Description : process when received the send sms signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSendSMS(
                              ATC_CONFIG_T *atc_config_ptr
                              )
{
    MN_CALLED_NUMBER_T    sc_addr;//The address of SMS
    MN_L3_MSG_UNIT_T    tpdu;  
    uint16              str_len = 0;
    char                * str_ptr = PNULL;
    ERR_MNSMS_CODE_E    err_code;
    uint8               ucs2_code = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //The address of SMS service exist
    if (s_stk_signal[dual_sys].item.send_sms.is_address) 
    {
        sc_addr = s_stk_signal[dual_sys].item.send_sms.address;
        
        if(sc_addr.num_len < 1)
        {
            SCI_TRACE_HIGH("ATC: ATC_ProcessSendSMS sc_addr.length is 0");
            return;
        }
        if(sc_addr.num_len > MN_SMS_ADDR_MAX_LEN)
        {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessSendSMS sc_addr.length = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_656_112_2_17_22_17_27_696,(uint8*)"d",sc_addr.num_len);
            sc_addr.num_len = MN_SMS_ADDR_MAX_LEN;
        }
        sc_addr = s_stk_signal[dual_sys].item.send_sms.address;
    }
    //The address of SMS service does not exist
    else
    {
        SCI_MEMCPY(&sc_addr,&g_sc_addr[dual_sys],sizeof(g_sc_addr[dual_sys]));
    }

    tpdu.length = s_stk_signal[dual_sys].item.send_sms.tpdu.length;
    if(tpdu.length < 1)
    {
        tpdu.length = 1;
    }
    if(tpdu.length > STK_MAX_STR_LEN)
    {
        tpdu.length = STK_MAX_STR_LEN;
    }
    
    SCI_MEMCPY((char*)tpdu.l3_msg,
        (char*)s_stk_signal[dual_sys].item.send_sms.tpdu.l3_msg,
        tpdu.length);

    //There is prompting text that sms is sending.
    if(s_stk_signal[dual_sys].item.send_sms.is_alpha_id)
    {
        ucs2_code = s_stk_signal[dual_sys].item.send_sms.alpha_id.id_ptr[0];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //check the length
            str_len = s_stk_signal[dual_sys].item.send_sms.alpha_id.len;
            if(str_len > STK_MAX_UCS2_STR_LEN)
            {
                str_len = STK_MAX_UCS2_STR_LEN;
            }
            //allocate the memory
            str_ptr = SCI_ALLOC_BASEZ(str_len*2+2);

            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_sms.alpha_id.id_ptr,
                str_len,(uint8*)str_ptr);
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                str_ptr);
            SCI_FREE(str_ptr);
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_stk_signal[dual_sys].item.send_sms.alpha_id.id_ptr);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        sprintf((char*)g_rsp_str, s_stk_prompt_str[AT_STK_STR_SENDSMS]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    //Send SMS 
    g_is_stk_sendsms[dual_sys] = TRUE;
    if(s_stk_signal[dual_sys].item.send_sms.is_packet_IND)
    {
        //SCI_TRACE_LOW("ATC: SMS is packed.");        
        err_code = MNSMS_SendSmsPDUEx(dual_sys, TRUE,&sc_addr,&tpdu,TRUE,FALSE);
        
        if (ERR_MNSMS_NONE != err_code)
        {
            //SCI_TRACE_LOW:"ATC: send sms pdu err code = %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_726_112_2_17_22_17_27_697,(uint8*)"d",err_code);
        }
    }
    else
    {
        err_code = MNSMS_SendSmsPDUEx(dual_sys, TRUE,&sc_addr,&tpdu,FALSE,FALSE);
        
        if (ERR_MNSMS_NONE != err_code)
        {
            //SCI_TRACE_LOW:"ATC: send sms pdu err code = %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_735_112_2_17_22_17_27_698,(uint8*)"d",err_code);
        }
    }
}

/*****************************************************************************/
//     Description : process when received the send ussd signal
//    Global resource dependence : s_stk_signal[]
//  Author:    @hanjun.liu
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessPlayTone(
                              ATC_CONFIG_T *atc_config_ptr
                              )
{
    uint16              str_len = 0;
    char                * str_ptr = PNULL;
    SIMAT_PLAY_TONE_CNF_T play_tone_cnf;
    uint8               ucs2_code = 0;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW("ATC:enter ATC_ProcessPlayTone");
    //There is prompting text that sms is sending.
    if(s_stk_signal[dual_sys].item.play_tone.is_alpha_id)
    {
        ucs2_code = s_stk_signal[dual_sys].item.play_tone.alpha_id.id_ptr[0];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //Check the length
            str_len = s_stk_signal[dual_sys].item.play_tone.alpha_id.len;
            if(str_len > STK_MAX_UCS2_STR_LEN)
            {
                str_len = STK_MAX_UCS2_STR_LEN;
            }
            //allocate the memory
            str_ptr = SCI_ALLOC_BASEZ(str_len*2+2);

            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.play_tone.alpha_id.id_ptr,
                str_len,(uint8*)str_ptr);
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                str_ptr);
            SCI_FREE(str_ptr);
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_stk_signal[dual_sys].item.play_tone.alpha_id.id_ptr);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    play_tone_cnf.result = SIMAT_TR_SUCCESS;    
    SIMAT_CnfPlayToneEx(dual_sys, &play_tone_cnf);
}

/*****************************************************************************/
//     Description : process when received the send ss signal
//    Global resource dependence : s_stk_signal[]
//  Author:    @hanjun.liu
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSendSS(
                              ATC_CONFIG_T *atc_config_ptr
                              )
{
    uint16  str_len = 0;
    uint8   * str_ptr = PNULL;
    uint8   ucs2_code = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //There is prompting text that sms is sending.
    if(s_stk_signal[dual_sys].item.send_ss.is_alpha_id)
    {
        ucs2_code = s_stk_signal[dual_sys].item.send_ss.alpha_id.id_ptr[0];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //Check the length of string.
            str_len = s_stk_signal[dual_sys].item.send_ss.alpha_id.len;
            if(str_len > STK_MAX_UCS2_STR_LEN)
            {
                str_len = STK_MAX_UCS2_STR_LEN;
            }
            //allocate the memory
            str_ptr = SCI_ALLOC_BASEZ((str_len+1)*2);
            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_ss.alpha_id.id_ptr,
                str_len,str_ptr);
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                (char *)str_ptr);
            SCI_FREE(str_ptr);
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_stk_signal[dual_sys].item.send_ss.alpha_id.id_ptr);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        sprintf((char*)g_rsp_str, s_stk_prompt_str[AT_STK_STR_SENDSS]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    //send ss
}

/*****************************************************************************/
//     Description : process when received the send ussd signal
//    Global resource dependence : s_stk_signal[]
//  Author:    @hanjun.liu
//    Note: 
/*****************************************************************************/
LOCAL void ATC_ProcessSendUSSD(
                              ATC_CONFIG_T *atc_config_ptr
                              )
{
    uint16              str_len = 0;
    char                * str_ptr = PNULL;
    ERR_MNSS_CODE_E     err_code;
    uint8               ucs2_code = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //There is prompting text that sms is sending.
    if(s_stk_signal[dual_sys].item.send_ussd.is_alpha_id)
    {
        ucs2_code = s_stk_signal[dual_sys].item.send_ussd.alpha_id.id_ptr[0];
        if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //Check the length
            str_len = s_stk_signal[dual_sys].item.send_ussd.alpha_id.len;
            if(str_len > STK_MAX_UCS2_STR_LEN)
            {
                str_len = STK_MAX_UCS2_STR_LEN;
            }
            //allocate the memory
            str_ptr = SCI_ALLOC_BASEZ(str_len*2+2);

            //change the uint8 to ASCII format
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_ussd.alpha_id.id_ptr,
                str_len,(uint8*)str_ptr);
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                str_ptr);
            SCI_FREE(str_ptr);
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\"",
                g_atc_info_table[AT_CMD_STGI].cmd_name->str_ptr,
                s_stk_signal[dual_sys].item.send_ussd.alpha_id.id_ptr);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        sprintf((char*)g_rsp_str, s_stk_prompt_str[AT_STK_STR_SENDUSSD]);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    //send ussd    
    err_code = MNSS_UserOriginateUSSDEx(dual_sys,TRUE,MN_SS_DEFAULT_DCS_VALUE,
        s_stk_signal[dual_sys].item.send_ussd.ussd.str_len,s_stk_signal[dual_sys].item.send_ussd.ussd.ussd_str);
    
    if (ERR_MNSS_NO_ERR != err_code)
    {
        //SCI_TRACE_LOW:"ATC: user originate ussd fail, err code = %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_898_112_2_17_22_17_27_699,(uint8*)"d",err_code);
    }
}
#endif

LOCAL void ATC_StartStkTimer(
                              ATC_CONFIG_T *atc_config_ptr,
                              MN_DUAL_SYS_E    dual_sys
                              )
{
    if ((atc_config_ptr->atc_config_nv.stk_time > 0) && (dual_sys < MN_SYS_NUMBER))
    {
        SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
            (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
            &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
    }
}

/*****************************************************************************/
//     Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//    Global resource dependence : 
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern ATC_STATUS ATC_STKProcessSendSmsCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_SEND_SMS_CNF_T    *sig_ptr
                )
{
    SIMAT_SEND_SMS_CNF_T  send_sms;

    //SCI_TRACE_LOW:"ATC:enter ATC_STKProcessSendSmsCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_928_112_2_17_22_17_27_700,(uint8*)"");

    if (AT_CMD_PSSTKSMS == s_stk_send_sms[sig_ptr->dual_sys].cmd_id)
    {
        sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_PSSTKSMS].cmd_name->str_ptr, sig_ptr->cause);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

        if (MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: Receive the error status: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_941_112_2_17_22_17_27_701,(uint8*)"d", sig_ptr->cause);
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_313); 
        }
        
        SCI_MEMSET((void *)&s_stk_send_sms[sig_ptr->dual_sys], 0, sizeof(ATC_STK_SEND_SM_T));
        g_is_stk_sendsms[sig_ptr->dual_sys] = FALSE;
        
        return S_ATC_SUCCESS;
    }
    
    //send the sms successfully.
    if(MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
    {
        //SCI_TRACE_LOW:"ATC:ATC_STKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_954_112_2_17_22_17_27_702,(uint8*)"");
        send_sms.result = SIMAT_TR_SUCCESS;
        //send the result to SIM.
        SIMAT_CnfSendSmsEx(sig_ptr->dual_sys, &send_sms); 
    }
    //fail to send the sms.
    else
    {
        //SCI_TRACE_LOW:"ATC:ATC_STKProcessSendSmsCnf,MN_SMS_FAIL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_962_112_2_17_22_17_27_703,(uint8*)"");
        send_sms.result = SIMAT_TR_SMS_ERROR;
        send_sms.ne_info = SIMAT_NP_NO_SPECIFIC_CAUSE;
        //send the result to SIM.
        SIMAT_CnfSendSmsEx(sig_ptr->dual_sys, &send_sms); 
    }
    g_is_stk_sendsms[sig_ptr->dual_sys] = FALSE;
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//     Description : process when user selected item in setup menu 
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_MainMenuCnf(
                              MN_DUAL_SYS_E dual_sys,
                              uint32  result, //value of result
                              uint8   n_select //item number to select
                              )
{
    BOOLEAN re_value = TRUE;
    SIMAT_MENU_SELECTION_IND_T menu_select;

    //Check the index
    if(n_select > s_stk_signal[dual_sys].setup_menu.item.item_data_object_num)
    {
        return FALSE;
    }

    s_mainmenu_selitem[dual_sys] = n_select;

    n_select--;
    //SCI_TRACE_LOW("ATC:n_select= %d",n_select);
    switch(result)
    {
        case ATC_RESULT_SELECTITEM:
        {
            menu_select.is_help_IND = FALSE;
            menu_select.menu_id = s_stk_signal[dual_sys].setup_menu.item.item_data_object_info[n_select][0];
            //Send the select of user to PS            
            SIMAT_IndMenuSelEx(dual_sys, &menu_select);
            break;
        }
        
        case ATC_RESULT_HELPINFO:
        {
            if(TRUE == s_stk_signal[dual_sys].setup_menu.help_info)
            {
                menu_select.is_help_IND = TRUE;
                //Send the select of user to PS                
                SIMAT_IndMenuSelEx(dual_sys, &menu_select);
            }
            else
            {
                re_value = FALSE;
            }
            break;
        }
        
        default:
        {
            re_value = FALSE;
            break;
        }
    }
    //SCI_TRACE_LOW("ATC:ATC_MainMenuCnf=%d",re_value);
    return(re_value);
}

/*****************************************************************************/
//     Description : process when user gives a response in display text
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_DisplayTextCnf(
                                 MN_DUAL_SYS_E dual_sys,
                                 uint32  cmd_type
                                 )
{
    BOOLEAN re_value = TRUE;
    SIMAT_DISPLAY_TEXT_CNF_T display_text;
    if(!s_stk_signal[dual_sys].item.display_text.is_ir)
    {
        switch(cmd_type)
        {
        case ATC_DISPLAY_TEXT:
            display_text.result = SIMAT_TR_SUCCESS;
            break;
            //User go back
        case ATC_BACK_MOVE:
            display_text.result = SIMAT_TR_BACK_MOVE;
            break;
            //User abort
        case ATC_END_SESSION:
            display_text.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            display_text.result = SIMAT_TR_NO_RESPONSE;            
            break;
        default:
            re_value = FALSE;
            break;
        }
        //Send the display text cnf
        if(re_value)
        {            
            SIMAT_CnfDisplayTextEx(dual_sys, &display_text);
        }
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : check the string which user input at get inkey
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckAndHandleGetInkey(
                                         MN_DUAL_SYS_E dual_sys,
                                         uint8 *key_ptr, //pointer of pointer
                                         uint8 str_len  //length of string
                                         )
{
    BOOLEAN re_value = TRUE;
    SIMAT_GET_INKEY_CNF_T get_inkey;

    if (NULL == key_ptr)
    {
        return FALSE;
    }
    switch(s_stk_signal[dual_sys].item.get_inkey.inkey_type)
    {
    //Require inputting digit
    case SIMAT_DIGIT_ONLY:
        if((*key_ptr >= '0' && *key_ptr <= '9')||('*' == *key_ptr)
            ||('#' == *key_ptr)||('+' == *key_ptr))
        {            
            if(str_len > STK_GETINKEY_CNF_ASCI_LEN)
            {
                str_len = STK_GETINKEY_CNF_ASCI_LEN;
            }
            //configure the get_inkey
            get_inkey.result = SIMAT_TR_SUCCESS;
            get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
            get_inkey.text_str.dcs = STK_DEFAULT_ALPHABET;
            *(get_inkey.text_str.text_ptr)   = *key_ptr;
        }
        //SCI_TRACE_LOW:"ATC:ATC_CheckAndHandleGetInkey,SIMAT_DIGIT_ONLY,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1108_112_2_17_22_17_28_704,(uint8*)"d",re_value);
        break;

    //Require inputting english string
    case SIMAT_SMS_ALPHABET:
        if(str_len > STK_GETINKEY_CNF_ASCI_LEN)
        {
            str_len = STK_GETINKEY_CNF_ASCI_LEN;
        }
        
        //configure the get_inkey
        get_inkey.result = SIMAT_TR_SUCCESS;
        get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
        get_inkey.text_str.dcs = STK_DEFAULT_ALPHABET;
        *get_inkey.text_str.text_ptr     = *key_ptr;
        *(get_inkey.text_str.text_ptr+1) = '\0';
        //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleGetInkey,SIMAT_SMS_ALPHABET,%d", re_value);
        break;

    //Require inputting UCS2 string
    case SIMAT_UCS2_ALPHABET:
        {
            uint8 ucs2_string[STK_GETINKEY_CNF_UCS2_LEN+2];

            SCI_MEMSET(ucs2_string,'0', sizeof(ucs2_string));
            if(str_len > STK_GETINKEY_CNF_UCS2_LEN)
            {
                str_len = STK_GETINKEY_CNF_UCS2_LEN;
            }
            //configure the get_inkey
            get_inkey.result = SIMAT_TR_SUCCESS;
            get_inkey.text_str.len = STK_GETINKEY_CNF_UCS2_LEN;
            get_inkey.text_str.dcs = STK_DEFAULT_ALPHABET;
            //Change the input the string into UCS2 string and assign it
            if(!ConvertAscUcs2IntoBin((char*)key_ptr,get_inkey.text_str.text_ptr,str_len/2))
            {
                return FALSE;
            }
            //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d", re_value);
        }

        break;

    //Require inputting YES or NO.
    case SIMAT_YES_NO:
        if(*key_ptr == 'Y' || *key_ptr == 'N')
        {
            //Check the length
            if(str_len > STK_GETINKEY_CNF_ASCI_LEN)
            {
                str_len = STK_GETINKEY_CNF_ASCI_LEN;
            }
            //configure the get_inkey
            get_inkey.result = SIMAT_TR_SUCCESS;
            get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
            get_inkey.text_str.dcs = STK_DEFAULT_ALPHABET;
                    //user select YES
            if(*key_ptr == 'Y')
            {
                *get_inkey.text_str.text_ptr = '1';
            }
            //user select NO
            else
            {
                *get_inkey.text_str.text_ptr = '0';
            }
            *(get_inkey.text_str.text_ptr+1)='\0';
        }
        //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleGetInkey,SIMAT_UCS2_ALPHABET,%d", re_value);
        break;

    default:
        re_value = FALSE;
        break;
    }
    if(re_value)
    {
        //Send the result to SIM.        
        SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : process when user gives a response in get inkey
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetInkeyCnf(
                              MN_DUAL_SYS_E dual_sys,
                              uint32  cmd_type, //the type of command
                              uint32  result,   //value of result
                              uint8   *key_ptr,//Pointer of data
                              uint8   keys_length  //Length of data
                              )
{
    BOOLEAN re_value = TRUE ;
    SIMAT_GET_INKEY_CNF_T get_inkey;
    //open the share buffer
    
    switch(cmd_type)
    {        
        //User go back
    case ATC_BACK_MOVE:
        get_inkey.result = SIMAT_TR_BACK_MOVE;
        //Send the get key cnf(go back)        
        SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);  
        break;

        //User abort
    case ATC_END_SESSION:
        get_inkey.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)
        SIMAT_CnfGetInkeyEx (dual_sys, &get_inkey);  
        break;

    case ATC_NO_RESPONSE:
        get_inkey.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)        
        SIMAT_CnfGetInkeyEx (dual_sys, &get_inkey);  
        break;

    case ATC_GET_INKEY:
        if(ATC_USER_RESPONSE == result)
        {
            if(ATC_CheckAndHandleGetInkey(dual_sys,key_ptr,keys_length))
            {
                //SCI_TRACE_LOW:"ATC:ATC_CheckAndHandleGetInkey==TRUE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1234_112_2_17_22_17_28_705,(uint8*)"");
            }
        }
        else if(ATC_USER_END_SSESSION == result)
        {
            get_inkey.result = SIMAT_TR_TERMINATED_BY_USER;
            //Send the get key cnf(user end)              
            SIMAT_CnfGetInkeyEx (dual_sys, &get_inkey);
        }
        else
        {
            re_value = FALSE;
        }
        break;
    default:
        re_value = FALSE;
        break;
    }
    //SCI_TRACE_LOW("ATC: re_value=%d",re_value);
    return(re_value);
}

/*****************************************************************************/
//     Description : check the string which user input at get input
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_IsDigitInput(
                               uint8 *data_ptr,  //pointer of data
                               uint8 str_len   //string of data
                               )
{
    uint32 i;
    for(i = 0;i < str_len;i++)
    {
        if(!((*(data_ptr+i) >= '0' && *(data_ptr+i) <= '9')||('*' == *(data_ptr+i))
            ||('#' == *(data_ptr+i))||('+' == *(data_ptr+i))))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : check the string which user input at get input
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckAndHandleGetInput(
                                         MN_DUAL_SYS_E dual_sys,
                                         uint8 *data_ptr,
                                         uint8 str_len
                                         )
{
    BOOLEAN re_value = FALSE;
    uint8 text_string[STK_MAX_STR_LEN+2];
    SIMAT_GET_INPUT_CNF_T get_input;

    //SCI_TRACE_LOW("ATC:enter ATC_CheckAndHandleGetInput");
    SCI_MEMSET(text_string,0,sizeof(text_string));
    //Check the length of string
    /*if(str_len < s_stk_signal[dual_sys].item.get_input.response_len.min_len ||
        str_len > s_stk_signal[dual_sys].item.get_input.response_len.max_len)
    {
        //SCI_TRACE_LOW:"ATC:error in length"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1297_112_2_17_22_17_28_706,(uint8*)"");
        return re_value;
    }*/
    //SCI_TRACE_LOW:"ATC: getinput type = %d, str_len = %d, min_len = %d max_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1305_112_2_17_22_17_28_707,(uint8*)"dddd",s_stk_signal[dual_sys].item.get_input.input_type,str_len,s_stk_signal[dual_sys].item.get_input.response_len.min_len,s_stk_signal[dual_sys].item.get_input.response_len.max_len);
    
    if(1 > str_len)
    {
        str_len = 1;
    }
    
    
    /*if (NULL == get_input.text_str.text_ptr)
    {
        return re_value;
    }*/
    //Require inputting digit
    switch(s_stk_signal[dual_sys].item.get_input.input_type)
    {
    case SIMAT_INPUT_DIGIT_ONLY:     //the input is be digit(0-9,*,#)
        if(str_len < s_stk_signal[dual_sys].item.get_input.response_len.min_len ||
           str_len > s_stk_signal[dual_sys].item.get_input.response_len.max_len)
        {
           //SCI_TRACE_LOW:"ATC:error in length"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1324_112_2_17_22_17_28_708,(uint8*)"");
           return re_value;
        }

        if(!ATC_IsDigitInput(data_ptr,str_len))
        {
            return FALSE;
        }
        //configure the get_input
        get_input.text_str.dcs = STK_DEFAULT_ALPHABET;
        get_input.result = SIMAT_TR_SUCCESS;
        get_input.text_str.len = str_len;
        SCI_MEMCPY(text_string,    data_ptr,str_len);
        re_value = TRUE;
        //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleGetInput,SIMAT_DIGIT_ONLY,%d",    re_value,str_len);
        break;

    case SIMAT_INPUT_SMS_ALPHABET:  //the input is unpacked sms alphabet
        if(str_len < s_stk_signal[dual_sys].item.get_input.response_len.min_len ||
           str_len > s_stk_signal[dual_sys].item.get_input.response_len.max_len)
        {
           //SCI_TRACE_LOW:"ATC:error in length"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1345_112_2_17_22_17_28_709,(uint8*)"");
           return re_value;
        }

        get_input.text_str.dcs = STK_DEFAULT_ALPHABET;
        get_input.result = SIMAT_TR_SUCCESS;
        get_input.text_str.len = str_len;
        SCI_MEMCPY(text_string,    data_ptr,str_len);
        re_value = TRUE;
        //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleGetInput,SIMAT_INPUT_SMS_ALPHABET,%d",    re_value);
        break;

    case SIMAT_INPUT_UCS2_ALPHABET:   //the input is ucs2 alphabet
        {
            if(str_len < (s_stk_signal[dual_sys].item.get_input.response_len.min_len *4) ||
               str_len > (s_stk_signal[dual_sys].item.get_input.response_len.max_len *4))
            {
               //SCI_TRACE_LOW:"ATC:error in length"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1362_112_2_17_22_17_28_710,(uint8*)"");
               return re_value;
            }
            //configure the get_inkey
            get_input.result = SIMAT_TR_SUCCESS;
            get_input.text_str.len = str_len/2;
            get_input.text_str.dcs = STK_UCS2_ALPHABET;
            //Change the ucs2 string to bin ucs2 and assign it text_string
            if(!ConvertAscUcs2IntoBin((char*)data_ptr,text_string,str_len/2))
            {
                return (re_value);
            }
            str_len = str_len/2;
            re_value = TRUE;
            //SCI_TRACE_LOW("ATC:SIMAT_INPUT_UCS2_ALPHABET,%d",re_value);
        }
        break;

    default:

        break;
    }
    //If SIM requre input packed string,pack it
    if(s_stk_signal[dual_sys].item.get_input.is_packed)
    {
        get_input.text_str.dcs = STK_PACKED_FORMAT;
        get_input.text_str.len = SIMAT_PackSmsAlphabet((uint8*)text_string, 
                                                        str_len,
                                                        get_input.text_str.text_ptr,
                                                        STK_MAX_STR_LEN
                                                        );
        
        
    }
    else
    {
        SCI_MEMCPY(get_input.text_str.text_ptr,text_string,str_len);
    }

    //Send the confirm to SIM.    
    SIMAT_CnfGetInputEx (dual_sys, &get_input);  
    return (re_value);
}

/*****************************************************************************/
//     Description : process when user gives a response in get input
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetInputCnf(
                              MN_DUAL_SYS_E dual_sys,
                              uint32  cmd_type,
                              uint32  result,
                              uint8   *data_ptr,
                              uint8   data_length
                              )
{
    BOOLEAN re_value = TRUE;
    SIMAT_GET_INPUT_CNF_T  get_input;

    //SCI_TRACE_LOW("ATC:enter ATC_GetInputCnf,%d,%d",cmd_type,result);
    
    switch(cmd_type)
    {
        //User go back
    case ATC_BACK_MOVE:
        get_input.result = SIMAT_TR_BACK_MOVE;
        //Send the get key cnf(go back)        
        SIMAT_CnfGetInputEx(dual_sys, &get_input);  
        break;

        //User abort
    case ATC_END_SESSION:
        get_input.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)        
        SIMAT_CnfGetInputEx (dual_sys, &get_input);  
        break;

    case ATC_NO_RESPONSE:
        get_input.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)
        SIMAT_CnfGetInputEx (dual_sys, &get_input);  
        break;

    case ATC_GET_INPUT:
        if(ATC_USER_RESPONSE == result)
        {
            if(ATC_CheckAndHandleGetInput(dual_sys,data_ptr,data_length))
            {
                //SCI_TRACE_LOW:"ATC:ATC_CheckAndHandleGetInkey==TRUE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1450_112_2_17_22_17_28_711,(uint8*)"");
            }
            else
            {
                re_value = FALSE;
            }
        }
        else if(ATC_USER_END_SSESSION == result)
        {
            get_input.result = SIMAT_TR_TERMINATED_BY_USER;
            //Send the get key cnf(user end)             
            SIMAT_CnfGetInputEx (dual_sys, &get_input); 
        }
        else
        {
            re_value = FALSE;
        }
        break;

        default:
            re_value = FALSE;
            break;
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : This function handle the call signals 
//    Global resource dependence : none
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern void  ATC_StkProcessCallSignals(
                                       MN_DUAL_SYS_E  dual_sys,
                                       BOOLEAN IsCallSucc
                                     )
{
    SIMAT_SETUP_CALL_CNF_T  setup_call;

    //SCI_TRACE_LOW("ATC:ATC_StkProcessCallSignals");

    if(TRUE == IsCallSucc)
    {
        //SCI_TRACE_LOW("ATC:IsCallSucc == TRUE");
        setup_call.result = SIMAT_TR_SUCCESS;
        //Send the get key cnf(user end)        
        SIMAT_CnfSetupCallEx (dual_sys, &setup_call);  
    }
    else
    {
        //SCI_TRACE_LOW("ATC:IsCallSucc == FALSE");
        setup_call.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)        
        SIMAT_CnfSetupCallEx (dual_sys, &setup_call);  
    }
    g_is_stk_call[dual_sys] = FALSE;
}

/*****************************************************************************/
//     Description : check the string which user input at get input
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN ATC_HandleSetupCall(
                                  ATC_CONFIG_T            *atc_config_ptr,
                                  SIMAT_SETUP_CALL_CNF_T  *setup_call,
                                  uint32 result
                                  )
#else
LOCAL BOOLEAN ATC_HandleSetupCall(
                                  SIMAT_SETUP_CALL_CNF_T  *setup_call,
                                  uint32 result
                                  )
#endif
{
    BOOLEAN              re_value = TRUE;
    MN_CALLED_NUMBER_T   called_num;             
    MN_SUBADDR_T         sub_addr;

    MN_CALL_TYPE_E       call_type;
    uint8                call_id;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(result)
    {
    case ATC_REFUSE_CALL:     //User refuse the call
        setup_call->result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)        
        SIMAT_CnfSetupCallEx (dual_sys, setup_call); 
        break;

    case ATC_ACCEPT_CALL:  //User accep the call
        SCI_MEMSET(&called_num, 0, sizeof(called_num));
        if (CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id) || 
            CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
        {
            re_value = FALSE;
            break;
        } 
        //check the status of PLMN.        
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
        if (PLMN_NO_SERVICE == plmn_info.plmn_status)
        {
            re_value = FALSE;
            break;
        }
        call_type = MN_CALL_TYPE_NORMAL;
        called_num = s_stk_signal[dual_sys].item.setup_call.call_parameter.address;
        
        //check the length
        if(s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len > MN_MAX_ADDR_BCD_LEN)
        {
            called_num.num_len = MN_MAX_ADDR_BCD_LEN;
        }
        else
        {
            called_num.num_len = s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len;
        }


        //There is sub address in the call number
        if(s_stk_signal[dual_sys].item.setup_call.call_parameter.is_cps)
        {
            sub_addr = s_stk_signal[dual_sys].item.setup_call.call_parameter.cps;
            
            if(s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len > MN_MAX_ADDR_BCD_LEN)
            {
                sub_addr.addr_len = MN_MAX_ADDR_BCD_LEN;
            }
            else
            {
                sub_addr.addr_len = s_stk_signal[dual_sys].item.setup_call.call_parameter.cps.addr_len;
            }            
            //SCI_MEMCPY(sub_addr.sub_num,s_stk_signal[dual_sys].item.setup_call.call_parameter.cps.sub_num,
            //sub_addr.addr_len);
            //Start a call with sub address.
            if (MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys,
                                        call_type, &called_num,
                                        &sub_addr, MN_CALL_CLIR_SUPPRESSION, PNULL))
            {
                g_is_stk_call[dual_sys] = TRUE;
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_START_IND,0xff,ATC_DOMAIN_CS);
            }

        }
        else //No sub address
        {            
            // start a call
            if (MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys,
                                    call_type, &called_num, 
                                    PNULL, MN_CALL_CLIR_SUPPRESSION, PNULL))
            {
                g_is_stk_call[dual_sys] = TRUE;
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_START_IND,0xff,ATC_DOMAIN_CS);
            }
        }
        break;

    default:
        re_value = FALSE;
        break;
    }
    return (re_value);
}

/*****************************************************************************/
//     Description : process when user gives a response in setup call
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN ATC_SetupCallCnf(
                               ATC_CONFIG_T   *atc_config_ptr,
                               uint32  cmd_type, //the type of command
                               uint32 result  //value of result
                               )
#else
LOCAL BOOLEAN ATC_SetupCallCnf(
                               uint32  cmd_type, //the type of command
                               uint32 result  //value of result
                               )
#endif
{
    BOOLEAN re_value = TRUE;
    SIMAT_SETUP_CALL_CNF_T  setup_call;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
  
    switch(cmd_type)
    {
        //User go back
    case ATC_BACK_MOVE:
        setup_call.result = SIMAT_TR_BACK_MOVE;
        //Send the get key cnf(go back)        
        SIMAT_CnfSetupCallEx(dual_sys, &setup_call); 
        break;

        //User abort
    case ATC_END_SESSION:
        setup_call.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)         
        SIMAT_CnfSetupCallEx (dual_sys, &setup_call); 
        break;

    case ATC_NO_RESPONSE:
        setup_call.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)        
        SIMAT_CnfSetupCallEx (dual_sys, &setup_call);  
        break;

    case ATC_SETUP_CALL:
        //SCI_TRACE_LOW("ATC:ATC_SetupCallCnf,ATC_SETUP_CALL");
    #ifdef _MUX_ENABLE_
        if(!ATC_HandleSetupCall(atc_config_ptr,&setup_call,result))
    #else
        if(!ATC_HandleSetupCall(&setup_call,result))
    #endif
        {
            re_value = FALSE;
        }
        break;

    default:
        re_value = FALSE;
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : process when user selected item in setup menu 
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_SelectItemHandle(
                                   MN_DUAL_SYS_E dual_sys,
                                   uint32  result, //value of result
                                   uint8   item_num     //ponter of data
                                   )
{
    BOOLEAN re_value = TRUE;
    int32 n_select = 0;
    SIMAT_SELECT_ITEM_CNF_T select_item;    

    switch(result)
    {
        case ATC_ST_SELECTITEM:
        n_select = item_num - 1;
        //Check the index
        if(n_select < s_stk_signal[dual_sys].menu_item_num)
        {
            select_item.result = SIMAT_TR_SUCCESS;
            select_item.item_id = s_stk_signal[dual_sys].menu_item_id[n_select];
            //Send the select of user to PS            
            SIMAT_CnfSelItemEx(dual_sys, &select_item);
        }
        else
        {
            re_value = FALSE;
        }
        break;

        case ATC_ST_HELPINFO:
        break;

        case ATC_ST_USERBORT:
        select_item.result = SIMAT_TR_TERMINATED_BY_USER;
        SIMAT_CnfSelItemEx(dual_sys, &select_item);
        break;

        case ATC_ST_RETURNBACK:
        select_item.result = SIMAT_TR_BACK_MOVE;        
        SIMAT_CnfSelItemEx(dual_sys, &select_item);
        break;

        default:
            re_value = FALSE;
            break;
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : process when user gives a response in select item
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_SelectItemCnf(
                                MN_DUAL_SYS_E dual_sys,
                                uint32  cmd_type,       //type of command
                                uint32  result,   //value of result
                                uint8   item_num            //select item 
                                )
{
    BOOLEAN re_value = TRUE;
    SIMAT_SELECT_ITEM_CNF_T select_item;
    switch(cmd_type)
    {
        //User go back
    case ATC_BACK_MOVE:
        select_item.result = SIMAT_TR_BACK_MOVE;
        //Send the display text cnf        
        SIMAT_CnfSelItemEx(dual_sys, &select_item);
        break;

        //User abort
    case ATC_END_SESSION:
        select_item.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the display text cnf        
        SIMAT_CnfSelItemEx(dual_sys, &select_item);
        break;

    case ATC_NO_RESPONSE:        
        select_item.result = SIMAT_TR_NO_RESPONSE;
        //Send the display text cnf        
        SIMAT_CnfSelItemEx(dual_sys, &select_item);
        break;

    case ATC_SELECT_ITEM:
        ATC_SelectItemHandle(dual_sys,result,item_num);
        break;

    default:
        re_value = FALSE;
        break;
    }
    return(re_value);
}

/*****************************************************************************/
//     Description : This function check and handle what user input for terminal
//                profile 
//    Global resource dependence : g_rsp_str
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern BOOLEAN ATC_CheckAndHandleConfig(
                                       ATC_CONFIG_T   *atc_config_ptr,
                                       char *config_char_ptr
                                       )
{
    uint8 config_str[STK_PROFILE_CHAR_LEN + 2];
    uint8 config_result[STK_PROFILE_RESULT_LEN];
    uint8 i = 0;
    BOOLEAN re_value = FALSE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(config_str,0, sizeof(config_str));
    SCI_MEMSET(config_result,0,sizeof(config_result));
    SCI_MEMCPY(config_str,config_char_ptr,sizeof(config_str));

    //check if input string between 160060C01F-5FFFFFFF7F and convert into bin format
    if(strcmp(config_char_ptr,g_stk_terminal_profile[AT_STK_TRP_MIN_VAL]) >= 0 &&
        strcmp(config_char_ptr,g_stk_terminal_profile[AT_STK_TRP_MAX_VAL]) <= 0)
    {
        //change it to the bit flow.
        while(i < STK_PROFILE_CHAR_LEN)
        {
            if((config_str[i] <= 0x39)&&(config_str[i+1] <= 0x39))
            {
                config_result[i/2] = ((config_str[i]-0x30)<<4)+
                                        (config_str[i+1]-0x30);
            }
            else if((config_str[i] > 0x39)&&(config_str[i+1] <= 0x39))
            {
                config_result[i/2] = ((config_str[i]-0x41+10)<<4)+
                                        (config_str[i+1]-0x30);
            }
            else if((config_str[i] <= 0x39)&&(config_str[i+1] > 0x39))
            {
                config_result[i/2] = ((config_str[i]-0x30)<<4)+
                                        (config_str[i+1]-0x41+10);
            }
            else if((config_str[i] > 0x39)&&(config_str[i+1] > 0x39))
            {
                config_result[i/2] = ((config_str[i]-0x41+10)<<4)+
                                        (config_str[i+1]-0x41+10);
            }
            else
            {
                return FALSE;
            }
            i = i + 2;
        }
        //Call the fucntion of SIM to store it.
        if (atc_config_ptr->sim_flag[dual_sys] && TRUE == s_stk_signal[dual_sys].is_support_stk)
        {            
            SIMAT_TerminalProfileEx(dual_sys, config_result,STK_PROFILE_RESULT_LEN);
            re_value = TRUE;
        }
        else
        {
            re_value = FALSE;
        }
    }
    //SCI_TRACE_LOW("ATC:ATC_CheckAndHandleConfig,re_value=%d",re_value);
    return (re_value);    
}
#endif /* _ULTRA_LOW_CODE_ */


/*****************************************************************************/
//     Description : This function send the response to sim if STK is activated.
//    Global resource dependence : 
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
LOCAL void ATC_InitCnfSetMenu(MN_DUAL_SYS_E dual_sys)
{
#ifndef _ULTRA_LOW_CODE_
    //Send the respond if stk_actflag==TRUE
    SIMAT_SETUP_MENU_CNF_T  smenu;
    smenu.result = SIMAT_TR_SUCCESS;    

    SIMAT_CnfSetMenuEx(dual_sys, &smenu);
#endif
}

/*****************************************************************************/
//     Description : This function handles the AT+STSF command 
//    Global resource dependence : g_rsp_str
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTSF )
{
#if 0
#ifndef _ULTRA_LOW_CODE_
#ifdef _ATC_ONLY
    uint32  return_value = S_ATC_SUCCESS;
    uint8   terminal_profile[STK_PROFILE_CHAR_LEN + 1] = {0};
    uint8   result_terminal_profile[STK_PROFILE_CHAR_LEN] = {0};
    uint16  length = STK_PROFILE_CHAR_LEN;
    BOOLEAN flag = FALSE;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_TEST:
        sprintf((char*)g_rsp_str, "%s: 0-2, (160060C01F-5FFFFFFF7F),1-255",
            g_atc_info_table[AT_CMD_STSF].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_READ:
        //SCI_TRACE_LOW("ATC:before SIMAT_GetMeProfile");
        SIMAT_GetMeProfile(result_terminal_profile,&length);
        ATC_TranUint8IntoChar(result_terminal_profile,
            STK_PROFILE_RESULT_LEN,terminal_profile);
        sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",
            g_atc_info_table[AT_CMD_STSF].cmd_name->str_ptr,
            atc_config_ptr->atc_config_nv.stk_actflag,
            (char*)terminal_profile,atc_config_ptr->atc_config_nv.stk_time);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    //set command AT+STSF=<mode>[,<config>][,<Timeout>]
    case ATC_CMD_TYPE_SET:
        // Check the <mode>
        if (!PARAM1_FLAG)
        {
            return ERR_INVALID_INDEX;
        }
        switch (PARA1.num)
        {
        case ATC_STK_DEACTIVATE:            
            if( TRUE == s_stk_signal[dual_sys].is_support_stk)
            {
                //SCI_TRACE_LOW("ATC:TRUE == s_stk_signal[dual_sys].is_support_stk");
                atc_config_ptr->atc_config_nv.stk_actflag = FALSE;
                ATC_WriteNVItem(atc_config_ptr);
                switch(s_stk_signal[dual_sys].cur_status)
                {
                case ATC_STATUS_DISPLAYTEXT:
                    flag = ATC_DisplayTextCnf(dual_sys,ATC_END_SESSION);
                    break;
                case ATC_STATUS_GETINKEY:
                    flag = ATC_GetInkeyCnf(dual_sys,ATC_END_SESSION,0,PNULL,0);
                    break;
                case ATC_STATUS_GETINPUT:
                    flag = ATC_GetInputCnf(dual_sys,ATC_END_SESSION,0,PNULL,0);
                    break;
                case ATC_STATUS_SELECTITEM:
                    flag = ATC_SelectItemCnf(dual_sys,ATC_END_SESSION,0,0);
                    break;
                case ATC_STATUS_SENDSMS:
                    flag = FALSE;
                    break;
                default:
                    break;
                }
                
                if(!flag)
                {
                    //SCI_TRACE_LOW:"ATC: stk deactivate fail."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1935_112_2_17_22_17_29_712,(uint8*)"");
                    return_value = ERR_OPERATION_NOT_ALLOWED;
                }
            }
            else
            {
                //SCI_TRACE_LOW("ATC:return_value = ERR_OPERATION_NOT_ALLOWED");
                return_value = ERR_OPERATION_NOT_ALLOWED;
            }
            break;

        case ATC_STK_ACTIVATE:
            if( TRUE == s_stk_signal[dual_sys].is_support_stk)
            {
                SIMAT_SETUP_MENU_CNF_T  smenu;

                if(ATC_STATUS_SETUPMENU == s_stk_signal[dual_sys].cur_status && 
                    FALSE == atc_config_ptr->atc_config_nv.stk_actflag)
                {
                    //Delete because STK requre to call it only once.
                    if(FALSE == s_is_setup_menu_cnf[dual_sys])
                    {
                        smenu.result = SIMAT_TR_SUCCESS;
                        SIMAT_CnfSetMenuEx(dual_sys, &smenu);
                        s_is_setup_menu_cnf[dual_sys] = TRUE;
                    }
                    //Return +STIN:0
                    sprintf((char*)g_rsp_str, "%s: 0",
                        g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);                            
                    s_cmd_stag[dual_sys] = ATC_CMD_INIT;
                    atc_config_ptr->atc_config_nv.stk_actflag = TRUE;
                    ATC_WriteNVItem(atc_config_ptr);    
                }
            }
            else
            {
                //SCI_TRACE_LOW("ATC:FALSE == s_stk_signal[dual_sys].is_support_stk");
                return_value = ERR_OPERATION_NOT_ALLOWED;
            }
            break;

        case ATC_STK_CONFIGURE:
            //If SIM card doesnt support STK,return FALSE
            if(FALSE == s_stk_signal[dual_sys].is_support_stk)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            if (!PARAM2_FLAG || STK_PROFILE_CHAR_LEN != PARA2.str_ptr->str_len)
            {
                //SCI_TRACE_LOW("ATC:ERR_INVALID_INDEX");
                return_value = ERR_INVALID_INDEX;
                break;
            }
            //check the config string and send it to sim.
            if(!ATC_CheckAndHandleConfig(atc_config_ptr,(char *)PARA2.str_ptr->str_ptr))
            {
                //SCI_TRACE_LOW:"ATC:ATC_CheckAndHandleConfig is false"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_1992_112_2_17_22_17_30_713,(uint8*)"");
                return_value = ERR_OPERATION_NOT_ALLOWED;
                break;
            }

            if(PARAM3_FLAG)
            {
                atc_config_ptr->atc_config_nv.stk_time = PARA3.num;
                ATC_WriteNVItem(atc_config_ptr);
            }
            break;

        default:
            return_value = S_ATC_FAIL;
            break;
        }
        break;

    default:

        break;
    }
    return(return_value);
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif/*_ATC_ONLY*/
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
#else
return ERR_OPERATION_NOT_SUPPORTED;
#endif
}
/*****************************************************************************/
//     Description : This function handles the AT+STGI command 
//    Global resource dependence : g_rsp_str
//    Author:       Liuhan
//    Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTGI )
{
#if 0
#ifndef _ULTRA_LOW_CODE_
#ifdef _ATC_ONLY
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag || FALSE == atc_config_ptr->sim_flag[dual_sys]
        || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        // Check the stag of command
        /*if(ATC_CMD_INIT != s_cmd_stag)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }*/
        // Check the <cmd_type>
        if (!PARAM1_FLAG)
        {
            return ERR_INVALID_INDEX;
        }
        if(PARA1.num != s_stk_signal[dual_sys].cur_status)
        {
            //SCI_TRACE_LOW:"ATC:PARA1.num != s_stk_signal[dual_sys].cur_status"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_2056_112_2_17_22_17_30_714,(uint8*)"");
            return ERR_INVALID_INDEX;
        }
        switch(PARA1.num)
        {
        case ATC_SETUP_MENU:
         ATC_StartStkTimer(atc_config_ptr,dual_sys);
            ATC_ProcessSetupMenu(atc_config_ptr);
            break;
        case ATC_DISPLAY_TEXT:
            ATC_ProcessDisplayText(atc_config_ptr);
            break;
        case ATC_GET_INKEY:
            ATC_ProcessGetInkey(atc_config_ptr);
            break;
        case ATC_GET_INPUT:
            ATC_ProcessGetInput(atc_config_ptr);
            break;
        case ATC_SETUP_CALL:
            ATC_ProcessSetupCall(atc_config_ptr);
            break;
        case ATC_SELECT_ITEM:
            ATC_ProcessSelectItem(atc_config_ptr);
            break;
        case ATC_SEND_SMS:
            ATC_ProcessSendSMS(atc_config_ptr);
            break;
       //@hanjun.liu
        case ATC_PLAY_TONE:
            ATC_ProcessPlayTone(atc_config_ptr);
            break;
        case ATC_SEND_SS:
            ATC_ProcessSendSS(atc_config_ptr);
            break;
        case ATC_SEND_USSD:
            ATC_ProcessSendUSSD(atc_config_ptr);
            break;

            default:
                return_value = ERR_OPERATION_NOT_ALLOWED;
                break;
        }
        if(S_ATC_SUCCESS == return_value)
        {
            s_cmd_stag[dual_sys] = ATC_CMD_STGI;
        }
    }
    else
    {
        return_value = ERR_OPERATION_NOT_ALLOWED;
    }
    return return_value;
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif /*_ATC_ONLY*/
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif
}


/*****************************************************************************/
//     Description : This function handles the AT+STIN command 
//    Global resource dependence : g_rsp_str
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTIN )
{
#if 0
#ifndef _ULTRA_LOW_CODE_
#ifdef _ATC_ONLY
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag || FALSE == atc_config_ptr->sim_flag[dual_sys]
        || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    if(ATC_CMD_TYPE_READ == ATC_GET_CMD_TYPE)
    {
        //Return +STIN: <index>
        if(ATC_CMD_INIT == s_cmd_stag[dual_sys])
        {
            sprintf((char*)g_rsp_str, "%s: %d",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr,
                s_stk_signal[dual_sys].cur_status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            return_value = ERR_OPERATION_NOT_ALLOWED;
        }

    }
    else
    {
        return_value = ERR_OPERATION_NOT_ALLOWED;
    }
    return return_value;
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif /*_ATC_ONLY*/
#else
    return S_ATC_FAIL; 
#endif/*_ULTRA_LOW_CODE_*/
#else
return ERR_OPERATION_NOT_SUPPORTED;
#endif
}

/*****************************************************************************/
//    Description : This function handles the AT+STGR command 
//    Global resource dependence : g_rsp_str
//    Author:       Liuhan
//    Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTGR )
{
#if 0
#ifndef _ULTRA_LOW_CODE_
#ifdef _ATC_ONLY
    uint32  return_value = S_ATC_SUCCESS;
    uint32  para2_num    = 0;
    uint32  para3_num    = 0;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        //Check the parameter
        return_value = ATC_CheckCondAndPara1(atc_config_ptr,PARAM1_FLAG,PARA1.num);
        if(S_ATC_SUCCESS != return_value)
        {
            return return_value;
        }
        if(PARAM2_FLAG)
        {
            para2_num = PARA2.num;
        }
        if(PARAM3_FLAG)
        {
            para3_num = PARA3.num;
        }

        //SCI_TRACE_LOW("ATC:s_stk_signal[dual_sys].cur_status=%d",s_stk_signal[dual_sys].cur_status); 
        switch(s_stk_signal[dual_sys].cur_status)
        {
        case ATC_STATUS_SETUPMENU:
            return_value = ATC_ResponseSetupMenu(dual_sys,PARA1.num,
                PARAM2_FLAG,para2_num,PARAM3_FLAG,para3_num);
            break;

        case ATC_STATUS_DISPLAYTEXT:
            return_value = ATC_ResponseDisplayText(dual_sys,PARA1.num,PARAM2_FLAG,PARAM3_FLAG);
            break;

        case ATC_STATUS_GETINKEY:
            return_value = ATC_ResponseGetInkey(atc_config_ptr,PARA1.num,
                PARAM2_FLAG,para2_num,PARAM3_FLAG,para3_num);
            break;

        case ATC_STATUS_GETINPUT:
            return_value = ATC_ResponseGetInput(atc_config_ptr,PARA1.num,
                PARAM2_FLAG,para2_num,PARAM3_FLAG,para3_num);
            break;

        case ATC_STATUS_SETUPCALL:
        #ifdef _MUX_ENABLE_
            return_value = ATC_ResponseSetupCall(atc_config_ptr,PARA1.num,PARAM2_FLAG,para2_num);
        #else
            return_value = ATC_ResponseSetupCall(PARA1.num,PARAM2_FLAG,para2_num);
        #endif
            break;
    
        case ATC_STATUS_SELECTITEM:
            return_value = ATC_ResponseSelectItem(dual_sys,PARA1.num,
                PARAM2_FLAG,para2_num,PARAM3_FLAG,para3_num);
            break;
        
        case ATC_STATUS_SENDSMS:
             {
                 SIMAT_SEND_SMS_CNF_T sendsms_cnf;
                 SCI_MEMSET(&sendsms_cnf, 0, sizeof(SIMAT_SEND_SMS_CNF_T));
                 
                 if(ATC_BACK_MOVE == PARA1.num || ATC_END_SESSION == PARA1.num)
                 {
                     sendsms_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                     sendsms_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;                     
                     SIMAT_CnfSendSmsEx(dual_sys, &sendsms_cnf);
                 }
             }
            break;
  
        case ATC_STATUS_PLAYTONE:
             {
                 SIMAT_PLAY_TONE_CNF_T play_tone_cnf;
                 SCI_MEMSET(&play_tone_cnf, 0, sizeof(SIMAT_PLAY_TONE_CNF_T));
                 
                 if(ATC_BACK_MOVE == PARA1.num || ATC_END_SESSION == PARA1.num)
                 {
                     play_tone_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                     play_tone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;                     
                     SIMAT_CnfPlayToneEx(dual_sys, &play_tone_cnf);
                 }
             }
            break;
   
        case ATC_STATUS_SENDSS:
            {
                SIMAT_SEND_SS_CNF_T  send_ss_cnf = {0};
                if(ATC_BACK_MOVE == PARA1.num || ATC_END_SESSION == PARA1.num)
                {
                    send_ss_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                    send_ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;                    
                    SIMAT_CnfSendSsEx(dual_sys, &send_ss_cnf);
                }
            }
            break;
   
        case ATC_STATUS_SENDUSSD:
            {
                SIMAT_SEND_USSD_CNF_T send_ussd_cnf = {0};
                if(ATC_BACK_MOVE == PARA1.num || ATC_END_SESSION == PARA1.num)
                {
                    send_ussd_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                    send_ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;                    
                    SIMAT_CnfSendUssdEx(dual_sys, &send_ussd_cnf);
                }
            }
            break;
 
        default:
            //SCI_ASSERT(FALSE);
            break;
        }
        //Kill the timer
        SCI_KILL_TIMER(P_ATC,(ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
            &T_ATC_STSF_EXP_IND[dual_sys],"T_ATC_STSF_EXP_IND");
    }
    else
    {
        return_value = ERR_OPERATION_NOT_ALLOWED;
    }
    return (return_value);
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif /*_ATC_ONLY*/
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
#else
return ERR_OPERATION_NOT_SUPPORTED;
#endif
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//     Description : This function handles the data send from the SIO in get input
//                 and get inkey. 
//    Global resource dependence : g_rsp_str
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern uint32 ATC_ProcessSioStkData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *data_ptr  // pointer to the given pdu buffer
                )
{
    uint8       terminater_char;     // indicates ther terminater char
    uint16      data_len;          // holds the gived pdu length
    BOOLEAN     flag = FALSE;
    
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/


    data_len              = (uint16)strlen((char*)data_ptr);
    terminater_char      = data_ptr[data_len - 1];
    data_ptr[data_len - 1] = '\0';
    data_len             -= 1;
    
    switch (terminater_char)
    {
        // cancel the operation
        case ESC:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            // call the different process
            //SCI_TRACE_LOW("ATC: sio stk data_len = %d",data_len);
            switch(s_stk_signal[dual_sys].cur_status)
            {
                case ATC_STATUS_GETINKEY:
                {
                    flag = ATC_GetInkeyCnf(dual_sys,s_cmd_type[dual_sys],s_result[dual_sys],data_ptr,data_len);
                    break;
                }
            
                case ATC_STATUS_GETINPUT:
                {
                    flag = ATC_GetInputCnf(dual_sys,s_cmd_type[dual_sys],s_result[dual_sys],data_ptr,data_len);
                    break;
                }
            
                default:
                    //SCI_ASSERT(FALSE);
                    break;
            }
            break;
        }
            
        default:
            break;
    }
    
    //SCI_TRACE_LOW("ATC:flag = %d",flag);
    if (!flag)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);         
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);        
    }
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//     Description : give the response when received the get input signal
//    Global resource dependence : s_stk_signal[]
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_StkOutputPrompt(
                                   ATC_CONFIG_T    *atc_config_ptr
                                    )
{
    uint32  return_value = S_ATC_SUCCESS;

    // output the '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_STK);

    return_value = S_ATC_DEFAULT_HANDLE;
    return return_value;
}

/*****************************************************************************/
//     Description : check the index and condition in ATC_ProcessSTGR
//    Global resource dependence : 
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_CheckCondAndPara1(
                                   ATC_CONFIG_T    *atc_config_ptr,
                                   BOOLEAN         para1_flag,
                                   uint8           para1_num
                                )
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //Check if it support Stk
    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag 
        || FALSE == atc_config_ptr->sim_flag[dual_sys] || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    //Check the step
    /*if(ATC_CMD_STGI != s_cmd_stag)
    {
        //SCI_TRACE_LOW:"ATC:s_cmd_stag=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_2446_112_2_17_22_17_30_715,(uint8*)"d",s_cmd_stag);
        return ERR_OPERATION_NOT_ALLOWED;            
    }*/
    // Check the <cmd_type>
    if (!para1_flag)
    {
        return ERR_INVALID_INDEX;
    }
    if((para1_num != s_stk_signal[dual_sys].cur_status) && (para1_num != ATC_BACK_MOVE)
        && (para1_num != ATC_BEYOND_ABILITIE) && (para1_num != ATC_UNABLE_PROCESS)
        && (para1_num != ATC_NO_RESPONSE) && (para1_num != ATC_END_SESSION))
    {
        return ERR_INVALID_INDEX;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//     Description : Give the response when received the setup menu signal
//    Global resource dependence : 
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSetupMenu(
                                   MN_DUAL_SYS_E dual_sys,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num,
                                   BOOLEAN para3_flag,
                                   uint8   para3_num                        
                                )
{
#if 0
    // check the index
    if(((!para2_flag || !para3_flag)&& ATC_RESULT_SELECTITEM == para2_num)
        || (para3_flag && ATC_RESULT_HELPINFO == para2_num))
    {
        return ERR_INVALID_INDEX;
    }

    //Give the response to setup menu
    if(!(ATC_SETUP_MENU == para1_num 
        && ATC_MainMenuCnf(dual_sys,para2_num,para3_num)))
    {
        //SCI_TRACE_LOW:"ATC:ATC_MainMenuCnf=FALSE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_2488_112_2_17_22_17_31_716,(uint8*)"");
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
#else
    return S_ATC_SUCCESS;
#endif
}

/*****************************************************************************/
//     Description : Give the response when received the display text signal
//    Global resource dependence : 
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseDisplayText(
                                   MN_DUAL_SYS_E dual_sys,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   BOOLEAN para3_flag
                                )
{
#if 0
    if(para2_flag || para3_flag)
    {
        return ERR_INVALID_INDEX;
    }
    if(!ATC_DisplayTextCnf(dual_sys,para1_num))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
#else
    return S_ATC_SUCCESS;
#endif
}

/*****************************************************************************/
//     Description : Give the response when received the get inkey signal
//    Global resource dependence : 
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseGetInkey(
                                   ATC_CONFIG_T    *atc_config_ptr,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num,
                                   BOOLEAN para3_flag,
                                   uint8   para3_num                        
                                )
{
#if 0
    uint8 * p_data_str = PNULL;//Just a null pointer
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    s_cmd_type[dual_sys] = para1_num;
    s_result[dual_sys]   = para2_num;
    //check the index
    if(para3_flag)
    {
        return ERR_INVALID_INDEX;
    }
    if((ATC_GET_INKEY == para1_num) && (ATC_USER_RESPONSE == para2_num))
    {
        return ATC_StkOutputPrompt(atc_config_ptr);
    }
    else
    {
        if(para2_flag && ATC_GET_INKEY != para1_num)
        {
            return ERR_INVALID_INDEX;
        }
        if(!ATC_GetInkeyCnf(dual_sys,para1_num,para2_num,p_data_str,0))
        {
            return S_ATC_FAIL;
        }
        else
        {
            //return ERR_OPERATION_NOT_ALLOWED;
            return S_ATC_SUCCESS;
        }
    }
#else 
return S_ATC_SUCCESS;
#endif
}

/*****************************************************************************/
//     Description : Give the response when received the get input signal
//    Global resource dependence : 
//  Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseGetInput(
                                   ATC_CONFIG_T    *atc_config_ptr,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num,
                                   BOOLEAN para3_flag,
                                   uint8   para3_num                        
                                )
{
#if 0
    uint8 * p_data_str = PNULL;//Just a null pointer
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    s_cmd_type[dual_sys] = para1_num;
    s_result[dual_sys]   = para2_num;
    //check the index
    if(para3_flag)
    {
        return  ERR_INVALID_INDEX;
    }
    //hanle the input string
    if((ATC_GET_INPUT == para1_num) && (ATC_USER_RESPONSE == para2_num))
    {
        return ATC_StkOutputPrompt(atc_config_ptr);
    }
    //handle the input result
    else
    {
        if(para2_flag && ATC_GET_INPUT != para1_num)
        {
            return ERR_INVALID_INDEX;
        }
        else
        {
            if(!ATC_GetInputCnf(dual_sys,para1_num,para2_num,p_data_str,0))
            {
                return S_ATC_FAIL;
            }
            else
            {
                //return ERR_OPERATION_NOT_ALLOWED;
                return S_ATC_SUCCESS;
            }
        }
    }
#else
return S_ATC_SUCCESS;
#endif    
}

/*****************************************************************************/
//     Description : Give the response when received the setup call signal
//    Global resource dependence : 
//    Author:    Liuhan
//    Note: 
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL uint32 ATC_ResponseSetupCall(
                                   ATC_CONFIG_T    *atc_config_ptr,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num
                                )

#else
LOCAL uint32 ATC_ResponseSetupCall(
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num
                                )
#endif
{    
    //Check the index
    if(!para2_flag)
    {
        return ERR_INVALID_INDEX;
    }
#ifdef _MUX_ENABLE_
    if(!ATC_SetupCallCnf(atc_config_ptr,para1_num,para2_num))
#else
    if(!ATC_SetupCallCnf(para1_num,para2_num))
#endif
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
}

/*****************************************************************************/
//    Description : Give the response when received the select item signal
//    Global resource dependence : 
//    Author:    Liuhan
//    Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSelectItem(
                                   MN_DUAL_SYS_E dual_sys,
                                   uint8   para1_num,
                                   BOOLEAN para2_flag,
                                   uint8   para2_num,
                                   BOOLEAN para3_flag,
                                   uint8   para3_num                        
                                )
{
#if 0
    //check the index
    if(ATC_STATUS_SELECTITEM == para1_num && (!para2_flag||!para3_flag))
    {
        return ERR_INVALID_INDEX;
    }
    if(!ATC_SelectItemCnf(dual_sys,para1_num,para2_num,para3_num))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
        #else
return S_ATC_SUCCESS;
        #endif
             }
  
                 

/*****************************************************************************/
//     Description : This function handle the siganl  SIMAT_PROVIDE_LOCAL_INFO
//    Global resource dependence : none
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
LOCAL void ATC_HandleProvidLocalInfo(
                            MN_DUAL_SYS_E       dual_sys,
                            SIMAT_LOCAL_INFO_TYPE_E info_type
                        )
{
    switch(info_type)
    {
        case SIMAT_LI_LOCATION_INFO: //the location information(MCC,MNC,LACm and Cell identity)
        {
        MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
        SIMAT_LAI_CNF_T lai_cnf;

        plmn_info =  MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
        lai_cnf.lai.lac = plmn_info.lac;
        lai_cnf.lai.cell_id = plmn_info.cell_id;
        lai_cnf.result  = SIMAT_TR_SUCCESS;
        lai_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;        
        SIMAT_CnfLAIEx(dual_sys, &lai_cnf);
        }
        break;
        case SIMAT_LI_IMEI:  //IMEI of the ME
        {
        MN_IMEI_T   imei_arr;
        SIMAT_GET_IMEI_CNF_T imei_cnf;
        if(MNNV_GetIMEIEx(dual_sys, imei_arr))
        {
        SCI_MEMCPY(imei_cnf.imei.imei_arr,imei_arr,SIMAT_IMEI_LEN);
        imei_cnf.result  = SIMAT_TR_SUCCESS;
        imei_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        }
        else
        {            
        //SCI_TRACE_LOW:"ATC: get imei failed."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_2741_112_2_17_22_17_31_717,(uint8*)"");
        imei_cnf.result = SIMAT_TR_MISS_INFO;
        }
        }
        break;
        case SIMAT_LI_NMR:   //network measurement results
        //havent support now
        {
            SIMAT_NMR_BA_CNF_T nmr_cnf;
            nmr_cnf.result = SIMAT_TR_TERMINATED_BY_USER;        
            SIMAT_CnfNMRBAEx(dual_sys, &nmr_cnf);
            break;
        }

        case SIMAT_LI_DT_TZ: //data,time and time zone
        //havent support now
        {
            SIMAT_GET_DT_TZ_CNF_T dt_tz_cnf;
            dt_tz_cnf.result = SIMAT_TR_TERMINATED_BY_USER;        
            SIMAT_CnfGetDttzEx(dual_sys, &dt_tz_cnf);
            break;
        }

        case SIMAT_LI_LANG:  //language setting
        //havent support now
        {
            SIMAT_GET_LANG_CNF_T lang_cnf;
            lang_cnf.result = SIMAT_TR_TERMINATED_BY_USER;        
            SIMAT_CnfGetLangEx(dual_sys, &lang_cnf);
            break;
        }

        case SIMAT_LI_TIMING_AD: //timing advance
        {
            //havent support now
            SIMAT_TIMING_AD_CNF_T time_ad_cnf;
            time_ad_cnf.result = SIMAT_TR_TERMINATED_BY_USER;        
            SIMAT_CnfTimeAdvanceEx(dual_sys, &time_ad_cnf);
            break;
        }

        default:
            break;   
    }
}

/*****************************************************************************/
//     Description : This function handles the STK signal 
//    Global resource dependence : none
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSTKSignal(
                        ATC_CONFIG_T       *atc_config_ptr,
                        xSignalHeaderRec   *sig_ptr
                        )
{
    MN_DUAL_SYS_E     dual_sys = MN_DUAL_SYS_1;
    //SCI_TRACE_LOW("ATC:ATC_ProcessSTKSignal");
    
    switch(sig_ptr->SignalCode)
    {
        case APP_MN_SIMAT_SETUP_MENU_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SETUP_MENU_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));

            //Save the data
            SCI_MEMCPY(&s_stk_signal[dual_sys].setup_menu,
                &(((APP_MN_SIMAT_SETUP_MENU_IND_T*)sig_ptr)->com),sizeof(SIMAT_SETUP_MENU_T));
            //Save in the s_stk_setupmenu
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
            s_stk_signal[dual_sys].is_support_stk = TRUE;
            
            //Send the cnf to SIM.
            if(TRUE == atc_config_ptr->atc_config_nv.stk_actflag)
            {
                if(FALSE == s_is_setup_menu_cnf[dual_sys])
                {
                    s_is_setup_menu_cnf[dual_sys] = TRUE;

                    //Return +STIN: 0
                    sprintf((char*)g_rsp_str, "%s: 0",
                        g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                ATC_InitCnfSetMenu(dual_sys);
            } 
            
            break;
        }

        case APP_MN_SIMAT_DISPLAY_TEXT_IND:
        {
            dual_sys = ((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:STK_DISPLAY_TEXT_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.display_text,
                &(((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->com),
                sizeof(SIMAT_DISPLAY_TEXT_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_DISPLAYTEXT;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            //Return +STIN: 1
            sprintf((char*)g_rsp_str, "%s: 1",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_StartStkTimer(atc_config_ptr,dual_sys);
            break;
        }

        case APP_MN_SIMAT_GET_INKEY_IND:
        {            
            dual_sys = ((APP_MN_SIMAT_GET_INKEY_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));

        //SCI_TRACE_LOW("ATC:STK_GET_INKEY_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.get_inkey,
                &(((APP_MN_SIMAT_GET_INKEY_IND_T*)sig_ptr)->com),sizeof(SIMAT_GET_INKEY_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_GETINKEY;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 2
            sprintf((char*)g_rsp_str, "%s: 2",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_StartStkTimer(atc_config_ptr,dual_sys);
            break;
        }

        case APP_MN_SIMAT_GET_INPUT_IND:
        {
            dual_sys = ((APP_MN_SIMAT_GET_INPUT_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));

        //SCI_TRACE_LOW("ATC:STK_GET_INPUT_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.get_input,
                &(((APP_MN_SIMAT_GET_INPUT_IND_T*)sig_ptr)->com),sizeof(SIMAT_GET_INPUT_T));
             s_stk_signal[dual_sys].is_support_stk = TRUE;
             s_stk_signal[dual_sys].cur_status = ATC_STATUS_GETINPUT;
             
#ifdef _ATC_ONLY
             s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 3
            sprintf((char*)g_rsp_str, "%s: 3",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_StartStkTimer(atc_config_ptr,dual_sys);
            break;
        }
    
        case APP_MN_SIMAT_SETUP_CALL_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SETUP_CALL_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:STK_SETUP_CALL_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.setup_call,
                &(((APP_MN_SIMAT_SETUP_CALL_IND_T*)sig_ptr)->com),sizeof(SIMAT_SETUP_CALL_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPCALL;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 4
            sprintf((char*)g_rsp_str, "%s: 4",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_StartStkTimer(atc_config_ptr,dual_sys);
            break;
        }

        case APP_MN_SIMAT_SELECT_ITEM_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SELECT_ITEM_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:STK_SELECT_ITEM_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.select_item,
                &(((APP_MN_SIMAT_SELECT_ITEM_IND_T*)sig_ptr)->com),sizeof(SIMAT_SELECT_ITEM_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SELECTITEM;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 6
            sprintf((char*)g_rsp_str, "%s: 6",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_StartStkTimer(atc_config_ptr,dual_sys);
            break;
        }

        case APP_MN_SIMAT_SEND_SMS_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SEND_SMS_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:STK_SEND_SMS_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_sms,
                &(((APP_MN_SIMAT_SEND_SMS_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_SM_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDSMS;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 9
            sprintf((char*)g_rsp_str, "%s: 9",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case APP_MN_SIMAT_SEND_SS_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SEND_SS_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:SIMAT_SEND_SS_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_ss,
                &(((APP_MN_SIMAT_SEND_SS_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_SS_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDSS;
            
#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 8
            sprintf((char*)g_rsp_str, "%s: 8",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        case APP_MN_SIMAT_SEND_USSD_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SEND_USSD_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:SIMAT_SEND_USSD_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_ussd,
                &(((APP_MN_SIMAT_SEND_USSD_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_USSD_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDUSSD;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 10
            sprintf((char*)g_rsp_str, "%s: 10",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
    
        case APP_MN_SIMAT_PLAY_TONE_IND:
        {
            dual_sys = ((APP_MN_SIMAT_PLAY_TONE_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:SIMAT_PLAY_TONE_REQ");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.play_tone,
                &(((APP_MN_SIMAT_PLAY_TONE_IND_T*)sig_ptr)->com),sizeof(SIMAT_PLAY_TONE_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_PLAYTONE;

#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            
            //Return +STIN: 10
            sprintf((char*)g_rsp_str, "%s: 5",
                g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case APP_MN_SIMAT_PRO_LOCAL_INFO_IND:
        {
            dual_sys = ((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:SIMAT_PROVIDE_LOCAL_INFO");
            ATC_HandleProvidLocalInfo(dual_sys,((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->com);        
            break;
        }

        case SIMAT_SESSION_TERMINATED_IND:
        {
            dual_sys = ((APP_MN_SIMAT_SESSION_TERMINATED_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));    
            //SCI_TRACE_LOW("ATC:STK_SESSION_TERMINATE_REQ");
            if((ATC_STATUS_SETUPMENU != s_stk_signal[dual_sys].cur_status) 
                 &&(TRUE == atc_config_ptr->atc_config_nv.stk_actflag))
            {
                //Return +STIN: 99
#ifdef _ATC_ONLY
                s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
                sprintf((char*)g_rsp_str, "%s: 99",
                    g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
            break;
        }
            
        //signals which doesnt support
        case SIMAT_EVENT_SETUP_EVENT_LIST: 
        {
            SIMAT_SETUP_EVENT_LIST_CNF_T setup_event_list_cnf;
            //SCI_TRACE_LOW("ATC:SIMAT_EVENT_SETUP_EVENT_LIST");
            setup_event_list_cnf.result = SIMAT_TR_TERMINATED_BY_USER;            
            SIMAT_CnfSetEventListEx(dual_sys, &setup_event_list_cnf);
            break;
        }

        case EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F:  
        {
            SIMAT_SETUP_IDLE_TEXT_CNF_T setup_idletext_cnf;    
            dual_sys = ((APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F");    
            setup_idletext_cnf.result = SIMAT_TR_TERMINATED_BY_USER;            
            SIMAT_CnfSetIdleTextEx(dual_sys, &setup_idletext_cnf);
            break;
        }

        case EV_MN_APP_SIMAT_SEND_DTMF_IND_F: 
        {
            SIMAT_SEND_DTMF_CNF_T dtmf_cnf;
            dual_sys = ((APP_MN_SIMAT_SEND_DTMF_IND_T*)sig_ptr)->dual_sys;
            ATC_SET_CURRENT_LINK_ID_NO_EVENT(atc_config_ptr,ATC_GetDualSimMuxLinkId(dual_sys));
            //SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SEND_DTMF_IND_F");    
            dtmf_cnf.result = SIMAT_TR_TERMINATED_BY_USER;            
            SIMAT_CnfSendDtmfEx(dual_sys, &dtmf_cnf);
            break;
        }

        case EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F:
        {
            //SCI_TRACE_LOW:"ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3082_112_2_17_22_17_32_718,(uint8*)"");
            break;
        }

        case EV_MN_APP_SIMAT_REFRESH_IND_F:    
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_REFRESH"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3087_112_2_17_22_17_32_719,(uint8*)"");
            break;

        case SIMAT_MO_CC_RSP:
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_CC_RSP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3092_112_2_17_22_17_32_720,(uint8*)"");
            break;
        }

        case SIMAT_EVENT_SS_CON_RSP:
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_SS_CON_RSP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3098_112_2_17_22_17_32_721,(uint8*)"");
            break;
        }

        case SIMAT_EVENT_USSD_CON_RSP:
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_USSD_CON_RSP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3104_112_2_17_22_17_32_722,(uint8*)"");
            break;
        }

        case EV_MN_APP_SIMAT_SMS_CTL_TO_MMI_IND_F:
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_SMS_CON_RSP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3110_112_2_17_22_17_32_723,(uint8*)"");
            break;
        }

        case SIMAT_EVENT_SMS_PP_RSP:
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_REFRESH"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3116_112_2_17_22_17_32_724,(uint8*)"");
            break;
        }

        case SIMAT_EVENT_SMS_CB_RSP: 
        {
            //SCI_TRACE_LOW:"ATC:SIMAT_EVENT_SMS_PP_RSP"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3122_112_2_17_22_17_32_725,(uint8*)"");
            break;
        }
            
        default:
            //SCI_ASSERT(FALSE);
            break;
    }
    
    if (s_first_stk_cmd[dual_sys] == 0)
    {
        s_first_stk_cmd[dual_sys] = sig_ptr->SignalCode;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//     Description : This function handle the ATC_STSF_EXP_IND signal 
//    Global resource dependence : none
//  Author:       Liuhan
//    Note:
/*****************************************************************************/
extern ATC_STATUS  ATC_ProcessTimerEXp (
                                        ATC_CONFIG_T       *atc_config_ptr
                                        )
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    switch(s_stk_signal[dual_sys].cur_status)
    {
        case ATC_STATUS_SETUPMENU:
        {
#ifdef _ATC_ONLY
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
#endif
            if(TRUE == atc_config_ptr->atc_config_nv.stk_actflag)
            {
            #ifndef _MUX_REDUCED_MODE_    //haisi ask no stk now.
                sprintf((char*)g_rsp_str, "%s: 99",
                    g_atc_info_table[AT_CMD_STIN].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            #endif
            }
            break;
        }

        case ATC_STATUS_DISPLAYTEXT:
        {
            ATC_DisplayTextCnf(dual_sys,ATC_NO_RESPONSE);
            break;
        }

        case ATC_STATUS_GETINKEY:
        {
            ATC_GetInkeyCnf(dual_sys,ATC_NO_RESPONSE,0,PNULL,0);
            break;
        }

        case ATC_STATUS_GETINPUT:
        {
            ATC_GetInputCnf(dual_sys,ATC_NO_RESPONSE,0,PNULL,0);
            break;
        }

        case ATC_STATUS_SELECTITEM:
        {
            ATC_SelectItemCnf(dual_sys,ATC_NO_RESPONSE,0,0);
            break;
        }

        default:
            break;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process stk pdu mode sms and send it
//  Global resource dependence : s_stk_send_sms[]
//  Author: Elvis Xu
//  Note: 
/*****************************************************************************/
#ifdef _MUX_ENABLE_
LOCAL BOOLEAN HandleStkPduModeSms(
             ATC_CONFIG_T    *atc_config_ptr,
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#else
LOCAL BOOLEAN HandleStkPduModeSms(
             uint8         *pdu_ptr,       // point to the pdu buffer
             uint16         pdu_len
             )
#endif
{
    MN_CALLED_NUMBER_T    sc_addr;//The address of SMS
    MN_L3_MSG_UNIT_T        tpdu = {0};  
    BOOLEAN                          is_need_compress = FALSE;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != pdu_ptr); /*assert verified check NULL pointer*/
    SCI_MEMCPY(&sc_addr, &s_stk_send_sms[dual_sys].address, sizeof(MN_CALLED_NUMBER_T));

    //SCI_TRACE_LOW:"ATC: HandleStkPduModeSms"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3222_112_2_17_22_17_32_726,(uint8*)"");

    if(sc_addr.num_len < 1)
    {
        SCI_TRACE_HIGH("ATC: HandleStkPduModeSms sc_addr.length is 0");
        return FALSE;
    }
    if(sc_addr.num_len > MN_SMS_ADDR_MAX_LEN)
    {
        //SCI_TRACE_LOW:"ATC: HandleStkPduModeSms sc_addr.length = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3231_112_2_17_22_17_32_727,(uint8*)"d", sc_addr.num_len);
        sc_addr.num_len = MN_SMS_ADDR_MAX_LEN;
    }

    if ((MN_MAX_L3_MESSAGE_SIZE < (pdu_len / 2))
        ||!ConvertHexToBin(pdu_ptr, pdu_len, tpdu.l3_msg))
    {
        //SCI_TRACE_LOW:"ATC:error ConvertHexToBin"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3238_112_2_17_22_17_32_728,(uint8*)"");
        return FALSE;
    }
    tpdu.length = (uint16)(pdu_len / 2);

    //Send STK SMS 
    g_is_stk_sendsms[dual_sys] = TRUE;
    is_need_compress = s_stk_send_sms[dual_sys].is_packet_IND;

    if (ERR_MNSMS_NONE !=  MNSMS_SendSmsPDUEx(dual_sys, TRUE, &sc_addr, &tpdu, is_need_compress, FALSE))
    {
        //SCI_TRACE_LOW:"ATC: Send Sms PDU failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3252_112_2_17_22_17_32_729,(uint8*)"");
        return FALSE;
    }
    else
    {
        ATC_ADD_EXPECTED_EVENT(atc_config_ptr, APP_MN_WRITE_SMS_CNF, 0xff, ATC_NO_DOMAIN);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function handle the STK SMS received from SIO
//  Global resource dependence : 
//  Author:       Elvis Xu
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSioStkSmsData(
                ATC_CONFIG_T    *atc_config_ptr,
                uint8           *pdu_ptr  // pointer to the given pdu buffer
                )
{
    uint8            terminater_char;     // indicates ther terminater char
    uint16          pdu_len = 0;                  // holds the gived pdu length
    BOOLEAN     flag = FALSE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != pdu_ptr);  /*assert verified check NULL pointer*/
    //SCI_TRACE_LOW:"ATC: ATC_ProcessSioStkSmsData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3278_112_2_17_22_17_32_730,(uint8*)"");

    pdu_len               = (uint16)strlen((char*)pdu_ptr);
    terminater_char   = pdu_ptr[pdu_len - 1];
    pdu_ptr[pdu_len - 1] = '\0';
    pdu_len -= 1;

    switch (terminater_char)
    {
        case ESC:
        {
            // cancel the operation
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            SCI_MEMSET(&s_stk_send_sms[dual_sys], 0, sizeof(ATC_STK_SEND_SM_T));
            flag = TRUE;
            break;
        }

        case CTRL_Z:
        {
            // pdu mode SMS
            if ((s_stk_send_sms[dual_sys].tpdu.length <= (pdu_len / 2))
                && (!atc_config_ptr->atc_config_nv.cmgf_flag))
            {
            #ifdef _MUX_ENABLE_
                flag = HandleStkPduModeSms(atc_config_ptr,pdu_ptr, pdu_len);
            #else
                flag = HandleStkPduModeSms(pdu_ptr, pdu_len);
            #endif
            }
            break;
        }

        default:
            break;
    }

    if (!flag)
    {
        ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_304);         
        SCI_MEMSET(&s_stk_send_sms[dual_sys], 0, sizeof(ATC_STK_SEND_SM_T));
        return S_ATC_FAIL;
    }
    return S_ATC_DEFAULT_HANDLE;
}



/*****************************************************************************/
//  Description : This function handles the APP_MN_SIMAT_REFRESH_CNF message and send
//  info to APP.
//  Global resource dependence : none
//  Author:      Elvis Xu 
/*****************************************************************************/
extern ATC_STATUS ATC_ProcessSIMRefreshCnf(
                        ATC_CONFIG_T    *atc_config_ptr,
                        APP_MN_SIMAT_REFRESH_CNF_T    *sig_ptr
                        )
{
    SIMAT_REFRESH_RESULT_T    refresh_result = {0};
    BOOLEAN    is_adn_file_changed = FALSE;      //whether adn file changed
    BOOLEAN    is_sms_changed = FALSE;            //whether sms file changed
    BOOLEAN    is_ok = FALSE;                               //whether success
    uint8           sim_file = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW:"ATC: ATC_ProcessSIMRefreshCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3342_112_2_17_22_17_32_731,(uint8*)"");
    
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != sig_ptr);   /*assert verified check NULL pointer*/

    refresh_result = sig_ptr->result;
    is_ok = refresh_result.is_ok;

    if (is_ok && refresh_result.is_file_changed)
    {
        for (sim_file = MN_SIMFILE_START + 1; sim_file < MN_SIMFILE_END; sim_file++)
        {
            if (refresh_result.flag_arr[sim_file])
            {
                switch (sim_file)
                {
                    case MN_SIMFILE_ADN:
                    {
                        is_adn_file_changed = TRUE;
                        break;
                    }

                    case MN_SIMFILE_SMS:
                    {
                        is_sms_changed = TRUE;
                        break;
                    }

                    case MN_SIMFILE_MSISDN:  //本机号码
                    case MN_SIMFILE_FDN:        //固定拨号
                    case MN_SIMFILE_SMSP:      //短消息参数
                    case MN_SIMFILE_SMSS:      //短消息状态
                    default:
                    {
                        //SCI_TRACE_LOW:"OnRefreshCnfSimFileChange:sim_file = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3376_112_2_17_22_17_32_732,(uint8*)"d", sim_file);
                        break;
                    }
                }
            }
        }
    }

    sprintf((char*)g_rsp_str, "%s: %d,%d,%d",\
                 g_atc_info_table[AT_CMD_STKREFRESH].cmd_name->str_ptr, is_ok, is_adn_file_changed, is_sms_changed);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    if (is_ok)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_UNKNOWN); 
    }

    //to close protecting mechanism
    s_stk_refresh_protect[dual_sys] = FALSE;

    return S_ATC_DEFAULT_HANDLE;
}

#endif /* _ULTRA_LOW_CODE_ */


/*****************************************************************************/
//  Description : This function handles the AT+PSSTKE command.This command is used
//  as STK Envelope Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKE)
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    uint32   status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_ 
    SIMAT_AT_CMD_ENVELOPE_IND_T envelop_ind;    
    BOOLEAN convert_rst = FALSE;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW:"ATC: ATC_ProcessPSSTKE"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3421_112_2_17_22_17_33_733,(uint8*)"");
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                envelop_ind.apdu_len = PARA1.str_ptr->str_len/2;
                SCI_MEMSET(&envelop_ind.apdu[0], 0, SIMAT_MAX_APDU_DATA_LEN);
                convert_rst = ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len, &envelop_ind.apdu[0]);
                
                if(convert_rst)
                {
                    SIMAT_IndATCmdEnvelopeEx((uint8)dual_sys, &envelop_ind);
                }
            }
            
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;    
            break;
        }
    }
#endif
    return status;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKR command.This command is used
//  as STK Response Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKR)
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    uint32   status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_ 
    SIMAT_AT_CMD_TERMINAL_RES_T terminal_ind;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    BOOLEAN convert_rst = FALSE;
    //SCI_TRACE_LOW:"ATC: ATC_ProcessPSSTKR"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3466_112_2_17_22_17_33_734,(uint8*)"");
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                terminal_ind.apdu_len = PARA1.str_ptr->str_len/2;
                SCI_MEMSET(&terminal_ind.apdu[0], 0, SIMAT_MAX_APDU_DATA_LEN);
                convert_rst = ConvertHexToBin(PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len, &terminal_ind.apdu[0]);
                if(convert_rst)
                    SIMAT_TerminalResponseEx((uint8)dual_sys, &terminal_ind);
            }
            
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;   
            break;
        }
    }
#endif
    return status;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKI command.This command is used
//  as STK setup menu display Cmd. transparent display mode.
//  Global resource dependence : g_rsp_str
//  Author:       
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKI)
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    uint32   status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_ 
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    uint8 temp_chr[SIMAT_MAX_RPDU_DATA_LEN*2+1];
    int16 i;
    //SCI_TRACE_LOW:"ATC:ATC_ProcessPSSTKI"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3508_112_2_17_22_17_33_735,(uint8*)"");
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if(1 == PARA1.num)
                {
                    dual_sys = ATC_CURRENT_SIM_ID_MUX;
                    if((dual_sys >= MN_DUAL_SYS_1) && (dual_sys < MN_DUAL_SYS_MAX))
                    {
                        s_setup_menu_request_cnt[dual_sys]++;
                        //SCI_TRACE_LOW:"ATC: save stk %d menu cnt = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3521_112_2_17_22_17_33_736,(uint8*)"dd", dual_sys, s_setup_menu_request_cnt[dual_sys]);
                        
                        if(s_setup_menu_len[dual_sys]>0)
                        {
                            //SCI_TRACE_LOW:"ATC: send stk menu cnt = %d, len = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3525_112_2_17_22_17_33_737,(uint8*)"dd", s_setup_menu_request_cnt[dual_sys], s_setup_menu_len[dual_sys]);
                            
                            for(i=0; i<s_setup_menu_request_cnt[dual_sys]; i++)
                            {
                                SCI_MEMSET((uint8 *)g_rsp_str, 0, MAX_ATC_RSP_LEN);
                                SCI_MEMSET((uint8 *)temp_chr, 0, SIMAT_MAX_RPDU_DATA_LEN*2+1);
                                ConvertBinToHex(s_setup_menu_data[dual_sys], s_setup_menu_len[dual_sys], temp_chr);
                                sprintf((char*)g_rsp_str, "%%SATI: \"%s\"", temp_chr);
                                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                            }
                            s_setup_menu_request_cnt[dual_sys] = 0;
                        }
                    }
                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                }
            }
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
#endif
    return status;
#endif
}

/*****************************************************************************/
//  Description : This function handles the STK signal 
//  Global resource dependence : none
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSIMATFwdInd(
                        ATC_CONFIG_T       *atc_config_ptr,
                        xSignalHeaderRec   *sig_ptr
                        )
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    uint8 temp_chr[SIMAT_MAX_RPDU_DATA_LEN*2+1];
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    int16  i;
    
    if(((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.is_setup_menu)
    {
        dual_sys = ((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->dual_sys;
        
        if((dual_sys >= MN_DUAL_SYS_1) && (dual_sys < MN_DUAL_SYS_MAX))
        {
            s_setup_menu_len[dual_sys] = ((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.data_len;
            SCI_MEMCPY(&s_setup_menu_data[dual_sys][0], ((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.data_arr, ((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.data_len);
            if(s_setup_menu_len[dual_sys] > 0 && s_setup_menu_request_cnt[dual_sys] > 0)
            {
                //SCI_TRACE_LOW:"ATC: stk %d setup menu cnt = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3584_112_2_17_22_17_33_738,(uint8*)"dd",dual_sys,s_setup_menu_request_cnt[dual_sys]);
                for(i=0; i<s_setup_menu_request_cnt[dual_sys]; i++)
                {
                    SCI_MEMSET((uint8 *)g_rsp_str, 0, MAX_ATC_RSP_LEN);
                    SCI_MEMSET((uint8 *)temp_chr, 0, SIMAT_MAX_RPDU_DATA_LEN*2+1);
                    ConvertBinToHex(s_setup_menu_data[dual_sys], s_setup_menu_len[dual_sys], temp_chr);
                    sprintf((char*)g_rsp_str, "%%SATI: \"%s\"", temp_chr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                s_setup_menu_request_cnt[dual_sys] = 0;
            }
        } 
    }
    else
    {
        SCI_MEMSET((uint8 *)g_rsp_str, 0, MAX_ATC_RSP_LEN);
        SCI_MEMSET((uint8 *)temp_chr, 0, SIMAT_MAX_RPDU_DATA_LEN*2+1);
        ConvertBinToHex(((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.data_arr, \
            ((APP_MN_SIMAT_FWD_PC_IND_T *)sig_ptr)->com.data_len, temp_chr);
        sprintf((char*)g_rsp_str, "%%SATI: \"%s\"", temp_chr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    
    return S_ATC_DEFAULT_HANDLE;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+PSSTKSMS command, which used to send sms under 
//                      transparent display mode
//  Global resource dependence : 
//  Author:       Elvis Xu
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessPSSTKSMS)
{
#ifndef _ULTRA_LOW_CODE_
    uint8     num_array[MN_MAX_ADDR_BCD_LEN] = {0};
    MN_CALLED_NUMBER_T * address_ptr = PNULL;
    MN_DUAL_SYS_E    dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //SCI_TRACE_LOW:"ATC: ATC_ProcessPSSTKSMS"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3622_112_2_17_22_17_33_739,(uint8*)"");
    // check the parameter
    if (!PARAM1_FLAG ||!PARAM2_FLAG || !PARAM3_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_SET)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if ((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC) 
        || (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC) 
        || (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
    {
        return ERR_INVALID_INDEX;
    }

    SCI_MEMSET((void *)&s_stk_send_sms[dual_sys], 0, sizeof(ATC_STK_SEND_SM_T));
    s_stk_send_sms[dual_sys].is_packet_IND = (BOOLEAN)PARA1.num;
    address_ptr = &s_stk_send_sms[dual_sys].address;

    if (atc_config_ptr->atc_config_nv.cmgf_flag && 
        (ATC_STK_MIN_SMS_LEN > PARA2.num 
        || MN_MAX_L3_MESSAGE_SIZE < PARA2.num))
    {
        return ERR_INVALID_INDEX;
    } 
    else
    {
        s_stk_send_sms[dual_sys].tpdu.length= (uint16)PARA2.num;
    }

    if (!PARA3.num )
    {
        s_stk_send_sms[dual_sys].is_address = FALSE;
        SCI_MEMCPY(address_ptr, &g_sc_addr[dual_sys], sizeof(MN_CALLED_NUMBER_T));
    }
    else
    {
        if (!PARAM4_FLAG)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        
        if (PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
        {
            return ERR_INVALID_INDEX;
        }
        
        if((1 < PARA3.num) ||(PNULL == PARA4.str_ptr->str_ptr) ||
            (2*MN_MAX_ADDR_BCD_LEN < PARA4.str_ptr->str_len))
        {
            return ERR_INVALID_INDEX;
        }
    
        s_stk_send_sms[dual_sys].is_address = TRUE;
        if (!ConvertHexToBin(PARA4.str_ptr->str_ptr, PARA4.str_ptr->str_len, num_array))
        {
            //SCI_TRACE_LOW:"ATC: convert tele string failed!\r\n  PARA4.str_ptr->str_ptr:%s "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3677_112_2_17_22_17_33_740,(uint8*)"s", PARA4.str_ptr->str_ptr);
            return ERR_INVALID_INDEX; 
        }

        address_ptr->number_type = (MN_NUMBER_TYPE_E)((num_array[0] & 0x70) >> 4);
        address_ptr->number_plan = (MN_NUMBER_PLAN_E)(num_array[0] & 0x0f);
        address_ptr->num_len = PARA4.str_ptr->str_len/2 - 1 ;
        SCI_MEMCPY((void *)address_ptr->party_num, &num_array[1], address_ptr->num_len);
    }

    s_stk_send_sms[dual_sys].cmd_id = AT_CMD_PSSTKSMS;
    // output the character '>'
    ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_STKSMS);

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}


/*****************************************************************************/
//  Description : This function handles the AT+STKREFRESH command.This command is used
//  refresh STK sim files etc.
//  Global resource dependence : none
//  Author:      Elvis Xu 
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSTKREFRESH)
{
#ifndef _ULTRA_LOW_CODE_
    SIMAT_REFRESH_T refresh = {0};  // the refresh type and the refresh file
    uint8 *file_data_ptr = PNULL;
    uint8   file_data_len = 0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    //SCI_TRACE_LOW:"ATC: ATC_ProcessSTKREFRESH"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3733_112_2_17_22_17_33_741,(uint8*)"");

    //mechanism protecting sim files from being refreshed again within allowed intervals
    if (s_stk_refresh_protect[dual_sys])
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if (!PARAM1_FLAG || !PARAM2_FLAG ||(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if ((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
        || (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
    {
        return ERR_INVALID_INDEX;
    }

    if (PARA1.num <= SIMAT_RF_RESET && PARA2.num <= 1)
    {
        refresh.refresh_type = (SIMAT_REFRESH_TYPE_E)PARA1.num;
        refresh.is_files_exist = (BOOLEAN)PARA2.num;
    }
    else
    {
        return ERR_INVALID_INDEX;
    }

    if (refresh.is_files_exist)
    {
        if (!PARAM3_FLAG)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
                
        if (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
        {
            return ERR_INVALID_INDEX;
        }

        if((PNULL == PARA3.str_ptr->str_ptr) ||(2*ATC_STK_MAX_FILE_LIST_LEN < PARA3.str_ptr->str_len))
        {
            return ERR_INVALID_INDEX;
        }

        file_data_len = PARA3.str_ptr->str_len / 2;
        file_data_ptr = (uint8 *)SCI_ALLOC_BASE(file_data_len + 1);
        SCI_ASSERT(PNULL != file_data_ptr); /*assert verified check NULL pointer*/
        SCI_MEMSET((void *)file_data_ptr, 0, file_data_len + 1);

        if (!ConvertHexToBin(PARA3.str_ptr->str_ptr, PARA3.str_ptr->str_len, file_data_ptr))
        {
            //SCI_TRACE_LOW:"ATC: convert file list string failed!\r\n  PARA3.str_ptr->str_ptr:%s "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3786_112_2_17_22_17_33_742,(uint8*)"s", PARA3.str_ptr->str_ptr);
            SCI_FREE(file_data_ptr);
            return ERR_INVALID_INDEX; 
        }

        if (!SIMAT_DecodeFileListEx(file_data_len, file_data_ptr, &refresh.file_list))
        {
            //SCI_TRACE_LOW:"ATC: file list decode failed!\r\n  data_len: %d, data_ptr: %p "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_STK_3793_112_2_17_22_17_33_743,(uint8*)"d", file_data_len, file_data_ptr);
            SCI_FREE(file_data_ptr);
            return ERR_INVALID_INDEX; 
        }
      
    }
    else
    {
        if (PARAM3_FLAG)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }

    //to open protecting mechanism
    s_stk_refresh_protect[dual_sys] = TRUE;

    //send stk refresh request to the sim
    MNSIM_RefreshEx(dual_sys, refresh);

    if (PNULL != file_data_ptr)
    {
        SCI_FREE(file_data_ptr);
    }

    return S_ATC_DEFAULT_HANDLE;
#else
    return S_ATC_FAIL;
#endif/*_ULTRA_LOW_CODE_*/
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
