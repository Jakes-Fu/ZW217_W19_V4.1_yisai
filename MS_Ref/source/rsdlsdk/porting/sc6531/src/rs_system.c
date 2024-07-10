#include <time.h>
#include <errno.h>

#include "rs_datatype.h"
#include "rs_system.h"
#include "rs_state.h"
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_thread.h"
#include "rs_mem.h"
#include "rs_md5.h"
#include "rs_fs.h"
#include "rs_param.h"
#include "rs_debug.h"

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
#include "atc.h"

//#include "Mminet_socket.h"
//#include "MQTTClient.h"
//#include "Mmipdp_export.h"
//#include "Mmk_authen.h"

void * g_msg_handle = RS_NULL;
static const rs_s8 dec2hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

LOCAL SCI_TIMER_PTR short_timer = RS_NULL;
LOCAL SCI_TIMER_PTR auto_check_timer = RS_NULL;
LOCAL SCI_TIMER_PTR auto_check_timer1 = RS_NULL;


extern PUBLIC BOOLEAN MMINETAPI_ConnectGPRS();
//extern void MMIAPINET_MqttSendSignal(NET_STATE_SOCKET msg_Signal, void *param);
extern PUBLIC BOOLEAN MMIAPIPDP_PsIsPdpLinkExist(void);
extern void MMINET_DeviceCloseFlymode(int app_id);

#define RS_OTA_CID 7

typedef struct 
{
	RS_MSG_HANDLER_CALLBACK callback;
	rs_u32 uMsgEvent;
	rs_s32 lParam1;
	rs_s32 lParam2;
} RS_MMI_Msg;

typedef struct RS_TIMER_CALLBACK_DATA_tag
{
	RS_TIMER_CALLBACK callback;
	void* data;
}RS_TIMER_CALLBACK_DATA;

RS_TIMER_CALLBACK_DATA short_timer_callback = {0};
void rs_sdktimer_callback(unsigned int param);
void rs_sys_autocheck_callback(unsigned int param);

void rs_sys_msg_queue_callback(void* msgData)
{
	if(msgData != 0)
	{
		RS_MMI_Msg* data;

		data = (RS_MMI_Msg *)msgData;
		if (data != RS_NULL && g_msg_handle != RS_NULL)
		{
			data->callback(g_msg_handle, data->uMsgEvent, data->lParam1, data->lParam2);
			rs_free(data);
		}
	}
}

/******************************************************************************************************/
/*
函数说明：系统相关功能初始化
参数说明：无
返回值：成功返回RS_ERR_OK
*/
rs_s32 rs_sys_init()
{
	int errCode = RS_ERR_OK;

	rs_create_thread();

	return errCode;
}

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time(rs_s8* buf)
{
	rs_u32 tickCount = SCI_GetTickCount();
	rs_sprintf(buf, "%d ", tickCount);

}

/*
函数说明：获取当前系统的时间
参数说明：buf 返回当前的系统的时间
返回值：
*/
void rs_sys_get_time_ex(rs_s8* buf)
{
	rs_u32 tickCount = SCI_GetTickCount();
	rs_sprintf(buf, "%d ", tickCount);
}

/*
函数说明：重启进入升级模式
特别说明：在其他的平台需要实现这个接口，windows模拟环境不需要实现
*/
void rs_sys_reboot()
{
	// 重启进入升级模式
	POWER_Reset();
	RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_reboot ..");
}

