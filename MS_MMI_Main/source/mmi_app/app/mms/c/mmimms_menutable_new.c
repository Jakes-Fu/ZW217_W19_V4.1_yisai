/****************************************************************************
** File Name:      mmimms_menutable.c                                      *
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

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "guimenu.h"
#include "mmi_text.h"
#include "mmieng_menutable.h"
#include "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"

#include "mmimms_menutable.h"
#include "mmimms_text.h"
#include "mmisms_text.h"
#include "mmiset_text.h"

#ifdef MMI_PDA_SUPPORT
#include "mmisms_image.h"
#endif
#include "mmi_modu_main.h"

//mms setting config
const GUIMENU_ITEM_T menu_mms_setting[] =
{
#ifdef MMI_MULTI_SIM_SYS_SINGLE
    {ID_MMS_SETTING_USER1,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_USER,  0, 0, 0, MENU_NULL},
#else
    {ID_MMS_SETTING_USER1,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_USER1,  0, 0, 0, MENU_NULL},   
    {ID_MMS_SETTING_USER2,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_USER2,  0, 0, 0, MENU_NULL},
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_MMS_SETTING_USER3,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_USER3,  0, 0, 0, MENU_NULL},   
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    {ID_MMS_SETTING_USER4,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_USER4,  0, 0, 0, MENU_NULL},   
#endif
#endif

    {ID_MMS_SETTING_RECV,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_SETTING_RECV_OPT,  0, 0, 0, MENU_NULL},
    {ID_MMS_SETTING_SEND,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_COMM_SEND_OPTION,  0, 0, 0, MENU_NULL},
#ifdef NANDBOOT_SUPPORT
    {ID_MMS_SETTING_SAVE_FILE_POS, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_SAVE_FILE_POSITION,0,0,0,MENU_NULL},
#endif
    {ID_MMS_SETTING_SOUND,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_MMS_PLAY_SOUND,  0, 0, 0, MENU_NULL},      
    {ID_MMS_SETTING_DEFAULT, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_SETTING_RE_DEFAULT,0,0,0,MENU_NULL},
};



#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_mms_inbox_mmsoption[] =
{
#ifndef MMIMMS_SMS_IN_1_SUPPORT //短彩合一
    {ID_MMS_ANSWER_SMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SMS,  IMAGE_COMMON_OPTMENU_WRITE_SMS, 0, 0, MENU_NULL},
    {ID_MMS_ANSWER_MMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS,  0, 0, 0, MENU_NULL},    
#endif
    {ID_MMS_INBOX_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  IMAGE_MESSAGE_OPTMENU_FORWARD, 0, 0, MENU_NULL},
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    {ID_MMS_CALL_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  IMAGE_COMMON_OPTMENU_CALL, 0, 0, MENU_MMS_CALL_OPT}, 
#else
    {ID_MMS_DIAL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  IMAGE_COMMON_OPTMENU_CALL, 0, 0, MENU_NULL},
#endif
#ifdef TTS_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
    {ID_MMS_TTS_PLAY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_SET_TTS_PLAY,  IMAGE_SMS_READ_SMS_PDA, 0, 0, MENU_NULL},
#endif
#else
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  IMAGE_COMMON_OPTMENU_SAVE_CONTENT, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
#endif
    //{ID_MMS_MENU_ADDTO_BLACKLIST_ITEM_ID, TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    //TXT_ADD_TO_BLACKLIST, 0, 0, 0, MENU_NULL},
    {ID_MMS_SAVE_MEDIA_FILE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_SAVE_MEDIA_FILE,  IMAGE_MESSAGE_OPTMENU_SAVE_FILE, 0, 0,MENU_NULL},
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#ifdef MMI_SMS_MOVE_SUPPORT
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  IMAGE_MESSAGE_OPTMENU_MOVE, 0, 0, MENU_MMS_SECURITY_SELECT},    
#endif
#endif
    {ID_MMS_INBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_MESSAGE_OPTMENU_DETAIL, 0, 0, MENU_NULL},
    //macro replace
    //#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
    {ID_MMS_EDIT_EXTRACT_STADDR,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
    TXT_EXTRACT_STADDR,IMAGE_MESSAGE_OPTMENU_INSERT_VCARD,0,0,MENU_NULL}, //提取网址
#endif

};
#else
const GUIMENU_ITEM_T menu_mms_inbox_mmsoption[] =
{
#if 1 //def MMI_GUI_STYLE_TYPICAL
    {ID_MMS_OUTBOX_PLAY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
#ifdef TTS_SUPPORT
        TXT_SET_TTS_PLAY,  
#else
        TXT_COMMON_PLAY,
#endif
        0, 0, 0, MENU_NULL},
#endif     
    {ID_MMS_INBOX_DEL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_DELETE,  0, 0, 0, MENU_NULL},    
#ifndef         MMIMMS_SMS_IN_1_SUPPORT
    {ID_MMS_ANSWER_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_REPLY,  0, 0, 0, MENU_MMS_ANSWER_OPT},   
#else
    {ID_MMS_ANSWER_SMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_REPLY,  0, 0, 0, MENU_NULL},
#endif
    {ID_MMS_INBOX_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_NULL},
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    {ID_MMS_CALL_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_MMS_CALL_OPT}, 
#else
    {ID_MMS_DIAL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_NULL},
#endif
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
    {ID_MMS_SAVE_MEDIA_FILE,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMM_SAVE_MEDIA_FILE,  0, 0, 0,MENU_NULL},  
#ifdef MMI_GUI_STYLE_TYPICAL    
    {ID_MMS_INBOX_EXTRACT_OR_NOT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_MMS_EXTRACT},
#endif    
    {ID_MMS_INBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL},
    //macro replace
    //#ifdef MMI_STREAMING_ENABLE
#ifdef STREAMING_SUPPORT_NEXTREAMING
    {ID_MMS_EDIT_EXTRACT_STADDR,TIP_NULL,{TXT_NULL,TXT_NULL,TXT_NULL},
    TXT_EXTRACT_STADDR,0,0,0,MENU_NULL}, //提取网址
#endif
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  0, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
};
#endif

const GUIMENU_ITEM_T menu_mms_answer_opt[] =
{
    {ID_MMS_ANSWER_SMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SMS,  0, 0, 0, MENU_NULL},
    {ID_MMS_ANSWER_MMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS,  0, 0, 0, MENU_NULL},    
};

//呼叫
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
const GUIMENU_ITEM_T menu_mms_call_option[] =
{
    {ID_MMS_DIAL,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_CALL_AUDIO,  0, 0, 0, MENU_NULL},
#ifdef VT_SUPPORT
    {ID_MMS_VIDEOPHONE_DAIL,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_CALL_VIDEO,  0, 0, 0, MENU_NULL},
#endif
#ifdef MMI_IP_CALL_SUPPORT
    {ID_MMS_IP_DIAL,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_CALL_IPCALL,  0, 0, 0, MENU_NULL},
#endif
};
#endif


#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_mms_sentbox_option[] =
{  
#if 0
    {ID_MMS_SENTBOX_PLAY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_PLAY,  0, 0, 0, MENU_NULL},    

    {ID_MMS_SENTBOX_DEL,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},

    {ID_MMS_SENTBOX_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_NULL},
#endif

    {ID_MMS_SENTBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_MESSAGE_OPTMENU_DETAIL, 0, 0, MENU_NULL},        

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#ifdef MMI_SMS_MOVE_SUPPORT
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE, IMAGE_MESSAGE_OPTMENU_MOVE, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
#endif
};
#else
const GUIMENU_ITEM_T menu_mms_sentbox_option[] =
{    
#if 1 //def MMI_GUI_STYLE_TYPICAL
    {ID_MMS_SENTBOX_PLAY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
#ifdef TTS_SUPPORT
        TXT_SET_TTS_PLAY,  
#else
        TXT_COMMON_PLAY,
#endif
        0, 0, 0, MENU_NULL},    
#endif        
    {ID_MMS_SENTBOX_DEL,  TIP_NULL, {TXT_NULL, TXT_NULL,TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
    {ID_MMS_SENTBOX_FORWARD,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_FORWARD,  0, 0, 0, MENU_NULL},
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
#ifdef MMI_GUI_STYLE_TYPICAL    
    {ID_MMS_INBOX_EXTRACT_OR_NOT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_MMS_EXTRACT},
#endif    
    {ID_MMS_SENTBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL},
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  0, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
};
#endif


#ifdef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_mms_outbox_option[] =
{
    {ID_MMS_OUTBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  IMAGE_MESSAGE_OPTMENU_DETAIL, 0, 0, MENU_NULL},        

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#ifdef MMI_SMS_MOVE_SUPPORT
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  IMAGE_MESSAGE_OPTMENU_MOVE, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
#endif
};
#else
const GUIMENU_ITEM_T menu_mms_outbox_option[] =
{
    
    {ID_MMS_OUTBOX_SEND,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SEND,  0, 0, 0, MENU_NULL},
    {ID_MMS_OUTBOX_PLAY,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
#ifdef TTS_SUPPORT
        TXT_SET_TTS_PLAY,  
#else
        TXT_COMMON_PLAY,
#endif
        0, 0, 0, MENU_NULL},
    {ID_MMS_OUTBOX_DEL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
    {ID_MMS_OUTBOX_EDIT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_EDIT,  0, 0, 0, MENU_NULL},
    {ID_MMS_INBOX_EXTRACT_OR_NOT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_COMMON_CONTENT_EXTRACT,  0, 0, 0, MENU_MMS_EXTRACT},
    {ID_MMS_OUTBOX_DETAIL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    STXT_DETAIL,  0, 0, 0, MENU_NULL}, 
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  0, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
};
#endif

const GUIMENU_ITEM_T menu_mms_inbox_notioption[] =
{
    
    {ID_MMS_INBOX_DOWNLOAD_MMS,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_DOWNLOAD_COMN,  0, 0, 0, MENU_NULL},
    {ID_MMS_INBOX_DEL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    {ID_MMS_CALL_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_MMS_CALL_OPT},
#else
    {ID_MMS_DIAL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_NULL},
#endif
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    {ID_MMS_MENU_MOVE_ITEM,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MOVE,  0, 0, 0, MENU_MMS_SECURITY_SELECT},
#endif
};

const GUIMENU_ITEM_T menu_mms_multi_pop_menu[] =
{
    {ID_MMS_MULTI_REPLACE,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_MMS_RICHEDIT_MULTI_REPLACE,  0, 0, 0, MENU_NULL},
    {ID_MMS_MULTI_DELETE,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_DELETE,  0, 0, 0, MENU_NULL},   
};

const GUIMENU_ITEM_T menu_mms_number_pop_menu[] =
{
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    {ID_MMS_CALL_OPT,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_MMS_CALL_OPT},
#else
    {ID_MMS_DIAL,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CALL,  0, 0, 0, MENU_NULL},
#endif
#ifndef MMIMMS_SMS_IN_1_SUPPORT
    {ID_MMS_ANSWER_OPT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_COMMON_WRITE_MESSAGE, 0, 0, 0, MENU_MMS_ANSWER_OPT}, 
#else
    {ID_MMS_ANSWER_SMS, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},
    TXT_COMMON_WRITE_MESSAGE, 0, 0, 0, MENU_NULL}, 
#endif
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
};

const GUIMENU_ITEM_T menu_mms_url_pop_menu[] =
{
    {ID_MMS_PREVIEW_URL_ENTER,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_BROWSE,  0, 0, 0, MENU_NULL},
    {ID_MMS_PREVIEW_URL_ADD_TO_BOOKMARK,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDTO_BOOKMARK,  0, 0, 0, MENU_NULL},
#ifndef PB_SUPPORT_LOW_MEMORY
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
#endif
};

const GUIMENU_ITEM_T menu_mms_extract[] =
{
    {ID_MMS_INBOX_EXTRACT_YES,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_COMMON_ENABLE_ON,  0, 0, 0, MENU_NULL},
    {ID_MMS_INBOX_EXTRACT_NO,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_CLOSE,  0, 0, 0, MENU_NULL},
};

const GUIMENU_ITEM_T menu_mms_email_pop_menu[] =
{
    {ID_MMS_SEND_MMS,TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_MMS_PREVIEW_NUMBER_SEND_MMS,  0, 0, 0, MENU_NULL},     
    {ID_MMS_ADD2PB,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
    TXT_ADDCONTACT,  0, 0, 0, MENU_MMS_EXTRACT_ADDTOPB_OPT},
};
const GUIMENU_ITEM_T menu_mms_extract_addtopb_opt[] = 
{
    {ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID,TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ADDTOPB_NEW, 0, 0, 0, MENU_NULL},
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    {ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID,TIP_NULL,{TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_ADDTOPB_EXIST, 0, 0, 0, MENU_NULL},
#endif
};

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
const GUIMENU_ITEM_T menu_mms_security_select[] =
{
    {ID_MMS_MENU_BOX_SECURITY_ITEM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS_SECURITY, 0, 0, 0, MENU_NULL},
    {ID_MMS_MENU_BOX_PREVIEW_ITEM, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS_PREVIEW, 0, 0, 0, MENU_NULL}
};
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
#ifdef MMIMMS_SMS_IN_1_SUPPORT
const GUIMENU_ITEM_T menu_mms_insert_select[] =
{
    {ID_MMS_INSERT_TXT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TEMPLATE, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_IMAGE, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_TAKE_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TAKE_PIC, 0, 0, 0, MENU_NULL},

    {ID_MMS_INSERT_AUDIO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_AUDIO, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_TAKE_AUDIO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TAKE_AUDIO, 0, 0, 0, MENU_NULL},

    {ID_MMS_INSERT_VIDEO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_VIDEO, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_TAKE_VIDEO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TAKE_VIDEO, 0, 0, 0, MENU_NULL},
    
    {ID_MMS_INSERT_NEXTPAGE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_INSERT_SLIDE, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_PREPAGE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_INSERT_SLIDER, 0, 0, 0, MENU_NULL},
           
    {ID_MMS_INSERT_ATTACH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_ATTACHMENT, 0, 0, 0, MENU_MMS_ATTACH_SUB_MENU},    
};
#else
const GUIMENU_ITEM_T menu_mms_insert_select[] =
{
    {ID_MMS_INSERT_TXT, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_TEXT, 0, 0, 0, MENU_NULL},
    
    {ID_MMS_INSERT_NEXTPAGE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_INSERT_SLIDE, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_PREPAGE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_INSERT_SLIDER, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_IMAGE, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_TAKE_PIC, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TAKE_PIC, 0, 0, 0, MENU_NULL},        
    {ID_MMS_INSERT_AUDIO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_AUDIO, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_VIDEO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_VIDEO, 0, 0, 0, MENU_NULL},
    {ID_MMS_INSERT_TAKE_VIDEO, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_SMS_TAKE_VIDEO, 0, 0, 0, MENU_NULL},        
    {ID_MMS_INSERT_ATTACH, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_ATTACHMENT, 0, 0, 0, MENU_MMS_ATTACH_SUB_MENU},
};
#endif
const GUIMENU_ITEM_T menu_mms_attach_subopt[] =
{
#ifdef MMI_VCARD_SUPPORT
    {ID_MMS_INSERT_ATTACH_VCARD, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_COMMON_VCARD, 0, 0, 0, MENU_NULL},  
#endif
    {ID_MMS_INSERT_ATTACH_OTHER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS_ATTACHMENT_OTHER, 0, 0, 0, MENU_NULL},
};
const GUIMENU_ITEM_T menu_mms_savefile_opt[] =
{
    {ID_MMS_SAVE_MMS_FILE, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_SAVE, 0, 0, 0, MENU_NULL},   
#ifdef SNS_SUPPORT
#ifdef WRE_WEIBO_SUPPORT
    {ID_MMS_SHARE_TO_SINA, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_SINA, 0, 0, 0, MENU_NULL},   
#endif
#ifdef WRE_FACEBOOK_SUPPORT
    {ID_MMS_SHARE_TO_FACEBOOK, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_FACEBOOK, 0, 0, 0, MENU_NULL},   
#endif
#ifdef WRE_TWITTER_SUPPORT
    {ID_MMS_SHARE_TO_TWITTER, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SHARE_TO_TWITTER, 0, 0, 0, MENU_NULL},   
#endif    
#endif
};

#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
const GUIMENU_ITEM_T menu_mms_edit_option[] =
{
    {MMIMMS_MENU_SEND_ITEM_ID,  TIP_NULL, {TXT_NULL, TXT_NULL, TXT_NULL},
        TXT_SEND,  0, 0, 0, MENU_NULL}            // 发送   
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T mmimms_menu_table[] = 
{
    {NULL},
    #include "mmimms_menutable.def"
};
#undef MENU_DEF

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : Register Input method menu group
//    Global resource dependence : none
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMIMMS_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_MMS, mmimms_menu_table);
}
