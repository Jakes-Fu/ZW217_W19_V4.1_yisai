/******************************************************************************
** File Name:      sim_global_variable.h                                     *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the header file that define global variable of    *
**                 SIM task                                                  *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2002.2         Raislin.Kong     Create                                    *
******************************************************************************/

#ifndef _SIM_GLOBAL_VARIABLE_H_
#define _SIM_GLOBAL_VARIABLE_H_

#ifdef   __cplusplus
extern   "C"
{
#endif

#define SIM_RESET_CARD_TIMES        4

#ifdef GSM_CUSTOMER_AFP_SUPPORT
#define SIM_USIM_PB_SET_NUM 2//Samsung
#else
#define SIM_USIM_PB_SET_NUM 4
#endif

typedef struct SIM_MNC_LEN_INFO_T_s
{
    BOOLEAN                 mnc_len_exist;  //TRUE      "length of MNC in the IMSI" exist
    uint8                   mnc_digit_num;  //default value is 2 digits
} SIM_MNC_LEN_INFO_T;

//all the ef setting in the SIM card
typedef struct
{
    BOOLEAN                 card_present;       //the SIM card present in the seat or not
    uint16                  t_protocol;         //the t protocol of the SIM card
    SIMDRV_INSERT_STATUS_E  card_insert_status; //the SIM card inserted state
    SIM_RECORD_EF_INFO_T    adn_info;           //the EF_ADN setting
    SIM_RECORD_EF_INFO_T    fdn_info;           //the EF_FDN setting
    SIM_RECORD_EF_INFO_T    msisdn_info;        //the EF_MSISDN setting
    SIM_RECORD_EF_INFO_T    lnd_info;           //the EF_LND setting
    SIM_RECORD_EF_INFO_T    sdn_info;           //the EF_SDN setting
    SIM_RECORD_EF_INFO_T    bdn_info;           //the EF_BDN setting
    SIM_RECORD_EF_INFO_T    smsp_info;          //the EF_SMSP setting

    SIM_SMS_SETTING_T       sms_info;           //the EF_SMS setting
    SIM_PHASE_E             card_phase;         //the Phase of the SIM card
    SIM_EF_MAPPING_SST_T    service_table;      //the service table of the SIM card
    SIM_GSM_SPECIFIC_DATA_T gsm_data;           //the GSM specific data of the SIM card
    SIM_MNC_LEN_INFO_T      mnc_len_info;       //the info of "length of MNC in the IMSI"
    SIM_EF_MAPPING_LOCI_T   loci_info;
    SIM_EF_MAPPING_IMSI_T   imsi_mapping;
} SIM_CARD_SETTING_T;

//#define CURRENT_SIM_TASK  ((SCI_IDENTIFY_THREAD() == P_SIM) ? 0:1)

//the global variables that defined in sim_main.c
extern SIM_CARD_SETTING_T   g_sim_card_setting[SIM_CARD_NUM];   //the setting of SIM card
extern uint32   g_sim_task_id[SIM_CARD_NUM];//the sim layer's taks id
extern uint32 g_sim_info_task_id[SIM_CARD_NUM];
extern SIM_CARD_STATE_E g_sim_card_state[SIM_CARD_NUM];//the sim card's state
extern uint8 g_sim_cphs_vm_record_len[SIM_CARD_NUM];
extern uint8 g_sim_r4_vm_record_len[SIM_CARD_NUM];
extern uint8 g_sim_r4_vm_mbi_record_len[SIM_CARD_NUM];
extern uint8 g_sim_r4_vm_mwis_record_len[SIM_CARD_NUM];
extern uint8 g_sim_r4_vm_mwis_record_num[SIM_CARD_NUM];
extern uint8 g_sim_r4_vm_vmwflag_len[SIM_CARD_NUM];

#ifdef SIM_SIMULATOR
extern SIMDRV_INSERT_STATUS_E g_sim_insert_status;
#endif

#ifdef SIM_TEST_IN_WIN32
extern BOOLEAN g_is_sim_card_inserted[SIM_CARD_NUM];
extern BOOLEAN g_is_mobile_power_on[SIM_CARD_NUM];
#endif

/*function:get data from data from bit_from to bit_to .
bit code from 1,not contain the bit that bit_from point*/
/*
8 7 6 5 4 3 2 1 0
   bit          from
*/

#define SIM_GET_DATA_BETWEEN(_data, _bit_from, _bit_number,_data_out)   \
{\
    SCI_ASSERT((_bit_from) + (_bit_number) <= 8); /*assert verified */ \
    _data_out = ((uint8) (((_data) & ((1 << (_bit_from + _bit_number)) - (1<< (_bit_from)))) >> (_bit_from))); \
}


/*function:insert data_in into data_out,return data_out
0<=bit_from <= bit_to<=8 */

/*#define SIM_SET_DATA_BETWEEN_8(_data_out, _data_in, _bit_from, _bit_number)  \
{ \
    int temp1,temp2,temp3;\
    SCI_ASSERT(_data_in < (uint8)(1 << (_bit_number))) ;  \
    SCI_ASSERT(_bit_from + _bit_number <= 8);  \
    temp1 = *(_data_out)& mask_bit[8- _bit_from - _bit_number];\
    temp2 = _data_in << (8- _bit_from - _bit_number );\
    temp3 = *(_data_out)& mask_bit_reserve[_bit_from];\
    *(_data_out) = temp1 + temp2 + temp3;\
}*/

typedef struct
{
    BOOLEAN cphs_optional_flag;
    SIM_EF_MAPPING_CPHSINFO_T cphsoptionalinfo;
} SIM_CARD_CPHSINFO_T;
extern SIM_CARD_CPHSINFO_T g_sim_cphsinfo[SIM_CARD_NUM];

extern BOOLEAN s_sim_is_ext_dualsim;
extern uint32 s_sim_ldo_stable_time;

extern uint32 s_sim_sim_poll_time[SIM_CARD_NUM];
extern uint32 s_sim_call_poll_time[SIM_CARD_NUM];
extern uint32 s_sim_stk_poll_time[SIM_CARD_NUM];

#ifdef SIM_3G_USIM_SUPPORT
extern SIM_APPLICATION_E s_sim_is_usim[SIM_CARD_NUM];
extern SIM_AID_S s_sim_usim_aid[SIM_CARD_NUM];
extern BOOLEAN s_sim_usim_adf_file_id_flag[SIM_CARD_NUM];
extern uint8 s_sim_usim_ecc_alpha_id_len[SIM_CARD_NUM];
extern BOOLEAN s_sim_gain_chv_remain_times_flag[SIM_CARD_NUM];
extern uint8 g_sim_netpar_file_size[SIM_CARD_NUM];
#endif

extern BOOLEAN s_sim_cdma_flag[SIM_CARD_NUM];
#ifdef SIM_CDMA_CARD_SUPPORT
extern BOOLEAN s_sim_cdma_only_flag[SIM_CARD_NUM];
#endif

extern uint8 s_sim_dn_ext1_num[SIM_CARD_NUM];
extern uint8 s_sim_dn_ext2_num[SIM_CARD_NUM];
extern uint8 s_sim_dn_ext3_num[SIM_CARD_NUM];

#ifdef GSM_DPHONE_SUPPORT
extern BOOLEAN s_sim_gsm_dphone_support[SIM_CARD_NUM];
extern uint8 s_dphone_reset_times[SIM_CARD_NUM];
extern SIMAT_SIG_REFRESH_TYPE_E s_dphone_refresh_type[SIM_CARD_NUM];
#endif

#ifdef _CPHS_SUPPORT
extern BOOLEAN s_sim_df_orange_exist[SIM_CARD_NUM];
#endif

extern uint8 s_sim_max_reset_times;

#ifdef GSM_DPHONE_VER2_SUPPORT
extern BOOLEAN s_sim_is_dphone_operation_skip[SIM_CARD_NUM];
#endif

#ifdef   __cplusplus
}
#endif
#endif  //_SIM_GLOBAL_VARIABLE_H_
