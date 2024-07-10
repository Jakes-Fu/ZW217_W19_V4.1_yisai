/******************************************************************************
 ** File Name:    atc_orange_prod.c                                                           *
 ** Author:         longting.zhao                                                                 *
 ** Date:           22/10/2012                                                                    *
 ** Copyright:     Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                    All Rights Reserved.                                                         *
 **                    This software is supplied under the terms of a license           *
 **                    agreement or non-disclosure agreement with Spreadtrum.     *
 **                    Passing on and copying of this document,and communication *
 **                    of its contents is not permitted without prior written             *
 **                    authorization.                                                                   *
 ** Description:   This file includes implementation of all AT commands            *
**                      demmanded by orange                                                      *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 22/10/2012  longting.zhao     Create.                                   *
 ******************************************************************************/

#include "atc_orange_prod.h"
#include "chg_drvapi.h"
#include "mnclassmark_api.h"
#include "version.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/

extern RSP_RESULT_STR     g_rsp_str;
extern MN_DUAL_SYS_E      g_current_card_id;

extern void ConvertBcdToDigitalStr(
                uint8         length,
                uint8         *bcd_ptr,      // in: the bcd code string
                uint8         *digital_ptr   // out: the digital string
                );

/*****************************************************************************/
//  Description : This function handle the AT+GMI command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMI )
{
    char    *manufactory_str = PNULL;

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        // get the manufactory name
        manufactory_str = (char*)CLASSMARK_GetManufacterName();
        SCI_ASSERT(PNULL != manufactory_str);
        sprintf((char*)g_rsp_str, "%s", manufactory_str);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        break;

    default:
        SCI_ASSERT(FALSE);
        break;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+GSN command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGSN )
{
    MN_IMEI_T           imei_arr; // the imei array
    uint8               temp_str[20];


    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        // get the manufactory name
        MNNV_GetIMEIEx(g_current_card_id, imei_arr);        

        ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_str);
        sprintf((char*)g_rsp_str, "%s",temp_str);    
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        break;

    default:
        SCI_ASSERT(FALSE);
        break;
    }
    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handle the AT+GMR command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessGMR )
{
    if(ATC_CMD_TYPE_EXECUTE == ATC_GET_CMD_TYPE)
    {
        // get the software version
#ifndef WIN32
        {
            uint8*    cur_str = PNULL;
            uint16    str_len = 0;
            
            sprintf((char*)g_rsp_str, "%s", COMMON_GetProjectVersionInfo());
            
            cur_str = g_rsp_str;
            while (*cur_str != '\0')
            {
                if (*cur_str == '\n')
                {
                    str_len = strlen((char *)g_rsp_str);
                    memmove((void *)(cur_str + 1), (void *)cur_str, strlen((char *)cur_str));
                    g_rsp_str[str_len + 1] = '\0';
                    *cur_str = '\r';
                    cur_str +=1;
                }
                cur_str++;
            }
        }
#endif
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        return S_ATC_SUCCESS;
    }
    else
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }
}

/*****************************************************************************/
//  Description : This function handle the AT$NWRAT command 
//                 select a new network type
//  Global resource dependence : none
//  Author: Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessDOLLAR_NWRAT)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    MN_PHONE_USER_SERVICE_SELECT_E    service_type = MN_PHONE_USER_SELECT_NONE;

    switch(ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if (!PARAM1_FLAG || !PARAM2_FLAG)
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }

        if ((PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
             ||( PARAM2_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC))
        {
            return ERR_INVALID_INDEX;
        }

        if (PARA1.num >= 2)
        {
            return ERR_OPERATION_NOT_SUPPORTED;
        }

        switch (PARA1.num)
        {
        //SELECT GSM ONLY
        case 0:
            if (PARA2.num == 1)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else
            {
                service_type = MN_PHONE_USER_SELECT_GSM_ONLY;
            }
            break;
        //SELECT BOTH GSM AND GPRS
        case 1:
            if (PARA2.num != 2)
            {
                return ERR_OPERATION_NOT_SUPPORTED;
            }
            else
            {
                service_type = MN_PHONE_USER_SELECT_GSM_GPRS_BOTH;
            }
            break;
        default :
            status = ERR_OPERATION_NOT_SUPPORTED;
        }

        MNPHONE_SetServiceTypeEx(g_current_card_id, service_type);
        break;

    case ATC_CMD_TYPE_READ:
        switch(MNPHONE_GetServiceTypeEx(g_current_card_id))
        {
        case MN_PHONE_USER_SELECT_GSM_ONLY:
            sprintf((char*)g_rsp_str, "$NWRAT: %d", 0);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        
        case MN_PHONE_USER_SELECT_GPRS_ONLY:
        case MN_PHONE_USER_SELECT_GSM_GPRS_BOTH:
            sprintf((char*)g_rsp_str, "$NWRAT: %d", 1);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        
        case MN_PHONE_USER_SELECT_NONE:
        default:
            status = ERR_NO_NETWORK_SERVICE;
            break;
        }
        break;        

    case ATC_CMD_TYPE_TEST:
        sprintf((char*)g_rsp_str, "$NWRAT: (0-1),(0-2)");
        ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        break;

    default:
        SCI_ASSERT(FALSE);
        break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handle the AT+CBC command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCBC )
{
    uint32        bcs = 0;

#if 0
    uint32        bcl = 0;
    // call the api for the battery
    bcs = CHR_CheckBatteryStaus();
    switch ( bcs )
    {
        case CHR_BATTERY_NONE_S:
        {
             bcs = AT_CBC_NO;
             break;
        }

        case CHR_BATTERY_NORMAL_S:
        {
             bcs = AT_CBC_NO_CHARGE;
             break;
        }

        case CHR_BATTERY_CHARGING_S:
        {
             bcs = AT_CBC_CHARGE;
             break;
        }

        default:
        {
             //SCI_TRACE_LOW:"ATC: Assert CBC,invalid type."
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_BASIC_CMD_9247_112_2_17_22_16_48_360,(uint8*)"");
             break;
        }
    }
    bcl = CHR_GetBatCapacity();
