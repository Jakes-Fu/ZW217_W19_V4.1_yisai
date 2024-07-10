/*****************************************************************************
** File Name:      mmi_keylock_position.h                                             *
** Author:         Yintang.Ren                                                 *
** Date:           15/08/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe kelock interface            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       Yintang.Ren	       Create
******************************************************************************/

#ifndef _MMI_KEYLOCK_POSITION_H_
#define _MMI_KEYLOCK_POSITION_H_

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
		
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef MMI_KEY_LOCK_EFFECT_SUPPORT

#ifdef MMI_KEY_LOCK_EFFECT_TYPE1_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR    20
//modify by michael wang for NEWMS00167310
#define KEY_LOCK_EFFECT1_ANIM_POS_Y             115
#define KEY_LOCK_EFFECT1_SIM_ICON_INTERVAL  4
#define KEY_LOCK_EFFECT1_TIME_OFFSET_Y         10
#elif defined MAINLCD_SIZE_240X400
#define KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR    15
#define KEY_LOCK_EFFECT1_ANIM_POS_Y             115
#define KEY_LOCK_EFFECT1_SIM_ICON_INTERVAL  4
#define KEY_LOCK_EFFECT1_TIME_OFFSET_Y         10
#elif defined MAINLCD_SIZE_320X480
#define KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR    20
#define KEY_LOCK_EFFECT1_ANIM_POS_Y             150
#define KEY_LOCK_EFFECT1_SIM_ICON_INTERVAL  4
#define KEY_LOCK_EFFECT1_TIME_OFFSET_Y         20
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define KEY_LOCK_EFFECT1_ANIM_MARGIN_HOR    10
#define KEY_LOCK_EFFECT1_ANIM_POS_Y             75
#define KEY_LOCK_EFFECT1_SIM_ICON_INTERVAL  4
#define KEY_LOCK_EFFECT1_TIME_OFFSET_Y         8
#else
#error "Don't support this LCD pixel"
#endif

#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_TYPE2_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define MMIKL_TOP_OFFSET_FROM_STATUS_BAR  5
#define MMIKL_DATE_WEEK_LABEL_HEIGHT      20
#elif defined MAINLCD_SIZE_240X400
#define MMIKL_TOP_OFFSET_FROM_STATUS_BAR  5
#define MMIKL_DATE_WEEK_LABEL_HEIGHT      20
#elif defined MAINLCD_SIZE_320X480
#define MMIKL_TOP_OFFSET_FROM_STATUS_BAR  40
#define MMIKL_DATE_WEEK_LABEL_HEIGHT      30
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIKL_TOP_OFFSET_FROM_STATUS_BAR  5
#define MMIKL_DATE_WEEK_LABEL_HEIGHT      15
#else
#error "Don't support this LCD pixel"
#endif

#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_TYPE3_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN                      10
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN                 32
#define KEY_LOCK_EFFECT3_TIME_UP_MARGIN                            16
#elif defined MAINLCD_SIZE_240X400
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN                       0
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN                 14
#define KEY_LOCK_EFFECT3_TIME_UP_MARGIN                                       14
#elif defined MAINLCD_SIZE_320X480
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN                      10
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN                 32
#define KEY_LOCK_EFFECT3_TIME_UP_MARGIN                                       26
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_UP_MARGIN                      10
#define KEY_LOCK_EFFECT3_MOVE_BAR_IMG_DOWN_MARGIN                 15
#define KEY_LOCK_EFFECT3_TIME_UP_MARGIN                                       15
#else
#error "Don't support this LCD pixel"
#endif
    
