#include "rs_notify_user.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rs_sdk_api.h"
#include "rs_error.h"
#include "rs_thread.h"
// 6531���ͷ�ļ�
#include "os_api.h"
#include "nv_item_id.h"
#include "mn_type.h"
#include "nvitem.h"
#include "version.h"
#include "sci_service.h"
#include "mn_events.h"
#include "sig_code.h"

#include "rs_debug.h"
#include "rs_param.h"
#include "rs_sdk_api_ex.h"
#include "rs_notify_user.h"

//#include "MQTTClient.h"


/****************************ȫ�ֱ������ߺ�������**************************/
extern void* rs_malloc(unsigned int allocSize);
extern void rs_free(void* memBlock);
extern void rs_mem_info();
/****************************�ֲ�����*************************************/


typedef struct 
{
	rs_s32 evt_id;
	rs_s32 param1;
	rs_s32 param2;
	rs_s32 exErrorCode;
} RS_EVENT_MSG;

void rs_user_msg_queue_callback(void* evt)
{
	rs_s32 ret = 0;
	rs_s32 param1 = 0;
	rs_s32 param2 = 0;
	rs_s32 exErrorCode = 0;
	rs_s32 msgID = 0;
	RS_EVENT_MSG* userData = (RS_EVENT_MSG*)evt;
	rs_u32 errCode = rs_sdk_getLastError();
	//HWND hWnd = rs_get_window_handle();
	rs_s32 initType = rs_sdk_getCheckInitType();

	if (userData == RS_NULL)
	{
		return;
	}
	else
	{
		msgID = userData->evt_id;
		param1 = userData->param1;
		param2 = userData->param2;
		exErrorCode = userData->exErrorCode;

		rs_free(userData);
	}

	RS_PORITNG_LOG(RS_LOG_INFO"rs_user_msg_queue_callback, param1=%d, param2=%d, exErrorCode=%d\n\r", param1, param2, exErrorCode);

	switch(msgID)
	{
	case USER_CHANGE_STATE:
		if(param1 == RS_FWDL_STATE_CHECKED)
		{
			if (param2 != 0)
			{

				if (initType == 0)
				{
					// TODO:��UI���棬��ô֪ͨ�û����°棻
				}
				else
				{
					// û�н���ֱ�ӽ�������
					if (rs_cb_have_ui() == 0)
					{
						ret = rs_sdk_download_ex(1);
						RS_PORITNG_LOG("rs_user_msg_queue_callback, auto check have new version, start downlaod \n", ret);
					}
				}
			}
			else
			{
				if (initType == 0)
				{
					//��ʾ�ֶ�����ģ�����Ӧ��֪ͨ�û�
					if (errCode == RS_ERR_OK)
					{
						// TODO:֪ͨ�û��Ѿ������°汾
					}
					else if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
					{
						// TODO:ϵͳ�ӿڴ��󡣱����дflash�ȡ�
					}
					else if (errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
					{
						// TODO:��������û�м�⵽�����汾
					}
					else
					{
						// TODO:����δ֪����
					}

					// send AT command to modem not have new version
				}
				else
				{
					//MMINET_DeviceSleepSetFlymode(2);
					rs_system_ppp_deactive();
					rs_system_ppp_deattach();
				}
			}
		}
		else if(param1 == RS_FWDL_STATE_DOWNLOADING)
		{
			// ��ʾ���ش�����ͣ״̬
			if (param2 == 1)
			{
				rs_mem_info();

				if (initType == 0)
				{
					if (errCode >= RS_ERR_UNSPECIFIC && errCode <= RS_ERR_FLASH_READ)
					{
						// TODO:ϵͳ�ӿڴ��󡣱����дflash�ȡ�
					}
					else if(errCode >= RS_ERR_XPT_OPEN_SOCK && errCode <= RS_ERR_XPT_GET_HOST_BY_NAME)
					{
						// TODO:�������⵼�������ж���
					}
					else if (errCode == RS_USER_CANCEL)
					{
						// TODO:�û�ȡ��������
					}
					else
					{
						// TODO:����δ֪����
					}
				}
				else
				{
					//MMINET_DeviceSleepSetFlymode(2);
					rs_system_ppp_deactive();
					rs_system_ppp_deattach();
				}

				
			}
		}
		else if(param1 == RS_FWDL_STATE_DOWNLOADED)
		{
			rs_mem_info();

			RS_PORITNG_LOG("%s, download completed or fail,param2=%d \n", __func__, param2);
			
			if (param2 == 0)
			{

				if (initType == 0)
				{
					// TODO:�����UI����ô�����û�ȥ������û�н���ֱ�ӽ�����������
					// send AT command to momdem download completed
				}
				else
				{
					// û�н���ֱ�ӽ�������
					ret = rs_sdk_install_ex();
					//MMINET_DeviceSleepSetFlymode(2);
					
					//MMIAPINET_MqttSendSignal(NET_STATE_RSFOTA_INSTALL_REQ, 0);
					RS_PORITNG_LOG("%s, start to install ret = %d \n", __func__, ret);
				}
			}
			else if (param2 == 2)
			{
				// TODO:��ʧ�ܵĴ����������ޣ��ײ�����������������ء������UI����ô��Ҫ���ص��û������棬û�н��治�ô���
				// send AT command to momdem download fail

			}
		}
		else if(param1 == RS_FWDL_STATE_INSTALLING)
		{
			if(param2 == 1)
			{
				// TODO:ϵͳ���󣬲���д�����flash�����ļ������ߵ��������ӿڴ���
			}
		}
		else if(param1 == RS_FWDL_STATE_INSTALLED)
		{
			// ������ɣ�һ�����ͻ��յ������Ϣ
			if (param2 == 0)
			{
				// TODO:��װ�ɹ��������UI��Ҫ֪ͨ�û�
			}
			else
			{
				// TODO:��װʧ�ܣ������UI��Ҫ֪ͨ�û�
			}
		}
		else if(param1 == RS_FWDL_STATE_REPORTED)
		{
			//MMINET_DeviceSleepSetFlymode(2);
			rs_system_ppp_deactive();
			rs_system_ppp_deattach();
		}

		break;

	case USER_DOWNLOAD_BREAK: // ������Ϣ���ǿ���֮�󷢳�����Ϣ

		if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADING)
		{
			// TODO:����û��ϴιػ�֮ǰ���ڽ����������񣬿���֮����������ʾ
		}
		else if (param1 == RS_FWDL_BREAK_STATE_RESUME)
		{
			// TODO:����û��ϴε�ǰ�ִ�����ͣ״̬������������������֮����������ʾ���Ƿ�Ҫ����
		}
		else if (param1 == RS_FWDL_BREAK_STATE_DOWNLOADED)
		{
			// TODO:����û��ϴο�����������ɵ�������������֮����������ʾ���Ƿ��������
		}
		break;

	case USER_DOWNLOAD_PROGRESS:
		{
			// TODO:�ڽ�������ʾ���ȣ�param1��ʾ�Ѿ����ص����ݵĳ��ȣ�param2��ʾ�����ݵĳ���
		}
		break;

	case USER_STARTED:
		{
			// OTA�Ѿ��������
		}
		break;
	default:
		break;
	}
}

static void _send_msg_to_ui(rs_u32 Msg, rs_s32 wParam, rs_s32 lParam, rs_s32 exErrorCode)
{
	rs_s32 ret = 0;
	RS_EVENT_MSG * msg;

	RS_PORITNG_LOG(RS_LOG_INFO"_send_msg_to_ui, Msg=%d, wParam=%d, lParam=%d\n\r", Msg, wParam, lParam);

	msg = (RS_EVENT_MSG*)rs_malloc(sizeof(RS_EVENT_MSG));
	if (msg == RS_NULL)
	{
		RS_PORITNG_LOG(RS_LOG_ERROR"_send_msg_to_ui, emery alloc fail\n\r");
		return;
	}

	msg->evt_id = Msg;
	msg->param1 = wParam;
	msg->param2 = lParam;
	msg->exErrorCode = exErrorCode;

#ifdef SUPPORT_USER_MSG
	rs_post_message_to_thread_with_code(UA_TASK_USER_MSG_SIGNAL_CODE,  (void*)msg);
#else
	rs_user_msg_queue_callback((void*)msg);
#endif
}



/**
����˵��������ģ��ִ��״̬֪ͨ�����翪ʼ����ʱ��ᷢһ����Ϣ����ʼ���ص�ʱ��ᷢһ����Ϣ
����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_state( RS_FWDL_STATE state, rs_s32 code)
{
#ifndef SUPPORT_USER_MSG
	rs_s32 initType = rs_sdk_getCheckInitType();
#endif
	
	RS_PORITNG_LOG(RS_LOG_INFO"notify msg, state = %d , code = %d\n", state, code);
	// ��Ҫע��RS_FWDL_STATE_DOWNLOADED��code==2��ʱ����Ҫֱ�ӷ�����ǰpkg����Ϊsdk�Ѿ������ˣ�Ӧ��ֱ�ӻص�������

	_send_msg_to_ui((rs_u32)USER_CHANGE_STATE, (rs_s32)state, (rs_s32)code, 0);
}


/**
����˵��������ص����������OTA��ҵ���ص㣬�������ӵĻص�����������֮��,
����ϴο���������ĳ��״̬����,��ô����֮�������û�ȥ������������
�����У���ô�ϴιػ�֮ǰ����������״̬��������ɣ�����֮����û�һ�����ѣ�
������ͣ������֮�������û��������ء�

����˵������state����ʾ��ǰ�����ڵ�һ��״̬
����ֵ��VOID
*/
void rs_cb_notify_break_state( RS_FWDL_BREAK_STATE state)
{
	RS_PORITNG_LOG(RS_LOG_INFO"break state = %d \n", state);

	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_BREAK, (rs_s32)state, 0, 0);
}


/**
����˵������������ָʾ 
����˵������current����ʾ��ǰ�Ѿ����ص�����
��total����ʾ����
����ֵ��VOID
*/
void rs_cb_progress(rs_s32 current, rs_s32 total)
{
	RS_PORITNG_LOG(RS_LOG_INFO"dl progress:current = %d , total = %d\n", current, total);

	_send_msg_to_ui((rs_u32)USER_DOWNLOAD_PROGRESS, (rs_s32)current, (rs_s32)total, 0);
}

/**
����˵������ʾOTA�ĺ���ģ���Ѿ��������
����˵����
����ֵ��VOID
*/
void rs_cb_started()
{
	RS_PORITNG_LOG(RS_LOG_INFO"ota started -----^-^----\n");
	
	_send_msg_to_ui((rs_u32)USER_STARTED, 0, 0, 0);
}
