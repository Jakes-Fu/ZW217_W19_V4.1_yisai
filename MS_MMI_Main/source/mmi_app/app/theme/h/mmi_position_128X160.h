/*****************************************************************************
** File Name:      mmi_position_128x160.h                                            *
** Author:                                                                   *
** Date:           02/20/2020                                                *
** Copyright:
** Description:    This file is used to describe t9 text input  for    128x160         *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME                   DESCRIPTION                               *
** 02/2020      fangfang.yao              Creat
******************************************************************************/

#ifndef _MMI_POSITION_128X160_H_
#define _MMI_POSITION_128X160_H_ 

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

//the dinamic range of contact_editbox height in sms_mms_in_one window
#ifdef MMI_MESSAGE_CONTACT_NUM_MATCH_SUPPORT
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MIN 0
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX 30
#define MMIMMS_CONTACT_EDITBOX_HEIGHT_MAX_H 30
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM 1
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_EX 2
#define MMIMMS_MATCHEDCONTACT_DDL_MAX_PAGE_NUM_H 1
#endif

#define MMISMS_CHATFORM_SIM_LABEL_WIDTH 48
#define MMISMS_CHATFORM_SIM_DATE_WIDTH  100
#define MMISMS_CHATFORM_ITEM_WIDTH 200
#define MMISMS_CHATFORM_ANIM_WIDTH 40

// status bar height
//fixed by hongjun.jia for Bug 2126 
#define MMI_STATUSBAR_HEIGHT                18//20
//browser
#define     MMIWAP_STATUSBAR_HEIGHT  MMI_STATUSBAR_HEIGHT
//the lcd width and height
#define MMI_MAINSCREEN_WIDTH                128
#define MMI_MAINSCREEN_HEIGHT               160

//font
#define MMI_DEFAULT_FONT_HEIGHT             16
//title
#define MMI_TITLE_HEIGHT                    MMITHEME_GetWinTitleHeight()

#ifdef GUIF_TITLE_PDASTYLE
#define MMI_TITLE_THEME_HEIGHT              MMITHEME_GetThemeTitleHeight_V()
#define MMI_TITLE_THEME_HEIGHT_H            MMITHEME_GetThemeTitleHeight_H()
#else
//fixed by hongjun.jia for bug 2453/2456/2457/2459
#define MMI_TITLE_THEME_HEIGHT              20//24
#define MMI_TITLE_THEME_HEIGHT_H            20//24
#endif

#ifdef MMI_PDA_SUPPORT
/*title button define start*/
#define	MMI_ISTYLE_TITLE_HEIGHT			45
#define	MMI_ISTYLE_TITLE_LEFT_BUTTON_X	10
#define	MMI_ISTYLE_TITLE_LEFT_BUTTON_WIDTH	80

#define	MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH	140
#define	MMI_ISTYLE_TITLE_MIDDLE_BUTTON_X		((MMI_MAINSCREEN_WIDTH-MMI_ISTYLE_TITLE_MIDDLE_BUTTON_WIDTH)>>1)

#define	MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH	32
#define	MMI_ISTYLE_TITLE_RIGHT_BUTTON_X		(MMI_MAINSCREEN_WIDTH-5-MMI_ISTYLE_TITLE_RIGHT_BUTTON_WIDTH)

#define	MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH	55
#define	MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_X		(MMI_MAINSCREEN_WIDTH-5-MMI_ISTYLE_TITLE_LONG_RIGHT_BUTTON_WIDTH)

#define	MMI_ISTYLE_TITLE_CALLLOG_BUTTON_WIDTH		90
#define	MMI_ISTYLE_TITLE_ALL_CALLLOG_BUTTON_X		(MMI_MAINSCREEN_WIDTH/2-90)
#define	MMI_ISTYLE_TITLE_MISS_CALLLOG_BUTTON_X		(MMI_MAINSCREEN_WIDTH/2)

#define	MMI_ISTYLE_TITLE_BUTTON_HEIGHT	32
/*title button define end*/

/*dialpad define start*/
#define	MMI_ISTYLE_DIAL_SIDE_BUTTON_WIDTH				107
#define	MMI_ISTYLE_DIAL_MIDDLE_BUTTON_WIDTH				106

#define	MMI_ISTYLE_DIAL_NUM_BUTTON_HEIGHT				66
#define	MMI_ISTYLE_DIAL_HASH_BUTTON_HEIGHT				67
#define	MMI_ISTYLE_DIAL_CALL_BUTTON_HEIGHT				64
/*dialpad define end*/

