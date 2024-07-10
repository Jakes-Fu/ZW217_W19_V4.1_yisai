/****************************************************************************
** File Name:      mmisms_chatreadwin.c                                    *
** Author:         liming deng                                             *
** Date:           1/4/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe deleting SMS.              *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 01/2012        minghu.mao       Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef MMI_SMS_CHAT_SUPPORT

#include "mmisms_chatreadwin.h"
#include "mmisms_read.h"
#if defined(MMI_VCARD_SUPPORT)
#include "mmivcard_export.h"
#endif

#if defined(IM_ENGINE_SOGOU) && defined(IM_SIMP_CHINESE_SUPPORT)
//#include "mmiset.h"
#include "mmiset_export.h"
#include "wdp_update_wintab.h"
#endif
#include "mmiidle_statusbar.h"

#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmimms_file.h"
#include "mmimms_edit.h"
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
#include "guibutton.h"
#endif
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#include "mmisrvaud_api.h"
#endif
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_anim.h"
#include "mmisms_id.h"
#include "guimenu.h"
#include "guilistbox.h"
#include "guitext.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmiidle_export.h"
#include "mmicc_export.h"
#include "tb_dal.h"
#include "guilabel.h"
#include "mmipb_export.h"
#include "guimsgbox.h"
#include "mmimms_export.h"
#include "mmimms_id.h"
#include "mmimms_text.h"
#include "mmi_appmsg.h"
#include "mmisms_menutable.h"
#include "mmipub.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmisms_commonui.h"
#include "guiurl.h"
#include "mmi_pubmenuwin.h"
#include "guiform.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmienvset_export.h"
#include "mmidc_export.h"
#include "mmicl_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmiudisk_export.h"
#include "mmiota_id.h"
#include "guirichtext.h"

#include "mmisms_set.h"
#include "mmisms_send.h"
#include "mmisms_edit.h"
#include "mmisms_appcontrol.h"
#include "mmisms_delete.h"
#include "mmialarm_export.h"
#include "mmisms_receive.h"

//#include "mmifmm.h"
#include "mmifmm_export.h"
#include "mmismsapp_interface.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//BEGIN for delete all msg 11/02/18

#define MMISMS_SENDFAIL_DATE_TEXT_WIDTH   192
#define MMISMS_TEXT_LAYOUT_MARGIN_WIDTH   20

#define MMISMS_SMS_DATE_WIDTH   100
#define MMISMS_SMS_MIN_TEXT_BG_WIDTH   40

#define MMISMS_CHATREAD_TEXT_MARGIN_LEFT  15
#define MMISMS_CHATREAD_TEXT_MARGIN_RIGHT 5
#define MMISMS_CHATRAED_TEXT_MARGIN_TOP  8
#define MMISMS_CHATREAD_TEXT_MARGIN_BOTTOM  8

#define MMISMS_PROMPT_STR_LEN           52
#if 0
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#define MMISMS_DELMSG_BOX_ITEM_NUM      6
#else
#define MMISMS_DELMSG_BOX_ITEM_NUM      5
#endif

#define MMISMS_DELMSG_TYPE_ITEM_NUM     4
#define MMISMS_DELMSG_MSGNUM_STR_LEN    10 
#define MMISMS_ONE_K_LEN                1024
#define MMISMS_SIZE_FLOAT_A             0.100000
#define MMISMS_SIZE_FLOAT_B             0.000000
#define MMISMS_MEMSTATE_DELMSG_MAX_LEN  30
#endif
//END
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    MMI_WIN_ID_T            win_id;
    uint32                  menu_id;
    MMISMS_CHAT_ITEM_T      chat_item[MMISMS_CHAT_MAX_ITEM];
    //MMI_QUEUEHANDLE_T       operation_queue;
    MMISMS_BCD_NUMBER_T     chat_number;
    uint8                   sender[MMISMS_NUMBER_MAX_LEN+1];//added for mms...
    MMISMS_SMSEDIT_CONTENT_T edit_content;
    wchar         edit_string[MMISMS_MAX_MESSAGE_LEN + 2];    //edit_string
    uint32        edit_strlen;
    MN_DUAL_SYS_E   dual_sys;
    MMISMS_READ_MSG_T   read_result;
    MMISMS_ORDER_ID_T       chat_del_order_id;
    uint16                  del_index;
    BOOLEAN         is_resent_cmd;
    BOOLEAN         is_email_number;
#ifdef MMIMMS_SMS_IN_1_SUPPORT    
    MMISMS_CHAT_INSERT_GLOBLE_T sms_chat_insert_info;
#endif
    
    BOOLEAN is_del_all;   
    BOOLEAN is_add_contact;
    BOOLEAN is_have_item;
#ifdef SNS_SUPPORT
    IMMISns *pMe;
#endif
   
}MMISMS_CHATREAD_WINDATA_T;

/**--------------------------------------------------------------------------*
**                         EXTERN DECLARE                                   *
**--------------------------------------------------------------------------*/
/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//MMISMS_DELMSG_GLOBLE_T g_sms_delmsg_all_info = {0};


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//LOCAL uint16 s_all_del_msg_num = 0;
//LOCAL BOOLEAN s_mms_is_sendrecv = FALSE;
//LOCAL BOOLEAN s_is_mms_del = FALSE;
/**---------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                                *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

#ifdef MMI_SMS_MOVE_SUPPORT

/*****************************************************************************/
//  Description : to handle the message of move query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenMoveQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveFinishCallback(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr);

#endif


#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinMoveSecurity(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinResendItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetChatItemDateWidth(void);

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_INDEX_E GetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 *index_ptr,
    MMISMS_ORDER_ID_T order_id
);
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptOK(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : truncate first line of msg content
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TruncateMsgContent(MMI_STRING_T *msg_content_ptr );

#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif
#endif

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : open the SMS copy query window
//  Global resource dependence :
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenCopyQuery(void);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyFinishCallback(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr);

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//  Description : to operate the copy
//  Global resource dependence :
//  Author:feingming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOperateCopy(void);

/*****************************************************************************/
//  Description : to handle the message of copy waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopySimToMe(void);

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopyMeToSim(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleChatWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinShareSNS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinReadTTS(void);
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinInsertOptOK(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : speed dial edit select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EditSelectVcardCallback(MMIPB_HANDLE_T handle);

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description :mms edit insert video
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertVideo(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertMusic(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertPic(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertNormalAttachment(MMI_WIN_ID_T pre_win_id);

/*****************************************************************************/
//  Description :聊天处理现拍,现摄,现录流程
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSHandleSnapshot(MMIFILE_FILE_INFO_T *file_info, MMIFMM_FILE_TYPE_E   file_type);

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenRecord(void);

/*****************************************************************************/
//  Description :录音 后 的 处 理
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void SMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr);

#endif

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDC(void);

/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDV(void);

/*****************************************************************************/
//  Description :拍 照 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr);

/*****************************************************************************/
//  Description :录 像 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr);
#endif

/*****************************************************************************/
//  Description : 处理阅读界面来一条相同号码的情况
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ChatWinSetLongSmsItemContent(
                                               MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               );

/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void ChatWinCalcChatItemSplitLenth(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,uint32  index);

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTag(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
);
#endif

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param );
#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenInsertOpt(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif
/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void ChatWinCloseCtrlFunc (MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : opertate after del single chat item
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void UpdateChatItemDisplayAfterDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);


#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description :update readed mms icon 
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_UpdateReadedMMSIcon(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description :create dyna child form for mms
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void SetMMSChatItemParam(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);

#endif

#if 0
/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
);
#endif

/*****************************************************************************/
//  Description : to get phone number
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSGetPhoneNumber(uint8 *number_ptr, uint16 src_len, wchar *focus_num_ptr, uint16 dest_len);

/*****************************************************************************/
//  Description :set chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 index,
    BOOLEAN is_active,
    MMISMS_ORDER_ID_T order_id
);

#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description : 获取彩信的相关信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_INFO_T *ReadMMSContent(int32 record_id);

/*****************************************************************************/
//  Description : 获取彩信相关部分信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void GetMMSTimeSujectSize(MMI_STRING_T *string_ptr, MMI_STRING_T  *time_str, MMISMS_ORDER_INFO_T *mms_display_info);
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelboxAll(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCallItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBNew(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBExist(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBBlackList(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);
#endif

/*****************************************************************************/
//     Description : to delete all mared message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinDelAllMarkedMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinForwardMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinLockOrUnLockMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
);

/*****************************************************************************/
//  Description : 号码添加到联系人后需要更新显示
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatReadWinUpdateDispNum(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : 获取聊天显示窗口中某条信息的order_id
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetChatWinItemId(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//  Description : to handle the message enable send
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsSendEnable(BOOLEAN is_update);

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendSms(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinOpenDelteQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDelQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
);

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerSms(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerMMS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr);

/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptInit(MMIPUB_MENU_NOTIFY_T *param );

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptOK(MMIPUB_MENU_NOTIFY_T *param );

/******************************************************************************
//  Description : Update Windata,and save it to global variables.
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMI_HANDLE_T win_id,BOOLEAN is_resend);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN ChatWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data);

/*****************************************************************************/
//     Description : get chat win w number,such as :10086, email@163.com
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetChatWinWNumber(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,wchar *wnumber);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
#ifdef MMIMMS_SMS_IN_1_SUPPORT
WINDOW_TABLE( MMISMS_SHOW_CHAT_SMS_WIN_TAB ) =
{
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif

    WIN_FUNC((uint32)HandleShowChatSmsWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_WIN_ID),
    WIN_STYLE(WS_DISABLE_SOFTKEY_TO_TITLE),
    WIN_TITLE(TXT_NULL),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),     
    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    //CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE,MMISMS_MAX_MESSAGE_LEN,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),

#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(TXT_COMM_INSERT, TXT_NULL, TXT_SEND, MMICOMMON_SOFTKEY_CTRL_ID, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif

    END_WIN
};
#else
WINDOW_TABLE( MMISMS_SHOW_CHAT_SMS_WIN_TAB ) =
{
#ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
#endif

    WIN_FUNC((uint32)HandleShowChatSmsWinMsg),
    WIN_ID(MMISMS_SHOW_CHAT_SMS_WIN_ID),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),

    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID),
    //CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
LOCAL BOOLEAN ChatWinReadSmsCallback(SMSSRV_CALLBACK_STRUCT_T *callback_data)
{
    MMISMS_ORDER_ID_T order_id = PNULL;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == callback_data)
    {
       return TRUE;
    }
    
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }

    if(MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))  //此窗口如已关闭，则win_data_ptr已被释放
    {
        win_data_ptr->read_result = MMISMS_GetReadResult();
        if(callback_data->result)
        {
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            order_id = MMISMS_GetChatOperOrderID();
            MMK_SendMsg(
                        (MMI_HANDLE_T)win_data_ptr->win_id,
                        MSG_SMS_FINISH_READ,
                        (ADD_DATA)order_id               
                       ); 
        }
        else
        {
            MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
            MMISMS_ClearOperInfo();
        }  
    }
      
    return TRUE;
}


/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL void SMS_SAVE(MN_DUAL_SYS_E dual_sys, MMISMS_CHATREAD_WINDATA_T * windata_p)
{
    SMS_SAVE_OPERATE_DATA_T op_data={0};

    op_data.dual_sys = dual_sys;
    op_data.is_needLoadSaved = FALSE;
    op_data.sms_addr = &windata_p->edit_content.dest_info;
    op_data.save_strlen = windata_p->edit_strlen;
    op_data.save_string_ptr = windata_p->edit_string;

    MMISMSAPP_SaveSms(op_data,PNULL,PNULL,PNULL);
}


/*****************************************************************************/
//  Description : SEND SMS. should be replaced by app control api later
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/\
LOCAL void SMS_SEND(MN_DUAL_SYS_E dual_sys, MMI_HANDLE_T  win_handle)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_handle);

    if (win_data_ptr == PNULL)
    {
        return;
    }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (!MMIAPICC_IsSimServiceAvail(dual_sys))
    {
        MMIAPICC_OpenSIMCallingPrompt();
        return;
    }
#endif

    MMISMSAPP_SendSmsWithData(dual_sys,
            win_data_ptr->is_resent_cmd,
            &win_data_ptr->edit_content.dest_info,
            win_data_ptr->edit_string,
            win_data_ptr->edit_strlen,
            PNULL
            );
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : select sim card callback
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  SendSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_HANDLE_T win_handle = (MMI_HANDLE_T)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    if (isReturnOK)
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(dual_sys))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return recode;
        }
#endif

        SMS_SEND(dual_sys,win_handle);
        //Don't need close chat read win
    }
    return recode;
}

#endif


/******************************************************************************
//  Description : Update Windata,and save it to global variables.
//  Global resource dependence :
//  Author:  sam.hua
//  Note:
 ******************************************************************************/
LOCAL void BuildSendData(MMI_HANDLE_T win_id,BOOLEAN is_resend_msg)
{
    MMI_STRING_T edit_string = {0};
    MMI_CTRL_ID_T ctrl_text_id = MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID;

    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);

    win_data_ptr->is_resent_cmd = is_resend_msg;
    SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
    GUIEDIT_GetString(ctrl_text_id, &edit_string);
    // to save the information
#ifdef SMS_AUTO_SIGNATURE_SUPPORT
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if (MMISMS_GetAutoSignatureFlag() && !MMIAPIMMS_GetIsMMSMode() &&!win_data_ptr->is_resent_cmd)
    {
        MMI_STRING_T sig_string = {0};
        MMI_STRING_T display_string = {0};
        wchar        signature_wstr[MMISMS_MAX_SIGNATURE_NUMBER_LEN + 2] = {0};
        uint16       signature_str_len = MMISMS_MAX_SIGNATURE_NUMBER_LEN;
#if defined(__BIG_ENDIAN) || defined(__BigEndian)
        signature_wstr[0] = 0x0a00;
#else
        signature_wstr[0] = 0x000a;
#endif
        signature_str_len = MMISMS_GetAutoSignatureContent(signature_wstr + 1) + 1;

        sig_string.wstr_ptr = signature_wstr;
        sig_string.wstr_len = signature_str_len;

        display_string.wstr_ptr = (wchar *)SCI_ALLOCAZ( ( MMISMS_MAX_MESSAGE_LEN + 1 ) * sizeof(wchar) );

        if (PNULL == display_string.wstr_ptr)
        {
            return;
        }

        MMIAPICOM_MergeTwoString(
            &edit_string,
            &sig_string,
            &display_string,
            MMISMS_MAX_MESSAGE_LEN
        );

        win_data_ptr->edit_strlen = display_string.wstr_len;
        SCI_MEMCPY(win_data_ptr->edit_string,display_string.wstr_ptr,sizeof(wchar)*win_data_ptr->edit_strlen);

        SCI_FREE(display_string.wstr_ptr);
    }
    else
#endif
#endif
    {
        SCI_MEMCPY(win_data_ptr->edit_string,edit_string.wstr_ptr,sizeof(wchar)*edit_string.wstr_len);
        win_data_ptr->edit_strlen = edit_string.wstr_len;

    }
    num_len = MMIAPICOM_GenDispNumber(
                  MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                  MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                  win_data_ptr->chat_number.number,
                  number,
                  MMISMS_PBNUM_MAX_LEN + 2
              );
    MMISMS_SetAddressToMessage((uint8 *)number, (uint8)strlen((char *)number), &win_data_ptr->edit_content.dest_info.dest_list);    
    MMISMS_SetMessageContent(win_data_ptr->edit_strlen, win_data_ptr->edit_string, TRUE, &win_data_ptr->edit_content);
              
}

/*****************************************************************************/
//     Description : handle mms edit send sms
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinSendSms(MMI_WIN_ID_T win_id)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(win_data_ptr->is_email_number)
    {
        MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        return;
    }
   // open the input number window
    if (0 < win_data_ptr->chat_number.number_len)
    {
        // send all message
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        uint16 sim_num = 0;
        uint16 sim_ok = 0;

        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        if (0 == sim_num)
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else if (1 == sim_num)
        {
            win_data_ptr->dual_sys = (MN_DUAL_SYS_E)sim_ok;

            SMS_SEND(win_data_ptr->dual_sys,win_id);
        }
        else
        {
            if (win_data_ptr->dual_sys == MN_DUAL_SYS_MAX)
            {
                MMIPB_BCD_NUMBER_T pb_num = {0};
                MN_DUAL_SYS_E cur_chat_sim_id = MMI_DUAL_SYS_MAX;
                MMISET_MAJOR_SIM_SEL_TYPE_E major_select_sim = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;

                pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                cur_chat_sim_id = MMISMS_GetCurChatSIMId(pb_num);

                major_select_sim = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_SMS);

                if (cur_chat_sim_id < MMI_DUAL_SYS_MAX
                    && MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_SMS))
                {
                    win_data_ptr->dual_sys = cur_chat_sim_id;
                }
                else if (major_select_sim < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
                {
                    win_data_ptr->dual_sys = MN_DUAL_SYS_1 + ((MN_DUAL_SYS_E)major_select_sim - MMISET_MAJOR_SIM_SEL_SIM1);
                }
                else
                {
#ifdef MMI_SMS_FIX_SEND_SIM
                    if (MMISMS_GetFixSIMSendingOnOFF())
                    {
                        win_data_ptr->dual_sys = (MN_DUAL_SYS_E)MMISMS_GetFixSIMSendingSIMSYS();
                    }
                    else
#endif
                    {                            
                        MMISMS_SelectSIMFunc(SendSelectSimCallback,(ADD_DATA)win_id,TRUE);
                        return;
                    }
                }
            }
            else
            {
                win_data_ptr->dual_sys = MMISMS_GetCurSendDualSys();
            }

            SMS_SEND(win_data_ptr->dual_sys,win_id);
        }
#else
        if (!MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_1))
        {
            MMIAPIPHONE_AlertSimNorOKStatus();
        }
        else
        {
            SMS_SEND(MN_DUAL_SYS_1,win_id);
        }
#endif
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ChatReadWinNewEventCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMISMS_NEW_SMS_INFO_T *new_sms_info_ptr = PNULL;
    MMIPB_BCD_NUMBER_T mt_num = {0};
    MN_SMS_MT_SMS_T *mt_sms_ptr = PNULL;
    BOOLEAN is_num_compare = FALSE;
    MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_MAX;    
    MN_SMS_STORAGE_E        storage = MN_SMS_NO_STORED;
    MN_SMS_RECORD_ID_T        record_id = 0;
    MMI_WIN_ID_T win_id = MMISMS_SHOW_CHAT_SMS_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
            
    if(PNULL == event_info_ptr || PNULL == win_data_ptr)
    {
        return;
    }
    new_sms_info_ptr = (MMISMS_NEW_SMS_INFO_T *)event_info_ptr->param_ptr;
    if(PNULL == new_sms_info_ptr)
    {
        SCI_TRACE_LOW("mmisms new_sms_info_ptr PNULL");
        return;
    }
    dual_sys = new_sms_info_ptr->dual_sys;
    storage = new_sms_info_ptr->storage;
    record_id = new_sms_info_ptr->record_id;
    
    mt_sms_ptr = &new_sms_info_ptr->mt_sms;
    mt_num.number_len = mt_sms_ptr->origin_addr_t.num_len;
    mt_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(mt_sms_ptr->origin_addr_t.number_type,MN_NUM_PLAN_UNKNOW);
    SCI_MEMCPY(
               mt_num.number,
               mt_sms_ptr->origin_addr_t.party_num,
               mt_num.number_len
               ); 

    if(0 == MMIAPIPB_BcdSmartCmp(&mt_num,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
    {
        is_num_compare = TRUE;
    }

    if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && is_num_compare
      )
    {               
        // to modify the status from to_be_read to READ
        MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);

        //MNSMS_ReadSmsEx(dual_sys, storage, record_id);
        MNSMS_UpdateSmsStatusEx(dual_sys, storage, record_id, MN_SMS_MT_READED);
        MMISMS_SetNeedUpdateInfo();
        if(new_sms_info_ptr->cur_chat_order_id)
        {
            new_sms_info_ptr->cur_chat_order_id->flag.is_need_update = TRUE;
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked() && MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
        {
            MMISMS_SetIsKeyLockNeedCountSms(TRUE);
        }
        else
#endif
        {
            MMISMS_SetIsKeyLockNeedCountSms(FALSE);
        }
        
    }
    else
    {
        MMISMS_SetIsKeyLockNeedCountSms(FALSE);
    }   
    if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
        && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
        && is_num_compare
      )
    {

        MMISMS_ChatSetMTGlobalReadMsg(
            dual_sys,
            &new_sms_info_ptr->user_data,    //IN:
            storage,        //IN:
            record_id,        //IN:
            mt_sms_ptr->dcs.alphabet_type,
            &mt_sms_ptr->time_stamp_t,
            &mt_sms_ptr->origin_addr_t,
            MMISMS_MT_HAVE_READ
            ); 

        win_data_ptr->read_result = MMISMS_GetReadResult();
        if((new_sms_info_ptr->cur_chat_order_id != PNULL) && ((new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms && new_sms_info_ptr->cur_chat_order_id->flag.is_receive_all)
            || (!new_sms_info_ptr->cur_chat_order_id->flag.is_concatenate_sms))
        )
        {
            MMIDEFAULT_TurnOnBackLight();
            // save the flag
            MMISMS_SetNewSMSRingFlag(TRUE);
            //MMISMS_SetSimIdOfNewMsg(dual_sys);
            MMISMS_PlayNewSMSRing(new_sms_info_ptr->dual_sys);
        }
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked() && MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID))
        {
            MMIAPIKL_SetNewSMSStatus();
            MMK_PostMsg(MMIKL_CLOCK_DISP_WIN_ID,MSG_KEYLOCK_UNREAD_MESSAGE,PNULL,0);
        }
#endif
        MMK_SendMsg(
                    MMISMS_SHOW_CHAT_SMS_WIN_ID,
                    MSG_SMS_NEW_SMS,
                    (ADD_DATA)new_sms_info_ptr->cur_chat_order_id
                    ); 
    }
      
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void ChatReadWinDataChangeCallback(MMIEVENT_INFO_T *event_info_ptr)
{
    MMI_WIN_ID_T win_id = MMISMS_SHOW_CHAT_SMS_WIN_ID;
    MMISMS_ORDER_ID_T chat_add_order_id = PNULL;
    MMISMS_DATACHANGE_TYPE_E *data_change_ptr = PNULL;    
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
    BOOLEAN is_num_compare = FALSE;
    if(PNULL == event_info_ptr)
    {
       SCI_TRACE_LOW("mmisms chatbox event ptr PNULL");
       return;
    }
    if(SMS_DATA_CHANGE_EVENT == event_info_ptr->event_type)
    {  
       data_change_ptr = (MMISMS_DATACHANGE_TYPE_E *)event_info_ptr->param_ptr;
       if(MMISMS_DATACHANGE_TYPE_ADD == *data_change_ptr) 
       {       
           chat_add_order_id = MMISMS_GetChatAddOrderID();
           if(chat_add_order_id != PNULL)
           {
               if(0 == MMIAPIPB_BcdSmartCmp((MMIPB_BCD_NUMBER_T *)&chat_add_order_id->number.bcd_number,(MMIPB_BCD_NUMBER_T *)&win_data_ptr->chat_number))
               {
                   is_num_compare = TRUE;
                   win_data_ptr->read_result = MMISMS_GetReadResult();
               }
                              
               if( !MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                   && MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID)
                   && is_num_compare
                   && (chat_add_order_id->flag.folder_type != MMISMS_FOLDER_SECURITY)
                 )
               {    
                   MMK_SendMsg(
                        win_id,
                        MSG_SMS_NEW_SMS,
                        (ADD_DATA)chat_add_order_id
                        );        
               }
               MMISMS_SetChatAddOrderID(PNULL);
           }
       }
       else if(MMISMS_DATACHANGE_TYPE_DELETE == *data_change_ptr)
       {
           MMK_SendMsg(win_id, MMISMS_MSG_UPDATELIST, PNULL);    //通知相应的窗口，更新list内容 
       }
    }      
}

/*****************************************************************************/
//  Description : to handle the message of show chat SMS window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleShowChatSmsWinMsg(
    MMI_WIN_ID_T        win_id,        //IN:
    MMI_MESSAGE_ID_E msg_id,        //IN:
    DPARAM            param        //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T name = {0};

    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint8 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMISMS_ORDER_ID_T oper_order_id = PNULL;
	MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)MMK_GetWinUserData(win_id);
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID;
    MULTIM_SELECT_RETURN_T *multim_return_info_ptr = PNULL;
    uint32 real_file_size                          = 0;
    uint32 insert_total_size                       = 0;
    GUIANIM_TYPE_E    pic_type                     = GUIANIM_TYPE_NONE;
    MMISMS_CHAT_INSERT_TYPE edit_type = MMISMS_INSERT_MUSIC;
    MMI_WIN_ID_T    quer_win_id = MMISMS_INSERT_PIC_QUERY_WIN_ID;
    MMI_STRING_T mms_content_str = {0};
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
    GUIEDIT_SetFont(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, MMISMS_GetWinFont());
#endif
#endif
    name.wstr_ptr = temp_arr;
    
    SCI_TRACE_LOW("MMISMS: HandleShowChatSmsWinMsg msg_id = 0x%x",msg_id);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr pnull");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:            
            cur_order_id = MMISMS_GetCurOperationOrderId();
            win_data_ptr->win_id = win_id;
            win_data_ptr->is_have_item = TRUE;
            MMISMS_GetNameInfo((MMISMS_STATE_T *)cur_order_id, &name, MMISMS_PBNAME_MAX_LEN);
            GUIWIN_SetTitleText(win_id, name.wstr_ptr, name.wstr_len, FALSE);
            SCI_MEMSET(&win_data_ptr->chat_item, 0x0, sizeof(MMISMS_CHAT_ITEM_T) * MMISMS_CHAT_MAX_ITEM);

            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
                                
            GUIFORM_PermitChildBg(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, FALSE);
            
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            GUIFORM_SetChildDock(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,GUIFORM_CHILD_DOCK_BOTTOM);
            GUIFORM_SetSizeRange(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,20, 100);
#endif

#ifdef MMI_GUI_STYLE_TYPICAL
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_OPTION, TXT_REPLY, STXT_RETURN, FALSE);
#else
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            GUIEDIT_SetSoftkey(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, 1, 1, TXT_NULL, TXT_SEND, PNULL);
#endif
#endif
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            if (cur_order_id != PNULL && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
            {
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};                                        
                MMIAPICOM_StrToWstr(cur_order_id->number.sender, wnumber);
                num_len = MMIAPICOM_Wstrlen(wnumber);
                if (URL_IsMailtoDefault((URL_CHAR_T const *)wnumber, (int32 *)&num_len))
                {
                    win_data_ptr->is_email_number = TRUE;
                }
   
                MMISMS_SetOperStatus(MMISMS_READ_CHAT_NUM_SMS);
                MMISMS_SetChatOperOrderID(cur_order_id);
                MMK_PostMsg(win_id,  MSG_MMS_FINISH_READ, PNULL, 0);
            }
            else if(cur_order_id != PNULL && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
            {
                SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T)); 
                if (!MMISMSAPP_ReadChatSms(cur_order_id,
                        ChatWinReadSmsCallback,
                        PNULL,
                        PNULL))
                {
                    MMK_CloseWin(win_id);
                    break;
                }        
            }
            
            
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT
            GUIEDIT_SetDefaultStringById(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, TXT_MMS_TOUCH_TO_INPUT);
#endif
            MMISMS_SetEditTextProperty(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
#endif
//////////////////////////////////////////////////////////////////////////
// fixed by feng.xiao for NEWMS00182269
// -add: compile swith for non-PDA ver
#ifdef MMI_PDA_SUPPORT
            HandleSmsSendEnable(FALSE);
#endif
            //MMK_SetAtvCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
            MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, TRUE); 
            MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, TRUE); 
            break;

        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
//////////////////////////////////////////////////////////////////////////
// fixed by feng.xiao for NEWMS00182269
// -add: compile swith for non-PDA ver
#ifdef MMI_PDA_SUPPORT
            HandleSmsSendEnable(TRUE);
#endif
            break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_UP:
            //case MSG_TP_PRESS_SHORT:
            // save the current msg_id
            //not slide
            if (!MMK_GET_TP_SLIDE(param))
            {
                GUI_RECT_T  form_rect = {0};
                MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
                MMI_CTRL_ID_T active_ctrl_id = 0;
                active_ctrl_id = MMK_GetActiveCtrlId(win_id);

                SCI_MEMCPY(&para, (MMI_MULTI_KEY_TP_MSG_PARAM_T*)param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
                GUICTRL_GetRect( MMK_GetCtrlPtr(active_ctrl_id), &form_rect);
                if (GUI_PointIsInRect(para.cur_tp_point, form_rect))
                {
           //         SetHandleOkMenuMsg(MSG_TP_PRESS_UP);
                    oper_order_id = GetChatWinItemId(win_data_ptr);
                    if (oper_order_id == PNULL)
                    {
                        break;
                    }
                    switch (oper_order_id->flag.msg_type)
                    {
                        case MMISMS_TYPE_SMS:

                            break;

                        case MMISMS_TYPE_MMS:
                            //解决cr MS00150274,U盘使用过程中不能看MMS
                            if (MMIAPIUDISK_UdiskIsRun())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                                break;
                            }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                            if (MMIAPIBROWSER_IsRunning())
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                                break;
                            }
#endif
#endif
                            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                            {
                                win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                                MMISMS_SetDelIndex(win_data_ptr->del_index);
                            }
                            if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                            {
#ifdef MMS_SUPPORT
                                MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                            }                        
                            MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);
                        
                            //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                            {
                                MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                            }
                            if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                            {
                                MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef MMS_SUPPORT
        case MSG_MMS_FINISH_READ:
            oper_order_id = MMISMS_GetChatOperOrderID();
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);

            SetMMSChatItemParam(win_data_ptr,win_id, both_rect, oper_order_id);
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);

            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }

            if (PNULL != oper_order_id->next_chat_ptr)
            {
                if (MMISMS_TYPE_SMS == oper_order_id->next_chat_ptr->flag.msg_type)
                {
                    SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                    MMISMSAPP_ReadChatSms(oper_order_id->next_chat_ptr,
                            ChatWinReadSmsCallback,
                            PNULL,
                            PNULL
                        );
                }
                else
                {
                    MMISMS_SetGlobalOperationOrderId( oper_order_id->next_chat_ptr );
                    MMISMS_SetChatOperOrderID(oper_order_id->next_chat_ptr);
                    MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_MMS_FINISH_READ, PNULL, 0);
                }
            }
            else
            {
                MMISMS_UpdateUnreadOrder();
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }

            break;

        case MSG_MMS_UPDATE_CHAT:
            {
                MMI_CTRL_ID_T  replace_child_ctrl = 0;
                uint16 i = 0;
                
                oper_order_id = MMISMS_GetChatOperOrderID();
                
                if (PNULL == oper_order_id)
                {
                    break;
                }
    
                for(i = 0;i<MMISMS_CHAT_MAX_ITEM;i++)
                {
                    if(win_data_ptr->chat_item[i].is_valid && win_data_ptr->chat_item[i].cur_order_id != PNULL && win_data_ptr->chat_item[i].cur_order_id->record_id == oper_order_id->record_id)
                    {
                        replace_child_ctrl = win_data_ptr->chat_item[i].parent_ctrl_id;
                        win_data_ptr->chat_item[i].is_valid = FALSE;
                        break;
                    }
                }
                
                if(i != MMISMS_CHAT_MAX_ITEM)
                {
                    GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
                    SCI_MEMSET(&win_data_ptr->chat_item[i],0x0,sizeof(MMISMS_CHAT_ITEM_T));
                }
                    
                MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
    
                SetMMSChatItemParam(win_data_ptr,win_id, both_rect, oper_order_id);
                MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
                if (MMK_IsFocusWin(win_id))
                {
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                }
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }
            break;
#endif

        case MSG_SMS_FINISH_READ:
            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            SetChatItemParam(win_id, both_rect, oper_order_id);
            {//temp debug	
            SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
            MMI_CheckAllocatedMemInfo();
            }
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);

            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }
            
            if (PNULL != oper_order_id->next_chat_ptr)
            {
                if (MMISMS_TYPE_SMS == oper_order_id->next_chat_ptr->flag.msg_type)
                {
                    SCI_MEMSET(&win_data_ptr->read_result,0,sizeof(MMISMS_READ_MSG_T));
                    MMISMSAPP_ReadChatSms(oper_order_id->next_chat_ptr,
                            ChatWinReadSmsCallback,
                            PNULL,
                            PNULL
                        );
                    {//temp debug	
                    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
                    MMI_CheckAllocatedMemInfo();
                    }
                }
                else
                {
                    MMISMS_SetGlobalOperationOrderId( oper_order_id->next_chat_ptr );
                    MMISMS_SetChatOperOrderID(oper_order_id->next_chat_ptr);
                    MMK_PostMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID,  MSG_MMS_FINISH_READ, PNULL, 0);
                }
            }
            else
            {
                MMISMS_UpdateUnreadOrder();
                MMK_CloseWin(MMISMS_READSMS_WAIT_WIN_ID);
                MMISMS_ClearOperInfo();
            }

            break;

        case MSG_SMS_NEW_SMS:
            oper_order_id = (MMISMS_ORDER_ID_T)param;
            if (PNULL == oper_order_id)
            {
                break;
            }
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
            SetChatItemParam(win_id, both_rect, oper_order_id);
            MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
            if (MMK_IsFocusWin(win_id))
            {
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
            }            
            break;
            
         
        case MMISMS_MSG_UPDATELIST:
            {        
                if(!win_data_ptr->is_del_all) //全部删除，直接关闭当前窗口，避免多次刷新
                {
#ifndef MMI_GUI_STYLE_TYPICAL
                    MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID);
                    UpdateChatItemDisplayAfterDelSingle(win_data_ptr);
#endif 
                    if (MMK_IsFocusWin(win_id))
                    {
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0); //刷新窗口
                    }
                }
            }

            break;
            
        case MMISMS_MSG_UPDATE_NUMBER:
            HandleChatReadWinUpdateDispNum(win_data_ptr);
            break;  
            
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_MENU:
            //check is no log or not
            {
                MMIPUB_MENU_PARAM_T menuparam = {0};
                MMI_CTRL_ID_T active_ctrl_id = 0;
                MMISMS_ORDER_ID_T order_id = PNULL;
                active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
                if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                {
                    order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
                }
                if (PNULL != order_id)
                {
                    MMISMS_SetGlobalOperationOrderId(order_id);
                }
                menuparam.group_id = MENU_SMS_CHAT_OPT;
                menuparam.win_id =   MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID;
                menuparam.ctrl_id =  MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID;

                menuparam.dst_handle = win_id;
                menuparam.func.OnMenuOk = HandleSmsChatMenuOptOK;
                menuparam.func.OnMenuInit = HandleSmsChatMenuOptInit;

                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;

#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_LONG:
        {
            MMISMS_ORDER_ID_T order_id = PNULL;
            MMI_CTRL_ID_T active_ctrl_id = 0;
            MMI_MULTI_KEY_TP_MSG_PARAM_T  para = {0};
            GUI_RECT_T  form_rect = {0};
            MMIPUB_MENU_PARAM_T menuparam = {0};
            active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);

            SCI_MEMCPY(&para, (MMI_MULTI_KEY_TP_MSG_PARAM_T*)param, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            GUICTRL_GetRect( MMK_GetCtrlPtr(active_ctrl_id), &form_rect);
            if (GUI_PointIsInRect(para.cur_tp_point, form_rect))
            {
                if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                {
                    order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
                }
                if (PNULL != order_id)
                {
                    MMISMS_SetGlobalOperationOrderId(order_id);
                        
                    menuparam.group_id = MENU_SMS_CHAT_ITEM_OPT;
                    menuparam.win_id =   MMISMS_SHOW_CHAT_ITEM_OPT_WIN_ID;
                    menuparam.ctrl_id =  MMISMS_SHOW_CHAT_ITEM_OPT_CTRL_ID;

                    menuparam.dst_handle = win_id;
                    menuparam.func.OnMenuInit = HandleSmsChatItemOptInit;
                    menuparam.func.OnMenuOk = HandleSmsChatItemOptOK;                    
                    MMIPUB_OpenPubMenuWin( &menuparam ); 
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
        case MMI_SELECT_CONTACT_CNF:    //added by taiping.lai 2011/01/18 添加联系人信息处理
            if (win_data_ptr->is_add_contact)
            {
                AddContactInfo2Editbox((MMIPB_HANDLE_T)param);
                win_data_ptr->is_add_contact = FALSE;
            }
            break;

        case MSG_PROMPTWIN_OK:            
            {
                //需要判断是否加帧显示
                if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr && MMIFMM_FILE_TYPE_PICTURE == win_data_ptr->sms_chat_insert_info.file_type)
                {
                    // allocate memory
                    MMI_STRING_T      str_ptr = {0};
                    MMI_STRING_T      str_to = {0};
    
                    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                    MMK_CloseWin(quer_win_id);
                    mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                    if (mms_content_str.wstr_ptr == PNULL)
                    {
                        //SCI_TRACE_LOW:"mmisms alloc failed"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28546_112_2_18_2_55_19_850, (uint8*)"");
                        if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                        {
                            SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                            win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                        }
                        break;
                    }
                    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                    str_to.wstr_ptr = wnumber;
                    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif 
                    if (MMIAPIMMS_EditAddMMSAttach(win_data_ptr->sms_chat_insert_info.file_info_ptr, win_data_ptr->sms_chat_insert_info.file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE))
                    {
                        MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_MMS_MULTIFILE_TOO_BIG);
                    }
    
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    if (PNULL != mms_content_str.wstr_ptr)
                    {
                        SCI_FREE(mms_content_str.wstr_ptr);
                        mms_content_str.wstr_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                }
            }
            break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(&quer_win_id);
            break;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
        case MSG_MULTIM_SELECTED_RETURN:
        {
            MMIFILE_FILE_INFO_T  *file_info = PNULL;

            file_info = (MMIFILE_FILE_INFO_T*)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
            if (PNULL == file_info)
            {
                //SCI_TRACE_LOW:"sms file info alloc error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28604_112_2_18_2_55_19_851, (uint8*)"");
                break;
            }

            multim_return_info_ptr = (MULTIM_SELECT_RETURN_T *)param;
            //选中多媒体文件返回

            MMIAPIFMM_GetFileInfoFormFullPath(multim_return_info_ptr->file_name_ptr, multim_return_info_ptr->file_name_len, file_info);
            real_file_size = file_info->file_size;
            edit_type = MMISMS_GetChatEditInsertOpType();
            insert_total_size = real_file_size ;
            if (0 == real_file_size)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
            }
            else if (insert_total_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;

                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);

                reduce_pic_size = reduce_width * reduce_height * 2;

                pic_type = MMIAPICOM_GetImgaeType(file_info->file_name, file_info->file_name_len);
                if (   MMIFMM_FILE_TYPE_PICTURE == multim_return_info_ptr->file_type
                        && GUIANIM_TYPE_IMAGE_JPG == pic_type
                        && real_file_size > reduce_pic_size
                        && reduce_pic_size <= MMIMMS_MAX_CONTENT_SIZE
                        && MMISMS_INSERT_ADD_ATTACH != edit_type
                   )
                {
               
                    win_data_ptr->sms_chat_insert_info.file_type = multim_return_info_ptr->file_type;
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        SCI_FREE(file_info);
                        break;
                    }
                    //SCI_MEMSET((char *)g_sms_chat_insert_info.file_info_ptr, 0, (sizeof(MMIFILE_FILE_INFO_T)));
                    SCI_MEMCPY((char *)win_data_ptr->sms_chat_insert_info.file_info_ptr, (char *)file_info, sizeof(MMIFILE_FILE_INFO_T));
                   
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC, IMAGE_PUBWIN_QUERY, &quer_win_id, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MMS_MULTIFILE_TOO_BIG);
                }
            }
            else
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL == mms_content_str.wstr_ptr)
                {
                    //SCI_TRACE_LOW:"mmisms alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852, (uint8*)"");

                    SCI_FREE(file_info);

                    break;
                }
                //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                str_to.wstr_ptr = wnumber;
                str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif 
                if (MMISMS_INSERT_ADD_ATTACH != edit_type)
                {
                    if (MMIAPIMMS_EditAddMMSAttach(file_info, multim_return_info_ptr->file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE))
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
                    }
                }
                else
                {
                    if (MMIAPIMMS_EditAddMMSAttach(file_info, MMIFMM_FILE_TYPE_NORMAL, PNULL, 0, PNULL, &mms_content_str, &str_to, FALSE))
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
                    }
                }
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    SCI_FREE(mms_content_str.wstr_ptr);
                    mms_content_str.wstr_ptr = PNULL;
                }
            }
            SCI_FREE(file_info);
        }

        break;
#endif

#if defined(MMI_VCARD_SUPPORT)
        case MMI_PB_VCARD_SELECTED:
        {
            uint32 vcard_data_len = 0;
            uint8 *vcard_buf_ptr  = 0;
            MMI_STRING_T vacard_name_str = {0};
            wchar vacard_suf[] = {'.', 'v', 'c', 'f', 0};

            vcard_buf_ptr = (uint8 *)SCI_ALLOCA(VCARD_MAX_FILE_LEN * sizeof(uint8));
            if (vcard_buf_ptr == PNULL)
            {
                break;
            }
            SCI_MEMSET(vcard_buf_ptr, 0x00, VCARD_MAX_FILE_LEN * sizeof(uint8));
            MMIAPIPB_GetSelectVcard((MMIPB_HANDLE_T)param, vcard_buf_ptr, VCARD_MAX_FILE_LEN, 0);
            vcard_data_len = strlen((char *)vcard_buf_ptr);
            vacard_name_str.wstr_ptr = SCI_ALLOCAZ((MMIMMS_FILE_NAME_LEN + 1) * sizeof(wchar));
            if (PNULL == vacard_name_str.wstr_ptr)
            {
                SCI_FREE(vcard_buf_ptr);
                break;
            }
            //SCI_MEMSET((char *)vacard_name_str.wstr_ptr, 0, ((MMIMMS_FILE_NAME_LEN+1)*sizeof(wchar)));
            MMIAPIPB_GetSelectName((uint32)param, &vacard_name_str, MMIMMS_FILE_NAME_LEN - MMIAPICOM_Wstrlen(vacard_suf), 0);
            MMIAPICOM_Wstrncpy(vacard_name_str.wstr_ptr + vacard_name_str.wstr_len, vacard_suf, MMIAPICOM_Wstrlen(vacard_suf));
            vacard_name_str.wstr_len = vacard_name_str.wstr_len + MMIAPICOM_Wstrlen(vacard_suf);
            insert_total_size = vcard_data_len;
            if (0 == vcard_data_len)
            {
                MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
            }
            else if (insert_total_size > MMIMMS_MMS_LIMIT_SIZE)
            {
                MMIPUB_OpenAlertWarningWin(TXT_MMS_MULTIFILE_TOO_BIG);
            }
            else
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                MMIPB_BCD_NUMBER_T pb_bcd = {0};
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
                pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                    str_to.wstr_ptr = wnumber;
                    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif                     

                    if (MMIAPIMMS_EditAddMMSAttach(PNULL, MMIFMM_FILE_TYPE_VCARD, vcard_buf_ptr, vcard_data_len, &vacard_name_str, &mms_content_str, &str_to, FALSE))
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
                    }
                    SCI_FREE(vcard_buf_ptr);
                    SCI_FREE(vacard_name_str.wstr_ptr);
                    if (PNULL != mms_content_str.wstr_ptr)
                    {
                        SCI_FREE(mms_content_str.wstr_ptr);
                        mms_content_str.wstr_ptr = PNULL;
                    }
                }
            }
        }
        break;
#endif
     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
        {
            ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;

            switch (ctrl_id)
            {
                case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
#ifdef MMIMMS_SMS_IN_1_SUPPORT                    
                    HandleChatWinOpenInsertOpt(win_data_ptr); 
#endif
                    break;

                case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
                    BuildSendData(win_id,FALSE);
                    HandleChatWinSendSms(win_id);
                    break;

                default:
                    if ( (ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) && (ctrl_id < MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM)) //信息内容提取处理
                    {
                        HandleChatSmsTag(win_data_ptr,ctrl_id);
                    }
                    break;

            }
            break;
        }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);

            if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID == active_ctrl_id)
            {
                MMI_STRING_T str = {0};
                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str);
                if (0 != str.wstr_len)
                {
                    BuildSendData(win_id,FALSE);
                    HandleChatWinSendSms(win_id);
                }
            }
            else if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                     && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        HandleChatSmsTag(win_data_ptr,active_ctrl_id);
                        break;

#ifdef MMS_SUPPORT
                    case MMISMS_TYPE_MMS:
                        //解决cr MS00150274,U盘使用过程中不能看MMS
                        if (MMIAPIUDISK_UdiskIsRun())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                            break;
                        }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                        if (MMIAPIBROWSER_IsRunning())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                            break;
                        }
#endif
#endif

                        if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                        {
                            win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                            MMISMS_SetDelIndex(win_data_ptr->del_index);
                        }                        
                        if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                        {
#ifdef MMS_SUPPORT
                            MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                        }
                        MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);
                                                
                        //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                        if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                        }
                        break;
#endif
                    default:
                        break;
                }
            }
        }
        break;

        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);

            if (MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID == active_ctrl_id)
            {
                GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_SEND, MIDDLE_BUTTON, TRUE);
                GUIEDIT_SetSoftkey(active_ctrl_id, 1, 1, TXT_NULL, TXT_SEND, PNULL);

                HandleSmsSendEnable(TRUE);
            }
            else if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                     && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
                        break;

                    case MMISMS_TYPE_MMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_READ, MIDDLE_BUTTON, TRUE);
                        break;

                    default:
                        break;
                }

            }
        }
        break;
#endif
#else           
        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);
            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                    && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, TRUE);
                        break;

                    case MMISMS_TYPE_MMS:
                        GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_READ, MIDDLE_BUTTON, TRUE);
                        break;

                    default:
                        break;
                }

            }
        }
        break;


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
        {
            MMI_CTRL_ID_T active_ctrl_id = MMK_GetActiveCtrlId(win_id);

            if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID
                    && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
            {
                oper_order_id = GetChatWinItemId(win_data_ptr);
                if (oper_order_id == PNULL)
                {
                    break;
                }
                switch (oper_order_id->flag.msg_type)
                {
                    case MMISMS_TYPE_SMS:
                        HandleChatSmsTag(win_data_ptr,active_ctrl_id);
                        break;
#ifdef MMS_SUPPORT
                    case MMISMS_TYPE_MMS:
                        //解决cr MS00150274,U盘使用过程中不能看MMS
                        if (MMIAPIUDISK_UdiskIsRun())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                            break;
                        }

#ifdef BROWSER_SUPPORT
#ifndef NANDBOOT_SUPPORT
                        if (MMIAPIBROWSER_IsRunning())
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_MMS_WEB_IN_USING);
                            break;
                        }
#endif
#endif                        
                        if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
                        {
                            win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
                            MMISMS_SetDelIndex(win_data_ptr->del_index);
                        }
                        if(MMISMS_MT_TO_BE_READ == oper_order_id->flag.mo_mt_type)
                        {
#ifdef MMS_SUPPORT
                            MMISMS_UpdateReadedMMSIcon(win_data_ptr);
#endif
                        }
                        MMIAPIMMS_ReadMMS((uint32)oper_order_id->record_id);

                        //避免左右键切换到彩信，按返回键后回到了刚刚读短信界面 cr117926
                        if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
                        {
                            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
                        }
                        if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
                        }
                        break;
#endif
                    default:
                        break;
                }
            }
        }
        break;
#endif

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            // to get the content which the user input
            MMI_STRING_T edit_string = {0};
            SCI_MEMSET( &edit_string, 0, sizeof( MMI_STRING_T ) );
            GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &edit_string);
            if (edit_string.wstr_len > 0)
            {
            
                num_len = MMIAPICOM_GenDispNumber(
                              MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                              MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                              win_data_ptr->chat_number.number,
                              number,
                              MMISMS_PBNUM_MAX_LEN + 2
                          );

                // to save the information
                win_data_ptr->edit_strlen = edit_string.wstr_len;
                SCI_MEMCPY(win_data_ptr->edit_string,edit_string.wstr_ptr,sizeof(wchar)*win_data_ptr->edit_strlen);
                MMISMS_SetAddressToMessage((uint8 *)number, (uint8)strlen((char *)number), &win_data_ptr->edit_content.dest_info.dest_list);    
                SMS_SAVE(MN_DUAL_SYS_1,win_data_ptr);
                
            }
            MMK_CloseWin( win_id );
        }
        break;

        case MSG_CLOSE_WINDOW:
#ifdef TTS_SUPPORT
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
#endif
            MMK_CloseWin(MMISMS_NEWMSG_WIN_ID);
            MMISMS_SetExceedUnreadNeedUpdate();
            MMISMS_UpdateUnreadOrder();            
            ChatWinCloseCtrlFunc(win_data_ptr);            
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }

    return recode;
}


/*****************************************************************************/
// Description : close ctrl func
// Global resource dependence :
// Author:  rong.gu
// Note:
/*****************************************************************************/
LOCAL void ChatWinCloseCtrlFunc (MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    if (win_data_ptr != PNULL)
    {
        SCI_FREE(win_data_ptr);
    }
    MMIFW_SetEventListener(SMS_NEWSMS_EVENT, ChatReadWinNewEventCallback, FALSE); 
    MMIFW_SetEventListener(SMS_DATA_CHANGE_EVENT, ChatReadWinDataChangeCallback, FALSE); 
}

/*****************************************************************************/
//  Description : 号码添加到联系人后需要更新显示
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatReadWinUpdateDispNum(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{        
    MMI_STRING_T name = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
    wchar temp_arr[MMISMS_PBNAME_MAX_LEN + 1] = {0};

    name.wstr_ptr = temp_arr;
    pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
    MMIAPIPB_GetNameByNumber(&pb_num,&name,MMISMS_PBNAME_MAX_LEN,FALSE);
    GUIWIN_SetTitleText(win_data_ptr->win_id, name.wstr_ptr, name.wstr_len, FALSE);     
}

/*****************************************************************************/
//  Description : 获取聊天显示窗口中某条信息的order_id
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_ID_T GetChatWinItemId(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return PNULL;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    return order_id;
}

/*****************************************************************************/
//  Description : to handle the message enable send
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleSmsSendEnable(BOOLEAN is_update)
{
    MMI_STRING_T str = {0};
    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str);
    if (0 == str.wstr_len)
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, 2, TRUE);
#else
        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_OPTION, TXT_NULL, STXT_RETURN, is_update);
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetSoftkeyBtnGray(MMISMS_SHOW_CHAT_SMS_WIN_ID, 2, FALSE);
#else
        GUIWIN_SetSoftkeyTextId(MMISMS_SHOW_CHAT_SMS_WIN_ID, STXT_OPTION, TXT_SEND, TXT_DELETE, is_update);
#endif
    }
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT 
/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinInsertOptOK(MMIPUB_MENU_NOTIFY_T *param )
{       
    uint32 group_id, menu_id;
   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleChatWinInsertOptOK param pnull");
        return;
    }
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    if (menu_id != MMISMS_MENU_INSERT_TXT)
    {
    #ifdef MMS_SUPPORT
        MMIMMS_ERROR_E edit_err = MMIMMS_NO_ERROR;
        edit_err = MMIAPIMMS_EnterEditCheck();
        if (MMIMMS_NO_ERROR != edit_err)
        {
            //提示彩信初始化失败
            MMK_CloseWin(param->dst_handle);
            MMIAPIMMS_EditMMSAlertWin(edit_err);
            return;
        }
    #endif
    }
    
    switch (menu_id)
    {
        case MMISMS_MENU_INSERT_SUBJECT:
            {
                MMI_STRING_T      str_ptr = {0};
                MMI_STRING_T      str_to = {0};
                MMI_STRING_T mms_content_str = {0};
                uint8 num_len = 0;
                uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

                GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
                mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
                if (PNULL == mms_content_str.wstr_ptr)
                {
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869, (uint8*)"");
                    break;
                }
                MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
                mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

                num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
                str_to.wstr_ptr = wnumber;
                str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
                MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif                 
                if(MMIAPIMMS_EditAddMMSSubject(&mms_content_str,&str_to))
                {
                    MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
                }
                
                if (PNULL != mms_content_str.wstr_ptr)
                {
                    SCI_FREE(mms_content_str.wstr_ptr);
                    mms_content_str.wstr_ptr = PNULL;
                }
            }
	    break;
         
        case MMISMS_MENU_INSERT_PIC:
        {
            SmsChatEditInsertPic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }    
        break;
    
        case MMISMS_MENU_INSERT_PHIZ:
            MMISMS_OpenPhizWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case MMISMS_MENU_INSERT_EMOJI:
            MMISMS_OpenEmojiWin();
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;

        case MMISMS_MENU_INSERT_CONTACT_INFO:
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_NUMBER,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                EditSmsSelectContactCallback
            );
            //SetIsAddContact(TRUE);
            win_data_ptr->is_add_contact = TRUE;
            MMK_SetAtvCtrl(param->dst_handle, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case     MMISMS_MENU_INSERT_TXT:  //插入范本
            MMISMS_CreateSelectWordWin(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID);
            break;
    
        case     MMISMS_MENU_INSERT_AUDIO:
        {
            SMSEditInsertMusic(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        break;
        case MMISMS_MENU_INSERT_TAKE_PIC:
        {
            SMSOpenDC();
        }
        break;

#ifdef MMI_RECORD_SUPPORT
        case MMISMS_MENU_INSERT_TAKE_AUDIO:
        {
            SMSOpenRecord();
        }
        break;
#endif
    
        case MMISMS_MENU_INSERT_TAKE_VIDEO:
        {
            SMSOpenDV();
        }
        break;
    
        case     MMISMS_MENU_INSERT_VIDEO:
        {
            SMSEditInsertVideo(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        break;
    
        case     MMISMS_MENU_INSERT_NEXTPAGE:
        case     MMISMS_MENU_INSERT_PREPAGE:
        {
            MMI_STRING_T      str_ptr = {0};
            MMI_STRING_T      str_to = {0};
            MMI_STRING_T mms_content_str = {0};
            uint8 num_len = 0;
            uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
            uint32 slide_direct = 0;
            GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
            mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
            if (PNULL == mms_content_str.wstr_ptr)
            {
                //SCI_TRACE_LOW:"mmisms alloc failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30692_112_2_18_2_55_23_869, (uint8*)"");
                break;
            }
            //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
            MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
            mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);
            
            num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
            str_to.wstr_ptr = wnumber;
            str_to.wstr_len = num_len;
        
            if (menu_id == MMISMS_MENU_INSERT_NEXTPAGE)
            {
                slide_direct = 1;
            }
            else
            {
                slide_direct = 0;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif   
            if (MMIAPIMMS_EditAddMMSSlide(slide_direct, &mms_content_str, &str_to))
            {
                MMIPUB_OpenAlertWaitingWin(TXT_MMS_SMS_TO_MMS);
            }
            SCI_FREE(mms_content_str.wstr_ptr);
        }
        break;
    
        case MMISMS_MENU_INSERT_ATTACH_VCARD:
            //MMIAPIPB_OpenNumberList(MMIPB_LIST_VCARD,MMIPB_LIST_TYPE_SINGLE,1,PNULL);
    
            MMIAPIPB_OpenMainWin(
                MMIPB_ALL_CONTACT,//
                MMIPB_LIST_RADIO, //IN: 号码列表的类型
                MMIPB_LIST_FIELD_WHOLE,
                1,//如果是多选时，此项限制了多选的最大个数
                PNULL,
                EditSelectVcardCallback
            );
    
            MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
            break;
    
        case MMISMS_MENU_INSERT_ATTACH_OTHER:
            MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
            SMSEditInsertNormalAttachment(MMISMS_SHOW_CHAT_SMS_WIN_ID);
            break;
    
        default:
            break;
    }
}
#endif


#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : truncate first line of msg content
//  Global resource dependence :
//  Author:sam.hua
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN TruncateMsgContent(MMI_STRING_T *msg_content_ptr )
{
    int16 i;
    if (msg_content_ptr == PNULL)
    {
        return FALSE;
    }
    for (i = 0; i < msg_content_ptr->wstr_len; i++)
    {
        if (msg_content_ptr->wstr_ptr[i] == 0x000d)
        {
            msg_content_ptr->wstr_len = i;
            return TRUE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptInit(MMIPUB_MENU_NOTIFY_T *param )
{       
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    MMI_STRING_T msg_content = {0};
#ifdef TTS_SUPPORT
    MMI_STRING_T    tts_read_menu = {0};
#endif
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T ctrl_id = MMISMS_SHOW_CHAT_ITEM_OPT_CTRL_ID;
    MMI_STRING_T    lock_menu = {0};
        
    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    MMISMS_ChatWinGetItemContent(&msg_content);
    TruncateMsgContent(&msg_content);
    //Truncate this content,only use first line
    GUIMENU_SetMenuTitle(&msg_content, ctrl_id);
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if (PNULL != cur_order_id && (MMISMS_MO_SEND_FAIL != cur_order_id->flag.mo_mt_type || 
                MMISMS_IsSmsBusy(cur_order_id) || 
                (MMISMS_TYPE_MMS == cur_order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(cur_order_id->record_id)
                ||MMIAPIUDISK_UdiskIsRun()
                ))))
    {
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_RESEND_ITEM_ID, FALSE);
    }
    if (PNULL != cur_order_id && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
    {
#ifdef MMI_SMS_COPY_SUPPORT
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_COPY_ITEM_ID,TRUE);
#endif
#ifdef SNS_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_SINA_ID, FALSE);
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_FACEBOOK_ID, FALSE);
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_SHARE_TO_TIWTTER_ID, FALSE);
#endif
#ifdef TTS_SUPPORT
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
#endif
    }
    if (PNULL != cur_order_id && MMISMS_TYPE_MMS == cur_order_id->flag.msg_type && MMIAPIUDISK_UdiskIsRun())
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_FORWARD_ITEM_ID,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_DEL_ITEM_ID,TRUE);
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_BOX_SECURITY_ITEM_ID,TRUE);
#endif
    }
#ifdef MMI_SMS_COPY_SUPPORT
    if (PNULL != cur_order_id && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type && MN_SMS_STORAGE_SIM == cur_order_id->flag.storage)
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_COPY_OPT,MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID,TRUE);
    }

    if (PNULL != cur_order_id && MMISMS_TYPE_SMS == cur_order_id->flag.msg_type && cur_order_id ->flag.storage == MN_SMS_STORAGE_ME)
    {
        GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_COPY_OPT,MMISMS_MENU_BOX_COPYTOME_ITEM_ID,TRUE);
    }
#endif
#ifdef TTS_SUPPORT
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
    } 
    GUIMENU_SetItemVisible(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, TRUE); 
    
    if (MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
    {
        MMI_GetLabelTextByLang(STXT_STOP, &tts_read_menu);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SET_TTS_PLAY, &tts_read_menu);
    }
    GUIMENU_SetStaticItem(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_TTS_READ_ID, &tts_read_menu, 0);
#endif
    if(PNULL != cur_order_id)
    {
        if (cur_order_id->flag.is_locked)
        {
            MMI_GetLabelTextByLang(TXT_SMS_UNLOCK, &lock_menu);
            GUIMENU_SetItemGrayed(ctrl_id,MENU_SMS_CHAT_ITEM_OPT,MMISMS_MENU_DEL_ITEM_ID,TRUE);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_LOCK, &lock_menu);
        }
        GUIMENU_SetStaticItem(ctrl_id, MENU_SMS_CHAT_ITEM_OPT, MMISMS_MENU_LOCK_ITEM_ID, &lock_menu, 0);
    }

}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatItemOptOK(MMIPUB_MENU_NOTIFY_T *param )
{       
    uint32 group_id, menu_id;
   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    group_id = param->group_id;
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    switch (menu_id)
    {
        case MMISMS_MENU_FORWARD_ITEM_ID:          
            HandleChatWinForwardMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleChatWinLockOrUnLockMsg(win_data_ptr);
            break;
                        
        case MMISMS_MENU_DEL_ITEM_ID:
            HandleChatWinDelSingle(win_data_ptr);
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:            
            HandleChatWinMoveSecurity(win_data_ptr);
            break;
#endif
#ifdef MMI_SMS_COPY_SUPPORT
        case MMISMS_MENU_BOX_COPYTOME_ITEM_ID:
            HandleChatWinCopySimToMe();
            break;

        case MMISMS_MENU_BOX_COPYTOSIM_ITEM_ID:
            HandleChatWinCopyMeToSim(win_data_ptr);
            break;
#endif
#ifdef SNS_SUPPORT
        case MMISMS_MENU_SHARE_TO_SINA_ID:
        case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
        case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
            HandleChatWinShareSNS(win_data_ptr);
            break;        
#endif
        
#ifdef TTS_SUPPORT
        case MMISMS_MENU_TTS_READ_ID:
            HandleChatWinReadTTS();
            break;            
#endif       
        case MMISMS_MENU_RESEND_ITEM_ID:
            HandleChatWinResendItem(win_data_ptr);
            break;
        default:
            break;
    }
}
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinResendItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
//    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
//    MMI_WIN_ID_T win_id = win_data_ptr->win_id;
//    uint16 num_len = 0;
    cur_order_id = MMISMS_GetCurOperationOrderId();
    if(PNULL == cur_order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        BuildSendData(win_data_ptr->win_id,TRUE);
        HandleChatWinSendSms(win_data_ptr->win_id);

    }
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support
    else if(MMISMS_TYPE_MAIL == cur_order_id->flag.msg_type)
    {
        MMIAPIMAIL_SendMailById((uint32)mail_getMsgIdByRecordId(cur_order_id->record_id));
    }
#endif
    else //is mms
    {
        MMIAPIMMS_OutboxSendMMS((uint32)cur_order_id->record_id);
    }
}


#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinReadTTS(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        MMI_STRING_T    msg_content = {0};
        if (MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
        }
        else
        {
            MMISMS_ChatWinGetItemContent(&msg_content);
            if (PNULL != msg_content.wstr_ptr)
            {
                MMITTS_PLAY_PARAM_T play_param = {0};
    
                play_param.txt_addr = (uint8 *)msg_content.wstr_ptr;
                play_param.txt_len_in_byte = msg_content.wstr_len * 2;
                play_param.is_ucs2 = TRUE;
                play_param.play_times = 1;
                play_param.tts_vol = MMIAPISET_GetMultimVolume();
                play_param.ring_type = TTS_RING_TYPE_SMS;
                play_param.win_handle = MMISMS_SHOW_CHAT_SMS_WIN_ID;
                play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
                play_param.digit_mode = TTS_DIGIT_AUTO;
                play_param.punc_mode = TTS_PUNC_OFF;
                play_param.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
                
                MMIAPITTS_PlayText(&play_param);
    
                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
    }        
}
#endif  
/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMIPB_BCD_NUMBER_T pb_num = {0};
#ifndef MMI_PDA_SUPPORT        
    MMI_STRING_T    lock_menu = {0};
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
#endif        

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }

    pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
    
    if(0 == MMISMS_GetChatAvailDeleteNum(pb_num))
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_DELBOX_ALL_ITEM_ID, TRUE);        
    }
    if(win_data_ptr->is_email_number)
    {
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_CALL_ITEM_ID, TRUE);                
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_ADDNUMTOPB_ITEM_ID, TRUE);                
#if defined(MMI_BLACKLIST_SUPPORT)        
        MMIAPICOM_EnableGrayed(MMISMS_SHOW_CHAT_SMS_OPT_WIN_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID, TRUE);                
#endif
    }
    
#ifndef MMI_PDA_SUPPORT    
    cur_order_id = MMISMS_GetCurOperationOrderId();    
    if (PNULL == cur_order_id)
    {
        return;
    }
    if (cur_order_id->flag.is_locked)
    {
        MMI_GetLabelTextByLang(TXT_SMS_UNLOCK, &lock_menu);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_LOCK, &lock_menu);
    }
    GUIMENU_SetStaticItem(MMISMS_SHOW_CHAT_SMS_OPT_CTRL_ID, MENU_SMS_CHAT_OPT, MMISMS_MENU_LOCK_ITEM_ID, &lock_menu, 0);
#endif    

    
}
/*****************************************************************************/
//     Description : handle chat menu opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleSmsChatMenuOptOK(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;
    
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    if ( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    win_data_ptr->menu_id = menu_id;
    switch (menu_id)
    {
#ifdef  MMIMMS_SMS_IN_1_SUPPORT    
        case MMISMS_MENU_INSERT_ELEMENT:
            HandleChatWinOpenInsertOpt(win_data_ptr); 

            break;
#endif
        case MMISMS_MENU_FORWARD_ITEM_ID:          
            HandleChatWinForwardMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_LOCK_ITEM_ID:
            HandleChatWinLockOrUnLockMsg(win_data_ptr);
            break;
            
        case MMISMS_MENU_DEL_ITEM_ID:   
            HandleChatWinDelSingle(win_data_ptr);
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMISMS_MENU_BOX_SECURITY_ITEM_ID:            
            HandleChatWinMoveSecurity(win_data_ptr);
            break;
#endif

#ifdef SNS_SUPPORT
        case MMISMS_MENU_SHARE_TO_SINA_ID:
        case MMISMS_MENU_SHARE_TO_FACEBOOK_ID:
        case MMISMS_MENU_SHARE_TO_TIWTTER_ID:
            HandleChatWinShareSNS(win_data_ptr);
            break;        
#endif

#ifdef TTS_SUPPORT
        case MMISMS_MENU_TTS_READ_ID:
            HandleChatWinReadTTS();
            break;            
#endif                   
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:          
            HandleChatWinAnswerSms(win_data_ptr);
            break;
            
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
            HandleChatWinAnswerMMS(win_data_ptr);
            break;
            
        case MMISMS_MENU_DELBOX_ALL_ITEM_ID:
            HandleChatWinDelboxAll(win_data_ptr);           
            break;
            
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_IPDIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
            HandleChatWinCallItem(win_data_ptr);              
            break;
            
        case  MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:                
            HandleChatWinAddPBNew(win_data_ptr); 
            break;
            
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case  MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:    
            HandleChatWinAddPBExist(win_data_ptr); 
            break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case  MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:    
            HandleChatWinAddPBBlackList(win_data_ptr); 
            break;
#endif            
        default:
            break;
    }

}

/*****************************************************************************/
//     Description : to handle the message of waiting window
//    Global resource dependence :
//  Author:rong.gu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReadWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
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

        case MSG_CLOSE_WINDOW:
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }
    return (recode);
}

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerSms(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint16 num_len = 0;
    MMI_STRING_T string_to = {0};
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MN_DUAL_SYS_E     dual_sys = MN_DUAL_SYS_MAX;
    MMISMS_ORDER_ID_T  cur_order_id = PNULL;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    cur_order_id = MMISMS_GetChatOperOrderID();
#ifdef MMI_SMS_REPLY_PATH_SUPPORT
    if (MMISMS_GetReplyPathFlag())
    {        
        if (cur_order_id != PNULL && cur_order_id->flag.is_concatenate_sms)
        {
            MMI_STRING_T prompt_str = {0};

            MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &prompt_str );
            MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMISMS_READSMS_WAIT_WIN_ID, IMAGE_NULL,
                               ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleReadWaitWinMsg);
        }
        // to read the pointed SMS
        //Just read the message,and the read cnf will updated the message center number
        MMISMSAPP_ReadSmsCenter(cur_order_id,PNULL,PNULL,PNULL);
    }
    else
#endif
    {
        if ( cur_order_id != PNULL)
        {
            dual_sys = cur_order_id->flag.dual_sys;
        }
        if (!win_data_ptr->is_email_number)
        {
            num_len = MMIAPICOM_GenDispNumber(
                MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                win_data_ptr->chat_number.number,
                number,
                MMISMS_PBNUM_MAX_LEN + 2);
            MMISMS_WriteNewMessage(
                dual_sys,
                PNULL,
                FALSE,
                number,
                num_len
            );
        }
        else
        {
            num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
            string_to.wstr_ptr = wnumber;
            string_to.wstr_len = num_len;
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIAPIMMS_SetIsEmailAddrExist(TRUE);
    #endif
            MMIAPIMMS_AnswerMMS(dual_sys, PNULL, &string_to);
        }

    }
}

/*****************************************************************************/
//     Description : handle chat win answer sms 
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAnswerMMS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint16 num_len = 0;
    MMI_STRING_T string_to = {0};
    wchar wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    else
    {
        return;
    }


    MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER);
    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);    
    string_to.wstr_ptr = wnumber;
    string_to.wstr_len = num_len;
    
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif          
    if(order_id != PNULL)
    {
        MMIAPIMMS_AnswerMMS(order_id->flag.dual_sys, PNULL, &string_to); //释放subject空间
    }


}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelboxAll(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

//	uint16 cur_index = 0;    
    MMIPB_BCD_NUMBER_T pb_num = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    if (MMISMS_IsUpdateOk())
    {

        pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
        MMISMS_SetLastChatMsgMarked(pb_num, TRUE);
    }
    win_data_ptr->is_del_all = TRUE;
    ChatWinOpenDelteQueryWin(win_data_ptr);
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }   
    if (MMISMS_IsUpdateOk())
    {
        MMI_CTRL_ID_T active_ctrl_id = 0;
        MMISMS_ORDER_ID_T order_id = PNULL;
    
        active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
        if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
        {
            order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
        }
        else
        {
            return;
        }
           
        if (MMISMS_TYPE_MMS == order_id->flag.msg_type)
        {
            //解决cr MS00150274,U盘使用过程中不能看MMS
            if (MMIAPIUDISK_UdiskIsRun())
            {
                MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
                return;
            }
        }    
        win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;        
        MMISMS_SetDelIndex(win_data_ptr->del_index);
        MMISMS_SetCurOrderMarked(order_id,TRUE);
        win_data_ptr->is_del_all = FALSE;
        //win_data_ptr->chat_del_order_id = order_id;
        ChatWinOpenDelteQueryWin(win_data_ptr);
    }

}


/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBNew(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);    
    MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);  
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBExist(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);    
    MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
}
#endif

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinMoveSecurity(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }   
    if (MMISMS_IsUpdateOk())
    {
#ifdef MMI_SMS_MOVE_SUPPORT    
        HandleChatWinOpenMoveQueryWin(win_data_ptr);
#endif
    }

}
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinAddPBBlackList(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    pb_bcd = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);  
    
    MMISMS_AddSenderToBlackList(pb_bcd);
}
#endif

/*****************************************************************************/
//     Description : handle chat win call item
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCallItem(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{

    uint16 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};    
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T order_id = PNULL;

	if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    else
    {
        return;
    }
    if(order_id == PNULL)
    {
        return;            
    }
    
    num_len = MMIAPICOM_GenDispNumber(
        MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
        MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
        win_data_ptr->chat_number.number,
        number,
        MMISMS_PBNUM_MAX_LEN + 2
        );

    if (num_len > 0)
    {
        CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

        if (MMISMS_MENU_DIAL_ITEM_ID == win_data_ptr->menu_id)
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                        );
        }
        else if (MMISMS_MENU_IPDIAL_ITEM_ID == win_data_ptr->menu_id)
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_IP_CALL,
                            PNULL
                        );
        }
        else
        {
            cc_result = MMIAPICC_MakeCall(
                            order_id->flag.dual_sys,
                            number,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_VIDEO_CALL,
                            PNULL
                        );
        }
    }
    else
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_EMPTY_NUM);
    }

}

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence :
//  Author: renwei
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleChatWinSnsInstallWarningWinMsg(
    MMI_WIN_ID_T       win_id,
    MMI_MESSAGE_ID_E   msg_id,
    DPARAM             param
)
{
    //int            err             = 0;
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    widget_id = pubwin_info_ptr->user_data;

    switch (msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&win_data_ptr->pMe);

            if (win_data_ptr->pMe)
            {
                IMMISNS_AppInstall(win_data_ptr->pMe, widget_id);

                IMMISNS_Release(win_data_ptr->pMe);
                win_data_ptr->pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
            break;
    }

    return recode;
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinShareSNS(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    int share_id = 0;

    if (MMISMS_MENU_SHARE_TO_SINA_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_SINA;
    }
    else if (MMISMS_MENU_SHARE_TO_FACEBOOK_ID == win_data_ptr->menu_id)
    {
        share_id = MMI_SNS_FACEBOOK;
    }
    else
    {
        share_id = MMI_SNS_TIWTTER;
    }

    IMMISNS_CreateObject(&win_data_ptr->pMe);

    if (win_data_ptr->pMe)
    {
        if (IMMISNS_IsFixAppInstalled(win_data_ptr->pMe, share_id) < 0)
        {
            IMMISNS_Release(win_data_ptr->pMe);
            win_data_ptr->pMe = PNULL;

            MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(), PNULL, TXT_WHETHER_INSTALL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_OKCANCEL, HandleChatWinSnsInstallWarningWinMsg, share_id);
        }
        else
        {
            MMI_STRING_T msg_content = {0};

            MMISMS_ChatWinGetItemContent(&msg_content);

            if (PNULL != msg_content.wstr_ptr)
            {
                uint32        time_num    = 500;

                IMMISNS_ShareToSns(win_data_ptr->pMe, share_id, SNS_MODULE_SMS , (uint32)&msg_content);
                MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);

                SCI_FREE(msg_content.wstr_ptr);
                msg_content.wstr_ptr = PNULL;
            }
        }
    }   
}
#endif

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinForwardMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{         

    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    if (PNULL == cur_order_id)
    {
        return;
    }
    if (MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {   
        MMI_STRING_T msg_content = {0};    
        MMISMS_ChatWinGetItemContent(&msg_content);
        // open edit window
        MMISMS_ClearDestNum();
        MMISMS_WriteNewMessage(
            MN_DUAL_SYS_MAX,
            &msg_content,
            FALSE,
            PNULL,
            0
        );
    
        if (PNULL != msg_content.wstr_ptr)
        {
            SCI_FREE(msg_content.wstr_ptr);
            msg_content.wstr_ptr = PNULL;
        }        
    }
    else //IS MMS, not OTA and PUSH
    {
        MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD);
        MMIAPIMMS_FarwardMMS((uint32)cur_order_id->record_id);
    }
}

/*****************************************************************************/
//     Description : handle chat win del all
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinLockOrUnLockMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{         
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    uint16 index = 0;
    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }

    if (PNULL == cur_order_id)
    {
        return;
    }
    
    MMISMS_SetGlobalOperationOrderId(cur_order_id);
    cur_order_id->flag.is_locked = !cur_order_id->flag.is_locked;    
    MMISMS_SetCurMsgLocked(cur_order_id->flag.is_locked);
    index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
    
    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        if (MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type) 
        {
            if(cur_order_id->flag.is_locked)
            {          
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_SMS_CHAT_LOCK_ICON);
            }      
            else
            {
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_NULL);
            }
        }
        else
        {     
            if(cur_order_id->flag.is_locked)
            {           
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_SMS_CHAT_LOCK_ICON);
            }
            else
            {
                GUILABEL_SetIcon(MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index,IMAGE_NULL);
            }         
        }
    }
    else
    {
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
    
        ctrl_info.is_ctrl_id = TRUE;
        ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;
        if (MMISMS_MT_NOT_DOWNLOAD == cur_order_id->flag.mo_mt_type  ||
                MMISMS_MT_NOT_NOTIFY == cur_order_id->flag.mo_mt_type ||
                MMISMS_MT_NEED_NOT_DOWNLOAD == cur_order_id->flag.mo_mt_type
           )
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_MMS_NOTIFY_NEW_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
            }
        }
        else if (MMISMS_MT_HAVE_READ == cur_order_id->flag.mo_mt_type || MMISMS_MO_SEND_SUCC == cur_order_id->flag.mo_mt_type || MMISMS_MO_SEND_FAIL == cur_order_id->flag.mo_mt_type)
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_READ_MMS_ICON;        
            }
            else
            {
                data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
            }
        }
        else
        {
            if(cur_order_id->flag.is_locked)
            {
                data_info.img_id = IMAGE_SMS_LOCK_NOT_READ_MMS_ICON;
            }
            else
            { 
                data_info.img_id = IMAGE_SMS_NOT_READ_MMS_ICON;
            }
        }
        
        display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);        
    }
}

/*****************************************************************************/
//     Description : to delete all mared message
//    Global resource dependence :
//  Author:liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinDelAllMarkedMsg(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    //避免反复写彩信索引文件
    LOCAL BOOLEAN is_mms_exist = FALSE;
    MMISMS_OPER_ERR_E del_result = MMISMS_NO_ERR;
    uint16 msg_type = MMISMS_TYPE_MAX;

    del_result = MMISMS_AppDeleteMsgBox(MMISMS_BOX_CHAT, &msg_type, MMISMS_FOLDER_NORMAL);

    if (MMISMS_FINISH_OPER == del_result) //begin deleting sms
    {
        //finish the del
        if (is_mms_exist)
        {
            MMIAPIMMS_SaveListInfo();
            is_mms_exist = FALSE;
        }

        UpdateChatItemDisplayAfterDelSingle(win_data_ptr);

#ifndef MMI_PDA_SUPPORT
        MMISMS_OpenMsgBox(TXT_DELETED, IMAGE_PUBWIN_SUCCESS);
#endif
        MMIPUB_CloseWaitWin(MMISMS_DELETE_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_MTBOX_WIN_ID);
        MMK_CloseWin(MMISMS_SENDSUCCBOX_WIN_ID);
        MMK_CloseWin(MMISMS_MOBOX_WIN_ID);
        if (win_data_ptr->is_del_all)
        {
            MMK_CloseWin(MMISMS_SHOW_CHAT_SMS_WIN_ID);
        }
        win_data_ptr->is_del_all = FALSE;
    }
    else if (msg_type == MMISMS_TYPE_MMS)
    {
        is_mms_exist = TRUE;
    }
    else if (MMISMS_TYPE_SMS == msg_type && MMISMS_DEL_FAIL == del_result)
    {
        MMK_PostMsg(MMISMS_DELETE_ANIM_WIN_ID, MMISMS_MSG_DELETE_NEXT, PNULL, 0);
    }
}


/*****************************************************************************/
//     Description : to handle the message of delete query window    
//    Global resource dependence : 
//  Author:rong.gu
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDeleteCallBack(void *user_date,MMISMS_DELETE_DATE_INFO *data_info_ptr)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)user_date;
    if(PNULL == win_data_ptr || PNULL == data_info_ptr)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("mmisms chatwin del call back");
    MMISMS_SetAllMsgMarked(MMISMS_BOX_CHAT, FALSE);

    MMISMS_SetDelAllFlag(FALSE);  
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        //finish the del
        if(data_info_ptr->is_mms_del)
        {
            MMIAPIMMS_SaveListInfo();
        }
       
        if(win_data_ptr->is_del_all ||!win_data_ptr->is_have_item )
        {
            //UpdateChatItemDisplayAfterDelSingle(win_data_ptr);
            MMK_CloseWin(win_data_ptr->win_id);
        }
       
    }
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMISMS_ClearOperInfo();       
    }
    //for 126318
    MMK_CloseWin(MMISMS_QUERY_WIN_ID);
    MMK_CloseWin(MMISMS_DELETE_ANIM_WIN_ID);
    return TRUE;
}

/*****************************************************************************/
//     Description : to handle the message of delete query window
//    Global resource dependence :
//  Author:fengming.huang
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinDelQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMISMS_OPERATE_DATA_INFO_T data_info = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_FALSE;
    } 
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            //HandleChatOperateDelorDelAll(win_data_ptr);		
#ifdef MMI_TIMERMSG_SUPPORT
        if (MMISMS_GetIsTimerMsg())
        {
            MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
    
            return recode;
        }
#endif
        
        MMISMS_SetDelAllFlag(TRUE);
        data_info.box_type = MMISMS_BOX_CHAT; 
        
        MMISMS_EnterDelWaitWin(&data_info,HandleChatWinDeleteCallBack,(void*)win_data_ptr);
        MMK_CloseWin(win_id);
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:

            
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            MMISMS_SetCurChatMsgMarked(pb_num, FALSE);
            
            win_data_ptr->is_del_all = FALSE;

            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

#ifdef MMI_SMS_MOVE_SUPPORT
/*****************************************************************************/
//  Description : to handle the message of move query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_MOVE_DATA_INFO_T data_info = {0};
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_FALSE;
    }
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:         
            MMISMS_SetMoveAllFlag(TRUE);
            MMISMS_SetCurMsgMarked(TRUE);    
            data_info.oper_status = MMISMS_MOVE_SMS_TO_SECURITY;
            data_info.box_type = MMISMS_BOX_CHAT;
            data_info.pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            //data_info.chat_index = GUILIST_GetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
            MMISMS_EnterMoveWaitWin(&data_info,HandleChatWinMoveFinishCallback,win_data_ptr);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetCurMsgMarked(FALSE);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenMoveQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
            
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        win_data_ptr->del_index = active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID;
        win_data_ptr->is_del_all = FALSE;
        MMISMS_SetDelIndex(win_data_ptr->del_index);
        order_id = win_data_ptr->chat_item[win_data_ptr->del_index].cur_order_id;
    } 
    if(PNULL == order_id)
    {        
        return;
    }
    MMI_GetLabelTextByLang(TXT_MOVE, &prompt_str1);
    
    if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
    {
        prompt_str2.wstr_len = order_id->display_content.content_len;
        prompt_str2.wstr_ptr = order_id->display_content.content;
    }
    else
    {
        MMI_WSTRNCPY(str,
                     MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                     order_id->display_content.content,
                     MMISMS_QUERY_STRING_MAX_NUM,
                     MMISMS_QUERY_STRING_MAX_NUM);

        MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                       MMISMS_ELLIPSE_STR_LEN,
                       (const uint8 *)ellipse_str,
                       MMISMS_ELLIPSE_STR_LEN,
                       MMISMS_ELLIPSE_STR_LEN);
        prompt_str2.wstr_ptr = str;
        prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
    }
    MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinMoveQueryWinMsg );

}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveFinishCallback(void *user_date,MMISMS_MOVE_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)user_date;
    if(PNULL == data_info_ptr || PNULL == win_data_ptr)
    {
       SCI_TRACE_LOW("mmisms chat win move error");
       return ret;
    }
    win_data_ptr->is_del_all = FALSE;

    if (!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }
   
    MMISMS_SetMoveAllFlag(FALSE);
   
    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif

    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }


        MMISMS_WriteFolderType();
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        if(!win_data_ptr->is_have_item)
        {          
            MMK_CloseWin(win_data_ptr->win_id);
        }                
        ret = TRUE;
    }
    else if(MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }

        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);
        MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);        
        MMISMS_ClearOperInfo();   
        ret = FALSE;
    }     
    else if(MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        //finish the move
        if(data_info_ptr->is_mms_move)
        {
            MMIAPIMMS_SaveListInfo();
        }
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_MOVE_ANIM_WIN_ID);     
        MMISMS_ClearOperInfo();     
        ret = FALSE;
    }

    return ret;
}


/*****************************************************************************/
//  Description : to handle the message of move waiting window
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinMoveWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
	win_data_ptr  = MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return MMI_RESULT_TRUE;
    }
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
            break;

        case MSG_CLOSE_WINDOW:
            win_data_ptr->is_del_all = FALSE;

            if (!MMISMS_GetMoveSingleFlag())
            {
                MMISMS_ClearOperInfo();
            }
           
            MMISMS_SetMoveAllFlag(FALSE);
           
            //全部删除完毕，将未处理链表中的sms插入到相应的链表中
            MMISMS_InsertUnhandledSmsToOrder();
            MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            SCI_TRACE_LOW("mmisms move cancel");            
            MMISMS_SetMoveAllFlag(FALSE);
            MMISMS_WriteFolderType();
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}
#endif

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: liming.deng
//    Note:
/*****************************************************************************/
LOCAL void ChatWinOpenDelteQueryWin(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    uint8 disp_str_len = 0;
    uint8 disp_num_len = 0;
    uint8 delete_msg_num[6] = {0};
    wchar disp_num[10] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T tmp_prompt = {0};

    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2 + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    uint32 delete_sms_num = 0;

    if (MMISMS_MENU_DELBOX_ALL_ITEM_ID == win_data_ptr->menu_id)//删除全部
    {        
        delete_sms_num = MMISMS_GetSmsBoxDeleteAvailNum(MMISMS_GetCurBoxType());

        MMI_GetLabelTextByLang(TXT_DELETE, &prompt_str1);

        disp_num_len = sprintf((char *)delete_msg_num, "%ld ", delete_sms_num);

        MMI_STRNTOWSTR(disp_num, disp_num_len, (const uint8 *)delete_msg_num, disp_num_len, disp_num_len);

        if (delete_sms_num > 1)
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEMS, &tmp_prompt);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SMS_MESSAGE_ITEM, &tmp_prompt);
        }

        // the total string wstr_len
        disp_str_len =  disp_num_len + tmp_prompt.wstr_len + 1;

        // allocate memory
        prompt_str2.wstr_ptr = SCI_ALLOC_APP((disp_str_len + 1) * sizeof(wchar));
        SCI_MEMSET(prompt_str2.wstr_ptr, 0x00, ((disp_str_len + 1)*sizeof(wchar)));

        // copy the num
        MMI_WSTRNCPY(prompt_str2.wstr_ptr, disp_str_len, disp_num, disp_num_len, disp_num_len);

        prompt_str2.wstr_len = disp_num_len;

        // copy the prompt string
        MMI_WSTRNCPY(
            &(prompt_str2.wstr_ptr[prompt_str2.wstr_len]),
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_ptr,
            tmp_prompt.wstr_len,
            tmp_prompt.wstr_len
        );
        prompt_str2.wstr_len += tmp_prompt.wstr_len;

        if (0 < delete_sms_num)
        {
            MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinDelQueryWinMsg );
        }
        else
        {
            MMIPUB_OpenAlertWinByTextPtr(PNULL, PNULL, &prompt_str2, IMAGE_PUBWIN_WARNING, &query_win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        if (PNULL != prompt_str2.wstr_ptr)
        {
            SCI_FREE(prompt_str2.wstr_ptr);
            prompt_str2.wstr_ptr = PNULL;
        }
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_SMS_DELETE_MESSAGE, &prompt_str1);
        //order_id = win_data_ptr->chat_del_order_id;
        order_id = MMISMS_GetCurOperationOrderId();
        if (PNULL == order_id)
        {
            SCI_TRACE_LOW("mmisms order_id == PNULL when delete");
            return;
        }
        if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
        {
            prompt_str2.wstr_len = order_id->display_content.content_len;
            prompt_str2.wstr_ptr = order_id->display_content.content;
        }
        else
        {
            MMI_WSTRNCPY(str,
                         MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                         order_id->display_content.content,
                         MMISMS_QUERY_STRING_MAX_NUM,
                         MMISMS_QUERY_STRING_MAX_NUM);

            MMI_STRNTOWSTR( str + MMISMS_QUERY_STRING_MAX_NUM,
                            MMISMS_ELLIPSE_STR_LEN,
                            (const uint8 *)ellipse_str,
                            MMISMS_ELLIPSE_STR_LEN,
                            MMISMS_ELLIPSE_STR_LEN);
            prompt_str2.wstr_ptr = str;
            prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
        }

        MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinDelQueryWinMsg );
    }
}
/*****************************************************************************/
//  Description : to get phone number
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSGetPhoneNumber(uint8 *number_ptr, uint16 src_len, wchar *focus_num_ptr, uint16 dest_len)
{
    uint16 i = 0;
    uint16 new_len = 0;
    if (PNULL == number_ptr || PNULL == focus_num_ptr)
    {
        return;
    }
    for (i = 0; i < dest_len; i++)
    {
        if (0 == focus_num_ptr[i] || (new_len == src_len))
        {
            break;
        }

        if ('-' == focus_num_ptr[i] || (focus_num_ptr[i] & 0xFF00))
        {
            continue;
        }

        number_ptr[new_len++] = focus_num_ptr[i];
    }

    number_ptr[new_len] = '\0';
}

/*****************************************************************************/
//  Description :set chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 index,
    BOOLEAN is_active,
    MMISMS_ORDER_ID_T order_id
)
{
    if(PNULL == win_data_ptr)
    {
        //SCI_PASSERT(0,("mmisms win_data_ptr PNULL"));
        return;
    }
    win_data_ptr->chat_item[index].is_valid = is_active;
    win_data_ptr->chat_item[index].cur_order_id = order_id;
    win_data_ptr->chat_item[index].parent_ctrl_id = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
    //SCI_TRACE_LOW:"SetChatWinIndex: index is %d,is_active is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30054_112_2_18_2_55_22_861, (uint8*)"dd", index, is_active);
}

/*****************************************************************************/
//  Description :get chat win index
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMISMS_CHAT_INDEX_E GetChatWinIndex(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    uint32 *index_ptr,
    MMISMS_ORDER_ID_T order_id
)
{
    uint32 max = MMISMS_CHAT_MAX_ITEM;
    uint32 i = 0;
    MMISMS_CHAT_INDEX_E result = MMISMS_CHAT_INDEX_MAX;
    MMISMS_ORDER_ID_T cur_longsms_order_id = PNULL;

    //long sms
    if (order_id->flag.is_concatenate_sms)
    {
        for (i = 0; i < max; i++)
        {
            if ((win_data_ptr->chat_item[i].is_valid)
                    && (PNULL != win_data_ptr->chat_item[i].cur_order_id)
               )
            {
                cur_longsms_order_id = win_data_ptr->chat_item[i].cur_order_id;
                {
                    if (MMISMS_HaveSamePart(cur_longsms_order_id, order_id))
                    {
                        *index_ptr = i;
                        return MMISMS_CHAT_INDEX_SAME_LONG_SMS;
                    }
                }
            }
        }
    }

    for (i = 0; i < max; i++)
    {
        if (!win_data_ptr->chat_item[i].is_valid)
        {
            *index_ptr = i;
            result = MMISMS_CHAT_INDEX_NEW;
            break;
        }
    }
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    //SCI_TRACE_LOW:"GetChatWinIndex: index is %d,result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30107_112_2_18_2_55_22_862, (uint8*)"dd", i, result);
    return result;
}

/*****************************************************************************/
//  Description : to handle chat sms tag
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatSmsTag(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,MMI_CTRL_ID_T ctrl_id)
{
    GUITEXT_TAG_INFO_T tag_info = {0};
	MMIPUB_MENU_PARAM_T menuparam = {0};
    GUITEXT_GetCurTag(ctrl_id, &tag_info);                
    switch (tag_info.type)
    {
        case GUITEXT_TAG_PHONENUM:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_NUMBER_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_NUMBER_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_NUMBER_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractNumberOptOk;    
                //menuparam.func.OnMenuInit = HandleChatWinExtractNumberOptInit;     
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;

        case GUITEXT_TAG_URL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_URL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_URL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_URL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractUrlOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#ifdef MMIEMAIL_SUPPORT
        case GUITEXT_TAG_EMAIL:
            {
                menuparam.group_id = MENU_SMS_EXTRACT_EMAIL_POP_MENU;
                menuparam.win_id =   MMISMS_EXTRACT_EMAIL_POP_WIN_ID;
                menuparam.ctrl_id =  MMISMS_EXTRACT_EMAIL_POP_CTRL_ID;    
                menuparam.dst_handle = win_data_ptr->win_id;
                menuparam.func.OnMenuOk = HandleChatWinExtractEmailOptOk;    
                MMIPUB_OpenPubMenuWin( &menuparam ); 
            }
            break;
#endif
        default:
            break;
    }
}

/*****************************************************************************/
//     Description : open the SMS delete query window
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_EnterSMSChatReadWin(MMISMS_ORDER_ID_T cur_order_id)
{
    MMI_HANDLE_T win_handle = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMISMS_BCD_NUMBER_T mms_bcd_num = {0};

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T*)SCI_ALLOCAZ(sizeof(MMISMS_CHATREAD_WINDATA_T));

    if (win_data_ptr == PNULL)
    {
        SCI_TRACE_LOW("mmisms malloc error");
        return;
    }
    
    if(MMISMS_TYPE_SMS == cur_order_id->flag.msg_type)
    {
        SCI_MEMSET(&win_data_ptr->chat_number, 0x0, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&win_data_ptr->chat_number, &cur_order_id->number.bcd_number, sizeof(MMISMS_BCD_NUMBER_T));
        MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,
           win_data_ptr->chat_number.number,
           win_data_ptr->chat_number.number_len << 1,
           (char*)win_data_ptr->sender);
    }
    else if(MMISMS_TYPE_MMS == cur_order_id->flag.msg_type)
    {
        MMISMS_MMSNumberToBcd((char*)cur_order_id->number.sender, mms_bcd_num.number, &mms_bcd_num.number_len);
        SCI_MEMSET(&win_data_ptr->chat_number, 0x0, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMCPY(&win_data_ptr->chat_number, &mms_bcd_num, sizeof(MMISMS_BCD_NUMBER_T));
        SCI_MEMSET(win_data_ptr->sender, 0x0, MMISMS_NUMBER_MAX_LEN + 1);
        SCI_MEMCPY(win_data_ptr->sender, cur_order_id->number.sender, (sizeof(uint8)*MMISMS_NUMBER_MAX_LEN));
    }
    win_data_ptr->dual_sys  =MN_DUAL_SYS_MAX;
    MMISMS_SetGlobalChatNumber(win_data_ptr->chat_number,win_data_ptr->sender);
    win_handle  = MMK_CreateWin((uint32*)MMISMS_SHOW_CHAT_SMS_WIN_TAB, PNULL);
	//MMISMS_SetCurOperationOrderId(MMISMS_BOX_CHAT,cur_order_id);
	MMISMS_SetGlobalOperationOrderId(cur_order_id);
	
    if(win_handle == PNULL)
    {
        SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create error");
        SCI_FREE(win_data_ptr);
        return;
    }
    SCI_TRACE_LOW("mmisms MMISMS_SHOW_CHAT_SMS_WIN_TAB create success");
    MMK_SetWinUserData(win_handle, (ADD_DATA)win_data_ptr);

}


/*****************************************************************************/
//     Description : add the contact information to the SMS edit window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
LOCAL void AddContactInfo2Editbox(MMIPB_HANDLE_T handle)
{
    MMI_STRING_T contact_str = {0};
    BOOLEAN is_right = TRUE;

    contact_str.wstr_ptr = SCI_ALLOC_APP((MMISMS_MAX_MESSAGE_LEN + 1) * sizeof(wchar));
    SCI_MEMSET(contact_str.wstr_ptr, 0, ((MMISMS_MAX_MESSAGE_LEN + 1)*sizeof(wchar)));

    // get the detail information from the phone_book
    MMISMS_GetContactInfo((uint32)handle,
                          &contact_str,
                          MMISMS_MAX_MESSAGE_LEN
                         );

    is_right = GUIEDIT_InsertString(
                   MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID,
                   contact_str.wstr_ptr,
                   contact_str.wstr_len
               );
    if (!is_right)
    {
        // prompt the error information
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_BEYOND_LEN);
    }

    if (PNULL != contact_str.wstr_ptr)
    {
        SCI_FREE(contact_str.wstr_ptr);
        contact_str.wstr_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : opertate after del single chat item
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void UpdateChatItemDisplayAfterDelSingle(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMI_CTRL_ID_T del_child_ctrl = 0;
    int32 i = 0;

    MMI_CTRL_ID_T ctrl_id = 0;

    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return ;
    }
    win_data_ptr->del_index = MMISMS_GetDelIndex();
    win_data_ptr->is_have_item = FALSE;
    if (win_data_ptr->del_index < MMISMS_CHAT_MAX_ITEM)
    {
        del_child_ctrl = win_data_ptr->chat_item[win_data_ptr->del_index].parent_ctrl_id;
    }
    else
    {
        return;
    }
    GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, del_child_ctrl);
    SCI_MEMSET(&win_data_ptr->chat_item[win_data_ptr->del_index], 0x0, sizeof(MMISMS_CHAT_ITEM_T));

    for (i = 0; i < MMISMS_CHAT_MAX_ITEM; i++)
    {
        ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, (i + 1));
        if (0 == ctrl_id || (ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID || ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM))
        {
            ctrl_id = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, i);
            if (ctrl_id >= MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID && ctrl_id < MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + MMISMS_CHAT_MAX_ITEM)
            {
                win_data_ptr->is_have_item = TRUE;
                break;
            }

        }
    }

      
    if (win_data_ptr->is_have_item)    
    {
        if (MMK_IsOpenWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {
            MMK_SetAtvCtrl(MMISMS_SHOW_CHAT_SMS_WIN_ID, ctrl_id);
        }
#ifdef TOUCH_PANEL_SUPPORT         
        if (!MMK_IsFocusWin(MMISMS_SHOW_CHAT_SMS_WIN_ID))
        {       
            GUIFORM_ForceLayout(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID);
        }
#endif        
    }

    SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30331_112_2_18_2_55_23_865, (uint8*)"d", win_data_ptr->del_index);
    win_data_ptr->del_index = MMISMS_CHAT_MAX_ITEM;
    MMISMS_SetDelIndex(win_data_ptr->del_index);
    MMISMS_SetGlobalChatItem(win_data_ptr->chat_item,MMISMS_CHAT_MAX_ITEM);
}

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL uint16 GetChatItemDateWidth(void)   // 12小时制加大长度
{

     MMISET_TIME_DISPLAY_TYPE_E time_disp_type = MMISET_TIME_24HOURS;
     time_disp_type = MMIAPISET_GetTimeDisplayType();
     if(MMISET_TIME_24HOURS == time_disp_type)
     {
         return MMISMS_CHATFORM_SIM_DATE_WIDTH;
     }
     else
     {
         return MMISMS_CHATFORM_SIM_DATE_WIDTH + 4;
     }
}

/*****************************************************************************/
//  Description :create dyna child form
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void SetChatItemParam(
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
    MMISMS_ORDER_ID_T order_id
)
{
    GUIFORM_DYNA_CHILD_T parent_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl = {0};
    MMI_STRING_T content_str = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
    GUI_FONT_T font_size = SONG_FONT_20;
    GUI_COLOR_T font_color = MMI_BLACK_COLOR;
    uint32 index = 0;
    GUI_BG_T chat_bg = {0};
    GUI_BG_T chat_focus_bg = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUILABEL_INIT_DATA_T    label_init = {0};
    GUITEXT_INIT_DATA_T date_text_init = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T replace_child_ctrl = 0;
    MMI_STRING_T text_content = {0};
    BOOLEAN is_need_replace = FALSE;
    MMI_STRING_T long_content_str = {0};
    int32 i = 0;
    uint16 long_content_alloc_len = 0;
    GUI_BORDER_T border = {0};

    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_DYNA_CHILD_T date_form_child_ctrl = {0};
    uint16 ver_space_ptr = 0;
    wchar sms_time[20] = {0};

    MMISMS_CHAT_INDEX_E chat_index = MMISMS_CHAT_INDEX_NEW;

    MMI_STRING_T sms_time_str = {0};
    uint8 time_len = 0;
    uint16 disp_width = 0;
    uint16 child_form_width = 0;
    uint16 width = 0;
    BOOLEAN is_send_msg = FALSE;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    GUIFORM_DYNA_CHILD_T sim_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T date_sim_form_child_ctrl = {0};
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    GUI_COLOR_T sim_color = MMI_BLACK_COLOR;
#endif
    MMI_STRING_T sim_str = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;

    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(win_id);
    if(PNULL == win_data_ptr)
    {
        //SCI_PASSERT(0,("mmisms win_data_ptr error"));  
        return;
    }
    chat_index = GetChatWinIndex(win_data_ptr,&index, order_id);
    //index is full
    if (MMISMS_CHAT_INDEX_MAX == chat_index)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        //SCI_TRACE_LOW:"SetChatItemParam:not find index!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29530_112_2_18_2_55_21_860, (uint8*)"");
#ifdef MMIMMS_SMS_IN_1_SUPPORT   //短彩合一第0 个控件为MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 1);
#else
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 0);
#endif
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
        index = replace_child_ctrl - MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
    }
    else if (MMISMS_CHAT_INDEX_SAME_LONG_SMS == chat_index)
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        GUITEXT_GetString(MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index, &text_content);

        long_content_alloc_len = (content_str.wstr_len + text_content.wstr_len);
        long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
        long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
        long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));
        if (PNULL == long_content_str.wstr_ptr)
        {
            return;
        }
        SCI_MEMCPY(long_content_str.wstr_ptr, text_content.wstr_ptr, text_content.wstr_len * sizeof(wchar));

        ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index);
        for (i = 0; i < MMISMS_SPLIT_MAX_NUM; i++)
        {
            if (win_data_ptr->chat_item[index].split_sms_enable[i])
            {
                break;
            }
        }
        if (
            ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM != 0) && (order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM - 1) > i)
            || ((order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM == 0) && ((MMISMS_SPLIT_MAX_NUM - 1) > i)))
        {
            order_id = win_data_ptr->chat_item[index].cur_order_id;
        }

        is_need_replace = TRUE;
    }
    else
    {
        //get msg content
        MMISMS_ShowMsgContent(
            win_data_ptr->read_result.read_content.alphabet,
            win_data_ptr->read_result.read_content.length,
            &win_data_ptr->read_result,
            &win_data_ptr->read_result.time_stamp,
            &win_data_ptr->read_result.address,
            win_data_ptr->read_result.language,
            TRUE,
            FALSE,
            &content_str
        );

        //first reached of a long sms
        if (order_id->flag.is_concatenate_sms)
        {
            if ( !order_id->flag.is_receive_all)
            {
                if (!MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)) //聊天界面来短信，而非读短信
                {
                    long_content_alloc_len = content_str.wstr_len;
                    long_content_alloc_len = MAX(long_content_alloc_len, MMISMS_MAX_MESSAGE_LEN);
                    long_content_alloc_len = long_content_alloc_len * sizeof(wchar);
                    long_content_str.wstr_ptr = SCI_ALLOCAZ((long_content_alloc_len + 1) * sizeof(wchar));

                    if (PNULL == long_content_str.wstr_ptr)
                    {
                        SCI_FREE( content_str.wstr_ptr );
                        return;
                    }

                    ChatWinSetLongSmsItemContent(win_data_ptr,index, order_id, text_content, content_str, &long_content_str);
                    is_need_replace = TRUE;
                }
                else
                {
                    order_id = MMISMS_GetOperationOrderId(0);
                    ChatWinCalcChatItemSplitLenth(win_data_ptr,index);
                }
            }
            else
            {
                order_id = MMISMS_GetOperationOrderId(0);
            }
        }
    }

    SetChatWinIndex(win_data_ptr,index, TRUE, order_id);

    //create parent form
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type)
    {
        form_init.layout_type = GUIFORM_LAYOUT_SBS;
    }
    else
    {
        form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    }
    form_init.both_rect = both_rect;

    parent_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    parent_form_child_ctrl.is_bg = FALSE;
    parent_form_child_ctrl.is_get_active = TRUE;
    parent_form_child_ctrl.init_data_ptr = &form_init;
    parent_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;

    GUIFORM_CreatDynaChildCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,  &parent_form_child_ctrl);
    //GUIFORM_PermitChildBg(parent_form_child_ctrl.child_handle,FALSE);

    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
        label_init.align = GUILABEL_ALIGN_LEFT;
    }
    else
    {
        label_init.align = GUILABEL_ALIGN_RIGHT;
    }
    date_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    date_form_child_ctrl.is_bg = FALSE;
    date_form_child_ctrl.is_get_active = FALSE;
    date_form_child_ctrl.init_data_ptr = &label_init;
    date_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index;

    //set text param
    text_form_child_ctrl.guid = SPRD_GUI_TEXTBOX_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = TRUE;
    text_form_child_ctrl.init_data_ptr = &date_text_init;
    text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;

    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    child_height.add_data = 20;
#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT
    font_size = MMISMS_GetWinFont();
#endif
#endif
    if (MMITHEME_IsMainScreenLandscape())
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, PNULL, &disp_width);
    }
    else
    {
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &disp_width, PNULL);
    }
    if (!is_need_replace)
    {
        width = GUI_GetStringWidth(font_size, content_str.wstr_ptr, content_str.wstr_len); //没有char space的情况
        width += MMITHEME_TEXT_CHAR_SPACE * content_str.wstr_len;
    }
    else
    {
        width = GUI_GetStringWidth(font_size, long_content_str.wstr_ptr, long_content_str.wstr_len);
        width += MMITHEME_TEXT_CHAR_SPACE * long_content_str.wstr_len;
    }
    
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
//set anim param
    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type) //发送失败才显示图标
    {
#ifdef MMI_TIMERMSG_SUPPORT
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
#endif
        GUIFORM_DYNA_CHILD_T date_text_form_child_ctrl = {0};
        GUIANIM_INIT_DATA_T anim_init = {0};
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIFORM_INIT_DATA_T date_text_form_init = {0};
        GUIFORM_DYNA_CHILD_T anim_form_child_ctrl = {0};

        anim_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
        anim_form_child_ctrl.is_bg = FALSE;
        anim_form_child_ctrl.is_get_active = FALSE;
        anim_form_child_ctrl.init_data_ptr = &anim_init;
        anim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;
        anim_init.both_rect = both_rect;

        ctrl_info.is_ctrl_id = TRUE;
        ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;

#ifdef MMI_TIMERMSG_SUPPORT
        event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);
        if (event_fast_info_ptr != PNULL)
        {
            data_info.img_id = IMAGE_SMS_CHATBOX_MO_TIMERMSG_ICON;
        }
        else
#endif
        {
            if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                    && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
            {
                is_send_msg = TRUE;
            }
            if (is_send_msg)
            {
                data_info.img_id = IMAGE_SMS_CHATBOX_MO_SENDING_ICON;
            }
            else
            {
                data_info.img_id = IMAGE_SMS_CHATBOX_MO_FAIL_ICON;
            }
        }
        display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;

        date_text_form_init.layout_type = GUIFORM_LAYOUT_ORDER;
        //date_text_form_init.both_rect = both_rect;
        date_text_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_text_form_child_ctrl.is_bg = FALSE;
        date_text_form_child_ctrl.is_get_active = TRUE;
        date_text_form_child_ctrl.init_data_ptr = &date_text_form_init;
        date_text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID + index;

        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &anim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);

        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);

        {
            child_width.type = GUIFORM_CHILD_WIDTH_FIXED;

            child_form_width = MAX(width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH, MMISMS_SENDFAIL_DATE_TEXT_WIDTH);

            if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
            {
                child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
            }
            child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);
            GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, date_text_form_child_ctrl.child_handle, &child_width);
            child_form_width = width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH;
            if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
            {
                child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
            }
            child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);

            GUIFORM_SetChildWidth(date_text_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);

            GUIFORM_SetChildAlign(date_text_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
        }

        GUIFORM_PermitChildFont(date_text_form_child_ctrl.child_handle, FALSE);

        GUIFORM_SetChildHeight(date_text_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_height);
        GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    }
    else
    {
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡
        sim_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        sim_form_child_ctrl.is_bg = FALSE;
        sim_form_child_ctrl.is_get_active = FALSE;
        sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID + index;
        sim_form_child_ctrl.init_data_ptr = &label_init;

        form_init.layout_type = GUIFORM_LAYOUT_SBS;
        date_sim_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_sim_form_child_ctrl.is_bg = FALSE;
        date_sim_form_child_ctrl.is_get_active = FALSE;
        date_sim_form_child_ctrl.init_data_ptr = &form_init;
        date_sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID + index;

        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_sim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &sim_form_child_ctrl);


        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, date_sim_form_child_ctrl.child_handle, &child_height);

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_form_width = width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH ;
        if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
        {
            child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
        }
        child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);


        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMISMS_CHATFORM_SIM_LABEL_WIDTH;
        GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, sim_form_child_ctrl.child_handle, &child_width);
        
        if (MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type
            || MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type
            || MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type
            )
        {
            child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
            child_width.add_data = GetChatItemDateWidth();
            GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_width);
        }
#else
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_form_width = width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH;
        if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
        {
            child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
        }

        child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);
        GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, &child_width);

        GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_height);
#endif

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡
        child_form_width = MAX(width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH, MMISMS_SENDFAIL_DATE_TEXT_WIDTH);
#else
        child_form_width = MAX(width + MMISMS_TEXT_LAYOUT_MARGIN_WIDTH, MMISMS_SMS_DATE_WIDTH);
#endif
        if (child_form_width > (disp_width - MMISMS_CHATFORM_ANIM_WIDTH))
        {
            child_form_width =  disp_width - MMISMS_CHATFORM_ANIM_WIDTH;
        }

        child_width.add_data = MAX(MMISMS_SMS_MIN_TEXT_BG_WIDTH, child_form_width);

        GUIFORM_SetChildWidth(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, &child_width);
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡
        GUIFORM_PermitChildFont(date_sim_form_child_ctrl.child_handle, FALSE);
#endif
        GUIFORM_PermitChildFont(parent_form_child_ctrl.child_handle, FALSE);
        if ((MMISMS_MO_SEND_SUCC == win_data_ptr->read_result.mo_mt_type))
        {
            GUIFORM_SetChildAlign(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, parent_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
            GUIFORM_SetChildAlign(parent_form_child_ctrl.child_handle, text_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            GUIFORM_SetAlign(date_sim_form_child_ctrl.child_handle, GUIFORM_CHILD_ALIGN_RIGHT);
#endif
        }
    }
    
    time_len = MMISMS_GetDateTimeStringByOrder(order_id,TRUE,FALSE,TRUE,sms_time);
    
    sms_time_str.wstr_len = (uint16)time_len ;
    sms_time_str.wstr_ptr = sms_time ;

    GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);

    if (MMISMS_MO_SEND_FAIL == win_data_ptr->read_result.mo_mt_type)
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        MMI_STRING_T fail_str = {0};
#ifdef MMI_TIMERMSG_SUPPORT
        MMIACC_SMART_EVENT_FAST_TABLE_T *event_fast_info_ptr = PNULL;
        event_fast_info_ptr = MMISMS_TIMERMSG_GetEventByOrder(order_id);
        if (PNULL == event_fast_info_ptr)
#endif
        {
            if (MMISMS_TYPE_SMS == order_id->flag.msg_type
                    && (MMISMS_SENDSTATUS_SENDING == order_id->backsend_status || MMISMS_SENDSTATUS_WAITING == order_id->backsend_status))
            {
                MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
            }
            else
            {
                MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
            }
        }

        wstr.wstr_len = sms_time_str.wstr_len + fail_str.wstr_len + 1;

        str = SCI_ALLOCAZ((wstr.wstr_len + 1) * sizeof(wchar));

        if (PNULL == str)
        {
            SCI_FREE( content_str.wstr_ptr );
            SCI_FREE( long_content_str.wstr_ptr);
            return;
        }

        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, wstr.wstr_len , sms_time_str.wstr_ptr, sms_time_str.wstr_len, sms_time_str.wstr_len);
#ifdef MMI_TIMERMSG_SUPPORT
        if (PNULL == event_fast_info_ptr)
#endif
        {
            MMIAPICOM_Wstrcat(str, L" ");
            MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        }

        wstr.wstr_len = MMIAPICOM_Wstrlen(str);
        GUILABEL_SetText(date_form_child_ctrl.child_handle, &wstr, FALSE);
        
        SCI_FREE(str);

        if(order_id->flag.is_locked)
        {          
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_SMS_CHAT_LOCK_ICON);
        } 
        else
        {
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_NULL);
        }
        
        {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
        }
        
    }
    else
    {
        if(order_id->flag.is_locked)
        {         
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_SMS_CHAT_LOCK_ICON);
        }
        else
        {
            GUILABEL_SetIcon(date_form_child_ctrl.child_handle,IMAGE_NULL);
        }

        GUILABEL_SetText(date_form_child_ctrl.child_handle, &sms_time_str, FALSE);
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡    
        sim_name = MMIAPISET_GetSimName(order_id->flag.dual_sys);

        sim_str.wstr_len  = sim_name.wstr_len;
        sim_str.wstr_ptr = sim_name.wstr_arr;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
        if (order_id != PNULL)
        {
            sim_color = MMIAPISET_GetSimColor(order_id->flag.dual_sys);
        }
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, sim_color);
#else
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
#endif
        GUILABEL_SetText(sim_form_child_ctrl.child_handle, &sim_str, FALSE);
#endif
        {//temp debug	
        SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
        MMI_CheckAllocatedMemInfo();
        }
    }
    //set bg
    chat_bg.bg_type = GUI_BG_IMG;
    chat_focus_bg.bg_type = GUI_BG_IMG;
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))

    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHATREAD_TEXT_MARGIN_LEFT,
                            MMISMS_CHATREAD_TEXT_MARGIN_RIGHT,
                            MMISMS_CHATRAED_TEXT_MARGIN_TOP,
                            MMISMS_CHATREAD_TEXT_MARGIN_BOTTOM);
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHATREAD_TEXT_MARGIN_RIGHT,
                            MMISMS_CHATREAD_TEXT_MARGIN_LEFT,
                            MMISMS_CHATRAED_TEXT_MARGIN_TOP,
                            MMISMS_CHATREAD_TEXT_MARGIN_BOTTOM);
    }

    GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, PNULL, &ver_space_ptr);
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, PNULL, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, PNULL, PNULL);
    //GUITEXT_SetBg(text_form_child_ctrl.child_handle,&chat_bg);
    GUIFORM_SetSpace(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, PNULL, &ver_space_ptr);

    //no border
    border.type = GUI_BORDER_NONE;
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);

    GUITEXT_SetTagType(text_form_child_ctrl.child_handle, GUITEXT_TAG_ALL, PNULL);

    //set font
#ifdef MMI_PDA_SUPPORT
    if ((MMISMS_MT_TO_BE_READ == win_data_ptr->read_result.mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == win_data_ptr->read_result.mo_mt_type))
    {
#ifdef MMI_PDA_SUPPORT
        if (!MMITHEME_IsIstyle())
#endif
        {
            font_color = MMI_GRAY_WHITE_COLOR;
        }
    }
#endif
    GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);

    //set string
    if (!is_need_replace)
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            content_str.wstr_ptr,
            content_str.wstr_len,
            FALSE
        );
    }
    else
    {
        GUITEXT_SetString(
            text_form_child_ctrl.child_handle,
            long_content_str.wstr_ptr,
            long_content_str.wstr_len,
            FALSE
        );
    }

#ifdef MMI_PDA_SUPPORT
    GUITEXT_SetHandleTpMsg(
        FALSE,  //in:
        text_form_child_ctrl.child_handle    //in:
    );
#endif

    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if ( MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id))
#endif        
    {
        if (MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
                || MMK_IsFocusWin(win_id)
                || MMK_IsFocusWin(MMISMS_SEND_ANIM_WIN_ID))

        {
            MMK_SetAtvCtrl(win_id, text_form_child_ctrl.child_handle);
#ifndef  MMI_PDA_SUPPORT
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
#endif
        }
    }

    if (PNULL != content_str.wstr_ptr)
    {
        SCI_FREE( content_str.wstr_ptr );
        content_str.wstr_ptr = PNULL;
    }
    if (PNULL != long_content_str.wstr_ptr)
    {
        SCI_FREE( long_content_str.wstr_ptr );
        long_content_str.wstr_ptr = PNULL;
    }
    {//temp debug	
    SCI_TRACE_MID("[MMI] MMI_CheckAllocatedMemInfo file:%s, line:%d", _D_FILE_NAME, _D_FILE_LINE);
    MMI_CheckAllocatedMemInfo();
    }
    
}

#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description :update readed mms icon 
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MMISMS_UpdateReadedMMSIcon(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    GUIANIM_CTRL_INFO_T ctrl_info = {0};
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + win_data_ptr->del_index;
    data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
    display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;
    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
}

/*****************************************************************************/
//  Description :create dyna child form for mms
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void SetMMSChatItemParam(
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
    MMI_WIN_ID_T win_id,
    GUI_BOTH_RECT_T both_rect,
	MMISMS_ORDER_ID_T order_id
)
{
    GUIFORM_DYNA_CHILD_T parent_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T anim_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T text_form_child_ctrl = {0};
    MMI_STRING_T content_str = {0};
    MMI_STRING_T time_str = {0};
    GUIFORM_INIT_DATA_T form_init = {0};
    GUIFORM_INIT_DATA_T date_sim_form_init = {0};
    GUI_FONT_T font_size = SONG_FONT_20;
    GUI_COLOR_T font_color = MMI_BLACK_COLOR;
    uint32 index = 0;
    GUI_BG_T chat_bg = {0};
    GUI_BG_T chat_focus_bg = {0};
    GUIANIM_INIT_DATA_T anim_init = {0};
    GUIANIM_CTRL_INFO_T ctrl_info = {0};
    GUIANIM_DATA_INFO_T data_info = {0};
    GUIANIM_DISPLAY_INFO_T display_info = {0};
    GUITEXT_INIT_DATA_T text_init = {0};
    GUIFORM_CHILD_WIDTH_T child_width = {0};
    MMI_CTRL_ID_T replace_child_ctrl = 0;
    GUI_BORDER_T border = {0};
    MMISMS_ORDER_INFO_T *mms_display_info = PNULL;
    GUIFORM_CHILD_HEIGHT_T   child_height = {0};
    GUIFORM_DYNA_CHILD_T date_form_child_ctrl = {0};
    GUILABEL_INIT_DATA_T    label_init = {0};

    GUIFORM_DYNA_CHILD_T date_text_form_child_ctrl = {0};
    GUIFORM_INIT_DATA_T date_text_form_init = {0};
    uint16 ver_space_ptr = 0;
    uint16 chat_item_index = 0;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    GUIFORM_DYNA_CHILD_T sim_form_child_ctrl = {0};
    GUIFORM_DYNA_CHILD_T date_sim_form_child_ctrl = {0};
#ifdef MMI_MULTISIM_COLOR_SUPPORT
    GUI_COLOR_T sim_color = MMI_BLACK_COLOR;
#endif
    MMI_STRING_T sim_str = {0};
    MMISET_SIM_NAME_T sim_name = {0};
#endif

    chat_item_index = GetChatWinIndex(win_data_ptr,&index, order_id);
    if (PNULL == order_id)
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam:PNULL == order_id"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29154_112_2_18_2_55_20_856, (uint8*)"");
        return;
    }
    if (MMISMS_MO_DRAFT == order_id->flag.mo_mt_type)
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam:MMISMS_MO_DRAFT"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29159_112_2_18_2_55_20_857, (uint8*)"");
        return;
    }
    //index is full
    if (MMISMS_CHAT_INDEX_MAX == chat_item_index)
    {
        mms_display_info = ReadMMSContent(order_id->record_id);
        GetMMSTimeSujectSize(&content_str, &time_str, mms_display_info);
        //SCI_TRACE_LOW:"SetMMSChatItemParam:not find index!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29167_112_2_18_2_55_20_858, (uint8*)"");
#ifdef MMIMMS_SMS_IN_1_SUPPORT   //短彩合一第0 个控件为MMISMS_SHOW_CHAT_SMS_EDITCTRL_FORM_ID
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 1);
#else
        replace_child_ctrl = GUIFORM_GetChildCtrlId(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, 0);
#endif
        GUIFORM_DestroyDynaChildCtrl(MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID, replace_child_ctrl);
        index = replace_child_ctrl - MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID;
    }
    else if (MMISMS_CHAT_INDEX_NEW == chat_item_index)
    {
        mms_display_info = ReadMMSContent(order_id->record_id);
        GetMMSTimeSujectSize(&content_str, &time_str, mms_display_info);
    }
    else
    {
        //SCI_TRACE_LOW:"SetMMSChatItemParam: else"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_29183_112_2_18_2_55_20_859, (uint8*)"");
        return;
    }

    if (PNULL == mms_display_info)
    {
        return;
    }
    SetChatWinIndex(win_data_ptr,index, TRUE, order_id);

    //create parent form
    form_init.layout_type = GUIFORM_LAYOUT_SBS;
    form_init.both_rect = both_rect;
    parent_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    parent_form_child_ctrl.is_bg = FALSE;
    parent_form_child_ctrl.is_get_active = TRUE;
    parent_form_child_ctrl.init_data_ptr = &form_init;
    parent_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_PARENT_ITEM_FORM_CTRL_ID + index;
    GUIFORM_CreatDynaChildCtrl(win_id, MMISMS_SHOW_CHAT_SMS_FORM_CTRL_ID,  &parent_form_child_ctrl);

    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
    {
        label_init.align = GUILABEL_ALIGN_RIGHT;
    }
    date_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
    date_form_child_ctrl.is_bg = FALSE;
    date_form_child_ctrl.is_get_active = FALSE;
    date_form_child_ctrl.init_data_ptr = &label_init;
    date_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_ITEM_FORM_CTRL_ID + index;

    date_text_form_init.layout_type = GUIFORM_LAYOUT_ORDER;
    //date_text_form_init.both_rect = both_rect;
    date_text_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
    date_text_form_child_ctrl.is_bg = FALSE;
    date_text_form_child_ctrl.is_get_active = TRUE;
    date_text_form_child_ctrl.init_data_ptr = &date_text_form_init;
    date_text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_TEXT_FORM_CTRL_ID + index;

    anim_form_child_ctrl.guid = SPRD_GUI_ANIM_ID;
    anim_form_child_ctrl.is_bg = FALSE;
    anim_form_child_ctrl.is_get_active = FALSE;
    anim_form_child_ctrl.init_data_ptr = &anim_init;
    anim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;
    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMISMS_SHOW_CHAT_SMS_LABEL_ITEM_FORM_CTRL_ID + index;

    if (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type  ||
            MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type ||
            MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type
       )
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_MMS_NOTIFY_NEW_ICON;
        }
        else
        {
            data_info.img_id = IMAGE_SMS_MMS_NOTIFY_NEW_ICON;
        }
    }
    else if (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type || MMISMS_MO_SEND_SUCC == mms_display_info->mo_mt_type || MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_READ_MMS_ICON;        
        }
        else
        {
            data_info.img_id = IMAGE_SMS_READ_MMS_ICON;
        }
    }
    else
    {
        if(order_id->flag.is_locked)
        {
            data_info.img_id = IMAGE_SMS_LOCK_NOT_READ_MMS_ICON;
        }
        else
        { 
            data_info.img_id = IMAGE_SMS_NOT_READ_MMS_ICON;
        }
    }
    //set anim align
    display_info.align_style = GUIANIM_ALIGN_RIGHT_VMIDDLE;

    //set text param
    text_form_child_ctrl.guid = SPRD_GUI_TEXTBOX_ID;
    text_form_child_ctrl.is_bg = FALSE;
    text_form_child_ctrl.is_get_active = TRUE;
    text_form_child_ctrl.init_data_ptr = &text_init;
    text_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID + index;

    //create parent child ctrl
    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
    {
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &anim_form_child_ctrl);
    }
    else
    {
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &anim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);
    }


#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡
    if (MMISMS_MO_SEND_FAIL != mms_display_info->mo_mt_type)
    {
        sim_form_child_ctrl.guid = SPRD_GUI_LABEL_ID;
        sim_form_child_ctrl.is_bg = FALSE;
        sim_form_child_ctrl.is_get_active = FALSE;
        sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_SIM_ITEM_FORM_CTRL_ID + index;
        sim_form_child_ctrl.init_data_ptr = &label_init;

        date_sim_form_init.layout_type = GUIFORM_LAYOUT_SBS;
        date_sim_form_child_ctrl.guid = SPRD_GUI_FORM_ID;
        date_sim_form_child_ctrl.is_bg = FALSE;
        date_sim_form_child_ctrl.is_get_active = FALSE;
        date_sim_form_child_ctrl.init_data_ptr = &date_sim_form_init;
        date_sim_form_child_ctrl.child_handle = MMISMS_SHOW_CHAT_SMS_DATE_AND_SIM_ITEM_FORM_CTRL_ID + index;

        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &date_sim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_sim_form_child_ctrl.child_handle,  &sim_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, parent_form_child_ctrl.child_handle,  &date_text_form_child_ctrl);

        GUIFORM_SetChildHeight(parent_form_child_ctrl.child_handle, date_sim_form_child_ctrl.child_handle, &child_height);
        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = MMISMS_CHATFORM_SIM_LABEL_WIDTH;
        GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, sim_form_child_ctrl.child_handle, &child_width);

        child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
        child_width.add_data = GetChatItemDateWidth();
        GUIFORM_SetChildWidth(date_sim_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_width);
    }
    else
#endif
    {
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &date_form_child_ctrl);
        GUIFORM_CreatDynaChildCtrl(win_id, date_text_form_child_ctrl.child_handle,  &text_form_child_ctrl);
        GUIFORM_SetChildHeight(date_text_form_child_ctrl.child_handle, date_form_child_ctrl.child_handle, &child_height);
    }

    //set anim
    GUIANIM_SetParam(&ctrl_info, &data_info, PNULL, &display_info);
    child_width.type = GUIFORM_CHILD_WIDTH_FIXED;
    child_width.add_data = MMISMS_CHATFORM_ANIM_WIDTH;
    GUIFORM_SetChildWidth(parent_form_child_ctrl.child_handle, ctrl_info.ctrl_id, &child_width);
    GUIFORM_SetSpace(parent_form_child_ctrl.child_handle, PNULL, &ver_space_ptr);
    //set bg
    chat_bg.bg_type = GUI_BG_IMG;
    chat_focus_bg.bg_type = GUI_BG_IMG;

    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type) )
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MO_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MO_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHATREAD_TEXT_MARGIN_LEFT,
                            MMISMS_CHATREAD_TEXT_MARGIN_RIGHT,
                            MMISMS_CHATRAED_TEXT_MARGIN_TOP,
                            MMISMS_CHATREAD_TEXT_MARGIN_BOTTOM);
    }
    else
    {
#if defined MMI_PDA_SUPPORT && defined MMI_ISTYLE_SUPPORT
        if (MMITHEME_IsIstyle())
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MT_ICON_ISTYLE;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON_ISTYLE;
        }
        else
#endif
        {
            chat_bg.img_id       = IMAGE_SMS_CHATBOX_BG_MT_ICON;
            chat_focus_bg.img_id = IMAGE_SMS_CHATBOX_BG_MT_FOCUS_ICON;
        }

        //set margin
        GUITEXT_SetMarginEx(text_form_child_ctrl.child_handle,
                            MMISMS_CHATREAD_TEXT_MARGIN_RIGHT,
                            MMISMS_CHATREAD_TEXT_MARGIN_LEFT,
                            MMISMS_CHATRAED_TEXT_MARGIN_TOP,
                            MMISMS_CHATREAD_TEXT_MARGIN_BOTTOM);
    }
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle, &chat_bg, PNULL, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle, &chat_focus_bg, PNULL, PNULL);

    border.type = GUI_BORDER_NONE;
    GUIFORM_SetChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);
    GUIFORM_SetActiveChildTheme(parent_form_child_ctrl.child_handle , PNULL, &border, PNULL);

    GUIFORM_PermitChildFont(date_text_form_child_ctrl.child_handle, FALSE);

#ifdef MMI_PDA_SUPPORT
    if (!MMITHEME_IsIstyle())
    {
        GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
    }
#if defined(MMI_ISTYLE_SUPPORT)	
    else
    {
        GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
    }
#endif	
#else
    GUILABEL_SetFont(date_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_BLACK_COLOR);
#endif

    if (MMISMS_MO_SEND_FAIL == mms_display_info->mo_mt_type)
    {
        wchar *str = PNULL;
        MMI_STRING_T wstr = {0};
        MMI_STRING_T fail_str = {0};

        if (MMISMS_TYPE_MMS == order_id->flag.msg_type
                && (MMIAPIMMS_IsProcessMMSById(order_id->record_id) || MMIAPIMMS_IsMMSInDownloadQueue(order_id->record_id)))
        {
            MMI_GetLabelTextByLang(TXT_SENDING, &fail_str);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_COMM_SEND_FAILED, &fail_str);
        }
        str = SCI_ALLOC_APPZ((MMISMS_CHAT_SENDFAIL_STR_MAX_LEN + 1) * sizeof(wchar));
        if (PNULL == str)
        {
            SCI_FREE( content_str.wstr_ptr );
            return;
        }
        wstr.wstr_ptr = str;
        MMI_WSTRNCPY(str, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN , time_str.wstr_ptr, time_str.wstr_len, MIN(time_str.wstr_len, MMISMS_CHAT_SENDFAIL_STR_MAX_LEN));
        MMIAPICOM_Wstrcat(str, L" ");
        MMIAPICOM_Wstrncat(str, fail_str.wstr_ptr, fail_str.wstr_len);
        wstr.wstr_len = MMIAPICOM_Wstrlen(str);
        GUILABEL_SetText(date_form_child_ctrl.child_handle, &wstr, FALSE);
        SCI_FREE(str);
    }
    else
    {
        GUILABEL_SetText(date_form_child_ctrl.child_handle, &time_str, FALSE);
#ifndef MMI_MULTI_SIM_SYS_SINGLE   //多卡 界面 标记卡    
        sim_name = MMIAPISET_GetSimName(order_id->flag.dual_sys);

        sim_str.wstr_len  = sim_name.wstr_len;
        sim_str.wstr_ptr = sim_name.wstr_arr;
#ifdef MMI_MULTISIM_COLOR_SUPPORT
        sim_color = MMIAPISET_GetSimColor(order_id->flag.dual_sys);
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, sim_color);
#else
        GUILABEL_SetFont(sim_form_child_ctrl.child_handle, MMI_DEFAULT_SMALL_FONT, MMI_DARK_GRAY_COLOR);
#endif
        GUILABEL_SetText(sim_form_child_ctrl.child_handle, &sim_str, FALSE);
#endif
    }
#ifdef MMI_PDA_SUPPORT
    //set font
    if ((MMISMS_MT_TO_BE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_HAVE_READ == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_NOTIFY == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NEED_NOT_DOWNLOAD == mms_display_info->mo_mt_type) ||
            (MMISMS_MT_NOT_DOWNLOAD == mms_display_info->mo_mt_type)
       )
    {
        if (!MMITHEME_IsIstyle())
        {
            font_color = MMI_GRAY_WHITE_COLOR;
        }
    }
    else
    {
        if (!MMITHEME_IsIstyle())
        {
            font_color = MMI_DARK_GRAY_COLOR;
        }
    }
#else
    font_color = MMI_BLACK_COLOR;
#endif

#ifdef MMI_SMS_FONT_SETTING_SUPPORT
#ifdef MMS_SMS_IN_1_SUPPORT
    font_size = MMISMS_GetWinFont();
#endif
#endif
    GUITEXT_SetFont(text_form_child_ctrl.child_handle, &font_size, &font_color);
    //set string
    GUITEXT_SetString(
        text_form_child_ctrl.child_handle,
        content_str.wstr_ptr,
        content_str.wstr_len,
        FALSE
    );

#ifdef MMI_PDA_SUPPORT
    GUITEXT_SetHandleTpMsg(
        FALSE,  //in:
        text_form_child_ctrl.child_handle    //in:
    );
#endif
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    if ( MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID != MMK_GetActiveCtrlId(win_id))
#endif        
    {
        if (MMK_IsFocusWin(win_id)
            || MMK_IsFocusWin(MMISMS_READSMS_WAIT_WIN_ID)
            || MMK_IsFocusWin(MMISMS_SEND_ANIM_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_NEWMSG_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_SENDING_WIN_ID)//发送waiting框未关闭返回时，设置active控件
            || MMK_IsFocusWin(MMIMMS_DOWNLOADING_WIN_ID)
            || MMK_IsFocusWin(MMIMMS_PREVIEW_WIN_ID)//预览mms界面打开时，设置active控件。防止预览界面关闭后，active控件丢失。
          )
        {
            MMK_SetAtvCtrl(win_id, text_form_child_ctrl.child_handle);
        }
    }
#ifndef  MMI_PDA_SUPPORT
    GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_READ, MIDDLE_BUTTON, FALSE);
#endif
    if (PNULL != content_str.wstr_ptr)
    {
        SCI_FREE( content_str.wstr_ptr );
    }

    if (PNULL != time_str.wstr_ptr)
    {
        SCI_FREE( time_str.wstr_ptr );
    }
}
#endif
#ifdef MMS_SUPPORT
/*****************************************************************************/
//  Description : 获取彩信的相关信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL MMISMS_ORDER_INFO_T *ReadMMSContent(int32 record_id)
{
    MMISMS_ORDER_INFO_T *mms_order_info_ptr = PNULL;

    mms_order_info_ptr = MMIAPIMMS_GetListItemByID(record_id);

    return mms_order_info_ptr;
}

/*****************************************************************************/
//  Description : 获取彩信相关部分信息
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void GetMMSTimeSujectSize(MMI_STRING_T *string_ptr, MMI_STRING_T  *time_str, MMISMS_ORDER_INFO_T *mms_display_info)
{
    uint16 copy_offset = 0;
    uint8 time_len = 0;
    wchar time[20] = {0};
    wchar cr_ch = MMIAPICOM_GetCRCh();
    uint8 cr_len = 1;
    MMI_STRING_T subject_temp_string = {0};
    MMI_STRING_T subject_none_string = {0};
    uint32 total_size = 0;
    MMI_STRING_T size_temp_string = {0};
    //char display_str_size[MMIMMS_SIZE_STRING_MAX_LEN+1] ={0};
    wchar display_wstr_size[MMIMMS_SIZE_STRING_MAX_LEN + 1] = {0};
    uint8 display_wstr_size_len = 0;

    if (PNULL == string_ptr || PNULL == time_str || PNULL == mms_display_info)
    {
        return;
    }
    total_size = mms_display_info->file_size;
    //time length

    time_len = MMISMS_GetDateTimeStringBySecond(mms_display_info->time,TRUE,FALSE,TRUE,time);
    
    time_str->wstr_ptr = SCI_ALLOCAZ((time_len + 1) * sizeof(wchar));
    if (PNULL == time_str->wstr_ptr)
    {
        return;
    }

    if (0 < time_len)
    {
        MMI_WSTRNCPY(
            time_str->wstr_ptr,
            time_len,
            (const wchar *)time,
            time_len,
            time_len
        );

        time_str->wstr_len = time_len;
    }

    //subject length
    MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT, &subject_temp_string);
    if (0 < mms_display_info->display_content.content_len)
    {
        string_ptr->wstr_len += subject_temp_string.wstr_len + mms_display_info->display_content.content_len + cr_len;
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_BLANK, &subject_none_string);
        string_ptr->wstr_len += subject_temp_string.wstr_len + subject_none_string.wstr_len + cr_len;
    }
    //size length
    MMI_GetLabelTextByLang(TXT_DETAIL_SIZE , &size_temp_string);
    display_wstr_size_len = MMIAPIMMS_GetMMSFileSizeStr(total_size, display_wstr_size);
    string_ptr->wstr_len += size_temp_string.wstr_len + display_wstr_size_len;

    // allocate memory
    string_ptr->wstr_ptr = SCI_ALLOC_APPZ((string_ptr->wstr_len + 1) * sizeof(wchar));
    if (PNULL == string_ptr->wstr_ptr )
    {
        //not free time
        return;
    }

    //subject:
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        subject_temp_string.wstr_len,
        subject_temp_string.wstr_ptr,
        subject_temp_string.wstr_len,
        subject_temp_string.wstr_len
    );
    copy_offset += subject_temp_string.wstr_len;
    //subject content
    if (0 < mms_display_info->display_content.content_len)
    {
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            mms_display_info->display_content.content_len,
            (const wchar*)mms_display_info->display_content.content,
            mms_display_info->display_content.content_len,
            mms_display_info->display_content.content_len
        );
        copy_offset += mms_display_info->display_content.content_len;
    }
    else
    {
        MMI_WSTRNCPY(
            &(string_ptr->wstr_ptr[copy_offset]),
            subject_none_string.wstr_len,
            subject_none_string.wstr_ptr,
            subject_none_string.wstr_len,
            subject_none_string.wstr_len
        );
        copy_offset += subject_none_string.wstr_len;
    }
    // CR
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        cr_len,
        &cr_ch,
        cr_len,
        cr_len
    );
    copy_offset += cr_len;

    //size:
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        size_temp_string.wstr_len,
        size_temp_string.wstr_ptr,
        size_temp_string.wstr_len,
        size_temp_string.wstr_len
    );
    copy_offset += size_temp_string.wstr_len;
    //size value
    MMI_WSTRNCPY(
        &(string_ptr->wstr_ptr[copy_offset]),
        display_wstr_size_len,
        display_wstr_size,
        display_wstr_size_len,
        display_wstr_size_len
    );
}
#endif
/*****************************************************************************/
//  Description : get chat content , delete time
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISMS_ChatWinGetItemContent(MMI_STRING_T *content)
{
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMI_STRING_T text_string = {0};

    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id < MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID || active_ctrl_id > MMISMS_MAX_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID)
    {
        //SCI_TRACE_LOW:"mmisms chat not focus in chat item"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_30347_112_2_18_2_55_23_866, (uint8*)"");
        return FALSE;
    }

    GUITEXT_GetString(active_ctrl_id, &text_string);

    if (text_string.wstr_ptr == PNULL)
    {
        return FALSE;
    }

    content->wstr_len = text_string.wstr_len;

    content->wstr_ptr = SCI_ALLOCA(content->wstr_len * sizeof(wchar) + 1);
    if (PNULL == content->wstr_ptr)
    {
        return FALSE;
    }
    SCI_MEMSET(content->wstr_ptr, 0x00, (content->wstr_len * sizeof(wchar) + 1));

    SCI_MEMCPY(content->wstr_ptr, text_string.wstr_ptr, (content->wstr_len * sizeof(wchar) + 1));

    return TRUE;
}

#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopySimToMe(void)
{
    if (MMISMS_IsUpdateOk())
    {
        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_ME);
        HandleChatWinOpenCopyQuery();
    }
}

/*****************************************************************************/
//     Description : handle chat win copy
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinCopyMeToSim(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    if (MMISMS_IsUpdateOk())
    {
        uint16 sim_num = 0;
        uint16 sim_ok = 0;
        MN_DUAL_SYS_E copy_dual_sys = MN_DUAL_SYS_1;
        uint32 copy_sim_num = 0;
        uint32 copy_mark_num = 0;
        MMIPB_BCD_NUMBER_T pb_num = {0};
        //uint16 cur_index = 0;
		//MMI_CTRL_ID_T cur_ctrl_id = 0;
        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

        MMISMS_SetCurMsgMarked(TRUE);

        MMISMS_SetOperStatus(MMISMS_COPY_SMS_TO_SIM);

        if (0 == sim_num)
        {
            MMISMS_SetCurMsgMarked(FALSE);
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
        }
        else if (1 == sim_num)
        {
            copy_dual_sys = (MN_DUAL_SYS_E)sim_ok;

            //cur_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
            //cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            copy_sim_num = MMISMS_GetCurChatMarkedMsgNumByDualsys(pb_num, MN_SMS_STORAGE_SIM, copy_dual_sys);
            copy_mark_num = MMISMS_GetCurChatMarkedMsgNum(pb_num, MN_SMS_STORAGE_ALL);

            if (copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurMsgMarked(FALSE);
            }
            else
            {
                MMISMS_SetCurSaveDualSys(copy_dual_sys);
                //MMISMS_OperateCopy();
                HandleChatWinOpenCopyQuery();
            }
        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        else
        {
            MMISMS_SelectSIMFunc(HandleChatWinCopySelectSimCallback, PNULL, FALSE);
        }
#endif
    }
}


/*****************************************************************************/
//  Description : open the SMS copy query window
//  Global resource dependence :
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenCopyQuery(void)
{
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMISMS_ORDER_ID_T order_id = PNULL;
    wchar str[ MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN + 1] = {0};
    uint8 ellipse_str[MMISMS_ELLIPSE_STR_LEN] = { '.', '.', '.'};
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;

    MMISMS_SetCurMsgMarked(TRUE);

    MMI_GetLabelTextByLang(TXT_COMMON_COPY, &prompt_str1);
    order_id = MMISMS_GetCurOperationOrderId();
    if (order_id->display_content.content_len <= MMISMS_QUERY_STRING_MAX_NUM )
    {
        prompt_str2.wstr_len = order_id->display_content.content_len;
        prompt_str2.wstr_ptr = order_id->display_content.content;
    }
    else
    {
        MMI_WSTRNCPY(str,
                     MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN * 2,
                     order_id->display_content.content,
                     MMISMS_QUERY_STRING_MAX_NUM,
                     MMISMS_QUERY_STRING_MAX_NUM);

        MMI_STRNTOWSTR(str + MMISMS_QUERY_STRING_MAX_NUM,
                       MMISMS_ELLIPSE_STR_LEN,
                       (const uint8 *)ellipse_str,
                       MMISMS_ELLIPSE_STR_LEN,
                       MMISMS_ELLIPSE_STR_LEN);
        prompt_str2.wstr_ptr = str;
        prompt_str2.wstr_len = (uint16)( MMISMS_QUERY_STRING_MAX_NUM + MMISMS_ELLIPSE_STR_LEN );
    }

    MMIPUB_OpenQueryWinByTextPtr(&prompt_str1, &prompt_str2, IMAGE_PUBWIN_QUERY, &query_win_id, HandleChatWinCopyQueryWinMsg );

}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyFinishCallback(void *user_date,MMISMS_COPY_RESULT_INFO *data_info_ptr)
{
    MMI_RESULT_E ret = FALSE;
    if(data_info_ptr == PNULL)
    {
       return ret;
    }
    MMISMS_SetMoveAllFlag(FALSE);

    if (!MMISMS_GetMoveSingleFlag())
    {
        MMISMS_ClearOperInfo();
    }

    //全部删除完毕，将未处理链表中的sms插入到相应的链表中
    MMISMS_InsertUnhandledSmsToOrder();
    MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
    MMIAPIALM_CheckAndStartDelayedAlarm();
    MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
    MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
    if (MMISMS_FINISH_OPER == data_info_ptr->ret_value || MMISMS_NO_ERR == data_info_ptr->ret_value) //begin deleting sms
    {
        MMISMS_WriteFolderType();
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = TRUE;
    }
    else if (MMISMS_NO_ENOUGH_SPACE == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_NO_SPACE, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        //MMK_CloseWin(MMISMS_SHOWMSG_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = FALSE;
    }
    else if (MMISMS_FINISH_OPER_ERR == data_info_ptr->ret_value)
    {
        MMISMS_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        MMIPUB_CloseWaitWin(MMISMS_COPY_ANIM_WIN_ID);
        MMISMS_ClearOperInfo();
        ret = FALSE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : to handle the message of copy query window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyQueryWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T query_win_id = MMISMS_QUERY_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;    
    MMISMS_COPY_DATA_INFO_T data_info = {0};
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr PNULL");
        return recode;
    }
    switch (msg_id)
    {
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            //HandleChatWinOperateCopy();
          
            #ifdef MMI_TIMERMSG_SUPPORT
            if (MMISMS_GetIsTimerMsg())
            {
                MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);
        
                return recode;
            }
            #endif
            MMISMS_SetMoveAllFlag(TRUE);
            MMISMS_SetCurMsgMarked(TRUE);  
            data_info.box_type = MMISMS_BOX_CHAT;
            data_info.oper_status = MMISMS_GetCurrentOperStatus();
            data_info.pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
                //GUILIST_GetCurItemIndex(MMISMS_CHATBOX_LISTBOX_CTRL_ID);
            MMISMS_EnterCopyWaitWin(&data_info,HandleChatWinCopyFinishCallback,PNULL);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetCurMsgMarked(FALSE);
            //MMK_CloseWin(win_id);
            MMIPUB_CloseQuerytWin(&query_win_id);
            break;

        default:
            recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

/*****************************************************************************/
//  Description : to operate the copy
//  Global resource dependence :
//  Author:feingming.huang
//  Note:
/*****************************************************************************/
LOCAL void HandleChatWinOperateCopy(void)
{
    MMI_STRING_T prompt_str = {0};

#ifdef MMI_TIMERMSG_SUPPORT
    if (MMISMS_GetIsTimerMsg())
    {
        MMISMS_OpenAlertWarningWin(TXT_SMS_BUSY);

        return;
    }
#endif

    MMISMS_SetMoveAllFlag(TRUE);
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);

    MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMISMS_COPY_ANIM_WIN_ID, IMAGE_NULL,
                       ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_CUSTOMER, HandleChatWinCopyWaitWinMsg);

    MMISMS_SetCurMsgMarked(TRUE);

    MMIAPICOM_SendSignalToMMITask(MMISMS_MSG_COPY_NEXT);
}

/*****************************************************************************/
//  Description : to handle the message of copy waiting window
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleChatWinCopyWaitWinMsg(
    MMI_WIN_ID_T        win_id,    //IN:
    MMI_MESSAGE_ID_E    msg_id,    //IN:
    DPARAM                param    //IN:
)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
            break;

        case MSG_CLOSE_WINDOW:
            MMISMS_SetMoveAllFlag(FALSE);

            if (!MMISMS_GetMoveSingleFlag())
            {
                MMISMS_ClearOperInfo();
            }

            //全部删除完毕，将未处理链表中的sms插入到相应的链表中
            MMISMS_InsertUnhandledSmsToOrder();
            MMISMS_ReadMMSPushFromUnhandledList(); //读取mms push 消息并传给mms模块
            MMIAPIALM_CheckAndStartDelayedAlarm();
            MAIN_SetMsgState(TRUE);
#ifdef MMI_TIMERMSG_SUPPORT
            MMISMS_TIMERMSG_CheckTimerMSG(TRUE);
#endif
            break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMISMS_SetMoveAllFlag(FALSE);
            MMISMS_WriteFolderType();
            if (!MMISMS_GetMoveSingleFlag())
            {
                MMK_CloseWin(win_id);
            }
            break;

        default:
            recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
            break;
    }

    return (recode);
}

#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_SMS_COPY_SUPPORT
/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence :
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChatWinCopySelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    uint32 copy_sim_num = 0;
    uint32 copy_mark_num = 0;
    MMIPB_BCD_NUMBER_T pb_num = {0};
    //MMI_CTRL_ID_T cur_ctrl_id = MMISMS_CHATBOX_LISTBOX_CTRL_ID;
    //uint16 cur_index = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
    if (isReturnOK)
    {
        if (!MMIAPIPHONE_IsSimCardOk((MN_DUAL_SYS_E)dual_sys))
        {
            MMIPUB_OpenAlertWarningWin(TXT_SIM_UNAVAILABLE);
            MMISMS_SetCurMsgMarked(FALSE);
        }
        else
        {
            //cur_index = GUILIST_GetCurItemIndex(cur_ctrl_id);
            pb_num = MMISMS_SmsBCDToPbBCD(win_data_ptr->chat_number);
            copy_sim_num = MMISMS_GetCurChatMarkedMsgNumByDualsys(pb_num, MN_SMS_STORAGE_SIM, (MN_DUAL_SYS_E)dual_sys);
            copy_mark_num = MMISMS_GetCurChatMarkedMsgNum(pb_num, MN_SMS_STORAGE_ALL);

            if (copy_sim_num == copy_mark_num)
            {
                MMIPUB_OpenAlertWarningWin(TXT_SMS_NO_MSG_COPY);
                MMISMS_SetCurMsgMarked(FALSE);

            }
            else
            {
                MMISMS_SetCurSaveDualSys((MN_DUAL_SYS_E)dual_sys);
                HandleChatWinOpenCopyQuery();
            }
        }
    }

    return recode;
}
#endif
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description : speed dial edit select contact callback
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void EditSelectVcardCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMI_PB_VCARD_SELECTED, (DPARAM)handle);
}

/*****************************************************************/
//  Description :短信编辑菜单选择联系人的callback
//  Global resource dependence : none
//  Author:
//  Note:
//  Date:
/*****************************************************************/
LOCAL void EditSmsSelectContactCallback(MMIPB_HANDLE_T handle)
{
    MMK_SendMsg(MMISMS_SHOW_CHAT_SMS_WIN_ID, MMI_SELECT_CONTACT_CNF, (DPARAM)handle);
}

/*****************************************************************************/
//  Description :mms edit insert video
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertVideo(MMI_WIN_ID_T pre_win_id)
{
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MOVIE_ALL, 0, 0, 0, PNULL, 0, -1};

    select_info.max_size = MMIMMS_MMS_LIMIT_SIZE;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectMovieWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_VIDEO);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertMusic(MMI_WIN_ID_T pre_win_id)
{
    int32 music_max_size     = 0;
    MMIFMM_SELECT_INFO_T        select_info = {MMIFMM_MUSIC_ALL, 0, 0, 0, PNULL, 0, -1};
    music_max_size = MMIMMS_MMS_LIMIT_SIZE;

    select_info.max_size = music_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectMusicWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_MUSIC);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}

/*****************************************************************************/
//  Description :mms edit insert pic
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SmsChatEditInsertPic(MMI_WIN_ID_T pre_win_id)
{
    MMIFMM_SELECT_INFO_T    select_info =
    {
        MMIFMM_PIC_ALL, 0,
        0, 0, PNULL, 0, -1
    };

    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] = MMIFMM_PIC_ALL;
    select_info.max_size = 0;
    select_info.win_id = pre_win_id;
#ifdef DRM_SUPPORT
    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL;
#endif
    if (MMIAPIFMM_OpenSelectPictureWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_PIC);
    }
    else
    {
#ifdef NANDBOOT_SUPPORT
        MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}

/*****************************************************************************/
//  Description :mms edit insert normal attachment
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSEditInsertNormalAttachment(MMI_WIN_ID_T pre_win_id)
{
    int32 file_max_size               = 0;
    MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL, 0, 0, 0, PNULL, 0, -1};

    file_max_size = MMIMMS_MMS_LIMIT_SIZE;

    select_info.max_size = file_max_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
    if (MMIAPIFMM_OpenSelectAllFileWin(&select_info))
    {
#ifdef DRM_SUPPORT
        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
        limit_value.method_level_limit = MMIFMM_DRM_METHOD_LIMIT_FL | MMIFMM_DRM_METHOD_LIMIT_CD;
        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
#endif
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_ADD_ATTACH);
    }
    else
    {
        MMISMS_SetChatEditInsertOpType(MMISMS_INSERT_INIT);
#ifdef NANDBOOT_SUPPORT
        MMIPUB_OpenAlertWarningWin(TXT_ADD_FAIL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_NO_SD_CARD_ALERT);
#endif
    }
    return;
}

/*****************************************************************************/
//  Description :聊天处理现拍,现摄,现录流程
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSHandleSnapshot(MMIFILE_FILE_INFO_T *file_info, MMIFMM_FILE_TYPE_E   file_type)
{
    MMI_STRING_T      str_ptr = {0};
    MMI_STRING_T      str_to = {0};
    uint16 wnumber[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    MMI_STRING_T mms_content_str = {0};
    uint16  num_len = 0;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    
    GUIEDIT_GetString(MMISMS_SHOW_CHAT_SMS_EDITCTRL_ID, &str_ptr);
    mms_content_str.wstr_ptr = SCI_ALLOCAZ((str_ptr.wstr_len + 1) * sizeof(wchar));
    if (PNULL == mms_content_str.wstr_ptr)
    {
        //SCI_TRACE_LOW:"mmisms alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT, MMISMSAPP_WINTAB_28671_112_2_18_2_55_19_852, (uint8*)"");
        return;
    }
    //SCI_MEMSET((uint8 *)mms_content_str.wstr_ptr, 0x00, ((str_ptr.wstr_len+1)*sizeof(wchar)));
    MMIAPICOM_Wstrncpy(mms_content_str.wstr_ptr, str_ptr.wstr_ptr, str_ptr.wstr_len);
    mms_content_str.wstr_len = MMIAPICOM_Wstrlen(mms_content_str.wstr_ptr);

    num_len = MMISMS_GetChatWinWNumber(win_data_ptr,wnumber);
    str_to.wstr_ptr = wnumber;
    str_to.wstr_len = num_len;
#ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIAPIMMS_SetIsEmailAddrExist(win_data_ptr->is_email_number);
#endif       
    if (MMIAPIMMS_EditAddMMSAttach(file_info, file_type, PNULL, 0, PNULL, &mms_content_str, &str_to, TRUE))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_SMS_TO_MMS, TXT_NULL, IMAGE_PUBWIN_WAIT, PNULL, PNULL, MMIPUB_SOFTKEY_NONE, PNULL);
    }

    if (PNULL != mms_content_str.wstr_ptr)
    {
        SCI_FREE(mms_content_str.wstr_ptr);
        mms_content_str.wstr_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDC(void)
{
    MMIDC_WORK_PARAM_T work_param = {0};
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR;

    work_param.work_mode = MMIDC_WORK_DC;
    //work_param.param.dc.max_photo_size = WORK_PHOTO_SIZE_640_480; //use default value
    work_param.work_callback = SMSDCWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param);
    if (MMIDC_WORK_SUCCESS != error_ret)
    {
        SCI_TRACE_LOW("sms dc error %d", error_ret);
    }

}

#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
//  Description :进入录音
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenRecord(void)
{
    MMIRECORD_WORK_PARAM_T parameter = {0};

    parameter.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT;
    parameter.max_file_time = NULL;
    parameter.record_support_type = MMIRECORD_WORK_FORMAT_WAV;
    parameter.work_callback = SMSRecordWorkCallBack;
    if(MMIAPIRECORD_OpenMainWin(&parameter) == MMIRECORD_WORK_SUCCESS)
    {
        SCI_TRACE_LOW("[MMIMMS]MMSOpenRecord mms record error!");
    }
}

/*****************************************************************************/
//  Description :录音 后 的 处 理
//  Global resource dependence :
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void SMSRecordWorkCallBack(MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    MMIFILE_FILE_INFO_T file_info = {0};

    if (PNULL == data_info_ptr)
    {
        return;
    }

    if (MMIRECORD_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_MUSIC);
            }
        }
    }

    return ;
}
#endif

