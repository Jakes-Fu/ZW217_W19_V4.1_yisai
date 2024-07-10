/*****************************************************************************
** File Name:      ctrlform_msg.c                                            *
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
**---------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "mmk_window_internal.h"
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
//  Description : form dispatch tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: form handle,dispatch to child
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpDownMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get form control which handle tp msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_OBJ_T* GetTpFormCtrlPtr(
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                       GUI_POINT_T      *tp_point_ptr
                                       );

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : get tp dock child point
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetTpDockChildPtr(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                          GUI_POINT_T       *tp_point_ptr
                                          );

/*****************************************************************************/
//  Description : is tp in child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInChild(
                          CTRLFORM_CHILD_T  *child_ptr,
                          GUI_POINT_T       *tp_point_ptr
                          );
#endif
#endif
/*****************************************************************************/
//  Description : form dispatch tp short msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: form not handle,dispatch to child,get return value
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpShortMsg(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                      DPARAM            param
                                      );

/*****************************************************************************/
//  Description : tp down control handle tp other msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TpDownCtrlHandleTpMsg(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : form dispatch tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpMoveMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : form dispatch tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpUpMsg(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                   DPARAM           param
                                   );

/*****************************************************************************/
//  Description : dispatch tp up msg to unit form child's active child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpUpMsgToChild(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                          CTRLFORM_CHILD_T  *child_ptr,
                                          DPARAM            param
                                          );

