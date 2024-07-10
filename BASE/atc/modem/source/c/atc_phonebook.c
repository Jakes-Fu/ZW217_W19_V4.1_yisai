/******************************************************************************
 ** File Name:      atc_phonebook.c                                           *
 ** Author:         Xueli.lv                                                  *
 ** DATE:           04/12/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains the functions that handle the AT       *
 **                 command for phonebook                                     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 04/12/2002     Xueli.lv         Create.                                   *
 ******************************************************************************/
#include "atc.h"
#include "atc_common.h"
#include "atc_phonebook.h"

/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/

/*lint -save -e752 */
extern MN_DUAL_SYS_E        g_current_card_id;//lint !e752
#define ATC_ADN_NUM_LEN 40
extern RSP_RESULT_STR          g_rsp_str;
extern const ATC_INFO_T        g_atc_info_table[];

static const char *atc_storage_str[PHONEBOOK_2G_MAX_STORAGE_NUM] =
{
    "SM", "FD", "ME", "LD", "MC", "RC", "MSISDN",
    "EMERGENCY", "BDN"
}; //lint !e785

static PHONEBOOK_STORAGE_E          s_default_storage[MN_SYS_NUMBER] = {PHONEBOOK_SIM_STORAGE};

/*record the last read index of phonebook*/
static uint16         s_last_read_index[MN_SYS_NUMBER][PHONEBOOK_2G_MAX_STORAGE_NUM] = {0};

/*record the last write index of phonebook*/
static uint16         s_last_written_index[MN_SYS_NUMBER][PHONEBOOK_2G_MAX_STORAGE_NUM] = {0};

/*****************************************************************************/
//  Description : This function starts to read phonebook info(ADN)
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL void HandlePhonebookRead(
    ATC_CONFIG_T           *atc_config_ptr,
    uint16                 cmd_id,           //command identity
    PHONEBOOK_ENTRY_T      *entry_ptr,       //[in] phonebook entry
    BOOLEAN                 is_usim         //[in] report usim/sim format info
    //APP_MN_READ_ADDITIONAL_ENTRY_T    *entry_additional_ptr
    //[in] used for usim pb entry,is_usim must be TRUE.
);

/*****************************************************************************/
//  Description : This function starts to read phonebook info(ADN)
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ReadPhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 additional_flag,  //[in]has adr,email and etc or not
    PHONEBOOK_ENTRY_T      *entry_ptr         //[in] empty entry except entry_id assigned
    //[out] adn info
);

/*****************************************************************************/
//  Description : This function is used to delete phonebook info
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS DeletePhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 is_usim,          //[in] usim or sim
    uint16                  entry_id          //[in]entry id to be delete
);

#ifdef _ATC_WAIT_L4
/*****************************************************************************/
//  Description : This function is used to update phonebook info
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS UpdatePhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 is_usim,          //[in] usim or sim
    PHONEBOOK_ENTRY_T      *entry_ptr,         //[in] entry to be update
    ATC_PB_ADDITIONAL_ENTRY_T  *entry_addition_ptr  //[in]available when is_usim = TRUE
);
#else
/*****************************************************************************/
//  Description : This function is used to update phonebook info
//  Global resource dependence:
//  Author:  wuding.yang
//  Note: We dont support USIM PB ANR/EMAIL, etc.
/*****************************************************************************/
LOCAL ATC_STATUS GSM_UpdatePhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 is_usim,          //[in] usim or sim
    PHONEBOOK_ENTRY_T      *entry_ptr         //[in] entry to be update
);
#endif

/*****************************************************************************/
//  Description : This function is used to get the telephone number(BCD format)
//              according to AT command parameters
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTelephoneNumByATPara(
    BOOLEAN     tel_flag,   //[in]telephone is present flag
    ATC_CUR_PARAM_STRING_T   *tel_num_ptr,   //[in]telephone info
    BOOLEAN     *is_plus_in_tel_num_ptr,     //[out]phone number with a prefix '+' or not
    uint8        *bcd_tel_num_ptr,    //[out]bcd phone number
    uint8        *bcd_tel_len_ptr     //[out]bcd phone number length
);
/*****************************************************************************/
//  Description : This function is used to get the telephone type
//              according to AT command parameters
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTelephoneTypeByATPara(
    BOOLEAN     tel_type_flag,    //[in]telephone type is present flag
    uint8       tel_type_value,   //[in]telephone type value input by user
    BOOLEAN     is_plus_in_tel_num,  //[in]the telephone num has '+' or not
    uint8       *tel_type_final     //[out]the telephone type according to
    //tel number and type input by user
);

/*****************************************************************************/
//  Description : This function initializes the static variables of phonebook
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_InitPhonebookStaticVar(void)
{
    int32 i,j;

    for (i = 0; i < MN_SYS_NUMBER; i++)
    {
        s_default_storage[i] = PHONEBOOK_SIM_STORAGE;
    
        for(j = 0; j < PHONEBOOK_2G_MAX_STORAGE_NUM; j++)
        {
            s_last_read_index[i][j]    = 0;
            s_last_written_index[i][j] = 0;
        }
    }

}

