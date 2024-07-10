/*****************************************************************************
** File Name:      mmiwidget_menutable.c                                        *
** Author:                                                                   *
** Date:                                                 *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to handle setting menutable             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18         Creat                                     *
******************************************************************************/

#define _MMIWIDGET_MENUTABLE_C_
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "window_parse.h"
//#include "sci_types.h"
#include "mmiwidget_menutable.h"
#include "guimenu.h"
#include "mmi_text.h"
#include "mmiwidget_text.h"
#include "mmiwidget_id.h"
#include "mmi_module.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmicc_text.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define TIP_NULL 0

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/







//pb favorite list
LOCAL const GUIMENU_ITEM_T menu_widget_pb_favorite_list_opt[] =
{
	    {ID_WIDGET_LOG_DETAIL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_DETAIL, 0, 0 ,0 , NULL},  // œÍ«È
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
	    {ID_WIDGET_LOG_LIST_DIAL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL, 0, 0, 0, MENU_WIDGET_DIAL_SUBMENU_OPT}, // ∫ÙΩ–
#else
	    {ID_WIDGET_DIAL_AUDIOCALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL, 0, 0, 0, NULL}, // ∫ÙΩ–
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
           {ID_WIDGET_SEND_SMS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_WRITE_MESSAGE, 0, 0, 0, NULL},
#else
#ifdef MMS_SUPPORT
	    {ID_WIDGET_LOG_LIST_SEND, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_WRITE_MESSAGE, 0, 0, 0, MENU_WIDGET_SEND_OPT},
#else
	    {ID_WIDGET_SEND_SMS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_WRITE_MESSAGE, 0, 0, 0, NULL},
#endif
#endif
	    {ID_WIDGET_LOG_LIST_DELETE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, NULL},// …æ≥˝
};

#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
LOCAL const GUIMENU_ITEM_T menu_widget_dial_submenu_opt[] =
{
	    {ID_WIDGET_DIAL_AUDIOCALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_AUDIO, 0, 0, 0, NULL}, // ”Ô“ÙµÁª∞
#ifdef VT_SUPPORT
	    {ID_WIDGET_DIAL_VIDEOCALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_VIDEO, 0, 0, 0, NULL}, // ø… ”µÁª∞
#endif    
#ifdef MMI_IP_CALL_SUPPORT
	    {ID_WIDGET_DIAL_IPCALL, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_IPCALL, 0, 0, 0, NULL}, // IP∫ÙΩ–
#endif
};
#endif

LOCAL const GUIMENU_ITEM_T menu_widget_send_opt[] =
{
	    {ID_WIDGET_SEND_SMS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SMS, 0, 0, 0, NULL},
#ifdef MMS_SUPPORT
	    {ID_CWIDGET_SEND_MMS, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_MMS, 0, 0, 0, NULL},
#endif
};



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

LOCAL const GUIMENU_GROUP_T widget_menu_table[] = 
{
    {NULL},
#include "mmiwidget_menutable.def"
};

#undef MENU_DEF
/*****************************************************************************/
// 	Description : Register SET menu group
//	Global resource dependence : none
//  Author: KELLY.LI
//	Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_WIDGET, widget_menu_table);
}

