/*****************************************************************************
** File Name:      ctrltextedit.h                                               *
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

#ifndef _CTRLTEXTEDIT_H_
#define _CTRLTEXTEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "ctrltextedit_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// textedit type info
#define CTRL_TEXTEDIT_TYPE     (TEXTEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// textedit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    // self inif param ...
} CTRLTEXTEDIT_INIT_PARAM_T;

//textedit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    BOOLEAN                         is_sms;         /*!<是否是短消息,短消息显示消息条数*/
    BOOLEAN                         is_pb_encode;   /*!<是否是联系人姓名*/

    GUIEDIT_SMS_ENCODE_E		    sms_encode;		/*!<短消息编码方式*/
    GUIEDIT_TEXT_SPCHAR_NUM_T       char_num;       /*!<特殊字符计数*/
    GUIEDIT_TEXT_LANGCHAR_NUM_T     lan_num;        /*!<多语特殊字符计数*/
    GUIEDIT_TEXT_MAX_T              max_len;        /*!<ucs2,ascii,r8字符最大长度*/
    GUIEDIT_TEXT_PAGE_T             page;           /*!<分页信息,只针对sms*/
    GUIEDIT_TEXT_UCS2_T             ucs2;           /*!<最大最小ucs2长度,只针对pb*/

    MMITHEME_EDIT_TEXT_T            theme;          //text theme
} CTRLTEXTEDIT_OBJ_T;

// textedit class for public or protected method
typedef struct
{
    CTRLBASEFLEX_VTBL_T             baseflex_ctrl_vtbl;

    // self vtbl ...
} CTRLTEXTEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get textedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TEXTEDIT_GetType(void);
/*****************************************************************************/
//  Description : get commit and underline length for ucs2 != ascii
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TEXTEDIT_GetAddLenForDifferent (
    CTRLTEXTEDIT_OBJ_T *textedit_ctrl_ptr, //in:
    GUIEDIT_ADD_STR_T  *add_str_ptr    //in/out:
);

/*****************************************************************************/
//  Description : adjust special char number,include r8 2byte,r8 1byte,default ext.
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void TEXTEDIT_AdjustSpecialCharNum (
    CTRLTEXTEDIT_OBJ_T *textedit_ctrl_ptr,
    BOOLEAN            is_add,         //in:is add or delete
    wchar              *str_ptr,       //in:
    uint16             str_len,        //in:
    uint16             del_start_pos   //in:only for delete
);

/*****************************************************************************/
//  Description : is need convert ucs2 char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN TEXTEDIT_IsConvertUcs2 (
    uint16                      ucs2_num,           //in:
    GUIEDIT_TEXT_LANGCHAR_NUM_T *langchar_num_ptr   //in:
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
