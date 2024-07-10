/*****************************************************************************
** File Name:      ctrlim_base.h                                             *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Creat
******************************************************************************/

#ifndef _CTRLIM_BASE_H_
#define _CTRLIM_BASE_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "guifont.h"
#include "guilcd.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum _GUIIM_BASE_CONSTANT
@brief 常数定义
*/
enum _GUIIM_BASE_CONSTANT
{
    GUIIM_COMMIT_BUFFER_MAX = 64, /*!< 提交字符的内存大小 */
    GUIIM_CUSTOM_CHAR_LIMIT = 18, /*!< 在CUSTOM输入法中，每个按键最多可容纳的可选字符数 */
    GUIIM_NOTIFY_QUEUE_LEN = 3, /*!< notify消息队列的长度，通常这个值不会很大，这个是一个消息多产生的返回处理 */
    GUIIM_TEXT_CFG_MAX_LEN = 6
};

/*! @enum _GUIIM_CUSTOM_KEY_T
@brief 可配置输入法的配置键值列表
@warning 
- 为什么定义在这里? 
    - 原则上，这个文件里的定义，用户是不可见的。因为custom是一个特殊的玩意儿，可以特别的知道。
- 用户什么时候会用到这些定义?
    - 当用户需要定制multitap中某个或某些键值的时候。用户可以通过接口来设定。
@sa GUIIM_CUSTOM_KEY_T
*/
enum _GUIIM_CUSTOM_KEY_T
{
    GUIIM_CUSTOM_KEY_CHR_A,
    GUIIM_CUSTOM_KEY_CHR_B,
    GUIIM_CUSTOM_KEY_CHR_C,
    GUIIM_CUSTOM_KEY_CHR_D,
    GUIIM_CUSTOM_KEY_CHR_E,
    GUIIM_CUSTOM_KEY_CHR_F,
    GUIIM_CUSTOM_KEY_CHR_G,
    GUIIM_CUSTOM_KEY_CHR_H,
    GUIIM_CUSTOM_KEY_CHR_I,
    GUIIM_CUSTOM_KEY_CHR_J,
    GUIIM_CUSTOM_KEY_CHR_K,
    GUIIM_CUSTOM_KEY_CHR_L,
    GUIIM_CUSTOM_KEY_CHR_M,
    GUIIM_CUSTOM_KEY_CHR_N,
    GUIIM_CUSTOM_KEY_CHR_O,
    GUIIM_CUSTOM_KEY_CHR_P,
    GUIIM_CUSTOM_KEY_CHR_Q,
    GUIIM_CUSTOM_KEY_CHR_R,
    GUIIM_CUSTOM_KEY_CHR_S,
    GUIIM_CUSTOM_KEY_CHR_T,
    GUIIM_CUSTOM_KEY_CHR_U,
    GUIIM_CUSTOM_KEY_CHR_V,
    GUIIM_CUSTOM_KEY_CHR_W,
    GUIIM_CUSTOM_KEY_CHR_X,
    GUIIM_CUSTOM_KEY_CHR_Y,
    GUIIM_CUSTOM_KEY_CHR_Z,

    GUIIM_CUSTOM_KEY_DEL,
    GUIIM_CUSTOM_KEY_COMMA,
    GUIIM_CUSTOM_KEY_PERIOD,
    GUIIM_CUSTOM_KEY_ENTER,
    GUIIM_CUSTOM_KEY_FN,
    GUIIM_CUSTOM_KEY_SHIFT,
    GUIIM_CUSTOM_KEY_AT_QWERTY,
    GUIIM_CUSTOM_KEY_SPACE,
    GUIIM_CUSTOM_KEY_AND,
    GUIIM_CUSTOM_KEY_QUESTION,
    GUIIM_CUSTOM_KEY_CTRL,

    GUIIM_CUSTOM_KEY_PLUS,                  // +
    GUIIM_CUSTOM_KEY_LEFT_PARENTHESIS,      // (
    GUIIM_CUSTOM_KEY_RIGHT_PARENTHESIS,     // )
    GUIIM_CUSTOM_KEY_MINUS,                 // -
    GUIIM_CUSTOM_KEY_DOUBLE_QUOTES,         // "
    GUIIM_CUSTOM_KEY_SEMICOLON,             // ;
    GUIIM_CUSTOM_KEY_COLON,                 // :
    GUIIM_CUSTOM_KEY_SLASH,                 // /
    GUIIM_CUSTOM_KEY_SHIFT_AND,             // &
    GUIIM_CUSTOM_KEY_EXCLAMATION,           // !

