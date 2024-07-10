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
** 31/3/2012        michael.wang         Create
******************************************************************************/

#ifndef _MMIKL_DCLOCK_C_

#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmikl_specdoublecirclelock.h"
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

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define KEY_LOCK_DC_TP_MOVE_TIME   20
#define KEY_LOCK_DC_BIG_LAYER_ANIM_TIME   100//100
#define KEY_LOCK_DC_LOCKRELEASE_TIME   50//100
#define KEY_LOCK_DC_TRIANGLE_ANIM_TIME   200//100
#define KEYLOCK_COUNT_STRING_SIZE_DC (KEYLOCK_COUNT_STRING_SIZE * 8)
#define KEY_LOCK_DC_BIG_CIRCLE_EXTEND_STEP 5
#define KEY_LOCK_UILAYER_ALPHA_MAX  255
#define KEY_LOCK_UILAYER_ALPHA_STEP 5
#define KEY_LOCK_MOVE_START_DISTANCE 20
#define KEY_LOCK_MOVE_INTERVAL_NUM 20
#define KEY_LOCK_TP_UP_ANIM_TIMES 6
#define KEY_LOCK_TRIANGLE_ANIM_TIMES_REVERSE 8
#define KEY_LOCK_TRIANGLE_ANIM_TIMES_NORMAL 6
#define KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES 4
//#define KEY_LOCK_TIP_TOP    30
#define KEY_LOCK_DISPLAY_LEFT_SCREEN_OFFSET    10
#define KEY_LOCK_NETWORK_STRING_TOP    290
#define KEY_LOCK_NETWORK_STRING_BOTTOM    310
#define KEY_LOCK_NETWORK_INTERVAL    18
#define KEY_LOCK_TIP_TOP    40
#define KEY_LOCK_TIP_BOTTOM    60
#define KEY_LOCK_DIGITALCLOCK_NUM    6 //CR140373
#define KEY_LOCK_DIGITALCLOCK_TOP    350
#define KEY_LOCK_DATA_TOP    430
#define KEY_LOCK_DATA_BOTTOM    470

#ifndef MAINLCD_SIZE_320X480
//the info about big and little circle, the same part of 240x400 and 240x320
#define SPEC_DC_BIG_CIRCLE_D     148     //大圆直径
#define SPEC_DC_BIG_CIRCLE_R     (SPEC_DC_BIG_CIRCLE_D/2)
#define SPEC_DC_LITTLE_CIRCLE_D      60  //小圆直径
#define SPEC_DC_LITTLE_CIRCLE_R      (SPEC_DC_LITTLE_CIRCLE_D/2)
#define SPEC_DC_LCD_WINTH_V      240  //竖屏屏幕宽度
#else
//for 320x480
#define SPEC_DC_BIG_CIRCLE_D     230     //大圆直径          //214
#define SPEC_DC_BIG_CIRCLE_R     (SPEC_DC_BIG_CIRCLE_D/2)
#define SPEC_DC_LITTLE_CIRCLE_D      230  //小圆直径          //94
#define SPEC_DC_LITTLE_CIRCLE_R      (SPEC_DC_LITTLE_CIRCLE_D/2)
#define SPEC_DC_LCD_WINTH_V      320  //竖屏屏幕宽度
#endif

#ifdef MAINLCD_SIZE_240X400
//the info about the location of each image for vertical 
#define SPEC_DC_TIME_TOP_V       46
#define SPEC_DC_TIME_BOTTOM_V       (SPEC_DC_TIME_TOP_V + 32)
#define SPEC_DC_DATE_TOP_V       90
#define SPEC_DC_DATE_FONT_HEIGHT 14 
#define SPEC_DC_MISSEVENT_TOP 320

//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define SPEC_DC_TIME_TOP_H       18
#define SPEC_DC_DATE_TOP_H      65
#define SPEC_DC_LCD_WINTH_H    400
#define SPEC_DC_MISSEVENT_TOP_H 180
#endif

#elif defined MAINLCD_SIZE_240X320
//the info about the location of each image for vertical 
#define SPEC_DC_TIME_TOP_V       40
#define SPEC_DC_TIME_BOTTOM_V       (SPEC_DC_TIME_TOP_V + 32)
#define SPEC_DC_DATE_TOP_V       84
#define SPEC_DC_DATE_FONT_HEIGHT 14 
#define SPEC_DC_MISSEVENT_TOP  250
//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define SPEC_DC_TIME_TOP_H       17
#define SPEC_DC_DATE_TOP_H      61
#define SPEC_DC_LCD_WINTH_H    320
#define SPEC_DC_MISSEVENT_TOP_H 180
#endif

#else //for 320x480
//the info about the location of each image for vertical
#define SPEC_DC_TIME_TOP_V      60 
#define SPEC_DC_TIME_BOTTOM_V       (SPEC_DC_TIME_TOP_V + 36)
#define SPEC_DC_DATE_TOP_V       109
#define SPEC_DC_DATE_FONT_HEIGHT 30
#define SPEC_DC_MISSEVENT_TOP 100

//the info about the location of each image for horizontal
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
#define SPEC_DC_TIME_TOP_H       21
#define SPEC_DC_DATE_TOP_H      69
#define SPEC_DC_LCD_WINTH_H    480
#define SPEC_DC_MISSEVENT_TOP_H 250
#endif

#endif


#define MMI_KL_SPECDC_CREATE_LAYER(create_info, win_handle, layer_width, layer_height, layer, result) \
{   \
    create_info.lcd_id = GUI_MAIN_LCD_ID;   \
    create_info.owner_handle = win_handle;  \
    create_info.offset_x = 0;   \
    create_info.offset_y = 0;   \
    create_info.width = layer_width;  \
    create_info.height = layer_height;    \
    create_info.is_bg_layer = FALSE;    \
    create_info.is_static_layer = FALSE ;    \
    result = UILAYER_CreateLayer(&create_info, &layer);     \
}

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
    GUI_RECT_T rect;
    MMI_IMAGE_ID_T img_id;
}MMI_KL_SPEC_DCELEMENT_IMG_T;

typedef enum
{
    SPECDC_UNLOCK_TYPE_NONE,
    SPECDC_UNLOCK_TYPE_RING,
    SPECDC_UNLOCK_TYPE_RELEASE,
    SPECDC_UNLOCK_TYPE_MAX
}SPECDC_UNLOCK_TYPE_E;


typedef enum
{
    SPECDC_ANIM_BIG_CIRCLE_EXTEND,
    SPECDC_ANIM_TRANGLE_NORMAL,
    SPECDC_ANIM_TRANGLE_REVERSE,
    SPECDC_ANIM_TYPE_MAX
}SPECDC_ANIM_TYPE_E;

typedef struct
{
    GUI_LCD_DEV_INFO data_layer;
    GUI_LCD_DEV_INFO big_circle_layer;
    GUI_LCD_DEV_INFO little_circle_layer;
    GUI_LCD_DEV_INFO tip_layer;
    GUI_LCD_DEV_INFO missevent_layer;
   // GUI_LCD_DEV_INFO bg_layer;
    uint8 tp_move_timer;
    uint8 big_layer_anim_timer;
    uint8 tp_up_anim_timer;
    uint8 triangle_timer;
    uint8 key_lock_release_timer;
    uint16 big_layer_anim_times;
    uint16 tp_up_anim_times;
    uint16 triangle_anim_times;
    uint16 key_lock_release_times;
}KEY_LOCK_SPECDC_INFO_T;

typedef struct
{
    GUI_POINT_T tp_down_point;
}KEY_LOCK_SPECDC_CUSTOM_T;

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL SPECDC_UNLOCK_TYPE_E s_unlock_type = SPECDC_UNLOCK_TYPE_NONE;    
LOCAL SPECDC_ANIM_TYPE_E s_kl_anim_type = SPECDC_ANIM_BIG_CIRCLE_EXTEND;
LOCAL KEY_LOCK_SPECDC_CUSTOM_T *s_kl_dc_custom = PNULL;
LOCAL KEY_LOCK_SPECDC_INFO_T s_kl_dc_info = {0};

