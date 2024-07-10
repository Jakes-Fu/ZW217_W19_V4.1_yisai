/*****************************************************************************
** File Name:      mmikl_effectmain.c                                        *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe effect keylock main function*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/
#ifdef MMI_KEY_LOCK_SUPPORT
#ifndef _MMIKL_EFFECTMAIN_C_

#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_appmsg.h"
#include "mmikl_effectmain.h"
#include "mmikl_export.h"
//#include "mmiset_lock_effect.h"
#include "mmiset_lock_effect_export.h"
#include "guistatusbar.h"
#include "mmidc_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "guibutton.h"
#include "guires.h"
#include "mmi_wallpaper_export.h"
#include "mmidisplay_data.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#include "mmiidle_export.h"
#include "mmipub.h"
#include "mmimp3_export.h"
#include "mmikl_slidedown.h"
#include "mmikl_slidescrollkey.h"
#include "mmikl_smartslide.h"
#include "mmikl_istyleslide.h"
#include "mmikl_drawgraphic.h"
#include "mmikl_dragunlock.h"
#include "mmikl_effectpsw.h"
#include "mmikl_fruitsplit.h"
#include "mmi_mainmenu_export.h"
#include "mmiset_id.h"
#include "guibutton.h"
#include "mmiset_wintab.h"
#include "mmikl_internal.h"
#include "ui_layer.h"
#ifdef TAKESCREENPHOTO_SUPPORT 
#include "mmitakescrphoto_main.h"
#endif
//michael wang
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
#include "Mmikl_doublecirclelock.h "
#endif
//spec
#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
#include "Mmikl_specdoublecirclelock.h "
#endif

#include "guiscrollkey.h"
#ifdef SLIDESHOW_WALLPAPER_SUPPORT
#include "mmi_livewallpaper_slideshow.h"
#endif

#include "mmicl_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN (20)
#ifdef MMI_KEYLOCK_MINI_SUPPORT
#define KEY_LOCK_QQVGA_IMAGE_SPACE  30
#endif
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint32 s_kl_proc_open_flag = 0;

typedef BOOLEAN (*HANDLE_FUNC)(MMI_WIN_ID_T win_id, uint16 msg_id, DPARAM param);

#define RUN_PROC_BEGIN(f_arr, n)  \
    if(n < MMISET_KEY_LOCK_EFFECT_MAX)   \
    {   \
        if(f_arr[n] != PNULL)    \
        {

#define RUN_PROC_END    \
        }   \
    }
#ifndef TOUCH_PANEL_SUPPORT
#ifdef UI_MULTILAYER_SUPPORT
LOCAL GUI_LCD_DEV_INFO  s_keylock_tips_layer_handle = {0,UILAYER_NULL_HANDLE};
#endif
#endif

#ifdef TOUCHPANEL_TYPE
LOCAL GUI_LCD_DEV_INFO  s_preview_button_layer_info = {0,UILAYER_NULL_HANDLE};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#ifdef TOUCHPANEL_TYPE

/*****************************************************************************/
//  Description : create button layer.
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateButtonLayer(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : CreateButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateButton(
                        MMI_WIN_ID_T win_id, 
                        MMI_CTRL_ID_T ctrl_id, 
                        MMI_TEXT_ID_T text_id, 
                        GUI_BOTH_RECT_T both_rect
                        );

/*****************************************************************************/
//  Description : SetStaticWallpaperOKButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetOKButtonCallback(void);

/*****************************************************************************/
//  Description : SetStaticWallpaperCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetCancelButtonCallback(void);

/*****************************************************************************/
//  Description : CreateSetPreviewButton
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetPreviewButton(MMI_WIN_ID_T win_id,MMISET_KEY_LOCK_EFFECT_E lock_type);
#endif

#ifndef TOUCH_PANEL_SUPPORT  
#ifdef UI_MULTILAYER_SUPPORT
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void CreateKeyPadUnLockTipsLayer(void)
{
    uint16                      image_width = 0,image_height = 0;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;

    //get lcd width and height
    
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
            
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_TIPS_BG,MMIKL_CLOCK_DISP_WIN_ID);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,
        &lcd_width,
        &lcd_height);  

        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = MMIKL_CLOCK_DISP_WIN_ID;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
#ifdef MAINLCD_SIZE_176X220
        create_info.height = image_height*2;
#else
        create_info.height = image_height;
#endif
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(
            &create_info,
            &s_keylock_tips_layer_handle);
        
           //set color key
        UILAYER_SetLayerColorKey(&s_keylock_tips_layer_handle,TRUE,UILAYER_TRANSPARENT_COLOR);

    }
}


/*****************************************************************************/
//  Description : hide idle tips
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void HideKeyPadUnLockTipsLayer(void)
{
    GUI_RECT_T          layer_rect = {0};
    GUI_LCD_DEV_INFO    *layer_handle_ptr = &s_keylock_tips_layer_handle;
    
    if (UILAYER_IsLayerActive(layer_handle_ptr))
    {
        //get layer
        UILAYER_RemoveBltLayer(layer_handle_ptr);

        //set layer rect
        layer_rect= UILAYER_GetLayerRect(layer_handle_ptr);      
        MMITHEME_StoreUpdateRect(layer_handle_ptr,layer_rect);
    }
}
               
/*****************************************************************************/
//  Description : DisplayKeyPadUnLockTipsLayer not use layer just directly draw to main lcd
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL void DisplayKeyPadUnLockTipsLayer(MMI_TEXT_ID_T  tips)
{
    uint16                      str_width = 0;
    GUI_RECT_T                  tips_rect = {0};
    MMI_STRING_T                tips_string = {0};
    GUISTR_STYLE_T              str_style = {0};
    GUISTR_STATE_T              str_state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK;
    uint16                      image_width = 0,image_height = 0;
    uint16                      lcd_width = 0;
    uint16                      lcd_height = 0;
    uint8                       str_margin = 10;
    UILAYER_APPEND_BLT_T        append_layer = {0};
    GUI_LCD_DEV_INFO            *layer_handle_ptr = &s_keylock_tips_layer_handle;
#ifdef MAINLCD_SIZE_176X220
    GUISTR_INFO_T               str_info = {0};
	uint16                      str_height = 0;
#endif
    
    if (UILAYER_IsLayerActive(layer_handle_ptr))
    {
#ifdef MAINLCD_SIZE_176X220

		GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
		
        MMI_GetLabelTextByLang(tips,&tips_string);
		
        //set string style
        str_style.font       = MMI_DEFAULT_NORMAL_FONT;
        str_style.font_color = MMI_DEFAULT_TEXT_COLOR;
        str_style.align      = ALIGN_HVMIDDLE;
		
        //get string width and height
        GUISTR_GetStringInfo(&str_style, &tips_string, str_state, &str_info);
        str_width = str_info.width;
		str_height = str_info.height;

		tips_rect.left = MAX(0,(lcd_width - (str_width + str_margin * 2) ) / 2) + str_margin;
        tips_rect.right = lcd_width - 1 - tips_rect.left;
        
		if((str_width + str_margin*2) > (lcd_width*2)) //三行显示
		{
            tips_rect.top  = MMI_STATUSBAR_HEIGHT + str_margin;
			tips_rect.bottom = tips_rect.top + str_height*3;
            str_style.align = ALIGN_TOP;			
        }
        else if((str_width + str_margin*2) > lcd_width) //双行显示 
        {
            tips_rect.top  = MMI_STATUSBAR_HEIGHT + str_margin;
			tips_rect.bottom = tips_rect.top + str_height*2;
            str_style.align = ALIGN_TOP;
        }
		else  //单行显示
		{
			tips_rect.top  = MMI_STATUSBAR_HEIGHT + str_margin;
			tips_rect.bottom = tips_rect.top + str_height;
		}

		//set layer position
        UILAYER_SetLayerPosition(layer_handle_ptr,
            tips_rect.left,
            tips_rect.top);

		//clear
        UILAYER_Clear(layer_handle_ptr);

        //blt
        append_layer.lcd_dev_info = *layer_handle_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);


        //display text
        GUISTR_DrawTextToLCDInRect(layer_handle_ptr,
            &tips_rect,
            &tips_rect,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
#else
		//get rect info
        GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_IDLE_KEYLOCK_TIPS_BG,MMIKL_CLOCK_DISP_WIN_ID);
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

        MMI_GetLabelTextByLang(tips,&tips_string);

        //set string style
        str_style.font       = MMI_DEFAULT_NORMAL_FONT;
        str_style.font_color = MMI_DEFAULT_TEXT_COLOR;
        str_style.align      = ALIGN_HVMIDDLE;

        //get string width
        str_width = GUISTR_GetStringWidth(&str_style,
            &tips_string,
            str_state);
        
        //get tips rect
        tips_rect.left = MAX(0,(lcd_width - (str_width + str_margin * 2) ) / 2);
        tips_rect.right = lcd_width - 1 - tips_rect.left;
        tips_rect.bottom = lcd_height/2+image_height;
        tips_rect.top  = tips_rect.bottom - image_height+1;
        
        if((str_width + str_margin*2) > lcd_width) //双行显示 
        {
            tips_rect.top  = tips_rect.bottom - image_height*2;
        }
        
        //set layer position
        UILAYER_SetLayerPosition(layer_handle_ptr,
            tips_rect.left,
            tips_rect.top);

        //clear
        UILAYER_Clear(layer_handle_ptr);

        
        //blt
        append_layer.lcd_dev_info = *layer_handle_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);

        //display bg image
        GUIRES_DisplayImg(PNULL,&tips_rect,
            PNULL,
            MMIKL_CLOCK_DISP_WIN_ID,
            IMAGE_IDLE_KEYLOCK_TIPS_BG,
            layer_handle_ptr);


        //display text
        tips_rect.right = tips_rect.right - str_margin;
        tips_rect.left = tips_rect.left + str_margin;
        tips_rect.bottom = tips_rect.bottom - str_margin;
        tips_rect.top  = tips_rect.top + str_margin;
        GUISTR_DrawTextToLCDInRect(layer_handle_ptr,
            &tips_rect,
            &tips_rect,
            &tips_string,
            &str_style,
            str_state,
            GUISTR_TEXT_DIR_AUTO);
#endif
    }
}
#endif
#endif
LOCAL const  uint32 s_win_handle_func_arr[MMISET_KEY_LOCK_EFFECT_MAX] = 
{
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
     (uint32)MMIKL_HandleKLDispWinMsg,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT        
     (uint32)MMIKL_HandleSlideDownUnlockWinMsg,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
     (uint32)MMIKL_HandleSlideScrollKeyWinMsg,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT
     (uint32)MMIKL_HandleSmartSlideDoneWinMsg,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
     (uint32)MMIKL_HandleIphoneStyleSlideWinMsg,
#endif
#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
     (uint32)MMIKL_HandleDrawGraphicWinMsg,
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
     (uint32)MMIKL_HandleDragIconOrRingUnlockWinMsg,
#endif
#ifdef MMI_KEY_LOCK_FRUIT_SPLIT_SUPPORT
     (uint32)MMIKL_HandleFruitSplitWinMsg,
#endif
//michael wang
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
     (uint32)MMIKL_HandleDoubleCircleWinMsg,
#endif
//spec
#ifdef MMI_KEY_LOCK_SPEC_DOUBLE_CIRCLE_SUPPORT
     (uint32)MMIKL_HandleSpecDoubleCircleWinMsg,
#endif

//michael wang
};

/*****************************************************************************/
//  Description : MMIKL_CreatEffectWallpaperCtrl
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_CreatEffectWallpaperCtrl(MMISET_KEY_LOCK_EFFECT_E lock_type)
{
    BOOLEAN                 result           = FALSE;

#if defined MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT
    if(MMISET_KEY_LOCK_EFFECT2 == lock_type )
    {
        result = TRUE;
    }
#endif

#if defined MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
    if(MMISET_KEY_LOCK_EFFECT6 == lock_type )
    {
        result = TRUE;
    }
#endif 
#if (defined MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT) || (defined MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT)
    if(result)
    {
#if defined MMI_WALLPAPER_SUPPORT
        MMIAPIIDLE_SetInitWallpaper_KL(FALSE, MMIKL_CLOCK_DISP_WIN_ID, MMIKL_MAIN_ANIM_BG_ID,TRUE );
#endif
    }
#endif
    return result;
}
#ifndef MMI_KEYLOCK_MINI_SUPPORT
/*****************************************************************************/
//  Description :  General  digital clock display.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayDigitalClock(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, MMI_IMAGE_ID_T start_img_id)
{
    GUI_POINT_T                 point = {0};
    SCI_TIME_T                  time = {0};
    MMISET_TIME_DISPLAY_TYPE_E  time_display_type = MMIAPISET_GetTimeDisplayType();
    BOOLEAN                     b_am = TRUE;
    uint16        am_image_height=0,image_width = 0,image_height = 0;
    uint16                      time_total_width = 0;
    
    TM_GetSysTime(&time);

    //12小时制
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GET_12STYLE_HOUR(time.hour, b_am);
    }

    //计算时间总的宽度，为了居中显示
    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id + 10,MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width  + image_width;//colon

#if 1    
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        GUIRES_GetImgWidthHeight(&image_width,&am_image_height, IMAGE_IDLE_KEYLOCK_NUM_AM,MMIKL_CLOCK_DISP_WIN_ID);
        time_total_width = time_total_width  + image_width;//am.pm
    }
