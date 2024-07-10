/*****************************************************************************
** File Name:      mmiim_tp_cstar_internal.h                                 *
** Author:                                                                   *
** Date:           2009/09/16                                                *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to handle tp cstar input                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2009/9/16     rui.xu        Create                                        *
******************************************************************************/

#ifndef _MMIIM_TP_CSTAR_INTERNAL_H_
#define _MMIIM_TP_CSTAR_INTERNAL_H_

#include "mmiim_sp_ml_data.h"
#include "mmiim_sp_ml26key_data.h"
#include "mmiim_tp_cstar_keyboard_cfg.h"

#if defined(TOUCH_PANEL_SUPPORT)
#include "mmiim_tp_multitap_26key.h"
#include "mmiim_tp_multitap_9key.h"
#endif

//#if defined(IM_ENGINE_CSTAR)
#ifndef MMIIM_MULTI_TAP
#define MMIIM_MULTI_TAP
//#endif


#define CAND_MAX_NUM                            64
#define MMIIM_CHOICE_STR_MAX_SHOW_NUM           5
#define MMIIM_CHOICE_MAX_NUM                    30                      //30，此数字来自cstar一句话：某歧义拼音的个数在极端情况时，可能达到20个之多
#define MMIIM_MAX_PYCHOICE_CHAR_NUM             7

#define TYPE_SET_KEYBOARD_NUM                   100
#define CAND_LEFT_ARROW_IDX                     (CAND_MAX_NUM + 1)
#define CAND_RIGHT_ARROW_IDX                    (CAND_MAX_NUM + 2)


#define MMIIM_TP_CSTAR_KEY_TYPE_MASK            (0xff00)                //用于过滤消息类型
#define MMIIM_TP_CSTAR_KEY_MASK                 (0x00ff)

#define TIPS_LEFT_MIN                           20
#define TIPS_SPACE_HEIGHT                       10
#define TIPS_FONT_T                             SONG_FONT_26
#define TIPS_MIN_WIDTH                          54
#define TIPS_CHAR_MAX_NUM                       64                      //用于保存输出tips字符最大长度


#define MMIIM_EXPL_TIMER_OUT_SHORT              (500)
#define MMIIM_EXPL_TIMER_OUT_NORMAL             (750)
#define MMIIM_EXPL_TIMER_OUT_LONG               (1000)
#define MMIIM_EXPL_TIMER_OUT_DEFAULT            MMIIM_EXPL_TIMER_OUT_NORMAL

#define MMIIM_TP_CSTAR_UPPER                    0x20
#define MMIIM_TP_CSTAR_CHARTIME                 1000

/*注意这里不要认为在前面的previous键，就是1，cstar是把向前翻处理成2*/
#define MMIIM_TP_CSTAR_PAGEFLAG_NONE            0x0
#ifdef IM_ENGINE_CSTAR
#define MMIIM_TP_CSTAR_PAGEFLAG_NEXT            0x0001
#define MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS        0x0002
#endif
#ifdef IM_ENGINE_SOGOU
#define MMIIM_TP_CSTAR_PAGEFLAG_NEXT            0x0002
#define MMIIM_TP_CSTAR_PAGEFLAG_PREVIOUS        0x0001
#endif

#define MMIIM_TP_KEY_DISPLAY_TEXT_LEN           10                      //文本显示字符串的内容长度

enum
{
    MMIIM_HW_DEFAULT_TRACE_COLOR = 0x001f,
    MMIIM_HW_DEFAULT_TRACE_WIDTH = 3,
    MMIIM_HW_DEFAULT_LABLE_WIDTH = 3,
};

enum
{
    MMIIM_TP_CSTAR_IDC_NONE,
    MMIIM_TP_CSTAR_IDC_INACTIVE,
    MMIIM_TP_CSTAR_IDC_ACTIVE,
    MMIIM_TP_CSTAR_IDC_PRESSED,

    MMIIM_TP_CSTAR_IDC_MAX,
};

typedef BOOLEAN(*MMIIM_STATE_FUNC)(void*, GUIIM_EVENT_DATA_U*, uint32);   //处理函数的定义

typedef uint16 MMIIM_STATE;                                             //输入法状态

typedef enum
{
    MMIIM_TP_CSTAR_FUNC_NONE,
    MMIIM_TP_CSTAR_FUNC_TIPS,

    MMIIM_TP_CSTAR_FUNC_STATE_MAX,
} MMIIM_TP_CSTAR_FUNC_STATE_T;


typedef struct MMIIM_TP_CSTSR_TIPS_DATA_T
{
    int16           frame_img_width;                                    //左边边框图的宽度
    int16           frame_img_height;
    int16           center_img_width;                                   //中间填充图的宽度
    int16           center_img_height;
    GUI_RECT_T      disp_rect;                                          //显示的全区域

} MMIIM_TP_CSTSR_TIPS_DATA_T;

/*处理按键的结构
[0, 80)：字符表中对应的字符索引
[80, 100)：特殊键
[100, max)：键盘状态值
*/
typedef struct MMIIM_TP_CSTAR_FUNC_T
{
    uint32           id;                                                 //处理函数需要的传入参数
    int16           rect_num;                                           //每个处理案件所占行数(一行占几个小框)
    MMIIM_TP_CSTAR_FUNC_STATE_T state;
    MMIIM_STATE_FUNC func;                                              //每个按键的处理函数

} MMIIM_TP_CSTAR_FUNC_T;


//press up回调函数的参数
typedef union
{
    uint16 iState;

} MMIIM_TP_CSTAR_CB_DATA_T;

typedef enum
{
    MMIIM_TP_CSTAR_MSG_NONE,
    MMIIM_TP_CSTAR_MSG_KEY_PRESS,
    MMIIM_TP_CSTAR_MSG_KEY_RELEASE,
    MMIIM_TP_CSTAR_MSG_KEY_LONG_PRESS,
    MMIIM_TP_CSTAR_MSG_KEY_LONG_RELEASE,
    MMIIM_TP_CSTAR_MSG_TPDOWN,
    MMIIM_TP_CSTAR_MSG_TPMOVE,
    MMIIM_TP_CSTAR_MSG_TPUP,
    MMIIM_TP_CSTAR_MSG_TPLONG,

    MMIIM_TP_CSTAR_MSG_MAX,
} MMIIM_TP_CSTAR_MSG_ID_T;

typedef enum
{
    MMIIM_TP_CSTAR_MSG_TP_NONE,
    MMIIM_TP_CSTAR_MSG_TP_KB,
    MMIIM_TP_CSTAR_MSG_TP_CAND, //从下往上数bar
    MMIIM_TP_CSTAR_MSG_TP_EDIT,
    MMIIM_TP_CSTAR_MSG_TP_CHOICE, //控制区域，各应用自定义，用于9键混淆音区
    MMIIM_TP_CSTAR_MSG_TP_AREA1,

    MMIIM_TP_CSTAR_MSG_TP_MAX
} MMIIM_TP_CSTAR_MSG_TP_AREA_T;

typedef struct
{
    MMIIM_TP_CSTAR_MSG_TP_AREA_T area;
    int32 idx; // key idx of all keypad
    int8 h_idx;
    int8 v_idx;
    GUI_RECT_T rect; //点所在小框区域
    GUI_POINT_T point;
} MMIIM_TP_CSTAR_MSG_PARA_TP_T;

typedef union
{
    MMIIM_TP_CSTAR_MSG_PARA_TP_T tp;
    uint8 key;  //物理键盘上发出的消息
} MMIIM_TP_CSTAR_MSG_PARAM_T;

typedef enum
{
    MMIIM_TP_CSTAR_STATE_NOINPUT,
    MMIIM_TP_CSTAR_STATE_INPUTING,
    MMIIM_TP_CSTAR_STATE_SELECTING,
    MMIIM_TP_CSTAR_STATE_ASSOCIATE,
    MMIIM_TP_CSTAR_STATE_CHOICE,
    MMIIM_TP_CSTAR_STATE_USER,

} MMIIM_TP_CSTAR_IM_STATE_T;

typedef struct
{
    MMIIM_TP_CSTAR_MSG_ID_T id;
    MMIIM_TP_CSTAR_MSG_PARAM_T para;
} MMIIM_TP_CSTAR_MSG_T;

typedef enum MMIIM_TP_CSTAR_KEY_STATE_T
{
    MMIIM_TP_CSTAR_KEY_STATE_NONE,                                      //不显示
    MMIIM_TP_CSTAR_KEY_STATE_INACTIVE,                                  //不可用状态，即灰化
    MMIIM_TP_CSTAR_KEY_STATE_ACTIVE,                                    //正常状态
    MMIIM_TP_CSTAR_KEY_STATE_PRESS,                                     //被按下

    MMIIM_TP_CSTAR_KEY_STATE_MAX,
} MMIIM_TP_CSTAR_KEY_STATE_T;

typedef enum MMIIM_TP_KEY_DISPLAY_TYPE_T
{
    MMIIM_TP_KEY_DISPLAY_TYPE_NONE,
        
    MMIIM_TP_KEY_DISPLAY_TYPE_TEXT,                                     //纯文本显示按键
    MMIIM_TP_KEY_DISPLAY_TYPE_IMAGE,                                    //纯图片显示按键
    MMIIM_TP_KEY_DISPLAY_TYPE_IMAGEANDTEXT,                             //背景图+文本显示按键
    
    MMIIM_TP_KEY_DISPLAY_TYPE_MAX
} MMIIM_TP_KEY_DISPLAY_TYPE_T;

//替换按键的结构
typedef struct MMIIM_TP_CSTAR_REPLACE_KEY_T
{
    uint8           hor_index;                                          //水平偏移格数
    uint8           ver_index;                                          //垂直偏移格数
    uint16          rect_cnt;                                           //占的格数

    MMIIM_TP_KEY_DISPLAY_TYPE_T display_key_type;                       //替换键的显示方式
    wchar text[MMIIM_TP_KEY_DISPLAY_TEXT_LEN];                          //文本内容

    MMIIM_TP_CSTAR_KEY_STATE_T  state;                                  //当前按键状态
    MMI_IMAGE_ID_T  img[MMIIM_TP_CSTAR_KEY_STATE_MAX];                  //需要替换图片资源列表

} MMIIM_TP_CSTAR_REPLACE_KEY_T;

typedef struct MMIIM_TP_CSTAR_CHOICE_BAR_T
{
    GUI_RECT_T                      rect;                               //显示PYchoice的整个绝对位置
    MMI_WIN_ID_T                    win_id;                             //下面这两项是为以后把显示剥出去作准备
    GUI_LCD_DEV_INFO                *lcd_info_ptr;
    MMI_IMAGE_ID_T                  bg_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];//背景图片

    GUI_RECT_T                      idc_up_rect;                        //向上箭头的区域，注意这个区域就是图片的区域，一开始就用图片的长宽给它赋值好了
    MMI_IMAGE_ID_T                  idc_up_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];
    MMIIM_TP_CSTAR_KEY_STATE_T      idc_up_state;                       //上下箭头的状态

    GUI_RECT_T                      idc_down_rect;                      //向下箭头的区域，这样求取图片的宽高，只要求一次即可
    MMI_IMAGE_ID_T                  idc_down_img[MMIIM_TP_CSTAR_KEY_STATE_MAX];
    MMIIM_TP_CSTAR_KEY_STATE_T      idc_down_state;

    int8                            has_hot;                            //当前是否有焦点
    int8                            hot_index;                          //当前页的当前焦点位置

    GUI_RECT_T                      choice_str_rect[MMIIM_CHOICE_STR_MAX_SHOW_NUM]; //定义混淆音字符串的最多可能区域，有效性从0开始
    uint8                           cur_page_line_cnt;                  //用于表示本页显示的实际行数
    uint8                           choice_line_cnt;                    //根据当前的配置的字体高度算出整个混淆区能显示的拼音实际最大行数

    uint16                          total_choice_cnt;                   //目前需要显示的行数，数据来自引擎

    int8                            is_choice_state;                    //当前是否是chioce左右切换状态
    int8                            choice_last_index;                  //当前页当前项的焦点位置
    int8                            choice_index;                       //当前选中项

    wchar                           pChoiceBuffer[MMIIM_CHOICE_MAX_NUM][MMIIM_MAX_PYCHOICE_CHAR_NUM];

    uint16                          page_cnt;
    uint16                          page_index;
    uint8                           pageflag;                           //上下翻页标志

} MMIIM_TP_CSTAR_CHOICE_BAR_T;

