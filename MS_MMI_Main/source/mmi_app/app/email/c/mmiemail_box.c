/***************************************************************************************
** File Name:      mmiemail_box.c                                                      *
** Author:                                                                             *
** Date:           05/13/2011                                                          *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:    This File will gather functions that all events to this app are     *
**                 handled. These functions don't be changed by project changed.       *
****************************************************************************************
**                         Important Edit History                                      *
** ------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                         *
** 05/2011                         create mmiemail_box.c                               *
**                                                                                     *
***************************************************************************************/

/**------------------------------------------------------------------------------------*
**                         Include Files                                               *
**------------------------------------------------------------------------------------*/
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
#include "mmi_solarlunar.h"

//#include "mmipic_image.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"

#include "mmiemail_id.h"
#include "sysMemFile.h"
#include "mmiemail_box.h"
#include "email_text.h"
#include "email_image.h"
#include "mmiemail_def.h"
#include "mmiemail_menutable.h"
#include "mmiemail_utils.h"
#include "mmiemail_sp_api.h"
#include "mmiemail_edit.h"
#include "mmiemail_view.h"

#include "mms_image.h"
#include "mmiemail_dlg.h"
#include "guitab.h"
#include "mmiemail_main.h"
#include "guiform.h"
#ifndef MMI_PDA_SUPPORT
#include "guilabel.h"
#endif
//#include "gui_listbox.h"
/**------------------------------------------------------------------------------------*
**                         Macro Declaration                                           *
**------------------------------------------------------------------------------------*/
//#define MMIEMAIL_RICHTEXT_ITEM_NUM_MAX  14
#define MMIEMAIL_FILE_NAME_MAX			40
//LOCAL uint32 s_marked_email_num = 0; //标记信息的数目

/**------------------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                           *
**------------------------------------------------------------------------------------*/
typedef struct email_box_globle_variable_tag
{
    uint32                      *email_id_ptr;
    EMAIL_VECTOR_T              *box_vector_ptr;
    MMIEMAIL_BOX_REASON_TYPE_E  box_reason_type;
    uint16                      selected_index;
    uint32                      delete_result;
    EMAIL_BOX_VECTORMODEL_T     box_mark_vectormodel;
    BOOLEAN                     *is_marked_ptr;
    MN_DUAL_SYS_E               recv_dual_sim;
    uint32                      email_id;
    uint16                      sort_type;
    uint16                      descent;
    MMIEMAIL_TRANS_TYPE_E       trans_type;
    EMAIL_MSGDATACACHE_T        email_datacache;
    EMAIL_BOXTYPE_E             cur_type;
    BOOLEAN                     is_need_sort;
}EMAIL_BOX_GLOBLE_VARIABLE_T;

typedef struct email_boxinfo_list_tag
{
    EMAIL_MSGHEAD_T                 *email_header_ptr;
    int32                           order_index;
    struct email_boxinfo_list_tag   *next_ptr;
}EMAIL_BOXINFO_LIST_T;

typedef struct email_box_orderlist_tag
{
    EMAIL_MSGHEAD_T                 *email_header_ptr;
    struct email_box_orderlist_tag  *next_ptr;
}EMAIL_BOX_ORDERLIST_T;

/**------------------------------------------------------------------------------------*
**                         Constant Variables                                          *
**------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
**                         STATIC DEFINITION                                           *
**------------------------------------------------------------------------------------*/
static EMAIL_BOX_GLOBLE_VARIABLE_T *s_email_box_globle_p = PNULL;
LOCAL BOOLEAN               s_is_delete_all = FALSE;
MMIEMAIL_TRANS_TYPE_E       g_email_trans_type = MMIEMAIL_TRANS_MSGHEAD; /*lint !e552 */
extern HEmlSPMgr            g_email_sp_handle;
extern uint32               g_email_is_send;
extern wchar                g_email_receive_str[EMA_MAX_ADDR_LEN];

extern uint16               inbox_sort_type;
extern uint16               other_box_sort_type;
/*-------------------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                                     *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : to update box's date by EMAIL_BOXTYPE_E.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateBoxData(EMAIL_BOXTYPE_E box_type);
/**************************************************************************************/
// Description : delete email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN PublicProcDelEmail(uint32 email_id);
/**************************************************************************************/
// Description : to send email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN PublicProcSending(uint32 email_id, uint32 net_id);
/**************************************************************************************/
// Description : edit email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN ComposerProcEdit(uint32 email_id);
/**************************************************************************************/
// Description : to view email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN ViewProcEnter(uint32 email_id);
/**************************************************************************************/
// Description : check,need to download email's body or not.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
//LOCAL int32 ViewProc_CheckDLBody(uint32 msg_id);
/**************************************************************************************/
// Description : move email to draft from outbox.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN OutBoxProc_MovetoDraft(uint32 email_id);
/**************************************************************************************/
// Description : receive new email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void RecvNewEmail(void);
/**************************************************************************************/
// Description : box list date init.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 InitBoxListIndex(EMAIL_BOXINFO_LIST_T *box_list_ptr);
/**************************************************************************************/
// Description : to order email in box by ordertype.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 MarkOrderIndex(
                           EMAIL_BOXINFO_LIST_T *box_list_ptr,
                           int32 box_by_ordertype
                           );
/**************************************************************************************/
// Description : order box list date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 OrderBoxList(
                         EMAIL_VECTOR_T *box_vector_ptr,
                         EMAIL_BOXINFO_LIST_T *box_list_ptr,
                         void *mem_hdr_ptr,
                         int32 vector_num
                         );
/**************************************************************************************/
// Description : clear box list info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 ClearBoxInfoList(
                             void *mem_hdr_ptr,
                             EMAIL_BOXINFO_LIST_T **box_list_pptr
                             );
/**************************************************************************************/
// Description : clear box order list info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 ClearBoxOrderList(
                              void *mem_hdr_ptr,
                              EMAIL_BOX_ORDERLIST_T **box_list_pptr
                              );
/**************************************************************************************/
// Description : box list order result.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 BoxOrderResult(
                             int32 box_by_ordertype,
                             uint32 data
                             );
/**************************************************************************************/
// Description : update inbox email's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN InBoxProc_UpdateEmailStatus(
                                          uint32 email_status,
                                          uint32 email_id
                                          );
/**************************************************************************************/
// Description : sort email by sort type.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 BoxMgrSortBy(uint32 sort_by_type);
/**************************************************************************************/
// Description : free email vector.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void MarkEmailVectorFree(void *data_ptr);
/**************************************************************************************/
// Description : to handle  box window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleBoxWinMsg(
                                   EMAIL_WINDOW_ID_E win_id, 
                                   MMI_MESSAGE_ID_E msg_id, 
                                   DPARAM param
                                   );
/**************************************************************************************/
// Description : to handle box's menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleBoxMenuWinMsg(
                                       EMAIL_WINDOW_ID_E win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM param
                                       );
/**************************************************************************************/
// Description : handle to waitting dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleWaitWinMsg(
                                    MMI_WIN_ID_T        win_id,  //IN:
                                    MMI_MESSAGE_ID_E    msg_id,  //IN:
                                    DPARAM              param    //IN:
                                    );

/**************************************************************************************/
// Description : handle to waitting dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllWaitWinMsg(
                                    MMI_WIN_ID_T        win_id,  //IN:
                                    MMI_MESSAGE_ID_E    msg_id,  //IN:
                                    DPARAM              param    //IN:
                                    );
/**************************************************************************************/
// Description : handle to selete SIM to receive email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleSimSelectRecvWinMsg(
                                             EMAIL_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
/**************************************************************************************/
// Description : handle to delete dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleRecvTypeMenuWinMsg(
                                            MMI_WIN_ID_T        win_id,  //IN:
                                            MMI_MESSAGE_ID_E    msg_id,  //IN:
                                            DPARAM              param    //IN:
                                            );
/**************************************************************************************/
// Description : to get cur focus box win id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_WIN_ID_T GetCurFocusChildBoxWinId(void);
/**************************************************************************************/
// Description : to handle the message of Email inbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleInBoxChildWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param
                                          );
/**************************************************************************************/
// Description : to handle the message of Email outbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleOutBoxChildWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
/**************************************************************************************/
// Description : to handle the message of Email sentbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleSentBoxChildWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            );
/**************************************************************************************/
// Description : to handle the message of Email draftbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleDraftBoxChildWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             );
/**************************************************************************************/
// Description : 添加tab页和子窗口.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void  AddTabsAndChildWindows(void);
/**************************************************************************************/
// Description : to check Email's datecache.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN BoxCheckDatecach(EMAIL_MSGDATACACHE_T *email_datacache_ptr);
/**************************************************************************************/
// Description : to set the forword's option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetForwordOpt(
                         MMI_CTRL_ID_T ctrl_id,
                         MMI_MENU_GROUP_ID_T group_id,
                         EMAIL_MSGDATACACHE_T *email_datacache_ptr
                         );
/**************************************************************************************/
// Description : handle to set date and time.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleSetDateTimeWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );
/**************************************************************************************/
// Description : creat a win can set date and time to use.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateSetDateTimeWin(MMI_CTRL_ID_T ctrl_id);
/*-------------------------------------------------------------------------------------*
**                          PUBLIC FUNCTION DECLARE                                    *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : to loda email list in box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN LoadEmailListFromOrder(
                                     EMAIL_BOXTYPE_E box_type, 
                                     MMI_CTRL_ID_T ctrl_id
                                     );
/**************************************************************************************/
// Description : to loda email date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void ListEmailFromOrder(
                              EMAIL_BOXTYPE_E box_type,
                              MMI_CTRL_ID_T ctrl_id
                              );
/**************************************************************************************/
// Description : to set the cur box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetCurType(EMAIL_BOXTYPE_E box_type);
/**************************************************************************************/
// Description : to get the cur box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL EMAIL_BOXTYPE_E GetCurType(void);
/**************************************************************************************/
// Description : to set item date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetListItemData(
                           EMAIL_BOXTYPE_E box_type, 
                           MMI_CTRL_ID_T     ctrl_id,
                           uint16            index
                           );
/**************************************************************************************/
// Description : set email box list item is grayed 
// Global resource dependence : s_email_box_globle_p
// Author: qiang.zhang
// Note:
/**************************************************************************************/
LOCAL void SetListItemGrayed(MMI_CTRL_ID_T     ctrl_id);
/**************************************************************************************/
// Description : to set all email marked.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetAllEmailMarked(
                             EMAIL_BOXTYPE_E box_type,
                             BOOLEAN           is_marked //IN:
                             );
/**************************************************************************************/
// Description : is need sort or not.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN IsNeedSort(void);
/**************************************************************************************/
// Description : clear email date cache in edit.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
//extern int32 MMIEMAIL_ClearEmailCache(
//                                      EMAIL_MSGDATACACHE_T *msg_datacache_ptr
//                                      );

/**************************************************************************************/
// Description : to update draftbox's e-mail read state.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN UpdateDraftReadState(uint32 email_id);
#if defined(MMI_PDA_SUPPORT)
/**************************************************************************************/
// Description : common func for mail box mark.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL void EmailBoxMark(void);
/**************************************************************************************/
// Description : common func for get list ctrl id.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL MMI_CTRL_ID_T GetEmailBoxListCtrlId(void);
/**************************************************************************************/
// Description : common func for mail box unmark all.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL void EmailBoxUnMarkAll(void);
#endif

/**************************************************************************************/
// Description : get create box window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_BOX_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxWinMsg),
        WIN_ID(EMAIL_BOX_WIN_ID),
        WIN_SOFTKEY(STXT_OPTION, TXT_COMM_OPEN, STXT_RETURN),
#if defined(MMI_GUI_STYLE_TYPICAL) || defined MMI_GUI_STYLE_MINISCREEN
        CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NULL, MMIEMAIL_MSGBOX_TAB_CTRL_ID),
#else
        CREATE_TAB_CTRL(GUI_TAB_ITEM_LEFT, GUITAB_STATE_NONEED_SCROLL|GUITAB_STATE_SINGLE_LINE, MMIEMAIL_MSGBOX_TAB_CTRL_ID),
#endif
        END_WIN
};
/**************************************************************************************/
// Description : get create inbox window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_INBOX_CHILD_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleInBoxChildWinMsg),
        WIN_ID(EMAIL_INBOX_CHILD_WIN_ID),
#if defined(MMI_PDA_SUPPORT)
        WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIEMAIL_INBOX_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_DELETE, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIEMAIL_INBOX_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, EMAIL_INBOX_LISTBOX_CTRL_ID, MMIEMAIL_INBOX_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_INBOX_LISTBOX_CTRL_ID),
#endif
        END_WIN
};
/**************************************************************************************/
// Description : get create outbox window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE( EMAIL_OUTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleOutBoxChildWinMsg),
        WIN_ID(EMAIL_OUTBOX_CHILD_WIN_ID),
#if defined(MMI_PDA_SUPPORT)
        WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIEMAIL_OUTBOX_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_DELETE, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIEMAIL_OUTBOX_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, EMAIL_OUTBOX_LISTBOX_CTRL_ID, MMIEMAIL_OUTBOX_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_OUTBOX_LISTBOX_CTRL_ID),
#endif
        END_WIN
};
/**************************************************************************************/
// Description : get create sentbox window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE( EMAIL_SENTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleSentBoxChildWinMsg),
        WIN_ID(EMAIL_SENTBOX_CHILD_WIN_ID),
#if defined(MMI_PDA_SUPPORT)
        WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIEMAIL_SENTBOX_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_DELETE, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIEMAIL_SENTBOX_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, EMAIL_SENTBOX_LISTBOX_CTRL_ID, MMIEMAIL_SENTBOX_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_SENTBOX_LISTBOX_CTRL_ID),
#endif
        END_WIN
};
/**************************************************************************************/
// Description : get create draftbox window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE( EMAIL_DRAFTBOX_CHILD_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleDraftBoxChildWinMsg),
        WIN_ID(EMAIL_DRAFTBOX_CHILD_WIN_ID),
#if defined(MMI_PDA_SUPPORT)
        WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIEMAIL_DRAFTBOX_FORM_CTRL_ID),
        CHILD_SOFTKEY_CTRL(TXT_DELETE, TXT_NULL, STXT_CANCEL, MMICOMMON_SOFTKEY_CTRL_ID, MMIEMAIL_DRAFTBOX_FORM_CTRL_ID),
        CHILD_LIST_CTRL(TRUE, GUILIST_TEXTLIST_E, EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, MMIEMAIL_DRAFTBOX_FORM_CTRL_ID),
#else
        WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, EMAIL_DRAFTBOX_LISTBOX_CTRL_ID),
#endif
        END_WIN
};
/**************************************************************************************/
// Description : get create inbox's menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_INBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create inbox's menu window, no option
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_INBOX_MENU_NONE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_NONE_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create outbox's menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_OUTBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_OUTBOX_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create sentbox's menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_SENTBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MENU_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_SNETBOX_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create draftbox's menu window
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_DRAFTBOX_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MENU_WIN_ID),
        WIN_STYLE(WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_DRAFTBOX_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create receive email menu of select sim
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_RECV_SELECT_SIM_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleSimSelectRecvWinMsg),
        WIN_ID(EMAIL_RECV_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, EMAIL_RECV_SELECT_SIM_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : get create receive email option of receive type
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_RECV_TYPE_MENU_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleRecvTypeMenuWinMsg),
        WIN_ID(EMAIL_RECV_TYPE_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_RECV_TYPE_OPT, EMAIL_RECV_TYPE_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
/**************************************************************************************/
// Description : mark email by date time
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
WINDOW_TABLE(EMAIL_MARK_BY_DATETIME_WIN_TAB) =
{
    WIN_STATUSBAR,
        WIN_TITLE(TXT_EMAIL_SET_DATETIME),
        WIN_FUNC((uint32)HandleSetDateTimeWinMsg),
        WIN_ID(EMAIL_MARK_BY_DATETIME_WIN_ID),
#ifdef MMI_PDA_SUPPORT
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,EMAIL_SET_DATE_FORM_CTRL_ID),
        CHILD_EDIT_TOUCH_DATE_CTRL(TRUE,EMAIL_SET_DATE_CTRL_ID,EMAIL_SET_DATE_FORM_CTRL_ID),
#else
        CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, EMAIL_SET_DATE_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, EMAIL_SET_DATE_LABLE_CTRL_ID, EMAIL_SET_DATE_FORM_CTRL_ID),
        CHILD_EDIT_DATE_CTRL(TRUE, EMAIL_SET_DATE_CTRL_ID, EMAIL_SET_DATE_FORM_CTRL_ID),
#endif
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};

#if defined(MMI_PDA_SUPPORT)
WINDOW_TABLE( EMAIL_INBOX_POP_MENU_WIN_TAB ) = 
{
    //    WIN_FUNC( (uint32)HandleBoxPopMenuWinMsg),
    WIN_FUNC( (uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_POP_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT), 
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        //    CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_POP_OPT, EMAIL_BOX_POP_MENU_CTRL_ID),
        CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_POP_OPT, EMAIL_BOX_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( EMAIL_OUTBOX_POP_MENU_WIN_TAB ) = 
{
    //    WIN_FUNC( (uint32)HandleBoxPopMenuWinMsg),
    WIN_FUNC( (uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_POP_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT), 
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        //    CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_POP_OPT, EMAIL_BOX_POP_MENU_CTRL_ID),
        CREATE_POPMENU_CTRL(MENU_EMAIL_OUTBOX_POP_OPT, EMAIL_BOX_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( EMAIL_SENTBOX_POP_MENU_WIN_TAB ) = 
{
    //    WIN_FUNC( (uint32)HandleBoxPopMenuWinMsg),
    WIN_FUNC( (uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_POP_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT), 
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        //    CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_POP_OPT, EMAIL_BOX_POP_MENU_CTRL_ID),
        CREATE_POPMENU_CTRL(MENU_EMAIL_SENTBOX_POP_OPT, EMAIL_BOX_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( EMAIL_DRAFTBOX_POP_MENU_WIN_TAB ) = 
{
    //    WIN_FUNC( (uint32)HandleBoxPopMenuWinMsg),
    WIN_FUNC( (uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_POP_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT), 
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        //    CREATE_POPMENU_CTRL(MENU_EMAIL_INBOX_POP_OPT, EMAIL_BOX_POP_MENU_CTRL_ID),
        CREATE_POPMENU_CTRL(MENU_EMAIL_DRAFTBOX_POP_OPT, EMAIL_BOX_MENU_CTRL_ID),
        END_WIN
};

WINDOW_TABLE(EMAIL_BOX_MARK_MENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleBoxMenuWinMsg),
        WIN_ID(EMAIL_BOX_MARK_MENU_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
        CREATE_POPMENU_CTRL(MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MENU_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
#endif

/**------------------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                         *
**------------------------------------------------------------------------------------*/
/**************************************************************************************/
// Description : to handle box's menu.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleBoxMenuWinMsg(
                                        EMAIL_WINDOW_ID_E win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM param
                                        )
{
    MMI_RESULT_E           err_code    = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T          ctrl_id     = EMAIL_BOX_MENU_CTRL_ID;
    MMI_MENU_ID_T          menu_id     = 0;
    MMI_MENU_GROUP_ID_T    group_id    = 0;
    uint16                 cur_index   = 0;
    MMI_CTRL_ID_T          cur_ctrl_id = 0;
    int                    mark_num    = 0;
    uint32                 unread_num  = 0;
    uint32                 eml_msg_id  = 0;
    int32                  ret         = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type && PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            SetListItemGrayed(ctrl_id);            
        }
        if (PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            switch (s_email_box_globle_p->box_reason_type)
            {
            case MMIEMAIL_BOX_FROM_INBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_INBOX_LISTBOX_CTRL_ID, PNULL, 0);
                if(EMAIL_SUCCESS != MMIEMAIL_InboxGetUnreadNum(&unread_num))
                {
                    EMA_UTILS_LOG_ERROR(("EmailAFmMgr_MainMenu_New, MMIEMAIL_InboxGetUnreadNum"));
                }
                cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                break;
            case MMIEMAIL_BOX_FROM_OUTBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_OUTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                break;
            case MMIEMAIL_BOX_FROM_SENTBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_SENTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                break;
            case MMIEMAIL_BOX_FROM_DRAFT:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                cur_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                break;
            default :
                break;
            }
            
#if defined(MMI_PDA_SUPPORT)
            if(MMIEMAIL_BOX_FROM_INBOX != s_email_box_globle_p->box_reason_type)
            {
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_INBOX_OPT_MARKASREAD, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_INBOX_OPT_MARKASUNREAD, TRUE);
            }
#endif
            
            if (0 == mark_num)
            {
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_CANCEL_MARK, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_CANCEL_MARKALL, TRUE);
#if defined(MMI_PDA_SUPPORT)
                if(MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
                {
                    GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_INBOX_OPT_MARKASREAD, TRUE);
                    GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_INBOX_OPT_MARKASUNREAD, TRUE);
                }
#endif
                
                if (MMIEMAIL_BOX_FROM_DRAFT == s_email_box_globle_p->box_reason_type)
                {
                    EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
                    
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
                    eml_msg_id = email_header_ptr->email_id;
                    MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
                    MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), eml_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
                    
                    if (0 == s_email_box_globle_p->email_datacache.to_addrs_num
                        && 0 == s_email_box_globle_p->email_datacache.cc_addrs_num
                        && 0 == s_email_box_globle_p->email_datacache.bcc_addrs_num)
                    {
                        GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_DRAFTBOX_OPT_SEND, TRUE);
                    }
                }
                
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_REPLY, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_REPLYTOALL, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_FORWORD, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_FORWORD, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_SORT, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_SEARCHMAIL, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_OUTBOX_OPT_RESEND, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_RESEND, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_SORT, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_DRAFTBOX_OPT_SORT, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_OUTBOX_OPT_MOVETODRAFT, FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_RECVEMAIL, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_REPLY, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_REPLYTOALL, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_FORWORD, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_FORWORD_ATTACH, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_FORWORD, TRUE);
                //GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_SORT, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_INBOX_OPT_SEARCHMAIL, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_OUTBOX_OPT_RESEND, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_RESEND, TRUE);
                //GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_SENTBOX_OPT_SORT, TRUE);
                GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_DRAFTBOX_OPT_SEND, TRUE);
                //GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_DRAFTBOX_OPT_SORT, TRUE);
                if(cur_ctrl_id ==EMAIL_DRAFTBOX_LISTBOX_CTRL_ID)
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_OUTBOX_OPT_MOVETODRAFT, TRUE);
                }
                else
                {
                    GUIMENU_SetItemGrayed(ctrl_id, group_id, EMAIL_OUTBOX_OPT_MOVETODRAFT, FALSE);
                }
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_MARK_BY_TIME, TRUE);
                    
                if (mark_num == MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr))
                {                    
                    GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_MARKALL, TRUE);
                }

                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASUNREAD, FALSE);
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASREAD, FALSE);
            }
            
            cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
            if (s_email_box_globle_p->is_marked_ptr[cur_index])
            {
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_MARK, TRUE);
            }
            else if (!s_email_box_globle_p->is_marked_ptr[cur_index])
            {
                GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_BOX_MARK_OPT, EMAIL_BOX_MARK_OPT_CANCEL_MARK, TRUE);
            }
            
            eml_msg_id = s_email_box_globle_p->email_id;
            MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
            MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), eml_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
            SetForwordOpt(ctrl_id, group_id, &s_email_box_globle_p->email_datacache);
        }
        
#if defined(MMI_PDA_SUPPORT)
        if (EMAIL_BOX_POP_MENU_WIN_ID == win_id )
        {
            MMI_STRING_T title_str = {0};
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            
            if (PNULL != s_email_box_globle_p->box_vector_ptr)
            {
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
                
                if (PNULL != email_header_ptr->wsz_subject_ptr)
                {
                    title_str.wstr_ptr = email_header_ptr->wsz_subject_ptr;
                    
                    title_str.wstr_len = EMA_UTILS_WSTRLEN(title_str.wstr_ptr);
                    GUIMENU_SetMenuTitle(&title_str,ctrl_id);
                }
            }
        }
#endif
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
        EMA_UTILS_TRACE("MMIEmail_HandleEditEditMenuWinMsg menu_id = 0x%x", menu_id);
        switch(menu_id)
        {
        case EMAIL_INBOX_OPT_RECVEMAIL:
            s_email_box_globle_p->is_need_sort = TRUE;
            MMIEMAIL_PublicProc_Retrieve();
            break;
            
        case EMAIL_INBOX_OPT_SEARCHMAIL:
            break;
            
        case EMAIL_INBOX_OPT_REPLY:
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLY, (void*)s_email_box_globle_p->email_id);
            break;
            
        case EMAIL_INBOX_OPT_REPLYTOALL:
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_REPLYALL, (void*)s_email_box_globle_p->email_id);
            break;
            
        case EMAIL_INBOX_OPT_CREATE_AN_EMAIL:
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_NEW, PNULL);
            break;
            
        case EMAIL_SENTBOX_OPT_FORWORD:
        case EMAIL_INBOX_OPT_FORWORD:
        case EMAIL_VIEW_FORWARD_OPT_WITH_ATTACHMENT:
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD, (void*)s_email_box_globle_p->email_id);
            break;
            
        case EMAIL_VIEW_FORWARD_OPT_WITHOUT_ATTACHMENT:
            MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_FORWARD_WITHOUT_ATTACH, (void*)s_email_box_globle_p->email_id);
            break;
            
        case EMAIL_INBOX_OPT_DELETE:
        case EMAIL_OUTBOX_OPT_DELETE:
        case EMAIL_SENTBOX_OPT_DELETE:
        case EMAIL_DRAFTBOX_OPT_DELETE:
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            }
            break;
            
        case EMAIL_INBOX_OPT_DELETEALL:
        case EMAIL_OUTBOX_OPT_DELETEALL:
        case EMAIL_SENTBOX_OPT_DELETEALL:
        case EMAIL_DRAFTBOX_OPT_DELETEALL:
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ALL;
                MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DELALLCONFIRM, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            }
            break;
            
        case EMAIL_BOX_MARK_OPT_MARK:
            {
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_OUTBOX:
                    cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_SENTBOX:
                    cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_DRAFT:
                    cur_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                    break;
                default :
                    break;
                }
                
                cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
                if (!s_email_box_globle_p->is_marked_ptr[cur_index])
                {
                    GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
                    if(s_email_box_globle_p->box_reason_type == MMIEMAIL_BOX_FROM_INBOX)
                    {
                        GUILIST_SetMaxSelectedItem(cur_ctrl_id, INBOX_MAX_EMAIL_NUMBER);
                    }
                    else
                    {
                        GUILIST_SetMaxSelectedItem(cur_ctrl_id, DRAFT_MAX_EMAIL_NUMBER);
                    }
                    GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[cur_index] = TRUE;
                }
            }
            break;
            
        case EMAIL_BOX_MARK_OPT_CANCEL_MARK:
            {
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_OUTBOX:
                    cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_SENTBOX:
                    cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_DRAFT:
                    cur_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                    break;
                default :
                    break;
                }
                
                cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
                if (s_email_box_globle_p->is_marked_ptr[cur_index])
                {
                    GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[cur_index] = FALSE;
                }

                mark_num = GUILIST_GetSelectedItemIndex(cur_ctrl_id, PNULL, 0);
                if (0 == mark_num )
                {
                    GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
                    GUIAPICTRL_Redraw(cur_ctrl_id);// 由于标记状态的改变，这里需要重画list控件
                }
            }
            break;
            
        case EMAIL_BOX_MARK_OPT_MARKALL:
            {
                uint32 box_vector_num = 0;
                
                box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_OUTBOX:
                    cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_SENTBOX:
                    cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_DRAFT:
                    cur_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                    break;
                default :
                    break;
                }
                
                GUILIST_SetTextListMarkable(cur_ctrl_id, TRUE);
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    GUILIST_SetMaxSelectedItem(cur_ctrl_id, INBOX_MAX_EMAIL_NUMBER);
                    break;
                default:
                    GUILIST_SetMaxSelectedItem(cur_ctrl_id, DRAFT_MAX_EMAIL_NUMBER);
                    break;
                }
                
                for (cur_index = 0; cur_index < box_vector_num; cur_index++)
                {
                    GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[cur_index] = TRUE;
                }
            }
            break;
            
        case EMAIL_BOX_MARK_OPT_CANCEL_MARKALL:
            {
                uint32 box_vector_num = 0;
                
                box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_OUTBOX:
                    cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_SENTBOX:
                    cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_DRAFT:
                    cur_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                    break;
                default :
                    break;
                }
#if !defined(MMI_PDA_SUPPORT)
                GUILIST_SetTextListMarkable(cur_ctrl_id, FALSE);
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    GUILIST_SetMaxSelectedItem(cur_ctrl_id, INBOX_MAX_EMAIL_NUMBER);
                    break;
                default:
                    GUILIST_SetMaxSelectedItem(cur_ctrl_id, DRAFT_MAX_EMAIL_NUMBER);
                    break;
                }
#endif
                for (cur_index = 0; cur_index < box_vector_num; cur_index++)
                {
                    GUILIST_SetSelectedItem(cur_ctrl_id, cur_index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[cur_index] = FALSE;
                }
            }
            break;
            
        case EMAIL_BOX_MARK_OPT_MARK_BY_TIME:
#ifdef MMI_PDA_SUPPORT
            MMK_CreatePubFormWin((uint32 *)EMAIL_MARK_BY_DATETIME_WIN_TAB, PNULL);
#else
            MMK_CreateWin((uint32*)EMAIL_MARK_BY_DATETIME_WIN_TAB, PNULL);
#endif            
            break;
            
        case EMAIL_INBOX_OPT_MARKASREAD:
        case EMAIL_INBOX_OPT_MARKASUNREAD:
            switch (s_email_box_globle_p->box_reason_type)
            {
            case MMIEMAIL_BOX_FROM_INBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_INBOX_LISTBOX_CTRL_ID, PNULL, 0);
                break;
            case MMIEMAIL_BOX_FROM_OUTBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_OUTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                break;
            case MMIEMAIL_BOX_FROM_SENTBOX:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_SENTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                break;
            case MMIEMAIL_BOX_FROM_DRAFT:
                mark_num = GUILIST_GetSelectedItemIndex(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, PNULL, 0);
                break;
            default :
                break;
            }
            
            if(0 == mark_num)
            {
                int     index_foucs = 0;
                uint32  email_status = 0;
                EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
                
                index_foucs = s_email_box_globle_p->selected_index;
                
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index_foucs, (void**)&email_header_ptr);
                EMA_UTILS_LOG_INFO(("HandleBoxMenuWinMsg email_header_ptr->mail_status = %d", email_header_ptr->mail_status));
                
                if (EMAIL_INBOX_OPT_MARKASREAD == menu_id)
                {
                    switch (email_header_ptr->mail_status)
                    {
                    case EMA_BASIC_UNREAD:
                        email_status = EMA_BASIC_READ;
                        break;
                    case EMA_BASIC_UNREAD_HEAD_ONLY:
                        email_status = EMA_BASIC_READ_HEAD_ONLY;
                        break;
                    case EMA_UNREAD_WITH_ATTACH:
                        email_status = EMA_READ_WITH_ATTACH;
                        break;
                    case EMA_UNREAD_AND_FORWARDED:
                        email_status = EMA_READ_AND_FORWARDED;
                        break;
                    case EMA_UNREAD_WITH_ATTACH_AND_FORWARDED:
                        email_status = EMA_READ_WITH_ATTACH_AND_FORWARDED;
                        break;
                    case EMA_UNREAD_AND_REPLIED:
                        email_status = EMA_READ_AND_REPLIED;
                        break;
                    case EMA_UNREAD_WITH_ATTACH_AND_REPLIED:
                        email_status = EMA_READ_WITH_ATTACH_AND_REPLIED;
                        break;
                    default :
                        email_status = email_header_ptr->mail_status;
                        break;
                    }
                } 
                else if (EMAIL_INBOX_OPT_MARKASUNREAD == menu_id)
                {
                    switch (email_header_ptr->mail_status)
                    {
                    case EMA_BASIC_READ:
                        email_status = EMA_BASIC_UNREAD;
                        break;
                    case EMA_BASIC_READ_HEAD_ONLY:
                        email_status = EMA_BASIC_UNREAD_HEAD_ONLY;
                        break;
                    case EMA_READ_WITH_ATTACH:
                        email_status = EMA_UNREAD_WITH_ATTACH;
                        break;
                    case EMA_READ_AND_FORWARDED:
                        email_status = EMA_UNREAD_AND_FORWARDED;
                        break;
                    case EMA_READ_WITH_ATTACH_AND_FORWARDED:
                        email_status = EMA_UNREAD_WITH_ATTACH_AND_FORWARDED;
                        break;
                    case EMA_READ_AND_REPLIED:
                        email_status = EMA_UNREAD_AND_REPLIED;
                        break;
                    case EMA_READ_WITH_ATTACH_AND_REPLIED:
                        email_status = EMA_UNREAD_WITH_ATTACH_AND_REPLIED;
                        break;
                    default:
                        email_status = email_header_ptr->mail_status;
                        break;
                    }
                }
                
                if(!InBoxProc_UpdateEmailStatus((uint16)email_status, s_email_box_globle_p->email_id_ptr[index_foucs]))
                {
                    EMA_UTILS_LOG_ERROR(("HandleBoxMenuWinMsg InBoxProc_UpdateEmailStatus failed"));
                }
                
                MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            }
            else
            {
                MMI_STRING_T         wait_str = {0};
                
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
                
                MMIPUB_OpenWaitWin(1, &wait_str, PNULL, PNULL, EMAIL_WAITING_WIN_ID, IMAGE_NULL,
                    ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleWaitWinMsg);
                
                if (EMAIL_INBOX_OPT_MARKASUNREAD == menu_id)
                {
                    MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_MARKASUNREAD, PNULL, 0);
                } 
                else if (EMAIL_INBOX_OPT_MARKASREAD == menu_id)
                {
                    MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_MARKASREAD, PNULL, 0);
                }

#if defined(MMI_PDA_SUPPORT)
                MMK_PostMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_MARK_CANCEL, PNULL, 0);
#endif
                MMK_CloseWin(win_id);
            }
            break;
            
        case EMAIL_OUTBOX_OPT_RESEND:
            {
                uint32 sentbox_email_num = 0;
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_SENTBOX, (uint32)&sentbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get sentbox's email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (SENTBOX_MAX_EMAIL_NUMBER <= sentbox_email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_SENTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                eml_msg_id = s_email_box_globle_p->email_id;
                MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
                MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), eml_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
                //if (PNULL == &s_email_box_globle_p->msg_datacache)/*永远为FALSE*/
                //{
                //    MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                //   
                //    MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
                //}
                //else
                MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FROMADDR, &s_email_box_globle_p->email_datacache);
                {
                    MMIEMAIL_GetAllRecipients(PNULL, &s_email_box_globle_p->email_datacache);
                    g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                    MMIEMAIL_SelectNet();
                }
            }
            break;
            
        case EMAIL_OUTBOX_OPT_MOVETODRAFT:
            mark_num = GUILIST_GetSelectedItemIndex(EMAIL_OUTBOX_LISTBOX_CTRL_ID, PNULL,0);
            if(0 == mark_num)
            {
                if(!OutBoxProc_MovetoDraft(s_email_box_globle_p->email_id))
                {
                    MMIPUB_OpenAlertWarningWin( TXT_EMAIL_MOVE_FAILED);
                    EMA_UTILS_LOG_ERROR(("HandleBoxMenuWinMsg OutBoxProc_MovetoDraft"));
                }
                else
                {
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMIPUB_OpenAlertSuccessWin( TXT_EMAIL_MOVE_SUCCESS);
#endif
                }
                
                MMK_SendMsg(EMAIL_OUTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            }
            else
            {
                MMI_STRING_T  wait_str = {0};
                
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_str);
                
                MMIPUB_OpenWaitWin(1,&wait_str,PNULL,PNULL,EMAIL_WAITING_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWaitWinMsg);
                
                MMK_PostMsg(EMAIL_OUTBOX_CHILD_WIN_ID, EVT_EMA_MOVETODRAFT, PNULL, 0);

                MMK_CloseWin(win_id);
            }
            break;
            
        case EMAIL_SENTBOX_OPT_RESEND:
            {
                uint32 outbox_email_num = 0;
                uint32 sentbox_email_num = 0;
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_OUTBOX, (uint32)&outbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get outbox's email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (OUTBOX_MAX_EMAIL_NUMBER <= outbox_email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_OUTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_SENTBOX, (uint32)&sentbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get sentbox's email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (SENTBOX_MAX_EMAIL_NUMBER <= sentbox_email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_SENTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                GUILIST_SetTextListMarkable(EMAIL_SENTBOX_LISTBOX_CTRL_ID, FALSE);
                eml_msg_id = s_email_box_globle_p->email_id;
                MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
                ret = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), eml_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
                if(EMAIL_SUCCESS != ret)
                {
                    EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, get email data failed");
                    break;
                }
                s_email_box_globle_p->email_datacache.email_id = 0;
                MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FROMADDR, &s_email_box_globle_p->email_datacache);
                if(TRUE == MMIEMAIL_EditProcSendEmail((uint32)&s_email_box_globle_p->email_datacache))
                {
                    MMIEMAIL_GetAllRecipients(PNULL, &s_email_box_globle_p->email_datacache);
                    g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                    MMIEMAIL_SelectNet();
                }
                else
                {
                    EMA_UTILS_TRACE("EMAIL_SENTBOX_OPT_RESEND, call MMIEMAIL_EditProcSendEmail failed");
                }
            }
            break;
            
        case EMAIL_DRAFTBOX_OPT_SEND:
            {
                uint32 outbox_email_num = 0;
                uint32 sentbox_email_num = 0;
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_OUTBOX, (uint32)&outbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get outbox's email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (OUTBOX_MAX_EMAIL_NUMBER <= outbox_email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_OUTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                if (!MMIEMAIL_GetEmailNum((uint32)EMA_SENTBOX, (uint32)&sentbox_email_num))
                {
                    EMA_UTILS_LOG_ERROR(("MMIEMAIL_EDIT_MENU_SEND, get sentbox's email num failed!!!"));
                    MMK_CloseWin(win_id);
                    return TRUE;
                }
                
                if (SENTBOX_MAX_EMAIL_NUMBER <= sentbox_email_num)
                {
                    MMK_CloseWin(win_id);
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_EMAIL_SENTBOX_FULL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
                    return TRUE;
                }
                
                eml_msg_id = s_email_box_globle_p->email_id;
                MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
                MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), eml_msg_id, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
                MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_FROMADDR, &s_email_box_globle_p->email_datacache);
                {
                    if (FALSE == BoxCheckDatecach(&s_email_box_globle_p->email_datacache))
                    {
                    }
                    else
                    {
                        MMIEMAIL_GetAllRecipients(PNULL, &s_email_box_globle_p->email_datacache);
                        g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
                        MMIEMAIL_SelectNet();
                    }
                }
            }
            break;
            
        case EMAIL_INBOX_SORT_OPT_REC_NEW:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_REC_NEW;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_REC_OLD:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_REC_OLD;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_SIZE_LARGE:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_SIZE_LARGE;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_SIZE_SMALL:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_SIZE_SMALL;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_PRO_HIGH:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_PRO_HIGH;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_PRO_LOW:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_PRO_LOW;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_SENDER:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_SENDER;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_INBOX_SORT_OPT_SUBJECT:
            GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_SUBJECT;
            inbox_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_INBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
        case EMAIL_OUTBOX_SORT_OPT_DATE:
            GUILIST_SetTextListMarkable(EMAIL_OUTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_DATE;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_OUTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
        case EMAIL_OUTBOX_SORT_OPT_RECV:
            GUILIST_SetTextListMarkable(EMAIL_OUTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_RECEIVER;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_OUTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;  
        case EMAIL_SENTBOX_SORT_OPT_DATE:
            GUILIST_SetTextListMarkable(EMAIL_SENTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_DATE;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_SENTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_SENTBOX_SORT_OPT_RECV:
            GUILIST_SetTextListMarkable(EMAIL_SENTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_RECEIVER;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_SENTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_DRAFTBOX_SORT_OPT_DATE:
            GUILIST_SetTextListMarkable(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_DATE;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_DRAFTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_DRAFTBOX_SORT_OPT_RECV:
            GUILIST_SetTextListMarkable(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_RECEIVER;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_DRAFTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;
            
        case EMAIL_DRAFTBOX_SORT_OPT_SUBJECT:
            GUILIST_SetTextListMarkable(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, FALSE);
            s_email_box_globle_p->sort_type = EMA_BOXS_ORDERBY_SUBJECT;
            other_box_sort_type = s_email_box_globle_p->sort_type;
            MMK_SendMsg(EMAIL_DRAFTBOX_CHILD_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            break;

#if defined(MMI_PDA_SUPPORT)
        case EMAIL_BOX_OPT_OPEN:
            if(MMIEMAIL_BOX_FROM_DRAFT == s_email_box_globle_p->box_reason_type)
            {
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_DRAFT, (void*)s_email_box_globle_p->email_id);
            }
            else
            {
                ViewProcEnter(s_email_box_globle_p->email_id);
            }
            break;
            
        case EMAIL_BOX_OPT_MARK:
            {
                MMI_CTRL_ID_T list_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                MMI_CTRL_ID_T form_ctrl_id = MMIEMAIL_INBOX_FORM_CTRL_ID;
                
                MMK_CloseWin(win_id);
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    list_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
                    form_ctrl_id = MMIEMAIL_INBOX_FORM_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_OUTBOX:
                    list_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
                    form_ctrl_id = MMIEMAIL_OUTBOX_FORM_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_SENTBOX:
                    list_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
                    form_ctrl_id = MMIEMAIL_SENTBOX_FORM_CTRL_ID;
                    break;
                case MMIEMAIL_BOX_FROM_DRAFT:
                    list_ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
                    form_ctrl_id = MMIEMAIL_DRAFTBOX_FORM_CTRL_ID;
                    break;
                default :
                    break;
                }
                
                switch (s_email_box_globle_p->box_reason_type)
                {
                case MMIEMAIL_BOX_FROM_INBOX:
                    GUILIST_SetMaxSelectedItem(list_ctrl_id, INBOX_MAX_EMAIL_NUMBER);
                    break;
                default:
                    GUILIST_SetMaxSelectedItem(list_ctrl_id, DRAFT_MAX_EMAIL_NUMBER);
                    break;
                }
                
                GUILIST_SetTextListMarkable(list_ctrl_id, TRUE);
                GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);        
                GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
            }
            break;
#endif
        default :
            break;
      }
      MMK_CloseWin(win_id);
      break;
      
   case MSG_PROMPTWIN_OK:
       MMIPUB_CloseQuerytWin(PNULL);
       MMK_CloseWin(win_id);
       break;
       
   case MSG_PROMPTWIN_CANCEL:
       MMIPUB_CloseQuerytWin(PNULL);
       break;
       
   case MSG_APP_CANCEL:
   case MSG_CTL_CANCEL:
       MMK_CloseWin(win_id);
       break;
       
   case MSG_CLOSE_WINDOW:
       break;
       
   default :
       err_code = MMI_RESULT_FALSE;
       break;
   }
   
   return err_code;
}

/**************************************************************************************/
// Description : email box exit
// Global resource dependence : none
// Author: 
// Note:
/**************************************************************************************/
LOCAL void BoxExit(void)
{
    if (PNULL != s_email_box_globle_p)
    {
        MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->is_marked_ptr);
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p);
    }
}

/**************************************************************************************/
// Description : to handle box window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleBoxWinMsg(
                                   EMAIL_WINDOW_ID_E win_id, 
                                   MMI_MESSAGE_ID_E msg_id, 
                                   DPARAM param
                                   )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_WIN_ID_T child_win_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIEMAIL_MSGBOX_TAB_CTRL_ID);
        AddTabsAndChildWindows();
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case EVT_EMA_UPDATE_BOX:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_UPDATE_BOX, PNULL);
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_TAB:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        break;
        
    case EVT_EMA_RESEND:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_RESEND, param);
        break;
        
    case EVT_EMA_DELETE_EML:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_DELETE_EML, PNULL);
        break;
        
    case EVT_EMA_DELETEALL_RESULT:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_DELETEALL_RESULT, PNULL);
        break;
        
    case EVT_EMA_MARKED_BY_DATE:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMA_MARKED_BY_DATE, param);
        break;
        
    case EVT_EMS_UPDATE_STATE_RESULT:
        child_win_id = GetCurFocusChildBoxWinId();
        MMK_SendMsg(child_win_id, EVT_EMS_UPDATE_STATE_RESULT, param);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_CLOSE_WINDOW:
        MMK_CloseParentWin(win_id);
        BoxExit();
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}

/**************************************************************************************/
// Description : Get box's Icon imageID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL uint32 GetBoxImageID(uint32 email_state)
{
    uint32 res_id = 0;
    
    switch(email_state)
    {
    case EMA_BASIC_UNREAD:
        res_id = IMAGE_EMAIL_UNREAD;//IDI_EMAIL_UNREAD;
        break;
    case EMA_BASIC_UNREAD_HEAD_ONLY:
        res_id = IMAGE_EMAIL_UNREAD_ONLY_WITH_HEADERS;//IDI_EMAIL_UNREAD_ONLY_WITH_HEADERS;
        break;
    case EMA_UNREAD_WITH_ATTACH:
        res_id = IMAGE_EMAIL_UNREAD_WITH_ATTACHMENT;//IDI_EMAIL_UNREAD_WITH_ATTACHMENT;
        break;
    case EMA_UNREAD_AND_FORWARDED:
        res_id = IMAGE_EMAIL_UNREAD_FORWARDED;//IDI_EMAIL_UNREAD_FORWARDED;
        break;
    case EMA_UNREAD_WITH_ATTACH_AND_FORWARDED:
        res_id = IMAGE_EMAIL_UNREAD_WITH_ATTACHEMNT_FORWARDED;//IDI_EMAIL_UNREAD_WITH_ATTACHEMNT_FORWARDED;
        break;
    case EMA_UNREAD_AND_REPLIED:
        res_id = IMAGE_EMAIL_UNREAD_REPLIED;//IDI_EMAIL_UNREAD_REPLIED;
        break;
    case EMA_UNREAD_WITH_ATTACH_AND_REPLIED:
        res_id = IMAGE_EMAIL_UNREAD_WITH_ATTACHEMNT_REPLIED;//IDI_EMAIL_UNREAD_WITH_ATTACHEMNT_REPLIED;
        break;
    case EMA_BASIC_READ:
        res_id = IMAGE_EMAIL_READ;//IDI_EMAIL_READ;
        break;
    case EMA_BASIC_READ_HEAD_ONLY:
        res_id = IMAGE_EMAIL_READ_ONLY_WITH_HEADERS;//IDI_EMAIL_READ_ONLY_WITH_HEADERS;
        break;
    case EMA_READ_WITH_ATTACH:
        res_id = IMAGE_EMAIL_READ_WITH_ATTACHMENT;//IDI_EMAIL_READ_WITH_ATTACHMENT;
        break;
    case EMA_READ_AND_FORWARDED:
        res_id = IMAGE_EMAIL_READ_FORWARDED;//IDI_EMAIL_READ_FORWARDED;
        break;
    case EMA_READ_WITH_ATTACH_AND_FORWARDED:
        res_id = IMAGE_EMAIL_READ_WITH_ATTACHEMNT_FORWARDED;//IDI_EMAIL_READ_WITH_ATTACHEMNT_FORWARDED;
        break;
    case EMA_READ_AND_REPLIED:
        res_id = IMAGE_EMAIL_READ_REPLIED;//IDI_EMAIL_READ_REPLIED;
        break;
    case EMA_READ_WITH_ATTACH_AND_REPLIED:
        res_id = IMAGE_EMAIL_READ_WITH_ATTACHEMNT_REPLIED;//IDI_EMAIL_READ_WITH_ATTACHEMNT_REPLIED;
        break;
    case EMA_SEND_FAILED:
        res_id = IMAGE_EMAIL_SENT_FAILED;//IDI_EMAIL_SENT_FAILED;
        break;
    case EMA_SEND_FAILED_WITH_ATTACH:
        res_id = IMAGE_EMAIL_SENT_FAILED_WITH_ATTACHMENT;//IDI_EMAIL_SENT_FAILED_WITH_ATTACHMENT;
        break;
    case EMA_SENT:
        res_id = IMAGE_EMAIL_SENT_SUCCESSFUL;//IDI_EMAIL_SENT_SUCCESSFUL;
        break;
    case EMA_SENT_WITH_ATTACH:
        res_id = IMAGE_EMAIL_SENT_SUCCESSFUL_WITH_ATTACHEMNT;//IDI_EMAIL_SENT_SUCCESSFUL_WITH_ATTACHEMNT;
        break;
    case EMA_BASIC_DRAFT:
        res_id = IMAGE_EMAIL_BASIC_DRAFT;//IDI_EMAIL_BASIC_DRAFT;
        break;
    case EMA_BASIC_DRAFT_WITH_ATTACH:
        res_id = IMAGE_EMAIL_BASIC_DRAFT_WITH_ATTACTHMENT;//IDI_EMAIL_BASIC_DRAFT_WITH_ATTACTHMENT;
        break;
    default:
        EMA_UTILS_LOG_ERROR(("email status failed."));
        res_id = IMAGE_EMAIL_BASIC_DRAFT;//IDI_EMAIL_BASIC_DRAFT;
        break;
    }
    
    return res_id;
}

/**************************************************************************************/
// Description : WstrCat2Dup
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL wchar *WstrCat2Dup(wchar *wstr_a_ptr, wchar *wstr_b_ptr, void *mem_hdr_ptr)
{
    uint32 wstr_len = 0;
    wchar *wstr_c_ptr = PNULL;
    
    if (PNULL != wstr_a_ptr)
    {
        wstr_len += EMA_UTILS_WSTRLEN(wstr_a_ptr);
    }
    if (PNULL != wstr_b_ptr)
    {
        wstr_len += EMA_UTILS_WSTRLEN((wchar *) L" ");
        wstr_len += EMA_UTILS_WSTRLEN(wstr_b_ptr);
    }
    
    wstr_c_ptr = EMA_UTILS_MALLOC(mem_hdr_ptr, wstr_len * 2 + 2);
    if (PNULL == wstr_c_ptr)
    {
        EMA_UTILS_LOG_ERROR(("WstrCat2, wstr_c_ptr is PNULL."));
        return PNULL;
    }
    EMA_UTILS_MEMSET(wstr_c_ptr, 0, wstr_len * 2 + 2);
    
    if (PNULL != wstr_a_ptr)
    {
        EMA_UTILS_WSTRCAT(wstr_c_ptr, wstr_a_ptr);
    }
    if (PNULL != wstr_b_ptr)
    {
        EMA_UTILS_WSTRCAT(wstr_c_ptr,(wchar *) L" ");
        EMA_UTILS_WSTRCAT(wstr_c_ptr, wstr_b_ptr);
    }
    
    return wstr_c_ptr;
}

/**************************************************************************************/
// Description : Add items in email box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 BoxAddItems(EMAIL_BOXTYPE_E box_type)
{
    EMAIL_MSGHEAD_T   *email_header_ptr = PNULL;
    uint32            box_vector_num    = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
    wchar             *date_ptr         = PNULL;
    wchar             *name_ptr         = PNULL;
    wchar             *line1_text_ptr   = PNULL;
    wchar             *line2_text_ptr   = PNULL;
    uint32            num               = 0;
    GUILIST_ITEM_T      item_t          = {0};
    GUILIST_ITEM_DATA_T item_data       = {0};
    EMAIL_VECTOR_T      *box_vector_ptr = s_email_box_globle_p->box_vector_ptr;
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;
    item_t.item_data_ptr = &item_data;
    
    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_COMM_OPEN;
    item_data.softkey_id[2] = STXT_RETURN;
    
    for (num = 0; num < box_vector_num; num++)
    {
        MMIEMAIL_VectorGetAt(box_vector_ptr, num, (void **)&email_header_ptr);
        if (PNULL == email_header_ptr)
        {
            EMA_UTILS_LOG_ERROR(("EMAFmMgr_BoxVectorAdd, email_header_ptr is PNULL."));
            return EMAIL_ERROR;
        }
        
        s_email_box_globle_p->email_id_ptr[num] = email_header_ptr->email_id;
        
        if (EMA_TIME == MMIEMAIL_IsToday(email_header_ptr->dwdate))
        {
            date_ptr = MMIEMAIL_GetDateTime(PNULL, email_header_ptr->dwdate, EMA_TIME);
        }
        else if (EMA_DATE_MD == MMIEMAIL_IsToday(email_header_ptr->dwdate))
        {
            date_ptr = MMIEMAIL_GetDateTime(PNULL, email_header_ptr->dwdate, EMA_DATE_MD);
        }
        else if (EMA_DATE_YMD == MMIEMAIL_IsToday(email_header_ptr->dwdate))
        {
            date_ptr = MMIEMAIL_GetDateTime(PNULL, email_header_ptr->dwdate, EMA_DATE_YMD);
        }

        if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
        {
            if (!EMA_UTILS_ISNULLWSTR(email_header_ptr->wsz_from_ptr))
            {
                //EMA_GetNameFromPhoneBookEX(s_email_box_globle_p->pStFmMgr->piShell,email_header_ptr->pwszFrom,&name_ptr);
            }
            if (PNULL == name_ptr)
            {
                name_ptr = EMA_UTILS_WSTRDUP(PNULL, email_header_ptr->wsz_from_ptr);
            }
            line1_text_ptr = WstrCat2Dup(name_ptr, date_ptr, PNULL);
        }
        else
        {
            if (PNULL == name_ptr)
            {
                name_ptr = EMA_UTILS_WSTRDUP(PNULL, email_header_ptr->wsz_to_ptr);
            }
            line1_text_ptr = WstrCat2Dup(name_ptr, date_ptr, PNULL);
        }
        
        if (PNULL != email_header_ptr->wsz_subject_ptr)
        {
             line2_text_ptr = EMA_UTILS_WSTRDUP(PNULL, email_header_ptr->wsz_subject_ptr);
        }
        
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[0].item_data.image_id = GetBoxImageID(email_header_ptr->mail_status);
        
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[1].item_data.text_buffer.wstr_ptr = line1_text_ptr;
        item_data.item_content[1].item_data.text_buffer.wstr_len = EMA_UTILS_WSTRLEN(line1_text_ptr);
        
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = line2_text_ptr;
        item_data.item_content[2].item_data.text_buffer.wstr_len = EMA_UTILS_WSTRLEN(line2_text_ptr);
        
        switch (box_type)
        {
        case EMA_INBOX:
            GUILIST_AppendItem(EMAIL_INBOX_LISTBOX_CTRL_ID, &item_t);
            break;
        case EMA_SENTBOX:
            GUILIST_AppendItem(EMAIL_SENTBOX_LISTBOX_CTRL_ID, &item_t);
            break;
        case EMA_OUTBOX:
            GUILIST_AppendItem(EMAIL_OUTBOX_LISTBOX_CTRL_ID, &item_t);
            break;
        case EMA_DRAFTBOX:
            GUILIST_AppendItem(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, &item_t);
            break;
        default :
            break;
        }
        
        EMA_UTILS_FREEIF(PNULL, date_ptr);
        EMA_UTILS_FREEIF(PNULL, name_ptr);
        EMA_UTILS_FREEIF(PNULL, line1_text_ptr);
        EMA_UTILS_FREEIF(PNULL, line2_text_ptr);
    }
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : BoxItemVectorFree
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void BoxItemVectorFree(void *data_ptr)
{
    EMAIL_MSGHEAD_T *email_header_ptr = (EMAIL_MSGHEAD_T *)data_ptr;
    
    if (PNULL != email_header_ptr)
    {
        EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, email_header_ptr->wsz_from_ptr);
        EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, email_header_ptr->wsz_to_ptr);
        EMA_UTILS_FREEIF(EMA_UTILS_MEMMGR_FOR_FREE, email_header_ptr->wsz_subject_ptr);
        EMA_UTILS_FREE(EMA_UTILS_MEMMGR_FOR_FREE, email_header_ptr);
    }
}

/**************************************************************************************/
// Description : Update the box's date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void UpdateBoxData(EMAIL_BOXTYPE_E box_type)
{
    int     result       = EMAIL_SUCCESS;
    uint32  vector_num   = 0;
    MMI_WIN_ID_T win_id  = 0;
    
    do
    {
        switch(box_type)
        {
        case EMA_INBOX:
            win_id = EMAIL_INBOX_CHILD_WIN_ID;
            break;
        case EMA_OUTBOX:
            win_id = EMAIL_OUTBOX_CHILD_WIN_ID;
            break;
        case EMA_SENTBOX:
            win_id = EMAIL_SENTBOX_CHILD_WIN_ID;
            break;
        case EMA_DRAFTBOX:
            win_id = EMAIL_DRAFTBOX_CHILD_WIN_ID;
            break;
        default:
            break;
        }
        
        if (PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
            s_email_box_globle_p->box_vector_ptr = PNULL;
        }
        
        result = MMIEMAIL_SP_Store_GetBoxInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, box_type, &s_email_box_globle_p->box_vector_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_InBoxProc_Enter, MMIEMAIL_SP_Store_GetBoxInfo failed.");
        
        BoxMgrSortBy(s_email_box_globle_p->sort_type);
        
        if (PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            MMIEMAIL_VectorSetPfnFree(s_email_box_globle_p->box_vector_ptr, BoxItemVectorFree);
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
        }
        
        if (vector_num > 0)
        {
            EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->email_id_ptr);
            s_email_box_globle_p->email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, vector_num * sizeof(uint32));
            EMA_UTILS_BREAKNULL(s_email_box_globle_p->email_id_ptr, result, "EmailAFmMgr_BoxMgr_New, s_email_box_globle_p->email_id_ptr is PNULL.");
            EMA_UTILS_MEMSET(s_email_box_globle_p->email_id_ptr, 0, vector_num * sizeof(uint32));
            
            EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->is_marked_ptr);
            s_email_box_globle_p->is_marked_ptr = (BOOLEAN *)EMA_UTILS_MALLOC(PNULL, vector_num * sizeof(BOOLEAN));
            EMA_UTILS_BREAKNULL(s_email_box_globle_p->is_marked_ptr, result, "UpdateBoxData, s_email_box_globle_p->email_id_ptr is PNULL.");
            EMA_UTILS_MEMSET(s_email_box_globle_p->is_marked_ptr, 0, vector_num * sizeof(BOOLEAN));
            
            BoxAddItems(box_type);
        }
        else
        {
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_NULL_TAB, PNULL);
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
        }
    } while(0);
    
    return;
}

/**************************************************************************************/
// Description : Create the email box.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_CreateBoxWin(MMIEMAIL_BOX_REASON_TYPE_E box_type, void *box_data_ptr)
{
    EMAIL_BOXTYPE_E ema_box_type = EMA_INBOX;

    EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p);
    s_email_box_globle_p = EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_BOX_GLOBLE_VARIABLE_T));
    if(PNULL == s_email_box_globle_p){
      return;
    }
    s_email_box_globle_p->box_reason_type = box_type;
    
    MMK_CreateParentWin((uint32 *)EMAIL_BOX_WIN_TAB, PNULL);
    MMK_CreateChildWin(EMAIL_BOX_WIN_ID, (uint32 *)EMAIL_INBOX_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(EMAIL_BOX_WIN_ID, (uint32 *)EMAIL_OUTBOX_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(EMAIL_BOX_WIN_ID, (uint32 *)EMAIL_DRAFTBOX_CHILD_WIN_TAB, NULL);
    MMK_CreateChildWin(EMAIL_BOX_WIN_ID, (uint32 *)EMAIL_SENTBOX_CHILD_WIN_TAB, NULL);
    
    switch(s_email_box_globle_p->box_reason_type)
    {
    case MMIEMAIL_BOX_FROM_INBOX:
        MMK_SetChildWinFocus(EMAIL_BOX_WIN_ID, EMAIL_INBOX_CHILD_WIN_ID);
        ema_box_type = EMA_INBOX;
        break;
    case MMIEMAIL_BOX_FROM_SENTBOX:
        MMK_SetChildWinFocus(EMAIL_BOX_WIN_ID, EMAIL_SENTBOX_CHILD_WIN_ID);
        ema_box_type = EMA_SENTBOX;
        break;
    case MMIEMAIL_BOX_FROM_OUTBOX:
        MMK_SetChildWinFocus(EMAIL_BOX_WIN_ID, EMAIL_OUTBOX_CHILD_WIN_ID);
        ema_box_type = EMA_OUTBOX;
        break;
    case MMIEMAIL_BOX_FROM_DRAFT:
        MMK_SetChildWinFocus(EMAIL_BOX_WIN_ID, EMAIL_DRAFTBOX_CHILD_WIN_ID);
        ema_box_type = EMA_DRAFTBOX;
        break;
    default:
        break;
    }
    
    SetCurType(ema_box_type);
    return;
}

/**************************************************************************************/
// Description : handle to delete dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC MMI_RESULT_E MMIEMAIL_DelDlg_HandleEvent(
                                      MMI_WIN_ID_T win_id,
                                      MMI_MESSAGE_ID_E msg_id,
                                      DPARAM param
                                      )
{
    MMI_RESULT_E  result   = MMI_RESULT_TRUE;
    int32         ret      = EMAIL_SUCCESS;
    int           mark_num = 0;
    MMI_WIN_ID_T  win_pid  = EMAIL_INBOX_CHILD_WIN_ID;
    MMI_CTRL_ID_T ctr_pid  = 0;
    
    switch (s_email_box_globle_p->box_reason_type)
    {
    case MMIEMAIL_BOX_FROM_INBOX:
        ctr_pid = EMAIL_INBOX_LISTBOX_CTRL_ID;
        win_pid = EMAIL_INBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_OUTBOX:
        ctr_pid = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
        win_pid = EMAIL_OUTBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_SENTBOX:
        ctr_pid = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
        win_pid = EMAIL_SENTBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_DRAFT:
        ctr_pid = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
        win_pid = EMAIL_DRAFTBOX_CHILD_WIN_ID;
        break;
    default :
        break;
    }
    
    switch(msg_id)
    {
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        switch (win_id)
        {
        case EMAIL_DELETE_DLG_ONE:
            mark_num = GUILIST_GetSelectedItemIndex(ctr_pid, PNULL, 0);
            if(0 == mark_num)
            {
                MMK_SendMsg(win_pid, EVT_EMA_DELETE_EML, PNULL);
            }
            else
            {
                uint32 vector_num = 0;
                uint32 i = 0;
                MMI_STRING_T   prompt_str = {0};
                
                if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                    s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                }
                MMIEMAIL_VectorNew(&s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                if (PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    result = MMI_RESULT_FALSE;
                    return result;
                }
                MMIEMAIL_VectorSetPfnFree(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, MarkEmailVectorFree);
                
                vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
                for (i = 0; i < vector_num; i++)
                {
                    if (s_email_box_globle_p->is_marked_ptr[i])
                    {
                        EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
                        uint32 *email_id_ptr = PNULL;
                        
                        email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, sizeof(uint32));
                        EMA_UTILS_BREAKNULL(email_id_ptr, ret, "MMIEMAIL_DelDlg_HandleEvent EMA_UTILS_MALLOC failed");
                        EMA_UTILS_MEMSET(email_id_ptr, 0x00, sizeof(uint32));
                        
                        MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, i, (void **)&email_header_ptr);
                        *email_id_ptr = email_header_ptr->email_id;
                        MMIEMAIL_VectorInsert(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, email_id_ptr);
                    }
                }
                
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
                MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,EMAIL_DELETEALL_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleWaitWinMsg);
                
                MMK_PostMsg(win_pid, EVT_EMA_DELETEMUL_TAB, PNULL, 0);
            }
            break;
        case EMAIL_DELETE_DLG_ALL:
            {
                MMI_STRING_T   prompt_str = {0};

                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
                MMIPUB_OpenWaitWin(1,&prompt_str,PNULL,PNULL,EMAIL_DELETEALL_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandleDelAllWaitWinMsg);
                
                MMK_PostMsg(win_pid, EVT_EMA_DELETEALL_TAB, PNULL, 0);
            }
            break;
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    
    return result;
}

/**************************************************************************************/
// Description : delete email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN PublicProcDelEmail(uint32 email_id)
{
    int result = EMAIL_SUCCESS;
    
    do
    {
        result = MMIEMAIL_SP_Store_DeleteEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_id);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, MMIEMAIL_SP_Store_DeleteEmail failed.");
        
        /*Mark deleted on server*/
        if (E_EMAILA_MAILSERV_IMAP4 == MMIEMAIL_SP_CFG_ActRecType(EMA_GETCFG_HANDLE(g_email_sp_handle)))
        {
            EMAIL_ACT_MSGPREFERENCE_T *email_pref_ptr = PNULL;
            BOOLEAN is_del_onserv = FALSE;

            email_pref_ptr = (EMAIL_ACT_MSGPREFERENCE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
            EMA_UTILS_BREAKNULL(email_pref_ptr, result, "EmailAUIC_PublicProc_DelEmail, email_pref_ptr is PNULL.");
            EMA_UTILS_MEMSET(email_pref_ptr, 0, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
            
            result = MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_pref_ptr);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, MMIEMAIL_SP_CFG_GetEmailPreference failed.");
            
            is_del_onserv = email_pref_ptr->is_del_onserv;
            
            result = MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, MMIEMAIL_SP_CFG_DEL_EmailPreference failed.");
            EMA_UTILS_FREE(PNULL, email_pref_ptr);
            
            if (TRUE == is_del_onserv)/*Mark deleted email on server.*/
            {
                result = MMIEMAIL_SP_TRS_Imap4Mark(EMA_GETTRANS_HANDLE(g_email_sp_handle), 
                    EMA_IMAP4_MARK_DEL_ON_SERVER,
                    EMA_SP_ACTIVE_ACCOUNT,
                    email_id);
                EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, MMIEMAIL_SP_TRS_Imap4Mark failed.");
            }
        }
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************************/
// Description : handle to waitting dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleWaitWinMsg(
                                    MMI_WIN_ID_T        win_id,    //IN:
                                    MMI_MESSAGE_ID_E    msg_id,    //IN:
                                    DPARAM              param      //IN:
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}


/**************************************************************************************/
// Description : handle to waitting dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleDelAllWaitWinMsg(
                                    MMI_WIN_ID_T        win_id,    //IN:
                                    MMI_MESSAGE_ID_E    msg_id,    //IN:
                                    DPARAM              param      //IN:
                                    )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        MMIEMAIL_SP_Store_CancelDeleteAllEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle));
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIEMAIL_SP_Store_CancelDeleteAllEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle));
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/**************************************************************************************/
// Description : handle to delete dialog.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleRecvTypeMenuWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = EMAIL_RECV_TYPE_MENU_CTRL_ID;
    MMI_MENU_ID_T menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    
    switch (msg_id)
    {
    case MSG_APP_RED:
        break;
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        EMA_UTILS_TRACE("MMIEmail_HandleEditEditMenuWinMsg msg_id = 0x%x", menu_id);
        switch (menu_id)
        {
        case EMAIL_RECV_TYPE_ONLYHEADER:
            {
                s_email_box_globle_p->trans_type = MMIEMAIL_TRANS_MSGHEAD;
                g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
                MMIEMAIL_SelectNet();
            }
            break;
        case EMAIL_RECV_TYPE_HEADER_BODY:
            {
                s_email_box_globle_p->trans_type = MMIEMAIL_TRANS_MSG;
                g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
                MMIEMAIL_SelectNet();
            }
            break;
        default:
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return (recode);
}

/**************************************************************************************/
// Description : to retrieve email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_PublicProc_Retrieve(void)
{
    EMAIL_ACT_MSGPREFERENCE_T *email_pref_ptr= PNULL;
    HStoreMgr                 emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
    EMASP_PRFITEM_VAL         val = {0};
    uint32                    wstr_len = 0;
    
    if (PNULL == emasp_store_mgr)
    {
        return FALSE;
    }
    
    if (SUCCESS != MMIEMAIL_SP_Store_GetActCfgItem(emasp_store_mgr, EMA_SP_ACTIVE_ACCOUNT,EMASP_PRFITEM_ACCOUNT_NAME, &val))
    {
        EMA_UTILS_LOG_ERROR(("MMIEmail_InitLoadSettingData, get EMASP_PRFITEM_ACCOUNT_NAME failed."));
        return FALSE;
    }
    EMA_UTILS_MEMSET(g_email_receive_str, 0x00, EMA_MAX_ADDR_LEN * sizeof(wchar));
    wstr_len = EMA_UTILS_WSTRLEN(val.pData) * sizeof(wchar);
    EMA_UTILS_MEMCPY(g_email_receive_str, val.pData, wstr_len);
    
    /*Get download mode.*/
    email_pref_ptr = (EMAIL_ACT_MSGPREFERENCE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
    if (PNULL == email_pref_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, email_pref_ptr is PNULL."));
        return FALSE;
    }
    EMA_UTILS_MEMSET(email_pref_ptr, 0, sizeof(EMAIL_ACT_MSGPREFERENCE_T));
    
    if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_GetEmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_pref_ptr))
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, MMIEMAIL_SP_CFG_GetEmailPreference failed."));
        return FALSE;
    }
    
    if (EMA_DOWNLOAD_ONLY_HEADERS == email_pref_ptr->download_mode)
    {
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr))
        {
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
            return FALSE;
        }
        EMA_UTILS_FREE(PNULL, email_pref_ptr);
        //return EmailAUIC_PublicProc_RetrieveHead(pStUICtrlMgr, EMA_SP_ACTIVE_ACCOUNT, dwParam);
        
        g_email_trans_type = MMIEMAIL_TRANS_MSGHEAD;
        g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
        MMIEMAIL_SelectNet();
    }
    else if (EMA_DOWNLOAD_FULL_MESSAGE == email_pref_ptr->download_mode)
    {
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr))
        {
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
            return FALSE;
        }
        EMA_UTILS_FREE(PNULL, email_pref_ptr);
        //return EmailAUIC_PublicProc_RetrieveHeadBody(pStUICtrlMgr, EMA_SP_ACTIVE_ACCOUNT, dwParam);
        
        g_email_trans_type = MMIEMAIL_TRANS_MSG;
        g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
        MMIEMAIL_SelectNet();
    }
    else if (EMA_DOWNLOAD_ASK_USER == email_pref_ptr->download_mode)
    {
        /*      char *pszBoxName = PNULL;*/
        //       if (pszTempBoxName != PNULL)
        //       {
        //          pszBoxName = EMA_UTILS_STRDUP(pStUICtrlMgr->mem_hdr_ptr, pszTempBoxName);
        //       }
        if (EMAIL_SUCCESS != MMIEMAIL_SP_CFG_DEL_EmailPreference(EMA_GETCFG_HANDLE(g_email_sp_handle), email_pref_ptr))
        {
            EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, MMIEMAIL_SP_CFG_DEL_EmailPreference failed."));
            return FALSE;
        }
        EMA_UTILS_FREE(PNULL, email_pref_ptr);
        //       stEmaBoxNameActId.pszBoxName = pszBoxName;
        //       if (!EmailAUIC_ProcCreateForm(pStUICtrlMgr, FORMID_RETRIEVE_POP, (void *)&stEmaBoxNameActId))
        //       {
        //          EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_Retrieve, EmailAUIC_ProcCreateForm failed."));
        //          return FALSE;
        //       }
        
        MMK_CreateWin((uint32*)EMAIL_RECV_TYPE_MENU_WIN_TAB, PNULL);
        
        return TRUE;
    }
    
    return TRUE;
}

/**************************************************************************************/
// Description : edit email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN ComposerProcEdit(uint32 email_id)
{
    int result = EMAIL_ERROR;
    EMAIL_MSGDATACACHE_T *date_chche_ptr = PNULL;
    
    do 
    {
        date_chche_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(date_chche_ptr, result, "EmailAUIC_ComposerProc_Edit, date_chche_ptr is PNULL.");
        EMA_UTILS_MEMSET(date_chche_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, date_chche_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_ComposerProc_Edit, MMIEMAIL_SP_Store_GetEmailData failed.");
        
        MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_DRAFT, (void *)date_chche_ptr);
        
    } while(0);
    
    if (EMAIL_SUCCESS != result)
    {
        MMIEMAIL_ReleaseEmailDataCache(PNULL, date_chche_ptr);
        return FALSE;
    }
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, date_chche_ptr);
    return TRUE;
}

