/*****************************************************************************
** File Name:   watch_steps.c                                                *
** Author:      longwei.qiao                                                 *
** Date:        2021/3/8                                                     *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of steps     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE                  NAME              DESCRIPTION                     *
** 03/08/2021             longwei.qiao         Create                        *
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/

#include "window_parse.h"
#include "mmi_appmsg.h"
#include "mmidisplay_data.h"
#include "mmk_type.h"
#include "mmiacc_id.h"
#include "mmiacc_nv.h"
#include "mmicom_trace.h"
#include "watch_commonwin_export.h"
#include "watch_steps.h"
#include "ual_cms_export.h"
#include "watch_sensor_id.h"
#include "guires.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define STEPS_DATA_RECT    DP2PX_RECT(0,72,239,119)
#define STEPS_RESTEPS_RECT DP2PX_RECT(80,182,159,201)

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL STEPS_DATA_INFO_T s_steps_data = {0};
LOCAL ual_sensor_handle_t s_steps_handle = 0;
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/
LOCAL void DisplayStepsWinBg(MMI_WIN_ID_T win_id)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T clientRect = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_FillRect(&lcd_dev_info, clientRect, MMI_BLACK_COLOR);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  res_launcher_ic_moving_object, win_id);
    img_rect = GUI_GetCenterRect(clientRect, img_width, img_height);
    GUIRES_DisplayImg(PNULL,
                       &img_rect,
                       PNULL,
                       win_id,
                       res_launcher_ic_moving_object,
                       &lcd_dev_info
                       );
}
LOCAL MMI_RESULT_E HandleReStepsQueryWinMSG( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;

    switch(msg_id)
    {
        case MSG_NOTIFY_CANCEL:
        {
            //clear  data
            do
            {
                STEPS_DATA_INFO_T steps_data = {0};
                s_steps_data.steps_count_sensor_history = s_steps_data.steps_count_sensor_history + s_steps_data.steps_count_display;
                s_steps_data.steps_count_display= 0;
            }while(0);
            MMK_CloseWin(win_id);
        }
        break;
        case MSG_NOTIFY_OK:
        {
            MMK_CloseWin(win_id);
        }
        break;
        default:
         break;
    }
    return recode;
}
LOCAL BOOLEAN GetStepsInfoCallBack(ual_cms_msg_t param)
{
    ual_sensor_step_counter_t *steps_info_ptr = PNULL; 
    steps_info_ptr = (ual_sensor_step_counter_t *)param.p_body;
    if(PNULL == steps_info_ptr)
    {
        return FALSE;
    }
    if(s_steps_data.steps_count_sensor_history == 0)
    {
        s_steps_data.steps_count_display = s_steps_data.steps_count_daily_history + steps_info_ptr->step_counter;
    }
    else
    {
        s_steps_data.steps_count_display = s_steps_data.steps_count_daily_history + steps_info_ptr->step_counter -  s_steps_data.steps_count_sensor_history;
    }
    TRACE_APP_SENSOR(" actual_steps_count = %d", s_steps_data.steps_count_display);
    TRACE_APP_SENSOR(" total_steps_count_daily = %d", s_steps_data.steps_count_sensor_history);
    TRACE_APP_SENSOR(" steps_count = %d", s_steps_data.steps_count_daily_history);
    // for display
    do
    {
         SCI_DATE_T	cur_date ={0};//MS curr date
         TM_GetSysDate(&cur_date);
         if(cur_date.mday != s_steps_data.date.mday 
            || cur_date.mon != s_steps_data.date.mon 
            || cur_date.year != s_steps_data.date.year)//不在同一天，清零
         {
               s_steps_data.steps_count_sensor_history = s_steps_data.steps_count_sensor_history + s_steps_data.steps_count_display;
               s_steps_data.steps_count_display= 0;
               s_steps_data.date.year = cur_date.year;
               s_steps_data.date.mon  = cur_date.mon;
               s_steps_data.date.mday = cur_date.mday;
         }
    }while(0);
    if(MMK_IsFocusWin(WATCH_STEPS_WIN_ID))
    {
        MMK_SendMsg(WATCH_STEPS_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else if(MMK_IsFocusWin(WATCH_LAUNCHER_MOVING_OBJECT_WIN_ID))
    {
        MMK_SendMsg(WATCH_LAUNCHER_MOVING_OBJECT_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else
    {
         //do nothing
    }
    return TRUE;
}
/*****************************************************************************/
//  Description: regester sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] TRUE/FALSE
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchSteps_RegesterSensor(void)
{
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
    s_steps_handle = ual_sensorhub_register_sensor (UAL_SENSOR_TYPE_STEP_COUNTER, GetStepsInfoCallBack);
#endif
#endif
    if(s_steps_handle == 0)
    {
        return FALSE;
    }
    return TRUE;
}
/*****************************************************************************/
//  Description: active sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchSteps_ActiveSensor(void)
{
    BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
    result = ual_sensorhub_active_sensor (s_steps_handle, 1000*1, 1000*10, UAL_SENHUB_NONWAKEUP);
    if(UAL_SENSORHUB_SUCCESS == result)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
#endif
#endif
    TRACE_APP_SENSOR(" result = %d ", result);
    return result;
}
/*****************************************************************************/
//  Description: deactive sensor
//  Parameter: [In] None
//             [Out] None
//             [Return] ual_sensorhub_res_e
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN AdultWatchSteps_DeactiveSensor(void)
{
    BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
   result =  ual_sensorhub_deactive_sensor (s_steps_handle);
   if(UAL_SENSORHUB_SUCCESS == result)
    {
        result = TRUE;
    }
#endif
#endif
   TRACE_APP_SENSOR("result = %d ", result);
   return result;
}
/*****************************************************************************/
//  Description: get steps count
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatchSteps_GetStepsCounter(void)
{
    TRACE_APP_SENSOR("steps_count_display = %d ", s_steps_data.steps_count_display);
    return s_steps_data.steps_count_display;
}
/*****************************************************************************/
//  Description: save steps info when power off
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_SaveInfoWhenPowerOff(void)
{
    SCI_DATE_T	tmp_date ={0};//MS curr date
    STEPS_DATA_INFO_T steps_data = {0};
    steps_data.steps_count_daily_history = s_steps_data.steps_count_display;
    TM_GetSysDate(&tmp_date);
    steps_data.date.year = tmp_date.year;
    steps_data.date.mon  = tmp_date.mon;
    steps_data.date.mday = tmp_date.mday;
    TRACE_APP_SENSOR("OFF:steps_data.date.year = %d, steps_data.date.mon = %d, steps_data.date.mday = %d", steps_data.date.year, steps_data.date.mon, steps_data.date.mday);
    MMINV_WRITE(MMINV_ACC_STEPS_INFO, &steps_data);
}
/*****************************************************************************/
//  Description: read steps info when power on
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_ReadInfoWhenPowerOn(void)
{
    MN_RETURN_RESULT_E nv_return = MN_RETURN_SUCCESS;
    SCI_DATE_T	cur_date ={0};//MS curr date
    STEPS_DATA_INFO_T steps_data = {0};
    MMINV_READ(MMINV_ACC_STEPS_INFO, &steps_data, nv_return);
    if (MN_RETURN_SUCCESS == nv_return)
    {
         TM_GetSysDate(&cur_date);
         TRACE_APP_SENSOR("ON:cur_date.year = %d, cur_date.mon = %d, cur_date.mday = %d", cur_date.year, cur_date.mon, cur_date.mday);
         if(cur_date.mday == steps_data.date.mday 
            && cur_date.mon == steps_data.date.mon 
            && cur_date.year == steps_data.date.year)//同一天
         {
               s_steps_data.steps_count_daily_history = steps_data.steps_count_daily_history;
         }
         else
         {
               s_steps_data.steps_count_daily_history = 0;//不在同一天，清零
         }
         s_steps_data.date.year = cur_date.year;
         s_steps_data.date.mon  = cur_date.mon;
         s_steps_data.date.mday = cur_date.mday;
    }
    else
    {
        steps_data.date.year = cur_date.year;
        steps_data.date.mon  = cur_date.mon;
        steps_data.date.mday = cur_date.mday;
        s_steps_data.steps_count_daily_history = 0;
        s_steps_data.steps_count_display = 0;
        s_steps_data.steps_count_sensor_history = 0;
        MMINV_WRITE(MMINV_ACC_STEPS_INFO, &steps_data);
    }
    s_steps_data.steps_count_display = s_steps_data.steps_count_daily_history;
}
/*****************************************************************************/
//  Description: buttin click event
//  Parameter: [In] point
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_ClickEvent(GUI_POINT_T  point)
{
	GUI_RECT_T resteps_rect = STEPS_RESTEPS_RECT;
    if (GUI_PointIsInRect(point, resteps_rect))
    {
        MMI_STRING_T tip_str = {0};
        WATCH_SOFTKEY_TEXT_ID_T softkey_text = {NULL ,NULL ,NULL};
        MMI_GetLabelTextByLang(TXT_MOVING_IS_RESTEPS, &tip_str);
        WatchCOM_QueryWin_2Btn_Enter(WATCH_STEPS_RESTEPS_WIN_ID,&tip_str, PNULL, PNULL, res_aw_ic_cancel ,res_aw_ic_done, softkey_text,HandleReStepsQueryWinMSG);
    }
}
/*****************************************************************************/
//  Description: draw steps info
//  Parameter: [In] win_id
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_DrawStepsInfo(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          dataRect       = STEPS_DATA_RECT;
    GUI_RECT_T          button_Rect    = STEPS_RESTEPS_RECT;
    MMI_STRING_T        dis_data_str   = {0};
    MMI_STRING_T        dis_button_str = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info   = {0};
    GUISTR_STYLE_T      text_style     = {0};
    GUISTR_STATE_T      text_state     = GUISTR_STATE_ALIGN;
    char str_data[STEPS_MAX_LENGTH +1] = {0};
    wchar wstr_data[STEPS_MAX_LENGTH +1]= {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    // for display
    do
    {
         SCI_DATE_T	cur_date ={0};//MS curr date
         TM_GetSysDate(&cur_date);
         if(cur_date.mday != s_steps_data.date.mday 
            || cur_date.mon != s_steps_data.date.mon 
            || cur_date.year != s_steps_data.date.year)//不在同一天，清零
         {
               s_steps_data.steps_count_display= 0;
         }
    }while(0);
    //get data
    MMIAPICOM_Int2Str(s_steps_data.steps_count_display,str_data,STEPS_MAX_LENGTH +1);
    MMI_STRNTOWSTR(wstr_data,STEPS_MAX_LENGTH +1,str_data,STEPS_MAX_LENGTH +1,STEPS_MAX_LENGTH +1);

    dis_data_str.wstr_ptr = wstr_data;
    dis_data_str.wstr_len = MMIAPICOM_Wstrnlen(wstr_data, STEPS_MAX_LENGTH +1);
    //set font param
    text_style.align      = ALIGN_HMIDDLE;
    text_style.font       = DP_FONT_48;
    text_style.font_color = AUDULT_BLUE1_COLOR;

    //转换窗口坐标到显示坐标
    dataRect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, dataRect);
    GUISTR_DrawTextToLCDInRect(
                      (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                      &dataRect,
                      &dataRect,
                      &dis_data_str,
                      &text_style,
                      text_state,
                      GUISTR_TEXT_DIR_AUTO
                      );

    /*draw button*/
    MMI_GetLabelTextByLang(TXT_MOVING_RESTEPS, &dis_button_str);
    //set font param
    text_style.align      = ALIGN_HMIDDLE;
    text_style.font       = DP_FONT_20;
    text_style.font_color = AUDULT_BLUE1_COLOR;

    //转换窗口坐标到显示坐标
    button_Rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, button_Rect);
    GUISTR_DrawTextToLCDInRect(
                      (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                      &button_Rect,
                      &button_Rect,
                      &dis_button_str,
                      &text_style,
                      text_state,
                      GUISTR_TEXT_DIR_AUTO
                      );
}

LOCAL MMI_RESULT_E HandleMsgStepsWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {

        }
        break;
    case MSG_FULL_PAINT:
        {
            DisplayStepsWinBg(win_id);
            AdultWatchSteps_DrawStepsInfo(win_id);
        }
        break;
        case MSG_TP_PRESS_UP:
        {
            GUI_POINT_T tp_point = {0};
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            AdultWatchSteps_ClickEvent(tp_point);
            break;
        }
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
/*****************************************************************************/
//  Description: get handle
//  Parameter: [In] None
//             [Out] None
//             [Return] s_steps_handle
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatch_GetStepsHandle(void)
{
     TRACE_APP_SENSOR("s_steps_handle = %d ", s_steps_handle);
     return s_steps_handle;
}
// window table of heart rate win
WINDOW_TABLE(WATCH_STEPS_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(WATCH_STEPS_WIN_ID),
    WIN_FUNC((uint32)HandleMsgStepsWindow),
    END_WIN
};

/*****************************************************************************/
//  Description: enter auto steps window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchSteps_MainWin_Enter( void )
{
    MMK_CreateWin((uint32 *)WATCH_STEPS_WIN_TAB,PNULL);
}

