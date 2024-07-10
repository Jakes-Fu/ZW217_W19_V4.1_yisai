/*****************************************************************************
** File Name:      ctrlprogress.h                                               *
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

#ifndef _CTRLPROGRESS_H_
#define _CTRLPROGRESS_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlprogress_export.h"

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
// progress type info
#define CTRL_PROGRESS_TYPE     (PROGRESS_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// image param
typedef struct
{
    BOOLEAN                     is_true;
    GUI_COLOR_T                 color;
    GUI_COLOR_T                 disable_color;
    MMI_IMAGE_ID_T              image_id;
    MMI_IMAGE_ID_T              disable_image_id;

} CTRLPROGRESS_IMAGE_T;

// progress init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLPROGRESS_INIT_PARAM_T;

//progress control
typedef struct _progress_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    int16                       max;
    int16                       min;
    int16                       cur;
    int16                       step;
   
	BOOLEAN                     is_vertical;

    CTRLPROGRESS_IMAGE_T        bg_info;
    CTRLPROGRESS_IMAGE_T        fg_info;

    // self member ...
} CTRLPROGRESS_OBJ_T;

// progress class for public or protected method
typedef struct _progress_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLPROGRESS_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get progress type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PROGRESS_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
