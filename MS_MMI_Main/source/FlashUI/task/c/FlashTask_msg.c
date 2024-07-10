/*
 * FileName: FlashTask_msg.c
 * Copyright (c) 2011 Microrapid Inc.
 * Author:   John <john@Microrapid.com>  Date: 2008.10.29
 * Description:
 * History:
 *     <author>   <time>    <version >   <desc>
 *	  Leonard	 2011.04.19				Add keys map for Mocor
 */

/* =======================================================================
 * Include headers
 * ======================================================================= */  
#include <string.h> 
#include "mmk_msg.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "amuse.h"
#include "amuse_input.h"
#include "amuse_config_device.h"
#include "amuse_file_device.h"
#include "amuse_key_device.h"
#include "FlashTask_main.h"
#include "FlashTask_player.h"
#include "FlashTask_msg.h"
#include "amuse_file_device.h"

#include "dal_chr.h"
#include "upm_api.h"

#include "threadx_os.h"
#include "FlashTask_MAL.h"


/* =======================================================================
 *  Macro and type defines
 * ======================================================================= */
 
#define	REGISTER_DRV_MSG_HANDLER(msg)		sMsgHandlers[FLASH_MSG_DRV_##msg] = on_##msg;
#define	REGISTER_PLAYER_MSG_HANDLER(msg)		sMsgHandlers[FLASH_MSG_##msg] = on_##msg; 
#define	CALL_MSG_HANDLER(sig)	if(sMsgHandlers[sig->SignalCode]){sMsgHandlers[sig->SignalCode](sig);}


#define IS_INPUT_EVENT(id)	((id) <= MSG_T9_PANEL_OPERATION)
#define FLASH_LOCK_KEY			KEY_WEB

/* =======================================================================
 *  External variables
 * ======================================================================= */

/* =======================================================================
 *  Global variables
 * ======================================================================= */ 
static FlashMsgHandler	sMsgHandlers[FLASH_MSG_PLAYER_MAX] = {0};
static FLASHTASK_EVENT_QUEUE sDrvEventQ;//mmk message queue.ID:0~255
static FLASHTASK_EVENT_QUEUE sSysEventQ;//system message queue.ID:
static amuse_input_event s_amuse_input;


/* =======================================================================
 *  Functions implement
 * ======================================================================= */

//=================================================================
// Event Queue utils
//=================================================================
static int FlashTask_EventQ_Init(FLASHTASK_EVENT_QUEUE *eq)
{
	eq->head = eq->tail = 0;
	return 0;
}

static int FlashTask_EventQ_Push(FLASHTASK_EVENT_QUEUE *eq, Flash_Signal *signal)
{	
	eq->msg_queue[eq->tail] = *signal;
	eq->tail++;

	if(eq->tail == MMK_MSG_QUEUE_LEN)
	{
		eq->tail = 0;
	}

	if(eq->tail == eq->head)
	{
		amuse_printf("msg queue overflow!\n");
		eq->head++;
		if(eq->head >= MMK_MSG_QUEUE_LEN)
		{
			eq->head = 0;
		}		
	}

	return 0;
}

static Flash_Signal *FlashTask_EventQ_Pop(FLASHTASK_EVENT_QUEUE *eq)
{
	Flash_Signal *e = &eq->msg_queue[eq->head];
	
	eq->head++;
	if(eq->head >= MMK_MSG_QUEUE_LEN)
	{
		eq->head = 0;
	}
	
	return e;
}

static int FlashTask_EventQ_Empty(FLASHTASK_EVENT_QUEUE *eq)
{
	return (eq->head == eq->tail);
}

static __inline void FlashTask_HandleEventQ(FLASHTASK_EVENT_QUEUE *evtQ)
{
	Flash_Signal *sig;
	
	while(!FlashTask_EventQ_Empty(evtQ))
	{
		sig = FlashTask_EventQ_Pop(evtQ);
		
		CALL_MSG_HANDLER(sig);
	}
}

static int FlashTask_PushSysEvent(Flash_Signal *evt)
{
	int mode = FlashTask_GetMode();
	if (mode == FLASHTASK_MODE_PLAY)
	{
		return FlashTask_EventQ_Push(&sSysEventQ, evt);
	}
	return 0;
}

static int FlashTask_PushDrvEvent(Flash_Signal *evt)
{
	int mode = FlashTask_GetMode();
	if (mode == FLASHTASK_MODE_PLAY)
	{
		return FlashTask_EventQ_Push(&sDrvEventQ, evt);
	}
	return 0;
}

//=================================================================
// Local message handlers
//=================================================================

// Driver messages
static int on_TP_DOWN(Flash_Signal *signal)
{
	s_amuse_input.type = AMUSE_MOUSEMOTION;
	s_amuse_input.mouse.x = signal->msg.tp.x;
	s_amuse_input.mouse.y = signal->msg.tp.y;
	s_amuse_input.mouse.button = AMUSE_MOUSE_BUTTON_LEFT;
	amuse_process_input(&s_amuse_input);
	s_amuse_input.type = AMUSE_MOUSEDOWN;
	amuse_process_input(&s_amuse_input);	
	return 0;
}

static int on_TP_UP(Flash_Signal *signal)
{
	s_amuse_input.type = AMUSE_MOUSEUP;
	s_amuse_input.mouse.x = signal->msg.tp.x;
	s_amuse_input.mouse.y = signal->msg.tp.y;
	s_amuse_input.mouse.button = AMUSE_MOUSE_BUTTON_LEFT;
	amuse_process_input(&s_amuse_input);
	return 0;
}

static int on_TP_MOVE(Flash_Signal *signal)
{
	s_amuse_input.type = AMUSE_MOUSEMOTION;
	s_amuse_input.mouse.x = signal->msg.tp.x;
	s_amuse_input.mouse.y = signal->msg.tp.y;
	s_amuse_input.mouse.button = AMUSE_MOUSE_BUTTON_LEFT;
	amuse_process_input(&s_amuse_input);

	return 0;
}

static int _Key_Proc(const Flash_Signal *signal, const short keytype)
{
	unsigned int keyMapInd;
	int defKey,softKey;
	amuse_input_event input={0};

	if( !is_valid_key(signal->msg.keycode) )
	{
		amuse_printf("_Key_Proc, invalild keycode:%d\n",signal->msg.keycode);
		return 0;
	}

	keyMapInd = get_key_index_from_hard_code(signal->msg.keycode);

	if(key_device_get_item(keyMapInd, &defKey, &softKey ) == NULL)
	{
		amuse_printf("_Key_Proc, invalild keyMapInd:%d\n",keyMapInd);
		return 0;
	}
	
	if( softKey < AMUSE_KEY_LAST )
	{
		input.key.type = keytype;
		input.key.mod = AMUSE_KMOD_NONE;//Normal mode
		input.key.code = (short)softKey;
		amuse_printf("_Key_Proc, softKey:%d,keytype:%d\n",softKey,keytype);
		return amuse_process_input(&input);
	}
	else
	{
		amuse_printf("_Key_Proc, invalild softKey:%d\n",softKey);
		return 0;
	}
}

static int on_KEY_DOWN(Flash_Signal *signal)
{
	//SCI_ASSERT(PNULL != signal);
	if(PNULL == signal)
	{
	   return 0;
	}
	return _Key_Proc(signal, AMUSE_KEYDOWN);
}

static int on_KEY_UP(Flash_Signal *signal)
{
	//SCI_ASSERT(PNULL != signal);
	if(PNULL == signal)
	{
	   return 0;
	}
	return _Key_Proc(signal, AMUSE_KEYUP);
}

static int on_KEY_SOFT_POWER_ON(Flash_Signal *signal)
{
	return 0;
}

static int on_KEY_SOFT_POWER_OFF(Flash_Signal *signal)
{
	return 0;
}

static int on_MOTION_SENSOR(Flash_Signal *signal)
{
	return 0;
}


// Player messages
static uint32 Switch_Flash_Mode(unsigned int mode)
{
	return 0;
}

//载入Flash文件并进行解析
static int on_PLAYER_OPEN(Flash_Signal *signal)
{
	file_desc	fd;
	int ret = 0;
	FP_Param_p fp = (FP_Param_p)signal->msg.param;

	fd.data = fp->fname;
	
	amuse_printf("on_PLAYER_OPEN called,input file=%s\n",fd.data);

	
	fd.type = FILE_DESC_FILE_PATH|FILE_DESC_SWF_FILE;
	if (!fd.data)
	{
		FlashTask_SendSignal(FLASH_MSG_PLAYER_OPEN_ERR,0);
		return 0;
	}

	FlashTask_EventQ_Init(&sDrvEventQ);
	FlashTask_SetMode(FLASHTASK_MODE_PLAY);
	ret = Flash_playfile_init(fd.data, fp->width, fp->height);
	if(ret == 0)
	{	
		FlashTask_MAL_Init();
		FlashTask_MAL_RegisterEvtHandler();
		FlashTask_MAL_RegisterASAPI();
		FlashTask_SendSignal(FLASH_MSG_PLAYER_DECODE,0);
	}
	else
	{
		FlashTask_RestoreFreq();
		FlashTask_SetMode(FLASHTASK_MODE_ERROR);
		FlashTask_SendSignal(FLASH_MSG_PLAYER_OPEN_ERR,0);
	}

	return ret;
}


