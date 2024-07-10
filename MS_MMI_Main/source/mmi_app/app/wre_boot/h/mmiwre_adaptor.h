#ifndef     _MMIWRE_ADAPTOR_H_
#define     _MMIWRE_ADAPTOR_H_

typedef struct tagCmdPara
{
    int     argc;
    void    *argv;
}CMDPARA, *PCMCPARA;
typedef enum tagKernelAction{
    KA_START,
    KA_NORMAL,          //normal startup
    KA_DOWNLOAD_WRE,    //download wre kernel
    KA_QUERY_VERSION,    //download wre kernel
    KA_VERIFY_WRE,    //download wre kernel
	KA_GET_PART2_URL,    //download wre kernel
    KA_END
}WRE_KA_T;
typedef struct tagKernelActionDescriptor{
    WRE_KA_T action;
    void* param;
}WRE_KAD_T;

typedef struct _tagWRE_APP_PARAM_T
{
    int  argc;
    uint16 apppath[148];
    uint16 appname[32];
    uint16 iconpath[148];
    uint8 *cmdline;
    BOOLEAN bFixApp;
    BOOLEAN bStore;
}WRE_APP_PARAM_T,*PWRE_APP_PARAM_T;

typedef struct
{
    void    *pCmdPara;
    void    *pMocorToWRE;
    void    *pWREToMocor;
    void    *pCfgPara;
    unsigned short **ppMainLcdBuffer;
    unsigned short *ppSubLcdBuffer;
    WRE_KAD_T action;
}WRE_COMM, *PWRE_COMM;

typedef int (* WRE_KERNEL_STARTUP_FUNC)(PWRE_COMM  pWre_Comm);

void WBOOT_MWIN_XXX_HandleSocketMsg(int sock,int event,int error,void* param_ptr);
BOOLEAN WBOOT_MWIN_XXX_PostMessage(uint32 hwnd, uint32 Msg, uint32 wParam, uint32 lParam);


PUBLIC int WREEntry(int argc,void * argv);

PUBLIC void UninstallWRE(void);

int WRE_ConstructCmdPara(PWRE_APP_PARAM_T pAppParam,PWRE_COMM *pWre_Comm);

PUBLIC BOOLEAN UpdateWRE(void);

PUBLIC long WRE_SDCardInsert(void);

PUBLIC uint32 WBOOT_MWIN_GetAsyncTaskId(void);

PUBLIC BOOLEAN  WBOOT_IsWRERunning(void);

PUBLIC BOOLEAN MMIWRE_MaxmizeAPPWithName(uint16 * appname);
PUBLIC int WRE_IsFixAppInstalled(uint32  fixed_app_id);

typedef struct tagMEMORYINFO{
    int dwFree;
    int dwUsed;
    int dwMaxUsed;
    int reserved[4];
}MEMRORYINFO,*PMEMORYINFO;
void GetMemoryInfo(PMEMORYINFO pInfo);

int WBOOT_WRE_RegEventNotify(uint16 msg_id);
void WBOOT_WRE_SetDeepSleepStatus(uint8 is_deep_sleep);

BOOLEAN  FindAppGUIDByName(uint16 * appname,uint32 * appid,uint16 * guid,uint16 * path,uint16 * iconpath,uint16 * apInname);
BOOLEAN  FindAppPathByGUID(uint16 * guid,uint16 * path,uint16 * appname,uint16 * iconpath);
BOOLEAN  FindAppPathByAppID(uint32 appid,uint16 * path,uint16 * appname,uint16 * iconpath);

int WRE_EntryApp(void);

PUBLIC void *DSystem_Load(uint16 *lpLibFileName);
PUBLIC BOOLEAN DSystem_Free(void *hHandle);
PUBLIC int * DSystem_GetEntry(void * hHandle, char *lpProcName);
PUBLIC BOOLEAN DSystem_Install(uint16 *lpFileName, uint32 nType);

typedef enum tagPREVIEWACTION_K{
	EXIT_PREVIEW,
	PREVIEWING	
}PREVIEWACTION_K;
typedef struct tagPREVIEWEVENT_K{
	 int         cbSize;
	 PREVIEWACTION_K		status;
	void* 	data_buf;
	char reserverd[20];
}PREVIEWEVENT_K;

