/******************************************************************************
 ** File Name:      atc_samsung_prod.c                                                     *
 ** Author:         Elvis.Xu                                             *
 ** Date:           05/10/2012                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains all commands related with Samsung production line tool     *
 ******************************************************************************

  ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/10/2012     Elvis.Xu         Create.                                   *
 ******************************************************************************/
#include "atc.h"
#include "atc_common.h"
#include "env_atc_signal.h"
#include "atc_samsung_prod.h"
#include "atc_gsm_sms.h"
#include "layer1_engineering.h"
#include "base_trc.h"
#include "dal_time.h"
#include "version.h"
#include "nv_item_id.h"
#include "dal_chr.h"
#include "custom_tone_adp.h"
#include "aud_gen.h"
#include "mnclassmark_api.h"
#include "prod_param.h"
#include "wdg_drvapi.h"
#include "production_test.h"
#include "gpio_prod_api.h"
#include "adc_parameter.h"
#include "keypad.h"
#include "ref_param.h"
#ifndef WIN32
#include "mnnv_api.h"
#endif
#include "sig_code.h"
#ifndef WIN32
#include "bt_abs.h"
#endif

#include "atc_sha1.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
#define ATC_NV_SIM_LOCK_CUSTOMIZE_DATA_ID 496

#define ATC_GET_BT_STATUS_INTERVAL       2000
#define  ATC_GET_BT_SEARCH_INTERVAL      8000

#ifndef wchar
typedef uint16 wchar;
#endif

ATC_POWER_INF_T    g_atc_power_info = {0};
uint8    g_atc_read_rssi_times = 12;
SCI_TMID    T_ATC_RSSI_READ_EXP_IND = PNULL;

SCI_TMID    T_ATC_BT_STATUS_IND = PNULL;
SCI_TMID    T_ATC_BT_SEARCH_IND = PNULL;

ATC_LOOP_TEST_INFO_T loop_test_info = {0};
static BOOLEAN ak_seed_no_verified  = FALSE;
static BOOLEAN msl_verified               = FALSE;

extern RSP_RESULT_STR     g_rsp_str;
extern MN_DUAL_SYS_E      g_current_card_id;
extern BOOLEAN               g_is_ps_act[MN_SYS_NUMBER];
extern BOOLEAN               g_is_ps_deactivate[MN_SYS_NUMBER] ;
extern const ATC_INFO_T            g_atc_info_table[];

extern PUBLIC BOOLEAN SPIFLASH_ReadUNID(uint8 *pUNID);
extern PUBLIC uint8* MMIAPICUS_GetDefaultSalesCode(void);
extern uint16 MMIAPICL_GetRecordInfo(void);
extern uint16 MMIAPIPB_GetPBRecordNumFromNV(void);
extern BOOLEAN KPD_GetKeyStatus(void);
extern void MMIDEFAULT_TurnOnBackLight(void);
extern BOOLEAN MMIFILE_GetDeviceSDStatus(void);
extern BOOLEAN MMIAPIFM_GetTotalAndUsedInExternalMem(uint32 *used_space_low ,uint32 *used_space_high ,uint32 *total_space_high ,uint32 *total_space_low);
extern wchar* MMIAPICOM_StrToWstr(const uint8* src,wchar* dst);
extern BOOLEAN MMIAPIFMM_IsFileExist(const wchar *full_path_ptr,uint16 full_path_len);
extern size_t MMIAPICOM_Wstrlen( const wchar* str );


/*****************************************************************************/
//  Description : This function used to check detal lock keys
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN key_check(uint8 *keys, uint16 key_len);

/*****************************************************************************/
//  Description : This function used to extract parameters
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetMultiParamsEx(
                         uint8 *in_ptr, 
                         uint8 in_len, 
                         ATC_PRODUCTION_LINE_PARAM_T *param_ptr
                         );

/*****************************************************************************/
//  Description : This function handle get IMEI check Digit
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
LOCAL uint8 ATC_GetIMEICheckDigit(uint8 * imei);

/*****************************************************************************/
//  Description : This function handle Record History test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
LOCAL void ATC_RecordHistoryNV(ATC_FACTORY_ITEM_T hist_nv);

/*****************************************************************************/
//  Description : This function is used to get the parameters (AT+IMEITEST,AT+SETTESTNV)
//  Global resource dependence : none
//  Author:       longting zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetParamsEx(
                         uint8 *in_ptr, 
                         uint8 in_len, 
                         ATC_PRODUCTION_LINE_PARAM_T *param_ptr
                         );
#ifndef WIN32
/*****************************************************************************/
//  Description : This function handle the AT+HEADINFO command 
//                 headinfo for phone information
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessHEADINFO)
{   

    uint8 headcoder[5] = {0x7F, 0xAA, 0xAF, 0x7E, 0x00};
    uint8 *model_name = NULL;
    uint16 model_name_len = 0;
    uint8 model_name_hex[91] = {0};
    uint8 country[] = "OPEN";
    uint8 country_hex[81]= {0};
    uint8 *customer_code = NULL;
    uint8 customer_code_hex[91] = {0};
    uint8 *release_date = NULL;
    uint8 release_date_hex[91] = {0};
    char *charger = "AA55";
    char charger_hex[49] = {0};
    uint8 *sw_version = NULL;
    uint8 sw_version_hex[91] = {0};
    uint32 manufacture_id = 0;
    uint8 manufacture_id_hex[21] = {0};
    uint32 did1 = 0;
    uint32 did2 = 0;
    uint16 device_id = 0;
    uint8 device_id_hex[21] = {0};
    uint8 unique_no[41] = {0};
    uint8 unique_no_str[9] = {0};
    uint8 *memory_name = NULL;
    uint8 memory_name_hex[91] = {0};
    char *sec_code = "-";
    uint8 sec_code_hex[41] = {0};
    char *etc = "AA55";
    char etc_hex[81] = {0};
    uint8 str_hex_tmp[90] = {0};
    uint8 i = 0;
    uint16 str_len = 0;

    SCI_TRACE_LOW("ATC: ATC_ProcessHEADINFO");
    
    if(!PARAM1_FLAG || !PARAM2_FLAG)
    {
        SCI_TRACE_SLIGHT("ATC: invalid parameter");
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS; 
    }

    // model name
    model_name = (uint8*)CLASSMARK_GetModelName();
    model_name_len = strlen((char *)model_name);
    
    ConvertBinToHex(model_name, model_name_len, str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: hex_len = %d model_name=%s", strlen((char *)str_hex_tmp), model_name);

    SCI_MEMSET(model_name_hex, '0', 90);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY((void *)model_name_hex, (void *)str_hex_tmp, str_len);

    SCI_TRACE_SLIGHT("ATC: model_name_hex=%s", model_name_hex);

    // country 
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    ConvertBinToHex(country, strlen((char*)country), str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: country hex =%s", str_hex_tmp);
    SCI_MEMSET(country_hex, '0', 80);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY((void *)country_hex, (void *)str_hex_tmp, str_len);

    // customer code
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    customer_code = MMIAPICUS_GetDefaultSalesCode();
    if(NULL != customer_code)
    {
        SCI_TRACE_SLIGHT("ATC: customer_code=%s", customer_code);
        ConvertBinToHex(customer_code, strlen((char *)customer_code), str_hex_tmp);
        SCI_TRACE_SLIGHT("ATC: customer_code hex =%s, len=%d", str_hex_tmp,strlen((char *)str_hex_tmp));
        SCI_MEMSET(customer_code_hex, '0', 90);
        str_len = strlen((char *)str_hex_tmp);
        SCI_MEMCPY(customer_code_hex, str_hex_tmp, str_len);
    }

    // Release date
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    #ifndef WIN32
    release_date = (uint8 *)VERSION_GetInfo(BUILD_TIME);
    #endif
    while(release_date[i]!=' ')
    {
        i++;
    }

    ConvertBinToHex(release_date, i, str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: release_date=%s, release_date hex =%s", release_date, str_hex_tmp);
    SCI_MEMSET(release_date_hex, '0', 90);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(release_date_hex, str_hex_tmp, str_len);

    // charger
    SCI_MEMSET(charger_hex, '0', 48);
    str_len = strlen(charger);
    SCI_MEMCPY(charger_hex, charger, str_len);

    // s/w version
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    #ifndef WIN32
    sw_version = (uint8 *)VERSION_GetInfo(BASE_VERSION);
    #endif
    SCI_TRACE_SLIGHT("ATC: sw_version=%s", sw_version);
    ConvertBinToHex(sw_version, strlen((char *)sw_version), str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: sw_version hex =%s", str_hex_tmp);
    SCI_MEMSET(sw_version_hex, '0', 90);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(sw_version_hex, str_hex_tmp, str_len);

    // checksum, Manufacture ID for Nor flash
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    PROD_GetFlashID(&manufacture_id, &did1, &did2); // SPIFLASH_ReadID(&manufacture_id, &did1, &did2);
    SCI_TRACE_SLIGHT("ATC: did1=%d, did2=%d", did1, did2);  
    sprintf((char *)&manufacture_id_hex[0], "%d", manufacture_id);
    ConvertBinToHex(manufacture_id_hex, strlen((char *)manufacture_id_hex), str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: manufacture_id_hex = %s", str_hex_tmp);
    SCI_MEMSET(manufacture_id_hex, '0', 20);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(manufacture_id_hex, str_hex_tmp, str_len);
    SCI_TRACE_SLIGHT("ATC: mid after cpy=%s", manufacture_id_hex);
    
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    device_id = (did1<<8) | (did2);
    SCI_TRACE_SLIGHT("ATC: device_id = %d", device_id);
    sprintf((char *)&device_id_hex[0], "%d", device_id);
    ConvertBinToHex(device_id_hex, strlen((char *)device_id_hex), str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: device_id_hex = %s", str_hex_tmp);
    SCI_MEMSET(device_id_hex, '0', 20);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(device_id_hex, str_hex_tmp, str_len);    
    
    // unique_no,
    SCI_MEMSET(unique_no, '0', 40);
    #ifndef WIN32
    SPIFLASH_ReadUNID(unique_no_str);
    #endif

    { 
        uint8    semi_unique_no_str    = 0;
        uint8    index = 0;
        for(i=0; i<9; i++, index+=2)
        {
            SCI_TRACE_SLIGHT("atc: un number[%d]=%x", i, unique_no_str[i]);

            semi_unique_no_str = (uint8)((unique_no_str[i] & 0xF0) >> 4);
            if (semi_unique_no_str <= 9) //semi_octet >= 0
            {
                unique_no[index] = (uint8)(semi_unique_no_str + '0');
            }
            else 
            {
                unique_no[index] = (uint8)(semi_unique_no_str + 'A' - 10);
            }

            semi_unique_no_str = (uint8)(unique_no_str[i] & 0x0f);
            if (semi_unique_no_str <= 9) // semi_octet >= 0
            {
                unique_no[index+1] = (uint8)(semi_unique_no_str + '0');
            }
            else 
            {
                unique_no[index+1] = (uint8)(semi_unique_no_str + 'A' - 10);
            }
        }
    }
    
    // memory_name
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    #ifndef WIN32
    memory_name = (uint8 *)VERSION_GetInfo(MEMORY_NAME);
    #endif
    SCI_TRACE_SLIGHT("ATC: memory_name=%s", memory_name);
    ConvertBinToHex(memory_name, strlen((char *)memory_name), str_hex_tmp);
    SCI_TRACE_SLIGHT("ATC: memory_name hex =%s", str_hex_tmp);
    SCI_MEMSET(memory_name_hex, '0', 90);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(memory_name_hex, str_hex_tmp, str_len);
    
    // sec code
    SCI_MEMSET(str_hex_tmp, 0x0, sizeof(str_hex_tmp));
    SCI_MEMSET(sec_code_hex, '0', 40);
    sprintf((char *)&str_hex_tmp[0], "%x", sec_code[0]);
    str_len = strlen((char *)str_hex_tmp);
    SCI_MEMCPY(sec_code_hex, str_hex_tmp, str_len);

    // Etc
    SCI_MEMSET(etc_hex, '0', 80);
    str_len = strlen(etc);
    SCI_MEMCPY(etc_hex, etc, str_len);
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // Read Headerinfo, hex mode
            if (1 == PARA1.num && 0==PARA2.num)
            {
                sprintf((char*)g_rsp_str, "+HEADINFO:%d,%x%x%x%x%.40s%.80s%.16s%.28s%.48s%.40s%.20s%.20s%.40s%.40s%.40s%.80s", PARA1.num, headcoder[0], headcoder[1], headcoder[2], headcoder[3],
                    model_name_hex, country_hex, customer_code_hex,release_date_hex,charger_hex,sw_version_hex, manufacture_id_hex, device_id_hex, unique_no, memory_name_hex, sec_code_hex, etc_hex);
            }
            // string mode
            else if(1 == PARA1.num && 1==PARA2.num)
            {
                sprintf((char*)g_rsp_str, "+HEADINFO:%d,\r\nModel Name = %.40s\r\nCountry/customer=%.80s\r\nCustomer Code = %.16s\r\nDate = %s\r\nCharger = %.48s\r\nS/W version = %.40s\r\nUnique Number = %.40s\r\nMemory Name = %.40s\r\nSec Code = %s\r\n", PARA1.num,
                    model_name, country, customer_code,release_date,charger,sw_version, unique_no, memory_name, sec_code);
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+SYSSLEEP command 
//                 set phone to sleep/wakeup
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSYSSLEEP)
{
    int               loop  = 0;
    SCI_TRACE_LOW("ATC: ATC_ProcessSYSSLEEP");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG || !PARAM2_FLAG)
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break; 
            }
            
            // sleep
            if (0 ==PARA1.num && 0==PARA2.num)
            {
                 sprintf((char *)g_rsp_str, "+SYSSLEEP:%d\r\n\r\nOK", PARA1.num);
                 ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                 SCI_Sleep(100);

 // Hold DSP and Download the simple DSP code to deep sleep
                {
                    void L1API_DSPCloseAllDevice (void);
                    //          SCI_TRACE_LOW( "DSP Close All Device" );
                    L1API_DSPCloseAllDevice();
                }
    
                SCI_EnterAssertMode();
                
                // Disable Interrupt controller
                INT_HAL_DisableINT();/*lint !e718 !e746 -esym(628, INT_HAL_DisableINT)*/
    
                // Disable the irq
                SCI_DisableIRQ();
                WDG_TimerStop();
    
                SCI_ProductTest_Current_Prepare_DeepSleep();
    
                OS_TickDelay (100);
                // Let ARM enter deep sleep
                SCI_Deep_Sleep_Test();/*lint !e718 !e746 -esym(628,SCI_Deep_Sleep_Test)*/
    
                for (loop = 0; loop < 0x4000; loop++)
                {
                  ;
                }
            }
            // wake up
            else if (0 ==PARA1.num && 1==PARA2.num)
            {
                MMIDEFAULT_TurnOnBackLight();
                sprintf((char *)g_rsp_str, "+SYSSLEEP:%d", PARA1.num);
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+FCEPTEST command 
//                 Ear phone test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCEPTEST)
{

    AUDIO_RESULT_E                error_code = AUDIO_NO_ERROR;
    BOOLEAN                            pin_detect = FALSE;
    uint16                          usVoiceFormat = 1; //app determine the voice format.
    static BOOLEAN     is_pcm_loopback_on = FALSE;
    static BOOLEAN is_packet_loopback_on = FALSE;
        
    SCI_TRACE_LOW("ATC: ATC_ProcessFCEPTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG || !PARAM2_FLAG || !PARAM3_FLAG || !PARAM4_FLAG)
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }

            // Ear Mic - Ear SPK PCM loopback on
            if ( 0==PARA1.num && 0==PARA2.num && 0==PARA3.num && 0==PARA4.num)
            {
                is_pcm_loopback_on = TRUE;

                AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);
                error_code = AUD_EnableVoiceLoopback(TRUE, 1, 0, 100);
                SCI_TRACE_SLIGHT("ATC: enabel voice loopback errorcode=%d", error_code);
                if(AUDIO_NO_ERROR == error_code)
                {
                    sprintf((char *)g_rsp_str, "+FCEPTEST:%d", PARA1.num);
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            // Ear Mic - Ear SPK Packet loopback on
            else if( 0==PARA1.num && 0==PARA2.num && 0==PARA3.num && 1==PARA4.num)
            {                
                is_packet_loopback_on = TRUE;

                AUDIO_SetDevMode(AUDIO_DEVICE_MODE_EARPHONE);
                error_code = AUD_EnableVoiceLoopback(TRUE,2,usVoiceFormat,100);
                if(AUDIO_NO_ERROR == error_code)
                {
                    sprintf((char *)g_rsp_str, "+FCEPTEST:%d", PARA1.num);
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            // loop back off
            else if( 0==PARA1.num && 0==PARA2.num && 1==PARA3.num && 0==PARA4.num)
            {
                SCI_TRACE_LOW("atc: is_pcm_loopback_on=%d,is_packet_loopback_on=%d",is_pcm_loopback_on,is_packet_loopback_on);
                // stop PCM loopback
                if(is_pcm_loopback_on)
                {  
                    error_code = AUD_EnableVoiceLoopback(FALSE, 1, 0, 100);
                    SCI_TRACE_SLIGHT("ATC: disabel voice loopback errorcode=%d", error_code);
                    if(AUDIO_NO_ERROR == error_code)
                    {
                        is_pcm_loopback_on = FALSE;
                        sprintf((char *)g_rsp_str, "+FCEPTEST:%d", PARA1.num);
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                // stop packet loopback
                else if(is_packet_loopback_on)
                {
                    error_code = AUD_EnableVoiceLoopback(FALSE,2,usVoiceFormat,100);
                    if(AUDIO_NO_ERROR == error_code)
                    {
                        is_packet_loopback_on = FALSE;
                        sprintf((char *)g_rsp_str, "+FCEPTEST:%d", PARA1.num);
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                }
            }
            // connector test
            else if( 1 == PARA1.num && 0==PARA2.num && 2==PARA3.num &&0==PARA4.num)
            {
                // 3.5 pin Detect
                pin_detect = GPIO_CheckHeadsetStatus();
                if(pin_detect)
                {
                    sprintf((char *)g_rsp_str, "+FCEPTEST:%d,FOUND", PARA1.num);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+FCEPTEST:%d,NOT FOUND", PARA1.num);
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }

            break;
        }

        default:
        {
             ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+TEMPTEST command 
//                 Temperature test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessTEMPTEST)
{

    int32 temperature = 0;
    uint32 temerature_adc = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessTEMPTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG || !PARAM2_FLAG || !PARAM3_FLAG)
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }
            
            if ( 1 == PARA1.num )
            {
                if(0==PARA2.num)
                {
                    if(0==PARA3.num || 1==PARA3.num || 2==PARA3.num)
                    {
                    #ifndef WIN32
                        temperature = ADC_GetEnvTemperature();
                    #endif
                        sprintf((char*)g_rsp_str,"+TEMPTEST:%d,%d", PARA1.num, temperature);
                    }
                }
                else if(1==PARA2.num)
                {
                    if(0==PARA3.num || 1==PARA3.num || 2==PARA3.num)
                    {
                    #ifndef WIN32
                        temerature_adc = ADC_GetTransceiverADCValue();
                    #endif
                        sprintf((char*)g_rsp_str,"+TEMPTEST:%d,%d", PARA1.num, temerature_adc);
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                        break;
                    }
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                    break;
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+SPKSTEST command 
//                 Speaker test & ringtone play
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSPKSTEST)
{
    uint32 result = 0;
    AUDIO_RESULT_E dev_result = AUDIO_NO_ERROR;
    AUDIO_RESULT_E vol_result = AUDIO_NO_ERROR;
    AUDIO_RESULT_E play_result = AUDIO_NO_ERROR;
    static HAUDIO audio_handle = INVALID_HANDLE;
    CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_T tCustomPara = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessSPKSTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG ||!PARAM2_FLAG || !PARAM3_FLAG)
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }
                
            if(INVALID_HANDLE == audio_handle)
            {
                audio_handle = AUDIO_CreateNormalHandle(hCustomCodec, SCI_NULL, hLAYER1, SCI_NULL, AudioDummyNotifyCallback);
            }

            if ( 0 == PARA1.num )
            {
                switch( PARA2.num )
                {
                    case 0:
                    {
                        // stereo / mono speaker play 
                            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                        break;
                    }

                    case 1:
                    case 2:
                    {
                        // speaker paly (max volume)
                        if(0 == PARA3.num)
                        {
                            tCustomPara.duration = 100000;
                            tCustomPara.freq1 = 2000;
                            tCustomPara.freq2 = 2000;
                            tCustomPara.freq3 = 2000;
                            tCustomPara.volume = 4096;
                            tCustomPara.hAudioHandle = audio_handle;
                            dev_result = AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
                            vol_result = AUDIO_SetVolume(9);
                            play_result = AUDIO_ExeCodecExtOpe(audio_handle, ANSI2UINT16("EXT_PLAY"), CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT, &tCustomPara, &result);
                            sprintf((char *)g_rsp_str, "+SPKSTEST:%d",PARA1.num);
                        }
                        // speaker play (min volume)
                        else if(1 == PARA3.num)
                        {
                            tCustomPara.duration = 100000;
                            tCustomPara.freq1 = 2000;
                            tCustomPara.freq2 = 2000;
                            tCustomPara.freq3 = 2000;
                            tCustomPara.volume = 1228;
                            tCustomPara.hAudioHandle = audio_handle;
                            dev_result = AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
                            vol_result = AUDIO_SetVolume(1);
                            play_result = AUDIO_ExeCodecExtOpe(audio_handle, ANSI2UINT16("EXT_PLAY"), CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT, &tCustomPara, &result);
                            sprintf((char *)g_rsp_str, "+SPKSTEST:%d", PARA1.num);

                            SCI_TRACE_LOW("ATC: min volume speaker dev_result=%d,vol_result=%d,play_result=%d, result=%d",dev_result,vol_result,play_result,result);
                        }
                        break;
                    }

                    case 3:
                    {
                        // receiver play (max volume)
                        if(0 == PARA3.num)
                        {
                            tCustomPara.duration = 100000;
                            tCustomPara.freq1 = 2000;
                            tCustomPara.freq2 = 2000;
                            tCustomPara.freq3 = 2000;
                            tCustomPara.volume = 4096;
                            tCustomPara.hAudioHandle = audio_handle;
                            dev_result = AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDHOLD);
                            vol_result = AUDIO_SetVolume(9);
                            play_result = AUDIO_ExeCodecExtOpe(audio_handle, ANSI2UINT16("EXT_PLAY"), CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT, &tCustomPara, &result);
                            sprintf((char *)g_rsp_str, "+SPKSTEST:%d", PARA1.num);

                            SCI_TRACE_SLIGHT("ATC: max volume receiver dev_result=%d,vol_result=%d,play_result=%d, result=%d",dev_result,vol_result,play_result,result);
                        }
                        // receiver paly (min volume)
                        else if(1 == PARA3.num)
                        {
                            tCustomPara.duration = 100000;
                            tCustomPara.freq1 = 2000;
                            tCustomPara.freq2 = 2000;
                            tCustomPara.freq3 = 2000;
                            tCustomPara.volume = 1228;
                            tCustomPara.hAudioHandle = audio_handle;
                            dev_result = AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDHOLD);
                            vol_result = AUDIO_SetVolume(1);
                            play_result = AUDIO_ExeCodecExtOpe(audio_handle, ANSI2UINT16("EXT_PLAY"), CUSTOM_TONE_ADP_EXT_OPE_PLAY_PARA_COUNT, &tCustomPara, &result);
                            sprintf((char *)g_rsp_str, "+SPKSTEST:%d", PARA1.num);
                        }
                        break;
                    }

                    case 4:
                    {
                        // speaker off
                        if(0 == PARA3.num)
                        {
                            play_result = AUDIO_ExeCodecExtOpe(audio_handle, ANSI2UINT16("EXT_STOP"), 0, PNULL, &result);
                            sprintf((char *)g_rsp_str, "+SPKSTEST:%d", PARA1.num);
                            SCI_TRACE_SLIGHT("ATC: speaker off play_result=%d", play_result); 
                        }
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
    }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}  

/*****************************************************************************/
//  Description : This function handle the AT+KEYSHORT command 
//                 check whether key is pressed or not
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessKEYSHORT)
{
    BOOLEAN key_status = FALSE;
    
    SCI_TRACE_SLIGHT("ATC: ATC_ProcessKEYSHORT");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG || !PARAM2_FLAG)
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }
       
            if(1==PARA1.num && 0==PARA2.num)
            {
                key_status = KPD_GetKeyStatus();
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            // press
            if( key_status )
            {
                sprintf((char*)g_rsp_str,"+KEYSHORT:%d,PRESS", PARA1.num);
            }
            // realeas
            else
            {
                sprintf((char*)g_rsp_str,"+KEYSHORT:%d,RELEASE", PARA1.num);
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}    

/*****************************************************************************/
//  Description : This function handle the AT+RSTVERIF command 
//                 Factory Reset Verify
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessRSTVERIF)
{
    BOOLEAN verify_flag = FALSE;
    uint16 record_count = 0;
    BOOLEAN result = FALSE;
    MN_DUAL_SYS_E dual_sys =  ATC_CURRENT_SIM_ID_MUX;
    BOOLEAN              me_message_flag  = FALSE;
    MN_SMS_RECORD_ID_T   me_total_amount;
    MN_SMS_RECORD_ID_T   me_used_amount;
    uint16  phonebook_record     = 0;    
    
    SCI_TRACE_LOW("ATC: ATC_ProcessRSTVERIF");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(0==PARA1.num && 0==PARA2.num)
            {
                result = DMNV_ReadRestoreFactoryFlag(&verify_flag);//lint !e718 !e746

                if(result)
                {
                    if(verify_flag)
                    {
                        GetStorageInfo(dual_sys,MN_SMS_STORAGE_ME, &me_total_amount, &me_used_amount, &me_message_flag);    
                        record_count = MMIAPICL_GetRecordInfo();
                        phonebook_record = MMIAPIPB_GetPBRecordNumFromNV();

                        //GetMemoCount
                        SCI_TRACE_LOW("ATC: RSTVERIF, me_used_amount = %d, record_count = %d, phonebook_record = %d ",
                                           me_used_amount,record_count,phonebook_record);

                        if(0==me_used_amount && 0==record_count && 0==phonebook_record)
                        {
                            sprintf((char *)g_rsp_str, "+RSTVERIF:%d,OK", PARA1.num);
                        }
                        else
                        {
                            ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        }
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                else
                {
                    if(!DMNV_WriteRestoreFactoryFlag(FALSE))//lint !e718 !e746
                    {
                        SCI_TRACE_LOW("ATC: write Restore factory NV fail");
                    }

                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }
            
            break;
        }
    
        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the AT+IMEITEST command 
//                 IMEI read write 
//  Global resource dependence : none
//  Author:longting zhao
//  Note: 
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessIMEITEST)
{
    int  i                                                           = 0;
    char  imei_str_tmp                                       = 0;
    uint8  temp_str[20]                                      = {0};
    uint8  imei[40]                                             ={0};
    uint8  *param_ptr                                       = NULL;
    uint8  param_len                                         = 0;
    uint8  imei_len                                             = 0;
    uint8  cmd_type                                          = 0;
    uint8  imei_checkdigit                                  = 0;
    MN_IMEI_T  imei_arr                                    = {0};
    NV_ITEM_ID_E  nv_imei_id                           = NV_IMEI;
    MN_DUAL_SYS_E  dual_sys                           =  ATC_CURRENT_SIM_ID_MUX;
    NVITEM_ERROR_E  ret                                  = NVERR_NONE;
    ATC_IMEI_OPERATION_TYPE operation_type  = ATC_OPERATION_INVALID;
    ATC_PRODUCTION_LINE_PARAM_T  param[3]  = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessIMEITEST");

    SCI_MEMSET(param, 0x0, sizeof(param));
    
    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    param_ptr++;
    param_len--;

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (!ATC_GetParamsEx(param_ptr, param_len, param))
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }

            if(0 == param_len)
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                break;
            }

            operation_type = (ATC_IMEI_OPERATION_TYPE)(param[0].param[0] - '0');
            switch(operation_type)
            {
                // read IMEI
                case ATC_READ_IMEI:
                {
                    if('0' == param[1].param[0])
                    {
                        dual_sys = MN_DUAL_SYS_1;
                    }
                    else if('2' == param[1].param[0])
                    {
                      #ifdef MULTI_SIM_SYS_DUAL
                        dual_sys = MN_DUAL_SYS_2;
                      #endif
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                        break;
                    }

                    if(MNNV_GetIMEIEx(dual_sys, imei_arr))
                    {
                        ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_str);
                        sprintf((char*)g_rsp_str, "+IMEITEST:%c,%s",*(param_ptr), temp_str);
                    }
                    else
                    {
                        SCI_TRACE_LOW("ATC: read SIM%d IMEI fail", dual_sys);
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    }

                    break;
                }

                // write IMEI
                case ATC_WRITE_IMEI_1:
                case ATC_WRITE_IMEI_2:
                {
                    if(ATC_WRITE_IMEI_1 == operation_type)
                    {
                        dual_sys = MN_DUAL_SYS_1;
                    }
                    else
                    {
                    #ifdef MULTI_SIM_SYS_DUAL
                        dual_sys = MN_DUAL_SYS_2;
                    #endif
                    }

                    imei_len = param[1].param_len;                    
                    SCI_TRACE_LOW("ATC: msl_verified=%d, ak_seed_no_verified=%d", msl_verified, ak_seed_no_verified);
 
                    if (ATC_IMEI_LENGTH != imei_len)
                    {
                        SCI_TRACE_LOW("ATC: IMEI length incorrect");
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        break;
                    }
                   
                    if( !msl_verified )
                    {
                        sprintf((char *)g_rsp_str, "+IMEITEST:NG NEEDMSLAUTH");
                        break;
                    }

                    if(!ak_seed_no_verified)
                    {
                        sprintf((char *)g_rsp_str, "+IMEITEST:NG NEEDAKSEEDAUTH");
                        break;
                    }

                    // calculate check digit
                    imei_checkdigit = ATC_GetIMEICheckDigit(param[1].param);

                    SCI_TRACE_SLIGHT("ATC: checkdigit=%d",imei_checkdigit);
                    
                    if(imei_checkdigit != (param[1].param[14]-'0'))
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        break;
                    }
                        
                    // switch each byte from [1]
                    for(i = 1; i < param[1].param_len - 1; i += 2)
                    {
                        imei_str_tmp = param[1].param[i];
                        param[1].param[i] = param[1].param[i+1];
                        param[1].param[i+1] = imei_str_tmp;
                    }

                    temp_str[0] = param[1].param[0];
                    temp_str[1] = 'A';
                    
                    SCI_MEMCPY(temp_str+2, param[1].param+1, param[1].param_len-1);

                    if(!ConvertHexToBin(temp_str, strlen((char *)temp_str), imei) )
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        break;
                    }
                    
                    nv_imei_id = (NV_ITEM_ID_E)MNNV_GetImeiId(dual_sys);//lint !e718 !e746
                    if(NV_IMEI==nv_imei_id || NV_IMEI1==nv_imei_id)
                    {
                    #ifndef WIN32
                        ret = NVITEM_UpdateCali(nv_imei_id, MN_MAX_IMEI_LENGTH, INPUT imei);
                    #endif
                        if(NVERR_NONE != ret)
                        {
                            SCI_TRACE_LOW("ATC: ret=%d", ret);
                            ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                            break;
                        }

                        sprintf((char*)g_rsp_str, "+IMEITEST:%c,OK",*(param_ptr));
                    }
                    else
                    {
                        SCI_TRACE_LOW("ATC: IMEI NV error");
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                         
                    break;
                }

                default:
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }
            }
            
            break;
        }
        
        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+GETTESTNV command 
//                 Get Test status in NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/

AT_CMD_FUNC(ATC_ProcessGETTESTNV)
{
    uint16          test_nv_index     = 0;
    ATC_FACTORY_ITEM_T          test_nv_buf[41] = {0};
    uint8            test_nv_id    = 0;
    uint8            test_nv_result_code = 0;
    ATC_FACTORY_ITEM_RSP_T        str_tmp = {0};
    BOOLEAN b_need_set_default_value = FALSE;

    SCI_TRACE_LOW("ATC: ATC_ProcessGETTESTNV");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            test_nv_index = PARA1.num -1;
            
            if(PARA1.num<1 || PARA1.num>40)
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                break;
            }

            if(DMNV_ReadTestNV((uint16 *)test_nv_buf))//lint !e718 !e746
            {
                if(0==test_nv_buf[test_nv_index].result_code )
                {
                    b_need_set_default_value = TRUE;
                }
                else
                {
                    test_nv_id = test_nv_buf[test_nv_index].id;
                    test_nv_result_code= test_nv_buf[test_nv_index].result_code;
                }
            }
            else
            {
                SCI_TRACE_SLIGHT("ATC:read NV fail, set default value");
                b_need_set_default_value = TRUE;
            }

            if (b_need_set_default_value)
            {
                SCI_TRACE_SLIGHT("ATC: set test NV default value");
                test_nv_id = PARA1.num;
                test_nv_result_code= 'N';
            }


            if(test_nv_id>=1 && test_nv_id<=9)
            {
                sprintf((char *)&(str_tmp.id[0]), "%02d", test_nv_id);
            }
            else if(test_nv_id>=10 && test_nv_id<=40)
            {
                sprintf((char *)&(str_tmp.id[0]), "%d", test_nv_id);
            }

            //sprintf((char *)&(str_tmp.result_code), "%c", test_nv_result_code);
            str_tmp.result_code = test_nv_result_code;
            sprintf((char *)g_rsp_str, "+GETTESTNV=%.2s,%c", str_tmp.id, str_tmp.result_code);

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}  

/*****************************************************************************/
//  Description : This function handle the AT+SETTESTNV command 
//                 Set test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          
AT_CMD_FUNC(ATC_ProcessSETTESTNV)
{
    uint8            *param_ptr   = NULL;
    uint8            param_len    = 0;
    uint8            cmd_type     = 0;
    ATC_PRODUCTION_LINE_PARAM_T parameter[2];
    ATC_FACTORY_ITEM_T        test_nv_buf[41] = {0};
    ATC_FACTORY_ITEM_T        hist_nv;
    uint16          test_nv_index     = 0;
    
    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;

    SCI_TRACE_LOW("ATC: ATC_ProcessSETTESTNV");
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);    
    SCI_TRACE_SLIGHT("ATC: para1.str=%s, cmd_type=%d", param_ptr, cmd_type);

    param_ptr++;
    param_len--;
   
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (ATC_GetParamsEx(param_ptr, param_len, parameter))
            {               
                if(strlen((char *)parameter[0].param) == 1)
                {
                    test_nv_index = (parameter[0].param[0]-'0')-1;
                }
                else if((strlen((char *)parameter[0].param) == 2))
                {
                    test_nv_index = (parameter[0].param[0]-'0')*10+ ((parameter[0].param[1]-'0'))-1;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }

                if((strlen((char *)parameter[1].param) != 1))
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                }

                if(test_nv_index>=40)
                {
                    SCI_TRACE_SLIGHT("ATC: wrong index");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }

                if(DMNV_ReadTestNV((uint16 *)test_nv_buf))
                {
                        SCI_TRACE_SLIGHT("ATC: read NV success");
                }
                else
                {
                    SCI_TRACE_SLIGHT("ATC: read NV fail");
                }

                test_nv_buf[test_nv_index].id = test_nv_index+1;

                SCI_MEMCPY(&test_nv_buf[test_nv_index].result_code, parameter[1].param, 1);
                if(DMNV_WriteTestNV((uint16 *)test_nv_buf))//lint !e718 !e746
                {
                    if(test_nv_index < 9)
                    {
                        sprintf((char *)g_rsp_str, "+SETTESTNV=%02d,%s", test_nv_index+1, parameter[1].param);
                    }
                    else if(test_nv_index>=9 && test_nv_index<40)
                    {
                        sprintf((char *)g_rsp_str, "+SETTESTNV=%d,%s", test_nv_index+1, parameter[1].param);
                    }

                    hist_nv.id = test_nv_buf[test_nv_index].id;
                    hist_nv.result_code = test_nv_buf[test_nv_index].result_code;

                    ATC_RecordHistoryNV(hist_nv);
                }
                else     
                {
                    SCI_TRACE_SLIGHT("ATC: write test nv fail");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }
            }
            else
            {
                SCI_TRACE_SLIGHT("ATC: parse settestnv para fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+GETFULLHISTNV command 
//                 Get all history test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          

AT_CMD_FUNC(ATC_ProcessGETFULLHISTNV)
{
    ATC_FACTORY_ITEM_T            hist_nv_buf[61] = {0};
    ATC_FACTORY_ITEM_RSP_T     str_tmp[60]       = {0};
    uint8                                     i                       =0;
    uint8                                     tmp_id              = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessGETFULLHISTNV");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if(!DMNV_ReadHistNV((uint16 *)hist_nv_buf))//lint !e718 !e746
            {
                SCI_TRACE_SLIGHT("ATC: first time read hist nv , fail");
                SCI_MEMSET(hist_nv_buf, 0xff, sizeof(hist_nv_buf));

                if(!DMNV_WriteHistNV((uint16 *)hist_nv_buf))//lint !e718 !e746
                {
                    SCI_TRACE_SLIGHT("ATC: write history NV fail");
                    ATC_RESPONSE_MSG(atc_config_ptr, "NG");
                    break;
                }
            }
            
            for(i=0; hist_nv_buf[i].result_code!=0xff&& i<60; i++)
            {
                tmp_id = hist_nv_buf[i].id & 0x7f;

                if(tmp_id>=1 && tmp_id<=9)
                {
                    sprintf((char *)&(str_tmp[i].id[0]), "%02d", tmp_id);
                }
                else if(tmp_id>=10 && tmp_id<=40)
                {
                    sprintf((char *)&(str_tmp[i].id[0]), "%d", tmp_id);
                }
                
                sprintf((char *)&(str_tmp[i].result_code), "%c", hist_nv_buf[i].result_code);
            }

            sprintf((char *)g_rsp_str, "+GETFULLHISTNV\r\n%s", (char *)str_tmp);

             SCI_TRACE_SLIGHT("ATC: str_tmp=%s",str_tmp);
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}
/*****************************************************************************/
//  Description : This function handle the AT+LOGERASE command 
//                 Erase history test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/          
AT_CMD_FUNC(ATC_ProcessLOGERASE)
{
    ATC_FACTORY_ITEM_T        test_nv_buf[60] = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessLOGERASE");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            SCI_MEMSET(test_nv_buf, 0xff, sizeof(test_nv_buf));
            if(!DMNV_WriteHistNV((uint16 *)test_nv_buf))
            {
                SCI_TRACE_SLIGHT("ATC: write his NV fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }

            sprintf((char *)g_rsp_str, "+LOGERASE:OK");

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

  
/*****************************************************************************/
//  Description : This function handle the AT+GETFULLTESTNV command 
//                 Get full test NV value
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFULLTESTNV)
{
    ATC_FACTORY_ITEM_T        test_nv_buf[41] = {0};
    ATC_FACTORY_ITEM_RSP_T        str_tmp[40] = {0};
    uint8 i =0;
   
    SCI_TRACE_LOW("ATC: ATC_ProcessGETFULLTESTNV");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if(!DMNV_ReadTestNV((uint16 *)test_nv_buf))
            {
                SCI_MEMSET(test_nv_buf, 'N', sizeof(test_nv_buf));

                for(i=0;  i<40; i++)
                {
                    test_nv_buf[i].id = i+1;
                }
                
                if(!DMNV_WriteTestNV((uint16 *)test_nv_buf))
                {
                    SCI_TRACE_SLIGHT("ATC: wrtite test NV fail");
                }
            }
           
            for(i=0;  i<40; i++)
            {
                if(i<10)
                {
                    sprintf((char *)&(str_tmp[i].id[0]), "%02d", i+1);
                }
                else
                {
                    sprintf((char *)&(str_tmp[i].id[0]), "%d", i+1);
                }

                if('N' != test_nv_buf[i].result_code 
                    && 'P' != test_nv_buf[i].result_code 
                    && 'F' != test_nv_buf[i].result_code )
                {
                    sprintf((char *)&(str_tmp[i].result_code), "%c", 'N');
                }
                else
                {
                    sprintf((char *)&(str_tmp[i].result_code), "%c", test_nv_buf[i].result_code);
                }
            }

            sprintf((char *)g_rsp_str, "+GETFULLTESTNV\r\n%s", (char *)str_tmp);

             SCI_TRACE_SLIGHT("ATC: str_tmp=%s",str_tmp);
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+LOOPTEST command 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessLOOPTEST)
{
    uint16                                     usVoiceFormat = 1;//app determine the voice format.
    MN_DUAL_SYS_E                               dual_sys = ATC_CURRENT_SIM_ID_MUX;
    MN_PHONE_CURRENT_PLMN_INFO_T   plmn_info;
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessLOOPTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (0 == PARA1.num && 0==PARA2.num)
            {
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

                {

                    for(i=MN_DUAL_SYS_1; i<MN_DUAL_SYS_MAX; i++)
                    {
                        MNPHONE_PowerOffPsEx(i); 
                    }
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN);

                    loop_test_info.mode = PARA3.num;
                    loop_test_info.voice_format = usVoiceFormat;
                    g_atc_power_info.cmd_id = AT_CMD_LOOPTEST;

                    return S_ATC_DEFAULT_HANDLE;
                }
            }
            else if(1 == PARA2.num)
            {
                // MIC - SPK Loopback Off
                if(0==PARA3.num)
                {
                    g_atc_power_info.cmd_id = AT_CMD_LOOPTEST;
                    SCI_TRACE_LOW("atc: cmd_id=%d", g_atc_power_info.cmd_id);
                    {

                        for(i=MN_DUAL_SYS_1; i<MN_DUAL_SYS_MAX; i++)
                        {
                            MNPHONE_PowerOnPsEx(i); 
                        }
                    }
                    
                    AUD_EnableVoiceLoopback(FALSE,2,usVoiceFormat,100);
                    sprintf((char *)g_rsp_str, "+LOOPTEST:%d", PARA1.num);

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    return S_ATC_SUCCESS;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
//  Description : This function handle the AT+BATTTEST command 
//                    battery status test
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessBATTTEST)
{
    CHGMNG_STATE_INFO_T *p_chgmng_info = NULL;
    char str_tmp[14]        = {0};
    uint32 jigo_status = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessBATTTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
		   #ifdef USB_SWITCH_SUPPORT
            jigo_status = ADC_GetJIGConnectStatus();
		   #endif
            SCI_TRACE_LOW("ATC: jigo status=%d", jigo_status);
            p_chgmng_info = CHGMNG_GetModuleState();
            
            // read status
            if(1==PARA1.num && 1==PARA2.num)
            {
                if(1 == jigo_status)
                {
                    sprintf((char *)g_rsp_str, "+BATTTEST:%d,JIGO",PARA1.num);
                }                
                else if(CHGMNG_STARTING==p_chgmng_info->chgmng_state
                          ||CHGMNG_CHARGING==p_chgmng_info->chgmng_state
                          ||CHGMNG_PULSECHARGING==p_chgmng_info->chgmng_state)
                {
                    sprintf((char *)g_rsp_str, "+BATTTEST:%d,CHAR",PARA1.num);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+BATTTEST:%d,BAOK",PARA1.num);
                }
            }
            // voltage
            else if(1==PARA1.num && 2==PARA2.num)
            {
                sprintf(str_tmp, "%d.%d", (p_chgmng_info->bat_cur_vol)/1000, (p_chgmng_info->bat_cur_vol)%1000/10);
                sprintf((char *)g_rsp_str, "+BATTTEST:%d,%s", PARA1.num, str_tmp);
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
//  Description : This function handle the AT+MAXPOWER command
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessMAXPOWER )
{
    uint16            band = 0;
    uint16            level = 0;
    uint8              op_code = 0;
    uint8              err_type = 0;  //0: no error(SUCCESS); 1: parameter invalid(NV); 2: work error(NG)
    BOOLEAN       is_power_on = FALSE;
    uint16            gainorpower = 0;
    uint8              pwr_db = 0;
    uint8              ms_band = ATC_EGSM900;
    uint16            arfcn = 25;
    MN_DUAL_SYS_E  i  = 0;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
            {
                op_code = PARA1.num;
                band = PARA2.num;
                level = PARA3.num;
            }
            else
            {
                err_type = 1;
                break;
            }

            // check Max Power Tx On or Off
            if ((band <= 3) && ((0 == level)|| (1 == level)))
            {
                is_power_on = TRUE;
            }
            else if((9 == band) && (9 == level))
            {
                is_power_on = FALSE;
            }
            else
            {
                is_power_on = FALSE;
                err_type = 1;
                break;
            }

            if (is_power_on)
            {
                switch (band)
                {
                    case 0:
                        ms_band = ATC_EGSM900;
                        arfcn = 25;
                        break;

                    case 1:
                        ms_band = ATC_DCS1800;
                        arfcn = 600;
                        break;

                    case 2:
                        ms_band = ATC_PCS1900;
                        arfcn = 600;
                        break;

                    case 3:
                        ms_band = ATC_GSM850;
                        arfcn = 128;
                        break;

                     default :
                        break;
                }

                pwr_db = level?10:0;
                gainorpower = L1API_GetGsmTestOutputPowerLev((uint32)ms_band, pwr_db);

                if ((g_atc_power_info.cmd_id != 0) && (g_atc_power_info.cmd_id != AT_CMD_MAXPOWER))
                {
                     SCI_TRACE_LOW("ATC: ATC_ProcessMAXPOWER,  AT+MAXPOWER excludes AT+READRSSI ongoing");
                     err_type = 2;
                     break;
                }

                //store power information for starting power transmission after ps off
                g_atc_power_info.cmd_id = AT_CMD_MAXPOWER;
                g_atc_power_info.arfcn = arfcn;
                g_atc_power_info.ms_band = ms_band;
                g_atc_power_info.gainpower = gainorpower;

                for(i=MN_DUAL_SYS_1; i<MN_DUAL_SYS_MAX; i++)
                {
                    if(MNPHONE_IsPsPowerOnEx(i))
                    {
                        g_atc_power_info.sim_insert[i] = TRUE;
                    }

                    SCI_TRACE_LOW("ATC: MNPHONE_PowerOffPsEx!, sim id=%d", i);

                    MNPHONE_PowerOffPsEx(i); 
                }

                err_type = 3;
            }//if (is_power_on)        
            else
            {
                // first close layer1 transmission, then power on PS
                L1API_GsmTxPwrTest(is_power_on, arfcn, ms_band, gainorpower);
                
                for(i=MN_DUAL_SYS_1; i<MN_DUAL_SYS_MAX; i++)
                {
                    SCI_TRACE_LOW("ATC: sim_insert[%d]=%d", i, (g_atc_power_info.sim_insert[i]));
                    if(g_atc_power_info.sim_insert[i])
                    {
                        MNPHONE_PowerOnPsEx(i);
                    }
                }
            }

            break;

        default:
            err_type = 2;
            break;
    }

    if(err_type == 3)
    {
        return S_ATC_DEFAULT_HANDLE;
    }
    else if (err_type == 0)
    {
        sprintf((char *)g_rsp_str, "%s: 0", g_atc_info_table[AT_CMD_MAXPOWER].cmd_name->str_ptr);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    else if (err_type == 1)
    {
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
    }
    else
    {
        ATC_RESPONSE_MSG(atc_config_ptr, "NG");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the AT+READRSSI command
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessREADRSSI )
{
    uint16            band = 0;
    uint16            level = 0;
    uint8              op_code = 0;
    uint8              ms_band = ATC_EGSM900;
    uint16            arfcn = 25;
    MN_DUAL_SYS_E    dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
            {
                op_code = PARA1.num;
                band = PARA2.num;
                level = PARA3.num;
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }

            if ((op_code > 0) || (band >= ATC_MAX_BAND_NUM))
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                break;
            }

            switch (band)
            {
                case 0:
                    ms_band = ATC_EGSM900;
                    arfcn = 25;
                    break;

                case 1:
                    ms_band = ATC_DCS1800;
                    arfcn = 600;
                    break;

                case 2:
                    ms_band = ATC_PCS1900;
                    arfcn = 600;
                    break;

                case 3:
                    ms_band = ATC_GSM850;
                    arfcn = 128;
                    break;

                 default :
                    break;
            }

            if(TRUE == MNPHONE_IsPsPowerOnEx(dual_sys))
            {
                g_is_ps_act[dual_sys]   = TRUE;
                g_is_ps_deactivate[dual_sys] = FALSE;
            }
            else
            {
                g_is_ps_act[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;
            }

            if ((g_atc_power_info.cmd_id != 0) && (g_atc_power_info.cmd_id != AT_CMD_READRSSI))
            {
                 SCI_TRACE_LOW("ATC: ATC_ProcessREADRSSI,  AT+READSSI excludes AT+MAXPOWER ongoing");
                 ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                 break;
            }

        SCI_KILL_TIMER(P_ATC, ATC_RSSI_READ_EXP_IND,
                                      &T_ATC_RSSI_READ_EXP_IND,
                                      "T_ATC_RSSI_READ_EXP_IND" );

            g_atc_power_info.cmd_id = AT_CMD_READRSSI;
            g_atc_power_info.arfcn = arfcn;
            g_atc_power_info.ms_band = ms_band;
            g_atc_power_info.dual_sys = dual_sys;

#ifdef _MUX_ENABLE_
            s_atc_rssi_link_id = act_config_ptr->current_link_id;
#endif

            if (g_is_ps_act[dual_sys])
            {
                g_atc_power_info.is_ps_active= TRUE;
                
                MNPHONE_PowerOffPsEx(dual_sys); 
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,(uint16)APP_MN_PS_POWER_OFF_CNF, 0xff, ATC_NO_DOMAIN);
                
                g_is_ps_act[dual_sys]   = FALSE;
                g_is_ps_deactivate[dual_sys] = TRUE;

                return S_ATC_DEFAULT_HANDLE;
            }
            else
            {
                SCI_TRACE_LOW("ATC: ATC_ProcessREADRSSI,  is_power_on%d, arfcn=%d, band=%d", 1, arfcn, ms_band);
                L1API_GsmRXPwrTest(TRUE, arfcn, ms_band);
                g_atc_read_rssi_times = 12;
                SCI_SET_TIMER(P_ATC, ATC_RSSI_READ_INTERVAL,
                                            ATC_RSSI_READ_EXP_IND,
                                            &T_ATC_RSSI_READ_EXP_IND, 
                                            "T_ATC_RSSI_READ_EXP_IND");
                return S_ATC_DEFAULT_HANDLE;
            }
        }
        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
    }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+DETALOCK command 
//                 Detail Lock Setting/Getting
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessDETALOCK)
{
    #define NV_SIM_LOCK_DETAL_DATA_ID 498
    MN_SIM_LOCK_DETAL_DATA_T * simlock_detal_data_ptr = SCI_NULL;
    MN_SIM_LOCK_DETAL_DATA_T * encrypt_detal_data_ptr = SCI_NULL;
    ATC_PRODUCTION_LINE_PARAM_T param_read[2] = {0};
    ATC_PRODUCTION_LINE_PARAM_T param_write[7] = {0};
    uint8 mck[9] ={0};
    uint8 nck[9] ={0};
    uint8 sck[9] ={0};
    uint8 spck[9] ={0};
    uint8 cpck[9] ={0};
    uint8 pck[9] ={0};
    uint8  *param_ptr   = NULL;
    uint8  param_len    = 0;
    uint8  cmd_type     = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessDETALOCK");

    simlock_detal_data_ptr = (MN_SIM_LOCK_DETAL_DATA_T *)SCI_ALLOC_BASEZ(sizeof(MN_SIM_LOCK_DETAL_DATA_T));
    encrypt_detal_data_ptr = (MN_SIM_LOCK_DETAL_DATA_T *)SCI_ALLOC_BASEZ(sizeof(MN_SIM_LOCK_DETAL_DATA_T));
    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    SCI_TRACE_SLIGHT("ATC: para1.str=%s, cmd_type=%d", param_ptr, cmd_type);

    param_ptr++; //ignore '='
    param_len--;
    
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if( !msl_verified || !ak_seed_no_verified)
            {
                SCI_TRACE_SLIGHT("ATC: msl or sk not verified");
                sprintf((char *)g_rsp_str, "+DETALOCK:NG");
                break;
            }

            //read detal lock
            if('1' == param_ptr[0])
            {
                if(FALSE == ATC_GetMultiParamsEx(param_ptr, param_len, param_read))
                {
                    SCI_TRACE_SLIGHT("ATC: invalid parameter");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }

                if('1' == param_read[0].param[0] && '0'==param_read[1].param[0])
                {
              
                    if(! DMNV_ReadDetaLock(simlock_detal_data_ptr))//lint !e718 !e746
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        break;
                    }		

                    sprintf((char*)mck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->mck[0],simlock_detal_data_ptr->mck[1],simlock_detal_data_ptr->mck[2],simlock_detal_data_ptr->mck[3],simlock_detal_data_ptr->mck[4],simlock_detal_data_ptr->mck[5],simlock_detal_data_ptr->mck[6],simlock_detal_data_ptr->mck[7]);
                    sprintf((char*)nck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->nck[0],simlock_detal_data_ptr->nck[1],simlock_detal_data_ptr->nck[2],simlock_detal_data_ptr->nck[3],simlock_detal_data_ptr->nck[4],simlock_detal_data_ptr->nck[5],simlock_detal_data_ptr->nck[6],simlock_detal_data_ptr->nck[7]);
                    sprintf((char*)sck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->sck[0],simlock_detal_data_ptr->sck[1],simlock_detal_data_ptr->sck[2],simlock_detal_data_ptr->sck[3],simlock_detal_data_ptr->sck[4],simlock_detal_data_ptr->sck[5],simlock_detal_data_ptr->sck[6],simlock_detal_data_ptr->sck[7]);
                    sprintf((char*)spck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->spck[0],simlock_detal_data_ptr->spck[1],simlock_detal_data_ptr->spck[2],simlock_detal_data_ptr->spck[3],simlock_detal_data_ptr->spck[4],simlock_detal_data_ptr->spck[5],simlock_detal_data_ptr->spck[6],simlock_detal_data_ptr->spck[7]);
                    sprintf((char*)cpck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->cpck[0],simlock_detal_data_ptr->cpck[1],simlock_detal_data_ptr->cpck[2],simlock_detal_data_ptr->cpck[3],simlock_detal_data_ptr->cpck[4],simlock_detal_data_ptr->cpck[5],simlock_detal_data_ptr->cpck[6],simlock_detal_data_ptr->cpck[7]);
                    sprintf((char*)pck,"%d%d%d%d%d%d%d%d",simlock_detal_data_ptr->pck[0],simlock_detal_data_ptr->pck[1],simlock_detal_data_ptr->pck[2],simlock_detal_data_ptr->pck[3],simlock_detal_data_ptr->pck[4],simlock_detal_data_ptr->pck[5],simlock_detal_data_ptr->pck[6],simlock_detal_data_ptr->pck[7]);
                    sprintf((char*)g_rsp_str, "+DETALOCK:%c,%s,%s,%s,%s,%s,%s",*(param_ptr), mck,nck,sck,spck,cpck,pck);
                    break;
                }
                else
                {
                    SCI_TRACE_SLIGHT("ATC: invalid parameter");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }
            }
            // write detal lock
            else if( '2' == param_ptr[0])
            {
                SCI_MEMSET((void *)param_write,0,7*sizeof(ATC_PRODUCTION_LINE_PARAM_T));
            
                if(FALSE == ATC_GetMultiParamsEx(param_ptr, param_len, param_write))
                {
                    SCI_TRACE_SLIGHT("ATC: invalid parameter");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    break;
                }
                
                if( !key_check(param_write[1].param, param_write[1].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,MASTER FAIL", param_write[0].param[0]);
                    break;
                }
                else if( !key_check(param_write[2].param, param_write[2].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,NWLOCK FAIL", param_write[0].param[0]);
                    break;
                }
                else if( !key_check(param_write[3].param, param_write[3].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,SUBSETLOCK FAIL", param_write[0].param[0]);
                    break;
                }
                else if( !key_check(param_write[4].param, param_write[4].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,SPLOCK FAIL", param_write[0].param[0]);
                    break;
                }
                else if( !key_check(param_write[5].param, param_write[5].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,CPLOCK FAIL", param_write[0].param[0]);
                    break;
                }
                else if( !key_check(param_write[6].param, param_write[6].param_len))
                {
                    sprintf((char *)g_rsp_str,"+DETALOCK:%c,SIMLOCK FAIL", param_write[0].param[0]);
                    break;
                }
                else
                {
                    uint8 i = 0;

                    for(i = 0; i < 8; i++)
                    {
                        simlock_detal_data_ptr->mck[i] = param_write[1].param[i]-'0';
                        simlock_detal_data_ptr->nck[i] = param_write[2].param[i]-'0';
                        simlock_detal_data_ptr->sck[i] = param_write[3].param[i]-'0';
                        simlock_detal_data_ptr->spck[i] = param_write[4].param[i]-'0';
                        simlock_detal_data_ptr->cpck[i] = param_write[5].param[i]-'0';
                        simlock_detal_data_ptr->pck[i] = param_write[6].param[i]-'0'; 
                    }

                   
                    Atc_SysCrHashSHA1(  simlock_detal_data_ptr->mck, encrypt_detal_data_ptr->mck  );
        			Atc_SysCrHashSHA1(  simlock_detal_data_ptr->nck, encrypt_detal_data_ptr->nck  );
        			Atc_SysCrHashSHA1(  simlock_detal_data_ptr->sck, encrypt_detal_data_ptr->sck  );
        			Atc_SysCrHashSHA1(  simlock_detal_data_ptr->spck, encrypt_detal_data_ptr->spck  );
        			Atc_SysCrHashSHA1(  simlock_detal_data_ptr->cpck, encrypt_detal_data_ptr->cpck  );
        			Atc_SysCrHashSHA1(  simlock_detal_data_ptr->pck, encrypt_detal_data_ptr->pck  );

                    
                    if(! DMNV_WriteDetaLock(encrypt_detal_data_ptr))//lint !e718 !e746
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                        break;
                    }

                    sprintf((char *)g_rsp_str, "+DETALOCK:2,OK");
                    break;
                }                
            }
            else
            {
                SCI_TRACE_SLIGHT("ATC: invalid parameter");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                break;
            }
        }
        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    if(NULL != simlock_detal_data_ptr)
    {
        SCI_FREE(simlock_detal_data_ptr);
    }
    
    if(NULL != encrypt_detal_data_ptr)
    {
        SCI_FREE(encrypt_detal_data_ptr);
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the AT+LOCKREAD command
//  Global resource dependence : g_rsp_str
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessLOCKREAD)
{
    MN_SIM_LOCK_CUSTOMIZE_DATA_T * simlock_customize_data_ptr = SCI_NULL;
    NETWORK_SUBSET_LOCKS_T     network_subset_locks;
    CORPORATE_LOCKS_T          corporate_locks;
    NETWORK_LOCKS_T            network_locks;
    SP_LOCKS_T                 sp_locks;
    uint32                           user_lock_status;

    SCI_TRACE_LOW("ATC: ATC_ProcessLOCKREAD");
    
    if(!PARAM1_FLAG || !PARAM2_FLAG)
    {
        SCI_TRACE_LOW("ATC: invalid parameter");
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        return S_ATC_SUCCESS; 
    }

    if ( 1 != PARA1.num )
    {
        SCI_TRACE_LOW("ATC: invalid parameter");
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        return S_ATC_SUCCESS; 
    }
       
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            simlock_customize_data_ptr = (MN_SIM_LOCK_CUSTOMIZE_DATA_T *)SCI_ALLOC_BASEZ(sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T));
            if(NVERR_NONE != EFS_NvitemRead(ATC_NV_SIM_LOCK_CUSTOMIZE_DATA_ID,
                                    sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T),
                                    (void *)simlock_customize_data_ptr))
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }

            user_lock_status = simlock_customize_data_ptr->SIM_lock_status;
            SCI_TRACE_SLIGHT("ATC: user_lock_status=%d", user_lock_status);

            if(0 == PARA2.num)
            {
                network_locks = simlock_customize_data_ptr->network_locks;

                if((0 == (user_lock_status & MN_SIM_NETWORK_LOCK)) ||
                   (0 == network_locks.numLocks))
                {
                     sprintf((char *)g_rsp_str,"+LOCKREAD:%d,NONE", PARA1.num);
                     break;
                }
                else if(1 == network_locks.numLocks)
                {
                    if(2 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d%02d",PARA1.num,network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }
                    else if(3 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d%03d",PARA1.num,network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }

                    break;
                }
                else if(2 == network_locks.numLocks)
                {
                    uint8 str_temp1[32] ={0};
                    uint8 str_temp2[32] ={0};

                    if(2 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)str_temp1,"%d%02d",network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }
                    else if(3 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)str_temp1,"%d%03d",network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }

                    if(2 == network_locks.locks[1].mnc_digit_num)
                    {
                        sprintf((char *)str_temp2,"%d%02d",network_locks.locks[1].mcc,network_locks.locks[1].mnc);
                    }
                    else if(3 == network_locks.locks[1].mnc_digit_num)
                    {
                        sprintf((char *)str_temp2,"%d%03d",network_locks.locks[1].mcc,network_locks.locks[1].mnc);
                    }
                    
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%s_%s",PARA1.num,str_temp1,str_temp2);
                    break;
                }
                else if(3 == network_locks.numLocks)
                {
                    uint8 str_temp1[32] ={0};
                    uint8 str_temp2[32] ={0};
                    uint8 str_temp3[32] ={0};

                    if(2 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)str_temp1,"%d%02d",network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }
                    else if(3 == network_locks.locks[0].mnc_digit_num)
                    {
                        sprintf((char *)str_temp1,"%d%03d",network_locks.locks[0].mcc,network_locks.locks[0].mnc);
                    }

                    if(2 == network_locks.locks[1].mnc_digit_num)
                    {
                        sprintf((char *)str_temp2,"%d%02d",network_locks.locks[1].mcc,network_locks.locks[1].mnc);
                    }
                    else if(3 == network_locks.locks[1].mnc_digit_num)
                    {
                        sprintf((char *)str_temp2,"%d%03d",network_locks.locks[1].mcc,network_locks.locks[1].mnc);
                    }

                    if(2 == network_locks.locks[2].mnc_digit_num)
                    {
                        sprintf((char *)str_temp3,"%d%02d",network_locks.locks[2].mcc,network_locks.locks[2].mnc);
                    }
                    else if(3 == network_locks.locks[2].mnc_digit_num)
                    {
                        sprintf((char *)str_temp3,"%d%03d",network_locks.locks[2].mcc,network_locks.locks[2].mnc);
                    }
                    
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%s_%s_%s",PARA1.num,str_temp1,str_temp2,str_temp3);
                    break;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else if(1 == PARA2.num)
            {
                network_subset_locks = simlock_customize_data_ptr->network_subset_locks;
 
                if((0 == (user_lock_status & MN_SIM_NETWORK_SUBSET_LOCK)) ||
                   (0 == network_subset_locks.numLocks))
                {
                     sprintf((char *)g_rsp_str,"+LOCKREAD:%d,NONE", PARA1.num);
                     break;
                }
                else if(1 == network_subset_locks.numLocks)
                {
                    sprintf((char*)g_rsp_str,"+LOCKREAD:%d,%d%d",PARA1.num,network_subset_locks.locks[0].network_subset[0],
                        network_subset_locks.locks[0].network_subset[1]);
                    break;
                }
                else if(2 == network_subset_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d%d_%d%d",PARA1.num,network_subset_locks.locks[0].network_subset[0],
                        network_subset_locks.locks[0].network_subset[1],network_subset_locks.locks[1].network_subset[0],network_subset_locks.locks[1].network_subset[1]);
                    break;
                }
                else if(3 == network_subset_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d%d_%d%d_%d%d",PARA1.num,network_subset_locks.locks[0].network_subset[0],
                        network_subset_locks.locks[0].network_subset[1],network_subset_locks.locks[1].network_subset[0],network_subset_locks.locks[1].network_subset[1],
                        network_subset_locks.locks[2].network_subset[0],network_subset_locks.locks[2].network_subset[1]);
                    break;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else if(2 == PARA2.num)
            {
                sp_locks = simlock_customize_data_ptr->SP_locks;
 
                if((0 == (user_lock_status & MN_SIM_SP_LOCK)) ||
                   (0 == sp_locks.numLocks))
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,NONE", PARA1.num);
                    break;
                }
                else if(1 == sp_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d",PARA1.num,sp_locks.locks[0].sp);
                    break;
                }
                else if(2 == sp_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d_%d",PARA1.num,sp_locks.locks[0].sp,
                                sp_locks.locks[1].sp);
                    break;
                }
                else if(3 == sp_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d_%d_%d",PARA1.num,sp_locks.locks[0].sp,
                            sp_locks.locks[1].sp,
                            sp_locks.locks[2].sp);
                    break;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else if(3 == PARA2.num)
            {
                corporate_locks = simlock_customize_data_ptr->corporate_locks;

                if((0 == (user_lock_status & MN_SIM_SP_LOCK)) ||
                   (0 == corporate_locks.numLocks))
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,NONE", PARA1.num);
                    break;
                }
                else if(1 == corporate_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d",PARA1.num,corporate_locks.locks[0].corporate);
                    break;
                }
                else if(2 == corporate_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d_%d",PARA1.num,corporate_locks.locks[0].corporate,
                                corporate_locks.locks[1].corporate);
                    break;
                }
                else if(3 == corporate_locks.numLocks)
                {
                    sprintf((char *)g_rsp_str,"+LOCKREAD:%d,%d_%d_%d",PARA1.num,corporate_locks.locks[0].corporate,
                            corporate_locks.locks[1].corporate,
                            corporate_locks.locks[2].corporate);
                    break;
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            break;
        }
    }

    if(NULL != simlock_customize_data_ptr)
    {
        SCI_FREE(simlock_customize_data_ptr);
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}


/*****************************************************************************/
//  Description : This function handle the AT+LVOFLOCK command 
//                 Read Level Of Lock
//  Global resource dependence : none
//  Author:winnie.zhu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessLVOFLOCK)
{
    MN_SIM_LOCK_CUSTOMIZE_DATA_T * simlock_customize_data_ptr = SCI_NULL;
    uint32 user_lock_status;
    uint16 rets =0;

    SCI_TRACE_LOW("ATC: ATC_ProcessLVOFLOCK");
    
    if(!PARAM1_FLAG || !PARAM2_FLAG)
    {
        SCI_TRACE_SLIGHT("ATC: invalid parameter");
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        return S_ATC_SUCCESS; 
    }

    if ( 1 != PARA1.num )
    {
        SCI_TRACE_SLIGHT("ATC: invalid parameter");
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        return S_ATC_SUCCESS; 
    }
       
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {  
            SCI_TRACE_LOW("ATC:msl_verified=%d,  ak_seed_no_verified=%d",msl_verified, ak_seed_no_verified);
            if( !msl_verified || !ak_seed_no_verified)
            {
                SCI_TRACE_SLIGHT("ATC: msl or sk not verified");
                sprintf((char *)g_rsp_str, "+LVOFLOCK:NG");
                break;
            }
            
            simlock_customize_data_ptr = (MN_SIM_LOCK_CUSTOMIZE_DATA_T *)SCI_ALLOC_BASEZ(sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T));
            if(NVERR_NONE != EFS_NvitemRead(ATC_NV_SIM_LOCK_CUSTOMIZE_DATA_ID,
                                    sizeof(MN_SIM_LOCK_CUSTOMIZE_DATA_T),
                                    (void *)simlock_customize_data_ptr))
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }

            user_lock_status = simlock_customize_data_ptr->SIM_lock_status;
            SCI_TRACE_SLIGHT("ATC: user_lock_status=%d", user_lock_status);

            if(( 0 == user_lock_status )|| (0xFFFFFFFF == user_lock_status ))
            {
                rets = 0x0;
                sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,%08d", PARA1.num, rets);
                break;
            }
 
            if(( 0 != (user_lock_status & MN_SIM_NETWORK_LOCK)) &&
                (0 == simlock_customize_data_ptr->network_locks.numLocks))
            {
                sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,NWLOCK FAIL", PARA1.num);
                break;
            }
            else
            {
                rets += 0x1;
                SCI_TRACE_SLIGHT("ATC:1 rets=%d", rets);
            }

            if(( 0 != (user_lock_status & MN_SIM_NETWORK_SUBSET_LOCK)) &&
                (0 == simlock_customize_data_ptr->network_subset_locks.numLocks))
            {
                sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,SUBSETLOCK FAIL", PARA1.num);
                break;
            }
            else
            {
                rets += 0x2;
                SCI_TRACE_SLIGHT("ATC:2 rets=%d", rets);
            }

            if(( 0 != (user_lock_status & MN_SIM_SP_LOCK)) &&
                (0 == simlock_customize_data_ptr->SP_locks.numLocks))
            {
                sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,SPLOCK FAIL", PARA1.num);
                break;
            }
            else
            {
                rets += 0x8;
                SCI_TRACE_SLIGHT("ATC:3 rets=%d", rets);
            }

            if(( 0 != (user_lock_status & MN_SIM_CORPORATE_LOCK)) &&
                (0 == simlock_customize_data_ptr->corporate_locks.numLocks))
            {
                sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,CPLOCK FAIL", PARA1.num);
                break;
            }
            else
            {
                rets += 0x10;
                SCI_TRACE_SLIGHT("ATC:4 rets=%d", rets);
            }

            sprintf((char *)g_rsp_str,"+LVOFLOCK:%d,%08d", PARA1.num,rets);
            break;
        }
    
        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    if(NULL != simlock_customize_data_ptr)
    {
        SCI_FREE(simlock_customize_data_ptr);
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+CALIDATE command 
//                 calibration date
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCALIDATE)
{
    uint32   cali_date = 0;
    SCI_TRACE_LOW("ATC: ATC_ProcessCALDATE");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // calibration date read
            if(1==PARA1.num && 0==PARA2.num && 0==PARA3.num && 0==PARA4.num)
            {
                cali_date = REF_GetCaliDateValue();

                if(cali_date > 0)
                {
                    sprintf((char *)g_rsp_str, "+CALIDATE:%d,%d", PARA1.num,cali_date);
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            // write calibration date
            else if(2==PARA1.num && PARAM2_FLAG && PARAM3_FLAG && PARAM4_FLAG)
            {
                if(PARA2.num>2099 || PARA2.num<2001 || PARA3.num>12 || PARA4.num>31)
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }

                cali_date = PARA2.num*10000+PARA3.num*100+PARA4.num;
                SCI_TRACE_SLIGHT("ATC: cali_date=%d",cali_date);
                    
                if(REF_WriteCaliDateValue(cali_date))
                {
                    sprintf((char *)g_rsp_str, "+CALIDATE:%d", PARA1.num);
                }
                else
                {
                    SCI_TRACE_SLIGHT("ATC: write cal date fail");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+MSLSECUR command 
//                 MSL security 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMSLSECUR)
{
    uint8           *param_ptr      = NULL;
    uint8           param_len       = 0;
    uint8           cmd_type        = 0;
    char            msl_code[33]    = {0};  // MSL code 32 bytes
    char            msl_address[11] = {0};  // MSL address 10 bytes
    char            msl_code_from_nv[33]    = {0};  // MSL code 32 bytes in phone
    char            msl_address_from_nv[11] = {0};  // MSL address 10 bytes in phone
    BOOLEAN         ret             = FALSE;
    ATC_PRODUCTION_LINE_PARAM_T parameter[2];
    uint8           msl_security_code_nv[44] = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessMSLSECUR");

    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    SCI_TRACE_SLIGHT("ATC: para1.str is%s, param_len=%d, cmd_type=%d", param_ptr, param_len, cmd_type);

    param_ptr++;
    param_len--;
    
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (ATC_GetParamsEx(param_ptr, param_len, parameter))
            {
                // MSL Address Read
                if('1' == parameter[0].param[0])
                {
                    ret = DMNV_ReadMSLSECUR(msl_security_code_nv);//lint !e718 !e746
                    SCI_TRACE_SLIGHT("ATC: ret=%d, msl_security_code in NV =%s", ret, msl_security_code_nv);
                    if(ret && 43==strlen((char *)msl_security_code_nv))
                    {
                        SCI_MEMCPY(msl_address, msl_security_code_nv, 10);
                        sprintf((char *)g_rsp_str, "+MSLSECUR:1,%s",msl_address);
                    }
                    else
                    {
                        SCI_TRACE_SLIGHT("ATC: read MSLSEC NV fail or empty");
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                // write MSL
                else if('2' == parameter[0].param[0])
                {
                    SCI_TRACE_SLIGHT("ATC: write MSL");
                    if(45==param_len && '_'==parameter[1].param[10]) 
                    {
                        // extract MSL address & MSL Code
                        SCI_MEMCPY(msl_address, parameter[1].param, 10);
                        SCI_MEMCPY(msl_code, parameter[1].param+11, 32);


                        ret = DMNV_ReadMSLSECUR(msl_security_code_nv);
                        // if MSL address/code exist in phone
                        if(ret && (43==strlen((char *)msl_security_code_nv)))
                        {
                            SCI_MEMCPY(msl_address_from_nv, msl_security_code_nv, 10);
                            SCI_MEMCPY(msl_code_from_nv, msl_security_code_nv+11, 32);
                             msl_address_from_nv[10] = '\0';
                             msl_code_from_nv[32] = '\0';

                            // if MSL address in phone is different to cmd data
                            // or if MSL code in phone is different to cmd data
                            if(0 != strncmp(msl_address, msl_address_from_nv, 10)
                               || 0 != strncmp(msl_code, msl_code_from_nv, 32))
                            {
                                msl_verified = FALSE;
                                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                                break;
                            }
                            else
                            {
                                msl_verified = TRUE;
                                sprintf((char *)g_rsp_str, "+MSLSECUR:2,OK");
                            }
                        }
                        // not exist, write address
                        else
                        {
                            ret = DMNV_WriteMSLSECUR(parameter[1].param);//lint !e718 !e746
                            SCI_TRACE_SLIGHT("ATC: write ret=%d", ret);
                            if ( ret )
                            {
                                msl_verified = TRUE;
                                sprintf((char *)g_rsp_str, "+MSLSECUR:2,OK");
                            }
                            else
                            {
                                msl_verified  = FALSE;
                                SCI_TRACE_SLIGHT("ATC: write msl address fail");
                                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                            }
                        }
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                    }
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+SERIALNO command 
//                 Serial number read/write
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSERIALNO)
{
    uint8            *param_ptr   = NULL;
    uint8            param_len    = 0;
    uint8            cmd_type     = 0;
    ATC_PRODUCTION_LINE_PARAM_T param[2]={0};
    BOOLEAN          ret          = FALSE;
    uint8            serial_number[12] = {0};

    SCI_TRACE_LOW("ATC: ATC_ProcessSERIALNO");

    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    SCI_TRACE_SLIGHT("ATC: para1.str=%s, cmd_type=%d", param_ptr, cmd_type);

    param_ptr++;
    param_len--;

    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if (ATC_GetParamsEx(param_ptr, param_len, param))
            {
                SCI_TRACE_LOW("ATC: param[0]=%d, param[1]_len=%d",param[0].param[0],strlen((char *)param[1].param));
                // Serial number read
                if('1' == param[0].param[0] && '0'==param[1].param[0])
                {
                    ret = DMNV_ReadSERIALNO(serial_number);//lint !e718 !e746
                    if(ret && 11==strlen((char *)serial_number))
                    {
                        sprintf((char *)g_rsp_str, "+SERIALNO:1,%s", serial_number);
                    }
                    else
                    {
                        SCI_TRACE_SLIGHT("ATC: read serial number fail or empty");
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                // write Serial number
                else if('2' == param[0].param[0] && 11==strlen((char *)param[1].param))
                {
                    ret = DMNV_WriteSERIALNO(param[1].param);//lint !e718 !e746
                    if(ret)
                    {
                        sprintf((char *)g_rsp_str, "+SERIALNO:2,OK");
                    }
                    else
                    {
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    }
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                }
            }
            else
            {
                SCI_TRACE_SLIGHT("ATC: parse serial number para fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+FACTORST command 
//                 Factory Reset
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFACTORST)
{
    SCI_TRACE_LOW("ATC: ATC_ProcessFACTORST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(0==PARA1.num && 0==PARA2.num)
            {
                if(DMNV_WriteRestoreFactoryFlag(SCI_TRUE))
                {
                    AT_FACTORST_T *sig_ptr = PNULL;
                    uint16 sig_size = 0;

                    sig_size = sizeof(AT_FACTORST_T);

                    SCI_CREATE_SIGNAL(sig_ptr,APP_ATC_FACTORST_IND,sig_size,P_APP);   /*lint !e64*/

                    sig_ptr->is_rst = TRUE;

                    SCI_SEND_SIGNAL(sig_ptr,P_APP);

                    sprintf((char*)g_rsp_str,"+FACTORST:%d", PARA1.num);
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+AKSEEDNO command 
//                 AK Seed Number read write 
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessAKSEEDNO)
{
    uint32 random_no        = 0;
    uint32 decrypt_no       = 0;
    char random_no_str[51]  = {0};
    uint8 i                 = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessAKSEEDNO");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // AK SEED NO Check Request 
            if( 0 == PARA1.num )
            {
                // decrypt AK Seed Number
                // decrypt_no = decrypt_seed_no();

                if((uint32)PARA2.num == decrypt_no)
                {
                    sprintf((char *)g_rsp_str, "+AKSEEDNO:%d,OK", PARA1.num);
                    ak_seed_no_verified = TRUE;
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+AKSEEDNO:%d,NG", PARA1.num);
                    ak_seed_no_verified = FALSE;
                }                
            }
            // AK Seed Number Read
            else if(1==PARA1.num && 0==PARA2.num)
            {
                srand(SCI_GetTickCount());
                for(i=0; i<50; i++)
                {
                    // generate random number
                    random_no = rand();
                    random_no_str[i] = random_no%100/10+'0';                    
                }
               
                sprintf((char *)g_rsp_str, "+AKSEEDNO:%d, %s", PARA1.num, random_no_str);
            }
            else
            {
                 ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }
                
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
// Description : This function handle the AT+KSTRINGB command 
//                 Key string block
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessKSTRINGB)
{
    BOOLEAN  key_string_flag = FALSE;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessKSTRINGB");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // set key string block off
            if(0 == PARA1.num && 0==PARA2.num)
            {
                if(!DMNV_WriteKeyStringBlockFlag(FALSE))//lint !e718 !e746
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }

                sprintf((char *)g_rsp_str, "+KSTRINGB:%d,OK", PARA1.num);
            }
            // set key string block on
            else if(0 == PARA1.num && 1==PARA2.num)
            {
                if(!DMNV_WriteKeyStringBlockFlag(TRUE))//lint !e718 !e746
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }

                sprintf((char *)g_rsp_str, "+KSTRINGB:%d,OK", PARA1.num);
            }
            // read key string block
            else if((1 == PARA1.num && 0==PARA2.num))
            {
                if(!DMNV_ReadKeyStringBlockFlag(&key_string_flag))//lint !e718 !e746
                {
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }

                // key string block on
                if(key_string_flag)
                {
                    sprintf((char *)g_rsp_str, "+KSTRINGB:%d,ON", PARA1.num);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+KSTRINGB:%d,OFF", PARA1.num);
                }                
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            }
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}
/*****************************************************************************/
// Description : This function handle the AT+SIMDETEC command 
//                 Detect if sim card exists
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSIMDETEC)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessSIMDETEC");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(1 == PARA1.num  
                && ((1<=PARA2.num)&&(PARA2.num<=MN_SYS_NUMBER)))
            {
                dual_sys = (MN_DUAL_SYS_E)(PARA2.num - 1);
                
                if(DMSIM_IsSIMExist(dual_sys))//lint !e718 !e746
                {
                    sprintf((char *)g_rsp_str, "+SIMDETEC:%d,SIM", PARA1.num);
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+SIMDETEC:%d,NOS", PARA1.num);
                }
            }
            else
            {
                ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }
            
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
//  Description : This function handle the AT+FACTOLOG command 
//  Global resource dependence : none
//  Author:Elvis.Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFACTOLOG)
{
    BOOLEAN  flag = TRUE;
    SCI_TRACE_LOW("ATC: ATC_ProcessFACTOLOG");

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if ((!PARAM1_FLAG)  || (!PARAM2_FLAG)  || (!PARAM3_FLAG))
            {
                flag = FALSE;
                break;
            }

            if((PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC) 
                ||(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
            {
                flag = FALSE;
                break;
            }

            if (1==PARA3.num &&PARAM4_FLAG && (PARAM4_TYPE == ATC_CMD_PARAM_TYPE_NUMERIC))
            {
                if (2 == PARA4.num)
                {
                    REFPARAM_SetEnableArmSleepFlag(FALSE);
                    MMIDEFAULT_TurnOnLCDBackLight();//lint !e718 !e746
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);//lint !e718 !e746
                }
                else 
                {
                    REFPARAM_SetEnableArmSleepFlag(TRUE);
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                }
            }


            //save in the NV.
            break;
        }

        default:
        {
            flag = FALSE;
            break;
        }
    }


    if (!flag)
    {
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
    }
    else
    {
        sprintf((char *)g_rsp_str, "+FACTOLOG: 0, OK");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+MISCD command 
//                 get the status of SMCD
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessMISCD)
{
    BOOLEAN mscd_status = FALSE;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessMISCD");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_READ:
        {
            mscd_status = GPIO_CheckSDCardStatus();
            SCI_TRACE_LOW("atc: mscd_status=%d", mscd_status);
            
            if(!mscd_status)
            {
                sprintf((char *)g_rsp_str, "+MISCD: OFF");
            }
            else
            {
                if(MMIFILE_GetDeviceSDStatus())
                {
                    sprintf((char *)g_rsp_str, "+MISCD: SON");
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+MISCD: HON");
                }
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+EMEMTEST command 
//                 get the status of external memory
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessEMEMTEST)
{
    int mem_status = 0;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessEMEMTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            mem_status = GPIO_CheckSDCardStatus();
            
            if(1==PARA1.num && 0==PARA2.num)
            {
                // does not support external memory
                
                if(mem_status)
                {
                    sprintf((char *)g_rsp_str, "+EMEMTEST:1,OK");
                }
                else
                {
                    sprintf((char *)g_rsp_str, "+EMEMTEST:1,NG");
                }         
            }
            // read total size and used size
            else if(1==PARA1.num && 2==PARA2.num)
            {
                uint32    used_space_low = 0;
                uint32    used_space_high  =0;
                uint32    total_space_high  = 0;
                uint32    total_space_low    = 0;
                char str_used_space[21] = {0};
                char str_total_space[40] = {0};

                if(!mem_status)
                {
                    sprintf((char *)g_rsp_str, "+EMEMTEST:1,NG");
                    break;
                }
                
                if(!MMIAPIFM_GetTotalAndUsedInExternalMem(&used_space_low, &used_space_high,&total_space_high, &total_space_low))
                {
                    ATC_RESPONSE_MSG(atc_config_ptr, "NA");                    
                }

                if(0 == used_space_high)
                {
                    sprintf(str_used_space, "%X", used_space_low);
                }
                else
                {
                    sprintf(str_used_space, "%X%X", used_space_high, used_space_low);
                }

               if(0 == total_space_high)
                {
                    sprintf(str_total_space, "%X", total_space_low);
                }
                else
                {
                    sprintf(str_total_space, "%X%X", total_space_high, total_space_low);
                }
                
                sprintf((char *)g_rsp_str, "%s,%s", str_total_space, str_used_space);
                
                SCI_TRACE_LOW("atc: used_space_low=0x%x,used_space_high=0x%x",used_space_low,used_space_high);
                SCI_TRACE_LOW("atc: total_space_low=0x%x,total_space_high=0x%x",total_space_low, total_space_high);
            }
            else
            {
                 ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }
         
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+FCMPTEST command 
//                 start/stop play mp3
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCMPTEST)
{
    AT_FCMPTEST_T *sig_ptr = PNULL;
    uint16 sig_size = 0;
    uint8 file_path[20] = "E:\\1.mp3";
    uint16 wstr_path[20] = {0};
                
    SCI_TRACE_LOW("ATC: ATC_ProcessFCMPTEST");

    sig_size = sizeof(AT_FCMPTEST_T);

    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(0==PARA1.num && 0==PARA2.num && 0==PARA3.num)
            {
                (void)MMIAPICOM_StrToWstr(file_path, wstr_path);
                
                if(MMIAPIFMM_IsFileExist(wstr_path, MMIAPICOM_Wstrlen(wstr_path)))
                {
                    SCI_CREATE_SIGNAL(sig_ptr,APP_ATC_PLAY_MP3_IND ,sig_size, P_APP);   /*lint !e64*/

                    SCI_SEND_SIGNAL(sig_ptr,P_APP);

                    sprintf((char*)g_rsp_str,"+FCMPTEST:0");
                }
                else
                {
                    SCI_TRACE_LOW("ATC: file %s not exist", (char *)file_path);
                    ATC_RESPONSE_MSG(atc_config_ptr, "NG");
                }
            }
            // stop play mp3
            else if(0==PARA1.num && 0==PARA2.num && 1==PARA3.num)
            {
                (void)MMIAPICOM_StrToWstr(file_path, wstr_path);
                
                if(MMIAPIFMM_IsFileExist(wstr_path, MMIAPICOM_Wstrlen(wstr_path)))
                {
                    SCI_CREATE_SIGNAL(sig_ptr,APP_ATC_STOP_MP3_IND ,sig_size, P_APP);   /*lint !e64*/

                    SCI_SEND_SIGNAL(sig_ptr,P_APP);

                    sprintf((char*)g_rsp_str,"+FCMPTEST:0");
                }
                else
                {
                    SCI_TRACE_LOW("ATC: file %s not exist", file_path);
                    ATC_RESPONSE_MSG(atc_config_ptr, "NG");
                }
            }
            else
            {
                 ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }
         
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+FCBTTEST command 
//                 switch BT function, search device and give research result within 10 seconds
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessFCBTTEST)
{
#ifdef BLUETOOTH_SUPPORT
    BT_STATUS ret = BT_SUCCESS;
    
    SCI_TRACE_LOW("ATC: ATC_ProcessFCBTTEST");
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            // BT on
            if(0==PARA1.num && 0==PARA2.num && 0==PARA3.num)
            {
                ret =  BT_Start() ;  // if return IS NOT BT_PENDING,  return ERROR to PC
                SCI_TRACE_LOW("atc: bt start ret =%d", ret);
                if(BT_PENDING == ret)
                {
                    SCI_SET_TIMER(P_ATC, ATC_GET_BT_STATUS_INTERVAL,
                                     ATC_BT_STATUS_IND, 
                                     &T_ATC_BT_STATUS_IND, 
                                     "T_ATC_BT_STATUS_IND" );

                    sprintf((char *)g_rsp_str, "+FCBTTEST:0");
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr, "NG");                    
                }
            }
            // search any equipment within 10 senconds, 
            // dont care BT's power on or power off
            else if(0==PARA1.num && 1==PARA2.num && 1==PARA3.num)
            {
                BOOLEAN bt_status = FALSE;
    
                bt_status = BT_GetState();
                if(bt_status)
                {
                    BT_SearchDevice(BT_SERVICE_ALL);
                    
                     SCI_SET_TIMER(P_ATC, ATC_GET_BT_SEARCH_INTERVAL,
                                     ATC_BT_SEARCH_DEVICE_IND, 
                                     &T_ATC_BT_SEARCH_IND, 
                                     "T_ATC_BT_SEARCH_IND" );

                     sprintf((char *)g_rsp_str, "+FCBTTEST:0");
                }                
                else
                {
                    sprintf((char *)g_rsp_str, "+FCBTTEST:NOT Actived");
                }            
            }
            // BT power off
            else if(0==PARA1.num && 2==PARA2.num && 0==PARA3.num)
            {
                ret =  BT_Stop();   // if return IS NOT BT_PENDING,  return ERROR to PC

                if(BT_PENDING == ret)                
                {
                    sprintf((char *)g_rsp_str, "+FCBTTEST:0");
                    
                    SCI_SET_TIMER(P_ATC, ATC_GET_BT_STATUS_INTERVAL,
                                     ATC_BT_STATUS_IND, 
                                     &T_ATC_BT_STATUS_IND, 
                                     "T_ATC_BT_STATUS_IND" );
                }
                else
                {
                    ATC_RESPONSE_MSG(atc_config_ptr, "NG");                    
                }
            }
            else
            {
                 ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }
#endif    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+GETFDATA command 
//                 get the value of the structure member passed as the parameter
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFDATA)
{
    AT_FACT_RECD_PARAM fail_rec = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessGETFDATA");

    SCI_MEMSET(&fail_rec, 'N', sizeof(AT_FACT_RECD_PARAM));
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!DMNV_ReadFactData(&fail_rec))
            {
                SCI_TRACE_LOW("ATC: read NV fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }
            
            switch(PARA1.num)
            {
                case 1:
                {
                    sprintf((char *)g_rsp_str, "+GETFDATA:%c", fail_rec.fail_record);
                    break;
                }

                case 2:
                {
                    sprintf((char *)g_rsp_str, "+GETFDATA:%c", fail_rec.retry);
                    break;
                }

                 case 3:
                {
                    sprintf((char *)g_rsp_str, "+GETFDATA:%c", fail_rec.dummy11);
                    break;
                }

                  case 4:
                {
                    sprintf((char *)g_rsp_str, "+GETFDATA:%c", fail_rec.dummy12);
                    break;
                }

                default:
                {
                    SCI_TRACE_LOW("ATC: invalid index");
                    ATC_RESPONSE_MSG(atc_config_ptr, "NA");
                    break;
                }
            }
         
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}


/*****************************************************************************/
// Description : This function handle the AT+SETFDATA command 
//                 set the value of the index passed as first parameter
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSETFDATA)
{
    uint8  *param_ptr                                       = NULL;
    uint8  param_len                                         = 0;
    uint8  cmd_type                                          = 0;
    ATC_PRODUCTION_LINE_PARAM_T  param[3]  = {0};
    AT_FACT_RECD_PARAM fail_rec = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessSETFDATA");

    SCI_MEMSET(&fail_rec, 'N', sizeof(AT_FACT_RECD_PARAM));

    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    param_ptr++;
    param_len--;
    
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            if ((!ATC_GetParamsEx(param_ptr, param_len, param))
                || (0==param_len))
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }

            if(DMNV_ReadFactData(&fail_rec))
            {
                uint8 index = 0;
                uint8 value = 0;

                if(1 == strlen((char *)param[0].param))
                {
                    index = param[0].param[0] - '0';
                }
                else if(2 == strlen((char *)param[0].param))
                {
                    index = param[0].param[1] - '0';
                }
                else
                {
                    SCI_TRACE_LOW("ATC: invalid index");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                    break;
                }
                
                value = param[1].param[0];

                SCI_TRACE_LOW("atc: param[0]:%s, param[1]:%c", param[0].param,param[1].param[0]);
                SCI_TRACE_LOW("atc: index =%d, value=0x%x, %c", index, value, value);

                switch(index)
                {
                    case 1:
                    {
                        fail_rec.fail_record = value;
                        break;
                    }

                    case 2:
                    {
                        fail_rec.retry = value;
                        break;
                    }

                    case 3:
                    {
                        fail_rec.dummy11= value;
                        break;
                    }

                    case 4:
                    {
                        fail_rec.dummy12= value;
                        break;
                    }

                    default:
                    {
                        SCI_TRACE_LOW("ATC: invalid index");
                        ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
                        break;
                    }
                }

                if(DMNV_WriteFactData(&fail_rec))
                {
                   sprintf((char *)g_rsp_str, "+SETFDATA:%s,%s", 
                                            param[0].param, param[1].param);
                }
                else
                {
                    SCI_TRACE_LOW("ATC: wrtie NV fail");
                    ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                }
            }
            else
            {
                SCI_TRACE_LOW("ATC: read NV fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }     
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

/*****************************************************************************/
// Description : This function handle the AT+GETFULLFDATA command 
//                 get the data of the all structure members
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGETFULLFDATA)
{
    AT_FACT_RECD_PARAM fail_rec = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessGETFULLFDATA");

    SCI_MEMSET(&fail_rec, 'N', sizeof(AT_FACT_RECD_PARAM));
    
    switch(ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_EXECUTE:
        {
            if(DMNV_ReadFactData(&fail_rec))
            {
                sprintf((char *)g_rsp_str, "+GETFULLFDATA:%c%c%c%c",
                             fail_rec.fail_record, fail_rec.retry, fail_rec.dummy11, fail_rec.dummy12);
            }
            else
            {
                SCI_TRACE_LOW("ATC: read NV fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }
                        
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}


/*****************************************************************************/
// Description : This function handle the AT+SETFULLFDATA command 
//                 set the data of the all structure members
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSETFULLFDATA)
{
    uint8  *param_ptr                                       = NULL;
    uint8  param_len                                         = 0;
    uint8  cmd_type                                          = 0;
    ATC_PRODUCTION_LINE_PARAM_T  param[3]  = {0};
    
    SCI_TRACE_LOW("ATC: ATC_ProcessSETFULLFDATA");

    param_ptr = PARA1.str_ptr->str_ptr;
    param_len = PARA1.str_ptr->str_len;
    
    ATC_GetCmdType(param_ptr, param_len, &cmd_type);
    param_ptr++;
    param_len--;
    
    switch(cmd_type)
    {
        case ATC_CMD_TYPE_SET:
        {
            AT_FACT_RECD_PARAM fail_rec = {0};
            
            SCI_MEMSET(&fail_rec, 'N', sizeof(AT_FACT_RECD_PARAM));
            
            SCI_TraceLow("atc: fail_rec init fail_record=%c, retry=%d, dumy1=%c, dumy2=%c", fail_rec.fail_record, fail_rec.retry , fail_rec.dummy11 ,fail_rec.dummy12 );

            if ((!ATC_GetParamsEx(param_ptr, param_len, param))
                || (0==param_len))
            {
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
                break;
            }

            fail_rec.fail_record = param[0].param[0];
            fail_rec.retry         = param[0].param[1];
            fail_rec.dummy11     = param[0].param[2];
            fail_rec.dummy12     = param[0].param[3];

            SCI_TraceLow("atc: fail_rec=%c, retry=%d, dumy1=%c, dumy2=%c", fail_rec.fail_record, fail_rec.retry , fail_rec.dummy11 ,fail_rec.dummy12 );

            
            if(DMNV_WriteFactData(&fail_rec))
            {
                sprintf((char *)g_rsp_str, "+SETFULLFDATA:%s", param[0].param);
            }
            else
            {
                SCI_TRACE_LOW("ATC: wrtie NV fail");
                ATC_RESPONSE_MSG(atc_config_ptr,  "NG");
            }

            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr,  "NA");
            break;
        }
    }     
    
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    return S_ATC_SUCCESS; 
}

#else
AT_CMD_FUNC(ATC_ProcessHEADINFO)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessSYSSLEEP)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessFCEPTEST)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessTEMPTEST)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessSPKSTEST)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessKEYSHORT)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessRSTVERIF)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessGETTESTNV)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessSETTESTNV)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessIMEITEST)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessGETFULLHISTNV)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessLOGERASE)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessGETFULLTESTNV)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessLOOPTEST)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessBATTTEST)
{
    return 1;
}
AT_CMD_FUNC( ATC_ProcessMAXPOWER )
    {
    return 1;
}
AT_CMD_FUNC( ATC_ProcessREADRSSI )
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessDETALOCK)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessLOCKREAD)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessLVOFLOCK)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessCALIDATE)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessMSLSECUR)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessSERIALNO)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessFACTORST)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessAKSEEDNO)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessKSTRINGB)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessSIMDETEC)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessFACTOLOG)
    {
    return 1;
}
AT_CMD_FUNC(ATC_ProcessMISCD)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessEMEMTEST)
{
    return 1;
}

