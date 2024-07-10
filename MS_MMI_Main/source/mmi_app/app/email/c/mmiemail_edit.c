/****************************************************************************
** File Name:      mmiemail_edit.c                                         *
** Author:                                                         	       *
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

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "guirichtext.h"
#include "guimenu.h"
#include "mmifmm_export.h"
#include "mmifmm_id.h"
#include "guibutton.h"
#include "mmisms_id.h"
#include "mmi_default.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "guistring.h"
#include "mmisms_export.h"
#include "gui_ucs2b_converter.h"

//testing start
//#include "mmipic_image.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guires.h"
//test end

#include "mmiemail_id.h"
#include "mmimms_id.h"
#include "sysMemFile.h"
#include "mmimms_internal.h"
#include "mms_image.h"
#include "mmiemail_edit.h"
#include "mmimms_menutable.h"
#include "email_text.h"
#include "email_image.h"
#include "mmiemail_def.h"
#include "mmiemail_menutable.h"
#include "mmiemail_utils.h"

#include "mmiemail_export.h"
#include "mmiemail_sp_api.h"
#include "AEEEMEError.h"
#include "mmiemail_dlg.h"
#include "mmiemail_box.h"
#include "mmiemail_signal.h"

#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif

#include "mmiudisk_export.h"
#include "mmiemail_net.h"
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#include "mmiemail_setting.h"
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
#include "mmifmm_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIEMAIL_RICHTEXT_ITEM_NUM_MAX  26
#define MMIEMAIL_ATTCHMENT_NUM_MAX      20
#define MMIEMAIL_FILESIZE_OFFSET		(1024 * 1024)
#ifdef CMCC_UI_STYLE
#define MMIEMAIL_FILESIZE_MAX           2024
#else
#define MMIEMAIL_FILESIZE_MAX           1024//1536
#endif

#define EMA_SIGN_NEWLINE_NUMBER_UP      1
#define EMA_SIGN_NEWLINE_NUMBER_DOWN    1
#define EMA_SIGN_DASHED                 25

#define EDIT_TEXT_RECT_TOP              (MMI_TITLE_HEIGHT - 11)
#define EDIT_TEXT_RECT_BOTTOM           (MMI_TITLE_HEIGHT - 1)
#define EDIT_TEXT_RECT_LEFT             ((int16)MMITHEME_GetClientRectWidth() - 50)
#define EDIT_TEXT_RECT_RIGHT            ((int16)MMITHEME_GetClientRectWidth() - 5)
 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

typedef enum
{
    MMIEMAIL_EDIT_RICHTEXT_TO,                   //0
    MMIEMAIL_EDIT_RICHTEXT_CC,                   //0
    MMIEMAIL_EDIT_RICHTEXT_BCC,                  //0
    MMIEMAIL_EDIT_RICHTEXT_SUBJECT,              //1
    MMIEMAIL_EDIT_RICHTEXT_ADDFILE,              //2
    MMIEMAIL_EDIT_RICHTEXT_TEXT,                 //3
    MMIEMAIL_EDIT_RICHTEXT_MAX
}MMIEMAIL_EDIT_RICHTEXT_INDEX_E; 


/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern HEmlSPMgr  g_email_sp_handle;
extern BOOLEAN    g_email_is_use_wifi;
uint32            g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
BOOLEAN           g_save_email_on_red_key = FALSE;
extern BOOLEAN    g_is_email_by_pb;
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIEMAIL_EDIT_RICHTEXT_INDEX_E s_email_edit_richtext_type[MMIEMAIL_RICHTEXT_ITEM_NUM_MAX] = {
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX,
    MMIEMAIL_EDIT_RICHTEXT_MAX,MMIEMAIL_EDIT_RICHTEXT_MAX};

MMIEMAIL_EDIT_T *s_email_edit_p = PNULL;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : add contact to list callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void AddContactToEditListCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description :remove attachment
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditRemoveAttachment(void);

/*****************************************************************************/
//  Description :exit edit window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditExit(void);

/*****************************************************************************/
//  Description :handle edit window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditWinMsg(
                                     EMAIL_WINDOW_ID_E win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     );

/*****************************************************************************/
//  Description :handle edit send option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSendOptMenuWinMsg(
                                                EMAIL_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//  Description :handle edit menu window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditMenuWinMsg(
                                         EMAIL_WINDOW_ID_E win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
//  Description :handle edit to list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditToListWinMsg(
                                           EMAIL_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description :handle edit cc list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditCcListWinMsg(
                                           EMAIL_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description :handle edit bcc list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBccListWinMsg(
                                            EMAIL_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description :handle edit subject list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSubjectWinMsg(
                                            EMAIL_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description :handle edit text window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTextWinMsg(
                                         EMAIL_WINDOW_ID_E win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         );

/*****************************************************************************/
//  Description :get receive number from str
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetReceiverNumberFromStr(MMI_STRING_T * to_str);

/*****************************************************************************/
//  Description :add to item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddToItem(void);


/*****************************************************************************/
//  Description :add cc item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddCcItem(void);


/*****************************************************************************/
//  Description :add bcc item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddBccItem(void);

/*****************************************************************************/
//  Description :add file item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddAddFileItem(
                              wchar * file_name_ptr,
                              uint32 file_size
                              );

/*****************************************************************************/
//  Description :add subject item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddSubjectItem(void);

/*****************************************************************************/
//  Description :add text item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddTextItem(void);

/*****************************************************************************/
//  Description :add all edit item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddAllEditItems(void);

/*****************************************************************************/
//  Description :create edit menu window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditMenuWin(void);

/*****************************************************************************/
//  Description :create send option menu window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateSendOptMenuWin(void);

/*****************************************************************************/
//  Description :insert attachment
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditInsertAttachment(
                                EMAIL_WINDOW_ID_E pre_win_id,
                                MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr,
                                BOOLEAN if_replace
                                );

/*****************************************************************************/
//  Description :enter correspond item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void  EditEnterCorrespondItem(
                                    MMI_CTRL_ID_T ctrl_id,
                                    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr
                                    );

/*****************************************************************************/
//  Description :set list box string
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ListBoxSetString(
                            MMI_CTRL_ID_T ctrl_id,
                            const wchar *str_ptr,
                            const uint16 str_len
                            );

/*****************************************************************************/
//  Description :insert node
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,       //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,           //位置
#if defined(MMI_PDA_SUPPORT)//@tao.xue
                             MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
#endif
                             BOOLEAN            is_graged
                             );


/*****************************************************************************/
//  Description :get edit richtext index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetEditItemIndex(MMIEMAIL_EDIT_RICHTEXT_INDEX_E item_type);

/*****************************************************************************/
//  Description :set edit content
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 SetEditContent(
                           MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr,
                           EMAIL_MSGDATACACHE_T *data_cache_ptr
                           );

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :handle location option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailEditOptWinMsg(
                                          EMAIL_WINDOW_ID_E win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//edit window
WINDOW_TABLE(EMAIL_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditWinMsg),    
    WIN_ID(EMAIL_EDIT_WIN_ID),
    WIN_TITLE(TXT_WRITE_MAIL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(EMAIL_EDIT_CTRL_ID),
    END_WIN
};

//to edit box
WINDOW_TABLE(EMAIL_EDIT_TO_LIST_WIN_TAB) = 
{
    WIN_TITLE(TXT_COMMON_RECIPIENT),
    WIN_FUNC((uint32)HandleEditToListWinMsg),    
    WIN_ID(EMAIL_EDIT_TO_LIST_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_EDIT_TO_LIST_FORM_CTRL_ID),  
//    CHILD_EDIT_LIST_CTRL(TRUE,EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1,EMAIL_EDIT_TO_LIST_CTRL_ID,EMAIL_EDIT_TO_LIST_FORM_CTRL_ID),   
//    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,EMAIL_EDIT_TO_LIST_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
    CREATE_EDIT_LIST_CTRL(EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1, EMAIL_EDIT_TO_LIST_CTRL_ID),
    END_WIN
};

//cc edit box
WINDOW_TABLE(EMAIL_EDIT_CC_LIST_WIN_TAB) = 
{
    WIN_TITLE(TXT_EMAIL_RECIPIENT_CC),
    WIN_FUNC((uint32)HandleEditCcListWinMsg),    
    WIN_ID(EMAIL_EDIT_CC_LIST_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_EDIT_CC_LIST_FORM_CTRL_ID),  
//    CHILD_EDIT_LIST_CTRL(TRUE,EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1,EMAIL_EDIT_CC_LIST_CTRL_ID,EMAIL_EDIT_CC_LIST_FORM_CTRL_ID),   
//    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,EMAIL_EDIT_CC_LIST_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
    CREATE_EDIT_LIST_CTRL(EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1, EMAIL_EDIT_CC_LIST_CTRL_ID),
    END_WIN
};

//bcc edit box
WINDOW_TABLE(EMAIL_EDIT_BCC_LIST_WIN_TAB) = 
{
    WIN_TITLE(TXT_EMAIL_RECIPIENT_BCC),
    WIN_FUNC((uint32)HandleEditBccListWinMsg),    
    WIN_ID(EMAIL_EDIT_BCC_LIST_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_EDIT_BCC_LIST_FORM_CTRL_ID),  
//    CHILD_EDIT_LIST_CTRL(TRUE,EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1,EMAIL_EDIT_BCC_LIST_CTRL_ID,EMAIL_EDIT_BCC_LIST_FORM_CTRL_ID),   
//    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,EMAIL_EDIT_BCC_LIST_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
    CREATE_EDIT_LIST_CTRL(EMA_MAX_RECPT_NUM, EMA_MAX_ADDR_LEN - 1, EMAIL_EDIT_BCC_LIST_CTRL_ID),
    END_WIN
};

//subject edit box
WINDOW_TABLE(EMAIL_EDIT_SUBJECT_TAB) = 
{
    WIN_TITLE(TXT_COMMON_SUBJECT),
    WIN_FUNC((uint32)HandleEditSubjectWinMsg),    
    WIN_ID(EMAIL_EDIT_SUBJECT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_EDIT_SUBJECT_FORM_CTRL_ID),  
//    CHILD_EDIT_TEXT_CTRL(TRUE,EMA_MAX_SUBJ_LEN,EMAIL_EDIT_SUBJECT_CTRL_ID,EMAIL_EDIT_SUBJECT_FORM_CTRL_ID),   
//    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,EMAIL_EDIT_SUBJECT_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(EMA_MAX_SUBJ_LEN, EMAIL_EDIT_SUBJECT_CTRL_ID),
    END_WIN
};

//text edit box
WINDOW_TABLE(EMAIL_EDIT_TEXT_TAB) = 
{
    WIN_TITLE(TXT_TEXT),
    WIN_FUNC((uint32)HandleEditTextWinMsg),    
    WIN_ID(EMAIL_EDIT_TEXT_WIN_ID),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
//    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_EDIT_TEXT_FORM_CTRL_ID),  
//    CHILD_EDIT_TEXT_CTRL(TRUE,EMA_MAX_TEXT_BODY_SIZE/sizeof(wchar),EMAIL_EDIT_TEXT_CTRL_ID,EMAIL_EDIT_TEXT_FORM_CTRL_ID),   
//    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,EMAIL_EDIT_TEXT_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(EMA_MAX_TEXT_BODY_SIZE/sizeof(wchar),EMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};


//edit menu
WINDOW_TABLE(EMAIL_EDIT_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditMenuWinMsg),
    WIN_ID(EMAIL_EDIT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(EMAIL_EDIT_SENDOPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEditSendOptMenuWinMsg),
    WIN_ID(EMAIL_EDIT_SENDOPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN 
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(EMAIL_EDIT_ATTACH_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEmailEditOptWinMsg),
        WIN_ID(EMAIL_EDIT_LOCATE_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_EDIT_ATTACH_OPT, EMAIL_EDIT_LOCATE_OPT_CTRL_ID),
        END_WIN
};
#endif


/*****************************************************************************/
//	Description : add contact to list callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void AddContactToEditListCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(EMAIL_EDIT_TO_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle); 
}

/*****************************************************************************/
//	Description : add contact to list callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void AddContactToCcListCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(EMAIL_EDIT_CC_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle); 
}

/*****************************************************************************/
//	Description : add contact to bcc list callback
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void AddContactToBccListCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(EMAIL_EDIT_BCC_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle); 
}
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
/*****************************************************************************/
//	Description : add file path is vailble
//	Global resource dependence : 
//	Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AddFilePathCheckVaild(EMAIL_ATTACH_T  *add_file)
{
    uint8 result =0;
    wchar file_path[2] = {0};
    MMI_WSTRNCPY(file_path,2,add_file->file_path,MMIEMAIL_FILE_NAME_LEN + 1,1);

    if( 0==file_path[0] -'C')
        return TRUE;
    else if(0==file_path[0] -'D')
        return TRUE;
    else if(0==file_path[0] -'E')
        return TRUE;
    else if(0==file_path[0] -'F')
        return TRUE;
    else
        return FALSE;
}
#endif
/*****************************************************************************/
//  Description :add signature
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditAddSignature(
                             wchar **ppwszSignature
                             )
{
   char     *dir_ptr = PNULL;
   wchar	*str_temp_ptr= PNULL;
   char     *temp_ptr = PNULL;
   SFS_HANDLE file = 0;
   uint32    file_size = 0;
   uint32	 read_size = 0;
   int32     result = EMAIL_SUCCESS;
   uint32    len = 0;
   int32     i = 0;
   int32     ret = 0;

   do 
   {
      if  (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetSignDir(EMA_GETCFG_HANDLE(g_email_sp_handle),
                                             EMA_SP_ACTIVE_ACCOUNT, &dir_ptr))
      {
         EMA_UTILS_LOG_ERROR(("EditAddSignature, MMIEMAIL_SP_CFG_GetSignDir failed."));
         return EMAIL_ERROR;
      }

	  str_temp_ptr= EMA_UTILS_STRTOWSTR(PNULL, dir_ptr);
	  EMA_UTILS_BREAKNULL(str_temp_ptr, ret, "out of memory");

      file =MMIAPIFMM_CreateFile(str_temp_ptr, SFS_MODE_OPEN_EXISTING | SFS_MODE_READ, PNULL, PNULL);
      if (file)
      {
         file_size = MMIAPIFMM_GetFileSize(file);
         /* signature only have one '/0' */
         if (file_size < 1)
         {
            result = EMAIL_SUCCESS;
            break;
         }
         len = file_size + EMA_SIGN_DASHED + 2 * EMA_SIGN_NEWLINE_NUMBER_UP + 2 * EMA_SIGN_NEWLINE_NUMBER_DOWN + 2;
         temp_ptr = EMA_UTILS_MALLOC(PNULL, len);
         EMA_UTILS_BREAKNULL(temp_ptr, result, "EditAddSignature, malloc temp_ptr failed!");
         EMA_UTILS_MEMSET(temp_ptr, 0, len);

         // Add newline 
         for (i = 0; i < EMA_SIGN_NEWLINE_NUMBER_UP; i++)
         {
            EMA_UTILS_STRCAT(temp_ptr, "\r\n");
         }
         // Add dashed
         for (i = 0; i < EMA_SIGN_DASHED; i++)
         {
            EMA_UTILS_STRCAT(temp_ptr, "-");
         }
         // Add newline
         for (i = 0; i < EMA_SIGN_NEWLINE_NUMBER_DOWN; i++)
         {
            EMA_UTILS_STRCAT(temp_ptr, "\r\n");
         }
         MMIAPIFMM_ReadFile(file, temp_ptr + 2 * EMA_SIGN_NEWLINE_NUMBER_UP + EMA_SIGN_DASHED + 2 * EMA_SIGN_NEWLINE_NUMBER_DOWN, len,
			 &read_size, 0);
			//IFILE_Read(file, temp_ptr, len);
         (*ppwszSignature) = EMA_UTILS_UTF8TOWSTR(PNULL, temp_ptr);
         EMA_UTILS_FREE(PNULL, temp_ptr);    
      }
      EMA_UTILS_FREEIF(PNULL, dir_ptr);
	  EMA_UTILS_FREEIF(PNULL, temp_ptr);

   } while(0);
   if (EMAIL_SUCCESS != result)   
	{      
      EMA_UTILS_FREEIF(PNULL, temp_ptr);
      EMA_UTILS_FREEIF(PNULL, (*ppwszSignature));
   }

   if (PNULL != file)
   {
	   MMIAPIFMM_CloseFile(file);
   }
	EMA_UTILS_FREEIF(PNULL, dir_ptr);
#undef  EMA_SIGN_NEWLINE_NUMBER_UP   
#undef  EMA_SIGN_NEWLINE_NUMBER_DOWN 
#undef  EMA_SIGN_DASHED
     
   return result;
}
/*****************************************************************************/
// 	Description: check recipients 
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
LOCAL int32 EditCheckRecipients(void)
{
	int32  result					      = 0;
	MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;

	edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
	
	if (PNULL != edit_doc_ptr)
	{
		if (0 != edit_doc_ptr->edit_to.wstr_len)
		{
			result = MMIEMAIL_EditCheckFieldLegality(edit_doc_ptr->edit_to.wstr_ptr);
			if (result == MMIEMAIL_ERROR_INVALID_ADDR)
			{
				MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
				return EMAIL_ERROR;
			}
		}
		
		if (0 != edit_doc_ptr->edit_cc.wstr_len)
		{
			result = MMIEMAIL_EditCheckFieldLegality(edit_doc_ptr->edit_cc.wstr_ptr);
			if (result == MMIEMAIL_ERROR_INVALID_ADDR)
			{
				MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
				return EMAIL_ERROR;
			}
		}
		
		if (0 != edit_doc_ptr->edit_bcc.wstr_len)
		{
			result = MMIEMAIL_EditCheckFieldLegality(edit_doc_ptr->edit_bcc.wstr_ptr);
			if (result == MMIEMAIL_ERROR_INVALID_ADDR)
			{
				MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
				return EMAIL_ERROR;
			}
		}
	}

	return result;
}

/*****************************************************************************/
//  Description :proceed send email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_EditProcSendEmail(uint32 param)
{
    EMAIL_MSGDATACACHE_T *data_cache_ptr = (EMAIL_MSGDATACACHE_T *)param;
    int result = EMAIL_SUCCESS;
    
    if (PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, bad param."));
        return FALSE;
    }
    
    if (0 == data_cache_ptr->email_id)
    {
        if (EMAIL_SUCCESS != (result = MMIEMAIL_SP_Store_SaveEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), data_cache_ptr)))
        {
            MMK_CloseWin(EMAIL_SENDING_WIN_ID);
            if (result == EME_NOSTORAGE)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
            }
            else if (result == EME_NOMEMORY || result == EMAIL_NOMEMORY)/*No memory*/
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_COMM_SEND_FAILED);
            }
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, MMIEMAIL_SP_Store_SaveEmail failed."));
            return FALSE;
        }
    }
    else
    {
        result = MMIEMAIL_SP_Store_GetBoxIdByEmailId(EMA_GETSTORE_HANDLE(g_email_sp_handle), data_cache_ptr->email_id, EMA_SP_ACTIVE_ACCOUNT, &data_cache_ptr->folder_id);
        if (EMAIL_SUCCESS != result)
        {
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, MMIEMAIL_SP_Store_GetBoxIdByEmailId failed."));
            return FALSE;
        }
        
        if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FULL, data_cache_ptr))
        {
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, MMIEMAIL_SP_Store_UpdateEmail failed."));
            return FALSE;
        }
    }
    
    /*Update e-mail information.(folder)*/
    if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_MoveEmailtoFolder(EMA_GETSTORE_HANDLE(g_email_sp_handle),
        EMA_SP_ACTIVE_ACCOUNT,
        data_cache_ptr->folder_id, 
        EMA_SP_ACTIVE_ACCOUNT,
        EMA_OUTBOX,
        data_cache_ptr->email_id))
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, MMIEMAIL_SP_Store_MoveEmailtoFolder failed."));
        return FALSE;
    }
    data_cache_ptr->folder_id = EMA_OUTBOX;
    
    if (data_cache_ptr->attach_num > 0)
    {
        data_cache_ptr->mail_status = EMA_SEND_FAILED_WITH_ATTACH;
    }
    else
    {
        data_cache_ptr->mail_status = EMA_SEND_FAILED;
    }
    
    result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, data_cache_ptr);
    if (SCI_SUCCESS != result)
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_SendEmail, EmailASP_Store_GetBoxIdByMsgId failed."));
        return FALSE;
    }
    
    //    if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_SendMsg(EMA_GETTRANS_HANDLE(g_email_sp_handle), data_cache_ptr->msg_id, EMA_SP_ACTIVE_ACCOUNT))
    //    {
    // 	   EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Sending, MMIEMAIL_SP_TRS_SendMsg failed."));
    // 	   return FALSE;
    //    }
    
    
    return TRUE;
}

/*****************************************************************************/
//  Description :proceed save draft
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditProcSaveDraft(uint32 param)
{
   EMAIL_MSGDATACACHE_T *data_cache_ptr = PNULL;
   int32  result = EMAIL_SUCCESS;

   if (PNULL == param)
   {
      EMA_UTILS_LOG_ERROR(("MMIEmail_SaveDraft, bad param."));
      return EMAIL_ERROR;
   }

   data_cache_ptr = (EMAIL_MSGDATACACHE_T *)param;
   data_cache_ptr->folder_id = EMA_DRAFTBOX;
   if (data_cache_ptr->attach_num > 0)
   {
      data_cache_ptr->mail_status = EMA_BASIC_DRAFT_WITH_ATTACH;
   }
   else
   {
      data_cache_ptr->mail_status = EMA_BASIC_DRAFT;
   }
   if (0 == data_cache_ptr->email_id)
   {
	   data_cache_ptr->act_id = EMA_SP_ACTIVE_ACCOUNT;
	   if (EMAIL_SUCCESS != (result = MMIEMAIL_SP_Store_SaveEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), data_cache_ptr)))
	   {
		   /*No enough space*/
		   if (result == EME_NOSTORAGE)
		   {
			   MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
			   return EMAIL_ERROR;
		   }
		   else if (result == EME_NOMEMORY || result == EMAIL_NOMEMORY)/*No memory*/
		   {
			   MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
			   return EMAIL_ERROR;
		   }
		   EMA_UTILS_LOG_ERROR(("MMIEmail_SaveDraft, MMIEMAIL_SP_Store_SaveEmail failed."));
		   return EMAIL_ERROR;
	   }
   }
   else
   {
      if (EMAIL_SUCCESS != (result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FULL, data_cache_ptr)))
      {
		   /*No enough space*/
		  if (result == EME_NOSTORAGE)
		  {		
			  MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
			  return EMAIL_ERROR;
		  }
		  else if (result == EME_NOMEMORY || result == EMAIL_NOMEMORY)/*No memory*/
		  {
			  MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
			  return EMAIL_ERROR;
		  }
         EMA_UTILS_LOG_ERROR(("MMIEmail_SaveDraft, MMIEMAIL_SP_Store_SaveEmail failed."));
         return EMAIL_ERROR;
      }
   }
   
   return result;
}

/*****************************************************************************/
//  Description :proceed enter edit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditProcEnter(uint32 param)
{
   EMAIL_MSGDATACACHE_T  *data_cache_ptr = PNULL;
   EMAIL_ACT_MSGPREFERENCE_T  preference = {0};
   EMAOPT_T          *opt_ptr = PNULL;
   EMAOPT_T          *temp_ptr = PNULL;

   data_cache_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
   if (PNULL == data_cache_ptr)
   {
      EMA_UTILS_LOG_ERROR(("EditProcEnter, data_cache_ptr is PNULL."));
      return EMAIL_ERROR;
   }
   EMA_UTILS_MEMSET(data_cache_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
   
   data_cache_ptr->act_id = EMA_SP_ACTIVE_ACCOUNT;
   if (data_cache_ptr->attach_num > 0)
   {
      data_cache_ptr->mail_status = EMA_BASIC_DRAFT_WITH_ATTACH;
   }
   else
   {
      data_cache_ptr->mail_status = EMA_BASIC_DRAFT;
   }
   /*
   if (SUCCESS != MMIEMAIL_SP_Store_CreateNew(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_CREATE_NEW, pStEMANewMsgEvtData, data_cache_ptr))
   {
      EMA_ReleaseNewMsgEvtData(PNULL, pStEMANewMsgEvtData);
      EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_Store_CreateNew failed."));
      return FALSE;
   }*/

   /*
   ** Get config about signature, then decide whether need to add signature.
   ** */
   if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle),
                                                EMA_SP_ACTIVE_ACCOUNT, &preference))
   {
      EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_CFG_GetEmailPreference failed."));
      return EMAIL_ERROR;
   }

   switch(preference.signature_mode)
   {
   case EMA_ADD_SIGNATURE_NO_EMAIL:
      break;
   case EMA_ADD_SIGNATURE_ONLY_NEW_EMAIL:
   case EMA_ADD_SIGNATURE_ALL_EMAIL:
      {
			/* add signature.*/
         EditAddSignature(&data_cache_ptr->data_buff_ptr);
      }
      break;
   default:
      break;
   }
   if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), &preference))
   {
      EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
      return EMAIL_ERROR;
   }
   
   // Init uipriority, read_confirm, delivery_confirm
   data_cache_ptr->uipriority = EMA_SETTING_FOR_ONE_MSG;
   data_cache_ptr->read_confirm = EMA_SETTING_FOR_ONE_MSG;
   data_cache_ptr->delivery_confirm = EMA_SETTING_FOR_ONE_MSG;   

   if (PNULL != param)
   {
      opt_ptr = (EMAOPT_T*)param;
      temp_ptr = opt_ptr;

      while(EMA_OPT_END != temp_ptr->opt_id)
      {
         switch(temp_ptr->opt_id)
         {
         case EMA_OPT_TO:
            if (data_cache_ptr->to_addrs_num < 10)
            {
               EMA_UTILS_TRACE("EditProcEnter,EMA_OPT_TO,temp_ptr->val_ptr =%S",temp_ptr->val_ptr);
               EMA_UTILS_WSTRCPY(data_cache_ptr->to_addrs[data_cache_ptr->to_addrs_num++], temp_ptr->val_ptr);
            }
            break;
         case EMA_OPT_CC:
            if (data_cache_ptr->cc_addrs_num < 10)
            {
               EMA_UTILS_WSTRCPY(data_cache_ptr->cc_addrs[data_cache_ptr->cc_addrs_num++], temp_ptr->val_ptr);
            }
            break;
         case EMA_OPT_BCC:
            if (data_cache_ptr->bcc_addrs_num < 10)
            {
               EMA_UTILS_WSTRCPY(data_cache_ptr->bcc_addrs[data_cache_ptr->bcc_addrs_num++], temp_ptr->val_ptr);
            }
            break;
		 case EMA_OPT_SUBJECT:
			 {
				 EMA_UTILS_FREEIF(PNULL, data_cache_ptr->subject_ptr);
				 data_cache_ptr->subject_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)temp_ptr->val_ptr);
			 }	
			 break;
		 case EMA_OPT_BOBY:
	          EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
			  data_cache_ptr->data_buff_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar *)temp_ptr->val_ptr);
			  break;
         default:
            break;
         }
         temp_ptr++;
      }
   }
   
   SetEditContent(s_email_edit_p->edit_doc_ptr, data_cache_ptr);

   MMIEMAIL_ReleaseEmailDataCache(PNULL, data_cache_ptr); 
   return EMAIL_SUCCESS;
}