/*****************************************************************************/
//  Description : This function handles the AT+CPBS command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBS)
{
#ifndef _ULTRA_LOW_CODE_
    int32                  i;
    uint8                  *storage_ptr = PNULL;
    uint32                 return_value = S_ATC_SUCCESS;
    PHONEBOOK_INFO_T       phonebook_info = {0};
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
            //set command AT+CPBS="<storage>"
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBS, param1 is not string!");
                return_value = ERR_INVALID_INDEX;
                break;
            }

            SCI_ASSERT(PNULL != PARA1.str_ptr->str_ptr);/*assert verified: check valid value*/
            storage_ptr = PARA1.str_ptr->str_ptr;

            if(AT_CPBS_MAXLENGTH < PARA1.str_ptr->str_len)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }
            else
            {
                storage_ptr[PARA1.str_ptr->str_len] = '\0';
            }

            // Compare the inputed storage string
            for(i = 0; i < PHONEBOOK_2G_MAX_STORAGE_NUM; i++)
            {
                if(!strcmp((char *)storage_ptr, atc_storage_str[i]))
                {
                    s_default_storage[dual_sys] = i;
                    break;
                }
            }

            if(PHONEBOOK_2G_MAX_STORAGE_NUM == i)
            {
                return_value = S_ATC_FAIL;
            }
            else
            {
                return_value = S_ATC_SUCCESS;
            }

            break;

            // read command AT+CPBS? "SM", "FD", "ME", "LD", "MC", "RC"
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (\"SM\",\"FD\",\"ME\",\"LD\",\"MC\",\"RC\",\"MSISDN\",\"EMERGENCY\")",
                    g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            return_value = S_ATC_SUCCESS;
            break;

            // read command AT+CPBS=?
        case ATC_CMD_TYPE_READ:

            if(s_default_storage[dual_sys] == PHONEBOOK_EMERGENCY_STORAGE)
            {
                sprintf((char *)g_rsp_str, "%s: \"%s\"", g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr,
                        atc_storage_str[s_default_storage[dual_sys]]);
            }
            else
            {
                if(atc_config_ptr->call_read_flag[dual_sys] == TRUE)
                {
                    sprintf((char *)g_rsp_str, "%s: \"%s\",%d,%d", g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr,
                            atc_storage_str[s_default_storage[dual_sys]], phonebook_info.used_record_num,
                            (phonebook_info.max_record_num/*-phonebook_info.used_record_num*/));
                }
                else
                {
                    sprintf((char *)g_rsp_str, "%s: \"%s\",0,0", g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr,
                            atc_storage_str[s_default_storage[dual_sys]]);
                }
            }

            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPBR command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBR)
{
#ifndef _ULTRA_LOW_CODE_
    int32                   index1;
    int32                   index2;
    PHONEBOOK_INFO_T        phonebook_info;
    PHONEBOOK_ENTRY_T       entry;
    BOOLEAN                 entry_exist_flag = FALSE;
    uint32                  return_value = S_ATC_SUCCESS;
    ERR_MNDATAMAG_CODE_E    read_result;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(PHONEBOOK_SIM_STORAGE    != s_default_storage[dual_sys] &&
         PHONEBOOK_NV_STORAGE   != s_default_storage[dual_sys] &&
         PHONEBOOK_FDN_STORAGE  != s_default_storage[dual_sys] &&
         PHONEBOOK_MSISDN_STORAGE  != s_default_storage[dual_sys]&&
         PHONEBOOK_LND_STORAGE != s_default_storage[dual_sys])

    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(atc_config_ptr->call_read_flag[dual_sys] != TRUE)
    {
        ATC_TRACE_LOW("ATC: SIM/USIM is not ready.");
        return ERR_SIM_BUSY;
    }

    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    if((!phonebook_info.is_phonebook_exist) ||
            0 == phonebook_info.max_record_num)
    {
        ATC_TRACE_LOW("ATC: +CPBR, no phonebook entity.");
        return ERR_SIM_WRONG;
    }

    switch(ATC_GET_CMD_TYPE)
    {
            //set command AT+CPBR=index1,index2
        case ATC_CMD_TYPE_SET:

            // check the index 1
            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            index1 = (uint16)PARA1.num;

            if(index1 > phonebook_info.max_record_num || index1 < 1)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBR: index:%d is out of range(1-%d)", index1, phonebook_info.max_record_num);
                return_value = ERR_INVALID_INDEX;
                break;
            }

            // check the index2
            index2 = PARAM2_FLAG ? (uint16)PARA2.num : index1;

            if((index1 > index2) || (index2 > phonebook_info.max_record_num))
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBR: index:%d is out of range(1-%d)", index2, phonebook_info.max_record_num);
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(0 == phonebook_info.used_record_num)
            {
                ATC_TRACE_LOW("ATC: no phonebook record.");
                return_value = ERR_NOT_FOUND;
                break;
            }

            //Find and print the result
            for(; index1 <= index2; index1++)
            {
                SCI_MEMSET(&entry, 0, sizeof(PHONEBOOK_ENTRY_T));

                entry.entry_id = (uint16)index1;

                read_result = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);

                if(ERR_MNDATAMAG_NO_ERR == read_result)
                {
                    /*read entry successfully, report +CPBR info*/
                #ifdef _ATC_WAIT_
                    HandlePhonebookRead(atc_config_ptr, AT_CMD_CPBR, &entry, FALSE, NULL);
                #else
                    HandlePhonebookRead(atc_config_ptr, AT_CMD_CPBR, &entry, FALSE);
                #endif

                    entry_exist_flag = TRUE;
                    //Record the read index of phonebook
                    s_last_read_index[dual_sys][s_default_storage[dual_sys]] = index1;
                }
            }

            if(entry_exist_flag)
            {
                return_value = S_ATC_SUCCESS;
            }
            else
            {
                /*no entry founded, report CME ERROR:NOT FOUND*/
                return_value = ERR_NOT_FOUND;
            }

            break;

            //Test command AT+CPBR=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (1-%d),%d,%d",
                    g_atc_info_table[AT_CMD_CPBR].cmd_name->str_ptr,
                    phonebook_info.max_record_num, phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len,
                    phonebook_info.max_alpha_len);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPBF command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBF)
{
#ifndef _ULTRA_LOW_CODE_
    MN_STRING_T           alpha;
    PHONEBOOK_ENTRY_T     entry;
    PHONEBOOK_INFO_T       phonebook_info; // holds the phonebook information
    BOOLEAN          present_flag;
    uint8          tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2] = {0};
    char           tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint32        return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint16 start_index = 0;
    BOOLEAN find_flag = FALSE;

    uint8     *number_ptr = NULL;
    uint8     i  = 0;
    BOOLEAN   is_internation_num_flag = FALSE;
	
    //Get the information of phonebook
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
            // the set command AT+CPBF=<findtext>
        case ATC_CMD_TYPE_SET:

            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBF, param1 is not string!");
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //check the length of input alpha.
            if(PARA1.str_ptr->str_len > phonebook_info.max_alpha_len)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            // get the alpha of entry.
            SCI_MEMSET(alpha.strbuf, '\0', MN_MAX_STRING_LEN);
            SCI_MEMSET(tel_alpha, '\0', (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2 + 1));

            PARA1.str_ptr->str_ptr[PARA1.str_ptr->str_len] = '\0';

            //string input is UCS2
            if(ConvertAscUcs2IntoBin((char *)(PARA1.str_ptr->str_ptr), alpha.strbuf, (PARA1.str_ptr->str_len) / 2) &&
                    (ATC_UCS2_80_CODE == *alpha.strbuf ||
                     ATC_UCS2_81_CODE == *alpha.strbuf ||
                     ATC_UCS2_82_CODE == *alpha.strbuf))
            {
                alpha.strlen = (PARA1.str_ptr->str_len) / 2;
            }
            else
            {
                SCI_MEMCPY(alpha.strbuf, PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len);
                alpha.strlen = PARA1.str_ptr->str_len;
                alpha.strbuf[PARA1.str_ptr->str_len] = '\0';
            }

            //find by name.
            for (start_index=1;start_index<=phonebook_info.max_record_num;start_index++)//start_index is absolute SIM index(start from 1)
            {
                SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));//clear
                SCI_MEMSET(tel_str, 0, (PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2));//clear
                SCI_MEMSET(tel_alpha, 0, (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1));//clear
                present_flag = PHONEBOOK_FindPhonebookEntryByAlphaEx_WithStartID(dual_sys, s_default_storage[dual_sys], start_index, &alpha, &entry);
                if(MN_RETURN_SUCCESS == present_flag)
                {
                    uint8   toa;
                    toa = (uint8)((entry.ton << 4) + entry.npi + 128);
                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
					
                    /*If tel_type is international, read tel_str add '+'*/	
                    if(entry.ton == MN_NUM_TYPE_INTERNATIONAL || tel_str[0] == '+')
                    {
                        number_ptr = tel_str;
                	    number_ptr[entry.number_len*2+2] = '\0';
                        for(i = entry.number_len*2+1; i>0 ; i--)
                        {
                            number_ptr[i] = number_ptr[i-1];
                        }
                        number_ptr[0] = '+';
                        is_internation_num_flag = TRUE;
                    }
					
                    SCI_MEMSET(tel_alpha, '\0', (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2 + 1));
                    if(ATC_UCS2_80_CODE == entry.alpha[0] || 
			           ATC_UCS2_81_CODE == entry.alpha[0] || 
			           ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8 *)entry.alpha, entry.alpha_len, (uint8 *)tel_alpha);
                        tel_alpha[entry.alpha_len * 2] = '\0';
                    }
                    else
                    {
                        SCI_MEMCPY(tel_alpha, (char *)entry.alpha, entry.alpha_len);
                        tel_alpha[entry.alpha_len] = '\0';
                    }

                   if(is_internation_num_flag)
                   {
                        sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                                g_atc_info_table[AT_CMD_CPBF].cmd_name->str_ptr,
                                entry.entry_id, number_ptr, toa, tel_alpha);                   
                   }
                   else
                   {
                        sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                                g_atc_info_table[AT_CMD_CPBF].cmd_name->str_ptr,
                                entry.entry_id, tel_str, toa, tel_alpha);                   
                   }

                    ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                    s_last_read_index[dual_sys][s_default_storage[dual_sys]] = entry.entry_id;

                    //If we find out one record, the next time we will start from this record.
                    //entry.entry_id is a SIM absolute index, start from 1 to max.
                    //start_index is an array index, start from 0 to max.
                    start_index = entry.entry_id;
                    find_flag = TRUE;
                }
                else
                {
                    if (find_flag)
                    {
                        //If we have found one or several records before, we donot send any AT rsp any more here.
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC:ConvertAscUcs2IntoBin");
                        return_value = ERR_OPERATION_NOT_ALLOWED;
                    }
                    break;//Not find, quit the loop
                }
            }

            break;

            // the test command AT+CPBF=?
        case ATC_CMD_TYPE_TEST:
            // return the <nlength> and <tlength>
            sprintf((char *)g_rsp_str, "%s: %d,%d",
                    g_atc_info_table[AT_CMD_CPBF].cmd_name->str_ptr,
                    phonebook_info.max_record_num, phonebook_info.max_alpha_len);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CPBW command
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBW)
{
#ifndef _ULTRA_LOW_CODE_
    uint16               index = 0;
    uint8                 number_type;
    uint32               return_value = S_ATC_SUCCESS;
    BOOLEAN          is_num_internation_type = FALSE;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    PHONEBOOK_ENTRY_T     entry;
    char                  tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
#ifdef HUAWEI_6500_MODEM
    char                  new_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1];
