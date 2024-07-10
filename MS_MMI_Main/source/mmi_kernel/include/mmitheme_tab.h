/*****************************************************************************
** File Name:      mmi_theme.h                                               *
** Author:         Great.Tian                                                *
** Date:           06/21/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004        Great.Tian       Create								     *
**																		     *
*****************************************************************************/

#ifndef  _MMITHEME_TAB_H_    
#define  _MMITHEME_TAB_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"

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


/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
// default tab hight
#define MMITAB_DEFAULT_ITEM_HEIGHT			            MMI_TAB_ITEM_DEFAULT_HEIGHT

// default tab width
#define MMITAB_DEFAULT_ITEM_WIDTH			            MMI_TAB_ITEM_DEFAULT_WIDTH

// default space value between two items
#define GUITAB_DEFAULT_ITEM_SPACE			            5

// timeout interval of timer
#define GUITAB_DEFAULT_TIMEOUT				            200

// item��ʾ��������ڵױߵ�space
#define MMITAB_DEFAULT_ITEM_TEXT_BOTTOM_SPACE			MMI_TAB_ITEM_TEXT_BOTTOM_SPACE

// TAB style
typedef struct
{
	BOOLEAN				is_display_prg;	                    //�Ƿ���ʾ������
    uint8				font_space;                         //�ּ��
	MMI_IMAGE_ID_T		prg_scroll_id;	                    //������ͼƬ
	MMI_IMAGE_ID_T		bg_image_id;                        //����ͼƬ 
    GUI_COLOR_T			bg_color;                           //������ɫ 
	MMI_IMAGE_ID_T		left_scroll_pressed_image_id;       //���pressedͼƬ 
	MMI_IMAGE_ID_T		left_scroll_released_image_id;      //���releasedͼƬ
	MMI_IMAGE_ID_T		left_scroll_disabled_image_id;      //���disabledͼƬ 
	MMI_IMAGE_ID_T		right_scroll_pressed_image_id;      //�ҹ�pressedͼƬ  
	MMI_IMAGE_ID_T		right_scroll_released_image_id;     //�ҹ�releasedͼƬ
	MMI_IMAGE_ID_T		right_scroll_disabled_image_id;     //�ҹ�disabledͼƬ 
    GUI_COLOR_T			sel_item_color;                     //ѡ��tab����ɫ
	GUI_COLOR_T			unsel_item_color;                   //δѡ��tab����ɫ
    GUI_COLOR_T         focus_rect_color;                   // ������Ҫ����item��ʾͼƬʱ��ʹ�ø���ɫ�������
    GUI_COLOR_T			font_color;                         //������ɫ
	GUI_COLOR_T			sel_font_color;                     //ѡ��������ɫ
    GUI_FONT_T			font;                               //��������
    uint32				item_default_width;                 // ��itemֻ��ʾͼƬʱ��item�Ŀ�ȣ����ֵС�ڵ���item���ܿ��
    uint32			    item_default_height;                // ��itemֻ��ʾͼƬʱ��item�Ŀ�ȣ����ֵС�ڵ���item���ܿ��
    MMI_IMAGE_ID_T      item_deficon_id;                    // ��֧�ֶ���ͼ���ʱ��,����������ɹ�,������Ĭ�ϵ���ʾͼ��
    MMI_IMAGE_ID_T      item_error_id;                      // ��֧�ֶ���ͼ���ʱ��,����������ɹ�,������Ĭ�ϵ���ʾͼ��
    uint32              scroll_card_def_width;              // card Ĭ�Ͽ��
    GUI_BG_T            height_bg_info;
	GUI_RECT_T          height_bg_margin;                   // ���������ĸ�����ı߾�
    GUI_RECT_T          text_margin;                        // ���������ĸ�����ı߾�
    BOOLEAN             is_show_cur_item_only;              // ֻ��ʾ��ǰ��item
	uint16				prg_height;	                        //�������߶�
	uint16				default_item_num;	                //Ĭ��һҳ��ʾ���item
	uint32				item_state;	                        //GUITAB_ITEM_STATE_T:item��ʼ״̬
	MMI_IMAGE_ID_T		tp_sel_img_id;	                    //tp downѡ��ͼƬ
	MMI_IMAGE_ID_T		dividing_line;	                    //tab�ָ���
} MMI_TAB_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get the tab style, is called by tab control
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTabStyle(MMI_TAB_STYLE_T  * tab_style_ptr);

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabChildWinTop(
                                         MMI_HANDLE_T win_handle
                                         );

/*****************************************************************************/
// 	Description : get the tab child win top
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabHeight(
                                    void
                                    );

/*****************************************************************************/
// 	Description : get the tab default width
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefWidth(void);

/*****************************************************************************/
// 	Description : get the tab default display item num
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetTabDefDispItemNum(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _MMITHEME_TAB_H_  */
/*! @} */
