/******************************************************************************
** File Name:      atc_stk.c                                                 *
** Author:                                                                   *
** DATE:           12/08/2003                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file contains the functions that handle the AT command
**                 regarding STK(SIM TOOLKIT) that stk pdu will transport to 
**                 application by atc for smartphone project
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** ------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                               *
** 12/08/2003     Liuhan           Create.                                   *
******************************************************************************/
#include "atc_stk.h"
#include "atc_common.h"
#include "atc_gsm_ss.h"
#include "sig_code.h"
#include "env_atc_signal.h"
#include "atc_stk_adapt.h"
#include "sci_codec.h"

//#include "sim_to_simat.h"


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
{
#endif

#define E_SIMAT_DISPLAY_TYPE_SMS     0
#define E_SIMAT_DISPLAY_TYPE_USSD   1
#define E_SIMAT_DISPLAY_TYPE_SS        2
#define E_SIMAT_DISPLAY_TYPE_DTMF   3

/**---------------------------------------------------------------------------*
**                         Declare Variable                                  *
**---------------------------------------------------------------------------*/
static STK_SINAL_T        s_stk_signal[MN_SYS_NUMBER] = {0};
static BOOLEAN            s_stk_is_active[MN_SYS_NUMBER] = {0};
static BOOLEAN            s_ap_is_active[MN_SYS_NUMBER] = {0};
static BOOLEAN            s_stk_is_pending[MN_SYS_NUMBER] = {0};
static STK_SIGNAL_CACHE_LIST_T s_stk_signal_cache[MN_SYS_NUMBER] = {0};
#ifdef _MUX_ENABLE_
static uint8              s_stk_link_id[MN_SYS_NUMBER] = {0};    
#endif
static BOOLEAN            s_stk_is_refresh_reset[MN_SYS_NUMBER] = {0};
static ATC_STK_REDIAL_INFO_T s_stk_redial_info[MN_SYS_NUMBER] = {0};

extern const ATC_INFO_T   g_atc_info_table[];
extern RSP_RESULT_STR     g_rsp_str;
extern MN_DUAL_SYS_E      g_current_card_id; 
static uint32               s_cmd_type[MN_SYS_NUMBER];
static uint32               s_result[MN_SYS_NUMBER];

BOOLEAN                   g_is_stk_sendsms[MN_SYS_NUMBER] = {0};

BOOLEAN                   g_is_stk_sendussd[MN_SYS_NUMBER] = {0};
BOOLEAN                   g_is_stk_sendss[MN_SYS_NUMBER] = {0};
BOOLEAN                   g_is_stk_refresh_reset[MN_SYS_NUMBER] = {0};
BOOLEAN                   g_is_stk_call[MN_SYS_NUMBER] = {0};

#ifdef _MUX_ENABLE_
extern  ATC_MUX_MOD_E g_atc_mux_mode;
#endif

static BOOLEAN           s_is_setup_menu_cnf[MN_SYS_NUMBER];
static uint8                  s_cmd_stag[MN_SYS_NUMBER];
static uint8                s_mainmenu_selitem[MN_SYS_NUMBER];
static uint32               s_first_stk_cmd[MN_SYS_NUMBER];

extern BOOLEAN g_atc_call_flag;
extern BOOLEAN g_atc_call_need_disc;

extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];

char                     *g_stk_terminal_profile[AT_STK_TRP_NUM_MAX] =
{
    "160060C01F000000000000000000000000",
    "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
};


const char*       s_stk_prompt_str[AT_STK_STR_MAX] =
{
	"Send Short Message",
	"Send SS",
	"Send USSD"
};

SCI_TMID                 T_ATC_STSF_EXP_IND[MN_SYS_NUMBER] = {PNULL};


ATC_STK_CALL_INFO_T g_stk_call_info[MN_SYS_NUMBER] = {0};
STATIC SCI_TMID         s_stk_dtmf_send_timer[MULTI_SYS_NUM] = {NULL};
STATIC SCI_TMID         s_stk_max_redial_duration_timer[MULTI_SYS_NUM] = {NULL};
STATIC SCI_TMID         s_stk_redial_timer[MULTI_SYS_NUM] = {NULL};

#ifdef ONTIM_PRJ
ATC_REFRESH_FILE_T     s_stk_refresh[MULTI_SYS_NUM] = {0};
#endif

// stk functions supported by modem
static const uint8 modem_supprot_profile[STK_PROFILE_CHAR_LEN] =
{

    0x7F,
    0xFF,
    0xFF,

    0xFF,
    0xFF,
    0x0F,

    0x0F,
    0xFF,
    0xFF,

    0x03,
    0xFF,
    0x1F,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00
};

// stk functions supported by modem,but ap does't care
static const uint8 ap_discard_profile[STK_PROFILE_CHAR_LEN] =
{
    0x76,
    0x1E,
    0x68,

    0xC0,
    0x9E,
    0x00,

    0x00,
    0x87,
    0x1C,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00,

    0x00,
    0x00,
    0x00
};

/*****************************************************************************/
//  Description : This function handles the item menu selection of AT+ESATENVECMD command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EnvelopCmd_MenuSelection(
    SIMAT_MENU_SELECTION_IND_T *menu_select_ptr,
    MN_DUAL_SYS_E   dual_sys 
);

/*****************************************************************************/
//  Description : This function handle the ATC_ActualProfileData
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProfileToNVData(
        uint8 *profile_data, 
        uint16 profile_len, 
        uint8 *profile_to_nv, 
        uint16 *profile_to_nv_len
        );
/*****************************************************************************/
//  Description : This function handle the ATC_ActualProfileData
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProfileResultData(
        uint8 *profile_data, 
        uint16 profile_len, 
        uint8 *result_profile, 
        uint16 *result_profile_len
        );

/*****************************************************************************/
//  Description : ATC_BuildProfileRsp
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildProfileRsp(
    ATC_CONFIG_T *atc_config_ptr,
    uint8 *data,
    uint16 len
);

/*****************************************************************************/
//  Description : ATC_BuildProfileRsp
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessSTKSignalPending(
    ATC_CONFIG_T       *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E   dual_sys
);

/*****************************************************************************/
//  Description : ATC_BuildProfileRsp
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessSTKSignalCache(
    ATC_CONFIG_T       *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
);


/*****************************************************************************/
//  Description : ATC_ESimAtDisplay
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  ATC_ESimAtDisplay(
    ATC_CONFIG_T       *atc_config_ptr,
    uint8                       display_type,
    uint8                 is_alpha_id,
    uint8                 is_icon_id,
    SIMAT_DO_ALPHA_ID_T *alpha_id_ptr,
    SIMAT_DO_ICON_ID_T  *icon_id_ptr
);
/*****************************************************************************/
//  Description : process when received the refresh ind signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessRefreshInd(
    ATC_CONFIG_T *atc_config_ptr
);
/*****************************************************************************/
//  Description : process when received the refresh cnf signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessRefreshCnf(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);
/*****************************************************************************/
//  Description : process when received the send data signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessSendData(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);
/*****************************************************************************/
//  Description : process when received the receive data signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessReceiveData(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);
/*****************************************************************************/
//  Description : process when received the launch browser signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessLaunchBrowser(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);
/*****************************************************************************/
//  Description : process when received the open channel signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessOpenChannel(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);
/*****************************************************************************/
//  Description : process when received the close channel signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessCloseChannel(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : process when received the get channel status ind signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessGetChannelStatusInd(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
);

/*****************************************************************************/
//  Description : it is uesd by UEIT to do refresh cmd like AP/MMI
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessRefreshCmd(
    ATC_CONFIG_T *atc_config_ptr,
    SIMAT_REFRESH_T *refresh_ptr,
    MN_DUAL_SYS_E  dual_sys
);

LOCAL int ATC_SignalCacheSignalNum(MN_DUAL_SYS_E dual_sys);

/**********************************************************************
//  Description:
//     start mo sms retry timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartDtmfTimer(MN_DUAL_SYS_E dual_sys,
                               uint32 timer_value);

/**********************************************************************
//  Description:
//     handle dtmf timer exp
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKDTMFTimerExp(uint32 lParam);

/*****************************************************************************/
//  Description : check whether the dtmf is present in the stk setup call string
//  Global resource dependence : 
//  Author: yongxia.zhang
//  Note:
/*****************************************************************************/
void ATC_CheckSTKCallNum(MN_CALLED_NUMBER_T *called_num,
                                    ATC_STK_CALL_INFO_T  *stk_call_info);

/**********************************************************************
//  Description:
//     send dtmf char
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKSendDtmf(MN_DUAL_SYS_E dual_sys);
    
/**********************************************************************
//  Description:
//     get the detail fail cause of sending sms failed
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_GetDetailSmsError(MN_DUAL_SYS_E dual_sys, SIMAT_SEND_SMS_CNF_T  *send_sms_cnf_ptr);
/**********************************************************************
//  Description:
//      the function conv the dttz from bcd to decimal,because the interface for atc and L4 is decimal,but the data
//    from the AP is BCD
//  Global resource dependence :
//  Author:Yongxia Zhang
//  Note:
***********************************************************************/
LOCAL void ATC_ConvDttz(SIMAT_DO_DT_TZ_T * dttz_ptr);

/**********************************************************************
//  Description:
//     start mo sms retry timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartMaxRedialDurationTimer(MN_DUAL_SYS_E dual_sys,uint32 timer_value);

/**********************************************************************
//  Description:
//     handle stk setup call max redial duration timer expired
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKMaxRedialDurationTimerExp(uint32 lParam);

/**********************************************************************
//  Description:
//     stop the setup call max redial duration timer 
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkStopMaxRedialDurationTimer(MN_DUAL_SYS_E dual_sys);

/**********************************************************************
//  Description:
//     start setup call redial timer when the stk setup call is not sucessfully
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartRedialTimer(MN_DUAL_SYS_E dual_sys,uint32 timer_value);

/**********************************************************************
//  Description:
//     handle dtmf timer exp
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKRedialTimerExp(uint32 lParam);

/**********************************************************************
//  Description:
//     stop the dtmf timer when call disconnected
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkStopRedialTimer(MN_DUAL_SYS_E dual_sys);

/**********************************************************************
//  Description:
//     judge whether need redial
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsNeedRedial(
                            MN_DUAL_SYS_E                       dual_sys,
                            ATC_STK_SETUP_CALL_FAIL_TYPE_E      fail_type,
                                MN_CALL_DISCONNECT_CALL_CAUSE_E     fail_cause,
                                uint8 *                             redial_category);

/**********************************************************************
//  Description:
//     judge whether call failed cause is the need redial cause
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsNeedRedialCause(
                                MN_DUAL_SYS_E                       dual_sys,
                                ATC_STK_SETUP_CALL_FAIL_TYPE_E      fail_type,
                                    MN_CALL_DISCONNECT_CALL_CAUSE_E     fail_cause,
                                    uint8 *                             fail_category);

/**********************************************************************
//  Description:
//     judge whether call failed cause is the need redial cause
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void GetRedialTimerVal(
                                MN_DUAL_SYS_E   dual_sys,
                                uint8           redial_category,
                                uint32          *timer_val);

/**********************************************************************
//  Description:
//     judge whether need redial,if so,then save call parameter and start redial max duration timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkPrepareRedial(MN_DUAL_SYS_E dual_sys);

/**********************************************************************
//  Description:
//     judge whether the call is disconnect by AP or Modem. the call may be disconnect by AP,In this case
//   the terminal response to the SIM is 0x23(user cleared down call before connection or network release)
//    If the call is disc for the stk max redial duration is exceeded,in this case the terminal response to the SIM is 0x21
//  (network currently unable to process command)
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsDiscByAP(MN_DUAL_SYS_E dual_sys, uint8 call_id);

/**********************************************************************
//  Description:
//     judge whether the call type is need redial type
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsRedialCallType(SIMAT_SETUP_CALL_TYPE_E call_type);

/**********************************************************************
//  Description:
//     free the setup call raw apdu,it is saved for setup call redial
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void FreeSetupCallRawApduPtr(MN_DUAL_SYS_E dual_sys);
/**********************************************************************
//  Description:
//     clear the redial global variable and stop related timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ClearRedialInfo(MN_DUAL_SYS_E dual_sys);

#ifdef ONTIM_PRJ
/*****************************************************************************/
//  Description : Send the response of refresh info. and result
//  Global resource dependence : s_stk_signal
//  Author: Eddie.Wang
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildRefreshCnf(
    ATC_CONFIG_T *atc_config_ptr,
    ATC_REFRESH_FILE_T *refresh_ptr
);

/*****************************************************************************/
//  Description : Decode the refresh command
//  Global resource dependence : s_stk_signal
//  Author: yongxia zhang
//  Note:
/*****************************************************************************/
LOCAL void ATC_DecodeRefreshFileList(
    APP_MN_SIMAT_REFRESH_IND_T *param_ptr,
    ATC_REFRESH_FILE_T *refresh_ptr
);
#endif

/*****************************************************************************/
//  Description : ATC_ESimAtDisplay
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void  ATC_ESimAtDisplay(
    ATC_CONFIG_T       *atc_config_ptr,
    uint8                       display_type,
    uint8                     is_alpha_id,
    uint8                         is_icon_id,
    SIMAT_DO_ALPHA_ID_T *alpha_id_ptr,
    SIMAT_DO_ICON_ID_T  *icon_id_ptr
)
{
    uint32 str_len;
    char  *str_ptr = PNULL;
    uint8 alpha_id = 1;
    uint8 self_explanatory = 1;


    if(PNULL == atc_config_ptr || PNULL == alpha_id_ptr || PNULL == icon_id_ptr)
    {
        return ;
    }

    if(0 == is_alpha_id)
    {
        alpha_id_ptr->len = 1;
        alpha_id_ptr->id_ptr[0] = ' ';
        alpha_id = 0;
    }

    /* Check the length of string. */
    str_len = alpha_id_ptr->len;

    if(str_len > STK_MAX_UCS2_STR_LEN)
    {
        str_len = STK_MAX_UCS2_STR_LEN;
    }

    //allocate the memory
    str_ptr = SCI_ALLOC_BASE((str_len + 1) * 2);
    /* change the uint8 to assci format */
    SCI_MEMSET(str_ptr, 0, ((str_len + 1) * 2));

    ATC_TranUint8IntoChar(alpha_id_ptr->id_ptr,
                          str_len, (uint8 *)str_ptr);

    if(1 == is_icon_id)
    {

        if(icon_id_ptr->is_self_explanatory)
        {
            self_explanatory = 0;
        }

        sprintf((char *)g_rsp_str, "+ESIMATDISPLAY: %d,%d,%d,%s,%d,%d",
                display_type,
                alpha_id,
                1,
                str_ptr,
                self_explanatory,
                icon_id_ptr->icon_id);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        sprintf((char *)g_rsp_str, "+ESIMATDISPLAY: %d,%d,%d,%s, , ",
                display_type,
                alpha_id,
                0,
                str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }

    SCI_FREE(str_ptr);

}

/*****************************************************************************/
//  Description : ATC_InitSimStkVar
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
void ATC_InitSimStkVar(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MN_SYS_NUMBER)
    {
        return;
    }

    g_is_stk_sendsms[dual_sys]          = FALSE;
    g_is_stk_call[dual_sys]             = FALSE;
    g_is_stk_sendussd[dual_sys]         = FALSE;
    g_is_stk_sendss[dual_sys]           = FALSE;
    g_is_stk_refresh_reset[dual_sys]    = FALSE;

    s_is_setup_menu_cnf[dual_sys]        = FALSE;
    s_cmd_stag[dual_sys]                        = ATC_CMD_INIT;
    s_mainmenu_selitem[dual_sys]      = 0;
    s_first_stk_cmd[dual_sys]   = 0;
    s_cmd_type[dual_sys]              = 0;
    s_result[dual_sys]                = 0;

    s_stk_is_active[dual_sys] = FALSE;
#ifdef _SUPPORT_IPC_
    s_ap_is_active[dual_sys]  = TRUE;//It seems that IPC will not send AT+SPUSATPROFILE?
#else
    s_ap_is_active[dual_sys]  = FALSE;
#endif
    s_stk_is_pending[dual_sys] = FALSE;

    SCI_MEMSET(&s_stk_signal[dual_sys], 0, sizeof(STK_SINAL_T));
    SCI_MEMSET(&s_stk_redial_info[dual_sys], 0, sizeof(ATC_STK_REDIAL_INFO_T));
#ifdef ONTIM_PRJ
    SCI_MEMSET(&s_stk_refresh[dual_sys],0,sizeof(ATC_REFRESH_FILE_T));
#endif

    s_stk_signal_cache[dual_sys].read = 0;
    s_stk_signal_cache[dual_sys].write = 0;
    s_stk_signal_cache[dual_sys].size = STK_MAX_CACHE_SIGNAL_NUM;
    SCI_MEMSET(&(s_stk_signal_cache[dual_sys].sig_list[0]), 0, sizeof(STK_SIGNAL_CACHE_T) * STK_MAX_CACHE_SIGNAL_NUM);

    SCI_MEMSET(&(g_stk_call_info[dual_sys]),0,sizeof(ATC_STK_CALL_INFO_T));
    g_stk_call_info[dual_sys].call_index = ATC_INVALID_CALLID; 
}

/*****************************************************************************/
//  Description : This function initializes the static variables of phonebook
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_InitStkVar(
    ATC_CONFIG_T        *atc_config_ptr
)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    for (i = MN_DUAL_SYS_1; i < MN_SYS_NUMBER; i++)
    {
        ATC_InitSimStkVar(i);
    }
}

/*****************************************************************************/
//  Description : This function send the response to sim if STK is activated.
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_InitCnfSetMenu(MN_DUAL_SYS_E dual_sys)
{
    //Send the respond if stk_actflag==TRUE
    SIMAT_SETUP_MENU_CNF_T  smenu;
    SCI_MEMSET(&smenu, 0x0, sizeof(SIMAT_SETUP_MENU_CNF_T));
    smenu.result = SIMAT_TR_SUCCESS;
    SIMAT_CnfSetMenuEx(dual_sys, &smenu);
    ATC_TRACE_LOW("ATC:ATC_InitCnfSetMenu");
    s_is_setup_menu_cnf[dual_sys] = FALSE;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : process when received the setup menu signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildProactiveRsp(
    MN_DUAL_SYS_E  dual_sys,
    ATC_CONFIG_T *atc_config_ptr,
    uint8 *data,
    uint16 len
)
{
    uint8   *temp_ptr = NULL;
    uint16   length = 0;

    temp_ptr = g_rsp_str;
    /*report +SPUSATPROCMDIND:<string>*/
    sprintf((char*)g_rsp_str, "+SPUSATPROCMDIND:");
    length = strlen((char *)g_rsp_str);
    temp_ptr = &g_rsp_str[length];
    //add <string>
    ATC_TranUint8IntoChar(data, len, (uint8 *)temp_ptr);

    length += len * 2;
    SCI_ASSERT(length < MAX_ATC_RSP_LEN);/*assert verified: check valid value*/
    g_rsp_str[length] = '\0';

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    s_stk_is_pending[dual_sys] = TRUE;  
}

/*****************************************************************************/
//  Description : process when received the setup menu signal
//  Global resource dependence : s_stk_signal
//  Author: Liuhan
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildSimatDisplayRsp(
                                ATC_CONFIG_T *atc_config_ptr,
                                uint8 *data,
                                uint16 len
                                )
{
    uint8   *temp_ptr = NULL;
    uint16   length = 0;

    temp_ptr = g_rsp_str;
    /*report +SPUSATDISPLAY:<string>*/
    sprintf((char*)g_rsp_str, "+SPUSATDISPLAY:");
    length = strlen((char*)g_rsp_str);
    temp_ptr = &g_rsp_str[length];
    //add <string>
    ATC_TranUint8IntoChar(data,len,(uint8 *)temp_ptr);
    
    length += len * 2;
    SCI_ASSERT(length < MAX_ATC_RSP_LEN);/*assert verified: check valid value*/
    g_rsp_str[length] = '\0';

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}

/*****************************************************************************/
//  Description : process when received the setup menu signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
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
//  Description : process when received the setup menu signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
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
    SIMAT_SETUP_MENU_T *mainmenu_ptr = &(s_stk_signal[g_current_card_id].setup_menu);

    // the title of main menu
    menu_len = mainmenu_ptr->alpha_id.len;
    SCI_TRACE_LOW("ATC: setupmenu menu_len = %d", menu_len);
    if (0 < menu_len)
    {
        ucs2_code = mainmenu_ptr->alpha_id.id_ptr[0];
		SCI_TRACE_LOW("ATC: setupmenu ucs2_code  = %d", ucs2_code);
        if (ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //Change to ascii format.
            ATC_TranUint8IntoChar(mainmenu_ptr->alpha_id.id_ptr, menu_len, menu_str);
        }
        else
        {
            SCI_MEMCPY(menu_str, mainmenu_ptr->alpha_id.id_ptr, menu_len);
            menu_str[menu_len] = '\0';
        }
		sprintf((char*)g_rsp_str,"%s:\"%s\"",
			    g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr, 
			    menu_str);
	    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    SCI_TRACE_LOW("ATC: setupmenu object num = %d", mainmenu_ptr->item.item_data_object_num);
    
    //Show the item of main menu
    for (i=0; i<mainmenu_ptr->item.item_data_object_num; i++)
    {
        start_pos = mainmenu_ptr->item.item_data_object_info[i][2];
        menu_len  = mainmenu_ptr->item.item_data_object_info[i][1];
        ucs2_code = mainmenu_ptr->item.item_data_object_buf[start_pos];
        if (ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
        {
            //change UCS2 code to ascii code.			
            ATC_TranUint8IntoChar(&(mainmenu_ptr->item.item_data_object_buf[start_pos]),menu_len, menu_str);
        }
        else
        {
            SCI_MEMCPY(menu_str, &(mainmenu_ptr->item.item_data_object_buf[start_pos]), menu_len);
        
        }
    	SCI_TRACE_LOW("ATC: setupmenu object num = %d,  ucs2_code = %d", menu_len, ucs2_code);
    	
    	sprintf((char*)g_rsp_str, "%s:%ld,%d,\"%s\",%d",
    		    g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		    i+1, mainmenu_ptr->item.item_data_object_num, 
                menu_str,mainmenu_ptr->help_info);
    	ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }	
}

/*****************************************************************************/
//  Description : process when received the display text signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessDisplayText(
    ATC_CONFIG_T *atc_config_ptr
)
{
    BOOLEAN  user_clear;     //wait for the user to clear the message or clear the message
    BOOLEAN  is_ir;          //is this command a immediate response
    uint16   str_len = 0;
    uint8    * str_ptr = PNULL;
    SIMAT_DISPLAY_TEXT_CNF_T display_text;	
    uint16   data_length = 0;
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;

	user_clear = s_stk_signal[dual_sys].item.display_text.user_clear;
	is_ir = s_stk_signal[dual_sys].item.display_text.is_ir; 
	//If command is a immediate response,handle it at once.
	if(is_ir)
	{
           display_text.result = SIMAT_TR_SUCCESS;		
           SIMAT_CnfDisplayTextEx(dual_sys, &display_text);  
	}
	//Get the text from s_stk_signal[g_current_card_id]
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
		sprintf((char*)g_rsp_str, "%s:%d,\"%s\",%d",
			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
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
			SCI_TRACE_LOW("ATC: error length in simat.");
		}
		
		sprintf((char*)g_rsp_str, "%s:%d,\"%s\",%d",
			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
			s_stk_signal[dual_sys].item.display_text.high_priority,
			(char*)str_ptr,user_clear);
		SCI_FREE(str_ptr);
		break;

    case STK_UCS2_ALPHABET:
		//SCI_TRACE_LOW("ATC:ATC_ProcessDisplayText:STK_UCS2_ALPHABET");
		//allocate the memory
		str_ptr = SCI_ALLOC_BASE((str_len+1)*2);
		
		SCI_MEMSET(str_ptr,0,((str_len+1)*2));

		ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.display_text.text_str.text_ptr,
			str_len,str_ptr);
		sprintf((char*)g_rsp_str, "%s:%d,\"80%s\",%d",
			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
			s_stk_signal[dual_sys].item.display_text.high_priority,str_ptr,user_clear);
		SCI_FREE(str_ptr);
        break;

    default:
		SCI_ASSERT(FALSE);
        break;
    }
	//Show the text
	ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

}

