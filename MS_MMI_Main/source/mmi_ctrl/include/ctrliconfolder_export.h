/*****************************************************************************
** File Name:      ctrliconfolder_export.h                                        *
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

#ifndef _CTRLICONFOLDER_EXPORT_H_
#define _CTRLICONFOLDER_EXPORT_H_ 

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
/*! @struct CTRLICONFOLDER_INIT_DATA_T
@brief �ļ��пؼ���ʼ������
*/
typedef struct 
{
   GUI_BOTH_RECT_T  both_rect;  	/*!<��������*/
} CTRLICONFOLDER_INIT_DATA_T;

/*! @struct CTRLICONFOLDER_NOTIFY_T
@brief �ļ��пؼ�MSG_NOTIFY_ICONFOLDER_APPEND_ICONЯ���Ĳ���
*/
typedef struct 
{
    uint16          	icon_index;		/*!<��ǶͼƬ����,��0��ʼ*/
    uint16          	folder_index;	/*!<�ļ�������,��0��ʼ*/
    MMI_NOTIFY_T	notify;			/*!<���ýṹ*/
} CTRLICONFOLDER_NOTIFY_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set icon folder background
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_SetBg(
                                   MMI_HANDLE_T     ctrl_id,//in
                                   GUI_BG_T         *bg_ptr //in
                                   );

/*****************************************************************************/
//  Description : set icon folder rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONFOLDER_SetRect(
                                  MMI_HANDLE_T      ctrl_id,
                                  GUI_RECT_T        *rect_ptr
                                  );

/*****************************************************************************/
//  Description : set icon folder total icons number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLICONFOLDER_SetTotalIcon(
                                       MMI_CTRL_ID_T    ctrl_id,    //in
                                       uint16           total_num,  //in
                                       uint16           folde_index //in
                                       );

/*****************************************************************************/
//  Description : append one icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_AppendIcon(
                                        uint16              index,          //in
                                        MMI_CTRL_ID_T       ctrl_id,        //in
                                        GUIANIM_DATA_INFO_T *data_info_ptr, //in
                                        GUIANIM_FILE_INFO_T *file_info_ptr  //in
                                        );

/*****************************************************************************/
//  Description : set selected item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLICONFOLDER_SetSelected(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         BOOLEAN        is_selected
                                         );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