    GUIIM_CUSTOM_KEY_0,
    GUIIM_CUSTOM_KEY_1,
    GUIIM_CUSTOM_KEY_2,
    GUIIM_CUSTOM_KEY_3,
    GUIIM_CUSTOM_KEY_4,
    GUIIM_CUSTOM_KEY_5,
    GUIIM_CUSTOM_KEY_6,
    GUIIM_CUSTOM_KEY_7,
    GUIIM_CUSTOM_KEY_8,
    GUIIM_CUSTOM_KEY_9,
    GUIIM_CUSTOM_KEY_STAR,
    GUIIM_CUSTOM_KEY_HASH,

    GUIIM_CUSTOM_KEY_MAX,
};

/*! @typedef GUIIM_CUSTOM_KEY_T
@brief 可配置输入法的配置键值类型
@sa _GUIIM_CUSTOM_KEY_T
*/
typedef int32 GUIIM_CUSTOM_KEY_T;


/*! @typedef GUIIM_METHOD_T
@brief 输入法类型定义
*/
typedef uint16 GUIIM_METHOD_T;

/*! @typedef GUIIM_LANGUAGE_T
@brief 输入语言定义
*/
typedef uint8 GUIIM_LANGUAGE_T;

#define MAKE_HIGHLIGHT_PARAM(start, end)    ((start<<16)|(0xffff&end))
#define SET_HIGHLIGHT_START(v)              (((uint32)v)>>16)
#define SET_HIGHLIGHT_END(v)                ((uint16)v&0xffff)


/*! @enum _GUIIM_INPUT_TYPE_T
@brief 输入方式，即键盘、手写和触笔。
@note 相同的输入法，在不同的输入方式下，需要启动的内核是不一样的。
@sa GUIIM_INPUT_TYPE_T
*/
enum _GUIIM_INPUT_TYPE_T
{
    GUIIM_INPUT_TYPE_NONE        = 0x00, /*!< 无输入类型 */
    GUIIM_INPUT_TYPE_KEYBOARD    = 1 << 0, /*!< 键盘输入(九键) */
    GUIIM_INPUT_TYPE_TOUCHPEN    = 1 << 1, /*!< 触笔输入(模拟九键) */
    GUIIM_INPUT_TYPE_QWERT       = 1 << 2, /*!< 全键盘输入 */
    GUIIM_INPUT_TYPE_TP_QWERT    = 1 << 3, /*!< 全键盘触笔 */
    GUIIM_INPUT_TYPE_HANDWRITING = 1 << 4, /*!< 手写输入 */
    
    GUIIM_INPUT_TYPE_ALL = ((1 << 5) - 1) /*!< 全部输入类型的集合 */
};

/*! @typedef GUIIM_INPUT_TYPE_T
@brief 输入方式类型
@sa _GUIIM_INPUT_TYPE_T
*/
typedef uint8 GUIIM_INPUT_TYPE_T;


/*! @enum _GUIIM_CAPITAL_MODE_T
@brief 大小写模式，即大写、小写和首字母大写。
@note 这个东西大部分输入法都是不需要的，即使少数的输入法用到，那也是要特殊的处理
- 所以，这个东西抽不来感觉并不是很合理。
- 顶多算一个属性，小的属性。
- 目前还是被当作一个大的属性。
@sa GUIIM_CAPITAL_MODE_T
*/
enum _GUIIM_CAPITAL_MODE_T
{
    GUIIM_MODE_LEAD_UPPER, /*!< 首字母大写 */
    GUIIM_MODE_LOWER, /*!< 小写 */
    GUIIM_MODE_UPPER, /*!< 大写 */

    GUIIM_MODE_MAX,
};

/*! @typedef GUIIM_CAPITAL_MODE_T
@brief 大小写模式的类型
@sa _GUIIM_CAPITAL_MODE_T
*/
typedef uint8 GUIIM_CAPITAL_MODE_T;

/*! @enum _GUIIM_TYPE_T
@brief 输入方法定义
@details 一个输入法的确定，四个因素
-# 语言
-# 输入法
-# 输入方式
-# 大小写模式
@note - 以上是目前使用的方式，或许也是常用的方式。不管是不是合理，但习惯就是这样
- 智能输入，不同的语言输入的smart是不同的，中文就是拼音
@note - 通常，在设置有限制的输入法的时候，会用english和abc来表示英文，而在实际的
应用中，我们会用multitap&smart加上english来设定英文的字母和智能输入
@sa GUIIM_TYPE_T
*/
enum _GUIIM_TYPE_T
{
    GUIIM_TYPE_NONE     = 0x00, /*!< 无输入法类型 */
    GUIIM_TYPE_SMART    = 1 << 0, /*!< 智能输入 */
    GUIIM_TYPE_STROKE   = 1 << 1, /*!< 笔划输入 */
    GUIIM_TYPE_MULTITAP = 1 << 2, /*!< 多次按键输入 */
    GUIIM_TYPE_DIGITAL  = 1 << 3, /*!< 数字输入 */
    GUIIM_TYPE_ABC      = 1 << 4, /*!< 英文abc输入 */
    GUIIM_TYPE_ENGLISH  = 1 << 5, /*!< 智能英文输入 */
    GUIIM_TYPE_SYS      = 1 << 6, /*!< 系统输入 */
    GUIIM_TYPE_COUNT    = 7,
};

/*! @typedef GUIIM_TYPE_T
@brief 输入方法类型定义
@note 这个定义是用户所能看到的一个大的类型
@sa _GUIIM_TYPE_T
*/
typedef uint8 GUIIM_TYPE_T;



/*! @enum _GUIIM_LIMIT_T
@brief 输入限制
@note 通过输入限制，来禁止切换到某些输入法
@sa GUIIM_LIMIT_T
*/
enum _GUIIM_LIMIT_T
{
    GUIIM_LIMIT_LOCAL = 1, /*!< 禁止本地语言输入 */
    GUIIM_LIMIT_ENGLISH = 1 << 1, /*!< 禁止英文输入 */
    GUIIM_LIMIT_DIGITAL = 1 << 2, /*!< 禁止数字输入 */
    GUIIM_LIMIT_SYMBOL = 1 << 3, /*!< 禁止符号输入 */
};

/*! @typedef GUIIM_LIMIT_T
@brief 输入限制类型
@sa _GUIIM_LIMIT_T
*/
typedef uint16 GUIIM_LIMIT_T;


/*! @enum _GUIIM_TAG_T
@brief 输入法的附加属性设定，可以设置多个附加属性
*/
enum _GUIIM_TAG_T
{
    GUIIM_TAG_NONE, /*!< 默认是无附加属性的 */
    GUIIM_TAG_CLASSIC_NUM, /*!< 经典９键输入0-9*# */
    GUIIM_TAG_PURE_NUM, /*!< 纯数字输入 */
    GUIIM_TAG_DOT_NUM, /*!< 数字和小数点输入 */
    GUIIM_TAG_PHONE_NUM, /*!< 电话号码输入 */
    GUIIM_TAG_PHONE_NUM_MIN, /*!< 受限制的电话号码输入，仅+和数字 */
	GUIIM_TAG_NUM_WITH_SWITCHING, /*!< 支持切换的数字输入，可切换到字符输入和符号输入 */
    GUIIM_TAG_EMAIL, /*!< Email输入 */
    GUIIM_TAG_URL, /*!< 网址输入 */
    GUIIM_TAG_PB_HUOPIN, /*!< 电话本火拼定制键盘 */
};

/*! @typedef GUIIM_TAG_T
@brief 输入法的附加属性定义
*/
typedef uint32 GUIIM_TAG_T;


/*! @struct GUIIM_CUSTOM_KEY_PARAM_T
@brief 定制按键的时候的传递参数结构
@sa guiim_custom_key_param_tag
*/
typedef struct guiim_custom_key_param_tag GUIIM_CUSTOM_KEY_PARAM_T;

/*! @struct guiim_custom_key_param_tag
@brief 定制按键的时候的传递参数结构
@sa GUIIM_CUSTOM_KEY_PARAM_T
*/
struct guiim_custom_key_param_tag
{
    GUIIM_TYPE_T im_type; /*!< 定制的输入法 */
    GUIIM_CUSTOM_KEY_T key; /*!< 定制的按键 */
    wchar key_values[GUIIM_CUSTOM_CHAR_LIMIT+1]; /*!< 按键上的字符列表 */
    uint32 key_count; /*!< 按键上的字符个数 */
    GUIIM_CUSTOM_KEY_PARAM_T *next_ptr; /*!< 单链表的下一个节点 */
};


