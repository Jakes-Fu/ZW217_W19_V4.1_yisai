#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
#include "os_api.h"
#include "sfs.h"
#include "mmi_module.h"
#include "mmk_app.h"
#include "guimenu.h"
#include "mmiwre_other.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "../../../../../../wre/wrekernel/include/sdk/config.h"
#include "mmiphone_export.h"
#include "dualbat_drvapi.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiwre_adaptor.h"
#include "mmisd_export.h"
#include "mmiwre.h"

#ifdef WIN32

#define  WRE_WREKERNEL_PATH                     L"e:\\wre"
#define  WRE_WRESTORE_PATH                      L"e:\\wre"
#define  WRE_WREAPP_PATH                        L"e:\\wreapp"
#define  WRE_WREREGISTER_PATH                   L"e:\\wre"

#else

#define  WRE_WREKERNEL_PATH                     L"e:\\wre"
#define  WRE_WRESTORE_PATH                      L"e:\\wre"
#define  WRE_WREAPP_PATH                        L"e:\\wreapp"
#define  WRE_WREREGISTER_PATH                   L"e:\\wre"

#endif

//kernel注册表所在目录

#define  WRE_WREAPP_SD_PATH                     L"e:\\wreapp"

const uint16 *s_wre_sys_path[WRE_SYS_PATH_TOTAL] =
{
    WRE_WREKERNEL_PATH,
    WRE_WREAPP_PATH,
    WRE_WREAPP_SD_PATH, 

    L"d:\\Audio",
    L"e:\\Audio",

    L"d:\\Video",
    L"e:\\Video",

    L"d:\\Ebook",
    L"e:\\Ebook",

    L"d:\\Photos",
    L"e:\\Photos",

    L"d:\\Others",
    L"e:\\Others",

    WRE_WRESTORE_PATH,
    WRE_WREREGISTER_PATH,
    NULL, NULL, NULL, NULL
};

#define  SYS_DISK_CHAR   0x0063  // 默认为C
#define  SD_DISK_CHAR    0x0065  //默认为E


#ifndef WIN32
extern const char* const  s_version_info[];
#endif
void WRE_GetPLVersion(VERSION_T * version)
{
    memset(version->version,0,VERSIN_LEN);
#ifdef WIN32
	strncpy(version->version, "MOCOR_W11.27", VERSIN_LEN-1);
#else
    strncpy(version->version, (char *)s_version_info[0],VERSIN_LEN-1);
#endif
}

void WRE_GetBOOTVersion(VERSION_T * version)
{
    memset(version->version,0,VERSIN_LEN);
    #ifdef WRE_STORE_OVERSEAS
    strncpy(version->version, "v7.0.8.20120830.1", VERSIN_LEN-1);
    #else
    strncpy(version->version, "v7.0.8.20120830", VERSIN_LEN-1);
    #endif
}

extern BOOLEAN CheckWREVerMatch(void);
extern BOOLEAN MMIAPIDC_IsOpened();	// changed by yifei
/*****************************************************************************/
// 	Description : get the WRE version
//	Global resource dependence : 
//  Author:jie.yang
//	Note:
/*****************************************************************************/

#ifndef WIN32
typedef struct _OSVERSIONINFOW {
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    wchar  szCSDVersion[ 128 ];       // Maintenance string for PSS usage
} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW;

BOOL  GetVersionEx(LPOSVERSIONINFOW lpVersionInformation);
#endif

