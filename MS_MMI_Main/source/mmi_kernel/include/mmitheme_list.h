/*****************************************************************************
** File Name:      mmitheme_list.h                                           *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create								     *
**																		     *
*****************************************************************************/

#ifndef  _MMITHEME_LIST_H_    
#define  _MMITHEME_LIST_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guititle.h"
#include "guiedit.h"
#include "guiwin.h"
#include "ui_layer.h"
#include "mmitheme_pos.h"
#include "mmitheme_update.h"
#include "mmi_default.h"
#include "guistring.h"


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
        
/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/

#define GUIITEM_CONTENT_MAX_NUM  6
#define GUIITEM_NUM_ICON_MAX     20

#define MMILIST_SLIDE_TIME       25
#define MMILIST_AUTO_SWITCH_TIME 500
#define MMILIST_NOT_DISP_TIME    500

#define MMILIST_INVALID_ITEM_INDEX  -1

#define THEMELIST_ITEM_COLOR_DEFAULT 0x0

typedef uint8 MMILIST_COLOR_ID_E;

enum
{
    MMILIST_COLOR_NONE = 0,
    MMILIST_COLOR_WECAHT_MINE,
    MMILIST_COLOR_WECAHT_OTHER,
    MMILIST_COLOR_ALARMCLOCK,
    MMILIST_COLOR_PEDOMETER,
    MMILIST_COLOR_SETTING,
	MMILIST_COLOR_WECAHT_TEXT,
    MMILIST_COLOR_MAX
};

typedef uint8 MMITHEME_COLOR_ID_E;

enum
{
    MMITHEME_COLOR_NONE = 0,
    MMITHEME_COLOR_1,
    MMITHEME_COLOR_2,
    MMITHEME_COLOR_3,
    MMITHEME_COLOR_4,
    MMITHEME_COLOR_5,
    MMITHEME_COLOR_6,
    MMITHEME_COLOR_7,
    MMITHEME_COLOR_8,
    MMITHEME_COLOR_9,
    MMITHEME_COLOR_BLUE,
#ifdef ADULT_WATCH_SUPPORT
    MMITHEME_COLOR_GRAY,
    MMITHEME_COLOR_B,
    MMITHEME_COLOR_RED,
#endif
//zmt add start
    MMITHEME_COLOR_RED,
    MMITHEME_COLOR_LIGHT_BLUE,
//zmt add end
    MMITHEME_COLOR_MAX
};

//ÿ��ö��ֵ��Ӧһ��theme����
// typedef enum
// {
// 	GUIITEM_THEME_DATA_0,
// 	GUIITEM_THEME_DATA_1,
//     GUIITEM_THEME_DATA_2,
//     GUIITEM_THEME_DATA_3,
//     GUIITEM_THEME_DATA_MAX
// 
// }GUIITEM_THEME_DATA_ID_E;
//��¼���ͼƬ����
typedef enum
{
    GUIITEM_THEME_ALERT_BG,               //��ʾ��ť�ı���
    GUIITEM_THEME_ITEM_FULL_BG,           //δѡ�еļ�¼ȫ�ı���
	GUIITEM_THEME_ITEM_HEAD_BG,           //δѡ�еļ�¼ͷ������
    GUIITEM_THEME_ITEM_BODAY_BG,          //δѡ�еļ�¼�в�����
    GUIITEM_THEME_ITEM_TAIL_BG,           //δѡ�еļ�¼β������
    GUIITEM_THEME_HIGHLIGHT_ITEM_FULL_BG, //ѡ�еļ�¼ȫ�ı���
	GUIITEM_THEME_HIGHLIGHT_ITEM_HEAD_BG, //ѡ�еļ�¼ͷ������
    GUIITEM_THEME_HIGHLIGHT_ITEM_BODAY_BG,//ѡ�еļ�¼�в�����
    GUIITEM_THEME_HIGHLIGHT_ITEM_TAIL_BG  //ѡ�еļ�¼β������
}GUIITEM_THEME_BGIMAGE_TYPE_E;

//ÿ��ö��ֵ��Ӧһ��theme����, ����ΪGUIITEM_THEME_STATE_MAX

typedef enum
{
    GUIITEM_THEME_DEFAULT,
    GUIITEM_THEME_NORMAL,
    GUIITEM_THEME_EXPAND,
    GUIITEM_THEME_SMALL,
    GUIITEM_THEME_BIG,
    GUIITEM_THEME_MAX

}GUIITEM_THEME_ID_E;

