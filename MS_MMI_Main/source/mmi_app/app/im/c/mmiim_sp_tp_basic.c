/*==============================================================================
File Name: mmiim_sp_tp_basic.c
Author: Lianxiang.Zhou
Date: 2010/05/17
Copyright: 2010 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2010/05/17   Lianxiang.Zhou  Create.
==============================================================================*/

#include "mmi_app_im_trc.h"
#include "mmiim_sp_tp_basic.h"

#include "guires.h"
#include "guistring.h"
#include "mmi_default.h"
#include "mmiim.h"


/*==============================================================================
Function Name: _OffsetRect
Description: _OffsetRect
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL __inline void _OffsetRect(GUI_RECT_T *rect_ptr, GUI_POINT_T *offset_ptr)
{
    rect_ptr->left += offset_ptr->x;
    rect_ptr->right += offset_ptr->x;
    rect_ptr->top += offset_ptr->y;
    rect_ptr->bottom += offset_ptr->y;

    return;
}

#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))
extern MMIIM_SP_TEXT_BAR_THEME_T g_text_bar_theme;

/*==============================================================================
Function Name: KbEn26KeyInit
Description: KbEn26KeyInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: KbEn26KeyUpper
Description: KbEn26KeyUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyUpper(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: KbEn26KeyLower
Description: KbEn26KeyLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyLower(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: Kb123Init
Description: Kb123Init
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void Kb123Init(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: Kb123SymbInit
Description: Kb123SymbInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void Kb123SymbInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: KbHwInit
Description: KbHwInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbHwInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: KbStrokeInit
Description: KbStrokeInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbStrokeInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Description: init 9key
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9keyInit(
                        MMIIM_SP_TP_KB_INFO_T *kb_ptr   // [in] key info
                        );

/*==============================================================================
Description: KbEn9KeyUpper
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9KeyUpper(
                         MMIIM_SP_TP_KB_INFO_T *kb_ptr      // [in] key info
                         );

/*==============================================================================
Description: KbEn9KeyLower
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9KeyLower(
                         MMIIM_SP_TP_KB_INFO_T *kb_ptr      // [in] key info
                         );
#endif

/*==============================================================================
Function Name: DrawRectGradient
Description: 描绘一个颜色渐变的矩形
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void DrawRectGradient(
    GUI_LCD_DEV_INFO *lcd_info_ptr,
    GUI_RECT_T *rect_ptr,
    GUI_COLOR_T c0,
    GUI_COLOR_T c1
);

#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))
/*lint -e785*/

////////////////////////////////////////////////////////////////////////////////
//en26key keyborad
enum
{
    en26key_w = 240, //键盘宽高
    en26key_h = 123,
    en26key_w_k_c = 22, //字母键宽
    en26key_w_k_t = 46, //标签键宽
    en26key_h_k = 28, //键高
    en26key_s = 1, //键间距
    en26key_x0 = 1, //键排布正常xy坐标
    en26key_x1 = 25,
    en26key_x2 = 49,
    en26key_x3 = 73,
    en26key_x4 = 97,
    en26key_x5 = 121,
    en26key_x6 = 145,
    en26key_x7 = 169,
    en26key_x8 = 193,
    en26key_x9 = 217,
    en26key_y0 = 5,
    en26key_y1 = 35,
    en26key_y2 = 65,
    en26key_y3 = 95
};


