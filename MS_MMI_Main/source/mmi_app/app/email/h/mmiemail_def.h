/***************************************************************************************
** File Name:      mmiemail_def.h                                                      *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:                                                                        *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create                                              *
**                                                                                     *
***************************************************************************************/
#ifndef _MMIEMAILA_DEF_H_
#define _MMIEMAILA_DEF_H_

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "os_api.h"
#include "mmi_module.h"
//#include "emailapp.h"
//#include "AEEQEEStore.h"

/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/


//#define EMA_NOT_BODY    0x10
//#define EMA_EXIST_BODY  0x11

/* Define Constant Macro Start */
//#define WEBOPT_END      0

//#define EMAILA_SHIELD_INPUT_TIME 1000

#define INBOX_MAX_EMAIL_NUMBER   300
#define DRAFT_MAX_EMAIL_NUMBER   100
#define SENTBOX_MAX_EMAIL_NUMBER 100
#define OUTBOX_MAX_EMAIL_NUMBER  100

#define EMA_SUBJ_REPLY_PREFIX    L"Re:"
#define EMA_SUBJ_REPLY_PREFIX_1  L"RE:"
#define EMA_SUBJ_REPLY_PREFIX_2  L"re:"
#define EMA_SUBJ_FWD_PREFIX      L"Fw:"
#define EMA_SUBJ_FWD_PREFIX_1    L"FW:"
#define EMA_SUBJ_FWD_PREFIX_2    L"fw:"

#define EMA_MAX_RECPT_NUM        10
#define EMA_MAX_ADDR_LEN         256
#define EMA_MAX_SUBJ_LEN         128
#define EMA_MAX_ATTACH_NUM       20
#define EMA_MAX_ATTACHNAME_LEN   1024
#define EMA_MAX_SUBSFOLDER_NUM   20


#define EMA_ACCOUNT_USERINFO_NAME_LENGTH     255
#define EMA_USERINFO_USER_ID_LENGTH          255
#define EMA_POP3_SERVER_PORT_LENGTH          5

#define EMA_MAX_TEXT_BODY_SIZE      20480  /* 20K, not include '\0' */

/* PRIORITY */
#define EMA_PRIORITY_HIGH           0x01
#define EMA_PRIORITY_NORMAL         0x03
#define EMA_PRIORITY_LOW            0x05


/*cancel delete*/
#define EMA_CANCEL_DELETE           0x0000fedc

/*0409*/
//#define EMA_FOLDER_MAX_LENGTH       EMAIL_FOLDER_MAX_LENGTH
#define EMA_FOLDER_MAX_LENGTH       128

#ifndef EMA_USE_SDCARD
#ifndef NANDBOOT_SUPPORT
#define EMA_USE_SDCARD			1
#endif
#endif

#ifdef EMA_USE_SDCARD
#define EMA_PROFILE_ACTIVE_FILE          "E:/Email/account/emlprofileactive.dat"
#define EMA_PROFILE_CUSTOM_FILE          "E:/Email/account/emlpfofilecustom.dat"
#define EMA_PROFILE_ACTIVE_FILE_1        "F:/Email/account/emlprofileactive.dat"
#define EMA_PROFILE_CUSTOM_FILE_1        "F:/Email/account/emlpfofilecustom.dat"
#else
#define EMA_PROFILE_ACTIVE_FILE          "D:/Email/account/emlprofileactive.dat"
#define EMA_PROFILE_CUSTOM_FILE          "D:/Email/account/emlpfofilecustom.dat"
#endif


/*define inner event from extended operation*/ 
#define EVT_EMA_INNER_START                  (MMI_MODULE_EMAIL << 16)
#define EVT_EMA_INNER_END                    ((MMI_MODULE_EMAIL << 16) + 0x100)