#define	MMI_ISTYLE_STATUSBAR_HEIGHT			MMI_STATUSBAR_HEIGHT

#define 	MMI_ISTYLE_LIST_ITEM_HEIGHT		42

#define	MMI_ISTYLE_TAB_HEIGHT				50
#define	MMI_ISTYLE_TAB_HEIGHT_H			50
#endif

#define MMI_DEFAULT_FONT_ASCII_WIDTH        8
//softkey
#define MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT   20//24  //全屏下SOFTKEY栏高度
        
#if defined (MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT) || defined(GUIF_SCROLLKEY)
#define MMI_SCROLLKEY_OFFSET_FROM_BOTTOM     72
#define MMI_FULLSCREEN_SCROLLKEY_BAR_HEIGHT 70
#endif
        
//listbox
#define MMI_LIST_ICON_LEFT_SPACE            2///1
#define MMI_LIST_TEXT_LEFT_SPACE            2///1
#define MMI_LIST_SPLIT_ITEM_HEIGHT          25
#define MMI_LIST_ITEM_HEIGHT		        24  //list窗口中每项list的高度
#define MMI_LIST_HEIGHT_SPACE		        3
#define MMI_LIST_BUTTON_HEIGHT              24
#define MMI_LIST_ITEM_TOP_MARGIN		    1
#define MMI_LIST_ITEM_BOTTOM_MARGIN		    1
#define MMI_LIST_TEXT_LEFT_SPACE_PS         20 //专用于phonesearch设置左边距离
#define MMI_LIST_RIGHT_SPACE                2


#define MMI_LIST_ITEM_HEIGHT_SMALL_FONT     18  //小字体的list高度

#define MMI_LIST_ITEM_RIGHT                 122 //127   //fixed @yanyan.an for CR 210623
#ifdef GUIF_LIST_PDASTYLE
#define MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT    (MMI_LIST_ITEM_HEIGHT+MMI_LIST_ITEM_HEIGHT)
#define MMI_LIST_ITEM_SELECT_ICON_WIDTH     24
#define MMI_LIST_ITEM_BIG_ICON_WIDTH        48
#define MMI_LIST_ICON_MARGIN                10
#else
#define MMI_LIST_DOUBLE_LINE_ITEM_HEIGHT    (MMI_LIST_ITEM_HEIGHT+MMI_LIST_ITEM_HEIGHT)
#define MMI_LIST_ITEM_SELECT_ICON_WIDTH     (MMI_LIST_ITEM_HEIGHT-2)//24
#define MMI_LIST_ITEM_BIG_ICON_WIDTH        (MMI_LIST_ITEM_HEIGHT-2)//MMI_LIST_ITEM_HEIGHT
#define MMI_LIST_ICON_MARGIN                6
#endif

#define MMI_LIST_2ANIM_ICON_WIDTH       (MMI_LIST_ITEM_SELECT_ICON_WIDTH + 4)

// second menu
#define MMI_SECMENU_ITEM_HEIGHT		        MMI_LIST_ITEM_HEIGHT

//idle
#ifdef MMI_RES_ORIENT_PORTRAIT
#define	MMI_IDLE_ICON_AREA_HEIGH	        50//35+16//23
#else
#define	MMI_IDLE_ICON_AREA_HEIGH	        40//32//23
#endif
#define MMI_IDLE_ICON_AREA_UP               0
#define MMI_IDLE_COMMON_NETWORKNAME_ADJUST  10    //基本风格IDLE界面运营商名字坐标调整
//pubwin
#define MMITHEME_PUBWIN_RECT_TOP            125
#define MMITHEME_PUBWIN_RECT_BOTTOM         196

//#define MMITHEME_PUBWIN_PROMPT_RECT             {0,MMITHEME_PUBWIN_RECT_TOP,78,MMITHEME_PUBWIN_RECT_BOTTOM}
#define MMITHEME_PUBWIN_BG_RECT                 {0,MMITHEME_PUBWIN_RECT_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_BOTTOM}
#define MMITHEME_PUBWIN_UP_BG_RECT              {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_TOP - 1}

#define MMITHEME_PUBWIN_TEXT_RECT              {0,MMITHEME_PUBWIN_RECT_TOP + 1,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_BOTTOM}

