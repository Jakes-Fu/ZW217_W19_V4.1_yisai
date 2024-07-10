/*****************************************************************************
** File Name:      ctrlform_internal.c                                       *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ctrlform.h"
#include "mmi_theme.h"
#include "mmk_handle_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                         TYPE AND CONSTANT                                 *
 *---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                               *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/
#ifdef CTRLFORM_DRUMP_LAYOUT_INFO
BOOLEAN g_trace_layout_info = FALSE;
#endif

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN FormTypeOf(
                         CTRLBASE_OBJ_T     *form_ptr
                         );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : dock child is valid
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDockValidChild(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                );
#endif

/*****************************************************************************/
//  Description : find the child pointer by handle in child list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetChildPtrByHandle(
                                  MMI_HANDLE_T      ctrl_handle,        //in:
                                  CTRLFORM_CHILD_T  *first_child_ptr,   //in:
                                  CTRLFORM_CHILD_T  **child_pptr        //in/out:
                                  );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get form pointer by control handle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC CTRLFORM_OBJ_T* CTRLFORM_GetPtr(
                                       MMI_HANDLE_T  ctrl_handle
                                       )
{
    CTRLBASE_OBJ_T *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT(FormTypeOf(ctrl_ptr)); /*assert verified*/
    }
    
    return (CTRLFORM_OBJ_T*)ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence : 
//  Author:Jassmine
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN FormTypeOf(
                         CTRLBASE_OBJ_T     *form_ptr
                         )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)form_ptr,CTRL_FORM_TYPE);
}

/*****************************************************************************/
//  Description : init form display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_InitDisplayRect(
                                     CTRLFORM_OBJ_T *form_ctrl_ptr
                                     )
{
#ifdef GUIF_FORM_DOCKABLE
    uint16          child_height = 0;
    uint16          child_width = 0;
#endif
    GUI_RECT_T      content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
    
    //set relative display rect
    form_ctrl_ptr->relative_disp_rect.left   = 0;
    form_ctrl_ptr->relative_disp_rect.top    = 0;
    form_ctrl_ptr->relative_disp_rect.right  = content_rect.right - content_rect.left;
    form_ctrl_ptr->relative_disp_rect.bottom = content_rect.bottom - content_rect.top;

    //except softkey
    if (CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr))
    {
        form_ctrl_ptr->relative_disp_rect.bottom = form_ctrl_ptr->relative_disp_rect.bottom - form_ctrl_ptr->softkey_child_ptr->height;
    }

#ifdef GUIF_FORM_DOCKABLE
    //except top child
    child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->top_child_ptr);
    form_ctrl_ptr->relative_disp_rect.top = form_ctrl_ptr->relative_disp_rect.top + child_height;

    //except bottom child
    child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->bottom_child_ptr);
    form_ctrl_ptr->relative_disp_rect.bottom = form_ctrl_ptr->relative_disp_rect.bottom - child_height;

    //except left child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->left_child_ptr);
    form_ctrl_ptr->relative_disp_rect.left = form_ctrl_ptr->relative_disp_rect.left + child_width;

    //except right child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->right_child_ptr);
    form_ctrl_ptr->relative_disp_rect.right = form_ctrl_ptr->relative_disp_rect.right - child_width;
#endif
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : get top/bottom dock child height,include space etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDockChildHeight(
                                          CTRLFORM_CHILD_T  *child_ptr      //in:
                                          )
{
    uint16  child_height = 0;

    if ((CTRLFORM_IsValidChild(child_ptr)) &&
        (0 < child_ptr->height) && 
        ((GUIFORM_CHILD_DOCK_TOP == child_ptr->dock_type) || (GUIFORM_CHILD_DOCK_BOTTOM == child_ptr->dock_type)))
    {
        //set child heigh
        child_height = child_ptr->height;

        //adjust child height
        CTRLFORM_AdjustHeightByRange(&child_height,&child_ptr->size_range);
        
        //add space
        child_height = (uint16)(child_height + child_ptr->ver_space);
    }

    return (child_height);
}

/*****************************************************************************/
//  Description : get left/right dock child width,include space etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDockChildWidth(
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         )
{
    uint16   child_width = 0;

    if ((CTRLFORM_IsValidChild(child_ptr)) &&
        (0 < child_ptr->width) && 
        ((GUIFORM_CHILD_DOCK_LEFT == child_ptr->dock_type) || (GUIFORM_CHILD_DOCK_RIGHT == child_ptr->dock_type)))
    {
        child_width = (uint16)(child_ptr->width + child_ptr->hor_space);
    }

    return (child_width);
}
#endif

