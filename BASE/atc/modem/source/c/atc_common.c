/******************************************************************************
 ** File Name:      atc_common.c                                              *
 ** Author:         Steven.Yao                                                *
 ** Date:           05/20/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains function definitions that are of use to*
 **                 all other ATC modules.                                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/20/2002     Steven.Yao       Create.                                   *
 ******************************************************************************/

/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include <string.h>
#include "atc_common.h"
#include "atc_basic_cmd.h"
#include "env_atc_signal.h"
//#include "efs.h"
#include "dal_audio.h"
#ifndef WIN32
#include "sci_codec.h"
#endif
/*lint -save -e752 */
extern MN_DUAL_SYS_E       g_current_card_id;//lint !e752
/**---------------------------------------------------------------------------*
**                         Declare Variable                                  *
**---------------------------------------------------------------------------*/
#ifdef WIN32
#ifdef _DEBUG
static uint8   *g_atc_status_desc[] =
{
    "ATC process success",
    "Error! Can not alloc memory from system memory",
    "Error! Our private heap memory exhausted.",
    "Error! AT command type mismatch.",
    "Error! AT command parameter number mismatch.",
    "Error! AT command parameter is not in range.",
    "Error! AT command parameter type mismatch.",
    "Error! AT command line parse error.",
    "AT command has not been completed, pending.",
    "AT command is handled, result code is OK.",
    "AT command is handled, result code is CONNECT.",
    "AT command is handled, result code is RING.",
    "AT command is handled, result code is NO CARRIER.",
    "AT command is handled, result code is ERROR.",
    NULL, // this result code is empty
    "AT command is handled, result code is NO DIALTONE.",
    "AT command is handled, result code is BUSY.",
    "AT command is handled, result code is NO ANSWER."
};
#endif /* End of #ifdef _DEBUG */
#else

#ifdef _DEBUG
static uint8   *g_atc_status_desc[] =
{
    (uint8 *)"ATC process success",
    (uint8 *)"Error! Can not alloc memory from system memory",
    (uint8 *)"Error! Our private heap memory exhausted.",
    (uint8 *)"Error! AT command type mismatch.",
    (uint8 *)"Error! AT command parameter number mismatch.",
    (uint8 *)"Error! AT command parameter is not in range.",
    (uint8 *)"Error! AT command parameter type mismatch.",
    (uint8 *)"Error! AT command line parse error.",
    (uint8 *)"AT command has not been completed, pending.",
    (uint8 *)"AT command is handled, result code is OK.",
    (uint8 *)"AT command is handled, result code is CONNECT.",
    (uint8 *)"AT command is handled, result code is RING.",
    (uint8 *)"AT command is handled, result code is NO CARRIER.",
    (uint8 *)"AT command is handled, result code is ERROR.",
    (uint8 *)NULL, // this result code is empty
    (uint8 *)"AT command is handled, result code is NO DIALTONE.",
    (uint8 *)"AT command is handled, result code is BUSY.",
    (uint8 *)"AT command is handled, result code is NO ANSWER."
};


#endif /* End of #ifdef _DEBUG */

#endif /* End of #ifdef _RTOS */
static const  ATC_CPWD_STR_T      s_cpwd_str[AT_PWD_MAX] =
{
    {8, "PS"},
    {8, "SC"},
    {4, "AO"},
    {4, "OI"},
    {4, "OX"},
    {4, "AI"},
    {4, "IR"},
    {4, "AB"},
    {4, "AG"},
    {4, "AC"},
    {8, "P2"},
    {8, "FD"},
    {8, "PN"},
    {8, "PU"},
    {8, "PP"},
    {8, "PC"}
};
// for the AT+CLCK command
static const char    *s_clck_str[AT_CLCK_MAX] =
{
    "PS", "SC", "AO", "OI", "OX", "AI", "IR", "AB", "AG", "AC", "PN", "FD", "PU", "PP", "PC"
};

const char *g_atc_charset[ATC_CHSET_MAX_NUM] =
{
    "IRA",
    "GSM",
    "HEX",
    "UCS2"
};

#ifdef HUAWEI_6500_MODEM
const AT_CMD_LIST_T cmd_list[]=
{
    {"+CPOL", 4},
    {"+COPS", 4},
    {"+CCFC", 36},
    {"+CPUC", 3},
    {"+CUSD", 2},
    {"+CSCA", 0},
    {"+CMGS", 0},
    {"+CMGW", 0},
    {"+CSCB", 6},
    {"+CPBF", 0},
    {"+CPBW", 10},
    {"+CPBP", 0},
    {"+CGDCONT", 8},
    {"+SPDUMMYKPD", 2},
    {"+SGMR", 8},
    {"+EVTS", 2},
    {"+CMSS", 2},
    {"+CMGC", 0},
    {"+CSVM", 2},
    {"+SADM", 2}
    
};
#endif

extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];

/******************************************************************************/
//  Description : This function formats the clck response.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
void ATC_FormatCpwdStr(
    uint8        *pwd_ptr
)
{
    sprintf((char *)pwd_ptr, "+CPWD: (\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d), (\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d),(\"%s\", %d)", \
            s_cpwd_str[AT_PWD_PS].fac_ptr, s_cpwd_str[AT_PWD_PS].number, \
            s_cpwd_str[AT_PWD_SC].fac_ptr, s_cpwd_str[AT_PWD_SC].number, \
            s_cpwd_str[AT_PWD_AO].fac_ptr, s_cpwd_str[AT_PWD_AO].number, \
            s_cpwd_str[AT_PWD_OI].fac_ptr, s_cpwd_str[AT_PWD_OI].number, \
            s_cpwd_str[AT_PWD_OX].fac_ptr, s_cpwd_str[AT_PWD_OX].number, \
            s_cpwd_str[AT_PWD_AI].fac_ptr, s_cpwd_str[AT_PWD_AI].number, \
            s_cpwd_str[AT_PWD_IR].fac_ptr, s_cpwd_str[AT_PWD_IR].number, \
            s_cpwd_str[AT_PWD_AB].fac_ptr, s_cpwd_str[AT_PWD_AB].number, \
            s_cpwd_str[AT_PWD_AG].fac_ptr, s_cpwd_str[AT_PWD_AG].number, \
            s_cpwd_str[AT_PWD_AC].fac_ptr, s_cpwd_str[AT_PWD_AC].number, \
            s_cpwd_str[AT_PWD_P2].fac_ptr, s_cpwd_str[AT_PWD_P2].number, \
            s_cpwd_str[AT_PWD_FD].fac_ptr, s_cpwd_str[AT_PWD_FD].number, \
            s_cpwd_str[AT_PWD_PN].fac_ptr, s_cpwd_str[AT_PWD_PN].number, \
            s_cpwd_str[AT_PWD_PU].fac_ptr, s_cpwd_str[AT_PWD_PU].number, \
            s_cpwd_str[AT_PWD_PP].fac_ptr, s_cpwd_str[AT_PWD_PP].number, \
            s_cpwd_str[AT_PWD_PC].fac_ptr, s_cpwd_str[AT_PWD_PC].number);
}

/******************************************************************************/
//  Description : This function formats the clck read command response string.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
void ATC_FormatClckReadRsp(
    uint8    *rsp_str,
    uint8    sc_state
)
{
    ATC_TRACE_LOW("The state is %d", sc_state);

    sprintf((char *)rsp_str, "+CLCK: (\"%s\", 0),(\"%s\", %d),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0)", \
            s_clck_str[AT_CLCK_PS], s_clck_str[AT_CLCK_SC], sc_state, \
            s_clck_str[AT_CLCK_AO], s_clck_str[AT_CLCK_OI], \
            s_clck_str[AT_CLCK_OX], s_clck_str[AT_CLCK_AI], \
            s_clck_str[AT_CLCK_IR], s_clck_str[AT_CLCK_AB], \
            s_clck_str[AT_CLCK_AG], s_clck_str[AT_CLCK_AC], \
            s_clck_str[AT_CLCK_PN], s_clck_str[AT_CLCK_FD], \
            s_clck_str[AT_CLCK_PU], s_clck_str[AT_CLCK_PP], \
            s_clck_str[AT_CLCK_PC]);
}

/******************************************************************************/
//  Description : This function formats the clck test command response string.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
void ATC_FormatClckTestRsp(
    uint8      *rsp_str
)
{
    sprintf((char *)rsp_str, "+CLCK: (\"%s\",\"%s\",\"%s\",\"%s\", \"%s\",\"%s\",\"%s\", \"%s\",\"%s\", \"%s\",\"%s\", \"%s\",\"%s\",\"%s\",\"%s\")", \
            s_clck_str[AT_CLCK_PS], s_clck_str[AT_CLCK_SC], \
            s_clck_str[AT_CLCK_AO], s_clck_str[AT_CLCK_OI], \
            s_clck_str[AT_CLCK_OX], s_clck_str[AT_CLCK_AI], \
            s_clck_str[AT_CLCK_IR], s_clck_str[AT_CLCK_AB], \
            s_clck_str[AT_CLCK_AG], s_clck_str[AT_CLCK_AC], \
            s_clck_str[AT_CLCK_PN], s_clck_str[AT_CLCK_FD], \
            s_clck_str[AT_CLCK_PU], s_clck_str[AT_CLCK_PP], \
            s_clck_str[AT_CLCK_PC]);
}

#ifdef WIN32
void ATCC_trace(const char *fmt, ...)
{
    char    trace_buf[1024];

    va_list     a;
    va_start(a, fmt);
    vsprintf(trace_buf, fmt, a);
    OutputDebugString(trace_buf);
    va_end(a);
}

void ATCC_WriteCmdRes(char *buf, uint len)
{
    buf[len] = '\0';

    printf("%s\n", buf);
}
#endif /* End of #ifdef WIN32 */

void ATCC_LogCurATCInfo(
    ATC_CUR_INFO_LIST_T *atc_info_ptr
)
{
    ATC_CUR_PARAM_INFO_LIST_T *param_info_ptr = NULL;
    uint        index = 0;

    SCI_ASSERT(NULL != atc_info_ptr);/*assert verified: check null pointer*/

    ATC_TRACE("Log ATC infomation:\n");

    while(NULL != atc_info_ptr)
    {
        ATC_TRACE("AT Command Index : %d\n", atc_info_ptr->cmd_index);
        ATC_TRACE("AT Command Type :");

        if((ATC_CMD_TYPE_BASIC & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" BASIC");
        }

        if((ATC_CMD_TYPE_EXTEND & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" EXTEND");
        }

        if((ATC_CMD_TYPE_EXECUTE & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" EXECUTE");
        }

        if((ATC_CMD_TYPE_SET & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" SET");
        }

        if((ATC_CMD_TYPE_READ & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" READ");
        }

        if((ATC_CMD_TYPE_TEST & atc_info_ptr->cmd_type))
        {
            ATC_TRACE_LOW(" TEST");
        }

        ATC_TRACE_LOW("\n");

        ATC_TRACE("AT Command Parameter Number : %d\n", atc_info_ptr->cur_param_num);

        param_info_ptr = atc_info_ptr->cur_param_list_ptr;

        for(index = 0; index < atc_info_ptr->cur_param_num; index++)
        {
            SCI_ASSERT(NULL != param_info_ptr);/*assert verified: check null pointer*/

            switch(param_info_ptr->param_type)
            {
                case ATC_CMD_PARAM_TYPE_NUMERIC:
                    ATC_TRACE("AT Command Parameter %d : %d\n",
                              index,
                              param_info_ptr->param_info.num);
                    break;

                case ATC_CMD_PARAM_TYPE_STRING:
                    ATC_TRACE("AT Command Parameter %d : %s\n",
                              index,
                              param_info_ptr->param_info.str_ptr->str_ptr);
                    break;

                case ATC_CMD_PARAM_TYPE_DEFAULT:
                    ATC_TRACE("AT Command Parameter %d : Default\n", index);
                    break;

                default:
                    break;
            }

            param_info_ptr = param_info_ptr->next;
        } // End of for loop

        atc_info_ptr = atc_info_ptr->next;
    } // End of loop while ( NULL != atc_info_ptr )
}


/******************************************************************************/
//  Description : This function displays the status description
//  Global resource dependence : g_atc_status_desc
//  Author : Steven.Yao
//  Note : None
/******************************************************************************/
void ATCS_TraceStatusDesc(
    ATC_STATUS status // the status of ATC processing
)
{
    ATC_TRACE("%s\n", g_atc_status_desc[status & 0x00FF]);
}

/*****************************************************************************/
//  Description : This function transform the BCD number to telephone string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void TransformBcdToStr(
    uint8                 *bcd_ptr, // in the BCD array
    int32                 length,      // in the length of BCD array
    uint8                 *tel_str     // out the telephone number string
)
{
    ATC_TransformBcdToStr(bcd_ptr, length, tel_str, NULL);
}

/*****************************************************************************/
//  Description : This function transform the BCD number to telephone string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
void ATC_TransformBcdToStr(
    uint8       *bcd_ptr,         // in the BCD array
    int32        bcd_length,      // in the length of BCD array
    uint8        *tel_str,        // out the telephone number string
    uint16       *tel_str_len_ptr  // out, the telephone number length
)
{
    int32        i, j;
    uint8        temp;
    uint16       str_len = 0;

    SCI_ASSERT(PNULL != tel_str);/*assert verified: check null pointer*/
    
    ATC_TRACE_LOW("ATC: the BCD CODE length is %d", bcd_length);
    
    if(MN_MAX_ADDR_BCD_LEN < bcd_length)
    {
        bcd_length = MN_MAX_ADDR_BCD_LEN;
    }

    

    for(i = 0; i < bcd_length; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if(0 == j)
            {
                temp = (uint8)(*bcd_ptr & 0x0f);
            }
            else
            {
                temp = ((*bcd_ptr & 0xf0) >> 4);
            }

            str_len++;

            switch(temp)
            {
                case 10:
                    *tel_str++ = '*';
                    break;
                case 11:
                    *tel_str++ = '#';
                    break;
                case 12:
                    *tel_str++ = 'a';
                    break;
                case 13:
                    *tel_str++ = 'b';
                    break;
                case 14:
                    *tel_str++ = 'c';
                    break;
                case 15:
                    /*current character is '\0'*/
                    str_len--;
                    break;
                default:
                    *tel_str++ = (uint8)(temp + '0');
                    break;
            }
        }

        bcd_ptr++;
    }

    if(NULL != tel_str_len_ptr)
    {
        *tel_str_len_ptr = str_len;
    }

    return;
}