#define EVT_EMA_DELETE_EML                   (EVT_EMA_INNER_START + 0x01)
#define EVT_EMA_UPDATE_BOX                   (EVT_EMA_INNER_START + 0x02)
#define EVT_EMA_SEND                         (EVT_EMA_INNER_START + 0x03)
#define EVT_EMA_RESEND                       (EVT_EMA_INNER_START + 0x04)
#define EVT_EMA_RECEIVE                      (EVT_EMA_INNER_START + 0x05)
#define EVT_EMA_WAITING_SENDMAIL             (EVT_EMA_INNER_START + 0x06)
#define EVT_EMA_WAITING_SAVEMAIL             (EVT_EMA_INNER_START + 0x07)
#define EVT_EMA_ASK_SAVEDIALOG               (EVT_EMA_INNER_START + 0x08)
#define EVT_EMA_MOVETODRAFT                  (EVT_EMA_INNER_START + 0x09)
#define EVT_EMA_MARKASREAD                   (EVT_EMA_INNER_START + 0x0A)
#define EVT_EMA_MARKASUNREAD                 (EVT_EMA_INNER_START + 0x0B)
#define EVT_EMA_UPDATE_LISTBOX_TAB           (EVT_EMA_INNER_START + 0x0C)
#define EVT_EMA_UPDATE_LISTBOX_NULL_TAB      (EVT_EMA_INNER_START + 0x0D)
#define EVT_EMA_DELETEALL_TAB                (EVT_EMA_INNER_START + 0x0E)
#define EVT_EMA_DELETEMUL_TAB                (EVT_EMA_INNER_START + 0x0F)
#define EVT_EMA_UPDATE_MAIN_NUM              (EVT_EMA_INNER_START + 0x10)
#define EVT_EMA_SIM_SELECT                   (EVT_EMA_INNER_START + 0x11)
#define EVT_EMA_NOSUBJECT_SEND               (EVT_EMA_INNER_START + 0x12)
#define EVT_EMA_SENDREADREPORT               (EVT_EMA_INNER_START + 0x13)
#define EVT_EMA_ASK_SENDREADREPORT           (EVT_EMA_INNER_START + 0x14)
#define EVT_EMA_ASK_DELETEFILE               (EVT_EMA_INNER_START + 0x15)
#define EVT_EMA_VIEW_UPDATE                  (EVT_EMA_INNER_START + 0x16)
#define EVT_EMA_FACTORY_RESET                (EVT_EMA_INNER_START + 0x17)
#define EVT_EMA_SEND_RET                     (EVT_EMA_INNER_START + 0x18)
#define EVT_EMA_RECEIVE_RET                  (EVT_EMA_INNER_START + 0x19)
#define EVT_EMA_UPDATE_MAIN_TITLE            (EVT_EMA_INNER_START + 0x1A)
#define EVT_EMA_DEL_ACC_RET			         (EVT_EMA_INNER_START + 0x1B)
#define EVT_EMA_MARKED_BY_DATE               (EVT_EMA_INNER_START + 0x1C)
#define EVT_EMA_SEND_FAILED                  (EVT_EMA_INNER_START + 0x1D)	
#if defined(MMI_PDA_SUPPORT)
#define EVT_EMA_MARK_CANCEL                  (EVT_EMA_INNER_START + 0x1E)
#endif
#define EVT_EMA_SAVE_EMAIL_RET               (EVT_EMA_INNER_START + 0x1F)
#define EVT_EMA_SEND_EMAIL_RET               (EVT_EMA_INNER_START + 0x20)
/*define handle SP event*/
#define EVT_EMA_SP_START                     EVT_EMA_INNER_END
#define EVT_EMA_MESSAGE_SIZE_NOTIFY          (EVT_EMA_SP_START + 0x01)
#define EVT_EMA_MESSAGE_NUMBER_NOTIFY        (EVT_EMA_SP_START + 0x02)
#define EVT_EMA_TRANS_RESULT                 (EVT_EMA_SP_START + 0x03)
#define EVT_EMA_NEW_MESSAGE                  (EVT_EMA_SP_START + 0x04)
#define EVT_EMA_MAILBOX_GETED                (EVT_EMA_SP_START + 0x05)
#define EVT_EMA_DELETEALL_RESULT             (EVT_EMA_SP_START + 0x06)
#define EVT_EMA_DELETEALL_NOTIFY             (EVT_EMA_SP_START + 0x07)
/*INBOX*/
#define EMA_BASIC_UNREAD                     0x01
#define EMA_BASIC_UNREAD_HEAD_ONLY           0x02
#define EMA_UNREAD_WITH_ATTACH               0x03
#define EMA_UNREAD_AND_FORWARDED             0x04
#define EMA_UNREAD_WITH_ATTACH_AND_FORWARDED 0x05
#define EMA_UNREAD_AND_REPLIED               0x06
#define EMA_UNREAD_WITH_ATTACH_AND_REPLIED   0x07

#define EMA_BASIC_READ                       0x08
#define EMA_BASIC_READ_HEAD_ONLY             0x09
#define EMA_READ_WITH_ATTACH                 0x0A
#define EMA_READ_AND_FORWARDED               0x0B
#define EMA_READ_WITH_ATTACH_AND_FORWARDED   0x0C
#define EMA_READ_AND_REPLIED                 0x0D
#define EMA_READ_WITH_ATTACH_AND_REPLIED     0x0E
/*OUTBOX*/
#define EMA_SEND_FAILED                      0x0F
#define EMA_SEND_FAILED_WITH_ATTACH          0x11
#define EMA_SEND_FAILED_READ                 0x12
#define EMA_SEND_FAILED_WITH_ATTACH_READ     0x13
/*SENTBOX*/
#define EMA_SENT                             0x14
#define EMA_SENT_WITH_ATTACH                 0x15
#define EMA_SENT_READ                        0x16
#define EMA_SENT_WITH_ATTACH_READ            0x17
/*draft*/
#define EMA_BASIC_DRAFT                      0x18
#define EMA_BASIC_DRAFT_WITH_ATTACH          0x19
#define EMA_BASIC_DRAFT_READ                 0x1A
#define EMA_BASIC_DRAFT_WITH_ATTACH_READ     0x1B
typedef uint32 EMA_MAIL_STATUS;


/*************** TRANS TYPE *********************/
/* SMTP */
#define EMA_TRANS_SMTP_SEND_MAIL                         0x01
   
/* POP3 */
#define EMA_TRANS_POP3_FETCH_NEW_MAILS_HEADER            0x11
#define EMA_TRANS_POP3_FETCH_NEW_MAILS                   0x12
#define EMA_TRANS_POP3_FETCH_SPEC_MAIL                   0x13
#define EMA_TRANS_POP3_DELETE_SPEC_MAIL                  0x14
#define EMA_TRANS_POP3_DELETE_ALL_MAIL                   0x15

/* IMAP4 */
#define EMA_TRANS_IMAP4_FETCH_NEW_MAILS_HEADER           0x21
#define EMA_TRANS_IMAP4_FETCH_NEW_MAILS                  0x22
#define EMA_TRANS_IMAP4_FETCH_SPEC_MAIL                  0x23
#define EMA_TRANS_IMAP4_FETCH_SPEC_MAIL_PART             0x24
#define EMA_TRANS_IMAP4_MARK_DEL_ON_SERVER               0x25
#define EMA_TRANS_IMAP4_UNMARK_DEL_ON_SERVER             0x26
#define EMA_TRANS_IMAP4_MARK_SEEN_ON_SERVER              0x27
#define EMA_TRANS_IMAP4_UNMARK_SEEN_ON_SERVER            0x28
#define EMA_TRANS_IMAP4_COPY_MAIL_TO_MAILBOX             0x29
#define EMA_TRANS_IMAP4_UPLOAD_MAIL_TO_SERVER            0x2A
#define EMA_TRANS_IMAP4_EXPUNGE_DEL_MAIL_IN_MAILBOX      0x2B
#define EMA_TRANS_IMAP4_CREAT_MAILBOX                    0x2C
#define EMA_TRANS_IMAP4_RENAME_MAILBOX                   0x2D
#define EMA_TRANS_IMAP4_DELETE_MAILBOX                   0x2E
#define EMA_TRANS_IMAP4_SUBSCRIBE_MAILBOX                0x2F
#define EMA_TRANS_IMAP4_UNSUBSCRIBE_MAILBOX              0x30
#define EMA_TRANS_IMAP4_LIST_SUBSCRIBED_MAILBOXES        0x31
#define EMA_TRANS_IMAP4_LIST_ALL_MAILBOXES               0x32

/*Setting*/
//#define EMA_SETTING_ACOUNT_CREATE                        0x40

typedef uint32  EMA_TRANS_TYPE;

#define EMA_MSGPART_CONTENT_TYPE_TEXT_PLAIN  0x00000001
#define EMA_MSGPART_CONTENT_TYPE_TEXT_HTML   0x00000002
#define EMA_MSGPART_CONTENT_TYPE_ATTACHMENT  0x00000003
#define EMA_MSGPART_CONTENT_TYPE_MAX         0xffffffff
typedef uint32 EMA_MSGPART_CONTENTTYPE;

/*************** Trans Id *********************/
#define EMA_ALL_TRANS 0xffffffff
//#define EMA_PROGRESS_INDI_MASK 0x80000001

/*************** Sort by *********************/
#define  EMA_BOXS_ORDERBY_SENDER        1
#define  EMA_BOXS_ORDERBY_RECEIVER      2
#define  EMA_BOXS_ORDERBY_DATE          3
#define  EMA_BOXS_ORDERBY_SUBJECT       4
#define  EMA_BOXS_ORDERBY_UNREAD        5
#define  EMA_BOXS_ORDERBY_SIZE_LARGE    6
#define  EMA_BOXS_ORDERBY_SIZE_SMALL    7
#define  EMA_BOXS_ORDERBY_REC_NEW       8
#define  EMA_BOXS_ORDERBY_REC_OLD       9
#define  EMA_BOXS_ORDERBY_PRO_HIGH      10
#define  EMA_BOXS_ORDERBY_PRO_LOW       11

#define  EMA_SETTING_FOR_ONE_MSG   0xffffff

#define MMIEMAIL_FILE_NAME_LEN		255
#define MMIEMAIL_MAX_ADDRESS_LEN 	EMA_MAX_ADDR_LEN
#define MMIEMAIL_ALL_ADDRESS_LEN    (EMA_MAX_RECPT_NUM * EMA_MAX_ADDR_LEN)
#define MMIEMAIL_MAX_ONE_EMAIL_SIZE (2 * 1024 * 1024)


#define EMA_TRANS_TYPE_SEND_EMAIL        0
#define	EMA_TRANS_TYPE_RETRIEVE_EMAIL    1
#define EMA_TRANS_TYPE_SEND_RR           2

/*-------------------------------------------------------------------
-------------------------------------------------------------------*/
typedef enum
{
    E_EMAILA_MAILSERV_POP3 = 0x00000051,
    E_EMAILA_MAILSERV_IMAP4 = 0x00000052
}EMAIL_MAILSERVTYPE_E;

typedef uint32  BOX_ID;

typedef struct email_msgdatacache_tag
{
    uint32           email_id;
    uint32           orig_email_id;
    uint32           act_id;
    uint32           folder_id;
    uint32           email_size; 
    int              to_addrs_num;
    int              cc_addrs_num;
    int		         bcc_addrs_num;
    int              attach_num;
    EMA_MAIL_STATUS  mail_status;
    uint32			 orig_email_status;
    uint32           uipriority;
    uint32           read_confirm;
    uint32           delivery_confirm;
    uint32           be_down;
    uint32           view_type;
    wchar           *read_report_ptr;
    wchar            from_addrs[EMA_MAX_ADDR_LEN + 1];
    wchar            to_addrs[EMA_MAX_RECPT_NUM][EMA_MAX_ADDR_LEN + 1];
    wchar            cc_addrs[EMA_MAX_RECPT_NUM][EMA_MAX_ADDR_LEN + 1];
    wchar            bcc_addrs[EMA_MAX_RECPT_NUM][EMA_MAX_ADDR_LEN + 1];
    wchar            attach_uri[EMA_MAX_ATTACH_NUM][EMA_MAX_ATTACHNAME_LEN + 1];
    wchar            attach_name[EMA_MAX_ATTACH_NUM][EMA_MAX_ATTACHNAME_LEN + 1];
    wchar            attach_section_id[EMA_MAX_ATTACH_NUM][EMA_MAX_ATTACHNAME_LEN + 1];
    wchar           *attach_size_ptr[EMA_MAX_ATTACH_NUM];
    wchar           *subject_ptr;
    wchar           *data_buff_ptr;
    char			*source_buff_ptr;
    wchar            from_addr_name[EMA_MAX_ADDR_LEN + 1];
    uint32           uidate;
}EMAIL_MSGDATACACHE_T;

typedef struct email_emncache_tag
{
    uint32 auth_type;
    uint32 port;
    uint32 uri_type;
    uint32 utc_time;
    char  *user_name_ptr;
    char  *host_ptr;
    char  *mail_box_name_ptr;
}EMAIL_EMNCACHE_T;

typedef enum
{
    EMA_INBOX = 1,
    EMA_OUTBOX,
    EMA_SENTBOX,
    EMA_DRAFTBOX,
    EMA_BOX_MAX = 0x7fffffff
}EMAIL_BOXTYPE_E;

