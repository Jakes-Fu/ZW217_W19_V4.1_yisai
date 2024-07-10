/*****************************************************************************
** File Name:      ctrltab.h                                               *
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

#ifndef _CTRLTAB_H_
#define _CTRLTAB_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrltab_export.h"

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
// tab type info
#define CTRL_TAB_TYPE     (TAB_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// tab init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T        base_ctrl_param;
    GUITAB_STATE_T               state;					/*!< tab״̬*/
    GUI_TAB_STYLE_E              style;	                /*!< ���䷽ʽ */
    uint32                       max_item_num;	        /*!< ������������ֵΪ ��\link #GUITAB_MAX_ITEM_NUM \endlink*/
    BOOLEAN                      is_active_item;        /*!< ��*/    // to load item actively

    // self inif param ...
} CTRLTAB_INIT_PARAM_T;

//tab control
typedef struct _tab_obj_tag
{
    CTRLBASE_OBJ_T          base_ctrl;
    GUITAB_STATE_T          tab_state;                  /*!< �ؼ�״̬*/

    // long press timer parameter
    uint32                  timeout;                    /*!< timer������ʱ�޼����ϵͳ����*/
    uint32                  auto_switch_time;           /*!< �Զ��л�����һ����ϵͳ����*/
    GUITAB_TIMER_STATE_T    timer_status;               /*!< ��ǰtimer״̬��ϵ����*/
    uint8                   timer_id;                   /*!< timer id ϵͳ����*/
    uint8                   text_timer_id;              /*!<item text������ʱ��*/
    uint16                  str_index;                  /*!<current item string start index*/

    BOOLEAN                 is_tp_down;                 /*!< �Ƿ�tp down*/
    uint32                  tp_down_item_index;         /*!< tp down item������*/
    GUI_POINT_T             tp_prev_point;              /*!< tpǰһ���������*/

    // information of tab item
    uint32                  max_item_num;               /*!< ���item��*/
    uint32                  total_item_num;             /*!< item������С�ڵ��������max_item_num*/
    uint32                  show_item_num;              /*!< ʵ������ʾ��item�������ݿؼ���ȼ������*/
    uint32                  cur_sel_item_index;         /*!< ��ǰitem������*/
    uint32                  pre_sel_item_index;         /*!< ǰһ��item������*/
    uint32                  start_show_item_index;      /*!< ��ʼ��ʾ*/
    uint32                  item_space;                 /*!< ���*/
    MMI_TAB_STYLE_T         tab_style;                  /*!< �ؼ�Ĭ�Ϸ��*/

    uint32                  item_height;                /*!< ��Ч����߶ȣ���������ʾtitle����*/
    uint32                  item_width;                 /*!< tab����Ч����Ŀ��*/
    GUI_RECT_T              item_rect;                  /*!< tab����ʾ����������������Ҽ�ͷ����Ҫȥ����ͷ��ռ����*/
    GUITAB_ITEM_T*          item_ptr;                   /*!< ����������*/
    GUITAB_ITEM_LIST_T      valid_item_list;            /*!< ��Ч������������*/

    int32                   cur_object_index;           /*!< ����tab��ͼƬ���ã���ʾ��ǰ���ڽ��������������*/
    // scroll parameter
    BOOLEAN                 is_scroll;                  /*!< �ؼ��Ƿ���Ҫ������ͷ*/
    BOOLEAN                 is_rotative;                /*!< �Ƿ�ѭ������*/
    GUITAB_SCROLL_CARD_T    left_scroll_card;           /*!< ����������Ϣ*/
    GUITAB_SCROLL_CARD_T    right_scroll_card;          /*!< �Ҳ��������Ϣ*/

    int32                   offset_x;                   /*!< ��һ��item���������ڿؼ�����x�����ƫ��*/

    // display style
    GUI_TAB_STYLE_E         style;                      /*!< TAB��ʾ��񣬼����뷽ʽ��Ŀǰ֧�������*/
    
    MMI_STRING_T            title_str_info;             /*!< ����ַ�����TAB�ؼ���˵������GUITAB_STATE_COMMON_TITLE��Чʱʹ��*/
    
    GUITAB_CAPTION_T        added_string;               /*!< ������Ϣ*/
    BOOLEAN                 is_reset_title;             /*!< �Ƿ�������������*/
    BOOLEAN                 is_reset_common_title;      /*!< �Ƿ���������ͳһ��title��ֻ��Ҫ����һ��*/
    ADD_DATA                title_ptr;                  /*!< ���ڼ�¼��Ƕ��title�ؼ�ָ��*/

    // self member ...
} CTRLTAB_OBJ_T;

// tab class for public or protected method
typedef struct _tab_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLTAB_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get tab type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TAB_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
