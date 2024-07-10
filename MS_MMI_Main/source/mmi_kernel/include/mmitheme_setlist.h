/******************************************************************************
** File Name:      mmitheme_setlist.h                                         *
** Author:         xiaoqing.lu                                                *
** Date:           11/04/2009                                                 *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct of           *
**                              guistatusbar control                          *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 04/2009        Xiaoqing.lu      Create			                          *
******************************************************************************/

#ifndef _MMITHEME_SETLIST_H_
#define _MMITHEME_SETLIST_H_ 

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guistring.h"
#include "guilistbox.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define MMISETLIST_ICONANIM_TIMEOUT                200 // ��������ʱ��ʱ����
#define MMISETLIST_ITEM_MAX                        32  // Ĭ�����10��item
#define MMISETLIST_TEXT_MARGIN                     4   // Ĭ�����10��item


typedef enum
{
    MMI_SETLIST_NONE, // ������

    MMI_SETLIST_ALLOC,
    MMI_SETLIST_APPEND_ITEM,        // append list item
    MMI_SETLIST_SET_ITEM_STYLE,     // set list item style
    MMI_SETLIST_GET_CUR_RECT,       // ���setlist����
    MMI_SETLIST_GET_TITLE_STRLEN,   // ��ȡsetlist title�ַ����ĳ���
    MMI_SETLIST_GET_TITLE_STRDATA,  // ��ȡsetlist title �ַ���
    MMI_SETLIST_IS_OPEN_FULL_WIN,   // �Ƿ��ȫ������

    MMI_SETLIST_MAX
}MMI_SETLIST_CALL_TYPE_E;

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

// ������ʾ������ö�ټ�˵���˲���Ҳ˵����item����ʾ������
typedef enum
{
    GUISETLIST_LAYOUT_NONE,         // 
    GUISETLIST_LAYOUT_TEXT,         // ֻ���ı���ʱ��,�ı���ʾ��alignȷ��
    GUISETLIST_LAYOUT_IMAGE_LEFT,   // ֻ��ͼƬ,ͼƬ��ʾ�����,���¾���
    GUISETLIST_LAYOUT_IMAGE_RIGHT,  // ֻ��ͼƬ,ͼƬ��ʾ���Ҳ�,���¾���
    GUISETLIST_LAYOUT_IMAGE_MIDDLE, // ֻ��ͼƬ,ͼƬ��ʾ���м�,���¾���
    GUISETLIST_LAYOUT_IMAGE_TEXT,   // ͼƬ��ǰ,�ı��ں�,Ĭ��ֵ��ͼƬ����,�ı���ʾ��alignȷ��
    GUISETLIST_LAYOUT_TEXT_IMAGE,   // �ı���ǰ,ͼƬ�ں�,�ı���ʾ��alignȷ��
    GUISETLIST_LAYOUT_MAX
}MMI_GUISETLIST_LAYOUT_TYPE_E;

// ���屳��������
typedef enum
{
    GUISETLIST_BG_NONE,
    GUISETLIST_BG_IMAGE,
    GUISETLIST_BG_COLOR,
    GUISETLIST_BG_MAX
}MMI_GUISETLIST_BG_TYPE_E;

// setting list��Ĭ����Ϣ
typedef struct
{
    // ���ַ��
    int16               char_space;  // char space 
    GUI_FONT_T          font_type;   // font type
    GUI_COLOR_T         font_color;  // font color
    GUI_ALIGN_E         align;       // font align
    GUISTR_STATE_T      str_state;   // string state
    GUIFONT_EFFECT_T    effect;     //string effect
    
    uint16              left_margin;
    uint16              right_margin;

    // ����������
    GUI_BG_T            bg_info;
    uint32              timer_out;   // timer out
    MMI_GUISETLIST_LAYOUT_TYPE_E layout;

    // �߿�,ֻ�пؼ�������GUISETLIST_STATE_FRAME״̬֮��Ż���ʾ�߿�
    GUI_BORDER_T        border; // �߿�

    // ��ͷͼ��ID
	MMI_IMAGE_ID_T      left_arrow_icon_down;
    MMI_IMAGE_ID_T      left_arrow_icon_up;
    MMI_IMAGE_ID_T      left_arrow_icon_gray;
	MMI_IMAGE_ID_T      right_arrow_icon_down;
    MMI_IMAGE_ID_T      right_arrow_icon_up;
    MMI_IMAGE_ID_T      right_arrow_icon_gray;

}MMI_GUISETLIST_STYLE_T;

// ��setlist������Ϣ
typedef struct  
{
    MMI_HANDLE_T parant_win_handle;
    MMI_HANDLE_T parant_ctrl_handle;
    GUILIST_TYPE_E list_type;
    uint16 cur_item_index;
    GUI_FONT_T list_font;
    GUI_COLOR_T font_color;
    GUI_COLOR_T title_font_color;
    GUI_BOTH_RECT_T win_both_rect;
    GUI_BOTH_RECT_T button_both_rect;
    uint32 check_allow_max_num;
}MMI_OPEN_SETLIST_WIN_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the setting list style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSetlistStyle(
                                        MMI_GUISETLIST_STYLE_T  *style_ptr
                                        );

/*****************************************************************************/
//  Description : ����Ĭ�ϵ����item��ֵ
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetSetMaxItemNum(void);

/*****************************************************************************/
//  Description : ����ʾ������ʱ��,��ȡ������ʾ��Ĭ��ͼƬ
//  Global resource dependence : 
//  Author: 
//  Note: ����������������߽���֮ǰ��ʾ��ͼƬ
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetDefaultItemIcon(void);

/*****************************************************************************/
//  Description : ����ʾ������ʱ��,��ȡ������ʾ��Ĭ��ͼƬ
//  Global resource dependence : 
//  Author: 
//  Note: ����������������߽���֮ǰ��ʾ��ͼƬ
/*****************************************************************************/
PUBLIC void MMITHEME_SetDefaultItemIcon(
                                        MMI_IMAGE_ID_T icon_id
                                        );

/*****************************************************************************/
//  Description : ��setlist����
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CreateSetlistChildWin(
                                           MMI_OPEN_SETLIST_WIN_T *data_ptr
                                           );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_SETLIST_H_
/*! @} */
