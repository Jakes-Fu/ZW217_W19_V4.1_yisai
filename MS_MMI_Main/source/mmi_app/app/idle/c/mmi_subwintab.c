/*****************************************************************************
** File Name:      mmi_subwintab.c                                           *
** Author:                                                                   *
** Date:           28/06/2005                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2006       Jassmine.Meng       Create
******************************************************************************/
#define _MMI_SUBWINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_idle_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiidle_subwintab.h"
#include "window_parse.h"
#include "guilcd.h"
#include "mmiidle_export.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmidisplay_data.h"/*lint -e766*/
#include "mmi_image.h"
#include "guifont.h"

#include "mmi_textfun.h"
#include "mmk_timer.h"
#include "mmimms_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifndef WIN32
#include "gpio_prod_api.h"
#endif
//#include "guires.h"
#include "mmi_theme.h"
#include "guistring.h"
#include "mmifm_export.h"
//#include "mmienvset.h"/*lint -e766*/
#include "mmienvset_export.h"
#ifdef CAMERA_SUPPORT
#include "mmidc_export.h"
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifndef SUBLCD_SIZE_NONE

#define SUB_BAT_MAX_LEVEL       5

//time
#define MMI_SUB_TIME_LEN        5   //不管12小时制还是24小时制，都只显示时间
#define MMI_SUB_TIME_AMPM_STR_LEN        2   //如果12小时制，显示am/pm

//date
#define MMI_SUB_DATE_LEN        10
//timer
#ifdef SUBLCD_PHONE_SIMU_SUPPORT
#define SUB_TIMER_TIMEOUT    100 //1200
#else
#define SUB_TIMER_TIMEOUT    500 //1200
#endif

#ifdef SUBLCD_DEV_SIZE_96X64
#define SUB_LCD_HEIGHE      64
#define SUB_LCD_WIDTH       96
#define SUB_POWEROFF_STR_Y  25
#define SUB_CALL_INFO_Y     30
#define SUB_TIME_X_24       18
#define SUB_TIME_X_12       6
#else
#define SUB_LCD_HEIGHE      96
#define SUB_LCD_WIDTH       96
#define SUB_POWEROFF_STR_Y  35
#define SUB_CALL_INFO_Y     50
#define SUB_TIME_X_24       18
#define SUB_TIME_X_12       6
#endif



/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL SUB_LCD_INFO_T    s_sub_lcd_info = {0}; //小屏显示信息

#ifndef SUBLCD_SIZE_NONE
LOCAL MMI_TEXT_ID_T            s_sub_text_id[SUB_CONTENT_MAX_NUM] = 
														{TXT_NULL,
														TXT_NULL,
														TXT_SUBLCD_INPUTPWD,
														TXT_SUBLCD_INPUTPIN,
														TXT_SUBLCD_INPUTPUK,
														TXT_SIM_REJECTED,
			                                                         #ifndef MMI_IDLE_MINI_SUPPORT
                                                                                                  TXT_SIM_REGISTRATION_FAILED,
                                                                              #else
                                                                                                  TXT_SIM_REGISTRATION_FAILED_MINI,
                                                                              #endif					
														TXT_PUK_BLOCKED,
														TXT_PUK2_BLOCKED,
														TXT_NO_SIM,
														TXT_CHARGE_WARNING,
														TXT_NULL,
														TXT_NULL,
														TXT_NULL,
														TXT_NULL,
														TXT_NULL,
														TXT_NULL,
														TXT_SUBLCD_CHARGING,
														TXT_PHONE_CHARGE_OK,
														TXT_CHARGE_OVER_VOLTAGE,
														TXT_DC_WAITING};//小屏字符串显示信息
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
//@cr36254 jian.ma start
extern uint8 MMIAPIENVSET_GetActiveModeId(void);
//@cr36254 jian.ma end

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : draw charge icon
//	Global resource dependence : none
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL void DrawChargeIcon(void);

#ifndef SUBLCD_SIZE_NONE
/*****************************************************************************/
// 	Description : display the sub lcd icon
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplaySubLcdIcon(void);
#endif

/*****************************************************************************/
// 	Description : display the time of sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/ 
LOCAL void DisplaySubLcdTime(void);

/*****************************************************************************/
// 	Description : display date of other string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplaySubLcdDateOrStr(void);

/*****************************************************************************/
// 	Description : display date on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplaySubLcdDate(void);

/*****************************************************************************/
// 	Description : display string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplayTextByPtr(                          
                            wchar       *str_ptr,
                            uint16      str_len,
                            int16       display_pos_y
                            );

/*****************************************************************************/
// 	Description : display cycle string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplayCycleText(
                           // BOOLEAN     is_ucs2,
                            wchar       *str_ptr,
                            uint16      str_len,
                            int16       display_pos_y
                            );
/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetSubWinTextStyle(
                              GUISTR_STYLE_T *style_ptr,
                              GUI_FONT_T     font,
                              GUI_COLOR_T    font_color
                              );

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplaySubwinText(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T     font,
                             GUI_COLOR_T    font_color,
                             wchar*   wstr_ptr,
                             uint16   wstr_len
                             );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : display cycle string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplayCycleText(
                           // BOOLEAN     is_ucs2,
                            wchar       *str_ptr,
                            uint16      str_len,
                            int16       display_pos_y
                            )
{
#ifndef SUBLCD_SIZE_NONE
	MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    GUI_RECT_T      rect = {0};
	GUI_LCD_DEV_INFO	lcd_dev = {0};

    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T   text = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT;

	lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
	lcd_dev.lcd_id = GUI_SUB_LCD_ID;


	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_SUB_LCD_DATE);

	
    rect.left = 0;
    rect.right = SUB_LCD_WIDTH - 1;
    rect.top = display_pos_y;
    rect.bottom = rect.top + GUI_GetFontHeight(idle_disp_style.text_font, UNICODE_HANZI);

    GetSubWinTextStyle(&text_style, idle_disp_style.text_font, MMITHEME_GetCurThemeFontColor(MMI_THEME_IDLE_TIME));
    
    text.wstr_len = str_len - s_sub_lcd_info.context_cur_index;
    text.wstr_ptr = str_ptr + s_sub_lcd_info.context_cur_index;
    
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev,
        (const GUI_RECT_T      *)&rect,       //the fixed display area
        (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
    
    
    s_sub_lcd_info.context_cur_index += 1;

    if (0 == str_len)//防错处理，防止没有文件名的Mp3
    {
        s_sub_lcd_info.context_cur_index = 0;
    }
    else
    {
        s_sub_lcd_info.context_cur_index %= str_len;
    }
#endif
}

/*****************************************************************************/
// 	Description : draw charge icon
//	Global resource dependence : none
//  Author:
//	Note:  
/*****************************************************************************/
LOCAL void DrawChargeIcon(void)
{
#ifndef SUBLCD_SIZE_NONE
    IDLE_WIN_INFO_T		*idle_info_ptr = MMIIDLE_GetIdleWinInfo();
	GUI_POINT_T			point= {0};
	GUI_LCD_DEV_INFO	lcd_dev = {0};
    MMI_WIN_ID_T        win_id = MAIN_IDLE_WIN_ID;
	uint16              width = 0;
	uint16              height = 0;
    MMI_IMAGE_ID_T		image_id = IMAGE_NULL;
	
	lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
	lcd_dev.lcd_id = GUI_SUB_LCD_ID;//SUBLCD_ID;
	point.y = 0;

	if(!MMK_IsOpenWin(MAIN_IDLE_WIN_ID)
#ifdef CAMERA_SUPPORT
        || MMIAPIDC_IsOpened()
#endif
        )
	{
		return;
	}
    if (idle_info_ptr->is_charge)
    {
		IMG_EnableTransparentColor(TRUE);
        MMISUB_StartSubLcdTimer();
        image_id = s_sub_lcd_info.cur_batlevel + SUBIDLE_ICON_BATTERIES_CHARGE1;
		if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
		{
			//charge icon, must at top right 
			point.x = MMI_SUBSCREEN_WIDTH - width- MMI_SUBSCREEN_ICON_SPACE;
		}
        GUIRES_DisplayImg(&point,
            PNULL,
            PNULL,
            VIRTUAL_WIN_ID,
            image_id,
            &lcd_dev);
        //set battery level
        s_sub_lcd_info.cur_batlevel++;
        if (s_sub_lcd_info.cur_batlevel > SUB_BAT_MAX_LEVEL)
        {
            s_sub_lcd_info.cur_batlevel = 0;
        }
	}
#endif
}

#ifndef SUBLCD_SIZE_NONE
/*****************************************************************************/
// 	Description : display the sub lcd icon
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:   没有考虑双卡的情况，需要与主屏统一  @$discuss
/*****************************************************************************/
LOCAL void DisplaySubLcdIcon(void)
{
    uint8				envset_mode = 0;
    MMI_IMAGE_ID_T		image_id = IMAGE_NULL;
    MMI_IMAGE_ID_T		sms_image_id = IMAGE_NULL;
    IDLE_WIN_INFO_T		*idle_info_ptr = MMIIDLE_GetIdleWinInfo();
    GUI_POINT_T			line_point[MMI_DUAL_SYS_MAX] = {{0,0},};
    GUI_LCD_DEV_INFO	lcd_dev = {0};
    MMI_WIN_ID_T        win_id = MAIN_IDLE_WIN_ID;
    uint16              width = 0;
    uint16              height = 0;
    BOOLEAN is_headset = GPIO_CheckHeadsetStatus();
    uint32 i = 0;

    if(!MMK_IsOpenWin(MAIN_IDLE_WIN_ID))
    {
        return;
    }
    lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
    lcd_dev.lcd_id = GUI_SUB_LCD_ID;//SUBLCD_ID;

    IMG_EnableTransparentColor(TRUE);

    win_id = VIRTUAL_WIN_ID;//@maggie add for sub
    
    SCI_MEMSET(line_point, 0x00, sizeof(line_point));
    for(i = 1; i<MMI_DUAL_SYS_MAX; i++)
    {
        line_point[i].x = 0;
        line_point[i].y = line_point[i-1].y + MMI_SUBSCREEN_LINE_HIEGHT;
    }
    
    //信号量
    // if (MMIAPIPHONE_IsSimAvailable())
    //{
#if defined(MMI_DUALMODE_ENABLE)
    switch(MMIAPIPHONE_GetTDOrGsm(MN_DUAL_SYS_1)) 
    {
    case MMI_GMMREG_RAT_GPRS:
         	image_id = idle_info_ptr->rxlevel[MN_DUAL_SYS_1] + SUBLCD_ICON_SIGNAL1_AND_GPRS;		
        	break;
    case MMI_GMMREG_RAT_3G:
        	image_id = idle_info_ptr->rxlevel[MN_DUAL_SYS_1] + SUBLCD_ICON_SIGNAL1_AND_T;
        	break;
    case MMI_GMMREG_RAT_UNKNOWN:
         	image_id = idle_info_ptr->rxlevel[MN_DUAL_SYS_1] + SUBIDLE_ICON_SIGNAL0;
        	break;
    default:
        	break;
    }

    //point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_SIGNAL_ICON);
    GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
    	PNULL,
    	PNULL,
    	win_id,
    	image_id,
    	&lcd_dev);		
    if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
    {
    	line_point[0].x += width + MMI_SUBSCREEN_ICON_SPACE;
    	//point.y += height;
    }		
    //jesse yu add for sublcd start
