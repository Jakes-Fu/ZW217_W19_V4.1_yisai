#include "abup_typedef.h"
#include "abup_debug.h"
#include "abup_define.h"
#include "abup_device.h"
#include "dal_time.h"
#include "mn_type.h"
#include "abup_build_datetime.h"
#include "abup_display.h"

const char* const  s_abup_project_name = ABUP_PROJECT_NAME;
const char* const  s_abup_version_info = ABUP_BUILD_TIME;

ABUP_BOOL s_abup_em_mode = ABUP_FALSE;

#define ABUP_NET_RETRY_MAXCNT 5
#define ABUP_NET_SEPARATE_SIZE (64*1024)

abup_extern void abup_get_device_time(abup_time_struct* time);
abup_extern void Abup_CK_confirm_lsk(void);
abup_extern void abup_display_fota_status(abup_int32 status);

void abupDownData(void)
{
	Abup_CK_confirm_lsk();
}

abup_int32 abup_sim_ready(void)
{
	abup_uint8 simno = MN_DUAL_SYS_1;
	
	if(DMSIM_SimIsReady(MN_DUAL_SYS_1))
		return E_ABUP_SUCCESS;
	else
		return E_ABUP_FAILURE;
}

abup_int32 abup_get_device_mid(abup_char* mid)
{
	abup_uint32 simno=0;

	ABUP_BOOL ret=ABUP_FALSE;
	MN_IMEI_T imei_arr={0};

	if(!mid)
		return E_ABUP_FAILURE;

	if(abup_em_mode() == ABUP_TRUE)
	{
		abup_em_get_mid(mid);
		return E_ABUP_SUCCESS;
	}
	
	ret = MNNV_GetIMEIEx(simno, imei_arr);
	if(ret == ABUP_FALSE)
		return E_ABUP_FAILURE;

	ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, (abup_uint8 *)mid);

	return E_ABUP_SUCCESS;
}

extern abup_char *COMMON_GetVersionInfo(abup_uint8 version_type);
abup_char* abup_get_device_version(void)
{
#ifndef WIN32
	static abup_char verno_str[100];
	abup_uint8 verno_size;

	//PLATFORM_VERSION: 0; BUILD_TIME: 7
	verno_size=abup_strlen(COMMON_GetVersionInfo(0))+abup_strlen(COMMON_GetVersionInfo(7));
	abup_memset(verno_str,0,100);
	abup_snprintf(verno_str, 100, "%s_%s",s_abup_project_name, s_abup_version_info);
	return verno_str;      
#else
	return "UIS8910_240x320BAR_16MB_DS_DEBUG_20200722_1417";
#endif
}

abup_char* abup_get_device_oem(void)
{
	return ADUPS_FOTA_SERVICE_OEM;	
}

abup_char* abup_get_device_model(void)
{
	return ADUPS_FOTA_SERVICE_MODEL;		
}

abup_char* abup_get_device_product_id(void)
{
	return ADUPS_FOTA_SERVICE_PRODUCT_ID;		
}

abup_char* abup_get_device_product_sec(void)
{
	return ADUPS_FOTA_SERVICE_PRODUCT_SEC;		
}

abup_char* abup_get_device_type(void)
{
	return ADUPS_FOTA_SERVICE_DEVICE_TYPE;		
}

abup_char* abup_get_device_platform(void)
{
	return ADUPS_FOTA_SERVICE_PLATFORM;		
}

abup_char* abup_get_device_network_type(void)
{
	return ADUPS_DEVICE_NETWORKTYPE;		
}

void abup_get_device_time(abup_time_struct* time)
{
	//applib_dt_get_rtc_time((applib_time_struct*)time);

	SCI_DATE_T  sysdate={0};
	SCI_TIME_T  systime={0};
	
	if(time != NULL)
	{
		if((TM_GetSysDate(&sysdate) == ERR_TM_NONE)	&& TM_GetSysTime(&systime) == ERR_TM_NONE)
		{
			time->nYear = sysdate.year;
			time->nMonth = sysdate.mon;
			time->nDay = sysdate.mday;
			time->nHour = systime.hour;
			time->nMin = systime.min;
			time->nSec = systime.sec;
			
			//return MR_SUCCESS;
		}
	}
	
	//return MR_FAILED;
}