#endif
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    if(PHONEBOOK_SIM_STORAGE != s_default_storage[dual_sys] &&
          PHONEBOOK_NV_STORAGE  != s_default_storage[dual_sys] &&
          PHONEBOOK_FDN_STORAGE != s_default_storage[dual_sys] &&
          PHONEBOOK_MSISDN_STORAGE != s_default_storage[dual_sys] &&
          PHONEBOOK_EMERGENCY_STORAGE != s_default_storage[dual_sys] &&
          PHONEBOOK_LND_STORAGE != s_default_storage[dual_sys])
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(atc_config_ptr->call_read_flag[dual_sys] != TRUE)
    {
        ATC_TRACE_LOW("ATC: SIM/USIM is not ready.");
        return ERR_SIM_BUSY;
    }

    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    if((!phonebook_info.is_phonebook_exist) ||
            0 == phonebook_info.max_record_num)
    {
        ATC_TRACE_LOW("ATC: +CPBW, no phonebook entity.");
        return ERR_SIM_WRONG;
    }

    switch(ATC_GET_CMD_TYPE)
    {
            // Set command AT+CPBW=[<index>][, <number>[,<type>[,<text>]]]
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                index = (uint16)PARA1.num;

                if(index > phonebook_info.max_record_num || index < 1)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPBW: index:%d is out of range(1-%d)", index, phonebook_info.max_record_num);
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                /*DELETE PROCEDURE*/
                if(!PARAM2_FLAG && !PARAM3_FLAG && !PARAM4_FLAG)
                {
                    // If only one parameter,delete the item of given index
                    return_value = DeletePhonebook(dual_sys, atc_config_ptr, s_default_storage[dual_sys], FALSE, index);
                    break;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBW: no index");
                index = PHONEBOOK_DEFAULT_ENTRY_ID;

                if(PARAM2_FLAG)
                {
                    if(phonebook_info.max_record_num == phonebook_info.used_record_num)
                    {
                        ATC_TRACE_LOW("ATC: ATC_ProcessCPBW: memory full.");
                        return_value = ERR_MEMORY_FULL;
                        break;
                    }                  
                }
            }

            /*WRITE PROCEDURE*/
            if(!PARAM1_FLAG && !PARAM2_FLAG)
            {
                // the Operation not permitted
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //check the length of input
            if(PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPBW, <number> is not string!");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                if(0 == PARA2.str_ptr->str_len)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                /*verify and convert(string to bcd) <number>*/
                if(!GetTelephoneNumByATPara(PARAM2_FLAG, PARA2.str_ptr,
                                            &is_num_internation_type,
                                            entry.number, &entry.number_len))
                {
                    // Operation not supported
                    return_value = ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
                    break;
                }
            }

            /*verify <type>*/
            if(!GetTelephoneTypeByATPara(PARAM3_FLAG, (uint8)PARA3.num,
                                         is_num_internation_type, &number_type))
            {
                // Operation not supported
                return_value = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

            // the IE of type of number
            entry.ton = (MN_NUMBER_TYPE_E)((number_type & 0x70) >> 4);  // get the bit 7,6,5 //@Shijun.cui 2005-09-14 CR31196
            entry.npi = (MN_NUMBER_PLAN_E)(number_type & 0x0f);  // get the bit 1,2,3,4

            // check the alpha
            if(PARAM4_FLAG)
            {
                if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCPBW, param4 is not string!");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                //check the length of input
                if(0 == PARA4.str_ptr->str_len ||
                        PARA4.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                SCI_MEMCPY(tel_alpha, (char *)PARA4.str_ptr->str_ptr, PARA4.str_ptr->str_len);
                //string input is UCS2
                ATC_TRACE_LOW("ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x", tel_alpha[0],
                              tel_alpha[1], tel_alpha[2], (PARA4.str_ptr->str_len) / 2);

#ifdef HUAWEI_6500_MODEM
                if(atc_config_ptr->cscs[dual_sys] == ATC_CHSET_UCS2)
                {
                    if(ConvertAscUcs2IntoBin(tel_alpha, entry.alpha, (PARA4.str_ptr->str_len) / 2))
                    {
                        if(PARA4.str_ptr->str_len / 2 < 14)
                        {
                            ATC_TRACE_LOW("ATC:ATC_ProcessCPBW,ATC_UCS2_CODE == *entry.alpha");
                            new_alpha[0] = '8';
                            new_alpha[1] = '0';
                            strcpy(new_alpha + 2, tel_alpha);
							
                            ConvertAscUcs2IntoBin(new_alpha, entry.alpha, (PARA4.str_ptr->str_len) / 2 +1);
                            entry.alpha_len  = PARA4.str_ptr->str_len / 2 +1;
                        }
                        else
                        {
                            return ERR_TEXT_STRING_TOO_LONG;
                        }
                    }
                    else
                    {
                        return ERR_INVALID_CHARACTERS_IN_TEXT_STRING;
                    }
                }
#else
               if(ConvertAscUcs2IntoBin(tel_alpha, entry.alpha, (PARA4.str_ptr->str_len) / 2) &&
			   ((PARA4.str_ptr->str_len / 2) > 2)&&
                        (ATC_UCS2_80_CODE == *entry.alpha ||
                         ATC_UCS2_81_CODE == *entry.alpha ||
                         ATC_UCS2_82_CODE == *entry.alpha))
                {
                    ATC_TRACE_LOW("ATC:ATC_ProcessCPBW,ATC_UCS2_CODE == *entry.alpha");
                    entry.alpha_len  = PARA4.str_ptr->str_len / 2;
                }
#endif
                //string input is ASCII.
                else
                {
                #ifdef HUAWEI_6500_MODEM
                    if(PARA4.str_ptr->str_len > phonebook_info.max_alpha_len)
		        #else
                    if(PARA4.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                #endif
                    {
                        return_value = ERR_INVALID_INDEX;
                        break;
                    }

                    PARA4.str_ptr->str_ptr[PARA4.str_ptr->str_len] = '\0';
                    entry.alpha_len  = PARA4.str_ptr->str_len;

                    if(PARA4.str_ptr->str_len < PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                    {
                        SCI_MEMCPY(((char *)entry.alpha), ((char *)PARA4.str_ptr->str_ptr), (entry.alpha_len + 1));
                    }
                    else
                    {
                        SCI_MEMCPY(entry.alpha, PARA4.str_ptr->str_ptr, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
                    }

                    ATC_TRACE_LOW("ATC:cpbw,PARA4.str_ptr=%x,%x,%x", *PARA4.str_ptr->str_ptr,
                                  *(PARA4.str_ptr->str_ptr + 1), *(PARA4.str_ptr->str_ptr + 2));

                    ATC_TRACE_LOW("ATC:cpbw,entry.alpha=%x,%x,%x", *entry.alpha, *(entry.alpha + 1), *(entry.alpha + 2));
                }
            }

            // call the update functions
            entry.entry_id = index /*- 1 */;
            ATC_TRACE_LOW("alhpa: %s, ton: %d, entry_id: %d, number_len: %d", entry.alpha, entry.ton, entry.entry_id, entry.number_len);

            return_value = GSM_UpdatePhonebook(dual_sys, atc_config_ptr, s_default_storage[dual_sys], FALSE, &entry);

            break;

            // test command AT+CPBW=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (1-%d),%d,(%d,%d,%d,%d),%d",
                    g_atc_info_table[AT_CMD_CPBW].cmd_name->str_ptr,
                    phonebook_info.max_record_num, ATC_ADN_NUM_LEN,
                    UNKNOWN_PLAN_UNKNOWN_TYPE, ISDN_TELE_PLAN_UNKNOWN_TYPE,
                    ISDN_TELE_PLAN_INTERNATIONAL_TYPE, ISDN_TELE_PLAN_NATIONAL_TYPE,
                    phonebook_info.max_alpha_len);
            // call the transfer function
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handles the APP_MN_ADN_UPDATE_CNF signal in
//  deleting only one item.
//  Global resource dependence :
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessDelItemCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_ADN_UPDATE_CNF_T    *sig_ptr,
    BOOLEAN                 expt_event_recv_flag
)
{
#ifndef _ULTRA_LOW_CODE_

    if(expt_event_recv_flag)
    {
        ATC_TRACE_LOW("ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by ATC.", sig_ptr->entry_id);

        //delete successfully.
        if(sig_ptr->is_ok)
        {
            //ATC_TRACE_LOW("ATC:ATC_ProcessDelItemCnf,true");
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
        }
        //fail to delete or update the item of phonebook
        else
        {
            //ATC_TRACE_LOW("ATC:ATC_ProcessDelItemCnf,FAIL");
            ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_UNKNOWN);
        }
    }
    else
    {
        ATC_TRACE_LOW("ATC:ATC_ProcessDelItemCnf, delete entry(id=%d) by MMI.", sig_ptr->entry_id);
    }

#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_LND_UPDATE_CNF signal in
//  deleting only one item.
//  Global resource dependence :
//  Author:       
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessDelLNDAllCnf(
    ATC_CONFIG_T             *atc_config_ptr,
    APP_MN_LND_UPDATE_CNF_T    *sig_ptr,
    BOOLEAN                 expt_event_recv_flag
)
{
#ifndef _ULTRA_LOW_CODE_

    ATC_TRACE_LOW("ATC:ATC_ProcessDelLNDAllCnf, delete entry(id=%d) by ATC.", sig_ptr->entry_id);

    //delete successfully.
    if(sig_ptr->is_ok)
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_OK, 0);
    }
    //fail to delete or update the item of phonebook
    else
    {
        ATC_BuildResultCodeRsp(atc_config_ptr, S_ATC_RESULT_CODE_ERROR , ERR_UNKNOWN);
    }

#endif /* _ULTRA_LOW_CODE_ */

    return S_ATC_DEFAULT_HANDLE;
}
#endif
/*****************************************************************************/
//  Description : This function handles the AT+CPBP command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBP)
{
#ifndef _ULTRA_LOW_CODE_
    uint8                 number_len;
    uint8                *number_ptr = PNULL;// holds the telephone number
    uint8                 number_type;
    PHONEBOOK_ENTRY_T     entry;
    PHONEBOOK_ENTRY_T     tmp_entry;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    BOOLEAN               status;
    uint8                 tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2] = {0};
    uint8                 i    = 0;
    BOOLEAN               is_internation_num_flag = FALSE;
    char                  tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint32                return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E         dual_sys = ATC_CURRENT_SIM_ID_MUX;
#ifdef HUAWEI_6500_MODEM
    uint8                  *new_alpha_ptr = NULL;
#endif

    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));
    SCI_MEMSET(((void *)&tmp_entry), 0, sizeof(PHONEBOOK_ENTRY_T));

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
            //set command AT+CPBP= <PhoneNumber>
        case ATC_CMD_TYPE_SET:

            //check the index
            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                ATC_TRACE_LOW("ATC: ATC_ProcessCPBP, param1 is not string!");
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //check the length of input
            if(PARA1.str_ptr->str_len > (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //Get the telpone number and chage it to BCD
            number_ptr = PARA1.str_ptr->str_ptr;
            number_ptr[PARA1.str_ptr->str_len] = '\0';

	        if(*number_ptr == '+')
	        {
	           number_ptr++;
		       PARA1.str_ptr->str_len--;
		  
                number_type = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                entry.ton = (MN_NUMBER_TYPE_E)((number_type & 0x70) >> 4);  // get the bit 7,6,5 //@Shijun.cui 2005-09-14 CR31196
                entry.npi = (MN_NUMBER_PLAN_E)(number_type & 0x0f);  // get the bit 1,2,3,4
	        }
            number_len = (uint8)strlen((char *)number_ptr);
            TransfromStrToBcd(tmp_entry.number, &tmp_entry.number_len, number_ptr, number_len);

            //find the item by phone number
            status = PHONEBOOK_FindPhonebookEntryByNumEx(dual_sys, s_default_storage[dual_sys], tmp_entry.number, tmp_entry.number_len, &entry);

            if(status == MN_RETURN_SUCCESS)
            {
                uint8   toa;
                toa = (uint8)((entry.ton << 4) + entry.npi + 128);
                TransformBcdToStr(entry.number, entry.number_len, tel_str);
				
                /*If tel_type is international, read tel_str add '+'*/	
                if(entry.ton == MN_NUM_TYPE_INTERNATIONAL || tel_str[0] == '+')
                {
                    number_ptr = tel_str;
            	      number_ptr[entry.number_len*2+2] = '\0';
                    for(i = entry.number_len*2+1; i>0 ; i--)
                    {
                        number_ptr[i] = number_ptr[i-1];
                    }
                    number_ptr[0] = '+';
                    is_internation_num_flag = TRUE;
                }
				
            #ifdef HUAWEI_6500_MODEM
                  new_alpha_ptr = entry.alpha;
                  new_alpha_ptr[entry.alpha_len+1] ='\0';
              
                if(ATC_UCS2_80_CODE == entry.alpha[0] ||ATC_UCS2_81_CODE == entry.alpha[0] ||ATC_UCS2_82_CODE == entry.alpha[0])
                {
                    new_alpha_ptr++;
                    entry.alpha_len--;          
                    ConvertBinToHex(new_alpha_ptr, entry.alpha_len, (uint8 *)tel_alpha);
                }
            #else
                if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                {
                    ConvertBinToHex((uint8 *)entry.alpha, entry.alpha_len, (uint8 *)tel_alpha);
                }
            #endif
                else
                {
                    //SCI_ASSERT(entry.alpha_len <= PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2);
                    SCI_MEMCPY(tel_alpha, (char *)entry.alpha, entry.alpha_len);
                }

               if(is_internation_num_flag)
               {
                     sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                            g_atc_info_table[AT_CMD_CPBP].cmd_name->str_ptr,
                            entry.entry_id, number_ptr, toa, tel_alpha);              
               }
               else
               {
                   sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                        g_atc_info_table[AT_CMD_CPBP].cmd_name->str_ptr,
                        entry.entry_id, tel_str, toa, tel_alpha);               
               }

                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                s_last_read_index[dual_sys][s_default_storage[dual_sys]] = entry.entry_id;
                return_value = S_ATC_SUCCESS;
            }
            else
            {
                return_value = S_ATC_FAIL;
            }

            break;
            // the test command AT+CPBP=?
        case ATC_CMD_TYPE_TEST:
            // return the <nlength> and <tlength>
            sprintf((char *)g_rsp_str, "%s: %d,%d",
                    g_atc_info_table[AT_CMD_CPBP].cmd_name->str_ptr,
                    phonebook_info.max_record_num, (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len));
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handles the AT+CPBN command
//  Global resource dependence : g_rsp_str,s_last_read_index
//                               s_last_written_index
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCPBN)
{
#ifndef _ULTRA_LOW_CODE_
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
    BOOLEAN              status;
    uint8                tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2] = {0};
    uint8               *number_ptr = NULL;
    char                 tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint8                tel_type;
    int32                i = 0;
    BOOLEAN              is_internation_num_flag = FALSE;
    uint16               stored_index = 0;
    uint32               return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));
    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
            //Test command AT+CPBN=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (0-5)",
                    g_atc_info_table[AT_CMD_CPBN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;
            //Set command AT+CPBN=<mode>
        case ATC_CMD_TYPE_SET:

            // check the index
            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //@Shijun.cui 2005-07-04 CR27123
            if(phonebook_info.used_record_num == 0)
            {
                return_value = S_ATC_SUCCESS;
                break;
            }

            //End CR27123

            switch(PARA1.num)
            {
                    //read the first the item
                case AT_CPBN_FIRST:
                    entry.entry_id = 1;

                    for(i = 0; i < phonebook_info.max_record_num; i++)
                    {
                        if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry))
                        {
                            break;
                        }
                        else
                        {
                            entry.entry_id++;
                        }
                    }

                    if(entry.entry_id <= phonebook_info.max_record_num)
                    {
                        s_last_read_index[dual_sys][s_default_storage[dual_sys]] = entry.entry_id;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                    //read the last the item
                case AT_CPBN_LAST:
                    entry.entry_id = phonebook_info.max_record_num ;

                    for(i = phonebook_info.max_record_num; i > 0; i--)
                    {
                        if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry))
                        {
                            break;
                        }
                        else
                        {
                            entry.entry_id--;
                        }
                    }

                    if(entry.entry_id > 0)
                    {
                        s_last_read_index[dual_sys][s_default_storage[dual_sys]] = entry.entry_id;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                    //Next valid item in alphabetical order
                case AT_CPBN_NEXT:
                    /*Find the previous item.If meeting the begin of phonebook,
                    turn to end of phonebook*/
                    stored_index = s_last_read_index[dual_sys][s_default_storage[dual_sys]];

                    do
                    {
                        if(stored_index < phonebook_info.max_record_num)
                        {
                            stored_index++;
                        }
                        else
                        {
                            stored_index = 1;
                        }

                        entry.entry_id = stored_index;
                        i++;
                    }
                    while((ERR_MNDATAMAG_NO_ERR != PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry))
                            && (i < phonebook_info.max_record_num));

                    //check if the item is found,or return error
                    if(i <= phonebook_info.max_record_num) //@Shijun.cui 2005-07-04 CR27123
                    {
                        s_last_read_index[dual_sys][s_default_storage[dual_sys]] = stored_index;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                    //Previous valid item in alphabetical order
                case AT_CPBN_PREVIOUS:
                    /*Find the previous item.If meeting the begin of phonebook,
                    turn to end of phonebook*/
                    stored_index = s_last_read_index[dual_sys][s_default_storage[dual_sys]];

                    do
                    {
                        if(stored_index > 1)
                        {
                            stored_index--;
                        }
                        else
                        {
                            stored_index = phonebook_info.max_record_num;
                        }

                        entry.entry_id = stored_index;
                        i++;
                    }
                    while((ERR_MNDATAMAG_NO_ERR != PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry))
                            && (i < phonebook_info.max_record_num));

                    //check if the item is found,or return error
                    if(i <= phonebook_info.max_record_num) //@Shijun.cui 2005-07-04 CR27123
                    {
                        s_last_read_index[dual_sys][s_default_storage[dual_sys]] = stored_index;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                    //Last item read
                case AT_CPBN_LATESTREAD:

                    if(s_last_read_index[dual_sys][s_default_storage[dual_sys]] != 0)
                    {
                        entry.entry_id = s_last_read_index[dual_sys][s_default_storage[dual_sys]];
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                    //Last item written
                case AT_CPBN_LATESTWRITTEN:

                    if(s_last_written_index[dual_sys][s_default_storage[dual_sys]] != 0)
                    {
                        entry.entry_id = s_last_written_index[dual_sys][s_default_storage[dual_sys]];
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                default:
                    SCI_ASSERT(FALSE);/*assert verified: check valid value*/
                    break;
            }

            //Get and show the item
            if(S_ATC_FAIL != return_value)
            {
                status = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);

                if(ERR_MNDATAMAG_NO_ERR == status)
                {
                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
                    tel_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                    entry.alpha[entry.alpha_len] = '\0';

			/*If tel_type is international, read tel_str add '+'*/	
                    if(entry.ton == MN_NUM_TYPE_INTERNATIONAL || tel_str[0] == '+')
                    {
                        number_ptr = tel_str;
                	    number_ptr[entry.number_len*2+2] = '\0';
                        for(i = entry.number_len*2+1; i>0 ; i--)
                        {
                            number_ptr[i] = number_ptr[i-1];
                        }
                        number_ptr[0] = '+';
                        is_internation_num_flag = TRUE;
                    }

                    if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8 *)entry.alpha, entry.alpha_len, (uint8 *)tel_alpha);
                    }
                    else
                    {
                        //SCI_ASSERT(entry.alpha_len <= PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2);
                        SCI_MEMCPY(tel_alpha, (char *)entry.alpha, entry.alpha_len);
                    }

                    if(is_internation_num_flag)
                    {
                        sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                                g_atc_info_table[AT_CMD_CPBN].cmd_name->str_ptr,
                                entry.entry_id, number_ptr, tel_type, tel_alpha);                    
                    }
                    else
                    {
                        sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                                g_atc_info_table[AT_CMD_CPBN].cmd_name->str_ptr,
                                entry.entry_id, tel_str, tel_type, tel_alpha);                    
                    }
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
                    return_value = S_ATC_SUCCESS;
                }
                else
                {
                    return_value = S_ATC_FAIL;
                }
            }

            break;
        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