LOCAL void DisplayLittleCircleAndLockAnimImage(MMI_WIN_ID_T    win_id, GUI_POINT_T tp_down_point,int16 offset_x,int16 offset_y);
LOCAL void DisplaySpecLockImage(MMI_WIN_ID_T    win_id, GUI_POINT_T tp_down_point,MMI_IMAGE_ID_T img_id);
LOCAL void SetBigLayerAlphaDC(uint8 alpha_value);

LOCAL GUI_LCD_DEV_INFO GetLayerDG(GUI_LCD_DEV_INFO *layer_info_ptr)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    
    if(layer_info_ptr->block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(layer_info_ptr))
        {
            lcd_dev_info = *layer_info_ptr;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  Append layer to blt layer array.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendLayer(GUI_LCD_DEV_INFO *layer)
{
    UILAYER_APPEND_BLT_T append_layer = {0};
    append_layer.lcd_dev_info = *layer;
    append_layer.layer_level = UILAYER_LEVEL_NORMAL;
    UILAYER_AppendBltLayer(&append_layer);
}


/*****************************************************************************/
//  Description :  display prompt tip
//  Global resource dependence :
//  Author: Michael.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayTipDC(void)
{
     GUISTR_STYLE_T text_style = {0};
     GUI_LCD_DEV_INFO lcd_dev = {0};
     GUI_RECT_T rect = {0};
     MMI_STRING_T tips_string = {0};
     wchar datewstr[30] = {0};

    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.align = ALIGN_HMIDDLE;

    rect.left = 0;
    rect.right = SPEC_DC_LCD_WINTH_V;
    rect.top = KEY_LOCK_TIP_TOP;
    rect.bottom = KEY_LOCK_TIP_BOTTOM;
    tips_string.wstr_ptr = datewstr;
    
      MMI_GetLabelTextByLang(TXT_KL_SPEC_SLIDE_UNLOCK,&tips_string);
      
    //lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);
    lcd_dev = GetLayerDG(&s_kl_dc_info.tip_layer);
    UILAYER_ClearRect(&lcd_dev, rect);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)(&lcd_dev),
        (const GUI_RECT_T *)&rect,
        (const GUI_RECT_T *)&rect,
        (const MMI_STRING_T *)&tips_string,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
        );    
}
/*****************************************************************************/
//  Description :  display network name osd
//  Global resource dependence :
//  Author: Michael.wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayNetworkNameDC(void)
{
    uint8 i = 0;
    GUISTR_STYLE_T text_style = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUI_RECT_T rect = {0};
    MMI_STRING_T tips_string = {0};    
    MMI_TEXT_ID_T network_name_id = TXT_NULL;
    PHONE_SERVICE_STATUS_T status = {0};
    wchar datewstr[30] = {0};

    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_NORMAL_FONT;
    text_style.align = ALIGN_LEFT;

    for( i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        rect.left = KEY_LOCK_DISPLAY_LEFT_SCREEN_OFFSET;
        rect.right = SPEC_DC_LCD_WINTH_V ;
        rect.top = KEY_LOCK_NETWORK_STRING_TOP+ KEY_LOCK_NETWORK_INTERVAL * i;
        rect.bottom = KEY_LOCK_NETWORK_STRING_BOTTOM + KEY_LOCK_NETWORK_INTERVAL * i;
        SCI_MEMSET(&status,0,sizeof(status));
        if(MMIAPIPHONE_GetSimExistedStatus(i))/*lint !e64*/
        {
            MMIAPIPHONE_GetCurServiceStatus(i,&status);    /*lint !e64*/
            network_name_id = MMIAPIPHONE_GetNetWorkNameId(MMIAPIPHONE_GetTDOrGsm(i), &(status.plmn));
        }
        else
        {
            network_name_id = TXT_INSERT_SIM;
        }
     MMI_GetLabelTextByLang(network_name_id,&tips_string);

        lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);
    UILAYER_ClearRect(&lcd_dev, rect);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)(&lcd_dev),
        (const GUI_RECT_T *)&rect,
        (const GUI_RECT_T *)&rect,
        (const MMI_STRING_T *)&tips_string,
        &text_style,
        GUISTR_STATE_ALIGN,
        GUISTR_TEXT_DIR_AUTO
        );
        
    }
}

/*****************************************************************************/
//  Description : display time
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayDigitalClockDC(void)
{
    
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    BOOLEAN is_landscape = 0;
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUI_RECT_T rect = {0};
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
    //time
    if(PNULL != s_kl_dc_custom)
    {
        GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_KL_SPECDC_NUM_0, MMIKL_CLOCK_DISP_WIN_ID);
    }
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = (lcd_width - img_width*6) / 2;
        rect.right = MIN(rect.left + img_width*6,MMI_LIST_ITEM_RIGHT);//CR140373
        rect.top = SPEC_DC_TIME_TOP_H;
        rect.bottom= SPEC_DC_TIME_TOP_H + img_height;      
    }
    else
#endif 
    {     
        rect.left = 0;
        rect.right = MIN(rect.left + img_width*KEY_LOCK_DIGITALCLOCK_NUM,MMI_LIST_ITEM_RIGHT);//CR140373
        rect.top = KEY_LOCK_DIGITALCLOCK_TOP;
        rect.bottom= rect.top + img_height;    
    }        
    
    lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);
    UILAYER_ClearRect(&lcd_dev, rect);
    MMIKL_DisplayDigitalClock(&(lcd_dev), rect, IMAGE_KL_SPECDC_NUM_0); 
}

/*****************************************************************************/
//  Description : display date
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
#if 0
LOCAL void DisplayDateDC(void)
{
    SCI_DATE_T date = {0};
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T m_string = {0};
    MMI_STRING_T final_string = {0};
    wchar m_buf[5] ={0}; 
    wchar datewstr[30] = {0};
    wchar wstr_buf[16] = {0};
    uint8 datestr[16] = {0};
    GUI_RECT_T rect = {0};
    GUI_LCD_DEV_INFO lcd_dev = {0};
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE    
    BOOLEAN is_landscape = FALSE;    
#endif
    
    MMI_TEXT_ID_T week_table[] = {
        TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
            TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
            TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
            TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
            TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
            TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
            TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
    };
    SCI_MEMSET(m_buf,0,sizeof(m_buf));
    SCI_MEMSET(datewstr,0,sizeof(datewstr));
    SCI_MEMSET(datestr,0,sizeof(datestr));
    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));   
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif    
    m_string.wstr_ptr = m_buf;
    TM_GetSysDate(&date);
    
    if(date.wday < 7)
    {
        MMI_GetLabelTextByLang(week_table[date.wday], &m_string);
    }
    
    MMIAPISET_GetCurrentDateStr(datestr,'-',sizeof(datestr));
    
    strncat(datestr," ",1);/*lint !e64*/
    MMIAPICOM_StrToWstr(datestr, wstr_buf);
    
    final_string.wstr_ptr = datewstr;
    MMIAPICOM_Wstrncpy(final_string.wstr_ptr,wstr_buf,MMIAPICOM_Wstrlen(wstr_buf));
    MMIAPICOM_Wstrncat(final_string.wstr_ptr, m_string.wstr_ptr,m_string.wstr_len);
    
    
    final_string.wstr_len = (uint16)MMIAPICOM_Wstrlen(final_string.wstr_ptr);
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_BIG_FONT;
    text_style.align =ALIGN_LEFT;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.left = 0;
        rect.right = SPEC_DC_LCD_WINTH_H;
        rect.top = SPEC_DC_DATE_TOP_H;
        rect.bottom = SPEC_DC_DATE_TOP_H + SPEC_DC_DATE_FONT_HEIGHT + 1;
    }
    else
#endif  
    {      
        rect.left = 2*KEY_LOCK_DISPLAY_LEFT_SCREEN_OFFSET;
        rect.right = SPEC_DC_LCD_WINTH_V;
        rect.top = KEY_LOCK_DATA_TOP;
        rect.bottom = KEY_LOCK_DATA_BOTTOM;                 
    }   
    
    lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);
     UILAYER_ClearRect(&lcd_dev, rect);
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
#endif

LOCAL void DisplayDateDay()
{
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T string = {0};
    uint8 datestr[32] = {0};
    wchar datewstr[16] = {0};
    
    SCI_DATE_T date = {0};
    
    
    
    GUI_LCD_DEV_INFO lcd_dev = {0};
    GUI_RECT_T rect = {0};
    
    MMI_TEXT_ID_T week_table[] = {
            TXT_SUNDAY,//       TXT_SHORT_IDLE_SUNDAY,
            TXT_MONDAY,//       TXT_SHORT_IDLE_MONDAY,
            TXT_TUESDAY,//      TXT_SHORT_IDLE_TUESDAY,
            TXT_WEDNESDAY,//       TXT_SHORT_IDLE_WEDNESDAY,
            TXT_THURSDAY,//       TXT_SHORT_IDLE_THURSDAY,
            TXT_FRIDAY,//       TXT_SHORT_IDLE_FRIDAY,
            TXT_SATURDAY,//       TXT_SHORT_IDLE_SATURDAY
    };
    
    /*
    MMI_TEXT_ID_T week_table_iphone[] = {
    TXT_IDLE_SHORT_SUNDAY,
    TXT_IDLE_SHORT_MONDAY,
    TXT_IDLE_SHORT_TUESDAY,
    TXT_IDLE_SHORT_WEDNESDAY,
    TXT_IDLE_SHORT_THURSDAY,
    TXT_IDLE_SHORT_FRIDAY,
    TXT_IDLE_SHORT_SATURDAY
    };
    */                                    
    wchar wstr_month[3] = {0};
    wchar wstr_day[3] = {0};
    MMI_STRING_T mouth_string = {0};
    MMI_STRING_T day_string = {0};
    MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    
    rect.left = 2*KEY_LOCK_DISPLAY_LEFT_SCREEN_OFFSET;
    rect.right = SPEC_DC_LCD_WINTH_V;
    rect.top = KEY_LOCK_DATA_TOP;
    rect.bottom = KEY_LOCK_DATA_BOTTOM;   
    
    text_style.font_color = MMI_WHITE_COLOR;
    text_style.font = MMI_DEFAULT_BIG_FONT;    
    text_style.align = ALIGN_LEFT;
    //text_style.effect = effect;
    
    MMIAPISET_GetCurrentDateStr(datestr,'-',16);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, 16 );    
    
    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &string);
    MMIAPISET_GetLanguageType(&lang_type);
    
    if(MMISET_LANGUAGE_SIMP_CHINESE == lang_type)        
    {
        MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
        text_style.font = CAF_FONT_25;
        date_display_type = MMIAPISET_GetDateDisplayType();
        switch(date_display_type)
        {
        case MMISET_DATE_YMD:
            wstr_month[0] = datewstr[5];
            wstr_month[1] = datewstr[6];
            
            wstr_day[0] = datewstr[8];
            wstr_day[1] = datewstr[9];
            
            break;
        case MMISET_DATE_MDY:
            wstr_month[0] = datewstr[0];
            wstr_month[1] = datewstr[1];
            
            wstr_day[0] = datewstr[3];
            wstr_day[1] = datewstr[4];
            
            
            break;
        case MMISET_DATE_DMY:
            wstr_month[0] = datewstr[3];
            wstr_month[1] = datewstr[4];
            
            wstr_day[0] = datewstr[0];
            wstr_day[1] = datewstr[1];
            
            break;
        default:
            break;
        }
        
        if('0' == wstr_month[0])
        {
            wstr_month[0] = wstr_month[1];
            wstr_month[1] = 0;
        }
        
        if('0' == wstr_day[0])
        {
            wstr_day[0] = wstr_day[1];
            wstr_day[1] = 0;
        }
        
        SCI_MEMSET(datewstr, 0, sizeof(datewstr));
        
        MMI_GetLabelTextByLang(TXT_DATE_MONTH, &mouth_string);
        MMI_GetLabelTextByLang(TXT_DATE_DAY, &day_string);
        MMIAPICOM_Wstrncpy(datewstr,wstr_month,MMIAPICOM_Wstrlen(wstr_month));
        MMIAPICOM_Wstrncat(datewstr,mouth_string.wstr_ptr,mouth_string.wstr_len);
        MMIAPICOM_Wstrncat(datewstr,wstr_day,MMIAPICOM_Wstrlen(wstr_day));
        MMIAPICOM_Wstrncat(datewstr,day_string.wstr_ptr,day_string.wstr_len);
        
        // MMI_GetLabelTextByLang(week_table_iphone[date.wday], &string);
    }
    
    MMIAPICOM_Wstrcat(datewstr, L"  ");
    MMIAPICOM_Wstrncat(datewstr, string.wstr_ptr,string.wstr_len);
    
    string.wstr_ptr = (wchar *)datewstr;
    string.wstr_len = MMIAPICOM_Wstrlen(datewstr);    
    
    lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);
    UILAYER_ClearRect(&lcd_dev, rect);
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)(&lcd_dev),
        (const GUI_RECT_T *)&rect,
        (const GUI_RECT_T *)&rect,
        (const MMI_STRING_T *)&string,
        &text_style,
        GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT,
        GUISTR_TEXT_DIR_AUTO
        );
}

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
    wchar    missed_call_wstr[KEYLOCK_COUNT_STRING_SIZE_DC]  = {0};
    wchar    missed_sms_wstr[KEYLOCK_COUNT_STRING_SIZE_DC]  = {0};
    char     count_str[KEYLOCK_COUNT_STRING_SIZE_DC] = {0};
    MMI_STRING_T        missed_call_text = {0};
    MMI_STRING_T        missed_sms_text = {0};
    
    GUISTR_STYLE_T event_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = *rect;
    
    missed_call = MMIAPICC_GetMissedCallNumber();    
    missed_sms = MMIAPISMS_GetAllUnreadMsgCount()
        + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
        ;

    if(0 == missed_call && 0 == missed_sms)
    {
        return;
    }

    event_text_style.align =ALIGN_LVMIDDLE;
    event_text_style.font = MMI_DEFAULT_NORMAL_FONT;
    event_text_style.font_color = MMI_WHITE_COLOR;

#ifdef MAINLCD_SIZE_320X480    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DC_MISS_CALL, win_id);
#endif    


    if(missed_call > 0)
    {
        MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &missed_call_text);               
        sprintf(count_str,"%d ",missed_call);
        str_len = strlen(count_str);
        MMI_STRNTOWSTR(missed_call_wstr, KEYLOCK_COUNT_STRING_SIZE_DC, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE_DC, str_len);
        if (KEYLOCK_COUNT_STRING_SIZE_DC > str_len + missed_call_text.wstr_len + 1)
        {
          MMIAPICOM_Wstrncpy(missed_call_wstr + str_len, missed_call_text.wstr_ptr , missed_call_text.wstr_len);
        }
        missed_call_text.wstr_ptr = missed_call_wstr;
        missed_call_text.wstr_len = str_len + missed_call_text.wstr_len;
     
#ifdef MAINLCD_SIZE_320X480    
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, dev_info_ptr, FALSE);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_DC_MISS_CALL, dev_info_ptr, FALSE);
#endif    


        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;
         UILAYER_ClearRect(dev_info_ptr, disp_rect);
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_call_text,
            &event_text_style,
            text_state,
            0
            );
    }
    
    if(missed_sms > 0)
    {
        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE_DC);
#ifdef MAINLCD_SIZE_320X480        
        /* Modify by michael on 3/8/2012 for NEWMS00177382 : Add new string accorrding the num of unread msg*/	
        if(1 == missed_sms )
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES, &missed_sms_text);
        }
        else if(missed_sms > 1)
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES2, &missed_sms_text);
        }
        /************************Modify over*************************/
#endif        
        sprintf(count_str,"%d ",missed_sms);
        str_len = strlen(count_str);
        
        MMI_STRNTOWSTR(missed_sms_wstr, KEYLOCK_COUNT_STRING_SIZE_DC, (const uint8*)count_str, KEYLOCK_COUNT_STRING_SIZE_DC,str_len);
        
        if (KEYLOCK_COUNT_STRING_SIZE_DC > str_len + missed_sms_text.wstr_len +1)
        {
            MMIAPICOM_Wstrncpy(missed_sms_wstr + str_len, missed_sms_text.wstr_ptr , missed_sms_text.wstr_len);
        }
        missed_sms_text.wstr_ptr = missed_sms_wstr;
        missed_sms_text.wstr_len = str_len + missed_sms_text.wstr_len;
        
        disp_rect = *rect;
        disp_rect.top += img_height + 2;
       
#ifdef  MAINLCD_SIZE_320X480 
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, dev_info_ptr, FALSE);
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, win_id);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_DC_SMS_UNREAD, dev_info_ptr, FALSE);
        
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_DC_SMS_UNREAD, win_id);
#endif

        disp_rect.left += img_width + 2;
        disp_rect.bottom = disp_rect.top + img_height;       
        UILAYER_ClearRect(dev_info_ptr, disp_rect);
        GUISTR_DrawTextToLCDInRect(
            dev_info_ptr,
            &disp_rect,
            &disp_rect,
            (const MMI_STRING_T *)&missed_sms_text,
            &event_text_style,
            text_state,
            0
            );
   }
}

/*****************************************************************************/
//  Description : display missed event
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayMissedEventDC(void)
{
    GUI_RECT_T rect = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    GUISTR_STYLE_T style= {0};
    MMI_STRING_T string = {0};
    GUISTR_INFO_T info = {0};    
    GUI_LCD_DEV_INFO lcd_dev = {0};
    uint16 rect_height = 0;
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    BOOLEAN is_landscape = FALSE;    
    
    is_landscape = MMITHEME_IsMainScreenLandscape();
#endif     
    style.font = MMI_DEFAULT_NORMAL_FONT;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);            
    MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &string); 
    
    GUISTR_GetStringInfo(&style, &string, GUISTR_STATE_NONE, &info);    
    rect_height = info.height * 2 + 8;    
    rect.right = lcd_width - 1;     
    
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE     
    if(is_landscape)
    {
        rect.top = SPEC_DC_MISSEVENT_TOP_H;
        rect.bottom = rect.top + rect_height;
    }
    else
#endif 
    {       
        rect.top = SPEC_DC_MISSEVENT_TOP;
        rect.bottom = rect.top + rect_height;               
    } 
    
  //  lcd_dev = GetLayerDG(&s_kl_dc_info.data_layer);    
    lcd_dev = GetLayerDG(&s_kl_dc_info.missevent_layer);    
    DisplayMissedEvent(MMIKL_CLOCK_DISP_WIN_ID, &rect, &lcd_dev);    
}
/*
LOCAL void DisplayBGDC(uint8 alpha)
{        
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    GUI_RECT_T main_rect = {0};        
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    main_rect.left = 0;
    main_rect.top = 0;
    main_rect.right = lcd_width -1;
    main_rect.bottom = lcd_height -1;

       LCD_FillArgbRect( &s_kl_dc_info.bg_layer, 
			main_rect, 
			MMI_BLACK_COLOR,
			alpha);
			
}
*/

/*****************************************************************************/
//  Description : start tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartTpMoveTimerDC(void)
{
    START_TIMER(s_kl_dc_info.tp_move_timer, KEY_LOCK_DC_TP_MOVE_TIME);
}

/*****************************************************************************/
//  Description : stop tp move timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopTpMoveTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.tp_move_timer);
}

/*****************************************************************************/
//  Description : start big layer extend anim timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartBigLayerAnimTimerDC(void)
{
    START_TIMER(s_kl_dc_info.big_layer_anim_timer, KEY_LOCK_DC_BIG_LAYER_ANIM_TIME);
}

/*****************************************************************************/
//  Description : stop big layer extend anim timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopBigLayerAnimTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.big_layer_anim_timer);
}

/*****************************************************************************/
//  Description : start triangle anim timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartTriangleAnimTimerDC(void)
{
    START_TIMER(s_kl_dc_info.triangle_timer, KEY_LOCK_DC_TRIANGLE_ANIM_TIME);
}

/*****************************************************************************/
//  Description : stop triangle anim timer
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopTriangleAnimTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.triangle_timer);
}

/*****************************************************************************/
//  Description : start the layer disappear anim when tp up.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartTpUpAnimTimerDC(void)
{
    START_TIMER(s_kl_dc_info.tp_up_anim_timer, KEY_LOCK_DC_BIG_LAYER_ANIM_TIME);
}

/*****************************************************************************/
//  Description : stop the layer disappear anim when tp up.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopTpUpAnimTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.tp_up_anim_timer);
}

/*****************************************************************************/
//  Description : start the layer disappear anim when key lock released.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StartKeyLockReleaseTimerDC(void)
{
    START_TIMER(s_kl_dc_info.key_lock_release_timer, KEY_LOCK_DC_LOCKRELEASE_TIME);
}

/*****************************************************************************/
//  Description : stop the layer disappear anim when key lock released.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL void StopKeyLockReleaseTimerDC(void)
{
    STOP_TIMER(s_kl_dc_info.key_lock_release_timer);
}

/*****************************************************************************/
//  Description : when (tp_cur - tp_down) > big circle R,the lock will released.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockReleaseEnable(int32 x,int32 y)
{
    int32 temp = SPEC_DC_BIG_CIRCLE_R ;
    return ((x * x) +(y * y))>(temp * temp) ? TRUE:FALSE;
}

/*****************************************************************************/
//  Description : anim of lock moved when tp move.
//  Global resource dependence : 
//  Author: Michael.wang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsLockMoveEnalble(int16 offset_x,int16 offset_y )
{
    if((ABS(offset_x) > KEY_LOCK_MOVE_START_DISTANCE) || (ABS(offset_y) > KEY_LOCK_MOVE_START_DISTANCE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
//  Description : Create layer.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO CreateLayerDC(MMI_WIN_ID_T win_id, GUI_RECT_T rect)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 width = 0, height = 0;
    
    if(rect.top < rect.bottom && rect.left < rect.right)
    {
        width = rect.right - rect.left + 1;
        height = rect.bottom - rect.top + 1;
        
        MMI_KL_SPECDC_CREATE_LAYER(create_info, win_id, width, height, layer, result);
        
        if(UILAYER_RESULT_SUCCESS == result)
        {
            AppendLayer(&layer);
            UILAYER_SetLayerPosition(&layer, rect.left, rect.top);
            UILAYER_Clear(&layer);
        }
    }
    return layer;
}

LOCAL void ClearDynamicLayerDC()
{
    UILAYER_Clear(&(s_kl_dc_info.little_circle_layer));
    UILAYER_Clear(&(s_kl_dc_info.big_circle_layer));                    
}

/******************************************************************************/
//  Description : release all layers
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAllLayersDC()
{
    //to reslove the pc-lint error
    GUI_LCD_DEV_INFO *little_circle_layer = &( s_kl_dc_info.little_circle_layer);
    GUI_LCD_DEV_INFO *big_circle_layer = &(s_kl_dc_info.big_circle_layer);
    GUI_LCD_DEV_INFO *data_layer = &( s_kl_dc_info.data_layer);
 //   GUI_LCD_DEV_INFO *bg_layer = &( s_kl_dc_info.bg_layer);
    UILAYER_Clear(& ( s_kl_dc_info.data_layer));    
    UILAYER_Clear(& s_kl_dc_info.little_circle_layer);
    UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
 //   UILAYER_Clear(&s_kl_dc_info.bg_layer);
    UILAYER_RELEASELAYER(little_circle_layer);
    UILAYER_RELEASELAYER(big_circle_layer);
    UILAYER_RELEASELAYER(data_layer);
//    UILAYER_RELEASELAYER(bg_layer);    
}

/******************************************************************************/
//  Description : create all layers
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void  CreateDynamicImageLayerDC(MMI_WIN_ID_T    win_id,GUI_POINT_T tp_down_point)
{
        GUI_RECT_T big_circle_rect = {0};
        GUI_RECT_T little_circle_rect = {0};

        big_circle_rect.left = tp_down_point.x -SPEC_DC_BIG_CIRCLE_R;
        big_circle_rect.top = tp_down_point.y -SPEC_DC_BIG_CIRCLE_R;
        big_circle_rect.right = tp_down_point.x + SPEC_DC_BIG_CIRCLE_R;
        big_circle_rect.bottom = tp_down_point.y + SPEC_DC_BIG_CIRCLE_R;

        little_circle_rect.left = tp_down_point.x -SPEC_DC_LITTLE_CIRCLE_R;
        little_circle_rect.top = tp_down_point.y -SPEC_DC_LITTLE_CIRCLE_R;
        little_circle_rect.right = tp_down_point.x + SPEC_DC_LITTLE_CIRCLE_R;
        little_circle_rect.bottom = tp_down_point.y + SPEC_DC_LITTLE_CIRCLE_R;
        
        s_kl_dc_info.big_circle_layer = CreateLayerDC(win_id,big_circle_rect);
        s_kl_dc_info.little_circle_layer = CreateLayerDC(win_id, little_circle_rect);	
}

/******************************************************************************/
//  Description : release all layers
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void ReleaseDynamicImageLayersDC()
{
    //to reslove the pc-lint error
    GUI_LCD_DEV_INFO *little_circle_layer = &( s_kl_dc_info.little_circle_layer);
    GUI_LCD_DEV_INFO *big_circle_layer = &(s_kl_dc_info.big_circle_layer);

    UILAYER_Clear(& s_kl_dc_info.little_circle_layer);
    UILAYER_Clear(&s_kl_dc_info.big_circle_layer);

    UILAYER_RELEASELAYER(little_circle_layer);
    UILAYER_RELEASELAYER(big_circle_layer);
}
/******************************************************************************/
//  Description : create all layers
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void  CreateImageLayerDC(MMI_WIN_ID_T    win_id)
{
    uint16 lcd_width = 0;
    uint16 lcd_height = 0; 
    GUI_RECT_T rect = {0};           
    GUI_RECT_T data_rect = {0};  
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T tip_rect = {0};  
    GUI_RECT_T missevent_rect = {0};  
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    GUIRES_GetImgWidthHeight(&img_width,&img_height, IMAGE_KL_SPECDC_NUM_0, MMIKL_CLOCK_DISP_WIN_ID);
    
    rect.left = 0;
    rect.top = 20;
    rect.right = lcd_width - 1;
    rect.bottom = lcd_height -1;

    data_rect.left = 10;
    data_rect.top = KEY_LOCK_NETWORK_STRING_TOP;
    data_rect.right =  img_width*KEY_LOCK_DIGITALCLOCK_NUM;
    data_rect.bottom = KEY_LOCK_DATA_BOTTOM;

    tip_rect.left = 0;
    tip_rect.right = SPEC_DC_LCD_WINTH_V;
    tip_rect.top = KEY_LOCK_TIP_TOP;
    tip_rect.bottom = KEY_LOCK_TIP_BOTTOM;

    missevent_rect.left = 0;
    missevent_rect.right = lcd_width - 1;
    missevent_rect.top = SPEC_DC_MISSEVENT_TOP;
    missevent_rect.bottom = 152;
    
  // s_kl_dc_info.bg_layer = CreateLayerDC(win_id,rect);
  // s_kl_dc_info.data_layer= CreateLayerDC(win_id,rect);
   s_kl_dc_info.tip_layer = CreateLayerDC(win_id,tip_rect);
   s_kl_dc_info.missevent_layer = CreateLayerDC(win_id,missevent_rect);
   s_kl_dc_info.data_layer= CreateLayerDC(win_id,data_rect);
   //s_kl_dc_info.big_circle_layer = CreateLayerDC(win_id,rect);
   //s_kl_dc_info.little_circle_layer = CreateLayerDC(win_id, rect);
}

/******************************************************************************/
//  Description : Display little circle according the img_id
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecLittleCircleImage(GUI_POINT_T tp_down_point,MMI_IMAGE_ID_T img_id)
{
    GUI_RECT_T little_circle_rect = {0};
    
    little_circle_rect.left = tp_down_point.x -SPEC_DC_LITTLE_CIRCLE_R;
    little_circle_rect.top = tp_down_point.y -SPEC_DC_LITTLE_CIRCLE_R;
    little_circle_rect.right = tp_down_point.x + SPEC_DC_LITTLE_CIRCLE_R;
    little_circle_rect.bottom = tp_down_point.y + SPEC_DC_LITTLE_CIRCLE_R;
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer)))
    {
        GUIRES_DisplayImg(PNULL, &(little_circle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, img_id, &(s_kl_dc_info.little_circle_layer));                
    }
}

/******************************************************************************/
//  Description : Display big circle
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecBigCircleDC(GUI_POINT_T tp_down_point)
{
    GUI_RECT_T big_circle_rect = {0};
    
    big_circle_rect.left = tp_down_point.x -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.top = tp_down_point.y -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.right = tp_down_point.x + SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.bottom = tp_down_point.y + SPEC_DC_BIG_CIRCLE_R;
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
        GUIRES_DisplayImg(PNULL, &(big_circle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_BIG_CIRCLE_EXTEND5, &(s_kl_dc_info.big_circle_layer));
    }
}

/******************************************************************************/
//  Description : Display big circle extend anim according the offset
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecBigCircleAnimDC(GUI_POINT_T tp_down_point,uint16 offset)
{
    GUI_RECT_T big_circle_rect = {0};
    
    big_circle_rect.left = tp_down_point.x -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.top = tp_down_point.y -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.right = tp_down_point.x + SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.bottom = tp_down_point.y + SPEC_DC_BIG_CIRCLE_R;
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.big_circle_layer);             
        GUIRES_DisplayImg(PNULL, &(big_circle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_BIG_CIRCLE_EXTEND1+ offset, &(s_kl_dc_info.big_circle_layer));
    }
}