/*****************************************************************************/
//  Description :proceed reply email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditProcReply(uint32 email_id)
{
    int         result = EMAIL_ERROR;
    int32       i = 0;
    EMAIL_MSGDATACACHE_T *orig_email_ptr = PNULL;
    EMAIL_MSGDATACACHE_T *new_email_ptr = PNULL; /*new e-mail data.*/
    EMAIL_ACT_MSGPREFERENCE_T *email_pref_ptr = PNULL;
    BOOLEAN                   is_include_orig = FALSE; 
    EMAIL_ACT_MSGPREFERENCE_T preference = {0}; 
    EMAIL_ACT_USERINFO_T      user_info = {0};
    uint      size      = 0;
    uint      len = 0;
    wchar    *temp_ptr = EMA_UTILS_WSTRDUP(PNULL,(wchar*)L"\r\n\r\n");
    wchar    *data_temp_ptr = PNULL;
    uint32   status  = 0;
    wchar    *email_addr_ptr = PNULL;
    EMASP_PRFITEM_VAL val = {0};   
    
    do 
    {
        orig_email_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(orig_email_ptr, result, "EditProcReply, orig_email_ptr is PNULL.");
        EMA_UTILS_MEMSET((void*)orig_email_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, orig_email_ptr);
        //EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_Store_GetEmailData failed.");
        if (EMAIL_SUCCESS != result)
        {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            return result;
        }//add by wuhui

        new_email_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(new_email_ptr, result, "EditProcReply, new_email_ptr is PNULL.");
        EMA_UTILS_MEMSET((void*)new_email_ptr, 0 , sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_CFG_GetUserInfoData(EMA_GETCFG_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, &user_info);
        EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_CFG_GetUserInfoData failed.");
        
        // Init uipriority, read_confirm, delivery_confirm
        new_email_ptr->uipriority = EMA_SETTING_FOR_ONE_MSG;
        new_email_ptr->read_confirm = EMA_SETTING_FOR_ONE_MSG;
        new_email_ptr->delivery_confirm = EMA_SETTING_FOR_ONE_MSG;
        
        if (0 == MMIAPICOM_Wstrlen(orig_email_ptr->from_addrs))
        {
            EMA_UTILS_LOG_ERROR(("EditProcReply, param error."));
            result = EMAIL_ERROR;
            break;
        }
        
        if (MMIEMAIL_EDIT_FROM_REPLY == s_email_edit_p->email_edit_enter_type)/*reply*/
        {
            new_email_ptr->to_addrs_num = 1;
            EMA_UTILS_WSTRLCPY(new_email_ptr->to_addrs[0],orig_email_ptr->from_addrs, EMA_MAX_ADDR_LEN + 1);
        }
        else if (MMIEMAIL_EDIT_FROM_REPLYALL == s_email_edit_p->email_edit_enter_type)/*reply all.*/
        {
            if (0 == MMIAPICOM_Wstrlen(orig_email_ptr->from_addrs))
            {
                EMA_UTILS_LOG_ERROR(("EditProcReply, param error."));
                result = EMAIL_ERROR;
                break;
            }
//          new_msg_ptr->to_addrs_num = orig_msg_ptr->to_addrs_num + 1;
//          new_msg_ptr->cc_addrs_num = orig_msg_ptr->cc_addrs_num;
//          new_msg_ptr->bcc_addrs_num = orig_msg_ptr->bcc_addrs_num;
            
            EMA_UTILS_WSTRLCPY(new_email_ptr->to_addrs[new_email_ptr->to_addrs_num], orig_email_ptr->from_addrs, EMA_MAX_ADDR_LEN + 1);
            new_email_ptr->to_addrs_num++;

            result = MMIEMAIL_SP_Store_GetActCfgItem(EMA_GETSTORE_HANDLE(g_email_sp_handle),EMA_SP_ACTIVE_ACCOUNT,EMASP_PRFITEM_ACCOUNT_EMAIL,&val);
            EMA_UTILS_BREAKIF(result, "get act cfg failed");
            
            /* -1 for awFromAddr */
            for (i = 0; i < orig_email_ptr->to_addrs_num && new_email_ptr->to_addrs_num < EMA_MAX_RECPT_NUM; i++)
            {
                email_addr_ptr = MMIEMAIL_GetEmailAddress(orig_email_ptr->to_addrs[i]);
                if (0 == EMA_UTILS_WSTRNICMP(email_addr_ptr, val.pData, EMA_UTILS_WSTRLEN((wchar*)val.pData)))
                {
                    continue;
                }

                if (EMA_UTILS_ISNULLWSTR(orig_email_ptr->to_addrs[i]))
                {
                    EMA_UTILS_LOG_ERROR(("EditProcReply, orig_email_ptr->to_addrs[i] error."));
                    result = EMAIL_ERROR;
                    break;
                }
                
                if (PNULL != user_info.addr_ptr)
                {
                    wchar *pwcAdd = MMIEMAIL_GetEmailAddress(orig_email_ptr->to_addrs[i]);
                    
                    if (0 == EMA_UTILS_WSTRNICMP(user_info.addr_ptr, pwcAdd, EMA_UTILS_WSTRLEN(user_info.addr_ptr)))
                    {
                        continue;
                    }
                }
                
                EMA_UTILS_WSTRLCPY(new_email_ptr->to_addrs[new_email_ptr->to_addrs_num], orig_email_ptr->to_addrs[i], EMA_MAX_ADDR_LEN + 1);
                new_email_ptr->to_addrs_num++;
            }
            
            for (i = 0; i < orig_email_ptr->cc_addrs_num && new_email_ptr->cc_addrs_num < EMA_MAX_RECPT_NUM; i++)
            {
                if (EMA_UTILS_ISNULLWSTR(orig_email_ptr->cc_addrs[i]))
                {
                    EMA_UTILS_LOG_ERROR(("EditProcReply, orig_email_ptr->cc_addrs[i] error."));
                    result = EMAIL_ERROR;
                    break;
                }
                if (PNULL != user_info.addr_ptr)
                {
                    wchar *pwcAdd = MMIEMAIL_GetEmailAddress(orig_email_ptr->cc_addrs[i]);
                    
                    if (0 == EMA_UTILS_WSTRNICMP(user_info.addr_ptr, pwcAdd, EMA_UTILS_WSTRLEN(user_info.addr_ptr)))
                    {
                        continue;
                    }
                }
                EMA_UTILS_WSTRLCPY(new_email_ptr->cc_addrs[new_email_ptr->cc_addrs_num], orig_email_ptr->cc_addrs[i], EMA_MAX_ADDR_LEN + 1);
                new_email_ptr->cc_addrs_num++;
            }
            
            for (i = 0; i < orig_email_ptr->bcc_addrs_num && new_email_ptr->bcc_addrs_num < EMA_MAX_RECPT_NUM; i++)
            {
                if (EMA_UTILS_ISNULLWSTR(orig_email_ptr->bcc_addrs[i]))
                {
                    EMA_UTILS_LOG_ERROR(("EditProcReply, orig_email_ptr->bcc_addrs[i] error."));
                    result = EMAIL_ERROR;
                    break;
                }
                if (PNULL != user_info.addr_ptr)
                {
                    if (0 == EMA_UTILS_WSTRCMP(user_info.addr_ptr, orig_email_ptr->bcc_addrs[i]))
                    {
                        continue;
                    }
                }
                EMA_UTILS_WSTRLCPY(new_email_ptr->bcc_addrs[new_email_ptr->bcc_addrs_num], orig_email_ptr->bcc_addrs[i], EMA_MAX_ADDR_LEN + 1);
                new_email_ptr->bcc_addrs_num++;
            }
        }
        EMA_UTILS_FREEIF(PNULL, new_email_ptr->subject_ptr);
        if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr))
        {
            size = EMA_UTILS_WSTRLEN(orig_email_ptr->subject_ptr);
        }
        else
        {
            size = 0;
        }
        size +=  EMA_UTILS_WSTRLEN((wchar*)EMA_SUBJ_REPLY_PREFIX)+2;
        new_email_ptr->subject_ptr = EMA_UTILS_MALLOC(PNULL, size*sizeof(wchar));
        EMA_UTILS_MEMSET(new_email_ptr->subject_ptr,0x00,size*sizeof(wchar));
        len = EMA_UTILS_WSTRLEN((wchar*)EMA_SUBJ_REPLY_PREFIX) * sizeof(wchar);

        if (EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr)
            || (0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_REPLY_PREFIX, len/sizeof(wchar))
            && 0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_REPLY_PREFIX_1, len/sizeof(wchar))
            && 0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_REPLY_PREFIX_2, len/sizeof(wchar))))
        {
            EMA_UTILS_MEMCPY(new_email_ptr->subject_ptr, EMA_SUBJ_REPLY_PREFIX, len/*EMA_MAX_SUBJ_LEN + 1*/);
        }
        
        if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr))
        {
            EMA_UTILS_WSTRCAT(new_email_ptr->subject_ptr, orig_email_ptr->subject_ptr);
        }
        
        email_pref_ptr = (EMAIL_ACT_MSGPREFERENCE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
        EMA_UTILS_BREAKNULL(email_pref_ptr, result, "EditProcReply, email_pref_ptr is PNULL.");
        EMA_UTILS_MEMSET((void*)email_pref_ptr, 0, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
        
        result = MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_pref_ptr);
        EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_CFG_GetEmailPreference failed.");
        
        is_include_orig = email_pref_ptr->is_include_original;
        
        result = MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr);
        EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_CFG_DEL_EmailPreference failed.");
        EMA_UTILS_FREE(PNULL, email_pref_ptr);
        
        new_email_ptr->act_id = orig_email_ptr->act_id;
        
        /*Save original message id.*/
        new_email_ptr->orig_email_id = email_id;
        
        /*Update original e-mail status.*/
        status = orig_email_ptr->mail_status;
        new_email_ptr->mail_status = orig_email_ptr->mail_status;
        if (orig_email_ptr->attach_num > 0)
        {
            if (EMA_UNREAD_WITH_ATTACH == orig_email_ptr->mail_status
                || EMA_UNREAD_WITH_ATTACH_AND_FORWARDED == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_UNREAD_WITH_ATTACH_AND_REPLIED;
            }
            else if(EMA_READ_WITH_ATTACH == orig_email_ptr->mail_status
                || EMA_READ_WITH_ATTACH_AND_FORWARDED == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_READ_WITH_ATTACH_AND_REPLIED;
            }
            //new_msg_ptr->orig_msg_status = EMA_READ_WITH_ATTACH_AND_REPLIED;
        }
        else
        {
            if (EMA_BASIC_UNREAD == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_UNREAD_AND_REPLIED;
            }
            else if(EMA_BASIC_READ == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_READ_AND_REPLIED;
            }
            //new_msg_ptr->orig_msg_status = EMA_READ_AND_REPLIED;
        }
        new_email_ptr->orig_email_status = orig_email_ptr->mail_status;
        
        if (status != orig_email_ptr->mail_status)
        {
//             result = MMIEMAIL_SP_Store_UpdateMsg(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, orig_msg_ptr);
//             EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_Store_UpdateMsg failed.");
//             
//             MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
        }
        
        /*
        ** Get config about signature, then decide whether need to add signature.
        ** */
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle),
            EMA_SP_ACTIVE_ACCOUNT, &preference))
        {
            EMA_UTILS_LOG_ERROR(("EditProcReply, MMIEMAIL_SP_CFG_GetEmailPreference failed."));
            return EMAIL_ERROR;
        }
        
        switch(preference.signature_mode)
        {
        case EMA_ADD_SIGNATURE_NO_EMAIL:
            break;
        case EMA_ADD_SIGNATURE_ONLY_NEW_EMAIL:
            break;
        case EMA_ADD_SIGNATURE_ALL_EMAIL:
            {
                wchar *pwszSignature = PNULL;
                uint32  data_len = 0;
                
                EditAddSignature(&pwszSignature);
                if (PNULL != new_email_ptr->data_buff_ptr)
                {
                    data_len = (EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr) * 2 + 2);
                    data_temp_ptr = EMA_UTILS_WSTRDUP(PNULL, new_email_ptr->data_buff_ptr);
                    EMA_UTILS_BREAKNULL(data_temp_ptr, result, "out of memory");
                }
                if (PNULL != pwszSignature)
                {
                    data_len += (EMA_UTILS_WSTRLEN(pwszSignature) * 2 + 6);
                }
                
                EMA_UTILS_FREEIF(PNULL, new_email_ptr->data_buff_ptr);
                new_email_ptr->data_buff_ptr = EMA_UTILS_MALLOC(PNULL, data_len);
                EMA_UTILS_BREAKNULL(new_email_ptr->data_buff_ptr, result, "out of memory");
                EMA_UTILS_MEMSET(new_email_ptr->data_buff_ptr, 0x00, data_len);
                
                if (PNULL != data_temp_ptr)
                {
                    EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, data_temp_ptr);
                }
                
                if (PNULL != pwszSignature)
                {
                    EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, (wchar*)L"\n");
                    EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, pwszSignature);
                }
                
                EMA_UTILS_FREEIF(PNULL, data_temp_ptr);
                EMA_UTILS_FREEIF(PNULL, pwszSignature);
            }
            break;
        default:
            break;
        }
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), &preference))
        {
            EMA_UTILS_LOG_ERROR(("EditProcReply, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
            return EMAIL_ERROR;
        }
        
        //result = MMIEMAIL_SP_Store_SaveMsg(EMA_GETSTORE_HANDLE(g_email_sp_handle), new_msg_ptr);
        //EMA_UTILS_BREAKIF(result, "EditProcReply, MMIEMAIL_SP_Store_CreateNew failed.");
        if (PNULL != orig_email_ptr->data_buff_ptr && TRUE == is_include_orig)
        {
            // 		  new_msg_ptr->data_buff_ptr = EMA_UTILS_WSTRDUP(PNULL, orig_msg_ptr->data_buff_ptr);
            // 		  EMA_UTILS_BREAKNULL(new_msg_ptr->data_buff_ptr, result, "EditProcReply: new_msg_ptr->data_buff_ptr is PNULL.");
            
            uint32  data_len = 0;
            uint32  data_size = 0;
            
            if (PNULL != new_email_ptr->data_buff_ptr)
            {
                data_len = (EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr) * 2 + 2);
            }
            if (PNULL != orig_email_ptr->data_buff_ptr)
            {
                data_len += (EMA_UTILS_WSTRLEN(orig_email_ptr->data_buff_ptr) * 2 + 64);
            }
            if (PNULL != new_email_ptr->data_buff_ptr)
            {
                new_email_ptr->data_buff_ptr = EMA_UTILS_REALLOC(PNULL,
                    new_email_ptr->data_buff_ptr, data_len);
            }
            else
            {
                new_email_ptr->data_buff_ptr = EMA_UTILS_MALLOC(PNULL,data_len);
                EMA_UTILS_MEMSET(new_email_ptr->data_buff_ptr,0x00,data_len);
            }
            
            if (PNULL != new_email_ptr->data_buff_ptr && PNULL != orig_email_ptr->data_buff_ptr)
            {
                //EMA_UTILS_WSTRCAT(new_msg_ptr->data_buff_ptr, L" \r\n\n-------------------\n");
                data_len = EMA_UTILS_WSTRLEN(temp_ptr) * 2 + 2; 
                data_size = EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr);
                //EMA_UTILS_WSPRINTF(new_msg_ptr->data_buff_ptr+data_size, data_len,L"%s",temp_ptr);
                EMA_UTILS_MEMCPY(new_email_ptr->data_buff_ptr+data_size, temp_ptr, data_len - 2);
                
                data_len = EMA_UTILS_WSTRLEN(orig_email_ptr->data_buff_ptr) * 2 + 2;
                data_size = EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr);
                
                //EMA_UTILS_WSPRINTF(new_msg_ptr->data_buff_ptr+data_size,data_len,L"%s", orig_msg_ptr->data_buff_ptr);
                EMA_UTILS_MEMCPY(new_email_ptr->data_buff_ptr+data_size,orig_email_ptr->data_buff_ptr, data_len - 2);
            }
        }
        
        SetEditContent(s_email_edit_p->edit_doc_ptr, new_email_ptr);
        
   } while(0);
   
   if (EMAIL_SUCCESS != result)
   {
       MMIEMAIL_SP_CFG_DEL_ActUserInfo(EMA_GETCFG_HANDLE(g_email_sp_handle), &user_info);
       MMIEMAIL_ReleaseEmailDataCache(PNULL, orig_email_ptr);
       MMIEMAIL_ReleaseEmailDataCache(PNULL, new_email_ptr);
       return EMAIL_ERROR;
   }
   MMIEMAIL_SP_CFG_DEL_ActUserInfo(EMA_GETCFG_HANDLE(g_email_sp_handle), &user_info);
   MMIEMAIL_ReleaseEmailDataCache(PNULL, orig_email_ptr);
   MMIEMAIL_ReleaseEmailDataCache(PNULL, new_email_ptr);
   EMA_UTILS_FREEIF(PNULL, temp_ptr);
   return result;
}

/*****************************************************************************/
//  Description :proceed forward email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
int32 EditProcForward(uint32 email_id)
{
    int result = EMAIL_ERROR;
    int32 i = 0;
    EMAIL_MSGDATACACHE_T *orig_email_ptr = PNULL;
    EMAIL_MSGDATACACHE_T *new_email_ptr = PNULL; /*new e-mail data.*/
    EMAIL_ACT_MSGPREFERENCE_T  preference = {0};  
    uint  size = 0;
    uint  len = 0;
    uint32   status = 0;
    int32   ret = 0;
    
    do 
    {
        orig_email_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(orig_email_ptr, result, "EditProcForward, orig_email_ptr is PNULL.");
        EMA_UTILS_MEMSET(orig_email_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, orig_email_ptr);
        //EMA_UTILS_BREAKIF(result, "EditProcForward, MMIEMAIL_SP_Store_GetEmailData failed.");
        if (EMAIL_SUCCESS != result)
        {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            return result;
        }//add by wuhui
        
        new_email_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(new_email_ptr, result, "EditProcReply, new_email_ptr is PNULL.");
        EMA_UTILS_MEMSET(new_email_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        // Init uipriority, read_confirm, delivery_confirm
        new_email_ptr->uipriority = EMA_SETTING_FOR_ONE_MSG;
        new_email_ptr->read_confirm = EMA_SETTING_FOR_ONE_MSG;
        new_email_ptr->delivery_confirm = EMA_SETTING_FOR_ONE_MSG;
        
        EMA_UTILS_MEMSET(new_email_ptr, 0 , sizeof(EMAIL_MSGDATACACHE_T));
        if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr))
        {
            size  = EMA_UTILS_WSTRLEN(orig_email_ptr->subject_ptr);
        }
        else
        {
            size = 0;
        }
        size +=  EMA_UTILS_WSTRLEN((wchar*)EMA_SUBJ_FWD_PREFIX)+2;
        new_email_ptr->subject_ptr = EMA_UTILS_MALLOC(PNULL, size*sizeof(wchar));
        EMA_UTILS_MEMSET(new_email_ptr->subject_ptr,0x00,size*sizeof(wchar));
        len = EMA_UTILS_WSTRLEN((wchar*)EMA_SUBJ_FWD_PREFIX)*sizeof(wchar);
        if (EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr)
            || (0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_FWD_PREFIX, len/sizeof(wchar))
                && 0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_FWD_PREFIX_1, len/sizeof(wchar))
                && 0 != EMA_UTILS_WSTRNICMP(orig_email_ptr->subject_ptr, EMA_SUBJ_FWD_PREFIX_2, len/sizeof(wchar))))
        {
            EMA_UTILS_MEMCPY(new_email_ptr->subject_ptr, EMA_SUBJ_FWD_PREFIX, len/*EMA_MAX_SUBJ_LEN + 1*/);
        }
        if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->subject_ptr))
        {
            EMA_UTILS_WSTRCAT(new_email_ptr->subject_ptr, orig_email_ptr->subject_ptr);
        }
        
        if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_FORWARD)
        {
            new_email_ptr->attach_num = orig_email_ptr->attach_num;
            for (i = 0; i < orig_email_ptr->attach_num; i++)
            {
                if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->attach_uri[i]))
                {
                /*
                *	wuzhongxiao 2010/11/01
                * 防止在字符拷贝时有内容遗失
                    */
                    len = EMA_UTILS_WSTRLEN(orig_email_ptr->attach_uri[i]) * sizeof(wchar);
                    EMA_UTILS_MEMCPY(new_email_ptr->attach_uri[i], orig_email_ptr->attach_uri[i], len);
                }
                if (!EMA_UTILS_ISNULLWSTR(orig_email_ptr->attach_name[i]))
                {
                    len = EMA_UTILS_WSTRLEN(orig_email_ptr->attach_name[i]) * sizeof(wchar);
                    EMA_UTILS_MEMCPY(new_email_ptr->attach_name[i], orig_email_ptr->attach_name[i], len);
                }
                if (PNULL != orig_email_ptr->attach_size_ptr[i])
                {
                    new_email_ptr->attach_size_ptr[i] = EMA_UTILS_WSTRDUP(PNULL, orig_email_ptr->attach_size_ptr[i]);
                }
            }
        }
        
        new_email_ptr->act_id = orig_email_ptr->act_id;
        
        /*
        ** Get config about signature, then decide whether need to add signature.
        ** */
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle),
            EMA_SP_ACTIVE_ACCOUNT, &preference))
        {
            EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_CFG_GetEmailPreference failed."));
            return EMAIL_ERROR;
        }
        
        switch(preference.signature_mode)
        {
        case EMA_ADD_SIGNATURE_NO_EMAIL:
            break;
        case EMA_ADD_SIGNATURE_ONLY_NEW_EMAIL:
            break;
        case EMA_ADD_SIGNATURE_ALL_EMAIL:
            {
                wchar *signature_ptr = PNULL;
                uint32  data_len = 0;
                EditAddSignature(&signature_ptr);
                if (PNULL != new_email_ptr->data_buff_ptr)
                {
                    data_len = (EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr) * 2 + 2);
                }
                if (PNULL != signature_ptr)
                {
                    data_len += (EMA_UTILS_WSTRLEN(signature_ptr) * 2 + 6);
                    new_email_ptr->data_buff_ptr = EMA_UTILS_MALLOC(PNULL, data_len);
                    EMA_UTILS_BREAKNULL(new_email_ptr->data_buff_ptr, ret, "out of memory");
                    EMA_UTILS_MEMSET(new_email_ptr->data_buff_ptr, 0x00, data_len);
                    EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, (wchar*)L"\n");
                    EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, signature_ptr);
                    EMA_UTILS_FREE(PNULL, signature_ptr);
                }
            }
            break;
        default:
            break;
        }
        
        if (PNULL != orig_email_ptr->data_buff_ptr)
        {
            // 		  new_msg_ptr->data_buff_ptr = EMA_UTILS_WSTRDUP(PNULL, orig_msg_ptr->data_buff_ptr);
            // 		  EMA_UTILS_BREAKNULL(new_msg_ptr->data_buff_ptr, result, 
            // 			  "EditProcForward: new_msg_ptr->data_buff_ptr is PNULL.");
            
            uint32  buffer_len = 0;
            wchar   *data_temp_ptr = PNULL;
            
            if (PNULL != new_email_ptr->data_buff_ptr)
            {
                buffer_len = (EMA_UTILS_WSTRLEN(new_email_ptr->data_buff_ptr) * 2 + 2);   
                data_temp_ptr = EMA_UTILS_WSTRDUP(PNULL, new_email_ptr->data_buff_ptr);
                EMA_UTILS_BREAKNULL(data_temp_ptr, result, "out of memory");
                EMA_UTILS_FREEIF(PNULL, new_email_ptr->data_buff_ptr);
            }
            
            buffer_len += (EMA_UTILS_WSTRLEN(orig_email_ptr->data_buff_ptr) * 2 + 10);
            
            new_email_ptr->data_buff_ptr = EMA_UTILS_MALLOC(PNULL, buffer_len);
            EMA_UTILS_BREAKNULL(new_email_ptr->data_buff_ptr, result, "out of memory");
            EMA_UTILS_MEMSET(new_email_ptr->data_buff_ptr,0x00,buffer_len);
            
            
            if (PNULL != data_temp_ptr)
            {
                EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, data_temp_ptr);
            }
            
            EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, (wchar*)L"\r\n\r\n");
            EMA_UTILS_WSTRCAT(new_email_ptr->data_buff_ptr, orig_email_ptr->data_buff_ptr);
            EMA_UTILS_FREEIF(PNULL, data_temp_ptr);
        }
        result = MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), &preference);
        EMA_UTILS_BREAKIF(result, "EditProcForward, MMIEMAIL_SP_CFG_DEL_EmailPreference failed.");
        
        //result = MMIEMAIL_SP_Store_SaveMsg(EMA_GETSTORE_HANDLE(g_email_sp_handle), new_msg_ptr);
        //EMA_UTILS_BREAKIF(result, "EditProcForward, MMIEMAIL_SP_Store_CreateNew failed.");
        
        /*Save original message id.*/
        new_email_ptr->orig_email_id = email_id;
        
        /*Update original e-mail status.*/
        status = orig_email_ptr->mail_status;
        new_email_ptr->mail_status = orig_email_ptr->mail_status;
        if (orig_email_ptr->attach_num > 0)
        {
            //modify by eva zhang, to resolv and unread problem
            if (EMA_UNREAD_WITH_ATTACH == orig_email_ptr->mail_status
                || EMA_UNREAD_WITH_ATTACH_AND_REPLIED == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_UNREAD_WITH_ATTACH_AND_FORWARDED;
            }
            else if(EMA_READ_WITH_ATTACH == orig_email_ptr->mail_status
                || EMA_READ_WITH_ATTACH_AND_REPLIED == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_READ_WITH_ATTACH_AND_FORWARDED;
            }
            //new_msg_ptr->orig_msg_status =EMA_READ_WITH_ATTACH_AND_FORWARDED;
        }
        else
        {
            if (EMA_BASIC_UNREAD == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_UNREAD_AND_FORWARDED;
            }
            else if(EMA_BASIC_READ == orig_email_ptr->mail_status)
            {
                orig_email_ptr->mail_status = EMA_READ_AND_FORWARDED;
            }
            //new_msg_ptr->orig_msg_status = EMA_READ_AND_FORWARDED;
        }
        new_email_ptr->orig_email_status = orig_email_ptr->mail_status;
        
        if (status != orig_email_ptr->mail_status)
        {
//             result = MMIEMAIL_SP_Store_UpdateMsg(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, orig_msg_ptr);
//             EMA_UTILS_BREAKIF(result, "EditProcForward, MMIEMAIL_SP_Store_UpdateMsg failed.");
//             
//             MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
        }
        
        SetEditContent(s_email_edit_p->edit_doc_ptr, new_email_ptr);
        
   } while(0);
   
   if (EMAIL_SUCCESS != result)
   {
       MMIEMAIL_ReleaseEmailDataCache(PNULL, orig_email_ptr);
       MMIEMAIL_ReleaseEmailDataCache(PNULL, new_email_ptr);
       return EMAIL_ERROR;
   }
   MMIEMAIL_ReleaseEmailDataCache(PNULL, orig_email_ptr);
   MMIEMAIL_ReleaseEmailDataCache(PNULL, new_email_ptr);
   return result;
}

/*****************************************************************************/
//  Description :proceed email edit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditProcEdit(uint32 email_id)
{
    int result = EMAIL_SUCCESS;
    EMAIL_MSGDATACACHE_T *email_data_ptr = PNULL;


    do 
    {
        email_data_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(email_data_ptr, result, "EditProcEdit, email_data_ptr is PNULL.");
        EMA_UTILS_MEMSET(email_data_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, email_data_ptr);
        //EMA_UTILS_BREAKIF(result, "EditProcEdit, MMIEMAIL_SP_Store_GetEmailData failed.");
        if (EMAIL_SUCCESS != result)
        {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            return result;
        }//add by wuhui
        SetEditContent(s_email_edit_p->edit_doc_ptr, email_data_ptr);
        SetEditContent(s_email_edit_p->edit_back_ptr, email_data_ptr);
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        MMIEMAIL_ReleaseEmailDataCache(PNULL, email_data_ptr);
        return EMAIL_ERROR;
    }
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, email_data_ptr);
    return result;
}

