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
 
#include "base_trc.h"
#include "atc.h"
#include "atc_common.h"
#include "atc_phonebook.h"
#include "sig_code.h"
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/

 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
extern RSP_RESULT_STR       g_rsp_str;
extern const ATC_INFO_T     g_atc_info_table[];
#ifndef _MUX_ENABLE_
extern  MN_DUAL_SYS_E g_current_card_id;
#endif

static const char * atc_storage_str[PHONEBOOK_2G_MAX_STORAGE_NUM] =
                {"SM", "FD", "ME", "LD", "MC", "RC", "MSISDN", "EMERGENCY", "BDN", "SDN"};

static PHONEBOOK_STORAGE_E      s_default_storage[MN_SYS_NUMBER] = {PHONEBOOK_SIM_STORAGE};

/*record the last read index of phonebook*/
static uint16         s_last_read_index[PHONEBOOK_2G_MAX_STORAGE_NUM]={0}; 

/*record the last write index of phonebook*/
static uint16         s_last_written_index[PHONEBOOK_2G_MAX_STORAGE_NUM]={0}; 

/*record the PB operation flag in AT*/
static BOOLEAN        g_atc_phonebook_flag = FALSE;
#endif


extern uint32 SCI_TransDefault2Unicode(uint8 gsm_ch,uint8 *uni_ch);

