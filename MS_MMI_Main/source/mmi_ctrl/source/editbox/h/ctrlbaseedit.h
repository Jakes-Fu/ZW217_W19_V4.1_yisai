/*****************************************************************************
** File Name:      ctrlbaseedit.h                                               *
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

#ifndef _CTRLBASEEDIT_H_
#define _CTRLBASEEDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "ctrlbaseedit_export.h"
#include "mmitheme_edit.h"
#include "mmi_string.h"

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
// baseedit type info
#define CTRL_BASEEDIT_TYPE     (BASEEDIT_GetType())

#define GUIEDIT_CURSOR_DEFAULT_WIDTH        2       //cursor default width
#define GUIEDIT_CURSOR_INVALID_COORDINATE   0x7F    //int16,cursor invalid coordinate

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// which will be set when GUIEDIT_SetHighlight
typedef enum
{
    SETHIGHLIGHT_START  = 0x0001,   // start pos
    SETHIGHLIGHT_END    = 0x0002,   // end pos
    SETHIGHLIGHT_BOTH   = 0x0003    // both pos
} SETHIGHLIGHT_E;

//edit add string
typedef struct
{
    BOOLEAN     is_im_commit;   //is im commit string
    BOOLEAN     is_list_item;   //is add list item
    BOOLEAN     is_list_item_object;    //is add list item object
    wchar       *commit_ptr;    //add string
    uint16      commit_len;     //add string len
    wchar       *underline_ptr; //underline string
    uint16      underline_len;  //underline string len
    wchar       *user_data_ptr; // valid when is_list_item_object is true
    uint16      user_data_len;  // valid when is_list_item_object is true
} GUIEDIT_ADD_STR_T;

/*! @struct GUIEDIT_CURSOR_T
@brief 编辑框光标信息
*/
typedef struct
{
    BOOLEAN         is_hide;        /*!<是否隐藏光标*/
    BOOLEAN         is_display;     /*!<是否显示光标*/
    BOOLEAN         is_rect_valid;  /*!<光标区域是否有效*/
    uint8           timer_id;       /*!<光标定时器*/
    int16           x_coordinate;   /*!<光标x坐标,无效值为GUIEDIT_CURSOR_INVALID_COORDINATE*/
    uint16          cur_pos;        /*!<当前光标位置,从0开始*/
    uint32          bg_buf_size;    /*!<光标背景buffer大小*/
    GUI_COLOR_T     *bg_buf_ptr;    /*!<保存光标背景数据,防止光标显示在字符上闪烁错误*/
    GUI_RECT_T      rect;           /*!<光标显示区域*/
} GUIEDIT_CURSOR_T;

/*! @struct GUIEDIT_LINE_T
@brief 编辑框行信息
*/
typedef struct
{
    int8        direction;  /*!<文本显示方向SPML_TEXT_DIR_T*/
    uint16      start_pos;  /*!<行起始位置*/
} GUIEDIT_LINE_T;

/*! @struct guiedit_config_key_tag
@brief 编辑框输入法键值配置信息edit im config custom key
*/
typedef struct guiedit_config_key_tag
{
    wchar               *value_ptr; /*!<键值数组*/
    uint32              value_count;/*!<键值个数*/
    GUIIM_TYPE_T        im_type;    /*!<输入法类型*/
    GUIIM_CUSTOM_KEY_T  custom_key; /*!<键值*/

    struct guiedit_config_key_tag   *prev_ptr;  /*!<前一个键值配置信息*/
    struct guiedit_config_key_tag   *next_ptr;  /*!<后一个键值配置信息*/
} GUIEDIT_CONFIG_KEY_T;

// baseedit init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    BOOLEAN                     is_rect_adaptive;   /*!<区域是否根据字符串自动调整*/
    uint16                      str_max_len;        /*!<允许的最大字符长度*/
    GUIEDIT_TYPE_E              type;               /*!<编辑框类型*/
} CTRLBASEEDIT_INIT_PARAM_T;

//baseedit control
typedef struct
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_permit_border;   /*!<是否允许显示边框*/
    BOOLEAN                     is_bg_transparent;  /*!<背景是否半透*/
    BOOLEAN                     is_rect_adaptive;   /*!<是否自动调整控件区域*/

    BOOLEAN                     is_create_im_active;/*!<当edit get active时是否创建im*/
    BOOLEAN                     is_disp_im_icon;    /*!<是否显示输入法图标*/
    MMI_HANDLE_T                im_ctrl_handle;     /*!<输入法handle*/
    GUIEDIT_CONFIG_KEY_T        *im_config_key_ptr; /*!<输入法配置键信息*/

    wchar                       *str_ptr;           /*!<文本*/
    uint16                      str_len;            /*!<文本长度*/
    uint16                      str_max_len;        /*!<文本最大长度,申请内存*/
	BOOLEAN						is_first_plus;  // 电话号码首字母是否为"+"
    // 单行时
    // line_info_ptr[0].start_pos记录文本的开始显示位置，在此之前的为不可见
    // line_info_ptr[1].start_pos为下一页显示的位置
    GUIEDIT_LINE_T              *line_info_ptr;     /*!<所有行信息,从0开始*/

    uint16                      total_line_num;     /*!<总行数,从1开始*/

    int32                       top_line_offset;    // top line离可见顶部的距离
                                                    // 在顶部之上为正，在顶部之下为负

    GUIEDIT_CURSOR_T            cursor;             /*!<光标信息*/
    GUIEDIT_HIGHLIGHT_T         highlight;          /*!<高亮信息*/

    GUIEDIT_TYPE_E              type;               /*!<编辑框类型*/
    GUIEDIT_STYLE_E             style;              /*!<编辑框风格*/
    GUIEDIT_DISPLAY_DIR_E       display_dir;        /*!<编辑框字符显示方向*/

    MMITHEME_EDIT_T             common_theme;       /*!<编辑框风格*/

    GUI_RECT_T                  original_rect;      /*!<控件原始区域,不包含输入法*/

    IGUICTRL_T                  *prgbox_ctrl_ptr;   /*!<滚动条指针*/

} CTRLBASEEDIT_OBJ_T;

// baseedit class for public or protected method
typedef struct
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    /*****************************************************************************/
    //  Description : 区域改变时的行为
    /*****************************************************************************/
    void (*OnRectChanged)(
            CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
            );

    /*****************************************************************************/
    //  Description : 获取对齐方式
    /*****************************************************************************/
    GUI_ALIGN_E (*GetAlign)(
            CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
            );

    /*****************************************************************************/
    //  Description : 设置对齐方式
    /*****************************************************************************/
    void (*SetAlign)(
            CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,
            GUI_ALIGN_E             align
            );

    /*****************************************************************************/
    //  Description : 获取输入法配置信息
    /*****************************************************************************/
    void (*GetImInfo)(
            CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
            BOOLEAN                *is_forbid_ptr,     //in/out:
            GUIIM_TYPE_T           *init_im_ptr,       //in/out:
            GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
            GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
            GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
            GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
            GUIIM_TAG_T            *tag_ptr            //in/out:
            );

    /*****************************************************************************/
    //  Description : 设置输入法配置信息
    /*****************************************************************************/
    void (*SetImInfo)(
            CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
            BOOLEAN                *is_forbid_ptr,     //in:may PNULL
            GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
            GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
            GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
            GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
            GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
            GUIIM_TAG_T            *tag_ptr
            );

    /*****************************************************************************/
    //  Description : 配置输入法星号键，井号键，和数字1键上的键值
    /*****************************************************************************/
    void (*ConfigImKey)(
            CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
            );

    /**************************************************************************/
    //  Description : 设置横屏时的新edit
    /**************************************************************************/
    void (*SetLandscapeEdit)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
        MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
        );

    /**************************************************************************/
    //  Description : 设置光标位置
    /**************************************************************************/
    void (*SetCursorPos)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
        uint16			    cur_pos                 // [in] from 0
        );

    /**************************************************************************/
    //  Description : 设置高亮
    /**************************************************************************/
    void (*SetHighlight)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //[in]
        uint16              start_pos,              //[in]
        uint16              end_pos,                //[in]
        SETHIGHLIGHT_E      highlight_type          //[in]
        );

    /**************************************************************************/
    //  Description : 获取显示的字符串内容
    /**************************************************************************/
    wchar* (*GetDispStr)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        uint16              start_pos,
        uint16              str_len
        );

    /**************************************************************************/
    //  Description : 获取总行数
    /**************************************************************************/
    uint16 (*GetLineNum)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
        uint16              display_width           // [in] width
        );

    /**************************************************************************/
    //  Description : 显示所有内容
    /**************************************************************************/
    void (*DisplayAll)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        BOOLEAN             is_update_prgbox
        );

    /**************************************************************************/
    //  Description : 检查行区域
    /**************************************************************************/
    void (*CheckLineRect)(
        CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
        GUI_RECT_T          *line_rect_ptr          // [in/out]
        );

} CTRLBASEEDIT_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get baseedit type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEEDIT_GetType(void);

