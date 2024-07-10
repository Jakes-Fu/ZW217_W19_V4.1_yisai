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
#include "mux_all.h"

#include "atc_gsm.h"
#include <stdio.h>
#include "atc_common.h"
#include "atc_gsm_ss.h"
#include "atc_basic_cmd.h"
#include "atc_stk.h"

#ifndef ATC_SAT_ENABLE
#include "layer1_engineering.h"
#include "atc_eng.h"
#include "atc_phonebook.h"
#endif
#include "record.h"
#include "sig_code.h"
#include "env_atc_signal.h"
#include "sci_api.h"
#include "tcpip_types.h"
#include "atc_tcp_if.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
extern MN_DUAL_SYS_E         g_current_card_id;

#define                AT_VTS_MAX_PARAMS_NUM       2
#define                AT_EVTS_MAX_PARAMS_NUM      2
#define                MAX_DTMF_STRING_LEN         29
extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T      g_atc_info_table[];

#ifndef ATC_SAT_ENABLE
extern BOOLEAN               g_is_stk_call[MN_SYS_NUMBER];
extern BOOLEAN               g_is_ps_act[MN_SYS_NUMBER];
static ATC_CALLING_NUM_T     s_mt_calling_num;
#else
extern uint8                 g_atc_scan_cell_flag;
#endif

extern MN_PHONE_PLMN_STATUS_E s_plmn_status[MN_SYS_NUMBER];
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
extern BOOLEAN               g_atc_pcm_flag;
static uint8                 s_tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1];
static uint8                 s_plmn_list_type = USER_CONTROLLED_PLMN;

static ATC_TELE_TYPE_E       s_tel_num_type;

BOOLEAN               g_atc_call_need_disc = FALSE;
#ifdef _MODEM_MODE 
static BOOLEAN s_Is_SleepAllowed = FALSE;
#endif
extern ATC_EMG_NUM_LIST_IND_T     *g_emg_num_list_ptr[MN_SYS_NUMBER];// save the emergency number list by network provided

BOOLEAN                      g_atc_call_flag = FALSE;   //to distinguish ATC call from MMI call
BOOLEAN                      g_atc_expire_ind = FALSE;
uint8                       g_atc_expire_call_id = 0xff;
extern uint8                 s_mt_need_open_codec_flag;

#ifdef _MUX_ENABLE_
extern TCPIP_PACKET_TYPE_E  g_packet_type[MN_GPRS_MAX_PDP_CONTEXT_COUNT+1];
extern ATC_MUX_MOD_E g_atc_mux_mode;
extern uint8                   atc_urc_link_id;

BOOLEAN is_cmux_recv = FALSE;
MUX_SYS_PARA_T cmux_sys_para =
{
    MUX_OPTION_BASIC,
    MUX_FRAME_UIH,
    MUX_BAUDRATE_115200,
    31, 10, 3, 30, 10, 2,
    {0}
};
#endif

uint8                        g_tel_str[2*MN_MAX_ADDR_BCD_LEN + 1];

ATC_LAST_CALLED_NUM_T g_last_mo_call = {0};
static ATC_TELE_TYPE_E atset_number_type = ISDN_TELE_PLAN_UNKNOWN_TYPE;
SCI_TMID                     T_ATC_CALL_EXP_IND = PNULL;
static uint8             s_atc_call_ring_num = 0;

static   uint16                     s_dtmf_send_at = 0;
static SCI_TIMER_PTR                s_dtmf_timer = PNULL;
static xSignalHeaderRec           *s_dtmf_sig_ptr = PNULL;
/*the followed two are used for AT+EVTS*/
#ifndef ATC_SAT_ENABLE
static   uint8                          s_dtmf_char = '?';
static   uint8                      s_is_start_dtmf = FALSE;
#endif

ATC_PLMN_INFO_T g_cur_plmn_info[MN_SYS_NUMBER] = {0};
static   BOOLEAN     s_is_self_discon[MN_CALL_MAX_CALL_NUM] = {FALSE};
static   uint32      s_atc_call_disc_cause = 0xffffffff;
static ATC_DTMF_DURATION_T s_atc_dtmf_duration_t[MN_SYS_NUMBER]={0};
uint32    s_pid_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};

#ifndef HUAWEI_6500_MODEM
#ifdef _MUX_ENABLE_
extern MN_DUAL_SYS_E   g_atc_modem_dial_sys;
#endif
#endif

#ifdef HUAWEI_6500_MODEM
uint8  g_undefined_cid = 0xff;
#endif
	
typedef enum
{
    GPRS_DIAL_99,
    GPRS_DIAL_98,
    GPRS_DIAL_MAX
} GPRS_DIAL_TYPE_E;

extern const ATC_PLMN_DETAILED_INFO_T s_atc_plmn_detailed_info_list[]
;

#ifndef ATC_SAT_ENABLE
extern ATC_UPLMN_INF_T    g_atc_uplmn_info[];

extern void SCI_ATC_SetSleepSwitchFlag(
    BOOLEAN b_enter_sleep   // The value used to set the flag.
);
extern BOOLEAN SCI_ATC_GetSleepSwitchFlag(void);
extern void  POWER_Reset(void);
#endif

/*****************************************************************************/
//  Description : This function transform PLMN structure to numeric string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL void TransformPlmnToNumeric(
    MN_PLMN_T     *plmn_ptr,   // IN: the plmn
    uint8         *numeric_str // OUT: the numeric string of the plmn
);

/*****************************************************************************/
//  Description : This function handle the AT+COPS set command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ManualSelectPlmn(
    MN_DUAL_SYS_E      dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    ATC_PLMN_FORMAT_E         oper_format,
    ATC_CUR_PARAM_STRING_T   *oper_ptr,
    uint32    rat
);

/*****************************************************************************/
//  Description : This function checks the DTMF string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDtmfStr(
    uint8          length,
    uint8          *str
);

/*****************************************************************************/
//  Description : This function checks the CLIR call string
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

LOCAL ATC_STATUS ATC_ProcessGprsDial( // return the handle result
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    uint8  *dial_string_ptr, // point to the dial string
    uint32 *cid_ptr//[out] cid
);

/*****************************************************************************/
//  Description : This function start the dtmf key
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartDTMFKey(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T  *atc_config_ptr,
    uint8   dtmf_key,
    uint8   callid);

/*****************************************************************************/
//  Description : This function stop the dtmf key
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopDTMFKey(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T  *atc_config_ptr,
    uint8   callid);

/*****************************************************************************/
//  Description : This function get the dtmf key from ATC_CMD_PARAM_T
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDTMFKey(ATC_CMD_PARAM_T   *param_ptr,  //in
                         uint8      *dtmf_ptr                   //out:dtmf key
                        );

/*****************************************************************************/
//  Description : This function get the dtmf keys from dtmf string
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDTMFString(uint8    *in_ptr,               //in: dtmf string
                            uint16  in_len,             //in: string length
                            uint8     *dtmf_ptr,              //out:dtmf keys
                            uint8     *count_ptr          //out:key count
                           );

/*****************************************************************************/
//  Description : This function is used to read preferred PLMN list.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadPreferredPlmnList(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T    *atc_config_ptr
);

/*****************************************************************************/
//  Description : This function is used to read max num of preferred PLMN list.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ReadPreferredPlmnMaxNum(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
//  Description : This function is used to write duration to the list
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_WriteDTMFDuration(MN_DUAL_SYS_E   dual_sys,
                                                                       uint16 duration);

/*****************************************************************************/
//  Description : This function is used to read duration from the list
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL uint16 ATC_ReadDTMFDuration(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
//  Description : This function is used to RESET dtmf duration buffer
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL void ATC_ResetDTMFDuration(void);

/*****************************************************************************/
//  Description : This function is used to check whether the list is full
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_IsDTMFDurationListFull(MN_DUAL_SYS_E   dual_sys);

#ifdef _SUPPORT_CCBS_
/*****************************************************************************/
//  Description : This function store the ccbs information when receive ATA signal 
//  Global resource dependence : none
//  Author:       Haoming.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_StoreCCBSCall(ATC_CCBS_INFO_T ccbs_info,
                                        MN_DUAL_SYS_E   dual_sys
                                        );
#endif // #ifdef _SUPPORT_CCBS_

/*****************************************************************************/
//  Description : This function handle the ATA command,This command is used to
//                answer a single call.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessA)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8    call_id;
    uint8    call_index;
    uint8    active_id;
#ifdef _SUPPORT_CCBS_
    uint8 ccbs_index = ATC_MAX_CCBS_NUM;
#endif
    SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
    g_atc_expire_call_id = 0xff;
    g_atc_expire_ind = FALSE;
    s_atc_call_ring_num = 0;

    if(CheckPresentActiveCall(&g_calls_state[dual_sys], &active_id))
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessA: there is an active call, not allowed another!");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessA, there is a incoming waiting call!");

        call_index = GetIndexOfCallContext(call_id, dual_sys);

        if(ATC_INVALID_CALLID == call_index)
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessA, invalid call index!");
            return ERR_OPERATION_NOT_ALLOWED;
        }

#ifdef _MUX_ENABLE_
        ATC_HandleGprsSuspendRsp(atc_config_ptr);
#endif 

        if(MN_RETURN_SUCCESS == MNCALL_ConnectCallEx(dual_sys, call_id, PNULL))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessA, call_id:%d%s",
                          g_calls_state[dual_sys].call_context[call_index].call_id,
                          (g_calls_state[dual_sys].call_context[call_index].call_id == 0 ? " WARNING! not 0" : ""));
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_SETUP_COMPLETE_IND,
                              g_calls_state[dual_sys].call_context[call_index].call_id, ATC_DOMAIN_CS, dual_sys);
        }
        else
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessA, MNCALL_ConnectCall failure!");
            return ERR_UNKNOWN;
        }

        if(ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[call_index].call_mode)
        {
            s_mt_need_open_codec_flag |= CC_MT_CALL_CONNECTING;

            if((s_mt_need_open_codec_flag ==  (CC_MT_CALL_CONNECTING|CC_TCH_RESOURCE_ASSIGNED)) && 
                (!atc_config_ptr->audio_flag))
            {
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
                ATC_EnableUplinkMute(FALSE);
                ATC_EnableRemoteMute(FALSE);
                
                ATC_EnableVoiceCodec(TRUE); 

                s_mt_need_open_codec_flag = 0;
                atc_config_ptr->audio_flag = TRUE;
            }
        }

        g_atc_call_flag = TRUE;
    }
#ifdef _SUPPORT_CCBS_
    else if((atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index < ATC_MAX_CCBS_NUM + 1)
        &&(atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index != 0)
        &&(ATC_INVALID_CALLID != atc_config_ptr->ccbs_info[dual_sys].cur_call_id))
    {        
        ATC_TRACE_LOW("ATC: ATC_ProcessA, there is a CCBS call!");
        //ccbs
        ccbs_index = atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index;
        call_id = atc_config_ptr->ccbs_info[dual_sys].cur_call_id;

        if(MN_RETURN_SUCCESS == MNCALL_RecallRspEx(dual_sys, call_id, TRUE, NULL))
        {            
            if(S_ATC_FAIL == ATC_StoreCCBSCall(atc_config_ptr->ccbs_info[dual_sys], dual_sys))
            {
                return ERR_UNKNOWN;
            }
            sprintf((char *)g_rsp_str, "+CCBS: 3,%d", call_id);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            
            call_index = GetIndexOfCallContext(call_id, dual_sys);
            ATC_TRACE_LOW("ATC: ATC_ProcessA, call_id:%d%s",
              g_calls_state[dual_sys].call_context[call_index].call_id,
              (g_calls_state[dual_sys].call_context[call_index].call_id == 0 ? " WARNING! not 0" : ""));
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_SYNC_IND,
                              g_calls_state[dual_sys].call_context[call_index].call_id, ATC_DOMAIN_CS, dual_sys);   
                              

        }
        else
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessA, MNCALL_RecallRspEx failure!");
            return ERR_UNKNOWN;
        }
       
        atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index = 0xff;
        atc_config_ptr->ccbs_info[dual_sys].cur_call_id = 0xff;
        
        g_atc_call_flag = TRUE;
           
    }
#endif // #ifdef _SUPPORT_CCBS_    
    else
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessA, no incoming waiting call or CCBS call!");
        return ERR_NOT_FOUND;
    }

    return S_ATC_SUCCESS;  //report OK immediately to protect transient disconnection 
}

/*****************************************************************************/
//  Description : This function check the call type. The following number is an emergency call
//                a)112 and 911 shall always be available.
//                b)Any emergency call number stored on a SIM/USIM when the SIM/USIM is present.
//                c)000,08,110,999,118 and 119 when a SIM/USIM is not present. 
//                d)Additional emergency call numbers that may have been downloaded by the serving network when the SIM/USIM is present.                  
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:         
/*****************************************************************************/
BOOLEAN IsEmergencyCall(ATC_CONFIG_T *atc_config_ptr, uint8 *num_ptr, MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_CURRENT_PLMN_INFO_T    plmn_info;
    
    uint8                           tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1];
    MNSIM_ECC_T                     sim_ecc = {0};
    MN_EMG_NUM_T                    network_ecc = {0};
    uint32                          i;
    uint32                          j;
    static const char* const s_emergency_call[] = 
    {
        "112", "911"
    };

    static const char* const s_emergency_call_no_sim[] =
    {
        "000", "08", "110", "999", "118", "119"
    };

    static const char* const s_emergency_call_no_sim_custom[] =
    {
        "120", "122"
    };
    
    /* ref: spec 22.101 section10.1.1. */
    /* 112 and 911 shall always be available. */
    for(i = 0; i < ARR_SIZE(s_emergency_call); i++)
    {
        if( !strcmp((char *)num_ptr, s_emergency_call[i]))
        {
            ATC_TRACE_LOW("ATC: this is a emergency call,112 or 911 !");
            return TRUE;
        }
    }
    /* Any emergency call number stored on a SIM/USIM when the SIM/USIM is present. */
    sim_ecc = MNSIM_GetEccEx(dual_sys);
    for(i = 0; i < sim_ecc.validate_num; i++)
    {
        SCI_MEMSET(tel_str, '\0', (MN_MAX_ADDR_BCD_LEN * 2 + 1));
        TransformBcdToStr(sim_ecc.call_codes[i].ecc_code, MN_MAX_ECC_LEN, tel_str);

        if(!strcmp((char *)num_ptr, (char *)tel_str))
        {
            ATC_TRACE_LOW("ATC: this is a sim emergency call!");
            return TRUE;
        }
    }
    
    /* 000, 08, 110, 999, 118 and 119 when a SIM/USIM is not present. */
    /* 000, 08, 110, 999, 118 and 119 are emergency num when sim is present *
     * and plmn status is emergency service . ref: feature phone  */
    plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

    if((!atc_config_ptr->sim_flag[dual_sys]) ||
        (PLMN_EMERGENCY_ONLY == plmn_info.plmn_status))
    {
        ATC_TRACE_LOW("ATC: sim/usim is invalid or unreachable!");
        for(i = 0; i < ARR_SIZE(s_emergency_call_no_sim); i++)
        {
            if( !strcmp((char *)num_ptr, s_emergency_call_no_sim[i]))
            {
                ATC_TRACE_LOW("ATC: this is a no sim emergency call!");
                return TRUE;
            }
        }
		
        //custom defined
        for(j = 0; j < ARR_SIZE(s_emergency_call_no_sim_custom); j++)
        {
            if( !strcmp((char *)num_ptr, s_emergency_call_no_sim_custom[j]))
            {
                ATC_TRACE_LOW("ATC: this is a custom defined no sim emergency call!");
                return TRUE;
            }
        }
    }

    /* Additional emergency call numbers that may have been downloaded by the serving network when the SIM/USIM is present. */
    if((g_emg_num_list_ptr[dual_sys] != PNULL)  && 
        (plmn_info.mcc == g_emg_num_list_ptr[dual_sys]->mcc))
    {
        for(i = 0; i < g_emg_num_list_ptr[dual_sys]->list_num; i++)
        {
            network_ecc = g_emg_num_list_ptr[dual_sys]->emg_num_list[i];
            SCI_MEMSET(tel_str, '\0', (MN_MAX_ADDR_BCD_LEN * 2 + 1));
            TransformBcdToStr(network_ecc.party_num, network_ecc.num_len, tel_str);
            if( !strcmp((char *)num_ptr, (char *)tel_str))
            {
                ATC_TRACE_LOW("ATC: this is a network provided emergency call!");
                return TRUE;
            }
        }
    }
    return FALSE;
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the ATD command 
//  Global resource dependence : none
//  Author:       chong.liu
//  Note:         Modify by Liuhan
/*****************************************************************************/
BOOLEAN ATD_ParsePukAndPinStr(uint8 *str, MN_BLOCK_T *puk_info, MN_BLOCK_T *pin_info)
{
    BOOLEAN result = FALSE;
    uint8       i = 0, j = 0;

    //judge input string length if is valid
    if ((str[4] == '*') && (strlen((char *)str) < 20))//pin
    {
        return (result);
    }
    else if ((str[4] != '*') && (strlen((char *)str) < 21))//pin2
    {
        return (result);
    }

    //decide puk or old pin start point
    if (str[4] == '*')//pin
    {
        i = 5;
    }
    else//pin2
    {
        i = 6;
    }

    //get puk or old pin length
    while ((str[i+j] != '*') && (j < 9))
    {
        j++;
    }

    //judge the length of puk or old pin if is valid
    if ((j < 4) || (j > 8))
    {
        return (result);
    }
    puk_info->blocklen = j;
    memcpy((char*)puk_info->blockbuf,(char*)(str + i),puk_info->blocklen);
    
    //decide new pin start point
    i = i +j + 1;
    j = 0;

    //get new pin length
    while ((str[i+j] != '*') && (j < 9))
    {
        j++;
    }

    //judge the length of new pin if is valid
    if ((j < 4) || (j > 8))
    {
        return (result);
    }
    pin_info->blocklen = j;
    memcpy((char*)pin_info->blockbuf,(char*)(str + i), pin_info->blocklen);

    //decide new pin confirm start point
    i = i +j + 1;
    j = 0;

    //get new pin confirm length
    while ((str[i+j] != '#') && (j < 9))
    {
        j++;
    }

    //judge the length of new pin confirm if is valid
    if ((j < 4) || (j > 8))
    {
        return (result);
    }

    //judge new pin and new pin confirm if is equal
    if (j == pin_info->blocklen)//length
    {
        if (0 == strncmp((char*)pin_info->blockbuf,(char*)(str + i),pin_info->blocklen))
        {
            result = TRUE;
            return (result);
        }
        else
        {
            result = FALSE;
            return (result);
        }
    }
    else
    {
        result = FALSE;
        return (result);
    }
}

/*****************************************************************************/
//  Description : This function handle the ATD command 
//  Global resource dependence : none
//  Author:       chong.liu
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_AtdChangePukAndPin( MN_DUAL_SYS_E    dual_sys,
                                      ATC_CONFIG_T     *atc_config_ptr,
                                      uint8            *telephone_buf,
                                      BOOLEAN * stop_enable
                                    )
{
    ERR_MNPHONE_CODE_E  err_code;
    MN_BLOCK_T puk_info= {0};
    MN_BLOCK_T pin_info= {0};    

     /*enter **05*PUK*NEW_PIN*NEW_PIN# to change pin*/
    if(!memcmp(telephone_buf, "**05*", 5))
    {
        if(ATD_ParsePukAndPinStr(telephone_buf, &puk_info, &pin_info))
        {
            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK, MNSIM_PIN1, &puk_info, &pin_info);
            if(err_code == ERR_MNPHONE_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                          APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                *stop_enable = TRUE;
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                *stop_enable = TRUE;
                return ERR_OPERATION_NOT_SUPPORTED;
            } 
        }
        else
        {
            *stop_enable = FALSE;
            return S_ATC_DEFAULT_HANDLE; 
        }
    }
     //enter **04*PIN*NEW_PIN*NEW_PIN# to change pin//
    else if(!memcmp(telephone_buf, "**04*", 5))
    {
        if(ATD_ParsePukAndPinStr(telephone_buf, &puk_info, &pin_info))
        {
            if(MNSIM_IsPin1EnableEx(MN_DUAL_SYS_1))//PIN1 activated
            {
                err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN1, &puk_info, &pin_info);
                if(err_code == ERR_MNPHONE_NO_ERR)
                {
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                                APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                    *stop_enable = TRUE;
                    return S_ATC_DEFAULT_HANDLE;
                }
                else
                {
                    *stop_enable = TRUE;
                    return ERR_OPERATION_NOT_SUPPORTED;
                } 
            }
            else
            {
                *stop_enable = TRUE;
                return  ERR_SERVICE_PROVIDER_PERSONALISATION_PIN_REQUIRED;
            }
        }
        else
        {
            *stop_enable = FALSE;
            return S_ATC_DEFAULT_HANDLE; 
        }
    }
    //enter **052*PUK2*NEW_PIN2*NEW_PIN2# to change pin2//
    else if(!memcmp(g_tel_str, "**052*", 6))
    {
        if(ATD_ParsePukAndPinStr(g_tel_str, &puk_info, &pin_info))
        {
            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_UNBLOCK, MNSIM_PIN2, &puk_info, &pin_info);
            if(err_code == ERR_MNPHONE_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                            APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                *stop_enable = TRUE;
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                *stop_enable = TRUE;
                return ERR_OPERATION_NOT_SUPPORTED;
            } 
        }
        else
        {
            *stop_enable = FALSE;
            return S_ATC_DEFAULT_HANDLE; 
        }
    }
    //enter **042*PIN2*NEW_PIN2*NEW_PIN2# to change pin2//
    else if(!memcmp(g_tel_str, "**042*", 6))
    {
        if(ATD_ParsePukAndPinStr(g_tel_str, &puk_info, &pin_info))
        {
            err_code = MNPHONE_OperatePinEx(dual_sys, MNSIM_PIN_CHANGE, MNSIM_PIN2, &puk_info, &pin_info);
            if(err_code == ERR_MNPHONE_NO_ERR)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                            APP_MN_SIM_PIN_FUNC_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                *stop_enable = TRUE;
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                *stop_enable = TRUE;
                return ERR_OPERATION_NOT_SUPPORTED;
            }
        }
        else
        {
            *stop_enable = FALSE;
            return S_ATC_DEFAULT_HANDLE; 
        }
    }
    else
    {
        *stop_enable = FALSE;
        return S_ATC_DEFAULT_HANDLE; 
    }
}