#define MMITHEME_PUBWIN_TEXT1_RECT              {49,141,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,161}
#define MMITHEME_PUBWIN_TEXT2_RECT              {49,162,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,182}

#define MMITHEME_PUBWIN_3L_TEXT1_RECT           {49,132,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,151}
#define MMITHEME_PUBWIN_3L_TEXT2_RECT           {49,152,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,171}
#define MMITHEME_PUBWIN_3L_TEXT3_RECT           {49,172,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,191}

#define MMITHEME_PUBWIN_IMAGE_DISP_RECT          {0,142,37,177}  // 需要修改坐标

#define MMITHEME_PUBWIN_WAIT_RECT               {0,MMITHEME_PUBWIN_RECT_TOP,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_BOTTOM}
#define MMITHEME_PUBWIN_ANIM_DISP_RECT          {0,MMITHEME_PUBWIN_RECT_TOP + 1,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_BOTTOM}  // 需要修改坐标
#define MMITHEME_PUBWIN_WAIT_BG_RECT            {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMITHEME_PUBWIN_RECT_TOP - 1}
#define MMITHEME_PUBWIN_WAIT_TEXT1_RECT         {10,30,165,54}
#define MMITHEME_PUBWIN_WAIT_TEXT2_RECT         {10,55,165,79}
#define MMITHEME_PUBWIN_WAIT_TEXT3_RECT         {10,80,165,104}
#define MMITHEME_PUBWIN_WAIT_CC_TEXT_RIGHT      100

//keylock
#define MINUTE_LEG_R_1		42
#define MINUTE_LEG_R_2		7
#define MINUTE_LEG_WIDTH	3
#define HOUR_LEG_R_1		25
#define HOUR_LEG_R_2		5
#define HOUR_LEG_WIDTH		5
//for gsensor compiling error
#define MINUTE_LEG_R_1_H	38
#define MINUTE_LEG_R_2_H	8
#define HOUR_LEG_R_1_H		28
#define HOUR_LEG_R_2_H		6
//endif

#define CIRCLE_CENTER_POINT	        {87, -59}
#define CIRCLE_CENTER_POINT_H_X 	160
#define CIRCLE_CENTER_POINT_H_Y	    -65


#define MMI_KEYLOCK_SHOW_ICON_MAX           3           //锁屏未处理信息显示图标的最大个数
#define MMI_KEYLOCK_SHOW_TOP                120         //锁屏未处理信息显示的TOP
#define MMI_KEYLOCK_SHOW_ICON_HEIGHT        16          //锁屏未处理信息图标的高度
#define MMI_KEYLOCK_SHOW_BOTTOM             MMI_KEYLOCK_SHOW_TOP + MMI_KEYLOCK_SHOW_ICON_HEIGHT         //锁屏未处理信息显示的BOTTOM
#define MMI_KEYLOCK_SHOW_ICON_WIDTH         16          //锁屏未处理信息图标的宽度
#define MMI_KEYLOCK_SHOW_EDGE               10          //锁屏未处理信息屏幕两边的间距
#define MMI_KEYLOCK_SHOW_SPACE              1           //锁屏未处理信息图标与文字间的间隔
#define MMI_KEYLOCK_SHOW_TEXT_WIDTH         32          //锁屏未处理信息文字显示的宽度
#define MMI_KEYLOCK_SHOW_GAP                4
#define MMI_KEYLOCK_ICON1_RECT               {MMI_KEYLOCK_SHOW_EDGE,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_TEXT1_RECT               {MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_ICON2_RECT               {MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP+1,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_TEXT2_RECT               {MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_ICON3_RECT               {MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_TEXT3_RECT               {MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP,MMI_KEYLOCK_SHOW_EDGE+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*3+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM}

//gsensor
#define MMI_KEYLOCK_SHOW_TOP_H              126     //锁屏未处理信息显示的TOP
#define MMI_KEYLOCK_SHOW_BOTTOM_H             MMI_KEYLOCK_SHOW_TOP_H + MMI_KEYLOCK_SHOW_ICON_HEIGHT         //锁屏未处理信息显示的BOTTOM
#define MMI_KEYLOCK_SHOW_EDGE_H             80          //锁屏未处理信息屏幕两边的间距

