
/*****************************************************************************
** File Name:      mmidcd_menutable.h                                        *
** Author:         violent.wei                                               *
** Date:           2007/09/24                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    					                     *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/09/24     violent.wei      Create                                    *
******************************************************************************/
#ifndef _MMIDCD_MENUTABLE_H_
#define _MMIDCD_MENUTABLE_H_

#ifdef MMI_ENABLE_DCD
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
	ID_DCD_MENU_START = (MMI_MODULE_DCD << 16),
	//dcd
	//for dcd menu
	ID_DCD_ITEM,      
	ID_DCD_ITEM_UPDATE_CHANNEL,
	ID_DCD_ITEM_DELETE,
	ID_DCD_MARK,	
	ID_DCD_SUB_MARK,
	ID_DCD_CANCEL_MARK,
	ID_DCD_MARK_ALL,
	ID_DCD_CANCEL_ALL_MARK,
	ID_DCD_ITEM_SEND,
	ID_DCD_ITEM_PREVIEW_LOCAL,
	ID_DCD_CHANNEL_MANAGEMENT,
	ID_DCD_ITEM_PREVIEW,
	ID_DCD_ITEM_SETTING,
	ID_DCD_ITEM_HELP,
	
	ID_DCD_ITEM_ADD_CHANNEL,
	ID_DCD_ITEM_DELETE_CHANNEL,
	ID_DCD_ITEM_DETAIL_CHANNEL,  //add by wz for dcd detail

	ID_DCD_ITEM_SETTING_DCD_OPEN_SWITCH,
	ID_DCD_ITEM_SETTING_CONNECT_MODE,
	/*modified by @jun.hu, 2009/7/27 begin +*/
       ID_DCD_ITEM_SETTING_SCROLL_TIME,
       ID_DCD_ITEM_SETTING_SHOW_READED,
       ID_DCD_ITEM_SETTING_MEMORY,
        ID_DCD_ITEM_SETTING_SIM_INTERET,
       ID_DCD_ITEM_SETTING_RECORY_FACRORTY,
    	/*modified by @jun.hu, 2009/7/27 end -*/
	ID_DCD_ITEM_SETTING_HELP,

	ID_DCD_ITEM_SEND_BY_SMS,
	ID_DCD_ITEM_SEND_BY_MMS,
	ID_DCD_ITEM_SAVE,	
	ID_DCD_LOCAL_ITEM,
	ID_DCD_LOCAL_ITEM_PREVIEW,
	ID_DCD_LOCAL_ITEM_DELETE,
	ID_DCD_LOCAL_ITEM_SEND_BY_SMS,
	ID_DCD_LOCAL_ITEM_SEND_BY_MMS,
	ID_DCD_PREVIEW,
       ID_DCD_PREVIEW_MORE,
       ID_DCD_PREVIEW_ABSTRACT,
        ID_DCD_NETWORK_SETTING,
        ID_DCD_NETWORK_HOMEPAGE,
        ID_DCD_NETWORK_APN,        
        ID_DCD_NETWORK_USER,        
        ID_DCD_NETWORK_PASSWORD,
        ID_DCD_NETWORK_PROXY,        
        ID_DCD_NETWORK_PORT,        
	ID_DCD_MENU_MAX
    
}MMIDCD_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIDCD_MENU_LABEL_START = (MMI_MODULE_DCD << 16),

    #include "mmidcd_menutable.def"

    MMIDCD_MENUTABLE_MAX
} MMIDCD_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/
#endif
#endif //_DCD_MENUTABLE_H_

