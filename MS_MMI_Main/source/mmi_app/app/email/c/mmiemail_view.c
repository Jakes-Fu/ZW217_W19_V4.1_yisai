/****************************************************************************
** File Name:      mmiemail_view.c                                         *
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
#include "mmisms_id.h"
#include "mmi_default.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "guistring.h"
#include "mmisms_export.h"
#include "gui_ucs2b_converter.h"

//#include "mmipic_image.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guires.h"
#include "mmidisplay_data.h"

#include "mmiemail_id.h"
#include "sysMemFile.h"
#include "mmiemail_view.h"
#include "email_text.h"
#include "email_image.h"
#include "mmiemail_def.h"
#include "mmiemail_menutable.h"
#include "mmiemail_utils.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_edit.h"

#include "mms_image.h"
#include "mmiemail_dlg.h"
#include "mmiemail_edit.h"
#include "mmiemail_box.h"

#include "mmicc_export.h"
#include "mmimms_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiemail_export.h"
#include "mmitheme_pos.h"

#if defined(MMI_PDA_SUPPORT)//@tao.xue
#include "mmisms_image.h"
#include "guibutton.h"
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIEMAIL_FILE_NAME_MAX			40

#define TEXT_RECT_TOP                   (MMI_TITLE_HEIGHT - 11)
#define TEXT_RECT_BOTTOM                (MMI_TITLE_HEIGHT - 1)
#define TEXT_RECT_LEFT                  ((int16)MMITHEME_GetClientRectWidth() - 80)
#define TEXT_RECT_RIGHT                 ((int16)MMITHEME_GetClientRectWidth() - 40)

#ifdef MAINLCD_SIZE_240X400
#define EMAIL_BUTTONFORM_HEIGHT 48
#else 
#define EMAIL_BUTTONFORM_HEIGHT 55
#endif

 /**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIEMAIL_VIEW_RICHTEXT_FROM_TITLE,          //0
    MMIEMAIL_VIEW_RICHTEXT_FROM,                //0
    MMIEMAIL_VIEW_RICHTEXT_TO_TITLE,            //0
    MMIEMAIL_VIEW_RICHTEXT_TO,                  //0
    MMIEMAIL_VIEW_RICHTEXT_CC_TITLE,            //0
    MMIEMAIL_VIEW_RICHTEXT_CC,                  //0
    MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE,            //0
    MMIEMAIL_VIEW_RICHTEXT_BCC,                  //0
    MMIEMAIL_VIEW_RICHTEXT_DATE,                  //0
    MMIEMAIL_VIEW_RICHTEXT_SUBJECT_TITLE,       //1
    MMIEMAIL_VIEW_RICHTEXT_SUBJECT,             //1
    MMIEMAIL_VIEW_RICHTEXT_ATTACH_TITLE,        //2
    MMIEMAIL_VIEW_RICHTEXT_ATTACH,              //2
    MMIEMAIL_VIEW_RICHTEXT_TEXT,                //3
    MMIEMAIL_VIEW_RICHTEXT_MAX
}MMIEMAIL_VIEW_RICHTEXT_INDEX_E; 

typedef struct 
{
    uint32				 menu_id;
    MMIEMAIL_VIEW_REASON_TYPE_E enter_type;
    EMAIL_MSGDATACACHE_T    data_cache;
    char               *focus_type_ptr;
    BOOLEAN            is_from_expand;
    BOOLEAN            is_subject_expand;
    BOOLEAN            is_attach_expand;
    BOOLEAN            is_content_extract;
    uint16				 focus_index;
    uint32             email_num;
    uint32             email_index;
    uint32             *email_id_ptr;
    uint32             send_email_id;
}MMIEMAIL_VIEW_T;

#if defined(MMI_PDA_SUPPORT)//@tao.xue
typedef struct
{
    MMI_CTRL_ID_T   ctrl_id;
    MMI_IMAGE_ID_T          bg_img_id;
    MMI_IMAGE_ID_T          pressed_bg_img_id;
    MMI_IMAGE_ID_T          fg_img_id;
    MMI_IMAGE_ID_T          pressed_fg_img_id;
}MMIEMAIL_BUTTON_T;
#endif
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
static MMIEMAIL_VIEW_T *s_email_view_p = PNULL;
#if defined(MMI_PDA_SUPPORT)//@tao.xue
LOCAL MMIEMAIL_BUTTON_T s_inbox_hot_buttons[] = 
{
    {MMIEMAIL_INBOX_BUTTON_REPLY,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_REPLY,IMAGE_EMAIL_REPLY},   
    {MMIEMAIL_INBOX_BUTTON_REPLY_ALL,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_REPLY_ALL,IMAGE_EMAIL_REPLY_ALL},
    {MMIEMAIL_INBOX_BUTTON_FORWARD,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_FORWARD,IMAGE_EMAIL_FORWARD},
    {MMIEMAIL_INBOX_BUTTON_DELETE,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_DELETE,IMAGE_EMAIL_DELETE},    
};

LOCAL MMIEMAIL_BUTTON_T s_outbox_hot_buttons[] = 
{
    {MMIEMAIL_OUTBOX_BUTTON_RESEND,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_RESEND,IMAGE_EMAIL_RESEND},
    {MMIEMAIL_OUTBOX_BUTTON_DELETE,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_DELETE,IMAGE_EMAIL_DELETE},    
};

LOCAL MMIEMAIL_BUTTON_T s_sentbox_hot_buttons[] = 
{
    {MMIEMAIL_SENTBOX_BUTTON_FORWARD,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_FORWARD,IMAGE_EMAIL_FORWARD},
    {MMIEMAIL_SENTBOX_BUTTON_RESEND,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_RESEND,IMAGE_EMAIL_RESEND},    
    {MMIEMAIL_SENTBOX_BUTTON_DELETE,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_EMAIL_DELETE,IMAGE_EMAIL_DELETE},    
};
#endif
/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
extern HEmlSPMgr		g_email_sp_handle;
extern uint32  g_email_is_send;
extern wchar   g_email_receive_str[EMA_MAX_ADDR_LEN];
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :handle add to pb option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddToPBOptWinMsg(
                                                    EMAIL_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                              );


/*****************************************************************************/
//  Description :handle location option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLocateOptWinMsg(
                                                   EMAIL_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                              );


/*****************************************************************************/
//  Description :handle save attachment window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewSaveAttachmentWinMsg(
                                                   EMAIL_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                                   );
                                                   
/*****************************************************************************/
//  Description :handle view attachment window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewAttachmentWinMsg(
                                                        MMI_WIN_ID_T win_id, 
                                                        MMI_MESSAGE_ID_E msg_id, 
                                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description :handle view detail window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewDetailWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                          );

/*****************************************************************************/
//  Description :handle view menu window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewMenuWinMsg(
                                                  EMAIL_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                              );


/*****************************************************************************/
//  Description :handle view window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewWinMsg(
                                              EMAIL_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description :add email all item number
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL uint32 ViewGetAllItemsNum(void);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void EmailDestroyButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIEMAIL_BUTTON_T *button_array,uint16 array_len);

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void EmailConstructButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIEMAIL_BUTTON_T *button_array,uint16 array_len);

