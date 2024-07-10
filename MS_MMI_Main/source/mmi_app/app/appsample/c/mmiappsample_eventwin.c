/*****************************************************************************
 ** File Name:   mmiappsample_eventwin.c
 ** Author:      sam.hua
 ** Date:        13/aug/2012
 ** Copyright:   2012 Spreadtrum, Incorporated. All Rights Reserved.         *
 ** Description: This file is used to describe event sample code 
 *****************************************************************************
 **                         Important Edit History                           *
 ** -------------------------------------------------------------------------*
 *  ** DATE           NAME             DESCRIPTION                           *
 *  13/aug/2012      sam.hua       written
 ****************************************************************************/


#include "mmiappsample_internal.h"

#include "mmiappsample_text.h"
#include "mmiappsample_id.h"
#include "mmiappsample_image.h"
#include "mmiappsample_menutable.h"

#include "mmi_pubmenuwin.h"
#include "window_parse.h"
#include "mmk_msg.h"

#include "guilistbox.h"


/******************************************************************************
//  Description : win handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleEventWinMsg(MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                              );


//FuncWinTAB
WINDOW_TABLE( MMIAPPSAMPLE_EVENT_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleAppSampleEventWinMsg),
    WIN_ID( MMIAPPSAMPLE_EVENT_WIN_ID ),
    WIN_TITLE(TXT_APPSAMPLE_EVENT),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIAPPSAMPLE_EVENTWIN_LIST_CTRL_ID),
    END_WIN
};

/******************************************************************************
//  Description : event win entry
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
PUBLIC void EnterAppSampleEventWin(void)
{
    MMK_CreateWin((uint32*)MMIAPPSAMPLE_EVENT_WIN_TAB, PNULL);
    return;
}

/******************************************************************************
//  Description : win handler
//	Global resource dependence : 
//  Author:  sam.hua 
//  Note:   
 ******************************************************************************/
LOCAL MMI_RESULT_E  HandleAppSampleEventWinMsg(MMI_WIN_ID_T win_id,
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM param
                                              )
{

    return MMI_RESULT_FALSE;
}


