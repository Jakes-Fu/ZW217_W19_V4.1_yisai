/*****************************************************************************
** File Name:      mmi_position_240x320.h                                            *
** Author:                                                                   *
** Date:           02/20/2020                                                *
** Copyright:
** Description:    This file is used to describe t9 text input    for   240x320   *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME                   DESCRIPTION                               *
** 02/2020      fangfang.yao              Creat
******************************************************************************/

#ifndef _MMI_POSITION__240X320H_
#define _MMI_POSITION__240X320H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#include "mmi_adapt_screen.h"

//the dinamic range of contact_editbox height in sms_mms_in_one window
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN DP2PX_VALUE(0)
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX DP2PX_VALUE(40)
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H DP2PX_VALUE(40)
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM DP2PX_VALUE(1)
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_EX DP2PX_VALUE(3)
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_H DP2PX_VALUE(2)
#endif

#define MMISMS_CHATFORM_SIM_LABEL_WIDTH DP2PX_VALUE(48)
#define MMISMS_CHATFORM_SIM_DATE_WIDTH  DP2PX_VALUE(100)
#define MMISMS_CHATFORM_ITEM_WIDTH DP2PX_VALUE(200)
#define MMISMS_CHATFORM_ANIM_WIDTH DP2PX_VALUE(40)

//the lcd width and height
#define MMI_MAINSCREEN_WIDTH                DP2PX_VALUE(240)

#ifndef ADULT_WATCH_SUPPORT
#define MMI_MAINSCREEN_HEIGHT               DP2PX_VALUE(240)
#else
    #ifdef DP_SCREEN_SIZE_240x240
    #define MMI_MAINSCREEN_HEIGHT DP2PX_VALUE(240)
    #else
    #define MMI_MAINSCREEN_HEIGHT               320
    #endif
#endif

#if defined(ZTE_SUPPORT_240X284)||defined(MAINLCD_DEV_SIZE_240X284)
#define MMI_MAINSCREEN_HEIGHT               DP2PX_VALUE(284)
#endif

//font
#define MMI_DEFAULT_FONT_HEIGHT             DP2PX_VALUE(20)
//title
#define MMI_TITLE_HEIGHT                    MMITHEME_GetWinTitleHeight()

#ifdef GUIF_TITLE_PDASTYLE
// status bar height
#define MMI_STATUSBAR_HEIGHT                    DP2PX_VALUE(30)//20 //19
#define MMI_TITLE_THEME_HEIGHT              MMITHEME_GetThemeTitleHeight_V()
#define MMI_TITLE_THEME_HEIGHT_H            MMITHEME_GetThemeTitleHeight_H()
#else
// status bar height
#define MMI_STATUSBAR_HEIGHT                DP2PX_VALUE(30)//20
#ifdef ADULT_WATCH_SUPPORT
#define MMI_TITLE_THEME_HEIGHT              DP2PX_VALUE(48)
#else
#define MMI_TITLE_THEME_HEIGHT              DP2PX_VALUE(48)//(28)
#endif
#define MMI_TITLE_THEME_HEIGHT_H            DP2PX_VALUE(32)
#endif
//browser
#define     MMIWAP_STATUSBAR_HEIGHT  MMI_STATUSBAR_HEIGHT

/*title button define start*/
#define	MMI_ISTYLE_TITLE_HEIGHT			DP2PX_VALUE(34)
#ifdef MMI_PDA_SUPPORT
#define	MMI_ISTYLE_TITLE_LEFT_BUTTON_X	DP2PX_VALUE(10)
#define	MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH	DP2PX_VALUE(80)

#define	MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH	DP2PX_VALUE(140)
#define	MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X		((MMI_MAINSCREEN_WIDTH-MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH)>>1)

#define	MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH	DP2PX_VALUE(32)
#define	MMI_ISTYLE_TITLE_RIGHT_BUTTON_X		(MMI_MAINSCREEN_WIDTH-DP2PX_VALUE(5)-MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH)

#define	MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH	DP2PX_VALUE(45)
#define	MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X		(MMI_MAINSCREEN_WIDTH-DP2PX_VALUE(5)-MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH)

#define	MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH		DP2PX_VALUE(60)
#define	MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X		(MMI_MAINSCREEN_WIDTH / 2 - DP2PX_VALUE(60))
#define	MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X		(MMI_MAINSCREEN_WIDTH/2)

#define	MMI_ISTYLE_TITLE_BUTTON_HEIGHT	DP2PX_VALUE(26)
/*title button define end*/

/*dialpad define start*/
#define	MMI_ISTYLE_DIAL_SIDE_BUTTON_WIDTH				DP2PX_VALUE(79)
#define	MMI_ISTYLE_DIAL_MIDDLE_BUTTON_WIDTH				DP2PX_VALUE(79)

#if defined(LOW_MEMORY_SUPPORT)
#define	MMI_ISTYLE_DIAL_NUM_BUTTON_HEIGHT				DP2PX_VALUE(42)
#define	MMI_ISTYLE_DIAL_HASH_BUTTON_HEIGHT				DP2PX_VALUE(42)
#define	MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT				DP2PX_VALUE(40)
#else
#define	MMI_ISTYLE_DIAL_NUM_BUTTON_HEIGHT				DP2PX_VALUE(53)
#define	MMI_ISTYLE_DIAL_HASH_BUTTON_HEIGHT				DP2PX_VALUE(53)
#define	MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT				DP2PX_VALUE(53)
#endif
/*dialpad define end*/

#define	MMI_ISTYLE_STATUSBAR_HEIGHT			MMI_STATUSBAR_HEIGHT

#define 	MMI_ISTYLE_LIST_ITEM_HEIGHT		DP2PX_VALUE(42)

#define	MMI_ISTYLE_TAB_HEIGHT				DP2PX_VALUE(42)
#define	MMI_ISTYLE_TAB_HEIGHT_H			DP2PX_VALUE(42)
#endif

#define MMI_DEFAULT_FONT_ASCII_WIDTH        DP2PX_VALUE(8)

#if defined (MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT) || defined(GUIF_SCROLLKEY)
#define MMI_SCROLLKEY_OFFSET_FROM_BOTTOM    DP2PX_VALUE(45)
#define MMI_FULLSCREEN_SCROLLKEY_BAR_HEIGHT DP2PX_VALUE(74)
#endif
        
//listbox
#define MMI_LIST_ICON_LEFT_SPACE            DP2PX_VALUE(5)
#define MMI_LIST_TEXT_LEFT_SPACE            DP2PX_VALUE(5)
#define MMI_LIST_SPLIT_ITEM_HEIGHT          DP2PX_VALUE(25)
#define MMI_LIST_ITEM_HEIGHT_SMALL_FONT     DP2PX_VALUE(18)  //小字体的list高度
#define MMI_LIST_TEXT_LEFT_SPACE_PS         DP2PX_VALUE(20)  //专用于phonesearch设置左边距离
#define MMI_LIST_RIGHT_SPACE                DP2PX_VALUE(5)


#define MMI_LIST_ITEM_RIGHT                 DP2PX_VALUE(234)

#define MMI_LIST_QBTHEME_ITEM_HEIGHT		DP2PX_VALUE(152)
#define MMI_LIST_QBTHEME_ITEM2_HEIGHT		DP2PX_VALUE(84)
#define MMI_LIST_QBTHEME_ANIM_WIDTH			DP2PX_VALUE(90)
#define MMI_LIST_QBTHEME_ANIM_HEIGHT		DP2PX_VALUE(74)
#define MMI_LIST_QBTHEME_ITEM2_SPACE		DP2PX_VALUE(10)
#define MMI_LIST_QBTHEME_ICON_WIDTH			DP2PX_VALUE(29)
#define MMI_LIST_QBTHEME_MARK_WIDTH			DP2PX_VALUE(32)
#define QB_FORM_MARGIN						DP2PX_VALUE(6)

#ifdef GUIF_LIST_PDASTYLE
#define MMI_LIST_ITEM_HEIGHT		        DP2PX_VALUE(50)
#define MMI_LIST_HEIGHT_SPACE		        DP2PX_VALUE(3)//
#define MMI_LIST_BUTTON_HEIGHT              DP2PX_VALUE(40)
#define MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT    MMI_LIST_ITEM_HEIGHT
#define MMI_LIST_ITEM_SELECT_ICON_WIDTH     DP2PX_VALUE(24)
#define MMI_LIST_ITEM_BIG_ICON_WIDTH        DP2PX_VALUE(40)
#define MMI_LIST_ICON_MARGIN                DP2PX_VALUE(10)
#else
#define MMI_LIST_ITEM_HEIGHT		        DP2PX_VALUE(26)//24  //list窗口中每项list的高度
#define MMI_LIST_HEIGHT_SPACE		        DP2PX_VALUE(3)//
#define MMI_LIST_BUTTON_HEIGHT              DP2PX_VALUE(24)
#define MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT    (MMI_LIST_ITEM_HEIGHT+MMI_LIST_ITEM_HEIGHT)
#ifdef ADULT_WATCH_SUPPORT
#define MMI_LIST_ITEM_SELECT_ICON_WIDTH     DP2PX_VALUE(36)
#else
#define MMI_LIST_ITEM_SELECT_ICON_WIDTH     DP2PX_VALUE(24)
#endif

#define MMI_LIST_ITEM_BIG_ICON_WIDTH        DP2PX_VALUE(40)
#ifdef ADULT_WATCH_SUPPORT
#define MMI_LIST_ICON_MARGIN                DP2PX_VALUE(0)
#else
#define MMI_LIST_ICON_MARGIN                DP2PX_VALUE(10)
#endif

#endif

#define MMI_LIST_2ANIM_ICON_WIDTH       (MMI_LIST_ITEM_SELECT_ICON_WIDTH + DP2PX_VALUE(4))

#define MMI_LIST_ITEM_TOP_MARGIN		    DP2PX_VALUE(6)
#define MMI_LIST_ITEM_BOTTOM_MARGIN		    DP2PX_VALUE(6)
// second menu
#ifdef GUIF_LIST_PDASTYLE
#define MMI_SECMENU_ITEM_HEIGHT		        DP2PX_VALUE(40)//MMI_LIST_ITEM_HEIGHT
#else
#define MMI_SECMENU_ITEM_HEIGHT		        MMI_LIST_ITEM_HEIGHT
#endif

//idle
#define	MMI_IDLE_ICON_AREA_HEIGH	        DP2PX_VALUE(23)
#define MMI_IDLE_ICON_AREA_UP               DP2PX_VALUE(4)        
#define MMI_IDLE_COMMON_NETWORKNAME_ADJUST  16    //基本风格IDLE界面运营商名字坐标调整 
//pubwin
//#define MMITHEME_PUBWIN_PROMPT_RECT             {0,203,97,287}
#define MMITHEME_PUBWIN_BG_RECT                 {0,DP2PX_VALUE(193),MMI_MAINSCREEN_RIGHT_MAX_PIXEL,DP2PX_VALUE(287)}
#define MMITHEME_PUBWIN_UP_BG_RECT              {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,DP2PX_VALUE(192)}
#define MMITHEME_PUBWIN_TITLE_RECT              {0,0,DP2PX_VALUE(219),DP2PX_VALUE(27)}

#define MMITHEME_PUBWIN_TEXT_RECT              {0,DP2PX_VALUE(194),MMI_MAINSCREEN_RIGHT_MAX_PIXEL,DP2PX_VALUE(287)}

#define MMITHEME_PUBWIN_TEXT1_RECT              DP2PX_RECT(62,216,239,240)
#define MMITHEME_PUBWIN_TEXT2_RECT              DP2PX_RECT(62,243,239,267)