static int on_PLAYER_OPEN_ERR(Flash_Signal *signal)
{
	Flash_playfile_destroy(0);
	amuse_printf("open error!\n");
	return 0;
}

//进行Flash解码以及走帧
static int on_PLAYER_DECODE(Flash_Signal *signal)
{
	int ret = 0;

	if( FLASHTASK_MODE_PLAY == FlashTask_GetMode())//add by Leonard 2011.05.20
	{
		amuse_printf("on_PLAYER_DECODE, once\n");
		//deal with driver event
		FlashTask_HandleEventQ(&sDrvEventQ);
		
		// deal with system event	
		FlashTask_HandleEventQ(&sSysEventQ);
		

		// run the decode routine
		ret = Flash_playfile_decode();
		switch(ret)
		{
			case AMUSE_FRAME_NEXT:
				FlashTask_SendSignal(FLASH_MSG_PLAYER_DECODE,0);
				break;
			case AMUSE_FRAME_EXIT:
				FlashTask_SendSignal(FLASH_MSG_PLAYER_EXIT,0);
				break;
			case AMUSE_FRAME_FAILED:
				FlashTask_SendSignal(FLASH_MSG_PLAYER_DECODE_ERR,0);
				break;
			default:
				break;
		}
	}
	return ret;
}

static int on_PLAYER_PAUSE(Flash_Signal *signal)
{
	/*lint -e(746)*/
	amuse_player_pause();
	FlashTask_SetMode(FLASHTASK_MODE_PAUSE);
	return 0;
}

static int on_PLAYER_RESUME(Flash_Signal *signal)
{
	/*lint -e(746)*/
	amuse_player_resume();
	FlashTask_SetMode(FLASHTASK_MODE_PLAY);
	FlashTask_SendSignal(FLASH_MSG_PLAYER_DECODE,0);
	amuse_display_update_rect(0, 0, amuse_screen_width-1, amuse_screen_height-1);
	return 0;
}

static int on_PLAYER_DECODE_ERR(Flash_Signal *signal)
{
	amuse_printf("decode error!\n");
	Flash_playfile_destroy(0);
	FlashTask_SetMode(FLASHTASK_MODE_ERROR);
	return 0;
}

static int on_PLAYER_RELOAD(Flash_Signal *signal)
{
	Flash_playfile_destroy(1);
	FlashTask_SetMode(FLASHTASK_MODE_IDLE);
	FlashTask_StartUI();
	return 0;
}
	
static int on_PLAYER_EXIT(Flash_Signal *signal)
{
	Flash_playfile_destroy(1);
	FlashTask_MAL_Destroy();
	FlashTask_SetMode(FLASHTASK_MODE_IDLE);
	FlashTask_RestoreFreq();
	amuse_print_destroy();
	return 0;
}


//=================================================================
// Exported Functions
//=================================================================
int FlashTask_SendSignal(uint16 cmd, uint32 para)
{
	Flash_Signal *sig_ptr = (Flash_Signal*)SCI_ALLOC(sizeof(Flash_Signal));

	SCI_ASSERT(PNULL != sig_ptr);/*assert verified*/
	sig_ptr->SignalCode = (uint16)cmd;
	sig_ptr->SignalSize = sizeof(Flash_Signal);
	sig_ptr->Sender = 0;
	sig_ptr->msg.param = para;

	return SCI_SendSignal((xSignalHeader)sig_ptr, P_FLASH);
}

int FlashTask_SendSignalToAPP(uint16 cmd, uint32 para)
{
	Flash_Signal *sig_ptr = (Flash_Signal*)SCI_ALLOC(sizeof(Flash_Signal));

	SCI_ASSERT(PNULL != sig_ptr);/*assert verified*/
	sig_ptr->SignalCode = (uint16)cmd;
	sig_ptr->SignalSize = sizeof(Flash_Signal);
	sig_ptr->Sender = P_FLASH;
	sig_ptr->msg.param = para;

	amuse_printf("FlashTask_SendSignalToAPP: SignalCode:%d,param:%d\n",cmd,para);

	return SCI_SendSignal((xSignalHeader)sig_ptr, P_APP);
}

int FlashTask_PushTPEvent(uint16 tp_evt, uint32 param)
{
	Flash_Signal *evt;
	FlashTPPress *tp = (FlashTPPress*)param;

	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = (uint16)tp_evt;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
  	evt->msg.tp = *tp;
	FlashTask_PushDrvEvent(evt);
	SCI_FREE(evt);
	return 0;
}

BOOLEAN FlashTask_HandleTPMsg(MMI_MESSAGE_ID_E msg_id, uint32 param)
{
    BOOLEAN         ret = TRUE;
 	FlashTPPress    tp = {0};

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_DOWN:	
		tp.x= MMK_GET_TP_X(param);
		tp.y = MMK_GET_TP_Y(param);
		FlashTask_PushTPEvent(FLASH_MSG_DRV_TP_DOWN, (uint32)&tp);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_UP:	
		tp.x= MMK_GET_TP_X(param);
		tp.y = MMK_GET_TP_Y(param);
		FlashTask_PushTPEvent(FLASH_MSG_DRV_TP_UP, (uint32)&tp);	
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_TP_PRESS_MOVE:	
		tp.x= MMK_GET_TP_X(param);
		tp.y = MMK_GET_TP_Y(param);
		FlashTask_PushTPEvent(FLASH_MSG_DRV_TP_MOVE, (uint32)&tp);
		break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	default:
	    ret = FALSE;
	    break;
	}
    return ret;
}

int FlashTask_PushKeyEvent(uint16 key_evt, uint32 param)
{
	Flash_Signal *evt;

	amuse_printf("KEY Push Event: %d, keycode = %d\n", key_evt - FLASH_MSG_DRV_KEY_DOWN, param);
	
	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = (uint16)key_evt;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
  	evt->msg.keycode= param;
	FlashTask_PushDrvEvent(evt);
	SCI_FREE(evt);
	//MMIDEFAULT_TurnOnBackLight();
	return 0;
}

BOOLEAN FlashTask_HandleKeyMsg(MMI_MESSAGE_ID_E msg_id)
{
    BOOLEAN ret = TRUE;

    switch(msg_id)
    {
    case MSG_APP_WEB:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_WEB);
		break;
	case MSG_APP_OK:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_OK);
		break;
	case MSG_APP_CANCEL:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_CANCEL);
		break;
	case MSG_APP_UP:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_UP);
		break;
	case MSG_APP_DOWN:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_DOWN);
		break;
	case MSG_APP_LEFT:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_LEFT);
		break;
	case MSG_APP_RIGHT:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_DOWN, (uint32)KEY_RIGHT);
		break;
    case MSG_KEYUP_WEB:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_WEB);
		break;
	case MSG_KEYUP_OK:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_OK);
		break;
	case MSG_KEYUP_CANCEL:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_CANCEL);
		break;
	case MSG_KEYUP_UP:
		amuse_PrintMemoryStat();//查看内存占用情况
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_UP);
		break;
	case MSG_KEYUP_DOWN:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_DOWN);
		break;
	case MSG_KEYUP_LEFT:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_LEFT);
		break;
	case MSG_KEYUP_RIGHT:
		FlashTask_PushKeyEvent(FLASH_MSG_DRV_KEY_UP, (uint32)KEY_RIGHT);
		break;
	default:
	    ret = FALSE;
	    break;
	}
    return ret;
}

int FlashTask_PushMotionSensorEvent(uint16 motion_evt, uint32 param)
{
	Flash_Signal *evt;

	//amuse_printf("KEY Push Event: %d, keycode = %d\n", key_evt - FLASH_MSG_DRV_KEY_DOWN, param);
	
	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = (uint16)motion_evt;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
	FlashTask_PushDrvEvent(evt);
	SCI_FREE(evt);
	//MMIDEFAULT_TurnOnBackLight();
	return 0;
}

int FlashTask_PushUSBEvent(uint16 usb_evt, uint32 param)
{
	Flash_Signal *evt;

	amuse_printf("USB Event: %d\n", usb_evt);
	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = FLASH_MSG_DRV_USB;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
  	evt->msg.param = usb_evt;
	FlashTask_PushSysEvent(evt);
	SCI_FREE(evt);
	return 0;
}

int FlashTask_PushPowerEvent(uint16 charge_evt, uint32 param)
{
	Flash_Signal *evt;

	amuse_printf("power Event: %d\n", charge_evt);
	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = FLASH_MSG_DRV_POWER;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
  	evt->msg.drv.power.m_type = charge_evt;	
	FlashTask_PushSysEvent(evt);
	SCI_FREE(evt);
	return 0;
}

int FlashTask_PushPSEvent(uint16 ps_evt, FlashPSMsg *ps)
{
	Flash_Signal *evt;

	amuse_printf("Push PS Event: event = %d, type = %d\n", ps_evt, ps->phone.m_type);
	
	evt = SCI_ALLOC(sizeof(Flash_Signal) + 16);
	evt->SignalCode = (uint16)ps_evt;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
  	memcpy(&evt->msg.ps, ps, sizeof(FlashPSMsg));
	FlashTask_PushSysEvent(evt);
	SCI_FREE(evt);

	return 0;
}

int FlashTask_PushNotification(uint16 noti_evt, FlashNotification *notification)
{	
	Flash_Signal *evt;
	FlashNotification * noti;
	int len;

	amuse_printf("Push Notification Event: event = %d\n", noti_evt);

	evt = SCI_ALLOC(sizeof(Flash_Signal)+16);
	evt->SignalCode = noti_evt;
	evt->SignalSize = sizeof(Flash_Signal);
	evt->Sender = 0;
	noti = &evt->msg.notification;

	memcpy(noti, notification, sizeof(FlashNotification));
	if (notification->m_title)
	{
		len = notification->title_len;
		noti->m_title = SCI_ALLOC(len+ 4);
		memcpy(noti->m_title, notification->m_title, len);
		noti->m_title[len] = noti->m_title[len+1] = 0;
	}
	else
	{
		noti->m_title = NULL;
	}
	
	if (notification->m_msg)
	{
		len = notification->msg_len;
		noti->m_msg = SCI_ALLOC(len + 4);
		memcpy(noti->m_msg, notification->m_msg, len);
		noti->m_msg[len] = noti->m_msg[len+1] = 0;
	}
	else
	{
		noti->m_msg = NULL;
	}
	FlashTask_PushSysEvent(evt);
	SCI_FREE(evt);
	// NOTE: No need to free title and message buffer here, it will free from the notification handler
	return 0;
}

int FlashTask_RegisterEventHandler(uint32 evt, FlashMsgHandler handler)
{
	if ((evt < FLASH_MSG_PLAYER_MAX) && handler)
	{
		sMsgHandlers[evt] = handler;
		return 0;
	}
	return -1;
}

//Flash事件初始,flash控件消息以及主要按键事件的绑定
int FlashTask_MsgHandle_Init(void)
{
	FlashTask_EventQ_Init(&sSysEventQ);
	FlashTask_EventQ_Init(&sDrvEventQ);
	memset(sMsgHandlers, 0, sizeof(sMsgHandlers));
	
	//Player messages handlers
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_OPEN);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_OPEN_ERR);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_DECODE);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_DECODE_ERR);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_EXIT);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_PAUSE);
	REGISTER_PLAYER_MSG_HANDLER(PLAYER_RESUME);
	//Driver events handlers
	REGISTER_DRV_MSG_HANDLER(TP_DOWN);
	REGISTER_DRV_MSG_HANDLER(TP_UP);
	REGISTER_DRV_MSG_HANDLER(TP_MOVE);
	
	REGISTER_DRV_MSG_HANDLER(KEY_DOWN);
	REGISTER_DRV_MSG_HANDLER(KEY_UP);
	
	REGISTER_DRV_MSG_HANDLER(KEY_SOFT_POWER_ON);
	REGISTER_DRV_MSG_HANDLER(KEY_SOFT_POWER_OFF);
	REGISTER_DRV_MSG_HANDLER(MOTION_SENSOR);

	//打开内存统计，0为关闭默认为关闭。内存统计将耗费一定的性能，正式发布版本建议关闭
	//amuse_PrintMemoryStat();使用此函数打印内存占用数据。
	amuse_EnableHiMemStat(1);

	amuse_print_init();

	return 0;
}

//Flash Task 主循环，用于从Flash Task消息队列中读取消息进行分发
int FlashTask_MainLoop(void)
{
	Flash_Signal *signal = NULL;	

	/*lint -e(716)*/
	while(TRUE)
	{	
		signal = (Flash_Signal*)SCI_GetSignal(P_FLASH);

		if(signal->SignalCode < FLASH_MSG_PLAYER_MAX)
		{
			CALL_MSG_HANDLER(signal);
		}
		else
		{
			SCI_SendSignal((xSignalHeader)signal, P_APP);
		}
		SCI_FREE(signal);
#if 0
		if ((FlashTask_GetPriority() < 31))
		{
			SCI_Sleep(15);
		}
		else
		{
			SCI_Sleep(10);
		}
#endif		
	}	
/*lint -unreachable */
	return 0;
}



/*Edit by script, ignore 3 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527
