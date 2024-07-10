/***************************************************************************
** File Name:      ctrlcheckbox.h                                          *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLCHECKBOX_H_
#define _CTRLCHECKBOX_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "ctrlbutton.h"
#include "ctrlcheckbox_export.h"
#include "mmitheme_checkbox.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
// button type info
#define CTRL_CHECKBOX_TYPE     (CHECKBOX_GetType())

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

// button init param
typedef struct
{
    CTRLBUTTON_INIT_PARAM_T     button_param;

} CTRLCHECKBOX_INIT_PARAM_T;

// checkbox object struct
typedef struct _checkbox_obj_tag
{
    CTRLBUTTON_OBJ_T        button_ctrl;

    MMITHEME_CHECKBOX_T     theme;

    BOOLEAN                 is_checked;
    BOOLEAN                 is_tp_checked_state;
} CTRLCHECKBOX_OBJ_T;

// checkbox class for public or protected method
typedef struct _checkbox_vtbl_tag
{
    CTRLBUTTON_VTBL_T      button_vtbl;

} CTRLCHECKBOX_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get checkbox type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T CHECKBOX_GetType();

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLCHECKBOX_H_
