/*****************************************************************************
** File Name:      mmibt_menutable.h                                            *
** Author:         lkelly.li                                               *
** Date:           2006/08/17                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to bt menu                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/08/17     kelly.li      Create                                    *
******************************************************************************/

#ifndef _MMIBT_MENUTABLE_H_
#define _MMIBT_MENUTABLE_H_

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
    BT_ID_MENU_START = (MMI_MODULE_BT << 16),              
	ID_MY_BT_SETTING,           
	ID_BT_NONFTP_DEVICE_DETAIL, 
	ID_BT_FTP_DEVICE_DETAIL,    
	ID_BT_HANDFREE,             
	ID_BT_DEVICE_LIST,          
	ID_BT_SEND_FILE,            
	ID_MY_BT_STATE,             
	ID_MY_BT_VISIBILITY,        
	ID_MY_BT_DEVICE_NAME,
	ID_BT_SAVE_FILE_LOCATION,
	ID_BT_DEVICE_NAME,        
	ID_BT_DEVICE_DELETE,        
	ID_BT_DEVICE_SERVICE_LIST,  
	ID_BT_BROWSER,              
	ID_BT_ACCESSING,           
	ID_SEND_FROM_BLUETOOTH,    
	ID_SEND_FROM_MMS,          
	ID_SEND_FROM_EMAIL,        
	ID_SEND_FROM_INFRARED_PORT, 
	ID_SET_BT_ON, 
	ID_SET_BT_OFF, 
	ID_SET_BT_VISIBLE, 
	ID_SET_BT_HIDE,
	ID_BT_SHARED_FILE_SETTING,
	ID_BT_HELP,
	ID_BT_SHARED_FILE_AUTHORITY,
	ID_BT_SHARED_FILE_DIRECTORY,
	ID_BT_SHARED_AUTHORITY_RW,
	ID_BT_SHARED_AUTHORITY_READ,
	ID_BT_SHARED_DIRECTORY_ALL,
	ID_BT_SHARED_DIRECTORY_PHONE,
	ID_BT_SHARED_DIRECTORY_SD,
	ID_BT_SHARED_DIRECTORY_DIR,
	ID_BT_DEVICE_CONNECT,
	ID_BT_DEVICE_DISCONNECT,	
	ID_BT_DEVICE_RENAME,
    ID_BT_DEVICE_DELETE_ALL,
	ID_BT_DEVICE_DETAIL,
	ID_BT_ACCREDIT_SETTINGS,
	ID_BT_ASK_EVERYTIME,
    ID_BT_AUTO_CONNECT,
    BT_ID_MENU_MAX
}BT_ID_MENU_E;



#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    BT_MENU_LABEL_START = (MMI_MODULE_BT << 16),
    #include "mmibt_menutable.def"
    BT_MENUTABLE_MAX
} BT_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* MENUTABLE_H */

