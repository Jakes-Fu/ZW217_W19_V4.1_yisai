
#ifdef SFR_SUPPORT_CTCC_SMS

/*
 * This file for CTCC SMS Self Reg,
 * follow < 2020.V1 >
 *
 * IMS sms reg -- OK
 * CDMA sms reg -- Not Support.
 */

#include "std_header.h"
#include "os_api.h"
#include "sci_types.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_nv.h"
#include "mmisms_send.h"
#include "sfr_dm_interface.h"
#include "version.h"
#include "sfr_sms.h"
#include "mmi_common.h"
#include "mmipb_export.h"


//ver :
//  2 - CDMA SMS
//  3 - IMS SMS
#define SMSREG_VER_CDMA     (0x02)
#define SMSREG_VER_IMS      (0x03)

//CMD TYPE : 3 - sms reg
#define SMSREG_CMD_REG      (0x03)

// FILL BIT
#define SMSREG_FILL_BIT     (0x00)

#define SMSREG_XML_B		"<a1>"
#define SMSREG_XML_E		"</a1>"
#define SMSREG_DM_B		"<b1>"
#define SMSREG_DM_E		"</b1>"
#define SMSREG_IMEI_1_B	"<b2>"
#define SMSREG_IMEI_1_E	"</b2>"
#define SMSREG_IMEI_2_B	"<b3>"
#define SMSREG_IMEI_2_E	"</b3>"
#define SMSREG_IMSI_B		"<b4>"
#define SMSREG_IMSI_E		"</b4>"
#define SMSREG_VER_B		"<b5>"
#define SMSREG_VER_E		"</b5>"

#define SFRREG_SMS_CONTENT_MAX  (136)

#define SFR_SMS_BUF_LEN     (SFRREG_SMS_CONTENT_MAX + 8)

#define SMS_START_RETRY_MAX     3
#define SFR_SMS_TIMER   (10 * 60 * 1000)
#define CTCC_SMS_SELFREG_CNF_INFO_LEN   2
#define CTCC_SMS_SELFREG_CNF_INFO_CH0	0x03
#define CTCC_SMS_SELFREG_CNF_INFO_CH1	0x04

LOCAL BOOLEAN s_sfr_sms_done[MMI_DUAL_SYS_MAX]={FALSE,};
LOCAL uint8 s_sfr_sms_starter_retry[MMI_DUAL_SYS_MAX] = {0,};
LOCAL uint8 s_sfr_sms_starter[MMI_DUAL_SYS_MAX] = {0,};
LOCAL BOOLEAN s_sfr_sms_running[MMI_DUAL_SYS_MAX] = {FALSE,};
LOCAL BOOLEAN s_sfr_sms_cnf_ok[MMI_DUAL_SYS_MAX] = {FALSE,};

PUBLIC BOOLEAN MMISFR_IsImsiChanged(MN_DUAL_SYS_E dual_sys)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MN_IMSI_T nv_imsi[MMI_DUAL_SYS_MAX] ={0};
    MN_IMSI_T sim_imsi;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }
    MMINV_READ(MMINV_PHONE_IMSI,nv_imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(nv_imsi,0,sizeof(nv_imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,nv_imsi);
    }

    sim_imsi = MNSIM_GetImsiEx(dual_sys);

    SCI_TRACE_LOW("[Self Reg SMS]: Telecom sim IMSI %d (%x%x%x%x%x%x%x%x)",
		sim_imsi.imsi_len,
		sim_imsi.imsi_val[0], sim_imsi.imsi_val[1], sim_imsi.imsi_val[2], sim_imsi.imsi_val[3],
		sim_imsi.imsi_val[4], sim_imsi.imsi_val[5], sim_imsi.imsi_val[6], sim_imsi.imsi_val[7]);
    SCI_TRACE_LOW("[Self Reg SMS]: Telecom NV IMSI %d (%x%x%x%x%x%x%x%x)",
		nv_imsi[dual_sys].imsi_len,
		nv_imsi[dual_sys].imsi_val[0], nv_imsi[dual_sys].imsi_val[1], nv_imsi[dual_sys].imsi_val[2], nv_imsi[dual_sys].imsi_val[3],
		nv_imsi[dual_sys].imsi_val[4], nv_imsi[dual_sys].imsi_val[5], nv_imsi[dual_sys].imsi_val[6], nv_imsi[dual_sys].imsi_val[7]);

    if (memcmp(&(nv_imsi[dual_sys]), &sim_imsi, sizeof(MN_IMSI_T)) == 0)
    {
        SCI_TRACE_LOW("[Self Reg SMS]: Telecom sim IMSI not changed.");
        return FALSE;
    }
    return TRUE;
}

