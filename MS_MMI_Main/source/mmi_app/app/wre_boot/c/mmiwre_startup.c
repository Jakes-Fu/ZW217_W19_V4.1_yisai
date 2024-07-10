#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "tasks_id.h"
#include "mmi_module.h"
#include "os_api.h"
#include "sfs.h"
#include "mmk_app.h"
#include "sci_types.h"
#include "watchdog.h"
#include "mmiwre_adaptor.h"
#include "mmiwre_other.h"
#include "mmiwre_export.h"
#include "mmi_default.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiset_func.h"
#include "mmidisplay_data.h"
#include "mmiwre_text.h"
#include "mmipub.h"
#include "arm_mmu.h"
#include "mmiwre.h"
#include "mmi_id.h"
#include "mmiwre_tapi.h"
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif

#include "mmi_applet_table.h"
//#define SIMULATOR_DEBUG

#if 0
typedef enum
{
    MMICAF_WIN_ID_START = (MMI_MODULE_WRE << 16),
    MMIMWIN_WIN_ID,
    MMICAF_MAX_WIN_ID
}MMICAF_WINDOW_ID_E;
#endif // delete by liang.chen, 2011-09-02

int gWR_MWIN_WIN_ID = MMIMWIN_WIN_ID;
int hot_reset_tag=0;
//MMI_HANDLE_T gWREWinHanlde =0;/*lint !e552*/

LOCAL MMI_RESULT_E HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
extern int GetAdaptorCommPara(PWRE_COMM *pWre_Comm,PWRE_APP_PARAM_T pAppParam);
extern void WreAddDll(char* ap_name_ptr,uint32 uID,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);

static void *hModule = NULL;

extern  PUBLIC void * GetDownLoadParam(void);
extern int g_wre_kernel_norflash_startup;
#ifdef MOTION_SENSOR_TYPE
uint32 g_old_sensor_open_flag = 0;
BOOLEAN g_wre_sensor_flag = FALSE;


void WRE_CloseSensorAutorotation(uint32 *old_sensor_open_flag_ptr);
void WRE_RestoreSensorAutorotation(uint32 old_sensor_open_flag);
//static void WRE_CloseSensorAutorotation(uint32 *old_sensor_open_flag_ptr);
//static void WRE_RestoreSensorAutorotation(uint32 old_sensor_open_flag);
#endif

#ifdef BLUETOOTH_SUPPORT
BOOLEAN g_wre_bt_flag = FALSE;
BOOLEAN WRE_GetOldBTState();
void WRE_SetOldBTState(BOOLEAN bBTFlag);
void WRE_RestoreBTState();
#endif

#ifndef WIN32
/*
extern void MMU_InvalideDCACHE(void);
extern void MMU_PHY_InvalideICACHEALL(void);
*/
/* 使用新的cache函数处理 */
// extern void MMU_IcacheSyncAll (void);    // changed by yifei
// extern void MMU_DcacheSyncAll (void);    // changed by yifei
#endif

LOCAL WRE_KERNEL_STARTUP_FUNC s_pWre_KernelProc = NULL;

static int isFileExist(wchar* file){
    SFS_HANDLE  fh = NULL;
    wchar wpath[256]={0};

    //WRE_Get_SYS_Dir(wpath, 256);
    MMIAPICOM_Wstrcpy(wpath, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    MMIAPICOM_Wstrcat(wpath,L"\\");
    MMIAPICOM_Wstrcat(wpath,file);
    
    fh = SFS_CreateFile(wpath, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */  

    if (NULL==fh)
    {
        return  0;
    }
    SFS_CloseFile(fh);
    return 1;
}

extern BYTE   g_strDebugParam[WRE_DEBUG_TOTAL];


/*
char* MMIWRE_Get_Part2_url()
{
	
	static char url[512]={0};
	{
		int ret = -1;
		
        WRE_APP_PARAM_T  param = {0};
        param.argc = (int)0xeaebece2;
        param.cmdline = (uint8 *)url;
		SCI_TRACE_LOW("enter MMIWRE_Get_Part2_ulr");
        ret = wre_startup(&param);
		SCI_TRACE_LOW("wre_startup [%x]",ret);
		return url;
		//return WRE_INIT_NO_ROOM==ret
	}
	
	
}*/


int WRE_unpack_part3();

int WRE_unpack_part3()
{
	int ret = 0;
    wchar  syspath[100] = {0};
    wchar  destfile[100] ={0};
    wchar  sourcefile[100] ={0};
	
	MMIAPICOM_Wstrcpy(syspath, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
    MMIAPICOM_Wstrcpy(destfile, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));	
    MMIAPICOM_Wstrcat(destfile,L"\\wrepart3.wpk.szip");	
    {
        MMIAPICOM_Wstrcpy(sourcefile, (const wchar*)WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));
        MMIAPICOM_Wstrcat(sourcefile, L"\\");
        MMIAPICOM_Wstrcat(sourcefile, L"wrepart3.wpk");
    }
    ret = unzip(sourcefile, destfile);
	if(ret)
    {
		SCI_TRACE_LOW("unzip part3 false");
		SFS_DeleteFile(sourcefile,NULL);
		SFS_DeleteFile(destfile,NULL);
        return 0;
    }
	ret = unpack(destfile,syspath);
    if(ret)
    {
		SCI_TRACE_LOW("unpack part3 false");
        return 0;
    }
	SCI_TRACE_LOW("unpack part3 OK");
	SFS_DeleteFile(sourcefile,NULL);
	SFS_DeleteFile(destfile,NULL);
    return	1;
}	// changed by yifei

int MiniInstaller_Is_WRE_OK()
{
	

#ifdef WIN32
            return 1;
#else


	{
			int ret = -1;
			SCI_TRACE_LOW("enter MiniInstaller_Is_WRE_OK ");
			SCI_TRACE_LOW(" MiniInstaller_Is_WRE_OK WRE_unpack_part3");
			ret = WRE_unpack_part3();
			if (!ret)
			{
				SCI_TRACE_LOW("unpack part3 false or part3 not exist");
			}
		}
#if 0
		{
			int ret = -1;
					char buff[32]={0} ;
					WRE_APP_PARAM_T  param = {0};
					param.argc = (int)0xeaebeced;
					param.cmdline = (uint8 *)buff;
					SCI_TRACE_LOW(" MiniInstaller_Is_WRE_OK check kernle version");
		
					ret = wre_startup(&param);
					
					if( WRE_INIT_NO_ROOM==ret){
		
						SCI_TRACE_LOW("wre ver:%s ",buff);
						if(strcmp(buff,"4.0.1")==0){
							SCI_TRACE_LOW("is wre 4.0.1 ");
							SCI_TRACE_LOW("MiniInstaller_Is_WRE_OK check kernle version returned [%x]",ret);
							return 0;
						}else{
							SCI_TRACE_LOW("is not wre 4.0.1 ");
						}
					}else{
						SCI_TRACE_LOW("MiniInstaller_Is_WRE_OK check kernle version returned [%x]",ret);
						return 0;
					}
		
		}
#endif

{
		int ret = -1;
		char buff[512]={0} ;
        WRE_APP_PARAM_T  param = {0};
        param.argc = (int)0xeaebece1;
        param.cmdline = (uint8 *)buff;
		SCI_TRACE_LOW(" MiniInstaller_Is_WRE_OK check kernle loadable");
        ret = wre_startup(&param);
		SCI_TRACE_LOW("MiniInstaller_Is_WRE_OK check kernle loadable returned [%x]",ret);
		return 0x51==ret?1:0;
}

#endif
}

BOOL WRE_IsOpenLockWin(){

	return MMK_IsOpenWin(MMIKL_CLOCK_DISP_WIN_ID);
}
 int CreateMwinDeskWin(void)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T        win_handle = 0;
    int locked = FALSE;
	SCI_TRACE_LOW("focus CreateMwinDeskWin");
	
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_WRE);
#endif

#ifdef MOTION_SENSOR_TYPE
	g_wre_sensor_flag = TRUE;

SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_109_112_2_18_3_11_0_194,(uint8*)"d",g_old_sensor_open_flag);
	if (g_old_sensor_open_flag == 0)
	{
		WRE_CloseSensorAutorotation(&g_old_sensor_open_flag);
	}
#endif

#ifdef BLUETOOTH_SUPPORT
        if (MMIAPIBT_IsBtOn())
        {
            SCI_TRACE_LOW("[WRE]MMIAPIBT_IsBtOn return TRUE");
            WRE_SetOldBTState(TRUE);
            MMIAPIBT_OpenBlueTooth(FALSE);
        }
        else
        {
            SCI_TRACE_LOW("[WRE]MMIAPIBT_IsBtOn return FALSE");
            WRE_SetOldBTState(FALSE);
        }
        
#endif

    win_create.applet_handle = SPRD_WRE_APPLET_ID;
    win_create.win_id = MMIMWIN_WIN_ID;
    win_create.func = HandleMsg;
    win_create.win_priority = WIN_ONE_LEVEL;//WIN_LOWEST_LEVEL;
    win_create.window_style = WS_DISABLE_RETURN_WIN|WS_NO_DEFAULT_STYLE; //heng.xiao modify for statusbar bug,hide statusbar


    /* create the MOCOR windows */
#ifdef MMI_KEY_LOCK_SUPPORT
	if (WRE_IsOpenLockWin())
	{
		extern BOOLEAN MMIAPIKL_UnlockPhone(void);
		SCI_TRACE_LOW("focus CreateMwinDeskWin locked ,so unlock");
		MMIAPIKL_UnlockPhone();	
		locked = TRUE;
	}
#endif
    win_handle = MMK_CreateWindow(&win_create);
#ifdef MMI_KEY_LOCK_SUPPORT
	if (locked)
	{
		extern BOOLEAN MMIAPIKL_LockPhone(void);
		SCI_TRACE_LOW("focus CreateMwinDeskWin relock");
		MMIAPIKL_LockPhone();

	}
#endif
    if(PNULL==win_handle)
    {
        return 1;
    }

    return 0;
}
 
/*LOCAL */void MMIWRE_WndDestroy(void)
{
#ifdef UI_P3D_SUPPORT
    MMI_Enable3DEffect(MMI_3D_EFFECT_WRE);
#endif
#ifdef MOTION_SENSOR_TYPE
//SCI_TRACE_LOW:"[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_138_112_2_18_3_11_0_195,(uint8*)"d",g_old_sensor_open_flag);
	if (g_old_sensor_open_flag != 0)
	{
		WRE_RestoreSensorAutorotation(g_old_sensor_open_flag);  
		g_old_sensor_open_flag=0;
		g_wre_sensor_flag = FALSE;
	} 
#endif
#ifdef BLUETOOTH_SUPPORT
        WRE_RestoreBTState();
#endif

#ifdef SNS_PULLING_SUPPORT
    if (STATUS_WRE_MINI != WRE_GetRuningStatus())
    {
        WREPDP_ResetPdpProc();
    }
#else
    WREPDP_ResetPdpProc();
#endif //SNS_PULLING_SUPPORT
}
PUBLIC void MMIWRE_CloseWin(void)
{
    MMI_WIN_ID_T win_id = MMIMWIN_WIN_ID;

    /* direct exit the wre, same as the home key. */
    /*
    WreMwinWin_HandleMsg(win_id, MSG_LOSE_FOCUS, 0);
    MMK_CloseWin(win_id);
    
    MMIWRE_WndDestroy();  
    */
    MMK_PostMsg(win_id, MSG_KEYUP_RED, PNULL,0);
}

PUBLIC void MMIWRE_CloseWindow(void)
{
    MMI_WIN_ID_T win_id = MMIMWIN_WIN_ID;

    SCI_TRACE_LOW("[WRE] MMIWRE_CloseWindow ");
    WreMwinWin_HandleMsg(win_id, MSG_LOSE_FOCUS, 0);
    MMK_CloseWin(win_id);
    
    MMIWRE_WndDestroy();  
}
#if 0
LOCAL MMI_RESULT_E HandleWreQueryWin(
                                    MMI_WIN_ID_T win_id, 
                                    MMI_MESSAGE_ID_E msg_id, 
                                    DPARAM param
                                   )
{
//     MMI_WIN_ID_T                query_win_id = MMISCH_DELETE_ANIM_WIN_ID;
//     uint32                      time_out = 0;
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    switch(msg_id)
    {
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            MMIWRE_MaxmizeAPPWithName(L"");//
            MMK_CloseWin(win_id);
            break;
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            WRE_EntryApp();
            MMK_CloseWin(win_id);
            break;
        default:
            result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
            break;
    }
     return result;
}
#endif
typedef  void (*MMIWRE_HANDLER)(void* app_handle);

PUBLIC void MMIWRE_MaxmizeAPP(void* app_handle)
{
    MMI_WIN_ID_T win_id = MMIMWIN_WIN_ID;
        //SCI_TRACE_LOW:"WREMM MMIWRE_MaxmizeAPP 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_196_112_2_18_3_11_0_196,(uint8*)"");

		
		if(MMK_IsOpenWin(MMIMWIN_WIN_ID)){
			SCI_TRACE_LOW("FG APP RUNNING");
			return;
		}

        CreateMwinDeskWin();
                //SCI_TRACE_LOW:"WREMM MMIWRE_MaxmizeAPP 2"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_199_112_2_18_3_11_0_197,(uint8*)"");
        WreMwinWin_HandleMsg(win_id, MSG_GET_FOCUS, 0);
                //SCI_TRACE_LOW:"WREMM MMIWRE_MaxmizeAPP 3"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_201_112_2_18_3_11_0_198,(uint8*)"");
        WRE_ShowApp(app_handle,WRE_APP_MAXMIZE);
                //SCI_TRACE_LOW:"WREMM MMIWRE_MaxmizeAPP 4"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_203_112_2_18_3_11_0_199,(uint8*)"");
    
    
}
PUBLIC void MMIWRE_CloseAPP(void* app_handle)
{
        //SCI_TRACE_LOW:"WREMM MMIWRE_CloseAPP 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_209_112_2_18_3_11_0_200,(uint8*)"");
        WRE_ShowApp(app_handle,WRE_APP_CLOSE);    
        //SCI_TRACE_LOW:"WREMM MMIWRE_CloseAPP 2"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_211_112_2_18_3_11_0_201,(uint8*)"");
}

PUBLIC BOOLEAN MMIWRE_ScheduleAPP(uint16 * appname,MMIWRE_HANDLER fp)
{
    void* app_handle = NULL;
    int    count = WRE_GetAppCount();
    int    i;
    WREAPP_INFO_T   wreApp;
    BOOLEAN   bRet;

    //SCI_TRACE_LOW:"MMIWRE_MaxmizeAPPWithName   count [%d]\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_222_112_2_18_3_11_0_202,(uint8*)"d",count);

    
     
    for(i=0;i<count;i++){
        
        if(STATUS_WRE_MINI != WRE_GetRuningStatus()){
            // SCI_ASSERT(0);        //fix 
            if(1 == count){
                //                MMIPUB_OpenAlertWinByTextIdEx(SPRD_WRE_APPLET_ID, PNULL, TXT_SMS_BUSY, TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                return TRUE;
            }
        }
        
        bRet = WRE_GetAppInfo(i,&wreApp);
        
        if(bRet)
        {
            if( 0 == MMIAPICOM_Wstrcmp(wreApp.appname,appname))
            {
                app_handle = wreApp.app_instance;
                fp(app_handle);
                
                //                MMIWRE_MaxmizeAPP(app_handle);
                
                return TRUE;
            }
        }
        /*
        else
        {
            //SCI_ASSERT(0);//RiancyZhang
            
        }
        */
    }
    
    
    
    return FALSE;
}


PUBLIC BOOLEAN MMIWRE_IsMinimizedAPPRunning()
{
    void* app_handle = NULL;
    int    count = WRE_GetAppCount();
    int    i;
    WREAPP_INFO_T   wreApp;
    BOOLEAN   bRet;

     
    for(i=0;i<count;i++){
        
        
        bRet = WRE_GetAppInfo(i,(PWREAPP_INFO_T)0x11);  // changed by yifei
        
        if(bRet)
        {
           
                return TRUE;
            
        }
     
    }
    
    
    
    return FALSE;
}


PUBLIC BOOLEAN MMIWRE_CloseAPPWithName(uint16 * appname)
{

    return     MMIWRE_ScheduleAPP(appname,MMIWRE_CloseAPP);
    
}
    
PUBLIC BOOLEAN MMIWRE_MaxmizeAPPWithName(uint16 * appname)
{
   return     MMIWRE_ScheduleAPP(appname,MMIWRE_MaxmizeAPP);
}

#if 0

