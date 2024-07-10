#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
#include "os_api.h"
#include "mmk_app.h"
#include "mmi_module.h"
#include "string.h"
#include "scm_api.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmiwre_export.h"
#include "watchdog.h"

#include "../../../../../../wre/wrekernel//include/sdk/connect_type.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_mdu.h"
#include "mmiwre_dial.h"
#include "mmiwre_other.h"
#include "mmiwre.h"
#include "mmidc_setting.h"
#include "mmidc_export.h"
#include "mmimp3_export.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmiebook.h"
#include "mmiebook_export.h"
#include "mmidisplay_data.h"
#include "mmiwre_id.h"
#include "window_parse.h"
#include "guictrl_api.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "arm_mmu.h"
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif

#ifdef NES_SUPPORT
#include "xnes_init.h"
#endif
#include "mmu_drvapi.h" // changed by yifei
#if defined(QBTHEME_SUPPORT)
#include "mmiwre_qbtheme.h"
#endif
#define  WRE_HTTP_DOWNNLOAD_URL     "51wre.com"
LOCAL WRE_COMM Wre_Comm = {0};
LOCAL CMDPARA  CmdPara = {0};
//LOCAL uint16   sModuleName[128];


LOCAL int      wretomocorfuncList[WRE_TO_MOCOR_FUNC_MAX + 1] = {0};

extern const int* const _mocor_fptrunk[];


WRE_APP_PARAM_T       wre_app_param;
WRE_DOWNLOAD_PARAM_T wre_dload_param = {0};
#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
WRETOMORCOR_INFO_T wretomocorapp = {0};
#endif
int g_wre_kernel_norflash_startup = 0;
//WRE中间件程序入口
extern int hot_reset_tag;

/*****************************************************************************/
//  Description : GetDownLoadParam
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void * GetDownLoadParam(void)
{
    return &wre_dload_param;   
}

PUBLIC void InitDownLoadParam()
{
    uint8 uFlag = 0;
    int i = WRE_FEATURE_NETCONNECT;
    memset(&wre_dload_param, 0, sizeof(WRE_DOWNLOAD_PARAM_T));    

    for (; i < WRE_FEATURE_TOTAL; i++)
    {
        if (WRE_GetFeatureSupport(i))
        {
            wre_dload_param.cmd[1] |= (1<<(uFlag++));
        }
		else
		{
			wre_dload_param.cmd[1] |= (0<<(uFlag++));
		}
    }

    SCI_TRACE_LOW("InitDownLoadParam: wre_dload_param = 0x%X", wre_dload_param.cmd[1]);
}

PUBLIC void SetDownLoadFlag()
{
    wre_dload_param.cmd[0] = 1;
}

PUBLIC uint8 IsDownLoadFlag()
{
    return wre_dload_param.cmd[0];
}


PUBLIC BOOLEAN MMIWRE_SetAppParam(uint16 * apppath,
                                  uint16 * appname,
                                  uint16 * iconname)
{
    MMIAPICOM_Wstrcpy(wre_app_param.appname,appname);
    MMIAPICOM_Wstrcpy(wre_app_param.iconpath,iconname);
    MMIAPICOM_Wstrcpy(wre_app_param.apppath,apppath);

    return TRUE;
}
                                  

/*****************************************************************************/
//  Description : IsWRERunning
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN  WBOOT_IsWRERunning(void)
{
	

    if(wretomocorfuncList[0] == NULL)
        return FALSE;
    else
        return TRUE;

	
}


/*****************************************************************************/
//  Description : freewrefunctbl
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void freewrefunctbl(void)
{

    memset(&wretomocorfuncList,0,sizeof(DWORD)*(WRE_TO_MOCOR_FUNC_MAX + 1));

}


/*****************************************************************************/
//  Description : ConstructCmdPara
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int WRE_ConstructCmdPara(PWRE_APP_PARAM_T pAppParam,PWRE_COMM *pWre_Comm)
{
/*
    CmdPara.argc = argc;
    if(argv)
    {
        //memset(sModuleName, 0, 128);
        //MMIAPICOM_StrToWstr(argv,&sModuleName[0]);
        MMIAPICOM_Wstrcpy(sModuleName,argv);
        //strcpy((char *)sModuleName, argv);
        CmdPara.argv = &sModuleName;
    }
    else
    {
        CmdPara.argv = PNULL;
    }
    */
    Wre_Comm.pCmdPara = (void *)pAppParam;
    *pWre_Comm = &Wre_Comm;
    return 1;
}


/*****************************************************************************/
//  Description : ConstructMocorToWREPara
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static int ConstructMocorToWREPara(void)
{


    Wre_Comm.pMocorToWRE = (void *)_mocor_fptrunk;

    return 1;
}


/*****************************************************************************/
//  Description : ConstructWREToMocorPara
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static int ConstructWREToMocorPara(void)
{
	
    Wre_Comm.pWREToMocor = (void *)wretomocorfuncList;

    return 1;
}


/*****************************************************************************/
//  Description : ConstructConfigPara
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static int ConstructConfigPara(void)
{
    return 1;
}


/*****************************************************************************/
//  Description : GetAdaptorCommPara
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int GetAdaptorCommPara(PWRE_COMM *pWre_Comm, PWRE_APP_PARAM_T pAppParam)
{
    ConstructMocorToWREPara();
    ConstructWREToMocorPara();
    ConstructConfigPara();

    *pWre_Comm = &Wre_Comm;

    return 1;
}


/*****************************************************************************/
//  Description : DispatchMwinMSG
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/


/* mmk_msg.c need use */
typedef void (* DISPATCHMWIN_PROC)(void);
void WBOOT_DispatchMwinMSG(void)
{
    DISPATCHMWIN_PROC  pProc = PNULL;

    pProc = (DISPATCHMWIN_PROC)(wretomocorfuncList[DispatchMwinMSG_FUNC]);
    if (pProc)
    {
       pProc();
    }
}


/*****************************************************************************/
//  Description : GetMwinMessageForMocor
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef BOOLEAN (* GETMWINMESSAGEFORMOCOR_PROC)(void);
BOOLEAN WBOOT_GetMwinMessageForMocor(void)
{
    GETMWINMESSAGEFORMOCOR_PROC  pProc = PNULL;
    pProc = (GETMWINMESSAGEFORMOCOR_PROC)(wretomocorfuncList[GetMwinMessageForMocor_FUNC]);
    if(!pProc)
    {
        return FALSE;
    }
    return pProc();
}


/*****************************************************************************/
//  Description : MwinWin_HandleMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef MMI_RESULT_E (* MWINWIN_HANDLEMSG_PROC)(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
MMI_RESULT_E WBOOT_MwinWin_HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MWINWIN_HANDLEMSG_PROC  pProc = PNULL;
    pProc = (MWINWIN_HANDLEMSG_PROC)(wretomocorfuncList[MwinWin_HandleMsg_FUNC]);

    if(pProc)
        return pProc(win_id, msg_id, param);
    else
        return MMI_RESULT_FALSE;
}


/*****************************************************************************/
//  Description : _vsnwprintf
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef int (* _VSNWPRINTF_PROC)(wchar_t *buf, size_t cnt, const wchar_t *fmt, va_list args);
static int _vsnwprintf(wchar_t *buf, size_t cnt, const wchar_t *fmt, va_list args)
{
    _VSNWPRINTF_PROC  pProc = PNULL;
    pProc = (_VSNWPRINTF_PROC)(wretomocorfuncList[_vsnwprintf_FUNC]);
    return pProc(buf, cnt, fmt, args);
}

typedef MMI_RESULT_E (* MMIWRE_DELETEHEAP_PROC)(int flag);
MMI_RESULT_E MMIWRE_PowerOff(int flag)
{
    MMIWRE_DELETEHEAP_PROC  pProc = PNULL;
    pProc = (MMIWRE_DELETEHEAP_PROC)(wretomocorfuncList[WRE_PowerOff_FUNC]);

    if(pProc)
        return pProc(flag);
    else
        return MMI_RESULT_FALSE;
}
static BOOL g_bNewMallocVer = FALSE; //版本兼容考虑
typedef void *(* WRE_MALLOC_PROC)(uint32 nSize);
typedef void (* WRE_FREE_PROC)(void *lpMemPtr);



void* UserMemoryAllocate(unsigned int dwSize);
void UserMemoryFree(void* ptr);

void *wre_malloc(uint32 nSize)
{
    WRE_MALLOC_PROC pProc = PNULL;
    pProc = (WRE_MALLOC_PROC)(wretomocorfuncList[WRE_TO_MOCOR_MALLOC_FUNC]);
    if (pProc)
    {
        g_bNewMallocVer = TRUE;
        SCI_TRACE_LOW("[WRE]wre_malloc: new boot, new kernel");
        return pProc(nSize);
    }
    else // new boot, old kernel
    {
        g_bNewMallocVer = FALSE;
        SCI_TRACE_LOW("[WRE]wre_malloc: new boot, old kernel");
        return SCI_ALLOCA(nSize);
    }    
}


void wre_free(void *lpMemPtr)
{
    WRE_FREE_PROC pProc = PNULL;
    if (!g_bNewMallocVer)
    { // new boot, old kernel
        SCI_TRACE_LOW("[WRE]wre_free: new boot, old kernel");
        SCI_FREE(lpMemPtr);
    }
    else
    {
        SCI_TRACE_LOW("[WRE]wre_free: new boot, new kernel");
        pProc = (WRE_FREE_PROC)(wretomocorfuncList[WRE_TO_MOCOR_FREE_FUNC]);
        if (pProc)
        {
            pProc(lpMemPtr);
        }        
    }
}

/*****************************************************************************/
//  Description : swprintf
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/


/*****************************************************************************/
//  Description : WRE_SDCardInsert
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC long WRE_SDCardInsert(void)
{
    SCM_STATUS  scmstatus   =   SCM_STATUS_NORMAL;
#ifndef  WIN32
    scmstatus = SCM_GetSlotStatus(SCM_SLOT_0);
#endif
    //SCI_TRACE_LOW:"WRE_SDCardInsert return %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_294_112_2_18_3_10_30_0,(uint8*)"d", scmstatus);
    return ((scmstatus == SCM_STATUS_NORMAL) ? TRUE : FALSE);
}

typedef struct _tagWREKERNEL_INFO_T
{
    uint32  state; //0,未下载 ，1，一下载安装
    uint32  times; //使用次数, 每使用多少次进行一个KERNEL下载演示
    uint32  version;//版本 0:表示初试版本，需要下载，每次下载后版本需要升级，增加1
}WREKERNEL_INFO_T,*PWREKERNEL_INFO_T;
static WREKERNEL_INFO_T wre_kernel_info;


/*****************************************************************************/
//  Description : WRE_ReadInfo
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

LOCAL BOOLEAN WRE_ReadInfo(MMIFILE_HANDLE hfile,WREKERNEL_INFO_T* pInfo)
{
    uint32  dwReaded,dwNum = sizeof(WREKERNEL_INFO_T);
    MMIFILE_ERROR_E   error;

    error = MMIAPIFMM_ReadFile(hfile,pInfo,dwNum,&dwReaded,NULL);
    if(error == SFS_NO_ERROR && dwReaded == dwNum)
    {
        return TRUE;
    }
    else
        return FALSE;
}


/*****************************************************************************/
//  Description : WRE_WriteInfo
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

LOCAL void  WRE_WriteInfo(MMIFILE_HANDLE hfile,WREKERNEL_INFO_T * pInfo)
{
    uint32  dwWrited,dwNum = sizeof(WREKERNEL_INFO_T);
    MMIAPIFMM_WriteFile(hfile,pInfo,dwNum,&dwWrited,NULL);
}

#if 0
PUBLIC void UpdateVersionFlag(BOOLEAN  version_updated)
{
    MMIFILE_HANDLE   file_handle          = 0;  
    wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};

    swprintf(file_name,L"e:\\wre\\sys\\wre_cfg.dat");
    file_handle = MMIAPIFMM_CreateFile(file_name,SFS_MODE_READ |SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING, PNULL, PNULL);/*lint !e655 */
    
    //获得WRE系统配置数据
    if (SFS_INVALID_HANDLE != file_handle)
    {
        if(version_updated)
        {
            wre_kernel_info.version++;
            wre_kernel_info.times++;
            wre_kernel_info.state = 1;
        }
        else
        {
            wre_kernel_info.times ++;
        }

        WRE_WriteInfo(file_handle,&wre_kernel_info);        

        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"WARNING!!! update fail!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_367_112_2_18_3_10_30_1,(uint8*)"");
    }
    return;
}
#endif


/*****************************************************************************/
//  Description : CheckWREVerMatch
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

extern int MiniInstaller_Is_WRE_OK(void);
extern BOOLEAN WRE_IsRuntimeEnvRestoreFromPreload(void);
extern BOOLEAN RestoreWREEnv(void);
BOOLEAN CheckWREVerMatch(void)
{ 
    BOOLEAN bRes = (BOOLEAN)MiniInstaller_Is_WRE_OK();
	uint8   ret;
	SCI_TRACE_LOW("[CheckWREVerMatch ] MiniInstaller_Is_WRE_OK =%d",bRes);
#ifdef WIN32
		return TRUE; 
#endif
	
	//check part2 if it is ready.
	if(bRes)
	{
		//WRE_Set_SYS_Dir(WRE_SYS_PATH_KERNEL,WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE));

		ret = MiniIntaller_Is_Store_OK(NULL);
		SCI_TRACE_LOW("[CheckWREVerMatch ] MiniIntaller_Is_Store_OK =%d",ret);
		if(ret == 0)
		{
			bRes =  TRUE;
		}
		else
		{
			bRes = FALSE;
		}
	}
	
    
#if !defined(WIN32)&&defined(WRE_VIRTUALDISK_SUPPORT)&&(defined(WRE_WREKERNEL_PRELOAD) || defined(WRE_WREKERNEL_NORFLASH))&&defined(WRE_WRESTORE_PRELOAD)
    if (!bRes)
    {
        wchar  path[256] = {0};
		SCI_TRACE_LOW("[CheckWREVerMatch ] WRE_WREKERNEL_NORFLASH");
       	MMIAPICOM_Wstrcpy(path, WRE_ROM_DISK_NAME);
    	MMIAPICOM_Wstrcat(path, L":\\wre");
        WRE_Set_SYS_Dir(WRE_SYS_PATH_KERNEL, path);
        WRE_Set_SYS_Dir(WRE_SYS_PATH_STORE, path);
        wre_app_param.bFixApp = TRUE;
        bRes = TRUE;        
		g_wre_kernel_norflash_startup = 1;
		SCI_TRACE_LOW("[NORFLASHTAG1 ] %d",g_wre_kernel_norflash_startup);
    }
	else
	{
		SCI_TRACE_LOW("[CheckWREVerMatch ] WRE_WREKERNEL_TFLASH");
		g_wre_kernel_norflash_startup = 0;
	}
#endif    

    return bRes;
}


/*****************************************************************************/
//  Description : DownloadAndInstallWRE
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void DownloadAndInstallWRE(BOOLEAN bStartup)
{
    if (!ShowLoading(bStartup))//run after installing
    {
        MMK_CloseApplet(SPRD_WRE_APPLET_ID);
    }
}



/*****************************************************************************/
//  Description : WRE_InstallKernel
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//Install wre kernel
extern uint16 *s_update_kernel_path;
#if 0
BOOLEAN WRE_InstallKernel(wchar * sourcefile,uint32 simNo,uint32 is_wifi_used,uint32 link_idx,uint32 is_net_type )
{
    int ret = 0;
    wchar  syspath[100] = {0};
    wchar  destfile[100] ={0};
#ifdef WIN32
    MMIAPICOM_Wstrcat(syspath,L"e:\\wre");
    MMIAPICOM_Wstrcat(destfile,L"e:\\wre");
#else
    //WRE_Get_SYS_Dir(syspath,100);
    //WRE_Get_SYS_Dir(destfile,100);
    MMIAPICOM_Wstrcpy(syspath, s_update_kernel_path);
    MMIAPICOM_Wstrcpy(destfile, s_update_kernel_path);
#endif

    //MMIAPICOM_Wstrcat(destfile,L"\\wrepart1.wpk.szip");
    MMIAPICOM_Wstrcat(destfile, L"\\");
    MMIAPICOM_Wstrcat(destfile, WRE_SYS_FILE_NAME_KERNELL_SZIP);                

    //SCI_TRACE_LOW:"enter WRE_InstallKernel!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_446_112_2_18_3_10_30_2,(uint8*)"");
    ret = unzip(sourcefile, destfile);
    if(ret)
    {
        //SCI_TRACE_LOW:"unzip failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_450_112_2_18_3_10_30_3,(uint8*)"");
				SFS_DeleteFile(sourcefile,NULL);
        return FALSE;
    }
    //SCI_TRACE_LOW:"WRE_InstallKernel unzip ok!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_454_112_2_18_3_10_30_4,(uint8*)"");
    SFS_DeleteFile(sourcefile,NULL);

    ret = unpack(destfile,syspath);
    if(ret)
    {
        //SCI_TRACE_LOW:"unpack failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_460_112_2_18_3_10_30_5,(uint8*)"");
			SFS_DeleteFile(destfile,NULL);
        return FALSE;
    }
    //SCI_TRACE_LOW:"WRE_InstallKernel unpack ok!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_464_112_2_18_3_10_30_6,(uint8*)"");
    SFS_DeleteFile(destfile,NULL);

    wre_dload_param.is_net_type = is_net_type;
    wre_dload_param.simNo       = simNo;
    wre_dload_param.is_wifi_used = is_wifi_used;
    wre_dload_param.link_index  =link_idx;
    strcpy(wre_dload_param.server_ip, WRE_HTTP_DOWNNLOAD_URL);

    return TRUE;

}

#endif
//Install kernel or store
BOOLEAN WRE_InstallKernelOrStore(wchar * sourcefile,uint32 step)
{
    int ret = 0;
    wchar  syspath[100] = {0};
    wchar  destfile[100] ={0};

	s_update_kernel_path = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL_UPDATE);

#ifdef WIN32
    MMIAPICOM_Wstrcat(syspath,L"e:\\wre");
    MMIAPICOM_Wstrcat(destfile,L"e:\\wre");
#else
    //WRE_Get_SYS_Dir(syspath,100);
    //WRE_Get_SYS_Dir(destfile,100);
    MMIAPICOM_Wstrcpy(syspath, s_update_kernel_path);
    MMIAPICOM_Wstrcpy(destfile, s_update_kernel_path);
#endif

    //MMIAPICOM_Wstrcat(destfile,L"\\wrepart1.wpk.szip");
    MMIAPICOM_Wstrcat(destfile, L"\\");
	if(step == ONEKEY_DOWNLOAD_STEP_PART1)
	{
		MMIAPICOM_Wstrcat(destfile, WRE_SYS_FILE_NAME_KERNELL_SZIP);                
	}
	else
	{
		MMIAPICOM_Wstrcat(destfile, WRE_SYS_FILE_NAME_STORE_SZIP);                
	}

    //SCI_TRACE_LOW:"enter WRE_InstallKernel!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_446_112_2_18_3_10_30_2,(uint8*)"");
    ret = unzip(sourcefile, destfile);
    if(ret)
    {
        //SCI_TRACE_LOW:"unzip failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_450_112_2_18_3_10_30_3,(uint8*)"");
		SFS_DeleteFile(sourcefile,NULL);
        return FALSE;
    }
    //SCI_TRACE_LOW:"WRE_InstallKernel unzip ok!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_454_112_2_18_3_10_30_4,(uint8*)"");
    SFS_DeleteFile(sourcefile,NULL);

    ret = unpack(destfile,syspath);
    if(ret)
    {
        //SCI_TRACE_LOW:"unpack failure!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_460_112_2_18_3_10_30_5,(uint8*)"");
			SFS_DeleteFile(destfile,NULL);
        return FALSE;
    }
    //SCI_TRACE_LOW:"WRE_InstallKernel unpack ok!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_464_112_2_18_3_10_30_6,(uint8*)"");
    SFS_DeleteFile(destfile,NULL);
    return TRUE;

}
/*****************************************************************************/
//  Description : UpdateWRE
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/


/*****************************************************************************/
//  Description : UninstallWRE
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void UninstallWRE(void)
{
    //wchar    file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    
    SFS_RenameFile(L"e:\\wre\\sys\\wrekernel.wrd", L"e:\\wre\\sys\\wre.rar", NULL);
}



/*****************************************************************************/
//  Description : WRE_EntryApp
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
int WRE_EntryApp(void)
{
    int ret = WRE_INIT_OK;    

    if(0 == wre_app_param.argc)
    {
        MMIAPICOM_Wstrcpy(wre_app_param.apppath,WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
        MMIAPICOM_Wstrcat(wre_app_param.apppath,L"\\appstore.exe");
    }

    ret = wre_startup(&wre_app_param);

#ifdef WRE_AUTO_STORE_SREEN_FIX //add for topwise 20120521  ---- 解决store分辨率不匹配的情况
	if(ret == WRE_INIT_OK)
	{
		uint16 *p = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL);
		SCI_TRACE_LOW("[wre]at WRE_EntryApp, check is T card,path = %s",p);
		if(p != NULL && p[0] == 'e') /*lint !e774*/
		{//wre目录在T卡	
			wre_load_store_ver_dat(p);	
		}
	}
#endif
    
    return ret;
}

BOOL WRE_IsFileExist(uint32 *lpFilePath)
{
    SFS_HANDLE  hFile = PNULL;
    
    if (lpFilePath == NULL)
        return FALSE;

    hFile = SFS_CreateFile((unsigned short*)lpFilePath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, PNULL, PNULL);
    if (hFile == NULL)
    {
        return FALSE;
    }

    SFS_CloseFile(hFile);

    return TRUE;
}

/*****************************************************************************/
//  Description : WREEntry
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
extern void MMIWRE_PrintNVInfo(void);

PUBLIC int WREMonkeyTest_EXE(const uint16 *asc_exe_name)
{
    int ret = WRE_INIT_OK;
    uint16 exe_name[256] = {0};
	uint16 appGuid[256] = {0};
	uint32  appid = 0;
    if(NULL == asc_exe_name )//|| TRUE == WRE_CheckUDiskRunState())
        return WRE_INIT_UDISK_NORUNNING;

    if(TRUE == WBOOT_IsWRERunning())
        return WRE_INIT_RUNNING;

    MMIAPICOM_StrToWstr(asc_exe_name,exe_name);
	SCI_TRACE_LOW(" WREMonkeyTest Start:exe_name  == %s",asc_exe_name);

	MMIAPICOM_Wstrcpy(wre_app_param.apppath,L"e:\\wreapp\\");
    MMIAPICOM_Wstrcat(wre_app_param.apppath,exe_name);
    MMIAPICOM_Wstrcat(wre_app_param.apppath,L"\\");
    MMIAPICOM_Wstrcat(wre_app_param.apppath,exe_name);
    MMIAPICOM_Wstrcat(wre_app_param.apppath,L".exe");
	MMIAPICOM_Wstrcpy(exe_name,wre_app_param.apppath);

	FindAppGUIDByName(exe_name,&appid,appGuid,wre_app_param.apppath,wre_app_param.iconpath,wre_app_param.appname);
	
	/*
	MMIAPICOM_Wstrcpy(wre_app_param.iconpath, wre_app_param.apppath);
    MMIAPICOM_Wstrcat(wre_app_param.apppath,L".exe");
	MMIAPICOM_Wstrcat(wre_app_param.iconpath,L".png");
    MMIAPICOM_Wstrcpy(wre_app_param.appname, exe_name);
    MMIAPICOM_Wstrcat(wre_app_param.appname,L".exe");
   
    wre_app_param.argc = TYPE_WREAPP_APP; //不走下载
    wre_app_param.bFixApp = FALSE;  //从T卡加载
	wre_app_param.bStore  = FALSE;
	 
    ret = WRE_EntryApp();
	*/
	MMIWRE_DynStartup(appid, wre_app_param.appname);
    SCI_TRACE_LOW("WREMonkeyTest_EXE == %d",ret);
   
    return ret;
}
PUBLIC int WREEntry(int argc,void * argv)
{
    int          ret = WRE_INIT_OK;
    uint16       appname[32];
    MMI_STRING_T prompt_str = {0};
    BOOL bWREVerMatch = FALSE;
#if !defined(WIN32)&&defined(WRE_VIRTUALDISK_SUPPORT)
    MMIWRE_ResetRuntimePath();  /*lint !e746*/
#endif

    wre_app_param.bFixApp = FALSE;
    wre_app_param.bStore = FALSE;

	

	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_669_112_2_18_3_10_30_8,(uint8*)"");
	//check if onekey downloading is on
	{
		extern BOOLEAN  WRE_IsBkDownloadOn(void);
		if(TRUE == WRE_IsBkDownloadOn())
		{
			return ret;
		}

	}

    bWREVerMatch = CheckWREVerMatch();
       

	//bWREVerMatch = TRUE;
//#endif
    //MMI_GetLabelTextByLang(TXT_WRE_ENTERTAINMENT, &prompt_str);         
    MMIWRE_PrintNVInfo();

    wre_app_param.argc = argc;
    //SCI_TRACE_LOW:"WREEntry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_564_112_2_18_3_10_30_7,(uint8*)"");
    if(argc == 0)
    {
 
        if(wre_app_param.cmdline)
        {
            SCI_FREE(wre_app_param.cmdline);
            wre_app_param.cmdline = NULL;
        }
 
        MMIAPICOM_Wstrcpy(wre_app_param.appname,L"appstore");
        wre_app_param.iconpath[0] = 0;
        wre_app_param.bStore = TRUE;
    }
    else if( argc == TYPE_WREAPP_APP && argv != NULL)
    {
        //MMIAPICOM_Wstrcpy(wre_app_param.guid,argv); 
        if(wre_app_param.cmdline)
        {
            SCI_FREE(wre_app_param.cmdline);
            wre_app_param.cmdline = NULL;
        }  

        wre_app_param.bFixApp = FALSE;
    }
    else if( argc == TYPE_WREFIX_APP)
    {
        uint16 * guid_str = SCI_ALLOC(40*sizeof(uint16));
        uint16 * store_id_str = SCI_ALLOC(40*sizeof(uint16));
        BOOL bRes = FALSE;
        BOOL bRet = FALSE;
        if( FALSE == MMIWRE_GetFixAppGuid(guid_str,store_id_str,(uint32)argv) )
        {
            SCI_FREE(guid_str);
            SCI_FREE(store_id_str);
            return WRE_INIT_APP_PARAERR;//参数错误，或者无法使用
        }

        bRes = FindAppPathByGUID(guid_str,wre_app_param.apppath,wre_app_param.appname,wre_app_param.iconpath);
        if(!bRes)
        {
            bRet = TRUE;
        }
        else
        {
            if(!WRE_IsFileExist((uint32 *)wre_app_param.apppath))
            {
                bRet = TRUE;
            }
        }
        if(bRet)
        {

            if(wre_app_param.cmdline)
            {
                SCI_FREE(wre_app_param.cmdline);
                wre_app_param.cmdline = NULL;
            }     

            MMIAPICOM_Wstrcpy(wre_app_param.apppath,WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
            MMIAPICOM_Wstrcat(wre_app_param.apppath,L"\\appstore.exe");

            MMIAPICOM_Wstrcpy(wre_app_param.appname,prompt_str.wstr_ptr);

            wre_app_param.iconpath[0] = 0;

            //set param
            wre_app_param.cmdline = SCI_ALLOC(200);
            MMIAPICOM_Wstrcpy((uint16*)wre_app_param.cmdline,guid_str);
            MMIAPICOM_Wstrcat((uint16*)wre_app_param.cmdline,L" ");           
            MMIAPICOM_Wstrcat((uint16*)wre_app_param.cmdline,store_id_str);

            //进入kernel下载
            wre_app_param.argc = 0;         
        }
        else
        {               
            wre_app_param.argc = 1;         
        }
        SCI_FREE(guid_str);
        SCI_FREE(store_id_str);

        wre_app_param.bFixApp = TRUE;
    }
    else if(argc == TYPE_WRE_DATA_APP)
    {
        uint32 len;

        if(wre_app_param.cmdline)
        {
           SCI_FREE(wre_app_param.cmdline);
           wre_app_param.cmdline = NULL;
        }
        if(argv != NULL)
        {
           len = SCI_STRLEN((char *)argv);	// changed by yifei
           wre_app_param.cmdline = (uint8 *)SCI_ALLOC((len + 1) * sizeof(uint16));
           memset((uint8 *)wre_app_param.cmdline, 0, (len + 1) * sizeof(uint16));
           MMIAPICOM_StrToWstr((uint8 *)argv, (wchar *)wre_app_param.cmdline);
        }

        wre_app_param.bFixApp = TRUE;

        InitDownLoadParam();

		return WRE_EntryApp();
    }
    else
    {
        //参数错误
        return WRE_INIT_APP_PARAERR;
    }
    //SCI_TRACE_LOW:"WREEntry1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_669_112_2_18_3_10_30_8,(uint8*)"");

    if(TRUE == WBOOT_IsWRERunning())
    {
        //SCI_TRACE_LOW:"[WRE]wre is running!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_673_112_2_18_3_10_30_9,(uint8*)"");
        
        //恢复最小化应用
        if(TRUE == MMIWRE_MaxmizeAPPWithName(wre_app_param.appname))
        {
            //SCI_TRACE_LOW:"[WRE]RestoreMiniWin "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_678_112_2_18_3_10_30_10,(uint8*)"");
            
            return WRE_INIT_OK;
        }else{
            WRE_EntryApp();
        }


        return WRE_INIT_RUNNING;
    }
    //>> added by yiwen.man 2011-7-21

    InitDownLoadParam();

#if 1
    if(TRUE == bWREVerMatch)    /*lint !e506 !e774*/
#else
    if(1)   /*lint !e506 !e774*/
#endif
    {
	 #ifdef WRE_AUTO_STORE_SREEN_FIX //add for topwise 20120521  ---- 解决store分辨率不匹配的情况
		if(argc == 0)
		{//启动store才判断分辨率
			uint16 *p = WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL);
			SCI_TRACE_LOW("[wre]check IsStoreScreenFix,path = %s",p);
			if(p != NULL && p[0] == 'e')
			{//wre目录在T卡
				int t;
				t = WRE_IsStoreScreenFix();
				if(t == STORE_SCREEN_NOT_FIXED)
				{
					OpenWREQueryWin(0);  /*lint !e746*/
					return WRE_INIT_QUERY_UPDATE;
				}
				else if(t == STORE_SCREEN_NOT_SURE)
				{
					OpenWREQueryWin(1);  /*lint !e746*/
					return WRE_INIT_QUERY_UPDATE;
				}
				
			}
		}
#endif
        ret = WRE_EntryApp();
    }
    else
    {
        hot_reset_tag = 0;
        //SCI_TRACE_LOW:"WREEntry DownloadAndInstallWRE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_703_112_2_18_3_10_31_11,(uint8*)"");
        SetDownLoadFlag();
        DownloadAndInstallWRE(TRUE);
    }
    return ret;
}


/*****************************************************************************/
//  Description : MWIN_XXX_PostMessage
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/


typedef BOOLEAN (* MWIN_XXX_POSTMESSAGE_PROC)(uint32 hwnd, uint32 Msg, uint32 wParam, uint32 lParam);
BOOLEAN WBOOT_MWIN_XXX_PostMessage(uint32 hwnd, uint32 Msg, uint32 wParam, uint32 lParam)
{
    /*
    return PostMessage((HWND)hwnd,(UINT)Msg,(WPARAM)wParam,(LPARAM)lParam); 
    */
    MWIN_XXX_POSTMESSAGE_PROC  pProc = PNULL;
    pProc = (MWIN_XXX_POSTMESSAGE_PROC)(wretomocorfuncList[MWIN_XXX_PostMessage_FUNC]);
    if(pProc)
    {
        return pProc(hwnd, Msg, wParam, lParam);
    }
    else
    {
        return FALSE;
    }
}

#if 0

LOCAL MMI_RESULT_E MWIN_ASYNCSOCK_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);


/*****************************************************************************/
//  Description : MWIN_ASYNCSOCK_HandlePsMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/




//PUBLIC MMI_APPLICATION_T       g_caf_asyn_app = {MWIN_ASYNCSOCK_HandlePsMsg, CT_APPLICATION, PNULL};       // g_caf_asyn_app define
LOCAL MMI_RESULT_E MWIN_ASYNCSOCK_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E      result = MMI_RESULT_FALSE;
    MmiOtherTaskMsgS* signal_ptr = (MmiOtherTaskMsgS*)param;
    
    //UNUSED(param);	// changed by yifei
    
    switch( msg_id )
    {
        case APP_WASYN_MSG_IND:
            SCI_ASSERT( PNULL != signal_ptr );  /*assert verified*/
            //SCI_TRACE_LOW:"ASYNCSOCK_HandlePsMsg, signal_ptr->msg_id = %lx"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_755_112_2_18_3_10_31_12,(uint8*)"d", signal_ptr->msg_id);
            MWIN_XXX_PostMessage((uint32)signal_ptr->handle,(uint32)signal_ptr->msg_id,(uint32)(signal_ptr->param_ptr),(uint32)(signal_ptr->size_of_param));
            break;
        default:
            break;
    }
    
    return result;
}
#endif

/*****************************************************************************/
//  Description : ClearThreadNormalQueue
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/


typedef void (* CLEARTHREADNORMALQUEUE_PROC)(uint32 threadId);
void WBOOT_ClearThreadNormalQueue(uint32 threadId)
{
    CLEARTHREADNORMALQUEUE_PROC  pProc = PNULL;
    pProc = (CLEARTHREADNORMALQUEUE_PROC)(wretomocorfuncList[ClearThreadNormalQueue_FUNC]);
    if (pProc)
        pProc(threadId);
}


/*****************************************************************************/
//  Description : GetWreCurMwinThd
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef uint32 (* GETWRECURMWINTHD_PROC)(void);
uint32 WBOOT_GetWreCurMwinThd(void)
{
    GETWRECURMWINTHD_PROC  pProc = PNULL;
    pProc = (GETWRECURMWINTHD_PROC)(wretomocorfuncList[GetWreCurMwinThd_FUNC]);
    if (pProc)
    {
        return pProc();
    }
    else
    {
        return 0;
    }
}
typedef int (*LD_GETMESSAGE_PROC)(void* p);
uint32 WRE_GetMessage(void* p)
{
    LD_GETMESSAGE_PROC  pProc = PNULL;
    pProc = (LD_GETMESSAGE_PROC)(wretomocorfuncList[WRE_LD_GetMessage_FUNC]);
    if (pProc)
    {
        return pProc(p);
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : IsMwinQuitMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef BOOLEAN (* ISMWINQUITMSG_PROC)(void);
BOOLEAN WBOOT_IsMwinQuitMsg(void)
{
    ISMWINQUITMSG_PROC  pProc = PNULL;
    pProc = (ISMWINQUITMSG_PROC)(wretomocorfuncList[IsMwinQuitMsg_FUNC]);
    if (pProc)
    {
        return pProc();
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : SetMwinMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef BOOLEAN (* SETMWINMSG_PROC)(void *ptr);
BOOLEAN WBOOT_SetMwinMsg(void *ptr)
{
    SETMWINMSG_PROC  pProc = PNULL;
    pProc = (SETMWINMSG_PROC)(wretomocorfuncList[SetMwinMsg_FUNC]);
    if (pProc)
    {
       return pProc(ptr);
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : MWIN_XXX_HandleSocketMsg
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef void (* MWIN_XXX_HANDLESOCKETMSG_PROC)(int sock,int event,DWORD error,void* param_ptr);
void WBOOT_MWIN_XXX_HandleSocketMsg(int sock,int event,int error,void* param_ptr)
{
    MWIN_XXX_HANDLESOCKETMSG_PROC  pProc = PNULL;
    pProc = (MWIN_XXX_HANDLESOCKETMSG_PROC)(wretomocorfuncList[MWIN_XXX_HandleSocketMsg_FUNC]);
    if (pProc)
        pProc(sock, event, error, param_ptr);
}


/*****************************************************************************/
//  Description : MWIN_GetAsyncTaskId
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef uint32 (* MWIN_GETASYNCTASKID_PROC)(void);
PUBLIC uint32 WBOOT_MWIN_GetAsyncTaskId(void)
{
    MWIN_GETASYNCTASKID_PROC  pProc = PNULL;
    pProc = (MWIN_GETASYNCTASKID_PROC)(wretomocorfuncList[MWIN_GetAsyncTaskId_FUNC]);
    if (pProc)
    {
       return pProc();
    }
    else
    {
        return 0;
    }
}


/*****************************************************************************/
//  Description : WRE_MSG_USB_Handle
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef void (* WRE_MSG_USB_HANDLE_PROC)(uint32 msg);
void WRE_MSG_USB_Handle(uint32 msg)
{
    WRE_MSG_USB_HANDLE_PROC  pProc = PNULL;
    pProc = (WRE_MSG_USB_HANDLE_PROC)(wretomocorfuncList[MSG_USB_Handle_FUNC]);
    if (pProc)  
        pProc(msg);

}


/*****************************************************************************/
//  Description : WRE_SmsReceiveCallBack
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef uint8 (* WRE_SmsReceiveCallBack_PROC)(uint32 simNo,char * tele_num,uint16 * sms_content,uint16 content_len);
uint8  WBOOT_WRE_SmsReceiveCallBack(uint32 simNo,char * tele_num,uint16 * sms_content,uint16 content_len)
{
    WRE_SmsReceiveCallBack_PROC  pProc = PNULL;
    pProc = (WRE_SmsReceiveCallBack_PROC)(wretomocorfuncList[WRE_SmsReceiveCallBack_FUNC]);
    if(pProc)
        return pProc(simNo,tele_num,sms_content,content_len);
    else
        return FALSE;

}


/*****************************************************************************/
//  Description : WRE_SendSmsCallback
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef void (* WRE_SendSmsCallback_PROC)(uint8 sendResult);
void WBOOT_WRE_SendSmsCallback(uint8 sendResult)
{
    WRE_SendSmsCallback_PROC  pProc = PNULL;
    pProc = (WRE_SendSmsCallback_PROC)(wretomocorfuncList[WRE_SendSmsCallback_FUNC]);
    if(pProc)
        pProc(sendResult);

}


/*****************************************************************************/
//  Description : WRE_MSG_Charge_Handle
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef void (* WRE_MSG_CHARGE_HANDLE_PROC)(uint32 msg);
void WRE_MSG_Charge_Handle(uint32 msg)
{
    WRE_MSG_CHARGE_HANDLE_PROC  pProc = PNULL;
    pProc = (WRE_MSG_CHARGE_HANDLE_PROC)(wretomocorfuncList[MSG_Charge_Handle_FUNC]);
    if (pProc)
        pProc(msg);

}


/*****************************************************************************/
//  Description : WRE_SetDeepSleepStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef void (* WRE_SetDeepSleepStatus_Proc)(uint8 sendResult);
void WBOOT_WRE_SetDeepSleepStatus(uint8 is_deep_sleep)
{
    WRE_SetDeepSleepStatus_Proc  pProc = PNULL;
    pProc = (WRE_SetDeepSleepStatus_Proc)(wretomocorfuncList[WRE_SetDeepSleepStatus_FUNC]);
    if(pProc)
        pProc(is_deep_sleep);

}
//extern MMI_HANDLE_T gWREWinHanlde;


/*****************************************************************************/
//  Description : boot_reboot
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

extern int gWR_MWIN_WIN_ID;
void boot_reboot(int  code ){
    //SCI_TRACE_LOW:"boot_reboot CALLED1"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_983_112_2_18_3_10_31_13,(uint8*)"");
    hot_reset_tag=1;
    //MwinWin_HandleMsg(0,WRE_MSG_CLOSE_WINDOW,0);
    //MMK_PostMsg(MMIMP3_ALL_MP3_LIST_OPT_WIN_ID, MSG_CLOSE_WINDOW, NULL, NULL);
    MMK_PostMsg(gWR_MWIN_WIN_ID, KEY_PRESSED|KEY_RED, NULL, NULL);
    MMK_PostMsg(gWR_MWIN_WIN_ID, KEY_RELEASED|KEY_RED, NULL, NULL);
    //SCI_TRACE_LOW:"boot_reboot END"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_989_112_2_18_3_10_31_14,(uint8*)"");
    //  if(VK_Code == VK_CANCEL && keys_status== WRE_KEY_PRESSED)
}


/*****************************************************************************/
//  Description : WRE_RegEventNotify
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef int (* WRE_REG_EVENT_NOTIFY_PROC)(uint32 msg);
int WBOOT_WRE_RegEventNotify(uint16 msg_id)
{
    WRE_REG_EVENT_NOTIFY_PROC pProc = PNULL;
    pProc = (WRE_REG_EVENT_NOTIFY_PROC)(wretomocorfuncList[WRE_RegEventNotify_FUNC]);
    if(pProc)
    {
        return pProc(msg_id);
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : WRE_LostFocusIsPenUped
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

typedef int (* WRE_LOSTFOCUSISPENUP_PROC)(void);
int WBOOT_WRE_LostFocusIsPenUped(void)
{
    WRE_LOSTFOCUSISPENUP_PROC pProc = PNULL;
    pProc = (WRE_LOSTFOCUSISPENUP_PROC)(wretomocorfuncList[WRE_LostFocusIsPenUped_FUNC]);
    if(pProc)
    {
        return pProc();
    }
    else
    {
        return 0;
    }
}

typedef int (*LPWRE_GetAppCount)(void);
extern void * OSAdapt_GetMiniBkFuncTbl(void);
int WRE_GetAppCount(void)
{
    int count = 0;
	

    MINI_BK_FUNC_T   *pBkFunc = (MINI_BK_FUNC_T*)(wretomocorfuncList[WRE_MiniBk_FUNCTBL]);

    if(pBkFunc)
    {
        LPWRE_GetAppCount  pGetCount  = (LPWRE_GetAppCount)(pBkFunc->pGetCount);/*lint !e611 */
        if(pGetCount == NULL){
            return 0;
        }
        count = pGetCount();
    }

    return count;
}

typedef BOOLEAN (*LPWRE_ShowApp)(void* hInstance,int opercode);
BOOLEAN WRE_ShowApp(void* app_handle,int opercode)
{

    MINI_BK_FUNC_T   *pBkFunc = (MINI_BK_FUNC_T*)(wretomocorfuncList[WRE_MiniBk_FUNCTBL]);




    LPWRE_ShowApp      pShowApp = (LPWRE_ShowApp)(pBkFunc->pShowApp);/*lint !e611 */

    return pShowApp(app_handle,opercode);
}

typedef BOOLEAN (*LPWRE_GetAppInfo)(int index,PWREAPP_INFO_T pInfo);
BOOLEAN WRE_GetAppInfo(int index,PWREAPP_INFO_T pInfo)
{

    MINI_BK_FUNC_T   *pBkFunc = (MINI_BK_FUNC_T*)(wretomocorfuncList[WRE_MiniBk_FUNCTBL]);
	
    LPWRE_GetAppInfo  pGetInfo = (LPWRE_GetAppInfo)(pBkFunc->pGetAppInfo);/*lint !e611 */
    if(pGetInfo == NULL){
            return NULL;
        }
    return pGetInfo(index,pInfo);
}

typedef uint32 (*LPWRE_GetWREStatus)(void);
uint32 WRE_GetRuningStatus(void)
{

    MINI_BK_FUNC_T   *pBkFunc = (MINI_BK_FUNC_T*)(wretomocorfuncList[WRE_MiniBk_FUNCTBL]);


    if(pBkFunc)
    {
        LPWRE_GetWREStatus      pGetStatus = (LPWRE_GetWREStatus)(pBkFunc->pGetWREStatus);/*lint !e611 */
          if(pGetStatus == NULL){
            return STATUS_WRE_NONE;
        }
        return pGetStatus();
    }
    else
    {
        return STATUS_WRE_NONE;
    }
}
void wre_kernel_pad1(void)
{
    //MMIWRE_CloseWin();     
}

typedef void (* KERNEL_PAD2_PROC)(void);
void wre_kernel_pad2(void)
{
    //KERNEL_PAD2_PROC pProc;
   // pProc = (KERNEL_PAD2_PROC)MMIWRE_RestoreWin;
    //pProc();
}
void wre_kernel_pad3(void)
{
}
void wre_kernel_pad4(void)
{
}
void wre_kernel_pad5(void)
{
}
void wre_kernel_pad6(void)
{
}
void wre_kernel_pad7(void)
{
}
void wre_kernel_pad8(void)
{
}
void wre_kernel_pad9(void)
{
}
void wre_kernel_pad10(void)
{
}
void wre_kernel_pad11(void)
{
}
void wre_kernel_pad12(void)
{
}
void wre_kernel_pad13(void)
{
}
void wre_kernel_pad14(void)
{
}
void wre_kernel_pad15(void)
{
}
void wre_kernel_pad16(void)
{
}

#ifndef WIN32
/*
extern void MMU_InvalideDCACHE(void);
extern void MMU_PHY_InvalideICACHEALL(void);
*/

/* 使用新的cache函数处理 */
//extern void MMU_IcacheSyncAll (void); // changed by yifei
//extern void MMU_DcacheSyncAll (void); // changed by yifei
#endif

/*****************************************************************************/
//  Description : wre_kernel_hook_proc
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
typedef void *(* HOOK_PROC)(int index);

void *wre_kernel_hook_proc(int index)
{
    HOOK_PROC retptr = NULL;

    switch(index)
    {
        case 1:
            retptr = (HOOK_PROC)DSystem_Install;
            break;
		case 2:
			//retptr = (HOOK_PROC)WRE_NetworkSelectGUI;
			break;

		case 3:
			//retptr = (HOOK_PROC)WRE_DownLoad_Window_Progress;
			break;
		case 4:
			//retptr = (HOOK_PROC)WRE_Get_Window_YY_Message;
			break;
		case 10:
#ifdef WIN32
#else		
            /*
			MMU_InvalideDCACHE();
			MMU_PHY_InvalideICACHEALL();	
            */
            /* 使用新的cache函数处理 */
            MMU_IcacheSyncAll();
            MMU_DcacheSyncAll();

#endif
			break;
#ifdef NES_SUPPORT
        case 101:
            retptr = (HOOK_PROC)xnes_get_frame_flush_cnt;
            break;
        case 102:
            retptr = (HOOK_PROC)xnes_next_frame;
            break;
        case 103:
            retptr = (HOOK_PROC)xnes_initialize;
            break;
        case 104:
            retptr = (HOOK_PROC)xnes_set_frame_skip;
            break;
        case 105:
            retptr = (HOOK_PROC)xnes_finalize;
            break;
        case 106:
            retptr = (HOOK_PROC)Xnes_All_Interface_init;
            break;
#endif
//hain
	#if defined(QBTHEME_SUPPORT)
        case 120:
            retptr = (HOOK_PROC)MMIAPIWRE_QBTHEME_Hook_Proc;
            break;
	#endif
        default:
            break;
    }
    return (void *)retptr;/*lint !e611*/
}
//////////////////////////////////////////////////////////////////////////
//#undef   MAX_PATH
//#define  MAX_PATH (148-1)

//static wchar g_LastPhotoPath[MAX_PATH]={0};
#if 0
typedef struct tagSHCAMERACAPTURE
{
    int         cbSize;
    int         hwndOwner;
    wchar       szFile[MAX_PATH];   
    wchar*      pszInitialDir;
    wchar*      pszDefaultFileName;
    wchar*      pszTitle;
    int         StillQuality;
    int         VideoTypes;
    int         nResolutionWidth;
    int         nResolutionHeight;
    int         nVideoTimeLimit;
    int         Mode;
}KSHCAMERACAPTURE, *PKSHCAMERACAPTURE;
#endif

/*
typedef enum
{   
    PHOTO_SIZE_176X220 = 0,
    PHOTO_SIZE_220X176,
    PHOTO_SIZE_240X320,
    PHOTO_SIZE_240X400,
    PHOTO_SIZE_400X240,
    PHOTO_SIZE_320X480,
    PHOTO_SIZE_480X320,
    PHOTO_SIZE_480X640,
    PHOTO_SIZE_320X240,
    PHOTO_SIZE_640X480,
    PHOTO_SIZE_1280X960,
    PHOTO_SIZE_1600X1200,
    PHOTO_SIZE_2048X1536,
    PHOTO_SIZE_2592X1944,
    PHOTO_SIZE_3264X2448,
    PHOTO_SIZE_MAX
}MMIDC_PHOTO_SIZE_E;
*/

/*****************************************************************************/
//  Description : wre_StoreLastPhotePath
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

static PKSHCAMERACAPTURE gpshcc=0;
void wre_StoreLastPhotePath(wchar* path,wchar* name){
    wchar dev[] = { '\\', 0};
    //SCI_TRACE_LOW:"enter wre_StoreLastPhotePath  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1250_112_2_18_3_10_32_15,(uint8*)"");

    if(gpshcc)
    {
        memset(gpshcc->szFile,0,CM_MAX_PATH*sizeof(wchar));
        MMIAPICOM_Wstrncpy(gpshcc->szFile,path,CM_MAX_PATH-1);
        MMIAPICOM_Wstrncat(gpshcc->szFile,dev,CM_MAX_PATH-1);
        MMIAPICOM_Wstrncat(gpshcc->szFile,name,CM_MAX_PATH-1);
    }
}


/*****************************************************************************/
//  Description : TracePhotoPath
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void TracePhotoPath(){
    char gbbuff[CM_MAX_PATH-1]={0};
    //SCI_TRACE_LOW:"enter TracePhotoPath  "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1269_112_2_18_3_10_32_16,(uint8*)"");
    if(gpshcc)
    {
        MMIAPICOM_WstrToStr(gpshcc->szFile,(uint8 *)gbbuff);
        //SCI_TRACE_LOW:"leave TracePhotoPath %s "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1273_112_2_18_3_10_32_17,(uint8*)"s",gbbuff);
    }
}


/*****************************************************************************/
//  Description : K_SHCameraCapture
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
typedef enum {
        CAMERACAPTURE_STILLQUALITY_DEFAULT = 0,
        CAMERACAPTURE_STILLQUALITY_LOW,
        CAMERACAPTURE_STILLQUALITY_NORMAL,
        CAMERACAPTURE_STILLQUALITY_HIGH
} CAMERACAPTURE_STILLQUALITY;

LOCAL BOOLEAN MMIWREDC_OpenPhotoWin(MMIDCSRV_MEM_CFG_E MEM_MOD)
{
    BOOLEAN  ret = FALSE;
    MMIDCSRV_WORK_PARAM_T work_param = {0};
    
    work_param.work_mode = MMIDCSRV_MODE_DC;
    
    if(MEM_MOD == MMIDCSRV_MEM_DC_MINI)
    {
        work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_MINI;
    }
    else
    {
        work_param.dc.mem_cfg = MMIDCSRV_MEM_DC_NORMAL;
    }
    work_param.dc.disp_mode = DCAMERA_DISP_MODE_MAX;
    
    work_param.dv.mem_cfg = MMIDCSRV_MEM_DV_NORMAL;
    work_param.dv.max_video_size = WORK_VIDEO_SIZE_MAX;
    work_param.dv.disp_mode = DRECORDER_DISP_MODE_MAX;
    
    ret = MMIAPIDCSRV_OpenMainWin(&work_param);
    
    SCI_TRACE_LOW("[MMIDC] MMIAPIDC_OpenPhotoWin ret=%d, mem_cfg = %d",ret, work_param.dc.mem_cfg);
    
    return ret;
}

LOCAL int32 WREDCWorkCallBack(MMIDC_WORK_ERROR_E error_ret, MMIDC_WORK_DATA_INFO_T *data_info_ptr)
{
    int32 ret = 0; 

    if (PNULL != data_info_ptr) 
    {
        if (MMIDC_WORK_SUCCESS == error_ret) 
        { 
            if((data_info_ptr->file_name_len >0)&&(PNULL!=data_info_ptr->file_name))
            {
                ret =1;
            } 
        }
    }

    SCI_TRACE_LOW("[WRE]: WREDCWorkCallBack ret = %d", ret);
    return ret; 
}

LOCAL void WREOpenDC(void) 
{
    MMIDC_WORK_PARAM_T work_param = {0}; 
    MMIDC_WORK_ERROR_E error_ret = MMIDC_WORK_ERROR; 

    work_param.work_mode = MMIDC_WORK_DC; 
    //work_param.param.dc.max_photo_size = WORK_PHOTO_SIZE_640_480; 
    work_param.work_callback = WREDCWorkCallBack;

    error_ret = MMIAPIDC_StartWork(&work_param); 
    SCI_TRACE_LOW("[WRE]: WREOpenDC error_ret = %d", error_ret);

}


void K_SHCameraCapture(PKSHCAMERACAPTURE pshcc)
{

    if(NULL == pshcc)
    {
        gpshcc=NULL;
        return;
    }
    gpshcc = pshcc;

    if(pshcc->Mem == MMIDCSRV_MEM_DC_MINI)
    {
        WREOpenDC();
    }
    else
    {
        MMIAPIDC_OpenPhotoWin();
    }
    
    switch (pshcc->StillQuality)
    {
        case CAMERACAPTURE_STILLQUALITY_LOW:
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_240X320);
            break;
        case CAMERACAPTURE_STILLQUALITY_NORMAL:
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_240X320);
            break;
        case CAMERACAPTURE_STILLQUALITY_HIGH:
            MMIAPIDC_SetPhotoSize(PHOTO_SIZE_2592X1944);
            break;
        default:
            break;
    }
    
    TracePhotoPath();
}

   
static int g_wre_camera_scan_mode=0;
static CAMERA_PREVIEW_CALLBACK_K g_wre_camera_callback=0;
int32 wre_in_camera_scan_mode()
{
    return g_wre_camera_scan_mode;
}
void wre_camera_exit_callback()
{
    PREVIEWEVENT_K cpp;
    cpp.data_buf=0;
    cpp.status=0;
    g_wre_camera_callback(&cpp);
}
void wre_camera_snap_callback(BYTE* b){
    PREVIEWEVENT_K cpp;
    cpp.data_buf=b;
    cpp.status=1;
    g_wre_camera_callback(&cpp);
}
void K_StartCameraPreview(CAMERA_PREVIEW_CALLBACK_K fp)
{
    g_wre_camera_callback=fp;
    g_wre_camera_scan_mode =1;
        {
            extern PUBLIC BOOLEAN MMIAPIDC_OpenPhotoWin(void);
               MMIAPIDC_OpenPhotoWin();
        }
}
void K_EndCameraPreview(void)
{
    g_wre_camera_scan_mode =0;
    g_wre_camera_callback = 0;

    MMIAPIDC_Exit();

}

void Adapt_EntryAudioPlayer(const wchar *lpFullFileName, int filename_len)
{
#ifdef MUSIC_PLAYER_SUPPORT
    MMIFILE_FILE_INFO_T  file_info = {0};
    MMIAPIFMM_GetFileInfoFormFullPath(lpFullFileName,(const uint32 )filename_len,&file_info);
    MMIAPIMP3_PlayMyDocumentMusic(&file_info);
#elif defined(MMI_AUDIO_PLAYER_SUPPORT)
    MMIAPIMP3_PlayFile(lpFullFileName,(const uint32)filename_len);
#endif
}

void Adapt_EntrySNSView(int app_id, int wreappid, int commmond_type, int commond_result)
{
	//SCI_TRACE_LOW:"Adapt_EntrySNSView"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1378_112_2_18_3_10_32_18,(uint8*)"");

#ifdef SNS_SUPPORT
    MMISNS_SendEvtToApp(app_id, wreappid, commmond_type, commond_result);
#endif
	//Adapt_EntryEBookView(lpFullFileName);
}


void Adapt_EntryEBookView(const uint16 *lpFullFileName, int filename_len)
{
    uint32                      file_size = 0;                                                      // 文件大小       
    uint16                      filelen = 0;                                               // 文件名长度，字符数
    uint16                      fullfilename_len = 0;                                           // 文件名长度，字符数
    MMIFILE_DEVICE_E            device = MMI_DEVICE_UDISK;
    uint16                      file_name[255+1]= {0};
    BOOLEAN                     is_empty = FALSE;

    if(PNULL == lpFullFileName)
    {
        is_empty = TRUE;
        file_size = 0;
    }
    else
    {
        fullfilename_len = MMIAPICOM_Wstrlen(lpFullFileName);//(uint16)FmmCombineFullFileName(list_data_ptr,PNULL,index,s_full_file_name,MMIFMM_FULL_FILENAME_LEN);     
        MMIAPIFMM_GetFileInfo(lpFullFileName, 
            fullfilename_len,
            &file_size,
            PNULL,
            PNULL);
    }

   
    if (0 == file_size || is_empty) 
    {
        //MMIPUB_OpenAlertWarningWinEx(SPRD_WRE_APPLET_ID, TXT_EMPTY_FILE);
        return ;
    }   
    
    device = MMIAPIFMM_GetDeviceTypeByPath(lpFullFileName, 1);
    MMIAPICOM_GetFileName(lpFullFileName, fullfilename_len, file_name, &filelen);
#ifdef EBOOK_SUPPORT
    MMIAPIEBOOK_ManagerWinMsg_4Fmm((uint8)device, file_name, filelen, file_size, lpFullFileName, fullfilename_len);  
#endif
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAnimParam(
                           BOOLEAN          is_update,  //是否立即刷新
                           wchar            *lpFullPath,
                           MMI_CTRL_ID_T    ctrl_id
                           )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_CALL_BACK_T     call_back = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    //set anim min period
    GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD,ctrl_id);

    //set anim param
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id    = ctrl_id;

    //get name
    anim_info.full_path_wstr_ptr = lpFullPath;
    anim_info.full_path_wstr_len = MMIAPICOM_Wstrlen(lpFullPath);

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update   = is_update;
    display_info.bg.bg_type  = GUI_BG_COLOR;
    display_info.bg.color    = MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam(&control_info,PNULL,&anim_info,&display_info);

    if ((GUIANIM_RESULT_SUCC == anim_result) ||
        (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[WRE]Adapt_EntryPhotoView: SetAnimParam Fail! anim_result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1467_112_2_18_3_10_32_19,(uint8*)"d", anim_result);
    }
    return result;
}

LOCAL MMI_RESULT_E HandleWREPicPreviewWinMsg(
                                             MMI_WIN_ID_T       win_id,
                                             MMI_MESSAGE_ID_E   msg_id,
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T           ctrl_id = MMIWRE_PIC_PREVIEW_CTRL_ID;
    GUI_BOTH_RECT_T         both_rect = MMITHEME_GetFullScreenBothRect();    
    
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            wchar *lpFullFileName = (wchar *)MMK_GetWinAddDataPtr(win_id);
            GUIAPICTRL_SetBothRect(ctrl_id,&both_rect);

            //set anim param
            SetAnimParam(FALSE, lpFullFileName, ctrl_id);

            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
    case MSG_APP_CANCEL:
#if 0 //def MMIWRE_MEM_CONFLICT_SOLUTION
        WreToMocorApp_Realse();
#endif
        MMK_CloseWin(win_id);
        MMK_CloseApplet(SPRD_WRE_APPLET_ID);
        break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result); 
}

WINDOW_TABLE( MMIWRE_PIC_PREVIEW_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandleWREPicPreviewWinMsg),
    WIN_ID( MMIWRE_PICTURE_PREVIEW_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
    WIN_STYLE(WS_HAS_ANIM_BG|WS_DISABLE_COMMON_BG),
    CREATE_ANIM_CTRL(MMIWRE_PIC_PREVIEW_CTRL_ID,MMIWRE_PICTURE_PREVIEW_WIN_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,TXT_NULL),
    END_WIN
};

void Adapt_EntryPhotoView(const wchar *lpFullFileName, int filename_len)
{
    //SCI_TRACE_LOW:"[WRE]Adapt_EntryPhotoView: lpFullFileName = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1521_112_2_18_3_10_32_20,(uint8*)"s", lpFullFileName);
#if 0 //def MMIWRE_MEM_CONFLICT_SOLUTION
    if(wretomocorapp.AppType == WRETOMORCOR_NONE)
    {
        uint16 len = 0;

        len = MMIAPICOM_Wstrlen(lpFullFileName);
        wretomocorapp.AppType = WRETOMORCOR_PHOTOVIEWER;
        WreToMocorApp_Realse();
        wretomocorapp.lpParam = (unsigned short *)SCI_ALLOCA(sizeof(wchar) * (len + 1));
        MMIAPICOM_Wstrcpy((wchar *) wretomocorapp.lpParam, lpFullFileName);
        MMIWRE_CloseWin();
        SCI_TRACE_LOW("Adapt_EntryPhotoView, malloc memory");
        return;
    }
#endif
    MMK_CreateWinByApplet(SPRD_WRE_APPLET_ID, (uint32 *)MMIWRE_PIC_PREVIEW_WIN_TAB, lpFullFileName);
}

/*****************************************************************************/
//  Description : WRE_IsFixAppInstalled
//  Global resource dependence : 
//  Author:qingjun.yu
//  Note:
/*****************************************************************************/

PUBLIC int WRE_IsFixAppInstalled(uint32  fixed_app_id)
{
#ifdef SNS_SUPPORT
    uint16 * guid_str = MMIWRE_GetAppGuid(fixed_app_id);
#else
    uint16 * guid_str = NULL;
#endif
    int  iRet = 0;

#ifndef WRE_VIRTUALDISK_SUPPORT
    BOOL bRes = FindAppPathByGUID(guid_str,wre_app_param.apppath,wre_app_param.appname,wre_app_param.iconpath);
    if(!bRes)
    {
        iRet = -2; //no record in WRE register
     }
     else
     {
#ifndef WIN32     
         if(!WRE_IsFileExist((uint32 *)wre_app_param.apppath))
         {
             iRet = -3; // app execute module not exist
        }
#endif		 
     }
#endif

     //SCI_TRACE_LOW:"[wre]WRE_IsFixAppInstalled return %d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1556_112_2_18_3_10_32_21,(uint8*)"d", iRet);

    return iRet;
}

#ifdef WRE_VIRTUALDISK_SUPPORT		

#include "disk_api.h"
#include "bsd.h"

#ifndef WIN32
const unsigned char s_wre_virtualDisk[] =
#include "mmiwre_virtualdisk.h"

#define WRE_ROM_DISK_SIZE    (sizeof(s_wre_virtualDisk)/sizeof(unsigned char))
#define WRE_RAM_DISK_SIZE    (150*1024)

#define WRE_ROM_DISK_SIZE_OFFSET    (64*1024)

#define WRE_DISK_NUM     2

typedef enum
{
	WRE_DISK_ROM,
	WRE_DISK_RAM,

	WRE_DISK_UNKNOWN
}WRE_DISK_TYPE;


typedef struct _tagWRE_VIRTUAL_DISK
{
    WRE_DISK_TYPE type;
    uint8 *ramdisk_mem_ptr;
    DISK_HANDLE_PTR lpHandDisk;
    DISK_CONFIG_T config;
    DWORD nCount;
}WRE_VIRTUAL_DISK,*PWRE_VIRTUAL_DISK;

WRE_VIRTUAL_DISK g_hWREVirtualDisk[WRE_DISK_NUM] = {0};

#endif

PUBLIC BOOLEAN WRE_CreateVirtualROMDisk()
{
#ifndef WIN32	
       if (g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk != NULL)
       {
            g_hWREVirtualDisk[WRE_DISK_ROM].nCount++;
            return TRUE;
       }
            
	//创建一个ROM 映射盘
	g_hWREVirtualDisk[WRE_DISK_ROM].ramdisk_mem_ptr = s_wre_virtualDisk;
	g_hWREVirtualDisk[WRE_DISK_ROM].type = WRE_DISK_ROM;
      g_hWREVirtualDisk[WRE_DISK_ROM].nCount = 1;
	g_hWREVirtualDisk[WRE_DISK_ROM].config.name = (char *)WRE_ROM_DISK_NAME;	// changed by yifei
	g_hWREVirtualDisk[WRE_DISK_ROM].config.type = BSD_NORFLASH;
	g_hWREVirtualDisk[WRE_DISK_ROM].config.attribute = BSD_DEVICE_READONLY;
	g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk= DISK_Create(s_wre_virtualDisk, WRE_ROM_DISK_SIZE+WRE_ROM_DISK_SIZE_OFFSET, &(g_hWREVirtualDisk[WRE_DISK_ROM].config));
	if (g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk == NULL)
	{		
		//SCI_TRACE_LOW:"[WRE]WRE_CreateVirtualROMDisk: Create ROM Disk Fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1612_112_2_18_3_10_32_22,(uint8*)"");
		return FALSE;
	}

        if (SFS_NO_ERROR == MMIAPIFMM_RegisterDevice(WRE_ROM_DISK_NAME, MMIAPICOM_Wstrlen(WRE_ROM_DISK_NAME)))
        {
	        //SCI_TRACE_LOW:"[WRE]RegisterDevice :success"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1618_112_2_18_3_10_32_23,(uint8*)"");
        }
        else
        {
	        //SCI_TRACE_LOW:"[WRE]RegisterDevice:fail, the format device"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1622_112_2_18_3_10_32_24,(uint8*)"");
	        MMIAPIFMM_FormatDevice(
		            WRE_ROM_DISK_NAME,
		            MMIAPICOM_Wstrlen(WRE_ROM_DISK_NAME),
		            MMIFILE_FORMAT_AUTO
		         );
        }  

	//SFS_RegisterDevice(STL_WRE_FS_PART, &format);
	
	//MMIAPIUDISK_SysDeviceinit();
    {
#if 0	
	if (Umem_GetExceptionState())
	{
		Umem_ExceptionHandler();
	}        
#endif
    }	
#endif		
	return TRUE;
}

PUBLIC BOOLEAN WRE_CreateVirtualRAMDisk()
{
#ifndef WIN32	
       //SCI_TRACE_LOW:"[WRE]WRE_CreateVirtualRAMDisk:"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1648_112_2_18_3_10_32_25,(uint8*)"");
	   
	if (g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk != NULL)
	{
              g_hWREVirtualDisk[WRE_DISK_RAM].nCount++;
		//SCI_TRACE_LOW:"[WRE]WRE_CreateVirtualRAMDisk: warning! lpHandDisk is already create"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1652_112_2_18_3_10_32_26,(uint8*)"");
		return TRUE ;
	}
       //创建一个RAM 映射盘
	g_hWREVirtualDisk[WRE_DISK_RAM].type = WRE_DISK_RAM;
       g_hWREVirtualDisk[WRE_DISK_RAM].nCount = 1;
	g_hWREVirtualDisk[WRE_DISK_RAM].config.name = (char *)WRE_RAM_DISK_NAME;	// changed by yifei
	g_hWREVirtualDisk[WRE_DISK_RAM].config.type = BSD_NORFLASH;
	g_hWREVirtualDisk[WRE_DISK_RAM].config.attribute = BSD_DEVICE_NORMAL;
	g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr= (uint8*)wre_malloc(WRE_RAM_DISK_SIZE);
	if (g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr == NULL)
	{
		//SCI_TRACE_LOW:"[WRE]WRE_CreateVirtualRAMDisk: Alloc memory Fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1663_112_2_18_3_10_32_27,(uint8*)"");
		return FALSE;
	}       

	memset(g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr, 0, WRE_RAM_DISK_SIZE);	// changed by yifei
	
	g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk= DISK_Create(g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr, WRE_RAM_DISK_SIZE, &(g_hWREVirtualDisk[WRE_DISK_RAM].config));
	if (g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk == NULL)
	{		
		//SCI_TRACE_LOW:"[WRE]WRE_CreateVirtualRAMDisk: DISK_Create Fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1672_112_2_18_3_10_32_28,(uint8*)"");
			  
		wre_free(g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr);
		g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr = PNULL;		
		
		return FALSE;
	}

	 if (SFS_NO_ERROR == MMIAPIFMM_RegisterDevice(WRE_RAM_DISK_NAME, MMIAPICOM_Wstrlen(WRE_RAM_DISK_NAME)))
        {
	        //SCI_TRACE_LOW:"[WRE]RegisterDevice : RAM disk success"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1682_112_2_18_3_10_32_29,(uint8*)"");
        }
        else
        {
	        //SCI_TRACE_LOW:"[WRE]RegisterDevice: RAM disk fail, the format device"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1686_112_2_18_3_10_32_30,(uint8*)"");
	        MMIAPIFMM_FormatDevice(
		            WRE_RAM_DISK_NAME,
		            MMIAPICOM_Wstrlen(WRE_RAM_DISK_NAME),
		            MMIFILE_FORMAT_AUTO
		         );
        }	
#endif
	//WRE_TestVirtualDisk();
		return TRUE;	// changed by yifei
}

PUBLIC void WRE_DestoryVirtualDisk()
{
#ifndef WIN32
    int i = 0;
    //SCI_TRACE_LOW:"[WRE]WRE_DestoryVirtualDisk: DISK_Delete!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1701_112_2_18_3_10_33_31,(uint8*)"");
    {	
	 if (g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk != NULL)
	 {
            g_hWREVirtualDisk[WRE_DISK_ROM].nCount--;
            if (g_hWREVirtualDisk[WRE_DISK_ROM].nCount == 0)
            {
                MMIAPIFMM_UnRegisterDevice(WRE_ROM_DISK_NAME, MMIAPICOM_Wstrlen(WRE_ROM_DISK_NAME));
                if (!DISK_Delete(g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk))
                {
                        //SCI_TRACE_LOW:"[WRE]WRE_DestoryVirtualDisk: Delete ROM disk Fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1708_112_2_18_3_10_33_32,(uint8*)"");
                }

                g_hWREVirtualDisk[WRE_DISK_ROM].lpHandDisk = NULL;
                g_hWREVirtualDisk[WRE_DISK_ROM].ramdisk_mem_ptr = NULL;	 	
            }
	 }
	 
        if (g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk != NULL)
        {
            g_hWREVirtualDisk[WRE_DISK_RAM].nCount--;
            if (g_hWREVirtualDisk[WRE_DISK_RAM].nCount == 0)
            {
                MMIAPIFMM_UnRegisterDevice(WRE_RAM_DISK_NAME, MMIAPICOM_Wstrlen(WRE_RAM_DISK_NAME));		
                if (!DISK_Delete(g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk))
                {
                        //SCI_TRACE_LOW:"[WRE]WRE_DestoryVirtualDisk: Delete RAM disk Fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_ADAPTOR_1720_112_2_18_3_10_33_33,(uint8*)"");
                }

                g_hWREVirtualDisk[WRE_DISK_RAM].lpHandDisk = NULL;
               wre_free(g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr);
               g_hWREVirtualDisk[WRE_DISK_RAM].ramdisk_mem_ptr = NULL;                
            }
        }        
    }
#endif
}

#endif
#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
void WreToMocorApp_Realse(void)
{
    if(wretomocorapp.lpParam != NULL)
    {
        SCI_TRACE_LOW("WreToMocorApp, free memory");
        SCI_FREE(wretomocorapp.lpParam);
        wretomocorapp.lpParam = NULL;
    }
}
#endif

#endif
