/***************************************************************************
** File Name:      ctrlbase.h                                              *
** Author:         hua.fang                                                *
** Date:           08/03/2011                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _CTRLBASE_H_
#define _CTRLBASE_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "caf.h"
#include "cafcontrol.h"
#include "cafshell.h"
#include "cafhelp.h"
#include "cafdc.h"

#include "type_manager.h"
#include "ctrlbase_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define CTRL_BASE_TYPE     (BASE_GetType())

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// init param for base control
typedef struct
{
    GUI_BOTH_RECT_T             both_rect;  /*!<横竖屏区域*/
} CTRLBASE_INIT_PARAM_T;

// base control object struct
// !!! 重要 !!!
// 该结构必须要与cafcontrol.h的CAF_GUICTRL_DATA_MEMBER保持一致
typedef struct
{
    OBJECT_TYPE_T               object;

    MMI_HANDLE_T                win_handle;         // window handle
    CAF_HANDLE_T                handle;
    MMI_CTRL_ID_T        		ctrl_id;

    CAF_BOTH_RECT_T             both_rect;
    CAF_RECT_T                  rect;
    CAF_RECT_T                  display_rect;

    CAF_LCD_DEV_INFO            lcd_dev_info;

    GUI_BG_T                    bg;
    GUI_BORDER_T                border;

    GUI_PADDING_T               padding;

    BOOLEAN                     can_active;
    BOOLEAN                     can_kbd_active;
    BOOLEAN                     is_active;
    BOOLEAN                     is_visible;
    BOOLEAN                     is_enabled;
    BOOLEAN                     is_tp_enabled;
    BOOLEAN                     is_kbd_enabled;

    CTRL_TP_PRIORITY_E          tp_priority;

    uint32                      user_data;                // user data

    MMI_TREE_NODE_T*            tree_node_ptr;

    BOOLEAN                     is_slide;

    GUICTRL_STATE_T             state;
} CTRLBASE_OBJ_T;

// base control vtbl for public or protected method
typedef struct _basectrl_vtbl_tag
{
    OBJECT_VTBL_T               type_vtbl;

    MMI_RESULT_E (*HandleEvent) (CTRLBASE_OBJ_T* me_ptr, MMI_MESSAGE_ID_E msg_id, void* param_ptr);
    BOOLEAN (*SetBothRect) (CTRLBASE_OBJ_T* me_ptr, const GUI_BOTH_RECT_T* both_rect_ptr);
    BOOLEAN (*GetBothRect) (CTRLBASE_OBJ_T* me_ptr, GUI_BOTH_RECT_T* both_rect_ptr);
    BOOLEAN (*SetRect) (CTRLBASE_OBJ_T* me_ptr, const GUI_RECT_T* rect_ptr);
    BOOLEAN (*GetRect) (CTRLBASE_OBJ_T* me_ptr, GUI_RECT_T* rect_ptr);
    BOOLEAN (*SetDisplayRect) (CTRLBASE_OBJ_T* me_ptr, const GUI_RECT_T* rect_ptr, BOOLEAN is_update);
    BOOLEAN (*GetDisplayRect) (CTRLBASE_OBJ_T* me_ptr, GUI_RECT_T* rect_ptr);
    BOOLEAN (*SetLcdDevInfo) (CTRLBASE_OBJ_T* me_ptr, const GUI_LCD_DEV_INFO* info_ptr);
    BOOLEAN (*GetLcdDevInfo) (CTRLBASE_OBJ_T* me_ptr, GUI_LCD_DEV_INFO* info_ptr);
    BOOLEAN (*SetBg) (CTRLBASE_OBJ_T* me_ptr, GUI_BG_T* bg_ptr);
    BOOLEAN (*SetBorder) (CTRLBASE_OBJ_T* me_ptr, GUI_BORDER_T* border_ptr);
    BOOLEAN (*SetFont) (CTRLBASE_OBJ_T* me_ptr, GUI_FONT_ALL_T* font_ptr);
    BOOLEAN (*GetFont) (CTRLBASE_OBJ_T* me_ptr, GUI_FONT_ALL_T* font_ptr);
    BOOLEAN (*SetPadding) (CTRLBASE_OBJ_T* me_ptr, GUI_PADDING_T* padding_ptr);
    BOOLEAN (*SetSlideState) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_slide);
    BOOLEAN (*GetSlideState) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetCanActive) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_active);
    BOOLEAN (*GetCanActive) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetCanKbdActive) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_active);
    BOOLEAN (*GetCanKbdActive) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetActive) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_active);
    BOOLEAN (*GetActive) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetEnabled) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_enabled);
    BOOLEAN (*GetEnabled) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetVisible) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_visible);
    BOOLEAN (*GetVisible) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetKbdEnabled) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_kbd_enabled);
    BOOLEAN (*GetKbdEnabled) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetTpEnabled) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_tp_enabled);
    BOOLEAN (*GetTpEnabled) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetTpPriority) (CTRLBASE_OBJ_T* me_ptr, CTRL_TP_PRIORITY_E tp_priority);
    CTRL_TP_PRIORITY_E (*GetTpPriority) (CTRLBASE_OBJ_T* me_ptr);
    BOOLEAN (*SetScrollbarEnabled) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_scrollbar_enabled);
    BOOLEAN (*SetUpDownLoop) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_loop);
    BOOLEAN (*SetLeftRightLoop) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_loop);
    BOOLEAN (*GetHeightByWidth) (CTRLBASE_OBJ_T* me_ptr, uint16 width, uint16* height_ptr);
    BOOLEAN (*GetSelectRect) (CTRLBASE_OBJ_T* me_ptr, GUI_RECT_T* rect_ptr);
    BOOLEAN (*GetSelectIndex) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN *is_first_ptr, BOOLEAN *is_last_ptr);
    BOOLEAN (*SetSelectIndex) (CTRLBASE_OBJ_T* me_ptr, BOOLEAN is_first, BOOLEAN is_last);

    // 该方法是为了兼容Softkey对原来SetState虚方法的重载，提供此方法用来设置它的button
    void (*OnStateChanged) (CTRLBASE_OBJ_T* me_ptr, GUICTRL_STATE_T state, BOOLEAN is_true);
} CTRLBASE_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get base control type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASE_GetType(void);