/*****************************************************************************/
//  Description :proceed email view send
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN EditProcViwerSend(wchar* pswzAddress)
{
    EMAIL_MSGDATACACHE_T  *data_cache_ptr = PNULL;
    EMAIL_ACT_MSGPREFERENCE_T  preference = {0};
    
    data_cache_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
    if (PNULL == data_cache_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EditProcEnter, data_cache_ptr is PNULL."));
        return EMAIL_ERROR;
    }
    EMA_UTILS_MEMSET(data_cache_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
    
    data_cache_ptr->act_id = EMA_SP_ACTIVE_ACCOUNT;
    if (data_cache_ptr->attach_num > 0)
    {
        data_cache_ptr->mail_status = EMA_BASIC_DRAFT_WITH_ATTACH;
    }
    else
    {
        data_cache_ptr->mail_status = EMA_BASIC_DRAFT;
    }
    
    if (pswzAddress)
    {
        int len = 0;
        len = EMA_UTILS_WSTRLEN(pswzAddress);
        len = len > EMA_MAX_ADDR_LEN ? EMA_MAX_ADDR_LEN:len;
        EMA_UTILS_WSTRLCPY(data_cache_ptr->to_addrs[0], pswzAddress, len + 1);
        data_cache_ptr->to_addrs_num = 1;
    }
    
    /*
    ** Get config about signature, then decide whether need to add signature.
    **
    */
    if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle),
        EMA_SP_ACTIVE_ACCOUNT, &preference))
    {
        EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_CFG_GetEmailPreference failed."));
        return EMAIL_ERROR;
    }
    
    switch(preference.signature_mode)
    {
    case EMA_ADD_SIGNATURE_NO_EMAIL:
        break;
    case EMA_ADD_SIGNATURE_ONLY_NEW_EMAIL:
    case EMA_ADD_SIGNATURE_ALL_EMAIL:
        {
            /* add signature.*/
            EditAddSignature(&data_cache_ptr->data_buff_ptr);
        }
        break;
    default:
        break;
    }
    if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), &preference))
    {
        EMA_UTILS_LOG_ERROR(("EditProcEnter, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
        return EMAIL_ERROR;
    }
    
    // Init uipriority, read_confirm, delivery_confirm
    data_cache_ptr->uipriority = EMA_SETTING_FOR_ONE_MSG;
    data_cache_ptr->read_confirm = EMA_SETTING_FOR_ONE_MSG;
    data_cache_ptr->delivery_confirm = EMA_SETTING_FOR_ONE_MSG;
    
    SetEditContent(s_email_edit_p->edit_doc_ptr, data_cache_ptr);
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, data_cache_ptr); 
    return EMAIL_SUCCESS;
}

/*****************************************************************************/
//  Description :proceed email view send
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditProcViewEdit(uint32 email_id)
{
    int result = EMAIL_SUCCESS;
    EMAIL_MSGDATACACHE_T *email_data_ptr = PNULL;
    
    do 
    {
        email_data_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(email_data_ptr, result, "EditProcEdit, email_data_ptr is PNULL.");
        EMA_UTILS_MEMSET(email_data_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, email_data_ptr);
        EMA_UTILS_BREAKIF(result, "EditProcEdit, MMIEMAIL_SP_Store_GetEmailData failed.");
        
        email_data_ptr->email_id = 0;
        SetEditContent(s_email_edit_p->edit_doc_ptr, email_data_ptr);
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        MMIEMAIL_ReleaseEmailDataCache(PNULL, email_data_ptr);
        return EMAIL_ERROR;
    }
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, email_data_ptr);
    return result;
}

/*****************************************************************************/
// 	Description: get all recipient 
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_GetEditRecipients(
                                        wchar *addr_ptr,
                                        wchar **add_addr_ptr,
                                        void *mem_mgr_ptr,
                                        uint32  num
                                        )
{
    wchar *pwszRecpt = PNULL;
    wchar *temp_ptr = PNULL;
    int32   result = EMAIL_ERROR;
    uint32  i = 0;
    uint32  len = 0;
    do 
    {
        len = (num + 1) * (EMA_MAX_ADDR_LEN + 1) * sizeof(wchar);
        pwszRecpt = EMA_UTILS_MALLOC(mem_mgr_ptr, len);
        EMA_UTILS_BREAKNULL(pwszRecpt, result, "MMIEMAIL_GetEditRecipients: Malloc pwszRecpt failed!");
        EMA_UTILS_MEMSET(pwszRecpt, 0 , len);
        for (i = 0; i < num; i++)
        {
            temp_ptr = addr_ptr + i * (EMA_MAX_ADDR_LEN + 1);
            EMA_UTILS_WSTRCAT(pwszRecpt, temp_ptr);
            if (i < (num - 1))
            {
                EMA_UTILS_WSTRCAT(pwszRecpt, (wchar*)L";");
            }
        }
        *add_addr_ptr = pwszRecpt;
    } while(0);
    
    return EMAIL_SUCCESS;
}

/*****************************************************************************/
//  Description :set edit content
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 SetEditContent(
                           MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr,
                           EMAIL_MSGDATACACHE_T *data_cache_ptr
                           )
{
    int	   i = 0;
    uint32 len = 0;
    EMAIL_ATTACH_T *temp_ptr = PNULL;
    EMAIL_ATTACH_T *header_ptr = PNULL;
    char           attach_size[64] = {0};
    int32          ret = 0;
	
    do 
    {
        EMA_UTILS_TRACE("!!!!!!!!!!!!Before Entering EMA_CopyEmailDataCache!!!!!!!!!!!!!!!!");
        
        if (EMAIL_SUCCESS != MMIEMAIL_CopyEmailDataCache(&s_email_edit_p->data_cache, data_cache_ptr, PNULL))
        {
            EMA_UTILS_TRACE("EmailAFmMgr_Compose_SetWidgetContent, MMIEMAIL_CopyEmailDataCache failed!");
            return EMAIL_ERROR;
        }
        
        EMA_UTILS_TRACE("!!!!!!!!!!!!MMIEMAIL_CopyEmailDataCache End!!!!!!!!!!!!!!!!");
        
        /*Show 'To:'*/
        if (data_cache_ptr->to_addrs_num > 0)
        {
            MMIEMAIL_GetEditRecipients(&data_cache_ptr->to_addrs[0][0], &edit_doc_ptr->edit_to.wstr_ptr,
                PNULL, data_cache_ptr->to_addrs_num);
            edit_doc_ptr->edit_to.wstr_len = EMA_UTILS_WSTRLEN(edit_doc_ptr->edit_to.wstr_ptr);
        }
        /*Show 'Cc:'*/
        if (data_cache_ptr->cc_addrs_num > 0)
        {
            MMIEMAIL_GetEditRecipients(&data_cache_ptr->cc_addrs[0][0], &edit_doc_ptr->edit_cc.wstr_ptr,
                PNULL, data_cache_ptr->cc_addrs_num);
            edit_doc_ptr->edit_cc.wstr_len = EMA_UTILS_WSTRLEN(edit_doc_ptr->edit_cc.wstr_ptr);
            s_email_edit_p->is_has_cc = TRUE;
        }
        
        /*Show 'Bcc'*/
        if (data_cache_ptr->bcc_addrs_num > 0)
        {
            MMIEMAIL_GetEditRecipients(&data_cache_ptr->bcc_addrs[0][0], &edit_doc_ptr->edit_bcc.wstr_ptr,
                PNULL, data_cache_ptr->bcc_addrs_num);
            edit_doc_ptr->edit_bcc.wstr_len = EMA_UTILS_WSTRLEN(edit_doc_ptr->edit_bcc.wstr_ptr);
            s_email_edit_p->is_has_bcc = TRUE;
        }
        
        /*Show attachment*/
        edit_doc_ptr->attach_num = data_cache_ptr->attach_num;
        for (i = 0; i < data_cache_ptr->attach_num; i++)
        { 
            temp_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ATTACH_T));
            EMA_UTILS_BREAKNULL(temp_ptr, ret, "out of memory");
            EMA_UTILS_MEMSET(temp_ptr, 0x00, sizeof(EMAIL_ATTACH_T));
            len = EMA_UTILS_WSTRLEN(data_cache_ptr->attach_uri[i]) * sizeof(wchar);
            EMA_UTILS_MEMCPY(temp_ptr->file_path, data_cache_ptr->attach_uri[i], len);
            MMIAPICOM_WstrToStr(data_cache_ptr->attach_size_ptr[i], (uint8*)attach_size);
            temp_ptr->file_size = (uint32)EMA_UTILS_ATOI((unsigned char*)attach_size);
            temp_ptr->file_name_ptr = EMA_UTILS_WSTRDUP(PNULL, data_cache_ptr->attach_name[i]);
            
            header_ptr = edit_doc_ptr->attach_list_ptr;
            if (PNULL == edit_doc_ptr->attach_list_ptr)
            {
                edit_doc_ptr->attach_list_ptr = temp_ptr;
            }
            else
            {
                while (header_ptr->next_ptr)
                {
                    header_ptr = header_ptr->next_ptr;
                }
                header_ptr->next_ptr = temp_ptr;
            }
        }
        EMA_UTILS_BREAKIF(ret, "operation failed");
        
        /*Show 'Subject'*/
        if (PNULL != data_cache_ptr->subject_ptr)
        {
            edit_doc_ptr->edit_subject.wstr_len = EMA_UTILS_WSTRLEN(data_cache_ptr->subject_ptr);
            edit_doc_ptr->edit_subject.wstr_ptr = EMA_UTILS_MALLOC(PNULL, edit_doc_ptr->edit_subject.wstr_len * sizeof(wchar) + 2);
            EMA_UTILS_BREAKNULL(edit_doc_ptr->edit_subject.wstr_ptr, ret, "out of memory");
            EMA_UTILS_MEMSET(edit_doc_ptr->edit_subject.wstr_ptr, 0, edit_doc_ptr->edit_subject.wstr_len * sizeof(wchar) + 2);
            EMA_UTILS_MEMCPY(edit_doc_ptr->edit_subject.wstr_ptr, data_cache_ptr->subject_ptr, edit_doc_ptr->edit_subject.wstr_len * sizeof(wchar));
        }
        
        
        /*Show 'text'*/
        if (PNULL != data_cache_ptr->data_buff_ptr)
        {
            edit_doc_ptr->edit_text.wstr_len = EMA_UTILS_WSTRLEN(data_cache_ptr->data_buff_ptr);
            edit_doc_ptr->edit_text.wstr_ptr = EMA_UTILS_MALLOC(PNULL, edit_doc_ptr->edit_text.wstr_len * sizeof(wchar) + 2);
            EMA_UTILS_BREAKNULL(edit_doc_ptr->edit_text.wstr_ptr, ret, "out of memory");
            EMA_UTILS_MEMSET(edit_doc_ptr->edit_text.wstr_ptr, 0, edit_doc_ptr->edit_text.wstr_len * sizeof(wchar) + 2);
            EMA_UTILS_MEMCPY(edit_doc_ptr->edit_text.wstr_ptr, data_cache_ptr->data_buff_ptr, edit_doc_ptr->edit_text.wstr_len * sizeof(wchar));
        }
        
        if (EMA_READ_AND_FORWARDED == data_cache_ptr->orig_email_status || EMA_READ_WITH_ATTACH_AND_FORWARDED == data_cache_ptr->orig_email_status)
        {
            //edit_ptr->bForward = TRUE;
        }

        if (0 < data_cache_ptr->attach_num)
        {
            data_cache_ptr->orig_email_status = EMA_BASIC_DRAFT_WITH_ATTACH_READ;
        }
        else
        {
            data_cache_ptr->orig_email_status = EMA_BASIC_DRAFT_READ;
        }

    } while (0);
    return EMAIL_SUCCESS;
}

//*****************************************************************************/
//  Description :decode recipient
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditDecodeReceipt
(
    wchar *addr_ptr,
    wchar *all_recpt_ptr,
    int32  *num_ptr
)
{

   wchar *pwcPos = all_recpt_ptr;
   wchar *pwcNext = all_recpt_ptr;
   uint32  dwCpyLen = 0;
   uint32  dwAllRecptLen = EMA_UTILS_WSTRLEN(all_recpt_ptr);
   int32  i = 0;
   int32   nNum = 0;

   
   if (dwAllRecptLen == 0)
   {
      return MMIEMAIL_NO_ERROR;
   }
   for (i = 0; i <= (int32)dwAllRecptLen; i++)
   {
      if (0 == EMA_UTILS_WSTRNICMP(pwcNext, (wchar*)L";", 1))
      {
			/*limited address length.*/
			dwCpyLen = dwCpyLen < EMA_MAX_ADDR_LEN ? dwCpyLen : EMA_MAX_ADDR_LEN;
         EMA_UTILS_WSTRLCPY(addr_ptr + (nNum * (EMA_MAX_ADDR_LEN + 1)),
                  pwcPos, dwCpyLen);
         nNum++;
         *num_ptr = nNum;
         pwcNext++;
         pwcPos = pwcNext;
         dwCpyLen = 0;
         if (nNum > EMA_MAX_RECPT_NUM 
             || (0 == EMA_UTILS_WSTRNICMP(pwcPos, (wchar*)L"\0", 1)))
         {
            break;
         }
      }  
      else if (0 == EMA_UTILS_WSTRNICMP(pwcNext, (wchar*)L"\0", 1))
      {
			/*limited address length.*/
			dwCpyLen = dwCpyLen < EMA_MAX_ADDR_LEN ? dwCpyLen : EMA_MAX_ADDR_LEN;
         EMA_UTILS_WSTRLCPY(addr_ptr + (nNum * (EMA_MAX_ADDR_LEN + 1)),
                  pwcPos, dwCpyLen + 1);
         nNum++;
         *num_ptr = nNum;
         pwcNext++;
         pwcPos = pwcNext;
         dwCpyLen = 0;
         if (nNum > EMA_MAX_RECPT_NUM)
         {
            break;
         }
      }
      else
      {
         pwcNext++;
         dwCpyLen++;  
      }    
   }
   return MMIEMAIL_NO_ERROR;   
}


//*****************************************************************************/
//  Description :save data cache
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int32 EditSaveDataCache(
                              MMIEMAIL_EDIT_T *edit_ptr,
                              EMAIL_MSGDATACACHE_T *data_cache_ptr
                              )
{
    uint32  len = 0;
    uint16  size = 0;
    wchar   *buff_ptr = PNULL;
    int32   i = 0;
    int32   ret = 0;
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
   	EMAIL_ATTACH_T   *header_ptr = PNULL;
    
    edit_doc_ptr = edit_ptr->edit_doc_ptr;
    
    do 
    {
        if (PNULL != edit_doc_ptr->edit_text.wstr_ptr && 0 != edit_doc_ptr->edit_text.wstr_len)
        {
            EMA_UTILS_TRACE("mmiemail:EditSaveDataCache, edit_doc_ptr->edit_text.wstr_len %d", edit_doc_ptr->edit_text.wstr_len);
            /* add by jiangdingqing, buffer size must < EMA_MAX_TEXT_BODY_SIZE */
            if (edit_doc_ptr->edit_text.wstr_len * 2 > EMA_MAX_TEXT_BODY_SIZE)
            {
                size = EMA_MAX_TEXT_BODY_SIZE + 2;
            }
            else
            {
                size = (uint16)(edit_doc_ptr->edit_text.wstr_len * 2 + 2);
            }
            EMA_UTILS_TRACE("mmiemail:EditSaveDataCache, the size is %d", size);
            
            buff_ptr = (wchar*)EMA_UTILS_MALLOC(PNULL, size);
            if (PNULL == buff_ptr)
            {
                return EMAIL_NOMEMORY;
            }
            
            EMA_UTILS_MEMSET(buff_ptr, 0x00, size);
            
            EMA_UTILS_MEMCPY(((char*)buff_ptr), edit_doc_ptr->edit_text.wstr_ptr, size - 2);
            
            EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
            data_cache_ptr->data_buff_ptr = buff_ptr;
        }
        else
        {
            data_cache_ptr->data_buff_ptr = EMA_UTILS_WSTRDUP(PNULL, (wchar*)L"");
            EMA_UTILS_BREAKNULL(data_cache_ptr->data_buff_ptr , ret, "out of memory");
        }
    } while (0);
    
    /* Subject*/
    //    IWIDGET_GetStaticTextEditText(edit_ptr->piSubject, &temp_ptr);
    //    if (PNULL == temp_ptr || EMA_UTILS_WSTRLEN(temp_ptr) == 0)
    //    {
    //       if(PNULL != temp_ptr)
    //          FREEIF(temp_ptr);
    //       temp_ptr = WSTRDUP(edit_ptr->pwszSubBuf);
    //    }
    
    //   temp_ptr = edit_ptr->stSubjectInfo.pwszStatic2;
    //temp_ptr = edit_ptr->pwszSubject;
    if (PNULL != edit_doc_ptr->edit_subject.wstr_ptr && 0 != edit_doc_ptr->edit_subject.wstr_len)
    {
        len = edit_doc_ptr->edit_subject.wstr_len * 2 + 2;
        EMA_UTILS_FREEIF(PNULL, data_cache_ptr->subject_ptr);
        data_cache_ptr->subject_ptr = EMA_UTILS_MALLOC(PNULL, len);
        EMA_UTILS_MEMSET(data_cache_ptr->subject_ptr,0x00,len);
        
        /* changed by jiangdingqing, add '\0' for subject */
        EMA_UTILS_MEMCPY(data_cache_ptr->subject_ptr, edit_doc_ptr->edit_subject.wstr_ptr, len - 2);
    }
    else
    {
        EMA_UTILS_FREEIF(PNULL, data_cache_ptr->subject_ptr);
        data_cache_ptr->subject_ptr = EMA_UTILS_MALLOC(PNULL, EMA_MAX_SUBJ_LEN + 1);
        EMA_UTILS_MEMSET(data_cache_ptr->subject_ptr, 0, EMA_MAX_SUBJ_LEN + 1);
    }
    //It get from StaticTextEdit, we should free the memory.
    
    /* To */
    if (PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len)
    {
        EditDecodeReceipt(&(data_cache_ptr->to_addrs[0][0]), 
            edit_doc_ptr->edit_to.wstr_ptr,
            (int32*)&data_cache_ptr->to_addrs_num);
    }
    else
    {
        data_cache_ptr->to_addrs_num = 0;
    }
    //It get from StaticTextEdit, we should free the memory.
    
    /* Cc*/
    if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr && 0 != edit_doc_ptr->edit_cc.wstr_len)
    {
        EditDecodeReceipt(&(data_cache_ptr->cc_addrs[0][0]), 
            edit_doc_ptr->edit_cc.wstr_ptr,
            (int32*)&data_cache_ptr->cc_addrs_num);
    }
    else
    {
        data_cache_ptr->cc_addrs_num = 0;
    }
    //It get from StaticTextEdit, we should free the memory.
    
    /* Bcc */
    if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr && 0 != edit_doc_ptr->edit_bcc.wstr_len)
    {
        EditDecodeReceipt(&(data_cache_ptr->bcc_addrs[0][0]), 
            edit_doc_ptr->edit_bcc.wstr_ptr,
            (int32*)&data_cache_ptr->bcc_addrs_num);
    }
    else
    {
        data_cache_ptr->bcc_addrs_num = 0;
    }
    //It get from StaticTextEdit, we should free the memory.
    /* Save attachments*/
    header_ptr = edit_doc_ptr->attach_list_ptr;
    while(header_ptr)
    {
        len = EMA_UTILS_WSTRLEN(header_ptr->file_path) * 2;
        EMA_UTILS_MEMSET(data_cache_ptr->attach_uri[i], 0, sizeof(wchar) * (EMA_MAX_ATTACHNAME_LEN + 1));
        EMA_UTILS_MEMCPY(data_cache_ptr->attach_uri[i], header_ptr->file_path, len);
        len = EMA_UTILS_WSTRLEN(header_ptr->file_name_ptr) * 2;
        EMA_UTILS_MEMSET(data_cache_ptr->attach_name[i], 0, sizeof(wchar) * (EMA_MAX_ATTACHNAME_LEN + 1));
        EMA_UTILS_MEMCPY(data_cache_ptr->attach_name[i], header_ptr->file_name_ptr, len);
        //MMIAPICOM_GetFileName(header_ptr->file_path, EMA_UTILS_WSTRLEN(header_ptr->file_path), data_cache_ptr->attach_name[i], &size);
        
        i++;
        header_ptr = header_ptr->next_ptr;
    }
    data_cache_ptr->attach_num = i;
    return EMAIL_SUCCESS;
}

/*****************************************************************************/
// 	Description: clear email cache
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_ClearEmailCache(
                                      EMAIL_MSGDATACACHE_T *data_cache_ptr
                                      )
{
    int i = 0;
    if (PNULL == data_cache_ptr)
    {
        EMA_UTILS_TRACE("data_cache_ptr, bad param");
        return MMIEMAIL_ERROR_FAIL;
    }
    
    /* added temply */
    for (i = 0; i < data_cache_ptr->attach_num; i++)
    {
        EMA_UTILS_FREEIF(PNULL, data_cache_ptr->attach_size_ptr[i]);
    }
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->data_buff_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->read_report_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->subject_ptr);
    EMA_UTILS_FREEIF(PNULL, data_cache_ptr->source_buff_ptr);
    /*data_cache_ptr->msg_id = 0;*/
    return MMIEMAIL_NO_ERROR;
}


//*****************************************************************************/
//  Description :get limited size
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int GetEditLimitedSize(
                             MMIEMAIL_EDIT_T *edit_ptr,
                             uint32 *pwsSize,
                             uint32 * pwsTotleSize
                             )
{
    HEmlSPMgr   sp_mgr_ptr = PNULL;
    HConfigMgr  cfg_mgr_ptr = PNULL;
    uint32      act_id = 0;
    int32       ret = EMAIL_SUCCESS;
    uint32      total_size = 0;
    uint32      max_size  = 0;
    EMAIL_ACT_MSGPREFERENCE_T email_pref = {0};
    char        *size_ptr = PNULL;
    wchar       *content_ptr = PNULL;
    uint32      text_size = 0;
    char        * utf8_temp_ptr = PNULL;
    EMAIL_ATTACH_T	*header_ptr = PNULL;
    uint32      result =0;
	
    if (edit_ptr == PNULL)
    {
        return EMAIL_ERROR;
    }
    
    do
    {
        sp_mgr_ptr = g_email_sp_handle;
        
        cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(sp_mgr_ptr);
        EMA_UTILS_BREAKNULL(cfg_mgr_ptr, ret, "EmailFmMgr_Compose_GetLIimitedSize, get sp config failed");
        
        ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr, &act_id);
        EMA_UTILS_BREAKIF(ret, "EmailFmMgr_Compose_GetLIimitedSize, get active act failed");
        
        ret = MMIEMAIL_SP_CFG_GetEmailPreference(cfg_mgr_ptr, act_id, &email_pref);
        EMA_UTILS_BREAKIF(ret, "EmailFmMgr_Compose_GetLIimitedSize, get email_pref failed");
        
        /* Get the attachment size*/
        header_ptr = edit_ptr->edit_doc_ptr->attach_list_ptr;
        while(header_ptr)
        {
            total_size += header_ptr->file_size;
            header_ptr = header_ptr->next_ptr;
        }
        
        size_ptr = EMA_UTILS_WSTRTOSTR(PNULL, email_pref.email_size_ptr);
        EMA_UTILS_BREAKNULL(size_ptr, ret, "EmailFmMgr_Compose_GetLIimitedSize, get size limit failed");
        
        /* Get the text size*/
        if (edit_ptr->edit_doc_ptr->edit_text.wstr_ptr)
        {
            utf8_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, edit_ptr->edit_doc_ptr->edit_text.wstr_ptr);
            EMA_UTILS_BREAKNULL(utf8_temp_ptr, ret, "out of memory");
            text_size = EMA_UTILS_STRLEN(utf8_temp_ptr);
            total_size += text_size;
            EMA_UTILS_FREE(PNULL,utf8_temp_ptr);
        }
        
        content_ptr = PNULL;
        text_size  = 0;
        /* Get the subject size*/
        
        if (edit_ptr->edit_doc_ptr->edit_subject.wstr_ptr)
        {
            utf8_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, edit_ptr->edit_doc_ptr->edit_subject.wstr_ptr);
            EMA_UTILS_BREAKNULL(utf8_temp_ptr, ret, "out of memory");
            text_size = EMA_UTILS_STRLEN(utf8_temp_ptr);
            total_size += text_size;
            EMA_UTILS_FREE(PNULL,utf8_temp_ptr);
        }
        
        text_size  = 0;
        
        /* Get the To size*/
        if (edit_ptr->edit_doc_ptr->edit_to.wstr_ptr)
        {
            utf8_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, edit_ptr->edit_doc_ptr->edit_to.wstr_ptr);
            EMA_UTILS_BREAKNULL(utf8_temp_ptr, ret, "out of memory");
            text_size = EMA_UTILS_STRLEN(utf8_temp_ptr);
            total_size += text_size;
            EMA_UTILS_FREE(PNULL,utf8_temp_ptr);
        }
        text_size  = 0;
        
        /* Get the CC size*/
        if (edit_ptr->edit_doc_ptr->edit_cc.wstr_ptr)
        {
            utf8_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, edit_ptr->edit_doc_ptr->edit_cc.wstr_ptr);
            EMA_UTILS_BREAKNULL(utf8_temp_ptr, ret, "out of memory");
            text_size = EMA_UTILS_STRLEN(utf8_temp_ptr);
            total_size += text_size;
            EMA_UTILS_FREE(PNULL,utf8_temp_ptr);
        }
        
        text_size  = 0;
        /* Get the BCC size*/
        if (edit_ptr->edit_doc_ptr->edit_bcc.wstr_ptr)
        {
            utf8_temp_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, edit_ptr->edit_doc_ptr->edit_bcc.wstr_ptr);
            EMA_UTILS_BREAKNULL(utf8_temp_ptr, ret, "out of memory");
            text_size = EMA_UTILS_STRLEN(utf8_temp_ptr);
            total_size += text_size;
            EMA_UTILS_FREE(PNULL,utf8_temp_ptr);
        }
        
        text_size  = 0;
        text_size = MMIEMAIL_FILESIZE_MAX;
        text_size = (uint32)EMA_UTILS_ATOI((unsigned char*)size_ptr);
        max_size = (uint32)EMA_UTILS_ATOI((unsigned char*)size_ptr)*MMIEMAIL_FILESIZE_MAX;
        
        if (total_size > max_size)
        {
            ret = EMAIL_ERROR;
        }
        else
        {
            if (pwsTotleSize)
            {
                *(pwsTotleSize) = total_size;
            }
            
            if (pwsSize)
            {
                //*(pwsSize)  = max_size - total_size;
                *(pwsSize)  = max_size;
            }
            
            ret = EMAIL_SUCCESS;
        }
        EMA_UTILS_FREE(PNULL, size_ptr);
        
    } while(0);
    
    result = MMIEMAIL_SP_CFG_DEL_EmailPreference(cfg_mgr_ptr, &email_pref);
    if(EMAIL_SUCCESS !=result)
    {
	EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_CFG_DEL_EmailPreference, error"));
    }
    EMA_UTILS_FREEIF(PNULL, size_ptr);
    
    return ret;
}

//*****************************************************************************/
//  Description :get store size
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL int EditGetStoreSize(uint32 *free_high, uint32 *free_low)
{
   int32       ret	  	 = EMAIL_SUCCESS;
   // 	uint32      max_size			= 0; 
   // 	uint32		act_id				= 0;
   // 	HConfigMgr   cfg_mgr_ptr = PNULL;
   // 	EMAIL_ACT_MSGPREFERENCE_T msg_pref = {0};
   // 	char	 *size_ptr  = PNULL; 

	do 
	{		
// 		cfg_mgr_ptr = MMIEMAIL_SP_GetCfgHandle(g_email_sp_handle);
// 
// 		ret = MMIEMAIL_SP_CFG_GetActiveAct(cfg_mgr_ptr, &act_id);
// 		EMA_UTILS_BREAKIF(ret, "EmailAFmMgr_Compose_CheckStoreSize, get active act failed");
// 		
// 		ret = MMIEMAIL_SP_CFG_GetMsgPreference(cfg_mgr_ptr, act_id, &msg_pref);
// 		EMA_UTILS_BREAKIF(ret, "EmailAFmMgr_Compose_CheckStoreSize, get msg pre failed");
// 		
// 		size_ptr = EMA_UTILS_WSTRTOSTR(PNULL, msg_pref.msg_size_ptr);
// 		EMA_UTILS_BREAKNULL(size_ptr, ret, "EmailAFmMgr_Compose_CheckStoreSize, get size limit failed");
// 		
// 		
// 		max_size = (uint32)EMA_UTILS_ATOI(size_ptr)*1024;

        if(MMIAPIUDISK_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD)) == DEVICE_STATUS_OK)
        {
            MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD), free_high, free_low);
            if (*free_high >0 || *free_low > MMIEMAIL_FILESIZE_OFFSET)
            {
                return ret;   
            }
        }

        
        if(MMIAPIUDISK_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)) == DEVICE_STATUS_OK)
        {
            MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1), free_high, free_low);
            if (*free_high >0 || *free_low > MMIEMAIL_FILESIZE_OFFSET)
            {
                return ret;   
            }
        }

        MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), free_high, free_low);
        
        
