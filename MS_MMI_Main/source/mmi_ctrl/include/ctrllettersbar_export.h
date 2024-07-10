/*****************************************************************************
** File Name:      ctrllettersbar_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLLETTERSBAR_EXPORT_H_
#define _CTRLLETTERSBAR_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase_export.h"

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
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<横竖屏区域*/
} GUILETTERSBAR_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author xiaoqing.lu nan.ji
@param      ctrl_id：［IN］滚动条的控件ID
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note
*******************************************************************************/
PUBLIC wchar  CTRLLETTERS_GetWchar (
                                    MMI_HANDLE_T ctrl_handle,
                                    uint16       item_pos
                                    );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_SetRect(
                                   MMI_HANDLE_T ctrl_handle, 
                                   const GUI_RECT_T* rect_ptr
                                   );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_SetBothRect(
                                       MMI_HANDLE_T ctrl_handle, 
                                       const GUI_BOTH_RECT_T* both_rect_ptr
                                       );

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLETTERS_Update(
                                  MMI_HANDLE_T ctrl_handle
                                  );

/*****************************************************************************/
// 	Description : create the progress control
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLLETTERS_Create(
                                       MMI_HANDLE_T win_handle,
                                       MMI_HANDLE_T parent_ctrl_handle,
                                       MMI_CTRL_ID_T ctrl_id
                                       );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
