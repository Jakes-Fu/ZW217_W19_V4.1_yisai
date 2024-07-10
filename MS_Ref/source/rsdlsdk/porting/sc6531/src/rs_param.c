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
 *	获取设备品牌
 */
static void get_OEMBrand(char *buf)
{
#if 0
	// TODO:客户可在这里添加获取品牌的接口
#else // default
	rs_strcpy(buf, "SPREADTRUM");
	return ;
#endif
}

/*
 *	获取型号
*/
static void get_OEMDevice(char *buf)
{
#if 0
	// TODO:客户可在这里添加获取型号的接口
#else //default
	rs_strcpy(buf, "SC6531E-M1");
	return ;
#endif
}


/*
 * 获取设备的唯一标示符号
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
 * 获取当前设备的软件版本号
*/
static void get_OEMCurrentVersion(char *buf)
{
#if 0
	// TODO:客户可在这里添加自定义的接口
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
 * 获取当前设备的语言类型
*/
static void get_OEMLanguage(char *buf)
{
	rs_strcpy(buf, "zh-CN");
}

/*
 * 获取运营商的ID
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
 * 获取OS的版本
*/
static void get_OEMOSVersion(char *buf)
{
	rs_strcpy(buf, "ThreadX4.0");
}

/*
 * 获取app key
*/
static void get_AppKey(char *buf)
{
	rs_strcpy(buf, "xjfykhqqb0qobkaeyooro5i1");
	
}

/*
 * 获取渠道号
*/
static void get_Channel(char *buf)
{
	rs_strcpy(buf, "Spreadtrum_6531E");
}

/*
 * 获取基站
*/
static void get_CellInfo(char *buf)
{
	MN_PHONE_CURRENT_PLMN_INFO_T plmn_info = {0};

	plmn_info = MNPHONE_GetCurrentPLMNInfoEx(MN_DUAL_SYS_1);
	rs_sprintf(buf, "%d-%d-%d", plmn_info.lac, plmn_info.cell_id, 0);
}

/**
函数说明：获取设备属性
参数说明：【key】设备属性标识
		  【value】属性值
返回值：成功返回0，失败返回1
注意：value的长度为64个字节，主要不要超过63个字节
*/
rs_s32 rs_cb_get_property(const rs_s8 *key, rs_s8 *value)
{
	if (0 == key || 0 == value) 
		return 1;

	if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVID, key)) // 获取设备ID
	{
		get_OEMIMEI(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MAN, key)) // 获取品牌
	{
		get_OEMBrand(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_MODEL, key)) // 获取型号
	{
		get_OEMDevice(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_FWV, key)) // 获取当前的软件版本
	{
		get_OEMCurrentVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LANG, key)) // 获取当前的语言
	{
		get_OEMLanguage(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OPERATORS, key)) // 获取运营商的ID
	{
		get_OEMCatoragy(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_OSV, key)) // 获取系统的版本号
	{
		get_OEMOSVersion(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_APP_KEY, key)) // 获取app key
	{
		get_AppKey(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_CHANNEL, key)) // 获取渠道号
	{
		get_Channel(value);
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_DEVMAC, key)) // 获取渠道号
	{
		value[0] = 0;
	}
	else if (0 == rs_strcmp(PROPERTY_KEY_DEVICE_LOC, key)) // 获取渠道号
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
函数说明：查询是否有用户界面
参数说明：
返回值：返回0，表示没有UI；返回1，表示有UI
场景说明：有没有UI在许多的场景还是不同的。比如上次正在下载过程中，由于某种特殊情况
导致手机重启，那么开机之后，如果有UI，由用户触发，如果没有UI则主动发起
*/
rs_s32 rs_cb_have_ui()
{
	return 0;
}

/**
函数说明：获取第一次自动检测距离开机的时间周期
参数说明：
返回值：返回值为距离开机的毫秒时间
*/
rs_u32 rs_cb_get_first_check_cycle()
{
	//return (90*1000);
	//return (1*60*1000);
	//return (3*60*1000);
	return (60*1000);
}

/**
函数说明：第一次自动检测周期完成之后，每次自动检测的间隔的时间
参数说明：
返回值：返回值为每次自检测的周期的间隔时间
*/
rs_u32 rs_cb_get_auto_check_cycle()
{
	return (24*60*60*1000);
	//return 8*60*60*1000;
	//return 90*1000;
}


/**
函数说明：自动检测周期到达
参数说明：无
返回值：成功返回RS_ERR_OK, 失败返回其他值
*/
rs_s32 rs_cb_auto_check()
{
	rs_s32 ret = RS_ERR_OK;
	RS_FWDL_STATE state = rs_sdk_getDLCurrentState();
	

	RS_PORITNG_LOG(RS_LOG_INFO"rs_cb_auto_check, current state is %d \r\n", state);
	if(rs_cb_have_ui() == 0)
	{
		// 没有UI
		if (state == RS_FWDL_STATE_DOWNLOAD_PAUSE)
		{
			// 如果上出现什么异常情况，这次直接下载
			ret = rs_sdk_download_ex(1);
			return ret;
		}
	        else if(state == RS_FWDL_STATE_DOWNLOADED)
	        {
	            // 如果已经下载但未安装，这次直接安装
	            ret =  rs_sdk_install_ex();
	            //MMIAPINET_MqttSendSignal(NET_STATE_RSFOTA_INSTALL_REQ, 0);
	            return ret;
	        }
	}

	//if (rs_sdk_needAutoCheck())
	{
		ret = rs_sdk_check_ex(1); // 函数内部有对状态的判断，如果此时不合适进行检测，函数会退出
	}

	return ret;
}

