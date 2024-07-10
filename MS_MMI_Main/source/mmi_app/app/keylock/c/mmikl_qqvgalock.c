/*****************************************************************************
** File Name:      mmikl_doublecirclelock.c                                  *
** Author:         michael.wang                                              *
** Date:           1/21/2012                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe move the little circle to   *
**                 unlock function                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 1/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIKL_DCLOCK_C_

#ifdef MMI_KEY_LOCK_EFFECT_QQVGA

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmikl_qqvgalock.h"
#include "mmikl_effectmain.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
//#include "mmi_keylock_position.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmi_id.h"
#include "ui_layer.h"
#include "guistring.h"
#include "guires.h"
#include "graphics_draw.h"
#include "mmikl_export.h"
#include "mmk_tp.h"
#include "mmk_timer.h"
#include "mmikl_internal.h"
#include "mmiset_export.h"
#include "chg_drvapi.h"
#include "mmiidle_export.h"
#include "Guistatusbar.h"
#include "mmicc_export.h"
#include "mmisms_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MAINLCD_SIZE_128X160
#define KEY_LOCK_QQVGA_TIME_TOP 30
#define KEY_LOCK_QQVGA_DATE_TOP 80
#define KEY_LOCK_QQVGA_MISSEVENT_TOP 80
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT 20
#else
#define KEY_LOCK_QQVGA_TIME_TOP 80
#define KEY_LOCK_QQVGA_DATE_TOP 110
#define KEY_LOCK_QQVGA_MISSEVENT_TOP 170
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT 50
#define KEY_LOCK_QQVGA_MISSEVENT_TOP_H 130
#define KEY_LOCK_QQVGA_MISSEVENT_LEFT_H 70
#define KEY_LOCK_QQVGA_IMAGE_SPACE  30
#endif
#define KEY_LOCK_QQVGA_SOFTKEY_TIMER 3000


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
LOCAL uint8 s_qqvga_softkey_timer = 0;
LOCAL MMI_KL_WIN_STATUS_E s_qqvga_mmi_kl_win_status = KL_LOCKED;


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDigitalClockDC()
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 img_width = 0;
    uint16 img_height =0;
    GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T rect = {0};

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_IDLE_TIME_0, MMIKL_CLOCK_DISP_WIN_ID);
    
    rect.left = 0;
    rect.right = lcd_width;
    rect.top = KEY_LOCK_QQVGA_TIME_TOP;
    rect.bottom = rect.top + img_height;
        
    MMIKL_DisplayDigitalClock(&(lcd_dev), rect, IMAGE_IDLE_TIME_0);
}
/*****************************************************************************/
//  Description : display date
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDateDC(void)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[16] = {0};
    uint8 datestr[16] = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    MMI_TEXT_ID_T week_table[] = {  
                                    TXT_SHORT_IDLE_SUNDAY,
                                    TXT_SHORT_IDLE_MONDAY,
                                    TXT_SHORT_IDLE_TUESDAY,
                                    TXT_SHORT_IDLE_WEDNESDAY,
                                    TXT_SHORT_IDLE_THURSDAY,
                                    TXT_SHORT_IDLE_FRIDAY,
                                    TXT_SHORT_IDLE_SATURDAY
                                 };
    
    SCI_MEMSET(m_buf,0,sizeof(m_buf));
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));   

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
    
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    
    strncat(datestr," ",1);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);
    
    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(final_string.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);
    
    
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = SONG_FONT_20;
    text_style.align = ALIGN_HMIDDLE;
 
    rect.left = 0;
    rect.right = lcd_width;
    rect.top = KEY_LOCK_QQVGA_DATE_TOP;
    rect.bottom = rect.top + text_style.font;                 
    
    GUISTR_DrawTextToLCDInRect( 
                                (const GUI_LCD_DEV_INFO *)(&lcd_dev),
                                (const GUI_RECT_T *)&rect,
                                (const GUI_RECT_T *)&rect,
                                (const MMI_STRING_T *)&final_string,
                                &text_style,
                                GUISTR_STATE_ALIGN,
                                GUISTR_TEXT_DIR_AUTO
                                );
}

#ifdef MAINLCD_SIZE_176X220 
/*****************************************************************************/
//  Description : display missed event
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL  void DisplayMissedEvent(MMI_WIN_ID_T win_id, GUI_RECT_T *rect, const GUI_LCD_DEV_INFO * dev_info_ptr)
{
    uint8   str_len = 0;
    uint32  missed_call = 0;
    uint32  missed_sms =0;       
    uint16 img_width = 0, img_height = 0;  
    wchar    missed_call_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    wchar    missed_sms_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)     
    uint32  missed_event =0;
    wchar    missed_event_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    MMI_STRING_T        missed_event_text = {0};