/*****************************************************************************/
//  Description : This function handles the AT+CNUM command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCNUM)
{
#ifndef _ULTRA_LOW_CODE_
    PHONEBOOK_INFO_T       phonebook_info;
    PHONEBOOK_ENTRY_T      entry;
    uint8                  tel_str[(PHONEBOOK_MAX_DN_NUMBER_LEN * 2) + 2] = {0};
    char                   tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint8                  tel_type;
    uint32                 i;
    uint32                 return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));
    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, PHONEBOOK_MSISDN_STORAGE);

    switch(ATC_GET_CMD_TYPE)
    {
            //Test command AT+CNUM?
        case ATC_CMD_TYPE_TEST:
            ATC_TRACE_LOW("ATC: ATC_CMD_TYPE_TEST");
            break;
            //Excute command AT+CNUM
        case ATC_CMD_TYPE_EXECUTE:

            //Get the MSISDN from the MN
            for(i = 0; i < phonebook_info.max_record_num; i++)
            {
                entry.entry_id = i + 1;

                if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_MSISDN_STORAGE, &entry))
                {
                    SCI_MEMSET(tel_str,'\0', sizeof (tel_str));

                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
                    tel_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                    entry.alpha[entry.alpha_len] = '\0';
                    SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));

                    if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8 *)entry.alpha, entry.alpha_len, (uint8 *)tel_alpha);
                    }
                    else
                    {
                        SCI_MEMCPY(tel_alpha, (char *)entry.alpha, entry.alpha_len);
                    }

                    sprintf((char *)g_rsp_str, "%s: \"%s\",\"%s\",%d",
                            g_atc_info_table[AT_CMD_CNUM].cmd_name->str_ptr, tel_alpha,
                            tel_str, tel_type);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
                }
            }

            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+CSVM command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessCSVM)
{
#ifndef _ULTRA_LOW_CODE_
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
    BOOLEAN              status = FALSE;
    BOOLEAN              is_voicemail_enable = FALSE;
    uint8                number_type;
    uint8                number_len;
    uint8                *number_ptr;      // holds the telephone number
    uint8                tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN * 2 +2] = {0};

    uint32               return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));
    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
            // test command AT+CSVM=?
        case ATC_CMD_TYPE_TEST:
            //+CSVM: (0-1),20,(129,145)
            sprintf((char *)g_rsp_str, "%s: (0-1),%d,(129,145)",
                    g_atc_info_table[AT_CMD_CSVM].cmd_name->str_ptr,
                    (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len));
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;
            // read command AT+CSVM?
        case ATC_CMD_TYPE_READ:
            entry.entry_id = 1; //the voice mail number is only one.
            status = PHONEBOOK_ReadVoiceMailAddrEx(dual_sys, &entry);

            if(MN_RETURN_SUCCESS == status)
            {
                ATC_TRACE_LOW("ATC:PHONEBOOK_ReadVoiceMailAddr IS OK");
                is_voicemail_enable = PHONEBOOK_IsVoiceMailAddrEnableEx(dual_sys);
                TransformBcdToStr(entry.number, entry.number_len, tel_str);
                number_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                entry.alpha[entry.alpha_len] = '\0';
                sprintf((char *)g_rsp_str, "%s: %d,\"%s\",%d",
                        g_atc_info_table[AT_CMD_CSVM].cmd_name->str_ptr,
                        is_voicemail_enable, tel_str, number_type);
                ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            }
            else
            {
                ATC_TRACE_LOW("ATC: PHONEBOOK_ReadVoiceMailAddr IS ERROR!");
                return_value = S_ATC_FAIL;
            }

            break;

            // set command AT+CSVM=<mode>[,<number>[,<type>]]
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                is_voicemail_enable = PARA1.num;

                // call the enable functions
                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_EnableVoiceMailAddrEx(dual_sys, is_voicemail_enable))
                {
                    ATC_TRACE_LOW("ATC:PHONEBOOK_EnableVoiceMailAddr failed");
                    return_value = S_ATC_FAIL;
                    break;
                }
            }
            else
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(PARAM2_FLAG && PARAM3_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_ProcessCSVM, param2 is not string!");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                //check the length of input
                if(PARA2.str_ptr->str_len > (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len)
                        || 0 == PARA2.str_ptr->str_len)
                {
                    ATC_TRACE_LOW("ATC:AT+CSVM,error in length of input");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                // Get the voice mail number
                number_ptr = PARA2.str_ptr->str_ptr;
                number_ptr[PARA2.str_ptr->str_len] = '\0';
                number_len = (uint8)strlen((char *)number_ptr);
                TransfromStrToBcd(entry.number, &entry.number_len, number_ptr, number_len);

                //hanle the para3
                number_type = (uint8)PARA3.num;

                if(!(number_type == UNKNOWN_PLAN_UNKNOWN_TYPE ||
                        number_type == ISDN_TELE_PLAN_UNKNOWN_TYPE ||
                        number_type == ISDN_TELE_PLAN_INTERNATIONAL_TYPE ||
                        number_type == ISDN_TELE_PLAN_NATIONAL_TYPE))
                {
                    // Operation not supported
                    ATC_TRACE_LOW("ATC:ERR_OPERATION_NOT_SUPPORTED");
                    return_value = ERR_OPERATION_NOT_SUPPORTED;
                    break;
                }

                // the IE of type of number
                entry.ton = (MN_NUMBER_TYPE_E)(number_type & 0x70); // get the bit 7,6,5
                entry.npi = (MN_NUMBER_PLAN_E)(number_type & 0x0f);  // get the bit 1,2,3,4

                // call the update functions
                entry.entry_id = 1; //the voice mail number is only one.

                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_UpdateVoiceMailAddrEx(dual_sys, &entry))
                {
                    ATC_TRACE_LOW("ATC:PHONEBOOK_UpdateVoiceMailAddr failed");
                    return_value = S_ATC_FAIL;
                    break;
                }

                //update the enable flag
                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_EnableVoiceMailAddrEx(dual_sys, is_voicemail_enable))
                {
                    ATC_TRACE_LOW("ATC:PHONEBOOK_EnableVoiceMailAddr failed");
                    return_value = S_ATC_FAIL;
                    break;
                }
            }
            else
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            return_value = S_ATC_SUCCESS;
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the AT+SDCP command
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC(ATC_ProcessSDCP)
{
#ifndef _ULTRA_LOW_CODE_
    uint8                *para_ptr = PNULL;
    int32                i; // loop index

    uint32              return_value = S_ATC_SUCCESS;
    PHONEBOOK_STORAGE_E storage = PHONEBOOK_2G_MAX_STORAGE_NUM;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    ERR_MNDATAMAG_CODE_E err_code = ERR_MNDATAMAG_IS_NULL;

    switch(ATC_GET_CMD_TYPE)
    {
            // test command AT+SDCP=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "%s: (LD,MC,RC)",
                    g_atc_info_table[AT_CMD_SDCP].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

            // read command AT+SDCP?
        case ATC_CMD_TYPE_READ:
            ATC_TRACE_LOW("ATC: ATC_CMD_TYPE_READ");
            break;

        case ATC_CMD_TYPE_SET:

            //check the index
            if(!PARAM1_FLAG)
            {
                ATC_TRACE_LOW("ATC: ERR_INVALID_INDEX");
                return_value = ERR_INVALID_INDEX;
                break;
            }

            para_ptr = PARA1.str_ptr->str_ptr;
            para_ptr[PARA1.str_ptr->str_len] = '\0';

            if(strcmp((char *)para_ptr, "LD")
                    && strcmp((char *)para_ptr, "MC")
                    && strcmp((char *)para_ptr, "RC"))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            // Compare the inputed storage string
            for(i = 0; i < PHONEBOOK_2G_MAX_STORAGE_NUM; i++)
            {
                if(atc_storage_str[i] && !strcmp((char *)para_ptr, atc_storage_str[i]))
                {
                    ATC_TRACE_LOW("ATC: storage = %d", i);
                    storage = (PHONEBOOK_STORAGE_E)i;
					
                    //delete all the item
                    err_code = PHONEBOOK_DeleteLndAllEntryEx(dual_sys);
                    if(err_code == ERR_MNDATAMAG_NO_ERR)
                    {
                        ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr,
                            APP_MN_LND_UPDATE_CNF, 0xff, ATC_DOMAIN_PHONEBOOK, dual_sys);
                    }
                    else
                    {
                        ATC_TRACE_LOW("ATC: Delete Lnd all entry Error");
                        return_value = ERR_INVALID_INDEX;
                        break;	               
                    }

                }
            }
			
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

/*****************************************************************************/
//  Description : This function handles the APP_MN_CALL_READY_IND message
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessCallReadyIndByPB(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    APP_MN_CALL_READY_IND_T *sig_ptr
)
{
    SIMCHECK_GET_SIM_TYPE_CNF_T temp;

    if(sig_ptr->is_usim)
    {
        atc_config_ptr->sim_type[dual_sys] = MN_CARD_TYPE_USIM;
    }
    else
    {
        MNPHONE_GetSimtypeEx(dual_sys,&temp);
        if ((MN_SIM_TYPE_SUCCESS == temp.cause) && 
            ((MN_CARD_TYPE_DUAL == temp.card_type) || (MN_CARD_TYPE_UIM == temp.card_type)))
        {
            //If EUICC has been used, use the MN saved value.
            //Because is_usim cannot indicate UIM or DUAL.
            atc_config_ptr->sim_type[dual_sys] = temp.card_type;
        }
        else
        {
            atc_config_ptr->sim_type[dual_sys] = MN_CARD_TYPE_SIM;
        }
    }

    if(1 == atc_config_ptr->spreadyflag)
    {
        sprintf((char *)g_rsp_str, "*SPREADY: 0");
        ATC_BuildUnsolicitedInfo(sig_ptr->dual_sys, atc_config_ptr, g_rsp_str);
    }

    return S_ATC_DEFAULT_HANDLE;
}

/*****************************************************************************/
//  Description : This function starts to read phonebook info(ADN)
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL void HandlePhonebookRead(
    ATC_CONFIG_T                *atc_config_ptr,
    uint16                                cmd_id,                //command identity
    PHONEBOOK_ENTRY_T     *entry_ptr,           //[in] phonebook entry
    BOOLEAN                           is_usim            //[in] report usim/sim format info
    //APP_MN_READ_ADDITIONAL_ENTRY_T    *entry_additional_ptr
    //[in] used for usim pb entry,is_usim must be TRUE.
)
{
    uint8    tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 2] = {0};
    uint8    tel_alpha[(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN<<1)+1] = {0};
    uint8    tel_type;
    uint8    is_ucs2 = 0;
    uint8     *number_ptr = NULL;
    uint8    i  = 0;
    BOOLEAN   is_internation_num_flag = FALSE;
#ifdef HUAWEI_6500_MODEM
    uint8                    *new_alpha_ptr = NULL;
#endif

#ifdef _ATC_WAIT_
    uint32  loop;
#endif
    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != entry_ptr);/*assert verified: check null pointer*/

    ATC_TRACE_LOW("ATC:HandlePhonebookRead,entry_id:%d,is_usim:%d", entry_ptr->entry_id, is_usim);
    //First step, decode ADN info.
    /*get telephone number string from BCD format*/
    SCI_MEMSET(tel_str, '\0', (PHONEBOOK_MAX_DN_NUMBER_LEN * 2 + 2));
    TransformBcdToStr(entry_ptr->number, entry_ptr->number_len, tel_str);
    /*get telephone type according to ton and npi info*/
    tel_type = (uint8)((entry_ptr->ton << 4) + entry_ptr->npi + 128);

    /*If tel_type is international, read tel_str add '+'*/	
    if(entry_ptr->ton == MN_NUM_TYPE_INTERNATIONAL || tel_str[0] == '+')
    {
        number_ptr = tel_str;
	    number_ptr[entry_ptr->number_len*2+2] = '\0';
        for(i = entry_ptr->number_len*2+1; i>0 ; i--)
        {
            number_ptr[i] = number_ptr[i-1];
        }
        number_ptr[0] = '+';
        is_internation_num_flag = TRUE;

    }
    
    /*get phonebook alpha id*/
    SCI_MEMSET(tel_alpha, '\0', (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2 + 1));

