/*==============================================================================
File Name: mmiim_touch_common.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_COMMON_H_
#define _MMIIM_TOUCH_COMMON_H_





/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_draw.h"

#if defined (MMI_IM_PDA_SUPPORT)

#include "guistring.h"

#include "mmiim.h"
#include "mmiim_internal.h"

#include "mmiim_image.h"
#include "mmiim_text.h"



typedef struct KEY_INFO_TAG KEY_INFO_T;
typedef struct VKEYBOARD_TAG VKEYBOARD_T;
typedef struct IM_DATA_TAG IM_DATA_T;

#define SCI_FREEIF(X)       \
    do {                    \
        if (X != PNULL)      \
        {                   \
            SCI_FREE(X);    \
            X = PNULL;       \
        }                   \
    }                       \
    while(0)

/*! 智能大小写功能简介:
--1.当处于LeadUpper状态时，输入一个字符或单词后，自动切换到lower
--2.当处于Lower状态，如果不是用户自己切换到的lower状态，则在一句话结束后，恢复到LeadUpper状态
*/
#define MMIIM_TOUCH_SMART_CAPS      //智能英文大小写


#define VKB_SLIDING_THRESHOLD (5) //n成，也就是十分之几
#define VKB_EFFECT_FRAME_CNT (12) //vkb特效的显示帧数
#define TIP_EXPAND_MULTIPLE ((float)1.6)     //tip放大按键的倍数，如果显示不下，则按最大字号显示
#define ABC_COMMIT_TICK (1100)   
#define TEXT_LEN_LIMIT (32)
#define PRINT_TEXT_LEN_LIMIT (TEXT_LEN_LIMIT)		//暂时以 TEXT_LEN_LIMIT来表示，与前期程序保持一致，虽然实际显示时，可能就1~8个字符 
#define SUBTEXT_LEN_LIMIT (1)
#define CAND_PAGE_LIMIT (10) //正常输入10页差不多了
#define LATIN_CANDIDATE_ROW (5)
#define LATIN_CANDIDATE_COUNT (32) //英文单页可以显示的最大个数
#define CH_CANDIDATE_COL (4)
#define CH_CANDIDATE_ROW (4)
//汉字保持20个侯选字就可以了 4*4, 根据具体的键盘布局设计更改
#define CH_CANDIDATE_COUNT (CH_CANDIDATE_COL*CH_CANDIDATE_ROW)
//没有拼音候选，多一行, 为扩展模式(expand)
#define CH_CANDIDATE_COUNT_E (CH_CANDIDATE_COL*(CH_CANDIDATE_ROW + 1))
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#define HW_FULLSCREEN_EXPAND_CAND_COL 4
#define HW_FULLSCREEN_EXPAND_CAND_ROW 3
#define HW_FULLSCREEN_EXPAND_CAND_COUNT (HW_FULLSCREEN_EXPAND_CAND_COL * HW_FULLSCREEN_EXPAND_CAND_ROW)
#endif
#define PINYIN_SEPARATOR ('\'')
#define ASSO_PAGE_LIMIT (3)
#define PINYIN_CONFIRM_LIMIT (8) //这是一个模糊的值，当已经确定了这么多之后，就不许再输入了
#define ZHUYIN_COMMIT_TICK (1200)
#define ZHUYIN_CONFIRM_LIMIT (8) //这是一个模糊的值，当已经确定了这么多之后，就不许再输入了

#if defined IM_HANDWRITING_HANWANG
#define HW_TRACE_CNT_LIMIT (512)
#elif defined IM_HANDWRITING_SOGOU
#define HW_TRACE_CNT_LIMIT (512)
#endif
#define HW_RECOGNIZE_TICK (900)
#define HW_TRACE_WIDTH (2*MMIIM_HW_THICKNESS_MAX - 1)

#define CAND_WIDTH_MIN (32)

#define KEY_CHAR_LINE_MAX_CHARS_NUM (4)     //9键键盘字符按键一行最多显示字符个数,不包含数字字符

#define VKB_PAGE_MAX	(4)		//一种输入法的虚拟键盘张数最大值

#if (defined IM_ENGINE_CSTAR || defined IM_ENGINE_SOGOU) 
#define CHECK_DATA(data_ptr) \
    do {\
        SCI_ASSERT(PNULL != data_ptr); /*assert verified*/ \
        SCI_ASSERT(sizeof(IM_DATA_T) == data_ptr->check_info); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_term_imm); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_set_method); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_is_inputting); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_draw_all); /*assert verified*/ \
    } while (0)
#else
#define CHECK_DATA(data_ptr) \
    do {\
        SCI_ASSERT(PNULL != data_ptr); /*assert verified*/ \
        SCI_ASSERT(sizeof(IM_DATA_T) == data_ptr->check_info); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->init_param.method_param_ptr->im_def_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_term_imm); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_set_method); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->f_draw_all); /*assert verified*/ \
    } while (0)
#endif

#define CHECK_DATA_EX(data_ptr) \
    do {\
        CHECK_DATA(data_ptr); \
        SCI_ASSERT(PNULL != data_ptr->vkb.keys); /*assert verified*/ \
        SCI_ASSERT(PNULL != data_ptr->data); /*assert verified*/ \
    } while (0)





/*! @enum VKEY_CODE_TAG
@brief 虚拟键值定义
@note
- 定制键部分
- 候选建
- 以上两个，是在定制键盘的时候要确定的，任何一个键盘都是独立设计的
*/
enum VKEY_CODE_TAG
{
    VKEY_NULL,

    VKEY_1,
    VKEY_2,
    VKEY_3,
    VKEY_4,
    VKEY_5,
    VKEY_6,
    VKEY_7,
    VKEY_8,
    VKEY_9,
    VKEY_0,

    VKEY_A,
    VKEY_B,
    VKEY_C,
    VKEY_D,
    VKEY_E,
    VKEY_F,
    VKEY_G,
    VKEY_H,
    VKEY_I,
    VKEY_J,
    VKEY_K,
    VKEY_L,
    VKEY_M,
    VKEY_N,
    VKEY_O,
    VKEY_P,
    VKEY_Q,
    VKEY_R,
    VKEY_S,
    VKEY_T,
    VKEY_U,
    VKEY_V,
    VKEY_W,
    VKEY_X,
    VKEY_Y,
    VKEY_Z,

    //此处为某些语言不使用普通键盘按键配置，则无法使用abc等键来表示含义
    //添加一些不规则按键处理，
    //此按键键码与ABC值相同，防止电话本查询，偏移量等出现问题
    VKEY_IRREGULAR = VKEY_A,        
    VKEY_IRREGULAR_1 = VKEY_IRREGULAR,      //VKEY_A
    VKEY_IRREGULAR_2,
    VKEY_IRREGULAR_3,
    VKEY_IRREGULAR_4,
    VKEY_IRREGULAR_5,
    VKEY_IRREGULAR_6,
    VKEY_IRREGULAR_7,
    VKEY_IRREGULAR_8,
    VKEY_IRREGULAR_9,
    VKEY_IRREGULAR_10,
    VKEY_IRREGULAR_11,
    VKEY_IRREGULAR_12,
    VKEY_IRREGULAR_13,
    VKEY_IRREGULAR_14,
    VKEY_IRREGULAR_15,
    VKEY_IRREGULAR_16,
    VKEY_IRREGULAR_17,
    VKEY_IRREGULAR_18,
    VKEY_IRREGULAR_19,
    VKEY_IRREGULAR_20,
    VKEY_IRREGULAR_21,
    VKEY_IRREGULAR_22,
    VKEY_IRREGULAR_23,
    VKEY_IRREGULAR_24,
    VKEY_IRREGULAR_25,
    VKEY_IRREGULAR_26,          //VKEY_Z
    VKEY_IRREGULAR_27,
    VKEY_IRREGULAR_28,
    VKEY_IRREGULAR_29,
    VKEY_IRREGULAR_30,
    VKEY_IRREGULAR_31,
    VKEY_IRREGULAR_32,
    VKEY_IRREGULAR_33,
    VKEY_IRREGULAR_34,
    VKEY_IRREGULAR_35,
    VKEY_IRREGULAR_36,
    VKEY_IRREGULAR_37,
    VKEY_IRREGULAR_38,
    VKEY_IRREGULAR_39,
    VKEY_IRREGULAR_40,
    VKEY_IRREGULAR_MAX = VKEY_IRREGULAR + 40,

    VKEY_SPACE,
    VKEY_DEL,
    VKEY_ENTER,
    VKEY_CAPS,
    VKEY_123,
    VKEY_SYMBOL,
    VKEY_123SYMBOL,
    VKEY_SWITCH,
    VKEY_SET,
    VKEY_LANGUAGE_SET,  //本地语和英文之间切换
    VKEY_HIDE,
    VKEY_EXPAND,
    VKEY_PAGEUP,
    VKEY_PAGEDOWN,
    VKEY_PY_PAGEUP,
    VKEY_PY_PAGEDOWN,
    VKEY_PY_SEPARATE,
    VKEY_FACE,
    VKEY_BACK,
    VKEY_LOCK,
    VKEY_PAGE,
    VKEY_DOT,       //数字的小数点
    VKEY_PERIOD,    //点号，与数据表的period相同，与上面的小数点做一个区分，以免混乱
    VKEY_PLUS,
    VKEY_STAR,
    VKEY_HASH,	//#
    VKEY_POUND,
    VKEY_MULTI_CHAR,    //这种按键配置为连续按键，输入按键上的不同字符
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    VKEY_WIN_PANEL,     //显示窗口区域按键，即非手写面板挡住窗口的部分
#endif
    VKEY_WRITE_PANEL,
	VKEY_SWITCH_VKB,	//多张键盘之间切换
	
    VKEY_CUSTOM,
    VKEY_CUSTOM_MAX = VKEY_CUSTOM + 10,

    VKEY_CANDIDATE,
    VKEY_CANDIDATE_MAX = VKEY_CANDIDATE + 64,

    VKEY_PY_CANDIDATE,
    VKEY_PY_CANDIDATE_MAX = VKEY_PY_CANDIDATE + 40,

    VKEY_SYMB_STR,
    VKEY_SYMB_STR_MAX = VKEY_SYMB_STR + 40,
};
typedef uint16 VKEY_CODE_T;



enum EDIT_STATE_TAG
{
    EDIT_STATE_NONE,
    EDIT_STATE_SEARCH,
    EDIT_STATE_WRITE,
    EDIT_STATE_COMMIT_DEFAULT,
    EDIT_STATE_ASSO,
};
typedef uint8 EDIT_STATE_T;


#if defined MMIIM_TOUCH_SMART_CAPS
enum CAPS_SWITCH_TYPE_E_
{
    CAPS_SWITCH_TYPE_NONE_E,    //初始化状态，大小写由初始化决定的，此状态没有设置
    CAPS_SWITCH_TYPE_USER_E,      //用户自己切换的大小写
    CAPS_SWITCH_TYPE_COMMIT_E,    //提交字符后，发生大小写切换
    CAPS_SWITCH_TYPE_CURSOR_E,    //光标移动后，发生大小写切换

    CAPS_SWITCH_TYPE_MAX_E,
};
typedef uint8 CAPS_SWITCH_TYPE_E;
#endif



/*! @enum KEY_STYLE_TAG
@brief 按键的分类，我们将panel上的item都设置成按键
*/
enum KEY_STYLE_TAG
{
    KEY_STYLE_CHARACTER,        /*!< 一般的字符按键 */
    KEY_STYLE_CONTROL,          /*!< 一般的控制按键 */
    KEY_STYLE_CUSTOM,           /*!< 定制的按键 */
    KEY_STYLE_CANDIDATE,        /*!< 候选按键 */
    KEY_STYLE_SYSTEM,           /*!< 系统级的控制案件，比较特殊的控制按键 */
    KEY_STYLE_ART,              /*!< 装饰部分 */
};
typedef uint8 KEY_STYLE_T;


//key add style-character,字符按键丝印数据从数据表读取，而显示时，因空间不够及其他原因，无法满足所有要求，
//故添加一个字段表示该属性
//因客户需求较多，这里做一个变更:低0~2位表示text字段属性，3,4位表示sub_text属性

#define KASC_TEXT_ATTR (7)		//((1 << 3) - 1)
#define KASC_SUBTEXT_TEXT_ATTR (24)	//((1 << 5) - KASC_TEXT_ATTR)	//

typedef enum _KEY_ADD_STYLE_CHARACTER_E
{
	//下面的情形默认表示subtext处于左上角显示
    KASC_EXCLUDE_DIGIT,      //单行显示所有字符，但是如果最后一个字符为数字，则不显示数字
    KASC_ALL_TEXT,      //单行显示所有字符，主要适用于subtext不存在的情况

    KASC_FIRST,      //显示第一个字符
    KASC_SELECT_CHAR,       //显示所有的字符，分开显示，并有一个高亮字符表示选择状态，高亮字符也可能不存在

    KASC_2LINE_EXCLUDE_DIGIT,      //两行显示所有字符，但是如果最后一个字符为数字，则不显示数字
    KASC_2LINE,      //两行显示所有字符

    KASC_2X4_EXCLUDE_DIGIT,      //两行显示，每行最大4个字符，但是如果最后一个字符为数字，则不显示数字
    KASC_2X4,      //两行显示，每行最大4个字符

	//下面的情形表示subtext居中显示，其他情况即text显示与前面的组合使用
	KASC_SUBTEXT_LEFT = 0,		//默认状态，subtext靠左侧显示
	KASC_SUBTEXT_MIDDLE = 8,	//这个与前面的发生组合，表示同时支持，这个组合应该不要和2行的混合使用，否则一个键上显示3行或者重合，均不划算
	KASC_SUBTEXT_RIGHT = 24,	//这个与前面的发生组合，表示同时支持，这项操作暂时没有处理，后期有需要再添加
	
} KEY_ADD_STYLE_CHARACTER_E;

//#if defined MMIIM_HW_FULLSCREEN_SUPPORT
//key add style-art:装饰部分没有做一个区分，某些按键的装饰部分可能不同
//故添加一个字段表示该属性
typedef enum _KEY_ADD_STYLE_ART_E
{
    KASA_DEFAULT_E,

    KASA_TRANSPARENT_E,     //该装饰按键绘制透明色
    KASA_NOT_DRAW_E,     //该装饰按键绘制面板背景色
    KASA_NOT_BG_E,     //该装饰按键不填充背景
    
    KASA_MAX_E,
}KEY_ADD_STYLE_ART_E;
//#endif

typedef union _KEY_ADD_STYLE_T
{
    KEY_ADD_STYLE_CHARACTER_E text_disp;       //text 字符串显示方式
//#if defined MMIIM_HW_FULLSCREEN_SUPPORT	
    KEY_ADD_STYLE_ART_E art_disp;       //art按键显示方式
//#endif	    
} KEY_ADD_STYLE_T;


/*! @struct KEY_THEME_TAG
@brief 按键的显示定义
*/
typedef struct KEY_THEME_TAG
{
    SHARP_T sharp;              /*!< 形状 */
    BORDER_T border;            /*!< 边框 */
    FILL_T fill;                /*!< 填充类型 */
    FILL_DATA_T fill_data;      /*!< 填充数据 */
    uint32 text_color;          /*!< 主文字颜色 */
    uint32 subtext_color;       /*!< 次文字颜色 */
    GUI_FONT_T text_font;       /*!< 主文字字体 */
    GUI_FONT_T subtext_font;    /*!< 次文字字体 */
} KEY_THEME_T;

/*! @struct MMIIM_TOUCH_THEME_TAG
@brief 输入法键盘显示定义
*/
typedef struct MMIIM_TOUCH_THEME_TAG
{
    //general define
    uint8 alpha;                            /*!< 面板的全局alpha */
    GUI_FONT_T font_min;                    /*!< 最小字体 */
    GUI_FONT_T font_max;                    /*!< 最大字体 */

    //panel define
    SHARP_T panel_sharp;                    /*!< 键盘形状 */
    uint8 panel_percent;                /*!< 键盘大小 */
    uint32 panel_color;                     /*!< 键盘颜色 */
    BORDER_T panel_border;                  /*!< 键盘边框 */

    //key define
    //1 key size follow panel size
    uint8 key_hor_space;                        /*!< 键间水平空白 */
    uint8 key_ver_space;                        /*!< 键间垂直空白 */  
    uint8 cand_hor_space;                        /*!< 候选字水平空白 */ 
    uint8 cand_ver_space;                        /*!< 候选字垂直空白 */ 
    uint8 key_padding;                      /*!< 键内空白 */
    KEY_THEME_T key_char[KEY_STATE_COUNT];  /*!< 字符按键 */
    KEY_THEME_T key_ctrl[KEY_STATE_COUNT];  /*!< 控制按键 */
    KEY_THEME_T key_cust[KEY_STATE_COUNT];  /*!< 定制按键 */
    KEY_THEME_T key_cand[KEY_STATE_COUNT];  /*!< 候选按键 */
    KEY_THEME_T key_sys[KEY_STATE_COUNT];   /*!< 系统按键 */
    KEY_THEME_T key_art[KEY_STATE_COUNT];   /*!< 装饰按键，对于装饰按键，我们用状态定义不同装饰 */

    //tip define
    //1 tip size follow panel size
    //2 key tip theme follow key theme
    KEY_THEME_T tip_theme;                  /*!< 按键tip显示 */
    uint8 tip_padding;                      /*!< 提示框内空白 */
    uint8 tip_alpha;                        /*!< tip的特别透明处理 */
    uint16 tip_delay;                       /*!< tip延迟消失的时间 */

    //提示信息消失时间
    uint16 cue_delay;                       /*!< cue延迟消失的时间 */

    //edit define
    // 1 edit size follow panel size
    // 2 edit 采用简单的方框写字的形式
    uint32 edit_border_color;               /*!< 主文字颜色 */
    uint32 edit_bg_color;                   /*!< 次文字颜色 */
    uint32 edit_text_color;                 /*!< 主文字颜色 */
    GUI_FONT_T edit_text_font;              /*!< 主文字字体 */
    uint8 edit_padding;                     /*!< edit的内部留空 */
} THEME_T;


