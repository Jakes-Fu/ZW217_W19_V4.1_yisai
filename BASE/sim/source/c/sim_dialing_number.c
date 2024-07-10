/******************************************************************************
** File Name:      sim_dialling_number.c                                     *
** Author:         Raislin.Kong                                              *
** Date:           2002.2                                                    *
** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
**                 All Rights Reserved.                                      *
**                 This software is supplied under the terms of a license    *
**                 agreement or non-disclosure agreement with Spreadtrum.    *
**                 Passing on and copying of this document,and communication *
**                 of its contents is not permitted without prior written    *
**                 authorization.                                            *
** Description:    This is the C file of dialling sim signal process         *
**                 functions                                                 *
******************************************************************************

******************************************************************************
**                        Edit History                                       *
** --------------------------------------------------------------------------*
** Date           Name             Description                               *
**---------------------------------------------------------------------------*
** 2001.2         Raislin.Kong     Create                                    *
** 2005.6         Yongxia.zhang    CR26616,modify SIMDN_ReadDN()             *
**                                 and SIMDN_UpdateDN()                      *
******************************************************************************/
#include "base_trc.h"
#include "sim_include.h"

#ifdef   __cplusplus
extern   "C"
{
#endif

#define SIM_DELETE_REAL_EXT//for Orange test

#define SIM_ADN_SERACH_SUPPORT

#ifdef SIM_SELF_TEST_PB_EXT
//NOKIA can support 48 numbers, that means 1 DN + 2 EXT, that means 30 bytes. (It use 25 bytes only)
#define SIM_SELF_TEST_PB_DN_LEN 20//bytes
#endif

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_ADN_SERACH_SUPPORT
uint8 *s_sim_adn_search_result[SIM_CARD_NUM] = {PNULL};
#endif
#endif

uint8 s_sim_dn_ext1_num[SIM_CARD_NUM] = {0};
uint8 s_sim_dn_ext2_num[SIM_CARD_NUM] = {0};
uint8 s_sim_dn_ext3_num[SIM_CARD_NUM] = {0};

typedef struct SIMDN_EXT_INFO_T_s
{
    uint16   dn_max_num;
    uint8   *ext_list_ptr;
} SIMDN_EXT_INFO_T;

STATIC SIMDN_EXT_INFO_T     s_lnd_ext_info[SIM_CARD_NUM];
STATIC SIMDN_EXT_INFO_T     s_adn_ext_info[SIM_CARD_NUM];
STATIC SIMDN_EXT_INFO_T     s_fdn_ext_info[SIM_CARD_NUM];
STATIC SIMDN_EXT_INFO_T  s_msisdn_ext_info[SIM_CARD_NUM];
STATIC SIMDN_EXT_INFO_T  s_sdn_ext_info[SIM_CARD_NUM];


//SIM_MAX_DN_NUMBER_LEN is 20(bytes) now(2010/5/1), 也就是说底层最多
//支持读取SIM PB 40 个数字，也即一条DN(10 bytes) + 一条EXT(11 bytes)。
//目前我把SIM_MAX_EXT_NUM 定为2，即假定底层记录EXT id
//可以记录2 条。
//目前看来NOKIA 最高只到50位数字,即一条DN + 两条EXT
//BTW，由于MMI限制，目前我们仅支持 存入20个数字(10bytes, only DN)
//#define SIM_MAX_EXT_NUM 2
#define SIM_MAX_EXT_NUM 1// 1 DN_rec(10bytes) + 1 EXT_rec(10 bytes)


#ifndef SIM_CODE_COMPACT
/*@zhangyongxia,CR4266,2003-8-29*/
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN prev_purge_fail[SIM_CARD_NUM][4] = {{FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}; //CR4266
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN prev_purge_fail[SIM_CARD_NUM][4] = {{FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}; //CR4266
#else
static BOOLEAN prev_purge_fail[SIM_CARD_NUM][4] = {{FALSE, FALSE, FALSE, FALSE}, {FALSE, FALSE, FALSE, FALSE}}; //CR4266
#endif
#endif

static const uint16 s_dn_info_arr[SIM_DN_T_NUM][5] =
{
    //------------------------------------------------------------------------------------------------
    //DN File Name              DN Service type         Extension File name     Extension service type note:File_Name
    //------------------------------------------------------------------------------------------------
    {(uint16)SIM_FILE_EF_ADN, (uint16)SIMSER_PH2_ADN, (uint16)SIM_FILE_EF_EXT1, (uint16)SIMSER_PH2_EXT1, 0},
    {(uint16)SIM_FILE_EF_FDN, (uint16)SIMSER_PH2_FDN, (uint16)SIM_FILE_EF_EXT2, (uint16)SIMSER_PH2_EXT2, 1},
    {(uint16)SIM_FILE_EF_MSISDN, (uint16)SIMSER_PH2_MSISDN, (uint16)SIM_FILE_EF_EXT1, (uint16)SIMSER_PH2_EXT1, 0},
    {(uint16)SIM_FILE_EF_LND, (uint16)SIMSER_PH2_LND, (uint16)SIM_FILE_EF_EXT1, (uint16)SIMSER_PH2_EXT1, 0},
    {(uint16)SIM_FILE_EF_SDN, (uint16)SIMSER_PH2_SDN, (uint16)SIM_FILE_EF_EXT3, (uint16)SIMSER_PH2_EXT3, 2},
    {(uint16)SIM_FILE_EF_BDN, (uint16)SIMSER_PH2_BDN, (uint16)SIM_FILE_EF_EXT4, (uint16)SIMSER_PH2_EXT4, 3},
};

#ifdef SIM_3G_USIM_SUPPORT
static uint8 s_sim_usim_telecom_pb_adn_num[SIM_CARD_NUM] = {0};
static uint8 s_sim_usim_telecom_pb_adn_records_num[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};//Each EFadn have different records num
static uint16 s_sim_usim_telecom_pb_adn_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};//the DFtele/DFpb/EFadn file_id
static uint16 s_sim_usim_telecom_pb_pbc_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_grp_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_anra_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_anrb_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_anrc_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_sne_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_uid_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_emaila_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_emailb_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_iap_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_aas_id[SIM_CARD_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_gas_id[SIM_CARD_NUM] = {0};
static uint16 s_sim_usim_telecom_pb_ccp1_id[SIM_CARD_NUM] = {0};
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_sim_is_telecom_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
static uint16 s_sim_usim_telecom_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A, 0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
static BOOLEAN s_sim_is_other_dn_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE}; //except EFadn, other(msisdn fdn sdb bdn ext2/3) is in DFtele or ADFusim
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_sim_is_telecom_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
static uint16 s_sim_usim_telecom_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
static BOOLEAN s_sim_is_other_dn_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE, TRUE}; //except EFadn, other(msisdn fdn sdb bdn ext2/3) is in DFtele or ADFusim
#else
static BOOLEAN s_sim_is_telecom_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE};
static uint16 s_sim_usim_telecom_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
static BOOLEAN s_sim_is_other_dn_in_dftel[SIM_CARD_NUM] = {TRUE, TRUE}; //except EFadn, other(msisdn fdn sdb bdn ext2/3) is in DFtele or ADFusim
#endif

static uint8 s_sim_usim_adfusim_pb_adn_num[SIM_CARD_NUM] = {0};
static uint8 s_sim_usim_adfusim_pb_adn_records_num[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};//Each EFadn have different records num
static uint16 s_sim_usim_adfusim_pb_adn_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};//the DFtele/DFpb/EFadn file_id
static uint16 s_sim_usim_adfusim_pb_pbc_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_grp_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_anra_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_anrb_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_anrc_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_sne_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_uid_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_emaila_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_emailb_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_iap_id[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_aas_id[SIM_CARD_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_gas_id[SIM_CARD_NUM] = {0};
static uint16 s_sim_usim_adfusim_pb_ccp1_id[SIM_CARD_NUM] = {0};
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_sim_is_adfusim_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE, FALSE};
static uint16 s_sim_usim_adfusim_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A, 0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_sim_is_adfusim_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE, FALSE};
static uint16 s_sim_usim_adfusim_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
#else
static BOOLEAN s_sim_is_adfusim_pb_exist[SIM_CARD_NUM] = {FALSE, FALSE};
static uint16 s_sim_usim_adfusim_pb_ext1_id[SIM_CARD_NUM] = {0x4F4A, 0x4F4A};//the DFtele/DFpb/EFext1 file_id
#endif

//if the phone book is exist in MF->DFtelecom->EFadn
#ifdef MULTI_SIM_SYS_QUAD
static BOOLEAN s_sim_is_telecom_adn_exist[SIM_CARD_NUM] = {TRUE, TRUE, TRUE, TRUE};
#elif defined (MULTI_SIM_SYS_TRI)
static BOOLEAN s_sim_is_telecom_adn_exist[SIM_CARD_NUM] = {TRUE, TRUE, TRUE};
#else
static BOOLEAN s_sim_is_telecom_adn_exist[SIM_CARD_NUM] = {TRUE, TRUE};
#endif
static uint8 s_sim_telecom_adn_adn_records_num[SIM_CARD_NUM][SIM_USIM_PB_SET_NUM] = {0};/*pclint !e551*/
#endif

#ifdef SIM_FILL_PHONEBOOK_FULL
static BOOLEAN s_sim_write_pb_full[4] = {FALSE,FALSE,FALSE,FALSE};
static BOOLEAN s_sim_write_pb_full_begin[4] = {FALSE,FALSE,FALSE,FALSE};
#endif
//----------------------Local function declare--------------------------

/********************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/********************************************************************************/
STATIC uint8 GetUsedExtEntrysInDnFile(                                  //RETURN: the number of used ext entry in dn file
    uint32 cur_sim_task,
    BOOLEAN           *used_ext_flag, //IN/OUT:
    SIM_DIALLING_NUM_TYPE_E dn_type    //IN:
);

/**********************************************************************
//    Description:
//      Get the dialling number file name of the dialing number type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E GetDNFileName(   //return value:the file name of the type dialling numer
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
);

/**********************************************************************
//    Description:
//      Get the extension file name of the dialing number type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E GetDNExtFileName(    //return value:the extension file name of the type dialling numer
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
);

/**********************************************************************
//    Description:
//      Get the dialing number service index in the SIM service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_TYPE_PH_2_E GetDNService(    //return value:the dialling number service index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
);

/**********************************************************************
//    Description:
//      Get the dialing number service index in the SIM service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC int16 GetDNPurgeFlagIndex(    //return value:purge flag index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
);
/**********************************************************************
//    Description:
//      Get the dialing number extension service index in the SIM
//    service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_TYPE_PH_2_E GetDNExtService( //return value:the dialling number extension service index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
);

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function access all the EFs that reference to the same EFext for
//    storage and indentify records in these file using extension data.the
//    existing chains have to be followed to the end.all referred extension
//    reords are noted.akk the extension records not noted are then marked
//    as "free" the set to be '0xff'
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:Reference to section 11 of GSM 11.11
***********************************************************************/
STATIC SIM_DN_OPERATION_RESULT_E PurgeDiallingNumber(   //return value:the purge opreate result
    uint32 cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E dn_type     //the type of the dialling number
);
#endif

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/********************************************************************************/
STATIC SIM_CCP_OPERATION_RESULT_E ReadCCPEntry(
    uint32               cur_sim_task,   //IN:
    uint8                ccp_id,         //IN:
    SIM_EF_MAPPING_CCP_T *ccp_ptr        //OUT:
);

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/********************************************************************************/
STATIC SIMDN_EXT_INFO_T *GetExtInfoPtr(
    uint32                   cur_sim_task,   //IN:
    SIM_DIALLING_NUM_TYPE_E  dn_type         //IN:
);

/**********************************************************************
//    Description:
//      Seek serveral empty corresponding extension record for the dialling
//    number,when there are not enough empty extension ,first purge the
//    extension file then seek again.
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_DN_OPERATION_RESULT_E SeekEmptyExtRecord(   //return value:the operate result of
    uint32 cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E dn_type,     //the type of the dialling number
    uint8 record_num,               //the empty ext number want to find
    uint8 *id_arr                   //the array to save the empty ext id
);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the 3G USIM FDN service active/deactive. 31.102 5.3.2
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMDN_USIMFDNService(
    uint32 cur_sim_task,
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type
);

/**********************************************************************
//    Description:
//      the function that process the 3G USIM BDN service active/deactive. 31.102 5.3.2
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMDN_USIMBDNService(
    uint32 cur_sim_task,
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type
);
#endif

/**********************************************************************
//    Description:
//      If a PB record' name is empty, copy the phone number to the name.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void SIMDN_ReMapEmptyNameDN(uint32 cur_sim_task, SIM_EF_MAPPING_DN_T *dn_mapping);

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E SIMDN_RemapDFpbADN(uint32 cur_sim_task, uint16 adn_rec_index, uint8 *remaped_adn_rec_index);

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
STATIC uint8 SIMDN_IsExtidUsedbyOtherDnid(
    uint32 cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E ext_service,
    uint8 ext_id);

#ifdef SIM_ADN_SERACH_SUPPORT
/**********************************************************************
//    Description:
//      Set EFadn search results, if the record equal to "FF", we do not need to read it
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC void SIMDN_SetADNSearchResult(
    uint32 cur_sim_task,
    uint16 empty_rec_num,
    uint16 adn_set_offset,
    uint8 *empty_rec);
#endif

STATIC void SIMDN_SetADNService(uint32 cur_sim_task);
#endif

/********************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/********************************************************************************/
STATIC uint8 GetUsedExtEntrysInDnFile(                                  //RETURN: the number of used ext entry in dn file
    uint32 cur_sim_task,
    BOOLEAN           *used_ext_flag, //IN/OUT:
    SIM_DIALLING_NUM_TYPE_E dn_type    //IN
)
{
    uint16 i = 0;
    uint8 j, k = 0;
    uint16 total_ext_num = 0;
    uint16 used_ext_num = 0;
    uint16 ext_id = 0;
    SIM_DIALLING_NUM_TYPE_E related_dn_type[2] = {SIM_DN_T_ADN, SIM_DN_T_MSISDN};
    SIMDN_EXT_INFO_T *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, dn_type);

    for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
    {
        for(j = 0; j < SIM_MAX_EXT_NUM; j++)
        {
            ext_id = *(dn_ext_info_ptr->ext_list_ptr + i * SIM_MAX_EXT_NUM + j);

            if((0XFF > ext_id) && (0 < ext_id))
            {
                used_ext_flag[ext_id - 1] = TRUE;//set as used flag, -1 is for array-index

            }
            else
            {
                break;
            }
        }
    }

    //ADN/MSISDN/LND use the same EXT1
    if((SIM_DN_T_ADN == dn_type) || (SIM_DN_T_LND == dn_type) || (SIM_DN_T_MSISDN == dn_type))
    {
        total_ext_num = s_sim_dn_ext1_num[cur_sim_task];

        if(SIM_DN_T_ADN == dn_type)
        {
            related_dn_type[0] = SIM_DN_T_MSISDN;
            related_dn_type[1] = SIM_DN_T_LND;
        }
        else if(SIM_DN_T_LND == dn_type)
        {
            related_dn_type[0] = SIM_DN_T_ADN;
            related_dn_type[1] = SIM_DN_T_MSISDN;
        }
        else if(SIM_DN_T_MSISDN == dn_type)
        {
            related_dn_type[0] = SIM_DN_T_ADN;
            related_dn_type[1] = SIM_DN_T_LND;
        }

        for(k = 0; k < 2; k++)
        {
            dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, related_dn_type[k]);

            for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
            {
                for(j = 0; j < SIM_MAX_EXT_NUM; j++)
                {
                    ext_id = *(dn_ext_info_ptr->ext_list_ptr + i * SIM_MAX_EXT_NUM + j);

                    if((0XFF > ext_id) && (0 < ext_id))
                    {
                        used_ext_flag[ext_id - 1] = TRUE;//set as used flag, -1 is for array-index
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    else if(SIM_DN_T_FDN == dn_type)
    {
        total_ext_num = s_sim_dn_ext2_num[cur_sim_task];
    }

    //count the used ext num
    for(i = 0; i < total_ext_num; i++)
    {
        if(used_ext_flag[i])
        {
            used_ext_num++;
        }
    }

    return (used_ext_num);
}

/********************************************************************************/
//  Description : Seek several empty corresponding extension record for the dialling
//                number,when there are not enough empty extension, first purge the
//                extension file then seek again.
//  Global resource dependence :
//  Author: wuding.yang
//  Note:
/********************************************************************************/
STATIC SIM_DN_OPERATION_RESULT_E SeekEmptyExtRecord(    //return value:the operate result
    uint32                  cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E dn_type,        //the type of the dialling number
    uint8                   record_num,     //the empty ext number want to find
    uint8                   *id_arr         //the array to save the empty ext id
)
{
    uint8 i, j;
    uint8   used_ext_num = 0;
    BOOLEAN used_ext_flag[SIM_MAX_RECORD_NUM] = {FALSE};
    uint8 need_ext_num = 255;//init
    uint8 real_ext_num = 0;
    SIM_DN_OPERATION_RESULT_E   result      = SIM_DN_SERVICE_NOT_ENABLE;
    SIM_SERVICE_TYPE_PH_2_E     ext_service = GetDNExtService(dn_type);

    if(SIMEF_IsServiceEnabled(cur_sim_task, ext_service))
    {
        SCI_MEMSET(used_ext_flag, 0, SIM_MAX_RECORD_NUM);//clear all

        // ADN, MSISDN and LND use EXT1
        if((SIM_DN_T_ADN == dn_type) || (SIM_DN_T_MSISDN == dn_type) || (SIM_DN_T_LND == dn_type))
        {
            //seek all to get all the empty records in EXT1
            used_ext_num = GetUsedExtEntrysInDnFile(cur_sim_task, used_ext_flag, dn_type);

            if(used_ext_num >= s_sim_dn_ext1_num[cur_sim_task])
            {
                //For example, some sim has 200 ADN records but only 7 EXT1 records.
                //So we have to count the used ext num. If all the ext records have been used,
                //we should return FAIL.
                result = SIM_DN_EXT_MEM_FULL;
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM: EXT1 exceed max %d, %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_433_112_2_17_22_58_17_4346, (uint8 *)"dd", used_ext_num, s_sim_dn_ext1_num[cur_sim_task]);
#endif
                return result;
            }

            real_ext_num = s_sim_dn_ext1_num[cur_sim_task];
        }
        else if(SIM_DN_T_FDN == dn_type)
        {
            //seek all to get all the empty records in EXT2
            used_ext_num = GetUsedExtEntrysInDnFile(cur_sim_task, used_ext_flag, dn_type);

            if(used_ext_num >= s_sim_dn_ext2_num[cur_sim_task])
            {
                //For example, some sim has 200 ADN records but only 7 EXT1 records.
                //So we have to count the used ext num. If all the ext records have been used,
                //we should return FAIL.
                result = SIM_DN_EXT_MEM_FULL;
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM: EXT2 exceed max %d, %d "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_451_112_2_17_22_58_17_4347, (uint8 *)"dd", used_ext_num, s_sim_dn_ext2_num[cur_sim_task]);
#endif
                return result;
            }

            real_ext_num = s_sim_dn_ext2_num[cur_sim_task];
        }

#ifdef WIN32
        else if(SIM_DN_T_SDN == dn_type)
        {
            //seek all to get all the empty records in EXT3
            used_ext_num = GetUsedExtEntrysInDnFile(cur_sim_task, used_ext_flag, dn_type);

            if(used_ext_num >= s_sim_dn_ext3_num[cur_sim_task])
            {
                //For example, some sim has 200 SDN records but only 7 EXT3 records.
                //So we have to count the used ext num. If all the ext records have been used,
                //we should return FAIL.
                result = SIM_DN_EXT_MEM_FULL;
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM: EXT3 exceed max %d, %d "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_470_112_2_17_22_58_17_4348, (uint8 *)"dd", used_ext_num, s_sim_dn_ext3_num[cur_sim_task]);
#endif
                return result;
            }

            real_ext_num = s_sim_dn_ext3_num[cur_sim_task];
        }

#endif


        j = 0;
        need_ext_num = 0;
        result = SIM_DN_EXT_MEM_FULL;//clear it

        for(i = 0; i < real_ext_num; i++) //the loop times should not exceed the real ext nums.
        {
            if(!used_ext_flag[i]) //find an empty record
            {
                id_arr[j] = i + 1; //record EF index should add 1
                j++;
                need_ext_num++;

                if(need_ext_num >= record_num) //get enough EXT records
                {
                    result = SIM_DN_OK;
                    break;//quit the loop
                }

                if(j > SIM_MAX_EXT_NUM) //check the memory
                {
                    break;
                }
            }
        }
    }
    else
    {
        result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    return result;
}



/**********************************************************************
//    Description:
//      Get the dialling number file name of the dialing number type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E GetDNFileName(   //return value:the file name of the type dialling numer
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
)
{
    SCI_ASSERT((SIM_DN_T_ADN <= dn_type) && (dn_type < SIM_DN_T_NUM));/*assert verified:Check_value */

    return (SIM_FILE_NAME_E)s_dn_info_arr[dn_type][0];
}


/**********************************************************************
//    Description:
//      Get the extension file name of the dialing number type
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E GetDNExtFileName(    //return value:the extension file name of the type dialling numer
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
)
{
    SCI_ASSERT((SIM_DN_T_ADN <= dn_type) && (dn_type < SIM_DN_T_NUM));/*assert verified:Check_value */

    return (SIM_FILE_NAME_E)s_dn_info_arr[dn_type][2];
}


/**********************************************************************
//    Description:
//      Get the dialing number service index in the SIM service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_TYPE_PH_2_E GetDNService(    //return value:the dialling number service index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
)
{
    SCI_ASSERT((SIM_DN_T_ADN <= dn_type) && (dn_type < SIM_DN_T_NUM));/*assert verified:Check_value */

    return (SIM_SERVICE_TYPE_PH_2_E)s_dn_info_arr[dn_type][1];
}


/**********************************************************************
//    Description:
//      Get the dialing number purge flag index in the SIM service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC int16 GetDNPurgeFlagIndex(    //return value:purge flag index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
)
{
    SCI_ASSERT((SIM_DN_T_ADN <= dn_type) && (dn_type < SIM_DN_T_NUM));/*assert verified:Check_value */
    return s_dn_info_arr[dn_type][4];
}

/**********************************************************************
//    Description:
//      Get the dialing number extension service index in the SIM
//    service table
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
STATIC SIM_SERVICE_TYPE_PH_2_E GetDNExtService( //return value:the dialling number extension service index
    SIM_DIALLING_NUM_TYPE_E dn_type    //the type of the dialling number
)
{
    SCI_ASSERT((SIM_DN_T_ADN <= dn_type) && (dn_type < SIM_DN_T_NUM));/*assert verified:Check_value */

    return (SIM_SERVICE_TYPE_PH_2_E)s_dn_info_arr[dn_type][3];
}

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function access all the EFs that reference to the same EFext for
//    storage and identify records in these file using extension data.the
//    existing chains have to be followed to the end.all referred extension
//    records are noted.akk the extension records not noted are then marked
//    as "free" the set to be '0xff'
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:Reference to section 11 of GSM 11.11
***********************************************************************/
STATIC SIM_DN_OPERATION_RESULT_E PurgeDiallingNumber(      //return value:the result of the operation
    uint32 cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E dn_type     //the type of the dialling number
)
{
    SIM_EF_MAPPING_DN_T dn_mapping;
    SIM_EF_MAPPING_EXT_T ext_mapping;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    SIM_DN_OPERATION_RESULT_E operate_result;
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_FILE_NAME_E dn_file_name, ext_file_name, ext_file_name1;
    SIM_SERVICE_TYPE_PH_2_E dn_service, ext_service;
    SIM_DIALLING_NUM_TYPE_E dn_type1;
    uint16 record_num, i, dn_num;
    uint8 ext_id;
    BOOLEAN arr[SIM_MAX_RECORD_NUM];
    BOOLEAN erasure_flag = FALSE;
    int16 s_purge_flag_index;/*@zhangyongxia,CR4266,2003-8-29*/

    operate_result  = SIM_DN_OK;

    ext_file_name   = GetDNExtFileName(dn_type);
    ext_service     = GetDNExtService(dn_type);

    SCI_MEMSET(arr, 0x0, (SIM_MAX_RECORD_NUM * sizeof(BOOLEAN)));

    //the extension service must be enabled
    if(SIMEF_IsServiceEnabled(cur_sim_task, ext_service))
    {
        ser_result = SIMSER_SelectFile(cur_sim_task, ext_file_name, &select_data);

        if(SIM_SERVICE_OK != ser_result)
        {
            //error handle
            return SIM_DN_SELECT_FILE_ERROR;
        }

        record_num = select_data.freespace_filesize / select_data.dfnumber_recordlen;
#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::record number of extfile 0x%x is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_636_112_2_17_22_58_18_4349, (uint8 *)"dd", ext_file_name, record_num);
#endif

        //check all the EF that reference to the extension file
        for(dn_type1 = SIM_DN_T_ADN; dn_type1 < SIM_DN_T_NUM; dn_type1 = (SIM_DIALLING_NUM_TYPE_E)(dn_type1 + 1))
        {
            dn_service = GetDNService(dn_type1);

            //check the dialling number service enabled or not
            if(!SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
            {
                continue;
            }

            //get the extension file the dialling number file reference to
            ext_file_name1 = GetDNExtFileName(dn_type1);

            //the dialling number reference to the extension file
            if(ext_file_name1 == ext_file_name)
            {
                dn_file_name = GetDNFileName(dn_type1);
                //get the dialling number in the dialling number file
                ser_result = SIMSER_SelectFile(cur_sim_task, dn_file_name, &select_data);

                if(SIM_SERVICE_OK != ser_result)
                {
                    //error handle
                    return SIM_DN_SELECT_FILE_ERROR;
                }

                dn_num = select_data.freespace_filesize / select_data.dfnumber_recordlen;
                // 04/24/2008 delete the trace
                //                 SCI_TRACE_LOW("SIM::record number of adnfile 0x%x is %d",dn_file_name,dn_num);

                //check every record in the dialling number file to
                for(i = 1; i <= dn_num; i++)
                {
                    //get a record of the dialling number file
                    ser_result = SIMSER_ReadEFRecord(cur_sim_task, dn_file_name, (uint8)i,
                                                     SIM_RECORD_ABSOLUTE, &dn_mapping);

                    if(SIM_SERVICE_OK != ser_result)
                    {
                        //error handle
                        return SIM_DN_READ_FILE_ERROR;
                    }

                    //the extension file is exist in this record
                    if(dn_mapping.is_ext_exist)
                    {
                        if(dn_mapping.ext_id != 0)
                        {
                            arr[dn_mapping.ext_id] = TRUE;
                        }

                        ext_id = dn_mapping.ext_id;
#ifndef _LESSEN_TRACE_SUPPORT
                        //SCI_TRACE_LOW:"SIM::dn_id 0x%x is related with ext_id 0x%x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_686_112_2_17_22_58_18_4350, (uint8 *)"dd", i, ext_id);
#endif

                        //check the extension record in the extension file
                        while(ext_id != 0)
                        {
                            //read the extension record
                            ser_result = SIMSER_ReadEFRecord(cur_sim_task, ext_file_name, ext_id,
                                                             SIM_RECORD_ABSOLUTE, &ext_mapping);

                            if(SIM_SERVICE_OK != ser_result)
                            {
                                //error handle
                                return SIM_DN_READ_FILE_ERROR;
                            }

                            //there are extension record after this extension record
                            if(ext_mapping.more_data)
                            {
#ifndef _LESSEN_TRACE_SUPPORT
                                //SCI_TRACE_LOW:"SIM::next id of ext_id 0x%x is 0x%x"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_703_112_2_17_22_58_18_4351, (uint8 *)"dd", ext_id, ext_mapping.next_record_id);
#endif
                                ext_id = ext_mapping.next_record_id;
                                arr[ext_id] = TRUE;
                            }
                            else
                            {
#ifndef _LESSEN_TRACE_SUPPORT
                                //SCI_TRACE_LOW:"SIM::next id of ext_id 0x%x is null"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_711_112_2_17_22_58_18_4352, (uint8 *)"d", ext_id);
#endif
                                break;
                            }
                        }
                    }
                }
            }
        }

        //erasure the extension file record
        for(i = 1; i <= record_num; i++)
        {
            //the record is not referenced(directly or indirectly) by any dialling number file
            //erasure this record and make free space
            if(!arr[i])
            {
#ifndef _LESSEN_TRACE_SUPPORT
                //SCI_TRACE_LOW:"SIM::No.0x%x record of the ext file is free"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_729_112_2_17_22_58_18_4353, (uint8 *)"d", i);
#endif
                ser_result = SIMSER_UpdateEFRecord(cur_sim_task, ext_file_name, (uint8)i,
                                                   SIM_RECORD_ABSOLUTE, NULL, TRUE);

                if(SIM_SERVICE_OK != ser_result)
                {
#ifndef _LESSEN_TRACE_SUPPORT
                    //SCI_TRACE_LOW:"SIM::update the record to free is failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_736_112_2_17_22_58_18_4354, (uint8 *)"");
#endif
                    //error handle
                    return SIM_DN_UPDATE_FILE_ERROR;
                }

                erasure_flag = TRUE;
            }
        }
    }
    else
    {
        operate_result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    /*@zhangyongxia,CR4266,2003-8-29*/
    s_purge_flag_index = GetDNPurgeFlagIndex(dn_type);

    if(!erasure_flag)
    {
#ifndef SIM_CODE_COMPACT
        prev_purge_fail[cur_sim_task][s_purge_flag_index] = TRUE;
#endif

        if(ser_result == SIM_SERVICE_OK)
        {
            operate_result = SIM_DN_EXT_MEM_FULL;
        }
    }

    return operate_result;
}

/**********************************************************************
//    Description:
//      the function that process the UPDATE CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_UpdateCCP(
    uint32 cur_sim_task,
    SIM_SIG_UPDATE_CCP_REQ_T *psig  //the update CCP request signal
)
{
    SIM_SIG_UPDATE_CCP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_CCP_CNF, sizeof(SIM_SIG_UPDATE_CCP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->ccp_id = psig->ccp_id;
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_CCP_OK;

    if(0 == psig->ccp_id)
    {
        pcnf->result = SIM_CCP_UPDATE_ERROR;
        //send the confirm signal to the signal requestor
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CCP))
    {
        //do the request operation
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_CCP, psig->ccp_id,
                                           SIM_RECORD_ABSOLUTE, &(psig->ccp), FALSE);

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_CCP_UPDATE_ERROR;
        }
    }
    else
    {
        pcnf->result = SIM_CCP_SERVICE_NOT_ENABLE;
    }


    //send the confirm signal to the signal requestor
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the ERASE CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_EraseCCP(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_CCP_REQ_T *psig //the erasure CCP request signal
)
{
    SIM_SIG_ERASE_CCP_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_ERASE_CCP_CNF, sizeof(SIM_SIG_ERASE_CCP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->ccp_id = psig->ccp_id;
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_CCP_OK;

    if(0 == psig->ccp_id)
    {
        pcnf->result = SIM_CCP_UPDATE_ERROR;
        //send the confirm signal to the signal requestor
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CCP))
    {
        //do the operation
        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, SIM_FILE_EF_CCP, psig->ccp_id,
                                           SIM_RECORD_ABSOLUTE, NULL, TRUE);

        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_CCP_UPDATE_ERROR;
        }
    }
    else
    {
        pcnf->result = SIM_CCP_SERVICE_NOT_ENABLE;
    }

    //send the confirm signal to the signal requestor
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author:
//  Note:
/********************************************************************************/
STATIC SIM_CCP_OPERATION_RESULT_E ReadCCPEntry(
    uint32               cur_sim_task,   //IN:
    uint8                ccp_id,         //IN:
    SIM_EF_MAPPING_CCP_T *ccp_ptr        //OUT:
)
{
    SIM_CCP_OPERATION_RESULT_E  ccp_result = SIM_CCP_OK;
    SIM_SERVICE_RESULT_E        ser_result = SIM_SERVICE_OK;

    if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_CCP))
    {
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_CCP, ccp_id, SIM_RECORD_ABSOLUTE, ccp_ptr);

        if(SIM_SERVICE_OK != ser_result)
        {
            ccp_result = SIM_CCP_READ_ERROR;
        }
    }
    else
    {
        ccp_result = SIM_CCP_SERVICE_NOT_ENABLE;
    }

    return (ccp_result);
}

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function that process the READ CCP request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_ReadCCP(
    uint32 cur_sim_task,
    SIM_SIG_READ_CCP_REQ_T *psig    //the read CCP request signal
)
{
    SIM_SIG_READ_CCP_CNF_T *pcnf;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_CCP_CNF, sizeof(SIM_SIG_READ_CCP_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->ccp_id        = psig->ccp_id;
    pcnf->command_ref   = psig->command_ref;

    if(0 == psig->ccp_id)
    {
        pcnf->result = SIM_CCP_READ_ERROR;
        //send the confirm signal to the signal requestor
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    pcnf->result        = ReadCCPEntry(cur_sim_task, pcnf->ccp_id, &(pcnf->ccp));

    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
STATIC uint8 SIMDN_IsExtidUsedbyOtherDnid(
    uint32 cur_sim_task,
    SIM_DIALLING_NUM_TYPE_E dn_type,
    uint8 ext_id)
{
    uint16 i = 0;//i must be uint16
    uint8 j, k = 0;
    uint8 multi_dn_rec_use_same_ext_rec = 0;
    SIM_DIALLING_NUM_TYPE_E related_dn_type[2] = {SIM_DN_T_ADN, SIM_DN_T_MSISDN};
    SIMDN_EXT_INFO_T *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, dn_type);

    if((0xFF != ext_id) && (0x0 != ext_id))
    {
        //可能出现多条DN 记录共用同一条EXT 记录的情况，
        //这时我们还需要找出其他也用到这条EXT 记录的其他DN 记录。
        for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
        {
            for(j = 0; j < SIM_MAX_EXT_NUM; j++)
            {
                if(ext_id == (*(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j)))
                {
                    multi_dn_rec_use_same_ext_rec++;
                    //SCI_TRACE_LOW:"SIM%d:DN EXT found multi dn_id %d use ext_id %d. %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_952_112_2_17_22_58_18_4355, (uint8 *)"dddd", cur_sim_task, i + 1, ext_id, multi_dn_rec_use_same_ext_rec);
                }
            }
        }

        if((SIM_DN_T_ADN == dn_type) || (SIM_DN_T_LND == dn_type) || (SIM_DN_T_MSISDN == dn_type))
        {
            if(SIM_DN_T_ADN == dn_type)
            {
                related_dn_type[0] = SIM_DN_T_MSISDN;
                related_dn_type[1] = SIM_DN_T_LND;
            }
            else if(SIM_DN_T_LND == dn_type)
            {
                related_dn_type[0] = SIM_DN_T_ADN;
                related_dn_type[1] = SIM_DN_T_MSISDN;
            }
            else if(SIM_DN_T_MSISDN == dn_type)
            {
                related_dn_type[0] = SIM_DN_T_ADN;
                related_dn_type[1] = SIM_DN_T_LND;
            }

            for(k = 0; k < 2; k++)
            {
                dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, related_dn_type[k]);

                for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
                {
                    for(j = 0; j < SIM_MAX_EXT_NUM; j++)
                    {
                        if(ext_id == (*(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j)))
                        {
                            multi_dn_rec_use_same_ext_rec++;
                            //SCI_TRACE_LOW:"SIM%d:related DN EXT found multi dn_id %d use ext_id %d. %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_985_112_2_17_22_58_18_4356, (uint8 *)"dddd", cur_sim_task, i + 1, ext_id, multi_dn_rec_use_same_ext_rec);
                        }
                    }
                }
            }
        }
    }

    return multi_dn_rec_use_same_ext_rec;
}

/**********************************************************************
//    Description:
//      the function that process the READ Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_ReadDN(
    uint32 cur_sim_task,
    SIM_SIG_READ_DN_REQ_T *psig     //the read dialling number request signal
)
{
    SIM_SIG_READ_DN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;
    SIM_SERVICE_RESULT_E  opt_result = SIM_SERVICE_OK;  // used for ccp and ext
    SIM_FILE_NAME_E dn_file_name, ext_file_name;
    SIM_SERVICE_TYPE_PH_2_E dn_service, ext_service;
    SIM_EF_MAPPING_DN_T dn_mapping;
    SIM_EF_MAPPING_EXT_T dn_ext;
    BOOLEAN more_ext = FALSE;
    uint8   ext_id  = 0;
    uint8   i       = 0;
    uint16  p       = 0;
    uint32              pos_offset       = 0;
    SIMDN_EXT_INFO_T    *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, psig->dn_type);
    uint8 dn_id = (uint8)psig->dn_id;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

    SCI_MEMSET(&dn_mapping, 0, sizeof(SIM_EF_MAPPING_DN_T)); //pclint

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_READ_DN_CNF, sizeof(SIM_SIG_READ_DN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->dn_id = psig->dn_id;
    pcnf->dn_type = psig->dn_type;
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_DN_OK;

    if(0 == psig->dn_id)
    {
        pcnf->result = SIM_DN_READ_FILE_ERROR;
        //send the confirm signal to the signal requestor
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    //get the dialling number relative infor
    dn_file_name    = GetDNFileName(psig->dn_type);
    ext_file_name   = GetDNExtFileName(psig->dn_type);
    dn_service      = GetDNService(psig->dn_type);
    ext_service     = GetDNExtService(psig->dn_type);

    //the dialing number service is enabled
    if(SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
    {
#ifdef SIM_3G_USIM_SUPPORT

        if(SIM_DN_T_ADN == psig->dn_type)
        {
#ifdef SIM_ADN_SERACH_SUPPORT

            if((PNULL != s_sim_adn_search_result[cur_sim_task])
                && (0xFF == s_sim_adn_search_result[cur_sim_task][psig->dn_id-1]))
            {
                s_sim_adn_search_result[cur_sim_task][psig->dn_id-1] = 0;//only for power on, the 1st READ
                SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
                return;
            }

#endif
            ////////////////////////////////////////test
            //psig->dn_id += 250;
            //SCI_TRACE_LOW:"SIM%d DN read started %d,"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1066_112_2_17_22_58_19_4357, (uint8 *)"dd", cur_sim_task, psig->dn_id);
            ////////////////////////////////////////
            dn_id = 0;
            dn_file_name = SIMDN_RemapDFpbADN(cur_sim_task, psig->dn_id, &dn_id);
        }

#endif

        //read the dialling number
        ser_result = SIMSER_ReadEFRecord(cur_sim_task, dn_file_name, dn_id,
                                         SIM_RECORD_ABSOLUTE, &dn_mapping);
#ifdef SIM_3G_USIM_SUPPORT

        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                && ((SIM_FILE_EF_MSISDN == dn_file_name)
                    || (SIM_FILE_EF_FDN == dn_file_name)
                    || (SIM_FILE_EF_SDN == dn_file_name)
                    || (SIM_FILE_EF_BDN == dn_file_name))))
        {
            if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
            {
                is_in_adf_usim = FALSE;
            }
            else
            {
                is_in_adf_usim = TRUE;
            }

            SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping);
        }

#endif

        //analysis the failed reason
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_DN_READ_FILE_ERROR;
        }
        else
        {
            //copy the alpha id
            pcnf->dn.alpha_id_len = MIN(SIM_MAX_DN_ALPHA_IDENTIFY_LEN, dn_mapping.alpha_id_len);
            SCI_MEMCPY(pcnf->dn.alpha_id, dn_mapping.alpha_id, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);

            //copy the dialling num
            pcnf->dn.number_len = MIN(SIM_EF_DN_DN_LEN, dn_mapping.dn_len);
            SCI_MEMCPY(pcnf->dn.numbers, dn_mapping.dialling_num, pcnf->dn.number_len);

            //the TON and NPI
            pcnf->dn.is_ton_npi_exist = dn_mapping.is_ton_npi_exist;

            if(pcnf->dn.is_ton_npi_exist)
            {
                pcnf->dn.ton = dn_mapping.ton;
                pcnf->dn.npi = dn_mapping.npi;
            }

            //the Capability and the Configuration of the dialling number
            pcnf->dn.is_ccp_exist = FALSE;

            if((dn_mapping.is_ccp_exist) && (0 < dn_mapping.ccp_id))
            {
                pcnf->dn.ccp_id = dn_mapping.ccp_id;

                if(SIM_CCP_OK == ReadCCPEntry(cur_sim_task, dn_mapping.ccp_id, &(pcnf->dn.ccp)))
                {
                    pcnf->dn.is_ccp_exist = TRUE;
                }
            }

            //the extension of the dialling number
            if(dn_mapping.is_ext_exist)
            {
                //the extension service is enabled
                if(SIMEF_IsServiceEnabled(cur_sim_task, ext_service))
                {
                    if((0 != dn_mapping.ext_id) && (0xFF != dn_mapping.ext_id)) //must be valid
                    {
                        more_ext = TRUE;
                        ext_id = dn_mapping.ext_id;
                    }
                    else
                    {
                        more_ext = FALSE;
                    }

                    if(more_ext)
                    {
                        if(SIM_FILE_EF_LND == dn_file_name)
                        {
                            for(p = 0; p < dn_ext_info_ptr->dn_max_num; p++)
                            {
                                if((0XFF == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * p))
                                    || (0 == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * p)))
                                {
                                    pos_offset = SIM_MAX_EXT_NUM * p;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if(PNULL != dn_ext_info_ptr)
                            {
                                pos_offset = SIM_MAX_EXT_NUM * (pcnf->dn_id - 1);
                            }
                        }
                    }

                    while(more_ext)
                    {
                        //read the dialling number extension
                        opt_result = SIMSER_ReadEFRecord(cur_sim_task, ext_file_name, ext_id,
                                                         SIM_RECORD_ABSOLUTE, &dn_ext);

#ifdef SIM_3G_USIM_SUPPORT

                        if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                            && ((SIM_SEC_STATUS_NOT_SATISFIED == opt_result)
                                && ((SIM_FILE_EF_EXT2 == ext_file_name)
                                    || (SIM_FILE_EF_EXT3 == ext_file_name))))
                        {
                            if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                            {
                                is_in_adf_usim = FALSE;
                            }
                            else
                            {
                                is_in_adf_usim = TRUE;
                            }

                            SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                            opt_result = SIMSER_ReadEFRecord(cur_sim_task, ext_file_name, ext_id, SIM_RECORD_ABSOLUTE, &dn_ext);
                        }

#endif

                        //analysis and process the failed reason
                        if(SIM_SERVICE_OK != opt_result)
                        {
                            //SCI_TRACE_LOW:"SIM::read ext_id:%d failed when read DN,result is %d."
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1199_112_2_17_22_58_19_4358, (uint8 *)"dd", dn_mapping.ccp_id, opt_result);
                            //  pcnf->result = SIM_DN_READ_FILE_ERROR;
                            break;
                        }

                        //additional data
                        if(SIM_EXT_RECORD_ADDTIONAL_DATA == dn_ext.record_type)
                        {
                            //check the length is OK
                            if(dn_ext.extension_data_len > SIM_EF_EXT_ADDITIONAL_DATA_LEN)
                            {
                                dn_ext.extension_data_len = SIM_EF_EXT_ADDITIONAL_DATA_LEN;
                            }

                            if(SIM_EF_DN_DN_LEN == pcnf->dn.number_len)
                            {
                                //is the dialling number reach the max dialling number length ?
                                if((SIM_MAX_DN_NUMBER_LEN - pcnf->dn.number_len) < dn_ext.extension_data_len)
                                {
                                    //copy the extension data after the dialling number
                                    SCI_MEMCPY((pcnf->dn.numbers + pcnf->dn.number_len), dn_ext.extension_data, (SIM_MAX_DN_NUMBER_LEN - pcnf->dn.number_len));
                                    pcnf->dn.number_len = SIM_MAX_DN_NUMBER_LEN;
                                    dn_ext.more_data = FALSE;//stop the loop after ++.
                                }
                                else
                                {
                                    //copy the extension data after the dialling number
                                    SCI_MEMCPY(pcnf->dn.numbers + pcnf->dn.number_len, dn_ext.extension_data, dn_ext.extension_data_len);
                                    pcnf->dn.number_len += dn_ext.extension_data_len;
                                }
                            }
                            else
                            {
                                //这里还有一种情况。比如DN 记录中号码很短，但是该条DN记录
                                //又指示还有ext 记录，因为这时ext记录有可能是called
                                //party subaddress，所以我们需要把ext记录读出来才知道。
                                //但读出来发现ext记录又是additional data，
                                //对比机在这种情况下是不拷贝ext additional data的。
                                //我认为应该把该条EXT记录视为无效，不去++。
                                break;//quit the loop
                            }
                        }
                        else
                        {
                            //called party subaddress. just check length.
                            if(dn_ext.extension_data_len > SIM_EF_EXT_DATA_LEN)
                            {
                                dn_ext.extension_data_len = SIM_EF_EXT_DATA_LEN;
                            }

                            dn_ext.more_data = FALSE;//stop the loop after ++.
                        }

                        if(0 == SIMDN_IsExtidUsedbyOtherDnid(cur_sim_task, psig->dn_type, ext_id))
                        {
                            pcnf->current_record_ext_num++;
                        }
                        else
                        {
                            //有可能出现多条DN记录共用同一条EXT记录的情况，
                            //这里我们不再上报有使用的ext_num，否则上层++会
                            //出现用到的ext_num超过ext_num 总数的情况，导致上层assert。
                        }

                        //to record the EXT id, SIM_MAX_EXT_NUM(2) is the maximum.
                        if((i < SIM_MAX_EXT_NUM) && (PNULL != dn_ext_info_ptr)) //check the array index
                        {
                            *(dn_ext_info_ptr->ext_list_ptr + pos_offset + i) = ext_id;
                            i++;
                        }

                        //Situation that ext record identifier pointing to ext num self should be avoided
                        if(ext_id == dn_ext.next_record_id)
                        {
                            break;
                        }

                        ext_id = dn_ext.next_record_id;

                        if((0 != ext_id) && (0xFF != ext_id)) //ext_id must be valid
                        {
                            more_ext = dn_ext.more_data;
                        }
                        else
                        {
                            more_ext = FALSE;
                        }
                    }
                }
            }
        }
    }
    else
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    //////////////////////////////
    //SCI_TRACE_LOW:"SIM%d DN_%d read finished %d, ext_num is %d, ext_id %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1308_112_2_17_22_58_19_4359, (uint8 *)"ddddd", cur_sim_task, psig->dn_type, pcnf->dn_id, pcnf->current_record_ext_num, dn_mapping.ext_id);
    //////////////////////////////

    //send the confirm signal to the signal requestor
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/********************************************************************************/
STATIC SIMDN_EXT_INFO_T *GetExtInfoPtr(
    uint32                   cur_sim_task,   //IN:
    SIM_DIALLING_NUM_TYPE_E  dn_type         //IN:
)
{
    SIMDN_EXT_INFO_T    *dn_ext_info_ptr = PNULL;

    switch(dn_type)
    {
        case SIM_DN_T_LND:
            dn_ext_info_ptr = &(s_lnd_ext_info[cur_sim_task]);
            break;
        case SIM_DN_T_ADN:
            dn_ext_info_ptr = &(s_adn_ext_info[cur_sim_task]);
            break;
        case SIM_DN_T_MSISDN:
            dn_ext_info_ptr = &(s_msisdn_ext_info[cur_sim_task]);
            break;
        case SIM_DN_T_SDN:
            dn_ext_info_ptr = &(s_sdn_ext_info[cur_sim_task]);
            break;
        case SIM_DN_T_FDN:
            dn_ext_info_ptr = &(s_fdn_ext_info[cur_sim_task]);
            break;

        default:
#ifndef _LESSEN_TRACE_SUPPORT
            //SCI_TRACE_LOW:"GetExtInfoPtr dn_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1346_112_2_17_22_58_19_4360, (uint8 *)"d", dn_type);
#endif
            break;
    }

    return (dn_ext_info_ptr);
}

/********************************************************************************/
//  Description : update the ext file and related global variables after updating
//                the dn record
//  Global resource dependence :
//  Author:
//  Note: "Called Party Subaddress" is not supported
/********************************************************************************/
STATIC SIM_DN_OPERATION_RESULT_E UpdateExtInfo(
    uint32                   cur_sim_task,   //IN:
    SIM_DIALLING_NUM_TYPE_E  dn_type,        //IN:
    uint16                    dn_id,          //IN:
    SIM_DIALLING_NUMBER_T    *update_dn_ptr, //IN: the dialling number
    SIM_EF_MAPPING_DN_T      *sim_mapping,   //IN/OUT:
    int8 *increased_ext_num                    //OUT
)
{
    SIM_EF_MAPPING_DN_T         dn_mapping;
    SIM_EF_MAPPING_EXT_T        dn_ext;
    SIM_FILE_NAME_E             ext_file_name   = SIM_FILE_EF_EXT1;
    SIM_SERVICE_RESULT_E        ser_result      = SIM_SERVICE_OK;
    SIM_DN_OPERATION_RESULT_E   dn_result       = SIM_DN_OK;
    SIM_SERVICE_TYPE_PH_2_E     ext_service     = GetDNExtService(dn_type);
    BOOLEAN                     is_ext_exist    = (update_dn_ptr->number_len > SIM_EF_DN_DN_LEN) ? TRUE : FALSE;
    int8                        ext_num         = ((update_dn_ptr->number_len + SIM_EF_DN_DN_LEN - 1) / SIM_EF_EXT_ADDITIONAL_DATA_LEN) - 1;
    int16                        cur_dn_id       = dn_id - 1;
    uint8                       update_dn_len   = SIM_EF_DN_DN_LEN;
    uint8                       i               = 0;
    uint8                       ext_id_list[SIM_MAX_EXT_NUM+1];//当SIM_MAX_EXT_NUM为1下面使用的地方有pclint，所以+1
    SIMDN_EXT_INFO_T            *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, dn_type);
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif
    uint8 temp_ext_id[SIM_MAX_EXT_NUM] = {0};

    *increased_ext_num = 0;

    if(SIMEF_IsServiceEnabled(cur_sim_task, ext_service))
    {
        if(SIM_DN_T_LND == dn_type)
        {
            // for LND, the entry id is invalid
            ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_LND, 0, SIM_RECORD_PREVIOUS, &dn_mapping);

            if(SIM_SERVICE_OK != ser_result)
            {
                dn_result = SIM_DN_READ_FILE_ERROR;
                return (dn_result);
            }

            if(PNULL != dn_ext_info_ptr)
            {
                cur_dn_id = 0;

                for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
                {
                    if(dn_mapping.is_ext_exist)
                    {
                        if(dn_mapping.ext_id == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i))
                        {
                            cur_dn_id = i;
                            break;
                        }
                    }
                    else
                    {
                        if((0XFF == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i))
                            || (0 == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i)))
                        {
                            cur_dn_id = i;
                            break;
                        }
                    }
                }
            }
        }

        if(PNULL != dn_ext_info_ptr)
        {
            for(i = 0; i < SIM_MAX_EXT_NUM; i++)
            {
                temp_ext_id[i] = *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * cur_dn_id + i);

                if(0xFF != temp_ext_id[i])
                {
                    //原来该条dn_id 就有对应的ext 记录
                    (*increased_ext_num)--;//可以为负数，负数表示本条dn_id 用到的ext数量减少了
                    //SCI_TRACE_LOW:"SIM%d: -- EXT %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1433_112_2_17_22_58_20_4361, (uint8 *)"dd", cur_sim_task, (*increased_ext_num));
                }

                // clear the value
                *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * cur_dn_id + i) = 0XFF;
                //SCI_TRACE_LOW:"SIM%d:DN delete dn_id_%d 's ext rec as 0xFF"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1437_112_2_17_22_58_20_4362, (uint8 *)"dd", cur_sim_task, dn_id);
                // for speeding the update procedure, the ext file is not updated.
                //但如果下面找不到空的EXT 记录的话，需要恢复此值。
                //为什么会找不到空的?比如当两条dn 记录(A,B)共用同一条ext 记录，而且现在ext记录全部被用了。
                //现在修改一条dn记录A，就应该还是找不到有空的ext记录，因为B 还在占用。

                if(0 != SIMDN_IsExtidUsedbyOtherDnid(cur_sim_task, dn_type, temp_ext_id[i]))
                {
                    //如果还有其他DN 记录也在使用这条EXT 记录，那么不能告知上层
                    //现在有多一条EXT 记录可用了。
                    (*increased_ext_num)++;//recover the num.
                    //SCI_TRACE_LOW:"SIM%d: -- EXT recover %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1448_112_2_17_22_58_20_4363, (uint8 *)"dd", cur_sim_task, (*increased_ext_num));
                }

