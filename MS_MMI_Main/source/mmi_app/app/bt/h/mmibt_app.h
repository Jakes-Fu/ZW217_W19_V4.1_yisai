/****************************************************************************
** File Name:      mmibt_app.h                                              *
** Author:         baokun.yin                                               *
** Date:           03/05/2009                                               *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file handle bt headset play music action proecess   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE             NAME             DESCRIPTI                             *
** 03/05/2009       baokun.yin       Create                                *
**                                                                         *
****************************************************************************/

#ifndef _MMIBT_APP_H_    
#define  _MMIBT_APP_H_   
//#ifdef WIN32
//#include "mmibt_win32_simu.h"
//#else 
#include "a2dp_api.h"
//#endif
#include "mmibt_export.h"
#include "mmisrv_handlelist.h"
#include "mmibt_nv.h"
#include "bt_abs.h"
#include "guilistbox.h"
/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define MMIBT_ACT_QUEUE_MAX_LEN  4
#define MMIBT_ANIM_INIT                 0
#define MMIBT_ANIM_STOP                 2
#if (defined MMIBT_FTS_SUPPORT) && (!defined PDA_UI_DROPDOWN_WIN)
#ifdef BT_HID_SUPPORT
#define MMIBT_MAIN_MEUN_NUM             9//HID will add one 
#else
#define MMIBT_MAIN_MEUN_NUM             8 
#endif
#elif (defined MMIBT_FTS_SUPPORT) || (!defined PDA_UI_DROPDOWN_WIN)
#ifdef BT_HID_SUPPORT
#define MMIBT_MAIN_MEUN_NUM             8//HID will add one 
#else
#define MMIBT_MAIN_MEUN_NUM             7 
#endif
#else
#define MMIBT_MAIN_MEUN_NUM             6
#endif
#define MMIBT_MAX_DEV_ITEMS				   20
#define MMIBT_FULL_PATH_LEN         (MMIFILE_FULL_PATH_MAX_LEN)
#define MMIBT_FTP_RW_MODE               "rw"
#define MMIBT_FTP_RO_MODE               "r"
#define SCI_FREEIF(X)       \
    do {                    \
        if (X != PNULL)     \
        {                   \
            SCI_FREE(X);    \
            X = PNULL;      \
        }                   \
    }                       \
    while(0)
#define  MMIBT_FILE_LENGTH_INFO             32          //file wstr_len info max wstr_len
#define MMIBT_FILE_MODE_MAX_LEN            4
#define MMIBT_SET_MAX_ITEM_NUM             2
#define  MMIBT_DEVICE_NAME_UCS2_MAX_LEN	   13
#define  MMIBT_DEVICE_NAME_ALPHA_MAX_LEN   20
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
	MMIBT_TRANSFER_END,
    MMIBT_TRANSFER_FREE,  //文件传输完毕，但还未断开连接
//added by tonny.chen for HL bt
#ifdef BT_HL_PATCH_SUPPORT
    MMIBT_TRANSFER_FTS_CONNECTED,
#endif
//end added 
    MMIBT_SENDING_FILE_FOREGROUND,
    MMIBT_SENDING_FILE_BACKGROUND,
    MMIBT_RECEIVEING_FILE_FOREGROUND,
    MMIBT_REVEIVEING_FILE_BACKGROUND,
}MMIBT_TRANSFER_STATUS_TYPE_E;

typedef enum
{  
	MMIBT_OPEN_DEVICE,	   //打开声音设备	
	MMIBT_CLOSE_DEVICE,	   //关闭声音设备
	MMIBT_RESUME_DEVICE,   //恢复声音设备
	MMIBT_PAUSE_DEVICE,	   //暂停声音设备
	MMIBT_FUNCTION_NUM
}MMIBT_FUNCTION_TYPE_E;