// 		if(pcSpace  < 30*1024)
// 		{
// 			pcSpace = 0;
// 		}
// 		else
// 		{
//             pcSpace = pcSpace - 30*1024;
// 		}		
	} while(0);
	
// 	MMIEMAIL_SP_CFG_DEL_MsgPreference(cfg_mgr_ptr, &msg_pref);
// 	EMA_UTILS_FREEIF(PNULL, size_ptr);
	return ret;
}

//*****************************************************************************/
//  Description :save as draft
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditSaveAsDraft(void)
{
    MMIEMAIL_EDIT_T *edit_ptr = s_email_edit_p;
    int result = EMAIL_SUCCESS;
    
    if(NULL == edit_ptr){
       return EMAIL_ERROR;
    }

    MMIEMAIL_ClearEmailCache(&edit_ptr->data_cache);
    EditSaveDataCache(edit_ptr, &edit_ptr->data_cache);
    
    result = EditProcSaveDraft((uint32)&edit_ptr->data_cache);    
    return result;
}


//*****************************************************************************/
//  Description :handle save as draft result
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIEMAIL_HandleEditSaveAsDraftResult(uint32 result)
{
    if (EMAIL_SUCCESS == result)
    {
        if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
        {
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
        }
        
        if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
        {
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
        }
        /*Save as draft successfully.*/
        
        MMK_CloseWin(EMAIL_WAITING_WIN_ID);
        MMK_CloseWin(EMAIL_EDIT_WIN_ID);
        if (g_save_email_on_red_key)
        {
            MMK_CloseMiddleWin(EMAIL_WIN_ID_START, EMAIL_MAX_WIN_ID);
        }
        else
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin(TXT_COMMON_MAIL_SAVED);
#endif
        }
    }
    else
    {
        MMK_CloseWin(EMAIL_WAITING_WIN_ID);
        MMK_CloseWin(EMAIL_EDIT_WIN_ID);
        if (g_save_email_on_red_key)
        {
            MMK_CloseMiddleWin(EMAIL_WIN_ID_START, EMAIL_MAX_WIN_ID);
        }
        else
        {
            MMIPUB_OpenAlertFailWin( TXT_EMAIL_SAVEASDRAFT_FAILED);
        }
    }
    if (g_save_email_on_red_key)
    {
        g_save_email_on_red_key = FALSE;
        EditExit();
    }
}

//*****************************************************************************/
//  Description :send email
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditSendEmail(void)
{
    int         result  = MMIEMAIL_ERROR_FAIL;
    EMAIL_MSGDATACACHE_T *data_cache_ptr = &s_email_edit_p->data_cache;
        
    do 
    {        
        MMIEMAIL_ClearEmailCache(&s_email_edit_p->data_cache);
        EditSaveDataCache(s_email_edit_p, &s_email_edit_p->data_cache);
        
        if (0 == data_cache_ptr->email_id)
        {
            result = MMIEMAIL_SP_Store_SaveEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), data_cache_ptr);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditSendEmail, MMIEMAIL_SP_Store_SaveEmail failed");            
        }
        else
        {
            result = MMIEMAIL_SP_Store_GetBoxIdByEmailId(EMA_GETSTORE_HANDLE(g_email_sp_handle), data_cache_ptr->email_id, EMA_SP_ACTIVE_ACCOUNT, &data_cache_ptr->folder_id);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditSendEmail, MMIEMAIL_SP_Store_GetBoxIdByEmailId failed");  
            
            result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FULL, data_cache_ptr);
            EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditSendEmail, MMIEMAIL_SP_Store_UpdateEmail failed");  
        }
        
        /*Update e-mail information.(folder)*/
        result = MMIEMAIL_SP_Store_MoveEmailtoFolder(EMA_GETSTORE_HANDLE(g_email_sp_handle),
            EMA_SP_ACTIVE_ACCOUNT,
            data_cache_ptr->folder_id, 
            EMA_SP_ACTIVE_ACCOUNT,
            EMA_OUTBOX,
            data_cache_ptr->email_id);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditSendEmail, MMIEMAIL_SP_Store_MoveEmailtoFolder failed");  

        data_cache_ptr->folder_id = EMA_OUTBOX;
        
        if (data_cache_ptr->attach_num > 0)
        {
            data_cache_ptr->mail_status = EMA_SEND_FAILED_WITH_ATTACH;
        }
        else
        {
            data_cache_ptr->mail_status = EMA_SEND_FAILED;
        }
        
        result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, data_cache_ptr);
        EMA_UTILS_BREAKIF(result, "MMIEMAIL_EditSendEmail, MMIEMAIL_SP_Store_MoveEmailtoFolder failed");  
    } while (0);
    
    return result;
}


//*****************************************************************************/
//  Description :handle edit send email result
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_HandleEditSendEmailResult(uint32 result)
{
    if (EMAIL_SUCCESS != result)
    {
        MMK_CloseWin(EMAIL_SENDING_WIN_ID);
        if (result == EME_NOSTORAGE)
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOENOUGHSPACE);
        }
        else if (result == EME_NOMEMORY || result == EMAIL_NOMEMORY)/*No memory*/
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_MEMORY);
        }
        else
        {
            MMIPUB_OpenAlertFailWin( TXT_COMM_SEND_FAILED);
        }
    }
    return EMAIL_SUCCESS;
}
/*****************************************************************************/
//  Description : add attachment
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN EditAddAttachment(
                                MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr,
                                MMIFILE_FILE_INFO_T   *file_info_ptr,
                                MMIFMM_FILE_TYPE_E  file_type
                                )
{
    //    BOOLEAN  result                                    = FALSE;
    wchar full_path_name [MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    EMAIL_ATTACH_T *temp_ptr = 0;
    EMAIL_ATTACH_T *header_ptr = 0;
    //	uint32			  attach_num = 0;
    // #ifdef DRM_SUPPORT
    //     MMIDRMFILE_HANDLE drm_handle = 0;
    //     DRM_RIGHTS_T drm_rights = {0};
    // #endif
    
    if(PNULL == file_info_ptr)
    {
        return FALSE;
    }
    
    if (edit_doc_ptr->attach_num >= EMA_MAX_ATTACH_NUM)
    {
        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_CANNT_ADD_MORE);
        s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_MAX;
        return FALSE;
    }
    
    MMIAPIFMM_CombineFullPath(file_info_ptr->device_name, file_info_ptr->device_name_len,
        file_info_ptr->dir_name, file_info_ptr->dir_name_len,
        file_info_ptr->file_name, file_info_ptr->file_name_len,
        full_path_name, &full_path_len);
    
#ifdef DRM_SUPPORT
    if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE, full_path_name))
    {   
        if (FALSE == MMIAPIDRM_IsCanForward(PNULL, PNULL, full_path_name))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_PROTECTION_NOT_OPERATE);
            return FALSE;
        }
    }
#endif
    
    temp_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ATTACH_T));
    if (PNULL == temp_ptr)
    {
        return FALSE;
    }
    EMA_UTILS_MEMSET(temp_ptr, 0x00, sizeof(EMAIL_ATTACH_T));
    
    temp_ptr->type = file_type;
    temp_ptr->file_size = file_info_ptr->file_size;
    EMA_UTILS_MEMCPY(temp_ptr->file_path, full_path_name, full_path_len * sizeof(wchar));
    temp_ptr->file_name_ptr = EMA_UTILS_WSTRDUP(PNULL, file_info_ptr->file_name);
    
    header_ptr = edit_doc_ptr->attach_list_ptr;
    if (PNULL == edit_doc_ptr->attach_list_ptr)
    {
        edit_doc_ptr->attach_list_ptr = temp_ptr;
    }
    else
    {
        while (header_ptr->next_ptr)
        {
            header_ptr = header_ptr->next_ptr;
        }
        header_ptr->next_ptr = temp_ptr;
    }
    
    edit_doc_ptr->attach_num++;
    return TRUE;
}

/*****************************************************************************/
//  Description :handle edit window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN JudgeDraftChange(void)
{
    uint32 i= 0;
    EMAIL_ATTACH_T *attach_temp1_ptr = PNULL;
    EMAIL_ATTACH_T *attach_temp2_ptr = PNULL;

    if (s_email_edit_p->edit_doc_ptr->edit_to.wstr_len != s_email_edit_p->edit_back_ptr->edit_to.wstr_len
        || 0 != EMA_UTILS_WSTRCMP(s_email_edit_p->edit_doc_ptr->edit_to.wstr_ptr, s_email_edit_p->edit_back_ptr->edit_to.wstr_ptr))
    {
        return TRUE;
    }
    else if (s_email_edit_p->edit_doc_ptr->edit_cc.wstr_len != s_email_edit_p->edit_back_ptr->edit_cc.wstr_len
        || 0 != EMA_UTILS_WSTRCMP(s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr, s_email_edit_p->edit_back_ptr->edit_cc.wstr_ptr))
    {
        return TRUE;
    }
    else if (s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_len != s_email_edit_p->edit_back_ptr->edit_bcc.wstr_len
        || 0 != EMA_UTILS_WSTRCMP(s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr, s_email_edit_p->edit_back_ptr->edit_bcc.wstr_ptr))
    {
        return TRUE;
    }
    else if (s_email_edit_p->edit_doc_ptr->edit_subject.wstr_len != s_email_edit_p->edit_back_ptr->edit_subject.wstr_len
        || 0 != EMA_UTILS_WSTRCMP(s_email_edit_p->edit_doc_ptr->edit_subject.wstr_ptr, s_email_edit_p->edit_back_ptr->edit_subject.wstr_ptr))
    {
        return TRUE;
    }
    else if (s_email_edit_p->edit_doc_ptr->edit_text.wstr_len != s_email_edit_p->edit_back_ptr->edit_text.wstr_len
        || 0 != EMA_UTILS_WSTRCMP(s_email_edit_p->edit_doc_ptr->edit_text.wstr_ptr, s_email_edit_p->edit_back_ptr->edit_text.wstr_ptr))
    {
        return TRUE;
    }
    else 
    {
        if (s_email_edit_p->edit_doc_ptr->attach_num != s_email_edit_p->edit_back_ptr->attach_num)
        {
            return TRUE;
        }
        
        attach_temp1_ptr = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
        attach_temp2_ptr = s_email_edit_p->edit_back_ptr->attach_list_ptr;
        
        for (i = 0; i < s_email_edit_p->edit_doc_ptr->attach_num; i++)
        {
            if (0 != EMA_UTILS_WSTRCMP(attach_temp1_ptr->file_path, attach_temp2_ptr->file_path))
            {
                return TRUE;
            }
            attach_temp1_ptr = attach_temp1_ptr->next_ptr;
            attach_temp2_ptr = attach_temp2_ptr->next_ptr;
        }
    }
    return FALSE;
}
/*****************************************************************************/
//  Description :handle saving email window message
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSavingEmailWin(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    #if 0
    MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    uint16 temp_size      = 0;
    #endif
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
//#ifdef MMI_PDA_SUPPORT
        MMK_PostMsg(win_id, EVT_EMA_WAITING_SAVEMAIL, 0, 0); 
//#endif
        break;
        
#if 0//ndef MMI_PDA_SUPPORT
    case MSG_TIMER: 
        if(PNULL != win_info_ptr && *(uint8 *)param == win_info_ptr->update_delay_timer_id) 
        {   
            MMK_PostMsg(win_id, EVT_EMA_WAITING_SAVEMAIL, 0, 0); 
        } 
        break;
#endif        
    case MSG_FULL_PAINT:
        err_code = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);        
        break;
        
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
    case MSG_CLOSE_WINDOW:
        EMA_UTILS_TRACE("HandleSavingEmailWin receive MSG_CLOSE_WINDOW");
        break;
    case EVT_EMA_WAITING_SAVEMAIL:
        MMIEMAIL_EmailEventInterface(MMIEMAIL_EMAIL_APP, MMIEMAIL_EVT_SAVE_EMAIL, PNULL, PNULL);
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }
    
    return err_code;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :handle location option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEmailEditOptWinMsg(
                                          EMAIL_WINDOW_ID_E win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_EDIT_LOCATE_OPT_CTRL_ID;
	uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
    uint16 richedit_index                          = 0;
    
    EMA_UTILS_TRACE("HandleLocateOptWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        GUIRICHTEXT_GetFocusItemIndex(EMAIL_EDIT_CTRL_ID, &richedit_index);
        switch(menu_id)
        {
        case EMAIL_EDIT_ATTACH_OPT_OPEN:
            {
                EMAIL_ATTACH_T *attach_temp = PNULL;
                int32          i = 0;
                
                richedit_index--;
                richedit_index--;//Added by wuhui, richedit_index > 1, subject's index = 1.
                if (s_email_edit_p->is_has_cc)
                {
                    richedit_index--;
                }
                
                if (s_email_edit_p->is_has_bcc)
                {
                    richedit_index--;
                }
                
                attach_temp = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
                if(MMIAPIEMAIL_GetIsPbSendEdit())
                {
                    MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc = {0};
					pb_edit_doc = MMIAPIEMAIL_GetSendVcardDocument();
                    attach_temp = pb_edit_doc.attach_list_ptr;
                }
#endif
                for(i = 0; i < (int)richedit_index; i++)
                {
                    attach_temp = attach_temp->next_ptr;
                }
                
                MMIAPIFMM_OpenFile(attach_temp->file_path);
                //查看选中的附件，可打开的直接打开，不支持打开的查看附件详情信息。
            }
            break;

        case EMAIL_EDIT_ATTACH_OPT_DELETE:
            MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_REMOVEATTCH_CONFIRM, EMAIL_DELETE_FILE_QUERY_WIN_ID); 
            break;
            
        default :
            break;
        }    
        MMK_CloseWin(win_id);
        break;
        
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);        
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    
    return err_code;
}
#endif
/*****************************************************************************/
//  Description :handle edit window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEditWinMsg(
                                    EMAIL_WINDOW_ID_E win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                    )
{
   MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
   MMI_CTRL_ID_T ctrl_id                          = EMAIL_EDIT_CTRL_ID;
   MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr        = PNULL;
   uint16 richedit_index                          = 0;
   uint16 item_index                              = MMIEMAIL_EDIT_RICHTEXT_TO;
   MMIFILE_FILE_INFO_T  file_info                 = {0};
//   uint16 content_num                             = 0;
   MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
 //  uint32 insert_total_size                       = 0;
 //  uint32 real_file_size                          = 0;
 //  uint32 replaceed_size                          = 0;
 //  GUIRICHTEXT_FOCUS_ELEM_T richtext_element      = {0};/*lint !e64*/
 //  uint8 *add_file_ptr                            = PNULL;
 //  uint16 add_file_len                            = 0;
   static BOOLEAN is_init_focus_item              = FALSE;
   GUI_RECT_T     richtext_rect                   = {0};
   static BOOLEAN is_switch_richtext              = FALSE;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
   MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_send_vcard_email_doc ={0};
#endif
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
   if(MMIAPIEMAIL_GetIsPbSendEdit()) /*lint !e746*/
   {
       pb_send_vcard_email_doc = MMIAPIEMAIL_GetSendVcardDocument();
       s_email_edit_p->edit_doc_ptr->attach_list_ptr = pb_send_vcard_email_doc.attach_list_ptr;
       s_email_edit_p->edit_doc_ptr->attach_num = pb_send_vcard_email_doc.attach_num;
   }
#endif
   edit_doc_ptr = s_email_edit_p->edit_doc_ptr;

   EMA_UTILS_TRACE("MMIEmail_HandleEditEditWinMsg msg_id = 0x%x", msg_id);
   switch(msg_id)
   {
   case MSG_LCD_SWITCH:
      is_switch_richtext = TRUE;
      MMIEMAIL_GetRichtextRect(&richtext_rect);
      GUIRICHTEXT_SetRect(ctrl_id, richtext_rect);
      break;
   case MSG_OPEN_WINDOW:
      MMIEMAIL_GetRichtextRect(&richtext_rect);
      GUIRICHTEXT_SetRect(ctrl_id, richtext_rect);
      is_init_focus_item = TRUE;
      GUIRICHTEXT_SetBorderColor(ctrl_id, MMI_DARK_GRAY_COLOR);
      GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
      GUIRICHTEXT_DisableClipBoard(ctrl_id);
      MMK_SetAtvCtrl(win_id, ctrl_id); 
      //MMIDEFAULT_SetBackLight(TRUE);
      is_switch_richtext = FALSE;
      //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
      GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
      break;
      
   case MSG_LOSE_FOCUS:
      {
         //MMIDEFAULT_AllowTurnOffBackLight(TRUE);
         if (GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
         {
            s_email_edit_p->focus_index = richedit_index;
         }
      }
      break;
      
   case MSG_GET_FOCUS:
       EMA_UTILS_TRACE("MMIEmail_HandleEditEditWinMsg MSG_GET_FOCUS");
       //MMIDEFAULT_AllowTurnOffBackLight(FALSE);
       MMK_SetAtvCtrl(win_id, ctrl_id);
       break;
       
   case MSG_FULL_PAINT:
       {
           MMIEMAIL_CLEAR_CLIENT(win_id);
           {
               uint32        size_limit  = 0;
               uint32        email_size  = 0;
               uint32        temp_total  = 0;
               uint32        free_high   = 0;
               uint32        free_low    = 0;
               uint32        temp_total2 = 0;
               char          *size_ptr   = PNULL;
               wchar         *temp_ptr   = PNULL;
#if !defined(MMI_PDA_SUPPORT)
               GUI_RECT_T    text_rect   = {0};
#endif
               //MMI_CTRL_ID_T titile_ctrl_id = MMK_GetWinTitleCtrlId(win_id);
               
               (void)EditGetStoreSize(&free_high, &free_low);
               
               GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
               EMA_UTILS_TRACE("HandleEditWinMsg++ the free size is %d, the size limit is %d, the msg size is %d",
                   free_low, size_limit, email_size);
//             if (email_size > size_limit ||
//                 (free_high == 0 && (email_size > free_low || MMIEMAIL_FILESIZE_OFFSET > free_low)))
//             {
//                ;
//             } 
//             else
            {
               size_ptr = EMA_UTILS_MALLOC(PNULL,50);
               if (PNULL == size_ptr)
               {
                  return MMI_RESULT_FALSE;
               }
               EMA_UTILS_MEMSET((void*)size_ptr,0x00,50);
               
               if(email_size >= 1024 * 1000)
               {
                  temp_total = ((email_size>>10)&0x3FF)*10/1024;
                  temp_total2 = (((email_size>>10)&0x3FF)*10%1024)*10/1024;
                  free_low = email_size >>20;
                  EMA_UTILS_SPRINTF(size_ptr, "%d.%d%dM", free_low, temp_total, temp_total2);
               }
                    else if (email_size >= 1024)
               {
                  temp_total = email_size & 0x3FF;
                  temp_total = temp_total *10/1024;
                  free_low = email_size >>10;
                  EMA_UTILS_SPRINTF(size_ptr, "%d.%dK", free_low, temp_total);
               }
               else
               {
                  EMA_UTILS_SPRINTF(size_ptr, "%dB", email_size);
               }
               
               if (EMA_UTILS_STRLEN(size_ptr))
               {
                  temp_ptr = EMA_UTILS_STRTOWSTR(PNULL, size_ptr);
                  EMA_UTILS_FREE(PNULL, size_ptr);
               }
               else
               {
                  EMA_UTILS_FREE(PNULL, size_ptr);
               }

#if !defined(MMI_PDA_SUPPORT)
               text_rect.top = EDIT_TEXT_RECT_TOP; 
               text_rect.bottom = EDIT_TEXT_RECT_BOTTOM; 
               text_rect.left = EDIT_TEXT_RECT_LEFT; 
               text_rect.right = EDIT_TEXT_RECT_RIGHT; 
               GUIWIN_SetTitleSubTextParam(win_id, &text_rect, PNULL, PNULL);
               GUIWIN_SetTitleSubText(win_id, TRUE, temp_ptr, EMA_UTILS_WSTRLEN(temp_ptr), TRUE);
               //GUITITLE_SetSubText(win_id, titile_ctrl_id, TRUE, temp_ptr, EMA_UTILS_WSTRLEN(temp_ptr) + 1, TRUE);
#endif
               EMA_UTILS_FREEIF(PNULL, temp_ptr);
            }
         }
         //判断是否需要画richedit的items，不需要则更新即可
         if (!is_switch_richtext &&(MMIEMAIL_EDIT_MENU_SEND == s_email_edit_p->edit_lose_focus_reason
            || MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT == s_email_edit_p->edit_lose_focus_reason
            || MMIEMAIL_EDIT_MENU_CANCLE == s_email_edit_p->edit_lose_focus_reason
            || MMIEMAIL_EDIT_MENU_SEND_OPTION == s_email_edit_p->edit_lose_focus_reason
            || MMIEMAIL_EDIT_MENU_MAX == s_email_edit_p->edit_lose_focus_reason
            || !MMK_IsFocusWin(EMAIL_EDIT_WIN_ID))
            && PNULL == param)//Add by wuhui
         {
            GUIRICHTEXT_Update(ctrl_id);
            
            if (s_email_edit_p->focus_index < MMIEMAIL_RICHTEXT_ITEM_NUM_MAX)
            {
               GUIRICHTEXT_SetFocusItem(ctrl_id, s_email_edit_p->focus_index);
            }
         }
         else
         {
            if (is_switch_richtext)
            {
               is_switch_richtext = FALSE; 
            }
            for (item_index = 0; item_index < MMIEMAIL_RICHTEXT_ITEM_NUM_MAX; item_index++)
            {
               s_email_edit_richtext_type[item_index] = MMIEMAIL_EDIT_RICHTEXT_MAX;
            }
            GUIRICHTEXT_DeleteAllItems(ctrl_id);
            GUIRICHTEXT_ResetState(ctrl_id);
            if (!AddAllEditItems())
            {
               //提示彩信编辑错误，退出编辑发送MSG_CLOSE_WINDOW消息
            }
            GUIRICHTEXT_Update(ctrl_id);
            if (s_email_edit_p->focus_index < MMIEMAIL_RICHTEXT_ITEM_NUM_MAX && is_init_focus_item)
            {
               s_email_edit_p->focus_index = GetEditItemIndex((uint32)s_email_edit_p->focus_index);
               is_init_focus_item = FALSE;
            }
            //设置焦点
            if (s_email_edit_p->focus_index< MMIEMAIL_RICHTEXT_ITEM_NUM_MAX)
            {
               GUIRICHTEXT_SetFocusItem(ctrl_id, s_email_edit_p->focus_index);
               //if (EMAIL_CONTEDIT_OPT_NODE_HALF_POP == GetEMAILEditLoseFocusReason())
               //{
               //   s_email_edit_p->focus_index = MMIEMAIL_RICHTEXT_ITEM_NUM_MAX;
               //}
            }
            GUIRICHTEXT_ScrollToFocus(ctrl_id);
         }
 
#ifdef MMI_PDA_SUPPORT
         if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[s_email_edit_p->focus_index])
         {
             GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, TXT_DELETE, TRUE);
         }
         else
         {
             GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, TRUE);
         }
#else
         if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[s_email_edit_p->focus_index])
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_VIEW, TXT_DELETE, TRUE);
         }
         else if (MMIEMAIL_EDIT_RICHTEXT_TO == s_email_edit_richtext_type[s_email_edit_p->focus_index])
         {
            if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_TO_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
            }
            else
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
            }
         }
         else if (MMIEMAIL_EDIT_RICHTEXT_CC == s_email_edit_richtext_type[s_email_edit_p->focus_index])
         {
            if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_CC_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
            }
            else
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
            }
         }
         else if (MMIEMAIL_EDIT_RICHTEXT_BCC == s_email_edit_richtext_type[s_email_edit_p->focus_index])
         {
            if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_BCC_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
            }
            else
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
            }
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);
         }
#endif         
         GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED/*GUIRICHTEXT_FOCUS_SHIFT_BASIC*/);
         s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_MAX;
      }
      break;
      
   case MSG_CTL_MIDSK:
   case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
      {
         break;
      }
      
      if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[richedit_index])
      {
#if defined(MMI_PDA_SUPPORT)
            MMK_CreateWin((uint32*)EMAIL_EDIT_ATTACH_OPT_WIN_TAB, PNULL);
#else
          EMAIL_ATTACH_T *attach_temp = PNULL;
          int32          i = 0;

          richedit_index--;
          richedit_index--;//Added by wuhui, richedit_index > 1, subject's index = 1.
          if (s_email_edit_p->is_has_cc)
          {
              richedit_index--;
          }
          
          if (s_email_edit_p->is_has_bcc)
          {
              richedit_index--;
          }
	       
          attach_temp = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
          for(i = 0; i < (int)richedit_index; i++)
          {
              attach_temp = attach_temp->next_ptr;
          }

          MMIAPIFMM_OpenFile(attach_temp->file_path);
         //查看选中的附件，可打开的直接打开，不支持打开的查看附件详情信息。
#endif
      }
      else
      {
          uint32       email_size = 0;
          uint32       size_limit = 0;
          
          GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
          
          EMA_UTILS_TRACE("HandleEditWinMsg++ the size limit is %d, the msg size is %d", size_limit, email_size);
          if (email_size >= size_limit)
          {
              MMIPUB_OpenAlertWarningWin( TXT_EMAIL_SIZE_EXCEED);
          }
          else
          {
              EditEnterCorrespondItem(ctrl_id, edit_doc_ptr);
          }
      }
      break;
      
#if defined(MMI_PDA_SUPPORT) 
    case MSG_APP_MENU:
#endif
   case MSG_CTL_OK:
   case MSG_APP_OK:
      //进入选项
      CreateEditMenuWin();
      break;
      
   case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
   case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
      if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
      {
         break;
      }
#ifdef MMI_PDA_SUPPORT
      if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[richedit_index])
      {
          GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, TXT_DELETE, TRUE);
      }
      else
      {
          GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_LEFT, STXT_RETURN, TRUE);
      }
#else
      if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[richedit_index])
      {
         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_VIEW, TXT_DELETE, TRUE);
      }
      else if (MMIEMAIL_EDIT_RICHTEXT_TO == s_email_edit_richtext_type[richedit_index])
      {
         if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_TO_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
         }
      }
      else if (MMIEMAIL_EDIT_RICHTEXT_CC == s_email_edit_richtext_type[richedit_index])
      {
         if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_CC_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
         }
      }
      else if (MMIEMAIL_EDIT_RICHTEXT_BCC == s_email_edit_richtext_type[richedit_index])
      {
         if ((GUIEDIT_GetListItemNum(EMAIL_EDIT_BCC_LIST_CTRL_ID)) < EMA_MAX_RECPT_NUM)
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);//STXT_MAIN_PB
         }
         else
         {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
         }
      }
      else
      {
         GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EDIT, STXT_RETURN, TRUE);
      }
#endif      
      break;
      
   case MSG_PROMPTWIN_OK:
      {
         GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index);
         MMIPUB_CloseQuerytWin(PNULL);
      }
      break;
      
   case MSG_PROMPTWIN_CANCEL:
      MMIPUB_CloseQuerytWin(PNULL);
      break;
      
   case MSG_MULTIM_SELECTED_RETURN:
      //选中多媒体文件返回
      {
         multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;
         MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr, multim_return_info_ptr->file_name_len, &file_info);
         //should check the size
         if (TRUE == EditAddAttachment(s_email_edit_p->edit_doc_ptr, &file_info, MMIFMM_FILE_TYPE_NORMAL))
         {
             is_switch_richtext = TRUE;
             MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
         }
      }
      break;

   case MSG_APP_RED:
        if (PNULL != edit_doc_ptr)
        {
           if (0 == edit_doc_ptr->edit_to.wstr_len && 0 == edit_doc_ptr->edit_cc.wstr_len 
               && 0 == edit_doc_ptr->edit_bcc.wstr_len && 0 == edit_doc_ptr->edit_subject.wstr_len
               && 0 == edit_doc_ptr->edit_text.wstr_len && PNULL == edit_doc_ptr->attach_list_ptr
               && MMIEMAIL_EDIT_FROM_DRAFT != s_email_edit_p->email_edit_enter_type)
           {
           }
           else
           {
               if (MMIEMAIL_EDIT_FROM_DRAFT == s_email_edit_p->email_edit_enter_type && FALSE == JudgeDraftChange())
               {
               }
               else
               {
                   uint32        email_num = 0;
                   
                   if (!MMIEMAIL_GetEmailNum((uint32)EMA_DRAFTBOX, (uint32)&email_num))
                   {
                       EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT, get email num failed!!!"));
                   }
                   else
                   {
                       if (DRAFT_MAX_EMAIL_NUMBER <= email_num)
                       {
                           MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_DRAFT_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                       }
                       else
                       {
                           g_save_email_on_red_key = TRUE;
                           MMK_SendMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_ASK_SAVEDIALOG, (DPARAM)TRUE);
                       }
                   }
               }
           }
        }
        err_code = MMI_RESULT_FALSE;
        break;
      
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
      //Changed by wuhui
      if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
      {
         break;
      }
#if !defined(MMI_PDA_SUPPORT)
      if (MMIEMAIL_EDIT_RICHTEXT_ADDFILE == s_email_edit_richtext_type[richedit_index])
      {
         //删除选中的附件，删除之前提示一个询问框。
         MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_REMOVEATTCH_CONFIRM, EMAIL_DELETE_FILE_QUERY_WIN_ID); 
      }
      else
#endif
      {
         //退出编辑主界面
         if (PNULL != edit_doc_ptr)
         {
            if (0 == edit_doc_ptr->edit_to.wstr_len && 0 == edit_doc_ptr->edit_cc.wstr_len 
               && 0 == edit_doc_ptr->edit_bcc.wstr_len && 0 == edit_doc_ptr->edit_subject.wstr_len
               && 0 == edit_doc_ptr->edit_text.wstr_len && PNULL == edit_doc_ptr->attach_list_ptr
               && MMIEMAIL_EDIT_FROM_DRAFT != s_email_edit_p->email_edit_enter_type)
            {
               MMK_CloseWin(win_id);
            }
            else
            {
               if (MMIEMAIL_EDIT_FROM_DRAFT == s_email_edit_p->email_edit_enter_type && FALSE == JudgeDraftChange())
               {
                   MMK_CloseWin(win_id);
               }
               else
               {
                   uint32        email_num = 0;
                   
                   if (!MMIEMAIL_GetEmailNum((uint32)EMA_DRAFTBOX, (uint32)&email_num))
                   {
                       EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT, get email num failed!!!"));
                       MMK_CloseWin(win_id);
                       return TRUE;
                   }
                   
                   if (DRAFT_MAX_EMAIL_NUMBER <= email_num)
                   {
                       MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_DRAFT_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                       MMK_CloseWin(win_id);
                   }
                   else
                   {
                       MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_ASKSAVE, EMAIL_SAVE_QUERY_WIN_ID); 
                   }
               }
            }
         }
      }
      break;
      
   case MSG_CLOSE_WINDOW:
       // MMIDEFAULT_AllowTurnOffBackLight(TRUE);
       if(g_is_email_by_pb)
       {
           g_is_email_by_pb =FALSE;
       }
       if (!g_save_email_on_red_key)
       {
           EditExit();
       }
      break;
      
//    case EVT_EMA_WAITING_SAVEMAIL:
//       EditSaveAsDraft(s_email_edit_p);
//       break;
      
//    case EVT_EMA_WAITING_SENDMAIL:
//       EMA_UTILS_TRACE("HandleEditWinMsg, receive EVT_EMA_WAITING_SENDMAIL");
//       EditSendEmail(s_email_edit_p);
//       break;
      
   case EVT_EMA_SEND_FAILED:
       EMA_UTILS_TRACE("HandleEditWinMsg, receive EVT_EMA_SEND_FAILED");
       MMK_CloseWin(win_id);
       MMIPUB_OpenAlertFailWin( TXT_COMM_SEND_FAILED);
       if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
       {
           MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
       }
       
       if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
       {
           MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
       }
       break;

   case EVT_EMA_SEND:
      {
         uint32 net_id = 0;
         if (PNULL != param)
         {
            net_id = *((uint32*)param);
         }
         EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_SendMsg, the msg id is %d, the net id is %d", s_email_edit_p->data_cache.email_id, net_id);
         MMIEMAIL_SP_TRS_SendEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), s_email_edit_p->data_cache.email_id, EMA_SP_ACTIVE_ACCOUNT, net_id);
      }
      break;
      
   case EVT_EMA_ASK_SAVEDIALOG:
       if (PNULL != param)
       {
           uint32       email_size = 0;
           uint32       size_limit = 0;
           MMI_STRING_T wait_str   = {0};
           uint32       free_high  = 0;
           uint32       free_low   = 0;
           
           if(EMAIL_SUCCESS == EditCheckRecipients())
           {
               (void)EditGetStoreSize(&free_high, &free_low);
               
               GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
               
               EMA_UTILS_TRACE("HandleEditMenuWinMsg++ the free size is %d, the size limit is %d, the msg size is %d",
                   free_low, size_limit, email_size);
               if (email_size > size_limit)
               {
                   MMK_CloseWin(win_id);
                   MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                   return TRUE;
               }
               
               if(free_high == 0 && (email_size > free_low || MMIEMAIL_FILESIZE_OFFSET > free_low))
               {
                   MMK_CloseWin(win_id);
                   MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);
                   return TRUE;
               }
               
               MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
               
               //开启等待窗口  
               MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,EMAIL_WAITING_WIN_ID,IMAGE_NULL,
                   ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleSavingEmailWin);
               
               //MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_WAITING_SAVEMAIL, 0, 0);
               //MMK_CloseWin(win_id);
               return TRUE;
           }
           else
           {
         //      MMK_CloseWin(win_id);
               return TRUE;
           }
       }
       else
       {
           MMK_CloseWin(win_id);
       }
      break;
      
   case EVT_EMA_ASK_DELETEFILE:
      EditRemoveAttachment();
#ifdef MMI_GUI_STYLE_TYPICAL
      MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_NONE, PNULL);
#endif
      MMK_SendMsg(win_id, MSG_FULL_PAINT, (DPARAM)TRUE);
      break;
      
   case EVT_EMA_NOSUBJECT_SEND:
      {
         uint32 size_limit = 0;
         uint32 email_size = 0;
         uint32 free_high  = 0;
         uint32 free_low   = 0;
         
         (void)EditGetStoreSize(&free_high, &free_low);
         
         GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
         
         EMA_UTILS_TRACE("HandleEditMenuWinMsg++ the free size is %d, the size limit is %d, the msg size is %d",
            free_low, size_limit, email_size);
         if (email_size > size_limit)
         {
            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
            MMK_CloseWin(win_id);
            return TRUE;
         }
         
         if(free_high == 0 && (email_size > free_low || MMIEMAIL_FILESIZE_OFFSET > free_low))
         {
            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);
            MMK_CloseWin(win_id);
            return TRUE;
         }
         
         g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
         MMIEMAIL_SelectNet();
      }
      break;
      
   default :
      err_code = MMI_RESULT_FALSE;
      break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :enter correspond item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditEnterCorrespondItem(MMI_CTRL_ID_T ctrl_id, MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr)
{
   uint16 richedit_index = 0;
   
   if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
   {
      return;
   }
   
   if (MMIEMAIL_EDIT_RICHTEXT_TO == s_email_edit_richtext_type[richedit_index])
   {
      s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_ADD_RECV;
      MMK_CreateWin((uint32*)EMAIL_EDIT_TO_LIST_WIN_TAB, PNULL);
   }
   else if (MMIEMAIL_EDIT_RICHTEXT_CC == s_email_edit_richtext_type[richedit_index])
   {
      s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_ADD_RECV;
      MMK_CreateWin((uint32*)EMAIL_EDIT_CC_LIST_WIN_TAB, PNULL);
   }
   else if (MMIEMAIL_EDIT_RICHTEXT_BCC == s_email_edit_richtext_type[richedit_index])
   {
      s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_ADD_RECV;
      MMK_CreateWin((uint32*)EMAIL_EDIT_BCC_LIST_WIN_TAB, PNULL);
   }
   else if (MMIEMAIL_EDIT_RICHTEXT_SUBJECT == s_email_edit_richtext_type[richedit_index])
   {
      s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_ADD_RECV;
      MMK_CreateWin((uint32*)EMAIL_EDIT_SUBJECT_TAB, PNULL);
   }
   else if (MMIEMAIL_EDIT_RICHTEXT_TEXT == s_email_edit_richtext_type[richedit_index])
   {
      s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_ADD_RECV;
      MMK_CreateWin((uint32*)EMAIL_EDIT_TEXT_TAB, PNULL);
   }
}

/*****************************************************************************/
//  Description :handle edit send option menu windows message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSendOptMenuWinMsg(
                                               EMAIL_WINDOW_ID_E win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_EDIT_SENDOPT_CTRL_ID;
    uint32 node_id                        = 0;

    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;

    EMA_UTILS_TRACE("HandleEditSendOptMenuWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
         MMIEMAIL_CLEAR_CLIENT(win_id);
         break;
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
       node_id = GUIMENU_GetCurNodeId(ctrl_id);
       switch (node_id)
       {
       case MMIEMAIL_EDIT_READCON_START:
          s_email_edit_p->data_cache.read_confirm = TRUE;
          break;
       case MMIEMAIL_EDIT_READCON_CANCLE:
          s_email_edit_p->data_cache.read_confirm = FALSE;
          break;
       case MMIEMAIL_EDIT_PRIORITY_LOW:
          s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_LOW;
          break;
       case MMIEMAIL_EDIT_PRIORITY_NORMAL:
          s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_NORMAL;
          break;
       case MMIEMAIL_EDIT_PRIORITY_HIGH:
          s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_HIGH;
          break;
       default :
          break;
       }
       MMK_CloseWin(win_id);
#ifdef MMI_GUI_STYLE_TYPICAL
       MMIPUB_OpenAlertSuccessWin(TXT_SET_SUCCESS);
#endif
       break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}	

/*****************************************************************************/
//  Description :handle edit menu window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditMenuWinMsg(
                                         EMAIL_WINDOW_ID_E win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param
                                         )
{
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_EDIT_MENU_CTRL_ID;
    uint32 node_id                        = 0;
    //MMI_MENU_ID_T          menu_id        = 0;
    //MMI_MENU_GROUP_ID_T    group_id       = 0;
    
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    EMA_UTILS_TRACE("MMIEmail_HandleEditEditMenuWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //打开popmenu
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入选项
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        s_email_edit_p->edit_lose_focus_reason = node_id;
        EMA_UTILS_TRACE("MMIEmail_HandleEditEditMenuWinMsg msg_id = 0x%x", node_id);
        switch(node_id)
        {
        case MMIEMAIL_EDIT_MENU_SEND:
            {
                uint32 outbox_email_num = 0;
                uint32 sentbox_email_num = 0;
                
                MMK_CloseWin(win_id);
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_OUTBOX, (uint32)&outbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get outbox's email num failed!!!"));
                    //MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (OUTBOX_MAX_EMAIL_NUMBER <= outbox_email_num)
                {
                    //MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_OUTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_SENTBOX, (uint32)&sentbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get sentbox's email num failed!!!"));
                    //MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (SENTBOX_MAX_EMAIL_NUMBER <= sentbox_email_num)
                {
                    //MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_SENTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                if (0 == s_email_edit_p->edit_doc_ptr->edit_cc.wstr_len)
                {
                    s_email_edit_p->data_cache.cc_addrs_num = 0;
                }
                if (0 == s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_len)
                {
                    s_email_edit_p->data_cache.bcc_addrs_num = 0;
                }

                MMIEMAIL_GetAllRecipients(PNULL, &s_email_edit_p->data_cache);
                if (EMAIL_SUCCESS == EditCheckRecipients())
                {
                    uint32        size_limit = 0;
                    uint32        email_size = 0;
                    uint32        free_high  = 0;
                    uint32        free_low   = 0;
                    
                    (void)EditGetStoreSize(&free_high, &free_low);
                    
                    GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
                    
                    EMA_UTILS_TRACE("HandleEditMenuWinMsg++ the free size is %d, the size limit is %d, the msg size is %d",
                        free_low, size_limit, email_size);
                    if (email_size > size_limit)
                    {
                        //MMK_CloseWin(win_id);
                        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                        return TRUE;
                    }
                    
                    if(free_high == 0 && (email_size > free_low || MMIEMAIL_FILESIZE_OFFSET > free_low))
                    {
                        //MMK_CloseWin(win_id);
                        MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);
                        return TRUE;
                    }
                    
                    if (s_email_edit_p->edit_doc_ptr->edit_subject.wstr_len == 0)
                    {
                        MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_MUST_SET_SUBJECT, EMAIL_NOSUBJECT_QUERY_WIN_ID);
                    }
                    else
                    {
                        int32         result  = 0;
                        
                        if (MMIEMAIL_EDIT_FROM_REPLY == s_email_edit_p->email_edit_enter_type
                            || MMIEMAIL_EDIT_FROM_REPLYALL == s_email_edit_p->email_edit_enter_type
                            || MMIEMAIL_EDIT_FROM_FORWARD == s_email_edit_p->email_edit_enter_type
                            || MMIEMAIL_EDIT_FROM_FORWARD_WITHOUT_ATTACH == s_email_edit_p->email_edit_enter_type)
                        {
                            if (s_email_edit_p->data_cache.mail_status != s_email_edit_p->data_cache.orig_email_status)
                            {
                                s_email_edit_p->data_cache.mail_status = s_email_edit_p->data_cache.orig_email_status;
                                s_email_edit_p->data_cache.email_id = s_email_edit_p->data_cache.orig_email_id;
                                result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, &s_email_edit_p->data_cache);
                                EMA_UTILS_BREAKIF(result, "EditProcForward, MMIEMAIL_SP_Store_UpdateEmail failed.");
                                
                                MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
                                s_email_edit_p->data_cache.email_id = 0;
                            }
                            s_email_edit_p->data_cache.mail_status = 0;
                        }
                        
                        g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                        MMIEMAIL_SelectNet();
                    }
                }

                return TRUE;
            }
            //break;
        case MMIEMAIL_EDIT_MENU_ADD_RECV:
            {
                uint16 richedit_index                 = 0;
                GUIRICHTEXT_GetFocusItemIndex(EMAIL_EDIT_CTRL_ID, &richedit_index);
                if (MMIEMAIL_EDIT_RICHTEXT_TO == s_email_edit_richtext_type[richedit_index])
                {
                    MMK_CreateWin((uint32 *)EMAIL_EDIT_TO_LIST_WIN_TAB, PNULL);
                }
                else if(MMIEMAIL_EDIT_RICHTEXT_CC == s_email_edit_richtext_type[richedit_index])
                {
                    MMK_CreateWin((uint32 *)EMAIL_EDIT_CC_LIST_WIN_TAB, PNULL);
                }
                else if(MMIEMAIL_EDIT_RICHTEXT_BCC == s_email_edit_richtext_type[richedit_index])
                {
                    MMK_CreateWin((uint32 *)EMAIL_EDIT_BCC_LIST_WIN_TAB, PNULL);
                }
            }
            break;
        case MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT:
            {
                uint32        email_num = 0;
                uint32        email_size = 0;
                uint32        size_limit = 0;
                MMI_STRING_T  wait_str   = {0};
                uint32        free_high  = 0;
                uint32        free_low   = 0;
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_DRAFTBOX, (uint32)&email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT, get email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (DRAFT_MAX_EMAIL_NUMBER <= email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_DRAFT_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                else
                {
                    if(EMAIL_SUCCESS == EditCheckRecipients())
                    {
                        (void)EditGetStoreSize(&free_high, &free_low);
                        
                        GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
                        
                        EMA_UTILS_TRACE("HandleEditMenuWinMsg++ the free size is %d, the size limit is %d, the msg size is %d",
                            free_low, size_limit, email_size);
                        if (email_size > size_limit)
                        {
                            MMK_CloseWin(win_id);
                            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                            return TRUE;
                        }
                        
                        if(free_high == 0 && (email_size > free_low || MMIEMAIL_FILESIZE_OFFSET > free_low))
                        {
                            MMK_CloseWin(win_id);
                            MMIPUB_OpenAlertWarningWin(TXT_EMAIL_NOSPACE);
                            return TRUE;
                        }
                        
                        MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
                        
                        //开启等待窗口  
                        MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,EMAIL_WAITING_WIN_ID,IMAGE_NULL,
                            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleSavingEmailWin);
                        
                        //MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_WAITING_SAVEMAIL, 0, 0);
                        MMK_CloseWin(win_id);
                        return TRUE;
                    }
                    else
                    {
                        MMK_CloseWin(win_id);
                        return TRUE;
                    }
                }
            }
            //break;
        case MMIEMAIL_EDIT_EXIT_WITHOUTSAVED:
        case MMIEMAIL_EDIT_MENU_CANCLE:
            MMK_CloseWin(win_id);
            MMK_CloseWin(EMAIL_EDIT_WIN_ID);
            return TRUE;
#if !defined(MMI_GUI_STYLE_TYPICAL)
        case MMIEMAIL_EDIT_MENU_ADD_CC:
        case MMIEMAIL_EDIT_MENU_ADD_BCC:
            {
                uint16                  checked_item_count = 0;
                uint32                  i = 0;
                GUIMENU_CHECK_INFO_T    check_info_ptr[4] = {0};
                BOOLEAN             is_has_cc = FALSE;
                BOOLEAN             is_has_bcc = FALSE;

                checked_item_count = GUIMENU_GetCheckInfo(ctrl_id, check_info_ptr, 4);
                
                for (i = 0; i < checked_item_count; i++)
                {
                    if (check_info_ptr[i].node_id == MMIEMAIL_EDIT_MENU_ADD_CC)
                    {
                        is_has_cc = TRUE;
                    }
                    else if (check_info_ptr[i].node_id == MMIEMAIL_EDIT_MENU_ADD_BCC)
                    {
                        is_has_bcc = TRUE;
                    }
                }

                if (is_has_cc)
                {
                    s_email_edit_p->is_has_cc = TRUE;
                }
                else
                {
                    s_email_edit_p->is_has_cc = FALSE;
                    EMA_UTILS_FREEIF(PNULL, s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr);
                    s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr = PNULL;
                    s_email_edit_p->edit_doc_ptr->edit_cc.wstr_len = 0;
                    s_email_edit_p->data_cache.cc_addrs_num = 0;
                }

                if (is_has_bcc)
                {
                    s_email_edit_p->is_has_bcc = TRUE;
                }
                else
                {
                    s_email_edit_p->is_has_bcc = FALSE;
                    EMA_UTILS_FREEIF(PNULL, s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr);
                    s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr = PNULL;
                    s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_len = 0;
                    s_email_edit_p->data_cache.bcc_addrs_num = 0;
				}
                if (is_has_cc || is_has_bcc)
                {
                    s_email_edit_p->focus_index = 1;
                }
                else
                {
                    s_email_edit_p->focus_index = 0;
                }
            }
            break;
#else
        case MMIEMAIL_EDIT_MENU_ADD_CC:
            if (s_email_edit_p->is_has_cc == TRUE)
            {
                s_email_edit_p->is_has_cc = FALSE;
                EMA_UTILS_FREEIF(PNULL, s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr);
                s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr = PNULL;
                s_email_edit_p->edit_doc_ptr->edit_cc.wstr_len = 0;
                s_email_edit_p->data_cache.cc_addrs_num = 0;
                if (0 == s_email_edit_p->focus_index)
                {
                    s_email_edit_p->focus_index = 0;
                }
                else
                {
                    s_email_edit_p->focus_index -= 1;
                }
            } 
            else if (FALSE == s_email_edit_p->is_has_cc)
            {
                s_email_edit_p->is_has_cc = TRUE;
                s_email_edit_p->focus_index = 1;
            }
            break;
        case MMIEMAIL_EDIT_MENU_ADD_BCC:
            if (TRUE == s_email_edit_p->is_has_bcc)
            {
                s_email_edit_p->is_has_bcc = FALSE;
                EMA_UTILS_FREEIF(PNULL, s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr);
                s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr = PNULL;
                s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_len = 0;
                s_email_edit_p->data_cache.bcc_addrs_num = 0;
                if (s_email_edit_p->focus_index == 0)
                {
                    s_email_edit_p->focus_index = 0;
                }
                else
                {
                    s_email_edit_p->focus_index -= 1;
                }
            } 
            else if (FALSE == s_email_edit_p->is_has_bcc)
            {
                s_email_edit_p->is_has_bcc = TRUE;
                if (TRUE == s_email_edit_p->is_has_cc)
                {
                    s_email_edit_p->focus_index = 2;
                } 
                else if (FALSE == s_email_edit_p->is_has_cc)
                {
                    s_email_edit_p->focus_index = 1;
                }
            }
            break;
#endif
        case MMIEMAIL_EDIT_MENU_ADD_ATTCHMENT:
            EditInsertAttachment(EMAIL_EDIT_WIN_ID, edit_doc_ptr, FALSE);
            break;
        case MMIEMAIL_EDIT_MENU_REMOVE_FILE:
            EditRemoveAttachment();
            break;
        case MMIEMAIL_EDIT_READCON_START:
            s_email_edit_p->data_cache.read_confirm = TRUE;
            break;
        case MMIEMAIL_EDIT_READCON_CANCLE:
            s_email_edit_p->data_cache.read_confirm = FALSE;
            break;
        case MMIEMAIL_EDIT_PRIORITY_LOW:
            s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_LOW;
            break;
        case MMIEMAIL_EDIT_PRIORITY_NORMAL:
            s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_NORMAL;
            break;
        case MMIEMAIL_EDIT_PRIORITY_HIGH:
            s_email_edit_p->data_cache.uipriority = EMA_PRIORITY_HIGH;
            break;
        default :
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_OK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        MMIPUB_CloseQuerytWin(PNULL);
        MMK_CloseWin(win_id);
        break;
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMIPUB_CloseQuerytWin(PNULL);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        s_email_edit_p->edit_lose_focus_reason = MMIEMAIL_EDIT_MENU_MAX;
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //返回编辑主界面
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :set list box string
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ListBoxSetString(MMI_CTRL_ID_T ctrl_id, const wchar *str_ptr, const uint16 str_len)
{
    wchar * cur_wstr                              = PNULL;
    uint16 cur_to_len                             = 0;
    wchar cur_to_wstr[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar *list_wstr                              = PNULL;
    uint16 temp_len                               = 0;
    uint16 num_len                                = 0;
    GUIEDIT_LIST_ITEM_T list_item_array[EMA_MAX_RECPT_NUM] = {0};
    uint16 list_item_num = 0;
    uint16 index = 0;
    
    if (PNULL == str_ptr || 0 == str_len)
    {
        return;
    }
    temp_len = (str_len + 1) * sizeof(wchar);
    list_wstr = EMA_UTILS_MALLOC(PNULL, temp_len);
    EMA_UTILS_MEMSET(list_wstr, 0, temp_len);
    MMI_WSTRNCPY(list_wstr, str_len, str_ptr, str_len, str_len);

    cur_wstr = list_wstr;
    GUIEDIT_ClearAllStr(ctrl_id);
    while ('\0' != *cur_wstr)
    {
        list_item_array[list_item_num].is_valid = TRUE;
//        list_item_array[list_item_num].is_object = TRUE;
        cur_to_len++;
        if (';' == *cur_wstr || '\0' == *(cur_wstr + 1))
        {
            if (';' != *cur_wstr && '\0' == *(cur_wstr + 1))
            {
                num_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, cur_to_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIEMAIL_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIEMAIL_MAX_ADDRESS_LEN, num_len);
                GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                cur_to_len = 0;
            }
            else
            {
                num_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, cur_to_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIEMAIL_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIEMAIL_MAX_ADDRESS_LEN, num_len);
//              GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                {
                    list_item_array[list_item_num].display_str_len = num_len;
                    list_item_array[list_item_num].display_str_ptr = SCI_ALLOC_APP((list_item_array[list_item_num].display_str_len+ 1)*sizeof(wchar));
                    if(PNULL == list_item_array[list_item_num].display_str_ptr)
                    {
                        return;
                    }
                    SCI_MEMSET((char*)list_item_array[list_item_num].display_str_ptr, 0x00 , (list_item_array[list_item_num].display_str_len + 1)*sizeof(wchar));
                    MMI_WSTRNCPY(list_item_array[list_item_num].display_str_ptr, list_item_array[list_item_num].display_str_len, 
                        cur_to_wstr, list_item_array[list_item_num].display_str_len, list_item_array[list_item_num].display_str_len);
                    list_item_array[list_item_num].user_str_len= list_item_array[list_item_num].display_str_len;
                    list_item_array[list_item_num].user_str_ptr= SCI_ALLOC_APP((list_item_array[list_item_num].user_str_len+ 1)*sizeof(wchar));
                    if(PNULL == list_item_array[list_item_num].user_str_ptr)
                    {
                        SCI_FREE(list_item_array[list_item_num].display_str_ptr);
                        list_item_array[list_item_num].display_str_ptr = PNULL;
                        return;
                    }
                    SCI_MEMSET((char*)list_item_array[list_item_num].user_str_ptr, 0 , (list_item_array[list_item_num].user_str_len + 1)*sizeof(wchar));
                    MMI_WSTRNCPY(list_item_array[list_item_num].user_str_ptr, list_item_array[list_item_num].user_str_len, 
                        cur_to_wstr, list_item_array[list_item_num].user_str_len, list_item_array[list_item_num].user_str_len);
                }
                cur_to_len = 0;
            }
            list_item_num++;
            if (list_item_num > EMA_MAX_RECPT_NUM)
            {
                list_item_num -= 1;
                break;
            }
        }
        cur_wstr = cur_wstr + 1;        
    }
    
    GUIEDIT_AddListItemArray(ctrl_id, list_item_array, list_item_num);
    for(index = 0; index < list_item_num; index++)
    {
        if(PNULL != list_item_array[index].display_str_ptr)
        {
            SCI_FREE(list_item_array[index].display_str_ptr);
            list_item_array[index].display_str_ptr = PNULL;
        }
        if(PNULL != list_item_array[index].user_str_ptr)
        {
            SCI_FREE(list_item_array[index].user_str_ptr);
            list_item_array[index].user_str_ptr = PNULL;
        }
    }
    EMA_UTILS_FREE(PNULL, list_wstr);
    list_wstr = PNULL;
}


/*****************************************************************************/
//  Description : set edit string
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void SetEditString(
                         const MMI_STRING_T *string_to_ptr, 
                         MMI_STRING_T * dest_ptr
                         )
{
    if(PNULL == string_to_ptr)
    {
        return;
    }

    if(string_to_ptr->wstr_len > 0)
    {
        if(dest_ptr->wstr_ptr)
        {
            EMA_UTILS_FREE(PNULL, dest_ptr->wstr_ptr);
            dest_ptr->wstr_ptr = PNULL;
			dest_ptr->wstr_len = 0;
        }

        dest_ptr->wstr_ptr = EMA_UTILS_MALLOC(PNULL, (string_to_ptr->wstr_len + 1) * sizeof(wchar));
        if(PNULL != dest_ptr->wstr_ptr)
        {
            EMA_UTILS_MEMSET(dest_ptr->wstr_ptr ,0,((string_to_ptr->wstr_len + 1)* sizeof(wchar)));
            MMI_WSTRNCPY(dest_ptr->wstr_ptr,string_to_ptr->wstr_len,
                string_to_ptr->wstr_ptr, string_to_ptr->wstr_len, string_to_ptr->wstr_len);
            dest_ptr->wstr_len = string_to_ptr->wstr_len;
        }
    }
    else if(PNULL != dest_ptr->wstr_ptr)
    {
        EMA_UTILS_MEMSET(dest_ptr->wstr_ptr, 0, (dest_ptr->wstr_len * sizeof(wchar)));
        dest_ptr->wstr_len = 0;
    }
    else
    {
        EMA_UTILS_TRACE("SetEditString string_to PNULL");
    }
}

LOCAL MMI_RESULT_E Email_AddEmailAddress(MMI_CTRL_ID_T edit_ctrl_id, MMIPB_HANDLE_T pb_handle)
{
    MMIPB_BCD_NUMBER_T pb_bcd_number               = {0};
    MMI_STRING_T       email_addr                  = {0};
    MMI_STRING_T       contact_name               = {0};
    uint32             pb_select_index             = 0;
    uint8  tele_num[MMIEMAIL_MAX_ADDRESS_LEN + 2]  = {0};

    uint16             to_number                   = 0;
    uint16             pb_select_num               = 0;
    uint32             max_number_len              = 0;
    uint8              tele_len                    = 0;
    wchar  tele_wstr[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar             *all_number_wstr             = PNULL;
    uint16            all_number_len               = 0;
    wchar             semicolon[]                  = {';', 0};
    uint16            temp_len                     = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr           = PNULL;
    uint32            index                        = 0;

    pb_select_num = MMIAPIPB_GetSelectCount(pb_handle);
    if (pb_select_num > 0)
    {
        temp_len = (MMIEMAIL_ALL_ADDRESS_LEN + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
        all_number_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
        if (PNULL == all_number_wstr)
        {
            EMA_UTILS_TRACE("malloc all_number_wstr failed");
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            return FALSE;
        }
        EMA_UTILS_MEMSET(all_number_wstr, 0, temp_len);
        
        {//解决上次的号码尾部没有分号问题
            to_number = GUIEDIT_GetListItemNum(edit_ctrl_id);
            if (0 < to_number)
            {
                num_str_ptr = EMA_UTILS_MALLOC(PNULL, to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                if (PNULL == num_str_ptr)
                {
                    EMA_UTILS_FREE(PNULL, all_number_wstr);
                    all_number_wstr = PNULL;
                    EMA_UTILS_TRACE("malloc num_str_ptr failed");
                    return FALSE;
                }
                GUIEDIT_GetListItemString(edit_ctrl_id,to_number,num_str_ptr);
                all_number_len = 0;
                for (index = 0; index < to_number; index++)
                {
                    tele_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                    MMI_WSTRNCPY(tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);                
                    if (tele_len > 0)
                    {
                        MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIEMAIL_ALL_ADDRESS_LEN, 
                            tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                        all_number_len = tele_len + all_number_len;
                        if (EMA_UTILS_WSTRCMP(all_number_wstr + all_number_len - 1, semicolon) != 0)
                        {
                            EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                            all_number_len++;
                        }
                    }
                    EMA_UTILS_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                }
                EMA_UTILS_FREE(PNULL, num_str_ptr);
                num_str_ptr = PNULL;
            }
        }
        
        EMA_UTILS_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
        EMA_UTILS_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
        EMA_UTILS_MEMSET(&email_addr, 0, sizeof(email_addr));
        email_addr.wstr_ptr = (wchar *)EMA_UTILS_MALLOC(PNULL, (MMIPB_MAX_MAIL_LEN + 1)*sizeof(wchar));
        if (PNULL == email_addr.wstr_ptr)
        {

            EMA_UTILS_TRACE("malloc email_addr failed");
            EMA_UTILS_FREE(PNULL, all_number_wstr);
            all_number_wstr = PNULL;
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            return FALSE;
        }
        EMA_UTILS_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));

        contact_name.wstr_ptr = (wchar *)EMA_UTILS_MALLOC(PNULL, (MMIEMAIL_FILE_NAME_LEN + 1)*sizeof(wchar));
        if (PNULL == contact_name.wstr_ptr)
        {
            EMA_UTILS_TRACE("malloc contact_name failed");
            EMA_UTILS_FREE(PNULL, email_addr.wstr_ptr);
            EMA_UTILS_FREE(PNULL, all_number_wstr);
            all_number_wstr = PNULL;
            MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
            return FALSE;
        }
        EMA_UTILS_MEMSET(contact_name.wstr_ptr, 0, ((MMIEMAIL_FILE_NAME_LEN + 1) * sizeof(wchar)));
  
        for (pb_select_index = 0; pb_select_index < MIN(EMA_MAX_RECPT_NUM, pb_select_num); pb_select_index++)
        {
            SCI_MEMSET(&pb_bcd_number, 0, sizeof(pb_bcd_number));
            SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIPB_MAX_MAIL_LEN + 1) * sizeof(wchar)));
            email_addr.wstr_len = 0;
            SCI_MEMSET(contact_name.wstr_ptr, 0, ((MMIEMAIL_FILE_NAME_LEN + 1) * sizeof(wchar)));
            contact_name.wstr_len = 0;
            
            MMIAPIPB_GetSelectNumber(pb_handle, &pb_bcd_number,MMIPB_BCD_NUMBER_MAX_LEN,  pb_select_index);
            MMIAPIPB_GetSelectEmail(pb_handle, &email_addr, MMIPB_MAX_MAIL_LEN, pb_select_index);
            MMIAPIPB_GetSelectName(pb_handle, &contact_name, MMIEMAIL_FILE_NAME_LEN, pb_select_index);
            //if (0 != pb_bcd_number.number_len)
            {
                max_number_len = MIN(MMISMS_NUMBER_MAX_BCD_LEN, pb_bcd_number.number_len);
                tele_len = MMIAPIPB_BCDNumberToString(&pb_bcd_number, (uint8)MMIEMAIL_MAX_ADDRESS_LEN + 2, tele_num);
//                     tele_len = MMIAPICOM_GenDispNumber( 
//                         MMIPB_GetNumberTypeFromUint8(pb_bcd_number.npi_ton), 
//                         (uint8)max_number_len, 
//                         pb_bcd_number.number,
//                         tele_num, 
//                         MMIEMAIL_MAX_ADDRESS_LEN + 2 
//                         );
                EMA_UTILS_TRACE("HandleEditCcListWinMsg,tele_len = %d",tele_len);
                if(0 != tele_len)
                {
                    MMI_STRNTOWSTR(tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, (uint8 *)tele_num, MMIEMAIL_MAX_ADDRESS_LEN, tele_len );
                    //todo to_edit_string里面是原有的的号码，在尾部添加；同时加上现在的号码
                    EMA_UTILS_WSTRLCAT(all_number_wstr + all_number_len, tele_wstr, tele_len);
                    all_number_len = all_number_len + tele_len;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                    all_number_len = all_number_len +1;
                }
                else
                {
                    EMA_UTILS_TRACE("warning_ tele_len 0");
                } 
            }//if (pb_number.number_len != 0)
            if (0 != email_addr.wstr_len)
            {
                if (0 != contact_name.wstr_len)
                {
                    EMA_UTILS_WSTRLCAT(all_number_wstr, (wchar*)L"\"", 1);
                    all_number_len = all_number_len +1;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, contact_name.wstr_ptr, contact_name.wstr_len);
                    all_number_len = all_number_len + contact_name.wstr_len;  
                    EMA_UTILS_WSTRLCAT(all_number_wstr, (wchar*)L"\"<", 2);
                    all_number_len = all_number_len +2;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                    all_number_len = all_number_len + email_addr.wstr_len;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, (wchar*)L">", 1);
                    all_number_len = all_number_len +1;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                    all_number_len = all_number_len +1;
                }
                else
                {
                    EMA_UTILS_WSTRLCAT(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                    all_number_len = all_number_len + email_addr.wstr_len;
                    EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                    all_number_len = all_number_len +1;
                }
            }
        }  
        //显示添加号码
        ListBoxSetString(
            edit_ctrl_id,
            all_number_wstr,
            all_number_len
            );
        //free email_addr
        if (PNULL != email_addr.wstr_ptr)
        {
            EMA_UTILS_FREE(PNULL, email_addr.wstr_ptr);
            email_addr.wstr_ptr = PNULL;
        }
        if (PNULL != contact_name.wstr_ptr)
        {
            EMA_UTILS_FREE(PNULL, contact_name.wstr_ptr);
            contact_name.wstr_ptr = PNULL;
        }
        EMA_UTILS_FREE(PNULL, all_number_wstr);
        all_number_wstr = PNULL;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description :handle edit to list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditToListWinMsg(
                                           EMAIL_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_CTRL_ID_T      ctrl_id                     = EMAIL_EDIT_TO_LIST_CTRL_ID;
    MMI_RESULT_E       err_code                    = MMI_RESULT_TRUE;
    uint16             to_number                   = 0;
    MMI_STRING_T       to_edit_string              = {0};
    uint8              tele_len                    = 0;
    wchar  tele_wstr[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar             *all_number_wstr             = PNULL;
    uint16            all_number_len               = 0;
    wchar             semicolon[]                  = {';', 0};
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr        = PNULL;
    uint16            temp_len                     = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr           = PNULL;
    uint32            index                        = 0;
    uint16            num_index                    = 0;
    uint8             is_have_empty                = 0;
    wchar num_temp[MMIEMAIL_MAX_ADDRESS_LEN + 1]   = {0};
    wchar             empty_wstr[]                 = {' '};
    
    if (PNULL == s_email_edit_p->edit_doc_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
    {
        to_edit_string = edit_doc_ptr->edit_to;
    }
    
    EMA_UTILS_TRACE("HandleEditToListWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //todo 打开彩信收件人的输入界面，没有收件人的时候左软键是电话簿（中软键也是进入电话簿），右软键是返回
        //当有收件人的时候左软键是确认，中软键是添加，右软键是返回
        GUIEDIT_SetListStyle(ctrl_id, GUIEDIT_LIST_STYLE_ALL);
        
        if (PNULL != to_edit_string.wstr_ptr && 0 != to_edit_string.wstr_len)
        {
            ListBoxSetString(
                ctrl_id,
                to_edit_string.wstr_ptr,
                to_edit_string.wstr_len
                );
        }
        
        //set softkey
        //GUIEDIT_SetSoftkey(ctrl_id,0,1,STXT_MAIN_PB,STXT_OK,PNULL);
        //GUIEDIT_SetSoftkey(ctrl_id,1,1,TXT_NULL,STXT_MAIN_PB,PNULL);
        //set im
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL , GUIIM_TYPE_ABC); /*lint !e655*/
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
        MMK_SetAtvCtrl(win_id, ctrl_id) ;
        break;

    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        {
            uint16      number  = 0;
            
            number = GUIEDIT_GetListItemNum(ctrl_id);
            if(EMA_MAX_RECPT_NUM <=number)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
            }
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        EMA_UTILS_MEMSET(((void *)&to_edit_string), 0, sizeof(MMI_STRING_T));
        //GUIEDIT_GetString(EMAIL_EDIT_TO_LIST_CTRL_ID, &box_to_edit_string);
        to_number = GUIEDIT_GetListItemNum(ctrl_id);
        
        if (0 < to_number)
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (to_number > EMA_MAX_RECPT_NUM -1)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
                    break;
                }
            }
            num_str_ptr = EMA_UTILS_MALLOC(PNULL, to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
            if (PNULL == num_str_ptr)
            {
                break;
            }
            GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
            
            all_number_len = 0;
            for (index = 0; index < to_number; index++)
            {
                all_number_len = all_number_len + num_str_ptr[index].str_len;
            }
            if (0 == all_number_len)
            {
                if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_to.wstr_ptr);
                    edit_doc_ptr->edit_to.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_to.wstr_len = 0;
                }
                /*MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                    MMIAPIPB_OpenMainWin(
                            MMIPB_ALL_CONTACT,//
                            MMIPB_LIST_CHECK, //IN: 号码列表的类型
                            MMIPB_LIST_FIELD_MAIL,
                            (uint16)EMA_MAX_RECPT_NUM,//如果是多选时，此项限制了多选的最大个数
                            PNULL,
                            AddContactToEditListCallback
                            );
                break;
            }
            temp_len = (all_number_len + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            //temp_len = (MMIEMAIL_ALL_ADDRESS_LEN + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            all_number_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
            if (PNULL == all_number_wstr)
            {
                EMA_UTILS_TRACE("malloc email_addr failed");
                MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
                break;
            }
            EMA_UTILS_MEMSET(all_number_wstr, 0, temp_len);
            all_number_len = 0;
            
            for (index = 0; index < to_number; index++)
            {
                tele_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                MMI_WSTRNCPY(tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);                
                
                for (num_index = 0; num_index <= tele_len; num_index++)
                {
                    MMI_WSTRNCPY(num_temp, MMIEMAIL_MAX_ADDRESS_LEN, tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    num_index = num_index - is_have_empty;
                    is_have_empty = 0;
                    if (0 == EMA_UTILS_WSTRNICMP(&(tele_wstr[num_index]), empty_wstr, 1))
                    {
                        tele_wstr[tele_len - 1] = 0;
                        tele_len--;
                        MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIEMAIL_MAX_ADDRESS_LEN, 
                            &(num_temp[num_index + 1]), MMIEMAIL_MAX_ADDRESS_LEN, tele_len - num_index);
                        is_have_empty = 1;
                    }
                    EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                }
                if (tele_len > 0)
                {
                    MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIEMAIL_ALL_ADDRESS_LEN, 
                        tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    all_number_len = tele_len + all_number_len;
                    if (EMA_UTILS_WSTRCMP(all_number_wstr + all_number_len - 1, semicolon) != 0)
                    {
                        EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                        all_number_len++;
                    }
                }
                EMA_UTILS_WSTRCPY(s_email_edit_p->data_cache.to_addrs[index], tele_wstr);
                EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                EMA_UTILS_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
            }
            
            s_email_edit_p->data_cache.to_addrs_num = to_number;
            
            //把得到的号码加入edit document中去
            EMA_UTILS_TRACE("MMIEmail_HandleEditEditWinMsg MMIEMAIL_SetEMAILTo");
            to_edit_string.wstr_ptr = all_number_wstr;
            to_edit_string.wstr_len = EMA_UTILS_WSTRLEN(all_number_wstr);
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                //根据to_edit_string得到编辑的号码个数to_number
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_RADIO,
                    (uint16)(EMA_MAX_RECPT_NUM - to_number),
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM - to_number),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToEditListCallback
                        );
            }
            else
            {
                uint32        size_limit  = 0;
                uint32        email_size  = 0;
                
                GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
                
                EMA_UTILS_TRACE("HandleEditToListWinMsg++ the size limit is %d, the msg size is %d",
                    size_limit, email_size);
                
                if(email_size + to_edit_string.wstr_len > size_limit)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                }
                else
                {
                    SetEditString(&to_edit_string, &edit_doc_ptr->edit_to);
                    MMK_CloseWin(win_id);
                }
            }
            
            EMA_UTILS_FREE(PNULL, num_str_ptr);
            num_str_ptr = PNULL;
            
            EMA_UTILS_FREE(PNULL, all_number_wstr);
            all_number_wstr = PNULL;
        }
        else
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_to.wstr_ptr);
                    edit_doc_ptr->edit_to.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_to.wstr_len = 0;
                }
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)EMA_MAX_RECPT_NUM,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToEditListCallback
                        );
            }
            else
            {
                if (PNULL != edit_doc_ptr->edit_to.wstr_ptr)
                {
                    int32 to_old_num =0;
                    int32 index_to_old_num =0;
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_to.wstr_ptr);
                    edit_doc_ptr->edit_to.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_to.wstr_len = 0;
                    to_old_num = s_email_edit_p->data_cache.to_addrs_num;
                    for(index_to_old_num=0;index_to_old_num<to_old_num;index_to_old_num++)
                    {
                        EMA_UTILS_MEMSET(s_email_edit_p->data_cache.to_addrs[index_to_old_num], 0, (EMA_MAX_ADDR_LEN + 1)*sizeof(wchar));
                        s_email_edit_p->data_cache.to_addrs_num =0;
                    }
                }
                MMK_CloseWin(win_id);
            }
        }
        break;
        
    case MMI_SELECT_CONTACT_CNF:
        Email_AddEmailAddress(ctrl_id, (MMIPB_HANDLE_T)param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 返回编辑主界面
        MMK_CloseWin(win_id);
        break;
    case MSG_ALERTWIN_CLOSED:
        MMIPUB_CloseAlertWin();
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}


/*****************************************************************************/
//  Description :handle edit cc list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditCcListWinMsg(
                                           EMAIL_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_CTRL_ID_T ctrl_id                        = EMAIL_EDIT_CC_LIST_CTRL_ID;
    MMI_RESULT_E err_code                        = MMI_RESULT_TRUE;
    uint16  to_number                            = 0;
    MMI_STRING_T  cc_edit_string                 = {0};
//    MMI_STRING_T box_to_edit_string              = {0};
//    MMI_STRING_T  all_number_str                 = {0};
    uint8  tele_len                              = 0;
    wchar  tele_wstr[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar  *all_number_wstr                      = PNULL;
    uint16   all_number_len                      = 0;
    wchar semicolon[]                            = {';', 0};
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr      = PNULL;
    uint16  temp_len                             = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr         = PNULL;
    uint16 index                                 = 0;
    uint16 num_index                             = 0;
    uint8 is_have_empty                          = 0;
    wchar num_temp[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar empty_wstr[]                           = {' '};
    
    
    if (PNULL == s_email_edit_p->edit_doc_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    
    if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr)
    {
        cc_edit_string = edit_doc_ptr->edit_cc;
    }
    EMA_UTILS_TRACE("HandleEditCcListWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //todo 打开彩信收件人的输入界面，没有收件人的时候左软键是电话簿（中软键也是进入电话簿），右软键是返回
        //当有收件人的时候左软键是确认，中软键是添加，右软键是返回
        GUIEDIT_SetListStyle(ctrl_id, GUIEDIT_LIST_STYLE_ALL);
        
        if (PNULL != cc_edit_string.wstr_ptr && 0 != cc_edit_string.wstr_len)
        {
            ListBoxSetString(
                ctrl_id,
                cc_edit_string.wstr_ptr,
                cc_edit_string.wstr_len
                );
        }
        
        //set softkey
        //GUIEDIT_SetSoftkey(ctrl_id,0,1,STXT_MAIN_PB,STXT_OK,PNULL);
        //GUIEDIT_SetSoftkey(ctrl_id,1,1,TXT_NULL,STXT_MAIN_PB,PNULL);
        //set im
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);/*lint !e655*/
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        {
            uint16      number  = 0;
            
            number = GUIEDIT_GetListItemNum(ctrl_id);
            if(EMA_MAX_RECPT_NUM <=number)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
            }
        }
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        EMA_UTILS_MEMSET(((void *)&cc_edit_string), 0, sizeof(MMI_STRING_T));
        //GUIEDIT_GetString(EMAIL_EDIT_TO_LIST_CTRL_ID, &box_to_edit_string);
        to_number = GUIEDIT_GetListItemNum(ctrl_id);

        if (0 < to_number)
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (to_number > EMA_MAX_RECPT_NUM -1)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
                    break;
                }
            }

            num_str_ptr = EMA_UTILS_MALLOC(PNULL, to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
            if (PNULL == num_str_ptr)
            {
                break;
            }
            GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
            
            all_number_len = 0;
            for (index = 0; index < to_number; index++)
            {
                all_number_len = all_number_len + num_str_ptr[index].str_len;
            }
            if (0 == all_number_len)
            {
                if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_cc.wstr_ptr);
                    edit_doc_ptr->edit_cc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_cc.wstr_len = 0;
                }
                /*MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)EMA_MAX_RECPT_NUM,//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToCcListCallback
                        );
                break;
            }
            temp_len = (all_number_len + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            //temp_len = (MMIEMAIL_ALL_ADDRESS_LEN + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            all_number_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
            if (PNULL == all_number_wstr)
            {
                EMA_UTILS_TRACE("malloc email_addr failed");
                MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
                break;
            }
            EMA_UTILS_MEMSET(all_number_wstr, 0, temp_len);
            all_number_len = 0;

            for (index = 0; index < to_number; index++)
            {
                tele_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                MMI_WSTRNCPY(tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);                
                
                for (num_index = 0; num_index <= tele_len; num_index++)
                {
                    MMI_WSTRNCPY(num_temp, MMIEMAIL_MAX_ADDRESS_LEN, tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    num_index = num_index - is_have_empty;
                    is_have_empty = 0;
                    if (0 == EMA_UTILS_WSTRNICMP(&(tele_wstr[num_index]), empty_wstr, 1))
                    {
                        tele_wstr[tele_len - 1] = 0;
                        tele_len--;
                        MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIEMAIL_MAX_ADDRESS_LEN, 
                            &(num_temp[num_index + 1]), MMIEMAIL_MAX_ADDRESS_LEN, tele_len - num_index);
                        is_have_empty = 1;
                    }
                    EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                }
                if (tele_len > 0)
                {
                    MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIEMAIL_ALL_ADDRESS_LEN, 
                        tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    all_number_len = tele_len + all_number_len;
                    if (EMA_UTILS_WSTRCMP(all_number_wstr + all_number_len - 1, semicolon) != 0)
                    {
                        EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                        all_number_len++;
                    }
                }

                EMA_UTILS_WSTRCPY(s_email_edit_p->data_cache.cc_addrs[index], tele_wstr);
                EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                EMA_UTILS_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
            }

            s_email_edit_p->data_cache.cc_addrs_num = to_number;
            //把得到的号码加入edit document中去
            EMA_UTILS_TRACE("MMIEmail_HandleEditEditWinMsg MMIEMAIL_SetEMAILCc");
            cc_edit_string.wstr_ptr = all_number_wstr;
            cc_edit_string.wstr_len = EMA_UTILS_WSTRLEN(all_number_wstr);
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                //根据to_edit_string得到编辑的号码个数to_number
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)(EMA_MAX_RECPT_NUM - to_number),
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM - to_number),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToCcListCallback
                        );
            }
            else
            {
                uint32        size_limit  = 0;
                uint32        email_size  = 0;
                
                GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
                
                EMA_UTILS_TRACE("HandleEditCcListWinMsg++ the size limit is %d, the msg size is %d",
                    size_limit, email_size);
                
                if(email_size + cc_edit_string.wstr_len > size_limit)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                }
                else
                {
                    SetEditString(&cc_edit_string,&edit_doc_ptr->edit_cc);
                    MMK_CloseWin(win_id);
                }
            }
            
            EMA_UTILS_FREE(PNULL, num_str_ptr);
            num_str_ptr = PNULL;
            
            EMA_UTILS_FREE(PNULL, all_number_wstr);
            all_number_wstr = PNULL;
        }
        else
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_cc.wstr_ptr);
                    edit_doc_ptr->edit_cc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_cc.wstr_len = 0;
                }
                /*MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToCcListCallback
                        );
            }
            else
            {
                if (PNULL != edit_doc_ptr->edit_cc.wstr_ptr)
                {
                    int32 cc_old_num =0;
                    int32 index_cc_old_num =0;
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_cc.wstr_ptr);
                    edit_doc_ptr->edit_cc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_cc.wstr_len = 0;
                    cc_old_num = s_email_edit_p->data_cache.cc_addrs_num;
                    for(index_cc_old_num=0;index_cc_old_num<cc_old_num;index_cc_old_num++)
                    {
                        EMA_UTILS_MEMSET(s_email_edit_p->data_cache.cc_addrs[index_cc_old_num], 0, (EMA_MAX_ADDR_LEN + 1)*sizeof(wchar));
                        s_email_edit_p->data_cache.cc_addrs_num =0;
                    }

                }
                MMK_CloseWin(win_id);
            }
        }
        
        break;
    case MMI_SELECT_CONTACT_CNF:
        Email_AddEmailAddress(ctrl_id, (MMIPB_HANDLE_T)param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 返回编辑主界面
        MMK_CloseWin(win_id);
        break;
    case MSG_ALERTWIN_CLOSED:
        MMIPUB_CloseAlertWin();
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}

/*****************************************************************************/
//  Description :handle edit bcc list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditBccListWinMsg(
                                            EMAIL_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_CTRL_ID_T ctrl_id                        = EMAIL_EDIT_BCC_LIST_CTRL_ID;
    MMI_RESULT_E err_code                        = MMI_RESULT_TRUE;
    uint16  to_number                            = 0;
    MMI_STRING_T  bcc_edit_string                = {0};
    uint8  tele_len                              = 0;
    wchar  tele_wstr[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar  *all_number_wstr                      = PNULL;
    uint16   all_number_len                      = 0;
    wchar semicolon[]                            = {';', 0};
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr      = PNULL;
    uint16  temp_len                             = 0;
    GUIEDIT_LIST_ITEM_STR_T	*num_str_ptr         = PNULL;
    uint16 index                                 = 0;
    uint16 num_index                             = 0;
    uint8 is_have_empty                          = 0;
    wchar num_temp[MMIEMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar empty_wstr[]                           = {' '};
    
    if (PNULL == s_email_edit_p->edit_doc_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    
    if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr)
    {
        bcc_edit_string = edit_doc_ptr->edit_bcc;
    }
    EMA_UTILS_TRACE("HandleEditBccListWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //todo 打开彩信收件人的输入界面，没有收件人的时候左软键是电话簿（中软键也是进入电话簿），右软键是返回
        //当有收件人的时候左软键是确认，中软键是添加，右软键是返回
        GUIEDIT_SetListStyle(ctrl_id, GUIEDIT_LIST_STYLE_ALL);
        
        if (PNULL != bcc_edit_string.wstr_ptr && 0 != bcc_edit_string.wstr_len)
        {
            ListBoxSetString(
                ctrl_id,
                bcc_edit_string.wstr_ptr,
                bcc_edit_string.wstr_len
                );
        }
        
        //set softkey
        //GUIEDIT_SetSoftkey(ctrl_id,0,1,STXT_MAIN_PB,STXT_OK,PNULL);
        //GUIEDIT_SetSoftkey(ctrl_id,1,1,TXT_NULL,STXT_MAIN_PB,PNULL);
        //set im
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH | GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);/*lint !e655*/
      GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
        //GUIEDIT_SetIm(EMAIL_EDIT_TO_LIST_CTRL_ID,GUIIM_TYPE_ENGLISH| GUIIM_TYPE_ABC | GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_NOTIFY_EDITBOX_MAXTEXT:
        {
            uint16      number  = 0;
            
            number = GUIEDIT_GetListItemNum(ctrl_id);
            if(EMA_MAX_RECPT_NUM <=number)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
            }
        }
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_MIDSK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        EMA_UTILS_MEMSET(((void *)&bcc_edit_string), 0, sizeof(MMI_STRING_T));
        //GUIEDIT_GetString(EMAIL_EDIT_TO_LIST_CTRL_ID, &box_to_edit_string);
        to_number = GUIEDIT_GetListItemNum(ctrl_id);
        
        if (0 < to_number)
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (to_number > EMA_MAX_RECPT_NUM -1)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_RECIPIENTS_EXCCED);
                    break;
                }
            }
            
            num_str_ptr = EMA_UTILS_MALLOC(PNULL, to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
            if (PNULL == num_str_ptr)
            {
                break;
            }
            GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
            
            all_number_len = 0;
            for (index = 0; index < to_number; index++)
            {
                all_number_len = all_number_len + num_str_ptr[index].str_len;
            }
            if (0 == all_number_len)
            {
                if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_bcc.wstr_ptr);
                    edit_doc_ptr->edit_bcc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_bcc.wstr_len = 0;
                }
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToBccListCallback
                        );
                break;
            }
            temp_len = (all_number_len + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            //temp_len = (MMIEMAIL_ALL_ADDRESS_LEN + EMA_MAX_RECPT_NUM + 1)*sizeof(wchar);
            all_number_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
            if (PNULL == all_number_wstr)
            {
                EMA_UTILS_TRACE("malloc email_addr failed");
                MMIPUB_OpenAlertFailWin(TXT_COMMON_NO_MEMORY);
                break;
            }
            EMA_UTILS_MEMSET(all_number_wstr, 0, temp_len);
            all_number_len = 0;
            
            for (index = 0; index < to_number; index++)
            {
                tele_len = MIN(MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                MMI_WSTRNCPY(tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);                
                
                for (num_index = 0; num_index <= tele_len; num_index++)
                {
                    MMI_WSTRNCPY(num_temp, MMIEMAIL_MAX_ADDRESS_LEN, tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    num_index = num_index - is_have_empty;
                    is_have_empty = 0;
                    if (0 == EMA_UTILS_WSTRNICMP(&(tele_wstr[num_index]), empty_wstr, 1))
                    {
                        tele_wstr[tele_len - 1] = 0;
                        tele_len--;
                        MMI_WSTRNCPY(&(tele_wstr[num_index]), MMIEMAIL_MAX_ADDRESS_LEN, 
                            &(num_temp[num_index + 1]), MMIEMAIL_MAX_ADDRESS_LEN, tele_len - num_index);
                        is_have_empty = 1;
                    }
                    EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                }
                if (tele_len > 0)
                {
                    MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIEMAIL_ALL_ADDRESS_LEN, 
                        tele_wstr, MMIEMAIL_MAX_ADDRESS_LEN, tele_len);
                    all_number_len = tele_len + all_number_len;
                    if (EMA_UTILS_WSTRCMP(all_number_wstr + all_number_len - 1, semicolon) != 0)
                    {
                        EMA_UTILS_WSTRLCAT(all_number_wstr, semicolon, 1);
                        all_number_len++;
                    }
                }
                
                EMA_UTILS_WSTRCPY(s_email_edit_p->data_cache.bcc_addrs[index], tele_wstr);
                EMA_UTILS_MEMSET((char *)num_temp, 0, sizeof(num_temp));
                EMA_UTILS_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
            }
            
            s_email_edit_p->data_cache.bcc_addrs_num = to_number;
            //把得到的号码加入edit document中去
            EMA_UTILS_TRACE("MMIEmail_HandleEditEditWinMsg MMIEMAIL_SetEMAILTo");
            bcc_edit_string.wstr_ptr = all_number_wstr;
            bcc_edit_string.wstr_len = EMA_UTILS_WSTRLEN(all_number_wstr);
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                //根据to_edit_string得到编辑的号码个数to_number
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)(EMA_MAX_RECPT_NUM - to_number),
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM - to_number),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToBccListCallback
                        );
            }
            else
            {
                uint32        size_limit  = 0;
                uint32        email_size  = 0;
                
                GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
                
                EMA_UTILS_TRACE("HandleEditCcListWinMsg++ the size limit is %d, the msg size is %d",
                    size_limit, email_size);
                
                if(email_size + bcc_edit_string.wstr_len > size_limit)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
                }
                else
                {
                    SetEditString(&bcc_edit_string, &edit_doc_ptr->edit_bcc);
                    MMK_CloseWin(win_id);
                }
            }
            
            EMA_UTILS_FREE(PNULL, num_str_ptr);
            num_str_ptr = PNULL;
            
            EMA_UTILS_FREE(PNULL, all_number_wstr);
            all_number_wstr = PNULL;
        }
        else
        {
            if (MSG_APP_WEB == msg_id || MSG_CTL_MIDSK == msg_id)
            {
                if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr)
                {
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_bcc.wstr_ptr);
                    edit_doc_ptr->edit_bcc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_bcc.wstr_len = 0;
                }
               /* MMIAPIPB_OpenNumberList(
                    MMIPB_LIST_FOR_MMS,
                    MMIPB_LIST_CHECK,
                    (uint16)EMA_MAX_RECPT_NUM,
                    PNULL
                    );*/
                MMIAPIPB_OpenMainWin(
                        MMIPB_ALL_CONTACT,//
                        MMIPB_LIST_CHECK, //IN: 号码列表的类型
                        MMIPB_LIST_FIELD_MAIL,
                        (uint16)(EMA_MAX_RECPT_NUM ),//如果是多选时，此项限制了多选的最大个数
                        PNULL,
                        AddContactToBccListCallback
                        );
            }
            else
            {
                if (PNULL != edit_doc_ptr->edit_bcc.wstr_ptr)
                {
                    int32 bcc_old_num =0;
                    int32 index_bcc_old_num =0;
                    EMA_UTILS_FREE(PNULL, edit_doc_ptr->edit_bcc.wstr_ptr);
                    edit_doc_ptr->edit_bcc.wstr_ptr = PNULL;
                    edit_doc_ptr->edit_bcc.wstr_len = 0;
                    bcc_old_num = s_email_edit_p->data_cache.bcc_addrs_num;
                    for(index_bcc_old_num=0;index_bcc_old_num<bcc_old_num;index_bcc_old_num++)
                    {
                        EMA_UTILS_MEMSET(s_email_edit_p->data_cache.bcc_addrs[index_bcc_old_num], 0, (EMA_MAX_ADDR_LEN + 1)*sizeof(wchar));
                        s_email_edit_p->data_cache.bcc_addrs_num =0;
                    }
                }
                MMK_CloseWin(win_id);
            }
        }
        
        break;
    case MMI_SELECT_CONTACT_CNF:
        Email_AddEmailAddress(ctrl_id, (MMIPB_HANDLE_T)param);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //todo 返回编辑主界面
        MMK_CloseWin(win_id);
        break;
    case MSG_ALERTWIN_CLOSED:
        MMIPUB_CloseAlertWin();
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}


/*****************************************************************************/
//  Description :handle edit subject list window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditSubjectWinMsg(
                                            EMAIL_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_STRING_T  subject_edit_string = {0};
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    
    if (PNULL != edit_doc_ptr)
    {
        if (PNULL != edit_doc_ptr->edit_subject.wstr_ptr)
        {
            subject_edit_string = edit_doc_ptr->edit_subject;
        }
    }
    EMA_UTILS_TRACE("HandleEditSubjectWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //todo 如果存在就显示主题
        if (PNULL != subject_edit_string.wstr_ptr)
        {
            GUIEDIT_SetString(
                EMAIL_EDIT_SUBJECT_CTRL_ID, 
                subject_edit_string.wstr_ptr,
                subject_edit_string.wstr_len
                );
        }
        MMK_SetAtvCtrl(win_id, EMAIL_EDIT_SUBJECT_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        //todo 保存主题，回到编辑主界面
        EMA_UTILS_MEMSET(&subject_edit_string, 0, sizeof(MMI_STRING_T));
        GUIEDIT_GetString( EMAIL_EDIT_SUBJECT_CTRL_ID, &subject_edit_string);
        
        if (PNULL != edit_doc_ptr)
        {
            uint32        size_limit  = 0;
            uint32        email_size  = 0;
            
            GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
            
            EMA_UTILS_TRACE("HandleEditSubjectWinMsg++ the size limit is %d, the msg size is %d",
                size_limit, email_size);
            
            if(email_size + subject_edit_string.wstr_len > size_limit)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
            }
            else
            {
                SetEditString(&subject_edit_string, &edit_doc_ptr->edit_subject);
            }
        }
        MMK_CloseWin(EMAIL_EDIT_SUBJECT_WIN_ID);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //todo 返回编辑主界面 
        MMK_CloseWin(EMAIL_EDIT_SUBJECT_WIN_ID);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :handle edit text window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditTextWinMsg(
                                            EMAIL_WINDOW_ID_E win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E     err_code               = MMI_RESULT_TRUE;
    MMI_STRING_T     mms_text_string        = {0};
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
	if (PNULL != edit_doc_ptr)
	{
		mms_text_string = edit_doc_ptr->edit_text;
	}
    EMA_UTILS_TRACE("HandleEditTextWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
        //显示文本
        if (PNULL != mms_text_string.wstr_ptr)
        {
            GUIEDIT_SetString(
                EMAIL_EDIT_TEXT_CTRL_ID, 
                mms_text_string.wstr_ptr,
                mms_text_string.wstr_len
                );
        }
        MMK_SetAtvCtrl(win_id, EMAIL_EDIT_TEXT_CTRL_ID);
        //GUIEDIT_SetCursorCurPos(EMAIL_EDIT_TEXT_CTRL_ID, 0);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
#if defined(MMI_PDA_SUPPORT) 
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        //todo 保存主题，回到编辑主界面
        EMA_UTILS_MEMSET(&mms_text_string, 0, sizeof(MMI_STRING_T));
        GUIEDIT_GetString(EMAIL_EDIT_TEXT_CTRL_ID, &mms_text_string);
        
        if (PNULL != edit_doc_ptr)
        {
            uint32        size_limit  = 0;
            uint32        email_size  = 0;
            
            GetEditLimitedSize(s_email_edit_p, &size_limit, &email_size);
            
            EMA_UTILS_TRACE("HandleEditSubjectWinMsg++ the size limit is %d, the msg size is %d",
                size_limit, email_size);
            
            if(email_size + mms_text_string.wstr_len > size_limit)
            {
                MMIPUB_OpenAlertWarningWin(TXT_EMAIL_SIZE_EXCEED);
            }
            else
            {
                SetEditString(&mms_text_string, &edit_doc_ptr->edit_text);
            }
        }
        MMK_CloseWin(EMAIL_EDIT_TEXT_WIN_ID);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //todo 返回编辑主界面 
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}


/*****************************************************************************/
//  Description :get receive number from str
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16  GetReceiverNumberFromStr(MMI_STRING_T * to_str)
{
    uint16  to_num    = 0;
    wchar  *to_wstr   = PNULL;
    uint8  *to_ptr    = PNULL;
    uint8  *temp_ptr  = PNULL;
    uint16 to_ptr_len = 0;
    
    if (PNULL == to_str)
    {
        return 0;
    }
    if (PNULL == to_str->wstr_ptr || 0 == to_str->wstr_len)
    {
        return 0;
    }
    to_wstr = to_str->wstr_ptr;
    to_ptr_len = to_str->wstr_len +1;
    to_ptr = (uint8 *)EMA_UTILS_MALLOC(PNULL, to_ptr_len);
    if (PNULL == to_ptr)
    {
        return 0;
    }
    EMA_UTILS_MEMSET( to_ptr, 0, to_ptr_len);
    MMI_WSTRNTOSTR((uint8 *)to_ptr, to_str->wstr_len, to_wstr, to_str->wstr_len, to_str->wstr_len);
    temp_ptr = to_ptr;
    while (*temp_ptr != '\0')
    {
        if (*temp_ptr == ';')
        {
            to_num++;
        }
        temp_ptr = temp_ptr + 1;
    }
    
    EMA_UTILS_FREE(PNULL, to_ptr);
    to_ptr = PNULL;
    
    return to_num;
}

/*****************************************************************************/
//  Description :add all edit item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AddAllEditItems(void)
{
   MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
   EMAIL_ATTACH_T  *add_file = PNULL;
   
   edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
   if (PNULL == edit_doc_ptr)
   {
      return FALSE;
   }
   
   EditAddToItem();
   
   if (s_email_edit_p->is_has_cc)
   {
      EditAddCcItem();
   }
   
   if (s_email_edit_p->is_has_bcc)
   {
      EditAddBccItem();
   }
   
   EditAddSubjectItem();

   //Changed by wuhui
   add_file = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
   if(MMIAPIEMAIL_GetIsPbSendEdit())
   {
       MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc = {0};
       pb_edit_doc = MMIAPIEMAIL_GetSendVcardDocument();
       add_file = pb_edit_doc.attach_list_ptr;
       if(!AddFilePathCheckVaild(add_file))  //fixed 166303
       {
           MMIPUB_OpenAlertFailWin(TXT_EMAIL_CREATE_NEW_ACC_FAILED);
           EditAddTextItem();
           return FALSE;
       }
   }
#endif
   while (PNULL != add_file)
   {  
      EditAddAddFileItem(add_file->file_name_ptr, add_file->file_size);
      add_file = add_file->next_ptr;
   }
   
   EditAddTextItem();
   
   return TRUE;
}

/*****************************************************************************/
//  Description :add to item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddToItem(void)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    //wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;

    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_to.wstr_ptr && 0 != edit_doc_ptr->edit_to.wstr_len)   
    {
        to_alloc_len = (edit_doc_ptr->edit_to.wstr_len+ 1) * sizeof(wchar);
        to_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, to_alloc_len);
        EMA_UTILS_MEMSET(to_wstr, 0, to_alloc_len);
        EMA_UTILS_WSTRLCPY(to_wstr, edit_doc_ptr->edit_to.wstr_ptr, edit_doc_ptr->edit_to.wstr_len);
        to_len = edit_doc_ptr->edit_to.wstr_len;
        EMA_UTILS_TRACE("EditAddToItem %d", to_len);
    }
    else
    {
        to_wstr = (wchar*)L" ";//temp_wch;
        to_len = EMA_UTILS_WSTRLEN(to_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_EMAIL_EDIT_TO;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = to_wstr;
    item.text_data.buf.len = to_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_24;
    
    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
    s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_TO;
    if (0 != EMA_UTILS_WSTRCMP(to_wstr, (wchar*)L" "))//temp_wch
    {
        EMA_UTILS_FREE(PNULL, to_wstr);
        to_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add cc item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddCcItem(void)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    //wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;
	
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_cc.wstr_ptr && 0 != edit_doc_ptr->edit_cc.wstr_len)   
    {
        to_alloc_len = (edit_doc_ptr->edit_cc.wstr_len+ 1) * sizeof(wchar);
        to_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, to_alloc_len);
        EMA_UTILS_MEMSET(to_wstr, 0, to_alloc_len);
        EMA_UTILS_WSTRLCPY(to_wstr, edit_doc_ptr->edit_cc.wstr_ptr, edit_doc_ptr->edit_cc.wstr_len);
        to_len = edit_doc_ptr->edit_cc.wstr_len;
        EMA_UTILS_TRACE("EditAddCcItem %d", to_len);
    }
    else
    {
        to_wstr = (wchar*)L" ";//temp_wch;
        to_len = EMA_UTILS_WSTRLEN(to_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_EMAIL_EDIT_CC;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = to_wstr;
    item.text_data.buf.len = to_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
	
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_24;
    
    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
    s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_CC;
    if (0 != EMA_UTILS_WSTRCMP(to_wstr, (wchar*)L" "))//temp_wch
    {
        EMA_UTILS_FREE(PNULL, to_wstr);
        to_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add bcc item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddBccItem(void)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    wchar *to_wstr                        = PNULL;
    uint16 to_len                         = 0;
    //wchar temp_wch[]                      = L" ";
    uint16 to_alloc_len                   = 0;
	
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_bcc.wstr_ptr && 0 != edit_doc_ptr->edit_bcc.wstr_len)   
    {
        to_alloc_len = (edit_doc_ptr->edit_bcc.wstr_len+ 1) * sizeof(wchar);
        to_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, to_alloc_len);
        EMA_UTILS_MEMSET(to_wstr, 0, to_alloc_len);
        EMA_UTILS_WSTRLCPY(to_wstr, edit_doc_ptr->edit_bcc.wstr_ptr, edit_doc_ptr->edit_bcc.wstr_len);
        to_len = edit_doc_ptr->edit_bcc.wstr_len;
        EMA_UTILS_TRACE("EditAddBccItem %d", to_len);
    }
    else
    {
        to_wstr = (wchar*)L" ";//temp_wch;
        to_len = EMA_UTILS_WSTRLEN(to_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_EMAIL_EDIT_BCC;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = to_wstr;
    item.text_data.buf.len = to_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
	
    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_24;
    
    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
    s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_BCC;
    if (0 != EMA_UTILS_WSTRCMP(to_wstr, (wchar*)L" "))//temp_wch
    {
        EMA_UTILS_FREE(PNULL, to_wstr);
        to_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add file item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddAddFileItem(wchar * file_name_ptr, uint32 file_size)
{
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    wchar *file_item_ptr                 = PNULL;
    uint32 file_item_len                   = 0;
//    wchar temp_wch[]                      = L" ";
//    uint16 to_alloc_len                   = 0;
    uint16 file_name_len                  = 0;
    char   temp_str[16]                   = {0};
    wchar  *file_size_ptr                 = PNULL;
    int32  ret = 0;
    uint32 len = 0;
    uint32 offset_pos = 0;
    
    do 
    {
        file_name_len = EMA_UTILS_WSTRLEN(file_name_ptr);
//         if (0 != file_name_len)
//         {
//             file_item_ptr = file_name_ptr;
//             file_str_len = file_name_len;
//         }
//         else
//         {
//             file_item_ptr = temp_wch;
//             file_str_len = EMA_UTILS_WSTRLEN(file_item_ptr);
//         }
        
        
        EMA_UTILS_SPRINTF(temp_str, "%dB", file_size);
        file_size_ptr = EMA_UTILS_STRTOWSTR(PNULL, temp_str);
        EMA_UTILS_BREAKNULL(file_size_ptr, ret, "EditAddAddFileItem, EMA_UTILS_STRTOWSTR failed");
        
        len = (file_name_len + EMA_UTILS_WSTRLEN(file_size_ptr) + 2 + 1) * sizeof(wchar);
        
        file_item_ptr = EMA_UTILS_MALLOC(PNULL, len);
        EMA_UTILS_BREAKNULL(file_item_ptr, ret, "out of memory");
        EMA_UTILS_MEMSET(file_item_ptr, 0x00, len);
        EMA_UTILS_WSTRCAT(file_item_ptr, (wchar*)L"(");
        EMA_UTILS_WSTRCAT(file_item_ptr, file_size_ptr);
        EMA_UTILS_WSTRCAT(file_item_ptr, (wchar*)L")");
        offset_pos = EMA_UTILS_WSTRLEN(file_size_ptr) + 2;
        if (file_name_len > 0)
        {
            EMA_UTILS_MEMCPY(file_item_ptr + offset_pos, file_name_ptr, file_name_len * sizeof(wchar));
        }
        EMA_UTILS_FREE(PNULL, file_size_ptr);
        file_item_len = offset_pos + file_name_len;

        item.img_type = GUIRICHTEXT_IMAGE_RES;
        item.img_data.res.id = IMAGE_MMS_ACCESSORY_FILE;
        
        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = file_item_ptr;
        item.text_data.buf.len = file_item_len;
        item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
        
        item.text_set_font = TRUE;
        item.setting_font = SONG_FONT_24;
        
        item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
        item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
        
        GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
        s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_ADDFILE;
        EMA_UTILS_FREE(PNULL, file_item_ptr);
        
    } while (0);


//     if (0 != EMA_UTILS_WSTRCMP(file_item_ptr, temp_wch))
//     {
//         EMA_UTILS_FREE(PNULL, file_name_wstr);
//         file_name_wstr = PNULL;
//     }
}

/*****************************************************************************/
//  Description :add subject item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddSubjectItem(void)
{
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    wchar *subject_wstr                   = PNULL;
    uint16 subject_len                    = 0;
    //wchar temp_wch[]                      = L" ";
    uint16 temp_len                       = 0;

    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;

    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_subject.wstr_ptr && 0 != edit_doc_ptr->edit_subject.wstr_len)   
    {
        temp_len = (edit_doc_ptr->edit_subject.wstr_len + 1) * sizeof(wchar);
        subject_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
        EMA_UTILS_MEMSET((char *)subject_wstr, 0, temp_len);
        subject_len = MIN(EMA_MAX_SUBJ_LEN , edit_doc_ptr->edit_subject.wstr_len);
        EMA_UTILS_WSTRLCPY(subject_wstr, edit_doc_ptr->edit_subject.wstr_ptr, subject_len);
        
    }
    else
    {
        subject_wstr = (wchar*)L" ";//temp_wch;
        subject_len = EMA_UTILS_WSTRLEN(subject_wstr);
    }
    
    item.img_type = GUIRICHTEXT_IMAGE_RES;
    item.img_data.res.id = IMAGE_MMS_EDIT_SUBJECT;
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = subject_wstr;
    item.text_data.buf.len = subject_len;
    item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

    item.text_set_font = TRUE;
    item.setting_font = SONG_FONT_24;
    
    item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    //item.tag_type = GUIRICHTEXT_TAG_NORMAL;
    item.align_type = GUIRICHTEXT_ALIGN_RIGHT;
    
    GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
    s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_SUBJECT;
    if (0 != EMA_UTILS_WSTRCMP(subject_wstr, (wchar*)L" "))//temp_wch
    {
        EMA_UTILS_FREE(PNULL, subject_wstr);
        subject_wstr = PNULL;
    }
}

/*****************************************************************************/
//  Description :add text item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditAddTextItem(void)
{
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
    uint16 index                          = 0;
    wchar *text_wstr                   = PNULL;
    uint16 text_len                    = 0;
    //wchar temp_wch[]                      = L" ";
    uint16 temp_len                       = 0;
    MMI_STRING_T text_str                 = {0};
    {
        edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
        
        if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->edit_text.wstr_ptr && 0 != edit_doc_ptr->edit_text.wstr_len)   
        {
            temp_len = (edit_doc_ptr->edit_text.wstr_len + 1) * sizeof(wchar);
            text_wstr = (wchar *)EMA_UTILS_MALLOC(PNULL, temp_len);
            EMA_UTILS_MEMSET((char *)text_wstr, 0, temp_len);
            text_len = MIN(EMA_MAX_TEXT_BODY_SIZE , edit_doc_ptr->edit_text.wstr_len);
            EMA_UTILS_WSTRLCPY(text_wstr, edit_doc_ptr->edit_text.wstr_ptr, text_len);
            
        }
        else
        {
            text_wstr = (wchar*)L" ";//temp_wch;
            text_len = EMA_UTILS_WSTRLEN(text_wstr);
        }
        
        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = text_wstr;
        item.text_data.buf.len = text_len;
        item.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
        
        item.text_set_font = TRUE;
        item.setting_font = SONG_FONT_21;       
        item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
        
        GUIRICHTEXT_AddItem(EMAIL_EDIT_CTRL_ID, &item, &index);
        s_email_edit_richtext_type[index] = MMIEMAIL_EDIT_RICHTEXT_TEXT;
        if (0 != EMA_UTILS_WSTRCMP(text_wstr, (wchar*)L" "))//temp_wch
        {
            EMA_UTILS_FREE(PNULL, text_wstr);
            text_wstr = PNULL;
        }
    }
}


/*****************************************************************************/
//  Description :create edit menu window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateEditMenuWin(void)
{
    MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    uint16                     menu_index   = 0;
    MMI_CTRL_ID_T	            ctrl_id      = EMAIL_EDIT_MENU_CTRL_ID;
    //   uint16                     content_num  = 0;
    //   uint32             additional_file_num  = 0;
    GUIMENU_POP_SEL_INFO_T     pop_info     = {0};
    uint16                  richedit_index  = 0;
    
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    if (PNULL == edit_doc_ptr)
    {
        return;
    }
    
    MMK_CreateWin((uint32 *)EMAIL_EDIT_MENU_WIN_TAB, PNULL);
    GUIMENU_CreatDynamic(PNULL, EMAIL_EDIT_MENU_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);
    
    GUIRICHTEXT_GetFocusItemIndex(EMAIL_EDIT_CTRL_ID, &richedit_index);
    
    if (edit_doc_ptr->edit_to.wstr_len != 0 || edit_doc_ptr->edit_cc.wstr_len != 0
        || edit_doc_ptr->edit_bcc.wstr_len != 0)
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_SEND,             //text_ptr 
            MMIEMAIL_EDIT_MENU_SEND,	 //node_id
            0,                          //parent_node_id
            menu_index++,               //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_NEW_MAIL,
#endif
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_SEND,             //text_ptr 
            MMIEMAIL_EDIT_MENU_SEND,	 //node_id
            0,                          //parent_node_id
            menu_index++,               //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_NEW_MAIL,
#endif
            TRUE
            );
    }
    
    if (MMIEMAIL_ATTCHMENT_NUM_MAX <= edit_doc_ptr->attach_num) 
    {
        InsertNodeByLabel(
            ctrl_id,                          //ctrl_id
            TXT_EMAIL_ADDFILE,                //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_ATTCHMENT, //node_id
            0,                                //parent_node_id
            menu_index++,                     //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_ADD_ATTACH,
#endif
            TRUE
            );
    } 
    else
    {
        InsertNodeByLabel(
            ctrl_id,                          //ctrl_id
            TXT_EMAIL_ADDFILE,                //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_ATTCHMENT, //node_id
            0,                                //parent_node_id
            menu_index++,                     //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_ADD_ATTACH,
#endif
            FALSE
            );
    }
    
    // 
    //    InsertNodeByLabel(
    //       ctrl_id,                        //ctrl_id
    //       TXT_COMM_SEND_OPTION,           //text_ptr 
    //       MMIEMAIL_EDIT_MENU_SEND_OPTION, //node_id
    //       0,                              //parent_node_id
    //       menu_index++,                   //index
    //       FALSE
    //       );
    
    InsertNodeByLabel(
        ctrl_id,                        //ctrl_id
        TXT_COMMON_PRIORITY,             //text_ptr 
        MMIEMAIL_EDIT_SENDOPT_PRIORITY, //node_id
        0,                              //parent_node_id
        menu_index++,                   //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_EMAIL_OPTMENU_PROPRITY,
#endif
        FALSE
        );
    
    menu_index = 0;
    InsertNodeByLabel(
        ctrl_id,                        //ctrl_id
        TXT_COMM_LOW,                  //text_ptr
        MMIEMAIL_EDIT_PRIORITY_LOW,     //node_id
        MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
        menu_index++,                   //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_NULL,
#endif
        FALSE
        );
    
    InsertNodeByLabel(
        ctrl_id,                        //ctrl_id
        TXT_NORMAL,               //text_ptr 
        MMIEMAIL_EDIT_PRIORITY_NORMAL,  //node_id
        MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
        menu_index++,                   //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_NULL,
#endif
        FALSE
        );
    
    InsertNodeByLabel(
        ctrl_id,                        //ctrl_id
        TXT_COMMON_HIGH,                 //text_ptr 
        MMIEMAIL_EDIT_PRIORITY_HIGH,    //node_id
        MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
        menu_index++,                   //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_NULL,
#endif
        FALSE
        );
    
    InsertNodeByLabel(
        ctrl_id,                        //ctrl_id
        TXT_COMM_CC_BCC,             //text_ptr 
        MMIEMAIL_EDIT_MENU_SET_CC_BCC, //node_id
        0,                              //parent_node_id
        menu_index++,                   //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_EMAIL_OPTMENU_CC_BCC,
#endif
        FALSE
        );
    
    menu_index = 0;
    if (s_email_edit_p->is_has_cc)
    {
        InsertNodeByLabel(
            ctrl_id,                   //ctrl_id
            TXT_EMAIL_ADDCC,           //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_CC, //node_id
            MMIEMAIL_EDIT_MENU_SET_CC_BCC,                         //parent_node_id
            menu_index++,              //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_NULL,
#endif
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,                   //ctrl_id
            TXT_EMAIL_ADDCC,           //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_CC, //node_id
            MMIEMAIL_EDIT_MENU_SET_CC_BCC,                         //parent_node_id
            menu_index++,              //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_NULL,
#endif
            FALSE
            );
    }
    
    if (s_email_edit_p->is_has_bcc)
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_EMAIL_ADDBCC,           //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_BCC, //node_id
            MMIEMAIL_EDIT_MENU_SET_CC_BCC,                          //parent_node_id
            menu_index++,               //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_NULL,
#endif
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_EMAIL_ADDBCC,           //text_ptr 
            MMIEMAIL_EDIT_MENU_ADD_BCC, //node_id
            MMIEMAIL_EDIT_MENU_SET_CC_BCC,                          //parent_node_id
            menu_index++,               //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_NULL,
#endif
            FALSE
            );
    }
    
    if (edit_doc_ptr->edit_to.wstr_len != 0 || edit_doc_ptr->edit_cc.wstr_len != 0
        || edit_doc_ptr->edit_bcc.wstr_len != 0 || edit_doc_ptr->edit_subject.wstr_len != 0
        || edit_doc_ptr->edit_text.wstr_len != 0 || edit_doc_ptr->attach_list_ptr != PNULL)
    {
        InsertNodeByLabel(
            ctrl_id,                          //ctrl_id
            TXT_COMM_SAVE_AS_DRAFT,            //text_ptr 
            MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT, //node_id
            0,                                //parent_node_id
            menu_index++,                     //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_SAVE_AS_DRAFT,
#endif
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,                          //ctrl_id
            TXT_COMM_SAVE_AS_DRAFT,            //text_ptr 
            MMIEMAIL_EDIT_MENU_SAVE_AS_DRAFT, //node_id
            0,                                //parent_node_id
            menu_index++,                     //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
            IMAGE_EMAIL_OPTMENU_SAVE_AS_DRAFT,
#endif
            TRUE
            );
    }
    
    InsertNodeByLabel(
        ctrl_id,                         //ctrl_id
        TXT_EMAIL_EXIT_WITHOUT_SAVE,     //text_ptr 
        MMIEMAIL_EDIT_EXIT_WITHOUTSAVED, //node_id
        0,                               //parent_node_id
        menu_index++,                    //index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        IMAGE_EMAIL_OPTMENU_NOT_SAVE,
#endif
        FALSE
        );
    
    pop_info.is_static = FALSE;
    pop_info.ctrl_id = ctrl_id;
    
    if (s_email_edit_p->data_cache.read_confirm == TRUE)
    {
        pop_info.node_id = MMIEMAIL_EDIT_READCON_START;
    }
    else
    {
        pop_info.node_id = MMIEMAIL_EDIT_READCON_CANCLE;
    }
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctrl_id, MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM, GUIMENU_STYLE_POPUP_RADIO);
    
    pop_info.node_id = MMIEMAIL_EDIT_SENDOPT_PRIORITY;
    if (s_email_edit_p->data_cache.uipriority == EMA_PRIORITY_HIGH)
    {
        pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_HIGH;
    }
    else if(s_email_edit_p->data_cache.uipriority == EMA_PRIORITY_LOW)
    {
        pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_LOW;
    }
    else
    {
        pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_NORMAL;
    }
    GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    GUIMENU_SetSubMenuStyle(ctrl_id, MMIEMAIL_EDIT_SENDOPT_PRIORITY, GUIMENU_STYLE_POPUP_RADIO);
    
    //Add by wuhui
    pop_info.node_id = MMIEMAIL_EDIT_MENU_SET_CC_BCC;
    if (s_email_edit_p->is_has_cc == TRUE)
    {
        pop_info.node_id = MMIEMAIL_EDIT_MENU_ADD_CC;
        GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    }
    
    if(s_email_edit_p->is_has_bcc == TRUE)
    {
        pop_info.node_id = MMIEMAIL_EDIT_MENU_ADD_BCC;
        GUIMENU_SetPopItemStatus(TRUE, &pop_info);
    }
    GUIMENU_SetSubMenuStyle(ctrl_id, MMIEMAIL_EDIT_MENU_SET_CC_BCC, GUIMENU_STYLE_POPUP_CHECK);
    
    return;
}

