/*****************************************************************************
** File Name:      ctrlsetlist.h                                               *
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

#ifndef _CTRLSETLIST_H_
#define _CTRLSETLIST_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlsetlist_export.h"

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

// setlist type info
#define CTRL_SETLIST_TYPE     (SETLIST_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// setlist init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLSETLIST_INIT_PARAM_T;

//setlist control
typedef struct _setlist_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;
    
    GUISETLIST_STATE_T      ctrl_state;      /*!< ��¼�ؼ�״̬*/
    GUISETLIST_TYPE_E       list_type;       /*!< ��¼�ؼ�״̬*/   

    BOOLEAN                 is_valid;        /*!< ��Ч��ʱ��,���ɼ�,
Ҳ��������κ���Ϣ,��Ĭ������¿ɼ�*/
    BOOLEAN                 is_need_arrow;   /*!< �Ƿ���Ҫ��ʾ���Ҽ�ͷ,
Ĭ������²���ʾ���û��ɵ�����غ�������*/
    BOOLEAN                 left_is_pressed; /*!< ����Ƿ񱻰���*/
    BOOLEAN                 right_is_pressed;/*!< �Ҳ��Ƿ񱻰���*/
    
    MMI_GUISETLIST_STYLE_T  default_style;   /*!< �ؼ���Ĭ������*/

    uint32                  item_max_num;    /*!< �����*/
    uint32                  cur_item_index;  /*!< ��ǰitem������*/
    uint32                  selected_cnt;    /*!< ѡ��item����*/
    
    BOOLEAN                 is_disptxt_auto; /*!< 
˵���ַ����ɿؼ���ϻ������û�����*/
    MMI_STRING_T            title_str_info;  /*!< ����ַ�����setlist��˵��*/
    MMI_STRING_T            disp_str;        /*!< 
����ַ������ڶ�ѡʱ��ʾ�ַ�*/
    GUI_RECT_T              tp_rect;         /*!< receive tp msg*/
    
    GUISETLIST_ITEM_T       item_info;       /*!< item����ָ��*/

    MMI_WIN_ID_T            win_id;          /*!< window id*/

    // self member ...
} CTRLSETLIST_OBJ_T;

// setlist class for public or protected method
typedef struct _setlist_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLSETLIST_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get setlist type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T SETLIST_GetType(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
