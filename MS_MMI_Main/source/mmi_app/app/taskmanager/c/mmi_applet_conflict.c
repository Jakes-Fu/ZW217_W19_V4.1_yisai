/*****************************************************************************
** File Name:      mmi_applet_conflict.c                                      *
** Author:                                                                   *
** Date:           27/04/2012                                                *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to applet manager                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2012       James.Zhang       Create                                    *
******************************************************************************/

#define _MMI_APPLET_CONFLICT_C_
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_applet_conflict.h"
#include "mmipub.h"
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_modu_main.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMI_APPLET_CONFLICT_T* s_applet_conflict_ptr = PNULL;
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get conflict applet handle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetConflictAppletHandle( void );

/*****************************************************************************/
//  Description : get applet text id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetAppletTextId( 
                                    MMI_HANDLE_T applet_handle
                                    );

/*****************************************************************************/
//  Description : resolve applet conflict
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResolveAppletConflict( 
                                 MMIPUB_INFO_T  *win_info_ptr
                                 );

/*****************************************************************************/
//  Description : handle conflict win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConflictWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        );

/*****************************************************************************/
//  Description : create conflict ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void CreateConflictPtr( 
                             const MMI_APPLET_CONFLICT_T* conflict_ptr
                             );

/*****************************************************************************/
//  Description : destroy conflict ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyConflictPtr( void );

/*****************************************************************************/
//  Description : notify resolve conflict
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyResolveConflict( void );

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryWin(
                        MMI_HANDLE_T handle
                        );

/*****************************************************************************/
//  Description : check applet conflict
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMI_CheckAppletConflict(
                                       const MMI_APPLET_CONFLICT_T* conflict_ptr
                                       )
{
    MMI_HANDLE_T handle = 0;

    if ( PNULL == conflict_ptr )
    {
        return FALSE;
    }

    CreateConflictPtr( conflict_ptr );
    
    if ( 0 != ( handle = GetConflictAppletHandle() ) )
    {
        OpenQueryWin( handle );
    }
    else
    {
        NotifyResolveConflict();

        DestroyConflictPtr();
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : get conflict applet handle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_HANDLE_T GetConflictAppletHandle( void )
{
    uint32       i      = 0;
    MMI_HANDLE_T handle = 0;
    MMI_APPLET_INFO_LIST_T* list_ptr = MMK_CreateAppletInfoList( 0 );
    
    for ( ; i < list_ptr->num_of_list; i++ )
    {
        if ( i != list_ptr->cur_index
            && MMK_GetAppletState( list_ptr->list_ptr[i].handle, MMI_APPLET_STATE_MEM_CONFLICT )
            && MMK_GetFirstAppletHandle() != list_ptr->list_ptr[i].handle )
        {
            handle = list_ptr->list_ptr[i].handle;
            break;
        }
    }

    MMK_DestroyAppletInfoList( list_ptr );

    return handle;
}

/*****************************************************************************/
//  Description : get applet text id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetAppletTextId( 
                                    MMI_HANDLE_T applet_handle
                                    )
{
    MMI_TEXT_ID_T       text_id = 0;
    const CAF_STATIC_APPLET_T *static_applet_ptr = MMK_GetStaticAppletPtr( MMK_GetGuidByHandle( applet_handle ) );

    if ( PNULL != static_applet_ptr )
    {
        text_id = static_applet_ptr->text_id;
    }

    return text_id;
}

/*****************************************************************************/
//  Description : resolve applet conflict
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResolveAppletConflict( 
                                 MMIPUB_INFO_T  *win_info_ptr
                                 )
{
    MMI_HANDLE_T handle = 0;

    if ( PNULL != win_info_ptr )
    {
        MMK_CloseApplet( GetConflictAppletHandle() );
        
        //仍然有产生冲突的applet
        if ( 0 != ( handle = GetConflictAppletHandle() ) )
        {
            OpenQueryWin( handle );
        }
        else
        {
            NotifyResolveConflict();

            MMK_CloseWin( win_info_ptr->win_handle );
        }
    }
}

/*****************************************************************************/
//  Description : create conflict ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void CreateConflictPtr( 
                             const MMI_APPLET_CONFLICT_T* conflict_ptr
                             )
{
    if ( PNULL != conflict_ptr )
    {
        DestroyConflictPtr();

        s_applet_conflict_ptr = SCI_ALLOC_APP( sizeof(MMI_APPLET_CONFLICT_T) + conflict_ptr->size_of_param );

        SCI_MEMCPY(s_applet_conflict_ptr, conflict_ptr, sizeof(MMI_APPLET_CONFLICT_T) );
        
        if ( conflict_ptr->size_of_param > 0 )
        {
            s_applet_conflict_ptr->param = (uint8*)s_applet_conflict_ptr + sizeof(MMI_APPLET_CONFLICT_T);
            SCI_MEMCPY(s_applet_conflict_ptr->param, conflict_ptr->param, conflict_ptr->size_of_param );
        }
    }
}

/*****************************************************************************/
//  Description : destroy conflict ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void DestroyConflictPtr( void )
{
    if ( PNULL != s_applet_conflict_ptr )
    {
        SCI_FREE(s_applet_conflict_ptr);
        s_applet_conflict_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : notify resolve conflict
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void NotifyResolveConflict( void )
{
    if ( PNULL != s_applet_conflict_ptr
        && PNULL != s_applet_conflict_ptr->func )
    {
        s_applet_conflict_ptr->func( 0, MSG_APPLET_RESOLVE_CONFLICT, s_applet_conflict_ptr->param );
    }
}

/*****************************************************************************/
//  Description : open query win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void OpenQueryWin(
                        MMI_HANDLE_T handle
                        )
{
    MMI_WIN_ID_T  win_id = MMIAPPLET_CONFLICT_WIN_ID;
    MMI_TEXT_ID_T alert_text_id = TXT_COMMON_MEMORY_CONFLICT;
    MMI_STRING_T  alert_text_str = {0};
    MMI_TEXT_ID_T app_text_id = GetAppletTextId( handle );
    MMI_STRING_T  app_text_str = {0};

    MMI_GetLabelTextByLang(alert_text_id, &alert_text_str);
    MMI_GetLabelTextByLang(app_text_id, &app_text_str);

    if ( !MMK_IsOpenWin( win_id ) )
    {
        MMIPUB_OpenQueryWinByTextPtr( &alert_text_str, &app_text_str, IMAGE_PUBWIN_QUERY, &win_id, HandleConflictWinMsg );
    }
    else
    {
        MMIPUB_SetAlertWinTextByPtr( win_id, &alert_text_str, &app_text_str, TRUE );
    }
}

/*****************************************************************************/
//  Description : handle conflict win msg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConflictWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E   result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T  *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_APP_MENU:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        ResolveAppletConflict( win_info_ptr );
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;

    case MSG_CLOSE_WINDOW:
        DestroyConflictPtr();
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
        break;
    }
    return (result);
}