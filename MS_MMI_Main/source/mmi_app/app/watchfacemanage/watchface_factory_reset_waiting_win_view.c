/*****************************************************************************
** File Name:      watchface_factory_reset_waiting_win_view.c                *
** Author:         qi.zhang                                               *
** Date:           07/18/2022                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define watch face note win view      *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/18/2022     qi.zhang         Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
#include "mmiset_id.h"

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

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 进入表盘恢复出厂设置等待界面
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_WaitingFactoryResetWinEnter( void )
{
    MMI_STRING_T    wait_text = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};

    TRACE_WATCHFACE("Enter");

    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
    WatchCOM_WaitingWin_1Line_Enter(WATCHFACE_FACTORY_RESET_WAIT_WIN_ID, &wait_text, 0, softKey, PNULL);
}
/*****************************************************************************/
//  Description : 关闭表盘恢复出厂设置等待界面
//  Parameter: [In] NONE
//             [Out]NONE
//             [Return]NONE
//  Author:qi.zhang
/*****************************************************************************/
PUBLIC void WATCHFACE_CloseWaitingFactoryResetWin(void)
{
    TRACE_WATCHFACE("enter");
    if(MMK_IsOpenWin( WATCHFACE_FACTORY_RESET_WAIT_WIN_ID ))
    {
        MMK_CloseWin(WATCHFACE_FACTORY_RESET_WAIT_WIN_ID);
    }
}


