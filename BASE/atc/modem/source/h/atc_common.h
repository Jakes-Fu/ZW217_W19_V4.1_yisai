/******************************************************************************
 ** File Name:      atc_common.h                                              *
 ** Author:         Steven.Yao                                                *
 ** DATE:           05/20/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains types and MICROS that are of use to all*
 **                 other ATC modules.                                        *
 ******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** -------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                                *
 ** 05/20/2002     Steven.Yao       Created.                                   *
 ** 06/20/2002     Ivan.Yin         start to edit and modity                    *
******************************************************************************/

#ifndef _ATC_COMMON_H
#define _ATC_COMMON_H
/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc.h"

#define ATC_TRACE_LOW     SCI_TRACE_LOW

#define       AT_CPWD_FAC_LEN       2
#define       AT_DTMF_BUF_LEN    20

//define the bit flow used in at+cwind
#define ATC_WIND_BIT0      0XFFFE // 1111111111111110
#define ATC_WIND_BIT1      0XFFFD // 1111111111111101
#define ATC_WIND_BIT2      0XFFFB // 1111111111111011
#define ATC_WIND_BIT3      0XFFF7 // 1111111111110111
#define ATC_WIND_BIT4      0XFFEF // 1111111111101111
#define ATC_WIND_BIT5      0XFFDF // 1111111111011111
#define ATC_WIND_BIT6      0XFFBF // 1111111110111111
#define ATC_WIND_BIT7      0XFF7F // 1111111101111111
#define ATC_WIND_BIT8      0XFEFF // 1111111011111111
#define ATC_WIND_BIT9      0XFDFF // 1111110111111111
#define ATC_16BIT1         0XFFFF // 1111111111111111

#ifndef _RTOS
#ifdef WIN32
#include <stdlib.h>
#include <memory.h>
#include <stdarg.h>

#include <windows.h>

#define  SCI_CREATE_SIGNAL
#define SCI_SEND_SIGNAL
#define APP_MN_SET_PDP_CONTEXT_CNF 100
#define APP_MN_ACTIVATE_PDP_CONTEXT_CNF 101
#define SIO_RECV_NEW_AT     1
#define SCI_RECEIVE_SIGNAL(X) 1
#define SIO_ATC_ReadCmdLine
#define SIO_ATC_WriteCmdRes   ATCC_WriteCmdRes
typedef struct
{
    unsigned short      SignalCode;
    unsigned short      SignalSize;
    struct MS_HEAD_tag  *Pre;
    struct MS_HEAD_tag  *Succ;
    unsigned long       Sender;
} xSignalHeaderRec;

void ATCC_trace(const char *, ...);

#define ATC_TRACE  \
    ATCC_trace("Module ATC : ");   \
    ATCC_trace

#ifdef _DEBUG

#define ATCC_LOG_CUR_ATC_INFO( _X ) \
    ATCC_LogCurATCInfo( _X );

#else

#define ATCC_LOG_CUR_ATC_INFO( _X )

#endif /* End of #ifdef _DEBUG */
#endif /* End of #ifdef WIN32 */
#else
#define ATC_TRACE     SCI_TRACE_LOW

#define ATCC_LOG_CUR_ATC_INFO( _X ) \
    ATCC_LogCurATCInfo( _X );


#endif

void ATCS_TraceStatusDesc(
    ATC_STATUS status // the status of ATC processing
);
typedef struct
{
    uint8       number;
    char        *fac_ptr;
} ATC_CPWD_STR_T;
#define ATC_PWD_MAX_LEN       8
#define ATC_CRSM_DATA_MAE_LEN /*256*/ 512
#define CMD_PARAM_MAX_LEN       256
typedef struct
{
    uint8        param_len;
    uint8        param[ATC_PWD_MAX_LEN];
} ATC_CPWD_PARAM_T;

#define ATC_SPBBTTEST_MAX_LEN 50
typedef struct
{
    uint8        param_len;
    uint8        param[ATC_SPBBTTEST_MAX_LEN];
}ATC_SPBTTEST_PARAM_T;

typedef struct
{
    uint16        param_len;
    uint8          param[ATC_CRSM_DATA_MAE_LEN];
} ATC_CRSM_PARAM_T;

typedef struct
{
    uint16          param_len;                      //param length(total) in AT Cmd string
    uint16          param_data_len;                 //param data length(real)
    uint8               param[CMD_PARAM_MAX_LEN];       //store param data
} ATC_CMD_PARAM_T;

