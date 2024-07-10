/*****************************************************************************
** File Name:      ctrlbaseflex.h                                               *
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

#ifndef _CTRLBASEFLEX_H_
#define _CTRLBASEFLEX_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbaseedit.h"
#include "ctrlbaseflex_export.h"

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
// baseflex type info
#define CTRL_BASEFLEX_TYPE     (BASEFLEX_GetType())

#define GUIEDIT_NUM_INFO_MAX_LEN            20      //the number info,include remaining characters and current index

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

//edit cursor move direction
typedef enum
{
    GUIEDIT_UPDATE_NONE,    //none
    GUIEDIT_UPDATE_PART,    //part 
    GUIEDIT_UPDATE_ALL,     //all
    GUIEDIT_UPDATE_MAX
} GUIEDIT_UPDATE_MODE_E;

//edit cursor move direction
typedef enum
{
    GUIEDIT_CURSOR_MOVE_NULL,
    GUIEDIT_CURSOR_MOVE_UP,
    GUIEDIT_CURSOR_MOVE_DOWN,
    GUIEDIT_CURSOR_MOVE_LEFT,
    GUIEDIT_CURSOR_MOVE_RIGHT,
    GUIEDIT_CURSOR_MOVE_MAX
} GUIEDIT_CURSOR_MOVE_E;

/*! @enum GUIEDIT_TP_REGION_E
@brief 触笔点击区域
*/
typedef enum
{
    GUIEDIT_TP_REGION_NONE,     /*!<保留位*/
    GUIEDIT_TP_REGION_HIGHLIGHT,/*!<高亮区域*/
    GUIEDIT_TP_REGION_MAX       /*!<保留位*/
} GUIEDIT_TP_REGION_E;

/*! @enum GUIEDIT_TP_STATE_E
@brief 触笔点击状态
*/
//tp state
typedef enum
{
    GUIEDIT_TP_STATE_NONE,  /*!<保留位*/
    GUIEDIT_TP_STATE_DRAG,  /*!托选状态*/
    GUIEDIT_TP_STATE_MAX    /*!<保留位*/
} GUIEDIT_TP_STATE_E;

// 当前处于哪个拖动状态
typedef enum
{
    GUIEDIT_DRAGSTATE_NORMAL,       // 普通的光标状态
    GUIEDIT_DRAGSTATE_NORMAL_RESET, // 普通的光标状态，同时去掉高亮
    GUIEDIT_DRAGSTATE_DRAG,         // 拖动定位光标状态
    GUIEDIT_DRAGSTATE_SELECT,       // 选择文本状态
    GUIEDIT_DRAGSTATE_MAX
} GUIEDIT_DRAGSTATE_E;

// 处于哪个滑块上
typedef enum
{
    GUIEDIT_SLIDER_NONE,            // 没有滑块，此时应该处于GUIEDIT_DRAGSTATE_NORMAL状态
    GUIEDIT_SLIDER_BLOCK,           // 拖动滑块，此时应该处于GUIEDIT_DRAGSTATE_DRAG
    GUIEDIT_SLIDER_LEFT_SELECTOR,   // 左选择块，此时应该处于GUIEDIT_DRAGSTATE_SELECT
    GUIEDIT_SLIDER_RIGHT_SELECTOR,  // 右选择块，此时应该处于GUIEDIT_DRAGSTATE_SELECT
} GUIEDIT_SLIDER_E;

//edit add actual string
typedef struct
{
    BOOLEAN     is_add_lf;          //is add LF
    wchar       *str_ptr;           //add string,dyna alloc
    uint16      str_len;            //add string len
    uint16      len_before_cursor;  //add string len before cursor
} GUIEDIT_ADD_ACTUAL_STR_T;

//edit update
typedef struct
{
    uint16                  start_line_index;   //update the first line index
    uint16                  end_line_index;     //update the last line index
    GUIEDIT_UPDATE_MODE_E   update_mode;        //update mode
} GUIEDIT_UPDATE_T;

// baseflex init param
typedef struct
{
    CTRLBASEEDIT_INIT_PARAM_T       baseedit_ctrl_param;

    // self inif param ...
} CTRLBASEFLEX_INIT_PARAM_T;

//baseflex control
typedef struct
{
    CTRLBASEEDIT_OBJ_T          baseedit_ctrl;

    BOOLEAN                     is_disp_num_info;   /*!<是否显示剩余字符或者列表信息*/
    BOOLEAN                     is_display_prg;     /*!<是否显示滚动条*/

    BOOLEAN                     is_tp_rect_change;  /*!<是否触笔down消息激活控件*/

    BOOLEAN                     is_disable_cutcopy; // 是否禁止剪切、复制
    BOOLEAN                     is_byword;          // 是否断词
    BOOLEAN                     is_del_after_full;  /*!<当输入满,再输入字符,是否删除第一个字符*/

    BOOLEAN                     is_tp_scroll_up;    /*!<是否触笔滚动字符串，向上的位置*/
    uint8                       tp_scroll_timer_id; /*!<触笔定时器*/
    GUI_POINT_T                 tp_move_point;      /*!<触笔移动点坐标*/

    GUIEDIT_TP_STATE_E          tp_state;           /*!<触笔点击状态*/
    GUIEDIT_TP_REGION_E         tp_region;          /*!<触笔点击区域*/

    wchar                       *default_str_ptr;   // 默认文本内容
    uint16                      default_str_len;    // 默认文本长度

    wchar                       *disable_char_ptr;  /*!<不允许输入的字符表*/
    uint16                      disable_char_len;   /*!<不允许输入的字符表长度*/

	wchar                       *add_enable_char_ptr;	/*!<除允许输入的字符之外,额外允许输入的字符表*/
    uint16                      add_enable_char_len;	/*!<除允许输入的字符之外,额外允许输入的字符表*/

    GUI_POINT_T                 prev_point;         //previous point

    BOOLEAN                     is_tp_moved;        /*!<触笔是否移动*/
    BOOLEAN                     is_set_skb;

#ifdef GUIF_CLIPBOARD
    BOOLEAN                     is_clipbrd_support; // 是否支持剪贴板
    BOOLEAN                     is_show_clipbrd;    // 是否显示了剪贴板
    MMI_HANDLE_T                clipbrd_win_handle; /*!<剪贴板窗口handle*/
    uint32                      slider[MMITHEME_SLIDER_TYPE_MAX];   // 滑块句柄
    GUIEDIT_DRAGSTATE_E         drag_state;                         // 当前滑动状态
    GUIEDIT_SLIDER_E            tpdown_slider;                      // TPDOWN时的，点击的区域
    uint8                       slider_dis_timer_id;                // 滑块消失定时器
#endif

    BOOLEAN                     is_tp_down;         //is tp down（是否按下，如果是，则在slide timer里不做滑动）
    uint8                       slide_timer_id;     //slide timer id
    float                       fling_velocity;     //fling velocity
    GUI_POINT_T                 start_point;        //slide start point
    MMK_TP_SLIDE_E              slide_state;        //slide state
    MMK_TP_SLIDE_E              tpdown_slide_state; //tp down slide state

    GUIEDIT_WIN_INTERCEPT_CTRL_MSG  WinInterceptCtrlMsgFunc;/*!<窗口是否截获控件消息,目前只能截获Cancel消息*/

    MMITHEME_BASEFLEX_T         flex_theme;
} CTRLBASEFLEX_OBJ_T;

// baseflex class for public or protected method
typedef struct
{
    CTRLBASEEDIT_VTBL_T            baseedit_ctrl_vtbl;

    /**************************************************************************/
    //  Description : 当添加文本后的通知回调
    /**************************************************************************/
    void (*OnTextAdded)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
        GUIEDIT_ADD_STR_T       *add_str_ptr
        );

    /**************************************************************************/
    //  Description : 当删除文本后的通知回调
    /**************************************************************************/
    void (*OnTextDeleted)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : 最大长度改变后的通知回调
    /**************************************************************************/
    void (*OnMaxLenChanged)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : 检测字符串是否合法
    /**************************************************************************/
    BOOLEAN (*IsStringValid)(                               // [ret] TRUE if char is valid
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
        wchar                   *str_ptr,
        uint16                  str_len
        );

    /**************************************************************************/
    //  Description : 显示剩余字数
    /**************************************************************************/
    void (*DisplayLeftNum)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : 获取剩余字数信息
    /**************************************************************************/
    uint16 (*GetLeftNumInfo)(                           // [ret] the left char num
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
        uint16                  str_max_len,            // [in] the max size of char
        wchar                   *str_ptr,               // [out] left char num string
        uint16                  *str_len_ptr            // [out] length of string
        );

    /**************************************************************************/
    //  Description : 检查被删字符，此方法在删除时所用
    /**************************************************************************/
    void (*CheckDelStr)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : 检查添加的字符，此方法在输入字符时所用
    /**************************************************************************/
    BOOLEAN (*CheckCommitStr)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
        uint16                      *delete_len_ptr         //out:
        );

    /**************************************************************************/
    //  Description : 在添加字符前做检查
    /**************************************************************************/
    void (*BeforeAddString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
        );

    /**************************************************************************/
    //  Description : 在添加字符后做检查
    /**************************************************************************/
    void (*AfterAddString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
        );

    /**************************************************************************/
    //  Description : 在删除字符前做检查
    /**************************************************************************/
    void (*BeforeDelString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
        );

    /**************************************************************************/
    //  Description : 文本解析
    /**************************************************************************/
    BOOLEAN (*ParseText)(
        CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
        uint16              start_line_index        //in:
        );

} CTRLBASEFLEX_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get baseflex type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEFLEX_GetType(void);

/*****************************************************************************/
//  Description : get baseflex pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASEFLEX_OBJ_T* BASEFLEX_GetPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : Re-parse edit text
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_ReparseEditText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : edit add string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_AddString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    GUIEDIT_UPDATE_T    *update_ptr,        //in/out:
    GUIEDIT_ADD_STR_T   *add_str_ptr,       //in:
    GUIEDIT_LIST_ITEM_T *list_item_ptr,
    uint16              list_item_count
);

/*****************************************************************************/
//  Description : edit delete string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_DelString (
    CTRLBASEFLEX_OBJ_T     *baseflex_ctrl_ptr, //in:
    GUIEDIT_UPDATE_T       *update_ptr     //in/out:
);

/*******************************************************************************************/
//  Description : whether support clipbrd
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN BASEFLEX_IsSupportClipbrd (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : display clip board menu
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayClipboardMenu (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUI_POINT_T             *point_ptr      //in:
);

/*****************************************************************************/
//  Description : close clip board menu
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_CloseClipboardMenu (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : handle edit copy msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditCopyMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit cut msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditCutMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit paste msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditPasetMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit select all msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditSelectAllMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 处理选择消息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleSelectClipbrdMsg (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : destroy slider
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DestroyEditSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : 改变滑块状态
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ChangeDragState (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_DRAGSTATE_E     drag_state
);

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ShowSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 开始滑块拖动
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleBeginSlider (
    CTRLBASEFLEX_OBJ_T *baseflex_ctrl_ptr,
    DPARAM             param
);

/*****************************************************************************/
//  Description : 拖动滑块
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleMoveSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : 滑块拖动结束
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEndSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : stop slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : handle slider timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleSliderTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : get edit reverse string
//  Global resource dependence :
//  Author: Jassmine
//  Note: 必须和FreeEditReverseString配对使用,负责内存泄露
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_GetReverseString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    wchar               **str_pptr,         //in/out:
    uint16              *str_len_ptr        //in/out:
);

/*****************************************************************************/
//  Description : free edit reverse string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_FreeReverseString (
    wchar     **str_pptr
);

/*****************************************************************************/
//  Description : check edit string is valid
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckEditString (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    wchar               *str_ptr,           //in:
    uint16              *str_len_ptr        //in/out:
);

/*****************************************************************************/
//  Description : check text string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckTextStr (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr, //in:
    wchar               *str_ptr,       //in:
    uint16              str_len         //in:
);

/*****************************************************************************/
//  Description : is digital char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_IsDigitalChar (
    wchar   char_code
);

/*****************************************************************************/
//  Description : check string is digital string
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CheckDigitalStr (
    wchar     *str_ptr,
    uint16    str_len
);

/*****************************************************************************/
//  Description : 截掉了非首位的+号以及后面的字符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEFLEX_InterceptPhoneNum (//截取后的长度
    wchar    *str_ptr,
    uint16   str_len
);

/*****************************************************************************/
//  Description : get text display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 BASEFLEX_GetDispMaxTop(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : get text relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 BASEFLEX_GetRelativeHeight(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : start cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StartCursorTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : stop cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopCursorTimer (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle cursor timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleCursorTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : display edit cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayCursor (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : reset don't display cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ResetCursor (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : get new cursor position for left right msg
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEFLEX_SetCursorPosForLeftRight (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    BOOLEAN                 is_left,
    BOOLEAN                 *is_cursor_set_ptr
);

/*****************************************************************************/
//  Description : adjust cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note: move cursor,delete char for multi language
/*****************************************************************************/
PUBLIC void BASEFLEX_AdjustCursorPos (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,	//in:
    BOOLEAN			        is_left,		    //in:
    uint16			        *cursor_pos_ptr	    //out:
);

/*****************************************************************************/
//  Description : move cursor and update cursor,string etc
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_MoveCursor (//is change
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,
    GUIEDIT_CURSOR_MOVE_E       move_direction
);

