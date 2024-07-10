/*****************************************************************************
** File Name:      ctrlprogress.c                                            *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE               NAME                     DESCRIPTION                   *
** 07/2012      nan.ji & xiaoqing.lu              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmitheme_progress.h"
#include "ctrlprogress.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set display rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlSetDispRect(
                                      CTRLBASE_OBJ_T* base_ctrl_ptr, 
                                      const GUI_RECT_T* rect_ptr,
                                      BOOLEAN is_update
                                      );

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlSetRect(
                                  CTRLBASE_OBJ_T* base_ctrl_ptr, 
                                  const GUI_RECT_T* rect_ptr
                                  );
/*****************************************************************************/
//  Description : display progress image
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawImage(
                             CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr,
                             CTRLPROGRESS_IMAGE_T   image_info,
                             GUI_RECT_T *disp_rect_ptr
                             );

/*****************************************************************************/
//  Description : display progress control
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDraw(
                        CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr
                        );

/*****************************************************************************/
//  Description : display progress fg
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawFg(
                          CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : display progress bg
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawBg(
                          CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr
                          );

/*****************************************************************************/
//  Description : init progress class
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlInitVtbl(
                                   CTRLPROGRESS_VTBL_T        *progress_vtbl_ptr
                                   );

/*****************************************************************************/
//  Description : progress construct
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlConstruct (
                                     CTRLBASE_OBJ_T           *base_ctrl_ptr,
                                     CTRLPROGRESS_INIT_PARAM_T   *progress_param_ptr
                                     );

/*****************************************************************************/
//  Description : progress destruct
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlDestruct (
                                    CTRLBASE_OBJ_T        *base_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressCtrlPackInitParam (
                                      GUIPROGRESS_INIT_DATA_T     *progress_init_ptr,   //in
                                      CTRLPROGRESS_INIT_PARAM_T   *progress_param_ptr   //out
                                      );

/*****************************************************************************/
//  Description : handle progress msg function
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProgressCtrlHandleMsg (
                                          CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
                                          MMI_MESSAGE_ID_E      msg_id,           //message
                                          DPARAM                param             //add data
                                          );

/*****************************************************************************/
//  Description : get progress pointer by control id
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL CTRLPROGRESS_OBJ_T* GetProgressPtr (
                                          MMI_HANDLE_T     ctrl_handle
                                          );

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:nan.ji & xiaoqing.lu
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ProgressTypeOf (
                              CTRLBASE_OBJ_T     *ctrl_ptr
                              );

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlGetHeightByWidth (
                                            CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
                                            uint16              width,          //in:
                                            uint16              *height_ptr     //in/out:
                                            );

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get progress type
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T PROGRESS_GetType(void)
{
    static TYPE_TAG_T    obj_type = 0;
    
    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
            CTRL_BASE_TYPE,                                  // parent class type
            "progress",                                         // control name
            sizeof (CTRLPROGRESS_OBJ_T),                        // size of control object struct
            (OBJECT_CONSTRUCTOR_FUNC) ProgressCtrlConstruct,    // control constructor function
            (OBJECT_DESTRUCTOR_FUNC) ProgressCtrlDestruct,      // control destructor function
            sizeof (CTRLPROGRESS_INIT_PARAM_T),                 // size of init param struct
            (PARAM_PACKING_FUNC) ProgressCtrlPackInitParam,     // convert init param
            sizeof (CTRLPROGRESS_VTBL_T),                       // size of virtual table
            (VTBL_INIT_FUNC) ProgressCtrlInitVtbl);             // init virtual table function
    }
    
    return obj_type;
}