#endif
    char str_tmp[14]        = {0};
    CHGMNG_STATE_INFO_T *p_chgmng_info = NULL;
    p_chgmng_info = CHGMNG_GetModuleState();
    
    if(CHGMNG_STARTING==p_chgmng_info->chgmng_state
      ||CHGMNG_CHARGING==p_chgmng_info->chgmng_state
      ||CHGMNG_PULSECHARGING==p_chgmng_info->chgmng_state)
    {
        bcs = AT_CBC_CHARGE;
    }
    // only battery, no charger connector
    else if(CHGMNG_IDLE == p_chgmng_info->chgmng_state)
    {
        bcs = AT_CBC_NO_CHARGE;
    }
    
    sprintf(str_tmp, "%d.%d", (p_chgmng_info->bat_cur_vol)/1000, (p_chgmng_info->bat_cur_vol)%1000/10);

    sprintf((char*)g_rsp_str, "+CBC: %ld,%s", bcs, str_tmp);
    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

    return S_ATC_SUCCESS;
}

/*****************************************************************************/
//  Description : This function handles the AT+CBST command
//  Global resource dependence : g_rsp_str
//  Author:       Wanghui
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCBST)
{
    ATC_STATUS     status = S_ATC_SUCCESS;
    uint8    atc_speed = 0;
    uint8    atc_name = 0;
    uint8    atc_ce = 0;

    SCI_TRACE_LOW( "ATC: ATC_ProcessCBST" );

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_SET:
        if(PARAM1_FLAG && PARAM2_FLAG && PARAM3_FLAG)
        {
        /*
            if(MN_RETURN_SUCCESS == MNCALL_SetBSType(PARA1.num,PARA2.num,PARA3.num))
            {
                status = S_ATC_SUCCESS;
            }
            else
            {
                status = ERR_INVALID_INDEX;
            }
        */
        }
        else
        {
            return ERR_OPERATION_NOT_ALLOWED;
        }
        break;

    case ATC_CMD_TYPE_READ:
        //MNCALL_GetBSType(&atc_speed,&atc_name,&atc_ce);
        sprintf((char*)g_rsp_str, "+CBST: %d,%d,%d", atc_speed,atc_name,atc_ce);
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        sprintf((char*)g_rsp_str, "+CBST:(84),(1),(0)");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    default:
        status = ERR_OPERATION_NOT_ALLOWED;
        break;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function handles the AT+GCAP command
//  Global resource dependence : g_rsp_str
//  Author:       Elvis Xu
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessGCAP)
{
    ATC_STATUS     status = S_ATC_SUCCESS;

    SCI_TRACE_LOW( "ATC: ATC_ProcessGCAP" );

    switch (ATC_GET_CMD_TYPE)
    {
    case ATC_CMD_TYPE_EXECUTE:
        sprintf((char*)g_rsp_str, "+CGAP: +CGSM");
        ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
        break;

    case ATC_CMD_TYPE_TEST:
        break;

    default:
        return ERR_OPERATION_NOT_ALLOWED;
    }

    return status;
} 