PUBLIC BOOLEAN MMIWRE_MaxmizeAPPWithName(uint16 * appname)
{
    void* app_handle = NULL;
    int    count = WRE_GetAppCount();
    int    i;
    WREAPP_INFO_T   wreApp;
    BOOLEAN   bRet;

    //SCI_TRACE_LOW:"MMIWRE_MaxmizeAPPWithName   count [%d]\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_286_112_2_18_3_11_0_203,(uint8*)"d",count);

    
     
    for(i=0;i<count;i++){
        
        if(STATUS_WRE_MINI != WRE_GetRuningStatus()){
           // SCI_ASSERT(0);        //fix 
           if(1 == count){
                MMIPUB_OpenAlertWinByTextIdEx(SPRD_WRE_APPLET_ID, PNULL, TXT_SMS_BUSY, TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                return TRUE;
           }
        }

        
        bRet = WRE_GetAppInfo(i,&wreApp);

        if(bRet){
            if( 0 == MMIAPICOM_Wstrcmp(wreApp.appname,appname)){
                app_handle = wreApp.app_instance;
                MMIWRE_MaxmizeAPP(app_handle);
                return TRUE;
            }
            
        }else{
            SCI_ASSERT(0);
        }
    }    
    
    

    return FALSE;
}

#endif

char* wre_i_GetBOOTVersion(void)
{
    static VERSION_T v;
    WRE_GetBOOTVersion(&v);
    return v.version;
}

//************************************************************************************************
#ifndef WIN32
	#define WRE_ASSERT_TRACING		//david li, 2011-12-08, this macro is used to tracing WRE app stack information when system goes to Asserting

	#ifdef WRE_ASSERT_TRACING
	    uint32      wre_kernel_filesize = 0;	//david li, 2011-12-08, this variable is used to memorize kernel file size
	#endif    
#endif
//************************************************************************************************



typedef int U32_T ;
typedef unsigned char U8_T;
typedef  char CHAR;

typedef struct XHQLMY{
    CHAR            ___a[4];       
    uint32          a___b;        
    uint32          ___abc;          
    uint32          abc___;                 
    uint32          a___bc;               
    uint32          ab___c;             
    uint32          ___abc___;            
    uint32          ___abc___d;        
    uint32          ___a___b___c___;         
    uint32          ___b___b___c___;          
    CHAR            ___a______[16];  
    CHAR            ___b___b___b___[16];         
    CHAR            ___c___b___c___c[8];                        
}ZYXWVU,*XZYZZM_T;

        
static void *LoadWreLib(uint16 *lpLibFileName)
{
#ifdef WIN32
    wchar_t module[MAX_PATH]={0};
    wchar_t path[MAX_PATH]={0};
    wchar_t *p=0;
    GetModuleFileNameW(0,module,MAX_PATH);
    p= wcsstr(module,L"\\MSDev.exe");
    *p=0;
    wcscpy(path,module);
    wcscat(path, L"\\");
    wcscat(path,WRE_SYS_FILE_NAME_KERNELL);
    return LoadLibraryW(path);
    //return LoadLibrary(lpLibFileName);
#else
    SFS_HANDLE  wre_kernel_handle = NULL;
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint32      filesize = 0;
    uint32      readsize = 0;
    void        *pKernelProc = NULL;
    char        *pFileBuf    = NULL;

//SCI_TRACE_LOW:"LoadWreLib"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_386_112_2_18_3_11_1_211,(uint8*)"");
    //SCI_TRACE_LOW:"LoadWreLib"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_387_112_2_18_3_11_1_212,(uint8*)"");
    //SCI_TRACE_LOW:"LoadWreLib"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_388_112_2_18_3_11_1_213,(uint8*)"");
    //SCI_TRACE_LOW:"LoadWreLib"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_389_112_2_18_3_11_1_214,(uint8*)"");
    //SCI_TRACE_LOW:"LoadWreLib"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_390_112_2_18_3_11_1_215,(uint8*)"");
    //SCI_TRACE_LOW:"LoadWreLib"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_391_112_2_18_3_11_1_216,(uint8*)"");
    {
       
       //testvsn();
        }
        
    /* malloc memory */
    wre_kernel_handle = SFS_CreateFile(lpLibFileName, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
    if(0 == wre_kernel_handle )
    {
        //SCI_TRACE_LOW:"wk wrekernel.wrd not exist!!!!!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_401_112_2_18_3_11_1_217,(uint8*)"");
        return 0;
    }
    result = SFS_GetFileSize(wre_kernel_handle, &filesize);

//************************************************************************************************
#ifndef WIN32
	#ifdef WRE_ASSERT_TRACING
		wre_kernel_filesize = filesize;		//david li, 2011-12-08, here to memorize kernel file size
	#endif
#endif
//************************************************************************************************


    //SCI_TRACE_LOW:"wk filesize:%d!!!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_415_112_2_18_3_11_1_218,(uint8*)"d",filesize);
    pKernelProc = SCI_ALLOC(filesize);
    //SCI_TRACE_LOW:"wk address:%x!!!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_417_112_2_18_3_11_1_219,(uint8*)"d",pKernelProc);
    pFileBuf = (char *)pKernelProc; 
    /* file copy */
#if 0
    while(filesize>0)
    {
        SFS_ReadFile(wre_kernel_handle, (void *)pFileBuf, 1024, &readsize, NULL);
        pFileBuf += readsize;
        filesize -= readsize;
    }
#else
	 SFS_ReadFile(wre_kernel_handle, (void *)pFileBuf, filesize, &readsize, NULL);
    
#endif
    SFS_CloseFile(wre_kernel_handle);
	if(readsize != filesize){
		SCI_TRACE_LOW("lader error 0x00!!!\n");	
		return 0;
	}
#ifndef WIN32
		{
				XZYZZM_T pF;
				unsigned char* puc=(unsigned char*)pKernelProc;
				pF = (XZYZZM_T)(pKernelProc);
			
			if( (puc[0]!='S') || (puc[1]!='K') || (puc[2]!='P') || (puc[3]!='O') ){
				  SCI_TRACE_LOW("lader error 0x1!!!\n");	
				SCI_FREE(pKernelProc);
				return 0;
			}
			
			if((pF->___abc!=5)){
								  SCI_TRACE_LOW("lader error 0x2!!!\n");	
				SCI_FREE(pKernelProc);
				return 0;
			}
			
			
				
		}
#endif


    SCI_TRACE_LOW("wk file closed!!!\n");

    return pKernelProc;
#endif
}

static BOOLEAN FreeWreLib(void *hHandle)
{
    s_pWre_KernelProc = NULL;
#ifdef WIN32
    FreeLibrary(hHandle);
#else
	if (1 == g_wre_kernel_norflash_startup){
		return TRUE;
	}else{
    SCI_FREE(hHandle);
}
    
#endif    
    return TRUE;
}
static unsigned char* YSLFWKXMYW(unsigned char* pFileData);
static int *GetWreProcAddress(void * hHandle, char *lpProcName)
{
    #ifdef WIN32
    return (int *)GetProcAddress(hHandle, lpProcName);
    #else
    return (int *)YSLFWKXMYW(hHandle);
    //return ((int)hHandle| 0x01 );
    #endif
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static void WGzGDjSLFZhyW(unsigned char *JMZLBMXY )
{
    U32_T *___A = NULL;
    U8_T *__B = NULL;
    U32_T  __C = 0;
    U32_T  ___D = 0;
    U32_T  ___E = 0;
    U32_T  ___F = 0;
    
    ___A = (U32_T *)JMZLBMXY;
    ___D = *___A++;
    __C = *(___A + ___D);
    __B = (U8_T *)(___A + ___D + 1);
    while(___D > 0)
    {
        ___E = *___A;
        ___F = *((U32_T *)(__B + ___E));
        ___F += (U32_T)__B;
        ___F += __C;
      
        *((U32_T *)(__B + ___E)) = ___F;
        ___A++;
        ___D--;
    }
    
   
}



static unsigned char* YSLFWKXMYW(unsigned char* pFileData)
{
    unsigned char* pA = NULL ;
    int *pB = 0;
    int nC;
    unsigned char* pD;
    
    unsigned char* pE;
   
    XZYZZM_T pF;
  
    pF = (XZYZZM_T)(pFileData);
   
    
    pA = pFileData + sizeof( ZYXWVU );
    
    pB = (int *)pA;
    nC = (*pB + 2) * (int)sizeof(int);
   
    WGzGDjSLFZhyW((U8_T *)(pFileData+sizeof( ZYXWVU )) );           
   
    
    pD = pFileData + sizeof( ZYXWVU ) + nC;
    pE = pFileData+sizeof(ZYXWVU) + pF->a___bc + nC; 
   
    
    return (unsigned char *)((int)pD |0x01);
}
//////////////////////////////////////////////////////////////////////////
#ifndef WIN32
#ifdef WRE_WREKERNEL_NORFLASH
#define ADSVA_SIZE (25029)
const unsigned char g_adsvahlper_bin[ADSVA_SIZE]={83,75,80,79,107,251,33,0,5,0,0,0,2,0,0,0,
239,92,0,0,4,0,0,0,100,4,0,0,196,97,0,0,
135,169,203,237,1,0,0,0,48,49,50,51,52,53,54,55,
56,57,97,98,99,100,101,97,98,99,100,101,97,98,99,100,
101,87,82,69,115,108,104,109,122,119,120,102,121,119,121,106,
3,0,0,0,124,0,0,0,104,23,0,0,60,35,0,0,
240,92,0,0,10,40,35,210,1,163,27,92,91,0,159,68,
31,4,7,10,13,16,19,22,25,28,16,72,120,68,112,71,
15,72,120,68,112,71,15,72,120,68,112,71,14,72,120,68,
112,71,14,72,120,68,112,71,13,72,120,68,112,71,13,72,
120,68,112,71,12,72,120,68,112,71,12,72,120,68,112,71,
11,73,192,70,8,96,0,32,112,71,0,0,33,41,0,0,
51,42,0,0,189,23,0,0,187,49,0,0,205,49,0,0,
151,49,0,0,225,49,0,0,7,29,0,0,209,22,0,0,
4,4,0,0,0,71,8,71,16,71,24,71,32,71,40,71,
48,71,56,71,120,71,0,0,0,32,160,227,4,0,81,227,
8,0,0,58,3,192,16,226,13,0,0,10,4,192,108,226,
2,0,92,227,1,32,192,228,1,32,192,164,1,32,192,196,
12,16,65,224,6,0,0,234,129,207,176,225,1,32,192,36,
1,32,192,36,1,32,192,68,30,255,47,225,120,71,0,0,
0,32,160,227,0,64,45,233,2,48,160,225,2,192,160,225,
2,224,160,225,32,16,81,226,12,80,160,40,12,80,160,40,
32,16,81,34,251,255,255,42,1,30,176,225,12,80,160,40,
12,0,160,72,0,64,189,232,1,17,176,225,4,32,128,36,
30,255,47,1,1,32,192,68,1,32,192,68,64,4,17,227,
1,32,192,20,30,255,47,225,120,71,0,0,0,32,160,227,
161,193,112,224,32,0,0,58,33,196,112,224,15,0,0,58,
0,4,160,225,255,36,130,227,33,194,112,224,23,0,0,58,
33,196,112,224,9,0,0,58,0,4,160,225,255,40,130,227,
33,196,112,224,0,4,160,33,255,44,130,35,33,194,112,224,
14,0,0,58,0,192,112,226,6,4,0,42,32,4,160,33,
161,195,112,224,128,19,65,32,2,32,162,224,33,195,112,224,
0,19,65,32,2,32,162,224,161,194,112,224,128,18,65,32,
2,32,162,224,33,194,112,224,0,18,65,32,2,32,162,224,
161,193,112,224,128,17,65,32,2,32,162,224,33,193,112,224,
0,17,65,32,2,32,162,224,161,192,112,224,128,16,65,32,
2,32,162,224,1,192,112,224,0,16,65,32,2,32,178,224,
229,255,255,42,2,0,160,225,30,255,47,225,56,64,45,233,
152,192,159,229,129,48,160,225,163,202,140,224,20,0,92,227,
128,52,1,226,0,224,160,227,13,0,0,170,0,0,92,227,
4,48,130,181,0,224,130,181,19,0,0,186,112,64,159,229,
52,204,160,225,12,64,1,224,0,64,148,225,12,48,193,17,
4,48,130,21,0,224,130,21,14,32,160,17,17,0,0,26,
6,0,0,234,51,0,92,227,4,0,0,202,20,192,76,226,
0,64,224,227,52,204,160,225,12,0,16,225,4,0,0,26,
3,0,130,232,3,16,160,225,14,0,160,225,56,64,189,232,
30,255,47,225,12,192,192,225,4,16,130,229,0,192,130,229,
12,32,160,225,1,48,160,225,162,14,0,235,246,255,255,234,
1,252,255,255,255,255,15,0,128,77,45,233,112,0,45,233,
128,100,193,227,144,196,150,226,0,128,160,225,2,0,160,225,
255,198,92,162,144,208,77,226,3,80,160,225,128,180,195,227,
2,160,160,225,1,112,160,225,8,0,0,202,1,0,0,26,
0,0,88,227,5,0,0,26,144,196,155,226,255,198,92,162,
2,0,0,202,10,0,0,26,0,0,80,227,8,0,0,10,
5,48,160,225,10,32,160,225,7,16,160,225,8,0,160,225,
246,13,0,235,144,208,141,226,240,1,189,232,0,76,189,232,
30,255,47,225,128,20,199,227,8,16,129,225,0,32,155,225,
9,0,0,26,0,0,81,227,2,0,0,26,1,0,160,227,
230,4,0,235,204,0,0,234,144,196,134,226,255,198,92,226,
0,0,88,3,248,255,255,10,199,0,0,234,0,0,87,227,
148,195,159,181,0,64,160,227,12,0,91,177,22,0,0,170,
192,196,155,226,208,199,92,162,2,64,160,163,18,0,0,170,
208,196,155,226,255,198,92,162,15,0,0,186,108,35,159,229,
75,42,130,224,20,0,82,227,4,0,0,218,52,48,98,226,
48,35,160,225,18,3,80,225,7,0,0,26,4,0,0,234,
0,0,80,227,20,48,98,2,59,35,160,1,18,3,91,1,
1,0,0,26,1,32,2,226,2,64,98,226,0,0,81,227,
16,0,0,26,0,0,85,227,14,0,0,170,0,0,87,227,
2,0,84,19,2,0,0,26,1,0,160,227,187,4,0,235,
117,0,0,234,1,0,84,227,2,0,0,26,1,0,160,227,
182,4,0,235,128,0,0,234,1,0,160,227,179,4,0,235,
187,31,143,226,125,0,0,234,0,0,80,227,83,0,0,26,
144,196,139,226,255,198,92,226,33,0,0,26,144,196,150,226,
255,198,92,162,61,0,0,170,208,4,134,226,255,6,64,226,
8,0,144,225,239,255,255,10,0,0,87,227,7,0,0,170,
208,196,150,226,255,198,92,162,0,0,160,179,1,0,160,163,
165,15,48,225,231,255,255,26,167,31,143,226,103,0,0,234,
208,196,150,226,255,198,92,162,5,0,160,225,4,0,0,186,
0,0,80,227,10,0,160,161,5,16,160,161,160,255,255,170,
3,0,0,234,0,0,80,227,10,0,160,177,128,20,37,178,
155,255,255,186,104,34,159,229,2,32,143,224,3,0,146,232,
151,255,255,234,208,196,139,226,255,198,92,226,27,0,0,26,
0,0,85,227,22,0,0,170,0,0,86,227,5,0,0,26,
0,0,88,227,2,0,160,19,128,4,0,27,7,48,160,225,
8,32,160,225,86,2,0,234,144,196,134,226,255,198,92,226,
136,47,143,18,12,0,146,24,144,208,141,18,7,16,160,17,
8,0,160,17,240,1,189,24,0,76,189,24,213,14,0,26,
0,0,88,227,7,48,160,1,8,32,160,1,122,15,143,2,
72,2,0,10,8,0,160,225,7,16,160,225,120,255,255,234,
64,4,85,227,7,48,160,1,8,32,160,1,7,16,160,1,
8,0,160,1,9,3,0,10,208,196,133,226,254,198,92,226,
6,0,0,26,0,0,87,227,7,16,160,161,8,0,160,161,
144,208,141,162,240,1,189,168,0,76,189,168,180,3,0,170,
128,20,199,227,116,128,141,229,0,0,88,227,73,0,0,26,
102,15,143,226,0,0,86,227,8,0,0,26,2,0,84,227,
0,0,87,19,179,255,255,10,1,0,84,227,177,255,255,26,
0,0,87,227,175,255,255,170,0,16,160,225,23,0,0,234,
144,196,134,226,255,198,92,226,42,0,0,26,0,0,87,227,
3,0,0,218,0,0,85,227,1,0,0,186,85,31,143,226,
14,0,0,234,0,0,87,227,1,0,0,218,0,0,85,227,
160,255,255,186,0,0,87,227,2,0,0,170,0,0,85,227,
2,0,84,163,244,255,255,10,0,0,87,227,5,0,0,170,
0,0,85,227,1,0,84,163,2,0,0,26,71,31,143,226,
3,0,145,232,62,255,255,234,0,0,87,227,2,0,0,170,
0,0,85,227,0,0,84,163,118,255,255,10,0,0,87,227,
0,0,85,179,1,0,0,170,2,0,84,227,137,255,255,10,
0,0,87,227,0,0,85,179,1,0,0,170,1,0,84,227,
212,255,255,10,0,0,87,227,0,0,85,179,1,0,0,170,
0,0,84,227,127,255,255,10,208,196,134,226,255,198,92,226,
12,0,0,26,0,0,87,227,2,0,0,218,148,32,143,226,
3,0,146,232,34,255,255,234,1,0,0,170,0,0,84,227,
92,255,255,10,2,0,84,227,0,0,224,19,1,0,160,3,
50,16,0,235,26,255,255,234,1,32,160,227,199,15,130,224,
140,0,141,229,4,0,144,225,82,255,255,10,192,196,155,226,
120,199,92,162,117,0,0,218,192,196,155,226,252,199,92,162,
96,0,159,229,30,0,0,218,0,0,86,225,22,0,0,202,
0,0,85,227,36,0,0,186,2,0,160,227,251,3,0,235,
32,32,143,226,12,0,146,232,233,1,0,234,0,0,240,127,
1,252,255,255,1,0,0,0,0,0,248,127,0,0,0,0,
0,0,0,0,16,87,0,0,0,0,0,0,0,0,240,63,
0,0,0,0,0,0,0,128,0,0,0,0,0,0,240,127,
0,0,0,0,0,0,240,255,255,255,239,63,208,196,150,226,
255,198,92,162,2,0,0,186,0,0,85,227,229,255,255,218,
9,0,0,234,0,0,86,225,2,0,0,170,0,0,85,227,
224,255,255,170,4,0,0,234,208,196,150,226,255,198,92,162,
6,0,0,218,0,0,85,227,218,255,255,218,2,0,160,227,
214,3,0,235,100,0,79,226,12,0,144,232,173,1,0,234,
128,32,79,226,12,0,146,232,7,16,160,225,8,0,160,225,
96,13,0,235,0,112,160,225,1,128,160,225,1,48,160,225,
0,32,160,225,70,16,0,235,0,96,160,225,234,15,143,226,
12,0,144,232,1,176,160,225,8,16,160,225,7,0,160,225,
63,16,0,235,230,47,143,226,12,0,146,232,63,13,0,235,
8,48,160,225,7,32,160,225,57,16,0,235,226,47,143,226,
12,0,146,232,57,13,0,235,11,48,160,225,6,32,160,225,
51,16,0,235,0,176,160,225,48,16,141,229,220,15,143,226,
12,0,144,232,7,0,160,225,8,16,160,225,44,16,0,235,
60,0,141,229,216,15,143,226,12,0,144,232,1,96,160,225,
8,16,160,225,7,0,160,225,37,16,0,235,0,112,160,225,
211,15,143,226,12,0,144,232,1,128,160,225,48,16,157,229,
11,0,160,225,30,16,0,235,8,48,160,225,7,32,160,225,
30,13,0,235,0,112,160,225,1,176,160,225,1,48,160,225,
0,32,160,225,60,0,157,229,6,16,160,225,155,12,0,235,
1,128,160,225,0,0,160,227,6,48,160,225,60,32,157,229,
36,13,0,235,11,48,160,225,7,32,160,225,29,1,0,234,
0,0,160,227,64,9,86,227,6,0,0,170,186,15,143,226,
12,0,144,232,116,0,157,229,5,16,0,235,116,0,141,229,
52,0,224,227,1,96,160,225,70,10,128,224,192,15,64,226,
255,0,64,226,44,0,141,229,6,6,160,225,32,6,160,225,
230,203,80,226,142,192,92,162,192,181,128,227,255,182,139,227,
0,128,160,211,8,0,0,218,164,18,159,229,1,0,80,225,
1,128,160,179,4,0,0,186,44,0,157,229,0,128,160,227,
1,0,128,226,44,0,141,229,64,185,75,226,132,2,159,229,
0,0,143,224,136,1,128,224,4,48,144,229,11,16,160,225,
136,48,141,229,0,32,144,229,116,0,157,229,132,32,141,229,
248,12,0,235,60,32,141,226,3,0,130,232,116,0,157,229,
132,48,141,226,12,0,147,232,11,16,160,225,99,12,0,235,
148,47,79,226,12,0,146,232,190,13,0,235,52,32,141,226,
3,0,130,232,1,48,160,225,60,16,141,226,0,32,160,225,
3,0,145,232,210,15,0,235,76,0,141,229,24,2,159,229,
1,112,160,225,0,96,160,227,0,0,143,224,0,0,144,229,
132,48,141,226,20,0,141,229,128,5,160,227,203,0,128,225,
8,9,128,224,128,10,128,226,24,0,141,229,0,16,160,225,
20,0,157,229,12,0,147,232,214,12,0,235,11,48,160,225,
116,32,157,229,193,12,0,235,12,0,141,229,20,48,141,226,
1,176,160,225,7,16,160,225,12,0,147,232,6,0,160,225,
183,15,0,235,60,48,141,226,12,0,147,232,183,12,0,235,
3,0,141,233,12,32,157,229,7,16,160,225,6,0,160,225,
11,48,160,225,174,15,0,235,12,0,157,233,175,12,0,235,
52,48,141,226,12,0,147,232,169,15,0,235,28,32,141,226,
3,0,130,232,76,32,157,229,7,16,160,225,7,48,160,225,
2,0,160,225,162,15,0,235,1,176,160,225,1,48,160,225,
36,0,141,229,0,32,160,225,157,15,0,235,3,0,141,233,
72,1,159,229,6,16,160,227,11,48,160,225,0,0,143,224,
36,32,157,229,212,1,0,235,12,0,157,233,148,15,0,235,
1,176,160,225,80,0,141,229,6,0,160,225,7,16,160,225,
7,48,160,225,76,32,157,229,20,12,0,235,28,48,141,226,
12,0,147,232,138,15,0,235,11,48,160,225,80,32,157,229,
14,12,0,235,80,176,141,226,3,0,139,232,7,16,160,225,
6,0,160,225,7,48,160,225,6,32,160,225,128,15,0,235,
36,32,141,226,3,0,130,232,212,0,143,226,12,0,144,232,
36,0,157,229,1,12,0,235,80,48,141,226,12,0,147,232,
254,11,0,235,184,0,143,226,12,0,144,232,0,96,160,227,
6,0,160,225,1,176,160,225,134,12,0,235,36,48,141,226,
12,0,147,232,131,12,0,235,80,48,141,226,12,0,147,232,
110,12,0,235,12,192,141,226,3,0,140,232,7,16,160,225,
6,0,160,225,11,48,160,225,6,32,160,225,100,15,0,235,
6,32,160,225,60,96,141,226,3,0,134,232,28,16,141,226,
3,0,145,232,11,48,160,225,93,15,0,235,0,96,160,225,
1,176,160,225,7,48,160,225,19,0,0,234,0,0,0,0,
0,0,208,63,85,85,85,85,85,85,213,63,0,0,0,0,
0,0,224,63,0,0,0,96,71,21,247,63,68,223,93,248,
11,174,84,62,254,130,43,101,71,21,247,63,0,0,0,0,
0,0,64,67,122,182,11,0,16,82,0,0,32,82,0,0,
244,80,0,0,0,0,0,0,0,0,8,64,12,16,141,226,
3,0,145,232,76,32,157,229,65,15,0,235,11,48,160,225,
6,32,160,225,197,11,0,235,1,176,160,225,1,48,160,225,
60,16,141,226,52,0,141,229,0,32,160,225,3,0,145,232,
190,11,0,235,60,48,141,226,0,112,160,227,7,0,160,225,
12,0,147,232,1,96,160,225,70,12,0,235,11,48,160,225,
52,32,157,229,49,12,0,235,100,0,141,229,234,15,143,226,
12,0,144,232,1,176,160,225,6,16,160,225,7,0,160,225,
39,15,0,235,108,32,141,226,3,0,130,232,228,15,143,226,
12,0,144,232,7,0,160,225,6,16,160,225,32,15,0,235,
0,96,160,225,224,15,143,226,12,0,144,232,1,112,160,225,
11,16,160,225,100,0,157,229,25,15,0,235,7,48,160,225,
6,32,160,225,157,11,0,235,100,35,159,229,2,32,143,224,
136,33,130,224,12,0,146,232,152,11,0,235,0,96,160,225,
44,0,157,229,104,16,141,229,180,14,0,235,1,176,160,225,
108,16,141,226,0,112,160,225,3,0,145,232,104,48,157,229,
6,32,160,225,141,11,0,235,40,35,159,229,2,32,143,224,
136,33,130,224,4,48,146,229,128,48,141,229,0,32,146,229,
124,32,141,229,133,11,0,235,11,48,160,225,7,32,160,225,
130,11,0,235,1,128,160,225,0,0,160,227,11,48,160,225,
7,32,160,225,11,12,0,235,124,48,141,226,12,0,147,232,
8,12,0,235,108,48,141,226,12,0,147,232,5,12,0,235,
104,48,157,229,6,32,160,225,240,11,0,235,88,16,141,229,
0,96,160,225,176,15,143,226,4,112,144,228,0,176,144,229,
1,16,68,226,140,0,157,229,0,64,160,227,1,0,144,225,
171,15,143,2,4,112,144,4,0,176,144,5,10,0,160,225,
5,16,160,225,4,32,160,225,5,48,160,225,241,11,0,235,
8,48,160,225,4,32,160,225,217,14,0,235,36,32,141,226,
3,0,130,232,88,48,157,229,6,32,160,225,5,16,160,225,
10,0,160,225,210,14,0,235,36,48,141,226,12,0,147,232,
86,11,0,235,1,96,160,225,100,0,141,229,4,0,160,225,
5,16,160,225,8,48,160,225,4,32,160,225,200,14,0,235,
0,80,160,225,1,128,160,225,1,48,160,225,0,32,160,225,
100,0,157,229,6,16,160,225,72,11,0,235,192,196,145,226,
144,200,92,162,1,160,160,225,1,64,160,225,120,0,141,229,
26,0,0,186,192,20,132,226,144,24,65,226,0,0,145,225,
15,0,0,26,128,15,143,226,12,0,144,232,100,0,157,229,
6,16,160,225,57,11,0,235,36,192,141,226,3,0,140,232,
120,0,157,229,10,16,160,225,8,48,160,225,5,32,160,225,
192,11,0,235,36,48,141,226,12,0,147,232,135,14,0,235,
32,0,0,42,2,0,160,227,40,2,0,235,11,48,160,225,
7,32,160,225,110,15,143,226,3,0,144,232,23,0,0,234,
180,33,159,229,128,20,196,227,2,0,81,225,21,0,0,58,
130,26,98,224,1,16,132,224,0,0,145,225,8,0,0,26,
8,48,160,225,5,32,160,225,10,16,160,225,120,0,157,229,
168,11,0,235,6,48,160,225,100,32,157,229,78,14,0,235,
8,0,0,58,2,0,160,227,16,2,0,235,11,48,160,225,
7,32,160,225,100,1,159,229,0,0,143,224,3,0,144,232,
46,2,0,235,22,253,255,234,128,4,196,227,196,39,31,229,
208,196,144,226,254,198,92,162,0,160,160,227,64,26,130,224,
21,0,0,218,1,0,129,226,64,25,160,227,49,0,132,224,
128,16,160,225,161,26,130,224,36,33,159,229,2,32,143,224,
32,49,159,229,0,0,84,227,51,49,192,225,0,6,160,225,
32,6,160,225,64,9,128,227,20,16,97,226,48,161,160,225,
0,160,106,178,8,16,160,225,5,0,160,225,0,32,146,229,
128,11,0,235,0,80,160,225,1,128,160,225,8,48,160,225,
5,32,160,225,6,16,160,225,100,0,157,229,235,10,0,235,
0,0,160,227,68,0,141,229,204,0,143,226,12,0,144,232,
68,0,157,229,1,64,160,225,93,14,0,235,5,32,160,225,
60,80,141,226,3,0,133,232,68,0,157,229,4,16,160,225,
8,48,160,225,107,11,0,235,6,48,160,225,100,32,157,229,
86,11,0,235,152,32,143,226,12,0,146,232,80,14,0,235,
0,80,160,225,144,0,143,226,12,0,144,232,1,96,160,225,
4,16,160,225,68,0,157,229,73,14,0,235,6,48,160,225,
5,32,160,225,205,10,0,235,0,96,160,225,1,128,160,225,
1,48,160,225,0,32,160,225,25,0,0,234,0,0,0,224,
9,199,238,63,245,1,91,20,224,47,62,190,253,3,58,220,
9,199,238,63,212,78,0,0,132,78,0,0,0,0,0,0,
0,0,240,63,0,0,0,0,0,0,240,191,254,130,43,101,
71,21,151,60,0,0,0,0,0,0,240,127,0,204,144,64,
16,77,0,0,204,76,0,0,255,255,15,0,0,0,0,0,
67,46,230,63,239,57,250,254,66,46,230,63,57,108,168,12,
97,92,32,190,60,16,141,226,3,0,145,232,171,10,0,235,
60,48,141,226,12,0,147,232,0,64,160,225,1,80,160,225,
52,11,0,235,8,48,160,225,6,32,160,225,31,11,0,235,
0,96,160,225,1,128,160,225,5,16,160,225,4,0,160,225,
5,48,160,225,4,32,160,225,21,14,0,235,68,32,141,226,
3,0,130,232,0,32,160,225,232,0,159,229,1,48,160,225,
5,16,160,227,0,0,143,224,75,0,0,235,68,48,141,226,
12,0,147,232,10,14,0,235,5,48,160,225,4,32,160,225,
10,11,0,235,92,32,141,226,3,0,130,232,0,32,160,225,
1,48,160,225,5,16,160,225,4,0,160,225,0,14,0,235,
28,32,141,226,3,0,130,232,156,0,143,226,12,0,144,232,
92,16,141,226,3,0,145,232,14,11,0,235,28,48,141,226,
12,0,147,232,219,11,0,235,36,192,141,226,3,0,140,232,
5,16,160,225,4,0,160,225,8,48,160,225,6,32,160,225,
239,13,0,235,8,48,160,225,6,32,160,225,115,10,0,235,
36,48,141,226,12,0,147,232,236,10,0,235,5,48,160,225,
4,32,160,225,251,10,0,235,85,47,79,226,12,0,146,232,
230,10,0,235,10,58,129,224,67,42,160,225,0,0,82,227,
3,16,160,193,1,0,0,202,10,32,160,225,113,0,0,235,
1,48,160,225,0,32,160,225,7,0,160,225,11,16,160,225,
215,13,0,235,102,252,255,234,252,74,0,0,0,0,0,0,
0,0,0,64,8,181,0,240,199,249,0,104,1,176,8,188,
24,71,16,181,4,28,0,240,191,249,4,96,16,188,8,188,
24,71,0,0,120,71,0,0,2,0,160,227,2,16,160,227,
222,0,0,234,120,71,0,0,16,64,45,233,207,0,0,235,
4,0,128,226,16,64,189,232,30,255,47,225,240,64,45,233,
1,64,65,226,132,17,128,224,4,48,45,229,0,80,160,225,
3,0,145,232,8,0,84,227,3,112,160,225,2,96,160,225,
4,241,143,144,15,0,0,234,14,0,0,234,64,0,0,234,
56,0,0,234,48,0,0,234,40,0,0,234,32,0,0,234,
24,0,0,234,16,0,0,234,8,0,0,234,7,48,160,225,
6,32,160,225,170,13,0,235,1,64,68,226,132,33,133,224,
12,0,146,232,45,10,0,235,8,0,84,227,246,255,255,202,
7,48,160,225,6,32,160,225,161,13,0,235,1,64,68,226,
132,33,133,224,12,0,146,232,36,10,0,235,7,48,160,225,
6,32,160,225,154,13,0,235,1,64,68,226,132,33,133,224,
12,0,146,232,29,10,0,235,7,48,160,225,6,32,160,225,
147,13,0,235,1,64,68,226,132,33,133,224,12,0,146,232,
22,10,0,235,7,48,160,225,6,32,160,225,140,13,0,235,
1,64,68,226,132,33,133,224,12,0,146,232,15,10,0,235,
7,48,160,225,6,32,160,225,133,13,0,235,1,64,68,226,
132,33,133,224,12,0,146,232,8,10,0,235,7,48,160,225,
6,32,160,225,126,13,0,235,1,64,68,226,132,33,133,224,
12,0,146,232,1,10,0,235,7,48,160,225,6,32,160,225,
119,13,0,235,1,64,68,226,132,33,133,224,12,0,146,232,
250,9,0,235,7,48,160,225,6,32,160,225,112,13,0,235,
1,32,68,226,130,33,133,224,12,0,146,232,4,208,141,226,
240,64,189,232,241,9,0,234,56,64,45,233,129,48,160,225,
2,64,160,225,1,32,160,225,163,58,176,225,224,80,143,226,
14,0,0,26,128,36,194,227,0,32,146,225,37,0,0,10,
212,32,143,226,12,0,146,232,93,13,0,235,129,48,160,225,
53,192,224,227,163,58,140,224,192,204,148,226,212,207,156,210,
1,32,160,225,1,0,0,170,12,0,149,232,38,0,0,234,
112,206,67,226,255,192,92,226,4,208,141,2,48,64,189,8,
1,48,160,1,0,32,160,1,212,9,0,10,4,48,131,224,
112,206,83,226,254,192,92,162,132,224,143,226,9,0,0,202,
0,0,83,227,112,20,194,195,255,22,193,195,3,26,129,193,
8,0,0,202,54,0,115,227,13,0,0,202,192,204,84,226,
212,207,92,162,5,0,0,218,1,48,160,225,0,32,160,225,
3,0,158,232,229,0,0,235,56,64,189,232,30,255,47,225,
1,48,160,225,0,32,160,225,3,0,149,232,223,0,0,235,
220,255,255,234,112,36,194,227,255,38,194,227,54,16,131,226,
1,26,130,225,32,32,143,226,12,0,146,232,44,13,0,235,
240,255,255,234,89,243,248,194,31,110,165,1,0,0,0,0,
0,0,80,67,0,0,0,0,0,0,240,127,0,0,0,0,
0,0,144,60,56,64,45,233,0,48,96,226,3,48,128,225,
128,36,193,227,163,47,130,225,255,38,98,226,112,36,130,226,
162,47,176,225,1,80,160,225,0,64,160,225,9,0,0,26,
48,32,143,226,12,0,146,232,244,12,0,235,5,0,0,42,
1,0,160,227,149,0,0,235,32,16,143,226,3,0,145,232,
56,64,189,232,30,255,47,225,5,16,160,225,4,0,160,225,
187,13,0,235,249,255,255,234,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,248,127,56,181,7,180,131,176,10,156,
1,173,41,28,0,32,5,171,3,170,0,148,0,240,0,248,
0,40,5,208,1,169,3,201,7,176,48,188,8,188,24,71,
31,32,128,6,4,64,33,28,2,32,0,240,31,248,0,32,
0,33,3,197,1,169,3,201,238,231,0,0,120,71,0,0,
0,192,153,229,0,192,156,229,4,0,159,229,12,0,128,224,
30,255,47,225,8,4,0,0,120,71,0,0,16,64,45,233,
246,255,255,235,16,64,189,232,30,255,47,225,120,71,0,0,
16,64,45,233,109,0,0,235,0,0,80,227,16,64,189,8,
30,255,47,1,16,64,189,232,17,0,0,234,112,181,14,78,
13,28,1,28,4,28,126,68,48,28,0,240,0,248,2,28,
176,66,6,209,41,28,32,28,0,240,32,248,112,188,8,188,
24,71,6,72,120,68,130,66,3,208,41,28,32,28,254,247,
131,253,0,32,242,231,0,0,169,234,255,255,137,234,255,255,
120,71,0,0,24,0,160,227,8,16,159,229,86,52,18,239,
30,255,47,225,33,1,0,0,38,0,2,0,56,181,66,30,
13,42,36,164,38,210,23,34,35,75,66,67,123,68,213,24,
23,61,2,40,20,209,72,1,1,213,32,164,27,224,36,72,
8,64,1,208,35,164,22,224,200,0,1,213,37,164,18,224,
136,0,1,213,38,164,14,224,72,0,12,213,39,164,10,224,
8,40,1,209,12,28,6,224,9,40,4,209,1,41,2,209,
38,165,0,224,43,165,10,32,0,240,96,248,40,120,0,40,
6,208,40,120,1,53,0,240,89,248,40,120,0,40,248,209,
32,120,0,40,6,208,32,120,1,52,0,240,79,248,32,120,
0,40,248,209,10,32,0,240,73,248,56,188,8,188,1,32,
24,71,0,0,0,0,0,0,8,70,0,0,73,110,118,97,
108,105,100,32,79,112,101,114,97,116,105,111,110,0,0,0,
2,0,0,8,68,105,118,105,100,101,32,66,121,32,90,101,
114,111,0,0,79,118,101,114,102,108,111,119,0,0,0,0,
85,110,100,101,114,102,108,111,119,0,0,0,73,110,101,120,
97,99,116,32,82,101,115,117,108,116,0,0,58,32,72,101,
97,112,32,109,101,109,111,114,121,32,99,111,114,114,117,112,
116,101,100,0,85,110,107,110,111,119,110,32,115,105,103,110,
97,108,0,0,120,71,0,0,30,255,47,225,8,181,0,171,
24,112,3,32,105,70,171,223,1,176,8,188,24,71,0,0,
4,192,159,229,15,192,140,224,28,255,47,225,167,250,255,255,
4,192,159,229,15,192,140,224,28,255,47,225,81,254,255,255,
120,71,192,70,188,250,255,234,120,71,192,70,229,250,255,234,
4,192,159,229,15,192,140,224,28,255,47,225,157,253,255,255,
15,180,16,181,200,176,156,33,104,70,254,247,215,252,75,168,
71,144,71,171,127,33,39,168,74,154,0,240,85,248,4,28,
0,32,71,144,8,73,39,170,121,68,104,70,1,240,248,248,
6,72,192,70,1,104,104,70,254,247,149,252,32,28,72,176,
16,188,8,188,4,176,24,71,220,67,0,0,4,4,0,0,
128,20,193,227,128,36,3,226,2,16,129,225,30,255,47,225,
128,20,193,227,128,36,3,226,2,16,129,225,30,255,47,225,
3,181,1,152,8,73,64,0,64,13,136,66,9,209,1,152,
0,3,6,209,0,152,0,40,3,209,1,32,2,176,8,188,
24,71,0,32,250,231,0,0,255,7,0,0,3,181,1,152,
8,73,64,0,64,13,136,66,9,209,1,152,0,3,2,209,
0,152,0,40,3,208,1,32,2,176,8,188,24,71,0,32,
250,231,0,0,255,7,0,0,255,181,137,176,29,28,9,156,
10,153,9,156,103,24,1,63,9,153,0,32,192,67,1,57,
143,66,113,210,7,28,9,153,56,28,121,26,1,49,10,145,
106,224,37,40,4,208,188,66,0,216,32,112,1,52,24,226,
0,38,4,33,8,34,11,152,1,48,11,144,0,120,43,40,
17,208,6,220,32,40,16,208,35,40,16,209,32,32,6,67,
241,231,45,40,4,208,48,40,9,209,1,32,6,67,234,231,
16,32,6,67,231,231,14,67,229,231,22,67,227,231,0,32,
192,67,6,144,11,153,8,120,2,28,48,58,10,42,4,210,
11,168,1,240,227,252,6,144,15,224,42,40,13,209,1,49,
11,145,41,104,1,201,41,96,6,144,6,152,0,40,4,218,
6,152,64,66,6,144,16,32,6,67,0,32,192,67,5,144,
11,152,1,120,46,41,24,209,65,28,11,145,8,120,2,28,
48,58,10,42,4,210,11,168,1,240,192,252,5,144,7,224,
42,40,5,209,1,49,11,145,41,104,1,201,41,96,5,144,
5,152,0,40,1,218,0,32,5,144,0,33,201,67,11,154,
16,120,104,40,7,208,0,224,13,226,108,40,3,208,76,40,
1,208,119,40,3,209,1,28,1,50,11,146,10,224,73,40,
8,209,83,120,54,43,5,209,147,120,52,43,2,209,1,28,
3,50,11,146,10,35,11,152,0,120,101,40,103,208,41,220,
88,40,101,208,29,220,67,40,99,208,69,40,95,208,71,40,
93,208,83,40,104,209,108,41,1,208,119,41,101,209,40,104,
8,200,40,96,6,152,5,153,2,150,0,38,0,144,1,145,
57,28,32,28,246,67,26,28,51,28,1,240,147,255,4,28,
119,225,90,40,107,208,98,40,106,208,99,40,31,208,100,40,
74,209,112,224,111,40,100,208,16,220,102,40,55,208,103,40,
53,208,105,40,103,208,110,40,62,209,108,41,100,209,41,104,
1,201,41,96,9,153,97,26,1,96,90,225,112,40,102,208,
115,40,101,208,117,40,100,208,120,40,45,209,88,225,240,6,
10,212,32,34,3,224,188,66,0,216,34,112,1,52,6,152,
1,56,6,144,0,40,246,220,104,41,7,209,188,66,3,216,
41,104,1,201,41,96,32,112,1,52,6,224,188,66,3,216,
41,104,1,201,41,96,32,112,1,52,32,32,6,224,207,224,
52,225,11,224,188,66,0,216,32,112,1,52,6,153,1,57,
6,145,0,41,246,220,36,225,53,225,89,224,240,6,10,212,
32,34,3,224,188,66,0,216,34,112,1,52,6,152,1,56,
6,144,0,40,246,220,108,41,1,208,119,41,10,209,188,66,
3,216,41,104,1,201,41,96,32,112,1,52,9,224,81,224,
10,225,7,225,188,66,3,216,41,104,1,201,41,96,32,112,
1,52,32,32,5,224,5,225,147,224,188,66,0,216,32,112,
1,52,6,153,1,57,6,145,0,41,246,220,241,224,114,224,
0,224,249,224,104,41,17,209,40,104,8,200,40,96,6,152,
5,153,2,150,0,38,0,144,1,145,57,28,32,28,246,67,
26,28,51,28,1,240,184,254,4,28,218,224,40,104,8,200,
40,96,6,152,5,153,2,150,0,38,0,144,1,145,57,28,
32,28,246,67,26,28,51,28,1,240,228,254,4,28,200,224,
40,104,8,200,40,96,6,152,5,153,2,150,0,38,0,144,
1,145,57,28,32,28,246,67,26,28,51,28,1,240,148,254,
4,28,182,224,104,41,26,209,41,104,1,201,41,96,0,40,
2,208,67,104,0,43,3,209,0,35,217,67,140,70,1,224,
0,136,132,70,5,153,6,152,26,28,1,145,57,28,0,144,
32,28,99,70,2,150,1,240,119,254,4,28,153,224,41,104,
1,201,41,96,0,40,2,208,67,104,0,43,3,209,0,35,
217,67,140,70,2,224,0,136,64,8,132,70,5,153,6,152,
26,28,1,145,57,28,0,144,32,28,99,70,2,150,1,240,
153,254,4,28,125,224,6,152,67,28,3,209,8,32,6,144,
1,32,6,67,6,152,5,153,2,150,0,144,1,145,40,104,
57,28,4,200,40,96,16,35,32,28,1,240,113,251,4,28,
103,224,41,104,1,201,41,96,9,153,97,26,1,96,96,224,
40,104,1,28,8,49,41,96,1,104,64,104,7,145,8,144,
7,169,3,201,255,247,10,254,0,40,13,208,99,73,121,68,
3,32,6,224,188,66,2,216,10,120,1,49,34,112,1,52,
1,56,0,40,246,220,68,224,7,169,3,201,255,247,224,253,
0,40,13,218,90,73,121,68,4,32,6,224,188,66,2,216,
10,120,1,49,34,112,1,52,1,56,0,40,246,220,48,224,
7,169,3,201,255,247,204,253,0,40,13,221,81,73,121,68,
4,32,6,224,188,66,2,216,10,120,1,49,34,112,1,52,
1,56,0,40,246,220,28,224,5,152,67,28,1,209,6,32,
5,144,73,160,12,200,7,169,3,201,2,240,15,254,1,210,
2,32,6,67,6,152,5,153,3,150,1,144,11,152,2,145,
2,120,7,171,57,28,0,146,12,203,32,28,1,240,210,251,
4,28,85,224,8,35,7,224,2,35,5,224,64,32,6,67,
16,35,1,224,2,32,6,67,73,41,23,209,41,104,1,201,
41,96,132,70,57,224,37,40,4,208,188,66,1,216,37,32,
32,112,1,52,11,153,8,120,0,40,4,208,188,66,0,216,
32,112,1,52,52,224,1,57,11,145,49,224,108,41,11,209,
176,7,4,213,41,104,1,201,41,96,132,70,29,224,41,104,
1,201,41,96,132,70,24,224,104,41,11,209,176,7,4,213,
41,104,1,201,41,96,132,70,15,224,41,104,1,201,41,96,
132,70,10,224,176,7,4,213,41,104,1,201,41,96,132,70,
3,224,41,104,1,201,41,96,132,70,5,153,6,152,1,145,
57,28,0,144,32,28,2,150,98,70,1,240,177,250,4,28,
11,152,1,48,11,144,11,152,0,120,0,40,0,208,128,229,
188,66,1,216,32,112,11,224,10,153,0,41,8,208,9,153,
10,154,97,26,145,66,1,217,56,112,1,224,121,28,8,112,
9,153,13,176,96,26,240,188,8,188,24,71,98,62,0,0,
62,62,0,0,30,62,0,0,0,0,0,0,0,0,0,0,
255,181,137,176,29,28,9,156,10,153,9,156,72,0,7,25,
2,63,9,153,0,32,192,67,2,57,143,66,114,210,7,28,
9,153,56,28,121,26,73,16,1,49,10,145,106,224,37,40,
4,208,188,66,0,216,32,128,2,52,0,226,0,38,4,33,
8,34,11,152,2,48,11,144,0,136,43,40,17,208,6,220,
32,40,16,208,35,40,16,209,32,32,6,67,241,231,45,40,
4,208,48,40,9,209,1,32,6,67,234,231,16,32,6,67,
231,231,14,67,229,231,22,67,227,231,0,32,192,67,6,144,
11,152,0,136,0,240,182,250,0,40,4,208,11,168,0,240,
153,253,6,144,17,224,11,152,1,136,42,41,13,209,2,48,
11,144,41,104,1,201,41,96,6,144,6,152,0,40,4,218,
6,152,64,66,6,144,16,32,6,67,0,32,192,67,5,144,
11,152,1,136,46,41,26,209,2,48,11,144,0,136,0,240,
145,250,0,40,4,208,11,168,0,240,116,253,5,144,9,224,
11,152,1,136,42,41,5,209,2,48,11,144,41,104,1,201,
41,96,5,144,5,152,0,40,1,218,0,32,5,144,0,33,
201,67,0,224,19,226,11,154,16,136,104,40,5,208,108,40,
3,208,76,40,1,208,119,40,3,209,1,28,2,50,11,146,
10,224,73,40,8,209,83,136,54,43,5,209,147,136,52,43,
2,209,1,28,6,50,11,146,10,35,11,152,0,136,101,40,
104,208,41,220,88,40,102,208,29,220,67,40,100,208,69,40,
96,208,71,40,94,208,83,40,105,209,108,41,1,208,119,41,
102,209,40,104,8,200,40,96,6,152,5,153,2,150,0,38,
0,144,1,145,57,28,32,28,246,67,26,28,51,28,0,240,
133,253,4,28,91,225,90,40,108,208,98,40,107,208,99,40,
32,208,100,40,75,209,113,224,111,40,101,208,17,220,102,40,
56,208,103,40,54,208,105,40,104,208,110,40,63,209,108,41,
101,209,41,104,1,201,41,96,9,153,97,26,73,16,1,96,
61,225,112,40,102,208,115,40,101,208,117,40,100,208,120,40,
45,209,89,225,240,6,10,212,32,34,3,224,188,66,0,216,
34,128,2,52,6,152,1,56,6,144,0,40,246,220,104,41,
7,209,188,66,3,216,41,104,1,201,41,96,32,128,2,52,
6,224,188,66,3,216,41,104,1,201,41,96,32,128,2,52,
32,32,6,224,208,224,53,225,11,224,188,66,0,216,32,128,
2,52,6,153,1,57,6,145,0,41,246,220,7,225,54,225,
89,224,240,6,10,212,32,34,3,224,188,66,0,216,34,128,
2,52,6,152,1,56,6,144,0,40,246,220,108,41,1,208,
119,41,10,209,188,66,3,216,41,104,1,201,41,96,32,128,
2,52,9,224,81,224,11,225,8,225,188,66,3,216,41,104,
1,201,41,96,32,128,2,52,32,32,5,224,6,225,147,224,
188,66,0,216,32,128,2,52,6,153,1,57,6,145,0,41,
246,220,212,224,114,224,0,224,250,224,104,41,17,209,40,104,
8,200,40,96,6,152,5,153,2,150,0,38,0,144,1,145,
57,28,32,28,246,67,26,28,51,28,0,240,169,252,4,28,
189,224,40,104,8,200,40,96,6,152,5,153,2,150,0,38,
0,144,1,145,57,28,32,28,246,67,26,28,51,28,0,240,
213,252,4,28,171,224,40,104,8,200,40,96,6,152,5,153,
2,150,0,38,0,144,1,145,57,28,32,28,246,67,26,28,
51,28,0,240,133,252,4,28,153,224,104,41,26,209,41,104,
1,201,41,96,0,40,2,208,67,104,0,43,3,209,0,35,
217,67,140,70,1,224,0,136,132,70,5,153,6,152,26,28,
1,145,57,28,0,144,32,28,99,70,2,150,0,240,104,252,
4,28,124,224,41,104,1,201,41,96,0,40,2,208,67,104,
0,43,3,209,0,35,217,67,140,70,2,224,0,136,64,8,
132,70,5,153,6,152,26,28,1,145,57,28,0,144,32,28,
99,70,2,150,0,240,138,252,4,28,96,224,6,152,67,28,
3,209,8,32,6,144,1,32,6,67,6,152,5,153,2,150,
0,144,1,145,40,104,57,28,4,200,40,96,16,35,32,28,
0,240,48,249,4,28,74,224,41,104,1,201,41,96,9,153,
97,26,73,16,1,96,66,224,40,104,1,28,8,49,41,96,
1,104,64,104,7,145,8,144,7,169,3,201,255,247,78,251,
0,40,13,208,100,73,121,68,3,32,6,224,188,66,2,216,
10,136,2,49,34,128,2,52,1,56,0,40,246,220,38,224,
7,169,3,201,255,247,36,251,0,40,13,218,91,73,121,68,
4,32,6,224,188,66,2,216,10,136,2,49,34,128,2,52,
1,56,0,40,246,220,18,224,7,169,3,201,255,247,16,251,
0,40,13,221,82,73,121,68,4,32,6,224,188,66,2,216,
10,136,2,49,34,128,2,52,1,56,0,40,246,220,115,224,
5,152,67,28,1,209,6,32,5,144,74,160,12,200,7,169,
3,201,2,240,83,251,1,210,2,32,6,67,6,152,5,153,
3,150,1,144,11,152,2,145,2,136,7,171,57,28,0,146,
12,203,32,28,0,240,146,249,4,28,85,224,8,35,7,224,
2,35,5,224,64,32,6,67,16,35,1,224,2,32,6,67,
73,41,23,209,41,104,1,201,41,96,132,70,57,224,37,40,
4,208,188,66,1,216,37,32,32,128,2,52,11,153,8,136,
0,40,4,208,188,66,0,216,32,128,2,52,52,224,2,57,
11,145,49,224,108,41,11,209,176,7,4,213,41,104,1,201,
41,96,132,70,29,224,41,104,1,201,41,96,132,70,24,224,
104,41,11,209,176,7,4,213,41,104,1,201,41,96,132,70,
15,224,41,104,1,201,41,96,132,70,10,224,176,7,4,213,
41,104,1,201,41,96,132,70,3,224,41,104,1,201,41,96,
132,70,5,153,6,152,1,145,57,28,0,144,32,28,2,150,
98,70,0,240,111,248,4,28,11,152,2,48,11,144,11,152,
0,136,0,40,0,208,122,229,188,66,1,216,32,128,12,224,
10,153,0,41,9,208,9,153,10,154,97,26,73,16,145,66,
1,217,56,128,1,224,185,28,8,128,9,153,13,176,96,26,
240,188,8,188,64,16,24,71,122,56,0,0,90,56,0,0,
62,56,0,0,0,0,0,0,0,0,0,0,2,28,16,28,
48,56,9,40,3,216,11,28,47,51,154,66,13,221,0,32,
192,67,10,41,9,221,19,28,65,59,25,43,6,216,11,28,
54,51,154,66,2,220,16,28,55,56,112,71,19,28,97,59,
25,43,250,216,86,49,138,66,247,220,16,28,87,56,112,71,
16,181,4,28,8,28,33,104,253,247,14,255,32,96,16,188,
8,188,8,28,24,71,0,0,16,181,5,76,1,33,137,2,
192,70,32,28,253,247,174,254,32,28,16,188,8,188,24,71,
4,0,0,0,1,28,1,32,48,57,10,41,0,211,0,32,
112,71,0,0,255,181,165,176,48,159,4,28,14,28,98,72,
120,68,1,28,76,57,122,6,0,212,1,28,1,145,248,6,
1,213,127,8,127,0,40,153,2,41,2,219,40,153,36,41,
4,221,0,32,41,176,240,188,8,188,24,71,48,32,249,7,
0,212,32,32,1,4,9,12,36,145,0,33,35,145,184,7,
21,213,46,152,39,153,1,56,0,41,5,218,45,34,35,146,
73,66,39,145,46,144,10,224,121,7,3,213,43,33,35,145,
46,144,4,224,57,7,2,213,32,33,35,145,46,144,184,6,
12,213,40,153,16,41,3,209,46,152,2,56,46,144,5,224,
40,153,8,41,2,209,46,152,1,56,46,144,0,37,39,152,
0,40,16,209,48,32,0,171,24,129,1,37,14,224,39,168,
40,153,255,247,133,255,1,153,64,0,9,90,40,28,1,53,
64,0,2,170,17,82,39,152,0,40,240,209,47,152,133,66,
0,221,47,149,46,169,3,201,64,26,17,34,57,28,17,64,
32,34,0,41,8,209,3,224,180,66,0,216,34,128,2,52,
1,28,1,56,0,41,247,220,35,153,0,41,4,208,180,66,
1,216,35,153,33,128,2,52,185,6,23,213,40,153,8,41,
5,209,180,66,1,216,48,33,33,128,2,52,14,224,40,153,
16,41,11,209,180,66,1,216,48,33,33,128,161,28,177,66,
3,216,1,155,64,51,91,136,11,128,140,28,249,6,9,212,
4,224,180,66,1,216,36,153,33,128,2,52,1,28,1,56,
0,41,246,220,48,35,3,224,180,66,0,216,35,128,2,52,
47,153,79,30,47,151,169,66,246,220,6,224,180,66,3,216,
105,0,2,171,89,90,33,128,2,52,41,28,1,61,0,41,
244,220,3,224,180,66,0,216,34,128,2,52,1,28,1,56,
0,41,247,220,32,28,77,231,220,53,0,0,255,181,149,176,
31,158,33,159,5,28,237,160,1,104,64,104,18,145,19,144,
0,36,0,32,6,144,30,152,103,40,8,208,30,152,71,40,
5,208,30,152,101,40,2,208,30,152,69,40,25,209,24,152,
23,153,64,0,0,41,6,209,24,153,9,3,3,209,65,13,
1,209,0,32,2,224,223,73,64,13,64,24,2,240,22,249,
4,144,221,160,5,145,12,200,4,152,1,240,157,253,18,144,
19,145,30,152,103,40,2,208,30,152,71,40,32,209,128,32,
7,67,215,160,12,200,18,169,3,201,2,240,111,249,15,211,
18,169,3,201,4,144,73,0,73,8,5,145,32,152,2,240,
245,248,2,144,3,145,4,171,12,203,2,240,95,249,5,216,
30,152,2,56,0,4,0,12,30,144,1,224,102,32,30,144,
113,30,30,152,20,145,101,40,2,208,30,152,69,40,96,209,
18,169,3,201,16,170,255,247,163,248,13,144,14,145,194,160,
12,200,13,169,3,201,2,240,255,248,11,217,16,169,3,201,
4,144,191,160,5,145,12,200,16,152,1,240,135,250,16,144,
17,145,17,224,188,160,12,200,13,169,3,201,2,240,46,249,
10,210,16,169,3,201,4,144,185,160,5,145,12,200,16,152,
1,240,116,250,16,144,17,145,16,171,12,203,182,160,3,200,
255,247,122,248,2,144,3,145,23,171,12,203,1,240,34,253,
5,145,32,153,4,144,48,31,102,34,1,144,2,145,22,153,
0,146,3,151,4,171,12,203,40,28,255,247,87,255,30,153,
132,28,1,128,16,169,3,201,2,240,52,248,5,28,20,34,
0,45,0,218,22,34,2,33,2,146,42,28,0,145,1,145,
10,35,32,28,22,153,255,247,117,254,25,176,240,188,8,188,
24,71,30,152,102,40,114,209,255,247,86,254,15,144,248,6,
1,213,127,8,127,0,48,32,249,7,0,212,32,32,0,4,
0,12,7,144,0,32,8,144,184,7,27,213,131,160,12,200,
23,169,3,201,2,240,210,248,9,210,45,32,8,144,23,169,
3,201,23,144,73,0,73,8,24,145,20,158,10,224,120,7,
3,213,43,32,8,144,20,158,4,224,56,7,2,213,32,32,
8,144,20,158,23,169,3,201,11,170,255,247,17,248,13,144,
14,145,32,152,0,40,54,221,32,156,1,60,23,224,126,160,
12,200,13,169,3,201,2,240,235,248,13,144,14,145,13,169,
3,201,9,170,254,247,252,255,13,144,14,145,9,169,3,201,
1,240,208,255,48,48,15,154,97,0,80,82,1,60,0,44,
229,218,32,156,32,156,54,27,0,32,6,144,102,160,12,200,
13,169,3,201,2,240,72,248,1,217,1,32,6,144,32,152,
1,40,1,218,184,6,6,213,32,28,1,52,46,33,15,154,
64,0,17,82,1,62,85,160,12,200,12,153,0,224,214,224,
11,152,1,240,101,255,52,209,32,28,1,52,48,33,15,154,
64,0,17,82,1,62,51,224,11,169,3,201,9,144,10,145,
89,160,12,200,11,169,3,201,1,240,126,252,11,144,12,145,
11,169,3,201,11,170,254,247,179,255,4,144,5,145,82,160,
12,200,11,169,3,201,2,240,147,248,4,144,5,145,9,171,
12,203,1,240,147,250,9,144,10,145,9,169,3,201,1,240,
121,255,48,48,1,28,32,28,1,52,15,154,64,0,17,82,
1,62,54,160,12,200,11,169,3,201,1,240,245,255,203,216,
0,32,18,224,15,154,65,0,82,90,19,28,48,59,8,43,
5,216,15,155,1,50,90,82,0,33,6,145,4,224,57,42,
2,209,15,155,48,34,90,82,1,48,160,66,2,218,6,153,
1,41,231,208,6,152,1,40,5,209,32,28,1,52,49,33,
15,154,64,0,17,82,0,32,15,154,97,0,80,82,32,152,
48,26,17,33,59,28,11,64,32,34,0,43,6,209,1,224,
42,128,2,53,1,28,1,56,0,41,249,220,8,153,0,41,
2,208,8,153,41,128,2,53,0,43,6,209,1,224,42,128,
2,53,1,28,1,56,0,41,249,220,249,6,7,212,2,224,
7,153,41,128,2,53,1,28,1,56,0,41,248,220,15,155,
57,6,57,213,185,6,55,212,0,33,1,224,2,51,1,60,
161,66,2,218,30,136,48,46,248,208,161,66,44,218,25,136,
46,41,41,209,2,51,1,60,38,224,0,0,0,0,0,0,
0,0,0,0,1,252,255,255,97,85,189,252,78,147,10,64,
0,0,0,0,0,0,16,192,0,0,0,0,0,0,224,63,
0,0,0,0,0,0,240,63,0,0,0,0,0,0,224,191,
0,0,0,0,0,0,240,191,0,0,0,0,0,0,36,64,
97,0,89,90,41,128,2,53,33,28,1,60,0,41,247,220,
1,224,42,128,2,53,1,28,1,56,0,41,249,220,40,28,
171,230,0,0,16,181,2,28,0,36,7,224,17,104,8,136,
2,49,17,96,10,33,97,67,68,24,48,60,16,104,0,136,
255,247,8,253,0,40,241,209,32,28,16,188,8,188,24,71,
15,180,8,181,4,169,0,145,107,70,4,73,3,154,254,247,
75,255,1,176,8,188,4,176,24,71,0,0,255,255,255,127,
240,180,4,175,224,207,28,28,0,42,3,209,26,74,122,68,
6,36,13,224,99,28,8,209,0,36,0,224,1,52,180,66,
6,210,19,93,0,43,249,209,2,224,180,66,0,217,52,28,
251,6,32,39,0,43,8,219,3,224,136,66,0,216,7,128,
2,48,43,28,1,61,163,66,247,220,0,35,6,224,136,66,
2,216,22,120,1,50,6,128,2,48,1,51,163,66,246,219,
3,224,136,66,0,216,7,128,2,48,42,28,1,61,162,66,
247,220,240,188,112,71,0,0,26,48,0,0,240,180,4,175,
224,207,28,28,0,42,3,209,26,74,122,68,6,36,14,224,
99,28,9,209,0,36,0,224,1,52,180,66,7,210,99,0,
211,90,0,43,248,209,2,224,180,66,0,217,52,28,251,6,
32,39,0,43,8,219,3,224,136,66,0,216,7,128,2,48,
43,28,1,61,163,66,247,220,0,35,6,224,136,66,2,216,
22,136,2,50,6,128,2,48,1,51,163,66,246,219,3,224,
136,66,0,216,7,128,2,48,42,28,1,61,162,66,247,220,
240,188,112,71,166,47,0,0,128,181,19,28,10,28,3,73,
254,247,194,254,128,188,8,188,24,71,0,0,255,255,255,127,
247,181,146,176,0,33,15,145,19,155,25,120,0,41,4,209,
0,32,21,176,240,188,8,188,24,71,1,38,4,120,71,28,
0,44,99,209,0,32,244,231,242,72,120,68,66,92,18,6,
9,213,2,224,1,54,60,120,1,55,0,44,113,208,1,93,
9,6,247,212,109,224,37,41,108,209,0,34,14,146,0,35,
13,147,0,37,11,147,10,147,9,147,0,33,8,147,19,155,
1,51,19,147,19,155,27,120,42,43,13,209,19,155,1,51,
19,147,1,35,13,147,7,224,10,32,104,67,19,155,101,70,
1,51,45,24,48,61,19,147,19,155,219,72,27,120,156,70,
120,68,195,92,91,7,239,212,0,45,46,209,237,67,44,224,
19,155,27,120,104,43,13,208,6,220,73,43,16,208,76,43,
29,209,1,35,9,147,27,224,108,43,6,208,119,43,22,209,
1,33,21,224,1,35,11,147,18,224,1,35,10,147,15,224,
19,155,91,120,54,43,11,209,19,155,155,120,52,43,7,209,
1,35,8,147,19,155,2,51,19,147,1,224,40,227,1,34,
0,42,2,209,19,155,1,51,19,147,0,42,208,208,19,155,
26,120,102,42,113,208,25,220,88,42,52,208,15,220,67,42,
108,208,69,42,105,208,71,42,103,208,83,42,103,209,10,155,
1,224,0,227,4,227,25,67,113,209,11,155,92,226,99,42,
110,208,100,42,40,208,101,42,89,209,86,224,111,42,29,208,
19,220,103,42,81,208,105,42,33,208,110,42,79,209,13,155,
0,43,5,209,20,155,25,104,1,201,25,96,113,30,1,96,
1,35,13,147,1,32,14,144,231,224,115,42,113,208,117,42,
7,208,120,42,59,209,16,34,12,146,10,224,8,34,12,146,
7,224,10,34,12,146,4,224,10,34,12,146,1,224,0,34,
12,146,0,33,7,145,6,145,5,145,2,224,1,54,60,120,
1,55,0,44,2,208,1,93,9,6,247,212,45,44,1,208,
43,44,10,209,1,32,45,44,0,208,0,32,6,144,1,54,
60,120,1,55,0,45,0,221,1,61,0,45,44,208,48,44,
42,209,1,54,60,120,1,55,0,45,0,221,1,61,1,33,
5,145,0,45,27,208,120,44,2,224,167,224,93,226,131,226,
1,208,88,44,19,209,12,154,0,42,1,209,16,34,12,146,
12,154,16,42,16,209,1,54,60,120,1,55,1,224,24,226,
68,226,0,45,0,221,1,61,0,33,5,145,4,224,12,154,
0,42,1,209,8,34,12,146,12,154,0,42,10,209,10,34,
12,146,7,224,1,54,60,120,1,55,0,45,0,221,1,61,
1,33,5,145,0,45,1,208,48,44,243,208,0,45,40,208,
0,224,195,225,32,6,0,14,17,144,12,153,255,247,254,250,
67,28,30,208,17,152,12,153,255,247,248,250,7,144,1,54,
60,120,1,55,0,45,0,221,1,61,1,33,5,145,16,224,
16,152,12,153,255,247,234,250,7,153,12,154,81,67,65,24,
7,145,1,54,60,120,1,55,0,45,0,221,1,61,1,33,
5,145,0,45,9,208,0,44,7,208,32,6,0,14,16,144,
12,153,255,247,211,250,67,28,226,209,5,153,0,41,60,208,
1,32,14,144,13,155,0,43,55,209,8,155,0,43,11,208,
7,152,6,153,64,66,0,41,0,209,7,152,20,155,25,104,
4,201,25,96,16,96,40,224,10,155,0,43,11,208,7,152,
6,153,64,66,0,41,0,209,7,152,20,155,25,104,4,201,
25,96,16,96,25,224,11,155,0,43,11,208,7,152,6,153,
64,66,0,41,0,209,7,152,20,155,25,104,4,201,25,96,
16,128,10,224,7,152,6,153,64,66,0,41,0,209,7,152,
20,155,25,104,4,201,25,96,16,96,145,224,47,161,74,104,
11,104,7,146,6,147,0,34,5,146,2,224,1,54,60,120,
1,55,0,44,4,208,43,72,120,68,0,93,0,6,245,212,
45,44,1,208,43,44,12,209,1,32,45,44,0,208,0,32,
5,144,0,45,0,221,1,61,0,45,113,208,1,54,60,120,
1,55,46,44,72,208,32,72,120,68,0,93,64,7,103,213,
32,28,48,56,1,240,178,252,6,144,7,145,1,54,60,120,
1,55,0,45,27,221,1,61,25,224,24,160,12,200,6,169,
3,201,1,240,85,253,3,144,32,28,48,56,4,145,1,240,
157,252,1,144,2,145,3,171,12,203,0,240,87,254,6,144,
7,145,1,54,60,120,1,55,0,45,0,221,1,61,0,45,
30,208,0,44,28,208,11,72,120,68,0,93,64,7,220,212,
22,224,0,0,138,47,0,0,36,47,0,0,0,0,0,0,
0,0,0,0,92,44,0,0,44,44,0,0,0,0,0,0,
0,0,36,64,204,43,0,0,72,104,10,104,7,144,6,146,
0,45,52,208,46,44,50,209,127,32,192,5,4,144,1,54,
60,120,1,55,0,45,33,221,1,61,31,224,196,73,4,152,
2,240,188,249,4,144,32,28,48,56,2,240,171,250,4,153,
2,240,216,250,2,240,48,249,2,144,3,145,11,28,0,224,
187,224,2,28,6,169,3,201,0,240,8,254,6,144,7,145,
1,54,60,120,1,55,0,45,0,221,1,61,0,45,6,208,
0,44,4,208,179,72,120,68,0,93,64,7,214,212,0,45,
89,208,101,44,1,208,69,44,85,209,0,32,4,144,1,54,
60,120,1,55,0,45,0,221,1,61,0,45,13,208,43,44,
1,208,45,44,9,209,1,32,45,44,0,208,0,32,1,54,
60,120,1,55,0,45,0,221,1,61,163,73,121,68,11,224,
4,154,10,35,90,67,18,25,48,58,4,146,1,54,60,120,
1,55,0,45,0,221,1,61,0,45,4,208,0,44,2,208,
10,93,82,7,236,212,0,40,1,208,152,160,1,224,120,70,
246,56,3,200,0,240,220,252,5,28,25,224,4,152,192,7,
12,213,40,28,2,240,208,248,11,28,3,145,6,169,2,28,
2,144,3,201,1,240,156,252,6,144,7,145,4,152,193,15,
8,24,64,16,4,144,41,28,40,28,2,240,99,250,5,28,
4,152,0,40,226,209,1,32,14,144,13,155,0,43,68,209,
9,155,0,43,18,208,20,155,24,104,32,200,24,96,5,152,
0,40,7,208,6,169,3,201,0,240,146,252,3,144,4,145,
3,197,50,224,6,169,3,201,3,197,46,224,10,155,0,43,
18,208,20,155,24,104,32,200,24,96,5,152,0,40,7,208,
6,169,3,201,0,240,124,252,3,144,4,145,3,197,28,224,
6,169,3,201,3,197,24,224,5,152,0,40,8,208,6,169,
3,201,0,240,109,252,3,144,4,145,1,144,2,145,3,224,
6,169,3,201,1,144,2,145,1,169,3,201,0,240,120,252,
20,155,25,104,4,201,25,96,16,96,121,224,10,155,25,67,
47,209,11,155,0,43,44,208,13,155,0,33,0,43,7,209,
20,155,26,104,2,202,26,96,2,224,1,54,60,120,1,55,
0,44,17,208,2,93,18,6,247,212,13,224,13,155,0,43,
1,209,12,112,1,49,14,154,1,50,14,146,1,54,60,120,
1,55,0,45,0,221,1,61,0,45,4,208,0,44,2,208,
2,93,18,6,234,213,13,155,0,43,73,209,0,32,8,112,
70,224,13,155,0,33,0,43,7,209,20,155,26,104,2,202,
26,96,2,224,1,54,60,120,1,55,0,44,17,208,2,93,
18,6,247,212,13,224,13,155,0,43,1,209,12,112,1,49,
14,154,1,50,14,146,1,54,60,120,1,55,0,45,0,221,
1,61,0,45,4,208,0,44,2,208,2,93,18,6,234,213,
13,155,0,43,28,209,0,32,8,112,25,224,10,155,25,67,
23,209,11,155,0,43,20,208,3,224,10,155,25,67,16,209,
11,155,0,44,12,208,13,155,0,43,4,209,20,155,25,104,
1,201,25,96,4,112,1,32,14,144,1,54,60,120,1,55,
32,224,0,44,30,208,13,155,0,43,4,209,20,155,25,104,
1,201,25,96,4,112,1,54,60,120,1,55,1,32,14,144,
16,224,1,54,60,120,1,55,0,44,2,208,1,93,9,6,
247,212,162,66,6,209,1,35,13,147,1,32,14,144,1,54,
60,120,1,55,14,152,0,40,6,208,13,155,0,43,3,209,
15,152,1,48,15,144,8,224,14,152,0,40,13,208,4,224,
161,66,10,209,1,54,60,120,1,55,19,155,1,51,19,147,
19,155,25,120,0,41,0,208,110,228,15,152,97,228,0,0,
0,0,32,65,46,43,0,0,232,42,0,0,154,153,153,153,
153,153,185,63,15,180,8,181,5,170,0,146,107,70,4,154,
254,247,182,253,1,176,8,188,4,176,24,71,15,180,8,181,
4,169,0,145,106,70,3,153,255,247,58,252,1,176,8,188,
4,176,24,71,15,180,8,181,4,169,0,145,107,70,4,73,
3,154,254,247,157,253,1,176,8,188,4,176,24,71,0,0,
255,255,255,127,128,181,19,28,10,28,3,73,254,247,144,253,
128,188,8,188,24,71,0,0,255,255,255,127,16,180,0,34,
5,224,1,51,3,96,10,35,90,67,138,24,48,58,3,104,
25,120,12,28,48,60,10,44,243,211,16,28,16,188,112,71,
255,181,149,176,32,159,4,28,14,28,97,72,120,68,1,28,
40,57,122,6,0,212,1,28,1,145,248,6,1,213,127,8,
127,0,24,153,2,41,2,219,24,153,36,41,4,221,0,32,
25,176,240,188,8,188,24,71,48,32,249,7,0,212,32,32,
1,6,9,14,20,145,0,33,19,145,184,7,21,213,30,152,
23,153,1,56,0,41,5,218,45,34,19,146,73,66,23,145,
30,144,10,224,121,7,3,213,43,33,19,145,30,144,4,224,
57,7,2,213,32,33,19,145,30,144,184,6,12,213,24,153,
16,41,3,209,30,152,2,56,30,144,5,224,24,153,8,41,
2,209,30,152,1,56,30,144,0,37,23,152,0,40,14,209,
48,32,0,171,24,114,1,37,12,224,23,168,24,153,254,247,
255,255,1,153,2,170,9,92,40,28,1,53,17,84,23,152,
0,40,242,209,31,152,133,66,0,221,31,149,30,169,3,201,
64,26,17,34,57,28,17,64,32,34,0,41,8,209,3,224,
180,66,0,216,34,112,1,52,1,28,1,56,0,41,247,220,
19,153,0,41,4,208,180,66,1,216,19,153,33,112,1,52,
185,6,23,213,24,153,8,41,5,209,180,66,1,216,48,33,
33,112,1,52,14,224,24,153,16,41,11,209,180,66,1,216,
48,33,33,112,97,28,177,66,3,216,1,155,32,51,91,120,
11,112,76,28,249,6,9,212,4,224,180,66,1,216,20,153,
33,112,1,52,1,28,1,56,0,41,246,220,48,35,3,224,
180,66,0,216,35,112,1,52,31,153,79,30,31,151,169,66,
246,220,5,224,180,66,2,216,2,169,73,93,33,112,1,52,
41,28,1,61,0,41,245,220,3,224,180,66,0,216,34,112,
1,52,1,28,1,56,0,41,247,220,32,28,80,231,0,0,
124,39,0,0,255,181,149,176,31,158,33,159,5,28,233,160,
1,104,64,104,18,145,19,144,0,36,0,32,6,144,30,152,
103,40,8,208,30,152,71,40,5,208,30,152,101,40,2,208,
30,152,69,40,25,209,24,152,23,153,64,0,0,41,6,209,
24,153,9,3,3,209,65,13,1,209,0,32,2,224,219,73,
64,13,64,24,1,240,146,249,4,144,217,160,5,145,12,200,
4,152,0,240,25,254,18,144,19,145,30,152,103,40,2,208,
30,152,71,40,32,209,128,32,7,67,211,160,12,200,18,169,
3,201,1,240,235,249,15,211,18,169,3,201,4,144,73,0,
73,8,5,145,32,152,1,240,113,249,2,144,3,145,4,171,
12,203,1,240,219,249,5,216,30,152,254,48,0,6,0,14,
30,144,1,224,102,32,30,144,113,30,30,152,20,145,101,40,
2,208,30,152,69,40,96,209,18,169,3,201,16,170,254,247,
31,249,13,144,14,145,190,160,12,200,13,169,3,201,1,240,
123,249,11,217,16,169,3,201,4,144,187,160,5,145,12,200,
16,152,0,240,3,251,16,144,17,145,17,224,184,160,12,200,
13,169,3,201,1,240,170,249,10,210,16,169,3,201,4,144,
181,160,5,145,12,200,16,152,0,240,240,250,16,144,17,145,
16,171,12,203,178,160,3,200,254,247,246,248,2,144,3,145,
23,171,12,203,0,240,158,253,5,145,32,153,4,144,48,31,
102,34,1,144,2,145,22,153,0,146,3,151,4,171,12,203,
40,28,255,247,87,255,30,153,68,28,1,112,16,169,3,201,
1,240,176,248,5,28,20,34,0,45,0,218,22,34,2,33,
2,146,42,28,0,145,1,145,10,35,32,28,22,153,255,247,
119,254,25,176,240,188,8,188,24,71,30,152,102,40,113,209,
254,247,210,254,15,144,248,6,1,213,127,8,127,0,48,32,
249,7,0,212,32,32,0,6,0,14,7,144,0,32,8,144,
184,7,27,213,127,160,12,200,23,169,3,201,1,240,78,249,
9,210,45,32,8,144,23,169,3,201,23,144,73,0,73,8,
24,145,20,158,10,224,120,7,3,213,43,32,8,144,20,158,
4,224,56,7,2,213,32,32,8,144,20,158,23,169,3,201,
11,170,254,247,141,248,13,144,14,145,32,152,0,40,52,221,
32,156,1,60,22,224,122,160,12,200,13,169,3,201,1,240,
103,249,13,144,14,145,13,169,3,201,9,170,254,247,120,248,
13,144,14,145,9,169,3,201,1,240,76,248,15,153,48,48,
8,85,1,60,0,44,230,218,32,156,32,156,54,27,0,32,
6,144,99,160,12,200,13,169,3,201,1,240,197,248,1,217,
1,32,6,144,32,152,1,40,1,218,184,6,5,213,46,33,
32,28,15,154,1,52,17,84,1,62,82,160,12,200,11,169,
3,201,0,224,206,224,0,240,227,255,50,209,48,34,32,28,
15,153,1,52,10,84,1,62,50,224,11,169,3,201,9,144,
10,145,87,160,12,200,11,169,3,201,0,240,253,252,11,144,
12,145,11,169,3,201,11,170,254,247,50,248,4,144,5,145,
79,160,12,200,11,169,3,201,1,240,18,249,4,144,5,145,
9,171,12,203,0,240,18,251,9,144,10,145,9,169,3,201,
0,240,248,255,48,48,1,28,32,28,15,154,1,52,17,84,
1,62,52,160,12,200,11,169,3,201,1,240,117,248,204,216,
0,32,48,34,16,224,15,153,9,92,11,28,48,59,8,43,
5,216,15,155,1,49,25,84,0,33,6,145,3,224,57,41,
1,209,15,153,10,84,1,48,160,66,2,218,6,153,1,41,
233,208,6,152,1,40,4,209,49,33,32,28,15,154,1,52,
17,84,15,153,0,32,8,85,32,152,48,26,17,33,59,28,
11,64,32,34,0,43,6,209,1,224,42,112,1,53,1,28,
1,56,0,41,249,220,8,153,0,41,2,208,8,153,41,112,
1,53,0,43,6,209,1,224,42,112,1,53,1,28,1,56,
0,41,249,220,249,6,7,212,2,224,7,153,41,112,1,53,
1,28,1,56,0,41,248,220,15,153,59,6,55,213,187,6,
53,212,0,35,1,224,1,49,1,60,163,66,2,218,14,120,
48,46,248,208,163,66,42,218,11,120,46,43,39,209,1,49,
1,60,36,224,0,0,0,0,0,0,0,0,1,252,255,255,
97,85,189,252,78,147,10,64,0,0,0,0,0,0,16,192,
0,0,0,0,0,0,224,63,0,0,0,0,0,0,240,63,
0,0,0,0,0,0,224,191,0,0,0,0,0,0,240,191,
0,0,0,0,0,0,36,64,11,93,43,112,1,53,35,28,
1,60,0,43,248,220,1,224,42,112,1,53,1,28,1,56,
0,41,249,220,40,28,180,230,240,180,4,175,224,207,28,28,
0,42,3,209,26,74,122,68,6,36,13,224,99,28,8,209,
0,36,0,224,1,52,180,66,6,210,19,93,0,43,249,209,
2,224,180,66,0,217,52,28,251,6,32,39,0,43,8,219,
3,224,136,66,0,216,7,112,1,48,43,28,1,61,163,66,
247,220,0,35,6,224,136,66,2,216,22,120,1,50,6,112,
1,48,1,51,163,66,246,219,3,224,136,66,0,216,7,112,
1,48,42,28,1,61,162,66,247,220,240,188,112,71,0,0,
114,33,0,0,240,180,4,175,224,207,28,28,0,42,3,209,
26,74,122,68,6,36,13,224,99,28,8,209,0,36,0,224,
1,52,180,66,6,210,19,93,0,43,249,209,2,224,180,66,
0,217,52,28,251,6,32,39,0,43,8,219,3,224,136,66,
0,216,7,112,1,48,43,28,1,61,163,66,247,220,0,35,
6,224,136,66,2,216,22,120,1,50,6,112,1,48,1,51,
163,66,246,219,3,224,136,66,0,216,7,112,1,48,42,28,
1,61,162,66,247,220,240,188,112,71,0,0,246,32,0,0,
120,71,0,0,128,20,33,226,30,255,47,225,120,71,0,0,
128,4,32,226,30,255,47,225,120,71,0,0,128,20,193,227,
30,255,47,225,120,71,0,0,128,4,192,227,30,255,47,225,
120,71,0,0,128,52,1,226,128,20,193,227,129,32,33,224,
127,4,18,227,224,8,18,3,47,0,0,10,192,196,160,227,
0,34,176,225,5,0,0,138,1,0,0,42,0,192,160,3,
5,0,0,234,128,37,0,226,2,193,156,224,2,0,0,74,
64,196,160,227,128,5,144,226,0,16,161,226,224,21,65,226,
64,9,81,227,4,0,0,186,255,6,81,227,23,0,0,170,
129,49,131,225,160,14,131,225,30,255,47,225,192,22,129,226,
3,0,45,233,12,80,45,233,2,246,255,235,12,80,189,232,
0,32,160,225,3,0,189,232,0,32,146,229,128,14,18,227,
128,197,140,19,136,192,140,19,224,21,129,18,3,16,129,17,
224,4,0,18,156,7,0,26,64,9,81,227,255,20,193,179,
129,49,131,225,160,14,131,225,12,32,160,225,18,7,0,234,
192,22,65,226,224,21,129,226,224,4,0,226,3,16,129,225,
64,197,140,227,136,192,140,227,109,5,0,234,64,9,17,227,
45,0,0,26,129,32,144,225,3,0,160,1,30,255,47,1,
3,0,45,233,12,80,45,233,226,245,255,235,12,80,189,232,
0,32,160,225,3,0,189,232,0,32,146,229,128,14,18,227,
1,0,0,26,64,25,160,227,191,255,255,234,0,192,160,227,
1,0,17,225,160,21,160,1,128,10,160,1,84,199,76,2,
161,34,176,225,1,24,160,1,32,24,129,1,0,8,160,1,
64,199,76,2,161,38,176,225,1,20,160,1,32,28,129,1,
0,4,160,1,128,200,76,2,161,40,176,225,1,18,160,1,
32,30,129,1,0,2,160,1,64,200,76,2,161,41,176,225,
1,17,160,1,32,31,129,1,0,1,160,1,128,201,76,2,
33,42,176,225,129,16,160,1,160,31,129,1,128,0,160,1,
64,201,76,2,12,16,129,224,159,255,255,234,3,16,129,225,
1,38,144,225,12,192,159,229,64,1,0,26,255,255,255,234,
112,8,193,227,30,255,47,225,136,0,0,4,120,71,0,0,
16,64,45,233,140,196,159,229,129,64,220,225,131,64,220,17,
89,0,0,10,3,0,49,225,128,52,35,66,141,0,0,74,
2,64,80,224,3,224,209,224,3,0,0,42,4,32,146,224,
14,48,163,224,4,0,80,224,14,16,193,224,33,74,160,225,
35,234,68,224,131,0,28,225,56,0,0,10,4,26,193,225,
12,48,195,225,64,57,131,227,32,192,126,226,21,0,0,58,
50,14,144,224,51,30,161,224,19,12,144,224,0,16,177,226,
64,9,81,227,25,0,0,42,4,26,129,224,16,64,189,232,
18,44,176,225,30,255,47,81,1,0,144,226,130,32,176,49,
30,255,47,17,0,0,80,227,1,0,192,19,0,16,161,2,
129,48,160,225,128,9,115,227,30,255,47,49,232,195,159,229,
96,20,65,34,6,5,0,234,1,0,82,227,3,32,163,224,
32,224,78,226,31,192,126,226,0,192,160,51,51,14,144,32,
4,26,161,224,33,10,84,225,231,255,255,10,4,26,65,224,
64,25,129,226,161,16,176,225,4,26,129,224,16,64,189,232,
96,0,176,225,7,0,0,58,0,0,176,226,18,204,176,49,
4,0,0,26,226,255,255,234,1,0,144,226,130,32,176,49,
226,255,255,26,222,255,255,234,129,48,160,225,128,9,115,227,
30,255,47,49,112,195,159,229,96,20,65,226,232,4,0,234,
131,192,146,225,9,0,0,10,132,202,176,225,4,26,193,225,
128,52,195,19,1,224,78,18,194,255,255,26,16,64,189,232,
2,0,144,224,3,16,161,224,17,192,160,227,171,0,0,234,
132,202,176,225,16,64,189,232,30,255,47,17,129,192,144,225,
30,255,47,1,17,192,160,227,164,0,0,234,1,0,80,227,
1,64,161,224,1,0,82,227,3,224,163,224,12,0,84,225,
12,0,94,145,4,195,159,229,16,64,189,232,194,0,0,138,
255,255,255,234,1,0,80,227,1,0,161,224,1,0,82,227,
3,32,163,224,2,0,80,225,5,0,0,10,128,9,112,227,
0,0,160,3,30,255,47,1,3,16,160,225,0,0,160,227,
30,255,47,225,0,0,160,227,3,0,49,225,30,255,47,81,
0,32,160,227,180,194,159,229,185,4,0,234,120,71,0,0,
16,64,45,233,156,194,159,229,129,64,220,225,131,64,220,17,
4,0,0,10,3,0,49,225,128,20,33,226,131,255,255,74,
128,52,35,226,15,0,0,234,3,16,33,224,2,0,32,224,
1,48,35,224,0,32,34,224,3,16,33,224,2,0,32,224,
124,0,0,234,120,71,0,0,16,64,45,233,84,194,159,229,
129,64,220,225,131,64,220,17,118,0,0,10,3,0,49,225,
128,52,35,66,113,255,255,74,2,64,80,224,3,224,209,224,
4,0,0,42,128,228,46,226,4,32,146,224,14,48,163,224,
4,0,80,224,14,16,193,224,33,74,160,225,35,234,68,224,
131,0,28,225,77,0,0,10,4,26,193,225,204,48,195,225,
0,32,114,226,204,48,227,224,32,192,126,226,16,0,0,58,
50,14,144,224,83,30,161,224,19,12,144,224,0,16,177,226,
21,0,0,74,4,26,129,224,16,64,189,232,18,44,176,225,
73,193,160,227,30,255,47,81,1,0,144,226,128,4,82,19,
30,255,47,17,0,0,80,227,1,16,129,2,1,0,192,19,
30,255,47,225,2,192,146,224,3,32,163,224,1,192,160,19,
130,32,140,225,32,224,78,226,30,192,126,226,39,0,0,154,
83,14,144,224,0,16,209,226,233,255,255,90,1,192,140,226,
18,204,176,225,0,0,176,224,1,16,161,224,132,234,129,224,
174,234,176,225,8,0,0,154,4,26,129,224,16,64,189,232,
172,15,144,224,128,4,92,51,30,255,47,17,0,0,80,227,
1,16,129,2,1,0,192,19,30,255,47,225,22,0,0,42,
128,25,145,226,0,0,80,3,16,64,189,8,40,0,0,10,
164,229,160,225,128,78,196,227,2,64,68,226,0,0,144,224,
1,16,161,224,1,64,68,226,64,9,17,227,250,255,255,10,
142,31,129,224,4,26,129,224,0,0,84,227,16,64,189,232,
30,255,47,193,96,20,129,226,18,192,160,227,190,3,0,234,
4,26,129,224,16,64,189,232,30,255,47,225,193,16,176,225,
96,0,160,225,4,26,129,224,16,64,189,232,18,0,0,234,
3,198,146,225,11,0,0,10,132,202,176,225,4,26,193,225,
4,0,0,10,128,52,195,227,1,224,78,226,0,32,114,226,
0,48,227,226,171,255,255,234,2,0,80,224,3,16,193,224,
16,64,189,232,4,0,0,234,16,64,189,232,129,192,144,225,
1,0,0,26,0,16,160,227,30,255,47,225,18,192,160,227,
129,32,176,225,0,0,16,1,249,255,255,10,128,9,82,227,
30,255,47,33,246,3,0,234,1,0,80,227,1,64,161,224,
1,0,82,227,3,224,163,224,12,0,84,225,12,0,94,145,
92,192,159,229,16,64,189,232,23,0,0,138,255,255,255,234,
1,0,80,227,1,0,161,224,1,0,82,227,3,32,163,224,
2,0,80,225,5,0,0,10,128,9,112,227,0,0,160,3,
30,255,47,1,128,20,35,226,0,0,160,227,30,255,47,225,
0,0,160,227,3,0,49,225,30,255,47,65,0,32,160,227,
12,192,159,229,14,4,0,234,0,0,224,255,17,0,0,16,
17,0,0,4,18,0,0,4,120,71,0,0,48,0,45,233,
64,89,160,227,1,0,80,227,129,64,165,224,1,0,82,227,
131,80,165,224,64,9,116,227,64,9,117,147,4,0,0,138,
64,9,84,227,48,0,189,232,3,16,160,33,2,0,160,33,
246,5,0,234,48,0,189,232,249,3,0,234,120,71,0,0,
128,10,17,227,246,3,0,10,79,32,12,226,72,0,82,227,
238,5,0,26,242,3,0,234,120,71,0,0,0,64,45,233,
128,233,160,227,129,0,126,225,0,0,80,3,131,0,126,145,
0,0,82,3,14,0,0,138,0,64,189,232,255,255,255,234,
3,192,145,225,3,0,0,74,3,0,81,225,2,0,80,1,
3,0,0,106,30,255,47,225,1,0,83,225,0,0,82,1,
30,255,47,113,1,192,160,227,2,0,92,51,0,0,92,99,
30,255,47,225,0,64,189,232,80,10,28,227,216,3,0,26,
205,255,255,234,120,71,0,0,112,64,45,233,3,16,33,224,
2,0,32,224,1,48,35,224,0,32,34,224,3,16,33,224,
2,0,32,224,1,0,0,234,120,71,0,0,112,64,45,233,
136,197,159,229,33,66,220,225,35,66,220,17,139,0,0,10,
33,66,12,224,3,80,33,224,165,79,132,225,35,82,28,224,
12,2,17,17,53,0,0,10,5,64,68,224,255,71,132,226,
129,21,160,225,131,53,160,225,160,26,129,225,162,58,131,225,
128,20,129,227,128,52,131,227,128,5,160,225,130,37,160,225,
150,0,0,235,68,56,131,224,128,106,176,225,160,5,160,225,
129,10,128,225,161,21,160,225,3,26,129,224,132,31,33,224,
10,0,0,10,134,224,176,225,192,100,160,51,7,0,0,58,
0,48,160,19,128,52,160,3,0,0,176,226,0,16,161,226,
64,100,160,227,163,15,16,225,163,15,192,225,192,100,160,19,
132,15,49,225,232,228,159,229,12,0,0,74,14,0,17,225,
1,224,222,17,9,0,0,10,112,64,189,232,30,255,47,225,
132,95,176,225,128,8,30,83,64,8,30,67,64,100,160,3,
192,100,160,19,1,0,144,2,0,16,161,226,239,255,255,234,
64,6,84,227,96,20,129,210,96,20,65,162,20,192,134,227,
64,197,140,163,112,64,189,232,255,2,0,218,140,3,0,234,
129,224,144,225,6,0,0,10,131,224,146,225,10,0,0,10,
5,0,21,225,42,0,0,10,1,224,212,227,9,0,0,10,
192,255,255,234,131,224,146,225,92,0,0,10,3,0,19,225,
128,20,33,66,112,64,189,232,30,255,47,225,84,196,159,229,
112,64,189,232,122,3,0,234,64,75,132,226,128,20,193,227,
32,230,145,225,0,10,160,1,80,73,68,2,1,0,17,225,
32,22,160,1,0,10,160,1,80,73,68,2,0,96,160,227,
161,226,176,225,1,24,160,1,16,96,134,2,161,230,176,225,
1,20,160,1,8,96,134,2,161,232,176,225,1,18,160,1,
4,96,134,2,161,233,176,225,1,17,160,1,2,96,134,2,
33,234,176,225,129,16,160,1,1,96,134,2,6,72,68,224,
16,230,160,225,32,96,102,226,48,22,129,225,14,0,160,225,
152,255,255,234,64,91,133,226,128,52,195,227,34,230,147,225,
2,42,160,1,80,89,69,2,3,0,19,225,34,54,160,1,
2,42,160,1,80,89,69,2,0,96,160,227,163,226,176,225,
3,56,160,1,16,96,134,2,163,230,176,225,3,52,160,1,
8,96,134,2,163,232,176,225,3,50,160,1,4,96,134,2,
163,233,176,225,3,49,160,1,2,96,134,2,35,234,176,225,
131,48,160,1,1,96,134,2,6,88,69,224,18,230,160,225,
32,96,102,226,50,54,131,225,14,32,160,225,181,255,255,234,
129,224,160,225,131,80,160,225,140,2,94,225,0,0,80,3,
140,2,85,145,0,0,82,3,112,64,189,232,56,195,159,229,
41,255,255,138,255,255,255,234,32,195,159,229,33,2,220,225,
6,0,0,26,35,2,220,225,0,0,160,227,8,0,0,10,
3,48,49,224,128,20,193,227,128,20,129,67,30,255,47,225,
3,48,33,224,0,0,160,227,128,20,3,226,30,255,47,225,
112,64,189,232,240,194,159,229,33,3,0,234,120,71,0,0,
128,73,45,233,112,0,45,233,35,72,160,225,34,104,160,225,
4,88,195,225,6,232,194,225,112,127,143,226,36,116,215,231,
148,119,44,224,128,200,108,226,156,7,7,224,167,121,160,225,
2,112,135,226,163,198,160,225,156,119,40,224,128,133,104,226,
40,200,160,225,12,136,200,225,152,7,2,224,156,7,3,224,
34,120,131,224,39,115,160,225,161,16,176,225,96,0,176,225,
0,176,160,51,128,180,160,35,161,199,160,225,151,12,8,224,
40,136,160,225,152,6,12,224,12,0,80,224,148,8,12,224,
12,16,193,224,152,14,12,224,12,184,91,224,44,8,208,224,
152,5,12,224,12,8,64,48,12,8,80,32,44,24,193,224,
8,56,160,225,33,193,160,225,151,12,8,224,40,136,160,225,
152,6,12,224,140,185,91,224,172,6,208,224,148,8,12,224,
140,9,64,48,140,9,80,32,172,22,193,224,152,14,12,224,
140,177,91,224,172,14,208,224,152,5,12,224,140,1,64,48,
140,1,80,32,172,30,193,224,1,29,160,225,32,19,129,225,
0,13,160,225,43,3,128,225,11,189,160,225,136,49,131,224,
161,199,160,225,151,12,8,224,40,136,160,225,152,6,12,224,
12,0,80,224,148,8,12,224,12,16,193,224,152,14,12,224,
12,184,91,224,44,8,208,224,152,5,12,224,12,8,64,48,
12,8,80,32,44,24,193,224,8,43,160,225,40,53,131,224,
33,193,160,225,151,12,8,224,40,136,160,225,152,6,12,224,
140,185,91,224,172,6,208,224,148,8,12,224,140,9,64,48,
140,9,80,32,172,22,193,224,152,14,12,224,140,177,91,224,
172,14,208,224,152,5,12,224,140,1,64,48,140,1,80,32,
172,30,193,224,1,29,160,225,32,19,129,225,0,13,160,225,
43,3,128,225,11,189,160,225,136,36,146,224,0,48,163,226,
4,72,133,225,6,88,142,225,0,224,160,227,11,176,155,224,
0,0,176,224,1,16,161,224,5,128,80,224,4,192,209,224,
8,0,160,33,12,16,160,33,14,224,174,224,0,96,160,227,
11,176,155,224,0,0,176,224,1,16,177,224,6,96,166,224,
5,128,80,224,4,192,209,224,0,96,214,226,8,0,160,33,
12,16,160,33,14,224,174,224,0,96,160,227,11,176,155,224,
0,0,176,224,1,16,177,224,6,96,166,224,5,128,80,224,
4,192,209,224,0,96,214,226,8,0,160,33,12,16,160,33,
14,224,174,224,0,112,145,225,1,112,160,19,142,4,146,224,
0,16,179,226,2,48,160,67,1,48,160,83,7,0,128,225,
240,0,189,72,0,73,189,72,30,255,47,65,0,0,144,224,
1,16,161,224,240,0,189,232,0,73,189,232,30,255,47,225,
128,128,127,126,125,124,123,122,121,120,119,118,118,117,116,115,
114,113,113,112,111,110,110,109,108,108,107,106,106,105,104,104,
103,102,102,101,100,100,99,99,98,97,97,96,96,95,95,94,
94,93,93,92,92,91,91,90,90,89,89,88,88,87,87,86,
86,85,85,85,84,84,83,83,82,82,82,81,81,80,80,80,
79,79,79,78,78,77,77,77,76,76,76,75,75,75,74,74,
74,73,73,73,72,72,72,71,71,71,71,70,70,70,69,69,
69,68,68,68,68,67,67,67,67,66,66,66,66,65,65,65,
0,0,255,7,0,0,240,127,20,0,0,8,20,0,0,4,
120,71,0,0,3,192,145,225,11,0,0,74,64,9,124,227,
3,0,0,74,3,0,81,225,30,255,47,17,2,0,80,1,
30,255,47,225,64,9,113,227,64,9,115,83,17,0,0,74,
3,0,81,225,2,0,80,1,30,255,47,225,64,9,124,227,
5,0,0,90,129,192,144,225,131,192,146,1,1,0,83,17,
30,255,47,17,0,0,82,1,30,255,47,225,128,201,160,227,
129,0,124,225,131,0,124,49,2,0,0,42,1,0,83,225,
0,0,82,1,30,255,47,225,0,192,159,229,82,254,255,234,
25,0,18,4,120,71,0,0,3,0,45,233,65,42,176,225,
129,53,160,225,160,58,131,225,128,52,131,19,8,0,0,74,
224,47,66,226,158,32,114,226,19,0,0,218,255,0,82,227,
0,16,160,195,51,18,160,209,1,0,160,225,8,208,141,226,
30,255,47,225,130,42,160,225,162,42,176,225,128,52,195,3,
0,16,160,227,224,47,66,226,158,32,114,226,6,0,0,186,
255,0,82,227,51,18,65,208,0,0,81,227,8,208,141,210,
1,0,160,209,30,255,47,209,255,255,255,234,3,0,189,232,
32,192,159,229,1,0,80,227,1,48,161,224,128,9,115,227,
64,203,140,131,37,254,255,138,29,2,0,58,255,255,255,234,
0,192,159,229,26,2,0,234,200,0,2,4,120,71,0,0,
64,52,160,227,0,16,160,225,4,0,0,234,120,71,0,0,
128,52,16,226,0,16,96,18,0,16,160,1,64,52,131,227,
33,8,176,225,64,55,131,18,1,24,176,1,30,255,47,1,
255,4,17,227,128,56,131,18,1,20,160,1,240,4,17,227,
64,56,131,18,1,18,160,1,192,4,17,227,128,57,131,18,
1,17,176,1,64,57,131,66,129,16,160,81,129,10,160,225,
193,21,131,224,30,255,47,225,120,71,0,0,3,192,145,225,
11,0,0,74,64,9,124,227,3,0,0,74,3,0,81,225,
30,255,47,17,2,0,80,1,30,255,47,225,64,9,113,227,
64,9,115,83,17,0,0,74,3,0,81,225,2,0,80,1,
30,255,47,225,64,9,124,227,5,0,0,90,129,192,144,225,
131,192,146,1,1,0,83,17,30,255,47,17,0,0,82,1,
30,255,47,225,128,201,160,227,129,0,124,225,131,0,124,49,
2,0,0,42,1,0,83,225,0,0,82,1,30,255,47,225,
0,192,159,229,236,253,255,234,25,0,19,4,120,71,0,0,
3,192,145,225,11,0,0,74,64,9,124,227,3,0,0,74,
3,0,81,225,30,255,47,17,2,0,80,1,30,255,47,225,
64,9,113,227,64,9,115,83,17,0,0,74,3,0,81,225,
2,0,80,1,30,255,47,225,64,9,124,227,5,0,0,90,
129,192,144,225,131,192,146,1,1,0,83,17,30,255,47,17,
0,0,82,1,30,255,47,225,128,201,160,227,129,0,124,225,
131,0,124,49,2,0,0,42,1,0,83,225,0,0,82,1,
30,255,47,225,0,192,159,229,203,253,255,234,25,0,22,4,
120,71,0,0,112,64,45,233,164,194,159,229,33,226,220,225,
35,226,220,17,145,0,0,10,33,226,12,224,3,80,33,224,
165,239,142,225,35,82,28,224,12,2,17,17,65,0,0,10,
5,224,142,224,255,231,78,226,140,18,193,225,140,50,195,225,
64,25,129,227,64,57,131,227,145,66,140,224,144,99,133,224,
4,96,150,224,12,80,181,224,145,67,140,224,0,48,172,226,
144,18,140,224,12,96,150,224,4,80,181,224,0,48,179,226,
1,0,17,225,1,96,134,19,3,32,224,227,20,192,160,227,
128,15,83,227,0,192,172,226,78,40,162,224,32,64,108,226,
19,20,160,225,21,4,160,225,53,28,129,225,54,12,128,225,
2,26,129,224,142,31,33,224,22,68,176,225,9,0,0,10,
132,192,176,225,192,68,160,51,6,0,0,58,128,52,131,3,
0,0,176,226,0,16,161,226,64,68,160,227,163,15,16,225,
163,15,192,225,192,68,160,19,142,15,49,225,212,193,159,229,
12,0,0,74,12,0,17,225,1,192,220,17,9,0,0,10,
112,64,189,232,30,255,47,225,142,79,176,225,128,8,28,83,
64,8,28,67,64,68,160,3,192,68,160,19,1,0,144,2,
0,16,161,226,239,255,255,234,64,6,94,227,96,20,129,210,
96,20,65,162,19,192,132,227,64,197,140,163,112,64,189,232,
225,0,0,218,110,1,0,234,129,64,144,225,131,64,146,17,
4,0,0,10,5,0,21,225,37,0,0,10,1,64,222,227,
4,0,0,10,181,255,255,234,0,0,160,227,142,31,160,225,
112,64,189,232,30,255,47,225,64,235,142,226,128,20,193,227,
32,102,145,225,0,10,160,1,80,233,78,2,1,0,17,225,
32,22,160,1,0,10,160,1,80,233,78,2,0,64,160,227,
161,98,176,225,1,24,160,1,16,64,132,2,161,102,176,225,
1,20,160,1,8,64,132,2,161,104,176,225,1,18,160,1,
4,64,132,2,161,105,176,225,1,17,160,1,2,64,132,2,
33,106,176,225,129,16,160,1,1,64,132,2,4,232,78,224,
16,100,160,225,32,64,100,226,48,20,129,225,6,0,160,225,
146,255,255,234,64,91,133,226,128,52,195,227,34,102,147,225,
2,42,160,1,80,89,69,2,3,0,19,225,34,54,160,1,
2,42,160,1,80,89,69,2,0,64,160,227,163,98,176,225,
3,56,160,1,16,64,132,2,163,102,176,225,3,52,160,1,
8,64,132,2,163,104,176,225,3,50,160,1,4,64,132,2,
163,105,176,225,3,49,160,1,2,64,132,2,35,106,176,225,
131,48,160,1,1,64,132,2,4,88,69,224,18,100,160,225,
32,64,100,226,50,52,131,225,6,32,160,225,186,255,255,234,
129,80,160,225,131,96,160,225,140,2,85,225,0,0,80,3,
140,2,86,145,0,0,82,3,112,64,189,232,56,192,159,229,
17,253,255,138,255,255,255,234,129,192,144,225,131,192,146,17,
5,0,0,10,3,16,33,224,0,0,160,227,128,20,1,226,
127,20,129,227,240,24,129,227,30,255,47,225,8,192,159,229,
15,1,0,234,0,0,255,7,0,0,240,127,19,0,0,4,
120,71,0,0,112,64,45,233,112,196,160,227,255,198,140,227,
12,0,17,225,71,0,0,10,1,224,220,225,104,0,0,10,
128,4,17,227,111,0,0,26,33,202,160,225,12,26,193,225,
64,25,129,227,253,192,140,226,172,192,176,225,96,207,140,226,
1,0,0,58,128,0,176,225,1,16,161,224,1,21,160,225,
32,27,129,225,0,5,160,225,64,36,160,227,0,48,160,227,
64,20,65,226,64,229,160,227,2,64,81,224,14,64,84,32,
4,16,160,33,142,32,130,32,128,0,176,225,1,16,161,224,
174,224,176,225,247,255,255,26,2,64,209,224,4,0,0,58,
128,84,80,226,0,64,212,226,4,16,160,33,5,0,160,33,
1,32,130,34,128,0,176,225,1,16,161,224,64,228,160,227,
3,80,80,224,2,64,209,224,4,0,0,58,14,80,85,224,
0,64,212,226,4,16,160,33,5,0,160,33,142,48,131,32,
128,0,176,225,1,16,161,224,174,224,160,225,128,0,94,227,
242,255,255,26,0,64,145,225,2,64,160,19,35,5,176,225,
2,11,128,225,34,21,160,225,12,26,129,224,100,64,176,225,
112,64,189,8,30,255,47,1,132,64,176,225,0,0,176,226,
0,16,161,226,4,0,20,225,1,0,192,3,112,64,189,232,
30,255,47,225,64,232,126,226,0,0,176,226,0,16,161,226,
112,64,189,232,30,255,47,225,1,198,144,225,112,64,189,8,
30,255,47,1,1,0,17,225,38,0,0,74,1,192,160,227,
32,230,145,225,0,10,160,1,20,192,76,2,1,0,17,225,
32,22,160,1,0,10,160,1,20,192,76,2,0,64,160,227,
161,226,176,225,1,24,160,1,16,64,132,2,161,230,176,225,
1,20,160,1,8,64,132,2,161,232,176,225,1,18,160,1,
4,64,132,2,161,233,176,225,1,17,160,1,2,64,132,2,
33,234,176,225,129,16,160,1,1,64,132,2,4,192,76,224,
16,84,160,225,32,64,100,226,48,20,129,225,5,0,160,225,
155,255,255,234,112,64,189,232,1,198,144,225,32,192,159,229,
158,252,255,26,255,255,255,234,1,0,17,225,30,255,47,81,
12,192,159,229,146,0,0,234,112,64,189,232,0,192,159,229,
143,0,0,234,23,0,0,4,120,71,0,0,3,0,45,233,
12,80,45,233,7,241,255,235,12,80,189,232,0,48,160,225,
3,0,189,232,0,48,147,229,64,7,19,227,128,20,1,18,
0,0,160,19,30,255,47,17,128,14,19,227,125,0,0,26,
1,0,17,225,64,203,140,67,96,62,160,227,33,42,160,225,
1,48,131,226,2,26,193,225,128,46,194,227,2,32,67,224,
64,25,129,227,53,0,82,227,34,0,0,138,27,0,0,10,
21,0,82,227,16,0,0,58,32,32,82,226,6,0,0,154,
32,48,98,226,0,0,80,227,49,2,160,225,17,35,160,225,
1,32,130,19,0,16,160,227,25,0,0,234,0,48,98,226,
32,32,130,226,17,19,160,225,48,18,129,225,16,35,160,225,
1,0,160,225,0,16,160,227,17,0,0,234,16,0,45,233,
32,48,98,226,16,67,160,225,17,51,160,225,48,2,131,225,
49,18,160,225,4,32,160,225,16,0,189,232,8,0,0,234,
1,38,144,225,0,0,160,227,0,16,160,227,128,36,160,227,
1,32,130,18,2,0,0,234,1,32,160,227,0,16,160,227,
0,0,160,227,64,11,28,227,64,203,204,227,128,20,129,19,
2,0,18,225,30,255,47,1,130,32,176,225,2,0,0,58,
11,0,0,10,1,0,144,226,0,16,161,226,3,0,45,233,
12,80,45,233,191,240,255,235,12,80,189,232,0,32,160,225,
3,0,189,232,0,48,146,229,8,48,131,227,0,48,130,229,
30,255,47,225,76,47,176,225,241,255,255,74,242,255,255,26,
1,0,144,226,0,16,161,226,1,0,192,227,238,255,255,234,
120,71,0,0,3,0,45,233,12,80,45,233,173,240,255,235,
12,80,189,232,0,48,160,225,3,0,189,232,0,48,147,229,
64,7,19,227,128,20,1,18,0,0,160,19,30,255,47,17,
128,14,19,227,30,255,47,1,0,64,45,233,96,230,160,227,
161,239,142,225,128,20,193,227,32,54,145,225,0,10,160,1,
80,233,78,2,1,0,17,225,32,22,160,1,0,10,160,1,
80,233,78,2,0,32,160,227,161,50,176,225,1,24,160,1,
16,32,130,2,161,54,176,225,1,20,160,1,8,32,130,2,
161,56,176,225,1,18,160,1,4,32,130,2,161,57,176,225,
1,17,160,1,2,32,130,2,33,58,176,225,129,16,160,1,
1,32,130,2,2,232,78,224,16,50,160,225,32,32,98,226,
48,18,129,225,3,0,160,225,142,31,33,224,1,224,206,227,
14,18,129,224,0,64,189,232,128,197,140,227,34,2,0,234,
120,71,0,0,16,0,28,227,1,32,160,1,15,0,45,233,
3,0,45,233,12,80,45,233,118,240,255,235,12,80,189,232,
0,32,160,225,3,0,189,232,0,16,146,229,1,57,204,225,
192,53,19,226,64,196,140,19,140,48,160,225,163,61,160,225,
3,16,129,225,112,55,1,226,3,192,140,225,0,16,130,229,
15,48,12,226,8,0,51,227,112,48,12,18,131,193,140,17,
128,59,160,3,12,48,211,1,192,200,140,3,128,5,28,227,
13,0,0,26,64,5,28,227,7,0,0,26,64,4,28,227,
20,0,0,26,128,6,28,227,11,0,0,26,64,15,17,227,
15,0,189,232,32,0,0,10,251,1,0,234,64,14,17,227,
15,0,189,232,17,0,0,10,247,1,0,234,128,14,17,227,
15,0,189,232,11,0,0,10,243,1,0,234,128,15,17,227,
15,0,189,232,240,1,0,26,16,0,28,227,3,32,33,16,
2,32,32,0,8,0,0,234,64,13,17,227,15,0,189,232,
0,0,0,10,232,1,0,234,16,0,28,227,30,255,47,225,
128,0,28,227,128,36,1,18,128,36,0,2,128,36,2,226,
16,0,28,227,56,0,143,18,3,0,144,24,2,16,129,17,
60,0,159,5,2,0,128,1,30,255,47,225,128,0,28,227,
128,36,1,18,128,36,0,2,16,0,28,227,40,0,143,18,
3,0,144,24,2,16,129,17,36,0,159,5,2,0,128,1,
170,1,0,234,0,0,0,0,0,0,240,127,255,255,255,255,
255,255,239,127,0,0,128,127,255,255,127,127,0,0,0,0,
0,0,248,127,0,0,192,127,120,71,0,0,128,200,128,226,
127,4,28,227,5,0,0,10,192,17,176,225,128,14,160,225,
224,21,129,226,30,255,47,81,112,20,65,226,30,255,47,225,
128,8,28,227,6,0,0,10,0,16,160,225,128,0,176,225,
30,255,47,1,128,36,1,226,218,53,130,226,129,17,160,225,
130,253,255,234,128,36,176,225,16,192,159,229,15,0,0,26,
255,255,255,234,112,24,128,227,2,0,160,225,30,255,47,225,
24,0,0,4,120,71,0,0,128,56,160,227,128,32,131,224,
129,48,131,224,128,8,114,227,128,8,115,147,132,255,255,138,
128,8,82,227,1,0,160,33,124,1,0,234,120,71,0,0,
64,8,16,227,126,255,255,10,79,32,12,226,72,0,82,227,
118,1,0,26,122,255,255,234,129,130,131,132,133,134,135,136,
137,139,140,141,142,143,145,146,147,149,150,151,153,154,156,157,
159,160,162,163,165,167,168,170,172,174,176,178,179,181,183,185,
188,190,192,194,197,199,201,204,206,209,212,215,217,220,223,226,
230,233,236,240,243,247,250,254,120,71,0,0,1,0,32,224,
0,16,33,224,1,0,32,224,0,0,0,234,120,71,0,0,
255,200,160,227,160,35,28,224,161,51,28,16,12,0,50,17,
12,0,51,17,71,0,0,10,1,0,48,225,64,47,130,67,
128,200,128,227,128,8,129,227,255,20,204,227,255,4,192,227,
0,64,45,233,3,32,66,224,0,0,81,225,29,192,79,226,
160,232,92,231,0,0,96,226,142,224,160,225,144,14,12,224,
129,16,160,49,76,194,160,225,156,14,3,224,142,227,160,225,
195,234,142,224,33,196,160,225,158,12,12,224,129,21,160,225,
44,58,160,225,144,19,33,224,125,40,130,226,33,196,160,225,
158,12,12,224,1,22,160,225,172,201,160,225,144,28,33,224,
64,239,2,226,66,40,162,224,0,0,113,225,0,16,129,32,
3,54,172,224,1,0,81,227,144,227,142,35,129,0,112,225,
80,227,142,35,4,224,142,3,8,16,142,227,0,64,189,232,
130,11,163,224,252,8,82,19,30,255,47,49,128,31,129,225,
1,0,192,3,252,8,82,227,30,255,47,49,12,0,0,90,
128,192,160,225,64,199,76,226,192,7,92,227,30,255,47,49,
96,4,128,226,4,0,17,227,1,32,240,17,225,32,176,1,
68,34,160,67,76,34,160,83,64,4,17,227,4,32,160,3,
189,0,0,234,128,192,160,225,64,199,140,226,254,4,92,227,
30,255,47,33,96,4,64,226,65,194,160,227,64,20,1,226,
1,192,140,225,22,255,255,234,161,51,12,224,1,0,48,225,
64,47,130,67,12,0,82,225,12,0,83,49,10,0,0,42,
129,192,176,225,25,0,0,10,128,192,176,225,130,11,160,1,
30,255,47,1,0,192,143,226,129,0,0,234,32,196,160,225,
33,4,160,225,12,16,160,225,172,255,255,234,64,199,160,227,
128,0,124,225,129,0,124,145,64,192,159,229,117,255,255,138,
255,255,255,234,64,199,160,227,128,0,124,225,129,0,124,1,
130,11,160,1,36,192,159,5,249,254,255,10,129,0,124,225,
0,0,160,3,128,4,192,19,130,11,128,225,30,255,47,225,
128,192,176,225,4,192,159,5,4,192,159,21,240,254,255,234,
4,0,0,4,4,0,0,8,120,71,0,0,64,36,160,227,
3,0,0,234,120,71,0,0,128,36,16,226,0,0,96,18,
64,36,130,227,32,24,176,225,128,38,130,18,0,8,176,1,
30,255,47,1,255,4,16,227,64,38,130,18,0,4,160,1,
240,4,16,227,128,39,130,18,0,2,160,1,192,4,16,227,
64,39,130,18,0,1,176,1,128,40,130,66,128,0,160,81,
128,28,176,225,64,4,162,224,4,193,176,19,1,0,192,35,
30,255,47,225,120,71,0,0,255,200,160,227,160,35,28,224,
161,51,28,16,12,0,50,17,12,0,51,17,42,0,0,10,
1,0,48,225,64,47,130,67,128,196,160,227,0,4,140,225,
1,20,140,225,3,32,130,224,144,193,131,224,128,40,66,226,
0,0,92,227,1,48,131,19,64,207,2,226,128,4,83,227,
131,48,160,49,66,40,162,224,35,4,176,225,130,11,160,224,
128,192,173,224,127,0,19,35,252,8,82,19,30,255,47,49,
1,0,192,3,252,8,82,227,30,255,47,49,12,0,0,90,
128,16,160,225,64,23,65,226,192,7,81,227,30,255,47,49,
96,4,128,226,127,0,19,227,236,192,176,17,108,193,240,1,
208,35,160,67,240,35,160,83,255,0,19,227,3,32,160,3,
69,0,0,234,128,200,128,226,140,192,176,225,30,255,47,65,
96,4,64,226,196,195,160,227,161,254,255,234,161,51,12,224,
1,0,48,225,64,47,130,67,12,0,82,225,12,0,83,49,
5,0,0,42,128,192,176,225,129,192,176,17,130,11,160,1,
30,255,47,1,200,192,79,226,13,0,0,234,64,199,160,227,
128,0,124,225,129,0,124,145,193,195,160,227,5,255,255,138,
255,255,255,234,128,192,176,225,129,192,176,17,255,32,130,19,
130,11,160,17,30,255,47,17,193,195,160,227,136,254,255,234,
120,71,0,0,0,4,160,225,1,20,160,225,255,8,18,227,
16,0,0,26,64,11,80,227,64,41,66,50,0,8,160,49,
255,4,16,227,128,42,66,2,0,4,160,1,240,4,16,227,
64,42,66,2,0,2,160,1,192,4,16,227,128,43,66,2,
0,1,176,1,128,0,160,81,64,43,130,66,255,8,19,227,
128,20,129,19,12,240,160,17,128,4,128,227,64,11,81,227,
64,57,67,50,1,24,160,49,255,4,17,227,128,58,67,2,
1,20,160,1,240,4,17,227,64,58,67,2,1,18,160,1,
192,4,17,227,128,59,67,2,1,17,176,1,129,16,160,81,
64,59,131,66,12,240,160,225,120,71,0,0,3,0,45,233,
12,80,45,233,219,238,255,235,12,80,189,232,0,192,160,225,
3,0,189,232,0,16,156,229,64,7,17,227,128,4,0,18,
30,255,47,17,128,14,17,227,77,0,0,26,128,52,0,226,
160,59,131,225,64,63,195,227,193,48,99,226,128,20,195,227,
23,0,81,227,25,0,0,202,32,48,99,226,16,19,176,225,
32,48,99,226,255,4,192,227,128,8,128,227,48,3,160,225,
128,52,3,226,3,0,128,225,12,0,0,10,1,48,160,225,
0,16,156,229,8,16,129,227,0,16,140,229,131,16,176,225,
4,0,0,58,1,0,128,18,2,0,0,26,194,63,240,225,
1,0,128,146,1,0,192,51,64,196,130,227,56,254,255,234,
2,192,160,225,64,4,18,227,53,254,255,26,30,255,47,225,
102,1,81,227,0,16,156,229,8,16,129,227,0,16,140,229,
128,4,3,98,4,0,0,106,128,4,176,225,128,4,3,226,
1,0,128,226,128,4,18,3,1,0,192,3,64,196,160,227,
39,254,255,234,120,71,0,0,3,0,45,233,12,80,45,233,
160,238,255,235,12,80,189,232,0,192,160,225,3,0,189,232,
0,16,156,229,64,7,17,227,128,4,0,18,30,255,47,17,
128,14,17,227,30,255,47,1,128,20,0,226,96,20,129,227,
255,8,16,227,0,4,160,1,64,22,65,2,255,8,16,227,
0,4,160,1,64,22,65,2,240,8,16,227,0,2,160,1,
128,23,65,2,192,8,16,227,0,1,160,1,64,23,65,2,
128,8,16,227,128,0,160,1,128,24,129,18,128,8,192,227,
1,0,128,225,128,197,130,227,39,0,0,234,120,71,0,0,
15,48,12,226,9,0,83,227,8,0,160,3,44,0,0,10,
8,0,83,227,30,255,47,17,64,0,28,3,22,0,0,10,
64,11,28,227,11,0,0,26,32,0,28,227,12,0,0,26,
16,0,28,227,0,32,160,1,1,32,160,17,128,4,224,3,
128,20,224,19,0,0,224,19,128,4,18,227,0,0,224,17,
1,16,224,17,30,255,47,225,0,0,160,227,0,16,160,227,
30,255,47,225,0,0,224,227,0,16,224,227,128,4,18,227,
0,0,224,17,1,16,224,17,30,255,47,225,16,0,28,227,
8,0,159,5,8,16,159,21,0,0,160,19,30,255,47,225,
0,0,192,127,0,0,248,127,120,71,0,0,0,80,45,233,
0,80,45,233,173,239,255,235,0,80,189,232,0,80,189,232,
15,48,12,226,9,0,51,227,30,255,47,17,0,0,0,234,
120,71,0,0,64,9,28,227,26,0,0,10,240,10,28,227,
6,0,0,10,8,0,16,227,1,0,0,26,2,0,112,226,
30,255,47,225,240,26,12,226,64,10,81,227,30,255,47,225,
224,20,160,227,128,30,17,224,192,21,129,35,3,0,45,233,
12,80,45,233,63,238,255,235,12,80,189,232,0,192,160,225,
3,0,189,232,0,48,156,229,240,52,195,227,1,48,131,225,
0,48,140,229,0,32,15,225,240,36,194,227,1,32,130,225,
2,240,40,225,30,255,47,225,0,8,28,224,1,0,160,19,
30,255,47,225,48,0,49,0,50,0,51,0,52,0,53,0,
54,0,55,0,56,0,57,0,65,0,66,0,67,0,68,0,
69,0,70,0,71,0,72,0,73,0,74,0,75,0,76,0,
77,0,78,0,79,0,80,0,81,0,82,0,83,0,84,0,
85,0,86,0,87,0,88,0,89,0,90,0,0,0,0,0,
48,0,49,0,50,0,51,0,52,0,53,0,54,0,55,0,
56,0,57,0,97,0,98,0,99,0,100,0,101,0,102,0,
103,0,104,0,105,0,106,0,107,0,108,0,109,0,110,0,
111,0,112,0,113,0,114,0,115,0,116,0,117,0,118,0,
119,0,120,0,121,0,122,0,0,0,0,0,60,78,85,76,
76,62,0,0,60,0,78,0,85,0,76,0,76,0,62,0,
0,0,0,0,78,0,97,0,110,0,0,0,45,0,73,0,
110,0,102,0,0,0,0,0,43,0,73,0,110,0,102,0,
0,0,0,0,48,49,50,51,52,53,54,55,56,57,65,66,
67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,
83,84,85,86,87,88,89,90,0,0,0,0,48,49,50,51,
52,53,54,55,56,57,97,98,99,100,101,102,103,104,105,106,
107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,
0,0,0,0,78,97,110,0,45,73,110,102,0,0,0,0,
43,73,110,102,0,0,0,0,8,8,8,8,8,8,8,8,
8,40,40,40,40,40,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,160,16,16,16,16,16,16,16,
16,16,16,16,16,16,16,16,4,4,4,4,4,4,4,4,
4,4,16,16,16,16,16,16,16,65,65,65,65,65,65,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,16,16,16,16,16,16,66,66,66,66,66,66,2,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,16,16,16,16,8,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,91,87,82,69,93,91,65,80,
80,93,37,115,0,0,0,0,0,0,0,0,0,0,240,63,
0,0,0,0,0,0,248,63,0,0,0,0,0,0,0,0,
0,0,0,64,3,184,226,63,0,0,0,0,0,0,0,0,
6,208,207,67,235,253,76,62,3,51,51,51,51,51,227,63,
255,171,111,219,182,109,219,63,77,38,143,81,85,85,213,63,
1,65,29,169,96,116,209,63,101,219,201,147,74,134,205,63,
239,78,69,74,40,126,202,63,62,85,85,85,85,85,197,63,
147,189,190,22,108,193,102,191,44,222,37,175,106,86,17,63,
241,107,210,197,65,189,187,190,208,164,190,114,105,55,102,62,
0,0,0,0,0,0,0,0,65,98,110,111,114,109,97,108,
32,116,101,114,109,105,110,97,116,105,111,110,0,0,0,65,
114,105,116,104,109,101,116,105,99,32,101,120,99,101,112,116,
105,111,110,58,32,0,73,108,108,101,103,97,108,32,105,110,
115,116,114,117,99,116,105,111,110,0,0,0,0,73,110,116,
101,114,114,117,112,116,32,114,101,99,101,105,118,101,100,0,
0,0,0,0,73,108,108,101,103,97,108,32,97,100,100,114,
101,115,115,0,0,0,0,0,0,0,0,84,101,114,109,105,
110,97,116,105,111,110,32,114,101,113,117,101,115,116,0,0,
0,0,83,116,97,99,107,32,111,118,101,114,102,108,111,119,
0,0,0,0,0,0,0,0,0,82,101,100,105,114,101,99,
116,58,32,99,97,110,39,116,32,111,112,101,110,58,32,0,
79,117,116,32,111,102,32,104,101,97,112,32,109,101,109,111,
114,121,0,0,0,0,0,85,115,101,114,45,100,101,102,105,
110,101,100,32,115,105,103,110,97,108,32,49,0,0,85,115,
101,114,45,100,101,102,105,110,101,100,32,115,105,103,110,97,
108,32,50,0,0,80,117,114,101,32,118,105,114,116,117,97,
108,32,102,110,32,99,97,108,108,101,100,0,67,43,43,32,
108,105,98,114,97,114,121,32,101,120,99,101,112,116,105,111,
110,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,59};
char* g_adsvahelper_ram = NULL;
typedef int (*MAIN_ENTRY)(int );
extern void _wre_adsva_helper_init(int* a_sprintf,int* a_vsprintf,int* a__vsnprintf,int* a_sscanf,int* f5,int* f6,int* f7,int* f8,int* f9);
#endif
#endif
void mytrace(char* str){
	SCI_TRACE_LOW("[WRE][ADSVA]%s",str);
}

void OpenADSVAHelper()
{
#ifdef WIN32
   // return (int *)GetProcAddress(hHandle, lpProcName);
#else

#ifdef WRE_WREKERNEL_NORFLASH

	MAIN_ENTRY fp = 0;
	g_adsvahelper_ram=(char*)SCI_ALLOC(ADSVA_SIZE);
	if(NULL  == g_adsvahelper_ram){
		return;	
	}
	memcpy(g_adsvahelper_ram,(char *)g_adsvahlper_bin,ADSVA_SIZE);	// changed by yifei
    fp = (MAIN_ENTRY)  (int *)YSLFWKXMYW((unsigned char*)g_adsvahelper_ram);    /*lint !e611*/
	//SCI_TRACE_LOW("fpfp:[%x][%x]",fp(1),fp(4));
	fp((int)mytrace);	 /*lint !e64 */
	_wre_adsva_helper_init(fp(1),fp(2),fp(3),fp(4),fp(5),fp(6),fp(7),fp(8),fp(9));  /*lint !e64*/
#endif
	
    
#endif
}
 void CloseADSVAHelper()
{
#ifndef WIN32
#ifdef WRE_WREKERNEL_NORFLASH
	if(NULL  == g_adsvahelper_ram){
		return;	
	}
	SCI_FREE(g_adsvahelper_ram);
	g_adsvahelper_ram = NULL;
	#endif
#endif
}

//////////////////////////////////////////////////////////////////////////
static LCD_ANGLE_E  lcd_angle=LCD_ANGLE_0;
    
	
	
	LCD_ANGLE_E get_lcd_angle(void)
	{
		return lcd_angle;
	}
	
	void set_lcd_angle(LCD_ANGLE_E angle)
	{
		lcd_angle = angle;
}
//extern void Wre_InitRtcEvent(void);
int wre_startup(PWRE_APP_PARAM_T pAppParam)
{
    wchar                   btempDir[256];
    PWRE_COMM               pWre_Comm = NULL;
    int                     ret = WRE_INIT_OK;

	SCI_TRACE_LOW("LOADER kernel loader ready: %d", SCI_GetTickCount());
	 lcd_angle=LCD_ANGLE_0;
#if 0
#if defined(MMI_PDA_SUPPORT)
	{
        GUI_LCD_DEV_INFO lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

		lcd_angle=LCD_ANGLE_0;
        if(MMK_GetScreenAngle() != LCD_ANGLE_0)
        {
            lcd_angle = MMK_GetScreenAngle();
           
           
            
			MMK_SetScreenAngle(LCD_ANGLE_0);
			GUI_FillRect(&lcd_dev_info, MMITHEME_GetFullScreenRect(), MMI_BLACK_COLOR);
			//MMITHEME_UpdateRect();	
			 
			
        }
    }
#endif

#endif

    if(s_pWre_KernelProc)
    {
        //Copy command line param to kernel.
        WRE_ConstructCmdPara(pAppParam,&pWre_Comm);

        //
        	{
        	
        		int cret= CreateMwinDeskWin();
			if(1 == cret){
				SCI_TRACE_LOW("[WRE]: CreateMwinDeskWin  error\n");
				#ifdef WRE_WREKERNEL_NORFLASH
            CloseADSVAHelper();
#endif
				WreMwinWin_HandleMsg(MMIMWIN_WIN_ID,  (WRE_KEY_RELEASED & 0xff00) | (WRE_KEY_RED & 0xff), 0);
				return;
			}

		}
        WreMwinWin_HandleMsg(MMIMWIN_WIN_ID, MSG_GET_FOCUS, 0);

        //SCI_TRACE_LOW:"[WRE]: startup a application\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_566_112_2_18_3_11_1_221,(uint8*)"");
        pWre_Comm->action.action=KA_NORMAL;
        pWre_Comm->action.param=NULL;

        ret = s_pWre_KernelProc(pWre_Comm);

        return  ret;
    }

#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
    wretomocorapp.AppType = WRETOMORCOR_NONE;
    WreToMocorApp_Realse();
#endif
#if (!defined(WIN32))&&defined(WRE_VIRTUALDISK_SUPPORT)&&(defined(WRE_WREKERNEL_PRELOAD)||defined(WRE_WREKERNEL_NORFLASH))
	if (pAppParam->bFixApp)
    {
        MMIAPICOM_Wstrcpy(btempDir, WRE_ROM_DISK_NAME);
        MMIAPICOM_Wstrcat(btempDir, L":\\wre");

        WRE_TraceWstr(L"[WRE]wre_startup: fixapp load kernel");
        WRE_TraceWstr(btempDir);
        g_wre_kernel_norflash_startup = 1;
    }
    else
#endif
    {
        MMIAPICOM_Wstrcpy(btempDir, WRE_Get_SYS_Dir(WRE_SYS_PATH_KERNEL));   
    }    
    MMIAPICOM_Wstrcat(btempDir, L"\\");
    MMIAPICOM_Wstrcat(btempDir,WRE_SYS_FILE_NAME_KERNELL);
#ifdef WIN32
	g_wre_kernel_norflash_startup =1;
#endif
	if (1 != g_wre_kernel_norflash_startup)
	{
		hModule         = LoadWreLib(btempDir);

		if (0 == hModule)
		{
			//SCI_TRACE_LOW:"error:WRE_INIT_LOADKERNEL_FALI \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_595_112_2_18_3_11_1_222,(uint8*)"");
			return WRE_INIT_LOADKERNEL_FALI;
		}
	}

    /*
    CreateMwinDeskWin();
    */
	if (1 == g_wre_kernel_norflash_startup)
	{
		extern int WRE_Kernel_Startup(PWRE_COMM  pWre_Comm);
		#ifdef WRE_WREKERNEL_NORFLASH
			s_pWre_KernelProc = WRE_Kernel_Startup;
		#else
			SCI_TRACE_LOW("WRE NORFLASH config error!!!");
		#endif
	}
	else
	{
		s_pWre_KernelProc = (WRE_KERNEL_STARTUP_FUNC)GetWreProcAddress( hModule, "WRE_Kernel_Startup");/*lint !e611 */
	}

    WRE_ConstructCmdPara(pAppParam,&pWre_Comm);
	
    GetAdaptorCommPara(&pWre_Comm, pAppParam);

    pWre_Comm->action.param=GetDownLoadParam();

    {
        if(IsDownLoadFlag() == 1)
        {
            //SCI_TRACE_LOW:" wre KA_DOWNLOAD_WRE \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_614_112_2_18_3_11_1_223,(uint8*)"");
            pWre_Comm->action.action=KA_DOWNLOAD_WRE;            
        }
        else
        {
            //SCI_TRACE_LOW:" wre KA_NORMAL \n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_619_112_2_18_3_11_1_224,(uint8*)"");
            pWre_Comm->action.action=KA_NORMAL;
        }
    }
    
    if(0xeaebeced == (unsigned int)pAppParam->argc){
        pWre_Comm->action.action=KA_QUERY_VERSION;
            pWre_Comm->action.param=pAppParam->cmdline;
    }
	if(0xeaebece1 == (unsigned int)pAppParam->argc){
        pWre_Comm->action.action=KA_VERIFY_WRE;
            pWre_Comm->action.param=pAppParam->cmdline;
    }
	if(0xeaebece2 == (unsigned int)pAppParam->argc){
        pWre_Comm->action.action=KA_GET_PART2_URL;
		pWre_Comm->action.param=pAppParam->cmdline;
    }
		SCI_TRACE_LOW("LOADER kernel end LoadWreLib 2: %d", SCI_GetTickCount());
    if(s_pWre_KernelProc)
    {
        #ifndef WIN32
        /*
        MMU_InvalideDCACHE();
        MMU_PHY_InvalideICACHEALL();
        */
        /* 使用新的cache函数处理 */
        MMU_IcacheSyncAll();
        MMU_DcacheSyncAll();
		
		if (0 == g_wre_kernel_norflash_startup){
      #ifdef WRE_ASSERT_TRACING
        {
           //extern void WreDelDll(char* ap_name_ptr,uint32 uID,uint32 ro_start,uint32 ro_end,uint32 rw_start,uint32 rw_end);
            #define	WRE_CHECK_NODE_ID_NONE		0xFFFFFFFF
            
            //SCI_TRACE_LOW:"====== Add wrekernel into check node: %s =======\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_638_112_2_18_3_11_1_225,(uint8*)"s", btempDir);
            wre_kernel_filesize -= (uint32)((uint32)s_pWre_KernelProc - (uint32)hModule);
            //SCI_TRACE_LOW:"====== wrekernel upload addr: 0x%08x, kernel size: %d =======\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_640_112_2_18_3_11_1_226,(uint8*)"dd", (uint32)hModule, wre_kernel_filesize);
            WreAddDll("wrekernel.wrd", (uint32)WRE_CHECK_NODE_ID_NONE, (uint32)s_pWre_KernelProc-1, (uint32)((uint32)s_pWre_KernelProc-1+(uint32)wre_kernel_filesize), (uint32)s_pWre_KernelProc-1, (uint32)((uint32)s_pWre_KernelProc-1+(uint32)wre_kernel_filesize));
            wre_kernel_filesize = 0;
        }
      #endif
		}
        #endif
		
        ret = s_pWre_KernelProc(pWre_Comm);

        if(ret == WRE_INIT_NO_ROOM)
        {
        	//SCI_TRACE_LOW:"error:WRE_INIT_NO_ROOM \n"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_650_112_2_18_3_11_1_227,(uint8*)"");
            FreeWreLib(hModule);
			s_pWre_KernelProc = NULL;
        }
		if(0xeaebece1 == (unsigned int)pAppParam->argc){
            FreeWreLib(hModule);
            s_pWre_KernelProc = NULL;
			return ret;
        }
    }
    if((ret == WRE_INIT_OK) || (ret == 0x7788))
    {
        int cret =CreateMwinDeskWin();
	if(1 == cret){
				SCI_TRACE_LOW("[WRE]: CreateMwinDeskWin  failed\n");
				#ifdef WRE_WREKERNEL_NORFLASH
            CloseADSVAHelper();
#endif
			 FreeWreLib(hModule);
			s_pWre_KernelProc = NULL;
				return WRE_INIT_NO_ROOM;
			}
    }

    //SCI_TRACE_LOW:" load wk ok \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_660_112_2_18_3_11_1_228,(uint8*)"");
    return ret;
	//#endif
}

extern WRE_APP_PARAM_T wre_app_param;
extern void freewrefunctbl(void);
extern void wre_free_array_audioptr(void);
LOCAL MMI_RESULT_E HandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E    recode  = MMI_RESULT_TRUE;
    int             ret     = WRE_INIT_OK;

    /*wre application be minimize,stop do any thing*/
    if(WRE_GetRuningStatus() == STATUS_WRE_MINI)
    {
        if(msg_id != MSG_OPEN_WINDOW)
        {
            if(msg_id == MSG_CLOSE_WINDOW)
            {
                /* exit WRE, permit the auto lock screen, by wei.zhou */
                MMIDEFAULT_AllowOpenKeylocWin(TRUE);
            }

            return MMI_RESULT_FALSE;
        }
    }
    recode = WreMwinWin_HandleMsg(win_id, msg_id, param);

    if(msg_id == MSG_USER_START)
    {
        FreeWreLib(hModule);
        WRE_UpdateWreRuningFlag();
        freewrefunctbl();
		wre_free_array_audioptr();
#ifdef WRE_VIRTUALDISK_SUPPORT		
        WRE_DestoryVirtualDisk();
#endif
		WREPDP_ResetPdpProc();
        MMK_CloseApplet(SPRD_WRE_APPLET_ID);
        /* revival the ui layer room */
        #if 0
        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
        #endif
        /* memory fragment revial, centralize processing, by wei.zhou */
        MMIWRE_GarbageRevial();
		#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_WRE);
        #endif	

#ifdef BLUETOOTH_SUPPORT
        WRE_RestoreBTState();
#endif
    
        if (hot_reset_tag == 1)
        {
            //SCI_TRACE_LOW:"WRE boot_reboot HandleMsg 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_691_112_2_18_3_11_1_229,(uint8*)"");
            ret     = WREEntry(0,0);
            //hot_reset_tag = 0;
            //SCI_TRACE_LOW:"WRE boot_reboot HandleMsg 1 ret=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_694_112_2_18_3_11_1_230,(uint8*)"d", ret);
        }
    #ifdef MMIWRE_MEM_CONFLICT_SOLUTION
        else
        {
           
            if(wretomocorapp.AppType == WRETOMORCOR_VIDEOPLAYER)
            {
                PWRETOMOCOR_VIDEO_PARAM lpParam = (PWRETOMOCOR_VIDEO_PARAM)wretomocorapp.lpParam;
                SCI_TRACE_LOW("[WRE] Adapt_EntryVideoPlayer and close wre");
                Adapt_EntryVideoPlayer(lpParam->VideoType, lpParam->strFileName, lpParam->nFileNameLen, lpParam->strUrl);
            }
            else if(wretomocorapp.AppType == WRETOMORCOR_BROWSER)
            {
                SCI_TRACE_LOW("[WRE] Adapt_EntryBrowser and close wre");
                Adapt_EntryBrowser(0, (char *)wretomocorapp.lpParam, 0);
            }
            else if(wretomocorapp.AppType == WRETOMORCOR_PHOTOVIEWER)
            {
                SCI_TRACE_LOW("[WRE] Adapt_EntryPhotoView and close wre");
                Adapt_EntryPhotoView((wchar *)wretomocorapp.lpParam, 0);
            }
        }
    #endif
    }

    /* kernel no room process flowchart.  */
    if((WRE_GetRuningStatus() == STATUS_WRE_NOROOM) && ((MSG_CLOSE_WINDOW == msg_id || MSG_KEYUP_SDCARD_DETECT == msg_id)))
    {
#ifdef SNS_SUPPORT
        if(wre_app_param.cmdline != NULL)
        {
            /* notify sns to realse memory */
            if(MMIAPICOM_Wstrncmp((wchar *)wre_app_param.cmdline, L"SNS", 3) == 0)
            {
                MMISNS_SendEvtToApp(SNS_MODULE_NOE, 0, 0, 0);
            }
        }
#endif
        MMIWRE_WndDestroy();//free some resource when window is closed.
        /* fixed the room leak bug, kernel heap allocate no enough, by wei.zhou */
        FreeWreLib(hModule);
        freewrefunctbl();
        wre_free_array_audioptr();
        /* abort revival, lock screen. */
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        /* revival the ui layer room */
        #if 0
        UILAYER_SetMainLayerMemFormat(UILAYER_MEM_DOUBLE_COPY);
        #endif
        /* memory fragment revial, centralize processing, by wei.zhou */
        MMIWRE_GarbageRevial();

#ifdef WRE_VIRTUALDISK_SUPPORT		
        WRE_DestoryVirtualDisk();
#endif
		WREPDP_ResetPdpProc();
        MMK_CloseApplet(SPRD_WRE_APPLET_ID);

#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_WRE);
#endif
#ifdef MOTION_SENSOR_TYPE
//SCI_TRACE_LOW:"[WRE]on sensorautortation:g_old_sensor_open_flag=%d"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_705_112_2_18_3_11_1_231,(uint8*)"d",g_old_sensor_open_flag);
    if (g_old_sensor_open_flag != 0)
	{
        WRE_RestoreSensorAutorotation(g_old_sensor_open_flag);  
		g_old_sensor_open_flag=0;
		g_wre_sensor_flag =FALSE;
	}   
#endif
    }
 
    return recode;
}

#ifdef MOTION_SENSOR_TYPE

 void WRE_CloseSensorAutorotation(uint32 *old_sensor_open_flag_ptr)
{
#if 0
    uint32 sensor_open_flag;

    sensor_open_flag = MMISET_GetIsSensorOpenFlag();

    *old_sensor_open_flag_ptr = sensor_open_flag;

    sensor_open_flag&=0x7;
    MMISET_SetIsSensorOpenFlag(sensor_open_flag);
    MMISET_ReturnToAngleSupport(sensor_open_flag);
#endif
}
 
 void WRE_RestoreSensorAutorotation(uint32 old_sensor_open_flag)
{
    /* 在进入WRE应用后，调用MOCOR其它操作导致MOCOR窗口异常 */
#if 0
    uint32 sensor_open_flag;

    sensor_open_flag = MMISET_GetIsSensorOpenFlag();

    old_sensor_open_flag&=0x00000038;

    sensor_open_flag&=0xFFFFFFC7;
    sensor_open_flag|=old_sensor_open_flag;

    MMISET_SetIsSensorOpenFlag(sensor_open_flag);
    MMISET_ReturnToAngleSupport(sensor_open_flag);
#endif
}
#endif
 
#ifdef BLUETOOTH_SUPPORT
BOOLEAN WRE_GetOldBTState()
{
    return g_wre_bt_flag;
}

void WRE_SetOldBTState(BOOLEAN bBTFlag)
{
    g_wre_bt_flag = bBTFlag;
}

void WRE_RestoreBTState()
{
    BOOLEAN bBTFlag = WRE_GetOldBTState();
    SCI_TRACE_LOW("[WRE]WRE_RestoreBTState: bBTFlag = %d", bBTFlag);
    if (bBTFlag)
    {
        MMIAPIBT_OpenBlueTooth(TRUE);
    }
}
#endif

static uint32 wre_runing_flags = 0;
/*****************************************************************************/
//  Description : WRE_UpdateWreRuningFlag 
//  Global resource dependence : 
//  Author:yiwen.man
//  Note: it means wre running times.
/*****************************************************************************/
void   WRE_UpdateWreRuningFlag(void)
{
    wre_runing_flags ++;
}

/*****************************************************************************/
//  Description : WRE_GetWreRuningFlag 
//  Global resource dependence : 
//  Author:yiwen.man
//  Note: get current wre running flags.
/*****************************************************************************/
uint32  WRE_GetWreRuningFlag(void)
{
    return wre_runing_flags;
}
#if 0
PUBLIC void *DSystem_Load(uint16 *lpLibFileName)
{
#ifdef WIN32
    wchar_t module[MAX_PATH]={0};
    wchar_t path[MAX_PATH]={0};
    wchar_t *p1=0;
    wchar_t *p2=0;
    GetModuleFileNameW(0,module,MAX_PATH);
    p1= wcsstr(module,L"\\MSDev.exe");
    *p1=0;
    wcscpy(path,module);
    wcscat(path, L"\\");

    p1 = lpLibFileName;
    p2 = p1;
    p1= wcsstr(p1,L"\\");
    while(p1 != NULL)
    {
        p2 = ++p1;
        p1= wcsstr(p1,L"\\");
    }

    wcscat(path,p2);

    return LoadLibraryW(path);
#else
    SFS_HANDLE  wre_kernel_handle = NULL;
    SFS_ERROR_E result = SFS_NO_ERROR;
    uint32      filesize = 0;
    uint32      readsize = 0;
    void        *pKernelProc = NULL;
    char        *pFileBuf    = NULL;

    /* malloc memory */
    wre_kernel_handle = SFS_CreateFile(lpLibFileName, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, 0, 0);/*lint !e655 */
    if(0 == wre_kernel_handle ){
            //SCI_TRACE_LOW:"There is no dpaper file!\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_809_112_2_18_3_11_1_232,(uint8*)"");
        return 0;
    }
    result = SFS_GetFileSize(wre_kernel_handle, &filesize);
        //SCI_TRACE_LOW:"dpaper file size:%d!!!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_813_112_2_18_3_11_1_233,(uint8*)"d",filesize);
    pKernelProc = SCI_ALLOC(filesize);

    //SCI_TRACE_LOW:"dpaper address:%x!!!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_816_112_2_18_3_11_1_234,(uint8*)"d",pKernelProc);

    pFileBuf = (char *)pKernelProc; 
    /* file copy */

    while(filesize>0)
    {
        SFS_ReadFile(wre_kernel_handle, (void *)pFileBuf, 1024, &readsize, NULL);
        pFileBuf += readsize;
        filesize -= readsize;
    }
    
    SFS_CloseFile(wre_kernel_handle);

    //SCI_TRACE_LOW:"dpaper file closed!!!\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_STARTUP_830_112_2_18_3_11_1_235,(uint8*)"");

    return pKernelProc;
#endif
}

PUBLIC BOOLEAN DSystem_Free(void *hHandle)
{
    return FreeWreLib(hHandle);
}

PUBLIC int * DSystem_GetEntry(void * hHandle, char *lpProcName)
{
    return GetWreProcAddress(hHandle, lpProcName);
}
#endif
PUBLIC BOOLEAN DSystem_Install(uint16 *lpFileName, uint32 nType)
{
    return TRUE;
}

PUBLIC void MMIAPIWRE_ExitWre(void)
{
    if (TRUE == WBOOT_IsWRERunning())
    {
        MMK_SendMsg(MMIMWIN_WIN_ID, MSG_KEYUP_SDCARD_DETECT, PNULL);
    }
}

#endif

#ifdef WRE_WREKERNEL_NORFLASH
#ifdef WIN32
#ifndef SIMULATOR_DEBUG  //WRE simulator debug OFF
int WRE_Kernel_Startup(PWRE_COMM  pWre_Comm){

	return 0;
}
#else

#endif
#endif
#endif
/*Edit by script, ignore 1 case. Fri Apr 27 09:38:56 2012*/ //IGNORE9527