/*****************************************************************************/
//  Description : init progress class
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlInitVtbl (
                                    CTRLPROGRESS_VTBL_T        *progress_vtbl_ptr
                                    )
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) progress_vtbl_ptr;
    
    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }
    
    base_ctrl_vtbl_ptr->HandleEvent = ProgressCtrlHandleMsg;
    
    // 以下接口用于实现内嵌form
    base_ctrl_vtbl_ptr->GetHeightByWidth = ProgressCtrlGetHeightByWidth;
    base_ctrl_vtbl_ptr->SetDisplayRect = ProgressCtrlSetDispRect;
    base_ctrl_vtbl_ptr->SetRect = ProgressCtrlSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}
/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlSetRect(
                                  CTRLBASE_OBJ_T* base_ctrl_ptr, 
                                  const GUI_RECT_T* rect_ptr
                                  )
{
    // CTRLPROGRESS_OBJ_T  *progress_ctrl_ptr = (CTRLPROGRESS_OBJ_T *)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    
    if (( PNULL == base_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }
    
    parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_PROGRESS_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
    
    if (PNULL != parent_vtbl_ptr)
    {
        parent_vtbl_ptr->SetRect(base_ctrl_ptr, rect_ptr);
    }
    
    return TRUE;
}
/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlSetDispRect(
                                      CTRLBASE_OBJ_T* base_ctrl_ptr, 
                                      const GUI_RECT_T* rect_ptr,
                                      BOOLEAN is_update
                                      )
{
    // CTRLPROGRESS_OBJ_T  *progress_ctrl_ptr = (CTRLPROGRESS_OBJ_T *)base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
    
    if (( PNULL == base_ctrl_ptr) || (PNULL == rect_ptr))
    {
        return FALSE;
    }
    
    parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_PROGRESS_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
    
    if (PNULL != parent_vtbl_ptr)
    {
        parent_vtbl_ptr->SetDisplayRect(base_ctrl_ptr, rect_ptr, is_update);
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
// 该方法用于将控件的对外初始化参数结构转换为内部的初始化参数结构
// 由于控件有继承关系，控件初始化时又带有参数，所以内部的初始化参数也必须有继承关系
/*****************************************************************************/
LOCAL void ProgressCtrlPackInitParam (
                                      GUIPROGRESS_INIT_DATA_T     *progress_init_ptr,   //in
                                      CTRLPROGRESS_INIT_PARAM_T   *progress_param_ptr   //out
                                      )
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) progress_param_ptr;
    
    if (PNULL == progress_init_ptr || PNULL == progress_param_ptr)
    {
        return;
    }
    
    base_ctrl_param_ptr->both_rect = progress_init_ptr->both_rect;
}

/*****************************************************************************/
//  Description : progress construct
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlConstruct (
                                     CTRLBASE_OBJ_T           *base_ctrl_ptr,
                                     CTRLPROGRESS_INIT_PARAM_T   *progress_param_ptr
                                     )
{
    BOOLEAN                result = TRUE;
    CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr = (CTRLPROGRESS_OBJ_T*) base_ctrl_ptr;
    MMI_PROGRESS_STYLE_T   progress_style = {0};

    if (PNULL == base_ctrl_ptr || PNULL == progress_param_ptr)
    {
        return FALSE;
    }
    
    MMITHEME_GetProgressStyle(&progress_style);
    
    progress_ctrl_ptr->bg_info.color = progress_style.bg_color;
    progress_ctrl_ptr->bg_info.image_id = progress_style.bg_image_id;
    progress_ctrl_ptr->bg_info.disable_color = progress_style.disable_bg_color;
    progress_ctrl_ptr->bg_info.disable_image_id = progress_style.disable_bg_image_id;
    
    progress_ctrl_ptr->fg_info.color = progress_style.fg_color;
    progress_ctrl_ptr->fg_info.image_id = progress_style.fg_image_id;
    progress_ctrl_ptr->fg_info.disable_color = progress_style.disable_fg_color;
    progress_ctrl_ptr->fg_info.disable_image_id = progress_style.disable_fg_image_id;
    
    
    return (result);
}

/*****************************************************************************/
//  Description : progress destruct
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlDestruct (
                                    CTRLBASE_OBJ_T    *base_ctrl_ptr
                                    )
{
    BOOLEAN             result = TRUE;
    CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr = (CTRLPROGRESS_OBJ_T*) base_ctrl_ptr;
    
    if (PNULL == progress_ctrl_ptr)
    {
        return FALSE;
    }
    
    // TODO
    // ...
    
    return (result);
}

/*****************************************************************************/
//  Description : handle progress msg function
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ProgressCtrlHandleMsg (
                                          CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
                                          MMI_MESSAGE_ID_E      msg_id,           //message
                                          DPARAM                param             //add data
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    CTRLPROGRESS_OBJ_T  *progress_ctrl_ptr = (CTRLPROGRESS_OBJ_T*) base_ctrl_ptr;
    CTRLBASE_VTBL_T     *parent_vtbl_ptr = (CTRLBASE_VTBL_T*)TYPEMNG_GetParentVtbl(CTRL_PROGRESS_TYPE, (OBJECT_TYPE_PTR)base_ctrl_ptr);
    
    if (PNULL == progress_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_CTL_OPEN:    
        break;

    case MSG_CTL_PAINT:
        ProgressDraw (progress_ctrl_ptr);
        break;
        
    case MSG_CTL_GET_ACTIVE:
        break;
        
    case MSG_CTL_LOSE_ACTIVE:
    case MSG_CTL_LOSE_FOCUS:
    case MSG_CTL_CLOSE:
        break;
        
    case MSG_NOTIFY_LOSE_ACTIVE:
        break;
        
    default:
        result = parent_vtbl_ptr->HandleEvent(base_ctrl_ptr, msg_id, param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : get progress pointer by control id
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL CTRLPROGRESS_OBJ_T* GetProgressPtr (
                                          MMI_HANDLE_T     ctrl_handle
                                          )
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (ProgressTypeOf (ctrl_ptr)); /*assert verified*/
    }
    
    return (CTRLPROGRESS_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:nan.ji & xiaoqing.lu
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN ProgressTypeOf (
                              CTRLBASE_OBJ_T     *ctrl_ptr
                              )
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_PROGRESS_TYPE);
}

/*****************************************************************************/
// 	Description : get control height by width,include content,border etc.
//	Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ProgressCtrlGetHeightByWidth (
                                            CTRLBASE_OBJ_T      *base_ctrl_ptr,  //in:
                                            uint16              width,          //in:
                                            uint16              *height_ptr     //in/out:
                                            )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL != base_ctrl_ptr && PNULL != height_ptr)
    {
        *height_ptr = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top + 1;
        result = TRUE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : ProgressCtrlAdjuestCur
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: return to head or to tail
/*****************************************************************************/
LOCAL void ProgressCtrlAdjuestCur(
                                  CTRLPROGRESS_OBJ_T *progress_ctrl_ptr
                                  )
{
    if (PNULL != progress_ctrl_ptr)
    {
        if (progress_ctrl_ptr->cur < progress_ctrl_ptr->min)
        {
            progress_ctrl_ptr->cur = progress_ctrl_ptr->min;
        }
        
        if (progress_ctrl_ptr->cur > progress_ctrl_ptr->max)
        {
            progress_ctrl_ptr->cur = progress_ctrl_ptr->max;
       }
    }
    
    return;
}

/*****************************************************************************/
//  Description : display progress control
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDraw(
                        CTRLPROGRESS_OBJ_T *progress_ctrl_ptr
                        )
{
    ProgressCtrlAdjuestCur(progress_ctrl_ptr);
    
    ProgressDrawBg(progress_ctrl_ptr);
    ProgressDrawFg(progress_ctrl_ptr);
    
    return;
}

/*****************************************************************************/
//  Description : display progress fg
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawFg(
                          CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr
                          )
{
    uint16     ctrl_width = 0;
    uint16     range = 0;
    GUI_RECT_T fg_rect = {0};
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    
    base_ctrl_ptr = (CTRLBASE_OBJ_T *)progress_ctrl_ptr;
    
    if (PNULL != progress_ctrl_ptr)
    {
        fg_rect = base_ctrl_ptr->rect;
        
        range = progress_ctrl_ptr->max - progress_ctrl_ptr->min;
        
        if (0 != range)
        {
            if (progress_ctrl_ptr->is_vertical)
            {
                ctrl_width = base_ctrl_ptr->rect.bottom - base_ctrl_ptr->rect.top;
                fg_rect.bottom = (progress_ctrl_ptr->cur - progress_ctrl_ptr->min)*ctrl_width/range + base_ctrl_ptr->rect.top;
            }
            else
            {
                ctrl_width = base_ctrl_ptr->rect.right - base_ctrl_ptr->rect.left;
                fg_rect.right = (progress_ctrl_ptr->cur - progress_ctrl_ptr->min)*ctrl_width/range + base_ctrl_ptr->rect.left;
            }
        }
        
        ProgressDrawImage(progress_ctrl_ptr, progress_ctrl_ptr->fg_info, &fg_rect);
    }
}

/*****************************************************************************/
//  Description : display progress bg
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawBg(
                          CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr
                          )
{
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    
    base_ctrl_ptr = (CTRLBASE_OBJ_T *)progress_ctrl_ptr;
    
    if (PNULL != progress_ctrl_ptr)
    {
        ProgressDrawImage(progress_ctrl_ptr, progress_ctrl_ptr->bg_info, &base_ctrl_ptr->rect);
    }
}

/*****************************************************************************/
//  Description : display progress image
//  Global resource dependence :
//  Author: nan.ji & xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ProgressDrawImage(
                             CTRLPROGRESS_OBJ_T     *progress_ctrl_ptr,
                             CTRLPROGRESS_IMAGE_T   image_info,
                             GUI_RECT_T *disp_rect_ptr
                             )
{
    GUI_COLOR_T color = 0;
    MMI_IMAGE_ID_T image_id = 0;
    GUI_RECT_T image_rect = {0};
    uint16 image_width = 0;
    uint16 image_height = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    GUI_RECT_T          cross_rect = {0};
    GUI_BG_DISPLAY_T    bg_display = {0};
    GUI_BG_T            bg_info = {0};
    
    base_ctrl_ptr = (CTRLBASE_OBJ_T *)progress_ctrl_ptr;
    
    if(PNULL != progress_ctrl_ptr
        && GUI_IntersectRect(&cross_rect, base_ctrl_ptr->display_rect, *disp_rect_ptr)
        && !GUI_IsInvalidRect(cross_rect))
    {
        if (base_ctrl_ptr->is_enabled)
        {
            color = image_info.color;
            image_id = image_info.image_id;
        }
        else
        {
            color = image_info.disable_color;
            image_id = image_info.disable_image_id;
        }
        
        if (MMITHEME_CheckImageID(image_id))
        {
            // 计算背景图片显示的区域
            GUIRES_GetImgWidthHeight(&image_width, &image_height, image_id, base_ctrl_ptr->win_handle);
            image_rect.bottom = image_height - 1;
            image_rect.right  = image_width - 1;
            
            if (disp_rect_ptr->top < cross_rect.top)
            {
                image_rect.top = (cross_rect.top - disp_rect_ptr->top);
                image_rect.bottom = image_height - 1;
            }
            else if (disp_rect_ptr->bottom > cross_rect.bottom)
            {
                image_rect.top = 0;
                image_rect.bottom = image_height - (disp_rect_ptr->bottom - cross_rect.bottom);
            }
            else
            {
                image_rect.top = 0;
                image_rect.bottom = image_height - 1;
            }

            //set bg display info
            bg_display.is_transparent = FALSE;
            bg_display.rect           = *disp_rect_ptr;
            bg_display.display_rect   = cross_rect;
            bg_display.img_rect       = image_rect;
            bg_display.ctrl_handle    = base_ctrl_ptr->handle;
            bg_display.win_handle     = base_ctrl_ptr->win_handle;
            
            bg_info.bg_type = GUI_BG_IMG;
            bg_info.img_id = image_id;
            GUI_DisplayBg(&bg_info, &bg_display, &(base_ctrl_ptr->lcd_dev_info));
        }
        else
        {
            GUI_FillRect(&base_ctrl_ptr->lcd_dev_info, cross_rect, color);
        }
    }
    
    return;
}

/*****************************************************************************/
//  Description : get current value
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLPROGRESS_GetValue(
                                   MMI_HANDLE_T ctrl_handle
                                   )
{
    int16 cur = 0;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        cur = progress_ctrl_ptr->cur;
    }
    
    return cur;
}


/*****************************************************************************/
//  Description : set current value
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetValue(
                                     MMI_HANDLE_T ctrl_handle,
                                     int16 cur_value
                                     )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->cur = cur_value;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set min
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetMin(
                                   MMI_HANDLE_T ctrl_handle,
                                   int16 min
                                   )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->min = min;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set max
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetMax(
                                   MMI_HANDLE_T ctrl_handle,
                                   int16 max
                                   )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->max = max;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetStep(
                                    MMI_HANDLE_T ctrl_handle,
                                    int16 step
                                    )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->step = step;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : set step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC int16 CTRLPROGRESS_GetStep(
                                  MMI_HANDLE_T ctrl_handle
                                  )
{
    int16 step = 0;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        step = progress_ctrl_ptr->step;
    }
    
    return step;
}

/*****************************************************************************/
//  Description : inc special step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_Increment(
                                      MMI_HANDLE_T ctrl_handle,
                                      int16 special_step
                                      )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->cur += special_step;
    }
    
    return result;
}

