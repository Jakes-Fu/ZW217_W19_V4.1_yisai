/****************************************************************************
** File Name:      mmibt_export.h                                         *
** Author:         Wenming feng                                            *
** Date:           Dec/22/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Dec/22/2005    Wenming feng     Create                                  *
**                                                                         *
****************************************************************************/

#ifndef _MMI_BT_EXPORT_H_    
#define  _MMI_BT_EXPORT_H_        

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvfmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "ffs_error.h"
#ifndef _WIN32
#include "audio_config.h"
#include "audio_api.h"
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


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
//add by xingdong.li
typedef enum
{
    MMIBT_A2DP_AUDIO_NONE,
    MMIBT_A2DP_AUDIO_PLAYER,
    MMIBT_A2DP_KURO_PLAYER,
    MMIBT_A2DP_MUSIC_PLAYER,
    MMIBT_A2DP_VIDEO_PLAYER,
    MMIBT_A2DP_ATV,
    MMIBT_A2DP_APP_TYPE_MAX
}MMIBT_A2DP_APP_TYPE_E; 

typedef enum
{
    MMIBT_A2DP_OPR_FUNC_CONNECT,
    MMIBT_A2DP_OPR_FUNC_PLAY,
    MMIBT_A2DP_OPR_FUNC_PAUSE,
    MMIBT_A2DP_OPR_FUNC_RESUME,
    MMIBT_A2DP_OPR_FUNC_STOP,
    MMIBT_A2DP_OPR_FUNC_DISCONNECT,
    MMIBT_A2DP_OPR_FUNC_PLAY_OR_PAUSE,
    MMIBT_A2DP_OPR_FUNC_PREV,
    MMIBT_A2DP_OPR_FUNC_NEXT,
    MMIBT_A2DP_OPR_FUNC_CLOSE_IND,
    MMIBT_A2DP_OPR_FUNC_TYPE_MAX,
}MMIBT_A2DP_OPR_FUNC_TYPE_E; 

typedef void (* MMI_A2DP_OPR_FUNC)(void);

typedef struct
{
    MMIBT_A2DP_APP_TYPE_E   app_type;
    uint32                  sample_rate;
    MMI_A2DP_OPR_FUNC       opr_func[MMIBT_A2DP_OPR_FUNC_TYPE_MAX];
}MMIBT_A2DP_APP_INFO_T; 

typedef  struct 
{
	uint32  sig_id;//bluetooth operate type
	uint32  data_len;
	char signal_data[200];
}BTSignalS; 

/* Get around the silly signal mechanism of MMK */
typedef struct _BT_SIGNAL {
    SIGNAL_VARS
    BT_MSG_T	    content;			// message content
} BT_SIGNAL;

 typedef enum
{
    MMIBT_MODULE_NONE,
    MMIBT_MODULE_CC,
	MMIBT_MODULE_UDISK,
    MMIBT_MODULE_DV,
    MMIBT_MODULE_MPEG4,
	MMIBT_MODULE_BLUETOOTH,
	MMIBT_MODULE_A2DP,
	MMIBT_MODULE_ST,
	MMIBT_MODULE_MAX
}MMIBT_HIDDEN_BT_MODULE_E;
 
typedef enum
{
    BT_A2DP_MUSIC_STOP,
    BT_A2DP_RESET_PARAM,
    BT_A2DP_STOP_REASON_NUM
}MMIA2DP_CLOSE_REASON_E;

typedef enum
{
	MMI_BT_ME_ON_CNF = ((0x97 << 8) | 1),
	MMI_BT_ME_OFF_CNF,
	MMI_BT_VISIBILE_CNF,
    MMI_BT_HIDDEN_CNF,
	MMI_BT_INQ_DEV_ADDR,
	MMI_BT_INQ_DEV_NAME,
	MMI_BT_INQ_COMP_CNF,
	MMI_BT_INQUIRY_CANCEL,
	MMI_BT_DEV_PIN_REQ,
	MMI_BT_SSP_NUM_IND,
	MMI_BT_DEV_PAIR_COMPLETE,
	MMI_BT_DEV_PAIR_COMPLETE_IND,
	MMI_BT_DEV_SERVICE_RESULT,     //Bluetooth got service result

	MMI_BT_A2DP_SRC_CONFIG_STREAM, //Bluetooth A2DP SRC config stream
	MMI_BT_A2DP_SRC_OPEN_STREAM,   //Bluetooth A2DP SRC open stream
	MMI_BT_A2DP_SRC_START_STREAM,  //Bluetooth A2DP SRC start stream
	MMI_BT_A2DP_SRC_SUSPEND_STREAM,//Bluetooth A2DP SRC suspend stream
	MMI_BT_A2DP_SRC_CLOSE_STREAM,  //Bluetooth A2DP SRC close stream

	MMI_BT_HFAG_SLC_CONNECTED,     //Bluetooth HFAG SLC connected
	MMI_BT_HFAG_SLC_CONNECTED_IND,
	MMI_BT_HFAG_SLC_DISCONNECTED,  //Bluetooth HFAG SLC disconnected
	MMI_BT_HFAG_DIAL_NUMBER,		//Bluetooth HFAG dialing number
	MMI_BT_HFAG_DIAL_MEMORY,		//Bluetooth HFAG dialing memory
	MMI_BT_HFAG_REDAIL_CALL,		//Bluetooth HFAG  redial call
	MMI_BT_HFAG_HFU_HOLD,			//Bluetooth hold hfag call 
	MMI_BT_HFAG_AUDIO_CONNECTED,	//Bluetooth HFAG audio connected
	MMI_BT_HFAG_AUDIO_DISCONNECTED,	//Bluetooth HFAG audio disconnected
    MMI_BT_DEV_AUTH_IND,
    
    MMI_BT_HFU_DTMF_NUMBER,        //Bluetooth hfu dtmf
	MMI_BT_HFU_CALL_ANSWER,        //Bluetooth hfu call answer
	MMI_BT_HFU_CALL_HANGUP,        //Bluetoothf HF Call 1 hungup
	MMI_BT_HFU_STATUS_REQ,			//Bluetooth set hfu status
	MMI_BT_HS_MIC_VOL_CHANGE,      //Bluetooth HS MIC volume change 
	MMI_BT_HS_SPK_VOL_CHANGE,      //Bluetooth HS SPK voluem change

	MMI_BT_HFU_CLCC,				//mdy by qing.yu@for cr238862
	MMI_BT_OPC_CONNECT_RESULT,      //Bluetooth FTP connect
	MMI_BT_OPC_SERVER_ABORT,		//Bluetooth server cancel send file
	MMI_BT_OPC_PUT_ABORT,			//Bluetooth server opc put abort
	MMI_BT_OPC_SEND_RES,			//Bluetooth server opc send file result	
	MMI_BT_FTS_SEND_RES,			//Bluetooth server FTS send file result	
	MMI_BT_FTS_GET_REQ,				//Bluetooth server fts get res
	MMI_BT_OPS_PUT_REQ,				//Bluetooth opp server put request
	MMI_BT_OPS_CLIENT_ABORT,		//Bluetooth opp server client abort
	MMI_BT_OPS_PUT_ABORT,			//Bluetooth opp server put abort
    MMI_BT_SEND_CANCELED,           // send canceled
	MMI_BT_RECEIVE_CANCELED,           //receive canceled

	MMI_BT_FTS_SHARED_FOLDER_UPDATE,//Bluetooth FTP shaired folder update
	MMI_BT_FTS_PUT_REQ,				//Bluetooth ftp server put request
	MMI_BT_FTS_CLIENT_ABORT,		//Bluetooth ftp server client abort
	MMI_BT_FTS_PUT_ABORT,			//Bluetooth ftp server PUT abort


	MMI_BT_DELETE_DEVICE_RES,		//Bluetooth delete device
	MMI_BT_PACKET_SENT,				//Bluetooth packet send 
	MMI_BT_PACKET_RECEIVED,			//Bluetooth packet received
	MMI_BT_FTS_RECEIVE_FILE_RES,		//Bluetooth fts receive file result
	MMI_BT_OPS_RECEIVE_FILE_RES,		//Bluetooth opp receive file result
	MMI_BT_FTS_DEL_REQ,				//Bluetooth delete file/dir req
	MMI_BT_FTS_DEL_FAIL,
	MMI_BT_FTS_ADD_FOLDER_REQ,		//Bluetooth add folder req
    MMI_BT_DUN_LINK_REQ,//DUN connected
	MMI_BT_DUN_LINK_SUC,//DUN connected
	MMI_BT_DUN_LINK_CLOSE,//DUN connected

//for a2dp begin
	MMI_BT_A2DP_OPERATION_BACKWARD, //Bluetooth a2dp operation backward
	MMI_BT_A2DP_OPERATION_FORWARD,	//Bluetooth A2dp operation forward
	MMI_BT_A2DP_OPERATION_VOL_UP,	//Bluetooth a2dp operation volume up
	MMI_BT_A2DP_OPERATION_VOL_DOWN,	//Bluetooth a2dp operation volume down
	MMI_BT_A2DP_OPERATION_PLAY,		//Bluetooth a2dp operation play
	MMI_BT_A2DP_OPERATION_STOP,		//Bluetooth a2dp operation stop
	MMI_BT_A2DP_OPERATION_PAUSE,	//Bluetooth a2dp operation pause
	MMI_BT_AVRCP_DISCONNECT,        //Bluetooth AVRCP disconnect ind
	MMI_BT_AVRCP_CONNECT,			//Bluetooth AVRCP connect ind
	MMI_BT_A2DP_CONNECTED,			//BLUEtooth a2dp connect
	MMI_BT_A2DP_CONNECTED_IND,
	MMI_BT_A2DP_DISCONNECTED,		//BLUEtooth a2dp disconnect
	MMI_BT_OPEN_DEVICE_CNF,		     //Bluetooth a2dp operation play cnf
	MMI_BT_RESUME_DEVICE_CNF,		//Bluetooth a2dp operation resume cnf
	MMI_BT_SUSPEND_DEVICE_CNF,		//Bluetooth a2dp operation pause cnf
	MMI_BT_A2DP_START_STREAM_IND,	//Bluetooth a2dp headset start stream ind
	MMI_BT_A2DP_SUSPEND_STREAM_IND, //Bluetooth a2dp headset suspend stream ind
	MMI_BT_A2DP_RESUME_STREAM_IND,	//Bluetooth a2dp headset resume stream ind
	MMI_BT_A2DP_ABORT_IND,			//Bluetooth a2dp abort ind
	MMI_BT_A2DP_ABORT_CFM,			//Bluetooth a2dp abort confirm
	MMI_BT_CLOSE_DEVICE_CNF,		//Bluetooth a2dp operation stop cnf
	MMI_BT_PAUSE_STOP_DEVICE_CNF,//Bluetooth pause or stop play cnf
	MMI_BT_PLAY_RESUNME_DEVICE_CNF,//Bluetooth play or resume play cnf
	
    MMI_BT_FTS_CONNECT_REQ,			//Bluetooth ftp server connect request
    MMI_BT_OPS_CONNECT_REQ,
//for a2dp end

    //+++ CR244706
    MMI_BT_FTS_DISCONNECT_IND,      //Bluetooth ftp server disconnect ind
    MMI_BT_OPS_DISCONNECT_IND,      //Bluetooth opp server disconnect ind
    MMI_BT_OPC_DISCONNECT_IND,       //Bluetooth opp client disconnect ind
    //--- CR244706

    //SPP Message begin
#ifdef BT_SPP_SUPPORT
    //MMI_BT_SPP_LINK_STATUS_RES,
    MMI_BT_SPP_DATA_RECIEVE_IND,
    MMI_BT_SPP_DATA_RECIEVE_CFM,
    MMI_BT_SPP_CONNECT_IND,
    MMI_BT_SPP_DISCONNECT_IND,
    MMI_BT_SPP_STATUS_IND,
    //MMI_BT_SPP_SERVICE_NAME_IND,   
#endif
    //SPP Message end
    //PBAP Message start
#ifdef BT_PBAP_SUPPORT
    MMI_BT_PBAP_TP_DISCONNECTED,
	MMI_BT_PBAP_COMPLETE,
	MMI_BT_PBAP_ABORT,
	MMI_BT_PBAP_PARAMS_RX,
	MMI_BT_PBAP_DATA_IND,
#endif
    
    //PBAP Message end
#ifdef BT_MAP_SUPPORT
    MMI_BT_MAP_TP_DISCONNECTED,
    MMI_BT_MAP_COMPLETE,
    MMI_BT_MAP_ABORT,
    MMI_BT_MAP_CLIENT_PARAMS_RX,
    MMI_BT_MAP_SERVER_PARAMS_RX,
    MMI_BT_MAP_DATA_IND,
    MMI_BT_MAP_MNS_SERVER_CONNECTED,
    MMI_BT_MAP_MNS_SERVER_PARAMS_RX,
    MMI_BT_MAP_MNS_SERVER_DATA_IND,
#endif

#ifdef BT_AV_SNK_SUPPORT
	MMI_BT_A2DP_SINK_CONNECTED,
	MMI_BT_A2DP_SINK_DISCONNECTED,
	MMI_BT_A2DP_SINK_STREAM_STARTED,
	MMI_BT_A2DP_SINK_STREAM_SUSPENDED,
#endif
#ifdef BT_AV_CT_SUPPORT
	MMI_BT_AVRCP_CT_CONNECTED,
	MMI_BT_AVRCP_CT_DISCONNECTED,
	MMI_BT_AVRCP_CT_MEDIA_ATTRIBUTES,
	MMI_BT_AVRCP_CT_PLAY_STATUS,
	MMI_BT_AVRCP_CT_TRACK_CHANGED,
	MMI_BT_AVRCP_CT_PLAYBACK_CHANGED,
	MMI_BT_AVRCP_CT_PLAYBACK_POS_CHANGED,
	MMI_BT_AVRCP_CT_PLAYER_APPLICATION_CHANGED,
	MMI_BT_AVRCP_CT_SUPPORTED_PLAYER_ATTRIBUTES,
#endif
#ifdef BT_HID_SUPPORT
      MMI_BT_HID_CONNECT_IND,
      MMI_BT_HID_DISCONNECT_IND,
#endif
#ifdef BT_BLE_SUPPORT
/*BLE begin */
    MMI_BT_BLE_CONNECTED,
    MMI_BT_BLE_DISCONNECTED,
    MMI_BT_BLE_DATA_IND,
/*BLE end */
#endif
    MMI_BT_MSG_MAX

}MMI_BT_CNF_E;