/******************************************************************************/
//  Description : Display triangle anim anim according the offset
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecTriangleAnimDC(MMI_WIN_ID_T    win_id, GUI_POINT_T tp_down_point,uint16 offset)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T big_circle_rect = {0};
    GUI_RECT_T triangle_rect = {0};
    
    big_circle_rect.left = tp_down_point.x -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.top = tp_down_point.y -SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.right = tp_down_point.x + SPEC_DC_BIG_CIRCLE_R;
    big_circle_rect.bottom = tp_down_point.y + SPEC_DC_BIG_CIRCLE_R;
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_SPECDC_ANIM_SHALLOW, win_id);
    
    triangle_rect.left = tp_down_point.x -img_width/2;
    triangle_rect.top = tp_down_point.y -img_height/2;
    triangle_rect.right = tp_down_point.x + img_width/2;
    triangle_rect.bottom = tp_down_point.y + img_height/2;

    if(UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        
        DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE);
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK);

        GUIRES_DisplayImg(PNULL, &(triangle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_ANIM_SHALLOW + offset, &(s_kl_dc_info.little_circle_layer));
/*
        if(SPECDC_ANIM_TRANGLE_NORMAL == s_kl_anim_type)
        {                    
            GUIRES_DisplayImg(PNULL, &(triangle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_ANIM_SHALLOW + offset, &(s_kl_dc_info.little_circle_layer));
        }
        else if(SPECDC_ANIM_TRANGLE_REVERSE == s_kl_anim_type)
        {
            GUIRES_DisplayImg(PNULL, &(triangle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_ANIM_OUTSIDE - offset, &(s_kl_dc_info.little_circle_layer));
        }            
*/
    }
    /*
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer)))
    {
        
        UILAYER_Clear(&s_kl_dc_info.big_circle_layer);
        
        GUIRES_DisplayImg(PNULL, &(big_circle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_BIG_CIRCLE_EXTEND5, &(s_kl_dc_info.big_circle_layer));
        
        if(SPECDC_ANIM_TRANGLE_NORMAL == s_kl_anim_type)
        {                    
            GUIRES_DisplayImg(PNULL, &(triangle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_ANIM_SHALLOW + offset, &(s_kl_dc_info.big_circle_layer));
        }
        else if(SPECDC_ANIM_TRANGLE_REVERSE == s_kl_anim_type)
        {
            GUIRES_DisplayImg(PNULL, &(triangle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_SPECDC_ANIM_OUTSIDE - offset, &(s_kl_dc_info.big_circle_layer));
        }            
        
    }
    */
}

/******************************************************************************/
//  Description : Display Lock image
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplaySpecLockImage(MMI_WIN_ID_T    win_id, GUI_POINT_T tp_down_point,MMI_IMAGE_ID_T img_id)
{
    uint16 img_width = 0;
    uint16 img_height = 0;
    GUI_RECT_T little_circle_rect = {0};
    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_SPECDC_LOCK, win_id);
    
    little_circle_rect.left = tp_down_point.x -img_width/2;
    little_circle_rect.top = tp_down_point.y -img_height/2;
    little_circle_rect.right = tp_down_point.x + img_width/2;
    little_circle_rect.bottom = tp_down_point.y + img_height/2;
    
    if(UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer)))
    {
        GUIRES_DisplayImg(PNULL, &(little_circle_rect), PNULL, MMIKL_CLOCK_DISP_WIN_ID, img_id, &(s_kl_dc_info.little_circle_layer));             
    }
}

/******************************************************************************/
//  Description : calculate the square value according parameter
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL uint32 CalcSquareValue(uint16 value)
{
    return (uint32)(value * value);
}
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

/******************************************************************************/
//  Description : display the anim of little circle and lock when to move
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void DisplayLittleCircleAndLockAnimImage(MMI_WIN_ID_T    win_id, GUI_POINT_T tp_down_point,int16 offset_x,int16 offset_y)
{
    uint16 temp_value = SPEC_DC_BIG_CIRCLE_R/KEY_LOCK_MOVE_INTERVAL_NUM;
    uint32 tp_offset = (uint32)((offset_x * offset_x) + (offset_y * offset_y));

    uint8 alpha_value = 0;
    alpha_value = KEY_LOCK_UILAYER_ALPHA_MAX/KEY_LOCK_UILAYER_ALPHA_STEP;

    //4,8,12,14,20 :分为5个区间，根据区间进行相应的image 显示，用于tp move 的渐变动画效果
    if(tp_offset <= (CalcSquareValue(4 * temp_value)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE);
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK);
    }
    else if((tp_offset > CalcSquareValue(4 * temp_value)) &&tp_offset <= CalcSquareValue(8 * temp_value))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        if((tp_offset > CalcSquareValue(4 * temp_value)) &&(tp_offset <= CalcSquareValue(6 * temp_value)))
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_80);        
        }
        else
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_70);        
        }
        
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK);
        //SetBigLayerAlphaDC(alpha_value);
    }
    else if((tp_offset > CalcSquareValue(8 * temp_value)) &&(tp_offset <= CalcSquareValue(12 * temp_value)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        if((tp_offset > CalcSquareValue(8 * temp_value)) &&(tp_offset <= CalcSquareValue(10 * temp_value)))
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_60);        
        }
        else
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_50);        
        }
        
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK_LEFT1);
        SetBigLayerAlphaDC(2*alpha_value);
    }
    else if((tp_offset > CalcSquareValue(12 * temp_value)) &&(tp_offset <= CalcSquareValue(16 * temp_value)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        if((tp_offset > CalcSquareValue(12 * temp_value)) &&(tp_offset <= CalcSquareValue(14 * temp_value)))
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_40);        
        }
        else
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_30);        
        }
        
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK_LEFT2);
        SetBigLayerAlphaDC(3*alpha_value);
    }
    else if((tp_offset > CalcSquareValue(16 * temp_value)) &&(tp_offset<= CalcSquareValue(20 * temp_value)))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        if((tp_offset > CalcSquareValue(16 * temp_value)) &&(tp_offset <= CalcSquareValue(18 * temp_value)))
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_20);        
        }
        else
        {
            DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_10);        
        }
        
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK_LEFT3);
        SetBigLayerAlphaDC(4*alpha_value);
    }
    else if( tp_offset> CalcSquareValue(20 * temp_value))
    {
        UILAYER_Clear(&s_kl_dc_info.little_circle_layer);
        DisplaySpecLittleCircleImage(tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE_SHALLOW_10);      
        DisplaySpecLockImage(win_id,tp_down_point,IMAGE_KL_SPECDC_LOCK_LEFT4);
        SetBigLayerAlphaDC(5*alpha_value);
    }
       
}

/******************************************************************************/
//  Description : weak layer alpha vale of layer to make the layer disappear
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void WeakLayerAlphaDC(uint8 alpha_value)
{
    UILAYER_WeakLayerAlpha( &(s_kl_dc_info.big_circle_layer),alpha_value);
    UILAYER_WeakLayerAlpha( &(s_kl_dc_info.little_circle_layer),alpha_value);    
}

LOCAL void SetBigLayerAlphaDC(uint8 alpha_value)
{
    UILAYER_SetLayerAlpha( &(s_kl_dc_info.big_circle_layer),alpha_value);
    UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.big_circle_layer);
}

LOCAL void SetLittleLayerAlphaDC(uint8 alpha_value)
{    
    UILAYER_SetLayerAlpha( &(s_kl_dc_info.little_circle_layer),alpha_value);    
    UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.little_circle_layer);
}

/******************************************************************************/
//  Description : weak the data layer when tp down 
//  Global resource dependence : none
//  Author: Michael wang
//  Note:
/*****************************************************************************/
LOCAL void WeakDataLayerAlphaDC(uint16 offset)
{
    uint8 alpha_value = 0;
    alpha_value = KEY_LOCK_UILAYER_ALPHA_MAX/KEY_LOCK_UILAYER_ALPHA_STEP*(KEY_LOCK_UILAYER_ALPHA_STEP - offset);
    UILAYER_SetLayerAlpha( &s_kl_dc_info.data_layer,alpha_value);
    UILAYER_SetLayerAlpha( &s_kl_dc_info.tip_layer,alpha_value);
    UILAYER_SetLayerAlpha( &s_kl_dc_info.missevent_layer,alpha_value);
    UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.data_layer);
    UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.tip_layer);
    UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.missevent_layer);
}

/******************************************************************************/
//  Description : rest key lock param state of DC.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ResetSpecKeyLockParaDC()
{
    s_unlock_type = SPECDC_UNLOCK_TYPE_NONE;  
    s_kl_anim_type = SPECDC_ANIM_BIG_CIRCLE_EXTEND;
    StopTpMoveTimerDC();
    StopBigLayerAnimTimerDC();
    StopTriangleAnimTimerDC();
    StopTpUpAnimTimerDC();
}

/******************************************************************************/
//  Description : set the param of dynamic layer
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void SetInitParamOfDynamicLayerSPECDC()
{    
    s_kl_dc_info.big_circle_layer.block_id = UILAYER_NULL_HANDLE;
    s_kl_dc_info.little_circle_layer.block_id = UILAYER_NULL_HANDLE;    
}

/******************************************************************************/
//  Description : judge the dynamic layer whether active
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDynamicLayerActive()
{
    BOOLEAN ret = FALSE;
    
    if((UILAYER_IsLayerActive(&(s_kl_dc_info.big_circle_layer))) || (UILAYER_IsLayerActive(&(s_kl_dc_info.little_circle_layer))))
    {
        ret = TRUE;
    }
    return ret;
}
    
   
//  Description : Init custom parameter.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  InitCustomParamDC()
{       
    SCI_MEMSET(&s_kl_dc_info, 0, sizeof(s_kl_dc_info));
    
    if(PNULL == s_kl_dc_custom)
    {
        s_kl_dc_custom = SCI_ALLOCA(sizeof(KEY_LOCK_SPECDC_CUSTOM_T));
        
        if( PNULL == s_kl_dc_custom)
        {
            return FALSE;
        }
    }      
    
    SCI_MEMSET(s_kl_dc_custom, 0, sizeof(KEY_LOCK_SPECDC_CUSTOM_T));

    s_kl_anim_type = SPECDC_ANIM_BIG_CIRCLE_EXTEND;
    s_unlock_type = SPECDC_UNLOCK_TYPE_NONE;   
    SetInitParamOfDynamicLayerSPECDC();

    return TRUE;
}

/******************************************************************************/
//  Description : Init the all param.
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN InitParamDC(MMI_WIN_ID_T    win_id)
{
    if(InitCustomParamDC())
    {        
        CreateImageLayerDC(win_id);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
//  Description : handle kl display window msg.
//  Global resource dependence : none
//  Author: michael.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleSpecDoubleCircleWinMsg(
                                              MMI_WIN_ID_T    win_id, 
                                              uint16          msg_id, 
                                              DPARAM          param
                                              )
{   
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_POINT_T point = {0};   

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {                
            UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
            if(!InitParamDC(win_id))
            {
                MMK_CloseWin(win_id);
                break;
            }            
        }        
        break;  
        
    case MSG_FULL_PAINT:  
        {
         //   DisplayBGDC(100);
            DisplayShadowBGDC();
            DisplayTipDC();
            DisplayNetworkNameDC();
            DisplayDigitalClockDC();
            DisplayDateDay();
            DisplayMissedEventDC();            
        }          
        break;
        
    case MSG_TIMER:	
        { 
            if(s_kl_dc_info.tp_up_anim_timer == *(uint8*)param) //up时动态图层渐隐效果
            {
                uint8 alpha_value = 0;
                SCI_TRACE_LOW("[MMIKL_DC]++++++tp_up_anim_timer == TRUE+++++++");
                StopTpUpAnimTimerDC();
                if(s_kl_dc_info.tp_up_anim_times < KEY_LOCK_TP_UP_ANIM_TIMES)
                {
                    alpha_value = KEY_LOCK_UILAYER_ALPHA_MAX/KEY_LOCK_UILAYER_ALPHA_STEP*(KEY_LOCK_UILAYER_ALPHA_STEP - s_kl_dc_info.tp_up_anim_times);
                    WeakLayerAlphaDC(alpha_value);
                    if(s_kl_dc_info.tp_up_anim_times > 0)
                    {
                        UILAYER_SetLayerAlpha(&s_kl_dc_info.data_layer,KEY_LOCK_UILAYER_ALPHA_MAX - alpha_value);
                        UILAYER_SetLayerAlpha(&s_kl_dc_info.tip_layer,KEY_LOCK_UILAYER_ALPHA_MAX - alpha_value);
                        UILAYER_SetLayerAlpha(&s_kl_dc_info.missevent_layer,KEY_LOCK_UILAYER_ALPHA_MAX - alpha_value);
                        UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.data_layer);
                        UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.tip_layer);
                        UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.missevent_layer);
                    }
                    s_kl_dc_info.tp_up_anim_times++;
                    StartTpUpAnimTimerDC();
                }
                else
                {
                    s_kl_dc_info.tp_up_anim_times = 0;
                    ClearDynamicLayerDC();
                    ReleaseDynamicImageLayersDC();
                }                                             
            }
            else if(s_kl_dc_info.big_layer_anim_timer == *(uint8*)param)    //大圆扩散效果
            {
                   SCI_TRACE_LOW("[MMIKL_DC]++++++big_layer_anim_timer == TRUE+++++++");
                if((PNULL  == s_kl_dc_custom) ||(SPECDC_UNLOCK_TYPE_NONE == s_unlock_type ))
                {
                    break;
                }   
                
                StopBigLayerAnimTimerDC();
                if(SPECDC_ANIM_BIG_CIRCLE_EXTEND == s_kl_anim_type)
                {
                    if(s_kl_dc_info.big_layer_anim_times < KEY_LOCK_DC_BIG_CIRCLE_EXTEND_STEP)
                    {
                        DisplaySpecBigCircleAnimDC(s_kl_dc_custom->tp_down_point,s_kl_dc_info.big_layer_anim_times);
                        WeakDataLayerAlphaDC(s_kl_dc_info.big_layer_anim_times);
                        s_kl_dc_info.big_layer_anim_times++;        
                        //To fix CR NEWMS00215132,when the biggest circle display,the trangle display at that time
                        if((KEY_LOCK_DC_BIG_CIRCLE_EXTEND_STEP - 1 )==s_kl_dc_info.big_layer_anim_times)
                        {
                            DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,0); 
                        }
                        StartBigLayerAnimTimerDC();
                    }
                    else
                    {
                        s_kl_anim_type = SPECDC_ANIM_TRANGLE_REVERSE;
                        s_kl_dc_info.big_layer_anim_times = 0;
                        StartTriangleAnimTimerDC();
                    }                        
                }
            }
            else if(s_kl_dc_info.triangle_timer== *(uint8*)param)   //三角正向及反向渐变效果
            {
                if((PNULL  == s_kl_dc_custom) ||(SPECDC_UNLOCK_TYPE_NONE == s_unlock_type ))
                {
                    break;
                }   
                
                StopTriangleAnimTimerDC();
                if(SPECDC_ANIM_TRANGLE_REVERSE == s_kl_anim_type)
                {       //KEY_LOCK_TRIANGLE_ANIM_TIMES_REVERSE - KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES == 延迟次数
                       if(s_kl_dc_info.triangle_anim_times < KEY_LOCK_TRIANGLE_ANIM_TIMES_NORMAL)
                       {
                            if(s_kl_dc_info.triangle_anim_times < KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES)
                            {
                                uint8 alpha_value = 0;
                                alpha_value = KEY_LOCK_UILAYER_ALPHA_MAX/KEY_LOCK_UILAYER_ALPHA_STEP*(KEY_LOCK_UILAYER_ALPHA_STEP - s_kl_dc_info.triangle_anim_times);
                                SetBigLayerAlphaDC(alpha_value);                                
                               // DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,0);    
                                DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,s_kl_dc_info.triangle_anim_times);                                           
                            }
                            else if(s_kl_dc_info.triangle_anim_times == KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES)
                            {
                                DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,0); 
                            }
                            s_kl_dc_info.triangle_anim_times++;
                        }
                        else
                        {
                            s_kl_anim_type = SPECDC_ANIM_TRANGLE_NORMAL;
                            s_kl_dc_info.triangle_anim_times = 0;
                        }
                        StartTriangleAnimTimerDC();
                }
                else if(SPECDC_ANIM_TRANGLE_NORMAL == s_kl_anim_type)
                {        //KEY_LOCK_TRIANGLE_ANIM_TIMES_NORMAL - KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES == 延迟次数
                    if(s_kl_dc_info.triangle_anim_times < KEY_LOCK_TRIANGLE_ANIM_TIMES_NORMAL)
                    {
                        if(s_kl_dc_info.triangle_anim_times < KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES)
                        {
                            DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,s_kl_dc_info.triangle_anim_times);                                
                        }
                        else if(s_kl_dc_info.triangle_anim_times == KEY_LOCK_TRIANGLE_ANIM_VALID_TIMES)
                        {
                            DisplaySpecTriangleAnimDC(win_id,s_kl_dc_custom->tp_down_point,0);                                
                        }
                        s_kl_dc_info.triangle_anim_times++;
                    }
                    else
                    {
                        s_kl_dc_info.triangle_anim_times = 0;
                    }
                    StartTriangleAnimTimerDC();
                }
            }       
            else if(s_kl_dc_info.key_lock_release_timer == *(uint8*)param)  //key lock released anim timer
            {
                StopKeyLockReleaseTimerDC();
                if(s_kl_dc_info.key_lock_release_times < KEY_LOCK_UILAYER_ALPHA_STEP)
                {
                    WeakDataLayerAlphaDC(s_kl_dc_info.key_lock_release_times);
                    s_kl_dc_info.key_lock_release_times++;
                    StartKeyLockReleaseTimerDC();
                }
                else
                {
                    s_kl_dc_info.key_lock_release_times = 0;
                    MMK_CloseWin(win_id);
                }
                
            }
            else if(s_kl_dc_info.tp_move_timer == *(uint8*)param)    //tp timer
            {
                MMI_TP_STATUS_E tp_status = 0;
                int16 offset_x = 0,  offset_y = 0;         
                
                StopTpMoveTimerDC();
                MMK_GetLogicTPMsg(&tp_status, &point);
                SCI_TRACE_LOW("[MMIKL_DC]++++++tp_move_timer == TRUE+++++++");
                if((PNULL  == s_kl_dc_custom) ||(SPECDC_UNLOCK_TYPE_NONE == s_unlock_type ))
                {
                    break;
                }   
                
                offset_x = point.x -s_kl_dc_custom->tp_down_point.x;
                offset_y = point.y -s_kl_dc_custom->tp_down_point.y;         
                
                if(IsLockMoveEnalble(offset_x,offset_y))
                {
                    StopTriangleAnimTimerDC();
                    StopBigLayerAnimTimerDC();
                    DisplaySpecBigCircleDC(s_kl_dc_custom->tp_down_point);                        
                    
                    if(IsLockReleaseEnable(offset_x,offset_y))
                    {
                        s_unlock_type = SPECDC_UNLOCK_TYPE_RELEASE;
                        ClearDynamicLayerDC();            
                        ReleaseDynamicImageLayersDC();
                        UILAYER_SetLayerAlpha(&s_kl_dc_info.data_layer,KEY_LOCK_UILAYER_ALPHA_MAX);
                        UILAYER_StoreLayerUpdateRect(&s_kl_dc_info.data_layer);
                        StartKeyLockReleaseTimerDC();
                    }
                    else
                    {
                        DisplayLittleCircleAndLockAnimImage(win_id,s_kl_dc_custom->tp_down_point,offset_x,offset_y);
                        StartTpMoveTimerDC();
                    }
                }
                else
                {
                    StartTpMoveTimerDC();
                }                                      
            }                             
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        {                  
            SCI_TRACE_LOW("[MMIKL_DC]++++++MSG_TP_PRESS_DOWN+++++++");
            if(SPECDC_UNLOCK_TYPE_NONE == s_unlock_type)
            {
                SCI_TRACE_LOW("[MMIKL_DC]++++++MSG_TP_PRESS_DOWN == TRUE+++++++");
                if(PNULL == s_kl_dc_custom)
                {
                    break;
                }                
                s_unlock_type = SPECDC_UNLOCK_TYPE_RING;
                s_kl_dc_custom->tp_down_point.x = MMK_GET_TP_X(param);
                s_kl_dc_custom->tp_down_point.y = MMK_GET_TP_Y(param);
            
               StopTpUpAnimTimerDC();
                if(IsDynamicLayerActive())
                {
                    ClearDynamicLayerDC();//防止up时渐隐效果过程中再次按下tp down的操作，此处需先release anim layers 
                    ReleaseDynamicImageLayersDC();             
                }
                       
            //   DisplayBGDC(180);
                CreateDynamicImageLayerDC(win_id,s_kl_dc_custom->tp_down_point);
                DisplaySpecLittleCircleImage(s_kl_dc_custom->tp_down_point,IMAGE_KL_SPECDC_LITTLE_CIRCLE);
                DisplaySpecLockImage(win_id,s_kl_dc_custom->tp_down_point,IMAGE_KL_SPECDC_LOCK);
                
                StartBigLayerAnimTimerDC(); //开启大圆扩散效果的定时器           
                StartTpMoveTimerDC();
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        {                     
            SCI_TRACE_LOW("[MMIKL_DC]++++++MSG_TP_PRESS_UP == TRUE+++++++");
            if( SPECDC_UNLOCK_TYPE_RING == s_unlock_type)
            {
                SCI_TRACE_LOW("[MMIKL_DC]++++++MSG_TP_PRESS_DOWN == TRUE+++++++");
            //    DisplayBGDC(100);           
                ResetSpecKeyLockParaDC();
                s_kl_dc_info.big_layer_anim_times = 0;
                s_kl_dc_info.tp_up_anim_times = 0;
                s_kl_dc_info.triangle_anim_times = 0;              
                StartTpUpAnimTimerDC();//开启up时动态图层渐隐效果的定时器              
            }
        }	   
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_LOSE_FOCUS:
        {         
            ResetSpecKeyLockParaDC();
            ClearDynamicLayerDC();
            ReleaseDynamicImageLayersDC();
        }        
        break;

    case MSG_GET_FOCUS:
        {   //lose focus时自动deactive 所有此window hanlde的layer handle,并且将它们移出blt arr,在get focus时我们需要手动append layer，否则数据将丢失
          //  AppendLayer(&s_kl_dc_info.bg_layer);
            UILAYER_SetMainLayerMemFormat(UILAYER_MEM_NONE);
            AppendLayer(&s_kl_dc_info.data_layer);
            //AppendLayer(&s_kl_dc_info.big_circle_layer);
            //AppendLayer(&s_kl_dc_info.little_circle_layer);
            AppendLayer(&s_kl_dc_info.tip_layer);
            AppendLayer(&s_kl_dc_info.missevent_layer);
        }
        break;
        
    case MSG_BACKLIGHT_TURN_OFF:
        {
            ResetSpecKeyLockParaDC();
            ClearDynamicLayerDC();
            ReleaseDynamicImageLayersDC();
        }
        break;
        
    case MSG_APP_OK:
        {            
            if (MMIKL_IsPreviewState())
            {
                MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_DOUBLE_CIRCLE_SPEC);
                MMK_CloseWin(win_id);
            }
            else
            {
                recode = MMI_RESULT_FALSE;
            }
        }        
        break;
        
    case MSG_APP_CANCEL:
        {
            if (MMIKL_IsPreviewState())
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                recode = MMI_RESULT_FALSE;   //popup the prompt info
            }
        }        
        break;           

    case MSG_CLOSE_WINDOW:
        {
            UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);

            SCI_MEMSET(&s_kl_dc_info, 0, sizeof(s_kl_dc_info));
            
            ResetSpecKeyLockParaDC();
            ReleaseAllLayersDC();
            ReleaseDynamicImageLayersDC();
            
            if(s_kl_dc_custom != PNULL)
            {
                SCI_FREE(s_kl_dc_custom);
                s_kl_dc_custom = PNULL;
            }         
        }             
        break;        
        
#ifdef MMI_KEY_LOCK_DC_SUPPORT_ROTATE  
    case MSG_LCD_SWITCH:                
#endif          
    case MSG_BACKLIGHT_TURN_ON:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    } 
    
    return recode;
}
#endif	//MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT

#endif
/*Edit by script, ignore 2 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