AT_CMD_FUNC(ATC_ProcessFCMPTEST)
{
    return 1;
}

AT_CMD_FUNC(ATC_ProcessFCBTTEST)
{
    return 1;
}

AT_CMD_FUNC(ATC_ProcessGETFDATA)
{
    return 1;
}

AT_CMD_FUNC(ATC_ProcessSETFDATA)
{
    return 1;
}

AT_CMD_FUNC(ATC_ProcessSETFULLFDATA)
{
    return 1;
}
AT_CMD_FUNC(ATC_ProcessGETFULLFDATA)
{
    return 1;
}
#endif
/*****************************************************************************/
//  Description : This function used to start the test of GSM Max Power Transmission
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmTxPwrTest(ATC_CONFIG_T *atc_config_ptr)
{
 

    SCI_TRACE_LOW("ATC: ATC_StartGsmTxPowerTest start MAX power transmission!");
    SCI_TRACE_LOW("ATC: is_power_on=%d, arfcn=%d, band=%d, gain=%d", 
        1, g_atc_power_info.arfcn, g_atc_power_info.ms_band, g_atc_power_info.gainpower);

    L1API_GsmTxPwrTest(TRUE, 
                                          g_atc_power_info.arfcn, 
                                          g_atc_power_info.ms_band, 
                                          g_atc_power_info.gainpower
                                          );

    sprintf((char *)g_rsp_str, "%s: 0", g_atc_info_table[AT_CMD_MAXPOWER].cmd_name->str_ptr);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
//#endif
    return;
}