/*! @struct GUIIM_METHOD_SET_T
@brief 输入法关联结构
@details 输入法属性配置，这个就是用来定义一个输入法的各个参数的。我们由多个参数来决定一个输入法。
\arg method 这个值肯定是要有的，并且唯一索引的
\arg lang 如果不设置，所有的语言都适用，默认GUIIM_LANG_NONE
\arg type 如果不设置，适用所有的输入类型，默认GUIIM_TYPE_NONE
\arg input 如果不设置，支持所有的输入方式，默认GUIIM_INPUT_TYPE_NONE
\arg capital 如果不设置，说该输入不区分大小写，默认GUIIM_MODE_MAX
*/
typedef struct
{
    GUIIM_METHOD_T method; /*!< 输入法 */
    GUIIM_LANGUAGE_T lang; /*!< 输入语言 */
    GUIIM_TYPE_T type; /*!< 输入类型 */
    GUIIM_INPUT_TYPE_T input; /*!< 输入方式 */
    GUIIM_CAPITAL_MODE_T capital; /*!< 大小写模式 */
} GUIIM_METHOD_SET_T;

/*! @typedef GUIIM_METHOD_CFG_T
@brief 定义为常数的配置结构类型
*/
typedef GUIIM_METHOD_SET_T const GUIIM_METHOD_CFG_T;



/*! @struct GUIIM_METHOD_PARAM_T
@brief 输入法的相关参数类型
@details 这是一个输入法相关的更详细的参数组合。
@note 
- input_lang
    - 当前实例的输入语言
    - 通常情况下会用默认的输入语言，即GUIIM_LANG_NONE - 去系统语言，即LOCAL
    - 默认情况下语言的切换会被记忆
    - 非默认情况下，不会有语言的切换选项 - 常见的设置为ENGLISH

- 全局相关的参数
\arg input_set 支持的输入方法集合
\arg type_set 支持的输入法类型集合
\arg limit 输入限制，这个是通过其他条件计算出来的最终结果

- 目前，limit的设置，是跟其他的限制功能重复的，是一个综合限制条件
*/
typedef struct
{
    GUIIM_METHOD_SET_T im_set; /*!< 输入法设定 */
    GUIIM_METHOD_CFG_T *im_def_ptr; /*!< 输入法定义数据地址 */

    GUIIM_LANGUAGE_T input_lang; /*!< 输入语言 */
    
    GUIIM_INPUT_TYPE_T input_set; /*!< 输入方式集合 */
    GUIIM_TYPE_T type_set; /*!< 输入类型集合 */
    
    GUIIM_LIMIT_T limit; /*!< 输入限制 */
    GUIIM_TAG_T tag; /*!< 输入法的附加属性 */
}GUIIM_METHOD_PARAM_T;


/*! @struct GUIIM_CONTEXT_T
@brief 获取上下文信息的结构
*/
typedef struct
{
    wchar* str_ptr; /*!< 当前编辑的文字指针 */
    uint32 str_len; /*!< 文字长度 */
    uint32 pos; /*!< 当前编辑的坐标 */
    
    BOOLEAN is_underline;   /*!highlight or not*/
    uint16 start_pos;      /*!highlight start pos*/
    uint16 end_pos;        /*!highlight end pos*/
    
} GUIIM_CONTEXT_T;

/*! @typedef GUIIM_GetContextFunction_T
@brief 获取上下文信息的函数类型
@warning - 控件向父控件获取数据的回调函数定义。
- 输入法与上层的交互，在一些应用中会做精细控制。
- 前提是编辑控件和输入法应用同时支持才可以。
@sa GUIIM_GetCapacityFunction_T
*/
typedef void (*GUIIM_GetContextFunction_T)(MMI_HANDLE_T, GUIIM_CONTEXT_T *);

/*! @struct GUIIM_CAPACITY_T
@brief 获取容量信息
*/
typedef struct
{
    uint32 max_len; /*!< 允许输入的最大长度 */
    uint32 cur_len; /*!< 当前输入长度 */
} GUIIM_CAPACITY_T;

/*! @typedef GUIIM_GetCapacityFunction_T
@brief 获取上下文容量信息的函数类型
@sa GUIIM_GetContextFunction_T
*/
typedef void (*GUIIM_GetCapacityFunction_T)(MMI_HANDLE_T, GUIIM_CAPACITY_T *);

/*! @struct GUIIM_INIT_KERNEL_T
@brief 控件给输入法应用的初始化参数
@note 初始化参数应该是尽量少的。这个和初始化输入法控件的参数是不同的。且用户不可见。
-# 用于显示app的
-# 用于初始设置输入法的
-# 特别需要的参数
*/
typedef struct
{
    CAF_HANDLE_T im_handle; /*!< 输入法控件handle */
    MMI_HANDLE_T win_handle; /*!< 窗口handle */
    MMI_WIN_ID_T win_id; /*!< 窗口id */

    GUI_LCD_DEV_INFO *lcd_info_ptr; /*!< 当前显示设备信息 */

    GUIIM_METHOD_PARAM_T *method_param_ptr; /*!< 输入法设置参数 */

    GUIIM_GetContextFunction_T f_get_context; /*!< 获取上下文函数 */
    GUIIM_GetCapacityFunction_T f_get_capacity; /*!< 获取容量信息函数 */
} GUIIM_INIT_KERNEL_T;


/*
*/
/*! @enum _GUIIM_EVENT_T
@brief 控件事件定义
@details 当控件需要想应用发出请求的时候，我们以事件的方式来传递参数
这些事件包括系统事件和空间定义的事件
-# 系统消息
    - 所有的系统消息会优先给应用处理，控件根据其返回值进行下一步处理
-# 控件定义消息
    - 控件对应用的控制部分
        -# draw ui - 当控件需要显示的时候，其实是显示的应用
        -# sleep - 将应用的状态清除，保留应用 - 这个需要不需要？
        -# force commit (underline) - 一些应用，比如密码输入，需要在极短的时间确定
        underline的内容，从而触发强制提交。虽然此处会出现输入仓促等情况导致用户
        体验很差，但是确实是特殊应用的特殊需求，引起的人后不良后果，是应用需要考
        虑的，im提供了杀人的刀，用还是不用
-# set类
    - 控件要求应用改变设置
        -# 输入法
        -# 输入语言、输入类型、输入方式、大小写模式
        -# 输入类型集合、输入方式集合
-# get类
    - 控件从应用获取应用实例的数据
        -# 输入法
        -# 输入语言、输入类型、输入方式、大小写模式
        -# 绘制区域、控制区域
        -# 编辑状态
-# custom类
    - 控件为满足特定应用的需求而定制一些配置
        -# 按键配置
        -# 虚拟键盘配置
@sa GUIIM_EVENT_T
*/
enum _GUIIM_EVENT_T
{
    GUIIM_EVENT_NONE,

    GUIIM_EVENT_SYS_MSG,

    GUIIM_EVENT_PAINT,
    GUIIM_EVENT_SLEEP,
    GUIIM_EVENT_FORCE_COMMIT,

    GUIIM_EVENT_SET_METHOD,
    GUIIM_EVENT_SET_LANG,
    GUIIM_EVENT_SET_TYPE,
    GUIIM_EVENT_SET_INPUT,
    GUIIM_EVENT_SET_CAPITAL,
    GUIIM_EVENT_SET_LIMIT,
    GUIIM_EVENT_SET_INPUT_SET,

    GUIIM_EVENT_GET_METHOD,
    GUIIM_EVENT_GET_LANG,
    GUIIM_EVENT_GET_TYPE,
    GUIIM_EVENT_GET_INPUT,
    GUIIM_EVENT_GET_CAPITAL,
    GUIIM_EVENT_GET_RECT,
    GUIIM_EVENT_GET_CTRL_RECT,
    GUIIM_EVENT_GET_EDIT_STATE,

    GUIIM_EVENT_CUSTOM_KEY,
    GUIIM_EVENT_CUSTOM_VKB,

    GUIIM_EVENT_MAX,
};

/*! @typedef GUIIM_EVENT_T
@brief 输入法控件事件类型
@sa _GUIIM_EVENT_T
*/
typedef uint8 GUIIM_EVENT_T;


/*! @enum _GUIIM_EDIT_STATE_T
@brief 输入法应用的输入状态
@details 精细区分输入法状态。
- 在不同的状态下，给出不同的softkey提示
-# 字符输入状态
    - 确定 x 删除
-# 字符候选且有小标签状态
    - 确定 x 取消
-# 字符候选且无小标签状态
    - 确定 x 返回
-# 手写状态
    保持不变
@sa GUIIM_EDIT_STATE_T
*/
enum _GUIIM_EDIT_STATE_T
{
    GUIIM_EDIT_NONE,
    GUIIM_EDIT_DEALING, /*!< 所有不区分的处理，统称，应用不想细分的时候，可统一返回 */
    GUIIM_EDIT_INPUTING,
    GUIIM_EDIT_SELECTING, /*!< 候选，且有小标签字符状态 */
    GUIIM_EDIT_CANDITATE, /*!< 候选，无小标签状态 */
    GUIIM_EDIT_HANDWRITING,
    GUIIM_EDIT_SPELL,       /*spell to add new word for samsung style*/
};

/*! @typedef GUIIM_EDIT_STATE_T
@brief 输入法输入状态类型
@sa _GUIIM_EDIT_STATE_T
*/
typedef uint8 GUIIM_EDIT_STATE_T;

/*! @union GUIIM_EVENT_DATA_U
@brief 与 GUIIM_EVENT_T 对应的参数
*/
typedef union
{
    struct
    {
        MMI_MESSAGE_ID_E msg_id;
        DPARAM param;
    } sys_msg;
    int32 paint;
    int32 sleep;

    GUIIM_METHOD_T set_method;
    GUIIM_LANGUAGE_T set_lang;
    GUIIM_TYPE_T set_type;
    GUIIM_INPUT_TYPE_T set_input;
    GUIIM_CAPITAL_MODE_T set_capital;
    GUIIM_TYPE_T set_limit;
    GUIIM_INPUT_TYPE_T set_input_set;

    GUIIM_METHOD_T get_method;
    GUIIM_LANGUAGE_T get_lang;
    GUIIM_TYPE_T get_type;
    GUIIM_INPUT_TYPE_T get_input;
    GUIIM_CAPITAL_MODE_T get_capital;
    GUI_RECT_T get_rect;
    GUI_RECT_T get_ctrl_rect;
    GUIIM_EDIT_STATE_T edit_state;

    GUIIM_CUSTOM_KEY_PARAM_T *custom_key_info_ptr;
} GUIIM_EVENT_DATA_U;


/*! @enum _GUIIM_NOTIFY_T
@brief 输入法应用的反馈
@details 都是通过输入法控件送出的
-# 提交类
    - 输入法应用向输入法控件提交输出，可提交字符、操作等
        -# commit char - 提交的确定字符和待定字符
        -# edit operatr - 长按删除、上下左右等----
-# 提示类
    - 输入法应用内部数据的变化，通知输入法控件
        -# 输入法应用自行设定了输入法、语言、切换了输入方式、大小写模式等
        -# 输入法应用的绘制区域、控制区域等发生了变化
-# 请求类
    - 输入法应用触发一个本身不能处理的操作，需要控件来完成
        -# 请求切换输入法
        -# 请求切换输入语言、输入类型、输入方式、大小写模式
        -# 请求刷屏
@sa GUIIM_NOTIFY_T
*/
enum _GUIIM_NOTIFY_T
{
    GUIIM_NOTIFY_NONE,

    GUIIM_NOTIFY_COMMIT,
	GUIIM_NOTIFY_DELETE,
    GUIIM_NOTIFY_LONG_DEL,

    GUIIM_NOTIFY_CHANGE_RECT,
    GUIIM_NOTIFY_CHANGE_EDIT_STATE,
    GUIIM_NOTIFY_SWITCH_METHOD,
    GUIIM_NOTIFY_SWITCH_CAPITAL,

    GUIIM_NOTIFY_SET_METHOD,
    GUIIM_NOTIFY_SET_LANG,
    GUIIM_NOTIFY_SET_TYPE,
    GUIIM_NOTIFY_SET_INPUT,
    GUIIM_NOTIFY_SET_CAPITAL,
    GUIIM_NOTIFY_PAINT,

    GUIIM_NOTIFY_TRANS_MSG,

    GUIIM_NOTIFY_HIDE,

    GUIIM_NOTIFY_SETCURSOR,
    GUIIM_NOTIFY_GETCURSOR,

    GUIIM_NOTIFY_SET_HIGHLIGHT,
    GUIIM_NOTIFY_COMMIT_UNDERLINE,
    
