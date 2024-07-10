#include "rs_param.h"
#include "rs_system.h"
#include "rs_error.h"
#include "rs_datatype.h"
#include "rs_sdk_api.h"
#include "rs_state.h"
#include "rs_std_fun.h"
#include "rs_debug.h"
#include "rs_sdk_api_ex.h"
//#include "mmicom_string.h"
#include "Version.h"
#include "version_software_mocor.h"
#include "mn_api.h"
#include "mn_type.h"
//#include "MQTTClient.h"

/***********************************************************************************/
//extern kal_uint32 Image$$VIVA_GFH$$Base;
/***********************************************************************************/
rs_u32 DiskFree(void);
//extern void MMIAPINET_MqttSendSignal(NET_STATE_SOCKET msg_Signal, void *param);

/*
 *	��ȡ�豸Ʒ��
 */
static void get_OEMBrand(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡƷ�ƵĽӿ�
#else // default
	rs_strcpy(buf, "SPREADTRUM");
	return ;
#endif
}

/*
 *	��ȡ�ͺ�
*/
static void get_OEMDevice(char *buf)
{
#if 0
	// TODO:�ͻ�����������ӻ�ȡ�ͺŵĽӿ�
#else //default
	rs_strcpy(buf, "SC6531E-M1");
	return ;
#endif
}


/*
 * ��ȡ�豸��Ψһ��ʾ����
*/
static void get_OEMIMEI(char *buf)
{
	uint8               temp_str[20] = {0};
	MN_IMEI_T      imei_arr;

        MNNV_GetIMEIEx(MN_DUAL_SYS_1, imei_arr);        
        ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, temp_str);

        if (rs_strlen(temp_str) > 0)
        {
            rs_strcpy(buf, "IMEI:");
            rs_strcat(buf,  (const char*)temp_str);
        }
        else
        {
            rs_strcpy(buf,  "IMEI:123456789000003");
        }
}

/*
 * ��ȡ��ǰ�豸������汾��
*/
static void get_OEMCurrentVersion(char *buf)
{
#if 0
	// TODO:�ͻ�������������Զ���Ľӿ�
#else // default
	const char* version =  _MOCOR_SW_VERSION_;
	if (version)
	{
		rs_strcpy(buf, version);
	}
	else
	{
		rs_strcpy(buf, "V2.0");
	}
#endif
}


/*
 * ��ȡ��ǰ�豸����������
*/
static void get_OEMLanguage(char *buf)
{
	rs_strcpy(buf, "zh-CN");
}