#ifdef SIM_DELETE_REAL_EXT
                else
                {
                    //没有其他DN记录使用这条EXT记录，从物理上删除它。
                    {
                        SIM_EF_MAPPING_EXT_T delete_dn_ext;

                        if((0xFF != temp_ext_id[i]) && (0x0 != temp_ext_id[i]))
                        {
                            SIMSER_UpdateEFRecord(cur_sim_task, ext_file_name, temp_ext_id[i], SIM_RECORD_ABSOLUTE, &delete_dn_ext, TRUE);
                        }
                    }
                }

#endif
            }
        }

        SCI_MEMSET(ext_id_list, 0xff, SIM_MAX_EXT_NUM);

        if(is_ext_exist)
        {
            dn_result = SeekEmptyExtRecord(cur_sim_task, dn_type, MIN(ext_num, SIM_MAX_EXT_NUM), ext_id_list);

            if(SIM_DN_OK == dn_result)
            {
                ext_file_name = GetDNExtFileName(dn_type);

                for(i = 0; i < ext_num; i++)
                {
                    if(i < (ext_num - 1))
                    {
                        SCI_MEMCPY(dn_ext.extension_data, update_dn_ptr->numbers + update_dn_len, SIM_EF_EXT_ADDITIONAL_DATA_LEN);
                        dn_ext.next_record_id       = ext_id_list[i+1];//ext_id_list[0]是放在dn记录的最后1 byte;这里的是放在ext记录的最后1 byte
                        dn_ext.more_data            = TRUE;
                        dn_ext.record_type          = SIM_EXT_RECORD_ADDTIONAL_DATA;
                        dn_ext.extension_data_len   = SIM_EF_EXT_ADDITIONAL_DATA_LEN;

                        update_dn_len += SIM_EF_EXT_ADDITIONAL_DATA_LEN;
                    }
                    else
                    {
                        //the last extension record
                        SCI_MEMSET(dn_ext.extension_data, 0xff, SIM_EF_EXT_DATA_LEN);
                        SCI_MEMCPY(dn_ext.extension_data, (update_dn_ptr->numbers + update_dn_len), (update_dn_ptr->number_len - update_dn_len));
                        dn_ext.more_data            = FALSE;
                        dn_ext.record_type          = SIM_EXT_RECORD_ADDTIONAL_DATA;
                        dn_ext.extension_data_len   = update_dn_ptr->number_len - update_dn_len;
                    }

                    // update the ext file in SIM
                    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, ext_file_name, ext_id_list[i], SIM_RECORD_ABSOLUTE, &dn_ext, FALSE);
#ifdef SIM_3G_USIM_SUPPORT

                    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                        && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                            && ((SIM_FILE_EF_EXT2 == ext_file_name)
                                || (SIM_FILE_EF_EXT3 == ext_file_name))))
                    {
                        if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                        {
                            is_in_adf_usim = FALSE;
                        }
                        else
                        {
                            is_in_adf_usim = TRUE;
                        }

                        SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, ext_file_name, ext_id_list[i], SIM_RECORD_ABSOLUTE, &dn_ext, FALSE);
                    }

#endif

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        if(0 == i)
                        {
                            sim_mapping->ext_id         = ext_id_list[0];//ext_id_list[0]是放在dn记录的最后1 byte
                            sim_mapping->is_ext_exist   = TRUE;
                        }

                        (*increased_ext_num)++;//1 ext record has been updated each time
                        //SCI_TRACE_LOW:"SIM%d: ++ EXT %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1525_112_2_17_22_58_20_4364, (uint8 *)"dd", cur_sim_task, (*increased_ext_num));
                    }
                    else
                    {
                        dn_result = SIM_DN_UPDATE_FILE_ERROR;
                        break;
                    }
                }
            }
            else
            {
                //没有找到空的ext 记录，需要恢复表中刚刚修改的0xFF 为原来的值.
                for(i = 0; i < SIM_MAX_EXT_NUM; i++)
                {
                    if(PNULL != dn_ext_info_ptr) //pclint
                    {
                        *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * cur_dn_id + i) = temp_ext_id[i];
                    }
                }
            }
        }

        if((SIM_DN_OK == dn_result) && (is_ext_exist))
        {
            if(PNULL != dn_ext_info_ptr)
            {
                for(i = 0; i < SIM_MAX_EXT_NUM; i++)
                {
                    *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * cur_dn_id + i) = ext_id_list[i];



                }
            }
        }
    }
    else
    {
        if(is_ext_exist)
        {
            // need update the ext file
            dn_result = SIM_DN_SERVICE_NOT_ENABLE;
        }
    }

    return (dn_result);
}