/*****************************************************************************/
//  Description : get form rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetRect(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr
                                   )
{
    GUI_RECT_T      form_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //set form rect
    form_rect = base_ctrl_ptr->rect;

    //except exist border
    if (GUI_BORDER_NONE != base_ctrl_ptr->border.type)
    {
        form_rect.left   = (int16)(form_rect.left + base_ctrl_ptr->border.width);
        form_rect.right  = (int16)(form_rect.right - base_ctrl_ptr->border.width);
        form_rect.top    = (int16)(form_rect.top + base_ctrl_ptr->border.width);
        form_rect.bottom = (int16)(form_rect.bottom - base_ctrl_ptr->border.width);
    }
    
    return (form_rect);
}

/*****************************************************************************/
//  Description : get content rect,except border and padding
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetContentRect(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr
                                          )
{
    GUI_RECT_T      content_rect = {0};
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //get form rect
    content_rect = CTRLFORM_GetRect(form_ctrl_ptr);

    //except padding
    content_rect.left   = (int16)(content_rect.left + base_ctrl_ptr->padding.left);
    content_rect.top    = (int16)(content_rect.top + base_ctrl_ptr->padding.top);
    content_rect.right  = (int16)(content_rect.right - base_ctrl_ptr->padding.right);
    content_rect.bottom = (int16)(content_rect.bottom - base_ctrl_ptr->padding.bottom);
    
    return (content_rect);
}

/*****************************************************************************/
//  Description : get form relative width,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLFORM_GetRelativeWidth(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        )
{
    uint32      relative_width = 0;
    
    //get relative width
    if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
    {
        relative_width = form_ctrl_ptr->relative_rect.right - form_ctrl_ptr->relative_rect.left + 1;
    }
    else
    {
        relative_width = form_ctrl_ptr->sbs_width;
    }
    
    return (relative_width);
}

/*****************************************************************************/
//  Description : get form relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLFORM_GetRelativeHeight(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         )
{
    uint32      relative_height = 0;
    
    //get relative height
    relative_height = form_ctrl_ptr->relative_rect.bottom - form_ctrl_ptr->relative_rect.top + 1;
    
    return (relative_height);
}

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T CTRLFORM_GetScrollBarRect(
                                            CTRLFORM_OBJ_T   *form_ctrl_ptr
                                            )
{
#ifdef GUIF_FORM_DOCKABLE
    uint16          child_height = 0;
    uint16          child_width = 0;
#endif
    GUI_RECT_T      prg_rect = {0};
    GUI_RECT_T      content_rect = {0};
    
    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
    
    //set prg rect
    prg_rect = content_rect;
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.right = (int16)(prg_rect.left + CTRLFORM_GetScrollBarWidth(form_ctrl_ptr) - 1);
    }
    else
    {
        prg_rect.left = (int16)(prg_rect.right - CTRLFORM_GetScrollBarWidth(form_ctrl_ptr) + 1);
    }
    
    //except softkey
    if (CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr))
    {
        prg_rect.bottom = (int16)(prg_rect.bottom - form_ctrl_ptr->softkey_child_ptr->height);
    }

#ifdef GUIF_FORM_DOCKABLE
    //except top child
    child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->top_child_ptr);
    prg_rect.top = (int16)(prg_rect.top + child_height);

    //except bottom child
    child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->bottom_child_ptr);
    prg_rect.bottom = (int16)(prg_rect.bottom - child_height);

    //except left child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->left_child_ptr);
    if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
    {
        prg_rect.left  = (int16)(prg_rect.left + child_width);
        prg_rect.right = (int16)(prg_rect.right + child_width);
    }

    //except right child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->right_child_ptr);
    if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
    {
        prg_rect.left  = (int16)(prg_rect.left - child_width);
        prg_rect.right = (int16)(prg_rect.right - child_width);
    }
#endif

    return (prg_rect);
}

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetScrollBarWidth(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         )
{
    uint16      bar_width = 0;
    
    if (form_ctrl_ptr->theme.is_support_slide)
    {
        bar_width = MMITHEME_GetSlideScrollBarWidth();
    }
    else
    {
        bar_width = MMITHEME_GetScrollBarWidth();
    }
    
    return (bar_width);
}

