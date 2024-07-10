/*****************************************************************************
** File Name:      mmi_pubmenuwin.c                                          *
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

#define _MMI_PUBMENUWIN_C_

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/

#include "mmi_pubmenuwin.h"
#include "mmi_id.h"
#include "guimenu.h"
#include "mmi_menutable.h"
#include "guiwin.h"
/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
typedef struct
{
    MMI_HANDLE_T  win_handle;
    MMI_HANDLE_T  ctrl_handle;
    MMI_HANDLE_T  dst_handle;
    MMIPUB_MENU_FUNC_T func;
    
} MMIPUB_MENU_T;

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
//  Description : handle menu window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    );

/*****************************************************************************/
//  Description : pub menu win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubMenuWinInit(
                          MMIPUB_MENU_T*  win_info_ptr,
                          MMIPUB_MENU_PARAM_T* param
                          );

/*****************************************************************************/
//  Description : pub menu win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void InitNotify(
                      MMIPUB_MENU_T*  win_info_ptr,
                      MMIPUB_MENU_NOTIFY_T* notify
                      )
                      
{
    notify->ctrl_handle = win_info_ptr->ctrl_handle;
    notify->ctrl_id     = MMK_GetCtrlId( notify->ctrl_handle );
    notify->win_handle  = win_info_ptr->win_handle;
    notify->win_id      = MMK_GetWinId( notify->win_handle );
    notify->dst_handle  = win_info_ptr->dst_handle;
}

/*****************************************************************************/
//  Description : handle menu window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void OnPubMenuOk(
                       MMI_WIN_ID_T      win_id
                       )
{
    MMIPUB_MENU_T   *win_info_ptr = (MMIPUB_MENU_T*)MMK_GetWinAddDataEx( win_id );
    MMIPUB_MENU_NOTIFY_T notify = {0};
    
    if ( PNULL == win_info_ptr )
    {
        return;
    }
    if ( PNULL != win_info_ptr->func.OnMenuOk )
    {
        InitNotify( win_info_ptr, &notify );
        GUIMENU_GetId( notify.ctrl_handle, &notify.group_id, &notify.menu_id );
        win_info_ptr->func.OnMenuOk( &notify );
    }
    MMK_CloseWin(win_id);  
}

/*****************************************************************************/
//  Description : handle menu window message
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void OnPubMenuCancel(
                           MMI_WIN_ID_T      win_id
                           )
{
    MMIPUB_MENU_T   *win_info_ptr = (MMIPUB_MENU_T*)MMK_GetWinAddDataEx( win_id );
    MMIPUB_MENU_NOTIFY_T notify = {0};
    
    if ( PNULL == win_info_ptr )
    {
        return;
    }
    if ( PNULL != win_info_ptr->func.OnMenuCancel )
    {
        InitNotify( win_info_ptr, &notify );
        win_info_ptr->func.OnMenuCancel( &notify );
    }
    MMK_CloseWin(win_id); 
}

/*****************************************************************************/
//  Description : handle menu window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMenuWinMsg(
                                    MMI_WIN_ID_T      win_id, 
                                    MMI_MESSAGE_ID_E  msg_id, 
                                    DPARAM            param
                                    )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMIPUB_MENU_T   *win_info_ptr = (MMIPUB_MENU_T*)MMK_GetWinAddDataEx( win_id );
    MMIPUB_MENU_NOTIFY_T notify = {0};
    
    if ( PNULL == win_info_ptr )
    {
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if ( PNULL != win_info_ptr->func.OnMenuInit )
        {
            InitNotify( win_info_ptr, &notify );
            win_info_ptr->func.OnMenuInit( &notify );
        }
        MMK_SetActiveCtrl( win_info_ptr->ctrl_handle, FALSE );
        break;
        
    case MSG_CLOSE_WINDOW:
        if ( PNULL != win_info_ptr->func.OnMenuDestory )
        {
            InitNotify( win_info_ptr, &notify );
            win_info_ptr->func.OnMenuDestory( &notify );
        }
        MMK_FreeWinAddDataEx( win_info_ptr->win_handle );
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
#ifdef GUIF_SOFTKEY_PDASTYLE
        //pda风格的softkey，popupmenu界面不显示softkey，对OK键（例如按下Lsoftkey）的响应为Cancel
        OnPubMenuCancel(win_id);
#else
        //非pda风格的softkey，popupmenu界面显示softkey，对OK键（例如点击softkey或按下Lsoftkey）的响应为OK
        OnPubMenuOk(win_id);
#endif
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        OnPubMenuCancel(win_id);
        break;
        
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
#ifndef MMI_PDA_SUPPORT  // menu 键不相应
#endif
        OnPubMenuOk(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : pub menu win init
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void PubMenuWinInit(
                          MMIPUB_MENU_T*  win_info_ptr,
                          MMIPUB_MENU_PARAM_T* param
                          )
{
    if ( PNULL != win_info_ptr
        && PNULL != param )
    {
        GUIMENU_INIT_DATA_T  init_data   = {0};
        MMI_CONTROL_CREATE_T create      = {0};
        MMI_CTRL_ID_T        ctrl_id     = ( 0 == param->ctrl_id ) ? MMIPUB_MENU_CTRL_ID : param->ctrl_id;

        //set init data
        if ( MENU_DYNA == param->group_id )
        {
            init_data.is_static = FALSE;
            init_data.node_max_num = GUIMENU_DYNA_NODE_MAX_NUM;
            init_data.menu_style = param->menu_style;//GUIMENU_STYLE_POPUP_AUTO;
        }
        else
        {
            init_data.is_static = TRUE;
            init_data.group_id  = param->group_id;            
        }

        create.ctrl_id           = ctrl_id;
        create.guid              = SPRD_GUI_MENU_ID;
        create.parent_win_handle = win_info_ptr->win_handle;
        create.init_data_ptr     = &init_data;
        MMK_CreateControl( &create );
        
        win_info_ptr->ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, ctrl_id );
    }
}



/*****************************************************************************/
//  Description : open pub menu win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenPubMenuWin(
                                  MMIPUB_MENU_PARAM_T* param
                                  )
{
    BOOLEAN              result = FALSE;
    MMI_WIN_ID_T         win_id = 0;
    MMI_HANDLE_T         applet_handle = 0;
    MMI_HANDLE_T         win_handle = 0;
    MMIPUB_MENU_T*       win_info_ptr = PNULL;
    MMI_WINDOW_CREATE_T  win_create = {0};
    
    if ( PNULL == param )
    {
        return;
    }

    if ( 0 != param->win_id )
    {
        win_id = param->win_id;
    }
    else
    {
        win_id = MMIPUB_MENU_WIN_ID;
    }

    if ( 0 == param->applet_handle )
    {
        applet_handle = MMK_GetFirstAppletHandle();
    }
    else
    {
        applet_handle = param->applet_handle;
    }
    
    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {   
        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_MENU_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_MENU_T));

        win_info_ptr->func = param->func;
        win_info_ptr->dst_handle = param->dst_handle;

        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleMenuWinMsg;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.window_style = WS_HAS_TRANSPARENT; //|WS_HAS_PROMPT_WIN;
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        win_create.add_data_ex_ptr = win_info_ptr;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }
        
        if (result)
        {
            GUIWIN_CreateSoftkeyDyn(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN);
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
			GUIWIN_CreateTitleDyna(win_id, TXT_NULL);
#endif
            PubMenuWinInit( win_info_ptr, param );
        }
        else
        {
            SCI_FREE(win_info_ptr);
        }
    }
}