#endif
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE(EMAIL_VIEW_ADD_TO_PB_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAddToPBOptWinMsg),
        WIN_ID(EMAIL_VIEW_ADDTOPB_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_ADDTOPB_OPT, EMAIL_VIEW_ADDTOPB_OPT_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(EMAIL_VIEW_LOCATE_NUMBER_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLocateOptWinMsg),
        WIN_ID(EMAIL_VIEW_LOCATE_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_LOCATE_NUMBER_OPT, EMAIL_VIEW_LOCATE_OPT_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_LOCATE_EMAIL_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLocateOptWinMsg),
        WIN_ID(EMAIL_VIEW_LOCATE_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_LOCATE_EMAIL_OPT, EMAIL_VIEW_LOCATE_OPT_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_LOCATE_URL_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLocateOptWinMsg),
        WIN_ID(EMAIL_VIEW_LOCATE_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_LOCATE_URL_OPT, EMAIL_VIEW_LOCATE_OPT_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(EMAIL_VIEW_SAVE_ATTACHMENT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewSaveAttachmentWinMsg),    
        WIN_ID(EMAIL_VIEW_SAVE_ATTACHMENT_WIN_ID),
        WIN_TITLE(STXT_SAVE),
#if defined(MMI_PDA_SUPPORT) 
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
      WIN_SOFTKEY(TXT_NULL, STXT_SAVE, TXT_NULL),
#else
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        CREATE_EDIT_TEXT_CTRL(MMIEMAIL_FILE_NAME_MAX, EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(EMAIL_VIEW_ATTACHMENT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewAttachmentWinMsg),    
        WIN_ID(EMAIL_VIEW_ATTACHMENT_WIN_ID),
        WIN_TITLE(TXT_EMAIL_VIEWATTACH),
        WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,  EMAIL_VIEW_ATTACHMENT_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_DETAIL_WIN_TABLE) = 
{
    WIN_TITLE(STXT_DETAIL),
        WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
        WIN_FUNC((uint32)HandleViewDetailWinMsg),
        WIN_ID(EMAIL_VIEW_DETAIL_WIN_ID),
        WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
        CREATE_RICHTEXT_CTRL(EMAIL_VIEW_DETAIL_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_INBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewMenuWinMsg),
        WIN_ID(EMAIL_VIEW_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_INBOX_OPT, EMAIL_VIEW_MENU_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_OUTBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewMenuWinMsg),
        WIN_ID(EMAIL_VIEW_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_OUTBOX_OPT, EMAIL_VIEW_MENU_CTRL_ID),
        END_WIN
};


WINDOW_TABLE(EMAIL_VIEW_SENTBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewMenuWinMsg),
        WIN_ID(EMAIL_VIEW_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_SNETBOX_OPT, EMAIL_VIEW_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(EMAIL_VIEW_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleViewWinMsg),    
        WIN_ID(EMAIL_VIEW_WIN_ID),
        WIN_TITLE(TXT_EMAIL_VIEW),
        WIN_SOFTKEY(STXT_OPTION, TXT_COMMON_OK, STXT_RETURN),
        //WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_RICHTEXT_CTRL(EMAIL_VIEW_CTRL_ID),
        END_WIN
};

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(EMAIL_VIEW_ATTACH_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleLocateOptWinMsg),
        WIN_ID(EMAIL_VIEW_LOCATE_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_POPMENU_CTRL(MENU_EMAIL_VIEW_ATTACH_OPT, EMAIL_VIEW_LOCATE_OPT_CTRL_ID),
        END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :handle location option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLocateOptWinMsg(
                                          EMAIL_WINDOW_ID_E win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_VIEW_LOCATE_OPT_CTRL_ID;
	uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
//    uint16  index                         = 0;
    int32   len                           = 0;
    wchar   *temp_ptr                        = PNULL;
    char    *locate_str_ptr                 = PNULL;
    int32   result                        = 0;
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
    MMIPB_MAIL_T pb_mail = {0};
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    MMIBROWSER_ENTRY_PARAM entry_param = {0};
//    const GUILIST_ITEM_T*  item_ptr = PNULL;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element      = {0};/*lint !e64*/
    EMASP_PRFITEM_VAL      val = {0}; 
    int32                  ret = 0;
    
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
        EMA_UTILS_TRACE("HandleLocateOptWinMsg msg_id = 0x%x", menu_id);
        GUIRICHTEXT_GetFocusElement(EMAIL_VIEW_CTRL_ID, &richtext_element);
        len = (richtext_element.data.len + 1) * sizeof(wchar);
        temp_ptr = EMA_UTILS_MALLOC(PNULL, len);
        EMA_UTILS_BREAKNULL(temp_ptr, ret, "out of memory");
        EMA_UTILS_MEMSET(temp_ptr, 0x00, len);
        EMA_UTILS_MEMCPY(temp_ptr, richtext_element.data.str_ptr, len - 2);
        //MMIEMAIL_VectorGetAt(s_email_view_p->pIRegexInfo, index, (void**)&temp_ptr);
        locate_str_ptr = EMA_UTILS_WSTRTOSTR(PNULL, temp_ptr);
        EMA_UTILS_BREAKNULL(locate_str_ptr, ret, "out of memory");
        switch(menu_id)
        {
        case EMAIL_VIEW_CALL_OPT_VOICE:
            {
		  if(EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_NET_PATH,&val))
		  {
			EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem, error"));
		  }
	         result = MMIAPICC_MakeCall((uint32)val.pData,
                    (uint8*)locate_str_ptr,
                    EMA_UTILS_STRLEN(locate_str_ptr),
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_NORMAL_CALL,
                    PNULL);
            }
            break;
#ifdef MMI_IP_CALL_SUPPORT
        case EMAIL_VIEW_CALL_OPT_IP:
            {
                if(EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMASP_PRFITEM_NET_PATH,&val))
	 	  {
			EMA_UTILS_LOG_ERROR(("MMIEMAIL_SP_Store_GetActCfgItem, error"));
		  }
		  result = MMIAPICC_MakeCall((uint32)val.pData,
                    (uint8*)locate_str_ptr,
                    EMA_UTILS_STRLEN(locate_str_ptr),
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_IP_CALL,
                    PNULL);
            }
            break;
#endif
        case EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_SMS:
            MMIAPISMS_WriteNewMessage(MN_DUAL_SYS_MAX, PNULL, (uint8*)locate_str_ptr, EMA_UTILS_STRLEN(locate_str_ptr));
            break;
        case EMAIL_VIEW_NUMBER_WRITEMESSAGE_OPT_MMS:
        case EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_MMS:
            {
                //send mms
                MMI_STRING_T string_to = {0};
                
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                string_to.wstr_len =  EMA_UTILS_WSTRLEN(temp_ptr);
                string_to.wstr_ptr = temp_ptr;
                MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL,&string_to);
            }
            break;
        case EMAIL_VIEW_ADDTOPB_OPT_NEW:
            {   
                if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
                {
                    MMIAPICOM_GenPartyNumber((uint8 *)locate_str_ptr,(uint16)EMA_UTILS_STRLEN((char *)locate_str_ptr), &party_num);
                    
                    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                    if (party_num.num_len > MMIPB_BCD_NUMBER_MAX_LEN)
                    {
                        pb_bcd.number_len = MMIPB_BCD_NUMBER_MAX_LEN;
                    }
                    else
                    {
                        pb_bcd.number_len = party_num.num_len;
                    }
                    EMA_UTILS_MEMCPY( 
                        pb_bcd.number,  
                        party_num.bcd_num, 
                        pb_bcd.number_len 
                        );
                    MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                    //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL
#if defined(MMI_PDA_SUPPORT)
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC
#endif
                )
                {
                    wchar   *email_address = PNULL;
                    
                    email_address = MMIEMAIL_GetEmailAddress(temp_ptr);

                    if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_MAIL_LEN)
                    {
                        pb_mail.wstr_len = MMIPB_MAX_MAIL_LEN;
                    }
                    else
                    {
                        pb_mail.wstr_len = EMA_UTILS_WSTRLEN(email_address);
                        if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
                        {
                            pb_mail.wstr_len -= 1;
                        }
                    }
                    
                    EMA_UTILS_MEMCPY(pb_mail.wstr, email_address, pb_mail.wstr_len*sizeof(wchar)); 
                    MMIAPIPB_AddContactWin(&pb_mail,PB_GROUP_ALL,MMIPB_ADD_TYPE_EMAIL); 
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
                {
                    if (EMA_UTILS_WSTRLEN(temp_ptr) > MMIPB_MAX_CUSTOM_TEXT_LEN)
                    {
                        pb_url.wstr_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
                    }
                    else
                    {
                        pb_url.wstr_len = EMA_UTILS_WSTRLEN(temp_ptr);
                    }
                    
                    EMA_UTILS_MEMCPY(pb_url.wstr, temp_ptr, pb_url.wstr_len*sizeof(wchar));
                    MMIAPIPB_AddContactWin(&pb_url,PB_GROUP_ALL,MMIPB_ADD_TYPE_URL);
                }
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case EMAIL_VIEW_ADDTOPB_OPT_EXIST:
            {   
                if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
                {
                    MMIAPICOM_GenPartyNumber((uint8 *)locate_str_ptr,(uint16)EMA_UTILS_STRLEN((char *)locate_str_ptr), &party_num);
                    
                    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                    if (party_num.num_len > MMIPB_BCD_NUMBER_MAX_LEN)
                    {
                        pb_bcd.number_len = MMIPB_BCD_NUMBER_MAX_LEN;
                    }
                    else
                    {
                        pb_bcd.number_len = party_num.num_len;
                    }
                    EMA_UTILS_MEMCPY( 
                        pb_bcd.number,  
                        party_num.bcd_num, 
                        pb_bcd.number_len 
                        );
                   // MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
                    MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL
#if defined(MMI_PDA_SUPPORT)
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC
                    || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC
#endif
                )
                {
                    wchar   *email_address = PNULL;

                    email_address = MMIEMAIL_GetEmailAddress(temp_ptr);

                    if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_MAIL_LEN)
                    {
                        pb_mail.wstr_len = MMIPB_MAX_MAIL_LEN;
                    }
                    else
                    {
                        pb_mail.wstr_len = EMA_UTILS_WSTRLEN(email_address);
                        if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
                        {
                            pb_mail.wstr_len -= 1;
                        }
                    }
                    
                    EMA_UTILS_MEMCPY(pb_mail.wstr, email_address, pb_mail.wstr_len*sizeof(wchar));
                   // MMIAPIPB_OpenEntryListForEx(&pb_mail, MMIPB_ADD_TYPE_EMAIL);
                    MMIAPIPB_AddToExistContact(&pb_mail, MMIPB_ADD_TYPE_EMAIL, PNULL);
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
                {
                    if (EMA_UTILS_WSTRLEN(temp_ptr) > MMIPB_MAX_CUSTOM_TEXT_LEN)
                    {
                        pb_url.wstr_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
                    }
                    else
                    {
                        pb_url.wstr_len = EMA_UTILS_WSTRLEN(temp_ptr);
                    }
                    
                    EMA_UTILS_MEMCPY(pb_url.wstr, temp_ptr, pb_url.wstr_len*sizeof(wchar));
                   // MMIAPIPB_OpenEntryListForEx(&pb_url, MMIPB_ADD_TYPE_URL);
                    MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_URL, PNULL);
                }
            }
            break;
#endif            
        case EMAIL_VIEW_EMAIL_WRITEMESSAGE_OPT_EMAIL:
            {
                MMI_STRING_T send_addr = {0};
                int32 temp_len =0;
                send_addr.wstr_ptr = (void*)temp_ptr;
                temp_len = EMA_UTILS_WSTRLEN(temp_ptr);
                send_addr.wstr_len = MIN(temp_len, MMIPB_MAX_MAIL_LEN);
                MMIAPIEMAIL_EditEmail(&send_addr);
            }
            break;
        case EMAIL_VIEW_LOCATE_URL_OPT_STARTBROWSER:
            {
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)locate_str_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);                       
            }
            break;	
        case EMAIL_VIEW_LOCATE_URL_OPT_ADDTOBOOKMARK:
            {
                entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)locate_str_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);
            }
            break;
#ifdef MMI_PDA_SUPPORT
        case EMAIL_VIEW_ATTACH_OPT_OPEN:
            {
                uint32 acttachindex = (uint32)s_email_view_p->focus_index; 
                uint32 item_num = 0;
                item_num = ViewGetAllItemsNum();
                item_num = item_num - (uint32)s_email_view_p->data_cache.attach_num - 1;
                acttachindex -= item_num;
                MMIAPIFMM_OpenFile(s_email_view_p->data_cache.attach_uri[acttachindex]);
            }
            break;
        case EMAIL_VIEW_ATTACH_OPT_SAVE:
            {
               uint32 acttachindex = (uint32)s_email_view_p->focus_index; 
               uint32 item_num = 0;
               item_num = ViewGetAllItemsNum();
               item_num = item_num - (uint32)s_email_view_p->data_cache.attach_num - 1;
               acttachindex -= item_num;
               MMK_CreateWin((uint32*)EMAIL_VIEW_SAVE_ATTACHMENT_WIN_TAB, (ADD_DATA)acttachindex);
            }
            break;
#endif
        default :
            break;
        }    
        EMA_UTILS_FREE(PNULL, temp_ptr);
        EMA_UTILS_FREE(PNULL, locate_str_ptr);
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

/*****************************************************************************/
//  Description : get pure email address
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_STRING_T GetPureEmailAddrss(wchar *address_ptr)
{
    MMI_STRING_T string_out = {0};
    wchar       *flag_begin_ptr = PNULL;
    wchar       *flag_end_ptr = PNULL;

   flag_begin_ptr = EMA_UTILS_WSTRCHR(address_ptr, L'<');
   if (PNULL == flag_begin_ptr)
   {
      string_out.wstr_ptr = address_ptr;
      string_out.wstr_len = EMA_UTILS_WSTRLEN(address_ptr); 
   }
   else
   {
      flag_end_ptr = EMA_UTILS_WSTRCHR(flag_begin_ptr, L'>');
      string_out.wstr_len = flag_end_ptr - flag_begin_ptr - 1;
      string_out.wstr_ptr = flag_begin_ptr + 1;
   }

   return string_out;
}

//*****************************************************************************/
//  Description : save file callback func
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void SaveFileCallBack(BOOLEAN is_success, FILEARRAY file_array)
{
   if (is_success)
   {
      if (0 < MMIAPIFILEARRAY_GetArraySize(file_array))
      {
         FILEARRAY_DATA_T          file_array_data = {0};
         wchar *file_path_ptr = PNULL;
         int32 file_len  = 0;
         wchar  *ext_ptr = PNULL;
         MMI_STRING_T  filename_string = {0};
         MMIFILE_ERROR_E file_error  = SFS_NO_ERROR;
         uint   len = 0;
         uint32 acttachindex = (uint32)s_email_view_p->focus_index; 
         uint32 item_num = 0;
         
         //read user selected dir
         MMIAPIFILEARRAY_Read( file_array, 0, &file_array_data);

         item_num = ViewGetAllItemsNum();
         item_num = item_num - (uint32)s_email_view_p->data_cache.attach_num - 1;
         acttachindex -= item_num;

         GUIEDIT_GetString(EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID, &filename_string);
         ext_ptr = EMA_UTILS_WSTRCHR(s_email_view_p->data_cache.attach_name[acttachindex], L'.');
         file_len = (file_array_data.name_len + filename_string.wstr_len + EMA_UTILS_WSTRLEN(ext_ptr) + 1 + 1) * sizeof(wchar);
         file_path_ptr = EMA_UTILS_MALLOC(PNULL, file_len);
         if (PNULL == file_path_ptr)
         {
             MMIAPIFILEARRAY_Destroy(&file_array);
             file_array = PNULL;  
             return;
         }
         EMA_UTILS_MEMSET(file_path_ptr, 0x00, file_len);
         EMA_UTILS_MEMCPY(file_path_ptr, file_array_data.filename, file_array_data.name_len * sizeof(wchar));
         EMA_UTILS_WSTRCAT(file_path_ptr, (wchar*)L"\\");
         EMA_UTILS_MEMCPY(file_path_ptr + file_array_data.name_len + 1, filename_string.wstr_ptr, filename_string.wstr_len * sizeof(wchar));
         len = EMA_UTILS_WSTRLEN(ext_ptr) * sizeof(wchar);
         EMA_UTILS_MEMCPY(file_path_ptr + file_array_data.name_len + filename_string.wstr_len + 1, ext_ptr, len);

         if (MMIAPIFMM_IsFileExist(file_path_ptr, EMA_UTILS_WSTRLEN(file_path_ptr)))
         {
            MMIPUB_OpenAlertWarningWin( TXT_FILE_EXISTED);
            MMIAPIFILEARRAY_Destroy(&file_array);
            file_array = PNULL;  
            return;
         }
         else
         {
            file_error = MMIAPIFMM_CopyFile(s_email_view_p->data_cache.attach_uri[acttachindex], file_path_ptr);
            EMA_UTILS_FREE(NULL, file_path_ptr);
            if (SFS_NO_ERROR == file_error)
            {
#ifdef MMI_GUI_STYLE_TYPICAL
               MMIPUB_OpenAlertSuccessWin( TXT_COMM_SAVE_SUCCESS);
#endif
            }
            else if (SFS_ERROR_NO_SPACE == file_error)
            {
               MMIPUB_OpenAlertWarningWin( TXT_EMAIL_NOENOUGHSPACE);
            }
            else
            {
               MMIPUB_OpenAlertWarningWin( TXT_MMI_SAVEFAILED);
            }
         }

         MMK_CloseWin(EMAIL_VIEW_SAVE_ATTACHMENT_WIN_ID);
      }
      else
      {
         EMA_UTILS_TRACE("OpenSelectTlsFileWinCallBack array size == 0");
      }
   }
   else
   {
      EMA_UTILS_TRACE("OpenSelectTlsFileWinCallBack failed");
   }
   
   MMIAPIFILEARRAY_Destroy(&file_array);
   file_array = PNULL;      
}

