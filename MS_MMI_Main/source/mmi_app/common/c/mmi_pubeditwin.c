#if 0//def MMI_PDA_SUPPORT
/*****************************************************************************
** File Name:      mmi_pubeditwin.c                                          *
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

#define _MMI_PUBEDITWIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipub.h"
#include "mmipub_internal.h"
#include "mmi_id.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
/*****************************************************************************/
//  Description : handle edit window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    );

/*****************************************************************************/
//  Description : pub edit win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubEditWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          );

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetEditRect(
                       MMIPUB_INFO_T *win_info_ptr
                       );

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
const MMIPUB_INIT_T g_pubeditwin_init = 
{
    HandleEditWinMsg,
    PubEditWinInit,
    MMIPUBWIN_EDIT,
    MMIPUB_EDIT_WIN_ID
};

/*****************************************************************************/
//  Description : handle edit window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditWinMsg(
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

    result = win_info_ptr->user_func( win_id, msg_id, param );
    //用户开自定义的timer里关窗口就会跑到这个case
    //如果不return下面处理waitwin timer的case里就会写野指针!
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return result;
    }

    //防止用户未调用接口导致内存泄漏
    //必须要呼叫的函数都写在此处, 防止用户遗忘调用
    if ( MSG_CLOSE_WINDOW == msg_id )
    {
        MMIPUB_DestroyWinData( win_info_ptr );
    }
    else if ( MSG_LCD_SWITCH == msg_id )
    {
        win_info_ptr->is_layout = FALSE;
    }
    else if ( MSG_NOTIFY_RESIZE == msg_id )
    {
        win_info_ptr->is_layout = FALSE;
    }
    else if ( MSG_FULL_PAINT == msg_id )
    {
        if ( !win_info_ptr->is_layout )
        {
            win_info_ptr->line_num = 1;
            
            MMIPUB_ReleaseLayer( win_info_ptr );
            
            MMIPUBLayout( win_info_ptr );
            SetEditRect( win_info_ptr );
            MMIPUBSetButton( win_info_ptr, FALSE );
        }

        MMIPUBDisplayWin( win_info_ptr );
    }

    return (result);
}

/*****************************************************************************/
//  Description : set edit rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetEditRect(
                       MMIPUB_INFO_T *win_info_ptr
                       )
{
    GUIEDIT_SetRect( win_info_ptr->ctrl_handle, &win_info_ptr->client_rect );
}

/*****************************************************************************/
//  Description : pub edit win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubEditWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          )
{
    if ( PNULL != win_info_ptr
        && PNULL != param )
    {
        GUIEDIT_INIT_DATA_T  init_data   = {0};
        MMI_CONTROL_CREATE_T create      = {0};
        MMI_CTRL_ID_T        ctrl_id     = ( 0 == param->ctrl_id ) ? MMIPUB_EDIT_CTRL_ID : param->ctrl_id;

        init_data.type           = param->ctrl_param.edit_param.edit_type;
        init_data.str_max_len    = param->ctrl_param.edit_param.max_len;
        init_data.add_data       = param->ctrl_param.edit_param.add_data;
        create.ctrl_id           = param->ctrl_id;
        create.guid              = param->guid;
        create.parent_win_handle = win_info_ptr->win_handle;
        create.init_data_ptr     = &init_data;
        
        MMK_CreateControl( &create );
        
        win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, ctrl_id );
        
        if( 0 != win_info_ptr->ctrl_handle )
        {
        }
    }
}


#endif


