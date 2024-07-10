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
//ÿ���������ʾ�Ľ���Ԫ����Ŀ�����ھֲ������������С
#define RICHTEXT_TAG_STRING_MAX_PER_LINE (10)
#define RICHTEXT_TEXT_MAX_LINE (GUIRICHTEXT_TEXT_MAX_LEN)
#define RICHTEXT_BORAD_WIDTH (DP2PX_VALUE(3)) //���ֵ��ϵͳԲ�ǿ�Ŀ�Ⱦ�����
#define RICHTEXT_DEFAULT_ITEM_BORDER (RICHTEXT_BORAD_WIDTH)
#define RICHTEXT_SLIDE_FLING_TICK (100)
#define RICHTEXT_SLIDE_RECOVER_TICK (5)
#define RICHTEXT_SLIDE_RECOVER_MIN (20)

#ifdef GUIF_CLIPBOARD
#define RICHTEXT_DRAG_SELECT_TICK (120)
#endif

/*! @def GUIRICHTEXT_LINE_LEN_MAX
@brief ������ʾ���ֵ�������
*/
#define GUIRICHTEXT_LINE_LEN_MAX (128)

/*! @def GetContHeight
@brief ��ȡ�ؼ���ʾ����ĸ߶�
*/
#define GetContHeight(ptr) ((ptr)->cont_rect.bottom - (ptr)->cont_rect.top + 1)
/*! @def GetContWidth
@brief ��ȡ�ؼ���ʾ����Ŀ��
*/
#define GetContWidth(ptr) ((ptr)->cont_rect.right - (ptr)->cont_rect.left + 1)
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// ��ǰ�����ĸ��϶�״̬
typedef enum
{
    GUIRICHTEXT_DRAGSTATE_NORMAL,       // ��ͨ�Ĺ��״̬
    GUIRICHTEXT_DRAGSTATE_SELECT,       // ѡ���ı�״̬
    GUIRICHTEXT_DRAGSTATE_MAX
} GUIRICHTEXT_DRAGSTATE_E;

// �����ĸ�������
typedef enum
{
    GUIRICHTEXT_SLIDER_NONE,            // û�л��飬��ʱӦ�ô���GUIRICHTEXT_DRAGSTATE_NORMAL״̬
    GUIRICHTEXT_SLIDER_LEFT_SELECTOR,   // ��ѡ��飬��ʱӦ�ô���GUIRICHTEXT_DRAGSTATE_SELECT
    GUIRICHTEXT_SLIDER_RIGHT_SELECTOR,  // ��ѡ��飬��ʱӦ�ô���GUIRICHTEXT_DRAGSTATE_SELECT
} GUIRICHTEXT_SLIDER_E;

//1 extern item data,��������ݶ��壬��������ͼƬ�����ֵ�һЩ��ϸ��Ϣ
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

//1.3 ����Ԫ�ص���ʾ״̬
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
    
//2 ITEM����չ����
//2.1 ��չ�ֶζ���
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

    //��ʱ����ͷָ��
    RICHTEXT_FOCUS_ELEMENT_NODE_T *focus_header_ptr;
    //��ʱ����βָ��
    RICHTEXT_FOCUS_ELEMENT_NODE_T *focus_tailer_ptr;
    //�洢����Ԫ�ص�����ָ�룬�Ŀؼ��Ƕ�̬�����
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
    int16   item_index;     // item����
    int16   line_index;     // ����item��������
    int16   pos_index;      // �������λ������
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
@brief ͼƬ����ʾ״̬����ͼƬ�첽��ʾʱ����Ҫ��¼��ʾ״̬
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

    MMI_IMAGE_ID_T bg_image_id; /*!< �ؼ��ı���ͼƬ */
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

    //item��������
    RICHTEXT_ITEM_T *first_item_ptr;

    //��ʾ����
    BOOLEAN need_cal_ext_data;
    BOOLEAN need_cal_rect;
    int32 disp_height;
    int32 disp_top;
    GUI_ALIGN_E disp_align;

    //����
    //ֻ֧��resͼƬ����Ϊ�����첽��ʾ
    MMI_IMAGE_ID_T bg_img_id;
    GUI_COLOR_T bg_color;
    
    //����
    GUIRICHTEXT_FOCUS_SHIFT_E focus_mode;
    RICHTEXT_ITEM_T *focus_item_ptr;

    //ͼƬ�Ƿ�ͬ������
    BOOLEAN img_is_syn_decode;
    //��ʾͼƬ���м����
    RICHTEXT_ITEM_T *img_appending_item_ptr;
    //����������ʾͼƬ��ʱ����ʾ��ͼ��
    MMI_IMAGE_ID_T anim_parsing_icon_id;
    //������ʾͼƬ�����ʱ����ʾ��ͼ��
    MMI_IMAGE_ID_T anim_error_icon_id;

    //��ʾ��صĲ���
    GUIRICHTEXT_FRAME_TYPE_E border_type;
    GUI_COLOR_T border_color;
    GUI_COLOR_T border_focus_color;

    GUI_FONT_T font;
    GUI_COLOR_T font_color;

    GUI_COLOR_T focusline_color;
    GUI_COLOR_T focus_bg_color;

    uint8 border_space; //����richtext���ı߿հ׾��룬����б߿�Ļ�����ֵ����Ϊ1
    uint8 item_border_space; //����item�ı߿���ռ�ľ��룬���������item�߿�Ļ�������Ҫ��Ӧ�����ã���text&image����������item
    uint8 char_space; //text���ּ�ľ���
    uint8 line_space; //text�м��
    uint8 item_space; //����item֮��ľ���
    uint8 img_text_space; //ͼƬ���ı�֮��ľ���
    uint8 txt_margin; //���ֵı߾࣬�ر�������item�߿�����׿��ܺÿ�һЩ

    //�绰������ʱ�洢��
    wchar edit_phonenumber[GUIURL_PHONE_NUMBER_LEN_MAX+1];

    GUI_POINT_T tp_start_point; //���ʵľ���λ��
    GUI_POINT_T tp_stop_point;

    RICHTEXT_TPMODE_T tp_mode;
    uint8 slide_timer;
    float slide_v;

    BOOLEAN is_info_protected;

    //select & copy
#ifdef GUIF_CLIPBOARD
    BOOLEAN is_clipboard_support; //�Ƿ�֧�ּ��а�
    BOOLEAN is_select_data_dirty;  //���е������Ƿ���Ҫ�ع�-����tp move��ctrl�ؽ�
    BOOLEAN is_line_data_dirty; //����Ϣ�Ƿ���Ҫ�ع�-������ת��Ļ
    uint16 select_len; //ѡ����ȫ������֮��
    uint16 select_item_cnt; //ѡ����item����
    RICHTEXT_SELECT_STATE_T select_state;
    uint8 drag_select_timer;
    //���ʵ�����λ��
    int32 v_start_x;
    int32 v_start_y;
    int32 v_stop_x;
    int32 v_stop_y;
    MMI_HANDLE_T clipbrd_win_handle; //clipbrd win handle

    uint32                      slider[MMITHEME_SLIDER_TYPE_MAX];   // ������
    GUIRICHTEXT_DRAGSTATE_E     drag_state;                         // ��ǰ����״̬
    GUIRICHTEXT_SLIDER_E        tpdown_slider;                      // TPDOWNʱ�ģ����������
    uint8                       slider_dis_timer_id;                // ������ʧ��ʱ��
    MMITHEME_DRAGMODE_E         drag_mode;                          // �������ģʽ
    uint32                      slider_display_period;              // ������ʾʱ��

    RICHTEXT_SEL_INFO_T         start_info;                         // ������ʼ��Ϣ
    RICHTEXT_SEL_INFO_T         stop_info;                          // ����������Ϣ
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