typedef enum
{
	MMIBT_HEADSET_NO_STATUS,		//初始状态
	MMIBT_HEADSET_STATUS_OPEN,	//调用device的play接口
	MMIBT_HEADSET_STATUS_RESUME,		//调用device的resume接口	
	MMIBT_HEADSET_STATUS_STOP,	//调用device的stop接口
	MMIBT_HEADSET_STATUS_PAUSE,	//调用device的pause接口
	MMIBT_HEADSET_MAX_STATUS_NUM
}MMIBT_HEADSET_STATUS_E;

typedef enum
{
    HEADSET_DISCONNECTED,
    HEADSET_CONNECTED,
    NOT_HEADSET, 
#ifdef MMI_PDA_SUPPORT
    PDA_PAIRED_OPT,    
#endif    
}MMIBT_DEVICEDETAILS_FUNC_TYPE_E;

typedef struct
{
	MMIBT_FUNCTION_TYPE_E action[MMIBT_ACT_QUEUE_MAX_LEN + 1];
    uint8               action_num;
}MMIA2DP_ACT_QUEUE_T;

//New UI
typedef enum
{
    MMIBT_OPC_NOT_CHANGE,//need not change 
    MMIBT_OPC_INIT,//no opertion
	MMIBT_OPC_AUTO_TEST,//no opertion
    MMIBT_OPC_MAIN,//Enter to BT Main menu
    MMIBT_OPC_OPP_SEND,//Send 
	MMIBT_OPC_FTP_SEND,//recieve files
    MMIBT_OPC_POWER_ONOFF,//open or close BT
    MMIBT_OPC_SEARCH,//search
	MMIBT_OPC_LIST_PAIRED,//list paired devices
    MMIBT_OPC_PAIR,//pair
	MMIBT_OPC_INPUT_PIN,//pair
	MMIBT_OPC_VISIBILITY,//visibility setting menu
    MMIBT_OPC_VISIBLE,//set to visible
    MMIBT_OPC_HIDE,//set to invisible
	MMIBT_OPC_SAVE_LOCATION,//save file location
	MMIBT_OPC_OPEN_HEADSET,//open bt headset
	MMIBT_OPC_CLOSE_HEADSET,//close bt headset
	MMIBT_OPC_RECIEVE,//recieve files  
    MMIBT_OPC_FTS_DLE,//recieve files 
	MMIBT_OPC_TRANSFER_AUDIO,//transfer audio to handfree or headset
	MMIBT_OPC_SHARED_FILE_SETTING,//setting shared file 
	MMIBT_OPC_SHARED_AUTHORITY,//set shared authority
	MMIBT_OPC_SHARED_DIRECTORY,//set shared directory
	MMIBT_OPC_DEVICE_RENAME,//Rename paired device
	MMIBT_OPC_DEVICE_DELETE,//Delete paired device
	MMIBT_OPC_DEVICE_DELETE_ALL,//delete all paired device
	MMIBT_OPC_DEVICE_DETAILS,//details
    MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT,
	MMIBT_OPC_HELP,//Help
#ifdef BT_DIALER_SUPPORT
    MMIBT_OPC_ABOUT,//About
    MMIBT_OPC_OPEN_HANDSET_GATEWAY,//open bt headset
	MMIBT_OPC_CLOSE_HANDSET_GATEWAY,//close bt headset
#endif
	MMIBT_FTS_CONNECT_REQ,
	MMIBT_OPS_CONNECT_REQ,
	MMIBT_HFG_AUDIO_CONNECT,
	MMIBT_OPC_NUMERIC_PASSKEY_CHECK    //pair
}MMIBT_OPERTATION_TYPE_E;

typedef struct 
{
	MMIBT_OPERTATION_TYPE_E	root_opertion;
	MMIBT_OPERTATION_TYPE_E	current_opertion;
} MMIBT_OPERATION_INFO_T;

typedef enum
{
	MMIBT_FILE_SERVER_SEND_CONNECT,//Send connecting
    MMIBT_FILE_SERVER_OPP_SEND,//OPP Send
    MMIBT_FILE_SERVER_OPP_RECIEVE,//OPP Recieve
    MMIBT_FILE_SERVER_FTP_SEND,//FTP Send
	MMIBT_FILE_SERVER_FTP_RECIEVE,//FTP Recieve
    MMIBT_FILE_SERVER_MAX
}MMIBT_FILE_SERVER_TYPE_E;

// @baokun fix 86558 begin
typedef enum
{
    MMIBT_OPP_SUSPEND,
    MMIBT_FTP_SUSPEND,
    MMIBT_PAIR_SUSPEND,
    MMIBT_INPUTPIN_SUSPEND,
    MMIBT_SEARCH_SUSPEND,
    MMIBT_A2DP_SUSPEND,
    MMIBT_SUSPEND_MAX
}MMIBT_SUSPEND_TYPE_E;
// @baokun fix 86558 end

//New UI
typedef struct 
{
	wchar						transfer_file_name[MMIFILE_FULL_PATH_MAX_LEN + 1];	//the name of transfer(send or recieve) file
    BOOLEAN                     is_temp_file;//is temp file, after send, it will be delete	
	uint32						transfered_size;								//transfer file size
	uint32						total_file_size;								//transfer file size
	MMIFILE_DEVICE_E            filedev;  
	BT_DEVICE_INFO              device_info;
	MMIBT_FILE_SERVER_TYPE_E	server_type;									//server type: OPP/FTP
} MMIBT_TRANSFER_FILE_INFO_T;

typedef enum
{
    MMIBT_SUCCESS,
    MMIBT_UDISK_RUNING,
    MMIBT_FOLDER_NOT_EXIST,
    MMIBT_UDISK_NOT_EXIST,
    MMIBT_SDCARD_NOT_EXIST,
    MMIBT_UDISK_SDCARD_NOT_EXIST,
	MMIBT_FORMAT_ERROR,
    MMIBT_FTP_BUSY,
    MMIBT_ERROR,
    MMIBT_SETTING_NUM
}MMIBT_SETTING_RET;

typedef BOOLEAN (*BTFUNCWINCALLBACK)(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len);

typedef struct BTDATA_SRV_HANDLE_TAG
{        
    MMI_HANDLE_T        win_handle;
    BTFUNCWINCALLBACK   call_back;
    void                *user_data;      
} BTDATA_SRV_HANDLE_T;

typedef struct BTDATA_SRV_HANDLE_TAG* BT_HANDLE_T;

typedef struct
{   
    MMI_SRVHANDLE_T     srvhandle_lst;
    BT_HANDLE_T         cur_handle; 
}DATASRV_BT_INSTANCE_T;


typedef BOOLEAN (*MMIBT_DeviceFunc)(uint32 sample_ret);

typedef struct 
{
	uint8    file_mode[MMIBT_FILE_MODE_MAX_LEN];
} MMIBT_SHAIRED_AUT_T;
typedef struct 
{
	wchar      shaired_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1];
} MMIBT_SHAIRED_DIR_T;
typedef union
{
	MMIFILE_DEVICE_E    filedev;  
	MMIBT_SHAIRED_DIR_T  dir;
	MMIBT_SHAIRED_AUT_T authority;          
}MMIBT_USER_SETTING_U;
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init a2dp action queue 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Init_ActionQueue(void);

/*****************************************************************************/
//  Description : Remove action  from queue 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_RemoveAction_FromQueue(MMIBT_FUNCTION_TYPE_E act);

/*****************************************************************************/
//  Description :  get action from queue head
//  Global resource dependence : none
//  Author: 
//  Note: return action in queue head, if queue empty, return BT_A2DP_NONE
/*****************************************************************************/
PUBLIC MMIBT_FUNCTION_TYPE_E MMIBT_Out_ActionQueue(void);

/*****************************************************************************/
//  Description :  a2dp action insert to action queue 
//  Global resource dependence : none
//  Author: 
//  Note: if queue full, return false, if act is stop, clean queue's actions and insert only this action
//        if act is pause, need clean the play and resume action
//        if action is play or resume, need clean the pause action
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_In_ActionQueue(MMIBT_FUNCTION_TYPE_E act);

/*****************************************************************************/
//  Description :  get is the action queue empty
//  Global resource dependence : none
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsActionQueueEmpty(void);

/*****************************************************************************/
//  Description :  process next action
//  Global resource dependence : none
//  Author: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HandleAction(MMIBT_FUNCTION_TYPE_E act);

/*****************************************************************************/
//  Description : Get application type.
//  Global resource dependence : none
//  Author: xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC  MMIBT_A2DP_APP_TYPE_E  MMIBT_GetA2dpAppType(
                                    void
                                    );

/*****************************************************************************/
//  Description : MMIBT_HandleAppOperation.
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_HandleAppOperation(MMIBT_A2DP_OPR_FUNC_TYPE_E opr_type);

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    );

/*****************************************************************************/
//  Description : MMIBT_SetAppInfo
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    );

/*****************************************************************************/
//  Description : pause device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_PauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );


/*****************************************************************************/
//  Description : open device 
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenDevice(
                    uint32 sample_ret
                    );

/*****************************************************************************/
//  Description : Resume  device 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIBT_ResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );


/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_StopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );


/*****************************************************************************/
//  Description : stop a2dp device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetAudioSampleRate(
                    void
                    );

/*****************************************************************************/
//  Description : get headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_STATUS_E MMIBT_GetDeviceStatus(void);

/*****************************************************************************/
//  Description : set headset status
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetDeviceStatus( MMIBT_HEADSET_STATUS_E audio_status);

/*****************************************************************************/
//  Description :  open HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGOpenDevice(uint32 sample_ret);

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGStopDevice( uint32 sample_ret);

/*****************************************************************************/
//  Description :  Resume HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGResumeDevice( uint32 sample_ret);

/*****************************************************************************/
//  Description :  stop HFG device
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HFGPauseDevice(void);

/*****************************************************************************/
//  Description : open device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_OpenDeviceCnf(BT_MSG_T *msg_body);

/*****************************************************************************/
//  Description :resume device confirm
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ResumeDeviceCnf(BT_MSG_T  *msg_body);

/*****************************************************************************/
//  Description : pause device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PauseDeviceCnf(BT_MSG_T *msg_body);

/*****************************************************************************/
//  Description : close device confirm
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_CloseDeviceCnf(BT_MSG_T     *msg_body);
/*****************************************************************************/
//  Description : get current a2dp device busy or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsBusy(void);

/*****************************************************************************/
//  Description : set device busy or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetBtDeviceBusy(BOOLEAN busy_flag);
#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendOneLineTextIconListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            MMI_IMAGE_ID_T   imgae_id,
                            uint16           pos,
                            MMI_TEXT_ID_T    left_text_id,
                            BOOLEAN          is_update
                            );
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendOneLineIconTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );
/*****************************************************************************/
//  Discription: append spliter
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendSplitListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id,
                            uint16           pos,
                            BOOLEAN          is_update
                            );
/*****************************************************************************/
//  Discription: append one line one item
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendTwoLineTextIconSmallTextListItem(	
                            MMI_CTRL_ID_T    ctrl_id,
                            MMI_TEXT_ID_T    text_id1,
                            MMI_IMAGE_ID_T   imgae_id,
                            MMI_STRING_T     str_info,
                            uint16           pos,
                            BOOLEAN          is_update
                            );

#endif 
/*****************************************************************************/
//  Discription: Create blue tooth edit device name win
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterEditWin(BT_DEVICE_INFO *device_info_ptr,BOOLEAN is_local_name);