#ifdef HUAWEI_6500_MODEM
    new_alpha_ptr = entry_ptr->alpha;
    new_alpha_ptr[entry_ptr->alpha_len+1] ='\0';
#endif	

    if(ATC_UCS2_80_CODE == entry_ptr->alpha[0] ||
            ATC_UCS2_81_CODE == entry_ptr->alpha[0] ||
            ATC_UCS2_82_CODE == entry_ptr->alpha[0])
    {
    #ifdef HUAWEI_6500_MODEM
        new_alpha_ptr++;
        entry_ptr->alpha_len--;          
        ATC_TranUint8IntoChar(new_alpha_ptr, entry_ptr->alpha_len, tel_alpha);
    #else
	    ATC_TranUint8IntoChar(entry_ptr->alpha, entry_ptr->alpha_len, tel_alpha);
    #endif
        is_ucs2 = 1;    //UCS2 coding
    }
    else
    {
        SCI_MEMCPY(tel_alpha, (char *)entry_ptr->alpha, entry_ptr->alpha_len);
        is_ucs2 = 0;    //GSM 7bit coding
    }

    if(is_usim)
    {
    #ifdef _ATC_WAIT_
        uint8    anr_str[MNSIM_ANR_MAX_NUM][PHONEBOOK_MAX_DN_NUMBER_LEN * 2 + 2];
        uint8    anr_type[MNSIM_ANR_MAX_NUM] = {0, 0, 0};
        uint8    email_alpha[(PHONEBOOK_EMAIL_STRING_MAX_LEN << 1) + 1] = {0};
        APP_MN_READ_ADDITIONAL_ENTRY_T     entry_add_info;
        APP_MN_READ_ADDITIONAL_ENTRY_T    *entry_add_ptr = NULL;

        if(NULL != entry_additional_ptr)
        {
            entry_add_ptr = entry_additional_ptr;
        }
        else
        {
            SCI_MEMSET(&entry_add_info, 0x0, sizeof(APP_MN_READ_ADDITIONAL_ENTRY_T));
            entry_add_ptr = &entry_add_info;
        }

        SCI_ASSERT(NULL != entry_add_ptr);/*assert verified: check null pointer*/

        //Second step, decode additional info, such as ADR,email...

        /*get ANR info*/
        for(loop = 0; loop < MNSIM_ANR_MAX_NUM; loop++)
        {
            SCI_MEMSET(anr_str[loop], '\0', (PHONEBOOK_MAX_DN_NUMBER_LEN * 2  + 2));

            if(entry_add_ptr->anr_ret[loop])
            {
                TransformBcdToStr(entry_add_ptr->anr_number[loop].number,
                                  entry_add_ptr->anr_number[loop].number_len,
                                  anr_str[loop]);

                if(0x0 != anr_str[loop][0] && 0xFF != anr_str[loop][0])
                {
                    // Number field is valid
                    anr_type[loop] = (uint8)((entry_add_ptr->anr_number[loop].ton << 4) +
                                             entry_add_ptr->anr_number[loop].npi + 128);
                }
                else
                {

                    if(0x1 == entry_add_ptr->anr_number[loop].ton)
                    {
                        //Only ton/npi exist
                        anr_type[loop] = (uint8)((entry_add_ptr->anr_number[loop].ton << 4) +
                                                 entry_add_ptr->anr_number[loop].npi + 128);
                    }
                    else
                    {
                        //ANR is empty
                        anr_type[loop] = 0x0;
                    }
                }
            }
        }

        /*get email alpha info*/
        SCI_MEMSET(email_alpha, '\0', (PHONEBOOK_EMAIL_STRING_MAX_LEN * 2 + 1));

        if(entry_add_ptr->email_ret)
        {
            SCI_MEMCPY(email_alpha,
                       (char *)entry_add_ptr->email_addr.address,
                       entry_add_ptr->email_addr.address_len);

            // email decoding to ASCII
            for(loop = 0; loop < entry_add_ptr->email_addr.address_len; loop++)
            {
                if(0 == email_alpha[loop])
                {
                    email_alpha[loop] = 0x40;
                }

                if(0x11 == email_alpha[loop])
                {
                    email_alpha[loop] = 0x5F;
                }
            }
        }
    #endif
#ifndef ATC_SAT_ENABLE
        if(AT_CMD_xor_SCPBR == cmd_id)
        {
        #ifdef _ATC_WAIT_
            /*report ^SCPBR:<index>,<num1>,<type>,<num2>,<type>,<num3>,<type>,<num4>,<type>,<text>,<coding>[,<email>][[...]*/
            sprintf((char *)g_rsp_str, "^SCPBR: %u,\"%s\",%d,\"%s\",%d,\"%s\",%d,\"%s\",%d,\"%s\",%d,\"%s\"",
                    entry_ptr->entry_id,    //<index>
                    tel_str, tel_type,        //<num1>,<type>
                    anr_str[0], anr_type[0],   //<num2>,<type>
                    anr_str[1], anr_type[1],   //<num3>,<type>
                    anr_str[2], anr_type[2],   //<num4>,<type>
                    tel_alpha, is_ucs2, email_alpha   //<text>,<coding>[,<email>]
                   );
        #else
            /*report info for ^SCPBR command*/
            sprintf((char *)g_rsp_str, "^SCPBR: %u,\"%s\",%d,\"\",,\"\",,\"\",,\"%s\",%d,\"\"",
                    entry_ptr->entry_id, tel_str, tel_type, tel_alpha, is_ucs2);
        #endif
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        }
#endif
    }
    else
    {
        if(AT_CMD_CPBR == cmd_id)
        {
            /*report info for +CPBR command*/
            if(is_internation_num_flag)
            {
                sprintf((char *)g_rsp_str, "%s: %u,\"%s\",%d,\"%s\"",
                        g_atc_info_table[AT_CMD_CPBR].cmd_name->str_ptr,
                        entry_ptr->entry_id, number_ptr, tel_type, tel_alpha);                
            }
            else
            {
                 sprintf((char *)g_rsp_str, "%s: %u,\"%s\",%d,\"%s\"",
                        g_atc_info_table[AT_CMD_CPBR].cmd_name->str_ptr,
                        entry_ptr->entry_id, tel_str, tel_type, tel_alpha);           
            }

            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        }
        else
        {
            /*report info for ^SCPBR command*/
            sprintf((char *)g_rsp_str, "^SCPBR: %u,\"%s\",%d,\"\",,\"\",,\"\",,\"%s\",%d,\"\"",
                    entry_ptr->entry_id, tel_str, tel_type, tel_alpha, is_ucs2);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
        }
    }

}