/**********************************************************************
//    Description:
//      the function that process the UPDATE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_UpdateDN(
    uint32                  cur_sim_task,
    SIM_SIG_UPDATE_DN_REQ_T *psig           //the update dialling number request signal
)
{
    SIM_SIG_UPDATE_DN_CNF_T     *pcnf           = PNULL;
    SIM_SERVICE_RESULT_E        ser_result      = SIM_SERVICE_ERROR_UNKNOW;
    SIM_DN_OPERATION_RESULT_E   operate_result  = SIM_DN_OK;
    SIM_FILE_NAME_E             dn_file_name    = SIM_FILE_EF_ADN;
    SIM_SERVICE_TYPE_PH_2_E     dn_service      = SIMSER_PH2_ADN;
    SIM_EF_MAPPING_DN_T         dn_mapping;
    uint8 dn_id = (uint8)psig->dn_id;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

#ifdef SIM_SELF_TEST_PB_EXT
    uint8 ywd_test_dn_buf[SIM_SELF_TEST_PB_DN_LEN] = {0};//30
#endif

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_UPDATE_DN_CNF, sizeof(SIM_SIG_UPDATE_DN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref   = psig->command_ref;
    pcnf->dn_id         = psig->dn_id;
    pcnf->dn_type       = psig->dn_type;

    pcnf->result    = SIM_DN_OK;
    pcnf->increased_ext_num = 0;

    //get the relative information of the dialling number
    dn_file_name    = GetDNFileName(psig->dn_type);
    dn_service      = GetDNService(psig->dn_type);


    if(SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
    {

#ifdef SIM_3G_USIM_SUPPORT

        ////////////////////////////////////////test
        //psig->dn_id = 287;
        ////////////////////////////////////////

        if(SIM_DN_T_ADN == psig->dn_type)
        {
            dn_id = 0;
            dn_file_name = SIMDN_RemapDFpbADN(cur_sim_task, psig->dn_id, &dn_id);
        }

#endif

        //copy the alpha id
        dn_mapping.alpha_id_len = psig->dn.alpha_id_len;

        if(dn_mapping.alpha_id_len > SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
        {
            dn_mapping.alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
        }

        SCI_MEMCPY(dn_mapping.alpha_id, psig->dn.alpha_id, dn_mapping.alpha_id_len);

#ifdef SIM_SELF_TEST_PB_EXT
        SCI_MEMSET(ywd_test_dn_buf, 0x55, SIM_SELF_TEST_PB_DN_LEN); //init with 0x55...

        SCI_MEMCPY(ywd_test_dn_buf, psig->dn.numbers, psig->dn.number_len); //copy the numbers from up-layer

        //the last byte maybe has an "F" as an end flag, remove it. For example, user set the number as "01234", we encode as "0x10 0x32 0xF4"
        if(0xF0 == (ywd_test_dn_buf[psig->dn.number_len -1] & 0xF0))
        {
            ywd_test_dn_buf[psig->dn.number_len -1] &= 0x0F;
            ywd_test_dn_buf[psig->dn.number_len -1] |= 0x50;
        }

        psig->dn.number_len = 25;//bytes, 1 byte means 2 numbers
        //psig->dn.number_len = SIM_SELF_TEST_PB_DN_LEN;
        //SCI_TRACE_LOW:"SIM: update DN EXT test. %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_1651_112_2_17_22_58_20_4365, (uint8 *)"d", psig->dn.number_len);
#endif

        //copy the dialling number.
        if(psig->dn.number_len > SIM_EF_DN_DN_LEN)
        {
            //get the extension needed number
            dn_mapping.dn_len = SIM_EF_DN_DN_LEN;
            dn_mapping.is_ext_exist = TRUE;
        }
        else
        {
            dn_mapping.is_ext_exist = FALSE;
            dn_mapping.dn_len = psig->dn.number_len;
        }

#ifdef SIM_SELF_TEST_PB_EXT
        SCI_MEMCPY(dn_mapping.dialling_num, ywd_test_dn_buf, dn_mapping.dn_len);
#else
        SCI_MEMCPY(dn_mapping.dialling_num, psig->dn.numbers, dn_mapping.dn_len);
#endif
        //the TON and NPI
        dn_mapping.is_ton_npi_exist = psig->dn.is_ton_npi_exist;

        if(dn_mapping.is_ton_npi_exist)
        {
            dn_mapping.ton = psig->dn.ton;
            dn_mapping.npi = psig->dn.npi;
        }

        //the CCP
        dn_mapping.is_ccp_exist = psig->dn.is_ccp_exist;

        if(dn_mapping.is_ccp_exist)
        {
            //just pointer out the ccp id,and the ccp content can be
            //update by the CCP update service
            dn_mapping.ccp_id = psig->dn.ccp_id;
        }

        operate_result = UpdateExtInfo(cur_sim_task, psig->dn_type, dn_id, &(psig->dn), &dn_mapping, &(pcnf->increased_ext_num));

        if(SIM_DN_OK == operate_result)
        {
            // update DN
            if(SIM_DN_T_LND == psig->dn_type)
            {
                //save the dialling number itself
                ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, 0, SIM_RECORD_PREVIOUS, &dn_mapping, FALSE);
            }
            else
            {
                //save the dialling number itself
                ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping, FALSE);
#ifdef SIM_3G_USIM_SUPPORT

                if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                        && ((SIM_FILE_EF_MSISDN == dn_file_name)
                            || (SIM_FILE_EF_FDN == dn_file_name)
                            || (SIM_FILE_EF_SDN == dn_file_name)
                            || (SIM_FILE_EF_BDN == dn_file_name))))
                {
                    if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                    {
                        is_in_adf_usim = FALSE;
                    }
                    else
                    {
                        is_in_adf_usim = TRUE;
                    }

                    SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping, FALSE);
                }
                else
#endif
                {
                    //SCI_TRACE_LOW("SIM: Test DN.");
                    if (SIM_SERVICE_OK != ser_result)
                    {
                        if ((SIM_FILE_EF_ADN == dn_file_name) 
    						|| (SIM_FILE_EF_TELECOM_PB_ADN == dn_file_name) 
    						|| (SIM_FILE_EF_TELECOM_PB1_ADN == dn_file_name)
    						|| (SIM_FILE_EF_USIM_PB_ADN == dn_file_name)
    						|| (SIM_FILE_EF_USIM_PB1_ADN == dn_file_name)) 
                        {
                            SIM_RESPONSE_DATA_OF_SELECT_T temp_select_response;
                            SIM_SERVICE_RESULT_E temp_result;
                            temp_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_MF, &temp_select_response);
                            if (SIM_SERVICE_OK == temp_result)
                            {
                                temp_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping, FALSE);
    							if (SIM_SERVICE_OK == temp_result)
    						    {
    						    	ser_result = SIM_SERVICE_OK;
    						    }
                            }
                        }
                    }
                }
	        }

            //the update is failed,handle the error
            if(SIM_SERVICE_OK != ser_result)
            {
                if((SIM_DN_T_LND != psig->dn_type) && (0 == dn_mapping.alpha_id_len))
                {
                    //For some SIM cards, it cannot save a PB record when the name is empty.
                    //So, if the first update failed, I'll check whether the name is empty.
                    //If the name is empty, I'll copy the phone number to the name and try again.
                    //And we do not deal with the LND, because it seldom occurs.
                    SIMDN_ReMapEmptyNameDN(cur_sim_task, &dn_mapping);
                    ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping, FALSE);
#ifdef SIM_3G_USIM_SUPPORT

                    if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                        && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                            && ((SIM_FILE_EF_MSISDN == dn_file_name)
                                || (SIM_FILE_EF_FDN == dn_file_name)
                                || (SIM_FILE_EF_SDN == dn_file_name)
                                || (SIM_FILE_EF_BDN == dn_file_name))))
                    {
                        if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                        {
                            is_in_adf_usim = FALSE;
                        }
                        else
                        {
                            is_in_adf_usim = TRUE;
                        }

                        SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                        ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping, FALSE);
                    }

#endif

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        SIMINSTR_ClearTempIntSignalQueue(cur_sim_task);
                    }
                    else
                    {
                        pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
                        goto SEND_CONFIRM;
                    }
                }
                else
                {
                    pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
                    goto SEND_CONFIRM;
                }
            }
        }
        else
        {
            pcnf->result = operate_result;
            goto SEND_CONFIRM;
        }

        //We will read this record out once we updated it.
        //Some SIM card cannot support some UCS2 coding
        //For example, we update a record with the alpha-string as "汉1234567890",
        //and the update is successfully, but the alpha-string become "汉12345" in SIM card,
        //So we'd better read it out and indicate the user.
        //And, we will only deal with the normal ADN/FDN/MSISDN.
#ifdef SIM_3G_USIM_SUPPORT

        if(((SIM_FILE_EF_ADN == dn_file_name) || (SIM_FILE_EF_MSISDN == dn_file_name) || (SIM_FILE_EF_FDN == dn_file_name)
            || (SIM_FILE_EF_TELECOM_PB_ADN == dn_file_name) || (SIM_FILE_EF_TELECOM_PB1_ADN == dn_file_name) || (SIM_FILE_EF_LND == dn_file_name)
            || (SIM_FILE_EF_USIM_PB_ADN == dn_file_name) || (SIM_FILE_EF_USIM_PB1_ADN == dn_file_name)))
#else
        if((SIM_FILE_EF_ADN == dn_file_name)
            || (SIM_FILE_EF_MSISDN == dn_file_name)
            || (SIM_FILE_EF_FDN == dn_file_name)
            || (SIM_FILE_EF_LND == dn_file_name))
#endif
        {
            SCI_MEMSET(&dn_mapping, 0xFF, sizeof(SIM_EF_MAPPING_DN_T));

            if(SIM_FILE_EF_LND == dn_file_name)
            {
                ser_result = SIM_SERVICE_ERROR_UNKNOW; // LND , we do not want to read EF record, so copy psig alpha id to pcnf
            }
            else
            {
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping);
#ifdef SIM_3G_USIM_SUPPORT

                if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                        && ((SIM_FILE_EF_MSISDN == dn_file_name)
                            || (SIM_FILE_EF_FDN == dn_file_name)
                            || (SIM_FILE_EF_SDN == dn_file_name)
                            || (SIM_FILE_EF_BDN == dn_file_name))))
                {
                    if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                    {
                        is_in_adf_usim = FALSE;
                    }
                    else
                    {
                        is_in_adf_usim = TRUE;
                    }

                    SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                    ser_result = SIMSER_ReadEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, &dn_mapping);
                }

#endif
            }

            if(SIM_SERVICE_OK == ser_result)
            {
                SCI_MEMCPY(pcnf->alpha_id, dn_mapping.alpha_id, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
                pcnf->alpha_id_len = dn_mapping.alpha_id_len;
            }
            else
            {
                //READ result cannot affect the UPDATE operation.
                //If read fail, we will notify user with the REQ's alpha-string. Because the update is OK.
                SCI_MEMCPY(pcnf->alpha_id, psig->dn.alpha_id, SIM_MAX_DN_ALPHA_IDENTIFY_LEN);
                pcnf->alpha_id_len = psig->dn.alpha_id_len;
            }
        }
    }
    else
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
    }


SEND_CONFIRM:
    //send the confirm signal to the signal requester
#ifdef SIM_FILL_PHONEBOOK_FULL//test code when do phonebook debug	
    if(s_sim_write_pb_full[cur_sim_task])
    {
        SCI_FREE_SIGNAL(pcnf);
    }
    else
#endif	
    {
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
    }
}

/**********************************************************************
//    Description:
//      the function that process the ERASE LND All request signal
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
void SIMDN_EraseLNDAll(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_LND_ALL_REQ_T  *psig    //the erasure LND request signal
)
{
    SIM_SIG_ERASE_LND_ALL_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_FILE_NAME_E dn_file_name;
    SIM_SERVICE_TYPE_PH_2_E dn_service;
    uint16 i = 0, j; //i must be uint16
    SIMDN_EXT_INFO_T    *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, SIM_DN_T_LND);
    BOOLEAN  is_record_num_exist = FALSE;
    uint8 ext1_record_to_be_delete[10] = {0};
    uint8 ext1_record_num = 0;
    uint8 temp_record_id[20] = {0};
    uint8 temp_index = 0;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_ERASE_LND_ALL_CNF, sizeof(SIM_SIG_ERASE_LND_ALL_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->result = SIM_DN_OK;
    pcnf->decreased_ext_num = 0;

    dn_file_name = GetDNFileName(SIM_DN_T_LND);
    dn_service = GetDNService(SIM_DN_T_LND);

    //check the corresponding service
    if(SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
    {
        for(i = 0; i < g_sim_card_setting[cur_sim_task].lnd_info.record_num; i++)
        {
            //mark the record as a empty record
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, 0, SIM_RECORD_PREVIOUS, NULL, TRUE);

            //the update operation failed,handle the error
            if(SIM_SERVICE_OK != ser_result)
            {
                pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
                break;
            }
        }

        //I will delele EXT1 info whatever the result of update LND EF is.
        if(PNULL != dn_ext_info_ptr)
        {
            //first, find out the valid extention record number
            for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
            {
                for(j = 0; j < SIM_MAX_EXT_NUM; j++)
                {
                    if((0XFF != *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j))
                        && (0 != *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j)))
                    {
                        temp_record_id[temp_index] = *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j);
                        *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i + j) = 0xFF;
                        temp_index++;
                    }
                }
            }

            //second, delete the repetitive
            for(i = 0; i < temp_index ; i++)
            {
                for(j = 0; j < ext1_record_num; j++)
                {
                    if(temp_record_id[i] == ext1_record_to_be_delete[j])
                    {
                        is_record_num_exist = TRUE;
                    }
                }

                if(!is_record_num_exist)
                {
                    ext1_record_to_be_delete[ext1_record_num] = temp_record_id[i];
                    ext1_record_num ++;
                }

                is_record_num_exist = FALSE;
            }

            pcnf->decreased_ext_num += ext1_record_num; //record the ext_num decreased

            for(i = 0; i < ext1_record_num; i++)
            {
                if(0 != SIMDN_IsExtidUsedbyOtherDnid(cur_sim_task, SIM_DN_T_LND, ext1_record_to_be_delete[i]))
                {
                    //如果还有其他DN 记录也在使用这条EXT 记录，那么不能告知上层
                    //现在有多一条EXT 记录可用了。
                    SCI_ASSERT(0 != pcnf->decreased_ext_num); /*assert verified:Check_value */
                    pcnf->decreased_ext_num--;//recover the num.
                }

#ifdef SIM_DELETE_REAL_EXT
                else
                {
                    //没有其他DN记录使用这条EXT记录，从物理上删除它。
                    SIM_EF_MAPPING_EXT_T delete_dn_ext;
                    dn_file_name = GetDNExtFileName(SIM_DN_T_LND);
                    SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, ext1_record_to_be_delete[i], SIM_RECORD_ABSOLUTE, &delete_dn_ext, TRUE);
                }

#endif
            }

        }
    }
    else
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

/**********************************************************************
//    Description:
//      the function that process the ERASE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_EraseDN(
    uint32 cur_sim_task,
    SIM_SIG_ERASE_DN_REQ_T  *psig    //the erasure dialling number request signal
)
{
    SIM_SIG_ERASE_DN_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result = SIM_SERVICE_ERROR_UNKNOW;
    SIM_FILE_NAME_E dn_file_name;
    SIM_SERVICE_TYPE_PH_2_E dn_service;
    int16 s_purge_flag_index;/*@zhangyongxia,CR4266,2003-8-29*/
    uint16 i = 0;//i must be uint16
    SIMDN_EXT_INFO_T    *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, psig->dn_type);
    uint32              pos_offset       = 0;
    SIM_EF_MAPPING_DN_T dn_mapping;
    uint8 dn_id = (uint8)psig->dn_id;
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif
    uint8 temp_ext_id[SIM_MAX_EXT_NUM] = {0};

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_ERASE_DN_CNF, sizeof(SIM_SIG_ERASE_DN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->dn_id = psig->dn_id;
    pcnf->dn_type = psig->dn_type;
    pcnf->result = SIM_DN_OK;
    pcnf->decreased_ext_num = 0;

    if((0 == psig->dn_id) && (SIM_DN_T_LND != psig->dn_type))
    {
        pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
        //send the confirm signal to the signal requestor
        SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
        return;
    }

    dn_file_name = GetDNFileName(psig->dn_type);
    dn_service = GetDNService(psig->dn_type);

    //check the corresponding service
    if(SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
    {

#ifdef SIM_3G_USIM_SUPPORT

        if(SIM_DN_T_ADN == psig->dn_type)
        {
            dn_id = 0;
            dn_file_name = SIMDN_RemapDFpbADN(cur_sim_task, psig->dn_id, &dn_id);
        }

#endif

        //the EFlnd is a cyclic EF,that's mean the record mode of the update operation
        //on the EFlnd only can be SIM_RECORD_PREVIOUS.
        if(SIM_DN_T_LND == psig->dn_type)
        {
            //mark the record as a empty record
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, 0, SIM_RECORD_PREVIOUS, NULL, TRUE);
        }
        else
        {
            //mark the record as a empty record
            ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, NULL, TRUE);
#ifdef SIM_3G_USIM_SUPPORT

            if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                    && ((SIM_FILE_EF_MSISDN == dn_file_name)
                        || (SIM_FILE_EF_FDN == dn_file_name)
                        || (SIM_FILE_EF_SDN == dn_file_name)
                        || (SIM_FILE_EF_BDN == dn_file_name))))
            {
                if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                {
                    is_in_adf_usim = FALSE;
                }
                else
                {
                    is_in_adf_usim = TRUE;
                }

                SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                ser_result = SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, dn_id, SIM_RECORD_ABSOLUTE, NULL, TRUE);
            }