typedef struct{
	char cmd[40];
	unsigned int map;
} AT_CMD_LIST_T;

enum
{
    AT_PWD_PS,
    AT_PWD_SC,
    AT_PWD_AO,
    AT_PWD_OI,
    AT_PWD_OX,
    AT_PWD_AI,
    AT_PWD_IR,
    AT_PWD_AB,
    AT_PWD_AG,
    AT_PWD_AC,
    AT_PWD_P2,
    AT_PWD_FD,
    AT_PWD_PN,

    AT_PWD_PU,
    AT_PWD_PP,
    AT_PWD_PC,

    AT_PWD_MAX
};

enum
{
    AT_CLCK_PS,
    AT_CLCK_SC,
    AT_CLCK_AO,
    AT_CLCK_OI,
    AT_CLCK_OX,
    AT_CLCK_AI,
    AT_CLCK_IR,
    AT_CLCK_AB,
    AT_CLCK_AG,
    AT_CLCK_AC,
    AT_CLCK_PN,
    AT_CLCK_FD,

    AT_CLCK_PU,
    AT_CLCK_PP,
    AT_CLCK_PC,

    AT_CLCK_MAX
};
#define ATCS_TRACE_STATUS ATCS_TraceStatusDesc

void ATCC_WriteCmdRes(char *buf, uint len);

/******************************************************************************/
//  Description : This function formats the clck response.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
extern void ATC_FormatCpwdStr(
    uint8        *pwd_ptr
);

/******************************************************************************/
//  Description : This function formats the clck read command response string.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
extern void ATC_FormatClckReadRsp(
    uint8    *rsp_str,
    uint8    sc_state
);

/******************************************************************************/
//  Description : This function formats the clck test command response string.
//  Global resource dependence :
//  Author : Xueli.lv
//  Note : None
/******************************************************************************/
extern void ATC_FormatClckTestRsp(
    uint8      *rsp_str
);


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
);

/*****************************************************************************/
//  Description : This function transform the BCD number to telephone string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern  void ATC_TransformBcdToStr(
    uint8       *bcd_ptr,         // in the BCD array
    int32        bcd_length,      // in the length of BCD array
    uint8        *tel_str,        // out the telephone number string
    uint16       *tel_str_len_ptr  // out, the telephone number length
);

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
);
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
);

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
    uint16        param_len,
    uint8      *cmd_type_ptr
);

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
    ATC_CPWD_PARAM_T *param_ptr,
    uint8       param_max_count
);

/*****************************************************************************/
//  Description : This function is used to get the parameters of AT set Command
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetCommandParams(
    uint8            *in_ptr,                       //in: param string
    uint16              in_len,                     //in: param length
    uint8       param_max_count,                   //in: the max params num of the cmd
    ATC_CMD_PARAM_T *cmd_info_ptr,          //out: cmd info
    uint8       *param_count,               //out:param count
    uint16          *offset_len_ptr,                //out: parse offset length
    BOOLEAN     *other_cmd_flag_ptr         //out: there are other AT cmds included or not
);

/*****************************************************************************/
//  Description : This function is used convert string data to number
//  Global resource dependence : none
//  Author:       minqian qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ConvertStringtoUint16(uint8 *str_ptr, uint16 length, uint16 *num_ptr);

/*****************************************************************************/
//  Description : This function is used to get the type commands(AT+ECIND)
//
//  Global resource dependence : none
//  Author:       Bin LI
//  Note:
/*****************************************************************************/
extern void ATC_GetUiccInfo(
    MN_DUAL_SYS_E dual_sys,
    ATC_CONFIG_T *atc_config_ptr,
    uint8      *sim_status_ptr,
    uint8       *pin_status_ptr,
    MN_CARD_TYPE_E      *card_type_ptr
);
/*****************************************************************************/
//  Description : This function is used to get the facility type for the security
//      commands.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetFacType(
    uint8        *param_ptr,
    uint8         param_len,
    uint8        *fac_type_ptr
);

/*****************************************************************************/
//  Description : This function is used to get the facility type for the AT+CLCK
//      command.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_GetClckFacType(
    uint8        *param_ptr,
    uint8         param_len,
    uint8        *fac_type_ptr);

/*****************************************************************************/
//  Description : This function is used to parse the DTMF string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ParseDtmfChar(
    uint8      *param_ptr,
    uint8       param_len,
    uint8      *dtmf_ptr
);