    GUIIM_NOTIFY_AUTO_SWITCH_METHOD,

    GUIIM_NOTIFY_MAX,
};

/*! @typedef GUIIM_NOTIFY_T
@brief 输入法应用的反馈类型定义
@sa _GUIIM_NOTIFY_T
*/
typedef uint8 GUIIM_NOTIFY_T;


/*! @struct GUIIM_COMMIT_DATA_T
@brief 返回给应用的消息参数
*/
typedef struct
{
    wchar commit[GUIIM_COMMIT_BUFFER_MAX + 1];
    uint16 commit_len;
    wchar underline[GUIIM_COMMIT_BUFFER_MAX + 1];
    uint16 underline_len;
} GUIIM_COMMIT_DATA_T;

typedef struct
{
    MMI_MESSAGE_ID_E msg_id;
    DPARAM param;    
} GUIIM_TRANS_MSG_DATA_T;

typedef enum
{
    GUIIM_SETCURSOR_TYPE_CUR,           // set offset value by current cursor
    GUIIM_SETCURSOR_TYPE_START,         // set offset value by start
    GUIIM_SETCURSOR_TYPE_END,           // set offset value by end

    GUIIM_SETCURSOR_TYPE_MAX
} GUIIM_SETCURSOR_TYPE_E;

// these macro is used by guiedit and guiim to pack up the param data
#define MAKE_CURSOR_PARAM(type, value)  ((type<<16)|(value&0xffff))
#define SETCURSOR_TYPE(v)               (((uint32)v)>>16)
#define SETCURSOR_VALUE(v)              ((uint16)v&0xffff)

typedef struct
{
    GUIIM_SETCURSOR_TYPE_E  type;       // set cursor type
    int16                   value;      // the cursor offset
} GUIIM_SETCURSOR_MSG_DATA_T;

typedef struct
{
    uint16                  start_index;
    uint16                  end_index;
} GUIIM_SET_HIGHLIGHT_MSG_DATA_T;

/*! @union GUIIM_NOTIFY_DATA_U
@brief 与 GUIIM_NOTIFY_T 对应的数据的联合定义
*/
typedef union
{
    GUIIM_COMMIT_DATA_T commit_data;
    uint32 long_del;

    GUI_RECT_T change_rect;
    GUIIM_EDIT_STATE_T edit_state;
    GUIIM_METHOD_T switch_method;
    GUIIM_LANGUAGE_T switch_lang;
    GUIIM_TYPE_T switch_type;
    GUIIM_INPUT_TYPE_T switch_input;
    GUIIM_CAPITAL_MODE_T switch_capital;

    GUIIM_METHOD_T set_method;
    GUIIM_LANGUAGE_T set_lang;
    GUIIM_TYPE_T set_type;
    GUIIM_INPUT_TYPE_T set_input;
    GUIIM_CAPITAL_MODE_T set_capital;

    GUIIM_TRANS_MSG_DATA_T trans_msg;

    GUIIM_SETCURSOR_MSG_DATA_T  setcursor_msg;
    uint16                      getcursor_value;

    GUIIM_SET_HIGHLIGHT_MSG_DATA_T  set_highlight_msg;
    uint32 commit_underline;

    uint32 paint;
} GUIIM_NOTIFY_DATA_U;

/*! @struct GUIIM_NOTIFY_PACK_T
@brief notify 消息队列中数据包的定义
*/
typedef struct
{
    GUIIM_NOTIFY_T notify;
    GUIIM_NOTIFY_DATA_U data;
}GUIIM_NOTIFY_PACK_T;

/*! @struct GUIIM_NOTIFY_QUEUE_T
@brief notify 消息队列定义
*/
typedef struct
{
    GUIIM_NOTIFY_PACK_T pack[GUIIM_NOTIFY_QUEUE_LEN];
    int8 pack_count;
    int8 pack_index;
}GUIIM_NOTIFY_QUEUE_T;


/*! @typedef GUIIM_APIS_INITIALIZE_T
@brief 应用初始化函数
@sa GUIIM_APIS_TERMINATE_T
@sa GUIIM_APIS_HANDLE_T
*/
typedef void* (*GUIIM_APIS_INITIALIZE_T)(GUIIM_INIT_KERNEL_T *init_data_ptr);
/*! @typedef GUIIM_APIS_TERMINATE_T
@brief 应用销毁函数
@sa GUIIM_APIS_INITIALIZE_T
@sa GUIIM_APIS_HANDLE_T
*/
typedef BOOLEAN (*GUIIM_APIS_TERMINATE_T)(void *handle);
/*! @typedef GUIIM_APIS_HANDLE_T
@brief 应用事件处理函数
@sa GUIIM_APIS_INITIALIZE_T
@sa GUIIM_APIS_TERMINATE_T
*/
typedef BOOLEAN (*GUIIM_APIS_HANDLE_T)(void* handle, GUIIM_EVENT_T event, GUIIM_EVENT_DATA_U *data_ptr);

/*! @struct guiim_api_tag
@brief 处理方法的打包结构，方便统一进出
*/
struct guiim_api_tag
{
    GUIIM_APIS_INITIALIZE_T init;
    GUIIM_APIS_TERMINATE_T term;
    GUIIM_APIS_HANDLE_T handle;
};

/*! @struct GUIIM_APIS_T
@brief 处理方法的打包结构
*/
typedef struct guiim_api_tag GUIIM_APIS_T;

/*! @typedef GUIIM_KERNEL_APIS_T
@brief 处理方法的打包结构
*/
typedef GUIIM_APIS_T GUIIM_KERNEL_APIS_T;

/*! @typedef GUIIM_APIS_PTR_T
@brief 处理方法的打包结构指针
*/
typedef GUIIM_APIS_T const *GUIIM_APIS_PTR_T;

/*! @struct GUIIM_APP_CFG_T
@brief 输入法内核关联结构
*/
typedef struct
{
    GUIIM_METHOD_T method;
    GUIIM_APIS_PTR_T apis_ptr;
} GUIIM_APP_CFG_T;

/*! @struct GUIIM_ICON_CFG_T
@brief 输入法相关的图标定义
*/
typedef struct
{
    GUIIM_METHOD_T method;
    MMI_IMAGE_ID_T icon;
} GUIIM_ICON_CFG_T;

/*! @struct GUIIM_TEXT_CFG_T
@brief 输入法相关的图标定义
*/
typedef struct
{
    GUIIM_METHOD_T method;
    wchar text[GUIIM_TEXT_CFG_MAX_LEN];
} GUIIM_TEXT_CFG_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief 输入法应用向输入法控件的处理消息队列增加记录
@author Lianxiang.Zhou
@param im_handle [in] 输入法控件句柄
@param pack_data_ptr [in] 消息数据指针
@retval TRUE 设置消息成功
@retval FALSE 设置消息失败
@return 返回设置是否成功
@note 这个函数的执行是异步的，在返回输入法控件的消息处理之后，统一处理消息队列
@sa GUIIM_CommitData
*******************************************************************************/
PUBLIC BOOLEAN CTRLIM_SetNotify(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);


/***************************************************************************//*!
@brief 提交输入法消息到控件
@author Lianxiang.Zhou
@param im_handle [in] 输入法控件句柄
@param pack_data_ptr [in] 消息数据指针
@retval TRUE 设置消息成功
@retval FALSE 设置消息失败
@return 返回设置是否成功
@note 这个是同步处理的，消息会被立刻处理提交到输入法控件的父控件
@sa GUIIM_SetNotify
*******************************************************************************/
PUBLIC BOOLEAN CTRLIM_CommitData(
    MMI_HANDLE_T im_handle,
    GUIIM_NOTIFY_PACK_T *pack_data_ptr
);

/***************************************************************************//*!
@brief 记录一个和输入法控件相关的窗口
@author Lianxiang.Zhou
@param im_handle [in] 输入法句柄
@param win_handle [in] 要记录的窗口句柄
@note 一些输入法控件相关的窗口，在输入法控件关闭之后，要将其关闭。
@sa GUIIM_RemoveWin
*******************************************************************************/
PUBLIC void CTRLIM_AppendWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
);

/***************************************************************************//*!
@brief 移除和输入法控件相关的窗口，即输入法控件不再关注该窗口
@author Lianxiang.Zhou
@param im_handle [in] 输入法句柄
@param win_handle [in] 要记录的窗口句柄
@note 一些输入法控件相关的窗口，在输入法控件关闭之后，要将其关闭。
@sa GUIIM_RemoveWin
*******************************************************************************/
PUBLIC void CTRLIM_RemoveWin(
    MMI_HANDLE_T im_handle,
    CAF_HANDLE_T win_handle
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
