/*****************************************************************************
** File Name:      sample_anim_ctrl.c                                        *
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

#define _SAMPLE_ANIM_CTRL_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"

#ifdef MMI_MOCOR_SAMPLE
#include "window_parse.h"
#include "guianim.h"
#include "guictrl_api.h"
#include "sample_id.h"
#include "sample_image.h"
#include "mmidisplay_data.h"
#include "mmitheme_pos.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

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
// 	Description : handle anim win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestAnimWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        );

/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/
//picture full screen preview
WINDOW_TABLE( MMI_TEST_ANIM_WIN_TAB ) = 
{
    WIN_ID(MMI_TEST_ANIM_WIN_ID),
	WIN_FUNC( (uint32)HandleTestAnimWinMsg),
    WIN_STYLE(WS_HAS_ANIM_BG),
	CREATE_ANIM_CTRL(MMI_TEST_ANIM_CTRL_ID,MMI_TEST_ANIM_WIN_ID),
	END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : handle anim win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTestAnimWinMsg(
                                        MMI_WIN_ID_T        win_id,
                                        MMI_MESSAGE_ID_E    msg_id,
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMI_TEST_ANIM_CTRL_ID;
    GUI_BOTH_RECT_T         anim_rect = MMITHEME_GetFullScreenBothRect();
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(ctrl_id,&anim_rect);
        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;

        data_info.img_id = HELLO_SPRD_TEST;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.bg.bg_type = GUI_BG_COLOR;
        display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

        GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_FULL_PAINT:
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:
        //当图片解码完成,可以显示app会收到此消息
        break;

    case MSG_CTL_ANIM_GIF_UPDATE:
        //当gif动画刷新每一帧,app会收到此消息
        break;

    case MSG_APP_OK:
    case MSG_APP_CANCEL:
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
// 	Description : creat anim win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateAnimWin(void)
{
    MMK_CreateWin((uint32*)MMI_TEST_ANIM_WIN_TAB, PNULL);
}
#endif
