#include <time.h>
#include <errno.h>

#include "rs_datatype.h"
#include "rs_system.h"
#include "rs_state.h"
#include "rs_sdk_api.h"
#include "rs_thread.h"
#include "rs_mem.h"
#include "rs_debug.h"
#include "rs_sdk_api_ex.h"
#include "rs_param.h"
#include "rs_notify_user.h"
#include "rs_socket.h"

// 6531相关头文件
#include "os_api.h"
#include "nv_item_id.h"
#include "mn_type.h"
#include "nvitem.h"
#include "version.h"
#include "sci_service.h"
#include "mn_events.h"
#include "sig_code.h"
#include "sci_api.h"
#include "in_message.h"
#include "Dal_time.h"
#include "dal_power.h"


//#include "Mminet_socket.h"

//#define UA_TASK_RUN_SIGNAL_CODE  ((0xF1 << 8) | 1)

BLOCK_ID s_uadl_thread_id = SCI_INVALID_BLOCK_ID;    // 线程ID
#define UADL_OS_THREAD_STACK_SIZE (50*1024)     // 线程占用的堆栈大小，设置不合理可能会引发异常如assert
#define s_uadl_thread_task_priority     41	//线程优先级，20为普通优先级
#define UA_OS_THREAD_QUEUE_NUM    20    //线程消息队列数
#define TIMER_NETWORK (2*60*1000)    

extern void MMINET_DeviceSleepSetFlymode(int app_id);

LOCAL SCI_TIMER_PTR g_ua_network_timer = 0x0;
//LOCAL rs_s32 g_network_cost_time = 0;
LOCAL rs_s32 g_tmp_network_state = 0;
LOCAL rs_s32 g_wait_network_active_mag = 0;
LOCAL rs_s32 auto_check_first = 1;


LOCAL void uaTimerNetworkCallback(unsigned int param);
LOCAL void createNetWorkTimer ();
LOCAL void cancelNetWorkTimer ();
LOCAL void rs_thread_register_event(void);


typedef struct RS_SIG_MSG_tag
{
	xSignalHeaderRec header; 
	void* msgDataEx;
} RS_SIG_MSG;