// struct for key index and key chars
// 一个键对应最大的字母个数
//#define MMIIM_SP_26KEY_CHAR_MAX 12
typedef struct _MMIIM_26KEY_DISPLAY_KEYCHAR_T
{
    //uint16  key_num;                              // key index
    wchar  key_chars[MMIIM_SP_26KEY_CHAR_MAX+1];   // chars on this key. the first char is count of chars
} MMIIM_KEY_TEXT_DISPLAY_DATA_T;

//虚拟键盘使用文本字符or字符串显示按钮信息，该功能代替图片显示虚拟键盘按钮
//该结构没有将MMIIM_26KEY_DISPLAY_KEYCHAR_T合并在一起，是为了某些结构数据的独立性，
//便于其他某些地方使用，比如此处可以供结构体MMIIM_SP_26KEY_KEYCHAR_T使用
typedef struct _MMIIM_TP_BUTTON_DISPLAY_TEXT
{
//要显示的哪一个字符，该值 <= disp_data.key_chars[0] -1; < 0 表示特殊含义，主要考虑替换键和9键的情况，
//想不出更好的办法了，当然添加字段或采用联合的方式比较好，这样结构体就复杂了点
//MMIIM_KB_KEY_DISP_TEXT_ALL， MMIIM_KB_KEY_DISP_TEXT_HALF_ALL， MMIIM_KB_KEY_DISP_TEXT_NONE等值
    int16 disp_index;	
    MMIIM_KEY_TEXT_DISPLAY_DATA_T disp_data;
}MMIIM_TP_KEY_DISPLAY_TEXT;

//下面的值仅仅给予MMIIM_TP_BUTTON_DISPLAY_TEXT.disp_index使用，切记
#define MMIIM_KB_KEY_DISP_TEXT_ALL (-1)		//某些情况下，可能需要显示所有的字符
#define MMIIM_KB_KEY_DISP_TEXT_CHAR_AND_DOT (-2)		////某些情况下，可能需要显示所有的字符，但实际长度不够，以点取代
#define MMIIM_KB_KEY_DISP_TEXT_NONE (-3)	//
#define MMIIM_KB_KEY_DISP_TEXT_WITHOUT_DIGIT (-4)	//应需求，某些情况下，不允许显示数字字符

//输入法状态结构
typedef struct MMIIM_TP_CSTAR_STATE_T
{
    int16                           vnum;                               //垂直方向的小格数
    int16                           hnum;                               //水平方向的小格数
    const MMIIM_TP_CSTAR_FUNC_T*    functab;                            //键盘上各个按键对应的函数操作集
    const uint16* const *           char_tab;                           //小写字符表
    uint16                          char_cnt;                           //字符个数，小写和大写必须是一一对应的，所以个数一定相等
    const MMIIM_TP_CSTAR_REPLACE_KEY_T* replace_key_tab;                //需要替换的按键
    uint16                          replace_key_cnt;                    //需要替换键的个数
    uint16                          priv_data;                          //此键盘可操作的权限集合
    GUI_RECT_T                      rect;                               //键盘放置绝对区域
    GUI_RECT_T                      img_rect;                           //显示图片相对区域
    MMI_IMAGE_ID_T                  normal_image_id;                    //正常显示图片ID，默认小写图片
    MMI_IMAGE_ID_T                  press_image_id;                     //选中时显示图片ID
    const MMIIM_TP_KEY_DISPLAY_TEXT **p_disp_text;                           //虚拟键盘按钮上显示字符or字符串
    uint16                          disp_text_cnt;                           //虚拟键盘按钮个数，理论上与char_cnt相等，但为了程序的可读性及扩展性，使用不同的值表示
} MMIIM_TP_CSTAR_STATE_T;


//输入法的句柄
typedef struct MMIIM_TP_CSTAR_HANDLE_T
{
    CAF_HANDLE_T                    ctrl_handle;                        //平台相关
    GUI_LCD_DEV_INFO const*         lcd_info_ptr;                       //平台相关
    GUI_LCD_DEV_INFO                tips_lcd_info;                      //tips_lcd

    GUIIM_INIT_KERNEL_T             init_param;                         //输入法控件相关参数
    GUIIM_EVENT_DATA_U*             event_data_ptr;                     //输入法控件事件相关，用于handlemsg

    MMIIM_TP_CSTAR_STATE_ID_T       stateId;                            //此状态ID用于保存各类键盘的类型值，通过此ID可以找到对应的键盘
    MMIIM_TP_CSTAR_STATE_ID_T       bak_stateId;                        //保存上一次键盘状态
    MMIIM_TP_CSTAR_STATE_ID_T       bak_stateIdii;                      //保存上上次键盘状态
    BOOLEAN                         is_full_keypad;                     //是否全键盘
    uint32                          bak_id;                             //用于保存按键的ID，对应表str字符表或者对应键盘状态ID，fun第一字段
    GUI_RECT_T                      press_rect;                         //被按下键的绝对区域
    GUI_RECT_T                      press_img_rect;                     //被按下键的图片的相对区域，相对都是相对图片左上角零点
    GUI_RECT_T                      cand_rect;                          //候选区的绝对区域
    GUI_RECT_T                      edit_rect;                          //编辑区的绝对区域
    GUI_RECT_T                      down_rect;                          //向下翻页按钮的绝对区域
    GUI_RECT_T                      up_rect;                            //向上翻页按钮的绝对区域
    GUI_RECT_T                      lock_rect;                          //锁键绝对区域
    GUI_RECT_T                      lock_img_rect;                      //锁键图片相对区域

    //tips相关
    GUI_RECT_T                      tips_rect;                          //tips层的区域
    uint8                           tips_timer_id;

    MMIIM_TP_CSTAR_CB_DATA_T        cb_data;                            //用于保存press up 回调up按键函数的键值

    BOOLEAN                         is_need_lock;                       //是否锁定
    BOOLEAN                         is_ass;                             //[]
    uint8                           pageflag;                           //翻页标志

    void*                           cstar_handle;                       //cstar输入引擎内核句柄
    unsigned short                  cstar_window_state;                 //直接使用cstar的状态，全用cstar的内容，
    //这个值也好编护，初始化时0，每次调处理函数时，即修改此状态

    uint16                          index;                              //保存当前焦点字
    wchar                           result[CAND_MAX_NUM];
    uint16                          result_len;                         //候选字或者联想字返回的个数
    wchar                           pPreBuffer[CAND_MAX_NUM];
    uint16                          nPreEdit;                           //编辑字个数
    wchar                           pCandTipsBuffer[TIPS_CHAR_MAX_NUM]; //用于保存candbar中被点击的字符串
    uint16                          nCandTips;
    wchar                           pUnderLine[CAND_MAX_NUM];
    uint16                          nUnderLine;                         //UnderLine个数
    wchar                           pUserWordBuffer[CAND_MAX_NUM];
    uint16                          nUserWord;
    
    uint32                          type_set;                           //设置

    void (*func)(struct MMIIM_TP_CSTAR_HANDLE_T*, MMIIM_TP_CSTAR_CB_DATA_T);

    MMIIM_TP_CSTAR_CHOICE_BAR_T     choice_bar;

    MMIIM_TP_CSTAR_IM_STATE_T       ui_state;                               //输入法应用状态

    //draw flag
    MMIIM_TP_CSTAR_MSG_PARA_TP_T    tp_last_msg_param;                  //触笔上一次点击所带参数
    BOOLEAN                         is_rect_change;
    BOOLEAN                         is_kb_dirty;
    BOOLEAN                         is_bar_edit_dirty;
    BOOLEAN                         is_bar_cand_dirty;
    BOOLEAN                         is_bar_choice_dirty;

    //多国语multitap 增加两个句柄指针
#if defined(TOUCH_PANEL_SUPPORT)
    MMIIM_TP_MULTITAP_26KEY_DATA_T* multitap_26key_date_ptr;            // multitap 9key pointer.
    MMIIM_TP_MULTITAP_9KEY_DATA_T*  multitap_9key_date_ptr;             // multitap 9key pointer.
#endif

    BOOLEAN mul_timer_opened;   //cstar在user造词状态，输入字符需要配置定时器控制是否连续按键，类似multitap输入

} MMIIM_TP_CSTAR_HANDLE_T;