/*****************************************************************************/
//  Description : process when received the get inkey signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessGetInkey(
    ATC_CONFIG_T *atc_config_ptr
)
{
    uint16   str_len;
    uint8*   str_ptr = NULL;
    uint16   data_length = 0;
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	
    //check the length of string
    str_len = s_stk_signal[dual_sys].item.get_inkey.text_str.len;
    if(str_len < 1)
    {
    	str_len = 1;
    }
    str_ptr = (uint8*)SCI_ALLOC_BASE((str_len+1)*2);
    SCI_ASSERT(str_ptr != NULL);
    SCI_MEMSET(str_ptr, 0, ((str_len+1)*2));
	
    //Copy the text according to its dcs
    switch(s_stk_signal[dual_sys].item.get_inkey.text_str.dcs)
    {
    case STK_DEFAULT_ALPHABET:
    case STK_UNPACKED_FORMAT:
    	//SCI_TRACE_LOW("ATC:ATC_ProcessGetInkey:STK_DEFAULT_ALPHABET");
    	sprintf((char*)g_rsp_str, "%s:%d,%d,\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.get_inkey.inkey_type,s_stk_signal[dual_sys].item.get_inkey.help_info,
    		(char*)s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr);
        break;
    
    case STK_PACKED_FORMAT:		
    	data_length = SIMAT_UnpackSmsAlphabet(s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr, 
    		str_len, str_ptr, STK_MAX_STR_LEN);
    	if(data_length > STK_MAX_STR_LEN)
    	{
    		SCI_TRACE_LOW("ATC: error data length in simat");
    	}
    	
    	sprintf((char*)g_rsp_str, "%s:%d,%d,\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.get_inkey.inkey_type,s_stk_signal[dual_sys].item.get_inkey.help_info,
    		(char*)str_ptr);
    	break;
    
    case STK_UCS2_ALPHABET:
    	//SCI_TRACE_LOW("ATC:ATC_ProcessGetInkey:STK_UCS2_ALPHABET");
    	ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.get_inkey.text_str.text_ptr,
    		str_len,str_ptr);
    	sprintf((char*)g_rsp_str, "%s:%d,%d,\"80%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.get_inkey.inkey_type,
    		s_stk_signal[dual_sys].item.get_inkey.help_info,
    		str_ptr);
        break;
    
    default:
    	SCI_ASSERT(FALSE);
        break;
    }
    
    SCI_FREE(str_ptr);
    
	//Show the text
	ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	SCI_TRACE_LOW("ATC:ATC_ProcessGetInkey,%d,%d",
		s_stk_signal[dual_sys].item.get_inkey.text_str.dcs,str_len);

}

/*****************************************************************************/
//  Description : process when received the get input signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
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
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	
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
    SCI_TRACE_LOW("ATC: process get input dcs = %d, str_len = %d",dcs,str_len);
    
    str_ptr = (uint8*)SCI_ALLOC_BASE((str_len+1)*2);
    SCI_ASSERT(str_ptr != NULL);
    SCI_MEMSET(str_ptr, 0, ((str_len+1)*2));
    
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
    
    		sprintf((char*)g_rsp_str, "%s:%d,%d,%d,%d,%d,\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,s_stk_signal[dual_sys].item.get_input.input_type,
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
    		SCI_TRACE_LOW("ATC: error length in simat.");
    	}
    	
    	sprintf((char*)g_rsp_str, "%s:%d,%d,%d,%d,%d,\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,s_stk_signal[dual_sys].item.get_input.input_type,
    		s_stk_signal[dual_sys].item.get_input.input_echo,s_stk_signal[dual_sys].item.get_input.response_len.min_len,
    		s_stk_signal[dual_sys].item.get_input.response_len.max_len,s_stk_signal[dual_sys].item.get_input.help_info,
    		(char*)str_ptr);
    	break;
    
    case STK_UCS2_ALPHABET:
    	//SCI_TRACE_LOW("ATC:ATC_ProcessGetInput:STK_UCS2_ALPHABET");
    	//change the uint8 to ASCII format
    	ATC_TranUint8IntoChar(text_ptr, str_len, str_ptr);
    	sprintf((char*)g_rsp_str, "%s:%d,%d,%d,%d,%d,\"80%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.get_input.input_type,
    		s_stk_signal[dual_sys].item.get_input.input_echo,s_stk_signal[dual_sys].item.get_input.response_len.min_len,
    		s_stk_signal[dual_sys].item.get_input.response_len.max_len,s_stk_signal[dual_sys].item.get_input.help_info,str_ptr);
        break;
    
    default:
    	SCI_ASSERT(FALSE);
        break;
    }
    
    SCI_FREE(str_ptr);
    
    //Show the text
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}
/*****************************************************************************/
//  Description : process when received the setup call signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessSetupCall(
    ATC_CONFIG_T *atc_config_ptr
)
{
    uint8   tel_str[MN_MAX_ADDR_BCD_LEN*2+1] = {0};
    uint8   tel_sub_addr[MN_MAX_ADDR_BCD_LEN*2+1] = {0};
    uint8   tel_num_len;
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	
    SCI_TRACE_LOW("ATC: enter ATC_ProcessSetupCall");
    
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
    	sprintf((char*)g_rsp_str,"%s:%d,\"%s\",\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.setup_call.call_parameter.call_type,tel_str,tel_sub_addr);
    }
    //No sub address.
    else
    {
    	sprintf((char*)g_rsp_str,"%s:%d,\"%s\"",
    		g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    		s_stk_signal[dual_sys].item.setup_call.call_parameter.call_type,tel_str);
    }
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}

/*****************************************************************************/
//  Description : process when received the select item signal
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
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
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	
    s_stk_signal[dual_sys].menu_item_num = 0;
    SCI_MEMSET(s_stk_signal[dual_sys].menu_item_id,0,STK_MAX_MENU_ITEM_NUM);

    //Show the title
    if(s_stk_signal[dual_sys].item.select_item.is_alpha_id)
    {
    	//SCI_TRACE_LOW("ATC:select_item.is_alpha_id==TRUE");
    	string_length = s_stk_signal[dual_sys].item.select_item.alpha_id.len;
    	if(string_length < 1)
    	{
    		string_length = 1;
    	}
    	
    	str_ptr = (uint8*)SCI_ALLOC_BASE((string_length+1)*2);
    	SCI_ASSERT(str_ptr != NULL);
    	SCI_MEMSET(str_ptr, 0, ((string_length+1)*2));
    
    	ucs2_code = s_stk_signal[dual_sys].item.select_item.alpha_id.id_ptr[0];
    	if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
    	{
    		//SCI_TRACE_LOW("ATC:select_item.is_alpha_id.id_ptr[0]==ATC_UCS2_CODE");
    		//change the uint8 to ASCII format
    		ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.select_item.alpha_id.id_ptr,
    			string_length,str_ptr);
    		sprintf((char*)g_rsp_str,"%s:%d,\"%s\"",
    			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    			s_mainmenu_selitem[dual_sys],str_ptr);
    	}
    	else
    	{
    		//SCI_TRACE_LOW("ATC:select_item.is_alpha_id.id_ptr[0]!=ATC_UCS2_CODE");
    		sprintf((char*)g_rsp_str,"%s:%d,\"%s\"",
    			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
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
		
		str_ptr = (uint8*)SCI_ALLOC_BASE((string_length+1)*2);
		SCI_ASSERT(str_ptr != NULL);
		SCI_MEMSET(str_ptr, 0, ((string_length+1)*2));

		ucs2_code = item_ptr.item_data_object_buf[last_item_offset + last_item_length];
		if(ATC_UCS2_80_CODE == ucs2_code || ATC_UCS2_81_CODE == ucs2_code || ATC_UCS2_82_CODE == ucs2_code)
		{
			//change the uint8 to ASCII format
			ATC_TranUint8IntoChar(&item_ptr.item_data_object_buf[last_item_offset + last_item_length],
				string_length,str_ptr);
			sprintf((char*)g_rsp_str,"%s:%d,%d,\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				i+1,item_ptr.item_data_object_num,str_ptr);
		}
		else
		{
			SCI_MEMCPY(str_ptr,&item_ptr.item_data_object_buf[last_item_offset + last_item_length],string_length);
			*(str_ptr+string_length)='\0';
			sprintf((char*)g_rsp_str,"%s:%d,%d,\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				i+1,item_ptr.item_data_object_num,str_ptr);
			//SCI_TRACE_LOW("ATC:ATC_UCS2_CODE != *(item_ptr.item_data_object_buf[last_item_offset + last_item_length])");
		}
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
		SCI_FREE(str_ptr);
	}
	
	s_stk_signal[dual_sys].menu_item_num = item_ptr.item_data_object_num;
}

extern MN_CALLED_NUMBER_T g_sc_addr[MN_SYS_NUMBER];
/*****************************************************************************/
//  Description : process when received the send sms signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessSendSMS(
    ATC_CONFIG_T *atc_config_ptr
)
{
    MN_CALLED_NUMBER_T	sc_addr;//The address of SMS
    MN_L3_MSG_UNIT_T	tpdu;  
    uint16              str_len = 0;
    char                * str_ptr = PNULL;
    ERR_MNSMS_CODE_E    err_code;
    uint8               ucs2_code = 0;
    MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;

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
    		SCI_TRACE_LOW("ATC: ATC_ProcessSendSMS sc_addr.length = %d",
    						sc_addr.num_len);
    		sc_addr.num_len = MN_SMS_ADDR_MAX_LEN;
    	}
    	sc_addr = s_stk_signal[dual_sys].item.send_sms.address;
    }
    //The address of SMS service does not exist
    else
    {
    	SCI_MEMCPY(&sc_addr,&g_sc_addr[dual_sys],sizeof(MN_CALLED_NUMBER_T));
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

    tpdu.length = MIN(tpdu.length, MN_MAX_L3_MESSAGE_SIZE);
    SCI_ASSERT(tpdu.length <= MN_MAX_L3_MESSAGE_SIZE);
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
    		str_ptr = SCI_ALLOC_BASE(str_len*2+2);
    		
    	    SCI_MEMSET(str_ptr,0, ((str_len+1)*2));
    
    		//change the uint8 to ASCII format
    		ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_sms.alpha_id.id_ptr,
    			str_len,(uint8*)str_ptr);
    		sprintf((char*)g_rsp_str, "%s:\"%s\"",
    			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
    			str_ptr);
    		SCI_FREE(str_ptr);
    	}
    	else
    	{
    		sprintf((char*)g_rsp_str, "%s:\"%s\"",
    			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
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
		    SCI_TRACE_LOW("ATC: send sms pdu err code = %d.",err_code);
		}
	}
	else
	{
		//SCI_TRACE_LOW("ATC: SMS is unpacked.");		
        err_code = MNSMS_SendSmsPDUEx(dual_sys, TRUE,&sc_addr,&tpdu,FALSE,FALSE);
		if (ERR_MNSMS_NONE != err_code)
		{
		    SCI_TRACE_LOW("ATC: send sms pdu err code = %d.",err_code);
		}
	}
}

/*****************************************************************************/
//  Description : process when received the send ussd signal
//  Global resource dependence : s_stk_signal
//  Author:    @hanjun.liu
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessPlayTone(
    ATC_CONFIG_T *atc_config_ptr
)
{
	uint16              str_len = 0;
	char                * str_ptr = PNULL;
	SIMAT_PLAY_TONE_CNF_T play_tone_cnf;
	uint8               ucs2_code = 0;
       MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	   
	SCI_TRACE_LOW("ATC:enter ATC_ProcessPlayTone");
	
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
			str_ptr = SCI_ALLOC_BASE(str_len*2+2);
			SCI_MEMSET(str_ptr,0, ((str_len+1)*2));

			//change the uint8 to ASCII format
			ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.play_tone.alpha_id.id_ptr,
				str_len,(uint8*)str_ptr);
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				str_ptr);
			SCI_FREE(str_ptr);
		}
		else
		{
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				s_stk_signal[dual_sys].item.play_tone.alpha_id.id_ptr);
		}
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	play_tone_cnf.result = SIMAT_TR_SUCCESS;	

       SIMAT_CnfPlayToneEx(dual_sys, &play_tone_cnf);
}

/*****************************************************************************/
//  Description : process when received the send ss signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessSendSS(
    ATC_CONFIG_T *atc_config_ptr
)
{
	uint16  str_len = 0;
	uint8   * str_ptr = PNULL;
	uint8   ucs2_code = 0;
       MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	   
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
			str_ptr = SCI_ALLOC_BASE((str_len+1)*2);
			//change the uint8 to ASCII format
			SCI_MEMSET(str_ptr,0, ((str_len+1)*2));

			ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_ss.alpha_id.id_ptr,str_len,str_ptr);
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				(char *)str_ptr);
			SCI_FREE(str_ptr);
		}
		else
		{
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				s_stk_signal[dual_sys].item.send_ss.alpha_id.id_ptr);
		}
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
	else
	{
		sprintf((char*)g_rsp_str, s_stk_prompt_str[AT_STK_STR_SENDSS]);
		ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	}
}

/*****************************************************************************/
//  Description : process when received the send ussd signal
//  Global resource dependence : s_stk_signal
//  Author:    @
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessSendUSSD(
    ATC_CONFIG_T *atc_config_ptr
)
{
	uint16              str_len = 0;
	char                * str_ptr = PNULL;
	ERR_MNSS_CODE_E     err_code;
	uint8               ucs2_code = 0;
       MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;

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
			str_ptr = SCI_ALLOC_BASE(str_len*2+2);
			SCI_MEMSET(str_ptr,0, ((str_len+1)*2));

			//change the uint8 to ASCII format
			ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.send_ussd.alpha_id.id_ptr,str_len,(uint8*)str_ptr);
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
				str_ptr);
			SCI_FREE(str_ptr);
		}
		else
		{
			sprintf((char*)g_rsp_str, "%s:\"%s\"",
				g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
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
	    SCI_TRACE_LOW("ATC: user originate ussd fail, err code = %d.",err_code);
	}
}

/*****************************************************************************/
//  Description : process when received the setup event list signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessSetupEventList(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_TRACE_LOW("ATC:SIMAT_SETUP_EVENT_LIST_REQ");
    SCI_MEMCPY(&s_stk_signal[dual_sys].item.setup_event_list,
               &(((SIMAT_SIG_SETUP_EVENT_LIST_REQ_T *)sig_ptr)->com), sizeof(SIMAT_SETUP_EVENT_LIST_T));

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPEVENTLIST;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((SIMAT_SIG_SETUP_EVENT_LIST_REQ_T*)sig_ptr)->raw_apdu.data_arr,
                            ((SIMAT_SIG_SETUP_EVENT_LIST_REQ_T*)sig_ptr)->raw_apdu.data_len);

    ATC_StartStkTimer(atc_config_ptr, dual_sys);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the setup idle mode text signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessSetupIdleModeText(
    ATC_CONFIG_T *atc_config_ptr
)
{
    MN_DUAL_SYS_E                     dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8   * str_ptr = PNULL;
    uint16               str_len = 0;
    uint16   data_length = 0;

    ATC_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F");
    //Get the text from s_stk_signal[g_current_card_id]
    str_len = s_stk_signal[dual_sys].item.setup_idle_text.text_string.len;
    if(str_len < 1)
    {
    	str_len = 1;
    }
    if(str_len > STK_MAX_STR_LEN)
    {
    	str_len = STK_MAX_STR_LEN;
    }	

    switch(s_stk_signal[dual_sys].item.setup_idle_text.text_string.dcs)
    {
        case STK_DEFAULT_ALPHABET:
        case STK_UNPACKED_FORMAT:
            sprintf((char*)g_rsp_str, "%s:\"%s\",%d",
                g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
                (char*)s_stk_signal[dual_sys].item.setup_idle_text.text_string.text_ptr,
                s_stk_signal[dual_sys].item.setup_idle_text.icon_id.icon_id);
            break;

	case STK_PACKED_FORMAT:
		//Allocate the memory
		str_ptr = SCI_ALLOC_BASE(STK_MAX_STR_LEN);
		data_length = SIMAT_UnpackSmsAlphabet(s_stk_signal[dual_sys].item.setup_idle_text.text_string.text_ptr, 
			str_len, (uint8 *)str_ptr, STK_MAX_STR_LEN);
		if(data_length > STK_MAX_STR_LEN)
		{
			SCI_TRACE_LOW("ATC: error length in simat.");
		}
		
		sprintf((char*)g_rsp_str, "%s:\"%s\",%d",
			g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr, (char*)str_ptr,
			s_stk_signal[dual_sys].item.setup_idle_text.icon_id.icon_id);
		SCI_FREE(str_ptr);
		break;

        case STK_UCS2_ALPHABET:
            //allocate the memory
            str_ptr = SCI_ALLOC_BASE((str_len+1)*2);
            
            SCI_MEMSET(str_ptr,0,((str_len+1)*2));
            
            ATC_TranUint8IntoChar(s_stk_signal[dual_sys].item.setup_idle_text.text_string.text_ptr,
            	str_len,str_ptr);
            sprintf((char*)g_rsp_str, "%s:\"80%s\",%d",
            	g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,str_ptr,
            	s_stk_signal[dual_sys].item.setup_idle_text.icon_id.icon_id);
            SCI_FREE(str_ptr);
            break;

        default:
            break;      
    }
	//Show the text
	ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
	
}

/*****************************************************************************/
//  Description : process when received the refresh ind signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessRefreshInd(
    ATC_CONFIG_T *atc_config_ptr
)
{
    MN_DUAL_SYS_E             dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8  file_num = 0;
    uint8  i = 0;
	
    file_num = s_stk_signal[dual_sys].item.refresh.file_list.file_num;

    SCI_TRACE_LOW("ATC: ProcessRefrehind, file_num = %d, is_file_exist = %d, refresh_type = %d",
		file_num,s_stk_signal[dual_sys].item.refresh.is_files_exist, s_stk_signal[dual_sys].item.refresh.refresh_type);
	
    if( s_stk_signal[dual_sys].item.refresh.is_files_exist)
    {
        for(i = 0; i< file_num; i++)
        {
            sprintf((char*)g_rsp_str, "%s:%d, %d",
                g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr,
                s_stk_signal[dual_sys].item.refresh.refresh_type,
                s_stk_signal[dual_sys].item.refresh.file_list.file_arr[i]);    
            //Show the text
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
    }
    else
    {
        /*no file*/
        sprintf((char *)g_rsp_str, "%s: 0,0",g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }

}

#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : check the string which user input at get inkey
//  Global resource dependence : s_stk_signal
//  Author: Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_CheckAndHandleGetInkey(
    MN_DUAL_SYS_E   dual_sys,
    uint8 *key_ptr, //pointer of pointer
    uint8 str_len  //length of string
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_GET_INKEY_CNF_T get_inkey = {0};

    if (NULL == key_ptr)
    {
        return FALSE;
    }
	
    switch(s_stk_signal[dual_sys].item.get_inkey.inkey_type)
    {
        //Requre inputting digit
    case SIMAT_DIGIT_ONLY:

        if((*key_ptr >= '0' && *key_ptr <= '9')||('*' == *key_ptr)
                ||('#' == *key_ptr)||('+' == *key_ptr))
        {
            //configure the get_inkey
            get_inkey.result = SIMAT_TR_SUCCESS;
            get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
            get_inkey.text_str.dcs = s_stk_signal[dual_sys].item.get_inkey.text_str.dcs;
            *(get_inkey.text_str.text_ptr)   = *key_ptr;
        }
        break;

        //Requre inputting english string
    case SIMAT_SMS_ALPHABET:

        //configure the get_inkey
        get_inkey.result = SIMAT_TR_SUCCESS;
        get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
        get_inkey.text_str.dcs = s_stk_signal[dual_sys].item.get_inkey.text_str.dcs;
        *get_inkey.text_str.text_ptr     = *key_ptr;
        *(get_inkey.text_str.text_ptr+1) = '\0';
        break;

        //Requre inputting UCS2 string
    case SIMAT_UCS2_ALPHABET:
    {
        uint8 ucs2_string[MN_MAX_SIMAT_DATA_OBJECT_LEN+2];

        SCI_MEMSET(ucs2_string,0,(MN_MAX_SIMAT_DATA_OBJECT_LEN + 2));
        //configure the get_inkey
        get_inkey.result = SIMAT_TR_SUCCESS;
        get_inkey.text_str.len = STK_GETINKEY_CNF_UCS2_LEN;
        get_inkey.text_str.dcs = s_stk_signal[dual_sys].item.get_inkey.text_str.dcs;

        //Change the input the string into UCS2 string and assign it
        if(!ConvertAscUcs2IntoBin((char *)key_ptr,get_inkey.text_str.text_ptr,str_len/2))
        {
            return FALSE;
        }
    }
    break;

    //Requre inputting YES or NO.
    case SIMAT_YES_NO:

        if((*key_ptr == 'Y') || (*key_ptr == 'y') || (*key_ptr == 'N') || (*key_ptr == 'n'))
        {
            //configure the get_inkey
            get_inkey.result = SIMAT_TR_SUCCESS;
            get_inkey.text_str.len = STK_GETINKEY_CNF_ASCI_LEN;
            get_inkey.text_str.dcs = s_stk_signal[dual_sys].item.get_inkey.text_str.dcs;

            //user select YES
            if((*key_ptr == 'Y') || (*key_ptr == 'y'))
            {
                *get_inkey.text_str.text_ptr = 0x01;
            }
            //user select NO
            else
            {
                *get_inkey.text_str.text_ptr = 0x00;
            }

            *(get_inkey.text_str.text_ptr+1)='\0';
        }

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
//  Description : process when received the send dtmf signal
//  Global resource dependence : s_stk_signal
//  Author: binli
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessLangNotify(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_TRACE_LOW("ATC:ATC_ProcessLangNotify");

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_LANGNOTIFY;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_LANG_NOTIFY_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_LANG_NOTIFY_IND_T*)sig_ptr)->raw_apdu.data_len);
    ATC_StartStkTimer(atc_config_ptr, dual_sys);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_STKProcessSendSmsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_SEND_SMS_CNF_T    *sig_ptr
)
{
    SIMAT_SEND_SMS_CNF_T  send_sms;

    SCI_MEMSET(&send_sms, 0x0, sizeof(SIMAT_SEND_SMS_CNF_T));
    ATC_TRACE_LOW("ATC:enter ATC_STKProcessSendSmsCnf");

    //send the sms successfully.
    if(MN_SMS_OPERATE_SUCCESS == sig_ptr->cause)
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSmsCnf,MN_SMS_OPERATE_SUCCESS");
        send_sms.result = SIMAT_TR_SUCCESS;
        send_sms.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        
    }
    else if( MN_SMS_SIM_STK_NOT_ALLOWED == sig_ptr->cause)
    {
        ATC_TRACE_LOW("ATC:send sms failed for stk mo sms control.");
        send_sms.result =  SIMAT_TR_PERM_PROBLEM_INN_CC;
        send_sms.cc_info = SIMAT_CS_CONTROL_ACTION_NOT_ALLOWED_CAUSE;
    }
    else
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSmsCnf,MN_SMS_FAIL");
        ATC_GetDetailSmsError(sig_ptr->dual_sys,&send_sms);

        if((0x11==sig_ptr->rl_cause)||(0x16==sig_ptr->rl_cause)||(MN_SMS_CAUSE_PROTOCOL_ERROR==sig_ptr->rl_cause))
        {
            ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SMS_REJECT_CP_CLASS, sig_ptr->rl_cause);
        }
        else if((MN_SMS_CAUSE_DEST_OUT_OF_ORDER==sig_ptr->rl_cause)||(MN_SMS_CAUSE_NET_OUT_OF_ORDER==sig_ptr->rl_cause)
          ||(MN_SMS_CAUSE_TEMP_FAILURE==sig_ptr->rl_cause)||(MN_SMS_CAUSE_CONGESTION==sig_ptr->rl_cause)
          ||(MN_SMS_CAUSE_RESOURCES_UNAV==sig_ptr->rl_cause)||(MN_SMS_CAUSE_INTERWORKING==sig_ptr->rl_cause))
        {
            ATC_UpSPERRORInfo(sig_ptr->dual_sys, atc_config_ptr, SMS_REJECT_RP_CLASS, sig_ptr->rl_cause);
        }
    }

    //send the result to SIM.
    SIMAT_CnfSendSmsEx(sig_ptr->dual_sys, &send_sms);

    g_is_stk_sendsms[sig_ptr->dual_sys] = FALSE;

    return S_ATC_SUCCESS;

}

