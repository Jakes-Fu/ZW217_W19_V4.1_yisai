/*****************************************************************************
** File Name:      ctrlform.h                                                *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

#ifndef _CTRLFORM_H_
#define _CTRLFORM_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlform_export.h"
#include "guiprgbox.h"
#include "guictrl.h"
#include "mmi_theme.h"
#include "mmitheme_form.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
//form type info
#define CTRL_FORM_TYPE     (FORM_GetType())

#define CTRLFORM_PERCENT_MAX 100 //control percent

#ifdef WIN32
#define CTRLFORM_DRUMP_LAYOUT_INFO
#endif

#ifdef CTRLFORM_DRUMP_LAYOUT_INFO
// 是否需要打印layout信息，定义在CTRLFORM_internal.c里
extern BOOLEAN g_trace_layout_info;

// 打印layout信息的宏
#define CTRLFORM_TRACE_LAYOUT    \
    if (g_trace_layout_info)        \
SCI_TRACE_LOW
#else
#define CTRLFORM_TRACE_LAYOUT(...)
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND STRUCT                                   *
**---------------------------------------------------------------------------*/
//init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T   base_ctrl_param;
    
    GUIFORM_LAYOUT_E        layout_type;/*!<子控件排列方式*/
} CTRLFORM_INIT_PARAM_T;

//form add child init data
typedef struct
{
    BOOLEAN             is_bg;          //is bg child
    BOOLEAN             is_softkey;     //is softkey child
    BOOLEAN             is_get_active;  //is get active
    void                *ctrl_ptr;      //control pointer
    MMI_CTRL_ID_T       ctrl_id;        //control id
    MMI_HANDLE_T        parent_handle;  //parent handle
} CTRLFORM_CHILD_INIT_T;

//form size rang
typedef struct
{
    uint16      min_size;   //min size
    uint16      max_size;   //max size
} CTRLFORM_SIZE_RANGE_T;

//form bg backup
typedef struct
{
    BOOLEAN     is_save;    //is save rgb565 data
    uint8       *buf_ptr;   //buffer
    uint16      img_width;  //image width
    uint16      img_height; //image height
} CTRLFORM_BG_BAK_T;

//form child
typedef struct ctrlform_child_tag
{
    BOOLEAN                     is_bg;              //is bg child
    BOOLEAN                     is_get_active;      //is get active
    
    uint16                      index;              //child index
    uint16                      hor_space;          //space relative to the upper child
    uint16                      ver_space;          //space relative to the left child
    uint16                      width;              //child width
    uint16                      height;             //child height
    uint16                      indent_width;       //child indentation width

    uint16                      dock_priority;      //dock, priority,1 is max,0 is not dock
    GUIFORM_CHILD_DOCK_E        dock_type;          //dock type

    CTRLFORM_SIZE_RANGE_T       size_range;         //child height range
    
    GUIFORM_CHILD_ALIGN_E       align;              //align
    
    IGUICTRL_T                  *child_ctrl_ptr;    //control pointer
    MMI_HANDLE_T                ctrl_handle;
    GUI_BIG_RECT_T              relative_rect;      //relative rect,relative to the form
    
    GUIFORM_CHILD_WIDTH_T       width_info;         //width info
    GUIFORM_CHILD_HEIGHT_T      height_info;        //height info
    GUIFORM_CHILD_DISPLAY_E     display_type;       //display type
    
    struct ctrlform_child_tag   *prev_ptr;          //previous child pointer
    struct ctrlform_child_tag   *next_ptr;          //next child pointer
} CTRLFORM_CHILD_T;

typedef struct
{
    BOOLEAN                     is_on_slide_paint;  //is on slide paint
    uint8                       slide_timer_id;     //slide timer id
    float                       fling_velocity;     //fling velocity
    GUI_POINT_T                 start_point;        //slide start point
    GUI_POINT_T                 prev_point;         //slide previous point
    MMK_TP_SLIDE_E              slide_state;        //slide state
    MMK_TP_SLIDE_E              tpdown_slide_state; //tp down slide state
} CTRLFORM_SLIDE_INFO_T;