char*  MMIWRE_GetVersionInfo(void)
{
    #define WRE_RET_STRING_LEN   (64)
    static char wre_ret_string[WRE_RET_STRING_LEN]={0};
    VERSION_T  version;
    
    memset(wre_ret_string,0,WRE_RET_STRING_LEN);
    
    WRE_GetBOOTVersion(&version);
    strcpy(wre_ret_string,"WRE Version: ");
    strcat(wre_ret_string,version.version);
    strcat(wre_ret_string," + ");
     
    {
        char buff[32]={0} ;
        WRE_APP_PARAM_T  param = {0};
        
#ifndef WRE_WREKERNEL_NORFLASH
    #ifdef WRE_VIRTUALDISK_SUPPORT		
        if (!WRE_CreateVirtualROMDisk())
        {
            return NULL;  // changed by yifei
        }
    #endif

    #if !defined(WIN32)&&defined(WRE_VIRTUALDISK_SUPPORT)
        MMIWRE_ResetRuntimePath();
    #endif        
     
        if (!CheckWREVerMatch())
        {
        #if !defined(WIN32)&&defined(WRE_VIRTUALDISK_SUPPORT)&&defined(WRE_WREKERNEL_PRELOAD)
            wchar  path[256] = {0};
            MMIAPICOM_Wstrcpy(path, WRE_ROM_DISK_NAME);
            MMIAPICOM_Wstrcat(path, L":\\wre");
            WRE_Set_SYS_Dir(WRE_SYS_PATH_KERNEL, path);
            WRE_Set_SYS_Dir(WRE_SYS_PATH_STORE, path);
        #endif
        }
        
        param.argc = (int)0xeaebeced;
        param.cmdline = (uint8 *)buff;
        wre_startup(&param);
        
        
    #ifdef WRE_VIRTUALDISK_SUPPORT
        WRE_DestoryVirtualDisk();
    #endif
#else
        {
        
            OSVERSIONINFOW os;
            GetVersionEx(&os);
            sprintf(buff,"%d.%d.%d.N",os.dwMajorVersion,os.dwMinorVersion,os.dwBuildNumber);
        }
#endif
        strcat(wre_ret_string,buff);
    }
    return wre_ret_string;
}


void WRE_GetPRJName(VERSION_T * version)
{
	/*heng.xiao add begin 20120723 --- 通过配置文件来修改idh名*/
	const uint16 *srcfile =  L"e:\\wre\\idh.cfg";
	uint8 file_buf[VERSIN_LEN + 1] = {0};
	uint32              size  = 0;
    SFS_HANDLE          hFile = PNULL;
	SFS_ERROR_E         err = SFS_NO_ERROR;
	
	hFile = SFS_CreateFile(srcfile, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if(hFile)
	{
		err = SFS_ReadFile(hFile, (void *)file_buf, VERSIN_LEN, &size, PNULL);
		SFS_CloseFile(hFile);
		if(SFS_NO_ERROR == err && size > 0) 
		{
			memset(version->version,0,VERSIN_LEN);
			strncpy(version->version,(const char*)file_buf,VERSIN_LEN-1);
			return ;		
		}
	}
	/*heng.xiao add end 20120723*/
	
    memset(version->version,0,VERSIN_LEN);
    
#ifdef WRE_STORE_OVERSEAS
#ifdef SPRD_CUSTOMER
    #ifdef MAINLCD_SIZE_240X320
        strncpy(version->version,"SPRDBQTCXXXX",VERSIN_LEN-1);
   #elif defined MAINLCD_SIZE_240X400
        strncpy(version->version,"SPRDBWTCXXXX",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_320X480
        strncpy(version->version,"SPRDBHTCXXXX",VERSIN_LEN-1);
    #else
        strncpy(version->version,"SPRDBQTCXXXX",VERSIN_LEN-1);
    #endif
#else
    #ifdef MAINLCD_SIZE_240X320
        strncpy(version->version,"SPRDBQTCXXXX",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_240X400
        strncpy(version->version,"SPRDBWTCXXXX",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_320X480
        strncpy(version->version,"SPRDBHTCXXXX",VERSIN_LEN-1);
    #else
        strncpy(version->version,"SPRDBQTCXXXX",VERSIN_LEN-1);
    #endif
#endif  /* end SPRD_CUSTOMER */
#else    
#ifdef SPRD_CUSTOMER
    #ifdef MAINLCD_SIZE_240X320
        strncpy(version->version,"SPRD_CTMQ_0001",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_320X480
        strncpy(version->version,"SPRD_CTMH_0001",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_240X400
        strncpy(version->version,"SPRD_CTMW_0001",VERSIN_LEN-1);
    #else
        strncpy(version->version,"SPRD_CTMQ_0001",VERSIN_LEN-1);
    #endif
#else
    #ifdef MAINLCD_SIZE_240X320
        strncpy(version->version,"SPRD_6530Q_0001",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_240X400
        strncpy(version->version,"SPRD_6530W_0001",VERSIN_LEN-1);
    #elif defined MAINLCD_SIZE_320X480
        strncpy(version->version,"SPRD_6530H_0001",VERSIN_LEN-1);
    #else
        strncpy(version->version,"SPRD_6530H_0001",VERSIN_LEN-1);
    #endif
    #endif
#endif  /* end SPRD_CUSTOMER */
}

void WRE_GetScreenSize(int* w,int* h)
{
#ifdef MAINLCD_SIZE_240X320
    *w=240;
    *h=320;
#elif defined MAINLCD_SIZE_240X400
    *w=240;
    *h=400;
#elif defined MAINLCD_SIZE_176X220
    *w=176;
    *h=220;
#elif defined MAINLCD_SIZE_320X480
    *w=320;
    *h=480;
#endif
}
typedef enum tagWREMODULE{
    START_MODULE,
    WIFI_MODULE	
}WREMODULE_T;

int WRE_GetCHIPVersion(void)
{
#ifdef CHIP_VER_6800H1
    return PROCESSOR_6800H1;
#elif defined CHIP_VER_6600L1
    return PROCESSOR_6600L1;
#elif defined  CHIP_VER_6600L2
    return PROCESSOR_6600L2;
#elif defined  CHIP_VER_6530
    return PROCESSOR_6530;
#else
    return PROCESSOR_UNKNOWN;
#endif
}

char* GetBOOTBuild(void){
    static char buff[64]={0};
    sprintf(buff,"%s-%s",__DATE__,__TIME__);
    return buff;
}
void WRE_GetIDHID(VERSION_T * version)
{
    
}


int WRE_Is_Module_OK(int m){
    int ret = -1;
    switch(m)
    {
    case 0x11://touch type
        #if defined TOUCHPANEL_TYPE_HW
            ret = 1; 
            break;
        #elif defined TOUCHPANEL_TYPE_NONE 
            ret = 2;
            break;
        #elif defined TOUCHPANEL_TYPE_MULTITP 
            ret = 3;
            break;
        #else
            ret = -2;
            break;
        #endif
        //break;
    case 0x12://java
         #if defined JAVA_SUPPORT_IA
            ret = 1; 
            break;
        #elif defined JAVA_SUPPORT_NONE 
            ret = 2;
            break;
        #else
            ret = -2;
             break;
        #endif
	case 0x13://nes
	#ifdef NES_SUPPORT
            ret = 1; 
            break;
        #else
            ret = 2;
            break;
        
        #endif
		
			
    default:
        break;
    }
    return ret;
}

/*********************************************************************
*  WRE_Get_SYS_Dir
*
* 获得WRE系统目录
*
* 参数
* 
*   [in] 目录类型 内核或者应用目录
* 返回值
*  返回指定的系统目录。
*/
int WRE_GetDefaultConnectLinkIndex(DIAL_TYPE NetType)
{
    int iRes = -1;
    switch(NetType)
    {
    case DIAL_WAP:
        iRes = 0;
        break;
    case DIAL_NET:
        iRes = 2;
        break;
    default:
        break;
    }
    
    return iRes;
}

/*********************************************************************
*  WRE_Get_SYS_Dir
*
* 获得WRE系统目录
*
* 参数
* 
*   [in] 目录类型 内核或者应用目录
* 返回值
*  返回指定的系统目录。
*/
MMIWRE_RUNTIME_PATH_T g_wre_Runtime_Path = {0};
extern BYTE g_strDebugParam[WRE_DEBUG_TOTAL];

const uint16 *WRE_Get_SYS_Dir(WRE_SYS_PATH_TYPE sysPathType)
{
    if (sysPathType >= WRE_SYS_PATH_KERNEL && sysPathType < WRE_SYS_PATH_TOTAL)
    {
        if (MMIWRE_GetDebugMode())
        {
            if (sysPathType == WRE_SYS_PATH_KERNEL)
            {
                if (g_strDebugParam[WRE_DEBUG_STARTPATH] == 0)
                {
                    return L"e:\\wre";
                }
                else if (g_strDebugParam[WRE_DEBUG_STARTPATH] == 1)
                {                    
                    return L"d:\\wre";
                }                
                else if (g_strDebugParam[WRE_DEBUG_STARTPATH] == 2)
                {
                    return L"c:\\wre";
                }
                else
                {
                    return L"e:\\wre";
                }
            }
        }

        if (sysPathType == WRE_SYS_PATH_KERNEL)
        {
             return g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR];
        }
        else if (sysPathType == WRE_SYS_PATH_STORE)
        {
             return g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR];
        }
        else if (sysPathType == WRE_SYS_PATH_WREAPP)
        {
             return g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR];
        }
        else if ((sysPathType == WRE_SYS_PATH_KERNEL_UPDATE)
            ||(sysPathType == WRE_SYS_PATH_STORE_UPDATE))
        {
            return WRE_Get_SYS_Dir_Update(sysPathType);
        }
        else if (sysPathType == WRE_SYS_PATH_KERNEL_PREV)
        {
            return g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_PREV];
        }
        else if (sysPathType == WRE_SYS_PATH_STORE_PREV)
        {
            return g_wre_Runtime_Path.store_path[RUNTIMEPATH_PREV];
        }
        else
        {
            return s_wre_sys_path[sysPathType];
        }        
    }   
    
    return NULL;
}

BOOLEAN WRE_Set_SYS_Dir(WRE_SYS_PATH_TYPE sysPathType, uint16 *lpSysPath)
{
    //SCI_TRACE_LOW:"WRE_Set_SYS_Dir: sysPathType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_331_112_2_18_3_10_34_42,(uint8*)"d", sysPathType);
    WRE_TraceWstr(lpSysPath);

    if (lpSysPath == NULL)
    {
        return FALSE;
    }

    if (sysPathType == WRE_SYS_PATH_KERNEL)
    {
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_PREV], g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR]);
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.kernel_path[RUNTIMEPATH_CUR], lpSysPath);
    }
    else if (sysPathType == WRE_SYS_PATH_STORE)
    {
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_PREV], g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR]);
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.store_path[RUNTIMEPATH_CUR], lpSysPath);
    }
    else if (sysPathType == WRE_SYS_PATH_WREAPP)
    {
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_PREV], g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR]);
        MMIAPICOM_Wstrcpy(g_wre_Runtime_Path.app_path[RUNTIMEPATH_CUR], lpSysPath);
    }
    else
    {
        return FALSE;
    }
    
    MMIWRE_WriteNV(MMIWRE_NV_RUNTIME_PATH);

    HideWreDirectroy(TRUE);

    return TRUE;
}