/*****************************************************************************/
//  Description : This function used to start the test of GSM Receive Power
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_StartGsmRxPwrTest(ATC_CONFIG_T *atc_config_ptr)
{
    SCI_TRACE_LOW("ATC: ATC_StartGsmRxPwrTest start Rx power test!");

    L1API_GsmRXPwrTest(TRUE, 
                                          g_atc_power_info.arfcn, 
                                          g_atc_power_info.ms_band);

    g_atc_read_rssi_times = 12;
    SCI_SET_TIMER(P_ATC, ATC_RSSI_READ_INTERVAL,
                                 ATC_RSSI_READ_EXP_IND, 
                                 &T_ATC_RSSI_READ_EXP_IND, 
                                 "T_ATC_RSSI_READ_EXP_IND" );
    
    return;
}
/*****************************************************************************/
//  Description : This function used to start the test of GSM Receive Power
//  Global resource dependence : g_rsp_str
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
void ATC_RssiReadExpInd(ATC_CONFIG_T *atc_config_ptr)
{
#ifndef _ULTRA_LOW_CODE_

    MN_DUAL_SYS_E  dual_sys = g_atc_power_info.dual_sys;
    BOOLEAN  flag = FALSE;
    int16    result = 0;

    SCI_TRACE_LOW("ATC: ATC_RssiReadExpInd, s_atc_read_rssi_times = %d", g_atc_read_rssi_times);

    ATC_SET_CURRENT_LINK_ID(atc_config_ptr, s_atc_rssi_link_id);

    flag = L1API_GetGsmRXPwrResult(g_atc_power_info.arfcn,
                                                     g_atc_power_info.ms_band,
                                                     &result);

    SCI_TRACE_LOW("atc: L1API_GetGsmRXPwrResult, flag=%d", L1API_GetGsmRXPwrResult);
    if (flag)
    {
        sprintf((char *)g_rsp_str, "%s: 0,%d", 
                 g_atc_info_table[AT_CMD_READRSSI].cmd_name->str_ptr, result);

        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

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

        return;
    }

    if (--g_atc_read_rssi_times)
    {
        SCI_SET_TIMER(P_ATC, ATC_RSSI_READ_INTERVAL,
                                     ATC_RSSI_READ_EXP_IND, 
                                     &T_ATC_RSSI_READ_EXP_IND, 
                                     "T_ATC_RSSI_READ_EXP_IND" );
    }
    else
    {
        ATC_RESPONSE_MSG(atc_config_ptr, "NA");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);

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
#endif
    return;
}