//form control
typedef struct _ctrlform_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;
    
    BOOLEAN                     is_display_prg;     //is display progress
    BOOLEAN                     is_permit_bg;       //child is permit set bg
    BOOLEAN                     is_permit_border;   //child is permit set border
    BOOLEAN                     is_permit_font;     //child is permit set font size
    BOOLEAN                     is_permit_font_color;     //child is permit set font  color
    BOOLEAN                     is_auto_dir;        //是否根据语言自动调整child显示的相对位置
    
    BOOLEAN                     is_valid_child;     //is has valid child,not bg and hide,include dock child
    BOOLEAN                     is_tp_down;         //is tp down
    BOOLEAN                     is_child_layer;     //child is need layer only for list
    BOOLEAN                     is_tp_in_dock;      //is tp in dock
    
    uint16                      first_valid_index;  //first valid child index,not bg, hide and dock child
    uint16                      last_valid_index;   //last valid child index,not bg, hide and dock child
    uint16                      layout_index;       //layout start child index,== child_num has layout
    uint16                      child_num;          //child number,except dock child
    uint16                      all_child_num;      //all child number, include dock child number
    uint16                      active_child_num;   //get active child number
    
    uint16                      sbs_width;          //all child width only for sbs
    uint16                      child_max_height;   //child max height

    CTRLFORM_SIZE_RANGE_T       size_range;         //form height range
    
    GUI_RECT_T                  original_rect;      //form original rect
    GUI_RECT_T                  writeback_rect;     //write back rect,for improve slide velocity
    GUI_BIG_RECT_T              relative_rect;      //relative rect(去除dock区域),relative to the form with except border and padding,原点是form除去边框和padding
    GUI_BIG_RECT_T              relative_disp_rect; //relative display rect(去除dock区域),relative to the form with except border and padding,原点是form除去边框和padding
    CTRLFORM_BG_BAK_T           bg_backup;          //back up bg,避免多次解码
    
    CTRLFORM_CHILD_T            *first_child_ptr;       //first child pointer
    CTRLFORM_CHILD_T            *cur_child_ptr;         //control display child
    CTRLFORM_CHILD_T            *active_child_ptr;      //active child pointer
    CTRLFORM_CHILD_T            *prev_active_child_ptr; //prev active child pointer

    CTRLFORM_CHILD_T            *bg_child_ptr;          //background child pointer
    CTRLFORM_CHILD_T            *softkey_child_ptr;     //softkey child pointer

#ifdef GUIF_FORM_DOCKABLE
    uint16                      cur_priority;       //dock priority,0 is max
    CTRLFORM_CHILD_T            *top_child_ptr;     //top child
    CTRLFORM_CHILD_T            *bottom_child_ptr;  //bottom child
    CTRLFORM_CHILD_T            *left_child_ptr;    //left child
    CTRLFORM_CHILD_T            *right_child_ptr;   //right child
#endif
    
    GUIFORM_LAYOUT_E            layout_type;        //layout type
    GUIFORM_TYPE_E              type;               //form type
    GUIFORM_STYLE_E             style;              //form style
    GUIFORM_CHILD_ALIGN_E       align;              //align,only for sbs
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
	GUIFORM_CHILD_ALIGN_E       child_align;        //align for all child,only for layout order
#endif
    MMI_DIRECTION_SET_E         display_dir;        //display direction when layout
    
    MMI_RESULT_E                tpshort_result;     //child control is handle tp short msg
    MMI_HANDLE_T                tpdown_ctrl_handle; //child control which handle tp down msg
    
    CTRLFORM_SLIDE_INFO_T       slide_info;         //slide info
    
    IGUICTRL_T                  *prgbox_ctrl_ptr;   //progress box control pointer
    struct _ctrlform_obj_tag    *tp_form_ctrl_ptr;  //handle tp msg form control,may dock form
    
    MMITHEME_FORM_T             theme;              //form theme
    
    MMI_HANDLE_T                win_handle;         //window handle
} CTRLFORM_OBJ_T;

//form class for public or protected method
typedef struct _form_vtbl_tag
{
    CTRLBASE_VTBL_T        base_ctrl_vtbl;
} CTRLFORM_VTBL_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get form type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T FORM_GetType(void);

/*****************************************************************************/
//  Description : get form pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLFORM_OBJ_T* CTRLFORM_GetPtr(
                                       MMI_HANDLE_T  ctrl_handle
                                       );

/*****************************************************************************/
//  Description : init form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_InitDisplayRect(
                                     CTRLFORM_OBJ_T *form_ctrl_ptr
                                     );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : get top/bottom dock child height,include space etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDockChildHeight(
                                          CTRLFORM_CHILD_T  *child_ptr      //in:
                                          );

/*****************************************************************************/
//  Description : get left/right dock child width,include space etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDockChildWidth(
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         );
#endif

/*****************************************************************************/
//  Description : get form rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetRect(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get content rect,except border and padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetContentRect(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : get form relative width,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLFORM_GetRelativeWidth(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get form relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLFORM_GetRelativeHeight(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetScrollBarRect(
                                            CTRLFORM_OBJ_T   *form_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetScrollBarWidth(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : get child top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildTop(
                                  CTRLFORM_CHILD_T  *child_ptr      //in:
                                  );

/*****************************************************************************/
//  Description : get child bottom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildBottom(
                                     CTRLFORM_CHILD_T   *child_ptr      //in:
                                     );

/*****************************************************************************/
//  Description : get child left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildLeft(
                                   CTRLFORM_CHILD_T *child_ptr      //in:
                                   );