/*****************************************************************************/
//  Description : This function handle the AT+XLEMA command
//  Global resource dependence : none
//  Author:      
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessXLEMA)
{
    ATC_STATUS                      status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E                   dual_sys = ATC_CURRENT_SIM_ID_MUX; 
    MNSIM_ECC_T                     sim_ecc = {0};
    uint8                           tel_str[MN_MAX_ADDR_BCD_LEN*2 + 1]={0};
    uint32                          i,j;
    uint32                          str_len;

    const char *all_emc_num_list[] =
    {
	    "112","911","000","08","110","118","119","999","120","122"
    };
		
    switch (ATC_GET_CMD_TYPE)
    {
     case ATC_CMD_TYPE_EXECUTE:
	 	
        /* SIM/USIM is not present. */
        if((!atc_config_ptr->sim_flag[dual_sys]))
        {
            SCI_TRACE_LOW("ATC: sim/usim is invalid or no sim inserted."); 
            sprintf((char *)g_rsp_str, "+XLEMA: \"%s\"", all_emc_num_list[0]);
            for(i=1; i<ARR_SIZE(all_emc_num_list); i++)
            {
                str_len = strlen((char*)g_rsp_str);
                sprintf((char *)&g_rsp_str[str_len], ",\"%s\"",all_emc_num_list[i]);
            }
        }
        else
        {
            /* Normal status, 112 and 911 shall always be available emergency calls */
            SCI_TRACE_LOW("ATC: normal status, emergency calls include:112 or 911 !");
            sprintf((char *)g_rsp_str, "%s: \"112\",\"911\" ", g_atc_info_table[AT_CMD_XLEMA].cmd_name->str_ptr); 
        }

        /* Any emergency call number stored on a SIM/USIM when the SIM/USIM is present. */
        sim_ecc = MNSIM_GetEccEx(dual_sys);
        if(sim_ecc.validate_num != 0)
        {
            //SCI_TRACE_LOW("ATC: this is a sim emergency call!");
            for(i=0; i < sim_ecc.validate_num; i++)
            {
                BOOLEAN exist_flag = FALSE;
                
                SCI_MEMSET(tel_str, '\0', (MN_MAX_ADDR_BCD_LEN * 2 + 1));
                TransformBcdToStr(sim_ecc.call_codes[i].ecc_code, MN_MAX_ECC_LEN, tel_str);

                for(j=0; j<ARR_SIZE(all_emc_num_list); j++)
                {
                    if(!strcmp((char *)tel_str, all_emc_num_list[j]))
                    {
                        exist_flag = TRUE;
                        break;
                    }
                }
                if(!exist_flag)
                {
                    str_len = strlen((char*)g_rsp_str);
                    sprintf((char *)(g_rsp_str+str_len), ",\"%s\"", tel_str);
                }
            }
			
        }
        ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
	     
        break;
    default:
        SCI_TRACE_LOW("ATC: ATC_ProcessXLEMA  Command Type not match");
        break;
    }
    return status;
}
#endif/*ATC_SAT_ENABLE*/
/*****************************************************************************/
//  Description : This function handle the ATD command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessD)
{
    MN_CALL_CLIR_E            clir = MN_CALL_CLIR_SUPPRESSION; // the default value
    MN_CALLED_NUMBER_T        called_num = {0};
    BOOLEAN                   is_clir_call = FALSE;
    BOOLEAN                   status = FALSE;
    uint8                     bcd_len = 0;
    ATC_STATUS                ss_status;
    MN_CALL_TYPE_E            call_type;
    uint32                    num_len = 0;
    uint32                    pdp_cid = 0xff;
    ATC_TELE_TYPE_E           temp_atset_number_type = ISDN_TELE_PLAN_UNKNOWN_TYPE;
    MN_DUAL_SYS_E             dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8                     i = 0;
    
#ifdef _MUX_ENABLE_
    uint8                     temp_link_id = atc_config_ptr->current_link_id;
#endif

    SCI_MEMSET(&called_num, 0, sizeof(MN_CALLED_NUMBER_T));

    if((ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)  || (!PARAM1_FLAG))
    {
        return ERR_OPERATION_NOT_ALLOWED;
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

    if(!memcmp(PARA1.str_ptr->str_ptr, "*99", 3) ||
            !memcmp(PARA1.str_ptr->str_ptr, "*98", 3))
    {
        //if plmn NO SERVICE or PLMN EMERGENCY ONLY,return
        if ((PLMN_NO_SERVICE == s_plmn_status[dual_sys]) ||(PLMN_EMERGENCY_ONLY==s_plmn_status[dual_sys]) )
        {
            //send NO CARRIER
            SCI_TRACE_LOW("ATC: ATC_ProcessD:PLMN_NO_SERVICE or  PLMN_EMERGENCY_ONLY: No Carrier");
            sprintf((char *)g_rsp_str, "NO CARRIER");
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

            return S_ATC_RESULT_CODE_NO_CARRIER;
        }
			
        ss_status = ATC_ProcessGprsDial(dual_sys, atc_config_ptr, PARA1.str_ptr->str_ptr, &pdp_cid);
        pdp_cid = MIN(pdp_cid, MN_GPRS_MAX_PDP_CONTEXT_COUNT);
		
#ifdef _MUX_ENABLE_
        g_packet_type[pdp_cid] = TCPIP_PKTTYPE_PPP;

        ATC_TRACE_LOW("ATC: ATC_ProcessD: dialing GPRS call cid = %d, link_id = %d,ss_status:%d", pdp_cid, atc_config_ptr->current_link_id, ss_status);
#endif

        if(ss_status == S_ATC_DEFAULT_HANDLE)
        {
            sprintf((char *)g_rsp_str, "^ORIG: %d,%d", (int)pdp_cid, ATC_CMCC_CALLIND_CALL_TYPE_PACKET_DATA);
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
            if (atc_config_ptr->async_mode_flag)
            {
                ss_status = S_ATC_SUCCESS;
            }
        }

        return ss_status;
    }

    SCI_MEMSET(g_tel_str, 0, (2 * MN_MAX_ADDR_BCD_LEN + 1));

    if(atc_config_ptr->atc_config_nv.clir_flag == 0)
    {
        clir = MN_CALL_CLIR_NETWORK_DECIDE;
    }
    else if(atc_config_ptr->atc_config_nv.clir_flag == 1)
    {
        clir = MN_CALL_CLIR_INVOCATION;
    }
    else if(atc_config_ptr->atc_config_nv.clir_flag == 2)
    {
        clir = MN_CALL_CLIR_SUPPRESSION;
    }

    num_len = PARA1.str_ptr->str_len;

    if(num_len > 3)
    {
        uint8 str_length = 0;

        while(str_length < num_len - 2)
        {
            if(PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'T' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'P' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'W' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == '@' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == ',' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == '#' ||
                    PARA1.str_ptr->str_ptr[num_len-str_length-1] == '!')
            {
                str_length++ ;
                continue; /* we ignore these modifiers */
            }

            break;
        }

        if(PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'i'  ||
                PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'I')  /* now we got the i/I flag */
        {
            clir = MN_CALL_CLIR_SUPPRESSION;

            if(PARA1.str_ptr->str_ptr[num_len-str_length-1] == 'I')
            {
                clir = MN_CALL_CLIR_INVOCATION;
            }

            if(str_length > 0)
            {
                memmove(&(PARA1.str_ptr->str_ptr[num_len-str_length-1]),
                        &(PARA1.str_ptr->str_ptr[num_len-str_length]),
                        str_length);
            }

            PARA1.str_ptr->str_ptr[num_len-1] = '\0';
            num_len--;
        }
    }

    if(PARA1.str_ptr->str_ptr[0] == '+')
    {
        temp_atset_number_type = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
        PARA1.str_ptr->str_ptr++;
        num_len--;
    }

    //Check the length of input number
    if(2 * MN_MAX_ADDR_BCD_LEN < num_len)
    {
        return ERR_INVALID_INDEX;
    }

    SCI_MEMCPY(g_tel_str, PARA1.str_ptr->str_ptr, num_len);

#ifndef _ULTRA_LOW_CODE_
    is_clir_call = HandleClirCall(g_tel_str, &num_len, &clir);

    if(!is_clir_call)
    {
    #ifndef ATC_SAT_ENABLE
        ss_status = ATC_AtdChangePukAndPin(dual_sys, atc_config_ptr, g_tel_str, &status);

        if(status)
        {
            return ss_status;
        }
    #endif/*ATC_SAT_ENABLE*/
        if(num_len == 2 && g_tel_str[0] == '0' && g_tel_str[1] == '8')
        {
            ss_status = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            ss_status = ATC_CheckSSStr(dual_sys, atc_config_ptr, g_tel_str, num_len);
        }

        if(S_ATC_DEFAULT_HANDLE != ss_status)
        {
            //it is a ss or ussd request
            if(S_ATC_SUCCESS == ss_status)
            {
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                return ss_status;
            }
        }
    }

#endif /* _ULTRA_LOW_CODE_ */

    g_tel_str[num_len] = '\0';

    // check call type
    // emergency call: treat ATD911,# as a emergency call for PTCRB case
    if(g_tel_str[num_len-1] == '#' && g_tel_str[num_len-2] == ',')
    {
        ATC_TRACE_LOW("ATC: this is a emergency call!");
        call_type = MN_CALL_TYPE_EMERGENCY;
        g_tel_str[num_len-2] = '\0';
        num_len -= 2;
    }
    // If the dial string is terminated by ';', the call is voice call.ref: v.25 6.3.1
    else
    {
        ATC_TRACE_LOW("ATC: this is a voice call!");

        call_type = MN_CALL_TYPE_NORMAL;
        
        /* ref: spec 22.101 section10.1.1. */
        /* 112 and 911 shall always be available. */
        /* Any emergency call number stored on a SIM/USIM when the SIM/USIM is present. */
        /* 000, 08, 110, 999, 118 and 119 when a SIM/USIM is not present. */
        if(IsEmergencyCall(atc_config_ptr, g_tel_str, dual_sys))
        {
            call_type = MN_CALL_TYPE_EMERGENCY;
        }
    }

    status = TransfromStrToBcd(called_num.party_num, &bcd_len, g_tel_str, num_len);

    if(!status)
    {
        return ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
    }

    called_num.num_len = bcd_len;

    switch(atset_number_type)
    {
        case ISDN_TELE_PLAN_UNKNOWN_TYPE:
            called_num.number_type = MN_NUM_TYPE_UNKNOW;
            called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
            break;

        case ISDN_TELE_PLAN_INTERNATIONAL_TYPE:
            called_num.number_type = MN_NUM_TYPE_INTERNATIONAL;
            called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
            break;

        case ISDN_TELE_PLAN_NATIONAL_TYPE:
            called_num.number_type = MN_NUM_TYPE_NATIONAL;
            called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
            break;

        case UNKNOWN_PLAN_UNKNOWN_TYPE:
        default:
            called_num.number_type = MN_NUM_TYPE_UNKNOW;
            called_num.number_plan = MN_NUM_PLAN_UNKNOW;
            break;
    }

    if(temp_atset_number_type == ISDN_TELE_PLAN_INTERNATIONAL_TYPE)
    {
        called_num.number_type = MN_NUM_TYPE_INTERNATIONAL;
        called_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
    }

    //record last call num for ATDL
    SCI_MEMSET((void *)&g_last_mo_call, 0, sizeof(ATC_LAST_CALLED_NUM_T));
    g_last_mo_call.called_num_present = TRUE;
    g_last_mo_call.call_type = call_type;
    g_last_mo_call.call_clir = clir;
    SCI_MEMCPY(&g_last_mo_call.called_num, &called_num, sizeof(MN_CALLED_NUMBER_T));

    //if previous at+chld command is processing, push current call into last call buffer
    if (atc_config_ptr->ss_operate[dual_sys] != ATC_INVALID_OPERATE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessD, ss_operate = %d", atc_config_ptr->ss_operate[dual_sys]);
        g_last_mo_call.is_buffer_call = TRUE;
#ifdef _MUX_ENABLE_
        g_last_mo_call.buffer_link_id = atc_config_ptr->current_link_id;
#endif
        return S_ATC_DEFAULT_HANDLE;
    }
    
    for(i=0; i<MN_SYS_NUMBER; i++)
    {
        if((i != dual_sys) && (g_calls_state[i].call_amount != 0))
        {
            SCI_TRACE_LOW("ATC: mo meet another sim mt. dual_sys = %d, i = %d, amount = %d", dual_sys, i, g_calls_state[i].call_amount);
            //exist a call in other sim card, reject the request.
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

            ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
            ATC_SET_CURRENT_LINK_ID_MUX(temp_link_id); 

            return S_ATC_DEFAULT_HANDLE;
        }
    }

#ifdef _MUX_ENABLE_
    ATC_HandleGprsSuspendRsp(atc_config_ptr);
#endif 

    // start a call
    if(MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys, call_type, &called_num, PNULL, clir, PNULL))
    {
#ifdef _MUX_ENABLE_
        ATC_TRACE_LOW("ATC:ATC_ProcessD call_link_id = %d ", atc_config_ptr->current_link_id);

        ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id, ATC_ATH_EV_NEW_CS_SRV, 0);
#else
        ATC_Link_ATHEventHandle(0, ATC_ATH_EV_NEW_CS_SRV, 0);
#endif

        g_atc_call_flag      = TRUE;
        g_atc_call_need_disc = FALSE;    //reset the neee_disc flag
        atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS;
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          (uint16)APP_MN_CALL_START_IND, 0xff, ATC_DOMAIN_CS, dual_sys);

        return S_ATC_DEFAULT_HANDLE;
    }
    else
    {
        if(ERR_MNCALL_FAIL_BY_FDN == MNCALL_GetErrCodeEx(dual_sys))
        {
            atc_config_ptr->ceer_val = CEER_ERR_241_FDN_RESTRICTION;
            return ERR_SIM_FDN_FAILED;
        }
        else 
        {
            return ERR_INVALID_INDEX;
        }   
    }
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_CALL_START_IND signal and return
//                the unsolicited result code.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallStartInd(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_CALL_START_IND_T *sig_ptr        // point to the APP_MN_CALL_START_IND signal
)
{
    MN_DUAL_SYS_E                dual_sys = sig_ptr->dual_sys;
    MN_CALL_TYPE_E               call_type = sig_ptr->call_type;
#ifndef ATC_SAT_ENABLE	
    MN_CALLED_NUMBER_T        called_num = sig_ptr->called_num;
#endif
    uint8                     call_index;
    uint8                    *party_number_ptr = PNULL;

    ATC_CMCC_CALLIND_CALL_TYPE_E    cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_VOICE;

    if(g_calls_state[dual_sys].call_amount >= MN_CALL_MAX_CALL_NUM)
    {
        ATC_TRACE_LOW("ATC: Assert Failure call_amount error %d.", g_calls_state[dual_sys].call_amount);
        return S_ATC_DEFAULT_HANDLE;
    }

    //set the initial value when call course starting
    s_atc_call_disc_cause = 0xffffffff;

    g_calls_state[dual_sys].call_amount++;

    ATC_TRACE_LOW("ATC: ATC_ProcessCallStartInd call_type:%d, call_amount: %d",
                  sig_ptr->call_type,
                  g_calls_state[dual_sys].call_amount - 1
                 );

    call_index = g_calls_state[dual_sys].call_amount - 1;
    
    g_calls_state[dual_sys].call_context[call_index].direction = FALSE;
    g_calls_state[dual_sys].call_context[call_index].call_id = sig_ptr->call_id;
    g_calls_state[dual_sys].call_context[call_index].call_entity_state = ATC_CALL_OUTGOING_STATE;
    g_calls_state[dual_sys].call_context[call_index].is_emergency_call = FALSE;

    switch(sig_ptr->call_type)
    {
        case MN_CALL_TYPE_NORMAL:
        case MN_CALL_TYPE_SPEECH_PREFER:
            g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_VOICE_MODE;
            break;

        case MN_CALL_TYPE_EMERGENCY:
            g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_VOICE_MODE;
            g_calls_state[dual_sys].call_context[call_index].is_emergency_call = TRUE;
            break;

        case MN_CALL_TYPE_DATA:
        case MN_CALL_TYPE_DATA_PREFER:
            g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_DATA_MODE;
            break;

        default:
            g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_UNKNOWN_CALLMODE;
            break;
    }

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

    party_number_ptr = g_calls_state[dual_sys].call_context[call_index].party_number;

    ATC_ConvertCalledNumToStr(&(sig_ptr->called_num),
                              &(g_calls_state[dual_sys].call_context[call_index].number_type),
                              party_number_ptr, NULL);

    ATC_UpECINDInfo(dual_sys, atc_config_ptr, NULL, 0, 2, sig_ptr->call_id + 1, 0, 0);

    ATC_UpDSCIInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, call_index, 2);


    if(g_calls_state[dual_sys].call_context[call_index].call_mode == ATC_VOICE_MODE)
    {
        cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_VOICE;

        if(sig_ptr->call_type == MN_CALL_TYPE_EMERGENCY)
        {
            cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_EMERGENCY;
        }
    }
    else if(g_calls_state[dual_sys].call_context[call_index].call_mode == ATC_DATA_MODE)
    {
        cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_CIRCUIT_DATA;
    }
    else
    {
        cmcc_call_type = ATC_CMCC_CALLIND_CALL_TYPE_INVALID;
    }

    sprintf((char *)g_rsp_str, "^ORIG: %d,%d", sig_ptr->call_id + 1, cmcc_call_type);
    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);


    //Send the unsolicited +SIND: 5
    ATC_UpSINDInfo(dual_sys,
                   atc_config_ptr,
                   (xSignalHeaderRec *)sig_ptr,
                   ATC_WIND_BIT4,
                   5,
                   (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));

    if( g_atc_call_flag)
    {
        // user maybe miss the first word, if opening audio when atc receives sync_ind message, 
        if((ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[call_index].call_mode) && 
            (!atc_config_ptr->audio_flag))
        {
            AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
            AUDIO_SetVolume(atc_config_ptr->speaker_volume);
            ATC_EnableUplinkMute(FALSE);
            ATC_EnableRemoteMute(FALSE);    
            
            ATC_EnableVoiceCodec(TRUE);
            ATC_HandleMoMuteAndLocalRingState(dual_sys, APP_MN_CALL_START_IND, call_index);
            atc_config_ptr->audio_flag = TRUE;
        }

        if(call_type == MN_CALL_TYPE_NORMAL || call_type == MN_CALL_TYPE_EMERGENCY || call_type == MN_CALL_TYPE_SPEECH_PREFER)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }

    }
#ifndef ATC_SAT_ENABLE	
    if(g_atc_call_flag && (ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[call_index].call_mode))
    {
        SCI_TRACE_LOW("ATC:ATC_Begin to update LND call count");
	ATC_RecordLNDCallInfo(dual_sys, PHONEBOOK_LND_STORAGE, &called_num);
    }