#define MMI_KEYLOCK_ICON1_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH,MMI_KEYLOCK_SHOW_BOTTOM}
#define MMI_KEYLOCK_TEXT1_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1,MMI_KEYLOCK_SHOW_BOTTOM_H}
#define MMI_KEYLOCK_ICON2_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*1+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*1+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP+1,MMI_KEYLOCK_SHOW_BOTTOM_H}
#define MMI_KEYLOCK_TEXT2_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*1+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*1,MMI_KEYLOCK_SHOW_BOTTOM_H}
#define MMI_KEYLOCK_ICON3_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*2+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*2+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM_H}
#define MMI_KEYLOCK_TEXT3_RECT_H             {MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*2+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_TOP_H,MMI_KEYLOCK_SHOW_EDGE_H+MMI_KEYLOCK_SHOW_ICON_WIDTH*3+MMI_KEYLOCK_SHOW_SPACE*3+MMI_KEYLOCK_SHOW_TEXT_WIDTH*3+MMI_KEYLOCK_SHOW_GAP*2,MMI_KEYLOCK_SHOW_BOTTOM_H}

#ifdef TOUCH_PANEL_SUPPORT

//以下带_H表示横屏时位置，反之为竖屏位置
//search list 
#define MMI_DIAL_SEARCHLIST_RECT_HEIGHT 66
#define MMI_DIAL_SEARCHLIST_RECT_HEIGHT_H 28

//dialing window key position

//dialing form
/*除了del按键和edit控件，其他button构成一个form,此处为该form的rect*/
#define MMI_DIAL_FORM_TOP                   110
#define MMI_DIAL_FORM_TOP_H                   84

/*该处设置各button之间的间隔*/
#define MMI_DIAL_FORM_VER_SPACE           		1
#define MMI_DIAL_FORM_HOR_SPACE           		0
#define MMI_DIAL_FORM_VER_SPACE_H           		1
#define MMI_DIAL_FORM_HOR_SPACE_H           		3

//option dial form 
#define MMI_DIAL_OPTION_BUTTON_FORM_LEFT 4
#define MMI_DIAL_OPTION_BUTTON_FORM_RIGHT 171
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP 173
#define MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM 194

#define MMI_DIAL_OPTION_BUTTON_FORM_LEFT_H 174
#define MMI_DIAL_OPTION_BUTTON_FORM_RIGHT_H 215
#define MMI_DIAL_OPTION_BUTTON_FORM_TOP_H 62
#define MMI_DIAL_OPTION_BUTTON_FORM_BOTTOM_H 150

//拨号盘数字button『收起』时list和button form之间的距离
#define MMI_DIAL_SPACE_LIST_AND_OPTIONFORM  5
#define MMI_DIAL_SPACE_LIST_AND_OPTIONFORM_H 5
//拨号盘数字button『展开』时list和button form之间的距离
#define MMI_DIAL_SPACE_LIST_AND_NUMFORM 5
#define MMI_DIAL_SPACE_LIST_AND_NUMFORM_H 2


#define MMI_DIAL_TOOLBAR_BUTTON_WIDTH  42


#define MMI_DIAL_OPTION_BUTTON_LIST_BOTTOM_H   146

#define MMI_DIAL_TOOLBAR_BUTTON_FORM_LEFT_H 174
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_TOP_H 62
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_RIGHT_H 215
#define MMI_DIAL_TOOLBAR_BUTTON_FORM_BOTTOM_H 82

#define MMIIDLE_TOOLBAR_START_POINT_X 20
#define MMIIDLE_TOOLBAR_START_POINT_Y  290

#define MMIIDLE_TOOLBAR_START_POINT_X_H 58
#define MMIIDLE_TOOLBAR_START_POINT_Y_H  100

#endif
        
//tv
#define MMITVOUT_REMINDER_TEXT_POS_LEFT			20		
#define MMITVOUT_REMINDER_TEXT_POS_TOP			150
#define MMITVOUT_REMINDER_TEXT_POS_RIGHT		156
#define MMITVOUT_REMINDER_TEXT_POS_BUTTOM		185

#ifdef TOUCH_PANEL_SUPPORT
//scroll bar
#define SCROLL_BAR_WIDTH					16      //菜单右侧滚动条的宽度
#else
#define SCROLL_BAR_WIDTH					3      //菜单右侧滚动条的宽度
#endif

//softkey icon
#define MMI_SOFTKEYBAR_FIRST_ICON_POS           50          // 第一个图标的位置
#define MMI_SOFTKEYBAR_SECOND_ICON_POS          80         // 第二个图标的位置
#define MMI_SOFTKEYBAR_THIRD_ICON_POS           110         // 第三个图标的位置

#if defined(MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT) ||defined(GUIF_SCROLLKEY)
#define MMI_SCROLLKEYBAR_FIRST_ICON_POS         50          // 第一个图标的位置
#define MMI_SCROLLKEYBAR_SECOND_ICON_POS        110         // 第二个图标的位置
#endif

//输入法spell、select条参数
#define MMI_SPELL_RECT_HEIGHT			19//15  //拼音框高度
#define MMI_SELECT_RECT_HEIGHT			19//15  //汉字候选框高度，拼音、笔划、手写模式时用到

// TAB控件
//fixed by hongjun.jia for bugzilla 2451/2452
#define	MMI_TAB_HEIGHT				40//46//53
#define	MMI_TAB_HEIGHT_H			40//46//53

#define	MMI_TAB_BOTTOM				(MMITHEME_GetTabHeight() - 1)//53
// top value of child window.  
#define		MMI_TAB_CHILD_WINDOW_TOP	MMI_TAB_BOTTOM + 1





//#ifndef MMI_FOUR_HW_ICON
#if (MMI_HARDWARE_ICON_MAX == 2 )
#define MMI_HARDWARE_ICON_1_LEFT         0
#define MMI_HARDWARE_ICON_1_RIGHT        (MMI_MAINSCREEN_WIDTH/2)
#define MMI_HARDWARE_ICON_2_LEFT         (MMI_HARDWARE_ICON_1_RIGHT+1)
#define MMI_HARDWARE_ICON_2_RIGHT         MMI_MAINSCREEN_WIDTH
#elif (MMI_HARDWARE_ICON_MAX != 4 )
#define MMI_HARDWARE_ICON_1_LEFT         13
#define MMI_HARDWARE_ICON_1_RIGHT        38

#define MMI_HARDWARE_ICON_2_LEFT          48
#define MMI_HARDWARE_ICON_2_RIGHT         73

#define MMI_HARDWARE_ICON_3_LEFT          83
#define MMI_HARDWARE_ICON_3_RIGHT         108

#define MMI_HARDWARE_ICON_4_LEFT          118
#define MMI_HARDWARE_ICON_4_RIGHT         133

#define MMI_HARDWARE_ICON_5_LEFT          143
#define MMI_HARDWARE_ICON_5_RIGHT         168
#else
#define MMI_HARDWARE_ICON_1_LEFT         13
#define MMI_HARDWARE_ICON_1_RIGHT        38

#define MMI_HARDWARE_ICON_2_LEFT          48
#define MMI_HARDWARE_ICON_2_RIGHT         73

#define MMI_HARDWARE_ICON_3_LEFT          83
#define MMI_HARDWARE_ICON_3_RIGHT         108

#define MMI_HARDWARE_ICON_4_LEFT          118
#define MMI_HARDWARE_ICON_4_RIGHT         133

#define MMI_HARDWARE_ICON_5_LEFT          143
#define MMI_HARDWARE_ICON_5_RIGHT         168
#endif