#endif
    char     count_str[KEYLOCK_COUNT_STRING_SIZE*8] = {0};
    //wchar   temp = '0';
    //wchar  count_str_doubulenum[KEYLOCK_COUNT_STRING_SIZE*8] = {0};
    MMI_STRING_T        missed_call_text = {0};
    MMI_STRING_T        missed_sms_text = {0};    
    GUISTR_STYLE_T event_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = *rect;
  
    missed_call = MMIAPICC_GetMissedCallNumber();
    missed_sms = MMIAPISMS_GetAllUnreadMsgCount()    
    + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
    ;

#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)  
    missed_event = MMIAPIALM_GetTotalTimesOfArrivedEvent();
#endif

#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)
    if(0 == missed_call && 0 == missed_sms && 0 == missed_event)
    {
        return;
    }
#endif    
    
    event_text_style.effect = FONT_EFFECT_REVERSE_EDGE; 
    event_text_style.angle = ANGLE_0;
    event_text_style.align = ALIGN_VMIDDLE;
    event_text_style.font = MMI_DEFAULT_SMALL_FONT;
    event_text_style.font_color = MMI_WHITE_COLOR;

    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_QQVGA_KEYLOCK_MISS_CALL, win_id);

#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)
    if(missed_call > 0)
    {
#endif    
        sprintf(count_str,"%d ",missed_call);
        str_len = strlen(count_str);
        MMI_STRNTOWSTR(missed_call_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8, str_len);
        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + missed_call_text.wstr_len + 1)
        {
            MMIAPICOM_Wstrncpy(missed_call_wstr + str_len, missed_call_text.wstr_ptr , missed_call_text.wstr_len);
        }
        missed_call_text.wstr_ptr = missed_call_wstr;
        missed_call_text.wstr_len = str_len + missed_call_text.wstr_len;

        MMIKL_DispImg(&disp_rect, win_id, IMAGE_QQVGA_KEYLOCK_MISS_CALL, dev_info_ptr, FALSE);
  
        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_call_text,
            &event_text_style,
            text_state,
            0
            );
#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)        
    }

    if(missed_sms > 0)
    {
#endif    
        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE*8);
        
        sprintf(count_str,"%d ",missed_sms);
        str_len = strlen(count_str);
        
        MMI_STRNTOWSTR(missed_sms_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8,str_len);
        
        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + missed_sms_text.wstr_len +1)
        {
            MMIAPICOM_Wstrncpy(missed_sms_wstr + str_len, missed_sms_text.wstr_ptr , missed_sms_text.wstr_len);
        }
        missed_sms_text.wstr_ptr = missed_sms_wstr;
        missed_sms_text.wstr_len = str_len + missed_sms_text.wstr_len;

        disp_rect = *rect;
#ifdef MAINLCD_SIZE_128X160        
        disp_rect.top += img_height + 2;
#else
        disp_rect.left += img_width+ KEY_LOCK_QQVGA_IMAGE_SPACE;
#endif

        MMIKL_DispImg(&disp_rect, win_id, IMAGE_QQVGA_KEYLOCK_MISS_MESSAGE, dev_info_ptr, FALSE);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_QQVGA_KEYLOCK_MISS_MESSAGE, win_id);

        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;     
        
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_sms_text,
            &event_text_style,
            text_state,
            0
            );
#if (!defined MAINLCD_SIZE_128X160) && (!defined MAINLCD_SIZE_176X220)        
    }

    if(missed_event > 0)
    {
        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE*8);
        
        sprintf(count_str,"%d ",missed_event);
        str_len = strlen(count_str);
        
        MMI_STRNTOWSTR(missed_event_wstr, KEYLOCK_COUNT_STRING_SIZE*8, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE*8,str_len);
        
        if (KEYLOCK_COUNT_STRING_SIZE*8 > str_len + missed_event_text.wstr_len +1)
        {
            MMIAPICOM_Wstrncpy(missed_event_wstr + str_len, missed_event_text.wstr_ptr , missed_event_text.wstr_len);
        }
        missed_event_text.wstr_ptr = missed_event_wstr;
        missed_event_text.wstr_len = str_len + missed_event_text.wstr_len;

#ifdef MAINLCD_SIZE_128X160        
        disp_rect.top += img_height + 2;
#else
        disp_rect.left += 2*(img_width+ KEY_LOCK_QQVGA_IMAGE_SPACE);
#endif

        MMIKL_DispImg(&disp_rect, win_id, IMAGE_QQVGA_KEYLOCK_NO, dev_info_ptr, FALSE);

        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_QQVGA_KEYLOCK_NO, win_id);

        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;     
        
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_event_text,
            &event_text_style,
            text_state,
            0
            );
    }
