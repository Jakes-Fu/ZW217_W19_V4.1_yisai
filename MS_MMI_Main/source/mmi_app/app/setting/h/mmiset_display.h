/*****************************************************************************
** File Name:      mmiset_display.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li          Create
******************************************************************************/

#ifndef _MMISET_DISPLAY_H_
#define _MMISET_DISPLAY_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmi_position.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//由于flash大小限制,开机动画只支持2种
#define MMISET_FIXED_POWER_ON_OFF_ANIMATE_MAX_NUM      2 //固定开关机开关机动画的最大数目

#define MMISET_TIME_30                      30000 //30s
#define MMISET_TIME_25                      25000 //25s
#define MMISET_TIME_20                      20000 //20s
#define MMISET_TIME_15                      15000 //15s
#define MMISET_TIME_10                      10000 //10s
#define MMISET_TIME_5                       5000 //5s
#define MMISET_TIME_40                     40000//40s
#define MMISET_TIME_60                     60000//60s
#define MMISET_TIME_120                   120000//120s
#define MMISET_BACKLIGHT_DEFAULT_TIME       MMISET_EDEFAULT_BACKLIGHT_TIME//MMISET_TIME_15 //待机屏幕背关默认亮度

#ifdef MULTI_THEME_SUPPORT
#define MMISET_THEME_MAX_NUM      2   //主题的最大数目
#endif

#if (defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON) && defined(MMI_COMMON_IDLE_SUPPORT)
#define  MMISET_IDLE_STYLE_SUPPORT
#endif

#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON || defined(MMI_COMMON_IDLE_SUPPORT)
#define MMISET_IDLE_DISPLAY_SUPPORT
#endif

#if defined MMI_KEY_LOCK_EFFECT_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
#define  MMISET_KEYLOCK_SUPPORT 
#endif
#endif

#if defined FONT_TYPE_SUPPORT_VECTOR && defined FILE_FONT_SUPPORT
#define MMISET_FONT_SUPPORT
#endif

#if !defined FLASH_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
#define MMISET_MAINMENU_EFFECT_SUPPORT
#endif
#endif

#if defined SCREENSAVER_SUPPORT
#define MMISET_SCREENSAVER_SUPPORT
#endif

#if !defined MMI_PDA_SUPPORT && defined MULTI_THEME_SUPPORT
#define MMISET_THEME_SUPPORT
#endif

#if !defined FLASH_SUPPORT && defined MMI_WALLPAPER_SUPPORT
#define MMISET_WALLPAPER_SUPPORT
#endif
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
#define MMISET_ANIM_EFFECT_SUPPORT
#endif

#if !defined MMI_KEY_LOCK_BACKLIGHT_IN_1_SUPPORT
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
#define MMISET_BACKLIGHT_TIME_SUPPORT
#endif
#endif

#define MMISET_KEYBACKLIGHT_SUPPORT

#if defined PDA_UI_SUPPORT_U_IDLE
#define MMISET_UIDLE_EFFECT_SUPPORT
#endif

#if defined DPHONE_SUPPORT 
#define MMISET_KEY_BOARD_LIGHT_SUPPORT
#endif

//#define MMISET_WALLPAPER_FIXED_ANIM_MAX_NUM     2
//#define MMISET_FIXED_ANIM_MAX_PAGE              4
//#define MMISET_FIXED_ANIM_TIME_INTERVAL         500

//set contrast 
//#define MMISET_CONTRAST_BG_X                0   
//#define MMISET_CONTRAST_BG_Y                MMI_TITLE_HEIGHT

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//固定的动画墙纸的信息，包括墙纸总帧数，墙纸图片
//typedef struct
//{
//    uint8           page_num;
//    MMI_IMAGE_ID_T     page[MMISET_FIXED_ANIM_MAX_PAGE];
// }MMISET_FIXED_ANIM_WALLPAPER_T;

//开关机动画的信息，包括动画id
typedef struct
{
    MMI_ANIM_ID_T                 animate_id;   //the id of animate
}MMISET_ANIMATE_SET_T;

