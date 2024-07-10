/*****************************************************************************
** File Name:       watch_commonwin_internal.c                               *
** Author:          fangfang.yao                                             *
** Date:            2020/02/28                                               *
** Copyright:                                                                *
** Description:     This file is used to process msg to commonwin            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                 NAME              DESCRIPTION
** 03/04/2020           fangfang.yao          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "dal_time.h"
#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmi_theme.h"
#include "mmidisplay_data.h"
#include "mmi_default.h"
#include "guitext.h"
#include "guilabel.h"
#include "guibutton.h"
#ifdef ADULT_WATCH_SUPPORT
#include "guianim.h"
#endif
#include "mmicom_trace.h"
#include "watch_common.h"
#include "watch_commonwin_export.h"
#include "watch_commonwin_internal.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define WATCH_COMMON_CUSTOM_STR_LINE_SPACE   DP2PX_VALUE(1)
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION                                   */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description: set text style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetTextBoxStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      text_rect,
                                            MMI_STRING_T    *custom_str_ptr,
                                            MMI_IMAGE_ID_T  image_id
                                           )
{
    GUI_BG_T    textBg      = {0};
    GUI_FONT_T  tempFont    = GUI_MAX_FONT_NUM;
    GUI_COLOR_T fontColor   = WATCH_COMMONWIN_FONT_COLOR;

    if( image_id != 0 )
    {
        textBg.bg_type = GUI_BG_IMG;
        textBg.img_id  = image_id;
    }
    else
    {
        textBg.bg_type = GUI_BG_COLOR;
        textBg.color   = MMI_BLACK_COLOR;
    }
#if defined SCREEN_SHAPE_CIRCULAR
    tempFont = DP_FONT_22;
#elif defined SCREEN_SHAPE_SQUARE
    tempFont = WATCH_COMMONWIN_NORMAL_FONT;
#endif
    GUITEXT_SetAlign( ctrl_id, ALIGN_HVMIDDLE );

    if( PNULL != custom_str_ptr )
    {
        GUITEXT_SetString( ctrl_id, custom_str_ptr->wstr_ptr, custom_str_ptr->wstr_len, FALSE );
    }
    else
    {
        TRACE_APP_COMMON("custom_str_ptr is PNULL");
    }

    GUITEXT_SetFont( ctrl_id, &tempFont, &fontColor );
    GUITEXT_SetLineSpace( ctrl_id, WATCH_COMMON_CUSTOM_STR_LINE_SPACE);
    GUITEXT_SetBg( ctrl_id, &textBg);
    GUITEXT_SetRect( ctrl_id, &text_rect);
}
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description: set label style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetAnimStyle(
    MMI_CTRL_ID_T   ctrl_id,
    GUI_RECT_T      image_rect,
    MMI_IMAGE_ID_T  image_id
    )
{
    GUIANIM_CTRL_INFO_T control_info = {0};
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;

    data_info.anim_id = image_id;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color = MMI_BLACK_COLOR;
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    GUIANIM_SetCtrlRect(ctrl_id,&image_rect);
    GUIANIM_SetDisplayRect(ctrl_id,&image_rect,FALSE);
}
#endif
/*****************************************************************************/
//  Description: set label style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] text_rect : label display rect
//             [In] custom_str_ptr : label display string
//             [In] image_id : label background image,if not, default fill color
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetLableStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      text_rect,
                                            MMI_STRING_T    *custom_str_ptr,
                                            MMI_IMAGE_ID_T  image_id
                                           )
{
    GUI_BG_T    lableBg    = {0};
    GUI_FONT_T  tempFont   = GUI_MAX_FONT_NUM;

    if( image_id != 0 )
    {
        lableBg.bg_type = GUI_BG_IMG;
        lableBg.img_id  = image_id;
    }
    else
    {
        lableBg.bg_type = GUI_BG_COLOR;
        lableBg.color   = MMI_BLACK_COLOR;
    }

    tempFont = WATCH_COMMONWIN_NORMAL_FONT;

    GUILABEL_SetAlign( ctrl_id, GUILABEL_ALIGN_MIDDLE );

    if( PNULL != custom_str_ptr )
    {
        GUILABEL_SetText( ctrl_id, custom_str_ptr, TRUE );
    }
    else
    {
        TRACE_APP_COMMON("custom_str_ptr is PNULL");
    }

    GUILABEL_SetFont( ctrl_id, tempFont, WATCH_COMMONWIN_FONT_COLOR );
    GUILABEL_SetRect( ctrl_id, &text_rect, TRUE );
    GUILABEL_SetBg( ctrl_id, &lableBg );
    GUILABEL_SetMargin( ctrl_id, 0 );
}

