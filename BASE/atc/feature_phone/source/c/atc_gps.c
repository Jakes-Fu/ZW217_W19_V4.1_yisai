/******************************************************************************
 ** File Name:      atc_gps.c                                           *
 ** Author:         CUIFANG.DENG                                               *
 ** Date:           07/06/2017                                               *
 ** Copyright:      Copyright 2001-2017 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains functions definitions about GPS ATC.  *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 07/06/2017     CUIFANG DENG         Create.                                   *
 ******************************************************************************/

#include "atc_basic_cmd.h"
#include "atc.h"
#include "sig_code.h"

#ifdef _MNGPS_SUPPORT_
#include "mn_gps_api.h"
#include "mn_gps_type.h"
#include "atc_gps.h"
#include "atc_plus_gprs.h"
#include "gps_interface.h"

extern ATC_CONFIG_T                    s_config_info;
extern RSP_RESULT_STR        g_rsp_str;
extern const ATC_INFO_T            g_atc_info_table[];
extern  MN_DUAL_SYS_E g_current_card_id;

BOOLEAN s_is_gps_act_pdp_req = FALSE;
uint8 pdp_ipv4[]="IP";
uint8 apn_cmcc[]="CMNET";
uint8 apn_uicc[]="UNINET";

ATC_GPS_CONFIG_T  gps_config ={0};

const char    *s_gpstest_str[6] =
{
    "NMEA/GGA","NMEA/GLL","NMEA/GSA","NMEA/GSV","NMEA/RMC","NMEA/VTG" 
};

extern int   stricmp(char *s1, const char *s2);

PUBLIC SCI_TIMER_PTR     s_gps_fix_timeout_timer_ptr = PNULL;
PUBLIC SCI_TIMER_PTR     s_gps_tracking_timer_ptr = PNULL;
PUBLIC SCI_TIMER_PTR     s_gps_interval_timer_ptr = PNULL;

#endif