/*****************************************************************************/
//  Description : This function handles the APP_MN_USSD_SERVICE_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_STKProcessSendUssdCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_USSD_SERVICE_CNF_T    *sig_ptr
)
{

    SIMAT_SEND_USSD_CNF_T  send_ussd_cnf;

    SCI_MEMSET(&send_ussd_cnf, 0x0, sizeof(SIMAT_SEND_USSD_CNF_T));
    ATC_TRACE_LOW("ATC:enter ATC_STKProcessSendUssdCnf");

    /* send the ussd successfully. */
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendUssdCnf,MN_SMS_OPERATE_SUCCESS");
        send_ussd_cnf.result = SIMAT_TR_SUCCESS;
        send_ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        send_ussd_cnf.ussd_return = sig_ptr->ussd_data;     
        /* send the result to SIM. */
        SIMAT_CnfSendUssdEx(sig_ptr->dual_sys, &send_ussd_cnf);
    }
    /* fail to send the ussd. */
    else if(MN_SS_OPERATION_ERROR == sig_ptr->result_type)
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendUssdCnf,MN_USSD_ERROR");
        send_ussd_cnf.result = SIMAT_TR_USSD_ERROR;
        send_ussd_cnf.ussd_info = sig_ptr->err_code;
        /* send the result to SIM. */
        SIMAT_CnfSendUssdEx(sig_ptr->dual_sys, &send_ussd_cnf);
    }
    else
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendUssdCnf,MN_USSD_REJECT");
        send_ussd_cnf.result = SIMAT_TR_USSD_ERROR;
        send_ussd_cnf.ussd_info = SIMAT_USSDP_NO_SPECIFIC_CAUSE;
        /* send the result to SIM. */
        SIMAT_CnfSendUssdEx(sig_ptr->dual_sys, &send_ussd_cnf);
    }

    g_is_stk_sendussd[sig_ptr->dual_sys] = FALSE;

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_STKProcessSendSsCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    ATC_SS_SERVICE_CNF_TYPE_E service_cnf_type,
    void    *sig_ptr
)
{
    SIMAT_SEND_SS_CNF_T  send_ss_cnf;
    MN_DUAL_SYS_E    dual_sys = ATC_CURRENT_SIM_ID_MUX;  
    BOOLEAN service_is_ok = TRUE;   

    SCI_MEMSET(&send_ss_cnf, 0x0, sizeof(SIMAT_SEND_SS_CNF_T));
    ATC_TRACE_LOW("ATC:enter ATC_STKProcessSendSsCnf");

    /* send the ss successfully. */
    switch(service_cnf_type)
    {
        case ATC_SS_SERVICE_CNF_INTERROGATE:
        case ATC_SS_SERVICE_CNF_DEACTIVATE:
        case ATC_SS_SERVICE_CNF_ACTIVATE:
        case ATC_SS_SERVICE_CNF_ERASE:
        case ATC_SS_SERVICE_CNF_REGISTER:
            //the structer for interrogate and regisiter is the same,so deal the same
            dual_sys = ((APP_MN_INTERROGATE_CNF_T *)sig_ptr)->dual_sys;
            if(MN_SS_OPERATION_SUCC == ((APP_MN_INTERROGATE_CNF_T *)sig_ptr)->result_type)
            {
                service_is_ok = TRUE;
            }
            else
            {
                service_is_ok = FALSE;
                send_ss_cnf.result = SIMAT_TR_SS_ERROR;

                if(((APP_MN_INTERROGATE_CNF_T *)sig_ptr)->result_type == MN_SS_OPERATION_REJECT)
                {
                    SCI_TRACE_LOW("ATC:send ss failure: MN_SS_OPERATION_REJECT");
                    send_ss_cnf.ss_info = SIMAT_SSP_NO_SPECIFIC_CAUSE;
                }
                else
                {
                    SCI_TRACE_LOW("ATC:send ss failure: MN_SS_OPERATION_ERROR");
                    send_ss_cnf.ss_info = ((APP_MN_INTERROGATE_CNF_T *)sig_ptr)->err_code;
                }
            }
            break;

        case ATC_SS_SERVICE_CNF_RELEASE:
            dual_sys = ((APP_MN_RELEASE_SS_IND_T *)sig_ptr)->dual_sys;
            if(((APP_MN_RELEASE_SS_IND_T *)sig_ptr)->error_present)
            {
                service_is_ok = FALSE;
                send_ss_cnf.result = SIMAT_TR_SS_ERROR;
                send_ss_cnf.ss_info = ((APP_MN_RELEASE_SS_IND_T *)sig_ptr)->error_code;
            }
            else
            {
                service_is_ok = TRUE;
            }
            break;
        default:
            break;
    }

    if(service_is_ok)
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSsCnf,MN_SMS_OPERATE_SUCCESS");
        send_ss_cnf.result = SIMAT_TR_SUCCESS;
        send_ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    }
    else
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSsCnf,MN_SS_FAIL");
        ATC_UpSPERRORInfo(dual_sys, atc_config_ptr, SS_SERVICE_REJECT_CLASS, (uint32)((APP_MN_REGISTER_SS_CNF_T *)sig_ptr)->err_code);
    }

    SIMAT_CnfSendSsEx(dual_sys, &send_ss_cnf);

    g_is_stk_sendss[dual_sys] = FALSE;

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handles the APP_MN_READ_SMS_CNF signal and handle
//                it
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_STKProcessSendSsRegPwdCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_REGISTER_PASSWORD_CNF_T    *sig_ptr
)
{
    SIMAT_SEND_SS_CNF_T  send_ss_cnf;

    SCI_MEMSET(&send_ss_cnf, 0x0, sizeof(SIMAT_SEND_SS_CNF_T));
    ATC_TRACE_LOW("ATC:enter ATC_STKProcessSendSsRegPwdCnf");

    /* send the ss successfully. */
    if(MN_SS_OPERATION_SUCC == sig_ptr->result_type)
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SMS_OPERATE_SUCCESS");
        send_ss_cnf.result = SIMAT_TR_SUCCESS;
        send_ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        /* send the result to SIM. */
        SIMAT_CnfSendSsEx(sig_ptr->dual_sys, &send_ss_cnf);
    }
    /* fail to send the ss. */
    else
    {
        ATC_TRACE_LOW("ATC:ATC_STKProcessSendSsRegPwdCnf,MN_SS_FAIL");
        send_ss_cnf.result = SIMAT_TR_SS_ERROR;
        send_ss_cnf.ne_info = SIMAT_NP_NO_SPECIFIC_CAUSE;
        /* send the result to SIM. */
        SIMAT_CnfSendSsEx(sig_ptr->dual_sys, &send_ss_cnf);
    }

    g_is_stk_sendss[sig_ptr->dual_sys] = FALSE;

    return S_ATC_SUCCESS;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : process when user gives a response in display text
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_DisplayTextCnf(
    MN_DUAL_SYS_E   dual_sys,
    uint32  cmd_type,
    uint32 result,
    uint32 add_info
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_DISPLAY_TEXT_CNF_T display_text = {0};

    if(!s_stk_signal[dual_sys].item.display_text.is_ir)
    {
        switch(cmd_type)
        {
        case ATC_STATUS_DISPLAYTEXT:
            display_text.result = (SIMAT_GENERAL_PC_RESULT_E)result;
            display_text.mp_info = (SIMAT_ME_PROBLEM_E)add_info;
            break;
            //User go back
        case ATC_BACK_MOVE:
            display_text.result = SIMAT_TR_BACK_MOVE;
            display_text.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
            //User abort
        case ATC_END_SESSION:
            display_text.result = SIMAT_TR_TERMINATED_BY_USER;
            display_text.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
            //User no response
        case ATC_NO_RESPONSE:
            display_text.result = SIMAT_TR_NO_RESPONSE;
            display_text.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
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
//  Description : process when user gives a response in get inkey
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetInkeyCnf(
    MN_DUAL_SYS_E     dual_sys,
    uint32  cmd_type, //the type of command
    uint32  result,   //value of result
    uint8   *key_ptr,//Pointer of data
    uint8   keys_length  //Length of data
)
{
    BOOLEAN re_value = TRUE ;
    SIMAT_GET_INKEY_CNF_T get_inkey;
    SCI_MEMSET(&get_inkey, 0x0,sizeof(SIMAT_GET_INKEY_CNF_T));

    switch(cmd_type)
    {
    case ATC_BACK_MOVE:
        get_inkey.result = SIMAT_TR_BACK_MOVE;
        SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);
        break;

        //User abort
    case ATC_END_SESSION:
        get_inkey.result = SIMAT_TR_TERMINATED_BY_USER;
        //Send the get key cnf(user end)
        SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);
        break;

    case ATC_NO_RESPONSE:
        get_inkey.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)
        SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);
        break;

    case ATC_STATUS_GETINKEY:

        if (ATC_USER_RESPONSE == result)
        {
            if(!ATC_CheckAndHandleGetInkey(dual_sys,key_ptr,keys_length))
            {
		  SCI_TRACE_LOW("ATC:GetInkeyCnf,CheckAndHandleGetInkey fail");
                re_value = FALSE;
            }
        }
        else
        {
            get_inkey.result = (SIMAT_GENERAL_PC_RESULT_E)result;
            SIMAT_CnfGetInkeyEx(dual_sys, &get_inkey);
        }

        break;

    default:
        re_value = FALSE;
        break;
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in get input
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetInputCnf(
    MN_DUAL_SYS_E     dual_sys,
    uint32  cmd_type, //the type of command
    uint32  result,   //value of result
    uint8   *key_ptr,//Pointer of data
    uint16   keys_length  //Length of data   
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_GET_INPUT_CNF_T  get_input = {0};

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
        SIMAT_CnfGetInputEx(dual_sys, &get_input);
        break;

    case ATC_NO_RESPONSE:
        get_input.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)
        SIMAT_CnfGetInputEx(dual_sys, &get_input);
        break;

    case ATC_STATUS_GETINPUT:

        if (SIMAT_TR_SUCCESS == result)
        {
            if(!ATC_CheckAndHandleGetInkey(dual_sys,key_ptr,keys_length))
            {
                re_value = FALSE;
            }
        }
        else
        {
            get_input.result = (SIMAT_GENERAL_PC_RESULT_E)result;
            SIMAT_CnfGetInputEx(dual_sys, &get_input);
        }

        break;

    default:
        re_value = FALSE;
        break;
    }

    return(re_value);
}