/*****************************************************************************/
//  Description : get child top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildTop(
                                  CTRLFORM_CHILD_T  *child_ptr      //in:
                                  )
{
    int32      child_top = 0;
    
    child_top = child_ptr->relative_rect.top;
    
    return (child_top);
}

/*****************************************************************************/
//  Description : get child bottom
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildBottom(
                                     CTRLFORM_CHILD_T   *child_ptr      //in:
                                     )
{
    int32      child_bottom = 0;
    
    child_bottom = child_ptr->relative_rect.bottom;

    return (child_bottom);
}

/*****************************************************************************/
//  Description : get child left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildLeft(
                                   CTRLFORM_CHILD_T *child_ptr      //in:
                                   )
{
    int32      child_left = 0;
    
    child_left = child_ptr->relative_rect.left;
    
    return (child_left);
}

/*****************************************************************************/
//  Description : get child right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetChildRight(
                                    CTRLFORM_CHILD_T    *child_ptr      //in:
                                    )
{
    int32      child_right = 0;
    
    child_right = child_ptr->relative_rect.right;
    
    return (child_right);
}

/*****************************************************************************/
//  Description : get child width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetChildWidth(
                                     CTRLFORM_CHILD_T   *child_ptr      //in:
                                     )
{
    uint16  child_width = 0;
    
    //get child control width
    child_width = (uint16)(child_ptr->relative_rect.right - child_ptr->relative_rect.left + 1);
    
    return (child_width);
}

/*****************************************************************************/
//  Description : get child height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetChildHeight(
                                      CTRLFORM_CHILD_T  *child_ptr      //in:
                                      )
{
    uint16  child_height = 0;
    
    //get child control height
    child_height = (uint16)(child_ptr->relative_rect.bottom - child_ptr->relative_rect.top + 1);

    //adjust child height
    CTRLFORM_AdjustHeightByRange(&child_height,&child_ptr->size_range);
    
    return (child_height);
}

/*****************************************************************************/
//  Description : adjust height by size range
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_AdjustHeightByRange(
                                         uint16                 *height_ptr,//in/out:
                                         CTRLFORM_SIZE_RANGE_T  *range_ptr  //in:
                                         )
{
    if ((PNULL != height_ptr) && 
        (PNULL != range_ptr))
    {
        //adjust height
        if (*height_ptr < range_ptr->min_size)
        {
            *height_ptr = range_ptr->min_size;
        }
        else if (*height_ptr > range_ptr->max_size)
        {
            *height_ptr = range_ptr->max_size;
        }
    }
}

/*****************************************************************************/
//  Description : get form display rect min top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMinTop(
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr
                                       )
{
    int32       min_top = 0;

    min_top = form_ctrl_ptr->relative_rect.top;

    return (min_top);
}

/*****************************************************************************/
//  Description : get form display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxTop(
                                       CTRLFORM_OBJ_T    *form_ctrl_ptr
                                       )
{
    uint16      display_height = 0;
    int32       max_top = 0;
    uint32      form_height = 0;
    
    //get display height
    display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
    
    //get form relative height
    form_height = CTRLFORM_GetRelativeHeight(form_ctrl_ptr);
    
    //get max top
    max_top = form_ctrl_ptr->relative_rect.top;
    if (form_height > display_height)
    {
        max_top = max_top + form_height - display_height;   /*lint !e737*/
    }
    
    return (max_top);
}

/*****************************************************************************/
//  Description : get form display rect min left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMinLeft(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        )
{
    int32       min_left = 0;

    min_left = form_ctrl_ptr->relative_rect.left;

    return (min_left);
}

/*****************************************************************************/
//  Description : get form display rect max left
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxLeft(
                                        CTRLFORM_OBJ_T  *form_ctrl_ptr
                                        )
{
    uint16      display_width = 0;
    int32       max_left = 0;
    uint32      form_width = 0;
    
    //get display width
    display_width = CTRLFORM_GetDisplayWidth(TRUE,form_ctrl_ptr);
    
    //get form relative width
    form_width = CTRLFORM_GetRelativeWidth(form_ctrl_ptr);
    
    //get max left
    max_left = form_ctrl_ptr->relative_rect.left;
    if (form_width > display_width)
    {
        max_left = max_left + form_width - display_width;   /*lint !e737*/
    }
    
    return (max_left);
}

