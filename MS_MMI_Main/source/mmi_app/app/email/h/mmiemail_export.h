/***************************************************************************************
** File Name:      mmiemail_export.h                                                   *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This file defines the function about nv                             *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAIL_EXPORT_H_
#define _MMIEMAIL_EXPORT_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
#include "mmisrvfmm_export.h"
#include "mmiemail_def.h" 
#endif
/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern  "C"
{
#endif

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
/* str opts */
#define EMA_OPT_WSTR_START          0x80
#define EMA_OPT_END                 0x00
    
#define EMA_OPT_TO                   (EMA_OPT_WSTR_START + 0x01)
#define EMA_OPT_CC                   (EMA_OPT_WSTR_START + 0x02)
#define EMA_OPT_BCC                  (EMA_OPT_WSTR_START + 0x03)
#define EMA_OPT_SUBJECT              (EMA_OPT_WSTR_START + 0x04)
#define EMA_OPT_BOBY                 (EMA_OPT_WSTR_START + 0x05)

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef enum
{
    EMAIL_SIG_BEGIN = 0x9000,
    EMAIL_SIG_TRANS_RESULT,
    EMAIL_SIG_NEW_MESSAGE,
    EMAIL_SIG_MAILBOX_GETED,
    EMAIL_SIG_MESSAGE_SIZE_NOTIFY,
    EMAIL_SIG_MESSAGE_NUMBER_NOTIFY,
    EMAIL_SIG_INFO_NOTIFY,
    EMAIL_SIG_REGEX_CALLBACK,
    EMAIL_SIG_DELETEALL_BEGIN = 0x9010,
    EMAIL_SIG_DELETEALL_RESULT,
    EMAIL_SIG_DELETEALL_NOTIFY,
    EMAIL_SIG_FACTORY_RESET_RESULT,
    EMAIL_SIG_DELETE_ACCOUNT_RESULT,
    EMAIL_SIG_UPDATE_STATE_RESULT,
    EMAIL_SIG_SAVE_EMAIL_RET,
    EMAIL_SIG_SEND_EMAIL_RET,
    EMAIL_SIG_END
} EMAIL_SIGNAL_E;

/* defines of EMAOpt struct */
typedef struct
{
	unsigned char  opt_id;
	char           res[3];
	void          *val_ptr;
}EMAOPT_T;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
typedef struct mmiemail_pb_send_vcard_document_tag
{
    MMI_STRING_T        edit_to;
    MMI_STRING_T        edit_cc;
    MMI_STRING_T        edit_bcc;
    MMI_STRING_T        edit_subject;
    MMI_STRING_T        edit_text;
    EMAIL_ATTACH_T      *attach_list_ptr;
    uint32				attach_num;
}MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T;
#endif
/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/*************************************************************************************/
// 	Description : email app init
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC void MMIAPIEMAIL_InitApp(void);

/**************************************************************************************/
// 	Description : entry email module
//	Global resource dependence :
//  Author: 
//	Note:
/**************************************************************************************/
PUBLIC void MMIAPIEMAIL_Entry(void);

/**************************************************************************************/
// 	Description : init email module
//	Global resource dependence :
//  Author: 
//	Note:
/**************************************************************************************/
PUBLIC void MMIAPIEMAIL_InitModule(void);

/*****************************************************************************/
// 	Description : judge email is running
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEMAIL_IsRunning(void);

/*****************************************************************************/
// 	Description : exit form email
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SyncEmail(void);

/*****************************************************************************/
// 	Description : email reset factory
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIEMAIL_ResetFactory(void);

/*************************************************************************************/
// 	Description : entry email edit with mail addr
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC int32 MMIAPIEMAIL_EditEmail(const MMI_STRING_T   *email_addr_to);

/*************************************************************************************/
// 	Description : entry email edit
//	Global resource dependence :
//  Author:
//	Note:
/*************************************************************************************/
PUBLIC int32 MMIAPIEMAIL_EditEmailEx(EMAOPT_T  *opt_ptr);
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
/*****************************************************************************/
//  Description : 设置pb的设置vcard
//  Global resource dependence : g_pb_edit_doc
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SetSendVcardDocument(MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T *pb_edit_doc_ptr);
/*****************************************************************************/
//  Description : 获取pb的设置vcard
//  Global resource dependence : g_pb_edit_doc
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T MMIAPIEMAIL_GetSendVcardDocument(void);
/*****************************************************************************/
//  Description : 设置pb的是否设置vcard
//  Global resource dependence : g_is_pb_send_edit
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_SetIsPbSendEdit(BOOLEAN is_pb_send_edit);
/*****************************************************************************/
//  Description : 获取pb的是否设置vcard
//  Global resource dependence : g_is_pb_send_edit
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEMAIL_GetIsPbSendEdit();
/*****************************************************************************/
//  Description : 通过pb给email发送vcard
//  Global resource dependence : 
//  Author: qiang.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIEMAIL_EditEMAILFromFile(MMIFILE_FILE_INFO_T *file_info_ptr); 
#endif  
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIEMAIL_InitApp	   		MMIAPIEMAIL_InitApp

#define 	MMIEMAIL_Entry   			MMIAPIEMAIL_Entry

#define 	MMIEMAIL_InitModule			MMIAPIEMAIL_InitModule

#define 	MMIEMAIL_IsRunning			MMIAPIEMAIL_IsRunning

#define 	MMIEMAIL_SyncEmail			MMIAPIEMAIL_SyncEmail

#define 	MMIEMAIL_ResetFactory		MMIAPIEMAIL_ResetFactory

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif/* _MMIEMAIL_EXPORT_H_*/
