/****************************************************************************
** File Name:      mmimms_internal.h                                       *
** Author:         minghu.mao                                              *
** Date:           05/26/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2009        minghu.mao       create
** 
****************************************************************************/

#ifndef _MMIMMS_INTERNAL_H
#define _MMIMMS_INTERNAL_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "mmimms_export.h"
#include "mmimms_id.h"
#include "mmidisplay_data.h"
#include "mmi_custom_define.h"
#include "mmimms_edit.h"
#include "mmimms_net.h"
#include "mmimms_push.h"
#include "guires.h"
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#include "mmisms_app.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define    MMIMMS_CLEAR_CLIENT(win_id)\
{\
GUI_RECT_T de_rect = MMITHEME_GetClientRect();\
GUI_LCD_DEV_INFO    de_lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};\
de_lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;\
de_lcd_dev_info.block_id = GUI_BLOCK_MAIN;\
GUIRES_DisplayImg(PNULL,&de_rect,&de_rect,win_id,IMAGE_COMMON_BG,&de_lcd_dev_info);\
}

//#define MMIMMS_MAX_NOTI_NUM           (MMIMMS_MAX_MMS_AND_MMSNOTI_NUM - MMIMMS_MAX_MMS_NUM)

#define MAX_NAME_LENGTH              MMS_FILE_NAME_LEN     //user address length
#define MMIMMS_ALL_ADDRESS_LEN       (MMIMMS_MAX_RECEIVER_NUM * 40)     //收件人的号码长度
#define MMIMMS_SLIDE_DURATION_MAX    600    //最大slide duaration
#define MMIMMS_SLIDE_DURATION_MIN    1      //最小slide duaration
#define ONE_K_LEN                    1024
#define ONE_KILO                     1000
#define MMIMMS_MULTIFILE_LIST_MAX_ITEM (20 * 2)
#define M_SECOND_1970_TO_1980      ((365*8 +366*2)*24*3600)
#define MMIMMS_TIME_STR_LEN          32
#define MMIMMS_SENDER_LEN            64

#define MMIMMS_SLIDE_TIME_DURING     1000
#define MMIMMS_SLIDE_TIME_STR_LEN    50
#define MMIMMS_SIZE_FLOAT_A          0.100000
#define MMIMMS_SIZE_FLOAT_B          0.000000

//max link setting number
#define MMIMMS_MAX_SETTING_NUM       20
#define MMIMMS_MAX_ATTACHMENT_NUM    5
//addtional file max number
#define MMIMMS_MAX_ADDFILE_NUM       10

//recieve and send option lable and DROPDOWNLIST coordinate
        
#define MMS_SETTING_DROPPAGE_NUM1                2
#define MMS_SETTING_DROPPAGE_NUM2                3
#define MMS_SETTING_DROPPAGE_NUM3                5
//滚动条颜色
#define MMIMMS_PROGRESS_BAR_COLOR                0xFCC0 

//百分比坐标
#define MMIMMS_PERCENT_ONE_HUNDRED               100
#define MMIMMS_PROGRESS_BAR_LEN                  115  //RESOURCE
#define MMIMMS_PROGRESS_BAR_TOP                  59
#define MMIMMS_PIC_DEFAULT_WIDTH                 98
#define MMIMMS_PIC_DEFAULT_HEIGHT                42
#define MMIMMS_MUSIC_DEFAULT_WIDTH               90
#define MMIMMS_MUSIC_DEFAULT_HEIGHT              13
#define MMIMMS_PAGE_BAR_HEIGHT                   10
#define MMIMMS_PAGE_BAR_HEIGHT_MAX               34 //for cr223525
#define MMIMMS_PAGE_BAR_LEN_MAX                  36 //for cr223525
#define MMIMMS_PAGE_BAR_BIN_LEN                  15
#define MMIMMS_PAGE_BAR_SLIDE_LEN                34
#define MMIMMS_PAGE_BAR_SIZE_LEN                 25
#define MMIMMS_PAGE_BAR_ATTACH_MARGIN_HEIGHT     2
#define MMIMMS_PAGE_BAR_ATTACH_MARGIN_LEFT       4        
#ifdef MMI_PDA_SUPPORT
#define MMIMMS_BUTTON_FORM_HEIGHT    (48)
#endif
/**---------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                 *
**---------------------------------------------------------------------------*/

//mms window
typedef enum
{
    EDIT_OPTION_WIN_ENTRY,        //彩信编辑窗口
    DOWNLOAD_WIN_ENTRY,           //彩信下载窗口
    SENDING_WIN_ENTRY,            //彩信发送等待窗口
    EDIT_PREVIEW_WIN_ENTRY,       //彩信预览窗口
    SETTING_WIN_ENTRY,            //设置窗口
    SENDING_RPT_WAIT_WIN_ENTRY,   //发送阅读报告等待窗口
    MMS_ENTRY_WIN_MAX
}MMS_ENTRY_WIN_E;                 //入口窗口

typedef struct tag_mms_recepients
{
    struct tag_mms_recepients     *next_ptr;    
    char           address[MAX_NAME_LENGTH];   //recepient address
}MMS_RECEPIENTS, *P_MMS_RECEPIENTS;

//彩信发送接收信息参数的全局变量
typedef struct 
{
    BOOLEAN is_downloading_mms;
    BOOLEAN is_sending_mms;
    BOOLEAN is_mms_retry;
    BOOLEAN is_stop_download_for_fly_mode;
    MMIMMS_PROGRESS_T * progress_ptr; 
    MMIMMS_MSG_RECV_END_T * recv_ptr;
    MMIMMS_MSG_SEND_END_T * send_ptr;
    MMIMMS_MSG_RECEIVED_T *recv_mms_info_ptr;
    uint32 mms_noti_op_record_id;     //当前阅读操作的彩信的id，包括正在下载，下载等待和未下载彩信通知，和正在阅读的彩信
    uint32 new_mms_record_id;         //当前提示新彩信的该条彩信的record id，使用该id来阅读收到的最新的一条彩信
    uint32 now_downloading_record_id; //正在下载的彩信的record id
}MMIMMS_SEND_RECV_GLOBLEE_T;

//mms link set index
typedef struct 
{
    uint16 mmimms_sim1_default_index;    
    uint16 mmimms_sim2_default_index;   
}MMIMMS_LINKLIST_DEFAULT_INDEX_T;

//mms is play sound
typedef enum
{
    MMS_MSG_SETTINGS_OFF,
    MMS_MSG_SETTINGS_ON,
    NUM_MMS_MSG_SETTINGS_ONOFF
}MESSAGES_MMS_MSG_SETTINGS_ONOFF;

//新彩信还是已有彩信
typedef enum
{
    SEND_NEW_MMS,
    SEND_OUTBOX_MMS,
    SEND_MMS_STATUS_MAX
}MMIMMS_SEND_MMS_STATUS;

//播放类型是阅览还是播放，主要确定是否有opt，如果阅览就没有opt
typedef enum 
{
    MMS_NONE,
    MMS_PLAY,
    MMS_PREVIEW,
    MMS_CONTENT_DISPLAY
}MMS_DISPLAY_CONTENT; 

//richtext items
typedef enum
{
    MMIMMS_RICHTEXT_TO,                   //0
    MMIMMS_RICHTEXT_SUBJECT,              //1
    MMIMMS_RICHTEXT_ADDFILE,              //2
    MMIMMS_RICHTEXT_TEXT,                 //3
    MMIMMS_RICHTEXT_PIC_OR_MOVIE,         //4
    MMIMMS_RICHTEXT_MUSIC,                //5
    MMIMMS_RICHTEXT_ADDMENU,              //6
    MMIMMS_RICHTEXT_MAX
}MMIMMS_RICHTEXT_ITEM_INDEX_E; 


//彩信类型
typedef enum
{
    MMIMMS_TYPE_MMS,
    MMIMMS_TYPE_MMS_PUSH,
    MMIMMS_TYPE_WAP_PUSH,
    MMIMMS_TYPE_OTA,
    MMIMMS_TYPE_MAX
} MMIMMS_MSG_TYPE_E;

typedef enum
{
#ifdef MMI_MMS_MINI_SUPPORT
    MMIMMS_ATTACHMENT_TYPE_OHTER,
#endif
    MMIMMS_ATTACHMENT_TYPE_VCARD,
#ifndef MMI_MMS_MINI_SUPPORT        
    MMIMMS_ATTACHMENT_TYPE_OHTER,
#endif    
    MMIMMS_ATTACHMENT_TYPE_CALENDAR,
    MMIMMS_ATTACHMENT_TYPE_MAX
} MMIMMS_ATTACHMENT_TYPE_E;

typedef enum
{
    MMIMMS_DETAIL_OPT_NULL,
    MMIMMS_DETAIL_OPT_DOWNLOAD,
    MMIMMS_DETAIL_OPT_UNDOWNLOAD,
    MMIMMS_DETAIL_OPT_DEL,
    MMIMMS_DETAIL_OPT_ANSWER,
    MMIMMS_DETAIL_OPT_ANSWER_SMS,
    MMIMMS_DETAIL_OPT_ANSWER_MMS,
    MMIMMS_DETAIL_OPT_CALL,
    MMIMMS_DETAIL_OPT_NOMAL_CALL,
    MMIMMS_DETAIL_OPT_IP_CALL,
    MMIMMS_DETAIL_OPT_VIDEO_CALL,
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    MMIMMS_DETAIL_OPT_MOVE_ITEM,
    MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM,
    MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM,
#endif
    MMIMMS_DETAIL_OPT_ADD2PB,
    MMIMMS_DETAIL_OPT_ADD2PB_NEW_ITEM,
    MMIMMS_DETAIL_OPT_ADD2PB_EXIST_ITEM,
    MMIMMS_DETAIL_OPT_MAX
}MMIMMS_DETAIL_OPT_ITEM;//彩信通知阅读界面opt menu id

//彩信编辑和阅读的时候字体大小
typedef enum
{    
    MMIMMS_CHARACTER_FONT_BIG,    //大
    MMIMMS_CHARACTER_FONT_NOMAL,  //普通大小
    MMIMMS_CHARACTER_FONT_SMALL,  //小
    MMIMMS_CHARACTER_FONT__MAX
}MMIMMS_CHARACTER_FONT_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : alert window when edit new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_EditMMSAlertWin(MMIMMS_ERROR_E op);

/*****************************************************************************/
// 	Description: 彩信窗口初始化函数，里面包括mms模块内存的初始化
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
void MMIMMS_WinInit(MMS_ENTRY_WIN_E mms_entry_e);

/*****************************************************************************/
// 	Description: 彩信窗口退出函数，里面包括mms模块内存的销毁化
//	Global resource dependence:
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
void MMIMMS_WinExit(MMS_ENTRY_WIN_E mms_entry_e);

/*****************************************************************************/
//  Description :create edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateEditWinExt(BOOLEAN is_permitted, DPARAM param_ptr);

/*****************************************************************************/
//  Description :create edit win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CreateEditWin(void);



/*****************************************************************************/
//  Description :create mms setting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSSettingWin(void);

/*****************************************************************************/
//  Description :add list item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_AppendOneLineTextListItem(MMI_CTRL_ID_T       ctrl_id,
                                      GUIITEM_STYLE_E    item_style,
                                      MMI_TEXT_ID_T       left_softkey_id,
                                      MMI_IMAGE_ID_T      icon_id,
                                      wchar*        wstr_ptr,
                                      uint16              wstr_len,
                                      uint16              pos,
                                      BOOLEAN             is_update,
                                      uint32 user_data
                                      );

/*****************************************************************************/
//  Description :create mms download waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMSMMI_CreateDownloadWaitingWin(MN_DUAL_SYS_E dual_sys, uint32 record_id);

/*****************************************************************************/
//  Description : recieve noti and download mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_RecvAndDownloadMMS(void);

/*****************************************************************************/
//  Description : mms read detail and download mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_DetailAndDownloadMMS(MN_DUAL_SYS_E   dual_sys, uint32 record_id);

/*****************************************************************************/
//  Description :create mms sending waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMSMMI_CreateSendingWaitingWin(MN_DUAL_SYS_E dual_sys, uint32 record_id);
/*****************************************************************************/
//  Description : for the case if close ps or switch on flight mode while sending win open
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_DestroyOperateWaitingWin(void);

/*****************************************************************************/
//  Description :create subject edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSubjectEditWin(void);

/*****************************************************************************/
//  Description :create slide time duration edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSlideDurationEditWin(void);

/*****************************************************************************/
//  Description :create to list edit win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateToListEditWin(void);

/*****************************************************************************/
//  Description :draw the mms edit page bar
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_DrawPageBarText(MMIMMS_WINDOW_ID_E win_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, BOOLEAN is_play_mms);

/*****************************************************************************/
//  Description : set current edit document 
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_MMSEditInit(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr, MMIMMS_RICHTEXT_ITEM_INDEX_E focus_index);

/*****************************************************************************/
//  Description :to make sure mms not in 1 can insert to edittext
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SetEditTextContent(wchar* str_phiz, uint16 str_len);

/*****************************************************************************/
//  Description :create mms preview win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSPreviewWin(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, MMS_DISPLAY_CONTENT play_or_pre);

/*****************************************************************************/
//  Description :out box send old mms  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_OutboxSendMMS(uint32 record_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :create sim card select win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateSimCardSelectWin(uint32 record_id);

/*****************************************************************************/
//  Description :create mms detail read win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSDetailShowWin(uint32 record_id);

/*****************************************************************************/
// 	Description: deal with the control msg and dispatch the msg
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMMS_HandleControlMsg(
                                    uint16   	    msg_id,
                                    DPARAM        param
                                    );

/*****************************************************************************/
//  Description :create mms send rpt waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSRPTSendWaitingWin(MMIMMS_EDIT_DOCUMENT_T *    editdoc_ptr);

/*****************************************************************************/
//  Description : play mms sound
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_PlayMMSSound(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :set if it is notifycation detail 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetPreviewMMSDetailFlag(BOOLEAN is_mms_noti_detail);

/*****************************************************************************/
//  Description :get if it is notifycation detail 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPreviewMMSDetailFlag(void);

/*****************************************************************************/
//  Description :reading edit mms document is send rpt
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsSendRPT(BOOLEAN is_send_rpt);

/*****************************************************************************/
//  Description :reading edit mms document is send rpt get
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_GetIsSendRPT(void);

/*****************************************************************************/
//  Description : set mms send status  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSSendStatus(MMIMMS_SEND_MMS_STATUS send_status);

/*****************************************************************************/
//  Description : get mms send status  
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_SEND_MMS_STATUS MMIMMS_GetMMSSendStatus(void);


#ifndef MMI_GUI_STYLE_TYPICAL

PUBLIC void MMIMMS_CreateMMSSettingsChildWin(MMI_WIN_ID_T	parent_win_id);

PUBLIC void  MMIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUILIST_ITEM_DATA_T *item_data,
                                      uint16 *index
                                      );
#else
/*****************************************************************************/
//  Description : set mms memory info to richtext item
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUIRICHTEXT_ITEM_T *item_data,
                                      uint16 *index
                                      );
#endif
/*****************************************************************************/
//  Description : alert window when receive new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_RecvMMSAlertWin(MMIMMS_PUSH_RECV_E mms_error_e);

/*****************************************************************************/
//  Description :get reading edit mms document 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T *  GetMMSReadEditDoc(void);

/*****************************************************************************/
//  Description : mms edit exit
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_MMSEditExit(void);

/*****************************************************************************/
//  Description : reading edit mms preview document init
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_MMSPreviewInit(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, uint8 play_or_preview);

/*****************************************************************************/
//  Description : reading edit mms preview document exit
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_MMSPreviewExit(void);