/*
 * ��ȡ��Ӫ�̵�ID
*/
static void get_OEMCatoragy(char *buf)
{
    MN_IMSI_T          imsi_str;

    imsi_str = MNSIM_GetImsiEx(MN_DUAL_SYS_1);    
    if(MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
    {
        ConvertBcdToDigitalStr(imsi_str.imsi_len, imsi_str.imsi_val, buf);
        buf[5] = 0;
    }
    else
    {
        rs_strcpy(buf, "46001");
    }
    
}

/*
 * ��ȡOS�İ汾
*/
static void get_OEMOSVersion(char *buf)
{
	rs_strcpy(buf, "ThreadX4.0");
}

/*
 * ��ȡapp key
*/
static void get_AppKey(char *buf)
{
	rs_strcpy(buf, "xjfykhqqb0qobkaeyooro5i1");
	
}

/*
 * ��ȡ������
*/
static void get_Channel(char *buf)
{
	rs_strcpy(buf, "Spreadtrum_6531E");
}

/*
 * ��ȡ��վ
*/
static void get_CellInfo(char *buf)
{
	MN_PHONE_CURRENT_PLMN_INFO_T plmn_info = {0};

	plmn_info = MNPHONE_GetCurrentPLMNInfoEx(MN_DUAL_SYS_1);
	rs_sprintf(buf, "%d-%d-%d", plmn_info.lac, plmn_info.cell_id, 0);
}

/**
����˵������ȡ�豸����
����˵������key���豸���Ա�ʶ
		  ��value������ֵ
����ֵ���ɹ�����0��ʧ�ܷ���1
ע�⣺value�ĳ���Ϊ64���ֽڣ���Ҫ��Ҫ����63���ֽ�
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value)
{
	if (0 == key || 0 == value) 
		return 1;

	if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVID, key)) // ��ȡ�豸ID
	{
		get_OEMIMEI(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MAN, key)) // ��ȡƷ��
	{
		get_OEMBrand(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MODEL, key)) // ��ȡ�ͺ�
	{
		get_OEMDevice(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_FWV, key)) // ��ȡ��ǰ������汾
	{
		get_OEMCurrentVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LANG, key)) // ��ȡ��ǰ������
	{
		get_OEMLanguage(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OPERATORS, key)) // ��ȡ��Ӫ�̵�ID
	{
		get_OEMCatoragy(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OSV, key)) // ��ȡϵͳ�İ汾��
	{
		get_OEMOSVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_APP_KEY, key)) // ��ȡapp key
	{
		get_AppKey(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_CHANNEL, key)) // ��ȡ������
	{
		get_Channel(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVMAC, key)) // ��ȡ������
	{
		value[0] = 0;
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LOC, key)) // ��ȡ������
	{
		get_CellInfo(value);
	}
	else
	{
		return 1;
	}
	
	if (key != 0 && value != 0 && (rs_strlen(key) > 0) && (rs_strlen(value) > 0))
	{
		RS_PORITNG_LOG(RS_LOG_INFO"key=%s, value = %s \r\n", key, value);
	}

	return 0;
}


/**
����˵������ѯ�Ƿ����û�����
����˵����
����ֵ������0����ʾû��UI������1����ʾ��UI
����˵������û��UI�����ĳ������ǲ�ͬ�ġ������ϴ��������ع����У�����ĳ���������
�����ֻ���������ô����֮�������UI�����û����������û��UI����������
*/
rs_s32 rs_cb_have_ui()
{
	return 0;
}

/**
����˵������ȡ��һ���Զ������뿪����ʱ������
����˵����
����ֵ������ֵΪ���뿪���ĺ���ʱ��
*/
rs_u32 rs_cb_get_first_check_cycle()
{
	//return (90*1000);
	//return (1*60*1000);
	//return (3*60*1000);
	return (60*1000);
}

/**
����˵������һ���Զ�����������֮��ÿ���Զ����ļ����ʱ��
����˵����
����ֵ������ֵΪÿ���Լ������ڵļ��ʱ��
*/
rs_u32 rs_cb_get_auto_check_cycle()
{
	return (24*60*60*1000);
	//return 8*60*60*1000;
	//return 90*1000;
}


/**
����˵�����Զ�������ڵ���
����˵������
����ֵ���ɹ�����RS_ERR_OK, ʧ�ܷ�������ֵ
*/
rs_s32 rs_cb_auto_check()
{
	rs_s32 ret = RS_ERR_OK;
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();
	

	RS_PORITNG_LOG(RS_LOG_INFO"rs_cb_auto_check, current state is %d \r\n", state);
	if(rs_cb_have_ui() == 0)
	{
		// û��UI
		if (state == RS_FWDL_STATE_DOWNLOAD_PAUSE)
		{
			// ����ϳ���ʲô�쳣��������ֱ������
			ret = rs_sdk_download_ex(1);
			return ret;
		}
	        else if(state == RS_FWDL_STATE_DOWNLOADED)
	        {
	            // ����Ѿ����ص�δ��װ�����ֱ�Ӱ�װ
	            ret =  rs_sdk_install_ex();
	            //MMIAPINET_MqttSendSignal(NET_STATE_RSFOTA_INSTALL_REQ, 0);
	            return ret;
	        }
	}

	//if (rs_sdk_needAutoCheck())
	{
		ret = rs_sdk_check_ex(1); // �����ڲ��ж�״̬���жϣ������ʱ�����ʽ��м�⣬�������˳�
	}

	return ret;
}

/**
����˵����������������Դ���
����˵������
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_check_retry_count()
{
	return 4;
}

/**
����˵������ȡ����������Դ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_retry_count()
{
	return 20;
}

/**
����˵������ȡ��������ϱ�������Դ���
����˵����
����ֵ���������Դ���
�ر�˵���������Դ�����Ҫ���ô�һ�㣬�������ϱ��ɹ���������ں����ж����Ӱ��
*/
rs_s32 rs_cb_get_report_retry_count()
{
	return 5;
}

/**
����˵������ȡ���ع��������ʧ�ܲ��ԣ�rs_cb_get_download_retry_count�������ع��������ʧ���˻��Զ����ԵĴ���
		����ܴ��������˴˺����趨�����ޣ���ô��������������
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_download_max_fail_count()
{
	return 10;
}

/**
����˵������ȡ�ϱ����������ʧ�ܲ��ԣ�rs_cb_get_report_retry_count�����ϱ����������ʧ���˻��Զ����ԵĴ�����
		����ܴ���������rs_cb_get_report_max_fail_count�����趨�����ޣ���ô�����ϱ���
����˵����
����ֵ���������Դ���
*/
rs_s32 rs_cb_get_report_max_fail_count()
{
	return 10;
}

/**
����˵����socket�����й����еĸ��ֳ�ʱ����
����˵����type ����ͬ��ʱ����
			0  gethostbyname�ĳ�ʱ����
			1  connect�ĳ�ʱ����
			2  send�ĳ�ʱ����
			3  recv�ĳ�ʱ����
����ֵ�����س�ʱ��ʱ�����ڣ���λΪ����
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type)
{
	rs_s32 timeout = 0;

	switch(type)
	{
	case 0: // gethostbyname ��ʱʱ��
		timeout = 30*1000;
		break;
	case 1: // connect ��ʱʱ��
		timeout = 15*1000;
		break;
	case 2: // send ��ʱʱ��
		timeout = 15*1000;
		break;
	case 3: // recv ��ʱʱ��
		timeout = 15*1000;
		break;
	case 4: // session ���ʱ��
		timeout = 10*1000;
		break;
	}

	return timeout;
}


/**
����˵������ȡ��ǰ���ʣ������Ƿ����
����˵����
����ֵ���������ڵ���33%,�򷵻�RS_TRUE,����RS_FALSE
�ر�˵��:bmt_status.volt��ص�ѹ�������п��ܸ��ݵ�ѹ�жϵ�ǰ���Ӿ�ȷ��
����ʣ�������������Ϊ���Ϊ3593500,���Ϊ4253900���ֳ�100�֣����ǰ�volt
��ӡ������Ϊ�˽�
������ע�⣬��MMITask.c��init�����ϵ������½���ǲ��еģ��п��ܳ���û�г�ʼ��
*/
rs_bool rs_sys_bettery_electricity_enough()
{
	return RS_TRUE;
}

/**
����˵������ȡ��ǰ���õĿռ�
����˵����
����ֵ�����ص�ǰ���õ�ʣ��ռ�,��λΪbyte
*/
rs_u32 rs_sys_available_space()
{
    return DiskFree();
}


#ifdef RS_SUPPORT_UPDATE_DATA_FS

/**
����˵������ȡ������������·��
����˵����
����ֵ����������·��
*/
const rs_s8* rs_device_get_update_file()
{
	return FLASH_UPDATE_FILE;
}

#else

/*
������������ݴ洢��flash�У���ô������Ϣ����Ҳ�϶���Ҫ��flash�д洢��
��Ϊbootloader��Ҫ��ȡ
*/

/**
����˵������ȡ�����������ش�ŵ�flash�ĵ�ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_addr()
{
	return FLASH_GetSperateLoadAddr() + 2*BLOCK_SIZE;;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info1_addr()
{
	return FLASH_GetSperateLoadAddr();
}

/**
����˵������ȡ��������Ϣ�Ĵ洢��ַ
����˵����
����ֵ�����ش洢��ַ
*/
rs_u32 rs_device_get_update_info2_addr()
{
	return FLASH_GetSperateLoadAddr()  + BLOCK_SIZE;
}

#endif

#ifdef RS_SUPPORT_UPDATE_INFO_FS

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info1_file()
{
	return RS_CONFIG_UPINFO_FILE1;
}

/**
����˵������ȡ��������Ϣ�Ĵ洢·��
����˵����
����ֵ�����ش洢·��
*/
const rs_s8* rs_device_get_update_info2_file()
{
	return RS_CONFIG_UPINFO_FILE2;
}

#endif



/**
����˵������ȡVIVA����ʼ��ַ
����˵������
����ֵ������VIVA����ʼ��ַ�����û��VIAVA�����򷵻�0 
*/
rs_u32 rs_sys_getVIVAStartAddr()
{
	//return (((rs_u32)&Image$$VIVA_GFH$$Base) & 0x00FFFFFF);
	return 0;
}


rs_u32 rs_cb_get_update_result()
{
	return 1;
}

rs_u32 DiskFree(void)  
{
	// �ܴ�С - 2*updateinfo size - backup size
	return (rs_u32)(FLASH_GetEndAddr() - FLASH_GetSperateLoadAddr() - 2*BLOCK_SIZE - BLOCK_SIZE);
}   