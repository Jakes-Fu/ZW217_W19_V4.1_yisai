/*******************************************************************************
** File Name: mmiim_base.h
** Author: Lianxiang.Zhou
** Date: 2009/08/08
** Copyright: 2009 Spreadtrum, Incorporated. All Rights Reserved.
** Description: 应用部分的公用定义
********************************************************************************
** Edit History
**------------------------------------------------------------------------------
** DATE         NAME            DESCRIPTION
** 2009/08/08   Lianxiang.Zhou  Create.
*******************************************************************************/

#ifndef _MMIIM_TP_BASE_H_
#define _MMIIM_TP_BASE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"

#if defined(MMIIM_USE_TOUCH_PANEL)

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//const def
enum
{
    MMIIM_TP_KEY_PER_ROW_MAX = 13,
    MMIIM_TP_TAG_MAX_LEN = 8 + 1, // 1 是 '\0'
    MMIIM_TP_SHIFT_VKEY_MAX = 5,
    //定义按键的高度及间距, pixel
    MMIIM_TP_VKEY_HEIGHT = 16,
    MMIIM_TP_VKEY_SPACE = 3,
};


/*
关于这个系列的定义，从0xe0000开始，是因为这段unicode是空白的，可以干净的无影响
这些特殊的character是输入法应用可以传给输入法控件处理的
因为在触笔输入的时候，一些特殊的按键或者按钮，如果统一处理的话，会简化程序
*/
/*
每一类的MAX都是下一类的起点，也就是说MAX在本类是无效的，是下一类的"零"
*/
#define MMIIM_TP_TAG_START (0xE000)

//组合字母类，用于快捷输入
#define MMIIM_TP_TAG_WORD_START (MMIIM_TP_TAG_START)
enum
{
    MMIIM_TP_TAG_WORD_DCOM = MMIIM_TP_TAG_WORD_START,
        
    MMIIM_TP_TAG_WORD_MAX
};


//切换类
#define MMIIM_TP_TAG_SHFIT_START (MMIIM_TP_TAG_WORD_MAX)
enum
{
    //同一种语言最多会切换几种键盘?5个应该够了
    MMIIM_TP_TAG_SHIFT_0 = MMIIM_TP_TAG_SHFIT_START,
    MMIIM_TP_TAG_SHIFT_1,
    MMIIM_TP_TAG_SHIFT_2,
    MMIIM_TP_TAG_SHIFT_3,
    MMIIM_TP_TAG_SHIFT_4,
    //大小写切换
    MMIIM_TP_TAG_SHIFT_CAP,
    //切换到符号
    MMIIM_TP_TAG_SHIFT_SYMBOL,
    //返回上一个键盘，上一个键盘是系统记忆的
    MMIIM_TP_TAG_SHIFT_BACK,
    //hindi vkb
    MMIIM_TP_TAG_SHIFT_HINDI_M1,
    MMIIM_TP_TAG_SHIFT_HINDI_M2,
    MMIIM_TP_TAG_SHIFT_HINDI_V1,
    MMIIM_TP_TAG_SHIFT_HINDI_V2,

    MMIIM_TP_TAG_SHFIT_MAX,
};

//语言选择类
#define MMIIM_TP_TAG_LANG_START (MMIIM_TP_TAG_SHFIT_MAX)
enum
{
    MMIIM_TP_TAG_LANG_ARABIC = MMIIM_TP_TAG_LANG_START,
    MMIIM_TP_TAG_LANG_CHS,
    MMIIM_TP_TAG_LANG_ENGLISH,
    MMIIM_TP_TAG_LANG_FRENCH,
    MMIIM_TP_TAG_LANG_GERMAN,
    MMIIM_TP_TAG_LANG_GREEK,
    MMIIM_TP_TAG_LANG_HINDI,
    MMIIM_TP_TAG_LANG_HUNGARIAN,
    MMIIM_TP_TAG_LANG_INDONESIAN,
    MMIIM_TP_TAG_LANG_ITALIAN,
    MMIIM_TP_TAG_LANG_MALAY,
    MMIIM_TP_TAG_LANG_PERSIAN,
    MMIIM_TP_TAG_LANG_PORTUGUESE,
    MMIIM_TP_TAG_LANG_RUSSIAN,
    MMIIM_TP_TAG_LANG_SPANISH,
    MMIIM_TP_TAG_LANG_TAGALOG,
    MMIIM_TP_TAG_LANG_THAI,
    MMIIM_TP_TAG_LANG_TURKISH,
    MMIIM_TP_TAG_LANG_URDU,
    MMIIM_TP_TAG_LANG_VIETNAMESE,
    
    MMIIM_TP_TAG_LANG_MAX,
};

