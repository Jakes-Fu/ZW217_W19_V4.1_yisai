/*****************************************************************************
** File Name:      ctrltextedit_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLTEXTEDIT_EXPORT_H_
#define _CTRLTEXTEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @struct GUIEDIT_TEXT_MAX_T
@brief 文本编辑框最大字符长度
*/
typedef struct
{
    uint16      ucs2;       /*!<ucs2 最大字符长度*/
    uint16      ascii;      /*!<ascii 最大字符长度*/
    uint16      r8_1byte;   /*!<R8单字节 最大字符长度*/
    uint16      r8_2byte;   /*!<R8双字节 最大字符长度*/
} GUIEDIT_TEXT_MAX_T;

/*! @struct GUIEDIT_TEXT_PAGE_T
@brief 文本编辑框分页信息,只针对sms
*/
typedef struct
{
    uint16      single_ucs2_len;        /*!<单条短信发送,最大ucs2字符个数*/
    uint16      multi_ucs2_len;         /*!<多条短信发送,每条的ucs2字符个数*/
    uint16      single_ascii_len;       /*!<单条短信发送,最大ascii字符个数*/
    uint16      multi_ascii_len;        /*!<多条短信发送,每条的ascii字符个数*/
    uint16      single_r8_1byte_len;    /*!<单条短信发送,R8单字节最大字符个数*/
    uint16      single_r8_2byte_len;    /*!<单条短信发送,R8双字节最大字符个数*/
    uint16      multi_r8_1byte_len;     /*!<多条短信发送,R8单字节最大字符个数*/
    uint16      multi_r8_2byte_len;     /*!<多条短信发送,R8双字节最大字符个数*/
} GUIEDIT_TEXT_PAGE_T;

/*! @struct GUIEDIT_TEXT_UCS2_T
@brief 文本编辑框最大最小ucs2长度,只针对pb
*/
typedef struct
{
    wchar   max;    /*!<最大ucs2长度*/
    wchar   min;    /*!<最小ucs2长度*/
} GUIEDIT_TEXT_UCS2_T;

/*! @struct GUIEDIT_TEXT_SPCHAR_NUM_T
@brief 文本编辑框特殊字符计数,只针对sms
*/
typedef struct
{
    uint16          ucs2;           /*!<sms ucs2编码字符个数*/
    uint16          default_ext;    /*!<sms 7Bit编码字符个数*/
    uint16          r8_1byte;       /*!<sms R8 single byte编码字符个数*/
    uint16          r8_2byte;       /*!<sms R8 double byte编码字符个数*/
} GUIEDIT_TEXT_SPCHAR_NUM_T;

/*! @struct GUIEDIT_TEXT_LANGCHAR_NUM_T
@brief 文本编辑框多语特殊字符计数,只针对sms
*/
typedef struct
{
    uint16      tur;        /*!<土耳其私有字符个数*/
    uint16      esp;        /*!<西班牙私有字符个数*/
    uint16      por;        /*!<葡萄牙私有字符个数*/
    uint16      tur_esp;    /*!<土耳其,西班牙共有字符个数*/
    uint16      esp_por;    //*!<西班牙,葡萄牙共有字符个数*/
} GUIEDIT_TEXT_LANGCHAR_NUM_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set text max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:当中英文输入字符个数不一致,或者控件创建没有设置时,需调用此函数
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextMaxLen (
    MMI_CTRL_ID_T     ctrl_id,
    uint16            unicode_max_len,
    uint16            ascii_max_len
);

/*****************************************************************************/
//  Description : set text max length
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextMaxLenEx (
    MMI_CTRL_ID_T       ctrl_id,
    GUIEDIT_TEXT_MAX_T  *max_len_ptr
);

/*****************************************************************************/
//  Description : set text page info
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetTextPage (
    MMI_CTRL_ID_T			ctrl_id,	//in:
    GUIEDIT_TEXT_PAGE_T		*page_ptr,	//in:may PNULL
    GUIEDIT_SMS_ENCODE_E	*encode_ptr	//in:may PNULL
);

/*****************************************************************************/
//  Description : set edit is support sms encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for sms
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetSmsEncode (
    MMI_CTRL_ID_T			ctrl_id,	//in:
    BOOLEAN                 is_sms      //in
);

/*****************************************************************************/
//  Description : set edit is support encode
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for pb
/*****************************************************************************/
PUBLIC void CTRLTEXTEDIT_SetPbEncode (
    BOOLEAN         is_support,
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
//  Description : judge whether editor support pb encode or not
//  Global resource dependence :
//  Author: Jassmine
//  Note:only for pb
/*****************************************************************************/
PUBLIC BOOLEAN CTRLTEXTEDIT_GetPbEncode (
    MMI_CTRL_ID_T   ctrl_id
);

/*****************************************************************************/
// 	Description : get the left number in Text EditBox
//	Global resource dependence :
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint16 CTRLTEXTEDIT_GetLeftNum (        //RETURN:
    MMI_CTRL_ID_T   ctrl_id,	//IN:
    uint16          *sms_num		//out:
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
