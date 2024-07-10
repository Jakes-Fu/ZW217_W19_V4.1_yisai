/****************************************************************************
** File Name:      mmibt_func.h                                         *
** Author:         Wenming feng                                            *
** Date:           08/02/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Aug/02/2005    Wenming feng     Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMIBT_FUNC_H_    
#define  _MMIBT_FUNC_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "guilistbox.h"
#include "window_parse.h"
#include "guimenu.h"
#include "mmi_appmsg.h"
#include "mmibt_export.h"
#include "mmifmm_export.h"
#include "mmibt_id.h"
#include "mmibt_menutable.h"
#include "mmifilearray_export.h"
#include "mmibt_nv.h"
#include "bt_abs.h"
#include "mmibt_app.h"
#ifndef WIN32
#include "audio_api.h"
#include "audio_config.h"
#else
#include "mmiaudio_simu.h"
#endif
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
#ifdef BT_SUPPORT_LOW_MEMORY  
#define BT_DEVICE_MAX_NUM                  10
#else
#define BT_DEVICE_MAX_NUM                  20
#endif
#define MMIBT_MEUN_ID_INDEX             0
#define MMIBT_MEUN_TEXT_INDEX           1

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMI_BT_VISIBILE, 
	MMI_BT_HIDE   
}MMIBT_VISIBBILE_TYPE;

typedef enum
{
	MMI_BT_SET_STATUS,
	MMI_BT_SET_VISIBILITY,
	MMI_BT_SET_MAX
}MMIBT_SET_TYPE_E;

typedef enum
{
	MMIBT_STORAGE_UDISK,
	MMIBT_STORAGE_SDCARD,
	MMIBT_STORAGE_MAX
}MMIBT_STORAGE_E;

typedef enum
{
	// @baokun cr91655 changed begin
	MMIBT_DEACTIVE_A2DP = 1,
	MMIBT_ACTIVE_A2DP = 1,
	MMIBT_OPEN_A2DP_HEADSET = 2,
	MMIBT_CLOSE_A2DP_HEADSET = 2,
	MMIBT_DELETE_A2DP_DEVICE = 3,
	MMIBT_A2DP_SET_MAX_ITEM = 3
	// @baokun cr91655 changed end
}BT_A2DP_SET_EARPHONE_E;


//New UI
typedef struct 
{
	uint16 hDev;
	uint8  dev_name[BT_DEVICE_NAME_SIZE];
} MMIBT_PAIRED_DEV_T;





typedef struct  MMIBT_VISIBILE_INFO_TAG
{
    BOOLEAN   is_hide;
    BT_SCAN_E back_val;
}MMIBT_VISIBILE_INFO_T;

#ifdef WIN32
typedef struct _BT_INFO {
	uint8 			status;
	uint8			visibility;
	wchar			local_name[BT_DEVICE_NAME_SIZE + 1];
	uint16			dev_count;
    BT_DEVICE_INFO	active_headset;
	BT_DEVICE_INFO	dev_list[BT_DEVICE_MAX_NUM];
} BT_INFO;
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
typedef enum
{
  BT_INDEX_OPEN =0,
  BT_INDEX_VISIBLE,
  BT_INDEX_PAIRES,
#ifndef PDA_UI_DROPDOWN_WIN
  BT_INDEX_FILETRANS,
#endif
#ifdef MMIBT_FTS_SUPPORT  
  BT_INDEX_SHARED,
#endif
#ifdef BT_HID_SUPPORT
  BT_INDEX_CAMERA,
#endif
  BT_INDEX_NAME,
  BT_INDEX_LOCATION,
  BT_INDEX_HELP,
  BT_INDEX_MAX
}BT_PDA_INDEX_E;
#endif

#ifdef BT_DIALER_SUPPORT
#ifdef BT_DIALER2_SUPPORT
typedef enum
{
  BT_PHONE_MAIN_INDEX_SETTINGS =0,
  BT_PHONE_MAIN_INDEX_PAIRES,
  BT_PHONE_MAIN_INDEX_MAX
}BT_PHONE_MAIN_INDEX_E;
#endif

typedef enum
{
  BT_PHONE_INDEX_ONOFF =0,
  BT_PHONE_INDEX_PAIRES,
//#ifndef PDA_UI_DROPDOWN_WIN
  //BT_PHONE_INDEX_FILETRANS,
//#endif
#if 0 //def BT_PBAP_SUPPORT
  BT_PHONE_INDEX_CONTACT,
#endif
#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
  BT_PHONE_INDEX_SMS,
#endif
  BT_PHONE_INDEX_SETTING,
#ifdef BT_HID_SUPPORT
  BT_PHONE_INDEX_CAMERA,
#endif
  BT_PHONE_INDEX_ABOUT,
  BT_PHONE_INDEX_MAX
}BT_PHONE_INDEX_E;

typedef enum
{
#ifdef BT_DIALER2_SUPPORT
  BT_SETTING_INDEX_ONOFF =0,
  BT_SETTING_INDEX_VISIBLE,
#else
  BT_SETTING_INDEX_VISIBLE =0,
#endif
  BT_SETTING_INDEX_NAME,
  BT_SETTING_INDEX_LOCATION,
#ifdef MMIBT_FTS_SUPPORT  
  BT_SETTING_INDEX_SHARED,
#endif
  BT_SETTING_INDEX_MAX
}BT_SETTING_INDEX_E;

#endif

//New UI
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
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
							 	MMIBT_OPERTATION_TYPE_E	root_opertion, //IN:
								MMIBT_OPERTATION_TYPE_E	current_opertion//IN:
							);
#ifdef BT_DUN_SUPPORT
  // @baokun DUN
/*****************************************************************************/
//  Discription: 
//  
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_OpenDUNWin(BT_MSG_T* msg_body_ptr);

 /*****************************************************************************/
//  Discription: 
//  
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNOpenedInd(BT_MSG_T* msg_body_ptr);

  /*****************************************************************************/
//  Discription: 
//  
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNClosedInd(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseDUNWin(void);

#endif
/*****************************************************************************/
//  Discription: set blue tooth file save location: u disk and sd card
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetFileSaveLocation(void);



/*****************************************************************************/
//  Discription: get blue tooth status from nv and show icon on the idle win
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetBtStatusForIcon(void);

/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendFile(
							const wchar     *filepath_name,		 //in
							uint16           filepath_len,		 //in
							uint32			 file_size,			 //in
							BOOLEAN          is_temp_file        //in
						   );
/*****************************************************************************/
//  Discription: send next file with blue tooth 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendNextFile(
                            const wchar     *filepath_name,      //in
                            uint16           filepath_len,       //in
                            uint32           file_size,           //in
				  BOOLEAN          is_temp_file        //in
                           );


/*****************************************************************************/
//  Discription: close all pb window except main window
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseAllWindow(void);

/*****************************************************************************/
//  Discription: cancel blue tooth ftp function when is in call(MT & MO)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_CancelFileTransfer(BOOLEAN is_close_query);



/*****************************************************************************/
//  Discription: Open the bluetooth To receive file device list win    
//  Global resource dependence: none 
//  Author: Wenming feng 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenSendFileDeviceListWin(void);
/*****************************************************************************/
//  Discription: get FTP server request  
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note://对方要通过FTP取走文件的提示框
//
/*****************************************************************************/
PUBLIC void MMIBT_GetFtpReq(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: receive bt on cnf 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetBtOnOffCnf( BT_MSG_T     *msg_body_ptr, BOOLEAN is_on);



/*****************************************************************************/
//  Discription: get inquired device complete confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_GetInqCompCnf(void);
/*****************************************************************************/
//  Discription: receive bt visible cnf 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetVisibleCnf(BT_MSG_T *msg_body_ptr, BOOLEAN is_visible);

/*****************************************************************************/
//  Discription: when get :MMI_BT_HFAG_SLC_CONNECT_FAILED
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
// PUBLIC void MMIBT_HfagConnFailed(void);


/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendFileFailedInd(void);
/*****************************************************************************/
//  Discription: open the window of new file win 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenNewReceivedFileWin( 
										   BT_MSG_T *msg_body_ptr
										   );

/*****************************************************************************/
//  Discription: open the window of new file win 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_DeleteDeviceCnf( 
								   BT_MSG_T *msg_body_ptr 
								   );

/*****************************************************************************/
//  Discription: get remote user alert status for call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIBT_GetAlertStatus(void);
								   
/*****************************************************************************/
//  Discription: get receive file fail indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ReceiveFileFail(void);
								   
