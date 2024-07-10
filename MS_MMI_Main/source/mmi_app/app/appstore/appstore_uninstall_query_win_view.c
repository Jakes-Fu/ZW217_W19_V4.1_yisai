/*****************************************************************************
** File Name:     appstore_uninstall_query_win_view.c                        *
** Author:        mingwei.jia                                                *
** Date:          09/06/2021                                                 *
** Copyright:     2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:   This file is used to define app  store query win view      *
**                function                                                   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/06/2021     mingwei.jia      Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "appstore_flow.h"
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "watch_commonwin_export.h"

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
//  Description : HandleUninstallQueryWinMsg
//  Parameter: [In] win_id
//             [In] msg_id
//             [In] param
//             [Out] None
//             [Return] MMI_RESULT_E
//  Author: fangfang.yao,mingwei.jia
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUninstallQueryWinMsg(MMI_WIN_ID_T      win_id,
                                                         MMI_MESSAGE_ID_E  msg_id,
                                                         DPARAM            param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    TRACE_APPSTORE("[APPSTORE_QUERY_WIN_VIEW]:enter win succ£¬msg_id = 0x%x",msg_id);;
    switch(msg_id)
    {
        case MSG_NOTIFY_OK:
        {
            //back
            APPSTORE_Flow_HandleHomeUninstallQueryWinButton1();
            MMK_CloseWin( win_id );
            break;
        }
        case MSG_NOTIFY_CANCEL:
        {
            // uninstalling
            APPSTORE_Flow_HandleHomeUninstallQueryWinButton2();
            MMK_CloseWin( win_id );
            break;
        }
        default:
        {
            TRACE_APPSTORE("[APPSTORE_QUERY_WIN_VIEW]:click invalid");
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
//  Description : APPSTORE_Uninstall_Query_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_Uninstall_Query_Win_Enter( void )
{
    MMI_STRING_T txt_msg = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    MMI_GetLabelTextByLang( TXT_UNINSTALL_QUERY, &txt_msg );
#ifdef  ADULT_WATCH_SUPPORT
    AdultWatchCOM_QueryWin_2Btn_Enter(APPSTORE_UNINSTALL_QUERY_WIN_ID,
                                      &txt_msg,
                                      PNULL,
                                      PNULL,
                                      image_watch_back,
                                      image_watch_delete,
                                      HandleUninstallQueryWinMsg);
#else
    WatchCOM_QueryWin_2Btn_Enter(APPSTORE_UNINSTALL_QUERY_WIN_ID,
                                     &txt_msg,
                                     PNULL,
                                     PNULL,
                                     image_watch_back,
                                     image_watch_delete,
                                     softkey_text,
                                     HandleUninstallQueryWinMsg);
#endif
    TRACE_APPSTORE("[APPSTORE_QUERY_WIN_VIEW]:enter win succ");
}