typedef uint8 MMIIM_TP_CSTAR_IDC_STATE_T;

typedef struct
{
    MMI_IMAGE_ID_T img_bg;
    MMI_IMAGE_ID_T img_idc_left[MMIIM_TP_CSTAR_IDC_MAX];
    MMI_IMAGE_ID_T img_idc_right[MMIIM_TP_CSTAR_IDC_MAX];

    GUI_FONT_T font_text;
    GUI_FONT_T font_label;
    GUI_COLOR_T color_text;
    GUI_COLOR_T color_label;
    GUI_COLOR_T color_hot;
} MMIIM_TP_CSTAR_TEXT_BAR_THEME_T;




/*****************************************************************************/
//  Description : 触摸屏上各个状态之间切换
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicth(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 字符键处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note: 处理直接上屏的内容，注意配置键盘函数表时，要注意
/*****************************************************************************/
PUBLIC BOOLEAN HandleChar(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 26键盘处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 9键和26键的区别是由输入法引擎决定的，主要是由CS_EVENT_KEYBOARD,CS_EVENT_CHAR
//  这两个不同的事件及后面跟的消息值不同引起的，我们在本地全局变量对按键对应的操作表中
//  配置事件时，要灵活选择，26: 'a', 'char'; 9: 'cs_vk_1','keyboard'
/*****************************************************************************/
PUBLIC BOOLEAN Handle26Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 9键盘处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 说明同上，要注意回退键
/*****************************************************************************/
PUBLIC BOOLEAN Handle9Key(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 返回键处理
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleReturn(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U*event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 输入法选择
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleMethodSelect(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 大小写之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleCaps(
    void* handle_ptr, //in:输入法句柄
    GUIIM_EVENT_DATA_U* event_data_ptr,//in&out:传入传出参数
    uint32 id                 //in:传入的id号
);

/*****************************************************************************/
//  Description : 9->26之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle9KeyTo26Key(
    void* handle_ptr, //in:输入法句柄
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id  //in:传入的id号
);

/*****************************************************************************/
//  Description : 26->9之间切换
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Handle26KeyTo9Key(
    void* handle_ptr, //in:输入法句柄
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id  //in:传入的id号
);

/*****************************************************************************/
//  Description : 锁键盘处理
//  Global resource dependence : none
//  Author: rui.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleLock(
    void *handle_ptr,
    GUIIM_EVENT_DATA_U *event_data_ptr,
    uint32 id
);

/*****************************************************************************/
//  Description : 切换到符号窗口
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN HandleSwicthSymbol(
    void* handle_ptr,
    GUIIM_EVENT_DATA_U* event_data_ptr,
    uint32 id
);



#endif//endif MMIIM_MULTI_TAP

#endif//_MMIIM_TP_CSTAR_INTERNAL_H_