#endif
    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_EMG_NUM_LIST_IND signal.
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:
/*****************************************************************************/
void ATC_ProcessEmgNumListInd(
    ATC_CONFIG_T            *atc_config_ptr,// point to the ATC_CONFIG_T struct.
    APP_MN_EMG_NUM_LIST_IND_T *sig_ptr        // point to the APP_MN_EMG_NUM_LIST_IND signal
)
{
    MN_DUAL_SYS_E      dual_sys = sig_ptr->dual_sys;
    uint8              emg_num_str[MN_MAX_ADDR_BCD_LEN*2 + 1];
    uint32             str_len;
    uint32             i;
    
    if(PNULL != g_emg_num_list_ptr[dual_sys])
    {
        SCI_FREE(g_emg_num_list_ptr[dual_sys]);
    }

    g_emg_num_list_ptr[dual_sys] = (ATC_EMG_NUM_LIST_IND_T *)SCI_ALLOC_BASE(sizeof(ATC_EMG_NUM_LIST_IND_T));

    g_emg_num_list_ptr[dual_sys]->mcc       = sig_ptr->mcc;
    g_emg_num_list_ptr[dual_sys]->list_num  = sig_ptr->list_num;

    for(i=0; i<g_emg_num_list_ptr[dual_sys]->list_num; i++)
    {
        g_emg_num_list_ptr[dual_sys]->emg_num_list[i] = sig_ptr->emg_num_list[i];
    }

    // send emg num list to TE, hex string
    SCI_MEMSET(emg_num_str, '\0', (MN_MAX_ADDR_BCD_LEN * 2 + 1));
    TransformBcdToStr(g_emg_num_list_ptr[dual_sys]->emg_num_list[0].party_num, 
                      g_emg_num_list_ptr[dual_sys]->emg_num_list[0].num_len, 
                      emg_num_str);
    
    
    sprintf((char *)g_rsp_str, "+SEMCNUM: %d,%s", g_emg_num_list_ptr[dual_sys]->mcc, emg_num_str);
    for(i=1; i<g_emg_num_list_ptr[dual_sys]->list_num; i++)
    {
        SCI_MEMSET(emg_num_str, '\0', (MN_MAX_ADDR_BCD_LEN * 2 + 1));
        TransformBcdToStr(g_emg_num_list_ptr[dual_sys]->emg_num_list[i].party_num, 
                      g_emg_num_list_ptr[dual_sys]->emg_num_list[i].num_len, 
                      emg_num_str);
        str_len = strlen((char*)g_rsp_str);
        sprintf((char *)&g_rsp_str[str_len], ",%s", emg_num_str);
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
}

#ifndef _ULTRA_LOW_CODE_
#ifdef _SUPPORT_GPRS_
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
    uint8 *first_ptr = PNULL;
    uint8 *last_ptr = PNULL;
    uint8   index = 0;

    first_ptr = dial_string_ptr + 3;
    last_ptr = first_ptr;

    while(*first_ptr != '\0')
    {
        if(*first_ptr == '#')
        {
            if(last_ptr != first_ptr)
            {
                param_len[index] = first_ptr - last_ptr - 1;
                param_ptr[index] = last_ptr + 1;
            }

            first_ptr ++;

            //*99# or * 98#
            if((*last_ptr == '#' || index <= 2 && *last_ptr == '*') && *first_ptr == '\0')
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
                param_ptr[index] = last_ptr + 1;
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
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    uint8  *dial_string_ptr ,// point to the dial string
    GPRS_DIAL_TYPE_E    gprs_dial_type,//gprs dial type
    uint32 *cid_ptr
)
{
    uint32    pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    uint16    param_len[3] = {0};
    uint8    *param_ptr[3] = {PNULL, PNULL, PNULL};
    uint8     param_pid_index = 0;
    BOOLEAN    is_pid = FALSE;
    uint8     *pid_ptr = PNULL;
    uint32    pid = 0;
#ifdef HUAWEI_6500_MODEM
    int32   ppp_init_result = 0;
    int32   set_ppp_result = 0;
    uint8   ppp_link_id = 0;
#endif

    if (!atc_config_ptr)
    {
        ATC_TRACE_LOW("ATC: ProcessGprsDial atc_config_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if (!dial_string_ptr)
    {
        ATC_TRACE_LOW("ATC: ProcessGprsDial dial_string_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    //decode the total string as format:"*<99>*<call address>*<L2P>*<pid>"
    //or "*<98>*<pid>"
    if(!DecGprsDialParam(dial_string_ptr, param_len, param_ptr))
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(GPRS_DIAL_99 == gprs_dial_type)
    {
    #ifdef HUAWEI_6500_MODEM
        /*check call address */
        if(PNULL != param_ptr[0] && (memcmp(param_ptr[0], "**", 2)))
        {
            SCI_TRACE_LOW("ATC: Process_ATD: string format mismatch!");
            return ERR_OPERATION_NOT_SUPPORTED;
        }
    #endif
	
        if(param_len[1])
        {
            if(!memcmp(param_ptr[1], "PPP", 3))
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
    else if(GPRS_DIAL_98 == gprs_dial_type)
    {
        if(PNULL != param_ptr[1] || PNULL != param_ptr[2])
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
        pid_ptr = (uint8 *)SCI_ALLOC_BASE(param_len[param_pid_index] + 1);
        SCI_MEMCPY((void *)pid_ptr, (void *)param_ptr[param_pid_index], param_len[param_pid_index]);
        pid_ptr[param_len[param_pid_index]] = 0;
        pid = atoi((char *)pid_ptr);
        SCI_FREE(pid_ptr);
    }
    else
    {
        /*
         * set default cid 1 for modem, 2 for feature phone
         */
        pid = 1;
    }

    SCI_TRACE_LOW("ATC: ATD*99 or 98#  RequestPdpId pdp_id:%d", pid);

    if(pid == 0 || pid > MN_GPRS_MAX_PDP_CONTEXT_COUNT)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    atc_config_ptr->pdp_id_arr[pid-1] = TRUE;
    pdp_id_arr[0] = pid;
    pdp_id_arr[1] = 0;
    *cid_ptr = pid;//return pid info

    SCI_MEMCPY(&s_pid_arr, &pdp_id_arr, sizeof(s_pid_arr));

#ifdef _MUX_ENABLE_

    /*if l4 has cleared pdp context but ATC task has not handled APP_MN_DEACTIVATE_PDP_CONTEXT_CNF yet,
      It is not allowed to activate pdp context with the same cid.*/
    if(ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint16)pid, ATC_DOMAIN_PS, dual_sys) ||
        ATC_Is_Expected_Event_Present(APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys))
    {
        ATC_TRACE_LOW("ATC: ProcessGprsDial, this pdp id is being deactivated.");
        return ERR_GPRS_OPERATION_FAILURE;
    }

#endif

#ifndef HUAWEI_6500_MODEM
    if(ERR_MNGPRS_NO_ERR != ATC_ActivatePdpContextEx(atc_config_ptr,dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED))
    //if(ERR_MNGPRS_NO_ERR != MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED))
    {
        return ERR_GPRS_OPERATION_FAILURE;
    }
#else
    ppp_link_id = ATC_Get_Data_Link_Id(atc_config_ptr->current_link_id, pid);

    SCI_TRACE_LOW("ATC: ATD*99 or 98#  pdp_id:%d", pid);
    g_undefined_cid = pid;

    ppp_init_result = PPPPS_InitPpp(ppp_link_id);

    if (!ppp_init_result)  
    {
        //AT send connect 
        if (!atc_config_ptr->async_mode_flag)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_CONNECT, 0);
        }
        else
        {
            sprintf((char *)g_rsp_str, "CONNECT");
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
        }
    
        //open modem service of ppp: mux,ppp transmission
        set_ppp_result = PPPPS_OpenModemService(ppp_link_id); 
		
        if(set_ppp_result)
        {
            return ERR_GPRS_OPERATION_FAILURE;
        }
		
        ATC_TRACE_LOW("ATC: Open MODEM service!");

    }
    else
    {
        return ERR_GPRS_OPERATION_FAILURE;        
    }
    
#endif

    return S_ATC_DEFAULT_HANDLE;
}
#endif/*_SUPPORT_GPRS_*/
#endif/*_ULTRA_LOW_CODE_*/

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
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T    *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    uint8  *dial_string_ptr, // point to the dial string
    uint32 *cid_ptr
)
{
#if !defined(_ULTRA_LOW_CODE_) && defined(_SUPPORT_GPRS_)
    GPRS_DIAL_TYPE_E    gprs_dial_type = GPRS_DIAL_MAX;
    ATC_STATUS status = S_ATC_SUCCESS;

    if(!memcmp(dial_string_ptr, "*99", 3))
    {
        gprs_dial_type = GPRS_DIAL_99;
    }
    else if(!memcmp(dial_string_ptr, "*98", 3))
    {
        gprs_dial_type = GPRS_DIAL_98;
    }
    else
    {
        return ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
    }


    status = ProcessGprsDial(dual_sys, atc_config_ptr, dial_string_ptr, gprs_dial_type, cid_ptr);

#ifndef HUAWEI_6500_MODEM
    if(S_ATC_DEFAULT_HANDLE == status)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessGprsDial cid=%d", *cid_ptr);

#ifdef _MUX_ENABLE_
        ATC_Link_ATHEventHandle(atc_config_ptr->current_link_id, ATC_ATH_EV_NEW_PPP_SRV, (uint8)*cid_ptr);
        ATC_Create_PSD_Call(*cid_ptr, atc_config_ptr->current_link_id, TRUE);
        g_atc_modem_dial_sys = dual_sys;
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          APP_MN_ACTIVATE_PDP_CONTEXT_CNF, *cid_ptr, ATC_DOMAIN_PS, g_atc_modem_dial_sys);
#else
        ATC_Link_ATHEventHandle(0, ATC_ATH_EV_NEW_PPP_SRV, (uint8)*cid_ptr);
        ATC_Create_PSD_Call(*cid_ptr, 0, TRUE);
#endif
    }
    else
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessGprsDial, failure");
    }
#endif

    return status;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handle the AT+VTD command, This command is used
//                to define the tone duration.
//  AT+VTD=<n>
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessVTD)
{
#ifdef _ULTRA_LOW_CODE_
    return S_ATC_FAIL;
#else
    ATC_STATUS    status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_config_ptr->vtd_value = PARA1.num;
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%d", atc_config_ptr->vtd_value);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "+VTD: (0-255)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function start the dtmf key
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StartDTMFKey(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T *atc_config_ptr,
    uint8        dtmf_key,
    uint8        callid
)
{
    if(callid >= MN_CALL_MAX_CALL_NUM)
    {
        ATC_TRACE_LOW("ATC: StartDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM");
        return FALSE;
    }

    if(MN_RETURN_SUCCESS == MNCALL_StartDTMFEx(dual_sys, dtmf_key, callid))
    {
        ATC_TRACE_LOW("ATC: MNCALL_StartDTMF, call id:%d dtmf key:%c", callid, dtmf_key);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          (uint16)APP_MN_START_DTMF_CNF, callid, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        ATC_TRACE_LOW("ATC: MNCALL_StartDTMF failed.");
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function stop the dtmf key
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN StopDTMFKey(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T  *atc_config_ptr,
    uint8   callid)
{
    if(callid >= MN_CALL_MAX_CALL_NUM)
    {
        ATC_TRACE_LOW("ATC: StopDTMFKey, call_id >= MN_CALL_MAX_CALL_NUM");
        return FALSE;
    }

    if(MN_RETURN_SUCCESS == MNCALL_StopDTMFEx(dual_sys, callid))
    {
        ATC_TRACE_LOW("ATC: MNCALL_StopDTMF, call id:%d.", callid);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                          (uint16)APP_MN_STOP_DTMF_CNF, callid, ATC_DOMAIN_CS, dual_sys);
    }
    else
    {
        ATC_TRACE_LOW("ATC: MNCALL_StopDTMF failed.");
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function get the dtmf key from ATC_CMD_PARAM_T
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDTMFKey(ATC_CMD_PARAM_T   *param_ptr,  //in
                         uint8      *dtmf_ptr                   //out:dtmf key
                        )
{
    if(NULL == param_ptr)
    {
        return FALSE;
    }

    if(param_ptr->param_len != param_ptr->param_data_len ||
            1 != param_ptr->param_len ||
            !CheckDtmfStr(1, param_ptr->param))
    {
        ATC_TRACE_LOW("ATC: GetDTMFKey, <dtmf> is not character or not in range.");
        return FALSE;
    }

    if(NULL != dtmf_ptr)
    {
        *dtmf_ptr = param_ptr->param[0];
    }

    ATC_TRACE_LOW("ATC: GetDTMFKey, %d.", param_ptr->param[0]);
    return TRUE;
}

/*****************************************************************************/
//  Description : This function get the dtmf keys from dtmf string
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDTMFString(uint8    *in_ptr,               //in: dtmf string
                            uint16  in_len,             //in: string length
                            uint8     *dtmf_ptr,              //out:dtmf keys
                            uint8     *count_ptr          //out:key count
                           )
{
    uint16  i;
    uint8   j = 0;

    if(NULL == in_ptr || in_len < 1 || NULL == dtmf_ptr)
    {
        return FALSE;
    }

    /*Format: <dtmf>,<dtmf>,<dtmf>....*/
    for(i = 0; i < in_len;)
    {
        dtmf_ptr[j] = in_ptr[i];
        j++;

        if(++i >= in_len)
        {
            break;
        }
        else
        {
            if(',' != in_ptr[i])
            {
                ATC_TRACE_LOW("ATC: <DTMF> string format wrong.");
                return FALSE;
            }

            i++;
        }
    }

    if(!CheckDtmfStr(j, dtmf_ptr))
    {
        return FALSE;
    }

    if(count_ptr)
    {
        *count_ptr = j;
    }

    ATC_TRACE_LOW("ATC: GetDTMFString, %s.", dtmf_ptr);
    return TRUE;
}


/*****************************************************************************/
//  Description : This function get the dtmf timer active or not
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_IsDTMFTimerActive(void)
{
    if(PNULL != s_dtmf_timer && SCI_IsTimerActive(s_dtmf_timer))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : This function reset dtmf timer
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ResetDTMFTimer(void)
{
    if(PNULL != s_dtmf_timer)
    {
        if(PNULL != s_dtmf_sig_ptr && SCI_IsTimerActive(s_dtmf_timer))
        {
            SCI_FREE(s_dtmf_sig_ptr);
        }

        SCI_DeleteTimer(s_dtmf_timer);
        s_dtmf_timer = PNULL;
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : This function reset the s_atc_dtmf_duration_t variable
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
void ATC_ResetDTMFVar(MN_DUAL_SYS_E   dual_sys)
{
    s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration = 0;
    
    s_atc_dtmf_duration_t[dual_sys].read_index = 0;

    s_atc_dtmf_duration_t[dual_sys].is_wait_stop_dtmf_cnf = FALSE;

    s_dtmf_send_at = 0;

    #ifndef ATC_SAT_ENABLE
    s_dtmf_char = '?';
    s_is_start_dtmf = FALSE;
    #endif
}

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
void  ATC_ProcessDTMFTimerCallback(
    uint32 lparam
)
{
    ATC_TRACE_LOW("ATC: enter the DTMF timer expire");
    SCI_ASSERT(PNULL != s_dtmf_timer);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != s_dtmf_sig_ptr);/*assert verified: check null pointer*/

    SCI_SEND_SIGNAL((xSignalHeaderRec *) s_dtmf_sig_ptr , P_ATC);
    SCI_DeactiveTimer(s_dtmf_timer);
}

/*****************************************************************************/
//  Description : This function handles the ATC_CALL_DTMF_EXP_IND signal
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessDTMFTimerExpInd(
    ATC_CONFIG_T         *atc_config_ptr,
    ATC_DTMF_TIMER_EXP_IND_T *sig_ptr
)
{
    if (!atc_config_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessDTMFTimerExpInd atc_config_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if (!sig_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessDTMFTimerExpInd sig_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    ATC_TRACE_LOW("ATC: ATC_ProcessDTMFTimerExpInd");

    if(FALSE == s_atc_dtmf_duration_t[sig_ptr->dual_sys].is_wait_stop_dtmf_cnf)
    {
        ATC_TRACE_LOW("ATC: no dtmf key to stop.");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(!StopDTMFKey(sig_ptr->dual_sys, atc_config_ptr, sig_ptr->call_id))
    {
        /*Stop dtmf key failed*/
        ATC_ResetDTMFVar(sig_ptr->dual_sys);
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle APP_MN_START_DTMF_CNF
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessStartDTMFCnf(
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    APP_MN_START_DTMF_CNF_T  *sig_ptr    // point to the APP_MN_START_DTMF_CNF signal
)
{
    MN_DUAL_SYS_E   dual_sys;
    if (!atc_config_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessStartDTMFCnf  atc_config_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if (!sig_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessStartDTMFCnf  sig_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    dual_sys = sig_ptr->dual_sys;
    
    ATC_TRACE_LOW("ATC:ATC_ProcessStartDTMFCnf,callid:%d,DTMF key:%c, AT cmd type:%d",
                  sig_ptr->call_id,  sig_ptr->dtmf_key, s_dtmf_send_at);

    switch(s_dtmf_send_at)
    {
    #ifndef ATC_SAT_ENABLE
        case AT_CMD_EVTS:
#if 0
            if(sig_ptr->req_is_accepted)
            {
                s_dtmf_send_at = 0;
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                ATC_ResetDTMFVar(dual_sys);
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, MN_CALL_ERR_NETWORK_REJECT);
            }
#endif
            break;

    #endif                
        case AT_CMD_VTS:

            if(sig_ptr->req_is_accepted)
            {
                uint16    duration = ATC_ReadDTMFDuration(dual_sys);

                if(duration)
                {
                    duration = duration * 100;
                }
                else
                {
                    duration = 50;
                }

                /*create ATC_CALL_DTMF_EXP_IND sig for stopping dtmf key*/
                s_dtmf_sig_ptr = NULL;
                SCI_CREATE_SIGNAL(s_dtmf_sig_ptr ,
                                  ATC_CALL_DTMF_EXP_IND ,
                                  sizeof(ATC_DTMF_TIMER_EXP_IND_T),
                                  P_ATC);

                /*create timer or active the existent timer*/
                if(PNULL == s_dtmf_timer)
                {
                    s_dtmf_timer = SCI_CreateTimer("DTMF Timer", ATC_ProcessDTMFTimerCallback,
                                                   0, duration, SCI_AUTO_ACTIVATE);
                }
                else
                {
                    SCI_ChangeTimer(s_dtmf_timer, ATC_ProcessDTMFTimerCallback, duration);
                    SCI_ActiveTimer(s_dtmf_timer);
                }

                if(PNULL == s_dtmf_timer || PNULL == s_dtmf_sig_ptr)
                {
                    if(PNULL != s_dtmf_timer)
                    {
                        ATC_TRACE_LOW("ATC: DTMF SIG WRONG.");
                        SCI_DeleteTimer(s_dtmf_timer);
                        s_dtmf_timer = PNULL;
                    }

                    if(PNULL != s_dtmf_sig_ptr)
                    {
                        ATC_TRACE_LOW("ATC: DTMF TIMER WRONG.");
                        SCI_FREE(s_dtmf_sig_ptr);
                    }

                    ATC_ResetDTMFVar(dual_sys);
                }
                else
                {
                    ((ATC_DTMF_TIMER_EXP_IND_T *)s_dtmf_sig_ptr)->call_id = sig_ptr->call_id;
                    ((ATC_DTMF_TIMER_EXP_IND_T *)s_dtmf_sig_ptr)->dual_sys = dual_sys;
                    s_atc_dtmf_duration_t[dual_sys].is_wait_stop_dtmf_cnf = TRUE;
                }
            }
            else
            {
                ATC_ResetDTMFTimer();
                ATC_ResetDTMFVar(dual_sys);
            }

            break;
        default:
            ATC_TRACE_LOW("ATC:ATC_ProcessStartDTMFCnf, invalid sender");
            break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle APP_MN_STOP_DTMF_CNF
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessStopDTMFCnf(
    ATC_CONFIG_T        *atc_config_ptr, // point to the ATC_CONFIG_T struct.
    APP_MN_STOP_DTMF_CNF_T  *sig_ptr    // point to the APP_MN_STOP_DTMF_CNF signal
)
{
    if (!atc_config_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessStopDTMFCnf  atc_config_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if (!sig_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessStopDTMFCnf  sig_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    ATC_TRACE_LOW("ATC:ATC_ProcessStopDTMFCnf,callid:%d,AT cmd type:%d",
                  sig_ptr->call_id, s_dtmf_send_at);

    switch(s_dtmf_send_at)
    {
    #ifndef ATC_SAT_ENABLE
        case AT_CMD_EVTS:
            ATC_ResetDTMFVar(sig_ptr->dual_sys);
#if 0
            if(sig_ptr->req_is_accepted)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, MN_CALL_ERR_NETWORK_REJECT);
            }
#endif
            break;
    #endif
        case AT_CMD_VTS:

            if(sig_ptr->req_is_accepted)
            {
                s_atc_dtmf_duration_t[sig_ptr->dual_sys].is_wait_stop_dtmf_cnf = FALSE;

                if(s_atc_dtmf_duration_t[sig_ptr->dual_sys].count_of_cur_duration == 0)
                {
                    ATC_ResetDTMFVar(sig_ptr->dual_sys);
                }
                else
                {
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      (uint16)APP_MN_START_DTMF_CNF, sig_ptr->call_id, ATC_DOMAIN_CS, sig_ptr->dual_sys);
                }
            }
            else
            {
                ATC_ResetDTMFTimer();
                ATC_ResetDTMFVar(sig_ptr->dual_sys);
            }

            break;
        default:
            ATC_TRACE_LOW("ATC:ATC_ProcessStopDTMFCnf, invalid sender");
            break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+VTS command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Refactoring by minqian.qian 2010.1.22 according to 27001 and CMCC EWalk specifications
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessVTS)
{
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    uint8                  dtmf_keys[AT_DTMF_BUF_LEN] = {0};
    uint8                  cmd_type;
    uint8                *temp_ptr;
    uint16                param_len;

    uint8                  call_id = 0;
    uint8                  call_index;
    uint16                dtmf_duration;
    uint8                  dtmf_count;

    BOOLEAN                     new_at_cmd_flag = FALSE;
    ATC_CMD_PARAM_T    vts_params[AT_VTS_MAX_PARAMS_NUM] = {0};   //params buffer
    uint8                            param_count = 0;
    uint16                          param_offset;        //the length parsed for param
    MN_DUAL_SYS_E         dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    param_len = (uint16)PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;

    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            temp_ptr++;
            param_len--;

            //check call state
            if(0 == g_calls_state[dual_sys].call_amount)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessVTS: no active call");
                return ERR_NOT_FOUND;
            }

            if(!CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id) &&
                    !CheckPresentOutgoingCall(&g_calls_state[dual_sys], &call_id))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessVTS: no active call found");
                return ERR_NOT_FOUND;
            }

            ATC_TRACE_LOW("ATC: ATC_ProcessVTS: call-id:%d", call_id);

            call_index = GetIndexOfCallContext(call_id, dual_sys);

            if(ATC_INVALID_CALLID == call_index  ||
                    g_calls_state[dual_sys].call_context[call_index].call_mode != ATC_VOICE_MODE)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessVTS: the mode of the call is not voice");
                return ERR_NOT_FOUND;
            }


            //get params
            if(ATC_GetCommandParams(temp_ptr, param_len, AT_VTS_MAX_PARAMS_NUM,
                                    vts_params, &param_count, &param_offset, &new_at_cmd_flag))
            {
                ATC_CMD_PARAM_T    *PARAM_1_PTR = NULL;
                ATC_CMD_PARAM_T    *PARAM_2_PTR = NULL;
                uint8 i;

                if (param_count > AT_VTS_MAX_PARAMS_NUM)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessVTS  param_count %d exceeds %d(AT_VTS_MAX_PARAMS_NUM)",
                            param_count, AT_VTS_MAX_PARAMS_NUM);
                    return ERR_OPERATION_NOT_ALLOWED;
                }
                PARAM_1_PTR = &vts_params[0];
                PARAM_2_PTR = &vts_params[1];

                //SCI_MEMSET(dtmf_buf,0,AT_DTMF_BUF_LEN);
                if(2 == param_count)
                {
                    /*AT+VTS=<dtmf>[,<duration>]*/
                    /*DTMF check*/
                    if(!GetDTMFKey(PARAM_1_PTR, &dtmf_keys[0]))
                    {
                        return ERR_INVALID_INDEX;
                    }

                    /*duration check*/
                    if(0 < PARAM_2_PTR->param_len)
                    {
                        if(PARAM_2_PTR->param_data_len != PARAM_2_PTR->param_len)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessVTS: <duration> param error. not numeric.");
                            return ERR_INVALID_INDEX;
                        }

                        if(!ConvertStringtoUint16(PARAM_2_PTR->param, PARAM_2_PTR->param_len, &dtmf_duration))
                        {
                            return ERR_INVALID_INDEX;
                        }

                        if(dtmf_duration > 255)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessVTS: <duration> not in range (0-255).");
                            return ERR_INVALID_INDEX;
                        }
                    }
                    else
                    {
                        dtmf_duration = atc_config_ptr->vtd_value;
                    }
                    dtmf_count = 1;                                        
                }
                else if(1 == param_count)
                {
                    if(PARAM_1_PTR->param_len == (PARAM_1_PTR->param_data_len + 2))
                    {
                        /*AT+VTS="<dtmf>,<dtmf>,<dtmf>...."*/
                        if(PARAM_1_PTR->param_data_len > MAX_DTMF_STRING_LEN)
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessVTS: too long dtmf string.");
                            return ERR_INVALID_INDEX;
                        }

                        //make sure the len of dtmf_buf must biger than MAX_DTMF_STRING_LEN / 2
                        if(!GetDTMFString(PARAM_1_PTR->param,
                                          PARAM_1_PTR->param_data_len,
                                          dtmf_keys,
                                          &dtmf_count))
                        {
                            return ERR_INVALID_INDEX;
                        }

                        dtmf_duration = atc_config_ptr->vtd_value;
                    }
                    else if(PARAM_1_PTR->param_len == PARAM_1_PTR->param_data_len && 1 == PARAM_1_PTR->param_len)
                    {
                        /*AT+VTS=<dtmf>*/

                        /*DTMF check*/
                        if(!GetDTMFKey(PARAM_1_PTR, &dtmf_keys[0]))
                        {
                            return ERR_INVALID_INDEX;
                        }

                        dtmf_duration = atc_config_ptr->vtd_value;
                        dtmf_count = 1;
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessVTS: <DTMF> param set error.");
                        return ERR_INVALID_INDEX;
                    }
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessVTS: params not supported.");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                s_dtmf_send_at = AT_CMD_VTS;

                //s_atc_dtmf.cur_key_p = 0;
                ATC_TRACE_LOW("ATC: send dtmf:%s,duration:%d*100ms", (char *)dtmf_keys, dtmf_duration);

                for(i = 0; i < dtmf_count; ++i)
                {
                    if(!ATC_IsDTMFDurationListFull(dual_sys))
                    {
                        if(MN_RETURN_SUCCESS != MNCALL_StartDTMFEx(dual_sys, dtmf_keys[i], call_id))
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessVTS, MNCALL_StartDTMF failed.");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                        else
                        {
                            if(0 == i)
                            {
                                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              (uint16)APP_MN_START_DTMF_CNF, call_id, ATC_DOMAIN_CS, dual_sys);
                            }
                            ATC_WriteDTMFDuration(dual_sys, dtmf_duration);
                        }
                    }
                }

                if (dtmf_count > 0)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
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

            if(new_at_cmd_flag)
            {
                temp_ptr += param_offset;
                param_len -= param_offset;

                if(param_len > 1)
                {
                    uint8          *new_at_ptr;

                    new_at_ptr  = PARA1.str_ptr->str_ptr;
                    SCI_MEMCPY(new_at_ptr, "AT", 2);
                    SCI_MEMCPY((new_at_ptr + 2), temp_ptr, param_len);
                    param_len += 2;
                    new_at_ptr[param_len] = atc_config_ptr->s3_reg;
                    param_len++;
#ifdef _MUX_ENABLE_
                    ATC_SendNewATInd(atc_config_ptr->current_link_id, param_len, new_at_ptr);
#else
                    SendNewATInd(param_len);
#endif
                    status = S_ATC_DEFAULT_HANDLE;
                }
            }

            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "(0-9,*,#,A,B,C,D)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handle the AT+EVTS command (OMS feature)
//  Global resource dependence : none
//  Author:       Yi.Jin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessEVTS)
{
    ATC_STATUS      status      = S_ATC_DEFAULT_HANDLE;
    uint8                  cmd_type;
    uint8                *temp_ptr;
    uint16                param_len;

    uint8                  call_id     = 0;
    uint8                    call_index;

    ATC_CMD_PARAM_T  vts_params[AT_EVTS_MAX_PARAMS_NUM] = {0};  //params buffer
    BOOLEAN                  new_at_cmd_flag = FALSE;
    uint16                       mode_param = 0;        //<mode> param  value of +EVTS cmd
    uint8                            dtmf_param;        //<dtmf> param  value of +EVTS cmd
    uint16                       param_offset;      //the length parsed for param
    uint8                            param_count = 0;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(!PARAM1_FLAG || ATC_GET_CMD_TYPE != ATC_CMD_TYPE_EXECUTE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    param_len = (uint16)PARA1.str_ptr->str_len;
    temp_ptr  = PARA1.str_ptr->str_ptr;

    ATC_GetCmdType(temp_ptr, param_len, &cmd_type);

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
            temp_ptr++;
            param_len--;

            //check call state
            if(0 == g_calls_state[dual_sys].call_amount)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: no active call");
                return ERR_NOT_FOUND;
            }

            if(!CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id) &&
                    !CheckPresentOutgoingCall(&g_calls_state[dual_sys], &call_id))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: no active call found");
                return ERR_NOT_FOUND;
            }

            call_index = GetIndexOfCallContext(call_id, dual_sys);

            if(ATC_INVALID_CALLID == call_index  ||
                    g_calls_state[dual_sys].call_context[call_index].call_mode != ATC_VOICE_MODE)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: the call:%d is not voice mode", call_id);
                return ERR_NOT_FOUND;
            }

            ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: call-id:%d", call_id);

            if(s_dtmf_send_at == AT_CMD_VTS)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: AT+VTS has not done.");
                return ERR_OPERATION_NOT_ALLOWED;
            }

            //get params
            if(ATC_GetCommandParams(temp_ptr, param_len, AT_EVTS_MAX_PARAMS_NUM,
                                    vts_params, &param_count, &param_offset, &new_at_cmd_flag))
            {
                ATC_CMD_PARAM_T *PARAM_1_PTR = NULL;
                ATC_CMD_PARAM_T *PARAM_2_PTR = NULL;

                if (param_count > AT_EVTS_MAX_PARAMS_NUM)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessEVTS  param_count %d exceeds %d(AT_EVTS_MAX_PARAMS_NUM)",
                                         param_count, AT_EVTS_MAX_PARAMS_NUM);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                PARAM_1_PTR = &vts_params[0];
                PARAM_2_PTR = &vts_params[1];

                /*<mode> check*/
                if(PARAM_1_PTR->param_len != PARAM_1_PTR->param_data_len || 1 != PARAM_1_PTR->param_len)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: <mode> param error. not numeric.");
                    return ERR_INVALID_INDEX;
                }

                if(!ConvertStringtoUint16(PARAM_1_PTR->param, PARAM_1_PTR->param_data_len, &mode_param))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: <mode> set error");
                    return ERR_INVALID_INDEX;
                }

                /*<DTMF> check*/
                if(0 == PARAM_2_PTR->param_len)
                {
                    //dtmf not set
                    dtmf_param = s_dtmf_char;

                    if(!CheckDtmfStr(1, &dtmf_param))
                    {
                        return ERR_INVALID_INDEX;
                    }
                }
                else
                {
                    if(!GetDTMFKey(PARAM_2_PTR, &dtmf_param))
                    {
                        return ERR_INVALID_INDEX;
                    }
                }

                if(0 == mode_param)
                {
                    //stop the dtmf which has been started
                    if(s_is_start_dtmf && (s_dtmf_char == dtmf_param))
                    {
                        if(StopDTMFKey(dual_sys, atc_config_ptr, call_id))
                        {
                            s_is_start_dtmf = FALSE;
                            s_dtmf_char = '?';
                            s_dtmf_send_at = AT_CMD_EVTS;
                            ATC_TRACE_LOW("ATC: StopDTMF = %c", dtmf_param);
                            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                            ATC_ResetDTMFVar(dual_sys);
                        }
                        else
                        {
                            ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: MNCALL_StopDTMF failed.");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: WARN:%d,%c", s_is_start_dtmf, s_dtmf_char);
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }
                else if(1 == mode_param)
                {
                    //start the dtmf
                    if(!s_is_start_dtmf)
                    {
                        //ATC_ResetDTMFVar();
                        if(StartDTMFKey(dual_sys, atc_config_ptr, dtmf_param, call_id))
                        {
                            s_dtmf_send_at = AT_CMD_EVTS;
                            s_is_start_dtmf = TRUE;
                            s_dtmf_char = dtmf_param;
                            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                        }
                        else
                        {
                            return ERR_OPERATION_NOT_ALLOWED;
                        }
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: DTMF is already started, pls stop firstly.");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessEVTS: <mode> set error");
                    return ERR_INVALID_INDEX;
                }
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            //send the rest AT Cmd
            if(new_at_cmd_flag)
            {
                temp_ptr += param_offset;
                param_len -= param_offset;

                if(param_len > 1)
                {
                    uint8          *new_at_ptr;

                    new_at_ptr  = PARA1.str_ptr->str_ptr;
                    SCI_MEMCPY(new_at_ptr, "AT", 2);
                    SCI_MEMCPY((new_at_ptr + 2), temp_ptr, param_len);
                    param_len += 2;
                    new_at_ptr[param_len] = atc_config_ptr->s3_reg;
                    param_len++;

#ifdef _MUX_ENABLE_
                    ATC_SendNewATInd(atc_config_ptr->current_link_id, param_len, new_at_ptr);
#else
                    SendNewATInd(param_len);
#endif

                    status = S_ATC_DEFAULT_HANDLE;
                }
            }

            break;
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "(0,1),(0-9,*,#,A,B,C,D)");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        case ATC_CMD_TYPE_READ:
        {
            if(s_is_start_dtmf)
            {
                sprintf((char *)g_rsp_str, "+evts: %d, %d", mode_param, (uint8)(s_dtmf_char));
            } 
            else
            {
                sprintf((char *)g_rsp_str, "+evts: %d", mode_param);
            }
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        }
        default:
            return   ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+CPLS command
//  Global resource dependence : none
//  Author:       sunny
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPLS)
{
    ATC_STATUS              status = S_ATC_SUCCESS;
#ifndef _ULTRA_LOW_CODE_

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                switch(PARA1.num)
                {
                    case USER_CONTROLLED_PLMN:
                        s_plmn_list_type = USER_CONTROLLED_PLMN;
                        break;

                    case OPERATOR_CONTROLLED_PLMN:
                        s_plmn_list_type = OPERATOR_CONTROLLED_PLMN;
                        break;

                    case HPLMN:
                        s_plmn_list_type = HPLMN;
                        break;
                    default:
                        status = ERR_INVALID_INDEX;
                        ATC_TRACE_LOW("ATC: plmn list type is invaild");
                        break;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_CPLS].cmd_name->str_ptr,
                    s_plmn_list_type);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0-2)",
                    g_atc_info_table[AT_CMD_CPLS].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

