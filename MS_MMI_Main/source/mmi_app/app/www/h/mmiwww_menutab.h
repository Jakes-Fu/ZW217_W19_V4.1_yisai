/****************************************************************************
** File Name:      mmiwww_menutab.h                                         *
** Author:                                                                  *
** Date:           06/15/2009                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the window for WWW Browser *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 06/2009        jiaoyou.wu       Create
** 
****************************************************************************/
#ifndef _MMIWWW_MENUTABLE_H_
#define _MMIWWW_MENUTABLE_H_

#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

#define TIP_NULL 0

//MENU ID
typedef enum
{
    ID_WWW_MENU_START = (MMI_MODULE_WWW<< 16),
    ID_WWW_SUBMENU_SEARCH_ENGINE,
    ID_WWW_SUBMENU_SAVED_PAGE,
    ID_WWW_SUBMENU_CLEAN_INPUT_HISTORY,
    ID_WWW_SUBMENU_SETTING,
    ID_WWW_ENGINE_CMCC, /* First Engine ID */
    ID_WWW_ENGINE_BAIDU,
    ID_WWW_ENGINE_GOOGLE,
    ID_WWW_ENGINE_YAHOO,
    ID_WWW_SETMENU_VIEWMODE,
    ID_WWW_SETMENU_AUTOWEBDATA,
    ID_WWW_SETMENU_CLEARWEBDATA,
    ID_WWW_SETMENU_SCRIPT,
    ID_WWW_SETMENU_ANIMATION,
    ID_WWW_SETMENU_PHOTO,
    ID_WWW_SETMENU_TABLE,
    ID_WWW_SETMENU_BGMUSIC,
    ID_WWW_SETMENU_POPUPWIN,
    ID_WWW_SETMENU_DISPALAYOPT,
    ID_WWW_SETMENU_OPT_DISPLAY,
    ID_WWW_SETMENU_OPT_NODISPLAY,
    ID_WWW_SETMENU_SETFONT,
    ID_WWW_SETMENU_SETCOOKIE,
    ID_WWW_SETMENU_COOKIEMODE,
    ID_WWW_SETMENU_CLEARCOOKIE,
    ID_WWW_SETMENU_SETCACHE,
    ID_WWW_SETMENU_CERTIFICATE,
    ID_WWW_SETMENU_WEBDATA,
    ID_WWW_SETMENU_ACCOUNT1,
    ID_WWW_SETMENU_ACCOUNT2,
    ID_WWW_SETMENU_ACCOUNT,
    ID_WWW_SETMENU_CHARSET,
    ID_WWW_SETMENU_POINTER_MODE,
    ID_WWW_SETMENU_DEFAULTSET,
    ID_WWW_SETMENU_CACHEMODE,
    ID_WWW_SETMENU_IF_USE_DISK_CACHE,
    ID_WWW_SETMENU_CLEAR_CACHE,
    ID_WWW_OPT_SEND_LINK,
    ID_WWW_OPT_SHOW_ADDRESS,
    ID_WWW_OPT_SORT_BY_DATE,
    ID_WWW_OPT_SORT_BY_NAME,
    ID_WWW_DELETE_OPT_DELETE_CUR,
    ID_WWW_DELETE_OPT_DELETE_CURDAY,
    ID_WWW_DELETE_OPT_DELETE_ALL,
    ID_WWW_SEND_OPT_VIA_SMS,
    ID_WWW_SEND_OPT_VIA_MMS,
    ID_WWW_BOOKMARK_SUBMENU_ADD,
    ID_WWW_BOOKMARK_SUBMENU_EDIT,
    ID_WWW_BOOKMARK_SUBMENU_DELETE,
    ID_WWW_BOOKMARK_SUBMENU_DELETE_ALL,
    ID_WWW_BOOKMARK_SUBMENU_PROPERTY,
    ID_WWW_BOOKMARK_SUBMENU_DETAIL_EDIT,
    ID_WWW_BOOKMARK_SUBMENU_DETAIL_DELETE,

    ID_WWW_BOOKMARK_SUBMENU_MOVE_TO,//3       
    ID_WWW_BOOKMARK_SUBMENU_ADD_FILE,
    ID_WWW_BOOKMARK_SUBMENU_ADD_FOLDER,

    ID_WWW_PAGE_OPEN,
    ID_WWW_OPEN_URL,
    ID_WWW_SEARCH,
    ID_WWW_BOOKMARK,
    ID_WWW_HISTORY,
    ID_WWW_PAGE_PILOT,
    ID_WWW_PAGE_REFRESH,
    ID_WWW_PAGE_FORWARD,
    ID_WWW_PAGE_BACK,
    ID_WWW_FULL_DISPLAY,
    ID_WWW_HORIZONTAL_DISPLAY,
    ID_WWW_PAGE_NAVIGATION,
    ID_WWW_PAGE_TOOL,
    ID_WWW_PAGE_SEARCH,
    ID_WWW_PAGE_COPY,
    ID_WWW_PAGE_PROPERTY,
    ID_WWW_DOWNLOAD_MANAGE,
    ID_WWW_MESSAGE,
    ID_WWW_CALL,
    ID_WWW_SAVE_AS,
    ID_WWW_PAGE_WINDOW,
    ID_WWW_NEW_WINDOW,
    ID_WWW_CLOSE_WINDOW,
    ID_WWW_PAGE_ZOOM,
    ID_WWW_PAGE_ZOOM_PERCENT_50, /* Do Not add id between PERCENT_50 and PERCENT_150 */
    ID_WWW_PAGE_ZOOM_PERCENT_75,
    ID_WWW_PAGE_ZOOM_PERCENT_100,
    ID_WWW_PAGE_ZOOM_PERCENT_125,
    ID_WWW_PAGE_ZOOM_PERCENT_150, 
    ID_WWW_PAGE_SAVE,
    ID_WWW_SAVE_BOOKMARK,
    ID_WWW_WEBPAGE,
    ID_WWW_PAGE_MEMO,
    ID_WWW_OPT_SORT,
    ID_WWW_MINIMIZE,
    ID_WWW_EXIT,

    ID_WWW_SHOTCUT_KEY_1,/* Do Not add id between ID_WWW_SHOTCUT_KEY_1 and ID_WWW_SHOTCUT_KEY_STAR */
    ID_WWW_SHOTCUT_KEY_2,
    ID_WWW_SHOTCUT_KEY_3,
    ID_WWW_SHOTCUT_KEY_4,
    ID_WWW_SHOTCUT_KEY_5,
    ID_WWW_SHOTCUT_KEY_6,
    ID_WWW_SHOTCUT_KEY_7,
    ID_WWW_SHOTCUT_KEY_8,
    ID_WWW_SHOTCUT_KEY_9,
    ID_WWW_SHOTCUT_KEY_0,
    ID_WWW_SHOTCUT_KEY_HASH,
    ID_WWW_SHOTCUT_KEY_STAR,

#if 0//@fen.xie
    ID_WWW_CC_VOICE,
    ID_WWW_CC_IP,
    ID_WWW_CC_VIDEO,
#endif    
    ID_WWW_HOMPAGE,
    ID_WWW_SETMENU_HOMEPAGE,
    ID_WWW_AS_HOMEPAGE,

    ID_WWW_NEW_SUB_WIN,
    ID_WWW_CLOSE_SUB_WIN,
    ID_WWW_WINDOW_MANAGER,

    ID_WWW_MENU_MAX
}MMIWWW_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIWWW_MENU_TABLE_START = (MMI_MODULE_WWW << 16),
#include "mmiwww_menutab.def"
    MMIWWW_MENUTABLE_MAX
} CMSBRW_MenuLabelE;

#undef MENU_DEF


/*-------------------------------------------------------------------------*/
/*                         VARIABLES                                       */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                         FUNCTION DECLARE                                */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register menu group
//	Global resource dependence :mmiwww_menu_table
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMIWWW_MENUTABLE_H_ */