/*****************************************************************************/
//  Description : This function is used to check the pin string.
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern BOOLEAN  ATC_CheckPinStr(
    uint8              *pin_str,
    uint8               length
);

/*****************************************************************************/
//  Description : This function is used to change the calling number to string.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
extern void ATC_ConvertCallingNumToStr(
    MN_CALLING_NUMBER_T    *calling_number_ptr,
    ATC_TELE_TYPE_E        *tel_type_ptr,
    uint8                  *tel_str
);

/*****************************************************************************/
//  Description : This function is used to change the called number to string.
//  Global resource dependence : g_rsp_str
//  Author:       Xueli.lv
/*****************************************************************************/
extern void ATC_ConvertCalledNumToStr(
    MN_CALLED_NUMBER_T     *called_number_ptr,  //input, call number structure
    ATC_TELE_TYPE_E        *tel_type_ptr,       //output,telephone number type
    uint8                  *tel_str_ptr,        //output,telephone number string
    uint16                 *tel_str_len_ptr     //output, telephone number length
);

/*****************************************************************************/
//  Description : This function change the ascci into ucs2.
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
extern BOOLEAN ConvertAscUcs2IntoBin(char *str_ptr, uint8 *ucs2_ptr, uint8 uni_str_len);


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
);

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
);
/*****************************************************************************/
//  Description : This function converts the  format mEe to bin
//  Global resource dependence :
//  Author:       Wang hui
//  Note:
/*****************************************************************************/
extern  BOOLEAN    ConvertmEeToBin(
    uint8        *mEe_ptr,     // in: the 'mEe' format string
    uint16        length,       // in: the length of binary string
    uint8        *bin_ptr      // out: the binary format string
);

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Author:       Wang hui
//  Description : Description : This function converts the  format mEe to bin
//  Author:       Wang hui
//  Description : Description : This function convert 'AtPsRate' to 'L4PsRate'
//  Author:       Pizer.Fan
//  Description : Description : This function convert 'L4PsRate' to 'AtPsRate'
//  Author:       Pizer.Fan
//  Description : This function traslate uint8 array into char for terminal
//                profile
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
extern void ATC_TranUint8IntoChar(
    uint8 *int_ptr,  //the input pointer of ucs2
    uint16 length,    //the length of ucs2
    uint8 *char_ptr   //the output pointer of ascii
);

/*****************************************************************************/
//  Description : This function traslate uint8 array into char for terminal
//                profile
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
//  Note:
/*****************************************************************************/
ATC_STATUS Atc_PlayMidi(ATC_CONFIG_T  *atc_config_ptr,
                        uint8         index,
                        uint32        times
                       );

/*****************************************************************************/
//  Description : This function is used to output data to  AT UART
//                profile
//  Global resource dependence :
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_WriteCmdRsp(
    MN_DUAL_SYS_E  dual_sys,
    uint8 *buf_ptr,  //the input point of data to SIO
    uint32  len      //the length of input data
);

/*****************************************************************************/
//  Description : get the index of call state according to the call id
//  Global resource dependence : g_calls_state
//  Author: William.Qian
//  Note:
/*****************************************************************************/
extern uint8 GetIndexOfCallContext(
    uint8           call_id,
    MN_DUAL_SYS_E   dual_sys
);

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
);

/******************************************************************************/
//  Description : This function get ATC_REG_NETWORK_ACT_E by MN_PHONE_CURRENT_PLMN_INFO_T
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
ATC_REG_NETWORK_ACT_E ATC_GetRegNwAct(
    ATC_CONFIG_T                      *atc_config_ptr,
    MN_PHONE_CURRENT_PLMN_INFO_T *register_status_ptr
);

/******************************************************************************/
//  Description : This function get converted rssi value for +CSQ
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
uint8 ATC_GetConvertedRSSIVlaueForCSQ(uint8 receivedrssi);

/******************************************************************************/
//  Description : This function get converted rssi value for +ECSQ
//  Global resource dependence : None
//  Author : minqian.qian
//  Note : None
/******************************************************************************/
uint8 ATC_GetConvertedRSSIVlaueForECSQ(uint8 receivedrssi);

