/*****************************************************************************
** File Name:      mmirecord_typical_wintab.c                                        *
** Author:         bin.ji                                                    *
** Date:           3/1/2005                                                  *
** Copyright:      2005 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe recorder                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 3/1/2005       bin.ji           Create                                    *
** 8/11/2006      haiyang.hu       Modify                                    *
******************************************************************************/

#define _MMIRECORD_TYPICAL_WINTAB_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_type.h"
#include "window_parse.h"
#include "mmk_app.h"

#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "mmi_mainmenu_export.h"
#include "mmi_image.h"
#include "mmimultim_text.h"
#include "mmimultim_image.h"
#include "mmicom_panel.h"
#include "mmitheme_pos.h"

#include "guilistbox.h"
#include "guianim.h"
#include "guicommon.h"
#include "guilcd.h"
#include "guilabel.h"
#include "guiref_scale.h"
#include "guires.h"
#include "guiform.h"
#include "guibutton.h"
#include "guiownerdraw.h"

#include "mmirecord_id.h"
#include "mmirecord_image.h"
#include "mmirecord_text.h"
#include "mmirecord_menutable.h"
#include "mmirecord_position.h"
#include "mmirecord_export.h"
#include "mmirecord_wintablel.h"
#include "mmirecord_appcontrol.h"
#include "mmirecord_main.h"
#include "mmidisplay_data.h"

#include "mmisrvfmm_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordBG(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : display record button
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordButton(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : display record time
//  Global resource dependence : none
//  Author: haiyanghu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTime(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : SetNameLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetNameLabelText(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFormParam(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : SetPortraitFormParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetPortraitFormParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : SetLandscapeFormParam
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetLandscapeFormParam(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : VolumeButtonProc
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolumeButtonProc(void);

/*****************************************************************************/
//  Description : ListButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListButtonProc(void);

/*****************************************************************************/
//  Description : RecordButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordButtonProc(void);

/*****************************************************************************/
//  Description : StopButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StopButtonProc(void);

/*****************************************************************************/
//  Description : PlayButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PlayButtonProc(void);
/*****************************************************************************/
//  Description : update vol icon
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateVolIcon(MMI_HANDLE_T win_handle) ;

/*****************************************************************************/
//  Description : display DisplayAnimal
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAnimal(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display record softkey
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordSoftKey(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//  Description : SetFocusButtonNormal
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetFocusButtonNormal(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : SwitchFocusButton
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusButton(MMI_WIN_ID_T win_id,
                             DPARAM     param);

/**--------------------------------------------------------------------------*
**                          WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/

//the window for record main play

WINDOW_TABLE(MMIRECORD_MAINPLAY_WIN_CTRL_TAB) =
{

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIRECORD_MAINPLAY_FORM_CTRL_ID),

    CHILD_OWNDRAW_CTRL(FALSE, MMIRECORD_TIME_OWNDRAW_CTRL_ID, MMIRECORD_MAINPLAY_FORM_CTRL_ID, (uint32)DisplayRecordTime),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIRECORD_NAME_LABEL_CTRL_ID, MMIRECORD_MAINPLAY_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIRECORD_FORM_ANIM_CTRL_ID, MMIRECORD_MAINPLAY_FORM_CTRL_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_SBS, MMIRECORD_TIME_FORM_CTRL_ID, MMIRECORD_MAINPLAY_FORM_CTRL_ID),
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIRECORD_FORM_ANIM_H_CTRL_ID, MMIRECORD_TIME_FORM_CTRL_ID),

    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIRECORD_TIME_ORDER_FORM_CTRL_ID, MMIRECORD_TIME_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIRECORD_HSPACE_OWNDRAW_H_CTRL_ID, MMIRECORD_TIME_ORDER_FORM_CTRL_ID, NULL),
    CHILD_OWNDRAW_CTRL(FALSE, MMIRECORD_TIME_OWNDRAW_H_CTRL_ID, MMIRECORD_TIME_ORDER_FORM_CTRL_ID, (uint32)DisplayRecordTime),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIRECORD_NAME_LABEL_H_CTRL_ID, MMIRECORD_TIME_ORDER_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIRECORD_BUTTON_FORM_CTRL_ID, MMIRECORD_MAINPLAY_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_COMMON_PLAYER_BTN_VOL, MMIRECORD_VOLUME_BUTTON_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_COMMON_BTN_PLAY_GREY, MMIRECORD_PLAY_BUTTON_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_COMMON_BTN_STOP, MMIRECORD_STOP_BUTTON_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_RECORD_RECORD_BTN_UNSEL, MMIRECORD_RECORD_BUTTON_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(TRUE, IMAGE_COMMON_BTN_LIST, MMIRECORD_LIST_BUTTON_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),
    END_WIN
};


/*****************************************************************************/
//  Description : HandleWebKeyMsg
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKeyMsg(MMI_HANDLE_T win_handle)
{
    MMI_CTRL_ID_T            ctrl_id = 0;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST])
    {
        ListButtonProc();
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_STOP_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_PAUSE_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
        {
            MMK_PostMsg(win_handle, MSG_RECORD_RESUME_PLAY, PNULL, PNULL);
        }
        else
        {
            MMK_PostMsg(win_handle, MSG_RECORD_START_PLAY, PNULL, PNULL);
        }
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_STOP_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])
    {
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
        }
        else
        {
            MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);
        }
    }

    return MMI_RESULT_TRUE;

}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordBG(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO        lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T   display_rect = MMITHEME_GetClientRect();

    GUIRES_DisplayImg(PNULL,
                      &display_rect,
                      &display_rect,
                      win_id,
                      IMAGE_COMMON_BG,
                      &lcd_dev_info);

}