#endif
        }

        //the update operation failed,handle the error
        if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
        }

        /*@zhangyongxia,CR4266,2003-8-29*/
        s_purge_flag_index = GetDNPurgeFlagIndex(psig->dn_type);

        if(pcnf->result == SIM_DN_OK)
        {
#ifndef SIM_CODE_COMPACT
            prev_purge_fail[cur_sim_task][s_purge_flag_index] = FALSE;
#endif

            //update the table
            if(SIM_DN_T_LND == pcnf->dn_type)
            {
                // for LND, the entry id is invalid
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_LND, 0, SIM_RECORD_PREVIOUS, &dn_mapping);

                if((SIM_SERVICE_OK == ser_result) && (PNULL != dn_ext_info_ptr))
                {
                    pos_offset = 0;

                    for(i = 0; i < dn_ext_info_ptr->dn_max_num; i++)
                    {
                        if(dn_mapping.is_ext_exist)
                        {
                            if(dn_mapping.ext_id == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i))
                            {
                                pos_offset = i * SIM_MAX_EXT_NUM;
                                break;
                            }
                        }
                        else
                        {
                            if((0XFF == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i))
                                || (0 == *(dn_ext_info_ptr->ext_list_ptr + SIM_MAX_EXT_NUM * i)))
                            {
                                pos_offset = i * SIM_MAX_EXT_NUM;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    pcnf->result = SIM_DN_READ_FILE_ERROR;
                }
            }
            else
            {
                pos_offset = SIM_MAX_EXT_NUM * (pcnf->dn_id - 1);
            }

            if((SIM_SERVICE_OK == ser_result) && (PNULL != dn_ext_info_ptr))
            {
                for(i = 0; i < SIM_MAX_EXT_NUM; i++)
                {
                    temp_ext_id[i] = *(dn_ext_info_ptr->ext_list_ptr + pos_offset + i);

                    if(0xFF != temp_ext_id[i])
                    {
                        //该条dn_id 有EXT 记录
                        pcnf->decreased_ext_num++;//record the ext_num decreased
                    }

                    *(dn_ext_info_ptr->ext_list_ptr + pos_offset + i) = 0xFF;//clear the ext_id

                    if(0 != SIMDN_IsExtidUsedbyOtherDnid(cur_sim_task, psig->dn_type, temp_ext_id[i]))
                    {
                        //如果还有其他DN 记录也在使用这条EXT 记录，那么不能告知上层
                        //现在有多一条EXT 记录可用了。
                        pcnf->decreased_ext_num--;//recover the num.
                    }

#ifdef SIM_DELETE_REAL_EXT
                    else
                    {
                        //没有其他DN记录使用这条EXT记录，从物理上删除它。
                        {
                            SIM_EF_MAPPING_EXT_T delete_dn_ext;
                            dn_file_name = GetDNExtFileName(psig->dn_type);

                            if((0xFF != temp_ext_id[i]) && (0x0 != temp_ext_id[i]))
                            {
                                SIMSER_UpdateEFRecord(cur_sim_task, dn_file_name, temp_ext_id[i], SIM_RECORD_ABSOLUTE, &delete_dn_ext, TRUE);
                            }
                        }
                    }

#endif
                }
            }
        }
    }
    else
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function that process the PURGE Dialling number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_PurgeDN(
    uint32 cur_sim_task,
    SIM_SIG_PURGE_DN_REQ_T  *psig    //the purge dialling number request signal
)
{
    SIM_SIG_PURGE_DN_CNF_T *pcnf;
    SIM_SERVICE_TYPE_PH_2_E dn_service;
    int16 s_purge_flag_index;/*@zhangyongxia,CR4266,2003-8-29*/

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_PURGE_DN_CNF, sizeof(SIM_SIG_PURGE_DN_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->dn_type = psig->dn_type;

    dn_service = GetDNService(psig->dn_type);

    if(SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
    {
        /*@zhangyongxia,CR4266,2003-8-29*/
        s_purge_flag_index = GetDNPurgeFlagIndex(psig->dn_type);

        if(!prev_purge_fail[cur_sim_task][s_purge_flag_index])
        {
            pcnf->result = PurgeDiallingNumber(cur_sim_task, psig->dn_type);
        }
        else
        {
            pcnf->result = SIM_DN_UPDATE_FILE_ERROR;
        }
    }
    else
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
    }

    //send the confirm signal to the signal requestor
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the 3G USIM FDN service active/deactive. 31.102 5.3.2
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMDN_USIMFDNService(
    uint32 cur_sim_task,
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type
)
{
    BOOLEAN is_fdn_enable = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_EST_T est;

    if((SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_FDN))
        && (SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EST)))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

        if(SIM_SERVICE_OK == ser_result)
        {
            if(SIM_DN_DISALBING == service_type)
            {
                if(est.is_FDN_enable)
                {
                    est.is_FDN_enable = FALSE;
                    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        is_fdn_enable = FALSE;
                    }
                    else
                    {
                        //disable FDN failed, FDN is still enabled.
                        is_fdn_enable = TRUE;
                    }
                }
                else
                {
                    //FDN is already disabled
                    is_fdn_enable = FALSE;
                }
            }
            else if(SIM_DN_ENABLING == service_type)
            {
                if(est.is_FDN_enable)
                {
                    //FDN is already enabled
                    is_fdn_enable = TRUE;
                }
                else
                {
                    est.is_FDN_enable = TRUE;
                    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        is_fdn_enable = TRUE;
                    }
                    else
                    {
                        //enable FDN failed
                        is_fdn_enable = FALSE;
                    }
                }
            }
            else//SIM_DN_CAPABILITY
            {
                is_fdn_enable = est.is_FDN_enable;
            }
        }
    }

    return is_fdn_enable;
}
#endif//SIM_3G_USIM_SUPPORT