/*****************************************************************************/
//  Description : This function justify whether the string can be convert to ira
//  Global resource dependence : 
//  Author:       Feng.shi
//  Note: for example "'0', '0', '3', 1';'0', '0', '3', 2'" can be convert to "1(0x31); 2(0x32)"
/*****************************************************************************/
BOOLEAN Is_Convert_2_IraStr(uint8 * input_out_str, uint16 max_len)
{
    uint16 i=0, len=0;
    uint8 temp_str[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2+1];
    SCI_MEMSET(&temp_str[0], 0, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2+1);
    while(input_out_str[len])
    {
        if(0 == (len %4))
        {
            if(input_out_str[len] != '0')
                return FALSE;
        }
        else if(1 == (len %4))
        {
            if(input_out_str[len] != '0')
                return FALSE;
        }
        else
        {
            temp_str[i] = input_out_str[len];
            i++;
        }
        len ++;
        if(len>max_len)
            return FALSE;
    }
    
    SCI_MEMSET(input_out_str, 0, len);
    ConvertHexToBin(&temp_str[0],i,input_out_str);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : This function initializes the static variables of phonebook
//  Global resource dependence : 
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_InitPhonebookStaticVar(MN_DUAL_SYS_E dual_sys)
{
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    int32 i;

    s_default_storage[dual_sys] = PHONEBOOK_SIM_STORAGE;
    
    for(i = 0;i < PHONEBOOK_2G_MAX_STORAGE_NUM;i++)
    {
        s_last_read_index[i]    = 0;
        s_last_written_index[i] = 0;
    }
#endif
}

/*****************************************************************************/
//  Description : This function convert ucs2 81 format to 80 format
//  Global resource dependence :
//  Author:       feng.shi
//  Note:        
/*****************************************************************************/
uint16 Convert81_to_80_UCS2(uint8 * input_ptr, uint8 *output_ptr)
{
    uint16                output_len = 0;
    uint16                               i = 0;
    uint16                               j = 0;
    BOOLEAN is_extend_default = FALSE;
    uint16                  base_char = 0;

    output_len = 2 * input_ptr[1];
    if(output_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
    {
        input_ptr[1] = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN / 2;
        output_len = 2 * input_ptr[1];
    }
    
    SCI_MEMSET((void*)output_ptr,0, output_len);
    
    j = (input_ptr[2] >> 1) & 0x7F;
    for(i = 0; i < input_ptr[1]; i++)
    {
        if((input_ptr[i+3] & 0x80) == 0x00)
        {
            // this code is default alphabet
            if(is_extend_default)  //this alphabet is extend default alphabet
            {
                input_ptr[i+3] += 0x80;
            }
            else
            {
                if(0x1b == input_ptr[i+3]) //the next alphabet is extend default alphabet
                {
                    is_extend_default = TRUE;
                    continue;
                }
            }

            SCI_TransDefault2Unicode (input_ptr[i+3],&(output_ptr[2*i]));
        }
        else
        {
            //output_ptr[2*i] = j;
            //output_ptr[2*i+1] = (input_ptr[i+3] & 0x7F) | ((input_ptr[2] << 7) & 0x80);
            base_char = (j << 8) + ((input_ptr[2] << 7) & 0x80);
            base_char += (input_ptr[i+3] & 0x7F);
            output_ptr[2*i] = (uint8)((base_char & 0xFF00) >> 8);
            output_ptr[2*i+1] = (uint8)(base_char & 0x00FF);
        }

        is_extend_default = FALSE;
    }
    return output_len;
}

/*****************************************************************************/
//  Description : This function convert ucs2 82 format to 80 format
//  Global resource dependence :
//  Author:       feng.shi
//  Note:        
/*****************************************************************************/
uint16 Convert82_to_80_UCS2(uint8 * input_ptr, uint8 *output_ptr)
{
    uint16                output_len = 0;
    uint16                               i = 0;
    BOOLEAN is_extend_default = FALSE;
    uint16                 base_char = 0;

    output_len = 2 * input_ptr[1];
    if(output_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
    {
        input_ptr[1] = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN / 2;
        output_len = 2 * input_ptr[1];
    }
    
    SCI_MEMSET((void*)output_ptr,0,output_len);
    
    for(i = 0; i < input_ptr[1]; i++)
    {
        if((input_ptr[i+4] & 0x80) == 0x00)
        {
            // this code is default alphabet
            if(is_extend_default)  //this alphabet is extend default alphabet
            {
                input_ptr[i+4] += 0x80;
            }
            else
            {
                if(0x1b == input_ptr[i+4]) //the next alphabet is extend default alphabet
                {
                    is_extend_default = TRUE;
                    continue;
                }
            }

            SCI_TransDefault2Unicode (input_ptr[i+4],&(output_ptr[2*i]));
        }
        else
        {
            base_char = (input_ptr[2] << 8) + input_ptr[3];
            base_char += (input_ptr[i+4] & 0x7F);
            output_ptr[2*i] = (uint8)((base_char & 0xFF00) >> 8);
            output_ptr[2*i+1] = (uint8)(base_char & 0x00FF);
            //  output_ptr[2*i] = input_ptr[2];
            //  output_ptr[2*i+1] = input_ptr[3] + (input_ptr[i+4] & 0x7F); 
        }

        is_extend_default = FALSE;
    }
    
    return output_len;
}

/*****************************************************************************/
//  Description : This function convert char to UCS2 bin
//  Global resource dependence :
//  Author:       feng.shi
//  Note:        
/*****************************************************************************/
uint16 ConvertCharToUcs2Bin(uint8 * out_ptr, uint8 *in_ptr, uint16 in_len)
{
    uint16 i=0;

    for(i=0; i<in_len; i++)
    {
        out_ptr[i*2] = 0;
        out_ptr[i*2+1] = in_ptr[i];
    }
    
    out_ptr[in_len * 2] = 0;
    return in_len * 2;
}

/*****************************************************************************/
//  Description : This function handles the AT+CPBS command 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli Lv
//  Note:         Modify by Liuhan
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBS )
{
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    int32                  i;
    uint8                  *storage_ptr = PNULL;
    uint32                 return_value = S_ATC_SUCCESS;
    PHONEBOOK_INFO_T       phonebook_info;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    // get the phonebook information     
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
        //set command AT+CPBS="<storage>"
        case ATC_CMD_TYPE_SET:
        {
            if(!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }
            
            if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_STRING)
            {
                return_value = ERR_OPERATION_NOT_ALLOWED;
                break;
            }
            
            SCI_ASSERT(PNULL != PARA1.str_ptr->str_ptr);/*assert verified*/
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
            for (i = 0; i < PHONEBOOK_2G_MAX_STORAGE_NUM; i++)
            {
                if (!strcmp((char*)storage_ptr, atc_storage_str[i]))
                {
                    s_default_storage[dual_sys] = i;
                    break;
                }
            }
            
            if(PHONEBOOK_2G_MAX_STORAGE_NUM ==i)
            {
                return_value = S_ATC_FAIL;
            }
            else
            {
                return_value = S_ATC_SUCCESS;
            }
            break;
        }
        
        // test command AT+CPBS? "SM", "FD", "ME", "LD", "MC", "RC"
        case ATC_CMD_TYPE_TEST: 
        {
            sprintf((char*)g_rsp_str, "%s: \"SM\",\"FD\",\"ME\",\"LD\",\"MC\",\"RC\""
                ,g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            return_value = S_ATC_SUCCESS;
            break;
        }

        // read command AT+CPBS?
        case ATC_CMD_TYPE_READ: 
        {
            sprintf((char*)g_rsp_str, "%s: \"%s\",%d,%d",g_atc_info_table[AT_CMD_CPBS].cmd_name->str_ptr, 
                atc_storage_str[s_default_storage[dual_sys]],    phonebook_info.used_record_num,
                (phonebook_info.max_record_num-phonebook_info.used_record_num));
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        default:
            //SCI_ASSERT(FALSE);
            break;
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
AT_CMD_FUNC( ATC_ProcessCPBR )
{
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    int32                index1;
    int32                index2;
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
#ifdef ATC_ORANGE_ENABLE
    ERR_MNDATAMAG_CODE_E              status;
#else
    BOOLEAN              status;
#endif
    uint8                tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1];
    uint8                tel_type;
    uint32               return_value = S_ATC_SUCCESS;
    char                 tel_alpha[(PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN<<1)+1] = {0};
    uint8                output_ptr[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN] = {0};
    uint16               output_len=0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof (entry));
        
    // get the phonebook information    
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
        //set command AT+CPBR=index1,index2
        case ATC_CMD_TYPE_SET:
        {
            // check the index 1
            if (!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }
            index1 = (uint16)PARA1.num;
            if ((index1 > phonebook_info.max_record_num)||(0 == index1))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            // check the index2
            index2 = PARAM2_FLAG ? (uint16)PARA2.num : index1;
            if ((index1 > index2) || (index2 > phonebook_info.max_record_num))
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            //Find and print the result
            //for (index1; index1<=index2; index1++)
            for ( ; index1<=index2; index1++)
            {
                SCI_MEMSET(&entry,0,sizeof(entry));
                entry.entry_id = (uint16)index1;            
                status = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);

                if (ERR_MNDATAMAG_NO_ERR == status)
                {
                    SCI_MEMSET(tel_str,'\0', sizeof (tel_str));

                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
                    tel_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                    SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                    /*if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8*)entry.alpha,entry.alpha_len,(uint8*)tel_alpha);
                    }*/
                    if(ATC_UCS2_80_CODE == entry.alpha[0])
                    {
                        //SCI_TRACE_LOW:"ATC:cpbr 80 type, %x, %x, %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_409_112_2_17_22_17_45_745,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);
                        if(entry.alpha_len >= 1)
                            ConvertBinToHex((uint8*)&entry.alpha[1],entry.alpha_len-1,(uint8*)&tel_alpha[0]);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                            //SCI_TRACE_LOW:"ATC: CONVERT TO IRA FAIL"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_415_112_2_17_22_17_45_746,(uint8*)"");
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                        
                    }
                    else if(ATC_UCS2_81_CODE == entry.alpha[0])
                    {
                        //SCI_TRACE_LOW:"ATC:cpbr 81 type, %x, %x, %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_423_112_2_17_22_17_45_747,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);

                        output_len = Convert81_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                        ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);

                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                    }
                    else if(ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        //SCI_TRACE_LOW:"ATC:cpbr 82 type, %x, %x, %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_438_112_2_17_22_17_45_748,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);

                        output_len = Convert82_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                        ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }

                    }
                    else
                    {
                        SCI_ASSERT(entry.alpha_len <= (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2)); /*assert verified check entry length*/
                        //SCI_TRACE_LOW:"ATC: cpbr str type, %x, %x, %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_454_112_2_17_22_17_45_749,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);
                        output_len = ATC_Default2Ascii((uint8*)&entry.alpha[0], (uint8*)&output_ptr[0], entry.alpha_len);
                        if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                        {   
                            if(output_len * 2 > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                            {
                                output_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN / 2;
                            }
                            #ifdef _ATC_SYNC_TOOL_
                            ConvertStr2Hex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                            #endif
                            //SCI_TRACE_LOW:"ATC: tel %x, %x, %x, %x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_463_112_2_17_22_17_45_750,(uint8*)"dddd", tel_alpha[0],tel_alpha[1],tel_alpha[2],tel_alpha[3]);
                        }
                        else 
                        {
                            SCI_MEMCPY(tel_alpha,(char*)&output_ptr[0],output_len);
                            if(strcmp((char*)atc_config_ptr->cscs, "IRA") != 0)
                            {
                                //SCI_TRACE_LOW:"ATC: unknown  cscs %s, use IRA"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_470_112_2_17_22_17_45_751,(uint8*)"s", (char*)atc_config_ptr->cscs);
                            }
                        }
                    }
                    sprintf((char*)g_rsp_str, "%s: %ld,\"%s\",%d,\"%s\"",
                        g_atc_info_table[AT_CMD_CPBR].cmd_name->str_ptr, 
                        index1,tel_str, tel_type, tel_alpha);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                    //Record the read index of phonebook
                    s_last_read_index[s_default_storage[dual_sys]] = index1; 
                }
            }
            
            return_value = S_ATC_SUCCESS;               
            break;
        }
            
        //Test command AT+CPBR=?
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (1-%d),%d,%d",
                g_atc_info_table[AT_CMD_CPBR].cmd_name->str_ptr, 
                phonebook_info.max_record_num,phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len, 
                phonebook_info.max_alpha_len);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        default:
            break;
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
AT_CMD_FUNC( ATC_ProcessCPBF )
{
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    MN_STRING_T           alpha; 
    PHONEBOOK_ENTRY_T     entry;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    BOOLEAN               present_flag;
    uint8                 tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1];
    char                  tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1];
    uint32                return_value = S_ATC_SUCCESS;
    uint8                 output_ptr[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN] = {0};
    uint16                output_len=0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;
    
    SCI_MEMSET(&entry, 0, sizeof (PHONEBOOK_ENTRY_T));   
    //SCI_MEMSET(tel_str,0,PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1);
    SCI_MEMSET(&output_ptr[0], 0, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
    //Get the information of phonebook  
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
        // the set command AT+CPBF=<findtext>
        case ATC_CMD_TYPE_SET:
        {
            if (!PARAM1_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
            {
                //ATC_TRACE:"ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_538_112_2_17_22_17_45_752,(uint8*)"");
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
            SCI_MEMSET(alpha.strbuf,'\0',MN_MAX_STRING_LEN);
            SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
            //string input is UCS2
            
            if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
            {
                //check the length of input alpha.
                if(PARA1.str_ptr->str_len > phonebook_info.max_alpha_len*2)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                if((PARA1.str_ptr->str_len)/2>PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN-1)
                    PARA1.str_ptr->str_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN-1; // because entry.alpha[0] is set 0x80;
                alpha.strbuf[0] = 0x80;
                ConvertAscUcs2IntoBin((char*)(PARA1.str_ptr->str_ptr), &alpha.strbuf[1], (PARA1.str_ptr->str_len)/2);
                alpha.strlen = (PARA1.str_ptr->str_len)/2 + 1;      
            
            }
            else
            {
                //check the length of input alpha.
                if(PARA1.str_ptr->str_len > phonebook_info.max_alpha_len)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }
                //SCI_ASSERT(PARA1.str_ptr->str_len <= MN_MAX_STRING_LEN);
                if(PARA1.str_ptr->str_len > MN_MAX_STRING_LEN/2-1)
                    PARA1.str_ptr->str_len = MN_MAX_STRING_LEN/2-1;
                alpha.strbuf[0]=0x80;
                alpha.strlen = ConvertCharToUcs2Bin(&alpha.strbuf[1], PARA1.str_ptr->str_ptr, PARA1.str_ptr->str_len)+1; //because add 0x80
                alpha.strbuf[alpha.strlen] = '\0';
            }
                
            //find by name.     
            present_flag = PHONEBOOK_FindPhonebookEntryByAlphaEx(dual_sys, s_default_storage[dual_sys], &alpha, &entry);

            if (present_flag==MN_RETURN_SUCCESS)
            {
                uint8   toa;
                toa = (uint8)((entry.ton << 4) + entry.npi +128);
                SCI_MEMSET(tel_str, '\0', PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1);
                TransformBcdToStr(entry.number, entry.number_len, tel_str);
                SCI_MEMSET(tel_alpha, '\0', sizeof (tel_alpha));
                /*
                if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                {
                    ConvertBinToHex((uint8*)entry.alpha,entry.alpha_len,(uint8*)tel_alpha);
                    tel_alpha[entry.alpha_len * 2] = '\0';
                }*/
                if(ATC_UCS2_80_CODE == entry.alpha[0])
                {
                    //SCI_TRACE_LOW:"__ 80 %x, %x, %x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_605_112_2_17_22_17_45_753,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);
                    if(entry.alpha_len >= 1)
                        ConvertBinToHex((uint8*)&entry.alpha[1],entry.alpha_len-1,(uint8*)&tel_alpha[0]);
                    if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }                   
                }
                else if(ATC_UCS2_81_CODE == entry.alpha[0])
                {
                    //SCI_TRACE_LOW:"__ 81 %x, %x, %x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_617_112_2_17_22_17_45_754,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);

                    output_len = Convert81_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                    ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                    if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }

                }
                else if(ATC_UCS2_82_CODE == entry.alpha[0])
                {
                    //SCI_TRACE_LOW:"__ 82 %x, %x, %x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_632_112_2_17_22_17_45_755,(uint8*)"ddd", entry.alpha[0],entry.alpha[1],entry.alpha[2]);
                    output_len = Convert82_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                    ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                    if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                }
                else
                {
                    SCI_ASSERT(entry.alpha_len <= PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2); /*assert verified check entry length*/
                    //SCI_MEMCPY(tel_alpha,(char*)entry.alpha,entry.alpha_len);
                    //tel_alpha[entry.alpha_len] = '\0';
                    output_len = ATC_Default2Ascii((uint8*)&entry.alpha[0], (uint8*)&output_ptr[0], entry.alpha_len);
                    if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                    {   
                        if(output_len * 2 > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                        {
                            output_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN / 2;
                        }
                        #ifdef _ATC_SYNC_TOOL_
                        ConvertStr2Hex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                        #endif
                        //SCI_TRACE_LOW:"ATC: tel %x, %x, %x, %x"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_656_112_2_17_22_17_45_756,(uint8*)"dddd", tel_alpha[0],tel_alpha[1],tel_alpha[2],tel_alpha[3]);
                    }
                    else 
                    {
                        SCI_MEMCPY(tel_alpha,(char*)&output_ptr[0],output_len);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") != 0)
                        {
                            //SCI_TRACE_LOW:"ATC: unknown  cscs %s, use IRA"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_663_112_2_17_22_17_45_757,(uint8*)"s", (char*)atc_config_ptr->cscs);
                        }
                    }
                }
                sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                    g_atc_info_table[AT_CMD_CPBF].cmd_name->str_ptr,
                       entry.entry_id, tel_str, toa, tel_alpha);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                s_last_read_index[s_default_storage[dual_sys]] = entry.entry_id; 
            }
            else
            {
                //SCI_TRACE_LOW("ATC:ConvertAscUcs2IntoBin");
                return_value = ERR_OPERATION_NOT_ALLOWED;
            }
            break;
        }

        // the test command AT+CPBF=? 
        case ATC_CMD_TYPE_TEST:
        {
            // return the <nlength> and <tlength>
            sprintf((char*)g_rsp_str, "%s: %d,%d", 
                g_atc_info_table[AT_CMD_CPBF].cmd_name->str_ptr,
                phonebook_info.max_record_num, phonebook_info.max_alpha_len);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        default:
            break;
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
AT_CMD_FUNC( ATC_ProcessCPBW )
{
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
    uint16                index = 0;
    uint8                 number_type;  
    uint8                 number_len;
    uint8                 *number_ptr;              // holds the telephone number
    uint32                return_value = S_ATC_SUCCESS;
    BOOLEAN               status;
    BOOLEAN               is_num_internation_type = FALSE;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    PHONEBOOK_ENTRY_T     entry; 
    char                  tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    ERR_MNDATAMAG_CODE_E  result = ERR_MNDATAMAG_NO_ERR;

    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof(entry));
        
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch (ATC_GET_CMD_TYPE)
    {
        // Set command AT+CPBW=<index>[, <number>[,<type>[,<text>]]]
        case ATC_CMD_TYPE_SET:
        {
            if (PARAM1_FLAG)
            {
                if(PARAM1_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                index = (uint16)PARA1.num;
                if ((index > phonebook_info.max_record_num)||(0 == index))
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }
            }
            // If only one parameter,delete the item of given index
            if (PARAM1_FLAG && !PARAM2_FLAG && !PARAM3_FLAG && !PARAM4_FLAG)
            {
                // delete the entry             
                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_DeletePhonebookEntryEx(dual_sys, s_default_storage[dual_sys], index))
                {
                    //if the type of storage is "sm" or "fd",return ok by message
                    if(AT_STORAGE_SM == s_default_storage[dual_sys] 
                        || AT_STORAGE_FD == s_default_storage[dual_sys])
                    {
                        g_atc_phonebook_flag = TRUE; // @Shijun.cui 2005-07-04 CR27405
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ADN_UPDATE_CNF,0xff,ATC_NO_DOMAIN);
                        return_value = S_ATC_FAIL;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }
                }
                else
                {
                    //if the type of storage is "sm" or "fd",return ok by message
                    if(AT_STORAGE_SM == s_default_storage[dual_sys] 
                        || AT_STORAGE_FD == s_default_storage[dual_sys])
                    {
                        g_atc_phonebook_flag = TRUE; // @Shijun.cui 2005-07-04 CR27405
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ADN_UPDATE_CNF,0xff,ATC_NO_DOMAIN);
                        return_value = S_ATC_DEFAULT_HANDLE;
                    }
                }
                break;
            }
            if (!PARAM1_FLAG && !PARAM2_FLAG)
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
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                if(PARA2.str_ptr->str_len > (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len) 
                    || 0 == PARA2.str_ptr->str_len)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }
                // Get the given phone number   
                number_ptr = PARA2.str_ptr->str_ptr;
                number_ptr[PARA2.str_ptr->str_len] = '\0';
                if (*number_ptr == '+')
                {
                   is_num_internation_type = TRUE;
                   number_ptr++;
                }
                number_len = (uint8)strlen((char*)number_ptr);
                status = TransfromStrToBcd(entry.number, &entry.number_len, number_ptr, number_len);
                if (!status)
                {
                    return_value = ERR_INVALID_CHARACTERS_IN_DIAL_STRING;
                    break;
                }
            }

            // check the number type 
            if (PARAM3_FLAG)
            {
                if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_NUMERIC)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                number_type = (uint8)PARA3.num;
                //when user input a phone number with a prefix '+',  force the number type as intertional type
                if (is_num_internation_type)
                {
                    number_type = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                }

                if (!(number_type == UNKNOWN_PLAN_UNKNOWN_TYPE ||
                      number_type == ISDN_TELE_PLAN_UNKNOWN_TYPE ||
                      number_type == ISDN_TELE_PLAN_INTERNATIONAL_TYPE ||
                      number_type == ISDN_TELE_PLAN_NATIONAL_TYPE))
                {
                    // Operation not supported
                      return_value = ERR_OPERATION_NOT_SUPPORTED;
                      break;
                }

            }
            else
            {
                //when user input a phone number with a prefix '+',  force the number type as intertional type
                if (is_num_internation_type)
                {
                    number_type = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                }
                else
                {
                    // set the default value
                    number_type = UNKNOWN_PLAN_UNKNOWN_TYPE;
                }
            }

            // check the alpha
            if (PARAM4_FLAG)
            {
                if(PARAM4_TYPE != ATC_CMD_PARAM_TYPE_STRING)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }

                //check the length of input
                if(PARA4.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2)
                {
                    return_value = ERR_INVALID_INDEX;
                    break;
                }
                SCI_MEMCPY(tel_alpha, (char*)PARA4.str_ptr->str_ptr,PARA4.str_ptr->str_len);
                //string input is UCS2
                //SCI_TRACE_LOW:"ATC:cpbw,tel_alpha=,%x,%x,%x,str_len,%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_875_112_2_17_22_17_46_758,(uint8*)"dddd",tel_alpha[0],tel_alpha[1],tel_alpha[2],(PARA4.str_ptr->str_len)/2);
                if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                {
                    if((PARA4.str_ptr->str_len)/2>PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN-1)
                        PARA4.str_ptr->str_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN-1; // because entry.alpha[0] is set 0x80;
                    if(Is_Convert_2_IraStr((uint8 *)tel_alpha, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN * 2))
                    {
                        entry.alpha_len = ATC_Ascii2default((uint8 *)tel_alpha, (uint8 *)entry.alpha, PARA4.str_ptr->str_len/4);
                        //SCI_TRACE_LOW:"ATC: %x %x %x %x %x len %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_883_112_2_17_22_17_46_759,(uint8*)"dddddd", entry.alpha[0], entry.alpha[1],entry.alpha[2],entry.alpha[3],entry.alpha[4],entry.alpha_len);
                    }
                    else
                    {
                        SCI_MEMCPY(tel_alpha, (char*)PARA4.str_ptr->str_ptr,PARA4.str_ptr->str_len);
                        entry.alpha[0] = 0x80;
                        #ifdef _ATC_SYNC_TOOL_
                        ConvertAscUcs2IntoBin(tel_alpha, &entry.alpha[1],(PARA4.str_ptr->str_len)/2);
                        #endif
                        entry.alpha_len  = PARA4.str_ptr->str_len/2 + 1;
                    }
                }
                //string input is ASCII.
                else
                {
                    if(PARA4.str_ptr->str_len > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                    {
                        return_value = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }

                    entry.alpha_len = ATC_Ascii2default((uint8 *)tel_alpha, (uint8 *)entry.alpha, PARA4.str_ptr->str_len);
                    
                    //SCI_TRACE_LOW:"ATC:cpbw,PARA4.str_ptr=%x,%x,%x"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_905_112_2_17_22_17_46_760,(uint8*)"ddd",*PARA4.str_ptr->str_ptr,*(PARA4.str_ptr->str_ptr+1),*(PARA4.str_ptr->str_ptr+2));

                    //SCI_TRACE_LOW("ATC:cpbw,entry.alpha=%x,%x,%x",*entry.alpha,*(entry.alpha+1),*(entry.alpha+2));
                }
            }

            // the IE of type of number
            entry.ton = (MN_NUMBER_TYPE_E)(number_type & 0x70)>>4;  //lint !e572 // get the bit 7,6,5 //@Shijun.cui 2005-09-14 CR31196
            entry.npi = (MN_NUMBER_PLAN_E)(number_type & 0x0f);  // get the bit 1,2,3,4

            // call the update functions 
            entry.entry_id = index;        
            result = PHONEBOOK_UpdatePhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);
            if(result != ERR_MNDATAMAG_NO_ERR)
            {
                //if the type of storage is "sm" or "fd",return ok by message
                if((AT_STORAGE_SM == s_default_storage[dual_sys] 
                    || AT_STORAGE_FD == s_default_storage[dual_sys]) &&
                    ERR_MNDATAMAG_RETURN_ERROR == result)
                {
                    //g_atc_phonebook_flag = TRUE; // @Shijun.cui 2005-07-04 CR27405
                    return_value = ERR_MEMORY_FULL;
                }
                else
                {
                    return_value = S_ATC_FAIL;
                }
            }
            else
            {
                //if the type of storage is "sm" or "fd",return ok by message
                if(AT_STORAGE_SM == s_default_storage[dual_sys] 
                    || AT_STORAGE_FD == s_default_storage[dual_sys])
                {
                    g_atc_phonebook_flag = TRUE; // @Shijun.cui 2005-07-04 CR27405
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ADN_UPDATE_CNF,0xff,ATC_NO_DOMAIN);
                    return_value = S_ATC_DEFAULT_HANDLE;
                }
                //Record the writen index
                s_last_written_index[s_default_storage[dual_sys]] = index; 
            }            
            break;
        }
            
            // test command AT+CPBW=?
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (1-%d),%d,(%d,%d,%d,%d),%d",
                g_atc_info_table[AT_CMD_CPBW].cmd_name->str_ptr,
                phonebook_info.max_record_num, (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len),
                UNKNOWN_PLAN_UNKNOWN_TYPE, ISDN_TELE_PLAN_UNKNOWN_TYPE, 
                ISDN_TELE_PLAN_INTERNATIONAL_TYPE, ISDN_TELE_PLAN_NATIONAL_TYPE,
                phonebook_info.max_alpha_len);
            // call the transfer function
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }

        default:
            break;
    }
    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}
 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
