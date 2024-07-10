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

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/20/2002     Steven.Yao       Created.                                  *
 ** 06/20/2002     Ivan.Yin         start to edit and modity                  *  
 ******************************************************************************/

#ifndef _ATC_COMMON_H
#define _ATC_COMMON_H
/*******************************************************************************
 **                        ATC Header Files                                    *
 ******************************************************************************/
#include "atc.h"


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
#define ATC_WIND_BIT10    0XFBFF // 1111101111111111
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
                }xSignalHeaderRec;
        
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
typedef struct
{
    uint8        param_len;
    uint8        param[ATC_PWD_MAX_LEN];
}ATC_CPWD_PARAM_T;

#define ATC_CRSM_DATA_MAE_LEN 256

typedef struct
{
    uint16        param_len;
    uint8        param[ATC_CRSM_DATA_MAE_LEN];
}ATC_CRSM_PARAM_T;

#define ATC_CID_MAX_LEN 2

typedef struct
{
    uint8        param_len;
    uint8        param[ATC_CID_MAX_LEN];
}ATC_CGCMOD_PARAM_T;

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
    AT_CLCK_MAX
};
#define ATCS_TRACE_STATUS ATCS_TraceStatusDesc

void ATCC_WriteCmdRes(char * buf, uint len );

/******************************************************************************/
// Description : This function formats the clck response.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
extern void ATC_FormatCpwdStr(
                    uint8        *pwd_ptr
                    );
                    
#ifndef _ULTRA_LOW_CODE_
/******************************************************************************/
// Description : This function formats the clck read command response string.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
/******************************************************************************/
extern void ATC_FormatClckReadRsp(
                    uint8    *rsp_str,
                    uint8    sc_state
                    );
#endif
                    
/******************************************************************************/
// Description : This function formats the clck test command response string.
// Global resource dependence : 
// Author : Xueli.lv 
// Note : None
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
             uint8                  *bcd_ptr, // in the BCD array
             int32                  length,      // in the length of BCD array
             uint8                  *tel_str     // out the telephone number string
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
                uint8       param_len,
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
                ATC_CPWD_PARAM_T *param_ptr
                );
                
#ifndef _ULTRA_LOW_CODE_
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
                );

/*****************************************************************************/
//  Description : This function translate uint8 array into char for terminal
//  Description : This function converts the text to hex
//  Global resource dependence : 
//  Author:       Feng.shi
//  Note:
/*****************************************************************************/
extern void ConvertStr2Hex(    
                        uint8        *txt_ptr,     // in: the IRA format string
                        uint16        length,       // in: the length of binary string 
                        uint8        *hex_ptr      // out: pointer to the hexadecimal format string
                        );

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
                        );

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

#endif
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
                );


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
                    MN_CALLED_NUMBER_T     *called_number_ptr,
                    ATC_TELE_TYPE_E        *tel_type_ptr,
                    uint8                  *tel_str);
                    
/*****************************************************************************/
//  Description : This function change the ascci into ucs2. 
//  Global resource dependence : g_rsp_str
//  Author:       Liuhan
/*****************************************************************************/
extern BOOLEAN ConvertAscUcs2IntoBin(char *str_ptr,uint8 *ucs2_ptr,uint8 uni_str_len);


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
//  Description : This function is used to output data to  AT UART
//                profile 
//  Global resource dependence : 
//  Author:       Shijun.cui
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_WriteCmdRsp(
                        uint8 * buf_ptr, //the input point of data to SIO
                        uint32  len      //the length of input data
                        );                                                                                                                                                                                                                                                                     

/*****************************************************************************/
//  Description : get the index of call state according to the call id
//  Global resource dependence : g_calls_state
//  Author: William.Qian
//  Note:
/*****************************************************************************/
extern uint8 GetIndexOfCallContext(
            uint8 call_id,
            MN_DUAL_SYS_E  dual_sys
            );

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:       King.Li
//  Note:
/*****************************************************************************/
extern void ConvertAtQosToL4Qos(
                         MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos
                         );

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:       King Li
//  Note:
/*****************************************************************************/
extern BOOLEAN ConvertmEeToBin(    
                        uint8   *mEe_ptr,     // in: the 'mEe' format string
                        uint16  length,       // in: the length of binary string 
                        uint8   *bin_ptr      // out: the binary format string
                        );

/*****************************************************************************/
//  Description : Description : This function converts the  format mEe to bin
//  Global resource dependence : 
//  Author:      King Li
//  Note:
/*****************************************************************************/
extern void ConvertL4QosToAtQos(
                         MNGPRS_3G_EXTEND_QOS_SET_T *ext_qos
                         );

/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
extern uint16 ATC_Default2Ascii(                          //RETURN:
                         uint8      *default_ptr,   //IN:
                         uint8      *ascii_ptr,     //OUT:
                         uint16     str_len         //IN:
                         );

/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
extern uint16 ATC_Ascii2default(                          //RETURN: the length of default string
                         uint8      *ascii_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len         //IN:
                         );

/*****************************************************************************/
//  Description : This function converts the ascii to ucs2 hex
//  Global resource dependence : 
//  Author:       Sunsome Ju
//  Note:
/*****************************************************************************/
extern BOOLEAN ATC_ConvertAsciiToUcs2Hex(    
                        uint8        *txt_ptr,     // in: the ascii format string
                        uint16        length,       // in: the length of binary string 
                        uint8        *hex_ptr      // out: pointer to the CSCS UCS2 format string
                        );

#ifdef BLUETOOTH_SUPPORT
PUBLIC void ATC_SendTaskCmd(char* input);
#endif
#endif /* End of #ifndef _ATC_COMMON_H */
