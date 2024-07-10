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
@brief ���ʵ������
*/
typedef enum
{
    GUIEDIT_TP_REGION_NONE,     /*!<����λ*/
    GUIEDIT_TP_REGION_HIGHLIGHT,/*!<��������*/
    GUIEDIT_TP_REGION_MAX       /*!<����λ*/
} GUIEDIT_TP_REGION_E;

/*! @enum GUIEDIT_TP_STATE_E
@brief ���ʵ��״̬
*/
//tp state
typedef enum
{
    GUIEDIT_TP_STATE_NONE,  /*!<����λ*/
    GUIEDIT_TP_STATE_DRAG,  /*!��ѡ״̬*/
    GUIEDIT_TP_STATE_MAX    /*!<����λ*/
} GUIEDIT_TP_STATE_E;

// ��ǰ�����ĸ��϶�״̬
typedef enum
{
    GUIEDIT_DRAGSTATE_NORMAL,       // ��ͨ�Ĺ��״̬
    GUIEDIT_DRAGSTATE_NORMAL_RESET, // ��ͨ�Ĺ��״̬��ͬʱȥ������
    GUIEDIT_DRAGSTATE_DRAG,         // �϶���λ���״̬
    GUIEDIT_DRAGSTATE_SELECT,       // ѡ���ı�״̬
    GUIEDIT_DRAGSTATE_MAX
} GUIEDIT_DRAGSTATE_E;

// �����ĸ�������
typedef enum
{
    GUIEDIT_SLIDER_NONE,            // û�л��飬��ʱӦ�ô���GUIEDIT_DRAGSTATE_NORMAL״̬
    GUIEDIT_SLIDER_BLOCK,           // �϶����飬��ʱӦ�ô���GUIEDIT_DRAGSTATE_DRAG
    GUIEDIT_SLIDER_LEFT_SELECTOR,   // ��ѡ��飬��ʱӦ�ô���GUIEDIT_DRAGSTATE_SELECT
    GUIEDIT_SLIDER_RIGHT_SELECTOR,  // ��ѡ��飬��ʱӦ�ô���GUIEDIT_DRAGSTATE_SELECT
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

    BOOLEAN                     is_disp_num_info;   /*!<�Ƿ���ʾʣ���ַ������б���Ϣ*/
    BOOLEAN                     is_display_prg;     /*!<�Ƿ���ʾ������*/

    BOOLEAN                     is_tp_rect_change;  /*!<�Ƿ񴥱�down��Ϣ����ؼ�*/

    BOOLEAN                     is_disable_cutcopy; // �Ƿ��ֹ���С�����
    BOOLEAN                     is_byword;          // �Ƿ�ϴ�
    BOOLEAN                     is_del_after_full;  /*!<��������,�������ַ�,�Ƿ�ɾ����һ���ַ�*/

    BOOLEAN                     is_tp_scroll_up;    /*!<�Ƿ񴥱ʹ����ַ��������ϵ�λ��*/
    uint8                       tp_scroll_timer_id; /*!<���ʶ�ʱ��*/
    GUI_POINT_T                 tp_move_point;      /*!<�����ƶ�������*/

    GUIEDIT_TP_STATE_E          tp_state;           /*!<���ʵ��״̬*/
    GUIEDIT_TP_REGION_E         tp_region;          /*!<���ʵ������*/

    wchar                       *default_str_ptr;   // Ĭ���ı�����
    uint16                      default_str_len;    // Ĭ���ı�����

    wchar                       *disable_char_ptr;  /*!<������������ַ���*/
    uint16                      disable_char_len;   /*!<������������ַ�����*/

	wchar                       *add_enable_char_ptr;	/*!<������������ַ�֮��,��������������ַ���*/
    uint16                      add_enable_char_len;	/*!<������������ַ�֮��,��������������ַ���*/

    GUI_POINT_T                 prev_point;         //previous point

    BOOLEAN                     is_tp_moved;        /*!<�����Ƿ��ƶ�*/
    BOOLEAN                     is_set_skb;

#ifdef GUIF_CLIPBOARD
    BOOLEAN                     is_clipbrd_support; // �Ƿ�֧�ּ�����
    BOOLEAN                     is_show_clipbrd;    // �Ƿ���ʾ�˼�����
    MMI_HANDLE_T                clipbrd_win_handle; /*!<�����崰��handle*/
    uint32                      slider[MMITHEME_SLIDER_TYPE_MAX];   // ������
    GUIEDIT_DRAGSTATE_E         drag_state;                         // ��ǰ����״̬
    GUIEDIT_SLIDER_E            tpdown_slider;                      // TPDOWNʱ�ģ����������
    uint8                       slider_dis_timer_id;                // ������ʧ��ʱ��
#endif

    BOOLEAN                     is_tp_down;         //is tp down���Ƿ��£�����ǣ�����slide timer�ﲻ��������
    uint8                       slide_timer_id;     //slide timer id
    float                       fling_velocity;     //fling velocity
    GUI_POINT_T                 start_point;        //slide start point
    MMK_TP_SLIDE_E              slide_state;        //slide state
    MMK_TP_SLIDE_E              tpdown_slide_state; //tp down slide state

    GUIEDIT_WIN_INTERCEPT_CTRL_MSG  WinInterceptCtrlMsgFunc;/*!<�����Ƿ�ػ�ؼ���Ϣ,Ŀǰֻ�ܽػ�Cancel��Ϣ*/

    MMITHEME_BASEFLEX_T         flex_theme;
} CTRLBASEFLEX_OBJ_T;

// baseflex class for public or protected method
typedef struct
{
    CTRLBASEEDIT_VTBL_T            baseedit_ctrl_vtbl;

    /**************************************************************************/
    //  Description : ������ı����֪ͨ�ص�
    /**************************************************************************/
    void (*OnTextAdded)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
        GUIEDIT_ADD_STR_T       *add_str_ptr
        );

    /**************************************************************************/
    //  Description : ��ɾ���ı����֪ͨ�ص�
    /**************************************************************************/
    void (*OnTextDeleted)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : ��󳤶ȸı���֪ͨ�ص�
    /**************************************************************************/
    void (*OnMaxLenChanged)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : ����ַ����Ƿ�Ϸ�
    /**************************************************************************/
    BOOLEAN (*IsStringValid)(                               // [ret] TRUE if char is valid
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
        wchar                   *str_ptr,
        uint16                  str_len
        );

    /**************************************************************************/
    //  Description : ��ʾʣ������
    /**************************************************************************/
    void (*DisplayLeftNum)(
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : ��ȡʣ��������Ϣ
    /**************************************************************************/
    uint16 (*GetLeftNumInfo)(                           // [ret] the left char num
        CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,     // [in]
        uint16                  str_max_len,            // [in] the max size of char
        wchar                   *str_ptr,               // [out] left char num string
        uint16                  *str_len_ptr            // [out] length of string
        );

    /**************************************************************************/
    //  Description : ��鱻ɾ�ַ����˷�����ɾ��ʱ����
    /**************************************************************************/
    void (*CheckDelStr)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr
        );

    /**************************************************************************/
    //  Description : �����ӵ��ַ����˷����������ַ�ʱ����
    /**************************************************************************/
    BOOLEAN (*CheckCommitStr)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr,        //out
        uint16                      *delete_len_ptr         //out:
        );

    /**************************************************************************/
    //  Description : ������ַ�ǰ�����
    /**************************************************************************/
    void (*BeforeAddString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
        );

    /**************************************************************************/
    //  Description : ������ַ��������
    /**************************************************************************/
    void (*AfterAddString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr,     //in:
        GUIEDIT_ADD_STR_T           *add_str_ptr,           //in/out:
        GUIEDIT_ADD_ACTUAL_STR_T    *actual_str_ptr         //in
        );

    /**************************************************************************/
    //  Description : ��ɾ���ַ�ǰ�����
    /**************************************************************************/
    void (*BeforeDelString)(
        CTRLBASEFLEX_OBJ_T          *baseflex_ctrl_ptr      //in:
        );

    /**************************************************************************/
    //  Description : �ı�����
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
//  Description : ����ѡ����Ϣ
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
//  Description : �ı们��״̬
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ChangeDragState (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    GUIEDIT_DRAGSTATE_E     drag_state
);

/*****************************************************************************/
//  Description : ��ʾ����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_ShowSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : ��ʼ�����϶�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleBeginSlider (
    CTRLBASEFLEX_OBJ_T *baseflex_ctrl_ptr,
    DPARAM             param
);

/*****************************************************************************/
//  Description : �϶�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEFLEX_HandleMoveSlider (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr,
    DPARAM                  param
);

/*****************************************************************************/
//  Description : �����϶�����
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
//  Note: �����FreeEditReverseString���ʹ��,�����ڴ�й¶
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
//  Description : �ص��˷���λ��+���Լ�������ַ�
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEFLEX_InterceptPhoneNum (//��ȡ��ĳ���
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
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr, //in��
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
//  Description : ��ʾ��������
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
//  Note: ������Ҫ�󣬵�ǰ���뷨�����ı�������ͼ�꣬���Ա��������ܽ�������չ
/*****************************************************************************/
PUBLIC void BaseFlexCtrlDisplayLeftNum (
    CTRLBASEFLEX_OBJ_T      *baseflex_ctrl_ptr
);

/*****************************************************************************/
//  Description : ��ȡʣ��������Ϣ
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
//  Description : ������ı����֪ͨ�ص�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list/psw����̳�
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
//  Description : ��ɾ���ı����֪ͨ�ص�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list����̳�
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
//  Description : ��󳤶ȸı���֪ͨ�ص�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// text/psw����̳�
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
//  Description : ����ַ����Ƿ�Ϸ�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// �����ӿؼ���������
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
//  Description : ��ʾʣ������
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list��������⴦��
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
//  Description : ��ȡʣ��������Ϣ
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// text��������⴦��
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
//  Description : ��鱻ɾ�ַ�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// �˷�����ɾ��ʱ�õ�����Ϊ��ɾ������ʱ����ȷ����ǰ���ַ���ѡ�У���list���Ļ���
// ��Ҫ��֤item object��������ѡ��
// list������һ�����컯�Ķ���������������baseflex��
// text��������⴦��
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
//  Description : �����ӵ��ַ�
//  Global resource dependence : 
//  Author: hua.fang
//  Param:
// add_str_ptr: ������ַ�
// actual_str_ptr: ʵ�ʱ�¼����ַ������Բ����أ���ϵͳͳһ¼��
// delete_len_ptr: ��ɾ�����ַ���ֻ����is_del_after_full���ʱ��Ч
//  return:
// TRUE: commit���ַ��Ѿ�������actual_str_ptr;
// FALSE: �������ַ�����Ҫ��������
//  Note:
// �˷����������ַ�ʱ���ã�������ַ������У��ַ��ı���������⴦���еĻ���Ϊ
// һ����������������������������
// list��text������һ�����컯�Ķ���������������baseflex��
/*****************************************************************************/
static __inline BOOLEAN VTLBASEFLEX_CheckCommitStr(     //[ret] TRUE: commit���ַ��Ѿ�������actual_str_ptr; FALSE: �������ַ�����Ҫ��������
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
//  Description : ������ַ�ǰ�����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit��������
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
//  Description : ������ַ��������
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit��������
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
//  Description : ��ɾ���ַ�ǰ�����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
// list, edit��������
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
//  Description : �ı�����
//  Global resource dependence : 
//  Author: hua.fang
//  Param:
// phonenum�������أ���Ϊ�д�С����任
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
////  Description : ��ȡָ������������ɵ�����������ֵ
////  Global resource dependence : 
////  Author: hua.fang
////  Param:
//// password�������أ���Ϊ�����Ǻ�
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
