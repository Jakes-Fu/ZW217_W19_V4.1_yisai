#ifdef WRE_SUPPORT
/****************************************************************************
** File Name:      wre_menutable.c                            
** Author:    Annie.an                                                              
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to define the menu and related information for game.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef WRE_SUPPORT
#define _WRE_MENUTABLE_C_  



#include "window_parse.h"
#include "guimenu.h"
#include "mmiwre_text.h"
#include "mmi_image.h"
#include "mmiwre_menutable.h"
#include "mmi_modu_main.h"
#include "mmiwre_id.h"
#include "mmi_module.h"
#include "mmi_menutable.h"


#if 0
// begin of demo menu 
const GUIMENU_ITEM_T  menu_demo_list[] =
{
    {IDDEMO_MENU_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_WRE_TEXT1,   IMAGE_SECMENU_ICON_PASTIME_LINKS,   0,0,0},
    {IDDEMO_MENU_ITEMEX_ID,  TIP_NULL, {STXT_RETURN, TXT_NULL, TXT_COMMON_OK},STXT_WRE_TEXT2,   IMAGE_SECMENU_ICON_PASTIME_LINKS,   0,0,0},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T demo_menu_table[] = 
{
    {NULL},
#include "mmiwre_menutable.def"
//MENU_DEF( MENU_DEMO_LIST, menu_demo_list, GUIMENU_STYLE_THIRD, STXT_MAIN_DEMO_TITLE, COMMON_IMAGE_NULL, COMMON_IMAGE_NULL, COMMON_IMAGE_NULL, GUIMENU_ITEM_NUM(menu_demo_list) )
};
#undef  MENU_DEF

/*****************************************************************************/
// 	Description :  Register menu group
//	Global resource dependence : 
//  Author:KELLY.LI
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWRE_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_WRE, demo_menu_table);
}

#endif


#endif


#endif //end of FILE
