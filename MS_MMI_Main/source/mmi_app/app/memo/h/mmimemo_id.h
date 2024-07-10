/*****************************************************************************
** File Name:      mmimemo_id.h                                          *
** Author:          fen.xie                                                    *
** Date:            05/08/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the memo id              *
*****************************************************************************/
#ifndef MMIMEMO_ID_H_
#define MMIMEMO_ID_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmimemo_wintable.h"

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
 
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    MMIMEMO_WIN_ID_START = (MMI_MODULE_MEMO << 16),

#include "mmimemo_id.def"

    MMIMEMO_MAX_WIN_ID
}MMIMEMO_WINDOW_ID_E;

#undef WIN_ID_DEF

typedef enum
{
    MMIMEMO_CONTROL_ID=MMIMEMO_MAX_WIN_ID,
   
    MMIMEMO_LIST_CTRL_ID,
    MMIMEMO_EDIT_FORM_CTRL_ID,
    MMIMEMO_EDIT_SOFTKEY_CTRL_ID,
    MMIMEMO_EDITBOX_CTRL_ID,
    MMIMEMO_EDIT_CHILD1_FORM_CTRL_ID,
    MMIMEMO_EDIT_CHILD2_FORM_CTRL_ID,
    MMIMEMO_EDIT_LABEL1_CTRL_ID,
    MMIMEMO_EDIT_LABEL2_CTRL_ID,
    MMIMEMO_EDIT_LABEL3_CTRL_ID,
    
    MMIMEMO_READ_FORM_CTRL_ID,
    MMIMEMO_READ_CHILD1_FORM_CTRL_ID,
    MMIMEMO_READ_CHILD1_0_FORM_CTRL_ID, //add for MINI 160X128 READ_WINTAB, two line style
	MMIMEMO_READ_CHILD1_1_FORM_CTRL_ID,
    MMIMEMO_READ_CHILD2_FORM_CTRL_ID,
    MMIMEMO_READ_BUTTON1_CTRL_ID,
    MMIMEMO_READ_BUTTON2_CTRL_ID,
    MMIMEMO_READ_BUTTON3_CTRL_ID,
    MMIMEMO_READ_LABEL1_CTRL_ID,
    MMIMEMO_READ_LABEL2_CTRL_ID,
    MMIMEMO_READ_LABEL3_CTRL_ID,
    MMIMEMO_READ_SOFTKEY_CTRL_ID,
    MMIMEMO_READ_TEXT_CTRL_ID,

    MMIMEMO_POPMEMU_CTRL_ID,

    MMIMEMO_CONTROL_ID_NUMBER
}MMIMEMO_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