/*****************************************************************************/
//  Description : This function handles the APP_MN_ADN_UPDATE_CNF signal in 
//  deleting only one item.
//  Global resource dependence : 
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS ATC_ProcessDelItemCnf(
                ATC_CONFIG_T             *atc_config_ptr,
                APP_MN_ADN_UPDATE_CNF_T    *sig_ptr
                )
{
    SCI_TraceLow("ATC_ProcessDelItemCnf,g_atc_phonebook_flag is %d",g_atc_phonebook_flag);
    if (g_atc_phonebook_flag)
    {
        //delete successfully.
        if(sig_ptr->is_ok)
        {
        #ifdef _MUX_REDUCED_MODE_
        #ifdef _ATC_ONLY
            sprintf((char*)g_rsp_str, "%s: %d",
                    g_atc_info_table[AT_CMD_CPBW].cmd_name->str_ptr, sig_ptr->dn_id);
        
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
        #endif
        #endif
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK,0);
        }
        //fail to delete or update the item of phonebook
        else
        {
            ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
        }
        g_atc_phonebook_flag = FALSE;
        return S_ATC_SUCCESS;
    }
    return S_ATC_DEFAULT_HANDLE;
}
#endif /* _ULTRA_LOW_CODE_ */

/*****************************************************************************/
//  Description : This function handles the AT+CPBP command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCPBP )
{
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    uint8                 number_len;
    uint8                 *number_ptr = PNULL;// holds the telephone number
    PHONEBOOK_ENTRY_T     entry;
    PHONEBOOK_INFO_T      phonebook_info; // holds the phonebook information
    BOOLEAN               status;
    uint8                 tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1] = {0};
    char                 tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint32                return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof (entry));
    
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
        //set command AT+CPBP= <PhoneNumber>
        case ATC_CMD_TYPE_SET:
        {
            //check the index
            if (!PARAM1_FLAG || ATC_CMD_PARAM_TYPE_STRING != PARAM1_TYPE)
            {
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
            number_len = (uint8)strlen((char*)number_ptr);
            TransfromStrToBcd(entry.number, &entry.number_len, number_ptr, number_len);

            //find the item by phone number     
            status = PHONEBOOK_FindPhonebookEntryByNumEx(dual_sys, s_default_storage[dual_sys], entry.number,entry.number_len, &entry);

            if (status==MN_RETURN_SUCCESS)
            {
                uint8   toa;
                toa = (uint8)((entry.ton << 4) + entry.npi +128);
                TransformBcdToStr(entry.number, entry.number_len, tel_str);
                
                if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                {
                    ConvertBinToHex((uint8*)entry.alpha,entry.alpha_len,(uint8*)tel_alpha);
                }
                else
                {
                    SCI_MEMCPY(tel_alpha,(char*)entry.alpha,entry.alpha_len);
                }
                sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                    g_atc_info_table[AT_CMD_CPBP].cmd_name->str_ptr, 
                    entry.entry_id, tel_str, toa, tel_alpha);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
                s_last_read_index[s_default_storage[dual_sys]] = entry.entry_id;
                return_value = S_ATC_SUCCESS;
            }
            else
            {
                return_value = S_ATC_FAIL;
            }
            break;
        }
    
        // the test command AT+CPBP=? 
        case ATC_CMD_TYPE_TEST:
        {
            // return the <nlength> and <tlength>
            sprintf((char*)g_rsp_str, "%s: %d,%d",
                g_atc_info_table[AT_CMD_CPBP].cmd_name->str_ptr, 
                phonebook_info.max_record_num, (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len));
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        default:
            break;
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
AT_CMD_FUNC( ATC_ProcessCPBN )
{
#if !defined(_ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
    BOOLEAN              status;
    uint8                tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN*2 + 1] = {0};
    char                 tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint8                tel_type;
    int32                i = 0;
    uint16               stored_index = 0;
    uint32               return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof(entry));
    
    // get the phonebook information    
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);


    switch(ATC_GET_CMD_TYPE)
    {
        //Test command AT+CPBN=?
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (0-5)",
                g_atc_info_table[AT_CMD_CPBN].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        //Set command AT+CPBN=<mode>
        case ATC_CMD_TYPE_SET:
        {
            // check the index 
            if (!PARAM1_FLAG)
            {
                return_value = ERR_INVALID_INDEX;
                break;
            }

            if (phonebook_info.used_record_num == 0)
            {
                return_value = S_ATC_SUCCESS;
                break;
            }

            switch(PARA1.num)
            {
                //read the first the item
                case AT_CPBN_FIRST:
                {
                    entry.entry_id = 1;
                    for(i = 0;i < phonebook_info.max_record_num;i++)
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
                        s_last_read_index[s_default_storage[dual_sys]] = entry.entry_id;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }
                    break;
                }

                //read the last the item
                case AT_CPBN_LAST:
                {
                    entry.entry_id = phonebook_info.max_record_num ;
                    for(i = phonebook_info.max_record_num;i > 0;i--)
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
                        s_last_read_index[s_default_storage[dual_sys]] = entry.entry_id;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }

                    break;
                }
            
                    //Next valid item in alphabetical order 
                case AT_CPBN_NEXT:
                {
                    /*Find the previous item.If meeting the begin of phonebook,
                    turn to end of phonebook*/
                    stored_index = s_last_read_index[s_default_storage[dual_sys]];
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
                        &&(i < phonebook_info.max_record_num));
                    
                    //check if the item is found,or return error
                    if(i <= phonebook_info.max_record_num) //@Shijun.cui 2005-07-04 CR27123
                    {
                        s_last_read_index[s_default_storage[dual_sys]] = stored_index;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }
                    break;
                }
            
                    //Previous valid item in alphabetical order 
                case AT_CPBN_PREVIOUS:
                {
                    /*Find the previous item.If meeting the begin of phonebook,
                    turn to end of phonebook*/
                    stored_index = s_last_read_index[s_default_storage[dual_sys]];
                    do
                    {
                        if(stored_index >1 )
                        {
                            stored_index--;
                        }
                        else
                        {
                            stored_index = phonebook_info.max_record_num;
                        }
                        entry.entry_id=stored_index;
                        i++;
                    }
                    while((ERR_MNDATAMAG_NO_ERR != PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry))
                        &&(i < phonebook_info.max_record_num));
                    
                    //check if the item is found,or return error
                    if(i <= phonebook_info.max_record_num) //@Shijun.cui 2005-07-04 CR27123
                    {
                        s_last_read_index[s_default_storage[dual_sys]] = stored_index;
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }
                    break;
                }
            
                    //Last item read
                case AT_CPBN_LATESTREAD: 
                {
                    if(s_last_read_index[s_default_storage[dual_sys]] != 0 )
                    {
                        entry.entry_id = s_last_read_index[s_default_storage[dual_sys]];
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }   
                    break;
                }
            
                    //Last item written
                case AT_CPBN_LATESTWRITTEN: 
                {
                    if(s_last_written_index[s_default_storage[dual_sys]] != 0 )
                    {
                        entry.entry_id = s_last_written_index[s_default_storage[dual_sys]];
                    }
                    else
                    {
                        return_value = S_ATC_FAIL;
                    }   
                    break;
                }
            
                default:
                    return S_ATC_FAIL;
            }

            //Get and show the item 
            if( S_ATC_FAIL != return_value )
            {           
                status = PHONEBOOK_ReadPhonebookEntryEx(dual_sys, s_default_storage[dual_sys], &entry);

                if (ERR_MNDATAMAG_NO_ERR == status)
                {
                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
                    tel_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                    entry.alpha[entry.alpha_len] = '\0';
                    
                    if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8*)entry.alpha,entry.alpha_len,(uint8*)tel_alpha);
                    }
                    else
                    {
                        //SCI_ASSERT(entry.alpha_len <= PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2);
                        SCI_MEMCPY(tel_alpha,(char*)entry.alpha,entry.alpha_len);
                    }
                    sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d,\"%s\"",
                        g_atc_info_table[AT_CMD_CPBN].cmd_name->str_ptr, 
                        entry.entry_id,tel_str, tel_type, tel_alpha);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                    return_value = S_ATC_SUCCESS;
                }
                else
                {
                    return_value = S_ATC_FAIL;
                }
            }
            break;
        }
    
        default:
            break;
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
AT_CMD_FUNC( ATC_ProcessCNUM )
{
#if !defined( _ULTRA_LOW_CODE_)||defined(_ATC_UIX8910_ENABLE_)
    PHONEBOOK_INFO_T       phonebook_info;
    PHONEBOOK_ENTRY_T      entry;
    uint8                  tel_str[(PHONEBOOK_MAX_DN_NUMBER_LEN * 2) + 1] = {0};
    char                   tel_alpha[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2 + 1] = {0};
    uint8                  tel_type;
    uint32                 i;   
    uint32                 return_value = S_ATC_SUCCESS;
    
    uint8   output_ptr[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN] = {0};
    uint16  output_len=0;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof(entry));
    SCI_MEMSET(&output_ptr[0], 0, PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
    // get the phonebook information    
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, PHONEBOOK_MSISDN_STORAGE);

    switch(ATC_GET_CMD_TYPE)
    {
        //Test command AT+CNUM=?
        case ATC_CMD_TYPE_TEST:
        {
            //SCI_TRACE_LOW:"ATC: ATC_CMD_TYPE_TEST"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1383_112_2_17_22_17_47_761,(uint8*)"");
            break;
        }
    
        //Excute command AT+CNUM
        case ATC_CMD_TYPE_EXECUTE:
        {
            //Get the MSISDN from the MN
            for(i = 0;i < phonebook_info.max_record_num;i++)
            {
                entry.entry_id = i + 1;         
                if(ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_MSISDN_STORAGE, &entry))
                {
                    SCI_MEMSET(tel_str,'\0', sizeof (tel_str));

                    TransformBcdToStr(entry.number, entry.number_len, tel_str);
                    tel_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                    entry.alpha[entry.alpha_len] = '\0';
                    SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                    
                    /*
                    if(ATC_UCS2_80_CODE == entry.alpha[0] || ATC_UCS2_81_CODE == entry.alpha[0] || ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8*)entry.alpha,entry.alpha_len,(uint8*)tel_alpha);
                    }
                    else
                    {
                        SCI_MEMCPY(tel_alpha,(char*)entry.alpha,entry.alpha_len);
                    }*/
                    if(ATC_UCS2_80_CODE == entry.alpha[0])
                    {
                        ConvertBinToHex((uint8*)&entry.alpha[1],entry.alpha_len-1,(uint8*)tel_alpha);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                    }
                    else if(ATC_UCS2_81_CODE == entry.alpha[0])
                    {
                        output_len = Convert81_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                        ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)tel_alpha);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                    }
                    else if(ATC_UCS2_82_CODE == entry.alpha[0])
                    {
                        output_len = Convert82_to_80_UCS2(&entry.alpha[0], &output_ptr[0]);
                        ConvertBinToHex((uint8*)&output_ptr[0], output_len, (uint8*)tel_alpha);
                        if(strcmp((char*)atc_config_ptr->cscs, "IRA") == 0)
                        {
                            if(Is_Convert_2_IraStr((uint8 *)&tel_alpha[0], sizeof(tel_alpha)) == FALSE)
                            {
                                SCI_MEMSET(tel_alpha,'\0', sizeof (tel_alpha));
                            }
                        }
                    }
                    else
                    {
                        SCI_ASSERT(entry.alpha_len <= (PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN*2)); /*assert verified check entry length*/
                        output_len = ATC_Default2Ascii((uint8*)&entry.alpha[0], (uint8*)&output_ptr[0], entry.alpha_len);
                        if(strcmp((char*)atc_config_ptr->cscs, "UCS2") == 0)
                        {   
                            if(output_len * 2 > PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN)
                            {
                                output_len = PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN / 2;
                            }
                            ConvertStr2Hex((uint8*)&output_ptr[0], output_len, (uint8*)&tel_alpha[0]);
                            //SCI_TRACE_LOW:"ATC: tel %x, %x, %x, %x"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1458_112_2_17_22_17_47_762,(uint8*)"dddd", tel_alpha[0],tel_alpha[1],tel_alpha[2],tel_alpha[3]);
                        }
                        else 
                        {
                            SCI_MEMCPY(tel_alpha,(char*)&output_ptr[0],output_len);
                            if(strcmp((char*)atc_config_ptr->cscs, "IRA") != 0)
                            {
                                //SCI_TRACE_LOW:"ATC: unknown  cscs %s, use IRA"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1465_112_2_17_22_17_47_763,(uint8*)"s", (char*)atc_config_ptr->cscs);
                            }
                        }
                            
                    }   
                    sprintf((char*)g_rsp_str, "%s: \"%s\",\"%s\",%d",
                        g_atc_info_table[AT_CMD_CNUM].cmd_name->str_ptr,tel_alpha,
                        tel_str,tel_type);
                    ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
                }
                else
                {
                    //SCI_TRACE_LOW:"ATC: PHONEBOOK_ReadPhonebookEntry read error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1477_112_2_17_22_17_47_764,(uint8*)"");
                }
            }
            break;
        }
    
        //Read command AT+CNUM?
        case ATC_CMD_TYPE_READ:
        {
            //SCI_TRACE_LOW:"ATC: ATC_CMD_TYPE_READ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1486_112_2_17_22_17_47_765,(uint8*)"");
            break;
        }
    
        default:
            break;
    }
    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#if 0
