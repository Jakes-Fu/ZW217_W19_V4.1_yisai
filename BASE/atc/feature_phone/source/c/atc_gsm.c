/******************************************************************************
 ** File Name:      atc_gsm.c                                                 *
 ** Author:         Xueli.lv                                                  *
 ** Date:           12/08/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 12/08/2002     Xueli.lv         Create.                                   *
 ******************************************************************************/
 
#include "base_trc.h"
#include "atc_gsm.h"
#include "stdio.h"
#include "atc_common.h"
#include "atc_gsm_ss.h"
#include "atc_basic_cmd.h"
#if 0
#include "atc_gsm_stk.h"
#endif

#ifndef _ULTRA_LOW_CODE_
#include "layer1_engineering.h"
#include "atc_sleep.h"
#endif

#include "sig_code.h"
#include "env_atc_signal.h"
#include "sci_api.h"
#include "audio_api.h"
#ifdef PPP_USB
#include "ppp_mp_api.h"
#endif
 /**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/

extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T      g_atc_info_table[];
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
 
#if defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
extern BOOLEAN g_is_hang_up_data_call;  // used as +++ATH flag
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
#if 0
extern BOOLEAN               g_is_stk_call[MN_SYS_NUMBER];
#endif
uint8    g_select_mode[MN_SYS_NUMBER] =
{
#ifdef MULTI_SIM_SYS_DUAL
    AUTO_MODE,
#elif defined MULTI_SIM_SYS_TRI
    AUTO_MODE,
    AUTO_MODE,
#elif defined MULTI_SIM_SYS_QUAD
    AUTO_MODE,
    AUTO_MODE,
    AUTO_MODE,
#endif
    AUTO_MODE
};

static ATC_PLMN_FORMAT_E     s_plmn_format[MN_SYS_NUMBER] =
{
#ifdef MULTI_SIM_SYS_DUAL
    PLMN_FORMAT_NUMBERIC,
#elif defined MULTI_SIM_SYS_TRI
    PLMN_FORMAT_NUMBERIC,
    PLMN_FORMAT_NUMBERIC,
#elif defined MULTI_SIM_SYS_QUAD
    PLMN_FORMAT_NUMBERIC,
    PLMN_FORMAT_NUMBERIC,
    PLMN_FORMAT_NUMBERIC,
#endif
    PLMN_FORMAT_NUMBERIC
};

extern ATC_CREG_STATE_T      g_creg_state[MN_DUAL_SYS_MAX];

#endif

static uint8                 s_tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1] = {0};
static ATC_TELE_TYPE_E       s_tel_num_type = UNKNOWN_PLAN_UNKNOWN_TYPE;
static ATC_CALLING_NUM_T     s_mt_calling_num;
BOOLEAN                      g_atc_call_need_disc = FALSE;
BOOLEAN                      g_atc_call_flag = FALSE;
BOOLEAN                      g_atc_expire_ind = FALSE;
MN_DUAL_SYS_E                g_atc_modem_dial_sys = MN_DUAL_SYS_1;

extern uint16                ring_times[MN_SYS_NUMBER]; 
extern uint16                have_ringed_times[MN_SYS_NUMBER];

uint8                        g_tel_str[2*MN_MAX_ADDR_BCD_LEN + 1];  

SCI_TMID                     T_ATC_CALL_EXP_IND = PNULL;

#ifdef _MUX_ENABLE_
BOOLEAN                      g_atc_international_call = FALSE;
BOOLEAN                      ppp_dialing_in_progress = FALSE;
extern BOOLEAN               g_atc_sms_flag;
extern BOOLEAN               g_ignore_sms_result;

static uint32 s_pdp_id_modem = 0;

BOOLEAN is_cmux_recv = FALSE;

MUX_SYS_PARA_T cmux_sys_para = {
    MUX_OPTION_BASIC,
    MUX_FRAME_UIH,
    MUX_BAUDRATE_115200,
    31,10,3,30,10,2,
    {0}
};
extern uint8 atc_urc_link_id;
extern uint8 g_call_exp_link_id;
extern uint8 g_mo_call_link_id;
extern BOOLEAN RRA_CheckTrafficState(uint8 req_rr);
extern BOOLEAN RRA_IsAnswerToPaging(void);
extern void GPRS_SendForceDeactivatePdpContext(MN_DUAL_SYS_E dual_sys, uint32 pdp_id);
extern void GPRS_ClearPdpStateAndResource(MN_DUAL_SYS_E  dual_sys, uint32  pdp_id);
#endif

#ifndef _MUX_ENABLE_
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

typedef enum
{
    GPRS_DIAL_99,
    GPRS_DIAL_98,
    GPRS_DIAL_MAX
}GPRS_DIAL_TYPE_E;

/*****************************************************************************/
//  Description:    This function is used to set voice codec enable/disable.
//  Author:         shujing.dong
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_EnableVoiceCodec ( // If succeed, return SCI_TRUE,
    // else return error value
    BOOLEAN is_enable                   // SCI_TRUE: Enable,   SCI_FALSE: Disable
);

/*****************************************************************************/

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)
/*****************************************************************************/
//  Description: This function transform PLMN structure to numeric string 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void TransformPlmnToNumeric(
                MN_PLMN_T     *plmn_ptr,   // IN: the plmn
                uint8         *numeric_str // OUT: the numeric string of the plmn
                );
/*****************************************************************************/
//  Description : This function transform PLMN structure to numeric string 
//  Global resource dependence : none
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
LOCAL void TransformNumericToPlmn(
                MN_PLMN_T     *plmn_ptr,   // out: the plmn
                uint8         *numeric_str // in: the numeric string of the plmn
                );
/*****************************************************************************/
//  Description: This function handle the AT+COPS set command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ManualSelectPlmn(
                 MN_DUAL_SYS_E dual_sys,
                 ATC_PLMN_FORMAT_E         oper_format,
                 ATC_CUR_PARAM_STRING_T    *oper_ptr,
                 uint32     rat
                );
#endif
/*****************************************************************************/
//  Description: This function is called to send the DTMF string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void SendDtmfStr(
            MN_DUAL_SYS_E dual_sys,
            uint8          call_id,
            uint8          length,
            uint8          *str
            );
/*****************************************************************************/
//  Description: This function checks the DTMF string 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDtmfStr(
            uint8          length,
            uint8          *str
            );
#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description: This function checks the CLIR call string 
//  Global resource dependence : none
//  Author:       Jason
//  Return:       TRUE - clir call; FALSE - non clir call
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleClirCall(
            uint8          *str,     // [in out]: call string pointer
            uint32         *length,  // [in out]: length of call string
            MN_CALL_CLIR_E *clir     // [out]: clir type
            );
#endif

LOCAL ATC_STATUS ATC_ProcessGprsDial( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8  *dial_string_ptr // point to the dial string
                );

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description: This function transform PLMN structure to numeric string 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void TransformPlmnToNumeric(
                MN_PLMN_T     *plmn_ptr,   // IN: the plmn
                uint8         *numeric_str // OUT: the numeric string of the plmn
                )
{
    uint16           temp;

    SCI_ASSERT((PNULL != plmn_ptr) && (PNULL != numeric_str)); /*assert verified*/
    temp = plmn_ptr->mcc;
    numeric_str[2] = (uint8)(temp % 10) + '0';       //MCC digit3
    temp /= 10;
    numeric_str[1] = (uint8)(temp % 10) + '0';       //MCC digit2
    temp /= 10;
    numeric_str[0] = (uint8)(temp % 10) + '0';       //MCC digit1

    temp = plmn_ptr->mnc;
    if (temp >= 100)
    {
        numeric_str[5] = (uint8)(temp % 10) + '0';   //MNC digit3
        temp /= 10;
        numeric_str[4] = (uint8)(temp % 10) + '0';   //MNC digit2
        temp /= 10;
        numeric_str[3] = (uint8)(temp % 10) + '0';   //MNC digit1
    }
    else
    {
        numeric_str[4] = (uint8)(temp % 10) + '0';   //MNC digit2
        temp /= 10;
        numeric_str[3] = (uint8)(temp % 10) + '0';   //MNC digit1     
    }
}

/*****************************************************************************/
//  Description: This function handle the AT+COPS set command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ManualSelectPlmn(
            MN_DUAL_SYS_E             dual_sys,
            ATC_PLMN_FORMAT_E         oper_format,
            ATC_CUR_PARAM_STRING_T    *oper_ptr,
            uint32                    rat
            )
{
    MN_PLMN_T                       selected_plmn;    // the selected PLMN used in the manual selection mode
    MN_GMMREG_RAT_E                 set_rat;
    int32                           plmn;
    char                            oper_str[6+1] = {0};
    int32                           oper_len = 0;

    /* Get rat */
    switch(rat)
    {
        case 0:
        case 1:
        {
            set_rat = MN_GMMREG_RAT_GPRS;
            break;
        }
    
        case 2:
        {
            set_rat = MN_GMMREG_RAT_3G;
            break;
        }
        
        default:
        {
            set_rat = MN_GMMREG_RAT_UNKNOWN;
            break;
        }
    }

    /* Manual PLMN selection with operation format. */
    switch(oper_format)
    {
#if 0   //delete full or short operator name
        case PLMN_FORMAT_FULL_NAME:
            plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_FULL_NAME, oper_ptr->str_ptr);

            if(!plmn_detailed_info)
            {
                //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, FULL_NAME, not found!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1805_112_2_17_22_17_4_534,(uint8*)"");
                return S_ATC_FAIL;
            }

            sprintf(oper_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
            oper_len = strlen(oper_str);
            break;

        case PLMN_FORMAT_SHORT_NAME:
            plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_SHORT_NAME, oper_ptr->str_ptr);

            if(!plmn_detailed_info)
            {
                //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, SHORT_NAME, not found!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1818_112_2_17_22_17_4_535,(uint8*)"");
                return S_ATC_FAIL;
            }

            sprintf(oper_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
            oper_len = strlen(oper_str);
            break;
#else
        case PLMN_FORMAT_FULL_NAME:
        case PLMN_FORMAT_SHORT_NAME:
             return  ERR_OPERATION_NOT_SUPPORTED;
#endif

        default: /* PLMN_FORMAT_NUMBERIC */
        {
            #ifdef _ATC_SYNC_TOOL_
            if(!CheckPlmnStr(oper_ptr->str_len, oper_ptr->str_ptr))
            {
                //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1835_112_2_17_22_17_4_536,(uint8*)"");
                return S_ATC_FAIL;
            }

            strcpy(oper_str, (char *)oper_ptr->str_ptr);
            oper_len = oper_ptr->str_len;
            #endif
            break;
        }
    }

    plmn = atoi(oper_str);
    //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, plmn = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1846_112_2_17_22_17_4_537,(uint8*)"d", plmn);

    if(oper_len == 6)
    {
        selected_plmn.mnc_digit_num = 3;
        selected_plmn.mcc = plmn / 1000;
        selected_plmn.mnc = plmn % 1000;
    }
    else if(oper_len == 5)
    {
        selected_plmn.mnc_digit_num = 2;
        selected_plmn.mcc = plmn / 100 ;
        selected_plmn.mnc = plmn % 100;
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, cops not support this plmn"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1862_112_2_17_22_17_4_538,(uint8*)"");
        return S_ATC_FAIL;
    }

    if(ERR_MNPHONE_NO_ERR != MNPHONE_SelectPLMNEx(dual_sys, TRUE, selected_plmn, set_rat))

    {
        //SCI_TRACE_LOW:"ATC: ManualSelectPlmn, failure in MN"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1872_112_2_17_22_17_4_539,(uint8*)"");
        return S_ATC_FAIL;
    }

    s_plmn_format[dual_sys] = oper_format;
    return S_ATC_SUCCESS;

}

#endif

/*lint -e508*/
/*****************************************************************************/
//  Description: This function handle the ATA command,This command is used to 
//                answer a single call. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessA )
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
       
    SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
    g_atc_expire_ind = FALSE;

    if (g_calls_state[dual_sys].call_context[0].call_entity_state == ATC_CALL_INCOMING_STATE)
    {
        // ask the MN task to connect the incoming call
        if(MN_RETURN_SUCCESS == MNCALL_ConnectCallEx(dual_sys, g_calls_state[dual_sys].call_context[0].call_id, PNULL))
        {
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                                                            (uint16)APP_MN_SETUP_COMPLETE_IND,
                                                            g_calls_state[dual_sys].call_context[0].call_id,
                                                            ATC_DOMAIN_CS);
        }
        else
        {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessA, MNCALL_ConnectCall failure!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_242_112_2_17_22_17_0_511,(uint8*)"");
            return ERR_UNKNOWN;
        }

        AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
        AUDIO_SetVolume(atc_config_ptr->speaker_volume);
        ATC_EnableVoiceCodec(TRUE);
        atc_config_ptr->audio_flag = TRUE;
        g_atc_call_flag = TRUE;
    }
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
    }
    return S_ATC_DEFAULT_HANDLE;
}
#ifdef PLATFORM_UWS6121E
#ifdef PPP_USB
LOCAL ATC_STATUS ATC_ProcessPPPDial( // return the handle result
    ATC_CONFIG_T    *atc_config_ptr, /* point to the ATC_CONFIG_T struct.*/
    uint8  *dial_string_ptr, /*point to the dial string*/
    uint8   dial_string_len, /*dial string length*/
    uint8   link_id
)
{
    uint8               cid = 0;
    MN_DUAL_SYS_E  dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if((!memcmp(dial_string_ptr,"*99",3)) || (!memcmp(dial_string_ptr,"*98",3)))
    {
        if((4 == dial_string_len) && (dial_string_ptr[3] == '#'))
        {
            /* for *99# or *98#, don't take the cid situaiton, pdp id will use the cid 1*/
            cid = 1;
            SCI_TRACE_LOW("ATC:  ATC_ProcessGprsDial: atd gprs string: *99# or *98#");
        }
        else if((8 == dial_string_len) 
                   && !memcmp(dial_string_ptr + 3,"***",3)
                   && (dial_string_ptr[6] >= '1'  &&  dial_string_ptr[6] <= '7')
                   && ('#' == dial_string_ptr[7]))
        {
            /* for *99***cid# or *98***cid , cid from 1-7*/
            cid = dial_string_ptr[6] - '0';
            SCI_TRACE_LOW( "ATC: ATC_ProcessGprsDial,*99***cid# or *98***cid, cid = %d ", 1, (cid));  
        }
        else
        {
            SCI_TRACE_LOW( "ATC:  atd gprs string format wrong!");
            return   ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
        }
    }
    else
    {
        SCI_TRACE_LOW( "ATC:  atd gprs string format wrong1!");    
        return   ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
    }
    
    if(PPP_E_SUCCESS == PPP_Start(link_id,cid,dual_sys))
    {
        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_CONNECT,0);
        SIO_ATC_SetDataMode(TRUE);
        SCI_TRACE_LOW( "ATC:  atd gprs success!"); 
        return   S_ATC_DEFAULT_HANDLE;        
    }
    else
    {
        SCI_TRACE_LOW( "ATC:  atd gprs operationg not allowed!");
        return  ERR_OPERATION_NOT_ALLOWED;   
    }
}
#endif
#endif
/*****************************************************************************/
//  Description: This function handle the ATD command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessD )
{
    MN_CALL_CLIR_E       clir = MN_CALL_CLIR_SUPPRESSION; // the default value
    MN_CALLED_NUMBER_T   called_num;             
    BOOLEAN                                           status = FALSE;
    uint8                                               bcd_len = 0;
    MN_CALL_TYPE_E                            call_type;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    uint32                                             num_len = 0;
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS           ss_status;
    BOOLEAN              is_clir_call = FALSE;
#endif
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&called_num, 0, sizeof(called_num));
#ifdef _MUX_ENABLE_
    g_mo_call_link_id = atc_config_ptr->current_link_id;
#endif


    if (!PARAM1_FLAG)
    {
        return ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
    }

    if((PARA1.str_ptr->str_ptr[0] == 'i' || PARA1.str_ptr->str_ptr[0] == 'I')
            || (PARA1.str_ptr->str_ptr[0] == 'p' || PARA1.str_ptr->str_ptr[0] == 'P')
            || (PARA1.str_ptr->str_ptr[0] == 'w' || PARA1.str_ptr->str_ptr[0] == 'W')
            || (PARA1.str_ptr->str_ptr[0] == 't' || PARA1.str_ptr->str_ptr[0] == 'T')
            || (PARA1.str_ptr->str_ptr[0] == '!')
            || (PARA1.str_ptr->str_ptr[0] == '@'))
    {
        PARA1.str_ptr->str_ptr = PARA1.str_ptr->str_ptr + 1;
        PARA1.str_ptr->str_len--;
    }
    
    if (!memcmp(PARA1.str_ptr->str_ptr,"*99",3)
        || !memcmp(PARA1.str_ptr->str_ptr,"*98",3) )
    { 
    #ifdef _MUX_ENABLE_
        uint32 i = 0;
        BOOLEAN is_rra_traffic = FALSE;
                   
        for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
        {
            if(RRA_CheckTrafficState(i))
            {
                is_rra_traffic = TRUE;
                break;
            }
        }
        
        if(is_rra_traffic && RRA_IsAnswerToPaging())
        {
            //SCI_TRACE_LOW:"ATC: rra is traffic when atd*99#"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_306_112_2_17_22_17_0_512,(uint8*)"");
            return ERR_GPRS_OPERATION_FAILURE;
        }
    #endif  
    #ifdef PLATFORM_UWS6121E
	#ifdef PPP_USB 
            return (ATC_ProcessPPPDial(atc_config_ptr, PARA1.str_ptr->str_ptr,PARA1.str_ptr->str_len,ATC_GetSioPort()));
	#endif
	#endif
    }
    
    SCI_MEMSET(g_tel_str, 0, sizeof(g_tel_str));

    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
         if (PARAM1_FLAG)
         {
             num_len = PARA1.str_ptr->str_len;
             //Check the length of input number
             if(2*MN_MAX_ADDR_BCD_LEN < num_len)
             {
                 return ERR_INVALID_INDEX;
             }
             SCI_MEMCPY(g_tel_str, PARA1.str_ptr->str_ptr, num_len);

#ifndef _ULTRA_LOW_CODE_
             if(atc_config_ptr->sim_flag[dual_sys])
             {           
                 is_clir_call = HandleClirCall(g_tel_str, &num_len, &clir);
                 
                 if(!is_clir_call)
                 {
                     ss_status = ATC_CheckSSStr(atc_config_ptr, g_tel_str, num_len);
                     if ( S_ATC_DEFAULT_HANDLE != ss_status )
                     {
                         //it is a ss or ussd request
                         if (S_ATC_SUCCESS == ss_status)
                         {
                         #ifdef _MUX_REDUCED_MODE_
                             sprintf((char*)g_rsp_str, "OK");
                             ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                         #endif
                             return S_ATC_DEFAULT_HANDLE;
                         }
                         else
                         {
                             return ss_status;
                         }
                     }
                 }
             }
#endif /* _ULTRA_LOW_CODE_ */

             g_tel_str[num_len] = '\0';
        
         #if defined(_MUX_REDUCED_MODE_)||defined(_ATC_UIX8910_ENABLE_)   //23.101  should be emergency call when no valid ec number in sim.
             if (!strcmp((char*)g_tel_str, "112") || !strcmp((char*)g_tel_str, "911") 
                ||((FALSE == atc_config_ptr->sim_flag[dual_sys]) &&(!strcmp((char*)g_tel_str, "000")||!strcmp((char*)g_tel_str, "08")
                ||!strcmp((char*)g_tel_str, "110")||!strcmp((char*)g_tel_str, "118")||!strcmp((char*)g_tel_str, "119")||!strcmp((char*)g_tel_str, "999"))))
         #else   
             if (!strcmp((char*)g_tel_str, "112"))
         #endif
             {
                 call_type = MN_CALL_TYPE_EMERGENCY;
             }
             else
             {
                 call_type = MN_CALL_TYPE_NORMAL;
             }
             
             //emergency call: treat ATD911,# as a emergency call for PTCRB case
             if(num_len>2)
             {
                 if (g_tel_str[num_len-1] == '#' && g_tel_str[num_len-2] ==',')
                 {
                     //SCI_TRACE_LOW:"ATC: this is a emergency call!"
                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_377_112_2_17_22_17_0_513,(uint8*)"");
                     call_type = MN_CALL_TYPE_EMERGENCY;
                     g_tel_str[num_len-2] = '\0';
                     num_len -= 2;
                 }
             }
             
             status = TransfromStrToBcd(called_num.party_num, 
                                        &bcd_len,
                                        g_tel_str, num_len);
             if (!status)
            {
                 return ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
            }

         }
         else 
         {
             return ERR_INVALID_INDEX;
         }
         //check the network status
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
        if (PLMN_NO_SERVICE == plmn_info.plmn_status)
        {
            atc_config_ptr->ceer_val = MN_CAUSE_NO_NETWORK_SERVICE;//NO NET SERVICE
            return ERR_NO_NETWORK_SERVICE;
        }
        
        if(call_type != MN_CALL_TYPE_EMERGENCY)
        {
            if(!atc_config_ptr->sim_flag[dual_sys])
            {
                return ERR_SIM_NOT_INSERTED;
            }
            else
            {
                if(atc_config_ptr->cpin[dual_sys] != ATC_CPIN_CODE_READY)
                {
                    if(atc_config_ptr->cpin[dual_sys] == ATC_CPIN_CODE_PIN1)
                    {
                        return ERR_SIM_PIN_REQUIRED;
                    }
                    else if(atc_config_ptr->cpin[dual_sys] == ATC_CPIN_CODE_PUK1)
                    {
                        return ERR_SIM_PUK_REQUIRED;
                    }
                    else
                    {
                        return ERR_SIM_FAILURE;
                    }
                }
            }
        }
        
#ifdef _MUX_ENABLE_
        {
            uint32                        i = 0;
            BOOLEAN is_rra_traffic = FALSE;
            BOOLEAN   is_call_exist = FALSE;
                       
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if(RRA_CheckTrafficState(i))
                {
                    is_rra_traffic = TRUE;
                    break;
                }
            }
            
            for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
            {
                if(g_calls_state[i].call_amount > 0)
                {
                    is_call_exist = TRUE;
                    break;
                }
            }
            
            if(is_rra_traffic && (!is_call_exist) && RRA_IsAnswerToPaging())
            {
                //SCI_TRACE_LOW:"ATC: rra is traffic when gsm atd"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_457_112_2_17_22_17_1_514,(uint8*)"");
                return ERR_OPERATION_NOT_ALLOWED;
            }
        }
#endif      
        called_num.num_len = bcd_len;
    #ifdef _MUX_ENABLE_
        if(g_atc_international_call)
        {
            called_num.number_type = MN_NUM_TYPE_INTERNATIONAL;
        }
        else
    #endif
        {
            called_num.number_type = MN_NUM_TYPE_UNKNOW;
        }
        called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
        // start a call
        if (MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys, call_type, &called_num, PNULL, clir, PNULL))
        {
            g_atc_call_flag = TRUE;
            g_atc_call_need_disc = FALSE;    //*CR54932 qing.xi, reset the neee_disc flag
            atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS; 
        #if !(defined( _MUX_REDUCED_MODE_   )||defined(_ATC_UIX8910_ENABLE_))
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_CALL_START_IND,0xff,ATC_DOMAIN_CS);
        #else
            sprintf((char*)g_rsp_str, "OK");
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        #endif  
            return S_ATC_DEFAULT_HANDLE;
        }
        else
        {
        #if defined( _MUX_REDUCED_MODE_   )||defined(_ATC_UIX8910_ENABLE_)
            sprintf((char*)g_rsp_str, "OK");
                       ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            #endif
            return ERR_INVALID_INDEX;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }    
}

/*****************************************************************************/
//  Description: This function handle the APP_MN_CALL_START_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallStartInd(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_CALL_START_IND_T  *sig_ptr    // point to the APP_MN_CALL_START_IND signal
                )
{
    ATC_TELE_TYPE_E  tele_type;
    uint8            *party_number_ptr;
    MN_DUAL_SYS_E    dual_sys = sig_ptr->dual_sys;
    
    if(g_calls_state[dual_sys].call_amount >= MN_CALL_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ATC: Assert Failure call_amount error %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_519_112_2_17_22_17_1_515,(uint8*)"d",g_calls_state[dual_sys].call_amount);
        return S_ATC_DEFAULT_HANDLE;
    }
    
    g_calls_state[dual_sys].call_amount++;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].direction = FALSE;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].call_id = sig_ptr->call_id;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].call_entity_state = ATC_CALL_OUTGOING_STATE;
    /*
    If ATH command was received before the APP_MN_CALL_START_IND,just disconnect 
    the current MO call.  
    */
    if(g_atc_call_need_disc && g_atc_call_flag)
    {
        MNCALL_DisconnectCallEx(dual_sys, (uint8)sig_ptr->call_id, MN_CAUSE_NORMAL_CLEARING, PNULL);

        g_atc_call_need_disc = FALSE;
        
        return S_ATC_DEFAULT_HANDLE;
    }
    
    g_atc_call_need_disc = FALSE;
    
    party_number_ptr = g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].party_number;
    
    ATC_ConvertCalledNumToStr(&(sig_ptr->called_num), &tele_type, party_number_ptr);
    
    //Send the unsolicited +SIND: 5
    ATC_UpSINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, ATC_WIND_BIT4, 5);

    return S_ATC_DEFAULT_HANDLE;
}

#if (!defined(_ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)) && defined(_SUPPORT_GPRS_)
/*****************************************************************************/
//    Description : This function decode parameter for gprs dial command 
//    Global resource dependence : none
//    Author:       Elvis.xu
//    Note:
//        *<99>*<call address>*<L2P>*<pid>
//        *<98>*<pid>
/*****************************************************************************/
LOCAL BOOLEAN DecGprsDialParam( 
                                               uint8  *dial_string_ptr ,
                                               uint16 *param_len,
                                               uint8 *param_ptr[]
                                               )
{
    BOOLEAN is_end = FALSE;
    uint8*   first_ptr = PNULL;
    uint8*    last_ptr = PNULL;
    uint8         index = 0;

    first_ptr = dial_string_ptr + 3;
    last_ptr = first_ptr;
    
    while(*first_ptr != '\0')
    {
        if(*first_ptr == '#')
        {
            if(last_ptr != first_ptr)
            {                
                param_len[index] = first_ptr - last_ptr - 1;
                param_ptr[index] = last_ptr+1;
            }
            first_ptr ++;
            
            //*99# or * 98#
            if( (*last_ptr== '#' || index <= 2 && *last_ptr == '*' ) && *first_ptr=='\0')
            {
               is_end = TRUE;
            }
            break;
        }
        
        if(*first_ptr == '*')
        {
            if(last_ptr != first_ptr)
            {                
                param_len[index] = first_ptr - last_ptr - 1;
                param_ptr[index] = last_ptr+1;
                index++;
            }
            
            last_ptr = first_ptr;
        }
        first_ptr++;
    
    }

    return is_end;
}


/*****************************************************************************/
//  Description : This function handle the GPRS ATD*99# or ATD*98#command 
//  Global resource dependence : none
//  Author:       Hyman
//  Note:
//        *<99>*<call address>*<L2P>*<pid>
//        *<98>*<pid>
/*****************************************************************************/
LOCAL ATC_STATUS ProcessGprsDial( // return the handle result
                ATC_CONFIG_T   *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8  *dial_string_ptr ,// point to the dial string
                GPRS_DIAL_TYPE_E    gprs_dial_type//gprs dial type
                )
{
    uint32     pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    uint16          param_len[3] = {0};
    uint8          *param_ptr[3] = {PNULL, PNULL, PNULL};
    uint8     param_pid_index = 0;
    BOOLEAN                is_pid = FALSE;
    uint8                   *pid_ptr = PNULL;
    uint32                         pid = 0;
    MN_DUAL_SYS_E  dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != dial_string_ptr); /*assert verified*/

    //decode the total string as format:"*<99>*<call address>*<L2P>*<pid>"
    //or "*<98>*<pid>"
    if(!DecGprsDialParam(dial_string_ptr, param_len, param_ptr))
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if (GPRS_DIAL_99 == gprs_dial_type)
    {
        if(param_len[1])
        {
            if( !memcmp(param_ptr[1], "PPP", 3))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }

        if(param_len[2])
        {
            is_pid = TRUE;
            param_pid_index = 2;
        }
    }
    else if (GPRS_DIAL_98 == gprs_dial_type)
    {
        if (PNULL != param_ptr[1] || PNULL != param_ptr[2])
        {
            return ERR_OPERATION_NOT_SUPPORTED;
        }
                
        if(param_len[0])
        {
            is_pid = TRUE;
            param_pid_index = 0;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    
    if(is_pid)
    {
        pid_ptr = (uint8 *)SCI_ALLOC_BASE(param_len[param_pid_index]+1);

        SCI_MEMCPY((void *)pid_ptr,(void *)param_ptr[param_pid_index], param_len[param_pid_index]);
        pid_ptr[param_len[param_pid_index]] = 0;
        pid = atoi((char*)pid_ptr);
        SCI_FREE(pid_ptr);
    }
    else
    {
        /*hyman
         * because wap/mms will use the pid= 1,so when after using wap,modem by at will active pdp
         * with wap apn.
         * so we default set the modem pid = 2,and change the nv setting too.
         */
        pid = 2; 
    
    }
    
    //SCI_TRACE_LOW:"ATC: ATD*99 or 98#  RequestPdpId pdp_id:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_704_112_2_17_22_17_1_516,(uint8*)"d", pid);
    
    if(pid == 0 || pid > MN_GPRS_MAX_PDP_CONTEXT_COUNT)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    atc_config_ptr->pdp_id_arr[pid-1] = TRUE;
    pdp_id_arr[0] = pid;
    pdp_id_arr[1] = 0;

    if (ERR_MNGPRS_NO_ERR != MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED))
    {
        return ERR_GPRS_OPERATION_FAILURE;
    }
    
#ifdef _MUX_ENABLE_
    ppp_dialing_in_progress = TRUE;
    s_pdp_id_modem = pid;
    g_atc_modem_dial_sys = dual_sys;
    //ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ACTIVATE_PDP_CONTEXT_CNF,0xff,ATC_DOMAIN_PS);
#else
       g_atc_modem_dial_sys = g_current_card_id;
#endif
    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, APP_MN_ACTIVATE_PDP_CONTEXT_CNF, pdp_id_arr[0], ATC_DOMAIN_PS);
    return S_ATC_DEFAULT_HANDLE;
}  

/*****************************************************************************/
//  Description : This function checks the plmn string.
//  Global resource dependence : none
//  Author:      sunny
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckPlmnStr(
    uint8          length,
    uint8          *str
)
{
    int32              i = 0;

    if(5!=length && 6!=length)
    {
        return FALSE;
    }

    for(i = 0; i < length; i++)
    {
        if(str[i] < '0' || str[i] > '9')
        {
            //SCI_TRACE_LOW:"ATC:invalid plmn num %c"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2013_112_2_17_22_17_4_543,(uint8*)"c", str[i]);
            return FALSE;
        }
    }

    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : This function handle the GPRS ATD command 
//  Global resource dependence : none
//  Author:       Ivan.Yin
//  Note: Modified by Hyman
//      the dial string is:
//        *<99>*<call address>*<L2P>*<pid>
//        *<98>*<pid>
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessGprsDial( // return the handle result
                ATC_CONFIG_T   * atc_config_ptr, // point to the ATC_CONFIG_T struct.
                uint8  *dial_string_ptr // point to the dial string
                )
{
#if (!defined(_ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)) && defined(_SUPPORT_GPRS_)
    GPRS_DIAL_TYPE_E    gprs_dial_type = GPRS_DIAL_MAX;

    if (!memcmp(dial_string_ptr,"*99",3))
    {
        gprs_dial_type = GPRS_DIAL_99;
    }
    else if (!memcmp(dial_string_ptr,"*98",3))
    {
        gprs_dial_type = GPRS_DIAL_98;
    }
    else
    {
        return ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
    }

    return(ProcessGprsDial(atc_config_ptr,dial_string_ptr, gprs_dial_type));

#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description: This function handle the AT+VTS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVTS )
{
#if !defined(_ULTRA_LOW_CODE_) ||defined(_ATC_UIX8910_ENABLE_)
 //reduce atc code Nemge
    uint8          dtmf_buf[AT_DTMF_BUF_LEN] = {0};
    uint8          cmd_type;
    uint8          *temp_ptr;
    uint8          param_len;
    uint8          call_id = 0;
    ATC_STATUS     status = S_ATC_SUCCESS;
    static BOOLEAN b_start_dtmf = FALSE;
    static uint8   s_dtmf_char;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    if (!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    param_len = PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;

    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);
    switch (cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (0==g_calls_state[dual_sys].call_amount)
            {
                return ERR_NOT_FOUND;
            }
            
            if (!CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
            {
                //SCI_TRACE_LOW:"ATC: no active call found."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_805_112_2_17_22_17_1_517,(uint8*)"");
                return ERR_NOT_FOUND;
            }
            
            //SCI_TRACE_LOW:"ATC: send dtmf id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_809_112_2_17_22_17_1_518,(uint8*)"d", call_id);
            
            //@Shijun.cui CR31537
            //startDTMF: AT+VTS=6,1
            //stopDTMF:  AT+VTS=6,0
            if (param_len == 4)//sample: AT+VTS=6,1
            {
                if (temp_ptr[2] == ',')
                {
                    if (!CheckDtmfStr(1,&temp_ptr[1]))
                    {
                        return ERR_INVALID_INDEX;
                    }
                    
                    if (temp_ptr[3] == '1')
                    {
                        if (!b_start_dtmf)
                        {
                            MNCALL_StartDTMFEx(dual_sys, temp_ptr[1], call_id);
                            b_start_dtmf = TRUE;
                            s_dtmf_char = temp_ptr[1];
                            //SCI_TRACE_LOW:"ATC: StartDTMF = %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_830_112_2_17_22_17_1_519,(uint8*)"d",temp_ptr[1]);
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else if (temp_ptr[3] == '0')
                    {
                        if (b_start_dtmf && (s_dtmf_char == temp_ptr[1]))
                        {
                            MNCALL_StopDTMFEx(dual_sys, call_id);
                            
                            b_start_dtmf = FALSE;
                            //SCI_TRACE_LOW:"ATC: StopDTMF!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_844_112_2_17_22_17_1_520,(uint8*)"");
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                    return status;
                }
            }
            if (b_start_dtmf)
            {
                //SCI_TRACE_LOW:"ATC: start a DTMF, but not stop it, operation refused!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_860_112_2_17_22_17_1_521,(uint8*)"");
                return ERR_OPERATION_NOT_ALLOWED;
            }
            //end CR31537
            
            dtmf_buf[0] = temp_ptr[1];
            temp_ptr  += 3;
            if (param_len >= 3)
            {
                param_len -= 3; 
                if (!ATC_ParseDtmfChar(temp_ptr, param_len, &dtmf_buf[1]))
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }
            }
            if (!CheckDtmfStr(strlen((char*)dtmf_buf),dtmf_buf))
            {
                return ERR_INVALID_INDEX;
            }

            SendDtmfStr(dual_sys,call_id, strlen((char*)dtmf_buf), dtmf_buf);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "+VTS: (0-9,*,#,A,B,C,D)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description: This function handle the AT+COPS command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCOPS )
{
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    //const ATC_PLMN_DETAILED_INFO_T  *plmn_detailed_info = NULL;
    ATC_STATUS                      status = S_ATC_DEFAULT_HANDLE;
    ERR_MNPHONE_CODE_E              err_code = ERR_MNPHONE_NO_ERR;
    MN_PLMN_T                       plmn;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&plmn, 0, sizeof(plmn));

    //If there is calls,return error
    if ((ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE) && (PARAM1_FLAG) && (PARA1.num == SET_ONLY_FORMAT))
    {
        //action should be performed
    }
    else
    {
        if ((g_calls_state[dual_sys].call_amount >= 1) &&
            (ATC_CMD_TYPE_READ != ATC_GET_CMD_TYPE))
        {
            //SCI_TRACE_LOW:"ATC: ATC_ProcessCOPS, not allowed when CALL is existed."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_928_112_2_17_22_17_2_522,(uint8*)"");
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if (atc_config_ptr->sim_flag[dual_sys]
                    && ATC_CPIN_CODE_READY != atc_config_ptr->cpin[dual_sys])
                {
                    //SCI_TRACE_LOW:"ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready."
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_942_112_2_17_22_17_2_523,(uint8*)"");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                switch(PARA1.num)
                {
                    case AUTO_MODE:
                    {
                        // set the auto select mode

                        err_code = MNPHONE_SelectPLMNEx(dual_sys, FALSE, plmn, MN_GMMREG_RAT_GPRS);

                        if (err_code != ERR_MNPHONE_NO_ERR)
                        {
                            //SCI_TRACE_LOW:"ATC: MNPHONE_SelectPLMN failure"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_959_112_2_17_22_17_2_524,(uint8*)"");
                            return ERR_UNKNOWN;
                        }
                        
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PLMN_SELECT_CNF,0xff,ATC_NO_DOMAIN);

                        g_select_mode[dual_sys] = AUTO_MODE;
                        break;
                    }

                    case MANUAL_MODE:
                    {
                        if (!PARAM2_FLAG || !PARAM3_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                        if (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        //manual select PLMN
                        if (PARAM4_FLAG)
                        {
                            status = ManualSelectPlmn(dual_sys, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, PARA4.num);
                        }
                        else
                        {
                            status = ManualSelectPlmn(dual_sys, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, MN_GMMREG_RAT_UNKNOWN);
                        }

                        if (S_ATC_SUCCESS == status)
                        {
                            g_select_mode[dual_sys] = MANUAL_MODE;
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                        
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PLMN_SELECT_CNF,0xff,ATC_NO_DOMAIN);
                        if(g_creg_state[dual_sys].oper_status == REGISTRATION_DENIED)
                        {
                            switch (atc_config_ptr->atc_config_nv.creg)
                            {
                                case ATC_CREG_DISABLE:
                                    break;
                                    
                                case ATC_CREG_ENABLE:
                                case ATC_CREG_AND_INFO:
                                {
                                    sprintf((char*)g_rsp_str, "%s: %d", 
                                        g_atc_info_table[AT_CMD_CREG].cmd_name->str_ptr, NOT_REGISTERED_AND_SEARCHING);
                                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                    break;
                                }
                            
                                default:
                                {
                                    //SCI_TRACE_LOW:"ATC: Assert Failure unknow creg."
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1017_112_2_17_22_17_2_525,(uint8*)"");
                                    break;  
                                }
                            }

                            g_creg_state[dual_sys].oper_status = NOT_REGISTERED_AND_SEARCHING;

                            switch (atc_config_ptr->cgreg)
                            {
                                case ATC_CREG_DISABLE:
                                        break;
                                        
                                case ATC_CREG_ENABLE:
                                case ATC_CREG_AND_INFO:
                                {
                                    sprintf((char*)g_rsp_str, "%s: %d", 
                                            g_atc_info_table[AT_CMD_CGREG].cmd_name->str_ptr, NOT_REGISTERED_AND_SEARCHING);
                                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                                        break;
                                }
                            
                                default:
                                {
                                    //SCI_TRACE_LOW:"ATC: Assert Failure unknow cgreg"
                                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1040_112_2_17_22_17_2_526,(uint8*)"");
                                    break;
                                }
                            }   

                        }
                        break;
                    }

                    case MANUAL_DEREGISTER:
                    {
                        if(ERR_MNPHONE_NO_ERR != MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_NONE))
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        g_select_mode[dual_sys] = MANUAL_DEREGISTER;
                        status = S_ATC_SUCCESS;
                        break;
                    }

                    case SET_ONLY_FORMAT:
                    {
                        if (!PARAM2_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                        }
                        else
                        {
                            if (PARA2.num == PLMN_FORMAT_NUMBERIC)
                            {
                                s_plmn_format[dual_sys] = (ATC_PLMN_FORMAT_E)PARA2.num;
                                status = S_ATC_SUCCESS;
                            }
                            else
                            {
                                status = ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }
                        break;
                    }

                    case MANUAL_MATO_MODE:
                    {
                        if (!PARAM2_FLAG || !PARAM3_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                        if (PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        //manual select PLMN at the first
                        if (PARAM4_FLAG)
                        {
                            status = ManualSelectPlmn(dual_sys, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, PARA4.num);
                        }
                        else
                        {
                            status = ManualSelectPlmn(dual_sys, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, MN_GMMREG_RAT_UNKNOWN);
                        }
                        
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PLMN_SELECT_CNF,0xff,ATC_NO_DOMAIN);

                        if (S_ATC_SUCCESS == status)
                        {
                            g_select_mode[dual_sys] = MANUAL_MATO_MODE;
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                        break;
                    }

                    default:
                    {
                        status = ERR_INVALID_INDEX;
                        break;
                    }
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
                MN_PHONE_CURRENT_PLMN_INFO_T  cur_plmn_info = {0};
                MN_PHONE_SELECT_PLMN_T        select_plmn = {0};
                uint8                         oper_str[6] = {0};
                uint8                         act=0;
                MN_GMMREG_RAT_E current_rat = MN_GMMREG_RAT_UNKNOWN;

                cur_plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                MNPHONE_GetPlmnSelectModeEx(dual_sys, &select_plmn);

                switch(cur_plmn_info.plmn_status)
                {
                    case PLMN_NO_SERVICE:
                    case PLMN_EMERGENCY_ONLY:
                    case PLMN_EMERGENCY_SIM_INVALID:
                    case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
                    case PLMN_REGISTER_SERVICE:
                    case PLMN_REGISTER_GPRS_ONLY:
                    {
                        sprintf((char *)g_rsp_str, "%s: %d",
                                    g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                    g_select_mode[dual_sys]);
                        status = S_ATC_SUCCESS;
                        break;
                    }

                    case PLMN_NORMAL_GSM_ONLY:
                    case PLMN_NORMAL_GPRS_ONLY:
                    case PLMN_NORMAL_GSM_GPRS_BOTH:
                    case PLMN_EMERGENCY_GPRS_ONLY:
                    {
                        plmn.mnc = cur_plmn_info.mnc;
                        plmn.mcc = cur_plmn_info.mcc;
                        current_rat = MNPHONE_GetCurrentRAT(dual_sys);
                        if (MN_GMMREG_RAT_4G == current_rat)
                       {
                            act = 7;
                       }
                            
                        if(PLMN_FORMAT_NUMBERIC == s_plmn_format[dual_sys])
                        {
                            TransformPlmnToNumeric(&plmn, oper_str);

                            sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                    g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                    g_select_mode[dual_sys], s_plmn_format[dual_sys], oper_str, act);
                        }
                        else
                        {
                            TransformPlmnToNumeric(&plmn, oper_str);
                            sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                    g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                    g_select_mode[dual_sys], 2, oper_str, act);
                        }
                        status = S_ATC_SUCCESS;
                        break;
                    }

                    default:
                    {
                        status = ERR_INVALID_INDEX;
                        //SCI_TRACE_LOW:"ATC: Assert Failure plmn_status."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1184_112_2_17_22_17_2_527,(uint8*)"");
                        break;
                    }
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            break;
        }            

        case ATC_CMD_TYPE_TEST:
        {
            //SCI_TRACE_LOW:"ATC: search the plmn list"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1196_112_2_17_22_17_2_528,(uint8*)"");

            err_code = MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS);

            if(ERR_MNPHONE_NO_ERR != err_code)
            {
                //SCI_TRACE_LOW:"ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1206_112_2_17_22_17_2_529,(uint8*)"");
                status = ERR_OPERATION_NOT_ALLOWED;
                return status;
            }
            
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PLMN_LIST_CNF,0xff,ATC_NO_DOMAIN);

            atc_config_ptr->plmn_search_flag = TRUE;
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description: This function handles the AT+SDMUT command.This command is used
//                to mute downlink of phone.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSDMUT )
{
    ATC_STATUS           status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            atc_config_ptr->sdmut_flag = PARA1.num;
            AUDIO_SetDownLinkMute(atc_config_ptr->sdmut_flag);
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SDMUT].cmd_name->str_ptr, atc_config_ptr->sdmut_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return status;
}

/*****************************************************************************/
//  Description: This function handle the AT+SSMP command, This command is used
//                to send the singal with max power. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSSMP )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS  at_result = S_ATC_SUCCESS;
    
    //SCI_TRACE_LOW("ATC: enter ATC_ProcessSSMP");
    if(L1API_ApplyMaxPower())
    {
        at_result = S_ATC_SUCCESS;
    }
    else
    {
        at_result = ERR_OPERATION_NOT_ALLOWED;
    }

    return (at_result);
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+CMUX command 
//  Global resource dependence : none
//  Author:      Wang hui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCMUX )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS           status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                cmux_sys_para.mux_mode = PARA1.num;
                if((cmux_sys_para.mux_mode!=0)&&(cmux_sys_para.mux_mode !=1))          
                {
                    SCI_ASSERT(FALSE);  /*assert verified check FALSE case*/         
                    break;
                }
                         
                  if(PARAM2_FLAG)
                  {
                       cmux_sys_para.subset = PARA2.num;
                  }
                  else
                  {
                       cmux_sys_para.subset = MUX_FRAME_UIH;
                  }
                    if(PARAM3_FLAG)
                  {
                       cmux_sys_para.port_speed = PARA3.num;
                  }
                  else
                  {
                       cmux_sys_para.port_speed = MUX_BAUDRATE_115200;
                  }
                  if(PARAM4_FLAG)
                  {
                       cmux_sys_para.N1 = PARA4.num;
                  }
                  else
                  {
                       cmux_sys_para.N1 = 31;      
                  }
                  if(PARAM5_FLAG)
                  {
                       cmux_sys_para.T1 = PARA5.num;
                  }
                  else
                  {
                       cmux_sys_para.T1 = 10;      
                  }
                  if(PARAM6_FLAG)
                  {
                       cmux_sys_para.N2 = PARA6.num;
                  }
                  else
                  {
                       cmux_sys_para.N2 = 3;       
                  }
                  if(PARAM7_FLAG)
                  {
                       cmux_sys_para.T2 = PARA7.num;
                  }
                  else
                  {
                       cmux_sys_para.T2 = 30;      
                  }
                  if(PARAM8_FLAG)
                  {
                       cmux_sys_para.T3 = PARA8.num;
                  }
                  else
                  {
                       cmux_sys_para.T3 = 10;      
                  }
                  if(PARAM9_FLAG)
                  {
                       cmux_sys_para.K = PARA9.num;
                  }
                  else
                  {
                       cmux_sys_para.K = 2;    
                  }
                  is_cmux_recv=TRUE;
                  atc_config_ptr->atc_config_nv.mux_mode = 1;
                  atc_urc_link_id =1;
                
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%lu,%d,%d,%d,%d,%d",g_atc_info_table[AT_CMD_CMUX].cmd_name->str_ptr, cmux_sys_para.mux_mode,cmux_sys_para.subset,
                cmux_sys_para.port_speed,cmux_sys_para.N1,cmux_sys_para.T1,cmux_sys_para.N2,cmux_sys_para.T2,cmux_sys_para.T3,cmux_sys_para.K);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
                  
        default:
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
#endif
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+SCMUX command 
//  Global resource dependence : none
//  Author:      Feng.shi
//  Note: param1=0 means close mux, =3 means open mux and link 0 is sim1 AT, 
//        link 1 is sim2 AT, link 2 is ppp link.
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMUX )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS           status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if(PARA1.num==0)
                {
                    MUX_Upon_Close(&g_mux_sys_info[0]);
                    is_cmux_recv=FALSE;
                    break;
                }
                else if(PARA1.num ==3)      
                {   
                    cmux_sys_para.mux_mode = 0;
                }
                else
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                    break;
                }
      
                  if(PARAM2_FLAG)
                  {
                       cmux_sys_para.subset = PARA2.num;
                  }
                  else
                  {
                       cmux_sys_para.subset = MUX_FRAME_UIH;
                  }
                    if(PARAM3_FLAG)
                  {
                       cmux_sys_para.port_speed = PARA3.num;
                  }
                  else
                  {
                       cmux_sys_para.port_speed = MUX_BAUDRATE_115200;
                  }
                  if(PARAM4_FLAG)
                  {
                       cmux_sys_para.N1 = PARA4.num;
                  }
                  else
                  {
                       cmux_sys_para.N1 = 31;      
                  }
                  if(PARAM5_FLAG)
                  {
                       cmux_sys_para.T1 = PARA5.num;
                  }
                  else
                  {
                       cmux_sys_para.T1 = 10;      
                  }
                  if(PARAM6_FLAG)
                  {
                       cmux_sys_para.N2 = PARA6.num;
                  }
                  else
                  {
                       cmux_sys_para.N2 = 3;       
                  }
                  if(PARAM7_FLAG)
                  {
                       cmux_sys_para.T2 = PARA7.num;
                  }
                  else
                  {
                       cmux_sys_para.T2 = 30;      
                  }
                  if(PARAM8_FLAG)
                  {
                       cmux_sys_para.T3 = PARA8.num;
                  }
                  else
                  {
                       cmux_sys_para.T3 = 10;      
                  }
                  if(PARAM9_FLAG)
                  {
                       cmux_sys_para.K = PARA9.num;
                  }
                  else
                  {
                       cmux_sys_para.K = 2;    
                  }
                  is_cmux_recv=TRUE;
                  atc_config_ptr->atc_config_nv.mux_mode = 1;
                  atc_urc_link_id =1;
                
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }
            
        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%lu,%d,%d,%d,%d,%d",g_atc_info_table[AT_CMD_CMUX].cmd_name->str_ptr, cmux_sys_para.mux_mode,cmux_sys_para.subset,
                cmux_sys_para.port_speed,cmux_sys_para.N1,cmux_sys_para.T1,cmux_sys_para.N2,cmux_sys_para.T2,cmux_sys_para.T3,cmux_sys_para.K);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
                  
        default:
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
        }
    }
#endif
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description: This function handles the APP_MN_PLMN_LIST_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPlmnListCnf(
                            ATC_CONFIG_T                   *atc_config_ptr,
                            APP_MN_PLMN_LIST_CNF_T         *sig_ptr
                            )
{
    ATC_STATUS                status = S_ATC_SUCCESS;
    uint8                   oper_str[6] = {0};
    int32                                   i = 0;
    uint8                     *temp_ptr = g_rsp_str;
    uint32                           state = 0;
    uint32                              rat = 0;

    //SCI_TRACE_LOW:"ATC: received the plmn list confirm "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1552_112_2_17_22_17_3_530,(uint8*)"");
    //SCI_TRACE_LOW:"ATC: %d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1553_112_2_17_22_17_3_531,(uint8*)"dd", sig_ptr->is_valid_result, sig_ptr->plmn_detail_list.plmn_num);

    if (atc_config_ptr->plmn_search_flag)
    {
        if(sig_ptr->is_valid_result &&
                (0 < sig_ptr->plmn_detail_list.plmn_num))
        {
            sprintf((char *)g_rsp_str, "%s: ",
                    g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr);
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            for(i = 0; i < sig_ptr->plmn_detail_list.plmn_num; i++)
            {
                if(sig_ptr->plmn_detail_list.plmn_details[i].plmn_is_registered)
                {
                    state = ATC_COPS_CURRENT_STATE;
                }
                else if(sig_ptr->plmn_detail_list.plmn_details[i].plmn_is_forbidden)
                {
                    state = ATC_COPS_FORBIDDEN_STATE;
                }
                else
                {
                    state = ATC_COPS_AVAILABLE_STATE;
                }

                switch(sig_ptr->plmn_detail_list.rat[i])
                {
                    case MN_GMMREG_RAT_GPRS:
                    {
                        rat = 0;
                        break;
                    }
                    case MN_GMMREG_RAT_3G:
                    {
                        rat = 2;
                        break;
                    }
                    case MN_GMMREG_RAT_4G:
                    {
                        rat = 7;
                        break;
                    }
                    default:
                    {
                        status = ERR_INVALID_INDEX;
                        //SCI_TRACE_LOW:"ACT: RAT value is wrong"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1593_112_2_17_22_17_3_532,(uint8*)"");
                        break;
                    }
                }

                TransformPlmnToNumeric(&sig_ptr->plmn_detail_list.plmn_details[i].plmn, oper_str);
                sprintf((char *)temp_ptr, "(%ld,\"\",\"\",\"%s\",%lu),", state, oper_str, rat);

                temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            }

            sprintf((char *)temp_ptr, ",(0-4),(0-7,15)");
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            g_rsp_str[strlen((char *)g_rsp_str)] = '\0';

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            // response a "OK"
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
        /*list len is 0, return +COPS: (),,(0-4),(0-2)*/
        else if(sig_ptr->is_valid_result &&
                (0 == sig_ptr->plmn_detail_list.plmn_num))
        {
            sprintf((char *)g_rsp_str, "%s: ",
                    g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr);
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            sprintf((char *)temp_ptr, "(),,(0-4),(0-7,15)");
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            g_rsp_str[strlen((char *)g_rsp_str)] = '\0';

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            /* response a "OK" */
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
        // The low layer is busying
        else if(!sig_ptr->is_valid_result &&
                (0 < sig_ptr->plmn_detail_list.plmn_num))
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,
                                   S_ATC_RESULT_CODE_ERROR, ERR_PHONE_FAILURE);
        }
        // there is no plmn list
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,
                                   S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
        }

        atc_config_ptr->plmn_search_flag = FALSE;
    }   
    return status;
}

