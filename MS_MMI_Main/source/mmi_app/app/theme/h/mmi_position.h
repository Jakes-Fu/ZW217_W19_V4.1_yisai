/*****************************************************************************
** File Name:      mmi_position.h                                            *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMI_POSITION_H_
#define _MMI_POSITION_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmitheme_pos.h"
#include "mmitheme_tab.h"
#include "mmitheme_title.h"

#if (defined DP_SCREEN_SIZE_240X240) || (defined MAINLCD_DEV_SIZE_240X320) || (defined MAINLCD_DEV_SIZE_240X240)
    #include "mmi_position_240X320.h"
#elif defined MAINLCD_DEV_SIZE_128X160
    #include "mmi_position_128X160.h"
#else
    #error
#endif

#include "mmi_adapt_screen.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#if defined TOUCH_PANEL_HWICON_SUPPORT_VIRTUAL_2KEY
#define MMI_HARDWARE_ICON_MAX DP2PX_VALUE(2) 
#elif defined TOUCH_PANEL_HWICON_SUPPORT_DIAL_PB_SMS_MAINMENU
#define MMI_HARDWARE_ICON_MAX DP2PX_VALUE(4) 
#else
#define MMI_HARDWARE_ICON_MAX DP2PX_VALUE(5) 
#endif

#define MMI_HW_SHORTCUT_BAR_HEIGHT              DP2PX_VALUE(25)//必须和TP_HWICON_Y_LENGHT一致
#define MMI_HW_SHORTCUT_BAR_LEFT                0
#define MMI_HW_SHORTCUT_BAR_TOP                 (MMI_MAINSCREEN_HEIGHT + 1)
#define MMI_HW_SHORTCUT_BAR_RIGHT               MMI_MAINSCREEN_WIDTH
#define MMI_HW_SHORTCUT_BAR_BOTTOM              (MMI_MAINSCREEN_HEIGHT + MMI_HW_SHORTCUT_BAR_HEIGHT)

#define MMI_HARDWARE_ICON_TOP                   MMI_HW_SHORTCUT_BAR_TOP
#define MMI_HARDWARE_ICON_BOTTOM                MMI_HW_SHORTCUT_BAR_BOTTOM

#define MMI_SUBSCREEN_WIDTH  DP2PX_VALUE(96)
#define MMI_SUBSCREEN_HIEGHT DP2PX_VALUE(96)	  
#define MMI_SUBSCREEN_LINE_HIEGHT DP2PX_VALUE(16)
#define MMI_SUBSCREEN_ICON_SPACE DP2PX_VALUE(1)	 

#define MMIPROGRESS_SPACE_NUM          (MMIPROGRESS_BAR_WIDTH/2)
#define MMIPROGRESS_BAR_HEITH        DP2PX_VALUE(5)

#ifdef GUIF_SOFTKEY_PDASTYLE
#ifdef MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT
#undef MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT
#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT 0
#endif
#endif

#ifdef MENU_CRYSTALCUBE_SUPPORT
    #define MENUICON_CRYSTALCUBE_SELECTICON_WIDTH      DP2PX_VALUE(56)
    #define MENUICON_CRYSTALCUBE_SELECTICON_HEIGHT     DP2PX_VALUE(56)
#endif

#ifdef SUB_LCD_96_64
    #define MMI_SUBSCREEN_WIDTH             DP2PX_VALUE(96)
    #define MMI_SUBSCREEN_HEIGHT            DP2PX_VALUE(64)
#else
    #define MMI_SUBSCREEN_WIDTH             DP2PX_VALUE(96)
    #define MMI_SUBSCREEN_HEIGHT            DP2PX_VALUE(96)
#endif

//softkey
#ifdef GUIF_SOFTKEY_PDASTYLE
	#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT   DP2PX_VALUE(0)  //全屏下SOFTKEY栏高度
	#define MMI_PDA_SOFTKEY_BAR_HEIGHT          DP2PX_VALUE(40) //全屏下SOFTKEY栏高度
#elif defined(MAINLCD_PANEL_SIZE_240X240) || defined (MAINLCD_PANEL_SIZE_360X360) || defined (MAINLCD_PANEL_SIZE_400X400) || defined(MAINLCD_DEV_SIZE_240X240)
	//标识为手表设备
	#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT   DP2PX_VALUE(0) // 保证在240X240分辨率下，隐去softkey

	//W307手表为圆形，使用新数值定义SOFTKEY
	#ifdef ADULT_WATCH_SUPPORT
		#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT   DP2PX_VALUE(60) 
	#endif
#else
#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT   DP2PX_VALUE(80) //全屏下softkey显示高度，仅为手机样式使用，保证在lcd需求为240 X240的情况下，填满屏幕所用
#endif

#define MMI_MAINSCREEN_RIGHT_MAX_PIXEL      (MMI_MAINSCREEN_WIDTH-1)
#define MMI_MAINSCREEN_BOTTOM_MAX_PIXEL	    (MMI_MAINSCREEN_HEIGHT-1)

#define MMI_LEFT_ARROW_RECT                 {MMI_TITLE_LEFT_ARROW_LEFT, MMI_TITLE_LEFT_ARROW_TOP, MMI_TITLE_LEFT_ARROW_RIGHT, MMI_TITLE_LEFT_ARROW_BOTTOM}
#define MMI_RIGHT_ARROW_RECT                {MMI_TITLE_RIGHT_ARROW_LEFT, MMI_TITLE_RIGHT_ARROW_TOP, MMI_TITLE_RIGHT_ARROW_RIGHT, MMI_TITLE_RIGHT_ARROW_BOTTOM}                                              MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT}

#define MMI_CLIENT_RECT_TOP                 MMI_TITLE_HEIGHT
#define MMI_CLIENT_RECT_BOTTOM              (MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT)

//default info
#define MMI_DEFAULT_SUBLCD_BLOCK	0

#define MMI_MAINSCREEN_MP3_LEFT     DP2PX_VALUE(10)
#define MMI_MAINSCREEN_MP3_WIDTH    (MMI_MAINSCREEN_WIDTH-MMI_MAINSCREEN_MP3_LEFT-MMI_MAINSCREEN_MP3_LEFT)

#define MMI_MAINSCREEN_IDLE_STK_LEFT     DP2PX_VALUE(10)
#define MMI_MAINSCREEN_IDLE_STK_WIDTH    (MMI_MAINSCREEN_WIDTH - 2*MMI_MAINSCREEN_IDLE_STK_LEFT)
#define MMI_MAINSCREEN_IDLE_STK_HEIGHT   DP2PX_VALUE(20)

#define MMI_DNLIST_TEXT_LEFT_SPACE              DP2PX_VALUE(5) // for dropdownlist control

//add for prompt window and msgbox default
#define MMI_PROMPT_BK_PIC_WINDTH	DP2PX_VALUE(132)               //the width of pub win background picture
#define MMI_PROMPT_BK_PIC_HEIGHT	DP2PX_VALUE(140)				//the height of pub win background picture
#define MMI_PROMPT_IMAGE_WIDTH		DP2PX_VALUE(56)                //the width of pub win image
#define MMI_PROMPT_IMAGE_HEIGHT		DP2PX_VALUE(59)                //the height of pub win image
#define MMI_PROMPT_SK_HEIGHT		DP2PX_VALUE(25)                //the height of pub win soft key
#define MMI_PROMPT_TEXT_HEIGHT		DP2PX_VALUE(20)                //the height of pun win text rectangle
		
#define MMI_PROMPT_WIN_LEFT			(MMI_MAINSCREEN_WIDTH - MMI_PROMPT_BK_PIC_WINDTH )/2//14
#define MMI_PROMPT_WIN_TOP			(MMI_MAINSCREEN_HEIGHT - MMI_PROMPT_BK_PIC_HEIGHT)/2//37//20
#define MMI_PROMPT_WIN_RIGHT		(MMI_MAINSCREEN_WIDTH - MMI_PROMPT_WIN_LEFT -1)//113//115
#define MMI_PROMPT_WIN_BOTTOM		(MMI_MAINSCREEN_HEIGHT - MMI_PROMPT_WIN_TOP)//130//121//122


#define MMI_PROMPT_WIN_RECT			{MMI_PROMPT_WIN_LEFT, MMI_PROMPT_WIN_TOP, MMI_PROMPT_WIN_RIGHT, MMI_PROMPT_WIN_BOTTOM}//{14, 20, 114, 121}//{14, 20, 115, 122}
#define MMI_PROMPT_IMAGE_LEFT       (MMI_MAINSCREEN_WIDTH - MMI_PROMPT_IMAGE_WIDTH)/2//14
#define MMI_PROMPT_IMAGE_TOP        MMI_PROMPT_WIN_TOP//37
#define MMI_PROMPT_IMAGE_RIGHT      (MMI_MAINSCREEN_WIDTH - MMI_PROMPT_IMAGE_LEFT)//114
#define MMI_PROMPT_IMAGE_BOTTOM     (MMI_PROMPT_IMAGE_TOP + MMI_PROMPT_IMAGE_HEIGHT)//90//67
#define MMI_PROMPT_IMAGE_RECT		{MMI_PROMPT_IMAGE_LEFT, MMI_PROMPT_IMAGE_TOP, MMI_PROMPT_IMAGE_RIGHT, MMI_PROMPT_IMAGE_BOTTOM}
#define MMI_PROMPT_TEXT_RECT		{MMI_PROMPT_WIN_LEFT,(MMI_PROMPT_IMAGE_BOTTOM + MMI_PROMPT_TEXT_HEIGHT),MMI_PROMPT_WIN_RIGHT,(MMI_PROMPT_IMAGE_BOTTOM + MMI_PROMPT_TEXT_HEIGHT *2)}//{14,90,114,105}//{14,77,114,93}//{14,77,115,93}
#define MMI_PROMPT_WAIT_IMAGE_RECT  {MMI_PROMPT_WIN_LEFT,MMI_PROMPT_WIN_TOP,(MMI_PROMPT_WIN_LEFT + MMI_PROMPT_BK_PIC_WINDTH),(MMI_PROMPT_WIN_TOP + MMI_PROMPT_IMAGE_HEIGHT)}
#define MMI_PROMPT_WAIT_TEXT_RECT   {MMI_PROMPT_WIN_LEFT,(MMI_PROMPT_WIN_TOP + MMI_PROMPT_IMAGE_HEIGHT),MMI_PROMPT_WIN_RIGHT,(MMI_PROMPT_WIN_BOTTOM - MMI_PROMPT_SK_HEIGHT)}
#define MMI_PROMPT_SK_RECT			{MMI_PROMPT_WIN_LEFT, (MMI_PROMPT_WIN_BOTTOM - MMI_PROMPT_SK_HEIGHT),MMI_PROMPT_WIN_RIGHT,MMI_PROMPT_WIN_BOTTOM}//{14, 112, 114, 130}//{15, 100, 113, 119}


#if defined(ADD_TITLE_IN_ALL_SETTING_MENU)
#define MMI_SPECIAL_TITLE_HEIGHT    45   //   240-3*MMI_LIST_ITEM_HEIGHT    //带返回箭头的title高度。  list一页只显示3个，显示4个的话太拥挤，不好手指操作
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
