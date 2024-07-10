/*****************************************************************************
** File Name:      watchface_note_win_view.c                                 *
** Author:         bin.wang1                                                 *
** Date:           09/06/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app store note win view       *
**                 function                                                  *
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
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
#include "watchface_set_note_win_view.h"


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
//  Description : WATCHFACE_Set_Note_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void WATCHFACE_Set_Note_Win_Enter( void )
{
    MMI_STRING_T txt_msg = { 0 };
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL, NULL, NULL};
    TRACE_WATCHFACE(" enter");

    MMI_GetLabelTextByLang( TXT_SET_WATCH_FACE_ALREADY, &txt_msg );
#ifdef  ADULT_WATCH_SUPPORT
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(WATCHFACE_SET_NOTE_WIN_ID, &txt_msg, 0, PNULL);
#else
    WatchCOM_NoteWin_2Line_Enter(WATCHFACE_SET_NOTE_WIN_ID, &txt_msg, 0, softkey_text, PNULL);
#endif
}
