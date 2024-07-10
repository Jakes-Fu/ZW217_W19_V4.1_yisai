/*****************************************************************************
** File Name:      ctrllistedit.h                                               *
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

#ifndef _CTRLLISTEDIT_H_
#define _CTRLLISTEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseflex.h"
#include "ctrllistedit_export.h"

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
// listedit type info
#define CTRL_LISTEDIT_TYPE     (LISTEDIT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// �жϷ������뷨ͬ����Ϣ������
typedef enum
{
    CTRLLISTEDIT_IMSYNC_NONE,       // ����Ҫͬ��
    CTRLLISTEDIT_IMSYNC_ADD,        // ��Ϊ���ַ���ͬ��
    CTRLLISTEDIT_IMSYNC_DEL,        // ��Ϊɾ�ַ���ͬ��
} CTRLLISTEDIT_IMSYNC_E;

// ���ڼ�¼list�����ı�������Ϣ
typedef struct guiedit_obj_info_tag
{
    uint16              start;          // ��ʼλ��
    uint16              len;            // ����
    MMI_STRING_T        user_data;      // �û����ݣ�GUIEDIT_GetListItemString����������Ϣ
    struct guiedit_obj_info_tag *next;  // ָ����һ��
} GUIEDIT_OBJ_INFO_T;

// listedit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    uint16                          item_num;       /*!<item����*/
    uint16                          item_max_len;   /*!<ÿ��item������ַ�����*/
} CTRLLISTEDIT_INIT_PARAM_T;

//listedit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    GUIEDIT_OBJ_INFO_T              *obj_info_ptr;      /*!<list������ı�������Ϣ*/

    uint16                          item_num;       /*!<item����*/
    uint16                          max_num;        /*!<���item����*/
    uint16                          item_max_len;   /*!<ÿ��item����ַ�����*/

    CTRLLISTEDIT_IMSYNC_E           im_sync_type;

    MMITHEME_EDIT_LIST_T            theme;       //list theme
} CTRLLISTEDIT_OBJ_T;

// listedit class for public or protected method
typedef struct
{
    CTRLBASEFLEX_VTBL_T             baseflex_ctrl_vtbl;

    // self vtbl ...
} CTRLLISTEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get listedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T LISTEDIT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
