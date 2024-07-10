/*****************************************************************************
** File Name:   watch_heartrate.c                                            *
** Author:      longwei.qiao                                                 *
** Date:        2021/3/8                                                     *
** Copyright:                                                                *
** Description: This file is used to process msg to main window of heartrate *
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
#include "watch_common.h"
#include "mmicom_trace.h"
#include "ual_cms_export.h"
#include "watch_heartrate.h"
#include "watch_sensor_id.h"
#include "guires.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define HEARTRATE_DATA_RECT DP2PX_RECT(80,88,159,127)
#define HEARTRATE_TITLE_RECT DP2PX_RECT(0,30,240,49)
#define HEARTRATE_TIP_RECT DP2PX_RECT(0,128,239,147)

#define HEARTRATE_DISPLAY_LEN_MAX  4
#define HEARTRATE_TIP_LEN_MAX  20


typedef struct
{
    BOOLEAN is_measured;
    uint32  heartrate;
    uint64  timestamp;
}HEARTRATE_DATA_INFO_T;

/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**---------------------------------------------------------------------------*/
LOCAL HEARTRATE_DATA_INFO_T s_heartrate_data = {0};
LOCAL ual_sensor_handle_t s_heartrate_handle = 0;
/*---------------------------------------------------------------------------*
**                          LOCAL FUNCTION                                   *
**---------------------------------------------------------------------------*/

LOCAL void DisplayHeartRateWinBg(MMI_WIN_ID_T win_id)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T img_rect = {0};
    GUI_RECT_T clientRect = MMITHEME_GetClientRectEx(win_id);
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_FillRect(&lcd_dev_info, clientRect, MMI_BLACK_COLOR);
    GUIRES_GetImgWidthHeight(&img_width, &img_height,  res_launcher_ic_heart_rate, win_id);
    img_rect = GUI_GetCenterRect(clientRect, img_width, img_height);
    GUIRES_DisplayImg(PNULL,
                       &img_rect,
                       PNULL,
                       win_id,
                       res_launcher_ic_heart_rate,
                       &lcd_dev_info
                       );
}
LOCAL void DisplayHeartRateTimeTip(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T        tip_str    = {0};
    MMI_STRING_T        temp_str    = {0};
    GUI_RECT_T          tip_rect    =  HEARTRATE_TIP_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUISTR_STYLE_T      text_style   = {0};
    GUISTR_STATE_T      text_state   = GUISTR_STATE_ALIGN;
    MMI_TM_T            tm_old = {0};
    MMI_TM_T            tm_new = {0};
    uint32              cur_time = 0;
    MMI_TEXT_ID_T       text_id = TXT_NULL;
    char str_data[HEARTRATE_TIP_LEN_MAX + 1] = {0};
    wchar wstr_data[HEARTRATE_TIP_LEN_MAX + 1]= {0};
    uint16 wstr_len = 0;
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    cur_time = MMIAPICOM_GetCurTime();

    tm_new = MMIAPICOM_Second2Tm(cur_time);
    tm_old = MMIAPICOM_Second2Tm((uint32)(s_heartrate_data.timestamp/1000)/*ms->s*/);

    TRACE_APP_SENSOR(" new year = %d,  mon= %d, day = %d, hour = %d, min = %d, secend = %d",
                  tm_new.tm_year, tm_new.tm_mon, tm_new.tm_mday, tm_new.tm_hour, tm_new.tm_min, tm_new.tm_sec);
    TRACE_APP_SENSOR(" old year = %d,  mon= %d, day = %d, hour = %d, min = %d, secend = %d",
                  tm_old.tm_year, tm_old.tm_mon, tm_old.tm_mday, tm_old.tm_hour, tm_old.tm_min, tm_old.tm_sec);

    if(tm_new.tm_year - tm_old.tm_year > 0)
    {
        //tip: "%d years ago"
        sprintf(str_data, "%d ", tm_new.tm_year - tm_old.tm_year);
        text_id = TXT_HEART_RATE_TIME_YEARS;
        
    }
    else if(tm_new.tm_mon - tm_old.tm_mon > 0)
    {
        //tip: "%d months ago"
        sprintf(str_data, "%d ", tm_new.tm_mon - tm_old.tm_mon);
        text_id = TXT_HEART_RATE_TIME_MONTHS;
   }
    else if(tm_new.tm_mday - tm_old.tm_mday > 0)
    {
        //tip: "%d days ago"
        sprintf(str_data, "%d ", tm_new.tm_mday - tm_old.tm_mday);
        text_id = TXT_HEART_RATE_TIME_DAYS;
    }
    else if(tm_new.tm_hour - tm_old.tm_hour > 0)
   {
        //tip: "%d hours ago"
        sprintf(str_data, "%d ", tm_new.tm_hour - tm_old.tm_hour);
        text_id = TXT_HEART_RATE_TIME_HOURS;
    }
    else if(tm_new.tm_min - tm_old.tm_min > 0)
    {
        //tip: "%d minutes ago"
        sprintf(str_data, "%d ", tm_new.tm_min - tm_old.tm_min);
        text_id = TXT_HEART_RATE_TIME_MINUTES;
    }
    else if(tm_new.tm_sec - tm_old.tm_sec > 0)
    {
        //tip: "%d secends ago"
        sprintf(str_data, "%d ", tm_new.tm_sec - tm_old.tm_sec);
        text_id = TXT_HEART_RATE_TIME_SECENDS;
    }
    else
    {
        //do nothing
    }
    MMI_STRNTOWSTR(wstr_data,HEARTRATE_TIP_LEN_MAX,str_data,HEARTRATE_TIP_LEN_MAX,HEARTRATE_TIP_LEN_MAX);
    MMI_GetLabelTextByLang(text_id, &temp_str);
    wstr_len = MMIAPICOM_Wstrlen(wstr_data);
    MMIAPICOM_StrcatFromSTRINGToUCS2(wstr_data, &wstr_len, &temp_str);
    tip_str.wstr_ptr = wstr_data;
    tip_str.wstr_len = wstr_len;
    text_style.align      = ALIGN_HMIDDLE;
    text_style.font       = DP_FONT_20;
    text_style.font_color = AUDULT_GRAY_COLOR;
    //转换窗口坐标到显示坐标
    tip_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, tip_rect);
    GUISTR_DrawTextToLCDInRect(
    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
    &tip_rect,
    &tip_rect,
    &tip_str,
    &text_style,
    text_state,
    GUISTR_TEXT_DIR_AUTO);
}
/*****************************************************************************/
//  Description: draw heartrate info
//  Parameter: [In] win_id
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_DrawHeartRateInfo(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T          dataRect     = HEARTRATE_DATA_RECT;
    MMI_STRING_T        titlt_str    = {0};
    GUI_RECT_T          title_rect    =  HEARTRATE_TITLE_RECT;
    MMI_STRING_T        dis_data_str = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {0};
    GUISTR_STYLE_T      text_style   = {0};
    GUISTR_STATE_T      text_state   = GUISTR_STATE_ALIGN;
    MMI_TEXT_ID_T       title_text_id = TXT_NULL;
    BOOLEAN             is_history_data = FALSE;
    char str_data[HEARTRATE_DISPLAY_LEN_MAX + 1] = {0};
    wchar wstr_data[HEARTRATE_DISPLAY_LEN_MAX + 1]= {0};
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);

    if(FALSE == s_heartrate_data.is_measured)
    {
        title_text_id = TXT_HEART_RATE_MEASURING;
        is_history_data = TRUE;
    }
    else
    {
        title_text_id = TXT_HEART_RATE_CURRENT;
        is_history_data = FALSE;
    }
    //draw title
    MMI_GetLabelTextByLang(title_text_id, &titlt_str);

    text_style.align      = ALIGN_HVMIDDLE;
    text_style.font       = DP_FONT_20;
    text_style.font_color = MMI_WHITE_COLOR;
    //转换窗口坐标到显示坐标
    title_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, title_rect);
    GUISTR_DrawTextToLCDInRect(
    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
    &title_rect,
    &title_rect,
    &titlt_str,
    &text_style,
    text_state,
    GUISTR_TEXT_DIR_AUTO);
    //draw  heartrate data
    MMIAPICOM_Int2Str(s_heartrate_data.heartrate,str_data,HEARTRATE_DISPLAY_LEN_MAX);
    MMI_STRNTOWSTR(wstr_data,HEARTRATE_DISPLAY_LEN_MAX,str_data,HEARTRATE_DISPLAY_LEN_MAX,HEARTRATE_DISPLAY_LEN_MAX);
    dis_data_str.wstr_ptr = wstr_data;
    dis_data_str.wstr_len = MMIAPICOM_Wstrnlen(wstr_data,HEARTRATE_DISPLAY_LEN_MAX);
    //set font param
    text_style.align      = ALIGN_HVMIDDLE;
    text_style.font       = DP_FONT_40;
    text_style.font_color = MMI_WHITE_COLOR;
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
    //draw tip
    if(s_heartrate_data.heartrate != 0 && is_history_data)
    {
        DisplayHeartRateTimeTip(win_id);
    }
}

LOCAL BOOLEAN GetHeartRateCallBack(ual_cms_msg_t param)
{
    ual_sensor_heart_rate_t *heartrate_info_ptr = PNULL; 
    heartrate_info_ptr = (ual_sensor_heart_rate_t *)param.p_body;
    if(PNULL == heartrate_info_ptr)
    {
        return FALSE;
    }

    s_heartrate_data.heartrate = heartrate_info_ptr->heart_rate;
    s_heartrate_data.timestamp = heartrate_info_ptr->timestamp;
    TRACE_APP_SENSOR(" heartrate = %d ", s_heartrate_data.heartrate);
    AdultWatchHeartRate_SetMeasuredStatus(TRUE);
    if(MMK_IsFocusWin(WATCH_HEARTRATE_WIN_ID))
    {
        MMK_SendMsg(WATCH_HEARTRATE_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
    else if(MMK_IsFocusWin(WATCH_LAUNCHER_HEART_RATE_WIN_ID))
    {
        MMK_SendMsg(WATCH_LAUNCHER_HEART_RATE_WIN_ID, MSG_FULL_PAINT, PNULL);
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
PUBLIC BOOLEAN AdultWatchHeartRate_RegesterSensor(void)
{
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
    s_heartrate_handle = ual_sensorhub_register_sensor (UAL_SENSOR_TYPE_HEART_RATE, GetHeartRateCallBack);
#endif

    TRACE_APP_SENSOR(" s_heartrate_handle = %d ", s_heartrate_handle);
    if(s_heartrate_handle == 0)
    {
        return FALSE;
    }
#endif
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
PUBLIC BOOLEAN AdultWatchHeartRate_ActiveSensor(void)
{
    BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
    result = ual_sensorhub_active_sensor (s_heartrate_handle, UAL_SENSOR_DELAY_NORMAL/*ms*/, UAL_SENSOR_DELAY_NORMAL*10/*ms*/, UAL_SENHUB_NONWAKEUP);
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
PUBLIC BOOLEAN AdultWatchHeartRate_DeactiveSensor(void)
{
    BOOLEAN result = FALSE;
#ifndef WIN32
#ifdef UAL_SENSORHUB_SUPPORT
    result = ual_sensorhub_deactive_sensor (s_heartrate_handle);
    if(UAL_SENSORHUB_SUCCESS == result)
    {
        result = TRUE;
    }
#endif
#endif
    TRACE_APP_SENSOR(" result = %d ", result);
    return result;
}
/*****************************************************************************/
//  Description: get heartrate
//  Parameter: [In] is_measured
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC uint32 AdultWatchHeartRate_GetHeartrate(void)
{
    TRACE_APP_SENSOR(" heartrate= %d", s_heartrate_data.heartrate);
    return s_heartrate_data.heartrate;
}

/*****************************************************************************/
//  Description: set measure status
//  Parameter: [In] is_measured
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_SetMeasuredStatus(BOOLEAN is_measured)
{
    TRACE_APP_SENSOR(" is_measuring= %d", is_measured);
    s_heartrate_data.is_measured = is_measured;
}
LOCAL MMI_RESULT_E HandleMsgHeartRateWindow( MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            AdultWatchHeartRate_ActiveSensor();
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        break;
    case MSG_FULL_PAINT:
        {
            DisplayHeartRateWinBg(win_id);
            AdultWatchHeartRate_DrawHeartRateInfo(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        AdultWatchHeartRate_DeactiveSensor();
        AdultWatchHeartRate_SetMeasuredStatus(FALSE);
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

// window table of heart rate win
WINDOW_TABLE(WATCH_HEARTRATE_WIN_TAB) =
{
    WIN_HIDE_STATUS,
    WIN_ID(WATCH_HEARTRATE_WIN_ID),
    WIN_FUNC((uint32)HandleMsgHeartRateWindow),
    END_WIN
};

/*****************************************************************************/
//  Description: enter heart rate window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: longwei.qiao
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchHeartRate_MainWin_Enter( void )
{
    MMK_CreateWin((uint32 *)WATCH_HEARTRATE_WIN_TAB,PNULL);
}