/*****************************************************************************/
//  Description : This function starts to read phonebook info(ADN)
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS ReadPhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 additional_flag,  //[in]has adr,email and etc or not
    PHONEBOOK_ENTRY_T      *entry_ptr         //[in] empty entry except entry_id assigned
    //[out] adn info
)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    ERR_MNDATAMAG_CODE_E result;

    SCI_ASSERT(NULL != atc_config_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != entry_ptr);/*assert verified: check null pointer*/

    result = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, storage, entry_ptr);

    if(ERR_MNDATAMAG_NO_ERR != result)
    {
        //ATC_TRACE_LOW("ATC: Read ADN entry(id:%d) failed,cause:%d.",entry_ptr->entry_id,result);
        status = S_ATC_FAIL;
    }

    return status;
}

/*****************************************************************************/
//  Description : This function is used to delete phonebook info
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS DeletePhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 is_usim,          //[in] usim or sim
    uint16                  entry_id          //[in]entry id to be delete
)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    ERR_MNDATAMAG_CODE_E  result_code;

    /*SIM entry operation procedure*/
    result_code = PHONEBOOK_DeletePhonebookEntryEx(dual_sys, storage, entry_id);

    if(ERR_MNDATAMAG_NO_ERR != result_code)
    {
        //if the type of storage is "sm" or "fd",return ok by message
        if(AT_STORAGE_SM == storage
                || AT_STORAGE_FD == storage)
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_ADN_UPDATE_CNF, entry_id, ATC_DOMAIN_PHONEBOOK, dual_sys);
            status = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            status = S_ATC_FAIL;
        }

        ATC_TRACE_LOW("ATC:PHONEBOOK_DeletePhonebookEntry failed,cause:%u", result_code);
    }
    else
    {
        //if the type of storage is "sm" or "fd",return ok by message
        if(AT_STORAGE_SM == storage
                || AT_STORAGE_FD == storage)
        {
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                              APP_MN_ADN_UPDATE_CNF, entry_id, ATC_DOMAIN_PHONEBOOK, dual_sys);
            status = S_ATC_DEFAULT_HANDLE;
        }
    }

    return status;
}

