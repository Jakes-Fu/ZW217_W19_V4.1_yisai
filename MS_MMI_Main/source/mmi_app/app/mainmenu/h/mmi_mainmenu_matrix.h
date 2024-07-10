/*****************************************************************************
** File Name:      mmi_mainmenu_matrix.h                                     *
** Author:                                                                   *
** Date:           10/17/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      xiaoqing.lu              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_MATRIX_H_
#define _MMI_MAINMENU_MATRIX_H_ 
#ifdef MATRIX_MAIN_MENU_SUPPORT
/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_link.h"

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


/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_MatrixRegister(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );

#ifdef ATEST_SUPPORT
extern PUBLIC GUI_RECT_T Atest_MatrixMenuGetItemRect(void *menu_ctrl_ptr, int16 item_index);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_MATRIX_H_
#endif // MATRIX_MAIN_MENU_SUPPORT
/*@}*/