/*****************************************************************************/
//  Description : This function transform telephone string to the BCD number.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN TransfromStrToBcd(
    uint8             *bcd_ptr,  // out: called BCD number array
    uint8             *length_ptr, // out: the BCD array length
    uint8              *tel_str,     // in:  telephone number string
    int32             str_length   // in: telephone number string length
)
{
    int32         i = 0;

    SCI_ASSERT(PNULL != length_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != tel_str);/*assert verified: check null pointer*/

    *length_ptr = (uint8)((str_length + 1) / 2);

    for(i = 0; i < *length_ptr; i++)
    {
        switch(*tel_str)
        {
            case '*':
                bcd_ptr[i] = 0x0A;
                break;
            case '#':
                bcd_ptr[i] = 0x0B;
                break;
            case 'a':
            case 'A':
                bcd_ptr[i] = 0x0C;
                break;
            case 'b':
            case 'B':
                bcd_ptr[i] = 0x0D;
                break;
            case 'c':
            case 'C':
                bcd_ptr[i] = 0x0E;
                break;
            default:

                if(*tel_str >= '0' && *tel_str <= '9')
                {
                    bcd_ptr[i] = (uint8)(*tel_str - '0');
                }
                else
                {
                    return FALSE;
                }

                break;
        }

        tel_str++;

        switch(*tel_str)
        {
            case '*':
                bcd_ptr[i] += 0xA0;
                break;
            case '#':
                bcd_ptr[i] += 0xB0;
                break;
            case 'a':
            case 'A':
                bcd_ptr[i] += 0xC0;
                break;
            case 'b':
            case 'B':
                bcd_ptr[i] += 0xD0;
                break;
            case 'c':
            case 'C':
                bcd_ptr[i] += 0xE0;
                break;
            case '\0':
                bcd_ptr[i] += 0xF0;
                break;
            default:

                if((*tel_str >= '0') &&
                        (*tel_str <= '9'))
                {
                    bcd_ptr[i] = (uint8)(bcd_ptr[i] + ((*tel_str - '0') << 4));
                }
                else
                {
                    return FALSE;
                }

                break;
        }

        tel_str++;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function transform imei string to the BCD number.
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
BOOLEAN   TransfromImeiStrToBcd(    // FALSE: There is invalid char
    uint8        *imei_ptr,     // in: the imei format string
    uint8        max_imei_len, // in
    uint8        max_dest_len,       // in: the length of imei string
    uint8        *dest_ptr      // out: pointer to the bcd format string
)
{
    int32         i = 0;
    uint8        ch=0;
	
    SCI_ASSERT(PNULL != imei_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != dest_ptr);/*assert verified: check null pointer*/
	
    //handle the first value
     ch = imei_ptr[0];
	
    if (ch >= '0' && ch <= '9')
    {
        *dest_ptr =(uint8)((ch - '0') << 4);
	 *dest_ptr |= 0x0A;
    }
    else
    {
        return FALSE;
    }

    dest_ptr++;

	 
    for(i = 1; (i < max_dest_len)&&(2*i < max_imei_len); i++)
    {
    
	//set the low 4 bits
        ch = imei_ptr[2*i-1];
	if(ch>= '0' && ch<= '9')
        {
              *dest_ptr = (uint8)(ch- '0');
        }
        else
        {
             return FALSE;
         }

	// set the high 4 bits
        ch = imei_ptr[2*i];
        if((ch >= '0') &&(ch <= '9'))
        {
             *dest_ptr |= (uint8)((ch - '0')<<4);
         }
         else
         {
             return FALSE;
         }
         dest_ptr++;
		 
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : This function convert the bcd code to digital string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void ConvertBcdToDigitalStr(
    uint8         length,
    uint8         *bcd_ptr,      // in: the bcd code string
    uint8         *digital_ptr   // out: the digital string
)
{
    int32         i;
    uint8         temp;

    // get the first digital
    temp = ((*bcd_ptr >> 4) & 0x0f);

    if(temp >= 0x0a)
    {
        *digital_ptr = (temp - 0x0a) + 'A';
    }
    else
    {
        *digital_ptr = temp + '0';
    }

    bcd_ptr++;
    digital_ptr++;


    for(i = 0; i < (length - 1); i++)
    {
        temp = *bcd_ptr;
        // get the low 4 bits
        temp &= 0x0f;

        // A -- F
        if(temp >= 0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }

        digital_ptr++;

        temp = *bcd_ptr;
        // get the high 4 bits
        temp = (temp & 0xf0) >> 4;

        if((temp == 0x0f) && (i == (length - 1)))
        {
            *digital_ptr = '\0';
            return;
        }
        // A -- F
        else if(temp >= 0x0a)
        {
            *digital_ptr = (temp - 0x0a) + 'A';
        }
        else
        {
            // 1 -- 9
            *digital_ptr = temp + '0';
        }

        digital_ptr++;
        bcd_ptr++;
    }

    *digital_ptr = '\0';
}

/*****************************************************************************/
//  Description : This function convert telephone number string to called number
//                structure.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertTeleStrToStruct(
    MN_CALLED_NUMBER_T  *called_number_ptr, // pointer to the called number structure
    uint8               *tel_str,     // in:  telephone number string
    int32               str_length   // in: telephone number string length
)
{
    uint8        bcd_len;
    uint8        *temp_ptr;
    BOOLEAN      temp_flag;

    temp_ptr = tel_str;

    if(*temp_ptr == '+')
    {
        called_number_ptr->number_type = MN_NUM_TYPE_INTERNATIONAL;
        temp_ptr++;
        str_length -= 1;
    }
    else
    {
        called_number_ptr->number_type = MN_NUM_TYPE_UNKNOW;
    }

    called_number_ptr->number_plan = MN_NUM_PLAN_ISDN_TELE;

    temp_flag = TransfromStrToBcd(called_number_ptr->party_num,
                                  &bcd_len, temp_ptr, str_length);
    called_number_ptr->num_len = bcd_len;
    return temp_flag;
}

/*****************************************************************************/
//  Description : This function is used to get the type commands(AT+CPIN,AT+CPWD,
//                AT+CLCK)
//
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void ATC_GetCmdType(
    uint8      *param_ptr,
    uint16        param_len,
    uint8      *cmd_type_ptr
)
{
    switch(*param_ptr)
    {
        case '?':

            if(param_len == 1)
            {
                *cmd_type_ptr = ATC_CMD_TYPE_READ;
            }
            else
            {
                // invalid type of command
                *cmd_type_ptr = 0xff;
            }

            break;

        case '=':

            if(2 == param_len && param_ptr[1] == '?')
            {
                *cmd_type_ptr = ATC_CMD_TYPE_TEST;
            }
            else
            {
                *cmd_type_ptr = ATC_CMD_TYPE_SET;
            }

            break;
        default:
            *cmd_type_ptr = 0xff;
            break;
    }
}
/*****************************************************************************/
//  Description : This function is used to get the type commands(AT+ECIND)
//
//  Global resource dependence : none
//  Author:       Bin LI
//  Note:
/*****************************************************************************/
void ATC_GetUiccInfo(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T *atc_config_ptr,
    uint8        *sim_status_ptr,
    uint8        *pin_status_ptr,
    MN_CARD_TYPE_E        *card_type_ptr
)
{
    //SIMCHECK_GET_SIM_TYPE_CNF_T sim_type_cnf = {MN_SIM_TYPE_FAILED, MN_CARD_TYPE_NUM};
    MN_SIM_TYPE_CAUSE_E sim_err_cause = MN_SIM_TYPE_FAILED;
    MNSIM_ERROR_E    sim_err_code = MNSIM_ERROR_NULL;
    MNSIM_PIN_STATUS_E pin1_status = MNSIM_PIN_READY;
    MNSIM_PIN_STATUS_E pin2_status = MNSIM_PIN_READY;

    SCI_ASSERT((NULL != sim_status_ptr) && (NULL != pin_status_ptr) && (NULL != card_type_ptr));/*assert verified: check null pointer*/
    
    *card_type_ptr = atc_config_ptr->sim_type[dual_sys];

    if (MNSIM_GetSimStatusEx(dual_sys, &sim_err_code))
    {
        sim_err_cause = MN_SIM_TYPE_SUCCESS;
        *sim_status_ptr = 0;
    }
    else
    {
        switch(sim_err_code)
        {
            case MNSIM_ERROR_REJECT_BY_NET:
            case MNSIM_ERROR_NULL:
            case MNSIM_ERROR_PIN1_UNINITIALIZED:
                sim_err_cause = MN_SIM_TYPE_SUCCESS;
                *sim_status_ptr = 0;
                break;

            case MNSIM_ERROR_POWER_OFF:
            case MNSIM_ERROR_INSERTED_ERROR:
            case MNSIM_ERROR_REMOVED:
                sim_err_cause = MN_SIM_TYPE_NO_SIM;
                *sim_status_ptr = 1;
                *pin_status_ptr = 28;
                break;

            default :
                sim_err_cause = MN_SIM_TYPE_NOT_READY;
                *sim_status_ptr = 2;
                break;
        }
    }
    pin1_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN1);
    pin2_status = MNSIM_GetPinStatusEx(dual_sys, MNSIM_PIN2);

    ATC_TRACE_LOW(" ATC: ATC_GetUiccInfo pin1_status %d, pin2_status %d", pin1_status, pin2_status);

    switch(pin1_status)
    {
        case MNSIM_PIN_READY:
            *pin_status_ptr = 0;

            switch(pin2_status)
            {
                case MNSIM_PIN_READY:
                    break;
                case MNSIM_PIN_PIN:
                    *pin_status_ptr = 2;
                    break;
                case MNSIM_PIN_PUK:
                    *pin_status_ptr = 4;
                    break;
                case MNSIM_PIN_BLOCKED:
                    *pin_status_ptr = 23;
                    break;
                default :
                    break;
            }

            break;

        case MNSIM_PIN_PIN:
            *pin_status_ptr = 1;

            if(MN_SIM_TYPE_NO_SIM != sim_err_cause)
            {
                *sim_status_ptr = 2;
            }

            break;

        case MNSIM_PIN_PUK:
            *pin_status_ptr = 3;

            if(MN_SIM_TYPE_NO_SIM != sim_err_cause)
            {
                *sim_status_ptr = 2;
            }

            break;

        case MNSIM_PIN_BLOCKED:
            *pin_status_ptr = 22;

            if(MN_SIM_TYPE_NO_SIM != sim_err_cause)
            {
                *sim_status_ptr = 2;
            }

            break;

        default :
            break;
    }

    if(1 == *sim_status_ptr)
    {
        *pin_status_ptr = 0;
        *card_type_ptr = MN_CARD_TYPE_NUM;
    }

    ATC_TRACE_LOW(" ATC: ATC_GetUiccInfo sim status  %d, pin_status %d, card_type %d, dual_sys %d", *sim_status_ptr, *pin_status_ptr, *card_type_ptr, dual_sys);
    return;
}
/*****************************************************************************/
//  Description : This function is used to get the parameters (AT+CPIN,AT+CPWD,
//                AT+CLCK)
//
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetParams(
    uint8            *in_ptr,
    uint8             in_len,
    ATC_CPWD_PARAM_T *param_ptr,
    uint8      param_max_count
)
{
    int32            i;
    int32            j = 0;
    uint16           k = 0;
    BOOLEAN          flag = TRUE;
    uint8       count = param_max_count;

    if(NULL == in_ptr || NULL == param_ptr || param_max_count < 1)
    {
        return FALSE;
    }

    for(i = 0; i < in_len; i++)
    {
        if(in_ptr[i] == ',')
        {
            k++;
            count--;
            j = 0;

            if(0 == count)
            {
                //overfull params
                flag = FALSE;
                break;
            }

#if 0

            if(k > 3)
            {
                flag = FALSE;
                break;
            }

#endif
        }
        else
        {
            if(j < ATC_PWD_MAX_LEN)
            {
                if(in_ptr[i] != '"')
                {
                    param_ptr[k].param[j++] = in_ptr[i];
                    param_ptr[k].param_len += 1;
                }
            }
            else if(in_ptr[i] != '"')
            {
                flag = FALSE;
                break;
            }
        }
    }

    ATC_TRACE_LOW("ATC: get the password parameters %d", flag);
    return flag;

}

/*****************************************************************************/
//  Description : This function is used to get the parameters of AT set Command
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetCommandParams(
    uint8            *in_ptr,                       //in: param string
    uint16              in_len,                     //in: param length
    uint8       param_max_count,                   //in: the max params num of the cmd
    ATC_CMD_PARAM_T *cmd_info_ptr,          //out: cmd info
    uint8       *param_count,               //out:param count
    uint16          *offset_len_ptr,                //out: parse offset length for other AT cmd
    BOOLEAN     *other_cmd_flag_ptr         //out: there are other AT cmds included or not
)
{
    uint16                i;
    uint16                j = 0;                        //param data count
    uint8                  param_index = 0;            //THE PARAMs COUNT
    uint16                data_index = 0;            //THE PARAM data COUNT
    BOOLEAN           is_param_start_f = TRUE;    //the first character flag of the param
    BOOLEAN           is_string    = FALSE;
    BOOLEAN           other_cmd_flag = FALSE;
    BOOLEAN           result = TRUE;
    uint16                offset = 0;
    BOOLEAN           string_checked = FALSE;


    if(NULL == in_ptr || NULL == cmd_info_ptr  || param_max_count < 1)
    {
        return FALSE;
    }

    for(i = 0; i < param_max_count; i++)
    {
        SCI_MEMSET(&cmd_info_ptr[i], 0, sizeof(ATC_CMD_PARAM_T));
    }

    ATC_TRACE_LOW("ATC:ATC_GetCommandParams,parsing,<data:> %s,<len:> %d", in_ptr, in_len);

    //GET PARAMS

    /*ATC_CMD_PARAM_T store the params of AT Cmd
       For example: AT+CMD1="1234"
       param_len will be 6
       param_data_len wll be 4
       param will be 1234
    */
    for(i = 0; i < in_len;)
    {
        //offset++;
        if(is_param_start_f)
        {
            is_param_start_f = FALSE;

            //if is string param, filter '"'
            if('"' == in_ptr[i])
            {
                is_string = TRUE;
            }
        }

        if(is_string)
        {
            uint16       start_p;
            uint16      str_count = 0;

            start_p = i + 1;

            for(j = start_p; j < in_len; j++)
            {
                if(('"' == in_ptr[j]) &&
                        (((j + 1) >= in_len) || (',' == in_ptr[j + 1])  || (';' == in_ptr[j + 1]))
                  )
                {
                    //this is a string
                    string_checked = TRUE;
                    str_count = j - start_p;
                    break;
                }
            }

            if(string_checked)
            {
                if(str_count >= CMD_PARAM_MAX_LEN)
                {
                    ATC_TRACE_LOW("ATC: invalid string param, overstep param max length:%d", CMD_PARAM_MAX_LEN);
                    result = FALSE;
                    break;
                }
                else
                {
                    SCI_MEMCPY(cmd_info_ptr[param_index].param, (in_ptr + start_p), str_count);
                    cmd_info_ptr[param_index].param_data_len = str_count;
                    cmd_info_ptr[param_index].param_len = str_count + 2;
                    is_string = FALSE;
                    i += cmd_info_ptr[param_index].param_len;
                }
            }
            else
            {
                ATC_TRACE_LOW("ATC: invalid string param");
                result = FALSE;
                break;
            }
        }
        else
        {
            if(in_ptr[i] == ';')
            {
                //another AT Command tag
                other_cmd_flag = TRUE;
                i++;        //offset character ';'
                break;
            }
            else if(in_ptr[i] == ',')
            {
                //another param tag
                is_param_start_f = TRUE;
                is_string = FALSE;
                param_index++;
                data_index = 0;

                if(param_max_count == param_index)
                {
                    ATC_TRACE_LOW("ATC: param overflow, max count of params is %d", param_max_count);
                    result = FALSE;
                    break;
                }
            }
            else
            {
                cmd_info_ptr[param_index].param_len++;

                if('"' == in_ptr[i])
                {
                    ATC_TRACE_LOW("ATC: invalid param, quotation mark wrong position, return position:%d", i);
                    result = FALSE;
                    break;
                }

                if(data_index >= CMD_PARAM_MAX_LEN)
                {
                    ATC_TRACE_LOW("ATC: invalid param, overstep param max length , return position:%d", i);
                    result = FALSE;
                    break;
                }
                else
                {
                    cmd_info_ptr[param_index].param[data_index] = in_ptr[i];
                    cmd_info_ptr[param_index].param_data_len += 1;
                    data_index += 1;
                }
            }

            i++;
        }
    }

    if(result && in_len > 0)
    {
        offset = i;

        if(NULL != other_cmd_flag_ptr)
        {
            *other_cmd_flag_ptr = other_cmd_flag;
        }

        if(NULL != offset_len_ptr)
        {
            *offset_len_ptr = offset;
        }

        if(NULL != param_count)
        {
            *param_count = param_index + 1;
        }

        ATC_TRACE_LOW("ATC: ATC_GetCommandParams, success, offset:%d,other_cmd_flag:%d",
                      offset, other_cmd_flag);

        for(i = 0; i < (param_index + 1); ++i)
        {
            ATC_TRACE_LOW("PARAM[%d]: | %d | %d | %s |",
                          i,
                          cmd_info_ptr[i].param_len,
                          cmd_info_ptr[i].param_data_len,
                          cmd_info_ptr[i].param);
        }
    }
    else
    {
        ATC_TRACE_LOW("ATC: ATC_GetCommandParams, failed!");
    }

    return result;
}

/*****************************************************************************/
//  Description : This function is used convert string data to number
//
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
BOOLEAN ConvertStringtoUint16(uint8 *str_ptr, uint16 length, uint16 *num_ptr)
{
    uint16  i;
    uint16  numeric_value = 0;
    uint16  numeric_max_value;

    if(NULL == str_ptr || length < 1)
    {
        return FALSE;
    }

    numeric_max_value = (65535 - 9) / 10;

    for(i = 0; i < length; ++i)
    {
        if(str_ptr[i] >= '0' && str_ptr[i] <= '9')
        {
            if(numeric_value > numeric_max_value)
            {
                ATC_TRACE_LOW("ATC: invalid param, numeric value overflow, return position:%d", i);
                return FALSE;
            }

            numeric_value = numeric_value * 10 + (int32)(str_ptr[i] - '0');
        }
        else
        {
            ATC_TRACE_LOW("ATC: invalid param, not numeric, return position:%d", i);
            return FALSE;
        }
    }

    ATC_TRACE_LOW("ATC: get the number: %d", numeric_value);

    if(NULL != num_ptr)
    {
        *num_ptr = numeric_value;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function is used to get the facility type for the security
//      commands.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetFacType(
    uint8        *param_ptr,
    uint8         param_len,
    uint8        *fac_type_ptr
)
{
    int32     i;
    BOOLEAN   flag = FALSE;

    param_ptr[param_len] = '\0';

    for(i = 0; i < AT_PWD_MAX; i++)
    {
        if(!strcmp((char *)param_ptr, s_cpwd_str[i].fac_ptr))
        {
            *fac_type_ptr = (uint8)i;
            flag = TRUE;
        }
    }

    ATC_TRACE_LOW("ATC: Get the facility type %d", flag);
    return flag;
}

/*****************************************************************************/
//  Description : This function is used to get the facility type for the AT+CLCK
//      command.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetClckFacType(
    uint8        *param_ptr,
    uint8         param_len,
    uint8        *fac_type_ptr)
{
    int32     i;
    BOOLEAN   flag = FALSE;

    if(NULL == param_ptr || NULL == fac_type_ptr)
    {
        return flag;
    }

    param_ptr[param_len] = '\0';

    for(i = 0; i < AT_CLCK_MAX; i++)
    {
        if(!strcmp((char *)param_ptr, s_clck_str[i]))
        {
            *fac_type_ptr = (uint8)i;
            flag = TRUE;
        }
    }

    ATC_TRACE_LOW("ATC: Get the CLCK facility type %d", flag);
    return flag;
}



/*****************************************************************************/
//  Description : This function is used to parse the DTMF string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#define AT_VTS_CMD_LEN     6
BOOLEAN ATC_ParseDtmfChar(
    uint8      *param_ptr,
    uint8       param_len,
    uint8      *dtmf_ptr
)
{
    int32          i;
    int32          j = 0;
    uint8          temp_str[AT_VTS_CMD_LEN];
    BOOLEAN        flag = TRUE;

    for(i = 0; i < param_len; i++)
    {
        if((param_len - i) >= AT_VTS_CMD_LEN)
        {
            SCI_MEMCPY(temp_str, &param_ptr[i], AT_VTS_CMD_LEN);
            temp_str[AT_VTS_CMD_LEN - 1] = '\0';

            if(!strcmp((char *)temp_str, "+VTS=") ||
                    !strcmp((char *)temp_str, "+vts="))
            {
                i += 5;
                dtmf_ptr[j++] = param_ptr[i];
                i++;
            }
            else
            {
                flag = FALSE;
                break;
            }
        }
        else
        {
            flag = FALSE;
            break;
        }
    }

    dtmf_ptr[j] = '\0';
    ATC_TRACE_LOW("ATC: Parse dtmf string %d", flag);
    return flag;
}

/*****************************************************************************/
//  Description : This function is used to check the pin string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN  ATC_CheckPinStr(
    uint8              *pin_str,
    uint8               length
)
{
    int32              i;
    BOOLEAN            flag = TRUE;

    if(NULL == pin_str)
    {
        return FALSE;
    }

    for(i = 0; i < length; i++)
    {
        if(*pin_str > '9' || *pin_str < '0')
        {
            flag = FALSE;
        }

        pin_str++;
    }

    ATC_TRACE_LOW("ATC: check pin string %d", flag);
    return flag;
}

/*****************************************************************************/
//  Description : This function is used to change the calling number to string.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
void ATC_ConvertCallingNumToStr(
    MN_CALLING_NUMBER_T    *calling_number_ptr,
    ATC_TELE_TYPE_E        *tel_type_ptr,
    uint8                  *tel_str
)
{
    uint8           *temp_str = PNULL;

    temp_str = tel_str;

    ATC_TRACE_LOW("ATC: the called number len: %d", calling_number_ptr->num_len);

    if(MN_NUM_PLAN_ISDN_TELE == calling_number_ptr->number_plan)
    {
        switch(calling_number_ptr->number_type)
        {
            case MN_NUM_TYPE_INTERNATIONAL:
                *tel_type_ptr = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                break;
            case MN_NUM_TYPE_NATIONAL:
                *tel_type_ptr = ISDN_TELE_PLAN_NATIONAL_TYPE;
                break;
            default:
                *tel_type_ptr = ISDN_TELE_PLAN_UNKNOWN_TYPE;
                break;
        }
    }
    else
    {
        *tel_type_ptr = UNKNOWN_PLAN_UNKNOWN_TYPE;
    }
/*
    if(calling_number_ptr->num_len > 0)
    {
        *temp_str = 0x80 + ((calling_number_ptr->number_type & 0x07) << 4) + (calling_number_ptr->number_plan & 0x0F);
    }
    else
    {
        *temp_str = 0;
    }
    temp_str++;
*/

    // This telphone number is international number
    if(MN_NUM_TYPE_INTERNATIONAL == calling_number_ptr->number_type)
    {
        *temp_str = '+';
        temp_str++;
    }

    // bcd code to digital string
    TransformBcdToStr((uint8 *)calling_number_ptr->party_num, calling_number_ptr->num_len, temp_str);

}

/*****************************************************************************/
//  Description : This function is used to change the called number to string.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli.lv
/*****************************************************************************/
void ATC_ConvertCalledNumToStr(
    MN_CALLED_NUMBER_T     *called_number_ptr,  //input, call number structure
    ATC_TELE_TYPE_E        *tel_type_ptr,       //output,telephone number type
    uint8                  *tel_str_ptr,        //output,telephone number string
    uint16                 *tel_str_len_ptr     //output, telephone number length
)
{
    uint8     *temp_str = PNULL;

    temp_str = tel_str_ptr;

    SCI_ASSERT(PNULL != called_number_ptr);/*assert verified: check null pointer*/
    ATC_TRACE_LOW("ATC: ATC_ConvertCalledNumToStr,Convert called number={%d,%d,%d}....",
                  called_number_ptr->number_type, called_number_ptr->number_plan,
                  called_number_ptr->num_len);

    if(NULL != tel_type_ptr)
    {
        /* bit     8  7  6  5  4  3  2  1
        **     ext| number   |  number    |
        **        | type     |   plan     |
            */
        /*tel type=(called_number_ptr->number_plan +
        (called_number_ptr->number_type << 4))|0x80*/
        if(MN_NUM_PLAN_ISDN_TELE == called_number_ptr->number_plan)
        {
            switch(called_number_ptr->number_type)
            {
                case MN_NUM_TYPE_INTERNATIONAL:
                    *tel_type_ptr = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                    break;
                case MN_NUM_TYPE_NATIONAL:
                    *tel_type_ptr = ISDN_TELE_PLAN_NATIONAL_TYPE;
                    break;
                default:
                    *tel_type_ptr = ISDN_TELE_PLAN_UNKNOWN_TYPE;
                    break;
            }
        }
        else
        {
            *tel_type_ptr = UNKNOWN_PLAN_UNKNOWN_TYPE;
        }

        ATC_TRACE_LOW("ATC: ATC_ConvertCalledNumToStr,Get telephone type:%d", *tel_type_ptr);
    }

    if(NULL != temp_str)
    {
        uint8    num_type_len = 0;

        // This telphone number is international number
        if(MN_NUM_TYPE_INTERNATIONAL == called_number_ptr->number_type)
        {
            *temp_str = '+';
            temp_str++;
            num_type_len = 1;
        }

        // bcd code to digital string
        ATC_TransformBcdToStr((uint8 *)called_number_ptr->party_num,
                              called_number_ptr->num_len,
                              temp_str,
                              tel_str_len_ptr);

        if(NULL == tel_str_len_ptr)
        {
            ATC_TRACE_LOW("ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s", tel_str_ptr);
        }
        else
        {
            *tel_str_len_ptr += num_type_len;

            ATC_TRACE_LOW("ATC: ATC_ConvertCalledNumToStr,Get telephone num:%s,the length:%d", tel_str_ptr, *tel_str_len_ptr);
        }

    }

}

/*****************************************************************************/
//  Description : This function change the ascci into ucs2.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
BOOLEAN ConvertAscUcs2IntoBin(char *str_ptr, uint8 *ucs2_ptr, uint8 uni_str_len)
{
    //Change the ucs2 char to ucs2
    uint8   i;
    uint8   temp;

    for(i = 0; i < uni_str_len; i++)
    {
        temp = 0;

        //Handle the first byte
        //0--9
        if(*(str_ptr + i * 2) >= '0' && *(str_ptr + i * 2) <= '9')
        {
            temp = (*(str_ptr + i * 2) - 0x30) << 4;
        }
        //A--F
        else if(*(str_ptr + i * 2) >= 'A' && *(str_ptr + i * 2) <= 'F')
        {
            temp = (*(str_ptr + i * 2) - 0x41 + 10) << 4;
        }
        //a--f
        else if(*(str_ptr + i * 2) >= 'a' && *(str_ptr + i * 2) <= 'f')
        {
            temp = (*(str_ptr + i * 2) - 0x61 + 10) << 4;
        }
        else
        {
            return FALSE;
        }

        ATC_TRACE_LOW("ATC:High temp=%x", temp);

        //handle the second byte.
        //0--9
        if(*(str_ptr + i * 2 + 1) >= '0' && *(str_ptr + i * 2 + 1) <= '9')
        {
            temp += *(str_ptr + i * 2 + 1) - 0x30;
        }
        //A--F
        else if(*(str_ptr + i * 2 + 1) >= 'A' && *(str_ptr + i * 2 + 1) <= 'F')
        {
            temp += *(str_ptr + i * 2 + 1) - 0x41 + 10;
        }
        //a--f
        else if(*(str_ptr + i * 2 + 1) >= 'a' && *(str_ptr + i * 2 + 1) <= 'f')
        {
            temp += *(str_ptr + i * 2 + 1) - 0x61 + 10;
        }
        else
        {
            return FALSE;
        }

        *(ucs2_ptr + i) = temp;
        ATC_TRACE_LOW("ATC:low temp=%x", temp);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : This function converts the hex to bin
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN   ConvertHexToBin(    // FALSE: There is invalid char
    uint8        *hex_ptr,     // in: the hexadecimal format string
    uint16       length,       // in: the length of hexadecimal string
    uint8        *bin_ptr      // out: pointer to the binary format string
)
{
    uint8        *dest_ptr = bin_ptr;
    int32        i = 0;
    uint8        ch;


    SCI_ASSERT(PNULL != hex_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != bin_ptr);/*assert verified: check null pointer*/

    for(i = 0; i < length; i += 2)
    {
        // the bit 8,7,6,5
        ch = hex_ptr[i];

        // digital 0 - 9
        if(ch >= '0' && ch <= '9')
        {
            *dest_ptr = (uint8)((ch - '0') << 4);
        }
        // a - f
        else if(ch >= 'a' && ch <= 'f')
        {
            *dest_ptr = (uint8)((ch - 'a' + 10) << 4);
        }
        // A - F
        else if(ch >= 'A' && ch <= 'F')
        {
            *dest_ptr = (uint8)((ch - 'A' + 10) << 4);
        }
        else
        {
            return FALSE;
        }

        // the bit 1,2,3,4
        ch = hex_ptr[i+1];/*lint !e661 */

        // digtial 0 - 9
        if(ch >= '0' && ch <= '9')
        {
            *dest_ptr |= (uint8)(ch - '0');
        }
        // a - f
        else if(ch >= 'a' && ch <= 'f')
        {
            *dest_ptr |= (uint8)(ch - 'a' + 10);
        }
        // A - F
        else if(ch >= 'A' && ch <= 'F')
        {
            *dest_ptr |= (uint8)(ch - 'A' + 10);
        }
        else
        {
            return FALSE;
        }

        dest_ptr++;
    }

    return TRUE;

}

/*****************************************************************************/
//  Description : This function converts the bin to hex
//  Global resource dependence :
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
void    ConvertBinToHex(
    uint8        *bin_ptr,     // in: the binary format string
    uint16        length,       // in: the length of binary string
    uint8        *hex_ptr      // out: pointer to the hexadecimal format string
)
{
    uint8        semi_octet;
    int32        i;

    SCI_ASSERT(PNULL != bin_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != hex_ptr);/*assert verified: check null pointer*/

    for(i = 0; i < length; i++)
    {
        // get the high 4 bits
        semi_octet = (uint8)((bin_ptr[i] & 0xF0) >> 4);

        if(semi_octet <= 9)  //semi_octet >= 0
        {
            *hex_ptr = (uint8)(semi_octet + '0');
        }
        else
        {
            *hex_ptr = (uint8)(semi_octet + 'A' - 10);
        }

        hex_ptr++;

        // get the low 4 bits
        semi_octet = (uint8)(bin_ptr[i] & 0x0f);

        if(semi_octet <= 9)  // semi_octet >= 0
        {
            *hex_ptr = (uint8)(semi_octet + '0');
        }
        else
        {
            *hex_ptr = (uint8)(semi_octet + 'A' - 10);
        }

        hex_ptr++;
    }
}

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence :
//  Author:       Wang hui
//  Note:
/*****************************************************************************/
BOOLEAN    ConvertmEeToBin(
    uint8        *mEe_ptr,     // in: the 'mEe' format string
    uint16        length,       // in: the length of binary string
    uint8        *bin_ptr      // out: the binary format string
)
{
    int32        i, j = 0;
    BOOLEAN  ret_val = TRUE;

    SCI_ASSERT(PNULL != bin_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != mEe_ptr);/*assert verified: check null pointer*/

    bin_ptr[0] = bin_ptr[1] = 0;

    for(i = 0; i < length && ret_val == TRUE; i++)
    {
        if(*mEe_ptr == '"')
        {
            mEe_ptr++;
        }
        else if(*mEe_ptr >= '0' && *mEe_ptr <= '9')
        {
            bin_ptr[j] = *mEe_ptr - '0' + 10 * bin_ptr[j];
            mEe_ptr++;
        }
        else if(*mEe_ptr == 'e' || *mEe_ptr == 'E')
        {
            j++;
            mEe_ptr++;

            if(j > 1)
            {
                ret_val = FALSE;
            }
        }
        else
        {
            ret_val = FALSE;
        }
    }

    return ret_val;
}

/*****************************************************************************/
//  Description : This function traslate uint8 array into char for terminal
//                profile
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
void ATC_TranUint8IntoChar(
    uint8 *int_ptr,  //the input pointer of ucs2
    uint16 length,    //the length of ucs2
    uint8 *char_ptr   //the output pointer of ascii
)
{
    int32        i, j;
    uint8        temp;

    SCI_ASSERT(PNULL != char_ptr);/*assert verified: check null pointer*/

    for(i = 0; i < length; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if(0 == j)
            {
                temp = (uint8)((*int_ptr & 0xf0) >> 4);
            }
            else
            {
                temp = (uint8)(*int_ptr & 0x0f);
            }

            switch(temp)
            {
                case 10:
                    *char_ptr++ = 'A';
                    break;
                case 11:
                    *char_ptr++ = 'B';
                    break;
                case 12:
                    *char_ptr++ = 'C';
                    break;
                case 13:
                    *char_ptr++ = 'D';
                    break;
                case 14:
                    *char_ptr++ = 'E';
                    break;
                case 15:
                    *char_ptr++ = 'F';
                    break;
                default:
                    *char_ptr++ = (uint8)(temp + '0');
                    break;
            }
        }

        int_ptr++;
    }

}

/*****************************************************************************/
//  Description : This function is used to output data to  AT UART
//                profile
//  Global resource dependence :
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
BOOLEAN ATC_WriteCmdRsp(
    MN_DUAL_SYS_E  dual_sys,
    uint8 *buf_ptr,  //the input point of data to SIO
    uint32 len       //the length of input data
)
{
    uint8 *pstr = NULL;
    xSignalHeaderRec *psig = PNULL;

    SCI_ASSERT(buf_ptr != PNULL);/*assert verified: check null pointer*/

    if(len > MAX_ATC_RSP_LEN)
    {
        ATC_TRACE_LOW("ATC: Assert Failure rsp len.");
        return FALSE;
    }

    pstr = (uint8 *)SCI_ALLOC_BASE(len);//free it in AT task

    if(pstr == PNULL)
    {
        return FALSE;
    }

    SCI_MEMCPY(pstr, buf_ptr, len);
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL(psig, ATC_WRITE_RSP_REQ, sizeof(AT_WRITE_RSP_T), SCI_IDENTIFY_THREAD());
    ((AT_WRITE_RSP_T*)psig)->len = len;
    ((AT_WRITE_RSP_T*)psig)->str = pstr;
    #ifdef _MUX_ENABLE_
    ((AT_WRITE_RSP_T*)psig)->link_id = ATC_GetMuxLinkIdFromSimId(dual_sys);
    #endif

    SCI_SEND_SIGNAL(psig, P_ATC);

    return TRUE;
}

/*****************************************************************************/
//  Description : get the index of call state according to the call id
//  Global resource dependence : g_calls_state
//  Author: William.Qian
//  Note:
/*****************************************************************************/
uint8 GetIndexOfCallContext(
    uint8           call_id,
    MN_DUAL_SYS_E   dual_sys
)
{
    uint8   i;
    uint8   call_index = ATC_INVALID_CALLID;

    if(call_id >= MN_CALL_MAX_CALL_NUM)
    {
        ATC_TRACE_LOW("ATC: GetIndexOfCallContext out of range, call_id = %d", call_id);
        return call_index;
    }

    for(i = 0; i < g_calls_state[dual_sys].call_amount; i++)
    {
        if(call_id == g_calls_state[dual_sys].call_context[i].call_id)
        {
            call_index = (uint8)i;
            break;
        }
    }

    return call_index;
}

/******************************************************************************/
//  Description : This function get gsm and gprs oper status
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
void ATC_GetOperStatus(
    MN_DUAL_SYS_E                    dual_sys,
    ATC_CONFIG_T                      *atc_config_ptr,
    MN_PHONE_CURRENT_PLMN_INFO_T     *plmn_info_ptr,
    OPER_STATUS_E                    *oper_status_ptr,           //output
    OPER_STATUS_E                    *gprs_oper_status_ptr       //output
)
{
    OPER_STATUS_E oper_status = UNKNOWN;
    OPER_STATUS_E gprs_oper_status = UNKNOWN;

    SCI_ASSERT(PNULL != atc_config_ptr && PNULL != plmn_info_ptr);/*assert verified: check null pointer*/

    if(atc_config_ptr->sim_err_reason[dual_sys] == MNSIM_ERROR_REJECT_BY_NET)
    {
        //sim reject by net
        oper_status = REGISTRATION_DENIED;
        gprs_oper_status = REGISTRATION_DENIED;
        
        /*output gsm and gprs oper status to invoker*/
        if(oper_status_ptr)
        {
            *oper_status_ptr = oper_status;
        }

        if(gprs_oper_status_ptr)
        {
            *gprs_oper_status_ptr = gprs_oper_status;
        }
        return;
    }
     
    switch(plmn_info_ptr->plmn_status)
    {
        case PLMN_NO_SERVICE:
            oper_status = NOT_REGISTERED_NOT_SEARCHING;
            gprs_oper_status = NOT_REGISTERED_NOT_SEARCHING;
            break;
        case PLMN_EMERGENCY_ONLY:
            oper_status = REGISTERED_EMERGENCE;
            gprs_oper_status = REGISTERED_EMERGENCE;
            break;
            
        case PLMN_NORMAL_GSM_ONLY:
            if(plmn_info_ptr->plmn_is_roaming)
            {
                oper_status = REGISTERED_ROAMING;
            }
            else
            {
                oper_status = REGISTERED_HOME_PLMN;
            }

            gprs_oper_status = NOT_REGISTERED_AND_SEARCHING;
            break;
        
        case PLMN_NORMAL_GPRS_ONLY:
            if(plmn_info_ptr->plmn_is_roaming)
            {
                gprs_oper_status = REGISTERED_ROAMING;
            }
            else
            {
                gprs_oper_status = REGISTERED_HOME_PLMN;
            }
            oper_status = REGISTERED_EMERGENCE;
            break;

        case PLMN_NORMAL_GSM_GPRS_BOTH:
            if(plmn_info_ptr->plmn_is_roaming)
            {
                oper_status = REGISTERED_ROAMING;
                gprs_oper_status = REGISTERED_ROAMING;
            }
            else
            {
                oper_status = REGISTERED_HOME_PLMN;
                gprs_oper_status = REGISTERED_HOME_PLMN;
            }
            break;

        default:
            break;
    }

    /*output gsm and gprs oper status to invoker*/
    if(oper_status_ptr)
    {
        *oper_status_ptr = oper_status;
    }

    if(gprs_oper_status_ptr)
    {
        *gprs_oper_status_ptr = gprs_oper_status;
    }
}


/******************************************************************************/
//  Description : This function get ATC_REG_NETWORK_ACT_E by MN_PHONE_CURRENT_PLMN_INFO_T
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
ATC_REG_NETWORK_ACT_E ATC_GetRegNwAct(
    ATC_CONFIG_T                      *atc_config_ptr,
    MN_PHONE_CURRENT_PLMN_INFO_T *register_status_ptr
)
{
    ATC_REG_NETWORK_ACT_E          nw_act = NW_ACT_0_GSM;
 

    SCI_ASSERT(PNULL != register_status_ptr);/*assert verified: check null pointer*/

    /* UTRAN */
    if(register_status_ptr->rat == MN_GMMREG_RAT_3G ||
            register_status_ptr->rat == MN_GMMREG_RAT_GPRS_AND_3G)
    {
        if(register_status_ptr->cell_capability.hsdpa_support &&
                register_status_ptr->cell_capability.hsupa_support)
        {
            nw_act = NW_ACT_6_UTRAN_w_HSDPA_and_HSUPA;
        }
        else if(register_status_ptr->cell_capability.hsupa_support)
        {
            nw_act = NW_ACT_5_UTRAN_w_HSUPA;
        }
        else if(register_status_ptr->cell_capability.hsdpa_support)
        {
            nw_act = NW_ACT_4_UTRAN_w_HSDPA;
        }
        else
        {
            nw_act = NW_ACT_2_UTRAN;
        }
    }
    /* GERAN */
    else if(register_status_ptr->rat == MN_GMMREG_RAT_GPRS)
    {
        if(register_status_ptr->cell_capability.edge_support 
            && atc_config_ptr->edge_support
            && (register_status_ptr->plmn_status == PLMN_NORMAL_GSM_GPRS_BOTH))
        {
            nw_act = NW_ACT_3_GSM_w_EGPRS;
        }
        else
        {
            nw_act = NW_ACT_0_GSM;
        }
    }
    /* OTHERS */
    else
    {
        nw_act = NW_ACT_0_GSM;
    }

    return nw_act;
}

/******************************************************************************/
//  Description : This function get converted rssi value for +CSQ
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
uint8 ATC_GetConvertedRSSIVlaueForCSQ(uint8 receivedrssi)
{
    return ((receivedrssi + 3) / 2);
}

/******************************************************************************/
//  Description : This function get converted rssi value for +ECSQ
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
uint8 ATC_GetConvertedRSSIVlaueForECSQ(uint8 receivedrssi)
{
    return ((receivedrssi + 3) / 2);
}

/*****************************************************************************/
//  Description : This function converts ucs2 string to hexadecimal string
//
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertUCS2ToHEX(
    uint8      *ucs2_str_ptr,    //in, points to the ucs2 string
    uint16      ucs2_str_len,    //in, ucs2 string length
    uint8      *hex_str_ptr,    //out, points to buffer used for storing the hex string
    uint16     *hex_str_len_ptr   //in, maximum length of buffer which pointed by hex_str_ptr
    //out, the length of the hex string converted from ucs2 string
)
{
    uint32       i;
    uint8       *dest_ptr = NULL;

    SCI_ASSERT(PNULL != hex_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != ucs2_str_ptr && NULL != hex_str_len_ptr);/*assert verified: check null pointer*/

    /*check, whether the maximum length of the hex buffer is enough for storing the hex string
    converted from ucs2 string.*/
    if((0 != ucs2_str_len % 4) || (ucs2_str_len / 2 > (*hex_str_len_ptr - 1)))
    {
        *hex_str_len_ptr = 0;
        return FALSE;
    }

    *hex_str_len_ptr = 0;
    dest_ptr = hex_str_ptr;

    for(i = 0; i < ucs2_str_len; i += 4)
    {
        /*the high 8bits character must be 0, which indicated in hexadecimal format "00"*/
        if(48 != ucs2_str_ptr[i] || 48 != ucs2_str_ptr[i+1])
        {
            return FALSE;
        }

        /*copy the low 8bits character to hex buffer*/
        *dest_ptr = (uint8)ucs2_str_ptr[i+2];
        dest_ptr++;
        *dest_ptr = (uint8)ucs2_str_ptr[i+3];
        dest_ptr++;
    }

    *hex_str_len_ptr = ucs2_str_len / 2;
    return TRUE;
}


/*****************************************************************************/
//  Description : This function converts hexadecimal string to ucs2 string
//
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertHEXToUCS2(
    uint8      *hex_str_ptr,    //in, points to the hex string
    uint16      hex_str_len,    //in, hex string length
    uint8      *ucs2_str_ptr,    //out, points to buffer used for storing the ucs2 string
    uint16     *ucs2_str_len_ptr   //in, maximum length of buffer which pointed by ucs2_str_ptr
    //out, the length of the ucs2 string converted from ira string
)
{
    uint32       i;
    uint8        *dest_ptr = NULL;

    SCI_ASSERT(PNULL != hex_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != ucs2_str_ptr && NULL != ucs2_str_len_ptr);/*assert verified: check null pointer*/

    /*NOTES: 16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]);
    UCS2 character strings are converted to hexadecimal numbers from 0000 to FFFF;
    e.g. "004100620063" equals three 16-bit characters with decimal values 65, 98 and 99.*/

    /*check, whether the maximum length of the ucs2 buffer is enough for storing the ucs2 string
    converted from hex string.*/
    if((0 != hex_str_len % 2) || hex_str_len * 2 > (*ucs2_str_len_ptr - 1))
    {
        *ucs2_str_len_ptr = 0;
        return FALSE;
    }

    dest_ptr = ucs2_str_ptr;

    for(i = 0; i < hex_str_len; i += 2)
    {
        /*set 0 as ucs2 high 8bits character, which indicated in hexadecimal format "00"*/
        *dest_ptr = (uint8)48;
        dest_ptr++;
        *dest_ptr = (uint8)48;
        dest_ptr++;
        /*copy low 8bits character to ucs2 buffer*/
        *dest_ptr = (uint8)hex_str_ptr[i];
        dest_ptr++;
        *dest_ptr = (uint8)hex_str_ptr[i+1];
        dest_ptr++;
    }

    *ucs2_str_len_ptr = hex_str_len * 2;
    return TRUE;
}


/*****************************************************************************/
//  Description : This function converts ucs2 string to ira string
//
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertUCS2ToIRA(
    uint8      *ucs2_str_ptr,    //in, points to the ucs2 string
    uint16      ucs2_str_len,    //in, ucs2 string length
    uint8      *ira_str_ptr,    //out, points to buffer used for storing the ira string
    uint16     *ira_str_len_ptr   //in, maximum length of buffer which pointed by ira_str_ptr
    //out, the length of the ira string converted from ucs2 string
)
{
    uint32       i;
    uint8        semi_octet;
    uint8        *dest_ptr = NULL;

    SCI_ASSERT(PNULL != ira_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != ucs2_str_ptr && NULL != ira_str_len_ptr);/*assert verified: check null pointer*/

    /*check, whether the maximum length of the ira buffer is enough for storing the ira string
    converted from ucs2 string.*/
    if((0 != ucs2_str_len % 4) || (ucs2_str_len / 4 > (*ira_str_len_ptr - 1)))
    {
        *ira_str_len_ptr = 0;
        return FALSE;
    }

    *ira_str_len_ptr = 0;
    dest_ptr = ira_str_ptr;

    for(i = 0; i < ucs2_str_len; i += 4)
    {
        /*the high 8bits character must be 0, which indicated in hexadecimal format "00"*/
        if(48 != ucs2_str_ptr[i] || 48 != ucs2_str_ptr[i+1])
        {
            return FALSE;
        }

        /*get the low 8bits character*/
        semi_octet = ucs2_str_ptr[i+2];

        if(semi_octet >= '0' && semi_octet <= '9')
        {
            // digital 0 - 9
            *dest_ptr = (uint8)((semi_octet - '0') << 4);
        }
        else if(semi_octet >= 'a' && semi_octet <= 'f')
        {
            // a - f
            *dest_ptr = (uint8)((semi_octet - 'a' + 10) << 4);
        }
        else if(semi_octet >= 'A' && semi_octet <= 'F')
        {
            // A - F
            *dest_ptr = (uint8)((semi_octet - 'A' + 10) << 4);
        }
        else
        {
            return FALSE;
        }

        // the bit 1,2,3,4
        semi_octet = ucs2_str_ptr[i+3];

        // digtial 0 - 9
        if(semi_octet >= '0' && semi_octet <= '9')
        {
            *dest_ptr |= (uint8)(semi_octet - '0');
        }
        else if(semi_octet >= 'a' && semi_octet <= 'f')
        {
            // a - f
            *dest_ptr |= (uint8)(semi_octet - 'a' + 10);
        }
        else if(semi_octet >= 'A' && semi_octet <= 'F')
        {
            // A - F
            *dest_ptr |= (uint8)(semi_octet - 'A' + 10);
        }
        else
        {
            return FALSE;
        }

        dest_ptr++;
    }

    *ira_str_len_ptr = ucs2_str_len / 4;
    return TRUE;
}


/*****************************************************************************/
//  Description : This function converts ira string to ucs2 string
//
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertIRAToUCS2(
    uint8      *ira_str_ptr,    //in, points to the ira string
    uint16      ira_str_len,    //in, ira string length
    uint8      *ucs2_str_ptr,    //out, points to buffer used for storing the ucs2 string
    uint16     *ucs2_str_len_ptr   //in, maximum length of buffer which pointed by ucs2_str_ptr
    //out, the length of the ucs2 string converted from ira string
)
{
    uint32       i;
    uint8        semi_octet;
    uint8        *dest_ptr = NULL;

    SCI_ASSERT(PNULL != ira_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(PNULL != ucs2_str_ptr && NULL != ucs2_str_len_ptr);/*assert verified: check null pointer*/

    /*NOTES: 16-bit universal multiple-octet coded character set (ISO/IEC10646 [32]);
    UCS2 character strings are converted to hexadecimal numbers from 0000 to FFFF;
    e.g. "004100620063" equals three 16-bit characters with decimal values 65, 98 and 99.*/

    /*check, whether the maximum length of the ucs2 buffer is enough for storing the ucs2 string
    converted from IRA string.*/
    if((*ucs2_str_len_ptr - 1) < 4 * ira_str_len)
    {
        *ucs2_str_len_ptr = 0;
        return FALSE;
    }

    dest_ptr = ucs2_str_ptr;

    for(i = 0; i < ira_str_len; i++)
    {
        /*set 0 as ucs2 high 8bits character, which should be converted to
            hexadecimal numbers "00"*/
        *dest_ptr = (uint8)48;
        dest_ptr++;
        *dest_ptr = (uint8)48;
        dest_ptr++;

        /*set ira character(e.g. decimal value:65) as ucs2 low 8bits character
        which should be converted to hexadecimal numbers(e.g. "41")*/
        semi_octet = (uint8)((ira_str_ptr[i] & 0xF0) >> 4);

        if(semi_octet <= 9)
        {
            *dest_ptr = (uint8)(semi_octet + '0');
        }
        else
        {
            *dest_ptr = (uint8)(semi_octet + 'A' - 10);
        }

        dest_ptr++;

        semi_octet = (uint8)(ira_str_ptr[i] & 0x0f);

        if(semi_octet <= 9)
        {
            *dest_ptr = (uint8)(semi_octet + '0');
        }
        else
        {
            *dest_ptr = (uint8)(semi_octet + 'A' - 10);
        }

        dest_ptr++;
    }

    *ucs2_str_len_ptr = 4 * ira_str_len;
    return TRUE;
}

/*****************************************************************************/
//  Description : This function transform IRA character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:     This is a interface function for TE and TA
/*****************************************************************************/
BOOLEAN ATC_TransformIRAToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
)
{
    uint16    dest_str_len = 0;
    BOOLEAN   result = TRUE;

    uint8     *temp_buffer_ptr = NULL;
    uint16    temp_str_len = 0;
    uint8     seven_bit_offset = 0;

    SCI_ASSERT(NULL != src_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != dest_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(dest_chset_type < ATC_CHSET_MAX_NUM);/*assert verified: check valid value*/

    if(0 == src_str_length || 0 == dest_str_max_len)
    {
        /*the originator string length and destination string maximum length must not be zero */
        ATC_TRACE_LOW("ATC:transform chset IRA,WARNING,invalid string length. src_str_length:%d,dest_str_max_len:%d",
                      src_str_length, dest_str_max_len);
        return FALSE;
    }

    //ATC_TRACE_LOW("ATC:ATC_TransformIRAToOtherChset.%d",dest_chset_type);
    switch(dest_chset_type)
    {
        case ATC_CHSET_IRA:

            /*check the dest buffer size is valid*/
            if(src_str_length > dest_str_max_len)
            {
                ATC_TRACE_LOW("ATC:transform chset IRA=>IRA,WARNING,dest string buffer overflow.");
                return FALSE;
            }

            /*Originator chset is the same to destination chset. Do not need to transform*/
            ATC_TRACE_LOW("ATC:originator chset and destination chset are both IRA.");
            SCI_MEMCPY(dest_str_ptr, src_str_ptr, src_str_length);
            dest_str_len = src_str_length;
            break;
        case ATC_CHSET_GSM:
            /*e.g. "13800210500" => "B1190E0693C56035180C"*/

            //Firset step, pack 8bits to 7bits(refer to 3GPP 23.038)
            /*Get the maximum length of 7bits string according to the IRA chset string length*/
            temp_str_len = (src_str_length * 7) / 8;

            if((src_str_length * 7) % 8 != 0)
            {
                temp_str_len += 1;
            }

            temp_str_len += 1;    //the last character stores '\0'
            temp_buffer_ptr = (uint8 *)SCI_ALLOC_BASE(temp_str_len);
            SCI_ASSERT(NULL != temp_buffer_ptr);/*assert verified: check null pointer*/
            SCI_MEMSET(temp_buffer_ptr, 0, temp_str_len);

            /*transform 8bit string to 7bit string*/
            if(SCI_SUCCESS != SCI_Pack8bitsTo7bits(src_str_ptr, (int16)src_str_length, temp_buffer_ptr, (int16 *)&temp_str_len, &seven_bit_offset))
            {
                result = FALSE;
                break;
            }

            ATC_TRACE_LOW("ATC:transform chset IRA,GSM string length:%d,sevent bit offset:%d.", temp_str_len, seven_bit_offset);

            /*check, whether the destination buffer length is enough or not*/
            if(temp_str_len * 2 >= dest_str_max_len)
            {
                result = FALSE;
                ATC_TRACE_LOW("ATC:transform chset IRA=>GSM,WARNING,dest string buffer overflow.");
                break;
            }

            /*transform 8 bit characters to hexadecimal values
            e.g. "123" => "313233"*/
            ConvertBinToHex(temp_buffer_ptr, temp_str_len, dest_str_ptr);
            dest_str_len = temp_str_len * 2;
            break;
        case ATC_CHSET_HEX:

            /*check, whether the destination buffer length is enough or not*/
            if(src_str_length * 2 >= dest_str_max_len)
            {
                result = FALSE;
                ATC_TRACE_LOW("ATC:transform chset IRA=>HEX,WARNING,dest string buffer overflow.");
                break;
            }

            /*transform 8 bit characters to hexadecimal string
            e.g. "123" => "313233"*/
            ConvertBinToHex(src_str_ptr, src_str_length, dest_str_ptr);
            dest_str_len = src_str_length * 2;
            break;
        case ATC_CHSET_UCS2:

            /*check, whether the destination buffer length is enough or not*/
            if(src_str_length * 4 >= dest_str_max_len)
            {
                result = FALSE;
                ATC_TRACE_LOW("ATC:transform chset IRA=>UCS2,WARNING,dest string buffer overflow.");
                break;
            }

            /*transform 8 bit characters to ucs2 string(in hexadecimal format)
            e.g. "123" => "003100320033"*/
            dest_str_len = dest_str_max_len;

            if(!ATC_ConvertIRAToUCS2(src_str_ptr, src_str_length, dest_str_ptr, &dest_str_len))
            {
                result = FALSE;
                break;
            }

            break;
        default:
            result = FALSE;
            break;
    }

    if(NULL != dest_str_len_ptr)
    {
        *dest_str_len_ptr = dest_str_len;
    }

    if(NULL != temp_buffer_ptr)
    {
        SCI_FREE(temp_buffer_ptr);
    }
        
    if(result)
    {
        ATC_TRACE_LOW("ATC:transform chset IRA => %s success. string length %d=>%d",
                      g_atc_charset[(uint8)dest_chset_type], src_str_length, dest_str_len);
    }
    else
    {
        ATC_TRACE_LOW("ATC:transform chset IRA => %s failed.", g_atc_charset[(uint8)dest_chset_type]);
    }

    return result;
}

/*****************************************************************************/
//  Description : This function transform GSM character set string to other
//                      character set string.
//                      it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:   GSM(7bits chset), refer to 3GPP 23.038
//  This is a interface function for TE and TA
/*****************************************************************************/
BOOLEAN ATC_TransformGSMToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
)
{
    uint16    dest_str_len = 0;
    BOOLEAN   result = TRUE;

    uint8     *temp_buffer_ptr = NULL;
    uint16    temp_str_len = 0;
    uint8     *temp_buffer_ptr2 = NULL;
    uint16    temp_str_len2 = 0;

    SCI_ASSERT(NULL != src_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != dest_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(dest_chset_type < ATC_CHSET_MAX_NUM);/*assert verified: check valid value*/

    if(0 == src_str_length ||0 == dest_str_max_len)
    {
        /*the originator string length and destination string maximum length must not be zero */
        ATC_TRACE_LOW("ATC:transform chset GSM,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d",
                      src_str_length, dest_str_max_len);
        return FALSE;
    }

    switch(dest_chset_type)
    {
        case ATC_CHSET_GSM:

            /*check the dest buffer size is valid*/
            if(src_str_length >= MIN(dest_str_max_len,MN_SMS_MAX_USER_VALID_DATA_LENGTH))
            {
                ATC_TRACE_LOW("ATC:transform chset GSM=>GSM,WARNING,dest string buffer overflow.");
                return FALSE;
            }

            /*Originator chset is the same to destination chset. Do not need to transform*/
            ATC_TRACE_LOW("ATC:originator chset and destination chset are both GSM.");
            SCI_MEMCPY(dest_str_ptr, src_str_ptr, src_str_length);
            dest_str_len = src_str_length;
            break;

        case ATC_CHSET_IRA:
        case ATC_CHSET_HEX:
        case ATC_CHSET_UCS2:
            //First step, convert hexadecimal characters to bin
            temp_str_len = src_str_length / 2;
            temp_buffer_ptr = (uint8 *)SCI_ALLOC_BASE((temp_str_len + 1));
            SCI_ASSERT(NULL != temp_buffer_ptr);/*assert verified: check null pointer*/
            SCI_MEMSET(temp_buffer_ptr, 0, (temp_str_len + 1));

            if(!ConvertHexToBin(src_str_ptr, src_str_length, temp_buffer_ptr))
            {
                result = FALSE;
                break;
            }

            ATC_TRACE_LOW("ATC:transform chset first step GSM(hex=>bin) success, gsm string len:%d.", temp_str_len);

            //Second step, unpack GSM(7 bits) to IRA(8 bits)
            /*e.g. "B1190E0693C56035180C" => "13800210500"*/
            temp_str_len2 = (temp_str_len * 8) / 7;
            temp_str_len2 += 1;    //the last character stores '\0'
            temp_buffer_ptr2 = (uint8 *)SCI_ALLOC_BASE(temp_str_len2);
            SCI_ASSERT(NULL != temp_buffer_ptr2);/*assert verified: check null pointer*/
            SCI_MEMSET(temp_buffer_ptr2, 0, temp_str_len2);

            /*transform 7bit string to 8bit string*/
            if(SCI_SUCCESS != SCI_Unpack7bitsTo8bits(temp_buffer_ptr, (int16)temp_str_len, 0, temp_buffer_ptr2, (int16 *)&temp_str_len2))
            {
                result = FALSE;
                break;
            }

            ATC_TRACE_LOW("ATC:transform chset second step GSM(bin)=>IRA success, IRA string length:%d.", temp_str_len2);

            //Third step, convert IRA to dest chset type
            if(!ATC_TransformIRAToOtherChset(temp_buffer_ptr2,
                                             temp_str_len2,
                                             dest_chset_type,
                                             dest_str_max_len,
                                             dest_str_ptr,
                                             &dest_str_len))
            {
                result = FALSE;
                break;
            }

            break;
        default:
            result = FALSE;
            break;
    }

    if(NULL != dest_str_len_ptr)
    {
        *dest_str_len_ptr = dest_str_len;
    }

    /*free memory*/
    if(NULL != temp_buffer_ptr)
    {
        SCI_FREE(temp_buffer_ptr);
    }

    if(NULL != temp_buffer_ptr2)
    {
        SCI_FREE(temp_buffer_ptr2);
    }

    if(result)
    {
        ATC_TRACE_LOW("ATC:transform chset GSM => %s success. string length %d=>%d",
                      g_atc_charset[(uint8)dest_chset_type], src_str_length, dest_str_len);
    }
    else
    {
        ATC_TRACE_LOW("ATC:transform chset GSM => %s failed.", g_atc_charset[(uint8)dest_chset_type]);
    }

    return result;
}

/*****************************************************************************/
//  Description : This function transform HEX character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:     This is a interface function for TE and TA
/*****************************************************************************/
BOOLEAN ATC_TransformHEXToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
)
{
    uint16    dest_str_len = 0;
    BOOLEAN   result = TRUE;

    uint8     *temp_buffer_ptr = NULL;
    uint16    temp_str_len = 0;

    SCI_ASSERT(NULL != src_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != dest_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(dest_chset_type < ATC_CHSET_MAX_NUM);/*assert verified: check valid value*/

    if(0 == src_str_length || (0 != src_str_length % 2) || 0 == dest_str_max_len)
    {
        /*the originator string length and destination string maximum length must not be zero */
        ATC_TRACE_LOW("ATC:transform chset HEX,WARNING,invalid string length.src_str_length:%d,dest_str_max_len:%d",
                      src_str_length, dest_str_max_len);
        return FALSE;
    }

    switch(dest_chset_type)
    {
        case ATC_CHSET_HEX:

            /*check the dest buffer size is valid*/
            if(src_str_length >= dest_str_max_len)
            {
                ATC_TRACE_LOW("ATC:transform chset HEX=>HEX,WARNING,dest string buffer overflow.");
                return FALSE;
            }

            /*Originator chset is the same to destination chset. Do not need to transform*/
            ATC_TRACE_LOW("ATC:originator chset and destination chset are both HEX.");
            SCI_MEMCPY(dest_str_ptr, src_str_ptr, src_str_length);
            dest_str_len = src_str_length;
            break;
        case ATC_CHSET_GSM:
        case ATC_CHSET_IRA:
            //First step,convert to IRA
            /*the temp buffer stores the IRA chset string*/
            temp_str_len = src_str_length / 2;
            //temp_str_len += 1;    //the last character stores '\0'
            temp_buffer_ptr = (uint8 *)SCI_ALLOC_BASE((temp_str_len + 1));
            SCI_ASSERT(NULL != temp_buffer_ptr);/*assert verified: check null pointer*/
            SCI_MEMSET(temp_buffer_ptr, 0, (temp_str_len + 1));

            /*convert hexadecimal string to 8 bit characters
            e.g. "313233" => "123"*/
            if(!ConvertHexToBin(src_str_ptr, src_str_length, temp_buffer_ptr))
            {
                result = FALSE;
                break;
            }

            ATC_TRACE_LOW("ATC:transform chset first step HEX=>IRA success, ira string len:%d.", temp_str_len);

            if(ATC_CHSET_GSM == dest_chset_type)
            {
                //Second step, convert to GSM
                if(!ATC_TransformIRAToOtherChset(temp_buffer_ptr,
                                                 temp_str_len,
                                                 ATC_CHSET_GSM,
                                                 dest_str_max_len,
                                                 dest_str_ptr,
                                                 &dest_str_len))
                {
                    result = FALSE;
                    break;
                }
            }
            else
            {
                //End, copy to destination buffer
                if(temp_str_len >= dest_str_max_len)
                {
                    result = FALSE;
                    ATC_TRACE_LOW("ATC:transform chset HEX=>IRA,WARNING,dest string buffer overflow.");
                    break;
                }

                SCI_MEMCPY(dest_str_ptr, temp_buffer_ptr, temp_str_len);
                dest_str_len = temp_str_len;
            }

            break;
        case ATC_CHSET_UCS2:
            dest_str_len = dest_str_max_len;

            if(!ATC_ConvertHEXToUCS2(src_str_ptr, src_str_length, dest_str_ptr, &dest_str_len))
            {
                ATC_TRACE_LOW("ATC: ATC_ConvertHEXToUCS2 failed.");
                result = FALSE;
                break;
            }

            break;
        default:
            result = FALSE;
            break;
    }

    if(NULL != dest_str_len_ptr)
    {
        *dest_str_len_ptr = dest_str_len;
    }

    if(NULL != temp_buffer_ptr)
    {
        SCI_FREE(temp_buffer_ptr);
    }


    if(result)
    {
        ATC_TRACE_LOW("ATC:transform chset HEX => %s success. string length %d=>%d",
                      g_atc_charset[(uint8)dest_chset_type], src_str_length, dest_str_len);
    }
    else
    {
        ATC_TRACE_LOW("ATC:transform chset HEX => %s failed.", g_atc_charset[(uint8)dest_chset_type]);
    }

    return result;
}

/*****************************************************************************/
//  Description : This function transform UCS2 character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:     This is a interface function for TE and TA
/*****************************************************************************/
BOOLEAN ATC_TransformUCS2ToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
)
{
    uint16    dest_str_len = 0;
    BOOLEAN   result = TRUE;

    uint8     *temp_buffer_ptr = NULL;
    uint16    temp_str_len = 0;

    SCI_ASSERT(NULL != src_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(NULL != dest_str_ptr);/*assert verified: check null pointer*/
    SCI_ASSERT(dest_chset_type < ATC_CHSET_MAX_NUM);/*assert verified: check valid value*/

    if(0 == src_str_length || (0 != src_str_length % 4) || 0 == dest_str_max_len)
    {
        /*the originator string length and destination string maximum length must not be zero */
        ATC_TRACE_LOW("ATC:transform chset UCS2,WARNING,invalid string length.");
        return FALSE;
    }

    switch(dest_chset_type)
    {
        case ATC_CHSET_UCS2:

            /*check the dest buffer size is valid*/
            if(src_str_length >= dest_str_max_len)
            {
                ATC_TRACE_LOW("ATC:transform chset UCS2=>UCS2,WARNING,dest string buffer overflow.");
                return FALSE;
            }

            /*Originator chset is the same to destination chset. Do not need to transform*/
            ATC_TRACE_LOW("ATC:originator chset and destination chset are both UCS2.");
            SCI_MEMCPY(dest_str_ptr, src_str_ptr, src_str_length);
            dest_str_len = src_str_length;
            break;
        case ATC_CHSET_IRA:
            dest_str_len = dest_str_max_len;

            if(!ATC_ConvertUCS2ToIRA(src_str_ptr, src_str_length, dest_str_ptr, &dest_str_len))
            {
                ATC_TRACE_LOW("ATC:ATC_ConvertUCS2ToIRA failed.");
                result = FALSE;
                break;
            }

            break;
        case ATC_CHSET_GSM:
            //First step, convert UCS2 to IRA
            temp_str_len = src_str_length / 4 + 1;  //the last character stores '\0'
            temp_buffer_ptr = (uint8 *)SCI_ALLOC_BASE(temp_str_len);
            SCI_ASSERT(NULL != temp_buffer_ptr);/*assert verified: check null pointer*/
            SCI_MEMSET(temp_buffer_ptr, 0, temp_str_len);

            if(!ATC_ConvertUCS2ToIRA(src_str_ptr, src_str_length, temp_buffer_ptr, &temp_str_len))
            {
                ATC_TRACE_LOW("ATC:ATC_ConvertUCS2ToIRA failed.");
                result = FALSE;
                break;
            }

            ATC_TRACE_LOW("ATC:transform chset first step UCS2=>IRA success, ira string len:%d.", temp_str_len);

            //Second step, convert IRA to GSM
            if(!ATC_TransformIRAToOtherChset(temp_buffer_ptr,
                                             temp_str_len,
                                             ATC_CHSET_GSM,
                                             dest_str_max_len,
                                             dest_str_ptr,
                                             &dest_str_len))
            {
                result = FALSE;
                break;
            }

            break;
        case ATC_CHSET_HEX:
            dest_str_len = dest_str_max_len;

            if(!ATC_ConvertUCS2ToHEX(src_str_ptr, src_str_length, dest_str_ptr, &dest_str_len))
            {
                ATC_TRACE_LOW("ATC:ATC_ConvertUCS2ToHEX failed.");
                result = FALSE;
                break;
            }

            break;
        default:
            result = FALSE;
            break;
    }

    if(NULL != dest_str_len_ptr)
    {
        *dest_str_len_ptr = dest_str_len;
    }

    if(NULL != temp_buffer_ptr)
    {
        SCI_FREE(temp_buffer_ptr);
    }


    if(result)
    {
        ATC_TRACE_LOW("ATC:transform chset UCS2 => %s success. string length %d=>%d",
                      g_atc_charset[(uint8)dest_chset_type], src_str_length, dest_str_len);
    }
    else
    {
        ATC_TRACE_LOW("ATC:transform chset UCS2 => %s failed.", g_atc_charset[(uint8)dest_chset_type]);
    }

    return result;
}

/*****************************************************************************/
//  Description : This function transform one character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:     This is a interface function for TE and TA
/*****************************************************************************/
BOOLEAN ATC_ChsetTransformation(
    ATC_CHARACTER_SET_TYPE_E     src_chset_type, //in, the origination character set type
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
)
{
    switch(src_chset_type)
    {
        case ATC_CHSET_GSM:
            return ATC_TransformGSMToOtherChset(src_str_ptr, src_str_length,
                                                dest_chset_type, dest_str_max_len, dest_str_ptr, dest_str_len_ptr);
        case ATC_CHSET_IRA:
            return ATC_TransformIRAToOtherChset(src_str_ptr, src_str_length,
                                                dest_chset_type, dest_str_max_len, dest_str_ptr, dest_str_len_ptr);
        case ATC_CHSET_HEX:
            return ATC_TransformHEXToOtherChset(src_str_ptr, src_str_length,
                                                dest_chset_type, dest_str_max_len, dest_str_ptr, dest_str_len_ptr);
        case ATC_CHSET_UCS2:
            return ATC_TransformUCS2ToOtherChset(src_str_ptr, src_str_length,
                                                 dest_chset_type, dest_str_max_len, dest_str_ptr, dest_str_len_ptr);
        default:
            break;
    }

    ATC_TRACE_LOW("ATC:transform chset %s => %s not support yet.",
                  g_atc_charset[(uint8)src_chset_type], g_atc_charset[(uint8)dest_chset_type]);
    return FALSE;
}

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:       King.Li
//  Note:
/*****************************************************************************/
void ConvertAtQosToL4Qos(
                         MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos
                         )
{

    ext_qos->traffic_class = (MN_GPRS_TRAFFIC_CLASS_E)(ext_qos->traffic_class == 4 ? 0: ext_qos->traffic_class+1);
    
    if(0 == ext_qos->delivery_order)
    {
        ext_qos->delivery_order = 2;
    }
    else if(2 == ext_qos->delivery_order)
    {
        ext_qos->delivery_order = 0;
    }
    
    switch(ext_qos->sdu_error_ratio[1])
    {
    case 0:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED;
        break;//MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED
    case 1:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_100k;//MN_GPRS_SDU_ERR_RATIO7, /* 1*10-1  */
        break;

    case 2:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_10k;//MN_GPRS_SDU_ERR_RATIO1, /* 1*10-2  */
        break;

    case 3:
        if(ext_qos->sdu_error_ratio[0] >= 7)
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_7k;//MN_GPRS_SDU_ERR_RATIO2, /* 7*10-3  */
        }
        else
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_1k;//MN_GPRS_SDU_ERR_RATIO3, /* 1*10-3  */
        }
        break;

    case 4:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_100;//MN_GPRS_SDU_ERR_RATIO4, /* 1*10-4  */
        break;

    case 5:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_10;//MN_GPRS_SDU_ERR_RATIO5, /* 1*10-5  */
        break;

    case 6:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_1;//MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
        break;         

    default:
        ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED;//MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
        break;    
    }

    switch(ext_qos->residual_ber[1])
    {
    case 0:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_SUBSCRIBED;         
        break;//MN_GPRS_BIT_ERROR_RATE_SUBSCRIBED

    case 1:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_5m;//MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
        break;

    case 2:
        if(ext_qos->residual_ber[0] >= 5)    
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_5m;//MN_GPRS_BIT_ERROR_RATE1, /* 5*10-2 */
        }
        else
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_1m;//MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
        }
        break;

    case 3:
        if(ext_qos->residual_ber[0] >= 5)
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_500k;//MN_GPRS_BIT_ERROR_RATE3, /* 5*10-3  */
        }
        else if(ext_qos->residual_ber[0] == 4)
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_400k;//MN_GPRS_BIT_ERROR_RATE4, /* 4*10-3  */
        }
        else
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_100k;//MN_GPRS_BIT_ERROR_RATE5, /* 1*10-3  */
        }
        break;

    case 4:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_10k;//MN_GPRS_BIT_ERROR_RATE6, /* 1*10-4  */
        break;

    case 5:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_1k;//MN_GPRS_BIT_ERROR_RATE7, /* 1*10-5  */
        break;

    case 6:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_100;//MN_GPRS_BIT_ERROR_RATE8, /* 1*10-6  */
        break;

    case 7:
    case 8:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_6;//MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
        break;

    default:
        ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_SUBSCRIBED;//MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
        break;    
    }

    /*Delivery of erroneous SDUs is set 0(no),1(yes),2(no detect) 0r 3(subscribed value)Ref spec 27007/10.1.7.
      0 is encoded to 3,1 is encoded to 2,2 is encoded to 1,3 is encoded to 0, Ref spec 24008/10.5.6.5.
     */
    ext_qos->delivery_of_err_sdu = 3 - ext_qos->delivery_of_err_sdu;    
}

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:      King Li
//  Note:
/*****************************************************************************/
void ConvertL4QosToAtQos(
                         MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos
                         )
{

    ext_qos->traffic_class = (MN_GPRS_TRAFFIC_CLASS_E)(ext_qos->traffic_class == 0 ? 4: ext_qos->traffic_class-1);
    
    if(0 == ext_qos->delivery_order)
    {
        ext_qos->delivery_order = 2;
    }
    else if(2 == ext_qos->delivery_order)
    {
        ext_qos->delivery_order = 0;
    }

    switch(ext_qos->sdu_error_ratio[0])
    {
    case MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED://MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED
        ext_qos->sdu_error_ratio[1] = 0;
        break;

    case MN_GPRS_SDU_ERR_RATIO_10k://MN_GPRS_SDU_ERR_RATIO1, /* 1*10-2  */
        ext_qos->sdu_error_ratio[1] = 2;
        break;

    case MN_GPRS_SDU_ERR_RATIO_7k://MN_GPRS_SDU_ERR_RATIO2, /* 7*10-3  */
        ext_qos->sdu_error_ratio[0] = 7;
        ext_qos->sdu_error_ratio[1] = 3;
        break;

    case MN_GPRS_SDU_ERR_RATIO_1k://MN_GPRS_SDU_ERR_RATIO3, /* 1*10-3  */
        ext_qos->sdu_error_ratio[0] = 1;
        ext_qos->sdu_error_ratio[1] = 3;
        break;

    case MN_GPRS_SDU_ERR_RATIO_100://MN_GPRS_SDU_ERR_RATIO4, /* 1*10-4  */
        ext_qos->sdu_error_ratio[0] = 1;
        ext_qos->sdu_error_ratio[1] = 4; 
        break;

    case MN_GPRS_SDU_ERR_RATIO_10://MN_GPRS_SDU_ERR_RATIO5, /* 1*10-5  */
        ext_qos->sdu_error_ratio[0] = 1;
        ext_qos->sdu_error_ratio[1] = 5; 
        break;

    case MN_GPRS_SDU_ERR_RATIO_1://MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
        ext_qos->sdu_error_ratio[0] = 1;
        ext_qos->sdu_error_ratio[1] = 6;
        break;

    case MN_GPRS_SDU_ERR_RATIO_100k://MN_GPRS_SDU_ERR_RATIO7, /* 1*10-1  */
        ext_qos->sdu_error_ratio[0] = 1;
        ext_qos->sdu_error_ratio[1] = 1;
        break;

    default:
        //invalid sdu_error_ratio
        ext_qos->sdu_error_ratio[0] = 0xff;
        ext_qos->sdu_error_ratio[1] = 0xff;     
        break;    
    }

    switch(ext_qos->residual_ber[0])
    {
    case 0://MN_GPRS_BIT_ERROR_RATE_SUBSCRIBED
        ext_qos->residual_ber[1] = 0;
        break;

    case MN_GPRS_RESIDUAL_BER_5m://MN_GPRS_BIT_ERROR_RATE1, /* 5*10-2 */
        ext_qos->residual_ber[0] = 5;
        ext_qos->residual_ber[1] = 2;
        break;

    case MN_GPRS_RESIDUAL_BER_1m://MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
        ext_qos->residual_ber[0] = 1;
        ext_qos->residual_ber[1] = 2;
        break;

    case MN_GPRS_RESIDUAL_BER_500k://MN_GPRS_BIT_ERROR_RATE3, /* 5*10-3  */
        ext_qos->residual_ber[0] = 5;
        ext_qos->residual_ber[1] = 3;
        break;

    case MN_GPRS_RESIDUAL_BER_400k://MN_GPRS_BIT_ERROR_RATE4, /* 4*10-3  */
        ext_qos->residual_ber[0] = 4;
        ext_qos->residual_ber[1] = 3;
        break;

    case MN_GPRS_RESIDUAL_BER_100k://MN_GPRS_BIT_ERROR_RATE5, /* 1*10-3  */
        ext_qos->residual_ber[0] = 1;
        ext_qos->residual_ber[1] = 3;
        break;

    case MN_GPRS_RESIDUAL_BER_10k://MN_GPRS_BIT_ERROR_RATE6, /* 1*10-4  */
        ext_qos->residual_ber[0] = 1;
        ext_qos->residual_ber[1] = 4;
        break;

    case MN_GPRS_RESIDUAL_BER_1k://MN_GPRS_BIT_ERROR_RATE7, /* 1*10-5  */
        ext_qos->residual_ber[0] = 1;
        ext_qos->residual_ber[1] = 5;
        break;

    case MN_GPRS_RESIDUAL_BER_100://MN_GPRS_BIT_ERROR_RATE8, /* 1*10-6  */
        ext_qos->residual_ber[0] = 1;
        ext_qos->residual_ber[1] = 6;
        break;

    case MN_GPRS_RESIDUAL_BER_6://MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
        ext_qos->residual_ber[0] = 6;
        ext_qos->residual_ber[1] = 8;
        break;

    default:
        //invalid residual_ber
        ext_qos->residual_ber[0] = 0xff;
        ext_qos->residual_ber[1] = 0xff;
        break;    
    }

    ext_qos->delivery_of_err_sdu = 3 - ext_qos->delivery_of_err_sdu;
}