#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_TYPE4_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y            245
#define KEY_LOCK_EFFECT4_SLIDE_BG_UP_MARGIN     18
#define KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN  12
#define KEY_LOCK_EFFECT4_TIME_UP_MARGIN              12
#define KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN 58
#elif defined MAINLCD_SIZE_240X400
#define KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y            325
#define KEY_LOCK_EFFECT4_SLIDE_BG_UP_MARGIN     18
#define KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN  12
#define KEY_LOCK_EFFECT4_TIME_UP_MARGIN              15
#define KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN 58
#elif defined MAINLCD_SIZE_320X480
#define KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y            384
#define KEY_LOCK_EFFECT4_SLIDE_BG_UP_MARGIN     24
#define KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN  21
#define KEY_LOCK_EFFECT4_TIME_UP_MARGIN              19
//#define KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN  87
#define KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN 87
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y            190
#define KEY_LOCK_EFFECT4_SLIDE_BG_UP_MARGIN     15
#define KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN  10
#define KEY_LOCK_EFFECT4_TIME_UP_MARGIN              10
#define KEY_LOCK_EFFECT4_SLIDE_BG_ADD_DC_RIGHT_MARGIN 87
#else
#error "Don't support this LCD pixel"
#endif
#define KEY_LOCK_EFFECT4_SLIDE_BG_RIGHT_MARGIN KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN
#define KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_X      KEY_LOCK_EFFECT4_SLIDE_BG_LEFT_MARGIN
#define KEY_LOCK_EFFECT4_SLIDE_BLOCK_POS_Y      (KEY_LOCK_EFFECT4_SLIDE_BG_POS_Y + KEY_LOCK_EFFECT4_SLIDE_BG_UP_MARGIN)
#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_TYPE5_SUPPORT

#if defined(MAINLCD_SIZE_240X320)
#define KL_PATTERN_TITLE_HEIGHT   78
#define KL_PATTERN_TITLE_LINE_HEIGHT   40+MMI_STATUSBAR_HEIGHT
#define KL_PATTERN_TIPS_HEIGHT     40
#define KL_PATTERN_TITLE_EVENT_HEIGHT 40+MMI_STATUSBAR_HEIGHT
#define KL_PATTERN_TIME_RECT   {16, MMI_STATUSBAR_HEIGHT, 180, MMI_STATUSBAR_HEIGHT+24}
#define KL_PATTERN_DATE_RECT   {16, MMI_STATUSBAR_HEIGHT+24, 120, MMI_STATUSBAR_HEIGHT+40}
#define KL_PATTERN_WEEK_RECT  {80, MMI_STATUSBAR_HEIGHT+24, 140, MMI_STATUSBAR_HEIGHT+40}

#define MMI_KL_PASSWARD_POINT_WIDTH   66             //两个点的距离
#define SETTING_LOCK_DOT_Y_OFFSET  10                   //设置图形锁界面时点在Y轴上的偏移
#define UNLOCK_DOT_Y_OFFSET  20                                //解锁图形界面时点在Y轴上的偏移
#elif defined(MAINLCD_SIZE_240X400)
#define KL_PATTERN_TITLE_HEIGHT   110                                                       //Title 高度位置
#define KL_PATTERN_TITLE_LINE_HEIGHT   40+MMI_STATUSBAR_HEIGHT       //Title直线的高度位置
#define KL_PATTERN_TIPS_HEIGHT     50                                                        //Tips高度位置
#define KL_PATTERN_TITLE_EVENT_HEIGHT   90                                            //事件显示高度位置
#define KL_PATTERN_TIME_RECT   {16, MMI_STATUSBAR_HEIGHT, 180, MMI_STATUSBAR_HEIGHT+24}
#define KL_PATTERN_DATE_RECT   {16, MMI_STATUSBAR_HEIGHT+24, 120, MMI_STATUSBAR_HEIGHT+40}
#define KL_PATTERN_WEEK_RECT  {80, MMI_STATUSBAR_HEIGHT+24, 140, MMI_STATUSBAR_HEIGHT+40}

#define MMI_KL_PASSWARD_POINT_WIDTH   75
#define SETTING_LOCK_DOT_Y_OFFSET  0
#define UNLOCK_DOT_Y_OFFSET  32
#elif defined(MAINLCD_SIZE_320X480)
#define KL_PATTERN_TITLE_HEIGHT   120
#define KL_PATTERN_TITLE_LINE_HEIGHT  (45+MMI_STATUSBAR_HEIGHT)
#define KL_PATTERN_TIPS_HEIGHT     50
#define KL_PATTERN_TITLE_EVENT_HEIGHT   90
#define KL_PATTERN_TIME_RECT   {16, MMI_STATUSBAR_HEIGHT, 180, MMI_STATUSBAR_HEIGHT+24}
#define KL_PATTERN_DATE_RECT   {16, MMI_STATUSBAR_HEIGHT+24, 120, MMI_STATUSBAR_HEIGHT+40}
#define KL_PATTERN_WEEK_RECT  {80, MMI_STATUSBAR_HEIGHT+24, 140, MMI_STATUSBAR_HEIGHT+40}

