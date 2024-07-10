/*****************************************************************************
** File Name:      guictrl_api.c                                             *
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

#ifndef _GUICTRL_API_C_
#define _GUICTRL_API_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "cafcontrol.h"
#include "guictrl_api.h"
#include "mmi_theme.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define CTRL_ALIGN_LEFT   0
#define CTRL_ALIGN_RIGHT  1
#define CTRL_ALIGN_TOP    2
#define CTRL_ALIGN_BOTTOM 3
#define CTRL_ALIGN_MAX    4
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set control state
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetState( 
                                MMI_HANDLE_T    ctrl_handle,
                                GUICTRL_STATE_T state,
                                BOOLEAN is_true
                                ) 
{
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {        
        IGUICTRL_SetState(ctrl_ptr, state, is_true);
    }
}

/*****************************************************************************/
//  Description : get control state
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_GetState( 
                                   MMI_HANDLE_T    ctrl_handle,
                                   GUICTRL_STATE_T state
                                   ) 
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        result = IGUICTRL_GetState( ctrl_ptr, state );
    }

    return (result);
}

/*****************************************************************************/
//  Description : set control rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetRect( 
                               MMI_HANDLE_T      ctrl_handle,
                               const GUI_RECT_T* rect_ptr
                               ) 
{
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_SetRect(ctrl_ptr, rect_ptr);
    }
}

/*****************************************************************************/
//  Description : get control rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_GetRect( 
                               MMI_HANDLE_T ctrl_handle,
                               GUI_RECT_T*  rect_ptr
                               ) 
{
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_GetRect(ctrl_ptr, rect_ptr);
    }
}

/*****************************************************************************/
// 	Description : set gui control lcd device
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetLcdDevInfo(
                                     MMI_HANDLE_T           ctrl_handle,    //in:
                                     const GUI_LCD_DEV_INFO *lcd_dev_ptr    //in:
                                     )
{
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_SetLcdDevInfo(ctrl_ptr,lcd_dev_ptr);
    }
}

/*****************************************************************************/
// 	Description : get gui control lcd device
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_GetLcdDevInfo(
                                        MMI_HANDLE_T        ctrl_handle,    //in:
                                        GUI_LCD_DEV_INFO    *lcd_dev_ptr    //in/out:
                                        )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        result = IGUICTRL_GetLcdDevInfo(ctrl_ptr,lcd_dev_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_GetDisplayRect(
                                         MMI_HANDLE_T   ctrl_handle,    //in:
                                         GUI_RECT_T     *rect_ptr       //in/out:
                                         )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        result = IGUICTRL_GetDisplayRect(ctrl_ptr,rect_ptr);
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Set control display rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_SetDisplayRect(
                                         MMI_HANDLE_T       ctrl_handle,//in:
                                         const GUI_RECT_T   *rect_ptr,  //in/out:
                                         BOOLEAN            is_update   //in:
                                         )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_SetDisplayRect(ctrl_ptr,rect_ptr,is_update);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Get control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_GetBothRect(
                                      MMI_HANDLE_T     ctrl_handle,
                                      GUI_BOTH_RECT_T* both_rect_ptr
                                      )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_GetBothRect(ctrl_ptr,both_rect_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Get control both rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_HandleEvent(
                                      MMI_HANDLE_T     ctrl_handle,
                                      CAF_MESSAGE_ID_E msg_id, 
                                      void* param_ptr
                                      )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_HandleEvent(ctrl_ptr, msg_id, param_ptr);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : Redraw ctrl [ send MSG_CTL_PAINT to Ctrl]
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_Redraw(
                                 MMI_HANDLE_T     ctrl_handle
                                 )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_HandleEvent(ctrl_ptr, MSG_CTL_PAINT, PNULL);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set control state
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIAPICTRL_SetBorder( 
                                 MMI_HANDLE_T    ctrl_handle,
                                 GUI_BORDER_T *border_ptr
                                 ) 
{
    IGUICTRL_T  *ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);

    if (PNULL != ctrl_ptr)
    {        
        IGUICTRL_SetBorder(ctrl_ptr, border_ptr);
    }
}

/*****************************************************************************/
//  Description : set control both rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_SetBothRect(
                                      MMI_HANDLE_T      ctrl_handle,
                                      GUI_BOTH_RECT_T   *both_rect_ptr
                                      )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = PNULL;
    GUI_RECT_T  ctrl_rect = {0};

    //get control pointer
    ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    if (PNULL != ctrl_ptr)
    {
        IGUICTRL_SetBothRect(ctrl_ptr,both_rect_ptr);

        if (MMK_IsWindowLandscape(MMK_GetWinHandleByCtrl(ctrl_handle)))
        {
            ctrl_rect = both_rect_ptr->h_rect;
        }
        else
        {
            ctrl_rect = both_rect_ptr->v_rect;
        }

        IGUICTRL_SetRect(ctrl_ptr,&ctrl_rect);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : set control background info
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_SetBg(
                                MMI_HANDLE_T    ctrl_handle,
                                GUI_BG_T       *bg_ptr
                                )
{
    BOOLEAN     result = FALSE;
    IGUICTRL_T  *ctrl_ptr = PNULL;

    //get control pointer
    ctrl_ptr = MMK_GetCtrlPtr(ctrl_handle);
    if ((PNULL != ctrl_ptr) && (PNULL != bg_ptr))
    {
        IGUICTRL_SetBg(ctrl_ptr, bg_ptr);

        result = TRUE;
    }

    return (result);
}

#ifdef UI_MULTILAYER_SUPPORT
/*****************************************************************************/
//  Description : play fade anim
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:多个控件隐藏特效
/*****************************************************************************/
LOCAL void PlayFadeAnim(
                        MMI_HANDLE_T win_handle,
                        IGUICTRL_T** ctrl_pptr,
                        uint16       total_num,
                        BOOLEAN      is_hide
                        )
{
    uint32      i         = 0;
    GUI_RECT_T  max_rect  = {0};
    GUI_RECT_T  ctrl_rect = {0};
    UILAYER_CREATE_T    create_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT( 0 != total_num ); /*assert verified*/
    if (0 == total_num)
    {
        return;
    }

    for ( i = 0; i < total_num; i++ )
    {
        IGUICTRL_GetRect( ctrl_pptr[i], &ctrl_rect );
        
        if ( 0 == i )
        {
            max_rect = ctrl_rect;
        }
        else
        {
            max_rect.left = MIN( max_rect.left, ctrl_rect.left );
            max_rect.top  = MIN( max_rect.top, ctrl_rect.top );
            max_rect.right = MAX( max_rect.right, ctrl_rect.right );
            max_rect.bottom = MAX( max_rect.bottom, ctrl_rect.bottom );
        }
    }
    
    if ( !GUI_IsInvalidRect( max_rect ) )
    {
        GUI_LCD_DEV_INFO     layer = {0};
        UILAYER_APPEND_BLT_T append_layer = {0};
        int16                step = 0;
        int16                alpha = 0;
        MMITHEME_HIDE_ANIM_T anim = {0};
        
        MMITHEME_GetHideAnim( &anim );
        
        // 创建层
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_handle;
        create_info.offset_x = max_rect.left;
        create_info.offset_y = max_rect.top;
        create_info.width = (max_rect.right + 1 - max_rect.left);
        create_info.height = (max_rect.bottom + 1 - max_rect.top);
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(
            &create_info,  
            &layer
            );
        
        // 设置color key
        UILAYER_SetLayerColorKey( &layer, TRUE, UILAYER_TRANSPARENT_COLOR );
        
        //fill color key
        UILAYER_Clear( &layer );
        
        append_layer.lcd_dev_info = layer;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
        
        //set new layer
        for ( i = 0; i < total_num; i++ )
        {
            IGUICTRL_SetLcdDevInfo( ctrl_pptr[i], &layer );
        }
        
        MMK_SendMsg( win_handle, MSG_FULL_PAINT, PNULL );

        step = 0xFF/anim.frame;
        
        if ( is_hide )
        {
            step = -step;
            alpha = 0xFF;
        }
        
        for ( i = 0; i < anim.frame ; i++ )
        {    
            alpha += step;
            
            UILAYER_SetLayerAlpha( &layer, (uint8)alpha );
            
            UILAYER_SetLayerPosition(&layer, max_rect.left, max_rect.top );
            
            MMITHEME_UpdateRect();
            
            SCI_Sleep(10);
        }
        
        // 释放层
        UILAYER_RELEASELAYER(&layer);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer( &layer );
    }
}

/*****************************************************************************/
//  Description : get ctrl align
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:根据控件区域获得控件的对齐方式
/*****************************************************************************/
LOCAL uint8 GetCtrlAlign(
                         GUI_RECT_T   win_rect,
                         GUI_RECT_T   rect
                         )
{
    uint8 align = CTRL_ALIGN_LEFT;
    uint16 left_space  = abs( rect.left - win_rect.left ) + abs( rect.right - win_rect.left );
    uint16 right_space = abs( rect.left - win_rect.right ) + abs( rect.right - win_rect.right );
    uint16 top_space   = abs( rect.top - win_rect.top ) + abs( rect.bottom - win_rect.top );
    uint16 bottom_space = abs( rect.top - win_rect.bottom ) + abs( rect.bottom - win_rect.bottom );
    uint16 h_min = 0;
    uint16 v_min = 0;
    GUI_ALIGN_E h_align = CTRL_ALIGN_LEFT;
    GUI_ALIGN_E v_align = CTRL_ALIGN_TOP;
    
    //根据rect和四周的差值判断rect属于哪个方向
    if ( left_space < right_space )
    {
        h_min = left_space;
    }
    else
    {
        h_align = CTRL_ALIGN_RIGHT;
        h_min = right_space;
    }
    
    if ( top_space < bottom_space )
    {
        v_min = top_space;
    }
    else
    {
        v_align = CTRL_ALIGN_BOTTOM;
        v_min = bottom_space;
    }
    
    if ( h_min < v_min )
    {
        align = h_align;
    }
    else
    {
        align = v_align;
    }
    
    return align;
}

/*****************************************************************************/
//  Description : play center slide anim
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:多个控件中间滑动
/*****************************************************************************/
LOCAL void PlayCenterSlideAnim(
                               MMI_HANDLE_T win_handle,
                               IGUICTRL_T** ctrl_pptr,
                               uint16       total_num,
                               BOOLEAN      is_hide
                               )
{
    uint32      i = 0;
    uint32      j = 0;
    GUI_RECT_T  rect_array[CTRL_ALIGN_MAX] = {0};
    GUI_RECT_T  ctrl_rect   = {0};
    GUI_RECT_T   win_rect   = {0};
    UILAYER_CREATE_T    create_info = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT( 0 != total_num ); /*assert verified*/
    if (0 == total_num)
    {
        return;
    }

    MMK_GetWinRect( win_handle, &win_rect );

    for ( i = 0; i < total_num; i++ )
    {
        uint8 align = CTRL_ALIGN_LEFT;
        
        IGUICTRL_GetRect( ctrl_pptr[i], &ctrl_rect );
        
        align = GetCtrlAlign( win_rect, ctrl_rect );
        
        if ( GUI_IsInvalidRect( rect_array[align] ) )
        {
            rect_array[align] = ctrl_rect;
        }
        else
        {
            rect_array[align].left = MIN( rect_array[align].left, ctrl_rect.left );
            rect_array[align].top  = MIN( rect_array[align].top, ctrl_rect.top );
            rect_array[align].right = MAX( rect_array[align].right, ctrl_rect.right );
            rect_array[align].bottom = MAX( rect_array[align].bottom, ctrl_rect.bottom );
        }
    }
    
    if ( !GUI_IsInvalidRect( rect_array[CTRL_ALIGN_LEFT] )
        || !GUI_IsInvalidRect( rect_array[CTRL_ALIGN_RIGHT] )
        || !GUI_IsInvalidRect( rect_array[CTRL_ALIGN_TOP] )
        || !GUI_IsInvalidRect( rect_array[CTRL_ALIGN_BOTTOM] ))
    {
        UILAYER_APPEND_BLT_T append_layer = {0};
        GUI_LCD_DEV_INFO  layer_array[CTRL_ALIGN_MAX] = {0};
        int16             start_x_array[CTRL_ALIGN_MAX]  = {0};
        int16             start_y_array[CTRL_ALIGN_MAX]  = {0};
        int16             h_step_array[CTRL_ALIGN_MAX]  = {0};
        int16             v_step_array[CTRL_ALIGN_MAX]  = {0};
        int16             space = 0;
        MMITHEME_HIDE_ANIM_T anim = {0};

        MMITHEME_GetHideAnim( &anim );

        for ( i = 0; i < CTRL_ALIGN_MAX; i++ )
        {
            if ( !GUI_IsInvalidRect( rect_array[i] ) )
            {
                if ( i < CTRL_ALIGN_TOP )
                {
                    space = rect_array[i].right - rect_array[i].left + 1;
                    h_step_array[i] = space/anim.frame;

                    if ( CTRL_ALIGN_LEFT == i )
                    {
                        if ( is_hide )
                        {
                            start_x_array[i] = rect_array[i].left;
                            h_step_array[i] = -h_step_array[i];
                        }
                        else
                        {
                            start_x_array[i] = rect_array[i].left - space + 1;
                        }
                    }
                    else
                    {
                        if ( is_hide )
                        {
                            start_x_array[i] = rect_array[i].left;
                        }
                        else
                        {
                            start_x_array[i] = rect_array[i].right;
                            h_step_array[i] = -h_step_array[i];
                        }
                    }
                }
                else
                {
                    space = rect_array[i].bottom - rect_array[i].top + 1;
                    v_step_array[i] = space/anim.frame;

                    if ( CTRL_ALIGN_TOP == i )
                    {
                        if ( is_hide )
                        {
                            start_y_array[i] = rect_array[i].top;
                            v_step_array[i] = -v_step_array[i];
                        }
                        else
                        {
                            start_y_array[i] = rect_array[i].top - space + 1;
                        }
                    }
                    else
                    {
                        if ( is_hide )
                        {
                            start_y_array[i] = rect_array[i].top;
                        }
                        else
                        {
                            start_y_array[i] = rect_array[i].bottom;
                            v_step_array[i] = -v_step_array[i];
                        }
                    }
                }
                
                // 创建层
                create_info.lcd_id = GUI_MAIN_LCD_ID;
                create_info.owner_handle = win_handle;
                create_info.offset_x = rect_array[i].left;
                create_info.offset_y = rect_array[i].top;
                create_info.width = (rect_array[i].right + 1 - rect_array[i].left);
                create_info.height = (rect_array[i].bottom + 1 - rect_array[i].top);
                create_info.is_bg_layer = FALSE;
                create_info.is_static_layer = FALSE; 
                UILAYER_CreateLayer(
                    &create_info,
                    &layer_array[i]
                    );
                
                // 设置color key
                UILAYER_SetLayerColorKey( &layer_array[i], TRUE, UILAYER_TRANSPARENT_COLOR );
                
                //fill color key
                UILAYER_Clear( &layer_array[i] );
            }
            else
            {
                layer_array[i].block_id = UILAYER_NULL_HANDLE;
            }
        }

        append_layer.lcd_dev_info = layer_array[CTRL_ALIGN_LEFT];
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
        append_layer.lcd_dev_info = layer_array[CTRL_ALIGN_RIGHT];
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
        append_layer.lcd_dev_info = layer_array[CTRL_ALIGN_TOP];
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
        append_layer.lcd_dev_info = layer_array[CTRL_ALIGN_BOTTOM];
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
        
        //数据保存到四个层上
        for ( j = 0; j < CTRL_ALIGN_MAX; j++ )
        {
            if ( UILAYER_NULL_HANDLE != layer_array[j].block_id )
            {
                //set new layer
                for ( i = 0; i < total_num; i++ )
                {
                    IGUICTRL_SetLcdDevInfo( ctrl_pptr[i], &layer_array[j] );
                }
                
                MMK_SendMsg( win_handle, MSG_FULL_PAINT, PNULL );
            }
        }

        for ( i = 0; i < anim.frame ; i++ )
        {
            for ( j = 0; j < CTRL_ALIGN_MAX; j++ )
            {
                start_x_array[j] += h_step_array[j];
                start_y_array[j] += v_step_array[j];
                if ( UILAYER_NULL_HANDLE != layer_array[j].block_id )
                {
                    UILAYER_SetLayerPosition(&layer_array[j], start_x_array[j], start_y_array[j] );
                }
            }

            MMITHEME_UpdateRect();
            
            SCI_Sleep(10);
        }
        
        for ( i = 0; i < CTRL_ALIGN_MAX; i++ )
        {
            // 释放层
            UILAYER_RELEASELAYER(layer_array + i);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer( layer_array + i );
        }
    }
}

/*****************************************************************************/
//  Description : set controls hide
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:多个控件隐藏特效
/*****************************************************************************/
PUBLIC BOOLEAN GUIAPICTRL_CtrlsHidle(
                                     MMI_HANDLE_T    win_handle,
                                     MMI_HANDLE_T*   ctrl_handle_ptr,
                                     uint16          ctrl_num,
                                     BOOLEAN         is_hide
                                     )
{
    BOOLEAN result = FALSE;
    
    if ( ctrl_num > 0 
        && PNULL != ctrl_handle_ptr )
    {
        uint32       i          = 0;
        IGUICTRL_T*  ctrl_ptr   = PNULL;
        uint16       total_num  = 0;
        IGUICTRL_T** ctrl_pptr  = SCI_ALLOC_APP( sizeof(IGUICTRL_T*) *ctrl_num );

        win_handle = MMK_ConvertIdToHandle( win_handle );
        
        //check ctrl total num
        for ( i = 0; i < ctrl_num; i++ )
        {
            //check valid ctrl ptr
            if ( win_handle == MMK_GetWinHandleByCtrl( ctrl_handle_ptr[i] ) )
            {
                if ( PNULL != ( ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle_ptr[i] ) )
                    && is_hide != IGUICTRL_GetState( ctrl_ptr, GUICTRL_STATE_INVISIBLE ) )
                {
                    ctrl_pptr[total_num] = ctrl_ptr;
                    total_num++;
                }
            }
        }
        
        if ( total_num > 0 )
        {
//            UILAYER_HANDLE_T  ui_layer_arr[2] = {0};
            MMITHEME_HIDE_ANIM_T anim = {0};
            GUI_LCD_DEV_INFO* dev_info_ptr = SCI_ALLOC_APP( sizeof(GUI_LCD_DEV_INFO) * total_num );

            //从隐藏到不隐藏, 需要提前设置为可见
            if ( !is_hide )
            {
                for ( i = 0; i < total_num; i++ )
                {
                    IGUICTRL_SetState( ctrl_pptr[i], GUICTRL_STATE_INVISIBLE, is_hide );
                }
            }

            MMITHEME_GetHideAnim( &anim );

            //get old layer
            for ( i = 0; i < total_num; i++ )
            {
                IGUICTRL_GetLcdDevInfo( ctrl_pptr[i], &dev_info_ptr[i] );
            }
            
            if ( MMITHEME_HIDE_FADE == anim.type )
            {
                PlayFadeAnim( win_handle, ctrl_pptr, total_num, is_hide );
            }
            else if ( MMITHEME_HIDE_CENTER_SILDE == anim.type )
            {
                PlayCenterSlideAnim( win_handle, ctrl_pptr, total_num, is_hide );
            }
            
            //set old layer
            for ( i = 0; i < total_num; i++ )
            {
                IGUICTRL_SetLcdDevInfo( ctrl_pptr[i], &dev_info_ptr[i] );
            }

            //从不隐藏到隐藏, 在最后绘制前要设置为隐藏
            if ( is_hide )
            {
                for ( i = 0; i < total_num; i++ )
                {
                    IGUICTRL_SetState( ctrl_pptr[i], GUICTRL_STATE_INVISIBLE, is_hide );
                }
            }
            
            //最后绘制一次
            MMK_SendMsg( win_handle, MSG_FULL_PAINT, PNULL );

            //恢复旧的层        
//            ui_layer_arr[0] = MMITHEME_GetBgLayer();
//            ui_layer_arr[1] = UILAYER_MAIN_HANDLE;
//            UILAYER_SetBltLayer(GUI_MAIN_LCD_ID, ui_layer_arr, 2);
            /*
            UILAYER_RemoveBltLayer(&layer_array[CTRL_ALIGN_TOP]);
            UILAYER_RemoveBltLayer(&layer_array[CTRL_ALIGN_LEFT]);
            UILAYER_RemoveBltLayer(&layer_array[CTRL_ALIGN_RIGHT]);
            UILAYER_RemoveBltLayer(&layer_array[CTRL_ALIGN_BOTTOM]);
            */

            SCI_FREE( dev_info_ptr );
        }
        else
        {
            MMK_SendMsg( win_handle, MSG_FULL_PAINT, PNULL );
        }
        
        SCI_FREE( ctrl_pptr );

        result = TRUE;
    }
    
    return result;
}
#endif //end of UI_MULTILAYER_SUPPORT
#endif
