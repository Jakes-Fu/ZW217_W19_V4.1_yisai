/*****************************************************************************
** File Name:      mmibrowser_menutable.h                                    *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser menutable       *
*****************************************************************************/
#ifndef MMIBROWSER_MENUTAB_H_
#define MMIBROWSER_MENUTAB_H_

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
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    ID_BRW_MENU_START = (MMI_MODULE_MMIBROWSER<< 16),

//    ID_POPUP_MENU_INPUTADD,
//    ID_POPUP_MENU_OPEN_HOMEPAGE,
//    ID_POPUP_MENU_SET_MAIN,
//    ID_POPUP_MENU_REFRESH,
//    ID_POPUP_MENU_STOP,
//    ID_POPUP_MENU_FORWARD,

//    ID_POPUP_MENU_SAVE_BOOKMARK,
//    ID_POPUP_MENU_SAVE_INTERNET_FILE,
//    ID_POPUP_MENU_BOOKMARK_LIST,

//    ID_POPUP_MENU_SUB_FILEINFO,
//     ID_POPUP_MENU_SUB_DISCONNECT,
//    ID_POPUP_MENU_SUB_CLEARCACHE,
//    ID_POPUP_MENU_SUB_CLEARCOOKIE,
//    ID_POPUP_MENU_TOOL,
//    ID_POPUP_MENU_EXIT,
//    ID_POPUP_MENU_HISTORY,
//    ID_POPUP_MENU_HAS_SAVE_FILE,
//    ID_POPUP_MENU_SUB_SETTING,
//    ID_POPUP_MENU_SUB_HELP,
//    ID_POPUP_MENU_OPEN,
//    ID_POPUP_MENU_CURR_PAGE,

    ID_POPUP_MENU_HISTORY_ENTER,
    ID_POPUP_MENU_HISTORY_DETAIL,
    ID_POPUP_MENU_HISTORY_DELETE,
    ID_POPUP_MENU_HISTORY_SEND_URL,
    ID_POPUP_MENU_HISTORY_DELETEALL,
#ifdef MMIDORADO_MY_NAV_SUPPORT
    ID_POPUP_MENU_HISTORY_ADD2MYNAV,
#endif
    ID_POPUP_MENU_HISTORY_AS_HOMEPAGE,

#if 0
    ID_POPUP_MENU_NAV_HIS_ADD,
    ID_POPUP_MENU_NAV_HIS_FORWARD,
    ID_POPUP_MENU_NAV_HIS_BACKWARD,
    ID_POPUP_MENU_NAV_HIS_REFRESH,
    ID_POPUP_MENU_NAV_HIS_CLEAN,
#endif

    ID_POPUP_MENU_BOOKMARK_OPEN,
    ID_POPUP_MENU_BOOKMARK_OPEN_FOLDER,
    ID_POPUP_MENU_BOOKMARK_ADD,
    ID_POPUP_MENU_BOOKMARK_ADD_FILE,
    ID_POPUP_MENU_BOOKMARK_ADD_FOLDER,
    ID_POPUP_MENU_BOOKMARK_SEND_URL,
#ifdef SNS_SUPPORT    
    ID_POPUP_MENU_BOOKMARK_SHARE2SNS,
    ID_POPUP_MENU_BRW_SHARE_TO_SINA,
    ID_POPUP_MENU_BRW_SHARE_TO_FACEBOOK,
    ID_POPUP_MENU_BRW_SHARE_TO_TWITTER,
#endif
    ID_POPUP_MENU_BOOKMARK_DELETE,
    ID_POPUP_MENU_BOOKMARK_DELETE_FOLDER,
    ID_POPUP_MENU_BOOKMARK_EDIT,
    ID_POPUP_MENU_BOOKMARK_EDIT_FOLDER,
    ID_POPUP_MENU_BOOKMARK_DETAIL,
    //ID_POPUP_MENU_BOOKMARK_EXIT_BROWSER,
    ID_POPUP_MENU_BOOKMARK_MARK,
    ID_POPUP_MENU_BOOKMARK_MOVE_TO,
    ID_POPUP_MENU_BOOKMARK_MARKED,
    ID_POPUP_MENU_BOOKMARK_UNMARKED,
    ID_POPUP_MENU_BOOKMARK_MARKED_ALL,
    ID_POPUP_MENU_BOOKMARK_UNMARKED_ALL,
#ifdef MMIDORADO_MY_NAV_SUPPORT
    ID_POPUP_MENU_BOOKMARK_ADD2MYNAV,
#endif
    ID_POPUP_MENU_BOOKMARK_AS_HOMEPAGE,
    
    ID_POPUP_MENU_SNAPSHOT_OPEN,
    ID_POPUP_MENU_SNAPSHOT_DELETE,
    ID_POPUP_MENU_SNAPSHOT_DETAIL,
    ID_POPUP_MENU_SNAPSHOT_RENAME,
    ID_POPUP_MENU_SNAPSHOT_MARK,
    ID_POPUP_MENU_SNAPSHOT_MARKED,
    ID_POPUP_MENU_SNAPSHOT_UNMARKED,
    ID_POPUP_MENU_SNAPSHOT_MARKED_ALL,
    ID_POPUP_MENU_SNAPSHOT_UNMARKED_ALL,

    //ID_BROWSER_CC_VOICE,
    //ID_BROWSER_CC_IP,
    //ID_BROWSER_CC_VIDEO,

#ifdef MMIDORADO_MY_NAV_SUPPORT
    ID_POPUP_MENU_MY_NAV_DELETE,
    ID_POPUP_MENU_MY_NAV_EDIT,
    ID_POPUP_MENU_MY_NAV_DETAIL,
#endif

    ID_POPUP_MENU_SAVE_IMAGE_VIEW,
    ID_POPUP_MENU_SAVE_IMAGE_SAVE,
    ID_POPUP_MENU_SAVE_IMAGE_DETAIL,

    ID_BRW_MENU_TOTAL
}ID_BRW_MENU_E;
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
	MMI_BRW_BEGIN_MENU_START = (MMI_MODULE_MMIBROWSER<< 16),
	#include "mmibrowser_menutable.def"
	MENUTABLE_MAX
} MMI_BRW_MENUTAB_E;
#undef MENU_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : reg menu
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_RegMenuGroup(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