/*****************************************************************************/
//  Description: This function handles the APP_MN_PLMN_SELECT_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPlmnSelCnf(
                               ATC_CONFIG_T                   *atc_config_ptr,
                               APP_MN_PLMN_SELECT_CNF_T       *sig_ptr)
{
    BOOLEAN    is_plmn_success = FALSE;
    MN_PLMN_T    plmn;
    MN_DUAL_SYS_E   dual_sys;
    
    dual_sys = sig_ptr->dual_sys;
    
    SCI_MEMSET(&plmn, 0, sizeof(plmn));
    // The current PLMN is select  
    if (sig_ptr->plmn_is_selected )
    {
        switch(sig_ptr->plmn_status)
        {
            // no service
            case PLMN_NO_SERVICE:  
            {
                if(MANUAL_MATO_MODE != g_select_mode[dual_sys])
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,
                        S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
                }
                break;
            }

                // voice service available
            case PLMN_NORMAL_GSM_ONLY:
                // data service availabe 
            case PLMN_NORMAL_GPRS_ONLY:
                // voice and data service available
            case PLMN_NORMAL_GSM_GPRS_BOTH:
            {
                is_plmn_success = TRUE;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK,0);
                break;
            }

                 // emergency service avaiable
            case PLMN_EMERGENCY_ONLY:
            case PLMN_EMERGENCY_SIM_INVALID:
            case PLMN_EMERGENCY_GPRS_ONLY:
            case PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY:
            {
                if (MANUAL_MATO_MODE != g_select_mode[dual_sys])
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR,
                        ERR_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY);
                }
                break;
            }

            default:
            {
                //SCI_TRACE_LOW:"ATC: Assert Failure unknow plmn_status."
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1710_112_2_17_22_17_3_533,(uint8*)"");
                break;
            }
        }
    }
    else
    {
        if (MANUAL_MATO_MODE != g_select_mode[dual_sys])
        {
            if (MN_MM_PLMN_NOT_ALLOWED == sig_ptr->cause)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PLMN_NOT_ALLOWED);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
            }
        }
    }

    //if it dont succeed in MANUAL_MATO_MODE,select the PLMN automatically.
    if ((FALSE == is_plmn_success) && (MANUAL_MATO_MODE == g_select_mode[dual_sys]))
    {
        ERR_MNPHONE_CODE_E err_code;

        err_code = MNPHONE_SelectPLMNEx(dual_sys, FALSE, plmn, MN_GMMREG_RAT_GPRS);

        if (ERR_MNPHONE_NO_ERR == err_code)
        {
            g_select_mode[dual_sys] = AUTO_MODE;
            return S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
            return S_ATC_RESULT_CODE_ERROR;
        }
    }

    return S_ATC_SUCCESS;
}
#if 0
/*****************************************************************************/
//  Description : This function query plmn detailed info
//  Global resource dependence :
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
const ATC_PLMN_DETAILED_INFO_T *ATC_GetPlmnDetailedInfo(
    ATC_PLMN_TYPE_E QueryType,
    unsigned char  *QueryData)
{
    int                            i = 0, length = 0;
    ATC_PLMN_DETAILED_INFO_T       query = { 0 };
    const ATC_PLMN_DETAILED_INFO_T stopper = {{0xFF, 0xFF, 0xFF}, "", "", "", 255, 255};

    /* Invalid parameters */
    if((!QueryData) || (QueryType >= PLMN_TYPE_MAX))
    {
        //SCI_TRACE_LOW:"ATC: ATC_GetPlmnDetailedInfo, Invalid parameters"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1896_112_2_17_22_17_4_540,(uint8*)"");
        return NULL;
    }

    switch(QueryType)
    {
        case PLMN_TYPE_FULL_NAME:
            /* Full name is too long */
            length = strlen((char *)QueryData);

            if(length > ATC_MAX_FULL_OP_NAME)
            {
                //SCI_TRACE_LOW:"ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1908_112_2_17_22_17_4_541,(uint8*)"d", length);
                return NULL;
            }

            /* Query */
            memcpy(query.full_operator_name, QueryData, length);

            while(0 != memcmp(s_atc_plmn_detailed_info_list[i].PLMNNumber_data, stopper.PLMNNumber_data, ATC_PLMN_LEN))
            {
                if(0 == memcmp(s_atc_plmn_detailed_info_list[i].full_operator_name, query.full_operator_name, ATC_MAX_FULL_OP_NAME))
                {
                    return &s_atc_plmn_detailed_info_list[i];
                }

                i++;
            }

            break;

        case PLMN_TYPE_SHORT_NAME:
            /* Full name is too long */
            length = strlen((char *)QueryData);

            if(length > ATC_MAX_SHORT_OP_NAME)
            {
                //SCI_TRACE_LOW:"ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_1933_112_2_17_22_17_4_542,(uint8*)"d", length);
                return NULL;
            }

            /* Query */
            memcpy(query.short_operator_name, QueryData, length);

            while(0 != memcmp(s_atc_plmn_detailed_info_list[i].PLMNNumber_data, stopper.PLMNNumber_data, ATC_PLMN_LEN))
            {
                if(0 == memcmp(s_atc_plmn_detailed_info_list[i].short_operator_name, query.short_operator_name, ATC_MAX_SHORT_OP_NAME))
                {
                    return &s_atc_plmn_detailed_info_list[i];
                }

                i++;
            }

            break;

        case PLMN_TYPE_NUMBERIC:
            /* Query */
            memcpy(query.PLMNNumber_data, QueryData, ATC_PLMN_LEN);

            while(0 != memcmp(s_atc_plmn_detailed_info_list[i].PLMNNumber_data, stopper.PLMNNumber_data, ATC_PLMN_LEN))
            {
                if(0 == memcmp(s_atc_plmn_detailed_info_list[i].PLMNNumber_data, query.PLMNNumber_data, ATC_PLMN_LEN))
                {
                    return &s_atc_plmn_detailed_info_list[i];
                }

                i++;
            }

            break;

        case PLMN_TYPE_MNC_MCC:
            /* Query */
            query.mnc_data = ((MN_PLMN_T *)QueryData)->mnc;
            query.mcc_data = ((MN_PLMN_T *)QueryData)->mcc;

            while(0 != memcmp(s_atc_plmn_detailed_info_list[i].PLMNNumber_data, stopper.PLMNNumber_data, ATC_PLMN_LEN))
            {
                if((query.mcc_data == s_atc_plmn_detailed_info_list[i].mcc_data) &&
                (query.mnc_data == s_atc_plmn_detailed_info_list[i].mnc_data))
                {
                    return &s_atc_plmn_detailed_info_list[i];
                }

                i++;
            }

            break;

        default:
            break;
    }

    return NULL;
}
#endif