/**************************************************************************************/
// Description : to view email by msgID.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN ViewProcEnter(uint32 email_id)
{
    int              result = EMAIL_ERROR;
    EMAIL_MSGDATACACHE_T *date_chche_ptr = PNULL;   
    
    do 
    {
        date_chche_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(date_chche_ptr, result, "EmailAUIC_ViewProc_Enter, date_chche_ptr is PNULL.");
        EMA_UTILS_MEMSET(date_chche_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, date_chche_ptr);
        //EMA_UTILS_BREAKIF(result, "EmailAUIC_BoxProc_Enter, MMIEMAIL_SP_Store_GetEmailData failed.");
        //EMA_UTILS_BREAKNULL(date_chche_ptr, result, "EmailAUIC_InBoxProc_Enter, date_chche_ptr is PNULL.");
        if (EMAIL_SUCCESS != result)
        {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            return FALSE;
        }//add by wuhui
        
        result = MMIEMAIL_SP_Store_GetViewEmlType(EMA_GETSTORE_HANDLE(g_email_sp_handle),
            email_id,
            EMA_SP_ACTIVE_ACCOUNT,
            &date_chche_ptr->view_type, &date_chche_ptr->source_buff_ptr);
        EMA_UTILS_BREAKIF(result, "EmailAUIC_InBoxProc_Enter, MMIEMAIL_SP_Store_GetViewEmlType failed.");
        
        switch(s_email_box_globle_p->box_reason_type)
        {
        case MMIEMAIL_BOX_FROM_OUTBOX:
            MMIEMAIL_CreateViewWin(MMIEMAIL_VIEW_FROM_OUTBOX, (void *)date_chche_ptr);
            break;
        case MMIEMAIL_BOX_FROM_SENTBOX:
            MMIEMAIL_CreateViewWin(MMIEMAIL_VIEW_FROM_SENTBOX, (void *)date_chche_ptr);
            break;
        case MMIEMAIL_BOX_FROM_INBOX:
            MMIEMAIL_CreateViewWin(MMIEMAIL_VIEW_FROM_INBOX, (void *)date_chche_ptr);
            break;
        default:
            break;
        }      
        
        // Send read report when first view this email
        if ((EMA_BASIC_UNREAD == date_chche_ptr->mail_status || EMA_BASIC_UNREAD_HEAD_ONLY == date_chche_ptr->mail_status)
            && PNULL != date_chche_ptr->read_report_ptr
            && MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
        {
            result = MMIEMAIL_CopyEmailDataCache(&s_email_box_globle_p->email_datacache, date_chche_ptr, PNULL);
            EMA_UTILS_BREAKIF(result, "ViewProcEnter, MMIEMAIL_CopyEmailDataCache failed.");
            MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_READREPORTCONTENT, EMAIL_CONFIRM_READREPORT);
        }
        
        /*Update e-mail read state.*/
        if (EMA_BASIC_UNREAD == date_chche_ptr->mail_status
            || EMA_BASIC_UNREAD_HEAD_ONLY == date_chche_ptr->mail_status
            || EMA_UNREAD_WITH_ATTACH == date_chche_ptr->mail_status
            || EMA_UNREAD_AND_FORWARDED == date_chche_ptr->mail_status
            || EMA_UNREAD_WITH_ATTACH_AND_FORWARDED == date_chche_ptr->mail_status
            || EMA_UNREAD_AND_REPLIED == date_chche_ptr->mail_status
            || EMA_UNREAD_WITH_ATTACH_AND_REPLIED == date_chche_ptr->mail_status
            || EMA_SEND_FAILED == date_chche_ptr->mail_status
            || EMA_SEND_FAILED_WITH_ATTACH == date_chche_ptr->mail_status
            || EMA_SENT == date_chche_ptr->mail_status
            || EMA_SENT_WITH_ATTACH == date_chche_ptr->mail_status)
        {
            switch(date_chche_ptr->mail_status)
            {
            case EMA_BASIC_UNREAD:
                if (date_chche_ptr->attach_num > 0)
                {
                    date_chche_ptr->mail_status = EMA_READ_WITH_ATTACH;
                }
                else
                {
                    date_chche_ptr->mail_status = EMA_BASIC_READ;
                }
                break;
            case EMA_UNREAD_WITH_ATTACH:
                date_chche_ptr->mail_status = EMA_READ_WITH_ATTACH;
                break;
            case EMA_BASIC_UNREAD_HEAD_ONLY:
                date_chche_ptr->mail_status = EMA_BASIC_READ_HEAD_ONLY;
                break;
            case EMA_UNREAD_AND_FORWARDED:
                date_chche_ptr->mail_status = EMA_READ_AND_FORWARDED;
                break;
            case EMA_UNREAD_WITH_ATTACH_AND_FORWARDED:
                date_chche_ptr->mail_status = EMA_READ_WITH_ATTACH_AND_FORWARDED;
                break;
            case EMA_UNREAD_AND_REPLIED:
                date_chche_ptr->mail_status = EMA_READ_AND_REPLIED;
                break;
            case EMA_UNREAD_WITH_ATTACH_AND_REPLIED:
                date_chche_ptr->mail_status = EMA_READ_WITH_ATTACH_AND_REPLIED;
                break;
            case EMA_SEND_FAILED:
                date_chche_ptr->mail_status = EMA_SEND_FAILED_READ;
                break;
            case EMA_SEND_FAILED_WITH_ATTACH:
                date_chche_ptr->mail_status = EMA_SEND_FAILED_WITH_ATTACH_READ;
                break;
            case EMA_SENT:
                date_chche_ptr->mail_status = EMA_SENT_READ;
                break;
            case EMA_SENT_WITH_ATTACH:
                date_chche_ptr->mail_status = EMA_SENT_WITH_ATTACH_READ;
                break;
            default:
                break;
            }
            result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, date_chche_ptr);
            EMA_UTILS_BREAKIF(result, "EmailAUIC_InBoxProc_Enter, MMIEMAIL_SP_Store_UpdateEmail failed.");
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            
            /*Mark read on server*/
            // 		 if (E_EMAILA_MAILSERV_IMAP4 == EmailASP_CFG_ActRecType(EMA_GETCFG_HANDLE(pStUICtrlMgr->g_email_sp_handle)))
            // 			{
            // 				result = EmailASP_TRS_Imap4Mark(EMA_GETTRANS_HANDLE(g_email_sp_handle), 
            // 					EMA_IMAP4_MARK_SEEN_ON_SERVER,
            // 					EMA_SP_ACTIVE_ACCOUNT,
            // 					msg_id);
            // 				EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, EmailASP_TRS_Imap4Mark failed.");
            // 
            // 			}
        }     
    } while(0);
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, date_chche_ptr);
    
    if (EMAIL_SUCCESS != result)
    {
        return FALSE;
    }
    
    return TRUE;
}

/**************************************************************************************/
// Description : receive new email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void RecvNewEmail(void)
{
/*   
   MMI_RESULT_E   send_result = EMAIL_SUCCESS;
   BOOLEAN is_sim1_ok         = FALSE;
   BOOLEAN is_sim2_ok         = FALSE;
   uint32 record_id           = 0;
   int32  result              = EMAIL_SUCCESS;
*/

   if (MMK_IsOpenWin(EMAIL_RECVING_WIN_ID))
   {
      MMIPUB_OpenAlertWarningWin(TXT_COMMON_HIGH);
      return;
   }

   MMIEMAIL_PublicProc_Retrieve();
   
/*      
#ifdef MMI_DUALSIM_SUPPORT
   MMIAPIPHONE_GetDualSimStatus(&is_sim1_ok, &is_sim2_ok);
   
   if (is_sim1_ok && !is_sim2_ok)
   {
      s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_1;
      MMIEmail_PublicProc_Retrieve();
   }
   else if (!is_sim1_ok && is_sim2_ok)
   {
      s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_2;
      MMIEmail_PublicProc_Retrieve();
   }
   else if (is_sim1_ok && is_sim2_ok)
   {
      switch(MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_MMS))
      {
      case MMISET_MAJOR_SIM_SEL_SIM1:
         EMA_UTILS_TRACE("MMIEmail_EditAndSendNewEMAIL dual sim1");                        
         s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_1;
         MMIEmail_PublicProc_Retrieve();
         break;
      case MMISET_MAJOR_SIM_SEL_SIM2:
         EMA_UTILS_TRACE("MMIEmail_EditAndSendNewEMAIL dual sim2");                        
         s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_2;
         MMIEmail_PublicProc_Retrieve();
         break;
      case MMISET_MAJOR_SIM_SEL_MANUAL_SEL:
         MMK_CreateWin((uint32 *)EMAIL_RECV_SELECT_SIM_WIN_TAB, PNULL);
         return;
      default:
         break;
      }
   }
   else 
   {
      MMIAPIPHONE_AlertSimNorOKStatus();
   }
#else
   if (MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_1))
   {
      s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_1;
      MMIEmail_PublicProc_Retrieve();
   }
   else
   {
      MMIAPIPHONE_AlertSimNorOKStatus();
   }
#endif
*/
   
   return;
}

/**************************************************************************************/
// Description : handle to selete SIM to receive email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E HandleSimSelectRecvWinMsg(
                                             EMAIL_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{    
    MMI_RESULT_E  recode             = MMI_RESULT_TRUE;
    MMI_STRING_T  text_str           = {0};
    uint16        list_index         = 0;
    GUILIST_ITEM_T    listbox_item   = {0};    /*lint !e64*/  
    
    EMA_UTILS_MEMSET(&text_str, 0, sizeof(MMI_STRING_T));
    EMA_UTILS_MEMSET(&listbox_item, 0, sizeof(GUILIST_ITEM_T));
    EMA_UTILS_TRACE("HandleSimSelectRecvWinMsg msg_id = 0x%x", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(EMAIL_RECV_SELECT_SIM_CTRL_ID, MMI_DUAL_SYS_MAX, FALSE );//max item 2
        //      MMIAPISET_AppendDualSimName(EMAIL_RECV_SELECT_SIM_CTRL_ID, STXT_OK, MMI_DUAL_SYS_MAX);
        MMK_SetAtvCtrl(EMAIL_RECV_SELECT_SIM_WIN_ID, EMAIL_RECV_SELECT_SIM_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        {
            list_index = GUILIST_GetCurItemIndex(EMAIL_RECV_SELECT_SIM_CTRL_ID); 
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (0 == list_index)
            {
                s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_1;
            }
            else 
            {
                s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_2;
            }
#else
			s_email_box_globle_p->recv_dual_sim = MN_DUAL_SYS_1;
#endif
            EMA_UTILS_TRACE("HandleSimSelectRecvWinMsg dual_sys = %d", s_email_box_globle_p->recv_dual_sim);
            MMK_CloseWin(win_id);
            //MMIEmail_CreateSendingWaitingWin(dual_sys, record_id);
            //MMK_PostMsg(EMAIL_EDIT_WIN_ID, EVT_EMA_INNER_WAITING_SENDMAIL, 0, 0);
            MMIEMAIL_PublicProc_Retrieve();
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //       if(!MMK_IsOpenWin(EMAIL_SENDING_WIN_ID))
        //       {
        //          //EMAILWin_Exit(SENDING_WIN_ENTRY);
        //       }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/**************************************************************************************/
// Description : free email vector.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void MarkEmailVectorFree(void *data_ptr)
{
    uint32 *email_id_ptr = (uint32 *)data_ptr;
    
    EMA_UTILS_FREEIF(PNULL, email_id_ptr);
}

/**************************************************************************************/
// Description : to send email.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN PublicProcSending(uint32 email_id, uint32 net_id)
{
    EMA_UTILS_TRACE("EmailASP_TRS_SendMsg, the email id is %d, the net id is %d", email_id, net_id);
    if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_SendEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, net_id))
    {
        EMA_UTILS_LOG_ERROR(("PublicProcSending, MMIEMAIL_SP_TRS_SendEmail failed."));
        return FALSE;
    }
    
    return TRUE;
}

/**************************************************************************************/
// Description : check,need to download email's body or not.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
// LOCAL int32 ViewProc_CheckDLBody(uint32 msg_id)
// {
//     int32    iRet = SUCCESS;
//     BOOLEAN  bDownload = FALSE;
//     wchar * pwszContent =PNULL;
//     
//     do 
//     {
//         ret = MMIEMAIL_SP_Store_Bedown(EMA_GETSTORE_HANDLE(g_email_sp_handle), 
//             EMA_SP_ACTIVE_ACCOUNT,
//             EMASP_INBOX_INDEX,
//             msg_id,
//             &bDownload);
//         EMA_UTILS_BREAKIF(ret, "EmailAUIC_ViewProc_CheckDLBody, get message download infor failed");
//         
//         if (bDownload)
//         {
//             s_email_box_globle_p->msg_id = msg_id;
//             MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_ASKDOWN, EMAIL_EMAILBODY_ASKDOWN);
//             ret = EMA_NOT_BODY;
//         }
//         else
//         {
//             ret = EMA_EXIST_BODY;
//         }
//         
//     } while(0);
//     
//     return ret;
// }

/**************************************************************************************/
// Description : to retrieve email body.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN PublicProcRetrieveEmlBody(uint32 email_id, uint32 net_id)
{
    //MMIEmail_CreateRecvingWaitingWin(s_email_box_globle_p->recv_dual_sim);
    
    EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveEmailBody, the email id is %d, the net id is %d", email_id, net_id);
    if (SUCCESS != MMIEMAIL_SP_TRS_RetrieveEmailBody(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, email_id, net_id))
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveEmlBody, MMIEMAIL_SP_TRS_RetrieveEmailBody failed."));
        return FALSE;
    }
    return TRUE;
}

/**************************************************************************************/
// Description : sort email by sort type.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 BoxMgrSortBy(uint32 sort_by_type)
{
    int result = EMAIL_ERROR;
    uint16 order_type = 0;
    uint16 order = 0;
    uint16 bit = 0;
    
    do 
    {
        bit = (uint16)sort_by_type - 1;
        order = (s_email_box_globle_p->descent & 1<<bit)?1:0;
        
        order_type = ((uint16)sort_by_type<<8) | order;
        
        result = BoxOrderResult(order_type, (uint32)s_email_box_globle_p->box_vector_ptr);
        EMA_UTILS_BREAKIF(result, "BoxMgrSortBy, BoxOrderResult failed.");
        
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : to order email in box by ordertype.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 MarkOrderIndex(EMAIL_BOXINFO_LIST_T *box_list_ptr, int32 box_by_ordertype)
{
    EMAIL_BOXINFO_LIST_T *start_list_ptr = box_list_ptr;
    EMAIL_BOXINFO_LIST_T *list_cur_ptr   = box_list_ptr;
    EMAIL_BOXINFO_LIST_T *list_next_ptr  = box_list_ptr;
    EMAIL_BOXINFO_LIST_T *list_pre_ptr   = box_list_ptr;
    EMAIL_BOXINFO_LIST_T *list_onlyem_ptr = box_list_ptr;
    uint32               order_index     = 1;
    uint32               order_type      = 0;
    
    if (PNULL == box_list_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EMA_Boxs_MarkOrderIndex(),input parameters is invalid!!!!"));
        return EMAIL_BADPARAM;
    }
    
    /*clear the order index to zero*/
    InitBoxListIndex(box_list_ptr);
    order_type = box_by_ordertype>>8;
    /*by Receiver:  if Receiver is PNULL ,first order them.*/
    if (order_type == EMA_BOXS_ORDERBY_RECEIVER)         // added by vincent
    {
        while(PNULL != list_onlyem_ptr)
        {
            if (list_onlyem_ptr->email_header_ptr->wsz_to_ptr == PNULL)
            {
                list_onlyem_ptr->order_index = order_index;
                order_index ++;
            }
            list_onlyem_ptr = list_onlyem_ptr->next_ptr;
        }
    }
    
    while (PNULL != list_cur_ptr)
    {
        /*have ordered ,jump them*/
        while (list_next_ptr->order_index != 0)
        {
            list_next_ptr = list_next_ptr->next_ptr;
            if (PNULL == list_next_ptr)
            {
                /*order finished,order_index is the max index */
                return SUCCESS;
            }
        }
        
        list_pre_ptr = list_next_ptr;
        list_next_ptr = list_next_ptr->next_ptr;
        
        while(PNULL != list_next_ptr)
        {
            if ( 0 == list_next_ptr->order_index)
            {
                switch(order_type)        // added by vincent
                {
                case EMA_BOXS_ORDERBY_SENDER:
                    {
                        if (EMA_UTILS_WSTRCMP(list_pre_ptr->email_header_ptr->wsz_from_ptr,list_next_ptr->email_header_ptr->wsz_from_ptr) <= 0)
                        {
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                        else
                        {
                            list_pre_ptr = list_next_ptr;
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                    }
                    break;
                    
                case EMA_BOXS_ORDERBY_RECEIVER:
                    {
                        if (EMA_UTILS_WSTRCMP(list_pre_ptr->email_header_ptr->wsz_to_ptr,list_next_ptr->email_header_ptr->wsz_to_ptr) <= 0)
                        {
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                        else
                        {
                            list_pre_ptr = list_next_ptr;
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                    }
                    break;
                    
                case EMA_BOXS_ORDERBY_DATE:
                    {
                        if (list_pre_ptr->email_header_ptr->dwdate <= list_next_ptr->email_header_ptr->dwdate)
                        {
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                        else
                        {
                            list_pre_ptr = list_next_ptr;
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                    }
                    break;
                    
                case EMA_BOXS_ORDERBY_SUBJECT:
                    {
                        if (EMA_UTILS_WSTRCMP(list_pre_ptr->email_header_ptr->wsz_subject_ptr,list_next_ptr->email_header_ptr->wsz_subject_ptr) <= 0)
                        {
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                        else
                        {
                            list_pre_ptr = list_next_ptr;
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                    }
                    break;
                    
                case EMA_BOXS_ORDERBY_UNREAD:
                    {
                        if (list_pre_ptr->email_header_ptr->mail_status <= list_next_ptr->email_header_ptr->mail_status)
                        {
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                        else
                        {
                            list_pre_ptr = list_next_ptr;
                            list_next_ptr = list_next_ptr->next_ptr;
                        }
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_REC_NEW:
                    if (list_pre_ptr->email_header_ptr->dwdate >= list_next_ptr->email_header_ptr->dwdate)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_REC_OLD:
                    if (list_pre_ptr->email_header_ptr->dwdate <= list_next_ptr->email_header_ptr->dwdate)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_SIZE_LARGE:
                    if (list_pre_ptr->email_header_ptr->email_size >= list_next_ptr->email_header_ptr->email_size)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_SIZE_SMALL:
                    if (list_pre_ptr->email_header_ptr->email_size <= list_next_ptr->email_header_ptr->email_size)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_PRO_HIGH:
                    if (list_pre_ptr->email_header_ptr->email_prio <= list_next_ptr->email_header_ptr->email_prio)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                case  EMA_BOXS_ORDERBY_PRO_LOW:
                    if (list_pre_ptr->email_header_ptr->email_prio >= list_next_ptr->email_header_ptr->email_prio)
                    {
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    else
                    {
                        list_pre_ptr = list_next_ptr;
                        list_next_ptr = list_next_ptr->next_ptr;
                    }
                    break;
                    
                default:
                    return EMAIL_ERROR;
            }
         }
         else
         {
             list_next_ptr = list_next_ptr->next_ptr;
         }
      }
      list_next_ptr = start_list_ptr;
      list_pre_ptr->order_index = order_index;
      order_index++;
      
      list_cur_ptr = list_cur_ptr->next_ptr;
   }
   
   return SUCCESS;
}

/**************************************************************************************/
// Description : box list date init.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 InitBoxListIndex(EMAIL_BOXINFO_LIST_T *box_list_ptr)
{
    EMAIL_BOXINFO_LIST_T *list_ptr = box_list_ptr;
    
    while (PNULL != list_ptr)
    {
        list_ptr->order_index = 0;
        list_ptr = list_ptr->next_ptr;
    }
    return SUCCESS;
}

/**************************************************************************************/
// Description : order box list date.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 OrderBoxList(
                         EMAIL_VECTOR_T *box_vector_ptr, 
                         EMAIL_BOXINFO_LIST_T *box_list_ptr, 
                         void *mem_hdr_ptr, 
                         int32 vector_num
                         )
{
    EMAIL_BOXINFO_LIST_T  *list_ptr         = box_list_ptr;
    EMAIL_BOX_ORDERLIST_T *order_list_cur_ptr = PNULL;
    EMAIL_BOX_ORDERLIST_T *order_list_next_ptr = PNULL;
    int32 i = 0;
    
    if ( PNULL == box_vector_ptr || PNULL == list_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EMA_OrderInboxList(),input parameters is invalid!!!!"));
        return EMAIL_ERROR;
    }
    
    order_list_cur_ptr = EMA_UTILS_MALLOC(mem_hdr_ptr,sizeof(EMAIL_BOX_ORDERLIST_T));
    if (PNULL == order_list_cur_ptr)
    {
        return EMAIL_ERROR;
    }
    EMA_UTILS_MEMSET(order_list_cur_ptr,0x00,sizeof(EMAIL_BOX_ORDERLIST_T));
    order_list_cur_ptr->email_header_ptr = EMA_UTILS_MALLOC(mem_hdr_ptr,sizeof(EMAIL_MSGHEAD_T));
    EMA_UTILS_MEMSET(order_list_cur_ptr->email_header_ptr,0x00,sizeof(EMAIL_MSGHEAD_T));
    
    order_list_next_ptr = order_list_cur_ptr;
    
    for (i = 1; i < vector_num + 1; i++)
    {
        while(PNULL != list_ptr)
        {
            if (list_ptr->order_index == i)
            {
                if (PNULL != list_ptr->email_header_ptr->wsz_subject_ptr)
                {
                    order_list_next_ptr->email_header_ptr->wsz_subject_ptr = 
                        EMA_UTILS_WSTRDUP(mem_hdr_ptr, list_ptr->email_header_ptr->wsz_subject_ptr); 
                }
                if (PNULL != list_ptr->email_header_ptr->wsz_from_ptr)
                {
                    order_list_next_ptr->email_header_ptr->wsz_from_ptr = 
                        EMA_UTILS_WSTRDUP(mem_hdr_ptr, list_ptr->email_header_ptr->wsz_from_ptr); 
                }
                
                order_list_next_ptr->email_header_ptr->dwdate = list_ptr->email_header_ptr->dwdate;
                
                if (PNULL != list_ptr->email_header_ptr->wsz_to_ptr)
                {
                    order_list_next_ptr->email_header_ptr->wsz_to_ptr = 
                        EMA_UTILS_WSTRDUP(mem_hdr_ptr, list_ptr->email_header_ptr->wsz_to_ptr); 
                }
                
                order_list_next_ptr->email_header_ptr->email_id = list_ptr->email_header_ptr->email_id;
                order_list_next_ptr->email_header_ptr->mail_status = list_ptr->email_header_ptr->mail_status;
                order_list_next_ptr->email_header_ptr->email_size = list_ptr->email_header_ptr->email_size;//add by wuhui
                order_list_next_ptr->email_header_ptr->email_prio = list_ptr->email_header_ptr->email_prio;//add by wuhui
                MMIEMAIL_VectorInsertAt(box_vector_ptr, i - 1, (void *)order_list_next_ptr->email_header_ptr);
                
                break;
            }
            list_ptr = list_ptr->next_ptr;
        }
        
        list_ptr = box_list_ptr;
        if ( i != vector_num)
        {
            order_list_next_ptr->next_ptr = EMA_UTILS_MALLOC(mem_hdr_ptr,sizeof(EMAIL_BOX_ORDERLIST_T));
            if (PNULL == order_list_next_ptr->next_ptr)
            {
                return EMAIL_ERROR;
            }
            EMA_UTILS_MEMSET(order_list_next_ptr->next_ptr,0x00,sizeof(EMAIL_BOX_ORDERLIST_T));
            order_list_next_ptr->next_ptr->email_header_ptr = EMA_UTILS_MALLOC(mem_hdr_ptr,sizeof(EMAIL_MSGHEAD_T));
            EMA_UTILS_MEMSET(order_list_next_ptr->next_ptr->email_header_ptr,0x00,sizeof(EMAIL_MSGHEAD_T));
            
            order_list_next_ptr = order_list_next_ptr->next_ptr;
        }
    }
    
    return ClearBoxOrderList(mem_hdr_ptr, &order_list_cur_ptr);
}

/**************************************************************************************/
// Description : clear box list info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 ClearBoxInfoList(void *mem_hdr_ptr, EMAIL_BOXINFO_LIST_T **box_list_pptr)
{
    EMAIL_BOXINFO_LIST_T *list_cur_ptr = PNULL;
    EMAIL_BOXINFO_LIST_T *list_next_ptr = PNULL;
    
    if (PNULL == box_list_pptr)
    {
        return EMAIL_ERROR;
    }
    if (PNULL == *box_list_pptr)
    {
        return SUCCESS;
    }
    list_cur_ptr = *box_list_pptr;
    
    while (PNULL != list_cur_ptr)
    {  
        list_next_ptr  = list_cur_ptr;
        list_cur_ptr = list_cur_ptr->next_ptr;
        if (PNULL != list_next_ptr->email_header_ptr)
        {
            //			EMA_UTILS_FREEIF(mem_hdr_ptr, list_next_ptr->msg_header_ptr->date_ptr);
            EMA_UTILS_FREEIF(mem_hdr_ptr, list_next_ptr->email_header_ptr->wsz_from_ptr);
            EMA_UTILS_FREEIF(mem_hdr_ptr, list_next_ptr->email_header_ptr->wsz_to_ptr);
            EMA_UTILS_FREEIF(mem_hdr_ptr, list_next_ptr->email_header_ptr->wsz_subject_ptr);
            EMA_UTILS_FREEIF(mem_hdr_ptr, list_next_ptr->email_header_ptr);
        }
        EMA_UTILS_FREEIF(mem_hdr_ptr,list_next_ptr);/*lint !e774*/
    }
    
    *box_list_pptr = PNULL;
    return SUCCESS;
}

/**************************************************************************************/
// Description : clear box order list info.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 ClearBoxOrderList(void *mem_hdr_ptr,EMAIL_BOX_ORDERLIST_T **box_list_pptr)
{
    EMAIL_BOX_ORDERLIST_T *list_cur_ptr = PNULL;
    EMAIL_BOX_ORDERLIST_T *list_next_ptr = PNULL;
    
    if (PNULL == box_list_pptr)
    {
        return EMAIL_ERROR;
    }
    if (PNULL == *box_list_pptr)
    {
        return EMAIL_SUCCESS;
    }
    list_cur_ptr = *box_list_pptr;
    
    while (PNULL != list_cur_ptr)
    {  
        list_next_ptr  = list_cur_ptr;
        list_cur_ptr = list_cur_ptr->next_ptr;
        list_next_ptr->email_header_ptr = PNULL;
        EMA_UTILS_FREEIF(mem_hdr_ptr,list_next_ptr);/*lint !e774*/
    }
    
    *box_list_pptr = PNULL;
    return EMAIL_SUCCESS;
}

/**************************************************************************************/
// Description : box list order result.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL int32 BoxOrderResult(int32 box_by_ordertype, uint32 data)
{
    int32 num        = 0;
    int32 vector_num = 0;
    int32 result     = EMAIL_SUCCESS;
    EMAIL_BOXINFO_LIST_T *list_cur_ptr  = PNULL;
    EMAIL_BOXINFO_LIST_T *list_next_ptr = PNULL;
    EMAIL_VECTOR_T       *vector_ptr    = (EMAIL_VECTOR_T *)data;
    
    if (PNULL == vector_ptr)
    {
        EMA_UTILS_LOG_ERROR(("EMA_BoxOrderResult(),input parameters is invalid!!!!"));
        return EMAIL_ERROR;
    }
    
    vector_num = MMIEMAIL_VectorGetVectorNumber(vector_ptr);
    if ( 0 == vector_num)
    {
        return EMAIL_SUCCESS;
    }
    
    do 
    {
        list_cur_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
        EMA_UTILS_BREAKNULL(list_cur_ptr, result, "EMAFmMgr_BoxOrderResult, list_cur_ptr is PNULL");
        EMA_UTILS_MEMSET(list_cur_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
        list_next_ptr  = list_cur_ptr;
        
        for (num = 0 ; num < vector_num ; num++)
        {
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            MMIEMAIL_VectorGetAt(vector_ptr, num, (void **)&email_header_ptr);
            if (PNULL != email_header_ptr)
            {
                list_next_ptr->email_header_ptr = (EMAIL_MSGHEAD_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGHEAD_T));
                EMA_UTILS_BREAKNULL(list_next_ptr->email_header_ptr, result, "EMAFmMgr_BoxOrderResult, list_next_ptr->email_header_ptr is PNULL");
                EMA_UTILS_MEMSET(list_next_ptr->email_header_ptr, 0, sizeof(EMAIL_MSGHEAD_T));
                MMIEMAIL_CopyEmailHead(list_next_ptr->email_header_ptr, email_header_ptr, PNULL);
            }
            
            if (num != vector_num - 1)
            {
                list_next_ptr->next_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
                if (PNULL == list_next_ptr->next_ptr)
                {
                    return EMAIL_ERROR;
                }
                EMA_UTILS_MEMSET(list_next_ptr->next_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
                list_next_ptr = list_next_ptr->next_ptr;
            }
        }
        
        result = MarkOrderIndex(list_cur_ptr,box_by_ordertype);
        EMA_UTILS_BREAKIF(result, "EMA_Boxs_MarkOrderIndex,failed!!!!");
        MMIEMAIL_VectorDeleteAll(vector_ptr);
        result = OrderBoxList(vector_ptr,list_cur_ptr,PNULL,vector_num);
        EMA_UTILS_BREAKIF(result, "EMA_OrderBoxList,failed!!!!");
        result = ClearBoxInfoList(PNULL,&list_cur_ptr);
        EMA_UTILS_BREAKIF(result, "EMA_ClearBoxInfoList,failed!!!!");
    } while(0);
    
    return result;
}

/**************************************************************************************/
// Description : update inbox email's status.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN InBoxProc_UpdateEmailStatus(uint32 email_status, uint32 email_id)
{
    EMAIL_MSGDATACACHE_T  *date_cache_ptr = PNULL;
    int result = SUCCESS;
    
    do 
    {
        date_cache_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        if (PNULL == date_cache_ptr)
        {
            EMA_UTILS_TRACE("InBoxProc_UpdateEmailStatus, date_cache_ptr is PNULL.");
            return FALSE;
        }
        EMA_UTILS_MEMSET(date_cache_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, date_cache_ptr);
        EMA_UTILS_BREAKIF(result, "InBoxProc_UpdateEmailStatus, get email dta failed");
        date_cache_ptr->mail_status = email_status;
        
//         result = MMIEMAIL_SP_Store_UpdateMsgHeader(EMA_GETSTORE_HANDLE(g_email_sp_handle), msg_id);
//         EMA_UTILS_BREAKIF(result, "InBoxProc_UpdateMsgStatus, MMIEMAIL_SP_Store_UpdateMsgHeader failed.");
        result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle),
            EMAILA_MSGMASK_STATE, date_cache_ptr);
        EMA_UTILS_BREAKIF(result, "InBoxProc_UpdateEmailStatus, MMIEMAIL_SP_Store_UpdateEmail failed."); 
    } while(0);
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, date_cache_ptr);
    
    return TRUE;
}

/**************************************************************************************/
// Description : move email to draft from outbox.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN OutBoxProc_MovetoDraft(uint32 email_id)
{
    int result = EMAIL_ERROR;
    
    do 
    {
        result = MMIEMAIL_SP_Store_MoveEmailtoFolder(EMA_GETSTORE_HANDLE(g_email_sp_handle),
            EMA_SP_ACTIVE_ACCOUNT,
            EMA_OUTBOX, 
            EMA_SP_ACTIVE_ACCOUNT,
            EMA_DRAFTBOX,
            email_id);
        EMA_UTILS_BREAKIF(result, "call EmailAUIC_OutBoxProc_MovetoDraft failed");
    } while(0);
    
    return TRUE;
}

/**************************************************************************************/
// Description : 添加tab页和子窗口
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void  AddTabsAndChildWindows( void )
{
    MMI_STRING_T str_text = {0};
    MMI_IMAGE_ID_T email_pda_tab[MMIEMAIL_BOX_FROM_MAX][2] = 
        {{IMAGE_EMAIL_INBOX_TAB_ON_ICON, IMAGE_EMAIL_INBOX_TAB_OFF_ICON},
        {IMAGE_EMAIL_SENDFAILBOX_TAB_ON_ICON, IMAGE_EMAIL_SENDFAILBOX_TAB_OFF_ICON},
        {IMAGE_EMAIL_SENDSUCCBOX_TAB_ON_ICON, IMAGE_EMAIL_SENDSUCCBOX_TAB_OFF_ICON},
        {IMAGE_EMAIL_DRAFTBOX_TAB_ON_ICON, IMAGE_EMAIL_DRAFTBOX_TAB_OFF_ICON}};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    MMI_IMAGE_ID_T istyle_email_pda_tab[MMIEMAIL_BOX_FROM_MAX][2] = 
        {{ISTYLE_IMAGE_EMAIL_INBOX_TAB_ON_ICON, ISTYLE_IMAGE_EMAIL_INBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_EMAIL_SENDFAILBOX_TAB_ON_ICON, ISTYLE_IMAGE_EMAIL_SENDFAILBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_EMAIL_SENDSUCCBOX_TAB_ON_ICON, ISTYLE_IMAGE_EMAIL_SENDSUCCBOX_TAB_OFF_ICON},
        {ISTYLE_IMAGE_EMAIL_DRAFTBOX_TAB_ON_ICON, ISTYLE_IMAGE_EMAIL_DRAFTBOX_TAB_OFF_ICON}};
#endif

    MMI_IMAGE_ID_T (*pda_tab_ptr)[2] = {PNULL};

#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
    if (MMITHEME_IsIstyle())
    {
        pda_tab_ptr = istyle_email_pda_tab;
    }
    else
#endif
    {
        pda_tab_ptr = email_pda_tab;
    }
    
    GUITAB_SetMaxItemNum(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_MAX);
    
    MMI_GetLabelTextByLang(TXT_INBOX , &str_text);
    GUITAB_AppendSimpleItem(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[0][0],
        pda_tab_ptr[0][1]);
    GUITAB_AddChildWindow(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        EMAIL_INBOX_CHILD_WIN_ID, 
        0);
    
    MMI_GetLabelTextByLang(TXT_COMM_OUTBOX, &str_text);
    GUITAB_AppendSimpleItem(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[1][0],
        pda_tab_ptr[1][1]);
    GUITAB_AddChildWindow(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        EMAIL_OUTBOX_CHILD_WIN_ID,
        1);
    
    MMI_GetLabelTextByLang(TXT_SENT, &str_text);
    GUITAB_AppendSimpleItem(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[2][0],
        pda_tab_ptr[2][1]);
    GUITAB_AddChildWindow(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        EMAIL_SENTBOX_CHILD_WIN_ID, 
        2);
    
    MMI_GetLabelTextByLang(TXT_COMM_DRAFT, &str_text);
    GUITAB_AppendSimpleItem(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        &str_text,
        pda_tab_ptr[3][0],
        pda_tab_ptr[3][1]);
    GUITAB_AddChildWindow(
        MMIEMAIL_MSGBOX_TAB_CTRL_ID,
        EMAIL_DRAFTBOX_CHILD_WIN_ID,
        3);
    
    switch (s_email_box_globle_p->box_reason_type)
    {
    case MMIEMAIL_BOX_FROM_INBOX:
        GUITAB_SetCurSel(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_INBOX);
        break;
    case MMIEMAIL_BOX_FROM_OUTBOX:
        GUITAB_SetCurSel(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_OUTBOX);
        break;
    case MMIEMAIL_BOX_FROM_SENTBOX:
        GUITAB_SetCurSel(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_SENTBOX);
        break;
    case MMIEMAIL_BOX_FROM_DRAFT:
        GUITAB_SetCurSel(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_DRAFT);
        break;
    default:
        GUITAB_SetCurSel(MMIEMAIL_MSGBOX_TAB_CTRL_ID, MMIEMAIL_BOX_FROM_INBOX);
        break;
    }
}

/**************************************************************************************/
// Description : to handle the message of Email inbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleInBoxChildWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E   recode         = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id        = EMAIL_INBOX_LISTBOX_CTRL_ID;
    LOCAL BOOLEAN  is_have_items  = FALSE;
    LOCAL BOOLEAN  is_need_update = FALSE;
    uint32         mark_num       = 0;
    uint16         selected_index = 0;
    uint16         total_num      = 0;
    uint16         num            = 0;
    uint16         top_index      = 0;
    uint16         page_num       = 0;
    uint32         net_id         = 0;
    int32          ret            = 0;
#if defined(MMI_PDA_SUPPORT)
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = MMIEMAIL_INBOX_FORM_CTRL_ID;
#endif

    EMA_UTILS_TRACE("HandleInBoxChildWinMsg msg_id = 0x%x", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, INBOX_MAX_EMAIL_NUMBER, FALSE);
        s_email_box_globle_p->selected_index = 0;
        s_email_box_globle_p->sort_type = inbox_sort_type;
        if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
        {
            // load items to list box
            is_have_items = LoadEmailListFromOrder(EMA_INBOX, ctrl_id);
            if (is_have_items)
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            }
            else
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
            }
        }
#if defined(MMI_PDA_SUPPORT)
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_ctrl_id,ctrl_id, &child_height);
        GUIFORM_IsSlide(form_ctrl_id, FALSE);
        GUIFORM_PermitChildBg(form_ctrl_id,FALSE);
        GUILIST_PermitBorder(ctrl_id,FALSE);
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, EVT_EMA_MARK_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
#if defined(MMI_PDA_SUPPORT)
        if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
#endif
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_email_box_globle_p->selected_index = selected_index;
            if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
            {
                EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
                ViewProcEnter(email_header_ptr->email_id);
            }
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case MSG_CTL_LIST_LONGOK:
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (PNULL == s_email_box_globle_p->box_vector_ptr && MMIEMAIL_BOX_FROM_INBOX != s_email_box_globle_p->box_reason_type)
        {
            break;
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if(PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
            s_email_box_globle_p->email_id = email_header_ptr->email_id;
        }
        if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
        {
            if (PNULL == s_email_box_globle_p->box_vector_ptr)
            {
                MMK_CreateWin((uint32*)EMAIL_INBOX_MENU_NONE_WIN_TAB, PNULL);
            }
            else
            {
#if defined(MMI_PDA_SUPPORT)
                if (MSG_CTL_LIST_LONGOK == msg_id)
                {
                    if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_INBOX_POP_MENU_WIN_TAB, PNULL);
                    }
                }
                else
                {
                    if (GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_BOX_MARK_MENU_WIN_TAB, PNULL);
                    }
                    else
                    {
                        MMK_CreateWin((uint32*)EMAIL_INBOX_MENU_WIN_TAB, PNULL);
                    }
                }
#else
                MMK_CreateWin((uint32*)EMAIL_INBOX_MENU_WIN_TAB, PNULL);
#endif
            }
        }
        break;
        
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(EMAIL_BOX_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->email_id_ptr);
        s_email_box_globle_p->email_id_ptr = PNULL;
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
#if defined(MMI_PDA_SUPPORT)
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, FALSE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
        
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
        }
        else
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
        }
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#else
        mark_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
#endif
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_NULL_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        UpdateBoxData(EMA_INBOX);
        GUILIST_SetCurItemIndex(ctrl_id, s_email_box_globle_p->selected_index);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_DELETE_EML:
        {
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            int32           vector_num = 0;
            s_email_box_globle_p->selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            if(!PublicProcDelEmail(email_header_ptr->email_id))
            {
                EMA_UTILS_LOG_INFO(("PublicProcDelEmail failed"));
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            
            MMIEMAIL_VectorDeleteAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index);

            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        }
        break;
        
    case EVT_EMA_DELETEMUL_TAB:
        {
            uint32 email_num = 0;
            uint32 marked_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 *email_id_ptr = PNULL;
            uint32 index = 0;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr 
                || PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result
                || EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            marked_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (0 == marked_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
		   GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
                MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
                {
                    MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
                }
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            else
            {
                uint32 masked_index = 0;
                email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, marked_num * sizeof(uint32));
                EMA_UTILS_MEMSET(email_id_ptr, 0, marked_num * sizeof(uint32));
                for (index = 0; index < email_num; index++)
                {
                    if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                    {
                        MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                        email_id_ptr[masked_index] = email_header_ptr->email_id;
                        masked_index++;
                    }
                }
                EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
                if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, email_id_ptr, marked_num))
                {
                    EMA_UTILS_LOG_ERROR(("HandleInBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
                }

                if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                    s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                }
                EMA_UTILS_FREEIF(PNULL, email_id_ptr);
            }
        }
        break;
        
    case EVT_EMA_DELETEALL_TAB:
        {
            uint32 email_num = 0;
            uint32 *emailid_list_ptr = PNULL;
            uint32 index = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            emailid_list_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, email_num * sizeof(uint32));
            EMA_UTILS_MEMSET(emailid_list_ptr, 0, email_num * sizeof(uint32));
            for (index = 0; index < email_num; index++)
            {
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                emailid_list_ptr[index] = email_header_ptr->email_id;
            }
            EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
            if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, emailid_list_ptr, email_num))
            {
                EMA_UTILS_LOG_ERROR(("HandleInBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
            }
            
            EMA_UTILS_FREEIF(PNULL, emailid_list_ptr);
        }
        break;

    case EVT_EMA_DELETEALL_RESULT:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_NULL_TAB, PNULL);
        } 
        else
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            SetAllEmailMarked(EMA_INBOX, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        }

        if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
        {
            MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
        }
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        recode = MMI_RESULT_TRUE;
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
        
    case EVT_EMA_SIM_SELECT:
        s_email_box_globle_p->trans_type = MMIEMAIL_TRANS_EMLBODY;
        g_email_is_send = EMA_TRANS_TYPE_RETRIEVE_EMAIL;
        MMIEMAIL_SelectNet();
        break;
        
    case EVT_EMA_MARKASUNREAD:
        {
            EMAIL_UPDATE_STATE_T *email_state_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 email_status  = 0;
            uint32 index       = 0;
            uint32 count_num   = 0;
            
            if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
            }
            MMIEMAIL_VectorNew(&s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            MMIEMAIL_VectorSetPfnFree(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, MarkEmailVectorFree);
            
            for (index = 0; index < (uint32)MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr); index++)
            {
                if (s_email_box_globle_p->is_marked_ptr[index])
                {
                    EMAIL_MSGHEAD_T *email_item_ptr = PNULL;
                    uint32 *email_id_ptr = PNULL;
                    
                    email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, sizeof(uint32));
                    EMA_UTILS_BREAKNULL(email_id_ptr, recode, "MMIEMAIL_DelDlg_HandleEvent EMA_UTILS_MALLOC failed");
                    EMA_UTILS_MEMSET(email_id_ptr, 0x00, sizeof(uint32));
                    
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_item_ptr);
                    *email_id_ptr = email_item_ptr->email_id;
                    MMIEMAIL_VectorInsert(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, email_id_ptr);
                }
            }
            
            mark_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            
            email_state_ptr = (EMAIL_UPDATE_STATE_T *)EMA_UTILS_MALLOC(PNULL, mark_num * sizeof(EMAIL_UPDATE_STATE_T));
            EMA_UTILS_MEMSET(email_state_ptr, 0, mark_num * sizeof(EMAIL_UPDATE_STATE_T));
            
            for (index = 0; index < (uint32)MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr); index++)
            {
                if (s_email_box_globle_p->is_marked_ptr[index])
                {
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void**)&email_header_ptr);
                    EMA_UTILS_LOG_INFO(("HandleBoxMenuWinMsg email_header_ptr->mail_status = %d", email_header_ptr->mail_status));
                    
                    switch (email_header_ptr->mail_status)
                    {
                    case EMA_BASIC_READ:
                        email_status = EMA_BASIC_UNREAD;
                        break;
                    case EMA_BASIC_READ_HEAD_ONLY:
                        email_status = EMA_BASIC_UNREAD_HEAD_ONLY;
                        break;
                    case EMA_READ_WITH_ATTACH:
                        email_status = EMA_UNREAD_WITH_ATTACH;
                        break;
                    case EMA_READ_AND_FORWARDED:
                        email_status = EMA_UNREAD_AND_FORWARDED;
                        break;
                    case EMA_READ_WITH_ATTACH_AND_FORWARDED:
                        email_status = EMA_UNREAD_WITH_ATTACH_AND_FORWARDED;
                        break;
                    case EMA_READ_AND_REPLIED:
                        email_status = EMA_UNREAD_AND_REPLIED;
                        break;
                    case EMA_READ_WITH_ATTACH_AND_REPLIED:
                        email_status = EMA_UNREAD_WITH_ATTACH_AND_REPLIED;
                        break;
                    default:
                        email_status = email_header_ptr->mail_status;
                        break;
                    }
                    email_state_ptr[count_num].state = MMIEMAIL_SP_Store_ConventToEmeStatus(email_status);
                    email_state_ptr[count_num].emailid = s_email_box_globle_p->email_id_ptr[index];
                    count_num++;
                }
            }
            
            recode = MMIEMAIL_SP_Store_UpdateEmailState(EMA_GETSTORE_HANDLE(g_email_sp_handle),
                email_state_ptr, mark_num);
            EMA_UTILS_BREAKIF(recode, "MMIEMAIL_SP_Store_UpdateEmailState failed.");
            
            if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
            }
            EMA_UTILS_FREEIF(PNULL, email_state_ptr);
        }
        break;
        
    case EVT_EMA_MARKASREAD:
        {
            EMAIL_UPDATE_STATE_T *email_state_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL; 
            uint32 email_status  = 0;
            uint32 index       = 0;
            uint32 count_num   = 0;
            
            if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
            }
            MMIEMAIL_VectorNew(&s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            MMIEMAIL_VectorSetPfnFree(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, MarkEmailVectorFree);
            
            for (index = 0; index < (uint32)MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr); index++)
            {
                if (s_email_box_globle_p->is_marked_ptr[index])
                {
                    EMAIL_MSGHEAD_T *email_item_ptr = PNULL;
                    uint32 *email_id_ptr = PNULL;
                    
                    email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, sizeof(uint32));
                    EMA_UTILS_BREAKNULL(email_id_ptr, recode, "MMIEMAIL_DelDlg_HandleEvent EMA_UTILS_MALLOC failed");
                    EMA_UTILS_MEMSET(email_id_ptr, 0x00, sizeof(uint32));
                    
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_item_ptr);
                    *email_id_ptr = email_item_ptr->email_id;
                    MMIEMAIL_VectorInsert(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr, email_id_ptr);
                }
            }
            
            mark_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            
            email_state_ptr = (EMAIL_UPDATE_STATE_T *)EMA_UTILS_MALLOC(PNULL, mark_num * sizeof(EMAIL_UPDATE_STATE_T));
            EMA_UTILS_MEMSET(email_state_ptr, 0, mark_num * sizeof(EMAIL_UPDATE_STATE_T));
            
            for (index = 0; index < (uint32)MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr); index++)
            {
                if (s_email_box_globle_p->is_marked_ptr[index])
                {
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void**)&email_header_ptr);
                    EMA_UTILS_LOG_INFO(("HandleBoxMenuWinMsg email_header_ptr->mail_status = %d", email_header_ptr->mail_status));
                    
                    switch (email_header_ptr->mail_status)
                    {
                    case EMA_BASIC_UNREAD:
                        email_status = EMA_BASIC_READ;
                        break;
                    case EMA_BASIC_UNREAD_HEAD_ONLY:
                        email_status = EMA_BASIC_READ_HEAD_ONLY;
                        break;
                    case EMA_UNREAD_WITH_ATTACH:
                        email_status = EMA_READ_WITH_ATTACH;
                        break;
                    case EMA_UNREAD_AND_FORWARDED:
                        email_status = EMA_READ_AND_FORWARDED;
                        break;
                    case EMA_UNREAD_WITH_ATTACH_AND_FORWARDED:
                        email_status = EMA_READ_WITH_ATTACH_AND_FORWARDED;
                        break;
                    case EMA_UNREAD_AND_REPLIED:
                        email_status = EMA_READ_AND_REPLIED;
                        break;
                    case EMA_UNREAD_WITH_ATTACH_AND_REPLIED:
                        email_status = EMA_READ_WITH_ATTACH_AND_REPLIED;
                        break;
                    default :
                        email_status = email_header_ptr->mail_status;
                        break;
                    }
                    email_state_ptr[count_num].state = MMIEMAIL_SP_Store_ConventToEmeStatus(email_status);
                    email_state_ptr[count_num].emailid = s_email_box_globle_p->email_id_ptr[index];
                    count_num++;
                }
            }
            
            recode = MMIEMAIL_SP_Store_UpdateEmailState(EMA_GETSTORE_HANDLE(g_email_sp_handle),
                email_state_ptr, mark_num);
            EMA_UTILS_BREAKIF(recode, "MMIEMAIL_SP_Store_UpdateEmailState failed.");
            
            if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
            }
            EMA_UTILS_FREEIF(PNULL, email_state_ptr);
        }
        break;
        
    case EVT_EMS_UPDATE_STATE_RESULT:
        GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, FALSE);
        MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
        MMK_CloseWin(EMAIL_WAITING_WIN_ID);
        break;
        
    case EVT_EMA_MARKED_BY_DATE:
        {
            uint32 box_vector_num = 0;
            uint32 mail_date = 0;
            int    marked_num = 0;
            uint32 index = 0;
            int    result = 0;
            EMAIL_BOXINFO_LIST_T *list_cur_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            
            if (param)
            {
                mail_date = *((uint32*)param);
            }
            list_cur_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
            EMA_UTILS_BREAKNULL(list_cur_ptr, recode, "EMAFmMgr_BoxOrderResult, list_cur_ptr is PNULL");
            EMA_UTILS_MEMSET(list_cur_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
            
            box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            GUILIST_SetMaxSelectedItem(EMAIL_INBOX_LISTBOX_CTRL_ID, INBOX_MAX_EMAIL_NUMBER);
            
            for (index = 0; index < box_vector_num; index++)
            {
                if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                {
                    GUILIST_SetSelectedItem(EMAIL_INBOX_LISTBOX_CTRL_ID, (uint16)index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[index] = FALSE;
                }
                
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                if (PNULL != email_header_ptr)
                {
                    list_cur_ptr->email_header_ptr = (EMAIL_MSGHEAD_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGHEAD_T));
                    EMA_UTILS_BREAKNULL(list_cur_ptr->email_header_ptr, recode, "EMAFmMgr_BoxOrderResult, list_next_ptr->email_header_ptr is PNULL");
                    EMA_UTILS_MEMSET(list_cur_ptr->email_header_ptr, 0, sizeof(EMAIL_MSGHEAD_T));
                    MMIEMAIL_CopyEmailHead(list_cur_ptr->email_header_ptr, email_header_ptr, PNULL);
                }
                if (list_cur_ptr->email_header_ptr->dwdate < mail_date)
                {
                    GUILIST_SetTextListMarkable(EMAIL_INBOX_LISTBOX_CTRL_ID, TRUE);
                    GUILIST_SetSelectedItem(EMAIL_INBOX_LISTBOX_CTRL_ID, (uint16)index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[index] = TRUE;
                    marked_num++;
                }
            }
            
            result = ClearBoxInfoList(PNULL, &list_cur_ptr);
            EMA_UTILS_BREAKIF(result, "EMA_ClearBoxInfoList,failed!!!!");
            
            if (0 == marked_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_EMAIL_MARK_BY_TIME_NOMAIL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                    MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;
        
    case EVT_EMA_RECEIVE:
       EMA_UTILS_LOG_INFO(("HandleBoxWinMsg EVT_EMA_INNER_INBOX_RECEIVE"));
       switch (s_email_box_globle_p->trans_type)
       {
       case MMIEMAIL_TRANS_MSGHEAD:
          if (param)
          {
             net_id= *((uint32*)param);
          }
          EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmailHead, the net id is %d", net_id);
          if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmailHead(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", net_id))
          {
             EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHead, MMIEMAIL_SP_TRS_RetrieveActEmailHead failed."));
             return recode;
          }
          break;
          
       case MMIEMAIL_TRANS_MSG:
          if (param)
          {
             net_id= *((uint32*)param);
          }
          EMA_UTILS_TRACE("MMIEMAIL_SP_TRS_RetrieveActEmail, the net id is %d", net_id);
          if (EMAIL_SUCCESS != MMIEMAIL_SP_TRS_RetrieveActEmail(EMA_GETTRANS_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, (char *)"Inbox", net_id))
          {
             EMA_UTILS_LOG_ERROR(("EmailAUIC_PublicProc_RetrieveHeadBody, MMIEMAIL_SP_TRS_RetrieveActEmail failed."));
             return FALSE;
          }
          break;
          
       case MMIEMAIL_TRANS_EMLBODY:
          if (param)
          {
             net_id= *((uint32*)param);
          }
          PublicProcRetrieveEmlBody(s_email_box_globle_p->email_id, net_id);
          break;
       default:
          break;
       }
       break;
       
   case MSG_GET_FOCUS:
       s_email_box_globle_p->box_reason_type = MMIEMAIL_BOX_FROM_INBOX;
       if (GetCurType() != EMA_INBOX) //从其他信箱切换回来
       {
           uint16 first_item_index = 0;
           uint16 cur_item_index = 0;
           for (num = 0; num < GUILIST_GetTotalItemNum(ctrl_id); num++)
           {
               GUILIST_SetSelectedItem(ctrl_id, num, FALSE);
           }
           GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
           GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
           SetAllEmailMarked(EMA_INBOX, FALSE);
           
           SetCurType(EMA_INBOX);
           if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
           {
               GUIMENU_SetFirstAndSelectedItem(first_item_index, cur_item_index, ctrl_id);
           }
           selected_index = GUILIST_GetCurItemIndex(ctrl_id);
           s_email_box_globle_p->sort_type = inbox_sort_type;
           is_have_items = LoadEmailListFromOrder(EMA_INBOX, ctrl_id);
           if (is_have_items)
           {
               GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
           }
           else
           {
               GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
               MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
               s_email_box_globle_p->box_vector_ptr = PNULL;
           }
           GUILIST_SetCurItemIndex(ctrl_id, selected_index);
           is_need_update = FALSE;
       }
       else
       {
           if (is_need_update || IsNeedSort())
           {
               selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_email_box_globle_p->sort_type = inbox_sort_type;
               is_have_items = LoadEmailListFromOrder(EMA_INBOX, ctrl_id);
               if (is_have_items)
               {
                   GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
               }
               else
               {
                   GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
               }
               GUILIST_SetCurItemIndex(ctrl_id, selected_index);
               is_need_update = FALSE;
               s_email_box_globle_p->is_need_sort = FALSE;
           }
       }
       GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_PDA_SUPPORT
       GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
       if ((GUILIST_GetTotalItemNum(ctrl_id) > 0))
       {
           if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
           {
               GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
           }
           else
           {
               GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
           }
       }
       else
       {
           GUILIST_SetTextListMarkable(ctrl_id, FALSE);
           GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
       }
#endif
       break;
       
   case EVT_EMA_UPDATE_BOX:
       selected_index = GUILIST_GetCurItemIndex(ctrl_id);
       total_num = GUILIST_GetTotalItemNum(ctrl_id);
       top_index = GUILIST_GetTopItemIndex(ctrl_id);
       page_num = GUILIST_GetPageItemNum(ctrl_id);
       
        s_email_box_globle_p->sort_type = inbox_sort_type;
       is_have_items = LoadEmailListFromOrder(EMA_INBOX, ctrl_id);
       if (is_have_items)
       {
           GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
       }
       else
       {
           GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
       }
       
       if (total_num < GUILIST_GetTotalItemNum(ctrl_id))
       {
           if((0 == top_index) && (selected_index != (page_num - 1)))
           {
               GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
               GUILIST_SetTopItemIndex(ctrl_id, top_index);
           }
           else
           {
               GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
               GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
           }
       }
       else
       {
           GUILIST_SetCurItemIndex(ctrl_id, selected_index);
           GUILIST_SetTopItemIndex(ctrl_id, top_index);
       }
       
       if (MMK_IsFocusWin(win_id))
       {
           MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
       }
       break;
       
#if defined(MMI_PDA_SUPPORT)
   case EVT_EMA_MARK_CANCEL:
       {
           EmailBoxUnMarkAll();
           
           GUILIST_SetTextListMarkable(ctrl_id, FALSE);
           GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
           if (MMK_IsFocusWin(win_id))
           {
               MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
           }
       }
       break;
#endif
       
   default:
       recode = MMI_RESULT_FALSE;
       break;
    }
    
    return recode;
}
 
/**************************************************************************************/
// Description : to handle the message of Email outbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleOutBoxChildWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E   recode         = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id        = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
    LOCAL BOOLEAN  is_have_items  = FALSE;
    LOCAL BOOLEAN  is_need_update = FALSE;
#if !defined(MMI_PDA_SUPPORT)
    uint32         mark_num       = 0;
#endif
    uint16         selected_index = 0;
    uint32         net_id         = 0;
    uint32         ret            = 0;
    uint16         total_num      = 0;
    uint16         top_index      = 0;
    uint16         page_num       = 0;
#if defined(MMI_PDA_SUPPORT)
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = MMIEMAIL_OUTBOX_FORM_CTRL_ID;
#endif
    
    EMA_UTILS_TRACE("HandleOutBoxChildWinMsg msg_id = 0x%x", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, OUTBOX_MAX_EMAIL_NUMBER, FALSE);
        s_email_box_globle_p->selected_index = 0;
        if (MMIEMAIL_BOX_FROM_OUTBOX == s_email_box_globle_p->box_reason_type)
        {
        s_email_box_globle_p->sort_type = other_box_sort_type;
            is_have_items = LoadEmailListFromOrder(EMA_OUTBOX, ctrl_id);
            if (is_have_items)
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            }
            else
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
        }
#if defined(MMI_PDA_SUPPORT)
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_ctrl_id,ctrl_id, &child_height);
        GUIFORM_IsSlide(form_ctrl_id, FALSE);
        GUIFORM_PermitChildBg(form_ctrl_id,FALSE);
        GUILIST_PermitBorder(ctrl_id,FALSE);
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, EVT_EMA_MARK_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
        
#if defined(MMI_PDA_SUPPORT)
        if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
#endif
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_email_box_globle_p->selected_index = selected_index;
            
            if (MMIEMAIL_BOX_FROM_OUTBOX == s_email_box_globle_p->box_reason_type)
            {
                EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
                
                ViewProcEnter(email_header_ptr->email_id);
            }
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case MSG_CTL_LIST_LONGOK:
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
        if (MMIEMAIL_BOX_FROM_OUTBOX != s_email_box_globle_p->box_reason_type)
        {
            break;
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if(PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
            s_email_box_globle_p->email_id = email_header_ptr->email_id;
        }
        
        if (MMIEMAIL_BOX_FROM_OUTBOX == s_email_box_globle_p->box_reason_type)
        {
            if (PNULL != s_email_box_globle_p->box_vector_ptr)
            {
#if defined(MMI_PDA_SUPPORT)
                if (MSG_CTL_LIST_LONGOK == msg_id)
                {
                    if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_OUTBOX_POP_MENU_WIN_TAB, PNULL);
                    }
                }
                else
                {
                    if (GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_BOX_MARK_MENU_WIN_TAB, PNULL);
                    }
                    else
                    {
                        MMK_CreateWin((uint32*)EMAIL_OUTBOX_MENU_WIN_TAB, PNULL);
                    }
                }
#else
                MMK_CreateWin((uint32*)EMAIL_OUTBOX_MENU_WIN_TAB, PNULL);
#endif
            }
        }
        break;
        
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(EMAIL_BOX_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->email_id_ptr);
        s_email_box_globle_p->email_id_ptr = PNULL;
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
#if defined(MMI_PDA_SUPPORT)
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, FALSE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
        
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
        }
        else
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
        }
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#else
        mark_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
	      GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
#endif
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_NULL_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        UpdateBoxData(EMA_OUTBOX);
        GUILIST_SetCurItemIndex(ctrl_id, s_email_box_globle_p->selected_index);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_DELETE_EML:
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            int32		vector_num = 0;
            s_email_box_globle_p->selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            if(!PublicProcDelEmail(email_header_ptr->email_id))
            {
                EMA_UTILS_LOG_INFO(("PublicProcDelEmail failed"));
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            
            MMIEMAIL_VectorDeleteAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index);
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
                MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        }
        break;
        
    case EVT_EMA_DELETEMUL_TAB:
        {
            uint32 email_num = 0;
            uint32 marked_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 *email_id_ptr = PNULL;
            uint32 index = 0;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr 
                || PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result
                || EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            marked_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (0 == marked_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
		   GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
                MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
                {
                    MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
                }
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            else
            {
                uint32 masked_index = 0;
                email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, marked_num * sizeof(uint32));
                EMA_UTILS_MEMSET(email_id_ptr, 0, marked_num * sizeof(uint32));
                for (index = 0; index < email_num; index++)
                {
                    if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                    {
                        MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                        email_id_ptr[masked_index] = email_header_ptr->email_id;
                        masked_index++;
                    }
                }
                EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
                if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, email_id_ptr, marked_num))
                {
                    EMA_UTILS_LOG_ERROR(("HandleOutBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
                }
                
                if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                    s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                }
                EMA_UTILS_FREEIF(PNULL, email_id_ptr);
            }
        }
        break;
        
    case EVT_EMA_DELETEALL_TAB:
        {
            uint32 email_num = 0;
            uint32 *emailid_list_ptr = PNULL;
            uint32 num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            emailid_list_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, email_num * sizeof(uint32));
            EMA_UTILS_MEMSET(emailid_list_ptr, 0, email_num * sizeof(uint32));
            for (num = 0; num < email_num; num++)
            {
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, num, (void **)&email_header_ptr);
                emailid_list_ptr[num] = email_header_ptr->email_id;
            }
            EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
            if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, emailid_list_ptr, email_num))
            {
                EMA_UTILS_LOG_ERROR(("HandleOutBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
            }
            
            EMA_UTILS_FREEIF(PNULL, emailid_list_ptr);
        }
        break;

    case EVT_EMA_DELETEALL_RESULT:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_NULL_TAB, PNULL);
        } 
        else
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            SetAllEmailMarked(EMA_OUTBOX, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        }
        
        if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
        {
            MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
        }
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        recode = MMI_RESULT_TRUE;
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
        
    case EVT_EMA_RESEND:
        if (param)
        {
            net_id= *((uint32*)param);
        }
        
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if(!PublicProcSending(s_email_box_globle_p->email_id_ptr[selected_index], net_id))
        {
            EMA_UTILS_LOG_ERROR(("HandleOutBoxChildWinMsg PublicProcSending failed."));
        }
        break;
        
    case EVT_EMA_MOVETODRAFT:
        {
            uint32 vector_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 num = 0;
            
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            for (num = 0; num < vector_num; num++)
            {
                if(s_email_box_globle_p->is_marked_ptr[num])
                {
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, num, (void**)&email_header_ptr);
                    
                    if(!OutBoxProc_MovetoDraft(email_header_ptr->email_id))
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_EMAIL_MOVE_FAILED);
                        EMA_UTILS_LOG_ERROR(("HandleBoxMenuWinMsg OutBoxProc_MovetoDraft"));
                    }
                }
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin( TXT_EMAIL_MOVE_SUCCESS);
#endif
            GUILIST_SetTextListMarkable(EMAIL_OUTBOX_LISTBOX_CTRL_ID, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_CloseWin(EMAIL_WAITING_WIN_ID);
        }
        break;

    case EVT_EMA_MARKED_BY_DATE:
        {
            uint32 box_vector_num = 0;
            uint32 mail_date = 0;
            int    marked_num = 0;
            uint32    index = 0;
            int    result = 0;
            EMAIL_BOXINFO_LIST_T *list_cur_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            
            if (param)
            {
                mail_date = *((uint32*)param);
            }
            list_cur_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
            EMA_UTILS_BREAKNULL(list_cur_ptr, recode, "EMAFmMgr_BoxOrderResult, list_cur_ptr is PNULL");
            EMA_UTILS_MEMSET(list_cur_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
            
            box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            GUILIST_SetMaxSelectedItem(EMAIL_OUTBOX_LISTBOX_CTRL_ID, DRAFT_MAX_EMAIL_NUMBER);
            
            for (index = 0; index < box_vector_num; index++)
            {
                if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                {
                    GUILIST_SetSelectedItem(EMAIL_OUTBOX_LISTBOX_CTRL_ID, index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[index] = FALSE;
                }
                
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                if (PNULL != email_header_ptr)
                {
                    list_cur_ptr->email_header_ptr = (EMAIL_MSGHEAD_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGHEAD_T));
                    EMA_UTILS_BREAKNULL(list_cur_ptr->email_header_ptr, recode, "EMAFmMgr_BoxOrderResult, list_next_ptr->email_header_ptr is PNULL");
                    EMA_UTILS_MEMSET(list_cur_ptr->email_header_ptr, 0, sizeof(EMAIL_MSGHEAD_T));
                    MMIEMAIL_CopyEmailHead(list_cur_ptr->email_header_ptr, email_header_ptr, PNULL);
                }
                if (list_cur_ptr->email_header_ptr->dwdate < mail_date)
                {
                    GUILIST_SetTextListMarkable(EMAIL_OUTBOX_LISTBOX_CTRL_ID, TRUE);
                    GUILIST_SetSelectedItem(EMAIL_OUTBOX_LISTBOX_CTRL_ID, (uint16)index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[index] = TRUE;
                    marked_num++;
                }
            }
            
            result = ClearBoxInfoList(PNULL, &list_cur_ptr);
            EMA_UTILS_BREAKIF(result, "EMA_ClearBoxInfoList,failed!!!!");
            
            if (0 == marked_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_EMAIL_MARK_BY_TIME_NOMAIL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                    MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        {
            uint16 num = 0;
            s_email_box_globle_p->box_reason_type = MMIEMAIL_BOX_FROM_OUTBOX;
            if (GetCurType() != EMA_OUTBOX)
            {
                uint16 first_item_index = 0;
                uint16 cur_item_index = 0;
                
                s_email_box_globle_p->box_reason_type = MMIEMAIL_BOX_FROM_OUTBOX;
                for (num = 0; num < GUILIST_GetTotalItemNum(ctrl_id); num++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, num, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
                GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                SetAllEmailMarked(EMA_OUTBOX, FALSE);
                
                SetCurType(EMA_OUTBOX);
                if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
                {
                    GUIMENU_SetFirstAndSelectedItem(first_item_index, cur_item_index, ctrl_id);
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_email_box_globle_p->sort_type = other_box_sort_type;
                is_have_items = LoadEmailListFromOrder(EMA_OUTBOX, ctrl_id);
                if (is_have_items)
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
                }
                else
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
                    s_email_box_globle_p->box_vector_ptr = PNULL;
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                is_need_update = FALSE;
            }
            else
            {
                if (is_need_update || IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_email_box_globle_p->sort_type = other_box_sort_type;
                    is_have_items = LoadEmailListFromOrder(EMA_OUTBOX, ctrl_id);
                    if (is_have_items)
                    {
                        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    is_need_update = FALSE;
                    s_email_box_globle_p->is_need_sort = FALSE;
                }
            }
            GUILIST_ValidCurItemPos(ctrl_id);
            
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
                {
                    GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
                }
                else
                {
                    GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
                }
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
#endif
        }
        break;
        
    case EVT_EMA_UPDATE_BOX:
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        total_num = GUILIST_GetTotalItemNum(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        page_num = GUILIST_GetPageItemNum(ctrl_id);
        
        s_email_box_globle_p->sort_type = other_box_sort_type;
        is_have_items = LoadEmailListFromOrder(EMA_OUTBOX, ctrl_id);
        if (is_have_items)
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
        }
        else
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        
        if (total_num < GUILIST_GetTotalItemNum(ctrl_id))
        {
            if((0 == top_index) && (selected_index != (page_num - 1)))
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index);
            }
            else
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
            }
        }
        else
        {
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            GUILIST_SetTopItemIndex(ctrl_id, top_index);
        }
        
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case EVT_EMA_MARK_CANCEL:
        {
            EmailBoxUnMarkAll();
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
            }
        }
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
 
/**************************************************************************************/
// Description : to handle the message of Email sentbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleSentBoxChildWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E    recode         = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id        = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
    LOCAL BOOLEAN   is_have_items  = FALSE;
    LOCAL BOOLEAN   is_need_update = FALSE;
#if !defined(MMI_PDA_SUPPORT)
    uint32          mark_num       = 0;
#endif
    uint16          selected_index = 0;
    uint32          net_id         = 0;
    int32           ret            = 0;
    uint16          total_num      = 0;
    uint16          top_index      = 0;
    uint16          page_num       = 0;
#if defined(MMI_PDA_SUPPORT)
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = MMIEMAIL_SENTBOX_FORM_CTRL_ID;
#endif
    
    EMA_UTILS_TRACE("HandleSentBoxChildWinMsg msg_id = 0x%x", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, SENTBOX_MAX_EMAIL_NUMBER, FALSE);
        s_email_box_globle_p->selected_index = 0;
        s_email_box_globle_p->sort_type = other_box_sort_type;
        if (MMIEMAIL_BOX_FROM_SENTBOX == s_email_box_globle_p->box_reason_type)
        {
            is_have_items = LoadEmailListFromOrder(EMA_SENTBOX, ctrl_id);
            if (is_have_items)
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            }
            else
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
        }
#if defined(MMI_PDA_SUPPORT)
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_ctrl_id,ctrl_id, &child_height);
        GUIFORM_IsSlide(form_ctrl_id, FALSE);
        GUIFORM_PermitChildBg(form_ctrl_id,FALSE);
        GUILIST_PermitBorder(ctrl_id,FALSE);
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, EVT_EMA_MARK_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
#if defined(MMI_PDA_SUPPORT)
        if (!GUILIST_GetListState(ctrl_id, GUILIST_STATE_ENABLE_MARK))
#endif
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_email_box_globle_p->selected_index = selected_index;
            if (MMIEMAIL_BOX_FROM_SENTBOX == s_email_box_globle_p->box_reason_type)
            {
                EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
                ViewProcEnter(email_header_ptr->email_id);
            }
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case MSG_CTL_LIST_LONGOK:
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
        if (MMIEMAIL_BOX_FROM_SENTBOX != s_email_box_globle_p->box_reason_type)
        {
            break;
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if(PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
            s_email_box_globle_p->email_id = email_header_ptr->email_id;
        }
        
        if (MMIEMAIL_BOX_FROM_SENTBOX == s_email_box_globle_p->box_reason_type)
        {
            if (PNULL != s_email_box_globle_p->box_vector_ptr)
            {
#if defined(MMI_PDA_SUPPORT)
                if (MSG_CTL_LIST_LONGOK == msg_id)
                {
                    if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_SENTBOX_POP_MENU_WIN_TAB, PNULL);
                    }
                }
                else
                {
                    if (GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_BOX_MARK_MENU_WIN_TAB, PNULL);
                    }
                    else
                    {
                        MMK_CreateWin((uint32*)EMAIL_SENTBOX_MENU_WIN_TAB, PNULL);
                    }
                }
#else
                MMK_CreateWin((uint32*)EMAIL_SENTBOX_MENU_WIN_TAB, PNULL);
#endif
            } 
            else
            {
            }
        }
        break;
        
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(EMAIL_BOX_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:        /* ???????? */
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->email_id_ptr);
        s_email_box_globle_p->email_id_ptr = PNULL;
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
#if defined(MMI_PDA_SUPPORT)
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, FALSE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
        
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
        }
        else
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
        }
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#else
        mark_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
#endif
        break;
        
    case EVT_EMA_UPDATE_BOX:
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        total_num = GUILIST_GetTotalItemNum(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        page_num = GUILIST_GetPageItemNum(ctrl_id);
        
        s_email_box_globle_p->sort_type = other_box_sort_type;
        is_have_items = LoadEmailListFromOrder(EMA_SENTBOX, ctrl_id);
        if (is_have_items)
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
        }
        else
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        
        if (total_num < GUILIST_GetTotalItemNum(ctrl_id))
        {
            if((0 == top_index) && (selected_index != (page_num - 1)))
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index);
            }
            else
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
            }
        }
        else
        {
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            GUILIST_SetTopItemIndex(ctrl_id, top_index);
        }
        
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_NULL_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        UpdateBoxData(EMA_SENTBOX);
        GUILIST_SetCurItemIndex(ctrl_id, s_email_box_globle_p->selected_index);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_DELETE_EML:
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            int32		   vector_num = 0;
            
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            if(!PublicProcDelEmail(email_header_ptr->email_id))
            {
                EMA_UTILS_LOG_INFO(("PublicProcDelEmail failed"));
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            MMIEMAIL_VectorDeleteAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index);
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
                MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        }
        break;
        
    case EVT_EMA_DELETEMUL_TAB:
        {
            uint32 email_num = 0;
            uint32 marked_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 *email_id_ptr = PNULL;
            uint32 index = 0;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr 
                || PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result
                || EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            marked_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (0 == marked_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
		   GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件
                MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
                {
                    MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
                }
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            else
            {
                uint32 masked_index = 0;
                email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, marked_num * sizeof(uint32));
                EMA_UTILS_MEMSET(email_id_ptr, 0, marked_num * sizeof(uint32));
                for (index = 0; index < email_num; index++)
                {
                    if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                    {
                        MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                        email_id_ptr[masked_index] = email_header_ptr->email_id;
                        masked_index++;
                    }
                }
                EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
                if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, email_id_ptr, marked_num))
                {
                    EMA_UTILS_LOG_ERROR(("HandleSentBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
                }
                
                if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                    s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                }
                EMA_UTILS_FREEIF(PNULL, email_id_ptr);
            }
        }
        break;
        
    case EVT_EMA_DELETEALL_TAB:
        {
            uint32 email_num = 0;
            uint32 *emailid_list_ptr = PNULL;
            uint32 num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            emailid_list_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, email_num * sizeof(uint32));
            EMA_UTILS_MEMSET(emailid_list_ptr, 0, email_num * sizeof(uint32));
            for (num = 0; num < email_num; num++)
            {
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, num, (void **)&email_header_ptr);
                emailid_list_ptr[num] = email_header_ptr->email_id;
            }
            EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
            if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, emailid_list_ptr, email_num))
            {
                EMA_UTILS_LOG_ERROR(("HandleSentBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
            }
            
            EMA_UTILS_FREEIF(PNULL, emailid_list_ptr);
        }
        break;
        
    case EVT_EMA_DELETEALL_RESULT:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_NULL_TAB, PNULL);
        } 
        else
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            SetAllEmailMarked(EMA_SENTBOX, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        }
        
        if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
        {
            MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
        }
        
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        
        recode = MMI_RESULT_TRUE;
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
        
    case EVT_EMA_RESEND:
        if (param)
        {
            net_id= *((uint32*)param);
        }
        {
            EMAIL_VECTOR_T   			 *box_vector_ptr = PNULL;
            int32                        result = 0;
            EMAIL_MSGHEAD_T              *header_ptr = PNULL;
            int32                        num = 0;
            
            result = MMIEMAIL_SP_Store_GetBoxInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, EMA_OUTBOX, &box_vector_ptr);
            EMA_UTILS_BREAKIF(result, "HandleSentBoxChildWinMsg, MMIEMAIL_SP_Store_GetBoxInfo failed.");
            
            num = MMIEMAIL_VectorGetVectorNumber(box_vector_ptr);
            MMIEMAIL_VectorGetAt(box_vector_ptr, num - 1, &header_ptr);
            
            if(!PublicProcSending(header_ptr->email_id, net_id))
            {
                EMA_UTILS_LOG_ERROR(("HandleSentBoxChildWinMsg PublicProcSending failed."));
            }
            MMIEMAIL_VectorDelete(box_vector_ptr);
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            EMA_UTILS_BREAKNULL(need_item_data_ptr, ret, "out of memory");
            index = need_item_data_ptr->item_index;
            SetListItemData(EMA_SENTBOX, ctrl_id, index);
        }
        break;
        
    case EVT_EMA_MOVETODRAFT:
        {
            uint32 vector_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 num = 0;
            
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            for (num = 0; num < vector_num; num++)
            {
                if(s_email_box_globle_p->is_marked_ptr[num])
                {
                    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, num, (void**)&email_header_ptr);
                    
                    if(!OutBoxProc_MovetoDraft(email_header_ptr->email_id))
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_EMAIL_MOVE_FAILED);
                        EMA_UTILS_LOG_ERROR(("HandleBoxMenuWinMsg OutBoxProc_MovetoDraft"));
                    }
                }
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin( TXT_EMAIL_MOVE_SUCCESS);
#endif
            GUILIST_SetTextListMarkable(EMAIL_SENTBOX_LISTBOX_CTRL_ID, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_CloseWin(EMAIL_WAITING_WIN_ID);
        }
        break;
        
    case EVT_EMA_MARKED_BY_DATE:
        {
            uint32 box_vector_num = 0;
            uint32 mail_date = 0;
            int    marked_num = 0;
            uint32    index = 0;
            int    result = 0;
            EMAIL_BOXINFO_LIST_T *list_cur_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            
            if (param)
            {
                mail_date = *((uint32*)param);
            }
            list_cur_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
            EMA_UTILS_BREAKNULL(list_cur_ptr, recode, "EMAFmMgr_BoxOrderResult, list_cur_ptr is PNULL");
            EMA_UTILS_MEMSET(list_cur_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
            
            box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            GUILIST_SetMaxSelectedItem(EMAIL_SENTBOX_LISTBOX_CTRL_ID, DRAFT_MAX_EMAIL_NUMBER);
            
            for (index = 0; index < box_vector_num; index++)
            {
                if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                {
                    GUILIST_SetSelectedItem(EMAIL_SENTBOX_LISTBOX_CTRL_ID, index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[index] = FALSE;
                }
                
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                if (PNULL != email_header_ptr)
                {
                    list_cur_ptr->email_header_ptr = (EMAIL_MSGHEAD_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGHEAD_T));
                    EMA_UTILS_BREAKNULL(list_cur_ptr->email_header_ptr, recode, "EMAFmMgr_BoxOrderResult, list_next_ptr->email_header_ptr is PNULL");
                    EMA_UTILS_MEMSET(list_cur_ptr->email_header_ptr, 0, sizeof(EMAIL_MSGHEAD_T));
                    MMIEMAIL_CopyEmailHead(list_cur_ptr->email_header_ptr, email_header_ptr, PNULL);
                }
                if (list_cur_ptr->email_header_ptr->dwdate < mail_date)
                {
                    GUILIST_SetTextListMarkable(EMAIL_SENTBOX_LISTBOX_CTRL_ID, TRUE);
                    GUILIST_SetSelectedItem(EMAIL_SENTBOX_LISTBOX_CTRL_ID, (uint16)index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[index] = TRUE;
                    marked_num++;
                }
            }
            
            result = ClearBoxInfoList(PNULL, &list_cur_ptr);
            EMA_UTILS_BREAKIF(result, "EMA_ClearBoxInfoList,failed!!!!");
            
            if (0 == marked_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_EMAIL_MARK_BY_TIME_NOMAIL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                    MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;
        
    case MSG_GET_FOCUS:
        {
            uint16 num = 0;
            s_email_box_globle_p->box_reason_type = MMIEMAIL_BOX_FROM_SENTBOX;
            if (GetCurType() != EMA_SENTBOX)
            {
                uint16 first_item_index = 0;
                uint16 cur_item_index = 0;
                
                for (num = 0; num < GUILIST_GetTotalItemNum(ctrl_id); num++)
                {
                    GUILIST_SetSelectedItem(ctrl_id, num, FALSE);
                }
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
                GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
                SetAllEmailMarked(EMA_SENTBOX, FALSE);
                
                SetCurType(EMA_SENTBOX);
                if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
                {
                    GUIMENU_SetFirstAndSelectedItem(first_item_index, cur_item_index, ctrl_id);
                }
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_email_box_globle_p->sort_type = other_box_sort_type;
                is_have_items = LoadEmailListFromOrder(EMA_SENTBOX, ctrl_id);
                if (is_have_items)
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
                }
                else
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
                    s_email_box_globle_p->box_vector_ptr = PNULL;
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                is_need_update = FALSE;
            }
            else
            {
                if (is_need_update || IsNeedSort())
                {
                    selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                    s_email_box_globle_p->sort_type = other_box_sort_type;
                    is_have_items = LoadEmailListFromOrder(EMA_SENTBOX, ctrl_id);
                    if (is_have_items)
                    {
                        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
                    }
                    else
                    {
                        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                    }
                    GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                    is_need_update = FALSE;
                    s_email_box_globle_p->is_need_sort = FALSE;
                }
            }
            GUILIST_ValidCurItemPos(ctrl_id);
            
#ifdef MMI_PDA_SUPPORT
            if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
                if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
                {
                    GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
                }
                else
                {
                    GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
                }
            }
            else
            {
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
                GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            }
#endif
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case EVT_EMA_MARK_CANCEL:
        {
            EmailBoxUnMarkAll();
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
            }
        }
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
 
/**************************************************************************************/
// Description : to handle the message of Email draftbox window.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleDraftBoxChildWinMsg(
                                              MMI_WIN_ID_T win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E  recode         = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id        = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
    LOCAL BOOLEAN is_have_items  = FALSE;
    LOCAL BOOLEAN is_need_update = FALSE;
#if !defined(MMI_PDA_SUPPORT)
    uint32        mark_num       = 0;
#endif
    uint16        selected_index = 0;
    uint32        net_id         = 0;
    uint32        ret            = 0;
    uint16        total_num      = 0;
    uint16        top_index      = 0;
    uint16        page_num       = 0;
#if defined(MMI_PDA_SUPPORT)
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    MMI_CTRL_ID_T form_ctrl_id = MMIEMAIL_DRAFTBOX_FORM_CTRL_ID;
#endif
    
    EMA_UTILS_TRACE("HandleDraftBoxChildWinMsg msg_id = 0x%x", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        GUILIST_SetMaxItem(ctrl_id, DRAFT_MAX_EMAIL_NUMBER, FALSE);
        s_email_box_globle_p->selected_index = 0;
        s_email_box_globle_p->sort_type = other_box_sort_type;
        if (MMIEMAIL_BOX_FROM_DRAFT == s_email_box_globle_p->box_reason_type)
        {
            is_have_items = LoadEmailListFromOrder(EMA_DRAFTBOX, ctrl_id);
            if (is_have_items)
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            }
            else
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            }
        }
#if defined(MMI_PDA_SUPPORT)
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(form_ctrl_id,ctrl_id, &child_height);
        GUIFORM_IsSlide(form_ctrl_id, FALSE);
        GUIFORM_PermitChildBg(form_ctrl_id, FALSE);
        GUILIST_PermitBorder(ctrl_id, FALSE);
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;
            
            EMA_UTILS_BREAKNULL(need_item_data_ptr, ret, "out of memory");
            index = need_item_data_ptr->item_index;
            SetListItemData(EMA_DRAFTBOX, ctrl_id, index);
        }
        break;
        
    case MSG_CTL_LIST_MARK_ITEM:
#if defined(MMI_PDA_SUPPORT)
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, FALSE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            GUILIST_SetSelectedItem(ctrl_id, selected_index, TRUE);
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
        
        if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
        }
        else
        {
            GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
        }
        MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
#else
        mark_num = GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
	     GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件		
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if (s_email_box_globle_p->is_marked_ptr[selected_index])
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = FALSE;
        }
        else
        {
            s_email_box_globle_p->is_marked_ptr[selected_index] = TRUE;
        }
#endif
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#if defined(MMI_PDA_SUPPORT)
        if (MSG_CTL_PENOK == msg_id)
        {
            if (MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
                return MMI_RESULT_TRUE;
            }
            else if (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMK_PostMsg(win_id, EVT_EMA_MARK_CANCEL, PNULL, 0);
                return MMI_RESULT_TRUE;
            }
        }
#endif
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
        
#if defined(MMI_PDA_SUPPORT)
        if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
#endif
        {
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_email_box_globle_p->selected_index = selected_index;
            if(MMIEMAIL_BOX_FROM_DRAFT == s_email_box_globle_p->box_reason_type)
            {
                EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, (uint32)selected_index, (void **)&email_header_ptr);
                MMIEMAIL_CreateEditWin(MMIEMAIL_EDIT_FROM_DRAFT, (void*)email_header_ptr->email_id);
                UpdateDraftReadState(email_header_ptr->email_id);
            }
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case MSG_CTL_LIST_LONGOK:
    case MSG_APP_MENU:
#endif
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            break;
        }
        
        if (MMIEMAIL_BOX_FROM_DRAFT != s_email_box_globle_p->box_reason_type)
        {
            break;
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        
        if(PNULL != s_email_box_globle_p->box_vector_ptr)
        {
            EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, selected_index, (void **)&email_header_ptr);
            s_email_box_globle_p->email_id = email_header_ptr->email_id;
        }
        
        if (s_email_box_globle_p->box_reason_type == MMIEMAIL_BOX_FROM_DRAFT)
        {
            if (PNULL != s_email_box_globle_p->box_vector_ptr)
            {
#if defined(MMI_PDA_SUPPORT)
                if (MSG_CTL_LIST_LONGOK == msg_id)
                {
                    if (!GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_DRAFTBOX_POP_MENU_WIN_TAB, PNULL);
                    }
                }
                else
                {
                    if (GUILIST_GetListState(ctrl_id,GUILIST_STATE_ENABLE_MARK))
                    {
                        MMK_CreateWin((uint32*)EMAIL_BOX_MARK_MENU_WIN_TAB, PNULL);
                    }
                    else
                    {
                        MMK_CreateWin((uint32*)EMAIL_DRAFTBOX_MENU_WIN_TAB, PNULL);
                    }
                }
#else
                MMK_CreateWin((uint32*)EMAIL_DRAFTBOX_MENU_WIN_TAB, PNULL);
#endif
            }
        }
        break;
        
    case MSG_APP_WEB:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseParentWin(EMAIL_BOX_WIN_ID);
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:        /* ???????? */
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        EMA_UTILS_FREEIF(PNULL, s_email_box_globle_p->email_id_ptr);
        s_email_box_globle_p->email_id_ptr = PNULL;
        break;
        
    case MSG_GET_FOCUS:
        s_email_box_globle_p->box_reason_type = MMIEMAIL_BOX_FROM_DRAFT;
        if (GetCurType() != EMA_DRAFTBOX)
        {
            uint16 num = 0;
            uint16 first_item_index = 0;
            uint16 cur_item_index = 0;
            
            for (num = 0; num < GUILIST_GetTotalItemNum(ctrl_id); num++)
            {
                GUILIST_SetSelectedItem(ctrl_id, num, FALSE);
            }
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            SetAllEmailMarked(EMA_DRAFTBOX, FALSE);
            
            SetCurType(EMA_DRAFTBOX);
            if (MMK_IsOpenWin(EMAIL_MAIN_MENU_WIN_ID))
            {
                GUIMENU_SetFirstAndSelectedItem(first_item_index, cur_item_index, ctrl_id);
            }
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            s_email_box_globle_p->sort_type = other_box_sort_type;
            is_have_items = LoadEmailListFromOrder(EMA_DRAFTBOX, ctrl_id);
            if (is_have_items)
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
            }
            else
            {
                GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
                s_email_box_globle_p->box_vector_ptr = PNULL;
            }
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            is_need_update = FALSE;
        }
        else
        {
            if (is_need_update || IsNeedSort())
            {
                selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                s_email_box_globle_p->sort_type = other_box_sort_type;
                is_have_items = LoadEmailListFromOrder(EMA_DRAFTBOX, ctrl_id);
                if (is_have_items)
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
                }
                else
                {
                    GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
                }
                GUILIST_SetCurItemIndex(ctrl_id, selected_index);
                is_need_update = FALSE;
                s_email_box_globle_p->is_need_sort = FALSE;
            }
        }
        GUILIST_ValidCurItemPos(ctrl_id);
#ifdef MMI_PDA_SUPPORT
        if (GUILIST_GetTotalItemNum(ctrl_id) > 0)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            if (0 == GUILIST_GetSelectedItemIndex(ctrl_id, PNULL, 0))
            {
                GUISOFTKEY_SetButtonState( win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, TRUE, FALSE);
            }
            else
            {
                GUISOFTKEY_SetButtonState(win_id, MMICOMMON_SOFTKEY_CTRL_ID, 0, FALSE, FALSE);
            }
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
        }
#endif
        break;
        
    case EVT_EMA_UPDATE_BOX:
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        total_num = GUILIST_GetTotalItemNum(ctrl_id);
        top_index = GUILIST_GetTopItemIndex(ctrl_id);
        page_num = GUILIST_GetPageItemNum(ctrl_id);
        
        s_email_box_globle_p->sort_type = other_box_sort_type;
        is_have_items = LoadEmailListFromOrder(EMA_DRAFTBOX, ctrl_id);
        if (is_have_items)
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_DEFAULT);
        }
        else
        {
            GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        }
        
        if (total_num < GUILIST_GetTotalItemNum(ctrl_id))
        {
            if((0 == top_index) && (selected_index != (page_num - 1)))
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index);
            }
            else
            {
                GUILIST_SetCurItemIndex(ctrl_id, (uint16)(selected_index + 1));
                GUILIST_SetTopItemIndex(ctrl_id, top_index + 1);
            }
        }
        else
        {
            GUILIST_SetCurItemIndex(ctrl_id, selected_index);
            GUILIST_SetTopItemIndex(ctrl_id, top_index);
        }
        
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_NULL_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        GUILIST_SetTitleIndexType(ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
        MMIEMAIL_VectorDelete(s_email_box_globle_p->box_vector_ptr);
        s_email_box_globle_p->box_vector_ptr = PNULL;
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_UPDATE_LISTBOX_TAB:
        GUILIST_RemoveAllItems(ctrl_id);
        UpdateBoxData(EMA_DRAFTBOX);
        GUILIST_SetCurItemIndex(ctrl_id, s_email_box_globle_p->selected_index);
        if (MMK_IsFocusWin(win_id))
        {
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
        }
        break;
        
    case EVT_EMA_DELETE_EML:
        {
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            int32		vector_num = 0;
            
            MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
            vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            
            if(!PublicProcDelEmail(email_header_ptr->email_id))
            {
                EMA_UTILS_LOG_INFO(("PublicProcDelEmail failed"));
                recode = MMI_RESULT_FALSE;
                return recode;
            }
            
            MMIEMAIL_VectorDeleteAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index);
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
            MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
                MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        }
        break;
        
    case EVT_EMA_DELETEMUL_TAB:
        {
            uint32 email_num = 0;
            uint32 marked_num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            uint32 *email_id_ptr = PNULL;
            uint32 index = 0;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr 
                || PNULL == s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result
                || EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            marked_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
            if (0 == marked_num)
            {
                GUILIST_SetTextListMarkable(ctrl_id, FALSE);
		   GUIAPICTRL_Redraw(ctrl_id);// 由于标记状态的改变，这里需要重画list控件		
                MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
                MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
                {
                    MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
                }
                MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            else
            {
                uint32 masked_index = 0;
                email_id_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, marked_num * sizeof(uint32));
                EMA_UTILS_MEMSET(email_id_ptr, 0, marked_num * sizeof(uint32));
                for (index = 0; index < email_num; index++)
                {
                    if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                    {
                        MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                        email_id_ptr[masked_index] = email_header_ptr->email_id;
                        masked_index++;
                    }
                }
                EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
                if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, email_id_ptr, marked_num))
                {
                    EMA_UTILS_LOG_ERROR(("HandleDraftBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
                }
                
                if (PNULL != s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr)
                {
                    MMIEMAIL_VectorDelete(s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr);
                    s_email_box_globle_p->box_mark_vectormodel.data_vector_ptr = PNULL;
                }
                EMA_UTILS_FREEIF(PNULL, email_id_ptr);
            }
        }
        break;
        
    case EVT_EMA_DELETEALL_TAB:
        {
            uint32 email_num = 0;
            uint32 *emailid_list_ptr = PNULL;
            uint32 num = 0;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            HStoreMgr emasp_store_mgr = MMIEMAIL_SP_GetStoreHandle(g_email_sp_handle);
            
            if (PNULL == s_email_box_globle_p->box_vector_ptr)
            {
                return MMI_RESULT_FALSE;
            }
            
            if (EMA_CANCEL_DELETE == s_email_box_globle_p->delete_result)
            {
                recode = MMI_RESULT_TRUE;
                return recode;
            }
            
            email_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            emailid_list_ptr = (uint32 *)EMA_UTILS_MALLOC(PNULL, email_num * sizeof(uint32));
            EMA_UTILS_MEMSET(emailid_list_ptr, 0, email_num * sizeof(uint32));
            for (num = 0; num < email_num; num++)
            {
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, num, (void **)&email_header_ptr);
                emailid_list_ptr[num] = email_header_ptr->email_id;
            }
            EMA_UTILS_BREAKNULL(emasp_store_mgr, ret, "out of memory");
            if (EMAIL_BADPARAM != MMIEMAIL_SP_Store_DeleteAllEmail(emasp_store_mgr, emailid_list_ptr, email_num))
            {
                EMA_UTILS_LOG_ERROR(("HandleDraftBoxChildWinMsg MMIEMAIL_SP_Store_DeleteAllEmail successfuly."));
            }
            
            EMA_UTILS_FREEIF(PNULL, emailid_list_ptr);
        }
        break;
        
    case EVT_EMA_DELETEALL_RESULT:
        if (PNULL == s_email_box_globle_p->box_vector_ptr)
        {
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_NULL_TAB, PNULL);
        } 
        else
        {
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            SetAllEmailMarked(EMA_DRAFTBOX, FALSE);
            MMK_SendMsg(win_id, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        }
        
        if(MMK_IsOpenWin(EMAIL_DELETEALL_WAIT_WIN_ID)) 
        {
            MMK_CloseWin(EMAIL_DELETEALL_WAIT_WIN_ID);
        }
        recode = MMI_RESULT_TRUE;
        
        GUILIST_SetTextListMarkable(ctrl_id, FALSE);
#if defined(MMI_PDA_SUPPORT)
        GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
#endif
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
#ifdef MMI_GUI_STYLE_TYPICAL
        MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_COMMON_DELETESUCESS, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL,
            MMIPUB_SOFTKEY_ONE, PNULL);
#endif
        break;
        
    case EVT_EMA_MARKED_BY_DATE:
        {
            uint32 box_vector_num = 0;
            uint32 mail_date = 0;
            int    marked_num = 0;
            uint32    index = 0;
            int    result = 0;
            EMAIL_BOXINFO_LIST_T *list_cur_ptr = PNULL;
            EMAIL_MSGHEAD_T *email_header_ptr = PNULL;
            
            if (param)
            {
                mail_date = *((uint32*)param);
            }
            list_cur_ptr = (EMAIL_BOXINFO_LIST_T *)EMA_UTILS_MALLOC(PNULL,sizeof(EMAIL_BOXINFO_LIST_T));
            EMA_UTILS_BREAKNULL(list_cur_ptr, recode, "EMAFmMgr_BoxOrderResult, list_cur_ptr is PNULL");
            EMA_UTILS_MEMSET(list_cur_ptr,0x00,sizeof(EMAIL_BOXINFO_LIST_T));
            
            box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
            GUILIST_SetMaxSelectedItem(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, DRAFT_MAX_EMAIL_NUMBER);
            
            for (index = 0; index < box_vector_num; index++)
            {
                if (TRUE == s_email_box_globle_p->is_marked_ptr[index])
                {
                    GUILIST_SetSelectedItem(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, (uint16)index, FALSE);
                    s_email_box_globle_p->is_marked_ptr[index] = FALSE;
                }
                
                MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, index, (void **)&email_header_ptr);
                if (PNULL != email_header_ptr)
                {
                    list_cur_ptr->email_header_ptr = (EMAIL_MSGHEAD_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGHEAD_T));
                    EMA_UTILS_BREAKNULL(list_cur_ptr->email_header_ptr, recode, "EMAFmMgr_BoxOrderResult, list_next_ptr->email_header_ptr is PNULL");
                    EMA_UTILS_MEMSET(list_cur_ptr->email_header_ptr, 0, sizeof(EMAIL_MSGHEAD_T));
                    MMIEMAIL_CopyEmailHead(list_cur_ptr->email_header_ptr, email_header_ptr, PNULL);
                }
                if (list_cur_ptr->email_header_ptr->dwdate < mail_date)
                {
                    GUILIST_SetTextListMarkable(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, TRUE);
                    GUILIST_SetSelectedItem(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID, (uint16)index, TRUE);
                    s_email_box_globle_p->is_marked_ptr[index] = TRUE;
                    marked_num++;
                }
            }
            
            result = ClearBoxInfoList(PNULL, &list_cur_ptr);
            EMA_UTILS_BREAKIF(result, "EMA_ClearBoxInfoList,failed!!!!");
            
            if (0 == marked_num)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, PNULL, TXT_EMAIL_MARK_BY_TIME_NOMAIL, IMAGE_PUBWIN_WARNING, PNULL, PNULL,
                    MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;
        
    case EVT_EMA_RESEND:
        if (param)
        {
            net_id= *((uint32*)param);
        }
        selected_index = GUILIST_GetCurItemIndex(ctrl_id);
        s_email_box_globle_p->selected_index = selected_index;
        if(!PublicProcSending(s_email_box_globle_p->email_id_ptr[selected_index], net_id))
        {
            EMA_UTILS_LOG_ERROR(("HandleDraftBoxChildWinMsg PublicProcSending failed."));
        }
        break;
        
    case EVT_EMA_NOSUBJECT_SEND:
        {
            uint32 emailid = 0;
            emailid = s_email_box_globle_p->email_id;
            MMIEMAIL_ClearEmailCache(&s_email_box_globle_p->email_datacache);
            MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), emailid, EMA_SP_ACTIVE_ACCOUNT, &s_email_box_globle_p->email_datacache);
            MMIEMAIL_GetAllRecipients(PNULL, &s_email_box_globle_p->email_datacache);
            g_email_is_send = EMA_TRANS_TYPE_SEND_EMAIL;
            MMIEMAIL_SelectNet();
        }
        break;
        
