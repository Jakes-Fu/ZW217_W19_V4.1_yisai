/****************************************************************************
** File Name:      ctrltoolbar.h                                            *
** Author:         James.Zhang                                              *
** Date:           9/02/2010                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 9/2010         James.Zhang      Create                                   *
**                                                                          *
****************************************************************************/

#ifndef _CTRLTOOLBAR_H_
#define _CTRLTOOLBAR_H_ 

#ifdef GUIF_TOOLBAR 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "guictrl.h"
#include "guitoolbar.h"
#include "mmitheme_toolbar.h"

#include "ctrlbase.h"
#include "ctrltoolbar_export.h"

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
// toolbar type info
#define CTRL_TOOLBAR_TYPE     (TOOLBAR_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef uint32 GUITOOLBAR_STATE_T;
#define GUITOOLBAR_STATE_INVALID           0x01     //需要重新计算区域等数据
#define GUITOOLBAR_STATE_VISIBLE           0x02     //可见 

//item info struct
typedef struct
{
    GUITOOLBAR_ITEM_T item;
    GUI_RECT_T        rect;
    BOOLEAN           is_gray;   //invalidate  
}GUITOOLBAR_ITEM_INFO_T;

// toolbar pop anim
typedef struct _TOOLBAR_POP_ANIM_
{
    GUITOOLBAR_ANIM_DIRECTION_E            dirction;
    GUITOOLBAR_ANIM_ACTION_E               act;
    GUI_RECT_T                  rect_start;
    GUI_RECT_T                  rect_stop;
    GUI_RECT_T                  rect_real;
    BOOLEAN                     is_poping;
    //uint8                       timer_id;
}GUITOOLBAR_POP_ANIM_T;

// toolbar auto shrink
typedef struct _TOOLBAR_AUTO_SHRINK
{
    BOOLEAN     is_open;
    uint32      interval;
    uint8       timer_id;
}GUITOOLBAR_AUTO_SHRINK_T;

// runtime status
typedef struct _TOOLBAR_RUNTIME_INFO
{
    uint16                      hit_index;
    uint16                      cur_page;
    uint16                      cur_page_items;
    uint16                      page_max_items;
    BOOLEAN                     is_show_end;
    BOOLEAN                     is_hit_more;
    BOOLEAN                     is_hit_icon;
    GUI_RECT_T                  rect_more;  
}GUITOOLBAR_RUNTIME_INFO;

// resource
typedef struct _TOOLBAR_RES_INFO
{
    uint16                      space; 
    uint16                      separator_width;
    uint16                      icon_width;
    uint16                      icon_height;   
}GUITOOLBAR_RES_ATTR;

// toolbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
} CTRLTOOLBAR_INIT_PARAM_T;

//toolbar control
typedef struct _toolbar_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    MMITHEMETOOLBAR_STYLE_T     style;
    GUI_ALIGN_E                 align;
    GUITOOLBAR_STATE_T          toolbar_state;
    GUITOOLBAR_ITEM_INFO_T*     item_info_ptr;
    uint16                      total_num;
    uint16                      click_index;
    GUITOOLBAR_RUNTIME_INFO     run_time;
    GUITOOLBAR_SHOW_TYPE_E      show_type;
    GUI_RECT_T                  client_rect;
    MMI_HANDLE_T                win_handle;
    GUITOOLBAR_POP_ANIM_T       pop_anim;
    GUITOOLBAR_AUTO_SHRINK_T    auto_shrink;
    GUITOOLBAR_RES_ATTR         res_attr;
    TOOLBAR_PRE_EXPAND_FUNC     func_ptr;
    void *                      param_ptr;   
    GUI_RECT_T                  icon_rect;
    GUI_POINT_T                 panel_pt;   // 左下
    GUI_RECT_T                  panel_rect;

} CTRLTOOLBAR_OBJ_T;

// toolbar class for public or protected method
typedef struct _toolbar_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLTOOLBAR_VTBL_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get toolbar type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TOOLBAR_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif


