/******************************************************************************
**  File Name:      mmiatv_id.h                                               *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about nv                  *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifdef ATV_SUPPORT
#ifndef _MMIATV_ID_H_
#define _MMIATV_ID_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id
 
// window ID
typedef enum
{
    ATV_WIN_ID_START = (MMI_MODULE_ATV << 16),
#include "mmiatv_id.def"

    MMIATV_MAX_WIN_ID
}ATV_WINDOW_ID_E;

#undef WIN_ID_DEF



// control id
typedef enum
{
    MMIATV_CTRL_ID_START = MMIATV_MAX_WIN_ID,

    //主选项
    MMIATV_MAIN_OPT_MENU_CTRL_ID,
    MMIATV_ID_MENU_ADD_FAVORITE,
    MMIATV_ID_MENU_FAVORITE_SERVICE_LIST,
    MMIATV_ID_MENU_SERVICE_LIST,
    MMIATV_ID_MENU_RECORD_LIST,
    MMIATV_ID_MENU_SEARCH_SERVICE,
    MMIATV_ID_MENU_SELECT_REGION,
    MMIATV_ID_MENU_SETTING,
    MMIATV_ID_MENU_BRIGHTNESS,
    MMIATV_ID_MENU_HELP,
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
#ifdef MMI_ATV_RECORD_SUPPORT
    MMIATV_ID_MENU_RECORD,
#endif
#endif
     //favorite service list opt
    MMIATV_FAVORITE_OPT_MENU_CTRL_ID,
    MMIATV_ID_MENU_EDIT,
    MMIATV_ID_MENU_DELETE,
    MMIATV_ID_MENU_DELETE_ALL,

    //list
    MMIATV_SERVICE_LIST_CTRL_ID,
    MMIATV_FAVORITE_SERVICE_LIST_CTRL_ID,
    MMIATV_SELECT_REGION_LIST_CTRL_ID,

    //edit
    MMIATV_EDIT_FAVORITE_NAME_CTRL_ID,

    //help
    MMIATV_HELP_TEXT_CTRL_ID,
    
    //setting
    MMIATV_SETTING_FORM_CTRL_ID,
    MMIATV_RECORD_TYPE_FORM_CTRL_ID,
    MMIATV_RECORD_DEV_FORM_CTRL_ID,
    MMIATV_LABEL_RECORD_DEV_CTRL_ID,
    MMIATV_DROPDOWNLIST_RECORD_DEV_CTRL_ID,
    MMIATV_LABEL_RECORD_TYPE_CTRL_ID,
    MMIATV_DROPDOWNLIST_RECORD_TYPE_CTRL_ID,

    //form
    MMIATV_FORM_CTRL_ID,
    MMIATV_VIDEO_OWNDRAW_CTRL_ID,
    MMIATV_TIP_LABEL_CTRL_ID,
    MMIATV_PROGRESS_OWNDRAW_CTRL_ID,
	MMIATV_PANEL_FORM_CTRL_ID,
    MMIATV_BUTTON_FORM_CTRL_ID,
    MMIATV_BUTTON_VOL_CTRL_ID,
    MMIATV_BUTTON_PRE_CTRL_ID,
    MMIATV_BUTTON_SCREEN_CTRL_ID,
    MMIATV_BUTTON_NEXT_CTRL_ID,
    MMIATV_BUTTON_RECORD_CTRL_ID,
    MMIATV_BUTTON_OPTION_CTRL_ID,
    MMIATV_BUTTON_RETURN_CTRL_ID,
    MMIATV_BUTTON_AREA_FORM_CTRL_ID,
	MMIATV_BTN_OWNER_DRAW_CTRL_ID, 
	MMIATV_ISTYLE_TITLE_FORM_CTRL_ID,
    MMIATV_ISTYLE_DUMY_CTRL_ID,
#ifdef MMI_ATV_MINI_SUPPORT  //6530_MINI
    MMIATV_LABEL_BACK_CTRL_ID,
    MMIATV_LABEL_OPTION_CTRL_ID,
    MMIATV_SIGNAL_LABEL_CTRL_ID,
#endif

#ifdef MMI_PDA_SUPPORT
  //  MMIATV_ISTYLE_TITLE_FORM_CTRL_ID,
  //  MMIATV_ISTYLE_DUMY_CTRL_ID,
 //   MMIATV_BTN_OWNER_DRAW_CTRL_ID,    //button布局填充，确保button垂直居中
    MMIATV_ID_MENU_VIDEO_TYPE_SELECT,
    MMIATV_ID_MENU_VIDEO_TYPE_AVI,
    MMIATV_ID_MENU_VIDEO_TYPE_3GP,
    MMIATV_FAVORITE_SERVICE_LIST_LONGOK_OPT_CTRL_ID,
    MMIATV_SERVICE_LIST_LONGOK_OPT_CTRL_ID,
#endif //MMI_PDA_SUPPORT
    MMIATV_ID_MENU_RECORD_DEVICE,
    MMIATV_ID_MENU_RECORD_UDISK,
    MMIATV_ID_MENU_RECORD_UDISK_MAX = MMIATV_ID_MENU_RECORD_UDISK + MMI_SDCARD_MAX_NUM,
    
    MMIATV_MAX_CTRL_ID
} ATV_CONTROL_ID_E;


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIATV_ID_H_ */
#endif


