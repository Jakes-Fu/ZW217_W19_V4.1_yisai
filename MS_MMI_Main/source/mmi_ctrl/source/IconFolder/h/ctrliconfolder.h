/*****************************************************************************
** File Name:      ctrliconfolder.h                                               *
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

#ifndef _CTRLICONFOLDER_H_
#define _CTRLICONFOLDER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrliconfolder_export.h"
#include "mmitheme_iconfolder.h"

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
// iconfolder type info
#define CTRL_ICONFOLDER_TYPE     (ICONFOLDER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// iconfolder init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
} CTRLICONFOLDER_INIT_PARAM_T;

//icon info
typedef struct
{
    BOOLEAN     is_load;        //图片是否已经加载
    uint32      anim_ctrl_add;  //动画控件的地址
} ICONFOLDER_ICON_T;

//item info
typedef struct
{
    BOOLEAN                 is_selected;//is selected
    ICONFOLDER_ICON_T    icon;       //icon
} ICONFOLDER_ITEM_T;

//iconfolder control
typedef struct
{
    CTRLBASE_OBJ_T              	base_ctrl;

    BOOLEAN                 		  	is_anim_callback;   //is anim call back function
    BOOLEAN                 		  	is_selected;        //is selected
    uint16                  		  	folder_index;       //folder in icon list index
    uint16                  		  	total_num;          //the total number of item

    ICONFOLDER_ITEM_T       	*item_ptr;          //item

    MMITHEME_ICONFOLDER_T      theme;              //theme

    MMI_HANDLE_T            		 win_handle;

} CTRLICONFOLDER_OBJ_T;

// iconfolder class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;
} CTRLICONFOLDER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get icon folder pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLICONFOLDER_OBJ_T* ICONFOLDER_GetPtr(
                                                  MMI_HANDLE_T  ctrl_handle
                                                  );

/*****************************************************************************/
//  Description : get icon rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T ICONFOLDER_GetIconRect(
                                            uint16                  index,              //in:
                                            CTRLICONFOLDER_OBJ_T    *iconfolder_ctrl_ptr//in:
                                            );

/*****************************************************************************/
//  Description : get icon angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetIconAngle(
                                         uint16                 index,              //in:
                                         CTRLICONFOLDER_OBJ_T   *iconfolder_ctrl_ptr//in:
                                         );

/*****************************************************************************/
//  Description : 获取总个数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetTotalNum(
                                        CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                                        );

/*****************************************************************************/
//  Description : 获取指定的动画控件
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASE_OBJ_T* ICONFOLDER_GetAnimCtrl(
                                        CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr,//in:
                                        uint16          index
                                        );

/*****************************************************************************/
//  Description : 获取最大个数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 ICONFOLDER_GetMaxIconNum(
                                          CTRLICONFOLDER_OBJ_T *iconfolder_ctrl_ptr//in:
                                          );

/*****************************************************************************/
//  Description : get iconfolder type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T ICONFOLDER_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