#define MMITHEME_PUBWIN_3L_TEXT1_RECT           DP2PX_RECT(62,200,239,225)
#define MMITHEME_PUBWIN_3L_TEXT2_RECT           DP2PX_RECT(62,228,239,253)
#define MMITHEME_PUBWIN_3L_TEXT3_RECT           DP2PX_RECT(62,256,239,280)

#define MMITHEME_PUBWIN_IMAGE_DISP_RECT          DP2PX_RECT(0,217,51,261)

#define MMITHEME_PUBWIN_WAIT_RECT               DP2PX_RECT(108,229,131,252)
#define MMITHEME_PUBWIN_ANIM_DISP_RECT          DP2PX_RECT(0,203,97,287)
#define MMITHEME_PUBWIN_WAIT_BG_RECT            DP2PX_RECT(0,0,239,192)

//#define MMITHEME_PUBWIN_CC_WAIT_BG_RECT         {0,0,239,185}
// #define MMITHEME_PUBWIN_CC_WAIT_RECT            {0, 186, 239, 280}
#define MMITHEME_PUBWIN_CC_CONNECT_ANIM_RECT    DP2PX_RECT(104, 222, 128, 246)
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_RECT     DP2PX_RECT(148, 195, 236, 287)
//width and height必需和rect保持一致,否则导致浏览图片时来电Assert
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_WIDTH    DP2PX_VALUE(80)   
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_HEIGHT   DP2PX_VALUE(90)
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_HEIGHT_H DP2PX_VALUE(75)

#define MMITHEME_PUBWIN_WAIT_TEXT1_RECT         DP2PX_RECT(10,60,229,88)
#define MMITHEME_PUBWIN_WAIT_TEXT2_RECT         DP2PX_RECT(10,90,229,118)
#define MMITHEME_PUBWIN_WAIT_TEXT3_RECT         DP2PX_RECT(10,120,229,148)
#define MMITHEME_PUBWIN_WAIT_CC_TEXT_RIGHT      DP2PX_VALUE(130)
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_RIGHT      DP2PX_VALUE(130)
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_TOP        DP2PX_VALUE(222)
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_LEFT       DP2PX_VALUE(98)
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_BOTTOM     DP2PX_VALUE(246)

//pub win
#define MMITHEME_PUBWIN_4WAIT_RECT			    {DP2PX_VALUE(10), DP2PX_VALUE(32), DP2PX_VALUE(229), DP2PX_VALUE(32) + MMI_DEFAULT_FONT_HEIGHT}
#define MMITHEME_PUBWIN_4WAIT_OFFSET			DP2PX_VALUE(5)   // offset between 2 text in vertical
#define MMITHEME_PUBWIN_4WAIT_OFFSET_EX			DP2PX_VALUE(30)  // offset between status text and name text in vertical

#define MMI_KEYLOCK_BACKGROUND_START_LEFT       DP2PX_VALUE(17)		
#define MMI_KEYLOCK_SLIDE_WIDTH					DP2PX_VALUE(44)
#define MMI_KEYLOCK_CUE_LEFT					DP2PX_VALUE(12)
#define MMI_KEYLOCK_CUE_TOP						MMI_MAINSCREEN_HEIGHT - DP2PX_VALUE(10) - DP2PX_VALUE(20)
#define MMI_KEYLOCK_CUE_RIGHT					MMI_MAINSCREEN_WIDTH - DP2PX_VALUE(12)
#define MMI_KEYLOCK_CUE_BOTTOM					MMI_MAINSCREEN_HEIGHT - DP2PX_VALUE(10)
    
//keylock
#define MINUTE_LEG_R_1		DP2PX_VALUE(48)
#define MINUTE_LEG_R_2		DP2PX_VALUE(10)
#define MINUTE_LEG_WIDTH	DP2PX_VALUE(3)
#define HOUR_LEG_R_1		DP2PX_VALUE(35)
#define HOUR_LEG_R_2		DP2PX_VALUE(8)
#define HOUR_LEG_WIDTH	DP2PX_VALUE(5)

//for gsensor compiling error
#define MINUTE_LEG_R_1_H	DP2PX_VALUE(38)
#define MINUTE_LEG_R_2_H	DP2PX_VALUE(8)
#define HOUR_LEG_R_1_H		DP2PX_VALUE(28)
#define HOUR_LEG_R_2_H		DP2PX_VALUE(6)
//endif
#define CIRCLE_CENTER_POINT	       {DP2PX_VALUE(120), DP2PX_VALUE(-89)}
#define CIRCLE_CENTER_POINT_H_X     DP2PX_VALUE(160)
#define CIRCLE_CENTER_POINT_H_Y    DP2PX_VALUE(-61)
#define MMI_KEYLOCK_SHOW_ICON_MAX           3           //锁屏未处理信息显示图标的最大个数
#define MMI_KEYLOCK_SHOW_TOP                DP2PX_VALUE(197)         //锁屏未处理信息显示的TOP
#define MMI_KEYLOCK_SHOW_ICON_HEIGHT        DP2PX_VALUE(20)          //锁屏未处理信息图标的高度
#define MMI_KEYLOCK_SHOW_BOTTOM             MMI_KEYLOCK_SHOW_TOP + MMI_KEYLOCK_SHOW_ICON_HEIGHT         //锁屏未处理信息显示的BOTTOM

