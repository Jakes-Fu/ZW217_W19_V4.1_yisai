#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************
** File Name:      mmi_publistwin.c                                          *
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

#define _MMI_PUBLISTWIN_C_

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/

#include "mmipub.h"
#include "mmipub_internal.h"
#include "mmi_id.h"
#include "mmi_image.h"

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
//  Description : handle list window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    );

/*****************************************************************************/
//  Description : pub list win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubListWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          );

/*****************************************************************************/
//  Description : set list rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetListRect(
                       MMIPUB_INFO_T *win_info_ptr
                       );

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/
const MMIPUB_INIT_T g_publistwin_init = 
{
    HandleListWinMsg,
    PubListWinInit,
    MMIPUBWIN_LIST,
    MMIPUB_LIST_WIN_ID,
};

/*****************************************************************************/
//  Description : handle list window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleListWinMsg(
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
    if ( MSG_CLOSE_WINDOW == msg_id )
    {
        MMIPUB_DestroyWinData( win_info_ptr );
    }
    else if ( MSG_LCD_SWITCH == msg_id )
    {
        MMIPUB_HandleLcdSwitch( win_info_ptr );
    }
    else if ( MSG_FULL_PAINT == msg_id )
    {
        if ( !win_info_ptr->is_layout )
        {
            win_info_ptr->line_num = GUILIST_GetTotalItemNum( win_info_ptr->ctrl_handle );
            
            MMIPUB_ReleaseLayer( win_info_ptr );
            
            MMIPUBLayout( win_info_ptr );
            SetListRect( win_info_ptr );
            MMIPUBSetButton( win_info_ptr, FALSE );
        }

        MMIPUBDisplayWin( win_info_ptr );
    }
    else if ( MSG_NOTIFY_UPDATE == msg_id )
    {
        MMIPUBClearClientRectBg( win_info_ptr, param );
    }

    return (result);
}

/*****************************************************************************/
//  Description : set list rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetListRect(
                       MMIPUB_INFO_T *win_info_ptr
                       )
{
    GUILIST_SetRect( win_info_ptr->ctrl_handle, &win_info_ptr->client_rect );
}

/*****************************************************************************/
//  Description : pub list win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubListWinInit(
                          MMIPUB_INFO_T*  win_info_ptr,
                          MMIPUB_PARAM_T* param
                          )
{
    if ( PNULL != win_info_ptr
        && PNULL != param )
    {
        GUILIST_INIT_DATA_T  init_data   = {0};
        MMI_CONTROL_CREATE_T create      = {0};
        MMI_CTRL_ID_T        ctrl_id     = ( 0 == param->ctrl_id ) ? MMIPUB_LIST_CTRL_ID : param->ctrl_id;

        init_data.type           = param->ctrl_param.list_param.list_type;
        create.ctrl_id           = param->ctrl_id;
        create.guid              = param->guid;
        create.parent_win_handle = win_info_ptr->win_handle;
        create.init_data_ptr     = &init_data;
        
        MMK_CreateControl( &create );
        
        win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, ctrl_id );
        
        if( 0 != win_info_ptr->ctrl_handle )
        {
            GUILIST_SetTextFont( win_info_ptr->ctrl_handle, win_info_ptr->pubwin_theme.normal_font, win_info_ptr->pubwin_theme.layout.button_font.color );
            GUILIST_SetBgColor( win_info_ptr->ctrl_handle, win_info_ptr->pubwin_theme.layout.ctrl_bg_color );
#ifdef MMI_PDA_SUPPORT
            GUILIST_SetCycScroll(win_info_ptr->ctrl_handle, FALSE);
#endif
        }
    }
}

#endif


