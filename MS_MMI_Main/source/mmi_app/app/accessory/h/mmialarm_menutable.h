/*****************************************************************************
** File Name:      mmialarm_menutable.h                                      *
** Author:         liqing.peng                                               *
** Date:           2006/09/12                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to alarm menu                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/12     liqing.peng      Create                                    *
******************************************************************************/

#ifndef _MMIALARM_MENUTABLE_H_
#define _MMIALARM_MENUTABLE_H_

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
    ALARM_ID_MENU_START = (MMI_MODULE_ALARM << 16),

    ID_ALM_CLOSE,
    ID_ALM_OPEN, 
    ID_ALM_RING,
    ID_ALM_MODE,
    ID_ALM_EDIT,
    ID_ALM_RING_INHERE, 
    ID_ALM_RING_SELFHOOD,
    ID_ALM_RING_MP3,
    //ID_ALM_RING_RECORD,

    ID_SCHEDULE, 
    ID_SCH_EDIT, 
    ID_SCH_VIEW,
    ID_SCH_DEL,
    ID_SCH_TO_DATE,
    ID_SCH_DELALL,

	ID_SCH_LIST_VIEW,
	ID_SCH_LIST_NEW,
	ID_SCH_LIST_EDIT,
	ID_SCH_LIST_DEL,
	ID_SCH_LIST_DEL_ALL,
	ID_SCH_LIST_LUNAR_DETAIL,
	ID_SCH_LIST_MARK,
	ID_SCH_LIST_SUB_MARK,
	ID_SCH_LIST_SUB_CANCEL_MARK,
	ID_SCH_LIST_SUB_MARK_ALL,
	ID_SCH_LIST_SUB_CANCEL_MARK_ALL,
	ID_SCH_LIST_SEND,
	ID_SCH_LIST_SEND_SMS,
	ID_SCH_LIST_SEND_MMS,
	ID_SCH_LIST_SEND_BT,
	ID_SCH_LIST_SEND_EMAIL,
#ifdef MMI_VCALENDAR_SUPPORT
	ID_SCH_VCALENDAR,
	ID_SCH_VCALENDAR_IMPORT,
	ID_SCH_VCALENDAR_EXPORT,
#endif	
	ID_SCH_LIST_DEL_MARK,
    ID_ALARM_RING_FIX,
    ID_ALARM_RING_DOWNLOAD,
    ID_ALARM_RING_FM,
    ID_FEAST_VIEW,
    ID_FEAST_NEW,
    ID_FEAST_INQUIRE,
    ID_FEAST_DELETE,
    ID_FEAST_DELALL,
    ID_ALARM_RING_RECORD,
#ifdef MMI_ALARM_DEL_ADD_SUPPORT
    ID_ALM_OPT_ADD,
    ID_ALM_OPT_EDIT,
    ID_ALM_OPT_DEL,
#endif
    
    ID_AUTO_POWERON,
    ID_AUTO_POWEROFF,

    ID_SCH_ALL_LIST_VIEW,
	ID_SCH_ALL_LIST_NEW,
	ID_SCH_ALL_LIST_EDIT,
	ID_SCH_ALL_LIST_DEL,
	ID_SCH_ALL_LIST_DEL_ALL,

    ALARM_ID_MENU_MAX
}ALARM_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    ALARM_MENU_LABEL_START = (MMI_MODULE_ALARM << 16),

    #include "mmialarm_menutable.def"

    ALARM_MENUTABLE_MAX
} ALARM_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIALARM_MENUTABLE_H_