#elif  !defined(MMI_MULTI_SIM_SYS_SINGLE)
    for(i = 0; i<MMI_DUAL_SYS_MAX; i++)
    {
        if (!MMIAPIPHONE_GetSimExistedStatus((MN_DUAL_SYS_E)i))
        {
            image_id = SUBIDLE_ICON_NO_SIM;
        }
        else if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus((MN_DUAL_SYS_E)i)
            || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus((MN_DUAL_SYS_E)i)
            || !MMIAPIIDLE_NeedShowIconAccordingToDualSysSetting((MN_DUAL_SYS_E)i))
        {
            image_id = SUBIDLE_ICON_SIGNAL0;
        }
        else
        {
            image_id = idle_info_ptr->rxlevel[i] + SUBIDLE_ICON_SIGNAL0;
        }

        GUIRES_DisplayImg(&(line_point[i]),
                PNULL,
                PNULL,
                win_id,
                image_id,
                &lcd_dev);

        if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
        {
            line_point[i].x += width + MMI_SUBSCREEN_ICON_SPACE;
            //point.y += height;
        }
     }
#else
    if (!MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_1))
    {
        image_id = SUBIDLE_ICON_NO_SIM;
    }
    else if (SIM_STATUS_PIN_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1)
        || SIM_STATUS_PUK_BLOCKED == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
    {
        image_id = SUBIDLE_ICON_SIGNAL0;
    }
    else
    {
        image_id = idle_info_ptr->rxlevel[MN_DUAL_SYS_1] + SUBIDLE_ICON_SIGNAL0;
    }
    //point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_SIGNAL_ICON);
    GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
            PNULL,
            PNULL,
            win_id,
            image_id,
            &lcd_dev);
	if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
	{
		line_point[MN_DUAL_SYS_1].x += width + MMI_SUBSCREEN_ICON_SPACE;
		//point.y += height;
	}
#endif
     
    //earphone
    if(is_headset)  //test
    {
        image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_EARPHONE;
    }
    else
    {
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
    }
	//point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_EARPHONE_ICON);
	if(IMAGE_NULL != image_id)
	{
		GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
			PNULL,
			PNULL,
			win_id,
			image_id,
			&lcd_dev);
		if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
		{
			line_point[MN_DUAL_SYS_1].x += width + MMI_SUBSCREEN_ICON_SPACE;
		}
	}
    //message
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIAPIPHONE_IsSimAvailable((MN_DUAL_SYS_E)i) && MMIAPISMS_IsReady())
        {
            if (MMIAPISMS_IsSMSFull()) //短信满
            {
            	sms_image_id = SUBIDLE_ICON_SMS_FULL;
            }
            else if (0 < MMIAPISMS_GetTobeReadMsgCount((MN_DUAL_SYS_E)i, MMISMS_TYPE_MMS)) //未读彩信
            {
            	sms_image_id = SUBIDLE_ICON_MMS;
            }
            else if (0 < MMIAPISMS_GetTobeReadMsgCount((MN_DUAL_SYS_E)i, MMISMS_TYPE_SMS)) //未读短信
            {
            	sms_image_id = SUBIDLE_ICON_MESSAGE;
            }
            else if (0 < MMIAPISMS_GetTobeReadMsgCount((MN_DUAL_SYS_E)i, MMISMS_TYPE_WAP_PUSH)) //未读push消息
            {
            	sms_image_id = SUBIDLE_ICON_MESSAGE;
            }
            else
            {
            	sms_image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
            }
            //point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_MESSAGE_ICON);
            if(IMAGE_NULL != sms_image_id)
            {
            	GUIRES_DisplayImg(&(line_point[i]),PNULL,PNULL,win_id,sms_image_id,&lcd_dev);
            	if(GUIRES_GetImgWidthHeight(&width, &height,sms_image_id, win_id))
            	{
            		line_point[i].x += width + MMI_SUBSCREEN_ICON_SPACE;
            	}	
            }
        }
     }
    
//jesse yu modified for sublcd end

    //alarm
    if(idle_info_ptr->is_alarm)
    {
        image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_OPEN_CLOCK;
    }
    else
    {
		image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
    }
	//point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_ALARM_ICON);
    GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
        PNULL,
        PNULL,
        win_id,
        image_id,
        &lcd_dev);
	if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
	{
		line_point[MN_DUAL_SYS_1].x += (width + MMI_SUBSCREEN_ICON_SPACE);
	}
    //envset(ring,vibration,silence)
    envset_mode = MMIAPIENVSET_GetActiveModeId();
    switch(envset_mode)
    {
    case MMIENVSET_SILENT_MODE:     // 安静环境（静音）
        image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_CALL_SILENT;
        break;

    case MMIENVSET_MEETING_MODE:    // 会议环境（振动）
        image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_CALL_VIBRA;
        break;

    //juan.zhao cr 172334,室内和室外图标区分开



    case MMIENVSET_INSIDE_MODE:     // 车内环境（振动+铃声）
            image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_CALL_LOW_RING;
            break;
    
    case MMIENVSET_NOISY_MODE:      // 户外环境（振动+铃声）
        image_id = (MMI_IMAGE_ID_T)SUBIDLE_ICON_CALL_RING_VIBRA;
        break;
        
    default:
        image_id = (MMI_IMAGE_ID_T)IMAGE_NULL;
        break;
    };
	//point = MMITHEME_GetIdleIconPos(MMI_SUB_IDLE_ENVSET_ICON);

    GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
        PNULL,
        PNULL,
        win_id,
        image_id,
        &lcd_dev);

    //charge

    if (idle_info_ptr->is_charge)
    {
        MMISUB_StartSubLcdTimer();
        image_id = s_sub_lcd_info.cur_batlevel + SUBIDLE_ICON_BATTERIES_CHARGE1;
		if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
		{
			//charge icon, must at top right 
			line_point[MN_DUAL_SYS_1].x = MMI_SUBSCREEN_WIDTH - width - MMI_SUBSCREEN_ICON_SPACE;
		}
        GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
            PNULL,
            PNULL,
            win_id,
            image_id,
            &lcd_dev);

        //set battery level
        s_sub_lcd_info.cur_batlevel++;
        if (s_sub_lcd_info.cur_batlevel > SUB_BAT_MAX_LEVEL)
        {
            s_sub_lcd_info.cur_batlevel = 0;
        }
    }
    else
    {
        s_sub_lcd_info.cur_batlevel = MMIAPIPHONE_GetBatCapacity();
        image_id = s_sub_lcd_info.cur_batlevel + SUBIDLE_ICON_BATTERIES0;
		if(GUIRES_GetImgWidthHeight(&width, &height,image_id, win_id))
		{
			//charge icon, must at top right 
			line_point[MN_DUAL_SYS_1].x = MMI_SUBSCREEN_WIDTH - width - MMI_SUBSCREEN_ICON_SPACE;
		}
        GUIRES_DisplayImg(&(line_point[MN_DUAL_SYS_1]),
            PNULL,
            PNULL,
            win_id,
            image_id,
            &lcd_dev);
    }

    IMG_EnableTransparentColor(FALSE);
}
#endif

/*****************************************************************************/
//  Description : 获得idle上的文本风格
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void GetSubWinTextStyle(
                              GUISTR_STYLE_T *style_ptr,
                              GUI_FONT_T     font,
                              GUI_COLOR_T    font_color
                              )
{
    if(PNULL == style_ptr)
    {
        //SCI_TRACE_LOW:"GetSubWinTextStyle ERROR! PNULL == style_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_607_112_2_18_2_23_38_12,(uint8*)"");
        return;
    }

    style_ptr->effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr->angle = ANGLE_0;
    style_ptr->align = ALIGN_LEFT;
    style_ptr->font = font;
    style_ptr->font_color = font_color;
}

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplaySubwinText(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T     font,
                             GUI_COLOR_T    font_color,
                             wchar*   wstr_ptr,
                             uint16   wstr_len
                             )
{
    GUISTR_STYLE_T text_style = {0};
    MMI_STRING_T   text = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN|GUISTR_STATE_EFFECT|GUISTR_STATE_EFFECT;

    GetSubWinTextStyle(&text_style, font, font_color);
    
    text.wstr_len = wstr_len;
    text.wstr_ptr = wstr_ptr;
    //display date
    GUISTR_DrawTextToLCDSingleLine( 
        (const GUI_LCD_DEV_INFO *)lcd_dev_ptr,
        (const GUI_POINT_T      *)point_ptr,
        (const MMI_STRING_T     *)&text,
        &text_style,
        text_state
        );    
}

/*****************************************************************************/
// 	Description : display the time of sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/ 
LOCAL void DisplaySubLcdTime(void)
{
#ifndef SUBLCD_SIZE_NONE
	MMI_IDLE_DISPLAY_T			idle_disp_style = {0};
    uint8                       time_24_str[MMI_SUB_TIME_LEN + 1] = {0};
	wchar                       time_24_wstr[MMI_SUB_TIME_LEN + 1] = {0};
        uint8                       time_12_str[MMI_SUB_TIME_LEN + MMI_SUB_TIME_AMPM_STR_LEN+1] = {0};
	wchar                       time_12_wstr[MMI_SUB_TIME_LEN + MMI_SUB_TIME_AMPM_STR_LEN +1] = {0};
    GUI_FONT_T                  time_font = 0;
    GUI_FONT_T                  time_ampm_font = 0;//juan.zhao for sublcd,20100301
    GUI_COLOR_T					color = 0;
    GUI_POINT_T                 point = {0};
    MMISET_TIME_DISPLAY_TYPE_E  time_type = MMISET_TIME_24HOURS;
    uint32						width = 0;
    uint32					heigh = 0;//juan.zhao for sublcd,20100301
        uint32				ampm_heigh = 0;//juan.zhao for sublcd,20100301
    uint32					ampm_width = 0;        //juan.zhao for sublcd,20100301
	GUI_LCD_DEV_INFO	lcd_dev = {0};

	lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
	lcd_dev.lcd_id = GUI_SUB_LCD_ID;
	
	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_SUB_LCD_TIME);
        time_font = idle_disp_style.text_font;

	color = MMITHEME_GetCurThemeFontColor(MMI_THEME_SUB_FONT);

    time_type = MMIAPISET_GetTimeDisplayType();
 
    switch (time_type)
    {
    case MMISET_TIME_12HOURS:
        point.x = SUB_TIME_X_12;
        point.y = idle_disp_style.text_frame_top;
        time_font-=2;
        MMIAPISET_GetTimeStr12(time_12_str);
		MMIAPICOM_StrToWstr((const char *)time_12_str, time_12_wstr);
        DisplaySubwinText(
						&lcd_dev,
						&point,
						time_font,
                        color,
						time_12_wstr,
						MMIAPICOM_Wstrlen(time_12_wstr)
						);

        break;

    case MMISET_TIME_24HOURS:
        point.x = SUB_TIME_X_24;
        point.y = idle_disp_style.text_frame_top;
        MMIAPISET_GetTimeStr24(time_24_str);
		MMIAPICOM_StrToWstr((const char *)time_24_str, time_24_wstr);
        DisplaySubwinText(
						&lcd_dev,
						&point,
						time_font,
						color,
						time_24_wstr,
						MMIAPICOM_Wstrlen(time_24_wstr)
						);
        break;
    default:
        break;
    }
#endif
}

/*****************************************************************************/
// 	Description : display date of other string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplaySubLcdDateOrStr(void)
{
#ifndef SUBLCD_SIZE_NONE
	MMI_IDLE_DISPLAY_T	idle_disp_style = {0};
    uint8				index = 0;
    int16				display_pos_y = 0;
    MMI_STRING_T		text_str = {0};

    if(0 == s_sub_lcd_info.content_num)
    {
        //SCI_TRACE_LOW:"DisplaySubLcdDateOrStr 0 == s_sub_lcd_info.content_num"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_783_112_2_18_2_23_38_15,(uint8*)"");
        return;
    }
	
    index = s_sub_lcd_info.sub_content[s_sub_lcd_info.content_num-1];
	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_SUB_LCD_DATE);

    if (s_sub_lcd_info.is_poweron)//开机状态
    {
        display_pos_y = idle_disp_style.text_frame_top;
        if ((SUB_CONTENT_CALL == index) ||
            (SUB_CONTENT_CAMERA_WAIT == index))
        {
            display_pos_y = SUB_CALL_INFO_Y;
        }
    }
    else//关机状态，不显示icon,time
    {
        display_pos_y = SUB_POWEROFF_STR_Y;
    }

    switch (s_sub_lcd_info.sub_content[s_sub_lcd_info.content_num-1])
    {
    case SUB_CONTENT_NONE:
        break;

    case SUB_CONTENT_DATE:
        DisplaySubLcdDate();
        break;

    case SUB_CONTENT_MP3:
        if(MMIAPIMP3_IsPlayerActive())
        {
            SCI_MEMSET(&text_str,0,sizeof(MMI_STRING_T));
            text_str.wstr_ptr = s_sub_lcd_info.mp3_name.name;
            SCI_MEMSET(text_str.wstr_ptr,0,(MMIMP3_NAME_MAX_LEN + 1)*sizeof(wchar));
            
            //get current mp3 name
            MMIAPIMP3_GetCurMp3Name(&text_str, MMIMP3_NAME_MAX_LEN);

            //set global variable
            //s_sub_lcd_info.mp3_name.is_ucs2 = text_str.is_ucs2;
            s_sub_lcd_info.mp3_name.length = text_str.wstr_len;

            DisplayTextByPtr(text_str.wstr_ptr ,text_str.wstr_len,display_pos_y);
        }
        else//when mp3 is paused, display data
        {
            DisplaySubLcdDate();
        }
        break;
        
    case SUB_CONTENT_ALARM:
        DisplayTextByPtr(s_sub_lcd_info.alarm_info.info,
                         s_sub_lcd_info.alarm_info.length,display_pos_y);
        break;
    case SUB_CONTENT_COUNTEDTIME:
        DisplayTextByPtr(s_sub_lcd_info.countedtime_info.info,
                         s_sub_lcd_info.countedtime_info.length,display_pos_y);
        break;

    case SUB_CONTENT_FM:
        if(FM_STOP != MMIAPIFM_GetStatus())
        {
            SCI_MEMSET(&text_str,0,sizeof(MMI_STRING_T));
            text_str.wstr_ptr = s_sub_lcd_info.fm_name.name;
            SCI_MEMSET(text_str.wstr_ptr,0,(MMIMP3_NAME_MAX_LEN + 1)*sizeof(wchar));

            //get fm name
            MMIAPIFM_GetCurFreqName(&text_str, MMIMP3_NAME_MAX_LEN);

            //set global variable
            //s_sub_lcd_info.mp3_name.is_ucs2 = text_str.is_ucs2;
            s_sub_lcd_info.fm_name.length = text_str.wstr_len;

            DisplayTextByPtr(text_str.wstr_ptr ,text_str.wstr_len,display_pos_y);
        }
        else//when fm is paused, display data
        {
            DisplaySubLcdDate();
        }
        break;
    case SUB_CONTENT_CALL:
        DisplayTextByPtr(s_sub_lcd_info.call_info.info,
                         s_sub_lcd_info.call_info.length,display_pos_y);
        break;

    case SUB_CONTENT_MISSED_CALL:
        DisplayTextByPtr(s_sub_lcd_info.missed_call.info,
                         s_sub_lcd_info.missed_call.length,display_pos_y);
        break;

    default:
        MMI_GetLabelTextByLang(s_sub_text_id[index],&text_str);
        DisplayTextByPtr(text_str.wstr_ptr,text_str.wstr_len,display_pos_y);
        break;
    }
#endif
}


/*****************************************************************************/
// 	Description : display date on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplaySubLcdDate(void)
{
#ifndef SUBLCD_SIZE_NONE  //有小屏
	MMI_IDLE_DISPLAY_T	idle_disp_style = {0};
    uint8       date_str[MMI_SUB_DATE_LEN + 1] = {0};
	wchar       date_wstr[MMI_SUB_DATE_LEN + 1] = {0};
    int16       display_pos_y = 0;

	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_SUB_LCD_DATE);

    if (s_sub_lcd_info.is_poweron)//开机状态
    {
        display_pos_y = idle_disp_style.text_frame_top;
    }
    else//关机状态，不显示icon,time
    {
        display_pos_y = SUB_POWEROFF_STR_Y;
    }

    //get system date
	MMIAPISET_GetCurrentDateStr(date_str, '.',MMI_SUB_DATE_LEN + 1);
	MMIAPICOM_StrToWstr((const char *)date_str, date_wstr);
	//SCI_TRACE_LOW:"DisplaySubLcdDate: pos=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_910_112_2_18_2_23_39_16,(uint8*)"d", display_pos_y);


    //display date
    DisplayTextByPtr(date_wstr,MMI_SUB_DATE_LEN,display_pos_y);

#endif
}

/*****************************************************************************/
// 	Description : display string on the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
LOCAL void DisplayTextByPtr(
                           // BOOLEAN     is_ucs2,
                            wchar       *str_ptr,
                            uint16      str_len,
                            int16       display_pos_y
                            )
{
#ifndef SUBLCD_SIZE_NONE

	MMI_IDLE_DISPLAY_T idle_disp_style = {0};
    uint16          str_width = 0;
    GUI_FONT_T      str_font = 0;
    GUI_POINT_T     point = {0};
	GUI_LCD_DEV_INFO	lcd_dev = {0};

	lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
	lcd_dev.lcd_id = GUI_SUB_LCD_ID;//SUBLCD_ID;

	idle_disp_style = MMITHEME_GetIdleDispStyle(MMI_SUB_LCD_DATE);
	str_font = idle_disp_style.text_font;

    //calculate the length of string
    str_width = GUI_CalculateStringPiexlNum(str_ptr, str_len,  str_font,0);

    if (SUB_LCD_WIDTH < str_width)
    {
        MMISUB_StartSubLcdTimer();
        DisplayCycleText(str_ptr,str_len,display_pos_y);
    }
    else
    {
        point.x = (SUB_LCD_WIDTH - str_width)>>1;
        point.y = display_pos_y;

        DisplaySubwinText(
            (const GUI_LCD_DEV_INFO *)&lcd_dev,
            (const GUI_POINT_T      *)&point,
            str_font,
            MMITHEME_GetCurThemeFontColor(MMI_THEME_SUB_FONT),
            str_ptr, 
            str_len
            );
    }
#endif
}

#endif
/*****************************************************************************/
// 	Description : init lcd display info
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_Init(void)
{
    //set global varible
    SCI_MEMSET(&s_sub_lcd_info,0,sizeof(SUB_LCD_INFO_T));

    //set not power off status
    s_sub_lcd_info.is_poweron = TRUE;
    
    //set the first string,第一个是日期，当No sim or Sim error,更换第一个字符串
    s_sub_lcd_info.sub_content[s_sub_lcd_info.content_num] = SUB_CONTENT_DATE;
    s_sub_lcd_info.content_num = 1;
}

/*****************************************************************************/
// 	Description : display the sub lcd idle picture
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:更换小屏墙纸，只需要更改此函数即可
/*****************************************************************************/
void MMISUB_DisplayIdlePic(void)
{
#ifndef SUBLCD_SIZE_NONE  //有小屏
	GUI_LCD_DEV_INFO	lcd_dev = {0};
    MMI_WIN_ID_T        win_id;
    GUI_POINT_T         dis_point = {0};
    GUI_RECT_T rect={0,0,25,25};

	lcd_dev.block_id = MMI_DEFAULT_SUBLCD_BLOCK;
	lcd_dev.lcd_id = GUI_SUB_LCD_ID;

    win_id = VIRTUAL_WIN_ID;//@maggie add for sub
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_SUBLCD_ICON_BACKGROUND,
        &lcd_dev);
        //LCD_FillRect(&lcd_dev, rect, 56);
#endif
}

/*****************************************************************************/
// 	Description : 设置手机的状态，是关机还是开机状态
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:只需要关机状态charge,alarm调用即可
/*****************************************************************************/
void MMISUB_SetMsStatus(
                        BOOLEAN     is_poweron
                        )
{
    s_sub_lcd_info.is_poweron = is_poweron;
}

/*****************************************************************************/
// 	Description : 小屏是否允许刷新
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_IsPermitUpdate(
                           BOOLEAN  is_update   //是否允许刷新
                           )
{
#ifndef SUBLCD_SIZE_NONE  //有小屏
    if (s_sub_lcd_info.is_update != is_update)
    {
        s_sub_lcd_info.is_update = is_update;
        
        //允许刷新则立即显示
        if (is_update)
        {
            MMISUB_UpdateDisplaySubLcd();
        }
    }
#endif
}

/*****************************************************************************/
// 	Description : update display the sub lcd icon,time,date or string
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_UpdateDisplaySubLcd(void)
{
#ifndef SUBLCD_SIZE_NONE
    uint8   index = 0;
   SCI_TRACE_LOW("MMISUB_UpdateDisplaySubLcd: start");
    if (s_sub_lcd_info.content_num<1||s_sub_lcd_info.content_num>SUB_CONTENT_MAX_NUM)
    {
        //SCI_TRACE_LOW:"MMISUB_UpdateDisplaySubLcd error! num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_1054_112_2_18_2_23_39_17,(uint8*)"d",s_sub_lcd_info.content_num);
        return;
    }
//============== MET MEX  start ===================
#ifdef MET_MEX_SUPPORT
    //在MEX平台运行应用的时候禁止刷新子屏
    if(MMIMEX_IsSysRunning())
    {
        SCI_TRACE_LOW("MMISUB_UpdateDisplaySubLcd: MMIMEX_IsSysRunning");
        return;
    }            
#endif // MET_MEX_SUPPORT
//============== MET MEX  end ===================
    index = s_sub_lcd_info.sub_content[s_sub_lcd_info.content_num-1];
    if (s_sub_lcd_info.is_update)
    {
        //display bg
        MMISUB_DisplayIdlePic();
        //display date or string
        DisplaySubLcdDateOrStr();

        
        if (s_sub_lcd_info.is_poweron)
        {
            //display icon
            DisplaySubLcdIcon();

            if ((SUB_CONTENT_CALL != index) &&
                (SUB_CONTENT_CAMERA_WAIT != index))
            {
                //display time
                DisplaySubLcdTime();
            }
        }

    }

    //test
#ifdef SUBLCD_PHONE_SIMU_SUPPORT
    MMISUB_StartSubLcdTimer();
#endif
    SCI_TRACE_LOW("MMISUB_UpdateDisplaySubLcd: end");
#if defined WIN32 && defined PRINTSCREEN_SUPPORT
    {

        BOOLEAN is_need_print = FALSE;

        if ( is_need_print )
        {
            MMIAPIPTSCR_Print(FALSE);
        }
    }
#endif
#endif
}

/*****************************************************************************/
// 	Description : set the battery sub lcd level for display
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/ 
PUBLIC void MMISUB_SetBatteryLevel(void)
{
	s_sub_lcd_info.cur_batlevel = 0;
}



/*****************************************************************************/
// 	Description : start the timer of the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_StartSubLcdTimer(void)
{
    if (0 == s_sub_lcd_info.timer_id)
    {
        s_sub_lcd_info.timer_id = MMK_CreateTimerCallback(SUB_TIMER_TIMEOUT, MMISUB_HandleTimerMsg, NULL, FALSE);
    }
}

/*****************************************************************************/
// 	Description : stop the timer of the sub lcd
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
void MMISUB_StopSubLcdTimer(void)
{
#ifndef SUBLCD_SIZE_NONE
    if(0 != s_sub_lcd_info.timer_id)
    {
        MMK_StopTimer(s_sub_lcd_info.timer_id);
        s_sub_lcd_info.timer_id = 0;
    }
#endif
}


/*****************************************************************************/
// 	Description : handle the timer msg of subidle window
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
void MMISUB_HandleTimerMsg(
                              uint8 timer_id,
                              uint32   param
                              )
{
#ifndef SUBLCD_SIZE_NONE

    if (s_sub_lcd_info.timer_id == timer_id)
    {
        MMISUB_StopSubLcdTimer();
        MMISUB_StartSubLcdTimer();
        MMISUB_UpdateDisplaySubLcd();
		DrawChargeIcon();
    }
#endif
}

/*****************************************************************************/
// 	Description : set the sub lcd display
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:供其他模块调用
/*****************************************************************************/
void MMISUB_SetSubLcdDisplay(
                             BOOLEAN            is_replace, //是否替换第一项(第一项只显示日期，SIM相关信息)
                             BOOLEAN            is_display, //显示还是清除
                             SUB_CONTENT_E      content,    //显示的内容
                             MMI_STRING_T       *str_ptr    //显示信息，可以为空，由来电，闹钟等填写
                             )
{
#ifndef SUBLCD_SIZE_NONE  //有小屏
    uint8   i = 0,j = 0;

    if (is_replace)
    {//更换第一项，只显示日期，关机充电。SIM卡相关信息，三者者之间的相互切换
        s_sub_lcd_info.sub_content[0] = content;
    }
    else
    {
        //判断是否有重复
        for (i=0;i<s_sub_lcd_info.content_num;i++)
        {
            if (content == s_sub_lcd_info.sub_content[i])
            {
                break;
            }
        }

        //是否已经有记录
        if (i < s_sub_lcd_info.content_num)
        {
            //删除某个显示字符串，后面的字符串前移
            for (j=i;j<(s_sub_lcd_info.content_num-1);j++)
            {
                s_sub_lcd_info.sub_content[j] = s_sub_lcd_info.sub_content[j+1]; 
            }
            s_sub_lcd_info.sub_content[j] = SUB_CONTENT_NONE;
            if (1 < s_sub_lcd_info.content_num)//至少应该函数日期或者No sim/Sim error
            {
                s_sub_lcd_info.content_num--;
            }
        }
    
        if (is_display)
        {
            //set global variable about content text
            s_sub_lcd_info.sub_content[s_sub_lcd_info.content_num] = content;
            s_sub_lcd_info.content_num++;

            switch (content)
            {
            case SUB_CONTENT_ALARM:
                if(PNULL == str_ptr)
                {
                    //SCI_TRACE_LOW:"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_ALARM, PNULL == str_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_1207_112_2_18_2_23_39_18,(uint8*)"");
                    break;
                }
                //s_sub_lcd_info.alarm_info.is_ucs2 = str_ptr->is_ucs2;
                s_sub_lcd_info.alarm_info.length = str_ptr->wstr_len;
				MMI_WSTRNCPY(s_sub_lcd_info.alarm_info.info,MMIACC_EVENT_TEXT_MAX_LEN,
					str_ptr->wstr_ptr,str_ptr->wstr_len,str_ptr->wstr_len);
				break;
         case SUB_CONTENT_COUNTEDTIME:
                  s_sub_lcd_info.countedtime_info.length = str_ptr->wstr_len;
                  MMI_WSTRNCPY(s_sub_lcd_info.countedtime_info.info,MMIACC_EVENT_TEXT_MAX_LEN,
                             str_ptr->wstr_ptr,str_ptr->wstr_len,str_ptr->wstr_len);

                   break;               

            case SUB_CONTENT_CALL://call事件,保存姓名或者电话号码
                if(PNULL == str_ptr)
                {
                    //SCI_TRACE_LOW:"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_CALL, PNULL == str_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_1225_112_2_18_2_23_39_19,(uint8*)"");
                    break;
                }
                //s_sub_lcd_info.call_info.is_ucs2 = str_ptr->is_ucs2;
                s_sub_lcd_info.call_info.length = str_ptr->wstr_len;
                MMI_WSTRNCPY(s_sub_lcd_info.call_info.info,CALL_INFO_MAX_LEN,
                           str_ptr->wstr_ptr,str_ptr->wstr_len,str_ptr->wstr_len);
                break;

            case SUB_CONTENT_MISSED_CALL://未接电话事件
                if(PNULL == str_ptr)
                {
                    //SCI_TRACE_LOW:"MMISUB_SetSubLcdDisplay ERROR! case SUB_CONTENT_MISSED_CALL, PNULL == str_ptr"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_SUBWINTAB_1237_112_2_18_2_23_39_20,(uint8*)"");
                    break;
                }
                
              //  s_sub_lcd_info.missed_call.is_ucs2 = str_ptr->is_ucs2;
                s_sub_lcd_info.missed_call.length = str_ptr->wstr_len;
                MMI_WSTRNCPY(s_sub_lcd_info.missed_call.info,MISSED_CALL_INFO_MAX_LEN,
                           str_ptr->wstr_ptr,str_ptr->wstr_len,str_ptr->wstr_len);
                break;
            
            default:
                break;
            }
        }
    }

    //display sub lcd
    s_sub_lcd_info.context_cur_index = 0;
    MMISUB_UpdateDisplaySubLcd();
#endif
}
#endif
