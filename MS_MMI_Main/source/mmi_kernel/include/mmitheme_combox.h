/******************************************************************************
** File Name:      mmitheme_combox.h                                         *
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

/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/
//the dropdownlist display style 
typedef struct
{
	GUI_BORDER_T   border;
	GUI_COLOR_T    font_color;            //������ɫ
	GUI_FONT_T     font;                  //����
	uint8          font_space;            //�ּ��
	BOOLEAN        is_ellipsis;

	uint16         item_height;           //ÿ��Item�ĸ߶�
	uint16         default_item_num;      //ÿ��Item�ĸ߶�       
	uint8          left_space;            //�ַ�����
	MMI_IMAGE_ID_T    unpressed_arrow_icon;   //δ���µ�������ͷͼ��
	MMI_IMAGE_ID_T    pressed_arrow_icon;     //�Ѱ��µ�������ͷͼ��
	
	GUI_BG_T      bg_info;
	GUI_BG_T      list_bg_info;
	
	BOOLEAN       has_icon;
	BOOLEAN       has_expand_button;
	BOOLEAN       has_function_button;
	BOOLEAN       is_list_expand;
}MMI_COMBOX_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Get ComBox Style
//  Global resource dependence : 
//  Author: nan & xiyuan
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetComBoxStyle(
									   MMI_COMBOX_STYLE_T* style_ptr
									   );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _MMITHEME_SETLIST_H_
/*! @} */
