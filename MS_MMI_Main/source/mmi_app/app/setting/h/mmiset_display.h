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
//����flash��С����,��������ֻ֧��2��
#define MMISET_FIXED_POWER_ON_OFF_ANIMATE_MAX_NUM      2 //�̶����ػ����ػ������������Ŀ

#define MMISET_TIME_30                      30000 //30s
#define MMISET_TIME_25                      25000 //25s
#define MMISET_TIME_20                      20000 //20s
#define MMISET_TIME_15                      15000 //15s
#define MMISET_TIME_10                      10000 //10s
#define MMISET_TIME_5                       5000 //5s
#define MMISET_TIME_40                     40000//40s
#define MMISET_TIME_60                     60000//60s
#define MMISET_TIME_120                   120000//120s
#define MMISET_BACKLIGHT_DEFAULT_TIME       MMISET_EDEFAULT_BACKLIGHT_TIME//MMISET_TIME_15 //������Ļ����Ĭ������

#ifdef MULTI_THEME_SUPPORT
#define MMISET_THEME_MAX_NUM      2   //����������Ŀ
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
//�̶��Ķ���ǽֽ����Ϣ������ǽֽ��֡����ǽֽͼƬ
//typedef struct
//{
//    uint8           page_num;
//    MMI_IMAGE_ID_T     page[MMISET_FIXED_ANIM_MAX_PAGE];
// }MMISET_FIXED_ANIM_WALLPAPER_T;

//���ػ���������Ϣ����������id
typedef struct
{
    MMI_ANIM_ID_T                 animate_id;   //the id of animate
}MMISET_ANIMATE_SET_T;

typedef enum
{
    MMISET_3D_STYLE_MAINMENU_NONE,
    MMISET_3D_STYLE_MAINMENU_MAGICMOVE,     //�ƶ�ħ��    
    MMISET_3D_STYLE_MAINMENU_ROTATE3D,      //3Dħ��
    MMISET_3D_STYLE_MAINMENU_SLIDEMOVE,     //��������
    MMISET_3D_STYLE_MAINMENU_ROTATEMOVE,    //��ת��Ȥ
    MMISET_3D_STYLE_MAINMENU_LUCKYBOARD,    //���˷���
    MMISET_3D_STYLE_MAINMENU_WRAPBAND,      //���ʴ�
    MMISET_3D_STYLE_MAINMENU_DISTORTMOVE,   //��ת����

    MMISET_3D_STYLE_MAINMENU_MAX,
}MMISET_3D_STYLE_MAINMENU_E;    //���˵���Ч
#ifdef UI_P3D_SUPPORT   
#define WINSE_DEF(winse_type, winse_setting_type)  winse_setting_type,
#endif
typedef enum
{
    MMISET_3D_STYLE_SCREEN_NONE,        // �رմ�����Ч
    MMISET_3D_STYLE_SCREEN_DEFAULT,     // ����Ӧ�ó����ص��趨    
    MMISET_3D_STYLE_SCREEN_AUTO,        // ��3DЧ��˳����ʾ
#ifdef EFFECT_WIN_SLIDE_SUPPORT    
    MMISET_3D_STYLE_SCREEN_SLIDEMOVE,   // ����ˮƽ�л�-����ԭ����
    MMISET_3D_STYLE_SCREEN_SLIDE,       // ����ˮƽ�л�-ԭ���ڸ���
#endif    
#ifdef EFFECT_ZOOM_SUPPORT    
    MMISET_3D_STYLE_SCREEN_ZOOM,        // ���ڽ�����С�����˳��ɴ��С
#endif    
#if 0 //def EFFECT_WIN_SLIDE_SUPPORT
    MMISET_3D_STYLE_SCREEN_FADE,        // ���ڵ���
#endif    
#ifdef EFFECT_3D_ROTATE_SUPPORT		    
    MMISET_3D_STYLE_SCREEN_3DROLL,      // ����3D��ת 
#endif    
// ������Ч���ڿ�������
#ifdef UI_P3D_SUPPORT     
#include "mmi_winse.def"
#endif
    MMISET_3D_STYLE_SCREEN_MAX,
}MMISET_3D_STYLE_SCREEN_E;    //���˵���Ч
#ifdef UI_P3D_SUPPORT   
#undef WINSE_DEF
#endif
typedef enum
{
    MMISET_G_SENSOR_SWITCH_NONE,        // �ر�G-sensor��Ч
    MMISET_G_SENSOR_SWITCH_DEFAULT,     // �л�ʱ����Ļ��ת
    MMISET_G_SENSOR_SWITCH_FADE,        // �л�ʱ����Ļ���뵭��
    MMISET_G_SENSOR_SWITCH_MAX,         // ��Чֵ
}MMISET_G_SENSOR_SWITCH_ANIM_E;    // G-sensor��ת��Ч

typedef struct
{
    MMISET_3D_STYLE_MAINMENU_E      mainmenu_3d_style; // ���˵���Ч
    MMISET_3D_STYLE_SCREEN_E        screen_3d_style;   // ������Ч
    BOOLEAN                         list_3d_style;     // ����3D��Ч
    MMISET_G_SENSOR_SWITCH_ANIM_E   g_sensor_anim;     // g-sensor��ת��Ч
}MMISET_3D_STYLE_T;   //3D��Ч
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
//�����Ч���
typedef enum
{
    SELECT_EFFECT_INDEX_STATIC = 0,//��ֹ
    SELECT_EFFECT_INDEX_ROUNDLIGHT, //���ƹ���Ч��
    SELECT_EFFECT_INDEX_FIRE, //����Ч��
    SELECT_EFFECT_INDEX_LIGHTING,//����Ч��
    SELECT_EFFECT_INDEX_WATER,
    SELECT_EFFECT_INDEX_SNOW,//SELECT_EFFECT_SNOW
    SELECT_EFFECT_INDEX_RANDOM,//��̬���
    SELECT_EFFECT_INDEX_MAX
}MMIMENU_SELECT_EFFECT_STYLE_INDEX_E;//ͼ��ѡ��Ч��ö��
#endif

//yangyu add begin

typedef enum
{
    PANEL_SPACE_STATION,//�ռ�վ
    PANEL_SPACE_SHIP,//����ɴ����ֱ���
    PANEL_DOLPHIN,//����
    PANEL_ASTRONAUT,//̫����
    PANEL_PANDA,//��è
    PANEL_BLACK_GREEN,//����ɫģ��ʱ��
    PANEL_DEER,//С¹
	PANEL_BEAR_DOLL,//������
	PANEL_SAILOR_MOON,//����Ů
	PANEL_PENGUIN,//���
	
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
//  Description : ���ʱ����ʾ��ʽѡ���radio list��item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendTimeDisplayTypeListItem(
                                                 MMI_CTRL_ID_T      ctrl_id
                                                 );

/*****************************************************************************/
//  Description : Ԥ�����ػ����������б����´����
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
//  Description : ��ʾС���Աȶȵ�ͼƬ
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplaySubLcdContrastPic(void);

/*****************************************************************************/
//  Description : ��ʾС���Աȶȵĳ�ʼͼƬ
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
//  Description : ����Աȶȵ���ʾ
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
//  Description : ��ӹ̶�ͼƬ��ǽֽ��text list��item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendFixedWallpaperListItem(
                                                MMI_CTRL_ID_T       ctrl_id
                                                );

/*****************************************************************************/
//  Description : �趨�̶�ͼƬΪǽֽ
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
//  Description : �趨�̶�����Ϊǽֽ
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_SetFixedAnimWallpaper(
//                                         MMI_CTRL_ID_T        ctrl_id
//                                          );

/*****************************************************************************/
//  Description : ��ӹ̶��Ķ���ǽֽ��text list��item
//  Global resource dependence : 
//  Author: scv
//  Note:
/*****************************************************************************/
//PUBLIC void MMISET_AppendFixedAnimWallpaper(
//                                            MMI_CTRL_ID_T   ctrl_id
//                                            );
#if defined DPHONE_SUPPORT
/*****************************************************************************/
//  Description : ��ӱ���Ƶ�radio list��item
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
//  Description : �趨�����ʱ��
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
//  Description : ��ʾ�Աȶȵĳ�ʼͼƬ
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayInitContrast(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
//  Description : ��ʾ�Աȶȵ�ͼƬ
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_DisplayContrastPic(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : ����Աȶȵ���ʾ
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
//  Description : ���3D style mainmenu
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
// 	Description : ��Ӵ��ڵ�3D��Ч
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
//  Description : ����3D style mainmenu�Ľ��
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
// 	Description : ���ô��ڵ�3D��Ч
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleScreen(
                                    MMI_CTRL_ID_T   ctrl_id
                                    );
#endif
/*****************************************************************************/
// 	Description : �õ����ڵ�3D��Ч����
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleScreen(void);

/*****************************************************************************/
//  Description : �õ�3D style mainmenu������
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_Get3DStyleMainmenu(void);

/*****************************************************************************/
//  Description : ���3D style other
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Append3DStyleOtherListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/*****************************************************************************/
//  Description : ����3D������Ч�Ŀ���/�ر����
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleOther(
                                   MMI_CTRL_ID_T        ctrl_id
                                   );

/*****************************************************************************/
// 	Description : ����g-sensor�л���Ч
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetGsensorSwitchStyle(
                                         uint32 g_sensor_anim
                                         );
#ifdef SET_3D_STYLE_SUPPORT
/*****************************************************************************/
//  Description : �ָ�Ĭ��ֵNV 3D style
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
// 	Description : ����list ��Ч�Ŀ���/�ر����
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_Set3DStyleList3DStyle(MMI_CTRL_ID_T	    ctrl_id);

#endif

#ifdef MCARE_V31_SUPPORT
/*****************************************************************************/
// 	Description : ����3������Ч����
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MCAREV31_Set3DStyleScreen(uint16 screen_3d_style);
#endif

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/*****************************************************************************/
// 	Description : �趨��Ƶǽֽ
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
// 	Description : �趨��̬ǽֽ
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