//ÿ��ö��ֵ��Ӧһ��layout����, ����ΪGUIITEM_THEME_STATE_MAX
typedef enum
{
    GUIITEM_LAYOUT_ONE_LINE_TEXT_SPLIT,
    GUIITEM_LAYOUT_ONE_LINE_TEXT_ICON,  
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_HALF_LINE_TEXT_ICON, 
#endif
    GUIITEM_LAYOUT_ONE_LINE_ICON_TEXT_ICON,
	GUIITEM_LAYOUT_ONE_LINE_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_ONE_LINE_TEXT_DEFAULT_FONT,//NEWMS00185675
#endif
    GUIITEM_LAYOUT_ONE_LINE_ICON_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_ONE_LINE_TWO_ICON_TEXT,
    GUIITEM_LAYOUT_ONE_LINE_NUMBER_TEXT,
#endif
    GUIITEM_LAYOUT_ONE_LINE_ICON_TWO_TEXT,
    GUIITEM_LAYOUT_ONE_LINE_ICON_TEXT_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_ONE_LINE_TWO_ICON_TWO_TEXT,
	GUIITEM_LAYOUT_ONE_LINE_BUTTON_TEXT,
	GUIITEM_LAYOUT_ONE_LINE_TWO_BUTTON_TEXT,
#endif
	GUIITEM_LAYOUT_ONE_LINE_SMALL_TEXT_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
	GUIITEM_LAYOUT_ONE_LINE_SMALL_TEXT_TEXT_ICON,
	GUIITEM_LAYOUT_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON,
#endif
    GUIITEM_LAYOUT_ONE_LINE_CHECK,
    GUIITEM_LAYOUT_ONE_LINE_CHECK_EXT,
    GUIITEM_LAYOUT_ONE_LINE_CHECK_ICON,

    GUIITEM_LAYOUT_TWO_LINE_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ICON_TEXT_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_NUMBER_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL,
#endif
    GUIITEM_LAYOUT_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ICON_TWO_TEXT_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL,
#endif
    GUIITEM_LAYOUT_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT_AND_TEXT_2,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
	GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON,
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON,
    GUIITEM_LAYOUT_TWO_LINE_CHECK,
#endif
    GUIITEM_LAYOUT_TWO_LINE_CHECK_ICON,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_2TEXT_AND_3ICON,
    GUIITEM_LAYOUT_TWO_LINE_2TEXT_AND_2ICON,
	GUIITEM_LAYOUT_TWO_LINE_ANIM_2TEXT_AND_ICON,  //wz for dcd
#endif
    GUIITEM_LAYOUT_TWO_LINE_TEXT_AND_TEXT_ANIM,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM,
#endif
    GUIITEM_LAYOUT_TWO_LINE_TEXT_2ANIM_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_BIGICON_TEXT,
#endif
#endif
    GUIITEM_LAYOUT_ONE_TWO_LINE_ICON_TEXT_AND_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT,
    GUIITEM_LAYOUT_ONE_TWO_LINE_CHECK,
    GUIITEM_LAYOUT_ONE_TWO_LINE_CHECK_ICON,
    GUIITEM_LAYOUT_ONE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON,
#endif
    GUIITEM_LAYOUT_THREE_LINE_TEXT,
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_THREE_LINE_TEXT_ICON,
#endif
    //GUIITEM_LAYOUT_ONE_TWO_LINE_SETTING_TEXT_AND_ICON_TEXT,
    GUIITEM_LAYOUT_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL,//mary
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON,
    
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT,
    
    GUIITEM_LAYOUT_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT,
#endif
    
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TEXT,//mary
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_ANIM_ANIM_TEXT,//mary
    GUIITEM_LAYOUT_TWO_LINE_CHECK_ANIM_TEXT,//mary
    GUIITEM_LAYOUT_TWO_LINE_RADIO_ANIM_TEXT,//mary
#endif
    GUIITEM_LAYOUT_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT,//mary
    GUIITEM_LAYOUT_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT,//mary
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    GUIITEM_LAYOUT_TWO_LINE_2TEXT_2ANIM_AND_TEXT,//mary
    GUIITEM_LAYOUT_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON,


    GUIITEM_LAYOUT_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT,
        
#ifdef MMI_PDA_SUPPORT
	GUIITEM_LAYOUT_TWO_LINE_TWO_ICON_TEXT_AND_ICON,
	GUIITEM_LAYOUT_ONE_LINE_TEXT_TEXT_ICON,
	GUIITEM_LAYOUT_ONE_LINE_TEXT_ISTYLE,
    GUIITEM_LAYOUT_ONE_LINE_ICON_TWO_TEXT_ICON,
#endif

//xiyuan edit
    GUIITEM_LAYOUT_ONE_LINE_LEFT_SPACE_TEXT,
#endif

    GUIITEM_LAYOUT_TWO_LINE_TEXT_ANIM_AND_TEXT,//minghu for cr123283 

#ifndef MMI_THEME_LIST_MINI_SUPPORT
	// cheng.luo
#ifdef QBTHEME_SUPPORT
	GUIITEM_LAYOUT_THREE_LINE_TEXT_TEXT_ANIM,
	GUIITEM_LAYOUT_ONE_LINE_ANIM_TEXT_ICON,
#endif
    GUIITEM_LAYOUT_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2,//Sam.hua
    GUIITEM_LAYOUT_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_3,//Sam.hua
    GUIITEM_LAYOUT_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL,
    
    GUIITEM_LAYOUT_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2,
#endif

    GUIITEM_LAYOUT_MAX

};

typedef uint16 GUIITEM_LAYOUT_ID_E;

//ÿ��ö��ֵ��Ӧitem�ĸ���״̬, ״̬��ͬ, ��Ӧ��layout��themeҲ��ͬ
typedef enum
{
    GUIITEM_THEME_STATE_DEFAULT,         //�������
	GUIITEM_THEME_STATE_FOCUS,           //����
    GUIITEM_THEME_STATE_INVALID,         //��Ч
    GUIITEM_THEME_STATE_FOCUS_INVALID,   //������Ч
    GUIITEM_THEME_STATE_LEFT_FOCUS,      
    GUIITEM_THEME_STATE_RIGHT_FOCUS,
    GUIITEM_THEME_STATE_MAX

}GUIITEM_THEME_STATE_E;

//ÿ��ö��ֵ��Ӧһ��GUIITEM_STYLE_T�ṹ, ��������content������,��ɫ,����
enum
{
    GUIITEM_STYLE_DEFAULT,
    //|---------------------------|
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT,

    //|---------------------------|
    //|        text          @    |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TEXT_ICON,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|        text          @    |
    //|---------------------------|
    GUIITEM_STYLE_HALF_LINE_TEXT_ICON,
#endif

