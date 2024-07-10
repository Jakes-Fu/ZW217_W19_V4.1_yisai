/*****************************************************************************
** File Name:      mmiatv_menutable.c                                        *
** Author:                                                                   *
** Date:           2010-05	                                                 *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe atv menu table              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "guimenu.h"
#include "mmiatv_menutable.h"
#include "mmiatv_id.h"
#include "mmiatv_text.h"
#include "mmiatv_image.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
//menu table
#ifndef MMI_PDA_SUPPORT

const GUIMENU_ITEM_T	mmiatv_main_opt_menu[] =
{
    {MMIATV_ID_MENU_ADD_FAVORITE,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ADD_FAVORITE_SERVICE,	 0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_FAVORITE_SERVICE_LIST,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MAINMENU_FAVORITE, 0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_SERVICE_LIST,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_COMMON_CHANNEL_LIST,	         0, 0, 0, MENU_NULL},

#ifdef MMI_ATV_RECORD_SUPPORT	
    {MMIATV_ID_MENU_RECORD_LIST,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_RECORD_LIST,	         0, 0, 0, MENU_NULL},    
#endif

//#ifdef MMI_ATV_MINI_SUPPORT
    {MMIATV_ID_MENU_BRIGHTNESS,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_BRIGHTNESS_ID,         0, 0, 0, MENU_NULL},
//#endif  

	{MMIATV_ID_MENU_SEARCH_SERVICE,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_SEARCH_SERVICE,	     0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_SELECT_REGION,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_SELECT_REGION,	     0, 0, 0, MENU_NULL},
#ifndef MMI_ATV_MINI_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMI_ATV_RECORD_SUPPORT 
	{MMIATV_ID_MENU_RECORD_DEVICE,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_RECORD_DEVICE,	         0, 0, 0, MMIATV_RECORD_STORE_STATUS}, 
#endif
#else
    {MMIATV_ID_MENU_SETTING,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_SETTING,	         0, 0, 0, MENU_NULL},    
#endif    
//    {MMIATV_ID_MENU_BRIGHTNESS,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_BRIGHTNESS_ID,         0, 0, 0, MENU_NULL},
#endif  //MMI_ATV_MINI_SUPPORT
#ifndef MMI_PDA_SUPPORT
    {MMIATV_ID_MENU_HELP,	                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_HELP_ID,	             0, 0, 0, MENU_NULL},
#endif
};

#else//pda style
const GUIMENU_ITEM_T	mmiatv_main_opt_menu[] =
{
    {MMIATV_ID_MENU_FAVORITE_SERVICE_LIST,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MAINMENU_FAVORITE, IMAGE_COMMON_OPTMENU_FAVORITE, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_SERVICE_LIST,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_COMMON_CHANNEL_LIST,	         IMAGE_ATV_CHANNEL_LIST, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_BRIGHTNESS,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_BRIGHT_ADJUST,	         IMAGE_ATV_ADJUST_BRIGHTNESS, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_ADD_FAVORITE,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ADD_FAVORITE_SERVICE,	 IMAGE_ATV_ADD_TO_FAVORITES, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_SEARCH_SERVICE,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_SEARCH_SERVICE,	     IMAGE_COMMON_OPTMENU_SEARCH, 0, 0, MENU_NULL},    
#ifdef MMI_ATV_RECORD_SUPPORT   
	{MMIATV_ID_MENU_RECORD_LIST,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_RECORD_LIST,	         0, 0, 0, MENU_NULL},
#endif 
    #ifdef MMIATV_RECORD_3GP_SUPPORT
    {MMIATV_ID_MENU_VIDEO_TYPE_SELECT,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_RECORD_FILE_TYPE,	         0, 0, 0, MMIATV_VIDEO_TYPE}, 
    #endif
#ifdef MMI_ATV_RECORD_SUPPORT	
    {MMIATV_ID_MENU_RECORD_DEVICE,	            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_RECORD_DEVICE,	         0, 0, 0, MMIATV_RECORD_STORE_STATUS}, 
#endif
	{MMIATV_ID_MENU_SELECT_REGION,	        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ATV_SELECT_REGION,	     0, 0, 0, MENU_NULL},    
 #ifndef MMI_PDA_SUPPORT
	{MMIATV_ID_MENU_HELP,	                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_HELP,	                 0, 0, 0, MENU_NULL},
#endif
};

const GUIMENU_ITEM_T	mmiatv_favorite_service_list_longok_opt_menu[] =
{
    {MMIATV_ID_MENU_EDIT,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_EDIT,	      0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_DELETE,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELETE,     0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T	mmiatv_service_list_longok_opt_menu[] =
{
    {MMIATV_ID_MENU_ADD_FAVORITE,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ADD_FAVORITE,	      0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T	mmiatv_video_type[] =
{
    {MMIATV_ID_MENU_VIDEO_TYPE_3GP,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_NULL,     0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_VIDEO_TYPE_AVI,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_NULL,	      0, 0, 0, MENU_NULL},  
};
#endif //MMI_PDA_SUPPORT

const GUIMENU_ITEM_T mmiatv_record_store_status[] =
{
    {MMIATV_ID_MENU_RECORD_UDISK,			 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_UDISK,  0, 0, 0, MENU_NULL},
 #ifndef DUAL_TCARD_SUPPORT
    {MMIATV_ID_MENU_RECORD_UDISK +1,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD,  0, 0, 0,  MENU_NULL},
#else
    {MMIATV_ID_MENU_RECORD_UDISK +1,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD0,  0, 0, 0,  MENU_NULL},
    {MMIATV_ID_MENU_RECORD_UDISK +2,	 TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},   TXT_COMMON_SD_CARD1,  0, 0, 0,  MENU_NULL},
#endif    
};

const GUIMENU_ITEM_T	mmiatv_favorite_list_opt_menu[] =
{
    {MMIATV_ID_MENU_EDIT,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_EDIT,	      0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_DELETE,         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELETE,     0, 0, 0, MENU_NULL},
    {MMIATV_ID_MENU_DELETE_ALL,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELALL, 0, 0, 0, MENU_NULL},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmiatv_menu_win_table[] = 
{
    {NULL},
#include "mmiatv_menu_wintable.def"
};

#undef MENU_DEF

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : Register atv menu group
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIATV_RegMenuWin(void)
{
    MMI_RegMenuGroup(MMI_MODULE_ATV, mmiatv_menu_win_table);
}