typedef enum 
{
    MMIBT_DEFAULT_HEADSET,//not support headset
    MMIBT_HFG_HEADSET,        //hfg headset
    MMIBT_A2DP_HEADSET        //a2dp headset
}MMIBT_HEADSET_TYPE;

#ifdef BT_DIALER_SUPPORT
typedef enum
{
    BT_PBAP_TYPE_NONE = 0,
    BT_PBAP_TYPE_PB,
    BT_PBAP_TYPE_SIM,
    BT_PBAP_TYPE_CL,
    BT_PBAP_TYPE_MAX
}BT_PBAP_TYPE_E;
#endif

#ifdef BT_DIALER_SUPPORT
#define MMIBT_PAIR_PWD_LEN             4
//the information about pair password
typedef struct
{
    uint8       pwd[MMIBT_PAIR_PWD_LEN];   //配对密码
    uint8       pwd_len;                  //密码长度
}MMIBT_PAIR_PWD_T;
#endif

#ifndef BLUETOOTH_SUPPORT
/******************************************************************************
 * BT stucture
 ******************************************************************************/
typedef uint32 BT_STATUS;
#define BT_SUCCESS				    0
#define BT_ERROR				    1
#define BT_PENDING				    2
#define BT_BUSY					    3
#define BT_NO_RESOURCE				4
#define BT_NOT_FOUND				5
#define BT_DEVICE_NOT_FOUND			6
#define BT_CONNECTION_FAILED		7
#define BT_TIMEOUT				    8
#define BT_NO_CONNECTION			9
#define BT_INVALID_PARM				10
#define BT_NOT_SUPPORTED			11
#define BT_CANCELLED				12

#define BT_IN_PROGRESS              19
#define BT_RESTRICTED               20
#define BT_INVALID_HANDLE           30
#define BT_PACKET_TOO_SMALL         31

#define BT_FAILED                   0x41
#define BT_DISCONNECT               0x43
#define BT_NO_CONNECT               0x44
#define BT_IN_USE                   0x45
#define BT_MEDIA_BUSY               0x46

#define BT_OFF                      0x50

#endif

//CR178620
#define MMIBT_SENDFILE_MAX_NUM  10
/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: This fucntion is used to open the bluetooth main menu win  
//  
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenMainMenuWin(void);

/****************************************************************************/
//  Description : enter bluetooth main window
//  Global resource dependence :
//  Author:bin.wang1
//  Note:
/****************************************************************************/
PUBLIC void MMIAPIBT_MainWin_Enter( void );

PUBLIC MMI_RESULT_E  HandleCanbeCancelWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );
/*****************************************************************************/
//  Discription:check headset permit to open
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HeadsetOpenIsPermit(BT_ADDRESS * bt_addr);



#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Discription: This fucntion is used to open the BtPhone main menu win  
//  
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtPhoneWin(void);

/*****************************************************************************/
//  Discription: This fucntion is used to judge it is submachine mode or not  
//  
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsSubMachineMode(void);

/*******************************************************************************/
//  Description : MMIBT_SetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapRequestType(BT_PBAP_TYPE_E pbapType);

/*******************************************************************************/
//  Description : MMIBT_GetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC BT_PBAP_TYPE_E MMIBT_GetPbapRequestType(void);

/*****************************************************************************/
//  Discription: check if pbap is connected
//  Global resource dependence: none 
//  Author: cheney wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_PbapIsConnected(void);
#endif

#ifdef BT_MAP_SUPPORT
/*****************************************************************************/
//  Discription: check if map is connected
//  Global resource dependence: none 
//  Author: cheney wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_MapIsConnected(void);
#endif

/*****************************************************************************/
//  Discription: This fucntion is used to update the bluetooth main menu win 
//  Global resource dependence: None
//  Author: Arvin zhang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_UpdateMainMenuWin(void);

/*****************************************************************************/
//  Discription: to close the BT paired device window opened by other app
//               (i.e. mp3,fmm) while sd card is being plugged in/out
//  Global resource dependence: None
//  Author: Arvin zhang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseBtPairedDeviceList(void);

/*****************************************************************************/
// 	Description :get vuart run status 
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_DUNIsRun(void);

/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseDUNWin(void);

/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:此接口已作废，请用新接口:MMIAPIBT_SendMultiFile();
//
/*****************************************************************************/
/*PUBLIC void MMIAPIBT_SendFile(
							const wchar     *filepath_name,		 //in
							uint16           filepath_len,		 //in
							uint32			 file_size,			 //in
							BOOLEAN          is_temp_file        //in
						   );
*/
/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SendMultiFile(MMIFMM_BT_SENDFILE_INFO_T *send_file_info_ptr,int file_num);

/*****************************************************************************/
//  Discription: get bt device status
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:TRUE: bt device on. FALSE: bt device off
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_IsBtOn(void);

/*****************************************************************************/
//  Discription: get bt sending connect and receiving window status
//  Global resource dependence: none 
//  Author: leiming.ma 
//  Note:TRUE: window is open . FALSE: window is closed
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsSendingOrReceiving(void);

/*****************************************************************************/
//  Discription: bt remote alert
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_RemoteAlerting(void);

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetBtCallStatus(uint32 status);

/*****************************************************************************/
//  Discription: bt call connect
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallConnect(BT_ADDRESS *addr);

/*****************************************************************************/
//  Discription: bt get active handfree 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIAPIBT_GetActivatedHS(BT_ADDRESS *addr);

/*****************************************************************************/
//  Description : MMIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectDevice(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : receive msg: MMIBT_IsHfuConnected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsHfuConnected(void);

/*****************************************************************************/
//  Description : Get HFU connected device BT address
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetHfuConnectedAddress(BT_ADDRESS *addr);

/*****************************************************************************/
//  Description : MMIBT_IsConnectToAddr
//  Global resource dependence : 
//  Author:cheney.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectToAddr(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : MMIBT_CreatePbapSyncTimer
//  Global resource dependence : 
//  Author: yanyan.an
//  Note: 创建超时定时器
/*****************************************************************************/
PUBLIC void MMIBT_CreatePbapSyncTimer(void);

/******************************************************************************/
// Description: MMIBT_StopPbapSyncTimer
// Global resource dependence: 
// Author: yanyan.an
// Note: 停止超时定时器
/******************************************************************************/
PUBLIC void MMIBT_StopPbapSyncTimer(void);

/*****************************************************************************/
//  Description : Open alert tip win
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_OpenAlertTipWin(MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapClientRegister(void);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIBT_HandlePubwinConnectWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Discription: bt Stop Alert 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopAlert(void);


/*****************************************************************************/
//  Discription: bt Stop Ring 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_StopRing(void);


/*****************************************************************************/
//  Discription: bt call disconnect 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_CallDisconnect(void);

/*****************************************************************************/
//  Discription: bt audio transfer 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note: 
//
/*****************************************************************************/
PUBLIC  BT_STATUS  MMIAPIBT_TransferAudioConn(uint8 direct);

/*****************************************************************************/
//  Discription: if blue tooth open send or receive file win
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_IsOpenFileWin(void);

								
/*****************************************************************************/
//  Discription: This fucntion is used to Init bluetooth app 
//  recive the bt app response 
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_AppInit(void);


/*****************************************************************************/
//  Discription: This function is used to deInit the Blue tooth sub app
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AppDeInit(void);

/*****************************************************************************/
//  Discription: Get if using blue tooth headset to call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN		MMIAPIBT_IsHeadsetUsedInCall(void);

/*****************************************************************************/
// 	Description : intial bt module  
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_InitModule(void);

/*****************************************************************************/
//  Discription: blue tooth action make a outgoing call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OutgoingCall(void);

/*****************************************************************************/
//  Discription: blue tooth action when receive incoming call
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_IncomingCall(MMI_STRING_T		prompt_str);

/*****************************************************************************/
//  Discription: open blue tooth ui test window to test
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenUitestWin(BOOLEAN		bluetooth_on);

///*****************************************************************************/
////  Discription: Hidden blue tooth for app
////  Global resource dependence: none 
////  Author: kelly.li 
////  Note:
////
///*****************************************************************************/
//PUBLIC BOOLEAN MMIAPIBT_HiddenBtForAPP(
//									MMIBT_HIDDEN_BT_MODULE_E	module,
//									BT_SCAN_E					visible_type									
// 									);

/*****************************************************************************/
//  Discription: recovery blue tooth status after app
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_RecoveryBtStatus(MMIBT_HIDDEN_BT_MODULE_E	module);

/*****************************************************************************/
//  Discription: if blue tooth is sending or receiving file 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
//PUBLIC BOOLEAN MMIBT_IsSendOrReceiveFile(void);

/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetVolume(uint8 channel, uint8 value);
 
//mdy by qing.yu@for cr238867
/*********************s********************************************************/
// Description : get bt volume
// Global resource dependence : 
// Author: qing.yu
// Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetVolume(uint8 channel);
 
/*****************************************************************************/
// Description : 蓝牙开关设置页面
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-6
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMIAPIBT_SetBluetoothMode(void);

/*****************************************************************************/
//  Discription: get the current bt address
//  Global resource dependence: none 
//  Author: samboo.shen
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetAddress(BT_ADDRESS *addr);

/*****************************************************************************/
//  Description : pause a2dp device of playing mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpPauseDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpOpenDevice_1(
                     uint32 sample_ret
                    );

PUBLIC BOOLEAN MMIAPIBT_A2dpOpenDevice(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    );

/*****************************************************************************/
//  Description : Resume a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIBT_A2dpResumeDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpStopDevice_1(
                    void
                    );

/*****************************************************************************/
//  Description : stop a2dp device to play mp3
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_A2dpStopDevice(
                    MMIBT_A2DP_APP_TYPE_E     a2dp_app_type
                    );

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpReadyForPlay(void);

/*****************************************************************************/
//  Description : MMIBT Is A2dp Ready For Play
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN MMIAPIBT_IsA2dpPaused(void);

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_A2DP_APP_TYPE_E MMIAPIBT_A2dpGetAppType(
                                    void
                                    );
                                    
/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpSetAppInfo(
                    MMIBT_A2DP_APP_INFO_T  a2dp_app_info
                    );

/*****************************************************************************/
//  Description : open a2dp device to play mp3
//  Global resource dependence : none
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_A2dpClearAppInfo(
                    MMIBT_A2DP_APP_TYPE_E   app_type
                    );
                    
/*****************************************************************************/
//  Description : get the sample rate of a2dp stream
//  Global resource dependence : none
//  Author: kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIBT_GetA2DPSampleRate(void);

/*****************************************************************************/
//  Discription: get blue tooth status from nv and show icon on the idle win
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_GetBtStatusForIcon(void);

/*****************************************************************************/
//  Discription: set save file location info 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SaveFileLocation(
									MMIFILE_DEVICE_E storage
									);

/*****************************************************************************/
//  Discription: get  file location info 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIAPIBT_GetFileLocation(void);

/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_SetFtpRootFolder(MMIFILE_DEVICE_E storage);

/*****************************************************************************/
// 	Description : set ftp root folder 
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_EnterEutMode(void);

/*****************************************************************************/
//  Description : open or close blue toothe
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBlueTooth(BOOLEAN is_open);
/*****************************************************************************/
//  Description : open or close blue toothe
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBlueToothInFlyMode(BOOLEAN is_open);


/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Reset(void);

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_ClearAllDevice(void);

/*****************************************************************************/
//  Description : Get Active Device to play music
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: first check if active A2DP device exist, only not active A2DP device exist, 
//        then check if exist HFG device
//  Return: TRUE: Exist active device to play; FALSE : No device to play
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetActiveBTHeadset(void);

/*****************************************************************************/
//  Description : get active device type
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_HEADSET_TYPE MMIAPIBT_GetActiveHeadsetType(void);

/*****************************************************************************/
//  Discription:  MMIAPIBT_CloseStorageMenu
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseStorageMenu(void);

/*****************************************************************************/
//  Description : 用户指定播放耳机类型，如果该类型不支持则不能播放
//  Global resource dependence : none
//  Paramter: headset_type:用户指定的耳机类型
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SpecifyHeadset(MMIBT_HEADSET_TYPE headset_type);

/*****************************************************************************/
//  Discription:  MMIAPIBT_ Open Bt Headset List
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtHeadsetList(void);

/*****************************************************************************/
//  Description : Get Active Headset Device Name
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetActiveHeadsetName(
                    wchar   *headset_name_ptr,
                    uint16  *name_len_ptr
                    );

/*****************************************************************************/
//  Discription: Get Call held status
//               call status
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 为BQB更新，底层协议要求支持3种情况:
//          1.没有hold call---0
//          2.只有hold call---2
//          3.既有hold call，又有active call ---1
//  Return:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetCallHeldStatus(void);


#ifdef BT_BQB_SUPPORT                    
//mdy by qing.yu@for cr238605 cr238609
/*****************************************************************************/
//  Discription: request sending +CIEV command, inform HF side about the current
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CIEVCommandIndicator(uint32 ciev_command,uint32 command_status);

/*****************************************************************************/
//  Discription: Set Call held status
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallHeldStatus(void);

/*****************************************************************************/
//  Discription: Set Signal Strength Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetSignalStrength(uint32 status);

/*****************************************************************************/
//  Discription: Set Battery Charge Information
//               call status
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetBatteryCharge(uint32 status);


/*****************************************************************************/
//  Description : update network status

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetNetWorkRoamStatus(BOOLEAN is_in_roam);

/*****************************************************************************/
//  Discription: set infomation of call
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallInfo(
                uint8 call_index,
                uint8 call_dir,
                uint8 call_status,
                uint8 mode,
                uint8 mpty,
                uint8* number,
                uint8* call_type);

//mdy by qing.yu@for cr238585
/*****************************************************************************/
//  Discription: set the name of net operator
//  Global resource dependence: None
//  Author: qing.yu 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetOperatorName(void);

//mdy by qing.yu@for cr238786 cr238787
/*****************************************************************************/
//  Description : inform bt has number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallSuccess(void);

/*****************************************************************************/
//  Description : inform bt no number to call

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetCallFail(void);

/*****************************************************************************/
//  Description : set my phone number

//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetMyPhoneNumber(void);

/*****************************************************************************/
//  Description : Set Service is valid
//  Global resource dependence : 
//  Author: baokun.yin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_Service_Indicator(BOOLEAN is_in_net);

/*****************************************************************************/
//  Description : Open BT Paried Device Windos
//  Global resource dependence : 
//  Author: qing.yu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_PariedHandFree(void);


#endif //BT_BQB_SUPPORT

#ifdef WIN32
/*****************************************************************************/
//  Description:   remote pair
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Pair(void);

/*****************************************************************************/
//  Description:   simulate send file to phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Send(const uint16* file_path);

/*****************************************************************************/
//  Description:   simulate cancel send file to phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Send_Cancel(void);

/*****************************************************************************/
//  Description:   simulate cancel receive file from phone
//  Author:         
//  Note:
//  Parameter:      None
//****************************************************************************/
PUBLIC void MMIBTAPI_Simu_Receive_Cancel(void);
#endif

/*****************************************************************************/
//  Discription: Handle delete paired device or not win msg  
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBTAPI_DeletePairedDevice(BOOLEAN is_all);
#ifdef BT_DUN_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_CloseDUNWin(void);
#endif
/*****************************************************************************/
//  Description : Open BT transfer file win(send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenTransferWin(void);
/*****************************************************************************/
//  Description : stop BT transfer file (send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_StopBTTransferFile(void);
/*****************************************************************************/
//  Discription: send Multi_file to other device cancelled
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_TransferCancelled(void);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendNextFileCallBack(void);

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuSwitchAudio(BOOLEAN is_audio_on);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuDialNumber(const uint8 *phone_num);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuAnswerCall(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuListCurrentCalls(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuHangup(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuSendDtmf(uint8 dtmfTone);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_HfuCallHold(uint8 hold_action);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_CanBeReconnect(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_DisconnectDevice(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_ConnectDevice(void);

/*****************************************************************************/
//  Description : MMIAPIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsConnectDevice(void);

/*****************************************************************************/
//  Discription: This fucntion is used to open the BtPhone main menu win  
//  
//  Global resource dependence: None
//  Author: yang.yang 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_OpenBtPhoneWin(void);

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : MMIAPIBT_IsPbapCancel
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsPbapCancel(void);

/*****************************************************************************/
//  Description : MMIAPIBT_SetPbapCancel
//  Global resource dependence : 
//  Author:allen lee
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetPbapCancel(BOOLEAN is_cancel);

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
//  Discription: sync confirm implementation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_DirectConnectPbapService(void);
#endif

#ifdef BT_SPP_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIBT_SppWriteData(const uint8 * src_buf, uint32 data_len);

#endif

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_GetSmsSynVal(void);
#endif

#ifdef BT_AV_SNK_SUPPORT
/*****************************************************************************/
//  Description : Create a transport connection for A2DP Sink
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_A2dpSinkConnect(BT_ADDRESS addr);

/*****************************************************************************/
//  Description : Disconnect for A2DP Sink
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_A2dpSinkDisconnect(void);

/*****************************************************************************/
//  Description : A2DP SINK connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsA2dpSinkConnected(void);

/*****************************************************************************/
//  Description : HFU connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_isHfuConnected(void);

#endif

#ifdef BT_AV_CT_SUPPORT
/*****************************************************************************/
//  Description : Create a transport connection for AVRCP CT
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AvrcpCtConnect(BT_ADDRESS addr);

/*****************************************************************************/
//  Description : Disconnect for AVRCP CT
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_AvrcpCtDisconnect(void);

/*****************************************************************************/
//  Description : AVRCP CT connection status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsAvrcpCtConnected(void);

/*****************************************************************************/
//  Description : Execute Avrcp Operation
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_SetAvrcpPanelKey(uint16 op, BOOLEAN press, BOOLEAN auto_release);

/*****************************************************************************/
//  Description : Get mda play status
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIAPIBT_GetMdaPlayStatus(void);

/*****************************************************************************/
//  Description : set a2dp sink audio volume
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_SetA2dpSinkVolume(uint32 volume);

/*****************************************************************************/
//  Description : get a2dp sink sample rate
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIBT_GetA2dpSinkSampleRate(void);

/*****************************************************************************/
//  Description : get a2dp sink channel num
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIBT_GetA2dpSinkchannelNum(void);

/*****************************************************************************/
//  Description : route a2dp sink audio
//  Global resource dependence :                                
//  Author: Kay Chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_RouteA2dpSinkAudio(void);
#endif

#if defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: Candice
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIBT_MapClientDeregister(void);

/*****************************************************************************/
//  Description : MMIBT_MapTpDisconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapTpDisconnect(BT_MSG_T* msg_body_ptr);

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

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapServerDataInd(BT_MSG_T* msg_body_ptr);
#endif

#ifdef UAL_BT_HFU_SUPPORT
/****************************************************************************/
//  Description : WATCHBT_GetConnectDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:获取配对设备的地址
/****************************************************************************/
PUBLIC void WATCHBT_GetBoundDevAddr(uint8 *p_conn_addr );

/****************************************************************************/
//  Description : WATCHBT_SetConnectDevAddr
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:设置配对设备的地址，第一次配对成功后，开始写
/****************************************************************************/
PUBLIC void WATCHBT_SetBoundDevAddr(uint8 *p_conn_addr );
/****************************************************************************/
//  Description : WATCHBT_HFUConnect
//  Global resource dependence :
//  Author:chunjuan.liang
//  Note:hfu connect，当前hfu只能连接一个
/****************************************************************************/
PUBLIC BOOLEAN WATCHBT_HFUConnect(void);

#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif
