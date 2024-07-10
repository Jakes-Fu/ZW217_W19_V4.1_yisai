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

#ifndef _ULTRA_LOW_CODE_
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

static const uint32 crc_ta[256]={ /* CRC ”‡ Ω±Ì */

        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
        0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
        0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
        0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
        0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
        0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
        0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
        0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
        0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
        0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
        0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
        0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
        0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
        0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
    };
    
#define  ACK  0x00
#define  NAK  0x01
#define  DATA 0x02

static const uint8 szNextFrameCmd[]  = {0x7E,0x00,0x7E};
static const uint8 szErrorFrameCmd[] = {0x7E,0x01,0x7E};

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

//extern const AUD_RING_DATA_INFO_T    g_atc_midi_table[]; //@Shijun.cui 2005-07-12 CR27598
extern ATC_CALLS_STATE_T     g_calls_state[MN_SYS_NUMBER];

#ifndef _ULTRA_LOW_CODE_
/******************************************************************************/
// Description : This function formats the clck response.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
extern void ATC_FormatCpwdStr(
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
extern void ATC_FormatClckReadRsp(
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
extern void ATC_FormatClckTestRsp(
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

#ifndef _ULTRA_LOW_CODE_
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

/*****************************************************************************/
//  Description : This function transform the BCD number to telephone string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
 extern  void TransformBcdToStr(
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
extern BOOLEAN TransfromStrToBcd(
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

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function convert telephone number string to called number
//                structure. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertTeleStrToStruct(
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
//  Description : This function is used to get the parameters (AT+CPIN,AT+CPWD,
//                AT+CLCK)
//     
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetParams(
                uint8            *in_ptr,
                uint8             in_len,
                ATC_CPWD_PARAM_T *param_ptr
                )
{
    int32              i    = 0;
    int32              j    = 0;
    uint16            k    = 0;
    BOOLEAN    flag    = TRUE;

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
            if (j < ATC_PWD_MAX_LEN)
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
//  Description : This function is used to get the facility type for the security
//  commands.     
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetFacType(
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
extern BOOLEAN ATC_GetClckFacType(
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
//  Description : This function is used to parse the DTMF string. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
#define AT_VTS_CMD_LEN     6
extern BOOLEAN ATC_ParseDtmfChar(
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

/*****************************************************************************/
//  Description : This function is used to check the pin string. 
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN  ATC_CheckPinStr(
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

/*****************************************************************************/
//  Description : This function is used to get the type commands(AT+CPIN,AT+CPWD,
//                AT+CLCK)
//     
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern void ATC_GetCmdType(
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
extern void ATC_ConvertCallingNumToStr(
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
extern void ATC_ConvertCalledNumToStr(
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

#ifndef _ULTRA_LOW_CODE_
/*****************************************************************************/
//  Description : This function change the ascci into ucs2. 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
extern BOOLEAN ConvertAscUcs2IntoBin(char *str_ptr,uint8 *ucs2_ptr,uint8 uni_str_len)
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
#endif

/*****************************************************************************/
//  Description : This function converts the hex to bin
//  Global resource dependence : 
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern  BOOLEAN   ConvertHexToBin(    // FALSE: There is invalid char
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
extern  void    ConvertBinToHex(    
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

#ifndef _ULTRA_LOW_CODE_
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
extern void ATC_TranUint8IntoChar(                               
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

/*****************************************************************************/
// Description : This function convert the bcd code to digital string
// Global resource dependence : none
// Author:       Xueli Lv
// Note:
/*****************************************************************************/
void ConvertBcdToDigitalStr(
                uint8         length,
                uint8         *bcd_ptr,      // in: the bcd code string
                uint8         *digital_ptr   // out: the digital string
                )
{
    int32               i = 0;
    uint8         temp = 0;

    // get the first digital
    temp = ((*bcd_ptr >> 4) &0x0f);
    if (temp >= 0x0a)
    {
        *digital_ptr = (temp - 0x0a) + 'A';
    }
    else
    {
        *digital_ptr = temp + '0';
    }
    
    bcd_ptr++;
    digital_ptr++;


    for (i=0; i<(length - 1); i++)
    {
        temp = *bcd_ptr;
        // get the low 4 bits
        temp &= 0x0f;
        // A -- F
        if (temp >= 0x0a)
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
        
        if ((temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';    
            return;
        }
        // A -- F
        else if (temp>=0x0a)
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
//  Description : Audio dummy notify callback function.
//  Global resource dependence : none
//  Author: shujing.dong
//  Note: 
/*****************************************************************************/
void AudioDummyNotifyCallback(
                HAUDIO hAudio, 
                uint32 notify_info, 
                uint32 affix_info
                )
{
    //do nothing
}

/*****************************************************************************/
//  Description : This function is used to output data to  AT UART
//                profile 
//  Global resource dependence : 
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_WriteCmdRsp(
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