#endif /* _ULTRA_LOW_CODE_ */
    return status;
}


/*****************************************************************************/
/*
//  Description : This function handle the APP_MN_SETUP_COMPLETE_IND signal and return
//                the unsolicited result code.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
需求: 运营商代码和当前网络状态：
使用 AT *PSNTRG=1 命令配置 , 配置之后网络状态变化时会上报：
*PSNTRG: <Registration state>, <GPRSstate>, <MCC>, <MNC>, <LAC>, <CI>,<PLMN Name>, [<Band indication>], [<Rat>],[<EGPRS state>]
<registration state>
0 Not registered
1 Registered, home PLMN
2 Not registered but searching (registration ongoing)
3 Registration denied
4 Unknown
5 Registered, roaming
6 Limited service (emergency)
<GPRS state>
0 No GPRS available on cell
1 GPRS available on cell and MS attached
2 GPRS available on cell but MS not attached
3 GPRS suspended
<mcc>
String type Mobile country code in numeric format (e.g "208")
<mnc>
String type Mobile network code in numeric format (e.g "10")
<lac>
String type Two byte location area code in hexadecimal format (e.g"3FA2")
<ci>
String type Two byte cell ID in hexadecimal format (e.g "6CA5")
<PLMN name>
String type Current PLMN Name in long alphanumeric format
<Band indication>
0 GSM 900
1 E-GSM 900
2 DCS 1800
3 DCS 1900
4 UMTS band
<Rat> Description
0 GSM
1 UMTS
<EGPRS state> Description
0 EGPRS service not available on cell
1 EGPRS service available on cell but MS not GPRS attached
2 EGPRS service available on cell
*/
/*****************************************************************************/

void BuildPSNTRGResponse(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T   *atc_config_ptr ,
    const MN_PHONE_CURRENT_PLMN_INFO_T    *register_status_ptr)
{
    uint8                  rat = 0;
    uint8                  gprs_state = 0;
    uint8                  egprs = 0;
    uint8                   band = 0;
    MN_PLMN_T               plmn = {0};
    unsigned char           *operator_name;
    OPER_STATUS_E                  oper_status;
    uint8                         oper_str[6] = {0};
    const ATC_PLMN_DETAILED_INFO_T *plmn_detailed_info = NULL;

    switch(register_status_ptr->plmn_status)
    {
        case PLMN_REGISTER_SERVICE:
            oper_status = NOT_REGISTERED_AND_SEARCHING;
            break;
        case PLMN_NO_SERVICE:
            oper_status = NOT_REGISTERED_NOT_SEARCHING;
            break;

        case PLMN_EMERGENCY_ONLY:
        case PLMN_EMERGENCY_GPRS_ONLY:

            if(MANUAL_DEREGISTER == atc_config_ptr->cops[dual_sys].mode)
            {
                oper_status = NOT_REGISTERED_NOT_SEARCHING;
            }
            else
            {
                oper_status = REGISTRATION_DENIED;
            }

            break;

        case PLMN_NORMAL_GSM_ONLY:

            if(register_status_ptr->plmn_is_roaming)
            {
                oper_status = REGISTERED_ROAMING;
            }
            else
            {
                oper_status = REGISTERED_HOME_PLMN;
            }

            gprs_state = 0;
            break;

        case PLMN_NORMAL_GSM_GPRS_BOTH:

            if(register_status_ptr->plmn_is_roaming)
            {
                oper_status = REGISTERED_ROAMING;
            }
            else
            {
                oper_status = REGISTERED_HOME_PLMN;
            }

            gprs_state = 1;
            break;

        case PLMN_NORMAL_GPRS_ONLY:
        case PLMN_REGISTER_GPRS_ONLY:
            oper_status = NOT_REGISTERED_AND_SEARCHING;
            gprs_state = 1;
            break;
        default:
            oper_status = UNKNOWN;
            gprs_state = 0;
            break;
    }

    /*GPRS status, postpone*/
    plmn.mnc = register_status_ptr->mnc;
    plmn.mcc = register_status_ptr->mcc;
    plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_MNC_MCC, (unsigned char *)&plmn);

    if(plmn_detailed_info)
    {
        operator_name = (unsigned char *)plmn_detailed_info->full_operator_name;
    }
    else
    {
        TransformPlmnToNumeric(&plmn, oper_str);
        operator_name = (unsigned char *)oper_str;
    }

    band = 0;

    /*rat*/
    switch(register_status_ptr->rat)
    {
        case MN_GMMREG_RAT_GPRS:
            rat = 0;
            break;
        case MN_GMMREG_RAT_3G:
        case MN_GMMREG_RAT_GPRS_AND_3G:
            rat = 1;
            break;
        default:
            rat = 2;
            break;
    }

    /*EGPRS state is postponed*/
    if(register_status_ptr->cell_capability.edge_support)
    {
        switch(gprs_state)
        {
            case 0:
                egprs = 0;
                break;
            case 1:
                egprs = 2;
                break;
            case 2:
                egprs = 1;
                break;
            case 3:
                egprs = 2;
                break;
            default:
                break;
        }
    }
    else
    {
        egprs = 0;
    }

    sprintf((char *)g_rsp_str, "*PSNTRG: %d,%d,\"%d\",\"%02d\",\"%04X\",\"%04X\",%s,%d,%d,%d",
            oper_status, gprs_state,
            register_status_ptr->mnc,
            register_status_ptr->mcc,
            register_status_ptr->lac,
            register_status_ptr->cell_id,
            operator_name,
            band,
            rat,
            egprs);
    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, (uint8 *)g_rsp_str);

}

/*****************************************************************************/
//  Description : This function handle the AT^DSCI command
//  Global resource dependence : none
//  Author:       peng.chen
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processstar_PSNTRG)
{
    MN_PHONE_CURRENT_PLMN_INFO_T   register_status;
    ATC_STATUS         status = S_ATC_SUCCESS;

    ATC_TRACE_LOW("ATC:ATC_Processstar_PSNTRG");

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(!PARAM1_FLAG)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(1 == PARA1.num)
    {
        atc_config_ptr->psntrg_flag = TRUE;
    }
    else
    {
        atc_config_ptr->psntrg_flag = FALSE;
        return status;
    }

    register_status = MNPHONE_GetCurrentPLMNInfoEx(ATC_CURRENT_SIM_ID_MUX);
    ATC_TRACE_LOW("ATC: ATC_Processstar_PSNTRG, plmn_status:%d,rat:%d,cell_cap:%d%d%d%d",
                  register_status.plmn_status,
                  register_status.rat,
                  register_status.cell_capability.mbms_support,
                  register_status.cell_capability.hsdpa_support,
                  register_status.cell_capability.hsupa_support,
                  register_status.cell_capability.edge_support);

    BuildPSNTRGResponse(ATC_CURRENT_SIM_ID_MUX, atc_config_ptr, &register_status);
    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+CMOD command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMOD)
{
    ATC_STATUS     status = S_ATC_SUCCESS;

    SCI_TRACE_LOW("ATC: ATC_ProcessCMOD");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
      #ifdef _ATC_WAIT_L4
            if(PARAM1_FLAG)
            {
                if(MN_CALL_VOICE_THEN_DATA >= PARA1.num)
                {
                    atc_config_ptr->cmod = PARA1.num;
                }
                else
                {
                    status = ERR_INVALID_INDEX;
                }
            }
            else
            {
                atc_config_ptr->cmod = MN_CALL_SINGLE_MODE;
            }
            #endif

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_CMOD].cmd_name->str_ptr, atc_config_ptr->cmod);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0-3)", g_atc_info_table[AT_CMD_CMOD].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+CBST command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCBST)
{
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : This function handles the ATDL command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessDL)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ATC_TRACE_LOW("ATC: ATC_ProcessDL");

    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        if(g_last_mo_call.called_num_present)
        {
            if(MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys, g_last_mo_call.call_type, &g_last_mo_call.called_num, PNULL, g_last_mo_call.call_clir, PNULL))
            {
                g_atc_call_flag = TRUE;
                g_atc_call_need_disc = FALSE;    //reset the neee_disc flag
                atc_config_ptr->cpas = ATC_CPAS_STATUS_PROGRESS;

                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                 (uint16)APP_MN_CALL_START_IND, 0xff, ATC_DOMAIN_CS, dual_sys);

                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                ATC_TRACE_LOW("ATC: failure in MNCALL_StartCall");
                return ERR_INVALID_INDEX;
            }
        }
        else
        {
            ATC_TRACE_LOW("ATC: no call number recently.");
            return ERR_INVALID_INDEX;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

}

/*****************************************************************************/
//  Description : This function handles the AT+CHUP command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHUP)
{
    MN_CALL_DISCONNECT_CALL_CAUSE_E cause = MN_CAUSE_NORMAL_CLEARING;
    ATC_CALLS_STATE_T *    calls_state_ptr = PNULL;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32         i;

    ATC_TRACE_LOW("ATC: ATC_ProcessCHUP\n");

    if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        return S_ATC_SUCCESS;
    }

    if(ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(SCI_IS_TIMER_ACTIVE(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND,
                       &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
        g_atc_expire_call_id = 0xff;
        g_atc_expire_ind = FALSE;
    }

    if(g_atc_call_flag)
    {
        g_atc_call_need_disc = TRUE;//assume there is a pending MO CALL
    }

    ATC_TRACE_LOW("ATC: g_calls_state.call_amout= %d", g_calls_state[dual_sys].call_amount);

    calls_state_ptr = &g_calls_state[dual_sys];

    if(calls_state_ptr->call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;

        //Release all the call
        for(i = 0; i < calls_state_ptr->call_amount; i++)
        {
            if (ATC_CALL_ALERTING_STATE == calls_state_ptr->call_context[i].call_entity_state)
            {
                cause = MN_CAUSE_USER_BUSY;
            }

            MNCALL_DisconnectCallEx(dual_sys, calls_state_ptr->call_context[i].call_id, cause, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_CALL_DISCONNECTED_IND,
                                  calls_state_ptr->call_context[i].call_id, ATC_DOMAIN_CS, dual_sys);
        }

        return S_ATC_DEFAULT_HANDLE;
    }
    else
    {
        if(g_atc_call_flag)
        {
            return S_ATC_DEFAULT_HANDLE;//do nothing if there is a pending MO CALL
        }

        return S_ATC_SUCCESS;
    }

}

/*****************************************************************************/
//  Description : This function handles the AT+TRACE command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessTRACE)
{
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : This function handles the AT+OFF command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessOFF)
{
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : This function handles the AT+RESET command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessRESET)
{
    ATC_STATUS status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: ATC_ProcessRESET");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                ATC_TRACE_LOW("ATC: no parameter 1");
                return ERR_INVALID_INDEX;
            }

            if(PARA1.num == 1)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                POWER_Reset();
                status = S_ATC_SUCCESS;
            }
            else
            {
                if(atc_config_ptr->reset_flag[dual_sys] == TRUE)
                {
                    ATC_TRACE_LOW("ATC: Previous Reset is in progress.");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                atc_config_ptr->reset_flag[dual_sys] = TRUE;
                g_is_ps_act[dual_sys]   = FALSE;
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_DEACT_PS;

                MNPHONE_PowerOffPsEx(dual_sys);
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_PS_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                status = S_ATC_DEFAULT_HANDLE;
            }

            break;

        case ATC_CMD_TYPE_EXECUTE:
        case ATC_CMD_TYPE_TEST:
        case ATC_CMD_TYPE_READ:
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPAUTO command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPAUTO)
{
    ATC_STATUS  status = S_ATC_SUCCESS;
    ATC_TRACE_LOW("ATC: ATC_ProcessRESET");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                return ERR_INVALID_INDEX;
            }

            atc_config_ptr->atc_config_nv.spauto_flag = PARA1.num;
            if(!ATC_WriteNVItem(atc_config_ptr, NV_ATC_CONFIG_ID))
            {
                return S_ATC_FAIL;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPAUTO].cmd_name->str_ptr,
                    atc_config_ptr->atc_config_nv.spauto_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0,1)", g_atc_info_table[AT_CMD_SPAUTO].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
}


