#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************
** File Name:      mmi_pubformwin.c                                          *
** Author:                                                                   *
** Date:           01/13/2009                                                *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2009      James.Zhang           Creat                                  *
******************************************************************************/

#define _MMI_PUBFORMWIN_C_

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/

#include "mmipub.h"
#include "mmipub_internal.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "guiform.h"
//#include "guires.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle form window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    );

/*****************************************************************************/
//  Description : pub form win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubFormWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          );

/*****************************************************************************/
//  Description : set form rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetFormRect(
                       MMIPUB_INFO_T *win_info_ptr
                       );

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
const MMIPUB_INIT_T g_pubformwin_init = 
{
    HandleFormWinMsg,
    PubFormWinInit,
    MMIPUBWIN_FORM,
    MMIPUB_FORM_WIN_ID
};


/*****************************************************************************/
//  Description : handle form window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFormWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    if ( PNULL == win_info_ptr || PNULL == win_info_ptr->user_func )
    {
        return MMI_RESULT_FALSE;
    }

    //转义消息
    if ( MMI_RESULT_TRUE == ( result = MMIPUBHandleButtonMsg( win_info_ptr, msg_id, param ) ) )
    {
        return result;
    }

    // 为防止在MMIPUBHandleButtonMsg里销毁了window，win_info_ptr变成野指针
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return result;
    }

    result = win_info_ptr->user_func( win_id, msg_id, param );
    //用户开自定义的timer里关窗口就会跑到这个case
    //如果不return下面处理waitwin timer的case里就会写野指针!
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return result;
    }

    //防止用户未调用接口导致内存泄漏
    //必须要呼叫的函数都写在此处, 防止用户遗忘调用
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set form border style, to hide the border
        {
            GUI_BORDER_T border = {0};

            border.type = GUI_BORDER_NONE;
            GUIFORM_SetActiveChildTheme(win_info_ptr->ctrl_handle, PNULL, &border, PNULL);
        }
        break;

    case MSG_CLOSE_WINDOW:
        MMIPUB_DestroyWinData( win_info_ptr );
        break;
        
    case MSG_LCD_SWITCH:
        MMIPUB_HandleLcdSwitch( win_info_ptr );

#ifndef MMI_MENUMASKLAYER_SUPPORT
        // fix NEWMS00193063
        // 修复转屏后背景变黑的问题
        if (MMK_IsFocusWin(win_info_ptr->win_handle))
        {
            MMK_UpdateScreen();
        }
#endif
        break;

    case MSG_NOTIFY_RESIZE:
    case MSG_NOTIFY_LINE_CHANGE:
        win_info_ptr->is_layout = FALSE;
        if (MMK_IsFocusWin(win_info_ptr->win_handle))
        {
            if ( win_info_ptr->pubwin_theme.is_support_anim )
            {
                MMK_SendMsg( win_info_ptr->win_handle, MSG_FULL_PAINT, PNULL );
            }
            else
            {
                MMK_UpdateScreen();
            }
        }
        break;
        
    case MSG_FULL_PAINT:
        if ( !win_info_ptr->is_layout )
        {
            GUI_BG_T bg = {0};
            
            bg.bg_type = GUI_BG_NONE;
            GUIFORM_SetBg(win_info_ptr->ctrl_handle,&bg);

            win_info_ptr->line_num = GUIFORM_GetHeight( win_info_ptr->ctrl_handle );
            
            MMIPUB_ReleaseLayer( win_info_ptr );
            
            MMIPUBLayout( win_info_ptr );
            SetFormRect( win_info_ptr );
            MMIPUBSetButton( win_info_ptr, FALSE );
        }

        MMIPUBDisplayWin( win_info_ptr );
        break;
        
    case MSG_NOTIFY_UPDATE:
        MMIPUBClearClientRectBg( win_info_ptr, param );
        break;

    default:
        break;
    }

    return (result);
}


/*****************************************************************************/
//  Description : set form rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetFormRect(
                       MMIPUB_INFO_T *win_info_ptr
                       )
{
    GUIFORM_SetRect( win_info_ptr->ctrl_handle, &win_info_ptr->client_rect );
}

