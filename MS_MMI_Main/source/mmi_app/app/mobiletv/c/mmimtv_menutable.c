/*****************************************************************************
** File Name:      mmimtv_menutable.c                                        *
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

#define _MMIMTV_MENUTABLE_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_menutable.h"
#include "mmimtv_menutable.h"
#include "mmimtv_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmimtv_id.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//#define TIP_NULL    0

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL const GUIMENU_ITEM_T	mmimtv_main_menu[] =
{
#ifndef MBBMS_SUPPORT
    {MTV_ID_NET_LIST_MENU,               TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_NETWORK_LIST,                0, 0, 0, MENU_NULL},
    {MTV_ID_SEARCH_MENU,		         TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_SEARCH,                     0, 0, 0, MENU_NULL},
    {MTV_ID_RECORDED_PROGRAM_LIST_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_RECORDED_PROGRAM_LIST,   0, 0, 0, MENU_NULL},
    {MTV_ID_EMERGENCY_BROADCASTING_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_EMERGENCY_BROADCASTING,  0, 0, 0, MENU_NULL},
    //to do {MTV_ID_BOOKING_RECORD_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_BOOKING_RECORD,	        0, 0, 0, MENU_NULL}, 
    {MTV_ID_OTHER_FUNCTIONS_MENU,		 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_OTHER_FUNCTIONS,	        0, 0, 0, MENU_NULL},          
    {MTV_ID_SETTING_MENU,                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_SETTING,                 0, 0, 0, MENU_NULL},    
    //{MTV_ID_SYSTEM_INFOR_MENU,           TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_SYSTEM_INFO,             0, 0, 0, MENU_NULL},    
    {MTV_ID_HELP_MENU,                   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_HELP,                    0, 0, 0, MENU_NULL},      
    {MTV_ID_EXIT_MENU,                   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_EXIT,                    0, 0, 0, MENU_NULL},
#else
	{MTV_ID_MBBMS_ADD_FAVORITE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_ADD_FAVORITE_SERVICE,	0, 0, 0, MENU_NULL},
	{MTV_ID_MBBMS_MAIN_INTERFACE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_MAIN_INTERFACE,		0, 0, 0, MENU_NULL},
    {MTV_ID_EMERGENCY_BROADCASTING_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_EB,                    0, 0, 0, MENU_NULL},      
	{MTV_ID_OTHER_FUNCTIONS_MENU,		 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_OTHER_FUNCTIONS,	        0, 0, 0, MENU_NULL},          
    {MTV_ID_SETTING_MENU,                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_SETTING,                 0, 0, 0, MENU_NULL},
    {MTV_ID_EXIT_MENU,                   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_EXIT,                    0, 0, 0, MENU_NULL},
#endif
};

#ifdef MBBMS_SUPPORT
//mbbms 当前频道已收藏，应提示
LOCAL const GUIMENU_ITEM_T	mmimtv_main_menu3[] =
{
    {MTV_ID_MBBMS_ADD_FAVORITE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_DEL_FAVORITE_SERVICE,	0, 0, 0, MENU_NULL},
    {MTV_ID_MBBMS_MAIN_INTERFACE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_MAIN_INTERFACE,		0, 0, 0, MENU_NULL},
    {MTV_ID_EMERGENCY_BROADCASTING_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_EB,                    0, 0, 0, MENU_NULL},
    {MTV_ID_OTHER_FUNCTIONS_MENU,		 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_OTHER_FUNCTIONS,	        0, 0, 0, MENU_NULL},
    {MTV_ID_SETTING_MENU,                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_SETTING,                 0, 0, 0, MENU_NULL},
    {MTV_ID_EXIT_MENU,                   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_EXIT,                    0, 0, 0, MENU_NULL},
};
//用于MBBMS无信号时兼容CMMB功能, 前两项ITEM的显示影射为网络列表/搜索
LOCAL const GUIMENU_ITEM_T	mmimtv_main_menu2[] =
{
    {MTV_ID_MBBMS_ADD_FAVORITE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_CMMB_NET_LIST,	0, 0, 0, MENU_NULL},
    {MTV_ID_MBBMS_MAIN_INTERFACE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_SEARCH,		        0, 0, 0, MENU_NULL},
    {MTV_ID_EMERGENCY_BROADCASTING_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_EB,            0, 0, 0, MENU_NULL},
    {MTV_ID_OTHER_FUNCTIONS_MENU,		 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_OTHER_FUNCTIONS,	        0, 0, 0, MENU_NULL},
    {MTV_ID_SETTING_MENU,                TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_SETTING,                 0, 0, 0, MENU_NULL},
    {MTV_ID_EXIT_MENU,                   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_EXIT,                    0, 0, 0, MENU_NULL},
};
#endif

LOCAL const GUIMENU_ITEM_T	mmimtv_record_list_option_menu[] =
{	
    {MTV_ID_PLAY_RECORD_MENU,       TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_PLAY,             0, 0, 0, MENU_NULL},
    {MTV_ID_DELETE_RECORD_MENU,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELETE,	    0, 0, 0, MENU_NULL},
    {MTV_ID_DELETE_ALL_RECORD_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELALL,               0, 0, 0, MENU_NULL},
    {MTV_ID_RENAME_RECORD_MENU,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_RENAME,    0, 0, 0, MENU_NULL},
    {MTV_ID_RECORD_DETAIL_MENU,     TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMM_DETAIL,    0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T    mmimtv_search_menu[] =
{
    {MTV_ID_AUTO_SEARCH_MENU,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_AUTO_SEARCH,      0, 0, 0, MENU_NULL},
    {MTV_ID_MANUAL_SEARCH_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MANUAL_SEARCH,    0, 0, 0, MENU_NULL},
    //to do {MTV_ID_CITY_SEARCH_MENU,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_AUTO_SEARCH,      0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T    mmimtv_system_infor_menu[] =
{
    {MTV_ID_INTELLIGENT_CARD_DETAIL_MENU,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_INTELLIGENT_CARD_DETAIL, 0, 0, 0, MENU_NULL},
    {MTV_ID_SOFTWARE_VERSION_DETAIL_MENU,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_SOFTWARE_VERSION_DETAIL, 0, 0, 0, MENU_NULL},
    {MTV_ID_KDA_VERSION_DETAIL_MENU,        TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_KDA_VERSION_DETAIL,      0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T    mmimtv_other_function_menu[] =
{
    {MTV_ID_DIAL_MENU,      TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMM_DIALPAD,         0, 0, 0, MENU_NULL},
    {MTV_ID_SMS_MENU,       TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMUNICATION_MESSAGE,  0, 0, 0, MENU_NULL},
    {MTV_ID_PHONEBOOK_MENU, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, STXT_MAIN_PB,     0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T    mmimtv_eb_list_opt_menu[] =
{
    {MTV_ID_EB_OPTION_SORT_BY_TIME,      TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_SORT_BY_TIME,         0, 0, 0, MENU_NULL},
    {MTV_ID_EB_OPTION_SORT_BY_GRADE,       TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_SORT_BY_LEVEL,  0, 0, 0, MENU_NULL},
    {MTV_ID_EB_OPTION_DELETE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELETE,     0, 0, 0, MENU_NULL},
    {MTV_ID_EB_OPTION_DELETE_ALL, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_DELALL,     0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T	mmimtv_service_option_menu[] =
{
    {MTV_ID_PLAY_MENU,      TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_PLAY,             0, 0, 0, MENU_NULL},
    {MTV_ID_ESG,            TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_ESG,              0, 0, 0, MENU_NULL},
    {MTV_ID_UPDATE_SERVICE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_UPDATE_SERVICE,   0, 0, 0, MENU_NULL},
    {MTV_ID_DELETE_SERVICE, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_COMMON_DELETE_CHANNEL,   0, 0, 0, MENU_NULL},
    
    //to do {MTV_ID_SORT_SERVICE,   TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_SORT_SERVICE,     0, 0, 0, MENU_NULL},
};

LOCAL const GUIMENU_ITEM_T	mmimtv_net_option_menu[] =
{	
    {MTV_ID_UPDATE_NET, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_UPDATE_NET,   0, 0, 0, MENU_NULL},
    {MTV_ID_DELETE_NET, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MTV_DLETE_NET,	0, 0, 0, MENU_NULL},
};

#ifdef MBBMS_SUPPORT
LOCAL const GUIMENU_ITEM_T    mmimbbms_search_rst_popup_opt[] =
{
    {MMIMBBMS_MENU_COLLECT_ITEM_ID,    TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_COLLECT_OR_UNCOLLECT,        0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_DETAIL_ITEM_ID,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_COMMON_PROGRAM_DETAIL,            0, 0, 0, PNULL},
    {MMIMBBMS_MENU_COMBO_MANAGE_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_PURCHASE_MANAGE,             0, 0, 0, MENU_NULL}
};


LOCAL const GUIMENU_ITEM_T    mmimbbms_program_list_popup_opt[] =
{
#if 0
    {MMIMBBMS_MENU_WATCH_ITEM_ID,    TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_WATCH,        0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_PROGRAM_DETAIL_ITEM_ID,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
        TXT_COMMON_PROGRAM_DETAIL,            0, 0, 0, PNULL},
    {MMIMBBMS_MENU_COLLECT_UNCOLLECT_PROGRAM_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_COLLECT_OR_UNCOLLECT_PROGRAM,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_COLLECT_UNCOLLECT_CHANNEL_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_COLLECT_OR_UNCOLLECT_CHANNEL,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_COMBO_MANAGE_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_PURCHASE_MANAGE,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_SEARCH_PROGRAM_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_SEARCH_PROGRAM,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_SERVICE_MANAGE_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_SERVICE_MANAGEMENT,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_SETTING_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        STXT_MAIN_SETTING,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_HELP_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_HELP,             0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_NOTIFY_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_NOTIFY,             0, 0, 0, MENU_NULL}
#endif
	{
		0
	}
};

LOCAL const GUIMENU_ITEM_T    mmimbbms_program_detail_popup_opt[] =
{
#if 0
    {MMIMBBMS_MENU_WATCH_ITEM_ID,    TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_WATCH,        0, 0, 0, MENU_NULL},
    {MMIMBBMS_MENU_COLLECT_UNCOLLECT_PROGRAM_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MBBMS_COLLECT_OR_UNCOLLECT_PROGRAM,             0, 0, 0, MENU_NULL}
#endif
	{
		0
	}
};

LOCAL const GUIMENU_ITEM_T	mmimbbms_update_menu[] =
{
    {MTV_ID_MBBMS_AUTO_UPDATE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_AUTO_UPDATE,	0, 0, 0, MENU_NULL},
    {MTV_ID_MBBMS_CITY_UPDATE_MENU,	 TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL}, TXT_MBBMS_UPDATE_ACCORDING_TO_CITY,		0, 0, 0, MENU_NULL},
};
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

LOCAL const GUIMENU_GROUP_T mmimtv_menu_table[] = 
{
    {NULL},
    #include "mmimtv_menu_wintable.def"
};

#undef MENU_DEF
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register mtv menu group
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMTV_RegMenuWin(void)
{
    MMI_RegMenuGroup(MMI_MODULE_MTV, mmimtv_menu_table);
}


