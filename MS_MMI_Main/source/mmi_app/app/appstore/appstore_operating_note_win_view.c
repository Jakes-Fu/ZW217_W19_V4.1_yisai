/*****************************************************************************
** File Name:      appstore_operating_note_win_view.c                        *
** Author:         mingwei.jia                                               *
** Date:           09/06/2021                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app store note win view       *
**                 function                                                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/06/2021     mingwei.jia      Create                                    *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_id.h"
#include "mmi_text.h"
#include "watch_commonwin_export.h"
#include "appstore_common.h"

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
//  Description : APPSTORE_Operating_Note_Win_Enter
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void APPSTORE_Operating_Note_Win_Enter( void )
{
    MMI_STRING_T operating_str = {0};
    WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
    MMI_GetLabelTextByLang(TXT_APPSTORE_OPERATING, &operating_str);
#ifdef  ADULT_WATCH_SUPPORT
    Adult_WatchCOM_NoteWin_2Line_Toast_Enter(APPSTORE_OPERATING_NOTE_WIN_ID, &operating_str, 0, PNULL);
#else
    WatchCOM_NoteWin_2Line_Enter(APPSTORE_OPERATING_NOTE_WIN_ID, &operating_str, 0, softkey_text, PNULL);
#endif
    TRACE_APPSTORE("[APPSTORE_NOTE_WIN_VIEW]:enter note win succ");
}