/*****************************************************************************/
//  Description : form dispatch tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpLongMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     );

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/
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
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
#ifdef TOUCH_PANEL_SUPPORT
    if (form_ctrl_ptr->theme.is_support_slide)
    {
        switch (msg_id)
        {
        case MSG_TP_PRESS_DOWN:
            //form handle,dispatch to child
            result = DispatchTpDownMsg(form_ctrl_ptr,param);
            break;

        case MSG_TP_PRESS_SHORT:
            //form not handle,dispatch to child,get return value
            result = DispatchTpShortMsg(form_ctrl_ptr,param);
            break;

        case MSG_TP_PRESS_MOVE:
            result = DispatchTpMoveMsg(form_ctrl_ptr,param);
            break;

        case MSG_TP_PRESS_UP:
            result = DispatchTpUpMsg(form_ctrl_ptr,param);
            break;

        case MSG_TP_PRESS_LONG:
            result = DispatchTpLongMsg(form_ctrl_ptr,param);
            break;
            
        default:
            break;
        }
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : form dispatch tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: form handle,dispatch to child
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpDownMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    BOOLEAN         get_result = FALSE;
    BOOLEAN         get_child_result = FALSE;
    uint32          topmost_level = 0;
    IGUICTRL_T      *ctrl_ptr = PNULL;
    GUI_POINT_T     tp_point = {0};
    MMI_HANDLE_T    win_handle = 0;
    MMI_HANDLE_T    ctrl_handle = 0;
    MMI_HANDLE_T    child_ctrl_handle = 0;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    //reset
    form_ctrl_ptr->is_tp_in_dock = FALSE;

    //get tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    //get handle by tp position
    get_result = MMK_GetHandleByPos(tp_point,TRUE,&win_handle,&ctrl_handle,0);
    
    //get control pointer
    ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    //get top most not form control,top most handle tp msg first
    if ((get_result) &&
        (0 != ctrl_handle) && 
        (PNULL != ctrl_ptr) && 
        (SPRD_GUI_FORM_ID != IGUICTRL_GetCtrlGuid(ctrl_ptr)))
    {
        for (topmost_level=GUICTRL_STATE_TOPMOST_MAX; topmost_level>0; topmost_level-=GUICTRL_STATE_TOPMOST_STEP)
        {
            if (IGUICTRL_GetState(ctrl_ptr,topmost_level))
            {
                return (result);
            }
        }
    }
    
    //is tp form rect and not tp prg and not im rect
    if ((MMK_PosIsCtrl(base_ctrl_ptr->handle,tp_point)) &&
        (GUI_PointIsInRect(tp_point,base_ctrl_ptr->rect)) &&
        ((PNULL == form_ctrl_ptr->prgbox_ctrl_ptr) || (!MMK_PosIsCtrl(IGUICTRL_GetCtrlHandle(form_ctrl_ptr->prgbox_ctrl_ptr),tp_point))))
    {
        result = MMI_RESULT_TRUE;
        
        //reset tp short result and tp down control
        form_ctrl_ptr->tpshort_result = MMI_RESULT_FALSE;
        form_ctrl_ptr->tpdown_ctrl_handle = 0;
        
        //get tp win handle and control handle
        if (get_result)
        {
            if ((0 != ctrl_handle) &&
                (ctrl_handle != base_ctrl_ptr->handle))
            {
                //set control active
                MMK_SetAtvCtrl(win_handle,ctrl_handle);
                
                //if tp control is form, dispatch msg to it's child
                if ((PNULL != ctrl_ptr) &&  
                    (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr)))
                {
                    //get tp child handle
                    get_child_result = MMK_GetChildHandleByPos(tp_point,ctrl_handle,&child_ctrl_handle);
                    if ((get_child_result) &&
                        (0 != child_ctrl_handle))
                    {
                        ctrl_handle = child_ctrl_handle;
                    }
                }
                
                //set tp down control handle
                form_ctrl_ptr->tpdown_ctrl_handle = ctrl_handle;
                
                //dispatch tp down msg to child control
                if (!MMK_RunCtrlProc(ctrl_handle,MSG_TP_PRESS_DOWN,param))
                {
                    //dispatch tp down msg to window
                    MMK_RunWinProc(win_handle,MSG_TP_PRESS_DOWN,param);
                    
                    //window may app close
                    if (!MMK_IsOpenWin(win_handle))
                    {
                        return (result);
                    }
                }
                
                //update active child
                CTRLFORM_UpdateChild(form_ctrl_ptr,form_ctrl_ptr->active_child_ptr,TRUE,TRUE);
            }
        }
        
        //get form control which handle tp msg
        form_ctrl_ptr->tp_form_ctrl_ptr = GetTpFormCtrlPtr(form_ctrl_ptr,&tp_point);
        if (PNULL != form_ctrl_ptr->tp_form_ctrl_ptr)
        {
            //fix NEWMS00198207
            //处在滚动状态,如果点在edit上,会出剪贴板滑块,刷新合成层,由于滑块只是一块小区域，
            //导致主层的内容部分刷到屏幕,部分没有,引起花屏
            if ((MMK_TP_SLIDE_NONE != form_ctrl_ptr->tp_form_ctrl_ptr->slide_info.slide_state) || 
                (UILAYER_IsPerDirectDraw()))
            {
                UILAYER_SetDirectDraw(FALSE);

                MMITHEME_StoreUpdateRect(&form_ctrl_ptr->tp_form_ctrl_ptr->theme.lcd_dev,
                    ((CTRLBASE_OBJ_T*)(form_ctrl_ptr->tp_form_ctrl_ptr))->display_rect);
            }

            CTRLFORM_HandleTpDown(form_ctrl_ptr->tp_form_ctrl_ptr,&tp_point);
        }
    }