/*****************************************************************************/
//  Description : This function handle asynchronous gps after receive
//                asynchronous events
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void ATC_ProcessGpsEvent(ATC_CONFIG_T *atc_config_ptr,xSignalHeaderRec *sig_ptr)
{
#ifdef _MNGPS_SUPPORT_
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(sig_ptr ->SignalCode)
    {
        case APP_MN_GPS_DOWNLOAD_CNF:
        {
            APP_MN_GPS_DOWNLOAD_CNF_T*param_ptr = (APP_MN_GPS_DOWNLOAD_CNF_T *)sig_ptr;
            SCI_TRACE_LOW("ATC: receive APP_MN_GPS_DOWNLOAD_CNF is_ok=%d", param_ptr->is_ok);            
            
            if(TRUE == param_ptr->is_ok)
            {
                if((ATC_SPGPSPWR == gps_config.gps_cmd_type) ||
                    (ATC_QGNSSC == gps_config.gps_cmd_type))
                {
                    MNGPS_InitReq(dual_sys);

                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_INIT_CNF, 0xff, ATC_NO_DOMAIN);  
                }
            }
            else
            {
                //sprintf((char*)g_rsp_str, "%s: ERROR", g_atc_info_table[AT_CMD_SPGPSPWR].cmd_name->str_ptr);
                //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_GPS_OPERATION_FAILED);
            }
   
            break;
        }

        case APP_MN_GPS_INIT_CNF:
        {
            APP_MN_GPS_INIT_CNF_T*param_ptr = (APP_MN_GPS_INIT_CNF_T *)sig_ptr;
            
            SCI_TRACE_LOW("ATC: receive APP_MN_GPS_INIT_CNF is_ok=%d", param_ptr->is_ok); 
            if(TRUE == param_ptr->is_ok)
            {
                if(ATC_QGNSSC == gps_config.gps_cmd_type)
                {
                    MNGPS_StartGpsReq(dual_sys);

                   ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_START_CNF, 0xff, ATC_NO_DOMAIN);  
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                }
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR, ERR_GPS_OPERATION_FAILED);
            }
   
            break;
        }

        case APP_MN_GPS_START_CNF:
        {
            APP_MN_GPS_INIT_CNF_T*param_ptr = (APP_MN_GPS_INIT_CNF_T *)sig_ptr;
            
            if(TRUE == param_ptr->is_ok)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR, ERR_GPS_OPERATION_FAILED);
            }
   
            break;
        }

        case APP_MN_GPS_STOP_CNF:
        {
            APP_MN_GPS_STOP_CNF_T*param_ptr = (APP_MN_GPS_STOP_CNF_T *)sig_ptr;

            if(gps_config.gps_cmd_type == ATC_SPGPSTTFF)
            {
                ATC_GpsStartTimer(dual_sys, ATC_FIX_INTERVAL, gps_config.interval_time);
            }
            else
            {
                if(TRUE == param_ptr->is_ok)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR, ERR_GPS_OPERATION_FAILED);
                }
            }
   
            break;
        }

        case APP_MN_GPS_RESET_CNF:
        {
            APP_MN_GPS_RESET_CNF_T*param_ptr = (APP_MN_GPS_RESET_CNF_T *)sig_ptr;

            SCI_TRACE_LOW("ATC: APP_MN_GPS_RESET_CNF param_ptr->is_ok=%d", param_ptr->is_ok);
            if(gps_config.gps_cmd_type == ATC_SPGPSTTFF)
            {
                gps_config.test_times++;
                SCI_TRACE_LOW("ATC: APP_MN_GPS_RESET_CNF gps_config.test_times=%d", gps_config.test_times);

                if(TRUE == param_ptr->is_ok)
                {
                     ATC_GpsStartTimer(dual_sys, ATC_FIX_TIMEOUT, gps_config.fix_timeout);

                    ATC_GpsStartTimer(dual_sys, ATC_FIX_TEST, 5000);

                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                              (uint16)APP_MN_GPS_FIX_IND, 0xff, ATC_NO_DOMAIN);     
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR, ERR_GPS_OPERATION_FAILED);

                }
                
            }
            else
            {

                if(TRUE == param_ptr->is_ok)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);

                    
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR, ERR_GPS_OPERATION_FAILED);
                }
            }
   
            break;
        }

        case APP_MN_GPS_READ_STATUS_CNF:
        {
            APP_MN_GPS_READ_STATUS_CNF_T*param_ptr = (APP_MN_GPS_READ_STATUS_CNF_T *)sig_ptr;

            if(MNGPS_NOT_FIX == param_ptr->fix_status)
            {
                sprintf((char*)g_rsp_str, "%s: Location Not Fix", g_atc_info_table[AT_CMD_SPGPSSTATUS].cmd_name->str_ptr);
            }
            else if(MNGPS_2D_FIX == param_ptr->fix_status)
            {
                sprintf((char*)g_rsp_str, "%s: Location 2D Fix", g_atc_info_table[AT_CMD_SPGPSSTATUS].cmd_name->str_ptr);

            }
            else if(MNGPS_3D_FIX == param_ptr->fix_status)
            {
                sprintf((char*)g_rsp_str, "%s: Location 3D Fix", g_atc_info_table[AT_CMD_SPGPSSTATUS].cmd_name->str_ptr);
            }
            else
            {
                sprintf((char*)g_rsp_str, "%s: Location Unknow", g_atc_info_table[AT_CMD_SPGPSSTATUS].cmd_name->str_ptr);
            }
            
            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
            
            break;
        }

        case APP_MN_GPS_READ_INFO_CNF:
        {
            APP_MN_GPS_READ_INFO_CNF_T*param_ptr = (APP_MN_GPS_READ_INFO_CNF_T *)sig_ptr;

            sprintf((char*)g_rsp_str, "%s: %f,%f,%f,%04d%02d%02d%02d%02d%02d,%d,%d,%f,%f", g_atc_info_table[AT_CMD_SPGPSINF].cmd_name->str_ptr,
            param_ptr->gps_info.longitude, param_ptr->gps_info.latitude, param_ptr->gps_info.altitude,
            param_ptr->gps_info.utc_time.year, param_ptr->gps_info.utc_time.month, param_ptr->gps_info.utc_time.day,
            param_ptr->gps_info.utc_time.hour, param_ptr->gps_info.utc_time.minute, param_ptr->gps_info.utc_time.second,
            param_ptr->gps_info.ttff_time, param_ptr->gps_info.satellite_num, param_ptr->gps_info.speed, param_ptr->gps_info.course);

            SCI_TRACE_LOW("%s", g_rsp_str);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
            
            break;
        }

        case APP_MN_GPS_OUTPUT_CNF:
        {
            APP_MN_GPS_OUTPUT_CNF_T*param_ptr = (APP_MN_GPS_OUTPUT_CNF_T *)sig_ptr;

            if(ATC_QGNSSC == gps_config.gps_cmd_type)
            {
                //do nothing, wait APP_MN_GPS_OUTPUT_INFO_IND message
            }
            else
            {

                if(TRUE == param_ptr->is_ok)
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                }
                else
                {
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_GPS_OPERATION_FAILED);
                }
            }
            
            break;
        }

        case APP_MN_GPS_POWEROFF_CNF:
        {
            APP_MN_GPS_POWEROFF_CNF_T*param_ptr = (APP_MN_GPS_POWEROFF_CNF_T *)sig_ptr;

            if(TRUE == param_ptr->is_ok)
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_GPS_OPERATION_FAILED);
            }
            
            break;
        }

        case APP_MN_GPS_OUTPUT_INFO_IND:
        {
            APP_MN_GPS_OUTPUT_INFO_IND_T*param_ptr = (APP_MN_GPS_OUTPUT_INFO_IND_T *)sig_ptr;

            if(ATC_QGNSSC == gps_config.gps_cmd_type)
            {
                if(ATC_NMEA_NONE == gps_config.nmea_type)
                {
                    MNGPS_OutputReq(dual_sys, 0, 1);
                }
                else if(ATC_NMEA_ALL == gps_config.nmea_type)
                {
                    SCI_TRACE_LOW("ATC:%s:%d,%d,%s", g_atc_info_table[AT_CMD_QGNSSRD].cmd_name->str_ptr, param_ptr->length, param_ptr->type,param_ptr->nmea_data);

                    if(ATC_NMEA_NONE == gps_config.receive_type)
                    {
                        sprintf((char*)g_rsp_str, "%s:%s", g_atc_info_table[AT_CMD_QGNSSRD].cmd_name->str_ptr, param_ptr->nmea_data);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        gps_config.receive_type = gps_config.receive_type | param_ptr->type;

                    }
                    else
                    {
                        sprintf((char*)g_rsp_str, "%s", param_ptr->nmea_data);
                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        gps_config.receive_type = gps_config.receive_type | param_ptr->type;

                    }


                    if((gps_config.receive_type & ATC_NMEA_GGA) && (gps_config.receive_type & ATC_NMEA_GLL) && 
                        (gps_config.receive_type & ATC_NMEA_GSA) && (gps_config.receive_type & ATC_NMEA_GSV) && 
                        (gps_config.receive_type & ATC_NMEA_RMC) && (gps_config.receive_type & ATC_NMEA_VTG))
                    {
                        gps_config.nmea_type = ATC_NMEA_NONE;
                        gps_config.gps_cmd_type= 0;
                        MNGPS_OutputReq(dual_sys, 0, 1);
                        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                    }
                }
                else
                {
                    if(param_ptr->type == gps_config.nmea_type)
                    {
                        gps_config.nmea_type = ATC_NMEA_NONE;
                        gps_config.gps_cmd_type= 0;

                        SCI_TRACE_LOW("ATC:%s:%d,%d,%s", g_atc_info_table[AT_CMD_QGNSSRD].cmd_name->str_ptr, param_ptr->length, param_ptr->type,param_ptr->nmea_data);

                        sprintf((char*)g_rsp_str, "%s:%s", g_atc_info_table[AT_CMD_QGNSSRD].cmd_name->str_ptr, param_ptr->nmea_data);

                        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                        ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                    }

                }
            }
            else
            {
                SCI_TRACE_LOW("ATC:%s:%d,%d,%s", g_atc_info_table[AT_CMD_SPGPSOUT].cmd_name->str_ptr, param_ptr->length, param_ptr->type,param_ptr->nmea_data);

                sprintf((char*)g_rsp_str, "%s:%s", g_atc_info_table[AT_CMD_SPGPSOUT].cmd_name->str_ptr, param_ptr->nmea_data);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            
            break;
        }

#ifdef _SUPPORT_GPRS_
        case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
        {
            APP_MN_GPRS_EXT_T*param_ptr = (APP_MN_GPRS_EXT_T *)sig_ptr;
            uint8 nsapi = 5;
            uint8 pdp_id = 1;

            s_is_gps_act_pdp_req = FALSE;

            if((((APP_MN_GPRS_EXT_T*)sig_ptr)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)sig_ptr)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
            {
                nsapi = param_ptr->nsapi;
                pdp_id = param_ptr->pdp_id;
                
                SCI_TRACE_LOW("ATC: GPS PDP activate success pdp_id = %d, s_nsapi=%d", pdp_id, nsapi);

                sprintf((char*)g_rsp_str, "%s:%d, %d", g_atc_info_table[AT_CMD_SPGPSPDP].cmd_name->str_ptr, pdp_id, nsapi);

                MNGPS_SendPdpInfoReq(dual_sys, pdp_id, nsapi);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);

            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR,ERR_GPRS_OPERATION_FAILURE);
            }
            
            break;
        }