typedef enum
{
    MMISET_3D_STYLE_MAINMENU_NONE,
    MMISET_3D_STYLE_MAINMENU_MAGICMOVE,     //移动魔术    
    MMISET_3D_STYLE_MAINMENU_ROTATE3D,      //3D魔方
    MMISET_3D_STYLE_MAINMENU_SLIDEMOVE,     //滑滑布丁
    MMISET_3D_STYLE_MAINMENU_ROTATEMOVE,    //旋转乐趣
    MMISET_3D_STYLE_MAINMENU_LUCKYBOARD,    //幸运翻板
    MMISET_3D_STYLE_MAINMENU_WRAPBAND,      //卷动彩带
    MMISET_3D_STYLE_MAINMENU_DISTORTMOVE,   //流转星云

    MMISET_3D_STYLE_MAINMENU_MAX,
}MMISET_3D_STYLE_MAINMENU_E;    //主菜单特效
#ifdef UI_P3D_SUPPORT   
#define WINSE_DEF(winse_type, winse_setting_type)  winse_setting_type,
#endif
typedef enum
{
    MMISET_3D_STYLE_SCREEN_NONE,        // 关闭窗口特效
    MMISET_3D_STYLE_SCREEN_DEFAULT,     // 依据应用场景特点设定    
    MMISET_3D_STYLE_SCREEN_AUTO,        // 各3D效果顺序显示
#ifdef EFFECT_WIN_SLIDE_SUPPORT    
    MMISET_3D_STYLE_SCREEN_SLIDEMOVE,   // 窗口水平切换-覆盖原窗口
    MMISET_3D_STYLE_SCREEN_SLIDE,       // 窗口水平切换-原窗口跟随
#endif    
#ifdef EFFECT_ZOOM_SUPPORT    
    MMISET_3D_STYLE_SCREEN_ZOOM,        // 窗口进入由小变大和退出由大变小
#endif    
#if 0 //def EFFECT_WIN_SLIDE_SUPPORT
    MMISET_3D_STYLE_SCREEN_FADE,        // 窗口淡化
#endif    
#ifdef EFFECT_3D_ROTATE_SUPPORT		    
    MMISET_3D_STYLE_SCREEN_3DROLL,      // 窗口3D翻转 
#endif    
// 后续特效请在框架中添加
#ifdef UI_P3D_SUPPORT     
#include "mmi_winse.def"
#endif
    MMISET_3D_STYLE_SCREEN_MAX,
}MMISET_3D_STYLE_SCREEN_E;    //主菜单特效
#ifdef UI_P3D_SUPPORT   
#undef WINSE_DEF
#endif
typedef enum
{
    MMISET_G_SENSOR_SWITCH_NONE,        // 关闭G-sensor特效
    MMISET_G_SENSOR_SWITCH_DEFAULT,     // 切换时，屏幕旋转
    MMISET_G_SENSOR_SWITCH_FADE,        // 切换时，屏幕淡入淡出
    MMISET_G_SENSOR_SWITCH_MAX,         // 无效值
}MMISET_G_SENSOR_SWITCH_ANIM_E;    // G-sensor旋转特效

typedef struct
{
    MMISET_3D_STYLE_MAINMENU_E      mainmenu_3d_style; // 主菜单特效
    MMISET_3D_STYLE_SCREEN_E        screen_3d_style;   // 窗口特效
    BOOLEAN                         list_3d_style;     // 其他3D特效
    MMISET_G_SENSOR_SWITCH_ANIM_E   g_sensor_anim;     // g-sensor旋转特效
}MMISET_3D_STYLE_T;   //3D特效
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
//点击特效相关
typedef enum
{
    SELECT_EFFECT_INDEX_STATIC = 0,//静止
    SELECT_EFFECT_INDEX_ROUNDLIGHT, //环绕光束效果
    SELECT_EFFECT_INDEX_FIRE, //火焰效果
    SELECT_EFFECT_INDEX_LIGHTING,//闪电效果
    SELECT_EFFECT_INDEX_WATER,
    SELECT_EFFECT_INDEX_SNOW,//SELECT_EFFECT_SNOW
    SELECT_EFFECT_INDEX_RANDOM,//动态随机
    SELECT_EFFECT_INDEX_MAX
}MMIMENU_SELECT_EFFECT_STYLE_INDEX_E;//图标选中效果枚举
#endif

//yangyu add begin

typedef enum
{
    PANEL_SPACE_STATION,//空间站
    PANEL_SPACE_SHIP,//宇宙飞船数字表盘
    PANEL_DOLPHIN,//海豚
    PANEL_ASTRONAUT,//太空人
    PANEL_PANDA,//熊猫
    PANEL_BLACK_GREEN,//黑绿色模拟时钟
    PANEL_DEER,//小鹿
	PANEL_BEAR_DOLL,//熊娃娃
	PANEL_SAILOR_MOON,//美少女
	PANEL_PENGUIN,//企鹅
	
}PANEL_INDEX_E;

typedef struct 
{
    BOOLEAN is_anolog_panel;
    PANEL_INDEX_E watch_index;
}WATCH_PANEL_TYPE_T;

//yangyu end

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set the mode of TV out
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetTVOutMmode(
//                              MMI_CTRL_ID_T       ctrl_id
//                              );

/*****************************************************************************/
//  Description : init display set
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayInit(void);

/*****************************************************************************/
//  Description : set the selected type of time display 
//  Global resource dependence : none
//  Author: SCV
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectTimeDisplyType(
                                           MMI_CTRL_ID_T        ctrl_id
                                           );

/*****************************************************************************/
//  Description : get the type of time display type
//  Global resource dependence : none
//  Author: SCV
//  Note:
/*****************************************************************************/
// PUBLIC MMISET_TIME_DISPLAY_TYPE_E MMISET_GetTimeDisplayType(void);

/*****************************************************************************/
//  Description : 添加时间显示格式选择的radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendTimeDisplayTypeListItem(
                                                 MMI_CTRL_ID_T      ctrl_id
                                                 );

/*****************************************************************************/
//  Description : 预览开关机动画返回列表后更新打勾情况
//  Global resource dependence : 
//  Author:scv
//  Note: 
/*****************************************************************************/
/*  //@xingdong.li, commented
PUBLIC void MMISET_UpdateSelectedAnimateItem(
MMI_CTRL_ID_T ctrl_id                                   
);
*/
#ifndef MMI_DISPLAYSET_MINI_SUPPORT
/*****************************************************************************/
//  Description :  set idle words content
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetIdleWordsContent( 
                                          MMI_STRING_T    *content_info
                                          );
#endif
/*****************************************************************************/
//  Description : 显示小屏对比度的图片
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplaySubLcdContrastPic(void);

/*****************************************************************************/
//  Description : 显示小屏对比度的初始图片
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplaySubLcdInitContrast(void);

/*****************************************************************************/
//  Description : set contrast of sub LCD
//  Global resource dependence : none
//  Author: scv
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetSubLCDContrast(
//                                     uint8    sublcd_contrast//the contrast of sublcd
//                                      );

/*****************************************************************************/
//  Description : update contrast of LCD
//  Global resource dependence : none
//  Author: scv
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_UpdateSubLCDContrast(
//                                        uint8  lcd_id,     //the id of lcd
//                                        uint8  lcd_contrast//the contrast of lcd
//                                         );

/*****************************************************************************/
//  Description : 调解对比度的显示
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_TuneUpSubLcdConstrast(
                                         MMI_MESSAGE_ID_E   msg_id
                                         );
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : set idle win display mode
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleDisplayMode(
                                      uint16  cur_index
                                      );
#endif
#endif
/*****************************************************************************/
//  Description : 添加固定图片的墙纸到text list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendFixedWallpaperListItem(
                                                MMI_CTRL_ID_T       ctrl_id
                                                );

/*****************************************************************************/
//  Description : 设定固定图片为墙纸
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetFixedWallpaper(
                                     MMI_CTRL_ID_T      ctrl_id
                                     );
#if !defined(MMI_DISPLAYSET_MINI_SUPPORT) || defined (MAINLCD_SIZE_128X160)
#if defined MMISET_IDLE_DISPLAY_SUPPORT
/*****************************************************************************/
//  Description : add idle display list to radio list 
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendIdleDisplayListItem(
                                             MMI_CTRL_ID_T      ctrl_id
                                             );
#endif
#endif
/*****************************************************************************/
//  Description : 设定固定动画为墙纸
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedAnimWallpaper(
//                                         MMI_CTRL_ID_T        ctrl_id
//                                          );

/*****************************************************************************/
//  Description : 添加固定的动画墙纸到text list的item
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendFixedAnimWallpaper(
//                                            MMI_CTRL_ID_T   ctrl_id
//                                            );
#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : 添加背光灯到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendBacklightListItem(
                                           MMI_CTRL_ID_T        ctrl_id
                                           );
#endif
 #ifdef MMISET_BACKLIGHT_TIME_SUPPORT  ||DPHONE_SUPPORT
/*****************************************************************************/
//  Description : 设定背光灯时间
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetBackLight(
                                MMI_CTRL_ID_T       ctrl_id
                                );
 #endif
#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : 显示对比度的初始图片
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayInitContrast(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//  Description : 显示对比度的图片
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayContrastPic(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 调解对比度的显示
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_TuneUpConstrast(
                                   MMI_MESSAGE_ID_E   msg_id
                                   );

/*****************************************************************************/
//  Description : factory reset the idle display mode 
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleDisplayMode(void);

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : add set tts list item
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendTTSListItem(
                                     MMI_CTRL_ID_T	    ctrl_id
                                     );
#endif


#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 添加3D style mainmenu
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleMainmenuListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );
#endif
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 添加窗口的3D特效
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleScreenListItem(
                                               MMI_CTRL_ID_T    ctrl_id
                                               );
#endif
#ifdef EFFECT_3D_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 设置3D style mainmenu的结果
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleMainmenu(
                                    MMI_CTRL_ID_T       ctrl_id
                                    );
#endif

#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
// 	Description : 设置窗口的3D特效
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleScreen(
                                    MMI_CTRL_ID_T   ctrl_id
                                    );
#endif
/*****************************************************************************/
// 	Description : 得到窗口的3D特效类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleScreen(void);

/*****************************************************************************/
//  Description : 得到3D style mainmenu的类型
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleMainmenu(void);

/*****************************************************************************/
//  Description : 添加3D style other
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleOtherListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/*****************************************************************************/
//  Description : 设置3D其他特效的开启/关闭情况
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleOther(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
// 	Description : 设置g-sensor切换特效
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetGsensorSwitchStyle(
                                         uint32 g_sensor_anim
                                         );
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
//  Description : 恢复默认值NV 3D style
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Reset3DStyle(void);
#endif
/*****************************************************************************/
//  Description : Set idle display style
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleStyle(
                                MMISET_IDLE_STYLE_E idle_style
                                );

/*****************************************************************************/
//Description : Set default system's wallpaperNV informatioin;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC void MMIAPISET_SetDefaultWallpaperNV(void);

/*****************************************************************************/
//Description : Get wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetWallpaperNVByStyle(MMISET_WALLPAPER_SET_T * wallpaper_info_ptr);

/*****************************************************************************/
//Description : Get wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISET_GetWallpaperNVByStyleEx(MMISET_IDLE_STYLE_E idle_style, MMISET_IDLE_SUBSTYLE_E idle_substyle, MMISET_WALLPAPER_SET_T * wallpaper_info_ptr);

/*****************************************************************************/
//Description : Set wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetWallpaperNVByStyle(MMISET_WALLPAPER_SET_T * wallpaper_info_ptr);

/*****************************************************************************/
//Description : Set wallpaperNV informatioin for different systems;
//Global resource dependence : 
//Author: Glen.Li
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetWallpaperNVByStyleEx(MMISET_IDLE_STYLE_E idle_style, MMISET_IDLE_SUBSTYLE_E idle_substyle, MMISET_WALLPAPER_SET_T * wallpaper_info_ptr);

/*****************************************************************************/
// 	Description : Set idle display style
//	Global resource dependence : none
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetIdleAndSubStyle(
                                MMISET_IDLE_STYLE_E     idle_style,
                                MMISET_IDLE_SUBSTYLE_E     idle_substyle
                                );
//  Description : reset idle display style
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetIdleStyle(void);

#ifdef BAR_PHONE_WIDGET 
PUBLIC MMISET_IDLE_STYLE_E MMIAPISET_GetPreIdleStyle(void);
#endif

#ifdef LCD_SLIDE_SUPPORT
/*****************************************************************************/
// 	Description : add slide open radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSlideOpenTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 );

/*****************************************************************************/
// 	Description : add slide close radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSlideCloseTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 );

/*****************************************************************************/
// 	Description : add keylock switch radio list item
//	Global resource dependence : 
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendKeylockSwitchTypeListItem(
                                                 MMI_CTRL_ID_T	    ctrl_id
                                                 );

/*****************************************************************************/
// 	Description : set the selected type of slide open 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectSlideOpenType(
                                           MMISET_SLIDE_OPEN_TYPE_E slide_open_type
                                           );
/*****************************************************************************/
// 	Description : set the selected type of slide close 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectSlideCloseType(
                                           MMISET_SLIDE_CLOSE_TYPE_E slide_close_type
                                           );

/*****************************************************************************/
// 	Description : set the selected type of keylock switch 
//	Global resource dependence : none
//  Author: jianhui.luo
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectKeylockSwitchType(
                                           MMISET_KEYLOCK_SWITCH_TYPE_E keylock_switch_type
                                           );

#endif

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
// 	Description : 设置list 特效的开启/关闭情况
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleList3DStyle(MMI_CTRL_ID_T	    ctrl_id);

#endif

#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
// 	Description : 设置3窗口特效类型
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MCAREV31_Set3DStyleScreen(uint16 screen_3d_style);
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
// 	Description : 设定视频墙纸
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetVideoWallpaper(
                                    MMI_CTRL_ID_T      ctrl_id
                                    );
#endif

 #ifdef LIVE_WALLPAPER_FRAMEWORK_SUPPORT
/*****************************************************************************/
// 	Description : 设定动态墙纸
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLiveWallpaper(
                                    MMI_CTRL_ID_T      ctrl_id,
                                    uint32 id
                                    );
#endif

/*****************************************************************************/
// 	Description : reset all styles' wallpaper at restoring Factory Setting.
//	Global resource dependence : 
//  Author: Glen.Li
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetAllStylesWallpaperAtRestoreFactory(void);

//yangyu add
PUBLIC void MMISET_SetWatchPanelStyle(WATCH_PANEL_TYPE_T style);
PUBLIC WATCH_PANEL_TYPE_T MMISET_GetWatchPanelStyle();
PUBLIC void MMIAPISET_GetCurrentDateMonDayStr(uint8* datestr,char symbol,uint16 datestr_len);

//yangyu end

#endif  //_MMISET_DISPLAY_H_