/*****************************************************************************/
//  Discription: set the inquire info,call the function when begin searching device
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_SetCurrentSevice(uint32	service);
/*****************************************************************************/
//  Discription: set the inquire info,call the function when begin searching device
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetCurrentSevice(void);
/*****************************************************************************/
//  Discription: set bt opertor
//  Global resource dependence: None
//  Author: 
//  Parameter: root_opertion : it descript the reason enter to BT (Set, Send or other Opt), 
//                             the next step will be determined  by root_opertion
//             current_opertion: it  descript current BT operation, after recived BT callback 
//                               message, it will decide what's to do. 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetOpertor(
                                MMIBT_OPERTATION_TYPE_E root_opertion, //IN:
                                MMIBT_OPERTATION_TYPE_E current_opertion//IN:
                            );
/*****************************************************************************/
//  Discription: get bt opertor
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC MMIBT_OPERATION_INFO_T MMIBT_GetOpertor(void);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void   MMIBT_SetInterruptOper(uint32	interrupt_oper );
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIBT_OPERTATION_TYPE_E MMIBT_GetInterruptOper(void);
/*****************************************************************************/
//  Discription: Open the blue tooth device list  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterDeviceListWin(MMIBT_TRANSFER_FILE_INFO_T *file_info_ptr,
                                    MMIBT_OPERTATION_TYPE_E	root_opertion,
                                    uint32         service);
/*****************************************************************************/
//  Discription: Open the blue tooth device details  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterDeviceDetailsWin(BT_DEVICE_INFO *device_ptr,
                                                  MMI_STRING_T *string_ptr,
                                                  MMIBT_DEVICEDETAILS_FUNC_TYPE_E func_type);
/*****************************************************************************/
//  Discription: 根据current_opt的不同，提示不同的提示信息，让用户选择是否要进行该操作
//  Global resource dependence: None
//  Author: 
//  current_opt：  当前的操作类型，根据该值的不同，显示不同的提示信息；
//  parent_win_id：用户选择了"Yes",或者"No"，要把用户选择通过窗口的信息
//                 发送该parent_win_id指向的窗口，如果parent_win_id为0，
//                 表示需要在本窗口中处理
//  Note : 
//  Return: 
/*****************************************************************************/
PUBLIC void OpenQueryWin(   MMI_WIN_ID_T             win_id,
                           MMI_STRING_T*            display_str1,
                           MMI_STRING_T*            display_str2,
                           MMIBT_OPERTATION_TYPE_E  opc
                        );

#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Discription: check the type of new received file is drm 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CheckNewFileType( uint16*file_suffix_ptr);
#endif
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpsConnectReq(BT_MSG_T *msg_body_ptr);
/*****************************************************************************/
//  Discription: Open the blue tooth device details  win   
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterFileTransferWin(wchar	*file_name_ptr,
                                                MMIBT_FILE_SERVER_TYPE_E  transfer_type,
                                                uint32     file_size,
                                                uint32     total_file_size);

/*****************************************************************************/
//  Discription: Clear the client rect  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  MMIBT_ClrClientRect(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Discription: Update current file list
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateCurrentFileList(void);

/*****************************************************************************/
//  Discription: BtPauseBGPlay
//  Global resource dependence: none 
//  Author: 
//  Note: pause background play in BT by MMIBT_SUSPEND_TYPE_E
/*****************************************************************************/
PUBLIC void BtPauseBGPlay(MMIBT_SUSPEND_TYPE_E type);
/*****************************************************************************/
//  Discription: BtResumeBGPlay
//  Global resource dependence: none 
//  Author: 
//  Note: resume background play in BT by MMIBT_SUSPEND_TYPE_E
//        first set s_bt_suspend[type] = FALSE
//        if s_bt_suspend[] all is FLASE, this resume permit to do
/*****************************************************************************/
PUBLIC void BtResumeBGPlay(MMIBT_SUSPEND_TYPE_E type);
/*****************************************************************************/
//  Discription:get type is need BT On first
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: TRUE: need BT On first
//          FALSE:no need BT On first
/*****************************************************************************/
PUBLIC BOOLEAN IsNeedBTOn(MMIBT_OPERTATION_TYPE_E type);
/*****************************************************************************/
//  Discription: close the prompt win and open done msg box
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC  void  ClosePrmAndOpenDoneWin(
                                    uint32 win_id,
                                    uint32 txt_id,
                                    uint32 img_id,
                                    uint32 tm_dur
                                    );
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenSearchDeviceWaitWin(
                                   uint32 service_type
                                   );
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetSelfPair(BOOLEAN  is_self_pair);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC  BT_DEVICE_INFO MMIBT_GetPinRequestInfo(void);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterFileIncomingWin(MMIBT_TRANSFER_FILE_INFO_T *param_ptr);
/*****************************************************************************/
//  Discription: Open storage select win 
//  Global resource dependence: None
//  Author: yuantao.xu
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_EnterStorageSelectWin(void);
/*****************************************************************************/
//  Discription: Open storage select win 
//  Global resource dependence: None
//  Author: yuantao.xu
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_EnterTextBoxWin(BT_ADDRESS *device_addr, MMIBT_OPERTATION_TYPE_E operation_type);
/*****************************************************************************/
//  Discription: Open the bluetooth set visibility  win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterSetVisibilityWin(void);

