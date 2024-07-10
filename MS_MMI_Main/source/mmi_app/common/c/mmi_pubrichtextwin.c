#if 0
/*****************************************************************************
** File Name:      mmi_pubrichtextwin.c                                      *
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

#define _MMI_PUBRICHTEXTWIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_common_trc.h"
#include "mmk_type.h"
#include "mmipub.h"
//#include "mmi_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "guiblock.h"
//#include "guilabel.h"
#include "mmi_theme.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "guianim.h"

//#include "mmicc_image.h"
//#include "cafapplet.h"
#include "mmi_resource.h"
#include "mmi_appmsg.h"
//#include "guires.h"
#include "guirichtext.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//pub richtext win infomation
typedef struct
{
    MMIPUB_TYPE_E           type;
    PROCESSMSG_FUNC         user_func;                  //用户传入的回调
    MMI_HANDLE_T            ctrl_handle;                //richtext handle
    MMI_HANDLE_T            dst_handle;                 //dst win handle
} MMIPUB_RICHTEXT_INFO_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle default window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理默认的消息, 如close window, 防止应用未调用接口导致内存泄漏
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubRichTextWinMsg(
                                           MMI_WIN_ID_T      win_id, 
                                           MMI_MESSAGE_ID_E  msg_id, 
                                           DPARAM            param
                                           );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle default window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理默认的消息, 如close window, 防止应用未调用接口导致内存泄漏
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubRichTextWinMsg(
                                           MMI_WIN_ID_T      win_id, 
                                           MMI_MESSAGE_ID_E  msg_id, 
                                           DPARAM            param
                                           )
{
    MMI_RESULT_E             result       = MMI_RESULT_FALSE;
    MMIPUB_RICHTEXT_INFO_T*  win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMIPUB_RICHTEXT_NOTIFY_T notify       = {0};

    if ( PNULL == win_info_ptr || MMIPUB_RICHTEXT != win_info_ptr->type )
    {
        return MMI_RESULT_FALSE;
    }

    //先执行用户的回调
    if ( PNULL != win_info_ptr->user_func )
    {
        result = win_info_ptr->user_func( win_id, msg_id, param );
    }

    if ( MMI_RESULT_FALSE == result )
    {
        notify.notify.src_handle = win_info_ptr->ctrl_handle;
        notify.notify.src_id     = MMK_GetCtrlId( win_info_ptr->ctrl_handle );
        notify.notify.code       = msg_id;

        switch( msg_id )
        {
        case MSG_OPEN_WINDOW:
            MMK_SendMsg( win_info_ptr->dst_handle, MSG_PUBRICHTEXT_OPEN, &notify );
            MMK_SetActiveCtrl( win_info_ptr->ctrl_handle, FALSE );
            break;
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            MMK_SendMsg( win_info_ptr->dst_handle, MSG_PUBRICHTEXT_OK, &notify );
            break;
            
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_SendMsg( win_info_ptr->dst_handle, MSG_PUBRICHTEXT_CANCEL, &notify );
            MMK_CloseWin( win_id );
            break;
            
        default:
            break;
        }
    }

    //防止用户未调用接口导致内存泄漏
    //必须要呼叫的函数都写在此处, 防止用户遗忘调用
    if ( MSG_CLOSE_WINDOW == msg_id )
    {
        MMK_FreeWinAddDataEx( win_id );
    }

    return result;
}

/*****************************************************************************/
//  Description : open richtext win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_OpenRichTextWin(
                                      MMIPUB_RICHTEXT_PARAM_T* param
                                      )
{
    BOOLEAN result = FALSE;

    if ( PNULL != param )
    {    
        MMI_WIN_ID_T        win_id        = param->win_id;
        MMI_WIN_ID_T        text_ctrl_id  = param->ctrl_id;
        MMI_HANDLE_T        applet_handle = ( 0 == param->applet_handle ) ? MMK_GetFirstAppletHandle() : param->applet_handle;
        MMI_HANDLE_T        win_handle    = 0;
        MMIPUB_RICHTEXT_INFO_T* win_info_ptr  = PNULL;
        MMI_WINDOW_CREATE_T win_create    = {0};
        GUI_RECT_T          text_rect     = MMITHEME_GetClientRect();
        
        //judge window is already open
        if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
        {            
            //set window parameter
            win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_RICHTEXT_INFO_T));
            SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_RICHTEXT_INFO_T));
            
            //set user data
            win_info_ptr->type = MMIPUB_RICHTEXT;
            win_info_ptr->user_func = param->user_func;
            
            win_create.applet_handle = applet_handle;
            //win_create.parent_win_handle = param->parent_win_handle;
            win_create.win_id = win_id;
            win_create.func = HandlePubRichTextWinMsg;
            win_create.win_priority = WIN_ONE_LEVEL;
            win_create.add_data_ptr = param->add_data_ptr;
            win_create.add_data_ex_ptr = win_info_ptr;

            if( 0 != ( win_handle = MMK_CreateWindow( &win_create ) ) )
            {
                IGUICTRL_T*             text_ptr     = PNULL;
                GUIRICHTEXT_INIT_DATA_T text_data    = {0};
                GUISOFTKEY_INIT_DATA_T  softkey_data = {0};
                GUITITLE_INIT_DATA_T    title_data   = {0};
                MMI_CONTROL_CREATE_T    create       = {0};
                
                title_data.text_id = param->title_id;
                
                create.ctrl_id = MMITHEME_GetTitleCtrlId();
                create.guid    = SPRD_GUI_TITLE_ID;
                create.parent_win_handle = win_handle;
                create.init_data_ptr = &title_data;
                MMK_CreateControl( &create );
                MMK_SetWinTitleCtrlId( win_handle, create.ctrl_id );

#ifndef MMIPUBWIN_PDASTYLE_SUPPORT	//pda大多数场景不显示softkey
                if ( param->is_user_skb )
                {
                    softkey_data.leftsoft_id  = param->leftsoft_id;
                    softkey_data.midsoft_id	  = param->midsoft_id;
                    softkey_data.rightsoft_id = param->rightsoft_id;
                }
                else
                {
                    softkey_data.leftsoft_id  = TXT_COMMON_OK;
                    softkey_data.midsoft_id	  = TXT_NULL;
                    softkey_data.rightsoft_id = STXT_RETURN;
                }

                SCI_MEMSET( &create, 0, sizeof(MMI_CONTROL_CREATE_T) );
                create.ctrl_id = MMITHEME_GetSoftkeyCtrlId();
                create.guid    = SPRD_GUI_SOFTKEY_ID;
                create.parent_win_handle = win_handle;
                create.init_data_ptr = &softkey_data;
                MMK_CreateControl( &create );
                MMK_SetWinSoftkeyCtrlId( win_handle, create.ctrl_id );
#endif

                text_data.both_rect.v_rect = (param->is_user_rect) ? param->text_rect : text_rect;
                text_data.both_rect.h_rect = text_data.both_rect.v_rect;

                SCI_MEMSET( &create, 0, sizeof(MMI_CONTROL_CREATE_T) );
                create.ctrl_id           = text_ctrl_id;
                create.guid              = SPRD_GUI_RICHTEXT_ID;
                create.parent_win_handle = win_handle;
                create.init_data_ptr     = &text_data;
                text_ptr = MMK_CreateControl( &create );

                win_info_ptr->ctrl_handle = IGUICTRL_GetCtrlHandle( text_ptr );
                win_info_ptr->dst_handle  = ( 0 == param->dst_handle ) ? win_handle : param->dst_handle;

                result = TRUE;
            }
            else
            {
                SCI_FREE( win_info_ptr );
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIPUB_OpenRichTextWin:win_id %d is already open!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBRICHTEXTWIN_243_112_2_18_1_58_38_51,(uint8*)"d", win_id);
        }
    }

    return result;
}
#endif




/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
