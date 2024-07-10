/*****************************************************************************
** File Name:      mmitheme_richtext.h                                       *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create								     *
**																		     *
*****************************************************************************/

#ifndef  _MMITHEME_RICHTEXT_H_    
#define  _MMITHEME_RICHTEXT_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guititle.h"
#include "guiedit.h"
#include "guiwin.h"
#include "ui_layer.h"
#include "mmitheme_pos.h"
#include "mmitheme_update.h"
#include "mmi_default.h"
#include "mmitheme_clipbrd.h"

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

//richtext 
typedef struct
{
    MMI_IMAGE_ID_T  bg_image_id;        //����ͼƬ
    
    MMI_IMAGE_ID_T  anim_parsing_icon_id;
    MMI_IMAGE_ID_T  anim_error_icon_id;
    
    GUI_FONT_T      font;               //����
    
    GUI_COLOR_T     bg_color;
    GUI_COLOR_T     font_color;
    GUI_COLOR_T     focusline_color;    
    GUI_COLOR_T     focus_bg_color;    
    GUI_COLOR_T     border_color;
    GUI_COLOR_T     border_focus_color;

	uint8			item_space;			//Item��ֱ����ļ��
	uint8			border_space;	    //Item�����ұ߿��ļ��
	uint8			char_space;			//�ı��������ַ��ļ��
	uint8			text_line_space;	//�����ı���ļ��
	uint8           img_text_space; 
    uint8           txt_margin;

    MMITHEME_DRAGMODE_E         drag_mode;              // �������ģʽ
    uint32                      slider_display_period;  // ������ʾʱ��
}MMI_RICHTEXT_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get the richtext style, is called by richtext control
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetRichTextStyle(
                                         MMI_RICHTEXT_STYLE_T  * richtext_style_ptr
                                         );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_RICHTEXT_H_