/*****************************************************************************/
//  Description : This function handles the AT+CHUPVT command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCHUPVT)
{
    ATC_CALLS_STATE_T * calls_state_ptr = PNULL;
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause = MN_CAUSE_NORMAL_CLEARING;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32         i;

    ATC_TRACE_LOW("ATC: ATC_ProcessCHUPVT");

    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCHUPVT, NOT set command!");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(!PARAM1_FLAG)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCHUPVT, no param1");
        return ERR_INVALID_INDEX;
    }

    if(SCI_IS_TIMER_ACTIVE(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND,
                       &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
        g_atc_expire_call_id = 0xff;
        g_atc_expire_ind = FALSE;
    }

    if(g_atc_call_flag)
    {
        g_atc_call_need_disc = TRUE;//assume there is a pending MO CALL
    }

    ATC_TRACE_LOW("ATC: g_calls_state.call_amout= %d", g_calls_state[dual_sys].call_amount);

    calls_state_ptr = &g_calls_state[dual_sys];

    if(calls_state_ptr->call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;

        if((PARAM1_FLAG) && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessCHUPVT, cause=%d", PARA1.num);
            cause = (MN_CALL_DISCONNECT_CALL_CAUSE_E)PARA1.num;
        }

        //Release all the call
        for(i = 0; i < calls_state_ptr->call_amount; i++)
        {
            if(!PARAM1_FLAG &&
               ATC_CALL_ALERTING_STATE == calls_state_ptr->call_context[i].call_entity_state)
            {
                cause = MN_CAUSE_USER_BUSY;
            }
        

            MNCALL_DisconnectCallEx(dual_sys, calls_state_ptr->call_context[i].call_id, cause, PNULL);

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, APP_MN_CALL_DISCONNECTED_IND,
                                  calls_state_ptr->call_context[i].call_id, ATC_DOMAIN_CS, dual_sys);
        }

        return S_ATC_DEFAULT_HANDLE;
    }
    else
    {
        if(g_atc_call_flag)
        {
            return S_ATC_DEFAULT_HANDLE;//do nothing if there is a pending MO CALL
        }

        return S_ATC_SUCCESS;
    }

}


/*****************************************************************************/
//  Description : This function handles the AT+CSTA command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSTA)
{
    uint32   status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atset_number_type = (ATC_TELE_TYPE_E)PARA1.num;
            }
            else
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "+CSTA:%d", atset_number_type);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+S32K command, This command is used
//                to set the sleep mode.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessS32K)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    BOOLEAN       sleep_mode;

#ifndef _ULTRA_LOW_CODE_

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            sleep_mode = PARA1.num;
#ifdef _MODEM_MODE
            s_Is_SleepAllowed = PARA1.num;
#endif
            ATC_TRACE_LOW("ATC_ProcessS32K sleep_mode = %d", sleep_mode);
            //set the sleep mode
            SCI_ATC_SetSleepSwitchFlag(sleep_mode);
            break;

        case ATC_CMD_TYPE_READ:
            //read the sleep mode
            sleep_mode = SCI_ATC_GetSleepSwitchFlag();
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_S32K].cmd_name->str_ptr, sleep_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SSMP command, This command is used
//                to send the singal with max power.
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSMP)
{
    ATC_STATUS  at_result = S_ATC_SUCCESS;

#ifdef _ULTRA_LOW_CODE_
    at_result = S_ATC_FAIL;
#else

    //SCI_TRACE_LOW("ATC: enter ATC_ProcessSSMP");
    if(L1API_ApplyMaxPower())
    {
        at_result = S_ATC_SUCCESS;
    }
    else
    {
        at_result = ERR_OPERATION_NOT_ALLOWED;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return (at_result);
}

#endif

/*****************************************************************************/
//  Description : This function handle the AT+COPS command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCOPS)
{
    const ATC_PLMN_DETAILED_INFO_T  *plmn_detailed_info = NULL;
    ATC_STATUS                      status              = S_ATC_DEFAULT_HANDLE;
    ERR_MNPHONE_CODE_E              err_code            = ERR_MNPHONE_NO_ERR;
    MN_PLMN_T                       plmn;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&plmn, 0, sizeof(plmn));

#ifndef _ULTRA_LOW_CODE_

    //If there is calls,return error
    if((ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE) && (PARAM1_FLAG) && (PARA1.num == SET_ONLY_FORMAT))
    {
        //action should be performed
    }
    else
    {
        if((g_calls_state[dual_sys].call_amount >= 1) &&
                (ATC_CMD_TYPE_READ != ATC_GET_CMD_TYPE))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessCOPS, not allowed when CALL is existed.");
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                if(atc_config_ptr->sim_flag[dual_sys]
                        && ATC_CPIN_CODE_READY != atc_config_ptr->cpin[dual_sys])
                {
                    if( ATC_CPIN_CODE_PIN2!=atc_config_ptr->cpin[dual_sys] 
                    && ATC_CPIN_CODE_PUK2!=atc_config_ptr->cpin[dual_sys])  //when PIN2 blocked, let it fall through
                    {
                        SCI_TRACE_LOW("ATC: ATC_ProcessCOPS, sim is not ready or cpin code is not ready.");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }
                }

#ifdef _MUX_ENABLE_

                if(ATC_GetPending_Event(atc_config_ptr, APP_MN_SIM_POWER_ON_CNF, ATC_INVALID_LINK_ID, dual_sys) ||
                        ATC_GetPending_Event(atc_config_ptr, APP_MN_PS_POWER_ON_CNF,  ATC_INVALID_LINK_ID, dual_sys) ||
                        ATC_GetPending_Event(atc_config_ptr, APP_MN_SIM_POWER_OFF_CNF, ATC_INVALID_LINK_ID, dual_sys) ||
                        ATC_GetPending_Event(atc_config_ptr, APP_MN_PS_POWER_OFF_CNF, ATC_INVALID_LINK_ID, dual_sys))
                {
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                if(ATC_GetPending_Event(atc_config_ptr, APP_MN_DETACH_GSM_CNF, ATC_INVALID_LINK_ID, dual_sys) &&
                        (PARA1.num == MANUAL_DEREGISTER ))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCOPS, APP_MN_DETACH_GSM_CNF is being handled.");
                    return ERR_OPERATION_NOT_ALLOWED;
                }
				
                if((ATC_GetPending_Event(atc_config_ptr, APP_MN_PLMN_SELECT_CNF, ATC_INVALID_LINK_ID, dual_sys) &&
                        (PARA1.num ==  AUTO_MODE || PARA1.num  == MANUAL_MODE || PARA1.num  == MANUAL_MATO_MODE))||
						ATC_GetPending_Event(atc_config_ptr, APP_MN_DETACH_GSM_CNF, ATC_INVALID_LINK_ID, dual_sys))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCOPS, APP_MN_PLMN_SELECT_CNF is being handled.");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

#endif

                switch(PARA1.num)
                {
                    case AUTO_MODE:
                        // set the auto select mode

                        //CG_WHD add for cops=0(reset the service type) b
                        if(atc_config_ptr->cops[dual_sys].mode == MANUAL_DEREGISTER)
                        {
                            if(ERR_MNPHONE_NO_ERR != MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_GPRS_BOTH))
                            {
                                SCI_TRACE_LOW("ATC: COPS 0 MNPHONE_SetServiceTypeEx failure line:%d",__LINE__);
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }

                        err_code = MNPHONE_SelectPLMNEx(dual_sys, FALSE, plmn, MN_GMMREG_RAT_GPRS);

                        if(err_code == ERR_MNPHONE_NO_ERR)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              (uint16)APP_MN_PLMN_SELECT_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                        }
                        else
                        {
                            ATC_TRACE_LOW("ATC: MNPHONE_SelectPLMN failure");
                            return ERR_UNKNOWN;
                        }

                        atc_config_ptr->cops[dual_sys].mode = AUTO_MODE;
                        if(atc_config_ptr->async_mode_flag)
                        {
                            return S_ATC_SUCCESS;
                        }
                        break;

                    case MANUAL_MODE:

                        if(!PARAM2_FLAG || !PARAM3_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                        if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(atc_config_ptr->cops[dual_sys].mode == MANUAL_DEREGISTER)
                        {
                            if(ERR_MNPHONE_NO_ERR != MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_GPRS_BOTH))
                            {
                                SCI_TRACE_LOW("ATC: COPS 1 MNPHONE_SetServiceTypeEx failure line:%d",__LINE__);
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }

                        //manual select PLMN
                        if(PARAM4_FLAG)
                        {
                            status = ManualSelectPlmn(dual_sys, atc_config_ptr, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, PARA4.num);
                        }
                        else
                        {
                            status = ManualSelectPlmn(dual_sys, atc_config_ptr, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, MN_GMMREG_RAT_UNKNOWN);
                        }

                        if(S_ATC_SUCCESS == status)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              (uint16)APP_MN_PLMN_SELECT_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            atc_config_ptr->cops[dual_sys].mode = MANUAL_MODE;
                            status = S_ATC_DEFAULT_HANDLE;
                        }

                        break;

                    case MANUAL_DEREGISTER:
                    {
                        MN_PHONE_CURRENT_PLMN_INFO_T       register_status = {0};
                        register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

                        if(ERR_MNPHONE_NO_ERR != MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_NONE))
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        SCI_MEMSET((void *)&g_cur_plmn_info[dual_sys], 0, sizeof(ATC_PLMN_INFO_T));

                        if((MANUAL_DEREGISTER == atc_config_ptr->cops[dual_sys].mode) ||
                            ((PLMN_NORMAL_GSM_ONLY !=register_status.plmn_status) && (PLMN_NORMAL_GSM_GPRS_BOTH !=register_status.plmn_status)))
                        {
                            SCI_TRACE_LOW("ATC: MANUAL_DEREGISTER  mode=%d,register_status.plmn_status=%d",atc_config_ptr->cops[dual_sys].mode, register_status.plmn_status);
                            atc_config_ptr->cops[dual_sys].mode = MANUAL_DEREGISTER;
                            status = S_ATC_SUCCESS;
                        }
                        else
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                                              (uint16)APP_MN_DETACH_GSM_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            atc_config_ptr->cops[dual_sys].mode = MANUAL_DEREGISTER;
                            status = S_ATC_DEFAULT_HANDLE;
                        }
                    }
                        break;

                    case SET_ONLY_FORMAT:

                        if(!PARAM2_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                        }
                        else
                        {
                            atc_config_ptr->cops[dual_sys].format = (ATC_PLMN_FORMAT_E)PARA2.num;
                            status = S_ATC_SUCCESS;
                        }

                        break;

                    case MANUAL_MATO_MODE:

                        if(!PARAM2_FLAG || !PARAM3_FLAG)
                        {
                            status = ERR_INVALID_INDEX;
                            break;
                        }

                        if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(atc_config_ptr->cops[dual_sys].mode == MANUAL_DEREGISTER)
                        {
                            if(ERR_MNPHONE_NO_ERR != MNPHONE_SetServiceTypeEx(dual_sys, MN_PHONE_USER_SELECT_GSM_GPRS_BOTH))
                            {
                                SCI_TRACE_LOW("ATC: COPS 4 MNPHONE_SetServiceTypeEx failure line:%d",__LINE__);
                                return ERR_OPERATION_NOT_SUPPORTED;
                            }
                        }

                        //manual select PLMN at the first
                        if(PARAM4_FLAG)
                        {
                            status = ManualSelectPlmn(dual_sys, atc_config_ptr, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, PARA4.num);
                        }
                        else
                        {
                            status = ManualSelectPlmn(dual_sys, atc_config_ptr, (ATC_PLMN_FORMAT_E)PARA2.num, PARA3.str_ptr, MN_GMMREG_RAT_UNKNOWN);
                        }

                        if(S_ATC_SUCCESS == status)
                        {
                            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                              (uint16)APP_MN_PLMN_SELECT_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                            atc_config_ptr->cops[dual_sys].mode = MANUAL_MATO_MODE;
                            status = S_ATC_DEFAULT_HANDLE;
                        }

                        break;

                    default:
                        status = ERR_INVALID_INDEX;
                        break;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            {
                MN_PHONE_CURRENT_PLMN_INFO_T  cur_plmn_info;
                uint8        oper_str[6] = {0};
                MN_NETWORK_NAME_T * name_ptr = PNULL;
                BOOLEAN   is_name_present = FALSE;
                uint8        *oper_name_ptr = PNULL;
                uint16       oper_name_len = 0;

                cur_plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

                switch(cur_plmn_info.plmn_status)
                {
                    case PLMN_NO_SERVICE:
                    case PLMN_EMERGENCY_ONLY:
                    case PLMN_REGISTER_SERVICE:
                    case PLMN_REGISTER_GPRS_ONLY:
                        sprintf((char *)g_rsp_str, "%s: %d",
                                g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                atc_config_ptr->cops[dual_sys].mode);
                        status = S_ATC_SUCCESS;
                        break;

                    case PLMN_NORMAL_GSM_ONLY:
                    case PLMN_NORMAL_GPRS_ONLY:
                    case PLMN_NORMAL_GSM_GPRS_BOTH:
                    case PLMN_EMERGENCY_GPRS_ONLY:
                        {
                            plmn.mnc = cur_plmn_info.mnc;
                            plmn.mcc = cur_plmn_info.mcc;
                            plmn.mnc_digit_num= cur_plmn_info.mnc_digit_num;

                            if(PLMN_FORMAT_NUMBERIC == atc_config_ptr->cops[dual_sys].format)
                            {
                                TransformPlmnToNumeric(&plmn, oper_str);

                                sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                        g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                        atc_config_ptr->cops[dual_sys].mode, atc_config_ptr->cops[dual_sys].format, oper_str, 0);
                            }
                            else
                            {
                                if ((g_cur_plmn_info[dual_sys].cur_plmn.mcc == plmn.mcc) &&
                                     (g_cur_plmn_info[dual_sys].cur_plmn.mnc == plmn.mnc))
                                {
                                    if ((PLMN_FORMAT_FULL_NAME == atc_config_ptr->cops[dual_sys].format) &&
                                         g_cur_plmn_info[dual_sys].full_name.network_name_exist)
                                    {
                                        is_name_present = TRUE;
                                        name_ptr = &g_cur_plmn_info[dual_sys].full_name;
                                    }
                                    else if ((PLMN_FORMAT_SHORT_NAME == atc_config_ptr->cops[dual_sys].format) &&
                                         g_cur_plmn_info[dual_sys].short_name.network_name_exist)
                                    {
                                        is_name_present = TRUE;
                                        name_ptr = &g_cur_plmn_info[dual_sys].short_name;
                                    }
                                    else
                                    {
                                        ATC_TRACE_LOW("ATC: ATC_ProcessCOPS plmn_format %d name not report", atc_config_ptr->cops[dual_sys].format);
                                    }

                                    if (is_name_present && (name_ptr != PNULL))
                                    {
                                        oper_name_len = MAX_NETWORK_NAME_LEN * 2 + 1;
                                        oper_name_ptr = (uint8 *)SCI_ALLOC_BASE(oper_name_len);
                                        SCI_ASSERT(NULL != oper_name_ptr);/*assert verified: check null pointer*/
                                        SCI_MEMSET(oper_name_ptr, 0, oper_name_len);

                                        if (ATC_ChsetTransformation((ATC_CHARACTER_SET_TYPE_E)name_ptr->code_scheme,
                                                                    name_ptr->name,
                                                                    name_ptr->length,
                                                                    ATC_CHSET_IRA,
                                                                    MAX_NETWORK_NAME_LEN * 2,
                                                                    oper_name_ptr,
                                                                    &oper_name_len))
                                        {
                                            oper_name_ptr[oper_name_len] = '\0';
                                        }
                                        else
                                        {
                                            ATC_TRACE_LOW("ATC: ATC_ProcessCOPS character set transformation error");
                                            is_name_present = FALSE;
                                        }
                                    }
                                }
                                else
                                {
                                    ATC_TRACE_LOW("ATC: ATC_ProcessCOPS mcc and mnc not matched");
                                }

                                if (is_name_present)
                                {
                                    sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                            g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                            atc_config_ptr->cops[dual_sys].mode, atc_config_ptr->cops[dual_sys].format,
                                            oper_name_ptr, 0);
                                }
                                else
                                {
                                    plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_MNC_MCC, (unsigned char *)&plmn);
    
                                    if(plmn_detailed_info && (PLMN_FORMAT_FULL_NAME == atc_config_ptr->cops[dual_sys].format))
                                    {
                                        sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                                g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                                atc_config_ptr->cops[dual_sys].mode, atc_config_ptr->cops[dual_sys].format,
                                                plmn_detailed_info->full_operator_name, 0);
                                    }
                                    else if(plmn_detailed_info && (PLMN_FORMAT_SHORT_NAME == atc_config_ptr->cops[dual_sys].format))
                                    {
                                        sprintf((char *)g_rsp_str, "%s: %d,%d,\"%s\",%lu",
                                                g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                                atc_config_ptr->cops[dual_sys].mode, atc_config_ptr->cops[dual_sys].format,
                                                plmn_detailed_info->short_operator_name, 0);
                                    }
                                    else
                                    {
                                        TransformPlmnToNumeric(&plmn, oper_str);
                                        sprintf((char *)g_rsp_str, "%s: %d,%d,\"\",%lu",
                                                g_atc_info_table[AT_CMD_COPS].cmd_name->str_ptr,
                                                atc_config_ptr->cops[dual_sys].mode, atc_config_ptr->cops[dual_sys].format, 0);
                                    }

                                }
 
                                if (oper_name_ptr != PNULL)
                                {
                                    SCI_FREE(oper_name_ptr);
                                }

                            }
                        }
                        status = S_ATC_SUCCESS;
                        break;

                    default:
                        status = ERR_INVALID_INDEX;
                        ATC_TRACE_LOW("ATC: Assert Failure plmn_status.");
                        break;
                }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            break;

        case ATC_CMD_TYPE_TEST:
            ATC_TRACE_LOW("ATC: search the plmn list");

            err_code = MNPHONE_ListPLMNEx(dual_sys, MN_GMMREG_RAT_GPRS);

            if(ERR_MNPHONE_NO_ERR == err_code)
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_PLMN_LIST_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCOPS, failure in MNPHONE_ListPLMN.");
                status = ERR_OPERATION_NOT_ALLOWED;
                return status;
            }

            atc_config_ptr->plmn_search_flag = TRUE;
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return status;

}