#ifdef BLUETOOTH_SUPPORT
/*****************************************************************************/
//  Description : This function used to get BT status: On/Off
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note:
/*****************************************************************************/
void ATC_GetBtStatus(ATC_CONFIG_T *atc_config_ptr)
{
    BOOLEAN bt_status = FALSE;

    SCI_TRACE_LOW("ATC: ATC_GetBtStatus");
    
    SCI_KILL_TIMER(P_ATC, ATC_BT_STATUS_IND,
                              &T_ATC_BT_STATUS_IND,
                              "T_ATC_BT_STATUS_IND" );

    bt_status = BT_GetState();  // TRUE : BT on
    SCI_TRACE_LOW("atc: bt_status =%d", bt_status);

    if(bt_status)
    {
        sprintf((char *)g_rsp_str, "+FCBTTEST:0,ON");
    }
    else
    {
        sprintf((char *)g_rsp_str, "+FCBTTEST:0,OFF");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
}

/*****************************************************************************/
//  Description : This function used to get BT Searched device count
//  Global resource dependence : g_rsp_str
//  Author: longting.zhao
//  Note:
/*****************************************************************************/
void ATC_GetBtDeviceCnt(ATC_CONFIG_T *atc_config_ptr)
{
    uint32 bt_devive_cnt = 0;

    SCI_TRACE_LOW("ATC: ATC_GetBtDeviceCnt");
    
    SCI_KILL_TIMER(P_ATC, ATC_BT_SEARCH_DEVICE_IND,
                              &T_ATC_BT_SEARCH_IND,
                              "T_ATC_BT_SEARCH_IND" );
#ifndef WIN32
    bt_devive_cnt = BT_GetSearchDeviceCount();
#endif    
    SCI_TRACE_LOW("atc: bt device cnt=%d", bt_devive_cnt);

    if(0 < bt_devive_cnt)
    {
        sprintf((char *)g_rsp_str, "+FCBTTEST:FOUND");
    }
    else
    {
        sprintf((char *)g_rsp_str, "+FCBTTEST:NOT FOUND");
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
}

#endif
/*****************************************************************************/
//  Description : This function handle get IMEI check Digit
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
LOCAL uint8 ATC_GetIMEICheckDigit(uint8 * imei)
{
    uint8  len                  = 14;
    char imei_tmp[15]      = {0};
    uint8 sum_odd         = 0;
    uint8 sum_even        = 0;
    uint8 sum_odd_tmp  = 0;
    uint8 sum                 = 0;
    uint8  i                      = 0;
    uint8   imei_checkdigit  = 0;

    // convert string number to digit
    for (i=0; i<len; ++i)
    {
        if (*(imei+i) >= '0' && *(imei+i)<='9')
        {
            imei_tmp[i] = *(imei+i) - '0';
        }
    }

    // double odd digits, add together individual digits 
    for (i=1; i<len; i+=2)
    {
        sum_odd_tmp =  imei_tmp[i]*2;

        if (sum_odd_tmp >= 10)
        {
            sum_odd = sum_odd + sum_odd_tmp/10;
        }

        sum_odd = sum_odd + sum_odd_tmp % 10;
    }

    // add even digit
    for (i=0; i<len; i+=2)
    {
        sum_even = sum_even + imei_tmp[i];
    }

    // add odd sum and even sum
    sum = sum_odd + sum_even;

    if (0 == (sum%10))
    {
        imei_checkdigit = 0;
    }
    else
    {
        imei_checkdigit = ( sum/10+1 )*10 - sum;
    }

    return imei_checkdigit;    
}    

/*****************************************************************************/
//  Description : This function handle Record History test NV
//  Global resource dependence : none
//  Author:longting zhao
//  Note:
/*****************************************************************************/
LOCAL void ATC_RecordHistoryNV(ATC_FACTORY_ITEM_T hist_nv)
{
    ATC_FACTORY_ITEM_T        hist_nv_buf[61] = {0};
    uint8                              i =0;
    uint8                              j = 0;

    SCI_TRACE_SLIGHT("ATC: ATC_RecordHistoryNV");
     
    SCI_MEMSET(hist_nv_buf, 0x0, sizeof(hist_nv_buf));

    if(!DMNV_ReadHistNV((uint16 *)hist_nv_buf))
    {
         SCI_MEMSET((char *)hist_nv_buf, 0xff, sizeof(hist_nv_buf));

        if(!DMNV_WriteHistNV((uint16 *)hist_nv_buf))
        {
            SCI_TRACE_SLIGHT("ATC: write his NV fail");
            return;
        }
    }

    // search for available space in history NV 
    for(i=0; i<60; i++)
    {
        if((hist_nv_buf[i].id>>7) == 1)
        {
            SCI_TRACE_SLIGHT("ATC: available space, i=%d", i);
            break;
        }
    }

    // history NV record has been full
    if(i >= 60)
    {
        for(j=0; j<60; j++)
        {
            hist_nv_buf[j].id = hist_nv_buf[j].id | 0x80;          
        }
        
        hist_nv_buf[0].id = hist_nv.id;
        hist_nv_buf[0].result_code = hist_nv.result_code;

        if(!DMNV_WriteHistNV((uint16 *)hist_nv_buf))
        {
            SCI_TRACE_SLIGHT("ATC: write his NV fail");
            return;
        }
    }
    else
    {
        hist_nv_buf[i].id = hist_nv.id;
        hist_nv_buf[i].result_code = hist_nv.result_code;
    }

    if(DMNV_WriteHistNV((uint16 *)hist_nv_buf))
    {
        SCI_TRACE_SLIGHT("ATC: write hist nv success");
    }
    else
    {
        SCI_TRACE_SLIGHT("ATC: write hist nv fail");
    }
}
/*****************************************************************************/
//  Description : This function is used to handle looptest functionalities,
//                     when receving APP_MN_PS_POWER_OFF_CNF
//  Global resource dependence : AT+LOOPTEST is executed
//  Author:       longting zhao
//  Note:
/*****************************************************************************/
void ATC_LoopTestPsOffCnf(ATC_CONFIG_T *atc_config_ptr)
{   
    switch( loop_test_info.mode)
    {
        // MIC - Receiver Loopback on (Packet loopback)
        case 2:
        {
            AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDHOLD);
            AUD_EnableVoiceLoopback(TRUE, 2, loop_test_info.voice_format,ATC_LOOP_TEST_DELAY_TIME);
            sprintf((char *)g_rsp_str, "+LOOPTEST:0");
            break;
        }

        // MIC - Loud SPK Loopback On (Packet Loopback)
        case 3:
        {
            AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
            AUD_EnableVoiceLoopback(TRUE, 2, loop_test_info.voice_format, ATC_LOOP_TEST_DELAY_TIME);
            sprintf((char *)g_rsp_str, "+LOOPTEST:0");
            break;
        }

        // MIC 2 - Loud SPK Loopback On (Packet Loopback)
        case 5:
        {
            AUDIO_SetDevMode(AUDIO_DEVICE_MODE_HANDFREE);
            AUD_EnableVoiceLoopback(TRUE, 2, loop_test_info.voice_format, ATC_LOOP_TEST_DELAY_TIME);
            sprintf((char *)g_rsp_str, "+LOOPTEST:0");
            break;
        }

        // MIC 쭯 Ear SPK Loopback On (PCM Loopback)
        case 7:
        {
            AUDIO_SetDevMode(AUDIO_DEVICE_MODE_TVOUT);
            AUD_EnableVoiceLoopback(TRUE, 1, loop_test_info.voice_format, ATC_LOOP_TEST_DELAY_TIME);
            sprintf((char *)g_rsp_str, "+LOOPTEST:0");
            break;
        }

        default:
        {
            ATC_RESPONSE_MSG(atc_config_ptr, "NA");
            break;
        }
    }

    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
    return; 
}

/*****************************************************************************/
//  Description : This function is used to get the parameters (AT+IMEITEST,AT+SETTESTNV)
//  Global resource dependence : none
//  Author:       longting zhao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetParamsEx(uint8 *in_ptr, uint8 in_len, ATC_PRODUCTION_LINE_PARAM_T *param_ptr)
{
    int32             i = 0;
    int32             j = 0;
    uint16           k = 0;
    BOOLEAN   flag = TRUE;

    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;
            
            if (3 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            if (j < ATC_PRODUCTION_LINE_CMD_MAX_LEN)
            {
                if (in_ptr[i] != '"')
                {
                    param_ptr[k].param[j++] = in_ptr[i];
                    param_ptr[k].param_len += 1;
                }
            }
            else
            {
                flag = FALSE;
                break;
            }
        }       
    }
    
    return flag;
}

