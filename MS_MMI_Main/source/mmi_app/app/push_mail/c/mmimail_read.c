/****************************************************************************
** File Name:      mmimail_read.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MMIMAIL_READ_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_app_push_mail_trc.h"
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmimail_internal.h"
#include "mmimail_wintab.h"
#include "mmimail_text.h"
#include "mmimail_image.h"
#include "mmimail_id.h"
#include "mmimail_menutable.h"
#include "mmimail_read.h"
#include "mail_attachment.h"
#include "mail_type.h"
#include "mail_util.h"
#include "mail_file.h"
#include "mail_fldr.h"
#include "guitab.h"
#include "guitext.h"
#include "guirichtext.h"
#include "guilistbox.h"
#include "gui_ucs2b_converter.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmifmm_image.h"
#include "mmifmm_export.h"
#include "mmipb_export.h"
//#include "mmipb_internal.h"
#include "mmidisplay_data.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmiidle_export.h"
#include "mmisms_id.h"
#include "mmisms_export.h"
//#include "mmiebook.h"
#include "mmiebook_export.h"
#include "mmiudisk_export.h"
#include "guictrl_api.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMAIL_READ_CONTENT_SEPERATOR      L"----------------------------------"

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//阅读邮件选项菜单
typedef enum
{
    MMIMAIL_NODE_READ_OPT_NULL,
    MMIMAIL_NODE_READ_OPT_REPLY,
    MMIMAIL_NODE_READ_OPT_REPLY_ALL,
    MMIMAIL_NODE_READ_OPT_FORWARD_ALL,
    MMIMAIL_NODE_READ_OPT_FORWARD_CONTENT,
    MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ACC,
    MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ALL,
    MMIMAIL_NODE_READ_OPT_ACCESSORY,
    MMIMAIL_NODE_READ_OPT_DEL,
    MMIMAIL_NODE_READ_OPT_SAVE_CONTACT,
    MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS,
    MMIMAIL_NODE_READ_OPT_EDIT,
    MMIMAIL_NODE_READ_OPT_SEND,
    MMIMAIL_NODE_READ_OPT_RESEND,
    MMIMAIL_NODE_READ_OPT_MAX
}MMIMAIL_NODE_READ_OPT_E;

//附件选项菜单
typedef enum
{
    MMIMAIL_NODE_ACCESSORY_OPT_NULL,
    MMIMAIL_NODE_ACCESSORY_OPT_DL_SAVE,
    MMIMAIL_NODE_ACCESSORY_OPT_MAX
}MMIMAIL_NODE_ACCESSORY_OPT_E;

//展开的附件列表信息
typedef struct
{
    mail_att_multi_file_list    *list_ptr;  //展开的附件列表
    uint16                      cur_index;  //当前查看的附件索引
    
}MMIMAIL_EXPAND_ACC_LIST_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/
//邮件浏览相关图标
LOCAL const MMI_IMAGE_ID_T s_mail_icon[MMIMAIL_ICON_MAX] =
{
    IMAGE_MAIL_EDIT_FROM,
    IMAGE_MAIL_EDIT_TO,
    IMAGE_MAIL_EDIT_CC,
    IMAGE_MAIL_EDIT_BCC,
    IMAGE_MAIL_EDIT_SUBJECT,
    IMAGE_MAIL_EDIT_CONTENT,
    IMAGE_MAIL_EDIT_ACCESSORY,
    IMAGE_MAIL_EDIT_TIME,
    IMAGE_MAIL_ACCOUNT
};

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL BOOLEAN   s_is_disp_all = FALSE; //是否显示所有联系人
LOCAL MAIL_MESSAGE_PART_T *s_cur_oper_accessory = PNULL; //当前正在操作的附件
LOCAL MMIMAIL_EXPAND_ACC_LIST_T s_cur_expand_acc_list_info = {0}; //当前展开的附件列表信息
LOCAL MAIL_ADDRESS_T    *s_cur_oper_addr = PNULL; //当前正在操作的邮件地址
LOCAL MAIL_ADDRESS_T    **s_cur_recver_list = PNULL; //当前收件人列表信息

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

extern int stricmp (const char *, const char *);

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 邮件阅读界面处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailReadWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
//	Description : 阅读邮件选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadMailOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//	Description : 处理阅读邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadMailOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_CTRL_ID_T      ctrl_id,   //IN:
                                 uint16             node_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 读取收件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadInboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 读取发件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadOutboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 );

/*****************************************************************************/
//	Description : 读取已发件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadSentboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 );

/*****************************************************************************/
// 	Description : 读取全部正文处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailReadAllContentWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            );

/*****************************************************************************/
//	Description : 等待阅读邮件窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReadWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );
                                            
/*****************************************************************************/
// 	Description : 附件为图片的预览窗口
//	Global resource dependence : 
//  Author: yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccPicurePreview(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//	Description : 附件列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccessoryListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
//	Description : 附件展开列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccExpandListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

/*****************************************************************************/
// 	Description : 附件列表界面选项菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/*****************************************************************************/
//	Description : 处理保存或下载附件的操作
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void HandleSaveOrDlAccessory(void);
                                             
/*****************************************************************************/
//	Description : 下载附件等待窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDlWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );
                                         
/*****************************************************************************/
//  Description : 显示邮件内容
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowMailContent(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                BOOLEAN                 is_disp_recver //是否显示收件人、抄送人和密送人
                                );
                                
/*****************************************************************************/
//  Description : 显示全部邮件正文
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowAllMailContent(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr
                                );

/*****************************************************************************/
//	Description : 设置阅读界面softkey
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void SetReadWinSoftkey(MMI_WIN_ID_T win_id, BOOLEAN is_disp_mid);

/*****************************************************************************/
//	Description : 打开阅读邮件选项窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  OpenMailReadOptWin(void);

/*****************************************************************************/
//	Description : 打开附件选项窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  OpenAccessoryOptWin(void);

/*****************************************************************************/
//	Description : 打开附件展开窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:经转换格式的附件列表
/*****************************************************************************/
LOCAL void  OpenAccessoryExpandWin(void);

/*****************************************************************************/
//	Description : 获取附件列表中间键文本
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetAccListItemMidSoftkey(MAIL_MESSAGE_PART_T *acc_ptr);

/*****************************************************************************/
//	Description : 释放当前查看mail占用的空间
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void FreeMailSpace(MAIL_MESSAGE_T   *whole_mail_ptr);

/*****************************************************************************/
//	Description : 切换到下一封邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void SwitchToNextMail(
                                    BOOLEAN is_next //TRUE:后一封  FALSE:前一封
                                );

/*****************************************************************************/
//  Description : 添加附件列表项
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void AppendAccessoryListItem(
                                            MMI_CTRL_ID_T           ctrl_id,
                                            MMI_TEXT_ID_T           left_softkey_id,
                                            MMI_TEXT_ID_T           mid_softkey_id,
                                            MMI_TEXT_ID_T           right_softkey_id,
                                            const wchar*            filename,
                                            uint16                  filename_len,
                                        	uint32                  filesize
                                          );

/*****************************************************************************/
//  Description : 更新附件列表项
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void ReplaceAccessoryListItem(
                                            MMI_CTRL_ID_T           ctrl_id,
                                            MMI_TEXT_ID_T           left_softkey_id,
                                            MMI_TEXT_ID_T           mid_softkey_id,
                                            MMI_TEXT_ID_T           right_softkey_id,
                                            const wchar*            filename,
                                            uint16                  filename_len,
                                        	uint32                  filesize,
                                        	uint16                  pos
                                          );
                               
/*****************************************************************************/
//	Description : 判断焦点是否在更多上
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOnReadMore(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//	Description : 判断焦点是否在网址上
//	Global resource dependence : none
//	Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAil_IsFocusOnURL(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//	Description : 打开网址
//	Global resource dependence : none
//	Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void MMIMAil_GoToFocusedURL(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//	Description : 更新邮件已读状态
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateMailReadState(uint32 record_id);

/*****************************************************************************/
//	Description : 获取当前正在操作的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MAIL_MESSAGE_PART_T * MMIMAIL_GetCurOperAcc(void);

/*****************************************************************************/
//	Description : 设置当前正在操作的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurOperAcc(MAIL_MESSAGE_PART_T * cur_acc);

/*****************************************************************************/
//	Description : 获取当前正在操作的展开列表中的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL mail_att_file_info * MMIMAIL_GetCurOperExpandAcc(void);

/*****************************************************************************/
//	Description : 设置当前正在操作的展开列表中的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurOperExpandAcc(uint16 cur_index);

/*****************************************************************************/
//	Description : 获取当前展开的附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL mail_att_multi_file_list * MMIMAIL_GetCurExpandAccList(void);

/*****************************************************************************/
//	Description : 设置当前展开的附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurExpandAccList(mail_att_multi_file_list * acc_list);

/*****************************************************************************/
//	Description : 判断当前显示邮件的附件是否已经下载
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAccessoryDownloaded(MAIL_MESSAGE_PART_T *accessory_ptr);

/*****************************************************************************/
//	Description : 判断当前显示邮件的附件是否进行了自动格式转换
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAccessoryAutoChanged(
                                            MAIL_MESSAGE_PART_T *accessory_ptr,
                                            uint16              *changed_num_ptr //转换之后的文件个数
                                            );

/*****************************************************************************/
//	Description : 打开附件列表窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenAccessoryListWin(void);

/*****************************************************************************/
//	Description : 加载附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccessoryList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              );

/*****************************************************************************/
//	Description : 加载附件展开列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccExpandList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              );

/*****************************************************************************/
//	Description : 打开所有收件人地址列表窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  MMIMAIL_OpenAllContactAddrListWin(void);

/*****************************************************************************/
//	Description : 加载所有联系人列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadRecverList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              );
                                              
/*****************************************************************************/
//	Description : 收件人列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAllRecverListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );
                                      
/*****************************************************************************/
//	Description : 收件人列表选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadRecvListOptWin(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
                                          
/*****************************************************************************/
//	Description : 选择分组界面处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadSelGroupWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : MailVideoPlayCallBack
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void MailVideoPlayCallBack(VP_CALLBACK_T *msg_ptr);
#endif /* VIDEO_PLAYER_SUPPORT */

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//邮件阅读界面
WINDOW_TABLE( MMIMAIL_READ_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailReadWinMsg),    
    WIN_SOFTKEY(STXT_OPTION, TXT_REPLY, STXT_RETURN),
    WIN_ID( MMIMAIL_READ_WIN_ID ),
	WIN_TITLE(TXT_MAIL_READ),
    CREATE_RICHTEXT_CTRL(MMIMAIL_READ_TEXT_CTRL_ID),
    END_WIN
};

//阅读收件箱邮件选项菜单
WINDOW_TABLE( MMIMAIL_READ_MAIL_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleReadMailOptWinMsg),
    WIN_ID(MMIMAIL_READ_MAIL_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//附件列表界面
WINDOW_TABLE( MMIMAIL_READ_ACCESSORY_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleReadAccessoryListWinMsg),    
    WIN_ID( MMIMAIL_READ_ACCESSORY_WIN_ID ),
    WIN_TITLE(TXT_MAIL_ACCESSORY_LIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION,TXT_NULL,STXT_RETURN),
    END_WIN
};

//附件展开列表界面
WINDOW_TABLE( MMIMAIL_READ_ACC_EXPAND_LIST_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleReadAccExpandListWinMsg),
    WIN_ID(MMIMAIL_READ_ACC_EXPAND_WIN_ID),
    WIN_TITLE(TXT_MAIL_ACCESSORY_LIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_SAVE_AS,TXT_NULL,STXT_RETURN),
    END_WIN
};

//查看图片附件窗口
WINDOW_TABLE( MMIMAIL_PIC_PREVIEW_WIN_TAB ) =
{
	WIN_TITLE(TXT_NULL),
    WIN_FUNC( (uint32)HandleAccPicurePreview), 
    WIN_ID( MMIMAIL_PICTURE_PREVIEW_WIN_ID ),
    CREATE_ANIM_CTRL(MMIMAIL_PICTURE_PREVIEW_CTRL_ID, MMIMAIL_PICTURE_PREVIEW_WIN_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};

//附件列表界面选项菜单
WINDOW_TABLE( MMIMAIL_READ_ACC_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleReadAccMenuOptWinMsg),
    WIN_ID(MMIMAIL_READ_ACC_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//联系人列表界面
WINDOW_TABLE( MMIMAIL_READ_ALL_RECVER_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleReadAllRecverListWinMsg),    
    WIN_ID( MMIMAIL_READ_ALL_RECVER_WIN_ID ),
    WIN_TITLE(TXT_COMMON_CONTACT),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_READ_ALL_RECVER_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION,TXT_NULL,STXT_RETURN),
    END_WIN
};

//联系人列表界面选项菜单
WINDOW_TABLE(MMIMAIL_READ_RECV_LIST_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleReadRecvListOptWin),    
    WIN_ID(MMIMAIL_READ_RECV_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_RECV_LIST_OPT,MMIMAIL_READ_RECV_LIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//保存到新联系人界面 选择分组窗口
WINDOW_TABLE( MMIMAIL_READ_SEL_GROUP_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleReadSelGroupWinMsg),    
    WIN_ID( MMIMAIL_READ_SEL_GROUP_WIN_ID ),
	WIN_TITLE(TXT_MAIL_SEL_GROUP),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN ),
    CREATE_MENU_CTRL(MENU_MAIL_SEL_GROUP, MMIMAIL_READ_SEL_GROUP_MENU_CTRL_ID),
    END_WIN
};

//阅读全部正文窗口
WINDOW_TABLE( MMIMAIL_READ_ALL_CONTENT_TAB ) = 
{
    WIN_FUNC((uint32) HandleMailReadAllContentWinMsg),    
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    WIN_ID( MMIMAIL_READ_ALL_CONTENT_WIN_ID ),
	WIN_TITLE(TXT_MAIL_CONTENT),
    CREATE_TEXT_CTRL(MMIMAIL_READ_ALL_CONTENT_TEXT_CTRL_ID),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : 读取指定邮件，开始解码过程
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_ReadMail(uint32 record_id)
{
    MMI_STRING_T    prompt_str = {0};

    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }
    
    if (!MMIMAIL_IsReady())
    {
        MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_INIT, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }
    
    //开启等待窗口，等待解码当前邮件
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
    MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMIMAIL_READ_WAITING_WIN_ID, IMAGE_NULL,
        ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleReadWaitingWinMsg);
        
    //调用接口通知mail线程开始解码
    mail_StartViewMail((int)record_id);
    return TRUE;
}

/*****************************************************************************/
//	Description : 打开邮件阅读窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenReadWin(uint32 record_id)
{   
    //保存当前正在浏览邮件的record id
    MMIMAIL_SetCurDispMailRecordId(record_id);

    //创建阅读窗口
    MMK_CreateWin((uint32 *)MMIMAIL_READ_WIN_TAB, PNULL);

    //标记未读邮件为已读
    UpdateMailReadState(record_id);

    return TRUE;
}

/*****************************************************************************/
//	Description : 邮件读取完毕处理
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_ReadFinished(MAIL_MESSAGE_T   *whole_mail_ptr)
{
    if (PNULL != whole_mail_ptr)
    {
        //先释放当前正在显示的邮件空间
        if (PNULL != MMIMAIL_GetCurDispMail())
        {
            MMIEMAIL_DestroyMessage(MMIMAIL_GetCurDispMail());
        }

        //保存当前正在显示的邮件
        MMIMAIL_SetCurDispMail(whole_mail_ptr);   

        //通知邮件列表窗口解码完成
        if (MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID))
        {
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: post MMIMAIL_MAILBOX_MAIN_WIN_ID ==> MSG_MAIL_FINISH_READ"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_691_112_2_18_2_48_17_329,(uint8*)"");
            MMK_PostMsg(MMIMAIL_MAILBOX_MAIN_WIN_ID, MSG_MAIL_FINISH_READ, 0, 0);
        }
        else
        {
            MAIL_ENTER_TYPE_E open_type = MMIMAIL_GetOpenType();
            
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: MMIMAIL_MAILBOX_MAIN_WIN_ID is not open"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_698_112_2_18_2_48_17_330,(uint8*)"");
            if (MMK_IsOpenWin(MMIMAIL_READ_WAITING_WIN_ID)) 
            {
                MMK_CloseWin(MMIMAIL_READ_WAITING_WIN_ID);
            }

            if ((MAIL_ENTER_READWIN == open_type)
                || (MAIL_ENTER_REPLYMAIL == open_type)
                || (MAIL_ENTER_FORWARDMAIL == open_type)
                || (MAIL_ENTER_EDITMAIL == open_type))
            {
                MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
                uint32 record_id = (uint32)MMIMAIL_GetOpenParam();
                
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: open_type : %d, record_id : %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_712_112_2_18_2_48_17_331,(uint8*)"dd", open_type, record_id);

                cur_mail_ptr = MMIMAIL_GetMailPtrByRecordId(record_id);

                if (PNULL == cur_mail_ptr)
                {
                    //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: get mail ptr fail!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_718_112_2_18_2_48_18_332,(uint8*)"");
                    MMIEMAIL_DestroyMessage(whole_mail_ptr);
                    MMIMAIL_SetCurDispMail(PNULL); 
                    MMIMAIL_OpenMsgBox(TXT_MAIL_VIEW_FAIL, IMAGE_PUBWIN_WARNING);
                    return;
                }
                
                MMIMAIL_SetCurOperationMail(cur_mail_ptr);

                if (MAIL_ENTER_READWIN == open_type)
                {
                    MMIAPIMAIL_OpenReadWin(record_id);
                }
                else if (MAIL_ENTER_REPLYMAIL == open_type)
                {
                    MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_REPLY, whole_mail_ptr);
                }
                else if (MAIL_ENTER_FORWARDMAIL == open_type)
                {
                    MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_FORWARD_ALL, whole_mail_ptr);
                }
                else if (MAIL_ENTER_EDITMAIL == open_type)
                {
                    MMIAPIMAIL_OpenEditWin(MMIMAIL_OPEN_TYPE_ORIGINAL, whole_mail_ptr);
                }
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: mail open type is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_746_112_2_18_2_48_18_333,(uint8*)"d", open_type);
            }
        }
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_ReadFinished: whole_mail_ptr is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_752_112_2_18_2_48_18_334,(uint8*)"");
        if (MMK_IsOpenWin(MMIMAIL_READ_WAITING_WIN_ID)) 
        {
            MMK_CloseWin(MMIMAIL_READ_WAITING_WIN_ID);
        }
        MMIMAIL_OpenMsgBox(TXT_MAIL_VIEW_FAIL, IMAGE_PUBWIN_WARNING);
    }
    return;
}

/*****************************************************************************/
//	Description : 获取邮件编辑界面或浏览界面的显示图标
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIMAIL_GetIconByType(MMIMAIL_ICON_ENUM_E type)
{
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;

    if (type < MMIMAIL_ICON_MAX)
    {
        image_id = s_mail_icon[type];
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetIconByType: type [%d] is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_776_112_2_18_2_48_18_335,(uint8*)"d", type);
    }

    return image_id;
}

/*****************************************************************************/
//	Description : 邮件中是否有接收邮件地址
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIL_IsHaveDestAddr(MAIL_MESSAGE_T   *whole_mail_ptr)
{
    BOOLEAN is_have = FALSE;

    if ((PNULL != whole_mail_ptr)
        &&((whole_mail_ptr->to_count > 0)
        || (whole_mail_ptr->cc_count > 0)
        || (whole_mail_ptr->bcc_count > 0)))
    {
        is_have = TRUE;
    }

    return is_have;
}

/*****************************************************************************/
// 	Description : 邮件阅读界面处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailReadWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
	MMI_RESULT_E    result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_TEXT_CTRL_ID;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type = MMIMAIL_OPEN_TYPE_DEFAULT;

    //COMM_TRACE:"MMIMAIL==> HandleMailReadWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_816_112_2_18_2_48_18_336,(uint8*)"d", msg_id);

	switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        if (MMIMAIL_INBOX == MMIMAIL_GetCurBoxType())
        {
            s_is_disp_all = FALSE; //收件箱邮件默认不显示收件人
        }
        else
        {
            s_is_disp_all = TRUE; //发件箱、草稿箱、已发件箱邮件默认显示收件人
        }
        ShowMailContent(ctrl_id, whole_mail_ptr, s_is_disp_all);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
        GUIWIN_SetTitleRollArrow(win_id, TRUE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        SetReadWinSoftkey(win_id, TRUE);
        break;

    case MSG_GET_FOCUS:
        ShowMailContent(ctrl_id, whole_mail_ptr, s_is_disp_all);
        SetReadWinSoftkey(win_id, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if (IsFocusOnReadMore(ctrl_id))
        {
            MMK_CreateWin((uint32 *)MMIMAIL_READ_ALL_CONTENT_TAB,PNULL);
        }
        else
        {
            //打开选项菜单
            OpenMailReadOptWin();        
        }
        break;

    case MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_KEY:
        if (IsFocusOnReadMore(ctrl_id))
        {
            //焦点在 更多 上，不显示中间键
            SetReadWinSoftkey(win_id, FALSE);
        }
        else if (MMIMAil_IsFocusOnURL(ctrl_id))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_BROWSE, STXT_RETURN, TRUE);
        }
        else
        {
            SetReadWinSoftkey(win_id, TRUE);
        }
        break;
    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (IsFocusOnReadMore(ctrl_id))
        {
            MMK_CreateWin((uint32 *)MMIMAIL_READ_ALL_CONTENT_TAB,PNULL);
        }
        else if (MMIMAil_IsFocusOnURL(ctrl_id))
        {
            MMIMAil_GoToFocusedURL(ctrl_id);
        }
        else
        {
            switch (MMIMAIL_GetCurBoxType())
            {
            case MMIMAIL_INBOX:
                open_type = MMIMAIL_OPEN_TYPE_REPLY;
                break;

            case MMIMAIL_OUTBOX:
            case MMIMAIL_DRAFTBOX:
                open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
                break;
                
            case MMIMAIL_SENTBOX:
                open_type = MMIMAIL_OPEN_TYPE_FORWARD_ALL;
                break;
                
            default:
                open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
                break;
            }
            MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
            //MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_TITLE_MOVE_PREV:
    case MSG_APP_LEFT:
        if (MAIL_ENTER_READWIN == MMIMAIL_GetOpenType())
        {
            if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
            {
                MMIAPISMS_HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            SwitchToNextMail(FALSE);
        }
        break;

    case MSG_CTL_TITLE_MOVE_NEXT:
    case MSG_APP_RIGHT:
        if (MAIL_ENTER_READWIN == MMIMAIL_GetOpenType())
        {
            if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
            {
                MMIAPISMS_HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            SwitchToNextMail(TRUE);
        }
        break;
        
    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        if (MAIL_ENTER_READWIN == MMIMAIL_GetOpenType())
        {
            if (MMK_IsOpenWin(MMISMS_SHOWMSG_WIN_ID))
            {
                MMK_PostMsg(MMISMS_SHOWMSG_WIN_ID, msg_id, PNULL, 0);
            }
        }
    
        //FreeMailSpace(whole_mail_ptr);
        MMK_CloseWin(win_id);
    	break;

    case MSG_APP_RED:
        //FreeMailSpace(whole_mail_ptr);
        MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
    	break;

    case MSG_CLOSE_WINDOW:
        FreeMailSpace(whole_mail_ptr);
        if (MAIL_ENTER_READWIN == MMIMAIL_GetOpenType())
        {
            MMIAPIMAIL_Exit();
        }
        break;
        
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
    
	return (result);
}

/*****************************************************************************/
//	Description : 阅读邮件选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadMailOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_READ_OPT_MENU_CTRL_ID;
    uint32 node_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleReadMailOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_989_112_2_18_2_48_18_337,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        if (!HandleReadMailOptMenuId(win_id, ctrl_id, node_id))
        {
            //COMM_TRACE:"MMIMAIL==> HandleReadMailOptWinMsg: node_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1005_112_2_18_2_48_18_338,(uint8*)"d", node_id);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
   
    return recode;
}

/*****************************************************************************/
//	Description : 处理阅读邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadMailOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 MMI_CTRL_ID_T      ctrl_id,   //IN:
                                 uint16             node_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();
       
    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadMailOptMenuId PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1037_112_2_18_2_48_18_339,(uint8*)"");
        return FALSE;
    }
    
    switch (node_id)
    {
    case MMIMAIL_NODE_READ_OPT_DEL:
        MMIMAIL_OpenDelteQueryWin();        
        MMK_CloseWin( win_id );
        break;
        
    case MMIMAIL_NODE_READ_OPT_SAVE_CONTACT:
        MMIMAIL_OpenAllContactAddrListWin();
        MMK_CloseWin( win_id );
        break;
        
    case MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS:
        s_is_disp_all = s_is_disp_all ? FALSE : TRUE;
        MMK_CloseWin( win_id );
        break;

    case MMIMAIL_NODE_READ_OPT_REPLY:
    case MMIMAIL_NODE_READ_OPT_REPLY_ALL:
    case MMIMAIL_NODE_READ_OPT_FORWARD_CONTENT:
    case MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ACC:
    case MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ALL:
    case MMIMAIL_NODE_READ_OPT_ACCESSORY:
        result = HandleReadInboxOptMenuId(win_id, node_id);
        break;

    case MMIMAIL_NODE_READ_OPT_EDIT:
    case MMIMAIL_NODE_READ_OPT_SEND:
        result = HandleReadOutboxOptMenuId(win_id, node_id);
        break;
        
    case MMIMAIL_NODE_READ_OPT_FORWARD_ALL:
    case MMIMAIL_NODE_READ_OPT_RESEND:
        result = HandleReadSentboxOptMenuId(win_id, node_id);
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 读取收件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadInboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
    
    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadInboxOptMenuId PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1100_112_2_18_2_48_18_340,(uint8*)"");
        return FALSE;
    }

    switch (node_id)
    {
    case MMIMAIL_NODE_READ_OPT_REPLY:
    case MMIMAIL_NODE_READ_OPT_REPLY_ALL:
    case MMIMAIL_NODE_READ_OPT_FORWARD_CONTENT:
    case MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ACC:
    case MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ALL:
        if (MMIMAIL_NODE_READ_OPT_REPLY == node_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_REPLY;
        }
        else if (MMIMAIL_NODE_READ_OPT_REPLY_ALL == node_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_REPLY_ALL;
        }
        else if (MMIMAIL_NODE_READ_OPT_FORWARD_CONTENT == node_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_FORWARD_CONTENT;
        }
        else if (MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ACC == node_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC;
        }
        else if (MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ALL == node_id)
        {
            open_type = MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL;
        }
        MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_NODE_READ_OPT_ACCESSORY:
        MMIMAIL_OpenAccessoryListWin();
        MMK_CloseWin(win_id);
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 读取发件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:草稿箱操作同发件箱
/*****************************************************************************/
LOCAL BOOLEAN HandleReadOutboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
       
    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadOutboxOptMenuId PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1163_112_2_18_2_48_18_341,(uint8*)"");
        return FALSE;
    }
    
    switch (node_id)
    {       
    case MMIMAIL_NODE_READ_OPT_SEND:
        if (MMIMAIL_IsHaveDestAddr(whole_mail_ptr))
        {
            MMIMAIL_MAIL_SUMMERY_T *cur_mail = MMIMAIL_GetCurOperationMail();
            if (PNULL != cur_mail)
            {
                MMIAPIMAIL_SendMailById(cur_mail->record_id);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> HandleReadOutboxOptMenuId: current operation mail is NULL!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1179_112_2_18_2_48_18_342,(uint8*)"");
            }
        }
        else
        {
            //打开邮件编辑界面
            open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
            MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        }
        MMK_CloseWin(win_id);
        break;
        
    case MMIMAIL_NODE_READ_OPT_EDIT:
        open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
        MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//	Description : 读取已发件箱邮件选项菜单
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleReadSentboxOptMenuId(
                                 MMI_WIN_ID_T       win_id,    //IN:
                                 uint16             node_id    //IN:
                                 )
{
    BOOLEAN result = TRUE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type = MMIMAIL_OPEN_TYPE_DEFAULT;
       
    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadSentboxOptMenuId PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1220_112_2_18_2_48_19_343,(uint8*)"");
        return FALSE;
    }

    switch (node_id)
    {
    case MMIMAIL_NODE_READ_OPT_FORWARD_ALL:
        open_type = MMIMAIL_OPEN_TYPE_FORWARD_ALL;
        MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        MMK_CloseWin(win_id);
        break;

    case MMIMAIL_NODE_READ_OPT_RESEND:
        open_type = MMIMAIL_OPEN_TYPE_ORIGINAL;
        MMIAPIMAIL_OpenEditWin(open_type, whole_mail_ptr);
        MMK_CloseWin(win_id);
        break;

    default:
        result = FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : 读取全部正文处理函数
//	Global resource dependence : 
//  Author:yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailReadAllContentWinMsg( 
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_ALL_CONTENT_TEXT_CTRL_ID;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();

    //COMM_TRACE:"MMIMAIL==> HandleMailReadAllContentWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1260_112_2_18_2_48_19_344,(uint8*)"d", msg_id);

    switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        ShowAllMailContent(ctrl_id, whole_mail_ptr);
//        GUITEXT_SetScrollMode(ctrl_id, GUITEXT_SCROLL_PAGE);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
    	break;
        
	default:
    	result = MMI_RESULT_FALSE;
    	break;
    }
	return (result);
}

/*****************************************************************************/
//	Description : 等待阅读邮件窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleReadWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleReadWaitingWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1296_112_2_18_2_48_19_345,(uint8*)"d", msg_id);

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

/*****************************************************************************/
// 	Description : 附件为图片的预览窗口
//	Global resource dependence : 
//  Author: yuanl
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAccPicurePreview(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    MMI_RESULT_E  result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIMAIL_PICTURE_PREVIEW_CTRL_ID;
    wchar         *full_path_name_ptr = (wchar *)MMK_GetWinAddDataPtr(win_id);
    uint16        full_path_len = 0;

    //COMM_TRACE:"MMIMAIL==> HandleAccPicurePreview, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1334_112_2_18_2_48_19_346,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T both_rect = {0};

            both_rect.h_rect.left = MMIMAIL_FULLSCREEN_CLIENT_LEFT;
            both_rect.h_rect.top = MMIMAIL_FULLSCREEN_CLIENT_TOP;
            both_rect.h_rect.bottom = MMIMAIL_FULLSCREEN_CLIENT_BOTTOM;
            both_rect.h_rect.right = MMIMAIL_FULLSCREEN_CLIENT_RIGHT;
            both_rect.v_rect.left = MMIMAIL_FULLSCREEN_CLIENT_LEFT;
            both_rect.v_rect.top = MMIMAIL_FULLSCREEN_CLIENT_TOP;
            both_rect.v_rect.bottom = MMIMAIL_FULLSCREEN_CLIENT_BOTTOM;
            both_rect.v_rect.right = MMIMAIL_FULLSCREEN_CLIENT_RIGHT;

            GUIAPICTRL_SetBothRect(MMIMAIL_PICTURE_PREVIEW_CTRL_ID, &both_rect);
            if(full_path_name_ptr != PNULL)
            {
                full_path_len = MMIAPICOM_Wstrlen(full_path_name_ptr);
            }
            
            GUIWIN_SetTitleText(win_id, full_path_name_ptr, full_path_len, FALSE);

            result = MMIAPIFMM_SetPreviewPicParam(FALSE,full_path_name_ptr,full_path_len,ctrl_id);

            MMK_SetAtvCtrl(win_id,ctrl_id);
        }

        break;

    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
    	break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/*****************************************************************************/
//	Description : 打开图片附件查看窗口
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void OpenPicturePreviewWin(wchar *full_path_name_ptr)
{
     wchar  *filename_ptr = PNULL;

     filename_ptr = SCI_ALLOCA(MMIFILE_FILE_NAME_MAX_LEN * sizeof(wchar));
     if(filename_ptr != PNULL)
     {
         MMIAPICOM_Wstrcpy(filename_ptr, full_path_name_ptr);
         MMK_CreateWin((uint32*)MMIMAIL_PIC_PREVIEW_WIN_TAB, (ADD_DATA)filename_ptr);
     }
}

#ifdef VIDEO_PLAYER_SUPPORT
/*****************************************************************************/
// 	Description : MailVideoPlayCallBack
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL void MailVideoPlayCallBack(VP_CALLBACK_T *msg_ptr)
{
    return;
}
#endif /* VIDEO_PLAYER_SUPPORT */

/*****************************************************************************/
//	Description : 附件查看处理函数
//	Global resource dependence : none
//	Author:lixu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleAccessoryOpen(
                                        wchar *full_path_name,  //in 全路径
                                        uint32 file_name_len    //in 全路径长度
                                        )
{
    comm_uint16 *file_ext = NULL;
    char file_suffix[MAIL_MAX_FILE_PATH_LENGTH + 1] = {0};
    MMIFILE_FILE_INFO_T file_info = {0};

    //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen enter\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1417_112_2_18_2_48_19_347,(uint8*)"");
    
    if ((PNULL == full_path_name) || (0 == file_name_len))
    {
        return FALSE;
    }
    //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen full_path_name = %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1423_112_2_18_2_48_19_348,(uint8*)"s",full_path_name);

    MMIAPIFMM_GetFileInfoFormFullPath(full_path_name,
                                        file_name_len, &file_info);

    file_ext = mail_Wstrrchr(full_path_name, file_name_len, '.');

    if(NULL == file_ext)
    {
        return FALSE;
    }
    file_ext++;

    MMIAPICOM_WstrToStr(file_ext, (uint8 *)file_suffix);
    //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen file_suffix = %s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1437_112_2_18_2_48_19_349,(uint8*)"s", file_suffix);
    
    if((0 == stricmp(file_suffix,"mid"))||
       (0 == stricmp(file_suffix,"mp3"))||
        (0 == stricmp(file_suffix,"wma"))||
        (0 == stricmp(file_suffix,"wav"))||
        (0 == stricmp(file_suffix,"amr"))||
        (0 == stricmp(file_suffix,"acc")))
    {
#ifdef  MUSIC_PLAYER_SUPPORT
        //音频附件播放
        MMIAPIMP3_PlayMyDocumentMusic(&file_info);
#endif
    }
    #ifdef VIDEO_PLAYER_SUPPORT
    else if((0 == stricmp(file_suffix,"mp4"))||

#ifdef AVI_DEC_SUPPORT
        (0 == stricmp(file_suffix,"avi"))||
#endif
        (0 == stricmp(file_suffix,"3gp")))
    {
        //视频附件浏览

        VP_PLAY_PARAM_T play_param  = {0};
/*        GUI_RECT_T full_rect = {0};


        if(0 == stricmp(file_suffix,"mp4"))
        {
            type = MMICOM_VIDEO_TYPE_MP4;
        }
        if(0 == stricmp(file_suffix,"3gp"))
        {
            type = MMICOM_VIDEO_TYPE_3GP;
        }
#ifdef AVI_DEC_SUPPORT
        if(0 == stricmp(file_suffix,"avi"))
        {
            type = MMICOM_VIDEO_TYPE_AVI;
        }
#endif
*/
        play_param.dual_sys = MN_DUAL_SYS_1;
        play_param.is_random_play = FALSE;
        play_param.mode = VP_REPEAT_PLAY_MODE_CLOSE;
        play_param.url_ptr = PNULL;
        play_param.display_param.audioSupport = 1;
        play_param.display_param.videoSupport = 1;
        play_param.is_scal_up = TRUE;
        play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
/*
    full_rect = MMITHEME_GetFullScreenRect();

    if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_90;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
    {
        play_param.display_param.RotateAngle = LCD_ANGLE_270;
        play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
    }
    else
    {
        play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
        play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
    }
    
    play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
    play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
    play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
    play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
    play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
*/
        MMIAPIVP_Play(&play_param, MailVideoPlayCallBack, 0);
    }
#endif
    else if((0 == stricmp(file_suffix,"jpg"))||
        (0 == stricmp(file_suffix,"jpeg"))||
        (0 == stricmp(file_suffix,"png"))||
        (0 == stricmp(file_suffix,"gif"))||
        (0 == stricmp(file_suffix,"bmp")))
    {
        //图片附件浏览
        //MMIAPIFMM_PreviewPicture(full_path_name);
        OpenPicturePreviewWin(full_path_name);
    }
#ifdef EBOOK_SUPPORT
    else if((0 == stricmp(file_suffix,"txt"))||
            (0 == stricmp(file_suffix,"lrc")))
    {
        //文档附件浏览
        comm_int32 file_size = 0;
        char* ptr = NULL;
        comm_uint32 wfile_name_len = 0;
        char file_name[MAIL_MAX_FILE_PATH_LENGTH + 1] = {0};
        wchar wfile_name[MAIL_MAX_FILE_PATH_LENGTH + 1] = {0};

        MMIAPICOM_WstrToStr(full_path_name, (uint8 *)file_name);
        file_size = mail_GetFileSizeByPath(file_name);
        if(0 >= file_size)
        {
            MMIMAIL_OpenMsgBox(TXT_EMPTY_FILE, IMAGE_PUBWIN_WARNING);
            return TRUE;
        }
        ptr = strrchr(file_name,'\\');

        //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen ptr=%x\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1544_112_2_18_2_48_19_350,(uint8*)"d", ptr);
        if (NULL != ptr)
        {
            ptr++;

            MMIAPICOM_StrToWstr((uint8 *)ptr,wfile_name);
            wfile_name_len = MMIAPICOM_Wstrlen(wfile_name);
            MMIAPIEBOOK_ManagerWinMsg_4Fmm(MMI_DEVICE_UDISK,wfile_name,wfile_name_len, 
                                        file_size, full_path_name,file_name_len);
        }
    }
#endif
#ifdef BROWSER_SUPPORT
    else if((0 == stricmp(file_suffix,"htm"))||
        (0 == stricmp(file_suffix,"xml"))||
        (0 == stricmp(file_suffix,"html")))
    {
        //网页附件浏览
        comm_uint16 header_len = 0;
        comm_uint16 str_len = 0;
        comm_uint8* url_ptr = NULL;
        char *file_header = "file:///";
        MMIBROWSER_ENTRY_PARAM entry_para = {0};

        header_len = strlen(file_header);
        str_len = (uint16)(MMIAPICOM_Wstrlen(full_path_name) * 3 + header_len + 1);
        url_ptr = SCI_ALLOCA(str_len * sizeof(uint8));
        if (PNULL == url_ptr)
        {
            return FALSE;
        }

        SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
        SCI_MEMCPY(url_ptr, file_header, header_len);
        str_len -= header_len;
        GUI_WstrToUTF8(url_ptr + header_len, str_len, full_path_name, MMIAPICOM_Wstrlen(full_path_name));

        entry_para.url_ptr = (char*)url_ptr;
        entry_para.type = MMIBROWSER_ACCESS_URL;
        entry_para.dual_sys = MN_DUAL_SYS_MAX;
        entry_para.user_agent_ptr = NULL;

        //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen entry_para.url_ptr=%s\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1586_112_2_18_2_48_19_351,(uint8*)"s",entry_para.url_ptr);
        MMIAPIBROWSER_Entry(&entry_para);
        SCI_FREE(url_ptr);
    }
#endif
#if defined MMI_VCARD_SUPPORT
    else if(0 == stricmp(file_suffix,"vcf"))
    {
        //vcf附件浏览
        comm_int32 file_size = 0;
        char file_name[MAIL_MAX_FILE_PATH_LENGTH + 1] = {0};

        MMIAPICOM_WstrToStr(full_path_name, (uint8 *)file_name);
        file_size = mail_GetFileSizeByPath(file_name);
        if(0 >= file_size)
        {
            return FALSE;
        }

        MMIAPIPB_OpenVcardFileContactWin(full_path_name);
        
        //MMIPB_OpenVCardFile(full_path_name);

    }  
#endif
    else
    {
        //其他类型不支持
        //COMM_TRACE:"MMIMAIL==> HandleAccessoryOpen file type not support!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1614_112_2_18_2_48_19_352,(uint8*)"");
        MMIMAIL_OpenMsgBox(TXT_COMMON_NO_SUPPORT, IMAGE_PUBWIN_WARNING);
    }
   
    return TRUE;
}

/*****************************************************************************/
//	Description : 附件列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccessoryListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID;
    uint16          list_item_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();    

    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadAccessoryListWinMsg PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1639_112_2_18_2_48_19_353,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    //COMM_TRACE:"MMIMAIL==> HandleReadAccessoryListWinMsg msg_id = 0x%x, have_items = %d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1643_112_2_18_2_48_19_354,(uint8*)"dd",msg_id, have_items);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        list_item_num = MAX(whole_mail_ptr->partcount, 1);
        GUILIST_SetMaxItem(ctrl_id, list_item_num, FALSE);

        //加载附件列表项
        have_items = MMIMAIL_LoadAccessoryList(ctrl_id, whole_mail_ptr);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        MMIMAIL_SetCurOperAcc(PNULL);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if ( have_items )
        {
            uint16 selected_index = 0;
            
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);

            //保存当前正在操作的附件
            MMIMAIL_SetCurOperAcc(whole_mail_ptr->mailmessageparts[selected_index]);

            //打开选项菜单
            OpenAccessoryOptWin();
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if ( have_items )
        {
            uint16 selected_index = 0;
            MAIL_MESSAGE_PART_T *acc_ptr = PNULL;
            
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);

            //保存当前正在操作的附件
            acc_ptr = whole_mail_ptr->mailmessageparts[selected_index];
            MMIMAIL_SetCurOperAcc(acc_ptr);

            if (!MMIMAIL_IsAccessoryDownloaded(acc_ptr))
            {
                //附件未下载,打开选项菜单
                OpenAccessoryOptWin();
            }
            else if (MMIMAIL_IsAccessoryAutoChanged(acc_ptr, PNULL))
            {
                //附件已下载，但是已进行格式转换，直接展开附件
                OpenAccessoryExpandWin();
            }
            else
            {
                wchar *full_path_name = PNULL;
                uint32 file_name_len = 0;
    
                full_path_name = mail_AttachFileName((char *)whole_mail_ptr->uidl, 
                                                     (char *)acc_ptr->contentid,&(acc_ptr->filename));
                if (NULL != full_path_name)
                {
                    //附件已下载，且为原始格式，进入查看附件
                    file_name_len = MMIAPICOM_Wstrlen(full_path_name);
                    if (!HandleAccessoryOpen(full_path_name, file_name_len))
                    {
                        MMIMAIL_OpenMsgBox(TXT_MAIL_VIEW_FAIL, IMAGE_PUBWIN_WARNING);
                    }
                    comm_free(full_path_name);/*lint !e774*/
                }
                else
                {
                    //COMM_TRACE:"MMIMAIL==> HandleReadAccessoryListWinMsg: full_path_name is NULL"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1728_112_2_18_2_48_19_355,(uint8*)"");
                    MMIMAIL_OpenMsgBox(TXT_MAIL_VIEW_FAIL, IMAGE_PUBWIN_WARNING);
                }
            }
        }
        break;

    case MSG_GET_FOCUS:
        if (have_items)
        {
            MAIL_MESSAGE_PART_T *acc_ptr = MMIMAIL_GetCurOperAcc();
            MMI_TEXT_ID_T   mid_text = TXT_NULL;
            uint16 selected_index = GUILIST_GetCurItemIndex(ctrl_id);
                       
            if (PNULL != acc_ptr)
            {
                //更新中间键显示    
                mid_text = GetAccListItemMidSoftkey(acc_ptr);
                ReplaceAccessoryListItem(ctrl_id,
                                        STXT_OPTION,
                                        mid_text,
                                        STXT_RETURN,
                                        acc_ptr->filename.wstr_ptr,
                                        acc_ptr->filename.wstr_len,
                                        acc_ptr->size,
                                        selected_index
                                        );
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPIFMM_FreeAccTmpSpace();
        MMIMAIL_SetCurOperAcc(PNULL);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 附件展开列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccExpandListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_ACCESSORY_LISTBOX_CTRL_ID;
    uint16          list_item_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();    

    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadAccExpandListWinMsg PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1795_112_2_18_2_48_20_356,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    //COMM_TRACE:"MMIMAIL==> HandleReadAccExpandListWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1799_112_2_18_2_48_20_357,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        MMIMAIL_IsAccessoryAutoChanged(MMIMAIL_GetCurOperAcc(), &list_item_num);
        list_item_num = MAX(list_item_num, 1);
        GUILIST_SetMaxItem(ctrl_id, list_item_num, FALSE);

        //加载附件列表项
        have_items = MMIMAIL_LoadAccExpandList(ctrl_id, whole_mail_ptr);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
        if ( have_items )
        {
            uint16  selected_index = 0;
            mail_att_file_info *cur_acc_ptr = PNULL;
            
            //保存当前正在操作的附件
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            MMIMAIL_SetCurOperExpandAcc(selected_index);
            cur_acc_ptr = MMIMAIL_GetCurOperExpandAcc();
            
            if (PNULL == cur_acc_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1839_112_2_18_2_48_20_358,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
    
            //打开文件管理器
            MMIAPIFMM_CopyAccToFolder(cur_acc_ptr->file_name.wstr_ptr, cur_acc_ptr->file_name.wstr_len);
        }
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if ( have_items )
        {
            uint16  selected_index = 0;
            mail_att_file_info *cur_acc_ptr = PNULL;
            
            //保存当前正在操作的附件
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);
            MMIMAIL_SetCurOperExpandAcc(selected_index);
            cur_acc_ptr = MMIMAIL_GetCurOperExpandAcc();
            
            if (PNULL == cur_acc_ptr)
            {
                //SCI_TRACE_LOW:"MMIMail HandleReadAccExpandListWinMsg PNULL == cur_acc_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1862_112_2_18_2_48_20_359,(uint8*)"");
                return MMI_RESULT_FALSE;
            }

            //查看文件
            if (!HandleAccessoryOpen(cur_acc_ptr->file_name.wstr_ptr, cur_acc_ptr->file_name.wstr_len))
            {
                MMIMAIL_OpenMsgBox(TXT_MAIL_VIEW_FAIL, IMAGE_PUBWIN_WARNING);
            }
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:  
        //释放附件展开列表申请的空间
        mail_freeMultiFileList(MMIMAIL_GetCurExpandAccList());
        //释放为拷贝附件申请的空间
        MMIAPIFMM_FreeAccTmpSpace();
        //恢复全局变量
        SCI_MEMSET(&s_cur_expand_acc_list_info, 0, sizeof(MMIMAIL_EXPAND_ACC_LIST_T));
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
// 	Description : 附件列表界面选项菜单处理函数
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAccMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_ACC_OPT_MENU_CTRL_ID;
    uint32          node_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleReadAccMenuOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1910_112_2_18_2_48_20_360,(uint8*)"d", msg_id);

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
        {
        case MMIMAIL_NODE_ACCESSORY_OPT_DL_SAVE:
            HandleSaveOrDlAccessory();
            break;
        
        default:
            //SCI_TRACE_LOW:"MMIMail HandleReadAccMenuOptWinMsg node_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1931_112_2_18_2_48_20_361,(uint8*)"d",node_id);
            return MMI_RESULT_FALSE;
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
    return recode;
}

/*****************************************************************************/
//	Description : 处理保存或下载附件的操作
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void HandleSaveOrDlAccessory(void)
{
    MMI_STRING_T    waiting_text = {0};
    MAIL_MESSAGE_T  *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    MAIL_MESSAGE_PART_T *acc_ptr = MMIMAIL_GetCurOperAcc();
    uint16          account_id = MMIMAIL_GetDefAccountId();
    wchar           *full_path_name = PNULL;
    uint16          full_path_len = 0;
    
    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleSaveOrDlAccessory PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1964_112_2_18_2_48_20_362,(uint8*)"");
        return ;
    }
    
    if (PNULL == acc_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleSaveOrDlAccessory PNULL == acc_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_1970_112_2_18_2_48_20_363,(uint8*)"");
        return ;
    }
    
    if (!MMIMAIL_IsAccessoryDownloaded(acc_ptr))
    {
        //弹出等待下载提示窗口
        MMI_GetLabelTextByLang(TXT_COMMON_DOWNLOADING_WAIT, &waiting_text);
        
        MMIPUB_OpenWaitWin(1, &waiting_text, PNULL, PNULL, MMIMAIL_DL_ACCESSORY_WAITING_WIN_ID, IMAGE_NULL,
            ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_ONE, HandleDlWaitingWinMsg);

        //附件不存在，则先下载附件
        mail_RetrAttachStart(account_id, 
                             (char *)whole_mail_ptr->uidl,
                             acc_ptr,
                             1
                             );
    }
    else if (MMIMAIL_IsAccessoryAutoChanged(acc_ptr, PNULL))
    {
        //附件已下载，但是已进行格式转换，直接展开附件
        OpenAccessoryExpandWin();
    }
    else
    {
        //打开本地目录
        full_path_name = mail_AttachFileName((char *)whole_mail_ptr->uidl, (char *)acc_ptr->contentid, &(acc_ptr->filename));
        if (PNULL != full_path_name)
        {
            full_path_len = MMIAPICOM_Wstrlen((const wchar *)full_path_name);
            MMIAPIFMM_CopyAccToFolder(full_path_name, full_path_len);
            comm_free(full_path_name);/*lint !e774*/
        }
    }
}

/*****************************************************************************/
//	Description : 下载附件等待窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDlWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    //COMM_TRACE:"MMIMAIL==> HandleDlWaitingWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2019_112_2_18_2_48_20_364,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);
        break;
               
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //通知mail线程取消下载
        
        MMK_CloseWin(win_id);
        break;

    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//	Description : 添加发件人
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailSender(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                             )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint32              string_len = 0;
    wchar               wstr_addr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailSender PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2063_112_2_18_2_48_20_365,(uint8*)"");
        return;
    }
    //发件人
    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id = MMIMAIL_GetIconByType(MMIMAIL_ICON_FROM);

    if ((PNULL != whole_mail_ptr->from[0])
        && (PNULL != whole_mail_ptr->from[0]->email) 
        && ((string_len = strlen((char *)whole_mail_ptr->from[0]->email)) > 0))
    {
        //判断是显示名字还是邮件地址
        if (whole_mail_ptr->from[0]->name.wstr_len > 0)
        {
            //显示名字
            item_data.text_data.buf.str_ptr = whole_mail_ptr->from[0]->name.wstr_ptr;
            item_data.text_data.buf.len    = whole_mail_ptr->from[0]->name.wstr_len;
            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            GUIRICHTEXT_AddItem (
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
        else
        {
            //显示邮件地址
            string_len = MIN(string_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);  
            MMIAPICOM_StrToWstr((const uint8 *)whole_mail_ptr->from[0]->email, wstr_addr);
            item_data.text_data.buf.str_ptr = wstr_addr;
            item_data.text_data.buf.len    = string_len;
            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            GUIRICHTEXT_AddItem (
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
    }
    else
    {
        //没有发件人时，只显示图标
        //COMM_TRACE:"MMIMAIL==> AddMailSender: sender is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2107_112_2_18_2_48_20_366,(uint8*)"");
        
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_NONE;
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
}

/*****************************************************************************/
//	Description : 添加接收帐号
//	Global resource dependence : none
//	Author:yuanl
//	Note:邮箱显示的都为默认帐号的邮件，所以当前查看邮件的帐号为默认帐号的地址
/*****************************************************************************/
LOCAL void AddMailAccount(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                             )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint16              account_num = 0;
    
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailAccount PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2134_112_2_18_2_48_20_367,(uint8*)"");
        return;
    }

    account_num = mail_GetAccountNum();

    if (account_num > 1) //多帐户则显示接收帐号
    {
        uint16  acc_id = MMIMAIL_GetDefAccountId(); //取当前的默认帐号
        char    *acc_addr = mail_SenderAddr(acc_id);
        wchar   acc_addr_wstr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
        
        item_data.img_type = GUIRICHTEXT_IMAGE_RES;
        item_data.img_data.res.id = MMIMAIL_GetIconByType(MMIMAIL_ICON_ACCOUNT);

        //显示接收帐号
        MMIAPICOM_StrToWstr((uint8 *)acc_addr, acc_addr_wstr);
        item_data.text_data.buf.str_ptr = acc_addr_wstr; /*lint !e733*/
        item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(acc_addr_wstr);
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
}

/*****************************************************************************/
//	Description : 添加联系人
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailContact(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_ADDRESS_T          **contact_addr,
                                uint16                  contact_count,
                                MMIMAIL_ICON_ENUM_E     icon_type,
                                uint16                  *index_ptr
                               )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint32              i = 0;
    uint32              string_len = 0;
    uint32              total_len = 0;
    wchar               *addr_str_ptr = PNULL;
    wchar               semicolon[]=L";";
    wchar               wstr_addr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar               wstr_null[] = L" ";

    if(PNULL == contact_addr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailContact: PNULL == contact_addr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2186_112_2_18_2_48_20_368,(uint8*)"");
        return;
    }

    item_data.img_type        = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id = MMIMAIL_GetIconByType(icon_type);

    //COMM_TRACE:"MMIMAIL==> AddMailContact: total num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2193_112_2_18_2_48_20_369,(uint8*)"d", contact_count);

    if (0 == contact_count)
    {
        //COMM_TRACE:"MMIMAIL==> AddMailContact: receiver is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2197_112_2_18_2_48_20_370,(uint8*)"");
        
        //没有收件人时，只显示图标
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;

        //Align left
        item_data.text_data.buf.str_ptr = wstr_null;
        item_data.text_data.buf.len = 1;

        GUIRICHTEXT_AddItem(
            ctrl_id,
            &item_data,
            index_ptr
            );

    }
    else if (1 == contact_count)
    {
        if ((PNULL != contact_addr[0])
            && (PNULL != contact_addr[0]->email)
            && ((string_len = strlen((char *)contact_addr[0]->email)) > 0))
        {
            if (contact_addr[0]->name.wstr_len > 0)
            {
                //显示名字 
                item_data.text_data.buf.str_ptr = contact_addr[0]->name.wstr_ptr;
                item_data.text_data.buf.len    = contact_addr[0]->name.wstr_len;
            }
            else
            {
                //显示邮件地址
                if (string_len <= MMIMAIL_MAX_ADDRESS_LEN)
                {
                    MMIAPICOM_StrToWstr((const uint8 *)contact_addr[i]->email, wstr_addr);
                    item_data.text_data.buf.str_ptr = wstr_addr;
                    item_data.text_data.buf.len    = string_len;
                }
                else
                {
                    //COMM_TRACE:"MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2237_112_2_18_2_48_21_371,(uint8*)"s", contact_addr[i]->email);
                }
            }
            
            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
        else
        {
            //没有收件人时，只显示图标
            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;

            //Align left
            item_data.text_data.buf.str_ptr = wstr_null;
            item_data.text_data.buf.len = 1;

            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
    }
    else
    {
        //合成多个收件人, 用;分隔
        for (i = 0; i < contact_count; i++)
        {
            if ((PNULL != contact_addr[i])
                && (PNULL != contact_addr[i]->email)
                && ((string_len = strlen((char *)contact_addr[i]->email)) > 0) 
                && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
            {
                if (contact_addr[i]->name.wstr_len > 0)
                {
                    //显示名字 
                    total_len += contact_addr[i]->name.wstr_len + 1;//加上分割符的空间
                }
                else
                {
                    //显示邮件地址
                    total_len += string_len + 1;//加上分割符的空间
                }
            }
            else
            {
                break;
            }
        }

        //收件人不为空
        if (total_len > 0)
        {
            addr_str_ptr = SCI_ALLOC_APP(total_len * 2 + 2);
            
            if (PNULL != addr_str_ptr)
            {
                SCI_MEMSET(addr_str_ptr, 0, total_len * 2 + 2);
                
                total_len = 0;
                for (i = 0; i < contact_count; i++)
                {
                    if ((PNULL != contact_addr[i])
                        && (PNULL != contact_addr[i]->email)
                        && ((string_len = strlen((char *)contact_addr[i]->email)) > 0) 
                        && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
                    {
                        if (contact_addr[i]->name.wstr_len > 0)
                        {
                            //显示名字 
                            MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)contact_addr[i]->name.wstr_ptr,contact_addr[i]->name.wstr_len);
                            MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)semicolon, 1);
                            total_len += contact_addr[i]->name.wstr_len + 1;//加上分割符的空间
                        }
                        else
                        {
                            //显示邮件地址
                            if (string_len <= MMIMAIL_MAX_ADDRESS_LEN)
                            {
                                MMIAPICOM_StrToWstr((const uint8 *)contact_addr[i]->email, wstr_addr);
                                MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)wstr_addr, string_len);
                                MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)semicolon, 1);
                                total_len += string_len + 1;//加上分割符的空间
                            }
                            else
                            {
                                //COMM_TRACE:"MMIMAIL==> AddMailContact: ==error== Email addr [%s] len > 128"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2329_112_2_18_2_48_21_372,(uint8*)"s", contact_addr[i]->email);
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                
                item_data.text_data.buf.str_ptr = addr_str_ptr;
                item_data.text_data.buf.len    = total_len;
                item_data.tag_type = GUIRICHTEXT_TAG_NONE;
                item_data.text_type = GUIRICHTEXT_TEXT_BUF;
                GUIRICHTEXT_AddItem(
                    ctrl_id,
                    &item_data,
                    index_ptr
                    );

                //释放空间
                SCI_FREE(addr_str_ptr);
                addr_str_ptr = PNULL;
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> AddMailContact: malloc memory fail, malloc size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2355_112_2_18_2_48_21_373,(uint8*)"d", (total_len * 2 + 2));
            }
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> AddMailContact: receiver is empty!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2360_112_2_18_2_48_21_374,(uint8*)"");

            //没有收件人时，只显示图标
            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;

            //Align left
            item_data.text_data.buf.str_ptr = wstr_null;
            item_data.text_data.buf.len = 1;

            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                index_ptr
                );
        }    
    }
}

/*****************************************************************************/
//	Description : 添加收件人
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailTo(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                               )
{
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailTo: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2391_112_2_18_2_48_21_375,(uint8*)"");
        return;
    }

    //COMM_TRACE:"MMIMAIL==> AddMailTo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2395_112_2_18_2_48_21_376,(uint8*)"");
    AddMailContact(ctrl_id, whole_mail_ptr->to, whole_mail_ptr->to_count, MMIMAIL_ICON_TO, index_ptr);
    return;
}

/*****************************************************************************/
//	Description : 添加抄送人
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailCc(
                        MMI_CTRL_ID_T           ctrl_id,
                        MAIL_MESSAGE_T          *whole_mail_ptr,
                        uint16                  *index_ptr
                        )
{
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailCc: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2412_112_2_18_2_48_21_377,(uint8*)"");
        return;
    }

    //COMM_TRACE:"MMIMAIL==> AddMailCc enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2416_112_2_18_2_48_21_378,(uint8*)"");
    AddMailContact(ctrl_id, whole_mail_ptr->cc, whole_mail_ptr->cc_count, MMIMAIL_ICON_CC, index_ptr);
    return;
}

/*****************************************************************************/
//	Description : 添加密送人
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailBcc(
                            MMI_CTRL_ID_T           ctrl_id,
                            MAIL_MESSAGE_T          *whole_mail_ptr,
                            uint16                  *index_ptr
                          )
{
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailBcc: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2433_112_2_18_2_48_21_379,(uint8*)"");
        return;
    }

    //COMM_TRACE:"MMIMAIL==> AddMailBcc enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2437_112_2_18_2_48_21_380,(uint8*)"");
    AddMailContact(ctrl_id, whole_mail_ptr->bcc, whole_mail_ptr->bcc_count, MMIMAIL_ICON_BCC, index_ptr);
    return;
}

/*****************************************************************************/
//	Description : 添加时间
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailTime(
                            MMI_CTRL_ID_T           ctrl_id,
                            MAIL_MESSAGE_T          *whole_mail_ptr,
                            uint16                  *index_ptr
                           )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_TM_T            mail_time = {0};
    char                time_str[MMIMAIL_MAX_LIST_DISP_LEN + 1] = {0};
    wchar               time_wstr[MMIMAIL_MAX_LIST_DISP_LEN + 1] = {0};
    uint32              string_len = 0;

    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailTime: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2460_112_2_18_2_48_21_381,(uint8*)"");
        return;
    }

    mail_DateStrToTime((const char *)whole_mail_ptr->date, &mail_time);
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d",mail_time.tm_year, mail_time.tm_mon, mail_time.tm_mday, mail_time.tm_hour, mail_time.tm_min, mail_time.tm_sec);

    MMIAPICOM_StrToWstr((uint8 *)time_str,time_wstr);
    string_len = MMIAPICOM_Wstrlen(time_wstr);
    if (string_len > 0)
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_RES;
        item_data.img_data.res.id    = MMIMAIL_GetIconByType(MMIMAIL_ICON_TIME);
        
        string_len = MIN(string_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);        
        item_data.text_data.buf.str_ptr = time_wstr;
        item_data.text_data.buf.len    = string_len;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddMailTime: time is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2487_112_2_18_2_48_21_382,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 添加主题
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailSubject(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                               )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    wchar wstr_null[] = L" ";

    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailSubject: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2506_112_2_18_2_48_21_383,(uint8*)"");
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id    = MMIMAIL_GetIconByType(MMIMAIL_ICON_SUBJECT);
    
    if (whole_mail_ptr->subject.wstr_len > 0)
    {        
        item_data.text_data.buf.str_ptr = whole_mail_ptr->subject.wstr_ptr;
        item_data.text_data.buf.len    = whole_mail_ptr->subject.wstr_len;
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddMailSubject: subject is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2527_112_2_18_2_48_21_384,(uint8*)"");
        
        //只显示图标
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;

        //Align left
        item_data.text_data.buf.str_ptr = wstr_null;
        item_data.text_data.buf.len = 1;

        GUIRICHTEXT_AddItem(
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
}

/*****************************************************************************/
//	Description : 添加附件列表
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailAccessory(
                                    MMI_CTRL_ID_T           ctrl_id,
                                    MAIL_MESSAGE_T          *whole_mail_ptr,
                                    uint16                  *index_ptr
                                 )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint32              i = 0;
    uint32              string_len = 0;
    uint32              total_len = 0;
    wchar               *str_ptr = PNULL;
    wchar               semicolon[]=L";";
    
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailAccessory: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2564_112_2_18_2_48_21_385,(uint8*)"");
        return;
    }
    
    //合成多个附件, 用;分隔
    for (i = 0; i < MAXMIMEPARTS; i++)
    {
        if ((PNULL != whole_mail_ptr->mailmessageparts[i])
            && ((string_len = whole_mail_ptr->mailmessageparts[i]->name.wstr_len) > 0) 
            && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
        {
            total_len += string_len + 1;//加上分割符的空间
        }
        else
        {
            break;
        }
    }

    //附件不为空
    if (total_len > 0)
    {
        str_ptr = SCI_ALLOC_APP(total_len * 2 + 2);
        
        if (PNULL != str_ptr)
        {
            SCI_MEMSET(str_ptr, 0, total_len * 2 + 2);
            
            total_len = 0;
            for (i = 0; i < MAXMIMEPARTS; i++)
            {
                if ((PNULL != whole_mail_ptr->mailmessageparts[i])
                    && ((string_len = whole_mail_ptr->mailmessageparts[i]->name.wstr_len) > 0) 
                    && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
                {
                    MMIAPICOM_Wstrncat(str_ptr, (const wchar *)whole_mail_ptr->mailmessageparts[i]->name.wstr_ptr, string_len);
                    MMIAPICOM_Wstrncat(str_ptr, (const wchar *)semicolon, 1);
                    total_len += string_len + 1;//加上分割符的空间
                }
                else
                {
                    break;
                }
            }

            item_data.text_data.buf.str_ptr = str_ptr;
            item_data.text_data.buf.len    = total_len;
            
            item_data.img_type        = GUIRICHTEXT_IMAGE_RES;
            item_data.img_data.res.id = MMIMAIL_GetIconByType(MMIMAIL_ICON_ACCESSORY);

            item_data.tag_type = GUIRICHTEXT_TAG_NONE;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                index_ptr
                );

            //释放空间
            SCI_FREE(str_ptr);
            str_ptr = PNULL;
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> AddMailAccessory: malloc memory fail, malloc size = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2629_112_2_18_2_48_21_386,(uint8*)"d", (total_len * 2 + 2));
        }
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddMailAccessory: accessory is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2634_112_2_18_2_48_21_387,(uint8*)"");
    }
}

/*****************************************************************************/
//	Description : 添加正文与标题之间的分隔符显示
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailSeperator(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                               )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    wchar   seperator[] = MMIMAIL_READ_CONTENT_SEPERATOR;
    
    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailSeperator: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2653_112_2_18_2_48_21_388,(uint8*)"");
        return;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_data.buf.str_ptr = seperator;
    item_data.text_data.buf.len    = MMIAPICOM_Wstrlen(seperator);
    item_data.tag_type = GUIRICHTEXT_TAG_NONE;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    GUIRICHTEXT_AddItem (
        ctrl_id,
        &item_data,
        index_ptr
        );
}

/*****************************************************************************/
//	Description : 添加正文
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailContent(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                uint16                  *index_ptr
                               )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    MMI_STRING_T        string = {0};

    if(PNULL == whole_mail_ptr ||PNULL == index_ptr )
    {
        //SCI_TRACE_LOW:"PUSH MAIL  AddMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2684_112_2_18_2_48_21_389,(uint8*)"");
        return;
    }
   
    if (whole_mail_ptr->textbody.wstr_len > 0)
    {
        item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
        item_data.img_data.res.id    = MMIMAIL_GetIconByType(MMIMAIL_ICON_CONTENT);
        
        item_data.text_data.buf.str_ptr = whole_mail_ptr->textbody.wstr_ptr;
        item_data.text_data.buf.len    = whole_mail_ptr->textbody.wstr_len;
#ifdef BROWSER_SUPPORT
        item_data.tag_type = GUIRICHTEXT_TAG_PARSE_URL;
#else
        item_data.tag_type = GUIRICHTEXT_TAG_NONE;
#endif
        item_data.text_type = GUIRICHTEXT_TEXT_BUF;

        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );

        //正文内容大于控件支持的最大字数时，显示更多，提示用户打开新窗口查看全部正文
        if (whole_mail_ptr->textbody.wstr_len > GUIRICHTEXT_TEXT_BUF_MAX_LEN)
        {
            MMI_GetLabelTextByLang(TXT_MAIL_MORE, &string);
            item_data.text_data.buf.str_ptr = string.wstr_ptr;
            item_data.text_data.buf.len    = string.wstr_len;
            item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;
            GUIRICHTEXT_AddItem (
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
    }
    else
    {
        //什么也不显示
        //COMM_TRACE:"MMIMAIL==> AddMailContent: content is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2726_112_2_18_2_48_21_390,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 显示邮件内容
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowMailContent(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr,
                                BOOLEAN                 is_disp_recver //是否显示收件人、抄送人和密送人
                                )
{
    uint16 index = 0;
    
    if(PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"PUSH MAIL  ShowMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2744_112_2_18_2_48_22_391,(uint8*)"");
        return;
    }

    //先删除所有节点，再重新添加
    GUIRICHTEXT_DeleteAllItems(ctrl_id);
    
    AddMailSender(ctrl_id, whole_mail_ptr, &index); //发件人

    if (MMIMAIL_INBOX == MMIMAIL_GetCurBoxType())
    {
        AddMailAccount(ctrl_id, whole_mail_ptr, &index); //接收帐号
    }
    
    if (is_disp_recver)
    {
        AddMailTo(ctrl_id, whole_mail_ptr, &index); //收件人
        AddMailCc(ctrl_id, whole_mail_ptr, &index); //抄送
        AddMailBcc(ctrl_id, whole_mail_ptr, &index); //密送
    }
    
    AddMailTime(ctrl_id, whole_mail_ptr, &index); //日期
    AddMailSubject(ctrl_id, whole_mail_ptr, &index); //主题
    AddMailAccessory(ctrl_id, whole_mail_ptr, &index); //附件列表

    if (whole_mail_ptr->textbody.wstr_len > 0) //有正文
    {
        AddMailSeperator(ctrl_id, whole_mail_ptr, &index); //添加正文与标题之间的分隔符显示
        AddMailContent(ctrl_id, whole_mail_ptr, &index); //正文
    }
}

/*****************************************************************************/
//  Description : 显示全部邮件正文
//  Global resource dependence : 
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void ShowAllMailContent(
                                MMI_CTRL_ID_T           ctrl_id,
                                MAIL_MESSAGE_T          *whole_mail_ptr
                                )
{
    wchar *wstr_ptr = PNULL;
    uint16 wstr_len = 0;
    
    if(PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"PUSH MAIL  ShowAllMailContent: PNULL == whole_mail_ptr ||PNULL == index_ptr "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2790_112_2_18_2_48_22_392,(uint8*)"");
        return;
    }

    wstr_ptr = whole_mail_ptr->textbody.wstr_ptr;
    wstr_len = whole_mail_ptr->textbody.wstr_len;
    
    GUITEXT_SetString(ctrl_id, wstr_ptr, wstr_len , FALSE);
}

/*****************************************************************************/
//	Description : 设置阅读界面softkey
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void SetReadWinSoftkey(MMI_WIN_ID_T win_id, BOOLEAN is_disp_mid)
{
    MMI_TEXT_ID_T leftsoft_id = STXT_OPTION;
    MMI_TEXT_ID_T rightsoft_id = STXT_RETURN;
    MMI_TEXT_ID_T middlesoft_id = TXT_NULL;

    if (is_disp_mid)
    {
        switch (MMIMAIL_GetCurBoxType())
        {
        case MMIMAIL_INBOX:
            middlesoft_id = TXT_REPLY;
            break;

        case MMIMAIL_OUTBOX:
        case MMIMAIL_DRAFTBOX:
            middlesoft_id = TXT_EDIT;
            break;
            
        case MMIMAIL_SENTBOX:
            middlesoft_id = TXT_COMMON_FORWARD;
            break;
            
        default:
            break;
        }
    }
    else
    {
        leftsoft_id = TXT_VIEW;
    }
    
    GUIWIN_SetSoftkeyTextId(win_id, leftsoft_id, middlesoft_id, rightsoft_id, TRUE);
}

/*****************************************************************************/
//	Description : 插入menu结点
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,        //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index           //位置
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  

/*****************************************************************************/
//	Description : 打开阅读邮件选项窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  OpenMailReadOptWin(void)
{
    MMI_WIN_ID_T                win_id = MMIMAIL_READ_MAIL_OPT_WIN_ID;
    MMI_CTRL_ID_T               ctrl_id = MMIMAIL_READ_OPT_MENU_CTRL_ID;
    //MMI_STRING_T                kstring = {0};
    MMI_TEXT_ID_T               text_id = s_is_disp_all ? TXT_MAIL_HIDE_ALL_RECEIVERS: TXT_MAIL_DISPLAY_ALL_RECEIVERS;
    uint32                      i = 0;

    MMK_CreateWin((uint32 *)MMIMAIL_READ_MAIL_OPT_WIN_TAB,PNULL);

    //创建动态选项菜单
    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);

    switch (MMIMAIL_GetCurBoxType())
    {
        case MMIMAIL_INBOX:
            InsertOptMenuNodeByLabel(ctrl_id, TXT_REPLY, MMIMAIL_NODE_READ_OPT_REPLY, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_MAIL_REPLY_ALL, MMIMAIL_NODE_READ_OPT_REPLY_ALL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_MAIL_FORWARD_CONTENT, MMIMAIL_NODE_READ_OPT_FORWARD_CONTENT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_MAIL_FORWARD_ACC, MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ACC, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_MAIL_FORWARD_ALL, MMIMAIL_NODE_READ_OPT_SERVER_FORWARD_ALL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_MAIL_READ_ACCESSORY, MMIMAIL_NODE_READ_OPT_ACCESSORY, 0, i++);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(!MMIMAIL_CurMailIsLocked())
#endif
            InsertOptMenuNodeByLabel(ctrl_id, TXT_DELETE, MMIMAIL_NODE_READ_OPT_DEL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_COMM_SAVE_CONTACT, MMIMAIL_NODE_READ_OPT_SAVE_CONTACT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, text_id, MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS, 0, i++);
            break;

        case MMIMAIL_OUTBOX:
            InsertOptMenuNodeByLabel(ctrl_id, TXT_EDIT, MMIMAIL_NODE_READ_OPT_EDIT, 0, i++);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(!MMIMAIL_IsSendingCurMail())
#endif
            InsertOptMenuNodeByLabel(ctrl_id, TXT_SEND, MMIMAIL_NODE_READ_OPT_SEND, 0, i++);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(!MMIMAIL_CurMailIsLocked() && !MMIMAIL_IsSendingCurMail())
#endif
            InsertOptMenuNodeByLabel(ctrl_id, TXT_DELETE, MMIMAIL_NODE_READ_OPT_DEL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_COMM_SAVE_CONTACT, MMIMAIL_NODE_READ_OPT_SAVE_CONTACT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, text_id, MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS, 0, i++);
            break;
        case MMIMAIL_DRAFTBOX:
            InsertOptMenuNodeByLabel(ctrl_id, TXT_EDIT, MMIMAIL_NODE_READ_OPT_EDIT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_SEND, MMIMAIL_NODE_READ_OPT_SEND, 0, i++);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(!MMIMAIL_CurMailIsLocked())
#endif
            InsertOptMenuNodeByLabel(ctrl_id, TXT_DELETE, MMIMAIL_NODE_READ_OPT_DEL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_COMM_SAVE_CONTACT, MMIMAIL_NODE_READ_OPT_SAVE_CONTACT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, text_id, MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS, 0, i++);
            break;
            
        case MMIMAIL_SENTBOX:
            InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_FORWARD, MMIMAIL_NODE_READ_OPT_FORWARD_ALL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_RESEND, MMIMAIL_NODE_READ_OPT_RESEND, 0, i++);
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(!MMIMAIL_CurMailIsLocked())
#endif
            InsertOptMenuNodeByLabel(ctrl_id, TXT_DELETE, MMIMAIL_NODE_READ_OPT_DEL, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, TXT_COMM_SAVE_CONTACT, MMIMAIL_NODE_READ_OPT_SAVE_CONTACT, 0, i++);
            InsertOptMenuNodeByLabel(ctrl_id, text_id, MMIMAIL_NODE_READ_OPT_DISP_ALL_RECEIVERS, 0, i++);
            break;
                        
        default:
            //COMM_TRACE:"MMIMAIL==> OpenMailReadOptWin:current box type is invalid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2929_112_2_18_2_48_22_393,(uint8*)"");
            break;
    }

    return;
}

/*****************************************************************************/
//	Description : 打开附件选项窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  OpenAccessoryOptWin(void)
{
    MMI_WIN_ID_T            win_id = MMIMAIL_READ_ACC_OPT_MENU_WIN_ID;
    MMI_CTRL_ID_T           ctrl_id = MMIMAIL_READ_ACC_OPT_MENU_CTRL_ID;
    MAIL_MESSAGE_PART_T     *acc_ptr = MMIMAIL_GetCurOperAcc();

    if (PNULL == acc_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail OpenAccessoryOptWin PNULL == acc_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_2948_112_2_18_2_48_22_394,(uint8*)"");
        return ;
    }
    
    MMK_CreateWin((uint32 *)MMIMAIL_READ_ACC_OPT_MENU_WIN_TAB,PNULL);
    
    //创建动态选项菜单
    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);

    if (!MMIMAIL_IsAccessoryDownloaded(acc_ptr))
    {
        //附件未下载
        InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_DOWNLOAD_COMN, MMIMAIL_NODE_ACCESSORY_OPT_DL_SAVE, 0, 0);
    }
    else if (MMIMAIL_IsAccessoryAutoChanged(acc_ptr, PNULL))
    {
        //附件已下载且进行了格式转换
        InsertOptMenuNodeByLabel(ctrl_id, TXT_EXPAND, MMIMAIL_NODE_ACCESSORY_OPT_DL_SAVE, 0, 0);
    }
    else
    {
        //附件已下载且为原始格式
        InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_SAVE_AS, MMIMAIL_NODE_ACCESSORY_OPT_DL_SAVE, 0, 0);
    }

    return;
}

/*****************************************************************************/
//	Description : 打开附件展开窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:经转换格式的附件列表
/*****************************************************************************/
LOCAL void OpenAccessoryExpandWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_READ_ACC_EXPAND_LIST_WIN_TAB,PNULL);
}

