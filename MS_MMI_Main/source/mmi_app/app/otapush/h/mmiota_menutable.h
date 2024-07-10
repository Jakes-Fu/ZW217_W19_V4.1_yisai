/****************************************************************************
** File Name:      mmiota_menutable.h                                      *
** Author:         minghu.mao                                                 *
** Date:           08/10/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2009        minghu.mao          create
** 
****************************************************************************/

#ifndef _MMIOTA_MENUTABLE_H
#define _MMIOTA_MENUTABLE_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
//Menu ID
typedef enum
{
    MMIOTA_MENU_MENU_START = (MMI_MODULE_OTA << 16),
        
    ID_OTA_CONFIG_SET_DELETE,             
    ID_OTA_CONFIG_SET_SAVE, 
    ID_OTA_CONFIG_SET_DETAIL,
    ID_OTA_CONFIG_SET_CALL,
    ID_OTA_CONFIG_SET_ADD2PB,
    ID_OTA_CONFIG_SET_EXISTNUM,
    ID_PUSH_MENU_MOVE_ITEM,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    ID_PUSH_MENU_BOX_SECURITY_ITEM,
    ID_PUSH_MENU_BOX_PREVIEW_ITEM,
#endif
    
    ID_CMSBRW_PUSHMSG_ENTER,
    ID_CMSBRW_PUSHMSG_DELETE,
    
    MMIOTA_MENU_MENU_MAX_ITEM_ID
}MMIOTA_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIOTA_MENU_LABEL_START = (MMI_MODULE_OTA << 16),
    #include "mmiota_menutable.def"
    MMIOTA_MENUTABLE_MAX
} MMIOTA_MENU_LABEL_E;

#undef MENU_DEF

#ifdef MMI_OTA_SUPPORT
/*****************************************************************************/
//     Description : Register MMS menu group
//    Global resource dependence : none
//  Author: aoke.hu
//    Note:
/*****************************************************************************/
PUBLIC void MMIOTA_RegMenuGroup(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIMMS_MENUTABLE_H_