    //|---------------------------|
    //|     @   text          @   |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON,
    
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|-----------------------------------------------------------------------------|
    //|                            @   text          @                              |
    //|���ַ����GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICONΨһ��ͬ������ͼƬ1������check|
    //|-----------------------------------------------------------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT,
#endif

    /* ���¶�Ϊ����item, checkΪ��ѡ, radioΪ��ѡ, number����itemǰ�������� */

    //|---------------------------|
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TEXT,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|        text               |
    //|---------------------------|//NEWMS00185675 ��GUIF_LIST_PDASTYLE����¸����������С����
    GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT,
#endif

    //|---------------------------|
    //|  @     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TEXT,
    
    //|---------------------------|
    //|  @     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  @  @  text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT,

    //|---------------------------|
    //|  ��   text                |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT,
#endif

    //|---------------------------|
    //|  @     text       text    |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT,
    
	//|---------------------------|
    //|   @   text           text   |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT,  // item pos is not differnt with above one
    
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  @  @  text       text    |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT,
	
	//|---------------------------|
    //|           | text |        |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT,
	//|---------------------------|
    //|   | text |      |text|    |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT,
#endif
 #ifdef XYSDK_SUPPORT
		GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_XMLY,
		GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_XMLY2,
		GUIITEM_STYLE_ONE_LINE_TEXT_ICON_XMLY,
		GUIITEM_STYLE_ONE_LINE_ICON_ICON_TEXT_ICON_XMLY,
#endif           

	//|---------------------------|
    //| text                text  |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
	//|---------------------------|
    //| text             text  @  |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON, //edited by hongjun.jia for cr178020

	//|---------------------------|
    //| text   text      @     @  |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON,
#endif

    //|---------------------------|
    //| ��     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_CHECK,

	//|---------------------------|
    //| ��    text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_CHECK_EXT,

    //|---------------------------|
    //| ��     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_RADIO,

    //|---------------------------|
    //| ��     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_RADIO_EXT,

    //|---------------------------|
    //| ��     text           @   |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_CHECK_ICON,
    
    //|---------------------------|
    //| ��     text           @   |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_RADIO_ICON,


    /* ���¶�Ϊ˫��item, AND��ʾ�ڶ��� */

    //|---------------------------|
    //|        text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT,

    //|---------------------------|
    //| text            text      |
    //| text            text      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT,

    //|---------------------------|
    //|  @     text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  @ @   text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,

    //|---------------------------|
    //|  ��    text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT,

    //|---------------------------|
    //|  @     text               |
    //|        text     text      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,

    //|---------------------------|
    //|  @     text               |
    //|        text     text      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL,
#endif

    //|---------------------------|
    //|  @ @   text               |
    //|        text     text      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,

    //|---------------------------|
    //|  @     text     text      |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT,

    //|---------------------------|
    //|  @     text     text      |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL,

    //|---------------------------|
    //|  @ @   text     text      |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT,

    //|---------------------------|
    //|  ����   text              |
    //|  ����            text     |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT,

    //|---------------------------|
    //|  ����   text              |
    //|  ����   text              |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  ����   text         @    |
    //|  ����   text              |
    //|---------------------------|
	GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON,

    //|---------------------------|
    //|  ����   text       @ @ @  |
    //|  ����   text              |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON,
    //|---------------------------|
    //| ��     text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_RADIO,
#endif
    //|---------------------------|
    //| ��     text           @   |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_CHECK_ICON,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  ����   text         @    |
    //|  ����   text              |
    //|---------------------------|
	GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON,  //wz for dcd
#endif

    //|---------------------------|
    //|         text         ���� |
    //|         text         ���� |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //| ����    text         ���� |
    //| ����    text         ���� |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM,
#endif

    //|---------------------------|
    //|  text           ���� ���� |
    //|  text           ���� ���� |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT,
    /* ���¶�Ϊ����ʱ˫��, �Ǹ���ʱ���е�item */

    //|---------------------------|
    //|  @     text               |
    //|        text               |
    //|---------------------------|

    //|---------------------------|
    //|  @     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT,

#ifndef MMI_THEME_LIST_MINI_SUPPORT    
    //|---------------------------|
    //|  @ @   text               |
    //|        text               |
    //|---------------------------|

    //|---------------------------|
    //|  @ @   text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT,

    //|---------------------------|
    //|  ��    text               |
    //|        text               |
    //|---------------------------|
    
    //|---------------------------|
    //|  ��    text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT,

    //|---------------------------|
    //|  @     text               |
    //|        text     text      |
    //|---------------------------|
    
    //|---------------------------|
    //|  @     text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT,

    //|---------------------------|
    //|  @ @   text               |
    //|        text     text      |
    //|---------------------------|

    //|---------------------------|
    //|  @ @   text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT,
            
    //|---------------------------|
    //|  ����   text       @ @ @  |
    //|  ����   text              |
    //|---------------------------|

    //|---------------------------|
    //|  ����   text       @ @ @  |
    //|---------------------------|
    GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON,
//    GUIITEM_STYLE_ONE_TWO_LINE_SETTING_TEXT_AND_ICON_TEXT,
#endif
    /*���¶�������*/
    
    //|---------------------------|
    //|        text               |
    //|        text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_THREE_LINE_TEXT,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|        text               |
    //|        text        @      |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_THREE_LINE_TEXT_ICON,    
#endif

    //|---------------------------|
    //|        text     @         |
    //|        text     text      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL,//mary

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //| ����   text     text      |
    //| ����   @text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON,

    //|---------------------------|
    //| ����   text     text      |
    //| ����   text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT,


    //|---------------------------|
    //| ����   @text             |
    //| ����   @text     text          |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT,
#endif

    //|---------------------------|
    //| ����   text               |
    //| ����                      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TEXT,//mary

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //| ����   text               |
    //| ����                      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT,//mary

    //|---------------------------|
    //| ����   text            �� |
    //| ����                      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT,//mary
#endif
    //|---------------------------|
    //| ����   text             ��|
    //| ����                      |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT,//mary

    //|---------------------------|
    //| ����   text             ��|
    //| ����   text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT,//mary  

#ifndef MMI_THEME_LIST_MINI_SUPPORT
    //|---------------------------|
    //|  text  text      ��?���� |
    //|  text           ���� ���� |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT,//mary
    
    //|---------------------------|
    //| ����   @text  text           |
    //| ����   @text              |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT,
    
#ifdef MMI_PDA_SUPPORT
    //|---------------------------|
    //|        text             	text@ 	|
    //|        text     @      			|
    //|---------------------------|
	GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON,
	
    //|---------------------------|
    //|        text             	text@ 	|
    //|---------------------------|
	GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON,
	
    //|---------------------------|
    //|  @     text       text   @|
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON,    
#endif

    //|---------------------------|
    //||_____| text               |
    //|---------------------------|
    GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT,   //xiyuan edit can adjust left space of text
#endif

    GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT,//minghu for cr123283

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
	//|---------------------------|
    //|        text               |
    //|        text               |
    //|        anim               |
    //|---------------------------|
	GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM,	//cheng.luo
	//|---------------------------|
    //|							  |
    //|   anim     text     icon  |
    //|                           |
    //|---------------------------|
	GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON,
#endif
#endif
    //|---------------------------|
    //|   @text            @ |
    //|   @text              |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2,


    //|---------------------------|
    //|  @     text     text      |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL,

#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
    //|---------------------------|
    //|  @     text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT,
#endif

    //|---------------------------|
    //| ����   text     text      |
    //| ����   text               |
    //|---------------------------|
    GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2,
#endif

//============================================================================//
//                                   For watch                                //
//============================================================================//
    //|---------------------------|
    //|     text          text   text      |
    //|---------------------------|
    GUIITEM_STYLE_SMALL_3STR,

    //|---------------------------|
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_1STR,

    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR,

    //|---------------------------|
    //|      text                @   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1ICON,

    //|---------------------------|
    //|   @     text       @text  |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR_1ICON_1STR,

    //|---------------------------|
    //|   @     text              |
    //|           text            |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_2STR,

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_1ICON,

    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR,

    //|---------------------------|
    //|  text             text    |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_3STR,

    //|---------------------------|
    //| @     text        text    |
    //|       text                |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_3STR,

    //|---------------------------|
    //|  text     @text   text    |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_1ICON_2STR,

    //|---------------------------|
    //|      text             ��   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_CHECK,

    //|---------------------------|
    //|      text            ��   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_RADIO,

    //|---------------------------|
    //|    text      |----->|     |
    //|---------------------------|
    GUIITEM_STYLE_1STR_SWITCH,

    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR_DYNA,

    //|---------------------------|
    //|    @   text               |
    //|        text               |
    //|           |@|     |@|     |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_2STR_2BUTTON_DYNA,

// watch commonlist start
    //|---------------------------|
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_LAYOUT1,

    //|---------------------------|
    //|   @     text              |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR_1LINE_LAYOUT1,

    //|---------------------------|
    //|      text             @   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1ICON_1LINE_LAYOUT1,

    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_2LINE_LAYOUT1,

    //|---------------------------|
    //|      text            ��   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_RADIO_LAYOUT1,

    //|---------------------------|
    //|      text             ��   |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_CHECK_LAYOUT1,

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_1ICON_2LINE_LAYOUT1,

    //|---------------------------|
    //|  text             text    |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_3STR_2LINE_LAYOUT1,

    //|---------------------------|
    //|    text       @text       |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT1,

    //|---------------------------|
    //|    text       @text       |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1ICON_1STR_1LINE_LAYOUT2,

    //|---------------------------|
    //|   @     text        @     |
    //|         text              |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_2STR_1ICON,
    
	//[create use for alarmClock]
	//|---------------------------|
       //|      text          icon   |
       //|      text                |
       //|---------------------------|
	GUIITEM_STYLE_TWO_TEXT_AND_ONE_ICON_MS,
	//[end]
// watch commonlist end
#ifdef ADULT_WATCH_SUPPORT
/*************Name Rule************
GUIITEM_STYLE_
[������+Context] [������Line]
[font] [Align/default:left]
[Check/Raido/default]_
ADULTWATCH
**********************************/

    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_3STR_3LINE_ADULTWATCH,
    
    //|---------------------------|
    //|  text         text        |
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_4STR_3LINE_ADULTWATCH,
    
    //|---------------------------|
    //|  text                     |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_2LINE_ADULTWATCH,


    //|---------------------------|
    //|   @    text               |
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_2STR_2LINE_ADULTWATCH,

    //|---------------------------|
    //|  text              @      |
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_2STR_1ICON_2LINE_ADULTWATCH,

    //|---------------------------|
    //|  text                     |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_LEFTALIGN_ADULTWATCH,

    //|---------------------------|
    //|           text            |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_CENTERALIGN_ADULTWATCH,
    //|---------------------------|
    //|        text          @    |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1ICON_1LINE_ADULTWATCH,
    //|---------------------------|
    //|    @         text         |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR_1LINE_ADULTWATCH,