/*****************************************************************************/
//	Description : 获取附件列表中间键文本
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetAccListItemMidSoftkey(MAIL_MESSAGE_PART_T *acc_ptr)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;

    if ((PNULL != acc_ptr)
        && (MMIMAIL_IsAccessoryDownloaded(acc_ptr))
        && (!MMIMAIL_IsAccessoryAutoChanged(acc_ptr, PNULL)))
    {
        //附件已下载且为原始格式
        text_id = STXT_PREVIEW;
    }

    return text_id;
}

/*****************************************************************************/
//	Description : 释放当前查看mail占用的空间
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void FreeMailSpace(MAIL_MESSAGE_T   *whole_mail_ptr)
{
    //释放邮件结构体所占内存
    MMIEMAIL_DestroyMessage(whole_mail_ptr);

    //设置当前显示邮件为空
    MMIMAIL_SetCurDispMail(PNULL);
}

/*****************************************************************************/
//	Description : 切换到下一封邮件
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void SwitchToNextMail(
                                    BOOLEAN is_next //TRUE:后一封  FALSE:前一封
                                )
{
    MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = PNULL;
    MMIMAIL_MAIL_SUMMERY_T *next_mail_ptr = PNULL;
    
    cur_mail_ptr = MMIMAIL_GetCurOperationMail();

    if (PNULL != cur_mail_ptr)
    {
        next_mail_ptr = is_next ? cur_mail_ptr->next_ptr : cur_mail_ptr->pre_ptr;
        if (PNULL != next_mail_ptr)
        {
            //保存当前正在操作的邮件
            MMIMAIL_SetCurOperationMail(next_mail_ptr);

            //设置当前操作类型
            SetHandleOkMenuOperation(MMIMAIL_OPER_OPEN_READ_WIN);

            //通知mail线程开始解码
            MMIAPIMAIL_ReadMail(next_mail_ptr->record_id);            
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> SwitchToNextMail: no next mail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3046_112_2_18_2_48_22_395,(uint8*)"");
        }
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> SwitchToNextMail: cur_mail_ptr is NULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3051_112_2_18_2_48_22_396,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 添加附件列表项
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void AppendAccessoryListItem(
                                            MMI_CTRL_ID_T           ctrl_id,
                                            MMI_TEXT_ID_T           left_softkey_id,
                                            MMI_TEXT_ID_T           mid_softkey_id,
                                            MMI_TEXT_ID_T           right_softkey_id,
                                            const wchar*            filename,
                                            uint16                  filename_len,
                                        	uint32                  filesize
                                          )
{
    GUILIST_ITEM_T       item_t    =  {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    if (filename_len == 0 || PNULL == filename)
    {
        //SCI_TRACE_LOW:"MMIMail AppendAccessoryListItem filename_len <= 0 OR PNULL == filename"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3075_112_2_18_2_48_22_397,(uint8*)"");
        return ;
    }
    
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;    
    item_t.item_data_ptr = &item_data;

    //设置softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    //根据文件类型设置图标
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType( MMIFMM_FILE_TYPE_NORMAL, MMI_DEVICE_UDISK );

    //设置文件名
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)filename;
    item_data.item_content[1].item_data.text_buffer.wstr_len = filename_len;

    //设置文件大小
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    MMIAPIFMM_GetFileSizeString( filesize, temp_wstr, MMIMAIL_FILE_SIZE_STR_LEN, TRUE);
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen( temp_wstr );

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : 更新附件列表项
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void ReplaceAccessoryListItem(
                                            MMI_CTRL_ID_T           ctrl_id,
                                            MMI_TEXT_ID_T           left_softkey_id,
                                            MMI_TEXT_ID_T           mid_softkey_id,
                                            MMI_TEXT_ID_T           right_softkey_id,
                                            const wchar*            filename,
                                            uint16                  filename_len,
                                        	uint32                  filesize,
                                        	uint16                  pos
                                          )
{
    GUILIST_ITEM_T       item_t    =  {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};

    if (filename_len == 0 || PNULL == filename)
    {
        //SCI_TRACE_LOW:"MMIMail ReplaceAccessoryListItem filename_len <=  0 OR  PNULL == filename"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3126_112_2_18_2_48_22_398,(uint8*)"");
        return ;
    }
    
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;    
    item_t.item_data_ptr = &item_data;

    //设置softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    //根据文件类型设置图标
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType( MMIFMM_FILE_TYPE_NORMAL, MMI_DEVICE_UDISK );

    //设置文件名
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)filename;
    item_data.item_content[1].item_data.text_buffer.wstr_len = filename_len;

    //设置文件大小
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    MMIAPIFMM_GetFileSizeString( filesize, temp_wstr, MMIMAIL_FILE_SIZE_STR_LEN, TRUE);
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen( temp_wstr );

    GUILIST_ReplaceItem(ctrl_id, &item_t, pos);
}

/*****************************************************************************/
//	Description : 判断焦点是否在更多上
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOnReadMore(MMI_CTRL_ID_T ctrl_id)
{
    GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
    BOOLEAN result = FALSE;
    
    GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);
    if (GUIRICHTEXT_TAG_TXT_NORMAL == focus_element.type)
    {
        result = TRUE;
    }
    
    return result;
}   

/*****************************************************************************/
//	Description : 判断焦点是否在网址上
//	Global resource dependence : none
//	Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAil_IsFocusOnURL(MMI_CTRL_ID_T ctrl_id)
{
#ifdef BROWSER_SUPPORT
    GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
    BOOLEAN result = FALSE;

    GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);

    if (GUIRICHTEXT_TAG_URL == focus_element.type)
    {
        result = TRUE;
    }

    return result;
#else
    return FALSE;
#endif
}   

/*****************************************************************************/
//	Description : 打开网址
//	Global resource dependence : none
//	Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL void MMIMAil_GoToFocusedURL(MMI_CTRL_ID_T ctrl_id)
{
#ifdef BROWSER_SUPPORT
    GUIRICHTEXT_FOCUS_ELEM_T focus_element = {0};
    MMIBROWSER_ENTRY_PARAM entry_para = {0};
    uint8 * url_ptr = PNULL;
    uint16 url_len = 0;

    GUIRICHTEXT_GetFocusElement(ctrl_id, &focus_element);

    if (GUIRICHTEXT_TAG_URL == focus_element.type)
    {
        if (PNULL != focus_element.data.str_ptr)
        {
            url_len = focus_element.data.len * 3 + 1;
            url_ptr = SCI_ALLOCA(url_len * sizeof(uint8));
            if (PNULL == url_ptr)
            {
                return;
            }

            SCI_MEMSET(url_ptr, 0x00, (focus_element.data.len * 3 + 1) * sizeof(uint8));
            GUI_WstrToUTF8(url_ptr, url_len, focus_element.data.str_ptr, focus_element.data.len);
            entry_para.url_ptr = (char*)url_ptr;
            entry_para.type = MMIBROWSER_ACCESS_URL;
            entry_para.dual_sys = MN_DUAL_SYS_MAX;
            entry_para.user_agent_ptr = NULL;
            MMIAPIBROWSER_Entry(&entry_para);
        }
    }
#endif
}

/*****************************************************************************/
//	Description : 更新邮件已读状态
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN UpdateMailReadState(uint32 record_id)
{
    MMIMAIL_MAIL_SUMMERY_T *cur_mail = PNULL;
    BOOLEAN is_complete = TRUE;

    //标记未读邮件为已读
    cur_mail = MMIMAIL_GetCurOperationMail();
    if ((PNULL != cur_mail) && (record_id == cur_mail->record_id))
    {
        if ((MMIMAIL_STATE_UNREAD == cur_mail->state) 
            || (MMIMAIL_STATE_UNREAD_WITH_ACC == cur_mail->state))
        {
            //判断当前邮件是否下载完整
            is_complete = mail_IsMsgIntegrated(record_id);
            if (!is_complete)
            {
                //未读且下载不完整时提示用户
                MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_INTEGRATED, IMAGE_PUBWIN_WARNING);
            }
    
            cur_mail->state -= ((int32)MMIMAIL_STATE_UNREAD - (int32)MMIMAIL_STATE_READ); //设置未读为已读
            MMIMAIL_DecreaseInboxBoxUnreadNum();
            MMIMAIL_SetNeedSort(TRUE);
            if (MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
            {
                MMK_SendMsg(MMIMAIL_MAIN_MENU_WIN_ID,
                    MSG_MAIL_NEEDUPDATEMENU,
                    PNULL);
            }
            MAIN_SetMsgState(TRUE); //更新icon区显示
        }
    }
    else
    {
        if (PNULL == cur_mail)
        {
            //COMM_TRACE:"MMIMAIL==> UpdateMailReadState: current operation mail ptr is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3274_112_2_18_2_48_23_399,(uint8*)"");
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> UpdateMailReadState: record_id = %d, cur_mail->record_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3278_112_2_18_2_48_23_400,(uint8*)"dd", record_id, cur_mail->record_id);
        }
    }

    return TRUE;
}

/*****************************************************************************/
//	Description : 获取当前正在操作的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MAIL_MESSAGE_PART_T * MMIMAIL_GetCurOperAcc(void)
{
    return s_cur_oper_accessory;
}

/*****************************************************************************/
//	Description : 设置当前正在操作的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurOperAcc(MAIL_MESSAGE_PART_T * cur_acc)
{
    s_cur_oper_accessory = cur_acc;
}

/*****************************************************************************/
//	Description : 获取当前正在操作的展开列表中的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL mail_att_file_info * MMIMAIL_GetCurOperExpandAcc(void)
{
    uint16 cur_index = 0;
    
    if (PNULL != s_cur_expand_acc_list_info.list_ptr)
    {
        cur_index = s_cur_expand_acc_list_info.cur_index;
        return &(s_cur_expand_acc_list_info.list_ptr->file_info[cur_index]);
    }    
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_GetCurOperExpandAcc: Error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3318_112_2_18_2_48_23_401,(uint8*)"");
        return PNULL;
    }
}

/*****************************************************************************/
//	Description : 设置当前正在操作的展开列表中的附件
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurOperExpandAcc(uint16 cur_index)
{
    s_cur_expand_acc_list_info.cur_index = cur_index;
}

/*****************************************************************************/
//	Description : 获取当前展开的附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL mail_att_multi_file_list * MMIMAIL_GetCurExpandAccList(void)
{
    return s_cur_expand_acc_list_info.list_ptr;
}

/*****************************************************************************/
//	Description : 设置当前展开的附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetCurExpandAccList(mail_att_multi_file_list * acc_list)
{
    s_cur_expand_acc_list_info.list_ptr = acc_list;
}

/*****************************************************************************/
//	Description : 判断当前显示邮件的附件是否已经下载
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAccessoryDownloaded(MAIL_MESSAGE_PART_T *accessory_ptr)
{
    BOOLEAN         result = FALSE;
    MAIL_MESSAGE_T  *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    
    if (PNULL == whole_mail_ptr ||PNULL == accessory_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail MMIMAIL_IsAccessoryDownloaded PNULL == whole_mail_ptr OR PNULL == accessory_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3361_112_2_18_2_48_23_402,(uint8*)"");
        return FALSE;
    }
    
    //判断附件是否存在
    result = mail_IsAttachFileExist((char *)whole_mail_ptr->uidl, 
                                    (char *)accessory_ptr->contentid, 
                                    &(accessory_ptr->filename));

    //COMM_TRACE:"MMIMAIL==> MMIMAIL_IsAccessoryDownloaded: result = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3370_112_2_18_2_48_23_403,(uint8*)"d", result);
    
    return result;
}

/*****************************************************************************/
//	Description : 判断当前显示邮件的附件是否进行了自动格式转换
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsAccessoryAutoChanged(
                                            MAIL_MESSAGE_PART_T *accessory_ptr,
                                            uint16              *changed_num_ptr //转换之后的文件个数
                                            )
{
    BOOLEAN         result = FALSE;
    uint16          num = 0;
    MAIL_MESSAGE_T  *whole_mail_ptr = MMIMAIL_GetCurDispMail();
    
    if (PNULL == whole_mail_ptr ||PNULL == accessory_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail MMIMAIL_IsAccessoryAutoChanged PNULL == whole_mail_ptr OR PNULL == accessory_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3390_112_2_18_2_48_23_404,(uint8*)"");
        return FALSE;
    }

    //判断附件是否存在
    num = mail_IsAttachMultiFile((char *)whole_mail_ptr->uidl, 
                                    (char *)accessory_ptr->contentid, 
                                    &(accessory_ptr->filename));

    //COMM_TRACE:"MMIMAIL==> MMIMAIL_IsAccessoryAutoChanged: changed num = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3399_112_2_18_2_48_23_405,(uint8*)"d", num);

    result = (num > 0) ? TRUE : FALSE;

    if (PNULL != changed_num_ptr)
    {
        *changed_num_ptr = num;
    }
    
    return result;
}