//特殊字符类
#define MMIIM_TP_TAG_CHAR_START (MMIIM_TP_TAG_LANG_MAX)
enum
{
    MMIIM_TP_TAG_CHAR_123 = MMIIM_TP_TAG_CHAR_START,
    MMIIM_TP_TAG_CHAR_ABC,
    MMIIM_TP_TAG_CHAR_PINYIN,
    MMIIM_TP_TAG_CHAR_VKEYBOARD,
    MMIIM_TP_TAG_CHAR_SYMBOL,
    
    MMIIM_TP_TAG_CHAR_MAX,
};

//控制字符类
#define MMIIM_TP_TAG_CTRL_START (MMIIM_TP_TAG_CHAR_MAX)
enum
{
    MMIIM_TP_TAG_CTRL_COMBINE = MMIIM_TP_TAG_CTRL_START,

    MMIIM_TP_TAG_CTRL_MAX,
};

//外部跳转类
#define MMIIM_TP_TAG_SWITCH_START (MMIIM_TP_TAG_CTRL_MAX)
enum
{
    MMIIM_TP_TAG_SWITCH_HANDWRITING = MMIIM_TP_TAG_SWITCH_START,

    MMIIM_TP_TAG_SWITCH_MAX,
};


//-----------新的键盘定义--------------
/*
分为两部分，一部分是定义键盘部分，一部分是解析定义后生成的数据
*/
typedef struct
{
    uint8 max_keys;
    uint8 fact_keys;
    wchar keys[MMIIM_TP_KEY_PER_ROW_MAX];
} const MMIIM_TP_VKEY_ROW_T;

typedef struct
{
    MMIIM_TP_VKEY_ROW_T *vkey_rows;
    int32 row_count;
    wchar const *tag_ptr;
    int32 tag_count;
} const MMIIM_TP_VKEY_LAYOUT_T;

/* 特殊字符显示的字符串结构定义 */
typedef struct
{
    wchar tag;
    wchar str[MMIIM_TP_TAG_MAX_LEN];
}MMIIM_TP_TAG_STRING_T;

//定义键状态
enum
{
    MMIIM_TP_KEY_STATE_INACTIVE = 0x00, //不可用状态，即灰化
    MMIIM_TP_KEY_STATE_ACTIVE = 0x01, //正常状态
    MMIIM_TP_KEY_STATE_SELECT = 0x02, //被选定
    MMIIM_TP_KEY_STATE_PRESS = 0x04, //被按下

    MMIIM_TP_KEY_STATE_MAX,
};
typedef uint8 MMIIM_TP_KEY_STATE_T;

enum
{
    //按键
    MMIIM_TP_KEY_TYPE_NORMAL,
    //特殊字符，需要特别显示，比如空格、换行、删除等
    MMIIM_TP_KEY_TYPE_SPECIAL_CHAR,
    //组合字母
    MMIIM_TP_KEY_TYPE_WORD,
    //语言切换
    MMIIM_TP_KEY_TYPE_LANG,
    //键盘切换
    MMIIM_TP_KEY_TYPE_VKEY,
    //输入法切换
    MMIIM_TP_KEY_TYPE_IM,
    //提示标签
    MMIIM_TP_KEY_TYPE_TAG,
    //控制块
    MMIIM_TP_KEY_TYPE_CTRL,
    //跳转块
    MMIIM_TP_KEY_TYPE_SWITCH,


    MMIIM_TP_KEY_TYPE_MAX,
};
typedef uint8 MMIIM_TP_KEY_TYPE_T;

typedef struct
{
    MMIIM_TP_KEY_STATE_T state;
    MMIIM_TP_KEY_TYPE_T type;
    wchar disp_char;
    GUI_RECT_T rect;
}MMIIM_TP_KEY_T;

typedef struct
{
    BOOLEAN is_initialized;
    GUI_RECT_T rect;
    int32 key_count;
    MMIIM_TP_VKEY_LAYOUT_T *layout_ptr;
    MMIIM_TP_KEY_T *keys_ptr;
    MMIIM_TP_VKEY_LAYOUT_T *auto_back_layout_ptr;
    wchar last_press_key;
}MMIIM_TP_VKB_T;

//定义输入法对应的初始化键盘
typedef struct
{
    GUIIM_METHOD_T method;
    MMIIM_TP_VKEY_LAYOUT_T *layout_ptr;
}MMIIM_TP_VKB_CFG_T;

typedef struct
{
    GUI_RECT_T rect_tips;
    GUI_COLOR_T color_tips_bg;
    GUI_COLOR_T color_font_tips;
    GUI_FONT_T font_tips;

    GUI_COLOR_T color_panel_bg;
    
    GUI_COLOR_T color_bg_inactive[4];
    
    GUI_COLOR_T color_bg_blk[4];
    GUI_COLOR_T color_bg_blk_press[4];

    GUI_COLOR_T color_bg_tag[4];
    GUI_COLOR_T color_bg_tag_press[4];
    GUI_COLOR_T color_bg_tag_select[4];

    GUI_COLOR_T color_font_default;
    GUI_COLOR_T color_font_normal;
    GUI_COLOR_T color_font_tag;
    
    GUI_FONT_T font_default;
    GUI_FONT_T font_normal;
    GUI_FONT_T font_tag;
}MMIIM_TP_VKEY_UI_T;

