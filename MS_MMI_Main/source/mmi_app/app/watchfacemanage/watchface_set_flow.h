/*****************************************************************************
** File Name:        watchface_set_flow.h                                        *
** Author:           bin.wang1                                               *
** Date:             08/17/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define watch face flow api           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/17/2021           bin.wang1             Create                         *
******************************************************************************/
#ifdef WATCHFACE_MANAGE_SUPPORT

#ifndef _WATCHFACE_SET_FLOW_H_
#define _WATCHFACE_SET_FLOW_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "watchface_set_win_data.h"


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 进入表盘管理
//  Parameter:
//             [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Enter( void );

/*****************************************************************************/
//  Description : Watch Face Flow模块提供的处理左侧button（设置为当前表盘）的函数
//  Parameter:
//             [In] list_item_index : 点击的buttion所在的item在list中的index
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton1( uint16 list_item_index );


/*****************************************************************************/
//  Description : Watch Face Flow模块提供的处理右侧button（删除表盘）的函数
//  Parameter:
//             [In] list_item_index : 点击的buttion所在的item在list中的index
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton2( uint16 list_item_index );

/*****************************************************************************/
//  Description : 为Watch face delete QueryWin提供的处理左侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton1( void );

/*****************************************************************************/
//  Description : 为Watch face delete QueryWin提供的处理右侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton2( void );

/*****************************************************************************/
//  Description : 删除表盘后更新界面的函数
//  Parameter:
//             [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_DeleteResulte( BOOLEAN is_success );

#endif
#endif