/*****************************************************************************/
//  Discription: Open the blue tooth incoming file win    
//  Global resource dependence: none 
//  Author:kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenIncomingFileWin(
									   BT_MSG_T *msg_body_ptr,
									   MMIBT_FILE_SERVER_TYPE_E		service_type
									   );


/*****************************************************************************/
//  Discription: This function used to Get the setting selected item
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC uint16  MMIBT_GetSetSelectItem(
									  MMIBT_SET_TYPE_E   set_type
									  );
/*****************************************************************************/
//  Discription: This fucntion is used to as mmi bluetooth callback funtion   
//  recive the bt app response 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_Response(void);
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsDelReq(BT_MSG_T	*msg_body_ptr);
/*****************************************************************************/
// 	Description : register bt module nv len and max item
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_RegBtNv(void);

/*****************************************************************************/
// 	Description : Register bt menu group
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIBT_RegMenuGroup(void);

/*****************************************************************************/
//  Discription: receive bt hidden cnf 
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_CancelSearchCnf(BT_MSG_T *msg_body_ptr);

/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_TransferCancelled(BOOLEAN is_send);

/*****************************************************************************/
//  Discription: phone is ready or not
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsPhoneReady(void);

/*****************************************************************************/
//  Discription: Open the blue tooth start win  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenBluetooth(BOOLEAN set_on, BOOLEAN is_need_cnf);

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Reset(void);

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ClearAllDevice(BOOLEAN is_delect_connect);

/*****************************************************************************/
//  Discription: the FTP request  by FTP
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsConnectReq(BT_MSG_T *msg_body_ptr);

/*****************************************************************************/
//  Discription: MMIBT_RedrawProgressBar
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_RedrawProgressBar();



/*****************************************************************************/
//  Discription:check file exist or not, if file not exist, open incoming window;
//              if file already exist,query user overwrite or not if user selected 
//              "Yes", open incoming window, if user select "No", cancel recevie file
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void CheckAuthorizeAndOpenIncomingWin(void);
/*****************************************************************************/
//  Discription: do operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN DoOperation(MMIBT_OPERTATION_TYPE_E  current_opt, BOOLEAN is_poweron_first);
/*****************************************************************************/
//  Discription: Handle query win msg 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleQueryWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           );
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetCurBtInfo(BT_DEVICE_INFO *device_info);
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC BT_DEVICE_INFO  MMIBT_GetCurBtInfo(void);
/*****************************************************************************/
//  Discription: Open CancelW aiting Window
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void OpenCancelWaitingWin(void);
/*****************************************************************************/
//  Description : setting the state of connecting animate ctrl
//  Global resource dependence :
//  Author: wanqiangzh
//  Note:   state 联网动画当前需要设定的状态:1,动画;2,静止
///*****************************************************************************/
PUBLIC void AnimatePlayCtrlByState(int32 state);
/*****************************************************************************/

/*****************************************************************************/
//  Discription: cancel operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS CancelOperation(MMIBT_OPERTATION_TYPE_E  current_opt);
/*****************************************************************************/
//  Discription: get bluetooth transfer file status.(foreground/background)
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIBT_TRANSFER_STATUS_TYPE_E  MMIBT_GetTransferStatus(void);
/*****************************************************************************/
//  Discription: set blue tooth send or receive file status
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferStatus(MMIBT_TRANSFER_STATUS_TYPE_E transfer_status);
/*****************************************************************************/
//  Discription: set blue tooth send or receive file Transfered Size
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferedSize(uint32 size);
/*****************************************************************************/
//  Discription: if blue tooth on transfer status
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_IsOnTransferStatus(void);
/*****************************************************************************/
//  Description : Open BT transfer file win(send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_OpenTransferWin(void);

/****************************************************************************/
//  Description : display receive file ratio info
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayFtpFileSizeInfo(
                                  uint32    file_size,
                                  MMI_CTRL_ID_T ctrl_id,
								  MMI_WIN_ID_T win_id
                                  );
/*****************************************************************************/
//  Discription: connect device to send file result ind
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceResCnf( BT_MSG_T* msg_body_ptr );
/*****************************************************************************/
//  Description : get transfer file name
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_FileName(MMI_STRING_T *file_name);
/*****************************************************************************/
//  Description : get transfer device name
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_DeviceName(MMI_STRING_T *device_name, uint16 max_str_len);
/*****************************************************************************/
//  Discription: Update current picture view list
//  Global resource dependence: none 
//  Author: haiwu.chen
//  Note: update picture view list when is needed
/*****************************************************************************/
PUBLIC void UpdateCurrentPictureViewList(void);
/*****************************************************************************/
//  Discription: Handle receive file waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  AssignReceiveFileCfg(MMIBT_FILE_SERVER_TYPE_E service_type, BOOLEAN is_overwrite);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
/*****************************************************************************/
//  Description : set ftp shared directory
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_SETTING_RET MMIBT_OpenFMMDirectory(void);
/*****************************************************************************/
//  Discription:  MMIBT_CloseStorageMenu
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseStorageMenu(void);

/*****************************************************************************/
//  Discription: get transfered file size
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetTransferedSize(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_HasConnected
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HasConnected(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : MMIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectDevice(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : MMIBT_DisconnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDevice(void);

/*****************************************************************************/
//  Description : MMIBT_ConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDevice(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : receive msg: AG device connected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceCnf(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : receive msg: AG device disconnected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDeviceCnf(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : MMIBT_Init_NVItems
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Init_NVItems(void);

/*****************************************************************************/
//  Discription: Clean BT Dialer Sync Data  
//  
//  Global resource dependence: None
//  Author: Cheney.Wu
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CleanBtDialerSyncData(void);

/*****************************************************************************/
//  Description : open headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void OpenHandsetGateway(const BT_ADDRESS *src_addr);

/*****************************************************************************/
//  Description : receive msg: MMIBT_GetHfuConnectedAddress
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetHfuConnectedAddress(BT_ADDRESS *addr);

#endif

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetSmsSynValue(void);

/*****************************************************************************/
//  Description : MMIBT_SMSSyncBegin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SMSSyncBegin(void);
#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapTpDisconnect(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapComplete(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbort(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapParamsRx(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapDataInd(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapConnect(const BT_ADDRESS *addr);

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapClientTpDisconnect(void);

/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapClientRegister(void);

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_PbapClientDeregister(void);

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbortClient(void);

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullPhonebook(void);

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullSIMPhonebook(void);

/*****************************************************************************/
//  Description : MMIBT_GetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetPbapCancelValue(void);

/*****************************************************************************/
//  Description : MMIBT_SetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapCancelValue(BOOLEAN value);

/*******************************************************************************/
//  Description :添加一条电话本记录
//  Parameter: msg_body_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   vcard_size[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_AddVcardToContact(BT_MSG_T  *msg_body_ptr);

/*******************************************************************************/
//  Description :添加一条电话本记录
//  Parameter: msg_body_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   vcard_size[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_AddVcardToCalllog(BT_MSG_T  *msg_body_ptr);

#ifdef WIN32
/*******************************************************************************/
//  Description :MMIBT_SimuNotifyPBStartParse
//  Parameter: void
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SimuNotifyPBStartParse(void);
/*******************************************************************************/
//  Description :init  BT_MSG_T  *msg_body_ptr, be used to test
//  Parameter: msg_body[in][out]: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : for test
/*****************************************************************************/
PUBLIC void MMIBT_InitPbapData(BT_MSG_T   *msg_body);
#endif

#endif

#if defined(BT_MAP_SUPPORT)
/*******************************************************************************/
//  Description : MMIBT_SMSServerDataReceive
//  Parameter: 
//  Global resource dependence :                                
//  Author: Naomi Huang
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_SMSServerDataReceive(BT_MSG_T  *msg_body_ptr);
/*******************************************************************************/
//  Description :
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_SMSDataReceive(BT_MSG_T  *msg_body_ptr);
/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: Candice
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_MapClientDeregister(void);
/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: Candice
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_MapClientDeregister(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapComplete(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapAbort(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapParamsRx(BT_MSG_T* msg_body_ptr);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapDataInd(BT_MSG_T* msg_body_ptr);

#endif

#ifdef BT_AV_SNK_SUPPORT
PUBLIC BOOLEAN MMIBT_RequestA2dpSinkAudio(void);

PUBLIC void MMIBT_ReleaseA2dpSinkAudio(void);

PUBLIC void MMIBT_SetA2dpSinkVolume(uint32 volume);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif
