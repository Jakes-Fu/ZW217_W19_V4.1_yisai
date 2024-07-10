/*****************************************************************************
** File Name:      mmiidle_istyle.c                                          *
** Author:                                                                   *
** Date:           07/05/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011      James.Zhang              Creat
******************************************************************************/

#define _MMIIDLE_ISTYLE_C_

#ifdef MMI_PDA_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "guimenu.h"
#include "guires.h"
#include "mmi_theme.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_modu_main.h"
#include "mmidisplay_data.h"
#include "guistring.h"
#include "mmi_image.h"
#include "ui_layer.h"
#include "mmi_link.h"
#include "mmi_text.h"
#include "mmi_nv.h"
#include "guilcd.h"
#include "mmi_mainmenu_export.h"
#include "mmk_tp.h"
#include "mmitheme_special.h"
#include "mmi_menutable.h"
#include "guiimg.h"
#include "mmiidle_export.h"
#include "img_dec_interface.h"
#ifdef DYNAMIC_MAINMENU_SUPPORT
#include "mmi_mainmenu_synchronize.h"
#endif
#include "guibutton.h"
#include "mmiidle_display.h"
#include "guistatusbar.h"
#include "mmiidle_statusbar.h"
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
#include "ctrlmainmenu_export.h"
#include "mmitheme_mainmenu.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : create istyle menu
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_CreateIStyleMenu(
                                     MMI_HANDLE_T win_handle
                                     )
{
#if defined MMI_ISTYLE_SUPPORT
    if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
    {
        if ( 0 == MMK_GetCtrlHandleByWin( win_handle, MAINMENUWIN_MENU_CTRL_ID ) )
        {
            // GUIMENU_INIT_DATA_T     init_data = {0};
            MMI_CONTROL_CREATE_T    create = {0};
            GUIMAINMENU_INIT_DATA_T init_data = {0};
            
            //set init data
            init_data.both_rect = MMITHEME_GetWinClientBothRect(win_handle);
            //init_data.is_static = TRUE;
            //init_data.group_id  = MENU_MAINMENU_ISTYLE;
            init_data.type = MMIMAINMENU_ISTYLE_E;
            
            //create
            create.ctrl_id           = MAINMENUWIN_MENU_CTRL_ID;
            create.guid              = SPRD_GUI_MAINMENU_ID;
            create.parent_win_handle = win_handle;
            create.init_data_ptr     = &init_data;
            
            MMK_CreateControl(&create);
            
            MMK_SetWinSupportAngle( win_handle, WIN_SUPPORT_ANGLE_0 );
            MMK_SetWinAngle( win_handle, 0, FALSE );
            
            
            MMIDILE_StatusBarInit();
        }
        GUIWIN_SetStbState( win_handle, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
		GUIWIN_SetStbBgIsDesplay(win_handle,TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
		GUIWIN_SetStbBgImageId(win_handle, IMAGE_IDLE_STATUS_BAR);
#endif
    }
    else
#endif		
    {
        if ( 0 != MMK_GetCtrlHandleByWin( win_handle, MAINMENUWIN_MENU_CTRL_ID ) )
        {
            MMK_DestroyDynaCtrl( MMK_GetCtrlHandleByWin( win_handle, MAINMENUWIN_MENU_CTRL_ID ) );
        
            GUIWIN_SetStbState( win_handle, GUISTATUSBAR_STATE_USE_LAYER, FALSE);
            MMIDILE_StatusBarInit();
        }
    }
}

#ifdef MMI_ISTYLE_SUPPORT
LOCAL MMI_RESULT_E HandleSomeSpecialMsg(
                                        MMI_WIN_ID_T         win_id, 
                                        MMI_MESSAGE_ID_E     msg_id, 
                                        DPARAM               param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_NOTIFY_MENU_SET_STATUS:
        if (MMIAPIUDISK_UdiskIsRun()) //U≈Ã π”√÷–
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        }
        
        else
        {
            if (*(GUIMENU_MOVE_STATE_T*)param == GUIMENU_STATE_MOVE_SET)
            {
#ifdef MMI_VIDEOWALLPAPER_SUPPORT
                MMIAPISETVWP_Pause();
#endif
                CTRLMAINMENU_InitMoveState(MAINMENUWIN_MENU_CTRL_ID);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle istyle idle message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIDLE_IStyleHandleMsg(
                                            MMI_WIN_ID_T         win_id,
                                            MMI_MESSAGE_ID_E     msg_id,
                                            DPARAM               param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
#if defined(MMI_ISTYLE_SUPPORT)
	if( MMITHEME_IsIstyle() )
	{
		result = MMIAPIISTYLESEARCH_HandleSearchWinMsg(win_id,msg_id,param); 
	} 
#endif
    if (MMI_RESULT_FALSE == result)
    {
        result = MMIAPIMENU_HandleKeySlide( win_id, msg_id, param );
    }
    
    if (MMI_RESULT_FALSE == result)
    {
        result = HandleSomeSpecialMsg(win_id, msg_id, param);
    }

    if (MMI_RESULT_FALSE == result)
    {
        result = MMIAPIMENU_HandleMainMenuWinMsg( win_id, msg_id, param );
    }
    
    return  result;        
}
#endif //MMI_ISTYLE_SUPPORT

#endif


/*Edit by script, ignore 1 case. Fri Apr 27 09:37:16 2012*/ //IGNORE9527
