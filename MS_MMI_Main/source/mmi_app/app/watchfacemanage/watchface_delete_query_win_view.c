/*****************************************************************************
** File Name:     watchface_delete_query_win_view.c                                 *
** Author:        bin.wang1                                                  *
** Date:          09/06/2021                                                 *
** Copyright:     2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:   This file is used to define app  store query win view      *
**                function                                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/06/2021     bin.wang1        Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "watch_commonwin_export.h"

#include "watchface_set_flow.h"
#include "watchface_delete_query_win_view.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                          LOCAL VARIABLES                                  *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : HandleDeleteQueryWinMsg
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteQueryWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    TRACE_WATCHFACE(" enter");

    switch(msg_id)
    {
        case MSG_NOTIFY_OK:
        {
            //back
            WATCHFACE_Set_Flow_HandleDeleteQueryWinButton1();
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_NOTIFY_CANCEL:
        {
            // delete
            WATCHFACE_Set_Flow_HandleDeleteQueryWinButton2();
            MMK_CloseWin( win_id );
            break;
        }
        default:
        {
            TRACE_WATCHFACE(" msg_id 0x%x", msg_id);
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : WATCHFACE_Delete_Query_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Delete_Query_Win_Enter( void )
{
    MMI_STRING_T txt_msg = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL, NULL, NULL};
#ifdef  ADULT_WATCH_SUPPORT
    MMI_GetLabelTextByLang( TXT_DELETE_WATCH_QUERY, &txt_msg );
    AdultWatchCOM_QueryWin_2Btn_Enter(WATCHFACE_DELETE_QUERY_WIN_ID,
                                      &txt_msg,
                                      PNULL,
                                      PNULL,
                                      image_watch_back,
                                      image_watch_delete,
                                      HandleDeleteQueryWinMsg);
#else
    WatchCOM_QueryWin_2Btn_Enter(WATCHFACE_DELETE_QUERY_WIN_ID,
                                 &txt_msg,
                                 PNULL,
                                 PNULL,
                                 image_watch_back,
                                 image_watch_delete,
                                 softkey_text,
                                 HandleDeleteQueryWinMsg);
#endif
}
/*****************************************************************************/
//  Description : WATCHFACE_Delete_Note_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Delete_Note_Win_Enter( void )
{
    MMI_STRING_T txt_msg = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL, NULL, NULL};
    TRACE_WATCHFACE("enter");

    MMI_GetLabelTextByLang( TXT_DELETE_DEFAULT_WATCH_FACE, &txt_msg );
#ifdef  ADULT_WATCH_SUPPORT
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(WATCHFACE_DELETE_DEFAULT_WF_NOTE_WIN_ID, &txt_msg, 0, PNULL);
#else
    WatchCOM_NoteWin_2Line_Enter(WATCHFACE_DELETE_DEFAULT_WF_NOTE_WIN_ID, &txt_msg, 0, softkey_text, PNULL);
#endif
}

