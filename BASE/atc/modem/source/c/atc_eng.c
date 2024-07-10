/******************************************************************************
 ** File Name:      atc_eng.c                                                  *
 ** Author:         Lisa.Lin                                                  *
 ** DATE:           24/04/2008                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains the functions that handle the AT command
 **                 for engineering mode
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 24/04/2008     Lisa.Lin         Create.                                   *
 ******************************************************************************/
#include "atc.h"
#include "atc_common.h"
#include "atc_eng.h"
#include "atc_gsm.h"
#include "mn_type.h"
#include "atc_basic_cmd.h"
#ifndef WIN32
#include "upm_config.h"
#include "ref_engineering.h"
#include "upm_api.h"
#include "gpio_drv.h"
#endif
#include "os_param.h"
#include "ref_param.h"
#include "ps_eng.h"

#include "tasks_id.h"
#include "keypad.h"
#include "chip.h"

#include "aud_proc_exp.h"

#include "ref_outport.h"
#include "audio_api.h"
#include "dal_power.h"
#include "layer1_engineering.h"
#include "production_test.h"
#include "mn_api.h"
//#include "bt_abs.h"


#define MAX_UPLMN_LEN  6
#define MIN_UPLMN_LEN  4
#ifndef COM_USER
#define COM_USER          0
#endif
#ifndef COM_DATA
#define COM_DATA          0
#endif
#ifndef COM_DEBUG
#define COM_DEBUG         1
#endif

#define COM_DSP_DEBUG  2
#define ATC_IQ_DATA_BUF_SIZE      (150<<8)

#define MN_MAX_DYNAENG_STR_LENGTH 255 /*max length of name of item*/
#define MN_MAX_DYNAENG_ARR_SIZE 50 /*max array size of one item*/
typedef struct ITEM_T
{
    uint8   item_name[MN_MAX_DYNAENG_STR_LENGTH];
    uint32 arr_count;
    uint32 arr_value[MN_MAX_DYNAENG_ARR_SIZE];
    uint32 min_value;
    uint32 max_value;
} ITEM_T;
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
static uint8           s_dummy_kpd_rsp[DUMMYMMI_RSP_MAX][MAX_DUMMY_KPD_RSP_LEN] = {0};
static ITEM_T          s_item = {0};

extern MN_DUAL_SYS_E       g_current_card_id;
extern RSP_RESULT_STR          g_rsp_str;
extern const ATC_INFO_T     g_atc_info_table[];
extern const AT_BAUTRATE_CONTEXT_T c_baudrate_data[];
extern BOOLEAN               g_is_ps_act[];
extern BOOLEAN               g_is_ps_deactivate[];

static BOOLEAN atc_armlog_enable = TRUE;
ATC_POWER_INF_T    g_atc_power_info = {0};
ATC_UPLMN_INF_T    g_atc_uplmn_info[MN_SYS_NUMBER] = {0};
static uint8       s_atc_iq_log_mode = 0;
    
#ifndef WIN32
extern void UCOM_init(void);
extern BOOLEAN RM_GetCalibrationPostMode(void);
#ifdef _ATC_WAIT_L4
extern PUBLIC void USB_Reset (void);
#endif
#endif

#if defined(CHIP_VER_6810) || defined(CHIP_VER_6820)
//defined in deepsleep_drvapi.h
PUBLIC void SCI_UPM_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_Calibration_EnableDeepSleep (uint32 mode);
PUBLIC void SCI_L1_EnableDeepSleep (uint32 mode);
#endif /*CHIP_VER_6810*/
#ifdef _ATC_WAIT_L4
extern PUBLIC void USB_AT_Isr(BOOLEAN is_plugin);
#endif

#ifndef _MUX_ENABLE_
BOOLEAN g_is_diag_cmd = FALSE;
uint8  *g_diag_cmd_buffer_ptr = PNULL;
#endif
static const char    *s_bt_mode_str[ATC_MAX_BT_MODE_NUM * 2] =
{
    "EUT", "CH", "TX", "TXPWR", "RX","RXPACKCOUNT", "eut", "ch", "tx", "txpwr", "rx", "rxpackcount"
};

/*****************************************************************************/
//  Description : This function is to convert uint8 array to string
//  Global resource dependence : None
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
void ATC_IntArray_To_Str(uint8 *a, uint8 len, char *str);

/*****************************************************************************/
//  Description : convert the second parm of the AT+SPDUMMYKPD command  to key code value
//
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
LOCAL int GetVKeyCode(ATC_CUR_PARAM_INFO_LIST_T *param_ptr);

/*****************************************************************************/
//  Description : This function used to sprintf calibration info for display 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCalibrateInfo(uint8* info_buf, uint16 buf_len);

/*****************************************************************************/
//  Description : This function handles the AT+SPPSRATE command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPPSRATE)
{
#ifdef _SUPPORT_GPRS_
#ifdef _GPRS_FULL_FUNCTION_
    MNGPRS_3G_EXTEND_QOS_SET_T ext_qos = {0};
    BOOLEAN item_setted[13] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    MN_PS_SERVICE_TYPE_E service_type;
    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
#endif
    MNGPRS_3G_EXTEND_QOS_SET_T qos_set = {0};
    uint8   *ptr = PNULL;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
    #ifdef _GPRS_FULL_FUNCTION_
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
            {
                service_type = (MN_PS_SERVICE_TYPE_E)PARA1.num;
                ext_qos.max_bt_ul = (uint16)PARA2.num;
                ext_qos.guaranteed_bt_ul = 0; //(uint16)PARA2.num;
                ext_qos.max_bt_dl = (uint16)PARA3.num;
                ext_qos.guaranteed_bt_dl = 0; //(uint16)PARA3.num;

                ATC_TRACE_LOW("ATC: service_type = %d, ul_rate = %d,dl_rate", service_type, ext_qos.max_bt_ul, ext_qos.max_bt_dl);
                ret = MNGPRS_SetStorageExtReqQosWithItemEx(dual_sys, MN_GPRS_STORAGE_NV, service_type, &ext_qos, item_setted);

                if(ret != ERR_MNGPRS_NO_ERR)
                {
                    return ERR_OPERATION_NOT_SUPPORTED;
                }
            }
            else
            {
                return ERR_INVALID_INDEX;
            }

            break;
    #endif
        case ATC_CMD_TYPE_READ:

            ptr = g_rsp_str;
            //qos_set=MNGPRS_GetExtendQos(BROWSER_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, BROWSER_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, BROWSER_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //qos_set=MNGPRS_GetExtendQos(MMS_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, MMS_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, MMS_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //qos_set=MNGPRS_GetExtendQos(DOWNLOAD_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, DOWNLOAD_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, DOWNLOAD_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //qos_set=MNGPRS_GetExtendQos(STREAMING_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, STREAMING_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, STREAMING_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //qos_set=MNGPRS_GetExtendQos(IM_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, IM_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, IM_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);
            ptr = g_rsp_str + strlen((char *)g_rsp_str);

            //qos_set=MNGPRS_GetExtendQos(POC_E);
            MNGPRS_ReadStorageExtReqQosEx(dual_sys, MN_GPRS_STORAGE_NV, POC_E, &qos_set);
            sprintf((char *)ptr, "%s:%d,%d,%d\r\n", g_atc_info_table[AT_CMD_SPPSRATE].cmd_name->str_ptr, POC_E,
                    qos_set.max_bt_ul, qos_set.max_bt_dl);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;

#endif

}

/*****************************************************************************/
//  Description : This function handles the AT^SYSINFO command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SYSINFO)
{
    ATC_TRACE("ATENG:ATC_Processxor_SYSINFO\n");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            ATC_UpSYSINFOInfo(ATC_CURRENT_SIM_ID_MUX, atc_config_ptr, FALSE);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPCOMDEBUG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCOMDEBUG)
{
    uint32        baud_rate;
    uint32        status = ERR_OPERATION_NOT_ALLOWED;
    int32         i;

#ifndef _ULTRA_LOW_CODE_

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                baud_rate = PARA1.num;

                for(i = 0; i < AT_IPR_MAX; i++)
                {
                    if(c_baudrate_data[i].atc_val == baud_rate)
                    {
                        baud_rate = c_baudrate_data[i].reg_val;

                        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
                        SCI_SLEEP(100);

                        SIO_SetBaudRate(COM_DEBUG, baud_rate);
                        status = S_ATC_DEFAULT_HANDLE;
                        break;
                    }
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:
            baud_rate = SIO_GetBaudRate(COM_DEBUG);

            for(i = 0; i < AT_IPR_MAX; i++)
            {
                if(baud_rate == c_baudrate_data[i].reg_val)
                {
                    baud_rate = c_baudrate_data[i].atc_val;
                    sprintf((char *)g_rsp_str, "%s: %ld", g_atc_info_table[AT_CMD_SPCOMDEBUG].cmd_name->str_ptr, baud_rate);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    status = S_ATC_SUCCESS;
                    break;
                }
            }

            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return status;
}

/*****************************************************************************/
//  Description : This function handles the ATC^SPN command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SPN)
{
    uint32                      status  = S_ATC_SUCCESS;
    int32                       i       = 0;
    ATC_SPN_NAME_CODE_SCHEM_E    coding  = ATC_SPN_NAME_7BIT_ALPHA;
    MNSIM_SPN_T                 spn;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                if(MN_RETURN_SUCCESS == MNSIM_GetSPNEx(ATC_CURRENT_SIM_ID_MUX, &spn))
                {
                    for(i = 0; i < MNSIM_SPN_ID_NUM_LEN; i++)
                    {
                        if(spn.id_num[i] == 0xff)
                        {
                            spn.id_num[i] = '\0';
                            break;
                        }
                        else if(spn.id_num[i] >= 0x80)
                        {
                            coding = ATC_SPN_NAME_UCS2;
                        }
                    }

                    sprintf((char *)g_rsp_str, "^SPN:%d,%d,%s", spn.display_condition, coding, spn.id_num);
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                }
                else
                {
                    status = ERR_NOT_FOUND;
                }
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "^SPN:(0-1)");
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        case ATC_CMD_TYPE_READ:
        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the ATC+ARMLOG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessARMLOG)
{
    uint32   status = S_ATC_SUCCESS;

#ifndef WIN32

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                atc_armlog_enable = PARA1.num;
                SCI_SetArmLogFlag(PARA1.num);
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        case ATC_CMD_TYPE_READ:

            if(REFPARAM_GetEnableArmLogFlag() != atc_armlog_enable)
            {
                atc_armlog_enable = REFPARAM_GetEnableArmLogFlag();
            }

            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_ARMLOG].cmd_name->str_ptr, atc_armlog_enable);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

#endif
    return status;
}

/*****************************************************************************/
//  Description : This function handles the ATC+UDISK command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessUDISK)
{
    uint32   status = S_ATC_SUCCESS;

#ifndef WIN32

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                UPM_SetService((UPM_SERVICETYPE_E)PARA1.num);
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }

            break;

        default:
            return ERR_OPERATION_NOT_ALLOWED;
    }

#endif
    return status;
}

/*****************************************************************************/
//  Description : This function is to convert uint8 array to string
//  Global resource dependence : None
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
void ATC_IntArray_To_Str(uint8 *a, uint8 len, char *str)
{
    uint32 i = 0;

    SCI_ASSERT(a != PNULL);/*assert verified: check null pointer*/
    SCI_ASSERT(str != PNULL);/*assert verified: check null pointer*/

    for(i = 0; i < len; i++)
    {
        char temp[4] = "\0";
        sprintf(temp, "%d", a[i]);
        strcat(str, temp);
    }
}

/*****************************************************************************/
//  Description : This function handles the AT+ECMMB command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMB)
{
    AT_Eccmb_Oper_E oper = ECMMB_OPER_ENTER_UNKNOWN;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessECMMB, NO param1 !");
                return ERR_INVALID_INDEX;
            }

            oper = (AT_Eccmb_Oper_E)PARA1.num;
            ATC_TRACE_LOW("ATC: ATC_ProcessECMMB, set command, oper=%d", oper);

            /* TODO: Execute CMMB Operation */
            /* ... */
            break;

        case ATC_CMD_TYPE_EXECUTE:
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+ECMMBSTS command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMBSTS)
{
    AT_Ecmmb_Sts_E sts = ECMMB_STS_ENTER_UNKNOWN;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            /* TODO: Read CMMB status */
            /* sts = (AT_Ecmmb_Sts_E)... */

            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_ECMMBSTS].cmd_name->str_ptr, sts);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_SET:
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+ECMMBEER command
//  Global resource dependence : g_rsp_str
//  Author:       Pizer.Fan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessECMMBEER)
{
    int cmmb_last_error = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
            /* TODO: Get CMMB last error */
            /* cmmb_last_error = ... */

            sprintf((char *)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_ECMMBEER].cmd_name->str_ptr, cmmb_last_error);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_SET:
        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+USBCT command
//  Global resource dependence :
//  Author:       xia.wei
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processstar_USBCT)
{
#ifndef WIN32

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
            #ifdef _ATC_WAIT_L4
                USB_Reset();
            #endif
                UCOM_init();
            }

            break;

        case ATC_CMD_TYPE_TEST:
            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : This function initializes the static variables for ENG module
//  Global resource dependence :
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
void ATC_InitEngVar(void)
{
    uint8            gsm_cell_info[ATC_MAX_GSM_CELL_INFO_LEN + 1] = {0};
    uint8           *gci_ptr = PNULL;

    atc_armlog_enable = REFPARAM_GetEnableArmLogFlag();
    gci_ptr = gsm_cell_info;
}

/*****************************************************************************/
//  Description : convert the second parm of the AT+SPDUMMYKPD command  to key code value
//
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
LOCAL int GetVKeyCode(ATC_CUR_PARAM_INFO_LIST_T *param_ptr)
{
    int key_code = 0xFFFF;

    if(PNULL == param_ptr)
    {
        return key_code;
    }

    if(ATC_CMD_PARAM_TYPE_NUMERIC == param_ptr->param_type)
    {
        if(param_ptr->param_info.num >= 0 && param_ptr->param_info.num <= 9)
        {
            /*SCI_VK_0 ... SCI_VK_9*/
            key_code = param_ptr->param_info.num + '0';
        }
    }
    else if(ATC_CMD_PARAM_TYPE_STRING == param_ptr->param_type)
    {
        SCI_ASSERT(PNULL != param_ptr->param_info.str_ptr);/*assert verified: check null pointer*/

        if(1 == param_ptr->param_info.str_ptr->str_len)
        {
            if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "0") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "1") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "2") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "3") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "4") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "5") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "6") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "7") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "8") ||
                    0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "9"))
            {
                uint16 value;

                /*SCI_VK_0 .... SCI_VK_9*/
                if(ConvertStringtoUint16(param_ptr->param_info.str_ptr->str_ptr, 1, &value))
                {
                    key_code = value + '0';
                }
            }
            else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "*"))
            {
                /*SCI_VK_STAR*/
                key_code = '*';
            }
            else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "#"))
            {
                /*SCI_VK_POUND*/
                key_code = '#';
            }
            else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "@"))
            {
                /*SCI_VK_AT*/
                key_code = '@';
            }
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "call") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "CALL"))
        {
            /*SCI_VK_CALL*/
            key_code = 0x01;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "power") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "POWER"))
        {
            /*SCI_VK_POWER*/
            key_code = 0x02;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "cancel") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "CANCEL"))
        {
            /*SCI_VK_CANCEL*/
            key_code = 0x03;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "up") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "UP"))
        {
            /*SCI_VK_UP*/
            key_code = 0x04;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "down") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "DOWN"))
        {
            /*SCI_VK_DOWN*/
            key_code = 0x05;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "left") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "LEFT"))
        {
            /*SCI_VK_LEFT*/
            key_code = 0x06;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "right") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "RIGHT"))
        {
            /*SCI_VK_RIGHT*/
            key_code = 0x07;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "mselect") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "MSELECT"))
        {
            /*SCI_VK_MENU_SELECT*/
            key_code = 0x08;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "mcancel") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "MCANCEL"))
        {
            /*SCI_VK_MENU_CANCEL*/
            key_code = 0x09;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "sup") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "SUP"))
        {
            /*SCI_VK_SIDE_UP*/
            key_code = 0x0A;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "sdown") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "SDOWN"))
        {
            /*SCI_VK_SIDE_DOWN*/
            key_code = 0x0B;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "ok") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "OK"))
        {
            /*SCI_VK_WEB*/
            /*confirm key for mocor*/
            key_code = 0x0D;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "web") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "WEB"))
        {
            /*SCI_VK_WEB*/
            // key_code = 0x0D;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "camera") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "CAMERA"))
        {
            /*SCI_VK_CAMERA*/
            key_code = 0x0E;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "flip") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "FLIP"))
        {
            /*SCI_VK_FLIP*/
            key_code = SCI_VK_FLIP;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "headsetbuttom") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "HEADSETBUTTOM"))
        {
            /*SCI_VK_HEADSET_BUTTOM*/
            key_code = SCI_VK_HEADSET_BUTTOM;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "headsetdetect") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "HEADSETDETECT"))
        {
            /*SCI_VK_HEADSET_DETECT*/
            key_code = SCI_VK_HEADSET_DETECT;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "sdcarddetect") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "SDCARDDETECT"))
        {
            /*SCI_VK_SDCARD_DETECT*/
            key_code = SCI_VK_SDCARD_DETECT;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "pause") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "PAUSE"))
        {
            /*SCI_VK_PAUSE*/
            key_code = SCI_VK_PAUSE;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "rewind") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "REWIND"))
        {
            /*SCI_VK_REWIND*/
            key_code = SCI_VK_REWIND;
        }
        else if(0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "fastforword") ||
                0 == strcmp((char *)param_ptr->param_info.str_ptr->str_ptr, "FASTFORWORD"))
        {
            /*SCI_VK_FASTFORWARD*/
            key_code = SCI_VK_FASTFORWARD;
        }
    }

    return key_code;
}
/*****************************************************************************/
//  Description : This function is a callback function, it receives and
//                      handles the message which is sent by MMI.
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
int32  ATC_ProcessAutoTestInfoFromMMI(
    const uint8   *str_ptr,     //the string type refer to AT+SPDUMMYKPDRSP
    uint32        str_len
)
{
    if(NULL == str_ptr)
    {
        return 1;
    }

    if(str_len > 0)
    {
        uint8   index = DUMMYMMI_RSP_MAX;

        //ATC_TRACE_LOW("ATC: ATC_ProcessDummyKPDRsp,data:%s",str_ptr);
        str_len = MIN((MAX_DUMMY_KPD_RSP_LEN - 1), str_len);

        if(0 == strncmp((char *)str_ptr, "MESSAGE", strlen("MESSAGE")))
        {
            /*the last message box information of MMI*/
            index = DUMMYMMI_RSP_MESSAGE;
        }
        else if(0 == strncmp((char *)str_ptr, "OPENWIN", strlen("OPENWIN")) ||
                0 == strncmp((char *)str_ptr, "CLOSEWIN", strlen("CLOSEWIN")))
        {
            /*the last win state information of MMI*/
            index = DUMMYMMI_RSP_WIN;
        }
        else if(0 == strncmp((char *)str_ptr, "BROWSERTITLE", strlen("BROWSERTITLE")))
        {
            /*the last browser title information of MMI*/
            index = DUMMYMMI_RSP_BROWSERTITLE;
        }
        else if(0 == strncmp((char *)str_ptr, "BROWSERCURSOR", strlen("BROWSERCURSOR")))
        {
            /*the last browser cursor state information of MMI*/
            index = DUMMYMMI_RSP_BROWSERCURSOR;
        }
        else if(0 == strncmp((char *)str_ptr, "SOFTKEY", strlen("SOFTKEY")))
        {
            /*the last window softkey information of MMI*/
            index = DUMMYMMI_RSP_SOFTKEY;
        }
        else if(0 == strncmp((char *)str_ptr, "IMAGEID", strlen("IMAGEID")))
        {
            /*the last image id information of MMI*/
            index = DUMMYMMI_RSP_IMAGEID;
        }
        else if(0 == strncmp((char *)str_ptr, "FOCUSWIN", strlen("FOCUSWIN")))
        {
            /*current focus window information of MMI*/
            index = DUMMYMMI_RSP_FOCUSWIN;
        }
        else if(0 == strncmp((char *)str_ptr, "WINSTRING", strlen("WINSTRING")))
        {
            /*current window context information of MMI*/
            index = DUMMYMMI_RSP_WINSTRING;
        }

        else if(0 == strncmp((char *)str_ptr, "BACKLIGHTS", strlen("BACKLIGHTS")))
        {
            /*current backlight state of MMI*/
            index = DUMMYMMI_RSP_BACKLIGHT;
        }

        if(DUMMYMMI_RSP_MAX != index)
        {
            SCI_MEMSET((void *)s_dummy_kpd_rsp[index], 0x0, MAX_DUMMY_KPD_RSP_LEN);
            SCI_MEMCPY(s_dummy_kpd_rsp[index], str_ptr, str_len);
        }
    }
    else
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessAutoTestInfoFromMMI,ERROR,wrong string length:%d", str_len);
    }

    return 0;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPD command