typedef struct email_msghead_tag
{
    uint32           email_type;
    uint32           email_id;
    uint32           act_id;
    uint32           dwdate;
    wchar            *wsz_subject_ptr;
    wchar            *wsz_from_ptr;
    wchar            *wsz_to_ptr;
    EMA_MAIL_STATUS  mail_status;
    unsigned int     email_size;
    unsigned int     email_prio;
}EMAIL_MSGHEAD_T;

typedef struct email_boxname_actid_tag
{
    char   *box_name_ptr;
    uint32  act_id;
}EMAIL_BOXNAME_ACTID_T;

typedef struct email_box_vectormodel_tag
{
    void   *data_vector_ptr;
    void   *listwdg_vector_ptr;
    uint32  deleteing_msg_num;
    uint32  delete_result;
    uint32  total_msg_num;
}EMAIL_BOX_VECTORMODEL_T;

/*
 *	Below struct is passed by SP and will be used in formmgr
 */
typedef struct email_transresult_tag
{
    uint32   trans_id;
    uint32   trans_type;
    uint32   email_id;
    int32    iresult;
    char     resp_desc[512];
}EMAIL_TRANSRESULT_T;

typedef struct email_msg_tag
{
    uint32   trans_id;
    uint32   trans_type;
    char    *uidl_ptr;
    char    *emailid_ptr;
}EMAIL_MSG_T;

typedef struct email_msg_size_notify_tag
{
    uint32  trans_id;
    uint32  trans_type;
    uint32  trans_fered_size;
    uint32  total_size;
}EMAIL_MSG_SIZE_NOTIFY_T;

typedef struct email_msg_nomber_notify_tag
{
    uint32  trans_id;
    uint32  trans_type;
    uint32  curr_email;
    uint32  total_email;
}EMAIL_MSG_NUMBER_NOTIFY_T;

typedef struct email_folder_name_tag
{
    wchar   *all_folder_name_ptr[EMA_MAX_SUBSFOLDER_NUM];
    uint32   folder_num;
    BOOLEAN  is_subs_folder[EMA_MAX_SUBSFOLDER_NUM];
}EMAIL_FOLDER_NAME_T;

typedef struct email_subbox_info_tag
{
    uint32    box_id;
    uint32    num; // for Inbox is unread num ,other is total
    wchar     wsz_folder[EMA_FOLDER_MAX_LENGTH +1];
    BOOLEAN   is_ser;
    char      ac_res[1];
}EMAIL_SUBBOX_INFO_T;

typedef struct email_attach_tag
{
    wchar				       file_path[MMIEMAIL_FILE_NAME_LEN + 1]; //content file name
    wchar                      *file_name_ptr;
    uint32		               type;			                     //content type int value
    uint32                     file_size;                            //content size
    struct email_attach_tag    *next_ptr;                            //net accessory info
}EMAIL_ATTACH_T;

typedef struct email_update_state_tag
{
    unsigned int emailid;
    unsigned int state;
}EMAIL_UPDATE_STATE_T;

typedef struct email_result_tag
{
    SIGNAL_VARS 
    uint32* result;
}EMAIL_RESULT_T;

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : Release EMAIL_MSGDATACACHE_T struct.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_ReleaseEmailDataCache(void *mem_mgr_ptr, EMAIL_MSGDATACACHE_T *data_cache_ptr);
/**************************************************************************************/
// Description : Copy EMAIL_MSGDATACACHE_T function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyEmailDataCache(EMAIL_MSGDATACACHE_T *dest_data_cache_ptr, EMAIL_MSGDATACACHE_T *src_data_cache_ptr, void *mem_mgr_ptr);
/**************************************************************************************/
// Description : Copy St_EMAALLFolderName function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyAllFolderName(EMAIL_FOLDER_NAME_T *dest_data_cache_ptr, EMAIL_FOLDER_NAME_T *src_data_cache_ptr, void *mem_mgr_ptr);
/**************************************************************************************/
// Description : Copy  EMAMsgHead function.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_CopyEmailHead(EMAIL_MSGHEAD_T *dest_data_cache_ptr, EMAIL_MSGHEAD_T *src_data_cache_ptr, void *mem_mgr_ptr);
/**************************************************************************************/
// Description : Get all recipients for EMAIL_MSGDATACACHE_T.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC int32 MMIEMAIL_GetAllRecipients(void *mem_mgr_ptr, EMAIL_MSGDATACACHE_T *data_cache_ptr);

PUBLIC uint32 MMIEMAIL_GetEmailTaskPriority(void);

#endif /* _MMIEMAILA_DEF_H_*/
