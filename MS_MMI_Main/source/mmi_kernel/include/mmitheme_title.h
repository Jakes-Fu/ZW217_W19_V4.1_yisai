/*****************************************************************************
** File Name:      mmitheme_title.h                                          *
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


#ifndef  _MMITHEME_TITLE_H_    
#define  _MMITHEME_TITLE_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guifont.h"
#include "guititle.h"
#include "guiwin.h"
#include "ui_layer.h"
#include "mmitheme_pos.h"
#include "mmitheme_update.h"
#include "mmi_default.h"

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


/*! \brief title style */
typedef struct 
{
    MMI_IMAGE_ID_T			bg_image;					/*!< background image */
    GUI_COLOR_T				bg_color;					/*!< background color */

    // Ä¬ÈÏ×Ö·û´®ÐÅÏ¢
	GUI_FONT_T				font;						/*!< font */
    GUI_COLOR_T				font_color;					/*!< font color */
	uint16					char_space;					/*!< space between chars */
    GUI_ALIGN_E             align_type; 
    
    // button
    BOOLEAN                 is_need_button;
	MMI_IMAGE_ID_T			left_button_icon_down;
    MMI_IMAGE_ID_T			left_button_icon_up;
    MMI_IMAGE_ID_T          left_button_icon_disable;
	MMI_IMAGE_ID_T			right_button_icon_down;
    MMI_IMAGE_ID_T			right_button_icon_up;
    MMI_IMAGE_ID_T          right_button_icon_disable;
    uint16                  button_margin;
    
    MMI_TEXT_ID_T           left_button_text_id;
    MMI_TEXT_ID_T           right_button_text_id;
    GUI_FONT_T              button_font_type;
    MMI_COLOR_ID_T          button_font_color;

    // ×Ó×Ö·û´®ÐÅÏ¢
    GUI_FONT_T				sub_font;
    MMI_COLOR_ID_T          sub_font_color;

    // ×Ö·û´®¹ö¶¯
    uint32                  scroll_time;
    uint32                  scroll_pixel_step;
    uint16                  title_text_margin; 
    uint16                  icon_space;
}MMI_TITLE_STYLE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get the title style, is called by the title control
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTitleStyle(
                                        MMI_TITLE_STYLE_T* title_style_ptr
	                                    );
/*****************************************************************************/
// 	Description : get the window style
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetWinTitleMaxLen(void);


/*****************************************************************************/
// 	Description : get title default ctrl id
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetTitleCtrlId(void);

/*****************************************************************************/
// 	Description : get the title bar
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetWinTitleBar(void);

#if defined (GUIF_TITLE_PDASTYLE)
/*****************************************************************************/
// 	Description : get the Theme title height
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeTitleHeight_V(void);

/*****************************************************************************/
// 	Description : get the Theme title height Horizontal
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetThemeTitleHeight_H(void);
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_TITLE_H_