typedef void (*CAMERA_PREVIEW_CALLBACK_K)(PREVIEWEVENT_K* cp);

void K_StartCameraPreview(CAMERA_PREVIEW_CALLBACK_K fp);
void K_EndCameraPreview(void);

//minimize wre app
typedef enum 
{
    STATUS_WRE_NONE = 0,
    STATUS_WRE_NORMAL = 1, //app mode
    STATUS_WRE_QUITING = 2, //wre sys is quiting
    STATUS_WRE_INITIAL = 3, //wre sys download or install mode.
    STATUS_WRE_MINI = 4,  //wre mini (background running) mode.
    STATUS_WRE_NOROOM = 5   //wre kernel no room.
}STATUS_WRE_E;

typedef struct _tagWREAPP_INFO_T
{
    void       *app_instance;
    uint16      appname[32];
    uint16     iconpath[100];
}WREAPP_INFO_T,*PWREAPP_INFO_T;

typedef struct _tagMINI_BK_FUNC_T
{
    void *  pGetCount;
    void *  pGetAppInfo;
    void *  pShowApp;
    void *  pGetWREStatus;
}MINI_BK_FUNC_T,*PMINI_BK_FUNC_T;


#define MAX_WRE_APP_NUM    (32)

//WRE backgrounp application operation
typedef enum 
{
    WRE_APP_MAXMIZE = 1,
    WRE_APP_MINIMIZE  = 2,
    WRE_APP_CLOSE  = 3   
    
}WREAPP_OPERATION_E;

//#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
typedef enum
{
    WRETOMORCOR_NONE,
    WRETOMORCOR_PHOTOVIEWER,
    WRETOMORCOR_VIDEOPLAYER,
    WRETOMORCOR_BROWSER,
    WRETOMORCOR_EBOOK,
}WRETOMORCOR_APP_E;

typedef struct _tagWRETOMORCOR_INFO_T
{
    uint16 AppType;
    unsigned short * lpParam;
}WRETOMORCOR_INFO_T,*PWRETOMORCOR_INFO_T;

typedef struct _tagWREAPP_VIDEO_PARAM
{
    uint16	  VideoType;			//视频来源文件还是网络
    uint16   strFileName[160];		//文件名
    uint16	  nFileNameLen;       //文件名长度
    char      *strUrl;             
}WRETOMOCOR_VIDEO_PARAM, *PWRETOMOCOR_VIDEO_PARAM;
extern WRETOMORCOR_INFO_T wretomocorapp;
void WreToMocorApp_Realse(void);
//#endif
void Adapt_EntryAudioPlayer(const uint16 *lpFullFileName, int filename_len);
void Adapt_EntryPhotoView(const uint16 *lpFullFileName, int filename_len);
void Adapt_EntryEBookView(const uint16 *lpFullFileName, int filename_len);
void Adapt_EntrySNSView(int app_id, int wreappid, int commmond_type, int commond_result);
BOOLEAN WRE_GetAppInfo(int index,PWREAPP_INFO_T pInfo);
int wre_startup(PWRE_APP_PARAM_T pAppParam);
PUBLIC void SetDownLoadFlag();
PUBLIC uint8 IsDownLoadFlag();
PUBLIC BOOLEAN WRE_CreateVirtualROMDisk();
PUBLIC BOOLEAN WRE_CreateVirtualRAMDisk();
PUBLIC void WRE_DestoryVirtualDisk();	
PUBLIC void WRE_TestVirtualDisk();
uint32 WRE_GetMessage(void* p);
PUBLIC int WREMonkeyTest_EXE(const uint16 *exe_name);
//add  20120521  --- 解决分辨率不相配
#define WRE_AUTO_STORE_SREEN_FIX   //可关闭此宏，将不检查store的分辨率
 #ifdef WRE_AUTO_STORE_SREEN_FIX 
 #define STORE_SCREEN_IS_FIXED     0
  #define STORE_SCREEN_NOT_FIXED 1
  #define STORE_SCREEN_NOT_SURE   2
int WRE_IsStoreScreenFix(void);  
void OpenWREQueryWin(int  format); 
BOOLEAN wre_load_store_ver_dat(uint16 *dir);
#endif
#endif
