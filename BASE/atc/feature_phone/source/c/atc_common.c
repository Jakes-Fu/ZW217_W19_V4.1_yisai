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
#include "base_trc.h"
#include "atc_common.h"
#include "atc_basic_cmd.h"
#include "env_atc_signal.h"
#include "efs.h"
#include "dal_audio.h"
#if 0
#ifdef BT_PS_BQB_TEST
#include "bt_abs.h"
#include "sig_code.h"
#endif
#endif
/**---------------------------------------------------------------------------*
 **                         Declare Variable                                  *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#ifdef _DEBUG
static uint8  * g_atc_status_desc[] =
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
static uint8  * g_atc_status_desc[] =
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

 
#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
static const  ATC_CPWD_STR_T      s_cpwd_str[AT_PWD_MAX] =
{
    {8,"PS"},
    {8,"SC"},
    {4,"AO"},
    {4,"OI"},
    {4,"OX"},
    {4,"AI"},
    {4,"IR"},
    {4,"AB"},
    {4,"AG"},
    {4,"AC"},
    {8,"P2"},
    {8,"FD"},
    {8,"PN"},
};

// for the AT+CLCK command
static const char*    s_clck_str[AT_CLCK_MAX] =
{
    "PS","SC","AO","OI","OX","AI","IR","AB","AG","AC","PN","FD"
};
#endif


//the table is for changing from 03.38 alphabet table to ASCII alphabet table  
//add by sunsome.ju
const unsigned char atc_default_to_ascii_table[128] =
{
    0x40,0xA3,0x24,0xA5,0xE8,0xE9,0xF9,0xEC,0xF2,0xC7,0x0A,0xD8,0xF8,0x0D,0xC5,0xE5,
    0x20,0x5F,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xC6,0xE6,0xDF,0xC9,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x20,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0xC4,0xD6,0xD1,0xDC,0xA7,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0xE4,0xF6,0xF1,0xFC,0xE0
};

const unsigned char atc_default_to_ascii_extension_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x5e,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x7B,0x7D,0x20,0x20,0x20,0x20,0x20,0x5c,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x5B,0x7e,0x5D,0x20,
    0x7c,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20
};

//the table is for changing from ASCII alphabet table to 03.38 alphabet table  
//add by sunsome.ju
const unsigned char atc_ascii_to_default_table[128] =
{
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x0A,0x20,0x20,0x0D,0x20,0x20,
    0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
    0x00,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x20,0x20,0x20,0x20,0x11,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x20,0x20,0x20,0x20,0x20
};

extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];
#if 0
#ifdef BT_PS_BQB_TEST
extern RSP_RESULT_STR     			g_rsp_str;
extern const ATC_INFO_T            	g_atc_info_table[];
#endif
#endif

#if !defined(_ULTRA_LOW_CODE_) || defined(ATC_ORANGE_ENABLE)||defined(_ATC_UIX8910_ENABLE_)
/******************************************************************************/
// Description : This function formats the clck response.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
void ATC_FormatCpwdStr(
                    uint8        *pwd_ptr
                    )
{
    sprintf((char*)pwd_ptr, "+CPWD: (\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d),(\"%s\",%d), (\"%s\",%d),(\"%s\",%d),(\"%s\",%d)",\
            s_cpwd_str[AT_PWD_PS].fac_ptr,s_cpwd_str[AT_PWD_PS].number,\
            s_cpwd_str[AT_PWD_SC].fac_ptr,s_cpwd_str[AT_PWD_SC].number,\
            s_cpwd_str[AT_PWD_AO].fac_ptr,s_cpwd_str[AT_PWD_AO].number,\
            s_cpwd_str[AT_PWD_OI].fac_ptr,s_cpwd_str[AT_PWD_OI].number,\
            s_cpwd_str[AT_PWD_OX].fac_ptr,s_cpwd_str[AT_PWD_OX].number,\
            s_cpwd_str[AT_PWD_AI].fac_ptr,s_cpwd_str[AT_PWD_AI].number,\
            s_cpwd_str[AT_PWD_IR].fac_ptr,s_cpwd_str[AT_PWD_IR].number,\
            s_cpwd_str[AT_PWD_AB].fac_ptr,s_cpwd_str[AT_PWD_AB].number,\
            s_cpwd_str[AT_PWD_AG].fac_ptr,s_cpwd_str[AT_PWD_AG].number,\
            s_cpwd_str[AT_PWD_AC].fac_ptr,s_cpwd_str[AT_PWD_AC].number,\
            s_cpwd_str[AT_PWD_P2].fac_ptr,s_cpwd_str[AT_PWD_P2].number,\
            s_cpwd_str[AT_PWD_FD].fac_ptr,s_cpwd_str[AT_PWD_FD].number,\
            s_cpwd_str[AT_PWD_PN].fac_ptr,s_cpwd_str[AT_PWD_PN].number);
}

/******************************************************************************/
// Description : This function formats the clck read command response string.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
void ATC_FormatClckReadRsp(
                    uint8    *rsp_str,
                    uint8    sc_state
                    )
{
    sprintf((char*)rsp_str, "+CLCK: (\"%s\", 0),(\"%s\", %d),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0),(\"%s\", 0)",\
            s_clck_str[AT_CLCK_PS], s_clck_str[AT_CLCK_SC], sc_state,\
            s_clck_str[AT_CLCK_AO], s_clck_str[AT_CLCK_OI],\
            s_clck_str[AT_CLCK_OX], s_clck_str[AT_CLCK_AI],\
            s_clck_str[AT_CLCK_IR], s_clck_str[AT_CLCK_AB],\
            s_clck_str[AT_CLCK_AG], s_clck_str[AT_CLCK_AC],\
            s_clck_str[AT_CLCK_PN], s_clck_str[AT_CLCK_FD]);
}

/******************************************************************************/
// Description : This function formats the clck test command response string.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
void ATC_FormatClckTestRsp(
                    uint8      *rsp_str
                    )
{
    sprintf((char*)rsp_str, "+CLCK: (\"%s\",\"%s\",\"%s\",\"%s\", \"%s\",\"%s\",\"%s\", \"%s\",\"%s\", \"%s\",\"%s\", \"%s\")",\
            s_clck_str[AT_CLCK_PS], s_clck_str[AT_CLCK_SC],\
            s_clck_str[AT_CLCK_AO], s_clck_str[AT_CLCK_OI],\
            s_clck_str[AT_CLCK_OX], s_clck_str[AT_CLCK_AI],\
            s_clck_str[AT_CLCK_IR], s_clck_str[AT_CLCK_AB],\
            s_clck_str[AT_CLCK_AG], s_clck_str[AT_CLCK_AC],\
            s_clck_str[AT_CLCK_PN], s_clck_str[AT_CLCK_FD]);
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
    int32              i    = 0;
    int32              j    = 0;
    uint16            k    = 0;
    BOOLEAN    flag    = TRUE;
    uint8       count = param_max_count;

    if (NULL == in_ptr || NULL == param_ptr || param_max_count < 1)
        return FALSE;

    for (i=0; i<in_len; i++)
    {
        if (in_ptr[i] == ',')
        {
            k++;
            j = 0;
            count--;
            if (0 == count)
            {
                //overfull params
                flag = FALSE;
                break;
            }
#if 0
            if (3 < k)
            {
                flag = FALSE;
                break;
            }
#endif
        }
        else
        {
            if (j < ATC_PWD_MAX_LEN)
            {
                if (in_ptr[i] != '"')
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

    return flag;

}

/*****************************************************************************/
//  Description : This function is used to get the facility type for the security
//  commands.     
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
    int32              i    = 0;
    BOOLEAN   flag    = FALSE;

    param_ptr[param_len] = '\0';

    for (i=0; i<AT_PWD_MAX; i++)
    {
        if (!strcmp((char*)param_ptr, s_cpwd_str[i].fac_ptr))
        {
            *fac_type_ptr = (uint8)i;
            flag = TRUE;
        }       
    }

    return flag;    
}

/*****************************************************************************/
//  Description : This function is used to get the facility type for the AT+CLCK 
//  command.     
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
BOOLEAN ATC_GetClckFacType(
                uint8        *param_ptr,
                uint8         param_len,
                uint8        *fac_type_ptr)
{
    int32              i    = 0;
    BOOLEAN   flag    = FALSE;

    param_ptr[param_len] = '\0';

    for (i=0; i<AT_CLCK_MAX; i++)
    {
        if (!strcmp((char*)param_ptr, s_clck_str[i]))
        {
            *fac_type_ptr = (uint8)i;
            flag = TRUE;
        }       
    }

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
    int32              i    = 0;
    BOOLEAN    flag   = TRUE;

    for (i=0; i<length; i++)
    {
        if (*pin_str > '9' || *pin_str < '0')
        {
            flag = FALSE;
        }
        pin_str++;
    }

    return flag;
}
#endif

#ifdef WIN32
void ATCC_trace(const char *fmt, ... )
{
    char    trace_buf[1024];

    va_list     a;
    va_start(a, fmt);
    vsprintf(trace_buf, fmt, a);
    OutputDebugString(trace_buf);
    va_end(a);
}

void ATCC_WriteCmdRes(char * buf, uint len )
{
    buf[len] = '\0';
    
    printf("%s\n",buf);
}
#endif /* End of #ifdef WIN32 */

/*****************************************************************************/
//  Description : operate the ascii character as the character in default ext table 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
LOCAL BOOLEAN AsciiIsDefaultExt(
                                uint8   ascii_char,         //IN:
                                uint8   *default_char_ptr   //OUT:
                                )
{
    BOOLEAN     result = TRUE;

    switch (ascii_char) 
    {
        case '^':
        {
            *default_char_ptr = 0x14;
            break;
        }

        case '{':
        {
            *default_char_ptr = 0x28;
            break;
        }

        case '}':
        {
            *default_char_ptr = 0x29;
            break;
        }

        case '\\':
        {
            *default_char_ptr = 0x2f;
            break;
        }

        case '[':
        {
            *default_char_ptr = 0x3c;
            break;
        }

        case '~':
        {
            *default_char_ptr = 0x3d;
            break;
        }

        case ']':
        {
            *default_char_ptr = 0x3e;
            break;
        }
            
        case '|':
        {
            *default_char_ptr = 0x40;
            break;
        }
            
        default:
        {
            *default_char_ptr = 0x20;
            result = FALSE;
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 ATC_Ascii2default(                          //RETURN: the length of default string
                         uint8      *ascii_ptr,      // IN:
                         uint8      *default_ptr, // OUT:
                         uint16     str_len          // IN:
                         )
{
    uint16  default_len    = 0;
    int32                   i     = 0;
    uint8     temp_char   = 0;

    SCI_ASSERT(PNULL != ascii_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != default_ptr); /*assert verified check NULL pointer*/
    
    for (i=0; i<str_len; i++)
    {
        if (ascii_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            // according to GSM 7bit default alphabet extension table
            if (AsciiIsDefaultExt(ascii_ptr[i], &temp_char))
            {
                default_ptr[default_len] = 0x1b;
                default_len++;
                default_ptr[default_len] = temp_char;
                default_len++;
            }
            else
            {
                default_ptr[default_len] = atc_ascii_to_default_table[ascii_ptr[i]];
                default_len++;
            }
        }
    }

    return (default_len);
}


/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 ATC_Default2Ascii(                          //RETURN:
                         uint8      *default_ptr,   //IN:
                         uint8      *ascii_ptr,     //OUT:
                         uint16     str_len         //IN:
                         )
{
    uint16 ascii_len    = 0;
    int32              i     = 0;

    SCI_ASSERT(PNULL != default_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != ascii_ptr);   /*assert verified check NULL pointer*/
    
    for (i=0; i<str_len; i++)
    {
        if (default_ptr[i] >= 0x80)
        {
            break;
        }
        else
        {
            if (default_ptr[i] == 0x1b)
            {
                i++;
                if (default_ptr[i] >= 0x80)
                {
                    break;
                }
                else
                {
                    ascii_ptr[ascii_len] = atc_default_to_ascii_extension_table[default_ptr[i]];
                    ascii_len++;
                }
            }
            else
            {
                 ascii_ptr[ascii_len] = atc_default_to_ascii_table[default_ptr[i]];
                 ascii_len++;
            }
        }
    }

    return (ascii_len);
}

#if !defined( _ULTRA_LOW_CODE_ ) ||defined(_ATC_UIX8910_ENABLE_)
LOCAL void ATCC_LogCurATCInfo(
        ATC_CUR_INFO_LIST_T * atc_info_ptr
        )
{
    ATC_CUR_PARAM_INFO_LIST_T * param_info_ptr = NULL;
    uint        index = 0;

    SCI_ASSERT( NULL != atc_info_ptr );/*assert verified*/

    //ATC_TRACE:"Log ATC infomation:\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_457_112_2_17_22_16_54_396,(uint8*)"" );
    
    while ( NULL != atc_info_ptr )
    {
        //ATC_TRACE:"AT Command Index : %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_461_112_2_17_22_16_54_397,(uint8*)"d", atc_info_ptr->cmd_index );
        //ATC_TRACE:"AT Command Type :"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_462_112_2_17_22_16_54_398,(uint8*)"" );
    
        if ( (ATC_CMD_TYPE_BASIC & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" BASIC"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_466_112_2_17_22_16_54_399,(uint8*)"" );
        }
    
        if ( (ATC_CMD_TYPE_EXTEND & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" EXTEND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_471_112_2_17_22_16_54_400,(uint8*)"" );
        }
    
        if ( (ATC_CMD_TYPE_EXECUTE & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" EXECUTE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_476_112_2_17_22_16_54_401,(uint8*)"" );
        }
    
        if ( (ATC_CMD_TYPE_SET & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" SET"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_481_112_2_17_22_16_54_402,(uint8*)"" );
        }
    
        if ( (ATC_CMD_TYPE_READ & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" READ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_486_112_2_17_22_16_54_403,(uint8*)"" );
        }
    
        if ( (ATC_CMD_TYPE_TEST & atc_info_ptr->cmd_type) )
        {
            //SCI_TRACE_LOW:" TEST"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_491_112_2_17_22_16_54_404,(uint8*)"" );
        }
    
        //ATC_TRACE:"AT Command Parameter Number : %d\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_494_112_2_17_22_16_54_405,(uint8*)"d", atc_info_ptr->cur_param_num);
    
        param_info_ptr = atc_info_ptr->cur_param_list_ptr;
        for( index = 0; index < atc_info_ptr->cur_param_num; index++ )
        {
            SCI_ASSERT( NULL != param_info_ptr );/*assert verified*/
    
            switch ( param_info_ptr->param_type )
            {
                case ATC_CMD_PARAM_TYPE_NUMERIC:
                {
                    //ATC_TRACE:"AT Command Parameter %d : %dn"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_507_112_2_17_22_16_54_406,(uint8*)"dd", index,param_info_ptr->param_info.num );
                    break;
                }
        
                case ATC_CMD_PARAM_TYPE_STRING:
                {
                    //ATC_TRACE:"AT Command Parameter %d : %sn"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_515_112_2_17_22_16_54_407,(uint8*)"ds", index,param_info_ptr->param_info.str_ptr->str_ptr );
                    break;
                }
        
                case ATC_CMD_PARAM_TYPE_DEFAULT:
                {
                    //ATC_TRACE:"AT Command Parameter %d : Default\n"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_521_112_2_17_22_16_54_408,(uint8*)"d", index );
                    break;
                }
        
                default:
                    break;
            }
    
            param_info_ptr = param_info_ptr->next;
        } // End of for loop

        atc_info_ptr = atc_info_ptr->next;
    } // End of loop while ( NULL != atc_info_ptr )
}
//#endif  Nemge
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

    for (i=0; i<param_len; i++)
    {
        if ( (param_len - i) >= AT_VTS_CMD_LEN)
        {
            SCI_MEMCPY(temp_str, &param_ptr[i], AT_VTS_CMD_LEN);
            temp_str[AT_VTS_CMD_LEN - 1] = '\0'; 

            if (!strcmp((char*)temp_str, "+VTS=") 
                || !strcmp((char*)temp_str, "+vts="))
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

    return flag;
}
//#ifndef _ULTRA_LOW_CODE_  Nemge
/*****************************************************************************/
//  Description : This function converts the ascii to ucs2 hex
//  Global resource dependence : 
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertAsciiToUcs2Hex(    
                        uint8        *txt_ptr,     // in: the ascii format string
                        uint16        length,       // in: the length of binary string 
                        uint8        *hex_ptr      // out: pointer to the CSCS UCS2 format string
                        )
{
    uint16            i    = 0;
    uint8 *tmp_ptr    = PNULL;
    
    SCI_ASSERT(PNULL != txt_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != hex_ptr); /*assert verified check NULL pointer*/
    
    tmp_ptr = SCI_ALLOC_BASE(length*2 + 1);
    if(tmp_ptr != PNULL)
    {
        SCI_MEMSET(&tmp_ptr[0],0,(length*2 + 1));
        for(i=0; i<length; i++)
        {
            tmp_ptr[i*2+1] = txt_ptr[i];
        }
        //SCI_TRACE_LOW:"atc: tmp %x, %x, %x, %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1379_112_2_17_22_16_56_413,(uint8*)"dddd", tmp_ptr[0], tmp_ptr[1], tmp_ptr[2], tmp_ptr[3]);
        ConvertBinToHex(&tmp_ptr[0], length*2, hex_ptr);
        hex_ptr[length*4] = '\0';
        SCI_FREE(tmp_ptr);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : This function converts the ucs2 hex to ascii
//  Global resource dependence : 
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
BOOLEAN ATC_ConvertUcs2HexToAscii(    
                        uint8        *hex_ptr,     // in: pointer to the CSCS UCS2 format string
                        uint16        length,       // in: the length of ascii string 
                        uint8        *txt_ptr      // out: the ascii format string
                        )
{
    uint16            i = 0;
    uint8 *tmp_ptr = PNULL;
    
    SCI_ASSERT(PNULL != txt_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != hex_ptr); /*assert verified check NULL pointer*/
    
    tmp_ptr = SCI_ALLOC_BASE(length / 2);
    if(tmp_ptr != PNULL)
    {
        SCI_MEMSET(&tmp_ptr[0],0,(length / 2));
        
        ConvertHexToBin(hex_ptr, length, tmp_ptr);
        for(i=0; i<(length/4); i++)
        {
            txt_ptr[i] = tmp_ptr[i*2+1];
        }
        
        //SCI_TRACE_LOW:"atc: tmp %x, %x, %x, %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1419_112_2_17_22_16_56_414,(uint8*)"dddd", txt_ptr[0], txt_ptr[1], txt_ptr[2], txt_ptr[3]);
        txt_ptr[length/4] = '\0';
        SCI_FREE(tmp_ptr);
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : This function translate uint8 array into char for terminal
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
    int32       i    = 0;
    int32       j    = 0;
    uint8 temp   = 0;

    SCI_ASSERT(PNULL != char_ptr);/*assert verified*/

    for (i = 0; i < length; i++)
    { 
        for(j = 0;j < 2;j++)
        {
            if(0 == j)
            {
                temp = (uint8)((*int_ptr & 0xf0) >> 4);
            }
            else
            {
                temp = (uint8)(*int_ptr & 0x0f);
            }
            
            switch (temp)
            {
                case 10:
                {
                    *char_ptr++ = 'A';
                    break;
                }

                case 11:
                {
                    *char_ptr++ = 'B';
                    break;
                }
                
                case 12:
                {
                    *char_ptr++ = 'C';
                    break;
                }
                
                case 13:
                {
                    *char_ptr++ = 'D';
                    break;
                }
                
                case 14:
                {
                    *char_ptr++ = 'E';
                    break;
                }
                
                case 15:
                {
                    *char_ptr++ = 'F';
                    break;
                }
                
                default:
                {
                    *char_ptr++ = (uint8)(temp + '0');
                    break;
                }
            }
        }
        int_ptr++;
    }
}
#endif

/******************************************************************************/
// Description : This function displays the status description
// Global resource dependence : g_atc_status_desc
// Author : Steven.Yao 
// Note : None
/******************************************************************************/
void ATCS_TraceStatusDesc( 
        ATC_STATUS status // the status of ATC processing
        )
{
    //ATC_TRACE:"%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_545_112_2_17_22_16_54_409,(uint8*)"s", g_atc_status_desc[status & 0x00FF] );
}

void IMEITransformBcdToStr(
             uint8                  *bcd_ptr, // in the BCD array
             int32                  length,      // in the length of BCD array
             uint8                  *tel_str     // out the telephone number string
             )
{
    int32        i    = 0;
    int32        j    = 0;
    uint8        temp   = 0;

    SCI_ASSERT(PNULL != tel_str); /*assert verified*/

    for (i = 0; i < length; i++)
    {
        for(j = 0; j < 2; j++)
        {
            if (0 == j)
            {
                if (i == 0)
                {
                    continue;
                }
                temp = (uint8)(*bcd_ptr & 0x0f);
            }
            else
            {
                temp = ((*bcd_ptr & 0xf0) >> 4);
            }

            *tel_str++ = (uint8)(temp + '0');
        }
        bcd_ptr++;
    }
    return;
}

BOOLEAN IMEITransfromStrToBcd(
                uint8             *bcd_ptr,  // out: called BCD number array
                uint8             *length_ptr, // out: the BCD array length
                uint8              *tel_str,     // in:  telephone number string
                int32             str_length   // in: telephone number string length
                )
{
    int32         i = 0;

    SCI_ASSERT(PNULL != length_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != tel_str);/*assert verified*/

    *length_ptr = (uint8)((str_length+1) / 2);

    for (i = 0; i < *length_ptr; i++)
    {
        if (*tel_str < '0' || *tel_str > '9')
        {
            return FALSE;
        }

        if (i == 0)
        {
            bcd_ptr[i] += 0x0a;
            bcd_ptr[i] = (uint8)(bcd_ptr[i] + ((*tel_str - '0') << 4));
            tel_str++;
            continue;
        }
        else
        {
            bcd_ptr[i] = (uint8)(*tel_str - '0');
        }

        tel_str++;
        if (*tel_str < '0' || *tel_str > '9')
        {
            return FALSE;
        }

        bcd_ptr[i] = (uint8)(bcd_ptr[i] + ((*tel_str - '0') << 4));
        tel_str++;
    }
    return TRUE;
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
    int32              i    = 0;
    int32              j    = 0;
    uint8        temp   = 0;

    SCI_ASSERT(PNULL != tel_str); /*assert verified*/

    if(MN_MAX_ADDR_BCD_LEN < length)
    {
        //SCI_TRACE_LOW:"ATC: Assert Failure bcd len."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_566_112_2_17_22_16_54_410,(uint8*)"");
        length = MN_MAX_ADDR_BCD_LEN;
    }

    for (i=0; i<length; i++)
    {
        for(j=0;j<2;j++)
        {
            if( 0 == j )
            {
                temp = (uint8)(*bcd_ptr & 0x0f);
            }
            else
            {
                temp = ((*bcd_ptr & 0xf0) >> 4);
            }
            
            switch (temp)
            {
                case 10:
                {
                    *tel_str++ = '*';
                    break;
                }
                
                case 11:
                {
                    *tel_str++ = '#';
                    break;
                }
                
                case 12:
                {
                    *tel_str++ = 'p';
                    break;
                }
                
                case 13:
                {
                    *tel_str++ = 'w';
                    break;
                }
                
                case 14:
                {
                    *tel_str++ = '+';
                    break;
                }
                
                case 15:
                {
                    break;
                }
                
                default:
                {
                    *tel_str++ = (uint8)(temp + '0');
                    break;
                }
            }
        }
        bcd_ptr++;
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

    SCI_ASSERT(PNULL != length_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != tel_str);/*assert verified*/

    *length_ptr = (uint8)((str_length+1) / 2);

    for(i=0; i < *length_ptr; i++)
    {
        switch (*tel_str)
        {
            case '*':
            {
                bcd_ptr[i] = 0x0A;
                break;
            }
            
            case '#':
            {
                bcd_ptr[i] = 0x0B;
                break;
            }
            
            case 'p':
            case 'P':
            {
                bcd_ptr[i] = 0x0C;
                break;
            }
            
            case 'w':
            case 'W':
            {
                bcd_ptr[i] = 0x0D;
                break;
            }
            
            case '+':
            {
                bcd_ptr[i] = 0x0E;
                break;
            }
            
            default:
            {
                if (*tel_str >= '0' && *tel_str <= '9')
                {
                    bcd_ptr[i] = (uint8)(*tel_str - '0');
                }
                else
                {
                    return FALSE;
                }
                break;
            }
        }

        tel_str++;

        switch (*tel_str)
        {
            case '*':
            {
                bcd_ptr[i] += 0xA0;
                break;
            }
            
            case '#':
            {
                bcd_ptr[i] += 0xB0;
                break;
            }
            
            case 'p':
            case 'P':
            {
                bcd_ptr[i] += 0xC0;
                break;
            }
            
            case 'w':
            case 'W':
            {
                bcd_ptr[i] += 0xD0;
                break;
            }
            
            case '+':
            {
                bcd_ptr[i] += 0xE0;
                break;
            }
            
            case '\0':
            {
                bcd_ptr[i] += 0xF0;
                break;
            }
            
            default:
            {
                if ((*tel_str >= '0') && 
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
        }
        tel_str++;
    }
    return TRUE;
}

#if !defined( _ULTRA_LOW_CODE_) || defined(_ATC_SYNC_TOOL_)
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
    uint8                bcd_len    = 0;
    uint8            *temp_ptr    = NULL;
    BOOLEAN      temp_flag     = FALSE;
    
    temp_ptr = tel_str;
    if (*temp_ptr == '+')
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
//  Description : This function change the ascci into ucs2. 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
BOOLEAN ConvertAscUcs2IntoBin(char *str_ptr,uint8 *ucs2_ptr,uint8 uni_str_len)
{
    //Change the ucs2 char to ucs2
    uint8          i    = 0;
    uint8    temp   = 0;
    
    for(i = 0;i < uni_str_len;i++)
    {
        temp = 0;
        //Handle the first byte
        //0--9
        if(*(str_ptr+i*2) >= '0' && *(str_ptr+i*2) <= '9')
        {
            temp = (*(str_ptr+i*2)-0x30)<<4;
        }
        //A--F
        else if(*(str_ptr+i*2) >= 'A' && *(str_ptr+i*2) <= 'F')
        {
            temp = (*(str_ptr+i*2)-0x41+10)<<4;
        }
        //a--f
        else if(*(str_ptr+i*2) >= 'a' && *(str_ptr+i*2) <= 'f')
        {
            temp = (*(str_ptr+i*2)-0x61+10)<<4;
        }
        else
        {
            return FALSE;
        }

        //handle the second byte.
        //0--9
        if(*(str_ptr+i*2+1) >= '0' && *(str_ptr+i*2+1) <= '9')
        {
            temp += *(str_ptr+i*2+1)-0x30;
        }
        //A--F
        else if(*(str_ptr+i*2+1) >= 'A' && *(str_ptr+i*2+1) <= 'F')
        {
            temp += *(str_ptr+i*2+1)-0x41+10;
        }
        //a--f
        else if(*(str_ptr+i*2+1) >= 'a' && *(str_ptr+i*2+1) <= 'f')
        {
            temp += *(str_ptr+i*2+1)-0x61+10;
        }
        else
        {
            return FALSE;
        }

        *(ucs2_ptr+i) = temp;
    }

    return TRUE;
} 

/*****************************************************************************/
//  Description : This function translate uint8 array into char for terminal
//  Description : This function converts the text to hex
//  Global resource dependence : 
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
void ConvertStr2Hex(    
                        uint8        *txt_ptr,     // in: the IRA format string
                        uint16        length,       // in: the length of binary string 
                        uint8        *hex_ptr      // out: pointer to the hexadecimal format string
                        )
{
    uint16 i=0;
    uint8 tmp_ptr[PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN]={0};
    
    SCI_ASSERT(PNULL != txt_ptr); /*assert verified check NULL pointer*/
    SCI_ASSERT(PNULL != hex_ptr); /*assert verified check NULL pointer*/
    if(length>PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN/2)
        return;
    
    SCI_MEMSET(&tmp_ptr[0],'\0', PHONEBOOK_MAX_DN_ALPHA_IDENTIFY_LEN);
    //SCI_TRACE_LOW:"atc: txt %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1344_112_2_17_22_16_56_411,(uint8*)"dddd", txt_ptr[0], txt_ptr[1], txt_ptr[2], txt_ptr[3]);

    for(i=0; i<length; i++)
    {
        tmp_ptr[i*2+1] = txt_ptr[i];
    }
    
    //SCI_TRACE_LOW:"atc: tmp %x, %x, %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1351_112_2_17_22_16_56_412,(uint8*)"dddd", tmp_ptr[0], tmp_ptr[1], tmp_ptr[2], tmp_ptr[3]);
    ConvertBinToHex(&tmp_ptr[0], length*2, hex_ptr);
}
#endif

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
                uint8       param_len,
                uint8      *cmd_type_ptr
                )
{
    switch (*param_ptr)
    {
        case '?':
        {
            if (1 == param_len)
            {
                *cmd_type_ptr = ATC_CMD_TYPE_READ;
            }
            else
            {
                // invalid type of command
                *cmd_type_ptr = 0xff;
            }
            break;
        }

        case '=':
        {
            if (param_ptr[1] == '?')
            {
                *cmd_type_ptr = ATC_CMD_TYPE_TEST;
            }
            else
            {
                *cmd_type_ptr = ATC_CMD_TYPE_SET;
            }
            break;
        }
        
        default:
        {
            *cmd_type_ptr = 0xff;
            break;
        }
    }
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

    //SCI_TRACE_LOW("ATC: the called number len: %d",calling_number_ptr->num_len);
    if (MN_NUM_PLAN_ISDN_TELE == calling_number_ptr->number_plan)
    {
        switch (calling_number_ptr->number_type)
        {
            case MN_NUM_TYPE_INTERNATIONAL:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                break;
            }
            
            case MN_NUM_TYPE_NATIONAL:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_NATIONAL_TYPE;
                break;
            }
            
            default:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_UNKNOWN_TYPE;
                break;
            }
        }
    }
    else
    {
        *tel_type_ptr = UNKNOWN_PLAN_UNKNOWN_TYPE;
    }
    
    if(calling_number_ptr->num_len > 0)
    {
        *temp_str = 0x80 + ((calling_number_ptr->number_type & 0x07) << 4) + (calling_number_ptr->number_plan & 0x0F);
    }
    else
    {
        *temp_str = 0;
    }
    temp_str++;

    // This telphone number is international number
    if (MN_NUM_TYPE_INTERNATIONAL == calling_number_ptr->number_type)
    {
        *temp_str = '+';
        temp_str++;
    }
    // bcd code to digital string
    TransformBcdToStr((uint8*)calling_number_ptr->party_num, calling_number_ptr->num_len, temp_str);

}

/*****************************************************************************/
//  Description : This function is used to change the called number to string. 
//  Global resource dependence : g_rsp_str
//  Author:       Xueli.lv
/*****************************************************************************/
void ATC_ConvertCalledNumToStr(
                    MN_CALLED_NUMBER_T     *called_number_ptr,
                    ATC_TELE_TYPE_E        *tel_type_ptr,
                    uint8                  *tel_str)
{
    uint8           *temp_str = PNULL;

    temp_str = tel_str;

    if (MN_NUM_PLAN_ISDN_TELE == called_number_ptr->number_plan)
    {
        switch (called_number_ptr->number_type)
        {
            case MN_NUM_TYPE_INTERNATIONAL:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_INTERNATIONAL_TYPE;
                break;
            }
            
            case MN_NUM_TYPE_NATIONAL:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_NATIONAL_TYPE;
                break;
            }
            
            default:
            {
                *tel_type_ptr = ISDN_TELE_PLAN_UNKNOWN_TYPE;
                break;
            }
        }
    }
    else
    {
        *tel_type_ptr = UNKNOWN_PLAN_UNKNOWN_TYPE;
    }
    
    if(called_number_ptr->num_len > 0)
    {
        *temp_str = 0x80 + ((called_number_ptr->number_type & 0x07) << 4) + (called_number_ptr->number_plan & 0x0F);
    }
    else
    {
        *temp_str = 0;
    }
    temp_str++;

    // This telphone number is international number
    if (MN_NUM_TYPE_INTERNATIONAL == called_number_ptr->number_type)
    {
        *temp_str = '+';
        temp_str++;
    }
    // bcd code to digital string
    TransformBcdToStr((uint8*)called_number_ptr->party_num, called_number_ptr->num_len, temp_str);
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
    uint8    *dest_ptr    = bin_ptr;
    int32                 i     = 0;
    uint8               ch    = 0;


    SCI_ASSERT(PNULL != hex_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != bin_ptr);/*assert verified*/

    for (i=0; i<length; i+=2)
    {
        // the bit 8,7,6,5
        ch = hex_ptr[i];
        // digital 0 - 9
        if (ch >= '0' && ch <= '9')
        {
            *dest_ptr =(uint8)((ch - '0') << 4);
        }
        // a - f
        else if (ch >= 'a' && ch <= 'f')
        {
            *dest_ptr = (uint8)((ch - 'a' + 10) << 4);
        }
        // A - F
        else if (ch >= 'A' && ch <= 'F')
        {
            *dest_ptr = (uint8)((ch -'A' + 10) << 4);
        }
        else
        {
            return FALSE;
        }
        
        // the bit 1,2,3,4
        ch = hex_ptr[i+1];
        // digtial 0 - 9 
        if (ch >= '0' && ch <= '9')
        {
            *dest_ptr |= (uint8)(ch - '0');
        }
        // a - f
        else if (ch >= 'a' && ch <= 'f')
        {
            *dest_ptr |= (uint8)(ch - 'a' + 10);
        }
        // A - F
        else if (ch >= 'A' && ch <= 'F')
        {
            *dest_ptr |= (uint8)(ch -'A' + 10); 
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
    uint8    semi_octet    = 0;
    int32                   i    = 0;

    SCI_ASSERT(PNULL != bin_ptr);/*assert verified*/
    SCI_ASSERT(PNULL != hex_ptr);/*assert verified*/

    for (i=0; i<length; i++)
    {
        // get the high 4 bits
        semi_octet = (uint8)((bin_ptr[i] & 0xF0) >> 4);
        if (semi_octet <= 9) //semi_octet >= 0
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
        if (semi_octet <= 9) // semi_octet >= 0
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
//  Description : This function is used to output data to  AT UART
//                profile 
//  Global resource dependence : 
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
BOOLEAN ATC_WriteCmdRsp(
                              uint8 * buf_ptr, //the input point of data to SIO
                              uint32 len       //the length of input data
                              )
{
    uint8 * pstr = NULL;
    xSignalHeaderRec * psig = PNULL;
    
    SCI_ASSERT( buf_ptr != PNULL );/*assert verified*/
    
    if(len > MAX_ATC_RSP_LEN)
    {
        //SCI_TRACE_LOW:"ATC: Assert Failure rsp len."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1529_112_2_17_22_16_56_415,(uint8*)"");
        return FALSE;
    }
    
    pstr = SCI_ALLOC_BASE(len);//free it in AT task
    
    SCI_MEMCPY(pstr, buf_ptr, len);
    //send signal to AT to write uart
    /*lint -e668*/
    SCI_CREATE_SIGNAL(psig,ATC_WRITE_RSP_REQ,sizeof(AT_WRITE_RSP_T),SCI_IDENTIFY_THREAD());
    ((AT_WRITE_RSP_T*)psig)->len = len;
    ((AT_WRITE_RSP_T*)psig)->str = pstr;
    /*lint +e668*/
        
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_ATC);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : get the index of call state according to the call id
//  Global resource dependence : g_calls_state
//  Author: William.Qian
//  Note:
/*****************************************************************************/
uint8 GetIndexOfCallContext(
            uint8 call_id,
             MN_DUAL_SYS_E  dual_sys
            )
{
    uint8                  i    = 0;
    uint8    call_index    = ATC_INVALID_CALLID;
    
    if(call_id >= MN_CALL_MAX_CALL_NUM) 
    {
        //SCI_TRACE_LOW:"ATC: GetIndexOfCallContext out of range, call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ATC_COMMON_1562_112_2_17_22_16_56_416,(uint8*)"d", call_id);
        return call_index;
    }

    for(i=0; i<g_calls_state[dual_sys].call_amount; i++)
    {
        if(call_id == g_calls_state[dual_sys].call_context[i].call_id)
        {
            call_index = i;
            break;
        }
    }
    return call_index;
}

#ifdef _SUPPORT_GPRS_
/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:       King Li
//  Note:
/*****************************************************************************/
BOOLEAN ConvertmEeToBin(    
                        uint8   *mEe_ptr,     // in: the 'mEe' format string
                        uint16  length,       // in: the length of binary string 
                        uint8   *bin_ptr      // out: the binary format string
                        )
{
    int32                 i   = 0;
    int32                 j   = 0;
    BOOLEAN  ret_val   = TRUE;

    SCI_ASSERT(PNULL != bin_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != mEe_ptr); /*assert verified*/
 
    bin_ptr[0] = bin_ptr[1] = 0;

    for (i=0; i < length && ret_val == TRUE; i++)
    {
        if(*mEe_ptr == '"')
        {
            mEe_ptr++;
        }
        else if(*mEe_ptr>='0' && *mEe_ptr <= '9')
        {
            bin_ptr[j] = *mEe_ptr - '0'+10*bin_ptr[j];
            mEe_ptr++;
        }
        else if(*mEe_ptr == 'e' ||*mEe_ptr == 'E' )
        {
            j++;
            mEe_ptr++;
            if(j>1)
                ret_val = FALSE;
        }
        else
        {
            ret_val = FALSE;
        }
    }

    return ret_val;
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
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED;
            break;//MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED
        }
        
        case 1:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_100k;//MN_GPRS_SDU_ERR_RATIO7, /* 1*10-1  */
            break;
        }

        case 2:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_10k;//MN_GPRS_SDU_ERR_RATIO1, /* 1*10-2  */
            break;
        }

        case 3:
        {
            if(ext_qos->sdu_error_ratio[0] >= 7)
            {
                ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_7k;//MN_GPRS_SDU_ERR_RATIO2, /* 7*10-3  */
            }
            else
            {
                ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_1k;//MN_GPRS_SDU_ERR_RATIO3, /* 1*10-3  */
            }
            break;
        }

        case 4:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_100;//MN_GPRS_SDU_ERR_RATIO4, /* 1*10-4  */
            break;
        }

        case 5:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_10;//MN_GPRS_SDU_ERR_RATIO5, /* 1*10-5  */
            break;
        }

        case 6:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_1;//MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
            break;         
        }

        default:
        {
            ext_qos->sdu_error_ratio[0] = MN_GPRS_SDU_ERR_RATIO_SUBSCRIBED;//MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
            break;    
        }
    }

    switch(ext_qos->residual_ber[1])
    {
        case 0:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_SUBSCRIBED;         
            break;//MN_GPRS_BIT_ERROR_RATE_SUBSCRIBED
        }

        case 1:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_5m;//MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
            break;
        }

        case 2:
        {
            if(ext_qos->residual_ber[0] >= 5)    
            {
                ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_5m;//MN_GPRS_BIT_ERROR_RATE1, /* 5*10-2 */
            }
            else
            {
                ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_1m;//MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
            }
            break;
        }

        case 3:
        {
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
        }

        case 4:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_10k;//MN_GPRS_BIT_ERROR_RATE6, /* 1*10-4  */
            break;
        }

        case 5:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_1k;//MN_GPRS_BIT_ERROR_RATE7, /* 1*10-5  */
            break;
        }

        case 6:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_100;//MN_GPRS_BIT_ERROR_RATE8, /* 1*10-6  */
            break;
        }

        case 7:
        case 8:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_6;//MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
            break;
        }

        default:
        {
            ext_qos->residual_ber[0] = MN_GPRS_RESIDUAL_BER_SUBSCRIBED;//MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
            break;    
        }
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
        {
            ext_qos->sdu_error_ratio[1] = 0;
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_10k://MN_GPRS_SDU_ERR_RATIO1, /* 1*10-2  */
        {
            ext_qos->sdu_error_ratio[1] = 2;
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_7k://MN_GPRS_SDU_ERR_RATIO2, /* 7*10-3  */
        {
            ext_qos->sdu_error_ratio[0] = 7;
            ext_qos->sdu_error_ratio[1] = 3;
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_1k://MN_GPRS_SDU_ERR_RATIO3, /* 1*10-3  */
        {
            ext_qos->sdu_error_ratio[0] = 1;
            ext_qos->sdu_error_ratio[1] = 3;
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_100://MN_GPRS_SDU_ERR_RATIO4, /* 1*10-4  */
        {
            ext_qos->sdu_error_ratio[0] = 1;
            ext_qos->sdu_error_ratio[1] = 4; 
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_10://MN_GPRS_SDU_ERR_RATIO5, /* 1*10-5  */
        {
            ext_qos->sdu_error_ratio[0] = 1;
            ext_qos->sdu_error_ratio[1] = 5; 
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_1://MN_GPRS_SDU_ERR_RATIO6, /* 1*10-6  */
        {
            ext_qos->sdu_error_ratio[0] = 1;
            ext_qos->sdu_error_ratio[1] = 6;
            break;
        }

        case MN_GPRS_SDU_ERR_RATIO_100k://MN_GPRS_SDU_ERR_RATIO7, /* 1*10-1  */
        {
            ext_qos->sdu_error_ratio[0] = 1;
            ext_qos->sdu_error_ratio[1] = 1;
            break;
        }

        default:
        {
            //invalid sdu_error_ratio
            ext_qos->sdu_error_ratio[0] = 0xff;
            ext_qos->sdu_error_ratio[1] = 0xff;     
            break;    
        }
    }

    switch(ext_qos->residual_ber[0])
    {
        case 0://MN_GPRS_BIT_ERROR_RATE_SUBSCRIBED
        {
            ext_qos->residual_ber[1] = 0;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_5m://MN_GPRS_BIT_ERROR_RATE1, /* 5*10-2 */
        {
            ext_qos->residual_ber[0] = 5;
            ext_qos->residual_ber[1] = 2;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_1m://MN_GPRS_BIT_ERROR_RATE2, /* 1*10-2  */
        {
            ext_qos->residual_ber[0] = 1;
            ext_qos->residual_ber[1] = 2;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_500k://MN_GPRS_BIT_ERROR_RATE3, /* 5*10-3  */
        {
            ext_qos->residual_ber[0] = 5;
            ext_qos->residual_ber[1] = 3;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_400k://MN_GPRS_BIT_ERROR_RATE4, /* 4*10-3  */
        {
            ext_qos->residual_ber[0] = 4;
            ext_qos->residual_ber[1] = 3;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_100k://MN_GPRS_BIT_ERROR_RATE5, /* 1*10-3  */
            ext_qos->residual_ber[0] = 1;
            ext_qos->residual_ber[1] = 3;
            break;

        case MN_GPRS_RESIDUAL_BER_10k://MN_GPRS_BIT_ERROR_RATE6, /* 1*10-4  */
        {
            ext_qos->residual_ber[0] = 1;
            ext_qos->residual_ber[1] = 4;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_1k://MN_GPRS_BIT_ERROR_RATE7, /* 1*10-5  */
        {
            ext_qos->residual_ber[0] = 1;
            ext_qos->residual_ber[1] = 5;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_100://MN_GPRS_BIT_ERROR_RATE8, /* 1*10-6  */
        {
            ext_qos->residual_ber[0] = 1;
            ext_qos->residual_ber[1] = 6;
            break;
        }

        case MN_GPRS_RESIDUAL_BER_6://MN_GPRS_BIT_ERROR_RATE9, /* 6*10-8  */
        {
            ext_qos->residual_ber[0] = 6;
            ext_qos->residual_ber[1] = 8;
            break;
        }

        default:
        {
            //invalid residual_ber
            ext_qos->residual_ber[0] = 0xff;
            ext_qos->residual_ber[1] = 0xff;
            break;    
        }
    }

    ext_qos->delivery_of_err_sdu = 3 - ext_qos->delivery_of_err_sdu;
}
#endif

#if 0
#ifdef BT_PS_BQB_TEST
/*****************************************************************************/
//  Description : This function handle asynchronous L2CAP after receive
//                asynchronous events
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
void ATC_ProcessL2capTestMsg(ATC_CONFIG_T *atc_config_ptr,xSignalHeaderRec *sig_ptr)
{
    BT_ATC_L2CAP_CACL_CNF_T* bt_sig_ptr =  (BT_ATC_L2CAP_CACL_CNF_T*)sig_ptr;
    
    SCI_TRACE_LOW("atc: ATC_ProcessL2capTestMsg, msg id =0x%x",sig_ptr->SignalCode); 
	
    switch(sig_ptr->SignalCode)
    {
        case ID_STATUS_TEST_L2CAP_CREAT_ACL_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str,"%s: CACL,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	    break;

	 case ID_STATUS_TEST_L2CAP_DIS_ACL_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: DACL,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

	 case ID_STATUS_TEST_L2CAP_REG_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: REG,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

	 case ID_STATUS_TEST_L2CAP_CONN_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: CONN,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

	 case ID_STATUS_TEST_L2CAP_DIS_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: DIS,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

	 case ID_STATUS_TEST_L2CAP_SENDDATA_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: SENDDATA,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

	 case ID_STATUS_TEST_L2CAP_ECHO_CNF:  
            if(bt_sig_ptr->status == BT_SUCCESS)
            {
                sprintf((char*)g_rsp_str, "%s: ECHO,OK",g_atc_info_table[AT_CMD_L2CAP].cmd_name->str_ptr);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);
            }
            else
            {
                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_ERROR ,ERR_UNKNOWN);
            }
    	     break;

    case ID_STATUS_TEST_L2CAP_DATA_IND:
            {
                BT_ATC_L2CAP_DATA_IND_T *sig = (BT_ATC_L2CAP_DATA_IND_T *)sig_ptr;
                SCI_TRACE_LOW("atc:rcv l2cap data len = %d.",sig->data_len);
                sprintf((char*)g_rsp_str, "+L2CAP: DATA , 0x%2x,0x%2x,0x%2x,0x%2x,0x%2x.",sig->data_ptr[0],\
                sig->data_ptr[1],sig->data_ptr[2],sig->data_ptr[3],sig->data_ptr[4]);
                ATC_BuildInfoRsp(atc_config_ptr, g_rsp_str);

                ATC_BuildResultCodeRsp(atc_config_ptr,S_ATC_RESULT_CODE_OK ,0);
            }
            break;
            
	default:
		break;
 
    }
      return;
}
#endif
#endif

/*****************************************************************************/
//  Description: This function handle the AT+L2CAP command. 
//  Global resource dependence : none
//  Author:      
/*****************************************************************************/
AT_CMD_FUNC( ATC_ProcessL2CAP )
{
#if 0
//#ifdef BT_PS_BQB_TEST
    ATC_L2CAP_CHANNEL_MODE_E    channel_mode;
    ATC_L2CAP_TYPE_MODE_E       operation_code;
    uint16                      data_length =0;
    uint8*                      data_ptr = PNULL;
    ATC_STATUS                  re_value = S_ATC_DEFAULT_HANDLE;

    SCI_TRACE_LOW("ATC: ATC_Process L2cap");
    if(ATC_CMD_TYPE_SET != ATC_GET_CMD_TYPE)
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessL2CAP error type");
        return ERR_OPERATION_NOT_SUPPORTED;
    }

    if (PARAM1_FLAG)
    {   
        SCI_TRACE_LOW("ATC:  ATC_ProcessL2CAP with param1");
        
        // get the parameter ACP command type
        operation_code = PARA1.num;    
    
        switch (operation_code)
        {
            // creat  the ACL service
            case CACL:
                L2CAPTest_CreatACL();
                SCI_TRACE_LOW("ACL:Creat ACL sevice success");
    	        ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_CREAT_ACL_CNF,0xff,ATC_NO_DOMAIN);
    	        break;
    
            // disconnect the ACL service
            case DACL:
                L2CAPTest_DisconnectACL();
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_DIS_ACL_CNF,0xff,ATC_NO_DOMAIN);	    
                break;
    
            case REG:
                {
                    if(!PARAM2_FLAG)
                    {
                        SCI_TRACE_LOW("ATC: ATC_Process L2cap error para2 not exist");
                        re_value = ERR_INVALID_INDEX;
                        break;	     
                    }
                    // get the call forwarded operation mode
                    channel_mode = PARA2.num;
    
                    if(PARA2.num > 7 && PARA2.num <1)
                    {
                        SCI_TRACE_LOW("ATC: ATC_Process L2cap error para2");
                        re_value = ERR_INVALID_INDEX;
                        break;	     	     
                    }
    
                    L2CAPTest_Register(channel_mode);
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_REG_CNF,0xff,ATC_NO_DOMAIN);
                    break;
                }
    
            // start  the ACL connect
            case CONN:                     
                L2CAPTest_Connect();
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_CONN_CNF,0xff,ATC_NO_DOMAIN);	     
                break;
        
             // erase the ACL connect
            case DISCONN:       
                {
                    L2CAPTest_DisConnect();
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_DIS_CNF,0xff,ATC_NO_DOMAIN);
                    break;
                }
    
            case SENDDATA:
                {
                    if(!PARAM2_FLAG || !PARAM3_FLAG)
                    {
                        re_value = ERR_INVALID_INDEX;
                        break;	     
                    }
					
                    if(PARAM3_TYPE != ATC_CMD_PARAM_TYPE_STRING
			  && PARA2.num <0)
                    {
                        re_value = ERR_OPERATION_NOT_ALLOWED;
                        break;
                    }
                    
                    SCI_ASSERT(PNULL != PARA3.str_ptr->str_ptr);/*assert verified*/
		      data_length = PARA2.num;
                    data_ptr = PARA3.str_ptr->str_ptr;			
    
                    L2CAPTest_SendData(data_length,data_ptr);
                    ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_SENDDATA_CNF,0xff,ATC_NO_DOMAIN);
                    break;
                }
    
            //Í¨¹ýL2CAP·¢ËÍecho
            case ECHO:
                L2CAPTest_Echo();
                ATC_ADD_EXPECTED_EVENT(atc_config_ptr,ID_STATUS_TEST_L2CAP_ECHO_CNF,0xff,ATC_NO_DOMAIN);	     
                break;
    
            default:
                break;
        }
    }
    else
    {
        SCI_TRACE_LOW("ATC: ATC_ProcessL2CAP error no param1");
        return ERR_INVALID_INDEX; 
    }

    return re_value;
#else
    return S_ATC_SUCCESS;
#endif
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
