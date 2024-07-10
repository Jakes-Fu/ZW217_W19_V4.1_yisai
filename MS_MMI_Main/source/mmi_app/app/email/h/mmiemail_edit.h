/****************************************************************************
** File Name:      mmiemail_edit.h                                         *
** Author:                                                          			 *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/
#ifndef _MMIEMAIL_EDIT_H_
#define _MMIEMAIL_EDIT_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "guianim.h"
#include "mmiemail_def.h"
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#include "mmisrvfmm_export.h"
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

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef enum
{
    MMIEMAIL_NO_ERROR = 0,
    MMIEMAIL_ERROR_FATAL,
    MMIEMAIL_ERROR_FAIL,
    MMIEMAIL_ERROR_INVALID_ADDR,
    MMIEMAIL_ERROR_MAX
}MMIEMAIL_ERROR_E;

typedef enum
{
    MMIEMAIL_EDIT_FROM_NEW,
    MMIEMAIL_EDIT_FROM_DRAFT,
    MMIEMAIL_EDIT_FROM_REPLY,
    MMIEMAIL_EDIT_FROM_REPLYALL,
    MMIEMAIL_EDIT_FROM_FORWARD,
    MMIEMAIL_EDIT_FROM_FORWARD_WITHOUT_ATTACH,
    MMIEMAIL_EDIT_FROM_VIEWSEND,
    MMIEMAIL_EDIT_FROM_PB_SEND,
    MMIEMAIL_EDIT_FROM_VIEW_EDIT,
    MMIEMAIL_EDIT_FROM_MAX
} MMIEMAIL_EDIT_REASON_TYPE_E;

typedef enum
{
    MMIEMAIL_EDIT_MENU_SEND = 1,
        MMIEMAIL_EDIT_MENU_ADD_RECV,
        MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT,
        MMIEMAIL_EDIT_MENU_CANCLE,
        MMIEMAIL_EDIT_MENU_ADD_CC,
        MMIEMAIL_EDIT_MENU_ADD_BCC,
        MMIEMAIL_EDIT_MENU_SEND_OPTION,
        MMIEMAIL_EDIT_MENU_ADD_ATTCHMENT,
        MMIEMAIL_EDIT_MENU_REMOVE_FILE,
        MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM,
        MMIEMAIL_EDIT_SENDOPT_PRIORITY,
        MMIEMAIL_EDIT_READCON_START,
        MMIEMAIL_EDIT_READCON_CANCLE,
        MMIEMAIL_EDIT_PRIORITY_LOW,
        MMIEMAIL_EDIT_PRIORITY_NORMAL,
        MMIEMAIL_EDIT_PRIORITY_HIGH,
        MMIEMAIL_EDIT_EXIT_WITHOUTSAVED,
        MMIEMAIL_EDIT_MENU_SET_CC_BCC,
        MMIEMAIL_EDIT_MENU_MAX
}MMIEMAIL_EDIT_MENU_E;
typedef struct mmiemail_edit_document_tag
{
    MMI_STRING_T        edit_to;
    MMI_STRING_T        edit_cc;
    MMI_STRING_T        edit_bcc;
    MMI_STRING_T        edit_subject;
    MMI_STRING_T        edit_text;
    EMAIL_ATTACH_T      *attach_list_ptr;
    uint32				attach_num;
}MMIEMAIL_EDIT_DOCUMENT_T;

typedef struct 
{
    BOOLEAN is_has_cc;
    BOOLEAN is_has_bcc;
    uint16 focus_index; 
    MMIEMAIL_EDIT_MENU_E edit_lose_focus_reason;    //lose focus reason
    MMIEMAIL_EDIT_REASON_TYPE_E email_edit_enter_type;           //email entry type
    MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr;   
    MMIEMAIL_EDIT_DOCUMENT_T *edit_back_ptr;   
    EMAIL_MSGDATACACHE_T     data_cache;
}MMIEMAIL_EDIT_T;
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: create edit window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_REASON_TYPE_E type, void *data_ptr);

/*****************************************************************************/
// 	Description: send email 
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_SelectNet(void);

/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_GetRichtextRect(GUI_RECT_T *rect_ptr);

/*****************************************************************************/
// 	Description: clear email cache
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_ClearEmailCache(EMAIL_MSGDATACACHE_T *data_cache_ptr);

/*****************************************************************************/
// 	Description: get all recipient 
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_GetEditRecipients
(
 wchar *addr_ptr,
 wchar **add_addr_ptr,
 void *mem_mgr_ptr,
 uint32  num
);


/*****************************************************************************/
//  Description :check the email address is Legality
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditCheckFieldLegality(wchar* addr_ptr);

/*****************************************************************************/
//  Description :proceed send email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_EditProcSendEmail(uint32 param);


//*****************************************************************************/
//  Description :send email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditSendEmail(void);

//*****************************************************************************/
//  Description :save as draft
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditSaveAsDraft(void);

//*****************************************************************************/
//  Description :handle save as draft result
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIEMAIL_HandleEditSaveAsDraftResult(uint32 result);

//*****************************************************************************/
//  Description :handle edit send email result
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_HandleEditSendEmailResult(uint32 result);
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
/*****************************************************************************/
// 	Description: EDIT add attachment
//	Global resource dependence:
//  Author:qiang.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_EditAddAttachment( 
                                          MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T *pb_edit_doc_ptr,
                                          MMIFILE_FILE_INFO_T   *file_info_ptr,
                                          MMIFMM_FILE_TYPE_E  file_type);
#endif
/**------------------------------------------------------------------------------------*
**                         Compiler Flag                                               *
**------------------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* _MMIEMAIL_EDIT_H*/