/*****************************************************************************/
//  Description : get baseedit pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC CTRLBASEEDIT_OBJ_T* BASEEDIT_GetPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : resize edit rect by im rect
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_ResizeRect (//edit is change
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr  //in
);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetCursorRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //in
    uint16              *cursor_line_ptr,   //in/out:may PNULL
    GUI_RECT_T          *cursor_rect_ptr    //in/out:
);

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetPosRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //in
    uint16              pos,                    //in
    GUI_RECT_T          *rect_ptr               //out
);

/*****************************************************************************/
//  Description : get current cursor line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:除第一行,光标前一个字符是换行符之外，光标不能显示在行首位置
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetCursorLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              cursor_pos
);

/*****************************************************************************/
//  Description : 光标的对齐方式
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_ALIGN_E BASEEDIT_GetCursorAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get cursor left coordinate
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC int16 BASEEDIT_GetLeftCoordinate (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in
    uint16              line_index,         //in
    uint16              pos,                //in
    GUI_RECT_T          text_rect           //in
);
/*****************************************************************************/
//  Description : display edit highlight
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayHighlight (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    BOOLEAN                is_highlight         //in:FASLE:only display string
);

/*****************************************************************************/
//  Description : get highlight start pos len in the line
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightStartPosAndLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index,         //in:
    uint16              *start_pos_ptr      //in/out:相对于行首
);

/*****************************************************************************/
//  Description : get edit highlight length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get the highlight minor position
//  Global resource dependence :
//  Author: Jassmine
//  Note: must
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightMinPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get the highlight max position
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetHighlightMaxPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : set edit no highlight
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_SetNoHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              pos
);

/*****************************************************************************/
//  Description : create im control and set it active
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_CreateImCtrl (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : destroy im control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_DestroyImCtrl (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr //in:
);

/*****************************************************************************/
//  Description : reset im
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ResetIm (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : set im active
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_SetImActive (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许TP IM
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_GetImTp (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : edit handle im switch input type, current im
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_HandleImSwitch (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr  //in:
);

/*****************************************************************************/
//  Description : convert commit msg to GUIEDIT_ADD_STR_T
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ConvertImCommitMsg(
    DPARAM              param,
    GUIEDIT_ADD_STR_T   *commit_str_ptr
);

/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_Init(
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : alloc edit string memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC wchar* BASEEDIT_AllocStrMemory (
    uint16     max_len
);

/*****************************************************************************/
//  Description : alloc edit line info memory
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_LINE_T* BASEEDIT_AllocLineMemory (
    uint16   max_len
);

/*****************************************************************************/
//  Description : notify parent control or parent window notify message
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEEDIT_NotifyParentMsg (
    MMI_HANDLE_T        ctrl_handle,
    MMI_MESSAGE_ID_E    notify_msg
);

/*****************************************************************************/
//  Description : notify parent total line number has change
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyLineMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              prev_total_num  //in:
);

/*****************************************************************************/
//  Description : notify parent edit move cursor
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_NotifyCursorMsg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : display edit background
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplayBg (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    GUI_RECT_T          bg_rect
);

/*****************************************************************************/
//  Description : update text string bg
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_UpdateTextBg (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr,     //in:
    uint16                  start_line_index,       //in:
    uint16                  end_line_index          //in:
);

/*****************************************************************************/
//  Description : get edit display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetDispHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get edit rect,except border
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetClientRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get edit line height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get edit line rect,except dividing line,progress bar
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetLineRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index          //in:from 0
);

/*****************************************************************************/
//  Description : get one line text rect,except margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEEDIT_GetTextRect (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              line_index      //in:from 0
);

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetLineStrLen (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index          //in:from 0
);

/*****************************************************************************/
//  Description : adjust cursor/highlight rect left position by align
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_AdjustLeftByAlign (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    uint16              line_index,         //in:
    GUI_RECT_T          *rect_ptr           //in/out:
);

/*****************************************************************************/
//  Description : get font height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetFontHeight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get edit separator char
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC char BASEEDIT_GetSeparatorChar (
    GUIEDIT_SEPARATOR_E    separator_style
);

/*****************************************************************************/
//  Description : edit is fixed format,for example time,date,IP
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsFixedFormat (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert line index,when display from right and bottom,need convert
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC uint16 BASEEDIT_ConvertLineIndex (
//    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
//    uint16              index
//);

/*****************************************************************************/
//  Description : 用来判断字符是否为换行符
//  Global resource dependence :
//  Author: Jassmine
//  Note:
// 由于密码风格不需要换行，所以永远也不可能是换行符
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsNewLineChar (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    uint16              char_index
);

/*****************************************************************************/
//  Description : get line number in one page
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetPageLineNum (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get total content height
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 BASEEDIT_GetTotalHeight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get display top line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetTopLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get display bottom line index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEEDIT_GetBottomLineIndex (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : get top line offset range
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_GetTopLineOffsetRange(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //in:
    int32               *min_top_line_offset_ptr,
    int32               *max_top_line_offset_ptr
);

/*****************************************************************************/
//  Description : edit's parent is control
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_IsParentCtrl(
                                     CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr
                                     );

/*****************************************************************************/
// Description : 让softkey处理消息
// Global resource dependence :
// Author:liqing.peng
// Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E BASEEDIT_HandleSoftkeyMsg (
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr,
    MMI_MESSAGE_ID_E       msg_id,
    DPARAM                 param
);

/*****************************************************************************/
// Description : 设置softkey的状态
// Global resource dependence :
// Author:liqing.peng
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEEDIT_SetSoftkeyBtnState (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr,
    uint16                  button_index,
    BOOLEAN                 is_gray,
    BOOLEAN		            is_long_press
);

/*****************************************************************************/
//  Description : display edit softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_DisplaySoftkey (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : reset edit softkey
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BASEEDIT_ResetSoftkey (
    CTRLBASEEDIT_OBJ_T      *baseedit_ctrl_ptr
);

/*****************************************************************************/
//  Description : set highlight position
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BaseEditCtrlSetHighlight (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr, //[in]
    uint16              start_pos,          //[in]
    uint16              end_pos,            //[in]
    SETHIGHLIGHT_E      highlight_type      //[in]
);

/*****************************************************************************/
//  Description : set cursor position
//  Global resource dependence :
//  Author: jassmine
//  Note:
/*****************************************************************************/
PUBLIC void BaseEditCtrlSetCursorPos (
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,	//in:
    uint16			    cur_pos			    //in:from 0
);

/*****************************************************************************/
//  Description : 区域改变时的行为
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// baseflex将重载该方法
/*****************************************************************************/
static __inline void VTLBASEEDIT_OnRectChanged(
                       CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
                       )
{
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->OnRectChanged)
    {
        baseedit_vtbl_ptr->OnRectChanged(baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 获取对齐方式
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline GUI_ALIGN_E VTLBASEEDIT_GetAlign(
    CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr
)
{
    GUI_ALIGN_E align = ALIGN_LEFT;
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->GetAlign)
    {
        align = baseedit_vtbl_ptr->GetAlign(baseedit_ctrl_ptr);
    }

    return align;
}

/*****************************************************************************/
//  Description : 设置对齐方式
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline void VTLBASEEDIT_SetAlign(
                       CTRLBASEEDIT_OBJ_T*     baseedit_ctrl_ptr,
                       GUI_ALIGN_E             align
                       )
{
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->SetAlign)
    {
        baseedit_vtbl_ptr->SetAlign(baseedit_ctrl_ptr, align);
    }
}

/*****************************************************************************/
//  Description : 获取输入法配置信息
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline void VTLBASEEDIT_GetImInfo(
    CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
    BOOLEAN                *is_forbid_ptr,     //in/out:
    GUIIM_TYPE_T           *init_im_ptr,       //in/out:
    GUIIM_TYPE_T           *allow_im_ptr,      //in/out:
    GUIIM_INPUT_TYPE_T     *init_input_ptr,    //in/out:
    GUIIM_INPUT_TYPE_T     *allow_input_ptr,   //in/out:
    GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in/out:
    GUIIM_TAG_T            *tag_ptr            //in/out:
    )
{
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->GetImInfo)
    {
        baseedit_vtbl_ptr->GetImInfo(
            baseedit_ctrl_ptr,
            is_forbid_ptr,
            init_im_ptr,
            allow_im_ptr,
            init_input_ptr,
            allow_input_ptr,
            capital_mode_ptr,
            tag_ptr
            );
    }
}

/*****************************************************************************/
//  Description : 设置输入法配置信息
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline void VTLBASEEDIT_SetImInfo(
            CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr, //in:
            BOOLEAN                *is_forbid_ptr,     //in:may PNULL
            GUIIM_TYPE_T           *init_im_ptr,       //in:may PNULL
            GUIIM_TYPE_T           *allow_im_ptr,      //in:may PNULL
            GUIIM_INPUT_TYPE_T     *init_type_ptr,     //in:may PNULL
            GUIIM_INPUT_TYPE_T     *allow_type_ptr,    //in:may PNULL
            GUIIM_CAPITAL_MODE_T   *capital_mode_ptr,  //in:may PNULL
            GUIIM_TAG_T            *tag_ptr
            )
{
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->SetImInfo)
    {
        baseedit_vtbl_ptr->SetImInfo(
            baseedit_ctrl_ptr,
            is_forbid_ptr,
            init_im_ptr,
            allow_im_ptr,
            init_type_ptr,
            allow_type_ptr,
            capital_mode_ptr,
            tag_ptr
            );
    }
}

/*****************************************************************************/
//  Description : 配置输入法星号键，井号键，和数字1键上的键值
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// basefixed已经重载，固定风格的edit无需再重载
// 各个flex风格的子控件需重载
/*****************************************************************************/
static __inline void VTLBASEEDIT_ConfigImKey(
            CTRLBASEEDIT_OBJ_T     *baseedit_ctrl_ptr
            )
{
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->ConfigImKey)
    {
        baseedit_vtbl_ptr->ConfigImKey(baseedit_ctrl_ptr);
    }
}

/*****************************************************************************/
//  Description : 设置横屏时的新edit
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 各个子控件都需重载
/*****************************************************************************/
static __inline void VTLBASEEDIT_SetLandscapeEdit(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    MMI_HANDLE_T        landscape_edit          // [in] the dst landscape edit
)
{
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->SetLandscapeEdit)
    {
        baseedit_vtbl_ptr->SetLandscapeEdit(baseedit_ctrl_ptr, landscape_edit);
    }
}

/*****************************************************************************/
//  Description : 设置光标位置
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 现在只有list风格比较特殊，会做处理
/*****************************************************************************/
static __inline void VTLBASEEDIT_SetCursorPos(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16			    cur_pos                 // [in] from 0
)
{
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->SetCursorPos)
    {
        baseedit_vtbl_ptr->SetCursorPos(baseedit_ctrl_ptr, cur_pos);
    }
}

/*****************************************************************************/
//  Description : 设置高亮
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 现在只有list风格比较特殊，会做处理
/*****************************************************************************/
static __inline void VTLBASEEDIT_SetHighlight(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //[in] ctrl ptr
    uint16              start_pos,              //[in]
    uint16              end_pos,                //[in]
    SETHIGHLIGHT_E      highlight_type          //[in]
)
{
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->SetHighlight)
    {
        baseedit_vtbl_ptr->SetHighlight(baseedit_ctrl_ptr, start_pos, end_pos, highlight_type);
    }
}

/*****************************************************************************/
//  Description : 获取显示的字符串内容
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// 如果是password风格，获取的是星号
/*****************************************************************************/
static __inline wchar* VTLBASEEDIT_GetDispStr(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     //[in] ctrl ptr
    uint16              start_pos,
    uint16              str_len
)
{
    wchar* disp_str_ptr = PNULL;
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->GetDispStr)
    {
        disp_str_ptr = baseedit_vtbl_ptr->GetDispStr(baseedit_ctrl_ptr, start_pos, str_len);
    }

    return disp_str_ptr;
}

/*****************************************************************************/
//  Description : 获取总行数
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// baseflex和basefixed各自重载
// phonenum如果是从下往上显示的，需要做特殊处理
// psw要算星号，需做特殊处理
/*****************************************************************************/
static __inline uint16 VTLBASEEDIT_GetLineNum(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,     // [in] ctrl ptr
    uint16              display_width           // [in] width
)
{
    uint16 line_num = 0;
    CTRLBASEEDIT_VTBL_T  *baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->GetLineNum)
    {
        line_num = baseedit_vtbl_ptr->GetLineNum(baseedit_ctrl_ptr, display_width);
    }

    return line_num;
}

/*****************************************************************************/
//  Description : 显示所有内容
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// baseflex和basefixed各做特殊处理
/*****************************************************************************/
static __inline void VTLBASEEDIT_DisplayAll(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    BOOLEAN             is_update_prgbox
)
{
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->DisplayAll)
    {
        baseedit_vtbl_ptr->DisplayAll(baseedit_ctrl_ptr, is_update_prgbox);
    }
}

/*****************************************************************************/
//  Description : 检查行区域
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// baseflex因为有滚动条，有特殊处理
/*****************************************************************************/
static __inline void VTLBASEEDIT_CheckLineRect(
    CTRLBASEEDIT_OBJ_T  *baseedit_ctrl_ptr,
    GUI_RECT_T          *line_rect_ptr          // [in/out]
)
{
    CTRLBASEEDIT_VTBL_T* baseedit_vtbl_ptr = (CTRLBASEEDIT_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEEDIT_TYPE, baseedit_ctrl_ptr);

    if (PNULL != baseedit_vtbl_ptr->CheckLineRect)
    {
        baseedit_vtbl_ptr->CheckLineRect(baseedit_ctrl_ptr, line_rect_ptr);
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