/*****************************************************************************/
//  Description : get form display rect max right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int32 CTRLFORM_GetDisplayMaxRight(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                         CTRLFORM_CHILD_T   *child_ptr      //in:
                                         )
{
    uint16      prg_width = 0;
#ifdef GUIF_FORM_DOCKABLE
    uint16      child_width = 0;
#endif
    int32       max_right = 0;
    GUI_RECT_T  content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
    
    //get max right
    max_right = content_rect.right - content_rect.left;

    //dock child not except progress width
    if (!CTRLFORM_IsDockChild(child_ptr))
    {
        if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
        {
            //get progress width
            prg_width = CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
        }

        if ((MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection()) && 
            (0 < prg_width))
        {
            max_right = max_right - prg_width;
        }
    }

#ifdef GUIF_FORM_DOCKABLE
    //except left child
    if ((0 == child_ptr->dock_priority) ||
        ((PNULL != form_ctrl_ptr->left_child_ptr) && (child_ptr->dock_priority > form_ctrl_ptr->left_child_ptr->dock_priority)))
    {
        child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->left_child_ptr);
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            max_right = max_right - child_width;
        }
    }

    //except right child
    if ((0 == child_ptr->dock_priority) ||
        ((PNULL != form_ctrl_ptr->right_child_ptr) && (child_ptr->dock_priority > form_ctrl_ptr->right_child_ptr->dock_priority)))
    {
        child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->right_child_ptr);
        if (MMI_DIRECTION_LEFT == MMITHEME_GetDisDirection())
        {
            max_right = max_right - child_width;
        }
    }
#endif

    return (max_right);
}

/*****************************************************************************/
//  Description : get form display rect width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDisplayWidth(
                                       BOOLEAN          is_except_prg,
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr
                                       )
{
    uint16      display_width = 0;
    uint16      prg_width = 0;
#ifdef GUIF_FORM_DOCKABLE
    uint16      child_width = 0;
#endif
    GUI_RECT_T  content_rect = {0};
    
    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
    
    if ((is_except_prg) && 
        (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr))
    {
        //get progress width
        prg_width = CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
    }

    //get display width
    display_width = (uint16)(content_rect.right - content_rect.left + 1 - prg_width);

#ifdef GUIF_FORM_DOCKABLE
    //except left child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->left_child_ptr);
    if ((0 < child_width) &&
        (display_width >= child_width))
    {
        display_width = (uint16)(display_width - child_width);
    }

    //except right child
    child_width = CTRLFORM_GetDockChildWidth(form_ctrl_ptr->right_child_ptr);
    if ((0 < child_width) && 
        (display_width >= child_width))
    {
        display_width = (uint16)(display_width - child_width);
    }
#endif
    
    return (display_width);
}

/*****************************************************************************/
//  Description : get form display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 CTRLFORM_GetDisplayHeight(
                                        CTRLFORM_OBJ_T   *form_ctrl_ptr
                                        )
{
    uint16      display_height = 0;
#ifdef GUIF_FORM_DOCKABLE
    uint16      child_height = 0;
#endif
    GUI_RECT_T  content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);
    if (!GUI_IsRectEmpty(content_rect))
    {
        //get display height
        display_height = (uint16)(content_rect.bottom - content_rect.top + 1);

        //except softkey
        if ((CTRLFORM_IsValidChild(form_ctrl_ptr->softkey_child_ptr)) &&
            (display_height >= form_ctrl_ptr->softkey_child_ptr->height))
        {
            display_height = (uint16)(display_height - form_ctrl_ptr->softkey_child_ptr->height);
        }

#ifdef GUIF_FORM_DOCKABLE
        //except top child
        child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->top_child_ptr);
        if ((0 < child_height) && 
            (display_height >= child_height))
        {
            display_height = (uint16)(display_height - child_height);
        }

        //except bottom child
        child_height = CTRLFORM_GetDockChildHeight(form_ctrl_ptr->bottom_child_ptr);
        if ((0 < child_height) && 
            (display_height >= child_height))
        {
            display_height = (uint16)(display_height - child_height);
        }
#endif

        //adjust height
        CTRLFORM_AdjustHeightByRange(&display_height,&form_ctrl_ptr->size_range);
    }

    return (display_height);
}

/*****************************************************************************/
//  Description : get form display rect by display top
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T CTRLFORM_GetDisplayRect(
                                              int32          display_top,    //in:
                                              CTRLFORM_OBJ_T *form_ctrl_ptr  //in:
                                              )
{
    uint16          display_height = 0;
    GUI_BIG_RECT_T  relative_disp_rect = form_ctrl_ptr->relative_disp_rect;
    
    //adjust display top
    if (display_top != form_ctrl_ptr->relative_disp_rect.top)
    {
        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
        
        //set display top and bottom
        relative_disp_rect.top    = display_top;
        relative_disp_rect.bottom = display_top + display_height - 1;
    }
    
    return (relative_disp_rect);
}

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
                                       )
{
    BOOLEAN     result = FALSE;
    uint16      display_width = 0;
    uint16      display_height = 0;
    
    //adjust display top and left
    if ((display_top != form_ctrl_ptr->relative_disp_rect.top) || 
        (display_left != form_ctrl_ptr->relative_disp_rect.left))
    {
        //get display width and height
        display_width  = CTRLFORM_GetDisplayWidth(TRUE,form_ctrl_ptr);
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
        
        //set display top and bottom
        form_ctrl_ptr->relative_disp_rect.top    = display_top;
        form_ctrl_ptr->relative_disp_rect.bottom = display_top + display_height - 1;
        
        //set display left and right
        if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection())
        {
            uint16      prg_width = 0;
            if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
            {
                //get progress width
                prg_width = CTRLFORM_GetScrollBarWidth(form_ctrl_ptr);
            }
            form_ctrl_ptr->relative_disp_rect.left  = display_left;
            form_ctrl_ptr->relative_disp_rect.right = display_left + display_width + prg_width;
        }
        else
        {
            form_ctrl_ptr->relative_disp_rect.left  = display_left;
            form_ctrl_ptr->relative_disp_rect.right = display_left + display_width - 1;
        }

        CTRLFORM_TRACE_LAYOUT(
            "^^^ CTRLFORM_SetDisplayRect: form_ctrl_ptr=0x%08x, "
            "relative_disp_rect={%d,%d,%d,%d}",
            form_ctrl_ptr,
            form_ctrl_ptr->relative_disp_rect.left,
            form_ctrl_ptr->relative_disp_rect.top,
            form_ctrl_ptr->relative_disp_rect.right,
            form_ctrl_ptr->relative_disp_rect.bottom);
        
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get first and last valid child index,judge valid child is exist
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_GetValidChild(
                                   CTRLFORM_OBJ_T    *form_ctrl_ptr  //in/out:
                                   )
{
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (0 < form_ctrl_ptr->all_child_num)
    {
        //reset
        form_ctrl_ptr->is_valid_child    = FALSE;
        form_ctrl_ptr->first_valid_index = 0;
        form_ctrl_ptr->last_valid_index  = 0;

#ifdef GUIF_FORM_DOCKABLE
        //dock child is valid
        form_ctrl_ptr->is_valid_child = GetDockValidChild(form_ctrl_ptr);
#endif

        if (0 < form_ctrl_ptr->child_num)
        {
            //the first child
            child_ptr = form_ctrl_ptr->first_child_ptr;
            while (PNULL != child_ptr)
            {
                if (CTRLFORM_IsValidChild(child_ptr))
                {
                    //set valid child index
                    form_ctrl_ptr->is_valid_child    = TRUE;
                    form_ctrl_ptr->first_valid_index = child_ptr->index;
                    form_ctrl_ptr->last_valid_index  = child_ptr->index;
                    break;
                }

                //next child
                child_ptr = child_ptr->next_ptr;
            }

            //the last child
            if (PNULL != form_ctrl_ptr->first_child_ptr)
            {
                child_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
                while ((PNULL != child_ptr) && (child_ptr != form_ctrl_ptr->first_child_ptr))
                {
                    if (CTRLFORM_IsValidChild(child_ptr))
                    {
                        //modify last valid child index
                        form_ctrl_ptr->last_valid_index = child_ptr->index;
                        break;
                    }

                    //previous child
                    child_ptr = child_ptr->prev_ptr;
                }
            }
        }
    }
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : dock child is valid
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDockValidChild(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                )
{
    BOOLEAN             result = FALSE;
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        if (CTRLFORM_IsValidChild(child_ptr))
        {
            result = TRUE;
            break;
        }

        //get next dock child
        child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    }

    return (result);
}
#endif

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
                                           )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if ((PNULL != child_pptr) &&
        (index < form_ctrl_ptr->child_num))
    {
        //get the start layout child pointer
        child_ptr = form_ctrl_ptr->first_child_ptr;

        while (PNULL != child_ptr)
        {
            if (child_ptr->index == index)
            {
                result = TRUE;
                break;
            }

            //next child
            child_ptr = child_ptr->next_ptr;
        }

        *child_pptr = child_ptr;
    }

    return (result);
}

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
                                            )
{
    BOOLEAN             result = FALSE;
#ifdef GUIF_FORM_DOCKABLE
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *dock_child_ptr = PNULL;
#endif
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //find softkey
    result = GetChildPtrByHandle(ctrl_handle,
                form_ctrl_ptr->softkey_child_ptr,
                &child_ptr);

#ifdef GUIF_FORM_DOCKABLE
    if (!result)
    {
        //find dock child
        cur_priority = 1;
        dock_child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
        while (PNULL != dock_child_ptr)
        {
            if (GetChildPtrByHandle(ctrl_handle,
                    dock_child_ptr,
                    &child_ptr))
            {
                result = TRUE;
                break;
            }

            //get next dock child
            dock_child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
        }
    }
#endif

    //find other
    if (!result)
    {
        result = GetChildPtrByHandle(ctrl_handle,
                    form_ctrl_ptr->first_child_ptr,
                    &child_ptr);
    }

    *child_pptr = child_ptr;

    return (result);
}