    //|---------------------------|
    //|    ��        text         |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_RADIO_ADULTWATCH,
    //|---------------------------|
    //|      text         ��      |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_RADIO_RIGHT_ADULTWATCH,
    //|---------------------------|
    //|             @             |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1LINE_CENTERALIGN_ADULTWATCH,
    //|---------------------------|
    //|   @     text       @      |
    //|---------------------------|
    GUIITEM_STYLE_1ICON_1STR_1ICON_1LINE_ADULTWATCH,
    //|---------------------------|
    //|      text              ��  |
    //|      text                 |
    //|---------------------------|
    GUIITEM_STYLE_2STR_2LINE_CHECK_ADULTWATCH,
    //|---------------------------|
    //|     ��       text          |
    //|---------------------------|
    GUIITEM_STYLE_1STR_1LINE_CHECK_LEFT_ADULTWATCH,
#endif
    //|---------------------------|
    //|        text               |
    //|---------------------------|
    GUIITEM_STYLE_1STR_ENG,//for engineer mode

    //|---------------------------|
    //| text   ��                 |
    //|---------------------------|
    GUIITEM_STYLE_1RADIO_ENG,//for engineer mode

    //|---------------------------|
    //| text    ��                |
    //|---------------------------|
    GUIITEM_STYLE_1CHECK_ENG,//for engineer mode

//zmt add start
#if defined(WORD_CARD_SUPPORT) || defined(HANZI_CARD_SUPPORT)
    GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS,
    GUIITEM_SYTLE_ZMT_BOOK_LIST_MS,
    GUIITEM_SYTLE_ZMT_UNIT_LIST_MS,
#endif
#ifdef WORD_CARD_SUPPORT
    GUIITEM_STYLE_POETRY_GRADE_LIST_MS,
    GUIITEM_STYLE_POETRY_ITEM_LIST_MS,
#endif
#ifdef FORMULA_SUPPORT
    GUIITEM_STYLE_FORMULA_ITEM_LIST_MS,
    GUIITEM_STYLE_CLASS_MAIN_LIST_MS,
    GUIITEM_STYLE_CLASS_SECTION_LIST_MS,
#endif
//zmt add end

//============================================================================//
//                                   For watch                                //
//============================================================================//
#ifdef _SW_ZDT_PRODUCT_
    GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_BIGICON,
    GUIITEM_STYLE_TWO_LINE_BIGICON_TWO_TEXT_AND_TEXT,
    GUIITEM_STYLE_ONE_LINE_BGICON_TEXT_MS, 
    GUIITEM_STYLE_ONE_LINE_BGICON_R_TEXT_MS, 
    GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_MS,
#endif
#ifdef ZTE_WATCH
    GUIITEM_STYLE_1ICON_1STR_1LINE_WITH_UNREAD_NUM,
#endif

    GUIITEM_STYLE_MAX,
    /* reserved for the client customize style */
    GUIITEM_STYLE_CUSTOMIZE = GUIITEM_STYLE_MAX
};

typedef uint16 GUIITEM_STYLE_E;

typedef uint32 GUIITEM_STYLE_STATE_T;
/* item style state */
//#define GUIITEM_STYLE_STATE_EXPAND            0x000000001   //item����\�Ǹ����߶Ȳ�ͬ��״̬
#define GUIITEM_STYLE_STATE_RADIO             0x000000002   //item�ܹ���ѡ��״̬
#define GUIITEM_STYLE_STATE_CHECK             0x000000004   //item�ܹ���ѡ��״̬
#define GUIITEM_STYLE_STATE_NUMBER            0x000000008   //item֧�����ֵ�״̬
//#define GUIITEM_STYLE_STATE_SETTING           0x000000010   //item֧��setting��״̬

typedef uint32 GUIITEM_CONTENT_STATE_T;
/* item style state */
#define GUIITEM_CONTENT_STATE_NONE             0x000000000   //none
#define GUIITEM_CONTENT_STATE_ELLIPSIS         0x000000001   //item content��ʾ...��״̬, ����ʱҲ�������
#define GUIITEM_CONTENT_STATE_MULTILINE        0x000000002   //item content���� // ����֧�֣���Ŀǰû���õ�
#define GUIITEM_CONTENT_STATE_CHECK            0x000000004   //content��Ҫ����
#define GUIITEM_CONTENT_STATE_EFFECT_STR       0x000000008   //content������ʾ��Ч
#define GUIITEM_CONTENT_STATE_DELETE           0x000000010   //ɾ����ť
#define GUIITEM_CONTENT_STATE_TEXT_R_ALIGN     0x000000020   //�����Ƿ�����ʾ��Ĭ�Ͽ�����ʾ
#define GUIITEM_CONTENT_STATE_TEXT_M_ALIGN     0x000000040   //�����Ƿ������ʾ��Ĭ�Ͽ�����ʾ
#define GUIITEM_CUSTOM_STYLE_MASK              0x8000      //�û��Զ���style�����־
typedef struct
{
     GUI_COLOR_T    color;
     GUI_FONT_T     font;
     uint8          char_space;
     uint8          line_space;
}GUIITEM_THEME_DATA_T;

typedef struct
{
    uint16                    item_height;                                  //item height
    GUI_RECT_T                item_content_rect[GUIITEM_CONTENT_MAX_NUM];   //item content rect
    GUI_RECT_T                item_select_icon_rect;                        //item selected icon rect
    GUI_RECT_T                item_num_icon_rect;                           //item num icon rect
}GUIITEM_LAYOUT_DATA_T;