#endif

        case APP_MN_GPS_ERR_IND:
        {
            APP_MN_GPS_ERR_IND_T*param_ptr = (APP_MN_GPS_ERR_IND_T *)sig_ptr;

            sprintf((char*)g_rsp_str, "+GPS %d ERR", param_ptr->dual_sys);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            break;
        }

        case APP_MN_GPS_FIX_IND:
        {
            APP_MN_GPS_FIX_IND_T*param_ptr = (APP_MN_GPS_FIX_IND_T *)sig_ptr;
            float dif_longitude = 0.0;
            float dif_latitude = 0.0;           

            SCI_TRACE_LOW("ATC:SPGPSTTFF APP_MN_GPS_FIX_IND %f, %f, %d", param_ptr->longitude, param_ptr->latitude, param_ptr->ttff_time);
            if(gps_config.gps_cmd_type == ATC_SPGPSTTFF)
            {
                ATC_GpsStopTimer(dual_sys, ATC_FIX_TIMEOUT);

               dif_longitude = param_ptr->longitude - gps_config.ref_longitude;
               dif_latitude = param_ptr->latitude - gps_config.ref_latitude;
 
               sprintf((char*)g_rsp_str, "%s:%d,%f,%f,%f,%f,%d", g_atc_info_table[AT_CMD_SPGPSTTFF].cmd_name->str_ptr, gps_config.test_times,\
                param_ptr->longitude, param_ptr->latitude, dif_longitude, dif_latitude, param_ptr->ttff_time);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                if(gps_config.test_times < gps_config.fix_times)
                {
                      ATC_GpsStartTimer(dual_sys, ATC_FIX_TRACKING, gps_config.interval_time);
                }
                
                else
                {
                    ATC_GpsStopTimer(dual_sys, ATC_FIX_TRACKING);
                    ATC_GpsStopTimer(dual_sys, ATC_FIX_INTERVAL);
                    gps_config.test_times = 0;
                    
                    //sprintf((char*)g_rsp_str, "%s:%d", g_atc_info_table[AT_CMD_SPGPSTTFF].cmd_name->str_ptr, gps_config.test_times);

                    //ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK, 0);
                    break;
                
                }

            }
            break;
        }
        
        default:
        SCI_TRACE_LOW("ATC: ATC_ProcessGpsEvent event type error");
        break;
    }

#else
    return;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSPWR command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSPWR )
{
#ifdef _MNGPS_SUPPORT_
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                gps_config.gps_cmd_type = ATC_SPGPSPWR;
                if(PARA1.num == 1)
                {
                    MNGPS_DownloadReq(dual_sys);
                    
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_DOWNLOAD_CNF, 0xff, ATC_NO_DOMAIN);
                }
                else if(PARA1.num == 0)
                {
                    MNGPS_PowerOffReq(dual_sys);

                    ATC_GpsStopTimer(dual_sys, ATC_FIX_TIMEOUT);
                    ATC_GpsStopTimer(dual_sys, ATC_FIX_TRACKING);
                    ATC_GpsStopTimer(dual_sys, ATC_FIX_INTERVAL);

                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_POWEROFF_CNF, 0xff, ATC_NO_DOMAIN);                    
                }
                
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_STATUS_INFO_T  status_info ={0};

            MNGPS_GetStatusInfo(dual_sys, &status_info);
            
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPGPSPWR].cmd_name->str_ptr, status_info.is_gps_power_on);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSFIX command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSFIX )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                gps_config.gps_cmd_type = ATC_SPGPSFIX;

                if(1 == PARA1.num)
                {
                    MNGPS_StartGpsReq(dual_sys);

                   ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_START_CNF, 0xff, ATC_NO_DOMAIN);  
                }
                else
                {
                    MNGPS_StopGpsReq(dual_sys);

                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_STOP_CNF, 0xff, ATC_NO_DOMAIN);  
                }
            
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_STATUS_INFO_T  status_info ={0};

            MNGPS_GetStatusInfo(dual_sys, &status_info);
            
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPGPSFIX].cmd_name->str_ptr, status_info.is_fix_on);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSRST command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSRST )
{
#ifdef _MNGPS_SUPPORT_
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                gps_config.gps_cmd_type = ATC_SPGPSRST;

                gps_config.reset_mode = PARA1.num;
                MNGPS_ResetGpsReq(dual_sys, PARA1.num);
                
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_RESET_CNF, 0xff, ATC_NO_DOMAIN);                       
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_STATUS_INFO_T  status_info ={0};

            MNGPS_GetStatusInfo(dual_sys, &status_info);
            
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_SPGPSRST].cmd_name->str_ptr, status_info.reset_mode);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
    
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSSTATUS command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSSTATUS )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    switch (ATC_GET_CMD_TYPE)
    {

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_ReadStatusReq(dual_sys);

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                              (uint16)APP_MN_GPS_READ_STATUS_CNF, 0xff, ATC_NO_DOMAIN);    
            break;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSINF command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSINF )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                MNGPS_ReadInfoReq(dual_sys);

                ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_READ_INFO_CNF, 0xff, ATC_NO_DOMAIN);               
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            /*MNGPS_ReadStatusReq(dual_sys, &status_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                              (uint16)APP_MN_GPS_READ_STATUS_CNF, 0xff, ATC_NO_DOMAIN);  
            break;*/
            return ERR_GPS_NOT_SUPPORT;

            
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSOUT command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSOUT )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32 time_interval = 1;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {

                if(PARAM2_FLAG)
                {
                    time_interval = PARA2.num;
                }
         
                MNGPS_OutputReq(dual_sys, PARA1.num, time_interval);

                ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_OUTPUT_CNF, 0xff, ATC_NO_DOMAIN);           
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_STATUS_INFO_T  status_info ={0};

            MNGPS_GetStatusInfo(dual_sys, &status_info);
            
            sprintf((char*)g_rsp_str, "%s: %d, %d", g_atc_info_table[AT_CMD_SPGPSOUT].cmd_name->str_ptr, status_info.ouput_type, status_info.time_interval);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSPDP command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSPDP )
{
#if defined(_MNGPS_SUPPORT_) && defined(_SUPPORT_GPRS_)

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNGPRS_CODE_E   ret     = ERR_MNGPRS_NO_ERR;
    BOOLEAN             param_used[7]= {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
    uint32                  pdp_id = 1;         //param_used[0]
    uint8    pdp_type[MAX_PDP_TYPE_LEN] = {0};      //param_used[1]
    uint8    apn[MAX_APN_LEN+1] = {0};       // param_used[2]
    MN_GPRS_PDP_ADDR_T      pdp_addr = {0};  // param_used[3]
    uint8       d_comp = 0;                     // param_used[4]
    uint8       h_comp = 0;                     // param_used[5]
    uint8    pco[MAX_PCO_LEN+1] = {0};   // param_used[6]
    uint32  pdp_id_arr[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
    uint8 i = 0;


    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                if(1 == PARA1.num)
                {
                    if (PARAM2_FLAG)
                    {
                        pdp_id = PARA2.num;
                    }
                    
                    if (PARAM3_FLAG)
                    {
                        ATC_CUR_PARAM_STRING_T *type_ptr = PNULL;
                    
                        if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING)      
                        {
                            SCI_TRACE_LOW("ATC: GPS MNGPRS_SetPdpContextEx PDP TYPE FAIL1");
                            return ERR_GPS_INVALID_PARAMETER;
                        }    
                        
                        type_ptr = PARA3.str_ptr;
                        for (i = 0; i < type_ptr->str_len; i++)
                        {
                            /*lint -e718*/
                            /*lint -e746*/
                            /*lint -e628*/
                            if (isalpha(*(type_ptr->str_ptr+ i)))/*lint !e48*/
                            {
                                *(type_ptr->str_ptr + i) = toupper(*(type_ptr->str_ptr + i));
                            }
                            /*lint +e746*/
                            /*lint +e718*/
                        }

                        // support IP/IPV6/IPV4V6
                        if (stricmp((char *)type_ptr->str_ptr, "IP") && 
                            stricmp((char *)type_ptr->str_ptr, "IPV6") && 
                            stricmp((char *)type_ptr->str_ptr, "IPV4V6"))
                        {
                            SCI_TRACE_LOW("ATC: GPS MNGPRS_SetPdpContextEx PDP TYPE FAIL2");
                            return ERR_GPS_INVALID_PARAMETER;
                        }
                                  
                       SCI_MEMCPY(pdp_type, type_ptr->str_ptr, type_ptr->str_len);
                                   
                       pdp_type[type_ptr->str_len] = '\0';
                       
                    }
                    else
                    {
                        SCI_MEMCPY(pdp_type, pdp_ipv4, sizeof(pdp_ipv4));
                    }
    
                    // APN
                    if (PARAM4_FLAG)
                    {
                        ATC_CUR_PARAM_STRING_T *apn_ptr  =  PNULL;
                        
                        if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                        {
                            SCI_TRACE_LOW("ATC: GPS MNGPRS_SetPdpContextEx APN FAIL");
                            return ERR_GPS_INVALID_PARAMETER;
                        }      
                        apn_ptr = PARA4.str_ptr;
                        
                        SCI_MEMCPY(apn, apn_ptr->str_ptr, apn_ptr->str_len);

                        apn[apn_ptr->str_len] = '\0';
                    }
                    else
                    {
                        MN_PHONE_CURRENT_PLMN_INFO_T       register_status = {0};
                        register_status = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);
                        if((register_status.mcc == 460) &&(register_status.mnc == 0 ||register_status.mnc == 2 || register_status.mnc == 7))
                        {
                            SCI_MEMCPY(apn, apn_cmcc, sizeof(apn_cmcc));
                        }
                        else  if((register_status.mcc == 460) &&(register_status.mnc == 1))
                        {
                            SCI_MEMCPY(apn, apn_uicc, sizeof(apn_uicc));
                        }
                        else
                        {
                            param_used[2] = FALSE;
                        }

                    }
                    ret = MNGPRS_SetPdpContextEx(dual_sys, param_used,
                    pdp_id,
                    pdp_type,
                    apn,
                    pdp_addr,
                    d_comp,
                    h_comp,
                    pco);

                    if(ERR_MNGPRS_NO_ERR != ret)
                    {
                        SCI_TRACE_LOW("ATC: GPS MNGPRS_SetPdpContextEx fail ret=%d", ret);
                        return ERR_GPS_OPERATION_FAILED;
                    }

                    pdp_id_arr[0]=pdp_id;  
                    ret = MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_arr, POC_E, MN_UNSPECIFIED);

                    if(ERR_MNGPRS_NO_ERR != ret)
                    {
                        SCI_TRACE_LOW("ATC: GPS active pdp fail ret=%d", ret);
                        return ERR_GPS_OPERATION_FAILED;
                    }

                    s_is_gps_act_pdp_req = TRUE;

                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_ACTIVATE_PDP_CONTEXT_CNF, pdp_id_arr[0], ATC_DOMAIN_PS);  
                }
                else if(0 == PARA1.num)
                {
                    BOOLEAN is_all = FALSE;
                    
                    if (PARAM2_FLAG)
                    {
                        pdp_id_arr[0] = PARA2.num;
                    }
                    else
                    {
                        is_all = TRUE;
                    }

#ifdef _MUX_ENABLE_
                    if(ATC_Check_Expected_Event(atc_config_ptr->current_link_id, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF, (uint8)pdp_id_arr[0], dual_sys))
                    {
                        return ERR_GPS_INVALID_PARAMETER;
                    }
                    else
#endif
                    {
                        ret = MNGPRS_DeactivatePdpContextEx(dual_sys, is_all, pdp_id_arr);

                        if(ERR_MNGPRS_NO_ERR != ret)
                        {
                            return ERR_GPS_OPERATION_FAILED;
                        }


#ifdef _MUX_ENABLE_
                        if(is_all)
                        {
                            ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                             ATC_CHECKID_FOR_DEACTIVATE_ALL_PDP, ATC_DOMAIN_PS, dual_sys);
                        }
                        else
                        {
                            ATC_Add_Expected_Event(atc_config_ptr, APP_MN_DEACTIVATE_PDP_CONTEXT_CNF,
                                             pdp_id_arr[0], ATC_DOMAIN_PS, dual_sys);
                        }                   
#endif
                    }
                }
            }
            else
            {
                return ERR_GPS_INVALID_PARAMETER;
            }
            return  S_ATC_DEFAULT_HANDLE;
        }

        case ATC_CMD_TYPE_READ:
        {
            MN_GPRS_CON_STATE_T    *pdp_con_state_arr = PNULL;            
            uint32   pdp_context_num;                

            ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr,&pdp_context_num);
            
            if(ret != ERR_MNGPRS_NO_ERR)
            {
                if(pdp_con_state_arr != PNULL)
                {
                    SCI_FREE(pdp_con_state_arr);
                }
                
                return ERR_GPS_OPERATION_FAILED;
            }
            sprintf((char*)g_rsp_str, "%s: ", g_atc_info_table[AT_CMD_SPGPSPDP].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);

            for (i = 0; i < pdp_context_num; i++)
            {
                if (MN_CONTEXT_ACTIVATED == pdp_con_state_arr[i].pdp_state)
                {
                    SCI_TRACE_LOW("ATC: SPGPSPDP active pdp fail PDP_ID=%d", i);
                    //sprintf((char*)g_rsp_str+strlen((char*)g_rsp_str),"%d ", pdp_con_state_arr[i].pdp_id);
                    
                    sprintf((char*)g_rsp_str,"pdp_id:%d ", pdp_con_state_arr[i].pdp_id);
                    ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);

                }
            }
                        
            if(pdp_con_state_arr != PNULL)
            {
                SCI_FREE(pdp_con_state_arr);    
            }

            //ATC_BuildInfoRsp(atc_config_ptr,g_rsp_str);

            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSC command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSC )
{
#ifdef _MNGPS_SUPPORT_
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                if(PARA1.num == 1)
                {
                    gps_config.gps_cmd_type = ATC_QGNSSC;
                    MNGPS_DownloadReq(dual_sys);
                    
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_DOWNLOAD_CNF, 0xff, ATC_NO_DOMAIN);            
                }
                else if(PARA1.num == 0)
                {
                    gps_config.gps_cmd_type = 0;
                    MNGPS_PowerOffReq(dual_sys);
                    
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                      (uint16)APP_MN_GPS_POWEROFF_CNF, 0xff, ATC_NO_DOMAIN);       
                }
                
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            MNGPS_STATUS_INFO_T  status_info ={0};

            MNGPS_GetStatusInfo(dual_sys, &status_info);
            
            sprintf((char*)g_rsp_str, "%s: %d", g_atc_info_table[AT_CMD_QGNSSC].cmd_name->str_ptr, status_info.is_gps_power_on);

            ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            
            return S_ATC_SUCCESS;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSRD command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSRD )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint32 time_interval = 1;
    ATC_GPS_NMEA_TYPE_E type = 0;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {

                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return ERR_GPS_INVALID_PARAMETER;
                }

                if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[0]) )
                {
                    type = 1;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[1]))
                {
                    type = 2;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[2])) 
                {
                    type = 4;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[3])) 
                {
                    type = 8;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[4]))
                {
                    type = 16;
                }
                else if (0 == stricmp((char *)PARA1.str_ptr->str_ptr, s_gpstest_str[5])) 
                {
                    type = 32;
                }
                else
                {
                    type = 0;
                }

                gps_config.gps_cmd_type = ATC_QGNSSRD;
                gps_config.nmea_type = type;
                MNGPS_OutputReq(dual_sys, (uint16)type, time_interval);

                ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_OUTPUT_CNF, 0xff, ATC_NO_DOMAIN);   
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            gps_config.gps_cmd_type = ATC_QGNSSRD;
            gps_config.nmea_type = ATC_NMEA_ALL;
            MNGPS_OutputReq(dual_sys, type, time_interval);

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                                  (uint16)APP_MN_GPS_OUTPUT_CNF, 0xff, ATC_NO_DOMAIN);   
            
            break;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+QGNSSCMD command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessQGNSSCMD )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                sprintf((char*)g_rsp_str, "%s: NEMOG+GE2", g_atc_info_table[AT_CMD_QGNSSCMD].cmd_name->str_ptr);

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                
                return S_ATC_SUCCESS;
            }
            else
            {
                status = ERR_GPS_INVALID_PARAMETER;
            }
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            return ERR_GPS_NOT_SUPPORT;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSTTFF command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSTTFF )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG ||!PARAM2_FLAG )
            {
                return ERR_GPS_INVALID_PARAMETER;
            }

            gps_config.fix_times      = PARA1.num;
            gps_config.fix_timeout   = PARA2.num*1000;
            gps_config.tracking_time = PARA3.num*1000;           
            gps_config.interval_time = PARA4.num*1000; 


            SCI_TRACE_LOW("ATC: SPGPSTTFF fix_times=%d, fix_timeoutt=%d, tracking_time=%d, interval_time=%d",  
                gps_config.fix_times, gps_config.fix_timeout, gps_config.tracking_time, gps_config.interval_time);


            if(PARAM5_TYPE != ATC_CMD_PARAM_TYPE_STRING || PARAM6_TYPE != ATC_CMD_PARAM_TYPE_STRING )
            {
                return ERR_GPS_INVALID_PARAMETER;
            }

            gps_config.ref_longitude = (float)atof((char*)PARA5.str_ptr->str_ptr);
            gps_config.ref_latitude = (float)atof((char*)PARA6.str_ptr->str_ptr);

            sprintf((char*)g_rsp_str, "ATC: SPGPSTTFF gps_config.ref_longitude:%f, ref_latitude:%f", gps_config.ref_longitude, gps_config.ref_latitude);
            SCI_TRACE_LOW("%s",g_rsp_str);  

            gps_config.gps_cmd_type = ATC_SPGPSTTFF;
            MNGPS_ResetGpsReq(dual_sys, gps_config.reset_mode);

            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                              (uint16)APP_MN_GPS_RESET_CNF, 0xff, ATC_NO_DOMAIN);     
            
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            return ERR_GPS_NOT_SUPPORT;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSASSERT command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSASSERT )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG)
            {
                return ERR_GPS_INVALID_PARAMETER;
            }
            
            SCI_TRACE_LOW("ATC: ATC_ProcessSPGPSASSERT PARA1.num=%d",PARA1.num);

            gps_config.gps_cmd_type = ATC_SPGPSASSERT;

            MNGPS_SendAssertReq(dual_sys, PARA1.num);
              
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            /*MNGPS_ReadStatusReq(dual_sys, &status_info);
            ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                              (uint16)APP_MN_GPS_READ_STATUS_CNF, 0xff, ATC_NO_DOMAIN);  
            break;*/

            return ERR_GPS_NOT_SUPPORT;
           
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

/*****************************************************************************/
//  Description : This function handles the AT+SPGPSLTEOPEN command 
//  Global resource dependence : g_rsp_str
//  Author:       
//  Note: open or close the Ephemeris prediction function
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessSPGPSLTEOPEN )
{
#ifdef _MNGPS_SUPPORT_

    ATC_STATUS     status = S_ATC_SUCCESS;
    
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG)
            {
                return ERR_GPS_INVALID_PARAMETER;
            }
            
            SCI_TRACE_LOW("ATC: ATC_ProcessSPGPSLTEOPEN PARA1.num=%d",PARA1.num);

             //GPS_Lte_Open(PARA1.num); //w217 for compile
              
            break;
        }

        case ATC_CMD_TYPE_READ:
        {
            return ERR_GPS_NOT_SUPPORT;
        }

        default:
            break;
    }
    
    return status;
#else
    return S_ATC_FAIL;
#endif
}

#ifdef _MNGPS_SUPPORT_
/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsFixTimeout(
    uint32 lparam
)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ATC_CONFIG_T   *atc_config_ptr;
    
    SCI_TRACE_LOW("ATC: enter ATC_ProcessGpsFixTimeout");
    
    atc_config_ptr = &s_config_info; 

    gps_config.gps_cmd_type = ATC_SPGPSTTFF;
    MNGPS_ResetGpsReq(dual_sys, gps_config.reset_mode);

    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                  (uint16)APP_MN_GPS_RESET_CNF, 0xff, ATC_NO_DOMAIN);     
}