/*****************************************************************************/
//  Description: set button style
//  Parameter: [In] ctrl_id : ctrl id
//             [In] btn_rect : button display rect
//             [In] btn_str : button display string
//             [In] btn_img : button display image
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_SetBtnStyle(
                                            MMI_CTRL_ID_T   ctrl_id,
                                            GUI_RECT_T      btn_rect,
                                            MMI_STRING_T    *btn_str_ptr,
                                            MMI_IMAGE_ID_T  btn_img
                                          )
{
    GUI_BG_T    btn_bg      = {0};
    GUI_FONT_T  tempFont    = GUI_MAX_FONT_NUM;

    if( btn_img == 0 )
    {
        btn_bg.bg_type  = GUI_BG_COLOR;
        btn_bg.color    = MMI_BLACK_COLOR;
    }
    else
    {
        btn_bg.bg_type = GUI_BG_IMG;
        btn_bg.img_id  = btn_img;
    }

    tempFont = WATCH_COMMONWIN_NORMAL_FONT;

    GUIBUTTON_SetBg( ctrl_id, &btn_bg );

    if( ( PNULL != btn_str_ptr ) && ( PNULL != btn_str_ptr->wstr_ptr ) && ( btn_str_ptr->wstr_len > 0 ))
    {
        GUIBUTTON_SetText( ctrl_id, btn_str_ptr->wstr_ptr, btn_str_ptr->wstr_len );
    }
    else
    {
        if( PNULL == btn_str_ptr )
        {
            TRACE_APP_COMMON("btn_str_ptr is PNULL");
        }
        else if( PNULL == btn_str_ptr->wstr_ptr )
        {
            TRACE_APP_COMMON("btn_str_ptr->wstr_ptr is PNULL");
        }
        else
        {
            TRACE_APP_COMMON("btn_str_ptr->wstr_len is %d",btn_str_ptr->wstr_len);
        }
    }

    GUIBUTTON_SetTextAlign( ctrl_id, ALIGN_HVMIDDLE );
    GUIBUTTON_SetRect( ctrl_id, &btn_rect );
    GUIBUTTON_SetFont( ctrl_id, &tempFont );
    GUIBUTTON_SetFontColor( ctrl_id, WATCH_COMMONWIN_FONT_COLOR);
}
#ifdef ADULT_WATCH_SUPPORT
/*****************************************************************************/
//  Description: create label ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: miao.xi
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateAnimCtrl(
    MMI_HANDLE_T    win_handle,
    MMI_CTRL_ID_T   ctrl_id
    )
{
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T    initData = {0};

    create.ctrl_id              = ctrl_id;
    create.guid                 = SPRD_GUI_ANIM_ID;
    create.parent_win_handle    = win_handle;
    create.init_data_ptr        = &initData;

    MMK_CreateControl( &create );
}
#endif
/*****************************************************************************/
//  Description: create label ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateLabelCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      )
{
    MMI_CONTROL_CREATE_T	create = {0};
    GUILABEL_INIT_DATA_T	initData = {0};

    create.ctrl_id              = ctrl_id;
    create.guid                 = SPRD_GUI_LABEL_ID;
    create.parent_win_handle    = win_handle;
    create.init_data_ptr        = &initData;

    MMK_CreateControl( &create );
}

/*****************************************************************************/
//  Description: create textbox ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateTextboxCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      )
{
    MMI_CONTROL_CREATE_T	create = {0};
    GUITEXT_INIT_DATA_T	initData = {0};

    create.ctrl_id              = ctrl_id;
    create.guid                 = SPRD_GUI_TEXTBOX_ID;
    create.parent_win_handle    = win_handle;
    create.init_data_ptr        = &initData;

    MMK_CreateControl( &create );
}

/*****************************************************************************/
//  Description: create button ctrl for window
//  Parameter: [In] win_handle : window handle
//             [In] ctrl_id : ctrl id
//             [In] guid : ctrl guid
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_CreateButtonCtrl(
                                        MMI_HANDLE_T    win_handle,
                                        MMI_CTRL_ID_T   ctrl_id
                                      )
{
    MMI_CONTROL_CREATE_T	create = {0};
    GUIBUTTON_INIT_DATA_T	initData = {0};

    create.ctrl_id              = ctrl_id;
    create.guid                 = SPRD_GUI_BUTTON_ID;
    create.parent_win_handle    = win_handle;
    create.init_data_ptr        = &initData;

    MMK_CreateControl( &create );
}

/*****************************************************************************/
//  Description: set commonwin layout
//  Parameter: [In] pTheme_ptr : commonwin sub theme info
//             [In] type : commonwin type
//             [Out] pTheme_ptr
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC void CommonWin_GetLayoutByType( WATCH_COMMONWIN_LAYOUT_T* pTheme_ptr,WATCH_COMMONWIN_TYPE_E type )
{
    switch(type)
    {
        case WATCH_COMMONWIN_TYPE_QUERY_1BTN:
        {
            GUI_RECT_T tipTempRect  = WATCH_QUERYWIN_TEXT_1BTN_RECT;
            GUI_RECT_T btnTempRect  = WATCH_QUERYWIN_BTN_1BTN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1Btn.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1Btn.btnRect, &btnTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_QUERY_1BTN_WITH2STR:
        {
            GUI_RECT_T tipTempRect1 = WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT1;
            GUI_RECT_T tipTempRect2  = WATCH_QUERYWIN_TEXT_1BTN_WITH2STR_RECT2;
            GUI_RECT_T btnTempRect  = WATCH_QUERYWIN_BTN_1BTN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1BtnWith2Str.tipRect1, &tipTempRect1,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1BtnWith2Str.tipRect2, &tipTempRect2,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1BtnWith2Str.btnRect, &btnTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_QUERY_2BTN:
        {
            GUI_RECT_T tipTempRect      = WATCH_QUERYWIN_TEXT_2BTN_RECT;
            GUI_RECT_T leftBtnTempRect  = WATCH_QUERYWIN_LEFTBTN_2BTN_RECT;
            GUI_RECT_T rightBtnTempRect = WATCH_QUERYWIN_RIGHTBTN_2BTN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.leftBtnRect, &leftBtnTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.rightBtnRect, &rightBtnTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_NOTE_FULLSCREEN:
        {
            GUI_RECT_T picTempRect  = WATCH_NOTEWIN_PIC_FULLSCREEN_RECT;
            GUI_RECT_T tipTempRect  = WATCH_NOTEWIN_TEXT_FULLSCREEN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNoteFullScreen.picRect, &picTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutNoteFullScreen.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_NOTE_1LINE:
        {
            GUI_RECT_T tipTempRect  = WATCH_NOTEWIN_TEXT_1LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNote1Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_NOTE_2LINE:
        {
            GUI_RECT_T tipTempRect  = WATCH_NOTEWIN_TEXT_2LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNote2Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_WAITING_FULLSCREEN:
        {
            GUI_RECT_T picTempRect  = WATCH_WAITINGWIN_PIC_FULLSCREEN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaitingFullScreen.picRect, &picTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case WATCH_COMMONWIN_TYPE_WAITING_1LINE:
        {
            GUI_RECT_T tipTempRect  = WATCH_WAITINGWIN_TEXT_1LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR:
        {
            GUI_RECT_T tipTempRect  = WATCH_WAITINGWIN_1ICON_1STR_TEXT_RECT;
            GUI_RECT_T picTempRect  = WATCH_WAITINGWIN_1ICON_1STR_PIC_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.picRect, &picTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case WATCH_COMMONWIN_TYPE_PROGRESS:
        {
            GUI_RECT_T top_img_rect      = WATCH_PROGRESSWIN_TOP_IMG_RECT;
            GUI_RECT_T progress_rect     = WATCH_PROGRESSWIN_PROGRESS_RECT;
            GUI_RECT_T bottom_img_rect   = WATCH_PROGRESSWIN_BOTTOM_IMG_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.topImgRect, &top_img_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.progressRect, &progress_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.bottomImgRect, &bottom_img_rect,sizeof(GUI_RECT_T) );
        }
        case WATCH_COMMONWIN_TYPE_PROGRESS_H:
        {
            GUI_RECT_T top_img_rect      = WATCH_PROGRESSWIN_TOP_IMG_RECT_H;
            GUI_RECT_T progress_rect     = WATCH_PROGRESSWIN_PROGRESS_RECT_H;
            GUI_RECT_T bottom_img_rect   = WATCH_PROGRESSWIN_BOTTOM_IMG_RECT_H;
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.topImgRect, &top_img_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.progressRect, &progress_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.bottomImgRect, &bottom_img_rect,sizeof(GUI_RECT_T) );
        }

		
        break;

#ifdef ADULT_WATCH_SUPPORT
        case ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN:
        {
            GUI_RECT_T btnTempRect  = ADULT_WATCH_QUERYWIN_BTN_1BTN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1Btn.btnRect, &btnTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case ADULT_WATCH_COMMONWIN_TYPE_QUERY_1BTN_1Str:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_QUERYWIN_TEXT_1BTN_1STR_RECT;
            GUI_RECT_T btnTempRect  = ADULT_WATCH_QUERYWIN_BTN_1BTN_1STR_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1Btn.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery1Btn.btnRect, &btnTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case ADULT_WATCH_COMMONWIN_TYPE_QUERY_2BTN:
        {
            GUI_RECT_T tipTempRect      = ADULT_WATCH_QUERYWIN_TEXT_2BTN_RECT;
            GUI_RECT_T leftBtnTempRect  = ADULT_WATCH_QUERYWIN_LEFTBTN_2BTN_RECT;
            GUI_RECT_T rightBtnTempRect = ADULT_WATCH_QUERYWIN_RIGHTBTN_2BTN_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.leftBtnRect, &leftBtnTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutQuery2Btn.rightBtnRect, &rightBtnTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case ADULT_WATCH_COMMONWIN_TYPE_WAITING_1LINE:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_WAITINGWIN_TEXT_1LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case ADULT_WATCH_COMMONWIN_TYPE_WAITING_1ICON_1STR:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_WAITINGWIN_1ICON_1STR_TEXT_RECT;
            GUI_RECT_T picTempRect  = ADULT_WATCH_WAITINGWIN_1ICON_1STR_PIC_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.picRect, &picTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case ADULT_WATCH_COMMONWIN_TYPE_WAITING_1STR_WAVEICON:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_WAITINGWIN_1STR_WAVEICON_TEXT_RECT;
            GUI_RECT_T picTempRect  = ADULT_WATCH_WAITINGWIN_1STR_WAVEICON_IMAGE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutWaiting1Icon1Str.picRect, &picTempRect,sizeof(GUI_RECT_T) );
        }
        break;
        case WATCH_COMMONWIN_TYPE_PROGRESS:
        {
            GUI_RECT_T top_img_rect      = WATCH_PROGRESSWIN_TOP_IMG_RECT;
            GUI_RECT_T progress_rect     = WATCH_PROGRESSWIN_PROGRESS_RECT;
            GUI_RECT_T bottom_img_rect   = WATCH_PROGRESSWIN_BOTTOM_IMG_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.topImgRect, &top_img_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.progressRect, &progress_rect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutProgress.bottomImgRect, &bottom_img_rect,sizeof(GUI_RECT_T) );
        }
        break;
#endif
#ifdef SCREEN_SHAPE_CIRCULAR
        case ADULT_WATCH_COMMONWIN_TYPE_NOTE_1ICON_1STR:
        {
            GUI_RECT_T picTempRect  = ADULT_WATCH_NOTEWIN_PIC_1ICON_1STR_RECT;
            GUI_RECT_T tipTempRect  = ADULT_WATCH_NOTEWIN_TEXT_1ICON_1STR_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNoteFullScreen.picRect, &picTempRect,sizeof(GUI_RECT_T) );
            SCI_MEMCPY( &pTheme_ptr->layoutNoteFullScreen.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case ADULT_WATCH_COMMONWIN_TYPE_NOTE_1LINE:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_NOTEWIN_TEXT_1LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNote1Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

        case ADULT_WATCH_COMMONWIN_TYPE_NOTE_2LINE:
        {
            GUI_RECT_T tipTempRect  = ADULT_WATCH_NOTEWIN_TEXT_2LINE_RECT;
            SCI_MEMCPY( &pTheme_ptr->layoutNote2Line.tipRect, &tipTempRect,sizeof(GUI_RECT_T) );
        }
        break;

#endif
        default:
            break;
    }

}

/*****************************************************************************/
//  Description: set commonwin sub theme info
//  Parameter: [In] input_str : the string which while be copied
//             [In] output_str_ptr : the output string
//             [Out] output_str_ptr
//             [Return] result : TRUE(copy success),FALSE(copy failed)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CommonWin_CopyStringInfo( MMI_STRING_T *input_str_ptr,MMI_STRING_T *output_str_ptr )
{
    BOOLEAN result = TRUE;
    if( PNULL != input_str_ptr )
    {
        if(( PNULL != input_str_ptr->wstr_ptr)&&(input_str_ptr->wstr_len > 0 ))
        {
            output_str_ptr->wstr_ptr = (wchar *)SCI_ALLOC_APP(sizeof(wchar)*WATCH_COMMON_CUSTOM_STR_LEN);
            if( PNULL == output_str_ptr->wstr_ptr )
            {
                result = FALSE;
                TRACE_APP_COMMON("output_str_ptr->wstr_ptr malloc fail");
                return result;
            }
            SCI_MEMSET(output_str_ptr->wstr_ptr, 0, sizeof(wchar)*WATCH_COMMON_CUSTOM_STR_LEN);

            SCI_MEMCPY(output_str_ptr->wstr_ptr, input_str_ptr->wstr_ptr,(sizeof(wchar)*(input_str_ptr->wstr_len + 1)));
            output_str_ptr->wstr_len = input_str_ptr->wstr_len;
        }
        else
        {
            if( PNULL == input_str_ptr->wstr_ptr )
            {
                TRACE_APP_COMMON("input_str_ptr->wstr_ptr is PNULL");
            }
            else
            {
                TRACE_APP_COMMON("input_str_ptr->wstr_len is %d",input_str_ptr->wstr_len);
            }
        }
    }
    else
    {
        TRACE_APP_COMMON("input_str_ptr is PNULL");
    }
    return result;
}