/*****************************************************************************/
// 	Description : update
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void BASE_Update (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : get ctrl guid
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T BASE_GetCtrlGuid (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : set gui control handle
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void BASE_SetCtrlHandle (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    CAF_HANDLE_T      ctrl_handle
);

/*****************************************************************************/
// 	Description : get gui control handle
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T BASE_GetCtrlHandle (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
);

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN BaseCtrlGetDisplayRectNotImp (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,  //in:
    GUI_RECT_T            *rect_ptr        //out:
);

/*****************************************************************************/
// 	Description : set gui control handle
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline MMI_RESULT_E VTLBASE_HandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,
    CAF_MESSAGE_ID_E      msg_id,
    void*                 param_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->HandleEvent (base_ctrl_ptr, msg_id, param_ptr);
}

/*****************************************************************************/
// 	Description : set control both rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetBothRect (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,  //in:
    const GUI_BOTH_RECT_T    *both_rect_ptr  //in:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetBothRect (base_ctrl_ptr, both_rect_ptr);
}

/*****************************************************************************/
// 	Description : get gui control both rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetBothRect (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    GUI_BOTH_RECT_T  *both_rect_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetBothRect (base_ctrl_ptr, both_rect_ptr);
}

/*****************************************************************************/
// 	Description : set gui control rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetRect (
    CTRLBASE_OBJ_T       *base_ctrl_ptr,
    const GUI_RECT_T     *rect_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetRect (base_ctrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : get gui control rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetRect (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    GUI_RECT_T       *rect_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetRect (base_ctrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set control display rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetDisplayRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,  //in:
    const GUI_RECT_T  *rect_ptr,       //in:
    BOOLEAN           is_update        //in:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetDisplayRect (base_ctrl_ptr, rect_ptr, is_update);
}

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetDisplayRect (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,  //in:
    GUI_RECT_T        *rect_ptr        //in:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetDisplayRect (base_ctrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set gui control lcd device
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetLcdDevInfo (
    CTRLBASE_OBJ_T            *base_ctrl_ptr,
    const GUI_LCD_DEV_INFO    *lcd_dev_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetLcdDevInfo (base_ctrl_ptr, lcd_dev_ptr);
}

/*****************************************************************************/
// 	Description : get gui control lcd device
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetLcdDevInfo (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_LCD_DEV_INFO   *lcd_dev_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetLcdDevInfo (base_ctrl_ptr, lcd_dev_ptr);
}

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetBg (
    CTRLBASE_OBJ_T *base_ctrl_ptr,
    GUI_BG_T       *bg_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetBg (base_ctrl_ptr, bg_ptr);
}

/*****************************************************************************/
// 	Description : set control boder info
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetBorder (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_BORDER_T       *border_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetBorder (base_ctrl_ptr, border_ptr);
}

/*****************************************************************************/
// 	Description : set font
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetFont (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_FONT_ALL_T     *font_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetFont (base_ctrl_ptr, font_ptr);
}

/*****************************************************************************/
// 	Description : get font
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetFont (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_FONT_ALL_T     *font_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetFont (base_ctrl_ptr, font_ptr);
}

/*****************************************************************************/
// 	Description : set control padding
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetPadding (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    GUI_PADDING_T      *padding_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetPadding (base_ctrl_ptr, padding_ptr);
}

/*****************************************************************************/
// 	Description : set slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_slide
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetSlideState (base_ctrl_ptr, is_slide);
}

/*****************************************************************************/
// 	Description : get slide state
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetSlideState (
    CTRLBASE_OBJ_T     *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetSlideState (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set whether ctrl can active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_active
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetCanActive (base_ctrl_ptr, can_active);
}

/*****************************************************************************/
// 	Description : get whether ctrl can active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetCanActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetCanActive (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             can_kbd_active
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetCanKbdActive (base_ctrl_ptr, can_kbd_active);
}

/*****************************************************************************/
// 	Description : get whether ctrl can active under kbd mode
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetCanKbdActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetCanKbdActive (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetActive (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_active
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetActive (base_ctrl_ptr, is_active);
}

/*****************************************************************************/
// 	Description : get active
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetActive (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetActive (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetEnabled (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_enabled
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetEnabled (base_ctrl_ptr, is_enabled);
}

/*****************************************************************************/
// 	Description : Get enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetEnabled (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set gui control visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetVisible (
    CTRLBASE_OBJ_T     *base_ctrl_ptr,
    BOOLEAN            is_visible
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetVisible (base_ctrl_ptr, is_visible);
}

/*****************************************************************************/
// 	Description : Get visible
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetVisible (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetVisible (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_kbd_enabled
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetKbdEnabled (base_ctrl_ptr, is_kbd_enabled);
}

/*****************************************************************************/
// 	Description : Get kbd enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetKbdEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetKbdEnabled (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    BOOLEAN             is_tp_enabled
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetTpEnabled (base_ctrl_ptr, is_tp_enabled);
}

/*****************************************************************************/
// 	Description : Get tp enabled
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetTpEnabled (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetTpEnabled (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,
    CTRL_TP_PRIORITY_E  tp_priority
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetTpPriority (base_ctrl_ptr, tp_priority);
}

/*****************************************************************************/
// 	Description : get tp priority
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline CTRL_TP_PRIORITY_E VTLBASE_GetTpPriority (
    CTRLBASE_OBJ_T      *base_ctrl_ptr
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetTpPriority (base_ctrl_ptr);
}

/*****************************************************************************/
// 	Description : set control is display scrollbar
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetScrollbarEnabled (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    BOOLEAN          is_enabled
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetScrollbarEnabled (base_ctrl_ptr, is_enabled);
}

/*****************************************************************************/
// 	Description : set control is circular handle up/down key
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetUpDownLoop (
    CTRLBASE_OBJ_T   *base_ctrl_ptr,
    BOOLEAN          is_loop
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetUpDownLoop (base_ctrl_ptr, is_loop);
}

/*****************************************************************************/
// 	Description : set control is circular handle left/right key
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetLeftRightLoop (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,
    BOOLEAN           is_loop
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetLeftRightLoop (base_ctrl_ptr, is_loop);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetHeightByWidth (
    CTRLBASE_OBJ_T  *base_ctrl_ptr,  //in:
    uint16          width,          //in:
    uint16          *height_ptr     //in/out:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetHeightByWidth (base_ctrl_ptr, width, height_ptr);
}

/*****************************************************************************/
// 	Description : set control select rect
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetSelectRect (
    CTRLBASE_OBJ_T *base_ctrl_ptr,  //in:
    GUI_RECT_T     *rect_ptr       //in/out:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetSelectRect (base_ctrl_ptr, rect_ptr);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_GetSelectIndex (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,  //in:
    BOOLEAN           *is_first_ptr,  //in/out:
    BOOLEAN           *is_last_ptr    //in/out:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->GetSelectIndex (base_ctrl_ptr, is_first_ptr, is_last_ptr);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline BOOLEAN VTLBASE_SetSelectIndex (
    CTRLBASE_OBJ_T    *base_ctrl_ptr,   //in:
    BOOLEAN           is_first,         //in:
    BOOLEAN           is_last           //in:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);
    return base_ctrl_vtbl->SetSelectIndex (base_ctrl_ptr, is_first, is_last);
}

/*****************************************************************************/
// 	Description : set control select index,is the first or last line
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
static __inline void VTLBASE_OnStateChanged (
    CTRLBASE_OBJ_T      *base_ctrl_ptr,     //in:
    GUICTRL_STATE_T     state,              //in:
    BOOLEAN             is_true             //in:
)
{
    CTRLBASE_VTBL_T* base_ctrl_vtbl = (CTRLBASE_VTBL_T*) TYPEMNG_GetVtbl (CTRL_BASE_TYPE, (OBJECT_TYPE_PTR) base_ctrl_ptr);

    if (PNULL != base_ctrl_vtbl->OnStateChanged)
    {
        base_ctrl_vtbl->OnStateChanged (base_ctrl_ptr, state, is_true);
    }
}

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/

#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBASE_H_