enum KEY_TAG_TAG
{
    KEY_TAG_NONE = 0x00,
    KEY_TAG_NORMAL = 0x01,
    KEY_TAG_EXPAND = 0x02,
};
typedef uint8 KEY_TAG_T;


typedef BOOLEAN(*KEY_TP_FUNC_T)(IM_DATA_T *, KEY_INFO_T *);
typedef BOOLEAN(*KEY_DRAW_T)(IM_DATA_T const *, GUILCD_DRAW_DC_T const *, KEY_INFO_T *);

/*! @struct KEY_INFO_TAG
@brief 按键的信息
@note
- 为了修正各key之间的距离一致，我们将统一使用"左边"来记，即我们的right是右边的一个的left

subtext
---------
icon text

*/
struct KEY_INFO_TAG
{
    GUI_RECT_T rect;                    /*!< 在panel上的相对位置，panel的虚拟大小为1000*1000 */
    uint16 code;                        /*!< 按键的代码 */
    uint8 is_inactive;                  /*!< 是否激活 */
    uint8 is_press;                     /*!< 是否按下 */
    uint8 is_hide;                      /*!< 时候隐藏 */
	uint8 is_paint;                      /*!< if true, will paint, or not paint this key when paint the keyboard*/
    uint8 has_tip;                      /*!< 是否显示tips */
    KEY_TAG_T tag;                      /*!< 键标签，用于标识按键用途 */
    //display style
    KEY_STYLE_T style;                  /*!< 类型，会按照类型决定显示的主题 */
    KEY_ADD_STYLE_T add_style;                  /*!< 附加类型，会按照类型决定附加类型，附加类型决定显示的主题 */
    //display data
    MMI_IMAGE_ID_T icon;                /*!< 图标 */
    wchar text[TEXT_LEN_LIMIT];         /*!< 字符 ，按键码相关的字符，用来查询联想字使用的unicode，提交的字符也从这个字段读取，而不是从display字段读取*/
    wchar print_text[PRINT_TEXT_LEN_LIMIT];         /*!< 显示字符串 ，与text的区别是，这个字符串仅仅用来按键显示*/
    wchar subtext[SUBTEXT_LEN_LIMIT];   /*!< 附加字符 */
    uint8 text_len;                     /*!< 字符长度 */
    uint8 print_text_len;                     /*!< 显示字符长度 */
    uint8 subtext_len;                  /*!< 附加字符长度 */
    int8 font_size;                     /*!< 字体大小的调整 */
    //key function
    KEY_TP_FUNC_T f_down;
    KEY_TP_FUNC_T f_move;
    KEY_TP_FUNC_T f_long;
    KEY_TP_FUNC_T f_up;
    KEY_DRAW_T f_draw;  //默认按键绘制函数均按DrawKey处理，如果需要设置，另行处理
};

enum CAPITTAL_MODE_TAG
{
    CAPITTAL_MODE_LOWER,
    CAPITTAL_MODE_UPPER,
    CAPITTAL_MODE_INITIAL,
};
typedef uint8 CAPITTAL_MODE_T;


/*! @enum KEYBOARD_STATE_TAG
@brief 键盘状态，用于记录键盘的即时信息
*/
enum KEYBOARD_STATE_TAG
{
    KEYBOARD_INITIAL = 0x0000,
    KEYBOARD_CANDIDATE = 0x0001,
    KEYBOARD_ASSOCIATE = 0x0002,
    KEYBOARD_CANDIDATE_EXPAND = 0x0004,
    KEYBOARD_ASSOCIATE_EXPAND = 0x0008,
    KEYBOARD_CHOOSE_NUMBER = 0x0010,
    KEYBOARD_CHOOSE_SYMBOL = 0x0020,
    KEYBOARD_CHOOSE_NUMBER_LOCK = 0x0040,
    KEYBOARD_CHOOSE_SYMBOL_LOCK = 0x0080,
    KEYBOARD_CHOOSE_LETTER = 0x0100,        //多国语言输入按键后，选择候选字符状态
    KEYBOARD_CHOOSE_MENU = 0x0200,        //类似iphone菜单选择状态
};

//全键盘布局类型
typedef enum _QWERT_KEYBOARD_LAYOUT_TYPE_E
{
    KEYBOARD_LAYOUT_26CHAR,     //KEYBOARD_LAYOUT_QWERT_DEFAULT,
    KEYBOARD_LAYOUT_28CHAR,   
    KEYBOARD_LAYOUT_40CHAR, 

	KEYBOARD_LAYOUT_ERR, 
}QWERT_KEYBOARD_LAYOUT_TYPE_E;

typedef uint32 KEYBOARD_STATE_T;

typedef BOOLEAN(*KB_SLIDING_FUNC_T)(IM_DATA_T*, int32);

/*! @struct VKEYBOARD_TAG
@brief 虚拟键盘的数据
*/
struct VKEYBOARD_TAG
{
    //key board define
    uint8 key_cnt;                              /*!< 按键数，我们一下子创建所有的按键，隐藏部分按键 */
    KEY_INFO_T *keys;                           /*!< 申请连续的空间，简化操作 */
    //kb state
    CAPITTAL_MODE_T cap;
    KEY_INFO_T *caps_key;
    KEY_INFO_T *hide_key;
    KEY_INFO_T *expand_key;
    KEY_INFO_T *vessel_key;
    KEY_INFO_T *pgup_key;
    KEY_INFO_T *pgdn_key;
    KEY_INFO_T *py_vessel_key;
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
    KEY_INFO_T *bg_key;
#endif    
    KEY_INFO_T *py_pgup_key;
    KEY_INFO_T *py_pgdn_key;
    KEY_INFO_T *back_key;
    KEY_INFO_T *lock_key;
    KEY_INFO_T *tip_key;
    KEYBOARD_STATE_T state;
	uint8 is_paint;				//need to paint
	uint8 charactor_first;		//使用到键盘数据表中的第一个按键，index值
    uint8 custom_first;
    uint8 custom_cnt;
    uint8 candidate_first;
    uint8 candidate_cnt;
    uint8 py_candidate_first;
    uint8 py_candidate_cnt;
    GUI_POINT_T down_point;                     /*!< 触笔按下得位置 */
    KEY_INFO_T *first_key;                      /*!< 触笔down的按键 */
    KEY_INFO_T *last_key;                       /*!< 最后的按键 */
    uint8 is_key_block;                         /*!< 键盘被锁定在last key，特殊的应用，比如手写 */
    uint8 is_expand;
    //funtion
    KB_SLIDING_FUNC_T f_sliding;
};