/*****************************************************************************/
//  Description :handle save attachment option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewSaveAttachmentWinMsg(
													 EMAIL_WINDOW_ID_E win_id, 
													 MMI_MESSAGE_ID_E msg_id, 
													 DPARAM param
													 )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint32 index = 0;
    MMI_STRING_T edit_string = {0};

    index = ((uint32)MMK_GetWinAddDataPtr(win_id))&0xFFFF;/*lint !e64*/
	
    EMA_UTILS_TRACE("HandleViewSaveAttachmentWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIEMAIL_CLEAR_CLIENT(win_id);
        break;
    case MSG_OPEN_WINDOW:
       {
          wchar  *temp_ptr = PNULL;
          GUIEDIT_SetTextMaxLen(EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID, MMIEMAIL_FILE_NAME_MAX, MMIEMAIL_FILE_NAME_MAX);
          temp_ptr = EMA_UTILS_WSTRCHR(s_email_view_p->data_cache.attach_name[index], L'.');

          GUIEDIT_SetString(EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID, 
             s_email_view_p->data_cache.attach_name[index],
             temp_ptr - s_email_view_p->data_cache.attach_name[index]);  
#ifdef MMI_PDA_SUPPORT
          GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
          GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, STXT_SAVE, FALSE);
          GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID);
#endif
          MMK_SetAtvCtrl(win_id, EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID);
       }
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:    
#endif    
    case MSG_CTL_OK:
    case MSG_APP_OK:
       {
         FILEARRAY ret_array = PNULL;
         MMIFMM_FILTER_T filter = {0};
         MMIFMM_OPENWIN_RET_E ret = FMM_OPENWIN_RET_NULL;

         GUIEDIT_GetString(EMAIL_VIEW_SAVE_ATTACHMENT_CTRL_ID, &edit_string);
         if (edit_string.wstr_len < 1)
         {
             MMIPUB_OpenAlertWarningWin( TXT_EMAIL_NAME_ERROR);
             break;
         }
         ret_array = MMIAPIFILEARRAY_Create();
         
         EMA_UTILS_MEMCPY(filter.filter_str, "*", sizeof("*"));
         
         ret = MMIAPIFMM_OpenFileWinByLayer(PNULL,
            0,
            &filter,
            FUNC_FIND_FOLDER,
            FALSE,
            PNULL,
            ret_array,
            SaveFileCallBack,
            FALSE);
         
         if (FMM_OPENWIN_SUCCESS != ret)
         {
            MMIAPIFILEARRAY_Destroy(&ret_array);
         }

         //MMK_CloseWin(win_id);
       }
        break;

#if defined(MMI_PDA_SUPPORT) 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK:
        if (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
        { 
            MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);  
            return MMI_RESULT_TRUE;
        }
        break;
#endif
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

//*****************************************************************************/
//  Description : add attachment items
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddAttachmentItems(MMI_CTRL_ID_T ctrl_id)
{
    int32 i = 0;
    
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    
	item_data.softkey_id[0] = STXT_SAVE;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;


    item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = IMAGE_MMS_ACCESSORY_FILE;

	 item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;	
	 item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
	
	for (i = 0; i < s_email_view_p->data_cache.attach_num; i++)
	{
		
		item_data.item_content[1].item_data.text_buffer.wstr_ptr = s_email_view_p->data_cache.attach_name[i];
		item_data.item_content[1].item_data.text_buffer.wstr_len= EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.attach_name[i]);
		
		item_data.item_content[2].item_data.text_buffer.wstr_ptr = s_email_view_p->data_cache.attach_size_ptr[i];
		item_data.item_content[2].item_data.text_buffer.wstr_len= EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.attach_size_ptr[i]);
		
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
}
	
/*****************************************************************************/
//  Description :handle view attachment window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewAttachmentWinMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param
                                               )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = EMAIL_VIEW_ATTACHMENT_CTRL_ID;
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            GUILIST_SetMaxItem(ctrl_id, EMA_MAX_ATTACH_NUM, FALSE);
            AddAttachmentItems(ctrl_id);
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
    case MSG_GET_FOCUS:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMK_CreateWin((uint32*)EMAIL_VIEW_SAVE_ATTACHMENT_WIN_TAB, PNULL);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:        /* ???????? */
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}

//*****************************************************************************/
//  Description : add detail items
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void AddDetailItems(void)
{
	GUIRICHTEXT_ITEM_T item_data = {0};    /*lint !e64*/ 
	MMI_STRING_T string = {0}; 
	MMI_CTRL_ID_T ctrl_id = EMAIL_VIEW_DETAIL_CTRL_ID; 
	uint16    index= 0;
	wchar	  *str_ptr = PNULL;

	item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;

	MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FROM, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    item_data.text_data.buf.str_ptr = s_email_view_p->data_cache.from_addrs;
	item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.from_addrs);
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

	MMI_GetLabelTextByLang(TXT_COMMON_TO, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
	
    if (s_email_view_p->data_cache.to_addrs_num > 0)
    {
        MMIEMAIL_GetEditRecipients(&s_email_view_p->data_cache.to_addrs[0][0], &str_ptr,
            PNULL, s_email_view_p->data_cache.to_addrs_num);
        item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(str_ptr);
        item_data.text_data.buf.str_ptr = str_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        EMA_UTILS_FREEIF(PNULL, str_ptr);
    }

	MMI_GetLabelTextByLang(TXT_EMAIL_CC, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

	if (s_email_view_p->data_cache.cc_addrs_num > 0)
	{
        MMIEMAIL_GetEditRecipients(&s_email_view_p->data_cache.cc_addrs[0][0], &str_ptr,
            PNULL, s_email_view_p->data_cache.cc_addrs_num);
        item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(str_ptr);
        item_data.text_data.buf.str_ptr = str_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        EMA_UTILS_FREEIF(PNULL, str_ptr);
	}

	MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);


	if (EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr) < 1)
	{
		MMI_GetLabelTextByLang(TXT_EMAIL_NOSUBJECT, &string);
		item_data.text_data.buf.len = string.wstr_len;
		item_data.text_data.buf.str_ptr = string.wstr_ptr;
	}
	else
	{
		item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr);
		item_data.text_data.buf.str_ptr = s_email_view_p->data_cache.subject_ptr;
	}
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);


	MMI_GetLabelTextByLang(TXT_COMMON_DATE, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

	str_ptr = MMIEMAIL_GetDateTime(PNULL, s_email_view_p->data_cache.uidate, EMA_DATE_YMD);
    item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(str_ptr);
    item_data.text_data.buf.str_ptr = str_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    EMA_UTILS_FREEIF(PNULL, str_ptr);

	MMI_GetLabelTextByLang(TXT_COMMON_TIME, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

    str_ptr = MMIEMAIL_GetDateTime(PNULL, s_email_view_p->data_cache.uidate, EMA_TIME);
	item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(str_ptr);
	item_data.text_data.buf.str_ptr = str_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
	EMA_UTILS_FREEIF(PNULL, str_ptr);


	MMI_GetLabelTextByLang(TXT_DETAIL_SIZE, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

	/*get MMS size*/
	
	MMIEMAIL_SetSize2Text(PNULL, s_email_view_p->data_cache.email_size,&str_ptr);
	item_data.text_data.buf.len = EMA_UTILS_WSTRLEN(str_ptr);
	item_data.text_data.buf.str_ptr = str_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
	EMA_UTILS_FREEIF(PNULL, str_ptr);

	MMI_GetLabelTextByLang(TXT_DETAIL_PRIORITY, &string);
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);


	if (EMA_PRIORITY_HIGH == s_email_view_p->data_cache.uipriority)
	{
		MMI_GetLabelTextByLang(TXT_COMMON_HIGH, &string);
	} 
	else if(EMA_PRIORITY_LOW == s_email_view_p->data_cache.uipriority)
	{
		MMI_GetLabelTextByLang(TXT_COMM_LOW, &string);
	}
	else
	{
		MMI_GetLabelTextByLang(TXT_NORMAL, &string);
	}
	item_data.text_data.buf.len = string.wstr_len;
    item_data.text_data.buf.str_ptr = string.wstr_ptr;
	GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
}


/*****************************************************************************/
//  Description :handle view detail window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewDetailWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    MMI_CTRL_ID_T ctrl_id = EMAIL_VIEW_DETAIL_CTRL_ID;
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIRICHTEXT_SetScroll(ctrl_id, TRUE);
        AddDetailItems();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description :remove email body
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// LOCAL int32 ViewRemoveBody(void)
// {
//    EMAIL_MSGDATACACHE_T *pDataCache = &s_email_view_p->data_cache;
//    int32 nResult = EMAIL_ERROR;
//    EMAIL_ATTACH_T  *add_file = PNULL;
//    
//    do 
//    {
//       nResult = EmailASP_Store_RemoveParts(EMA_GETSTORE_HANDLE(g_email_sp_handle), (uint32)pDataCache->msg_id,
//          EMA_SP_ACTIVE_ACCOUNT, FALSE);
// 
//       if (EMA_READ_WITH_ATTACH == pDataCache->mail_status)
//       {
//          pDataCache->mail_status = EMA_BASIC_READ;
//          nResult = EmailASP_Store_UpdateMsg(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, pDataCache);
//          EMA_UTILS_BREAKIF(nResult, "EmailAUIC_InBoxProc_Enter, EmailASP_Store_UpdateMsg failed.");
//       }
// 
//       add_file = s_email_view_p->attach_list;
//       while(PNULL != add_file)
//       {
//          s_email_view_p->attach_list = add_file->next;
//          EMA_UTILS_FREE(PNULL, add_file);
//          add_file = s_email_view_p->attach_list;
//       }
// 
//       if(PNULL != s_email_view_p->view_text.wstr_ptr &&  0 != s_email_view_p->view_text.wstr_len)
//       {
//          EMA_UTILS_FREE(PNULL, s_email_view_p->view_text.wstr_ptr);
//          s_email_view_p->view_text.wstr_ptr = PNULL;
//          s_email_view_p->view_text.wstr_len = 0;
// 	   }
// 
//    } while (0);
//    
//    
//    return EMAIL_SUCCESS;
// }

/*****************************************************************************/
//  Description :handle view menu window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleViewMenuWinMsg(
                                                   EMAIL_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                              )
{
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_VIEW_MENU_CTRL_ID;
	uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
    GUIMENU_POP_SEL_INFO_T pop_info       = {0};
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
    uint    len         = 0;
    BOOLEAN is_content_extract = FALSE;
    wchar   *email_address = PNULL;
    
    EMA_UTILS_TRACE("HandleViewMenuWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
       GUIMENU_GetId( ctrl_id, &group_id, &menu_id);
       if (MMIEMAIL_VIEW_FROM_INBOX == s_email_view_p->enter_type)
       {
          if (EMA_BASIC_UNREAD_HEAD_ONLY == s_email_view_p->data_cache.mail_status || EMA_BASIC_READ_HEAD_ONLY == s_email_view_p->data_cache.mail_status)
          {
             GUIMENU_SetItemGrayed(ctrl_id, group_id,EMAIL_VIEW_INBOX_OPT_CONTENTEXTRACT,TRUE);
          }
          else
          {
             GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_VIEW_INBOX_OPT_RECVFULLMESSAGE,FALSE);
          }

          if (s_email_view_p->data_cache.attach_num < 1)
          {
             GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_VIEW_INBOX_OPT_FORWARD_ATTACH,FALSE);
          }
          else
          {
             GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_VIEW_INBOX_OPT_FORWARD,FALSE);
          }

          if (PNULL != s_email_view_p->focus_type_ptr 
              && MMIEMAIL_VIEW_RICHTEXT_FROM_TITLE != s_email_view_p->focus_type_ptr[s_email_view_p->focus_index])
          {
              GUIMENU_SetItemGrayed(ctrl_id, group_id,EMAIL_VIEW_INBOX_OPT_SAVETO,TRUE);
          }
       }
       else if (MMIEMAIL_VIEW_FROM_SENTBOX == s_email_view_p->enter_type)
       {
          if (s_email_view_p->data_cache.attach_num < 1)
          {
             GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_VIEW_SENTBOX_OPT_FORWARD_ATTACH,FALSE);
          }
          else
          {
             GUIMENU_SetItemVisible(ctrl_id, group_id,EMAIL_VIEW_SENTBOX_OPT_FORWARD,FALSE);
          }
       }

       pop_info.is_static = TRUE;    
       pop_info.ctrl_id = ctrl_id;
       pop_info.group_id = MENU_EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT;
       if (s_email_view_p->is_content_extract)
       {
          pop_info.menu_id = EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON;
         //pop_info.node_id = 0;
       }
       else
       {
          pop_info.menu_id = EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF;
          //pop_info.node_id = 1;
       }
	   GUIMENU_SetPopItemStatus(TRUE, &pop_info);
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
       GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
       s_email_view_p->menu_id = menu_id;
       EMA_UTILS_TRACE("MMIEmail_HandleEditEditMenuWinMsg msg_id = 0x%x", menu_id);
       switch(menu_id)
       {
       case EMAIL_VIEW_INBOX_OPT_DETAIL:
       case EMAIL_VIEW_OUTBOX_OPT_DETAIL:
       case EMAIL_VIEW_SENTBOX_OPT_DETAIL:
            MMK_CreateWin((uint32*)EMAIL_VIEW_DETAIL_WIN_TABLE, PNULL);
            MMK_CloseWin(win_id);
            return err_code;
       case EMAIL_VIEW_INBOX_OPT_RECVFULLMESSAGE:
          g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
          EMA_UTILS_MEMSET(g_email_receive_str, 0x00, EMA_MAX_ADDR_LEN * sizeof(wchar));
          len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr) * sizeof(wchar);
          EMA_UTILS_MEMCPY(g_email_receive_str, s_email_view_p->data_cache.subject_ptr, len);
          MMIEMAIL_SelectNet();
          break;
       case EMAIL_VIEW_INBOX_OPT_REPLY:
          MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLY, (void*)s_email_view_p->data_cache.email_id);
          break;
       case EMAIL_VIEW_INBOX_OPT_REPLYTOALL:
          MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLYALL, (void*)s_email_view_p->data_cache.email_id);
          break;
       case EMAIL_VIEW_INBOX_OPT_FORWARD:
       case EMAIL_VIEW_SENTBOX_OPT_FORWARD:
         MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD, (void*)s_email_view_p->data_cache.email_id);
         break;
       case EMAIL_VIEW_INBOX_OPT_DELETE:
       case EMAIL_VIEW_OUTBOX_OPT_DELETE:
       case EMAIL_VIEW_SENTBOX_OPT_DELETE:
          MMIEMAIL_OpenQueryWinByID(TXT_DELETE_QUERY, EMAIL_VIEW_DELETE_QUERY_WIN_ID);
          break;
       case EMAIL_VIEW_OUTBOX_OPT_SEND:
           s_email_view_p->send_email_id = s_email_view_p->data_cache.email_id;
           MMIEMAIL_GetAllRecipients(PNULL, &s_email_view_p->data_cache);
           g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
           MMIEMAIL_SelectNet();
           break;
       case EMAIL_VIEW_SENTBOX_OPT_RESEND:
           {
               EMAIL_MSGDATACACHE_T    *eml_cache_ptr = {0};
               int32                   ret = 0;

               EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, begin to resend email");
               eml_cache_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
               EMA_UTILS_BREAKNULL(eml_cache_ptr, ret, "EMAIL_VIEW_SENTBOX_OPT_RESEND, malloc EMAIL_MSGDATACACHE_T failed");
               EMA_UTILS_MEMSET(eml_cache_ptr, 0x00, sizeof(EMAIL_MSGDATACACHE_T));
               ret = MMIEMAIL_CopyEmailDataCache(eml_cache_ptr, &s_email_view_p->data_cache, PNULL);
               if(EMAIL_SUCCESS != ret)
               {
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, copy email data failed");
                   break;
               }
               eml_cache_ptr->email_id = 0;
               EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, begin to call MMIEMAIL_EditProcSendEmail");
               if(TRUE == MMIEMAIL_EditProcSendEmail((uint32)eml_cache_ptr))
               {
                   s_email_view_p->send_email_id = eml_cache_ptr->email_id;
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, the send msg id is %d", s_email_view_p->send_email_id);
                   MMIEMAIL_GetAllRecipients(PNULL, eml_cache_ptr);
                   g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                   MMIEMAIL_SelectNet();
               }
               else
               {
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, call MMIEMAIL_EditProcSendEmail failed");
               }
               MMIEMAIL_ReleaseEmailDataCache(PNULL, eml_cache_ptr);
           }
          break;	
       case EMAIL_VIEW_OUTBOX_OPT_EDIT:
          MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_VIEW_EDIT, (void *)s_email_view_p->data_cache.email_id);
          break;
       case EMAIL_VIEW_ADDTOPB_OPT_NEW:
          email_address = MMIEMAIL_GetEmailAddress(s_email_view_p->data_cache.from_addrs);
          if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_CUSTOM_TEXT_LEN)
          {
             pb_url.wstr_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
          }
          else
          {
             if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
             {
                pb_url.wstr_len = EMA_UTILS_WSTRLEN(email_address) - 1;
             }
             else
             {
                pb_url.wstr_len = EMA_UTILS_WSTRLEN(email_address);
             }
          }
          EMA_UTILS_MEMCPY(pb_url.wstr, email_address, pb_url.wstr_len*sizeof(wchar));
          MMIAPIPB_AddContactWin(&pb_url,PB_GROUP_ALL,MMIPB_ADD_TYPE_EMAIL);
          break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
       case EMAIL_VIEW_ADDTOPB_OPT_EXIST:
          email_address = MMIEMAIL_GetEmailAddress(s_email_view_p->data_cache.from_addrs);
          if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_CUSTOM_TEXT_LEN)
          {
             pb_url.wstr_len = MMIPB_MAX_CUSTOM_TEXT_LEN;
          }
          else
          {
             if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
             {
                 pb_url.wstr_len = EMA_UTILS_WSTRLEN(email_address) - 1;
             }
             else
             {
                 pb_url.wstr_len = EMA_UTILS_WSTRLEN(email_address);
             }
          }
          EMA_UTILS_MEMCPY(pb_url.wstr, email_address, pb_url.wstr_len*sizeof(wchar));
          //MMIAPIPB_OpenEntryListForEx(&pb_url, MMIPB_ADD_TYPE_EMAIL);
          MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_EMAIL, PNULL);
          break;
#endif
       case EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON:
           is_content_extract = s_email_view_p->is_content_extract;
           if (TRUE != s_email_view_p->is_content_extract)
           {
               s_email_view_p->is_content_extract = TRUE;
           }
          break;
       case EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF:
          is_content_extract = s_email_view_p->is_content_extract;
          if (FALSE != s_email_view_p->is_content_extract)
          {
              s_email_view_p->is_content_extract = FALSE;
          }
          break;
       case EMAIL_VIEW_FORWARD_OPT_WITH_ATTACHMENT:   
          MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD, (void*)s_email_view_p->data_cache.email_id);
          break;
       case EMAIL_VIEW_FORWARD_OPT_WITHOUT_ATTACHMENT:
          MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD_WITHOUT_ATTACH, (void*)s_email_view_p->data_cache.email_id);          
          break;
       default :
          break;
       }    
       MMK_CloseWin(win_id);
       if (is_content_extract != s_email_view_p->is_content_extract
           && (EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON == s_email_view_p->menu_id 
           || EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF == s_email_view_p->menu_id))
       {
           MMK_SendMsg(EMAIL_VIEW_WIN_ID, MSG_FULL_PAINT, 0);
       }
       break;
    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(ctrl_id, &group_id, &s_email_view_p->menu_id);
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
		  s_email_view_p->menu_id = EMAIL_MENU_MAX_ITEM_ID,
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
//  Description :add email from item
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddFromItem(void)
{
   GUIRICHTEXT_ITEM_T item              = {0};/*lint !e64*/
   uint16 index                         = 0;
   wchar *set_str                       = PNULL;
   uint16 set_len                       = 0;
   MMI_STRING_T  string = {0};
   wchar   *temp_ptr = PNULL;
   int     i = 0;
   uint    len = 0;
   int32   ret = 0;
   
   do 
   {
       if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
       {
           MMI_GetLabelTextByLang(TXT_COMMON_DETAIL_FROM, &string);
           temp_ptr = s_email_view_p->data_cache.from_addrs;
       }
       else
       {
           if (s_email_view_p->data_cache.to_addrs_num > 0)
           {
               MMI_GetLabelTextByLang(TXT_COMMON_TO, &string);
               temp_ptr = s_email_view_p->data_cache.to_addrs[0];
           }
           else if (s_email_view_p->data_cache.cc_addrs_num > 0)
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_CC, &string);
               temp_ptr = s_email_view_p->data_cache.cc_addrs[0];         
           }
           else if (s_email_view_p->data_cache.bcc_addrs_num > 0)
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_BCC, &string);
               temp_ptr = s_email_view_p->data_cache.bcc_addrs[0];         
           }
       }
       
       if (!s_email_view_p->is_from_expand)
       {
           set_len = string.wstr_len + EMA_UTILS_WSTRLEN(temp_ptr);
           set_str = EMA_UTILS_MALLOC(PNULL, (set_len + 1) * sizeof(wchar));
           EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
           EMA_UTILS_MEMSET(set_str, 0x00, (set_len + 1) * sizeof(wchar));
           if (PNULL != string.wstr_ptr)
           {
               EMA_UTILS_MEMCPY(set_str, string.wstr_ptr, string.wstr_len * sizeof(wchar));
           }
           len = EMA_UTILS_WSTRLEN(temp_ptr) * sizeof(wchar);
           if (PNULL != temp_ptr)
           {
               EMA_UTILS_MEMCPY(set_str + string.wstr_len, temp_ptr, len);
           }
           //set_len = MMIEmail_ViewTruncateTheString(set_str);
       }
       else
       {
           set_len = string.wstr_len;
           set_str = EMA_UTILS_WSTRDUP(PNULL, string.wstr_ptr); 
           EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
       }
       
       item.text_type = GUIRICHTEXT_TEXT_BUF;
       item.text_data.buf.str_ptr = set_str;
       item.text_data.buf.len = set_len;
       item.text_frame_type = GUIRICHTEXT_FRAME_NONE;
       
       item.text_set_font = TRUE;
       item.setting_font = SONG_FONT_24;
       
       item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
       item.align_type = GUIRICHTEXT_ALIGN_LEFT;
       item.text_set_bg = TRUE;
       item.setting_bg_color = MMI_DARK_GRAY_COLOR;
       
       GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
       if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
       {
           s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_FROM_TITLE;
       }
       else
       {
           if (s_email_view_p->data_cache.to_addrs_num > 0)
           {
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_TO_TITLE;
           }
           else if (s_email_view_p->data_cache.cc_addrs_num > 0)
           {
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_CC_TITLE;
           }
           else if (s_email_view_p->data_cache.bcc_addrs_num > 0)
           {
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE;
           }      
       }
       
       EMA_UTILS_FREE(PNULL, set_str);
       
       if (s_email_view_p->is_from_expand)
       {
           item.text_set_bg = FALSE;
           if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
           {
               item.text_data.buf.str_ptr = s_email_view_p->data_cache.from_addrs;
               item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.from_addrs);
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_FROM;
               
               if (s_email_view_p->data_cache.to_addrs_num > 0)
               {
                   MMI_GetLabelTextByLang(TXT_COMMON_TO, &string);
                   item.text_data.buf.str_ptr = string.wstr_ptr;
                   item.text_data.buf.len = string.wstr_len;
                   
                   GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
                   s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_TO_TITLE;
               }
           }
           
           for (i = 0; i< s_email_view_p->data_cache.to_addrs_num; i++)
           {
               item.text_data.buf.str_ptr = s_email_view_p->data_cache.to_addrs[i];
               item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.to_addrs[i]);
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_TO;
           }
           
           
           if ( s_email_view_p->data_cache.cc_addrs_num > 0 && s_email_view_p->focus_type_ptr[0] != MMIEMAIL_VIEW_RICHTEXT_CC_TITLE)
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_CC, &string);
               item.text_data.buf.str_ptr = string.wstr_ptr;
               item.text_data.buf.len = string.wstr_len;
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_CC_TITLE;
           }
           
           for (i = 0; i< s_email_view_p->data_cache.cc_addrs_num; i++)
           {
               item.text_data.buf.str_ptr = s_email_view_p->data_cache.cc_addrs[i];
               item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.cc_addrs[i]);
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_CC;
           }
           
           
           if ( s_email_view_p->data_cache.bcc_addrs_num > 0 && s_email_view_p->focus_type_ptr[0] != MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE)
           {
               MMI_GetLabelTextByLang(TXT_EMAIL_BCC, &string);
               item.text_data.buf.str_ptr = string.wstr_ptr;
               item.text_data.buf.len = string.wstr_len;
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE;
           }
           
           for (i = 0; i< s_email_view_p->data_cache.bcc_addrs_num; i++)
           {
               item.text_data.buf.str_ptr = s_email_view_p->data_cache.bcc_addrs[i];
               item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.bcc_addrs[i]);
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_BCC;
           }
       }
   } while (0);
}

/*****************************************************************************/
//  Description :add date item
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddDateItem(void)
{
   GUIRICHTEXT_ITEM_T item              = {0};/*lint !e64*/
   uint16 index                         = 0;
   wchar *set_str                       = PNULL;
   uint16 set_len                       = 0;
   
   set_str = MMIEMAIL_GetDateTime(PNULL, s_email_view_p->data_cache.uidate, EMA_DATE_YMD);
   set_len = EMA_UTILS_WSTRLEN(set_str);

   item.text_type = GUIRICHTEXT_TEXT_BUF;
   item.text_data.buf.str_ptr = set_str;
   item.text_data.buf.len = set_len;
   item.text_frame_type = GUIRICHTEXT_FRAME_NONE;
   
   item.text_set_font = TRUE;
   item.setting_font = SONG_FONT_24;
   
   item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
   item.align_type = GUIRICHTEXT_ALIGN_LEFT;
   item.text_set_bg = TRUE;
   item.setting_bg_color = MMI_DARK_GRAY_COLOR;
   
   GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
   s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_DATE;
   EMA_UTILS_FREEIF(PNULL, set_str);
}

/*****************************************************************************/
//  Description :add email subject item
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddSubjectItem(void)
{
   GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
   uint16 index                          = 0;
   wchar *set_str                       = PNULL;
   uint16 set_len                        = 0;
   MMI_STRING_T  string = {0};
   uint  len = 0; 
   int32 ret = 0;

   do 
   {
       MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT, &string);
       if (!s_email_view_p->is_subject_expand)
       {
           set_len = string.wstr_len + EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr);
           set_str = EMA_UTILS_MALLOC(PNULL, (set_len + 1) * sizeof(wchar));
           EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
           EMA_UTILS_MEMSET(set_str, 0x00, (set_len + 1) * sizeof(wchar));
           EMA_UTILS_MEMCPY(set_str, string.wstr_ptr, string.wstr_len * sizeof(wchar));
           len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr) * sizeof(wchar);
           EMA_UTILS_MEMCPY(set_str + string.wstr_len, s_email_view_p->data_cache.subject_ptr, len);
       }
       else
       {
           set_len = string.wstr_len;
           set_str = EMA_UTILS_WSTRDUP(PNULL, string.wstr_ptr); 
           EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
       }
       
       item.text_type = GUIRICHTEXT_TEXT_BUF;
       item.text_data.buf.str_ptr = set_str;
       item.text_data.buf.len = set_len;
       item.text_frame_type = GUIRICHTEXT_FRAME_NONE;
       
       item.text_set_font = TRUE;
       item.setting_font = SONG_FONT_24;
       
       item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
       item.align_type = GUIRICHTEXT_ALIGN_LEFT;
       item.text_set_bg = TRUE;
       item.setting_bg_color = MMI_DARK_GRAY_COLOR;
       
       GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
       s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_SUBJECT_TITLE;
       
       EMA_UTILS_FREE(PNULL, set_str);
       
       if (s_email_view_p->is_subject_expand)
       {
           item.text_set_bg = FALSE;
           
           item.text_data.buf.str_ptr = s_email_view_p->data_cache.subject_ptr;
           item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr);
           
           GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
           s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_SUBJECT;
       }
   } while (0);
}

/*****************************************************************************/
//  Description :add email file item
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddFileItem(void)
{
   GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
   uint16 index                          = 0;
   wchar *set_str                       = PNULL;
   uint16 set_len                        = 0;
   MMI_STRING_T  string = {0};
   wchar    *temp_ptr = PNULL;
   char    acattach_num[32] = {0};
   int     i = 0;
   uint    len = 0;
   int32   ret = 0;

   do 
   {
       MMI_GetLabelTextByLang(TXT_EMAIL_ATTACH_NUM, &string);
       
       EMA_UTILS_SPRINTF(acattach_num, "%d", s_email_view_p->data_cache.attach_num);
       temp_ptr = EMA_UTILS_STRTOWSTR(PNULL, acattach_num);
       EMA_UTILS_BREAKNULL(temp_ptr, ret, "out of memory");
       
       set_len = EMA_UTILS_WSTRLEN(temp_ptr) + string.wstr_len;
       set_str = EMA_UTILS_MALLOC(PNULL, set_len * sizeof(wchar));
       EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
       len = EMA_UTILS_WSTRLEN(temp_ptr) * sizeof(wchar);
       EMA_UTILS_MEMCPY(set_str, temp_ptr, len);
       EMA_UTILS_MEMCPY(set_str + EMA_UTILS_WSTRLEN(temp_ptr), string.wstr_ptr, string.wstr_len * sizeof(wchar));
       
       item.img_type = GUIRICHTEXT_IMAGE_RES;
       item.img_data.res.id = IMAGE_MMS_ACCESSORY_FILE;
       
       item.text_type = GUIRICHTEXT_TEXT_BUF;
       item.text_data.buf.str_ptr = set_str;
       item.text_data.buf.len = set_len;
       item.text_frame_type = GUIRICHTEXT_FRAME_NONE;
       
       item.text_set_font = TRUE;
       item.setting_font = SONG_FONT_24;
       
       item.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
       item.text_set_bg = TRUE;
       item.setting_bg_color = MMI_DARK_GRAY_COLOR;
       
       GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
       s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_ATTACH_TITLE;
       
       EMA_UTILS_FREE(PNULL, set_str);
       EMA_UTILS_FREE(PNULL, temp_ptr);
       
       if (s_email_view_p->is_attach_expand)
       {
           item.text_set_bg = FALSE;
           
           for (i=0; i <s_email_view_p->data_cache.attach_num; i++)
           {
               set_len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.attach_name[i]) 
                   + EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.attach_size_ptr[i]) + 2;
               set_str = EMA_UTILS_MALLOC(PNULL, (set_len + 1) * sizeof(wchar));
               EMA_UTILS_BREAKNULL(set_str, ret, "out of memory");
               EMA_UTILS_MEMSET(set_str, 0x00, (set_len + 1) * sizeof(wchar));
               EMA_UTILS_WSTRCAT(set_str, (wchar*)L"(");
               EMA_UTILS_WSTRCAT(set_str, s_email_view_p->data_cache.attach_size_ptr[i]);
               EMA_UTILS_WSTRCAT(set_str, (wchar*)L")");
               EMA_UTILS_WSTRCAT(set_str, s_email_view_p->data_cache.attach_name[i]);
               
               item.text_data.buf.str_ptr = set_str;
               item.text_data.buf.len = set_len;
               
               GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
               s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_ATTACH;
               
               EMA_UTILS_FREE(PNULL, set_str);
           }
       }
   } while (0);
}


/*****************************************************************************/
//  Description :add email text item
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddTextItem(void)
{
   GUIRICHTEXT_ITEM_T item               = {0};/*lint !e64*/
   uint16 index                          = 0;
//   wchar *set_str                       = PNULL;
//   uint16 set_len                        = 0;
   MMI_STRING_T  string = {0};
   
   if (s_email_view_p->data_cache.mail_status == EMA_BASIC_UNREAD_HEAD_ONLY || s_email_view_p->data_cache.mail_status == EMA_BASIC_READ_HEAD_ONLY)
   {
      MMI_GetLabelTextByLang(TXT_EMAIL_RECV_ONLY_HEADER, &string);
      item.text_data.buf.str_ptr = string.wstr_ptr;
      item.text_data.buf.len = string.wstr_len;
      item.tag_type = GUIRICHTEXT_TAG_TXT_ALL;
   }
   else
   {
       EMA_UTILS_TRACE("s_email_view_p->data_cache.data_buff_ptr len is  %d", MMIAPICOM_Wstrlen((const wchar*)s_email_view_p->data_cache.data_buff_ptr));
       EMA_UTILS_TRACE("s_email_view_p->data_cache.data_buff_ptr is  %d", *s_email_view_p->data_cache.data_buff_ptr);
       EMA_UTILS_TRACE("s_email_view_p->data_cache.data_buff_ptr is  %d", *(s_email_view_p->data_cache.data_buff_ptr+1));
       EMA_UTILS_TRACE("s_email_view_p->data_cache.data_buff_ptr is  %d", *(s_email_view_p->data_cache.data_buff_ptr+2));
      item.text_data.buf.str_ptr = s_email_view_p->data_cache.data_buff_ptr;
      item.text_data.buf.len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.data_buff_ptr);
      if (s_email_view_p->is_content_extract || item.text_data.buf.len < 1)
      {
         item.tag_type = GUIRICHTEXT_TAG_PARSE;
      }
      else
      {
         item.tag_type = GUIRICHTEXT_TAG_NORMAL;
      }
   }
 
   
   item.text_type = GUIRICHTEXT_TEXT_BUF;
   //item.text_frame_type = GUIRICHTEXT_FRAME_NONE;
   
   item.text_set_font = TRUE;
   item.setting_font = SONG_FONT_24;
   
//    item.align_type = GUIRICHTEXT_ALIGN_LEFT;
//    item.tag_type = GUIRICHTEXT_TAG_NORMAL;
//    item.frame_type = GUIRICHTEXT_FRAME_NORMAL;
   
   GUIRICHTEXT_AddItem(EMAIL_VIEW_CTRL_ID, &item, &index);
   s_email_view_p->focus_type_ptr[index] = MMIEMAIL_VIEW_RICHTEXT_TEXT;  
}

/*****************************************************************************/
//  Description :add email all item number
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL uint32 ViewGetAllItemsNum(void)
{
    uint32 item_num = 0;
    
    item_num++;
    if (s_email_view_p->is_from_expand)
    {
        if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
        {
            item_num++;
        }
        
        EMA_UTILS_TRACE("ViewGetAllItemsNum, the to, cc, bcc num is %d, %d, %d", s_email_view_p->data_cache.to_addrs_num,
            s_email_view_p->data_cache.cc_addrs_num, s_email_view_p->data_cache.bcc_addrs_num);
        if (s_email_view_p->data_cache.to_addrs_num > 0)
        {
            item_num += (uint32)s_email_view_p->data_cache.to_addrs_num + 1;
        }
        
        if (s_email_view_p->data_cache.cc_addrs_num > 0)
        {
            item_num += (uint32)s_email_view_p->data_cache.cc_addrs_num + 1;
        }
        
        if (s_email_view_p->data_cache.bcc_addrs_num > 0)
        {
            item_num += (uint32)s_email_view_p->data_cache.bcc_addrs_num + 1;
        }
        
        if (MMIEMAIL_VIEW_FROM_OUTBOX == s_email_view_p->enter_type || MMIEMAIL_VIEW_FROM_SENTBOX == s_email_view_p->enter_type)
        {
            item_num--;
        }
    }   
    
    //date
    item_num++;
    
    //subject
    item_num++;
    if (s_email_view_p->is_subject_expand)
    {
        item_num++;
    }
    
    //attachment
    if (s_email_view_p->data_cache.attach_num > 0)
    {
        item_num++;
        if (s_email_view_p->is_attach_expand)
        {
            item_num += (uint32)s_email_view_p->data_cache.attach_num;
        }
    }
    
    //text
    item_num++;

    return item_num;
}

/*****************************************************************************/
//  Description :add email all item number
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL void ViewAddAllItems(void)
{
//    EMAIL_ATTACH_T  *add_file = PNULL;
    uint32 item_num = 0;
    
    item_num = ViewGetAllItemsNum();
    EMA_UTILS_TRACE("ViewAddAllItems, the item num is %d", item_num);
    EMA_UTILS_FREEIF(PNULL, s_email_view_p->focus_type_ptr);
    s_email_view_p->focus_type_ptr = EMA_UTILS_MALLOC(PNULL, item_num + 1);
    if (PNULL == s_email_view_p->focus_type_ptr)
    {
        return;
    }
    EMA_UTILS_MEMSET(s_email_view_p->focus_type_ptr, 0x00, item_num + 1);
    
    ViewAddFromItem();
    ViewAddDateItem();
    ViewAddSubjectItem();
    
    if (s_email_view_p->data_cache.attach_num > 0)
    { 
        ViewAddFileItem();
    } 
    ViewAddTextItem();
    GUIRICHTEXT_SetFocusItem(EMAIL_VIEW_CTRL_ID, s_email_view_p->focus_index);  
    //GUIRICHTEXT_SetItemSpace(EMAIL_VIEW_CTRL_ID, 0);
}

/*****************************************************************************/
//  Description :set email view content
//  Global resource dependence : 
//  Author:
//  Note: 
/****************************************************************************/
LOCAL int32 ViewSetContent
(
	EMAIL_MSGDATACACHE_T *data_ptr
)
{
//     wchar                    *pwszRecpt = PNULL;
// 	int32					 result					= EMAIL_ERROR;
// 	int						 i						= 0;
// 	uint32					 dwLen					= 0;
// 	int 					 uiNum					= 0; 
// 	EMAIL_ATTACH_T		 *temp = 0;
// 	EMAIL_ATTACH_T		 *header = 0;
// 	char					 acattachsize[64] = {0};

	
	EMA_UTILS_TRACE("!!!!!!!!!!!!Before Entering EMA_CopyEmailDataCache!!!!!!!!!!!!!!!!");

	if (EMAIL_SUCCESS != MMIEMAIL_CopyEmailDataCache(&s_email_view_p->data_cache, data_ptr, PNULL))
	{
		EMA_UTILS_TRACE("ViewSetContent, EMA_CopyEmailDataCache failed!");
		return EMAIL_ERROR;
	}

	EMA_UTILS_TRACE("!!!!!!!!!!!!EMA_CopyEmailDataCache End!!!!!!!!!!!!!!!!");

	/*show from*/
// 	if (MMIAPICOM_Wstrlen(pDataCache->from_addrs)> 0)
// 	{
// 		s_email_view_p->view_from.wstr_len = MMIAPICOM_Wstrlen(pDataCache->from_addrs);
// 		s_email_view_p->view_from.wstr_ptr = EMA_UTILS_MALLOC(PNULL, s_email_view_p->view_from.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_ASSERT(s_email_view_p->view_from.wstr_ptr);
// 		EMA_UTILS_MEMSET(s_email_view_p->view_from.wstr_ptr, 0, s_email_view_p->view_from.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_MEMCPY(s_email_view_p->view_from.wstr_ptr, pDataCache->from_addrs, s_email_view_p->view_from.wstr_len * sizeof(wchar));
// 	}
// 
//    /*Show 'To:'*/
// 	if (pDataCache->to_addrs_num > 0)
// 	{
// 		MMIEmail_EditGetRecipient(&pDataCache->to_addrs[0][0], &s_email_view_p->view_to.wstr_ptr,
// 			PNULL, pDataCache->to_addrs_num);
// 		s_email_view_p->view_to.wstr_len = MMIAPICOM_Wstrlen(s_email_view_p->view_to.wstr_ptr);
// 	}
// 
// 	/*Show 'Cc:'*/
// 	if (pDataCache->cc_addrs_num > 0)
// 	{
// 		MMIEmail_EditGetRecipient(&pDataCache->cc_addrs[0][0], &s_email_view_p->view_cc.wstr_ptr,
// 			PNULL, pDataCache->cc_addrs_num);
// 		s_email_view_p->view_cc.wstr_len = MMIAPICOM_Wstrlen(s_email_view_p->view_cc.wstr_ptr);
// 	}
//     
// 	/*Show attachment*/
// 	for (i = 0; i < pDataCache->attach_num; i++)
// 	{ 
// 		if (i == 5)
// 		{
// 			break;
// 		}
// 
// 		temp = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ATTACH_T));
// 		EMA_UTILS_ASSERT(temp);
// 		EMA_UTILS_MEMSET(temp, 0x00, sizeof(EMAIL_ATTACH_T));
// 		
// 		EMA_UTILS_MEMCPY(temp->file_path_ptr, pDataCache->attach_uri[i], MMIAPICOM_Wstrlen(pDataCache->attach_uri[i]) * sizeof(wchar));
// 		MMIAPICOM_WstrToStr(pDataCache->attach_size_ptr[i], acattachsize);
// 		temp->filesize = atoi(acattachsize);
// 		
// 		header = s_email_view_p->attach_list;
// 		if (PNULL == s_email_view_p->attach_list)
// 		{
// 			s_email_view_p->attach_list = temp;
// 		}
// 		else
// 		{
// 			while (header->next)
// 			{
// 				header = header->next;
// 			}
// 			header->next = temp;
// 		}
// 	}
// 
// 	/*Show 'Subject'*/
// 	if (PNULL != pDataCache->subject_ptr)
// 	{
// 		s_email_view_p->view_subject.wstr_len = MMIAPICOM_Wstrlen(pDataCache->subject_ptr);
// 		s_email_view_p->view_subject.wstr_ptr = EMA_UTILS_MALLOC(PNULL, s_email_view_p->view_subject.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_ASSERT(s_email_view_p->view_subject.wstr_ptr);
// 		EMA_UTILS_MEMSET(s_email_view_p->view_subject.wstr_ptr, 0, s_email_view_p->view_subject.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_MEMCPY(s_email_view_p->view_subject.wstr_ptr, pDataCache->subject_ptr, s_email_view_p->view_subject.wstr_len * sizeof(wchar));
// 	}
// 
// 	/*Show 'text'*/
// 	if (PNULL != pDataCache->data_buff_ptr)
// 	{
// 		s_email_view_p->view_text.wstr_len = MMIAPICOM_Wstrlen(pDataCache->data_buff_ptr);
// 		s_email_view_p->view_text.wstr_ptr = EMA_UTILS_MALLOC(PNULL, s_email_view_p->view_text.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_ASSERT(s_email_view_p->view_text.wstr_ptr);
// 		EMA_UTILS_MEMSET(s_email_view_p->view_text.wstr_ptr, 0, s_email_view_p->view_text.wstr_len * sizeof(wchar) + 2);
// 		EMA_UTILS_MEMCPY(s_email_view_p->view_text.wstr_ptr, pDataCache->data_buff_ptr, s_email_view_p->view_text.wstr_len * sizeof(wchar));
// 	}
    return EMAIL_SUCCESS;
}


/*****************************************************************************/
//  Description : email view exit
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void ViewExit(void)
{	
    if (PNULL != s_email_view_p)
    {
        EMA_UTILS_FREEIF(PNULL, s_email_view_p->focus_type_ptr);
        EMA_UTILS_FREEIF(PNULL, s_email_view_p->email_id_ptr);
        MMIEMAIL_ClearEmailCache(&s_email_view_p->data_cache);
        EMA_UTILS_FREEIF(PNULL, s_email_view_p);
    }
}

/*****************************************************************************/
//  Description :is focus on form item
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOnFromTitle(void)
{
   if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] != MMIEMAIL_VIEW_RICHTEXT_FROM_TITLE
      && s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] != MMIEMAIL_VIEW_RICHTEXT_TO_TITLE
      && s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] != MMIEMAIL_VIEW_RICHTEXT_CC_TITLE
      && s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] != MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE)
   {
      return FALSE;
   }
   
   if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
   {
      if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM_TITLE)
      {
         return TRUE;
      }
   }
   else
   {
      if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO_TITLE)
      {
         return TRUE;
      }
      else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC_TITLE
         && s_email_view_p->data_cache.to_addrs_num < 1)
      {
         return TRUE;
      }
      else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC_TITLE
         && s_email_view_p->data_cache.to_addrs_num < 1 && s_email_view_p->data_cache.cc_addrs_num < 1 )
      {
         return TRUE;
      }
   }
   
   return FALSE;  
}


/*****************************************************************************/
//  Description :handle add to pb option window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAddToPBOptWinMsg(
                                           EMAIL_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E  err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                 = EMAIL_VIEW_ADDTOPB_OPT_CTRL_ID;
	uint32  group_id                      = 0;
    uint32  menu_id                       = 0;
    int32   len                           = 0;
    wchar   *temp_ptr                     = PNULL;
    int32   result                        = 0;
    MMIPB_MAIL_T email_text = {0};
    GUIRICHTEXT_ITEM_T   item     = {0};
    
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
       GUIRICHTEXT_GetItem(EMAIL_VIEW_CTRL_ID, s_email_view_p->focus_index, &item);
       len = (item.text_data.buf.len + 1) * sizeof(wchar);
       temp_ptr = EMA_UTILS_MALLOC(PNULL, len);
       EMA_UTILS_BREAKNULL(temp_ptr, result, "out of memory");
       EMA_UTILS_MEMSET(temp_ptr, 0x00, len);
       EMA_UTILS_MEMCPY(temp_ptr, item.text_data.buf.str_ptr, len - 2);
       switch(menu_id)
       {
       case EMAIL_VIEW_ADDTOPB_OPT_NEW:
          {
            wchar   *email_address = PNULL;

            email_address = MMIEMAIL_GetEmailAddress(temp_ptr);

             if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_MAIL_LEN)
             {
                email_text.wstr_len = MMIPB_MAX_MAIL_LEN;
             }
             else
             {
                email_text.wstr_len = EMA_UTILS_WSTRLEN(email_address);
                if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
                {
                    email_text.wstr_len -= 1;
                }
             }
             EMA_UTILS_MEMCPY(email_text.wstr, email_address, email_text.wstr_len*sizeof(wchar));
             
             MMIAPIPB_AddContactWin(&email_text,PB_GROUP_ALL,MMIPB_ADD_TYPE_EMAIL);
          }
          break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
       case EMAIL_VIEW_ADDTOPB_OPT_EXIST:
          {
            wchar   *email_address = PNULL;

            email_address = MMIEMAIL_GetEmailAddress(temp_ptr);

            if (EMA_UTILS_WSTRLEN(email_address) > MMIPB_MAX_MAIL_LEN)
            {
               email_text.wstr_len = MMIPB_MAX_MAIL_LEN;
            }
            else
            {
               email_text.wstr_len = EMA_UTILS_WSTRLEN(email_address);
                if(PNULL != EMA_UTILS_WSTRCHR(email_address, L'>'))
                {
                    email_text.wstr_len -= 1;
                }
            }
            EMA_UTILS_MEMCPY(email_text.wstr, email_address, email_text.wstr_len*sizeof(wchar));
             
           //  MMIAPIPB_OpenEntryListForEx(&email_text, MMIPB_ADD_TYPE_EMAIL);
            MMIAPIPB_AddToExistContact(&email_text, MMIPB_ADD_TYPE_EMAIL, PNULL);
          }
          break;
#endif
       default :
          break;
       }    
       EMA_UTILS_FREE(PNULL, temp_ptr);
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

