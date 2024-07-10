/*! @file guistatusbar_data.h
@brief 此文件介绍了statusbar_data控件的对外接口和数据类型
@author xiaoqing.lu
@version 1.0
@date 12/23/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 12/23/2008     Xiaoqing.lu      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(暂无)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_data模块
@details 该文件不对外开放，所有app都应该调用guiwin.h中的封装接口
@{
*******************************************************************************/

#ifndef _GUI_STATUSBAR_DATA_H_
#define _GUI_STATUSBAR_DATA_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "ctrlstatusbar_data.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#define GUISTBDATA_InitData CTRLSTBDATA_InitData
#define GUISTBDATA_AppendItemData CTRLSTBDATA_AppendItemData
#define GUISTBDATA_SetItemVisible CTRLSTBDATA_SetItemVisible
#define GUISTBDATA_IsItemVisible CTRLSTBDATA_IsItemVisible
#define GUISTBDATA_SetItemValid CTRLSTBDATA_SetItemValid
#define GUISTBDATA_IsItemValid CTRLSTBDATA_IsItemValid
#define GUISTBDATA_SetItemDisable CTRLSTBDATA_SetItemDisable
#define GUISTBDATA_IsItemDisable CTRLSTBDATA_IsItemDisable
#define GUISTBDATA_SetItemHandleFunc CTRLSTBDATA_SetItemHandleFunc
#define GUISTBDATA_SetItemHandleFuncValid CTRLSTBDATA_SetItemHandleFuncValid
#define GUISTBDATA_ItemIsHandleFuncValid CTRLSTBDATA_ItemIsHandleFuncValid
#define GUISTBDATA_SetItemIcon CTRLSTBDATA_SetItemIcon
#define GUISTBDATA_GetItemIcon CTRLSTBDATA_GetItemIcon
#define GUISTBDATA_SetItemIconCurFrame CTRLSTBDATA_SetItemIconCurFrame
#define GUISTBDATA_GetItemIconCurFrame CTRLSTBDATA_GetItemIconCurFrame
#define GUISTBDATA_GetItemIconNextFrame CTRLSTBDATA_GetItemIconNextFrame
#define GUISTBDATA_SetItemIconValid CTRLSTBDATA_SetItemIconValid
#define GUISTBDATA_IsItemIconValid CTRLSTBDATA_IsItemIconValid
#define GUISTBDATA_SetItemIconStyle CTRLSTBDATA_SetItemIconStyle
#define GUISTBDATA_GetItemIconStyle CTRLSTBDATA_GetItemIconStyle
#define GUISTBDATA_SetItemIconId CTRLSTBDATA_SetItemIconId
#define GUISTBDATA_SetItemText CTRLSTBDATA_SetItemText
#define GUISTBDATA_GetItemText CTRLSTBDATA_GetItemText
#define GUISTBDATA_SetItemTextValid CTRLSTBDATA_SetItemTextValid
#define GUISTBDATA_IsItemTextValid CTRLSTBDATA_IsItemTextValid
#define GUISTBDATA_SetItemRect CTRLSTBDATA_SetItemRect
#define GUISTBDATA_GetItemRect CTRLSTBDATA_GetItemRect
#define GUISTBDATA_GetItemTotalNum CTRLSTBDATA_GetItemTotalNum
#define GUISTBDATA_GetItemData CTRLSTBDATA_GetItemData
#define GUISTBDATA_SetItemRightAlign CTRLSTBDATA_SetItemRightAlign
#define GUISTBDATA_SetItemCenterAlign CTRLSTBDATA_SetItemCenterAlign
#define GUISTBDATA_IsItemRightAlign CTRLSTBDATA_IsItemRightAlign
#define GUISTBDATA_IsItemCenterAlign CTRLSTBDATA_IsItemCenterAlign
#define GUISTABATA_SetItemBgInter CTRLSTABATA_SetItemBgInter
#define GUISTABATA_SetOwnerDraw CTRLSTABATA_SetOwnerDraw
 
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _GUI_STATUSBAR_DATA_H_


/*@}*/