abup_uint32 abup_get_utc_time_sec(abup_time_struct* time)
{
	abup_uint32 utc_time=0;

	if(time)
	{
		utc_time = abup_dt_curtime2utcsec(time);

		abup_DebugPrint("abup timestamp:%d; systime:%04d-%02d-%02d,%02d:%02d:%02d", \
			utc_time,
			time->nYear,time->nMonth,time->nDay,
			time->nHour,time->nMin,time->nSec);
	}
	else
		abup_DebugPrint("abup timestamp: time is null");
		
	return utc_time;
}

abup_uint8 abup_get_device_battery_level(void)
{
	return 0;
}

void abup_device_start_reboot(void)
{	
	abup_DebugPrint("abup: reboot, please waiting...");
	SCI_Sleep(1000);
    {
        extern POWER_FOTA_Update_Reset();
        POWER_FOTA_Update_Reset();
    }
}

void* abup_memalloc(abup_int32 size)
{
	void *ptr=NULL;
	
	ptr = SCI_ALLOC_APP(size);
	return ptr;
}

void abup_memfree(void *p)
{
	if(p != NULL)
	{
		abup_uint32 cursize=0;
		
		//abup_DebugPrint("abup_memfree:%X", p);
        SCI_FREE(p);
		p = NULL;
	}
}

void* abup_memalloc_ext(abup_int32 size)
{
	void *ptr=NULL;
	ptr = abup_memalloc(size);
	return ptr;
}
void abup_memfree_ext(void *p)
{
	if(p != NULL)
        abup_memfree((void **)&p);
}

void abup_memclean(void)
{
#ifdef ABUP_DMEM_DEBUG
	abup_uint16 idx=0;
	abup_uint32 cursize=0;
	for(idx=0;idx<ABUP_DMEM_MAXNUM;idx++)
	{
		if(s_dmem_info.dmem[idx].addr != NULL)
		{			
			abup_DebugPrint("abup_memclean curfind:0x%X, size:%d, idx:%d", \
				s_dmem_info.dmem[idx].addr, s_dmem_info.dmem[idx].size, idx);

			abup_memfree(s_dmem_info.dmem[idx].addr);

			cursize = s_dmem_info.dmem[idx].size;
			s_dmem_info.totalsize -= cursize;
			s_dmem_info.totalnum --;
			if(s_dmem_info.totalnum < 0)
				s_dmem_info.totalnum = 0;
			
			s_dmem_info.dmem[idx].addr = NULL;
			s_dmem_info.dmem[idx].size = 0;
		}
	}

	abup_DebugPrint("abup_memclean: totalnum:%d,totalsize:%d", \
		s_dmem_info.totalnum, s_dmem_info.totalsize);
#endif
}

void abup_meminfo(void)
{
#ifdef ABUP_DMEM_DEBUG
	abup_DebugPrint("abup_meminfo: totalnum:%d,totalsize:%d", \
		s_dmem_info.totalnum, s_dmem_info.totalsize);
#endif	
}

abup_uint32 abup_entry_critical_timeout(void)
{
	return 30*1000;
}

void abup_entry_save_context(void)
{
	abup_DebugPrint_Ext("[ABUP_COMMON]:","abup_entry_save_context");
}
extern abup_uint8 abup_get_dl_state(void);
extern abup_uint8 abup_get_fota_state(void);
extern void exit_ota_mode();

void abup_exit_restore_context(void)
{
	
	abup_DebugPrint_Ext("[ABUP_COMMON]:","abup_exit_restore_context abup_get_dl_state() is %d",abup_get_dl_state());
	if(abup_get_dl_state()==ABUP_DL_SUCCESS)
	{
		abup_device_start_reboot();
	}
	else
	{
		abup_memclean();
	}
	abup_DebugPrint("abup_exit_restore_context");
	abup_task_uninit();
}


void abup_get_device_data_point1(abup_char** name,abup_char** value)
{
	static abup_char areacode[10]={'\0'};
	*name=NULL;//"areaCode";
	*value=NULL;//areacode;
}
/*****************************
**up_result 输入，标识升级结果
**:1   升级成功
**:其他  升级失败
******************************/
abup_int32 abup_get_upgrade_result(abup_int32 up_result)
{
	if(up_result==1)
	{
		//表示升级成功
		abup_DebugPrint("abup_get_upgrade_result,up_result:%d",up_result);
	}
}

