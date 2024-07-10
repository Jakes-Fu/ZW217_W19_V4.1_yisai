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
//  Description : ������̹���
//  Parameter:
//             [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Enter( void );

/*****************************************************************************/
//  Description : Watch Face Flowģ���ṩ�Ĵ������button������Ϊ��ǰ���̣��ĺ���
//  Parameter:
//             [In] list_item_index : �����buttion���ڵ�item��list�е�index
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton1( uint16 list_item_index );


/*****************************************************************************/
//  Description : Watch Face Flowģ���ṩ�Ĵ����Ҳ�button��ɾ�����̣��ĺ���
//  Parameter:
//             [In] list_item_index : �����buttion���ڵ�item��list�е�index
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleListItemButton2( uint16 list_item_index );

/*****************************************************************************/
//  Description : ΪWatch face delete QueryWin�ṩ�Ĵ������button������ȡ�����ĺ���
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton1( void );

/*****************************************************************************/
//  Description : ΪWatch face delete QueryWin�ṩ�Ĵ����Ҳ�button������ȡ�����ĺ���
//  Parameter: [In] None
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Flow_HandleDeleteQueryWinButton2( void );

/*****************************************************************************/
//  Description : ɾ�����̺���½���ĺ���
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