/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsTrackingTimeout(
    uint32 lparam
)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ATC_CONFIG_T   *atc_config_ptr;
    
    SCI_TRACE_LOW("ATC: enter ATC_ProcessGpsTrackingTimeout");
    
    atc_config_ptr = &s_config_info; 

    MNGPS_StopGpsReq(dual_sys);
   
    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                  (uint16)APP_MN_GPS_STOP_CNF, 0xff, ATC_NO_DOMAIN);    
}

/*****************************************************************************/
//  Description : This function handles timer callback
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void  ATC_ProcessGpsIntervalTimeout(
    uint32 lparam
)
{
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ATC_CONFIG_T   *atc_config_ptr;
    
    SCI_TRACE_LOW("ATC: enter ATC_ProcessGpsIntervalTimeout");
    atc_config_ptr = &s_config_info; 

    gps_config.gps_cmd_type = ATC_SPGPSTTFF;
    MNGPS_ResetGpsReq(dual_sys, gps_config.reset_mode);
       
    ATC_ADD_EXPECTED_EVENT(atc_config_ptr, 
                  (uint16)APP_MN_GPS_RESET_CNF, 0xff, ATC_NO_DOMAIN);    
}

/*****************************************************************************/
//  Description : This function handles timer 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_GpsStartTimer(MN_DUAL_SYS_E dual_sys, uint32 time_type, uint32 timer_value)
{
    SCI_TRACE_LOW("ATC: START ATC_GpsStartTimer time_type=%d", time_type);

    if(ATC_FIX_TIMEOUT == time_type)
    {
        if(NULL == s_gps_fix_timeout_timer_ptr)
        {
            s_gps_fix_timeout_timer_ptr = SCI_CreateTimer(
                                                 "ATC_GPS_FIX_TIMEOUT",
                                                 ATC_ProcessGpsFixTimeout,
                                                 (uint32)dual_sys,
                                                 timer_value,
                                                 SCI_NO_ACTIVATE
                                             );
        }

        if(SCI_IsTimerActive(s_gps_fix_timeout_timer_ptr))
        {
            SCI_DeactiveTimer(s_gps_fix_timeout_timer_ptr);
        }

        SCI_ChangeTimer(s_gps_fix_timeout_timer_ptr, ATC_ProcessGpsFixTimeout, timer_value);
        SCI_ActiveTimer(s_gps_fix_timeout_timer_ptr);

    }
    else if(ATC_FIX_TRACKING == time_type)
    {
        if(NULL == s_gps_tracking_timer_ptr)
        {
            s_gps_tracking_timer_ptr = SCI_CreateTimer(
                                                 "ATC_GPS_TRACKING_TIMEOUT",
                                                 ATC_ProcessGpsTrackingTimeout,
                                                 (uint32)dual_sys,
                                                 timer_value,
                                                 SCI_NO_ACTIVATE
                                             );
        }

        if(SCI_IsTimerActive(s_gps_tracking_timer_ptr))
        {
            SCI_DeactiveTimer(s_gps_tracking_timer_ptr);
        }

        SCI_ChangeTimer(s_gps_tracking_timer_ptr, ATC_ProcessGpsTrackingTimeout, timer_value);
        SCI_ActiveTimer(s_gps_tracking_timer_ptr);
    }
    else if(ATC_FIX_INTERVAL == time_type)
    {
        if(NULL == s_gps_interval_timer_ptr)
        {
            s_gps_interval_timer_ptr = SCI_CreateTimer(
                                                 "ATC_GPS_INTERVAL_TIMEOUT",
                                                 ATC_ProcessGpsIntervalTimeout,
                                                 (uint32)dual_sys,
                                                 timer_value,
                                                 SCI_NO_ACTIVATE
                                             );
        }

        if(SCI_IsTimerActive(s_gps_interval_timer_ptr))
        {
            SCI_DeactiveTimer(s_gps_interval_timer_ptr);
        }

        SCI_ChangeTimer(s_gps_interval_timer_ptr, ATC_ProcessGpsIntervalTimeout, timer_value);
        SCI_ActiveTimer(s_gps_interval_timer_ptr);
    }
    else
    {
        return;
    }
        
    return;
}

/*****************************************************************************/
//  Description : This function handles timer 
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
LOCAL void ATC_GpsStopTimer(MN_DUAL_SYS_E dual_sys, uint32 time_type)
{
    SCI_TRACE_LOW("ATC: STOP ATC_GpsStopTimer time_type=%d", time_type);

    if(ATC_FIX_TIMEOUT == time_type)
    {
        if(PNULL != s_gps_fix_timeout_timer_ptr)
        {
            SCI_DeleteTimer(s_gps_fix_timeout_timer_ptr);
            s_gps_fix_timeout_timer_ptr = PNULL;
        }

    }
    else if(ATC_FIX_TRACKING == time_type)
    {

        if(PNULL != s_gps_tracking_timer_ptr)
        {
            SCI_DeleteTimer(s_gps_tracking_timer_ptr);
            s_gps_tracking_timer_ptr = PNULL;
        }

    }
    else if(ATC_FIX_INTERVAL == time_type)
    {
 
        if(PNULL != s_gps_interval_timer_ptr)
        {
            SCI_DeleteTimer(s_gps_interval_timer_ptr);
            s_gps_interval_timer_ptr = PNULL;
        }

    }
    else
    {
        return;
    }
        
    return;
}
#endif