uint16 *WRE_Get_SYS_Dir_Update(WRE_SYS_PATH_TYPE sysPathType)
{
    static uint16 sysDirUpdate[32] = {0};
    BOOL bRes = FALSE;
    memset(sysDirUpdate, 0, sizeof(sysDirUpdate));

    //SCI_TRACE_LOW:"WRE_Get_SYS_Dir_Update: sysPathType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_372_112_2_18_3_10_34_43,(uint8*)"d", sysPathType);

    if (sysPathType == WRE_SYS_PATH_KERNEL_UPDATE)
    {
        MMIAPICOM_Wstrcpy(sysDirUpdate, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    }
    else if (sysPathType == WRE_SYS_PATH_STORE_UPDATE)
    {
        MMIAPICOM_Wstrcpy(sysDirUpdate, WRE_Get_SYS_Dir(WRE_SYS_PATH_STORE));
    }
    else
    {
        //SCI_TRACE_LOW:"WRE_Get_SYS_Dir_Update Fail: not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_384_112_2_18_3_10_34_44,(uint8*)"");
        return NULL;
    }

    if (!WRE_IsSystemPath(sysDirUpdate) && WRE_IsDiskFreeSpaceEnough(sysPathType, sysDirUpdate))
    {
        bRes = TRUE;        
    }
    else
    {
        uint16 device = WRE_GetUpdateDevice(sysPathType);
        if (0 != device)
        {
            *sysDirUpdate = device;            
            bRes = TRUE;
        }
        else
        {
            bRes = FALSE;
        }
    }

    if (bRes)
    {
        WRE_TraceWstr(sysDirUpdate);        
        return sysDirUpdate;
    }
    else
    {
        //SCI_TRACE_LOW:"WRE_Get_SYS_Dir_Update Fail:"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_413_112_2_18_3_10_34_45,(uint8*)"");
        return NULL;
    }

}

/*********************************************************************
*  WRE_Get_Preload_Dir
*
* 获得WRE系统目录
*
* 参数
* 
*   [in] 目录类型 内核或者应用目录
* 返回值
*  返回指定的系统目录。
*/
BOOLEAN WRE_Get_Preload_Dir(LPPRELOAD_INFO lpPreloadInfo, WRE_SYS_PATH_TYPE sysPathType)
{
    BOOLEAN bRes = TRUE;

    //SCI_TRACE_LOW:"WRE_Get_Preload_Dir: sysPathType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_433_112_2_18_3_10_34_46,(uint8*)"d", sysPathType);

    if (lpPreloadInfo == NULL)
    {
        return FALSE;
    }

    if (sysPathType == WRE_SYS_PATH_KERNEL)
    {        
        if (WRE_GetFeatureSupport(WRE_FEATURE_KERNEL_PRELOAD))
        {
#if defined(WRE_WREKERNEL_PRELOAD)||defined(WRE_WREKERNEL_NORFLASH)
            MMIAPICOM_Wstrcpy(lpPreloadInfo->PreloadPath, WRE_WREKERNEL_PRELOAD_PATH);
            lpPreloadInfo->PreloadType = WRE_WREKERNEL_PRELOAD_TYPE;
#endif            
        }
        else
        {
            bRes = FALSE;
        }
    }    
    else if (sysPathType == WRE_SYS_PATH_STORE)
    {
        if (WRE_GetFeatureSupport(WRE_FEATURE_STORE_PRELOAD))
        {
#ifdef WRE_WRESTORE_PRELOAD            
            MMIAPICOM_Wstrcpy(lpPreloadInfo->PreloadPath, WRE_WRESTORE_PRELOAD_PATH);
            lpPreloadInfo->PreloadType = WRE_WRESTORE_PRELOAD_TYPE;
#endif            
        }
        else
        {
            bRes = FALSE;
        }
    }   
    else if (sysPathType == WRE_SYS_PATH_WREAPP)
    {
        if (WRE_GetFeatureSupport(WRE_FEATURE_APP_PRELAOD))
        {
        }
        else
        {
            bRes = FALSE;
        }
    }
    else
    {
        bRes = FALSE;
    }    
    
    WRE_TraceWstr(lpPreloadInfo->PreloadPath);
    //SCI_TRACE_LOW:"WRE_Get_Preload_Dir: PreloadType = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_480_112_2_18_3_10_34_47,(uint8*)"d", lpPreloadInfo->PreloadType);

    return bRes;
}

/*********************************************************************
*  WRE_GetPhoneInfo
*
* 获取手机配置类信息,如屏幕、键盘类型等
*
* 参数
* info
*   [out] buf,是which对应的结构类型指针,其返回的内容需由参数which对应的info结构来解析
* len
*   [in] buf的长度,其值须为info的结构类型长度
* which
*   [in] 表示想get哪种PhoneInfo,which为枚举类型-PhoneInfoType
* 返回值
*  TRUE--- 成功,参数info返回PhoneInfo数据
*  FALSE-- 失败
*/
BOOLEAN WRE_GetPhoneInfo(char* info,int len,int which)
{
    if(info == NULL || info == (char*)-1)
    {
        return FALSE;
    }
    if (which < 0 || which > INFO_TYPE_MAX)
    {
        return FALSE;
    }
    switch(which)
    {
    case SCREEN_TOUCH_TYPE:
        {
            SCREEN_TOUCH *touch_type;
            if (len != (int)sizeof(SCREEN_TOUCH))
            {
                return FALSE;
            }
            touch_type = (SCREEN_TOUCH *)info;
            *touch_type = CAN_TOUCH;       //please config SCREEN_TOUCH_TYPE here
            break;
        }
    case KEYBOARD_TYPE:
        {
            KEYBOARD* keyboard;
            if (len != (int)sizeof(KEYBOARD))
            {
                return FALSE;
            }
            keyboard = (KEYBOARD *)info;
#if defined KEYPAD_TYPE_TYPICAL_KEYPAD
            *keyboard = KEYPAD_TYPICAL; 
            break;
#elif defined KEYPAD_TYPE_QWERTY_KEYPAD 
            *keyboard = KEYPAD_QWERTY;
            break;
#elif defined KEYPAD_TYPE_COMPACT_KEYPAD 
            *keyboard = KEYPAD_COMPACT;
            break;
#elif defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
            *keyboard = FOUR_KEY_UNIVERSE;
            break;
#else
            *keyboard = KEYPAD_CUSTOMIZE;
            break;
#endif
        }
    case WIFI_INFO_TYPE:
        {
            WIFI_STATE  * wifistate = (WIFI_STATE*)info;
#ifdef MMI_WIFI_SUPPORT
            if (MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
            {
                *wifistate = WIFI_CONNECTED;
            }
            else
            {
                *wifistate = WIFI_DISCONNECT;
            }
#else
            *wifistate = (WIFI_STATE)WIFI_NOT_SUPPORT;
#endif
            break;
        }	
	case SD_STAT_INFO_TYPE:
		{
			BOOLEAN *sdstate;
			
			if (len != (int)sizeof(BOOLEAN))
			{
				return FALSE;
			}
			
			sdstate = (BOOLEAN*)info;
			if (FALSE == MMISD_IsSDExistEx())
			{
				*sdstate = FALSE;
			}
			else
			{
				*sdstate = TRUE;
			}
			
			break;
		}
	case BACKLIGHT_INFO_TYPE:
		{
			BOOLEAN *backlight_state;
			
			if (len != (int)sizeof(BOOLEAN))
			{
				return FALSE;
			}
			
			backlight_state = (BOOLEAN*)info;
			
			if (FALSE == MMIDEFAULT_IsBacklightOn())
			{
				*backlight_state = FALSE;
			}
			else
			{
				*backlight_state = TRUE;
			}
			
			break;
		}
    case POWER_STATUS_TYPE:
        {
            POWER_STATE *p_power_state;
#ifdef MMI_DUAL_BATTERY_SUPPORT
            DBAT_STATE_INFO_T* dbat_info;
#else
    #ifndef WIN32
            extern BOOLEAN CHGMNG_IsChargeConnect(void);
    #endif
#endif

            if (len != (int)sizeof(POWER_STATE))
            {
                return FALSE;
            }
            p_power_state = (POWER_STATE *)info;
#ifdef MMI_DUAL_BATTERY_SUPPORT
            dbat_info=DBAT_GetState();
            if (dbat_info == NULL)
            {
                return FALSE;
            }
            p_power_state->charge_state = (dbat_info->module_state != DBAT_CONNECT_CHARGING? 0 : 1);
            p_power_state->bat_remain_cap = dbat_info->p_bat[dbat_info->cur_bat]->remain_cap;
            p_power_state->cur_vol = dbat_info->p_bat[dbat_info->cur_bat]->vol;
            p_power_state->bat_level = MMIAPIPHONE_GetBatLevel(p_power_state->cur_vol);
#else
            p_power_state->bat_remain_cap = CHR_GetBatCapacity();
            p_power_state->cur_vol = CHR_GetCurVoltage();
#ifdef WIN32
            p_power_state->charge_state = FALSE;
#else
            p_power_state->charge_state = CHGMNG_IsChargeConnect();
            p_power_state->charge_state = FALSE;
#endif            
            p_power_state->bat_level = MMIAPIPHONE_GetBatCapacity();
#endif
        }
        break;
    case SIGNAL_STRENGTH_TYPE:
        {
            SIGNAL_INFO* p_signal_state;
            MN_PHONE_SCELL_MEAS_T scell = {0};
            
#ifndef _WIN32
   #ifdef MMI_DUALMODE_ENABLE
            scell = em_GetRSSIEx(MMIAPISET_GetActiveSim());
   #else
            scell = MNPHONE_GetRSSIEx(MMIAPISET_GetActiveSim());
   #endif
#else
            scell.arfcn = 123;
            scell.rxlev =67;// 567;
#endif
            scell = MNPHONE_GetRSSIEx(MMIAPISET_GetActiveSim());
            if (len != (int)sizeof(MN_PHONE_SCELL_MEAS_T))
            {
                return FALSE;
            }
            p_signal_state = (SIGNAL_INFO*)info;
            memset(p_signal_state,0,len);
            p_signal_state->arfcn = scell.arfcn;
            p_signal_state->rxlev = scell.rxlev;
            //p_signal_state->cell_id = scell.cell_id;
            //p_signal_state->lac = scell.lac; 
        }
        break;
    case VIBRATE_SUPPORT_TYPE:
        {
            BOOLEAN *app_support;
			
            if (len != (int)sizeof(BOOLEAN))
            {
                return FALSE;
            }
			
            app_support = (BOOLEAN*)info;
/* #ifdef MCR_SUPPORT_VIBRATE */
#if 1
            *app_support = TRUE;
#else /* not define MCR_SUPPORT_VIBRATE */
            *app_support = FALSE;
#endif /* MCR_SUPPORT_VIBRATE */
        }
        break;
    case BLUETOOTH_SUPPORT_TYPE:
        {
            BOOLEAN *app_support;
			
            if (len != (int)sizeof(BOOLEAN))
            {
                return FALSE;
            }
			
            app_support = (BOOLEAN*)info;
#ifdef BLUETOOTH_SUPPORT
            *app_support = TRUE;
#else /* not define BLUETOOTH_SUPPORT */
            *app_support = FALSE;
#endif /* BLUETOOTH_SUPPORT */
        }
        break;
    case GPS_SUPPORT_TYPE:
        {
            BOOLEAN *app_support;
			
            if (len != (int)sizeof(BOOLEAN))
            {
                return FALSE;
            }
			
            app_support = (BOOLEAN*)info;
#ifdef MMI_GPS_SUPPORT
            *app_support = TRUE;
#else /* not define GPS_SUPPORT */
            *app_support = FALSE;
#endif /* GPS_SUPPORT */
        }
        break;
    case GRAVITY_SENSOR_TYPE:
        {
            BOOLEAN *app_support;
			
            if (len != (int)sizeof(BOOLEAN))
            {
                return FALSE;
            }
			
            app_support = (BOOLEAN*)info;
#ifdef MOTION_SENSOR_TYPE
            *app_support = TRUE;
#else /* not define MOTION_SENSOR_TYPE */
            *app_support = FALSE;
#endif /* MOTION_SENSOR_TYPE */
        }
        break;
     case VIRTUAL_DISK_TYPE:  
	{
#ifdef WRE_VIRTUALDISK_SUPPORT
          //SCI_TRACE_LOW:"[WRE]WRE_GetPhoneInfo: virtual disk: len =%d"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_CFG_746_112_2_18_3_10_35_48,(uint8*)"d", len);
           if (len < 6)
	   	return FALSE;
	     MMIAPICOM_Wstrcpy((uint16 *)info, WRE_RAM_DISK_NAME);
	     MMIAPICOM_Wstrcat((uint16 *)info, L":");
           return WRE_CreateVirtualRAMDisk();
#else
            return FALSE;
#endif
	}
	//break; // changed by yifei
     case CAMERA_STATUS_TYPE:
     {
         BOOLEAN *IsOpened;
			
         if (len != (int)sizeof(BOOLEAN))
         {
             return FALSE;
         }
         IsOpened = (BOOLEAN *)info;
         *IsOpened = MMIAPIDC_IsOpened();
     }
     break;

     default :
        return FALSE;
    }
    return TRUE;
}


/*****************************************************************************/
//  Description : Adapt_GetSysDisk
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint16 WRE_GetSysDisk(void)
{
    return SYS_DISK_CHAR;//0x0063;
}

/*****************************************************************************/
//  Description : Adapt_GetSDDisk
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
uint16 WRE_GetSDDisk(void)
{
    return SD_DISK_CHAR;//0x0065;
}

/*****************************************************************************/
//  Description : WRE_GetInstallDevice
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:
/*****************************************************************************/
WRE_FILE_DEVICE_E WRE_GetInstallDevice(uint16 *pSysPath)
{
    WRE_FILE_DEVICE_E store_idx = WRE_DEVICE_UDISK;
    //const uint16 *pSysPath = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL);
    if (pSysPath)
    {
        if ((*pSysPath == L'D' || *pSysPath == L'd') 
            && (*(pSysPath+1) == L':'))
        {
            store_idx = WRE_DEVICE_UDISK;
        }
        else if ((*pSysPath == L'E' || *pSysPath == L'e') 
        && (*(pSysPath+1) == L':'))
        {
            store_idx = WRE_DEVICE_SDCARD_1;
        }
        else if ((*pSysPath == L'F' || *pSysPath == L'f') 
        && (*(pSysPath+1) == L':'))
        {
            store_idx = WRE_DEVICE_SDCARD_2;
        }
        else
        {
        }
    }

    return store_idx;
}

BOOLEAN WRE_IsRuntimeEnvRestoreFromPreload()
{
#ifdef WRE_RUNTIME_ENVIRONMENT_PRELOAD
        return TRUE;
#else
        return FALSE;
#endif    
}

#define WRE_KERNEL_MINSPACE       (2*1024*1024)
#define WRE_STORE_MINSPACE        (1*1024*1024)
uint32 WRE_GetKernelMinSpace(WRE_SYS_PATH_TYPE sysPathType)
{
    if (sysPathType == WRE_SYS_PATH_KERNEL || sysPathType== WRE_SYS_PATH_KERNEL_UPDATE)
    {
        return WRE_KERNEL_MINSPACE;
    }
    else if (sysPathType == WRE_SYS_PATH_STORE || sysPathType == WRE_SYS_PATH_STORE_UPDATE)
    {
        return WRE_STORE_MINSPACE;
    }
    else
    {
        return 0;
    }
}


/************************************************************************/
//  Description : WRE_GetDownLoadLinkIndex
//  Global resource dependence : 
//  Author:yiwen.man
//  Note:下载WRE内核时的拨号索引，IDH请酌情配置
/************************************************************************/

typedef struct _tagWRE_DOWNLOAD_DIALPARAM
{
    PHONE_PLMN_NAME_E hplmn;     //运营商编号
	uint8   iNetIndex;           //默认可用拨号索引
}WRE_DOWNLOAD_DIALPARAM;

static WRE_DOWNLOAD_DIALPARAM  s_WreDownLoadDialParam[] = 
{
	{PLMN_CMCC, 2},   //中国移动net拨号
	{PLMN_UNICOM, 0}  //中国联通wap拨号
};

uint8 WRE_GetDownLoadLinkIndex(uint32 dual_sys)
{
	uint32 i = 0;
    PHONE_PLMN_NAME_E plmn = MMIAPIPHONE_GetNetworkName((MN_DUAL_SYS_E)dual_sys);
    for (; i < sizeof(s_WreDownLoadDialParam)/sizeof(WRE_DOWNLOAD_DIALPARAM); i++)
    {
		if ( s_WreDownLoadDialParam[i].hplmn== plmn)
		{
			return s_WreDownLoadDialParam[i].iNetIndex;
		}
    }
	
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

#ifdef WRE_PREINSTALL_APP_SUPPORT

//Please fill this list table with valid application guid and app id.
CONST FIXED_APP_INFO_T  fixed_app_list[MAX_WRE_FIXED_APP_NUM] =

{
//#ifdef WRE_STORE_OVERSEAS
//    {WRE_FIX_ID_FRUIT,   L"B27B6542-FAB3-4541-A10B-E4AF9EB05605", L"10200010000001"}, //水果忍者?#else
//	 {WRE_FIX_ID_FRUIT,   L"B27B6542-FAB3-4541-A10B-E4AF9EB05605", L"10200010000001"}, //水果忍者?
//#endif
	{WRE_FIX_ID_FRUIT,   L"B27B6542-FAB3-4541-A10B-E4AF9EB05605", L"10200010000001"},
//    {WRE_FIX_ID_LIGHT,  L"D3D34282-469F-4e53-9896-527A50685C11", L"10500010000002"}  //开关灯        
};
\
FIXED_APP_INFO_T *WRE_GetFixAppList()
{
    return fixed_app_list;
}
#endif

//wre是否支持联网
#define  WRE_NETCONNECT_SUPPORT     (TRUE)
//wre是否支持完整性检查
#define  WRE_CHECKSUM_SUPPORT       (TRUE)

#define WRE_ONEKEY_DOWNLOAD_SUPPORT (TRUE)

extern WRE_APP_PARAM_T wre_app_param;
uint8 WRE_GetFeatureSupport(WRE_FEATURE_TYPE future_type)
{
    uint8 uRes = 0;   
    
    switch(future_type)
    {
    case WRE_FEATURE_NETCONNECT:
        {
            uRes = WRE_NETCONNECT_SUPPORT;
        }
        break;
    case WRE_FEATURE_CHECKSUM:
        {
            uRes = WRE_CHECKSUM_SUPPORT;
        }
        break;
    case WRE_FEATURE_KERNEL_PRELOAD:
        {
#if defined(WRE_WREKERNEL_PRELOAD)||defined(WRE_WREKERNEL_NORFLASH)
                uRes = 1;
#else
                uRes = 0;
#endif
        }
        break;
    case WRE_FEATURE_STORE_PRELOAD:
        {
#ifdef WRE_WRESTORE_PRELOAD
                uRes = 1;
#else
                uRes = 0;
#endif
        }
        break;
    case WRE_FEATURE_APP_PRELAOD:
        {
#ifdef WRE_WREAPP_PRELOAD
                uRes = 1;
#else
                uRes = 0;
#endif
        }
        break;
    case WRE_FEATURE_FIXAPP:
        uRes = wre_app_param.bFixApp;
        SCI_TRACE_LOW("WRE_GetFeatureSupport: WRE_FEATURE_FIXAPP uRes = %d", uRes);
        break;
    case WRE_FEATURE_APPTYPE:
        uRes = wre_app_param.bStore;
        break;
    case WRE_FEATURE_ONEKEYDOWNLOAD:
        uRes = WRE_ONEKEY_DOWNLOAD_SUPPORT;
        break;
    default:
        break;
    }
    
    return uRes;

}

#endif
