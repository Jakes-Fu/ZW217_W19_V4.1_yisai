/*****************************************************************************
** File Name:      sample_iconlist_ctrl.c                                    *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _SAMPLE_ICONLIST_CTRL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE

#include "window_parse.h"
#include "guiiconlist.h"
#include "sample_id.h"
#include "sample_text.h"
#include "sample_image.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIHELLO_ICONLIST_ICON_NUM      8

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : handle icon list win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestIconlistWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//picture full screen preview
WINDOW_TABLE( MMI_TEST_ICONLIST_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_ICONLIST_WIN_ID),
    WIN_TITLE(TXT_HELLO_TITLE),        
	WIN_FUNC( (uint32)HandleTestIconlistWinMsg),
	CREATE_ICONLIST_CTRL(MMI_TEST_ICONLIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
	END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : handle icon list win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestIconlistWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    uint16                      icon_index = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T               ctrl_id = MMI_TEST_ICONLIST_CTRL_ID;
    GUIICONLIST_DISP_T          icon_list = {0};
    GUIANIM_DATA_INFO_T         data_info = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIICONLIST_SetTotalIcon(ctrl_id,MMIHELLO_ICONLIST_ICON_NUM);
        GUIICONLIST_SetCurIconIndex(0,ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_ICONLIST_APPEND_TEXT:
        //append icon item
        icon_index = *((uint16 *)param);

        //title
        MMI_GetLabelTextByLang(TXT_HELLO_TITLE,&icon_list.name_str);

        //softkey
        icon_list.softkey_id[0] = TXT_NULL;
        icon_list.softkey_id[1] = TXT_NULL;
        icon_list.softkey_id[2] = STXT_RETURN;

        GUIICONLIST_AppendText(icon_index,ctrl_id,&icon_list);
        break;

    case MSG_CTL_ICONLIST_APPEND_ICON:
        //append icon
        icon_index = *((uint16 *)param);

        data_info.img_id = HELLO_SPRD_TEST;
        data_info.type   = GUIANIM_TYPE_IMAGE_BMP;
        
        //append icon
        GUIICONLIST_AppendIcon(icon_index,ctrl_id,&data_info,PNULL);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : creat icon list win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateIconlistWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ICONLIST_WIN_TAB, PNULL);
}
#endif