abup_uint32 abup_str2hex(const abup_char* pszStr, abup_uint32 u4Len)
{
	abup_uint32 u4Idx;
	abup_uint32 u4ReturnValue = 0;

	if ((pszStr == NULL) || (u4Len == 0))
	{
		return 0;
	}

	u4Len = (u4Len > 8) ? 8 : u4Len;

	for (u4Idx = 0;
		u4Idx < u4Len;
		u4Idx++)
	{
		if ((pszStr[u4Idx] >= '0') && (pszStr[u4Idx] <= '9'))
		{
			u4ReturnValue = u4ReturnValue << 4;
			u4ReturnValue += (abup_uint32)(abup_uint8)(pszStr[u4Idx] - '0');
		}
		else
		if ((pszStr[u4Idx] >= 'A') && (pszStr[u4Idx] <= 'F'))
		{
			u4ReturnValue = u4ReturnValue << 4;
			u4ReturnValue += (abup_uint32)(abup_uint8)(pszStr[u4Idx] - 'A' ) + 10;
		}
		else
		if ((pszStr[u4Idx] >= 'a') && (pszStr[u4Idx] <= 'f'))
		{
			u4ReturnValue = u4ReturnValue << 4;
			u4ReturnValue += (abup_uint32)(abup_uint8)(pszStr[u4Idx] - 'a') + 10;
		}
		else
		{
			return 0;
		}
	}

	return u4ReturnValue;
}

/****************************
*download_status  网络状态 DL_NETWORK_ERROR 下载出错  DL_NO_SPACE 空间不足  ABUP_DL_SUCCESS 下载成功
*abupstatus CK_NEW_VERSION表示有新的版本   UP_SUCCESS 表示上报成功
*abupfileSize  差分包的大小
* abupstate fota state -->STATE_CV  表示检测版本 STATE_DL 下载差分包版本  STATE_RD  上报下载差分包下载结果  STATE_RU 上报升级结果
*abupDlResult 下载差分包结果 abup_DL_SUCCESS 表示下载成功
*version_name 最新版本的版本名字
**************************/
void GetMainStates(abup_char *version_name,abup_int32 download_status,abup_int32 abupstatus,abup_int32 abupfileSize,abup_int32 abupstate,abup_uint8 abupDlResult)
{
	if((abupstate == STATE_DL)&&(abupfileSize == abup_recvd_size()))
	{
		//进入这里面表示下载差分包成功的
	}
	
	if((abupstate == STATE_DL)&&(abupDlResult != ABUP_DL_SUCCESS))
	{
		//进入这里面表示下载差分包失败的			
        abup_display_fota_status(FOTA_STATE_ERROR);      
	}

	if((abupstate == STATE_CV)&&(abupstatus==FOTA_SUCCESS))
	{
		//进入这里面表示有新的版本			
	}

	if((abupstate == STATE_CV)&&(abupstatus==FOTA_CV_LAST_VERSION))
	{
		//进入这里面表示当前版本是最新版本			
        abup_display_fota_status(FOTA_STATE_LAST_VER);      
	}
	
	abup_DebugPrint( "GetMainStates,version_name:%s",version_name);
}

void abup_notify_fotastate(abup_int32 status)
{
    FotaDisplayState state = 0;
	abup_DebugPrint("abup_notify_fotastate: %d", status);
    if (status == STATE_CV)
        state = FOTA_STATE_CV;
    else if (status == STATE_DL)
        state = FOTA_STATE_DL;    
    else if (status == STATE_RD)
        state = FOTA_STATE_RD;
    else if (status == STATE_RU)
        state = FOTA_STATE_RU;
    else if (status == STATE_RG)
        state = FOTA_STATE_RG; 
    else 
        return;
    abup_display_fota_status(state);      
}

void abup_process_status_cb(abup_int32 status,abup_uint8 fota_status)
{
	abup_DebugPrint("abup_process_status_cb: %d,%d", status,fota_status);

	switch(status)
	{
		case FOTA_SUCCESS:
			break;

		default:
			break;
	}
}

void abup_fota_atcmd_custom(abup_char *cmd, abup_char *para0, abup_char *para1,abup_char *para2, abup_write func)
{
}

void abup_em_set_mode(ABUP_BOOL mode)
{
	s_abup_em_mode = mode; 
}

ABUP_BOOL abup_em_mode(void)
{
	return s_abup_em_mode;
}

void abup_em_set_mid(abup_char* mid)
{
}

void abup_em_get_mid(abup_char* mid)
{
}

abup_int16 abup_net_retry_maxtime(void)
{
	return ABUP_NET_RETRY_MAXCNT;
}

abup_uint32 abup_net_separate_size(void)
{
	return ABUP_NET_SEPARATE_SIZE;
}

