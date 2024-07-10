/*****************************************************************************
** File Name:      ctrltext.h                                                *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _CTRLTEXT_H_
#define _CTRLTEXT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guistring.h"
#include "guitext.h"
#include "mmitheme_text.h"
#include "mmitheme_clipbrd.h"

#include "ctrlbase.h"
#include "ctrltext_export.h"

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
// text type info
#define CTRL_TEXT_TYPE     (TEXT_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

// text init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;
} CTRLTEXT_INIT_PARAM_T;

//tp state
typedef enum
{
    GUITEXT_TP_STATE_NONE, //none
    GUITEXT_TP_STATE_DRAG, //托选状态
    GUITEXT_TP_STATE_MAX
} GUITEXT_TP_STATE_E;

//tp region
typedef enum
{
    GUITEXT_TP_REGION_NONE,     /*!<保留位*/
    GUITEXT_TP_REGION_HIGHLIGHT,/*!<高亮区域*/
    GUITEXT_TP_REGION_MAX       /*!<保留位*/
} GUITEXT_TP_REGION_E;

//text highlight info
typedef struct
{
    uint16      start_pos;  /*!<高亮启示位置*/
    uint16      end_pos;    /*!<高亮结束位置*/
} GUITEXT_HIGHLIGHT_T;

//one tag info
typedef struct guitext_tag
{
    uint16              start_pos;  //tag start position
    uint16              len;        //tag length
    GUITEXT_TAG_TYPE_E  type;       //tag type

    struct guitext_tag  *prev_ptr;  //previous tag pointer
    struct guitext_tag  *next_ptr;  //next tag pointer
} GUITEXT_TAG_T;

//text line info
typedef struct
{
    int8            direction;      //SPML_TEXT_DIR_T:multi language
    uint16          start_pos;      //line char start position
    uint16          tag_num;        //tag number in one line
    GUITEXT_TAG_T   *start_tag_ptr; //start tag pointer in one line
} GUITEXT_LINE_T;

// 当前处于哪个拖动状态
typedef enum
{
    GUITEXT_DRAGSTATE_NORMAL,       // 普通的光标状态
    GUITEXT_DRAGSTATE_NORMAL_RESET, // 普通的光标状态，重置高亮
    GUITEXT_DRAGSTATE_SELECT,       // 选择文本状态
    GUITEXT_DRAGSTATE_MAX
} GUITEXT_DRAGSTATE_E;

// 处于哪个滑块上
typedef enum
{
    GUITEXT_SLIDER_NONE,            // 没有滑块，此时应该处于GUITEXT_DRAGSTATE_NORMAL状态
    GUITEXT_SLIDER_LEFT_SELECTOR,   // 左选择块，此时应该处于GUITEXT_DRAGSTATE_SELECT
    GUITEXT_SLIDER_RIGHT_SELECTOR,  // 右选择块，此时应该处于GUITEXT_DRAGSTATE_SELECT
} GUITEXT_SLIDER_E;

//text control
typedef struct _text_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    BOOLEAN                     is_active;          //text is active
    BOOLEAN                     is_display_prg;     //is display progress

    BOOLEAN                     is_tp_down;         //is tp down
    uint8                       slide_timer_id;     //slide timer id
    
    BOOLEAN                     is_time_scroll;     //is time scroll
    uint8                       scroll_timer_id;    //scroll timer id

    BOOLEAN                     is_tp_scroll_up;    //is tp scroll up
    uint8                       tp_scroll_timer_id; //tp scroll timer id
    int16                       tp_down_y;          //tp down y
    GUI_POINT_T                 tp_move_point;      //tp move point

    wchar                       *str_ptr;           //current string
    uint16                      str_len;            //string length

    uint16                      page_line_num;      //line number in one page
    uint16                      top_line_index;     //line index at the top,from 0
    uint16                      bottom_line_index;  //line index at the bottom,from 0
    uint16                      total_line_num;     //total line number,from 1
    GUITEXT_LINE_T              *line_info_ptr;     //all line info,from 0

    GUITEXT_TP_STATE_E          tp_state;           //tp state;
    GUITEXT_TP_REGION_E         tp_region;          //tp region
    GUITEXT_HIGHLIGHT_T         highlight;          //highlight info
    
    uint16                      tag_num;            //tag total number
    GUITEXT_TAG_T               *first_tag_ptr;     //first tag pointer
    GUITEXT_TAG_T               *cur_tag_ptr;       //current tag pointer

    float                       fling_velocity;     //fling velocity
    GUI_POINT_T                 start_point;        //slide start point
    GUI_POINT_T                 prev_point;         //slide previous point

    MMK_TP_SLIDE_E              slide_state;        //slide state
    MMK_TP_SLIDE_E              tpdown_slide_state; //tp down slide state

    GUI_BIG_RECT_T              relative_rect;      //relative rect,relative to the text with except border,原点是text除去边框
    GUI_BIG_RECT_T              relative_disp_rect; //relative display rect,relative to the text with except border,原点是text除去边框

    IGUICTRL_T                  *prgbox_ctrl_ptr;   //progress box control pointer

#ifdef GUIF_CLIPBOARD
    BOOLEAN                     is_clipbrd_support; // whether support clipbord
    BOOLEAN                     is_tp_moved;        //check whether tp is moved or not
    MMI_HANDLE_T                clipbrd_win_handle; //clipbrd win handle
#endif

    uint32                      slider[MMITHEME_SLIDER_TYPE_MAX];   // 滑块句柄
    GUITEXT_DRAGSTATE_E         drag_state;                         // 当前滑动状态
    GUITEXT_SLIDER_E            tpdown_slider;                      // TPDOWN时的，点击的区域
    uint8                       slider_dis_timer_id;                // 滑块消失定时器

    MMITHEME_TEXT_T             theme;              //text theme
    
    MMI_HANDLE_T                win_handle;         //window handle
} CTRLTEXT_OBJ_T;

// text class for public or protected method
typedef struct _text_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

} CTRLTEXT_VTBL_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get text type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T TEXT_GetType();

/*****************************************************************************/
//  Description : display text background
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayBg(
                              GUI_RECT_T        bg_rect,
                              CTRLTEXT_OBJ_T    *text_ctrl_ptr
                              );

/*****************************************************************************/
//  Description : display text control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayText(
                                BOOLEAN          is_update_prgbox,
                                CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : handle text tp move msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E GUITEXT_HandleTextTpMove(
                                             CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                             DPARAM          param
                                             );

/*****************************************************************************/
//  Description : stop tp scroll timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_StopTpScrollTimer(
                                      CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                      );

/*****************************************************************************/
//  Description : get text rect,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetRect(
                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                  );

/*****************************************************************************/
//  Description : get text line rect,except dividing line,progress bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetLineRect(
                                      uint16            line_index,     //in:from 0
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : get line relative rect,except dividing line,progress bar
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_BIG_RECT_T GUITEXT_GetLineRelativeRect(
                                                  uint16            line_index,     //in:from 0
                                                  CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                                  );

/*****************************************************************************/
//  Description : get one line text string rect,except margin
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T GUITEXT_GetLineStrRect(
                                         uint16             line_index,     //in:from 0
                                         CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                         );

/*****************************************************************************/
//  Description : set text display rect
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_SetDisplayRect(
                                      int32             display_top,    //in:
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr  //in:
                                      );

/*****************************************************************************/
//  Description : get text display rect height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetDispHeight(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                    );

/*****************************************************************************/
//  Description : get text display rect max top
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 GUITEXT_GetDispMaxTop(
                                   CTRLTEXT_OBJ_T   *text_ctrl_ptr
                                   );

/*****************************************************************************/
//  Description : get text relative height,except border
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUITEXT_GetRelativeHeight(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get text line height,include line space
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineHeight(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                    );

/*****************************************************************************/
//  Description : get scroll bar width
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetScrollBarWidth(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                        );

/*****************************************************************************/
//  Description : get line string length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineStrLen(
                                    uint16          line_index,     //in:from 0
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                    );

/*****************************************************************************/
//  Description : get line string region style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetLineStrRegion(//region number
                                       BOOLEAN                  is_highlight,       //in:
                                       uint16                   line_index,         //in:from 0
                                       uint16                   region_max_num,     //in:
                                       CTRLTEXT_OBJ_T           *text_ctrl_ptr,     //in:
                                       GUIFONT_REGION_STYLE_T   *region_style_ptr   //in/out:
                                       );

/*****************************************************************************/
//  Description : display text highlight string
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayHighlight(
                                     BOOLEAN            is_highlight,   //in:FASLE:only display string
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : get current highlight line index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightLineIndex(
                                            BOOLEAN         is_end,
                                            uint16          highlight_pos,
                                            CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                            );

/*****************************************************************************/
//  Description : get text highlight length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightLen(
                                      CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                      );

/*****************************************************************************/
//  Description : get highlight start pos len in the line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightStartPosAndLen(
                                                 uint16             *start_pos_ptr, //in/out:相对于行首
                                                 uint16             line_index,     //in:
                                                 CTRLTEXT_OBJ_T     *text_ctrl_ptr  //in:
                                                 );

/*****************************************************************************/
//  Description : get highlight string and length
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetHighlightStr(
                                       wchar            **highlight_str_pptr,   //in/out:
                                       uint16           *highlight_len_ptr,     //in/out:
                                       CTRLTEXT_OBJ_T   *text_ctrl_ptr          //in:
                                       );

/*****************************************************************************/
//  Description : get the highlight minor position
//  Global resource dependence : 
//  Author: Jassmine
//  Note: must 
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightMinPos(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get the highlight max position
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUITEXT_GetHighlightMaxPos(
                                CTRLTEXT_OBJ_T  *text_ctrl_ptr
                                );

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_GetPosRect(
                                  CTRLTEXT_OBJ_T     *text_ctrl_ptr,      //in
                                  uint16             pos,
                                  GUI_RECT_T         *rect_ptr   //in/out:
                                  );

#ifdef GUIF_CLIPBOARD

/*******************************************************************************************/
//  Description : whether support clipbrd
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*********************************************************************************************/
PUBLIC BOOLEAN GUITEXT_IsSupportClipbrd(
                                        CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                        );

/*****************************************************************************/
//  Description : display clip board menu
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DisplayClipboardMenu(
                                         GUI_POINT_T     *point_ptr,     //in:
                                         CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                         );

/*****************************************************************************/
//  Description : close clip board menu
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_CloseClipboardMenu(
                                      CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                      );

/*****************************************************************************/
//  Description : handle text copy msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextCopyMsg(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr
                                     );

/*****************************************************************************/
//  Description : handle text select msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextSelectMsg(
                                       CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                       );

/*****************************************************************************/
//  Description : handle text select all msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleTextSelectAllMsg(
                                          CTRLTEXT_OBJ_T    *text_ctrl_ptr
                                          );

/*****************************************************************************/
//  Description : destroy slider
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_DestroyTextSlider(
                                     CTRLTEXT_OBJ_T  *text_ctrl_ptr  //in:
                                     );

/*****************************************************************************/
//  Description : 显示滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_ShowSlider(
                               CTRLTEXT_OBJ_T  *text_ctrl_ptr
                               );

/*****************************************************************************/
//  Description : 改变滑块状态
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_ChangeDragState(
                                    CTRLTEXT_OBJ_T  *text_ctrl_ptr,
                                    GUITEXT_DRAGSTATE_E drag_state
                                    );

/*****************************************************************************/
//  Description : handle slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUITEXT_HandleSliderTimer(
                                         CTRLTEXT_OBJ_T     *text_ctrl_ptr,  //in:
                                         uint8              time_id
                                         );

/*****************************************************************************/
//  Description : 开始滑块拖动
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleBeginSlider(
                                     CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                     DPARAM             param
                                     );

/*****************************************************************************/
//  Description : 拖动滑块
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleMoveSlider(
                                    CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                    DPARAM             param
                                    );

/*****************************************************************************/
//  Description : 滑块拖动结束
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void GUITEXT_HandleEndSlider(
                                   CTRLTEXT_OBJ_T     *text_ctrl_ptr,
                                   DPARAM             param
                                   );

#endif  // GUIF_CLIPBOARD

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