/**********************************************************************
//    Description:
//      the function that process the FDN enable/disable check
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_FDNCapabilityCheck(
    uint32 cur_sim_task
)
{
    BOOLEAN is_fdn_enable = FALSE;
    SIM_SERVICE_RESULT_E  ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        is_fdn_enable = SIMDN_USIMFDNService(cur_sim_task, SIM_DN_CAPABILITY);
    }
    else
#endif//SIM_3G_USIM_SUPPORT
    {
        //the FDN is actived
        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_FDN))
        {
            //if ADN is not actived,service 3 is enabled
            if(!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_ADN))
            {
                ////////pcnf->is_enable = TRUE;
                //there may be no ADN, so we'd better to SELECT FDN.
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_FDN, &select_data);

                if(SIM_SERVICE_OK == ser_result)
                {
                    if(0 == ((select_data.file_status) & SIM_FILE_INVALIDATE_MARK))
                    {
                        is_fdn_enable = FALSE;
                    }
                    else
                    {
                        is_fdn_enable = TRUE;
                    }
                }
                else
                {
                    is_fdn_enable = FALSE;
                }
            }
            //if ADN is activated,Then check the response data of EFadn,if EFadn if invalidate.
            //in all other case service 3 id disabled.
            else
            {
                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_ADN, &select_data);

                if(SIM_SERVICE_OK != ser_result)
                {
                    if(SIM_SERVICE_SELECT_NO_SUCH_FILE == ser_result)
                    {
                        is_fdn_enable = FALSE;
                    }
                    else
                    {
                        //error handle
                        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_FDN, &select_data);

                        if(SIM_SERVICE_OK == ser_result)
                        {
                            if(0 == ((select_data.file_status) & SIM_FILE_INVALIDATE_MARK))
                            {
                                is_fdn_enable = FALSE;
                            }
                            else
                            {
                                is_fdn_enable = TRUE;
                            }
                        }
                        else
                        {
                            is_fdn_enable = FALSE;
                        }
                    }
                }
                else
                {
                    if(0 == ((select_data.file_status) & SIM_FILE_INVALIDATE_MARK))
                    {
                        is_fdn_enable = TRUE;
                    }
                    else
                    {
                        is_fdn_enable = FALSE;
                    }
                }
            }
        }
        else
        {
            //No FDN, no FDN service.
            is_fdn_enable = FALSE;
        }
    }

    return is_fdn_enable;
}

/**********************************************************************
//    Description:
//      the function that process the FDN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_FDNService(
    uint32 cur_sim_task,
    SIM_SIG_FDN_SERVICE_REQ_T  *psig    //the FDN service request signal
)
{
    BOOLEAN need_check_flag = FALSE;
    SIM_SIG_FDN_SERVICE_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_FDN_SERVICE_CNF, sizeof(SIM_SIG_FDN_SERVICE_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->service_type = psig->service_type;
    pcnf->result = SIM_DN_OK;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        if((SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_FDN))
            && (SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EST)))
        {
            pcnf->is_fdn_exist = TRUE;
            pcnf->is_enable = SIMDN_USIMFDNService(cur_sim_task, psig->service_type);
        }
        else
        {
            pcnf->is_fdn_exist = FALSE;
            pcnf->is_enable = FALSE;
        }
    }
    else
#endif//SIM_3G_USIM_SUPPORT
    {
        if(SIM_DN_ENABLING == psig->service_type)
        {
            //The Invalidate/Rehabilitate flag of the EFadn,which is implicitly cleared
            //by the INVLAIDATE command,is at the same time the indicator for the state
            //of the service 3.
            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_ADN))
            {
                ser_result = SIMSER_InvalidateEF(cur_sim_task, SIM_FILE_EF_ADN);

                if(SIM_SERVICE_OK != ser_result)
                {
                    pcnf->result = SIM_DN_INVALIDATE_FILE_ERROR;
                    //the enabling operation failed,so the service keep on disable
                    pcnf->is_enable = FALSE;
                }
                else
                {
                    pcnf->is_enable = TRUE;
                }

                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_TELECOM, &select_data);
            }
            else
            {
                need_check_flag = TRUE;
            }
        }
        else if(SIM_DN_DISALBING == psig->service_type)
        {
            //The invalidate/rehabilitate flag of EFadn,which  is implicit set by the
            //REHABILITATE command,is at the same time the indicator for the state of the
            //the service n~3.
            if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_ADN))
            {
                ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_ADN);

                if(SIM_SERVICE_OK != ser_result)
                {
                    pcnf->result = SIM_DN_REHABILITATE_FILE_ERROR;
                    //the disabling operation failed,so the service keep on enable
                    pcnf->is_enable = TRUE;
                }
                else
                {
                    pcnf->is_enable = FALSE;
                }

                ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_DF_TELECOM, &select_data);
            }
            else
            {
                need_check_flag = TRUE;
            }
        }
        else//SIM_DN_CAPABILITY
        {
            need_check_flag = TRUE;
        }

        if(need_check_flag)
        {
            pcnf->is_enable = SIMDN_FDNCapabilityCheck(cur_sim_task);
        }

        if(SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_FDN))
        {
            pcnf->is_fdn_exist = TRUE;
        }
        else
        {
            pcnf->is_fdn_exist = FALSE;
        }
    }

    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      the function that process the 3G USIM BDN service active/deactive. 31.102 5.3.2
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC BOOLEAN SIMDN_USIMBDNService(
    uint32 cur_sim_task,
    SIM_DN_SERVICE_OPERATION_TYPE_E service_type
)
{
    BOOLEAN is_bdn_enable = FALSE;
    SIM_SERVICE_RESULT_E ser_result;
    SIM_EF_MAPPING_EST_T est;

    if((SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_BDN))
        && (SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_EST)))
    {
        ser_result = SIMSER_ReadWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

        if(SIM_SERVICE_OK == ser_result)
        {
            if(SIM_DN_DISALBING == service_type)
            {
                if(est.is_BDN_enable)
                {
                    est.is_BDN_enable = FALSE;
                    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        is_bdn_enable = FALSE;
                    }
                    else
                    {
                        //disable BDN failed, BDN is still enabled.
                        is_bdn_enable = TRUE;
                    }
                }
                else
                {
                    //BDN is already disabled
                    is_bdn_enable = FALSE;
                }
            }
            else if(SIM_DN_ENABLING == service_type)
            {
                if(est.is_BDN_enable)
                {
                    //BDN is already enabled
                    is_bdn_enable = TRUE;
                }
                else
                {
                    est.is_BDN_enable = TRUE;
                    ser_result = SIMSER_UpdateWholeTransparentEF(cur_sim_task, SIM_FILE_EF_USIM_EST, &est);

                    if(SIM_SERVICE_OK == ser_result)
                    {
                        is_bdn_enable = TRUE;
                    }
                    else
                    {
                        //enable BDN failed, BDN is still disabled.
                        is_bdn_enable = FALSE;
                    }
                }
            }
            else//SIM_DN_CAPABILITY
            {
                is_bdn_enable = est.is_BDN_enable;
            }
        }
    }

    return is_bdn_enable;
}
#endif//SIM_3G_USIM_SUPPORT

/**********************************************************************
//    Description:
//      the function that process the BDN enable/disable check
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_BDNCapabilityCheck(
    uint32 cur_sim_task
)
{
#if 0
    BOOLEAN is_bdn_enable = FALSE;
    SIM_SERVICE_RESULT_E  ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        is_bdn_enable = SIMDN_USIMBDNService(cur_sim_task, SIM_DN_CAPABILITY);
    }
    else
#endif//SIM_3G_USIM_SUPPORT
    {
        //BDN service is "Enabled" only if service 31 is allocated and activated and
        //EFbdn is not invalidated.in all other case,the BDN service is "disabled"
        if((SIM_PH_1 == g_sim_card_setting[cur_sim_task].card_phase)
            || (!SIMEF_IsServiceEnabled(cur_sim_task, SIMSER_PH2_BDN)))
        {
            is_bdn_enable = FALSE;
        }
        else
        {
            ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_BDN, &select_data);

            if(SIM_SERVICE_OK != ser_result)
            {
                is_bdn_enable = FALSE;
            }
            else
            {
                //decode the select data
                if(0 == ((select_data.file_status) & SIM_FILE_INVALIDATE_MARK))
                {
                    is_bdn_enable = FALSE;
                }
                else
                {
                    is_bdn_enable = TRUE;
                }
            }
        }
    }

    return is_bdn_enable;
#endif
    return FALSE;
}

/**********************************************************************
//    Description:
//      the function that process the BDN service request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_BDNService(
    uint32 cur_sim_task,
    SIM_SIG_BDN_SERVICE_REQ_T    *psig    //the BDN service request signal
)
{
#if 0
    SIM_SIG_BDN_SERVICE_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;

    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_BDN_SERVICE_CNF, sizeof(SIM_SIG_BDN_SERVICE_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref = psig->command_ref;
    pcnf->service_type = pcnf->service_type;
    pcnf->result = SIM_DN_OK;

#ifdef SIM_3G_USIM_SUPPORT

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        pcnf->is_enable = SIMDN_USIMBDNService(cur_sim_task, psig->service_type);
    }
    else
#endif//SIM_3G_USIM_SUPPORT
    {
        if(SIM_DN_ENABLING == psig->service_type)
        {
            //BDN service is "Enabled" only if service 31 is allocated and activated and
            //EFbdn is not invalidated.in all other case,the BDN service is "disabled"
            if((SIM_PH_1 == g_sim_card_setting[cur_sim_task].card_phase)
                || (!g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_BDN].allocated))
            {
                pcnf->is_enable = FALSE;
                pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
            }
            else
            {
                //To enable the BDN capability,just to rehabilitate EFbdn.
                ser_result = SIMSER_RehabilitateEF(cur_sim_task, SIM_FILE_EF_BDN);

                if(SIM_SERVICE_OK != ser_result)
                {
                    pcnf->result = SIM_DN_REHABILITATE_FILE_ERROR;
                    //the enabling operation failed,so the service keep on disable
                    pcnf->is_enable = FALSE;
                }
                else
                {
                    pcnf->is_enable = TRUE;
                }
            }
        }
        else if(SIM_DN_DISALBING == psig->service_type)
        {
            //BDN service is "Enabled" only if service 31 is allocated and activated and
            //EFbdn is not invalidated.in all other case,the BDN service is "disabled"
            if((SIM_PH_1 == g_sim_card_setting[cur_sim_task].card_phase)
                || (!g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_BDN].allocated))
            {
                pcnf->is_enable = FALSE;
                pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
            }
            else
            {
                //to diable the BDN capability,just invalidate EFbdn.
                ser_result = SIMSER_InvalidateEF(cur_sim_task, SIM_FILE_EF_BDN);

                if(SIM_SERVICE_OK != ser_result)
                {
                    pcnf->result = SIM_DN_INVALIDATE_FILE_ERROR;
                    //the disabling operation failed,so the service keep on enable
                    pcnf->is_enable = TRUE;
                }
                else
                {
                    pcnf->is_enable = FALSE;
                }
            }
        }
        else//SIM_DN_CAPABILITY
        {
            pcnf->is_enable = SIMDN_BDNCapabilityCheck(cur_sim_task);
        }
    }

    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
#endif
}

#ifndef SIM_CODE_COMPACT
/**********************************************************************
//    Description:
//      the function that process the get free ext number number request signal
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMDN_GetFreeExtInfo(
    uint32 cur_sim_task,
    SIM_SIG_GET_FREE_EXT_INFO_REQ_T *psig   //the get free ext number request signal
)
{
    SIM_SIG_GET_FREE_EXT_INFO_CNF_T *pcnf;
    SIM_SERVICE_RESULT_E  ser_result;
    SIM_DN_OPERATION_RESULT_E operate_result;
    SIM_FILE_NAME_E ext_file_name;
    SIM_SERVICE_TYPE_PH_2_E dn_service, ext_service;
    SIM_RESPONSE_DATA_OF_SELECT_T select_data;
    uint8 i, ext_id[255];
    uint8 empty_ext[SIM_EF_EXT_RECORD_LEN];
    uint16 data_len;
    uint16 total_record_num_of_ext;
    BOOLEAN is_purged = FALSE;
    uint8 record_num;
    uint8 record[255];
#ifdef SIM_3G_USIM_SUPPORT
    BOOLEAN is_in_adf_usim = FALSE;
#endif

#ifndef _LESSEN_TRACE_SUPPORT
    //SCI_TRACE_LOW:"SIM::enter SIMDN_GetFreeExtInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_2706_112_2_17_22_58_22_4366, (uint8 *)"");
#endif
    //create the confirm signal for the request signal
    SCI_CREATE_SIGNAL(pcnf, SIM_GET_FREE_EXT_INFO_CNF, sizeof(SIM_SIG_GET_FREE_EXT_INFO_CNF_T), g_sim_task_id[cur_sim_task]);
    pcnf->command_ref   = psig->command_ref;
    pcnf->dn_type       = psig->dn_type;
    pcnf->result    = SIM_DN_OK;
    pcnf->free_ext_record_num = 0;

    //get the relative information of the dialling number
    ext_file_name    = GetDNExtFileName(psig->dn_type);
    dn_service      = GetDNService(psig->dn_type);
    ext_service     = GetDNExtService(psig->dn_type);

    if((!SIMEF_IsServiceEnabled(cur_sim_task, dn_service))
        || (!SIMEF_IsServiceEnabled(cur_sim_task, ext_service)))
    {
        pcnf->result = SIM_DN_SERVICE_NOT_ENABLE;
        goto SEEK_FREE_EXT_END;
    }

    //make a empty extension record used to seek in EF_ext
    SIMFS_EncodeEFFile(cur_sim_task, ext_file_name, &data_len, empty_ext, NULL, TRUE);

    //get the record number of the extension file
    ser_result = SIMSER_SelectFile(cur_sim_task, ext_file_name, &select_data);

    if(SIM_SERVICE_OK != ser_result)
    {
        pcnf->result = SIM_DN_SELECT_FILE_ERROR;
        goto SEEK_FREE_EXT_END;
    }

    total_record_num_of_ext = select_data.freespace_filesize / select_data.dfnumber_recordlen;
    //SCI_TRACE_LOW:"SIM::total record number of extfile 0x%x is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_2738_112_2_17_22_58_22_4367, (uint8 *)"dd", ext_file_name, total_record_num_of_ext);

SEEK_FREE_EXT_START:
    i = 0;
    pcnf->free_ext_record_num = 0;

    //continue to find other empty extension record if needed
    while(i < total_record_num_of_ext)
    {
        if(i == 0)
        {
            //seek the first record from the first record of the extension file
            ser_result = SIMSER_SeekInFile(cur_sim_task, ext_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_BEGINNING_FORWARD,
                                           SIM_EF_EXT_RECORD_LEN, empty_ext, &record_num, record);
            ext_id[i] = record[0];
#ifdef SIM_3G_USIM_SUPPORT

            if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                    && ((SIM_FILE_EF_EXT2 == ext_file_name)
                        || (SIM_FILE_EF_EXT3 == ext_file_name))))
            {
                if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                {
                    is_in_adf_usim = FALSE;
                }
                else
                {
                    is_in_adf_usim = TRUE;
                }

                SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                ser_result = SIMSER_SeekInFile(cur_sim_task, ext_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_BEGINNING_FORWARD,
                                               SIM_EF_EXT_RECORD_LEN, empty_ext, &record_num, record);
                ext_id[i] = record[0];
            }

#endif
        }
        else
        {
            //seek other record from the next record
            ser_result = SIMSER_SeekInFile(cur_sim_task, ext_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_NEXT_FORWARD,
                                           SIM_EF_EXT_RECORD_LEN, empty_ext, &record_num, record);
            ext_id[i] = record[0];
#ifdef SIM_3G_USIM_SUPPORT

            if((USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                && ((SIM_SEC_STATUS_NOT_SATISFIED == ser_result)
                    && ((SIM_FILE_EF_EXT2 == ext_file_name)
                        || (SIM_FILE_EF_EXT3 == ext_file_name))))
            {
                if(SIMPB_GetOtherDNLocationDFtel(cur_sim_task))
                {
                    is_in_adf_usim = FALSE;
                }
                else
                {
                    is_in_adf_usim = TRUE;
                }

                SIMPB_SetOtherDNLocationDFtel(cur_sim_task, is_in_adf_usim);
                ser_result = SIMSER_SeekInFile(cur_sim_task, ext_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_NEXT_FORWARD,
                                               SIM_EF_EXT_RECORD_LEN, empty_ext, &record_num, record);
                ext_id[i] = record[0];
            }

#endif
        }

#ifndef _LESSEN_TRACE_SUPPORT
        //SCI_TRACE_LOW:"SIM::seek result = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_2803_112_2_17_22_58_22_4368, (uint8 *)"d", ser_result);
#endif

        //can not find any empty extension record
        if(SIM_SERVICE_SEEK_NOT_FIND_PATTERN == ser_result)
        {
            //if has find free ext record
            if(pcnf->free_ext_record_num != 0)
            {
                pcnf->result = SIM_DN_OK;
                goto SEEK_FREE_EXT_END;
            }

            //the purge operation has been successfully execute
            if(is_purged)
            {
                pcnf->result = SIM_DN_EXT_MEM_FULL;
                goto SEEK_FREE_EXT_END;
            }

            //if still not purged,purge the extension file
            operate_result = PurgeDiallingNumber(cur_sim_task, psig->dn_type);

            if(SIM_DN_OK == operate_result)
            {
                is_purged = TRUE;
                goto SEEK_FREE_EXT_START;
            }
            //purge extension file failed
            else
            {
                pcnf->result = operate_result;
                goto SEEK_FREE_EXT_END;
            }
        }
        //the seek operate failed for some other reason
        else if(SIM_SERVICE_OK != ser_result)
        {
            pcnf->result = SIM_DN_SEEK_EXT_ERROR;
            goto SEEK_FREE_EXT_END;
        }

        pcnf->free_ext_record_num++;
        i++;
    }

SEEK_FREE_EXT_END:
    //SCI_TRACE_LOW:"SIM::result is 0x%x,free ext record number is 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_2848_112_2_17_22_58_22_4369, (uint8 *)"dd", pcnf->result, pcnf->free_ext_record_num);
    //send the confirm signal to the signal requester
    SCI_SEND_SIGNAL((xSignalHeaderRec *)pcnf, ((xSignalHeaderRec *)psig)->Sender);
}
#endif

/**********************************************************************
//    Description:
//      If a PB record' name is empty, copy the phone number to the name.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC void SIMDN_ReMapEmptyNameDN(uint32 cur_sim_task, SIM_EF_MAPPING_DN_T *dn_mapping)
{
    uint8 i = 0;
    uint8 num_of_numbers = 0;
    uint8 low_4_bits = 0;
    uint8 high_4_bits = 0;
    uint8 real_alpha_id_len = dn_mapping->alpha_id_len;
    uint8 real_number_len = dn_mapping->dn_len;
    uint8 *ptemp_alpha_id = dn_mapping->alpha_id;
    uint8 loop_times = 0;

    if(0 == real_alpha_id_len)
    {
        real_alpha_id_len = g_sim_card_setting[cur_sim_task].adn_info.alpha_id_len;

        if(real_alpha_id_len > SIM_MAX_DN_ALPHA_IDENTIFY_LEN)
        {
            real_alpha_id_len = SIM_MAX_DN_ALPHA_IDENTIFY_LEN;
        }

        real_number_len = dn_mapping->dn_len;

        if(real_number_len > SIM_EF_DN_DN_LEN)
        {
            real_number_len = SIM_EF_DN_DN_LEN;
        }

        loop_times = MIN(real_alpha_id_len / 2, real_number_len);

        for(i = 0; i < loop_times; i++)
        {
            low_4_bits = dn_mapping->dialling_num[i] & 0xf;

            if(low_4_bits <= 9)//It should  be numbers.
            {
                *ptemp_alpha_id = low_4_bits + '0';//number to ascii
                num_of_numbers++;
            }
            else
            {
                *ptemp_alpha_id =  0xff;
            }

            ptemp_alpha_id++;

            high_4_bits = (dn_mapping->dialling_num[i] >> 4);

            if(high_4_bits <= 9)//It should  be numbers.
            {
                *ptemp_alpha_id = high_4_bits + '0';//number to ascii
                num_of_numbers++;
            }
            else
            {
                *ptemp_alpha_id =  0xff;
            }

            ptemp_alpha_id++;
        }

        dn_mapping->alpha_id_len = num_of_numbers;
    }
}

#ifdef SIM_3G_USIM_SUPPORT
/**********************************************************************
//    Description:
//      Set wether the EFadn in DFtelecom/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinDFteleDFpb(uint32 cur_sim_task, BOOLEAN is_adn_exist, uint8 adn_num, uint16 *adn_id, uint16 ext1_id, uint16 *iap_id, uint16 *pbc_id, uint16 *grp_id, uint16 aas_id, uint16 gas_id, uint16 *anra_id, uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 ccp1_id, uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id)
{
    uint8 i=0;
    
    s_sim_is_telecom_pb_exist[cur_sim_task] = is_adn_exist;

    SCI_TRACE_LOW("SIM%d: SIMDN_SetIsADNinDFteleDFpb is_adn_exist = %d",cur_sim_task,is_adn_exist);

    if(is_adn_exist)
    {
        s_sim_usim_telecom_pb_adn_num[cur_sim_task] = adn_num;

        if(s_sim_usim_telecom_pb_adn_num[cur_sim_task] > SIM_USIM_PB_SET_NUM)
        {
            s_sim_usim_telecom_pb_adn_num[cur_sim_task] = SIM_USIM_PB_SET_NUM;
        }

        for(i = 0; i < s_sim_usim_telecom_pb_adn_num[cur_sim_task]; i++)
        {
            s_sim_usim_telecom_pb_iap_id[cur_sim_task][i] = *iap_id;
            iap_id++;

            s_sim_usim_telecom_pb_adn_id[cur_sim_task][i] = *adn_id;
            adn_id++;

            s_sim_usim_telecom_pb_pbc_id[cur_sim_task][i] = *pbc_id;
            pbc_id++;

            s_sim_usim_telecom_pb_grp_id[cur_sim_task][i] = *grp_id;
            grp_id++;

            s_sim_usim_telecom_pb_anra_id[cur_sim_task][i] = *anra_id;
            anra_id++;

            s_sim_usim_telecom_pb_anrb_id[cur_sim_task][i] = *anrb_id;
            anrb_id++;

            s_sim_usim_telecom_pb_anrc_id[cur_sim_task][i] = *anrc_id;
            anrc_id++;

            s_sim_usim_telecom_pb_sne_id[cur_sim_task][i] = *sne_id;
            sne_id++;

            s_sim_usim_telecom_pb_uid_id[cur_sim_task][i] = *uid_id;
            uid_id++;

            s_sim_usim_telecom_pb_emaila_id[cur_sim_task][i] = *emaila_id;
            emaila_id++;

            s_sim_usim_telecom_pb_emailb_id[cur_sim_task][i] = *emailb_id;
            emailb_id++;
            SCI_TRACE_LOW("SIM%d:DN_SetIsADNinDFteleDFpb ADN 0x%x, EXT1 0x%x. %d sets", cur_sim_task, s_sim_usim_telecom_pb_adn_id[cur_sim_task][i], ext1_id, adn_num);
        }

        s_sim_usim_telecom_pb_ext1_id[cur_sim_task] = ext1_id;
        s_sim_usim_telecom_pb_aas_id[cur_sim_task] = aas_id;
        s_sim_usim_telecom_pb_gas_id[cur_sim_task] = gas_id;
        s_sim_usim_telecom_pb_ccp1_id[cur_sim_task] = ccp1_id;
    }
}

/**********************************************************************
//    Description:
//      Get wether the EFadn in DFtelecom/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinDFteleDFpb(
    uint32 cur_sim_task,
    uint8 *adn_num,
    uint16 *adn_id, uint16 *ext1_id, uint16 *iap_id, uint16 *pbc_id,
    uint16 *grp_id, uint16 *aas_id, uint16 *gas_id, uint16 *anra_id,
    uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 *ccp1_id,
    uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id)
{
    uint8 i = 0;

    if(s_sim_is_telecom_pb_exist[cur_sim_task])
    {
        *adn_num = s_sim_usim_telecom_pb_adn_num[cur_sim_task];

        for(i = 0; i < s_sim_usim_telecom_pb_adn_num[cur_sim_task]; i++)
        {
            *iap_id = s_sim_usim_telecom_pb_iap_id[cur_sim_task][i];
            iap_id++;

            *adn_id = s_sim_usim_telecom_pb_adn_id[cur_sim_task][i];
            adn_id++;

            *pbc_id = s_sim_usim_telecom_pb_pbc_id[cur_sim_task][i];
            pbc_id++;

            *grp_id = s_sim_usim_telecom_pb_grp_id[cur_sim_task][i];
            grp_id++;

            *anra_id = s_sim_usim_telecom_pb_anra_id[cur_sim_task][i];
            anra_id++;

            *anrb_id = s_sim_usim_telecom_pb_anrb_id[cur_sim_task][i];
            anrb_id++;

            *anrc_id = s_sim_usim_telecom_pb_anrc_id[cur_sim_task][i];
            anrc_id++;

            *sne_id = s_sim_usim_telecom_pb_sne_id[cur_sim_task][i];
            sne_id++;

            *uid_id = s_sim_usim_telecom_pb_uid_id[cur_sim_task][i];
            uid_id++;

            *emaila_id = s_sim_usim_telecom_pb_emaila_id[cur_sim_task][i];
            emaila_id++;

            *emailb_id = s_sim_usim_telecom_pb_emailb_id[cur_sim_task][i];
            emailb_id++;
        }

        *ext1_id = s_sim_usim_telecom_pb_ext1_id[cur_sim_task];
        *aas_id = s_sim_usim_telecom_pb_aas_id[cur_sim_task];
        *gas_id = s_sim_usim_telecom_pb_gas_id[cur_sim_task];
        *ccp1_id = s_sim_usim_telecom_pb_ccp1_id[cur_sim_task];
    }

    return s_sim_is_telecom_pb_exist[cur_sim_task];
}

/**********************************************************************
//    Description:
//      Get wether we the EFadn in DFtelecom is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinDFtele(uint32 cur_sim_task,uint8 *adn_num)
{
    *adn_num = 1;
    return s_sim_is_telecom_adn_exist[cur_sim_task];
}

/**********************************************************************
//    Description:
//      Set wether the EFadn in DFtelecom is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinDFtele(uint32 cur_sim_task, BOOLEAN is_adn_exist)
{
    s_sim_is_telecom_adn_exist[cur_sim_task] = is_adn_exist;

    SCI_TRACE_LOW("SIM%d: SIMDN_SetIsADNinDFtele is_adn_exist = %d",cur_sim_task,is_adn_exist);
}
/**********************************************************************
//    Description:
//     Accoding to the absolute adn record index, find oud the exact EFadn file name and index.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
STATIC SIM_FILE_NAME_E SIMDN_RemapDFpbADN(uint32 cur_sim_task, uint16 adn_rec_index, uint8 *remaped_adn_rec_index)
{
    SIM_FILE_NAME_E remaped_adn_name = SIM_FILE_EF_ADN;
    uint8 i, adn_num = 0;
    uint16 iap_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 adn_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 pbc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 grp_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anra_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 sne_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 uid_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emaila_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emailb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 ext1_file_id, aas_id, gas_id, ccp1_id = 0;
    uint16 temp_remaped_adn_rec_index = 0;

    temp_remaped_adn_rec_index = adn_rec_index;

    if(SIMDN_GetIsADNinDFteleDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        for(i = 0; i < SIM_USIM_PB_SET_NUM; i++)
        {
            if(temp_remaped_adn_rec_index <= s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][i])
            {
                break;//quit the loop. An usim 1st ADN has 250 records, the 2nd ADN has only 50 records.
            }
            else
            {
                temp_remaped_adn_rec_index -= s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][i];
            }
        }

        if(1 == i)
        {
            remaped_adn_name = SIM_FILE_EF_TELECOM_PB1_ADN;
        }

#if 0
        else if(2 == adn_ef_index)
        {
            *remaped_adn_rec_index = adn_rec_index - (adn_ef_index * (g_sim_card_setting[cur_sim_task].adn_info.record_num));
            remaped_adn_name = SIM_FILE_EF_TELECOM_PB2_ADN;
        }

#endif
        else
        {
            remaped_adn_name = SIM_FILE_EF_TELECOM_PB_ADN;
        }
    }
    else if(SIMDN_GetIsADNinDFtele(cur_sim_task, &adn_num))
    {
        remaped_adn_name = SIM_FILE_EF_ADN;
    }
    else if(SIMDN_GetIsADNinADFusimDFpb(cur_sim_task, &adn_num, \
                                              &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id, \
                                              &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        for(i = 0; i < SIM_USIM_PB_SET_NUM; i++)
        {
            if(temp_remaped_adn_rec_index <= s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][i])
            {
                break;//quit the loop. An usim 1st ADN has 250 records, the 2nd ADN has only 50 records.
            }
            else
            {
                temp_remaped_adn_rec_index -= s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][i];
            }
        }

        if(1 == i)
        {
            remaped_adn_name = SIM_FILE_EF_USIM_PB1_ADN;
        }

        else
        {
            remaped_adn_name = SIM_FILE_EF_USIM_PB_ADN;
        }
    }
    SCI_TRACE_LOW("SIM%d DN GetDNFileName 0x%x, remap from %d to %d", cur_sim_task, remaped_adn_name, adn_rec_index, temp_remaped_adn_rec_index);
    SCI_ASSERT((temp_remaped_adn_rec_index <= 255) && (0 != temp_remaped_adn_rec_index));/*assert verified:Check_value */
    
    *remaped_adn_rec_index = (uint8)temp_remaped_adn_rec_index;
    return remaped_adn_name;
}

/**********************************************************************
//    Description:
//     Get the EFpbr infomation.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_GetPBRInfo(uint32 cur_sim_task)
{
    SIM_SERVICE_RESULT_E ser_result;
    SIM_RESPONSE_DATA_OF_SELECT_T select_rsp;
    SIM_EF_MAPPING_PBR_T pbr;
    uint16 ext1_file_id, aas_file_id, gas_file_id, ccp1_file_id = 0;
    uint16 iap_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 adn_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 pbc_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 grp_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anra_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrb_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrc_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 sne_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 uid_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emaila_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emailb_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint8 i, loop_times = 0;
    BOOLEAN dftele_pb_flag = FALSE;
    BOOLEAN adfusim_pb_flag = FALSE;

    //init first
    SIMDN_SetIsADNinDFtele(cur_sim_task, FALSE);
    SIMDN_SetIsADNinDFteleDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);
    SIMDN_SetIsADNinADFusimDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);

    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
    {
        //first to check the DFtelecom/EFadn,can not check DFtelecom/DFpb/EFpbr,else the software will set SIMDN_SetIsADNinDFteleDFpb,
        //and the file SIM_FILE_EF_ADN will remap to DFtelecom/DFpb/EFadn
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_ADN, &select_rsp);
        if(SIM_SERVICE_OK == ser_result)
        {
            SIMDN_SetIsADNinDFtele(cur_sim_task, TRUE);
        }
        
        //second to check DFtelecom/DFpb/EFpbr
        ext1_file_id = 0;//clear
        aas_file_id = 0;
        gas_file_id = 0;
        ccp1_file_id = 0;
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_TELECOM_PB_PBR, &select_rsp);
        SCI_TRACE_LOW("SIM%d: SIMDN_GetPBRInfo select SIM_FILE_EF_TELECOM_PB_PBR ser_result = %d!",cur_sim_task,ser_result);
        if(SIM_SERVICE_OK == ser_result)
        {
            loop_times = (uint8)(select_rsp.freespace_filesize / select_rsp.dfnumber_recordlen);

            if(loop_times > SIM_USIM_PB_SET_NUM)
            {
                loop_times = SIM_USIM_PB_SET_NUM;
            }

            for(i = 0; i < loop_times; i++)
            {
                SCI_MEMSET(&pbr, 0, sizeof(pbr)); //clear
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_TELECOM_PB_PBR, (i + 1), SIM_RECORD_ABSOLUTE, &pbr);

                if(SIM_SERVICE_OK == ser_result)
                {
                    dftele_pb_flag = TRUE;

                    if(0 == i)
                    {
                        if(0x4F00 != (0xFF00 & pbr.adn_file_id)) //Invalid file id!
                        {
                            SCI_TRACE_LOW("SIM%d: PBR rec1 Invalid ADN file id 0x%x!!!", cur_sim_task, pbr.adn_file_id);

                            //we will continue to use EFadn in DFtele, not in DFtele/EFpb.
                            //SIMDN_SetIsADNinDFteleDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);
                            //return;
                            dftele_pb_flag = FALSE;
                            break;//quit the loop
                        }

                        //according to 31.102 Annex.G TableG.1
                        ext1_file_id = pbr.ext1_file_id;//only one EFext1
                        aas_file_id = pbr.aas_file_id;//only one EFaas
                        gas_file_id = pbr.gas_file_id;//only one EFgas
                        ccp1_file_id = pbr.ccp1_file_id;//only one EFiap
                    }

                    if(0x4F00 != (0xFF00 & pbr.adn_file_id)) //Invalid file id!
                    {
                        SCI_TRACE_LOW("SIM%d: PBR rec%d Invalid ADN file id 0x%x!!!", cur_sim_task, (i + 1), pbr.adn_file_id);
                        break;//end the loop
                    }
                    else
                    {
                        iap_file_id[i] = pbr.iap_file_id;
                        adn_file_id[i] = pbr.adn_file_id;
                        pbc_file_id[i] = pbr.pbc_file_id;
                        grp_file_id[i] = pbr.grp_file_id;
                        anra_file_id[i] = pbr.anra_file_id;
                        anrb_file_id[i] = pbr.anrb_file_id;
                        anrc_file_id[i] = pbr.anrc_file_id;
                        sne_file_id[i] = pbr.sne_file_id;
                        uid_file_id[i] = pbr.uid_file_id;
                        emaila_file_id[i] = pbr.emaila_file_id;
                        emailb_file_id[i] = pbr.emailb_file_id;
                    }
                }
                else
                {
                    if(0 == i)
                    {
                        //we will continue to use EFadn in DFtele, not in DFtele/EFpb.
                        //SIMDN_SetIsADNinDFteleDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);
                        //return;
                        dftele_pb_flag = FALSE;
                    }
                    else
                    {
                        //the 1st record is ok, but the 2nd(3rd) record is not ok.
                        //do nothing
                    }

                    break;//quit the loop
                }
            }

            if(dftele_pb_flag)
            {
                //i is the ADN set num, do not need i+1
                SIMDN_SetIsADNinDFteleDFpb(cur_sim_task, TRUE, i, &adn_file_id[0], ext1_file_id, \
                                           &iap_file_id[0], &pbc_file_id[0], &grp_file_id[0], aas_file_id, gas_file_id, &anra_file_id[0], \
                                           &anrb_file_id[0], &anrc_file_id[0], &sne_file_id[0], ccp1_file_id, &uid_file_id[0], &emaila_file_id[0], &emailb_file_id[0]);
            }
        }

        //third to check the ADFusim/DFpb/EFpbr
        ext1_file_id = 0;//clear
        aas_file_id = 0;
        gas_file_id = 0;
        ccp1_file_id = 0;
        ser_result = SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_USIM_PB_PBR, &select_rsp);

        if(SIM_SERVICE_OK == ser_result)
        {
            loop_times = (uint8)(select_rsp.freespace_filesize / select_rsp.dfnumber_recordlen);

            if(loop_times > SIM_USIM_PB_SET_NUM)
            {
                loop_times = SIM_USIM_PB_SET_NUM;
            }

            for(i = 0; i < loop_times; i++)
            {
                SCI_MEMSET(&pbr, 0, sizeof(pbr)); //clear
                ser_result = SIMSER_ReadEFRecord(cur_sim_task, SIM_FILE_EF_USIM_PB_PBR, (i + 1), SIM_RECORD_ABSOLUTE, &pbr);

                if(SIM_SERVICE_OK == ser_result)
                {
                    adfusim_pb_flag = TRUE;

                    if(0 == i)
                    {
                        if(0x4F00 != (0xFF00 & pbr.adn_file_id)) //Invalid file id!
                        {
                            SCI_TRACE_LOW("SIM%d: ADFUSIM PBR rec1 Invalid ADN file id 0x%x!!!", cur_sim_task, pbr.adn_file_id);
                            adfusim_pb_flag = FALSE;
                            break;//quit the loop
                        }

                        //according to 31.102 Annex.G TableG.1
                        ext1_file_id = pbr.ext1_file_id;//only one EFext1
                        aas_file_id = pbr.aas_file_id;//only one EFaas
                        gas_file_id = pbr.gas_file_id;//only one EFgas
                        ccp1_file_id = pbr.ccp1_file_id;//only one EFiap
                    }

                    if(0x4F00 != (0xFF00 & pbr.adn_file_id)) //Invalid file id!
                    {
                        SCI_TRACE_LOW("SIM%d: ADFUSIM PBR rec%d Invalid ADN file id 0x%x!!!", cur_sim_task, (i + 1), pbr.adn_file_id);
                        break;//end the loop
                    }
                    else
                    {
                        iap_file_id[i] = pbr.iap_file_id;
                        adn_file_id[i] = pbr.adn_file_id;
                        pbc_file_id[i] = pbr.pbc_file_id;
                        grp_file_id[i] = pbr.grp_file_id;
                        anra_file_id[i] = pbr.anra_file_id;
                        anrb_file_id[i] = pbr.anrb_file_id;
                        anrc_file_id[i] = pbr.anrc_file_id;
                        sne_file_id[i] = pbr.sne_file_id;
                        uid_file_id[i] = pbr.uid_file_id;
                        emaila_file_id[i] = pbr.emaila_file_id;
                        emailb_file_id[i] = pbr.emailb_file_id;
                    }
                }
                else
                {
                    if(0 == i)
                    {
                        adfusim_pb_flag = FALSE;
                    }
                    else
                    {
                        //the 1st record is ok, but the 2nd(3rd) record is not ok.
                        //do nothing
                    }

                    break;//quit the loop
                }
            }

            if(adfusim_pb_flag)
            {
                //i is the ADN set num, do not need i+1
                SIMDN_SetIsADNinADFusimDFpb(cur_sim_task, TRUE, i, &adn_file_id[0], ext1_file_id, \
                                          &iap_file_id[0], &pbc_file_id[0], &grp_file_id[0], aas_file_id, gas_file_id, &anra_file_id[0], \
                                          &anrb_file_id[0], &anrc_file_id[0], &sne_file_id[0], ccp1_file_id, &uid_file_id[0], &emaila_file_id[0], &emailb_file_id[0]);
            }
        }

        //End Check the ADFusim/DFpb/EFpbr
        
        //set whether the adn service is available or not according to the check result
        SIMDN_SetADNService(cur_sim_task);
    }
    else
    {
        //if it is an 2G application,set the ADN in DFtelecom/EFadn default
        SIMDN_SetIsADNinDFtele(cur_sim_task, TRUE);
    }

    return;
}

/**********************************************************************
//    Description:
//     To get the ADN's infomation. Because the USIM ADN is not the same to the SIM ADN.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
SIM_SERVICE_RESULT_E SIMDN_GetUSIMADNRecordInfomation(uint32 cur_sim_task, SIM_RECORD_EF_INFO_T *record_info)
{
    SIM_SERVICE_RESULT_E ser_result = SIM_SERVICE_OK;
    SIM_FILE_NAME_E adn_file_name = SIM_FILE_EF_ADN;
    SIM_RECORD_EF_INFO_T temp_rec_info;
    uint8 adn_num = 0;
    uint16 iap_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 adn_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 pbc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 grp_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anra_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 sne_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 uid_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emaila_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emailb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 ext1_file_id, aas_id, gas_id, ccp1_id = 0;
#ifdef SIM_ADN_SERACH_SUPPORT
    SIM_SERVICE_RESULT_E search_ser_result;
    uint8 record_num = 0;
    uint8 record_len = 0;
    uint8 sim_search_buf[255] = {0};
#endif
    SIM_FILE_NAME_E adn_file_arr[4];
    uint8 adn_num_arr[4]={0};
    uint8 adn_count = 0;
    uint8 i = 0;
    adn_file_arr[0]= SIM_FILE_EF_ADN;
   
    if(SIMDN_GetIsADNinDFteleDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        adn_file_arr[adn_count] = SIM_FILE_EF_TELECOM_PB_ADN;
        adn_num_arr[adn_count++] = adn_num; 
    }
    
    if(SIMDN_GetIsADNinDFtele(cur_sim_task, &adn_num))
    {
        adn_file_arr[adn_count] = SIM_FILE_EF_ADN;
        adn_num_arr[adn_count++] = adn_num; 
    }

    if(SIMDN_GetIsADNinADFusimDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        adn_file_arr[adn_count] = SIM_FILE_EF_USIM_PB_ADN;
        adn_num_arr[adn_count++] = adn_num; 
    }

    if(adn_count == 0)
    {
        ser_result = SIM_SERVICE_SELECT_FILE_ERROR;
        SCI_TRACE_LOW("SIM%d:no any phonebook in the card.",cur_sim_task);
        return ser_result;
    }

    for(i = 0; i < adn_count;i++)
    {
        adn_file_name = adn_file_arr[i];
        adn_num = adn_num_arr[i];
        
        ser_result = SIMSER_GetRecordInfo(cur_sim_task, adn_file_name, record_info);

        //the ptr "record_info" is associated with the global variable "g_sim_card_setting[cur_sim_task].adn_info".
        //So we valued it with the 1st EFadn. But we will not valued it with the 2nd EFadn.
        if(SIM_SERVICE_OK == ser_result)
        {

            //SCI_TRACE_LOW:"SIMDN_GetUSIMADNRecordInfomation, 1st ADN has %d rec, max rec%d. %d %d %d %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_3193_112_2_17_22_58_23_4374, (uint8 *)"dddddd", record_info->record_num, record_info->max_record_num, record_info->alpha_id_len, record_info->max_alpha_id_len, record_info->original_alpha_id_len, record_info->other_content_len);

            //Each EFadn should has less than 255 records.
            SCI_ASSERT(record_info->record_num <= 255);/*assert verified:Check_value */

            if(adn_file_name == SIM_FILE_EF_TELECOM_PB_ADN)
            {
                s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][0] = (uint8)record_info->record_num;
            }
            else if(adn_file_name == SIM_FILE_EF_ADN)
            {
                s_sim_telecom_adn_adn_records_num[cur_sim_task][0] = (uint8)record_info->record_num;
            }
            else
            {
                s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][0] = (uint8)record_info->record_num;
            }

#ifdef SIM_ADN_SERACH_SUPPORT

            if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
            {
                if(PNULL == s_sim_adn_search_result[cur_sim_task])
                {
                    s_sim_adn_search_result[cur_sim_task] = (uint8 *)SCI_ALLOC_BASEZ(SIM_MAX_ADN_NUM); //alloc
                }

                record_len = record_info->original_alpha_id_len + record_info->other_content_len;
                //if seek an empty record, the patten can be set to PNULL, because we will encode an empty record for
                //seek later
                search_ser_result = SIMSER_SeekInFile(cur_sim_task, adn_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_BEGINNING_FORWARD,
                                                      record_len, PNULL, &record_num, sim_search_buf);

                if(SIM_SERVICE_OK != search_ser_result)
                {
                    SIMDN_SetADNSearchResult(cur_sim_task, 0, 0, PNULL);

                    SCI_TRACE_LOW("SIM%d:seek adn0 result=%d.",cur_sim_task,search_ser_result);
                    //bug729571, seek the adn need 8s time,if you power off, then the seek operation should be interrupted, and the next adn1 seek 
                    //should not be done,else the card will send 0x60 0x60......, and the SIM task queue will be fulled with SIM_INT_IND signal(type = TB_SIMINT_NEED_WAITTING) 
                    if(SIM_SERVICE_CARD_OUT == search_ser_result)
                    {
                        ser_result = SIM_SERVICE_CARD_OUT;
                        break;//receive power off,return directlly;
                    }
                }
                else
                {
                    SIMDN_SetADNSearchResult(cur_sim_task, record_num, 0, sim_search_buf);
                }
            }

#endif

            if(adn_num > 1) //We support several ADN sets. Now we support 500 records at most.
            {
                if(adn_file_name == SIM_FILE_EF_TELECOM_PB_ADN)
                {
                    adn_file_name = SIM_FILE_EF_TELECOM_PB1_ADN;
                }
                else
                {
                    adn_file_name = SIM_FILE_EF_USIM_PB1_ADN;
                }
                SCI_MEMCPY(&temp_rec_info, record_info, sizeof(SIM_RECORD_EF_INFO_T)); //valued it with 1st EFadn.
                ser_result = SIMSER_GetRecordInfo(cur_sim_task, adn_file_name, &temp_rec_info);

                if(SIM_SERVICE_OK == ser_result)
                {

                    //SCI_TRACE_LOW:"SIMDN_GetUSIMADNRecordInfomation, 2nd ADN has %d rec, max rec%d. %d %d %d %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_3232_112_2_17_22_58_23_4375, (uint8 *)"dddddd", temp_rec_info.record_num, temp_rec_info.max_record_num, temp_rec_info.alpha_id_len, temp_rec_info.max_alpha_id_len, temp_rec_info.original_alpha_id_len, temp_rec_info.other_content_len);

                    //Each EFadn should has less than 255 records.
                    SCI_ASSERT(temp_rec_info.record_num <= 255);/*assert verified:Check_value */
                    if(adn_file_name == SIM_FILE_EF_TELECOM_PB1_ADN)
                    {
                        s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][1] = (uint8)temp_rec_info.record_num;
                    }
                    else
                    {
                        s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][1] = (uint8)temp_rec_info.record_num;
                    }
                    record_info->record_num += temp_rec_info.record_num;

#ifdef SIM_ADN_SERACH_SUPPORT

                    if(USIM_3G_APPLICATION == s_sim_is_usim[cur_sim_task])
                    {
                        record_len = record_info->original_alpha_id_len + record_info->other_content_len;
                        //if seek an empty record, the patten can be set to PNULL, because we will encode an empty record for
                        //seek later
                        SCI_MEMSET(sim_search_buf, 0x0, sizeof(sim_search_buf)); //clear
                        search_ser_result = SIMSER_SeekInFile(cur_sim_task, adn_file_name, SIM_SEEK_TYPE_2, SIM_SEEK_BEGINNING_FORWARD,
                                                              record_len, PNULL, &record_num, sim_search_buf);

                        if(search_ser_result != SIM_SERVICE_OK)
                        {
                            SIMDN_SetADNSearchResult(cur_sim_task, 0, (record_info->record_num - temp_rec_info.record_num), PNULL);

                            SCI_TRACE_LOW("SIM%d:seek adn1 result=%d.",cur_sim_task,search_ser_result);
                            //bug729571, seek the adn need 8s time,if you power off, then the seek operation should be interrupted, and the next adn1 seek 
                            //should not be done,else the card will send 0x60 0x60......, and the SIM task queue will be fulled with SIM_INT_IND signal(type = TB_SIMINT_NEED_WAITTING) 
                            if(SIM_SERVICE_CARD_OUT == search_ser_result)
                            {
                                ser_result = SIM_SERVICE_CARD_OUT;
                                break;//receive power off,return directlly;
                            }
                        }
                        else
                        {
                            SIMDN_SetADNSearchResult(cur_sim_task, record_num, (record_info->record_num - temp_rec_info.record_num), sim_search_buf);
                        }
                    }

#endif

                    //If we should support more record num than 500, we should continue here...
                }

                ser_result = SIM_SERVICE_OK;//we've got the 1st ADN set successfully although the 2nd ADN fail.
            }

            break;//if we got one kind of the phonebook ok,return directlly;
        }
        //this is to face the situation: PBR record contain ADN file id ,while ADN file under DF PB is not exist. jacky
        else
        {
            if(adn_file_name == SIM_FILE_EF_TELECOM_PB_ADN)
            {
                SIMDN_SetIsADNinDFteleDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);
            }
            else if(adn_file_name == SIM_FILE_EF_ADN)
            {
                SIMDN_SetIsADNinDFtele(cur_sim_task, FALSE);
            }
            else
            {
                SIMDN_SetIsADNinADFusimDFpb(cur_sim_task, FALSE, 0, NULL,0,0,NULL,NULL,0,0,NULL,NULL,NULL,NULL,0,NULL,NULL,NULL);
            }
        }
    }
    
    return ser_result;
}

