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
// 判断发给输入法同步消息的类型
typedef enum
{
    CTRLLISTEDIT_IMSYNC_NONE,       // 不需要同步
    CTRLLISTEDIT_IMSYNC_ADD,        // 因为加字符而同步
    CTRLLISTEDIT_IMSYNC_DEL,        // 因为删字符而同步
} CTRLLISTEDIT_IMSYNC_E;

// 用于记录list风格的文本对象信息
typedef struct guiedit_obj_info_tag
{
    uint16              start;          // 起始位置
    uint16              len;            // 长度
    MMI_STRING_T        user_data;      // 用户数据，GUIEDIT_GetListItemString获得这里的信息
    struct guiedit_obj_info_tag *next;  // 指向下一个
} GUIEDIT_OBJ_INFO_T;

// listedit init param
typedef struct
{
    CTRLBASEFLEX_INIT_PARAM_T       baseflex_ctrl_param;

    uint16                          item_num;       /*!<item个数*/
    uint16                          item_max_len;   /*!<每个item的最大字符长度*/
} CTRLLISTEDIT_INIT_PARAM_T;

//listedit control
typedef struct
{
    CTRLBASEFLEX_OBJ_T              baseflex_ctrl;

    GUIEDIT_OBJ_INFO_T              *obj_info_ptr;      /*!<list风格下文本对象信息*/

    uint16                          item_num;       /*!<item个数*/
    uint16                          max_num;        /*!<最大item个数*/
    uint16                          item_max_len;   /*!<每个item最大字符个数*/

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
