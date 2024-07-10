/*****************************************************************************
** File Name:      mmidisplay_data.c                                         *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#define _MMIDISPLAY_DATA_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_theme_trc.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmi_image.h"

#ifdef DP_SCREEN_SIZE_240X240
    #include "mmidisplay_data_240x320.c"
#elif defined MAINLCD_SIZE_128X160
    #include "mmidisplay_data_128x160.c"
#elif defined MAINLCD_SIZE_176X220
    #include "mmidisplay_data_176x220.c"
#elif defined(MAINLCD_SIZE_128X64)
    #include "mmidisplay_data_128x64.c"	
#else
    #include "mmidisplay_data_240x320.c"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/
//待机界面内屏icon和动画的的显示位置，和MMI_IDLE_ICON_INDEX_E类型一一对应
#ifdef MMI_MULTI_SIM_SYS_DUAL
#define TITLE_ICON_NUM      11
#else
#define TITLE_ICON_NUM      10
#endif

LOCAL const GUI_POINT_T mmi_idle_icon_anim[TOTAL_IDLE_ICONANIM_NUMBER] =   
{
    0,0,    //MAIN LCD背景图片起始位置坐标 
    0,MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT,    //MAIN LCD中SOFTKEY BAR 起始位置坐标
    0,1,												    //MAIN LCD信号量 
#ifdef MMI_MULTI_SIM_SYS_DUAL
    30, 1,                                                  //MAIN LCD信号量2
#endif    
    1 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM +10, 1,		/*27*/	//MAIN LCD gprs状态标志
    3 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 8, 1,		/*59*/	//MAIN LCD短信提示和短信息满提示
    8 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3   , 1,	/*139*/	//MAIN LCD 耳机
    2 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 12, 1,		/*46*/	//MAIN LCD闹钟提示
    7 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3   , 1,	/*122*/	//MAIN LCD情景模式：静音，铃声，振动提示
    4 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 8, 1,		/*76*/	//MAIN LCD呼叫转移
    6 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 3  , 1,		/*105*/	//MAIN LCD蓝牙
    9 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM - 2, 1, 		/*151*/	//MAIN LCD电池
    24,18,          //关机下main lcd 充电动画
    29,9,           //MAIN LCD闹钟闹铃动画
    5 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM + 6, 1,	 	/*91*/   //MAIN LCD通话线路状态
    8 * MMI_MAINSCREEN_WIDTH / TITLE_ICON_NUM ,    1, 		/*无效*/	//MAIN LCD SD卡提示
};

//IDLE界面显示的文字的坐标，颜色，大小
#if defined SUBLCD_DEV_SIZE_96X64
LOCAL const MMI_IDLE_DISPLAY_T  mmi_sub_96x64_display[] =
{
    //待机SUB LCD时间 
	{0,
     25,
	 IDLE_ALIGN_X_CENTER,
	 SONG_FONT_22},
	
	//待机SUB LCD日期  
	{0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16,
	},

	//待机SUB LCD星期
	{0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16, 
	},

	//SUB LCD闹钟到了的时间文本显示
	{0,
	32, 
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_20            
	},
	//SUB LCD倒计时文本显示
    {0,
	45,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16
	},
};
#else
LOCAL const MMI_IDLE_DISPLAY_T  mmi_sub_96x96_display[] =
{
    //待机SUB LCD时间 
	{0,
     50,
	 IDLE_ALIGN_X_CENTER,
	 SONG_FONT_22},
	
	//待机SUB LCD日期  
	{0,
	75,
	IDLE_ALIGN_X_CENTER,
	SONG_FONT_16,
	},

	//待机SUB LCD星期
	{0,
	50,
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_NORMAL_FONT, 
	},

	//SUB LCD闹钟到了的时间文本显示
	{0,
	32, 
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_BIG_FONT            
	},
	//SUB LCD倒计时文本显示
    {0,
	50,
	IDLE_ALIGN_X_CENTER,
	MMI_DEFAULT_TEXT_FONT
	},
};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern const MMI_IDLE_DISPLAY_T  mmi_idle_display[];

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get idle display style
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMI_IDLE_DISPLAY_T MMITHEME_GetIdleDispStyle(
                                                    MMI_IDLE_DISPLAY_INDEX_E    e_idle_disp_index
                                                    )
{
    //uint16  height = 0;

    if ((MMI_MAIN_LCD_NONE >= e_idle_disp_index) && (TOTAL_IDLE_DISPLAY_NUMBER <= e_idle_disp_index))
    {
        //SCI_TRACE_LOW:"== MMITHEME_GetIdleDispStyle == e_idle_disp_index %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDISPLAY_DATA_119_112_2_18_3_1_51_5,(uint8*)"d", e_idle_disp_index);
        return mmi_idle_display[0];
    }

    switch(e_idle_disp_index)
    {
    case MMI_SUB_LCD_TIME:
    case MMI_SUB_LCD_DATE:
    case MMI_SUB_LCD_WEEK:
    case MMI_SUB_LCD_ALARM:			
	case MMI_SUB_LCD_COUNTEDTIME:
#ifdef SUBLCD_DEV_SIZE_96X64
        return mmi_sub_96x64_display[e_idle_disp_index - (uint32)(MMI_SUB_LCD_TIME)];
#else
        return mmi_sub_96x96_display[e_idle_disp_index - (uint32)(MMI_SUB_LCD_TIME)];
#endif
    default:
        break;
    }

    return (mmi_idle_display[e_idle_disp_index-1]);
}

/*****************************************************************************/
// 	Description : get idle icon display position
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMITHEME_GetIdleIconPos(
                                           MMI_IDLE_ICON_INDEX_E    e_icon
                                           )
{
    if (e_icon < TOTAL_IDLE_ICONANIM_NUMBER)
    {
    	return (mmi_idle_icon_anim[e_icon]);
    }
    else
    {
        //SCI_TRACE_LOW:"== MMITHEME_GetIdleIconPos == e_icon: %d is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDISPLAY_DATA_161_112_2_18_3_1_52_7,(uint8*)"d",e_icon);
    	return (mmi_idle_icon_anim[0]);
    }
}

