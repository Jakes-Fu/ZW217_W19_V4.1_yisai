/*****************************************************************************
** File Name:      mmi_mainmenu_sweets.h                                     *
** Author:                                                                   *
** Date:           10/17/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**
******************************************************************************/

#ifndef _MMI_MAINMENU_SWEETS_H_
#define _MMI_MAINMENU_SWEETS_H_

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
/*****************************************************************************/
//  Description : Sweets Register
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SweetsRegister (
    CTRLMAINMENU_PM_APIS_T *apis
);


/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef ATEST_SUPPORT
extern PUBLIC GUI_RECT_T Atest_SWEETS_GetItemDisRect(void *menu_ctrl_ptr, uint16 item_index );
extern PUBLIC uint16 Atest_SWEETS_GetPageItemNum(void *menu_ctrl_ptr, uint16   page_index);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_MMI_MAINMENU_SWEETS_H_

/*@}*/