/* 设置窗口的数据信息 */
typedef struct
{
    GUIIM_LANGUAGE_T lang;
    GUIIM_TYPE_T type;
    GUIIM_INPUT_TYPE_T input;
    MMI_TEXT_ID_T text_id;
} SETTING_MENU_DATA_T;


typedef struct MULTI_TAP_TAG
{
    KEY_INFO_T      *last_key;
    uint8           last_key_index;	//连续按键的次数 - 按键字符个数 * 循环次数
    uint8           commit_timer;
} MULTI_TAP_T;


//某些输入法存在多张键盘的情况，在多数情况下，该值无效，但特殊语言需要，如藏语
//这里有一个约定，输入法进入符号面板或进入设置窗口等，返回后，还是进入第一张键盘，
//这种设计方案可以减少键盘页码管理的问题，iphone也是这种设计
typedef struct VKB_MULTI_PAGE_TAG
{
	uint8           total_page;	//当前输入法存在键盘的张数
    uint8           cur_page;	//当前键盘的第几页，从0开始计算
    uint16           reserved;	//当前键盘的第几页
	QWERT_KEYBOARD_LAYOUT_TYPE_E layout[VKB_PAGE_MAX];
} VKB_MULTI_PAGE_T;


//输入法选择菜单操作方式
typedef enum _E_IM_SET_SELECT_TYPE 
{
    EISST_NONE,
    EISST_MOVE_SELECT,     //点击后，move选择菜单，类似iphone操作长按风格后的菜单
    EISST_CLICK_SELECT,    //点击后，弹出选择菜单，再次点击    选择菜单，类似右键菜单的功能
} E_IM_SET_SELECT_TYPE;

typedef void (*F_TermImm_T)(IM_DATA_T *data_ptr);
typedef BOOLEAN(*F_SetMethod_T)(IM_DATA_T *data_ptr, GUIIM_METHOD_T method);
typedef BOOLEAN(*F_IsInputting_T)(IM_DATA_T *data_ptr);
typedef BOOLEAN(*F_DrawAll_T)(IM_DATA_T *data_ptr);

/*! @struct DATA_TAG
@brief 输入法数据
*/
struct IM_DATA_TAG
{
    uint32                  check_info;         /*!< 数据的检查信息，用于简单的数据检测 */
    CAF_HANDLE_T            ctrl_handle;        /*!< 输入法句柄 - 输入法控件信息 */
    GUIIM_INIT_KERNEL_T     init_param;         /*!< 初始化参数 - 输入法控件信息 */

    THEME_T                 theme;              /*!< 显示主题 */

    GUI_LCD_DEV_INFO        ui_layer;           /*!< 主要的数据显示层 */
    GUI_LCD_DEV_INFO        tip_layer;          /*!< 提示数据显示层 */
    GUI_LCD_DEV_INFO        edit_layer;         /*!< 预编辑数据显示层 */
    GUI_LCD_DEV_INFO        setting_im_layer;   /*!< 改变输入法显示层 */

    GUI_RECT_T              tip_rect;           /*!< tip层的大小和位置信息 */
    GUI_RECT_T              edit_rect;          /*!< 预编辑层的大小和位置信息 */

    BOOLEAN                 is_tip_display;     /*!< 提示框是否显示 */
    BOOLEAN                 is_setting_display; /*!< 切换输入法是否显示 */
    uint8                   tip_hide_timer;     /*!< 提示框的隐藏定时器 - 提示框要延迟消失 */

    MMIIM_KEYBOARD_SIZE_T   kb_size;            /*!< 输入法面板的大小，在nv中 */
    GUI_RECT_T              rect;               /*!< 输入法的控制区域，也即是面板的大小 */
    GUI_RECT_T              rect_disp_tips;               /*!< tips控制显示字符区域*/

    BOOLEAN                 is_symbol_panel;    /*!< 是否进入了符号输入状态 */
    VKEYBOARD_T             vkb;                /*!< 虚拟键盘 */
	VKB_MULTI_PAGE_T 		vkb_multi_page;			/*!某些语言存在多张键盘的情况*/
    void*                   data;               /*!< 输入法的核心数据 */
    MULTI_TAP_T multi_tap_data;    /*键盘连续按键数据*/   
    
    GUI_POINT_T             first_tp;           /*!< tp的第一点 */
    GUI_POINT_T             last_tp;            /*!< tp的最后一点 */

    MMI_HANDLE_T            more_setting_win_handle;/*!< 更多设置的窗口句柄 */
    BOOLEAN                 is_in_effect_enabled;   /*!< 显示特效是否打开 */
    BOOLEAN                 is_out_effect_enabled;  /*!< 显示特效是否打开 */

    SETTING_MENU_DATA_T const *setting_menu_data;   /*!< 设置菜单的数据 */
    uint32                  setting_menu_size;      /*!< 设置菜单的大小 */

    F_TermImm_T             f_term_imm;         /*!< 销毁引擎 */
    F_SetMethod_T           f_set_method;       /*!< 设置输入法 */
    F_IsInputting_T         f_is_inputting;     /*!< 获取输入状态 */
    F_DrawAll_T             f_draw_all;         /*!< 刷新ui */

    //输入法切换提示文字timer
    uint8 cue_hide_timer_id;
    
    //for setting im in istyle
    BOOLEAN 				settingim_halfposition; 	/*!< 切换输入法有半个显示不全时，上方还是下方显示不全 */
    E_IM_SET_SELECT_TYPE    setting_isst_e;     /*移动操作菜单，or直接点击操作菜单*/
	uint8 					settingim_timer_id;			/*!< 切换输入法timer id*/
    int16 					settingim_top_item;			/*!< 切换输入法滚动显示，当前显示的第一个item实际的index*/
    int16 					settingim_last_highlight;	/*!< 切换输入法最后一个高亮显示的item */
    int16 					settingim_item_count ;	/*!< setting_menu_index的数目*/
    int16 * 				settingim_menu_index ; 		/*!< save actual im language index of setting_menu_data*/
};


#ifdef __cplusplus
extern "C"
{
#endif


extern THEME_T const g_theme_thumb_default;
extern THEME_T const g_theme_qwert_default;
extern THEME_T const g_theme_symbol_default;
extern THEME_T const g_theme_sys_default;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
extern THEME_T const g_hw_fullscreen_theme_default;
#endif
extern GUI_RECT_T const g_thumb_panel_position[];
#if defined MMIIM_HW_FULLSCREEN_SUPPORT
extern GUI_RECT_T const g_hw_fullscreen_position[];
#endif
extern uint32 const g_thumb_panel_position_size;
extern GUI_RECT_T const g_thumb_panel_position_adjust_page_icon[];
extern uint32 const g_thumb_panel_position_adjust_page_icon_size;
extern uint8 const g_thumb_panel_expand_page_cnt;
extern uint8 const g_thumb_panel_expand_py_cnt;
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
extern wchar const * const g_thumb_cust_cn_symb;
#endif
extern wchar const * const g_thumb_cust_symb;

#if defined MMIIM_HW_FULLSCREEN_SUPPORT
#if defined IM_SIMP_CHINESE_SUPPORT || defined IM_TRAD_CHINESE_SUPPORT
extern wchar const * const g_hw_fullscreen_cust_cn_symb;
#endif
extern wchar const * const g_hw_fullscreen_cust_symb;
#endif

extern wchar const *g_thumb_key_disp[];
extern uint8 const g_thumb_key_disp_len[];
extern char const *g_thumb_data_lower[];
extern char const *g_thumb_data_upper[];
extern GUI_RECT_T const g_stroke_panel_position[];
extern uint32 const g_stroke_panel_position_size;
extern wchar const * const g_stroke_symb;
extern GUI_RECT_T const g_qwert_panel_position[];
extern uint32 const g_qwert_panel_position_size;
extern uint8 const g_qwert_panel_expand_page_cnt;
extern GUI_RECT_T const g_digit_panel_position[];
extern uint32 const g_digit_panel_position_size;
extern GUI_RECT_T const g_sys_panel_position[];
extern uint32 const g_sys_panel_position_size;

extern GUIIM_TYPE_T g_default_ch_type;
extern GUIIM_TYPE_T g_default_en_type;
extern GUIIM_INPUT_TYPE_T g_default_input_type;
//进入手写前，记录非手写输入方式，防止手写时，切换语言，导致输入方式s_default_input_type记录错误
extern GUIIM_INPUT_TYPE_T g_entry_hw_input_type;
#if defined MMIIM_TOUCH_SMART_CAPS
extern CAPS_SWITCH_TYPE_E g_caps_switch_type;
#endif
extern uint16 const g_kb_keymap[];
extern uint32 const g_kb_keymap_size;
extern int32 g_select_letter_index;
extern const wchar normal_mul_sym[TEXT_LEN_LIMIT];  
extern const wchar normal_9key_mul_sym[TEXT_LEN_LIMIT];
#if defined IM_ARABIC_SUPPORT
extern const wchar arabic_mul_sym[TEXT_LEN_LIMIT];
#endif

PUBLIC __inline uint16 RGB888TO565(uint32 rgb888)
{
    unsigned int r888 = (rgb888 & 0xff0000) >> 16;
    unsigned int g888 = (rgb888 & 0x00ff00) >> 8;
    unsigned int b888 = (rgb888 & 0x0000ff);
    unsigned int r565 = r888 >> 3;
    unsigned int g565 = g888 >> 2;
    unsigned int b565 = b888 >> 3;
    unsigned int rgb565 = (r565 << (5 + 6)) | (g565 << 5) | b565;

    return rgb565;
}


PUBLIC __inline uint32 RGB565TO888(uint16 rgb565)
{
    unsigned int r565 = (rgb565 >> 11) & 0x1f;
    unsigned int g565 = (rgb565 >> 5) & 0x3f;
    unsigned int b565 = rgb565 & 0x1f;
    unsigned int r888 = r565 << 3;
    unsigned int g888 = g565 << 2;
    unsigned int b888 = b565 << 3;
    unsigned int rgb888 = (r888 << 16) | (g888 << 8) | b888;

    return (rgb888 | 0xff000000); //set max alpha
}




PUBLIC __inline void ConvertRect(
    int16 width,
    int16 height,
    GUI_RECT_T const *relative,
    GUI_RECT_T *absolute,
    uint8 hor_space,
    uint8 ver_space)
{
    SCI_ASSERT(PNULL != relative); /*assert verified*/
    SCI_ASSERT(PNULL != absolute); /*assert verified*/

    absolute->left = (relative->left - 1 + hor_space / 2) * width / 1000;
    absolute->right = (relative->right - hor_space / 2)* width / 1000;
    absolute->top = (relative->top - 1 + ver_space *7 / 10) * height / 1000;   
    absolute->bottom = (relative->bottom - ver_space * 3 / 10)* height / 1000;   //底部少留些距离

    return;
}



#if defined MMIIM_HW_FULLSCREEN_SUPPORT
/*==============================================================================
Description: 相对面板坐标转换为绝对屏幕坐标
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC __inline void HwFsConvertRect(
    int16 width,
    int16 height,
    GUI_RECT_T const *relative,
    int16 offset_y,     //偏移量，像素点个数，不是index
    GUI_RECT_T *absolute,
    uint8 hor_space, 
    uint8 ver_space)
{
    if (PNULL == relative || PNULL == absolute)
    {
        SCI_TraceLow ("HwFsConvertRect");
        return;
    }

    absolute->left = (relative->left - 1 + hor_space / 2) * width / 1000;
    absolute->right = (relative->right - hor_space / 2)* width / 1000;
    absolute->top = (relative->top - 1 + ver_space * 7 / 10) * height / 1000 + offset_y;
    absolute->bottom = (relative->bottom - ver_space * 3 / 10) * height / 1000 + offset_y - 1;

    return;
}
#endif


PUBLIC __inline char UpperCase(char c)
{
    return (('a' <= c && c <= 'z') ? (c - 'a' + 'A') : c);
}


PUBLIC __inline char LowerCase(char c)
{
    return (('A' <= c && c <= 'Z') ? (c - 'A' + 'a') : c);
}


PUBLIC __inline void DcToDstStr(
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    GUISTR_BUF_T *des_str_buf
)
{
    SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != des_str_buf); /*assert verified*/

    des_str_buf->buffer = dc_data_ptr->lcd_buffer_ptr;
    des_str_buf->mem_width = dc_data_ptr->mem_width;
    des_str_buf->width = dc_data_ptr->width;
    des_str_buf->height = dc_data_ptr->height;
#ifdef MMIIM_LAYER_565
    des_str_buf->type = GUISTR_BUF_RGB565;
#else
    des_str_buf->type = GUISTR_BUF_ARGB888;
#endif
    return;
}

PUBLIC BOOLEAN HasConfigSelectInputType(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_TYPE_T type);
PUBLIC BOOLEAN HasConfigSelectInput(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T guiim_lang, GUIIM_INPUT_TYPE_T input);


PUBLIC KEY_INFO_T *FindKey(
    IM_DATA_T *data_ptr,
    GUI_POINT_T *point_ptr
);
PUBLIC KEY_INFO_T *FindKeyWithCode(
    IM_DATA_T *data_ptr,
    uint16 code
);
PUBLIC BOOLEAN SlidingPanel(IM_DATA_T *data_ptr, int32 dir);
/* settings */
PUBLIC MMI_HANDLE_T CreateSettingWin(IM_DATA_T *data_ptr);
PUBLIC MMI_RESULT_E HandleSettingWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E msg_id,
    DPARAM param
);
PUBLIC BOOLEAN SettingMethod(
    IM_DATA_T *data_ptr,
    GUIIM_LANGUAGE_T lang,
    GUIIM_TYPE_T type,
    GUIIM_INPUT_TYPE_T input
);
PUBLIC void ResetMethod(IM_DATA_T *data_ptr);
PUBLIC GUIIM_METHOD_T GetMethodBySwitchLang(IM_DATA_T *data_ptr, GUIIM_LANGUAGE_T lang);

/* ui */
/*==============================================================================
Description: Because the speed is too slow when use etrump font. 
    			IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key paint status    			
Global resource dependence: NONE
Author: yaoguang.chen
Note:   
==============================================================================*/
PUBLIC void MMIIM_SetVKeyPaintStatus (KEY_INFO_T *key_ptr, BOOLEAN is_paint);

/*==============================================================================
Description: Because the speed is too slow when use etrump font. 
    			IME can't use so many draw keys when press down or up.
		So if we need to draw one key, we need to set the key paint status  
		All keys need to paint, but when is hide status, it won't paint.
Global resource dependence: NONE
Author: yaoguang.chen
Note:   
==============================================================================*/
PUBLIC void MMIIM_SetVkbPaintStatus (IM_DATA_T *data_ptr, BOOLEAN is_paint);

PUBLIC BOOLEAN DrawVkb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN MMIIM_DrawAll(IM_DATA_T *data_ptr);
PUBLIC int32 GetThemeIndex(KEY_INFO_T *key_ptr);
PUBLIC void DrawKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC BOOLEAN DrawLangSwitchKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC void DrawKeyRaw(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
);

PUBLIC void DrawKeySelectLetter(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,         //tip_rect
    KEY_INFO_T *key_ptr
);