/*****************************************************************************/
// 	Description : process when user selected item in setup menu 
//	Global resource dependence : s_stk_signal[g_current_card_id]
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_MainMenuCnf(
    MN_DUAL_SYS_E   dual_sys,
    uint32   result,    //value of result
    uint8     n_select //item number to select
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_MENU_SELECTION_IND_T menu_select;
    SCI_TRACE_LOW("ATC: AT_MainMenuCnf");

    if(n_select > s_stk_signal[dual_sys].setup_menu.item.item_data_object_num)
    {
        return FALSE;
    }
    s_mainmenu_selitem[dual_sys] = n_select;
    n_select--;
    SCI_TRACE_LOW("ATC:n_select= %d",n_select);
	
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
            SCI_TRACE_LOW("ATC:s_stk_signal[dual_sys].setup_menu.help_info= %d",s_stk_signal[dual_sys].setup_menu.help_info);

            if(TRUE == s_stk_signal[dual_sys].setup_menu.help_info)
            {
                menu_select.is_help_IND = TRUE;
                menu_select.menu_id = s_stk_signal[dual_sys].setup_menu.item.item_data_object_info[n_select][0];

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
    SCI_TRACE_LOW("ATC:ATC_MainMenuCnf=%d",re_value);
    return(re_value);
}
#endif /* _ULTRA_LOW_CODE_ */
/*****************************************************************************/
//  Description : This function handle the call signals
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
void  ATC_StkProcessCallSignals(
    BOOLEAN IsCallSucc,
    MN_DUAL_SYS_E dual_sys,
    uint8   call_id,
    ATC_STK_SETUP_CALL_FAIL_TYPE_E  fail_type,
    MN_CALL_DISCONNECT_CALL_CAUSE_E fail_cause
)
{
    SIMAT_SETUP_CALL_CNF_T  setup_call;
    uint32 timer_value = 0;
    uint8 redial_category = 0;
    BOOLEAN is_need_redial = FALSE;


    ATC_TRACE_LOW("ATC:ATC_StkProcessCallSignals");

    SCI_MEMSET(&setup_call, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
    if(!s_stk_redial_info[dual_sys].is_first_call_attempt_finished)     
    {
        s_stk_redial_info[dual_sys].is_first_call_attempt_finished = TRUE;
    }

    if(TRUE == IsCallSucc)
    {
        ATC_TRACE_LOW("ATC:setup call success.");
        setup_call.result = SIMAT_TR_SUCCESS;
    }
    else
    {
        ATC_TRACE_LOW("ATC:setup call failed,type is %d,cause is %d.",fail_type,fail_cause);
        if((fail_type == ATC_STK_CALL_FAIL_ERROR) && 
            (fail_cause == MN_CAUSE_SIM_STK_NOT_ALLOWED))
        {
            ATC_TRACE_LOW("ATC:setup call failed for call control.");
            setup_call.result = SIMAT_TR_PERM_PROBLEM_INN_CC;
        }
        else if(IsDiscByAP(dual_sys,call_id))
        {
            ATC_TRACE_LOW("ATC:setup call failed for disconnect by AP.");
            setup_call.result = SIMAT_TR_USER_CLEAR_CALL;
            is_need_redial = FALSE;                 
        }
        else if(IsNeedRedial(dual_sys,fail_type,fail_cause,&redial_category))
        {
            is_need_redial = TRUE;
            s_stk_redial_info[dual_sys].fail_cause = fail_cause;
        }
        else
        {
            setup_call.result = SIMAT_TR_NE_UNABLE;
            if(s_stk_redial_info[dual_sys].is_self_disc_for_exp)
            {
                ATC_TRACE_LOW("ATC:setup call failed for max redial timer expiration.");
                setup_call.ne_info = s_stk_redial_info[dual_sys].fail_cause|0x80;                     
            }
            else
            {
                //see 12.12.3 additional information for network problem,the first byte shall be the cause value
                //of the cause informtion element returned by the network.Bit 8 shall be set to 1.
                setup_call.ne_info = fail_cause|0x80;
            }
        }
    }

    if(!is_need_redial) 
    {
        //stop timer
        ATC_StkStopMaxRedialDurationTimer(dual_sys);
        ATC_StkStopRedialTimer(dual_sys);

        SIMAT_CnfSetupCallEx(dual_sys, &setup_call);

        ClearRedialInfo(dual_sys);
        
        SCI_MEMSET(&s_stk_redial_info[dual_sys],0,sizeof(ATC_STK_REDIAL_INFO_T));

        g_is_stk_call[dual_sys] = FALSE;
    }           

    if(IsCallSucc)      
    {
        //check whether is dtmf need to send
        if(g_stk_call_info[dual_sys].is_dtmf)
        {
            g_stk_call_info[dual_sys].call_index = call_id;
            ATC_STKSendDtmf(dual_sys);
        }
    }

    if(is_need_redial)
    {
        GetRedialTimerVal(dual_sys,redial_category,&timer_value);
        if(timer_value != 0)
        {
            ATC_StartRedialTimer(dual_sys,timer_value);
        }

        s_stk_redial_info[dual_sys].is_redial_ongoing = FALSE;
    }
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : process when user selected item in setup menu
//  Global resource dependence : s_stk_signal
//  Author: Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SelectItemHandle(
    MN_DUAL_SYS_E    dual_sys,
    uint8   item_num     //ponter of data
)
{
    BOOLEAN re_value = TRUE;
    int32 n_select = 0;
    SIMAT_SELECT_ITEM_CNF_T select_item = {0};

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

    return(re_value);
}
/*****************************************************************************/
//  Description : process when user gives a response in select item
//  Global resource dependence : s_stk_signal
//  Author:    Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SelectItemCnf(
    MN_DUAL_SYS_E    dual_sys,
    uint32  cmd_type,       //type of command
    uint32  result,   //value of result
    uint8   item_num            //select item
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SELECT_ITEM_CNF_T select_item = {0};

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

    case ATC_STATUS_SELECTITEM:
	 switch(result)
	 {
	     case ATC_ST_USERBORT:
               select_item.result = SIMAT_TR_TERMINATED_BY_USER;
               //Send the display text cnf
               SIMAT_CnfSelItemEx(dual_sys, &select_item);
               break;

	     case ATC_ST_SELECTITEM:
		 ATC_SelectItemHandle(dual_sys,item_num);
		 break;

	     case ATC_ST_HELPINFO:
		 select_item.result = SIMAT_TR_HELP_INFO;
		  SIMAT_CnfSelItemEx(dual_sys, &select_item);
                break;	

	     case ATC_ST_RETURNBACK:
		   select_item.result = SIMAT_TR_BACK_MOVE;
                //Send the display text cnf
                SIMAT_CnfSelItemEx(dual_sys, &select_item);
                break;

		default:
			break;
	 }
        break;

    default:
        re_value = FALSE;
        break;
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : This function handles the data send from the SIO in get input
//                 and get inkey.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
uint32 ATC_ProcessSioStkData(
    ATC_CONFIG_T    *atc_config_ptr,
    uint8           *data_ptr  // pointer to the given pdu buffer
)
{
	uint8       terminater_char;     // indicates ther terminater char
	uint16      data_len;          // holds the gived pdu length
       BOOLEAN     flag = FALSE;
       MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;

	SCI_ASSERT(PNULL != atc_config_ptr);
	SCI_ASSERT(PNULL != data_ptr);

	data_len              = (uint16)strlen((char*)data_ptr);
	terminater_char      = data_ptr[data_len - 1];
	data_ptr[data_len - 1] = '\0';
	data_len             -= 1;
	
	switch (terminater_char)
	{
		// cancel the operation
	case ESC:
		ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
		flag = TRUE;
		break;

	case CTRL_Z:
		// call the different process
		SCI_TRACE_LOW("ATC: sio stk data_len = %d",data_len);
		switch(s_stk_signal[dual_sys].cur_status)
		{
		case ATC_STATUS_GETINKEY:
			flag = ATC_GetInkeyCnf(dual_sys,s_cmd_type[dual_sys],s_result[dual_sys],data_ptr,data_len);
			break;
		case ATC_STATUS_GETINPUT:
			flag = ATC_GetInputCnf(dual_sys,s_cmd_type[dual_sys],s_result[dual_sys],data_ptr,data_len);
			break;
		default:
			SCI_ASSERT(FALSE);
			break;
		}
		break;
		
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
//  Description : process when user gives a response in setup event list
//  Global resource dependence : s_stk_signal
//  Author:    Wanghui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SetupEventListCnf(
    uint32  cmd_type,
    uint32  result,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SETUP_EVENT_LIST_CNF_T setup_event_list_cnf;
    SCI_MEMSET(&setup_event_list_cnf, 0x0, sizeof(SIMAT_SETUP_EVENT_LIST_CNF_T));

    switch(cmd_type)
    {
        case ATC_SETUP_EVENT_LIST:

            if(0 == result)
            {
                setup_event_list_cnf.result = SIMAT_TR_SUCCESS;
            }
            else
            {
                setup_event_list_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_event_list_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }

            break;
            //User go back
        case ATC_BACK_MOVE:
            setup_event_list_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
            //User abort
        case ATC_END_SESSION:
            setup_event_list_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            setup_event_list_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the display text cnf
    if(re_value)
    {
        SIMAT_CnfSetEventListEx(dual_sys, &setup_event_list_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in setup idle mode text
//  Global resource dependence : s_stk_signal
//  Author:    Wanghui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SetupIdleModeTextCnf(
    MN_DUAL_SYS_E dual_sys,
    uint32  cmd_type,
    uint32  result,  //value of result
    uint32  add_info  //additional information    
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SETUP_IDLE_TEXT_CNF_T setup_idletext_cnf;
    SCI_MEMSET(&setup_idletext_cnf, 0x0, sizeof(SIMAT_SETUP_IDLE_TEXT_CNF_T));
    switch(cmd_type)
    {
            //case ATC_SETUP_EVENT_LIST:
        case ATC_SETUP_IDLE_MODE_TEXT:
            setup_idletext_cnf.result = (SIMAT_GENERAL_PC_RESULT_E)result;
	     setup_idletext_cnf.mp_info = (SIMAT_ME_PROBLEM_E)add_info;
            break;
            //User go back
        case ATC_BACK_MOVE:
            setup_idletext_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
            //User abort
        case ATC_END_SESSION:
            setup_idletext_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            setup_idletext_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the display text cnf
    if(re_value)
    {
        SIMAT_CnfSetIdleTextEx(dual_sys, &setup_idletext_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in send dtmf
//  Global resource dependence : s_stk_signal
//  Author:    Wanghui
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendDtmfCnf(
    uint32  cmd_type,
    uint32  result,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SEND_DTMF_CNF_T dtmf_cnf;
    SCI_MEMSET(&dtmf_cnf, 0x0, sizeof(SIMAT_SEND_DTMF_CNF_T));
    switch(cmd_type)
    {
            //case ATC_SETUP_EVENT_LIST:
        case ATC_SEND_DTMF:

            if(0 == result)
            {
                dtmf_cnf.result = SIMAT_TR_SUCCESS;
            }
            else if(2 == result)
            {
                dtmf_cnf.result = SIMAT_TR_ME_UNABLE;
                dtmf_cnf.mp_info = SIMAT_MP_NOT_IN_SPEECH_CALL;
            }
            else
            {
                dtmf_cnf.result = SIMAT_TR_ME_UNABLE;
                dtmf_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            }

            break;
            //User go back
        case ATC_BACK_MOVE:
            dtmf_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
            //User abort
        case ATC_END_SESSION:
            dtmf_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            dtmf_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the display text cnf
    if(re_value)
    {
        SIMAT_CnfSendDtmfEx(dual_sys, &dtmf_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
// 	Description : check the string which user input at get input
//	Global resource dependence : s_stk_signal[g_current_card_id]
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_HandleSetupCall(
								  SIMAT_SETUP_CALL_CNF_T  *setup_call,
								  uint32 result
								  )
{
	BOOLEAN                        re_value = TRUE;
	MN_CALLED_NUMBER_T   called_num;             
	MN_SUBADDR_T             sub_addr;
     MN_DUAL_SYS_E               dual_sys= ATC_CURRENT_SIM_ID_MUX;
	MN_CALL_TYPE_E            call_type;
	uint8                               call_id;
	MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;

	switch(result)
	{
            case ATC_REFUSE_CALL:     //User refuse the call
        		setup_call->result = SIMAT_TR_TERMINATED_BY_USER;
				
        		//Send the get key cnf(user end)		
		        SIMAT_CnfSetupCallEx(dual_sys, setup_call); 
					
        		break;
        
            case ATC_ACCEPT_CALL:  //User accep the call
        		SCI_MEMSET(&called_num, 0, sizeof(MN_CALLED_NUMBER_T));
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

        			//Start a call with sub address.
                            if (MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys,
                										call_type, &called_num,
                										&sub_addr, MN_CALL_CLIR_SUPPRESSION, PNULL))
        			
        			{
        				g_is_stk_call[dual_sys] = TRUE;
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
// 	Description : process when user gives a response in setup call
//	Global resource dependence : s_stk_signal[g_current_card_id]
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN ATC_SetupCallCnf(
    MN_DUAL_SYS_E    dual_sys,
    ATC_CONFIG_T   *atc_config_ptr,
    uint32  cmd_type, //the type of command
    uint32  result,  //value of result
    uint32  add_info  //additional information
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SETUP_CALL_CNF_T  setup_call = {0};
    SCI_TRACE_LOW("ATC:SetupCallCnf,dual_sys:%d,cmd_type:%d,result:%",dual_sys,cmd_type,result);

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
        SIMAT_CnfSetupCallEx(dual_sys, &setup_call);
        break;

    case ATC_NO_RESPONSE:
        setup_call.result = SIMAT_TR_NO_RESPONSE;
        //Send the get key cnf(no respnse)
        SIMAT_CnfSetupCallEx(dual_sys, &setup_call);
        break;

    case ATC_STATUS_SETUPCALL:

        if(SIMAT_TR_SUCCESS == result)
        {
            if(!ATC_HandleSetupCall(&setup_call,result))
                {
                    re_value = FALSE;
                }
        }
        else
        {
            if(SIMAT_TR_NE_UNABLE == (SIMAT_GENERAL_PC_RESULT_E)result)
            {
                setup_call.result = (SIMAT_GENERAL_PC_RESULT_E)result;
                setup_call.ne_info = (SIMAT_ME_PROBLEM_E)add_info;
                SIMAT_CnfSetupCallEx(dual_sys, &setup_call);
            }
            else
            {
                setup_call.result = (SIMAT_GENERAL_PC_RESULT_E)result;
                setup_call.mp_info = (SIMAT_ME_PROBLEM_E)add_info;
                SIMAT_CnfSetupCallEx(dual_sys, &setup_call);
            }
        }

        break;

    default:
        re_value = FALSE;
        break;
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in play tone
//  Global resource dependence : s_stk_signal
//  Author: Liuhan
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_PlayToneCnf(
    MN_DUAL_SYS_E  dual_sys,
    uint32  cmd_type,
    uint32 result,
    uint32 add_info
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_PLAY_TONE_CNF_T play_tone = {0};

    switch(cmd_type)
    {
    case ATC_STATUS_PLAYTONE:
        play_tone.result = (SIMAT_GENERAL_PC_RESULT_E)result;
        play_tone.mp_info = (SIMAT_ME_PROBLEM_E)add_info;
        break;
        //User go back
    case ATC_BACK_MOVE:
        play_tone.result = SIMAT_TR_BACK_MOVE;
        play_tone.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        break;
        //User abort
    case ATC_END_SESSION:
        play_tone.result = SIMAT_TR_TERMINATED_BY_USER;
        play_tone.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        break;
        //User no response
    case ATC_NO_RESPONSE:
        play_tone.result = SIMAT_TR_NO_RESPONSE;
        play_tone.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
        break;

    default:
        re_value = FALSE;
        break;
    }

    //Send the play tone cnf
    if(re_value)
    {
        SIMAT_CnfPlayToneEx(dual_sys,&play_tone);
    }

    return(re_value);
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : process when user gives a response in refresh
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendRefreshCnf(
    uint32  cmd_type,
    ATC_CONFIG_T       *atc_config_ptr,
    MN_DUAL_SYS_E      dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_REFRESH_CNF_T     refresh_cnf;

    SCI_MEMSET((void *)&refresh_cnf, 0x0, sizeof(SIMAT_REFRESH_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            refresh_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
            //User abort
        case ATC_END_SESSION:
            refresh_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            refresh_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    if(s_stk_is_refresh_reset[dual_sys])
    {
        //refresh reset need not send terminal response to SIM
        re_value = FALSE;
        s_stk_is_refresh_reset[dual_sys] = FALSE;
    }

    //Send the refresh cnf
    if(re_value)
    {
        SIMAT_CnfRefreshEx(dual_sys, &refresh_cnf);
    }

    /*report ^STIN:REFRESH RESULT <string>*/
    if(refresh_cnf.result == SIMAT_TR_SUCCESS)
    {
        sprintf((char*)g_rsp_str, "^STIN:REFRESH RESULT, 0");
    }
    else
    {
        sprintf((char*)g_rsp_str, "^STIN:REFRESH RESULT, 1");           
    }

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in send data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendSendDataCnf(
    uint32  cmd_type,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_SEND_DATA_CNF_T send_data_cnf;

    SCI_MEMSET((void *)&send_data_cnf, 0x0, sizeof(SIMAT_SEND_DATA_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            send_data_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            send_data_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            send_data_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the send data cnf
    if(re_value)
    {
        SIMAT_CnfSendDataEx(dual_sys, &send_data_cnf);
    }

    return(re_value);
}
/*****************************************************************************/
//  Description : process when user gives a response in receive data
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendReceiveDataCnf(
    uint32  cmd_type,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_RECEIVE_DATA_CNF_T receive_data_cnf;

    SCI_MEMSET((void *)&receive_data_cnf, 0x0, sizeof(SIMAT_RECEIVE_DATA_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            receive_data_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            receive_data_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            receive_data_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the receive data cnf
    if(re_value)
    {
        SIMAT_CnfReceiveDataEx(dual_sys, &receive_data_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in launch browser
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendLaunchBrowserCnf(
    uint32  cmd_type,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_LAUNCH_BROWSER_CNF_T lauch_browser_cnf;

    SCI_MEMSET((void *)&lauch_browser_cnf, 0x0, sizeof(SIMAT_LAUNCH_BROWSER_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            lauch_browser_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            lauch_browser_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            lauch_browser_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the launch browser cnf
    if(re_value)
    {
        SIMAT_CnfLaunchBrowserEx(dual_sys, &lauch_browser_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in open channel
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendOpenChannelCnf(
    uint32  cmd_type,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_OPEN_CHANNEL_CNF_T open_channel_cnf;

    SCI_MEMSET((void *)&open_channel_cnf, 0x0, sizeof(SIMAT_OPEN_CHANNEL_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            open_channel_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            open_channel_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            open_channel_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the open channel cnf
    if(re_value)
    {
        SIMAT_CnfOpenChannelEx(dual_sys, &open_channel_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response in Close channel
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendCloseChannelCnf(
    uint32  cmd_type,
    uint8   channel_id,    // 1 - 7
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_CLOSE_CHANNEL_CNF_T close_channel_cnf;

    SCI_MEMSET((void *)&close_channel_cnf, 0x0, sizeof(SIMAT_CLOSE_CHANNEL_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            close_channel_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            close_channel_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            close_channel_cnf.channel_id = channel_id;
            close_channel_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the close channel cnf
    if(re_value)
    {
        SIMAT_CnfCloseChannelEx(dual_sys, &close_channel_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when user gives a response of get channel status
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_SendGetChannelStatusCnf(
    uint32  cmd_type,
    MN_DUAL_SYS_E dual_sys
)
{
    BOOLEAN re_value = TRUE;
    SIMAT_GET_CHANNEL_STATUS_CNF_T get_channel_status_cnf;

    SCI_MEMSET((void *)&get_channel_status_cnf, 0x0, sizeof(SIMAT_GET_CHANNEL_STATUS_CNF_T));

    switch(cmd_type)
    {
        case ATC_BACK_MOVE:
            get_channel_status_cnf.result = SIMAT_TR_BACK_MOVE;
            break;
        case ATC_END_SESSION:
            get_channel_status_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case ATC_NO_RESPONSE:
            get_channel_status_cnf.result = SIMAT_TR_NO_RESPONSE;
            break;
        default:
            re_value = FALSE;
            break;
    }

    //Send the get channel status cnf
    if(re_value)
    {
        SIMAT_CnfGetChannelStatusEx(dual_sys, &get_channel_status_cnf);
    }

    return(re_value);
}

/*****************************************************************************/
//  Description : process when received the refresh cnf signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessRefreshCnf(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    APP_MN_SIMAT_REFRESH_CNF_T  *param_ptr = (APP_MN_SIMAT_REFRESH_CNF_T *)sig_ptr;
    SIMAT_REFRESH_CNF_T refresh_cnf;
#ifdef ONTIM_PRJ
    ATC_REFRESH_FILE_T *refresh_ptr = NULL;
#endif

    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCnf, is_ok=%d,is_file_changed=%d",
                  param_ptr->result.is_ok, param_ptr->result.is_file_changed);
    SCI_MEMSET(&refresh_cnf, 0, sizeof(SIMAT_REFRESH_CNF_T));

    //Kill the timer
    SCI_KILL_TIMER(P_ATC, (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                   &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
    
    if(param_ptr->result.is_ok)
    {
        refresh_cnf.result = SIMAT_TR_SUCCESS;
        refresh_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    }
    else
    {
        refresh_cnf.result = SIMAT_TR_ME_UNABLE;
        refresh_cnf.mp_info = SIMAT_MP_SCREEN_BUSY;
    }

    if(s_stk_is_refresh_reset[dual_sys])
    {
        //refresh reset need not send terminal response to SIM
        s_stk_is_refresh_reset[dual_sys] = FALSE;
    }
    else
    {
        //respond refresh cnf
        SIMAT_CnfRefreshEx(dual_sys, &refresh_cnf);
    }

#ifdef ONTIM_PRJ
    refresh_ptr = &s_stk_refresh[dual_sys];
    
    ATC_BuildRefreshCnf(atc_config_ptr, refresh_ptr);
#else
    /*report ^STIN:REFRESH RESULT <string>*/
    if(refresh_cnf.result == SIMAT_TR_SUCCESS)
    {
        sprintf((char*)g_rsp_str, "^STIN:REFRESH RESULT, 0");
    }
    else
    {
        sprintf((char*)g_rsp_str, "^STIN:REFRESH RESULT, 1");           
    }

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
#endif

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
    
    if(s_stk_is_pending[dual_sys] == TRUE)
    {
        if(ATC_SignalCacheSignalNum(dual_sys) < 1)
        {
            s_stk_is_pending[dual_sys] = FALSE;
            ATC_TRACE_LOW("ATC:ATC_ProcessRefreshCnf No pending Event\n");
        }
        else
        { 
            ATC_ProcessSTKSignalCache(atc_config_ptr, dual_sys);
#ifdef _MUX_ENABLE_
            atc_config_ptr->current_link_id= s_stk_link_id[dual_sys];
#endif
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_SPUSATTERMINAL].cmd_name->str_ptr, 0);
            
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            return S_ATC_SUCCESS;
        }   
    }

    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : process when received the send data signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessSendData(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_SEND_DATA_IND_T *param_ptr = (APP_MN_SIMAT_SEND_DATA_IND_T *)sig_ptr;

    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessSendData, channel id:%d",
                  param_ptr->send_data.channel_id);

    SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_data, &param_ptr->send_data,
               sizeof(SIMAT_SEND_DATA_T));
    s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDDATA;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_SEND_DATA_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_SEND_DATA_IND_T*)sig_ptr)->raw_apdu.data_len);
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the receive data signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessReceiveData(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_RECEIVE_DATA_IND_T *param_ptr = (APP_MN_SIMAT_RECEIVE_DATA_IND_T *)sig_ptr;
    
    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessReceiveData, channel id:%d",
                  param_ptr->receive_data.channle_id);

    SCI_MEMCPY(&s_stk_signal[dual_sys].item.receive_data, &param_ptr->receive_data,
               sizeof(SIMAT_RECEIVE_DATA_T));
    s_stk_signal[dual_sys].cur_status = ATC_STATUS_RECEIVEDATA;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_RECEIVE_DATA_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_RECEIVE_DATA_IND_T*)sig_ptr)->raw_apdu.data_len);        
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the launch browser signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessLaunchBrowser(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_LAUNCH_BROWSER_IND_T *param_ptr = (APP_MN_SIMAT_LAUNCH_BROWSER_IND_T *)sig_ptr;
   
    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessLaunchBrowser");

    SCI_MEMCPY(&s_stk_signal[dual_sys].item.launch_browser, &param_ptr->com,
               sizeof(SIMAT_LAUNCH_BROWSER_T));
    s_stk_signal[dual_sys].cur_status = ATC_STATUS_LAUNCHBROWSER;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)sig_ptr)->raw_apdu.data_len);            
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the open channel signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessOpenChannel(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_OPEN_CHANNEL_IND_T *param_ptr = (APP_MN_SIMAT_OPEN_CHANNEL_IND_T *)sig_ptr;
    
    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessOpenChannel");

    SCI_MEMCPY(&s_stk_signal[dual_sys].item.open_channel, &param_ptr->open_channel,
               sizeof(SIMAT_OPEN_CHANNEL_RELATED_GPRS_T));
    s_stk_signal[dual_sys].cur_status = ATC_STATUS_OPENCHANNEL;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_OPEN_CHANNEL_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_OPEN_CHANNEL_IND_T*)sig_ptr)->raw_apdu.data_len);        
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the close channel signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessCloseChannel(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_CLOSE_CHANNEL_IND_T *param_ptr = (APP_MN_SIMAT_CLOSE_CHANNEL_IND_T *)sig_ptr;
    
    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessCloseChannel, channel_id = %d",
                  param_ptr->close_channel.channel_id);

    SCI_MEMCPY(&s_stk_signal[dual_sys].item.close_channel, &param_ptr->close_channel,
               sizeof(SIMAT_CLOSE_CHANNEL_T));
    s_stk_signal[dual_sys].cur_status = ATC_STATUS_CLOSECHANNEL;
    

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_CLOSE_CHANNEL_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_CLOSE_CHANNEL_IND_T*)sig_ptr)->raw_apdu.data_len);        
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : process when received the get channel status ind signal
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ATC_ProcessGetChannelStatusInd(
    ATC_CONFIG_T *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
#ifndef _ULTRA_LOW_CODE_
    APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T *param_ptr = (APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T *)sig_ptr;

    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != param_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ProcessGetChannelStatusInd");

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_GETCHANNELSTATUS;

    ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
    ATC_BuildProactiveRsp(dual_sys,
                            atc_config_ptr,
                            ((APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T*)sig_ptr)->raw_apdu.data_arr,
                            ((APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T*)sig_ptr)->raw_apdu.data_len);    
    ATC_StartStkTimer(atc_config_ptr, dual_sys);
#endif

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles string to byte
//  Global resource dependence
//  Author:
//  Note:
/*****************************************************************************/
BOOLEAN ATC_StringToByte(char *str_ptr , uint16 str_length, uint8 *byte_data_ptr, uint16 *length_ptr)
{
    uint16 loop = 0;

    if((NULL == str_ptr) || (NULL == byte_data_ptr) || (NULL == length_ptr))
    {
        ATC_TRACE_LOW("ATC:: ATC_StringToByte pointer is NULL!");

        return FALSE;
    }

    if(str_length % 2)
    {
        *length_ptr = str_length / 2 + 1;
    }
    else
    {
        *length_ptr = str_length / 2;
    }

    for(loop = 0; loop < str_length; loop++)
    {
        if((str_ptr[loop] >= 'a') && (str_ptr[loop] <= 'f'))
        {
            if(loop % 2)
            {
                byte_data_ptr[loop/2]  += str_ptr[loop] - 'a' + 0x0A;
            }
            else
            {
                byte_data_ptr[loop/2]  = (str_ptr[loop] - 'a' + 0x0A) << 4;
            }
        }
        else if((str_ptr[loop] >= 'A') && (str_ptr[loop] <= 'F'))
        {
            if(loop % 2)
            {
                byte_data_ptr[loop/2]  += str_ptr[loop] - 'A' + 0x0A;
            }
            else
            {
                byte_data_ptr[loop/2]  = (str_ptr[loop] - 'A' + 0x0A) << 4;
            }
        }
        else if((str_ptr[loop] >= '0') && (str_ptr[loop] <= '9'))
        {
            if(loop % 2)
            {
                byte_data_ptr[loop/2]  += str_ptr[loop] - '0';
            }
            else
            {
                byte_data_ptr[loop/2]  = (str_ptr[loop] - '0') << 4;
            }
        }
        else
        {
            ATC_TRACE_LOW("ATC:: invalid number:%x" , str_ptr[loop]);

            return FALSE;
        }
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATCALLSETUP command
//  Global resource dependence : g_rsp_str
//  Author:       binli
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATCALLSETUP )
{
    BOOLEAN    is_accept  = TRUE;
    MN_CALLED_NUMBER_T called_num;
    uint8 call_id = 0xff;
    MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
    MN_CALL_TYPE_E call_type = MN_CALL_TYPE_NORMAL;
    MN_SUBADDR_T sub_addr;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    SCI_MEMSET(&plmn_info, 0x0, sizeof(MN_PHONE_CURRENT_PLMN_INFO_T));
    SCI_MEMSET(&sub_addr, 0x0, sizeof(MN_SUBADDR_T));

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (!PARAM1_FLAG || (PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
            {
                ATC_TRACE_LOW("Param1  invalid\n");
                return ERR_INVALID_INDEX;
            }

            is_accept = PARA1.num;

            if(!is_accept)
            {
                SIMAT_SETUP_CALL_CNF_T setup_call_cnf;
                SCI_MEMSET(&setup_call_cnf, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
                setup_call_cnf.result = SIMAT_TR_USER_DENY_CALL;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
                ClearRedialInfo(dual_sys);
                
                return S_ATC_SUCCESS;
            }

            SCI_MEMSET(&called_num, 0, sizeof(MN_CALLED_NUMBER_T));

            if(CheckPresentWaitCall(&g_calls_state[dual_sys], &call_id) ||
                    CheckPresentActiveCall(&g_calls_state[dual_sys], &call_id))
            {
                SIMAT_SETUP_CALL_CNF_T setup_call_cnf;
                SCI_MEMSET(&setup_call_cnf, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
                setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
                ClearRedialInfo(dual_sys);
                return S_ATC_FAIL;
            }


            /* check the status of PLMN. */
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

            if(PLMN_NO_SERVICE == plmn_info.plmn_status)
            {
                SIMAT_SETUP_CALL_CNF_T setup_call_cnf;
                SCI_MEMSET(&setup_call_cnf, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
                setup_call_cnf.result = SIMAT_TR_LIMITED_SERVICE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
                ClearRedialInfo(dual_sys);
                return S_ATC_FAIL;
            }

            call_type = MN_CALL_TYPE_NORMAL;
            called_num = s_stk_signal[dual_sys].item.setup_call.call_parameter.address;

            /* check the length */
            if(s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len > MN_MAX_ADDR_BCD_LEN)
            {
                called_num.num_len = MN_MAX_ADDR_BCD_LEN;
            }
            else
            {
                called_num.num_len = s_stk_signal[dual_sys].item.setup_call.call_parameter.address.num_len;
            }

            //check whether the dtmf is present,remove it from the tele number
            ATC_CheckSTKCallNum(&called_num,&g_stk_call_info[dual_sys]);
            g_stk_call_info[dual_sys].call_index = ATC_INVALID_CALLID;

            /* There is sub address in the call number */
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

                SIMAT_SetOperSourceEx(dual_sys,SIMAT_OPER_SETUP_CALL);

                /* Start a call with sub address. */
                if(MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys,
                                                           call_type, &(g_stk_call_info[dual_sys].call_num),
                                                           &sub_addr, MN_CALL_CLIR_SUPPRESSION, PNULL))
                {
                    g_is_stk_call[dual_sys] = TRUE;
                    g_atc_call_flag = TRUE;
                    g_atc_call_need_disc = FALSE;

#ifdef _MUX_ENABLE_
                    ATC_TRACE_LOW("ATC:ATC_ProcessESATCALLSETUP call_link_id = %d ", atc_config_ptr->current_link_id);
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_CALL_START_IND, 0xff, ATC_DOMAIN_CS, dual_sys);
#endif
                }

            }
            else /* No sub address */
            {
                SIMAT_SetOperSourceEx(dual_sys,SIMAT_OPER_SETUP_CALL);
                
                /* start a call */
                if(MN_RETURN_SUCCESS == MNCALL_StartCallEx(dual_sys,
                                                           call_type, &(g_stk_call_info[dual_sys].call_num),
                                                           PNULL, MN_CALL_CLIR_SUPPRESSION, PNULL))
                {
                    g_is_stk_call[dual_sys] = TRUE;
                    g_atc_call_flag = TRUE;
                    g_atc_call_need_disc = FALSE;

#ifdef _MUX_ENABLE_
                    ATC_TRACE_LOW("ATC:ATC_ProcessSPUSATCALLSETUP call_link_id = %d ",atc_config_ptr->current_link_id);
                    ATC_Add_Expected_Event(atc_config_ptr, APP_MN_CALL_START_IND, 0xff, ATC_DOMAIN_CS, dual_sys);
#endif
                }
            }

            break;
        }
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATPROFILE command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUSATPROFILE )
{
    uint32  return_value = S_ATC_SUCCESS;
    uint8 *profile_data = NULL;
    uint16 profile_data_len = 0;
    uint8  result_profile[STK_PROFILE_CHAR_LEN] = {0};
    uint16 result_profile_len = 0;
    ATC_CUR_PARAM_STRING_T *profile_download_ptr;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
            profile_data = (uint8 *)SCI_ALLOC_BASE(18);
            SCI_ASSERT(NULL != profile_data);/*assert verified: check null pointer*/
            SCI_MEMSET(profile_data, 0xff, 18);

            //report profile result data
            ATC_ProfileResultData(profile_data, profile_data_len, result_profile, &result_profile_len);
            ATC_BuildProfileRsp(atc_config_ptr, result_profile, result_profile_len);

            ATC_TRACE_LOW("ATC:+SPUSATPROFILE, Profile download :s_stk_signal.is_support_stk:%d", s_stk_signal[dual_sys].is_support_stk);
            s_ap_is_active[dual_sys] = TRUE;
            ATC_ProcessSTKSignalCache(atc_config_ptr, dual_sys);
            SCI_FREE(profile_data);
            break;
        case ATC_CMD_TYPE_SET:
            {
                if(!PARAM1_FLAG)
                {
                    ATC_TRACE_LOW("ATC:Param1  invalid\n");
                    return ERR_INVALID_INDEX;
                }

                ATC_TRACE_LOW("ATC:+SPUSATPROFILE: Param1 length=%d\n", PARA1.str_ptr->str_len);

                if((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING) ||
                        (PARA1.str_ptr->str_len > STK_PROFILE_CHAR_LEN * 2))
                {
                    ATC_TRACE_LOW("ATC:+SPUSATPROFILE: Param1 is not string or string length is too long\n");
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                if(s_ap_is_active[dual_sys] == TRUE)
                {
                    ATC_TRACE_LOW("ATC:+SPUSATPROFILE,s_ap_is_active[%d] is already TRUE.", dual_sys);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                profile_download_ptr = PARA1.str_ptr;

                profile_data = (uint8 *)SCI_ALLOC_BASE(profile_download_ptr->str_len / 2 + 1);
                SCI_ASSERT(NULL != profile_data);/*assert verified: check null pointer*/
                SCI_MEMSET(profile_data, 0x0, (profile_download_ptr->str_len / 2 + 1));

                if(!ATC_StringToByte((char *)profile_download_ptr->str_ptr, profile_download_ptr->str_len, profile_data, &profile_data_len))
                {
                    ATC_TRACE_LOW("ATC:+SPUSATPROFILE, String to Byte  error\n");
                    SCI_FREE(profile_data);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                //report profile result data
                ATC_ProfileResultData(profile_data, profile_data_len, result_profile, &result_profile_len);
                ATC_BuildProfileRsp(atc_config_ptr, result_profile, result_profile_len);

                ATC_TRACE_LOW("ATC:+SPUSATPROFILE,Profile download :s_stk_signal.is_support_stk:%d", s_stk_signal[dual_sys].is_support_stk);
                s_ap_is_active[dual_sys] = TRUE;
                ATC_ProcessSTKSignalCache(atc_config_ptr, dual_sys);
                SCI_FREE(profile_data);
            }

            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#ifdef _MUX_ENABLE_
    atc_config_ptr->current_link_id= s_stk_link_id[dual_sys];
#endif
    return(return_value);
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
// 	Description : give the response when received the get input signal
//	Global resource dependence : s_stk_signal[g_current_card_id]
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_StkOutputPrompt(
					               ATC_CONFIG_T    *atc_config_ptr
     							   )
{
	uint32  return_value = S_ATC_SUCCESS;

	// output the '>'
	sprintf((char*)g_rsp_str, "%c%c> ", atc_config_ptr->s3_reg, atc_config_ptr->s4_reg);
	ATC_SendAsyncInfo(atc_config_ptr, ATC_CTRLZ_STK);

	return_value = S_ATC_DEFAULT_HANDLE;
	return return_value;

}

/*****************************************************************************/
// 	Description : check the index and condition in ATC_ProcessSTGR
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_CheckCondAndPara1(
    MN_DUAL_SYS_E     dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    BOOLEAN         para1_flag,
    uint8           para1_num
)
{
    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag  
            || FALSE == atc_config_ptr->sim_flag[dual_sys] || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    // Check the <cmd_type>
    if (!para1_flag)
    {
        return ERR_INVALID_INDEX;
    }

    if((para1_num == s_stk_signal[dual_sys].cur_status) || (para1_num == ATC_BACK_MOVE)
            || (para1_num == ATC_BEYOND_ABILITIE) || (para1_num == ATC_UNABLE_PROCESS)
            || (para1_num == ATC_NO_RESPONSE) || (para1_num == ATC_END_SESSION))
    {

        return S_ATC_SUCCESS;
    }

    return ERR_INVALID_INDEX;
}
/*****************************************************************************/
// 	Description : Give the response when received the setup menu signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSetupMenu(
    MN_DUAL_SYS_E     dual_sys,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    if(((!para2_flag || !para3_flag)&& ATC_RESULT_SELECTITEM == para2_num)
       /* || (para3_flag && ATC_RESULT_HELPINFO == para2_num)*/)
    {
        return ERR_INVALID_INDEX;
    }
    //Give the response to setup menu
    if(!(ATC_SETUP_MENU == para1_num
            && ATC_MainMenuCnf(dual_sys,para2_num,para3_num)))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
}
/*****************************************************************************/
// 	Description : Give the response when received the display text signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseDisplayText(
    MN_DUAL_SYS_E     dual_sys,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    if((!para2_flag || !para3_flag) && (ATC_BACK_MOVE != para1_num)&&
            (ATC_BEYOND_ABILITIE != para1_num)&&(ATC_UNABLE_PROCESS != para1_num)
            &&(ATC_NO_RESPONSE!= para1_num)&&(ATC_END_SESSION!= para1_num))
    {
        return ERR_INVALID_INDEX;
    }

    if(!ATC_DisplayTextCnf(dual_sys,para1_num, para2_num,para3_num))
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
    else
    {
        return S_ATC_SUCCESS;
    }
}
/*****************************************************************************/
// 	Description : Give the response when received the get inkey signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
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
    uint8 * p_data_str = PNULL;//Just a null pointer
     MN_DUAL_SYS_E     dual_sys= ATC_CURRENT_SIM_ID_MUX;
    
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
    		return S_ATC_SUCCESS;
    	}
    }
}

/*****************************************************************************/
// 	Description : Give the response when received the get input signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
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
	uint8 *p_data_str = PNULL;//Just a null pointer
    MN_DUAL_SYS_E     dual_sys= ATC_CURRENT_SIM_ID_MUX;

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
				return S_ATC_SUCCESS;
			}
		}
	}
}
/*****************************************************************************/
// 	Description : Give the response when received the setup call signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSetupCall(
    MN_DUAL_SYS_E    dual_sys,
    ATC_CONFIG_T    *atc_config_ptr,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    //Check the index
    if((!para2_flag || !para3_flag)&& (ATC_BACK_MOVE != para1_num)&&
            (ATC_BEYOND_ABILITIE != para1_num)&&(ATC_UNABLE_PROCESS != para1_num)
            &&(ATC_NO_RESPONSE!= para1_num)&&(ATC_END_SESSION!= para1_num))
    {
        return ERR_INVALID_INDEX;
    }

    if(!ATC_SetupCallCnf(dual_sys, atc_config_ptr,para1_num,para2_num,para3_num))
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        else
        {
            return S_ATC_SUCCESS;
        }
}
/*****************************************************************************/
//  Description : Give the response when received the play tone signal
//  Global resource dependence :
//  Author: Eddie.Wang
//  Note:
/*****************************************************************************/
LOCAL uint32 ATC_ResponsePlayTone(
    MN_DUAL_SYS_E     dual_sys,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    if((!para2_flag || !para3_flag)&& (ATC_BACK_MOVE != para1_num)&&
            (ATC_BEYOND_ABILITIE != para1_num)&&(ATC_UNABLE_PROCESS != para1_num)
            &&(ATC_NO_RESPONSE!= para1_num)&&(ATC_END_SESSION!= para1_num))
    {
        return ERR_INVALID_INDEX;
    }

    if(ATC_PLAY_TONE != para1_num)
    {
        return ERR_INVALID_INDEX;
    }

    if(!ATC_PlayToneCnf(dual_sys,para1_num,para2_num,para3_num))
    {
        return S_ATC_FAIL;
    }
    else
    {
        return S_ATC_SUCCESS;
    }

}
/*****************************************************************************/
// 	Description : Give the response when received the select item signal
//	Global resource dependence : 
//  Author:	Liuhan
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSelectItem(
    MN_DUAL_SYS_E    dual_sys,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    if(ATC_STATUS_SELECTITEM == para1_num && (!para2_flag||!para3_flag))
    {
        SCI_TRACE_LOW("ATC:ATC_ResponseSelectItem param incorrect");
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
}
/*****************************************************************************/
// 	Description : Give the response when received the setup call signal
//	Global resource dependence : 
//  Author:	
//	Note: 
/*****************************************************************************/
LOCAL uint32 ATC_ResponseSetupIdleModeText(
    MN_DUAL_SYS_E    dual_sys,
    uint8   para1_num,
    BOOLEAN para2_flag,
    uint8   para2_num,
    BOOLEAN para3_flag,
    uint8   para3_num
)
{
    //Check the index
    if((!para2_flag || !para3_flag)&& (ATC_BACK_MOVE != para1_num)&&
            (ATC_BEYOND_ABILITIE != para1_num)&&(ATC_UNABLE_PROCESS != para1_num)
            &&(ATC_NO_RESPONSE!= para1_num)&&(ATC_END_SESSION!= para1_num))
    {
        return ERR_INVALID_INDEX;
    }

    if(!ATC_SetupIdleModeTextCnf(dual_sys,para1_num,para2_num,para3_num))
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        else
        {
            return S_ATC_SUCCESS;
        }
}
/*****************************************************************************/
//  Description : This function handles the AT^STSF command
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STSF)
{
    uint32  return_value = S_ATC_SUCCESS;
    uint8   terminal_profile[STK_PROFILE_CHAR_LEN + 1] = {0};
    uint8   result_terminal_profile[STK_PROFILE_CHAR_LEN] = {0};
    uint16  length = STK_PROFILE_CHAR_LEN;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    BOOLEAN flag = FALSE;

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    switch(ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_TEST:
        sprintf((char *)g_rsp_str, "%s:0-2",
                g_atc_info_table[AT_CMD_xor_STSF].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_READ:
        SIMAT_GetMeProfile(terminal_profile,&length);
        ATC_TranUint8IntoChar(terminal_profile,
                              length, result_terminal_profile);

        sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d",
                g_atc_info_table[AT_CMD_xor_STSF].cmd_name->str_ptr,
                atc_config_ptr->atc_config_nv.stk_actflag,
                (char *)result_terminal_profile,atc_config_ptr->atc_config_nv.stk_time);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

     //set command AT^STSF=<mode>[,<config>][,<Timeout>]
    case ATC_CMD_TYPE_SET:

        // Check the <mode>
        if (!PARAM1_FLAG)
        {
            return ERR_INVALID_INDEX;
        }

        SCI_TRACE_LOW("ATC: ^STSF,Type: %d, is_support_stk:%d",PARA1.num,s_stk_signal[dual_sys].is_support_stk);

        switch (PARA1.num)
        {
        case ATC_STK_DEACTIVATE:

            if( TRUE == s_stk_signal[dual_sys].is_support_stk)
            {

                atc_config_ptr->atc_config_nv.stk_actflag = FALSE;
                ATC_WriteNVItem(atc_config_ptr, NV_ATC_CONFIG_ID);

                switch(s_stk_signal[dual_sys].cur_status)
                {
                case ATC_STATUS_DISPLAYTEXT:
                    flag = ATC_DisplayTextCnf(dual_sys, ATC_END_SESSION,0,0);
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
                    return_value = ERR_OPERATION_NOT_ALLOWED;
                }
            }
            else
            {
                return_value = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_STK_ACTIVATE:

            if( TRUE == s_stk_signal[dual_sys].is_support_stk)
            {
                SIMAT_SETUP_MENU_CNF_T  smenu = {0};

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

                    //Return ^STIN:0
                    sprintf((char *)g_rsp_str, "%s:0", g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    s_cmd_stag[dual_sys] = ATC_CMD_INIT;
                    atc_config_ptr->atc_config_nv.stk_actflag = TRUE;

                    ATC_WriteNVItem(atc_config_ptr, NV_ATC_CONFIG_ID);
                }
                else
                {
                    s_cmd_stag[dual_sys] = ATC_CMD_INIT;
                    atc_config_ptr->atc_config_nv.stk_actflag = TRUE;

                    ATC_WriteNVItem(atc_config_ptr, NV_ATC_CONFIG_ID);
                }
            }
            else
            {
                return_value = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        case ATC_STK_CONFIGURE:
            //If SIM card doesnt support STK,return FALSE
            if(FALSE == s_stk_signal[dual_sys].is_support_stk)
            {
                return ERR_OPERATION_NOT_ALLOWED;
            }

            if (!PARAM2_FLAG || (PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //check the config string and send it to sim.
            if(!ATC_CheckAndHandleConfig(dual_sys,atc_config_ptr,(char *)PARA2.str_ptr->str_ptr))
            {
                return_value = ERR_OPERATION_NOT_ALLOWED;
                break;
            }

            if(PARAM3_FLAG)
            {
                atc_config_ptr->atc_config_nv.stk_time = PARA3.num;
                ATC_WriteNVItem(atc_config_ptr, NV_ATC_CONFIG_ID);
            }

            break;

        default:
            return_value = S_ATC_FAIL;
            break;
        }

        break;

    default:
        SCI_ASSERT(FALSE);
        break;
    }

    return return_value;
}
/*****************************************************************************/
//  Description : This function handles the AT^STGI command
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STGI)
{
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag
    	|| FALSE == atc_config_ptr->sim_flag[dual_sys]
    	|| FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
    	return ERR_OPERATION_NOT_ALLOWED;
    }
	
	if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
	{
		// Check the <cmd_type>
		if (!PARAM1_FLAG)
		{
			return ERR_INVALID_INDEX;
		}
		if(PARA1.num != s_stk_signal[dual_sys].cur_status && PARA1.num != ATC_REFRESH)
		{
			SCI_TRACE_LOW("ATC:PARA1.num != s_stk_signal[dual_sys].cur_status, %d",s_stk_signal[dual_sys].cur_status);
			return ERR_INVALID_INDEX;
		}
		switch(PARA1.num)
		{
		case ATC_SETUP_MENU:
			if(atc_config_ptr->atc_config_nv.stk_time > 0)
			{
                        SCI_SET_TIMER(P_ATC,atc_config_ptr->atc_config_nv.stk_time*1000*30, (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND+dual_sys),
                                                  &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
			}
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
		case ATC_PLAY_TONE:
			ATC_ProcessPlayTone(atc_config_ptr);
			break;
	       case ATC_REFRESH:
		   	ATC_ProcessRefreshInd(atc_config_ptr);
			break;
		case ATC_SEND_SS:
			ATC_ProcessSendSS(atc_config_ptr);
			break;
		case ATC_SEND_USSD:
			ATC_ProcessSendUSSD(atc_config_ptr);
			break;
		case ATC_SETUP_IDLE_MODE_TEXT:
			ATC_ProcessSetupIdleModeText(atc_config_ptr);
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
       else if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
       {
           sprintf((char *)g_rsp_str, "%s:(0-11)",
                   g_atc_info_table[AT_CMD_xor_STGI].cmd_name->str_ptr);
           ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
         
       }
	else
	{
		return_value = ERR_OPERATION_NOT_ALLOWED;
	}
	return return_value;
}

/*****************************************************************************/
//  Description : This function handles the AT^STIN command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STIN)
{
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

	if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag 
		|| FALSE == atc_config_ptr->sim_flag[dual_sys]
		|| FALSE == s_stk_signal[dual_sys].is_support_stk)
	{
	      SCI_TRACE_LOW("ATC: ^STIN is can't find as support stk fail");
		return ERR_OPERATION_NOT_ALLOWED;
	}
	if(ATC_CMD_TYPE_READ == ATC_GET_CMD_TYPE)
	{
		//Return ^STIN: <index>
		if(ATC_CMD_INIT == s_cmd_stag[dual_sys])
		{
			sprintf((char*)g_rsp_str, "%s:%d",
				g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr,
				s_stk_signal[dual_sys].cur_status);
			ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
		}
		else
		{
		    SCI_TRACE_LOW("ATC: s_cmd_stage is not ATC_CMD_INIT");
    		    return_value = ERR_OPERATION_NOT_ALLOWED;
		}

	}
	else
	{
		return_value = ERR_OPERATION_NOT_ALLOWED;
	}
	return return_value;

}
/*****************************************************************************/
//  Description : This function handles the AT^STGR command
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_STGR)
{
    uint32  return_value = S_ATC_SUCCESS;
    uint32  para1_num    = 0; 
    uint32  para2_num    = 0;
    uint32  para3_num    = 0;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        if (!PARAM1_FLAG)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        para1_num = PARA1.num;


        //Check the parameter
        return_value = ATC_CheckCondAndPara1(dual_sys,atc_config_ptr,PARAM1_FLAG,para1_num);

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

	 SCI_TRACE_LOW("ATC: ^STGR,cur_status=%d,para1_num=%d",s_stk_signal[dual_sys].cur_status, para1_num);

        switch(s_stk_signal[dual_sys].cur_status)
        {
        case ATC_STATUS_SETUPMENU:
            return_value = ATC_ResponseSetupMenu(dual_sys,para1_num, PARAM2_FLAG,para2_num,PARAM3_FLAG,para3_num);
            break;

        case ATC_STATUS_DISPLAYTEXT:
            return_value = ATC_ResponseDisplayText(dual_sys,
                                                   para1_num,
                                                   PARAM2_FLAG,
                                                   para2_num,
                                                   PARAM3_FLAG,
                                                   para3_num
                                                  );
            break;

        case ATC_STATUS_GETINKEY:
            return_value = ATC_ResponseGetInkey(atc_config_ptr,
                                                para1_num,
                                                PARAM2_FLAG,
                                                para2_num,
                                                PARAM3_FLAG,
                                                para3_num
                                               );
        break;

        case ATC_STATUS_GETINPUT:
            return_value = ATC_ResponseGetInput(atc_config_ptr,
                                                para1_num,
                                                PARAM2_FLAG,
                                                para2_num,
                                                PARAM3_FLAG,
                                                para3_num
                                               );
        break;

        case ATC_STATUS_SETUPCALL:
            return_value = ATC_ResponseSetupCall(
                               dual_sys,
                               atc_config_ptr,
                               para1_num,
                               PARAM2_FLAG,
                               para2_num,
                               PARAM3_FLAG,
                               para3_num
                           );
            break;

        case ATC_STATUS_PLAYTONE:
            return_value = ATC_ResponsePlayTone(dual_sys,
                                                para1_num,
                                                PARAM2_FLAG,
                                                para2_num,
                                                PARAM3_FLAG,
                                                para3_num
                                               );
            break;
			
        case ATC_STATUS_SELECTITEM:
            return_value = ATC_ResponseSelectItem(dual_sys,
                                                  para1_num,
                                                  PARAM2_FLAG,
                                                  para2_num,
                                                  PARAM3_FLAG,
                                                  para3_num
                                                 );
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

	case ATC_STATUS_SETUPIDLEMODETEXT:
            return_value = ATC_ResponseSetupIdleModeText(dual_sys,
                                        para1_num,
                                        PARAM2_FLAG,
                                        para2_num,
                                        PARAM3_FLAG,
                                        para3_num
                                       );
            break;
	
			
        default:
            SCI_TRACE_LOW("ATC: ^STGR cmd_type is not allow");
            break;
        }

        //Kill the timer
        SCI_KILL_TIMER(P_ATC,(ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND+dual_sys),
                       &T_ATC_STSF_EXP_IND[dual_sys],"T_ATC_STSF_EXP_IND");
    }
    else if(ATC_CMD_TYPE_TEST == ATC_GET_CMD_TYPE)
    {
        sprintf((char *)g_rsp_str, "%s:(0,1,2,3,4,6,11,12,99),(0-2)",
                g_atc_info_table[AT_CMD_xor_STGR].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
      
    }
    else
    {
        return_value = ERR_OPERATION_NOT_ALLOWED;
    }
    return (return_value);
}
#endif
/*****************************************************************************/
//  Description : This function handles the AT+STED command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSTED)
{
    return ERR_OPERATION_NOT_SUPPORTED;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATCAPREQ command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUSATCAPREQ)
{
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
            ATC_TRACE_LOW("ATC: SPUSATCAPREQ command is read command");

            if(s_stk_signal[dual_sys].is_support_stk == TRUE)
            {
                sprintf((char *)g_rsp_str, "%s: 0",
                        g_atc_info_table[AT_CMD_SPUSATCAPREQ].cmd_name->str_ptr);
            }
            else
            {
                sprintf((char *)g_rsp_str, "%s: 1",
                        g_atc_info_table[AT_CMD_SPUSATCAPREQ].cmd_name->str_ptr);
            }

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;

    }

    return return_value;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPUSATENVECMD command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUSATENVECMD)
{
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //Check if it support Stk
    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag
       || FALSE == atc_config_ptr->sim_ready[dual_sys] 
       || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(s_ap_is_active[dual_sys] == FALSE
       || s_stk_is_active[dual_sys] == FALSE 
       || s_stk_is_pending[dual_sys] == TRUE)
    {
        ATC_TRACE_LOW("ATC:+SPUSATENVECMD,s_ap_is_active:%d or s_stk_is_active:%d or s_stk_is_pending: %d.",
                      s_ap_is_active[dual_sys], s_stk_is_active[dual_sys], s_stk_is_pending[dual_sys]);
        return ERR_OPERATION_NOT_ALLOWED;
    }

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            {
                ATC_CUR_PARAM_STRING_T *envelop_data_ptr;
                uint8 *out_data;
                BOOLEAN ret = FALSE;
                uint16 out_data_len = 0;
                STK_ENVELOP_RESULT_T stk_envelop_result;

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                SCI_MEMSET(&stk_envelop_result, 0, sizeof(STK_ENVELOP_RESULT_T));

                envelop_data_ptr = PARA1.str_ptr;


                ATC_TRACE_LOW("ATC: SPUSATENVECMD set command ");

                out_data = (uint8 *)malloc(envelop_data_ptr->str_len / 2 + 1);

                ATC_StringToByte((char *)envelop_data_ptr->str_ptr, envelop_data_ptr->str_len, out_data, &out_data_len);

                ret = PhoneSever_STK_Envelop_Handle(dual_sys, out_data, out_data_len, &stk_envelop_result);

                ATC_TRACE_LOW("SPUSATENVECMD event_tag =%x ", stk_envelop_result.event_tag);

                if(ret)
                {
                    switch(stk_envelop_result.event_tag)
                    {
                        case SIMAT_DTAG_MENU_SELECTION:
                            ATC_TRACE_LOW("ATC  SPUSATENVECMD Menu Selection\n");

                            if(s_stk_signal[dual_sys].cur_status == ATC_STATUS_SETUPMENU)
                            {
                                ret =  ATC_EnvelopCmd_MenuSelection(&stk_envelop_result.item.menu_sel_ind, dual_sys);
                            }
                            else
                            {
                                ATC_TRACE_LOW("SPUSATENVECMD set cmd:stk current status is not setupmenu");
                                ret = FALSE;
                            }
                            break;

                        case SIMAT_DTAG_EVENT_DOWNLOAD:
                            break;

                        default:
                            ATC_TRACE_LOW("SPUSATENVECMD set cmd: tag is invalid");
                            ret = FALSE;
                            break;

                    }

                }

                sprintf((char *)g_rsp_str, "%s: %d",
                        g_atc_info_table[AT_CMD_SPUSATENVECMD].cmd_name->str_ptr, (BOOLEAN)(!ret));

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                free(out_data);
                //Kill the timer
                SCI_KILL_TIMER(P_ATC, (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                               &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
            }
            break;


        default:
            return_value = ERR_OPERATION_NOT_ALLOWED;
            break;

    }

    return return_value;

}


/*****************************************************************************/
//  Description : This function handles the AT+SPUSATTERMINAL command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPUSATTERMINAL)
{
    uint32  return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    //Check if it support Stk
    if(FALSE == atc_config_ptr->atc_config_nv.stk_actflag
       || FALSE == atc_config_ptr->sim_ready[dual_sys]
       || FALSE == s_stk_signal[dual_sys].is_support_stk)
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(s_ap_is_active[dual_sys] == FALSE || s_stk_is_active[dual_sys] == FALSE)
    {
        ATC_TRACE_LOW("ATC:+SPUSATTERMINAL,s_ap_is_active:%d or s_stk_is_active:%d",
                      s_ap_is_active[dual_sys], s_stk_is_active[dual_sys]);
        return ERR_OPERATION_NOT_ALLOWED;
    }

#ifdef _MUX_ENABLE_
    s_stk_link_id[dual_sys]= atc_config_ptr->current_link_id;
#endif

    if(s_stk_is_pending[dual_sys] == TRUE)
    {
        if(ATC_SignalCacheSignalNum(dual_sys) < 1)
        {
            s_stk_is_pending[dual_sys] = FALSE;
            ATC_TRACE_LOW("ATC:No pending Event\n");
        }
        else
        {
            ATC_ProcessSTKSignalCache(atc_config_ptr, dual_sys);
#ifdef _MUX_ENABLE_
            atc_config_ptr->current_link_id= s_stk_link_id[dual_sys];
#endif
            sprintf((char *)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_SPUSATTERMINAL].cmd_name->str_ptr, 0);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

            SCI_KILL_TIMER(P_ATC, (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                       &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
            return S_ATC_SUCCESS;
        }            
    }

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            {
                uint8 *out_data;
                uint16 out_data_len = 0;
                BOOLEAN ret = TRUE;
                ATC_CUR_PARAM_STRING_T *ternimal_data_ptr;
                STK_TERMINAL_RESULT_T  stk_terminal_result;

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }

                SCI_MEMSET(&stk_terminal_result, 0, sizeof(STK_TERMINAL_RESULT_T));

                ternimal_data_ptr = PARA1.str_ptr;

                ATC_TRACE_LOW("ATC: terminal set command ");

                out_data = (uint8 *)malloc(ternimal_data_ptr->str_len / 2 + 1);

                ATC_StringToByte((char *)ternimal_data_ptr->str_ptr, ternimal_data_ptr->str_len, out_data, &out_data_len);

                ret = PhoneSever_STK_TerminalRsp_Handle(out_data, out_data_len, &stk_terminal_result);

                if(ret)
                {

                    switch(stk_terminal_result.pc_cmd_type)
                    {
                        case SIMAT_PC_REFRESH:
                            SIMAT_CnfRefreshEx(dual_sys, &(stk_terminal_result.item.refresh_cnf));
                            break;

                        case SIMAT_PC_SETUP_CALL:
                            SIMAT_CnfSetupCallEx(dual_sys, &(stk_terminal_result.item.setup_call_cnf));
                            ClearRedialInfo(dual_sys);
                            break;

                        case SIMAT_PC_SMS:
                            SIMAT_CnfSendSmsEx(dual_sys, &(stk_terminal_result.item.send_sms_cnf));
                            break;

                        case SIMAT_PC_SEND_SS:
                            SIMAT_CnfSendSsEx(dual_sys, &(stk_terminal_result.item.send_ss_cnf));
                            break;

                        case SIMAT_PC_SEND_USSD:
                            SIMAT_CnfSendUssdEx(dual_sys, &(stk_terminal_result.item.send_ussd_cnf));
                            break;

                        case SIMAT_PC_SEND_DTMF:
                            SIMAT_CnfSendDtmfEx(dual_sys, &(stk_terminal_result.item.send_dtmf_cnf));
                            break;

                        case SIMAT_PC_PLAY_TONE:
                            SIMAT_CnfPlayToneEx(dual_sys, &(stk_terminal_result.item.play_tone_cnf));
                            break;

                        case SIMAT_PC_DISPLAY_TEXT:
                            SIMAT_CnfDisplayTextEx(dual_sys, &(stk_terminal_result.item.display_text_cnf));
                            break;

                        case SIMAT_PC_GET_INKEY:
                            SIMAT_CnfGetInkeyEx(dual_sys, &(stk_terminal_result.item.get_inkey_cnf));
                            break;

                        case SIMAT_PC_GET_INPUT:
                            SIMAT_CnfGetInputEx(dual_sys, &(stk_terminal_result.item.get_input_cnf));
                            break;

                        case SIMAT_PC_SELECT_ITEM:
                            SIMAT_CnfSelItemEx(dual_sys, &(stk_terminal_result.item.select_item_cnf));
                            break;

                        case SIMAT_PC_SETUP_MENU:
                            if(SIMAT_TR_SUCCESS == stk_terminal_result.item.setup_menu_cnf.result)
                            {
                                SIMAT_CnfSetMenuEx(dual_sys, &(stk_terminal_result.item.setup_menu_cnf));
                            }
                            break;

                        case SIMAT_PC_PROVIDE_LOCATION_INFO:
                            if(out_data != NULL)
                            {
                                if(out_data[4]  == SIMAT_LI_LANG)
                                {
                                    SIMAT_CnfGetLangEx(dual_sys, &(stk_terminal_result.item.get_lang_cnf));
                                }
                                else
                                {
                                    ATC_ConvDttz(&(stk_terminal_result.item.get_dttz_cnf.dt_tz));                                                            
                                    SIMAT_CnfGetDttzEx(dual_sys, &(stk_terminal_result.item.get_dttz_cnf));
                                }
                            }
                            break;

                        case SIMAT_PC_SETUP_IDLE_MODE_TEXT:
                            SIMAT_CnfSetIdleTextEx(dual_sys, &(stk_terminal_result.item.idle_text_cnf));
                            break;

                        case SIMAT_PC_SETUP_EVENT_LIST:
                            SIMAT_CnfSetEventListEx(dual_sys, &(stk_terminal_result.item.setup_eventlist_cnf));
                            break;

                        case SIMAT_PC_SEND_DATA:
                            SIMAT_CnfSendDataEx(dual_sys, &(stk_terminal_result.item.send_data_cnf));
                            break;

                        case SIMAT_PC_RECEIVE_DATA:
                            SIMAT_CnfReceiveDataEx(dual_sys, &(stk_terminal_result.item.receive_data_cnf));
                            break;

                        case SIMAT_PC_LAUNCH_BROWSER:
                            SIMAT_CnfLaunchBrowserEx(dual_sys, &(stk_terminal_result.item.launch_browser_cnf));
                            break;

                        case SIMAT_PC_OPEN_CHANNEL:
                            SIMAT_CnfOpenChannelEx(dual_sys, &(stk_terminal_result.item.open_channel_cnf));
                            break;

                        case SIMAT_PC_CLOSE_CHANNEL:
                            SIMAT_CnfCloseChannelEx(dual_sys, &(stk_terminal_result.item.close_channel_cnf));
                            break;

                        case SIMAT_PC_GET_CHANNEL_STATUS:
                            SIMAT_CnfGetChannelStatusEx(dual_sys, &(stk_terminal_result.item.get_channel_status_cnf));
                            break;

                        default:
                            ret = FALSE;
                            break;
                    }
                }

                sprintf((char *)g_rsp_str, "%s: %d",
                        g_atc_info_table[AT_CMD_SPUSATTERMINAL].cmd_name->str_ptr, (BOOLEAN)(!ret));

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                free(out_data);
                //Kill the timer
                SCI_KILL_TIMER(P_ATC, (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                               &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
            }
            break;

        default:
            return_value = ERR_OPERATION_NOT_ALLOWED;
            break;

    }

    return return_value;
}

/*****************************************************************************/
//  Description : This function handles the item menu selection of AT+ESATENVECMD command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_EnvelopCmd_MenuSelection(
    SIMAT_MENU_SELECTION_IND_T *menu_select_ptr,
    MN_DUAL_SYS_E   dual_sys 
)
{
    BOOLEAN re_value = TRUE;

    if(!menu_select_ptr)
    {
        return FALSE;
    }

    SIMAT_IndMenuSelEx(dual_sys, menu_select_ptr);
    return  re_value;
}

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function handle the siganl  SIMAT_PROVIDE_LOCAL_INFO
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_HandleProvidLocalInfo(
    MN_DUAL_SYS_E    dual_sys,
    SIMAT_LOCAL_INFO_TYPE_E info_type
)
{
   SCI_TRACE_LOW("ATC:ATC_HandleProvidLocalInfo,info_type = %d",info_type);
   
   switch(info_type)
   {
    case SIMAT_LI_LOCATION_INFO: 
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

    	if(MNNV_GetIMEIEx(g_current_card_id, imei_arr))
    	{
    	    SCI_MEMCPY(imei_cnf.imei.imei_arr,imei_arr,SIMAT_IMEI_LEN);
    	    imei_cnf.result  = SIMAT_TR_SUCCESS;
    	    imei_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
    	}
    	else
    	{			
    	    SCI_TRACE_LOW("ATC: get imei failed.");
    	    imei_cnf.result = SIMAT_TR_MISS_INFO;
    	}
    }
     break;
    case SIMAT_LI_NMR:   //network measurement results
    {
    	SIMAT_NMR_BA_CNF_T nmr_cnf;
	nmr_cnf.result = SIMAT_TR_TERMINATED_BY_USER;		
        SIMAT_CnfNMRBAEx(dual_sys, &nmr_cnf);
    }
     break;

    case SIMAT_LI_DT_TZ: //data,time and time zone
    {
    	SIMAT_GET_DT_TZ_CNF_T dt_tz_cnf;
	dt_tz_cnf.result = SIMAT_TR_TERMINATED_BY_USER;		
        SIMAT_CnfGetDttzEx(dual_sys, &dt_tz_cnf);
    }
     break;

    case SIMAT_LI_LANG:  //language setting
    {
    	SIMAT_GET_LANG_CNF_T lang_cnf;
	lang_cnf.result = SIMAT_TR_TERMINATED_BY_USER;		
        SIMAT_CnfGetLangEx(dual_sys, &lang_cnf);
    }
     break;

    case SIMAT_LI_TIMING_AD: //timing advance
    {
    	SIMAT_TIMING_AD_CNF_T time_ad_cnf;
	time_ad_cnf.result = SIMAT_TR_TERMINATED_BY_USER;		
        SIMAT_CnfTimeAdvanceEx(dual_sys, &time_ad_cnf);
    }
     break;

     default:
     break;   
   }
}
/*****************************************************************************/
//  Description : This function handles the STK signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSTKSignal(
    ATC_CONFIG_T       *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr
)
{
	MN_DUAL_SYS_E     dual_sys = ATC_CURRENT_SIM_ID_MUX;
	SCI_TRACE_LOW("ATC:ATC_ProcessSTKSignal");
	
	switch(sig_ptr->SignalCode)
	{
	case APP_MN_SIMAT_SETUP_MENU_IND:
		{
			dual_sys = ((APP_MN_SIMAT_SETUP_MENU_IND_T*)sig_ptr)->dual_sys;

			SCI_MEMCPY(&s_stk_signal[dual_sys].setup_menu, &(((APP_MN_SIMAT_SETUP_MENU_IND_T*)sig_ptr)->com),sizeof(SIMAT_SETUP_MENU_T));
			//Save in the s_stk_setupmenu
			s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
	              s_stk_signal[dual_sys].is_support_stk = TRUE;
			
			//SCI_TRACE_LOW("ATC:STK_SETUP_MENU_REQ,%d",item_num);
			if(TRUE == atc_config_ptr->atc_config_nv.stk_actflag)
			{
				if(FALSE == s_is_setup_menu_cnf[dual_sys])
				{
					s_is_setup_menu_cnf[dual_sys] = TRUE;

					//Return ^STIN: 0
					sprintf((char*)g_rsp_str, "%s:0",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
					ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
				}
				ATC_InitCnfSetMenu(dual_sys);
			}									
		}
		break;

	case APP_MN_SIMAT_DISPLAY_TEXT_IND:
		
		dual_sys = ((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->dual_sys;
		
		SCI_TRACE_LOW("ATC:STK_DISPLAY_TEXT_REQ");
	       SCI_MEMCPY(&s_stk_signal[dual_sys].item.display_text, &(((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->com), sizeof(SIMAT_DISPLAY_TEXT_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_DISPLAYTEXT;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 1
		sprintf((char*)g_rsp_str, "%s:1",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys,atc_config_ptr, g_rsp_str);
		if(atc_config_ptr->atc_config_nv.stk_time > 0)
		{
                  SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
                      (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                      &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
		}
		break;

	case APP_MN_SIMAT_GET_INKEY_IND:
		
		dual_sys = ((APP_MN_SIMAT_GET_INKEY_IND_T*)sig_ptr)->dual_sys;
		
		SCI_TRACE_LOW("ATC:STK_GET_INKEY_REQ");
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.get_inkey, &(((APP_MN_SIMAT_GET_INKEY_IND_T*)sig_ptr)->com),sizeof(SIMAT_GET_INKEY_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_GETINKEY;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 2
		sprintf((char*)g_rsp_str, "%s:2", g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys,atc_config_ptr, g_rsp_str);
		if(atc_config_ptr->atc_config_nv.stk_time > 0)
		{
                  SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
                      (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                      &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
		}
		break;

	case APP_MN_SIMAT_GET_INPUT_IND:
		dual_sys = ((APP_MN_SIMAT_GET_INPUT_IND_T*)sig_ptr)->dual_sys;
		
		SCI_TRACE_LOW("ATC:STK_GET_INPUT_REQ");
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.get_input, &(((APP_MN_SIMAT_GET_INPUT_IND_T*)sig_ptr)->com),sizeof(SIMAT_GET_INPUT_T));
              s_stk_signal[dual_sys].is_support_stk = TRUE;
 		s_stk_signal[dual_sys].cur_status = ATC_STATUS_GETINPUT;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 3
		sprintf((char*)g_rsp_str, "%s:3",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys,atc_config_ptr, g_rsp_str);
		if(atc_config_ptr->atc_config_nv.stk_time > 0)
		{
                  SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
                      (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                      &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
		}
		break;
		
	case APP_MN_SIMAT_SETUP_CALL_IND:
		dual_sys = ((APP_MN_SIMAT_SETUP_CALL_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:STK_SETUP_CALL_REQ");
		
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.setup_call, &(((APP_MN_SIMAT_SETUP_CALL_IND_T*)sig_ptr)->com),sizeof(SIMAT_SETUP_CALL_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPCALL;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 4
		sprintf((char*)g_rsp_str, "%s:4",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys,atc_config_ptr, g_rsp_str);
		if(atc_config_ptr->atc_config_nv.stk_time > 0)
		{
                  SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
                      (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                      &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
		}
		break;

	case APP_MN_SIMAT_PLAY_TONE_IND:
		dual_sys = ((APP_MN_SIMAT_PLAY_TONE_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:SIMAT_PLAY_TONE_REQ");
		
             SCI_MEMCPY(&s_stk_signal[dual_sys].item.play_tone, &(((APP_MN_SIMAT_PLAY_TONE_IND_T*)sig_ptr)->com),sizeof(SIMAT_PLAY_TONE_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_PLAYTONE;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 5
		sprintf((char*)g_rsp_str, "%s:5",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		break;
		
	case APP_MN_SIMAT_SELECT_ITEM_IND:
		dual_sys = ((APP_MN_SIMAT_SELECT_ITEM_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:STK_SELECT_ITEM_REQ");
		
             SCI_MEMCPY(&s_stk_signal[dual_sys].item.select_item, &(((APP_MN_SIMAT_SELECT_ITEM_IND_T*)sig_ptr)->com),sizeof(SIMAT_SELECT_ITEM_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SELECTITEM;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 6
		sprintf((char*)g_rsp_str, "%s:6",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		if(atc_config_ptr->atc_config_nv.stk_time > 0)
		{
                  SCI_SET_TIMER(P_ATC, atc_config_ptr->atc_config_nv.stk_time*1000*30,
                      (ATC_MN_SIGNAL_E)(ATC_STSF_EXP_IND + dual_sys),
                      &T_ATC_STSF_EXP_IND[dual_sys], "T_ATC_STSF_EXP_IND");
		}
		break;

	case APP_MN_SIMAT_SEND_SS_IND:
		dual_sys = ((APP_MN_SIMAT_SEND_SS_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:SIMAT_SEND_SS_REQ");
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_ss,&(((APP_MN_SIMAT_SEND_SS_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_SS_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDSS;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;

		//Return ^STIN: 8
		sprintf((char*)g_rsp_str, "%s:8",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		break;
		
	case APP_MN_SIMAT_SEND_SMS_IND:
		dual_sys = ((APP_MN_SIMAT_SEND_SMS_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:STK_SEND_SMS_REQ");
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_sms, &(((APP_MN_SIMAT_SEND_SMS_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_SM_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDSMS;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;
		
		//Return ^STIN: 9
		sprintf((char*)g_rsp_str, "%s:9",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		break;

	case APP_MN_SIMAT_SEND_USSD_IND:
		dual_sys = ((APP_MN_SIMAT_SEND_USSD_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:SIMAT_SEND_USSD_REQ");
              SCI_MEMCPY(&s_stk_signal[dual_sys].item.send_ussd, &(((APP_MN_SIMAT_SEND_USSD_IND_T*)sig_ptr)->com),sizeof(SIMAT_SEND_USSD_T));
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SENDUSSD;
		s_cmd_stag[dual_sys] = ATC_CMD_INIT;

		//Return ^STIN: 10
		sprintf((char*)g_rsp_str, "%s:10",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
		ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		break;

	case APP_MN_SIMAT_PRO_LOCAL_INFO_IND:
		dual_sys = ((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:SIMAT_PROVIDE_LOCAL_INFO");
		ATC_HandleProvidLocalInfo(dual_sys,((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->com);		
		break;

	case SIMAT_SESSION_TERMINATED_IND:
		dual_sys = ((APP_MN_SIMAT_SESSION_TERMINATED_IND_T*)sig_ptr)->dual_sys;
		SCI_TRACE_LOW("ATC:STK_SESSION_TERMINATE_REQ");
		if((ATC_STATUS_SETUPMENU != s_stk_signal[dual_sys].cur_status) 
		     &&(TRUE == atc_config_ptr->atc_config_nv.stk_actflag))
		{
			//Return ^STIN: 99
			s_cmd_stag[dual_sys] = ATC_CMD_INIT;
			sprintf((char*)g_rsp_str, "%s:99",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
			ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
		}
		s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
		break;
		
	//signals which doesnt support	
    case SIMAT_SETUP_EVENT_LIST_REQ:
		{
                    SIMAT_SETUP_EVENT_LIST_CNF_T setup_event_list_cnf;
                    SCI_TRACE_LOW("ATC:SIMAT_EVENT_SETUP_EVENT_LIST");
                    setup_event_list_cnf.result = SIMAT_TR_TERMINATED_BY_USER;			
                    SIMAT_CnfSetEventListEx(dual_sys, &setup_event_list_cnf);
		}
		break;
			
    case APP_MN_SIMAT_SETUP_IDLE_TEXT_IND:  
            dual_sys = ((APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T*)sig_ptr)->dual_sys;
            
            SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F");
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.setup_idle_text, &(((APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T*)sig_ptr)->com), 
				      sizeof(SIMAT_SETUP_IDLE_TEXT_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPIDLEMODETEXT;
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
            
            //Return ^STIN: 12
            sprintf((char*)g_rsp_str, "%s:12",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
        break;

    case APP_MN_SIMAT_SEND_DTMF_IND: 
		{
			SIMAT_SEND_DTMF_CNF_T dtmf_cnf;
			dual_sys = ((APP_MN_SIMAT_SEND_DTMF_IND_T*)sig_ptr)->dual_sys;
			
			//SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SEND_DTMF_IND_F");	
			dtmf_cnf.result = SIMAT_TR_TERMINATED_BY_USER;			
                      SIMAT_CnfSendDtmfEx(dual_sys, &dtmf_cnf);
		}
		break;

    case APP_MN_SIMAT_LANG_NOTIFY_IND:   
		SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_LANG_NOTIFY_IND_F");
		break;

    case APP_MN_SIMAT_REFRESH_IND:
            dual_sys = ((APP_MN_SIMAT_REFRESH_IND_T*)sig_ptr)->dual_sys;
            
            SCI_MEMCPY(&s_stk_signal[dual_sys].item.refresh, &(((APP_MN_SIMAT_REFRESH_IND_T*)sig_ptr)->com),sizeof(SIMAT_REFRESH_T));
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_REFRESH;
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
            
            //Return ^STIN: 7
            sprintf((char*)g_rsp_str, "%s:7",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);

            ATC_ProcessRefreshCmd(atc_config_ptr, &s_stk_signal[dual_sys].item.refresh, dual_sys);
            break;
			
    case APP_MN_SIMAT_REFRESH_CNF:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_REFRESH_CNF");	
            ATC_ProcessRefreshCnf(atc_config_ptr, sig_ptr, ((APP_MN_SIMAT_REFRESH_CNF_T*)sig_ptr)->dual_sys);
            break;
			
    case APP_MN_SIMAT_LAUNCH_BROWSER_IND:
            dual_sys = ((APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)sig_ptr)->dual_sys;
            
            SCI_TRACE_LOW("ATC:EV_MN_APP_SIMAT_SETUP_IDLE_TEXT_IND_F");	
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPIDLEMODETEXT;
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
            
            //Return ^STIN: 11
            sprintf((char*)g_rsp_str, "%s:11",g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
            ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
            break;
		
    case SIMAT_MO_CC_RSP:    
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_CC_RSP");
		break;

    case SIMAT_EVENT_SS_CON_RSP:   
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_SS_CON_RSP");
		break;

    case SIMAT_EVENT_USSD_CON_RSP: 
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_USSD_CON_RSP");
		break;

    case EV_MN_APP_SIMAT_SMS_CTL_TO_MMI_IND_F:   
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_SMS_CON_RSP");
		break;

    case SIMAT_EVENT_SMS_PP_RSP:    
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_REFRESH");
		break;

    case SIMAT_EVENT_SMS_CB_RSP: 
		SCI_TRACE_LOW("ATC:SIMAT_EVENT_SMS_PP_RSP");
		break;

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
#endif /* _ULTRA_LOW_CODE_ */

int ATC_SignalCacheFreeSize(MN_DUAL_SYS_E dual_sys)
{
    int size;

    size = s_stk_signal_cache[dual_sys].read - s_stk_signal_cache[dual_sys].write;

    if(size <= 0)
    {
        size += s_stk_signal_cache[dual_sys].size;
    }

    size--;
    ATC_TRACE_LOW("ATC_SignalCacheFreeSize:size=%d\n", size);
    return size;
}

int ATC_SignalCacheSignalNum(MN_DUAL_SYS_E dual_sys)
{
    int size;

    size = s_stk_signal_cache[dual_sys].write - s_stk_signal_cache[dual_sys].read;

    if(size < 0)
    {
        size += s_stk_signal_cache[dual_sys].size;
    }

    ATC_TRACE_LOW("ATC_SignalCacheSignalNum:size=%d\n", size);
    return size;
}

BOOLEAN ATC_SignalCacheInsert(xSignalHeaderRec *sig_ptr, int size, MN_DUAL_SYS_E dual_sys)
{
    xSignalHeaderRec *sig_dst_ptr = NULL;

    ATC_TRACE_LOW("ATC_SignalCacheInsert\n");

    if(ATC_SignalCacheFreeSize(dual_sys) < 1)
    {
        return FALSE;
    }

    s_stk_is_pending[dual_sys] = TRUE;
    sig_dst_ptr = (xSignalHeaderRec *) & (s_stk_signal_cache[dual_sys].sig_list[s_stk_signal_cache[dual_sys].write]);

    SCI_MEMCPY(sig_dst_ptr, sig_ptr, size);

    s_stk_signal_cache[dual_sys].write = (s_stk_signal_cache[dual_sys].write + 1) % s_stk_signal_cache[dual_sys].size;

    return TRUE;
}

xSignalHeaderRec *ATC_SignalCacheGet(MN_DUAL_SYS_E dual_sys)
{
    xSignalHeaderRec *sig_ptr = NULL;

    ATC_TRACE_LOW("ATC_SignalCacheGet\n");

    if(ATC_SignalCacheSignalNum(dual_sys) < 1)
    {
        s_stk_is_pending[dual_sys] = FALSE;
        return NULL;
    }

    sig_ptr = (xSignalHeaderRec *) & (s_stk_signal_cache[dual_sys].sig_list[s_stk_signal_cache[dual_sys].read]);

    s_stk_signal_cache[dual_sys].read = (s_stk_signal_cache[dual_sys].read + 1) % s_stk_signal_cache[dual_sys].size;

    if(ATC_SignalCacheSignalNum(dual_sys) < 1)
    {
        s_stk_is_pending[dual_sys] = FALSE;
    }

    return sig_ptr;

}

/*****************************************************************************/
//  Description : This function handles the STK signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSTKSignalPending(
    ATC_CONFIG_T       *atc_config_ptr,
    xSignalHeaderRec   *sig_ptr,
    MN_DUAL_SYS_E   dual_sys
)
{
    ATC_STATUS ret = S_ATC_SUCCESS;
    ATC_TRACE_LOW("ATC:ATC_ProcessSTKSignalPending");

    if(s_ap_is_active[dual_sys] == TRUE && s_stk_is_pending[dual_sys] == FALSE)
    {
        ATC_TRACE_LOW("ATC:Status error");
        return ret;
    }

    ATC_TRACE_LOW("ATC :Sig_code=0x%04x", sig_ptr->SignalCode);

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_SIMAT_SETUP_MENU_IND:

            if(ATC_SignalCacheInsert(sig_ptr, sizeof(APP_MN_SIMAT_SETUP_MENU_IND_T), dual_sys) == TRUE)
            {
                ATC_InitCnfSetMenu(dual_sys);
            }
            else
            {
                SIMAT_SETUP_MENU_CNF_T  smenu;
                smenu.result = SIMAT_TR_ME_UNABLE;
                smenu.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetMenuEx(((APP_MN_SIMAT_SETUP_MENU_IND_T*)sig_ptr)->dual_sys, &smenu);
            }

            break;

        case APP_MN_SIMAT_DISPLAY_TEXT_IND:
            //cache display text command and waiting terminal response from AP
            if(ATC_SignalCacheInsert(sig_ptr, sizeof(APP_MN_SIMAT_DISPLAY_TEXT_IND_T), dual_sys) == TRUE)
            {
                SCI_TRACE_LOW("ATC:cache display text command and waiting terminal response from AP.");
                //ATC_DisplayTextCnf(ATC_DISPLAY_TEXT, ((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->dual_sys);
            }
            else
            {
                SIMAT_DISPLAY_TEXT_CNF_T display_text;
                display_text.result = SIMAT_TR_ME_UNABLE;
                display_text.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfDisplayTextEx(((APP_MN_SIMAT_DISPLAY_TEXT_IND_T*)sig_ptr)->dual_sys, &display_text);
            }

            break;

        case APP_MN_SIMAT_GET_INKEY_IND:
            {
                SIMAT_GET_INKEY_CNF_T get_inkey_cnf;
                SCI_MEMSET(&get_inkey_cnf, 0, sizeof(SIMAT_GET_INKEY_CNF_T));
                get_inkey_cnf.result = SIMAT_TR_ME_UNABLE;
                get_inkey_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfGetInkeyEx(((APP_MN_SIMAT_GET_INKEY_IND_T*)sig_ptr)->dual_sys, &get_inkey_cnf);
            }
            break;

        case APP_MN_SIMAT_GET_INPUT_IND:
            {
                SIMAT_GET_INPUT_CNF_T get_input_cnf;
                SCI_MEMSET(&get_input_cnf, 0, sizeof(SIMAT_GET_INPUT_CNF_T));
                get_input_cnf.result = SIMAT_TR_ME_UNABLE;
                get_input_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfGetInputEx(((APP_MN_SIMAT_GET_INPUT_IND_T*)sig_ptr)->dual_sys, &get_input_cnf);
            }
            break;

        case APP_MN_SIMAT_SETUP_CALL_IND:
            {
                SIMAT_SETUP_CALL_CNF_T setup_call_cnf;
                SCI_MEMSET(&setup_call_cnf, 0, sizeof(SIMAT_SETUP_CALL_CNF_T));
                setup_call_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_call_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetupCallEx(((APP_MN_SIMAT_SETUP_CALL_IND_T*)sig_ptr)->dual_sys, &setup_call_cnf);
            }
            break;

        case APP_MN_SIMAT_SELECT_ITEM_IND:
            {
                SIMAT_SELECT_ITEM_CNF_T select_item_cnf;
                SCI_MEMSET(&select_item_cnf, 0, sizeof(SIMAT_SELECT_ITEM_CNF_T));
                select_item_cnf.result = SIMAT_TR_ME_UNABLE;
                select_item_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSelItemEx(((APP_MN_SIMAT_SELECT_ITEM_IND_T*)sig_ptr)->dual_sys, &select_item_cnf);
            }
            break;

        case APP_MN_SIMAT_SEND_SMS_IND:
            {
                SIMAT_SEND_SMS_CNF_T sendsms_cnf;
                SCI_MEMSET(&sendsms_cnf, 0, sizeof(SIMAT_SEND_SMS_CNF_T));
                sendsms_cnf.result = SIMAT_TR_ME_UNABLE;
                sendsms_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSendSmsEx(((APP_MN_SIMAT_SEND_SMS_IND_T*)sig_ptr)->dual_sys, &sendsms_cnf);
            }
            break;

        case APP_MN_SIMAT_SEND_SS_IND:
            {
                SIMAT_SEND_SS_CNF_T  send_ss_cnf = {0};
                send_ss_cnf.result = SIMAT_TR_ME_UNABLE;
                send_ss_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSendSsEx(((APP_MN_SIMAT_SEND_SS_IND_T*)sig_ptr)->dual_sys, &send_ss_cnf);
            }
            break;

        case APP_MN_SIMAT_SEND_USSD_IND:
            {
                SIMAT_SEND_USSD_CNF_T send_ussd_cnf;
                SCI_MEMSET(&send_ussd_cnf, 0, sizeof(SIMAT_SEND_USSD_CNF_T));
                send_ussd_cnf.result = SIMAT_TR_ME_UNABLE;
                send_ussd_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSendUssdEx(((APP_MN_SIMAT_SEND_USSD_IND_T*)sig_ptr)->dual_sys, &send_ussd_cnf);
            }
            break;

        case APP_MN_SIMAT_PLAY_TONE_IND:
            {
                SIMAT_PLAY_TONE_CNF_T play_tone_cnf;
                play_tone_cnf.result = SIMAT_TR_ME_UNABLE;
                play_tone_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfPlayToneEx(((APP_MN_SIMAT_PLAY_TONE_IND_T*)sig_ptr)->dual_sys, &play_tone_cnf);
            }
            break;

        case APP_MN_SIMAT_PRO_LOCAL_INFO_IND:
            {
                ATC_TRACE_LOW("ATC:SIMAT_PROVIDE_LOCAL_INFO");

                if(((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->com == SIMAT_LI_DT_TZ)
                {
                    SIMAT_GET_DT_TZ_CNF_T dt_tz_cnf;
                    dt_tz_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                    SIMAT_CnfGetDttzEx(((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->dual_sys, &dt_tz_cnf);
                }
                else if(((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->com == SIMAT_LI_LANG)
                {
                    SIMAT_GET_LANG_CNF_T lang_cnf;
                    lang_cnf.result = SIMAT_TR_TERMINATED_BY_USER;
                    SIMAT_CnfGetLangEx(((APP_MN_SIMAT_PRO_LOCAL_INFO_IND_T*)sig_ptr)->dual_sys, &lang_cnf);
                }
            }
            break;

        case SIMAT_SESSION_TERMINATED_IND:
            ATC_SignalCacheInsert(sig_ptr, sizeof(*sig_ptr), dual_sys);
            break;

        case   SIMAT_SETUP_EVENT_LIST_REQ:
            if(ATC_SignalCacheInsert(sig_ptr, sizeof(SIMAT_SIG_SETUP_EVENT_LIST_REQ_T), dual_sys) == TRUE)
            {
                ATC_SetupEventListCnf(ATC_SETUP_EVENT_LIST, 0,
                                                         ((SIMAT_SIG_SETUP_EVENT_LIST_REQ_T*)sig_ptr)->dual_sys);
            }
            else
            {
                SIMAT_SETUP_EVENT_LIST_CNF_T setup_event_list_cnf;
                setup_event_list_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_event_list_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetEventListEx(((SIMAT_SIG_SETUP_EVENT_LIST_REQ_T*)sig_ptr)->dual_sys, &setup_event_list_cnf);
            }

            break;

        case APP_MN_SIMAT_SETUP_IDLE_TEXT_IND:
            {
                SIMAT_SETUP_IDLE_TEXT_CNF_T setup_idle_text_cnf;
                setup_idle_text_cnf.result = SIMAT_TR_ME_UNABLE;
                setup_idle_text_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSetIdleTextEx(((APP_MN_SIMAT_SETUP_IDLE_TEXT_IND_T*)sig_ptr)->dual_sys, &setup_idle_text_cnf);
            }
            break;

        case APP_MN_SIMAT_SEND_DTMF_IND:
            {
                SIMAT_SEND_DTMF_CNF_T send_dtmf_cnf;
                send_dtmf_cnf.result = SIMAT_TR_ME_UNABLE;
                send_dtmf_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSendDtmfEx(((APP_MN_SIMAT_SEND_DTMF_IND_T*)sig_ptr)->dual_sys, &send_dtmf_cnf);
            }
            break;
        case APP_MN_SIMAT_LANG_NOTIFY_IND:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_LANG_NOTIFY_IND");
            ATC_SignalCacheInsert(sig_ptr, sizeof(APP_MN_SIMAT_LANG_NOTIFY_IND_T), dual_sys);
            break;
        case APP_MN_SIMAT_REFRESH_IND:
            {
                SIMAT_REFRESH_CNF_T refresh_cnf;
                refresh_cnf.result = SIMAT_TR_ME_UNABLE;
                refresh_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfRefreshEx(((APP_MN_SIMAT_REFRESH_IND_T*)sig_ptr)->dual_sys, &refresh_cnf);
            }
            break;
        case APP_MN_SIMAT_REFRESH_CNF:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_REFRESH_CNF");
            ATC_ProcessRefreshCnf(atc_config_ptr, sig_ptr, ((APP_MN_SIMAT_REFRESH_CNF_T*)sig_ptr)->dual_sys);
            break;
        case APP_MN_SIMAT_SEND_DATA_IND:
            {
                SIMAT_SEND_DATA_CNF_T send_data_cnf;
                SCI_MEMSET(&send_data_cnf, 0, sizeof(SIMAT_SEND_DATA_CNF_T));
                send_data_cnf.result = SIMAT_TR_ME_UNABLE;
                send_data_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfSendDataEx(((APP_MN_SIMAT_SEND_DATA_IND_T*)sig_ptr)->dual_sys, &send_data_cnf);
            }
            break;
        case APP_MN_SIMAT_RECEIVE_DATA_IND:
            {
                SIMAT_RECEIVE_DATA_CNF_T receive_data_cnf;
                SCI_MEMSET(&receive_data_cnf, 0, sizeof(SIMAT_RECEIVE_DATA_CNF_T));
                receive_data_cnf.result = SIMAT_TR_ME_UNABLE;
                receive_data_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfReceiveDataEx(((APP_MN_SIMAT_RECEIVE_DATA_IND_T*)sig_ptr)->dual_sys, &receive_data_cnf);
            }
            break;
        case APP_MN_SIMAT_LAUNCH_BROWSER_IND:
            {
                SIMAT_LAUNCH_BROWSER_CNF_T lauch_browser_cnf;
                lauch_browser_cnf.result = SIMAT_TR_ME_UNABLE;
                lauch_browser_cnf.mp_info = SIMAT_LB_BROWSER_UNAVAILABLE_CAUSE;
                SIMAT_CnfLaunchBrowserEx(((APP_MN_SIMAT_LAUNCH_BROWSER_IND_T*)sig_ptr)->dual_sys, &lauch_browser_cnf);
            }
            break;
        case APP_SIMAT_OPEN_CHANNEL_IND:
            {
                SIMAT_OPEN_CHANNEL_CNF_T open_channel_cnf;
                SCI_MEMSET(&open_channel_cnf, 0, sizeof(SIMAT_OPEN_CHANNEL_CNF_T));
                open_channel_cnf.result = SIMAT_TR_ME_UNABLE;
                open_channel_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfOpenChannelEx(((APP_MN_SIMAT_OPEN_CHANNEL_IND_T*)sig_ptr)->dual_sys, &open_channel_cnf);
            }
            break;
        case APP_SIMAT_CLOSE_CHANNEL_IND:
            {
                SIMAT_CLOSE_CHANNEL_CNF_T close_channel_cnf;
                SCI_MEMSET(&close_channel_cnf, 0, sizeof(SIMAT_CLOSE_CHANNEL_CNF_T));
                close_channel_cnf.result = SIMAT_TR_ME_UNABLE;
                close_channel_cnf.mp_info = SIMAT_BI_NO_SPECIFIC_CAUSE;
                SIMAT_CnfCloseChannelEx(((APP_MN_SIMAT_CLOSE_CHANNEL_IND_T*)sig_ptr)->dual_sys, &close_channel_cnf);
            }
            break;
        case SIMAT_MO_CC_RSP:
            break;

        case SIMAT_EVENT_SS_CON_RSP:
            break;

        case SIMAT_EVENT_USSD_CON_RSP:
            break;

        case APP_MN_SIMAT_SMS_CTL_TO_MMI_IND:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_SMS_CTL_TO_MMI_IND");
            break;

        case SIMAT_EVENT_SMS_PP_RSP:
            break;

        case SIMAT_EVENT_SMS_CB_RSP:
            break;
        case APP_MN_SIMAT_MORE_TIME_IND:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_MORE_TIME_IND");
            break;
        case APP_MN_SIMAT_CC_TO_MMI_IND:
            ATC_TRACE_LOW("ATC:APP_MN_SIMAT_CC_TO_MMI_IND");
            break;
        case APP_SIMAT_GET_CHANNEL_STATUS_IND:
            {
                SIMAT_GET_CHANNEL_STATUS_CNF_T get_channel_status_cnf;
                SCI_MEMSET(&get_channel_status_cnf, 0, sizeof(SIMAT_GET_CHANNEL_STATUS_CNF_T));
                get_channel_status_cnf.result = SIMAT_TR_ME_UNABLE;
                get_channel_status_cnf.mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
                SIMAT_CnfGetChannelStatusEx(((APP_MN_SIMAT_GET_CHANNEL_STATUS_IND_T*)sig_ptr)->dual_sys, &get_channel_status_cnf);
            }
            break;
        default:
            SCI_ASSERT(FALSE);/*assert verified: check valid value*/
            break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : This function handles the STK signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessSTKSignalCache(
    ATC_CONFIG_T       *atc_config_ptr,
    MN_DUAL_SYS_E    dual_sys
)
{
    ATC_STATUS ret = S_ATC_FAIL;
    xSignalHeaderRec *sig_ptr = NULL;

    ATC_TRACE_LOW("ATC:ATC_ProcessSTKSignalCache");

    if(s_ap_is_active[dual_sys] == FALSE || s_stk_is_pending[dual_sys] == FALSE)
    {
        ATC_TRACE_LOW("ATC:Status error or NO Pending Event");
        return ret;
    }

    if(ATC_SignalCacheSignalNum(dual_sys) < 1)
    {
        ATC_TRACE_LOW("ATC:No pending Event\n");
        return ret;
    }

    sig_ptr = ATC_SignalCacheGet(dual_sys);

    if(sig_ptr == NULL)
    {
        ATC_TRACE_LOW("ATC:Sig_Ptr error");
        return ret;
    }

    switch(sig_ptr->SignalCode)
    {
        case APP_MN_SIMAT_SETUP_MENU_IND:
            ATC_ProcessSetupMenu(atc_config_ptr);
            ret = S_ATC_SUCCESS;
            break;

        case APP_MN_SIMAT_DISPLAY_TEXT_IND:
            ATC_ProcessDisplayText(atc_config_ptr);
            ret = S_ATC_SUCCESS;
            break;

        case SIMAT_SETUP_EVENT_LIST_REQ:
            ATC_ProcessSetupEventList(atc_config_ptr, sig_ptr, dual_sys);
            ret = S_ATC_SUCCESS;
            break;

        case SIMAT_SESSION_TERMINATED_IND:
            // set proactive report
            ATC_SET_CURRENT_LINK_ID_MUX(ATC_GetMuxLinkIdFromSimId(dual_sys));
            sprintf((char *)g_rsp_str, "+SPUSATENDSESSIONIND");
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPMENU;
            if(ATC_SignalCacheSignalNum(dual_sys) < 1)
            {
                s_stk_is_pending[dual_sys] = FALSE;
                ATC_TRACE_LOW("ATC:ATC_ProcessSTKSignalCache No pending Event\n");
            }
            else
            {
                ATC_ProcessSTKSignalCache(atc_config_ptr, dual_sys);
            }                
            ret = S_ATC_SUCCESS;
            break;
        
        case APP_MN_SIMAT_LANG_NOTIFY_IND:
            ATC_TRACE_LOW("ATC:ATC_ProcessSTKSignalCache APP_MN_SIMAT_LANG_NOTIFY_IND");
            ATC_ProcessLangNotify(atc_config_ptr,sig_ptr, dual_sys);
            ret = S_ATC_SUCCESS;
            break;
            
        default:
            ATC_TRACE_LOW("ATC: signal code:0x%04x, do nothing.", sig_ptr->SignalCode);
            break;
    }

    return ret;
}

/*****************************************************************************/
//  Description : This function handle the ATC_STSF_EXP_IND signal
//  Global resource dependence : none
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS  ATC_ProcessTimerEXp (ATC_CONFIG_T  *atc_config_ptr )
{
#ifndef _ULTRA_LOW_CODE_
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    switch(s_stk_signal[dual_sys].cur_status)
    {
        case ATC_STATUS_SETUPMENU:
        {
            s_cmd_stag[dual_sys] = ATC_CMD_INIT;
            if(TRUE == atc_config_ptr->atc_config_nv.stk_actflag)
            {
                sprintf((char*)g_rsp_str, "%s: 99",
                    g_atc_info_table[AT_CMD_xor_STIN].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            break;
        }

        case ATC_STATUS_DISPLAYTEXT:
        {
            ATC_DisplayTextCnf(dual_sys,ATC_NO_RESPONSE,0,0);
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
#endif
/*****************************************************************************/
//  Description : This function handle the ATC_ActualProfileData
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProfileToNVData(uint8 *profile_data, uint16 profile_len, uint8 *profile_to_nv, uint16 *profile_to_nv_len)
{
    int i = 0;

    SCI_ASSERT(profile_data != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(profile_to_nv != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(profile_to_nv_len != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(profile_len <= STK_PROFILE_CHAR_LEN);/*assert verified: check valid value*/

    *profile_to_nv_len = profile_len;

    for(i = 0; i < profile_len; i++)
    {
        profile_to_nv[i] = ((profile_data[i] & modem_supprot_profile[i]) | (ap_discard_profile[i]));

    }

}

/*****************************************************************************/
//  Description : This function handle the ATC_ActualProfileData
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProfileResultData(uint8 *profile_data, uint16 profile_len, uint8 *result_profile, uint16 *result_profile_len)
{
    int i = 0;

    SCI_ASSERT(profile_data != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(result_profile != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(result_profile_len != NULL);/*assert verified: check null pointer*/
    SCI_ASSERT(profile_len <= STK_PROFILE_CHAR_LEN);/*assert verified: check valid value*/

    *result_profile_len = profile_len;

    for(i = 0; i < profile_len; i++)
    {
        result_profile[i] = ((profile_data[i] & modem_supprot_profile[i]) & (~ap_discard_profile[i]));

    }

}

/*****************************************************************************/
//  Description : ATC_BuildProfileRsp
//  Global resource dependence : s_stk_signal
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildProfileRsp(
    ATC_CONFIG_T *atc_config_ptr,
    uint8 *data,
    uint16 len
)
{
    char data_str[STK_PROFILE_CHAR_LEN*2+1] = {0};

    ATC_TranUint8IntoChar(data, len, (uint8 *)data_str);

    sprintf((char *)g_rsp_str, "+SPUSATPROFILE:%s",
            data_str);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
}


/*****************************************************************************/
//  Description : This function check and handle what user input for terminal
//                profile
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
BOOLEAN ATC_CheckAndHandleConfig(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T *atc_config_ptr,
    char         *config_char_ptr
)
{
    uint8   config_str[STK_PROFILE_CHAR_LEN + 2];
    uint8   config_result[STK_PROFILE_RESULT_LEN];
    BOOLEAN re_value    = FALSE;
    uint8   i           = 0;


    SCI_MEMSET(config_str,   0, (STK_PROFILE_CHAR_LEN + 2));
    SCI_MEMSET(config_result, 0, STK_PROFILE_RESULT_LEN);

    SCI_MEMCPY(config_str, config_char_ptr, STK_PROFILE_CHAR_LEN);

    if(strcmp(config_char_ptr, g_stk_terminal_profile[AT_STK_TRP_MIN_VAL]) >= 0 &&
            strcmp(config_char_ptr, g_stk_terminal_profile[AT_STK_TRP_MAX_VAL]) <= 0)
    {
        //change it to the bit flow.
        while((i < STK_PROFILE_CHAR_LEN) && (config_str[i] != 0))
        {
            if((config_str[i] <= 0x39) && (config_str[i+1] <= 0x39))
            {
                config_result[i/2] = ((config_str[i] - 0x30) << 4) +
                                     (config_str[i+1] - 0x30);
            }
            else if((config_str[i] > 0x39) && (config_str[i+1] <= 0x39))
            {
                config_result[i/2] = ((config_str[i] - 0x41 + 10) << 4) +
                                     (config_str[i+1] - 0x30);
            }
            else if((config_str[i] <= 0x39) && (config_str[i+1] > 0x39))
            {
                config_result[i/2] = ((config_str[i] - 0x30) << 4) +
                                     (config_str[i+1] - 0x41 + 10);
            }
            else if((config_str[i] > 0x39) && (config_str[i+1] > 0x39))
            {
                config_result[i/2] = ((config_str[i] - 0x41 + 10) << 4) +
                                     (config_str[i+1] - 0x41 + 10);
            }
            else
            {
                return FALSE;
            }

            i = i + 2;
        }

        //Call the fucntion of SIM to store it.
        if(atc_config_ptr->sim_ready[dual_sys] && TRUE == s_stk_signal[dual_sys].is_support_stk)
        {

            ATC_TRACE_LOW(" add sim_terminal_profile!!!!!!");
            SIMAT_TerminalProfileEx(dual_sys, config_result, STK_PROFILE_RESULT_LEN);
            re_value = TRUE;
        }
        else
        {
            re_value = FALSE;
        }
    }

    ATC_TRACE_LOW("ATC:ATC_CheckAndHandleConfig,re_value=%d", re_value);
    return (re_value);
}

/*****************************************************************************/
//  Description : This function handles the AT+SPSTPARAM command
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSTPARAM)
{
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : it is uesd by UEIT to do refresh cmd like AP/MMI
//  Global resource dependence : g_rsp_str
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void ATC_ProcessRefreshCmd(
    ATC_CONFIG_T *atc_config_ptr,
    SIMAT_REFRESH_T *refresh_ptr,
    MN_DUAL_SYS_E  dual_sys
)
{
#ifdef ONTIM_PRJ
    ATC_REFRESH_FILE_T refresh_file = {0};
#endif

    ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCmd, refresh_type = %d ", refresh_ptr->refresh_type);

    switch(refresh_ptr->refresh_type)
    {
    case SIMAT_RF_RESET: 
#ifdef ONTIM_PRJ
        refresh_file.refresh_type = refresh_ptr->refresh_type;
        ATC_BuildRefreshCnf(atc_config_ptr, &refresh_file);
        s_stk_is_pending[dual_sys] = FALSE;
#else
        MNPHONE_PowerOffPsEx(dual_sys);
        s_stk_is_refresh_reset[dual_sys] = TRUE;
        g_is_stk_refresh_reset[dual_sys] = TRUE;
        s_stk_is_pending[dual_sys] = FALSE;//refresh reset not need terminal response
		
        if (ERR_MNDATAMAG_NO_ERR != MNSIM_RefreshEx(dual_sys, *refresh_ptr))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!");
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_SIMAT_REFRESH_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
        }   
#endif
        break;
    case SIMAT_RF_FILE_CHANGE:
    case SIMAT_RF_INITIAL:
    case SIMAT_RF_INITIAL_FILE_CHANGE:
	case SIMAT_RF_3G_SESSION_RESET:
        if (ERR_MNDATAMAG_NO_ERR != MNSIM_RefreshEx(dual_sys, *refresh_ptr))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!");
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_SIMAT_REFRESH_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
        }             
        break;
    case SIMAT_RF_INITIAL_FULL_FILE_CHANGE:
#ifdef GSM_DPHONE_SUPPORT
        //NOTICE: Maybe your encryption-sim card will send another 
        //refresh type which is NOT "SIMAT_RF_INITIAL_FULL_FILE_CHANGE".
        //then you should remove these code to other case XXX:
        SIMAT_DphoneRefreshEx(dual_sys,SIMAT_RF_INITIAL_FULL_FILE_CHANGE);
#else
        if (ERR_MNDATAMAG_NO_ERR != MNSIM_RefreshEx(dual_sys, *refresh_ptr))
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCmd, refresh cmd execute failure!");
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                                  (uint16)APP_MN_SIMAT_REFRESH_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);            
        }      
#endif
        break;
    case SIMAT_RF_RESERVED:
    default:
        ATC_TRACE_LOW("ATC: ATC_ProcessRefreshCmd, current refresh type is not support yet!");
        break;
    }
}

/*****************************************************************************/
//  Description : This function handles the AT+SPSTACTION command.
//                   This command is used to execute encryption card operation.
//  Global resource dependence : g_rsp_str
//  Author:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPSTACTION)
{
    return S_ATC_FAIL;
}

/*****************************************************************************/
//  Description : conv the bcd tel to str
//  Global resource dependence : 
//  Author: yongxia.zhang
/*****************************************************************************/
BOOLEAN ATC_ConvTelHexToAscii(uint8 tel_hex,uint8 *tel_ptr)
{
    BOOLEAN is_conv_success = TRUE;

    switch (tel_hex) 
    {
        case AT_DIALBCD_0:
        case AT_DIALBCD_1:
        case AT_DIALBCD_2:
        case AT_DIALBCD_3:
        case AT_DIALBCD_4:
        case AT_DIALBCD_5: 
        case AT_DIALBCD_6:
        case AT_DIALBCD_7:
        case AT_DIALBCD_8:
        case AT_DIALBCD_9:
            *tel_ptr = tel_hex + '0';
            break;
    
        case AT_DIALBCD_STAR:
            *tel_ptr = '*';         // *
            break;
    
        case AT_DIALBCD_HASH:
            *tel_ptr = '#';         // #
            break;
    
        case AT_DIALBCD_PAUSE:
            *tel_ptr = 'P';         // P
            break;
    
        case AT_DIALBCD_WILD:
            *tel_ptr = 'w';         // w 
            break;
    
        case AT_DIALBCD_EXPANSION:
            is_conv_success = FALSE;
            break;

        default:
            is_conv_success = FALSE;
            break;
        }
    
    return is_conv_success;
}
/*****************************************************************************/
//  Description : check whether the dtmf is present in the stk setup call string
//  Global resource dependence : 
//  Author: yongxia.zhang
//  Note:
/*****************************************************************************/
void ATC_CheckSTKCallNum(MN_CALLED_NUMBER_T *called_num,
                                    ATC_STK_CALL_INFO_T  *stk_call_info)    
{
    uint8 *bcd_ptr = PNULL;
    uint8 bcd_tel_len = 0;
    uint8 nibble[2] = {0, 0};
    uint8 i = 0,j = 0;
    uint8 dtmf_len = 0,tel_num = 0;
    BOOLEAN is_first_dtmf_seprator = TRUE;
    uint8 tel_str;

    SCI_ASSERT(called_num != PNULL);/* assert verified: null pointer */
    SCI_ASSERT(stk_call_info != PNULL);/* assert verified: null pointer */

    bcd_ptr = called_num->party_num;
    bcd_tel_len = called_num->num_len;

    SCI_MEMSET(stk_call_info,0,sizeof(ATC_STK_CALL_INFO_T));

    //init tel number
    stk_call_info->call_num.number_plan = called_num->number_plan;
    stk_call_info->call_num.number_type = called_num->number_type;  

    dtmf_len = 0;
    tel_num = 2*bcd_tel_len;
    for (i = 0; i < bcd_tel_len; i++) 
    {
        nibble[1] = ((*bcd_ptr) & (uint8)0xF0) >> 4;
        nibble[0] = (*bcd_ptr & (uint8)0x0F);

        for (j = 0; j <= 1; j++) 
        {
            if(nibble[j] == AT_DIALBCD_PAUSE)
            {
                if(is_first_dtmf_seprator)
                {
                    stk_call_info->is_dtmf = TRUE;
                    tel_num = 2*i + j;
                    is_first_dtmf_seprator = FALSE;
                    continue; //not copy seprator
                }
            }

            if(stk_call_info->is_dtmf)
            {
                if(ATC_ConvTelHexToAscii(nibble[j],&tel_str))
                {
                    stk_call_info->dtmf_str[dtmf_len++] = tel_str;
                }
                else
                {
                    i = bcd_tel_len;//stop the decode
                    break;
                }
            }
        }
        bcd_ptr++;
    }

    stk_call_info->dtmf_len = dtmf_len;
    if(dtmf_len == 0)
    {
        stk_call_info->is_dtmf = FALSE;
    }
    stk_call_info->dtmf_index = 0;

    SCI_MEMSET(&stk_call_info->call_num.party_num[0],0,MN_MAX_ADDR_BCD_LEN);
    if(tel_num%2 == 0)
    {
        stk_call_info->call_num.num_len = tel_num/2;
        SCI_MEMCPY(&stk_call_info->call_num.party_num[0],called_num->party_num,stk_call_info->call_num.num_len);
    }
    else
    {
        stk_call_info->call_num.num_len = (tel_num+1)/2;
        SCI_MEMCPY(&stk_call_info->call_num.party_num[0],called_num->party_num,stk_call_info->call_num.num_len);
        //last unused high nibble must be fill with 0xF
        stk_call_info->call_num.party_num[stk_call_info->call_num.num_len - 1] = stk_call_info->call_num.party_num[stk_call_info->call_num.num_len - 1] | (0xF0); 
    }
}

/**********************************************************************
//  Description:
//     start mo sms retry timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartDtmfTimer(MN_DUAL_SYS_E dual_sys,
                               uint32 timer_value)
{
    SCI_TRACE_LOW("ATC:: START STK DTMF TIMER.");

    if(NULL == s_stk_dtmf_send_timer[dual_sys])
    {
        s_stk_dtmf_send_timer[dual_sys] = SCI_CreateTimer(
                                             "AT STK DTMF SEND Timer",
                                             ATC_STKDTMFTimerExp,
                                             (uint32)dual_sys,
                                             timer_value,
                                             SCI_NO_ACTIVATE
                                         );
    }

    if(SCI_IsTimerActive(s_stk_dtmf_send_timer[dual_sys]))
    {
        SCI_DeactiveTimer(s_stk_dtmf_send_timer[dual_sys]);
    }

    SCI_ChangeTimer(s_stk_dtmf_send_timer[dual_sys], ATC_STKDTMFTimerExp, timer_value);
    SCI_ActiveTimer(s_stk_dtmf_send_timer[dual_sys]);

    return;
}

/**********************************************************************
//  Description:
//     send dtmf char
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKSendDtmf(MN_DUAL_SYS_E dual_sys)
{
    uint8 dtmf_ch;
    BOOLEAN is_vailid_dtmf = TRUE;

    SCI_TRACE_LOW("ATC:atc dtmf len = %d,cur_index = %d.",g_stk_call_info[dual_sys].dtmf_len,g_stk_call_info[dual_sys].dtmf_index );

    if(( g_stk_call_info[dual_sys].is_dtmf) && 
        (g_stk_call_info[dual_sys].dtmf_len > 0))   
    {
        for(;g_stk_call_info[dual_sys].dtmf_index < g_stk_call_info[dual_sys].dtmf_len;g_stk_call_info[dual_sys].dtmf_index++)
        {
            dtmf_ch = g_stk_call_info[dual_sys].dtmf_str[g_stk_call_info[dual_sys].dtmf_index];

            if(!((dtmf_ch >= '0' && dtmf_ch <= '9') || 
            (dtmf_ch == '*') ||
            (dtmf_ch == '#' ||
            (dtmf_ch =='p') ||
            (dtmf_ch =='P') ||
            (dtmf_ch =='w') ||
            (dtmf_ch =='W')))
            )
            {
                  SCI_TRACE_LOW("ATC: is invalid dtmf %c.",dtmf_ch);
                is_vailid_dtmf = FALSE;
            }

            if(is_vailid_dtmf)
            {
                if((dtmf_ch == 'p') ||(dtmf_ch == 'P'))
                {
                    ATC_StartDtmfTimer(dual_sys,AT_STK_DTMF_PAUSE_TIME);
                    g_stk_call_info[dual_sys].dtmf_index++;
                    return;
                }
                else
                {
                    if(MN_RETURN_SUCCESS != MNCALL_StartDTMFEx(dual_sys,dtmf_ch,g_stk_call_info[dual_sys].call_index))
                    {
                        //stop the dtmf send
                        g_stk_call_info[dual_sys].is_dtmf = FALSE;
                        g_stk_call_info[dual_sys].dtmf_len = 0;
                        return;
                    }

                    if(MN_RETURN_SUCCESS != MNCALL_StopDTMFEx(dual_sys,g_stk_call_info[dual_sys].call_index))
                    {
                        //stop the dtmf send
                        g_stk_call_info[dual_sys].is_dtmf = FALSE;
                        g_stk_call_info[dual_sys].dtmf_len = 0;
                        return;
                    }
                }
            }
        }

        //while end 
        g_stk_call_info[dual_sys].is_dtmf = FALSE;
        g_stk_call_info[dual_sys].dtmf_len = 0;
        g_stk_call_info[dual_sys].call_index = ATC_INVALID_CALLID;  
    }
}
    
/**********************************************************************
//  Description:
//     handle dtmf timer exp
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKDTMFTimerExp(uint32 lParam)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)lParam;

    SCI_TRACE_LOW("ATC:: STK DTMF TIMER EXP.");

    ATC_STKSendDtmf(dual_sys);

    return;
}

/**********************************************************************
//  Description:
//     stop the dtmf timer when call disconnected
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
void ATC_StkStopDtmfTimer(MN_DUAL_SYS_E dual_sys,uint8 call_index)
{
    if((g_stk_call_info[dual_sys].is_dtmf) && 
        (call_index == g_stk_call_info[dual_sys].call_index))   
    {
        if(NULL == s_stk_dtmf_send_timer[dual_sys])
        {
            return;
        }

        if(SCI_IsTimerActive(s_stk_dtmf_send_timer[dual_sys]))
        {
            SCI_DeactiveTimer(s_stk_dtmf_send_timer[dual_sys]);
        }
        
        //clear the dtmf variable
        g_stk_call_info[dual_sys].is_dtmf = FALSE;
        g_stk_call_info[dual_sys].dtmf_index = 0;
        g_stk_call_info[dual_sys].dtmf_len = 0;
        g_stk_call_info[dual_sys].call_index = ATC_INVALID_CALLID;
    }

    return;
}

/**********************************************************************
//  Description:
//     get the detail fail cause of sending sms failed
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_GetDetailSmsError(MN_DUAL_SYS_E dual_sys, SIMAT_SEND_SMS_CNF_T  *send_sms_cnf_ptr)
{
    MN_SEND_SMS_SMS_CAUSE_T fail_cause;

    SCI_MEMSET(&fail_cause, 0, sizeof(MN_SEND_SMS_SMS_CAUSE_T));
    SCI_MEMSET(send_sms_cnf_ptr,0,sizeof(SIMAT_SEND_SMS_CNF_T));

    fail_cause = MNSMS_GetLastMoFailCauseEx(dual_sys);

    switch(fail_cause.cause_type)
    {
        case MN_SEND_SMS_SUCCESS:
            send_sms_cnf_ptr->result = SIMAT_TR_SUCCESS;
            break;
        case MN_SEND_SMS_CANCEL_SUCCESS:
            send_sms_cnf_ptr->result = SIMAT_TR_TERMINATED_BY_USER;
            break;
        case MN_SEND_SMS_PLMN_NO_SER:
        case MN_SEND_SMS_SIM_NOT_READY:
            send_sms_cnf_ptr->result = SIMAT_TR_ME_UNABLE;
            send_sms_cnf_ptr->mp_info = SIMAT_MP_NO_SERVICE;
            break;
        case MN_SEND_SMS_MN_BUSY:
            send_sms_cnf_ptr->result = SIMAT_TR_ME_UNABLE;
            send_sms_cnf_ptr->mp_info = SIMAT_MP_BUSY_ON_CALL;
            break;
        case MN_SEND_SMS_FDN_FAILED:
            //for stk send sms,it should never go to here
            send_sms_cnf_ptr->result = SIMAT_TR_ME_UNABLE;
            send_sms_cnf_ptr->mp_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
        case MN_SEND_SMS_STK_NOT_ALLOWED:
            send_sms_cnf_ptr->result = SIMAT_TR_PERM_PROBLEM_INN_CC;
            send_sms_cnf_ptr->cc_info = SIMAT_CS_CONTROL_ACTION_NOT_ALLOWED_CAUSE;
            break;
        case MN_SEND_SMS_LOW_LAYER_FAIL:
            send_sms_cnf_ptr->result = SIMAT_TR_NE_UNABLE;
            send_sms_cnf_ptr->ne_info = SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
        case MN_SEND_SMS_CP_ERROR:
            send_sms_cnf_ptr->result = SIMAT_TR_NE_UNABLE;
            send_sms_cnf_ptr->ne_info = fail_cause.cause_value;
            break;
        case MN_SEND_SMS_RP_ERROR:
            send_sms_cnf_ptr->result = SIMAT_TR_SMS_ERROR;
            send_sms_cnf_ptr->sms_info =  fail_cause.cause_value;
            break;
        default:
            send_sms_cnf_ptr->result = SIMAT_TR_SMS_ERROR;
            send_sms_cnf_ptr->sms_info =  SIMAT_MP_NO_SPECIFIC_CAUSE;
            break;
    }

    return;
}

/**********************************************************************
//  Description:
//      the function conv the dttz from bcd to decimal,because the interface for atc and L4 is decimal,but the data
//    from the AP is BCD
//  Global resource dependence :
//  Author:Yongxia Zhang
//  Note:
***********************************************************************/
void ATC_ConvDttz(SIMAT_DO_DT_TZ_T * dttz_ptr)    //in:the get date-time and time-zone confirm
{
    uint8 high_nibble = 0,low_nibble = 0;
    uint8 i = 0;
    
       SCI_ASSERT(dttz_ptr != PNULL); /*assert verified*/
    
    for(i = 0; i < 6;i++)
    {
        high_nibble = dttz_ptr->dt_tz_arr[i] & 0xf;
        low_nibble = (dttz_ptr->dt_tz_arr[i] & 0xf0) >> 4;
        dttz_ptr->dt_tz_arr[i]  = high_nibble*10 + low_nibble;
    }

    //time zone is special,bit3 is the sign(0 positive,1:negative)
    high_nibble = dttz_ptr->dt_tz_arr[6] & 0x7;
    low_nibble = (dttz_ptr->dt_tz_arr[6] & 0xf0) >> 4; 
    if(dttz_ptr->dt_tz_arr[6] & 0x08)
    {
        dttz_ptr->dt_tz_arr[6]  = - (high_nibble*10 + low_nibble);
    }
    else
    {
        dttz_ptr->dt_tz_arr[6]  = high_nibble*10 + low_nibble;
    }
}

/**********************************************************************
//  Description:
//     start mo sms retry timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartMaxRedialDurationTimer(MN_DUAL_SYS_E dual_sys,
                               uint32 timer_value)
{
    if(NULL == s_stk_max_redial_duration_timer[dual_sys])
    {
        s_stk_max_redial_duration_timer[dual_sys] = SCI_CreateTimer(
                                             "AT STK MAX REDIAL DURATION Timer",
                                             ATC_STKMaxRedialDurationTimerExp,
                                             (uint32)dual_sys,
                                             timer_value,
                                             SCI_NO_ACTIVATE
                                         );
    }

    if(SCI_IsTimerActive(s_stk_max_redial_duration_timer[dual_sys]))
    {
        SCI_DeactiveTimer(s_stk_max_redial_duration_timer[dual_sys]);
    }

    timer_value = MIN(timer_value,AT_STK_MAX_REDIAL_DURATION);
    
    SCI_TRACE_LOW("ATC:: start stk max redial duration timer,timer val is %d.",timer_value);
    SCI_ChangeTimer(s_stk_max_redial_duration_timer[dual_sys], ATC_STKMaxRedialDurationTimerExp, timer_value);
    SCI_ActiveTimer(s_stk_max_redial_duration_timer[dual_sys]);

    return;
}

/**********************************************************************
//  Description:
//     handle stk setup call max redial duration timer expired
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKMaxRedialDurationTimerExp(uint32 lParam)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)lParam;
    SIMAT_SETUP_CALL_CNF_T  setup_call_cnf = {0};   

    SCI_TRACE_LOW("ATC:: stk setup call redial max duration timer expried.");

    ATC_StkStopRedialTimer(dual_sys);

    s_stk_redial_info[dual_sys].is_max_redial_duration_exp = TRUE;  
    
    //if the call setp has not been successful and the time elapsed since the first call setup attempt has exceeded
    //the duration requested by the SIM,then the ME shall inform the SIM using TEMINAL RESPONSE(network currently
    //unable to process command),and the redial mechanism shall be terminated
    if(s_stk_redial_info[dual_sys].is_first_call_attempt_finished)      
    {
        if(s_stk_redial_info[dual_sys].is_redial_ongoing)
        {
            //terminate the redial call
            SCI_TRACE_LOW("ATC::there is a redial call is ongoing,so terminate the %d th call.",g_stk_call_info[dual_sys].call_index);

            MNCALL_DisconnectCallEx(dual_sys,g_stk_call_info[dual_sys].call_index, MN_CAUSE_NORMAL_CLEARING,PNULL);

            s_stk_redial_info[dual_sys].is_self_disc_for_exp = TRUE;
        }
        else
        {
            SCI_TRACE_LOW("ATC:: do not redial for max duration timer expried,the prev fail cause is 0x%x.",s_stk_redial_info[dual_sys].fail_cause);
            g_is_stk_call[dual_sys] = FALSE;
            SCI_MEMSET(&setup_call_cnf, 0x0, sizeof(SIMAT_SETUP_CALL_CNF_T));
            setup_call_cnf.result = SIMAT_TR_NE_UNABLE;
            setup_call_cnf.ne_info = s_stk_redial_info[dual_sys].fail_cause|0x80;                     
            SIMAT_CnfSetupCallEx(dual_sys, &setup_call_cnf);
            ClearRedialInfo(dual_sys);
        }
    }

    return;
}

/**********************************************************************
//  Description:
//     stop the setup call max redial duration timer 
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkStopMaxRedialDurationTimer(MN_DUAL_SYS_E dual_sys)
{
    if(NULL != s_stk_max_redial_duration_timer[dual_sys])
    {
        if(SCI_IsTimerActive(s_stk_max_redial_duration_timer[dual_sys]))
        {
            SCI_TRACE_LOW("ATC:: stop max redial duration timer");
            SCI_DeactiveTimer(s_stk_max_redial_duration_timer[dual_sys]);
        }
    }
    return;
}

/**********************************************************************
//  Description:
//     start setup call redial timer when the stk setup call is not sucessfully
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StartRedialTimer(MN_DUAL_SYS_E dual_sys,uint32 timer_value)
{
    if(NULL == s_stk_redial_timer[dual_sys])
    {
        s_stk_redial_timer[dual_sys] = SCI_CreateTimer(
                                         "AT STK REDIAL Timer",
                                         ATC_STKRedialTimerExp,
                                         (uint32)dual_sys,
                                         timer_value,
                                         SCI_NO_ACTIVATE
                                     );
    }

    //first deactive timer  
    if(SCI_IsTimerActive(s_stk_redial_timer[dual_sys]))
    {
        SCI_DeactiveTimer(s_stk_redial_timer[dual_sys]);
    }

    SCI_TRACE_LOW("ATC:: start redial timer,timer val is %d.",timer_value);

    s_stk_redial_info[dual_sys].curr_redial_cnt++;
    SCI_ChangeTimer(s_stk_redial_timer[dual_sys], ATC_STKRedialTimerExp, timer_value);
    SCI_ActiveTimer(s_stk_redial_timer[dual_sys]);

    return;
}

/**********************************************************************
//  Description:
//     handle dtmf timer exp
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_STKRedialTimerExp(uint32 lParam)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)lParam;

    uint8   *temp_ptr = NULL;
    uint16   length = 0;

//#ifdef _MUX_ENABLE_
//    ATC_CONFIG_T   *atc_config_ptr = ATC_GetConfigInfoPtr();
//#endif

    SCI_TRACE_LOW("ATC:: stk setup call redial timer expired.");

    /*report +SPUSATSETUPCALL:<string>*/
    sprintf((char*)g_rsp_str, "+SPUSATSETUPCALL:");
    length = strlen((char*)g_rsp_str);
    temp_ptr = &g_rsp_str[length];
    //add <string>
    ATC_TranUint8IntoChar(s_stk_redial_info[dual_sys].raw_apdu_ptr,
                            s_stk_redial_info[dual_sys].raw_apdu_len,
                            (uint8 *)temp_ptr);
    
    length += s_stk_redial_info[dual_sys].raw_apdu_len * 2;
    SCI_ASSERT(length < MAX_ATC_RSP_LEN);/* assert verified: NULL_PTR */
    g_rsp_str[length] = '\0';

    s_stk_signal[dual_sys].cur_status = ATC_STATUS_SETUPCALL;
//#ifdef _MUX_ENABLE_
    //SCI_TRACE_LOW("ATC: ATC_STKRedialTimerExp current_link_id=%d", atc_config_ptr->current_link_id);
//#endif
    
    ATC_WriteCmdRsp(dual_sys, (uint8 *)g_rsp_str, strlen((char*)g_rsp_str));
    
    return;
}

/**********************************************************************
//  Description:
//     stop the dtmf timer when call disconnected
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkStopRedialTimer(MN_DUAL_SYS_E dual_sys)
{
    if(NULL != s_stk_redial_timer[dual_sys])
    {
        if(SCI_IsTimerActive(s_stk_redial_timer[dual_sys]))
        {
            SCI_TRACE_LOW("ATC:: stk stop redial timer.");
            SCI_DeactiveTimer(s_stk_redial_timer[dual_sys]);
        }
    }
    
    return;
}

/**********************************************************************
//  Description:
//     judge whether need redial
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsNeedRedial(
                            MN_DUAL_SYS_E                       dual_sys,
                            ATC_STK_SETUP_CALL_FAIL_TYPE_E      fail_type,
                                MN_CALL_DISCONNECT_CALL_CAUSE_E     fail_cause,
                                uint8 *                             redial_category)
{
    //judge the setup call type,whether need
    if(s_stk_redial_info[dual_sys].is_need_redial_type)
    {
        //judge whether the max redial duration is exceeded
        if(s_stk_redial_info[dual_sys].is_max_redial_duration_exp)
        {
            SCI_TRACE_LOW("ATC::max duration is exceeded,so do not redial.");
            return FALSE;
        }

        //judge whether the fail cause is the need redial cause         
        if(IsNeedRedialCause(dual_sys,fail_type,fail_cause,redial_category))
        {
            //judge whether the max redial cnt is exceeded
            if(s_stk_redial_info[dual_sys].curr_redial_cnt > s_stk_redial_info[dual_sys].max_redial_cnt)
            {
                SCI_TRACE_LOW("ATC::max redial count limit is exceeded,so do not redial.");
                return FALSE;
            }
            else
            {
                SCI_TRACE_LOW("ATC::need redial.");
                return TRUE;
            }               
        }
        else
        {
            SCI_TRACE_LOW("ATC::fail cause is not the need redial cause,so do not redial.");
            return FALSE;
        }
    }

    return FALSE;
}

/**********************************************************************
//  Description:
//     judge whether call failed cause is the need redial cause
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsNeedRedialCause(
                                MN_DUAL_SYS_E                       dual_sys,
                                ATC_STK_SETUP_CALL_FAIL_TYPE_E      fail_type,
                                    MN_CALL_DISCONNECT_CALL_CAUSE_E     fail_cause,
                                    uint8 *                             fail_category)
{
    BOOLEAN is_need_redial = FALSE;

    *fail_category = 0;
    
    if((fail_type == ATC_STK_CALL_SUCC) ||
            (fail_type == ATC_STK_CALL_FAIL_ERROR))
    {
        is_need_redial = FALSE;
    }
    else
    {
        //according to the  GSM02.07
        if(fail_cause == 17)
        {
            is_need_redial = TRUE;
            *fail_category = 1;
        }
        else if((fail_cause == 18) ||
                (fail_cause == 19) ||
                (fail_cause == 27) ||
                (fail_cause == 34) ||
                (fail_cause == 41) ||
                (fail_cause == 42) ||
                (fail_cause == 44) ||
                (fail_cause == 47) )
        {
            is_need_redial = TRUE;
            *fail_category = 2;
        }
        else if((fail_cause == 1) ||
                (fail_cause == 3) ||
                (fail_cause == 22) ||
                (fail_cause == 28) ||
                (fail_cause == 38) )
        {
            is_need_redial = TRUE;
            *fail_category = 3;
        }
    }           

    if((*fail_category == 1) ||
        (*fail_category == 2))
    {
        s_stk_redial_info[dual_sys].max_redial_cnt = 10; 
    }
    else if(*fail_category == 3)
    {
        s_stk_redial_info[dual_sys].max_redial_cnt = 1; 
    }
    else
    {
        s_stk_redial_info[dual_sys].max_redial_cnt = 0; 
    }
    
    return is_need_redial;
}

/**********************************************************************
//  Description:
//     judge whether call failed cause is the need redial cause
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void GetRedialTimerVal(
                        MN_DUAL_SYS_E   dual_sys,
                        uint8           redial_category,
                        uint32          *timer_val)
{
    uint8 curr_redial_cnt = s_stk_redial_info[dual_sys].curr_redial_cnt;
    
    *timer_val = 0;

    if((redial_category == 1) ||
        (redial_category == 2))
    {
        s_stk_redial_info[dual_sys].max_redial_cnt = 10; 
    }
    else
    {
        s_stk_redial_info[dual_sys].max_redial_cnt = 1; 
    }

    if(curr_redial_cnt <= s_stk_redial_info[dual_sys].max_redial_cnt)
    {
        //according to the  GSM02.07
        if(curr_redial_cnt == 1)
        {
            *timer_val = 5000;//5s
        }
        else if(curr_redial_cnt <= 4)
        {
            *timer_val = 60000;//60s=1min
        }
        else
        {
            *timer_val = 180000;//180s = 3min 
        }
    }
}

/**********************************************************************
//  Description:
//     judge whether need redial,if so,then save call parameter and start redial max duration timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ATC_StkPrepareRedial(MN_DUAL_SYS_E dual_sys)
{
    //start redial duration timer
    if(IsRedialCallType(s_stk_signal[dual_sys].item.setup_call.call_parameter.call_type))
    {
        if(s_stk_signal[dual_sys].item.setup_call.call_parameter.duration != 0)
        {
            s_stk_redial_info[dual_sys].is_need_redial_type = TRUE;
            s_stk_redial_info[dual_sys].is_first_call_attempt_finished = FALSE;
            s_stk_redial_info[dual_sys].is_max_redial_duration_exp = FALSE;
            s_stk_redial_info[dual_sys].is_redial_ongoing = FALSE;
            s_stk_redial_info[dual_sys].is_self_disc_for_exp = FALSE;
            s_stk_redial_info[dual_sys].curr_redial_cnt = 1;
            s_stk_redial_info[dual_sys].max_redial_cnt = 1;
            s_stk_redial_info[dual_sys].fail_cause = MN_CAUSE_USER_BUSY;

            ATC_StartMaxRedialDurationTimer(dual_sys,s_stk_signal[dual_sys].item.setup_call.call_parameter.duration);
        }
        else
        {
            SCI_TRACE_LOW("ATC::redial duration is invalid,so do not redial.");
            s_stk_redial_info[dual_sys].is_need_redial_type = FALSE;
        }
    }
}

/**********************************************************************
//  Description:
//     judge whether the call is disconnect by AP or Modem. the call may be disconnect by AP,In this case
//   the terminal response to the SIM is 0x23(user cleared down call before connection or network release)
//    If the call is disc for the stk max redial duration is exceeded,in this case the terminal response to the SIM is 0x21
//  (network currently unable to process command)
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsDiscByAP(MN_DUAL_SYS_E dual_sys, uint8 call_id)
{
    SCI_TRACE_LOW("ATC::IsDiscByAP,the call id is %d.",call_id);
    //if the call is self disconnect and is not disconnect by modem for the max redial expired
    if(ATC_IsSelfDisc(call_id) && (!s_stk_redial_info[dual_sys].is_self_disc_for_exp))
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//  Description:
//     judge whether the call type is need redial type
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL BOOLEAN IsRedialCallType(SIMAT_SETUP_CALL_TYPE_E call_type)
{
    if((call_type == SIMAT_STC_FREE_REDIAL) ||
            (call_type == SIMAT_STC_HOLD_REDIAL) ||
            (call_type == SIMAT_STC_DISCON_REDIAL))
    {
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************
//  Description:
//     free the setup call raw apdu,it is saved for setup call redial
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void FreeSetupCallRawApduPtr(MN_DUAL_SYS_E dual_sys)
{
    if(s_stk_redial_info[dual_sys].raw_apdu_ptr != NULL)
    {
        SCI_FREE(s_stk_redial_info[dual_sys].raw_apdu_ptr);
    }
}

/**********************************************************************
//  Description:
//     clear the redial global variable and stop related timer
//  Global resource dependence :
//  Author:yongxia.zhang
//  Note:
***********************************************************************/
LOCAL void ClearRedialInfo(MN_DUAL_SYS_E dual_sys)
{
    //stop redial timer
    ATC_StkStopMaxRedialDurationTimer(dual_sys);
    ATC_StkStopRedialTimer(dual_sys);

    FreeSetupCallRawApduPtr(dual_sys);
  
    SCI_MEMSET(&s_stk_redial_info[dual_sys],0,sizeof(ATC_STK_REDIAL_INFO_T));
}

#ifdef ONTIM_PRJ
LOCAL uint16 ATC_AddStringComma(uint8* str, uint16 str_len, uint8 file_number, uint8* com_str)
{
    int j = 0;
    
    for(j=0; j<str_len; j++)
    {
        if((('4'==str[j])&&('F'==str[j+1] || 'f'==str[j+1]))||(('6'==str[j])&&('F'==str[j+1] || 'f'==str[j+1])))
        {
            strncpy((char*)com_str, (char*)&str[j], 4);
            com_str=com_str+4;
            *com_str++ = ',';
            j=j+3;
        }
        else
        {
            *com_str++ = str[j];
        }
    }
    
    return str_len+file_number;
}

/*****************************************************************************/
//  Description : Send the response of refresh info. and result
//  Global resource dependence : s_stk_signal
//  Author: Eddie.Wang
//  Note:
/*****************************************************************************/
LOCAL void ATC_BuildRefreshCnf(
    ATC_CONFIG_T *atc_config_ptr,
    ATC_REFRESH_FILE_T *refresh_ptr
)
{
    ATC_REFRESH_TYPE_E refresh_type = ATC_SIM_REFRESH_UNKOWN;
    uint8   *temp_ptr = NULL;
    //uint8    *file_id_ptr = NULL;
    uint16   length = 0;
    uint8    str[200]= {0};
    uint8    com_str[250] = {0};
    uint16   com_len = 0 ;
    
    temp_ptr = g_rsp_str;

    /*report +SPUSATREFRESH: <refresh_result>,<file_num>,<file_path>*/
    sprintf((char *)g_rsp_str, "+SPUSATREFRESH:");
    length = strlen((char *)g_rsp_str);
    temp_ptr = &g_rsp_str[length];

    /*add <refresh_result>*/
    switch (refresh_ptr->refresh_type)
    {
        case SIMAT_RF_INITIAL_FULL_FILE_CHANGE:
        case SIMAT_RF_INITIAL_FILE_CHANGE:
        case SIMAT_RF_INITIAL:
            refresh_type = ATC_SIM_REFRESH_INIT;
            break;

        case SIMAT_RF_FILE_CHANGE:
            refresh_type = ATC_SIM_REFRESH_FILE_UPDATE;
            break;

        case SIMAT_RF_RESET:
        case SIMAT_RF_3G_APPLICATIN_RESET:
        case SIMAT_RF_3G_SESSION_RESET:
            refresh_type = ATC_SIM_REFRESH_RESET;
            break;

        case SIMAT_RF_RESERVED:
        default:
            refresh_type = ATC_SIM_REFRESH_UNKOWN;
            break;
    }

    sprintf((char *)temp_ptr, "%d",refresh_type);
    length += 1;
    temp_ptr = &g_rsp_str[length];

    if(refresh_ptr->is_files_exist)
    {
        /*add <file_num>*/
        sprintf((char *)temp_ptr, ",%d," ,refresh_ptr->file_list.file_num);
        length += 3;
        temp_ptr = &g_rsp_str[length];

        /*add <file_path>*/
        ATC_TranUint8IntoChar(
            refresh_ptr->file_list.file_data_arr,
            refresh_ptr->file_list.file_data_len,
            str
            );
        com_len = (refresh_ptr->file_list.file_data_len) * 2;
        com_len = ATC_AddStringComma(str, com_len, refresh_ptr->file_list.file_num, com_str);
        sprintf((char*)temp_ptr, "%s" , com_str);
        length += com_len;
        SCI_ASSERT(length < MAX_ATC_RSP_LEN);
        g_rsp_str[length-1] = '\0';
    }
    else
    {
        /*no file*/
        sprintf((char *)temp_ptr, ",0,0");
    }

    ATC_BuildUnsolicitedInfo(ATC_CURRENT_SIM_ID_MUX, atc_config_ptr, (uint8 *)g_rsp_str);
}

/*****************************************************************************/
//  Description : Decode the refresh command
//  Global resource dependence : s_stk_signal
//  Author: yongxia zhang
//  Note:
/*****************************************************************************/
LOCAL void ATC_DecodeRefreshFileList(
    APP_MN_SIMAT_REFRESH_IND_T *param_ptr,
    ATC_REFRESH_FILE_T *refresh_ptr
)
{
    uint8 *ptemp = param_ptr->raw_apdu.data_arr;

    SCI_MEMSET(refresh_ptr,0,sizeof(ATC_REFRESH_FILE_T));
    
    refresh_ptr->is_files_exist = param_ptr->com.is_files_exist;
    refresh_ptr->refresh_type = param_ptr->com.refresh_type;
    refresh_ptr->file_list.file_num = param_ptr->com.file_list.file_num;

    //if the refresh type is file change or init and file change,we need decode the file list
    if((param_ptr->com.refresh_type == SIMAT_RF_FILE_CHANGE) || 
    (param_ptr->com.refresh_type == SIMAT_RF_INITIAL_FILE_CHANGE))
    {
        //find 0xd0
        if(*ptemp == 0xD0)
        {
            ptemp++;
            if(*ptemp < 0x80)
            {
                ptemp++;
            }
            else
            {
                ptemp += 2;
            }

            //command details
            ptemp += 5;

            //Device identities
            ptemp += 4;

            //File List
            if((*ptemp != 0x12) && (*ptemp != 0x92))
            {
                SCI_ASSERT(FALSE);
            }
            ptemp++;

            //len
            if(*ptemp < 0x80)
            {
                refresh_ptr->file_list.file_data_len = *ptemp - 1;
                ptemp++;
            }
            else
            {
                ptemp++;
                refresh_ptr->file_list.file_data_len = *ptemp - 1;
                ptemp++;
            }

            //Number of files (n)
            SCI_ASSERT(*ptemp == refresh_ptr->file_list.file_num);
            ptemp++;

            //Files
            SCI_MEMCPY(refresh_ptr->file_list.file_data_arr,ptemp,refresh_ptr->file_list.file_data_len);
        }
        else
        {
            SCI_ASSERT(FALSE);
        }
    }        
}
#endif

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
