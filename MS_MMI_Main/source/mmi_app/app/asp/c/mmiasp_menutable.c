/****************************************************************************
** File Name:      mmiasp_menutable.c                                      *
** Author:                                                                 *
** Date:           2008.11.26                                              *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the ANSWER PHONE          *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmiasp_cortrol_layer.h"
#ifdef MMI_PDA_SUPPORT
#include "mmiset_text.h"
#include "mmimp3_text.h"
#endif



/**------------------------------------------------------------------------*
**                         MACRO DEFINITION                                *
**-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                              */
/*-------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 

/*--------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                          */
/*--------------------------------------------------------------------------*/
#ifndef MMI_PDA_SUPPORT
const GUIMENU_ITEM_T menu_asp_msg_opt[] =
{
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
    {MMIASP_MENU_CALL_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_ASP_CALL_OPT},    
#else
    {MMIASP_MENU_CALL_AUDIO_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_NULL},    
#endif
    {MMIASP_MENU_MSG_ITEM_ID,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_REPLY,  0, 0, 0, MENU_ASP_SEND_MSG_OPT},
    {MMIASP_MENU_DELETE_ITEM_ID,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MARK_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MARK,      0, 0, 0, MENU_ASP_MARK_OPT},    
    {MMIASP_MENU_SEND_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SEND,          0, 0, 0, MENU_ASP_SEND_OPT},
    {MMIASP_MENU_SETTING_ITEM_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_SETTING,       0, 0, 0, MENU_NULL},
    {MMIASP_MENU_HELP_ITEM_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_HELP,       0, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_play_win_msg_opt[] =
{
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
    {MMIASP_MENU_CALL_ITEM_ID,    TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_ASP_CALL_OPT},   
#else
    {MMIASP_MENU_CALL_AUDIO_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_NULL},    
#endif
    {MMIASP_MENU_MSG_ITEM_ID,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_REPLY,  0, 0, 0, MENU_ASP_SEND_MSG_OPT},
    {MMIASP_MENU_DELETE_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},
    
};
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
const GUIMENU_ITEM_T menu_asp_call_opt[] =
{
    {MMIASP_MENU_CALL_AUDIO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_AUDIO,   0, 0, 0, MENU_NULL},    
#ifdef VT_SUPPORT
    {MMIASP_MENU_CALL_VIDEO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_VIDEO,   0, 0, 0, MENU_NULL},  
#endif
#ifdef MMI_IP_CALL_SUPPORT
    {MMIASP_MENU_CALL_IP_ITEM_ID,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_IPCALL, 0, 0, 0, MENU_NULL}
#endif
};
#endif
const GUIMENU_ITEM_T menu_asp_sendmsg_opt[] =
{
    {MMIASP_MENU_MSG_SMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS, 0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MSG_MMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS, 0, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_mark_opt[] =
{
    {MMIASP_MENU_MARK_SINGLE_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MARK,           0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_UNMARK_SINGLE_ITEM_ID,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CANCEL_MARK,    0, 0, 0, MENU_NULL},
    {MMIASP_MENU_MARK_ALL_ITEM_ID,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_MARK_ALL,        0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_UNMARK_ALL_ITEM_ID,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CANCEL_MARK_ALL, 0, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_send_opt[] =
{
    {MMIASP_MENU_SEND_MMS_ITEM_ID,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS,           0, 0, 0, MENU_NULL},    
#if defined(BLUETOOTH_SUPPORT)
    {MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BLUETOOTH,     0, 0, 0, MENU_NULL}
#endif
};

const GUIMENU_ITEM_T menu_asp_set_music_opt[] =
{
    {MMIASP_CUSTOM_RING_FIXED,		TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_RING_FIXED, 0, 0, 0, MENU_NULL},
    {MMIASP_CUSTOM_RING_DOWNLOAD,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_MORE_RING, 0, 0, 0, MENU_NULL},
    {MMIASP_CUSTOM_RING_RECORDONE,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_RECORD_ANSWER_WORD, 0, 0, 0, MENU_NULL}
};

#else
const GUIMENU_ITEM_T menu_asp_msglocallist_longok_opt[] = 
{					 
    {MMIASP_MENU_CALL_AUDIO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_AUDIO,   0, 0, 0, MENU_NULL},    
#ifdef VT_SUPPORT
    {MMIASP_MENU_CALL_VIDEO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_VIDEO,   0, 0, 0, MENU_NULL},  
#endif
#ifdef MMI_IP_CALL_SUPPORT
    {MMIASP_MENU_CALL_IP_ITEM_ID,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_IPCALL, 0, 0, 0, MENU_NULL},
#endif
	{MMIASP_MENU_MSG_SMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS, 0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MSG_MMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},/*TXT_MMS*/TXT_SET_ORIGIN_REPLY_MMS, 0, 0, 0, MENU_NULL},
	{MMIASP_MENU_DELETE_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL}, 
	//{MMIASP_MENU_RENAME_ITEM_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL	},
	{MMIASP_MENU_SEND_MMS_ITEM_ID,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},/*TXT_MMS*/TXT_COMM_MMS_SEND,           0, 0, 0, MENU_NULL},    
#if defined(BLUETOOTH_SUPPORT)
    {MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_BLUETOOTH,     0, 0, 0, MENU_NULL},
#endif        
	//{MMIASP_MENU_SEND_EMAIL_ITEM_ID, TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE, 0, 0, 0, MENU_NULL	}
};

const GUIMENU_ITEM_T menu_asp_msg_opt[] =
{
    //{MMIASP_MENU_CALL_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_ASP_CALL_OPT},    
    //{MMIASP_MENU_MSG_ITEM_ID,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_REPLY,  0, 0, 0, MENU_ASP_SEND_MSG_OPT},
    //{MMIASP_MENU_DELETE_ITEM_ID,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_DELETE,        0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MARK_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MARK,      IMAGE_COMMON_OPTMENU_MARK, 0, 0, MENU_ASP_MARK_OPT},    
    //{MMIASP_MENU_SEND_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_SEND,          0, 0, 0, MENU_ASP_SEND_OPT},
    {MMIASP_MENU_SETTING_ITEM_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},STXT_MAIN_SETTING,       IMAGE_COMMON_OPTMENU_SET, 0, 0, MENU_NULL},
    {MMIASP_MENU_HELP_ITEM_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_HELP,       IMAGE_COMMON_OPTMENU_HELP, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_play_win_msg_opt[] =
{
	{MMIASP_MENU_CALL_AUDIO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_AUDIO,   IMAGE_ASP_MENU_AUDIO_CALL, 0, 0, MENU_NULL},    
#ifdef VT_SUPPORT
    {MMIASP_MENU_CALL_VIDEO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_VIDEO,   IMAGE_COMMON_OPTMENU_VIDIO_CALL, 0, 0, MENU_NULL},  
#endif
#ifdef MMI_IP_CALL_SUPPORT
	{MMIASP_MENU_CALL_IP_ITEM_ID,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_IPCALL, IMAGE_COMMON_OPTMENU_IP_CALL, 0, 0, MENU_NULL},
#endif
	{MMIASP_MENU_MSG_SMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS, IMAGE_ASP_MENU_REPLAY_SMS, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MSG_MMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS, IMAGE_COMMON_OPTMENU_REPLY_MMS, 0, 0, MENU_NULL},
    //{MMIASP_MENU_CALL_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CALL,     0, 0, 0, MENU_ASP_CALL_OPT},    
    //{MMIASP_MENU_MSG_ITEM_ID,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_REPLY,  0, 0, 0, MENU_ASP_SEND_MSG_OPT},
    {MMIASP_MENU_DELETE_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_DELETE,        IMAGE_COMMON_OPTMENU_DELETE, 0, 0, MENU_NULL}
};
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
const GUIMENU_ITEM_T menu_asp_call_opt[] =
{
    {MMIASP_MENU_CALL_AUDIO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_AUDIO,   0, 0, 0, MENU_NULL},    
#ifdef VT_SUPPORT
    {MMIASP_MENU_CALL_VIDEO_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_VIDEO,   0, 0, 0, MENU_NULL},  
#endif
#ifdef MMI_IP_CALL_SUPPORT
    {MMIASP_MENU_CALL_IP_ITEM_ID,     TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_CALL_IPCALL, 0, 0, 0, MENU_NULL}
#endif
};
#endif
const GUIMENU_ITEM_T menu_asp_sendmsg_opt[] =
{
    {MMIASP_MENU_MSG_SMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_SMS, 0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_MSG_MMS_ITEM_ID,  TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS, 0, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_mark_opt[] =
{
    {MMIASP_MENU_MARK_SINGLE_ITEM_ID,    TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},STXT_MARK,           0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_UNMARK_SINGLE_ITEM_ID,  TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CANCEL_MARK,    0, 0, 0, MENU_NULL},
    {MMIASP_MENU_MARK_ALL_ITEM_ID,       TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_MARK_ALL,        0, 0, 0, MENU_NULL},    
    {MMIASP_MENU_UNMARK_ALL_ITEM_ID,     TIP_NULL, {STXT_OK, TXT_NULL, STXT_RETURN},TXT_CANCEL_MARK_ALL, 0, 0, 0, MENU_NULL}
};

const GUIMENU_ITEM_T menu_asp_send_opt[] =
{
    {MMIASP_MENU_SEND_MMS_ITEM_ID,       TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMS,           0, 0, 0, MENU_NULL},    
#if defined(BLUETOOTH_SUPPORT)        
    {MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID, TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_BLUETOOTH,     0, 0, 0, MENU_NULL}
#endif    
};

const GUIMENU_ITEM_T menu_asp_set_music_opt[] =
{
    {MMIASP_CUSTOM_RING_FIXED,		TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_RING_FIXED, 0, 0, 0, MENU_NULL},
    {MMIASP_CUSTOM_RING_DOWNLOAD,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_MORE_RING, 0, 0, 0, MENU_NULL},
    {MMIASP_CUSTOM_RING_RECORDONE,	TIP_NULL, {TXT_COMMON_OK, TXT_NULL, STXT_RETURN},TXT_MMIASP_RECORD_ANSWER_WORD, 0, 0, 0, MENU_NULL}
};
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
_ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T asp_menu_table[] = 
{
    {NULL},
    #include "Mmiasp_menutable.def"
};

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINE                                    */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : Register Input method menu group
//    Global resource dependence : none
//  Author: byte.guo
//    Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_ASP, asp_menu_table);
}