/*****************************************************************************/
//	Description : 打开附件列表窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_OpenAccessoryListWin(void)
{
    return MMK_CreateWin((uint32 *)MMIMAIL_READ_ACCESSORY_LIST_WIN_TAB,PNULL);
}

/*****************************************************************************/
//	Description : 加载附件列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccessoryList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              )
{
    uint16 i = 0;
    MMI_TEXT_ID_T   mid_text = TXT_NULL;

    if(PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMAIL_LoadAccessoryList whole_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3434_112_2_18_2_48_23_406,(uint8*)"");
        return FALSE;
    }
    GUILIST_RemoveAllItems(ctrl_id);
    
    if (whole_mail_ptr->partcount > 0)
    {
        for (i = 0; i < whole_mail_ptr->partcount; i++)
        {
            mid_text = GetAccListItemMidSoftkey(whole_mail_ptr->mailmessageparts[i]);
            AppendAccessoryListItem(ctrl_id,
                                    STXT_OPTION,
                                    mid_text,
                                    STXT_RETURN,
                                    whole_mail_ptr->mailmessageparts[i]->filename.wstr_ptr,
                                    whole_mail_ptr->mailmessageparts[i]->filename.wstr_len,
                                    whole_mail_ptr->mailmessageparts[i]->size
                                    );
        }
        return TRUE;        
    }
    else
    {
        MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
        return FALSE;
    }
}

/*****************************************************************************/
//	Description : 加载附件展开列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadAccExpandList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              )
{
    uint16  i = 0;
    uint16  list_num = 0;
    MAIL_MESSAGE_PART_T         *acc_ptr = MMIMAIL_GetCurOperAcc();
    mail_att_multi_file_list    *list_ptr = PNULL;
    MMIFILE_FILE_INFO_T         file_info = {0};
    
    if(PNULL == whole_mail_ptr ||PNULL == acc_ptr )
    {
        //SCI_TRACE_LOW:"MMIMAIL_LoadAccExpandList whole_mail_ptr || acc_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3479_112_2_18_2_48_23_407,(uint8*)"");
        return FALSE;
    }
    
    if (MMIMAIL_IsAccessoryAutoChanged(acc_ptr, &list_num))
    {
        list_ptr = mail_AttachMultiContent((char *)whole_mail_ptr->uidl, 
                                        (char *)acc_ptr->contentid, 
                                        &(acc_ptr->filename));

        if (PNULL == list_ptr)
        {
            //SCI_TRACE_LOW:"MMIMail MMIMAIL_LoadAccExpandList PNULL == list_ptr"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3491_112_2_18_2_48_23_408,(uint8*)"");
            return FALSE;
        }
        
        MMIMAIL_SetCurExpandAccList(list_ptr);
        
        GUILIST_RemoveAllItems(ctrl_id);
        for (i = 0; i < list_num; i++)
        {
            //解析全路径
            SCI_MEMSET(&file_info, 0, sizeof(MMIFILE_FILE_INFO_T));
            MMIAPIFMM_GetFileInfoFormFullPath((const wchar *)list_ptr->file_info[i].file_name.wstr_ptr,
                                                list_ptr->file_info[i].file_name.wstr_len,
                                                &file_info);        
            AppendAccessoryListItem(ctrl_id,
                                    TXT_COMMON_SAVE_AS,
                                    STXT_PREVIEW,
                                    STXT_RETURN,
                                    file_info.file_name,
                                    file_info.file_name_len,
                                    list_ptr->file_info[i].file_size
                                    );
        }
        return TRUE;        
    }
    else
    {
        MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
        return FALSE;
    }
}

/*****************************************************************************/
//	Description : 打开所有收件人地址列表窗口
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  MMIMAIL_OpenAllContactAddrListWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_READ_ALL_RECVER_LIST_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : 添加收件人列表项
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void AppendRecverListItem(
                                        MMI_CTRL_ID_T           ctrl_id,
                                        MMI_TEXT_ID_T           left_softkey_id,
                                        MMI_TEXT_ID_T           mid_softkey_id,
                                        MMI_TEXT_ID_T           right_softkey_id,
                                        MAIL_ADDRESS_T          *mail_addr
                                      )
{
    GUILIST_ITEM_T       item_t    =  {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    wchar temp_wstr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};

    if(PNULL == mail_addr)
    {
        //SCI_TRACE_LOW:"AppendRecverListItem mail_addr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3550_112_2_18_2_48_23_409,(uint8*)"");
        return;
    }
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT;    
    item_t.item_data_ptr = &item_data;

    //设置softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_BUFFER;
    //显示email地址或名字
    if (mail_addr->name.wstr_len > 0)
    {
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = mail_addr->name.wstr_ptr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = mail_addr->name.wstr_len;
    }
    else
    {
        MMIAPICOM_StrToWstr((const uint8 *)mail_addr->email, temp_wstr);
        item_data.item_content[0].item_data.text_buffer.wstr_ptr = temp_wstr;
        item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen((const wchar *)temp_wstr);
    }

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//	Description : 加载所有联系人列表
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_LoadRecverList(
                                                MMI_CTRL_ID_T   ctrl_id,
                                                MAIL_MESSAGE_T  *whole_mail_ptr
                                              )
{
    uint16  i = 0;
    uint16  j = 0;
    BOOLEAN have_recver = FALSE;

    if(PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMAIL_LoadRecverList whole_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3593_112_2_18_2_48_23_410,(uint8*)"");
        return FALSE;
    }

    GUILIST_RemoveAllItems(ctrl_id);

    //根据规范要求，收件箱和已发件箱的邮件可以保存发件人地址
    if (((MMIMAIL_INBOX == MMIMAIL_GetCurBoxType())
        || (MMIMAIL_SENTBOX == MMIMAIL_GetCurBoxType()))
        && (whole_mail_ptr->from_count > 0))
    {
        for (i = 0; i < whole_mail_ptr->from_count; i++)
        {
            AppendRecverListItem(ctrl_id,
                                    STXT_OPTION,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    whole_mail_ptr->from[i]
                                    );
            s_cur_recver_list[j++] = whole_mail_ptr->from[i];
        }
        have_recver = TRUE;        
    }
    
    if (whole_mail_ptr->to_count > 0)
    {
        for (i = 0; i < whole_mail_ptr->to_count; i++)
        {
            AppendRecverListItem(ctrl_id,
                                    STXT_OPTION,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    whole_mail_ptr->to[i]
                                    );
            s_cur_recver_list[j++] = whole_mail_ptr->to[i];
        }
        have_recver = TRUE;        
    }
    
    if (whole_mail_ptr->cc_count > 0)
    {
        for (i = 0; i < whole_mail_ptr->cc_count; i++)
        {
            AppendRecverListItem(ctrl_id,
                                    STXT_OPTION,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    whole_mail_ptr->cc[i]
                                    );
            s_cur_recver_list[j++] = whole_mail_ptr->cc[i];
        }
        have_recver = TRUE;        
    }
    
    if (whole_mail_ptr->bcc_count > 0)
    {
        for (i = 0; i < whole_mail_ptr->bcc_count; i++)
        {
            AppendRecverListItem(ctrl_id,
                                    STXT_OPTION,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    whole_mail_ptr->bcc[i]
                                    );
            s_cur_recver_list[j++] = whole_mail_ptr->bcc[i];
        }
        have_recver = TRUE;        
    }
    
    if (!have_recver)
    {
        MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, TXT_NULL);
    }

    return have_recver;
}

/*****************************************************************************/
//	Description : 收件人列表窗口处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadAllRecverListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_ALL_RECVER_LISTBOX_CTRL_ID;
    uint16          list_item_num = 0;
    uint16          max_item_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    MAIL_MESSAGE_T   *whole_mail_ptr = MMIMAIL_GetCurDispMail();    

    if (PNULL == whole_mail_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleReadAllRecverListWinMsg PNULL == whole_mail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3689_112_2_18_2_48_23_411,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
        
    //COMM_TRACE:"MMIMAIL==> HandleReadAllRecverListWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3693_112_2_18_2_48_23_412,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        max_item_num = whole_mail_ptr->to_count + whole_mail_ptr->cc_count + whole_mail_ptr->bcc_count;
        if ((MMIMAIL_INBOX == MMIMAIL_GetCurBoxType())
            || (MMIMAIL_SENTBOX == MMIMAIL_GetCurBoxType()))
        {
            //根据规范要求，收件箱和已发件箱的邮件可以保存发件人地址
            max_item_num += whole_mail_ptr->from_count;
        }
        if (max_item_num > 0)
        {
            s_cur_recver_list = comm_alloc(max_item_num * sizeof(MAIL_ADDRESS_T *));
            if (PNULL != s_cur_recver_list)
            {
                SCI_MEMSET(s_cur_recver_list, 0, max_item_num * sizeof(MAIL_ADDRESS_T *));
            }
            else
            {
                //SCI_TRACE_LOW:"MMIMail HandleReadAllRecverListWinMsg PNULL == s_cur_recver_list"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3715_112_2_18_2_48_23_413,(uint8*)"");
                return MMI_RESULT_FALSE;
            }
        }
        list_item_num = MAX(max_item_num, 1);
        GUILIST_SetMaxItem(ctrl_id, list_item_num, FALSE);

        //加载收件人列表项
        have_items = MMIMAIL_LoadRecverList(ctrl_id, whole_mail_ptr);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        s_cur_oper_addr = PNULL;
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if ( have_items )
        {
            uint16 selected_index = 0;
            
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);

            //保存当前正在操作的邮件地址
            s_cur_oper_addr = s_cur_recver_list[selected_index];

            //打开选项菜单
            MMK_CreateWin((uint32 *)MMIMAIL_READ_RECV_LIST_OPT_WIN_TAB,PNULL);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        if (PNULL != s_cur_recver_list)
        {
            comm_free(s_cur_recver_list);
        }
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
//	Description : 收件人列表选项菜单处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadRecvListOptWin(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIMAIL_READ_RECV_LIST_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleReadRecvListOptWin, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3793_112_2_18_2_48_24_414,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_SAVETO_NEW_ITEM_ID:
            MMK_CreateWin((uint32 *)MMIMAIL_READ_SEL_GROUP_WIN_TAB,PNULL);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case MMIMAIL_MENU_SAVETO_EXIST_ITEM_ID:
            {
                MMIPB_MAIL_T email_text = {0};
                uint16  num_len = 0;
                
                num_len = strlen((char *)s_cur_oper_addr->email);                
                email_text.wstr_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                MMI_STRNTOWSTR(email_text.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)s_cur_oper_addr->email, num_len, email_text.wstr_len);
                //@@@ MMIAPIPB_OpenEntryListForEx(&email_text, MMIPB_ADD_TYPE_EMAIL);
                MMIAPIPB_AddToExistContact(&email_text, MMIPB_ADD_TYPE_EMAIL,PNULL);
            }
            break;
#endif            
        default:
            //COMM_TRACE:"MMIMAIL==> HandleReadRecvListOptWin, menu_id = %d is invalid"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3827_112_2_18_2_48_24_415,(uint8*)"d", menu_id);
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
   
    return recode;
}

/*****************************************************************************/
//	Description : 选择分组界面处理函数
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReadSelGroupWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_READ_SEL_GROUP_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id= 0;
    MMI_MENU_ID_T       menu_id = 0;

    //COMM_TRACE:"MMIMAIL==> HandleReadSelGroupWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_READ_3861_112_2_18_2_48_24_416,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        //要求只能保存到手机上，将判断条件去掉，sim卡项一直灰化 yuanl modify 2010.8.5
        //if (MMI_SIM_TYPE_USIM != MMIPB_GetSimTypeByGroupID(PB_GROUP_SIM_1))
        {
            //非USIM卡，没有EMAIL域，使SIM卡项变灰
            GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
            MMIAPICOM_EnableGrayed(win_id, group_id, MMIMAIL_MENU_SIM_ITEM_ID, TRUE);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_PHONE_ITEM_ID:
            {
                MMIPB_MAIL_T email_text = {0};
                uint16 num_len = 0;

                num_len = strlen((char *)s_cur_oper_addr->email);                
                email_text.wstr_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                MMI_STRNTOWSTR(email_text.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)s_cur_oper_addr->email, num_len, email_text.wstr_len);
                MMIAPIPB_AddContactWin(&email_text, PB_GROUP_ALL, MMIPB_ADD_TYPE_EMAIL);
            }
            break;

        case MMIMAIL_MENU_SIM_ITEM_ID:
            {
                MMIPB_MAIL_T email_text = {0};
                uint16 num_len = 0;

                num_len = strlen((char *)s_cur_oper_addr->email);                
                email_text.wstr_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                MMI_STRNTOWSTR(email_text.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)s_cur_oper_addr->email, num_len, email_text.wstr_len);
                MMIAPIPB_AddContactWin(&email_text, PB_GROUP_SIM_1, MMIPB_ADD_TYPE_EMAIL);
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

	return recode;
}



/*Edit by script, ignore 8 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