/*****************************************************************************/
//  Description :inc step
//  Global resource dependence :
//  Author: xiaoqing.lu & nan.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_Step(
                                 MMI_HANDLE_T ctrl_handle
                                 )
{
    BOOLEAN result = FALSE;
    CTRLPROGRESS_OBJ_T *progress_ctrl_ptr = GetProgressPtr(ctrl_handle);
    
    if (PNULL != progress_ctrl_ptr)
    {
        result = TRUE;
        progress_ctrl_ptr->cur += progress_ctrl_ptr->step;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : set rect
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_SetCtrlRect(
                                        MMI_HANDLE_T ctrl_handle, 
                                        const GUI_RECT_T* rect_ptr
                                        )
{
    return ProgressCtrlSetRect((CTRLBASE_OBJ_T*)GetProgressPtr(ctrl_handle), rect_ptr);
}

/*****************************************************************************/
//  Description : display slider control
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLPROGRESS_UpdateCtrl(
                                       MMI_HANDLE_T ctrl_handle
                                       )
{
    ProgressDraw(GetProgressPtr(ctrl_handle));
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : create the progress control
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: return ctrl handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLPROGRESS_Create(
                                        MMI_HANDLE_T win_handle,
                                        MMI_HANDLE_T parent_ctrl_handle,
                                        MMI_CTRL_ID_T ctrl_id
                                        )
{
    MMI_HANDLE_T ctrl_handle = 0;
    CTRLBASE_OBJ_T *base_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIPROGRESS_INIT_DATA_T   init_data = {0};

    create.ctrl_id           = ctrl_id;
    create.guid              = SPRD_GUI_PROGRESS_ID;
    create.parent_win_handle = win_handle;
    create.parent_ctrl_handle = parent_ctrl_handle;
    create.init_data_ptr     = &init_data;

    base_ctrl_ptr = (CTRLBASE_OBJ_T *)MMK_CreateControl( &create );
        
    if (PNULL != base_ctrl_ptr)
    {
        ctrl_handle = base_ctrl_ptr->handle;
    }

    return ctrl_handle;
}