#define MMI_KL_PASSWARD_POINT_WIDTH   95
#define SETTING_LOCK_DOT_Y_OFFSET  0
#define UNLOCK_DOT_Y_OFFSET  35
#else
#define KL_PATTERN_TITLE_HEIGHT   78
#define KL_PATTERN_TITLE_LINE_HEIGHT   40+MMI_STATUSBAR_HEIGHT
#define KL_PATTERN_TIPS_HEIGHT     40
#define KL_PATTERN_TITLE_EVENT_HEIGHT 40+MMI_STATUSBAR_HEIGHT
#define KL_PATTERN_TIME_RECT   {16, MMI_STATUSBAR_HEIGHT, 180, MMI_STATUSBAR_HEIGHT+24}
#define KL_PATTERN_DATE_RECT   {16, MMI_STATUSBAR_HEIGHT+24, 120, MMI_STATUSBAR_HEIGHT+40}
#define KL_PATTERN_WEEK_RECT  {80, MMI_STATUSBAR_HEIGHT+24, 140, MMI_STATUSBAR_HEIGHT+40}

#define MMI_KL_PASSWARD_POINT_WIDTH   66
#define SETTING_LOCK_DOT_Y_OFFSET  10
#define UNLOCK_DOT_Y_OFFSET  20
#endif

#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_TYPE6_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define KEY_LOCK_EFFECT6_TARGET_WIDTH               60
#define KEY_LOCK_EFFECT6_TARGET_OFFSET              14
#define KEY_LOCK_EFFECT6_TARGET_RISE                  40

#define KEY_LOCK_EFFECT6_PANEL_Y                        200
#define KEY_LOCK_EFFECT6_PANEL_HEIGHT               50

#define KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT       20
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_UP          5
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_DOWN     5
#elif defined MAINLCD_SIZE_240X400
#define KEY_LOCK_EFFECT6_TARGET_WIDTH               60
#define KEY_LOCK_EFFECT6_TARGET_OFFSET              14
#define KEY_LOCK_EFFECT6_TARGET_RISE                  40

#define KEY_LOCK_EFFECT6_PANEL_Y                        250
#define KEY_LOCK_EFFECT6_PANEL_HEIGHT               60

#define KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT       10
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_UP          5
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_DOWN     5
#elif defined MAINLCD_SIZE_320X480
#define KEY_LOCK_EFFECT6_TARGET_WIDTH               80
#define KEY_LOCK_EFFECT6_TARGET_OFFSET              20
#define KEY_LOCK_EFFECT6_TARGET_RISE                  50

#define KEY_LOCK_EFFECT6_PANEL_Y                        300
#define KEY_LOCK_EFFECT6_PANEL_HEIGHT               70

#define KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT       20
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_UP          5
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_DOWN     5
#elif defined MAINLCD_SIZE_176X220
#define KEY_LOCK_EFFECT6_TARGET_WIDTH               40
#define KEY_LOCK_EFFECT6_TARGET_OFFSET              10
#define KEY_LOCK_EFFECT6_TARGET_RISE                  25

#define KEY_LOCK_EFFECT6_PANEL_Y                        150
#define KEY_LOCK_EFFECT6_PANEL_HEIGHT               35

#define KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT       10
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_UP          5
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_DOWN     5
#elif defined MAINLCD_SIZE_128X160
#define KEY_LOCK_EFFECT6_TARGET_WIDTH               40
#define KEY_LOCK_EFFECT6_TARGET_OFFSET              10
#define KEY_LOCK_EFFECT6_TARGET_RISE                  25

#define KEY_LOCK_EFFECT6_PANEL_Y                        120
#define KEY_LOCK_EFFECT6_PANEL_HEIGHT               35

#define KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT       10
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_UP          5
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_DOWN     5
#else
#error "Don't support this LCD pixel"
#endif
#define KEY_LOCK_EFFECT6_ICONS_MARGIN_RIGHT  KEY_LOCK_EFFECT6_ICONS_MARGIN_LEFT

#endif

/*---------------------------------------------------------------------------*/

#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT

#ifdef MAINLCD_SIZE_240X320
#define KEY_LOCK_PSW_CODE_HMARGIN 6
#elif defined MAINLCD_SIZE_240X400
#define KEY_LOCK_PSW_CODE_HMARGIN 6
#elif defined MAINLCD_SIZE_320X480
#define KEY_LOCK_PSW_CODE_HMARGIN 13
#else
#define KEY_LOCK_PSW_CODE_HMARGIN 13
#endif

#endif

#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
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

