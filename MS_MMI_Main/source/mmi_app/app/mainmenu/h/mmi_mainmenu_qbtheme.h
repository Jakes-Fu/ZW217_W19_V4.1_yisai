/*****************************************************************************
** File Name:      mmi_mainmenu_qbtheme.h                                    *
** Author:                                                                   *
** Date:           10/23/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2012      hua.fang              Create
******************************************************************************/

#ifndef _MMI_MAINMENU_QBTHEME_H_
#define _MMI_MAINMENU_QBTHEME_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_internal.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#if (defined QBTHEME_SUPPORT) || (defined MMI_SAM_THEME)

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
//  Description : 注册千变主题菜单项的信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_RegisterQbTheme(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );
                                          

#ifdef ATEST_SUPPORT
PUBLIC void Atest_QbThemeMenuGetItemInfo(
    void     *qbtheme_ptr,
    GUIMENU_ITEM_POSITION_E         position,
    uint16 page_index,
    uint16 item_index,
    CTRLMAINMENU_ITEM_T *item_ptr
);
#endif

#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_QBTHEME_H_

/*@}*/