/*****************************************************************************/
//  Description : This function handle the AT+CPOL command
//  Global resource dependence : none
//  Author:       sunny
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPOL)
{
    ATC_STATUS                      status              = S_ATC_DEFAULT_HANDLE;
#ifndef _ULTRA_LOW_CODE_
    const ATC_PLMN_DETAILED_INFO_T  *plmn_detailed_info = NULL;
    MN_PHONE_WRITE_PLMN_T           plmn_item;
    int                             plmn;
    char                            plmn_str[7] = {0};
    int                             plmn_len = 0;
    uint32                          max_num     = 0;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;


    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG && !PARAM3_FLAG)
            {
                if(!PARAM2_FLAG)
                {
                    status = ERR_OPERATION_NOT_ALLOWED;
                }
                else
                {
                    atc_config_ptr->cops[dual_sys].format = (ATC_PLMN_FORMAT_E)PARA2.num;
                    status = S_ATC_SUCCESS;
                }

                break;
            }

            if(PARAM1_FLAG && !PARAM3_FLAG)
            {
                plmn_item.order_of_plmn = PARA1.num;
                plmn_item.plmn_list_type = (MN_PHONE_PLMN_LIST_TYPE_E)s_plmn_list_type;
                plmn_item.is_delete_item = TRUE;// delete item in the plmn list

                if(ERR_MNPHONE_NO_ERR != MNPHONE_WritePLMNtoSimEx(dual_sys, plmn_item))
                {
                    ATC_TRACE_LOW("ATC: MNPHONE_WritePLMNtoSim failure.");
                    return ERR_UNKNOWN;
                }

                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_PREFER_PLMN_CHANGE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
                break;
            }

            if(!PARAM1_FLAG && PARAM3_FLAG)
            {
                plmn_item.order_of_plmn = 0;//invalid order
            }
            else
            {
                plmn_item.order_of_plmn = PARA1.num;
            }

            plmn_item.plmn_list_type = (MN_PHONE_PLMN_LIST_TYPE_E)s_plmn_list_type;
            plmn_item.is_delete_item = FALSE;// write item in the plmn list

            if(!PARAM2_FLAG || PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            /* write PLMN with special format. */
            switch(PARA2.num)
            {
                case PLMN_FORMAT_FULL_NAME:
                    plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_FULL_NAME, PARA3.str_ptr->str_ptr);

                    if(!plmn_detailed_info)
                    {
                        ATC_TRACE_LOW("ATC:CPOL FULL_NAME not found!");
                        return S_ATC_FAIL;
                    }

                    sprintf(plmn_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
                    plmn_len = strlen(plmn_str);
                    break;

                case PLMN_FORMAT_SHORT_NAME:
                    plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_SHORT_NAME, PARA3.str_ptr->str_ptr);

                    if(!plmn_detailed_info)
                    {
                        ATC_TRACE_LOW("ATC:CPOL SHORT_NAME not found!");
                        return S_ATC_FAIL;
                    }

                    sprintf(plmn_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
                    plmn_len = strlen(plmn_str);
                    break;

                default: /* PLMN_FORMAT_NUMBERIC */
                    strcpy(plmn_str, (char *)PARA3.str_ptr->str_ptr);
                    plmn_len = PARA3.str_ptr->str_len;
                    break;
            }

            if(!CheckPlmnStr((uint8)plmn_len, (uint8 *)plmn_str))
            {
                ATC_TRACE_LOW("ATC:CPOL not support this plmn");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            plmn = atoi(plmn_str);
            ATC_TRACE_LOW("ATC:cpol plmn = %d", plmn);

            if(plmn_len == 6)
            {
                plmn_item.plmn.mnc_digit_num = 3;
                plmn_item.plmn.mcc = plmn / 1000;
                plmn_item.plmn.mnc = plmn % 1000;
            }
            else if(plmn_len == 5)
            {
                plmn_item.plmn.mnc_digit_num = 2;
                plmn_item.plmn.mcc = plmn / 100;
                plmn_item.plmn.mnc = plmn % 100;
            }
            else
            {
                ATC_TRACE_LOW("ATC: cpol not support this plmn");
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            if(PARAM4_FLAG)
            {
                if(PARA4.num)
                {
                    plmn_item.is_gsm_act = TRUE;
                }
            }

            if(PARAM5_FLAG)
            {
                if(PARA5.num)
                {
                    ATC_TRACE_LOW("ATC: cpol not support to set gsm_compact_act ");
                    return ERR_OPERATION_NOT_SUPPORTED;
                    /*plmn_item.is_gsm_compact_act= TRUE;*/
                }
            }

            if(PARAM6_FLAG)
            {
                if(PARA6.num)
                {
                    plmn_item.is_td_act = TRUE;
                }
            }

            if(ERR_MNPHONE_NO_ERR != MNPHONE_WritePLMNtoSimEx(dual_sys, plmn_item))
            {
                ATC_TRACE_LOW("ATC: MNPHONE_WritePLMNtoSim failure");
                return S_ATC_FAIL;
            }

            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              (uint16)APP_MN_PREFER_PLMN_CHANGE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            break;

        case ATC_CMD_TYPE_READ:
            ATC_TRACE_LOW("ATC: reading the plmn list");

            // read all items from the plmn list
            if(ReadPreferredPlmnList(dual_sys, atc_config_ptr))
            {
                status = S_ATC_SUCCESS;
            }
            else
            {
                status = S_ATC_FAIL;
            }

            break;

        case ATC_CMD_TYPE_TEST:
            max_num = ReadPreferredPlmnMaxNum(dual_sys);

            if(0 < max_num)
            {
                sprintf((char *)g_rsp_str, "%s: (1-%ld),(0-2)", \
                        g_atc_info_table[AT_CMD_CPOL].cmd_name->str_ptr,
                        max_num);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                status = S_ATC_SUCCESS;
            }

            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return status;

}

/*****************************************************************************/
//  Description : This function is used to read preferred PLMN list.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadPreferredPlmnList(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T    *atc_config_ptr
)
{
    const ATC_PLMN_DETAILED_INFO_T  *plmn_detailed_info = NULL;
    MN_PHONE_PLMN_SEL_LIST_T    plmnsel_list = {0};
    BOOLEAN   result = TRUE;
    uint8          plmn_str[ATC_MAX_FULL_OP_NAME] = "";
    uint32        num_per_lin = 0;
    uint32        offset = 0;
    uint32        i = 0;
    BOOLEAN       is_gsm_act = FALSE;
    //BOOLEAN       is_compactgsm_act = FALSE;
    BOOLEAN       is_td_act = FALSE;
    

    SCI_MEMSET(plmn_str, 0, sizeof(plmn_str));

    if(ERR_MNPHONE_NO_ERR == MNPHONE_GetPlmnSelectListEx(dual_sys, &plmnsel_list))
    {
        for(i = 0; i < plmnsel_list.plmn_validate_num; i++)
        {
            plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_MNC_MCC, (unsigned char *)&plmnsel_list.plmn_arr[i]);

            if (plmnsel_list.rat[i] == MN_GMMREG_RAT_GPRS)
            {
                is_gsm_act = TRUE;
            }else if(plmnsel_list.rat[i] == MN_GMMREG_RAT_3G)
            {
                is_td_act = TRUE;
            }

            if(plmn_detailed_info && (PLMN_FORMAT_FULL_NAME == atc_config_ptr->cops[dual_sys].format))
            {
                strcpy((char *)plmn_str, (char *)plmn_detailed_info->full_operator_name);
                plmn_str[strlen((char *)plmn_detailed_info->full_operator_name)] = '\0';
            }
            else if(plmn_detailed_info && (PLMN_FORMAT_SHORT_NAME == atc_config_ptr->cops[dual_sys].format))
            {
                strcpy((char *)plmn_str, (char *)plmn_detailed_info->short_operator_name);
                plmn_str[strlen((char *)plmn_detailed_info->short_operator_name)] = '\0';
            }
            else
            {
                TransformPlmnToNumeric(&plmnsel_list.plmn_arr[i], plmn_str);
            }

            if(i < plmnsel_list.plmn_validate_num - 1)
            {
                num_per_lin = sprintf((char *)(g_rsp_str + offset), "%s: %lu,%d,\"%s\",%d,%d,%d\r\n",
                                      g_atc_info_table[AT_CMD_CPOL].cmd_name->str_ptr,
                                      i + 1, atc_config_ptr->cops[dual_sys].format, plmn_str,
                                      is_gsm_act, 0, is_td_act);
            }
            else
            {
                num_per_lin = sprintf((char *)(g_rsp_str + offset), "%s: %lu,%d,\"%s\",%d,%d,%d",
                                      g_atc_info_table[AT_CMD_CPOL].cmd_name->str_ptr,
                                      i + 1, atc_config_ptr->cops[dual_sys].format, plmn_str,
                                      is_gsm_act, 0, is_td_act);
            }

            offset += num_per_lin;

            if((offset + num_per_lin > MAX_ATC_RSP_LEN)
                    || (i + 1 == plmnsel_list.plmn_validate_num))
            {
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                offset = 0;
            }

            SCI_MEMSET(plmn_str, 0, ATC_MAX_FULL_OP_NAME);

            is_gsm_act = FALSE;
            //is_compactgsm_act = FALSE;
            is_td_act = FALSE;
        }
    }
    else
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : This function is used to read max num of preferred PLMN list.
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
LOCAL uint32 ReadPreferredPlmnMaxNum(MN_DUAL_SYS_E   dual_sys)
{
    uint32                      max_num = 0;
    MN_PHONE_PLMN_SEL_LIST_T    plmnsel_list;

    if(ERR_MNPHONE_NO_ERR == MNPHONE_GetPlmnSelectListEx(dual_sys, &plmnsel_list))
    {
        max_num = plmnsel_list.plmn_max_num;
    }

    SCI_TRACE_LOW("ATC: ReadPreferredPlmnMaxNum max_num = %d", max_num);
    return (max_num);
}

/*****************************************************************************/
//  Description : This function handles the AT+SDMUT command.This command is used
//                to mute downlink of phone.
//  AT+CRSL=<sound level>
//  Global resource dependence : none
//  Author:       hanjun.liu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDMUT)
{
    ATC_STATUS           status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            atc_config_ptr->sdmut_flag = PARA1.num;
            ATC_EnableRemoteMute(atc_config_ptr->sdmut_flag);
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SDMUT].cmd_name->str_ptr, atc_config_ptr->sdmut_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}


/*****************************************************************************/
//  Description : This function handles the AT+CEER. This command is used to
//  get the cause of call release when the last call set up.
//  Global resource dependence : none
//  Author: Xueli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCEER)
{
#ifndef _ULTRA_LOW_CODE_
    ATC_STATUS status = S_ATC_SUCCESS;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            sprintf((char *)g_rsp_str, "+CEER: ");
            ATC_GetCeerErrVerbose(((int32)atc_config_ptr->ceer_val), ((char *)g_rsp_str + strlen((char *)g_rsp_str)));
            //sprintf((char*)g_rsp_str, "+CEER: Error %ld", atc_config_ptr->ceer_val);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
#else
    return S_ATC_SUCCESS;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifdef _MODEM_MODE 
/*****************************************************************************/
//  Description : This function check the value of Is_SleepAllowed
//  Global resource dependence : none
//  Author:       Brand
//  Note:
/*****************************************************************************/
BOOLEAN ATC_SleepIsAllowed(void)
{
    return s_Is_SleepAllowed;
}
#endif

/*****************************************************************************/
//  Description : This function handles the APP_MN_PREFER_PLMN_CHANGE_CNF at the
//                 AT+CPOL
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPreferPlmnChangCnf(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_PREFER_PLMN_CHANGE_CNF_T         *sig_ptr
)
{
#ifndef ATC_SAT_ENABLE
    MN_PHONE_PLMN_SEL_LIST_T tpm_plmn_list = {0};
    MN_DUAL_SYS_E            dual_sys = MN_DUAL_SYS_1;
    uint8                    index = 0;
    uint8                    i;

    if ((sig_ptr == PNULL) || (atc_config_ptr == PNULL))
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessPreferPlmnChangCnf, warning empty pointer!");
        return S_ATC_DEFAULT_HANDLE;
    }

    SCI_TRACE_LOW("ATC_ProcessPreferPlmnChangCnf result = %d", sig_ptr->is_ok);

    tpm_plmn_list = sig_ptr->new_prefer_plmn_list;
    dual_sys = sig_ptr->dual_sys;

    if ((sig_ptr->is_ok)&&(g_atc_uplmn_info[dual_sys].mode == ATC_UPLMN_SET))
    {
        if (tpm_plmn_list.plmn_validate_num < tpm_plmn_list.plmn_max_num)
        {
            index = g_atc_uplmn_info[dual_sys].index;
            if (index + 1 > tpm_plmn_list.plmn_validate_num)
            {
                index = tpm_plmn_list.plmn_validate_num++;
            }

            tpm_plmn_list.plmn_arr[index] = g_atc_uplmn_info[dual_sys].plmn;           
            tpm_plmn_list.rat[index] = g_atc_uplmn_info[dual_sys].rat;

            MNSIM_SetPLMNWACTEx(dual_sys, &tpm_plmn_list);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_PREFER_PLMN_CHANGE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
        }
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
        }
    }
    else if ((sig_ptr->is_ok)&&(g_atc_uplmn_info[dual_sys].mode == ATC_UPLMN_GET))
    {
        uint8    oper_str[6+1] = {0};
        uint8    access_type = 2;
        uint8   *ptr = g_rsp_str;

        for(i = 0; i < tpm_plmn_list.plmn_validate_num; i++)
        {
            if (tpm_plmn_list.rat[i] == MN_GMMREG_RAT_3G)
            {
                access_type = 0;  //UMTS
            }
            else if (tpm_plmn_list.rat[i] == MN_GMMREG_RAT_VDS_GSM)
            {
                access_type = 1;  //GSM
            }
            else
            {
                access_type = 2;  //OTHER
            }

            TransformPlmnToNumeric(&tpm_plmn_list.plmn_arr[i], oper_str);

            SCI_TRACE_LOW("ATC: +SPUPLMN, oper_str:%s",oper_str);

            sprintf((char *)ptr, "%s:%lu,%s,%d\r\n",g_atc_info_table[AT_CMD_SPUPLMN].cmd_name->str_ptr,i,oper_str,access_type);

            ptr = g_rsp_str + strlen((char *)g_rsp_str);
        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    else
#endif
    {
        if(sig_ptr->is_ok)
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
        }
    }

#ifndef ATC_SAT_ENABLE
    ATC_InitUplmnCfgInfo(dual_sys);
#endif
    
    return S_ATC_DEFAULT_HANDLE;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_PLMN_LIST_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPlmnListCnf(
    ATC_CONFIG_T                   *atc_config_ptr,
    APP_MN_PLMN_LIST_CNF_T         *sig_ptr
)
{
    ATC_STATUS              status = S_ATC_SUCCESS;
    uint8                   oper_str[6] = {0};
    int32                   i = 0;
    uint8                   *temp_ptr = g_rsp_str;
    uint32                  state;
    uint32                  rat = 0;

    const ATC_PLMN_DETAILED_INFO_T *plmn_detailed_info = NULL;
    MN_PHONE_CURRENT_PLMN_INFO_T register_status = {0};
    MN_DUAL_SYS_E            dual_sys = ATC_CURRENT_SIM_ID_MUX;

    ATC_TRACE_LOW("ATC: received the plmn list confirm ");
    ATC_TRACE_LOW("ATC: %d,%d", sig_ptr->is_valid_result, sig_ptr->plmn_detail_list.plmn_num);
    
#ifdef ATC_SAT_ENABLE
    if(g_atc_scan_cell_flag)
    {
    	g_atc_scan_cell_flag = FALSE;
        return status;
    }
#endif

    if(atc_config_ptr->plmn_search_flag)
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
                    /* get the current register state*/
                    register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                    if ((register_status.plmn_status == PLMN_NORMAL_GSM_ONLY ||
                       register_status.plmn_status == PLMN_NORMAL_GPRS_ONLY ||
                       register_status.plmn_status == PLMN_NORMAL_GSM_GPRS_BOTH)
                       && (sig_ptr->plmn_detail_list.plmn_details[i].plmn.mcc == register_status.mcc) 
                       && (sig_ptr->plmn_detail_list.plmn_details[i].plmn.mnc == register_status.mnc)
                       && (sig_ptr->plmn_detail_list.plmn_details[i].plmn.mnc_digit_num == register_status.mnc_digit_num))
                    {
                        state = ATC_COPS_CURRENT_STATE;
                    }
                    else
                    {
                        state = ATC_COPS_AVAILABLE_STATE;
                    }
                    
                }

                switch(sig_ptr->plmn_detail_list.rat[i])
                {
                    case MN_GMMREG_RAT_GPRS:
                        rat = 0;
                        break;
                    case MN_GMMREG_RAT_3G:
                        rat = 2;
                        break;
                    default:
                        status = ERR_INVALID_INDEX;
                        ATC_TRACE_LOW("ACT: RAT value is wrong");
                        break;
                }

                SCI_MEMSET(&oper_str[0], 0, sizeof(oper_str));
                TransformPlmnToNumeric(&sig_ptr->plmn_detail_list.plmn_details[i].plmn, oper_str);
                plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_MNC_MCC, (unsigned char *)&sig_ptr->plmn_detail_list.plmn_details[i].plmn);

                if(plmn_detailed_info)
                {
                    sprintf((char *)temp_ptr, "(%ld,\"%s\",\"%s\",\"%s\",%lu),", state,
                            plmn_detailed_info->full_operator_name,
                            plmn_detailed_info->short_operator_name,
                            oper_str, rat);
                }
                else
                {
                    sprintf((char *)temp_ptr, "(%ld,\"\",\"\",\"%s\",%lu),", state, oper_str, rat);
                }

                temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];
            }


            sprintf((char *)temp_ptr, ",(0-4),(0-2)");
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

            sprintf((char *)temp_ptr, "(),,(0-4),(0-2)");
            temp_ptr = &g_rsp_str[strlen((char *)g_rsp_str)];

            g_rsp_str[strlen((char *)g_rsp_str)] = '\0';

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

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
//  Description : This function handles the APP_MN_PLMN_SELECT_CNF signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessPlmnSelCnf(
    ATC_CONFIG_T                *atc_config_ptr,
    APP_MN_PLMN_SELECT_CNF_T    *sig_ptr
)
{
    BOOLEAN     is_plmn_success = FALSE;
    MN_PLMN_T   plmn;

    SCI_MEMSET(&plmn, 0, sizeof(plmn));

    // The current PLMN is select
    if(sig_ptr->plmn_is_selected)
    {
        switch(sig_ptr->plmn_status)
        {
                // no service
            case PLMN_NO_SERVICE:

                if(MANUAL_MATO_MODE != atc_config_ptr->cops[sig_ptr->dual_sys].mode)
                {
                    if ((atc_config_ptr->cops[sig_ptr->dual_sys].mode != AUTO_MODE) 
                        || (!atc_config_ptr->async_mode_flag))
                    {
                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
                    }
                }

                break;

                // voice service available
            case PLMN_NORMAL_GSM_ONLY:
                // data service available
            case PLMN_NORMAL_GPRS_ONLY:
                // voice and data service available
            case PLMN_NORMAL_GSM_GPRS_BOTH:
                is_plmn_success = TRUE;
                if ((atc_config_ptr->cops[sig_ptr->dual_sys].mode != AUTO_MODE) 
                    || (!atc_config_ptr->async_mode_flag))
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                }
                break;

                // emergency service available
            case PLMN_EMERGENCY_ONLY:
            case PLMN_EMERGENCY_GPRS_ONLY:
                if(MANUAL_MATO_MODE != atc_config_ptr->cops[sig_ptr->dual_sys].mode)
                {
                    if ((atc_config_ptr->cops[sig_ptr->dual_sys].mode != AUTO_MODE) 
                        || (!atc_config_ptr->async_mode_flag))
                    {
                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY);
                    }
                }

                break;

            default:
                ATC_TRACE_LOW("ATC: Assert Failure unknow plmn_status.");
                break;
        }
    }
    else
    {
        if(MANUAL_MATO_MODE != atc_config_ptr->cops[sig_ptr->dual_sys].mode)
        {
            ATC_TRACE_LOW("ATC: PlmnSelCnf sig_ptr->cause: 0x%x", sig_ptr->cause);
            if ((atc_config_ptr->cops[sig_ptr->dual_sys].mode != AUTO_MODE) 
                || (!atc_config_ptr->async_mode_flag))
            {
                if(MN_MM_PLMN_NOT_ALLOWED == sig_ptr->cause)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_PLMN_NOT_ALLOWED);
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
                }
            }
        }
    }

    //if it dont succeed in MANUAL_MATO_MODE,select the PLMN automatically.
    if((FALSE == is_plmn_success) && (MANUAL_MATO_MODE == atc_config_ptr->cops[sig_ptr->dual_sys].mode))
    {
        ERR_MNPHONE_CODE_E err_code;

        ATC_TRACE_LOW("ATC: ATC_ProcessPlmnSelCnf, is_plmn_success:FALSE, MANUAL_MATO_MODE");

        err_code = MNPHONE_SelectPLMNEx(sig_ptr->dual_sys, FALSE, plmn, MN_GMMREG_RAT_UNKNOWN);


        if(ERR_MNPHONE_NO_ERR == err_code)
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              (uint16)APP_MN_PLMN_SELECT_CNF, 0xff, ATC_NO_DOMAIN, sig_ptr->dual_sys);
            atc_config_ptr->cops[sig_ptr->dual_sys].mode = AUTO_MODE;
            return S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            if ((atc_config_ptr->cops[sig_ptr->dual_sys].mode != AUTO_MODE) 
                || (!atc_config_ptr->async_mode_flag))
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, ERR_NO_NETWORK_SERVICE);
            }
            return S_ATC_RESULT_CODE_ERROR;
        }
    }

    //clear information of g_cur_plmn_info
    SCI_MEMSET((void *)&g_cur_plmn_info[sig_ptr->dual_sys], 0, sizeof(ATC_PLMN_INFO_T));

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+COPS set command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ManualSelectPlmn(
    MN_DUAL_SYS_E      dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    ATC_PLMN_FORMAT_E         oper_format,
    ATC_CUR_PARAM_STRING_T    *oper_ptr,
    uint32                    rat
)
{
    const ATC_PLMN_DETAILED_INFO_T *plmn_detailed_info = NULL;
    MN_GMMREG_RAT_E    set_rat;
    MN_PLMN_T          selected_plmn;    // the selected PLMN used in the manual selection mode
    int32              plmn;
    char               oper_str[6+1] = {0};
    int32              oper_len = 0;

    /* Get rat */
    switch(rat)
    {
        case 0:
        case 1:
            set_rat = MN_GMMREG_RAT_GPRS;
            break;
        case 2:
            set_rat = MN_GMMREG_RAT_3G;
            break;
        default:
            set_rat = MN_GMMREG_RAT_UNKNOWN;
            break;
    }

    /* Manual PLMN selection with operation format. */
    switch(oper_format)
    {
        case PLMN_FORMAT_FULL_NAME:
            plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_FULL_NAME, oper_ptr->str_ptr);

            if(!plmn_detailed_info)
            {
                ATC_TRACE_LOW("ATC: ManualSelectPlmn, FULL_NAME, not found!");
                return S_ATC_FAIL;
            }

            sprintf(oper_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
            oper_len = strlen(oper_str);
            break;

        case PLMN_FORMAT_SHORT_NAME:
            plmn_detailed_info = ATC_GetPlmnDetailedInfo(PLMN_TYPE_SHORT_NAME, oper_ptr->str_ptr);

            if(!plmn_detailed_info)
            {
                ATC_TRACE_LOW("ATC: ManualSelectPlmn, SHORT_NAME, not found!");
                return S_ATC_FAIL;
            }

            sprintf(oper_str, "%d%02d", plmn_detailed_info->mcc_data, plmn_detailed_info->mnc_data);
            oper_len = strlen(oper_str);
            break;

        default: /* PLMN_FORMAT_NUMBERIC */

            if(!CheckPlmnStr(oper_ptr->str_len, oper_ptr->str_ptr))
            {
                ATC_TRACE_LOW("ATC: ManualSelectPlmn, NUMBERIC, cops not support this plmn");
                return S_ATC_FAIL;
            }

            strcpy(oper_str, (char *)oper_ptr->str_ptr);
            oper_len = oper_ptr->str_len;
            break;
    }

    plmn = atoi(oper_str);
    ATC_TRACE_LOW("ATC: ManualSelectPlmn, plmn = %d", plmn);

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
        ATC_TRACE_LOW("ATC: ManualSelectPlmn, cops not support this plmn");
        return S_ATC_FAIL;
    }

    if(ERR_MNPHONE_NO_ERR != MNPHONE_SelectPLMNEx(dual_sys, TRUE, selected_plmn, set_rat))

    {
        ATC_TRACE_LOW("ATC: ManualSelectPlmn, failure in MN");
        return S_ATC_FAIL;
    }

    atc_config_ptr->cops[dual_sys].format = oper_format;
    return S_ATC_SUCCESS;

}

