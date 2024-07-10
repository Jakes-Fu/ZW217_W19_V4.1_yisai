/*****************************************************************************
** File Name:                                              *
** Author:                                                *
** Date:                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:    This file is used to describe punctuation                 *
*******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "watch_launcher_common_panel_item.h"
#include "guictrl.h"
#include "guibutton.h"
#include "mmk_type.h"

#include "graphics_draw.h"
#include "guistring.h"
#include "mmiphone_export.h"
#include "chg_drvapi.h"
#ifdef ADULT_WATCH_SUPPORT
#include "watch_steps.h"
#include "watch_heartrate.h"
#endif

//#include "watch_launcher_common.h"
//#include "mmicom_trace.h"
/**--------------------------------------------------------------------------*
 **                         RECT DEFINITION                                  *
 **--------------------------------------------------------------------------*/


#define RGB888_TO_RGB565(r, g, b)  (((r << 8) & 0xf800) | ((g << 3) & 0x07e0) | ((b >> 3) & 0x1f))


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
LOCAL void GetStrByType(WATCH_PAN_CONTROLLER_DATA_TYPE_E type, MMI_STRING_T *str);
LOCAL void GetStrByType(WATCH_PAN_CONTROLLER_DATA_TYPE_E type, MMI_STRING_T *str)
{
char text[4] = {0};
int batLevel = 0;

switch (type)
{
    case WATCH_PAN_CONTROLLER_DATA_TYPE_STEP_NUMBER:
        {
             char str_data[STEPS_MAX_LENGTH +1] = {0};
             wchar wstr_data[STEPS_MAX_LENGTH +1]= {0};
             MMIAPICOM_Int2Str(AdultWatchSteps_GetStepsCounter(),str_data,STEPS_MAX_LENGTH +1);
             MMI_STRNTOWSTR(wstr_data,STEPS_MAX_LENGTH +1,str_data,STEPS_MAX_LENGTH +1,STEPS_MAX_LENGTH +1); 
             MMIAPICOM_Wstrncpy(str->wstr_ptr,wstr_data,MMIAPICOM_Wstrlen(wstr_data));
             str->wstr_len = MMIAPICOM_Wstrnlen(wstr_data, STEPS_MAX_LENGTH +1);
        }
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_STAND_NUMBER:
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_HEART_RATE:
        {
             char str_data[STEPS_MAX_LENGTH +1] = {0};
             wchar wstr_data[STEPS_MAX_LENGTH +1]= {0};
             MMIAPICOM_Int2Str(AdultWatchHeartRate_GetHeartrate(),str_data,STEPS_MAX_LENGTH);
             MMI_STRNTOWSTR(wstr_data,STEPS_MAX_LENGTH,str_data,STEPS_MAX_LENGTH,STEPS_MAX_LENGTH); 
             MMIAPICOM_Wstrncpy(str->wstr_ptr,wstr_data,MMIAPICOM_Wstrlen(wstr_data));
             str->wstr_len = MMIAPICOM_Wstrnlen(wstr_data, STEPS_MAX_LENGTH);
        }
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_CALORIE:
        {
            wchar wtext[2] = {'0',0};
            str->wstr_len = 1;
            MMIAPICOM_Wstrncpy(str->wstr_ptr,wtext,MMIAPICOM_Wstrlen(wtext));
            //str->wstr_ptr = wtext;
        }
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_CHARGE:
        {
            wchar wtext[4] = {'0','0','0',0};
            CHGMNG_STATE_INFO_T* p_chgmng_info = CHGMNG_GetModuleState();

            batLevel = p_chgmng_info->bat_remain_cap;
            sprintf(text, "%d", batLevel);
            MMIAPICOM_StrToWstr(text, wtext);
            MMIAPICOM_Wstrncpy(str->wstr_ptr,wtext,MMIAPICOM_Wstrlen(wtext));
            //str->wstr_ptr = wtext;
            str->wstr_len = MMIAPICOM_Wstrlen(wtext);
        }
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_DAY:
        {
            SCI_DATE_T s_sys_date = {0};
            char day_buffer[3] = {0};
            wchar wday_buffer[3] = {0};
            TM_GetSysDate(&s_sys_date);
            sprintf(day_buffer, "%02d", s_sys_date.mday);
            MMIAPICOM_StrToWstr(day_buffer, wday_buffer);
            MMIAPICOM_Wstrncpy(str->wstr_ptr,wday_buffer,MMIAPICOM_Wstrlen(wday_buffer));
            str->wstr_len = MMIAPICOM_Wstrlen(wday_buffer);
        }
        break;
    case WATCH_PAN_CONTROLLER_DATA_TYPE_WEEK:
        {
            SCI_DATE_T s_sys_date = {0};
            MMI_STRING_T weektextStr ={0};
            TM_GetSysDate(&s_sys_date);
            weektextStr = WatchLAUNCHER_GetWeekString(s_sys_date.wday);
            MMIAPICOM_Wstrncpy(str->wstr_ptr,weektextStr.wstr_ptr,weektextStr.wstr_len);
            str->wstr_len = weektextStr.wstr_len;
        }
        break;
    default:
        break;
}
}

