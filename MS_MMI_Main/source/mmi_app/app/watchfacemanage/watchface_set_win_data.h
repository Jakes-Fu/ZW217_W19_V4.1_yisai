/*****************************************************************************
** File Name:        watchface_set_win_data.h                                *
** Author:           bin.wang1                                               *
** Date:             08/17/2021                                              *
** Copyright:                                                                *
** Description:    This file is used to define watch face set data api       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/17/2021           bin.wang1           Create                           *
******************************************************************************/
#ifdef WATCHFACE_MANAGE_SUPPORT

#ifndef _WATCHFACE_SET_WIN_DATA_H_
#define _WATCHFACE_SET_WIN_DATA_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "os_api.h"

#include "mmicom_trace.h"
#include "watch_common_list.h"

#include "watchface_config.h"

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
//  Description : ���ٱ��̹�������������
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Set_Data_DestroyWinData( void );

/*****************************************************************************/
//  Description : �������̹�������������
//  Parameter: [In] num ������������
//             [In] �б�����ָ������
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_CreateWinData( uint16 num, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_item_ptr_array );

/*****************************************************************************/
//  Description : ��ȡ�����б��б��̵�����
//  Parameter: [In] None
//             [Out] None
//             [Return] ��������
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC uint16 WATCHFACE_Set_Data_GetListItemNum( void );

/*****************************************************************************/
//  Description : ɾ�������б��е�һ������
//  Parameter: [In] ��������
//             [Out] None
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_DelListItem( uint16 index );

/*****************************************************************************/
//  Description : ��ȡ�����б��еı�������
//  Parameter: [In] index ��������
//             [Out] �������ݵ�ַ
//             [Return] WATCHFACE_STATUS_RES_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC WATCHFACE_STATUS_RES_E WATCHFACE_Set_Data_GetListItemData( uint16 index, WATCHCOM_LIST_ITEM_STYLE_1ICON_2STR_2BUTTON_DYNA_T** p_list_item_data );

#endif
#endif
