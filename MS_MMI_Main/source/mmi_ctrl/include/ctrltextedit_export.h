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
@brief �ı��༭������ַ�����
*/
typedef struct
{
    uint16      ucs2;       /*!<ucs2 ����ַ�����*/
    uint16      ascii;      /*!<ascii ����ַ�����*/
    uint16      r8_1byte;   /*!<R8���ֽ� ����ַ�����*/
    uint16      r8_2byte;   /*!<R8˫�ֽ� ����ַ�����*/
} GUIEDIT_TEXT_MAX_T;

/*! @struct GUIEDIT_TEXT_PAGE_T
@brief �ı��༭���ҳ��Ϣ,ֻ���sms
*/
typedef struct
{
    uint16      single_ucs2_len;        /*!<�������ŷ���,���ucs2�ַ�����*/
    uint16      multi_ucs2_len;         /*!<�������ŷ���,ÿ����ucs2�ַ�����*/
    uint16      single_ascii_len;       /*!<�������ŷ���,���ascii�ַ�����*/
    uint16      multi_ascii_len;        /*!<�������ŷ���,ÿ����ascii�ַ�����*/
    uint16      single_r8_1byte_len;    /*!<�������ŷ���,R8���ֽ�����ַ�����*/
    uint16      single_r8_2byte_len;    /*!<�������ŷ���,R8˫�ֽ�����ַ�����*/
    uint16      multi_r8_1byte_len;     /*!<�������ŷ���,R8���ֽ�����ַ�����*/
    uint16      multi_r8_2byte_len;     /*!<�������ŷ���,R8˫�ֽ�����ַ�����*/
} GUIEDIT_TEXT_PAGE_T;

/*! @struct GUIEDIT_TEXT_UCS2_T
@brief �ı��༭�������Сucs2����,ֻ���pb
*/
typedef struct
{
    wchar   max;    /*!<���ucs2����*/
    wchar   min;    /*!<��Сucs2����*/
} GUIEDIT_TEXT_UCS2_T;

/*! @struct GUIEDIT_TEXT_SPCHAR_NUM_T
@brief �ı��༭�������ַ�����,ֻ���sms
*/
typedef struct
{
    uint16          ucs2;           /*!<sms ucs2�����ַ�����*/
    uint16          default_ext;    /*!<sms 7Bit�����ַ�����*/
    uint16          r8_1byte;       /*!<sms R8 single byte�����ַ�����*/
    uint16          r8_2byte;       /*!<sms R8 double byte�����ַ�����*/
} GUIEDIT_TEXT_SPCHAR_NUM_T;

/*! @struct GUIEDIT_TEXT_LANGCHAR_NUM_T
@brief �ı��༭����������ַ�����,ֻ���sms
*/
typedef struct
{
    uint16      tur;        /*!<������˽���ַ�����*/
    uint16      esp;        /*!<������˽���ַ�����*/
    uint16      por;        /*!<������˽���ַ�����*/
    uint16      tur_esp;    /*!<������,�����������ַ�����*/
    uint16      esp_por;    //*!<������,�����������ַ�����*/
} GUIEDIT_TEXT_LANGCHAR_NUM_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set text max length
//  Global resource dependence :
//  Author: Jassmine
//  Note:����Ӣ�������ַ�������һ��,���߿ؼ�����û������ʱ,����ô˺���
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
