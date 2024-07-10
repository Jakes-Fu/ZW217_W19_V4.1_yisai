/***************************************************************************
** File Name:      mmimail_internal.h                                      *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define module internal relative    *
**                 contents.                                               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_INTERNEL_H_    
#define  _MMIMAIL_INTERNEL_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmimail_export.h"
#include "comm_debug.h"
#include "comm_memory.h"
#include "mmifmm_export.h"
#include "mail_message.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**----------------------------------------------------------------------*
 **                         MACRO DEFINITION                             *
 **----------------------------------------------------------------------*/
#define MMIMAIL_MAX_CONTENT_LEN             (50 * 1024)     //正文可输入最大字数 50K
#define MMIMAIL_MIN_EDIT_CONTENT_LEN        (10 * 1024)     //正文可编辑最少预留字数 10K
#define MMIMAIL_MAX_SUBJECT_LEN             (1 * 1024)      //主题可输入最大字数 1K
#define MMIMAIL_MAX_RECEIVER_NUM            20              //收件人最大个数,包括to/cc/bcc (范围:1~30)
#define MMIMAIL_MAX_ADDRESS_LEN             128             //邮件地址的最大长度，同MAIL_SETTING_INFO_T帐号定义
#define MMIMAIL_ALL_ADDR_LEN                (MMIMAIL_MAX_RECEIVER_NUM * MMIMAIL_MAX_ADDRESS_LEN)
#define MMIMAIL_MAX_NAME_LEN                64              //邮件显示名字的最大长度
#define MMIMAIL_MAX_ACC_NUM                 10              //附件总个数
#define MMIMAIL_MAX_ACC_SIZE                (1024 * 1024)   //附件总大小: 1M
#define MMIMAIL_MAX_FILENAME_LEN            MMIFMM_FILENAME_LEN         //文件名的最大长度    
#define MMIMAIL_MAX_FULLNAME_LEN            MMIFMM_FULL_FILENAME_LEN    //全路径的最大长度

#define MMIMAIL_MAX_ACC_NAME_LEN            128             //帐号名称最大长度
#define MMIMAIL_MAX_PASSWORD_LEN            16              //同MAIL_SETTING_INFO_T密码定义

#define MMIMAIL_MAX_LIST_DISP_LEN           50              //列表内容显示的最大长度

#define MMIMAIL_FILE_SIZE_STR_LEN           10              //文件大小字符串最大长度

#define MMIMAIL_MAX_CONTENT_SIZE            65535           //邮件正文能表示的最大值，因为正文长度为 uint16 类型

#define MMIMAIL_MAX_MEMSTATE_LABEL_LEN      (50)

#define MMIMAIL_FULLSCREEN_CLIENT_LEFT      0 
#define MMIMAIL_FULLSCREEN_CLIENT_TOP       MMI_TITLE_HEIGHT
#define MMIMAIL_FULLSCREEN_CLIENT_RIGHT     MMI_MAINSCREEN_RIGHT_MAX_PIXEL
#define MMIMAIL_FULLSCREEN_CLIENT_BOTTOM    MMI_CLIENT_RECT_BOTTOM

#define MMIMAIL_APPLY_MSG                   L"KTYX5"    //开通139邮箱短信
#define MMIMAIL_APPLY_NUM                   "10086"     //开通139邮箱号码

#define MMIMAIL_LOGOUT_MSG                  L"QXYX5"    //注销139邮箱短信

#define MMIMAIL_CHECK_MSG                   L"JCZH PT=TM001 VER=1.0B20100610"//账号检查请求
#define MMIMAIL_CHECK_NUM                   "10658172"  //账号检查号码

/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/
//邮箱帐号信息
typedef struct
{
    uint16  accountid;                              //账户id
    wchar   name[MMIMAIL_MAX_ACC_NAME_LEN + 1];     //帐号名称
    wchar   addr[MMIMAIL_MAX_ADDRESS_LEN + 1];      //帐号地址
    wchar   password[MMIMAIL_MAX_PASSWORD_LEN + 1]; //帐号密码
    wchar   addr_name[MMIMAIL_MAX_ADDRESS_LEN + 1]; //用户名称、地址别名
    
}MMIMAIL_ACCOUNT_INFO_T;

//邮件状态
typedef enum
{
    MMIMAIL_STATE_NULL,
    MMIMAIL_STATE_READ,              //已读
    MMIMAIL_STATE_READ_WITH_ACC,     //已读有附件
    MMIMAIL_STATE_UNREAD,            //未读
    MMIMAIL_STATE_UNREAD_WITH_ACC,   //未读有附件
    MMIMAIL_STATE_SERVER_REJECT,     //服务器拒绝
    MMIMAIL_STATE_SEND_FAIL,         //发送不成功
    MMIMAIL_STATE_SENDING,           //正在发送
    MMIMAIL_STATE_WAIT_SEND,         //等待发送
    MMIMAIL_STATE_SENT,              //已发信箱
    MMIMAIL_STATE_SENT_WITH_ACC,     //已发信箱有附件
    MMIMAIL_STATE_DRAFT,             //草稿箱
    MMIMAIL_STATE_DRAFT_WITH_ACC,    //草稿箱有附件
    MMIMAIL_STATE_MAX
}MMIMAIL_STATE_E;

//邮件图标
typedef enum
{
    MMIMAIL_ICON_FROM,          //发件人
    MMIMAIL_ICON_TO,            //收件人
    MMIMAIL_ICON_CC,            //抄送
    MMIMAIL_ICON_BCC,           //密送
    MMIMAIL_ICON_SUBJECT,       //标题
    MMIMAIL_ICON_CONTENT,       //正文
    MMIMAIL_ICON_ACCESSORY,     //附件
    MMIMAIL_ICON_TIME,          //时间
    MMIMAIL_ICON_ACCOUNT,       //接收帐号
    MMIMAIL_ICON_MAX
}MMIMAIL_ICON_ENUM_E;

//单封邮件概要信息
typedef struct MMIMAIL_MAIL_SUMMERY_T_TAG
{
    uint32              record_id;                  //索引
    uint32              time;                       //时间
    MMIMAIL_BOX_TYPE_E  box_type;                   //邮箱类型
    MMIMAIL_STATE_E     state;                      //邮件状态
    BOOLEAN             is_marked;                  //是否标记   
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
    uint32               item_index;             //邮箱list 控件索引
#endif

    wchar   addr[MMIMAIL_MAX_LIST_DISP_LEN + 1];    //邮件地址
    wchar   *name;                                  //联系人姓名，目前没有使用，邮件列表不显示本地联系人名字
    wchar   subject[MMIMAIL_MAX_LIST_DISP_LEN + 1]; //主题

    struct MMIMAIL_MAIL_SUMMERY_T_TAG *next_ptr;
    struct MMIMAIL_MAIL_SUMMERY_T_TAG *pre_ptr;
    
}MMIMAIL_MAIL_SUMMERY_T;

//邮箱相关信息
typedef struct
{
    uint16  total_inbox_num;                    //收件箱总邮件数
    uint16  total_outbox_num;                   //发件箱总邮件数
    uint16  total_sentbox_num;                  //已发邮件总数
    uint16  total_draftbox_num;                 //草稿箱总邮件数

    uint16  total_inbox_unread_num;             //未读邮件总数

    MMIMAIL_MAIL_SUMMERY_T *inbox_mail_list;     //收件箱邮件链表
    MMIMAIL_MAIL_SUMMERY_T *outbox_mail_list;    //发件箱邮件链表
    MMIMAIL_MAIL_SUMMERY_T *sentbox_mail_list;   //已发邮件链表
    MMIMAIL_MAIL_SUMMERY_T *draftbox_mail_list;  //草稿箱邮件链表
    
}MMIMAIL_BOX_INFO_T;

//操作类型
typedef enum
{
    MMIMAIL_OPER_NULL,                        //空状态
    MMIMAIL_OPER_OPEN_READ_WIN,     //打开邮件阅读窗口
    MMIMAIL_OPER_OPEN_EDIT_WIN,     //打开邮件编辑窗口
    MMIMAIL_OPER_OPEN_POPMENU       //打开弹出菜单
    
}MMIMAIL_OPERATION_T;

//静音时段枚举
typedef enum
{
    MMIMAIL_EDIT_SILENT_START_TIME, //开始时间
    MMIMAIL_EDIT_SILENT_END_TIME   //结束时间
    
}MMIMAIL_EDIT_SILENT_TIME_TYPE_E;

//邮箱管理相关信息
typedef struct
{
    BOOLEAN                 is_have_account;        //是否有开通的本地帐号
    BOOLEAN                 is_ready;               //邮箱是否准备好
    MAIL_ENTER_TYPE_E       enter_type;             //邮箱启动类型
    DPARAM                  enter_param;            //启动参数

    MMIMAIL_ACCOUNT_INFO_T  def_account;            //默认帐号
    MMIMAIL_BOX_INFO_T      def_account_box_info;   //默认帐号邮箱信息

    MMIMAIL_OPERATION_T     ok_operation;           //按ok键的操作类型
    
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_editwin_type;  //打开编辑窗口的类型，包括新建、转发、回复等操作类型
    
    MMIMAIL_BOX_TYPE_E      cur_box_type;           //当前正在操作的邮箱类型
    MMIMAIL_MAIL_SUMMERY_T  *cur_oper_mail;         //当前正在操作的邮件，包括概要信息，用于列表显示
    MAIL_MESSAGE_T          *cur_disp_mail;         //当前正在显示的邮件，完整邮件内容
    uint32                  cur_disp_record_id;     //当前正在显示的邮件 record_id
    BOOLEAN                 is_mail_sendby_id;      //标记当前邮件正在通过id发送
    
    uint32                  marked_mail_num;        //标记的邮件数量
    uint8                   mainmenu_win_timer_id;  //邮箱主菜单定时刷新定时器

    uint16                  cur_sel_account_idx;    //当前选择的帐号index
    MMIMAIL_ACCOUNT_INFO_T  cur_edit_account;       //当前正在编辑的account信息，保存临时编辑内容
    SCI_TIME_T              cur_edit_silent_start_time; //当前正在编辑的静音时段的开始时间
    SCI_TIME_T              cur_edit_silent_end_time;   //当前正在编辑的静音时段的结束时间
    MMIMAIL_EDIT_SILENT_TIME_TYPE_E cur_edit_time_type; //当前正在编辑的时间类型

    BOOLEAN                 is_account_num_change;  //帐号数目是否变化
    BOOLEAN                 is_account_name_change; //帐号名称是否变化
    
    BOOLEAN                 is_def_account_change;  //默认帐号是否变化

    BOOLEAN                 is_logout_oper;         //当前是否为注销操作

    BOOLEAN                 is_del_account_oper_by_bomb;         //当前是否为炸弹消息触发的删除账户操作
    
    BOOLEAN                 is_need_sort;           //是否需要重新排序
    
}MMIMAIL_MANAGE_INFO_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif
#endif