/*****************************************************************************/
//  Description : This function is used to update phonebook info
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL ATC_STATUS GSM_UpdatePhonebook(
    MN_DUAL_SYS_E   dual_sys,
    ATC_CONFIG_T           *atc_config_ptr,
    PHONEBOOK_STORAGE_E     storage,          //[in] storage type
    BOOLEAN                 is_usim,          //[in] usim or sim
    PHONEBOOK_ENTRY_T      *entry_ptr         //[in] entry to be update
)
{
    ATC_STATUS    status = S_ATC_SUCCESS;
    ERR_MNDATAMAG_CODE_E  result_code;
    uint16                entry_id;

    SCI_ASSERT(NULL != entry_ptr);/*assert verified: check null pointer*/
    entry_id = entry_ptr->entry_id;

    /*SIM entry operation procedure*/
    result_code = PHONEBOOK_UpdatePhonebookEntryEx(dual_sys, storage, entry_ptr);

    if(ERR_MNDATAMAG_NO_ERR != result_code)
    {
        //if the type of storage is "sm" or "fd",return ok by message
        if(AT_STORAGE_SM == storage || AT_STORAGE_FD == storage)
        {
            status = S_ATC_DEFAULT_HANDLE;
        }
        else
        {
            ATC_TRACE_LOW("ATC:PHONEBOOK_UpdatePhonebookEntry failed,cause:%lu", result_code);
            status = S_ATC_FAIL;
        }
    }
    else
    {
        //if the type of storage is "sm" or "fd",return ok by message
        if(AT_STORAGE_SM == storage|| AT_STORAGE_FD == storage)
        {
            if(entry_id==0)
            {
                entry_id=0xff;
                SCI_TRACE_LOW("CPBW index is default, reset as %d",entry_id);
            }
            
            ATC_ADD_EXPECTED_EVENT_MUX(atc_config_ptr, 
                       APP_MN_ADN_UPDATE_CNF, entry_id, ATC_DOMAIN_PHONEBOOK, dual_sys);
            status = S_ATC_DEFAULT_HANDLE;
            
            //Record the writen index
            s_last_written_index[dual_sys][storage] = entry_id;
        }
   
    }

    return status;
}

