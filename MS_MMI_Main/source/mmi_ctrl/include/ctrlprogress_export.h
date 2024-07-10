/*****************************************************************************
** File Name:      ctrlprogress_export.h                                        *
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

#ifndef _CTRLPROGRESS_EXPORT_H_
#define _CTRLPROGRESS_EXPORT_H_ 

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
    GUI_BOTH_RECT_T     both_rect;  /*!<ºáÊúÆÁÇøÓò*/
} GUIPROGRESS_INIT_DATA_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get current value
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLPROGRESS_GetValue(
                                   MMI_HANDLE_T ctrl_handle
                                   );

/*****************************************************************************/
//  Description : set current value
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetValue(
                                     MMI_HANDLE_T ctrl_handle,
                                     int16 cur_value
                                     );

/*****************************************************************************/
//  Description : set min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetMin(
                                   MMI_HANDLE_T ctrl_handle,
                                   int16 min
                                   );

/*****************************************************************************/
//  Description : set max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetMax(
                                   MMI_HANDLE_T ctrl_handle,
                                   int16 max
                                   );

/*****************************************************************************/
//  Description : set step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetStep(
                                    MMI_HANDLE_T ctrl_handle,
                                    int16 step
                                    );

/*****************************************************************************/
//  Description : set step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLPROGRESS_GetStep(
                                  MMI_HANDLE_T ctrl_handle
                                  );

/*****************************************************************************/
//  Description : inc special step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_Increment(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 special_step
                                      );

/*****************************************************************************/
//  Description :inc step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_Step(
                                 MMI_HANDLE_T ctrl_handle
                                 );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetCtrlRect(
                                        MMI_HANDLE_T ctrl_handle, 
                                        const GUI_RECT_T* rect_ptr
                                        );

/*****************************************************************************/
//  Description : display progress control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_UpdateCtrl(
                                       MMI_HANDLE_T ctrl_handle
                                       );

/*****************************************************************************/
// 	Description : create the progress control
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLPROGRESS_Create(
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