/*****************************************************************************/
//  Description : check cursor position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_CheckCursor (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : set edit top line index to make cursor visible
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_MakeCursorVisible (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : display information bar,include im icon and remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_DisplayInfoBar (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : update text display for moving cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_UpdateForMoveCursor (
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,
    BOOLEAN                     is_update_str,
    uint16			            prev_cursor_pos
);

/*****************************************************************************/
//  Description : update edit display
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_UpdateDisplay (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr, //in：
    GUIEDIT_UPDATE_T        *update_ptr     //in:may PNULL for fixed format
);

/*****************************************************************************/
//  Description : get first char index for single line edit
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_GetFirstCharIndex (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : progress is change,creat or destroy progress control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEFLEX_CreateOrDestroyPrgCtrl (//is update all
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : get scroll bar rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEFLEX_GetScrollBarRect (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : set line info,include start position,total line number and top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BaseFlexCtrlParseText (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    uint16              start_line_index        //in:
);

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : stop tp scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : handle tp edit scroll timer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpScrollTimer (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : handle edit tp down msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditTpDown (
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,
    DPARAM              param
);

/*****************************************************************************/
//  Description : handle edit tp short msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEFLEX_HandleTpShort (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle edit tp move msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpMove (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : handle edit tp long msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpLong (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : handle edit tp up msg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleEditTpUp (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : stop tp slide timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_StopTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : handle text tp slide timer msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleTpSlideTimer(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr
);
#endif

/*****************************************************************************/
//  Description : 显示所有内容
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BaseFlexCtrlDisplayAll (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_update_prgbox
);

/*****************************************************************************/
//  Description : display the number of remaining characters
//  Global resource dependence :
//  Author: Jassmine
//  Note: 自需求要求，当前输入法改用文本而勿用图标，即对本函数功能进行了扩展
/*****************************************************************************/
PUBLIC void BaseFlexCtrlDisplayLeftNum (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : 获取剩余字数信息
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BaseFlexCtrlGetLeftNumInfo ( // [ret] the left char num
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
    uint16                  str_max_len,            // [in] the max size of char
    wchar                   *str_ptr,               // [out] left char num string
    uint16                  *str_len_ptr            // [out] length of string
);

/*****************************************************************************/
//  Description : 当添加文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list/psw风格会继承
/*****************************************************************************/
static __inline void VTLBASEFLEX_OnTextAdded(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_ADD_STR_T       *add_str_ptr
    )
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->OnTextAdded)
    {
        baseflex_vtbl_ptr->OnTextAdded(baseflex_ctrl_ptr, add_str_ptr);
    }
}

/*****************************************************************************/
//  Description : 当删除文本后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list风格会继承
/*****************************************************************************/
static __inline void VTLBASEFLEX_OnTextDeleted(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
    )
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->OnTextDeleted)
    {
        baseflex_vtbl_ptr->OnTextDeleted(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 最大长度改变后的通知回调
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// text/psw风格会继承
/*****************************************************************************/
static __inline void VTLBASEFLEX_OnMaxLenChanged(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
    )
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->OnMaxLenChanged)
    {
        baseflex_vtbl_ptr->OnMaxLenChanged(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 检测字符串是否合法
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline BOOLEAN VTLBASEFLEX_IsStringValid(      // [ret] TRUE if char offset is valid
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    wchar                   *str_ptr,
    uint16                  str_len
    )
{
    BOOLEAN result = FALSE;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->IsStringValid)
    {
        result = baseflex_vtbl_ptr->IsStringValid(baseflex_ctrl_ptr, str_ptr, str_len);
    }

    return result;
}

/*****************************************************************************/
//  Description : 显示剩余字数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list风格做特殊处理
/*****************************************************************************/
static __inline void VTLBASEFLEX_DisplayLeftNum(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
    )
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->DisplayLeftNum)
    {
        baseflex_vtbl_ptr->DisplayLeftNum(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取剩余字数信息
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// text风格做特殊处理
/*****************************************************************************/
static __inline uint16 VTLBASEFLEX_GetLeftNumInfo(  // [ret] the left char num
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
    uint16                  str_max_len,            // [in] the max size of char
    wchar                   *str_ptr,               // [out] left char num string
    uint16                  *str_len_ptr            // [out] length of string
    )
{
    uint16 left_num = 0;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->GetLeftNumInfo)
    {
        left_num = baseflex_vtbl_ptr->GetLeftNumInfo(
                baseflex_ctrl_ptr,
                str_max_len,
                str_ptr,
                str_len_ptr);
    }

    return left_num;
}

/*****************************************************************************/
//  Description : 检查被删字符
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 此方法在删除时用到，因为在删除动作时，在确保当前有字符被选中，像list风格的话，
// 需要保证item object对象被整体选中
// list风格会有一个差异化的动作，其他都公用baseflex的
// text风格做特殊处理
/*****************************************************************************/
static __inline void VTLBASEFLEX_CheckDelStr(
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
    )
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->CheckDelStr)
    {
        baseflex_vtbl_ptr->CheckDelStr(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 检查添加的字符
//  Global resource dependence : 
//  Author: hua.fang
//  Param:
// add_str_ptr: 输入的字符
// actual_str_ptr: 实际被录入的字符，可以不返回，由系统统一录入
// delete_len_ptr: 被删除的字符，只有在is_del_after_full风格时有效
//  return:
// TRUE: commit的字符已经拷贝到actual_str_ptr;
// FALSE: 不拷贝字符，需要后续处理
//  Note:
// 此方法在输入字符时所用，在添加字符过程中，字符的编码会有特殊处理，有的会做为
// 一个对象来处理，所以这里做异样化
// list和text风格会有一个差异化的动作，其他都公用baseflex的
/*****************************************************************************/
static __inline BOOLEAN VTLBASEFLEX_CheckCommitStr(     //[ret] TRUE: commit的字符已经拷贝到actual_str_ptr; FALSE: 不拷贝字符，需要后续处理
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
    uint16                      *delete_len_ptr         //out:
)
{
    BOOLEAN result = TRUE;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->CheckCommitStr)
    {
        result = baseflex_vtbl_ptr->CheckCommitStr(
                baseflex_ctrl_ptr,
                add_str_ptr,
                actual_str_ptr,
                delete_len_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : 在添加字符前做检查
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit风格会重载
/*****************************************************************************/
static __inline void VTLBASEFLEX_BeforeAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->BeforeAddString)
    {
        baseflex_vtbl_ptr->BeforeAddString(baseflex_ctrl_ptr, actual_str_ptr);
    }
}

/*****************************************************************************/
//  Description : 在添加字符后做检查
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit风格会重载
/*****************************************************************************/
static __inline void VTLBASEFLEX_AfterAddString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
    GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
    GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
)
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->AfterAddString)
    {
        baseflex_vtbl_ptr->AfterAddString(baseflex_ctrl_ptr, add_str_ptr, actual_str_ptr);
    }
}

/*****************************************************************************/
//  Description : 在删除字符前做检查
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit风格会重载
/*****************************************************************************/
static __inline void VTLBASEFLEX_BeforeDelString(
    CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
)
{
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->BeforeDelString)
    {
        baseflex_vtbl_ptr->BeforeDelString(baseflex_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 文本解析
//  Global resource dependence : 
//  Author: hua.fang
//  Param:
// phonenum风格会重载，因为有大小字体变换
/*****************************************************************************/
static __inline BOOLEAN VTLBASEFLEX_ParseText(
    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
    uint16              start_line_index        //in:
)
{
    BOOLEAN result = TRUE;
    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

    if (PNULL != baseflex_vtbl_ptr->ParseText)
    {
        result = baseflex_vtbl_ptr->ParseText(
                baseflex_ctrl_ptr,
                start_line_index);
    }

    return result;
}

///*****************************************************************************/
////  Description : 获取指定宽度内能容纳的字数和象素值
////  Global resource dependence : 
////  Author: hua.fang
////  Param:
//// password风格会重载，因为会变成星号
///*****************************************************************************/
//static __inline BOOLEAN VTLBASEFLEX_GetStringInfo(
//    CTRLBASEFLEX_OBJ_T  *baseflex_ctrl_ptr,     //in:
//    int8                dir,                    //in:
//    uint16              start_pos,              //in:
//    uint16              str_len,                //in:
//    uint16              line_width,             //in:
//    BOOLEAN             is_byword,              //in:
//    uint16              *char_num_ptr,          //out:
//    uint16              *string_pixel_ptr       //out:
//)
//{
//    BOOLEAN result = FALSE;
//    CTRLBASEFLEX_VTBL_T *baseflex_vtbl_ptr = (CTRLBASEFLEX_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEFLEX_TYPE, baseflex_ctrl_ptr);

//    if (PNULL != baseflex_vtbl_ptr->GetStringInfo)
//    {
//        result = baseflex_vtbl_ptr->GetStringInfo(
//                baseflex_ctrl_ptr,
//                dir,
//                start_pos,
//                str_len,
//                line_width,
//                is_byword,
//                char_num_ptr,
//                string_pixel_ptr);
//    }

//    return result;
//}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