#ifdef MMI_RES_ORIENT_PORTRAIT
#define MMI_MAIN_TIME_0_X           0+44
#define MMI_MAIN_TIME_1_X           10+44
#define MMI_MAIN_TIME_2_X           25+44//30
#define MMI_MAIN_TIME_3_X           35+44//40
#define MMI_MAIN_TIME_H             20//26+44
#define MMI_MAIN_TIME_COLON_X       20+44
#define MMI_MAIN_TIME_AM_PM_X       45+44//50
#else
#define MMI_MAIN_TIME_0_X           0+59
#define MMI_MAIN_TIME_1_X           10+59
#define MMI_MAIN_TIME_2_X           25+59//30
#define MMI_MAIN_TIME_3_X           35+59//40
#define MMI_MAIN_TIME_H             20//26+59
#define MMI_MAIN_TIME_COLON_X       20+59
#define MMI_MAIN_TIME_AM_PM_X       45+59//50
#endif
#ifndef DCD_SUPPORT
#define MMI_MAIN_TIME_Y             22
#define MMI_MAIN_TIME_COLON_Y       26
#define MMI_MAIN_TIME_AM_PM_Y       26
//gsensor compliling error
#define IDLE_DATE_TOP_H 			32
#define IDLE_WEEK_TOP_H 		    IDLE_DATE_TOP_H
#define IDLE_DATE_LEFT_H          160    
#define IDLE_WEEK_LEFT_H          270
#define MMI_MAIN_TIME_Y_H           20
#define MMI_MAIN_TIME_COLON_Y_H     24
#define MMI_MAIN_TIME_AM_PM_Y_H       24
//end
#else
#define MMI_MAIN_TIME_Y             29
#define MMI_MAIN_TIME_COLON_Y       33
#define MMI_MAIN_TIME_AM_PM_Y       33
//gsensor compliling error
#define MMI_MAIN_TIME_Y_H           184
#define MMI_MAIN_TIME_COLON_Y_H     MMI_MAIN_TIME_Y_H + 5
#define MMI_MAIN_TIME_AM_PM_Y_H       33
#define IDLE_DATE_TOP_H 			188
#define IDLE_WEEK_TOP_H 		    IDLE_DATE_TOP_H
#define IDLE_DATE_LEFT_H          160    
#define IDLE_WEEK_LEFT_H          270
//end
#endif
//pub win
#define MMITHEME_PUBWIN_4WAIT_RECT				{10,22,165,22+MMI_DEFAULT_FONT_HEIGHT}
#define MMITHEME_PUBWIN_4WAIT_OFFSET			3
#define MMITHEME_PUBWIN_4WAIT_OFFSET_EX         13 

#define MMI_KEYLOCK_BACKGROUND_START_LEFT       15		
#define MMI_KEYLOCK_SLIDE_WIDTH					21
#define MMI_KEYLOCK_CUE_LEFT					12
#define MMI_KEYLOCK_CUE_TOP						173
#define MMI_KEYLOCK_CUE_RIGHT					MMI_MAINSCREEN_WIDTH - 10
#define MMI_KEYLOCK_CUE_BOTTOM					193


//#define MMITHEME_PUBWIN_CC_WAIT_BG_RECT {0, 0, 175,125}
// #define MMITHEME_PUBWIN_CC_WAIT_RECT {0, 125, 175, 200}
#define MMITHEME_PUBWIN_CC_CONNECT_ANIM_RECT {76, 149, 100, 173}
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_RECT {107, 126, 174, 193} 
//width and height必需和rect保持一致,否则导致浏览图片时来电Assert
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_WIDTH    60   
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_HEIGHT   65
#define MMITHEME_PUBWIN_CC_PROMP_IMAGE_HEIGHT_H 55

#define MMITHEME_PUBWIN_WAIT_CC_ANIM_TOP 155
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_BOTTOM 170
#define MMITHEME_PUBWIN_WAIT_CC_ANIM_RIGHT 100

//progress bar 
#define MMIPROGRESS_BAR_GROOVE_LEFT  45
#define MMIPROGRESS_BAR_LEFT         46
#define MMIPROGRESS_BAR_WIDTH        84

//iconlist 176*220
#ifdef GUIF_ICONLIST_DELIMITER
#define MMITHEME_DELIMITER_WIDTH            175     //分隔符宽度
#define MMITHEME_DELIMITER_HEIGHT           30      //分隔符高度
#define MMITHEME_DELIMITER_STR_LEFT_SPACE   10      //分隔符内字符的左留白的大小(默认左对齐)
#define MMITHEME_DELIMITER_BG_LEFT_SPACE    0       //分隔符较大，分隔符背景较小的某些特例情况
#define MMITHEME_DELIMITER_BG_WIDTH         0
#define MMITHEME_DELIMITER_BG_TOP_SPACE     0
#define MMITHEME_DELIMITER_BG_HEIGHT        0
#endif

#ifdef GUIF_TITLE_PDASTYLE
#define MMI_TITLE_TEXT_MARGIN_PDA           5
#define MMI_TITLE_THEME_HEIGHT_PDA          25
#define MMI_TITLE_THEME_HEIGHT_H_PDA        25
#define MMI_TITLE_THEME_HEIGHT_ISTYLE_PDA          45
#define MMI_TITLE_THEME_HEIGHT_H_ISTYLE_PDA        45
#endif

#define MMI_TAB_ITEM_TEXT_BOTTOM_SPACE      3

#define MMI_TAB_CARD_DEFAULT_WIDTH          24