/*****************************************************************************/
//  Description : pub form win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubFormWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          )
{
    GUI_BG_T    form_child_bg = {0};

    if ( PNULL != win_info_ptr
        && PNULL != param )
    {
        form_child_bg.bg_type = GUI_BG_COLOR;
        form_child_bg.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PUBWIN_BG);
        form_child_bg.shape = GUI_SHAPE_RECT;

        //创建edit
        if ( SPRD_GUI_EDITBOX_ID == param->guid )
        {
            GUIFORM_INIT_DATA_T  init_data    = {0};
            MMI_CONTROL_CREATE_T create       = {0};
            GUI_BOTH_RECT_T      full_rect    = MMITHEME_GetFullScreenBothRect();

            full_rect.v_rect.left  += win_info_ptr->pubwin_theme.layout.client_h_margin;
            full_rect.v_rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;
            full_rect.h_rect.left  += win_info_ptr->pubwin_theme.layout.client_h_margin;
            full_rect.h_rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;

            init_data.both_rect      = full_rect;
            init_data.layout_type    = GUIFORM_LAYOUT_ORDER;
            create.ctrl_id           = MMIPUB_FORM_CTRL_ID;
            create.guid              = SPRD_GUI_FORM_ID;
            create.parent_win_handle = win_info_ptr->win_handle;
            create.init_data_ptr     = &init_data;
            
            MMK_CreateControl( &create );
            
            win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_FORM_CTRL_ID );

            //GUIFORM_SetType( win_info_ptr->ctrl_handle, GUIFORM_TYPE_TP );
            
            if( 0 != win_info_ptr->ctrl_handle )
            {
                GUILABEL_INIT_DATA_T label_init = {0};
                GUIEDIT_INIT_DATA_T  edit_init = {0};
                GUIFORM_DYNA_CHILD_T child_init = {0};
                MMITHEME_FORM_T      form_theme = {0};

                if ( 0 != param->ctrl_param.edit_param.label_id )
                {
                    label_init.align = param->ctrl_param.edit_param.label_align;

                    //get param
                    child_init.child_handle  = param->ctrl_param.edit_param.label_id;
                    child_init.guid          = SPRD_GUI_LABEL_ID;
                    child_init.init_data_ptr = (void*)&label_init;
                    
                    GUIFORM_CreatDynaChildCtrl( win_info_ptr->win_handle, win_info_ptr->ctrl_handle, &child_init );
     
                    MMITHEME_GetFormTheme( &form_theme );

                    form_theme.child.font.color = win_info_ptr->pubwin_theme.font_color;
                    //form_theme.child.font.font  = win_info_ptr->pubwin_theme.normal_font;

                    GUIFORM_SetChildTheme(
                            win_info_ptr->ctrl_handle,
                            &form_child_bg, PNULL, &form_theme.child.font );

//                    GUIFORM_SetActiveChildTheme(
//                            win_info_ptr->ctrl_handle,
//                            &form_child_bg, PNULL, PNULL);

                    GUILABEL_SetFont(param->ctrl_param.edit_param.label_id,win_info_ptr->pubwin_theme.small_font,win_info_ptr->pubwin_theme.font_color);
                }

                edit_init.is_rect_adaptive = TRUE;
                edit_init.type        = param->ctrl_param.edit_param.edit_type;
                edit_init.str_max_len = param->ctrl_param.edit_param.max_len;
                edit_init.add_data    = param->ctrl_param.edit_param.add_data;

                //get param
                child_init.is_get_active = TRUE;
                child_init.child_handle  = param->ctrl_id;
                child_init.guid          = SPRD_GUI_EDITBOX_ID;
                child_init.init_data_ptr = &edit_init;
                
                GUIFORM_CreatDynaChildCtrl( win_info_ptr->win_handle, win_info_ptr->ctrl_handle, &child_init );
                
                GUIEDIT_SetMargin(param->ctrl_id,5,5);
            }
        }
        else
        {
            GUIFORM_INIT_DATA_T  init_data   = {0};
            MMI_CONTROL_CREATE_T create      = {0};
            MMI_CTRL_ID_T        ctrl_id     = ( 0 == param->ctrl_id ) ? MMIPUB_FORM_CTRL_ID : param->ctrl_id;
            
            init_data.layout_type    = param->ctrl_param.form_param.layout_type;
            create.ctrl_id           = param->ctrl_id;
            create.guid              = param->guid;
            create.parent_win_handle = win_info_ptr->win_handle;
            create.init_data_ptr     = &init_data;
            
            MMK_CreateControl( &create );
            
            win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, ctrl_id );

            //GUIFORM_SetType( win_info_ptr->ctrl_handle, GUIFORM_TYPE_TP );

            if (SPRD_GUI_FORM_ID == param->guid)
            {
                GUIFORM_SetActiveChildTheme(
                        win_info_ptr->ctrl_handle,
                        &form_child_bg, PNULL, PNULL);

                GUIFORM_SetChildTheme(
                        win_info_ptr->ctrl_handle,
                        &form_child_bg, PNULL, PNULL);
            }
            
            if( 0 != win_info_ptr->ctrl_handle )
            {
            }
        }
    }
}

#endif
