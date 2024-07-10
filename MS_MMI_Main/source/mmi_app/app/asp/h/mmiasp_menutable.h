/*****************************************************************************
** File Name:      mmiasp_menutable.h                                        *
** Author:         plum.peng                                                 *
** Date:           2008/12/01                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe asp menu id                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/12/01     plum.peng        Create                                    *
******************************************************************************/

#ifndef _MMIASP_MENUTABLE_H_
#define _MMIASP_MENUTABLE_H_

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
    ID_ASP_MENU_START = (MMI_MODULE_ASP << 16),
    MMIASP_MENU_CALL_ITEM_ID,
    MMIASP_MENU_MSG_ITEM_ID,
    MMIASP_MENU_DELETE_ITEM_ID,
    MMIASP_MENU_MARK_ITEM_ID,
    MMIASP_MENU_SEND_ITEM_ID,
    MMIASP_MENU_SETTING_ITEM_ID,
    MMIASP_MENU_CALL_AUDIO_ITEM_ID,
    MMIASP_MENU_CALL_VIDEO_ITEM_ID,
    MMIASP_MENU_CALL_IP_ITEM_ID,
    MMIASP_MENU_MSG_SMS_ITEM_ID,
    MMIASP_MENU_MSG_MMS_ITEM_ID,
    MMIASP_MENU_MARK_SINGLE_ITEM_ID,
    MMIASP_MENU_UNMARK_SINGLE_ITEM_ID,
    MMIASP_MENU_MARK_ALL_ITEM_ID,
    MMIASP_MENU_UNMARK_ALL_ITEM_ID,
    MMIASP_MENU_SEND_MMS_ITEM_ID,
    MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID,
    MMIASP_MENU_HELP_ITEM_ID,
    
    MMIASP_CUSTOM_RING_FIXED,
    MMIASP_CUSTOM_RING_DOWNLOAD,
    MMIASP_CUSTOM_RING_RECORDONE,
    
#ifdef MMI_PDA_SUPPORT
	//MMIASP_MENU_RENAME_ITEM_ID,
	//MMIASP_MENU_SEND_EMAIL_ITEM_ID,
#endif
    ID_ASP_MENU_MAX
}MMIASP_ID_MENU_E;


#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIASP_MENU_LABEL_START = (MMI_MODULE_ASP << 16),

    #include "mmiasp_menutable.def"

    MMIASP_MENUTABLE_MAX
} MMIASP_MENU_LABEL_E;

#undef MENU_DEF


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif


