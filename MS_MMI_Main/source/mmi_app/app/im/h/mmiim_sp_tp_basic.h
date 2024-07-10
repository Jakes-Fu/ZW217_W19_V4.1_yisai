/*==============================================================================
File Name: mmiim_sp_tp_basic.h
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
#ifndef _MMIIM_SP_TP_BASIC_H_
#define _MMIIM_SP_TP_BASIC_H_


#include "mmiim_sp_basic.h"


#define MMIIM_SP_COMMOM_KEY_TIP_LEN (5)
#define MMIIM_SP_TP_BASIC_MAX_TEXT_LEN (10)

//特殊键的定义。
//键盘上一些按键是特殊意义的，比如大小写切换，输入法切换等
//之前做的tp过于复杂了，这里做最大限度的简化
#define MMIIM_SP_TP_TAG_START (0xE000)

enum
{
    MMIIM_SP_TP_TAG_SHIFT_CAP = MMIIM_SP_TP_TAG_START, //大小写
    MMIIM_SP_TP_TAG_SHIFT_LANG, //语言切换，比如中英文
    MMIIM_SP_TP_TAG_SHIFT_IM, //切换输入法
    MMIIM_SP_TP_TAG_SHIFT_KB, //9、26键切换
    MMIIM_SP_TP_TAG_SYMBOL, //切换到符号面板
    MMIIM_SP_TP_TAG_DIGITAL, //切换到数字面板
    MMIIM_SP_TP_TAG_LOCK, //切换到数字面板
    MMIIM_SP_TP_TAG_TOPARROW,   // 上箭头
    MMIIM_SP_TP_TAG_BOTTOMARROW,// 下箭头
    MMIIM_SP_TP_TAG_TEXT0,      // 文本区
    MMIIM_SP_TP_TAG_TEXT1,
    MMIIM_SP_TP_TAG_TEXT2,
    MMIIM_SP_TP_TAG_TEXT3,

    MMIIM_SP_TP_TAG_MAX,
};



enum
{
    MMIIM_SP_TP_KEY_STATE_NONE, //不显示
    MMIIM_SP_TP_KEY_STATE_INACTIVE, //不可用状态，即灰化
    MMIIM_SP_TP_KEY_STATE_ACTIVE, //正常状态
    MMIIM_SP_TP_KEY_STATE_PRESS, //被按下

    MMIIM_TP_KEY_STATE_MAX,
};
typedef uint8 MMIIM_SP_TP_KEY_STATE_T;

// key information in panel
typedef struct
{
    GUI_RECT_T rect; //相对位置

    MMIIM_SP_TP_KEY_STATE_T state;

    wchar value;

    BOOLEAN has_img;
    MMI_IMAGE_ID_T img[MMIIM_TP_KEY_STATE_MAX];
} MMIIM_SP_TP_KEY_INFO_T;

// text information that will be displayed in panel
typedef struct mmiim_sp_tp_text_info_tag
{
    GUI_RECT_T  rect;       // 相对位置
    uint16      text_len;   // length of text
    wchar       text[MMIIM_SP_TP_BASIC_MAX_TEXT_LEN];   // displayed text
} MMIIM_SP_TP_TEXT_INFO_T;


enum
{
    MMIIM_SP_TP_KB_TYPE_HW,
    MMIIM_SP_TP_KB_TYPE_EN26KEY,
    MMIIM_SP_TP_KB_TYPE_123,
    MMIIM_SP_TP_KB_TYPE_123SYMB,
    MMIIM_SP_TP_KB_TYPE_STROKE,
    MMIIM_SP_TP_KB_TYPE_EN9KEY,

    MMIIM_SP_TP_KB_TYPE_MAX,
};
typedef uint8 MMIIM_SP_TP_KB_TYPE_T;



typedef struct
{
    GUI_POINT_T             offset;
    GUI_RECT_T              rect; //绝对位置

    MMI_WIN_ID_T            win_id;
    GUI_LCD_DEV_INFO        *lcd_info_ptr;

    MMI_IMAGE_ID_T          img[MMIIM_TP_KEY_STATE_MAX];

    MMIIM_SP_TP_KB_TYPE_T   type;               // panel type
    MMIIM_SP_TP_KEY_INFO_T  *keys_ptr;          // key information
    uint16                  key_cnt;            // key info count

    MMIIM_SP_TP_TEXT_INFO_T *text_ptr;          // displayed text infomation
    uint16                  text_cnt;           // text info count

    int32                   last_key_idx;
} MMIIM_SP_TP_KB_INFO_T;


// 跟MMIIM_SP_MSG_TP_AREA_T对应的区域设置
typedef struct
{
    MMIIM_SP_TP_KB_INFO_T *kb_ptr;
    MMIIM_SP_TEXT_BAR_T *bar_ptr[4]; //从下往上数bar
    GUI_RECT_T *area_rect_ptr[2]; //控制区域，各应用自定义
} MMIIM_SP_TP_AREA_INFO_T;



//简单的手绘按键的数据结构，其风格用常数表示
typedef struct
{
    GUI_RECT_T rect;
    wchar str[MMIIM_SP_COMMOM_KEY_TIP_LEN];
    size_t str_len;
    MMIIM_SP_TP_KEY_STATE_T state;
}MMIIM_SP_COMMON_KEY_T;




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
);

/*==============================================================================
Function Name: MMIIM_SP_TP_KbTerm
Description: MMIIM_SP_TP_KbTerm
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbTerm(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TP_KbDraw
Description: MMIIM_SP_TP_KbDraw
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbDraw(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TP_KbGetKeyIndex
Description: MMIIM_SP_TP_KbGetKeyIndex
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
int32 MMIIM_SP_TP_KbGetKeyIndex(MMIIM_SP_TP_KB_INFO_T *kb_ptr, GUI_POINT_T *point_ptr);

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
);

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
                            );

/*==============================================================================
Function Name: MMIIM_SP_TP_KbUpper
Description: MMIIM_SP_TP_KbUpper
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbUpper(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TP_KbLower
Description: MMIIM_SP_TP_KbLower
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TP_KbLower(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TP_KbRelease
Description: MMIIM_SP_TP_KbRelease
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbRelease(MMIIM_SP_TP_KB_INFO_T *kb_ptr);

/*==============================================================================
Description: MMIIM_SP_TP_KbPress
Author: hua.fang
Global resource dependence: NONE
Note: 使某键处于按下状态
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbPress(
                            MMIIM_SP_TP_KB_INFO_T *kb_ptr,  // [in]
                            uint16 index                    // [in]
                            );

/*==============================================================================
Description: MMIIM_SP_TP_KbEnable
Author: hua.fang
Global resource dependence: NONE
Note: 使某键失效
==============================================================================*/
BOOLEAN MMIIM_SP_TP_KbEnable(
                             MMIIM_SP_TP_KB_INFO_T *kb_ptr,     // [in]
                             wchar key_value,                   // [in]
                             BOOLEAN enabled                    // [in]
                             );

/*==============================================================================
Function Name: MMIIM_SP_TransMsg
Description: MMIIM_SP_TransMsg
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TransMsg(
    GUIIM_EVENT_DATA_U const *event_data_ptr,
    MMIIM_SP_TP_AREA_INFO_T const *tp_area_info_ptr,
    MMIIM_SP_MSG_T *msg_ptr
);


/*==============================================================================
Function Name: MMIIM_SP_TextBarTPON
Description: 当触笔停留在text bar上的时候显示焦点等
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
void MMIIM_SP_TextBarTPON(MMIIM_SP_TEXT_BAR_T *bar_ptr, int32 idx, BOOLEAN *dirty_flg_ptr);

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
);

/*==============================================================================
Function Name: MMIIM_SP_ComKeyPress
Description: MMIIM_SP_ComKeyPress
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ComKeyPress(MMIIM_SP_COMMON_KEY_T *key_ptr);

/*==============================================================================
Function Name: MMIIM_SP_ComkeyRelease
Description: MMIIM_SP_ComkeyRelease
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
BOOLEAN MMIIM_SP_ComkeyRelease(MMIIM_SP_COMMON_KEY_T *key_ptr);

/*==============================================================================
Function Name: MMIIM_SP_TpSettingCreateWin
Description: 创建输入法设置菜单窗口
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_TpSettingCreateWin(MMI_HANDLE_T im_handle);

/*==============================================================================
Function Name: MMIIM_SP_SettingCreateWin
Description: MMIIM_SP_SettingCreateWin
Author: Lianxiang.Zhou
Global resource dependence: NONE
Note:
==============================================================================*/
MMI_HANDLE_T MMIIM_SP_SettingCreateWin(MMI_HANDLE_T im_handle);

#endif //_MMIIM_SP_TP_BASIC_H_

