/*****************************************************************************
** File Name:      ctrlform_tp.c                                             *
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
#ifdef TOUCH_PANEL_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "chng_freq.h"
#include "mmk_app.h"
#include "mmk_tp.h"
#include "ctrlform.h"
#include "guires.h"

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
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
                             CTRLFORM_OBJ_T *form_ctrl_ptr
                             );

/*****************************************************************************/
//  Description : check tp move can make slide or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CanTpMakeSlide(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr,     //in:  
                             GUI_POINT_T        *start_point_ptr,   //in:
                             GUI_POINT_T        *end_point_ptr      //in:
                             );

/*****************************************************************************/
//  Description : is up/down slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUpDownSlide(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle form up/down slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormUpDownSlide(
                                 GUI_POINT_T        tp_point,
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr
                                 );

/*****************************************************************************/
//  Description : double memory write back
//  Global resource dependence : 
//  Author: Jassmine 
//  Note:
/*****************************************************************************/
LOCAL void DoDoubleMemWriteBack(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr,
                                int32           offset_y
                                );
/*****************************************************************************/
//  Description : is need paint parent
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPaintParent(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            );

/*****************************************************************************/
//  Description : handle form left/right slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormLeftRightSlide(
                                    GUI_POINT_T     tp_point,
                                    CTRLFORM_OBJ_T  *form_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : set relative display rect for slide sbs form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSbsDisplayRect(
                                int32           display_left,   //in:
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                );