//                dummy keypad operation
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPD)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    xSignalHeaderRec        *key_sig_ptr    = PNULL;
    uint8               softkey_cmd;
    int                  key_code = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if((!PARAM1_FLAG) || (!PARAM2_FLAG))
            {
                status = ERR_INVALID_INDEX;
            }
            else
            {
                softkey_cmd = (uint8)PARA1.num;
                key_code = (int)GetVKeyCode(cur_cmd_info->cur_param_list_ptr->next);//PARA2.num;

                if(0xFFFF == key_code)
                {
                    return  ERR_INVALID_INDEX;
                }

                SCI_MEMSET((void *)s_dummy_kpd_rsp[DUMMYMMI_RSP_MESSAGE], 0x0, MAX_DUMMY_KPD_RSP_LEN);
                SCI_MEMSET((void *)s_dummy_kpd_rsp[DUMMYMMI_RSP_WINSTRING], 0x0, MAX_DUMMY_KPD_RSP_LEN);

                switch(softkey_cmd)
                {
                    case DIAG_KEY_MSG_DOWN_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                        ATC_TRACE_LOW("ATC: SIMULATOR KPD: generate down key, key code is 0x%x", key_code);
                        break;
                    case DIAG_KEY_MSG_UP_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                        ATC_TRACE_LOW("ATC: SIMULATOR KPD: generate up key, key code is 0x%x", key_code);
                        break;
                    case DIAG_NORMAL_KEY_MSG_INFO_F:
                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_DOWN, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);

                        SCI_CREATE_SIGNAL(key_sig_ptr, KPD_UP, sizeof(KPDSVR_SIG_T), SCI_IdentifyThread());
                        SCI_ASSERT(PNULL != key_sig_ptr);/*assert verified: check null pointer*/

                        ((KPDSVR_SIG_T *)key_sig_ptr)->key = key_code;
                        SCI_SEND_SIGNAL(key_sig_ptr, KPDSVR);
                        ATC_TRACE_LOW("ATC: SIMULATOR KPD: generate down and up key, key code is 0x%x", key_code);
                        break;
                    default:
                        status = ERR_OPERATION_NOT_SUPPORTED;
                        break;
                }
            }

            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}
/*****************************************************************************/
//  Description : This function handles the AT+SPDUMMYKPDRSP command
//                get dummy keypad operation response
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDUMMYKPDRSP)
{
    uint8       result_type;

    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSPDUMMYKPDRSP Command Type not match");
        return S_ATC_FAIL;
    }

    if(PARAM1_FLAG)
    {
        result_type = (uint8)PARA1.num;
    }
    else
    {
        result_type = 0;
    }

    if (result_type >= DUMMYMMI_RSP_MAX)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSPDUMMYKPDRSP result_type %d exceeds %d(DUMMYMMI_RSP_MAX)", 
                    result_type, DUMMYMMI_RSP_MAX);
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if(0 != strlen((char *)s_dummy_kpd_rsp[result_type]))
    {
        sprintf((char *)g_rsp_str, "%s", s_dummy_kpd_rsp[result_type]);
    }
    else
    {
        sprintf((char *)g_rsp_str, "%s", "EMPTY");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPCEER command
//  Global resource dependence : none
//  Author:       minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCEER)
{
    ATC_STATUS     status = S_ATC_SUCCESS;

    ATC_TRACE_LOW("ATC: ATC_ProcessSPCEER.");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                return ERR_INVALID_INDEX;
            }

            if(PARA1.num > CEER_WAP_ATTACH)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else if(CEER_WAP_ATTACH == PARA1.num)
            {
                /*report string error cause*/
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else
            {
                /*the others, report numeric error cause*/
                sprintf((char *)g_rsp_str, "%s: %d,%lu",
                        g_atc_info_table[AT_CMD_SPCEER].cmd_name->str_ptr,
                        (uint8)PARA1.num,
                        atc_config_ptr->ceer_code[PARA1.num]);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }

            break;

        case ATC_CMD_TYPE_READ:

            if(atc_config_ptr->ceer_last_type >= CEER_WAP_ATTACH)
            {
                /*not include CEER_WAP_ATTACH*/
                return ERR_OPERATION_NOT_ALLOWED;
            }

            sprintf((char *)g_rsp_str, "%s: %d,%lu",
                    g_atc_info_table[AT_CMD_SPCEER].cmd_name->str_ptr,
                    atc_config_ptr->ceer_last_type,
                    atc_config_ptr->ceer_code[atc_config_ptr->ceer_last_type]);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        case ATC_CMD_TYPE_TEST:
            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPCHIPTYPE command
//                Get chip type
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPCHIPTYPE)
{
    uint32  chiptype;

    if (ATC_CMD_TYPE_EXECUTE != ATC_GET_CMD_TYPE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSPCHIPTYPE Command Type not match");
        return S_ATC_FAIL;
    }

    chiptype = (uint32)CHIP_GetChipType();
    sprintf((char *)g_rsp_str, "+SPCHIPTYPE: %lu", chiptype);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPEQPARA command
//                      Get(Set) eq para from(to) RAM or FALSH
//  Global resource dependence :
//  Author:     minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPEQPARA)
{
#if !defined(CHIP_VER_6810) && !defined(CHIP_VER_6820)
    AUDIO_EQ_STRUCT_T   *eq_para_ptr = NULL;
    uint8       *buffer_ptr = NULL;
    AUDIO_RESULT_E      set_result;

    if(ATC_CMD_TYPE_SET == ATC_GET_CMD_TYPE)
    {
        if(!PARAM1_FLAG)
        {
            ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: <n> is invalid.");
            return ERR_INVALID_INDEX;
        }

        /*PARA1.num is <n>*/
        switch(PARA1.num)
        {
            case 0:   //get the maximum count of eq para type
#ifndef WIN32
                sprintf((char *)g_rsp_str, "%s: %lu",
                        g_atc_info_table[AT_CMD_SPEQPARA].cmd_name->str_ptr,
                        AUDPROC_EXP_GetEqParaSetNum());
#endif
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            case 1:   //get eq para structure info

                if((!PARAM2_FLAG) || (!PARAM3_FLAG) || PARAM4_FLAG)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: <type> or <dest> is invalid.");
                    return ERR_INVALID_INDEX;
                }

                /*PARA2.num is <type>;
                   PARA3.num is <dest>*/
                if(0 == PARA3.num)
                {
#ifndef WIN32
                    /*get eq info from ram*/
                    eq_para_ptr = AUDPROC_EXP_GetEqSetFromRam((AUD_PROC_EQPARA_SET_E)PARA2.num);
#endif
                }
                else
                {
#ifndef WIN32
                    /*get eq info from flash*/
                    eq_para_ptr = AUDPROC_EXP_GetEqSetFromFlash((AUD_PROC_EQPARA_SET_E)PARA2.num);
#endif
                }

                if(NULL == eq_para_ptr)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: Get EQ PARA failed.");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                buffer_ptr = (uint8 *)SCI_ALLOC_BASE((sizeof(AUDIO_EQ_STRUCT_T) + 1) * 2);
                if (!buffer_ptr)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA buffer_ptr pointer is PNULL");
                    return ERR_MEMORY_FULL;
                }
                SCI_MEMSET(buffer_ptr, 0x0, ((sizeof(AUDIO_EQ_STRUCT_T) + 1) * 2));
                /*convert eq structure data to hex string*/
                ConvertBinToHex((uint8 *)eq_para_ptr, sizeof(AUDIO_EQ_STRUCT_T), buffer_ptr);

                sprintf((char *)g_rsp_str, "%s: %s",
                        g_atc_info_table[AT_CMD_SPEQPARA].cmd_name->str_ptr,
                        buffer_ptr);

                SCI_FREE(buffer_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                break;
            case 2:     //set eq para info

                if((!PARAM2_FLAG) || (!PARAM3_FLAG) || (!PARAM4_FLAG))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: <type> or <dest> or <data> is invalid.");
                    return ERR_INVALID_INDEX;
                }

                /*<data> type must be string*/
                if(ATC_CMD_PARAM_TYPE_STRING != PARAM4_TYPE)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: <data> type is not string.");
                    return ERR_INVALID_INDEX;
                }

                /*check <data> string length, <data> must be hex string*/
                if(PARA4.str_ptr->str_len != (sizeof(AUDIO_EQ_STRUCT_T) * 2))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: <data> length:%d != %d*2.", PARA4.str_ptr->str_len, sizeof(AUDIO_EQ_STRUCT_T));
                    return ERR_INVALID_INDEX;
                }

                eq_para_ptr = (AUDIO_EQ_STRUCT_T *)SCI_ALLOC_BASE(sizeof(AUDIO_EQ_STRUCT_T));
                if (!eq_para_ptr)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA eq_para_ptr pointer is PNULL");
                    return ERR_MEMORY_FULL;
                }
                SCI_MEMSET(eq_para_ptr, 0x0, sizeof(AUDIO_EQ_STRUCT_T));

                /*convert hex string to bin*/
                if(!ConvertHexToBin(PARA4.str_ptr->str_ptr, (uint16)PARA4.str_ptr->str_len, (uint8 *)eq_para_ptr))
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: ConvertHexToBin failed.");
                    SCI_FREE(eq_para_ptr);
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                /*PARA2.num is <type>;
                   PARA3.num is <dest>*/
                if(0 == PARA3.num)
                {
#ifndef WIN32
                    /*set eq info to ram*/
                    set_result = AUDPROC_EXP_SetEqSetToRam((AUD_PROC_EQPARA_SET_E)PARA2.num, eq_para_ptr);
#endif
                }
                else
                {
#ifndef WIN32
                    /*set eq info to flash*/
                    set_result = AUDPROC_EXP_SetEqSetToFlash((AUD_PROC_EQPARA_SET_E)PARA2.num, eq_para_ptr);
#endif
                }

                SCI_FREE(eq_para_ptr);

                if(AUDIO_NO_ERROR != set_result)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSPEQPARA: Set EQ PARA failed.");
                    return ERR_OPERATION_NOT_ALLOWED;
                }

                break;

            default:
                return ERR_OPERATION_NOT_SUPPORTED;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    return S_ATC_SUCCESS;
#else
    return ERR_OPERATION_NOT_SUPPORTED;
#endif
}

