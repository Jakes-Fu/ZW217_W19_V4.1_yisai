/*****************************************************************************
** File Name:      ctrlrichtext.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLRICHTEXT_H_
#define _CTRLRICHTEXT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlrichtext_export.h"

/* general header file includes */
#include "sci_types.h"
#include "guicommon.h"

#include "mmitheme_clipbrd.h"
#include "guiurl.h"

#include "guiprgbox.h"
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
// richtext type info
#define CTRL_RICHTEXT_TYPE     (RICHTEXT_GetType())

#define RICHTEXT_TAG_STRING_MAX_NUM (511)
//每行最多能显示的焦点元素数目，用于局部变量的数组大小
#define RICHTEXT_TAG_STRING_MAX_PER_LINE (10)
#define RICHTEXT_TEXT_MAX_LINE (GUIRICHTEXT_TEXT_MAX_LEN)
#define RICHTEXT_BORAD_WIDTH (DP2PX_VALUE(3)) //这个值是系统圆角框的宽度决定的
#define RICHTEXT_DEFAULT_ITEM_BORDER (RICHTEXT_BORAD_WIDTH)
#define RICHTEXT_SLIDE_FLING_TICK (100)
#define RICHTEXT_SLIDE_RECOVER_TICK (5)
#define RICHTEXT_SLIDE_RECOVER_MIN (20)

#ifdef GUIF_CLIPBOARD
#define RICHTEXT_DRAG_SELECT_TICK (120)
#endif

/*! @def GUIRICHTEXT_LINE_LEN_MAX
@brief 单行显示文字的最大个数
*/
#define GUIRICHTEXT_LINE_LEN_MAX (128)

/*! @def GetContHeight
@brief 获取控件显示区域的高度
*/
#define GetContHeight(ptr) ((ptr)->cont_rect.bottom - (ptr)->cont_rect.top + 1)
/*! @def GetContWidth
@brief 获取控件显示区域的宽度
*/
#define GetContWidth(ptr) ((ptr)->cont_rect.right - (ptr)->cont_rect.left + 1)
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// 当前处于哪个拖动状态
typedef enum
{
    GUIRICHTEXT_DRAGSTATE_NORMAL,       // 普通的光标状态
    GUIRICHTEXT_DRAGSTATE_SELECT,       // 选择文本状态
    GUIRICHTEXT_DRAGSTATE_MAX
} GUIRICHTEXT_DRAGSTATE_E;

// 处于哪个滑块上
typedef enum
{
    GUIRICHTEXT_SLIDER_NONE,            // 没有滑块，此时应该处于GUIRICHTEXT_DRAGSTATE_NORMAL状态
    GUIRICHTEXT_SLIDER_LEFT_SELECTOR,   // 左选择块，此时应该处于GUIRICHTEXT_DRAGSTATE_SELECT
    GUIRICHTEXT_SLIDER_RIGHT_SELECTOR,  // 右选择块，此时应该处于GUIRICHTEXT_DRAGSTATE_SELECT
} GUIRICHTEXT_SLIDER_E;

//1 extern item data,额外的数据定义，用于描述图片和文字的一些详细信息
//1.1 image
typedef struct
{
    void   *anim_ptr;
    uint16 orig_width;
    uint16 orig_height;
    uint16 disp_width;
    uint16 disp_height;
    GUI_BIG_RECT_T disp_rect;
}RICHTEXT_EXT_DATA_IMAGE_T;

//1.2 text
typedef struct
{
    uint32 line_num;
    uint16 *line_start_pos_ptr;
    uint16 line_height;

    GUI_BIG_RECT_T disp_rect;

#ifdef GUIF_CLIPBOARD
    uint16 select_len;
    uint16 select_pos;
    uint16 select_line_begin;
    uint16 select_pos_begin;
    uint16 select_line_end;
    uint16 select_pos_end;
#endif
}RICHTEXT_EXT_DATA_TEXT_T;

//1.3 焦点元素的显示状态
typedef enum
{
    RICHTEXT_FOCUS_UNKNOW,
    RICHTEXT_FOCUS_TOP_OUT,
    RICHTEXT_FOCUS_TOP_OVER,
    RICHTEXT_FOCUS_IN,
    RICHTEXT_FOCUS_OVER,
    RICHTEXT_FOCUS_BOTTOM_OVER,
    RICHTEXT_FOCUS_BOTTOM_OUT,

    RICHTEXT_FOCUS_INVALID,
}RICHTEXT_FOCUS_DISP_E;
    
//2 ITEM的扩展定义
//2.1 扩展字段定义
//2.1.1 data
typedef struct
{
    GUIRICHTEXT_ITEM_T orig_data;

    RICHTEXT_EXT_DATA_IMAGE_T img_ext_data;
    RICHTEXT_EXT_DATA_TEXT_T txt_ext_data;
}RICHTEXT_ITEM_DATA_T;

//2.1.2 focus type
typedef struct
{
    GUIRICHTEXT_TAG_TYPE_E tag_type;
    uint16 abs_begin;
    uint16 abs_end;
    uint16 line_begin;
    uint16 pos_begin;
    uint16 line_end;
    uint16 pos_end;
}RICHTEXT_FOCUS_ELEM_T;


typedef struct richtext_focus_element_node_tag
{
    GUIRICHTEXT_TAG_TYPE_E tag_type;
    uint16 abs_begin;
    uint16 abs_end;

    struct richtext_focus_element_node_tag * next_ptr;
}RICHTEXT_FOCUS_ELEMENT_NODE_T;

//2.2 all info
typedef struct richtext_item_def_tag
{
    uint16 index;
    
    uint16 width;
    int32 height;

    GUI_BIG_RECT_T virtual_rect;

    //临时链表头指针
    RICHTEXT_FOCUS_ELEMENT_NODE_T *focus_header_ptr;
    //临时链表尾指针
    RICHTEXT_FOCUS_ELEMENT_NODE_T *focus_tailer_ptr;
    //存储焦点元素的数组指针，改控件是动态申请的
    RICHTEXT_FOCUS_ELEM_T *focus_elem;
    uint16 focus_count;
    uint16 focus_index;

    RICHTEXT_ITEM_DATA_T data;

    struct richtext_item_def_tag *prev_ptr;
    struct richtext_item_def_tag *next_ptr;
}RICHTEXT_ITEM_T;


#ifdef GUIF_CLIPBOARD
enum
{
    RICHTEXT_STATE_SELECT_NONE,
    RICHTEXT_STATE_SELECTING,
    RICHTEXT_STATE_SELECTED,
};
typedef uint8 RICHTEXT_SELECT_STATE_T;
#endif

enum
{
    RICHTEXT_TPMODE_NONE,
    RICHTEXT_TPMODE_DRAG,
    RICHTEXT_TPMODE_SLIDE,
};
typedef uint8 RICHTEXT_TPMODE_T;

typedef struct
{
    int16   item_index;     // item索引
    int16   line_index;     // 所在item的行索引
    int16   pos_index;      // 这行里的位置索引
} RICHTEXT_SEL_INFO_T;

//settings
typedef enum
{
    //display - font
    RICHTEXT_SETTINGS_FONT,
    RICHTEXT_SETTINGS_FONT_COLOR,
    //display - space
    RICHTEXT_SETTINGS_RECT,
    RICHTEXT_SETTINGS_SPACE_BORDER,
    RICHTEXT_SETTINGS_SPACE_ITEM_BORDER,
    RICHTEXT_SETTINGS_SPACE_ITEM,
    RICHTEXT_SETTINGS_SPACE_CHAR,
    RICHTEXT_SETTINGS_SPACE_LINE,
    RICHTEXT_SETTINGS_SPACE_IMG_TXT,
    RICHTEXT_SETTINGS_TXT_MARGIN,
    //display - focus
    RICHTEXT_SETTINGS_FOCUS_MODE,
    RICHTEXT_SETTINGS_FOCUS_LINE_COLOR,
    RICHTEXT_SETTINGS_FOCUS_BG_COLOR,
    //display - bg
    RICHTEXT_SETTINGS_BG_COLOR,
    RICHTEXT_SETTINGS_BG_IMG,
    //display - frame
    RICHTEXT_SETTINGS_FRAME_TYPE,
    RICHTEXT_SETTINGS_FRAME_COLOR,
    RICHTEXT_SETTINGS_FRAME_FOCUS_COLOR,
    //display - anim
    RICHTEXT_SETTINGS_ANIM_PARSING_ICON,
    RICHTEXT_SETTINGS_ANIM_ERROR_ICON,
    //control
    RICHTEXT_SETTINGS_FOCUS_ITEM,
    RICHTEXT_SETTINGS_SCROLL_BAR,
    RICHTEXT_SETTINGS_IMG_SYN_DECODE,
}RICHTEXT_SETTINGS_E;

/*! @typedef RICHTEXT_IMG_DISPLAY_E
@brief 图片的显示状态，当图片异步显示时，需要记录显示状态
*/
typedef enum
{
    RICHTEXT_IMG_DISPLAY_FAIL,
    RICHTEXT_IMG_DISPLAY,
    RICHTEXT_IMG_DISPLAY_APPENDING
} RICHTEXT_IMG_DISPLAY_E;

// richtext init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    MMI_IMAGE_ID_T bg_image_id; /*!< 控件的背景图片 */
} CTRLRICHTEXT_INIT_PARAM_T;

//richtext control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

    MMI_WIN_ID_T win_id;
    
    BOOLEAN is_focus;

    GUI_RECT_T cont_rect;
    GUI_RECT_T scroll_rect;
    
    BOOLEAN need_scroll;
    uint16 scrollbar_width;
    uint16 scroll_step;
    IGUICTRL_T *prgbox_ptr;

    //item链表数据
    RICHTEXT_ITEM_T *first_item_ptr;

    //显示数据
    BOOLEAN need_cal_ext_data;
    BOOLEAN need_cal_rect;
    int32 disp_height;
    int32 disp_top;
    GUI_ALIGN_E disp_align;

    //背景
    //只支持res图片，因为不能异步显示
    MMI_IMAGE_ID_T bg_img_id;
    GUI_COLOR_T bg_color;
    
    //焦点
    GUIRICHTEXT_FOCUS_SHIFT_E focus_mode;
    RICHTEXT_ITEM_T *focus_item_ptr;

    //图片是否同步解码
    BOOLEAN img_is_syn_decode;
    //显示图片的中间变量
    RICHTEXT_ITEM_T *img_appending_item_ptr;
    //设置正在显示图片的时候显示的图标
    MMI_IMAGE_ID_T anim_parsing_icon_id;
    //设置显示图片错误的时候显示的图标
    MMI_IMAGE_ID_T anim_error_icon_id;

    //显示相关的参数
    GUIRICHTEXT_FRAME_TYPE_E border_type;
    GUI_COLOR_T border_color;
    GUI_COLOR_T border_focus_color;

    GUI_FONT_T font;
    GUI_COLOR_T font_color;

    GUI_COLOR_T focusline_color;
    GUI_COLOR_T focus_bg_color;

    uint8 border_space; //整个richtext的四边空白距离，如果有边框的话，该值至少为1
    uint8 item_border_space; //单个item的边框所占的距离，如果有设置item边框的话，则需要相应的设置，是text&image，不是整个item
    uint8 char_space; //text文字间的距离
    uint8 line_space; //text行间距
    uint8 item_space; //两个item之间的距离
    uint8 img_text_space; //图片和文本之间的距离
    uint8 txt_margin; //文字的边距，特别在设置item边框后，留白可能好看一些

    //电话号码临时存储区
    wchar edit_phonenumber[GUIURL_PHONE_NUMBER_LEN_MAX+1];

    GUI_POINT_T tp_start_point; //触笔的绝对位置
    GUI_POINT_T tp_stop_point;

    RICHTEXT_TPMODE_T tp_mode;
    uint8 slide_timer;
    float slide_v;

    BOOLEAN is_info_protected;

    //select & copy
#ifdef GUIF_CLIPBOARD
    BOOLEAN is_clipboard_support; //是否支持剪切板
    BOOLEAN is_select_data_dirty;  //所有的数据是否需要重构-比如tp move，ctrl重建
    BOOLEAN is_line_data_dirty; //行信息是否需要重构-比如旋转屏幕
    uint16 select_len; //选定的全部长度之和
    uint16 select_item_cnt; //选定的item个数
    RICHTEXT_SELECT_STATE_T select_state;
    uint8 drag_select_timer;
    //触笔的虚拟位置
    int32 v_start_x;
    int32 v_start_y;
    int32 v_stop_x;
    int32 v_stop_y;
    MMI_HANDLE_T clipbrd_win_handle; //clipbrd win handle

    uint32                      slider[MMITHEME_SLIDER_TYPE_MAX];   // 滑块句柄
    GUIRICHTEXT_DRAGSTATE_E     drag_state;                         // 当前滑动状态
    GUIRICHTEXT_SLIDER_E        tpdown_slider;                      // TPDOWN时的，点击的区域
    uint8                       slider_dis_timer_id;                // 滑块消失定时器
    MMITHEME_DRAGMODE_E         drag_mode;                          // 滑块操作模式
    uint32                      slider_display_period;              // 滑块显示时间

    RICHTEXT_SEL_INFO_T         start_info;                         // 高亮开始信息
    RICHTEXT_SEL_INFO_T         stop_info;                          // 高亮结束信息
#endif
} CTRLRICHTEXT_OBJ_T;

// richtext class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    // self vtbl ...
} CTRLRICHTEXT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get richtext type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T RICHTEXT_GetType(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