//en 26key 的布局，默认用小写的英文输入的布局
//其他使用相同键盘的则需要对特定的按键做键值设置
LOCAL MMIIM_SP_TP_KEY_INFO_T const en26key_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{en26key_x0, en26key_y0, en26key_x0 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'q'},
    {{en26key_x1, en26key_y0, en26key_x1 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'w'},
    {{en26key_x2, en26key_y0, en26key_x2 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'e'},
    {{en26key_x3, en26key_y0, en26key_x3 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'r'},
    {{en26key_x4, en26key_y0, en26key_x4 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 't'},
    {{en26key_x5, en26key_y0, en26key_x5 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'y'},
    {{en26key_x6, en26key_y0, en26key_x6 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'u'},
    {{en26key_x7, en26key_y0, en26key_x7 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'i'},
    {{en26key_x8, en26key_y0, en26key_x8 + en26key_w_k_c, en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'o'},
    {{en26key_x9, en26key_y0, en26key_w - en26key_s,      en26key_y0 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'p'},

    {{en26key_x0, en26key_y1, en26key_x0 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'a'},
    {{en26key_x1, en26key_y1, en26key_x1 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 's'},
    {{en26key_x2, en26key_y1, en26key_x2 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'd'},
    {{en26key_x3, en26key_y1, en26key_x3 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'f'},
    {{en26key_x4, en26key_y1, en26key_x4 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'g'},
    {{en26key_x5, en26key_y1, en26key_x5 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'h'},
    {{en26key_x6, en26key_y1, en26key_x6 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'j'},
    {{en26key_x7, en26key_y1, en26key_x7 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'k'},
    {{en26key_x8, en26key_y1, en26key_x8 + en26key_w_k_c, en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'l'},
    {{en26key_x9, en26key_y1, en26key_w - en26key_s,      en26key_y1 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\n'},

    {{en26key_x0, en26key_y2, en26key_x0 + en26key_w_k_t, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_CAP},
    //
    {{en26key_x2, en26key_y2, en26key_x2 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'z'},
    {{en26key_x3, en26key_y2, en26key_x3 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'x'},
    {{en26key_x4, en26key_y2, en26key_x4 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'c'},
    {{en26key_x5, en26key_y2, en26key_x5 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'v'},
    {{en26key_x6, en26key_y2, en26key_x6 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'b'},
    {{en26key_x7, en26key_y2, en26key_x7 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'n'},
    {{en26key_x8, en26key_y2, en26key_x8 + en26key_w_k_c, en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'm'},
    {{en26key_x9, en26key_y2, en26key_w - en26key_s,      en26key_y2 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},

    {{en26key_x0, en26key_y3, en26key_x0 + en26key_w_k_t, en26key_y3 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    //
    {{en26key_x2, en26key_y3, en26key_x2 + en26key_w_k_t, en26key_y3 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_KB},
    //
    {{en26key_x4, en26key_y3, en26key_x4 + en26key_w_k_t, en26key_y3 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    //
    {{en26key_x6, en26key_y3, en26key_x6 + en26key_w_k_t, en26key_y3 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_DIGITAL},
    //
    {{en26key_x8, en26key_y3, en26key_w - en26key_s,      en26key_y3 + en26key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ' '},
    //
};


////////////////////////////////////////////////////////////////////////////////
//digital keyborad
enum
{
    digital_w = 240, //键盘宽高
    digital_h = 123,
    digital_w_k = 47, //字母键宽
    digital_w_t = 95, //字母键宽
    digital_h_k = 40, //键高
    digital_h_t = 28, //标签高
    digital_s = 1, //键间距
    digital_x0 = 1, //键排布正常xy坐标
    digital_x1 = 49,
    digital_x2 = 97,
    digital_x3 = 145,
    digital_x4 = 193,
    digital_y0 = 5,
    digital_y1 = 50,
    digital_y2 = 95
};


LOCAL MMIIM_SP_TP_KEY_INFO_T const digital_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{digital_x0, digital_y0, digital_x0 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '1'},
    {{digital_x1, digital_y0, digital_x1 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '2'},
    {{digital_x2, digital_y0, digital_x2 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '3'},
    {{digital_x3, digital_y0, digital_x3 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '4'},
    {{digital_x4, digital_y0, digital_w - digital_s,    digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '5'},

    {{digital_x0, digital_y1, digital_x0 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '6'},
    {{digital_x1, digital_y1, digital_x1 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '7'},
    {{digital_x2, digital_y1, digital_x2 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '8'},
    {{digital_x3, digital_y1, digital_x3 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '9'},
    {{digital_x4, digital_y1, digital_w - digital_s,    digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '0'},

    {{digital_x0, digital_y2, digital_x0 + digital_w_k, digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{digital_x1, digital_y2, digital_x1 + digital_w_t, digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    //
    {{digital_x3, digital_y2, digital_w - digital_s,    digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},
    //
};

LOCAL MMIIM_SP_TP_KEY_INFO_T const digital_symb_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{digital_x0, digital_y0, digital_x0 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '('},
    {{digital_x1, digital_y0, digital_x1 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ')'},
    {{digital_x2, digital_y0, digital_x2 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '#'},
    {{digital_x3, digital_y0, digital_x3 + digital_w_k, digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ','},
    {{digital_x4, digital_y0, digital_w - digital_s,    digital_y0 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '.'},

    {{digital_x0, digital_y1, digital_x0 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '*'},
    {{digital_x1, digital_y1, digital_x1 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '-'},
    {{digital_x2, digital_y1, digital_x2 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '+'},
    {{digital_x3, digital_y1, digital_x3 + digital_w_k, digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'P'},
    {{digital_x4, digital_y1, digital_w - digital_s,    digital_y1 + digital_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, 'W'},

    {{digital_x0, digital_y2, digital_x0 + digital_w_k, digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{digital_x1, digital_y2, digital_x1 + digital_w_t, digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_LOCK},
    //
    {{digital_x3, digital_y2, digital_w - digital_s,    digital_y2 + digital_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},
    //
};


////////////////////////////////////////////////////////////////////////////////
//hw keyborad
#if defined (MAINLCD_SIZE_320X480)
enum
{
    hw_w = 320, //键盘宽高
    hw_h = 104,
    hw_w_k = 62, //标签键宽
    hw_h_k = 50, //键高
    hw_s = 2, //键间距
    hw_x0 = 1, //键排布正常xy坐标
    hw_x1 = 65,
    hw_x2 = 129,
    hw_x3 = 193,
    hw_x4 = 257,
    hw_y0 = 2,
    hw_y1 = 55
};
#elif defined (MAINLCD_SIZE_176X220)
enum
{
    hw_w = 176, //键盘宽高
    hw_h = 41,
    hw_w_k = 33, //标签键宽
    hw_h_k = 18, //键高
    hw_s = 1, //键间距
    hw_x0 = 4, //键排布正常xy坐标
    hw_x1 = 38,
    hw_x2 = 72,
    hw_x3 = 106,
    hw_x4 = 140,
    hw_y0 = 3,
    hw_y1 = 21
};
#elif defined (MAINLCD_SIZE_240X400)//横屏的时候，QVGA与HQVGA不一样
enum
{
    hw_w = 240, //键盘宽高
    hw_h = 67,
    hw_w_k = 44, //标签键宽
    hw_h_k = 30, //键高
    hw_s = 2, //键间距
    hw_x0 = 6, //键排布正常xy坐标
    hw_x1 = 52,
    hw_x2 = 98,
    hw_x3 = 144,
    hw_x4 = 190,
    hw_y0 = 3,
    hw_y1 = 35
};
#else
enum
{
    hw_w = 240, //键盘宽高
    hw_h = 80,
    hw_w_k = 46, //标签键宽
    hw_h_k = 35, //键高
    hw_s = 2, //键间距
    hw_x0 = 1, //键排布正常xy坐标
    hw_x1 = 49,
    hw_x2 = 97,
    hw_x3 = 145,
    hw_x4 = 193,
    hw_y0 = 5,
    hw_y1 = 43
};
#endif

LOCAL MMIIM_SP_TP_KEY_INFO_T const hw_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{hw_x0, hw_y0, hw_x0 + hw_w_k, hw_y0 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    {{hw_x1, hw_y0, hw_x1 + hw_w_k, hw_y0 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_DIGITAL},
    {{hw_x2, hw_y0, hw_x2 + hw_w_k, hw_y0 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '?'},
    {{hw_x3, hw_y0, hw_w - hw_s,    hw_y0 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},
    //

    {{hw_x0, hw_y1, hw_x0 + hw_w_k, hw_y1 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{hw_x1, hw_y1, hw_x1 + hw_w_k, hw_y1 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ','},
    {{hw_x2, hw_y1, hw_x2 + hw_w_k, hw_y1 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '.'},
    {{hw_x3, hw_y1, hw_x3 + hw_w_k, hw_y1 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\n'},
    {{hw_x4, hw_y1, hw_w - hw_s,    hw_y1 + hw_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ' '},
};

//手写的横屏
#if defined (MAINLCD_SIZE_320X480)
enum
{
    hw_w_h = 480, //键盘宽高
    hw_h_h = 58,
    hw_w_k_h = 94, //标签键宽
    hw_h_k_h = 27, //键高
    hw_s_h = 2, //键间距
    hw_x0_h = 1, //键排布正常xy坐标
    hw_x1_h = 97,
    hw_x2_h = 193,
    hw_x3_h = 289,
    hw_x4_h = 385,
    hw_y0_h = 3,
    hw_y1_h = 31
};
#elif defined (MAINLCD_SIZE_176X220)
enum
{
    hw_w_h = 220, //键盘宽高
    hw_h_h = 33,
    hw_w_k_h = 41, //标签键宽
    hw_h_k_h = 14, //键高
    hw_s_h = 1, //键间距
    hw_x0_h = 6, //键排布正常xy坐标
    hw_x1_h = 48,
    hw_x2_h = 90,
    hw_x3_h = 132,
    hw_x4_h = 174,
    hw_y0_h = 3,
    hw_y1_h = 18
};
#elif defined (MAINLCD_SIZE_240X400)//横屏的时候，QVGA与HQVGA不一样
enum
{
    hw_w_h = 400, //键盘宽高
    hw_h_h = 47,
    hw_w_k_h = 68, //标签键宽
    hw_h_k_h = 21, //键高
    hw_s_h = 4, //键水平间距
    hw_x0_h = 22, //键排布正常xy坐标
    hw_x1_h = 94,
    hw_x2_h = 166,
    hw_x3_h = 238,
    hw_x4_h = 310,
    hw_y0_h = 3,
    hw_y1_h = 26
};
#else
enum
{
    hw_w_h = 320, //键盘宽高
    hw_h_h = 38,
    hw_w_k_h = 59, //标签键宽
    hw_h_k_h = 16, //键高
    hw_s_h = 2, //键水平间距
    hw_x0_h = 7, //键排布正常xy坐标
    hw_x1_h = 69,
    hw_x2_h = 131,
    hw_x3_h = 193,
    hw_x4_h = 255,
    hw_y0_h = 3,
    hw_y1_h = 21
};
#endif

LOCAL MMIIM_SP_TP_KEY_INFO_T const hw_layout_h[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{hw_x0_h, hw_y0_h, hw_x0_h + hw_w_k_h, hw_y0_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    {{hw_x1_h, hw_y0_h, hw_x1_h + hw_w_k_h, hw_y0_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_DIGITAL},
    {{hw_x2_h, hw_y0_h, hw_x2_h + hw_w_k_h, hw_y0_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '?'},
    {{hw_x3_h, hw_y0_h, hw_w_h - hw_s_h,    hw_y0_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},
    //

    {{hw_x0_h, hw_y1_h, hw_x0_h + hw_w_k_h, hw_y1_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{hw_x1_h, hw_y1_h, hw_x1_h + hw_w_k_h, hw_y1_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ','},
    {{hw_x2_h, hw_y1_h, hw_x2_h + hw_w_k_h, hw_y1_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '.'},
    {{hw_x3_h, hw_y1_h, hw_x3_h + hw_w_k_h, hw_y1_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\n'},
    {{hw_x4_h, hw_y1_h, hw_w_h - hw_s_h,    hw_y1_h + hw_h_k_h}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ' '},
};

////////////////////////////////////////////////////////////////////////////////
//stroke keyborad
enum
{
    stroke_w = 240, //键盘宽高
    stroke_h = 123,
    stroke_w_k = 62, //字母键宽
    stroke_w_t1 = 46, //标签键宽
    stroke_w_t2 = 63, //标签键宽
    stroke_h_k = 43, //键高
    stroke_h_t = 28, //下排标签
    stroke_s = 1, //键间距
    stroke_x_k0 = 1,
    stroke_x_k1 = 65,
    stroke_x_k2 = 129,
    stroke_x_k3 = 193,
    stroke_x_t0 = 1,
    stroke_x_t1 = 49,
    stroke_x_t2 = 113,
    stroke_x_t3 = 177,
    stroke_y0 = 5,
    stroke_y1 = 50,
    stroke_y2 = 95
};


LOCAL MMIIM_SP_TP_KEY_INFO_T const stroke_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    {{stroke_x_k0, stroke_y0, stroke_x_k0 + stroke_w_k, stroke_y0 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '1'},
    {{stroke_x_k1, stroke_y0, stroke_x_k1 + stroke_w_k, stroke_y0 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '2'},
    {{stroke_x_k2, stroke_y0, stroke_x_k2 + stroke_w_k, stroke_y0 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '3'},
    {{stroke_x_k3, stroke_y0, stroke_w - stroke_s,      stroke_y0 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},

    {{stroke_x_k0, stroke_y1, stroke_x_k0 + stroke_w_k, stroke_y1 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '4'},
    {{stroke_x_k1, stroke_y1, stroke_x_k1 + stroke_w_k, stroke_y1 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '5'},
    {{stroke_x_k2, stroke_y1, stroke_x_k2 + stroke_w_k, stroke_y1 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '6'},
    {{stroke_x_k3, stroke_y1, stroke_w - stroke_s,      stroke_y1 + stroke_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\n'},

    {{stroke_x_t0, stroke_y2, stroke_x_t0 + stroke_w_t1, stroke_y2 + stroke_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{stroke_x_t1, stroke_y2, stroke_x_t1 + stroke_w_t2, stroke_y2 + stroke_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    {{stroke_x_t2, stroke_y2, stroke_x_t2 + stroke_w_t2, stroke_y2 + stroke_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_DIGITAL},
    {{stroke_x_t3, stroke_y2, stroke_w - stroke_s,       stroke_y2 + stroke_h_t}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ' '},
};


////////////////////////////////////////////////////////////////////////////////
// 9key
enum
{
    en9key_w    = 240,  // panel width
    en9key_h    = 123,  // panel height

    en9key_w_k  = 46,   // button width
    en9key_h_k  = 28,   // button height
    en9key_x_k0 = 49,   // button x start position
    en9key_x_k1 = 97,
    en9key_x_k2 = 145,
    en9key_x_k3 = 193,
    en9key_y_k0 = 4,    // button y start position
    en9key_y_k1 = 34,
    en9key_y_k2 = 64,
    en9key_y_k3 = 94,

    en9key_w_t  = 43,   // text width
    en9key_h_t  = 20,   // text height
    en9key_x_t0 = 4,    // text x start position
    en9key_y_t0 = 22,   // text y start position
    en9key_y_t1 = 43,
    en9key_y_t2 = 64,
    en9key_y_t3 = 85,

    en9key_w_a  = 45,   // arrow width
    en9key_h_a  = 16,   // arrow height
    en9key_x_a  = 2,    // arrow x
    en9key_y_a0 = 6,    // top arrow y
    en9key_y_a1 = 104  // bottom arrow y
};

LOCAL MMIIM_SP_TP_KEY_INFO_T const en9key_layout[] =
{
    //rect(l t r b) state ch, img通常都不设置，在初始化键盘的时候设置
    // 1 line
    {{en9key_x_k0, en9key_y_k0, en9key_x_k0 + en9key_w_k, en9key_y_k0 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_CAP},
    {{en9key_x_k1, en9key_y_k0, en9key_x_k1 + en9key_w_k, en9key_y_k0 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '2'},
    {{en9key_x_k2, en9key_y_k0, en9key_x_k2 + en9key_w_k, en9key_y_k0 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '3'},
    {{en9key_x_k3, en9key_y_k0, en9key_x_k3 + en9key_w_k, en9key_y_k0 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\b'},

    // 2 line
    {{en9key_x_k0, en9key_y_k1, en9key_x_k0 + en9key_w_k, en9key_y_k1 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '4'},
    {{en9key_x_k1, en9key_y_k1, en9key_x_k1 + en9key_w_k, en9key_y_k1 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '5'},
    {{en9key_x_k2, en9key_y_k1, en9key_x_k2 + en9key_w_k, en9key_y_k1 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '6'},
    {{en9key_x_k3, en9key_y_k1, en9key_x_k3 + en9key_w_k, en9key_y_k1 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '\n'},

    // 3 line
    {{en9key_x_k0, en9key_y_k2, en9key_x_k0 + en9key_w_k, en9key_y_k2 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '7'},
    {{en9key_x_k1, en9key_y_k2, en9key_x_k1 + en9key_w_k, en9key_y_k2 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '8'},
    {{en9key_x_k2, en9key_y_k2, en9key_x_k2 + en9key_w_k, en9key_y_k2 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, '9'},
    {{en9key_x_k3, en9key_y_k2, en9key_x_k3 + en9key_w_k, en9key_y_k2 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, ' '},

    // 4 line
    {{en9key_x_k0, en9key_y_k3, en9key_x_k0 + en9key_w_k, en9key_y_k3 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_IM},
    {{en9key_x_k1, en9key_y_k3, en9key_x_k1 + en9key_w_k, en9key_y_k3 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SHIFT_KB},
    {{en9key_x_k2, en9key_y_k3, en9key_x_k2 + en9key_w_k, en9key_y_k3 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_SYMBOL},
    {{en9key_x_k3, en9key_y_k3, en9key_x_k3 + en9key_w_k, en9key_y_k3 + en9key_h_k}, MMIIM_SP_TP_KEY_STATE_ACTIVE, MMIIM_SP_TP_TAG_DIGITAL},

    // arrow
    {{en9key_x_a, en9key_y_a0, en9key_x_a + en9key_w_a, en9key_y_a0 + en9key_h_a}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_TOPARROW},
    {{en9key_x_a, en9key_y_a1, en9key_x_a + en9key_w_a, en9key_y_a1 + en9key_h_a}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_BOTTOMARROW},

    // text region
    {{en9key_x_t0, en9key_y_t0, en9key_x_t0 + en9key_w_t, en9key_y_t0 + en9key_h_t}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_TEXT0},
    {{en9key_x_t0, en9key_y_t1, en9key_x_t0 + en9key_w_t, en9key_y_t1 + en9key_h_t}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_TEXT1},
    {{en9key_x_t0, en9key_y_t2, en9key_x_t0 + en9key_w_t, en9key_y_t2 + en9key_h_t}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_TEXT2},
    {{en9key_x_t0, en9key_y_t3, en9key_x_t0 + en9key_w_t, en9key_y_t3 + en9key_h_t}, MMIIM_SP_TP_KEY_STATE_INACTIVE, MMIIM_SP_TP_TAG_TEXT3},
};

LOCAL MMIIM_SP_TP_TEXT_INFO_T const en9key_text_layout[] =
{
    {{en9key_x_t0, en9key_y_t0, en9key_x_t0 + en9key_w_t, en9key_y_t0 + en9key_h_t}, 0, {0}},
    {{en9key_x_t0, en9key_y_t1, en9key_x_t0 + en9key_w_t, en9key_y_t1 + en9key_h_t}, 0, {0}},
    {{en9key_x_t0, en9key_y_t2, en9key_x_t0 + en9key_w_t, en9key_y_t2 + en9key_h_t}, 0, {0}},
    {{en9key_x_t0, en9key_y_t3, en9key_x_t0 + en9key_w_t, en9key_y_t3 + en9key_h_t}, 0, {0}},
};

/*lint +e785*/



/*==============================================================================
Function Name: KbEn26KeyInit
Description: KbEn26KeyInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    size_t keyboard_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->rect.left = 0;
    kb_ptr->rect.top = 0;
    kb_ptr->rect.right = en26key_w - 1;
    kb_ptr->rect.bottom = en26key_h - 1;

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN26_KB_BACKGROUND;//IMG_IMSP_EN26_LOWER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN26_LOWER_ON;

    keyboard_size = sizeof(en26key_layout);

    kb_ptr->key_cnt = ARR_SIZE(en26key_layout);
    kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
    //SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
    if(PNULL == kb_ptr->keys_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(kb_ptr->keys_ptr, en26key_layout, keyboard_size);

    kb_ptr->offset.y -= en26key_h;

    return;
}


/*==============================================================================
Function Name: KbEn26KeyUpper
Description: KbEn26KeyUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyUpper(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    //uint16 key_idx = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    //for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    //{
    //    kb_ptr->keys_ptr[key_idx].value = toupper(kb_ptr->keys_ptr[key_idx].value);
    //}

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN26_KB_BACKGROUND;//IMG_IMSP_EN26_UPPER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN26_UPPER_ON;

    return;
}


/*==============================================================================
Function Name: KbEn26KeyLower
Description: KbEn26KeyLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn26KeyLower(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    //uint16 key_idx = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    //for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    //{
    //    kb_ptr->keys_ptr[key_idx].value = tolower(kb_ptr->keys_ptr[key_idx].value);
    //}

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN26_KB_BACKGROUND;//IMG_IMSP_EN26_LOWER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN26_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN26_LOWER_ON;

    return;
}


/*==============================================================================
Function Name: Kb123Init
Description: Kb123Init
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void Kb123Init(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    size_t keyboard_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->rect.left = 0;
    kb_ptr->rect.top = 0;
    kb_ptr->rect.right = digital_w - 1;
    kb_ptr->rect.bottom = digital_h - 1;

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_NUM_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_NUM_ON;

    keyboard_size = sizeof(digital_layout);

    kb_ptr->key_cnt = ARR_SIZE(digital_layout);
    kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
    //SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
    if(PNULL == kb_ptr->keys_ptr)
    {
        return;
    }
    SCI_MEMCPY(kb_ptr->keys_ptr, digital_layout, keyboard_size);

    kb_ptr->offset.y -= digital_h;

    return;
}


/*==============================================================================
Function Name: Kb123SymbInit
Description: Kb123SymbInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void Kb123SymbInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    size_t keyboard_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->rect.left = 0;
    kb_ptr->rect.top = 0;
    kb_ptr->rect.right = digital_w - 1;
    kb_ptr->rect.bottom = digital_h - 1;

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_NUM_SYMB_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_NUM_SYMB_ON;

    keyboard_size = sizeof(digital_symb_layout);

    kb_ptr->key_cnt = ARR_SIZE(digital_symb_layout);
    kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
    //SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
    if(PNULL == kb_ptr->keys_ptr)
    {
        return;
    }
    SCI_MEMCPY(kb_ptr->keys_ptr, digital_symb_layout, keyboard_size);

    kb_ptr->offset.y -= digital_h;

    return;
}



/*==============================================================================
Function Name: KbHwInit
Description: KbHwInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbHwInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    size_t keyboard_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
	if (MMITHEME_IsMainScreenLandscape())
	{
		kb_ptr->rect.left = 0;
		kb_ptr->rect.top = 0;
		kb_ptr->rect.right = hw_w_h - 1;
		kb_ptr->rect.bottom = hw_h_h - 1;

		kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_HW_OFF;
		kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_HW_ON;

		keyboard_size = sizeof(hw_layout_h);

		kb_ptr->key_cnt = ARR_SIZE(hw_layout_h);
		kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
		//SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
        if(PNULL == kb_ptr->keys_ptr)
        {
            return;
        }
		SCI_MEMCPY(kb_ptr->keys_ptr, hw_layout_h, keyboard_size);

		kb_ptr->offset.y -= hw_h_h;
	}
	else
	{
		kb_ptr->rect.left = 0;
		kb_ptr->rect.top = 0;
		kb_ptr->rect.right = hw_w - 1;
		kb_ptr->rect.bottom = hw_h - 1;

		kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_HW_OFF;
		kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_HW_ON;

		keyboard_size = sizeof(hw_layout);

		kb_ptr->key_cnt = ARR_SIZE(hw_layout);
		kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
		//SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
        if(PNULL == kb_ptr->keys_ptr)
        {
            return;
        }
		SCI_MEMCPY(kb_ptr->keys_ptr, hw_layout, keyboard_size);

		kb_ptr->offset.y -= hw_h;
	}

    return;
}



/*==============================================================================
Function Name: KbStrokeInit
Description: KbStrokeInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbStrokeInit(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    size_t keyboard_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    kb_ptr->rect.left = 0;
    kb_ptr->rect.top = 0;
    kb_ptr->rect.right = stroke_w - 1;
    kb_ptr->rect.bottom = stroke_h - 1;

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_STROKE_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_STROKE_ON;

    keyboard_size = sizeof(stroke_layout);

    kb_ptr->key_cnt = ARR_SIZE(stroke_layout);
    kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
    //SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
    if(PNULL == kb_ptr->keys_ptr)
    {
        return;
    }
    SCI_MEMCPY(kb_ptr->keys_ptr, stroke_layout, keyboard_size);

    kb_ptr->offset.y -= stroke_h;

    return;
}

/*==============================================================================
Description: init 9key
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9keyInit(
                        MMIIM_SP_TP_KB_INFO_T *kb_ptr   // [in] key info
                        )
{
    size_t keyboard_size = 0;
    int32 text_layout_size = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->rect.left = 0;
    kb_ptr->rect.top = 0;
    kb_ptr->rect.right = en9key_w - 1;
    kb_ptr->rect.bottom = en9key_h - 1;

    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN9_KB_BACKGROUND;//IMG_IMSP_EN9_LOWER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN9_LOWER_ON;

    keyboard_size = sizeof(en9key_layout);

    kb_ptr->key_cnt = ARR_SIZE(en9key_layout);
    kb_ptr->keys_ptr = (MMIIM_SP_TP_KEY_INFO_T *)SCI_ALLOC_APP(keyboard_size);
    //SCI_ASSERT(PNULL != kb_ptr->keys_ptr); /*assert verified*/
    if(PNULL == kb_ptr->keys_ptr)
    {
        return;
    }
    
    SCI_MEMCPY(kb_ptr->keys_ptr, en9key_layout, keyboard_size);

    // set text region information
    text_layout_size = sizeof(en9key_text_layout);

    kb_ptr->text_cnt = ARR_SIZE(en9key_text_layout);
    kb_ptr->text_ptr = (MMIIM_SP_TP_TEXT_INFO_T *)SCI_ALLOC_APP(text_layout_size);
    SCI_MEMCPY(kb_ptr->text_ptr, en9key_text_layout, text_layout_size);

    kb_ptr->offset.y -= en9key_h;

    return;
}

/*==============================================================================
Description: KbEn9KeyUpper
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9KeyUpper(
                         MMIIM_SP_TP_KB_INFO_T *kb_ptr      // [in] key info
                         )
{
    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN9_KB_BACKGROUND;//IMG_IMSP_EN9_UPPER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN9_UPPER_ON;

    return;
}

/*==============================================================================
Description: KbEn9KeyLower
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
LOCAL void KbEn9KeyLower(
                         MMIIM_SP_TP_KB_INFO_T *kb_ptr      // [in] key info
                         )
{
    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = IMG_IMSP_EN9_KB_BACKGROUND;//IMG_IMSP_EN9_LOWER_OFF;
    kb_ptr->img[MMIIM_SP_TP_KEY_STATE_PRESS] = IMG_IMSP_EN9_KB_BACKGROUND_HIGHLIGHT;//IMG_IMSP_EN9_LOWER_ON;

    return;
}



/*==============================================================================
Function Name: MMIIM_SP_TP_KbInit
Description: MMIIM_SP_TP_KbInit
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbInit(
    MMIIM_SP_TP_KB_INFO_T *kb_ptr,
    MMIIM_SP_TP_KB_TYPE_T type,
    GUI_POINT_T *offset_ptr
)
{
    uint16 i = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    //SCI_ASSERT(type < MMIIM_SP_TP_KB_TYPE_MAX); /*assert verified*/
    if(PNULL == kb_ptr || type >= MMIIM_SP_TP_KB_TYPE_MAX)
    {
        return;
    }
    
    kb_ptr->offset = *offset_ptr;

    kb_ptr->type = type;

    switch (type)
    {
    case MMIIM_SP_TP_KB_TYPE_HW:
        KbHwInit(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_EN26KEY:
        KbEn26KeyInit(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_123:
        Kb123Init(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_123SYMB:
        Kb123SymbInit(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_STROKE:
        KbStrokeInit(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_EN9KEY:
        KbEn9keyInit(kb_ptr);
        break;

    default:
        //SCI_ASSERT(0); /*assert verified*/
        break;
    }

    _OffsetRect(&kb_ptr->rect, &kb_ptr->offset);

    for (i = 0; i < kb_ptr->text_cnt; i++)
    {
        _OffsetRect(&kb_ptr->text_ptr[i].rect, &kb_ptr->offset);
    }

    kb_ptr->last_key_idx = -1;

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TP_KbTerm
Description: MMIIM_SP_TP_KbTerm
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbTerm(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    SCI_FREE(kb_ptr->keys_ptr);

    return;
}



/*==============================================================================
Function Name: MMIIM_SP_TP_KbDraw
Description: MMIIM_SP_TP_KbDraw
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbDraw(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    BOOLEAN is_kb_disp = FALSE;
    uint16 key_idx = 0;
    uint16 text_idx = 0;
    MMI_IMAGE_ID_T key_img = 0;
    GUI_RECT_T key_rect = {0};
    BOOLEAN is_key_disp = FALSE;

    MMI_STRING_T    text_info = {0};
    GUISTR_STYLE_T  text_style = {0};
    GUISTR_STATE_T  text_state = GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN;
    GUI_RECT_T      text_rect = {0};

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    //1 画背景
    is_kb_disp = GUIRES_DisplayImg(&(kb_ptr->offset), PNULL, PNULL,
                                   kb_ptr->win_id, kb_ptr->img[MMIIM_SP_TP_KEY_STATE_ACTIVE], kb_ptr->lcd_info_ptr);

    if (!is_kb_disp)
    {
        //SCI_TRACE_LOW:"MMIIM_SP_TP_KbDraw, kb display false."
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIIM_SP_TP_BASIC_1138_112_2_18_2_26_41_72,(uint8*)"");
        return;
    }

    //2 画键的特殊设置(特殊键和键状态)
    for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    {
        if (kb_ptr->keys_ptr[key_idx].has_img)
        {
            key_img = kb_ptr->keys_ptr[key_idx].img[kb_ptr->keys_ptr[key_idx].state];

            //有图片，且图片不空的时候，用设置的图片
            if (0 != key_img)
            {
                key_rect = kb_ptr->keys_ptr[key_idx].rect;
                _OffsetRect(&key_rect, &(kb_ptr->offset));

                is_key_disp = GUIRES_DisplayImg(PNULL, &key_rect, PNULL,
                                                kb_ptr->win_id, key_img, kb_ptr->lcd_info_ptr);

                if (!is_kb_disp)
                {
                    return;
                }

                continue;
            }
        }

        if (MMIIM_SP_TP_KEY_STATE_ACTIVE != kb_ptr->keys_ptr[key_idx].state)
        {
            key_img = kb_ptr->img[kb_ptr->keys_ptr[key_idx].state];

            key_rect = kb_ptr->keys_ptr[key_idx].rect;
            _OffsetRect(&key_rect, &(kb_ptr->offset));

            is_key_disp = GUIRES_DisplayImg(PNULL, &key_rect, &(kb_ptr->keys_ptr[key_idx].rect),
                                            kb_ptr->win_id, key_img, kb_ptr->lcd_info_ptr);
            if (!is_kb_disp)
            {
                return;
            }
        }
    }

    // draw text
    for (text_idx = 0; text_idx < kb_ptr->text_cnt; text_idx++)
    {
        if (0 < kb_ptr->text_ptr[text_idx].text_len)
        {
            text_info.wstr_ptr = kb_ptr->text_ptr[text_idx].text;
            text_info.wstr_len = kb_ptr->text_ptr[text_idx].text_len;

            text_style.font = g_text_bar_theme.font_text;
            text_style.font_color = g_text_bar_theme.color_text;
            text_style.align = ALIGN_HVMIDDLE;

            text_rect = kb_ptr->text_ptr[text_idx].rect;
            _OffsetRect(&text_rect, &(kb_ptr->offset));

            GUISTR_DrawTextToLCDInRect(
                    kb_ptr->lcd_info_ptr, 
                    &kb_ptr->text_ptr[text_idx].rect, &kb_ptr->text_ptr[text_idx].rect,
                    &text_info, &text_style, text_state, GUISTR_TEXT_DIR_AUTO);
        }
    }

    return;
}




/*==============================================================================
Function Name: MMIIM_SP_TP_KbGetKeyIndex
Description: MMIIM_SP_TP_KbGetKeyIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TP_KbGetKeyIndex(MMIIM_SP_TP_KB_INFO_T *kb_ptr, GUI_POINT_T *point_ptr)
{
    GUI_POINT_T point = {0};
    uint16 key_idx = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != point_ptr); /*assert verified*/
    if(PNULL == kb_ptr || PNULL == point_ptr)
    {
        return -1;
    }
    
    point.x = point_ptr->x - kb_ptr->offset.x;
    point.y = point_ptr->y - kb_ptr->offset.y;

    for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    {
        if (GUI_PointIsInRect(point, kb_ptr->keys_ptr[key_idx].rect))
        {
            break;
        }
    }

    return ((kb_ptr->key_cnt == key_idx) ? -1 : key_idx);
}


/*==============================================================================
Function Name: MMIIM_SP_TP_KbSetImg
Description: MMIIM_SP_TP_KbSetImg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TP_KbSetImg(
    MMIIM_SP_TP_KB_INFO_T *kb_ptr,
    wchar key_value,
    MMI_IMAGE_ID_T img_inactive,
    MMI_IMAGE_ID_T img_active,
    MMI_IMAGE_ID_T img_press
)
{
    uint16 key_idx = 0;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return  key_idx;
    }
    
    for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    {
        if (kb_ptr->keys_ptr[key_idx].value == key_value)
        {
            kb_ptr->keys_ptr[key_idx].has_img = TRUE;
            kb_ptr->keys_ptr[key_idx].img[MMIIM_SP_TP_KEY_STATE_INACTIVE] = img_inactive;
            kb_ptr->keys_ptr[key_idx].img[MMIIM_SP_TP_KEY_STATE_ACTIVE] = img_active;
            kb_ptr->keys_ptr[key_idx].img[MMIIM_SP_TP_KEY_STATE_PRESS] = img_press;

            break;
        }
    }

    return key_idx;
}

/*==============================================================================
Description: MMIIM_SP_TP_SetText
Author: hua.fang
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_TP_SetText(
                            MMIIM_SP_TP_KB_INFO_T *kb_ptr,
                            uint16                index,
                            wchar                 *text_ptr,
                            uint16                text_len
                            )
{
    if (PNULL == kb_ptr || MMIIM_SP_TP_BASIC_MAX_TEXT_LEN <= text_len
        || index >= kb_ptr->text_cnt || PNULL == kb_ptr->text_ptr )
    {
        return FALSE;
    }

    SCI_MEMSET(kb_ptr->text_ptr[index].text, 0, MMIIM_SP_TP_BASIC_MAX_TEXT_LEN * sizeof(wchar));

    if (PNULL != text_ptr && 0 < text_len)
    {
        MMI_MEMCPY(
                kb_ptr->text_ptr[index].text, MMIIM_SP_TP_BASIC_MAX_TEXT_LEN * sizeof(wchar),
                text_ptr,(text_len * sizeof(wchar)),
                (text_len * sizeof(wchar)));

        kb_ptr->text_ptr[index].text_len = text_len;
    }
    else
    {
        kb_ptr->text_ptr[index].text_len = 0;
    }

    return TRUE;
}


/*==============================================================================
Function Name: MMIIM_SP_TP_KbUpper
Description: MMIIM_SP_TP_KbUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbUpper(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    switch (kb_ptr->type)
    {
    case MMIIM_SP_TP_KB_TYPE_EN26KEY:
        KbEn26KeyUpper(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_EN9KEY:
        KbEn9KeyUpper(kb_ptr);
        break;

    default:
        //SCI_ASSERT(0); /*assert verified*/
        break;
    }

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TP_KbLower
Description: MMIIM_SP_TP_KbLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbLower(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return;
    }
    
    switch (kb_ptr->type)
    {
    case MMIIM_SP_TP_KB_TYPE_EN26KEY:
        KbEn26KeyLower(kb_ptr);
        break;

    case MMIIM_SP_TP_KB_TYPE_EN9KEY:
        KbEn9KeyLower(kb_ptr);
        break;

    default:
        //SCI_ASSERT(0); /*assert verified*/
        break;
    }

    return;
}

/*==============================================================================
Description: MMIIM_SP_TP_KbPress
Author: hua.fang
Global resource dependence: NONE
Note: 使某键处于按下状态
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbPress(
                            MMIIM_SP_TP_KB_INFO_T *kb_ptr,  // [in]
                            uint16 index                    // [in]
                            )
{
    BOOLEAN is_changed = FALSE;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    //SCI_ASSERT(index < kb_ptr->key_cnt); /*assert verified*/
    if(PNULL == kb_ptr || index >= kb_ptr->key_cnt)
    {
        return  FALSE;
    }
    
    if(MMIIM_SP_TP_KEY_STATE_ACTIVE == kb_ptr->keys_ptr[index].state)
    {
        kb_ptr->keys_ptr[index].state = MMIIM_SP_TP_KEY_STATE_PRESS;
        kb_ptr->last_key_idx = index;
        is_changed = TRUE;
    }


    return is_changed;
}

/*==============================================================================
Function Name: MMIIM_SP_TP_KbRelease
Description: MMIIM_SP_TP_KbRelease
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 返回是否有键被释放
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbRelease(MMIIM_SP_TP_KB_INFO_T *kb_ptr)
{
    BOOLEAN has_key = FALSE;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return  FALSE;
    }
    
    if (-1 != kb_ptr->last_key_idx)
    {
        if (MMIIM_SP_TP_KEY_STATE_PRESS == kb_ptr->keys_ptr[kb_ptr->last_key_idx].state)
        {
            kb_ptr->keys_ptr[kb_ptr->last_key_idx].state = MMIIM_SP_TP_KEY_STATE_ACTIVE;
        }

        kb_ptr->last_key_idx = -1;
        has_key = TRUE;
    }

    return has_key;
}

/*==============================================================================
Description: MMIIM_SP_TP_KbEnable
Author: hua.fang
Global resource dependence: NONE
Note: 使某键失效
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbEnable(
                             MMIIM_SP_TP_KB_INFO_T *kb_ptr,     // [in]
                             wchar key_value,                   // [in] 用户自定义的值，而非索引
                             BOOLEAN enabled                    // [in]
                             )
{
    uint16 key_idx = 0;
    BOOLEAN is_changed = FALSE;

    //SCI_ASSERT(PNULL != kb_ptr); /*assert verified*/
    if(PNULL == kb_ptr)
    {
        return  FALSE;
    }
    
    for (key_idx = 0; key_idx < kb_ptr->key_cnt; key_idx++)
    {
        if (kb_ptr->keys_ptr[key_idx].value == key_value)
        {
            if (enabled)
            {
                kb_ptr->keys_ptr[key_idx].state = MMIIM_SP_TP_KEY_STATE_ACTIVE;
            }
            else
            {
                kb_ptr->keys_ptr[key_idx].state = MMIIM_SP_TP_KEY_STATE_INACTIVE;
            }

            is_changed = TRUE;

            break;
        }
    }

    return is_changed;
}
#endif

/*==============================================================================
Function Name: DrawRectGradient
Description: 描绘一个颜色渐变的矩形
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note: 颜色计算部分要小心小点遗漏
==============================================================================*/
LOCAL void DrawRectGradient(
    GUI_LCD_DEV_INFO *lcd_info_ptr,
    GUI_RECT_T *rect_ptr,
    GUI_COLOR_T c0,
    GUI_COLOR_T c1
)
{
    int16 i = 0;
    int16 lines = 0;
    GUI_COLOR_T disp_color = 0;
    int32 dr = (int32)(c1 & 0xf800) - (c0 & 0xf800);
    int32 dg = (int32)(c1 & 0x07e0) - (c0 & 0x07e0);
    int32 db = (int32)(c1 & 0x001f) - (c0 & 0x001f);
    int32 ddr, ddg, ddb;
    int32 nr, ng, nb;

    if (PNULL == rect_ptr)
    {
        return;
    }

    lines = rect_ptr->bottom - rect_ptr->top;

    dr >>= 11;
    dg >>= 5;

    for (i = 0; i <= lines; i++)
    {
        ddr = i*dr;
        ddr /= lines;
        ddg = i*dg;
        ddg /= lines;
        ddb = i*db;
        ddb /= lines;

        nr = ((c0 & 0xf800) >> 11) + ddr;
        ng = ((c0 & 0x07e0) >> 5) + ddg;
        nb = (c0 & 0x001f) + ddb;

        disp_color = (GUI_COLOR_T)((nr << 11) + (ng << 5) + nb);

        LCD_DrawHLine(lcd_info_ptr,
                      rect_ptr->left,
                      rect_ptr->top + i,
                      rect_ptr->right,
                      disp_color);
    }

    return;
}


/*==============================================================================
Function Name: MMIIM_SP_TransMsg
Description: 转化系统消息为应用定义消息
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TransMsg(
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_TP_AREA_INFO_T const *tp_area_info_ptr,
    MMIIM_SP_MSG_T *msg_ptr
)
{
#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))    
    BOOLEAN is_tp_in_rect = FALSE;
    int32 bar_idx = 0;
    int32 area_idx = 0;
#endif
    //SCI_ASSERT(PNULL != event_data_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != tp_area_info_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != msg_ptr); /*assert verified*/
    if(PNULL == event_data_ptr || PNULL == tp_area_info_ptr||PNULL == msg_ptr)
    {
        return;
    }
    
    msg_ptr->id = MMIIM_SP_MSG_NONE;

    //消息转换
    switch (event_data_ptr->sys_msg.msg_id)
    {
#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))        
    case MSG_TP_PRESS_DOWN:
        msg_ptr->id = MMIIM_SP_MSG_TPDOWN;
        break;

    case MSG_TP_PRESS_MOVE:
        msg_ptr->id = MMIIM_SP_MSG_TPMOVE;
        break;

    case MSG_TP_PRESS_UP:
        msg_ptr->id = MMIIM_SP_MSG_TPUP;
        break;

    case MSG_TP_PRESS_LONG:
        msg_ptr->id = MMIIM_SP_MSG_TPLONG;
        break;
#endif

    default:

        switch (event_data_ptr->sys_msg.msg_id & MMIIM_SP_KEY_TYPE_MASK)
        {
        case KEY_PRESSED:
            msg_ptr->id = MMIIM_SP_MSG_KEY_PRESS;
            break;

        case KEY_RELEASED:
            msg_ptr->id = MMIIM_SP_MSG_KEY_RELEASE;
            break;

        case KEY_LONG_PRESSED:
            msg_ptr->id = MMIIM_SP_MSG_KEY_LONG_PRESS;
            break;

        case KEY_LONG_RELEASED:
            msg_ptr->id = MMIIM_SP_MSG_KEY_LONG_RELEASE;
            break;

        case KEY_REPEATED:
            msg_ptr->id = MMIIM_SP_MSG_KEY_REPEAT_PRESS;
            break;

        default:
            break;
        }

        break;
    }       //lint !e764        //没有tp时，走默认物理输入

    //非处理数据，返回
    if (MMIIM_SP_MSG_NONE == msg_ptr->id)
    {
        return;
    }

    //先判断物理按键输入
    if (MMIIM_SP_MSG_KEY_PRESS == msg_ptr->id
             || MMIIM_SP_MSG_KEY_RELEASE == msg_ptr->id
             || MMIIM_SP_MSG_KEY_LONG_PRESS == msg_ptr->id
             || MMIIM_SP_MSG_KEY_LONG_RELEASE == msg_ptr->id
#ifdef IM_ENGINE_T9
			 || MMIIM_SP_MSG_KEY_REPEAT_PRESS == msg_ptr->id
#endif
			 )
    {
        msg_ptr->para.key = MMIIM_PhysicalKeyMsgToImKey(event_data_ptr->sys_msg.msg_id);
    }

    //如果存在TP输入，则判断TP输入
#if (defined (TOUCH_PANEL_SUPPORT) && (!defined MMI_IM_PDA_SUPPORT))
    //获取消息参数
    else if (MMIIM_SP_MSG_TPDOWN == msg_ptr->id
            || MMIIM_SP_MSG_TPMOVE == msg_ptr->id
            || MMIIM_SP_MSG_TPUP == msg_ptr->id
            || MMIIM_SP_MSG_TPLONG == msg_ptr->id)
    {
        msg_ptr->para.tp.point.x = MMK_GET_TP_X(event_data_ptr->sys_msg.param);
        msg_ptr->para.tp.point.y = MMK_GET_TP_Y(event_data_ptr->sys_msg.param);

        msg_ptr->para.tp.area = MMIIM_SP_MSG_TP_NONE;

        do
        {
            //kb
            if (PNULL != tp_area_info_ptr->kb_ptr)
            {
                is_tp_in_rect = GUI_PointIsInRect(msg_ptr->para.tp.point,
                                                  tp_area_info_ptr->kb_ptr->rect);

                if (is_tp_in_rect)
                {
                    msg_ptr->para.tp.area = MMIIM_SP_MSG_TP_KB;
                    msg_ptr->para.tp.idx = MMIIM_SP_TP_KbGetKeyIndex(
                                               tp_area_info_ptr->kb_ptr,
                                               &msg_ptr->para.tp.point);

                    break;
                }
            }

            //bars
            for (bar_idx = 0; bar_idx < 4; bar_idx++)
            {
                if (PNULL == tp_area_info_ptr->bar_ptr[bar_idx]) continue;

                if (0 == tp_area_info_ptr->bar_ptr[bar_idx]->seg_cnt) continue;

                is_tp_in_rect = GUI_PointIsInRect(msg_ptr->para.tp.point,
                                                  tp_area_info_ptr->bar_ptr[bar_idx]->rect);

                if (is_tp_in_rect)
                {
                    msg_ptr->para.tp.area = MMIIM_SP_MSG_TP_BAR0 + bar_idx;
                    msg_ptr->para.tp.idx = MMIIM_SP_TextBarGetTp(
                                               tp_area_info_ptr->bar_ptr[bar_idx],
                                               &msg_ptr->para.tp.point);

                    break;
                }
            }

            if (MMIIM_SP_MSG_TP_NONE != msg_ptr->para.tp.area) break;

            //control area
            for (area_idx = 0; area_idx < 2; area_idx++)
            {
                if (PNULL == tp_area_info_ptr->area_rect_ptr[area_idx]) continue;

                is_tp_in_rect = GUI_PointIsInRect(msg_ptr->para.tp.point,
                                                  *(tp_area_info_ptr->area_rect_ptr[area_idx]));

                if (is_tp_in_rect)
                {
                    msg_ptr->para.tp.area = MMIIM_SP_MSG_TP_AREA0 + area_idx;
                    break;
                }
            }

            //如果还有后续的区域，则此处需要加上区域判断
            //if (MMIIM_SP_MSG_TP_NONE != msg_ptr->para.tp.area) break;
        }
        while (0);
    }
#endif

    return;
}

/*==============================================================================
Function Name: MMIIM_SP_TextBarTPON
Description: 当触笔停留在text bar上的时候显示焦点等
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarTPON(MMIIM_SP_TEXT_BAR_T *bar_ptr, int32 idx, BOOLEAN *dirty_flg_ptr)
{
    if (MMIIM_SP_TEXT_BAR_TP_IDC_LEFT == idx)
    {
        MMIIM_SP_PRESS_IDC(bar_ptr->idc_left);
        *dirty_flg_ptr = TRUE;
    }
    else if (MMIIM_SP_TEXT_BAR_TP_IDC_RIGHT == idx)
    {
        MMIIM_SP_PRESS_IDC(bar_ptr->idc_right);
        *dirty_flg_ptr = TRUE;
    }
    else if (0 <= idx && idx < bar_ptr->seg_cnt
             && bar_ptr->hot_index != idx)
    {
        bar_ptr->hot_index = idx;
        *dirty_flg_ptr = TRUE;
    }

    return;
}

//commom key 用线条描出来的按键
/*==============================================================================
Function Name: MMIIM_SP_ComKeyDraw
Description: MMIIM_SP_ComKeyDraw
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ComKeyDraw(
    GUI_LCD_DEV_INFO *lcd_info_ptr,
    MMIIM_SP_COMMON_KEY_T *key_ptr
)
{
    const GUI_COLOR_T bg_color_inactive[4] = {0x9cd3, 0x9cd3, 0x9cd3, 0x9cd3};
    const GUI_COLOR_T bg_color_active[4] = {0x9cd3, 0x528a, 0x0040, 0x0040};
    const GUI_COLOR_T bg_color_press[4] = {0xfc82, 0xfc82, 0xfc82, 0xfc82};
    const GUI_COLOR_T color_font = 0xe71c;
    const GUI_FONT_T font_key = SONG_FONT_14;
    GUI_COLOR_T const *bg_color_ptr = PNULL;
    GUI_RECT_T rect_draw = {0};
    MMI_STRING_T string = {0};
    GUISTR_STYLE_T style = {0};

    //SCI_ASSERT(PNULL != lcd_info_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != key_ptr); /*assert verified*/
    if(PNULL == lcd_info_ptr || PNULL == key_ptr)
    {
        return  FALSE;
    }
    
    switch (key_ptr->state)
    {
    case MMIIM_SP_TP_KEY_STATE_INACTIVE:
        bg_color_ptr = bg_color_inactive;
        break;

    case MMIIM_SP_TP_KEY_STATE_ACTIVE:
        bg_color_ptr = bg_color_active;
        break;

    case MMIIM_SP_TP_KEY_STATE_PRESS:
        bg_color_ptr = bg_color_press;
        break;

    default:
        break;
    }

    if (PNULL == bg_color_ptr)
    {
        return FALSE;
    }

    //draw key
    // 1 draw bg
    //上半个
    rect_draw = key_ptr->rect;
    rect_draw.bottom += rect_draw.top;
    rect_draw.bottom /= 2;
    DrawRectGradient(lcd_info_ptr, &rect_draw, bg_color_ptr[0], bg_color_ptr[1]);
    //下半个
    rect_draw.top = rect_draw.bottom + 1;
    rect_draw.bottom = key_ptr->rect.bottom;
    DrawRectGradient(lcd_info_ptr, &rect_draw, bg_color_ptr[2], bg_color_ptr[3]);

    //draw string=
    string.wstr_ptr = key_ptr->str;
    string.wstr_len = key_ptr->str_len;

    style.align = ALIGN_HVMIDDLE;
    style.font = font_key;
    style.font_color = color_font;

    GUISTR_DrawTextToLCDInRect(lcd_info_ptr,
                               &key_ptr->rect,
                               &key_ptr->rect,
                               &string,
                               &style,
                               GUISTR_STATE_SINGLE_LINE,
                               GUISTR_TEXT_DIR_AUTO);

    return TRUE;
}

/*==============================================================================
Function Name: MMIIM_SP_ComKeyPress
Description: MMIIM_SP_ComKeyPress
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ComKeyPress(MMIIM_SP_COMMON_KEY_T *key_ptr)
{
    BOOLEAN is_changed = FALSE;
    
    if(MMIIM_SP_TP_KEY_STATE_ACTIVE == (key_ptr)->state)
    {
        key_ptr->state = MMIIM_SP_TP_KEY_STATE_PRESS;
        is_changed = TRUE;
    }

    return is_changed;
}


/*==============================================================================
Function Name: MMIIM_SP_ComkeyRelease
Description: MMIIM_SP_ComkeyRelease
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ComkeyRelease(MMIIM_SP_COMMON_KEY_T *key_ptr)
{
    BOOLEAN is_changed = FALSE;

    if(MMIIM_SP_TP_KEY_STATE_PRESS == (key_ptr)->state)
    {
        (key_ptr)->state = MMIIM_SP_TP_KEY_STATE_ACTIVE;
        is_changed = TRUE;
    }

    return is_changed;

}




/*Edit by script, ignore 4 case. Thu Apr 26 19:00:56 2012*/ //IGNORE9527
