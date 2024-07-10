/*****************************************************************************
** File Name:      mmimtv_menutable.h                                        *
** Author:                                                                   *
** Date:           2010-10-26                                                *
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mobile tv module            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010-10         aoke.hu          Create                                   *
******************************************************************************/

#ifndef _MMIMTV_MENUTABLE_H_
#define _MMIMTV_MENUTABLE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
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
//Menu ID
typedef enum
{
    MMIMTV_ID_MENU_START = (MMI_MODULE_MTV << 16),
    MTV_ID_NET_LIST_MENU,
    MTV_ID_SEARCH_MENU,
    MTV_ID_RECORDED_PROGRAM_LIST_MENU,
    MTV_ID_EMERGENCY_BROADCASTING_MENU,
	MTV_ID_BOOKING_RECORD_MENU,
    MTV_ID_OTHER_FUNCTIONS_MENU,
	MTV_ID_SETTING_MENU,
	MTV_ID_SYSTEM_INFOR_MENU,
	MTV_ID_HELP_MENU,
	MTV_ID_EXIT_MENU,
#ifdef MBBMS_SUPPORT
	MTV_ID_MBBMS_ADD_FAVORITE_MENU,
	MTV_ID_MBBMS_MAIN_INTERFACE_MENU,
    MTV_ID_MBBMS_AUTO_UPDATE_MENU,
    MTV_ID_MBBMS_CITY_UPDATE_MENU,
#endif

    MTV_ID_PLAY_RECORD_MENU,
    MTV_ID_DELETE_RECORD_MENU,
    MTV_ID_DELETE_ALL_RECORD_MENU,
    MTV_ID_RENAME_RECORD_MENU,
    MTV_ID_RECORD_DETAIL_MENU,

    MTV_ID_AUTO_SEARCH_MENU,
    MTV_ID_MANUAL_SEARCH_MENU,
    MTV_ID_CITY_SEARCH_MENU,

    MTV_ID_PLAY_MENU,
    MTV_ID_EB_OPTION_CTRL_MENU,
    //system information
    MTV_ID_SYSTEM_INFOR_CTRL_MENU,
    MTV_ID_INTELLIGENT_CARD_DETAIL_MENU,
    MTV_ID_SOFTWARE_VERSION_DETAIL_MENU,
    MTV_ID_KDA_VERSION_DETAIL_MENU,
    
    //go to other functions
    MTV_ID_OTHER_FUNCTIONS_CTRL_MENU,
    MTV_ID_DIAL_MENU,
    MTV_ID_SMS_MENU,
    MTV_ID_PHONEBOOK_MENU,

    MMIMTV_ID_MENU_MAX
}MMIMTV_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIMTV_MENU_LABEL_START = (MMI_MODULE_MTV << 16),
    #include "mmimtv_menu_wintable.def"
    MMIMTV_MENUTABLE_MAX
} MMMTV_MENU_LABEL_E;

#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Register mtv menu group
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_RegMenuWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