/*****************************************************************************/
//  Description : This function handle the ATH command
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessH)
{
    MN_CALL_DISCONNECT_CALL_CAUSE_E  cause_value = MN_CAUSE_NORMAL_CLEARING;
    ATC_CALLS_STATE_T *  calls_state_ptr = PNULL;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8                            link_id;
    uint32                           i;

#ifdef _MUX_ENABLE_
    link_id = atc_config_ptr->current_link_id;
#else
    link_id = 0;
#endif

    if (ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessH  Command Type not match");
        return S_ATC_FAIL;
    }

    if(SCI_IS_TIMER_ACTIVE(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND))
    {
        SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND,
                       &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
        g_atc_expire_call_id = 0xff;
        g_atc_expire_ind = FALSE;
    }

    if(g_atc_call_flag)
    {
        g_atc_call_need_disc = TRUE;//assume there is a pending MO CALL
    }

    ATC_TRACE_LOW("ATC: link_id:%d,g_calls_state.call_amout= %d", link_id, g_calls_state[dual_sys].call_amount);

    /* if disconnect_modem occured, then  ath will be dropped */
    if(TRUE == ATC_Link_ATHEventHandle(link_id, ATC_ATH_EV_RECV_ATH, atc_link_inst[link_id].ath_cid))
    {
        return S_ATC_SUCCESS; /* we must return success to avoid ath sent again  and again by pc*/
    }

    calls_state_ptr = &g_calls_state[dual_sys];
    if(calls_state_ptr->call_amount >= 1)
    {
        atc_config_ptr->ath_flag = TRUE;

        if((PARAM1_FLAG) && (PARAM1_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessH, cause_value=%d", PARA1.num);
            cause_value = (MN_CALL_DISCONNECT_CALL_CAUSE_E)PARA1.num;
        }

        //Release all the call
        for(i = 0; i < calls_state_ptr->call_amount; i++)
        {
            if(!PARAM1_FLAG &&
               ATC_CALL_ALERTING_STATE == calls_state_ptr->call_context[i].call_entity_state)
            {
                cause_value = MN_CAUSE_USER_BUSY;
            }

            MNCALL_DisconnectCallEx(dual_sys, calls_state_ptr->call_context[i].call_id, cause_value, PNULL);

        #ifdef _MUX_ENABLE_
            ATC_Clear_All_Exp_Result(calls_state_ptr->call_context[i].call_id, ATC_DOMAIN_CS, dual_sys);
            ATC_Add_Expected_Event(atc_config_ptr, APP_MN_CALL_DISCONNECTED_IND, 
                              calls_state_ptr->call_context[i].call_id, ATC_DOMAIN_CS, dual_sys);
        #endif
        }

        return S_ATC_DEFAULT_HANDLE;
    }
    else
    {
        if(g_atc_call_flag)
        {
            return S_ATC_DEFAULT_HANDLE;//do nothing if there is a pending MO CALL
        }

        return S_ATC_SUCCESS;
    }

}


/*****************************************************************************/
//  Description : This function handle the APP_MN_SETUP_IND signal and return
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
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;
    uint8           call_index = 0xff;
    uint8           i;

    if(atc_config_ptr->urc_level !=ATC_URC_ALL_STATUS_ON
        && atc_config_ptr->urc_level != ATC_URC_CALL_SMS_STATUS_ON)
    {
        return S_ATC_FAIL;
    }
	
    if (!atc_config_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSetupInd  atc_config_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if (!sig_ptr)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSetupInd  sig_ptr pointer is PNULL");
        return S_ATC_FAIL;
    }

    if(g_calls_state[dual_sys].call_amount >= MN_CALL_MAX_CALL_NUM)
    {
        ATC_TRACE_LOW("ATC: Assert Failure call_amount %d.", g_calls_state[dual_sys].call_amount);
        return S_ATC_DEFAULT_HANDLE;
    }
    
    for(i = 0; i < MN_SYS_NUMBER; i++)
    {
        if((i != dual_sys) && (g_calls_state[i].call_amount != 0))
        {
            SCI_TRACE_LOW("ATC: mt meet another sim mo call, dual_sys = %d, i = %d, amount = %d", dual_sys, i, g_calls_state[i].call_amount);
            //exist a MO call in other sim card, disconnect it.

            MNCALL_DisconnectCallEx((MN_DUAL_SYS_E)i, 0, MN_CAUSE_NORMAL_CLEARING, PNULL);
        }
    }

#ifdef _MUX_ENABLE_
    ATC_HandleGprsSuspendRsp(atc_config_ptr);
#endif 

    s_mt_need_open_codec_flag = 0;
   
    s_is_self_discon[sig_ptr->call_id] = FALSE;
    
    //set the initial value when call course starting
    s_atc_call_disc_cause = 0xffffffff;

    g_calls_state[dual_sys].call_amount++;
    call_index = g_calls_state[dual_sys].call_amount - 1;

    g_calls_state[dual_sys].call_context[call_index].direction = TRUE;
    g_calls_state[dual_sys].call_context[call_index].call_id = sig_ptr->call_id;
    g_calls_state[dual_sys].call_context[call_index].call_entity_state = ATC_CALL_INCOMING_STATE;

    if(sig_ptr->call_type == MN_CALL_TYPE_NORMAL || sig_ptr->call_type == MN_CALL_TYPE_EMERGENCY || sig_ptr->call_type == MN_CALL_TYPE_SPEECH_PREFER)
    {
        g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_VOICE_MODE;
    }
    else if(sig_ptr->call_type == MN_CALL_TYPE_DATA || sig_ptr->call_type == MN_CALL_TYPE_DATA_PREFER)
    {
        g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_DATA_MODE;
    }
    else
    {
        g_calls_state[dual_sys].call_context[call_index].call_mode = ATC_UNKNOWN_CALLMODE;
    }

    if(sig_ptr->calling_num_present)
    {
        uint8              *party_number_ptr;

        party_number_ptr =  g_calls_state[dual_sys].call_context[call_index].party_number;

        ATC_ConvertCallingNumToStr(&(sig_ptr->calling_num), 
              &(g_calls_state[dual_sys].call_context[call_index].number_type), party_number_ptr);
    }
    else
    {
        g_calls_state[dual_sys].call_context[call_index].number_type = UNKNOWN_PLAN_UNKNOWN_TYPE;
        g_calls_state[dual_sys].call_context[call_index].party_number[0] = '\0';
    }
    #ifndef ATC_SAT_ENABLE
    s_mt_calling_num.calling_num = sig_ptr->calling_num;
    #endif

    // the call wait service is provide
    if(1 < g_calls_state[dual_sys].call_amount)
    {
        if(atc_config_ptr->ccwa_flag[dual_sys] && sig_ptr->calling_num_present)
        {
            ATC_TRACE_LOW("ATC: the call number len: %d", sig_ptr->calling_num.num_len);
            SCI_MEMSET(s_tel_str, 0, (MN_MAX_ADDR_BCD_LEN * 2 + 1));

            ATC_ConvertCallingNumToStr(&(sig_ptr->calling_num), &s_tel_num_type, s_tel_str);

            sprintf((char *)g_rsp_str, "%s: \"%s\",%d,%d,,%d",
                    g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                    s_tel_str, s_tel_num_type, 1, sig_ptr->calling_num.present_ind);

            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

            ATC_UpECINDInfo(dual_sys, atc_config_ptr, NULL, 0, 5, sig_ptr->call_id + 1, 0, 0);

            //Send the unsolicited +SIND: 5
            ATC_UpSINDInfo(dual_sys,
                           atc_config_ptr,
                           (xSignalHeaderRec *)sig_ptr,
                           ATC_WIND_BIT4,
                           5,
                           (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));
        #ifndef ATC_SAT_ENABLE
            s_mt_calling_num.calling_num_present = TRUE;
        #endif
        }
        else if(atc_config_ptr->ccwa_flag[dual_sys] && !sig_ptr->calling_num_present)
        {
            s_tel_str[0] = '\0';

            sprintf((char *)g_rsp_str, "%s: \"\",%d,%d,,%d",
                    g_atc_info_table[AT_CMD_CCWA].cmd_name->str_ptr,
                    UNKNOWN_PLAN_UNKNOWN_TYPE, 1, sig_ptr->calling_num.present_ind);
            ATC_UpECINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 0, 5, sig_ptr->call_id + 1, 0, 0);
        }

        ATC_UpDSCIInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, call_index, 5);

        return S_ATC_SUCCESS;
    }

    //Send the unsolicited +SIND: 5
    ATC_UpSINDInfo(dual_sys,
                   atc_config_ptr,
                   (xSignalHeaderRec *)sig_ptr,
                   ATC_WIND_BIT4,
                   5,
                   (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));
    ATC_UpECINDInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, 0, 4, sig_ptr->call_id + 1, 0, 0);


    ATC_UpDSCIInfo(dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr,  call_index, 4);

    // handle the calling number string
    if(sig_ptr->calling_num_present)
    {
        ATC_TRACE_LOW("ATC: the call number len: %d", sig_ptr->calling_num.num_len);
      #ifndef ATC_SAT_ENABLE
        s_mt_calling_num.calling_num_present = TRUE;
       #endif

    }
    else
    {
        ATC_TRACE_LOW("ATC: no call number present!");
        s_tel_str[0] = '\0';
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_ALERTING_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
ATC_STATUS ATC_ProcessAlertingInd(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_ALERTING_IND_T   *sig_ptr
)
{
    uint8           call_index;

    call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

    ATC_TRACE_LOW("ATC: ATC_ProcessAlertingInd");
    if(ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: invalid call index!");
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(MN_CALL_LOCAL_USER_ALERTING == sig_ptr->alerting_type)
    {
#ifdef _ATC_ONLY
        // Send Alert message to network
        MNCALL_IndicateAlertEx(sig_ptr->dual_sys, sig_ptr->call_id, PNULL);
#endif

        s_atc_call_ring_num = 0;
        atc_config_ptr->cpas = ATC_CPAS_STATUS_RING;
#ifndef ATC_SAT_ENABLE
        SCI_ASSERT(MN_MAX_ADDR_BCD_LEN >= s_mt_calling_num.calling_num.num_len);/*assert verified: check valid value*/

        SCI_MEMSET(s_tel_str, 0, (MN_MAX_ADDR_BCD_LEN * 2 + 1));

        ATC_ConvertCallingNumToStr(&(s_mt_calling_num.calling_num), &s_tel_num_type, s_tel_str);
		
        if(atc_config_ptr->atc_config_nv.clip_flag)
        {
            if(s_mt_calling_num.calling_num_present)
            {
                sprintf((char *)g_rsp_str, "%s: \"%s\",%d",
                        g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, s_tel_num_type);
            }
            else
            {
                s_tel_str[0] = '\0';
                sprintf((char *)g_rsp_str, "%s: \"%s\",%d",
                        g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, UNKNOWN_PLAN_UNKNOWN_TYPE);
            }

            ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
        }
#endif
        g_atc_expire_ind = TRUE;
        g_atc_expire_call_id = sig_ptr->call_id;
        // start the timer to periodic show the calling number
        SCI_SET_TIMER(P_ATC, 50, ATC_CALL_EXP_IND,
                      &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
    }

    if(MN_CALL_REMOTE_USER_ALERTING == sig_ptr->alerting_type)
    {
        g_calls_state[sig_ptr->dual_sys].call_context[call_index].call_entity_state = ATC_CALL_ALERTING_STATE;

        //Send the unsolicited +SIND: 5
        ATC_UpSINDInfo(sig_ptr->dual_sys,
                       atc_config_ptr,
                       NULL,
                       ATC_WIND_BIT1,
                       2,
                       (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));

        ATC_UpECINDInfo(sig_ptr->dual_sys, atc_config_ptr, NULL, 0, 3, sig_ptr->call_id + 1, 0, 0);

        ATC_UpDSCIInfo(sig_ptr->dual_sys, atc_config_ptr, (xSignalHeaderRec *)sig_ptr, call_index, 3);

        if( g_atc_call_flag && 
            (ATC_VOICE_MODE == g_calls_state[sig_ptr->dual_sys].call_context[call_index].call_mode))
        {
            ATC_HandleMoMuteAndLocalRingState(sig_ptr->dual_sys, APP_MN_ALERTING_IND, call_index);
        }
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_PROGRESS_IND signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:         
/*****************************************************************************/
ATC_STATUS ATC_ProcessProgressInd(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_PROGRESS_IND_T   *sig_ptr
)
{
    uint8           call_index;

    ATC_TRACE_LOW("ATC:ATC_ProcessProgressInd(), descrip = %d",sig_ptr->progress_ind.descrip);
    
    call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

    if(ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: invalid call index!");
        return ERR_OPERATION_NOT_ALLOWED;
    }
    
    if ( g_atc_call_flag && 
        (ATC_VOICE_MODE == g_calls_state[sig_ptr->dual_sys].call_context[call_index].call_mode) &&
        ((sig_ptr->progress_ind.descrip >= 1) && (sig_ptr->progress_ind.descrip <= 3) || 
        (sig_ptr->progress_ind.descrip >= 6) && (sig_ptr->progress_ind.descrip <= 20)))
    {
        ATC_HandleMoMuteAndLocalRingState(sig_ptr->dual_sys, APP_MN_PROGRESS_IND, call_index);
    }
    
    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the APP_MN_DISC_INFO_IND signal
//  Global resource dependence : none
//  Author:       xiaonian.li
//  Note:         
/*****************************************************************************/
ATC_STATUS ATC_ProcessDiscInfoInd(
    ATC_CONFIG_T            *atc_config_ptr,
    APP_MN_DISC_INFO_IND_T   *sig_ptr
)
{
    uint8           call_index;
    MN_DUAL_SYS_E   dual_sys = sig_ptr->dual_sys;

    SCI_TRACE_LOW("ATC:ATC_ProcessDiscInfoInd()");
    
    call_index = GetIndexOfCallContext(sig_ptr->call_id, sig_ptr->dual_sys);

    if(ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: invalid call index!");
        return ERR_OPERATION_NOT_ALLOWED;
    }
   
    //disable codec to avoid unnessary voice from network (sometimes network close chanel early)
    if ( g_atc_call_flag && atc_config_ptr->audio_flag && 
        (g_calls_state[dual_sys].call_amount <= 1))
    {
         ATC_EnableVoiceCodec(FALSE);
         atc_config_ptr->audio_flag = FALSE;
    }

#ifdef _SUPPORT_CCBS_
    //if ccbs is allowed, tell the dte
    if (TRUE == sig_ptr->is_ccbs_allowed)
    {
        atc_config_ptr->ccbs_info[dual_sys].cur_call_id = sig_ptr->call_id;
        sprintf((char *)g_rsp_str, "+CCBS: 1,%d", sig_ptr->call_id);
        ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, (uint8 *)g_rsp_str);
    }
#endif // #ifdef _SUPPORT_CCBS_

    //to record the actual cause of call disconnection
    s_atc_call_disc_cause = (uint32)sig_ptr->cause;

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function transform PLMN structure to numeric string
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

    SCI_ASSERT((PNULL != plmn_ptr) && (PNULL != numeric_str));/*assert verified: check null pointer*/
    temp = plmn_ptr->mcc;
    numeric_str[2] = (uint8)(temp % 10) + '0';       //MCC digit3
    temp /= 10;
    numeric_str[1] = (uint8)(temp % 10) + '0';       //MCC digit2
    temp /= 10;
    numeric_str[0] = (uint8)(temp % 10) + '0';       //MCC digit1

    temp = plmn_ptr->mnc;

    //if (temp >= 100)
    if(plmn_ptr->mnc_digit_num == 3)
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
//  Description : This function handle the APP_MN_CALL_DISCONNECTED_IND or APP_MN_CALL_ERR_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallDiscInd(
    ATC_CONFIG_T            *atc_config_ptr,
    xSignalHeaderRec        *sig_ptr
)
{
    BOOLEAN                         waitingcall_existed_b4_flag = FALSE;
    BOOLEAN                         is_voice_call               = FALSE;
    uint8                           call_id                     = 0;
    uint8                           call_index                  = 0;
    int32                           i;
    MN_DUAL_SYS_E                   dual_sys                    = MN_DUAL_SYS_1;
    ATC_STK_SETUP_CALL_FAIL_TYPE_E  fail_type = ATC_STK_CALL_SUCC;
    MN_CALL_DISCONNECT_CALL_CAUSE_E fail_cause = MN_CAUSE_ERROR_NONE;
    ATC_CMCC_CM_CALL_END_STATUS_E   end_status = ATC_CM_CALL_END_OFFLINE;
#ifdef _MUX_ENABLE_
    uint8   current_link_id = atc_config_ptr->current_link_id;
#endif

#ifdef _SUPPORT_CCBS_
    uint8 ccbs_index = 0xff;
#endif // #ifdef _SUPPORT_CCBS_

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_CALL_DISCONNECTED_IND:
            dual_sys    = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->dual_sys;
            call_index  = GetIndexOfCallContext(((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id, dual_sys);
            call_id     = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id;
            fail_type = ATC_STK_CALL_FAIL_DISC;
            fail_cause = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->disconnected_cause;   
            break;

        case APP_MN_CALL_ERR_IND:
            dual_sys    = ((APP_MN_CALL_ERR_IND_T *)sig_ptr)->dual_sys;
            call_index  = GetIndexOfCallContext(((APP_MN_CALL_ERR_IND_T *)sig_ptr)->call_id, dual_sys);
            call_id     = ((APP_MN_CALL_ERR_IND_T *)sig_ptr)->call_id;
            fail_type = ATC_STK_CALL_FAIL_ERROR;
            fail_cause = ((APP_MN_CALL_ERR_IND_T *)sig_ptr)->error_cause;
            break;

        default:
            ATC_TRACE_LOW("ATC: Assert Failure unknow disc code.");
            break;
    }

    if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        waitingcall_existed_b4_flag = TRUE;
    }

    if(ATC_INVALID_CALLID != call_index)
    {
        if(ATC_HOLD_IDLE == g_calls_state[dual_sys].call_context[call_index].hold_state)
        {
            ATC_ResetDTMFTimer();
            ATC_ResetDTMFVar(dual_sys);
            ATC_TRACE_LOW("ATC_ResetDTMFInfo: ATC_HOLD_IDLE, reset the duration list!");
        }
        else
        {
            ATC_TRACE_LOW("ATC_ResetDTMFInfo: ATC_CALL_HOLD, need not to reset the duration list!");
        }
        if(!atc_config_ptr->ath_flag && atc_config_ptr->cssu_flag[dual_sys])
        {
            if(g_calls_state[dual_sys].call_context[call_index].hold_state == ATC_CALL_HOLD &&
                    g_calls_state[dual_sys].call_context[call_index].call_entity_state == ATC_CALL_ACTIVE_STATE)
            {
                sprintf((char *)g_rsp_str, "+CSSU:5");
                ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
            }
        }

        if (s_atc_call_disc_cause == 0xffffffff)
        {
            atc_config_ptr->ceer_val = fail_cause;
        }
        else
        {
            atc_config_ptr->ceer_val = s_atc_call_disc_cause;
            s_atc_call_disc_cause = 0xffffffff;
        }
    
        atc_config_ptr->ceer_code[CEER_CALL_DISCONNECT] = atc_config_ptr->ceer_val;
        atc_config_ptr->ceer_last_type = CEER_CALL_DISCONNECT;

        ATC_UpDSCIInfo(dual_sys, atc_config_ptr, sig_ptr, call_index, 6);

        if(ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[call_index].call_mode)
        {
            is_voice_call = TRUE;
        }

        for(i = call_index + 1; i < g_calls_state[dual_sys].call_amount; i++)
        {
            g_calls_state[dual_sys].call_context[i -1] = g_calls_state[dual_sys].call_context[i];
        }

        SCI_MEMSET(&g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1], 0, sizeof(ATC_CALL_CONTEXT_T));
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].call_id = ATC_INVALID_CALLID;
        g_calls_state[dual_sys].call_amount--;
    }

    ATC_TRACE_LOW("ATC: ATC_ProcessCallDiscInd, call_amount:%d,g_atc_call_flag:%d,is_voice_call:%d",
                  g_calls_state[dual_sys].call_amount, g_atc_call_flag, is_voice_call);

    if((0 == g_calls_state[dual_sys].call_amount) && g_atc_call_flag)
    {
        uint8 temp_index;

        if( atc_config_ptr->audio_flag)
        {
            ATC_EnableVoiceCodec(FALSE);
            atc_config_ptr->audio_flag = FALSE;
        }
        g_atc_pcm_flag = FALSE;
        g_atc_call_flag = FALSE;

        temp_index =  (ATC_INVALID_CALLID == call_index) ? 0 : call_index;
        ATC_HandleMoMuteAndLocalRingState(dual_sys, sig_ptr->SignalCode, temp_index);  
    }

#ifndef ATC_SAT_ENABLE
    //If call is setup by STK.
    if(TRUE == g_is_stk_call[dual_sys])
    {
        ATC_StkProcessCallSignals(FALSE, dual_sys,call_id,fail_type,atc_config_ptr->ceer_val);
    }

    //stop the dtmf sending timer if any
    ATC_StkStopDtmfTimer(dual_sys,call_index);
#endif

    if(!CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        // no wait call, check & kill call exp timer
        if(SCI_IS_TIMER_ACTIVE(P_ATC, ATC_CALL_EXP_IND, &T_ATC_CALL_EXP_IND))
        {
            SCI_KILL_TIMER(P_ATC, ATC_CALL_EXP_IND,
                           &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
            g_atc_expire_call_id = 0xff;
            g_atc_expire_ind = FALSE;
        }
    }

    ATC_TRACE_LOW("ss state %d", atc_config_ptr->ss_operate[dual_sys]);
#ifdef _MUX_ENABLE_
    if ((atc_config_ptr->ss_operate[dual_sys] != ATC_ADD_HELD_TO_CONVERSATION) &&
        (atc_config_ptr->ss_operate[dual_sys] != ATC_HOLD_CALLS_AND_ACCEPT_OTHERS) &&
        (atc_config_ptr->ss_operate[dual_sys] != ATC_HOLD_ACTIVE_CALL_ONLY))
    {
        ATC_Clear_All_Exp_Result(call_id, ATC_DOMAIN_CS, dual_sys);
    }
#endif

    switch(atc_config_ptr->ss_operate[dual_sys])
    {
        case ATC_INVALID_OPERATE:

            if(!atc_config_ptr->ath_flag)
            {
                uint32 error_code = 0;
                if(APP_MN_CALL_DISCONNECTED_IND == sig_ptr->SignalCode &&
                    MN_CAUSE_ALERTING_NO_ANSWER == ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->disconnected_cause)
                {
                    error_code = S_ATC_RESULT_CODE_NO_ANSWER;
                }
                else
                {
                    error_code = S_ATC_RESULT_CODE_NO_CARRIER;
                }

                call_id = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id;
                if (s_is_self_discon[call_id])
                {
                    s_is_self_discon[call_id] = FALSE;
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
                }
                else
                {
                    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
                    ATC_BuildResultCodeRsp(atc_config_ptr, error_code, 0);
                    ATC_SET_CURRENT_LINK_ID_MUX(current_link_id);
                }

                sprintf((char *)g_rsp_str, "+ECEER: %lu,%d", atc_config_ptr->ceer_val, \
                        ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id + 1);

                ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, (uint8 *)g_rsp_str);

                //Send the unsolicited +SIND: 6
                ATC_UpSINDInfo(dual_sys,
                               atc_config_ptr,
                               (xSignalHeaderRec *)sig_ptr,
                               ATC_WIND_BIT5,
                               6,
                               (BOOLEAN)(0 != atc_config_ptr->atc_config_nv.creg));
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                atc_config_ptr->ath_flag = FALSE;

                end_status = ATC_CM_CALL_END_CLIENT_END;

            }

            atc_config_ptr->cpas = ATC_CPAS_STATUS_READY;
            break;

        case ATC_REL_HELD_CALLS_OR_SET_UDUB:

            if(waitingcall_existed_b4_flag == FALSE &&
                    CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            }

            break;

        case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_OTHERS:

            if(CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                // Determine if there is a waiting call
                if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
                {
                    ATC_TRACE_LOW("connect call id = %d", call_id);
                    MNCALL_ConnectCallEx(dual_sys, call_id, PNULL);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                                      (uint16)APP_MN_SETUP_COMPLETE_IND, call_id, ATC_DOMAIN_CS, dual_sys);
                }
                else if(CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
                {
                    ATC_TRACE_LOW("ATC: retrieve call id = %d", call_id);
                    MNCALL_RetrieveCallEx(dual_sys, call_id);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_CALL_RETRIEVED_CNF, call_id, ATC_DOMAIN_CS, dual_sys);
                }
                else
                {
                    ATC_TRACE_LOW("ATC: chld disconnect call success");
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                    atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
                }
            }
            break;

        case ATC_REL_SPECIFIC_X_CALL_BASE:
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            break;

        case ATC_REL_EXISTING_CALLS:
        case ATC_REL_SPECIFIC_EXISTING_X_CALL_BASE:
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            break;

        case ATC_REL_ACTIVE_CALLS_AND_ACCEPT_HELD_CALL:
            if(CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
            {
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  APP_MN_CALL_DISCONNECTED_IND, call_id, ATC_DOMAIN_CS, dual_sys);
            }
            else
            {
                if(CheckPresentHeldCall(&g_calls_state[dual_sys], &call_id))
                {
                    ATC_TRACE_LOW("ATC: retrieve call id = %d", call_id);
                    MNCALL_RetrieveCallEx(dual_sys, call_id);
                    ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                      APP_MN_CALL_RETRIEVED_CNF, call_id, ATC_DOMAIN_CS, dual_sys);
                }
                else
                {
                    ATC_TRACE_LOW("ATC: chld disconnect call success");
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                    atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
                }
            }
            break;

        case ATC_ADD_HELD_TO_CONVERSATION:
        case ATC_HOLD_CALLS_AND_ACCEPT_OTHERS:
        case ATC_HOLD_ACTIVE_CALL_ONLY:
            {
            #ifdef _MUX_ENABLE_
                BOOLEAN result = FALSE;
                uint16  SignalCode = 0xffff;
                uint8   tmp_link_id = atc_config_ptr->current_link_id;

                if (atc_config_ptr->ss_operate[dual_sys] == ATC_ADD_HELD_TO_CONVERSATION)
                {
                    SignalCode = APP_MN_BUILD_MPTY_CNF;
                }
                else
                {
                    SignalCode = APP_MN_CALL_HELD_CNF;
                }
            
                result = ATC_Receive_Expected_Event(atc_config_ptr, SignalCode,
                                    call_id, ATC_DOMAIN_CS, dual_sys);
                if (result == TRUE)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
                }
                else if (atc_config_ptr->ss_operate[dual_sys] == ATC_HOLD_CALLS_AND_ACCEPT_OTHERS)
                {
                    result = ATC_Receive_Expected_Event(atc_config_ptr, APP_MN_CALL_RETRIEVED_CNF,
                                                  call_id, ATC_DOMAIN_CS, dual_sys);
                    if (result == TRUE)
                    {
                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
                    }
                }

                atc_config_ptr->current_link_id = tmp_link_id;
                ATC_Clear_All_Exp_Result(call_id, ATC_DOMAIN_CS, dual_sys);
            #else
                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            #endif

                ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
                atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            }
            break;

        default:
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR, 0);
            atc_config_ptr->ss_operate[dual_sys] = ATC_INVALID_OPERATE;
            break;
    }


    if(APP_MN_CALL_DISCONNECTED_IND == sig_ptr->SignalCode)
    {
        if(end_status != ATC_CM_CALL_END_CLIENT_END || atc_config_ptr->ceer_val > MN_CAUSE_NORMAL_UNSPECIFIED)
        {
            end_status = ATC_CM_CALL_END_NETWORK_END;
        }

        if(atc_config_ptr->ceer_val > MN_CAUSE_INTERWORKING)
        {
            end_status = ATC_CM_CALL_END_NO_SRV;
        }
    }
    else
    {
        end_status = ATC_CM_CALL_END_CONF_FAILED;
    }