/*****************************************************************************/
//  Description: This function handle the APP_MN_SETUP_COMPLETE_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSetupCompleteInd(
                ATC_CONFIG_T                 *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_SETUP_COMPLETE_IND_T  *sig_ptr
                ) 
{
     sprintf((char*)g_rsp_str, "CONNECT");
     ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);    
     return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function transform PLMN structure to numeric string 
//  Global resource dependence : none
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
LOCAL void TransformNumericToPlmn(
                MN_PLMN_T     *plmn_ptr,   // out: the plmn
                uint8         *numeric_str // in: the numeric string of the plmn
                )
{
    //SCI_TRACE_LOW:"ATC: plmn %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2365_112_2_17_22_17_5_550,(uint8*)"s", numeric_str);
    
    plmn_ptr->mcc = (*numeric_str -'0') *100 + (*(numeric_str+1) -'0')*10 + (*(numeric_str+2) -'0');

    if(*(numeric_str+5))
    {
        plmn_ptr->mnc = (*(numeric_str+3) -'0') *100 + (*(numeric_str+4) -'0')*10 + (*(numeric_str+5) -'0');
        plmn_ptr->mnc_digit_num = 3;
    }
    else
    {
        plmn_ptr->mnc = (*(numeric_str+3) -'0')*10 + (*(numeric_str+4) -'0');
        plmn_ptr->mnc_digit_num = 2;
    }
}

/*****************************************************************************/
//  Description: This function handles the APP_MN_ACMMAX_IND signal. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessAcmInd(
                  ATC_CONFIG_T          *atc_config_ptr
                  )
{
     MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
     
     if (0==g_calls_state[dual_sys].call_amount)
     {
         ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);           
     }
     
     atc_config_ptr->ceer_val = MN_CAUSE_ACM_EQ_OR_GT_ACMMAX;
     return S_ATC_SUCCESS;
}

//#endif Nemge
 /*****************************************************************************/
//  Description: This function checks the DTMF string. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDtmfStr(
            uint8          length,
            uint8          *str
            )
{
    int32                  i  = 0;
    BOOLEAN    status = TRUE;

    for (i=0; i<length; i++)
    {
        if ((str[i] >= '0' && str[i] <= '9') ||
            (str[i] >= 'A' && str[i] <= 'D') ||
            str[i] == '*' || str[i] == '#')
        {
            //SCI_TRACE_LOW:"ATC: the dtmf value is %c"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2666_112_2_17_22_17_5_557,(uint8*)"c", str[i]);
            // do nothing
        }
        else
        {
            status = FALSE;
        }
    }
    
    return status;
}

/*****************************************************************************/
//  Description: This function is called to send the DTMF string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void SendDtmfStr(
                     MN_DUAL_SYS_E dual_sys,
            uint8          call_id,
            uint8          length,
            uint8          *str
            )
{
    int32           i;
    
    if(MN_CALL_MAX_CALL_NUM <= call_id)
    {
        //SCI_TRACE_LOW:"ATC: Assert Failure error call id %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2693_112_2_17_22_17_6_558,(uint8*)"d",call_id);
        return;
    }
    
    for (i=0; i<length; i++)
    {       
        MNCALL_StartDTMFEx(dual_sys, (int8)*str, call_id);
        SCI_SLEEP(500);//CR318114,2014.06.03 	
        MNCALL_StopDTMFEx(dual_sys, call_id);
        SCI_SLEEP(20); //shijun.cui 2005-03-22 avoid MN queue full CR19444
        str++;
    }
}

//#ifndef _ULTRA_LOW_CODE_  Nemge
/*****************************************************************************/
//  Description: This function checks the CLIR call string 
//  Global resource dependence : none
//  Author:       Jason
//  Return:       TRUE - clir call; FALSE - non clir call
//  Note:         TS 02.30 Annex B
/*****************************************************************************/
LOCAL BOOLEAN HandleClirCall(
            uint8          *str,     // [in,out]: call string pointer
            uint32         *length,  // [in,out]: length of call string
            MN_CALL_CLIR_E *clir     // [out]: clir type
            )
{
    
    if(*length<=4)
    {
        return FALSE;
    }
    
    if (!memcmp(str,"*31#",4))
    {
        *clir = MN_CALL_CLIR_SUPPRESSION;
        *length -= 4;
        memmove(str,str+4,*length);
        
        return TRUE;
    }
    
    if (!memcmp(str,"#31#",4))
    {
        *clir = MN_CALL_CLIR_INVOCATION;
        *length -= 4;
        memmove(str,str+4,*length);
        
        return TRUE;
    }
    
    return FALSE;
}
#endif/*_ULTRA_LOW_CODE_*/
 