/*****************************************************************************/
//  Description :create send option menu window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void CreateSendOptMenuWin(void)
{
   MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
   uint16 menu_index                       = 0;
   uint16 child_index				          = 0;
   MMI_CTRL_ID_T	ctrl_id                  = EMAIL_EDIT_SENDOPT_CTRL_ID;
   GUIMENU_POP_SEL_INFO_T pop_info         = {0};
   
   edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
   if (PNULL == edit_doc_ptr)
   {
      return;
   }
   
   MMK_CreateWin((uint32 *)EMAIL_EDIT_SENDOPT_WIN_TAB,PNULL);
   GUIMENU_CreatDynamic(PNULL, EMAIL_EDIT_SENDOPT_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);
   
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_EMAIL_READCONFIRM,//text_ptr 
      MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM,//node_id
      0, //parent_node_id
      menu_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_COMMON_PRIORITY,//text_ptr 
      MMIEMAIL_EDIT_SENDOPT_PRIORITY,//node_id
      0, //parent_node_id
      menu_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   // read confirm
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_COMMON_ON,//text_ptr 
      MMIEMAIL_EDIT_READCON_START,//node_id
      MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM, //parent_node_id
      child_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_COMMON_OFF,//text_ptr 
      MMIEMAIL_EDIT_READCON_CANCLE,//node_id
      MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM, //parent_node_id
      child_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   child_index = 0;
   // priority 
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_COMM_LOW,//text_ptr 
      MMIEMAIL_EDIT_PRIORITY_LOW,//node_id
      MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
      child_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_NORMAL,//text_ptr 
      MMIEMAIL_EDIT_PRIORITY_NORMAL,//node_id
      MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
      child_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   InsertNodeByLabel(
      ctrl_id,//ctrl_id
      TXT_COMMON_HIGH,//text_ptr 
      MMIEMAIL_EDIT_PRIORITY_HIGH,//node_id
      MMIEMAIL_EDIT_SENDOPT_PRIORITY, //parent_node_id
      child_index++,//index
#if defined(MMI_PDA_SUPPORT)//@tao.xue
      IMAGE_NULL,
#endif
      FALSE
      );
   
   pop_info.is_static = FALSE;
   pop_info.ctrl_id = ctrl_id;

	if (s_email_edit_p->data_cache.read_confirm == TRUE)
	{
		pop_info.node_id = MMIEMAIL_EDIT_READCON_START;
	}
	else
	{
		pop_info.node_id = MMIEMAIL_EDIT_READCON_CANCLE;
	}
	GUIMENU_SetPopItemStatus(TRUE, &pop_info);
	GUIMENU_SetSubMenuStyle(ctrl_id, MMIEMAIL_EDIT_SENDOPT_READ_CONFIRM,  GUIMENU_STYLE_POPUP_RADIO);

	pop_info.node_id = MMIEMAIL_EDIT_SENDOPT_PRIORITY;
	if (s_email_edit_p->data_cache.uipriority == EMA_PRIORITY_HIGH)
	{
		pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_HIGH;
	}
	else if(s_email_edit_p->data_cache.uipriority == EMA_PRIORITY_LOW)
	{
		pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_LOW;
	}
	else
	{
		pop_info.node_id = MMIEMAIL_EDIT_PRIORITY_NORMAL;
	}
	GUIMENU_SetPopItemStatus(TRUE, &pop_info);
	GUIMENU_SetSubMenuStyle(ctrl_id, MMIEMAIL_EDIT_SENDOPT_PRIORITY,  GUIMENU_STYLE_POPUP_RADIO);
    
    return;    
}

/*****************************************************************************/
//  Description :insert node
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
#if defined(MMI_PDA_SUPPORT)//@tao.xue
                             MMI_IMAGE_ID_T      select_icon_id, //item selected icon id
#endif
                             BOOLEAN            is_graged
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
#if defined(MMI_PDA_SUPPORT)//@tao.xue
    node_item.select_icon_id = select_icon_id;
#endif
    node_item.is_grayed = is_graged;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}


/*****************************************************************************/
//  Description :check the email address is Legality
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_EditCheckFieldLegality(wchar* addr_ptr)
{
   char       *field_ptr    = PNULL;
   char       *next_addr_ptr  = PNULL;
   int32      result     = MMIEMAIL_NO_ERROR; /*must be init success*/
   
   if (PNULL == addr_ptr)
   {
      EMA_UTILS_TRACE("EME_SetAddrField(),input parameters is invalid!!!!");
      return MMIEMAIL_ERROR_FAIL;
   }

   do
   {
       field_ptr = EMA_UTILS_WSTRTOUTF8(PNULL, addr_ptr);
       if(field_ptr == NULL)
       {
           return MMIEMAIL_ERROR_FAIL;
       }
       
       next_addr_ptr = field_ptr;
       while(*next_addr_ptr)
       {
           char* name_ptr = NULL;
           char* email_ptr = NULL;
           
           if(*next_addr_ptr == ' ' || *next_addr_ptr == '\t' || *next_addr_ptr == ';' /*|| *next_addr_ptr == ','*/)
           {
               next_addr_ptr++;
               continue;
           }
           
           email_ptr = next_addr_ptr;
           while(*next_addr_ptr /*&& *next_addr_ptr != ',' */&& *next_addr_ptr != ';')
           {
               //process email
               if(*next_addr_ptr == '\"')
               {
                   *next_addr_ptr = 0;
                   name_ptr = ++next_addr_ptr;
                   while(*next_addr_ptr)
                   {
                       if(*next_addr_ptr == '\"')
                       {
                           *next_addr_ptr = 0;
                           break;
                       }
                       next_addr_ptr++;
                   }
               }
               
               //process email
               if(*next_addr_ptr == '<')
               {
                   *next_addr_ptr = 0;
                   if(NULL == name_ptr)
                   {
                       name_ptr = email_ptr;
                       if(*(name_ptr + EMA_UTILS_STRLEN(name_ptr)) == ' ')
                       {
                           *(name_ptr + EMA_UTILS_STRLEN(name_ptr)) = 0;
                       }
                   }
                   
                   email_ptr = ++next_addr_ptr;
                   while(*next_addr_ptr)
                   {
                       if(*next_addr_ptr == '>')
                       {
                           *next_addr_ptr = 0;
                           break;
                       }
                       next_addr_ptr++;
                   }
               }
               
               next_addr_ptr++;
           }
           
           if(NULL != email_ptr)
           {
               int32 len =0;
               char *signal_email_ptr =PNULL;
               char *pont_prt = PNULL;
               char * at_prt= PNULL;
               char * temp_email_ptr = email_ptr;
               while(temp_email_ptr)
               {
                   if(*temp_email_ptr && (*temp_email_ptr != ';'))
                   {
                       temp_email_ptr++;
                       len++;
                   }
                   else
                   {
                       break;
                   }
               }
               signal_email_ptr =EMA_UTILS_MALLOC(PNULL,len+1);
               if(signal_email_ptr)
               {
                   EMA_UTILS_MEMSET(signal_email_ptr, 0, len+1);
                   strncpy(signal_email_ptr,email_ptr,len);
                   if('.'==*(signal_email_ptr+(len-1)))
                   {
                       result = MMIEMAIL_ERROR_INVALID_ADDR;
                       break;
                   }
	            at_prt = EMA_UTILS_STRCHR(signal_email_ptr, '@');
               }
               
               if(at_prt == NULL /*|| NULL != EMA_UTILS_STRCHR(++at_prt,'@')*/)
               {
                   result = MMIEMAIL_ERROR_INVALID_ADDR;
                   EMA_UTILS_FREEIF(PNULL,signal_email_ptr);/*lint !e774*/
                   break;
               }
               
               pont_prt = EMA_UTILS_STRCHR(at_prt, '.');
               if(NULL == pont_prt /*|| pont_prt == email_ptr + EMA_UTILS_STRLEN(email_ptr) || pont_prt == at_prt + 1*/)
               {
                   result = MMIEMAIL_ERROR_INVALID_ADDR;
                   break;
               }
               EMA_UTILS_FREEIF(PNULL,signal_email_ptr);/*lint !e774*/
           }
           else
           {
               result = MMIEMAIL_ERROR_INVALID_ADDR;
               break;
           }
      }
      
   } while(0);
   
   /*error handling */
   EMA_UTILS_FREE(PNULL, field_ptr);
   
   return result;
}
/*****************************************************************************/
// 	Description: MMIEMAIL_ConnectNet
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void MMIEMAIL_ConnectNet(void)
{
    EMASP_PRFITEM_VAL      val = {0}; 
    MN_DUAL_SYS_E          dual_sys = MN_DUAL_SYS_1;
    uint32                 result =0;
	
    result = MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_NET_PATH,&val);
    if(EMAIL_SUCCESS != result)
    {
	EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem, error"));
    }

    dual_sys = (uint32)val.pData;

   if (dual_sys < MMI_DUAL_SYS_MAX)
   {
      if (!MMIAPIPHONE_IsSimOk(dual_sys))
      {
         MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
         return;
      }
      g_email_is_use_wifi = FALSE;
      if (g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
      {
         MMIEMAIL_CreateSendingWin(dual_sys);
      }
      else if(g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
      {
         MMIEMAIL_CreateRecvingWaitingWin(dual_sys);
      }
      else if(g_email_is_send == EMA_TRANS_TYPE_SEND_RR)
      {
         MMIEMAIL_PDPActive(dual_sys);
      }
   }
   else
   {
#ifdef MMI_WIFI_SUPPORT
      g_email_is_use_wifi = TRUE;
      if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
      {
         if(g_email_is_send == EMA_TRANS_TYPE_SEND_EMAIL)
         {
            MMIEMAIL_CreateSendingWin(0);
            EMA_UTILS_TRACE("MMIEMAIL_SelectNet the wifi is open, use wifi to send msg");
            if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
            {

            }
            else if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
            {
               MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RESEND, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_BOX_WIN_ID))
            {
               MMK_PostMsg(EMAIL_BOX_WIN_ID, EVT_EMA_RESEND, 0, 0);
            }
         }
         else if (g_email_is_send == EMA_TRANS_TYPE_RETRIEVE_EMAIL)
         {
            MMIEMAIL_CreateRecvingWaitingWin(0);
            EMA_UTILS_TRACE("MMIEMAIL_SelectNet the wifi is open, use wifi to retrieve msg");
            if (MMK_IsOpenWin(EMAIL_VIEW_WIN_ID))
            {
               MMK_PostMsg(EMAIL_VIEW_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
            {
               MMK_PostMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
            else if (MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
            {
               MMK_PostMsg(EMAIL_SETTING_WIN_ID, EVT_EMA_RECEIVE, 0, 0);
            }
         }
         else if (g_email_is_send == EMA_TRANS_TYPE_SEND_RR)
         {
            EMA_UTILS_TRACE("MMIEMAIL_SelectNet the wifi is open, use wifi to send rr");
            MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_SENDREADREPORT, 0, 0);
         }
      }
#endif
   }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIEMAIL_HandleReselectNetQueryWinMsg(MMI_WIN_ID_T win_id,
                                                                                                          MMI_MESSAGE_ID_E msg_id,
                                                                                                          DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        MMK_CloseWin(win_id);
        MMIEMAIL_OpenNetSelectWin();
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (result);
}

/*****************************************************************************/
// 	Description: select sim card 
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_SelectNet(void)
{
   if(MMIAPISET_GetFlyMode())
   {
        MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
        return ;
   }
   
   if (MMIEMAIL_IsNetAvailable())
   {
        if (MMIEMAIL_IsCurrentConfigNetAvailable())
        {
            MMIEMAIL_ConnectNet();
        }
        else
        {
            MMI_WIN_ID_T query_win_id = EMAIL_RESELECT_NET_QUERY_WIN_ID;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_RESELECT_NET, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_HandleReselectNetQueryWinMsg);
        }
   }
   else
   {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SIM);
   }

    return;
}

/*****************************************************************************/
//  Description :insert attachment
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditInsertAttachment(EMAIL_WINDOW_ID_E pre_win_id, MMIEMAIL_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN if_replace)
{
//   int32 file_max_size               = 0;
//   uint32 attachment_file_size       = 0;
   MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL,0,0,0,PNULL,0,-1};
//   uint16 richedit_index             = 0;
//   uint8 *add_file_ptr               = PNULL;
//   uint16 add_file_len               = 0;
//   GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
   uint32					 total_size = 0;
   
   if (PNULL == edit_doc_ptr)
   {
      return;
   }
   
   GetEditLimitedSize(s_email_edit_p, &select_info.max_size, &total_size);
   select_info.max_size -= total_size;
   select_info.win_id = pre_win_id;
   select_info.ring_vol = -1;
   if(!MMIAPIFMM_OpenSelectAllFileWin(&select_info))
   {
      MMIPUB_OpenAlertWarningWin(TXT_EMAIL_ATTACH_FAILED);
   }
   return;
}

/*****************************************************************************/
//  Description :remove attachment
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditRemoveAttachment(void)
{
    uint16 richedit_index                 = 0;
    uint16 attach_index                   = 0;
    MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    EMAIL_ATTACH_T		 *attach_ptr = PNULL;
    EMAIL_ATTACH_T   	 *attach_header_ptr = PNULL;
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
    MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
#endif
    
    edit_doc_ptr = s_email_edit_p->edit_doc_ptr;
    GUIRICHTEXT_GetFocusItemIndex(EMAIL_EDIT_CTRL_ID, &richedit_index);
    
    richedit_index--;
    richedit_index--;//Added by wuhui, richedit_index > 1, subject's index = 1.
    if (s_email_edit_p->is_has_cc)
    {
        richedit_index--;
    }
    
    if (s_email_edit_p->is_has_bcc)
    {
        richedit_index--;
    }
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT    
    pb_edit_doc = MMIAPIEMAIL_GetSendVcardDocument();
    if(PNULL!=pb_edit_doc.attach_list_ptr && PNULL==edit_doc_ptr->attach_list_ptr  && 0 ==edit_doc_ptr->attach_num)
    {
        edit_doc_ptr = (MMIEMAIL_EDIT_DOCUMENT_T*)&pb_edit_doc;
    }
#endif
    if (richedit_index == 0 && edit_doc_ptr->attach_list_ptr)
    {
        attach_ptr = edit_doc_ptr->attach_list_ptr;
        edit_doc_ptr->attach_list_ptr = attach_ptr->next_ptr;
        EMA_UTILS_FREEIF(PNULL, attach_ptr->file_name_ptr);
        EMA_UTILS_FREE(PNULL, attach_ptr);
#if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
        if(MMIAPIEMAIL_GetIsPbSendEdit())
        {
            MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_temp_edit_doc ={0};
            MMIAPIEMAIL_SetSendVcardDocument(&pb_temp_edit_doc);
            MMIAPIEMAIL_SetIsPbSendEdit(FALSE);
        }
#endif
    }
    else
    {
        attach_header_ptr = edit_doc_ptr->attach_list_ptr;
        richedit_index--;
        while(attach_header_ptr)
        {
            if (attach_index == richedit_index)
            {
                attach_ptr = attach_header_ptr->next_ptr;
                attach_header_ptr->next_ptr = attach_ptr->next_ptr;
                EMA_UTILS_FREEIF(PNULL, attach_ptr->file_name_ptr);
                EMA_UTILS_FREE(PNULL, attach_ptr);
                break;
            }
            attach_index++;
            attach_header_ptr = attach_header_ptr->next_ptr;
        }
    }
    
    edit_doc_ptr->attach_num --;
    return;
}

/*****************************************************************************/
//  Description : edit init 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL int32 EditInit(MMIEMAIL_EDIT_REASON_TYPE_E etype, void *pdata)
{
   int32 result = EMAIL_ERROR;

   MMIEmail_ThreadEnable();

   s_email_edit_p->email_edit_enter_type = etype;
   s_email_edit_p->edit_doc_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(MMIEMAIL_EDIT_DOCUMENT_T));
   if (PNULL == s_email_edit_p->edit_doc_ptr)
   {
       EMA_UTILS_TRACE("out of memory");
       return EMAIL_NOMEMORY;
   }
   EMA_UTILS_MEMSET(s_email_edit_p->edit_doc_ptr, 0x00, sizeof(MMIEMAIL_EDIT_DOCUMENT_T));
   if (!g_email_sp_handle)
   {
      result = MMIEMAIL_SP_InitHandle(&g_email_sp_handle);
   }
   
   if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_NEW)
   {
      result = EditProcEnter((uint32)pdata);
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_DRAFT)
   {
       s_email_edit_p->edit_back_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(MMIEMAIL_EDIT_DOCUMENT_T));
       if (PNULL == s_email_edit_p->edit_back_ptr)
       {
           EMA_UTILS_TRACE("out of memory");
           EMA_UTILS_FREE(NULL, s_email_edit_p->edit_doc_ptr);
           return EMAIL_NOMEMORY;
       }
       EMA_UTILS_MEMSET(s_email_edit_p->edit_back_ptr, 0x00, sizeof(MMIEMAIL_EDIT_DOCUMENT_T));
       result = EditProcEdit((uint32)pdata);
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_REPLY 
      || s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_REPLYALL)
   {
      result = EditProcReply((uint32)pdata);
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_FORWARD
      || s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_FORWARD_WITHOUT_ATTACH)
   {
      result = EditProcForward((uint32)pdata);
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_VIEWSEND)
   {
      result = EditProcViwerSend((wchar*)pdata);
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_PB_SEND)
   {
      result = EMAIL_SUCCESS;
   }
   else if (s_email_edit_p->email_edit_enter_type == MMIEMAIL_EDIT_FROM_VIEW_EDIT)
   {
      result = EditProcViewEdit((uint32)pdata);
   }

   return result;
}

/*****************************************************************************/
//  Description :exit edit window
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void EditExit(void)
{
    EMAIL_ATTACH_T  *add_file = PNULL;
    BOOLEAN is_pb_send_edit =FALSE;
    is_pb_send_edit =MMIAPIEMAIL_GetIsPbSendEdit();
    if (PNULL != s_email_edit_p && !is_pb_send_edit)
    {
        
        if (PNULL != s_email_edit_p->edit_doc_ptr)
        {
            if(PNULL != s_email_edit_p->edit_doc_ptr->edit_to.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_to.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_cc.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_bcc.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_doc_ptr->edit_subject.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_subject.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_doc_ptr->edit_text.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr->edit_text.wstr_ptr);
            }
            
            add_file = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
            while(PNULL != add_file)
            {
                s_email_edit_p->edit_doc_ptr->attach_list_ptr = add_file->next_ptr;
                EMA_UTILS_FREEIF(PNULL, add_file->file_name_ptr);
                EMA_UTILS_FREE(PNULL, add_file);
                add_file = s_email_edit_p->edit_doc_ptr->attach_list_ptr;
            }
            EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_doc_ptr);
            
            s_email_edit_p->edit_doc_ptr = PNULL;
        }
        
        if (PNULL != s_email_edit_p->edit_back_ptr)
        {
            if(PNULL != s_email_edit_p->edit_back_ptr->edit_to.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_to.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_back_ptr->edit_cc.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_cc.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_back_ptr->edit_bcc.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_bcc.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_back_ptr->edit_subject.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_subject.wstr_ptr);
            }
            if(PNULL != s_email_edit_p->edit_back_ptr->edit_text.wstr_ptr)
            {
                EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr->edit_text.wstr_ptr);
            }
            
            add_file = s_email_edit_p->edit_back_ptr->attach_list_ptr;
            while(PNULL != add_file)
            {
                s_email_edit_p->edit_back_ptr->attach_list_ptr = add_file->next_ptr;
                EMA_UTILS_FREEIF(PNULL, add_file->file_name_ptr);
                EMA_UTILS_FREE(PNULL, add_file);
                add_file = s_email_edit_p->edit_back_ptr->attach_list_ptr;
            }
            EMA_UTILS_FREE(PNULL, s_email_edit_p->edit_back_ptr);
            
            s_email_edit_p->edit_back_ptr = PNULL;
        }
        
        
        MMIEMAIL_ClearEmailCache(&s_email_edit_p->data_cache);
        EMA_UTILS_FREEIF(PNULL, s_email_edit_p);
    }
    
    if (!MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
    {
        MMIEMAIL_ExitSetting();
    }
    // #if defined MMI_VCARD_SUPPORT && defined MMIEMAIL_SUPPORT
    //    if(MMIAPIEMAIL_GetIsPbSendEdit())
    //    {
    //        MMIEMAIL_PB_SEND_VCARD_DOCUMENT_T pb_edit_doc ={0};
    //        MMIAPIEMAIL_SetSendVcardDocument(&pb_edit_doc);
    //        MMIAPIEMAIL_SetIsPbSendEdit(FALSE);
    //        if(MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
    //        {
    //            MMK_CloseWin(EMAIL_EDIT_WIN_ID);
    //        }
    //        if(MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
    //        {
    //            MMK_CloseWin(EMAIL_MAIN_MENU_WIN_ID);
    //        }
    //        if(MMK_IsOpenWin(EMAIL_SETTING_WIN_ID))
    //        {
    //            MMK_CloseWin(EMAIL_SETTING_WIN_ID);
    //        }
    //    }
    // #endif
}


/*****************************************************************************/
//  Description :get edit richtext index
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetEditItemIndex( MMIEMAIL_EDIT_RICHTEXT_INDEX_E item_type)
{
    uint16 item_index = MMIEMAIL_RICHTEXT_ITEM_NUM_MAX;
    
    for (item_index = 0; item_index < MMIEMAIL_RICHTEXT_ITEM_NUM_MAX; item_index ++)
    {
        if (s_email_edit_richtext_type[item_index] == item_type)
        {
            break;
        }
    }
    return item_index;
}

/*****************************************************************************/
//  Description :get richtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_GetRichtextRect(GUI_RECT_T *rect_ptr)
{
   uint16 mainscreen_height          = 0;
   uint16 mainscreen_width           = 0;
#if !defined(MMI_PDA_SUPPORT) 
   uint16 page_bar_height            = 0;
#endif

   if (PNULL == rect_ptr)
   {
      return FALSE;
   }
   if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
   {
      return FALSE;
   }
   

   rect_ptr->left = 0;
#if defined MMI_ONLY_IDLE_DISP_STATUSBAR
      rect_ptr->top = MMI_TITLE_HEIGHT + 1;
#else
      rect_ptr->top = MMI_STATUSBAR_HEIGHT + MMI_TITLE_HEIGHT + 1;
#endif
   rect_ptr->right = mainscreen_width - 1;
#if defined(MMI_PDA_SUPPORT)    
   rect_ptr->bottom = mainscreen_height - 1;
#else
   rect_ptr->bottom = mainscreen_height - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT - page_bar_height -1;
#endif
   
   return TRUE;
}


/*****************************************************************************/
//  Description : Enter edit check
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMIEMAIL_ERROR_E EnterEditCheck(void)
{
   MMIEMAIL_ERROR_E result_val = MMIEMAIL_NO_ERROR;
   
   //     if(!MMIAPISMS_IsEMAILOrderReady())
   //     {
   //         result_val = MMIEMAIL_ERROR_NOT_INIT;
   //     }
   //     else 
   //	if(MMIAPIUDISK_UdiskIsRun()) //U盘使用中
   //    {
   //        result_val = MMIEMAIL_ERROR_UDISK_USING;
   //    }
   //#ifndef BROWSER_SUPPORT_NETFRONT
   //    else if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
   //    {
   //        result_val = MMIEMAIL_ERROR_CC_IN_CALL;
   //    }
   //#endif
   //     else if(MMIAPIMTV_IsMTVRunning()) //CMMB播放中
   //     {
   //         result_val = MMIEMAIL_ERROR_CMMB_RUNNING;
   //     }
   return result_val;
}

/*****************************************************************************/
// 	Description: create edit window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_REASON_TYPE_E etype, void *pdata)
{
   int result = EMAIL_ERROR;
   MMIEMAIL_ERROR_E edit_err = MMIEMAIL_NO_ERROR;
//   MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;

   EMA_UTILS_FREEIF(PNULL, s_email_edit_p);
   s_email_edit_p = EMA_UTILS_MALLOC(PNULL, sizeof(MMIEMAIL_EDIT_T));
   if(PNULL == s_email_edit_p){
      return EMAIL_NOMEMORY;
   }
   
   edit_err = EnterEditCheck();
   if (MMIEMAIL_NO_ERROR != edit_err)
   {
      //MMIEmail_EditAlertWin(edit_err);
      result = EMAIL_ERROR;
   }
   else
   {
// 	   if (MMK_IsOpenWin(EMAIL_EDIT_WIN_ID))
// 	   {
// 		   MMIPUB_OpenAlertWarningWin(TXT_MMS_EDIT_WIN_IN_USED);
// 		   return FALSE; 
	   
       result = EditInit(etype, pdata);
       if (EMAIL_SUCCESS == result)
       {
           MMK_CreateWin((uint32 *)EMAIL_EDIT_WIN_TAB, PNULL);
       }
   } 
   return result;
}
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
                                             MMIFMM_FILE_TYPE_E  file_type)
{
    int32 result =0;
    MMIEMAIL_EDIT_DOCUMENT_T *edit_doc_ptr = (MMIEMAIL_EDIT_DOCUMENT_T *)pb_edit_doc_ptr;
    MMIFILE_FILE_INFO_T   *file_info =PNULL;
    file_info = file_info_ptr;
    result = EditAddAttachment(edit_doc_ptr, file_info, MMIFMM_FILE_TYPE_NORMAL);
    return result;
}
#endif
/*Edit by script, ignore 9 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