LOCAL int MMISFR_GetIMEI(MN_DUAL_SYS_E dual_sys, char *buff, int buff_len)
{
    int i;
    char imei_str[18]={0};
    MN_IMEI_T tmp_imei;
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys == MN_DUAL_SYS_MAX)
    {
	sprintf(buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);
	return strlen(buff);
    }
#endif
    if (MNNV_GetIMEIEx(dual_sys, tmp_imei))
    {
        for (i = 0; i < 8; i++)
        {
            imei_str[2 * i] = (tmp_imei[i]) & 0x0F;
            imei_str[2 * i + 1] = tmp_imei[i] >> 4;
        }
    }
    sprintf(buff,
            "%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",
            imei_str[1], imei_str[2], imei_str[3], imei_str[4],
            imei_str[5], imei_str[6], imei_str[7], imei_str[8],
            imei_str[9], imei_str[10], imei_str[11], imei_str[12],
            imei_str[13], imei_str[14], imei_str[15]);

    return strlen(buff);
}

LOCAL int MMISFR_GetIMSI(MN_DUAL_SYS_E dual_sys, char *buff, int buff_len)
{
    MN_IMSI_T tmp_imsi = MNSIM_GetImsiEx(dual_sys);

    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)tmp_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN<<1, buff);

    return strlen(buff);
}

LOCAL int MMISFR_SmsBuildData(MN_DUAL_SYS_E dual_sys, uint8 * msg_ptr)
{
    char buff[128];
    int len;
    MN_DUAL_SYS_E dual_sim;
    int temp_len=0;
    char prochead[4]= {0};
    uint8 *message = PNULL;

    message = SCI_ALLOC_APPZ(SFR_SMS_BUF_LEN);
    if(message == NULL) return 0;
    SCI_MEMSET(message, 0, SFR_SMS_BUF_LEN);

    strcpy(message, SMSREG_XML_B);

    //Device mod
    strcat(message, SMSREG_DM_B);
    strcat(message, sfr_dm_get_model());
#if 0    //CDMA
    strcat(message, SMSREG_DM_E);
#endif

    //IMEI1
    strcat(message, SMSREG_IMEI_1_B);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMEI(dual_sys, buff, len);
    strcat(message, buff);
#if 0    //CDMA
    strcat(message, SMSREG_IMEI_1_E);
#endif

    //IMEI2
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    dual_sim = (dual_sys == MN_DUAL_SYS_1) ? MN_DUAL_SYS_2 : MN_DUAL_SYS_1;
#else
    dual_sim = MN_DUAL_SYS_MAX;
#endif
    strcat(message, SMSREG_IMEI_2_B);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMEI(dual_sim, buff, len);
    strcat(message, buff);
#if 0    //CDMA
    strcat(message, SMSREG_IMEI_2_E);
#endif

    //IMSI
    strcat(message, SMSREG_IMSI_B);
    SCI_MEMSET(buff, 0, 128);
    MMISFR_GetIMSI(dual_sys, buff, len);
    strcat(message, buff+1);//+1 is skip the "9"
#if 0    //CDMA
    strcat(message, SMSREG_IMSI_E);
#endif

    //Software version
    strcat(message, SMSREG_VER_B);
    strcat(message, sfr_dm_get_sw_version());
#if 0    //CDMA
    strcat(message, SMSREG_VER_E);
#endif

    strcat(message, SMSREG_XML_E);

    SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SmsBuildData [%s]", message);

    temp_len = strlen(message);
    prochead[0] = SMSREG_VER_IMS;
    prochead[1] = SMSREG_CMD_REG;
    prochead[2] = temp_len;//content LENGTH
    prochead[3] = SMSREG_FILL_BIT;//FILL BIT

    SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SmsBuildData temp_len = %d", temp_len);

    SCI_MEMCPY(msg_ptr, prochead, 4);
    SCI_MEMCPY(&msg_ptr[4], message, temp_len);

#if 0    //CDMA
    if (prochead[0] == SMSREG_VER_CDMA)
    {
        sprintf(msg_ptr + 4 + strlen(message),"%08x", img_crc32(0, msg_ptr, 4 + strlen(message)));
    }
#endif

    SCI_FREE(message);

    return (temp_len + 4);
}

LOCAL BOOLEAN MMISFR_Send_CallBack(BOOLEAN is_succss, DPARAM parm)
{
    if (is_succss)
    {
        SCI_TraceLow("[Self Reg SMS] MMISFR_Send_CallBack [succss]");
    }
    else
    {
        SCI_TraceLow("[Self Reg SMS] MMISFR_Send_CallBack [Fialure] volte : 0x%x (maybe not open)", MMIAPIPHONE_GetVolteInfo());
    }
}

LOCAL void MMISFR_SendSMS(MN_DUAL_SYS_E dual_sys)
{
    MMISMS_SEND_DATA_T send_data = {0};
    uint8 *server_id = "10659401";
    uint8 *msg_ptr = PNULL;
    int data_len=0;
    BOOLEAN result = FALSE;

    msg_ptr = SCI_ALLOC_APPZ(SFR_SMS_BUF_LEN);
    if(msg_ptr == NULL)
    {
        SCI_TraceLow("[Self Reg SMS] MMISFR_SendSMS malloc ERROR");
        return;
    }

    SCI_MEMSET(msg_ptr, 0, SFR_SMS_BUF_LEN);

    data_len = MMISFR_SmsBuildData(dual_sys, msg_ptr);
    if (data_len <= 0)
    {
        SCI_TraceLow("[Self Reg SMS] MMISFR_SmsBuildData ERROR");
        return;
    }

    send_data.binary_sms_data.is_binary = TRUE;
    send_data.binary_sms_data.binary_length = data_len;
    send_data.binary_sms_data.binary_data = msg_ptr;
    send_data.dst_port = PNULL;
    send_data.src_port = PNULL;
    send_data.dest_addr_ptr = (uint8 *)server_id;
    send_data.dest_addr_len = strlen(server_id);
    send_data.dual_sys = dual_sys;
    send_data.is_need_packet = FALSE;
    send_data.send_callback = MMISFR_Send_CallBack;
    send_data.is_sfr_sms = TRUE;

    SCI_TraceBuf("[Self Reg SMS] MMISFR_SendSMS msg data: ", send_data.binary_sms_data.binary_data,send_data.binary_sms_data.binary_length);

    result = MMIAPISMS_SendSmsForOther(&send_data);

    SCI_TRACE_LOW("[Self Reg SMS] MMIAPISMS_SendSmsForOther RESULT = %d", result);
    SCI_FREE(msg_ptr);

    if (result)
    {
        //MMISFR_SMSSetCnfOk(dual_sys, TRUE);
    }
}


PUBLIC void MMISFR_NVSetIMSI(uint8 dual_sys)
{
    MN_IMSI_T imsi[MMI_DUAL_SYS_MAX] = {0};
    MN_RETURN_RESULT_E  return_value = MN_RETURN_FAILURE;
    MN_IMSI_T sim_imsi;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }
    MMINV_READ(MMINV_PHONE_IMSI,imsi,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(imsi,0,sizeof(imsi));
        MMINV_WRITE(MMINV_PHONE_IMSI,imsi);
    }
    sim_imsi = MNSIM_GetImsiEx(dual_sys);
    SCI_MEMCPY(&(imsi[dual_sys]), &sim_imsi,sizeof(MN_IMSI_T));

    MMINV_WRITE(MMINV_PHONE_IMSI, imsi);
}

LOCAL void MMISFR_SelfRegisterBySMS(uint8 timer_id, uint32 param)
{
    MN_DUAL_SYS_E dual_sys = (MN_DUAL_SYS_E)param;

    SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SelfRegisterBySMS dual_sys =%d----running=%d --CNF ok =%d", dual_sys, s_sfr_sms_running[dual_sys],s_sfr_sms_cnf_ok[dual_sys]);

    if( !s_sfr_sms_running[dual_sys])
    {
        SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SelfRegisterBySMS Timer can NOT found");
        MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
        return;
    }
    if(s_sfr_sms_cnf_ok[dual_sys])
    {
        //write NV and stop timer
        SCI_TRACE_LOW("[Self Reg SMS]MMISFR_SelfRegisterBySMS -------- OK -- finish.");
        MMISFR_NVSetIMSI(dual_sys);
        MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
        s_sfr_sms_done[dual_sys] = TRUE;
        s_sfr_sms_running[dual_sys] = FALSE;
    }
    else
    {
        MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
        //Resend
        SCI_TRACE_LOW("[Self Reg SMS]MMISFR_SelfRegisterBySMS resend retry = %d", s_sfr_sms_starter_retry[dual_sys]);
        if(s_sfr_sms_starter_retry[dual_sys] ++ < SMS_START_RETRY_MAX)
        {
            MMISFR_SendSMS(dual_sys);
            s_sfr_sms_starter[dual_sys] = MMK_CreateTimerCallback(SFR_SMS_TIMER, MMISFR_SelfRegisterBySMS, (uint32)dual_sys, FALSE);
        }
        else
        {
            s_sfr_sms_done[dual_sys] = TRUE;
        }
    }
}

PUBLIC BOOLEAN MMISFR_SMSGetRunningStatus(MN_DUAL_SYS_E dual_sys)
{
    return s_sfr_sms_running[dual_sys];
}

PUBLIC BOOLEAN MMISFR_SmsIsSelfRegCnfInfo(DPARAM param)
{
    BOOLEAN is_self_reg_sms = FALSE;
    APP_MN_SMS_IND_T *sig_ptr = (APP_MN_SMS_IND_T *)param;
    APP_SMS_USER_DATA_T sms_user_data_t = {0};
    MMIPB_BCD_NUMBER_T orig_num = {0};
    uint8 tele_string[128] ={0};
    uint8 tele_string_len = 0;

    orig_num.number_len = sig_ptr->sms_t.origin_addr_t.num_len;
    orig_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(sig_ptr->sms_t.origin_addr_t.number_type, MN_NUM_PLAN_UNKNOW );
    SCI_MEMCPY(&orig_num.number,&sig_ptr->sms_t.origin_addr_t.party_num,orig_num.number_len);
    tele_string_len = MMIAPIPB_BCDNumberToString(&orig_num, 128, tele_string);

    SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SmsIsSelfRegCnfInfo NUM = %s", tele_string);

    if(0 == strncmp((char*)tele_string, "10659401", tele_string_len))
    {
        MMISMS_DecodeUserHeadAndContent(sig_ptr->dual_sys, sig_ptr->sms_t.user_head_is_exist,
                &(sig_ptr->sms_t.dcs), &(sig_ptr->sms_t.user_data_t), &sms_user_data_t);

        SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SmsIsSelfRegCnfInfo LEN = %s", sms_user_data_t.user_valid_data_t.length);

        if(sms_user_data_t.user_valid_data_t.length == CTCC_SMS_SELFREG_CNF_INFO_LEN
                && sms_user_data_t.user_valid_data_t.user_valid_data_arr[0] == CTCC_SMS_SELFREG_CNF_INFO_CH0
                && sms_user_data_t.user_valid_data_t.user_valid_data_arr[1] == CTCC_SMS_SELFREG_CNF_INFO_CH1)
        {
            SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SmsIsSelfRegCnfInfo Content Confirm OK");
            MMISFR_SMSSetCnfOk(sig_ptr->dual_sys, TRUE);
        }
        is_self_reg_sms = TRUE;
    }
    return is_self_reg_sms;
}

PUBLIC void MMISFR_SMSSetCnfOk(MN_DUAL_SYS_E dual_sys, BOOLEAN isSuccess)
{
    s_sfr_sms_cnf_ok[dual_sys] = isSuccess;

    if(isSuccess)
    {
        //write NV and stop timer
        SCI_TRACE_LOW("[Self Reg SMS] MMISFR_SelfRegisterBySMS CNF OK");
        MMISFR_NVSetIMSI(dual_sys);
        MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
        s_sfr_sms_done[dual_sys] = TRUE;
        s_sfr_sms_running[dual_sys] = FALSE;
    }
}

PUBLIC void MMISFR_SMS_Run(MN_DUAL_SYS_E dual_sys)
{
    if(s_sfr_sms_done[dual_sys])
    {
        SCI_TraceLow("[Self Reg SMS] Already done.");
        return;
    }
    if(s_sfr_sms_running[dual_sys])
    {
        SCI_TraceLow("[Self Reg SMS] Running.");
        return;
    }

    MMK_StopTimer(s_sfr_sms_starter[dual_sys]);
    s_sfr_sms_running[dual_sys] = TRUE;
    SCI_TRACE_LOW("[Self Reg SMS] Self Register by SMS");

    s_sfr_sms_starter_retry[dual_sys] = 0;
    s_sfr_sms_starter[dual_sys] = MMK_CreateTimerCallback(30*1000, MMISFR_SelfRegisterBySMS, (uint32)dual_sys, FALSE);
}

#endif // SFR_SUPPORT_CTCC_SMS