#ifdef HUAWEI_6500_MODEM
unsigned int ATC_GetMap(char* buf)
{
    int arr_size = sizeof(cmd_list)/sizeof(AT_CMD_LIST_T);
    int i;
    char tmp_buf[40] = {0};
    int cmp_len;
    char* pos=NULL;
    char* tmp_pos = NULL;
    
    pos = strstr(buf, "=");
    
    if(pos != NULL)
    {
        cmp_len = pos - buf > 36 ? 36 : pos - buf;
        
        memcpy(tmp_buf, buf, cmp_len);
        
        while(cmp_len)
        {
            tmp_buf[cmp_len-1] = toupper(tmp_buf[cmp_len-1]);
            cmp_len--;
        }
		
        for(i=0; i<arr_size; i++)
        {
            tmp_pos = strstr(tmp_buf,cmd_list[i].cmd);
            if(tmp_pos != NULL)
            {
            	   return cmd_list[i].map;
            }
        }
    }
    
    return 0;
}

BOOLEAN ATC_CheckNum(char *buf, int len)
{
    int i;
    
    while(*buf == ' ')
    {
    	buf++;
    	len--;
    }
    
    while(*(buf+len-1) == ' ')
    {
    	len--;
    }
    
    for(i=0; i<len; i++)
    {
    	if(*buf>= '0' && *buf<='9')
    	{
    		buf++;
    		continue;
    	}
    	else
    	{
    		if(i == 0)
    		{
    			if(*buf == '+' || *buf == '-')
    			{
    				buf++;
    				continue;
    			}
				
                     if(*buf == '?' && len == 1)
                     {
                         return TRUE;
                     }
    		}
    		return FALSE;
    	}
    
    }
    return TRUE;
}

BOOLEAN ATC_StrCheck(char *buf)
{
    char *pos = NULL;
    char *tmp_pos = NULL;
    char *sem_pos = NULL;
    int len,tmp_len, str_len;
    int param_no=0;
    unsigned int map=0;
    BOOLEAN flag = FALSE;

	SCI_TRACE_LOW("ATC: strcheck %s,len = %d", buf, strlen(buf));
     
    map = ATC_GetMap(buf);
    pos = strstr(buf, "=");
    str_len = strlen((char *)buf);
	
    if(pos != NULL)
    {
        len = strlen(pos);
    
    	while(len>1)
    	{
            sem_pos = strstr(pos+1, ";");
            tmp_pos = strstr(pos+1, ","); //pos: =,
            
            if(tmp_pos != NULL && ((sem_pos == NULL) || (sem_pos > tmp_pos)))
            {
                tmp_len = tmp_pos - pos - 1;
                if(!flag && (!ATC_CheckNum(pos+1, tmp_len) || ((map >> param_no) & 0x1)))
                {
                	if(*(pos+1) != '"' && *(tmp_pos-1) != '"')
                	{
                		int s_num = 0;
                		while(*(pos+1+s_num) == ' ')
                		    s_num++;
                		memmove(pos+2+s_num, pos+1+s_num, strlen(pos+1+s_num));
                		*(pos+1+s_num) = '"';
                		
                		s_num=0;
                            while(*(tmp_pos-s_num) == ' ')
                		    s_num++;
                		memmove(tmp_pos+2-s_num, tmp_pos+1-s_num, strlen(tmp_pos+1-s_num));
                							
                		*(tmp_pos+1-s_num) = '"';
                		len = strlen(tmp_pos+2);
                		pos = pos + tmp_len + 3;
						
				str_len = str_len +2;
                	}
                	else
                	{
                         len = strlen(tmp_pos);
                         pos = pos + tmp_len + 1;
                         if((*(pos-tmp_len) == '"' && *(tmp_pos-1) != '"') || ((pos-tmp_len == tmp_pos-1) && *(pos-tmp_len) == '"'))
                         {
                         	flag = TRUE;
                         	continue;
                         }
                	}
                }
                else
                {
                	len = strlen(tmp_pos);
                	pos = pos + tmp_len + 1;
                	if((*(pos-tmp_len) != '"' && *(tmp_pos-1) == '"') || ((pos-tmp_len == tmp_pos-1) && *(tmp_pos-1) == '"'))
                	{
                	    flag = FALSE;
                	}
                }
            }
            else if(sem_pos != NULL)
            {
                tmp_len = sem_pos - pos - 1;
                
                if(!ATC_CheckNum(pos+1, tmp_len) || ((map >> param_no) & 0x1))
                {
                	if(!flag && (*(pos+1) != '"' && *(pos + tmp_len) != '"'))
                	{
                		int s_num = 0;
                		while(*(pos+1+s_num) == ' ')
                			s_num++;
                
                		memmove(pos+2+s_num, pos+1+s_num, strlen(pos+1+s_num));
                		*(pos+1+s_num) = '"';
                
                		s_num=0;
                            while(*(sem_pos-s_num) == ' ')
                			s_num++;
                		memmove(sem_pos+2-s_num, sem_pos+1-s_num, strlen(sem_pos+1-s_num));
                		
                		*(pos+ tmp_len + 2) = '"';
                		len = strlen(sem_pos+2);
                		pos = pos + tmp_len + 3;//after;
                		str_len = str_len +2;
                	}
                	else
                	{
                		len = strlen(sem_pos);
                		pos = pos + tmp_len + 1;
                		if((*(pos-tmp_len) == '"' && *(pos + tmp_len) != '"') || ((pos-tmp_len == pos + tmp_len) && *(pos-tmp_len) == '"'))
                		{
                			flag = TRUE;
                			continue;
                		}
                	}
                }
                else
                {
                	len = strlen(sem_pos);
                	pos = pos + tmp_len + 1;
                	if(*(pos-tmp_len) != '"' && *(pos + tmp_len) == '"' || ((pos-tmp_len == pos + tmp_len) && *(pos + tmp_len) == '"'))
                	{
                		flag = FALSE;
                	}
                }
                
                //new cmd line
                if(strlen(pos) <= 3)
                	break;
                
                param_no = 0;
                map = ATC_GetMap(pos);
                pos = strstr(pos, "=");
                if(pos ==  NULL)
                	break;
                len = strlen(pos+1);
                
                continue;
            }
            else
            {
                tmp_len = strlen(pos+1);
                while((*(pos+tmp_len) == 0x0d || *(pos+tmp_len) == 0x0a) && tmp_len)
                {
                	tmp_len--;
                }
                
                if(!ATC_CheckNum(pos+1, tmp_len) || ((map >> param_no) & 0x1))
                {
                	if(*(pos+1) != '"' && *(pos + tmp_len) != '"')
                	{
                		int s_num = 0;
                		while(*(pos+1+s_num) == ' ')
                		    s_num++;
                
                		memmove(pos+2+s_num, pos+1+s_num, strlen(pos+1+s_num));
                		*(pos+1+s_num) = '"';

				memmove(pos+ tmp_len + 3, pos+ tmp_len + 2, strlen(pos+ tmp_len + 2));
                		*(pos+ tmp_len + 2) = '"';
			       str_len = str_len +2;
                	}
                }
                break;
            }
                
                if(!flag)
                param_no++;
    	}
    }

    *(buf +str_len) = '\0';
    *(buf +str_len+1) = '\0';


	return TRUE;
}

#endif