/* tip */
/*==============================================================================
Description: 绘制按键 - 已知theme
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void DrawTipKey(
    IM_DATA_T const *data_ptr,
    GUILCD_DRAW_DC_T const *dc_data_ptr,
    KEY_THEME_T const *theme_ptr,
    GUI_RECT_T const *rect_ptr,
    KEY_INFO_T *key_ptr
);
PUBLIC void ShowTip(IM_DATA_T *data_ptr,  KEY_INFO_T *key_ptr);
PUBLIC void HideTip(IM_DATA_T *data_ptr);
PUBLIC void HideTipCB(uint8 timer_id, uint32 param);

/* 显示层的创建、释放 */
PUBLIC BOOLEAN CreateUILayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseUILayer(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN CreateTipLayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseTipLayer(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN CreateEditLayer(IM_DATA_T *data_ptr);
PUBLIC void ReleaseEditLayer(IM_DATA_T *data_ptr);

/* 虚拟键盘函数 */
/* 设置 */
PUBLIC void InitVkbCaps(IM_DATA_T *data_ptr);
PUBLIC void SetVkbNextCaps(IM_DATA_T *data_ptr);
PUBLIC void SetVkbCaps(IM_DATA_T *data_ptr, CAPITTAL_MODE_T cap);
PUBLIC void SetHideKey(IM_DATA_T * data_ptr, uint8 cand_cnt);
PUBLIC void SwitchExpandKey(IM_DATA_T *data_ptr);
PUBLIC void ReleaseVkb(VKEYBOARD_T *vkb_ptr);
PUBLIC void SetPanel(IM_DATA_T *data_ptr);
PUBLIC uint16 GetPanelKeyRowNum (IM_DATA_T *data_ptr);

/* 操作 */
PUBLIC BOOLEAN VkbDown(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbMove(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkbUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpDeal(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void HidePanel(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN VkeyUpHide(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyLongSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyLongDel(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeySetDown (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN VkeyUpLangSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN PanelSlideCueCloseByTimer (IM_DATA_T *data_ptr);
PUBLIC void PanelSlideCueCloseCB (uint8 timer_id, uint32 param);
PUBLIC BOOLEAN PanelSlideCueClose (IM_DATA_T *data_ptr, BOOLEAN immediately_paint);

/* 打包提交 */
PUBLIC void CommitChar(IM_DATA_T *data_ptr, wchar c);
PUBLIC void CommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len);
PUBLIC BOOLEAN CapsCommitChar(IM_DATA_T *data_ptr, wchar c);
PUBLIC BOOLEAN CapsCommitStr(IM_DATA_T *data_ptr, wchar *str, uint32 len);
#if defined MMIIM_TOUCH_SMART_CAPS
PUBLIC void SetCapsSwitchType (CAPS_SWITCH_TYPE_E switch_type);
PUBLIC CAPS_SWITCH_TYPE_E GetCapsSwitchType ();
PUBLIC BOOLEAN DialogSeperateChar (wchar c);
PUBLIC BOOLEAN WordSeperateChar (wchar c);    
#endif



PUBLIC void SetMethodSelectMenuDefaultIndex (int32 index);
PUBLIC int32 GetMethodSelectMenuDefaultIndex ( );
PUBLIC void SetLetterSelectIndex(int32 index);
PUBLIC int32 GetLetterSelectIndex();
PUBLIC int32 MMIIM_MethodSettingMenuDown (IM_DATA_T *    data_ptr, GUI_POINT_T point, BOOLEAN repaint);
PUBLIC int32 RefreshTipLetterSelectIndex(IM_DATA_T *data_ptr, GUI_POINT_T *point_ptr, BOOLEAN repaint);
PUBLIC BOOLEAN ChooseLetter(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC const wchar *  GetMultiSymCharPtr(GUIIM_LANGUAGE_T lang);
PUBLIC BOOLEAN MultiCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

PUBLIC QWERT_KEYBOARD_LAYOUT_TYPE_E QwertKbLayoutType (GUIIM_METHOD_T method, uint8 cur_page);
//PUBLIC THEME_T QwertKbGetTheme (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);  //暂时没有区分键盘主题
PUBLIC GUI_RECT_T const*QwertKbGetPosition_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint32 QwertKbGetKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint8 QwertKbGetCharKeyCnt (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);
PUBLIC uint16 const*QwertKbGetKeyMap_ptr (QWERT_KEYBOARD_LAYOUT_TYPE_E layout);

PUBLIC BOOLEAN MMIIM_IsLimitSwitchLang (GUIIM_METHOD_PARAM_T *method_param_ptr);
PUBLIC int32 MMIIM_ConfigSettingMenu (IM_DATA_T *data_ptr);
PUBLIC int32 MMIIM_CreateSettingMenuLayer (IM_DATA_T *data_ptr, GUI_POINT_T pos);
PUBLIC void MMIIM_DrawSettingMenu (IM_DATA_T *data_ptr, int16 highlight_index);
PUBLIC int32 MMIIM_OpenMethodSettingMenu (IM_DATA_T *    data_ptr, GUI_POINT_T pos);
PUBLIC int32 MMIIM_CloseMethodSettingMenu (IM_DATA_T *    data_ptr);
PUBLIC int32 MMIIM_MethodSettingMenuRefresh (IM_DATA_T *    data_ptr, GUI_POINT_T point, BOOLEAN repaint);
PUBLIC MMI_RESULT_E MMIIM_MethodSettingMenuUp(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN MMIIM_VkeyUpSet(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC GUIIM_CAPITAL_MODE_T MMIIM_GetIMCaps(CAPITTAL_MODE_T cap);

PUBLIC int32 MMIIM_TOUCH_KbUnifiedCharKeysFontSize (IM_DATA_T *data_ptr);
/*==============================================================================
Description: 获取面板统一字符大小(data, 按键类型, 按键附加属性)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC GUI_FONT_T MMIIM_TOUCH_ComputKbUnifiedFontSize (IM_DATA_T *data_ptr, KEY_STYLE_T key_style);

/*==============================================================================
Description: 设置统一字符大小 (data, 按键类型, 按键附加属性)
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SetKeyFontSize (
    IM_DATA_T *data_ptr, 
    KEY_STYLE_T key_style, 
    GUI_FONT_T font_size
);


PUBLIC int32 MMIIM_TOUCH_InitVkbMultiPageParam (
    GUIIM_METHOD_T method, 
    VKB_MULTI_PAGE_T *vkb_multi_page_ptr
);

/*==============================================================================
Description: 设置虚拟键盘的当前页，某些输入法存在多张键盘的情况发生
	此处约定按键字符个数相等
	后期再添加字符个数不等的键盘，如繁体中文的键盘字符个数就不同
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ResetQwertVkbChars (
    IM_DATA_T *data_ptr, 
    uint8 set_page
);

/*==============================================================================
Description: 因当前程序无法满足显示字符设置的情况，导致需要将按键码字符串和
			按键显示字符串分离，此处为了保持前期程序一致性，将按键码字符串
			同步到显示字符串，如果某些地方需要修改显示字符串，在其他函数中处理
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeDisplayText (IM_DATA_T *data_ptr, KEY_STYLE_T key_style);

/*==============================================================================
Description: 因当前程序无法满足显示字符设置的情况，导致需要将按键码字符串和
			按键显示字符串分离，此处为了保持前期程序一致性，将按键码字符串
			同步到显示字符串，如果某些地方需要修改显示字符串，在其他函数中处理
Global resource dependence: NONE
Author: yaoguang.chen
Note: 1.后期程序维护时，可能将display_text字段空间变小，所以到时候的同步长度
		就会仅仅保留display_text能够处理的长度，防止内存越界
	  2.不能随便使用这个函数，否则会将所有的字符串同步了，而某些
	    不需要同步的会出问题	
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_SynchronizeAllDisplayText (IM_DATA_T *data_ptr);

/*==============================================================================
Description: 某些语言，显示字符串和按键码不同，需要自定义显示的字符串
Global resource dependence: NONE
Author: yaoguang.chen
Note: 
==============================================================================*/
PUBLIC int32 MMIIM_TOUCH_ThumbKbRedefinePrintText (IM_DATA_T *data_ptr);

#ifdef __cplusplus
}
#endif





#endif //defined (MMI_IM_PDA_SUPPORT) 


#endif /*_MMIIM_TOUCH_COMMON_H_*/