/*****************************************************************************/
//  Description: This function handle the ATH command 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessH )
{
    uint8         i;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    if(ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if (SCI_IS_TIMER_ACTIVE(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,
            &T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
        g_atc_expire_ind = FALSE;
    }
    if(g_atc_call_flag)
    {
        g_atc_call_need_disc = TRUE;//*CR54932 qing.xi,assume there is a pending MO CALL
    }
	#ifdef PLATFORM_UWS6121E
	#ifdef PPP_USB
    if(PPP_IsPPPProcessing())
    {
       uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
         pdp_id_arr[0]=PPP_GetCid(); 
         if( MNGPRS_DeactivatePdpContextEx(dual_sys, FALSE,pdp_id_arr) != ERR_MNGPRS_NO_ERR)
        {
            SCI_TRACE_LOW("atc: deactive pdp context fail");
            return  ERR_INVALID_INDEX;
        }
    }
	#endif
	#endif
#if defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    if(g_is_hang_up_data_call)
    {
        uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
        
        g_is_hang_up_data_call = FALSE;
        
        SCI_TRACE_LOW("atc: hang up data call");
        
         pdp_id_arr[0]=1; 
         if( MNGPRS_DeactivatePdpContextEx(dual_sys, FALSE,pdp_id_arr) != ERR_MNGPRS_NO_ERR)
        {
            SCI_TRACE_LOW("atc: deactive pdp context fail");
            return  ERR_INVALID_INDEX;
        }
    }
#endif
    if (g_calls_state[dual_sys].call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;
        //Release all the call
        for (i=0; i<g_calls_state[dual_sys].call_amount; i++)
        {
            if(g_calls_state[dual_sys].call_context[i].call_entity_state == ATC_CALL_INCOMING_STATE)
            {
                cause = MN_CAUSE_USER_BUSY;
            }
            else
            {
                cause = MN_CAUSE_NORMAL_CLEARING;
            }

            MNCALL_DisconnectCallEx(dual_sys, (uint8)g_calls_state[dual_sys].call_context[i].call_id, cause, PNULL);

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_DISCONNECTED_IND,0xff,ATC_DOMAIN_CS);
        }
        return S_ATC_DEFAULT_HANDLE;
    }
    else 
    {   
        if(g_atc_call_flag)
        {
            return S_ATC_DEFAULT_HANDLE;//*CR54932 do nothing if there is a pending MO CALL
        }
        return S_ATC_SUCCESS;
    }
    
}


/*****************************************************************************/
//  Description: This function handle the APP_MN_SETUP_IND signal and return 
//                the unsolicited result code. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSetupInd(
                ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
                APP_MN_SETUP_IND_T  *sig_ptr         // point to the APP_MN_SETUP_IND signal
                )
{
    MN_DUAL_SYS_E   dual_sys= sig_ptr->dual_sys;
    SCI_ASSERT(PNULL != atc_config_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != sig_ptr); /*assert verified*/
    
    if(g_calls_state[dual_sys].call_amount >= MN_CALL_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ATC: Assert Failure call_amount %d."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2100_112_2_17_22_17_4_544,(uint8*)"d",g_calls_state[dual_sys].call_amount);
        return S_ATC_DEFAULT_HANDLE;
    }
            
    g_calls_state[dual_sys].call_amount++;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].direction = TRUE;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].call_id = sig_ptr->call_id;
    g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].call_entity_state = ATC_CALL_INCOMING_STATE;
    
    if (sig_ptr->calling_num_present)
    {
        uint8              *party_number_ptr;
        ATC_TELE_TYPE_E    tele_type; // hold number type
        
        party_number_ptr =  g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].party_number;
        
        ATC_ConvertCallingNumToStr(&(sig_ptr->calling_num), &tele_type, party_number_ptr);
    }
    else
    {
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].party_number[0] = 0;
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].party_number[1] = '\0';
    }
    
    // the call wait service is provide
    if (1 < g_calls_state[dual_sys].call_amount)
    {
        //SCI_TRACE_LOW:"ATC: call amount %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2127_112_2_17_22_17_4_545,(uint8*)"d", g_calls_state[dual_sys].call_amount);

        if (atc_config_ptr->ccwa_flag && sig_ptr->calling_num_present)
        {
            //SCI_TRACE_LOW("ATC: the call number len: %d",sig_ptr->calling_num.num_len);
            SCI_MEMSET(s_tel_str,0, sizeof(s_tel_str));

            ATC_ConvertCallingNumToStr(&(sig_ptr->calling_num), &s_tel_num_type, s_tel_str);

            sprintf((char*)g_rsp_str, "%s: \"%s\",%d,%d", 
                        g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                        &(s_tel_str[1]), g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount-1].party_number[0], ATC_VOICE);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            //Send the unsolicited +SIND: 5
            ATC_UpSINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, ATC_WIND_BIT4, 5);
            
            s_mt_calling_num.calling_num = sig_ptr->calling_num;
            s_mt_calling_num.calling_num_present = TRUE;
        }
        else if (atc_config_ptr->ccwa_flag && !sig_ptr->calling_num_present)
        {
            s_tel_str[0] = '\0';
            
            sprintf((char*)g_rsp_str, "%s: \"\",%d,%d", 
                        g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr, ISDN_TELE_PLAN_UNKNOWN_TYPE, ATC_VOICE);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        return S_ATC_SUCCESS;
    }
    else
    {
        if (0 == atc_config_ptr->crc)
        { 
            sprintf((char*)g_rsp_str, "RING");      
        }
        else 
        {
            sprintf((char*)g_rsp_str, "+CRING: VOICE");      
        }
    }

    // the unsolicited result code "+CRING:" is disable
    atc_config_ptr->cpas = ATC_CPAS_STATUS_RING;

    SCI_MEMSET(&s_mt_calling_num, 0, sizeof(s_mt_calling_num));
    
    // handle the calling number string
    if (sig_ptr->calling_num_present)
    {
        s_mt_calling_num.calling_num = sig_ptr->calling_num;
        s_mt_calling_num.calling_num_present = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"ATC: no call number present!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2186_112_2_17_22_17_4_546,(uint8*)"");
        s_tel_str[0] = '\0';
    }
    
    return S_ATC_SUCCESS;   
}
 
/*****************************************************************************/
//  Description: This function handle the APP_MN_ALERTING_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_ProcessAlertingInd(
               ATC_CONFIG_T            *atc_config_ptr, 
               APP_MN_ALERTING_IND_T   *sig_ptr
               )
 {
     MN_DUAL_SYS_E   dual_sys= sig_ptr->dual_sys;
     
     if (MN_CALL_LOCAL_USER_ALERTING == sig_ptr->alerting_type)
     {
        // Send Alert message to network
        MNCALL_IndicateAlertEx(dual_sys, sig_ptr->call_id, PNULL);
        
        atc_config_ptr->cpas = ATC_CPAS_STATUS_RING;
        
     #ifdef _MUX_ENABLE_    
        if(ppp_dialing_in_progress)//if ppp dialilng exist and no "CONNECT" arise, when MT call arise, force deactive it and send "NO CARRIER" TO AP
        {       
            uint8 temp_link_id=0; 
            uint32 pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
  
            //SCI_TRACE_LOW:"ATC: force detach sim %d pdp %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2217_112_2_17_22_17_5_547,(uint8*)"dd",g_atc_modem_dial_sys, s_pdp_id_modem);
            pdp_id_arr[0] = s_pdp_id_modem;
            MNGPRS_DeactivatePdpContextEx(g_atc_modem_dial_sys,FALSE,pdp_id_arr);
            GPRS_SendForceDeactivatePdpContext(g_atc_modem_dial_sys,pdp_id_arr[0]);
            GPRS_ClearPdpStateAndResource(g_atc_modem_dial_sys,pdp_id_arr[0]);

            ppp_dialing_in_progress = FALSE;
            s_pdp_id_modem = 0;
            temp_link_id = atc_config_ptr->current_link_id;
            atc_config_ptr->current_link_id = g_mo_call_link_id;
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
            atc_config_ptr->current_link_id = temp_link_id;
        }
        
        if(g_atc_sms_flag)
        {
            g_ignore_sms_result = TRUE;
            //SCI_TRACE_LOW:"ATC: send sms fail when cring"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2234_112_2_17_22_17_5_548,(uint8*)"");
            ATC_BuildSmsCodeRsp(atc_config_ptr, ATC_SMS_ERR_512);
        }
     #endif
     
        //SCI_TRACE_LOW:"ATC: call amount %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2239_112_2_17_22_17_5_549,(uint8*)"d", g_calls_state[dual_sys].call_amount);
    #ifdef _MUX_REDUCED_MODE_
        if (g_calls_state[dual_sys].call_amount<=1)
    #endif
        {    
            if (0 == atc_config_ptr->crc)
            {
                sprintf((char*)g_rsp_str, "RING");
            }
            else 
            {
                sprintf((char*)g_rsp_str, "+CRING: VOICE");         
            }
        
            // indicate the unsolicited result code that incoming call
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            // start the timer to periodic show the calling number
            SCI_SET_TIMER(P_ATC, ATC_CLIP_INTERVAL, ATC_CALL_EXP_IND, 
                &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
        #ifdef _MUX_ENABLE_
            g_call_exp_link_id = atc_config_ptr->current_link_id;
        #endif
            g_atc_expire_ind = TRUE;        
        }
        
    #ifdef _MUX_REDUCED_MODE_
        if (atc_config_ptr->atc_config_nv.clip_flag && (g_calls_state[dual_sys].call_amount<=1))
    #else
        if (atc_config_ptr->atc_config_nv.clip_flag)
    #endif
        {
            #if 1
            if (s_mt_calling_num.calling_num_present)
            {
                SCI_ASSERT(MN_MAX_ADDR_BCD_LEN >= s_mt_calling_num.calling_num.num_len); /*assert verified*/
                
                SCI_MEMSET(s_tel_str,0, sizeof(s_tel_str));
                
                ATC_ConvertCallingNumToStr(&(s_mt_calling_num.calling_num), &s_tel_num_type, s_tel_str);
                sprintf((char*)g_rsp_str, "%s: \"%s\",%d",
                            g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, &s_tel_str[1], s_tel_num_type);
            }
            else
            {
                s_tel_str[0] = '\0';
                sprintf((char*)g_rsp_str, "%s: \"%s\",%d",
                            g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, UNKNOWN_PLAN_UNKNOWN_TYPE);

            }
            #endif
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
     }

          //Send the unsolicited "WIND:2"
     if (MN_CALL_REMOTE_USER_ALERTING == sig_ptr->alerting_type)
     {
        ATC_UpSINDInfo(dual_sys, atc_config_ptr, PNULL, ATC_WIND_BIT1, 2);
     }
     
     return S_ATC_SUCCESS;
}
 
/*****************************************************************************/
//  Description: This function handle the APP_MN_CALL_HELD_CNF signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessCallDiscInd(
                    ATC_CONFIG_T            *atc_config_ptr,
                    xSignalHeaderRec        *sig_ptr
                    )
{
    uint8                          call_id = 0;
    int32                                  i = 0;
    uint8                    call_index = 0;
    MN_DUAL_SYS_E      dual_sys = MN_DUAL_SYS_1;

    //SCI_TRACE_LOW:"atc: ATC_ProcessCallDiscInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2397_112_2_17_22_17_5_551,(uint8*)"");

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_CALL_DISCONNECTED_IND:
        {
            dual_sys = ((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->dual_sys;
            call_index = GetIndexOfCallContext(((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->call_id,dual_sys);  
            //SCI_TRACE_LOW:"atc: receive APP_MN_CALL_DISCONNECTED_IND call id =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2405_112_2_17_22_17_5_552,(uint8*)"d", ((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->call_id);

            break;
        }
            
        case APP_MN_CALL_ERR_IND:
        {
            dual_sys = ((APP_MN_CALL_ERR_IND_T*)sig_ptr)->dual_sys;
            call_index = GetIndexOfCallContext(((APP_MN_CALL_ERR_IND_T*)sig_ptr)->call_id,dual_sys);
            //SCI_TRACE_LOW:"atc: receive APP_MN_CALL_ERR_IND call id =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2414_112_2_17_22_17_5_553,(uint8*)"d", ((APP_MN_CALL_ERR_IND_T*)sig_ptr)->call_id);
            break;
        }
            
        default:
        {
            //SCI_TRACE_LOW:"ATC: Assert Failure unknow disc code."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2420_112_2_17_22_17_5_554,(uint8*)"");
            break;
        }
    }
        
    if (ATC_INVALID_CALLID!=call_index)
    {
        for(i=call_index + 1; i<g_calls_state[dual_sys].call_amount; i++)
        {
            g_calls_state[dual_sys].call_context[i-1] = g_calls_state[dual_sys].call_context[i];
        }
        SCI_MEMSET(&g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1], 0, sizeof(ATC_CALL_CONTEXT_T));
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].call_id = ATC_INVALID_CALLID;
        g_calls_state[dual_sys].call_amount--;
    }
	
    if ((0 == g_calls_state[dual_sys].call_amount) && g_atc_call_flag)
    {
        ATC_EnableVoiceCodec(FALSE);
        atc_config_ptr->audio_flag = FALSE;
        g_atc_call_flag = FALSE;
    }

#if 0
#ifndef _ULTRA_LOW_CODE_
    //If call is setup by STK.
    if(TRUE == g_is_stk_call[dual_sys])
    {
        ATC_StkProcessCallSignals(dual_sys,FALSE);
    }
#endif
#endif
    if (APP_MN_CALL_DISCONNECTED_IND == sig_ptr->SignalCode)
    {
        atc_config_ptr->ceer_val = (uint32)((APP_MN_CALL_DISCONNECTED_IND_T*)sig_ptr)->disconnected_cause;
    }
    else
    {
        //modify for DIAG test
        atc_config_ptr->ceer_val = (uint32)((APP_MN_CALL_ERR_IND_T*)sig_ptr)->error_cause + MN_CALL_ERR_EMERGENCY_CALL_ONLY; //avoid the same value with disconnect cause
    }
    
    if(!CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        // no wait call, check & kill call exp timer
        if (SCI_IS_TIMER_ACTIVE(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND))
        {
            SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,
                &T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
            g_atc_expire_ind = FALSE;
        }
    }

    //SCI_TRACE_LOW:"atc: ss state %x,cpas %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2472_112_2_17_22_17_5_555,(uint8*)"dd",atc_config_ptr->ss_operate, atc_config_ptr->cpas);
    switch (atc_config_ptr->ss_operate)
    {
        case ATC_INVALID_OPERATE:
        {
            //if cpas is status ready, it means that mt call doesn't sent "RING" to AP, then "NO CARRIER" should be sent to AP
            if ((!atc_config_ptr->ath_flag) && (atc_config_ptr->cpas != ATC_CPAS_STATUS_READY))
            {   
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
                //Send the unsolicited +SIND: 6
                ATC_UpSINDInfo(dual_sys, atc_config_ptr,(xSignalHeaderRec *)sig_ptr, ATC_WIND_BIT5, 6);

            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                atc_config_ptr->ath_flag = FALSE;
            }
            
            if (0 == g_calls_state[dual_sys].call_amount)
            {
                atc_config_ptr->cpas = ATC_CPAS_STATUS_READY;
            }
            break;
        }

        case ATC_REL_HELD_CALLS_OR_SET_UDUB:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
            break;
        }
            
        case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS:
        {
            // Determine if there is a waiting call
            if (CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
            {
                MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,
                            (uint16)APP_MN_SETUP_COMPLETE_IND,
                            call_id,
                            ATC_DOMAIN_CS);
            }
#ifndef _ULTRA_LOW_CODE_
            else if (CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
            {
                MNCALL_RetrieveCallEx(dual_sys, call_id);
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_RETRIEVED_CNF,call_id,ATC_DOMAIN_CS);
            }
#endif
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
                atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
            }
            break;
        }

        case ATC_REL_SPECIFIC_X_CALL_BASE:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
            atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
            break;
        }

        default:
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,0);
            atc_config_ptr->ss_operate = ATC_INVALID_OPERATE;
            break;
        }
    }
    
    return S_ATC_SUCCESS;
}

 /*****************************************************************************/
