/****************************************************************************
** File Name:      mmicom_string.h                                         *
** Author:                                                                 *
** Date:           1/4/2008                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, applicaiton, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2008        jianma       Create
** 
****************************************************************************/
#ifndef MMICOM_STRING_H
#define MMICOM_STRING_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_font.h"
#include "mmk_type.h"
#include "caf.h"
#include "mn_type.h"
#include "mmi_string.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#define     MMICOM_81STR_OFFSET 3
#define     MMICOM_82STR_OFFSET 4

#define MMICOM_URL_WTAI_TEL_HEADER                          "tel:"
#define MMICOM_URL_WTAI_WP_MC_EMPTY_HEADER        "wtai://wp/mc"
#define MMICOM_URL_WTAI_WP_MC_HEADER                    "wtai://wp/mc;"
#define MMICOM_URL_WTAI_WP_SD_HEADER                    "wtai://wp/sd;"
#define MMICOM_URL_WTAI_WP_AP_EMPTY_HEADER        "wtai://wp/ap"
#define MMICOM_URL_WTAI_WP_AP_HEADER                    "wtai://wp/ap;"
#define MMICOM_URL_WTAI_SMS_HEADER                          "sms:"
#define MMICOM_URL_WTAI_SMS_TO_HEADER                   "smsto:"
#define MMICOM_URL_WTAI_MMS_HEADER                          "mms:"
#define MMICOM_URL_WTAI_MMS_TO_HEADER                   "mmsto:"
#define MMICOM_URL_WTAI_MAIL_HEADER                         "mail:"
#define MMICOM_URL_WTAI_MAIL_TO_HEADER                  "mailto:"
#define MMICOM_URL_HTTP_HEADER                                  "http://"
#define MMICOM_URL_HTTPS_HEADER                                 "https://"
#define MMICOM_URL_RTSP_HEADER                                  "rtsp://"
#define MMICOM_URL_MMS_HEADER                                   "mms://"
#define MMICOM_URL_FTP_HEADER                                   "ftp://"
#define MMICOM_URL_TELENT_HEADER                            "telnet://"
#define MMICOM_URL_FILE_HEADER                                  "file:///"
#define MMICOM_URL_ABOUT_BLANK                                  "about:blank"

/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : operate the ascii character as the character in default ext table 
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_AsciiAsDefaultExt(
                                           uint8    ascii_char,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           );

/*****************************************************************************/
//  Description : Count default extension character
//  Global resource dependence : none
//  Author: Tracy Zhang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_CountDefaultExt(                            //RETURN: the num of default extension character
                           wchar    *ascii_ptr,     //IN:
                           uint16   str_len         //IN:
                           );

/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Default2Ascii(                          //RETURN:
                                      uint8     *default_ptr,   //IN:
                                      uint8      *ascii_ptr,        //OUT:
                                      uint16     str_len            //IN:
                                      );

/*****************************************************************************/
//  Description : Change from 03.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_DefaultToWstr(                          //RETURN:
                                      uint8     *default_ptr,   //IN:
                                      wchar      *ascii_ptr,        //OUT:
                                      uint16     str_len            //IN:
                                      );

/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 03.38 alphabet table
//  Global resource dependence : none
//  Author: sunsome.ju
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Ascii2default(                          //RETURN: the length of default string
                         uint8      *ascii_ptr,     //IN:
                         uint8      *default_ptr,   //OUT:
                         uint16     str_len         //IN:
                         );

/*****************************************************************************/
// 	Description : encode alphabet under sim standard
//	Global resource dependence : none
//  Author:baokun
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPICOM_EncodeGSMAlphabet(
	PHONEBOOK_ALPHABET_TYPE_E	alpha_type,
	uint8						input_len,
    uint8	   					*input_ptr,
    uint16						*output_len,
    uint8						*output_ptr,
    uint16                      max_out_len
);

/*****************************************************************************
Description : encode alphabet under sim standard
Global resource dependence : none
Author: baokun.yin
Note:
*****************************************************************************/
BOOLEAN MMIAPICOM_DecodeGSMAlphabet(
									PHONEBOOK_ALPHABET_TYPE_E	*alpha_type,//OUT
									uint16						input_len,//IN
									uint8	   					*input_ptr,//IN
									uint16                      output_max_len,//IN
									uint16						*output_len,//OUT
									uint8						*output_ptr//OUT
									);

/*****************************************************************************/
// 	Description :  alphabet under sim standard encode len
//	Global resource dependence : none
//  Author:baokun
//	Note:
/*****************************************************************************/
uint16 MMIAPICOM_GetUCS2MaxEncodeLen(
	PHONEBOOK_ALPHABET_TYPE_E	alpha_type,
	uint8                       alpha_max_len,
	uint8						wchar_len,
    wchar	   					*wchar_ptr
 );

/*****************************************************************************/
// Description : when the digit is "xx." ,change it to be "xx"
// Global resource dependence : 
// Author:   zhaohui
// Note:   
/*****************************************************************************/
PUBLIC void MMIAPICOM_CheckDecimalPoint(uint8 *buf);

/*******************************************************************************************/
//  Description : parse sim string
//  Global resource dependence :
//  Author: wancan.you
//  Note:   
/*********************************************************************************************/
PUBLIC void MMIAPICOM_ParseSimStr(uint8* sim_str,//IN:
                                            uint32 sim_str_len,//IN:
                                            MMI_STRING_T* mmi_str,//OUT:
                                            uint32 mmi_buf_len//IN:
                                            );

/*****************************************************************************/
// 	Description : convert SIM str to the real phone entry alpha
//	Global resource dependence : none
//  Author: dave.ruan
//	Note: OUT len and data
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_ConvSIMStr2PhoneAlpha(
										   uint8	*alpha_ptr,		//IN/OUT:
										   uint8	*alpha_len_ptr,	//IN/OUT:
										   uint8	max_len	//IN/OUT:
										   ); 

//  Description : operate the wchar as the character in default ext table 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WcharAsDefaultExt(
                                           wchar    data_in,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           );
/*****************************************************************************/
//  Description : operate the wchar as the character in default greek 
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_WcharAsDefaultGreek(
                                           wchar    data_in,         //IN:
                                           uint8    *default_char_ptr   //OUT:
                                           );
/*****************************************************************************/
//  Description : Change from ASCII alphabet table to 23.38 alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Wchar2default(                          //RETURN: the length of default string
                                      wchar      *wchar_ptr,     //IN:
                                      uint8      *default_ptr,   //OUT:
                                      uint16     str_len         //IN:
                                      );

/*****************************************************************************/
//  Description : Change from 23.38 alphabet table to ASCII alphabet table
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
//  Return: 
/*****************************************************************************/
PUBLIC uint16 MMIAPICOM_Default2Wchar(                          //RETURN:
                                      uint8      *default_ptr,   //IN:
                                      wchar      *wchar_ptr,     //OUT:
                                      uint16     str_len         //IN:
                                      );
/*****************************************************************************/
//  Description : ÅÐ¶ÏwstringÊÇ·ñÐ¡ÓÚ 0XFFµÄASCII ×Ö·û´®
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsASCIIStringForPB(
                                        const wchar* wstr_ptr, 
                                        uint16 wstr_len
                                        );

/*****************************************************************************/
//  Description : ÅÐ¶ÏwstringÊÇ·ñÎª default ×Ö·û´®
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultString(
                                          const wchar* wstr_ptr, 
                                          uint16 wstr_len
                                          );

/*****************************************************************************/
//  Description : ÅÐ¶ÏÊÇ·ñÎª default ×Ö·û
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultChar(uint16 ch);

/*****************************************************************************/
//  Description : ÅÐ¶ÏÊÇ·ñÎª default ext ×Ö·û
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPICOM_IsDefaultExtChar(uint16 ch);

/*****************************************************************************/
//  Description : Normalize Url
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_NormalizeUrl(uint8 *in_url, uint8* out_url, uint16 out_max_len, BOOLEAN is_escape);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICOM_PrimoGenerateReplaceStrByTextId
(
    MMI_TEXT_ID_T text_id,
    const wchar* find_str,
    const wchar* replace_str,
    uint16 result_max_len,
    MMI_STRING_T *result_str//out
);

#endif  /* FILENAME_H */