#endif    
}
#endif
#ifdef MAINLCD_SIZE_176X220 
/*****************************************************************************/
//  Description : display missed event
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayMissedEventDC(void)
{
    uint16 img_width = 0;
    uint16 img_height = 0;    
    uint16 lcd_width = 0, lcd_height = 0; 
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
#ifdef MAINLCD_SIZE_176X220
    BOOLEAN is_landscape = FALSE;
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_QQVGA_KEYLOCK_MISS_CALL, MMIKL_CLOCK_DISP_WIN_ID);

    rect.right = lcd_width - 1;         
    rect.bottom = rect.top + lcd_height;

#ifdef MAINLCD_SIZE_176X220    
    if(is_landscape)
    {
            rect.left = KEY_LOCK_QQVGA_MISSEVENT_LEFT_H;
            rect.top = KEY_LOCK_QQVGA_MISSEVENT_TOP_H;
    }
    else
    {
#endif    
            rect.left = KEY_LOCK_QQVGA_MISSEVENT_LEFT;
            rect.top = KEY_LOCK_QQVGA_MISSEVENT_TOP;
#ifdef MAINLCD_SIZE_176X220            
    }
#endif
   
    DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);
}
#endif
/*****************************************************************************/
//  Description : do 50% alpha blending for bmp in LCD buffer and a color
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 给背景蒙上一层透明色，防止白色背景影响显示效果
/*****************************************************************************/
LOCAL void DisplayShadowBGDC(void)
{        
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_COLOR_T main_color = MMI_BLACK_COLOR;
    GUI_LCD_DEV_INFO main_lcd_info = {0};
    GUI_RECT_T main_rect = {0};        
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    main_rect.left = 0;
    main_rect.top = 0;
    main_rect.right = lcd_width -1;
    main_rect.bottom = lcd_height -1;
    
    LCD_MistBmp(&main_lcd_info,main_rect,main_color);
}



/*****************************************************************************/
//  Description : full paint 显示函数
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
#if 0
LOCAL void DisplayFullPaintDC(void)
{        
    DisplayShadowBGDC();
    DisplayDateDC();
    DisplayMissedEventDC();
    
}
#endif
/*****************************************************************************/
//  Description : start tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartUnlockConfirmTimerQQVGA(void)
{
    START_TIMER(s_qqvga_softkey_timer, KEY_LOCK_QQVGA_SOFTKEY_TIMER);
}


/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopUnlockConfirmTimerQQVGA(void)
{
    STOP_TIMER(s_qqvga_softkey_timer);
}

/******************************************************************************/
//  Description : handle kl display window msg.
//  Global resource dependence : none
//  Author: michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleSpecQQVGAWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              uint16          msg_id, 
                                              DPARAM          param
                                              )
{   
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {   
            s_qqvga_mmi_kl_win_status = KL_LOCKED;        
        }        
        break;  
        
    case MSG_FULL_PAINT:  
        {
            DisplayShadowBGDC();
            DisplayDigitalClockDC();
            DisplayDateDC();
#ifdef MAINLCD_SIZE_176X220 
            DisplayMissedEventDC();
#endif
        }          
        break;        
        
    case MSG_APP_OK:
    case MSG_APP_MENU:
        {
           if ((MMIDEFAULT_IsBacklightOn()) &&(s_qqvga_mmi_kl_win_status == KL_LOCKED))
            {
                s_qqvga_mmi_kl_win_status = KL_UNLOCKED_ONCE;
                StartUnlockConfirmTimerQQVGA();
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, (MMI_TEXT_ID_T)TXT_COMMON_OK, TRUE);
                MMIDEFAULT_TurnOnBackLight();
            }       
            else if (KL_UNLOCKED_ONCE == s_qqvga_mmi_kl_win_status)
            {
                StopUnlockConfirmTimerQQVGA();
                StartUnlockConfirmTimerQQVGA();
            }
            else
            {
                 MMIDEFAULT_TurnOnBackLight();
            }
        }        
        break;
        
    case MSG_APP_CANCEL:
        {
           if ((MMIDEFAULT_IsBacklightOn()) && (KL_UNLOCKED_ONCE == s_qqvga_mmi_kl_win_status))
            {
                s_qqvga_mmi_kl_win_status = KL_UNLOCKED;
                MMK_CloseWin(win_id);               
            }
        }        
        break;       

        case MSG_TIMER:	
            {
                if (s_qqvga_softkey_timer == *(uint8*)param)
                {
                    //stop timer
                    StopUnlockConfirmTimerQQVGA();
                    
                    if (KL_UNLOCKED_ONCE == s_qqvga_mmi_kl_win_status)
                    {
                        s_qqvga_mmi_kl_win_status = KL_LOCKED;
                        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_IDLE_UNLOCK, TXT_NULL, TXT_NULL, TRUE);
                    }
                }
                
            }
        break;

    case MSG_CLOSE_WINDOW:
        {
            s_qqvga_mmi_kl_win_status = KL_UNLOCKED;
           StopUnlockConfirmTimerQQVGA();
        }             
        break;               
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    } 
    
    return recode;
}
#endif	//MMI_KEY_LOCK_EFFECT_QQVGA

#endif
