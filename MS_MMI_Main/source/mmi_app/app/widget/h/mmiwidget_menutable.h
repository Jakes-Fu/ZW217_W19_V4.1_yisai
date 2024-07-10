/*****************************************************************************
** File Name:                                             *
** Author:                                                     *
** Date:           2006/09/18                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to set menu                          *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18          Create                                    *
******************************************************************************/

#ifndef _MMIWIDGET_MENUTABLE_H_
#define _MMIWIDGET_MENUTABLE_H_

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


//Menu ID
typedef enum
{
    WIDGET_ID_MENU_START = (MMI_MODULE_WIDGET << 16),
 
	ID_WIDGET_LOG_DETAIL,
	ID_WIDGET_LOG_LIST_DIAL,
	ID_WIDGET_LOG_LIST_SEND,
	ID_WIDGET_SEND_SMS,
	ID_WIDGET_LOG_LIST_DELETE,
	ID_WIDGET_DIAL_AUDIOCALL,
	ID_WIDGET_DIAL_VIDEOCALL,
	ID_WIDGET_DIAL_IPCALL,
	ID_CWIDGET_SEND_MMS,
       SET_ID_MENU_MAX
}WIDGET_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    WIDGET_MENU_LABEL_START = (MMI_MODULE_WIDGET << 16),

    #include "mmiwidget_menutable.def"

    WIDGET_MENUTABLE_MAX
} WIDGET_MENU_LABEL_E;

#undef MENU_DEF
/*****************************************************************************/
// 	Description : Register SET menu group
//	Global resource dependence : none
//  Author: KELLY.LI
//	Note:
/*****************************************************************************/
PUBLIC void MMIWIDGET_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIWIDGET_MENUTABLE_H_