/*****************************************************************************/
//  Description :handle view window message
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleViewWinMsg(
                                    EMAIL_WINDOW_ID_E win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                    )
{
    MMI_RESULT_E err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                          = EMAIL_VIEW_CTRL_ID;  
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element      = {0};/*lint !e64*/    
    static BOOLEAN is_init_focus_item              = FALSE;
    GUI_RECT_T      richtext_rect                  = {0};
    static BOOLEAN is_switch_richtext              = TRUE;
    uint            len = 0;

    EMA_UTILS_TRACE("HandleViewWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_LCD_SWITCH:
        //is_switch_richtext = TRUE;
        MMIEMAIL_GetRichtextRect(&richtext_rect);
#ifdef MMI_PDA_SUPPORT
          richtext_rect.bottom -= EMAIL_BUTTONFORM_HEIGHT;
#endif
        GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
        break;
    case MSG_OPEN_WINDOW:
       {
          MMIEMAIL_GetRichtextRect(&richtext_rect);
#ifdef MMI_PDA_SUPPORT
          richtext_rect.bottom -= EMAIL_BUTTONFORM_HEIGHT;
#endif
          GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
          is_init_focus_item = TRUE;    
          GUIRICHTEXT_SetBorderColor(ctrl_id, MMI_GRAY_WHITE_COLOR);
          GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
          GUIRICHTEXT_DisableClipBoard(ctrl_id);
          MMK_SetAtvCtrl(win_id, ctrl_id); 
          MMIDEFAULT_SetBackLight(TRUE);
          is_switch_richtext = TRUE;
          s_email_view_p->is_content_extract = TRUE;
          MMIDEFAULT_AllowTurnOffBackLight(FALSE);
          GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
          
#if !defined(MMI_PDA_SUPPORT)//@tao.xue
          if (s_email_view_p->email_num > 0)
          {
             MMI_CTRL_ID_T titile_ctrl_id = MMK_GetWinTitleCtrlId(win_id);
             GUITITLE_BUTTON_STATE_E left_arrow = TITLE_BUTTON_NORMAL;
             GUITITLE_BUTTON_STATE_E right_arrow = TITLE_BUTTON_NORMAL;

             GUIWIN_SetTitleRollArrow(win_id,TRUE);        
             GUITITLE_SetRollArrowStatus(win_id, titile_ctrl_id, GUITITLE_BUTTON_LEFT, left_arrow);
             GUITITLE_SetRollArrowStatus(win_id, titile_ctrl_id, GUITITLE_BUTTON_RIGHT, right_arrow);
          }
#endif
#ifdef MMI_PDA_SUPPORT
        if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
       {
            EmailDestroyButtonFormCtrl(win_id,MMIEMAIL_INBOX_BUTTON_FORM,s_inbox_hot_buttons,
                                                            sizeof(s_inbox_hot_buttons)/sizeof(s_inbox_hot_buttons[0])
                                                            );
            EmailConstructButtonFormCtrl(win_id,MMIEMAIL_INBOX_BUTTON_FORM,s_inbox_hot_buttons,
                                                            sizeof(s_inbox_hot_buttons)/sizeof(s_inbox_hot_buttons[0])
                                                            );
       }
       else if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_OUTBOX)
       {
            EmailDestroyButtonFormCtrl(win_id,MMIEMAIL_OUTBOX_BUTTON_FORM,s_outbox_hot_buttons,
                                                            sizeof(s_outbox_hot_buttons)/sizeof(s_outbox_hot_buttons[0])
                                                            );
            EmailConstructButtonFormCtrl(win_id,MMIEMAIL_OUTBOX_BUTTON_FORM,s_outbox_hot_buttons,
                                                            sizeof(s_outbox_hot_buttons)/sizeof(s_outbox_hot_buttons[0])
                                                            );
       }
       else if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_SENTBOX)
       {
            EmailDestroyButtonFormCtrl(win_id,MMIEMAIL_SENTBOX_BUTTON_FORM,s_sentbox_hot_buttons,
                                                            sizeof(s_sentbox_hot_buttons)/sizeof(s_sentbox_hot_buttons[0])
                                                            );
            EmailConstructButtonFormCtrl(win_id,MMIEMAIL_SENTBOX_BUTTON_FORM,s_sentbox_hot_buttons,
                                                            sizeof(s_sentbox_hot_buttons)/sizeof(s_sentbox_hot_buttons[0])
                                                            );
       }
       
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
       }
       break;
    case MSG_LOSE_FOCUS:
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &s_email_view_p->focus_index);
        }        
        break;
    case MSG_GET_FOCUS:
        EMA_UTILS_TRACE("HandleViewWinMsg MSG_GET_FOCUS");
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
        {
            MMIEMAIL_CLEAR_CLIENT(win_id);
            //判断是否需要画richedit的items，不需要则更新即可
            if ((!is_switch_richtext 
                && s_email_view_p->menu_id != EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_ON
                && s_email_view_p->menu_id != EMAIL_VIEW_EMAIL_CONTENTEXTRACT_OPT_OFF)
               || !MMK_IsFocusWin(EMAIL_VIEW_WIN_ID))
            {
                GUIRICHTEXT_Update(ctrl_id);  
                GUIRICHTEXT_SetFocusItem(ctrl_id, s_email_view_p->focus_index);           
            }
            else
            {
                //MMI_CTRL_ID_T titile_ctrl_id = MMK_GetWinTitleCtrlId(win_id);
                char          acstring[32] = {0};
                wchar         *subtext = PNULL;
                GUI_RECT_T text_rect = {0};

                if (is_switch_richtext)
                {
                    is_switch_richtext = FALSE; 
                }

                GUIRICHTEXT_DeleteAllItems(ctrl_id);
                GUIRICHTEXT_ResetState(ctrl_id);
				ViewAddAllItems();
                GUIRICHTEXT_Update(ctrl_id);
                GUIRICHTEXT_ScrollToFocus(ctrl_id);
                s_email_view_p->menu_id = EMAIL_MENU_MAX_ITEM_ID;

                EMA_UTILS_SPRINTF(acstring, "%d/%d", s_email_view_p->email_index + 1, s_email_view_p->email_num);
                subtext = EMA_UTILS_STRTOWSTR(PNULL, acstring); 
                
                text_rect.top=TEXT_RECT_TOP; 
                text_rect.bottom = TEXT_RECT_BOTTOM; 
                text_rect.left = TEXT_RECT_LEFT; 
                text_rect.right = TEXT_RECT_RIGHT; 
                GUIWIN_SetTitleSubTextParam(win_id, &text_rect, PNULL, PNULL);
                GUIWIN_SetTitleSubText(win_id, TRUE, subtext, EMA_UTILS_WSTRLEN(subtext), TRUE);
                EMA_UTILS_FREEIF(PNULL, subtext);
                GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED/*GUIRICHTEXT_FOCUS_SHIFT_BASIC*/);   
                
                GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
                if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_URL, STXT_RETURN, TRUE);
                }
                else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL, STXT_RETURN, TRUE);
                }
                else// if (richtext_element.type == GUIRICHTEXT_FOCUS_TXT_NORMAL)
                {
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
                }
            }       
        }        
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      {
#if defined(MMI_PDA_SUPPORT)//@tao.xue
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMIEMAIL_INBOX_BUTTON_REPLY == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLY, (void*)s_email_view_p->data_cache.email_id);
                return MMI_RESULT_TRUE;
            }
            else if (MMIEMAIL_INBOX_BUTTON_REPLY_ALL == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLYALL, (void*)s_email_view_p->data_cache.email_id);
                return MMI_RESULT_TRUE;
            }
            else if (MMIEMAIL_INBOX_BUTTON_FORWARD == ((MMI_NOTIFY_T*) param)->src_id
                        || MMIEMAIL_SENTBOX_BUTTON_FORWARD == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD, (void*)s_email_view_p->data_cache.email_id);
                return MMI_RESULT_TRUE;
            }
            else if (MMIEMAIL_INBOX_BUTTON_DELETE == ((MMI_NOTIFY_T*) param)->src_id
                        || MMIEMAIL_OUTBOX_BUTTON_DELETE == ((MMI_NOTIFY_T*) param)->src_id
                        || MMIEMAIL_SENTBOX_BUTTON_DELETE == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMIEMAIL_OpenQueryWinByID(TXT_DELETE_QUERY, EMAIL_VIEW_DELETE_QUERY_WIN_ID);
                return MMI_RESULT_TRUE;
            }
            else if (MMIEMAIL_OUTBOX_BUTTON_RESEND == ((MMI_NOTIFY_T*) param)->src_id)
            {
               s_email_view_p->send_email_id = s_email_view_p->data_cache.email_id;
               MMIEMAIL_GetAllRecipients(PNULL, &s_email_view_p->data_cache);
               g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
               MMIEMAIL_SelectNet();
               return MMI_RESULT_TRUE;
            }
            else if (MMIEMAIL_SENTBOX_BUTTON_RESEND == ((MMI_NOTIFY_T*) param)->src_id)
            {
               EMAIL_MSGDATACACHE_T    *eml_cache_ptr = {0};
               int32                   ret = 0;

               EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, begin to resend email");
               eml_cache_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
               EMA_UTILS_BREAKNULL(eml_cache_ptr, ret, "EMAIL_VIEW_SENTBOX_OPT_RESEND, malloc EMAIL_MSGDATACACHE_T failed");
               EMA_UTILS_MEMSET(eml_cache_ptr, 0x00, sizeof(EMAIL_MSGDATACACHE_T));
               ret = MMIEMAIL_CopyEmailDataCache(eml_cache_ptr, &s_email_view_p->data_cache, PNULL);
               if(EMAIL_SUCCESS != ret)
               {
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, copy email data failed");
                   break;
               }
               eml_cache_ptr->email_id = 0;
               EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, begin to call MMIEMAIL_EditProcSendEmail");
               if(TRUE == MMIEMAIL_EditProcSendEmail((uint32)eml_cache_ptr))
               {
                   s_email_view_p->send_email_id = eml_cache_ptr->email_id;
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, the send msg id is %d", s_email_view_p->send_email_id);
                   MMIEMAIL_GetAllRecipients(PNULL, eml_cache_ptr);
                   g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                   MMIEMAIL_SelectNet();
               }
               else
               {
                   EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, call MMIEMAIL_EditProcSendEmail failed");
               }
               MMIEMAIL_ReleaseEmailDataCache(PNULL, eml_cache_ptr);
                return MMI_RESULT_TRUE;
            }
        }
#endif
         GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
         GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &s_email_view_p->focus_index); 

         if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
         {
             MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_NUMBER_OPT_WIN_TAB, PNULL);
         }
         else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
         {
            MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_URL_OPT_WIN_TAB, PNULL);
         }
         else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL)
         {
            MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_EMAIL_OPT_WIN_TAB, PNULL);
         }
         else if (IsFocusOnFromTitle())
         {
            s_email_view_p->is_from_expand = 0x01 & ~s_email_view_p->is_from_expand;
            is_switch_richtext = TRUE; 
         }
         else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_SUBJECT_TITLE
            && EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr) >0)
         {
            s_email_view_p->is_subject_expand = 0x01 & ~s_email_view_p->is_subject_expand;
            is_switch_richtext = TRUE; 
         }
         else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_ATTACH_TITLE)
         {
            s_email_view_p->is_attach_expand = 0x01 & ~s_email_view_p->is_attach_expand;
            is_switch_richtext = TRUE; 
         }
         else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM)
         {
#if defined(MMI_PDA_SUPPORT)
            MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_EMAIL_OPT_WIN_TAB, PNULL);
#else
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLY, (void*)s_email_view_p->data_cache.email_id);
#endif
         }
         else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO
            || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC
            || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC)
         {
#if defined(MMI_PDA_SUPPORT)
            MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_EMAIL_OPT_WIN_TAB, PNULL);
#else
            GUIRICHTEXT_ITEM_T   item     = {0};
            EMAOPT_T  stOpts[2] = {0};
            
            GUIRICHTEXT_GetItem(ctrl_id, s_email_view_p->focus_index, &item);
            
            stOpts[0].opt_id = EMA_OPT_TO;
            stOpts[0].val_ptr = (void*)item.text_data.buf.str_ptr;
            
            stOpts[1].opt_id = EMA_OPT_END;
            stOpts[1].val_ptr = PNULL;
            
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, (void*)&stOpts);	
#endif
         }
         else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_ATTACH)
         {
#if defined(MMI_PDA_SUPPORT)
            MMK_CreateWin((uint32*)EMAIL_VIEW_ATTACH_OPT_WIN_TAB, PNULL);
#else
            uint32 acttachindex = (uint32)s_email_view_p->focus_index; 
            uint32 item_num = 0;
            item_num = ViewGetAllItemsNum();
            item_num = item_num - (uint32)s_email_view_p->data_cache.attach_num - 1;
            acttachindex -= item_num;
            MMIAPIFMM_OpenFile(s_email_view_p->data_cache.attach_uri[acttachindex]); 
#endif
            //break;
         }
         else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TEXT
            && (s_email_view_p->data_cache.mail_status == EMA_BASIC_UNREAD_HEAD_ONLY || s_email_view_p->data_cache.mail_status == EMA_BASIC_READ_HEAD_ONLY))
         {
            g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
            EMA_UTILS_MEMSET(g_email_receive_str, 0x00, EMA_MAX_ADDR_LEN * sizeof(wchar));
            len = EMA_UTILS_WSTRLEN(s_email_view_p->data_cache.subject_ptr) * sizeof(wchar);
            EMA_UTILS_MEMCPY(g_email_receive_str, s_email_view_p->data_cache.subject_ptr, len);
            MMIEMAIL_SelectNet();
         }
      
         if (is_switch_richtext)
         {
#if 0
             is_switch_richtext = FALSE; 
             GUIRICHTEXT_DeleteAllItems(ctrl_id);
             GUIRICHTEXT_ResetState(ctrl_id);
             ViewAddAllItems();
             GUIRICHTEXT_Update(ctrl_id);
             GUIRICHTEXT_ScrollToFocus(ctrl_id);
             s_email_view_p->menu_id = EMAIL_MENU_MAX_ITEM_ID;
             GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED/*GUIRICHTEXT_FOCUS_SHIFT_BASIC*/);   
             
             GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
             if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
             {
                 GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
             }
             else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
             {
                 GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_URL, STXT_RETURN, TRUE);
             }
             else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL)
             {
                 GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL, STXT_RETURN, TRUE);
             }
             else// if (richtext_element.type == GUIRICHTEXT_FOCUS_TXT_NORMAL)
             {
                 GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
             }