/*****************************************************************************/
//  Description : This function handles the AT+CSVM command 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessCSVM )
{
#ifndef _ULTRA_LOW_CODE_
    PHONEBOOK_INFO_T     phonebook_info;
    PHONEBOOK_ENTRY_T    entry;
    BOOLEAN              status = FALSE;
    BOOLEAN              is_voicemail_enable = FALSE;
    uint8                number_type;  
    uint8                number_len;
    uint8                *number_ptr;      // holds the telephone number
    uint8                tel_str[PHONEBOOK_MAX_DN_NUMBER_LEN * 2 +1] = {0};

    uint32               return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    SCI_MEMSET(&entry, 0, sizeof(entry));
    // get the phonebook information    
    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);

    switch(ATC_GET_CMD_TYPE)
    {
        // test command AT+CSVM=?
        case ATC_CMD_TYPE_TEST:
        {
            //+CSVM: (0-1),20,(129,145) 
            sprintf((char*)g_rsp_str, "%s: (0-1),%d,(129,145)",
                g_atc_info_table[AT_CMD_CSVM].cmd_name->str_ptr,
                (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len));
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        // read command AT+CSVM?
        case ATC_CMD_TYPE_READ:
        {
            entry.entry_id = 1; //the voice mail number is only one.
            status = PHONEBOOK_ReadVoiceMailAddrEx(dual_sys, &entry);
            
            if (MN_RETURN_SUCCESS == status)
            {       
                is_voicemail_enable = PHONEBOOK_IsVoiceMailAddrEnableEx(dual_sys);
                TransformBcdToStr(entry.number, entry.number_len, tel_str);
                number_type = (uint8)((entry.ton << 4) + entry.npi + 128);
                entry.alpha[entry.alpha_len] = '\0';
                sprintf((char*)g_rsp_str, "%s: %d,\"%s\",%d",
                    g_atc_info_table[AT_CMD_CSVM].cmd_name->str_ptr,
                    is_voicemail_enable,tel_str,number_type);
                ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            }
            else
            {
                //SCI_TRACE_LOW("ATC: PHONEBOOK_ReadVoiceMailAddr IS ERROR!");
                return_value = S_ATC_FAIL;
            }
            break;
        }

            // set command AT+CSVM=<mode>[,<number>[,<type>]] 
        case ATC_CMD_TYPE_SET:
        {
            if(PARAM1_FLAG)
            {
                is_voicemail_enable = PARA1.num;
                // call the enable functions
                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_EnableVoiceMailAddrEx(dual_sys, is_voicemail_enable))
                {
                    //SCI_TRACE_LOW("ATC:PHONEBOOK_EnableVoiceMailAddr failed");
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
                //check the length of input
                if(PARA2.str_ptr->str_len > (phonebook_info.max_normal_num_len + phonebook_info.max_ext_num_len)
                    ||0 == PARA2.str_ptr->str_len)
                {
                    //SCI_TRACE_LOW:"ATC:AT+CSVM,error in length of input"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1586_112_2_17_22_17_47_766,(uint8*)"");
                    return_value = ERR_INVALID_INDEX;
                    break;
                }
                // Get the voice mail number
                number_ptr = PARA2.str_ptr->str_ptr;
                number_ptr[PARA2.str_ptr->str_len] = '\0';
                number_len = (uint8)strlen((char*)number_ptr);
                TransfromStrToBcd(entry.number, &entry.number_len, number_ptr, number_len);

                //hanle the para3
                number_type = (uint8)PARA3.num;

                if (!(number_type == UNKNOWN_PLAN_UNKNOWN_TYPE ||
                      number_type == ISDN_TELE_PLAN_UNKNOWN_TYPE ||
                      number_type == ISDN_TELE_PLAN_INTERNATIONAL_TYPE ||
                      number_type == ISDN_TELE_PLAN_NATIONAL_TYPE))
                {
                    // Operation not supported
                    //SCI_TRACE_LOW:"ATC:ERR_OPERATION_NOT_SUPPORTED"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1605_112_2_17_22_17_47_767,(uint8*)"");
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
                    //SCI_TRACE_LOW:"ATC:PHONEBOOK_UpdateVoiceMailAddr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1618_112_2_17_22_17_47_768,(uint8*)"");
                    return_value = S_ATC_FAIL;
                    break;
                }
                //update the enable flag
                if(ERR_MNDATAMAG_NO_ERR != PHONEBOOK_EnableVoiceMailAddrEx(dual_sys, is_voicemail_enable))
                {
                    //SCI_TRACE_LOW:"ATC:PHONEBOOK_EnableVoiceMailAddr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1625_112_2_17_22_17_47_769,(uint8*)"");
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
        }

        default:
            break;
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
AT_CMD_FUNC( ATC_ProcessSDCP )
{
#ifndef _ULTRA_LOW_CODE_
    PHONEBOOK_INFO_T     phonebook_info;
    uint8                *para_ptr = PNULL;
    int32                i; // loop index
    int32                j; 
    uint32               return_value = S_ATC_SUCCESS;
    MN_DUAL_SYS_E dual_sys = ATC_CURRENT_SIM_ID_MUX;

    switch(ATC_GET_CMD_TYPE)
    {
        // test command AT+SDCP=?
        case ATC_CMD_TYPE_TEST:
        {
            sprintf((char*)g_rsp_str, "%s: (LD,MC,RC)",
                g_atc_info_table[AT_CMD_SDCP].cmd_name->str_ptr);
            ATC_BuildInfoRsp(atc_config_ptr, (uint8*)g_rsp_str);
            break;
        }
    
        // read command AT+SDCP?
        case ATC_CMD_TYPE_READ:
        {
            //SCI_TRACE_LOW:"ATC: ATC_CMD_TYPE_READ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1676_112_2_17_22_17_47_770,(uint8*)"");
            break;
        }

        case ATC_CMD_TYPE_SET:
        {
            //check the index 
            if (!PARAM1_FLAG)
            {
                //SCI_TRACE_LOW:"ATC: ERR_INVALID_INDEX"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_PHONEBOOK_1685_112_2_17_22_17_47_771,(uint8*)"");
                return_value = ERR_INVALID_INDEX;
                break;
            }
            para_ptr = PARA1.str_ptr->str_ptr;
            para_ptr[PARA1.str_ptr->str_len] = '\0';

            // Compare the inputed storage string
            for (i=0; i < PHONEBOOK_2G_MAX_STORAGE_NUM; i++)
            {
                //if para_ptr=atc_storage_str,strcmp will return 0
                if(!strcmp((char*)para_ptr, atc_storage_str[i]))
                {
                    s_default_storage[dual_sys] = i; 
                                    
                    phonebook_info = PHONEBOOK_GetFileInfoEx(dual_sys, s_default_storage[dual_sys]);
                    //delete all the item
                    for (j = 1; j <= phonebook_info.max_record_num; j++)
                    {                   
                        PHONEBOOK_DeletePhonebookEntryEx(dual_sys, s_default_storage[dual_sys], j);
                        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,APP_MN_ADN_UPDATE_CNF,0xff,ATC_NO_DOMAIN);
                    }   
                    break;
                }
            }
            break;
        }

        default:
            //SCI_ASSERT(FALSE);
            break;
    }
    return return_value;
#else
    return S_ATC_FAIL;
#endif /* _ULTRA_LOW_CODE_ */
}

#endif
