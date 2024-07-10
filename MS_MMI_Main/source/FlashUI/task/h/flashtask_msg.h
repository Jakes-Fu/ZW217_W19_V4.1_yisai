/*
 * FileName: FlashTask_msg.h
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 */
 
#ifndef FLASHTASK_MSG_H
#define FLASHTASK_MSG_H

#include "os_api.h"
#include "mmk_type.h"

//=================================================================
// Touch Panel event
//=================================================================
typedef struct
{
	int	x;
	int	y;
#ifndef WIN32 
	int	x2;
	int	y2;
#endif
	uint32	tick_count;//I68_tp_scx
}FlashTPPress;


typedef struct
{
	int				m_type;
	uint8			m_call_index;
	void*			m_sema;//cain_flash
	//CALL_STRING		m_number;
	//CALL_STRING		m_name;
}FlashCallMsg;


typedef struct
{
	int				m_type;
	//  TODO: add your code here
}FlashPhoneMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	//  TODO: add your code here
}FlashContactMsg;

typedef struct
{
	BOOLEAN			m_is_mms;
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashSmsMsg;

//cain
typedef struct
{
	BOOLEAN			m_is_mms;
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashMmsMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashRecorderMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashFmMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashAlarmMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashScheduleMsg;

typedef struct
{
	int				m_type;
	int				m_data;
	void*			m_pData;
	//  TODO: add your code here
}FlashBluetoothMsg;

typedef struct
{
	int				m_type;
}FlashSettingMsg;

typedef struct 
{
	int				m_type;
	int				m_data;	
	//  TODO: add your code here
}FlashSTKMsg;

typedef struct
{
	int 				m_type;
}FlashSSMsg;

typedef struct
{
	int				m_type;
	int				m_data;	
	//  TODO: add your code here
}FlashPowerMsg;

typedef struct 
{
	int				m_type;
	int				m_data;
	void*			m_pData1;
	void*			m_pData2;
	// TODO: add your code here
}FlashJavaMsg;

typedef union
{
	FlashPhoneMsg	phone;
	FlashCallMsg		call;
	FlashContactMsg	contact;
	FlashSmsMsg		sms;
	FlashMmsMsg		mms;//cain
	FlashSettingMsg	setting;
	FlashSTKMsg		stk;
	FlashRecorderMsg	rec;
	FlashFmMsg		fm;
	FlashBluetoothMsg 	bt;
	FlashAlarmMsg	alarm;
	FlashJavaMsg		java;//A8_JAVA_LSM
//	FlashMmsMsg			mms;//A8_MMS_LSM
	FlashScheduleMsg	schedule;
	
}FlashPSMsg;

typedef union
{
	FlashPowerMsg	power;
}FlashDrvMsg;


typedef struct
{
	int					m_type;
	//NotificationCallBack	m_onOK;
	//NotificationCallBack	m_onCancel;
	unsigned long			m_time;
	char					*m_title;
	uint16				title_len;
	BOOLEAN				title_is_ucs2;
	char					*m_msg;
	uint16				msg_len;
	BOOLEAN				msg_is_ucs2;
}FlashNotification;



//=================================================================
// Message type
//=================================================================
typedef union
{
	uint32			keycode;
	FlashTPPress		tp;
	FlashPSMsg		ps;
	FlashNotification	notification;
	FlashDrvMsg		drv;
	uint32			param;
}MSG_PARAM;

typedef struct
{
	SIGNAL_VARS
	MSG_PARAM msg;
}Flash_Signal;


//=================================================================
// Event Queue
//=================================================================
#define MMK_MSG_QUEUE_LEN 	256

typedef struct
{
	int head;
	int tail;
	Flash_Signal msg_queue[MMK_MSG_QUEUE_LEN];
}FLASHTASK_EVENT_QUEUE;



#define FLASH_TASK_TICK_INTERVAL		20

//=================================================================
// Flash Task signal id
//=================================================================
#define MUSIC_GROUP (0xDF)
#define UDISK_GROUP (0xE0)

typedef enum
{
	FLASH_SIGNAL_MUSIC_INIT = ((MUSIC_GROUP << 8) | 1),
	FLASH_SIGNAL_MUSIC_START_PLAY,
	FLASH_SIGNAL_MUSIC_STOP_PLAY,
	FLASH_SIGNAL_MUSIC_MAX
}FLASH_SIGNAL_MUSIC;

typedef enum
{
	FLASH_SIGNAL_UDISK_INIT = ((UDISK_GROUP << 8) | 1),
	FLASH_SIGNAL_UDISK_START_UDISK,

	FLASH_SIGNAL_UDISK_MAX
}FLASH_SIGNAL_UDISK;

//=================================================================
// Flash Task events
//=================================================================
typedef enum
{
	// Timer
	FLASH_MSG_TIMER_TICK = 0,
	// TP events
	FLASH_MSG_DRV_TP_DOWN,
	FLASH_MSG_DRV_TP_UP,
	FLASH_MSG_DRV_TP_MOVE,
	FLASH_MSG_DRV_TP_LONG,
	FLASH_MSG_DRV_TP_DOUBLE,
	FLASH_MSG_DRV_TP_DRAG,
	// Key events
	FLASH_MSG_DRV_KEY_DOWN,
	FLASH_MSG_DRV_KEY_UP,
	FLASH_MSG_DRV_KEY_RESERVED1,
	FLASH_MSG_DRV_KEY_SOFT_POWER_ON,
	FLASH_MSG_DRV_KEY_SOFT_POWER_OFF,
	FLASH_MSG_DRV_KEY_RESERVED2,
	
	// Usb events
	FLASH_MSG_DRV_USB,
	// Power events:charge + change frequence
	FLASH_MSG_DRV_POWER,
	// Motion Sensor
	FLASH_MSG_DRV_MOTION_SENSOR,//15


	FLASH_MSG_DRV_EVENT_MAX
}FLASH_DRV_CMD;

typedef enum
{
	FLASH_MSG_PS_CALL = FLASH_MSG_DRV_EVENT_MAX+1,
	FLASH_MSG_PS_CONTACT,
	FLASH_MSG_PS_SMS,
	FLASH_MSG_PS_MMS,//cain	
	FLASH_MSG_PS_PHONE,
	FLASH_MSG_PS_SS,
	FLASH_MSG_PS_SMSCB,
	FLASH_MSG_PS_STK,
	FLASH_MSG_PS_GPRS,
	FLASH_MSG_NOTIFICATION,
	FLASH_MSG_IM,
	FLASH_MSG_SETTING,
	FLASH_MSG_MUSIC,
	FLASH_MSG_REC,
	FLASH_MSG_FM,
	FLASH_MSG_PS_BLUETOOTH,
	FLASH_MSG_ALARM,
	FLASH_MSG_JAVA,//A8_JAVA_LSM modify
	FLASH_MSG_MMS,//A8_MMS_LSM modify
	FLASH_MSG_SCHEDULE,//20
	
	// ....
	FLASH_MSG_PS_MAX	//37
}FLASH_PS_CMD;

typedef enum 
{
	FLASH_MSG_PLAYER_MIN = FLASH_MSG_PS_MAX + 1,
 	FLASH_MSG_PLAYER_OPEN,
	FLASH_MSG_PLAYER_OPEN_ERR,
	FLASH_MSG_PLAYER_DECODE,
	FLASH_MSG_PLAYER_DECODE_ERR,
	FLASH_MSG_PLAYER_EXIT,
	FLASH_MSG_PLAYER_RELOAD,
	//FLASH_MSG_PLAYER_OPEN_UI,
	//FLASH_MSG_PLAYER_OPEN_LOGO,
	FLASH_MSG_PLAYER_PAUSE,
	FLASH_MSG_PLAYER_RESUME,
	FLASH_MSG_PLAYER_MAX
}FLASH_PLAYER_CMD;


typedef enum
{
    MMIFLASH_MSG_MIN = 0x204,
    MMIFLASH_MSG_ENTRY_SCREEN,
	MMIFLASH_MSG_INVOKE_MCARE_LIST,
    MMIFLASH_MSG_MAX
} MMIFLASH_MSG_TYPE_E;

//=================================================================
// Player parameters
//=================================================================
#define FILE_PATH_LENGTH 	(256)

typedef struct
{
	char 		fname[FILE_PATH_LENGTH+1];
	unsigned int	width;
	unsigned int	height;
}FP_Param_t, *FP_Param_p;


#define FILE_DESC_FILE_PATH		0x01
#define FILE_DESC_UNKNOWN_FILE	0x02
#define FILE_DESC_SWF_FILE		0x04
#define FILE_DESC_IMAGE_FILE		0x08
#define FILE_DESC_MEM_BUF		0x10

typedef struct
{
	int     type;
	char *  data;
}file_desc;


//=================================================================
// Flash Task message handler type
//=================================================================
typedef	int (*FlashMsgHandler)(Flash_Signal *signal);


//================================================================
// Event functions
//================================================================

/****************************************************************
 * Function:       FlashTask_PushTPEvent
 * Description:   Push the Touch Panel event to the FLASH TASK event queue
 * Params:
 			@tp_evt: The touch panel event code, it can be FLASH_MSG_DRV_TP_DOWN...
 			@param: The parameters of this event, it's type of FlashTPPress
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
//int FlashTask_PushTPEvent(uint16 tp_evt, uint32 param);
BOOLEAN FlashTask_HandleTPMsg(MMI_MESSAGE_ID_E msg_id, uint32 param);

/****************************************************************
 * Function:       FlashTask_PushKeyEvent
 * Description:   Push the Key event to the FLASH TASK event queue
 * Params:
 			@key_evt: The key event code, it can be FLASH_MSG_DRV_TP_DOWN...
 			@param: The keycode of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
//int FlashTask_PushKeyEvent(uint16 key_evt, uint32 param);
BOOLEAN FlashTask_HandleKeyMsg(MMI_MESSAGE_ID_E msg_id);

/****************************************************************
 * Function:       FlashTask_PushMotionSensorEvent
 * Description:   Push the Motion Sensor event to the FLASH TASK event queue
 * Params:
 			@motion_evt: The motion sensor event code
 			@param: The keycode of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_PushMotionSensorEvent(uint16 motion_evt, uint32 param);


/****************************************************************
 * Function:       FlashTask_PushUSBEvent
 * Description:   Push the USB event to the FLASH TASK event queue
 * Params:
 			@usb_evt: The USB event code
 			@param: The parameters of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_PushUSBEvent(uint16 usb_evt, uint32 param);


/****************************************************************
 * Function:       FlashTask_PushPowerEvent
 * Description:   Push the Power event to the FLASH TASK event queue
 * Params:
 			@charge_evt: The charge event code
 			@param: The parameters of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_PushPowerEvent(uint16 charge_evt, uint32 param);


/****************************************************************
 * Function:       FlashTask_PushPSEvent
 * Description:   Push the PS event to the FLASH TASK event queue
 * Params:
 			@ps_evt: The PS event code, it can be FLASH_MSG_PS_CALL...
 			@ps: The parameters of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_PushPSEvent(uint16 ps_evt, FlashPSMsg *ps);



/****************************************************************
 * Function:       FlashTask_PushNotification
 * Description:   Push the notification event to the FLASH TASK event queue
 * Params:
 			@noti_evt: The notification event code, it can be FLASH_MSG_NOTIFICATION_MSGBOX...
 			@notification: The parameters of this event
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_PushNotification(uint16 noti_evt, FlashNotification *notification);


/****************************************************************
 * Function:       FlashTask_RegisterEventHandler
 * Description:   Register an external event handler
 * Params:
 			@evt: The event code of this function to handle
 			@handler: The handler funtion pointer
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_RegisterEventHandler(uint32 evt, FlashMsgHandler handler);



/****************************************************************
 * Function:       FlashTask_SendSignal
 * Description:   Send the signal to the FLASH TASK
 * Params:
 			@cmd: The signal code
 			@handler: The parameters of this signal
 * Return:        
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_SendSignal(uint16 cmd, uint32 para);
//================================================================

int FlashTask_SendSignalToAPP(uint16 cmd, uint32 para);

//================================================================
// Message handling functions
//================================================================

/****************************************************************
 * Function:       FlashTask_MsgHandle_Init
 * Description:   Initialize the default message handlers
 * Params:	None
 * Return:	
 			0: succeed
 			other: failed
 * Others:         
 ****************************************************************/
int FlashTask_MsgHandle_Init(void);


/****************************************************************
 * Function:       FlashTask_MainLoop
 * Description:   The main loop of the FLASH TASK, it poll and handles all the task signals and events
 * Params:	None
 * Return:	None
 * Others:         
 ****************************************************************/
int FlashTask_MainLoop(void);
//================================================================


#endif // FLASHTASK_MSG_H

