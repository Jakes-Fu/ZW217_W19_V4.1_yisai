/*****************************************************************************
** File Name:      ctrlform_move.c                                           *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Create
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "ctrlform_export.h"
#include "ctrlform.h"

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

/*---------------------------------------------------------------------------*
 *                          LOCAL FUNCTION DECLARE                           *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get display top for handle key up message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyUp(
                                    int32               *disp_top_ptr,      //in/out:
                                    int32               *disp_left_ptr,     //in/out:
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                                    CTRLFORM_CHILD_T    **active_child_pptr //in/out:
                                    );

/*****************************************************************************/
//  Description : get previous or next valid/active child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidOrAtvChildPtr(
                                    BOOLEAN             is_valid,       //in:
                                    BOOLEAN             is_prev,        //in:
                                    BOOLEAN             is_cir_handle,  //in:
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                                    CTRLFORM_CHILD_T    *cur_child_ptr, //in:
                                    CTRLFORM_CHILD_T    **out_child_pptr//in/out:
                                    );

/*****************************************************************************/
//  Description : get previous child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetPrevChild(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                     CTRLFORM_CHILD_T   *cur_child_ptr, //in:
                                     BOOLEAN            is_cir_handle   //in:
                                     );

/*****************************************************************************/
//  Description : get next child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetNextChild(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                     CTRLFORM_CHILD_T   *cur_child_ptr, //in:
                                     BOOLEAN            is_cir_handle   //in:
                                     );

/*****************************************************************************/
//  Description : get display top for handle key down message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyDown(
                                      int32             *disp_top_ptr,      //in/out:
                                      int32             *disp_left_ptr,     //in/out:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,     //in:
                                      CTRLFORM_CHILD_T  **active_child_pptr //in/out:
                                      );

/*****************************************************************************/
//  Description : get display top for handle key left message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyLeft(
                                      int32             *disp_top_ptr,      //in/out:
                                      int32             *disp_left_ptr,     //in/out:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,     //in:
                                      CTRLFORM_CHILD_T  **active_child_pptr //in/out:
                                      );

/*****************************************************************************/
//  Description : get display top for handle key right message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyRight(
                                       int32            *disp_top_ptr,      //in/out:may PNULL
                                       int32            *disp_left_ptr,     //in/out:may PNULL
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr,     //in:
                                       CTRLFORM_CHILD_T **active_child_pptr //in/out:
                                       );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : adjust display top by select rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_AdjustDisplayTopBySelect(
                                                 int32              *display_top_ptr,   //in:
                                                 CTRLFORM_OBJ_T     *form_ctrl_ptr,     //in:
                                                 CTRLFORM_CHILD_T   *child_ptr          //in:
                                                 )
{
    BOOLEAN         result = FALSE;
    BOOLEAN         is_first_line = FALSE;
    BOOLEAN         is_last_line = FALSE;
    uint16          display_height = 0;
    int32           display_top = *display_top_ptr;
    int32           child_bottom = 0;
    int32           display_max_top = 0;
    GUI_RECT_T      select_rect = {0};
    GUI_BIG_RECT_T  relative_rect = {0};
    
    //get edit select rect
    if ((PNULL != child_ptr) && 
        (!CTRLFORM_IsDockChild(child_ptr)) &&
        (IGUICTRL_GetSelectRect(child_ptr->child_ctrl_ptr,&select_rect)))
    {
        //convert rect to relative rect
        relative_rect.top    = select_rect.top;
        relative_rect.bottom = select_rect.bottom;
        CTRLFORM_ScreenToClient(form_ctrl_ptr,
            PNULL,
            &relative_rect.top,
            PNULL,
            &relative_rect.bottom);
        
        //get select index
        IGUICTRL_GetSelectIndex(child_ptr->child_ctrl_ptr,&is_first_line,&is_last_line);
        
        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
        
        //edit cursor is not display all
        if (relative_rect.top < display_top)
        {
            //is the first line
            if (is_first_line)
            {
                //get display top by child top
                display_top = CTRLFORM_GetChildTop(child_ptr);
            }
            else
            {
                display_top = relative_rect.top;
            }
        }
        else if (relative_rect.bottom > (display_top+display_height-1))
        {
            //is the last line
            if (is_last_line)
            {
                //get child bottom
                child_bottom = CTRLFORM_GetChildBottom(child_ptr);
                
                //get display top by child bottom
                display_top = child_bottom - display_height + 1;
            }
            else
            {
                display_top = relative_rect.bottom - display_height + 1;
            }
        }
        
        //get display max top
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
        
        //adjust display top
        display_top = (int32)MIN(display_top,display_max_top);
        
        //set display top
        *display_top_ptr = display_top;
        
        result = TRUE;
    }
    
    return (result);
}

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
                                            )
{
    BOOLEAN         result = FALSE;
    
    switch (msg_id)
    {
    case MSG_APP_UP:
        result = GetDisplayTopForKeyUp(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
        break;
        
    case MSG_APP_DOWN:
        result = GetDisplayTopForKeyDown(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
        break;
        
    case MSG_APP_LEFT:
        if (CTRLFORM_IsSbsForm(form_ctrl_ptr))
        {
            //display child from right
            if (CTRLFORM_IsDisplayFromRight(form_ctrl_ptr))
            {
                result = GetDisplayTopForKeyRight(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
            }
            else
            {
                result = GetDisplayTopForKeyLeft(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
            }
        }
        break;
        
    case MSG_APP_RIGHT:
        if (CTRLFORM_IsSbsForm(form_ctrl_ptr))
        {
            //display child from right
            if (CTRLFORM_IsDisplayFromRight(form_ctrl_ptr))
            {
                result = GetDisplayTopForKeyLeft(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
            }
            else
            {
                result = GetDisplayTopForKeyRight(disp_top_ptr,disp_left_ptr,form_ctrl_ptr,active_child_pptr);
            }
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"CTRLFORM_GetDisplayTopForKey: order form don't handle msg %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_MOVE_255_112_2_18_3_18_33_164,(uint8*)"d",msg_id);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get display top for handle key up message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyUp(
                                    int32               *disp_top_ptr,      //in/out:
                                    int32               *disp_left_ptr,     //in/out:
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr,     //in:
                                    CTRLFORM_CHILD_T    **active_child_pptr //in/out:
                                    )
{
    BOOLEAN             result = FALSE;
    uint16              display_height = 0;
    uint16              child_height = 0;
    int32               display_top = 0;
    int32               display_left = 0;
    int32               child_top = 0;
    int32               display_max_top = 0;
    int32               scroll_step = 0;
    CTRLFORM_CHILD_T    *active_child_ptr = form_ctrl_ptr->active_child_ptr;
    CTRLFORM_CHILD_T    *prev_child_ptr = PNULL;
    
    //set init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    //get child top
    child_top = CTRLFORM_GetChildTop(form_ctrl_ptr->cur_child_ptr);
    
    //get display height
    display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
    
    //get display max top
    display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
    
    //current control top is not display all
    if ((!CTRLFORM_IsDockChild(form_ctrl_ptr->cur_child_ptr)) && 
        (display_top > child_top))
    {
        //get scroll step
        scroll_step = display_height * form_ctrl_ptr->theme.scroll_ratio.num1 / form_ctrl_ptr->theme.scroll_ratio.num2;
        
        //adjust scroll step
        if ((form_ctrl_ptr->cur_child_ptr->index > form_ctrl_ptr->first_valid_index) &&
            (display_top < (child_top + scroll_step)))
        {
            scroll_step = display_top - child_top;
        }
        
        //get display top by scroll step
        display_top = CTRLFORM_GetDisplayTopByStep(TRUE,
            scroll_step,
            display_max_top,
            form_ctrl_ptr);
        
        result = TRUE;
    }
    else if ((CTRLFORM_IsOrderForm(form_ctrl_ptr)) && 
             (GetValidOrAtvChildPtr(TRUE,TRUE,form_ctrl_ptr->theme.is_cir_handle_ud,form_ctrl_ptr,form_ctrl_ptr->cur_child_ptr,&prev_child_ptr)))
    {
        //set current child
        form_ctrl_ptr->cur_child_ptr = prev_child_ptr;

        //set current child select index is last
        if (form_ctrl_ptr->theme.is_cir_handle_ud)
        {
            result = IGUICTRL_SetSelectIndex(prev_child_ptr->child_ctrl_ptr,FALSE,TRUE);
        }
        
        //previous control is all display
        if ((CTRLFORM_IsDockChild(prev_child_ptr)) ||
            (CTRLFORM_IsRectCovered(form_ctrl_ptr->relative_disp_rect,prev_child_ptr->relative_rect)))
        {
            //previous child is get active
            if (CTRLFORM_IsGetAtvChild(prev_child_ptr))
            {
				//recursive call should have a definite termination condition to avoid stack overflow
                /*if (prev_child_ptr == active_child_ptr)
                {
                    //get display top
                    result = GetDisplayTopForKeyUp(&display_top,&display_left,form_ctrl_ptr,&active_child_ptr);
                }
                else*/
                {
                    //set active control
                    active_child_ptr = prev_child_ptr;
                    
                    result = TRUE;
                }
            }
            else
            {
                //get display top
                result = GetDisplayTopForKeyUp(&display_top,&display_left,form_ctrl_ptr,&active_child_ptr);
            }
        }
        else
        {
            //previous child is get active
            if (CTRLFORM_IsGetAtvChild(prev_child_ptr))
            {
                //set active control
                active_child_ptr = prev_child_ptr;
            }
            
            //get previous control height
            child_height = CTRLFORM_GetChildHeight(prev_child_ptr);
            
            //previous control height > display height
            if (child_height > display_height)
            {
                //set display top
                display_top = prev_child_ptr->relative_rect.bottom - display_height + 1;
            }
            else
            {
                //get display top by child top
                display_top = CTRLFORM_GetChildTop(prev_child_ptr);
            }
            
            //adjust display top
            display_top = (int32)MIN(display_top,display_max_top);
            
            result = TRUE;
        }
    }
    
    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }
    
    //set display left
    if (PNULL != disp_left_ptr)
    {
        *disp_left_ptr = display_left;
    }
    
    //set active child
    if (PNULL != active_child_pptr)
    {
        *active_child_pptr = active_child_ptr;
    }
    
    return (result);
}

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
                                          )
{
    int32   display_top = 0;
    int32   display_min_top = 0;
    
    if (is_up)
    {
        //get display min top
        display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);

        //set display top
        if (form_ctrl_ptr->relative_disp_rect.top - display_min_top > scroll_step)
        {
            display_top = form_ctrl_ptr->relative_disp_rect.top - display_min_top - scroll_step;
        }
        else
        {
            display_top = display_min_top;
        }
    }
    else
    {
        //set display top
        display_top = (int32)MIN((form_ctrl_ptr->relative_disp_rect.top + scroll_step),display_max_top);
    }
    
    return (display_top);
}

/*****************************************************************************/
//  Description : get previous or next valid/active child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetValidOrAtvChildPtr(
                                    BOOLEAN             is_valid,       //in:
                                    BOOLEAN             is_prev,        //in:
                                    BOOLEAN             is_cir_handle,  //in:
                                    CTRLFORM_OBJ_T      *form_ctrl_ptr, //in:
                                    CTRLFORM_CHILD_T    *cur_child_ptr, //in:
                                    CTRLFORM_CHILD_T    **out_child_pptr//in/out:
                                    )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *out_child_ptr = PNULL;

    if (is_prev)
    {
        //set previous valid child
        out_child_ptr = GetPrevChild(form_ctrl_ptr,cur_child_ptr,is_cir_handle);

        //get previous valid child
        while ((PNULL != out_child_ptr) &&
               (out_child_ptr != cur_child_ptr))
        {
            if (is_valid)
            {
                if (CTRLFORM_IsValidChild(out_child_ptr))
                {
                    result = TRUE;
                    break;
                }
            }
            else
            {
                if (CTRLFORM_IsGetAtvChild(out_child_ptr))
                {
                    result = TRUE;
                    break;
                }
            }
            
            //previous child
            out_child_ptr = GetPrevChild(form_ctrl_ptr,out_child_ptr,is_cir_handle);
        }
    }
    else
    {
        //get next valid child
        out_child_ptr = GetNextChild(form_ctrl_ptr,cur_child_ptr,is_cir_handle);

        //get next valid child
        while ((PNULL != out_child_ptr) &&
               (out_child_ptr != cur_child_ptr))
        {
            if (is_valid)
            {
                if (CTRLFORM_IsValidChild(out_child_ptr))
                {
                    result = TRUE;
                    break;
                }
            }
            else
            {
                if (CTRLFORM_IsGetAtvChild(out_child_ptr))
                {
                    result = TRUE;
                    break;
                }
            }

            //next child
            out_child_ptr = GetNextChild(form_ctrl_ptr,out_child_ptr,is_cir_handle);
        }
    }

    //set previous or next valid child
    if (PNULL != out_child_pptr)
    {
        *out_child_pptr = out_child_ptr;
    }

    return (result);
}

/*****************************************************************************/
//  Description : get previous child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetPrevChild(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                     CTRLFORM_CHILD_T   *cur_child_ptr, //in:
                                     BOOLEAN            is_cir_handle   //in:
                                     )
{
    CTRLFORM_CHILD_T    *out_child_ptr = PNULL;

#ifdef GUIF_FORM_DOCKABLE
    //top child
    if (cur_child_ptr == form_ctrl_ptr->top_child_ptr)
    {
        if (is_cir_handle)
        {
            //find bottom to middle
            if (PNULL != form_ctrl_ptr->bottom_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->bottom_child_ptr;
            }
            else if (PNULL != form_ctrl_ptr->first_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
            }
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->bottom_child_ptr)
    {
        //find middle to top
        if (PNULL != form_ctrl_ptr->first_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
        }
        else if (PNULL != form_ctrl_ptr->top_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->top_child_ptr;
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->left_child_ptr)
    {
        if (is_cir_handle)
        {
            //find right to middle
            if (PNULL != form_ctrl_ptr->right_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->right_child_ptr;
            }
            else if (PNULL != form_ctrl_ptr->first_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
            }
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->right_child_ptr)
    {
        //find middle to left
        if (PNULL != form_ctrl_ptr->first_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->first_child_ptr->prev_ptr;
        }
        else if (PNULL != form_ctrl_ptr->left_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->left_child_ptr;
        }
    }
    else
#endif
    {
        //set previous valid child
        if (cur_child_ptr->index > form_ctrl_ptr->first_valid_index)
        {
            out_child_ptr = cur_child_ptr->prev_ptr;
        }
        else
        {
            //find middle
            switch (form_ctrl_ptr->layout_type)
            {
            case GUIFORM_LAYOUT_ORDER:
#ifdef GUIF_FORM_DOCKABLE
                if (PNULL != form_ctrl_ptr->top_child_ptr)
                {
                    out_child_ptr = form_ctrl_ptr->top_child_ptr;
                }
                else
                {
                    if (is_cir_handle)
                    {
                        if (PNULL != form_ctrl_ptr->bottom_child_ptr)
                        {
                            out_child_ptr = form_ctrl_ptr->bottom_child_ptr;
                        }
                        else
                        {
                            out_child_ptr = cur_child_ptr->prev_ptr;
                        }
                    }
                }
#else
                if (is_cir_handle)
                {
                    out_child_ptr = cur_child_ptr->prev_ptr;
                }
#endif
                break;

            case GUIFORM_LAYOUT_SBS:
#ifdef GUIF_FORM_DOCKABLE
                if (PNULL != form_ctrl_ptr->left_child_ptr)
                {
                    out_child_ptr = form_ctrl_ptr->left_child_ptr;
                }
                else
                {
                    if (is_cir_handle)
                    {
                        if (PNULL != form_ctrl_ptr->right_child_ptr)
                        {
                            out_child_ptr = form_ctrl_ptr->right_child_ptr;
                        }
                        else
                        {
                            out_child_ptr = cur_child_ptr->prev_ptr;
                        }
                    }
                }
#else
                if (is_cir_handle)
                {
                    out_child_ptr = cur_child_ptr->prev_ptr;
                }
#endif
                break;

            default:
                break;
            }
        }
    }

    if ((PNULL == out_child_ptr) && 
        (is_cir_handle))
    {
        out_child_ptr = cur_child_ptr;
    }

    return (out_child_ptr);
}

/*****************************************************************************/
//  Description : get next child pointer by current child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetNextChild(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr, //in:
                                     CTRLFORM_CHILD_T   *cur_child_ptr, //in:
                                     BOOLEAN            is_cir_handle   //in:
                                     )
{
    CTRLFORM_CHILD_T    *out_child_ptr = PNULL;

#ifdef GUIF_FORM_DOCKABLE
    //top child
    if (cur_child_ptr == form_ctrl_ptr->top_child_ptr)
    {
        //find middle to bottom
        if (PNULL != form_ctrl_ptr->first_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->first_child_ptr;
        }
        else if (PNULL != form_ctrl_ptr->bottom_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->bottom_child_ptr;
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->bottom_child_ptr)
    {
        if (is_cir_handle)
        {
            //find top to middle
            if (PNULL != form_ctrl_ptr->top_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->top_child_ptr;
            }
            else if (PNULL != form_ctrl_ptr->first_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->first_child_ptr;
            }
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->left_child_ptr)
    {
        //find middle to right
        if (PNULL != form_ctrl_ptr->first_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->first_child_ptr;
        }
        else if (PNULL != form_ctrl_ptr->right_child_ptr)
        {
            out_child_ptr = form_ctrl_ptr->right_child_ptr;
        }
    }
    else if (cur_child_ptr == form_ctrl_ptr->right_child_ptr)
    {
        if (is_cir_handle)
        {
            //find left to middle
            if (PNULL != form_ctrl_ptr->left_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->left_child_ptr;
            }
            else if (PNULL != form_ctrl_ptr->first_child_ptr)
            {
                out_child_ptr = form_ctrl_ptr->first_child_ptr;
            }
        }
    }
    else
#endif
    {
        //set previous valid child
        if (cur_child_ptr->index < form_ctrl_ptr->last_valid_index)
        {
            out_child_ptr = cur_child_ptr->next_ptr;
        }
        else
        {
            //find middle
            switch (form_ctrl_ptr->layout_type)
            {
            case GUIFORM_LAYOUT_ORDER:
#ifdef GUIF_FORM_DOCKABLE
                if (PNULL != form_ctrl_ptr->bottom_child_ptr)
                {
                    out_child_ptr = form_ctrl_ptr->bottom_child_ptr;
                }
                else
                {
                    if (is_cir_handle)
                    {
                        if (PNULL != form_ctrl_ptr->top_child_ptr)
                        {
                            out_child_ptr = form_ctrl_ptr->top_child_ptr;
                        }
                        else
                        {
                            out_child_ptr = form_ctrl_ptr->first_child_ptr;
                        }
                    }
                }
#else
                if (is_cir_handle)
                {
                    out_child_ptr = form_ctrl_ptr->first_child_ptr;
                }
#endif
                break;

            case GUIFORM_LAYOUT_SBS:
#ifdef GUIF_FORM_DOCKABLE
                if (PNULL != form_ctrl_ptr->right_child_ptr)
                {
                    out_child_ptr = form_ctrl_ptr->right_child_ptr;
                }
                else
                {
                    if (is_cir_handle)
                    {
                        if (PNULL != form_ctrl_ptr->left_child_ptr)
                        {
                            out_child_ptr = form_ctrl_ptr->left_child_ptr;
                        }
                        else
                        {
                            out_child_ptr = form_ctrl_ptr->first_child_ptr;
                        }
                    }
                }
#else
                if (is_cir_handle)
                {
                    out_child_ptr = form_ctrl_ptr->first_child_ptr;
                }
#endif
                break;

            default:
                break;
            }
        }
    }

    if ((PNULL == out_child_ptr) && 
        (is_cir_handle))
    {
        out_child_ptr = cur_child_ptr;
    }

    return (out_child_ptr);
}

/*****************************************************************************/
//  Description : rect1 is covered rect2
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLFORM_IsRectCovered(
                                      GUI_BIG_RECT_T     rect1,
                                      GUI_BIG_RECT_T     rect2
                                      )
{
    BOOLEAN     result = TRUE;
    
    if ((rect2.top < rect1.top) || 
        (rect2.bottom > rect1.bottom) || 
        (rect2.left < rect1.left) || 
        (rect2.right > rect1.right))
    {
        result = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get display top for handle key down message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyDown(
                                      int32             *disp_top_ptr,      //in/out:
                                      int32             *disp_left_ptr,     //in/out:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,     //in:
                                      CTRLFORM_CHILD_T  **active_child_pptr //in/out:
                                      )
{
    BOOLEAN             result = FALSE;
    uint16              display_height = 0;
    uint16              child_height = 0;
    int32               display_top = 0;
    int32               display_left = 0;
    int32               child_bottom = 0;
    int32               display_max_top = 0;
    int32               scroll_step = 0;
    CTRLFORM_CHILD_T    *active_child_ptr = form_ctrl_ptr->active_child_ptr;
    CTRLFORM_CHILD_T    *next_child_ptr = PNULL;
    
    //set init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    //get child bottom
    child_bottom = CTRLFORM_GetChildBottom(form_ctrl_ptr->cur_child_ptr);
    
    //get display height
    display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
    
    //get display max top
    display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
    
    //current control bottom is not display all
    if (!(CTRLFORM_IsDockChild(form_ctrl_ptr->cur_child_ptr)) && 
        (form_ctrl_ptr->relative_disp_rect.bottom < child_bottom))
    {
        //get scroll step
        scroll_step = display_height * form_ctrl_ptr->theme.scroll_ratio.num1 / form_ctrl_ptr->theme.scroll_ratio.num2;
        
        //adjust scroll step
        if ((form_ctrl_ptr->cur_child_ptr->index < form_ctrl_ptr->last_valid_index) && 
            ((form_ctrl_ptr->relative_disp_rect.bottom + scroll_step) > child_bottom))
        {
            scroll_step = child_bottom - form_ctrl_ptr->relative_disp_rect.bottom;
        }
        
        //get display top by scroll step
        display_top = CTRLFORM_GetDisplayTopByStep(FALSE,
            scroll_step,
            display_max_top,
            form_ctrl_ptr);
        
        result = TRUE;
    }
    else if ((CTRLFORM_IsOrderForm(form_ctrl_ptr)) && 
             (GetValidOrAtvChildPtr(TRUE,FALSE,form_ctrl_ptr->theme.is_cir_handle_ud,form_ctrl_ptr,form_ctrl_ptr->cur_child_ptr,&next_child_ptr)))
    {
        //set current child
        form_ctrl_ptr->cur_child_ptr = next_child_ptr;

        //set current child select index is first
        if (form_ctrl_ptr->theme.is_cir_handle_ud)
        {
            result = IGUICTRL_SetSelectIndex(next_child_ptr->child_ctrl_ptr,TRUE,FALSE);
        }
        
        //next control is all display
        if ((CTRLFORM_IsDockChild(next_child_ptr)) ||
            (CTRLFORM_IsRectCovered(form_ctrl_ptr->relative_disp_rect,next_child_ptr->relative_rect)))
        {
            //next child is get active
            if (CTRLFORM_IsGetAtvChild(next_child_ptr))
            {
				//recursive call should have a definite termination condition to avoid stack overflow
                /*if (next_child_ptr == active_child_ptr)
                {
                    //get display top
                    result = GetDisplayTopForKeyDown(&display_top,&display_left,form_ctrl_ptr,&active_child_ptr);
                }
                else*/
                {
                    //set active control
                    active_child_ptr = next_child_ptr;
                    
                    result = TRUE;
                }
            }
            else
            {
                //get display top
                result = GetDisplayTopForKeyDown(&display_top,&display_left,form_ctrl_ptr,&active_child_ptr);
            }
        }
        else
        {
            //next child is get active
            if (CTRLFORM_IsGetAtvChild(next_child_ptr))
            {
                //set active control
                active_child_ptr = next_child_ptr;
            }
            
            //get next control height
            child_height = CTRLFORM_GetChildHeight(next_child_ptr);
            
            //next control height < display height
            if (child_height > display_height)
            {
                //set display top
                display_top = next_child_ptr->relative_rect.top;
            }
            else
            {
                //get child bottom
                child_bottom = CTRLFORM_GetChildBottom(next_child_ptr);
                
                //get display top by child bottom
                if (child_bottom < display_height)
                {
                    display_top = CTRLFORM_GetChildTop(next_child_ptr);
                }
                else
                {
                    display_top = child_bottom - display_height + 1;
                }
            }
            
            //adjust display top
            display_top = (int32)MIN(display_top,display_max_top);
            
            result = TRUE;
        }
    }
    
    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }
    
    //set display left
    if (PNULL != disp_left_ptr)
    {
        *disp_left_ptr = display_left;
    }
    
    //set active child
    if (PNULL != active_child_pptr)
    {
        *active_child_pptr = active_child_ptr;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get display top for handle key left message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyLeft(
                                      int32             *disp_top_ptr,      //in/out:
                                      int32             *disp_left_ptr,     //in/out:
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,     //in:
                                      CTRLFORM_CHILD_T  **active_child_pptr //in/out:
                                      )
{
    BOOLEAN             result = FALSE;
    uint16              child_width = 0;
    uint16              display_width = 0;
    int32               display_top = 0;
    int32               display_left = 0;
    int32               display_max_left = 0;
    CTRLFORM_CHILD_T    *active_child_ptr = form_ctrl_ptr->active_child_ptr;
    CTRLFORM_CHILD_T    *prev_child_ptr = PNULL;
    
    //set init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    //get previous active child
    if (GetValidOrAtvChildPtr(FALSE,TRUE,form_ctrl_ptr->theme.is_cir_handle_lr,form_ctrl_ptr,form_ctrl_ptr->cur_child_ptr,&prev_child_ptr))
    {
        //set current child
        form_ctrl_ptr->cur_child_ptr = prev_child_ptr;

        //set current child select index is last
        if (form_ctrl_ptr->theme.is_cir_handle_lr)
        {
            result = IGUICTRL_SetSelectIndex(prev_child_ptr->child_ctrl_ptr,FALSE,TRUE);
        }
        
        //set active control
        active_child_ptr = prev_child_ptr;
        
        //previous control is all display
        if ((!CTRLFORM_IsDockChild(prev_child_ptr)) &&
            (!CTRLFORM_IsRectCovered(form_ctrl_ptr->relative_disp_rect,prev_child_ptr->relative_rect)))
        {
            //get previous control width
            child_width = CTRLFORM_GetChildWidth(prev_child_ptr);
            
            //get display width
            display_width = CTRLFORM_GetDisplayWidth(TRUE,form_ctrl_ptr);
            
            //previous control width > display width
            if (child_width > display_width)
            {
                //set display left
                display_left = prev_child_ptr->relative_rect.right - display_width + 1;
            }
            else
            {
                //get display left by child left
                display_left = CTRLFORM_GetChildLeft(prev_child_ptr);
            }
            
            //get display max left
            display_max_left = CTRLFORM_GetDisplayMaxLeft(form_ctrl_ptr);
            
            //adjust display left
            display_left = (int32)MIN(display_left,display_max_left);
        }
        
        result = TRUE;
    }
    
    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }
    
    //set display left
    if (PNULL != disp_left_ptr)
    {
        *disp_left_ptr = display_left;
    }
    
    //set active child
    if (PNULL != active_child_pptr)
    {
        *active_child_pptr = active_child_ptr;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get display top for handle key right message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetDisplayTopForKeyRight(
                                       int32            *disp_top_ptr,      //in/out:may PNULL
                                       int32            *disp_left_ptr,     //in/out:may PNULL
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr,     //in:
                                       CTRLFORM_CHILD_T **active_child_pptr //in/out:
                                       )
{
    BOOLEAN             result = FALSE;
    uint16              child_width = 0;
    uint16              display_width = 0;
    int32               display_top = 0;
    int32               display_left = 0;
    int32               child_right = 0;
    int32               display_max_left = 0;
    CTRLFORM_CHILD_T    *active_child_ptr = form_ctrl_ptr->active_child_ptr;
    CTRLFORM_CHILD_T    *next_child_ptr = PNULL;
    
    //set init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    if (GetValidOrAtvChildPtr(FALSE,FALSE,form_ctrl_ptr->theme.is_cir_handle_lr,form_ctrl_ptr,form_ctrl_ptr->cur_child_ptr,&next_child_ptr))
    {
        //set current child
        form_ctrl_ptr->cur_child_ptr = next_child_ptr;

        //set current child select index is first
        if (form_ctrl_ptr->theme.is_cir_handle_lr)
        {
            result = IGUICTRL_SetSelectIndex(next_child_ptr->child_ctrl_ptr,TRUE,FALSE);
        }
        
        //set active control
        active_child_ptr = next_child_ptr;
        
        //next control is all display
        if ((!CTRLFORM_IsDockChild(next_child_ptr)) &&
            (!CTRLFORM_IsRectCovered(form_ctrl_ptr->relative_disp_rect,next_child_ptr->relative_rect)))
        {
            //get next control width
            child_width = CTRLFORM_GetChildWidth(next_child_ptr);
            
            //get display width
            display_width = CTRLFORM_GetDisplayWidth(TRUE,form_ctrl_ptr);
            
            //next control width > display width
            if (child_width > display_width)
            {
                //set display top
                display_left = next_child_ptr->relative_rect.left;
            }
            else
            {
                //get child right
                child_right = CTRLFORM_GetChildRight(next_child_ptr);
                
                //get display left by child right
                if (child_right < display_width)
                {
                    display_left = next_child_ptr->relative_rect.left;
                }
                else
                {
                    display_left = child_right - display_width + 1;
                }
            }
            
            //get display max left
            display_max_left = CTRLFORM_GetDisplayMaxLeft(form_ctrl_ptr);
            
            //adjust display top
            display_left = (int32)MIN(display_left,display_max_left);
        }
        
        result = TRUE;
    }
    
    //set display top
    if (PNULL != disp_top_ptr)
    {
        *disp_top_ptr = display_top;
    }
    
    //set display left
    if (PNULL != disp_left_ptr)
    {
        *disp_left_ptr = display_left;
    }
    
    //set active child
    if (PNULL != active_child_pptr)
    {
        *active_child_pptr = active_child_ptr;
    }
    
    return (result);
}