if(ATC_CM_CALL_END_CLIENT_END == end_status || ATC_CM_CALL_END_NETWORK_END == end_status|| ATC_CM_CALL_END_NETWORK_END == end_status)
    {
        sprintf((char *)g_rsp_str, "^CEND: %d,,%d,%d", ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id + 1, \
                end_status, (int)atc_config_ptr->ceer_val);
    }
    else
    {
        sprintf((char *)g_rsp_str, "^CEND: %d,,%d", ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id + 1, \
                end_status);
    }        

#ifdef _SUPPORT_CCBS_
    ccbs_index = atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index;
    call_id = ((APP_MN_CALL_DISCONNECTED_IND_T *)sig_ptr)->call_id;
    if ((call_id == atc_config_ptr->ccbs_info[dual_sys].cur_call_id)
        && (ccbs_index < ATC_MAX_CCBS_NUM + 1))
    {        
        atc_config_ptr->ccbs_info[dual_sys].cur_ccbs_index = 0xff;
        atc_config_ptr->ccbs_info[dual_sys].cur_call_id = 0xff;
    }
#endif // #ifdef _SUPPORT_CCBS_

    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, (uint8 *)g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_ACMMAX_IND signal.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessAcmInd(
    ATC_CONFIG_T          *atc_config_ptr,
    APP_MN_ACMMAX_IND_T  *sig_ptr 
)
{
    if(0 == g_calls_state[sig_ptr->dual_sys].call_amount)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_NO_CARRIER, 0);
    }

    atc_config_ptr->ceer_val = MN_CAUSE_ACM_EQ_OR_GT_ACMMAX;
    atc_config_ptr->ceer_code[CEER_CALL_DISCONNECT] = MN_CAUSE_ACM_EQ_OR_GT_ACMMAX;
    atc_config_ptr->ceer_last_type = CEER_CALL_DISCONNECT;
    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handles the ATC_CALL_EXP_IND signal
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessCallExpInd(
    ATC_CONFIG_T            *atc_config_ptr
)
{
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8 call_index;
    uint8 call_id;
    uint8 spauto_flag = 0;

    ATC_TRACE_LOW("ATC: enter the timer expire");

    spauto_flag = atc_config_ptr->atc_config_nv.spauto_flag;
    if(!g_atc_expire_ind)
    {
        ATC_TRACE_LOW("ATC: unexpected timer expire");
        return S_ATC_SUCCESS;
    }

    if(!CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id))
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCallExpInd, CheckPresentWaitCall failure!");
        g_atc_expire_call_id = 0xff;
        g_atc_expire_ind = FALSE;
        s_atc_call_ring_num = 0;
        return ERR_OPERATION_NOT_ALLOWED;
    }

    call_index = GetIndexOfCallContext(call_id, dual_sys);

    if(ATC_INVALID_CALLID == call_index)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessCallExpInd, invalid call index!");
        g_atc_expire_ind = FALSE;
        s_atc_call_ring_num = 0;
        return ERR_OPERATION_NOT_ALLOWED;
    }

    //show the prompting message
    if(0 == atc_config_ptr->crc)
    {
        sprintf((char *)g_rsp_str, "RING");
        s_atc_call_ring_num++;
    }
    else
    {
        //sprintf((char*)g_rsp_str, "+CRING: voice");
        if(g_calls_state[dual_sys].call_context[call_index].call_mode == ATC_DATA_MODE)
        {
            sprintf((char *)g_rsp_str, "+CRING: SYNC");
        }
        else
        {
            sprintf((char *)g_rsp_str, "+CRING: VOICE");
            s_atc_call_ring_num++;
        }
    }

    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
#ifndef ATC_SAT_ENABLE
    if(atc_config_ptr->atc_config_nv.clip_flag)
    {
        if(s_mt_calling_num.calling_num_present)
        {
            sprintf((char *)g_rsp_str, "%s: \"%s\",%d,\"\",0,,%d",
                    g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, s_tel_num_type,
                    s_mt_calling_num.calling_num.present_ind);

        }
        else
        {
            sprintf((char *)g_rsp_str, "%s: \"%s\",%d",
                    g_atc_info_table[AT_CMD_CLIP].cmd_name->str_ptr, s_tel_str, UNKNOWN_PLAN_UNKNOWN_TYPE);

        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
#endif

    if((0 != atc_config_ptr->s0_reg && s_atc_call_ring_num == atc_config_ptr->s0_reg) || (spauto_flag & 0x1))
    {
        if(MN_RETURN_SUCCESS != MNCALL_ConnectCallEx(dual_sys, call_id, NULL))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessCallExpInd, MNCALL_ConnectCall failure!");
            g_atc_expire_ind = FALSE;
            s_atc_call_ring_num = 0;
            return ERR_OPERATION_NOT_ALLOWED;
        }

        //SCI_KILL_TIMER(P_ATC,ATC_CALL_EXP_IND,&T_ATC_CALL_EXP_IND,"T_ATC_CALL_EXP_IND");
        g_atc_expire_ind = FALSE;
        s_atc_call_ring_num = 0;
        g_atc_call_flag = TRUE;

        if(ATC_VOICE_MODE == g_calls_state[dual_sys].call_context[call_index].call_mode)
        {
            s_mt_need_open_codec_flag |= CC_MT_CALL_CONNECTING;

            if((s_mt_need_open_codec_flag ==  (CC_MT_CALL_CONNECTING|CC_TCH_RESOURCE_ASSIGNED)) && 
                (!atc_config_ptr->audio_flag))
            {
                AUDIO_SetDevMode((AUDIO_DEVICE_MODE_TYPE_E)(atc_config_ptr->audio_mode));
                AUDIO_SetVolume(atc_config_ptr->speaker_volume);
                ATC_EnableUplinkMute(FALSE);
                ATC_EnableRemoteMute(FALSE); 
            
                ATC_EnableVoiceCodec(TRUE);
                s_mt_need_open_codec_flag = 0;
                atc_config_ptr->audio_flag = TRUE;
            }
        }
    }
    else
    {
        SCI_SET_TIMER(P_ATC, ATC_CLIP_INTERVAL, ATC_CALL_EXP_IND,
                      &T_ATC_CALL_EXP_IND, "T_ATC_CALL_EXP_IND");
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+CMUX command
//  Global resource dependence : none
//  Author:      Wang hui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCMUX)
{
    ATC_STATUS      status  = S_ATC_SUCCESS;

#ifdef _MUX_ENABLE_

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                cmux_sys_para.mux_mode = (MUX_STATUS_E)PARA1.num;

                if((cmux_sys_para.mux_mode != 0) && (cmux_sys_para.mux_mode != 1))
                {
                    status = ERR_OPERATION_NOT_SUPPORTED;
                    break;
                }

                if(PARAM2_FLAG)
                {
                    cmux_sys_para.subset = (MUX_FRAME_E)(PARA2.num);
                }
                else
                {
                    cmux_sys_para.subset = MUX_FRAME_UIH;
                }

                if(PARAM3_FLAG)
                {
                    cmux_sys_para.port_speed = (MUX_BAUDRATE_E)(PARA3.num);
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

                is_cmux_recv = TRUE;
                atc_config_ptr->atc_config_nv.mux_mode = 1;
                if (ATC_MUX_SINGLE_MAP == g_atc_mux_mode)
                {
                    atc_urc_link_id = 1;
                }
                else
                {
                    atc_config_ptr->current_link_id = ATC_GetMuxLinkIdFromSimId(g_current_card_id);
                }

                ATC_ClrCmdLine(ATC_MAX_LINK_ID);
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%lu,%d,%d,%d,%d,%d", g_atc_info_table[AT_CMD_CMUX].cmd_name->str_ptr, cmux_sys_para.mux_mode, cmux_sys_para.subset,
                    cmux_sys_para.port_speed, cmux_sys_para.N1, cmux_sys_para.T1, cmux_sys_para.N2, cmux_sys_para.T2, cmux_sys_para.T3, cmux_sys_para.K);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

#endif
    return status;
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//    Description : This function handle the AT+SCMUX command 
//    Global resource dependence : none
//    Author:      Feng.shi
//    Note: param1=0 means close mux, =3 means open mux and link 1 is sim1 AT, 
//        link 2 is sim2 AT,...,  link 5 is ppp link.
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSCMUX )
{
    ATC_STATUS           status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
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
            if (ATC_MUX_SINGLE_MAP == g_atc_mux_mode)
            {
                atc_urc_link_id = 1;
            }
            else
            {
                atc_config_ptr->current_link_id = ATC_GetMuxLinkIdFromSimId(g_current_card_id);
            }

            ATC_ClrCmdLine(ATC_MAX_LINK_ID);
        }
        else
        {
            status = ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case ATC_CMD_TYPE_READ:
        sprintf((char*)g_rsp_str, "%s: %d,%d,%d,%lu,%d,%d,%d,%d,%d",g_atc_info_table[AT_CMD_CMUX].cmd_name->str_ptr, cmux_sys_para.mux_mode,cmux_sys_para.subset,
            cmux_sys_para.port_speed,cmux_sys_para.N1,cmux_sys_para.T1,cmux_sys_para.N2,cmux_sys_para.T2,cmux_sys_para.T3,cmux_sys_para.K);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
 
    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }
#endif
    return status;
}
#endif

/*****************************************************************************/
//  Description : This function checks the DTMF string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckDtmfStr(
    uint8          length,
    uint8          *str
)
{
    int32              i;
    BOOLEAN            status = TRUE;

    for(i = 0; i < length; i++)
    {
        if((str[i] >= '0' && str[i] <= '9') ||
                (str[i] >= 'A' && str[i] <= 'D') ||
                str[i] == '*' || str[i] == '#')
        {
            ATC_TRACE_LOW("ATC: the dtmf value is %c", str[i]);
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
//  Description : This function checks the CLIR call string
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
    SCI_ASSERT(str != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(length != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(clir != NULL);/*assert verified: check null pointer*/

    if(*length <= 4)
    {
        return FALSE;
    }

    if(!memcmp(str, "*31#", 4))
    {
        *clir = MN_CALL_CLIR_SUPPRESSION;
        *length -= 4;
        memmove(str, str + 4, *length);

        return TRUE;
    }

    if(!memcmp(str, "#31#", 4))
    {
        *clir = MN_CALL_CLIR_INVOCATION;
        *length -= 4;
        memmove(str, str + 4, *length);

        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : This function checks the plmn string.
//  Global resource dependence : none
//  Author:      sunny
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CheckPlmnStr(
    uint8          length,
    uint8          *str
)
{
    int32              i;

    if((length < 4) || (length > 6))
    {
        return FALSE;
    }

    for(i = 0; i < length; i++)
    {
        if(str[i] < '0' || str[i] > '9')
        {
            ATC_TRACE_LOW("ATC:invalid plmn num %c", str[i]);
            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function handle the AT+SMMSWAP command for swapping between 
//                      single sim mux mode and multi sims mux mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSMMSWAP)
{
    ATC_STATUS  status = S_ATC_SUCCESS;
#ifdef _MUX_ENABLE_
    ATC_TRACE_LOW("ATC: ATC_ProcessSMMSWAP");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(!PARAM1_FLAG)
            {
                return ERR_INVALID_INDEX;
            }
            
            g_atc_mux_mode = PARA1.num;
            break;

        case ATC_CMD_TYPE_READ:
            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SMMSWAP].cmd_name->str_ptr,
                          g_atc_mux_mode);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }
#endif

    return status;
}

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
        ATC_TRACE_LOW("ATC: ATC_GetPlmnDetailedInfo, Invalid parameters");
        return NULL;
    }

    switch(QueryType)
    {
        case PLMN_TYPE_FULL_NAME:
            /* Full name is too long */
            length = strlen((char *)QueryData);

            if(length > ATC_MAX_FULL_OP_NAME)
            {
                ATC_TRACE_LOW("ATC: ATC_GetPlmnDetailedInfo, FULL_NAME is too long (%d)", length);
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
                ATC_TRACE_LOW("ATC: ATC_GetPlmnDetailedInfo, SHORT_NAME is too long (%d)", length);
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

/*****************************************************************************/
//  Description : This function restore the gsm  configs
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
void ATC_RestoreGsmConfig(
    ATC_CONFIG_T         *atc_config_ptr,
    MN_DUAL_SYS_E   dual_sys
)
{
    //set +COPS(<format>) default value when AT&F
    atc_config_ptr->cops[dual_sys].format = PLMN_FORMAT_NUMBERIC;
}

/*****************************************************************************/
//  Description : This function handle the APP_MN_DISCONNECTING_IND signal
//  Global resource dependence : none
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessDisconnectingInd(
    ATC_CONFIG_T         *atc_config_ptr,
    APP_MN_DISCONNECTING_IND_T    *sig_ptr
)
{
    s_is_self_discon[sig_ptr->call_id] = TRUE;

    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : Judge whether the call is disconnect by AP/Modem,the call may be disconnect by Modem
//  for the stk max redial duration is exceeded
//  Global resource dependence : none
//  Author:       Yongxia zhang
//  Note:
/*****************************************************************************/
BOOLEAN ATC_IsSelfDisc(uint8 call_id)
{
    if(call_id < MN_CALL_MAX_CALL_NUM)
    {
        return s_is_self_discon[call_id];
    }
    else
    {
        SCI_PASSERT(FALSE,("ATC::ATC_IsSelfDisc,call id is invalid is %d.",call_id)); /*assert verified*/
    }
}


/*****************************************************************************/
//  Description : This function is used to write duration to the list
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_WriteDTMFDuration(MN_DUAL_SYS_E   dual_sys, uint16 duration)
{
    if(s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration < ATC_MAX_DTMF_ENTRY)
    {
        uint8 write_index = s_atc_dtmf_duration_t[dual_sys].read_index + s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration;
        if(write_index >= ATC_MAX_DTMF_ENTRY)
        {
            write_index = 0;
        }
        s_atc_dtmf_duration_t[dual_sys].duration[write_index] = duration;
        s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration ++;
        return TRUE;
    }
    else
    {
        ATC_TRACE_LOW("ATC: the s_atc_dtmf_duration_t[%d] is full!", dual_sys);
        return FALSE;
    }
}
/*****************************************************************************/
//  Description : This function is used to read duration from the list
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL uint16 ATC_ReadDTMFDuration(MN_DUAL_SYS_E   dual_sys)
{
    if(0 < s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration)
    {
        uint16 duration = s_atc_dtmf_duration_t[dual_sys].duration[s_atc_dtmf_duration_t[dual_sys].read_index];
        s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration --;
        s_atc_dtmf_duration_t[dual_sys].read_index ++;
        if(s_atc_dtmf_duration_t[dual_sys].read_index >= ATC_MAX_DTMF_ENTRY)
        {
            s_atc_dtmf_duration_t[dual_sys].read_index = 0;
        }
        return duration;
    }
    else
    {
        ATC_TRACE_LOW("ATC: the s_atc_dtmf_duration_t[%d] is empty!", dual_sys);
        return 0;
    }
}

/*****************************************************************************/
//  Description : This function is used to RESET dtmf duration buffer
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL void ATC_ResetDTMFDuration(void)
{
    SCI_MEMSET(s_atc_dtmf_duration_t, 0, sizeof(s_atc_dtmf_duration_t));
    
}

/*****************************************************************************/
//  Description : This function is used to check whether the list is full
//  Global resource dependence : s_atc_dtmf_duration_t[MN_SYS_NUMBER]
//  Author: Haoming.Lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_IsDTMFDurationListFull(MN_DUAL_SYS_E   dual_sys)
{
    if(s_atc_dtmf_duration_t[dual_sys].count_of_cur_duration < ATC_MAX_DTMF_ENTRY)
    {
        return FALSE;
    }
    return TRUE;    
}

#ifdef _SUPPORT_CCBS_
/*****************************************************************************/
//  Description : This function store the ccbs information when receive ATA signal and return
//                the unsolicited result code.
//  Global resource dependence : none
//  Author:       Haoming.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_StoreCCBSCall(
                                                                  ATC_CCBS_INFO_T ccbs_info,
                                                                  MN_DUAL_SYS_E   dual_sys
                                                                  )
{
    if(g_calls_state[dual_sys].call_amount + 1 < MN_CALL_MAX_CALL_NUM)
    {
        g_calls_state[dual_sys].call_amount++;
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].direction = FALSE;
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].call_id = ccbs_info.cur_call_id;
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].call_entity_state = ATC_CALL_OUTGOING_STATE;
        g_calls_state[dual_sys].call_context[g_calls_state[dual_sys].call_amount - 1].call_mode = ATC_VOICE_MODE;

        s_is_self_discon[ccbs_info.cur_call_id] = FALSE;

        //set the initial value when call course starting
        s_atc_call_disc_cause = 0xffffffff;
        g_atc_call_need_disc = FALSE;    //reset the need_disc flag
        return S_ATC_SUCCESS;
    }
    else
    {
        ATC_TRACE_LOW("ATC: ATC_StoreCCBSCall:Assert Failure call_amount error %d.", g_calls_state[dual_sys].call_amount);
        return S_ATC_FAIL;
    }
}
#endif // #ifdef _SUPPORT_CCBS_

#ifdef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function used to handle the response when GPRS service 
//                was suspended.
//  Global resource dependence : none
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
void ATC_HandleGprsSuspendRsp(ATC_CONFIG_T *atc_config_ptr)
{
    uint8 temp_link_id = 0;
    uint8 link_id = 0;
    uint16 pdp_id = 0;
    uint32 i;

    if (atc_config_ptr->async_mode_flag)
    {
        return;
    }

    temp_link_id = atc_config_ptr->current_link_id;
    for (i = 0; i < MN_SYS_NUMBER; i++)
    {
        if(ATC_Get_Clean_Expected_Event((MN_DUAL_SYS_E)i, APP_MN_ACTIVATE_PDP_CONTEXT_CNF, ATC_DOMAIN_PS, &pdp_id, &link_id))
        {
            ATC_PDP_ACT_REQ_T          mn_info = {0};
            ATC_TRACE_LOW("ATC: return pdp result when enter call state, current link id = %d, link id = %d, pdp_id = %d.", atc_config_ptr->current_link_id, link_id, pdp_id);
            ATC_SET_CURRENT_LINK_ID_MUX(link_id);
            sprintf((char*)g_rsp_str, "+CME ERROR: %d", MN_GPRS_ERR_TEMPORARILY_BLOCKED);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str); //this error indicates APP try dial again
            mn_info.pdp_id[0] = pdp_id;
            MNGPRS_DeactivatePdpContextEx((MN_DUAL_SYS_E)i, FALSE, mn_info.pdp_id);
        }
        
        if(ATC_Get_Clean_Expected_Event((MN_DUAL_SYS_E)i, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, ATC_DOMAIN_PS, &pdp_id, &link_id))
        {
            ATC_TRACE_LOW("ATC: return pdp deact result when enter call state, current link id = %d, link id = %d, pdp_id = %d.", atc_config_ptr->current_link_id, link_id, pdp_id);
            ATC_SET_CURRENT_LINK_ID_MUX(link_id);
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
    }

    ATC_SET_CURRENT_LINK_ID_MUX(temp_link_id);

    return;
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