#ifdef GUITAB_PDASTYLE_SUPPORT
#define MMI_TAB_DISP_MAX_ITEM_NUM           4
#define MMI_TAB_DISP_MAX_ITEM_NUM_H         4
#define MMI_TAB_DISP_MAX_ITEM_NUM_ISTYLE           5
#define MMI_TAB_ITEM_DEFAULT_HEIGHT         64
#define MMI_TAB_ITEM_DEFAULT_WIDTH          44
#define MMI_TAB_ITEM_DEFAULT_WIDTH_H        55
#define MMI_TAB_ITEM_DEFAULT_WIDTH_ISTYLE          35
#define MMI_TAB_PRG_HEIGHT				    4
#else
#define MMI_TAB_DISP_MAX_ITEM_NUM           4
#define MMI_TAB_DISP_MAX_ITEM_NUM_H         4
#define MMI_TAB_ITEM_DEFAULT_HEIGHT         22
#define MMI_TAB_ITEM_DEFAULT_WIDTH          32
#define MMI_TAB_ITEM_DEFAULT_WIDTH_H        43
#define MMI_TAB_PRG_HEIGHT				    4
#endif

//  soft key 文字距离水平和垂直方向的距离
#define MMI_SOFTKEY_TEXT_HOR_SPACE  2

#define MMI_NUMBER_EDIT_TOP_MARGIN    1
#define MMI_NUMBER_EDIT_HEIGHT    25
//password
#define MMI_PASSWORD_EDIT_LEFT      20
#define MMI_PASSWORD_EDIT_TOP       48
#define MMI_PASSWORD_EDIT_RIGHT     (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMI_PASSWORD_EDIT_LEFT)
#define MMI_PASSWORD_EDIT_BOTTOM    (MMI_PASSWORD_EDIT_TOP + 30)
#define MMI_PASSWORD_IMAGE_LEFT         MMI_TIME_IMAGE_LEFT
#define MMI_PASSWORD_IMAGE_TOP          MMI_TIME_IMAGE_TOP
#define MMI_PASSWORD_EDIT_TOP_MARGIN    8
#define MMI_PASSWORD_EDIT_TOP_MARGIN_H    1     //横屏时，输入法虚拟键盘打开edit调整后，高度不够，引起assert
#define MMI_PASSWORD_EDIT_HEIGHT    25

//time/date
#define MMI_TIME_EDIT_LEFT          5
#define MMI_TIME_EDIT_TOP           48
#define MMI_TIME_EDIT_RIGHT         (MMI_MAINSCREEN_RIGHT_MAX_PIXEL - MMI_TIME_EDIT_LEFT)
#define MMI_TIME_EDIT_BOTTOM        (MMI_TIME_EDIT_TOP + 40)
#define MMI_TIME_IMAGE_LEFT         48
#define MMI_TIME_IMAGE_TOP          90
#define MMI_DATE_EDIT_LEFT          MMI_TIME_EDIT_LEFT//176*220的DATE控件需要调整坐标
#define MMI_DATE_EDIT_TOP           MMI_TIME_EDIT_TOP
#define MMI_DATE_EDIT_RIGHT         MMI_TIME_EDIT_RIGHT
#define MMI_DATE_EDIT_BOTTOM        MMI_TIME_EDIT_BOTTOM

//fixed by hongjun.jia for bugzilla 2461-2464
#define MMI_TITLE_TEXT_L_MARGIN	            10//35

//title
#define MMI_TITLE_ARROW_MARGIN	        3
#define MMI_TITLE_ARROW_RECT_WIDTH      20 
#define MMI_TITLE_ISTYLE_MENU_ICON_MARGIN	 3

#define MMI_TITLE_LEFT_ARROW_WIDTH      20
#define MMI_TITLE_LEFT_ARROW_HEIGHT     18
#define MMI_TITLE_RIGHT_ARROW_WIDTH     20
#define MMI_TITLE_RIGHT_ARROW_HEIGHT    18

#define MMI_TITLE_LEFT_ARROW_LEFT       3
#define MMI_TITLE_LEFT_ARROW_TOP        1
#define MMI_TITLE_LEFT_ARROW_RIGHT          ( MMI_TITLE_LEFT_ARROW_LEFT + MMI_TITLE_LEFT_ARROW_WIDTH - 1)
#define MMI_TITLE_LEFT_ARROW_BOTTOM      (MMI_TITLE_LEFT_ARROW_TOP+MMI_TITLE_LEFT_ARROW_HEIGHT-1)

#define MMI_TITLE_RIGHT_ARROW_LEFT		(MMI_MAINSCREEN_WIDTH - MMI_TITLE_LEFT_ARROW_LEFT -MMI_TITLE_RIGHT_ARROW_WIDTH)
#define MMI_TITLE_RIGHT_ARROW_TOP       MMI_TITLE_LEFT_ARROW_TOP
#define MMI_TITLE_RIGHT_ARROW_RIGHT          ( MMI_TITLE_RIGHT_ARROW_LEFT + MMI_TITLE_RIGHT_ARROW_WIDTH - 1)
#define MMI_TITLE_RIGHT_ARROW_BOTTOM      (MMI_TITLE_RIGHT_ARROW_TOP+MMI_TITLE_RIGHT_ARROW_HEIGHT-1)

// tips position info
#define MMITIPS_RECT_MIN_HEIGHT                22   // 默认高度
#define MMITIPS_RECT_MAX_WIDTH                 155  // 默认宽度

#ifdef MMI_RES_ORIENT_PORTRAIT
#define MMI_COMMOM_IDLE_STK_TEXT_Y   121
#else
#define MMI_COMMOM_IDLE_STK_TEXT_Y   96
#endif

#ifdef MENU_CRYSTALCUBE_SUPPORT
    #define MMI_CRYSTALCUBE_REGION_WIDTH           120//192
    #define MMI_CRYSTALCUBE_REGION_HEIGHT          140//224
#endif

    #define MMI_CUBEPAGE_PAGE_RECT          {24, 61, 24+MMI_CRYSTALCUBE_REGION_WIDTH-1, 61+MMI_CRYSTALCUBE_REGION_HEIGHT-1}//jinx

//============================================================================//
//                                   For watch                                //
//============================================================================//

#define WATCHLIST_ITEM_HEIGHT_SMALL                      30
#define WATCHLIST_ITEM_HEIGHT                            80
#define WATCHLIST_TEXT_WIDGH_BIG                         214
#define WATCHLIST_TEXT_WIDGH_MID                         155
#define WATCHLIST_TEXT_WIDGH_SMALL                       126
#define WATCHLIST_TEXT_WIDGH_MIN                         100

#define WATCHLIST_BG_ICON_LEFT_SPACE                     15
#define WATCHLIST_BG_ICON_TOP_SPACE                      13
#define WATCHLIST_BG_ICON_RIGHT_SPACE                    226
#define WATCHLIST_BG_ICON_BOTTOM_SPACE                   64

#define WATCHLIST_TEXT_LEFT_SPACE                        13

#define WATCHLIST_SMALL_3TEXT_TOP_SPACE                  2
#define WATCHLIST_SMALL_3TEXT_BOTTOM_SPACE               28
#define WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE          75
#define WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE         107
#define WATCHLIST_SMALL_3TEXT_THIRD_WIDGH_SPACE          50
#define WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE           2
#define WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE          (WATCHLIST_SMALL_3TEXT_FIRST_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_FIRST_WIDGH_SPACE +WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE)
#define WATCHLIST_SMALL_3TEXT_THIRD_LEFT_SPACE           (WATCHLIST_SMALL_3TEXT_SECOND_LEFT_SPACE + WATCHLIST_SMALL_3TEXT_SECOND_WIDGH_SPACE +WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE)
#define WATCHLIST_SMALL_3TEXT_INTERVAL_SPACE             2

#define WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE         48
#define WATCHLIST_ICON_SMALL_WIDGH_HEIGHT_SPACE          24
#define WATCHLIST_ICON_TEXT_LEFT_SPACE                   13
#define WATCHLIST_ICON_TEXT_TOP_SPACE                    15
#define WATCHLIST_ICON_TO_TEXT_SPACE                     12

#define WATCHLIST_SECOND_ICON_LEFT_SPACE                 (WATCHLIST_ICON_TEXT_LEFT_SPACE + WATCHLIST_ICON_NORMAL_WIDGH_HEIGHT_SPACE + WATCHLIST_ICON_TO_TEXT_SPACE*2 + WATCHLIST_TEXT_WIDGH_MIN )

//============================================================================//
//                                   For watch                                //
//============================================================================//


//============================================================================//
//                           For engineer mode                                //
//============================================================================//
#define MMI_LIST_ITEM_HEIGHT_ENG		        (55)//list for engineer mode hight


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
