/*****************************************************************************
** File Name:      watchface_install_note_win_view.c                         *
** Author:         chunjuan.liang                                            *
** Date:           27/07/2022                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app store note win view       *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 27/07/2021     chunjuan.liang        Create                               *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_id.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmicom_trace.h"
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

/*---------------------------------------------------------------------------*
**                          GLOBAL FUNCTION                                  *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : WATCHFACE_Note_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Install_Fail_Note_Win_Enter(void)
{
    MMI_STRING_T txt_msg = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL, NULL, NULL};
    TRACE_WATCHFACE(" enter");

    MMI_GetLabelTextByLang( TXT_INSTALL_FAIL, &txt_msg );
#ifdef  ADULT_WATCH_SUPPORT
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(WATCHFACE_NOTE_WIN_ID, &txt_msg, 0, PNULL);
#else
    WatchCOM_NoteWin_2Line_Enter(WATCHFACE_NOTE_WIN_ID, &txt_msg, 0, softkey_text, PNULL);
#endif
}