/*****************************************************************************/
//  Description : get child right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildRight(
                                    CTRLFORM_CHILD_T    *child_ptr      //in:
                                    );

/*****************************************************************************/
//  Description : get child width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetChildWidth(
                                     CTRLFORM_CHILD_T   *child_ptr      //in:
                                     );

/*****************************************************************************/
//  Description : get child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetChildHeight(
                                      CTRLFORM_CHILD_T  *child_ptr      //in:
                                      );

/*****************************************************************************/
//  Description : adjust height by size range
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_AdjustHeightByRange(
                                         uint16                 *height_ptr,//in/out:
                                         CTRLFORM_SIZE_RANGE_T  *range_ptr  //in:
                                         );

/*****************************************************************************/
//  Description : get form display rect min top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMinTop(
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : get form display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxTop(
                                       CTRLFORM_OBJ_T    *form_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : get form display rect min left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMinLeft(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get form display rect max left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxLeft(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get form display rect max right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxRight(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         );

/*****************************************************************************/
//  Description : get form display rect width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDisplayWidth(
                                       BOOLEAN          is_except_prg,
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : get form display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDisplayHeight(
                                        CTRLFORM_OBJ_T   *form_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get form display rect by display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T CTRLFORM_GetDisplayRect(
                                              int32          display_top,    //in:
                                              CTRLFORM_OBJ_T *form_ctrl_ptr  //in:
                                              );

/*****************************************************************************/
//  Description : set form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDisplayRect(
                                       int32             display_top,    //in:
                                       int32             display_left,   //in:
                                       CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                       );

/*****************************************************************************/
//  Description : get first and last valid child index,judge valid child is exist
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_GetValidChild(
                                   CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                   );

/*****************************************************************************/
//  Description : get child control pointer by index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetChildPtrByIndex(
                                           uint16           index,          //in:
                                           CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                                           CTRLFORM_CHILD_T **child_pptr    //in/out:
                                           );

/*****************************************************************************/
//  Description : get child control pointer by handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetChildPtrByHandle(
                                            MMI_HANDLE_T        ctrl_handle,    //in:
                                            CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                                            CTRLFORM_CHILD_T    **child_pptr    //in/out:
                                            );

/*****************************************************************************/
//  Description : child is valid
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsValidChild(
                                     CTRLFORM_CHILD_T     *child_ptr
                                     );

/*****************************************************************************/
//  Description : form is get active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGetAtv(
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : form is gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGray(
                               CTRLFORM_OBJ_T    *form_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : child is get active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGetAtvChild(
                                      CTRLFORM_CHILD_T    *child_ptr
                                      );

/*****************************************************************************/
//  Description : child is form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsFormChild(
                                    CTRLFORM_CHILD_T  *child_ptr
                                    );

/*****************************************************************************/
//  Description : is order form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsOrderForm(
                                    CTRLFORM_OBJ_T   *form_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : is sbs form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSbsForm(
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : is unit form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitForm(
                                   CTRLFORM_OBJ_T    *form_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : is unit form and it only include one control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitFormSingle(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : is unit form child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitFormChild(
                                        CTRLFORM_CHILD_T  *child_ptr
                                        );

/*****************************************************************************/
//  Description : form child is active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAtvFormChild(
                                       CTRLFORM_CHILD_T   *child_ptr
                                       );

/*****************************************************************************/
//  Description : child is anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAnimChild(
                                    CTRLFORM_CHILD_T  *child_ptr
                                    );

/*****************************************************************************/
//  Description : child is dock top/bottom or left/right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsDockChild(
                                    CTRLFORM_CHILD_T    *child_ptr
                                    );

/*****************************************************************************/
//  Description : child is set min/max size
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSetSize(
                                  CTRLFORM_CHILD_T  *child_ptr
                                  );

/*****************************************************************************/
//  Description : is display child from right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsDisplayFromRight(
                                           CTRLFORM_OBJ_T    *form_ctrl_ptr
                                           );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : get the dock child pointer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLFORM_CHILD_T* CTRLFORM_GetDockChildPtr(
                                                  uint16            *priority_ptr,  //in/out:
                                                  CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                                  );
#endif

/*****************************************************************************/
//  Description : update child display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_UpdateChild(
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr,
                                    CTRLFORM_CHILD_T    *child_ptr,
                                    BOOLEAN             is_tp_down,
                                    BOOLEAN             is_update
                                    );

/*****************************************************************************/
//  Description : convert relative rect to screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_ClientToScreen(
                                    CTRLFORM_OBJ_T  *form_ctrl_ptr, //in:
                                    GUI_BIG_RECT_T  *rect_ptr       //in/out:may PNULL
                                    );

/*****************************************************************************/
//  Description : convert screen rect to relative rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_ScreenToClient(
                                    CTRLFORM_OBJ_T  *form_ctrl_ptr, //in:
                                    int32           *left_ptr,      //in/out:may PNULL
                                    int32           *top_ptr,       //in/out:may PNULL
                                    int32           *right_ptr,     //in/out:may PNULL
                                    int32           *bottom_ptr     //in/out:may PNULL
                                    );

/*****************************************************************************/
//  Description : display form control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_Display(
                             BOOLEAN         is_paint,
                             BOOLEAN         is_update_prgbox,
                             CTRLFORM_OBJ_T  *form_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : display form control by rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DisplayRect(
                                 GUI_RECT_T      *rect_ptr,      //in:
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : display form dividing line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_DisplayChildLine(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                                      CTRLFORM_CHILD_T  *child_ptr,     //in:
                                      GUI_RECT_T        child_rect      //in:
                                      );

/*****************************************************************************/
//  Description : layout all child control, set param and set display rect etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_Layout(//is re-layout
                               CTRLFORM_OBJ_T    *form_ctrl_ptr, //in:
                               BOOLEAN           is_first_layout,//in:
                               BOOLEAN           is_update       //in:
                               );

/*****************************************************************************/
//  Description : set form param,include bg,border etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetParam(//is unit form
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                 );

/*****************************************************************************/
//  Description : set child control param,include bg,border,font etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetChildParam(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr, //in:
                                   CTRLFORM_CHILD_T *child_ptr      //in:
                                   );

/*****************************************************************************/
//  Description : set child control display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_SetChildRect(
                                  BOOLEAN            is_update,      //in:
                                  CTRLFORM_OBJ_T     *form_ctrl_ptr  //in:
                                  );

/*****************************************************************************/
//  Description : get form height by width,include border,padding etc
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetHeightByWidth(
                                         CTRLBASE_OBJ_T *base_ctrl_ptr, //in:
                                         uint16         width,          //in:
                                         uint16         *height_ptr     //in/out:
                                         );

/*****************************************************************************/
//  Description : adjust display top by select rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_AdjustDisplayTopBySelect(
                                                 int32               *display_top_ptr,   //in:
                                                 CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                                                 CTRLFORM_CHILD_T    *child_ptr          //in:
                                                 );

/*****************************************************************************/
//  Description : get display top for handle key up or down message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_GetDisplayTopForKey(
                                            int32               *disp_top_ptr,      //in/out:
                                            int32               *disp_left_ptr,     //in/out:
                                            CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                                            CTRLFORM_CHILD_T    **active_child_pptr,//in/out:
                                            MMI_MESSAGE_ID_E    msg_id              //in:
                                            );

/*****************************************************************************/
//  Description : get display top by scroll step
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayTopByStep(
                                          BOOLEAN           is_up,          //in:
                                          int32             scroll_step,    //in:
                                          int32             display_max_top,//in:up,may 0
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr  //in:
                                          );

/*****************************************************************************/
//  Description : rect1 is covered rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsRectCovered(
                                      GUI_BIG_RECT_T     rect1,
                                      GUI_BIG_RECT_T     rect2
                                      );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : calculate all dock child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_CalcAllDockChild(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                      );
#endif

/*****************************************************************************/
//  Description : calculate all child control width/height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_CalcAllChild(//is create prg
                                     CTRLFORM_OBJ_T *form_ctrl_ptr  //in/out:
                                     );

/*****************************************************************************/
//  Description : form dispatch tp msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E CTRLFORM_DispatchTpMsg(
                                           CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                           MMI_MESSAGE_ID_E  msg_id,
                                           DPARAM            param
                                           );

/*****************************************************************************/
//  Description : handle form tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpDown(
                                  CTRLFORM_OBJ_T *form_ctrl_ptr,
                                  GUI_POINT_T    *tp_point_ptr
                                  );

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_StopTpSlideTimer(
                                      CTRLFORM_OBJ_T     *form_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : handle form tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpMove(
                                  CTRLFORM_OBJ_T *form_ctrl_ptr,
                                  GUI_POINT_T    *tp_point_ptr
                                  );

/*****************************************************************************/
//  Description : handle form tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpUp(
                                CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                GUI_POINT_T      *tp_point_ptr
                                );

/*****************************************************************************/
//  Description : handle form tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleSlideTimer(
                                      CTRLFORM_OBJ_T     *form_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : reset form slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_ResetSlide(
                                BOOLEAN          is_update,
                                CTRLFORM_OBJ_T   *form_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : remove child from list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_RemoveChildFromList(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         );

//zmt add start
/*****************************************************************************/
//  Description : set form no display scrollbar
//  Global resource dependence : 
//  Author: fys
//  Note:2024.06.29
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_SetDisplayScrollBar(
                                MMI_HANDLE_T form_handle, 
                                BOOLEAN is_display
                                );
//zmt add end
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