#endif

    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id,MMIKL_CLOCK_DISP_WIN_ID);
    time_total_width = time_total_width + image_width * 4 ;//有四个数字

    point.y = rect.top + (rect.bottom - rect.top - image_height) / 2;
    if(rect.right - rect.left >= time_total_width)
    {
        point.x = rect.left + (rect.right - rect.left - time_total_width)/2;
    }
    else
    {
        point.x = rect.left;
    }

    //HOUR
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,start_img_id + time.hour/10, dev);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,start_img_id + time.hour%10, dev);

    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,start_img_id + 10, dev);

    //COLON IMAGE WIDTH
    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id + 10, MMIKL_CLOCK_DISP_WIN_ID);
    //MIN
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,start_img_id + time.min/10, dev);

    GUIRES_GetImgWidthHeight(&image_width,&image_height, start_img_id,MMIKL_CLOCK_DISP_WIN_ID);
    point.x = point.x + image_width;
    GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,start_img_id + time.min%10, dev);
    //AM.PM
#if 1    
    if(time_display_type == MMISET_TIME_12HOURS)
    {
        point.x = point.x + image_width;
        if(image_height<am_image_height)
        {
            point.y -=(am_image_height-image_height);
        }
        
        if(b_am )
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_AM, dev);
        else
            GUIRES_DisplayImg(&point,PNULL,PNULL,MMIKL_CLOCK_DISP_WIN_ID,IMAGE_IDLE_KEYLOCK_NUM_PM, dev);
    }
#endif    
}

/*****************************************************************************/
//  Description :  Display day.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
void DisplayPLMN(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, GUI_FONT_T font, GUI_ALIGN_E align, GUI_COLOR_T color)
{
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T string = {0};
    wchar wstr_buf[MMIPHONE_MAX_OPER_NAME_LEN] = {0};

    SCI_MEMSET(wstr_buf,0,sizeof(wstr_buf));

    text_style.font_color = color;
    text_style.font = font;
    text_style.align = align;
    
    string.wstr_ptr = wstr_buf;
    MMIAPIPHONE_GetSimAndServiceString(MMIAPISET_GetActiveSim(),&string);
    GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)dev,
                    (const GUI_RECT_T *)&rect,
                    (const GUI_RECT_T *)&rect,
                    (const MMI_STRING_T *)&string,
                    &text_style,
                    GUISTR_STATE_ALIGN,
                    GUISTR_TEXT_DIR_AUTO
                    ); 
}

/*****************************************************************************/
//  Description :  Display date and day
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayDateAndDay(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, GUI_FONT_T font, GUI_ALIGN_E align, GUI_COLOR_T color, GUIFONT_EFFECT_T effect )
{
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T string = {0};
    uint8 datestr[32] = {0};
    wchar datewstr[16] = {0};

    SCI_DATE_T date = {0};
    MMI_TEXT_ID_T week_table[] = {
                                        TXT_SHORT_IDLE_SUNDAY,
                                        TXT_SHORT_IDLE_MONDAY,
                                        TXT_SHORT_IDLE_TUESDAY,
                                        TXT_SHORT_IDLE_WEDNESDAY,
                                        TXT_SHORT_IDLE_THURSDAY,
                                        TXT_SHORT_IDLE_FRIDAY,
                                        TXT_SHORT_IDLE_SATURDAY
                                    };

    MMIAPISET_GetCurrentDateStr(datestr,'-',16);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, 16 );    

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &string);

    MMIAPICOM_Wstrcat(datewstr, L"  ");
    MMIAPICOM_Wstrncat(datewstr, string.wstr_ptr,string.wstr_len);

    string.wstr_ptr = (wchar *)datewstr;
    string.wstr_len = MMIAPICOM_Wstrlen(datewstr);
    
    text_style.font_color = color;
    text_style.font = font;
    text_style.align = align;
    text_style.effect = effect;

    GUISTR_DrawTextToLCDInRect( 
                    (const GUI_LCD_DEV_INFO *)dev,
                    (const GUI_RECT_T *)&rect,
                    (const GUI_RECT_T *)&rect,
                    (const MMI_STRING_T *)&string,
                    &text_style,
                    GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT,
                    GUISTR_TEXT_DIR_AUTO
                    );
}
#endif
/*****************************************************************************/
//  Description :  Display image which is pack the GUI API for special using.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_DispImg(
                          GUI_RECT_T * display_rect, 
                          MMI_WIN_ID_T win_id, 
                          MMI_IMAGE_ID_T img_id, 
                          const GUI_LCD_DEV_INFO * dev_info_ptr, 
                          BOOLEAN is_align_mid
                          )
{
    GUI_RECT_T rect = *display_rect;
    uint16 img_width = 0, img_height = 0;
    if(is_align_mid)
    {
        GUIRES_GetImgWidthHeight(&img_width, &img_height, img_id, win_id);
        rect.left = rect.left + (rect.right - rect.left + 1 - img_width) / 2;
        rect.top = rect.top + (rect.bottom - rect.top + 1 - img_height) / 2;
        GUIRES_DisplayImg(PNULL, &rect, PNULL, win_id, img_id, dev_info_ptr);
    }
    else
    {
        GUIRES_DisplayImg(PNULL, display_rect, PNULL, win_id, img_id, dev_info_ptr);
    }
}

/*****************************************************************************/
//  Description :  Display the missed call num and missed SMS num.
//  Global resource dependence : 
//  Author: 
//  Date:
/*****************************************************************************/
PUBLIC void MMIKL_DisplayMissedEvent(MMI_WIN_ID_T win_id, GUI_RECT_T *rect, const GUI_LCD_DEV_INFO * dev_info_ptr)
{
    uint32  missed_call = 0;
    uint32  missed_sms =0;
    uint8   str_len = 0;
    wchar    missed_call_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    wchar    missed_sms_wstr[KEYLOCK_COUNT_STRING_SIZE*8]  = {0};
    char     count_str[KEYLOCK_COUNT_STRING_SIZE*8] = {0};
    MMI_STRING_T        missed_call_text = {0};
    MMI_STRING_T        missed_sms_text = {0};
    
    GUISTR_STYLE_T event_text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS|GUISTR_STATE_EFFECT;
    GUI_RECT_T disp_rect = *rect;

    uint16 img_width = 0, img_height = 0;
    
    missed_call = MMIAPICC_GetMissedCallNumber();
    missed_sms = MMIAPISMS_GetAllUnreadMsgCount()
    + MMIAPISMS_GetUnDownloadMsgCount(MN_DUAL_SYS_MAX)
    ;
#ifndef MMI_KEYLOCK_MINI_SUPPORT
    if(0 == missed_call && 0 == missed_sms)
    {
        return;
    }
#endif    
    event_text_style.effect = FONT_EFFECT_REVERSE_EDGE; 
    event_text_style.angle = ANGLE_0;
    event_text_style.align = ALIGN_VMIDDLE;
    event_text_style.font = MMI_DEFAULT_SMALL_FONT;
    event_text_style.font_color = MMI_WHITE_COLOR;
#ifndef MMI_KEYLOCK_MINI_SUPPORT    
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, win_id);
#else
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_QQVGA_KEYLOCK_MISS_CALL, win_id);
#endif
#ifndef MMI_KEYLOCK_MINI_SUPPORT
    if(missed_call > 0)
    {
        MMI_GetLabelTextByLang(TXT_KL_MISS_CALLS, &missed_call_text);
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
#ifndef MMI_KEYLOCK_MINI_SUPPORT
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_CALL_EFFECT2, dev_info_ptr, FALSE);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_QQVGA_KEYLOCK_MISS_CALL, dev_info_ptr, FALSE);
#endif
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
#ifndef MMI_KEYLOCK_MINI_SUPPORT
    }
    if(missed_sms > 0)
    {
#endif        
        SCI_MEMSET(count_str, 0, KEYLOCK_COUNT_STRING_SIZE*8);
        /* Modify by michael on 3/8/2012 for NEWMS00177382 : Add new string accorrding the num of unread msg*/	
#ifndef MMI_KEYLOCK_MINI_SUPPORT
        if(1 == missed_sms )
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES, &missed_sms_text);
        }
        else if(missed_sms > 1)
        {
            MMI_GetLabelTextByLang(TXT_KL_UNREAD_MESSAGES2, &missed_sms_text);
        }
#endif        
        /************************Modify over*************************/
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
#if defined(MAINLCD_SIZE_176X220) && defined(MMI_KEYLOCK_MINI_SUPPORT)        
        disp_rect.left += img_width+ KEY_LOCK_QQVGA_IMAGE_SPACE;                   
#else
        disp_rect.top += img_height + 4;
#endif
#ifndef MMI_KEYLOCK_MINI_SUPPORT        
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, dev_info_ptr, FALSE);
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_IDLE_KEYLOCK_MESSAGE_EFFECT2, win_id);
#else
        MMIKL_DispImg(&disp_rect, win_id, IMAGE_QQVGA_KEYLOCK_MISS_MESSAGE, dev_info_ptr, FALSE);
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_QQVGA_KEYLOCK_MISS_MESSAGE, win_id);
#endif

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
#ifndef MMI_KEYLOCK_MINI_SUPPORT        
    }
#endif   
}

/*****************************************************************************/
//  Description :  Init process flag.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_InitProcFlag(void)
{
    s_kl_proc_open_flag = 0;
}
#ifndef MMI_KEYLOCK_MINI_SUPPORT
/*****************************************************************************/
//  Description :  Set the flag that the corresponding process will be opened later.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMIKL_SetProcOpenFlag(KL_PROC_FLAG_E flag)
{
    s_kl_proc_open_flag  |= (1 << flag);
}
#endif
/*****************************************************************************/
//  Description :  Open the process by the flag.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note: Only one process would be opened.
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_OpenEffectProc(MMI_WIN_ID_T win_id)
{
    //KL_PROC_FLAG_E flag = 0;
    uint i = 0;
    BOOLEAN ret = TRUE;
    if(0 == s_kl_proc_open_flag)
    {
        return FALSE;
    }
    for(i = 0; i < PROC_MAX; i++)
    {
        if(!(s_kl_proc_open_flag & (1 << i)))
        {
            continue;
        }
        else
        {
            switch(i)
            {
            case PROC_CALL:
                //MMIAPICL_OpenCallsListWindow(MMICL_CALL_MISSED);
                //MMIAPICC_ClearMissedCallNum();                
                MMIAPIIDLE_EnterDialWin();
                break;
            case PROC_SMS:
                //MMIAPISMS_ReadNewSms(MMISMS_BOX_MT);
                MMIAPISMS_OpenMainMenuWin();
                break;
            case PROC_AUDIO:
#ifdef MMI_AUDIO_PLAYER_SUPPORT 
                //if(MMIAUDIO_IsAudioPlayerBgPaused())
                //{
                //    MMIPUB_OpenAlertWarningWin(TXT_COMMON_MP3_SUSPEND);
                //}
                //else
                {
                    MMIAPIMP3_OpenMp3Player(); 
                }   
#endif
                break;
            case PROC_CAM:
                MMK_ReturnIdleWin();
                MMIAPIDC_OpenPhotoWin(); 
                break;
            case PROC_MAINMENU:
                /* Modify by michael on 3/19/2012 for NEWMS00179923 :单独处理iphone情况，iphone idle即为MainMenu */
#if defined MMI_ISTYLE_SUPPORT 				
                if(MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
                {
                    MMK_ReturnIdleWin();
                }
                else
#endif					
                {
                    /*处理另一个bug，打开的窗口，再次open，他不会自动get focus，先关再开*/
                    MMK_ReturnIdleWin();
                    MMIAPIMENU_CreatMainMenu();
                }      
                /************************Modify over*************************/
                break;
            default:
                ret = FALSE;
                break;
            }
            break;
        }
    }
    return ret;
}

/******************************************************************************/
//  Description : Create control dynamically.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateKLWinCtrlDynamic(MMI_WIN_ID_T    win_id)
{
    GUI_BOTH_RECT_T  anim_ctrl_rect  = MMITHEME_GetFullScreenBothRect();
    MMI_CONTROL_CREATE_T create          = {0};

    //GUILABEL_INIT_DATA_T label_init_data = {0};  
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT     
    GUISCROLLKEY_INIT_DATA_T scrollkey_init_data = {0};
#endif

/************************Modify over*************************/
    GUIANIM_INIT_DATA_T    anim_init_data = {0};
#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT 
    BOOLEAN        is_mute = FALSE;
#endif
    MMISET_KEY_LOCK_EFFECT_E lock_type = 0;
    if (MMIKL_IsPreviewState())
    {
        lock_type = (uint32)MMK_GetWinAddDataPtr(win_id);
    }
    else
    {
        lock_type = MMIAPISET_GetKeyLockEffect();
    }

    
    create.parent_win_handle = MMIKL_CLOCK_DISP_WIN_ID;

    anim_init_data.both_rect = anim_ctrl_rect;
    create.init_data_ptr = &anim_init_data;

    MMIKL_CreatWallpaperCtrl(lock_type);
    
    switch(lock_type)
    {
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT 
    case MMISET_KEY_LOCK_EFFECT0:
        create.guid = SPRD_GUI_ANIM_ID;
        create.ctrl_id = MMIKL_CLOCK_ANIM_ID;
        MMK_CreateControl( &create );
#ifdef TOUCH_PANEL_SUPPORT 
        create.ctrl_id = MMIKL_PRESS_ANIM_BG_ID;
        MMK_CreateControl( &create );
        
        create.ctrl_id = MMIKL_PRESS_ANIM_CALL_ID;
        MMK_CreateControl( &create );
        
        create.ctrl_id = MMIKL_PRESS_ANIM_MESSAGE_ID;
        MMK_CreateControl( &create );
        
        create.ctrl_id = MMIKL_PRESS_ANIM_EVENT_ID;
        MMK_CreateControl( &create );
#endif
        //create softkey control
        {
            MMI_CONTROL_CREATE_T softkey_create = {0};
            GUISOFTKEY_INIT_DATA_T softkey_init = {0};

            softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
            softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
            softkey_create.init_data_ptr     = &softkey_init;
            softkey_create.parent_win_handle = MMIKL_CLOCK_DISP_WIN_ID;
         
            MMK_CreateControl(&softkey_create);
            MMK_SetWinSoftkeyCtrlId(MMIKL_CLOCK_DISP_WIN_ID,softkey_create.ctrl_id);
        }
#ifdef TOUCH_PANEL_SUPPORT 
        //btn_init_data.both_rect = anim_ctrl_rect;
        {
            MMI_CONTROL_CREATE_T button_create = {0};

            GUIBUTTON_INIT_DATA_T btn_init_data = {0};
            btn_init_data.bg.img_id = IMAGE_IDLE_KEYLOCK_PRESS_UP;
            btn_init_data.bg.bg_type = GUI_BG_IMG;
            button_create.init_data_ptr = &btn_init_data;
            button_create.guid = SPRD_GUI_BUTTON_ID;
            button_create.ctrl_id = MMIKL_PRESS_BOTTON_ID;
            button_create.parent_win_handle = MMIKL_CLOCK_DISP_WIN_ID;
            MMK_CreateControl(&button_create);
        }
#endif
        break;
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT                
    case MMISET_KEY_LOCK_EFFECT1:
        break;
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT        
    case MMISET_KEY_LOCK_EFFECT2:
        create.guid = SPRD_GUI_ANIM_ID;
        create.ctrl_id = MMIKL_CLOCK_ANIM_ID;
        MMK_CreateControl( &create );

        scrollkey_init_data.left_scroll_skin_idx = GUISCROLLKEY_SKIN_GREEN_ID;
        scrollkey_init_data.left_scroll_key_id = IMAGE_IDLE_KEYLOCK_L_UNLOCK;
        scrollkey_init_data.left_scroll_text_id = STXT_UNLOCK;
        
        scrollkey_init_data.right_scroll_skin_idx = GUISCROLLKEY_SKIN_GRAY_ID;
        
        switch (MMIAPIENVSET_GetActiveModeId())
        {
        case MMIENVSET_STANDARD_MODE:  // 普通环境
        case MMIENVSET_INSIDE_MODE:      // 车内环境
        case MMIENVSET_NOISY_MODE:       // 户外环境
            scrollkey_init_data.right_scroll_text_id = TXT_KL_SOUND_OFF;
            break;

        case MMIENVSET_SILENT_MODE:      // 安静环境               
        case MMIENVSET_MEETING_MODE:     // 会议环境
            scrollkey_init_data.right_scroll_text_id = TXT_KL_SOUND_ON;
            is_mute = TRUE;
            break;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT 
        case MMIENVSET_EARPHONE_MODE:
        case MMIENVSET_BLUETOOTH_MODE:
            if (MMIAPIENVSET_GetMuteState())
            {
                scrollkey_init_data.right_scroll_text_id = TXT_KL_SOUND_ON;
                is_mute = TRUE;
            }
            else
            {
                scrollkey_init_data.right_scroll_text_id = TXT_KL_SOUND_OFF;
            }
            break;
#endif
        default:
            scrollkey_init_data.right_scroll_text_id = TXT_KL_SOUND_OFF;
            break;
        }
        
        if(is_mute)
        {
            scrollkey_init_data.right_scroll_key_id = IMAGE_IDLE_KEYLOCK_R_VIBRATE_ON;
        }
        else
        {
            scrollkey_init_data.right_scroll_key_id = IMAGE_IDLE_KEYLOCK_R_SOUND_ON;
        }
        
        create.guid = SPRD_GUI_SCROLLKEY_ID;
        create.ctrl_id = MMICOMMON_SCROLLKEY_CTRL_ID;
        create.init_data_ptr = &scrollkey_init_data;   
        MMK_CreateControl( &create ); 
/* Modify by michael on 2/23/2012 for NEWMS00171320 : 修改俄语下text的显示字体大小（俄语比较长）*/
#if defined  (MAINLCD_SIZE_240X400) || defined (MAINLCD_SIZE_240X320)  
   {
	 GUI_FONT_T scrollkey_font = SONG_FONT_20;
	 MMISET_LANGUAGE_TYPE_E language_type = MMISET_LANGUAGE_ENGLISH;
 
        MMIAPISET_GetLanguageType(&language_type);
#ifdef IM_RUSSIAN_SUPPORT
        if(MMISET_LANGUAGE_RUSSIAN ==  language_type)
        {
            GUISCROLLKEY_Setfont(win_id,MMICOMMON_SCROLLKEY_CTRL_ID,PNULL,&scrollkey_font);
        } 
#endif
   }
#endif  
/************************Modify over*************************/        

#ifdef GUIF_SCROLLKEY
        MMK_SetWinScrollkeyCtrlId(create.parent_win_handle, MMICOMMON_SCROLLKEY_CTRL_ID);
#endif
        break;
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT        
    case MMISET_KEY_LOCK_EFFECT3:
#endif        

#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT        
    case MMISET_KEY_LOCK_EFFECT4:
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT
    case MMISET_KEY_LOCK_EFFECT5:
#endif

#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT
    case MMISET_KEY_LOCK_EFFECT6:
 #endif
 #if 0//def MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
      case MMISET_KEY_LOCK_DOUBLE_CIRCLE:
        #ifndef MMI_PDA_SUPPORT
        {
            MMI_CONTROL_CREATE_T softkey_create = {0};
            GUISOFTKEY_INIT_DATA_T softkey_init = {0};

            softkey_create.ctrl_id           = MMITHEME_GetSoftkeyCtrlId();
            softkey_create.guid              = SPRD_GUI_SOFTKEY_ID;
            softkey_create.init_data_ptr     = &softkey_init;
            softkey_create.parent_win_handle = MMIKL_CLOCK_DISP_WIN_ID;
         
            MMK_CreateControl(&softkey_create);
            MMK_SetWinSoftkeyCtrlId(MMIKL_CLOCK_DISP_WIN_ID,softkey_create.ctrl_id);
        }
        #endif
#endif
    default:
        break;
    }
}

#ifdef TOUCHPANEL_TYPE
/*****************************************************************************/
//  Description : create button layer.
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateButtonLayer(MMI_WIN_ID_T win_id)
{
    UILAYER_CREATE_T   create_info = {0};
    UILAYER_RESULT_E  result = UILAYER_RESULT_SUCCESS;

    /*创建BUTTON使用的层*/
    create_info.lcd_id          = LCD_ID_0;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    
    if(!MMITHEME_IsMainScreenLandscape())
    {
        create_info.width           = MMI_MAINSCREEN_WIDTH;
        create_info.height          = MMI_MAINSCREEN_HEIGHT;
    }
    else
    {
        create_info.width           = MMI_MAINSCREEN_HEIGHT;
        create_info.height          = MMI_MAINSCREEN_WIDTH;
    }

    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   

    result = UILAYER_CreateLayer(&create_info, &s_preview_button_layer_info);
    if(UILAYER_RESULT_SUCCESS == result)
    {
        UILAYER_APPEND_BLT_T  append_blt = {0};
        UILAYER_Clear(&s_preview_button_layer_info);
        append_blt.layer_level = UILAYER_LEVEL_NORMAL;
        append_blt.lcd_dev_info = s_preview_button_layer_info;
        UILAYER_AppendBltLayer(&append_blt); 
        return TRUE;
    }
    else
    {
        s_preview_button_layer_info.block_id = UILAYER_NULL_HANDLE;
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : CreateButton
//  Global resource dependence : none
//  Author: arvin.wu
//  Note: 
/*****************************************************************************/
LOCAL void CreateButton(MMI_WIN_ID_T win_id, MMI_CTRL_ID_T ctrl_id, MMI_TEXT_ID_T text_id, GUI_BOTH_RECT_T both_rect)
{
    GUIBUTTON_INIT_DATA_T   init_data = {0};
    MMI_HANDLE_T handle = 0;
    GUI_FONT_ALL_T font_all = {0};
    GUI_BG_T btn_bg = {0};
    
    init_data.both_rect = both_rect;
    init_data.bg.bg_type = GUI_BG_IMG;
#ifdef MMI_PDA_SUPPORT                
    init_data.bg.img_id = IMAGE_PUBWIN_BUTTON_PDA;
#else
    init_data.bg.img_id = IMAGE_SOFTKEY_BG;
#endif

    handle = GUIBUTTON_CreateDynamic(win_id/*MMK_GetFocusWinId()*/, ctrl_id, &init_data);
    if (0 !=handle)
    {
        GUIBUTTON_SetTextId(handle, text_id);

        btn_bg.bg_type = GUI_BG_IMG;
#ifdef GUIF_SOFTKEY_PDASTYLE                    
        btn_bg.img_id = IMAGE_PUBWIN_BUTTON_HL_PDA;    
#else
        btn_bg.img_id = IMAGE_SOFTKEY_BG_DOWN;    
#endif
        GUIBUTTON_SetPressedBg(handle, &btn_bg);
        GUIBUTTON_SetRunSheen(handle, FALSE);
#ifdef MMI_PDA_SUPPORT
        font_all.color = MMI_BLACK_COLOR;
#else
        font_all.color = MMI_WHITE_COLOR;
#endif
        font_all.font = MMI_DEFAULT_NORMAL_FONT;
        GUIBUTTON_SetFont(handle, &font_all);
    }
}
/*****************************************************************************/
//  Description : SetStaticWallpaperOKButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetOKButtonCallback(void)
{
    MMK_SendMsg(MMIKL_CLOCK_DISP_WIN_ID, MSG_APP_OK, PNULL);
}

/*****************************************************************************/
//  Description : SetStaticWallpaperCancelButtonCallback
//  Global resource dependence : none
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
LOCAL void SetCancelButtonCallback(void)
{
    MMK_SendMsg(MMIKL_CLOCK_DISP_WIN_ID, MSG_APP_CANCEL, PNULL);
}

/*****************************************************************************/
//  Description : CreateSetPreviewButton
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CreateSetPreviewButton(MMI_WIN_ID_T win_id,MMISET_KEY_LOCK_EFFECT_E lock_type)
{
    MMI_CTRL_ID_T   ctrl_ok_id = MMIKL_BUTTON_SOFTKEY_LEFT_CTRL_ID;
    MMI_CTRL_ID_T   ctrl_cancel_id = MMIKL_BUTTON_SOFTKEY_RIGHT_CTRL_ID;
    uint16          button_icon_width = 0;
    uint16          button_icon_height = 0;
    uint16          layer_width = 0;
    uint16          layer_height = 0;
    GUI_RECT_T      rc = {0};
    GUI_BOTH_RECT_T  both_rect = {0};

#ifdef MMI_PDA_SUPPORT            
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_PUBWIN_BUTTON_PDA, win_id);
#else
    GUIRES_GetImgWidthHeight(&button_icon_width, &button_icon_height, IMAGE_SOFTKEY_BG, win_id);
#endif
    GUILCD_GetLogicWidthHeight(MAIN_LCD_ID, &layer_width, &layer_height);
    button_icon_width = layer_width/3 - 4;

    rc.left = ((layer_width/2) - button_icon_width)/2;
    rc.right = rc.left + button_icon_width;

#if defined MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
    if (MMISET_KEY_LOCK_EFFECT4 == lock_type)
    {
        rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN*4;
    }
    else
#endif
    {
#if defined MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
        if (MMISET_KEY_LOCK_EFFECT0 == lock_type)
        {
            rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN*3;
        }
        else
#endif
        {
            rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN;
        }
    }

    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;
    CreateButton(win_id, ctrl_ok_id, TXT_COMMON_OK, both_rect);

    rc.left = ((layer_width/2) - button_icon_width)/2 + (layer_width/2);
    rc.right = rc.left + button_icon_width;

#if defined MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
    if (MMISET_KEY_LOCK_EFFECT4 == lock_type)
    {
        rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN*4;
    }
    else
#endif
    {
#if defined MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
        if (MMISET_KEY_LOCK_EFFECT0 == lock_type)
        {
            rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN*3;
        }
        else
#endif
        {
            rc.bottom = layer_height - MMI_KEYLOCK_PREVIEW_BUTTON_BOTTOM_MARGIN;
        }
    }
    rc.top = rc.bottom - button_icon_height;
    both_rect.v_rect = rc;
    both_rect.h_rect = rc;
    CreateButton(win_id, ctrl_cancel_id, STXT_CANCEL, both_rect);

    GUIBUTTON_SetCallBackFunc(ctrl_ok_id, (GUIBUTTON_CALLBACK_FUNC)SetOKButtonCallback);
    GUIBUTTON_SetCallBackFunc(ctrl_cancel_id, (GUIBUTTON_CALLBACK_FUNC)SetCancelButtonCallback);

    if(CreateButtonLayer(win_id))
    {
        MMK_SetCtrlLcdDevInfo(ctrl_ok_id, &s_preview_button_layer_info);
        MMK_SetCtrlLcdDevInfo(ctrl_cancel_id, &s_preview_button_layer_info);
    }
}
#endif


/******************************************************************************/
//  Description : handle kl display window msg
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleEffeckKLWinMsg(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param
                        )
{   
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_RESULT_E ret = MMI_RESULT_FALSE;
    MMISET_KEY_LOCK_EFFECT_E lock_type = MMISET_KEY_LOCK_EFFECT_MAX;
    
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
    static MMISET_WALLPAPER_SET_T s_wallpaper_info = {0};
    #endif
    
    if (MMIKL_IsPreviewState())
    {
	    lock_type = (uint32)MMK_GetWinAddDataPtr(win_id);
    }
    else
    {
        lock_type = MMIAPISET_GetKeyLockEffect();
    }   
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if(
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT
        MMISET_KEY_LOCK_EFFECT0 != lock_type && 
#endif
#ifdef MMI_KEY_LOCK_DOUBLE_CIRCLE_SUPPORT
        MMISET_KEY_LOCK_DOUBLE_CIRCLE != lock_type &&
#endif
        MMISET_KEY_LOCK_EFFECT_MAX != lock_type)
        {
            MMK_SetWinSupportAngle(win_id, WIN_SUPPORT_ANGLE_0);
            MMK_SetWinAngle(win_id, LCD_ANGLE_0, FALSE);  
        }

        CreateKLWinCtrlDynamic(win_id);
#ifndef EFFECT_RIPPLE_SUPPORT //144786 
        GUIWIN_CreateStbDyn(win_id);
		
#ifdef PDA_UI_DROPDOWN_WIN
        GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
		GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
        GUIWIN_SetStbBgIsDesplay(win_id, TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
        GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
        GUIWIN_SetStbState( win_id, GUISTATUSBAR_STATE_SCROLLMSG_PAUSE, TRUE);
#endif

        MMIKL_InitProcFlag();    
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT        
        MMIKL_SetPswFlagPsw(TRUE);
#endif

#ifndef TOUCH_PANEL_SUPPORT 
#ifdef UI_MULTILAYER_SUPPORT
        CreateKeyPadUnLockTipsLayer();
#endif
#endif
    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
        MMIAPISET_GetWallpaperSetInfo(&s_wallpaper_info);
    #endif
        break;
    #ifdef MMI_PDA_SUPPORT
    case MSG_KEYLONG_RED://for cr NEWMS00191055
        return MMI_RESULT_TRUE;
        case MSG_KEYLONG_POWER:
    	return MMI_RESULT_FALSE;
    
    #endif
    default:
        break;
    }
    #ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
        #ifdef SLIDESHOW_WALLPAPER_SUPPORT
    
    if(MMISET_WALLPAPER_LIVE == s_wallpaper_info.wallpaper_type&&
        MMI_LIVE_WALLPAPER_TYPE_SLIDESHOW == s_wallpaper_info.wallpaper_id)
    {
        #if ((defined MMI_PDA_SUPPORT)&&(defined MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT)) 
        if(lock_type != MMISET_KEY_LOCK_EFFECT0)
        #endif
        {
            MMIAPILIVEWALLPAPER_HandleEvent(win_id, msg_id, param);
        }
    }
        #endif
    #endif
    
    if (MMIKL_IsPreviewState())
    {
		if(MSG_KEYUP_RED == msg_id)
	    {
            MMK_CloseWin(win_id);
	    }
        else
		{
			RUN_PROC_BEGIN(s_win_handle_func_arr, lock_type)
				ret = ((HANDLE_FUNC)(s_win_handle_func_arr[lock_type]))(win_id, msg_id, param);    
			RUN_PROC_END
		}
    }
    else
    {
        RUN_PROC_BEGIN(s_win_handle_func_arr, lock_type)
            ret = ((HANDLE_FUNC)(s_win_handle_func_arr[lock_type]))(win_id, msg_id, param);    
        RUN_PROC_END
    }


    if(ret)
    {
        /* if is MSG_CLOSE_WINDOW msg, we should do some other thing, otherwise return immediately */
        if(msg_id != MSG_CLOSE_WINDOW && msg_id != MSG_OPEN_WINDOW &&msg_id != MSG_FULL_PAINT && msg_id != MSG_BACKLIGHT_TURN_ON && msg_id != MSG_GET_FOCUS )
        {
            return ret;
        }
    }
    
    switch(msg_id)
    {
    #ifdef TOUCHPANEL_TYPE
    case MSG_OPEN_WINDOW:
        if (MMIKL_IsPreviewState())//创建button
        {
            CreateSetPreviewButton(win_id,lock_type);
        }
        break;
        
    case MSG_FULL_PAINT:
        if (MMIKL_IsPreviewState())//重新 append button 
        {
        	if (UILAYER_IsLayerActive(&s_preview_button_layer_info))
    	    {
                UILAYER_APPEND_BLT_T  append_blt = {0};
                append_blt.layer_level = UILAYER_LEVEL_NORMAL;
                append_blt.lcd_dev_info = s_preview_button_layer_info;
                UILAYER_AppendBltLayer(&append_blt); 
    	    }
        }
        break;
 #endif
     case MSG_BACKLIGHT_TURN_ON:
     case MSG_GET_FOCUS:
        if (MMITHEME_IsDynaAllocWallpaperBuf())
        {
            //update wallpaper
            MMIKL_CreatWallpaperCtrl(lock_type);
        }
        
        break;
    case MSG_CLOSE_WINDOW:
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        if(MMIAPISET_IsKLPswOn() && MMIKL_IsCanOpenPsw())
        {
            MMIAPIKL_OpenPswWin(FALSE, PNULL);
        }
        else
#endif        
        {
            if (!MMIKL_IsPreviewState())
            {
                MMIKL_OpenEffectProc(win_id);

#ifdef MMIWIDGET_WEATHER_ANIM
                MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_NEED_ANIM, PNULL, 0);
#endif       
            }
        }

        if (!MMIKL_IsPreviewState())
        {
            MMIDEFAULT_StartAutoKeylockTimer();
            MMIDEFAULT_TurnOnLCDBackLight();    
        }
        
        MMIKL_ReSetWinStatus();
        MMIAPIKL_PlayKeyLockRing();
        
        //stop keypad display timer id
        MMIKL_StopTipsShowTimer();
        MMIKL_StopUnlockConfirmTimer();
#ifdef MMI_SMS_CHAT_SUPPORT
        if(MMIAPISMS_GetIsKeyLockNeedCountSms())
        {
            MMIAPISMS_SetIsKeyLockNeedCountSms(FALSE);
            MAIN_SetMsgState(TRUE);
        }
#endif

#ifndef TOUCH_PANEL_SUPPORT  
#ifdef UI_MULTILAYER_SUPPORT       

        UILAYER_RELEASELAYER(&s_keylock_tips_layer_handle); /*lint !e506 !e774 !e831*/
#endif
#endif
#ifdef TOUCHPANEL_TYPE
        if(UILAYER_IsLayerActive(&s_preview_button_layer_info))
        {
            UILAYER_RELEASELAYER(&s_preview_button_layer_info);   /*lint !e506 !e774*/
        }
        s_preview_button_layer_info.block_id = UILAYER_NULL_HANDLE;
#endif
        break;
        
    case MSG_CTL_ANIM_DISPLAY_IND:
    case MSG_CTL_ANIM_GIF_UPDATE:   
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT         
        if(lock_type != MMISET_KEY_LOCK_EFFECT0)
#endif
        {
            GUIANIM_DISPLAY_IND_T   display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
            if (MMK_GetCtrlHandleByWin(win_id,MMIKL_MAIN_ANIM_BG_ID) == display_ind.ctrl_handle)
            {
                if( GUIANIM_RESULT_SUCC != display_ind.result)
                {
                    #ifdef SLIDESHOW_WALLPAPER_SUPPORT
                    if(MMILiveWallpaper_NeedResetWallpaper(s_wallpaper_info.wallpaper_id))
                    #endif
                    {
                        MMIAPISET_ResetDefaultWallpaper();
                    }
               				
                }
				
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                }
            }
        }
        break;
       
    case MSG_KEYLOCK_UNREAD_MESSAGE:
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
        break;
    case MSG_KEYDOWN_R:
    case MSG_KEYDOWN_T:
    case MSG_KEYDOWN_Y:
    case MSG_KEYDOWN_U:
    case MSG_KEYDOWN_F:
    case MSG_KEYDOWN_G:
    case MSG_KEYDOWN_H:
    case MSG_KEYDOWN_J:
    case MSG_KEYDOWN_V:
    case MSG_KEYDOWN_B:
    case MSG_KEYDOWN_N:
    case MSG_KEYDOWN_M:
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
    case MSG_KEYDOWN_0:
        if (!MMIKL_IsPreviewState())
        {
#ifdef MMI_KEY_LOCK_EFFECT_TYPE0_SUPPORT 
            //if(lock_type != MMISET_KEY_LOCK_EFFECT0)
#endif
            {
                msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
#ifdef OLD_CC_SUPPORT  //for 217 ram cutdown:remove old cc
                MMIAPICC_OpenEmergencyWin(msg_id);
#endif
            }
        }
        break;
#ifndef TOUCH_PANEL_SUPPORT               
    case MSG_APP_OK:
        if ((MMIDEFAULT_IsBacklightOn()))
        {
#ifdef UI_MULTILAYER_SUPPORT            
            DisplayKeyPadUnLockTipsLayer(TXT_KEYLOCK_KEYPAD_UNLOCK_TIP_2);
#endif
            MMIKL_StartTipsShowTimer(win_id);
			MMIKL_SetWinStatus(KL_UNLOCKED_ONCE);
            MMIKL_StartUnlockConfirmTimer(win_id);
         }       

        break;
    case MSG_APP_CANCEL:
        if (KL_LOCKED == MMIKL_GetWinStatus())
        {
		//michael wang for NEWMS00184366
            MMIKL_StartTipsShowTimer(win_id);
#ifdef UI_MULTILAYER_SUPPORT        
            DisplayKeyPadUnLockTipsLayer(TXT_KEYLOCK_KEYPAD_UNLOCK_TIP);
#endif
        }
        else if ((MMIDEFAULT_IsBacklightOn()) && 
            (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus()))
        {
            //s_e_mmi_kl_win_status = KL_UNLOCKED;
            MMIKL_SetWinStatus(KL_UNLOCKED);
            //检查s_st_mmi_kl_event，判断是否有未读SMS、MMS、MISSED CALL发生
            //如果有，需要调用这3个模块的函数，来弹出各自的提示窗口。
            MMK_CloseWin(win_id);   
#ifdef MMIWIDGET_WEATHER_ANIM
            MMK_PostMsg(MAIN_IDLE_WIN_ID, MSG_WEATHER_NEED_ANIM, PNULL, 0);
#endif            
            MMIKL_CheckPromptStatus();
        }
        break;
    case MSG_KEYDOWN_WEB:
    case MSG_KEYDOWN_GREEN:
    case MSG_KEYDOWN_UP:
    case MSG_KEYDOWN_DOWN:
    case MSG_KEYDOWN_LEFT:
    case MSG_KEYDOWN_RIGHT:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYDOWN_HASH:

 
#ifdef UI_MULTILAYER_SUPPORT
        DisplayKeyPadUnLockTipsLayer(TXT_KEYLOCK_KEYPAD_UNLOCK_TIP);
#endif
        MMIKL_StartTipsShowTimer(win_id);
        MMIKL_SetWinStatus(KL_LOCKED);
        break;

    case MSG_TIMER:
        if (MMIKL_GetUnlockConfirmTimerID() == *(uint8*)param)
        {
            //stop timer
            MMIKL_StopUnlockConfirmTimer();
            if (KL_UNLOCKED_ONCE == MMIKL_GetWinStatus())
            {
                MMIKL_SetWinStatus(KL_LOCKED);
            }
        }
        else if(MMIKL_GetTipsTimerID() == *(uint8*)param)
        {
#ifndef TOUCH_PANEL_SUPPORT 
#ifdef UI_MULTILAYER_SUPPORT
            HideKeyPadUnLockTipsLayer();
#endif
#endif            
            MMIKL_StopTipsShowTimer();
        }
        break;

    //双击耳机键，呼出最近一次拨出电话
    case MSG_APP_HEADSET_BUTTON: 
        MMK_NeedKbdClkNotifyOnce();
        break;

     case MSG_KEY_DBLCLK:
        MMIAPICL_MakeCallByRecordIndex(MMICL_CALL_DIALED,0);
        break;
   

        /* Modify by michael on 3/25/2012 for NEWMS00184065 : */
#ifndef MMI_PDA_SUPPORT
#ifdef TAKESCREENPHOTO_SUPPORT  
    case MSG_KEYUP_CAMERA:
        {
            MMI_TEXT_ID_T text_id = 0;
            uint32 dis_time =1000;
            
            if (MMITSCRPHOTO_TakePhoto())
            {
                text_id = TXT_PRINTSCREEN_COMPLETE;
                //    print_alert_img = IMAGE_PUBWIN_SUCCESS;
            }
            else
            {
                text_id = TXT_MMI_SAVEFAILED;
                // print_alert_img = IMAGE_PUBWIN_WARNING;
            }           
            MMIPUB_DisplayTipsIDEx(PNULL, text_id, MMIPUB_TIPS_DIS_MIDDLE, &dis_time);
        }
        break;
#endif        
#endif
        /************************Modify over*************************/
#endif
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}
/*****************************************************************************/
//  Description : Handle Anim Ctrl decode result
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleAnimDisInfo(
                        MMI_WIN_ID_T    win_id, 
                        uint16          msg_id, 
                        DPARAM          param,
                        uint16    *fail_file_num_ptr
                        )
{
    GUIANIM_DISPLAY_IND_T   display_ind = *(GUIANIM_DISPLAY_IND_T *)param;
    if (MMK_GetCtrlHandleByWin(win_id,MMIKL_MAIN_ANIM_BG_ID) == display_ind.ctrl_handle)
    {
        if( GUIANIM_RESULT_SUCC != display_ind.result)
        {
            (*fail_file_num_ptr)++;
            MMIAPISET_ResetDefaultWallpaper();
        }
				
        if (MMK_IsFocusWin(win_id))
        {
                    MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
    }
}
#ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
//  Description : MMILIVEWALLPAPER_GetPreviewFgInfo
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMILIVEWALLPAPER_GetKLFgInfo(LW_FG_INFO *lw_fg_info_ptr)
{
    
    if(lw_fg_info_ptr)
    {
        lw_fg_info_ptr->disp_dev_info.lcd_id = 0;
        lw_fg_info_ptr->disp_dev_info.block_id = 0;
        
        lw_fg_info_ptr->lw_fg_pos_info.rc = MMITHEME_GetFullScreenRect();//获得前景层的区域信息
        lw_fg_info_ptr->lw_fg_pos_info.x = 0;//由于预览界面没有前景层，所以前景层始终不变
        lw_fg_info_ptr->lw_fg_pos_info.y = 0;//由于预览界面没有前景层，所以前景层始终不变
    }
}
#endif

#endif	//MMI_KEY_LOCK_EFFECT_SUPPORT
#endif
#endif