/*****************************************************************************/
//  Description : This function converts ucs2 string to hexadecimal string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertUCS2ToHEX(
    uint8      *ucs2_str_ptr,    //in, points to the ucs2 string
    uint16      ucs2_str_len,    //in, ucs2 string length
    uint8      *hex_str_ptr,    //out, points to buffer used for storing the hex string
    uint16     *hex_str_len_ptr   //in, maximum length of buffer which pointed by hex_str_ptr
    //out, the length of the hex string converted from ucs2 string
);
/*****************************************************************************/
//  Description : This function converts hexadecimal string to ucs2 string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertHEXToUCS2(
    uint8      *hex_str_ptr,    //in, points to the hex string
    uint16      hex_str_len,    //in, hex string length
    uint8      *ucs2_str_ptr,    //out, points to buffer used for storing the ucs2 string
    uint16     *ucs2_str_len_ptr   //in, maximum length of buffer which pointed by ucs2_str_ptr
    //out, the length of the ucs2 string converted from ira string
);
/*****************************************************************************/
//  Description : This function converts ucs2 string to ira string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertUCS2ToIRA(
    uint8      *ucs2_str_ptr,    //in, points to the ucs2 string
    uint16      ucs2_str_len,    //in, ucs2 string length
    uint8      *ira_str_ptr,    //out, points to buffer used for storing the ira string
    uint16     *ira_str_len_ptr   //in, maximum length of buffer which pointed by ira_str_ptr
    //out, the length of the ira string converted from ucs2 string
);
/*****************************************************************************/
//  Description : This function converts ira string to ucs2 string
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertIRAToUCS2(
    uint8      *ira_str_ptr,    //in, points to the ira string
    uint16      ira_str_len,    //in, ira string length
    uint8      *ucs2_str_ptr,    //out, points to buffer used for storing the ucs2 string
    uint16     *ucs2_str_len_ptr   //in, maximum length of buffer which pointed by ucs2_str_ptr
    //out, the length of the ucs2 string converted from ira string
);
/*****************************************************************************/
//  Description : This function transform IRA character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_TransformIRAToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
);
/*****************************************************************************/
//  Description : This function transform GSM character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_TransformGSMToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
);
/*****************************************************************************/
//  Description : This function transform HEX character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_TransformHEXToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
);
/*****************************************************************************/
//  Description : This function transform UCS2 character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_TransformUCS2ToOtherChset(
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
);
/*****************************************************************************/
//  Description : This function transform one character set string to other
//                 character set string.
//                 it is used for AT command which is related to +CSCS command.
//  Global resource dependence : none
//  Author:   minqian.qian
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ChsetTransformation(
    ATC_CHARACTER_SET_TYPE_E     src_chset_type, //in, the origination character set type
    uint8    *src_str_ptr,           //in, the origination character set string
    uint16    src_str_length,        //in, the origination character set string length
    ATC_CHARACTER_SET_TYPE_E     dest_chset_type,//in, the destination character set type
    uint16    dest_str_max_len,   //in, the destination character set string maximum length
    uint8     *dest_str_ptr,      //out, the destination character set string
    uint16    *dest_str_len_ptr   //out, the destination character set string length
);

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:       King.Li
//  Note:
/*****************************************************************************/
void ConvertAtQosToL4Qos(
                         MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos
                         );

/*****************************************************************************/
//  Description : Judge whether the call is disconnect by AP/Modem,the call may be disconnect by Modem
//  for the stk max redial duration is exceeded
//  Global resource dependence : none
//  Author:       Yongxia zhang
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_IsSelfDisc(uint8 call_id);

/*****************************************************************************/
//  Description : This function convert the bcd code to digital string
//  Global resource dependence : none
//  Author:       Xueli Lv
//  Note:
/*****************************************************************************/
extern void ConvertBcdToDigitalStr(
    uint8         length,
    uint8         *bcd_ptr,      // in: the bcd code string
    uint8         *digital_ptr   // out: the digital string
);
/*****************************************************************************/
//  Description : This function transform imei string to the BCD number.
//  Global resource dependence : none
//  Author:       
//  Note:
/*****************************************************************************/
extern  BOOLEAN   TransfromImeiStrToBcd(    // FALSE: There is invalid char
    uint8        *imei_ptr,     // in: the imei format string
    uint8        max_imei_len, // in
    uint8        max_dest_len,       // in: the length of imei string
    uint8        *dest_ptr      // out: pointer to the bcd format string
);

unsigned int ATC_GetMap(char* buf);
	
BOOLEAN ATC_CheckNum(char *buf, int len);

BOOLEAN ATC_StrCheck(char *buf);

#endif /* End of #ifndef _ATC_COMMON_H */