typedef struct
{
    MMIIM_TP_VKEY_LAYOUT_T * layout_from_ptr;
    wchar disp_char;
    MMIIM_TP_VKEY_LAYOUT_T * layout_to_ptr;
}MMIIM_TP_VKB_SWITCH_T;

typedef struct
{
    wchar disp_char;
    GUIIM_METHOD_T method;
}MMIIM_TP_VKEY_IM_T;

typedef struct
{
    wchar disp_char;
    MMIIM_TP_VKEY_LAYOUT_T * layout_ptr;
}MMIIM_TP_VKEY_VKB_T;

//-----------新的键盘定义--------------
extern MMIIM_TP_VKEY_UI_T const g_mmiim_tp_ui_cfg;

/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus 
extern "C"{ 
#endif 

/******************************************************************************/
// Description: 由键盘布局得到键盘的初始化数据
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIIM_ConstructVkb(
                                  MMIIM_TP_VKB_T *vkb_ptr,
                                  MMIIM_TP_VKEY_LAYOUT_T *layout_ptr,
                                  GUI_RECT_T *rect_ptr
                                  );

/******************************************************************************/
// Description: 销毁键盘
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DestroyVkb(MMIIM_TP_VKB_T *vkb_ptr);

/******************************************************************************/
// Description: 绘制键盘
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawVkb(
                          GUI_LCD_DEV_INFO *lcd_info_ptr,
                          MMIIM_TP_VKB_T *vkb_ptr
                          );

/******************************************************************************/
// Description: 画出按键
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_DrawKey(GUI_LCD_DEV_INFO *lcd_info_ptr, MMIIM_TP_KEY_T *key_ptr);

/******************************************************************************/
// Description: 清除键盘的键盘的某个状态
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ClearVkbState(
                                MMIIM_TP_VKB_T *vkb_ptr,
                                MMIIM_TP_KEY_STATE_T state
                                );

/******************************************************************************/
// Description: 设定键盘上的某个键为指定状态
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_KeyStateSet(
                              MMIIM_TP_VKB_T *vkb_ptr,
                              wchar c,
                              MMIIM_TP_KEY_STATE_T state
                              );

/******************************************************************************/
// Description: 设定键盘上的某个键的指定状态
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_KeyStateUnset(
                                MMIIM_TP_VKB_T *vkb_ptr,
                                wchar c,
                                MMIIM_TP_KEY_STATE_T state
                                );

/******************************************************************************/
// Description: 得到触笔点击的按键
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_KEY_T* MMIIM_GetPenKey(
                                       MMIIM_TP_VKB_T *vkb_ptr,
                                       GUI_POINT_T const *point_ptr
                                       );

/******************************************************************************/
// Description: 得到触笔点击的按键
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_KEY_T* MMIIM_GetKeyByChar(
                                          MMIIM_TP_VKB_T *vkb_ptr,
                                          wchar c
                                          );

/******************************************************************************/
// Description: 由输入法获得键盘布局
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetVkbLayout(GUIIM_METHOD_T method);

/******************************************************************************/
// Description: 获取切换键盘的布局
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetSwitchLayout(
                                                      MMIIM_TP_VKEY_LAYOUT_T *layout_ptr,
                                                      wchar disp_char
                                                      );

/******************************************************************************/
// Description: 获取切换键盘的布局
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC MMIIM_TP_VKEY_LAYOUT_T * MMIIM_GetVkeyLayout(wchar disp_char);

/******************************************************************************/
// Description: 获取按键对应的输入法
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC GUIIM_METHOD_T MMIIM_GetVkeyIm(wchar disp_char);

/******************************************************************************/
// Description: 显示按键的提示框
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_ShowTips(
                           GUI_LCD_DEV_INFO const *lcd_info_ptr,
                           MMIIM_TP_VKB_T const *vkb_ptr,
                           MMIIM_TP_KEY_T const *key_ptr
                           );

/******************************************************************************/
// Description: 隐藏按键的提示框
// Global resource dependence: NONE
// Author: Lianxiang.Zhou
// Note:
/******************************************************************************/
PUBLIC void MMIIM_HideTips(GUI_LCD_DEV_INFO const *lcd_info_ptr);


#ifdef __cplusplus 
}
#endif

#endif //MMIIM_USE_TOUCH_PANEL

#endif //_MMIIM_TP_BASE_H_




