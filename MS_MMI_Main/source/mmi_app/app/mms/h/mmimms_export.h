/****************************************************************************
** File Name:      mmimms_export.h                                         *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMIMMS_EXPORT_H
#define _MMIMMS_EXPORT_H

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mn_type.h"
#include "mmisms_export.h"
#include "guirichtext.h"
#include "block_mem.h"
#include "mmi_custom_define.h"
#include "mmifmm_export.h"

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

#define MMIMMS_DEFAULT_DIR_LEN     3 
#define MMIMMS_DEFAULT_DIR         (g_mmimms_dir_wstr)

//#define MMIMMS_MAX_ONE_MMS_SIZE     (100 * 1024)
#define MMIMMS_RESERVE_HEAD_SIZE    (1*1024)                       //预留彩信头的大小
#define MMIMMS_RESERVE_LIST_SIZE    (2*1024)                       //预留保存LIST的空间
#define MMIMMS_BLOCK_BUFFER_SIZE    (2*MMIMMS_MAX_ONE_MMS_SIZE)
#define MMIMMS_MAX_CONTENT_SIZE     (MMIMMS_MMS_LIMIT_SIZE-MMIMMS_RESERVE_HEAD_SIZE)
#define MMIMMS_MIN_RESERVE_SPACE    (MMIMMS_MMS_LIMIT_SIZE + 10*1024)

#define MMIMMS_SUFFIX_LEN           12
#define MAX_MULTIFILE_NAME_LENGTH   (MMIFMM_FILE_FILE_NAME_MAX_LEN/3)
#define MMIMMS_SIZE_STRING_MAX_LEN  6  //彩信大小字符串的最大长度，以k为单位,精确到0.1，最大300k

#define MMIMMS_MMS_LIMIT_SIZE MMIAPIMMS_GetMMSLimitSize()        //mms limit size
#define MMIMMS_LIMIT_SIZE_TXT    (50*1024)

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMMS_NO_ERROR = 0,     
    MMIMMS_NO_OPERATION,
    MMIMMS_ERROR_FATAL,
    MMIMMS_ERROR_FAIL,
    MMIMMS_ERROR_NOT_INIT,
    MMIMMS_ERROR_UDISK_USING, 
    MMIMMS_ERROR_CC_IN_CALL,
    MMIMMS_ERROR_CMMB_RUNNING,
    MMIMMS_ERROR_EDITING,
    MMIMMS_ERROR_SENDRECVING,
    MMIMMS_ERROR_NO_CONTENT, 
    MMIMMS_ERROR_NO_MEMORY, 
    MMIMMS_ERROR_NO_SPACE,
    MMIMMS_ERROR_FILENAME,
    MMIMMS_ERROR_INVALID_DATA,
    MMIMMS_ERROR_INVALID_SIZE,
    MMIMMS_ERROR_CONFLICT,    
    MMIMMS_ERROR_MAX_SLIDE,
    MMIMMS_ERROR_NOT_SUPPORT,
    MMIMMS_ERROR_SFS_ERROR, 
    MMIMMS_ERROR_SIM_ERROR,     
    MMIMMS_ERROR_ENCODE_ERROR,     
    MMIMMS_ERROR_SETTING_ERROR,
    MMIMMS_ERROR_INVALID_URL,     
    MMIMMS_ERROR_PUSH_NOT_EXIST,    
    MMIMMS_ERROR_CANCEL, 
    MMIMMS_ERROR_NETLINK_FAILED,   
    MMIMMS_ERROR_NETLINK_FDN_ONLY,   
    MMIMMS_ERROR_NETLINK_DISCONNECT,  
    MMIMMS_ERROR_GATEWAY_CONNECT_FAILED, 
    MMIMMS_ERROR_NETCONNECT_FAILED,   
    MMIMMS_ERROR_INVALIDPERIOD,   
    MMIMMS_ERROR_SERVICE_DENIED,  
    MMIMMS_ERROR_FORMAT_CORRUPT,   
    MMIMMS_ERROR_ADDRESS_UNRESOLVED,   
    MMIMMS_ERROR_MESSAGE_NOT_FOUND, 
    MMIMMS_ERROR_SERVER_BUSY,
    MMIMMS_ERROR_NOT_ACCEPTED,
    MMIMMS_ERROR_UNSUPORTED_MESSAGE, 
    MMIMMS_ERROR_RECV_NEED_AUTH,
    MMIMMS_ERROR_RECV_ERROR_DATA,
    MMIMMS_ERROR_IN_VT_CALL,
    MMIMMS_ERROR_FDN_NUMBER,
    MMIMMS_ERROR_INVALID_ADDRESS,
#ifdef DATA_ROAMING_SUPPORT
    MMIMMS_ERROR_DATA_ROAMING_DISABLE,  //added by feng.xiao
#endif
    MMIMMS_ERROR_MAX  
}MMIMMS_ERROR_E;

typedef enum _MMS_FOLDER_TYPE
{
    MMIMMS_FOLDER_NULL,
    MMIMMS_FOLDER_INBOX,    
    MMIMMS_FOLDER_SENTBOX,
    MMIMMS_FOLDER_OUTBOX,
    MMIMMS_FOLDER_DRAFTBOX,
    MMIMMS_FOLDER_MAX
}MMIMMS_FOLDER_TYPE_E;

typedef enum
{
    MMIMMS_OP_FOWARD=1,
    MMIMMS_OP_EDIT,
    MMIMMS_OP_SEND,
    MMIMMS_OP_ANSWER,
    MMIMMS_OP_DEL,
    MMIMMS_OP_PREVIEW
}MMIMMS_OPERATION_TYPE_E;


//彩信编辑入口类型
typedef enum
{
    MMIMMS_EDIT_FROM_WRITE_NEW,                   //写新彩信进入彩信编辑
    MMIMMS_EDIT_FROM_INBOX_READSMS_TO_FARWARD,    //阅读短消息转发彩信彩信编辑
    MMIMMS_EDIT_FROM_CL_SEND,                     //CL模块发送彩信
    MMIMMS_EDIT_FROM_PB_SEND,                     //PB模块发送彩信
    MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER,           //进入收件箱列表回复彩信
    MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER,        //收件箱阅读彩信回复彩信
    MMIMMS_EDIT_FROM_INBOX_READ_NUMBER_SEND,      //彩信号码发送彩信
    MMIMMS_EDIT_FROM_OUTBOX_READ_TO_EDIT,         //发件箱阅读彩信编辑彩信
    MMIMMS_EDIT_FROM_DRAFTBOX_EDIT,               //草稿箱编辑彩信
    MMIMMS_EDIT_FROM_SENDBOX_LIST_FARWARD,        //已发信箱列表转发彩信
    MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD,       //收件箱阅读彩信转发彩信
    MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD,     //已发信箱阅读彩信转发彩信
    MMIMMS_EDIT_FROM_IMAGE_SEND,                  //FMM,DC,PICVIEW模块图片发送彩信
    MMIMMS_EDIT_FROM_MUSIC_SEND,                  //MP3,RECORD,FMM模块音乐发送彩信
    MMIMMS_EDIT_FROM_VIDEO_SEND,                  //FMM,DC视频发送彩信
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    MMIMMS_EDIT_FROM_DRAFTBOX_EDIT_SMS,//草稿编辑短信
    #endif
    MMIMMS_EDIT_FROM_MAX                          
} MMIMMS_EDIT_REASON_TYPE_E;

typedef struct MMIMMS_NV_SETTING_T_tag
{
    BOOLEAN     is_need_delivery_rpt_recv;       //接收选项的送达报告
    BOOLEAN     is_need_delivery_rpt_send;       //发送选项的送达报告
    BOOLEAN     is_need_read_rpt_send;
    BOOLEAN 	is_anonymous;
    BOOLEAN     is_play_sound;
    BOOLEAN     is_advertisingallowable; 
    BOOLEAN     is_extract_content;
    uint8       priority;
    uint8       retrieve_mode;
#ifdef MMIMMS_SET_ROAMING_DOWNLOAD    
    uint8       roaming_retrieve_mode;
#endif
#ifdef DATA_ROAMING_SUPPORT
    uint8       retrieve_mode_roam;             //漫游接收方式(added by feng.xiao)
#endif
    uint8       valid_period;
    uint8       sim_index[MMI_DUAL_SYS_MAX];   //MMS设置的索引
    uint8       is_auto_sign;                   //目前尚无自动签名功能
    MMIFILE_DEVICE_E device_type;                //优先存储位置
}MMIMMS_NV_SETTING_T;

typedef void (* MMSCONTROL_CALLBACK) (uint16 msg_id,DPARAM param);

extern const wchar g_mmimms_dir_wstr[];
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init mms
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIAPIMMS_AppInit(void);

/*****************************************************************************/
//  Description : init mms list
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void	MMIAPIMMS_ListInit(void);

/*****************************************************************************/
//  Description : Reset the mms setting to factory setting 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ResetMMSFactorySetting(void);

/*****************************************************************************/
//  Description : MMS is sending or receiving 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsSendingOrRecving(void);

/*****************************************************************************/
//  Description : Exit mms module 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ExitMMS (void);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : mms is playing ring  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsAudioInUse(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : ansmer mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_AnswerMMS(
                                   MN_DUAL_SYS_E        dual_sys,
                                   const MMI_STRING_T   *subject_ptr,
                                   const MMI_STRING_T   *string_to_ptr
                                   );

/*****************************************************************************/
//  Description : delete user info/ delete all mms file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DelUserInfo(void);

/*****************************************************************************/
//  Description : set mms lock flag
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetLockFlag(uint32 record_id,BOOLEAN is_locked);

/*****************************************************************************/
//  Description : set mms list
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SaveListInfo(void);

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : set mms memory info to richtext item
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPIMMS_SetMMSMemState(
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
PUBLIC void  MMIAPIMMS_SetMMSMemState(
                                      MMI_CTRL_ID_T ctrl_id,
                                      GUIRICHTEXT_ITEM_T *item_data,
                                      uint16 *index
                                      );
#endif
/*****************************************************************************/
//  Description : edit new mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditNewMMS(void);

/*****************************************************************************/
//  Description : open mms setting win
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_OpenMMSSettingWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsBlockConflict(BLOCK_MEM_ID_E id);

/*****************************************************************************/
//  Description : edit mms from file
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_EditMMSFromFile(
                                      MMIFILE_FILE_INFO_T   *file_info_ptr,
                                      MMIFMM_FILE_TYPE_E   file_type
                                      );
/*****************************************************************************/
//  Description :to make sure mms not in 1 can insert to edittext
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SetEditTextContent(wchar* str_phiz, uint16 str_len);

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : start auto download
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_CheckAutoDownload(void);

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_HandleTimer(DPARAM param);
#endif /* Reduce Code size */
/*****************************************************************************/
//  Description : get mms enter reason   [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_REASON_TYPE_E MMIAPIMMS_GetMMSEditEnterReason(void);

/*****************************************************************************/
//  Description : set mms enter reason  [minghumao记得整理]
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_REASON_TYPE_E edit_type);

/*****************************************************************************/
//  Description : mms is full or not
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsMMSFull(void);

/*****************************************************************************/
//  Description : delete one mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_DeleteMMS(uint32 record_id,
                             MMI_WIN_ID_T win_id
                             );

/*****************************************************************************/
//  Description : farward mms 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_FarwardMMS(int32      record_id);

/*****************************************************************************/
//  Description : draft box edit mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_DraftEditMMS(uint32 record_id);

/*****************************************************************************/
//  Description : inbox, sendfailbox, sendsuccbox read mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_ReadMMS(int32      record_id);

/*****************************************************************************/
//  Description : out box send mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_OutboxSendMMS(int32      record_id);

/*****************************************************************************/
//  Description : Get mms subject by record id
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetSubjectById(uint32 record_id, MMI_STRING_T *subject_str);

/*****************************************************************************/
//  Description :mms  detail sender
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DetailSenderString(MMI_STRING_T *str_dst, uint8 *sender_ptr,  MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description :mms  detail time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DetailTimeString(MMI_STRING_T *str_dst, uint32 time);

/*****************************************************************************/
//  Description : handle play ring timer
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_HandleNetTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : play mms sound
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_PlayMMSSound(MN_DUAL_SYS_E dual_sys);

PUBLIC void MMIAPIMMS_CloseNewMMSPromptWin(void);
/*****************************************************************************/
//  Description : brwoser send url by mms 
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_BrwSendUrlByMMS(const MMI_STRING_T   *string_url_ptr);

/*****************************************************************************/
//  Description : if mms in edit
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsEditMMS(void);

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsDownloadingMMS(void);

/*****************************************************************************/
//  Description : if it is downloading mms
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsSendingMMS(void);

/*****************************************************************************/
//  Description : if record id is downloading 
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsProcessMMSById(uint32 record_id);

/*****************************************************************************/
//  Description : if record id is in download queue and wait for downloading
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSInDownloadQueue(uint32 record_id);

/*****************************************************************************/
//  Description : edit mms from content
//  Global resource dependence : none
//  Author:aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_EditMMSFromContent(
                                         const MMI_STRING_T     *subject_ptr,       //主题
                                         const MMI_STRING_T     *content_info_ptr,      //文字内容
                                         MMIFILE_FILE_INFO_T    *file_info_ptr,     //媒体文件
                                         MMIFMM_FILE_TYPE_E     file_type
                                         );

#if 0 /* Reduce Code size */
/*****************************************************************************/
//  Description : mms is playing video  
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsVideoInUse(void);
#endif /* Reduce Code size */

/*****************************************************************************/
//  Description : 重新启动自动下载链表下载彩信，在飞行模式关闭后调用
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_RestartAutodownMMS(void);

/*****************************************************************************/
//  Description :清空自动下载链表下载彩信，在飞行模式开始后调用，包括停止正在下载的彩信
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_StopAutodownMMS(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ClearRecvSendModule(void);

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//  Description : set mms lock flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityMMS(uint32 record_id,
                                      BOOLEAN is_to_security);
#endif