#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: Open the bluetooth set visibility  win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_EnterSharedWin(void);
#endif

/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CreateUitestWin(void);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SetPinCancel(BOOLEAN  is_pin_cancel);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsPinCancel(void);
/*****************************************************************************/
//  Discription: open pin input win   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CreatePinInputWin(BT_DEVICE_INFO *param_ptr);
/*****************************************************************************/
//  Discription: mmibt search device   
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_StartOperationWin(MMI_HANDLE_T win_handle,
                                            MMIBT_OPERTATION_TYPE_E operation_type, BTFUNCWINCALLBACK callback);
/*****************************************************************************/
//  Discription: mmibt search device cnf  
//  Global resource dependence: none 
//  Author: yuantao.xu 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_StopOperationWin(MMIBT_OPERTATION_TYPE_E operation_type);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void CreatePubWinCallBack(MMI_HANDLE_T win_handle,const wchar *str_ptr,int str_len);
/*****************************************************************************/
//  Discription:  check is disk space enough
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DiskEnoughSpaceForOverWrite(wchar *file_name,
										  uint32 dest_size, 
										  MMIFILE_DEVICE_E devie);
/*****************************************************************************/
//  Discription: save ftp folder to nv
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_UpdateFtpSharedFolder(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: This function is used to get the connect status of current earphone
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsDeviceActivedEarphone(const BT_ADDRESS	*addr_ptr);
#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Discription: check the java directory whether exist
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_CheckJavaDirExist(void);
#endif
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_DIAL_NUMBER
//	make a call from HFAG
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagDiaNumber(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_REDIAL
//	make a call from HFAG in the call record
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagRedailCall(void);
/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_DIAL_MEMORY
//	make a call from HFAG in the call record
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_HfagDiaMemory(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: when get ID_STATUS_HFAG_AUDIO_CONNECTED
//	change audio device from vb to pcm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_AudioConnected(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Discription: when get ID_STATUS_HFAG_AUDIO_DISCONNECTED
//	change audio device from pcm to vb
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_AudioDisconnected(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Discription: when get : ID_STATUS_HFAG_HFU_STATUS_REQ
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetHfuStatus(void);
/*****************************************************************************/
//  Discription: Get if the device is visible:inquiry enable and page enable
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetIsDeviceVisible(void);
/*****************************************************************************/
// 	Description : intial bt module  
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void	MMIBT_GetFileUserSetting(BT_NV_ITEM_E type, MMIBT_USER_SETTING_U *setting_ptr);
/*****************************************************************************/
//  Discription: set save file location info 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_SaveUserSetting(BT_NV_ITEM_E type, MMIBT_USER_SETTING_U *setting_ptr);
/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_SetFtpRootFolder(MMIFILE_DEVICE_E storage);
/*****************************************************************************/
//  Discription: append item to list control(device name to list)  
//  Global resource dependence: none 
//  Author: Wenming feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendDeviceListItem(
                                       BT_DEVICE_INFO*  device,
									   GUIITEM_STYLE_E  item_style,
                                       MMI_TEXT_ID_T    left_softkey_id,
                                       MMI_TEXT_ID_T    middle_softkey_id,
                                       MMI_TEXT_ID_T    right_softkey_id,
                                       MMI_CTRL_ID_T    ctrl_id
                                       );
/*****************************************************************************/
//  Discription: append item to list control(device name to list) 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_AppendStringListItem(
                                      MMI_TEXT_ID_T    text_id,
                                      GUIITEM_STYLE_E  item_style,
                                      MMI_TEXT_ID_T    left_softkey_id,
                                      MMI_TEXT_ID_T    middle_softkey_id,
                                      MMI_TEXT_ID_T    right_softkey_id,
                                      MMI_CTRL_ID_T    ctrl_id
                                      );
/*****************************************************************************/
//  Discription: This fucntion is used to Get the Paired bt dev list
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPairedDevList(
									uint32     service,
									uint8  *paired_index_ptr
									);
/*****************************************************************************/
//  Discription: update the idle blue tooth icon
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_UpdateBtIdleIcon(
								  BOOLEAN   is_power_on,
								  BOOLEAN   is_visible
								  );
/*****************************************************************************/
//  Discription: get the put request, OPP Server, FTP Server
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_GetReceiveFilePutReq( 
										BT_MSG_T						*msg_body_ptr,
										MMIBT_FILE_SERVER_TYPE_E		service_type
										);
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsDelFail(BT_MSG_T    *msg_body_ptr);

/*****************************************************************************/
//  Discription: the request of add folder by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsAddFolderReq(BT_MSG_T	*msg_body_ptr);
/*****************************************************************************/
//  Description : set ftp root folder after device power on
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void SetFtpRootFolderAfterPowerOn(void);

/*****************************************************************************/
//  Discription: judge if file open 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN IsFileUsed(const uint16 *file_name);
/*****************************************************************************/
//  Discription: DeviceIsPaired
//  Global resource dependence: none 
//  Author: 
//  Note: get bt device is paired by address
/*****************************************************************************/
PUBLIC BOOLEAN DeviceIsPaired(BT_ADDRESS * addr);

#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
/*****************************************************************************/
//  Discription:check the deviece is connect ftp device 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedFTP(BT_ADDRESS * in_addr);


/*****************************************************************************/
//  Discription:check is connect ftp device 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectedFTP(void);

#endif

/*****************************************************************************/
//  Discription:check the deviece is on transfer status 
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsOnTransferStatus(BT_ADDRESS * in_addr,BT_ADDRESS * transfer_addr);

/*****************************************************************************/
//  Discription:get device icon by device info
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: device icon
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T GetDeviceListIcon(BT_DEVICE_INFO *dev);
#ifdef BT_BQB_SUPPORT
/*****************************************************************************/
//  Discription:
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ClccInd(BT_MSG_T* msg_body);

/*****************************************************************************/
//  Discription:
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_PariedHandFree(void);
#endif
/*****************************************************************************/
//  Description : Cancel BT transfer file (send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_StopBTTransferFile(void);
/*****************************************************************************/
//  Discription: Callback fuunction of bluetooth
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CallBackFunc(DPARAM param);
/*****************************************************************************/
//  Discription: Callback fuunction of bluetooth A2DP
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_A2DPCallBackFunc(DPARAM param);
/*****************************************************************************/
//  Discription: This function used to process msg comeback form blue tooth
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E AppHandleBTMsg(
								  PWND app_ptr,
								  uint16 msg_id,
								  DPARAM param
								  );
/*****************************************************************************/
//  Discription: This fucntion is used to send signal from bluetooth to mmi
//  recive the bt app response 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendSignal(uint32 sig_id,DPARAM param);


#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : Get Connected Device Name
//  Global resource dependence : none
//  Author: cheney wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetConnectedDeviceName(
                                               wchar   *headset_name_ptr,
                                               uint16  *name_len_ptr
                                               );
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif
