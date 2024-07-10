/*****************************************************************************
** File Name:      mmikl_Istyleslide.c                                       *
** Author:         jian.ma                                                   *
** Date:           12/12/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe Iphone style slide function *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011        jian.ma          Create
******************************************************************************/

#ifndef _MMIKL_ISTYLESLIDE_C_

#include "mmi_app_keylock_trc.h"
#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmikl_Istyleslide.h"
#include "ui_layer.h"
#include "guistring.h"
#include "guires.h"
#include "mmi_id.h"
#include "mmikl_export.h"
#include "mmidisplay_data.h"
#include "mmi_textfun.h"
#include "mmi_keylock_position.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmikl_internal.h"
#include "mmiset_export.h"
#include "cafcontrol.h"
#include "mmiset_nv.h"
#include "mmikl_effectmain.h"
#include "mmi_wallpaper_export.h"
#include "guistatusbar.h"
#include "mmk_tp.h"
#include "mmi_appmsg.h"
#include "mmidc_export.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

#define KEY_LOCK_EFFECT4_TP_MOVE_TIME   20
#define KEY_LOCK_EFFECT4_AUTO_MOVE_TIME       30
#define KEY_LOCK_EFFECT4_TEXT_SCROLL_TIME       80
#define KEY_LOCK_EFFECT4_TEXT_SCROLL_START_TIME 1000
#define KEY_LOCK_EFFECT4_AUTO_MOVE_FRAME_NUM    5
#define KEY_LOCK_EFFECT4_TEXT_ALPHA         128

#define KEY_LOCK_EFFECT4_SCROLL_TEXT_WIDTH    30    
#define KEY_LOCK_EFFECT4_SCROLL_TEXT_FRAME_NUM  15



typedef struct
{
    GUI_LCD_DEV_INFO layer;
    GUI_LCD_DEV_INFO text_layer;
    uint16 layer_pos_x;
    uint16 layer_pos_y;
    uint16 text_layer_pos_x;
    uint16 text_layer_pos_y;
    uint16 tp_move_timer;
    uint16 auto_move_timer;
    uint16 text_scroll_timer;
}MMI_KEY_LOCK_EFFECT4_INFO_T;

LOCAL MMI_KEY_LOCK_EFFECT4_INFO_T s_key_lock_effect4_info = {0};

#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
LOCAL BOOLEAN s_is_show_camera_icon = FALSE;
#endif

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
//  Description :  set key lock control parameters of effect type4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect4(void);


/*****************************************************************************/
//  Description :  Get text layer of effect4 type.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetTextLayerEffect4(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect4_info.text_layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect4_info.text_layer))
        {
            lcd_dev_info = s_key_lock_effect4_info.text_layer;
        }
    }
    return lcd_dev_info;
}


/*****************************************************************************/
//  Description :  Create text layer of effect4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateTextLayerEffect4(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = GetTextLayerEffect4();
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 str_width = 0;
    GUISTR_STYLE_T style = {0};
    MMI_STRING_T   string = {0};
    GUISTR_STATE_T       state = 0;
    uint16 image_width = 0, image_height = 0;
    uint16 slide_bg_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;


    //remove old layrer
    if (UILAYER_IsLayerActive(&layer))
    {
        UILAYER_RemoveBltLayer(&layer);
    }

    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT4_SLIDE_BLOCK, MMIKL_CLOCK_DISP_WIN_ID);
    
    style.font = MMI_DEFAULT_BIG_FONT;
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
    if(s_is_show_camera_icon)
    {
        MMI_GetLabelTextByLang(TXT_KL_EFFECT4_SLIDE_RIGHT_ADD_DC, &string);
    }
    else
#endif
    {
        MMI_GetLabelTextByLang(TXT_KL_EFFECT4_SLIDE_RIGHT, &string);
    }
    //MMI_GetLabelTextByLang(TXT_KL_EFFECT4_SLIDE_RIGHT, &string);
    str_width = GUISTR_GetStringWidth(&style, &string, state);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIKL_CLOCK_DISP_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = str_width;
    create_info.height = image_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    result = UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect4_info.text_layer = layer;
        
        /* ÎÄ×ÖÔÚ»¬¿éÓÒ±ß¾ÓÖÐÏÔÊ¾*/
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT     
        if(s_is_show_camera_icon)
        {
            slide_bg_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN;
        }
        else
#endif
        {
            slide_bg_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
        }
        
        s_key_lock_effect4_info.text_layer_pos_x = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_X + image_width + /*»¬¿éÓÒ±ß*/
            (lcd_width - KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_X - image_width - slide_bg_right_margin - str_width)/2;/*¾ÓÖÐ*/
 
        s_key_lock_effect4_info.text_layer_pos_y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;
        UILAYER_SetLayerPosition(&layer, s_key_lock_effect4_info.text_layer_pos_x, s_key_lock_effect4_info.text_layer_pos_y);
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_effect4_info.text_layer.block_id = 0;
    }
}


/*****************************************************************************/
//  Description :  Append move layer of effect4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendTextLayerEffect4(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetTextLayerEffect4();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}


/*****************************************************************************/
//  Description :  Get move layer of effect4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetMoveLayerEffect4(void)
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if(s_key_lock_effect4_info.layer.block_id != 0)
    {
        if (UILAYER_IsMultiLayerEnable() && UILAYER_IsLayerActive(&s_key_lock_effect4_info.layer))
        {
            lcd_dev_info = s_key_lock_effect4_info.layer;
        }
    }
    return lcd_dev_info;
}

/*****************************************************************************/
//  Description :  Append move layer of effect4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void AppendMoveLayerEffect4(void)
{
    UILAYER_APPEND_BLT_T append_layer;

    append_layer.lcd_dev_info = GetMoveLayerEffect4();
    if(append_layer.lcd_dev_info.block_id != 0)
    {
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer); 
    }
}

/*****************************************************************************/
//  Description :  set move layer pos off effect3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetMoveLayerPosEffect4(uint16 x)
{
    GUI_LCD_DEV_INFO layer = GetMoveLayerEffect4();

    if(layer.block_id != 0)
    {
        UILAYER_SetLayerPosition(&layer, x, s_key_lock_effect4_info.layer_pos_y);
    }    
}

/*****************************************************************************/
//  Description :  set key lock control parameters of effect type3.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void CreateMoveLayerEffect4(void)
{
    UILAYER_RESULT_E result  = 0;
    
    UILAYER_CREATE_T    create_info = {0};
    GUI_LCD_DEV_INFO     layer = {0};
    uint16 lcd_width = 0, lcd_height = 0;
    uint16 image_width = 0, image_height = 0;
    
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_KL_EFFECT4_SLIDE_BLOCK, MMIKL_CLOCK_DISP_WIN_ID);
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
    
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = MMIKL_CLOCK_DISP_WIN_ID;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = image_width;
    create_info.height = image_height;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = FALSE;
    result = UILAYER_CreateLayer(
        &create_info,  
        &layer
        );
    
    if(UILAYER_RESULT_SUCCESS == result)
    {
        s_key_lock_effect4_info.layer = layer;
//        AppendMoveLayerEffect4();
        s_key_lock_effect4_info.layer_pos_x = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_X;
        s_key_lock_effect4_info.layer_pos_y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;
        UILAYER_SetLayerPosition(&layer, s_key_lock_effect4_info.layer_pos_x, s_key_lock_effect4_info.layer_pos_y);
        UILAYER_Clear(&layer);
    }
    else
    {
        s_key_lock_effect4_info.layer.block_id = 0;
    }
}

/*****************************************************************************/
//  Description :  set key lock control parameters of effect type4.
//  Global resource dependence :
//   Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetKeyLockControlParamEffect4(void)
{
    GUI_BG_T                    bg = {0};

    CreateTextLayerEffect4();
    CreateMoveLayerEffect4();
    
    //set softkey common bg
    GUIWIN_SetSoftkeyCommonBg(MMIKL_CLOCK_DISP_WIN_ID,&bg);

}

/*****************************************************************************/
//  Description : Move layer auto move by anim frame index.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void MoveLayerAutoMoveEffect4(uint16 frame_index)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};
    int16 layer_pos_x = 0, layer_pos_y = 0;
    uint16 new_layer_pos_x = 0;
    uint16 move_distance = 0;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;
    uint16 slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
    
    layer = GetMoveLayerEffect4();

    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);    
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT        
        if(s_is_show_camera_icon)
        {
            slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN;
        }
        else
#endif
        {
            slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
        }
        
        move_distance = (lcd_width- KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN -slide_right_margin)/2 - layer_info.layer_width;
        new_layer_pos_x = KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN + (lcd_width- KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN -slide_right_margin)/2 + move_distance * frame_index / KEY_LOCK_EFFECT4_AUTO_MOVE_FRAME_NUM;
        UILAYER_SetLayerPosition(&layer, new_layer_pos_x, layer_pos_y);
    }    
}

/*****************************************************************************/
//  Description : make a judgement if tp move distance is enough to unlock key.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsDisEnoughToUnlockEffect4(uint16 distance)
{
    GUI_LCD_DEV_INFO layer = {0};
    uint16           lcd_width = 0;
    uint16           lcd_height = 0;
    uint16           slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
    
    layer = GetMoveLayerEffect4();
    
    if(layer.block_id != 0)
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
        if(s_is_show_camera_icon)
        {
            slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN;
        }
        else
#endif
        {
            slide_right_margin = KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
        }
        
        if(s_key_lock_effect4_info.layer_pos_x + distance > (lcd_width- KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN -slide_right_margin)/2)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : make a judgement if tp point in move layer area.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpPointInMoveLayerEffect4(uint16 x, uint16 y)
{
    GUI_LCD_DEV_INFO layer = {0};
    UILAYER_INFO_T layer_info = {0};
    int16 layer_pos_x = 0, layer_pos_y = 0;
    BOOLEAN res = FALSE;
    
    layer = GetMoveLayerEffect4();

    if(layer.block_id != 0)
    {
        UILAYER_GetLayerInfo(&layer, &layer_info);
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);    
        
        /* To make the unlock easier, we enlarge the rect's top and bottom 5 pixels */
        if(x >= layer_pos_x && x <= layer_pos_x + layer_info.layer_width && y >= layer_pos_y - 5 && y <= layer_pos_y + layer_info.layer_height + 5)
        {
            res = TRUE;
        }
        else
        {
            res = FALSE;
        }
    }
    return res;

}

#ifdef KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
/*****************************************************************************/
//  Description : make a judgement if tp point in camera icon rectangle
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTpPointInCameraIcon(uint16 x, uint16 y)
{
    BOOLEAN             res = FALSE;
    uint16              img_width = 0, img_height = 0;
    uint16              lcd_width = 0, lcd_height = 0;
    int16               icon_pos_x = 0, icon_pos_y = 0;
    GUI_POINT_T         pos = {0, KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y};
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT4_DC_PRESSED, MMIKL_CLOCK_DISP_WIN_ID);
    icon_pos_x = lcd_width - img_width - KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
    icon_pos_y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;

    if(!s_is_show_camera_icon)
    {
        return FALSE;
    }
    
    if(x >= icon_pos_x && x <= icon_pos_x + img_width && y >= icon_pos_y && y <= icon_pos_y + img_height)
    {
        res = TRUE;
    }
    else
    {
        res = FALSE;
    }
    
    return res;

}
#endif

LOCAL void MMIKL_DisplayDateDay(GUI_LCD_DEV_INFO *dev, GUI_RECT_T rect, GUI_FONT_T font, GUI_ALIGN_E align, GUI_COLOR_T color, GUIFONT_EFFECT_T effect )
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
        MMI_TEXT_ID_T week_table_iphone[] = {
                                        TXT_IDLE_SHORT_SUNDAY,
                                        TXT_IDLE_SHORT_MONDAY,
                                        TXT_IDLE_SHORT_TUESDAY,
                                        TXT_IDLE_SHORT_WEDNESDAY,
                                        TXT_IDLE_SHORT_THURSDAY,
                                        TXT_IDLE_SHORT_FRIDAY,
                                        TXT_IDLE_SHORT_SATURDAY
                                    };
   	 wchar wstr_month[3] = {0};
     wchar wstr_day[3] = {0};
	 MMI_STRING_T mouth_string = {0};
	 MMI_STRING_T day_string = {0};
     MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;


    MMIAPISET_GetCurrentDateStr(datestr,'-',16);
    MMI_STRNTOWSTR( datewstr, 16, datestr, 16, 16 );    

    TM_GetSysDate(&date);
    MMI_GetLabelTextByLang(week_table[date.wday], &string);
    MMIAPISET_GetLanguageType(&lang_type);

    if((MMISET_LANGUAGE_SIMP_CHINESE == lang_type)&&
        (MMISET_KEY_LOCK_EFFECT4 == MMIAPISET_GetKeyLockEffect()))
    {
        MMISET_DATE_DISPLAY_TYPE_E  date_display_type   = MMISET_DATE_YMD;
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

        SCI_MEMSET(datewstr, 0, sizeof(datewstr));

        MMI_GetLabelTextByLang(TXT_DATE_MONTH, &mouth_string);
        MMI_GetLabelTextByLang(TXT_DATE_DAY, &day_string);
        MMIAPICOM_Wstrncpy(datewstr,wstr_month,MMIAPICOM_Wstrlen(wstr_month));
        MMIAPICOM_Wstrncat(datewstr,mouth_string.wstr_ptr,mouth_string.wstr_len);
        MMIAPICOM_Wstrncat(datewstr,wstr_day,MMIAPICOM_Wstrlen(wstr_day));
        MMIAPICOM_Wstrncat(datewstr,day_string.wstr_ptr,day_string.wstr_len);

        MMI_GetLabelTextByLang(week_table_iphone[date.wday], &string);
	}

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


/*****************************************************************************/
//  Description : Display slide back ground of effect4.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplaySlideBgEffect4(void)
{
    GUI_LCD_DEV_INFO layer = {0, 0};
    GUI_RECT_T rect = {0};
    GUI_POINT_T pos = {0, KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y};
    uint16 img_width = 0, img_height = 0;
    uint16 lcd_width = 0, lcd_height = 0;
    /* slide  bg */
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
    if(s_is_show_camera_icon)
    {
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_SLIDE_BG_ADD_DC, &layer);

        //camera icon
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT4_DC_RELEASED, MMIKL_CLOCK_DISP_WIN_ID);
        pos.x = lcd_width - img_width - KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
        pos.y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_DC_RELEASED, &layer);
    }
    else
#endif
    {
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_SLIDE_BG, &layer);
    }
    
    /* time bg */
    pos.x = 0;
    pos.y = MMITHEME_GetStatusBarHeight();
    GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_TIME_BG, &layer);

    /* time */
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT4_DIGIT0, MMIKL_CLOCK_DISP_WIN_ID);
    rect.top = MMITHEME_GetStatusBarHeight() + KEY_LOCK_EFFECT4_TIME_UP_MARGIN;
    rect.bottom = rect.top + img_height;
    rect.right = lcd_width - 1;
    MMIKL_DisplayDigitalClock(&layer, rect, IMAGE_KL_EFFECT4_DIGIT0);

    /* day and date */
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT4_TIME_BG, MMIKL_CLOCK_DISP_WIN_ID);

    rect.top = rect.bottom + 2;
    rect.bottom = MMITHEME_GetStatusBarHeight() + img_height;
    rect.right = lcd_width - 1;
    MMIKL_DisplayDateDay(&layer, rect, MMI_DEFAULT_SMALL_FONT, ALIGN_HVMIDDLE, MMI_DEFAULT_TEXT_COLOR, FONT_EFFECT_NONE);
    
}

/*****************************************************************************/
//  Description : Update layer of effect4 type.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void UpdateLayerEffect4(BOOLEAN is_pen_down)
{
    int16 layer_pos_x = 0, layer_pos_y = 0;
    GUI_LCD_DEV_INFO layer = {0};
    GUI_POINT_T pos = {0};

    layer = GetMoveLayerEffect4();
    if(layer.block_id != 0)
    {
        UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);
        AppendMoveLayerEffect4();    
        
        UILAYER_SetLayerPosition(&layer, 0, 0);
        UILAYER_Clear(&layer);
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_SLIDE_BLOCK, &layer);
        UILAYER_SetLayerPosition(&layer, layer_pos_x, layer_pos_y);
    }
    else
    {
 
        pos.x = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_X;
        pos.y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;
        GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_SLIDE_BLOCK, &layer);
    }
}

/*****************************************************************************/
//  Description : Display text of effect4 type.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void DisplayTextEffect4(void)
{
    GUI_LCD_DEV_INFO layer = {0};
    GUISTR_STYLE_T style = {0};
    MMI_STRING_T string = {0};
    GUISTR_STATE_T state = GUISTR_STATE_ALIGN;
    GUI_RECT_T rect = {0};
    UILAYER_INFO_T layer_info = {0};
    int16 layer_pos_x = 0, layer_pos_y = 0;

    layer = GetTextLayerEffect4();
    UILAYER_GetLayerPosition(&layer, &layer_pos_x, &layer_pos_y);
    AppendTextLayerEffect4();    
    
    UILAYER_SetLayerPosition(&layer, 0, 0);
    UILAYER_Clear(&layer);

    style.font = MMI_DEFAULT_BIG_FONT;
    style.align = ALIGN_HVMIDDLE;
    style.font_color = MMI_WHITE_COLOR;
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
    if(s_is_show_camera_icon)
    {
        MMI_GetLabelTextByLang(TXT_KL_EFFECT4_SLIDE_RIGHT_ADD_DC, &string);
    }
    else
#endif
    {
        MMI_GetLabelTextByLang(TXT_KL_EFFECT4_SLIDE_RIGHT, &string);
    }
    
    UILAYER_GetLayerInfo(&layer, &layer_info);

    rect.left = 0;
    rect.top = 0;
    rect.right = layer_info.layer_width - 1;
    rect.bottom = layer_info.layer_height - 1;
    //GUISTR_DrawTextToLCDSingleLine(&layer, &pos, &string, &style, state);
    GUISTR_DrawTextToLCDInRect(&layer,
        &rect,
        &rect,
        &string,
        &style,
        state,
        GUISTR_TEXT_DIR_AUTO);

    UILAYER_SetLayerPosition(&layer, layer_pos_x, layer_pos_y);
    /* decrease the alpha to half */
    UILAYER_WeakLayerAlpha(&layer, KEY_LOCK_EFFECT4_TEXT_ALPHA);
}

/*****************************************************************************/
//  Description : Make the text to gradual effect.
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void GradualTextEffect4(uint16 distance)
{
    GUI_LCD_DEV_INFO     layer = {0};
    uint8 *layer_buf = PNULL;
    uint16 lcd_width = 0, lcd_height = 0;
    uint32 alpha = 0;
    UILAYER_INFO_T layer_info = {0};
    int32 count = 0;
    uint32 n = 0;
    uint32 *u32_p;
    
    layer = GetTextLayerEffect4();
    layer_buf = UILAYER_GetLayerBufferPtr(&layer);
    if(PNULL == layer_buf || layer.block_id == 0)
    {
        return;
    }
    u32_p = (uint32 *)layer_buf;

    UILAYER_GetLayerInfo(&layer, &layer_info);
    if (DATA_TYPE_ARGB888 != layer_info.data_type)
    {
        return;
    }
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if(distance < lcd_width/2 - s_key_lock_effect4_info.layer_pos_x)
    {
        alpha =(uint32)(KEY_LOCK_EFFECT4_TEXT_ALPHA * distance / (lcd_width/2 - s_key_lock_effect4_info.layer_pos_x));
        alpha = alpha*3/2;  /* make text change more quickly */
        if(alpha < KEY_LOCK_EFFECT4_TEXT_ALPHA)
        {
            alpha = KEY_LOCK_EFFECT4_TEXT_ALPHA - alpha;
        }
        else
        {
            alpha = 0;
        }
    }

    if(0 == alpha)
    {
        //return;
    }
    DisplayTextEffect4();
    
    count = layer_info.mem_width * layer_info.layer_height;
    while (count)
    {
        n = (*u32_p)>>24;
        if(n != 0)
        {
            /*  n*alpha/KEY_LOCK_EFFECT4_TEXT_ALPHA, that is n*alpha>>7. */
            //*u32_p = ((n*(alpha>>7)) << 24) |((*u32_p)&0xffffff);
            *u32_p = ((n*alpha>>7) << 24) |((*u32_p)&0xffffff);/*lint !e504*/
        }
        u32_p++;
        count--;
    }   
    UILAYER_StoreLayerUpdateRect(&layer);
}

/*****************************************************************************/
//  Description : Scroll text of effect4 type..
//  Global resource dependence : 
//  Author: Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void ScollTextEffect4(uint32 frame_index)
{
    GUI_LCD_DEV_INFO     layer = {0};
    uint8 *layer_buf = PNULL;
    UILAYER_INFO_T layer_info = {0};
    uint32 n = 0;
    uint32 *u32_p = PNULL;
    uint16 start_p = 0, end_p = 0;
    int32 i = 0, j = 0;
    
    layer = GetTextLayerEffect4();
    layer_buf = UILAYER_GetLayerBufferPtr(&layer);

    
    if(PNULL == layer_buf || layer.block_id == 0)
    {
        return;
    }
    u32_p = (uint32 *)layer_buf;

    UILAYER_GetLayerInfo(&layer, &layer_info);
    DisplayTextEffect4();

    end_p = (layer_info.layer_width + KEY_LOCK_EFFECT4_SCROLL_TEXT_WIDTH)* frame_index / KEY_LOCK_EFFECT4_SCROLL_TEXT_FRAME_NUM;
    if(end_p >= KEY_LOCK_EFFECT4_SCROLL_TEXT_WIDTH)
    {
        start_p = end_p - KEY_LOCK_EFFECT4_SCROLL_TEXT_WIDTH;
    }
    else
    {
        start_p = 0;
    }
    if(end_p >= layer_info.layer_width)
    {
        end_p = layer_info.layer_width - 1;
    }

    if(layer_info.mem_width != layer_info.layer_width)
    {
        return;
    }

    for(i = start_p; i < end_p; i++)
    {
        u32_p += i;
        for(j = 0; j < layer_info.layer_height; j++)
        {
            n = (*u32_p)>>24;
            if(n != 0)
            {
                /* n*2, to double the alpha */
                *u32_p = ((n*2) << 24) |((*u32_p)&0xffffff);
            }
            u32_p += layer_info.mem_width;
        }
        u32_p = (uint32 *)layer_buf;
    }
    
    UILAYER_StoreLayerUpdateRect(&layer);    
}

/******************************************************************************/
//  Description : handle kl display window msg of type4.
//  Global resource dependence : none
//  Author: 
//  Note: Slide right block unlock style
/*****************************************************************************/
PUBLIC BOOLEAN MMIKL_HandleIphoneStyleSlideWinMsg(
                                                    MMI_WIN_ID_T    win_id, 
                                                    uint16          msg_id, 
                                                    DPARAM          param
                                                    )
{
    MMI_RESULT_E            recode  =   MMI_RESULT_TRUE;
    static GUI_POINT_T tp_down_point = {0};
    static GUI_POINT_T tp_last_point = {0};
    static uint16 auto_move_frame_index = 0;
    static uint16 scroll_text_frame = 0;
    //SCI_TRACE_LOW:"HandleKLWinEffect4Msg: 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_ISTYLESLIDE_858_112_2_18_2_31_2_4,(uint8*)"d",msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:       
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetStbState( win_id, GUISTATUSBAR_STATE_USE_LAYER, TRUE);
		GUIWIN_SetStbBgIsDesplay(win_id,TRUE);
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR        
		GUIWIN_SetStbBgImageId(win_id, IMAGE_IDLE_STATUS_BAR);
#endif
#endif    
#ifdef KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
        s_is_show_camera_icon = FALSE;
#endif
        SetKeyLockControlParamEffect4();
        tp_down_point.x = 0;
        tp_last_point.x = 0;
        s_key_lock_effect4_info.tp_move_timer = 0;
        s_key_lock_effect4_info.text_scroll_timer = 0;
        s_key_lock_effect4_info.auto_move_timer = 0;
        break;
    case MSG_FULL_PAINT:
        DisplaySlideBgEffect4();
        if(0 == s_key_lock_effect4_info.text_scroll_timer)
        {
            if(MMIDEFAULT_IsBacklightOn() && 0 == s_key_lock_effect4_info.tp_move_timer)
            {
                DisplayTextEffect4();
                scroll_text_frame = 0;
                START_TIMER(s_key_lock_effect4_info.text_scroll_timer, KEY_LOCK_EFFECT4_TEXT_SCROLL_START_TIME);
            }
        }
        if(s_key_lock_effect4_info.tp_move_timer != 0)
        {
            UpdateLayerEffect4(TRUE);
        }
        else
        {
            UpdateLayerEffect4(FALSE);
        }
        break;
    case MSG_BACKLIGHT_TURN_ON:
        SetMoveLayerPosEffect4(s_key_lock_effect4_info.layer_pos_x);
        tp_down_point.x = 0;
        tp_last_point.x = 0;
        s_key_lock_effect4_info.tp_move_timer = 0;
        s_key_lock_effect4_info.text_scroll_timer = 0;
        
        break;
    case MSG_BACKLIGHT_TURN_OFF:
        STOP_TIMER(s_key_lock_effect4_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect4_info.text_scroll_timer);
        break;
    case MSG_KEYDOWN_RED:
        if (MMIKL_IsPreviewState())
        {
            recode = MMI_RESULT_FALSE;
        }
        #ifdef KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
        else
        {
            MMK_NeedKbdClkNotifyOnce();
        }
        #endif
            
        break;
#ifdef KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
    case MSG_KEY_DBLCLK:
        {
            uint8 key_code = *(uint8*)param;
            //SCI_TRACE_LOW:"MSG_KEY_DBLCLK, key = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKL_ISTYLESLIDE_924_112_2_18_2_31_2_5,(uint8*)"d", key_code);
            if(KEY_RED!= key_code)
            {
                break;
            }
            s_is_show_camera_icon= !s_is_show_camera_icon;
            STOP_TIMER(s_key_lock_effect4_info.text_scroll_timer);    
            CreateTextLayerEffect4();
         
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL );
        }
        break;
#endif
    case MSG_APP_OK:
  //  case MSG_APP_MENU:
        if (MMIKL_IsPreviewState())
        {
            MMIAPISET_SetKeyLockEffect(MMISET_KEY_LOCK_EFFECT4);
            MMK_CloseWin(win_id);
        }
		else
		{

			recode = MMI_RESULT_FALSE;		
		}
        break;
        
    case MSG_APP_CANCEL:
        if (MMIKL_IsPreviewState())
        {
            MMK_CloseWin(win_id);
        }
		else
		{
			recode = MMI_RESULT_FALSE;	
		}
        break;           
    case MSG_LOSE_FOCUS:
        STOP_TIMER(s_key_lock_effect4_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect4_info.text_scroll_timer);        
        SetMoveLayerPosEffect4(s_key_lock_effect4_info.layer_pos_x);
        break;

    case MSG_GET_FOCUS:
        if(MMIKL_GetWinStatus() == KL_UNLOCKED)
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_TIMER:
        if (s_key_lock_effect4_info.tp_move_timer == *(uint8*)param)
        {
            GUI_POINT_T     tp_point = {0};
            MMI_TP_STATUS_E state = {0};
            
            MMK_GetLogicTPMsg(&state, &tp_point);

            if(tp_point.x > tp_down_point.x)
            {
                if(IsDisEnoughToUnlockEffect4(tp_point.x - tp_down_point.x))
                {
                    auto_move_frame_index = 0;
                    MoveLayerAutoMoveEffect4(auto_move_frame_index++);
                    UILAYER_Clear(&s_key_lock_effect4_info.text_layer);
                    
                    START_TIMER(s_key_lock_effect4_info.auto_move_timer, KEY_LOCK_EFFECT4_AUTO_MOVE_TIME);
                    s_key_lock_effect4_info.tp_move_timer = 0;
                }
                else
                {
                    if(tp_point.x != tp_last_point.x)
                    {
                        GradualTextEffect4(tp_point.x - tp_down_point.x);
                        SetMoveLayerPosEffect4(s_key_lock_effect4_info.layer_pos_x + tp_point.x - tp_down_point.x);
                        tp_last_point.x = tp_point.x;
                    }
                    START_TIMER(s_key_lock_effect4_info.tp_move_timer, KEY_LOCK_EFFECT4_TP_MOVE_TIME);
                }
            }
            else
            {
                if(tp_last_point.x != s_key_lock_effect4_info.layer_pos_x)
                {
                    SetMoveLayerPosEffect4(s_key_lock_effect4_info.layer_pos_x);
                    tp_last_point.x = s_key_lock_effect4_info.layer_pos_x;
                }
                START_TIMER(s_key_lock_effect4_info.tp_move_timer, KEY_LOCK_EFFECT4_TP_MOVE_TIME);
            }
        }        
        else if(s_key_lock_effect4_info.auto_move_timer == *(uint8*)param)
        {
            if(auto_move_frame_index > KEY_LOCK_EFFECT4_AUTO_MOVE_FRAME_NUM)
            {
                MMK_CloseWin(win_id);
            }
            else
            {
                MoveLayerAutoMoveEffect4(auto_move_frame_index++);
                START_TIMER(s_key_lock_effect4_info.auto_move_timer, KEY_LOCK_EFFECT4_AUTO_MOVE_TIME);
            }
        }      
        else if(s_key_lock_effect4_info.text_scroll_timer == *(uint8*)param)
        {
            ScollTextEffect4(scroll_text_frame++);
            scroll_text_frame = scroll_text_frame%(KEY_LOCK_EFFECT4_SCROLL_TEXT_FRAME_NUM + 1);
            if(0 == scroll_text_frame)
            {
                START_TIMER(s_key_lock_effect4_info.text_scroll_timer, KEY_LOCK_EFFECT4_TEXT_SCROLL_START_TIME);
            }
            else
            {
                START_TIMER(s_key_lock_effect4_info.text_scroll_timer, KEY_LOCK_EFFECT4_TEXT_SCROLL_TIME);
            }
        }   
        else
        {
            recode = MMI_RESULT_FALSE;
        }
        
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        if (KL_LOCKED == MMIKL_GetWinStatus()||MMIKL_IsPreviewState())
        {
            GUI_POINT_T point = {0};
        
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
       
            if(IsTpPointInMoveLayerEffect4(point.x, point.y))
            {
                tp_down_point.x = point.x;
                tp_down_point.y = point.y;
                STOP_TIMER(s_key_lock_effect4_info.text_scroll_timer);
                DisplayTextEffect4();
                UpdateLayerEffect4(TRUE);
                START_TIMER(s_key_lock_effect4_info.tp_move_timer, KEY_LOCK_EFFECT4_TP_MOVE_TIME);
            }
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
            else if(IsTpPointInCameraIcon(point.x, point.y))
            {
                uint16 img_width = 0, img_height = 0;
                uint16 lcd_width = 0, lcd_height = 0;
                GUI_POINT_T pos = {0, KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y};
				GUI_LCD_DEV_INFO layer = {0, 0};

                GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
                GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_KL_EFFECT4_DC_PRESSED, MMIKL_CLOCK_DISP_WIN_ID);
                pos.x = lcd_width - img_width - KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN;
                pos.y = KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y;
                GUIRES_DisplayImg(&pos, PNULL, PNULL, MMIKL_CLOCK_DISP_WIN_ID, IMAGE_KL_EFFECT4_DC_PRESSED, &layer);

            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        if (KL_LOCKED == MMIKL_GetWinStatus()||MMIKL_IsPreviewState())
        {
            GUI_POINT_T point = {0};
        
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            if(s_key_lock_effect4_info.tp_move_timer != 0)
            {
                STOP_TIMER(s_key_lock_effect4_info.tp_move_timer);
                tp_down_point.x = 0;
                tp_down_point.y = 0;
                
                DisplayTextEffect4();        
                SetMoveLayerPosEffect4(s_key_lock_effect4_info.layer_pos_x);
                UpdateLayerEffect4(FALSE);

                if(0 == s_key_lock_effect4_info.text_scroll_timer)
                {
                    scroll_text_frame = 0;
                    START_TIMER(s_key_lock_effect4_info.text_scroll_timer, KEY_LOCK_EFFECT4_TEXT_SCROLL_START_TIME);
                }
            }
#ifdef  KEY_LOCK_EFFECT4_SUPPORT_CAMERA_SHORTCUT
            else if(IsTpPointInCameraIcon(point.x, point.y))
            {
                MMK_ReturnIdleWin();
                MMIAPIDC_OpenPhotoWin(); 
                MMIKL_SetWinStatus(KL_UNLOCKED);
                MMK_CloseWin(win_id);
            }
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_NOTIFY_STATUSBAR_PAINT:
        break;
    case MSG_NOTIFY_STATUSBAR_PAINT_END:
        break;

    case MSG_KEY_LOCK_CLOSE_WIN:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:  
        STOP_TIMER(s_key_lock_effect4_info.tp_move_timer);
        STOP_TIMER(s_key_lock_effect4_info.text_scroll_timer);    
        STOP_TIMER(s_key_lock_effect4_info.auto_move_timer);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;

    }
    
    return recode;
}

#endif	//MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT
#endif




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:57 2012*/ //IGNORE9527