/*****************************************************************************/
//  Description :mms  detail sender
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSDetailSenderString(MMI_STRING_T *str_dst, uint8 *sender_ptr,  MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description :mms  detail time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSDetailTimeString(MMI_STRING_T *str_dst, uint32 time, BOOLEAN is_send_time);

/*****************************************************************************/
//  Description : set mms enter reason  [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_REASON_TYPE_E edit_type);

/*****************************************************************************/
//  Description : get mms enter reason   [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_REASON_TYPE_E MMIMMS_GetMMSEditEnterReason(void);

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_HandleTimer(DPARAM param);

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_HandleNetTimer(uint8 timer_id, uint32 param);


/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_IsDownloadingMMS(void);

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_IsSendingMMS(void);

/*****************************************************************************/
//  Description : set now operation  read record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetNowReadMMSId(uint32 record_id);

/*****************************************************************************/
//  Description : get now operation  read record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32  MMIMMS_GetNowReadMMSId(void);

/*****************************************************************************/
//  Description : is sim card ok
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSimCardOK(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description :set rocord id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_SetSendNewMMSId(uint32 record_id);

/*****************************************************************************/
//  Description : set now operation  mms retry send or receive
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsMmsRetry(BOOLEAN is_retry);

/*****************************************************************************/
//  Description : get now operation  mms retry send or receive
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsMmsRetry(void);

/*****************************************************************************/
//  Description :get preview is play video 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayVideo(void);

/*****************************************************************************/
//  Description :get preview is play music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayMusic(void);

/*****************************************************************************/
//  Description : set is stop mms downloading for fly mode open
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsStopDownloadForFlyMode(BOOLEAN is_open);
/*****************************************************************************/
//  Description : get is stop mms downloading for fly mode open
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsStopDownloadForFlyMode(void);

/*****************************************************************************/
//  Description :get default pic width and height
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPicDefaultWidthAndHeight(uint16 *width_ptr, uint16 *height_ptr);

/*****************************************************************************/
//  Description :get default music width and height
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMusicDefaultWidthAndHeight(uint16 *width_ptr, uint16 *height_ptr);

/*****************************************************************************/
//  Description :get rechtext rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetRichtextRect(GUI_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description :get pagebar left bin rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPagebarLbinRect(GUI_RECT_T *rect_ptr);

/*****************************************************************************/
//  Description :get pagebar right bin rect
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPagebarRbinRect(GUI_RECT_T *rect_ptr);


/*****************************************************************************/
//  Description : change edit document attachment text code type
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:编辑草稿箱彩信，收件箱和已发信箱转发，发件箱编辑发送需要调用本
//       函数来改变text类型的附件的编码类型，把uniconde编码变成utf8编码
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_AttachmentTextEncodeType(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr);

/*****************************************************************************/
//  Description : 下载链表是否为空
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsSendRecvListEmpty(void);

/*****************************************************************************/
//  Description :open mms alert win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSMMI_OpenMMSAlertWin(
                                   MMI_TEXT_ID_T     text1_id,           //text 1,no default TXT_NULL
                                   MMI_TEXT_ID_T     text2_id,           //text 2,no default TXT_NULL
                                   MMI_IMAGE_ID_T    image_id            //alert image id
                                   );

/*****************************************************************************/
//  Description :Set answer sim card
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMS_SetAnswerSim(MN_DUAL_SYS_E answer_sim);

/*****************************************************************************/
//  Description :set mms name list by to string /for draft box and sendfail box
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSNameListByTo(MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr);

/*****************************************************************************/
//  Description : is pdp permit using,G ner and in phone can not use pdp
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : get now operation download record id
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetNowDownloadMMSId(void);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description :if the file is drm file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetDrmFileTypeFromName(uint8 * file_name_ptr);

/*****************************************************************************/
//  Description :if the mms can farward
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMMSForbidFarward(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description : edit new mms 
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_InitData(void);    

/*****************************************************************************/
//  Description :add edit entry init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_SlideInit(void);

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetIsMMSMode( BOOLEAN is_mms_mode);

/*****************************************************************************/
//  Description :设置是否有主题
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditIsSubjectExist( BOOLEAN is_have_subject);

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetIsMMSMode(void);

/*****************************************************************************/
//  Description :设置是否有email地址
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSEditIsEmailAddrExist(BOOLEAN is_have_email_addr);

/*****************************************************************************/
//  Description :判断是否有email地址
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetMMSEditIseEmailAddrExist(void);

#endif
/*****************************************************************************/
//  Description :关闭编辑窗口
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_CloseEditWin(void);

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsAllowSendMMSByFDN(MN_DUAL_SYS_E dual_sys, MMI_STRING_T edit_number);

/*****************************************************************************/
//  Description : is MMS number Valid
//  Global resource dependence : 
//  Author:minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsMMSNumberValid(MMI_STRING_T edit_number);

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetWinStatusBarVisible(MMI_WIN_ID_T win_id, BOOLEAN is_visible);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_Interupt(void);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set if the content saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDRM_SetSaved(wchar* w_saved_fname, BOOLEAN isSaved);
#endif
/*****************************************************************************/
//  Description : get file type
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIFMM_FILE_TYPE_E MMIMMS_GetFileType(
                                             const wchar  *full_path_ptr, //in
                                             uint16       full_path_len   //in
                                             );
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_EXPORT_H*/
