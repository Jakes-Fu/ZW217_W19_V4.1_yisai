/*****************************************************************************
** File Name:      mmi_mainmenu_slide_page.h                                       *
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

#ifndef _MMI_MAINMENU_SLIDE_PAGE_H_
#define _MMI_MAINMENU_SLIDE_PAGE_H_ 

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
//  Description : 获取宫格菜单项的位置信息
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: xiyuan.ma edit 2011.11
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_SlideRegister(
                                          CTRLMAINMENU_PM_APIS_T *apis
                                          );

#ifdef ATEST_SUPPORT
PUBLIC GUI_RECT_T Atest_GetSlideMenuItemRect(void *menu_ctrl_ptr, int16 item_in_page_index);
PUBLIC MMIMAINMENU_ITEM_INFO_T* Atest_SlidePageMenuGetItemInfoByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16 item_index);
PUBLIC MMIMAINEMENU_ITEM_LIST_T *Atest_GetSlidePageMenuLinkListPtr(
    void   *qbtheme_ptr,
    uint32                       page_index);
PUBLIC MMI_LINK_NODE_T *Atest_SlidePageMenuSearchItemByIndex(
    MMIMAINEMENU_ITEM_LIST_T *page_item_info_ptr,
    uint16                   item_index);
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMI_MAINMENU_DATA_H_

/*@}*/