/*****************************************************************************/
//  Description : This function handle the AT+SPREF command
//                 Automatic downloader
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPREF)
{
#ifdef _ATC_WAIT_
    if (ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        ATC_TRACE_LOW("ATC: ATC_ProcessSPREF Command Type not match");
        return S_ATC_FAIL;
    }

    if(PARAM1_FLAG && PARAM1_TYPE == ATC_CMD_PARAM_TYPE_STRING)
    {
        if(0 != REF_HandleATCommand(PARA1.str_ptr->str_ptr))
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
#endif
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+SCPFRE command.This command is used
//  to set or get frequency value.
//  Global resource dependence : g_rsp_str
//  Author:       minqian.qian
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCPFRE)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    uint8        band;
    uint16        frequency, freq; // 0 - set gpio value, 1 - get gpio value
    BOOLEAN   ismode = FALSE;

    switch(ATC_GET_CMD_TYPE)
    {

        case ATC_CMD_TYPE_SET:

            // check the type
            if(PARAM1_FLAG && PARAM2_FLAG)
            {
                //PARA1.num define the band
                band = (uint8)PARA1.num;
                //PARA2.num define the frequency
                frequency = (uint16)PARA2.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }

            /*in CALIBRATION_POST_MODE, should set the frequency parameter to global variable
               msg_head->seq_num higher 2 bytes carry the frequency information; big endian
               frequency information includes frequency(11bit) and band(4bit);
               bit    0   1   2   3   4   5   6   7    8   9   10  11  12  13  14  15
                       |                                |                                         |
                               seq_num[0]                       seq_num[1]
                       |                                     |   |    |    |    |    |    |   |
                       |                                       frequency(low 8 bit)
                       |   |   |        |   |   |   |
             frequency(high 3 bit)      band(4 bits)
                */
            freq = (frequency & 0xff) | ((frequency & 0x700) << 5) | ((band & 0xf) << 8);
            POWER_SetCalibrationPostModeFreq(freq);
            //ATC_TRACE_LOW("ATC: Set SCPFRE the freq is 0x%x", freq);
            //response OK
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
            sprintf((char *)g_rsp_str, "%s: 0x%x", g_atc_info_table[AT_CMD_SCPFRE].cmd_name->str_ptr, freq);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        case ATC_CMD_TYPE_READ:
#ifndef WIN32
            ismode = RM_GetCalibrationPostMode();

            if(ismode)
            {
                freq = POWER_GetCalibrationPostModeFreq();
                frequency = (freq & 0xff) | ((freq & 0xe000) >> 5);
                band = (freq & 0xf00) >> 8;
                sprintf((char *)g_rsp_str, "%s: %d,%d", g_atc_info_table[AT_CMD_SCPFRE].cmd_name->str_ptr,
                        frequency, band);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
#endif
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }

            break;

        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }

    return  status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SL1MON command 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSL1MON )
{
    LAYER1_MONITOR_ITEM_T monitor_item;
    uint16 str_len = 0;
    
    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        SCI_MEMSET(&monitor_item, 0, sizeof(LAYER1_MONITOR_ITEM_T));
#ifndef WIN32        
        LAYER1_GetLayer1MonItem(&monitor_item);
#endif

        str_len = sprintf((char*)g_rsp_str, "bsic_search_total = %d \r\nbsic_search_succ = %d \r\nbsic_search_fail = %d \r\nbsic_confirm_total = %d \r\nbsic_confirm_succ = %d \r\nbsic_confirm_fail = %d \r\n",
                    monitor_item.bsic_search_total,
                    monitor_item.bsic_search_succ,
                    monitor_item.bsic_search_fail,
                    monitor_item.bsic_confirm_total,
                    monitor_item.bsic_confirm_succ,
                    monitor_item.bsic_confirm_fail
                    );
       
        str_len += sprintf((char*)g_rsp_str + str_len, "scell_bcchdec_continue = %d \r\nscell_bcchdec_single = %d \r\nscell_bcch_total = %d \r\nscell_bad_bcch = %d \r\n",
                    monitor_item.scell_bcchdec_continue,
                    monitor_item.scell_bcchdec_single,
                    monitor_item.scell_bcch_total,
                    monitor_item.scell_bad_bcch
                    );
        
        str_len += sprintf((char*)g_rsp_str + str_len, "ncell_bcchdec_continue = %d \r\nncell_bcchdec_single = %d \r\nncell_bcch_total = %d \r\nncell_bad_bcch = %d \r\n",
                    monitor_item.ncell_bcchdec_continue,
                    monitor_item.ncell_bcchdec_single,
                    monitor_item.ncell_bcch_total,
                    monitor_item.ncell_bad_bcch
                    );
        
        str_len += sprintf((char*)g_rsp_str + str_len, "rxlev_total = %d \r\nrxlev_bad = %d \r\npch_block_total = %d \r\nbad_pch_num = %d \r\ndummy_pch_cnt = %d \r\ndetected_dummy_pch_cnt = %d \r\n",
                    monitor_item.rxlev_total,
                    monitor_item.rxlev_bad,
                    monitor_item.pch_block_total,
                    monitor_item.bad_pch_num,
                    monitor_item.dummy_pch_cnt,
                    monitor_item.detected_dummy_pch_cnt
                    );
        
        str_len += sprintf((char*)g_rsp_str + str_len, "burst_pch_cnt1 = %d \r\nburst_pch_cnt2 = %d \r\nburst_pch_cnt3 = %d \r\nburst_pch_cnt4 = %d \r\n",
                    monitor_item.burst_pch_cnt1,
                    monitor_item.burst_pch_cnt2,
                    monitor_item.burst_pch_cnt3,
                    monitor_item.burst_pch_cnt4
                    );
        
        str_len += sprintf((char*)g_rsp_str + str_len, "rla_sort_total = %d \r\nsend_rach_num = %d \r\nrx_agch_num = %d \r\nsend_prach_num = %d \r\n",
                    monitor_item.rla_sort_total,
                    monitor_item.send_rach_num,
                    monitor_item.rx_agch_num,
                    monitor_item.send_prach_num
                    );
        
        str_len += sprintf((char*)g_rsp_str + str_len, "rx_pagch_num = %d \r\nbcch_camp_num = %d \r\npwrswp_num = %d \r\nhandover_total = %d \r\nhandover_fail = %d \r\n",
                    monitor_item.rx_pagch_num,
                    monitor_item.bcch_camp_num,
                    monitor_item.pwrswp_num,
                    monitor_item.handover_total,
                    monitor_item.handover_fail
                    );
                    
        str_len += sprintf((char*)g_rsp_str + str_len, "gprs_rcvd_block = %d \r\ngprs_discarded_block = %d \r\ngood_pdtch_num = %d \r\nbad_pdtch_num = %d ",
                    monitor_item.gprs_rcvd_block,
                    monitor_item.gprs_discarded_block,
                    0,                                         //monitor_item.good_pdtch_num,  not support
                    0                                          //monitor_item.bad_pdtch_num    not support
                    );
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+SADC command 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSADC )
{
    uint32 battery_calibration[2]={0};
    uint32 program_calibration[2]={0};
    uint32 invalid_calibration = 0;
    uint32 chr_pm_ver_support = 0;
    
    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
#ifndef WIN32    
        REF_GetAdcCalibrationPara(battery_calibration, program_calibration, invalid_calibration, chr_pm_ver_support);
#endif
        
        sprintf((char*)g_rsp_str, "%s: 0x%x,0x%x,0x%x,0x%x",
                g_atc_info_table[AT_CMD_SADC].cmd_name->str_ptr,
                program_calibration[0],
                program_calibration[1],
                battery_calibration[0],
                battery_calibration[1]);
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    }
    else
    {
        return ERR_OPERATION_NOT_SUPPORTED;
    }
    
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+SGSIMG command to Get Sim image
//  Global resource dependence : none
//  Author:   elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSGSIMG)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_SIM_IMG_T     sim_img_info = {0};
    uint8   *img_body_ptr = PNULL;
    uint8   img_id = 0xff;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessSGSIMG");    
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (!PARAM1_FLAG)
        {
            return ERR_INVALID_INDEX;
        }

        if (!atc_config_ptr->sim_flag[dual_sys])
        {
            return ERR_SIM_NOT_INSERTED;
        }

        img_id = PARA1.num;
        
        if (TRUE != MNSIM_GetImgEx(dual_sys, img_id, &sim_img_info))
        {
            return ERR_SIM_WRONG;
        }
        
        img_body_ptr = (uint8 *)SCI_ALLOC_BASEZ(sim_img_info.img_len * 2 + 1);
        if (PNULL == img_body_ptr)
        {
            return ERR_MEMORY_FULL;
        }
        
        ConvertBinToHex(sim_img_info.img_body, sim_img_info.img_len, img_body_ptr);
        img_body_ptr[sim_img_info.img_len * 2] = '\0';
        
        sprintf((char *)g_rsp_str, "%s: %d,%d,%d,%d,%d,%d,%d,%s",
                   g_atc_info_table[AT_CMD_SGSIMG].cmd_name->str_ptr,
                   sim_img_info.img_width,
                   sim_img_info.img_height,
                   sim_img_info.img_cs,
                   sim_img_info.img_len,
                   sim_img_info.bits_per_img_point,
                   sim_img_info.num_of_clut_entry,
                   sim_img_info.location_clut,
                   img_body_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        SCI_FREE(img_body_ptr);
        break;
        
    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }
    
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SDRMOD command 
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDRMOD)
{
    ATC_STATUS  status = S_ATC_SUCCESS;
    uint8       debug_mode = 0;
     
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG)
        {
            if(0 == PARA1.num)
            {
                OSPARAM_SetAssertMode(SCI_ASSERT_DEBUG_MODE);
            }
            else
            {
                OSPARAM_SetAssertMode(SCI_ASSERT_RELEASE_MODE);
            }
        }
        else
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
        }
        break;
    case ATC_CMD_TYPE_READ:
        OSPARAM_GetAssertMode((uint8*)&debug_mode); 
        
        sprintf((char*)g_rsp_str, "%s: %d",
                g_atc_info_table[AT_CMD_SDRMOD].cmd_name->str_ptr,
                (debug_mode == SCI_ASSERT_RELEASE_MODE) ? 1 : 0);
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SGMR command 
//  Global resource dependence : none
//  Author:
//  Note: parameter - dual_sys, read/write, type, string
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSGMR)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
    
