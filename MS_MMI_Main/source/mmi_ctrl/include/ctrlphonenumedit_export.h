/*****************************************************************************
** File Name:      ctrlphonenumedit_export.h                                        *
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

#ifndef _CTRLPHONENUMEDIT_EXPORT_H_
#define _CTRLPHONENUMEDIT_EXPORT_H_ 

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

/*! @struct GUIEDIT_FONT_T
@brief ��С������Ϣ
*/
typedef struct
{
    BOOLEAN             is_valid;           // /*!<�ýṹ�Ƿ���Ч*/
    GUI_FONT_T          big_font;           // /*!<������*/
    GUI_FONT_T          mid_font;           // /*!<������*/
    GUI_FONT_T          small_font;         // /*!<С����*/
} GUIEDIT_FONT_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit phone number string,�ص��˷���λ��+���Լ�������ַ�
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_GetPhoneNumString (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    MMI_STRING_T      *str_ptr    //in:/out
);

/*****************************************************************************/
//  Description :����phone number edit��������,�������ַ�,�Ƿ�ɾ����һ���ַ�
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetPhoneNumStyle (
    BOOLEAN        is_delete,
    MMI_CTRL_ID_T  ctrl_id
);

/*****************************************************************************/
//  Description : set phone number edit display direction
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for phone number multi line
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPNEDIT_SetPhoneNumDir (
    MMI_CTRL_ID_T             ctrl_id,    //in
    GUIEDIT_DISPLAY_DIR_E     display_dir //in
);

/*****************************************************************************/
//  Description : set dtmf num for phonenum
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPNEDIT_SetDtmfNum (
    MMI_CTRL_ID_T             ctrl_id,    //in
    BOOLEAN                   is_dtmf_num //in
);

/*****************************************************************************/
//  Description : set window intercept edit control msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetWinInterceptCtrlMsg (
    MMI_CTRL_ID_T                    ctrl_id,
    GUIEDIT_WIN_INTERCEPT_CTRL_MSG   func
);

/*****************************************************************************/
//  Description : set edit font extend
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetFontEx (
    MMI_CTRL_ID_T   ctrl_id,      //in
    GUIEDIT_FONT_T  *font_ptr     //in
);

/*****************************************************************************/
//  Description : set edit line is adaptive or not
//  Global resource dependence :
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void CTRLPNEDIT_SetDialEditAdaptive (
    MMI_CTRL_ID_T   ctrl_id,      //in
    BOOLEAN is_font_adaptive,     //in
    BOOLEAN is_line_adaptive    //in
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