#endif
             MMK_PostMsg(win_id, MSG_FULL_PAINT, 0, 0);
         }
      }
      break;

#if defined(MMI_PDA_SUPPORT) 
    case MSG_NOTIFY_TITLE_MENU_ICON:
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
//        GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
//        if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
//        {
//           MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_NUMBER_OPT_WIN_TAB, PNULL);
//        }
//        else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
//        {
//           MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_URL_OPT_WIN_TAB, PNULL);
//        }
//        else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL)
//        {
//           MMK_CreateWin((uint32*)EMAIL_VIEW_LOCATE_EMAIL_OPT_WIN_TAB, PNULL);
//        }
//        else 
#if !defined(MMI_PDA_SUPPORT)
       if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM
          || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO
          || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC
          || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC)
       {
          MMK_CreateWin((uint32 *)EMAIL_VIEW_ADD_TO_PB_OPT_WIN_TAB, PNULL);
       }
       else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_ATTACH)
       {
           uint32 acttachindex = (uint32)s_email_view_p->focus_index; 
           uint32 item_num = 0;
           item_num = ViewGetAllItemsNum();
           item_num = item_num - (uint32)s_email_view_p->data_cache.attach_num - 1;
           acttachindex -= item_num;
           MMK_CreateWin((uint32*)EMAIL_VIEW_SAVE_ATTACHMENT_WIN_TAB, (ADD_DATA)acttachindex);
       }
        else 
#endif
        if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_SUBJECT)
       {
          //do nothing
       }
       else if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_INBOX)
       {
          MMK_CreateWin((uint32 *)EMAIL_VIEW_INBOX_MENU_WIN_TAB, PNULL);
       }
       else if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_OUTBOX)
       {
          MMK_CreateWin((uint32 *)EMAIL_VIEW_OUTBOX_MENU_WIN_TAB, PNULL);
       }
       else if (s_email_view_p->enter_type == MMIEMAIL_VIEW_FROM_SENTBOX)
       {
          MMK_CreateWin((uint32 *)EMAIL_VIEW_SENTBOX_MENU_WIN_TAB, PNULL);
       }
       break;
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP: 
        {
            GUIRICHTEXT_GetFocusElement(ctrl_id, &richtext_element);
            GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &s_email_view_p->focus_index);
#if !defined(MMI_PDA_SUPPORT)//@tao.xue
            if (richtext_element.type == GUIRICHTEXT_FOCUS_PHONENUM)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
            }
            else if (richtext_element.type == GUIRICHTEXT_FOCUS_URL)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_URL, STXT_RETURN, TRUE);
            }
            else if (richtext_element.type == GUIRICHTEXT_FOCUS_EMAIL)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL, STXT_RETURN, TRUE);
            }
            else if (s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_FROM)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_REPLY, STXT_RETURN, TRUE);
            }
            else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_TO
               || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_CC
               || s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_BCC)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_WRITE_MAIL, STXT_RETURN, TRUE);
            }
            else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_SUBJECT)
            {
               GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
            }
            else if(s_email_view_p->focus_type_ptr[s_email_view_p->focus_index] == MMIEMAIL_VIEW_RICHTEXT_ATTACH)
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_SAVE, TXT_VIEW, STXT_RETURN, TRUE);
            }
            else
            {
               GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, TRUE);
            }
#endif            
        }
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
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        ViewExit();
        break;
	case EVT_EMA_DELETE_EML:
		{
			uint32 email_msg_id = 0;

			email_msg_id = s_email_view_p->data_cache.email_id;
			MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_DELETE_EML, (DPARAM)email_msg_id);
            MMK_CloseWin(win_id);
		}
		break;

   case EVT_EMA_RECEIVE:
   case EVT_EMA_RESEND:
      {
         uint32 netid =0;
         if (PNULL != param)
         {
            netid= *((uint32*)param);
         }

         if (msg_id == EVT_EMA_RESEND)
         {
            EMA_UTILS_TRACE("MMEMAIL-----MMIEMAIL_SP_TRS_SendEmail, the msg id is %d, the net id is %d", s_email_view_p->send_email_id, netid);
            MMIEMAIL_SP_TRS_SendEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), s_email_view_p->send_email_id, EMA_SP_ACTIVE_ACCOUNT, netid);
         }
         else
         {
            EMA_UTILS_TRACE("MMEMAIL-----MMIEMAIL_SP_TRS_RetrieveEmailBody, the msg id is %d, the net id is %d", s_email_view_p->data_cache.email_id, netid);
            MMIEMAIL_SP_TRS_RetrieveEmailBody(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, s_email_view_p->data_cache.email_id, netid);
         }
      }
		break;
   case EVT_EMA_VIEW_UPDATE:
      {
         uint32 email_msg_id = 0;

         email_msg_id = s_email_view_p->data_cache.email_id;
         MMIEMAIL_ClearEmailCache(&s_email_view_p->data_cache);
         MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_view_p->data_cache);

         if (s_email_view_p->data_cache.attach_num > 0)
         {
            s_email_view_p->data_cache.mail_status = EMA_READ_WITH_ATTACH;
         }
         else
         {
            s_email_view_p->data_cache.mail_status = EMA_BASIC_READ;
         }

         MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, &s_email_view_p->data_cache);
         
         is_switch_richtext = TRUE;
         s_email_view_p->is_content_extract = TRUE;
         MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);

         MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, 0);
         MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, 0);
      }
      break;
   case MSG_APP_LEFT:
   case MSG_APP_RIGHT:
   case MSG_CTL_TITLE_MOVE_PREV:
   case MSG_CTL_TITLE_MOVE_NEXT:
      if (s_email_view_p->email_num > 0)
      {
         if (MSG_APP_RIGHT == msg_id || MSG_CTL_TITLE_MOVE_NEXT == msg_id)
         {
            s_email_view_p->email_index = (s_email_view_p->email_index + 1) % s_email_view_p->email_num;
         }
         else if (MSG_APP_LEFT == msg_id || MSG_CTL_TITLE_MOVE_PREV == msg_id)
         {
            if (0 == s_email_view_p->email_index)
            {
               s_email_view_p->email_index = s_email_view_p->email_num - 1;
            }
            else
            {
               s_email_view_p->email_index--;
            }
         }
         
         MMIEMAIL_ClearEmailCache(&s_email_view_p->data_cache);
         MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), s_email_view_p->email_id_ptr[s_email_view_p->email_index], 
            EMA_SP_ACTIVE_ACCOUNT, &s_email_view_p->data_cache);

         s_email_view_p->focus_index = 0;
         s_email_view_p->is_content_extract = TRUE;
         is_switch_richtext = TRUE;

         if (EMA_BASIC_UNREAD == s_email_view_p->data_cache.mail_status 
            || EMA_UNREAD_WITH_ATTACH == s_email_view_p->data_cache.mail_status 
            || EMA_BASIC_UNREAD_HEAD_ONLY == s_email_view_p->data_cache.mail_status)
         {
            if (s_email_view_p->data_cache.mail_status == EMA_UNREAD_WITH_ATTACH 
               || s_email_view_p->data_cache.attach_num > 0)
            {
               s_email_view_p->data_cache.mail_status = EMA_READ_WITH_ATTACH;
            }
            else if(EMA_BASIC_UNREAD == s_email_view_p->data_cache.mail_status)
            {
               s_email_view_p->data_cache.mail_status = EMA_BASIC_READ;
            }
            else if (EMA_BASIC_UNREAD_HEAD_ONLY == s_email_view_p->data_cache.mail_status)
            {
               s_email_view_p->data_cache.mail_status = EMA_BASIC_READ_HEAD_ONLY;
            }

            MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, &s_email_view_p->data_cache);
            
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
         }
         MMK_SendMsg(win_id, MSG_FULL_PAINT, 0);
      }
      break;
    default :
       err_code = MMI_RESULT_FALSE;
       break;
    }
	
    
    return err_code;
}

/*****************************************************************************/
//  Description :get all email id
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ViewGetAllMsgId()
{
   EMAIL_VECTOR_T *box_vector_ptr = PNULL;
   int32    i = 0;
   EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
      
   MMIEMAIL_GetVectorinfoAndSelectIndex(&s_email_view_p->email_index, &box_vector_ptr);
   s_email_view_p->email_num = MMIEMAIL_VectorGetVectorNumber(box_vector_ptr);

   s_email_view_p->email_id_ptr = EMA_UTILS_MALLOC(PNULL, sizeof(uint32) * s_email_view_p->email_num);
   if (PNULL == s_email_view_p->email_id_ptr)
   {
       return;
   }
   EMA_UTILS_MEMSET(s_email_view_p->email_id_ptr, 0x00, sizeof(uint32) * s_email_view_p->email_num);

   for (i = 0; i < (int32)s_email_view_p->email_num; i++)
   {
      MMIEMAIL_VectorGetAt(box_vector_ptr, i, &email_header_ptr);
      s_email_view_p->email_id_ptr[i] = email_header_ptr->email_id;
   }
}
/*****************************************************************************/
// 	Description: create view window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_CreateViewWin(MMIEMAIL_VIEW_REASON_TYPE_E view_type, void *data_ptr)
{
    
    //s_email_view_p->enter_type = MMIEMAIL_VIEW_FROM_INBOX;
    EMA_UTILS_FREEIF(PNULL, s_email_view_p);
    s_email_view_p = EMA_UTILS_MALLOC(PNULL, sizeof(MMIEMAIL_VIEW_T));
    if(PNULL == s_email_view_p){
      return;
    }
    s_email_view_p->enter_type = view_type;
    ViewGetAllMsgId();
    ViewSetContent((EMAIL_MSGDATACACHE_T*)data_ptr);
   // ViewAddAllItems();
    MMK_CreateWin((uint32 *)EMAIL_VIEW_WIN_TAB, PNULL);
    return;
}
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void EmailDestroyButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIEMAIL_BUTTON_T *button_array,uint16 array_len)
{
    uint16 index = 0;
    if(0 != MMK_GetCtrlHandleByWin(win_id, ctrl_id))
    {
        MMK_DestroyControl(ctrl_id);
        for(index = 0; index < array_len; index++)
        {
            MMK_DestroyDynaCtrl(button_array[index].ctrl_id);
        }
    }
}

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void EmailConstructButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIEMAIL_BUTTON_T *button_array,uint16 array_len)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    uint16 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    
    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    //create MMIIDLE_DIAL_BUTTON_FORM_CTRL_ID control
    form_init.both_rect.v_rect.left =  client_rect.v_rect.left;
    form_init.both_rect.v_rect.top = client_rect.v_rect.bottom - EMAIL_BUTTONFORM_HEIGHT + 1;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;
    
    form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    form_init.both_rect.h_rect.top = client_rect.h_rect.bottom - EMAIL_BUTTONFORM_HEIGHT + 1;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    form_init.layout_type = GUIFORM_LAYOUT_SBS;    

    form_ctrl.ctrl_id           = ctrl_id;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);
        
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(ctrl_id, &bg);
    GUIFORM_SetMargin(ctrl_id, 6);
    //not slide
    GUIFORM_IsSlide(ctrl_id,FALSE);

    for(index = 0; index < array_len; index++)
    {    
        form_child_ctrl.child_handle = button_array[index].ctrl_id;
        form_child_ctrl.guid = SPRD_GUI_BUTTON_ID;
        
        button_init.bg.bg_type = GUI_BG_IMG;
        button_init.bg.img_id  = button_array[index].bg_img_id;
        form_child_ctrl.init_data_ptr = &button_init;
        
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;

        GUIFORM_CreatDynaChildCtrl(win_id, ctrl_id,  &form_child_ctrl);

        bg.img_id = button_array[index].bg_img_id;
        GUIBUTTON_SetBg(button_array[index].ctrl_id, &bg);
        
        bg.img_id = button_array[index].pressed_bg_img_id;
        GUIBUTTON_SetPressedBg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].fg_img_id;
        GUIBUTTON_SetFg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].pressed_fg_img_id;
        GUIBUTTON_SetPressedFg(button_array[index].ctrl_id, &bg);     

        GUIBUTTON_SetRunSheen(button_array[index].ctrl_id, FALSE);

        child_height.type = GUIFORM_CHILD_HEIGHT_PERCENT;
        child_height.add_data = 100;
        GUIFORM_SetChildHeight(MMK_GetCtrlHandleByWin(win_id, ctrl_id), MMK_GetCtrlHandleByWin(win_id, button_array[index].ctrl_id), &child_height);

    }
}
#endif


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