/*****************************************************************************/
//  Description :拍 照 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIMMS_WINDOW_ID_E quer_win_id = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (PNULL == data_info_ptr)
    {
        return ret;
    }
    
    if (MMIDC_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                LCD_INFO_T lcd_info = {0};
                uint32 reduce_pic_size = 0;
                uint16 reduce_width = 0;
                uint16 reduce_height = 0;
                GUIANIM_TYPE_E pic_type = GUIANIM_TYPE_NONE;
                GUILCD_GetInfo((LCD_ID_E)GUI_MAIN_LCD_ID, &lcd_info);
                reduce_width = MIN(MMIMMS_PIC_REDUCED_WIDTH_MAX, lcd_info.lcd_width);
                reduce_height = MIN(MMIMMS_PIC_REDUCED_HEIGHT_MAX, lcd_info.lcd_height);

                reduce_pic_size = reduce_width * reduce_height * 2;

                pic_type = MMIAPICOM_GetImgaeType(file_info.file_name, file_info.file_name_len);
                if (GUIANIM_TYPE_IMAGE_JPG == pic_type)
                {
                    win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_PICTURE;
                    if (PNULL != win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        SCI_FREE(win_data_ptr->sms_chat_insert_info.file_info_ptr);
                        win_data_ptr->sms_chat_insert_info.file_info_ptr = PNULL;
                    }
                    win_data_ptr->sms_chat_insert_info.file_info_ptr = (MMIFILE_FILE_INFO_T *)SCI_ALLOCAZ(sizeof(MMIFILE_FILE_INFO_T));
                    if (PNULL == win_data_ptr->sms_chat_insert_info.file_info_ptr)
                    {
                        win_data_ptr->sms_chat_insert_info.file_type = MMIFMM_FILE_TYPE_NORMAL;
                        return ret;
                    }

                    SCI_MEMCPY((char *)win_data_ptr->sms_chat_insert_info.file_info_ptr, (char *)&file_info, sizeof(MMIFILE_FILE_INFO_T));
                    MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_ADJUST_PIC, IMAGE_PUBWIN_QUERY, &quer_win_id, PNULL);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_MMS_MULTIFILE_TOO_BIG);
                }
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_PICTURE);
            }
        }
    }

    return ret;
}

/*****************************************************************************/
//  Description :现拍图片
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL void SMSOpenDV(void)
{
    MMIDC_WORK_PARAM_T work_param = {0};
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR;

    work_param.work_mode = MMIDC_WORK_DV;
    work_param.param.dv.max_file_size = MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT;
    work_param.param.dv.max_video_size = WORK_VIDEO_SIZE_160_128;
    work_param.work_callback = SMSDVWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param);
    if (MMIDC_WORK_SUCCESS != error_ret)
    {
        SCI_TRACE_LOW("sms dv error %d", error_ret);
    }

}

/*****************************************************************************/
//  Description :录 像 后 的 处 理
//  Global resource dependence :
//  Author:rong.gu
//  Note:
/*****************************************************************************/
LOCAL int32 SMSDVWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0;
    MMIFILE_FILE_INFO_T file_info = {0};

    if (PNULL == data_info_ptr)
    {
        return ret;
    }

    if (MMIDC_WORK_SUCCESS == error_ret)
    {
        if (data_info_ptr->file_name_len > 0)
        {
            MMIAPIFMM_GetFileInfoFormFullPath(data_info_ptr->file_name, data_info_ptr->file_name_len, &file_info);
            if (0 == file_info.file_size )
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ADD_FAIL, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else if (file_info.file_size > MMIMMS_MMS_LIMIT_SIZE - MMIMMS_SIZE_LEFT)
            {
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_MMS_MULTIFILE_TOO_BIG, TXT_NULL, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                SMSHandleSnapshot(&file_info, MMIFMM_FILE_TYPE_MOVIE);
            }
        }
    }
    return ret;
}

#endif

/*****************************************************************************/
//  Description : 处理阅读界面来一条相同号码的情况
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN ChatWinSetLongSmsItemContent(
                                               MMISMS_CHATREAD_WINDATA_T *win_data_ptr,
                                               uint32  index,
                                               MMISMS_ORDER_ID_T order_id,
                                               MMI_STRING_T orig_content,
                                               MMI_STRING_T new_insert_content,
                                               MMI_STRING_T *string_ptr //out
                                               )
{
    int32 i = 0;
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    uint16 pos = 0;
    uint16 prompt_len = 0;

    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;

    uint16 split_pos_before_len = 0;
    uint16 split_pos_after_len = 0;
    uint16 split_pos_after_pos = 0;
    
    if (PNULL == order_id || PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms param error");
        return FALSE ;
    }
     
    chat_item = &win_data_ptr->chat_item[index];

    //SCI_TRACE_LOW:"MMISMS_SetChatLongSmsItemContet:seq_len is %d,ref_num is %d,max is %d,seq is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMS_READ_5245_112_2_18_2_53_0_177,(uint8*)"dddd",chat_item->split_sms_length[order_id->longsms_seq_num],order_id->longsms_max_num,order_id->longsms_seq_num);
    max_num = order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    seq_num = order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
    seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num; 
            
    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    SCI_TRACE_LOW("ChatWinSetLongSmsItemContent:orig_content.wstr_len is %d",orig_content.wstr_len);
    SCI_TRACE_LOW("ChatWinSetLongSmsItemContent:split[0] is %d,split[1] is %d,split[2] is %d,split[3] is %d",chat_item->split_sms_length[0],chat_item->split_sms_length[1],chat_item->split_sms_length[2],chat_item->split_sms_length[3]);
    if(orig_content.wstr_len == 0)  //当前长短信第一个到达的信息
    {                    
        for(i = 0; i < max_num; i++)
        {
            if(i + 1 == seq_num)
            {               
                MMIAPICOM_Wstrncpy(
                &string_ptr->wstr_ptr[pos],
                new_insert_content.wstr_ptr,
                new_insert_content.wstr_len
                );
                pos += new_insert_content.wstr_len;                         
                chat_item->split_sms_length[i] = new_insert_content.wstr_len;
                chat_item->split_sms_enable[i] = TRUE;
            }
            else
            {
                if (MN_SMS_UCS2_ALPHABET != win_data_ptr->read_result.read_content.alphabet
                && MMIAPICOM_IsASCIIString(sms_lost_promt.wstr_ptr, sms_lost_promt.wstr_len))//ascii
                {
                    uint8 temp_prompt_str[MMISMS_PROMPT_STR_LEN + 1] = {0};
                    uint16 default_len = 0;
                    
                    sms_lost_promt.wstr_len = MIN(sms_lost_promt.wstr_len, MMISMS_PROMPT_STR_LEN);
                    
                    MMI_WSTRNTOSTR(temp_prompt_str, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_ptr, MMISMS_PROMPT_STR_LEN, sms_lost_promt.wstr_len);
                    
                    prompt_len = sms_lost_promt.wstr_len;
                    
                    default_len = MMIAPICOM_Default2Wchar(temp_prompt_str, &string_ptr->wstr_ptr[pos], sms_lost_promt.wstr_len);
                }
                else
                {
                    MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_ptr,
                    sms_lost_promt.wstr_len,
                    sms_lost_promt.wstr_len);                
                }
                pos += sms_lost_promt.wstr_len;  
                chat_item->split_sms_length[i] = sms_lost_promt.wstr_len;
                chat_item->split_sms_enable[i] = FALSE;
            }
        } 
    }
    else
    {
        for(i = 0; i < seq_num - 1; i++)
        {
            split_pos_before_len += chat_item->split_sms_length[i];  
        }  
    
        split_pos_after_pos = split_pos_before_len + sms_lost_promt.wstr_len;            
        
        for(i = seq_num; i < max_num; i++)
        {
            split_pos_after_len += chat_item->split_sms_length[i];  
        } 
        
        if(split_pos_before_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            split_pos_before_len,
            orig_content.wstr_ptr,
            split_pos_before_len,
            split_pos_before_len);
            pos += split_pos_before_len;
        }
        
        MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
        new_insert_content.wstr_len,
        new_insert_content.wstr_ptr,
        new_insert_content.wstr_len,
        new_insert_content.wstr_len);
        pos += new_insert_content.wstr_len;
        chat_item->split_sms_length[seq_num - 1] = new_insert_content.wstr_len;
        
        if(split_pos_after_len != 0)
        {
            MMI_WSTRNCPY(&string_ptr->wstr_ptr[pos],
            orig_content.wstr_len,
            orig_content.wstr_ptr + split_pos_after_pos,
            split_pos_after_len,
            split_pos_after_len);
            pos += split_pos_after_len;        
        }
    }
    
    string_ptr->wstr_len = pos;
    return TRUE;
}


/*****************************************************************************/
//  Description : to set chat long sms content
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void ChatWinCalcChatItemSplitLenth(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,uint32  index)
{
    MMISMS_CHAT_ITEM_T *chat_item = PNULL;
    MMI_STRING_T sms_lost_promt = {0};
    uint16 max_num = 0;
    uint16 seq_num = 0;
    uint16 i = 0;
    MMISMS_ORDER_ID_T         cur_order_id = PNULL;
    if(PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr error");
        return ;
    }

    MMI_GetLabelTextByLang(TXT_SMS_LOST_PROMPT,&sms_lost_promt);
    chat_item = &win_data_ptr->chat_item[index];
    cur_order_id = MMISMS_GetOperationOrderId(0);
    if(cur_order_id != PNULL)
	{
        max_num = cur_order_id->longsms_max_num % MMISMS_SPLIT_MAX_NUM;
	}
    max_num = (0 == max_num ) ? MMISMS_SPLIT_MAX_NUM : max_num;
    
    while( i < MMISMS_SPLIT_MAX_NUM )
    {
        cur_order_id = MMISMS_GetOperationOrderId(i);
        if(cur_order_id != PNULL)
        {
            seq_num = cur_order_id->longsms_seq_num % MMISMS_SPLIT_MAX_NUM;
            seq_num = (0 == seq_num ) ? MMISMS_SPLIT_MAX_NUM : seq_num;         
            index = seq_num - 1;
            chat_item->split_sms_length[index] = win_data_ptr->read_result.read_content.split_sms_length[index];
            chat_item->split_sms_enable[i] = TRUE;             
        }
        i++;  
    }
    for(i = 0; i < max_num; i++)
    {   
        if(0 == chat_item->split_sms_length[i])
        {
            chat_item->split_sms_length[i] = sms_lost_promt.wstr_len; 
            chat_item->split_sms_enable[i] = FALSE;
        }        
    }    
}

/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    uint8 *number_ptr = PNULL;
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    CC_RESULT_E cc_result = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if(PNULL == cur_order_id)
    {
        return;
    }
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    number_ptr = SCI_ALLOCAZ(tag_info.str_len + 1);
    if (PNULL == number_ptr)
    {
        return;
    }
    SMSGetPhoneNumber(number_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len); //对-连接的号码处理
  
    switch (menu_id)
    {
        case MMISMS_MENU_DIAL_ITEM_ID:
        case MMISMS_MENU_VIDEOCALL_ITEM_ID:
        case MMISMS_MENU_IPDIAL_ITEM_ID:
            num_len = SCI_STRLEN((char *)number_ptr);
            if (MMISMS_MENU_DIAL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_NORMAL_CALL,
                                PNULL
                            );
            }
            else if (MMISMS_MENU_VIDEOCALL_ITEM_ID == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_VIDEO_CALL,
                                PNULL
                            );
            }
            else
            {
                cc_result = MMIAPICC_MakeCall(
                                (MN_DUAL_SYS_E)cur_order_id->flag.dual_sys,
                                number_ptr,
                                num_len,
                                PNULL,
                                PNULL,
                                CC_CALL_SIM_MAX,
                                CC_CALL_IP_CALL,
                                PNULL
                            );
            }
            break;
        case MMISMS_MENU_ANSWER_SMS_ITEM_ID:
            MMIAPISMS_AnswerMessage((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, PNULL, number_ptr,
                                    (uint8)(strlen((char *)number_ptr) > MMIMMS_MAX_ADDRESS_LEN ? MMIMMS_MAX_ADDRESS_LEN : strlen((char *)number_ptr)));
            break;
        case MMISMS_MENU_ANSWER_MMS_ITEM_ID:
        {
            num_len = strlen((char *)number_ptr);
            //answer mms
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)number_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;

        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            MMIAPIPB_AddContactWin(&pb_bcd, PB_GROUP_ALL, MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
        }
        break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            // save to phonebook
            MMIAPICOM_GenPartyNumber(number_ptr, (uint16)SCI_STRLEN((char *)number_ptr), &party_num);

            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY(
                pb_bcd.number,
                party_num.bcd_num,
                pb_bcd.number_len
            );
            //  MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
        }
        break;
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
        case MMISMS_MENU_EXTRACT_ADDTO_BLACKLIST_ITEM_ID:
            HandleChatAddSenderToBlackList((uint8 *)number_ptr);
            break;
#endif

        default :
            break;
    }    
    SCI_FREE(number_ptr);
}

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : add sender to blacklist
//  Global resource dependence :
//  Author:jixin.xu
//  Note:
/*****************************************************************************/
LOCAL void HandleChatAddSenderToBlackList(
    uint8 *num_array_ptr    //IN:
)
{
    MMIPB_BCD_NUMBER_T    pb_bcd = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};

    MMIAPICOM_GenPartyNumber(num_array_ptr, (uint16)SCI_STRLEN((char *)num_array_ptr), &party_num);
    pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
    pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
    SCI_MEMCPY(
        pb_bcd.number,
        party_num.bcd_num,
        pb_bcd.number_len
    );
    MMISMS_AddSenderToBlackList(pb_bcd);
}
#endif


/*****************************************************************************/
//     Description : handle mms chat item opt
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractNumberOptInit(MMIPUB_MENU_NOTIFY_T *param )
{
    
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractUrlOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    wchar  *url_winparam_ptr              = PNULL;
#ifndef PB_SUPPORT_LOW_MEMORY
    MMIPB_CUSTOM_TEXT_T pb_url = {0};
#endif
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    if(PNULL == cur_order_id)
    {
        return;
    }
        
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    url_winparam_ptr = SCI_ALLOCAZ((tag_info.str_len + 1) * sizeof(wchar));
    if( PNULL == url_winparam_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     
    MMIAPICOM_Wstrncpy(url_winparam_ptr, tag_info.str_ptr, tag_info.str_len);

    switch (menu_id)
    {
        case MMISMS_MENU_EXACT_URL_BROWSE_ITEM_ID:
        {
          
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));
    
            SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    
    #ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ACCESS_URL;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
#ifdef  CMCC_UI_STYLE
                entry_param.is_alert = TRUE;
#endif                
                MMIAPIBROWSER_Entry(&entry_param);
            }
    #endif
    
            SCI_FREE(url_ptr);
            url_ptr = PNULL;          
        }
        break;
        case MMISMS_MENU_EXTRACT_URL_ADDTOBOOKBARK_ITEM_ID:
        {
            uint8 *url_ptr = PNULL;
            uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
    
            url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
            if ( PNULL == url_ptr)
            {
                break;
            }
    
            //SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
    
            GUI_WstrToUTF8(url_ptr, str_len, url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
    #ifdef BROWSER_SUPPORT
            {
                MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
    
                entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                entry_param.dual_sys = MN_DUAL_SYS_MAX;
                entry_param.url_ptr = (char *)url_ptr;
                entry_param.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&entry_param);
            }
    #endif
            SCI_FREE(url_ptr);
            url_ptr = PNULL;
        }
        break;
    
    #ifndef PB_SUPPORT_LOW_MEMORY
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                MMIAPIPB_AddContactWin(&pb_url, PB_GROUP_ALL, MMIPB_ADD_TYPE_URL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            uint16 str_len = (uint16)MMIAPICOM_Wstrlen(url_winparam_ptr);
    
            if (str_len > 0)
            {
                str_len = MIN(str_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                SCI_MEMSET(&pb_url, 0x00, sizeof(MMIPB_CUSTOM_TEXT_T));
                MMIAPICOM_Wstrncpy(pb_url.wstr,
                                   url_winparam_ptr,
                                   str_len);
                pb_url.wstr_len = str_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_url,MMIPB_ADD_TYPE_URL);
                MMIAPIPB_AddToExistContact(&pb_url, MMIPB_ADD_TYPE_URL, PNULL);
            }
        }
        break;
    #endif
    #endif
    
        default :
            break;
    }
    
    SCI_FREE(url_winparam_ptr);     
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinExtractEmailOptOk(MMIPUB_MENU_NOTIFY_T *param )
{
    uint32 menu_id = 0;   
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    MMI_CTRL_ID_T active_ctrl_id = 0;
    MMISMS_ORDER_ID_T cur_order_id = PNULL;
    GUITEXT_TAG_INFO_T tag_info = {0};
    uint8 num_len = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject                 = {0};
    uint8 *email_ptr = PNULL;
    MMIPB_MAIL_T pb_email = {0};
    
    if( PNULL == param )
    {
        SCI_TRACE_LOW("mmisms HandleSmsChatMenuOptOK param pnull");
        return;
    }
    menu_id = param->menu_id;
    
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(param->dst_handle);
    if (PNULL == win_data_ptr)
    {
        SCI_TRACE_LOW("mmisms win_data_ptr param PNULL");
        return;
    }
       
    active_ctrl_id = MMK_GetActiveCtrlId(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if (active_ctrl_id >= MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID && (active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID) < MMISMS_CHAT_MAX_ITEM)
    {
        cur_order_id = win_data_ptr->chat_item[active_ctrl_id - MMISMS_SHOW_CHAT_SMS_ITEM_FORM_CTRL_ID].cur_order_id;
    }
    GUITEXT_GetCurTag(active_ctrl_id, &tag_info);

    if (PNULL == cur_order_id )
    {
        return;
    }
    //进入文件管理器界面选择文件

    email_ptr = SCI_ALLOCAZ((tag_info.str_len + 1) * sizeof(wchar));
    if( PNULL == email_ptr )
    {
        SCI_TRACE_LOW("mmisms alloc error");
        return;
    }     

    MMI_WSTRNTOSTR((uint8 *)email_ptr, tag_info.str_len , tag_info.str_ptr, tag_info.str_len, tag_info.str_len);
    
    switch (menu_id)
    {
        case MMISMS_MENU_EXTRACT_SEND_MMS_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                string_to.wstr_ptr = (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
                MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                string_to.wstr_len = num_len;
                MMIAPIMMS_GetSubjectById(cur_order_id->record_id, &string_subject);
                MMIAPIMMS_AnswerMMS((MN_DUAL_SYS_E)cur_order_id->flag.dual_sys, &string_subject, &string_to);
                SCI_FREE(string_to.wstr_ptr);
                string_to.wstr_ptr = PNULL;
            }
        }
        break;
    
        case MMISMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                MMIAPIPB_AddContactWin(&pb_email, PB_GROUP_ALL, MMIPB_ADD_TYPE_EMAIL);
            }
        }
        break;
    #ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMISMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
        {
            num_len = strlen((char *)email_ptr);
    
            if (num_len > 0)
            {
                num_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                SCI_MEMSET(&pb_email, 0x0, sizeof(MMIPB_MAIL_T));
                MMI_STRNTOWSTR(pb_email.wstr, num_len, (const uint8 *)email_ptr, num_len, num_len);
                pb_email.wstr_len = num_len;
                // MMIAPIPB_OpenEntryListForEx(&pb_email,MMIPB_ADD_TYPE_EMAIL);
                MMIAPIPB_AddToExistContact(&pb_email, MMIPB_ADD_TYPE_EMAIL, PNULL);
            }
        }
        break;
    #endif
        default :
            break;
    }
    
    SCI_FREE(email_ptr);    
}

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL void HandleChatWinOpenInsertOpt(MMISMS_CHATREAD_WINDATA_T *win_data_ptr)
{
    MMIPUB_MENU_PARAM_T menuparam = {0};
    menuparam.group_id = MENU_SMS_CHATEDIT_INSERT_SELECT;
    menuparam.win_id =   MMISMS_SHOW_CHAT_INSERT_OPT_WIN_ID;
    menuparam.ctrl_id =  MMISMS_CHAT_INSERT_OPT_CTRL_ID;    
    menuparam.dst_handle = win_data_ptr->win_id;
    menuparam.func.OnMenuOk = HandleChatWinInsertOptOK;                    
    MMIPUB_OpenPubMenuWin( &menuparam ); 
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC MMISMS_CHAT_INSERT_TYPE MMISMS_GetChatEditInsertOpType(void)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return MMISMS_INSERT_INIT;
    }
    else
    {
        return win_data_ptr->sms_chat_insert_info.insert_type;
    }    
}

/*****************************************************************************/
//     Description : handle extract url
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
PUBLIC void MMISMS_SetChatEditInsertOpType(MMISMS_CHAT_INSERT_TYPE insert_type)
{
    MMISMS_CHATREAD_WINDATA_T *win_data_ptr = PNULL;
    win_data_ptr = (MMISMS_CHATREAD_WINDATA_T *)MMK_GetWinUserData(MMISMS_SHOW_CHAT_SMS_WIN_ID);
    if(PNULL == win_data_ptr)
    {
        return;
    }
    win_data_ptr->sms_chat_insert_info.insert_type = insert_type;
}
#endif

/*****************************************************************************/
//     Description : get chat win wstring number,such as :10086, email@163.com
//    Global resource dependence :
//  Author: rong.gu
//    Note:
/*****************************************************************************/
LOCAL uint16 MMISMS_GetChatWinWNumber(MMISMS_CHATREAD_WINDATA_T *win_data_ptr,wchar *wnumber)
{
    uint16 num_len = 0;
    uint8 number[MMISMS_PBNUM_MAX_LEN + 2] = {0};
    if(PNULL == win_data_ptr || PNULL == wnumber)
    {
        return 0;
    }
    if(!win_data_ptr->is_email_number)
    {
        MMIAPICOM_GenDispNumber(
                      MMIPB_GetNumberTypeFromUint8(win_data_ptr->chat_number.npi_ton),
                      MIN(MN_MAX_ADDR_BCD_LEN, win_data_ptr->chat_number.number_len),
                      win_data_ptr->chat_number.number,
                      number,
                      MMISMS_PBNUM_MAX_LEN + 2
                  );
        MMIAPICOM_StrToWstr(number, wnumber);
    }
    else
    {
        MMIAPICOM_StrToWstr(win_data_ptr->sender, wnumber);    
    }
    num_len = (uint16)MMIAPICOM_Wstrlen(wnumber);
    return num_len;                    
} 

#endif
