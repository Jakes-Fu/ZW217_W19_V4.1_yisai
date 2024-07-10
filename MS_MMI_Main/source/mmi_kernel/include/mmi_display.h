/*****************************************************************************
** File Name:      mmi_display.h                                               *
** Author:                                                                   *
** Date:           20/12/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe gui common data struct      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2004        Great.Tian          Create                                   *
******************************************************************************/
/*!
 *  \addtogroup mmi_display_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_display.h
 *  \author	Great.Tian
 *  \date	Dec,2004
 *  \brief 	This file is used to describe the mmi display attribute
 */

#ifndef _MMI_DISPLAY_H_
#define _MMI_DISPLAY_H_

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include    "guicommon.h"


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
typedef enum
{
	MMI_INPUT_TIME_WINDOW,							/*!< time£¬week£¬date input window */	
	MMI_INPUT_DIGITAL_WINDOW,						/*!< password and number input window */
	TOTAL_INPUT_WINDOW_NUMBER						/*!< the max window number which data input  */
} MMI_MAIN_MENU_INDEX_E;							/*!< window type which data input */

/*! \brief IDLE screen display the font coordinate ,color,size */
typedef struct
{
    uint8						text_frame_top;		/*!< text display area for y coordinate */
    GUI_COLOR_T					text_color;			/*!< color  */
    GUI_FONT_T					text_font;			/*!< size */
}MMI_IDLE_DISPLAY_T;	

/*! \brief 2LEVEL MENU DISPLAY ABOUT GROUP */
typedef struct
{
    uint16						menu_group;			/*!< menu group */
    uint16						title_background;	/*!< title back ground */
}MMI_2LEVEL_MENU_ABOUT_GROUP_T;

/*! \brief The data structure for defining the list window */
typedef struct
{

    uint8						title_background_heigh;		/*!< list window title height */
    uint8						title_content_interval;		/*!< The interval of two level menu from title and content */
    GUI_COLOR_T					title_background_color;     /*!< list window title background color */
    GUI_COLOR_T					title_text_color;			/*!<  The title text color of list window */
    GUI_FONT_T					title_text_font;			/*!<  The title text font for  list window*/
    uint8						left_icon_frame_width;      /*!<The width of left icon for  list window */
    uint8						list_item_height;			/*!< The height of every list item for  list window */
    GUI_COLOR_T					list_item_text_color;		/*!<The item text color for  list window */
    GUI_FONT_T					list_item_text_font;		/*!<The item text font for  list window  */
    uint8						right_icon_frame_width;     /*!<The width of right item icon  for  list window */
    GUI_COLOR_T					content_background_color;	/*!<The list content background color for  list window É« */
}MMI_LIST_WINDOW_DISPLAY_T;

/*! \brief The data structure for defining the input data window */
typedef struct
{   
    uint8						title_background_heigh;									/*!< The title background icon height of data input window */
    GUI_COLOR_T					title_background_color;									/*!< The title background color of data input window */
    GUI_COLOR_T					title_text_color;										/*!<The title text color   of data input window*/
    GUI_FONT_T					title_text_font;										/*!< The title text font of data input window */
    GUI_POINT_T					icon_start;												/*!<The icon begin position of data input window  */    
    GUI_POINT_T					picture_start;											/*!<The picture begin position of data input window  */
     
    GUI_RECT_T					editbox_rect[TOTAL_INPUT_WINDOW_NUMBER];				/*!<The edit box size of data input window  */ 
    GUI_COLOR_T					menu_item_editbox_color[TOTAL_INPUT_WINDOW_NUMBER];		/*!<The edit box text color of data input window  */
    GUI_FONT_T					menu_item_editbox_font[TOTAL_INPUT_WINDOW_NUMBER];		/*!<The edit box text font of data input window  */
    
    GUI_RECT_T					textbox_rect[TOTAL_INPUT_WINDOW_NUMBER-1];				/*!<The textbox size of data input window ,for password and number there is no textbox so distract 1 */ 
    GUI_COLOR_T					menu_item_textbox_color[TOTAL_INPUT_WINDOW_NUMBER-1] ;	/*!<The textbox text color of data input window  */
    GUI_FONT_T					menu_item_textbox_font[TOTAL_INPUT_WINDOW_NUMBER-1] ;	/*!<The textbox text font of data input window  */
    GUI_COLOR_T					content_background_color;								/*!<The  background  color of data input window  */
}MMI_INPUT_WINDOW_DISPLAY_T;


/*! \brief The data structure of special display  window */
typedef struct
{
	GUI_RECT_T					background_anim_rect;	/*!< aninate background dispaly position */
    uint8						text_frame1_top;		/*!< text_frame1 top */
    uint8						text_frame2_top;		/*!< text_frame2 top */
    GUI_COLOR_T					title_text_color;		/*!< title text color */  
    GUI_FONT_T					title_text_font;		/*!< title text font */
}MMI_EXCEPTIVE_WINDOW_DISPLAY_T;

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  

/*! @} */
     