#if defined(MMI_PDA_SUPPORT)
    case EVT_EMA_MARK_CANCEL:
        {
            EmailBoxUnMarkAll();
            
            GUILIST_SetTextListMarkable(ctrl_id, FALSE);
            GUIFORM_SetChildDisplay(form_ctrl_id, MMICOMMON_SOFTKEY_CTRL_ID, GUIFORM_CHILD_DISP_HIDE);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0); //刷新窗口
            }
        }
        break;
#endif
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/**************************************************************************************/
// Description : Get the focus child win id.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_WIN_ID_T GetCurFocusChildBoxWinId(void)
{
    MMI_WIN_ID_T cur_child_winid = 0;
    
    switch(s_email_box_globle_p->box_reason_type)
    {
    case MMIEMAIL_BOX_FROM_INBOX:
        cur_child_winid = EMAIL_INBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_OUTBOX:
        cur_child_winid = EMAIL_OUTBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_SENTBOX:
        cur_child_winid = EMAIL_SENTBOX_CHILD_WIN_ID;
        break;
    case MMIEMAIL_BOX_FROM_DRAFT:
        cur_child_winid = EMAIL_DRAFTBOX_CHILD_WIN_ID;
        break;
    default:
        break;
    }
    
    return cur_child_winid;
}

/**************************************************************************************/
// Description : 根据box_type生成控件list;
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN LoadEmailListFromOrder(EMAIL_BOXTYPE_E box_type, MMI_CTRL_ID_T ctrl_id)
{
    BOX_ID box_id = (BOX_ID)box_type;
    uint32 email_num = 0;
    
    if ((EMA_INBOX == box_type && MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
        ||(EMA_DRAFTBOX == box_type && MMIEMAIL_BOX_FROM_DRAFT == s_email_box_globle_p->box_reason_type)
        ||(EMA_OUTBOX == box_type && MMIEMAIL_BOX_FROM_OUTBOX == s_email_box_globle_p->box_reason_type)
        ||(EMA_SENTBOX == box_type && MMIEMAIL_BOX_FROM_SENTBOX == s_email_box_globle_p->box_reason_type))
    {
        if (!MMIEMAIL_GetEmailNum(box_id, (uint32)&email_num))
        {
            EMA_UTILS_LOG_ERROR(("LoadEmailListFromOrder, get email num failed!!!"));
        }
    }
    
    EMA_UTILS_TRACE("LoadEmailListFromOrder the count of list box is %d", email_num); 
    if (email_num > 0)
    {
        ListEmailFromOrder(box_type, ctrl_id);
        return TRUE;
    }
    else
    {
        GUILIST_RemoveAllItems(ctrl_id);
        return FALSE;
    }
}

/**************************************************************************************/
// Description : to list email box the list buffer
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void ListEmailFromOrder(EMAIL_BOXTYPE_E box_type, MMI_CTRL_ID_T ctrl_id)
{
    switch(box_type)
    {
    case EMA_INBOX:
        GUILIST_RemoveAllItems(EMAIL_INBOX_LISTBOX_CTRL_ID);
        UpdateBoxData(box_type);
        break;
        
    case EMA_OUTBOX:
        GUILIST_RemoveAllItems(EMAIL_OUTBOX_LISTBOX_CTRL_ID);
        UpdateBoxData(box_type);
        break;
        
    case EMA_SENTBOX:
        GUILIST_RemoveAllItems(EMAIL_SENTBOX_LISTBOX_CTRL_ID);
        UpdateBoxData(box_type);
        break;
        
    case EMA_DRAFTBOX:
        GUILIST_RemoveAllItems(EMAIL_DRAFTBOX_LISTBOX_CTRL_ID);
        UpdateBoxData(box_type);
        break;
        
    default:
        EMA_UTILS_TRACE ("EMAIL_ListEmailFromOrder: the box_type is %d", box_type);
        break;
    }
}

/**************************************************************************************/
// Description : to set the global CurOrderId
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetCurType(
                      EMAIL_BOXTYPE_E box_type
                      )
{
    s_email_box_globle_p->cur_type = box_type;
}

/**************************************************************************************/
// Description : to set the global CurOrderId
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
LOCAL EMAIL_BOXTYPE_E GetCurType(
                                 void
                                 )
{
    return s_email_box_globle_p->cur_type;
}

/**************************************************************************************/
// Description : append list item data
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
PUBLIC  void SetListItemData(
                             EMAIL_BOXTYPE_E   box_type, 
                             MMI_CTRL_ID_T     ctrl_id,
                             uint16            index
                             )
{
//    //GUILIST_ITEM_T item_info = {0};
//    //GUILIST_ITEM_DATA_T item_data = {0};
//    MMISMS_ORDER_ID_T current_order_id = PNULL;
//    MMISMS_STATE_T sms_order_info ={0};
//    
//    current_order_id = MMISMS_GetLinkedArrayPtrIdInList(box_type, index); 
//    
//    if (current_order_id != PNULL)
//    {  
//       MMISMS_GetSmsInfoFromOrder(current_order_id, &sms_order_info);        
//       //EMA_UTILS_MEMSET(&item_info, 0, sizeof(GUILIST_ITEM_T)); 
//       
//       FormListBoxItem(            
//          &sms_order_info,
//          ctrl_id,
//          index
//          );        
//       
//       if (sms_order_info.flag.is_marked)
//       {
//          GUILIST_SetTextListMarkable(ctrl_id, TRUE);
//          GUILIST_SetSelectedItem(ctrl_id, index, TRUE);
//       }
//    }
//    else
//    {
//       if(MMISMS_BOX_SECURITY == box_type)
//       {
//          MMISMS_SetEmptyStr2List(ctrl_id, TXT_SMS_EMPTY, STXT_OPTION);    
//       }
//       else
//       {
//          MMISMS_SetEmptyStr2List(ctrl_id, TXT_SMS_EMPTY, TXT_NULL);    
//       }       
//    }
}
/**************************************************************************************/
// Description : set email box list item is grayed 
// Global resource dependence : s_email_box_globle_p
// Author: qiang.zhang
// Note:
/**************************************************************************************/
LOCAL void SetListItemGrayed(MMI_CTRL_ID_T     ctrl_id)
{
    EMAIL_MSGHEAD_T  *email_header_ptr = PNULL;
    
    MMIEMAIL_VectorGetAt(s_email_box_globle_p->box_vector_ptr, s_email_box_globle_p->selected_index, (void **)&email_header_ptr);
    
    EMA_UTILS_LOG_INFO(("HandleBoxMenuWinMsg email_header_ptr->mail_status = %d", email_header_ptr->mail_status));
    if (EMA_BASIC_UNREAD == email_header_ptr->mail_status
        || EMA_BASIC_UNREAD_HEAD_ONLY == email_header_ptr->mail_status
        || EMA_UNREAD_WITH_ATTACH == email_header_ptr->mail_status
        || EMA_UNREAD_AND_FORWARDED == email_header_ptr->mail_status
        || EMA_UNREAD_WITH_ATTACH_AND_FORWARDED == email_header_ptr->mail_status
        || EMA_UNREAD_AND_REPLIED == email_header_ptr->mail_status
        || EMA_UNREAD_WITH_ATTACH_AND_REPLIED == email_header_ptr->mail_status)
    {
        GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASUNREAD, TRUE);
        GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASREAD, FALSE);
    }
    else if(EMA_BASIC_READ == email_header_ptr->mail_status
        || EMA_BASIC_READ_HEAD_ONLY == email_header_ptr->mail_status
        || EMA_READ_WITH_ATTACH == email_header_ptr->mail_status
        || EMA_READ_AND_FORWARDED == email_header_ptr->mail_status
        || EMA_READ_WITH_ATTACH_AND_FORWARDED == email_header_ptr->mail_status
        || EMA_READ_AND_REPLIED == email_header_ptr->mail_status
        || EMA_READ_WITH_ATTACH_AND_REPLIED == email_header_ptr->mail_status)
    {
        GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASREAD, TRUE);
        GUIMENU_SetItemGrayed(ctrl_id, MENU_EMAIL_SET_STATUS_OPT, EMAIL_INBOX_OPT_MARKASUNREAD, FALSE);
    }
}
/**************************************************************************************/
// Description : mark all email
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetAllEmailMarked(
                             EMAIL_BOXTYPE_E   box_type,
                             BOOLEAN           is_marked //IN:
                             )
{
//     MMISMS_ORDER_ID_T  cur_order_id = PNULL;
//     MMISMS_ORDER_ID_T  cur_long_order_id = PNULL;
    
    switch(box_type) 
    {
//     case MMISMS_BOX_MT:
//         cur_order_id =s_mmisms_order.mt_undownloaded_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         cur_order_id =s_mmisms_order.mt_unread_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         cur_order_id =s_mmisms_order.mt_readed_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         break;
//         
//     case MMISMS_BOX_SENDSUCC:
//         cur_order_id =s_mmisms_order.mo_sendsucc_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         break;
//         
//     case MMISMS_BOX_SENDFAIL:
//         cur_order_id =s_mmisms_order.mo_sendfail_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         break;
//         
//     case MMISMS_BOX_NOSEND:
//         cur_order_id =s_mmisms_order.mo_draft_head_ptr;
//         while (cur_order_id != PNULL)
//         {
//             cur_long_order_id = cur_order_id;
//             while (cur_long_order_id != PNULL)
//             {
//                 cur_long_order_id->flag.is_marked = is_marked;
//                 cur_long_order_id = cur_long_order_id->next_long_sms_ptr;
//             }
//             cur_order_id = cur_order_id->next_ptr;
//         }
//         break;
        
    default:
        break;
    }
}

/**************************************************************************************/
// Description : 是否需要重新排序
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN IsNeedSort(void)
{
    return s_email_box_globle_p->is_need_sort;
}

/**************************************************************************************/
// Description : to get email number by box vector
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_GetEmailNum(uint32 wParam, uint32 dwParam)
{
    EMAIL_VECTOR_T *box_vector_ptr = PNULL;
    uint32         vector_num = 0; 
    
    if (EMAIL_SUCCESS != MMIEMAIL_SP_Store_GetBoxInfo(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMA_SP_ACTIVE_ACCOUNT, wParam, &box_vector_ptr))
    {
        EMA_UTILS_LOG_ERROR(("EmailAUIC_Boxs_GetMsgNum, MMIEMAIL_SP_Store_GetBoxInfo failed."));
        return FALSE;
    }
    if (PNULL != box_vector_ptr )
    {
        vector_num = MMIEMAIL_VectorGetVectorNumber(box_vector_ptr);
        if (vector_num > 0)
        {
            *(uint32*)dwParam = vector_num;
        }
    }
    MMIEMAIL_VectorDelete(box_vector_ptr);
    return TRUE;
}

/**************************************************************************************/
// Description : to get box vector and select index
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
PUBLIC void MMIEMAIL_GetVectorinfoAndSelectIndex(uint32 *puiindex, EMAIL_VECTOR_T **ppivetor)
{
    MMI_CTRL_ID_T cur_ctrl_id = 0;
    
    *ppivetor = s_email_box_globle_p->box_vector_ptr;
    
    if (MMIEMAIL_BOX_FROM_INBOX == s_email_box_globle_p->box_reason_type)
    {
        cur_ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
    }
    else if (MMIEMAIL_BOX_FROM_OUTBOX == s_email_box_globle_p->box_reason_type)
    {
        cur_ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
    }
    else if (MMIEMAIL_BOX_FROM_SENTBOX == s_email_box_globle_p->box_reason_type)
    {
        cur_ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
    }
    
    *puiindex = GUILIST_GetCurItemIndex(cur_ctrl_id);
}

/**************************************************************************************/
// Description : to check EMAIL_MSGDATACACHE_T
// Global resource dependence : s_email_box_globle_p
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN BoxCheckDatecach(EMAIL_MSGDATACACHE_T *email_datacache_ptr)
{
    MMI_STRING_T receipient_str = {0};
    int          result = 0;
    
    if (email_datacache_ptr->to_addrs_num > 0)
    {
        MMIEMAIL_GetEditRecipients(&email_datacache_ptr->to_addrs[0][0], &receipient_str.wstr_ptr,
            PNULL, email_datacache_ptr->to_addrs_num);
        receipient_str.wstr_len = EMA_UTILS_WSTRLEN(receipient_str.wstr_ptr);
        
        result = MMIEMAIL_EditCheckFieldLegality(receipient_str.wstr_ptr);
        if (MMIEMAIL_ERROR_INVALID_ADDR == result)
        {
            MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
            return FALSE;
        }
    }
    
    if (email_datacache_ptr->cc_addrs_num > 0)
    {
        MMIEMAIL_GetEditRecipients(&email_datacache_ptr->cc_addrs[0][0], &receipient_str.wstr_ptr,
            PNULL, email_datacache_ptr->cc_addrs_num);
        receipient_str.wstr_len = EMA_UTILS_WSTRLEN(receipient_str.wstr_ptr);
        
        result = MMIEMAIL_EditCheckFieldLegality(receipient_str.wstr_ptr);
        if (MMIEMAIL_ERROR_INVALID_ADDR == result)
        {
            MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
            return FALSE;
        }
    }
    
    if (email_datacache_ptr->bcc_addrs_num > 0)
    {
        MMIEMAIL_GetEditRecipients(&email_datacache_ptr->bcc_addrs[0][0], &receipient_str.wstr_ptr,
            PNULL, email_datacache_ptr->bcc_addrs_num);
        receipient_str.wstr_len = EMA_UTILS_WSTRLEN(receipient_str.wstr_ptr);
        
        result = MMIEMAIL_EditCheckFieldLegality(receipient_str.wstr_ptr);
        if (MMIEMAIL_ERROR_INVALID_ADDR == result)
        {
            MMIPUB_OpenAlertWarningWin( TXT_COMMON_MAIL_ADDR_INVALID);
            return FALSE;
        }
    }
    
    if (0 == EMA_UTILS_WSTRLEN(email_datacache_ptr->subject_ptr))
    {
        MMIEMAIL_OpenQueryWinByID(TXT_EMAIL_MUST_SET_SUBJECT, EMAIL_NOSUBJECT_QUERY_WIN_ID_EX);
        return FALSE;
    }
    
    return TRUE;
}

/**************************************************************************************/
// Description : to set the forword's option.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void SetForwordOpt(
                         MMI_CTRL_ID_T ctrl_id,
                         MMI_MENU_GROUP_ID_T group_id,
                         EMAIL_MSGDATACACHE_T *email_datacache_ptr
                         )
{
    if (email_datacache_ptr->attach_num < 1)
    {
        GUIMENU_SetItemVisible(ctrl_id, group_id, EMAIL_INBOX_OPT_FORWORD_ATTACH, FALSE);
    }
    else
    {
        GUIMENU_SetItemVisible(ctrl_id, group_id, EMAIL_INBOX_OPT_FORWORD, FALSE);
    }
}

/**************************************************************************************/
// Description : handle to set date and time.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL MMI_RESULT_E  HandleSetDateTimeWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param
                                            )
{
    MMI_RESULT_E   err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id  = EMAIL_SET_DATE_CTRL_ID;
    MMISET_DATE_T  date = {0};
    uint32         mail_date = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_PDA_SUPPORT
        GUIFORM_IsSlide(EMAIL_SET_DATE_FORM_CTRL_ID,FALSE);
#endif
        CreateSetDateTimeWin(ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        GUIEDIT_GetDate(ctrl_id, 
            &date.date_year,
            &date.date_month,
            &date.date_day);
#ifdef CALENDAR_SUPPORT
        if (MMICALENDAR_IsDateValid(date.date_year, date.date_month, date.date_day))
        {
            mail_date = MMIAPICOM_Tm2Second(
                (uint32)0, 
                (uint32)0, 
                (uint32)0, 
                (uint32)date.date_day,
                (uint32)date.date_month,
                (uint32)date.date_year);
            
            MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_MARKED_BY_DATE, &mail_date);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_INVALID_DATE);
            break;
        }
#endif
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(win_id);
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/**************************************************************************************/
// Description : creat a win can set date and time to use.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL void CreateSetDateTimeWin(MMI_CTRL_ID_T ctrl_id)
{
    SCI_DATE_T            sys_date = {0};
#ifndef MMI_PDA_SUPPORT
    MMI_STRING_T title = {0};
    
    MMI_GetLabelTextByLang(TXT_EMAIL_YEAR_MON_AND_DAY, &title);
    GUILABEL_SetText(EMAIL_SET_DATE_LABLE_CTRL_ID, &title, FALSE); 
#endif
    
    
    GUIEDIT_SetFont(ctrl_id, MMI_SET_DATE_FONT);
    TM_GetSysDate(&sys_date);
    GUIEDIT_SetDate(ctrl_id,sys_date.year,sys_date.mon,sys_date.mday);
}

/**************************************************************************************/
// Description : to update draftbox's e-mail read state.
// Global resource dependence : 
// Author: 
// Note:
/**************************************************************************************/
LOCAL BOOLEAN UpdateDraftReadState(uint32 email_id)
{
    int              result = EMAIL_ERROR;
    EMAIL_MSGDATACACHE_T *date_chche_ptr = PNULL;
    
    do 
    {
        date_chche_ptr = (EMAIL_MSGDATACACHE_T *)EMA_UTILS_MALLOC(PNULL, sizeof(EMAIL_MSGDATACACHE_T));
        EMA_UTILS_BREAKNULL(date_chche_ptr, result, "UpdateDraftReadState, date_chche_ptr is PNULL.");
        EMA_UTILS_MEMSET(date_chche_ptr, 0, sizeof(EMAIL_MSGDATACACHE_T));
        
        result = MMIEMAIL_SP_Store_GetEmailData(EMA_GETSTORE_HANDLE(g_email_sp_handle), email_id, EMA_SP_ACTIVE_ACCOUNT, date_chche_ptr);
        if (EMAIL_SUCCESS != result)
        {
            MMI_WIN_ID_T query_win_id = EMAIL_DELETE_DLG_ONE;
            
            MMIPUB_OpenQueryWinByTextId(TXT_EMAIL_DATA_ERROR, IMAGE_PUBWIN_QUERY, &query_win_id, MMIEMAIL_DelDlg_HandleEvent);
            return FALSE;
        }//add by wuhui
        
        /*Update e-mail read state.*/
        if (EMA_BASIC_DRAFT == date_chche_ptr->mail_status)
        {
            date_chche_ptr->mail_status = EMA_BASIC_DRAFT_READ;
        } 
        else if (EMA_BASIC_DRAFT_WITH_ATTACH == date_chche_ptr->mail_status)
        {
            date_chche_ptr->mail_status = EMA_BASIC_DRAFT_WITH_ATTACH_READ;
        }
        
        result = MMIEMAIL_SP_Store_UpdateEmail(EMA_GETSTORE_HANDLE(g_email_sp_handle), EMAILA_MSGMASK_STATE, date_chche_ptr);
        EMA_UTILS_BREAKIF(result, "UpdateDraftReadState, MMIEMAIL_SP_Store_UpdateEmail failed.");
        MMK_SendMsg(EMAIL_BOX_WIN_ID, EVT_EMA_UPDATE_LISTBOX_TAB, PNULL);
        MMK_SendMsg(EMAIL_MAIN_MENU_WIN_ID, EVT_EMA_UPDATE_MAIN_NUM, PNULL);
        
        /*Mark read on server*/
        // 		 if (E_EMAILA_MAILSERV_IMAP4 == EmailASP_CFG_ActRecType(EMA_GETCFG_HANDLE(pStUICtrlMgr->g_email_sp_handle)))
        // 			{
        // 				result = EmailASP_TRS_Imap4Mark(EMA_GETTRANS_HANDLE(g_email_sp_handle), 
        // 					EMA_IMAP4_MARK_SEEN_ON_SERVER,
        // 					EMA_SP_ACTIVE_ACCOUNT,
        // 					msg_id);
        // 				EMA_UTILS_BREAKIF(result, "EmailAUIC_PublicProc_DelEmail, EmailASP_TRS_Imap4Mark failed.");
        // 
        // 			}
    } while(0);
    
    MMIEMAIL_ReleaseEmailDataCache(PNULL, date_chche_ptr);
    
    if (EMAIL_SUCCESS != result)
    {
        return FALSE;
    }
    
    return TRUE;
}

#if defined(MMI_PDA_SUPPORT)
/**************************************************************************************/
// Description : common func for mail box mark.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL void EmailBoxMark(void)
{         
    uint16                 cur_index   = 0;
    MMI_CTRL_ID_T ctrl_id = GetEmailBoxListCtrlId();
    
    cur_index = GUILIST_GetCurItemIndex(ctrl_id);
    if (!s_email_box_globle_p->is_marked_ptr[cur_index])
    {
        GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
        s_email_box_globle_p->is_marked_ptr[cur_index] = TRUE;
    }
    else
    {
        GUILIST_SetSelectedItem(ctrl_id, cur_index, FALSE);
        s_email_box_globle_p->is_marked_ptr[cur_index] = FALSE;
    }
}

/**************************************************************************************/
// Description : common func for mail box unmark all.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL void EmailBoxUnMarkAll(void)
{
    uint16                 cur_index   = 0;
    MMI_CTRL_ID_T ctrl_id = GetEmailBoxListCtrlId();
    uint32 box_vector_num = 0;
    
    box_vector_num = MMIEMAIL_VectorGetVectorNumber(s_email_box_globle_p->box_vector_ptr);
    
    for (cur_index = 0; cur_index < box_vector_num; cur_index++)
    {
        GUILIST_SetSelectedItem(ctrl_id, cur_index, FALSE);
        s_email_box_globle_p->is_marked_ptr[cur_index] = FALSE;
    }
}

/**************************************************************************************/
// Description : common func for get list ctrl id.
// Global resource dependence : 
// Author:tao.xue 
// Note:
/**************************************************************************************/
LOCAL MMI_CTRL_ID_T GetEmailBoxListCtrlId(void)
{
    MMI_CTRL_ID_T ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
    
    switch (s_email_box_globle_p->box_reason_type)
    {
    case MMIEMAIL_BOX_FROM_INBOX:
        ctrl_id = EMAIL_INBOX_LISTBOX_CTRL_ID;
        break;
    case MMIEMAIL_BOX_FROM_OUTBOX:
        ctrl_id = EMAIL_OUTBOX_LISTBOX_CTRL_ID;
        break;
    case MMIEMAIL_BOX_FROM_SENTBOX:
        ctrl_id = EMAIL_SENTBOX_LISTBOX_CTRL_ID;
        break;
    case MMIEMAIL_BOX_FROM_DRAFT:
        ctrl_id = EMAIL_DRAFTBOX_LISTBOX_CTRL_ID;
        break;
    default :
        break;
    }
    
    return ctrl_id;
}
#endif


/*Edit by script, ignore 9 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
