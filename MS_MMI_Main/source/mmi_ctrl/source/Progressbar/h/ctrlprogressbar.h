/*****************************************************************************
** File Name:      ctrlprogressbar.h                                   *
** Author:                                                                   *
** Date:              07/23/2012                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                                                                                          *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                                      *
** 07/2012     xiyuan.ma              Creat
******************************************************************************/

#ifndef _CTRLPROGRESSBAR_H_
#define _CTRLPROGRESSBAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "cafcontrol.h"

#include "ctrlbase.h"
#include "ctrlprogressbar_export.h"
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
//progressbar type info
#define CTRL_PROGRESSBAR_TYPE (CTRLPROGRESSBAR_GetType())
#define RECT_PROGRESSBAR_HEIGHT     (15)
#if defined (MAINLCD_SIZE_128X160)
#define RECT_PROGRESSBAR_WIDTH		(128)
#elif   defined (MAINLCD_SIZE_240X284)
#define RECT_PROGRESSBAR_WIDTH		(240)
#elif   defined (MAINLCD_SIZE_240X240)
#define RECT_PROGRESSBAR_WIDTH		(240)
#elif   defined (MAINLCD_SIZE_240X320)
#define RECT_PROGRESSBAR_WIDTH		(240)
#else
#define RECT_PROGRESSBAR_WIDTH		(240)
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//progressbar init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T      base_ctrl_param;

    BOOLEAN                                is_forbid_paint;/*!< �Ƿ�ˢ�� */
    uint32                                     lcd_id;         /*!< lcd_id */
    GUIPRGBOX_STYLE_E             style;          /*!< ������ģʽ */
    MMITHEME_PRGBOX_TYPE_E  theme_type;     /*!< ���������� */
} CTRLPROGRESSBAR_INIT_PARAM_T;

//struct CTRLTIPS_CTRL_T
typedef struct _progressbar_obj_tag
{
    CTRLBASE_OBJ_T                  base_ctrl;

    BOOLEAN                         is_forbid_paint;    /*!< �������Ƿ�ˢ�� */
    CAF_LCD_DEV_INFO                disp_layer;         /*!< ��������ʾ�Ĳ� */
#if defined(GUIF_PRGBOX_AUTOHIDE)
    BOOLEAN                         is_active;          /*!< ��ʽ��¼����״̬�������ڷǼ���״̬�Ĳ��� */
    BOOLEAN                         is_fobid_thumb;          /*!< ��ʽ��¼����״̬�������ڷǼ���״̬�Ĳ��� */
    uint8                           remove_timer_id;    /*!< ���ع�������timer */
    int16                           last_tp_pos;        /*!< ���һ����Ч�ƶ��� */
    int16                           last_tp_offset;     /*!< ���һ����Ч����top����Ϊ��Ϣ��post����ʵʱ���� */
    uint32                          out_state;          /*!< ������״̬ */
    uint16                          thumb_width;        /*!< ����Ŀ�Ϊ�˱����ظ����㣬��¼���� */
    uint16                          thumb_height;       /*!< �����ĸߣ�ͬ�� */
#endif

    uint32                          cur_item_index;     /*!< ��ǰitem��ţ���"0"��ʼ */ 
    uint32							first_item_index;   /*!< ��һ��item��ţ���"0"��ʼ */ 
    uint32                          total_item_num;     /*!< ����item���� */ 
	int16                          *letter_postion_table;     /*<ÿ����ĸ��y����>*/
    uint32                          items_per_page;     /*!< ÿҳitem���� */ 

    GUIPRGBOX_STYLE_E               prgbox_style;       /*!< ��������ʾ��ʽ */ 
    MMITHEME_PRGBOX_TYPE_E    theme_type;         /*!< ����ģʽ */  
    GUI_RECT_T                      pre_arrow_rect;     /*!< ǰһ����ͷ����(��Դ���) */  
    GUI_RECT_T                      next_arrow_rect;    /*!< ��һ����ͷ����(��Դ���) */ 
    GUI_RECT_T                      pre_groove_rect;    /*!< ǰһ������������(��Դ���) */ 
    GUI_RECT_T                      next_groove_rect;   /*!< ��һ������������(��Դ���) */ 
    GUI_RECT_T                      scroll_bar_rect;    /*!< ��������������(��Դ���) */ 
    
    MMITHEME_PRGBOX_VER_SCROLL_T     ver_scroll_theme;   /*!< ������������� */ 
    MMITHEME_PRGBOX_HOR_SCROLL_T    hor_scroll_theme;   /*!< ������������� */ 
    MMITHEME_PRGBOX_PROCESS_T          process_theme;      /*!< ���������� */ 
    
    GUI_POINT_T                     pre_tp_point;       /*!< ��ǰ���ʴ��� */  
    uint32                          point_offset;       /*!< ����bar�ڵ�ƫ��λ�� */  
    BOOLEAN                         is_move_valid;      /*!< �Ƿ���ƶ� */
    BOOLEAN                         is_loop_scroll;     /*!< �Ƿ�ѭ�������� */  
    
    BOOLEAN                         is_need_slide_block;/*!< �Ƿ���Ҫ���� */ 
    BOOLEAN                         is_need_bg;         /*!< �Ƿ�Ҫ���� */ 
    BOOLEAN                         is_letter_bar_valid;/*!< ��ĸ�������Ƿ���Ч������ʾ�ռ�Ƚ�С���������Ч���Ҳ���ʾ*/ 
    uint16                          *base_letters_table;/*<������ĸ��>*/
    uint16                          letters_table_len;  /*<������ĸ��ĳ���>*/
    MMI_WIN_ID_T win_id;                                /*!< ����id */ 

    uint32                          top_limit;    // for wide progress bar adjustable region.
    uint32                          bottom_limit;
    
	GUI_RECT_T                      label_rect;//�ı���ʾ����
	GUI_RECT_T                      fg_rect;//ǰ����ʾ����
	BOOLEAN							is_waiting_indicator;
    MMI_STRING_T                    *rect_progress_title; 
    // GUIANIM_CTRL_INFO_T             *waiting_ind_animctrl_handle;
} CTRLPROGRESSBAR_OBJ_T;

//progressbar class for public or protected method
typedef struct _progressbar_vtbl_tag
{
    CTRLBASE_VTBL_T                       base_ctrl_vtbl;
} CTRLPROGRESSBAR_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*********************************************************************/
//  Description : get progressbar type
//  Global resource dependence :
//  Author: xiyuan.ma
//  Note:
/*********************************************************************/
PUBLIC TYPE_TAG_T CTRLPROGRESSBAR_GetType();

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_CTRLPROGRESSBAR__H_