/**
函数说明：向消息接收模块发送消息
参数说明：nMsgQParam 消息句柄
uMsgEvent 消息事件
lParam1 消息参数1
lParam2 消息参数2
callback 消息接收模块收到消息之后，调用此接口处理消息
返回值：0表示成功，-1表示失败 
*/
rs_bool rs_sys_msg_queue_send(rs_s32 nMsgQParam, rs_u32 uMsgEvent, rs_s32 lParam1, rs_s32 lParam2, RS_MSG_HANDLER_CALLBACK callback)
{
	rs_bool ret = RS_TRUE;
	RS_MMI_Msg * msg;

	msg = (RS_MMI_Msg*)rs_malloc(sizeof(RS_MMI_Msg));
	if (msg == RS_NULL)
	{
		//RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_msg_queue_send malloc fail\n");
		return RS_FALSE;
	}

	msg->callback = callback;
	msg->uMsgEvent = uMsgEvent;
	msg->lParam1 = lParam1;
	msg->lParam2 = lParam2;

	rs_post_message_to_thread_with_code(UA_TASK_RUN_SIGNAL_CODE, msg);

	return ret;
}

/**
函数说明：输入日志到文件或者其他
参数说明：需要输出的日志内容
返回值：无
*/
void rs_cb_printLog(const rs_s8* msg)
{
	if(0 == msg) 
		return;

	SCI_TRACE_LOW(msg); 
	//FT_AT_Trace(msg);
}
/**
函数说明：创建消息队列 
参数说明：userHandle 使用者的句柄
返回值：RS_NULL表示失败，否则为成功
*/
rs_s32 rs_sys_msg_queue_create(void* userHandle)
{
	g_msg_handle = userHandle;
	return 0;
}

void rs_sdktimer_callback(unsigned int param)
{
/*
	if(param != 0)
	{
		RS_TIMER_CALLBACK_DATA* data;

		data = (RS_TIMER_CALLBACK_DATA *)param;
		if (data != RS_NULL)
		{
			data->callback(data->data);
			rs_free(data);
		}
	}	
	*/

	if (short_timer_callback.callback != RS_NULL)
	{
		short_timer_callback.callback(short_timer_callback.data);

		short_timer_callback.callback = RS_NULL;
		short_timer_callback.data = RS_NULL;
	}
}


/**
函数说明：启动定时器
参数说明：timerPeriod消息时间
返回值：返回定时器句柄，非0值表示成功，否则为失败 
*/
rs_s32 rs_sys_timer_start(rs_s32 timerPeriod, RS_TIMER_CALLBACK callback, void* data)
{
	SCI_TIMER_PTR ua_timer;
	/*
	RS_TIMER_CALLBACK_DATA *timerCallback;

	timerCallback = (RS_TIMER_CALLBACK_DATA*)rs_malloc(sizeof(RS_TIMER_CALLBACK_DATA));
	if (timerCallback == RS_NULL)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_timer_start malloc fail");
		return 0;
	}
	timerCallback->callback = callback;
	timerCallback->data = data;
*/
	
	short_timer_callback.callback = callback;
	short_timer_callback.data = data;
	

/*
	ua_timer =  SCI_CreatePeriodTimer("rssdktimer", rs_sdktimer_callback, (rs_u32)timerCallback, timerPeriod, SCI_AUTO_ACTIVATE); 
	if(0x0 == ua_timer)
	{
		RS_PORITNG_LOG(RS_LOG_DEBUG"create sdk short timer failed\n");
	}
*/
	if (short_timer == RS_NULL)
	{
		short_timer =  SCI_CreatePeriodTimer("rssdktimer", rs_sdktimer_callback, 0,  timerPeriod, 0); 
		if(0x0 == short_timer)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"create sdk short timer failed\n");
			return 0;
		}

		if(SCI_ActiveTimer((SCI_TIMER_PTR)short_timer) != SCI_SUCCESS)
		{
		        RS_PORITNG_LOG(RS_LOG_DEBUG"active sdk short  timer failed");
			return 0;
		}
	}
	else
	{
		if(SCI_ChangeTimer((SCI_TIMER_PTR)short_timer, rs_sdktimer_callback, timerPeriod) != SCI_SUCCESS)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"SCI_ChangeTimer failed\n");
			return 0;
		}
		
		if(SCI_ActiveTimer((SCI_TIMER_PTR)short_timer) != SCI_SUCCESS)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"SCI_ActiveTimer failed\n");
			return 0;
		}
	}

	return (rs_s32)short_timer;
}

/**
函数说明：取消定时器
参数说明：nTimerParam定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_timer_cancel(rs_s32 handle)
{
	rs_bool ret = RS_TRUE;
	SCI_TIMER_PTR ua_timer = (SCI_TIMER_PTR)handle;

	//RS_PORITNG_LOG(RS_LOG_DEBUG"cancel timer handle = %d\n", handle);

	if(0x0 == ua_timer) 
		return ret;

	if(SCI_IsTimerActive(ua_timer))
	{
		SCI_DeactiveTimer(ua_timer);
	}
	//SCI_DeleteTimer(ua_timer);

	if (short_timer_callback.callback != RS_NULL)
	{
		short_timer_callback.callback = RS_NULL;
		short_timer_callback.data = RS_NULL;
	}

	return ret;
}

/*
函数说明：自动检测周期到达，执行自动检测
参数说明：
返回值：
注意: 定时器的会掉函数最好能够在0.1s内完成，否则可能有问题
*/
void rs_sys_autocheck_callback(unsigned int param)
{
	rs_post_message_to_thread_with_code(UA_TASK_AUTO_CHECK_TIMER_COME_SIGNAL_CODE, 0);
}

/**
函数说明：启动主程序
参数说明：
返回值：rs_true表示成功，rs_false表示失败 
*/
/*根据不同的平台做不同的启动处理*/
rs_s32 rs_sys_autocheck_timer_start(rs_s32 timerPeriod)
{
	RS_PORITNG_LOG(RS_LOG_DEBUG"rs_sys_autocheck_timer_start, enter,  timerPeriod = %d\n", timerPeriod);
	if (auto_check_timer == RS_NULL)
	{
		auto_check_timer =  SCI_CreatePeriodTimer("autoruntimer", rs_sys_autocheck_callback, 0, timerPeriod, 1); 
		if(0x0 == auto_check_timer)
		{
			RS_PORITNG_LOG(RS_LOG_DEBUG"create auto check timer failed");
			return 0;
		}

		if(SCI_ActiveTimer((SCI_TIMER_PTR)auto_check_timer) != SCI_SUCCESS)
		{
		        RS_PORITNG_LOG(RS_LOG_DEBUG"active auto check timer failed");
			return 0;
		}
	}
	else
	{
		if(SCI_ChangeTimer((SCI_TIMER_PTR)auto_check_timer, rs_sys_autocheck_callback, timerPeriod) != SCI_SUCCESS)
		{
                        RS_PORITNG_LOG(RS_LOG_DEBUG"change auto check timer failed");
                        return 0;
		}
		
		if(SCI_ActiveTimer((SCI_TIMER_PTR)auto_check_timer) != SCI_SUCCESS)
		{
		        RS_PORITNG_LOG(RS_LOG_DEBUG"active auto check timer failed");
			return 0;
		}
	}

	return (rs_s32)auto_check_timer;
}


/**
函数说明：获取autoruntimer的handle
参数说明：
返回值：timer的handle
*/
rs_s32 rs_sys_get_autoruntimer_handle()
{
	return (rs_s32)auto_check_timer;
}

/**
函数说明：取消自动检测定时器
参数说明：handle定时器句柄 
返回值：rs_true表示成功，rs_false表示失败 
*/
rs_bool rs_sys_autocheck_timer_cancel(rs_s32 handle)
{
	rs_bool ret = RS_TRUE;
	SCI_TIMER_PTR ua_timer = (SCI_TIMER_PTR)handle;

	if(0x0 == ua_timer) 
		return ret;

	if(SCI_IsTimerActive(ua_timer))
	{
		SCI_DeactiveTimer(ua_timer);
	}
	//SCI_DeleteTimer(ua_timer);

	return ret;

}


/**
函数说明：判断SIM是否已经插入
参数说明：无
返回值：RS_TRUE表示有效，RS_FALSE表示无效 
*/
rs_bool rs_sys_simcard_insert()
{
	return RS_TRUE;
}

/**
函数说明：判断SIM是否可以识别
参数说明：无
返回值：RS_TRUE表示可以识别，RS_FALSE表示不能识别
*/
rs_bool rs_sys_simcard_recognize()
{
	return RS_TRUE;
}

/**
函数说明：判断imei是否已经写入
参数说明：无
返回值：RS_TRUE表示已经写入，RS_FALSE表示没有写入
*/
rs_bool rs_sys_imei_valid()
{
	return RS_TRUE;
}

/**
函数说明：获取已经下载差分升级包的md5值
参数说明：deltaSize 升级包的长度
md5String 升级包的md5的值
返回值：RS_TRUE表示成功，RS_FALSE表示失败 
*/
rs_bool rs_sys_get_updatefile_md5_string(rs_s32 deltaSize, rs_s8* md5String)
{
	RS_MD5_CTX md5Ctx;
	rs_u8 buf[PAGE_SIZE];
	rs_s32 readSize = 0;
	unsigned char digest[16] = {0};

#ifdef RS_SUPPORT_UPDATE_DATA_FS
	RS_FILE fileHandle = RS_FS_INVALID;
	const rs_s8* filePath = rs_device_get_update_file();

	RS_MD5Init (&md5Ctx);
	fileHandle = rs_fs_open(filePath, RS_FS_OPEN_READ);
	if (fileHandle != RS_FS_INVALID)
	{
		rs_s32 fileSize = rs_fs_size(fileHandle);

		rs_s32 i = 0;

		if (deltaSize > fileSize)
			return RS_FALSE;

		while(readSize < fileSize)
		{
			rs_s32 len = 0;
			len = rs_fs_read(fileHandle, buf, PAGE_SIZE);
			if (len < 0)
				return RS_FALSE;

			RS_MD5Update (&md5Ctx, buf, len);

			readSize += len;
		}

		RS_MD5Final (&md5Ctx);
		rs_fs_close(fileHandle);

		for (i=0; i<16; i++) 
		{
			md5String[i * 2] = dec2hex[(md5Ctx.digest[i] & 0xF0) >> 4];
			md5String[i * 2 + 1] = dec2hex[md5Ctx.digest[i] & 0x0F];
		}
		md5String[32] = 0;
	}
	else
	{
		return RS_FALSE;
	}

	return RS_TRUE;
#else
	rs_u32 flashAddr = 0;
	rs_s32 i = 0;

	RS_MD5Init (&md5Ctx);
	flashAddr = rs_device_get_update_addr();

	while(readSize < deltaSize)
	{
		rs_s32 ret = 0;
		ret = rs_flash_readPage(flashAddr + readSize, buf, PAGE_SIZE);
		if (ret != RS_ERR_OK)
			return RS_FALSE;

		if (deltaSize - readSize > PAGE_SIZE)
		{
			RS_MD5Update (&md5Ctx, buf, PAGE_SIZE);
			readSize += PAGE_SIZE;
		}
		else
		{
			RS_MD5Update (&md5Ctx, buf, deltaSize - readSize);
			readSize = deltaSize;
		}
	}

	RS_MD5Final (&md5Ctx);

	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5Ctx.digest[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5Ctx.digest[i] & 0x0F];
	}
	md5String[32] = 0;

	return RS_TRUE;
#endif
}

rs_bool rs_sys_get_data_md5_string(rs_u8* src_data, rs_s32 data_size, rs_s8* md5String)
{
	RS_MD5_CTX md5Ctx;
	rs_s32 i = 0;

	RS_MD5Init (&md5Ctx);
	RS_MD5Update (&md5Ctx, src_data, data_size);
	RS_MD5Final (&md5Ctx);

	for (i=0; i<16; i++) 
	{
		md5String[i * 2] = dec2hex[(md5Ctx.digest[i] & 0xF0) >> 4];
		md5String[i * 2 + 1] = dec2hex[md5Ctx.digest[i] & 0x0F];
	}
	md5String[32] = 0;

	return RS_TRUE;	
}

rs_u32 rs_sys_get_pid()
{
	return 0;
}


/**
函数说明：读取外部配置文件debug.ini中的内容
返回结果作为系统的日志级别
文件地址在rs_param.h中RS_CONFIG_DEBUGINFO_FILE定义
参数说明：
返回值：返回数字1-5
*/
rs_u32 rs_sys_get_log_level()
{
	rs_u8 buf[10];
	RS_FILE rs_file = -1;
	static rs_u32 log_level = 5;
	static int log_level_unknow = 1;

	/*
	if(log_level_unknow == 0)
	{
		return log_level;
	}
	
	log_level_unknow = 0; //目标是为了以下语句当函数rs_sys_get_log_level第一次被调用时才会执行
	if(rs_fs_exists(RS_CONFIG_DEBUGINFO_FILE))
	{
		rs_file = rs_fs_open(RS_CONFIG_DEBUGINFO_FILE,RS_FS_OPEN_READ);
		if(rs_file != RS_FS_INVALID)
		{//打开成功
			//到文件开始位置
			rs_fs_seek(rs_file,0,RS_FS_SEEK_SET);
			if(rs_fs_read(rs_file,buf,1)>0)
			{
				if(buf[0]>='0' && buf[0]<='9')
				{					
					log_level = buf[0] - 0x30;
				}
			}
			rs_fs_close(rs_file);
		}
	}*/

	return log_level;//数字越高可能的打印的log越多
}


rs_bool rs_system_ppp_active_state()
{
	MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
	uint32 pdp_con_num         = 0;
	BOOLEAN is_active           = FALSE;
	ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
	MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;
	uint32 i = 0;

	ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num);
	if(ret != ERR_MNGPRS_NO_ERR)
	{
		if(pdp_con_state_arr != PNULL)
		{
			SCI_FREE(pdp_con_state_arr);
		}
		
		return RS_FALSE;
	}

	for(i = 0; i < pdp_con_num; i++)
	{
		if(pdp_con_state_arr[i].pdp_state == MN_CONTEXT_DEACTIVATED)
		{
			is_active = FALSE;
		}
		else
		{
			is_active = TRUE;
		}
	}

	if(pdp_con_state_arr != PNULL)
	{
		SCI_FREE(pdp_con_state_arr);
	}

	if (is_active)
	{
		return RS_TRUE;
	}
	else
	{
		return RS_FALSE;
	}
}

rs_bool rs_system_is_ppp_attached()
{
	MN_PHONE_CURRENT_PLMN_INFO_T plmn_info;
	MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;


	plmn_info = MNPHONE_GetCurrentPLMNInfoEx(dual_sys);

	if((PLMN_NORMAL_GPRS_ONLY == plmn_info.plmn_status) ||
		(PLMN_NORMAL_GSM_GPRS_BOTH == plmn_info.plmn_status))
	{
		return RS_TRUE;
	}
	else
	{
		return RS_FALSE;
	}	
}


rs_bool rs_system_ppp_attach()
{
	MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
	ERR_MNPHONE_CODE_E ret = ERR_MNPHONE_NO_ERR;

	ret = MNPHONE_GprsAttachEx(dual_sys, MN_PHONE_GPRS_ATTACH);
	if (ret == ERR_MNPHONE_NO_ERR)
	{
		return RS_TRUE;
	}
	else
	{
		return RS_FALSE;
	}
}


rs_bool rs_system_ppp_deattach()
{
//#define RSFOTA_SUPPORT_GPRS_ATTACH
#ifdef RSFOTA_SUPPORT_GPRS_ATTACH
	MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
	if(ERR_MNPHONE_NO_ERR != MNPHONE_GprsDetachEx(dual_sys, MN_PHONE_GPRS_DETACH))
	{
		return RS_FALSE;
	}
	
	return RS_TRUE;
#else
	return RS_TRUE;
#endif
}

rs_bool rs_system_ppp_active()
{
	MN_DUAL_SYS_E   dual_sys = MN_DUAL_SYS_1;
	MN_IMSI_T          imsi_str;
	rs_s8 buf[20] = {0};
	rs_s8 apn[10] = {0};
	BOOLEAN context_param[7] = {TRUE,TRUE,TRUE,FALSE,FALSE,FALSE,FALSE};
	MN_GPRS_PDP_ADDR_T      pdp_addr = {0}; 
	uint32 pdp_id_array[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};

	imsi_str = MNSIM_GetImsiEx(dual_sys);    
	if(MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
	{
	    ConvertBcdToDigitalStr(imsi_str.imsi_len, imsi_str.imsi_val, buf);
	    buf[5] = 0;
	}

	RS_PORITNG_LOG(RS_LOG_DEBUG"operater id is %s",  buf);
	
	if (rs_strcmp(buf, "46000") == 0 ||
		rs_strcmp(buf, "46002") == 0 ||
		rs_strcmp(buf, "46007") == 0)
	{
		rs_strcpy(apn, "cmnet");
	}
	else if (rs_strcmp(buf, "46001") == 0 ||
		rs_strcmp(buf, "46006") == 0)
	{
		rs_strcpy(apn, "uninet");
	}
	else
	{
		return RS_FALSE;
	}

	if(ERR_MNGPRS_NO_ERR != MNGPRS_SetPdpContextEx(dual_sys, context_param, RS_OTA_CID, (uint8 *)"IP", (uint8 *)apn, pdp_addr, 0, 0, 0))
	{
		return RS_FALSE;
	}

	pdp_id_array[0] = RS_OTA_CID;

	if(ERR_MNGPRS_NO_ERR != MNGPRS_ActivatePdpContextEx(dual_sys, FALSE, pdp_id_array, POC_E, MN_UNSPECIFIED))
	{
		return RS_FALSE;
	}

	return RS_TRUE;
}

rs_bool rs_system_ppp_deactive()
{
	MN_GPRS_CON_STATE_T *pdp_con_state_arr  = PNULL;
	uint32 pdp_con_num         = 0;
	BOOLEAN is_active           = FALSE;
	ERR_MNGPRS_CODE_E ret = ERR_MNGPRS_NO_ERR;
	MN_DUAL_SYS_E  dual_sys = MN_DUAL_SYS_1;
	uint32 pdp_id_array[MN_GPRS_MAX_PDP_CONTEXT_COUNT] = {0};
	uint32 i = 0;
	
	ret = MNGPRS_ReadPdpContextStateEx(dual_sys, &pdp_con_state_arr, &pdp_con_num);
	if(ret != ERR_MNGPRS_NO_ERR)
	{
		if(pdp_con_state_arr != PNULL)
		{
			SCI_FREE(pdp_con_state_arr);
		}

		return RS_FALSE;
	}

	for(i = 0; i < pdp_con_num; i++)
	{
		if(pdp_con_state_arr[i].pdp_state == MN_CONTEXT_DEACTIVATED)
		{
			is_active = FALSE;
		}
		else
		{
			if (pdp_con_state_arr[i].pdp_id == RS_OTA_CID)
				is_active = TRUE;
		}
	}

	if(pdp_con_state_arr != PNULL)
	{
		SCI_FREE(pdp_con_state_arr);
	}
	
	pdp_id_array[0] = RS_OTA_CID;
	ret = MNGPRS_DeactivatePdpContextEx(dual_sys, FALSE, pdp_id_array);
	if(ERR_MNGPRS_NO_ERR != ret)
		return FALSE;
	else
		return TRUE;
}