/*****************************************************************************/
//  Description : is form child exist layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsChildExistLayer(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle form tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpDown(
                                  CTRLFORM_OBJ_T *form_ctrl_ptr,
                                  GUI_POINT_T    *tp_point_ptr
                                  )
{
    if ((form_ctrl_ptr->theme.is_support_slide) &&
        (form_ctrl_ptr->is_valid_child))
    {
        //is tp down
        form_ctrl_ptr->is_tp_down = TRUE;
        
        //set start and previous point
        form_ctrl_ptr->slide_info.start_point = *tp_point_ptr;
        form_ctrl_ptr->slide_info.prev_point  = *tp_point_ptr;
        
        //set tp down slide state
        form_ctrl_ptr->slide_info.tpdown_slide_state = form_ctrl_ptr->slide_info.slide_state;
        
        //add velocity item
        MMK_AddVelocityItem(tp_point_ptr->x,tp_point_ptr->y);
        
        if (MMK_TP_SLIDE_FLING != form_ctrl_ptr->slide_info.slide_state)
        {
            //start tp slide timer
            StartTpSlideTimer(form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : start tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartTpSlideTimer(
                             CTRLFORM_OBJ_T *form_ctrl_ptr
                             )
{
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //include progress bar
    if (0 == form_ctrl_ptr->slide_info.slide_timer_id)
    {
        form_ctrl_ptr->slide_info.slide_timer_id = MMK_CreateWinTimer(base_ctrl_ptr->handle,
            form_ctrl_ptr->theme.slide_period,
            FALSE);
    }
}

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_StopTpSlideTimer(
                                      CTRLFORM_OBJ_T     *form_ctrl_ptr
                                      )
{
    if (0 < form_ctrl_ptr->slide_info.slide_timer_id)
    {
        MMK_StopTimer(form_ctrl_ptr->slide_info.slide_timer_id);
        form_ctrl_ptr->slide_info.slide_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle form tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpMove(
                                  CTRLFORM_OBJ_T *form_ctrl_ptr,
                                  GUI_POINT_T    *tp_point_ptr
                                  )
{
    //is glide
    if ((form_ctrl_ptr->theme.is_support_slide) &&
        (form_ctrl_ptr->is_valid_child))
    {
        if (CanTpMakeSlide(form_ctrl_ptr,&form_ctrl_ptr->slide_info.start_point,tp_point_ptr))
        {
            //set slide state
            if ((MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.tpdown_slide_state) ||
                (MMK_IsWayChanged(form_ctrl_ptr->slide_info.fling_velocity,
                        &form_ctrl_ptr->slide_info.start_point,
                        tp_point_ptr,
                        FALSE)))
            {
                form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_TP_SCROLL;
            }
        }
        
        //add velocity item
        MMK_AddVelocityItem(tp_point_ptr->x,tp_point_ptr->y);
    }
}

/*****************************************************************************/
//  Description : check tp move can make slide or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CanTpMakeSlide(
                             CTRLFORM_OBJ_T     *form_ctrl_ptr,     //in:  
                             GUI_POINT_T        *start_point_ptr,   //in:
                             GUI_POINT_T        *end_point_ptr      //in:
                             )
{
    BOOLEAN     result = FALSE;

    //up/down slide
    if (IS_TP_MOVE_Y(start_point_ptr->y,end_point_ptr->y))
    {
        if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
        {
            result = TRUE;
        }
        else
        {
            if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
            {
                result = TRUE;
            }
        }
    }

    if ((!result) &&
        (CTRLFORM_IsSbsForm(form_ctrl_ptr)))
    {
        //left/right slide
        if ((IS_TP_MOVE_X(start_point_ptr->x,end_point_ptr->x)) && 
            (form_ctrl_ptr->sbs_width > form_ctrl_ptr->relative_rect.right - form_ctrl_ptr->relative_rect.left + 1))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle form tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleTpUp(
                                CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                GUI_POINT_T      *tp_point_ptr
                                )
{   
    //is tp down
    form_ctrl_ptr->is_tp_down = FALSE;
    
    //stop slide timer
    CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
    
    if ((form_ctrl_ptr->is_valid_child) && 
        ((MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state) ||
        (CanTpMakeSlide(form_ctrl_ptr,&form_ctrl_ptr->slide_info.start_point,tp_point_ptr))))
    {
        //add velocity item
        MMK_AddVelocityItem(tp_point_ptr->x,tp_point_ptr->y);
        
        //calculate current velocity
        if ((form_ctrl_ptr->slide_info.slide_state == MMK_TP_SLIDE_FLING) && 
            (abs((int)form_ctrl_ptr->slide_info.fling_velocity) > FLING_MIN_VELOCITY))
        {
            MMK_ComputeCurrentVelocity(PNULL,&form_ctrl_ptr->slide_info.fling_velocity,FLING_MAX_SUM_VELOCITY,FLING_MAX_SUM_VELOCITY);
        }
        else
        {
            MMK_ComputeCurrentVelocity(PNULL,&form_ctrl_ptr->slide_info.fling_velocity,FLING_MAX_VELOCITY, FLING_MAX_VELOCITY);
        }
        
        //fling velocity
        if (FLING_MIN_VELOCITY > abs((int32)form_ctrl_ptr->slide_info.fling_velocity))
        {
            //reset velocity item
            MMK_ResetVelocityItem();
            form_ctrl_ptr->slide_info.fling_velocity = 0;
            
            //reset slide state
            form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
            
            //reset form slide
            CTRLFORM_ResetSlide(TRUE,form_ctrl_ptr);
        }
        else
        {
            //enter fling state
            form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_FLING;
            
            //start tp slide timer
            StartTpSlideTimer(form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : handle form tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_HandleSlideTimer(
                                      CTRLFORM_OBJ_T     *form_ctrl_ptr
                                      )
{
    BOOLEAN             is_updown = FALSE;
    GUI_POINT_T         tp_point = {0};
    MMI_TP_STATUS_E     tp_status = MMI_TP_NONE;

    //stop slide timer
    CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);

    // Don't do slide work if prgbox_ctrl_ptr is NULL.FIX BUG:1886918
#ifdef JAVASCRIPT_ECO_ENVIRONMENT_SUPPORT
    if (form_ctrl_ptr->prgbox_ctrl_ptr == NULL)
    {
        return;
    }
#endif
    if (MMK_IsFocusWin(form_ctrl_ptr->win_handle))
    {
        //start tp slide timer
        StartTpSlideTimer(form_ctrl_ptr);

        if (MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state)
        {
            MMK_GetLogicTPMsg(&tp_status,&tp_point);

            //handle slide
            if ((!form_ctrl_ptr->is_tp_down) || 
                (CanTpMakeSlide(form_ctrl_ptr,&form_ctrl_ptr->slide_info.prev_point,&tp_point)))
            {
                form_ctrl_ptr->slide_info.is_on_slide_paint = TRUE;

                //is up/down slide
                is_updown = IsUpDownSlide(form_ctrl_ptr);

                if (is_updown)
                {
                    HandleFormUpDownSlide(tp_point,form_ctrl_ptr);
                }
                else
                {
                    HandleFormLeftRightSlide(tp_point, form_ctrl_ptr);
                }

                form_ctrl_ptr->slide_info.is_on_slide_paint = FALSE;

                //set previous point
                form_ctrl_ptr->slide_info.prev_point = tp_point;

                VTLBASE_SetSlideState((CTRLBASE_OBJ_T*)form_ctrl_ptr,TRUE);
            }
        }
    }
}

/*****************************************************************************/
//  Description : is up/down slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsUpDownSlide(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            )
{
    BOOLEAN     result = FALSE;

    if (CTRLFORM_IsOrderForm(form_ctrl_ptr))
    {
        result = TRUE;
    }
    else
    {
        if (PNULL != form_ctrl_ptr->prgbox_ctrl_ptr)
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle form up/down slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormUpDownSlide(
                                 GUI_POINT_T        tp_point,
                                 CTRLFORM_OBJ_T     *form_ctrl_ptr
                                 )
{
    BOOLEAN     is_reset = FALSE;
    uint16      display_height = 0;
    int32       scroll_step = 0;
    int32       display_top = 0;
    int32       display_left = 0;
    int32       display_min_top = 0;
    int32       display_max_top = 0;
    int32       offset_y = 0;
    
    //init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    offset_y = display_top;
    
    switch (form_ctrl_ptr->slide_info.slide_state)
    {
    case MMK_TP_SLIDE_TP_SCROLL:
        scroll_step = tp_point.y - form_ctrl_ptr->slide_info.prev_point.y;
        
        display_top = display_top - scroll_step;
        break;
        
    case MMK_TP_SLIDE_FLING:
        //get fling offset      
        scroll_step = MMK_GetFlingOffset(0,form_ctrl_ptr->slide_info.fling_velocity,(float)FLING_TIME,&form_ctrl_ptr->slide_info.fling_velocity);

        //get display height
        display_height = CTRLFORM_GetDisplayHeight(form_ctrl_ptr);
        
        //get display min and max top
        display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
        display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
        
        if (0 == form_ctrl_ptr->slide_info.fling_velocity)
        {
            //stop slide timer
            CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
            
            //reset slide state
            form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
            
            is_reset = TRUE;
        }
        else if (0 > form_ctrl_ptr->slide_info.fling_velocity)//up
        {
            //set display top
            display_top = display_top - scroll_step;
            
            //last item display 1/2 in display rect
            if (display_top > display_max_top + (display_height>>1))
            {
                //stop slide timer
                CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
                
                //reset slide state
                form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
                
                display_top = display_max_top + (display_height>>1);
                
                is_reset = TRUE;
            }
        }
        else//down
        {
            //set display top
            display_top = display_top - scroll_step;
            
            //last item display 1/2 in display rect
            if ((display_min_top > display_top) &&
                ((-display_top) > (display_height>>1)))
            {
                //stop slide timer
                CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);
                
                //reset slide state
                form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
                
                display_top = display_min_top - (display_height>>1);
                
                is_reset = TRUE;
            }
        }
        break;
        
    default:
        break;
    }
    
    if (CTRLFORM_SetDisplayRect(display_top,display_left,form_ctrl_ptr))
    {
        SCI_MEMSET(&form_ctrl_ptr->writeback_rect,0,sizeof(GUI_RECT_T));
        
        DoDoubleMemWriteBack(form_ctrl_ptr,offset_y);
        
        //若无回写才需要擦图层
        if (GUI_IsZeroRect(form_ctrl_ptr->writeback_rect))
        {
            //update display
            CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
        }
        
        //set child rect
        CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
        
        SCI_MEMSET(&form_ctrl_ptr->writeback_rect, 0, sizeof(GUI_RECT_T));
    }
    
    //reset
    if (is_reset)
    {
        CTRLFORM_ResetSlide(TRUE,form_ctrl_ptr);
    }
    else
    {
        UILAYER_SetDirectDraw(TRUE);
    }
}

/*****************************************************************************/
//  Description : double memory write back
//  Global resource dependence : 
//  Author: Jassmine 
//  Note:
/*****************************************************************************/
LOCAL void DoDoubleMemWriteBack(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr,
                                int32           offset_y
                                )
{
    int32               offset = 0;
    GUI_RECT_T          src_rc = {0};
    GUI_RECT_T          dst_rc = {0};
    GUI_RECT_T          clip_rc = {0};
    GUI_BIG_RECT_T      display_rect = {0};
    CTRLBASE_OBJ_T      *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;

    if ((PNULL != form_ctrl_ptr) &&
        (!IsChildExistLayer(form_ctrl_ptr)) &&
        (!GUICTRL_GetState((IGUICTRL_T*)form_ctrl_ptr,GUICTRL_STATE_SLIDE)) && //滑动第一帧需要重画
        (IsPaintParent(form_ctrl_ptr)) && 
        (!(UILAYER_GetLayerMemFormat(&base_ctrl_ptr->lcd_dev_info) & UILAYER_MEM_FORMAT_DOUBLE)))
    {
        offset = offset_y - form_ctrl_ptr->relative_disp_rect.top;
        if (0 != offset)
        {
            //set src display rect
            display_rect = form_ctrl_ptr->relative_disp_rect;
            CTRLFORM_ClientToScreen(form_ctrl_ptr,
                &display_rect);

            //set src rect
            src_rc.left   = (int16)display_rect.left;
            src_rc.top    = (int16)display_rect.top;
            src_rc.right  = (int16)display_rect.right;
            src_rc.bottom = (int16)display_rect.bottom;

            //set dst rect
            dst_rc = src_rc;
            dst_rc.top    = (int16)(src_rc.top + offset);
            dst_rc.bottom = (int16)(src_rc.bottom + offset);

            //update display
            CTRLFORM_DisplayRect(&src_rc,form_ctrl_ptr);

            //set clip rect
            if (GUI_IntersectRect(&clip_rc,src_rc,dst_rc))
            {
                if (UILAYER_DoubleMemWriteBack(&base_ctrl_ptr->lcd_dev_info,&dst_rc,&clip_rc,&src_rc))
                {
                    //如果回写bufer成功则将相应的区域值上
                    form_ctrl_ptr->writeback_rect = clip_rc;
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : is need paint parent
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsPaintParent(
                            CTRLFORM_OBJ_T  *form_ctrl_ptr
                            )
{
    BOOLEAN         result = FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;

    switch (base_ctrl_ptr->bg.bg_type)
    {
    case GUI_BG_NONE:
        result = TRUE;
        break;

    case GUI_BG_IMG:
        if (GUIRES_IsArgb(base_ctrl_ptr->bg.img_id,form_ctrl_ptr->win_handle))
        {
            result = TRUE;
        }
        break;

    default:
        result = FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle form left/right slide
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void HandleFormLeftRightSlide(
                                    GUI_POINT_T     tp_point,
                                    CTRLFORM_OBJ_T  *form_ctrl_ptr
                                    )
{
    int32       scroll_step = 0;
    int32       display_left = 0;

    //init display left
    display_left = form_ctrl_ptr->relative_disp_rect.left;

    switch (form_ctrl_ptr->slide_info.slide_state)
    {
    case MMK_TP_SLIDE_TP_SCROLL:
        scroll_step  = tp_point.x - form_ctrl_ptr->slide_info.prev_point.x;

        display_left = display_left - scroll_step;

        //不能滑出界
        if (form_ctrl_ptr->relative_rect.left > display_left)
        {
            display_left = form_ctrl_ptr->relative_rect.left;
        }
        else if (display_left > form_ctrl_ptr->sbs_width - (form_ctrl_ptr->relative_disp_rect.right - form_ctrl_ptr->relative_disp_rect.left + 1))
        {
            display_left = form_ctrl_ptr->relative_rect.left + form_ctrl_ptr->sbs_width - (form_ctrl_ptr->relative_disp_rect.right - form_ctrl_ptr->relative_disp_rect.left + 1);
        }
        break;

    case MMK_TP_SLIDE_FLING:
        //stop slide timer
        CTRLFORM_StopTpSlideTimer(form_ctrl_ptr);

        //reset velocity item
        MMK_ResetVelocityItem();

        //reset slide state
        form_ctrl_ptr->slide_info.slide_state = MMK_TP_SLIDE_NONE;
        form_ctrl_ptr->slide_info.fling_velocity = 0;
        break;

    default:
        break;
    }

    if (SetSbsDisplayRect(display_left,form_ctrl_ptr))
    {
        //update display
        CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);

        //set child rect
        CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : set relative display rect for slide sbs form
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetSbsDisplayRect(
                                int32           display_left,    //in:
                                CTRLFORM_OBJ_T  *form_ctrl_ptr  //in:
                                )
{
    BOOLEAN     result = FALSE;
    uint16      display_width = 0;

    //adjust display left
    if (display_left != form_ctrl_ptr->relative_disp_rect.left)
    {
        //get display width
        display_width = (uint16)(form_ctrl_ptr->relative_disp_rect.right - form_ctrl_ptr->relative_disp_rect.left + 1);

        //set display left and right
        form_ctrl_ptr->relative_disp_rect.left  = display_left;
        form_ctrl_ptr->relative_disp_rect.right = display_left + display_width - 1;

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : reset form slide display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLFORM_ResetSlide(
                                BOOLEAN          is_update,
                                CTRLFORM_OBJ_T   *form_ctrl_ptr
                                )
{
    uint16      i = 0;
    uint16      reset_slide_frame = 0;
    int32       display_top = 0;
    int32       display_left = 0;
    int32       display_min_top = 0;
    int32       display_max_top = 0;
    int32       move_y_offset = 0;
    int32       scroll_step = 0;
    
    //init display top
    display_top  = form_ctrl_ptr->relative_disp_rect.top;
    display_left = form_ctrl_ptr->relative_disp_rect.left;
    
    //get display min and max top
    display_min_top = CTRLFORM_GetDisplayMinTop(form_ctrl_ptr);
    display_max_top = CTRLFORM_GetDisplayMaxTop(form_ctrl_ptr);
    
    //is need reset
    if ((display_min_top > display_top) ||
        (display_max_top < display_top))
    {
#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif
        
        if (display_min_top > display_top)
        {
            move_y_offset = display_min_top - display_top;
        }
        else
        {
            move_y_offset = display_max_top - display_top;
        }
        
        if (is_update)
        {
            reset_slide_frame = form_ctrl_ptr->theme.reset_slide_frame;
        }
        else
        {
            reset_slide_frame = form_ctrl_ptr->theme.reset_slide_start;
        }
        
        for (i=form_ctrl_ptr->theme.reset_slide_start; i<=reset_slide_frame; i++)
        {
            //calculate scroll step
            scroll_step = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1,(int16)move_y_offset,reset_slide_frame,i);
            
            if (CTRLFORM_SetDisplayRect(display_top + scroll_step,display_left,form_ctrl_ptr))
            {
                //update display
                CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
                
                //set child rect
                CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
                
                if (i < reset_slide_frame)
                {
                    UILAYER_SetDirectDraw(TRUE);
                    
                    //update
                    MMITHEME_UpdateRect();
                }
            }
        }
        
        //若前一帧为direct draw,需要重画
        if ((MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state) || 
            (UILAYER_IsPerDirectDraw()))
        {
            //update display
            CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
            
            //set child rect
            CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
        }
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif
    }
    else
    {
        //若前一帧为direct draw,需要重画
        if ((MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state) || 
            (UILAYER_IsPerDirectDraw()))
        {
            //update display
            CTRLFORM_Display(FALSE,TRUE,form_ctrl_ptr);
            
            //set child rect
            CTRLFORM_SetChildRect(TRUE,form_ctrl_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : is form child exist layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsChildExistLayer(
                                CTRLFORM_OBJ_T  *form_ctrl_ptr
                                )
{
    BOOLEAN             result = FALSE;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;

    if (form_ctrl_ptr->is_child_layer)
    {
        result = TRUE;
    }
    else
    {
        child_ptr = form_ctrl_ptr->first_child_ptr;
        while (PNULL != child_ptr)
        {
            if ((CTRLFORM_IsValidChild(child_ptr)) && 
                (CTRLFORM_IsFormChild(child_ptr)))
            {
                if (IsChildExistLayer((CTRLFORM_OBJ_T *)child_ptr->child_ctrl_ptr))
                {
                    result = TRUE;
                    break;
                }
            }

            child_ptr = child_ptr->next_ptr;
        }
    }

    return (result);
}

#endif