/*****************************************************************************/
//  Discription: 
//  Global 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void AdultWatchCOM_PanController(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO lcd_dev_info, WATCHCOM_PAN_DATA_T data)
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STYLE_T text1_style = {0};
    GUISTR_STYLE_T text2_style = {0};
    GUISTR_STATE_T state = {0};
    GUI_RECT_T imgRect = {0};
    GUI_RECT_T strRect = {0};
    GUI_RECT_T str1Rect = {0};
    GUI_RECT_T str2Rect = {0};
    MMI_STRING_T strText1 = {0};
    MMI_STRING_T strText2 = {0};
    MMI_STRING_T strText = {0};
    wchar wtext[10] = {0};
    WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T *controllerData = PNULL;
    WATCH_PAN_CONTROLLER_DATA_2STR_T *controllerData_2str = PNULL;

    text_style.align = ALIGN_HVMIDDLE;
    text_style.font = DP_FONT_14;
    text_style.font_color = MMI_WHITE_COLOR;

    text1_style.align = ALIGN_HVMIDDLE;
    text1_style.font = DP_FONT_26;
    text1_style.font_color = RGB888_TO_RGB565(0xe0,0xff,0x0);

    text2_style.align = ALIGN_HVMIDDLE;
    text2_style.font = DP_FONT_16;
    text2_style.font_color = MMI_WHITE_COLOR;
    switch(data.controllerType)
    {
        case WATCH_PAN_CONTROLLER_TYPE_1STR_1IMG:

            controllerData = (WATCH_PAN_CONTROLLER_DATA_1STR_1IMG_T*)(data.controllerData);
            imgRect.top = data.rect.top + controllerData->rectImg.top;
            imgRect.left = data.rect.left + controllerData->rectImg.left;
            imgRect.bottom = data.rect.top + controllerData->rectImg.bottom;
            imgRect.right = data.rect.left + controllerData->rectImg.right;

            strRect.top = data.rect.top + controllerData->strRect.top;
            strRect.left = data.rect.left + controllerData->strRect.left;
            strRect.bottom = data.rect.top + controllerData->strRect.bottom;
            strRect.right = data.rect.left + controllerData->strRect.right;

            GUIRES_DisplayImg(NULL, &(imgRect), NULL, win_id, controllerData->imgID, &lcd_dev_info);
            strText.wstr_ptr = wtext;
            GetStrByType(controllerData->strType, &strText);
            GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&(strRect),
            (const GUI_RECT_T      *)&(strRect),
            (const MMI_STRING_T    *)&(strText),
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
            break;
        case WATCH_PAN_CONTROLLER_TYPE_2STR:

            controllerData_2str = (WATCH_PAN_CONTROLLER_DATA_2STR_T*)(data.controllerData);

            str1Rect.top = data.rect.top + controllerData_2str->rect1.top;
            str1Rect.left = data.rect.left + controllerData_2str->rect1.left;
            str1Rect.bottom = data.rect.top + controllerData_2str->rect1.bottom;
            str1Rect.right = data.rect.left + controllerData_2str->rect1.right;

            str2Rect.top = data.rect.top + controllerData_2str->rect2.top;
            str2Rect.left = data.rect.left + controllerData_2str->rect2.left;
            str2Rect.bottom = data.rect.top + controllerData_2str->rect2.bottom;
            str2Rect.right = data.rect.left + controllerData_2str->rect2.right;



            strText1.wstr_ptr = wtext;

            GetStrByType(controllerData_2str->str1Type, &strText1);
            GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&(str1Rect),
            (const GUI_RECT_T      *)&(str1Rect),
            (const MMI_STRING_T    *)&(strText1),
            &text1_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
            strText2.wstr_ptr = wtext;
            GetStrByType(controllerData_2str->str2Type, &strText2);
            GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
            (const GUI_RECT_T      *)&(str2Rect),
            (const GUI_RECT_T      *)&(str2Rect),
            (const MMI_STRING_T    *)&(strText2),
            &text2_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );
            break;
        default:
            break;
    }
}



