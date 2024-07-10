/****************************************************************************
** File Name:      mmimms_menutable.h                                      *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_MENUTABLE_H
#define _MMIMMS_MENUTABLE_H

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
    MMIMMS_MENU_MENU_START = (MMI_MODULE_MMS << 16),
        
    //minghu 09-06-10 MENU_MMS_SETTING
    ID_MMS_SETTING_USER1,               //sim¿¨1ÅäÖÃÑ¡Ôñ
    ID_MMS_SETTING_USER2,               //sim¿¨2ÅäÖÃÑ¡Ôñ
    ID_MMS_SETTING_USER3,              //sim¿¨3ÅäÖÃÑ¡Ôñ
    ID_MMS_SETTING_USER4,              //sim¿¨4ÅäÖÃÑ¡Ôñ
    ID_MMS_SETTING_USER,
    ID_MMS_SETTING_RECV,
    ID_MMS_SETTING_SEND,
    ID_MMS_SETTING_SOUND,               //ÊÇ·ñ²¥·ÅÉùÒô
    ID_MMS_SETTING_DEFAULT,             //»Ö¸´Ä¬ÈÏÉèÖÃ
    ID_MMS_SETTING_SAVE_FILE_POS,

//    ID_MMS_SETTING_AUTO_DOWNLOAD,       //ÊÇ·ñ×Ô¶¯ÏÂÔØ  
//    ID_MMS_SETTING_ADVERTISING,         //ÊÇ·ñÔÊÐí¹ã¸æ

    ID_MMS_SETTING_NETWORK_SETTINGTWO,
    ID_MESSAGE_SETTING_MMS,
    ID_MMS_SETTING_NETWORK_SETTING,
    ID_MMS_SETTING_ARRIVE_REPORT,
    ID_MMS_SETTING_READ_REPORT,
    ID_MMS_SETTING_ANONYMITY,
    ID_MMS_SETTING_PRIORITY,    
                  
    ID_MMS_SETTING_PERIOD,

    //inbox mms play opt menu id
    ID_MMS_INBOX_DEL,
    ID_MMS_ANSWER_OPT,
    ID_MMS_INBOX_FORWARD,
    ID_MMS_CALL_OPT,
    ID_MMS_INBOX_PICKOUT_NUMBER,
    ID_MMS_INBOX_EXTRACT_OR_NOT,
    ID_MMS_INBOX_EXTRACT_YES,
    ID_MMS_INBOX_EXTRACT_NO,
    ID_MMS_SAVE_MEDIA_FILE,
    ID_MMS_INBOX_DETAIL,
    ID_MMS_EDIT_EXTRACT_STADDR,
    ID_MMS_ANSWER_SMS,
    ID_MMS_ANSWER_MMS,
    ID_MMS_DIAL,
    ID_MMS_IP_DIAL,
    ID_MMS_VIDEOPHONE_DAIL,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    ID_MMS_MENU_BOX_SECURITY_ITEM,
    ID_MMS_MENU_BOX_PREVIEW_ITEM,
    ID_MMS_MENU_MOVE_ITEM,
#endif

    //mms noti detail inbox opt menu id
    ID_MMS_INBOX_DOWNLOAD_MMS,
    ID_MMS_ADD2PB,
    //sent box mms play opt menu id
    ID_MMS_SENTBOX_PLAY,
    ID_MMS_SENTBOX_DEL,
    ID_MMS_SENTBOX_FORWARD,
    ID_MMS_SENTBOX_DETAIL,

    //out box mms play opt menu id
    ID_MMS_OUTBOX_SEND,
    ID_MMS_OUTBOX_PLAY,
    ID_MMS_OUTBOX_DEL,
    ID_MMS_OUTBOX_EDIT,
    ID_MMS_OUTBOX_DETAIL,
    
    //mms edit multifile edit pop menu
    ID_MMS_MULTI_REPLACE,
    ID_MMS_MULTI_DELETE,

    ID_MMS_SEND_SMS,
    ID_MMS_SEND_MMS,
    ID_MMS_PREVIEW_URL_ENTER,
    ID_MMS_PREVIEW_URL_ADD_TO_BOOKMARK,
    ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID,
    ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID,
    

    MMIMMS_MENU_SEND_ITEM_ID,
    #ifndef MMI_GUI_STYLE_TYPICAL
    #ifdef TTS_SUPPORT
    ID_MMS_TTS_PLAY,
    #endif
    ID_MMS_INSERT_PIC,
    ID_MMS_INSERT_TXT,
    ID_MMS_INSERT_AUDIO,
    ID_MMS_INSERT_VIDEO,
    ID_MMS_INSERT_NEXTPAGE,
    ID_MMS_INSERT_PREPAGE,
    ID_MMS_INSERT_TAKE_PIC,

#ifdef MMI_RECORD_SUPPORT
    ID_MMS_INSERT_TAKE_AUDIO,
#endif
    ID_MMS_INSERT_TAKE_VIDEO,    
    ID_MMS_INSERT_ATTACH,
    ID_MMS_INSERT_ATTACH_VCARD,
    ID_MMS_INSERT_ATTACH_OTHER,
    ID_MMS_SAVE_MMS_FILE,
    #endif
#ifdef SNS_SUPPORT
    ID_MMS_SHARE_TO_SINA,
    ID_MMS_SHARE_TO_FACEBOOK,
    ID_MMS_SHARE_TO_TWITTER,
#endif
    MMIMMS_MENU_MENU_MAX_ITEM_ID
}MMIMMS_ID_MENU_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIMMS_MENU_LABEL_START = (MMI_MODULE_MMS << 16),
    #include "mmimms_menutable.def"
    MMIMMS_MENUTABLE_MAX
} MMIMMS_MENU_LABEL_E;

#undef MENU_DEF


/*****************************************************************************/
//     Description : Register MMS menu group
//    Global resource dependence : none
//  Author: aoke.hu
//    Note:
/*****************************************************************************/
PUBLIC void MMIMMS_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif //_MMIMMS_MENUTABLE_H_