/*****************************************************************************/
//  Description : This function is used to get the telephone number(BCD format)
//              according to AT command parameters
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTelephoneNumByATPara(
    BOOLEAN     tel_flag,   //[in]telephone is present flag
    ATC_CUR_PARAM_STRING_T   *tel_num_ptr,   //[in]telephone info
    BOOLEAN     *is_plus_in_tel_num_ptr,     //[out]phone number with a prefix '+' or not
    uint8        *bcd_tel_num_ptr,    //[out]bcd phone number
    uint8        *bcd_tel_len_ptr     //[out]bcd phone number length
)
{
    uint8     *number_ptr = NULL;
    uint8     number_len;
    BOOLEAN   cmd_flag;
    BOOLEAN   is_internation_num = FALSE;

    SCI_ASSERT(NULL != tel_num_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != bcd_tel_num_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != bcd_tel_len_ptr);/*assert verified: check null pointer*/

    if(tel_flag)
    {
        /*telephone number is input*/

        if(tel_num_ptr->str_len > ATC_ADN_NUM_LEN)
        {
            tel_num_ptr->str_ptr[ATC_ADN_NUM_LEN] = '\0';
            tel_num_ptr->str_len = ATC_ADN_NUM_LEN;
        }

        // Get the given phone number
        number_ptr = tel_num_ptr->str_ptr;
        number_ptr[tel_num_ptr->str_len] = '\0';

        if(*number_ptr == '+')
        {
            is_internation_num = TRUE;
            number_ptr++;
        }

        number_len = (uint8)strlen((char *)number_ptr);
        cmd_flag = TransfromStrToBcd(bcd_tel_num_ptr, bcd_tel_len_ptr, number_ptr, number_len);

        if(!cmd_flag)
        {
            return FALSE;
        }

        if(0 == tel_num_ptr->str_len)
        {
            bcd_tel_len_ptr = 0;
        }

        if(NULL != is_plus_in_tel_num_ptr)
        {
            *is_plus_in_tel_num_ptr = is_internation_num;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : This function is used to get the telephone type
//              according to AT command parameters
//  Global resource dependence:
//  Author:  minqian.qian
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetTelephoneTypeByATPara(
    BOOLEAN     tel_type_flag,    //[in]telephone type is present flag
    uint8       tel_type_value,   //[in]telephone type value input by user
    BOOLEAN     is_plus_in_tel_num,  //[in]the telephone num has '+' or not
    uint8       *tel_type_final     //[out]the telephone type according to
    //tel number and type input by user
)
{
    SCI_ASSERT(NULL != tel_type_final);/*assert verified: check null pointer*/

    if(is_plus_in_tel_num)
    {
        //when user input a phone number with a prefix '+',  force the number type as intertional type
        *tel_type_final = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
        return TRUE;
    }

    if(tel_type_flag)
    {
        /*telephone type is input */
        if(!(tel_type_value == UNKNOWN_PLAN_UNKNOWN_TYPE ||
                tel_type_value == ISDN_TELE_PLAN_UNKNOWN_TYPE ||
                tel_type_value == ISDN_TELE_PLAN_INTERNATIONAL_TYPE ||
                tel_type_value == ISDN_TELE_PLAN_NATIONAL_TYPE))
        {
            // Operation not supported
            return FALSE;
        }
        else
        {
            //set the value input by user
            *tel_type_final = tel_type_value;
        }
    }
    else
    {
        /*telephone type is not input */
        // set the default value
        *tel_type_final = UNKNOWN_PLAN_UNKNOWN_TYPE;
    }

    return TRUE;
}

#ifndef ATC_SAT_ENABLE
/*****************************************************************************/
//  Description : This function handles the AT^SCPBW command
//  Global resource dependence : g_rsp_str
//  Author:       WangHui
//  Modified by:  Eddie.Wang
//  Note:         Vcard is not support now .realize this function as cpbw
//                    Eddie.Wang add VCARD support at 2009/10/02
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SCPBW)
{
    uint32                return_value = S_ATC_SUCCESS;
#ifndef _ULTRA_LOW_CODE_
    uint16                index = 0;
    uint8                 number_type;
    BOOLEAN               is_num_internation_type = FALSE;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    PHONEBOOK_ENTRY_T     entry;
    char                  tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    BOOLEAN               is_usim;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;


    if(PHONEBOOK_SIM_STORAGE != s_default_storage[dual_sys])
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(atc_config_ptr->call_read_flag[dual_sys] != TRUE ||
       atc_config_ptr->sim_type[dual_sys] == MN_CARD_TYPE_NUM)
    {
        ATC_TRACE_LOW("ATC: SIM/USIM is not ready.");
        return ERR_SIM_BUSY;
    }

    if(MN_CARD_TYPE_USIM == atc_config_ptr->sim_type[dual_sys])
    {
        is_usim = TRUE;
    }
    else
    {
        is_usim = FALSE;
    }

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);
    SCI_MEMSET(((void *)&entry), 0, sizeof(PHONEBOOK_ENTRY_T));

    if((!phonebook_info.is_phonebook_exist) ||
            0 == phonebook_info.max_record_num)
    {
        ATC_TRACE_LOW("ATC: ^SCPBW, no phonebook entity.");
        return ERR_SIM_WRONG;
    }

    switch(ATC_GET_CMD_TYPE)
    {
            // Set command AT+CPBW=<index>[, <number>[,<type>[,<text>]]]
        case ATC_CMD_TYPE_SET:

            if(PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW, <index> is not numeric!");
                    return ERR_INVALID_INDEX;
                }

                index = (uint16)PARA1.num;

                if(index > phonebook_info.max_record_num || index < 1)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: index:%d is out of range(1-%d)", index, phonebook_info.max_record_num);
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                /*DELETE PROCEDURE*/
                if(!PARAM2_FLAG && !PARAM3_FLAG && !PARAM4_FLAG && !PARAM5_FLAG && !PARAM6_FLAG && !PARAM7_FLAG &&
                        !PARAM8_FLAG && !PARAM9_FLAG && !PARAM10_FLAG && !PARAM11_FLAG && !PARAM12_FLAG)
                {
                    /*If only one parameter,delete the entry according to the given entry id*/
                    return_value = DeletePhonebook(dual_sys, atc_config_ptr, s_default_storage[dual_sys], is_usim, index);
                    break;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: <index1> use default value");
                index = PHONEBOOK_DEFAULT_ENTRY_ID;
            }

            /*WRITE PROCEDURE*/
            if(!PARAM1_FLAG && !PARAM2_FLAG)
            {
                // the Operation not permitted
                ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: no index 1 and 2");
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(phonebook_info.max_record_num == phonebook_info.used_record_num)
            {
                ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: memory full.");
                return_value = ERR_MEMORY_FULL;
                break;
            }

            //check the length of input
            if(PARAM2_FLAG)
            {
                if(PARAM2_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW, <num1> is not string!");
                    return ERR_INVALID_INDEX;
                }

                if(0 == PARA2.str_ptr->str_len && !PARAM3_FLAG)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: index2 length is zero");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                /*verify and convert(string to bcd) num1*/
                if(!GetTelephoneNumByATPara(PARAM2_FLAG, PARA2.str_ptr,
                                            &is_num_internation_type,
                                            entry.number, &entry.number_len))
                {
                    // Operation not supported
                    return_value = ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
                    break;
                }
            }

            /*verify type1*/
            if(!GetTelephoneTypeByATPara(PARAM3_FLAG, (uint8)PARA3.num,
                                         is_num_internation_type, &number_type))
            {
                // Operation not supported
                return_value = ERR_OPERATION_NOT_SUPPORTED;
                break;
            }

            /*get number type and plan of <num1>*/
            entry.ton = (MN_NUMBER_TYPE_E)((number_type & 0x70) >> 4); // get the bit 7,6,5 //@Shijun.cui 2005-09-14 CR31196
            entry.npi = (MN_NUMBER_PLAN_E)(number_type & 0x0f);  // get the bit 1,2,3,4

            // check the alpha
            if(PARAM10_FLAG)
            {
                if(PARAM10_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW, <text> is not string!");
                    return ERR_INVALID_INDEX;
                }

                //check the length of input
                if(0 == PARA10.str_ptr->str_len ||
                        PARA10.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2)
                {
                    ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: <text> length is not in range(1-%d)",
                                  PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                SCI_MEMCPY(tel_alpha, (char *)PARA10.str_ptr->str_ptr, PARA10.str_ptr->str_len);
                //string input is UCS2
                ATC_TRACE_LOW("ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x", tel_alpha[0],
                              tel_alpha[1], tel_alpha[2], (PARA10.str_ptr->str_len) / 2);

                if(ConvertAscUcs2IntoBin(tel_alpha, entry.alpha, (PARA10.str_ptr->str_len) / 2) &&
                        (ATC_UCS2_80_CODE == *entry.alpha ||
                         ATC_UCS2_81_CODE == *entry.alpha ||
                         ATC_UCS2_82_CODE == *entry.alpha))
                {
                    ATC_TRACE_LOW("ATC:ATC_ProcessCPBW,ATC_UCS2_CODE == *entry.alpha");
                    entry.alpha_len  = PARA10.str_ptr->str_len / 2;
                }
                //string input is ASCII.
                else
                {
                    if(PARA10.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                    {
                        ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBW: index length is TOO long");
                        return_value = ERR_INVALID_INDEX;
                        break;
                    }

                    PARA10.str_ptr->str_ptr[PARA10.str_ptr->str_len] = '\0';
                    entry.alpha_len  = PARA10.str_ptr->str_len;

                    if(PARA10.str_ptr->str_len < PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                    {
                        SCI_MEMCPY(((char *)entry.alpha), ((char *)PARA10.str_ptr->str_ptr), (entry.alpha_len + 1));
                    }
                    else
                    {
                        SCI_MEMCPY(entry.alpha, PARA10.str_ptr->str_ptr, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
                    }

                    ATC_TRACE_LOW("ATC:^scpbw,PARA10.str_ptr=%x,%x,%x", *PARA10.str_ptr->str_ptr,
                                  *(PARA10.str_ptr->str_ptr + 1), *(PARA10.str_ptr->str_ptr + 2));

                    ATC_TRACE_LOW("ATC:cpbw,entry.alpha=%x,%x,%x", *entry.alpha, *(entry.alpha + 1), *(entry.alpha + 2));
                }
            }

            // call the update functions
            entry.entry_id = index;
            return_value = GSM_UpdatePhonebook(dual_sys, atc_config_ptr, s_default_storage[dual_sys], is_usim, &entry);
            break;

            // test command AT+CPBW=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "^SCPBW: (1-%d),%d,(%d,%d,%d,%d),%d,0",
                    phonebook_info.max_record_num, (ATC_ADN_NUM_LEN/*phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len*/),
                    UNKNOWN_PLAN_UNKNOWN_TYPE, ISDN_TELE_PLAN_UNKNOWN_TYPE,
                    ISDN_TELE_PLAN_INTERNATIONAL_TYPE, ISDN_TELE_PLAN_NATIONAL_TYPE,
                    phonebook_info.max_alpha_len);
            // call the transfer function
            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#endif /* _ULTRA_LOW_CODE_ */
    return return_value;
}

/*****************************************************************************/
//  Description : This function handles the AT^SCPBR command
//  Global resource dependence : g_rsp_str
//  Author:      WangHui
//  Modified by: Eddie.Wang
//  Note:        Vcard is not support now .realize this function as cpbr
//                   Eddie.Wang add VCARD support at 2009/10/02
/*****************************************************************************/
AT_CMD_FUNC(ATC_Processxor_SCPBR)
{
    uint32               return_value = S_ATC_SUCCESS;
#ifndef _ULTRA_LOW_CODE_
    int32                index1;
    int32                index2;
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
    ERR_MNDATAMAG_CODE_E    read_result;
    BOOLEAN               entry_exist_flag = FALSE;
    BOOLEAN               is_usim;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;


    if(PHONEBOOK_SIM_STORAGE != s_default_storage[dual_sys])
    {
        return ERR_OPERATION_NOT_ALLOWED;
    }

    if(atc_config_ptr->call_read_flag[dual_sys] != TRUE ||
            atc_config_ptr->sim_type[dual_sys] == MN_CARD_TYPE_NUM)
    {
        ATC_TRACE_LOW("ATC: SIM/USIM is not ready.");
        return ERR_SIM_BUSY;
    }

    if(MN_CARD_TYPE_USIM == atc_config_ptr->sim_type[dual_sys])
    {
        is_usim = TRUE;
    }
    else
    {
        is_usim = FALSE;
    }

    // get the phonebook information
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    if((!phonebook_info.is_phonebook_exist) ||
            0 == phonebook_info.max_record_num)
    {
        ATC_TRACE_LOW("ATC: ^SCPBR, no phonebook entity.");
        return ERR_SIM_WRONG;
    }

    switch(ATC_GET_CMD_TYPE)
    {
            //set command AT+CPBR=index1,index2
        case ATC_CMD_TYPE_SET:

            // check the index 1
            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            index1 = (uint16)PARA1.num;

            if(index1 > phonebook_info.max_record_num || index1 < 1)
            {
                ATC_TRACE_LOW("ATC: ATC_Processxor_SCPBR: index:%d is out of range(1-%d)", index1, phonebook_info.max_record_num);
                return_value = ERR_INVALID_INDEX;
                break;
            }

            // check the index2
            index2 = PARAM2_FLAG ? (uint16)PARA2.num : index1;

            if((index1 > index2) || (index2 > phonebook_info.max_record_num))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if(0 == phonebook_info.used_record_num)
            {
                ATC_TRACE_LOW("ATC: no phonebook record.");
                return_value = ERR_NOT_FOUND;
                break;
            }

            /*read sim records*/
            for(; index1 <= index2; index1++)
            {
                SCI_MEMSET(&entry, 0, sizeof(PHONEBOOK_ENTRY_T));
                entry.entry_id = (uint16)index1;

                read_result = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);

                if(ERR_MNDATAMAG_NO_ERR == read_result)
                {
                    HandlePhonebookRead(atc_config_ptr, AT_CMD_xor_SCPBR, &entry, FALSE);
                    //HandlePhonebookRead(atc_config_ptr, AT_CMD_xor_SCPBR, &entry, FALSE, NULL);
                    //Record the read index of phonebook
                    s_last_read_index[dual_sys][s_default_storage[dual_sys]] = index1;
                    entry_exist_flag = TRUE;
                }

            }

            if(entry_exist_flag)
            {
                return_value = S_ATC_SUCCESS;
            }
            else
            {
                /*no entry founded, report CME ERROR:NOT FOUND*/
                return_value = ERR_NOT_FOUND;
            }

            break;
            //Test command AT+CPBR=?
        case ATC_CMD_TYPE_TEST:
            sprintf((char *)g_rsp_str, "^SCPBR: (1-%d),%d,%d,0",
                    phonebook_info.max_record_num, ATC_ADN_NUM_LEN, /*phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len*/
                    phonebook_info.max_alpha_len);

            ATC_BuildInfoRsp(atc_config_ptr, (uint8 *)g_rsp_str);
            break;

        default:
            return ERR_OPERATION_NOT_SUPPORTED;
    }

#endif /* _ULTRA_LOW_CODE_ */

    return return_value;
}

/*****************************************************************************/
// 	Description : Update Call Info
//	Global resource dependence : none
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN ATC_RecordLNDCallInfo(MN_DUAL_SYS_E dual_sys, PHONEBOOK_STORAGE_E storage, MN_CALLED_NUMBER_T *call_num_ptr)
{
    ERR_MNDATAMAG_CODE_E     err_code = ERR_MNDATAMAG_IS_NULL;
    PHONEBOOK_ENTRY_T            entry;
    PHONEBOOK_INFO_T              phonebook_info = {0};
	
    BOOLEAN            status;
    PHONEBOOK_ENTRY_T           output_entry = {0};
	
    if (dual_sys >= MN_DUAL_SYS_MAX || PNULL == call_num_ptr)
    {
        return FALSE;
    }

    if (storage != PHONEBOOK_LND_STORAGE) 
    {
        return FALSE;
    }

    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, storage);

    if ( !phonebook_info.is_phonebook_exist)
    {
        SCI_TRACE_LOW("ATC: ATC_UpdateCallInfo warning ldn or mdn file not exist!");
        return FALSE;
    }
	
    SCI_MEMSET(&entry, 0, sizeof(entry));
		
    if(phonebook_info.used_record_num <= NV_MAX_LN_RECORDS)
    {
        //find the item by phone number     
        status = PHONEBOOK_FindPhonebookEntryByNumEx(dual_sys, PHONEBOOK_SIM_STORAGE, (uint8 *)(&call_num_ptr->party_num),call_num_ptr->num_len, &output_entry);
        if(status == MN_RETURN_SUCCESS)
        {
            //the call num is exist in SIM PB 
            if(output_entry.alpha_len > 0)
            {
                SCI_MEMCPY(entry.alpha, output_entry.alpha, output_entry.alpha_len);
                entry.alpha_len = output_entry.alpha_len ;
                SCI_TRACE_LOW("ATC: the alpha lenght is %d",entry.alpha_len);
            }
        }
	else
	{
	     SCI_TRACE_LOW("ATC: the called number is not exist in the SIM PB");
	}
		 
        entry.ton = call_num_ptr->number_type;
        entry.npi = call_num_ptr->number_plan;
        entry.entry_id = 0;
        entry.number_len = MIN(PHONEBOOK_MAX_DN_NUMBER_LEN, call_num_ptr->num_len);
		
        SCI_MEMSET(entry.number, 0, PHONEBOOK_MAX_DN_NUMBER_LEN);
        SCI_MEMCPY(entry.number, call_num_ptr->party_num, entry.number_len);
    

        SCI_TRACE_LOW("ATC: ATC_UpdateCallInfo call_num info is:%d,%d,%d",entry.number_len, entry.entry_id,entry.alpha_len);
        
        err_code = PHONEBOOK_UpdatePhonebookEntryEx(dual_sys, storage, &entry);

    }


    //Record the writen index
    //s_last_written_index[dual_sys][storage] = entry.entry_id;

    if (ERR_MNDATAMAG_NO_ERR != err_code)
    {    
	 SCI_TRACE_LOW("ATC: Update Call Info failed");
        return FALSE;
    }

    return TRUE;
}
#endif
