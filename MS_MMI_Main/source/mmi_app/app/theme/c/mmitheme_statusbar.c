/****************************************************************************
** File Name:      mmi_theme.c                                              *
** Author:         Xiaoqing.lu                                              *
** Date:           12/15/2008                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mmi theme.             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        Xiaoqing.lu       Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_theme_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmitheme_statusbar.h"
#include "mmidisplay_data.h"
#include "mmi_id.h"
#include "mmi_image.h"
#include "guifont.h"
#include "guistatusbar.h"

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#include "mmidropdown_position.h"
#endif
#include "mmi_theme.h"
#include "mmikl_export.h"
#include "mmicc_export.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMITHEME_STB_SCROLLKEY_HEIGHT  DP2PX_VALUE(24)//32//20 //滑动栏的高度
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : 状态栏的TP消息自定义处理
// Global resource dependence : 
// Author: bin.ji
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMITHEME_HandleStatusbarTPMsg(
                                                  MMI_HANDLE_T win_handle, //窗口句柄
                                                  MMI_MESSAGE_ID_E  msg_id,   //事件类型
                                                  DPARAM            param     //附带的处理参数
                                                  )
{
#ifdef PDA_UI_DROPDOWN_WIN
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    GUI_POINT_T     point = {0};
    static GUI_POINT_T  down_point   = {0};   
    MMI_HANDLE_T    ctrl_handle = MMK_GetWinStatusBarHandle(win_handle);
    
    if (0 == ctrl_handle)
    {
        return MMI_RESULT_FALSE;
    }

/*lint -e764*/
    switch( msg_id )
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if(!GUIWIN_IsStbDropDown(win_handle))
        {
            point.x=MMK_GET_TP_X(param);            
            point.y=MMK_GET_TP_Y(param);
            down_point = point;
            GUIWIN_SetStbPressed(win_handle, TRUE);
#ifdef OLD_CC_SUPPORT //for 217 ram cutdown:remove old cc
            if (!MMIAPICC_IsCcOnTopWin()
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
                &&!MMIAPIKL_IsPswWinOpen()
#endif
                )
            {
                //MMIDROPDOWNWIN_Create();
            }
#endif
        }

        break;
    case MSG_TP_PRESS_MOVE:
        if(!GUIWIN_IsStbDropDown(win_handle))
        {
            GUI_RECT_T ctrl_rect = {0};

            GUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &ctrl_rect);

            point.x=MMK_GET_TP_X(param);
            point.y=MMK_GET_TP_Y(param);            
            if(point.y > ctrl_rect.bottom - MMITHEME_STB_SCROLLKEY_HEIGHT)
            {     
                GUIWIN_SetStbPressed(win_handle, FALSE);

                //SCI_TRACE_LOW:"status bar, tp move %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMITHEME_STATUSBAR_100_112_2_18_3_3_23_51,(uint8*)"d", point.y - down_point.y);
                if(point.y - down_point.y > MMIDROPDOWN_INIT_SPEED)
                {
                    //MMIDROPDOWNWIN_SetSlideState();
                }
            }
            down_point = point;
        }
        break;
    case MSG_TP_PRESS_UP:
        if(!GUIWIN_IsStbDropDown(win_handle))
        {
            if(GUIWIN_IsStbPressed(win_handle))
            {
                GUIWIN_SetStbPressed(win_handle, FALSE);
                
                //目前MMI_WIN_TEXT_DATE为0，如果不为0，需要修改下面的0
                GUIWIN_SetStbItemVisible(0/*MMI_WIN_TEXT_DATE*/,FALSE);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
/*lint -e764*/

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : get the status bar style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetStatusBarStyle(
                                          MMI_STATUSBAR_STYLE_T  *style_ptr
                                          )
{
    //SCI_PASSERT(PNULL != style_ptr, ("MMITHEME_GetStatusBarStyle: the param is PNULL!")); /*assert verified*/
    if (PNULL == style_ptr)
    {
        return FALSE;
    }
#ifdef MAINLCD_SIZE_128X64    
    style_ptr->bg_info.bg_type = GUI_BG_COLOR;
    style_ptr->bg_info.color = MMI_WHITE_COLOR;
	style_ptr->bg_info.img_id = 0;

    style_ptr->bg_info.shape = GUI_SHAPE_RECT;
	
    style_ptr->item_space = DP2PX_VALUE(1);
	
    style_ptr->font_type = MMI_STATUS_FONT_TYPE;
    style_ptr->font_color = MMI_BLACK_COLOR;
    style_ptr->align = ALIGN_HVMIDDLE;//ALIGN_LEFT;
    style_ptr->item_height = DP2PX_VALUE(16);
    style_ptr->item_width = MMISTATUSBAR_ITEM_WIDTH;
    style_ptr->timer_out = MMISTATUSBAR_ICONANIM_TIMEOUT;
#else
    //style_ptr->bg_info.bg_type = GUI_BG_IMG;
	style_ptr->bg_info.bg_type = GUI_BG_COLOR;

#if defined (MMI_PDA_SUPPORT) && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        style_ptr->bg_info.img_id = IMAGE_ISTYLE_STATUS_BAR;
    }
    else
#endif
    {
        //style_ptr->bg_info.img_id = IMAGE_STATUS_BAR;
    }    
	
    style_ptr->bg_info.color = MMI_BLACK_COLOR;
    style_ptr->bg_info.shape = GUI_SHAPE_ROUNDED_RECT;

    style_ptr->item_space = MMISTATUSBAR_ITEM_SPACE;

    style_ptr->font_type = MMI_STATUS_FONT_TYPE;
    style_ptr->font_color = MMI_BLACK_COLOR;
    style_ptr->align = ALIGN_HVMIDDLE;//ALIGN_LEFT;
    style_ptr->item_height = MMISTATUSBAR_ITEM_HEIGHT;
    style_ptr->item_width = MMISTATUSBAR_ITEM_WIDTH;
    style_ptr->timer_out = MMISTATUSBAR_ICONANIM_TIMEOUT;
    style_ptr->is_transparent = FALSE;//xiyuan edit 2012.4.11

#endif

    style_ptr->scrollmsg_timer_out = MMISTATUSBAR_SCROLLMSG_TIMEOUT;
    style_ptr->scrollmsg_pause_timer_out = MMISTATUSBAR_SCROLLMSG_PAUSE_TIMEOUT;
	style_ptr->layer_alpha = 165;

    return TRUE;
} 

/*****************************************************************************/
// 	Description : get the status bar rect
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetStatusBarCtrlId(void)
{
	return MMICOMMON_STATUSBAR_CTRL_ID;
}

/*****************************************************************************/
// 	Description : get the height of statusbar
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetStatusBarHeight(void)
{
    return MMI_STATUSBAR_HEIGHT;
}



/*Edit by script, ignore 3 case. Thu Apr 26 19:01:08 2012*/ //IGNORE9527
