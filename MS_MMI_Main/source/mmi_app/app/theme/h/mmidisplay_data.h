/*****************************************************************************
** File Name:      mmidisplay_data.h                                         *
** Author:                                                                   *
** Date:           23/06/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004        Edison.rao          Create                                 *
******************************************************************************/

#ifndef _MMIDISPLAY_DATA_H_
#define _MMIDISPLAY_DATA_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guicommon.h"
#include "mmi_position.h"
#include "mmi_custom_define.h"

#include "mmidisplay_color.h"
#include "mmidisplay_font.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef enum
{
	MMI_MAIN_BACKGROUND_PIC,		//MAIN LCD背景图片
	MMI_MAIN_SOFTKEY_BAR,           //MAIN LCD SOFTKEY BAR

	MMI_MAIN_IDLE_SIGNAL_ICON,      //MAIN LCD信号量提示 
#ifdef MMI_MULTI_SIM_SYS_DUAL
	MMI_MAIN_IDLE_SIGNAL_ICON_2,    //MAIN LCD信号量2提示 
#endif
	MMI_MAIN_IDLE_GPRS_ICON,	//MAIN LCD gprs状态标志显示，@zhaohui add for gprs icon
    MMI_MAIN_IDLE_MESSAGE_ICON,     //MAIN LCD短信提示
	MMI_MAIN_IDLE_EARPHONE_ICON,    //MAIN LCD 耳机提示
    MMI_MAIN_IDLE_ALARM_ICON,		//MAIN LCD闹钟提示
	MMI_MAIN_IDLE_ENVSET_ICON,		//MAIN LCD情景模式：静音，铃声，振动提示
	MMI_MAIN_IDLE_CALL_FORWARD,		//MAIN LCD呼叫转移
	MMI_MAIN_IDLE_BLUE_TOOTH,		//MAIN LCD蓝牙
	MMI_MAIN_IDLE_BATTERY_ICON,     //MAIN LCD电池提示
	MMI_MAIN_IS_CHARGING_ANIM,      //MAIN LCD关机下 充电动画
	MMI_MAIN_IS_ALARM_ANIM,			//MAIN LCD闹钟闹铃动画
    MMI_MAIN_IDLE_LINE_INFO_ICON,	//MAIN LCD通话线路状态
    MMI_MAIN_IDLE_SD_ICON,          //MAIN LCD SD卡提示
    TOTAL_IDLE_ICONANIM_NUMBER
} MMI_IDLE_ICON_INDEX_E;

typedef enum
{
	IDLE_ALIGN_LEFT,			//左对齐
	IDLE_ALIGN_RIGHT,			//右对齐
	IDLE_ALIGN_X_CENTER,		//居屏幕中间
	IDLE_ALIGN_CUSTOM,			//自定义
	IDLE_ALIGN_MAX		
}MMI_IDLE_DISPLAY_ALIGN_E;

//IDLE界面显示的文字的坐标，颜色，大小
typedef struct
{
	uint16						text_frame_x;		//文本显示区域的其实X坐标
    uint16						text_frame_top;		//文本显示区域的起始Y坐标
	MMI_IDLE_DISPLAY_ALIGN_E	text_align;			//文本显示对齐方式
    GUI_FONT_T					text_font;			//文本显示字体大小
}MMI_IDLE_DISPLAY_T;	

//定义待机界面下需要显示的枚举
typedef enum
{
    MMI_MAIN_LCD_NONE,
    MMI_MAIN_LCD_PLMN,				//待机MAIN LCD运营商文字
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_MAIN_LCD_PLMN2,				//待机MAIN LCD运营商文字
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    MMI_MAIN_LCD_PLMN3,				//待机MAIN LCD运营商文字
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    MMI_MAIN_LCD_PLMN4,				//待机MAIN LCD运营商文字
#endif
#endif
//    MMI_MAIN_LCD_SPN,               //待机MAIN LCD Service Providor name
    
	//MMI_MAIN_LCD_COMMEMORATE_DAY,	//待机MAIN LCD纪念日
	MMI_MAIN_LCD_CUSTOM_WORD,		//待机MAIN LCD自定义词语
	MMI_MAIN_LCD_COUNTEDTIME,		//待机MAIN LCD 倒记时显示,@zhaohui,counted time
	MMI_MAIN_LCD_CALENDAR,			//待机MAIN LCD 日历显示
	//MMI_MAIN_LCD_ALARM,				//待机MIAN LCD闹钟到了的时间文本显示
	
    MMI_MAIN_LCD_MP3,				//待机MAIN LCD MP3背景播放mp3 name
    MMI_MAIN_LCD_FM,                //待机MAIN LCD FM背景播放FM name
	MMI_MAIN_LCD_TIME,				//待机MAIN LCD时间
	MMI_MAIN_LCD_DATE,				//待机MAIN LCD日期
	MMI_MAIN_LCD_WEEK,				//待机MAIN LCD星期
    
	MMI_SUB_LCD_TIME,				//待机SUB LCD时间
	MMI_SUB_LCD_DATE,				//待机SUB LCD日期
	MMI_SUB_LCD_WEEK,				//待机SUB LCD星期	
	MMI_SUB_LCD_ALARM,				//SUB LCD闹钟到了的时间文本显示
	MMI_SUB_LCD_COUNTEDTIME,        //SUB LCD倒记时显示,
	MMI_MAIN_LCD_STK_IDLE_MODE,     //STK  在set up idle mode text需要显示的文本

    MMI_MAIN_LCD_LEFT_SOFTKEY,      //左软键
    MMI_MAIN_LCD_RIGHT_SOFTKEY,     //右软键

	TOTAL_IDLE_DISPLAY_NUMBER
} MMI_IDLE_DISPLAY_INDEX_E;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  
/*****************************************************************************/
// 	Description : get idle display style
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
MMI_IDLE_DISPLAY_T  MMITHEME_GetIdleDispStyle(MMI_IDLE_DISPLAY_INDEX_E e_idle_disp_index);

/*****************************************************************************/
// 	Description : get idle icon display position
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMITHEME_GetIdleIconPos(MMI_IDLE_ICON_INDEX_E e_icon);
  
  
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  