/*****************************************************************************/
//  Description : find the child pointer by handle in child list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetChildPtrByHandle(
                                  MMI_HANDLE_T      ctrl_handle,        //in:
                                  CTRLFORM_CHILD_T  *first_child_ptr,   //in:
                                  CTRLFORM_CHILD_T  **child_pptr        //in/out:
                                  )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *child_ptr = first_child_ptr;

    while (PNULL != child_ptr)
    {
        if ((MMK_GetCtrlId(child_ptr->ctrl_handle ) == ctrl_handle) ||
            (child_ptr->ctrl_handle == ctrl_handle))
        {
            *child_pptr = child_ptr;
            result = TRUE;
            break;
        }

        //next child
        child_ptr = child_ptr->next_ptr;
    }

    return (result);
}

/*****************************************************************************/
//  Description : child is valid
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsValidChild(
                                     CTRLFORM_CHILD_T     *child_ptr
                                     )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != child_ptr) &&
        (MMK_IsHandleValid(child_ptr->ctrl_handle)) &&
        (!child_ptr->is_bg) && 
        (GUIFORM_CHILD_DISP_HIDE != child_ptr->display_type))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : form is get active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGetAtv(
                                 CTRLFORM_OBJ_T  *form_ctrl_ptr
                                 )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T     *child_ptr = PNULL;
    
    if ((PNULL != form_ctrl_ptr) && 
        (form_ctrl_ptr->is_valid_child))
    {
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            if (CTRLFORM_IsGetAtvChild(child_ptr))
            {
                result = TRUE;
                break;
            }
            
            //next child
            child_ptr = child_ptr->next_ptr;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : form is gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGray(
                               CTRLFORM_OBJ_T    *form_ctrl_ptr
                               )
{
    BOOLEAN             result = TRUE;
    CTRLFORM_CHILD_T     *child_ptr = PNULL;
    
    if ((PNULL != form_ctrl_ptr) && 
        (form_ctrl_ptr->is_valid_child))
    {
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            if (GUIFORM_CHILD_DISP_GRAY != child_ptr->display_type)
            {
                result = FALSE;
                break;
            }
            
            //next child
            child_ptr = child_ptr->next_ptr;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : child is get active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsGetAtvChild(
                                      CTRLFORM_CHILD_T    *child_ptr
                                      )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != child_ptr && 
        (!child_ptr->is_bg) &&
        (child_ptr->is_get_active) && 
        (GUIFORM_CHILD_DISP_NORMAL == child_ptr->display_type))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : child is form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsFormChild(
                                    CTRLFORM_CHILD_T  *child_ptr
                                    )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != child_ptr) && 
        (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is order form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsOrderForm(
                                    CTRLFORM_OBJ_T   *form_ctrl_ptr
                                    )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != form_ctrl_ptr) && 
        (GUIFORM_LAYOUT_ORDER == form_ctrl_ptr->layout_type))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is sbs form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSbsForm(
                                  CTRLFORM_OBJ_T    *form_ctrl_ptr
                                  )
{
    BOOLEAN     result = FALSE;

    if ((PNULL != form_ctrl_ptr) && 
        (GUIFORM_LAYOUT_SBS == form_ctrl_ptr->layout_type))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is unit form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitForm(
                                   CTRLFORM_OBJ_T    *form_ctrl_ptr
                                   )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != form_ctrl_ptr && GUIFORM_STYLE_UNIT == form_ctrl_ptr->style)
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is unit form and it only include one control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitFormSingle(
                                         CTRLFORM_OBJ_T  *form_ctrl_ptr
                                         )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != form_ctrl_ptr) && 
        (GUIFORM_STYLE_UNIT == form_ctrl_ptr->style) && 
        (1 == form_ctrl_ptr->all_child_num))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : is unit form child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsUnitFormChild(
                                        CTRLFORM_CHILD_T  *child_ptr
                                        )
{
    BOOLEAN             result = FALSE;
    GUIFORM_STYLE_E     style = GUIFORM_STYLE_NORMAL;
    
    if ((PNULL != child_ptr) &&
        (CTRLFORM_IsFormChild(child_ptr)))
    {
        //get form style
        style = CTRLFORM_GetStyle(IGUICTRL_GetCtrlHandle(child_ptr->child_ctrl_ptr));
        
        if (GUIFORM_STYLE_UNIT == style)
        {
            result = TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : form child is active
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAtvFormChild(
                                       CTRLFORM_CHILD_T   *child_ptr
                                       )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_OBJ_T      *form_ctrl_ptr = PNULL;
    
    if ((PNULL != child_ptr) &&
        (CTRLFORM_IsFormChild(child_ptr)) &&
        (CTRLFORM_IsGetAtvChild(child_ptr)))
    {
        form_ctrl_ptr = CTRLFORM_GetPtr(IGUICTRL_GetCtrlHandle(child_ptr->child_ctrl_ptr));
        if ((PNULL != form_ctrl_ptr) && 
            (PNULL != form_ctrl_ptr->active_child_ptr))
        {
            result = TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : child is anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsAnimChild(
                                    CTRLFORM_CHILD_T  *child_ptr
                                    )
{
    BOOLEAN     result = FALSE;
    
    if ((PNULL != child_ptr) && 
        (SPRD_GUI_ANIM_ID == IGUICTRL_GetCtrlGuid(child_ptr->child_ctrl_ptr)))
    {
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : child is dock top/bottom or left/right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsDockChild(
                                    CTRLFORM_CHILD_T    *child_ptr
                                    )
{
    BOOLEAN     result = FALSE;

    if (PNULL != child_ptr)
    {
        switch (child_ptr->dock_type)
        {
        case GUIFORM_CHILD_DOCK_TOP:
        case GUIFORM_CHILD_DOCK_BOTTOM:
        case GUIFORM_CHILD_DOCK_LEFT:
        case GUIFORM_CHILD_DOCK_RIGHT:
            result = TRUE;
            break;

        default:
            break;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : child is set min/max size
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsSetSize(
                                  CTRLFORM_CHILD_T  *child_ptr
                                  )
{
    BOOLEAN         result = FALSE;

    if ((0 != child_ptr->size_range.min_size) || 
        (0xFFFF != child_ptr->size_range.max_size))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : is display child from right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsDisplayFromRight(
                                           CTRLFORM_OBJ_T    *form_ctrl_ptr
                                           )
{
    BOOLEAN     result = FALSE;
    
    if ((form_ctrl_ptr->is_auto_dir) && 
        (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()))
    {
        result = TRUE;
    }
    
    return (result);
}

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
                                                  )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    //top child
    if ((PNULL != priority_ptr) &&
        (PNULL != form_ctrl_ptr->top_child_ptr) &&
        (*priority_ptr == form_ctrl_ptr->top_child_ptr->dock_priority))
    {
        //set child
        child_ptr = form_ctrl_ptr->top_child_ptr;

        (*priority_ptr)++;
        result = TRUE;
    }

    //bottom child
    if ((!result) &&
        (PNULL != priority_ptr) &&
        (PNULL != form_ctrl_ptr->bottom_child_ptr) &&
        (*priority_ptr == form_ctrl_ptr->bottom_child_ptr->dock_priority))
    {
        //set child
        child_ptr = form_ctrl_ptr->bottom_child_ptr;

        (*priority_ptr)++;
        result = TRUE;
    }

    //left child
    if ((!result) &&
        (PNULL != priority_ptr) &&
        (PNULL != form_ctrl_ptr->left_child_ptr) &&
        (*priority_ptr == form_ctrl_ptr->left_child_ptr->dock_priority))
    {
        //set child
        child_ptr = form_ctrl_ptr->left_child_ptr;

        (*priority_ptr)++;
        result = TRUE;
    }

    //right child
    if ((!result) &&
        (PNULL != priority_ptr) &&
        (PNULL != form_ctrl_ptr->right_child_ptr) &&
        (*priority_ptr == form_ctrl_ptr->right_child_ptr->dock_priority))
    {
        //set child
        child_ptr = form_ctrl_ptr->right_child_ptr;

        (*priority_ptr)++;
        result = TRUE;
    }

    return (child_ptr);
}
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
                                    )
{
    BOOLEAN         result = FALSE;
    CTRLFORM_OBJ_T  *child_form_ctrl_ptr = PNULL;
    
    if ((GUIFORM_TYPE_TP == form_ctrl_ptr->type) &&
        (PNULL != child_ptr) &&
        (CTRLFORM_IsUnitFormChild(child_ptr)))
    {
        //get form pointer
        child_form_ctrl_ptr = CTRLFORM_GetPtr(child_ptr->ctrl_handle);
        
        if (PNULL != child_form_ctrl_ptr)
        {
            if (is_tp_down != child_form_ctrl_ptr->is_tp_down)
            {
                //set is tp down
                child_form_ctrl_ptr->is_tp_down = is_tp_down;
                
                //set form param
                CTRLFORM_SetParam(child_form_ctrl_ptr);
            }
            
            if (is_update)
            {
                //update unit form and all child
                CTRLFORM_Display(FALSE,TRUE,child_form_ctrl_ptr);
                
                //set child rect
                CTRLFORM_SetChildRect(TRUE,child_form_ctrl_ptr);
            }
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : convert relative rect to screen rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_ClientToScreen(
                                    CTRLFORM_OBJ_T  *form_ctrl_ptr, //in:
                                    GUI_BIG_RECT_T  *rect_ptr       //in/out:may PNULL
                                    )
{
    int32           left_offset = 0;
    int32           top_offset = 0;
    GUI_RECT_T      content_rect = {0};

    if ((PNULL != form_ctrl_ptr) &&
        (PNULL != rect_ptr))
    {
        //get content rect
        content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

        //set left offset
        left_offset = content_rect.left + form_ctrl_ptr->relative_rect.left - form_ctrl_ptr->relative_disp_rect.left;

        //set top offset
        top_offset = content_rect.top + form_ctrl_ptr->relative_rect.top - form_ctrl_ptr->relative_disp_rect.top;

        //left
        rect_ptr->left = rect_ptr->left + left_offset;

        //right
        rect_ptr->right = rect_ptr->right + left_offset;

        //top
        rect_ptr->top = rect_ptr->top + top_offset;

        //bottom
        rect_ptr->bottom = rect_ptr->bottom + top_offset;
    }
}

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
                                    )
{
    int32           left_offset = 0;
    int32           top_offset = 0;
    GUI_RECT_T      content_rect = {0};

    //get content rect
    content_rect = CTRLFORM_GetContentRect(form_ctrl_ptr);

    //set left offset
    left_offset = form_ctrl_ptr->relative_disp_rect.left - form_ctrl_ptr->relative_rect.left - content_rect.left;

    //set top offset
    top_offset = form_ctrl_ptr->relative_disp_rect.top - form_ctrl_ptr->relative_rect.top - content_rect.top;

    //left
    if (PNULL != left_ptr)
    {
        *left_ptr = *left_ptr + left_offset;
    }

    //right
    if (PNULL != right_ptr)
    {
        *right_ptr = *right_ptr + left_offset;
    }

    //top
    if (PNULL != top_ptr)
    {
        *top_ptr = *top_ptr + top_offset;
    }

    //bottom
    if (PNULL != bottom_ptr)
    {
        *bottom_ptr = *bottom_ptr + top_offset;
    }
}