typedef struct
{
    GUIITEM_THEME_ID_E       content_theme_id[GUIITEM_CONTENT_MAX_NUM];    //item content theme
    GUIITEM_CONTENT_STATE_T  content_state[GUIITEM_CONTENT_MAX_NUM];       //item content state
    GUIITEM_LAYOUT_ID_E      layout_id;                                    //item layout id
    int16                    main_index;                                   //item main index, use for scroll, -1 is invalid value
    int16                    auxiliary_index;                              //item auxiliary index, use for display string in multi lines,
}GUIITEM_STYLE_T;


typedef enum
{
    GUILIST_PREV_BUF,       //��һ��ͼƬ
    GUILIST_NEXT_BUF,       //��һ��ͼƬ
    GUILIST_PREV_BG_BUF,    //��һ��ͼƬ����
    GUILIST_NEXT_BG_BUF,    //��һ��ͼƬ����
    
    GUILIST_BUF_MAX
}GUILIST_3D_BUF_E;

 /*! @struct GUILIST_SOFTKEY_TYPE_E
 @brief list softkey���ͣ��ı�id����ͼƬid ,Ĭ��֧���ı�id
 */
typedef enum
{
    GUILIST_SK_TYPE_TEXT_ID = 0,     /*!< ��ʾ�ı� */ 
    GUILIST_SK_TYPE_IMAGE_ID,    /*!< ��ʾicon */  

    GUILIST_SK_TYPE_MAX,
}GUILIST_SOFTKEY_TYPE_E;

typedef struct
{
	GUI_RECT_T      back_img_disp_rect;         // ������ʾ������
    GUI_COLOR_T     highlight_back_color;		// highlight background color 
    BOOLEAN         is_disp_hilight_image;      // �Ƿ���ʾ������ͼƬ
    MMI_IMAGE_ID_T  highlight_back_image;	    // highlight bkground image
	uint32			a_percent_lucency;			// ͼƬ����ɫ͸����
    uint8			left_space;					// left space 
    uint8			right_space;			    // right space 
    uint16          tag_width;                  // width of mark, check, radio, num, etc. 
    uint16          tag_img_width;              // icon width of mark, check, radio, num, etc. 
    uint16          avatar_checkbox_width;      // width of avatar check mark.
    uint16          radio_tag_width;            // width of radio mark.
    uint16          toggle_tag_width;           //width of toggle mark.
    uint32          text_time;                  // �����ƶ���ʱ�䣬�Ժ���Ϊ��λ
    MMI_IMAGE_ID_T  marked_image_id;            // ���ͼƬid	
    MMI_IMAGE_ID_T  avatar_marked_image_id;     // ?????id	
    MMI_IMAGE_ID_T  radio_selected_image_id;    // ��ѡѡ��ͼ��
    MMI_IMAGE_ID_T  radio_unselected_image_id;  // ��ѡδѡ��ͼ��
    MMI_IMAGE_ID_T  check_selected_image_id;    // ��ѡѡ��ͼ��
    MMI_IMAGE_ID_T  check_unselected_image_id;  // ��ѡδѡ��ͼ��
    MMI_IMAGE_ID_T  avatar_checkbox_selected_image_id;    // ?????????
    MMI_IMAGE_ID_T  avatar_checkbox_unselected_image_id;  // ???��??????
	MMI_IMAGE_ID_T  avatar_checkbox_unselected_avatar_image_1_id;
	MMI_IMAGE_ID_T  avatar_checkbox_unselected_avatar_image_2_id;
    MMI_IMAGE_ID_T  toggle_on_image_id; //???????
    MMI_IMAGE_ID_T  toggle_off_image_id;//??????
#ifdef GUIF_LIST_PDASTYLE
    MMI_IMAGE_ID_T  expand_image_id;            // ��չͼ�� ">"
	MMI_IMAGE_ID_T  delete_item_image_id;       // ��������ʾɾ��ͼ�� "-"
	MMI_IMAGE_ID_T  delete_query_image_id;      // �Ƿ�ɾ����ͼ�� "|"
	MMI_IMAGE_ID_T  add_item_image_id;          // ��Ӽ�¼ͼ�� "+"
#endif
    uint32			long_key_period;			//continuous move time period (UP ARROW or DOWN ARROW)
    MMI_IMAGE_ID_T  num_icon_id[GUIITEM_NUM_ICON_MAX];            //item number icon

    GUI_COLOR_T     item_font_color;   // �Զ���item color������Item״̬����ȡֵ

        
    GUILIST_SOFTKEY_TYPE_E  left_key_type;
    GUILIST_SOFTKEY_TYPE_E  mid_key_type;
    GUILIST_SOFTKEY_TYPE_E  right_key_type;
    BOOLEAN         is_custom_right_sk; //?????????????????????????????
    
    //soft key text id
    MMI_TEXT_ID_T   text_ok_id;
    MMI_TEXT_ID_T   text_return_id;
    MMI_TEXT_ID_T   text_cancel_id;
    MMI_TEXT_ID_T   text_select_id;
    MMI_TEXT_ID_T   text_mark_id;
    MMI_TEXT_ID_T   text_null_id;
    MMI_TEXT_ID_T   text_right_marked_id;

    MMI_IMAGE_ID_T   image_null_id;
    MMI_IMAGE_ID_T   image_ok_id;
    MMI_IMAGE_ID_T   image_return_id;
    MMI_IMAGE_ID_T   image_cancel_id;
    MMI_IMAGE_ID_T   image_select_id;
    MMI_IMAGE_ID_T   image_mark_id;
    MMI_IMAGE_ID_T   image_right_marked_id;

    MMI_TEXT_ID_T   text_empty_id;
    GUI_COLOR_T     text_empty_color;
    GUI_FONT_T      text_empty_font;

    MMI_IMAGE_ID_T  title_bg_image_id;
    MMI_IMAGE_ID_T  icon_default_id;
    MMI_IMAGE_ID_T  icon_error_id;
    GUI_COLOR_T     mist_color;   // ����Ч��������item����ʾһ�㱡���ɫ�ʣ�������item��
    
    uint32          auto_switch_time; // �Զ��л�
    uint32          slide_time;       // �����ƶ���ʱ�䣬�Ժ���Ϊ��λ
    MMI_IMAGE_ID_T  prgbar_image_id;  // �Զ����������ID

    // 
    GUI_BORDER_T    border;  // �ؼ��߿�
    GUI_BG_T        bg_info; // �ؼ�����

    BOOLEAN         is_slide_used;    /*!< �Ƿ�֧��slide*/

    // 
    GUI_BG_T        split_item_bg_info; // ��Ϊ�ָ�����item����
    GUI_COLOR_T     split_item_text_font_color; //????item????????????
    uint32          split_item_text_alpha;  //????item??????????????
	uint16			split_line_hight;		//???????????
    GUI_COLOR_T	split_line_color_with_label; //????label????????????
    GUI_COLOR_T     split_line_color;

    BOOLEAN         is_hl_disappear; // �Ƿ������ʧ

    //tips
    uint16          tip_hide_tick;
    MMI_IMAGE_ID_T  tip_bg;
    GUI_COLOR_T     tip_font_color;
    
    GUISOFTKEY_INDEX select_cancel_index; // checklist���͵�list�����л�cancel��select��ʾsoftkey�İ�ť������Ĭ�������

    //slide tip
    //MMI_IMAGE_ID_T  slide_top_tip_image_id;
    //MMI_IMAGE_ID_T  slide_bottom_tip_image_id;
    
    /*microsoft project need to support+++*/
    //???content??color style
    //THEMELIST_CONTENT_COLOR_STYLE_T      content_color_style[GUIITEM_CONTENT_MAX_NUM];    
    int16           custom_content_item;    //?????font color??content_item?????? 
    GUI_BORDER_T    content_border;         // content???
    //focus???????????????
    uint16          triangle_width;         //??????????
    MMI_IMAGE_ID_T  left_triangle_icon_id;  //??????????image id
    uint16          left_triangle_left_space;//??????????????
    MMI_IMAGE_ID_T  right_triangle_icon_id; //??????????image id
    uint16          right_triangle_right_space;//??????????????
    //Quick Action??
    uint16          quick_icon_bg_width;//quick????????????
    uint16          quick_icon_margin; //quick??????
    uint16          quick_icon_width;       //Quick??????
    MMI_IMAGE_ID_T  left_quick_icon_image;    //Qucik????image id
    MMI_IMAGE_ID_T  right_quick_icon_image;    //Qucik????image id
    uint16          quick_and_triangel_space;  //Quick????????????????

    MMI_IMAGE_ID_T  avatar_mask_image_id;   //item avatar mask icon    
    uint8			avatar_mask_left_space;	// left space 
    uint16          avatar_mask_width;      // mask width
    MMI_IMAGE_ID_T  initial_mask_image_id;   //item avatar mask icon    
    uint8			initial_mask_left_space; // left space 
    uint16          initial_mask_width;      // mask width 
    MMI_IMAGE_ID_T  simsel_icon_image;         //Quick Action icon for sim select
    uint16          simsel_width;           //QuickAction width for sim selection

    MMI_IMAGE_ID_T  image_accessory_id;    //Accessory image
    /*microsoft project need to support---*/
}MMITHEME_LIST_STYLE_T;

typedef enum
{
    GUILIST_TYPE_TEXT_ID, // �м���ʾ��������
    GUILIST_TYPE_ICON_ID  // �м���ʾ����ͼƬ
}GUILIST_CUSTDATA_TYPE_E;

/*! @struct GUILIST_OWNER_DRAW_DATA_T
@brief �ؼ��Ի�ص�����������Ϣ
*/
typedef struct 
{
    GUILIST_CUSTDATA_TYPE_E data_type;
    GUISTR_STYLE_T  str_style;     /*!< �ı���Դ��� */

    MMI_TEXT_ID_T   left_text_id;  /*!< �ı���ԴID */
    MMI_IMAGE_ID_T  left_icon_id;  /*!< ͼƬ��ԴID */

    MMI_TEXT_ID_T   right_text_id; /*!< �ı���ԴID */
    MMI_IMAGE_ID_T  right_icon_id; /*!< ͼƬ��ԴID */

} MMITHEME_LIST_CUSTDATA_T;

// THEMELIST_CONTENT_STYLE_T
typedef struct  
{
    GUIITEM_CONTENT_STATE_T state;

    GUI_RECT_T rect;
    GUI_RECT_T rect_focus;
    GUI_FONT_T font;
    GUI_FONT_T font_focus;

    uint8 scale_left;
    uint8 scale_right;    
} THEMELIST_CONTENT_STYLE_T;

// THEMELIST_ITEM_STYLE_T
typedef struct
{
    uint16 height; // common item height
    uint16 height_focus; // item height on focus state

    THEMELIST_CONTENT_STYLE_T content[GUIITEM_CONTENT_MAX_NUM]; // content style

    int16      main_index; // item main index, use for scroll, -1 is invalid value
    int16      sub_index;  // item auxiliary index, use for display string in multi lines,

    int16 base_width;
} THEMELIST_ITEM_STYLE_T;
typedef struct _MMITHEME_LIST_ITEMSTYLE_NODE
{
    MMI_HANDLE_T            m_hOwner;
    GUIITEM_STYLE_E         m_style_id;
    uint32                  m_style_state;
    THEMELIST_ITEM_STYLE_T  m_style;
    struct _MMITHEME_LIST_ITEMSTYLE_NODE*  m_next_ptr;
} MMITHEME_LIST_ITEMSTYLE_NODE_T, *MMITHEME_LIST_ITEMSTYLE_NODE_PTR;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get the item style state
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMITHEME_GetItemStyleState( 
//                                           GUIITEM_STYLE_E style,
//                                           GUIITEM_STYLE_STATE_T src_state
//                                           );

/*****************************************************************************/
// 	Description : get the list style, is called by the list control
//	Global resource dependence : 
//  Author:Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC const GUIITEM_STYLE_T* MMITHEME_GetItemStyle( GUIITEM_STYLE_E style );

/*****************************************************************************/
// 	Description : get the list style, is called by the list control
//	Global resource dependence : 
//  Author:Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC const GUIITEM_LAYOUT_DATA_T* MMITHEME_GetItemLayoutData(
                                                                GUIITEM_LAYOUT_ID_E layout_id,
                                                                GUIITEM_THEME_STATE_E state
                                                                );

/******************************************************************************/
/*! \brief get the list box style, is called by the lis list control
 *  \param[out]		listbox_style_ptr	pointer to list box style
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	get success
 * 		- <i><c>FALSE</c></i>:	get failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetListStyle(MMITHEME_LIST_STYLE_T* list_style_ptr);

#ifdef EFFECT_3D_LIST_ROLL_SUPPORT
/*****************************************************************************/
// 	Description : is need 3d list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsNeed3DList( void );

/*****************************************************************************/
// 	Description : alloc 3d list buffer
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Alloc3DListBuf(
                                    uint32 buf_size,
                                    MMI_HANDLE_T    win_handle
                                    );

/*****************************************************************************/
// 	Description : save 3d list buffer
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Save3DListBuf(
                                   GUILIST_3D_BUF_E type,
                                   GUI_RECT_T rect
                                   );

/*****************************************************************************/
// 	Description : set 3d list direction
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Set3DListDirection(
                                        BOOLEAN is_next
                                        );

/*****************************************************************************/
// 	Description : start 3d list anim
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Start3DListAnim( void );

/*****************************************************************************/
// 	Description : start 3d list anim
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Free3DListBuf( void );
#endif
#if 0
/*****************************************************************************/
// 	Description : display the list information
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayListIndex(
                                     const GUI_LCD_DEV_INFO* dev_info_ptr,
                                     uint16 cur_idx,  //[IN] current index, from 0----
                                     uint16 total_idx, //[IN] total index, from 1----
                                     MMI_IMAGE_ID_T title_bg_image_id,
                                     MMI_WIN_ID_T win_id
	                                 );
#endif
/*****************************************************************************/
// 	Description : get the list item backgroup image id
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetListItemBGImage(GUIITEM_THEME_BGIMAGE_TYPE_E item_bg_type);
#if 0
/*****************************************************************************/
// 	Description : get the list item backgroup color
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetListItemBGColor(BOOLEAN is_highlight);
#endif
/*****************************************************************************/
// 	Description : get the list item disp data
//	Global resource dependence : 
//  Author: xiaoqinglu
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetListItemDefData(MMITHEME_LIST_CUSTDATA_T* data_ptr);

/*****************************************************************************/
// 	Description : get the item style state
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetItemContentColor( 
                                                MMITHEME_COLOR_ID_E color_id
                                                );

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC THEMELIST_ITEM_STYLE_T *THEMELIST_GetStyle(GUIITEM_STYLE_E style_id);
/*****************************************************************************/
// 	Description : get content text color
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetContentTextColor( 
    GUIITEM_THEME_STATE_E state,
    const THEMELIST_ITEM_STYLE_T *item_style_ptr,
    uint16 content_index
);

/*****************************************************************************/
//  Description : get content text color
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T THEMELIST_GetSplitItemTextColor(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC int16 THEMELIST_GetListBaseWidth(void);

//============================================================================//
//                                   For watch                                //
//============================================================================//

/*****************************************************************************/
// 	Description : ����common List����
//             [In] None
//             [Out] None
//             [Return] bg_info : listItem background info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC GUI_BG_T WATCHTHEME_GetListItemBg(void);

/*****************************************************************************/
// 	Description : ����common List hightlight item ����
//             [In] None
//             [Out] None
//             [Return] bg_info : hightlight listItem background info
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC GUI_BG_T WATCHTHEME_GetListHightLightItemBg(void);
/*****************************************************************************/
//  Description : ע���µ�list iem style����
//  Global resource dependence : n/a
//  Author: dandan.cao
//  Note:
/*****************************************************************************/
PUBLIC GUIITEM_STYLE_E MMITHEME_CustomListItemStyleReg(MMI_HANDLE_T owner,                   //owner, ����list ptr, ����Ϊ��
                                                       THEMELIST_ITEM_STYLE_T* style_ptr,     //�Զ���style, ����Ϊ��
                                                       uint32    style_state          //�Զ���style_state, Ĭ��Ϊ0
                                                       );

/*****************************************************************************/
//  Description: ע��ָ��List�����Զ���item style����
//  Global resource dependence :
//  Author: dandan.cao
//  Note: �ɿؼ�destructʱ�Զ����ã�app�ɺ��Դ˺���
/*****************************************************************************/
PUBLIC void MMITHEME_CustomListItemStyleUnreg(uint32 owner);
//============================================================================//
//                                   For watch                                //
//============================================================================//

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_LIST_H_

    