#ifndef WIN32
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    SGMR_OPER_E               op = OP_READ;
    MN_IMEI_T                  imei={0};
    NVITEM_ERROR_E          ret = NVERR_NONE;
    ATC_SGMR_TYPE_E      type;
    uint8                     sv_num = 0;
    uint32            nv_imei_id[4] = {0};
    PRODUCTION_TEST_INFO_T  product_test_info;
    MN_IMEI_T                  imei_arr; 
                
    SCI_MEMSET(&imei, 0, sizeof(MN_IMEI_T));
    SCI_MEMSET(&product_test_info, 0x0, sizeof(PRODUCTION_TEST_INFO_T));

    SCI_TRACE_LOW("ATC: ATC_ProcessSGMR");
   
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
            {
                dual_sys = (MN_DUAL_SYS_E)PARA1.num;
                op = (SGMR_OPER_E)PARA2.num;
                type = (ATC_SGMR_TYPE_E)PARA3.num;
            }
            else
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            switch(type)
            {
                case SGMR_SN:
                {
                    if(OP_READ == op)
                    {
                        if(SCI_SUCCESS != SCI_GetProductionTestInfo(&product_test_info))
                        {
                            SCI_TRACE_LOW("ATC: get production test info fail");
                            return ERR_OPERATION_NOT_ALLOWED;
                        }

                        SCI_TRACE_LOW("ATC: read SN = %s", product_test_info.serial_num);
                        
                        sprintf((char*)g_rsp_str, "%s: %s",
                                    g_atc_info_table[AT_CMD_SGMR].cmd_name->str_ptr,
                                    product_test_info.serial_num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else 
                    {
                     TEST_DATA_INFO_T *prod_info_ptr = NULL;

					prod_info_ptr = (TEST_DATA_INFO_T*)SCI_ALLOC_BASE(sizeof(TEST_DATA_INFO_T) + 4 );
			 		SCI_ASSERT(NULL != prod_info_ptr); /*assert verified*/

					SCI_TRACE_LOW("get prodect info");
					NVITEM_GetProductInfo_Protect((uint8 *) prod_info_ptr, sizeof(TEST_DATA_INFO_T) );

					prod_info_ptr->header.Magic = 0x53503039;//SP09_SPPH_MAGIC_NUMBER;
					SCI_MEMCPY((uint8*)&prod_info_ptr->header.SN, PARA4.str_ptr->str_ptr, MAX_SN_LEN);

					NVITEM_UpdateProductInfo_Protect(FLASH_GetProductInfoAddr(), (uint8 *) prod_info_ptr, sizeof(TEST_DATA_INFO_T));
					SCI_FREE(prod_info_ptr);
                    }
                    
                    break;
                }

                case SGMR_IMEI:
                {
                    nv_imei_id[0] = NV_IMEI;
                    nv_imei_id[1] = NV_IMEI1;
                    nv_imei_id[2] = NV_IMEI2;
                    nv_imei_id[3] = NV_IMEI3;

                    if(OP_READ == op) //read
                    {
                        uint8 temp_char[MN_MAX_IMEI_LENGTH*2+1] = {0};

			   MNNV_GetIMEI(dual_sys, imei_arr);        
			   ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_char);

                        sprintf((char*)g_rsp_str, "%s: %s",
                        g_atc_info_table[AT_CMD_SGMR].cmd_name->str_ptr,
                        temp_char);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //write
                    {
                        if(!PARAM4_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM4_TYPE)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(PARA4.str_ptr->str_len > MN_MAX_IMEI_LENGTH * 2)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(!TransfromImeiStrToBcd(PARA4.str_ptr->str_ptr, PARA4.str_ptr->str_len, MN_MAX_IMEI_LENGTH, (uint8*)imei))
                        {
                            SCI_TRACE_LOW("ATC: invalid imei character!");
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        ret = NVITEM_UpdateCali(nv_imei_id[dual_sys], MN_MAX_IMEI_LENGTH, (void*)imei);

                        if(ret != NVERR_NONE)
                        {
                            SCI_TRACE_LOW("ATC: write IMEI %d error code = 0x%x", dual_sys, ret);
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                    }
                    
                    break;
                }

                case SGMR_SV:
                {
                    if(OP_READ == op) //read
                    {
                        NVITEM_ERROR_E  result;

                        result = EFS_NvitemRead(NV_IMEISVN, 
                        sizeof(uint8),
                        (uint8 *)&sv_num); 
                        sprintf((char*)g_rsp_str, "%s: %02d",
                        g_atc_info_table[AT_CMD_SGMR].cmd_name->str_ptr,
                        sv_num);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //write
                    {
                        if(!PARAM4_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM4_TYPE)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(PARA4.str_ptr->str_len > sizeof(uint8) * 2)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(!ConvertHexToBin(PARA4.str_ptr->str_ptr, PARA4.str_ptr->str_len, &sv_num))
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        NVITEM_UpdateCali(NV_IMEISVN, sizeof(uint8), (uint8*)&sv_num);
                    }
                    break;
                }

                case SGMR_CALI:
                {
                    if(OP_READ == op)
                    {
                        GetCalibrateInfo(g_rsp_str, MAX_ATC_RSP_LEN);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //not support write calibration info
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                    
                    break;
                }

                case SGMR_CALI_VAL:
                {
                    if(OP_READ == op)
                    {
                        uint32 reserved_info = 0;
                        reserved_info = REFPARAM_GetCaliMarkerValue();
                        sprintf((char*)g_rsp_str, "%s: 0x%x",
                        g_atc_info_table[AT_CMD_SGMR].cmd_name->str_ptr,
                        reserved_info);

                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //not support write calibration info
                    {
                        status = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                    
                    break;
                }

                case SCMR_CHECK_PHASE:
                {
                    int index  = 0;

                    if(SCI_SUCCESS != SCI_GetProductionTestInfo(&product_test_info))
                    {
                        SCI_TRACE_LOW("ATC: get production test info fail");
                        return ERR_OPERATION_NOT_ALLOWED;
                    }

                    SCI_TRACE_LOW("ATC: product_test_info=%d", product_test_info.total_item_num);

                    sprintf((char*)g_rsp_str, "+SGMR: ");

                    for(index=0; index<product_test_info.total_item_num; index++)
                    {
                        SCI_TRACE_LOW("ATC: test_item_index=%d,name=%s",index,product_test_info.item_info[index].item_name);
                        if(index == 0)
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str), "%s,%s,%d",
                                product_test_info.item_info[index].item_name,
                                product_test_info.item_info[index].test_state,
                                product_test_info.item_info[index].error_code);
                        }
                        else
                        {
                            sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str), ",%s,%s,%d",
                                product_test_info.item_info[index].item_name,
                                product_test_info.item_info[index].test_state,
                                product_test_info.item_info[index].error_code);
                        }
                    }                    
                    
                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    break;
                }

                case SGMR_C_IMEI:
                {
#ifdef _C_IMEI_SUPPORT

                    if(OP_READ == op) //read
                    {
                        uint8 temp_char[MN_MAX_IMEI_LENGTH*2+1] = {0};
                        BOOLEAN res =0;

			   res = MNNV_GetCIMEI(dual_sys, imei_arr);    
                        if(!res)
                        {
                             SCI_TRACE_LOW("ATC: read C-IMEI error ");
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
			   ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_char);

                        sprintf((char*)g_rsp_str, "%s: %s",
                        g_atc_info_table[AT_CMD_SGMR].cmd_name->str_ptr,
                        temp_char);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    }
                    else //write
                    {
                        if(!PARAM4_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM4_TYPE)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(PARA4.str_ptr->str_len > MN_MAX_IMEI_LENGTH * 2)
                        {
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        if(!TransfromImeiStrToBcd(PARA4.str_ptr->str_ptr, PARA4.str_ptr->str_len, MN_MAX_IMEI_LENGTH, (uint8*)imei))
                        {
                            SCI_TRACE_LOW("ATC: invalid C-imei character!");
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }

                        ret = NVITEM_UpdateCali(NV_C_IMEI, MN_MAX_IMEI_LENGTH, (void*)imei);

                        if(ret != NVERR_NONE)
                        {
                            SCI_TRACE_LOW("ATC: write C-IMEI %d error code = 0x%x", dual_sys, ret);
                            return ERR_OPERATION_NOT_SUPPORTED;
                        }
                    }
                    break;
#else
                    return ERR_OPERATION_NOT_SUPPORTED;
#endif
                }
                
                default:
                    break;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        case ATC_CMD_TYPE_TEST:
        case ATC_CMD_TYPE_EXECUTE:    
        default:
            status = ERR_OPERATION_NOT_SUPPORTED;
            break;
    }
#endif
    
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SFPL command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSFPL)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
    MN_PHONE_FPLMN_LIST_T fplmn_list;
    MN_DUAL_SYS_E         dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32                i = 0;
    uint16                str_len = 0;
     
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:

        MNPHONE_GetFplmnListEx(dual_sys, &fplmn_list);
        str_len = 0;
        
        if(fplmn_list.plmn_num >= 1)
        {
            str_len += sprintf((char*)g_rsp_str + str_len, "%s: %c%c%d, %d",
                              g_atc_info_table[AT_CMD_SFPL].cmd_name->str_ptr,
                              0x0d,0x0a,
                              fplmn_list.plmn_arr[0].mcc,
                              fplmn_list.plmn_arr[0].mnc
                              );
        
            for(i=1; i<fplmn_list.plmn_num; i++)
            {
                if(str_len + 10 > MAX_ATC_RSP_LEN)
                {
                    break;
                }
                str_len += sprintf((char*)g_rsp_str + str_len, "%c%c%d, %d",
                                  0x0d,0x0a,
                                  fplmn_list.plmn_arr[i].mcc,
                                  fplmn_list.plmn_arr[i].mnc
                                  );
            }
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: ",
                        g_atc_info_table[AT_CMD_SFPL].cmd_name->str_ptr);
        }
        
                              
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    
    case ATC_CMD_TYPE_READ:
        break;
    default:
        break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SEPL command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSEPL)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
    MN_PHONE_EPLMN_LIST_T eplmn_list = {0};
    MN_DUAL_SYS_E         dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32                i = 0;
    uint16                str_len = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
       
            MNNV_GetEPLMNListEx(dual_sys, &eplmn_list);
        
            if(eplmn_list.plmn_num >= 1)
            {
                str_len += sprintf((char*)g_rsp_str + str_len, "%s: %c%c%d, %d",
                                  g_atc_info_table[AT_CMD_SEPL].cmd_name->str_ptr,
                                  0x0d,0x0a,
                                  eplmn_list.plmn_arr[0].mcc,
                                  eplmn_list.plmn_arr[0].mnc
                                  );
                
                for(i=1; i<eplmn_list.plmn_num; i++)
                {
                    if(str_len + 10 > MAX_ATC_RSP_LEN)
                    {
                        break;
                    }
                    str_len += sprintf((char*)g_rsp_str + str_len, "%c%c%d, %d",
                                      0x0d,0x0a,
                                      eplmn_list.plmn_arr[i].mcc,
                                      eplmn_list.plmn_arr[i].mnc
                                      );
                }
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: ",
                            g_atc_info_table[AT_CMD_SEPL].cmd_name->str_ptr);
            }
            
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    
            break;
        
        case ATC_CMD_TYPE_READ:
            break;
        default:
            break;
    }
    
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPGSMFRQ command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPGSMFRQ)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E         dual_sys = ATC_CURRENT_SIM_ID_MUX;
    static BOOLEAN        is_force_arfcn[MN_SYS_NUMBER] = {FALSE};
    static uint8          arfcn_cnt[MN_SYS_NUMBER] = {0}; 
    uint8                 tmp_cnt = 0;
    uint8                 i;
    static uint16         arfcn_list[MN_SYS_NUMBER][6] = {0};
    
    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG)
        {
            is_force_arfcn[dual_sys] = PARA1.num;
        }
        
        if(is_force_arfcn[dual_sys])
        {
            if(PARAM2_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA2.num;
            }
            if(PARAM3_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA3.num;
            }
            if(PARAM4_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA4.num;
            }
            if(PARAM5_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA5.num;
            }
            if(PARAM6_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA6.num;
            }
            if(PARAM7_FLAG)
            {
                arfcn_list[dual_sys][tmp_cnt++] = PARA7.num;
            }
            
            arfcn_cnt[dual_sys] = tmp_cnt;
            
            PS_ForceCampon(dual_sys, tmp_cnt, arfcn_list[dual_sys]);
            
        }
        else
        {
            for(i=0; i<MN_SYS_NUMBER; i++)
            {
                is_force_arfcn[i] = FALSE;
            }
            PS_CancelForceCampon(); //cancel all card force campon
        }
        break;
    
    case ATC_CMD_TYPE_READ:
        if(is_force_arfcn[dual_sys])
        {
            uint16 str_len = 0;
            if(arfcn_cnt[dual_sys] >= 1)
            {
                str_len += sprintf((char*)g_rsp_str + str_len, "%s: %d",
                                  g_atc_info_table[AT_CMD_SPGSMFRQ].cmd_name->str_ptr,
                                  arfcn_list[dual_sys][0]
                                  );
                                      
                for(i=1; i<arfcn_cnt[dual_sys]; i++)
                {
                    str_len += sprintf((char*)g_rsp_str + str_len, ",%d",
                                      arfcn_list[dual_sys][i]
                                      );
                }
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: ",
                          g_atc_info_table[AT_CMD_SPGSMFRQ].cmd_name->str_ptr
                          );
            }
        }
        else
        {
            sprintf((char*)g_rsp_str, "%s: ",
                  g_atc_info_table[AT_CMD_SPGSMFRQ].cmd_name->str_ptr
                  );
        }
        
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;
    default:
        break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function used to sprintf calibration info for display 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetCalibrateInfo(uint8* info_buf, uint16 buf_len)
{
    uint8 tmp_buf[20] = {0};
    int32 length = 0 ;
    int32 result = 0;
    int32 offset = 0;
    uint32 reserved_info = 0;
    int32 i =0 ;

    const char *item[] = {  "PCS AGC ",  "PCS APC ",  "DCS AGC ", "DCS APC ", 
                            "EGSM AGC ", "EGSM APC ", "850AGC ",  "850APC ", 
                            "AFC ", "ADC ",
                            "PCS Final test ", "DCS Final test ", "GSM Final test ", "GSM850 Final test ",
                            "reserved ", "reserved ", "reserved ", 
                            "reserved ", "reserved ", "reserved ", 
                            "reserved ", "reserved ", "reserved ", "reserved ", 
                            "reserved ", "reserved ", "reserved ", 
                            "Test ",
                            "Custom1 ", "Custom2 ", "Custom3 ", "Custom4 ", 
                            };
    
    const int32 item_type[] = {1,1,1,1,
                               1,1,1,1,
                               1,1,
                               2, 2, 2, 2,
                               0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0,
                               3,
                               4, 4, 4, 4
                               };
    // 0:reserved 1:adc calibrate 2:test 3:custom

    const char  *if_calibrated[] = {"uncalibrated", "calibrated"};
    const char *if_passed[] = {"failed" , "passed"};

    if(PNULL == info_buf || 20 > buf_len)
    {
        return FALSE;
    }

    //从NV中读取adc_reserved[7]的信息

    reserved_info = REFPARAM_GetCaliMarkerValue();

    SCI_MEMSET(info_buf, 0, buf_len);
    offset += sprintf((char*)info_buf, "Calibration Info");

    for(i = 0; i<32; i++)
    {
        //dont display type 0, 2 and 3
        if(1 != item_type[i] && 2 != item_type[i])  
        {
            continue;
        } 
        
        //serial num                   
        SCI_MEMSET(tmp_buf, 0, 20);
        length = sprintf((char*)tmp_buf,"\r\nBIT%d:",i);
        if(offset + length >= buf_len)
        {
            return TRUE ;
        }
        SCI_MEMCPY(info_buf + offset, tmp_buf, length);
        offset += length;

        //item 
        length = strlen(item[i]); 
        if(offset + length >= buf_len)
        {
            return TRUE;
        }
        SCI_MEMCPY(info_buf + offset, item[i], length) ;
        offset += length;

        //result
        switch(item_type[i])
        {
        case 1:
            result = (reserved_info>>i) & 0x1;

            length = strlen(if_calibrated[result]);
            if(offset + length >= buf_len) 
            {
                return TRUE;
            }
            SCI_MEMCPY(info_buf + offset, if_calibrated[result], length) ;
            offset += length;
            break;
        case 2:
            result = (reserved_info>>i) & 0x1;

            length = strlen(if_passed[result]);
            if(offset + length >= buf_len) 
            {
                return TRUE;
            }
            SCI_MEMCPY(info_buf + offset, if_passed[result], length) ;
            offset += length;
            break;
        default:
            break;
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : This function handle the AT+SCDSLEEP command 
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSCDSLEEP)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
    
    #define ENABLE_DEEP_SLEEP       1
    #define DISABLE_DEEP_SLEEP      0

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:

#if defined(CHIP_VER_6810) || defined(CHIP_VER_6820)
        L1API_DSPCloseAllDevice();
        /* let linux control deep sleep */
        SCI_L1_EnableDeepSleep(ENABLE_DEEP_SLEEP);
        SCI_UPM_EnableDeepSleep(ENABLE_DEEP_SLEEP);
        SCI_Calibration_EnableDeepSleep(ENABLE_DEEP_SLEEP);
#endif
        break;
    
    case ATC_CMD_TYPE_READ:
        break;
    default:
        break;
    }
    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+SUSB command to inform usb connection status
//  Global resource dependence : none
//  Author: Elvis xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSUSB)
{
    ATC_STATUS            status = S_ATC_SUCCESS;
#ifdef MODEM_PLATFORM
    uint8    mode = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG && (ATC_CMD_PARAM_TYPE_NUMERIC == PARAM1_TYPE))
            {
                mode = (uint8)PARA1.num;
#ifdef _ATC_WAIT_L4
                if (mode == 1)
                {
                    //usb interface plugged in
                    USB_AT_Isr(TRUE);
                    atc_config_ptr->usb_flag = TRUE;
                }
                else
                {
                    //usb interface plugged out
                    USB_AT_Isr(FALSE);
                    atc_config_ptr->usb_flag = FALSE;
                }
#endif                
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
   
            break;
        
        case ATC_CMD_TYPE_READ:
            sprintf((char*)g_rsp_str, "%s: %d", 
                      g_atc_info_table[AT_CMD_SUSB].cmd_name->str_ptr,
                      atc_config_ptr->usb_flag);
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
//  Description : This function handle the AT+STPTEST command to transmit the given power
//  Global resource dependence : none
//  Author: Elvis xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSTPTEST)
{
    ATC_STATUS  status = S_ATC_SUCCESS;
    uint16 band = 0;
    uint16 arfcn = 0;
    uint16 level = 0;
    uint8  ms_band = EGSM900;
    uint8 mode = 0;  // 0: close
                             // 1: start transmit given power and power off PS
                             // 2:  start transmit given power and NOT power off PS
    MN_DUAL_SYS_E    dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_PHONE_CURRENT_PLMN_INFO_T   plmn_info;
    static BOOLEAN flag = FALSE;   // FALSE means stop transmit given power and not power off PS

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG)
            {
                mode = PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            

            if(PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
            {
                band = PARA2.num;
                arfcn = PARA3.num;
                level = PARA4.num;

                if ((band == 850) && (arfcn >= 128) && (arfcn <= 251))
                {
                    ms_band = GSM850;
                }
                else if ((band == 900) && (arfcn <= 124))
                {
                    ms_band = EGSM900;
                }
                else if ((band == 1800) && (arfcn >= 512) && (arfcn <= 885))
                {
                    ms_band = DCS1800;
                }
                else if ((band == 1900) && (arfcn >= 512) && (arfcn <= 810))
                {
                    ms_band = PCS1900;
                }
                else
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessSTPTEST wrong match (band = % d, arfcn = %d)!", band, arfcn);
                    status = ERR_OPERATION_NOT_SUPPORTED;
                    break;
                }
            }
            else if (mode != 0)
            {
                status = ERR_OPERATION_NOT_ALLOWED;
                break;
            }

            // start transmit given power and not power off PS
            if(2 ==mode )
            {
                flag = TRUE;
                L1API_SetMaxTxPwrTch(flag, level);
                return S_ATC_SUCCESS;
            }
            // stop transmit given power and not power off PS 
            else if((0 == mode) && flag)
            {
                flag = FALSE;
                L1API_SetMaxTxPwrTch(flag, 0xff);
                return S_ATC_SUCCESS;
            }
            
            SCI_MEMSET(&plmn_info, 0, sizeof(MN_PHONE_CURRENT_PLMN_INFO_T));
            plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
            switch(plmn_info.plmn_status)
            {
                case PLMN_NORMAL_GSM_ONLY:
                case PLMN_NORMAL_GSM_GPRS_BOTH:
                case PLMN_EMERGENCY_ONLY:
                    g_is_ps_act[dual_sys]   = TRUE;
                    g_is_ps_deactivate[dual_sys] = FALSE;
                    break;

                default:
                    g_is_ps_act[dual_sys]   = FALSE;
                    g_is_ps_deactivate[dual_sys] = TRUE;
                    break;
            }

            if ((g_atc_power_info.cmd_id != 0) && (g_atc_power_info.cmd_id != AT_CMD_STPTEST))
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessSTPTEST,  AT+STPTEST excludes from another ongoing");
                status = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

            if (g_is_ps_act[dual_sys])
            {
                if (0 == mode)
                {
                    SCI_TRACE_LOW("ATC: ATC_ProcessSTPTEST,  PS is power on, don't need to shut down power transmission");
                    status = ERR_OPERATION_NOT_SUPPORTED;
                    break;
                }
                //store power information for starting power transmission after ps off
                g_atc_power_info.cmd_id = AT_CMD_STPTEST;
                g_atc_power_info.arfcn = arfcn;
                g_atc_power_info.ms_band = ms_band;
                g_atc_power_info.gainpower = level;
                g_atc_power_info.is_ps_active= TRUE;

                MNPHONE_PowerOffPsEx(dual_sys); 
                ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,(uint16)APP_MN_PS_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);

                g_is_ps_act[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;
                atc_config_ptr->cfun_status[dual_sys] = ATC_FUN_DEACT_PS;
                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                SCI_TRACE_LOW("ATC: arfcn=%d, band=%d, level=%d", arfcn, ms_band, level);
                L1API_GsmTxPwrTest((BOOLEAN)mode, arfcn, ms_band, level);

                if (0 == mode)
                {
                    if (g_atc_power_info.is_ps_active)
                    {
                        MNPHONE_PowerOnPsEx(dual_sys);      
                        g_is_ps_act[dual_sys]   = TRUE;
                        g_is_ps_deactivate[dual_sys] = FALSE;
                    }
                    else
                    {
                        SCI_MEMSET(&g_atc_power_info, 0, sizeof(ATC_POWER_INF_T));
                    }
                }
            }                
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function used to start the test of GSM given power Transmission
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmTxPwrTest(ATC_CONFIG_T *atc_config_ptr)
{
    SCI_TRACE_LOW("ATC: is_power_on=%d, arfcn=%d, band=%d, gain=%d", 
        1, g_atc_power_info.arfcn, g_atc_power_info.ms_band, g_atc_power_info.gainpower);

    L1API_GsmTxPwrTest(TRUE, 
                       g_atc_power_info.arfcn, 
                       g_atc_power_info.ms_band, 
                       g_atc_power_info.gainpower
                       );

    //sprintf((char *)g_rsp_str, "%s: 0", g_atc_info_table[AT_CMD_STPTEST].cmd_name->str_ptr);
    //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    return;
}

/*****************************************************************************/
//  Description : This function handles the AT+SSWAPCOM command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSWAPCOM)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    REF_PARAM_T    ref_param = {0};
    uint8          nv_status = 0;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG && PARAM2_FLAG)
            {
                nv_status = EFS_NvitemRead(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param));    
                if (NVERR_NONE == nv_status)
                {
                    ref_param.com_debug = PARA1.num;
                    ref_param.com_data = PARA2.num;
                    EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param), 1);
                }
                else
                {
                    status = ERR_NOT_FOUND;
                }
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
   
            break;
        
        case ATC_CMD_TYPE_READ:
            nv_status = EFS_NvitemRead(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param));    
            if (NVERR_NONE == nv_status)
            {
                sprintf((char*)g_rsp_str, "%s: %d,%d", 
                          g_atc_info_table[AT_CMD_SSWAPCOM].cmd_name->str_ptr,
                          ref_param.com_debug,
                          ref_param.com_data);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                status = ERR_NOT_FOUND;
            }
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SASYNC command to set async mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSASYNC)
{
    ATC_STATUS     status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG)
            {
                atc_config_ptr->async_mode_flag = PARA1.num;
            }
            else
            {
                status = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        
        case ATC_CMD_TYPE_READ:
            sprintf((char*)g_rsp_str, "%s: %d", 
                      g_atc_info_table[AT_CMD_SASYNC].cmd_name->str_ptr,
                      atc_config_ptr->async_mode_flag);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;

        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPTEST command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPTEST)
{
    uint8 type = 0;
    uint8 flag = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T info;
    MN_PHONE_USER_SERVICE_SELECT_E user_service;
    uint8 *ptr = PNULL;
    MN_GPRS_EXT_QOS_INFO_T ext_qos = {0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:

        if (PARAM1_FLAG && PARAM2_FLAG)
        {
            type=( uint8 )PARA1.num;

            switch(type)
            {
            case TRAFFIC_CLASS_TEST:
#ifdef _GPRS_FULL_FUNCTION_
                {
                    ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
                    BOOLEAN item_setted[13] = {0,0,0,0,1,0,0,0,0,0,0,0,0};

                    ext_qos.traffic_class=(MN_GPRS_TRAFFIC_CLASS_E)PARA2.num;
                    ret= MNGPRS_SetStorageExtReqQosWithItemEx(dual_sys, MN_GPRS_STORAGE_NV, DOWNLOAD_E, &ext_qos, item_setted);
                    if(ret!=ERR_MNGPRS_NO_ERR)
                    {
                        return ERR_OPERATION_NOT_SUPPORTED;
                    }
                }
#endif
                break;

            case SERVICE_TYPE_TEST:
                user_service=(MN_PHONE_USER_SERVICE_SELECT_E)PARA2.num+1;
                MNPHONE_SetServiceTypeEx(dual_sys, user_service);
                break;
                
            case INTEGRALITY_TEST:
            case RW_TEST:
            case AOC_TEST:
            case GSIM_SUPPORT_TEST:
            case PS_VERSION_TEST:
            case WAKEUP_CONDITION_TEST:
            case DSP_LOG_TEST:
            case FBAND_TEST:
            case CELL_CAPABILITY_TEST:
                return ERR_OPERATION_NOT_SUPPORTED;

            default:
                break;
            }
        }
        else if(PARAM1_FLAG && (!PARAM2_FLAG))
        {
            if((uint8 )PARA1.num == CELL_CAPABILITY_TEST)
            {
                // get cell capability
                info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

                if(info.cell_capability.mbms_support)
                {
                    flag = (1<<0);
                }

                if(info.cell_capability.hsdpa_support)
                {
                    flag |= (1<<1);
                }

                if(info.cell_capability.hsupa_support)
                {
                    flag |= (1<<2);
                }

                if(info.cell_capability.edge_support)
                {
                    flag |= (1<<3);
                }

                sprintf((char *)g_rsp_str, "%s:%d,%d", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CELL_CAPABILITY_TEST, flag);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                return ERR_INVALID_INDEX;
            }
        }
        else
        {
            return ERR_INVALID_INDEX;
        }

        break;

    case ATC_CMD_TYPE_READ:
        //get fta flag
        ptr = g_rsp_str;
        //get INTEGRALITY flag
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, INTEGRALITY_TEST, 0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        //get real network flag
        sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, RW_TEST, 0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        //em_get_aoc_nv_param(&aoc_vl);
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, AOC_TEST, 0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        // get gsm support flag
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, GSIM_SUPPORT_TEST, 0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        // get PS Version
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, PS_VERSION_TEST, 0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        // get traffic class
        //ext_qos= MNGPRS_GetExtendQos(DOWNLOAD_E);
        MNGPRS_ReadStorageExtReqQosEx(dual_sys,MN_GPRS_STORAGE_NV, DOWNLOAD_E, &ext_qos);
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, TRAFFIC_CLASS_TEST, ext_qos.traffic_class);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        // get service class
        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, SERVICE_TYPE_TEST, MNPHONE_GetServiceTypeEx(dual_sys));
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        // get cell capability
        info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

        if(info.cell_capability.mbms_support)
        {
            flag = (1<<0);
        }

        if(info.cell_capability.hsdpa_support)
        {
            flag |= (1<<1);
        }

        if(info.cell_capability.hsupa_support)
        {
            flag |= (1<<2);
        }

        if(info.cell_capability.edge_support)
        {
            flag |= (1<<3);
        }

        sprintf((char *)ptr, "%s:%d,%d\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, CELL_CAPABILITY_TEST,flag);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        //wake up condition for dsp
        //em_get_wakeup_condition_for_dsp_stat(&s_item,0);
        sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, WAKEUP_CONDITION_TEST,0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        //em_get_dsp_log_switch(&s_item,0);
        sprintf((char *)ptr, "%s:%d,%lu\r\n", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, DSP_LOG_TEST,0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        //FBAND support flag
        sprintf((char *)ptr, "%s:%d,%d", g_atc_info_table[AT_CMD_SPTEST].cmd_name->str_ptr, FBAND_TEST,0);
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        break;

    default:
        SCI_ASSERT(0);/*assert verified:Check_value */
        break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the UPLMN info
//  Global resource dependence :
//  Author: Elvis.Xu
//  Note:
/*****************************************************************************/
void ATC_InitUplmnCfgInfo(MN_DUAL_SYS_E  dual_sys)
{
    g_atc_uplmn_info[dual_sys].index = 0xff;
    g_atc_uplmn_info[dual_sys].mode = ATC_UPLMN_MAX;
    g_atc_uplmn_info[dual_sys].plmn.mnc_digit_num = 0;
    g_atc_uplmn_info[dual_sys].plmn.mcc = 0;
    g_atc_uplmn_info[dual_sys].plmn.mnc = 0;
    g_atc_uplmn_info[dual_sys].rat = MN_GMMREG_RAT_UNKNOWN;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPUPLMN command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPUPLMN )
{
    ATC_STATUS      status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if (!atc_config_ptr->sim_flag[dual_sys])
    {
        return ERR_SIM_NOT_INSERTED;
    }

    if (ATC_UPLMN_MAX != g_atc_uplmn_info[dual_sys].mode)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessSPUPLMN, mode %d pending", g_atc_uplmn_info[dual_sys].mode);
        return ERR_SIM_BUSY;
    }

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:

        if (PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
        {
            char uplmn_str[MAX_UPLMN_LEN+1] = {0};
            uint8 uplmn_len = 0;
            uint32 tmp_plmn = 0;


            g_atc_uplmn_info[dual_sys].index = (uint8)PARA1.num;

            if (PARA3.num == 0)
            {
                g_atc_uplmn_info[dual_sys].rat = MN_GMMREG_RAT_3G;
            }
            else if (PARA3.num == 1)
            {
                g_atc_uplmn_info[dual_sys].rat = MN_GMMREG_RAT_VDS_GSM;
            }
            else
            {
                g_atc_uplmn_info[dual_sys].rat = MN_GMMREG_RAT_UNKNOWN;
            }


            if((PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)||(PARA2.str_ptr->str_len > MAX_UPLMN_LEN))
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }

            strcpy(uplmn_str, (char*)PARA2.str_ptr->str_ptr);
            uplmn_len = (uint8)PARA2.str_ptr->str_len;
            uplmn_str[uplmn_len] = '\0';
            
            if(!CheckPlmnStr(uplmn_len, (uint8*)uplmn_str))
            {
                return ERR_INVALID_INDEX;
            }

            tmp_plmn = (uint32)atoi(uplmn_str);

            if(uplmn_len == 6)
            {
                g_atc_uplmn_info[dual_sys].plmn.mnc_digit_num = 3;
                g_atc_uplmn_info[dual_sys].plmn.mcc = tmp_plmn/1000;
                g_atc_uplmn_info[dual_sys].plmn.mnc = tmp_plmn%1000;
            }
            else if(uplmn_len == 5)
            {
                g_atc_uplmn_info[dual_sys].plmn.mnc_digit_num = 2;
                g_atc_uplmn_info[dual_sys].plmn.mcc = tmp_plmn/100;
                g_atc_uplmn_info[dual_sys].plmn.mnc = tmp_plmn%100;
            }
            else  //uplmn_len 4
            {
                g_atc_uplmn_info[dual_sys].plmn.mnc_digit_num = 1;
                g_atc_uplmn_info[dual_sys].plmn.mcc = tmp_plmn/10;
                g_atc_uplmn_info[dual_sys].plmn.mnc = tmp_plmn%10;
            }

            g_atc_uplmn_info[dual_sys].mode = ATC_UPLMN_SET;
            MNSIM_GetPLMNWACTEx(dual_sys);
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_PREFER_PLMN_CHANGE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
            status = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            status = ERR_INVALID_INDEX;
        }

        break;

    case ATC_CMD_TYPE_READ:
        g_atc_uplmn_info[dual_sys].mode = ATC_UPLMN_GET;
        MNSIM_GetPLMNWACTEx(dual_sys);
        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, (uint16)APP_MN_PREFER_PLMN_CHANGE_CNF, 0xff, ATC_NO_DOMAIN, dual_sys);
        status = S_ATC_DEFAULT_HANDLE;
        break;

    case ATC_CMD_TYPE_TEST:
    default:
        break;
    }

    return status;
}


/*****************************************************************************/
//  Description : This function handles the AT+SPID command
//  Global resource dependence : g_rsp_str
//  Author:       Lisa Lin
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPID)
{
    MN_ENG_PHONE_INFO_T phone_info = {0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_IMEI_T imei = {0};
    uint8   *ptr=PNULL;
    char    temp_str[20]="\0";

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:

        if(MN_RETURN_SUCCESS != MNENG_GetPhoneInfoEx(dual_sys, &phone_info))
        {
            SCI_TRACE_LOW("ATC: fail to get GetPhoneInfo");
            return ERR_OPERATION_NOT_ALLOWED;
        }

        //to get tmsi
        ptr = g_rsp_str;
        if (phone_info.tmsi.valid_flag)
        {
            ATC_IntArray_To_Str(phone_info.tmsi.tmsi_value,sizeof(phone_info.tmsi.tmsi_value), temp_str);
            sprintf((char *)ptr, "%s%s\r\n", "TMSI:",temp_str);
        }
        else
        {
            sprintf((char *)ptr, "%s\r\n", "TMSI:");
        }
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        memset(temp_str,0,sizeof(temp_str));

        //to get ptmsi
        if (phone_info.ptmsi.valid_flag)
        {
            ATC_IntArray_To_Str(phone_info.ptmsi.ptmsi_value,sizeof(phone_info.ptmsi.ptmsi_value), temp_str);
            sprintf((char *)ptr, "%s%s\r\n", "PTMSI:",temp_str);
        }
        else
        {
            sprintf((char *)ptr, "%s\r\n", "PTMSI:");
        }
        ptr = g_rsp_str + strlen((char *)g_rsp_str);

        memset(temp_str,0,sizeof(temp_str));

        //to get imeisv
        if (MNNV_GetIMEIEx(dual_sys, imei))
        {
            ATC_IntArray_To_Str(imei, sizeof(imei), temp_str);
            sprintf((char *)ptr, "%s%s", "IMEISV:",temp_str);
        }
        else
        {
            sprintf((char *)ptr, "%s", "IMEISV:");
        }

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        break;

    default:
        SCI_ASSERT(0);/*assert verified:Check_value */
        break;
    }

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT^CARDMODE command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_CARDMODE)
{
    uint32   status = S_ATC_SUCCESS;
    SIMCHECK_GET_SIM_TYPE_CNF_T  atc_sim_type;
    AT_SimType atc_simtype = UNKNOWN_MODE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

 
    atc_sim_type.card_type = MN_CARD_TYPE_NUM;
    atc_config_ptr->spreadyflag = 1;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        MNPHONE_GetSimtypeEx(dual_sys, &atc_sim_type);

        if((atc_sim_type.cause == MN_SIM_TYPE_SUCCESS)
          ||(atc_sim_type.cause == MN_SIM_TYPE_NOT_READY))
        {
            if(TRUE == atc_config_ptr->sim_flag[dual_sys])
            {
                if ((atc_sim_type.card_type == MN_CARD_TYPE_SIM) 
                    || (atc_sim_type.card_type == MN_CARD_TYPE_DUAL))//Dual is CDMA GSM dual
                {
                    atc_simtype = SIM_MODE;
                }
                else if(atc_sim_type.card_type == MN_CARD_TYPE_USIM)
                {
                    atc_simtype = USIM_MODE;
                }
                else
                {
                    atc_simtype = UNKNOWN_MODE;
                }
#if 0
                if(MNRSF_IsTestSIM(dual_sys) && (SIM_MODE==atc_simtype ))
                {
                    atc_simtype = TEST_SIM_MODE;
                }
                else if(MNRSF_IsTestSIM(dual_sys) && (USIM_MODE==atc_simtype ))
                {
                    atc_simtype = TEST_USIM_MODE;
                }
#endif
                sprintf((char *)g_rsp_str, "^CARDMODE:%d", atc_simtype);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if(atc_config_ptr->call_read_flag[dual_sys] == 1)
                {
                    sprintf((char *)g_rsp_str, "*SPREADY: 0");
                    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
                }

                if(atc_config_ptr->sms_is_ready[dual_sys] == 1)
                {
                    sprintf((char *)g_rsp_str, "*SPREADY: 1");
                    ATC_BuildUnsolicitedInfo(dual_sys, atc_config_ptr, g_rsp_str);
                }
            }
            else
            {
                status = ERR_SIM_NOT_INSERTED/*ERR_SIM_FAILURE*/;
            }
        }
        else if(atc_sim_type.cause == MN_SIM_TYPE_NO_SIM)
        {
            status = ERR_SIM_NOT_INSERTED;
        }
        else
        {
            status = ERR_SIM_FAILURE;
        }

        break;

    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the ATC^SYSCONFIG command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SYSCONFIG)
{
    return ERR_OPERATION_NOT_SUPPORTED;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPDSP command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDSP)
{
    return ERR_OPERATION_NOT_SUPPORTED;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPFRQ command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPFRQ)
{
    return ERR_OPERATION_NOT_SUPPORTED;
}

/*****************************************************************************/
//  Description : This function handles the AT+SPDIAG command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPDIAG)
{
    uint32   status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (!PARAM1_FLAG || (PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING))
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, no param");
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }

        if (PARA1.str_ptr->str_len == 0)
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, empty sting");
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
        }

        break;

    default:
        status = ERR_OPERATION_NOT_ALLOWED;
        break;
    }

    if (status != S_ATC_SUCCESS)
    {
        sprintf((char *)g_rsp_str, "+SPDIAG:%d", status);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return status;
    }
    else
    {
        uint8 *new_at_cmd_ptr = PNULL;
        uint32 cmd_len = 0;

        cmd_len = PARA1.str_ptr->str_len + 2; //<CR><cmd><CR>
        new_at_cmd_ptr = (uint8*)SCI_ALLOC_BASEZ(cmd_len + 1);

        if (new_at_cmd_ptr == PNULL)
        {
            SCI_TRACE_LOW("ATC: ATC_ProcessSPDIAG, memory full");
            status = ERR_MEMORY_FULL;
            sprintf((char *)g_rsp_str, "+SPDIAG:%d", status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        }
        else
        {
            new_at_cmd_ptr[0] = 0x0d;  //<CR>
            SCI_MEMCPY(&new_at_cmd_ptr[1], PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
            new_at_cmd_ptr[cmd_len - 1] = 0x0d;  //<CR>
            new_at_cmd_ptr[cmd_len] = '\0';
#ifdef _MUX_ENABLE_
            ATC_SendNewATInd(atc_config_ptr->current_link_id, cmd_len, new_at_cmd_ptr);
            SCI_FREE(new_at_cmd_ptr);
#else
            if (g_diag_cmd_buffer_ptr)
            {
                SCI_FREE(g_diag_cmd_buffer_ptr);
                g_diag_cmd_buffer_ptr = PNULL;
            }
            g_diag_cmd_buffer_ptr = new_at_cmd_ptr;
            g_is_diag_cmd = TRUE;
            
            SendNewATInd(cmd_len);
#endif
        }
        
    }

    return S_ATC_DEFAULT_HANDLE;
}

#ifndef _MUX_ENABLE_
/*****************************************************************************/
//  Description : This function is used to read command line from diag command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
void ATC_ReadDiagCmdLine (
    uint8 *data_ptr,       // Output argument, point to the buf which is alloced
    uint length,           // The length of cmd line which should be read from
    uint *read_length_ptr  // The actual read length
)
{
    if (g_diag_cmd_buffer_ptr)
    {
        *read_length_ptr = length;
        SCI_MEMCPY(data_ptr, g_diag_cmd_buffer_ptr, length);

        SCI_FREE(g_diag_cmd_buffer_ptr);
        g_diag_cmd_buffer_ptr = PNULL;
        g_is_diag_cmd = FALSE;
    }
    else
    {
        *read_length_ptr = 0;
        g_is_diag_cmd = FALSE;
    }
}
#endif


/*****************************************************************************/
//  Description : This function is used to get the parameters of diag command
//  Global resource dependence : none
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetDiagParams(
    uint8            *in_ptr,
    uint8             in_len,
    ATC_DIAG_PARAM_T *param_ptr
)
{
    uint8            i;
    uint8            j = 0;
    BOOLEAN          flag = TRUE;

    if(NULL == in_ptr || NULL == param_ptr || in_len < 2)
    {
        return FALSE;
    }

    SCI_MEMSET((void*)param_ptr->mode, 0, ATC_MAX_DIAG_MODE_LEN);
    SCI_MEMSET((void*)param_ptr->act_value, 0, ATC_MAX_DIAG_ACT_LEN);
    param_ptr->mode_len = 0;
    param_ptr->cmd_type = ATC_CMD_TYPE_SET;
    
    
    for(i = 0; i < in_len; i++)
    {
        if(in_ptr[i] == ',')
        {
            i++;
            if ((in_ptr[i] != '"') && (i < in_len))
            {
                for (j = 0; ((i + j) < in_len) && (j < ATC_MAX_DIAG_ACT_LEN); j++ )
                {
                    param_ptr->act_value[j] = in_ptr[i + j];
                }
                param_ptr->cmd_type = ATC_CMD_TYPE_SET;
            }
            else
            {
                flag = FALSE;
            }
            
            break;
            
        }
        else if (in_ptr[i] == '?')
        {
            param_ptr->cmd_type = ATC_CMD_TYPE_READ;
            break;
        }
        else
        {
            if(j < ATC_MAX_DIAG_MODE_LEN)
            {
                if(in_ptr[i] != '"')
                {
                    param_ptr->mode[j++] = in_ptr[i];
                    param_ptr->mode_len += 1;
                }
            }
            else if(in_ptr[i] != '"')
            {
                flag = FALSE;
                break;
            }
        }
    }

    SCI_TRACE_LOW("ATC: get the diag parameters %d", flag);
    return flag;
}

/*****************************************************************************/
//  Description : This function handles the AT+SGETIQ command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSGETIQ)
{
    ATC_STATUS      status = S_ATC_SUCCESS;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:

        if (PARAM1_FLAG)
        {
            SCI_TRACE_LOW("ATC_ProcessSGETIQ  Enter!!!  %d",PARA1.num);
            s_atc_iq_log_mode = PARA1.num;
            
            if (PARA1.num == 1)
            {
                AUDIO_EnableDsp_Log_IQData(DSP_LOG_DATA_ON);
            }
            else if(PARA1.num == 2)
            {
                AUDIO_EnableDsp_Log_IQData(DSP_IQ_DATA_ON);
            }
            else
            {
                AUDIO_EnableDsp_Log_IQData(DSP_GETLOG_OFF);
            }
        }
        else
        {
            status = ERR_OPERATION_NOT_SUPPORTED;
        }

        break;

    case ATC_CMD_TYPE_READ:
        sprintf((char *)g_rsp_str, "+SGETIQ: %d", s_atc_iq_log_mode);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;        

    default:
        status = ERR_OPERATION_NOT_SUPPORTED;
        break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+SSIMDETPIN command to set async mode
//  Global resource dependence : g_rsp_str
//  Author:       Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSSIMDETPIN)
{
    BOOLEAN gpio_status = TRUE;
    uint32 sim_status = 0;
    ATC_STATUS     status = S_ATC_SUCCESS;
 
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
#ifndef WIN32
            if (GPIO_GetDataMask(3))
            {
                if (!GPIO_GetDirection(3))
                {
                    //the current customer use GPIO_3 as the sim detect PIN
                    gpio_status = GPIO_GetValue(3);// get value. 
                }
            }
#endif
            if (gpio_status)//high level means no-sim
            {
                sim_status = 0;
            }
            else//low level means sim is inserted
            {
                sim_status = 1;
            }
           sprintf((char*)g_rsp_str, "%s: %d", 
                      g_atc_info_table[AT_CMD_SSIMDETPIN].cmd_name->str_ptr,
                      sim_status);
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            break;
        default:
            status = ERR_OPERATION_NOT_ALLOWED;
            break;
    }
 
    return status;
}