//  Description: This function handles the ATC_CALL_EXP_IND signal 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessCallExpInd (
                ATC_CONFIG_T            *atc_config_ptr
                )
{
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    have_ringed_times[dual_sys]++;

    if((have_ringed_times[dual_sys] >= ring_times[dual_sys])&&(ring_times[dual_sys]!=0))
    {
        SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
        g_atc_expire_ind = FALSE;
           
        // ask the MN task to connect the incoming call
        MNCALL_ConnectCallEx(dual_sys, 0, PNULL);

        AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
        AUDIO_SetVolume(atc_config_ptr->speaker_volume);
        ATC_EnableVoiceCodec(TRUE);
        atc_config_ptr->audio_flag = TRUE;
        g_atc_call_flag = TRUE;
        have_ringed_times[dual_sys] = 0;

        return S_ATC_SUCCESS;
    }
    
    if ( !g_atc_expire_ind )
    {
        //SCI_TRACE_LOW:"ATC: unexpected timer expire"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_2607_112_2_17_22_17_5_556,(uint8*)"");
        return S_ATC_SUCCESS;
    }

    // show the prompting message
    if (0 == atc_config_ptr->crc)
    {
        sprintf((char*)g_rsp_str, "RING");
    }
    else 
    {
        sprintf((char*)g_rsp_str, "+CRING: VOICE");         
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#if 1
    if (atc_config_ptr->atc_config_nv.clip_flag )
    {
        if (s_mt_calling_num.calling_num_present)
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\",%d",
                        g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, &s_tel_str[1], s_tel_num_type);
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\",%d",
                        g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, UNKNOWN_PLAN_UNKNOWN_TYPE);
        }
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);    
    }
#endif
    SCI_SET_TIMER(P_ATC, ATC_CLIP_INTERVAL, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
        
#ifdef _MUX_ENABLE_
    g_call_exp_link_id = atc_config_ptr->current_link_id;
#endif

    return S_ATC_SUCCESS;
}

AT_CMD_FUNC( ATC_ProcessSPACTCARD )
{
#ifdef _ATC_UIX8910_ENABLE_
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_MAX;
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if (0 == PARA1.num || 1 == PARA1.num)
                    g_current_card_id =  PARA1.num;
                else
                    status = ERR_OPERATION_NOT_ALLOWED;
            }  
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char *)g_rsp_str,"SPACTCARD: %d",g_current_card_id);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char *)g_rsp_str,"SPACTCARD: (0,1)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC( ATC_ProcessCHUP )
{
#ifdef _ATC_UIX8910_ENABLE_
    uint8         i;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    
    if(ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if (SCI_IS_TIMER_ACTIVE(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,
            &T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
        g_atc_expire_ind = FALSE;
    }
    if(g_atc_call_flag)
    {
        g_atc_call_need_disc = TRUE;//*CR54932 qing.xi,assume there is a pending MO CALL
    }

    if (g_calls_state[dual_sys].call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;
        //Release all the call
        for (i=0; i<g_calls_state[dual_sys].call_amount; i++)
        {
            if(g_calls_state[dual_sys].call_context[i].call_entity_state == ATC_CALL_INCOMING_STATE)
            {
                cause = MN_CAUSE_USER_BUSY;
            }
            else
            {
                cause = MN_CAUSE_NORMAL_CLEARING;
            }

            MNCALL_DisconnectCallEx(dual_sys, (uint8)g_calls_state[dual_sys].call_context[i].call_id, cause, PNULL);

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_CALL_DISCONNECTED_IND,0xff,ATC_DOMAIN_CS);
        }
        return S_ATC_DEFAULT_HANDLE;
    }
    else 
    {   
        if(g_atc_call_flag)
        {
            return S_ATC_DEFAULT_HANDLE;//*CR54932 do nothing if there is a pending MO CALL
        }
        return S_ATC_SUCCESS;
    }

#else
    return S_ATC_FAIL;
#endif
}

AT_CMD_FUNC( ATC_ProcessEVTS )
{
#ifdef _ATC_UIX8910_ENABLE_
    uint8          dtmf_key;
    uint8          call_id = 0;
    ATC_STATUS     status = S_ATC_SUCCESS;
    static BOOLEAN b_start_dtmf = FALSE;
    static uint8   s_dtmf_char;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (0==g_calls_state[dual_sys].call_amount)
            {
                return ERR_NOT_FOUND;
            }
            
            if (!CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
            {
                SCI_TRACE_LOW("ATC: no active call found\n");
                return ERR_NOT_FOUND;
            }

            SCI_TRACE_LOW("ATC: ATC: send dtmf id = %d\n", call_id);
            if (PARAM1_FLAG && PARAM2_FLAG)
            {
                b_start_dtmf = PARA1.num;
                dtmf_key = PARA2.str_ptr->str_ptr[0];
            }
            else
                return ERR_OPERATION_NOT_ALLOWED;

            if (!b_start_dtmf && CheckDtmfStr(1, &dtmf_key))
            {
                MNCALL_StartDTMFEx(dual_sys, dtmf_key, call_id);
                b_start_dtmf = TRUE;
                s_dtmf_char = dtmf_key;
            }
            else if (b_start_dtmf && CheckDtmfStr(1, &dtmf_key))
            {
                if (b_start_dtmf && (s_dtmf_char == dtmf_key))
                {
                    MNCALL_StopDTMFEx(dual_sys, call_id);
                    
                    b_start_dtmf = FALSE;
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_GSM_844_112_2_17_22_17_1_520,(uint8*)"");
                }
            }
            break;
        }

        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "+EVTS: (0-9,*,#,A,B,C,D)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
        {
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }
    }
    return status;
#else
    return S_ATC_FAIL;
#endif
}

void ATC_ProcessCallAudioRestart(ATC_CONFIG_T *atc_config_ptr)
{
    if (ATC_IsVoiceCodecEnable())
    {
        ATC_EnableVoiceCodec(FALSE);
        ATC_EnableVoiceCodec(TRUE);
    }
}

#if 0
/*****************************************************************************/
//  Description: This function handle the AT+VTD command, This command is used
//                to define the tone duration. 
//  AT+VTD=<n>
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessVTD )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS    status = S_ATC_SUCCESS;
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                atc_config_ptr->vtd_value = PARA1.num;
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_VTD].cmd_name->str_ptr,
                        atc_config_ptr->vtd_value);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description: This function handles the AT+CEER. This command is used to 
//  get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCEER)
{
#ifndef _ULTRA_LOW_CODE_
    sprintf((char*)g_rsp_str, "+CEER: Error %ld", atc_config_ptr->ceer_val);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description: This function handle the AT+S32K command, This command is used
//                to set the sleep mode. 
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessS32K )
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS           status = S_ATC_SUCCESS;
    BOOLEAN       sleep_mode = FALSE;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            sleep_mode = PARA1.num;
            //set the sleep mode
            SCI_ATC_SetSleepSwitchFlag(sleep_mode);
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            //read the sleep mode
            sleep_mode = SCI_ATC_GetSleepSwitchFlag();
            sprintf((char*)g_rsp_str, "%s: %d", 
                g_atc_info_table[AT_CMD_S32K].cmd_name->str_ptr,sleep_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }

        default:
            break;
    }
    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
#endif


#ifdef FOTA_SUPPORT_ADUPS
#ifdef WIN32
AT_CMD_FUNC( ATC_ProcessSPGPSFIX ){}
AT_CMD_FUNC( ATC_ProcessSPGPSOUT ){}
AT_CMD_FUNC( ATC_ProcessSPGPSINF ){}
AT_CMD_FUNC( ATC_ProcessSPGPSSTATUS ){}
AT_CMD_FUNC( ATC_ProcessSPGPSRST ){}
AT_CMD_FUNC( ATC_ProcessSPGPSPWR ){}
#endif
#endif