/*****************************************************************************/
//  Description : This function used to extract parameters
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ATC_GetMultiParamsEx(uint8 *in_ptr, uint8 in_len, ATC_PRODUCTION_LINE_PARAM_T *param_ptr)
{
    int32            i    = 0;
    int32            j = 0;
    uint16           k = 0;
    BOOLEAN          flag = TRUE;

    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;
            
            if (7 < k)
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            if (j < ATC_PRODUCTION_LINE_CMD_MAX_LEN)
            {
                if (in_ptr[i] != '"')
                {
                    param_ptr[k].param[j++] = in_ptr[i];
                    param_ptr[k].param_len += 1;
                }
            }
            else
            {
                flag = FALSE;
                break;
            }
        }       
    }

    return flag;
}

/*****************************************************************************/
//  Description : This function used to check detal lock keys
//  Author: winnie.zhu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN key_check(uint8 *keys, uint16 key_len)
{
    uint i;

    if(8 !=key_len)
    {
        return FALSE;
    }
    for(i=0; i<key_len; i++)
    {
        if(!((keys[i])>='0'&&(keys[i])<='9'))
        {
            return FALSE;
        }
    }
    return TRUE;
}

BOOLEAN Atc_SysCrHashSHA1(uint8 *szIn, uint8 *szOut)
{
	SHA_CTX		context;
	CrUINT8		digest[UNLOCK_KEY_MAX];		// 128bits
	uint8		input[UNLOCK_KEY_MAX];		// 128bits

	// NULL문자 이후의 쓰레기 값을 지운다.
	memset(input, 0, UNLOCK_KEY_MAX);
	memset(digest, 0, UNLOCK_KEY_MAX);
	strcpy(input, szIn);

	// generate hash
	ATC_SHA1_Init(&context);
	ATC_SHA1_Update(&context, (CrUINT8 *)input, 16);
	ATC_SHA1_Final(digest, &context);

	memcpy(szOut, digest, 20);

    SCI_TRACE_LOW("szIn=%s", szIn);
    SCI_TRACE_LOW("szOut=%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            szOut[0], szOut[1], szOut[2], szOut[3], szOut[4],  szOut[5],  szOut[6], szOut[7],
            szOut[8], szOut[9], szOut[10],szOut[11],szOut[12], szOut[13], szOut[14],szOut[15],
            szOut[16],szOut[17],szOut[18],szOut[19]);
	return TRUE;
}