/**
函数说明：允许检测最大重试次数
参数说明：无
返回值：返回重试次数
*/
rs_s32 rs_cb_get_check_retry_count()
{
	return 4;
}

/**
函数说明：获取下载最大重试次数
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_retry_count()
{
	return 20;
}

/**
函数说明：获取升级完成上报最大重试次数
参数说明：
返回值：返回重试次数
特别说明：此重试次数需要设置大一点，尽量让上报成功，否则对于后期判断造成影响
*/
rs_s32 rs_cb_get_report_retry_count()
{
	return 5;
}

/**
函数说明：获取下载过程中最大失败测试，rs_cb_get_download_retry_count是在下载过程中如果失败了会自动重试的次数
		如果总次数超过了此函数设定的上限，那么放弃此升级包。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_download_max_fail_count()
{
	return 10;
}

/**
函数说明：获取上报过程中最大失败测试，rs_cb_get_report_retry_count是在上报过程中如果失败了会自动重试的次数。
		如果总次数超过了rs_cb_get_report_max_fail_count函数设定的上限，那么放弃上报。
参数说明：
返回值：返回重试次数
*/
rs_s32 rs_cb_get_report_max_fail_count()
{
	return 10;
}

/**
函数说明：socket在运行过程中的各种超时设置
参数说明：type 代表不同超时类型
			0  gethostbyname的超时设置
			1  connect的超时设置
			2  send的超时设置
			3  recv的超时设置
返回值：返回超时的时间周期，单位为毫秒
*/
rs_s32 rs_cb_get_socket_timeout(rs_s32 type)
{
	rs_s32 timeout = 0;

	switch(type)
	{
	case 0: // gethostbyname 超时时间
		timeout = 30*1000;
		break;
	case 1: // connect 超时时间
		timeout = 15*1000;
		break;
	case 2: // send 超时时间
		timeout = 15*1000;
		break;
	case 3: // recv 超时时间
		timeout = 15*1000;
		break;
	case 4: // session 间隔时间
		timeout = 10*1000;
		break;
	}

	return timeout;
}


/**
函数说明：获取当前电池剩余的量是否充足
参数说明：
返回值：电量大于等于33%,则返回RS_TRUE,否则RS_FALSE
特别说明:bmt_status.volt电池电压，厂家有可能根据电压判断当前更加精确的
电量剩余情况，比如认为最低为3593500,最高为4253900，分成100分，我们把volt
打印出来作为了解
另外请注意，在MMITask.c的init中马上调用如下借口是不行的，有可能程序还没有初始化
*/
rs_bool rs_sys_bettery_electricity_enough()
{
	return RS_TRUE;
}

/**
函数说明：获取当前可用的空间
参数说明：
返回值：返回当前可用的剩余空间,单位为byte
*/
rs_u32 rs_sys_available_space()
{
    return DiskFree();
}


#ifdef RS_SUPPORT_UPDATE_DATA_FS

/**
函数说明：获取升级包的下载路径
参数说明：
返回值：返回下载路径
*/
const rs_s8* rs_device_get_update_file()
{
	return FLASH_UPDATE_FILE;
}

#else

/*
如果升级包数据存储于flash中，那么升级信息数据也肯定是要在flash中存储的
因为bootloader需要读取
*/

/**
函数说明：获取升级包的下载存放的flash的地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_addr()
{
	return FLASH_GetSperateLoadAddr() + 2*BLOCK_SIZE;;
}

/**
函数说明：获取升级包信息的存储地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_info1_addr()
{
	return FLASH_GetSperateLoadAddr();
}

/**
函数说明：获取升级包信息的存储地址
参数说明：
返回值：返回存储地址
*/
rs_u32 rs_device_get_update_info2_addr()
{
	return FLASH_GetSperateLoadAddr()  + BLOCK_SIZE;
}

#endif

#ifdef RS_SUPPORT_UPDATE_INFO_FS

/**
函数说明：获取升级包信息的存储路径
参数说明：
返回值：返回存储路径
*/
const rs_s8* rs_device_get_update_info1_file()
{
	return RS_CONFIG_UPINFO_FILE1;
}

/**
函数说明：获取升级包信息的存储路径
参数说明：
返回值：返回存储路径
*/
const rs_s8* rs_device_get_update_info2_file()
{
	return RS_CONFIG_UPINFO_FILE2;
}

#endif



/**
函数说明：获取VIVA的起始地址
参数说明：无
返回值：返回VIVA的起始地址，如果没有VIAVA分区则返回0 
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
	// 总大小 - 2*updateinfo size - backup size
	return (rs_u32)(FLASH_GetEndAddr() - FLASH_GetSperateLoadAddr() - 2*BLOCK_SIZE - BLOCK_SIZE);
}   