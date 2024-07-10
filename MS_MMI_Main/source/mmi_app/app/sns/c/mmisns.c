/*****************************************************************************
** File Name:      mmisns.c                                               *
** Author:                                                                   *
** Date:           4/01/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 4/2009         renwei       Create                                   *
******************************************************************************/

/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#ifdef SNS_SUPPORT
#include "std_header.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmidisplay_data.h"
#include "mmisns.h"
#include "guifont.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

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
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelloSprdWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );
/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

WINDOW_TABLE( MMI_HELLO_SPRD_WIN_TAB ) = 
{
    WIN_ID(MMI_HELLO_SPRD_WIN_ID),
    WIN_FUNC((uint32)HandleHelloSprdWinMsg),
//     WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
//     WIN_TITLE(TXT_RING_SELECT_ALARM),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHelloSprdWinMsg(
                                         MMI_WIN_ID_T     win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM           param
                                         )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        break;

    case MSG_GET_FOCUS:
        break;

    case MSG_LOSE_FOCUS:
        break;

    case MSG_FULL_PAINT:
        {

        }
        break;

    case MSG_KEYDOWN_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        break;

    default:
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : open hello sprd windows
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMI_CreateHelloSprdWin(void)
{
    MMK_CreateWin((uint32*)MMI_HELLO_SPRD_WIN_TAB, PNULL);
}
#endif