/*****************************************************************************
** File Name:        appstore_flow.h                                         *
** Author:           xiuyun.wang                                             *
** Date:             08/12/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define app store flow api            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/12/2021           xiuyun.wang           Create                         *
******************************************************************************/
#ifndef _APPSTORE_FLOW_H_
#define _APPSTORE_FLOW_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_home_win_data.h"
#include "appstore_control.h"
#include "appstore_common.h"

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
//  Description :获取s_p_full_app_list
//  Parameter: [In] None
//             [Out] None
//             [Return] s_p_full_app_list
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_APP_LIST_T* APPSTORE_Flow_GetFullAppList( void );

/*****************************************************************************/
//  Description : get full app list and set data list.
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_UpdateFullAppListForData( void );

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块Uninstall QueryWin提供的处理左侧button（处理取消）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeUninstallQueryWinButton1( void );

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块Uninstall QueryWin提供的处理右侧button（处理卸载）的函数
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeUninstallQueryWinButton2( void );

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块提供的处理左侧button（处理安装/更新）的函数
//  Parameter: [In] list_item_index : 需要处理的list item的index
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeWinButton1(uint16 list_item_index);

/*****************************************************************************/
//  Description : APP Store HomeWin Flow模块提供的处理右侧button（处理安装/更新）的函数
//  Parameter: [In] list_item_index : 需要处理的list item的index
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Flow_HandleHomeWinButton2(uint16 list_item_index);

/*****************************************************************************/
//  Description : 进入APP Store 应用程序
//  Parameter: [In] None
//             [Out] None
//             [Return] APPSTORE_RES_E
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC APPSTORE_RES_E APPSTORE_Enter( void );

#endif