/**********************************************************************
//    Description:
//     To set the MSISDN/FDN/SDN/BDN/EXT2/EXT3 are in DFtelecom or ADFusim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMPB_SetOtherDNLocationDFtel(uint32 cur_sim_task, BOOLEAN is_other_dn_in_dftel)
{
    s_sim_is_other_dn_in_dftel[cur_sim_task] = is_other_dn_in_dftel;
}

/**********************************************************************
//    Description:
//     To get the MSISDN/FDN/SDN/BDN/EXT2/EXT3 are in DFtelecom or ADFusim.
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMPB_GetOtherDNLocationDFtel(uint32 cur_sim_task)
{
    return s_sim_is_other_dn_in_dftel[cur_sim_task];
}

/**********************************************************************
//    Description:
//      Set wether the EFadn in ADFusim/DFpb is exist
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
void SIMDN_SetIsADNinADFusimDFpb(uint32 cur_sim_task, BOOLEAN is_adn_exist, uint8 adn_num, uint16 *adn_id, uint16 ext1_id, uint16 *iap_id, uint16 *pbc_id, uint16 *grp_id, uint16 aas_id, uint16 gas_id, uint16 *anra_id, uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 ccp1_id, uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id)
{
    uint8 i = 0;

    s_sim_is_adfusim_pb_exist[cur_sim_task] = is_adn_exist;

    SCI_TRACE_LOW("SIM%d:SIMDN_SetIsADNinADFusimDFpb is_adn_exist=%d",cur_sim_task,is_adn_exist);
    if(is_adn_exist)
    {
        s_sim_usim_adfusim_pb_adn_num[cur_sim_task] = adn_num;

        if(s_sim_usim_adfusim_pb_adn_num[cur_sim_task] > SIM_USIM_PB_SET_NUM)
        {
            s_sim_usim_adfusim_pb_adn_num[cur_sim_task] = SIM_USIM_PB_SET_NUM;
        }

        for(i = 0; i < s_sim_usim_adfusim_pb_adn_num[cur_sim_task]; i++)
        {
            s_sim_usim_adfusim_pb_iap_id[cur_sim_task][i] = *iap_id;
            iap_id++;

            s_sim_usim_adfusim_pb_adn_id[cur_sim_task][i] = *adn_id;
            adn_id++;

            s_sim_usim_adfusim_pb_pbc_id[cur_sim_task][i] = *pbc_id;
            pbc_id++;

            s_sim_usim_adfusim_pb_grp_id[cur_sim_task][i] = *grp_id;
            grp_id++;

            s_sim_usim_adfusim_pb_anra_id[cur_sim_task][i] = *anra_id;
            anra_id++;

            s_sim_usim_adfusim_pb_anrb_id[cur_sim_task][i] = *anrb_id;
            anrb_id++;

            s_sim_usim_adfusim_pb_anrc_id[cur_sim_task][i] = *anrc_id;
            anrc_id++;

            s_sim_usim_adfusim_pb_sne_id[cur_sim_task][i] = *sne_id;
            sne_id++;

            s_sim_usim_adfusim_pb_uid_id[cur_sim_task][i] = *uid_id;
            uid_id++;

            s_sim_usim_adfusim_pb_emaila_id[cur_sim_task][i] = *emaila_id;
            emaila_id++;

            s_sim_usim_adfusim_pb_emailb_id[cur_sim_task][i] = *emailb_id;
            emailb_id++;
            SCI_TRACE_LOW("SIM%d:DN_SetADNinADFusimDFpb ADN 0x%x, EXT1 0x%x. %d sets", cur_sim_task, s_sim_usim_adfusim_pb_adn_id[cur_sim_task][i], ext1_id, adn_num);
        }

        s_sim_usim_adfusim_pb_ext1_id[cur_sim_task] = ext1_id;

        s_sim_usim_adfusim_pb_aas_id[cur_sim_task] = aas_id;
        s_sim_usim_adfusim_pb_gas_id[cur_sim_task] = gas_id;
        s_sim_usim_adfusim_pb_ccp1_id[cur_sim_task] = ccp1_id;
    }
}

/**********************************************************************
//    Description:
//      Get wether we use the EFadn in ADFusim/DFpb
//    Global resource dependence :
//    Author:wuding.yang
//    Note:
***********************************************************************/
BOOLEAN SIMDN_GetIsADNinADFusimDFpb(
    uint32 cur_sim_task,
    uint8 *adn_num,
    uint16 *adn_id, uint16 *ext1_id, uint16 *iap_id, uint16 *pbc_id,
    uint16 *grp_id, uint16 *aas_id, uint16 *gas_id, uint16 *anra_id,
    uint16 *anrb_id, uint16 *anrc_id, uint16 *sne_id, uint16 *ccp1_id,
    uint16 *uid_id, uint16 *emaila_id, uint16 *emailb_id)
{
    uint8 i = 0;

    if(s_sim_is_adfusim_pb_exist[cur_sim_task])
    {
        *adn_num = s_sim_usim_adfusim_pb_adn_num[cur_sim_task];

        for(i = 0; i < s_sim_usim_adfusim_pb_adn_num[cur_sim_task]; i++)
        {
            *iap_id = s_sim_usim_adfusim_pb_iap_id[cur_sim_task][i];
            iap_id++;

            *adn_id = s_sim_usim_adfusim_pb_adn_id[cur_sim_task][i];
            adn_id++;

            *pbc_id = s_sim_usim_adfusim_pb_pbc_id[cur_sim_task][i];
            pbc_id++;

            *grp_id = s_sim_usim_adfusim_pb_grp_id[cur_sim_task][i];
            grp_id++;

            *anra_id = s_sim_usim_adfusim_pb_anra_id[cur_sim_task][i];
            anra_id++;

            *anrb_id = s_sim_usim_adfusim_pb_anrb_id[cur_sim_task][i];
            anrb_id++;

            *anrc_id = s_sim_usim_adfusim_pb_anrc_id[cur_sim_task][i];
            anrc_id++;

            *sne_id = s_sim_usim_adfusim_pb_sne_id[cur_sim_task][i];
            sne_id++;

            *uid_id = s_sim_usim_adfusim_pb_uid_id[cur_sim_task][i];
            uid_id++;

            *emaila_id = s_sim_usim_adfusim_pb_emaila_id[cur_sim_task][i];
            emaila_id++;

            *emailb_id = s_sim_usim_adfusim_pb_emailb_id[cur_sim_task][i];
            emailb_id++;
        }

        *ext1_id = s_sim_usim_adfusim_pb_ext1_id[cur_sim_task];
        *aas_id = s_sim_usim_adfusim_pb_aas_id[cur_sim_task];
        *gas_id = s_sim_usim_adfusim_pb_gas_id[cur_sim_task];
        *ccp1_id = s_sim_usim_adfusim_pb_ccp1_id[cur_sim_task];
    }

    return s_sim_is_adfusim_pb_exist[cur_sim_task];
}

/**********************************************************************
//    Description:
//      According the adn info to set whether the adn service is available
//      if the DFtelecom/DFpb/EFadn is exist,then the adn service is available
//      if the DFtelecom/EFadn is exist,then the adn service is available
//      if the ADFusim/DFpb/EFadn is exist,then the adn service is available
//    Global resource dependence :
//    Author:yongxia.zhang
//    Note:
***********************************************************************/
void SIMDN_SetADNService(uint32 cur_sim_task)
{
    uint8 adn_num = 0;
    uint16 iap_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 adn_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 pbc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 grp_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anra_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 sne_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 uid_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emaila_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emailb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 ext1_file_id, aas_id, gas_id, ccp1_id = 0;
    SIM_RESPONSE_DATA_OF_SELECT_T select_response;

    if(SIMDN_GetIsADNinDFteleDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].allocated = TRUE;
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].activated = TRUE;

        if(0 != ext1_file_id)
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = TRUE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = TRUE;
        }
        else
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = FALSE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = FALSE;
        }
    }
    else if(SIMDN_GetIsADNinDFtele(cur_sim_task, &adn_num))
    {
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].allocated = TRUE;
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].activated = TRUE;

        if(SIM_SERVICE_OK == SIMSER_SelectFile(cur_sim_task, SIM_FILE_EF_EXT1, &select_response))
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = TRUE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = TRUE;
        }
        else
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = FALSE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = FALSE;
        }
    }
    else if(SIMDN_GetIsADNinADFusimDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].allocated = TRUE;
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].activated = TRUE;

        if(0 != ext1_file_id)
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = TRUE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = TRUE;
        }
        else
        {
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].allocated = FALSE;
            g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_EXT1].activated = FALSE;
        }
    }
    else
    {
        SCI_TRACE_LOW("SIM%d: DFtelecom/adn and DFtelecom/pb and Adfusim/pb are all not exist!",cur_sim_task);
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].allocated = FALSE;
        g_sim_card_setting[cur_sim_task].service_table.service_table[SIMSER_PH2_ADN].activated = FALSE;
    }
}
#endif//SIM_3G_USIM_SUPPORT

/********************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note:
/********************************************************************************/
void SIMDN_Init(
    uint32          cur_sim_task,
    SIM_FILE_NAME_E dn_name,
    uint16           dn_max_num
)
{
    SIMDN_EXT_INFO_T    *dn_ext_info_ptr = PNULL;
    uint32 memory_size = 0;
    uint16 temp_dn_max_num = 0;

    switch(dn_name)
    {
        case SIM_FILE_EF_ADN:
            dn_ext_info_ptr = &(s_adn_ext_info[cur_sim_task]);
            temp_dn_max_num = MIN(dn_max_num, SIM_MAX_ADN_NUM); //double check
            break;

        case SIM_FILE_EF_LND:
            dn_ext_info_ptr = &(s_lnd_ext_info[cur_sim_task]);
            temp_dn_max_num = MIN(dn_max_num, SIM_MAX_LND_NUM); //double check
            break;

        case SIM_FILE_EF_MSISDN:
            dn_ext_info_ptr = &(s_msisdn_ext_info[cur_sim_task]);
            temp_dn_max_num = MIN(dn_max_num, SIM_MAX_MSISDN_NUM); //double check
            break;

        case SIM_FILE_EF_SDN:
            dn_ext_info_ptr = &(s_sdn_ext_info[cur_sim_task]);
            temp_dn_max_num = MIN(dn_max_num, SIM_MAX_SDN_NUM); //double check
            break;

        case SIM_FILE_EF_FDN:
            dn_ext_info_ptr = &(s_fdn_ext_info[cur_sim_task]);
            temp_dn_max_num = MIN(dn_max_num, SIM_MAX_FDN_NUM); //double check
            break;

        default:
            break;
    }

    if((PNULL != dn_ext_info_ptr) && (0 < temp_dn_max_num))
    {
        dn_ext_info_ptr->dn_max_num = temp_dn_max_num;

        if(PNULL != dn_ext_info_ptr->ext_list_ptr)
        {
            SCI_FREE(dn_ext_info_ptr->ext_list_ptr);
        }

        memory_size = temp_dn_max_num * SIM_MAX_EXT_NUM;
        dn_ext_info_ptr->ext_list_ptr = (uint8 *)SCI_ALLOC_BASEZ(memory_size);
        SCI_ASSERT(PNULL != dn_ext_info_ptr->ext_list_ptr); /*assert verified*/

        SCI_MEMSET(dn_ext_info_ptr->ext_list_ptr, 0xFF, memory_size);
    }
}

/********************************************************************************/
//  Description :
//     Clear the global variables because of a different sim card inserted.
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_HotSwap_ClearStaticVariable(uint32 cur_sim_task)
{
#ifdef SIM_3G_USIM_SUPPORT
    uint8 i = 0;
    s_sim_is_telecom_pb_exist[cur_sim_task] = FALSE;
    s_sim_usim_telecom_pb_adn_num[cur_sim_task] = 0;

    for(i = 0; i < SIM_USIM_PB_SET_NUM; i++)
    {
        s_sim_usim_telecom_pb_adn_id[cur_sim_task][i] = 0;
        s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][i] = 0;
    }

    s_sim_usim_telecom_pb_ext1_id[cur_sim_task] = 0x4F4A;
    s_sim_is_other_dn_in_dftel[cur_sim_task] = TRUE;

#ifdef SIM_ADN_SERACH_SUPPORT

    if(PNULL != s_sim_adn_search_result[cur_sim_task])
    {
        SCI_FREE(s_sim_adn_search_result[cur_sim_task]);//alloc
    }

#endif

    s_sim_is_adfusim_pb_exist[cur_sim_task] = FALSE;
    s_sim_usim_adfusim_pb_adn_num[cur_sim_task] = 0;

    for(i = 0; i < SIM_USIM_PB_SET_NUM; i++)
    {
        s_sim_usim_adfusim_pb_adn_id[cur_sim_task][i] = 0;
        s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][i] = 0;
    }
    s_sim_usim_adfusim_pb_ext1_id[cur_sim_task] = 0x4F4A;

    s_sim_is_telecom_adn_exist[cur_sim_task] = FALSE;
    s_sim_telecom_adn_adn_records_num[cur_sim_task][0] = 0;
#endif

    s_sim_dn_ext1_num[cur_sim_task] = 0;
    s_sim_dn_ext2_num[cur_sim_task] = 0;
    s_sim_dn_ext3_num[cur_sim_task] = 0;

#ifndef SIM_CODE_COMPACT
    prev_purge_fail[cur_sim_task][0] = FALSE;
    prev_purge_fail[cur_sim_task][1] = FALSE;
    prev_purge_fail[cur_sim_task][2] = FALSE;
    prev_purge_fail[cur_sim_task][3] = FALSE;
#endif
}

#ifdef SIM_3G_USIM_SUPPORT
#ifdef SIM_ADN_SERACH_SUPPORT
/**********************************************************************
//    Description:
//      Set EFadn search results, if the record equal to "FF", we do not need to read it
//    Global resource dependence :
//    Author:Jacky.ying
//    Note:
***********************************************************************/
STATIC void SIMDN_SetADNSearchResult(
    uint32 cur_sim_task,
    uint16 empty_rec_num,
    uint16 adn_set_offset,
    uint8 *empty_rec)
{
    uint16 i = 0;
    uint8 *ptemp = s_sim_adn_search_result[cur_sim_task];

    if(NULL != ptemp)
    {
        ptemp += adn_set_offset;

        //SCI_TRACE_LOW:"SIMDN%d: search the empty record number is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, SIM_DIALING_NUMBER_3423_112_2_17_22_58_24_4376, (uint8 *)"dd", cur_sim_task, empty_rec_num);

        for(i = 0; i < empty_rec_num; i++)
        {
            *(ptemp + (*empty_rec - 1)) = 0xFF;
            empty_rec++;
        }
    }
}
#endif
#endif

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_ClearEXT1Info(uint32 cur_sim_task)
{
    SIMDN_EXT_INFO_T *dn_ext_info_ptr = NULL;

    dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, SIM_DN_T_LND);

    if(NULL != dn_ext_info_ptr->ext_list_ptr)
    {
        SCI_MEMSET(dn_ext_info_ptr->ext_list_ptr, 0xFF, (dn_ext_info_ptr->dn_max_num * SIM_MAX_EXT_NUM));
    }

    dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, SIM_DN_T_MSISDN);

    if(NULL != dn_ext_info_ptr->ext_list_ptr)
    {
        SCI_MEMSET(dn_ext_info_ptr->ext_list_ptr, 0xFF, (dn_ext_info_ptr->dn_max_num * SIM_MAX_EXT_NUM));
    }

    dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, SIM_DN_T_ADN);

    if(NULL != dn_ext_info_ptr->ext_list_ptr)
    {
        SCI_MEMSET(dn_ext_info_ptr->ext_list_ptr, 0xFF, (dn_ext_info_ptr->dn_max_num * SIM_MAX_EXT_NUM));
    }
}

/********************************************************************************/
//  Description :
//
//  Global resource dependence : none
//  Author: wuding.yang
//  Note:
/********************************************************************************/
void SIMDN_ClearEXT2Info(uint32 cur_sim_task)
{
    SIMDN_EXT_INFO_T *dn_ext_info_ptr = GetExtInfoPtr(cur_sim_task, SIM_DN_T_FDN);

    if(NULL != dn_ext_info_ptr->ext_list_ptr)
    {
        SCI_MEMSET(dn_ext_info_ptr->ext_list_ptr, 0xFF, (dn_ext_info_ptr->dn_max_num * SIM_MAX_EXT_NUM));
    }
}

#ifdef SIM_FILL_PHONEBOOK_FULL
/********************************************************************************/
//  Description :
//      set flag to fill phone book full when check card present
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/********************************************************************************/
void SIM_SetWritePhonebookFull(MN_DUAL_SYS_E  dual_sys)
{
    SCI_TRACE_LOW("SIM%d:SIM_SetWritePhonebookFull",dual_sys);
    s_sim_write_pb_full_begin[dual_sys] = TRUE; 
}
/********************************************************************************/
//  Description :
//      Get flag whether to fill phone book full when check card present
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/********************************************************************************/
BOOLEAN SIM_GetWritePhonebookFull(MN_DUAL_SYS_E  dual_sys)
{
    SCI_TRACE_LOW("SIM%d:SIM_GetWritePhonebookFull,flag=%d",dual_sys,s_sim_write_pb_full_begin[dual_sys]);
    return s_sim_write_pb_full_begin[dual_sys];
}

/********************************************************************************/
//  Description :
//      fill phone book full when check card present
//  Global resource dependence : none
//  Author: yongxia.zhang
//  Note:
/********************************************************************************/
void SIM_WritePhonebookFull(MN_DUAL_SYS_E  dual_sys)
{
    uint32 i = 0;
    uint8 adn_num = 0;
    uint32 cur_sim_task = dual_sys; 
    uint32 max_adn_record_num = 0;
    uint16 iap_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 adn_file_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 pbc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 grp_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anra_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 anrc_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 sne_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 uid_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emaila_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 emailb_id[SIM_USIM_PB_SET_NUM] = {0};
    uint16 ext1_file_id, aas_id, gas_id, ccp1_id = 0;
    SIM_SIG_UPDATE_DN_REQ_T update_req;

    s_sim_write_pb_full[cur_sim_task] = TRUE;
   
    if(SIMDN_GetIsADNinDFteleDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        for(i=0;i<s_sim_usim_telecom_pb_adn_num[cur_sim_task];i++)
        {
            SCI_TRACE_LOW("SIM%d:SIM_WritePhonebookFull the %d telecom/pb/adn_num=%d.",cur_sim_task,i,s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][i]);
            max_adn_record_num += s_sim_usim_telecom_pb_adn_records_num[cur_sim_task][i];
        }
    }
    else if(SIMDN_GetIsADNinDFtele(cur_sim_task, &adn_num))
    {
        max_adn_record_num = s_sim_telecom_adn_adn_records_num[cur_sim_task][0];
    }
    else if(SIMDN_GetIsADNinADFusimDFpb(cur_sim_task, &adn_num,
                                     &adn_file_id[0], &ext1_file_id, &iap_id[0], &pbc_id[0], &grp_id[0], &aas_id,
                                     &gas_id, &anra_id[0], &anrb_id[0], &anrc_id[0], &sne_id[0], &ccp1_id, &uid_id[0], &emaila_id[0], &emailb_id[0]))
    {
        for(i=0;i<s_sim_usim_adfusim_pb_adn_num[cur_sim_task];i++)
        {
            SCI_TRACE_LOW("SIM%d:SIM_WritePhonebookFull the %d adfusim/pb/adn_num=%d.",cur_sim_task,i,s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][i]);
            max_adn_record_num += s_sim_usim_adfusim_pb_adn_records_num[cur_sim_task][i];
        }
    }

    SCI_TRACE_LOW("SIM%d:SIM_WritePhonebookFull max_adn_record_num=%d.",cur_sim_task,max_adn_record_num);
    
    for(i=0;i<max_adn_record_num;i++)
    {
        SCI_MEMSET(&update_req,0,sizeof(update_req));
        
        update_req.dn_type = SIM_DN_T_ADN;
        update_req.dn_id = i + 1;
        if(i<10)
        {
            update_req.dn.alpha_id_len = 1;
            update_req.dn.alpha_id[0] = i + 0x30;
            update_req.dn.number_len = 1;
            update_req.dn.numbers[0] = i|0xF0;
            if(i == 1)
            {
                uint8 j=0;

                update_req.dn.number_len = SIM_MAX_DN_NUMBER_LEN;

                for(j=0;j<SIM_MAX_DN_NUMBER_LEN;j++)
                update_req.dn.numbers[j] = 0x88;
            }
        }
        else if(i<100)
        {
            update_req.dn.alpha_id_len = 2;
            update_req.dn.alpha_id[0] = i/10 + 0x30; 
            update_req.dn.alpha_id[1] = i%10 + 0x30;
            update_req.dn.number_len = 1;
            update_req.dn.numbers[0] = i/10 + ((i%10)<<4);
        }
        else
        {
            uint8 digit1=0,digit2 = 0,digit3=0;
            update_req.dn.alpha_id_len = 3;
            update_req.dn.alpha_id[0] = i/100 + 0x30; 
            update_req.dn.alpha_id[1] = (i%100)/10 + 0x30; 
            update_req.dn.alpha_id[2] = i%10 + 0x30; 
            update_req.dn.number_len = 2;
            digit1= i/100;
            digit2 = (i%100)/10;
            digit3 = i%10;
            update_req.dn.numbers[0] = digit1 + (digit2<<4);
            update_req.dn.numbers[1] = digit3|0xF0;;
        }
        update_req.dn.is_ccp_exist = FALSE;
        update_req.dn.is_ton_npi_exist = TRUE;
        update_req.dn.ton = 0;
        update_req.dn.npi = 1;
        SCI_TRACE_LOW("SIM%d:update dn_id=%d,num =0x%2x,0x%2x",cur_sim_task,i,update_req.dn.numbers[0],update_req.dn.numbers[1]);
        SIMDN_UpdateDN(cur_sim_task,&update_req);
    }

    s_sim_write_pb_full[cur_sim_task] = FALSE; 
    s_sim_write_pb_full_begin[dual_sys] = FALSE;
}
#endif

#ifdef   __cplusplus
}
#endif