// 检查下载的执行入口
LOCAL void Ua_Task_Entry(uint32 argc, void *argv)
{
	RS_SIG_MSG *psig;
	unsigned short signalCode;

	rs_thread_register_event();

	while(1)
	{			
		void* msgData = 0;
		psig = SCI_NULL;

		SCI_RECEIVE_SIGNAL((xSignalHeader)psig,  s_uadl_thread_id);	 //阻塞获取，等待定时器发送
		signalCode = psig->header.SignalCode;
		msgData = psig->msgDataEx;

		RS_PORITNG_LOG(RS_LOG_DEBUG"thread recv msg, signalCode = 0X%X\n", signalCode);

		if(UA_TASK_RUN_SIGNAL_CODE == signalCode)
		{
			rs_sys_msg_queue_callback(msgData);
		}
		else if (UA_TASK_AUTO_CHECK_TIMER_COME_SIGNAL_CODE == signalCode)
		{
			SCI_TIME_T  curTime = {0};
			RS_FWDL_STATE state = rs_sdk_getDLCurrentState();
			rs_s32 timerPeriod = 0;
			rs_s32 interCycle = 0;
			rs_s32 handle = rs_sys_get_autoruntimer_handle();
			rs_sys_autocheck_timer_cancel(handle);

			interCycle = rs_reg_get_internal_cycle();
			timerPeriod = rs_cb_get_auto_check_cycle();
			if (interCycle != 0)
				timerPeriod = interCycle;
			rs_sys_autocheck_timer_start(timerPeriod);

			RS_PORITNG_LOG(RS_LOG_DEBUG"UA_TASK_AUTO_CHECK_TIMER_COME_SIGNAL_CODE,state = %d", state);

			if (RS_FWDL_STATE_DOWNLOADED == state)
			{
				// 直接安装
				rs_sdk_autoCheck();
			}
			else
			{
				if (rs_system_ppp_active_state())
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_autocheck_callback, network can use\n");
					rs_sdk_autoCheck();
				}
				else
				{
					if (rs_system_is_ppp_attached())
					{
						if(rs_system_ppp_active())
						{
							g_wait_network_active_mag = 1;
							createNetWorkTimer();
						}
						else
						{
							RS_PORITNG_LOG(RS_LOG_DEBUG"active fail\n");
						}
					}
					else
					{
						if(rs_system_ppp_attach())
						{
							g_wait_network_active_mag = 1;
							createNetWorkTimer();
						}
						else
						{
							RS_PORITNG_LOG(RS_LOG_DEBUG"attach fail\n");
						}
					}
				}
			}
		}
		else if(UA_TASK_CHECK_SIGNAL_CODE == signalCode)
		{
			rs_sdk_check_ex(1);                                
		}
		else if(UA_TASK_DOWNLOAD_SIGNAL_CODE == signalCode)
		{
			rs_sdk_download_ex(1);                              
		}
		else if(UA_TASK_INSTALL_SIGNAL_CODE == signalCode)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"recv install update package command \n");
			rs_sdk_install_ex();                              
		}
		/*
		else if(UA_TASK_CHECK_NETWORK_SIGNAL_CODE == signalCode)
		{
			createNetWorkTimer();
			g_wait_network_active_mag = 1;
		}*/
		else if(UA_TASK_NETWORK_CAN_USE_SIGNAL_CODE == signalCode)
		{	
			if (g_wait_network_active_mag == 1)
			{
				g_wait_network_active_mag = 0;
				cancelNetWorkTimer();
				rs_sdk_autoCheck();
			}
			else
			{
				RS_PORITNG_LOG(RS_LOG_DEBUG"recv network active message, but no need deal\n");
			}
		}
		else if(UA_TASK_CHECK_NETWORK_TIMER_COME_SIGNAL_CODE == signalCode)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"wait pdp message timeout\n");
			g_wait_network_active_mag = 0;
			rs_system_ppp_deactive();
			rs_system_ppp_deattach();
			//MMINET_DeviceSleepSetFlymode(2);	
		}
		/*
		else if(UA_TASK_CHANGE_AUTOCHECK_TIMER_SIGNAL_CODE == signalCode)
		{
			rs_s32 timerPeriod = 0;
			rs_s32 interCycle = 0;
			rs_s32 handle = rs_sys_get_autoruntimer_handle();
			rs_sys_autocheck_timer_cancel(handle);

			interCycle = rs_reg_get_internal_cycle();
			timerPeriod = rs_cb_get_auto_check_cycle();
			if (interCycle != 0)
				timerPeriod = interCycle;
			rs_sys_autocheck_timer_start(timerPeriod);
		}*/
#ifdef SUPPORT_USER_MSG
		else if (UA_TASK_USER_MSG_SIGNAL_CODE == signalCode)
		{
			rs_user_msg_queue_callback(msgData);
		}
