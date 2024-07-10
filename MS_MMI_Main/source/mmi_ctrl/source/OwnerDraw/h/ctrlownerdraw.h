/*****************************************************************************
** File Name:      ctrlownerdraw.h                                   *
** Author:                                                                   *
** Date:              07/23/2012                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                                                                                          *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                                      *
** 07/2012     xiyuan.ma              Creat
******************************************************************************/

#ifndef _CTRLOWNERDRAW_H_
#define _CTRLOWNERDRAW_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"

#include "ctrlbase.h"
#include "ctrlownerdraw_export.h"
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
//ownerdraw type info
#define CTRL_OWNERDRAW_TYPE (CTRLOWNERDRAW_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//ownerdraw init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T base_ctrl_param;

    GUIOWNDRAW_CALL_BACK_FUNC OwnDrawFunc; /*!<控件回调函数*/
} CTRLOWNERDRAW_INIT_PARAM_T;

//struct CTRLTIPS_CTRL_T
typedef struct _ownerdraw_obj_tag
{
    CTRLBASE_OBJ_T base_ctrl;

    BOOLEAN is_permit_border; //is permit display border
    uint16 height;//height
    GUIOWNDRAW_CALL_BACK_FUNC OwnDrawFunc;//owner draw func
    GUIOWNDRAW_TP_HANDLER_T MsgCallback;
    ADD_DATA add_data;
} CTRLOWNERDRAW_OBJ_T;

//ownerdraw class for public or protected method
typedef struct _ownerdraw_vtbl_tag
{
    CTRLBASE_VTBL_T                       base_ctrl_vtbl;
} CTRLOWNERDRAW_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*********************************************************************/
//  Description : get ownerdraw type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*********************************************************************/
PUBLIC TYPE_TAG_T CTRLOWNERDRAW_GetType();

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLOWNERDRAW__H_