#endif

    return (result);
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : get form control which handle tp msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_OBJ_T* GetTpFormCtrlPtr(
                                       CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                       GUI_POINT_T      *tp_point_ptr
                                       )
{
    IGUICTRL_T          *ctrl_ptr = PNULL;
    MMI_HANDLE_T        parent_handle = 0;
    CTRLFORM_OBJ_T      *tp_form_ctrl_ptr = PNULL;
#ifdef GUIF_FORM_DOCKABLE
    CTRLFORM_CHILD_T    *tp_child_ctrl_ptr = PNULL;
#endif

    //set tp form pointer
    tp_form_ctrl_ptr = form_ctrl_ptr;

    //get parent handle
    parent_handle = MMK_GetParentCtrlHandle(form_ctrl_ptr->tpdown_ctrl_handle);
    while (0 != parent_handle)
    {
        //is form
        ctrl_ptr = MMK_GetCtrlPtr(parent_handle);
        if (SPRD_GUI_FORM_ID == IGUICTRL_GetCtrlGuid(ctrl_ptr))
        {
#ifdef GUIF_FORM_DOCKABLE
            //tp in form dock child
            tp_child_ctrl_ptr = GetTpDockChildPtr((CTRLFORM_OBJ_T *)ctrl_ptr,tp_point_ptr);
            if (PNULL != tp_child_ctrl_ptr)
            {
                form_ctrl_ptr->is_tp_in_dock = TRUE;

                //set tp form is PNULL
                tp_form_ctrl_ptr = PNULL;
                break;
            }
#else
            GUI_INVALID_PARAM(tp_point_ptr);
#endif

            //is form has progress
            if (PNULL != ((CTRLFORM_OBJ_T*)ctrl_ptr)->prgbox_ctrl_ptr)
            {
                tp_form_ctrl_ptr = (CTRLFORM_OBJ_T *)ctrl_ptr;
                break;
            }
        }

        //get parent handle
        parent_handle = MMK_GetParentCtrlHandle(parent_handle);
    }

    return (tp_form_ctrl_ptr);
}

#ifdef GUIF_FORM_DOCKABLE
/*****************************************************************************/
//  Description : get tp dock child point
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL CTRLFORM_CHILD_T* GetTpDockChildPtr(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                         GUI_POINT_T        *tp_point_ptr
                                         )
{
    uint16              cur_priority = 0;
    CTRLFORM_CHILD_T    *child_ptr = PNULL;
    CTRLFORM_CHILD_T    *tp_child_ptr = PNULL;

    //find dock child
    cur_priority = 1;
    child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
    while (PNULL != child_ptr)
    {
        //is tp in child
        if (IsTpInChild(child_ptr,tp_point_ptr))
        {
            tp_child_ptr = child_ptr;
            break;
        }
        else
        {
            //get next dock child
            child_ptr = CTRLFORM_GetDockChildPtr(&cur_priority,form_ctrl_ptr);
        }
    }

    return (tp_child_ptr);
}

/*****************************************************************************/
//  Description : is tp in child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpInChild(
                          CTRLFORM_CHILD_T  *child_ptr,
                          GUI_POINT_T       *tp_point_ptr
                          )
{
    BOOLEAN     result = FALSE;
    GUI_RECT_T  child_rect = {0};

    //is tp child rect
    if ((PNULL != child_ptr) &&
        (MMK_PosIsCtrl(child_ptr->ctrl_handle,*tp_point_ptr)))
    {
        //get child rect
        IGUICTRL_GetRect(child_ptr->child_ctrl_ptr,&child_rect);

        if (GUI_PointIsInRect(*tp_point_ptr,child_rect))
        {
            result = TRUE;
        }
    }

    return (result);
}
#endif
#endif

/*****************************************************************************/
//  Description : form dispatch tp short msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: form not handle,dispatch to child,get return value
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpShortMsg(
                                      CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                      DPARAM            param
                                      )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    //tp down control handle msg
    result = TpDownCtrlHandleTpMsg(form_ctrl_ptr,MSG_TP_PRESS_SHORT,param);
    
    //set tp short msg handle result
    form_ctrl_ptr->tpshort_result = result;
    
    if (MMI_RESULT_FALSE == result)
    {
        //dispatch tp msg to window
        result = MMK_RunWinProc(form_ctrl_ptr->win_handle,MSG_TP_PRESS_SHORT,param);
    }
    else
    {
#ifdef TOUCH_PANEL_SUPPORT
        //stop slide timer
        if (PNULL != form_ctrl_ptr->tp_form_ctrl_ptr)
        {
            CTRLFORM_StopTpSlideTimer(form_ctrl_ptr->tp_form_ctrl_ptr);
        }
#endif
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : tp down control handle tp other msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E TpDownCtrlHandleTpMsg(
                                         CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                         MMI_MESSAGE_ID_E   msg_id,
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLBASE_OBJ_T  *base_ctrl_ptr = (CTRLBASE_OBJ_T*)form_ctrl_ptr;
    
    if ((0 != form_ctrl_ptr->tpdown_ctrl_handle) &&
        (MMK_IsResponseToClick(form_ctrl_ptr->tpdown_ctrl_handle)))
    {
        if (MMK_IsFocusWin(MMK_GetWinHandleByCtrl(form_ctrl_ptr->tpdown_ctrl_handle)))
        {
            //fix NEWMS00198207
            //处在滚动状态,如果点在edit上,会出剪贴板滑块,刷新合成层,由于滑块只是一块小区域，
            // 导致主层的内容部分刷到屏幕,部分没有,引起花屏
            if ((MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state) || 
                (UILAYER_IsPerDirectDraw()))
            {
                UILAYER_SetDirectDraw(FALSE);

                MMITHEME_StoreUpdateRect(&form_ctrl_ptr->theme.lcd_dev,
                    base_ctrl_ptr->display_rect);
            }
            
            //dispatch tp msg to tp down control
            result = MMK_RunCtrlProc(form_ctrl_ptr->tpdown_ctrl_handle,msg_id,param);
        }
        else
        {
            //按住tp切换窗口,再松开tp时,丢弃该消息,避免花屏
            //set tp down control handle
            form_ctrl_ptr->tpdown_ctrl_handle = 0;
            //SCI_TRACE_LOW:"TpDownCtrlHandleTpMsg:tp down control is not focus!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFORM_MSG_338_112_2_18_3_18_35_165,(uint8*)"");
            
            result = MMI_RESULT_TRUE;
        }
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : form dispatch tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpMoveMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_POINT_T     tp_point = {0};
    
    //set not tp down
    CTRLFORM_UpdateChild(form_ctrl_ptr,form_ctrl_ptr->active_child_ptr,FALSE,FALSE);
    
    //tp in dock or handle tp short msg
    if ((form_ctrl_ptr->is_tp_in_dock) ||
        (form_ctrl_ptr->tpshort_result))
    {
        //dispatch msg to child,tp down control handle msg
        result = TpDownCtrlHandleTpMsg(form_ctrl_ptr,MSG_TP_PRESS_MOVE,param);
        if (MMI_RESULT_FALSE == result)
        {
            //dispatch tp msg to window
            result = MMK_RunWinProc(form_ctrl_ptr->win_handle,MSG_TP_PRESS_MOVE,param);
        }
    }
    else
    {
        //get tp press point
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        
        //form slide
        if (PNULL != form_ctrl_ptr->tp_form_ctrl_ptr)
        {
            CTRLFORM_HandleTpMove(form_ctrl_ptr->tp_form_ctrl_ptr,&tp_point);
        }
        
        result = MMI_RESULT_TRUE;
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : form dispatch tp up msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpUpMsg(
                                   CTRLFORM_OBJ_T   *form_ctrl_ptr,
                                   DPARAM           param
                                   )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT
    GUI_POINT_T     tp_point = {0};
    
    //get tp press point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    //update active child
    CTRLFORM_UpdateChild(form_ctrl_ptr,form_ctrl_ptr->active_child_ptr,FALSE,TRUE);
    
    //form handle
    if (PNULL != form_ctrl_ptr->tp_form_ctrl_ptr)
    {
        //form is slide
        if (MMK_TP_SLIDE_NONE != form_ctrl_ptr->tp_form_ctrl_ptr->slide_info.slide_state)
        {
            ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->is_slide = TRUE;
        }

        CTRLFORM_HandleTpUp(form_ctrl_ptr->tp_form_ctrl_ptr,&tp_point);
    }
    else if (MMK_TP_SLIDE_NONE != form_ctrl_ptr->slide_info.slide_state)
    {
        ((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->is_slide = TRUE;
    }

    //dispatch msg to child,tp down control handle msg
    result = TpDownCtrlHandleTpMsg(form_ctrl_ptr,MSG_TP_PRESS_UP,param);
    if (MMI_RESULT_FALSE == result)
    {
        //dispatch tp msg to active child
        result = DispatchTpUpMsgToChild(form_ctrl_ptr,form_ctrl_ptr->active_child_ptr,param);
        if (MMI_RESULT_FALSE == result)
        {
            //dispatch tp msg to window
            result = MMK_RunWinProc(form_ctrl_ptr->win_handle,MSG_TP_PRESS_UP,param);
        }
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : dispatch tp up msg to unit form child's active child
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpUpMsgToChild(
                                          CTRLFORM_OBJ_T    *form_ctrl_ptr,
                                          CTRLFORM_CHILD_T   *child_ptr,
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E                    result = MMI_RESULT_FALSE;
#ifdef TOUCH_PANEL_SUPPORT    
    GUI_POINT_T                     tp_point = {0};
    GUI_RECT_T                      ctrl_rect = {0};
    CTRLFORM_OBJ_T                  *child_form_ctrl_ptr = PNULL;
    CTRLFORM_CHILD_T                *active_child_ptr = PNULL;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    *msg_param_ptr = PNULL;
    
    if ((PNULL != child_ptr) &&
        (CTRLFORM_IsUnitForm(form_ctrl_ptr)) &&
        (CTRLFORM_IsUnitFormChild(child_ptr)))
    {
        //get child form pointer
        child_form_ctrl_ptr = CTRLFORM_GetPtr(child_ptr->ctrl_handle);
        
        //get active child pointer
        if (PNULL != child_form_ctrl_ptr)
        {
            active_child_ptr = child_form_ctrl_ptr->active_child_ptr;
            while (CTRLFORM_IsFormChild(active_child_ptr))
            {
                child_form_ctrl_ptr = CTRLFORM_GetPtr(active_child_ptr->ctrl_handle);
                if (PNULL != child_form_ctrl_ptr)
                {
                    active_child_ptr = child_form_ctrl_ptr->active_child_ptr;
                }
                else
                {
                    active_child_ptr = PNULL;
                    break;
                }
            }
            
            if ((PNULL != child_form_ctrl_ptr) && 
                (PNULL != active_child_ptr))
            {
                //modify param tp point
                msg_param_ptr = (MMI_MULTI_KEY_TP_MSG_PARAM_T *)param;
                tp_point = msg_param_ptr->cur_tp_point;
                
                IGUICTRL_GetRect(active_child_ptr->child_ctrl_ptr,&ctrl_rect);
                msg_param_ptr->cur_tp_point.x = ctrl_rect.left;
                msg_param_ptr->cur_tp_point.y = ctrl_rect.top;
                
                //a pair tp msg down and up
                IGUICTRL_HandleEvent(active_child_ptr->child_ctrl_ptr,MSG_TP_PRESS_DOWN,param);
                result = IGUICTRL_HandleEvent(active_child_ptr->child_ctrl_ptr,MSG_TP_PRESS_UP,param);
                
                //reset param tp point
                msg_param_ptr->cur_tp_point = tp_point;
            }
        }
    }
#endif
    
    return (result);
}

/*****************************************************************************/
//  Description : form dispatch tp long msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DispatchTpLongMsg(
                                     CTRLFORM_OBJ_T     *form_ctrl_ptr,
                                     DPARAM             param
                                     )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
    //form not slide,dispatch tp long msg to child
    if (MMK_TP_SLIDE_NONE == form_ctrl_ptr->slide_info.slide_state)
    {
        //tp down control handle msg
        result = TpDownCtrlHandleTpMsg(form_ctrl_ptr,MSG_TP_PRESS_LONG,param);
        if (MMI_RESULT_FALSE == result)
        {
            //dispatch tp msg to window
            result = MMK_RunWinProc(form_ctrl_ptr->win_handle,MSG_TP_PRESS_LONG,param);
        }
    }
    
    return (result);
}