#endif
		else if(SOCKET_CONNECT_EVENT_IND == signalCode)
		{
			fota_event_handle(psig, RS_SOCKET_MSG_CONNECT);
		}
		else if(SOCKET_READ_EVENT_IND == signalCode)
		{
			fota_event_handle(psig, RS_SOCKET_MSG_READ);
		}
		else if(SOCKET_WRITE_EVENT_IND == signalCode)
		{
			fota_event_handle(psig, RS_SOCKET_MSG_WRITE);
		}
		else if(SOCKET_CONNECTION_CLOSE_EVENT_IND == signalCode)
		{
			fota_event_handle(psig, RS_SOCKET_MSG_CLOSE);
		}
		else if(SOCKET_ASYNC_GETHOSTBYNAME_CNF == signalCode)
		{
			fota_get_ip_handle(psig);
		}
		else if(APP_MN_ACTIVATE_PDP_CONTEXT_CNF == signalCode)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"recv APP_MN_ACTIVATE_PDP_CONTEXT_CNF");
			if (g_wait_network_active_mag == 1)
			{
				g_wait_network_active_mag = 0;
				cancelNetWorkTimer();
				
				if((((APP_MN_GPRS_EXT_T*)psig)->result >= MN_GPRS_ERR_SUCCESS) && (((APP_MN_GPRS_EXT_T*)psig)->result <= MN_GPRS_ERR_SUCCESS_SINGLE_ONLY))
				{
					APP_MN_GPRS_EXT_T*param_ptr = (APP_MN_GPRS_EXT_T *)psig;
					uint8 nsapi = 5;
					uint8 pdp_id = 1;
			
					nsapi = param_ptr->nsapi;
					pdp_id = param_ptr->pdp_id;

					rs_socket_set_netid(nsapi);
					rs_sdk_autoCheck();
				}
				else
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"recv network active fail message\n");
				}
			}
			else
			{
				RS_PORITNG_LOG(RS_LOG_DEBUG"recv network active message 2, but no need deal\n");
			}
		}
		else if(APP_MN_GPRS_ATTACH_RESULT == signalCode)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"recv APP_MN_GPRS_ATTACH_RESULT");
			if (g_wait_network_active_mag == 1)
			{
				APP_MN_GPRS_ATTACH_RESULT_T*param_ptr = (APP_MN_GPRS_ATTACH_RESULT_T *)psig;
				
				g_wait_network_active_mag = 0;
				cancelNetWorkTimer();
				
				if(param_ptr->is_attach_ok) 
				{
					if(rs_system_ppp_active())
					{
						g_wait_network_active_mag = 1;
						createNetWorkTimer();
					}
				}
				else
				{
					RS_PORITNG_LOG(RS_LOG_DEBUG"recv network attach fail message\n");
				}
			}
			else
			{
				RS_PORITNG_LOG(RS_LOG_DEBUG"recv network active message 2, but no need deal\n");
			}			
		}
		else
		{
			//RS_PORITNG_LOG(RS_LOG_DEBUG"thread recv msg,  but not support \n");
		}

		SCI_FREE_SIGNAL(psig);
	}
}

rs_s32  rs_create_thread()
{
	s_uadl_thread_id = SCI_INVALID_BLOCK_ID;
	s_uadl_thread_id = SCI_CreateAppThread("T_UA_DL", "Q_RS_DL", Ua_Task_Entry, 0, NULL, UADL_OS_THREAD_STACK_SIZE, 
		UA_OS_THREAD_QUEUE_NUM, s_uadl_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);

	if(SCI_INVALID_BLOCK_ID == s_uadl_thread_id)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"create app thread failed\n");
	}
}


void rs_post_message_to_thread_with_code(unsigned short signalCode,  void* msgData)
{
	RS_SIG_MSG* sig_ptr;
	BLOCK_ID dest_id;
	uint32 status;

	if(SCI_INVALID_BLOCK_ID == s_uadl_thread_id)
	{
		//RS_PORITNG_LOG(RS_LOG_DEBUG"message thread not exit\n");
		return;
	}

	// 创建消息，并赋值
	sig_ptr = SCI_ALLOC(sizeof(RS_SIG_MSG));
	SCI_ASSERT(SCI_NULL != sig_ptr);
	sig_ptr->header.SignalCode = signalCode;
	sig_ptr->header.SignalSize = sizeof(RS_SIG_MSG);
	sig_ptr->header.Sender = 0;
	sig_ptr->msgDataEx = msgData;
	/*
	if(SCI_SUCCESS != SCI_IsThreadQueueAvilable(s_uadl_thread_id))
	{
	RS_PORITNG_LOG(RS_LOG_DEBUG"ua timer send signalCode failed signalCode = %d,queue is full", signalCode);	
	return;
	}
	*/
	// 发送消息
	status = SCI_SendSignal((xSignalHeader)sig_ptr, s_uadl_thread_id);
	if (SCI_SUCCESS != status)
	{
		// 发送失败，进行出错处理
		//RS_PORITNG_LOG(RS_LOG_DEBUG"send msg fail, code= %d", signalCode);	
	}

}

rs_s32  tmp_NetworkCanUse()
{
	if (g_tmp_network_state == 0)
		return FALSE;
	else
		return TRUE;
}

// 定时器回调函数
LOCAL void uaTimerNetworkCallback(unsigned int param)
{
	 //RS_PORITNG_LOG(RS_LOG_DEBUG"network timer come");
        rs_post_message_to_thread_with_code(UA_TASK_CHECK_NETWORK_TIMER_COME_SIGNAL_CODE,  0);

	
	// #############################
	/*if(Remo_IsPdpActive() )
	{
		// 可能和auto check存在并发发送此消息
		rs_post_message_to_thread_with_code(UA_TASK_NETWORK_CAN_USE_SIGNAL_CODE,  0);
	}*/
	/*
	else
	{
		g_network_cost_time += TIMER_NETWORK; // 单位s
		if(g_network_cost_time >  rs_cb_get_auto_check_cycle() - 60* 1000) // 这个周期要小于auto check的周期，否则存在并发现象
		{
			rs_post_message_to_thread_with_code(UA_TASK_CHECK_NETWORK_TIMER_COME_SIGNAL_CODE,  0);
		}
	}*/
}


// 创建定时器
LOCAL void createNetWorkTimer ()
{
	//创建周期性定时器，该定时器自动激活
	unsigned int timer = TIMER_NETWORK;	
	char timer_name[8]={'R','S','N','T','W','K','0',0};

	cancelNetWorkTimer();

	if (g_ua_network_timer == 0)
	{
		// 创建一个周期性的定时器。当定时器时间到时，调用超时回调函数timer_fun，该定时器自动重新开始计时。
		g_ua_network_timer =  SCI_CreatePeriodTimer(timer_name,	uaTimerNetworkCallback, 0, 	timer, 0);
		if(0x0 == g_ua_network_timer)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"create network timer fail\n");
			return;
		}

		if(SCI_ActiveTimer((SCI_TIMER_PTR)g_ua_network_timer) != SCI_SUCCESS)
		{
			return;
		}
	}
	else
	{
		if(SCI_ChangeTimer((SCI_TIMER_PTR)g_ua_network_timer, uaTimerNetworkCallback, timer) != SCI_SUCCESS)
		{
			return;
		}
		
		if(SCI_ActiveTimer((SCI_TIMER_PTR)g_ua_network_timer) != SCI_SUCCESS)
		{
			return;
		}		
	}
}


LOCAL void cancelNetWorkTimer ()
{
	if(0x0 == g_ua_network_timer) 
		return;

	if(SCI_IsTimerActive(g_ua_network_timer))
	{
		SCI_DeactiveTimer(g_ua_network_timer);
	}
	//SCI_DeleteTimer(g_ua_network_timer);

	//g_network_cost_time = 0;

	return;	
}


BLOCK_ID rs_get_thread_id()
{
	return s_uadl_thread_id;
}



LOCAL void rs_thread_register_event(void)
{
    POWER_RESTART_CONDITION_E   start_condition = POWER_GetRestartCondition();

    if ((RESTART_BY_ALARM != start_condition && RESTART_BY_CHARGE != start_condition)
        || 1 == CLASSMARK_GetModelType() )
	{
	        //Register phone event
	        SCI_RegisterMsg( MN_APP_PHONE_SERVICE, 
	                                (uint8)EV_MN_APP_NETWORK_STATUS_IND_F, 
	                                (uint8)(MAX_MN_APP_PHONE_EVENTS_NUM -1), 
	                                SCI_NULL );

	        SCI_RegisterMsg(MN_APP_GPRS_SERVICE,
	                                (uint8)EV_MN_APP_SET_PDP_CONTEXT_CNF_F,
	                                (uint8)(MAX_MN_APP_GPRS_EVENTS_NUM - 1),
	                                SCI_NULL); 
	}

}