#define MMI_KEYLOCK_SHOW_ICON_WIDTH         DP2PX_VALUE(20)          //锁屏未处理信息图标的宽度
#define MMI_KEYLOCK_SHOW_EDGE               DP2PX_VALUE(20)          //锁屏未处理信息屏幕两边的间距
#define MMI_KEYLOCK_SHOW_SPACE              DP2PX_VALUE(3)           //锁屏未处理信息图标与文字间的间隔
#define MMI_KEYLOCK_SHOW_TEXT_WIDTH         DP2PX_VALUE(40)          //锁屏未处理信息文字显示的宽度
#define MMI_KEYLOCK_SHOW_GAP                DP2PX_VALUE(5)
#define MMI_KEYLOCK_ICON1_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_TEXT1_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_ICON2_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP+1,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_TEXT2_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_ICON3_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_TEXT3_RECT               DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*3+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM)
//gsensor
#define MMI_KEYLOCK_SHOW_TOP_H              DP2PX_VALUE(126)     //锁屏未处理信息显示的TOP
#define MMI_KEYLOCK_SHOW_BOTTOM_H             MMI_KEYLOCK_SHOW_TOP_H + MMI_KEYLOCK_SHOW_ICON_HEIGHT         //锁屏未处理信息显示的BOTTOM
#define MMI_KEYLOCK_SHOW_EDGE_H             DP2PX_VALUE(80)          //锁屏未处理信息屏幕两边的间距

#define MMI_KEYLOCK_ICON1_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH,MMI_KEYLOCK_SHOW_BOTTOM)
#define MMI_KEYLOCK_TEXT1_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1,MMI_KEYLOCK_SHOW_BOTTOM_H)
#define MMI_KEYLOCK_ICON2_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP+1,MMI_KEYLOCK_SHOW_BOTTOM_H)
#define MMI_KEYLOCK_TEXT2_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_BOTTOM_H)
#define MMI_KEYLOCK_ICON3_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM_H)
#define MMI_KEYLOCK_TEXT3_RECT_H             DP2PX_RECT(MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*3+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM_H)

#define MMI_DIAL_OPTION_BUTTON_FORM_HEIGHT DP2PX_VALUE(40)
#ifdef TOUCH_PANEL_SUPPORT

//以下带_H表示横屏时位置，反之为竖屏位置
//search list 
#define MMI_DIAL_SEARCHLIST_RECT_HEIGHT DP2PX_VALUE(66)
#define MMI_DIAL_SEARCHLIST_RECT_HEIGHT_H DP2PX_VALUE(28)


/*该处设置各button之间的间隔*/
#define MMI_DIAL_FORM_VER_SPACE           		0
#define MMI_DIAL_FORM_HOR_SPACE           		0
#define MMI_DIAL_FORM_VER_SPACE_H           		0
#define MMI_DIAL_FORM_HOR_SPACE_H           		DP2PX_VALUE(7)

//option dial form 
#define MMI_DIAL_OPTION_BUTTON_FORM_LEFT DP2PX_VALUE(10)
#define MMI_DIAL_OPTION_BUTTON_FORM_RIGHT DP2PX_VALUE(229)
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP DP2PX_VALUE(246)
#define MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM DP2PX_VALUE(285)


//dialing window key position
//dialing form
/*除了del按键和edit控件，其他button构成一个form,此处为该form的rect*/
#define MMI_DIAL_FORM_TOP                   DP2PX_VALUE(135)
#define MMI_DIAL_FORM_TOP_H                   DP2PX_VALUE(93)

#define MMI_DIAL_OPTION_BUTTON_FORM_LEFT_H DP2PX_VALUE(255)
#define MMI_DIAL_OPTION_BUTTON_FORM_RIGHT_H DP2PX_VALUE(310)
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP_H DP2PX_VALUE(56)
#define MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM_H DP2PX_VALUE(205)

#define MMIIDLE_TOOLBAR_START_POINT_X DP2PX_VALUE(50)
#define MMIIDLE_TOOLBAR_START_POINT_Y  DP2PX_VALUE(290)

#define MMIIDLE_TOOLBAR_START_POINT_X_H DP2PX_VALUE(120)
#define MMIIDLE_TOOLBAR_START_POINT_Y_H  DP2PX_VALUE(100)

#define MMI_DIAL_TOOLBAR_BUTTON_WIDTH DP2PX_VALUE(60)

#define MMI_DIAL_OPTION_BUTTON_LIST_BOTTOM_H DP2PX_VALUE(206)

#define MMI_DIAL_TOOLBAR_BUTTON_FORM_LEFT_H DP2PX_VALUE(253)
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_TOP_H DP2PX_VALUE(56)  //56
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_RIGHT_H DP2PX_VALUE(312)
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_BOTTOM_H DP2PX_VALUE(92) //92

//拨号盘数字button『收起』时list和button form之间的距离
#define MMI_DIAL_SPACE_LIST_AND_OPTIONFORM  DP2PX_VALUE(5)
#define MMI_DIAL_SPACE_LIST_AND_OPTIONFORM_H DP2PX_VALUE(5)
//拨号盘数字button『展开』时list和button form之间的距离
#define MMI_DIAL_SPACE_LIST_AND_NUMFORM DP2PX_VALUE(5)
#define MMI_DIAL_SPACE_LIST_AND_NUMFORM_H DP2PX_VALUE(4)
#endif

//tv
#define MMITVOUT_REMINDER_TEXT_POS_LEFT			DP2PX_VALUE(20	)	
#define MMITVOUT_REMINDER_TEXT_POS_TOP			DP2PX_VALUE(210)
#define MMITVOUT_REMINDER_TEXT_POS_RIGHT		DP2PX_VALUE(220)
#define MMITVOUT_REMINDER_TEXT_POS_BUTTOM		DP2PX_VALUE(250)

#ifdef TOUCH_PANEL_SUPPORT
//scroll bar
#define SCROLL_BAR_WIDTH					DP2PX_VALUE(16)      //菜单右侧滚动条的宽度
#else
#define SCROLL_BAR_WIDTH					DP2PX_VALUE(3)      //菜单右侧滚动条的宽度
#endif

//soft key icon
#define MMI_SOFTKEYBAR_FIRST_ICON_POS           DP2PX_VALUE(77)          // 第一个图标的位置
#define MMI_SOFTKEYBAR_SECOND_ICON_POS          DP2PX_VALUE(107)         // 第二个图标的位置
#define MMI_SOFTKEYBAR_THIRD_ICON_POS           DP2PX_VALUE(137)         // 第三个图标的位置

#if defined(MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT) ||defined(GUIF_SCROLLKEY)
#define MMI_SCROLLKEYBAR_FIRST_ICON_POS         DP2PX_VALUE(77)          // 第一个图标的位置
#define MMI_SCROLLKEYBAR_SECOND_ICON_POS        DP2PX_VALUE(137)         // 第二个图标的位置
#endif

//输入法spell、select条参数
#define MMI_SPELL_RECT_HEIGHT			DP2PX_VALUE(19)//15  //拼音框高度
#define MMI_SELECT_RECT_HEIGHT			DP2PX_VALUE(19)//15  //汉字候选框高度，拼音、笔划、手写模式时用到

// TAB控件
// TAB控件
#ifdef GUITAB_PDASTYLE_SUPPORT
#define	MMI_TAB_HEIGHT				DP2PX_VALUE(32)//44
#define	MMI_TAB_HEIGHT_H			DP2PX_VALUE(32)//44
#else
#define	MMI_TAB_HEIGHT				DP2PX_VALUE(72)
#define	MMI_TAB_HEIGHT_H			DP2PX_VALUE(72)
#endif

#define	MMI_TAB_BOTTOM				(MMITHEME_GetTabHeight() - 1)//53
// top value of child window.  
#define	MMI_TAB_CHILD_WINDOW_TOP	MMI_TAB_BOTTOM + 1 //54




#if (MMI_HARDWARE_ICON_MAX == 2 )
#define MMI_HARDWARE_ICON_1_LEFT         0
#define MMI_HARDWARE_ICON_1_RIGHT        (MMI_MAINSCREEN_WIDTH/2)
#define MMI_HARDWARE_ICON_2_LEFT         (MMI_HARDWARE_ICON_1_RIGHT+1)
#define MMI_HARDWARE_ICON_2_RIGHT         MMI_MAINSCREEN_WIDTH
#elif (MMI_HARDWARE_ICON_MAX == 4 )
#define MMI_HARDWARE_ICON_4_LEFT         DP2PX_VALUE(195)
#define MMI_HARDWARE_ICON_4_RIGHT        DP2PX_VALUE(218)

#define MMI_HARDWARE_ICON_3_LEFT         DP2PX_VALUE(142)
#define MMI_HARDWARE_ICON_3_RIGHT        DP2PX_VALUE(164)

#define MMI_HARDWARE_ICON_2_LEFT         DP2PX_VALUE(75)
#define MMI_HARDWARE_ICON_2_RIGHT        DP2PX_VALUE(105)

#define MMI_HARDWARE_ICON_1_LEFT         DP2PX_VALUE(26)
#define MMI_HARDWARE_ICON_1_RIGHT        DP2PX_VALUE(46)

#else
#define MMI_HARDWARE_ICON_1_LEFT         DP2PX_VALUE(13)
#define MMI_HARDWARE_ICON_1_RIGHT        DP2PX_VALUE(34)

#define MMI_HARDWARE_ICON_2_LEFT         DP2PX_VALUE(60)
#define MMI_HARDWARE_ICON_2_RIGHT        DP2PX_VALUE(81)

#define MMI_HARDWARE_ICON_3_LEFT         DP2PX_VALUE(108)
#define MMI_HARDWARE_ICON_3_RIGHT        DP2PX_VALUE(129)

#define MMI_HARDWARE_ICON_4_LEFT         DP2PX_VALUE(156)
#define MMI_HARDWARE_ICON_4_RIGHT        DP2PX_VALUE(177)

#define MMI_HARDWARE_ICON_5_LEFT         DP2PX_VALUE(204)
#define MMI_HARDWARE_ICON_5_RIGHT        DP2PX_VALUE(227)
#endif

#define MMI_MAIN_TIME_0_X           DP2PX_VALUE(4)
#define MMI_MAIN_TIME_1_X           DP2PX_VALUE(21)
#define MMI_MAIN_TIME_COLON_X       DP2PX_VALUE(40)
#define MMI_MAIN_TIME_2_X           DP2PX_VALUE(48)
#define MMI_MAIN_TIME_3_X           DP2PX_VALUE(67)
#define MMI_MAIN_TIME_AM_PM_X       DP2PX_VALUE(86)
#define IDLE_DATE_LEFT	 			 (MMI_MAIN_TIME_AM_PM_X + DP2PX_VALUE(20)+DP2PX_VALUE(2))


#define IDLE_DATE_TOP 				DP2PX_VALUE(26)
#define IDLE_WEEK_TOP 				DP2PX_VALUE(42)
#define IDLE_WEEK_LEFT	 			DP2PX_VALUE(160)
#define IDLE_DATE_TOP_H 			DP2PX_VALUE(26)
#define IDLE_WEEK_TOP_H 		       DP2PX_VALUE(42)
#define IDLE_DATE_LEFT_H                   DP2PX_VALUE(240)   
#define IDLE_DATE_LEFT_MARGIN         DP2PX_VALUE(5)

#define MMI_MAIN_TIME_Y             DP2PX_VALUE(26)
#define MMI_MAIN_TIME_Y_H         DP2PX_VALUE(26)//250 crMS00187659 @murphyxie
#define MMI_MAIN_TIME_H             DP2PX_VALUE(38)
#define MMI_MAIN_TIME_COLON_Y       MMI_MAIN_TIME_Y + DP2PX_VALUE(5)
#define MMI_MAIN_TIME_COLON_Y_H     MMI_MAIN_TIME_Y_H + DP2PX_VALUE(5)
#define MMI_MAIN_TIME_AM_PM_Y       MMI_MAIN_TIME_Y + DP2PX_VALUE(12)
#define MMI_MAIN_TIME_AM_PM_Y_H     MMI_MAIN_TIME_Y_H + DP2PX_VALUE(12)

#ifdef QBTHEME_SUPPORT
#define QB_PANEL_LAYER_HEIGHT		DP2PX_VALUE(40)
#define QB_PANEL_BACK_BTN_RECT      DP2PX_RECT(2, 5, 2+27-1, 5+27-1)
#define QB_PANEL_NAME_LABEL_RECT	DP2PX_RECT(31, 5, 31+80, 5+15-1)          //{11, 5, 11+100, 5+15-1}
#define QB_PANEL_SIZE_LABEL_RECT	DP2PX_RECT(31, 5+15+1, 31+80, 5+15+15+1)  //{11, 5+15+1, 11+100, 5+15+15+1}
#define QB_PANEL_LEFT_BTN_RECT		DP2PX_RECT(125, 4, 125+51-1, 4+34-1)
#define QB_PANEL_RIGHT_BTN_RECT		DP2PX_RECT(125+51, 4, 125+51+51-1, 4+34-1)
#define QB_PANEL_LEFT_KEY_RECT		DP2PX_RECT(31, 284, 31+82-1, 284+32-1)
#define QB_PANEL_RIGHT_KEY_RECT		DP2PX_RECT(208-82+1, 284, 208,315)
#define QB_PANEL_MID_KEY_RECT		  DP2PX_RECT(79, 284, 79+82-1, 315)
#endif

//progress bar 
#define MMIPROGRESS_BAR_GROOVE_LEFT  DP2PX_VALUE(56)
#define MMIPROGRESS_BAR_LEFT         DP2PX_VALUE(57)
#define MMIPROGRESS_BAR_WIDTH        DP2PX_VALUE(124)

//iconlist
#ifdef GUIF_ICONLIST_DELIMITER
#define MMITHEME_DELIMITER_WIDTH            DP2PX_VALUE(239)     //分隔符宽度
#define MMITHEME_DELIMITER_HEIGHT           DP2PX_VALUE(30)      //分隔符高度
#define MMITHEME_DELIMITER_STR_LEFT_SPACE   DP2PX_VALUE(10)      //分隔符内字符的左留白的大小(默认左对齐)
#define MMITHEME_DELIMITER_BG_LEFT_SPACE    0       //分隔符较大，分隔符背景较小的某些特例情况
#define MMITHEME_DELIMITER_BG_WIDTH         0
#define MMITHEME_DELIMITER_BG_TOP_SPACE     0
#define MMITHEME_DELIMITER_BG_HEIGHT        0
#endif

#ifdef GUIF_TITLE_PDASTYLE
#define MMI_TITLE_TEXT_MARGIN_PDA           DP2PX_VALUE(5)
#define MMI_TITLE_THEME_HEIGHT_PDA          DP2PX_VALUE(32)//20
#define MMI_TITLE_THEME_HEIGHT_H_PDA        DP2PX_VALUE(32)//20
#define MMI_TITLE_THEME_HEIGHT_ISTYLE_PDA          DP2PX_VALUE(34)
#define MMI_TITLE_THEME_HEIGHT_H_ISTYLE_PDA        DP2PX_VALUE(34)
#endif
#define MMI_TAB_ITEM_TEXT_BOTTOM_SPACE      DP2PX_VALUE(7)

#define MMI_TAB_CARD_DEFAULT_WIDTH          DP2PX_VALUE(40)

#ifdef GUITAB_PDASTYLE_SUPPORT
#define MMI_TAB_DISP_MAX_ITEM_NUM           DP2PX_VALUE(4)
#define MMI_TAB_DISP_MAX_ITEM_NUM_H         DP2PX_VALUE(5)
#define MMI_TAB_DISP_MAX_ITEM_NUM_ISTYLE           DP2PX_VALUE(5)
#define MMI_TAB_ITEM_DEFAULT_HEIGHT         DP2PX_VALUE(44)
#define MMI_TAB_ITEM_DEFAULT_WIDTH          DP2PX_VALUE(60)
#define MMI_TAB_ITEM_DEFAULT_WIDTH_H        DP2PX_VALUE(64)
#define MMI_TAB_ITEM_DEFAULT_WIDTH_ISTYLE          DP2PX_VALUE(48)
#define MMI_TAB_PRG_HEIGHT				    DP2PX_VALUE(3)
#else
#define MMI_TAB_DISP_MAX_ITEM_NUM           DP2PX_VALUE(4)
#define MMI_TAB_DISP_MAX_ITEM_NUM_H       DP2PX_VALUE(5)
#define MMI_TAB_ITEM_DEFAULT_HEIGHT         DP2PX_VALUE(33)
#define MMI_TAB_ITEM_DEFAULT_WIDTH          DP2PX_VALUE(40)
#define MMI_TAB_ITEM_DEFAULT_WIDTH_H      DP2PX_VALUE(48)
#define MMI_TAB_PRG_HEIGHT				DP2PX_VALUE(4)
#endif

//  soft key 文字距离水平和垂直方向的距离
#define MMI_SOFTKEY_TEXT_HOR_SPACE  DP2PX_VALUE(2)

#define MMI_NUMBER_EDIT_TOP_MARGIN    DP2PX_VALUE(20)
#define MMI_NUMBER_EDIT_HEIGHT    DP2PX_VALUE(30)
//password
#define MMI_PASSWORD_EDIT_LEFT      DP2PX_VALUE(30)
#define MMI_PASSWORD_EDIT_TOP       DP2PX_VALUE(68)
#define MMI_PASSWORD_EDIT_RIGHT     (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMI_PASSWORD_EDIT_LEFT)
#define MMI_PASSWORD_EDIT_BOTTOM    (MMI_PASSWORD_EDIT_TOP + DP2PX_VALUE(30))
#define MMI_PASSWORD_IMAGE_LEFT         MMI_TIME_IMAGE_LEFT
#define MMI_PASSWORD_IMAGE_TOP          MMI_TIME_IMAGE_TOP
#define MMI_PASSWORD_EDIT_TOP_MARGIN    DP2PX_VALUE(20)
#define MMI_PASSWORD_EDIT_TOP_MARGIN_H    DP2PX_VALUE(20)     
#define MMI_PASSWORD_EDIT_HEIGHT    DP2PX_VALUE(30)

//time/date
#define MMI_TIME_EDIT_LEFT          DP2PX_VALUE(10)
#define MMI_TIME_EDIT_TOP           DP2PX_VALUE(68)
#define MMI_TIME_EDIT_RIGHT         (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMI_TIME_EDIT_LEFT)
#define MMI_TIME_EDIT_BOTTOM        (MMI_TIME_EDIT_TOP + DP2PX_VALUE(55))
#define MMI_TIME_IMAGE_LEFT         DP2PX_VALUE(73)
#define MMI_TIME_IMAGE_TOP          DP2PX_VALUE(160)
#define MMI_DATE_EDIT_LEFT          MMI_TIME_EDIT_LEFT
#define MMI_DATE_EDIT_TOP           MMI_TIME_EDIT_TOP
#define MMI_DATE_EDIT_RIGHT         MMI_TIME_EDIT_RIGHT
#define MMI_DATE_EDIT_BOTTOM        MMI_TIME_EDIT_BOTTOM
//title左侧margin调整为10，配合居左显示以保证所有title都可完整显示，也比较美观
#define MMI_TITLE_TEXT_L_MARGIN	            DP2PX_VALUE(10)//45
//title
#define MMI_TITLE_ARROW_MARGIN	        DP2PX_VALUE(3)
#define MMI_TITLE_ARROW_RECT_WIDTH      DP2PX_VALUE(24) 
#define MMI_TITLE_ISTYLE_MENU_ICON_MARGIN	 DP2PX_VALUE(3)

#define MMI_TITLE_LEFT_ARROW_WIDTH      DP2PX_VALUE(24) 
#define MMI_TITLE_LEFT_ARROW_HEIGHT     DP2PX_VALUE(22)
#define MMI_TITLE_RIGHT_ARROW_WIDTH    DP2PX_VALUE(24)
#define MMI_TITLE_RIGHT_ARROW_HEIGHT   DP2PX_VALUE(22)

#define MMI_TITLE_LEFT_ARROW_LEFT       DP2PX_VALUE(3)
#define MMI_TITLE_LEFT_ARROW_TOP        DP2PX_VALUE(3)
#define MMI_TITLE_LEFT_ARROW_RIGHT      ( MMI_TITLE_LEFT_ARROW_LEFT + MMI_TITLE_LEFT_ARROW_WIDTH - 1)
#define MMI_TITLE_LEFT_ARROW_BOTTOM     (MMI_TITLE_LEFT_ARROW_TOP+MMI_TITLE_LEFT_ARROW_HEIGHT-1)

#define MMI_TITLE_RIGHT_ARROW_LEFT		(MMI_MAINSCREEN_WIDTH - MMI_TITLE_LEFT_ARROW_LEFT -MMI_TITLE_RIGHT_ARROW_WIDTH)
#define MMI_TITLE_RIGHT_ARROW_TOP       MMI_TITLE_LEFT_ARROW_TOP
#define MMI_TITLE_RIGHT_ARROW_RIGHT     ( MMI_TITLE_RIGHT_ARROW_LEFT + MMI_TITLE_RIGHT_ARROW_WIDTH - 1)
#define MMI_TITLE_RIGHT_ARROW_BOTTOM    (MMI_TITLE_RIGHT_ARROW_TOP+MMI_TITLE_RIGHT_ARROW_HEIGHT-1)

// tips position info
#define MMITIPS_RECT_MIN_HEIGHT                DP2PX_VALUE(22)   // 默认高度
#define MMITIPS_RECT_MAX_WIDTH                 DP2PX_VALUE(218)  // 默认宽度

#ifdef MENU_CRYSTALCUBE_SUPPORT
    #define MMI_CRYSTALCUBE_REGION_WIDTH           DP2PX_VALUE(180)//192
    #define MMI_CRYSTALCUBE_REGION_HEIGHT          DP2PX_VALUE(220)//224
#endif

    #define MMI_CUBEPAGE_PAGE_RECT          {DP2PX_VALUE(24), DP2PX_VALUE(61), DP2PX_VALUE(24)+MMI_CRYSTALCUBE_REGION_WIDTH-1, DP2PX_VALUE(61)+MMI_CRYSTALCUBE_REGION_HEIGHT-1}//jinx


//============================================================================//
//                                   For watch                                //
//============================================================================//

#define WATCHLIST_ITEM_HEIGHT_SMALL                      DP2PX_VALUE(30)
#define WATCHLIST_ITEM_HEIGHT                            DP2PX_VALUE(80)
#define WATCHLIST_TEXT_WIDGH_BIG                         DP2PX_VALUE(214)
#define WATCHLIST_TEXT_WIDGH_MID                         DP2PX_VALUE(155)
#define WATCHLIST_TEXT_WIDGH_SMALL                       DP2PX_VALUE(126)
#define WATCHLIST_TEXT_WIDGH_MIN                         DP2PX_VALUE(100)

#define WATCHLIST_BG_ICON_LEFT_SPACE                     DP2PX_VALUE(15)
#define WATCHLIST_BG_ICON_TOP_SPACE                      DP2PX_VALUE(13)
#define WATCHLIST_BG_ICON_RIGHT_SPACE                    DP2PX_VALUE(226)
#define WATCHLIST_BG_ICON_BOTTOM_SPACE                   DP2PX_VALUE(64)

#define WATCHLIST_TEXT_LEFT_SPACE                        DP2PX_VALUE(13)

#define WATCHLIST_SMALL_3TEXT_TOP_SPACE                  DP2PX_VALUE(2)
#define WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE               DP2PX_VALUE(28)
#define WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE          DP2PX_VALUE(75)
#define WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE         DP2PX_VALUE(107)
#define WATCHLIST_SMALL_3TEXT_THIRD_WIDGH_SPACE          DP2PX_VALUE(50)
#define WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE           DP2PX_VALUE(2)
#define WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE          (WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE +WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE)
#define WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE           (WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE +WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE)
#define WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE             DP2PX_VALUE(2)

#define WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE         DP2PX_VALUE(60) //DP2PX_VALUE(48)
#define WATCHLIST_ICON_SMALL_WIDGH_HEIGHT_SPACE          DP2PX_VALUE(24)
#define WATCHLIST_ICON_TEXT_LEFT_SPACE                   DP2PX_VALUE(13)
#define WATCHLIST_ICON_TEXT_TOP_SPACE                    DP2PX_VALUE(15)
#define WATCHLIST_ICON_TO_TEXT_SPACE                     DP2PX_VALUE(12)

#define WATCHLIST_SECOND_ICON_LEFT_SPACE                 (WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE*2 + WATCHLIST_TEXT_WIDGH_MIN )
#ifdef ADULT_WATCH_SUPPORT
#define ADULTWATCHLIST_LEFT_SPACE                        DP2PX_VALUE(36)
#define ADULTWATCHLIST_TOP_SPACE                         DP2PX_VALUE(8)
#define ADULTWATCHLIST_RIGHT_SPACE                       DP2PX_VALUE(204)//240-36
#define ADULTWATCHLIST_BOTTOM_SPACE                      DP2PX_VALUE(4)
#define ADULTWATCHLIST_2LINE_TOP_SPACE                   DP2PX_VALUE(10)
#define ADULTWATCHLIST_2LINE_BOTTOM_SPACE                DP2PX_VALUE(10)
#define ADULTWATCHLIST_1LINE_TOP_SPACE                   DP2PX_VALUE(14)
#define ADULTWATCHLIST_1LINE_BOTTOM_SPACE                DP2PX_VALUE(14)
#define ADULTWATCHLIST_3LINE_SENCOND_WORD_LEFT           DP2PX_VALUE(130)

#define ADULTWATCHLIST_FIRST_LINE_HEIGH                  DP2PX_VALUE(20)
#define ADULTWATCHLIST_SECOND_LINE_HEIGH                 DP2PX_VALUE(16)
#define ADULTWATCHLIST_ITEM_OFFESET                      DP2PX_VALUE(4)

#define ADULTWATCHLIST_ICON_HEIGH                        DP2PX_VALUE(36)
#define ADULTWATCHLIST_ICON_AND_TEXT_OFFESET             DP2PX_VALUE(8)
#define ADULTWATCHLIST_ONLY_1ICON_WIDTH                  DP2PX_VALUE(48)
#define ADULTWATCHLIST_1ICON_1STR_1ICON_RIGHTICON_WIDTH  DP2PX_VALUE(32)

#define ADULTWATCHLIST_WIDTH                             DP2PX_VALUE(240)
#define ADULTWATCHLIST_3STR_3LINE_HEIGHT                 DP2PX_VALUE(72)
#define ADULTWATCHLIST_2STR_2LINE_HEIGHT                 DP2PX_VALUE(60)
#define ADULTWATCHLIST_1STR_1LINE_HEIGHT                 DP2PX_VALUE(48)

#define ADULTWATCHLIST_RADIO_LEFT_SPACE                  DP2PX_VALUE(8)//45

#endif
//for appstore, watchface
#define WATCHLIST_ITEM_BIG_HEIGHT                        DP2PX_VALUE(96)
#define WATCHLIST_ICON_LEFT_SPACE                        DP2PX_VALUE(36)
#define WATCHLIST_ICON_TOP_SPACE                         DP2PX_VALUE(12)
#define WATCHLIST_ICON_WIDGH_HEIGHT_SPACE                DP2PX_VALUE(36)
#define WATCHLIST_ICON_TO_TEXT                           DP2PX_VALUE(8)
#define WATCHLIST_TEXT_WIDGH_SPACE                       DP2PX_VALUE(124)
#define WATCHLIST_1TEXT_HEIGHT_SPACE                     DP2PX_VALUE(20)
#define WATCHLIST_2TEXT_HEIGHT_SPACE                     DP2PX_VALUE(18)
#define WATCHLIST_BUTTON_LEFT_SPACE                      DP2PX_VALUE(100)
#define WATCHLIST_BUTTON_TO_BUTTON                       DP2PX_VALUE(8)
#define WATCHLIST_BUTTON_WIDGH_SPACE                     DP2PX_VALUE(48)
#define WATCHLIST_BUTTON_HEIGHT_SPACE                    DP2PX_VALUE(24)
#define WATCHLIST_BUTTON_TOP_SPACE                       DP2PX_VALUE(60)
#define WATCHLIST_TEXT_TOP_SPACE                         DP2PX_VALUE(2)
#define WATCHLIST_TEXT_TO_TEXT                           DP2PX_VALUE(2)
#define WATCHLIST_1STR_1LINE_HEIGHT                      DP2PX_VALUE(48)
#define WATCHLIST_LEFT_SPACE                             DP2PX_VALUE(36)
#define WATCHLIST_ICON_HEIGH                             DP2PX_VALUE(36)
#define WATCHLIST_ICON_AND_TEXT_OFFESET                  DP2PX_VALUE(8)
#define WATCHLIST_1LINE_TOP_SPACE                        DP2PX_VALUE(14)
#define WATCHLIST_RIGHT_SPACE                            DP2PX_VALUE(204)
#define WATCHLIST_1LINE_BOTTOM_SPACE                     DP2PX_VALUE(14)
//============================================================================//
//                                   For watch                                //
//============================================================================//



//============================================================================//
//                           For engineer mode                                //
//============================================================================//
#define MMI_LIST_ITEM_HEIGHT_ENG		        DP2PX_VALUE(55)//list for engineer mode hight


//============================================================================//
//                           For engineer mode                                //
//============================================================================//
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
