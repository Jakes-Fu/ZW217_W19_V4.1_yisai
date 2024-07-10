/*****************************************************************************
** File Name:      mmidc_id.h                                                *
** Author:                                                                   *
** Date:           2006/09/06                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe dc win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/06     gang.tong        Create                                    *
******************************************************************************/

#ifndef _MMIDC_CAMERA_ID_H_
#define _MMIDC_CAMERA_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    DC_WIN_ID_START = (MMI_MODULE_CAMERA << 16),

#include "mmidc_camera_id.def"

    DC_MAX_WIN_ID
}DC_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    DC_CTRL_ID_ID_START = DC_MAX_WIN_ID,//(MMI_MODULE_CAMERA << 16),

    MMIDC_STORAGE_OPT_LISTBOX_CTRL_ID,
#ifdef MMIDC_MINI_RESOURCE_SIZE    
    MMIDC_MODE_OPT_LISTBOX_CTRL_ID,
    MMIDC_EFFECT_OPT_LISTBOX_CTRL_ID,
#endif 
#if defined MMIDC_ADD_SPECIAL_EFFECT
		MMIDC_WHITE_BALANCE_OPT_LISTBOX_CTRL_ID,
		
		MMIDC_CONTRAST_SET_SETLIST_CTRL_ID,
		MMIDC_BRIGHTNESS_SET_SETLIST_CTRL_ID,

		MMIDC_CONTRAST_UP_BUTTON_CTRL_ID,
		MMIDC_CONTRAST_DOWN_BUTTON_CTRL_ID,
		MMIDC_BRIGHTNESS_UP_BUTTON_CTRL_ID,
		MMIDC_BRIGHTNESS_DOWN_BUTTON_CTRL_ID,
		
#endif   
    MMIDC_PHOTO_REVIEW_OPTION_MENU_CTRL_ID,

    MMIDC_SEND_MENU_CTRL_ID,
    MMIDC_HELP_TEXT_CTRL_ID,

    MMIDC_SETTING_LIST_CTRL_ID,
    
    MMIDC_PDA_CTRL_ID_START,
    MMIDC_PDA_BUTTON_CAPTURE_CTRL_ID = MMIDC_PDA_CTRL_ID_START,   
    MMIDC_PDA_BUTTON_SENSOR_CTRL_ID,
    MMIDC_PDA_BUTTON_ZOOM_CTRL_ID, 
#if defined (MMIDC_F_U_CAMERA)
    MMIDC_PDA_BUTTON_UCAMERA_CTRL_ID,
#endif
#ifdef PIC_VIEWER_SUPPORT
    MMIDC_PDA_BUTTON_PIC_CTRL_ID, 
#endif
    MMIDC_PDA_BUTTON_EXPAND_CTRL_ID,    
    MMIDC_PDA_LABEL_CTRL_ID,    
    MMIDC_PDA_ICON_TYPE_LIST_CTRL_ID, 
    MMIDC_PDA_ICON_SETTING_TYPE_LIST_CTRL_ID,    

    MMIDC_PDA_PREVIEW_IMG_CTRL_ID,

    MMIDC_PDA_BUTTON_CMD_1_CTRL_ID,
    MMIDC_PDA_BUTTON_CMD_2_CTRL_ID,
    MMIDC_PDA_BUTTON_CMD_3_CTRL_ID,

    MMIDC_PDA_BUTTONLIST_MODE_CTRL_ID,
    MMIDC_PDA_BUTTONLIST_IMAGE_PROPERTY_CTRL_ID,
    MMIDC_PDA_BUTTONLIST_WHITE_BLANCE_CTRL_ID,
    MMIDC_PDA_BUTTONLIST_EFFECT_CTRL_ID,    
    MMIDC_PDA_BUTTONLIST_SETTING_CTRL_ID,

    MMIDC_PDA_BUTTON_ARROW_LEFT_CTRL_ID,
    MMIDC_PDA_BUTTON_ARROW_RIGHT_CTRL_ID,

#if defined(MMIDC_F_EFFECT_360)    
    MMIDC_PDA_BUTTON_EFFECT_360_LEFT_CTRL_ID,
    MMIDC_PDA_BUTTON_EFFECT_360_RIGHT_CTRL_ID,
#endif    
    MMIDC_PDA_CTRL_ID_END,

#if defined(MMIDC_F_U_CAMERA)
    MMIDC_UCAMERA_ICONLIST_CTRL_ID,
#endif

    DC_MAX_CTRL_ID 
}DC_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