/*****************************************************************************/
//  Description : 获得用户设定的单页彩信播放时间
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetSlideTimes(void);


/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:louis.wei
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPIMMS_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             );

/*****************************************************************************/
//  Description : virtual win active recieve and send mms
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_ActiveRecvSendModule(void);

/*****************************************************************************/
//  Description : 下载链表是否还有彩信在等待下载
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMMS_IsMMSWaitingDownload(void);

/*****************************************************************************/
//  Description : 正在下载彩信的folder type
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_FOLDER_TYPE_E MMIAPIMMS_GetDownloadingMMSFolderType(void);

/*****************************************************************************/
//  Description : Set mms net setting index  
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_Setting_SetNetSettingIndex(
                                                 MN_DUAL_SYS_E dual_sys,
                                                 uint8         index
                                                 );

/*****************************************************************************/
//  Description : 获取彩信总条数
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetMMSTotalNumber(void);

/*****************************************************************************/
//  Description : 获取彩信总大小
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetTotalSizeAndAllSize(uint32 *total_size_ptr, uint32 *all_space_ptr);

/*****************************************************************************/
//  Description : is in mms moudle window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsInMMSWin(void);

/*****************************************************************************/
//  Description : is pdp permit using,G ner and in phone can not use pdp
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_IsPdpPermitUsing(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_SMS_CHAT_SUPPORT
/*****************************************************************************/
//  Description : api for sms module
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC MMISMS_ORDER_INFO_T *MMIAPIMMS_GetListItemByID(int32 record_id);
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
PUBLIC void MMIAPIMMS_CreateMMSSettingsChildWin(MMI_WIN_ID_T	parent_win_id);
#endif

#ifdef MMIMMS_SMS_IN_1_SUPPORT
/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetIsMMSMode( BOOLEAN is_mms_mode);

/*****************************************************************************/
//  Description :判断是否是彩信界面的操作
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_GetIsMMSMode( void);

/*****************************************************************************/
//  Description : create mms win
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSubject(MMI_STRING_T *mms_content_string,MMI_STRING_T   *string_to_ptr);

/*****************************************************************************/
//  Description :关闭编辑窗口
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CloseEditWin( void);

/*****************************************************************************/
//  Description : create mms win
//  Global resource dependence : 
//  Author: rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_CreateMMSEditWin(void);
/*****************************************************************************/
//  Description : edit mms add mms attach
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSAttach(
                                      MMIFILE_FILE_INFO_T   *file_info_ptr,
                                      MMIFMM_FILE_TYPE_E   file_type,
                                      uint8   *file_data_ptr,
                                      uint32 file_data_len,
                                      MMI_STRING_T *vacard_str,    
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr,
                                      BOOLEAN is_multi_file  //多媒体文件,music，video，pic
                                      );
/*****************************************************************************/
//  Description : edit mms add mms slide
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditAddMMSSlide(
                                      uint32 slide_direct,                                      
                                      MMI_STRING_T *mms_content_string,
                                      MMI_STRING_T   *string_to_ptr
                                      );     
/*****************************************************************************/
//  Description : is emali addr exist
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetIsEmailAddrExist(BOOLEAN is_have_email_addr);

#endif
/*****************************************************************************/
//  Description :change mms file size to size string
//  Global resource dependence : 
//  Author:jixin.xu
//  Note:   
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetMMSFileSizeStr(
                                uint32 total_size, //in
                                wchar *display_wstr_size //out
                                );

/*****************************************************************************/
//  Description : Get mms net setting index 
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIMMS_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMS_SetMMSLimitSize(uint32 mms_limit);
#endif

/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIMMS_GetMMSLimitSize(void);

/*****************************************************************************/
//  Description : alert window when edit new mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_EditMMSAlertWin(MMIMMS_ERROR_E mms_error_e);

/*****************************************************************************/
//  Description : 检测是否可以进入彩信编辑模块
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMIMMS_ERROR_E MMIAPIMMS_EnterEditCheck(void);

#ifdef MMISEARCH_SUPPORT_ONLYMMS
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_SearchMMSContent(uint32 record_id, wchar *search_str, uint16 search_len);
#endif

#ifdef MMI_MMS_LIST_DISPLAY_MULTI_PEOPLE
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/

PUBLIC uint16 MMIAPIMMS_GetSendNumberArray(uint32 record_id, wchar (*sender_array)[MMISMS_NUMBER_MAX_LEN + 1], uint16 array_size);
#endif

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_Interupt(void);

/*****************************************************************************/
//  Description : for the case if close ps or switch on flight mode while sending win open
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMMS_DestroyOperateWaitingWin(void);

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : set if the content saved
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMMSDRM_SetSaved(wchar* w_saved_fname, BOOLEAN isSaved);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMMS_EXPORT_H*/
