/******************************************************************************
** File Name:      sim_main.h                                                *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file of sim task                       *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_MAIN_H_
#define _SIM_MAIN_H_
#ifdef   __cplusplus
extern   "C"
{
#endif
/*#define SIM_RESET_CARD_TIMES      4
//all the ef setting in the SIM card
typedef struct
{
    BOOLEAN             card_present;   //the SIM card present in the seat or not
    uint16              t_protocol;     //the t protocol of the SIM card
    TB_SIM_INSERT_STATUS_E card_insert_status; //the sim card inserted state
    SIM_RECORD_EF_INFO_T   adn_info;        //the EF_ADN setting
    SIM_RECORD_EF_INFO_T   fdn_info;        //the EF_FDN setting
    SIM_RECORD_EF_INFO_T   msisdn_info; //the EF_MSISDN settting
    SIM_RECORD_EF_INFO_T   lnd_info;        //the EF_LND setting
    SIM_RECORD_EF_INFO_T   sdn_info;        //the EF_SDN settting
    SIM_RECORD_EF_INFO_T   bdn_info;        //the EF_BDN setting
    SIM_RECORD_EF_INFO_T   smsp_info;       //the EF_SMSP setting
    SIM_SMS_SETTING_T   sms_info;       //the EF_SMS setting
    SIM_PHASE_E         card_phase;     //the Phase of the SIM card
    SIM_EF_MAPPING_SST_T service_table;  //the service table of the SIM card
    SIM_GSM_SPECIFIC_DATA_T gsm_data;      //the GSM specific data of the SIM card
}SIM_CARD_SETTING_T;
*/
typedef struct
{
    SIGNAL_VARS
} SIM_SIG_CLOCK_STOP_IND_T;
#ifdef   __cplusplus
}
#endif
#endif //_SIM_MAIN_H_