/*****************************************************************************/
//  Description : SetFocusButtonNormal
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetFocusButtonNormal(MMI_WIN_ID_T win_id)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);
    MMIRECORD_BUTTON_E      button = MMIRECORD_BUTTON_VOLUME;

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    for(button = MMIRECORD_BUTTON_VOLUME; button < MMIRECORD_BUTTON_MAX; button++)
    {
        if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[button])
        {
            record_win_data_ptr->button_validate[button] = MMIRECORD_BUTTON_STATE_NORMAL;
        }
    }

    return;

}
/*****************************************************************************/
//  Description : SwitchFocusButton
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SwitchFocusButton(MMI_WIN_ID_T win_id,
                             DPARAM     param)
{

    MMI_CTRL_ID_T            ctrl_id = 0;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == param
            || PNULL == record_win_data_ptr)
    {
        return;
    }

    ctrl_id = *(MMI_CTRL_ID_T*)param;

    switch(ctrl_id)
    {
    case MMIRECORD_PLAY_BUTTON_CTRL_ID:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }
        else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }

        break;

    case MMIRECORD_STOP_BUTTON_CTRL_ID:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }
        else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }

        break;

    case MMIRECORD_RECORD_BUTTON_CTRL_ID:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }
        else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }

        break;

    case MMIRECORD_LIST_BUTTON_CTRL_ID:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST])
        {
            SetFocusButtonNormal(win_id);
            record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST] = MMIRECORD_BUTTON_STATE_FOCUS;
            DisplayRecordButton(win_id);
        }

        break;

    default:
        break;
    }

}

/*****************************************************************************/
//  Description : Recorder Main Play win
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIRECORD_HandleRecordMainPlayWinMsg(
    MMI_WIN_ID_T      win_id,
    MMI_MESSAGE_ID_E    msg_id,
    DPARAM                param
)
{
    MMI_RESULT_E            recode  = MMI_RESULT_TRUE;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_CreateCtrlByWinTabEx(win_id, MMIRECORD_MAINPLAY_WIN_CTRL_TAB);

        if(!SetFormParam(win_id))
        {
            MMK_CloseWin(win_id);
        }

        MMIRECORD_UpdateButtonStatusReady(win_id);
        break;

    case MSG_FULL_PAINT:    //ÏÔÊ¾±³¾°Í¼£¬ÒôÁ¿µ÷½Ú¸Ë
        DisplayRecordBG(win_id);
        MMIRECORD_UpdateScreen(win_id, TRUE);
        break;

    case MSG_UPDATE_VOL_ICON:
        UpdateVolIcon(win_id);
        break;

    case MSG_RECORD_FILE_DELETED:
        MMIRECORD_UpdateButtonStatusReady(win_id);
        MMIRECORD_UpdateScreen(win_id, FALSE);
        break;

    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        HandleWebKeyMsg(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
                || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
                || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
                || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
        {
            MMK_PostMsg(win_id, MSG_STOP_RECORD_PLAY, PNULL, PNULL);
        }
        else
        {
            MMK_CloseWin(win_id);
        }

        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        SwitchFocusButton(win_id, param);
        break;


    case MSG_LCD_SWITCH:
        SetFormParam(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}


/*****************************************************************************/
//  Description : display DisplayAnimal
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayAnimal(MMI_WIN_ID_T win_id)
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUI_RECT_T                      anim_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_ptr = PNULL;
    control_info.ctrl_id = MMIRECORD_FORM_ANIM_CTRL_ID;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

    if(MMITHEME_IsMainScreenLandscape())
    {
        control_info.ctrl_id = MMIRECORD_FORM_ANIM_H_CTRL_ID;
        GUICTRL_GetRect(MMK_GetCtrlPtr(MMIRECORD_FORM_ANIM_H_CTRL_ID), &anim_rect);
    }
    else
    {
        control_info.ctrl_id = MMIRECORD_FORM_ANIM_CTRL_ID;
        GUICTRL_GetRect(MMK_GetCtrlPtr(MMIRECORD_FORM_ANIM_CTRL_ID), &anim_rect);
    }


    GUIRES_DisplayImg(PNULL,
                      &anim_rect,
                      &anim_rect,
                      win_id,
                      IMAGE_COMMON_BG,
                      &lcd_dev_info);



    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        data_info.img_id = IMAGE_RECORD_PLAY_BG;
        GUIANIM_SetParam(&control_info, &data_info, &file_info, &display_info);
        GUIANIM_UpdateDisplay(control_info.ctrl_id);
    }
    else
    {
        data_info.img_id = IMAGE_COMMON_RECORD_BG;
        GUIANIM_SetParam(&control_info, &data_info, &file_info, &display_info);
        GUIANIM_UpdateDisplay(control_info.ctrl_id);
    }
}


/*****************************************************************************/
//  Description : display record
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateScreen(MMI_WIN_ID_T win_id,
                                   BOOLEAN is_full_paint)
{

    if(!MMK_IsFocusWin(win_id))
    {
        if(!is_full_paint)
        {
            return;
        }
    }


    DisplayAnimal(win_id);
    DisplayRecordButton(win_id);
    DisplayRecordSoftKey(win_id);
    SetNameLabelText(win_id);

}

/*****************************************************************************/
//  Description : display record time
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordTime(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    /*lint -e529*/
    MMIRECORD_TIME_T  cur_time = {0};
    GUI_POINT_T disp_point = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    wchar time[10][2] = {L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9"};
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T      text_style      = {0};
    uint16 img_num_width = 0;
    uint16 img_point_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T rect = {0};

    if(PNULL == owner_draw_ptr)
    {
        return;
    }

    if(MMITHEME_IsMainScreenLandscape())
    {
        disp_point.x =  owner_draw_ptr->ctrl_rect.left + MMIRECORD_TIME_OWNDRAW_H_X;
        disp_point.y =  owner_draw_ptr->ctrl_rect.top + MMIRECORD_TIME_OWNDRAW_H_Y;
    }
    else
    {
        disp_point.x =  owner_draw_ptr->ctrl_rect.left + MMIRECORD_TIME_OWNDRAW_X;
        disp_point.y =  owner_draw_ptr->ctrl_rect.top + MMIRECORD_TIME_OWNDRAW_Y;
    }

    string.wstr_len = 1;

    text_style.font = MMIRECORD_TIME_FONT;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;


    cur_time = MMIRECORD_GetRecordTime();

    string.wstr_ptr = time[cur_time.hour_high];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + MMIRECORD_TIME_SPACE;
    string.wstr_ptr = time[cur_time.hour_low];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + MMIRECORD_TIME_SPACE;
    disp_point.y =  disp_point.y - 3;
    string.wstr_ptr = L":";
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + 9;
    disp_point.y =  disp_point.y + 3;
    string.wstr_ptr = time[cur_time.minute_high];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + MMIRECORD_TIME_SPACE;
    string.wstr_ptr = time[cur_time.minute_low];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + MMIRECORD_TIME_SPACE;
    disp_point.y =  disp_point.y - 3;
    string.wstr_ptr = L":";
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );


    disp_point.x = disp_point.x + 9;
    disp_point.y =  disp_point.y + 3;
    string.wstr_ptr = time[cur_time.second_high];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    disp_point.x = disp_point.x + MMIRECORD_TIME_SPACE;
    string.wstr_ptr = time[cur_time.second_low];
    GUISTR_DrawTextToLCDSingleLine(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_POINT_T      *)&disp_point,
        (const MMI_STRING_T     *)&string,
        (GUISTR_STYLE_T         *)&text_style,
        GUISTR_STATE_WORDBREAK
    );

    /*lint -e529*/
}

/*****************************************************************************/
//  Description : display record softkey
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordSoftKey(MMI_WIN_ID_T win_id)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr
            || !MMK_IsFocusWin(win_id))
    {
        return;
    }

    if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_PAUSE, TXT_NULL, (MMI_TEXT_ID_T)STXT_STOP, TRUE);

    }
    else if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_COMMON_COMMON_CONTINUE, TXT_NULL, (MMI_TEXT_ID_T)STXT_STOP, TRUE);
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)STXT_OPTION, TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, TRUE);
    }

}


/*****************************************************************************/
//  Description : UpdateButtonStatusPlaying
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusPlaying(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;
}


/*****************************************************************************/
//  Description : UpdateButtonStatusPlayPause
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusPlayPause(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;
}

/*****************************************************************************/
//  Description : UpdateButtonStatusRecording
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusRecording(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}

/*****************************************************************************/
//  Description : UpdateButtonStatusRecordPaused
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusRecordPaused(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}

/*****************************************************************************/
//  Description : MMIRECORD_UpdateButtonStatusStopped
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusStopped(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }


    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;
}

/*****************************************************************************/
//  Description : UpdateButtonStatusReady
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_UpdateButtonStatusReady(MMI_HANDLE_T win_handle)
{
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]  = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]    = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]    = MMIRECORD_BUTTON_STATE_GRAY;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]        = MMIRECORD_BUTTON_STATE_FOCUS;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]  = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]   = MMIRECORD_BUTTON_STATE_HIDE;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_OPTION]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST]        = MMIRECORD_BUTTON_STATE_NORMAL;
    record_win_data_ptr->button_validate[MMIRECORD_BUTTON_BACK]        = MMIRECORD_BUTTON_STATE_NORMAL;

}

/*****************************************************************************/
//  Description : display record button
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void DisplayRecordButton(MMI_HANDLE_T win_handle)
{
    MMIRECORD_BUTTON_E  btn_id = MMIRECORD_BUTTON_PLAY;
    MMI_CTRL_ID_T ctrl_id = MMIRECORD_VOLUME_BUTTON_CTRL_ID;
    GUI_BG_T buttonFg = {0};
    GUI_BG_T buttonBg = {0};
    GUI_BG_T buttonBg_press = {0};
    GUI_BG_T button_grey = {0};
    GUI_BG_T buttonFg_press = {0};
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    buttonBg.bg_type = GUI_BG_IMG;
    buttonFg.bg_type = GUI_BG_IMG;
    button_grey.bg_type = GUI_BG_IMG;
    buttonBg_press.bg_type = GUI_BG_IMG;
#ifndef MMI_RES_LOW_COST
    buttonBg.img_id = IMAGE_COMMON_BTN_FOCUS;
    buttonBg_press.img_id = IMAGE_COMMON_BTN_PRESS;
#endif

    if(!MMK_IsFocusWin(win_handle))
    {
        return;
    }

    for(btn_id = MMIRECORD_BUTTON_VOLUME; btn_id < MMIRECORD_BUTTON_MAX; btn_id ++)
    {
        if(record_win_data_ptr->button_validate[btn_id] == MMIRECORD_BUTTON_STATE_FOCUS)
        {
            buttonBg.bg_type = GUI_BG_IMG;
        }
        else
        {
            buttonBg.bg_type = GUI_BG_NONE;
        }

        ctrl_id = 0;

        switch(btn_id)
        {
        case MMIRECORD_BUTTON_PLAY://²¥·ÅÍ£Ö¹Â¼Òô
            buttonFg.img_id = IMAGE_COMMON_BTN_PLAY;
            button_grey.img_id = IMAGE_COMMON_BTN_PLAY_GREY;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
            {
                ctrl_id = MMIRECORD_PLAY_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_PAUSE:
            buttonFg.img_id = IMAGE_COMMON_BTN_PAUSE;
            button_grey.img_id = IMAGE_COMMON_BTN_PLAY_GREY;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
            {
                ctrl_id = MMIRECORD_PLAY_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_STOP:     //Í£Ö¹²¥·Å»òÕßÂ¼Òô
            buttonFg.img_id = IMAGE_COMMON_BTN_STOP;
            button_grey.img_id = IMAGE_COMMON_BTN_STOP_GREY;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
            {
                ctrl_id = MMIRECORD_STOP_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_RECORD_STOP:
            buttonFg.img_id = IMAGE_COMMON_BTN_STOP;
            button_grey.img_id = IMAGE_COMMON_BTN_STOP_GREY;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
            {
                ctrl_id = MMIRECORD_STOP_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_RECORD_PAUSE: //Â¼Òô
            buttonFg.img_id = IMAGE_COMMON_BTN_PAUSE;
            button_grey.img_id = IMAGE_RECORD_RECORD_BTN_DIS;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
            {
                ctrl_id = MMIRECORD_RECORD_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_RECORD:
            buttonFg.img_id = IMAGE_RECORD_RECORD_BTN_UNSEL;
            button_grey.img_id = IMAGE_RECORD_RECORD_BTN_DIS;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])
            {
                ctrl_id = MMIRECORD_RECORD_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_LIST:     //ÁÐ±íÂ¼ÒôÎÄ¼þ
            buttonFg.img_id = IMAGE_COMMON_BTN_LIST;
            button_grey.img_id = IMAGE_COMMON_BTN_LIST_GREY;

            if(MMIRECORD_BUTTON_STATE_HIDE != record_win_data_ptr->button_validate[MMIRECORD_BUTTON_LIST])
            {
                ctrl_id = MMIRECORD_LIST_BUTTON_CTRL_ID;
            }

            break;

        case MMIRECORD_BUTTON_VOLUME:     //ÒôÁ¿
            if(0 == MMIAPISET_GetMultimVolume())
            {
                buttonFg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
                button_grey.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_GREY;
            }
            else
            {
                buttonFg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
                button_grey.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_GREY;
            }

            ctrl_id = MMIRECORD_VOLUME_BUTTON_CTRL_ID;
            break;

        default:
            break;
        }

        if(0 != ctrl_id)
        {
            if(MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[btn_id]
                    || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[btn_id])
            {
                GUIBUTTON_SetFg(ctrl_id, &buttonFg);
                GUIBUTTON_SetPressedFg(ctrl_id, &buttonFg);
                GUIBUTTON_SetBg(ctrl_id, &buttonBg);
                GUIBUTTON_SetPressedBg(ctrl_id, &buttonBg_press);
                GUIBUTTON_SetGrayed(ctrl_id, FALSE, NULL, FALSE);
                GUIFORM_SetChildDisplay(MMIRECORD_BUTTON_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_NORMAL);

                if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[btn_id])
                {
                    MMK_SetAtvCtrl(win_handle, ctrl_id);
                }

            }
            else
            {
                GUIBUTTON_SetGrayedFg(ctrl_id, TRUE, &button_grey, FALSE);
                GUIFORM_SetChildDisplay(MMIRECORD_BUTTON_FORM_CTRL_ID, ctrl_id, GUIFORM_CHILD_DISP_GRAY);
            }

            GUIBUTTON_Update(ctrl_id);
        }
    }
}


/*****************************************************************************/
//  Description : MMIRECORD_LoadRecordMenu
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIRECORD_LoadRecordMenu(MMI_HANDLE_T parent_handle)
{
    MMI_CTRL_ID_T           ctrl_id     = MMIRECORD_STARTSTATUS_OPTIONMENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id    = MENU_RECORD_START_OPTION;
    MMI_MENU_ID_T           menu_id     = ID_RECORD_STORIGE_POS;
    GUIMENU_BUTTON_INFO_T   button_info                   = {0};
    GUIMENU_TITLE_INFO_T        title_info                      = {0};
    MMI_STRING_T title_str = {0};
    MMIFILE_DEVICE_E        device = MMI_DEVICE_UDISK;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};
    MMIRECORD_WORK_FORMAT_E   file_formattype = MMIRECORD_WORK_FORMAT_MAX;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(parent_handle);


    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    if(MMIRECORD_BUTTON_STATE_GRAY == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]
            && MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]
            || MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])
    {
        menu_id = ID_RECORD_START;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_STOPPED_PLAY;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_AGAIN;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_DELETE;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_INSERT;//recordwork
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        menu_id = ID_RECORD_SEND;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR)
                || MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT; //andy.he_record
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

    }
    else
    {
        menu_id = ID_RECORD_START;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_STOPPED_PLAY;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_AGAIN;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        menu_id = ID_RECORD_DELETE;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);

        if(record_win_data_ptr->is_insert_record)
        {
            menu_id = ID_RECORD_SEND;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
            menu_id = ID_RECORD_INSERT;//recordwork
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_SEND;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
            menu_id = ID_RECORD_INSERT;//recordwork
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR)
                || MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

    }

    if(MMIRECORD_IsExistValidateDevice())
    {
        menu_id = ID_RECORD_STORIGE_POS;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        group_id = MENU_RECORD_STORE_STATUS;

        for(device = MMI_DEVICE_UDISK; device < MMI_DEVICE_NUM; device++)
        {
            menu_id = ID_RECORD_UDISK + device - MMI_DEVICE_UDISK;

            if(MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(device), MMIAPIFMM_GetDevicePathLen(device)))
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
            }
            else
            {
                GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
            }
        }

        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.group_id = MENU_RECORD_STORE_STATUS;
        pop_item_info.menu_id = ID_RECORD_UDISK + MMIRECORD_GetRecordDeviceIndex();
        GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
    }
    else
    {
        menu_id = ID_RECORD_STORIGE_POS;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
    }


    //file-format style sub context menu  andy.he_record
    if(record_win_data_ptr->is_insert_record)
    {
        menu_id = ID_RECORD_STORIGE_FORMAT;
        group_id = MENU_RECORD_START_OPTION;
        GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
    }
    else
    {
        group_id = MENU_RECORD_STORE_FORMAT;

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_MP3))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        if(MMIRECORD_IsSupport(MMIRECORD_WORK_FORMAT_AMR))
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, TRUE);
        }
        else
        {
            menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
            GUIMENU_SetItemVisible(ctrl_id, group_id, menu_id, FALSE);
        }

        pop_item_info.is_static = TRUE;
        pop_item_info.ctrl_id = ctrl_id;
        pop_item_info.group_id = MENU_RECORD_STORE_FORMAT;
        file_formattype = MMIRECORD_GetRecordFileFormatType();

        if(MMIRECORD_WORK_FORMAT_MP3 == file_formattype)
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_MP3;
        }
        else if(MMIRECORD_WORK_FORMAT_AMR == file_formattype)
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_AMR;
        }
        else
        {
            pop_item_info.menu_id = ID_RECORD_STORIGE_FORMAT_WAV;
        }

        GUIMENU_SetPopItemStatus(TRUE, &pop_item_info);
    }
}

/*****************************************************************************/
//  Description : SetNameLabelText
//  Global resource dependence :
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void SetNameLabelText(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T cur_file_s = {0};
    MMI_CTRL_ID_T            ctrl_id = 0;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_id);

    if(PNULL == record_win_data_ptr)
    {
        return;
    }

    cur_file_s.wstr_ptr = record_win_data_ptr->record_file_name;
    cur_file_s.wstr_len = record_win_data_ptr->record_file_name_len;

    cur_file_s.wstr_len = MMIAPICOM_Wstrlen(cur_file_s.wstr_ptr);
    GUILABEL_SetText(MMIRECORD_NAME_LABEL_H_CTRL_ID, &cur_file_s, FALSE);
    GUILABEL_SetText(MMIRECORD_NAME_LABEL_CTRL_ID, &cur_file_s, FALSE);

    if(MMITHEME_IsMainScreenLandscape())
    {
        ctrl_id = MMIRECORD_NAME_LABEL_H_CTRL_ID;
        GUIOWNDRAW_Update(MMIRECORD_TIME_OWNDRAW_H_CTRL_ID);
    }
    else
    {
        ctrl_id = MMIRECORD_NAME_LABEL_CTRL_ID;
        GUIOWNDRAW_Update(MMIRECORD_TIME_OWNDRAW_CTRL_ID);
    }

    MMK_SendMsg(ctrl_id, MSG_CTL_PAINT, NULL);

}


/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetFormParam(MMI_WIN_ID_T win_id)
{
    /*lint -e529*/
    GUI_BG_T    FormBg = {0};

    //set form not support slide
    GUIFORM_IsSlide(MMIRECORD_MAINPLAY_FORM_CTRL_ID, FALSE);


    GUIBUTTON_PermitBorder(MMIRECORD_PLAY_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIRECORD_STOP_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIRECORD_RECORD_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIRECORD_LIST_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_PermitBorder(MMIRECORD_VOLUME_BUTTON_CTRL_ID, FALSE);

    GUIBUTTON_SetRunSheen(MMIRECORD_PLAY_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIRECORD_STOP_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIRECORD_RECORD_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIRECORD_LIST_BUTTON_CTRL_ID, FALSE);
    GUIBUTTON_SetRunSheen(MMIRECORD_VOLUME_BUTTON_CTRL_ID, FALSE);

    GUILABEL_SetFont(MMIRECORD_NAME_LABEL_CTRL_ID,  MMIRECORD_FILENAME_FONT, MMI_WHITE_COLOR);
    GUILABEL_SetFont(MMIRECORD_NAME_LABEL_H_CTRL_ID, MMIRECORD_FILENAME_FONT, MMI_WHITE_COLOR);
//@for light function modify
#if defined MAINLCD_SIZE_240X320
    FormBg.img_id = IMAGE_THEME_BG;
    FormBg.bg_type = GUI_BG_IMG;
    GUIFORM_SetBg(MMIRECORD_MAINPLAY_FORM_CTRL_ID, &FormBg);
#endif
    FormBg.img_id = IMAGE_COMMON_BTN_BG;
    FormBg.bg_type = GUI_BG_IMG;
    GUIFORM_SetBg(MMIRECORD_BUTTON_FORM_CTRL_ID, &FormBg);

    if(MMITHEME_IsMainScreenLandscape())
    {
        SetLandscapeFormParam(win_id);
    }
    else
    {
        SetPortraitFormParam(win_id);
    }

    GUIBUTTON_SetCallBackFunc(MMIRECORD_PLAY_BUTTON_CTRL_ID, PlayButtonProc);
    GUIBUTTON_SetCallBackFunc(MMIRECORD_STOP_BUTTON_CTRL_ID, StopButtonProc);
    GUIBUTTON_SetCallBackFunc(MMIRECORD_RECORD_BUTTON_CTRL_ID, RecordButtonProc);
    GUIBUTTON_SetCallBackFunc(MMIRECORD_LIST_BUTTON_CTRL_ID, ListButtonProc);
    GUIBUTTON_SetCallBackFunc(MMIRECORD_VOLUME_BUTTON_CTRL_ID, VolumeButtonProc);

    GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
    return TRUE;
    /*lint -e529*/
}
/*****************************************************************************/
//  Description : SetPortraitFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
void SetPortraitFormParam(MMI_WIN_ID_T win_id)
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    uint16                  height_title = 0;
    uint16                  hspace = 0;



    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_TIME_OWNDRAW_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_NAME_LABEL_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_FORM_ANIM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_TIME_FORM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIOWNDRAW_SetHeight(MMIRECORD_TIME_OWNDRAW_CTRL_ID, MMIRECORD_TIME_OWNDRAW_HEIGHT);

    GUIRES_GetImgWidthHeight(PNULL, &height_title, MMITHEME_GetWinTitleBar(), win_id);
#ifdef MAINLCD_SIZE_240X320
    hspace = MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE + 45 - height_title;
#else
    hspace = MMIRECORD_PORTRAIT_BUTTONFORM_HSPACE;
#endif
    GUIFORM_SetChildSpace(MMIRECORD_MAINPLAY_FORM_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID, NULL, &hspace);

    hspace = MMIRECORD_PORTRAIT_ANIM_HSPACE;
    GUIFORM_SetChildSpace(MMIRECORD_MAINPLAY_FORM_CTRL_ID, MMIRECORD_FORM_ANIM_CTRL_ID, NULL, &hspace);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIRECORD_FORM_ANIM_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    data_info.img_id = IMAGE_COMMON_RECORD_BG;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info, &data_info, &file_info, &display_info);

}
/*****************************************************************************/
//  Description : SetLandscapeFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
void SetLandscapeFormParam(MMI_WIN_ID_T win_id)
{
    /*lint -e529*/
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    uint16 hspace = 0;
    uint16                  space = 0;
    GUI_RECT_T          win_rect = {0};
    uint16                  win_width = 0;        /*lint -e529*/

    GUIFORM_CHILD_WIDTH_T   sbs = {0};


    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_TIME_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_TIME_OWNDRAW_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_NAME_LABEL_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
    GUIFORM_SetChildDisplay(MMIRECORD_MAINPLAY_FORM_CTRL_ID,
                            MMIRECORD_FORM_ANIM_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);

    hspace = MMIRECORD_LANDSCAPE_BUTTONFORM_HSPACE;
    GUIFORM_SetChildSpace(MMIRECORD_MAINPLAY_FORM_CTRL_ID, MMIRECORD_BUTTON_FORM_CTRL_ID, NULL, &hspace);

    hspace = MMIRECORD_LANDSCAPE_ANIM_HSPACE;
    GUIFORM_SetChildSpace(MMIRECORD_MAINPLAY_FORM_CTRL_ID, MMIRECORD_FORM_ANIM_H_CTRL_ID, NULL, &hspace);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIRECORD_FORM_ANIM_H_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
    data_info.img_id = IMAGE_COMMON_RECORD_BG;
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info, &data_info, &file_info, &display_info);
    GUIOWNDRAW_SetHeight(MMIRECORD_HSPACE_OWNDRAW_H_CTRL_ID, MMIRECORD_HSPACE_OWNDRAW_H_CTRL_HEIGHT);
    GUIOWNDRAW_SetHeight(MMIRECORD_TIME_OWNDRAW_H_CTRL_ID, MMIRECORD_TIME_OWNDRAW_HEIGHT);
    sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
    GUIRES_GetImgWidthHeight(&sbs.add_data, PNULL, IMAGE_COMMON_RECORD_BG, win_id);
    GUIFORM_SetChildWidth(MMIRECORD_TIME_FORM_CTRL_ID, MMIRECORD_FORM_ANIM_H_CTRL_ID, &sbs);

}
/*****************************************************************************/
//  Description : PlayButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E PlayButtonProc(void)
{
    MMI_HANDLE_T  win_handle = MMIRECORD_MAINPLAY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_PAUSE_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_RESUME_PLAY, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_PLAY])
    {
        MMK_PostMsg(win_handle, MSG_RECORD_START_PLAY, PNULL, PNULL);
    }

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : StopButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E StopButtonProc(void)
{
    MMI_HANDLE_T  win_handle = MMIRECORD_MAINPLAY_WIN_ID;

    MMK_PostMsg(win_handle, MSG_STOP_RECORD_PLAY, PNULL, PNULL);

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : RecordButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RecordButtonProc(void)
{
    MMI_HANDLE_T  win_handle = MMIRECORD_MAINPLAY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);


    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_PAUSE])
    {
        MMK_PostMsg(win_handle, MSG_PAUSE_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD_STOP])
    {
        MMK_PostMsg(win_handle, MSG_RESUME_RECORD, PNULL, PNULL);
    }
    else if(MMIRECORD_BUTTON_STATE_FOCUS == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_RECORD])
    {
        MMK_PostMsg(win_handle, MSG_START_RECORD, PNULL, PNULL);

    }

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : ListButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E ListButtonProc(void)
{
    MMI_HANDLE_T  win_handle = MMIRECORD_MAINPLAY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    MMIRECORD_OpenRecordFileList();
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : VolumeButtonProc
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolumeButtonProc(void)
{
    MMI_HANDLE_T  win_handle = MMIRECORD_MAINPLAY_WIN_ID;
    RECORD_WIN_DATA_T       *record_win_data_ptr = (RECORD_WIN_DATA_T *)MMK_GetWinUserData(win_handle);

    if(PNULL == record_win_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if(MMIRECORD_BUTTON_STATE_FOCUS ==  record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME]
            || MMIRECORD_BUTTON_STATE_NORMAL == record_win_data_ptr->button_validate[MMIRECORD_BUTTON_VOLUME])
    {
        MMIRECORD_TurnVol(MMICOM_PANEL_OPEN_NONE);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : update vol icon
//  Global resource dependence : none
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
LOCAL void UpdateVolIcon(MMI_HANDLE_T win_handle)
{


    GUI_BG_T buttonFg = {0};
    GUI_BG_T buttonBg = {0};

    buttonFg.bg_type = GUI_BG_IMG;

    if(0 == MMIAPISET_GetMultimVolume())
    {
        buttonFg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
    }
    else
    {
        buttonFg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
    }

    GUIBUTTON_SetFg(MMIRECORD_VOLUME_BUTTON_CTRL_ID, &buttonFg);
    buttonBg.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIRECORD_VOLUME_BUTTON_CTRL_ID, &buttonBg);

    if(MMK_IsFocusWin(win_handle))
    {
        GUIBUTTON_Update(MMIRECORD_VOLUME_BUTTON_CTRL_ID);
    }

}






/*Edit by script, ignore 2 case. Thu Apr 26 19:01:05 2012*/ //IGNORE9527


/*Edit by script, ignore 1 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
