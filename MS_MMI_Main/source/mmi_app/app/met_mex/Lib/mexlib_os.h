/************************************************************************
* 版权所有 (C)2010,扬讯科技。
* 
* 文件名称： // MexLib_Os.h
* 文件标识：
* 内容摘要： //mex平台os 库
*
************************************************************************/

#if defined(MEX_TARGET_SERVER) && !defined(__MEXLIB_OS_SERVER_H__) || defined(MEX_TARGET_CLIENT) && !defined(__MEXLIB_OS_CLIENT_H__)

#if defined(MEX_TARGET_SERVER)
	#define __MEXLIB_OS_SERVER_H__
#elif defined(MEX_TARGET_CLIENT)
	#define __MEXLIB_OS_CLIENT_H__
#endif

#if defined(MEX_TARGET_SERVER)
#if !defined(__MEXLIB_DEF_SERVER_H__)
#error Please Include MexLib_Def.h instead of MexLib_Os.h!!!
#endif
#elif defined(MEX_TARGET_CLIENT)
#if !defined(__MEXLIB_DEF_CLIENT_H__)
#error Please Include MexLib_Def.h instead of MexLib_Os.h!!!
#endif
#endif
//common macro definitions for OS
#define MEX_H_LAUNCH_FUNC_OS					MEX_H_LAUNCH_FUNC( Os )
#define MEX_C_LIB_FUNC_NAME_OS					MEX_C_LIB_FUNC_NAME( Os )

#define MEX_H_GET_FUNC_DEFINE_OS(x)				MEX_H_GET_FUNC_DEFINE( Os, x )

#define MEX_H_ENUM_FUNC_START_OS				MEX_H_ENUM_FUNC_START( Os )
#define MEX_H_ENUM_FUNC_OS(x)					MEX_H_ENUM_FUNC( Os, x )
#define MEX_H_ENUM_FUNC_END_OS					MEX_H_ENUM_FUNC_END( Os )

#define MEX_H_NB_FUNC_OS						MEX_H_NB_FUNC( Os )
//end common macro definitions

#if defined(MEX_TARGET_CLIENT)
	#define MEX_CALL_APP_BEGIN( iAppId )			{ int32 __iOldApp = GetCurrentApp(); SetCurrentApp( iAppId );
	#define MEX_CALL_APP_END()						SetCurrentApp( __iOldApp ); }
#endif

//文件操作fseek的起始位置
#ifndef SEEK_SET
	#define SEEK_SET			0		//文件开头
	#define SEEK_CUR			1		//当前读取位置
	#define SEEK_END			2		//文件结尾
#endif

//定义输入框的输入类型
typedef enum 
{
	E_MITT_NUMERIC,						//输入数字
    E_MITT_ALPHANUMERIC_UPPERCASE,		//输入大写字母、数字
    E_MITT_ALPHANUMERIC_LOWERCASE,		//输入小写字母、数字
    E_MITT_SM_PINYIN,					//输入汉语拼音
    E_MITT_SM_STROKE,					//输入汉语笔画
    E_MITT_SM_MULTITAP_PINYIN			//输入汉语全拼
}E_Mex_Input_Type;

//输入法标题最长的支持
#define MEX_TITLE_BUFFER_LENGTH		64

//输入框的上下文环境
typedef struct _Mex_Input_Context
{
	int32			m_iInputType;								//当前的输入框类型
	wchar_t			m_pInputTitle[MEX_TITLE_BUFFER_LENGTH];		//当前的输入框标题
	uint32			m_iInputMaxNum;								//当前的输入字符最大个数
	uint8 			*m_pInputBuffer;							//当前的输入缓冲
	bool 			m_bIsPassword;								//当前是否为密码模式
	bool			m_bCanChange;								//当前是否允许切换输入法
	int32			m_iAppId;									//调用输入框的App ID
	Mex_Input_Func	m_pInputFunc;								//输入框的回调函数指针
	bool			m_bIsRunning;								//是否已经退出
	int32			m_iInputBufferPool;							//当前缓存申请的哪块内存池
} Mex_Input_Context;



//时间日期，用于GetDateTime
typedef struct
{
	uint16 year;		//年
	uint8 month;		//月
	uint8 day;			//日
    uint8  dayIndex;	// 星期 0=Sunday
	uint8 hour;			//小时
	uint8 minute;		//分
	uint8 second;		//秒
} Mex_DateTime;

//坐标
typedef struct
{
	int32 m_iPosX;		//x
	int32 m_iPosY;		//y
}Mex_Point;

//原GUI库部分函数 begin ////////////////////////////////////
typedef struct _Mex_GuiBuffer
{
	int32 size;
	uint16 width,height;
	int16 clipx1, clipy1, clipx2, clipy2;
	void *buffer;
	uint8 vcf;
} Mex_GuiBuffer;

// image struct
typedef struct _Mex_Image
{
	uint16 type;				                 // 类型
	uint16 attr;                                 // 属性
	int32 width, height;                         // 宽度、高度
	int16 center_x, center_y;                    // 中心
	uint16 draw_x1, draw_y1, draw_x2, draw_y2;   // 绘制区域
	uint16 draw_width, draw_height;              // 绘制区域宽高
	mex_color *buffer;                           // 图像/调色版
	byte *Abuffer;                               // alpha缓存
	byte *img_Ibuffer;                           // 图像->调色块索引缓存
	int16 Ibuffer_width, Ibuffer_height;         // 图像->调色块索引缓存宽度及高度
	byte **palChk_Ibuffer;                       // 调色块->调色板索引缓存
	int16 palChk_width, palChk_height;           // 调色块->调色板索引缓存宽度及高度
	byte palChk_count;                           // 调色块数目
	byte palClr_count;                           // 调色板颜色数
	byte alpha;                                  // alpha等级（0~16）
	uint16 **accel;                              // 加速器
	mex_color colorkey;                          // 透明色
} Mex_Image;

// 系统状态栏显示类型
typedef enum
{
	E_MQST_INVALID = -1,	//非法值

	E_MQST_ONLINE,			//在线
	E_MQST_OFFLINE,			//离线
	E_MQST_MSG,				//来消息
	E_MQST_HIDE,			//隐身
	E_MQST_AWAY,			//离开

	E_MQST_MAX
}E_MEX_QQ_STATUS_TYPE;



//系统引导界面显示配置
typedef struct _Mex_Config_Loading_Page
{
	bool			m_bShowColorBar;
	mex_color		m_cBgColor;
	mex_color		m_cFontColor;
	uint8			m_uFontPos;
	const wchar_t*  m_szLoadStr;
}Mex_Config_Loading_Page;



// system defines
#define MEX_GUI_COLOR_FORMAT_1                  0   /* 1-bit , black/white color */
#define MEX_GUI_COLOR_FORMAT_8                  1   /* 8-bit , index color */
#define MEX_GUI_COLOR_FORMAT_16                 2   /* 16-bit, rgb 565 */
#define MEX_GUI_COLOR_FORMAT_24                 3   /* 24-bit, B,G,R       8,8,8 */
#define MEX_GUI_COLOR_FORMAT_32                 4   /* 32-bit, B,G,R,A  8,8,8,8 */
#define MEX_COLOR_FORMAT                        MEX_GUI_COLOR_FORMAT_16

#define MEX_GDI_SUCCEED                         0
#define MEX_GDI_IMAGE_TYPE_BMP                  1
#define MEX_GDI_IMAGE_TYPE_BMP_FILE             6
#define MEX_GDI_IMAGE_TYPE_GIF                  3
#define MEX_GDI_IMAGE_TYPE_GIF_FILE             7
#define MEX_GDI_IMAGE_TYPE_JPG                  9
#define MEX_GDI_IMAGE_TYPE_JPG_FILE             10
#define MEX_GDI_IMAGE_TYPE_PNG                  19
#define MEX_GDI_IMAGE_TYPE_PNG_FILE             20

// font defines
// font size
#define MEX_FONTSIZE_SMALL                      0x0000
#define MEX_FONTSIZE_MEDIUM                     0x0001
#define MEX_FONTSIZE_LARGE                      0x0002
#define MEX_FONTSIZE_SUBLCD                     0x0003
#define MEX_FONTSIZE_DIALER                     0x0004
#define MEX_FONTSIZE_VIRTUAL_KEYBOARD           0x0005

// font attr
#define MEX_FONTATTR_NORMAL                     0x0001
#define MEX_FONTATTR_BOLD                       0x0002
#define MEX_FONTATTR_ITALIC                     0x0004
#define MEX_FONTATTR_OBLIQUE                    0x0008
#define MEX_FONTATTR_UNDERLINE                  0x0010
#define MEX_FONTATTR_STRIKETHROUGH              0x0020

// font position
#define MEX_FONTPOS_DEFAULT                     0x0001
#define MEX_FONTPOS_VTOP                        0x0002
#define MEX_FONTPOS_VMIDDLE                     0x0004
#define MEX_FONTPOS_VBOTTOM                     0x0008
#define MEX_FONTPOS_HLEFT                       0x0010
#define MEX_FONTPOS_HCENTER                     0x0020
#define MEX_FONTPOS_HRIGHT                      0x0040
//原GUI库部分函数 end ////////////////////////////////////

#define MEX_APP_ATTRIB_FULL_NO_NEED_LAST_SCREEN		0x00000001		//表示全屏程序每次绘图时不需要上次的屏幕数据
#define MEX_APP_ATTRIB_FULL_MANUAL_UPDATE_SCREEN	0x00000002		//表示全屏程序仅当自己调用Invalidate()时才会收到Paint消息
#define MEX_APP_ATTRIB_RESERVED2					0x00000004		//保留属性，以后扩展用
#define MEX_APP_ATTRIB_RESERVED3					0x00000008		//保留属性，以后扩展用
#define MEX_APP_ATTRIB_RESERVED4					0x00000010		//保留属性，以后扩展用
#define MEX_APP_ATTRIB_RESERVED5					0x00000020		//保留属性，以后扩展用
#define MEX_APP_ATTRIB_RESERVED6					0x00000040		//保留属性，以后扩展用
#define MEX_APP_ATTRIB_RESERVED7					0x00000080		//保留属性，以后扩展用

//文件属性
#define MEX_FS_ATTR_READ_ONLY        0x01
#define MEX_FS_ATTR_HIDDEN           0x02
#define MEX_FS_ATTR_SYSTEM           0x04
#define MEX_FS_ATTR_VOLUME           0x08
#define MEX_FS_ATTR_DIR              0x10
#define MEX_FS_ATTR_ARCHIVE          0x20
#define MEX_FS_LONGNAME_ATTR         0x0F


MEX_H_DECLARE_FUNC( funcptr**,		ExchangeLibPtrPtr,				(int32 iLibId, int32 iNbFuncsInLib) )
MEX_H_DECLARE_FUNC( const char*,	GetLaunchParam,					(void) )

MEX_H_DECLARE_FUNC( int32,			LaunchAppEx,					(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam) )
MEX_H_DECLARE_FUNC( int32,			LaunchApp,						(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir) )
MEX_H_DECLARE_FUNC( bool,			RegisterLaunchAppAfterExit,		(bool bEnable, const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir) )
MEX_H_DECLARE_FUNC( void,			FreeApp,						(int32 iAppId) )
MEX_H_DECLARE_FUNC( void,			RegisterMsgHandler,				(int32 iAppId, MexApp_MsgHandler_Func pFunc) )
MEX_H_DECLARE_FUNC( void,			RegisterExitFunc,				(int32 iAppId, MexApp_Exit_Func pFunc, bool bAmILib) )
MEX_H_DECLARE_FUNC( MexApp_MsgHandler_Func,	GetMsgHandler,			(int32 iAppId) )
MEX_H_DECLARE_FUNC( void,			Exit,							(void) )

MEX_H_DECLARE_FUNC( bool,			LoadAppIntoSRam,				(int32 iAppId) )
MEX_H_DECLARE_FUNC( void,			FreeAppFromSRam,				(int32 iAppId) )
MEX_H_DECLARE_FUNC( bool,			LoadLibIntoSRam,				(funcptr pLibFuncPtr) )
MEX_H_DECLARE_FUNC( void,			FreeLibFromSRam,				(funcptr pLibFuncPtr) )

MEX_H_DECLARE_FUNC( int32,			GetCurrentApp,					(void) )
MEX_H_DECLARE_FUNC( void,			SetCurrentApp,					(int32 iAppId) )

MEX_H_DECLARE_FUNC( const wchar_t*,	GetSystemRootDir,				(void) )
MEX_H_DECLARE_FUNC( const wchar_t*,	GetAppRootDir,					(void) )
MEX_H_DECLARE_FUNC( const wchar_t*,	GetAppDir,						(const wchar_t *szAppFileName) )

MEX_H_DECLARE_FUNC( const wchar_t*, GetCurAppDir,					(wchar_t* curAppDir) )
MEX_H_DECLARE_FUNC( const wchar_t*, GetCurAppFileName,				(wchar_t* curAppFileName) )

MEX_H_DECLARE_FUNC( bool,			BeginPaint,						(void) )
MEX_H_DECLARE_FUNC( void,			EndPaint,						(void) )
MEX_H_DECLARE_FUNC( bool,			Invalidate,						(void) )

MEX_H_DECLARE_FUNC( int32,			GetFreeMemory,					(E_Mex_MemoryPool ePool) )
MEX_H_DECLARE_FUNC( int32,			GetAppAttribute,				(int32 iAppId) )
MEX_H_DECLARE_FUNC( int32,			SetAppAttribute,				(int32 iAppId, int32 iAttribute) )
MEX_H_DECLARE_FUNC( bool,			PauseAppTimer,					(int32 iAppId, int32 iTimerId, bool bPause) )

//memory issues are handled by app itself, saved in system, paint to screen by manager
MEX_H_DECLARE_FUNC( uint16*,		GetScreenLayerBuffer,			(int32 iLayer) )
MEX_H_DECLARE_FUNC( void,			DisableDbBufferBlt,				(bool bDisable) )
MEX_H_DECLARE_FUNC( void,			GetFullScreenBuffer,			(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight) )
MEX_H_DECLARE_FUNC( void,			GetAppScreenBuffer,				(int32 iAppId, uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight) )
MEX_H_DECLARE_FUNC( void,			RegisterScreenBuffer,			(uint16 *pBuffer, uint16 uWidth, uint16 uHeight) )
MEX_H_DECLARE_FUNC( void,			UnRegisterScreenBuffer,			(void) )
MEX_H_DECLARE_FUNC( void,			GetAppScreenKeyColor, 			(int32 iAppId, uint16 *pKeyColor, bool *pbIsTrans ) )
MEX_H_DECLARE_FUNC( void,			RegisterScreenKeyColor,			(uint16 uKeyColor, bool bIsTrans ) )

MEX_H_DECLARE_FUNC( void*,			GetAppScreenImage, 				(int32 iAppId) )

MEX_H_DECLARE_FUNC( int32,			SetTimer,						(int32 iTime) )
MEX_H_DECLARE_FUNC( bool,			KillTimer,						(int32 iTimerId) )
MEX_H_DECLARE_FUNC( bool,			SetTimerInterval,				(int32 iAppId, int32 iTimerId, int32 iNewTime, int32 *pOldTime) )

MEX_H_DECLARE_FUNC( int32,			gettime,						(void) )

MEX_H_DECLARE_FUNC( int32,			GetNextAppId,					(int32 iCurAppId) )
MEX_H_DECLARE_FUNC( bool,			GetAppInfoFromId,				(int32 iCurAppId, int32 *piAppType, int16 *piAppVersion, wchar_t **ppAppName, wchar_t **ppFileDir, wchar_t **ppFileName) )
MEX_H_DECLARE_FUNC( bool,			GetAppInfoFromFile,				(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion) )
MEX_H_DECLARE_FUNC( bool,			GetAppInfoFromFile_ext,			(const wchar_t *szFullFileName, int32 *piAppType, int16 *piAppVersion) )

MEX_H_DECLARE_FUNC( E_Mex_RunMode,	GetRunMode,						(void) )
MEX_H_DECLARE_FUNC( bool,			SwitchRunMode, 					(E_Mex_RunMode newMode) )
MEX_H_DECLARE_FUNC( void*,			GetRegistry,					(void) )

MEX_H_DECLARE_FUNC( int32,			SendMessage,					(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMessage, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3) )

MEX_H_DECLARE_FUNC( int32,			GetPhoneConfig,					(E_Mex_Phone_Config cfg) )

MEX_H_DECLARE_FUNC( int32,			ConvertSysDef,					(E_SYS2MEX_Def_Convert param) )

MEX_H_DECLARE_FUNC( void,			GetDateTime,					(Mex_DateTime *pDt) )


//file functions
MEX_H_DECLARE_FUNC( int32*, 		fopenw_org, 		(const wchar_t *filename, const wchar_t *mode) )	//note: org fs_open
MEX_H_DECLARE_FUNC( int32*, 		fopenw_ext, 		(const wchar_t *filename, const wchar_t *mode, const wchar_t *path, const wchar_t *mexFileName, bool bCheckInsideMexOnly) )	//note: support only read, write, readwrite
MEX_H_DECLARE_FUNC( int32*, 		fopenw, 			(const wchar_t *filename, const wchar_t *mode) )	//note: support only read, write, readwrite
MEX_H_DECLARE_FUNC( int32*, 		fopen,				(const char *filename, const char *mode) )
MEX_H_DECLARE_FUNC( int32,			fseek,				(int32* handle, long offset, int32 whence) )
MEX_H_DECLARE_FUNC( int32,			ftell,				(int32* handle) )
MEX_H_DECLARE_FUNC( int32,			fclose, 			(int32* handle) )
MEX_H_DECLARE_FUNC( size_t,			GetFileSize, 		(int32* handle) )
MEX_H_DECLARE_FUNC( size_t, 		fread,				(void *ptr, size_t size, size_t nitems, int32* handle) )
MEX_H_DECLARE_FUNC( size_t, 		fwrite, 			(const void *ptr, size_t size, size_t nitems, int32* handle) )
MEX_H_DECLARE_FUNC( int32,			fflush, 			(int32* handle) )
MEX_H_DECLARE_FUNC( int32,			remove, 			(const wchar_t *file) )
MEX_H_DECLARE_FUNC( int32,			remove_ext, 		(const wchar_t *file) )
MEX_H_DECLARE_FUNC( int32,			rename, 			(const wchar_t *ofile, const wchar_t *nfile) )
MEX_H_DECLARE_FUNC( int32,			rename_ext, 		(const wchar_t *ofile, const wchar_t *nfile) )

MEX_H_DECLARE_FUNC( int32,			CreateDir, 			(const wchar_t *dir) )
MEX_H_DECLARE_FUNC( int32,			CreateDir_ext, 		(const wchar_t *dir) )
MEX_H_DECLARE_FUNC( int32,			RemoveDir,			(const wchar_t *dir) )
MEX_H_DECLARE_FUNC( int32,			RemoveDir_ext,		(const wchar_t *dir) )
MEX_H_DECLARE_FUNC( bool,			IsDirExist_ext,		(const wchar_t *dir) )

MEX_H_DECLARE_FUNC( int32,			FindFirst,			(const wchar_t *path, const wchar_t *namePattern, wchar_t *fileName, uint32 maxLength) )
MEX_H_DECLARE_FUNC( int32,			FindDirFirst,		(const wchar_t *path, wchar_t *fileName, uint32 maxLength) )
MEX_H_DECLARE_FUNC( int32,			FindNext,			(int32 findHandle, wchar_t *fileName, uint32 maxLength) )
MEX_H_DECLARE_FUNC( int32,			FindClose,			(int32 findHandle) )

MEX_H_DECLARE_FUNC( void,			log,				(int32 iFilter, const char *pText, ...) )
MEX_H_DECLARE_FUNC( int32,			getlogfilter,		(void) )
MEX_H_DECLARE_FUNC( void,			setlogfilter,		(int32 iFilter) )

MEX_H_DECLARE_FUNC( void,			StartProfiling,		(bool bRestart, int32 iNbFrames) )
MEX_H_DECLARE_FUNC( void,			AddProfilingData,	(bool bIsStart, int16 *piLevel, const char *pText) )


//key state
MEX_H_DECLARE_FUNC( E_Mex_KeyEvent,	GetKeyState,		(E_Mex_KeyCode keyCode) )
MEX_H_DECLARE_FUNC( E_Mex_KeyEvent,	SetKeyState,		(E_Mex_KeyEvent keyEvent, E_Mex_KeyCode keyCode) )

//virtual memory operations
MEX_H_DECLARE_FUNC( void*,			VMalloc,			(size_t iSize) )
MEX_H_DECLARE_FUNC( void,			VFree,				(void* pVmPtr) )
MEX_H_DECLARE_FUNC( void*,			VMemcpy,			(void* pVmPtr, void *pMemPtr, size_t size) )
MEX_H_DECLARE_FUNC( void*,			VToMem,				(void *pMemPtr, void *pVmPtr, size_t size) )
MEX_H_DECLARE_FUNC( void*,			VPMalloc,			(const char *szKeyName, size_t iSize, bool *pbAlreadyExist) )
MEX_H_DECLARE_FUNC( void,			VPFree,				(void* pVmPtr) )

//special memory pool operations
MEX_H_DECLARE_FUNC( void,			SetUseReservedMem,			(int32 iAppID, bool bUse) )

//timer call back
MEX_H_DECLARE_FUNC( int32,			StartSystemTimer,			(int32 iTime, MexFuncPtr func) )
MEX_H_DECLARE_FUNC( bool, 			KillSystemTimer,			(int32 iTimeId) )
MEX_H_DECLARE_FUNC( bool, 			SetSystemTimerInterval,		(int32 iTimerId, int32 iNewTime, int32 *pOldTime) )

//manager save/load
MEX_H_DECLARE_FUNC( bool, 			RegisterLaunchOnScreen,		(int32 iScreenId) )
MEX_H_DECLARE_FUNC( bool, 			RegisterLaunchOnStartup,	(void) )
MEX_H_DECLARE_FUNC( void, 			GetLastFullScreenBuffer,	(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight) )

//input
MEX_H_DECLARE_FUNC( bool, 			InputBox_Create,			(wchar_t* pInputTitle, E_Mex_Input_Type iInputType, uint32 iMaxNum, bool bIsPassword, bool bCanChange, uint8 * pInputBuffer, Mex_Input_Func pInputFunc ) )

//paint msg
MEX_H_DECLARE_FUNC( bool, 			PaintAllPause,				(void) )
MEX_H_DECLARE_FUNC( bool, 			PaintAllResume,				(void) )

MEX_H_DECLARE_FUNC( int32,			zip_uncompress, 			(int8 * pDest, unsigned long* lDestLen, const int8 * pSource, unsigned long lSourceLen) )

MEX_H_DECLARE_FUNC( void, 			DisablePaintOpt,			(int32 iMaxNbFrames) )

//flash memory operations
MEX_H_DECLARE_FUNC( void*,			smalloc,					(size_t iSize) )
MEX_H_DECLARE_FUNC( void,			sfree,						(void* psPtr) )
MEX_H_DECLARE_FUNC( bool,			NotifyLockSRam,				(void) )		//告诉系统本程序将暂时使用所有的sram空间
MEX_H_DECLARE_FUNC( bool,			NotifyUnlockSRam,			(void) )		//告诉系统本程序sram空间用好了

//platform operations
MEX_H_DECLARE_FUNC( void,			ShutdownPlatform,			(E_Mex_PlatformShutdown eOp) )

MEX_H_DECLARE_FUNC( bool, 			RegisterLaunchOnFullAppExit,	(void) )

MEX_H_DECLARE_FUNC( bool,			SeparateAbsPath,			(wchar_t **pathPart, wchar_t **namePart, const wchar_t *path) )

MEX_H_DECLARE_FUNC( bool, 			CreateDir_ext_recur,		(const wchar_t *szFullDir) )

MEX_H_DECLARE_FUNC( void,			ExitPlatform,				( bool bExitAll ) )

//get app reg size
MEX_H_DECLARE_FUNC( bool,			GetAppRegistSizeFromId,		(int32 iCurAppId, uint16 *puWidth, uint16 *puHeight ) )
MEX_H_DECLARE_FUNC( bool,			GetAppRegistSizeFromFile, 	(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, uint16 *puWidth, uint16 *puHeight ) )
MEX_H_DECLARE_FUNC( uint32,			GetCurrentTime,				(void) )
MEX_H_DECLARE_FUNC( void,			UtcSec2DateTime,			(uint32 utc_sec, Mex_DateTime *pdt) )

//get memory from other places
MEX_H_DECLARE_FUNC( void*,			xmalloc,					(size_t iSize) )
MEX_H_DECLARE_FUNC( void,			xfree,						(void* pPtr) )

//manager function control
MEX_H_DECLARE_FUNC( bool,			DisableQuitControl,			(bool bDisable) )

//extra info, in case...
MEX_H_DECLARE_FUNC( bool,			GetAppExtraInfoFromId,		(int32 iCurAppId, int16 *ppInfo ) )
MEX_H_DECLARE_FUNC( bool,			GetAppExtraInfoFromFile, 	(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int16 *pInfo ) )

//system extra info, internal use
MEX_H_DECLARE_FUNC( int32,			GetSystemParam,				(void) )

//sleep some time...
MEX_H_DECLARE_FUNC( void,			Sleep,						(int32 iTicks) )

//launch system app..
MEX_H_DECLARE_FUNC( void,			RegisterLaunchSystemApp,	(E_MEX_Entry_System_App eApp, void *pParam, int32 iParamSize) )

MEX_H_DECLARE_FUNC( int32,			GetFileAttributes_ext,		(const wchar_t *FileName) )
MEX_H_DECLARE_FUNC( int32,			SetFileAttributes_ext,		(const wchar_t *FileName, int8 Attributes) )

MEX_H_DECLARE_FUNC( int32,			GetFileAttributes,			(const wchar_t *FileName) )
MEX_H_DECLARE_FUNC( int32,			SetFileAttributes,			(const wchar_t *FileName, int8 Attributes) )

// 原GUI库部分函数 begin ////////////////////////////////////
MEX_H_DECLARE_FUNC( Mex_GuiBuffer,	SelectGuiBuffer,				(Mex_GuiBuffer *buffer) )
MEX_H_DECLARE_FUNC( void,			GetBufferClipper,				(int16 *x1, int16 *x2, int16 *y1, int16 *y2) )
MEX_H_DECLARE_FUNC( void,			SetBufferClipper,				(int16 x1, int16 x2, int16 y1, int16 y2) )
MEX_H_DECLARE_FUNC( void,			SetBufferSubClipper,			(int16 x1, int16 x2, int16 y1, int16 y2) )

MEX_H_DECLARE_FUNC( void,			GetCharDimension,				(uint32 ch, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize) )
MEX_H_DECLARE_FUNC( void,			GetStringDimension,				(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize) )
MEX_H_DECLARE_FUNC( void,			GetStringSpacingDimension,		(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize, int16 charSpace) )
MEX_H_DECLARE_FUNC( uint16,			GetStringExDimension,			(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, uint8 fontAttr, uint8 fontSize, bool lineWrap) )
MEX_H_DECLARE_FUNC( uint16,			GetStringExSpacingDimension,	(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, uint8 fontAttr, uint8 fontSize, bool lineWrap, int16 lineSpace, int16 columnSpace) )
MEX_H_DECLARE_FUNC( int32,			DrawString,						(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize) )
MEX_H_DECLARE_FUNC( int32,			DrawStringSpacing,				(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize, int16 charSpace) )
MEX_H_DECLARE_FUNC( int32,          DrawStringEx,					(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap) )
MEX_H_DECLARE_FUNC( int32,          DrawStringExSpacing,			(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap, int16 lineSpace, int16 columnSpace) )
MEX_H_DECLARE_FUNC( void,			DrawChar,						(uint8 *pCharData, uint32 dataSize, uint16 dataWidth, uint16 dataHeight, int32 x, int32 y, mex_color clr, uint8 fontAttr) )
MEX_H_DECLARE_FUNC( int32,			GetCharData,					(int32 ch, uint8 **ppCharData, uint16 *pDataWidth, uint16 *pDataHeight, uint8 fontSize) )

MEX_H_DECLARE_FUNC( void,			DrawPointInternal,				(int32 x, int32 y, mex_color clr) )
MEX_H_DECLARE_FUNC( void,			DrawLineInternal,				(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr) )
MEX_H_DECLARE_FUNC( void,			DrawFrameRectInternal,			(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr) )
MEX_H_DECLARE_FUNC( void,			DrawFillRectInternal,			(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr) )
MEX_H_DECLARE_FUNC( void,			DrawFrameCircleInternal,		(int32 x, int32 y, int32 r, mex_color clr) )
MEX_H_DECLARE_FUNC( void,			DrawFillCircleInternal,			(int32 x, int32 y, int32 r, mex_color clr) )

MEX_H_DECLARE_FUNC( uint16,			GetImageTypeFromMemInternal,			(uint8 *ptr))
MEX_H_DECLARE_FUNC( uint16,			GetImageTypeFromFileInternal,			(const char *ptr) )
MEX_H_DECLARE_FUNC( int32,			GetImageDimensionFromMemInternal,		(uint8 img_type,uint8 *img_ptr,int32 img_size,int32 *width, int32 *height) )
MEX_H_DECLARE_FUNC( int32,			GetImageDimensionFromFileInternal,		(const char *image_name, int32 *width, int32 *height) )
MEX_H_DECLARE_FUNC( int32,			GetGifImageFrameCountFromMemInternal,	(uint8 *GIF_src, uint32 size, int32 *frame_count) )
MEX_H_DECLARE_FUNC( int32,			GetGifImageFrameCountFromFileInternal,	(const char *filename, int32 *frame_count) )
MEX_H_DECLARE_FUNC( int32,			DrawImageFromMemInternal,				(int32 x, int32 y, uint8 *img_ptr, uint8 img_type, int32 img_size) )
MEX_H_DECLARE_FUNC( int32,			DrawImageFromFileInternal,				(int32 x, int32 y, const char *img_name) )
MEX_H_DECLARE_FUNC( int32,			DrawImageFromMemFramesInternal,			(int32 x, int32 y, uint8 *img_ptr, uint8 img_type, int32 img_size, uint32 frame_no) )
MEX_H_DECLARE_FUNC( int32,			DrawImageFromFileFramesInternal,		(int32 x, int32 y, const char *img_name,uint32 frame_no) )
// 原GUI库部分函数 end ////////////////////////////////////

//app extra info, internal use
MEX_H_DECLARE_FUNC( void,			GetAppParam,				(int32 iAppId, int32 *pResults) )

MEX_H_DECLARE_FUNC( void,			GetAdmFunctions,			(funcptr *pResults) )
	
MEX_H_DECLARE_FUNC( bool,			IsAppPermissiveToDo,		( int32 iCurAppId, E_Mex_App_Permission_Type ePermission ) )

MEX_H_DECLARE_FUNC( void,			GetActScreenBuffer,			(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight) )

MEX_H_DECLARE_FUNC( bool,			UnCompressDataFromMemory,		(uint8 *memData, uint8 *orgData) )
MEX_H_DECLARE_FUNC( bool,			UnCompressDataFromFile,			(int32 *file_handle, uint8 *orgData) )

//easy manager functions... for future use like intents
MEX_H_DECLARE_FUNC( int32,			CallManagerFunction,		(E_Mex_Mgr_Manager_Func eFunc, int32 iParam0, int32 iParam1, int32 iParam2) )

//simulate key press after system shuts down
MEX_H_DECLARE_FUNC( void,			RegisterSimulateKey,		(E_Mex_KeyEvent eKeyEvent, E_Mex_KeyCode eKeyCode) )

//thread timer call back
MEX_H_DECLARE_FUNC( int32,			StartThreadTimer,			(int32 iTime, MexFuncPtr func) )
MEX_H_DECLARE_FUNC( bool, 			KillThreadTimer,			(int32 iTimeId) )
MEX_H_DECLARE_FUNC( bool, 			SetThreadTimerInterval,		(int32 iTimerId, int32 iNewTime, int32 *pOldTime) )

//simple lock/unlock function for threads
MEX_H_DECLARE_FUNC( void, 			TakeMutex,					(int32 iIdx) )
MEX_H_DECLARE_FUNC( void, 			GiveMutex,					(int32 iIdx) )

//get or set app position
MEX_H_DECLARE_FUNC( bool,			GetAppPosition,				(int32 iAppId, int16 * pCenterPosX, int16 * pCenterPosY) )
MEX_H_DECLARE_FUNC( bool,			SetAppPosition,				(int32 iAppId, int16 iCenterPosX, int16 iCenterPosY) )

//just for qq app
MEX_H_DECLARE_FUNC( int8,					RegSystemStatusIcon,		(int32 iAppid) )

MEX_H_DECLARE_FUNC( void,					UnRegSystemStatusIcon,		(int8 handle,int32 iAppid) )

MEX_H_DECLARE_FUNC( void,					SetSystemStatusIcon,		(int8 handle,E_MEX_QQ_STATUS_TYPE type,int32 iAppid) )

MEX_H_DECLARE_FUNC( E_MEX_QQ_STATUS_TYPE,	GetSystemStatusIcon,		(int8 handle,int32 iAppid) )

MEX_H_DECLARE_FUNC( void,					SetSystemStatusIconStatus,	(int8 handle,bool bShow,int32 iAppid) )

#if MEX_SDK_VER >= 1001
MEX_H_DECLARE_FUNC( bool,			GetScreenLayerBufferWithInfo,		(int32 iLayer, uint16 **ppScrBuf, uint16 *puWidth, uint16 *puHeight) )
#endif

#if MEX_SDK_VER >= 1004
MEX_H_DECLARE_FUNC( void*,			tmalloc,					(size_t iSize) )
MEX_H_DECLARE_FUNC( void,			tfree,						(void* psPtr) )
#endif

#if MEX_SDK_VER >= 1009
MEX_H_DECLARE_FUNC( int8,			Fmgr_Create,				(uint32 sel_type, wchar_t* base_path,Fmgr_CallBack pfCB) )
MEX_H_DECLARE_FUNC( void, 			Fmgr_Cancel,				(void) )
MEX_H_DECLARE_FUNC( void,			Fmgr_SetFilter,				(E_Mex_Fmgr_Filtertype filter) )
MEX_H_DECLARE_FUNC( void, 			Fmgr_Show,					(void) )
#endif

#if MEX_SDK_VER >= 1010
MEX_H_DECLARE_FUNC( void,			Pen_Block,					(void) )
MEX_H_DECLARE_FUNC( void, 			Pen_unBlock,				(void) )
MEX_H_DECLARE_FUNC( void,			KeyPad_Lock,				(void) )
MEX_H_DECLARE_FUNC( void, 			KeyPad_unLock,				(void) )
MEX_H_DECLARE_FUNC( void,			ReStartKeyPadLock,			(void) )
#endif

#if MEX_SDK_VER >= 1011
MEX_H_DECLARE_FUNC( int32,			DrawImageScaleFromFileInternal,				(int32 x1, int32 y1, int32 x2, int32 y2, const char *img_name) )
MEX_H_DECLARE_FUNC( int32,			DrawImageScaleFromFileFramesInternal,		(int32 x1, int32 y1, int32 x2, int32 y2, const char *img_name,uint32 frame_no) )

#endif

#if MEX_SDK_VER >= 1015
MEX_H_DECLARE_FUNC( int32,			SetROCache,				    (uint32 start_addr, uint32 total_size, uint32 *cache_size) )
MEX_H_DECLARE_FUNC( bool,			ClearROCache,				(int32 cache_id) )
#endif
    
#if MEX_SDK_VER >= 1017
MEX_H_DECLARE_FUNC( void,			SetSystemStatusIcon_ext,				    (int32 icon_id, int32 image_id) )
MEX_H_DECLARE_FUNC( void,			SetSystemStatusIconStatus_ext,				(int32 icon_id, bool bShow) )
MEX_H_DECLARE_FUNC(	int,			Rgb565ToYuv420,								(unsigned int *srcYUV, 
								  												unsigned char *dstY, unsigned char *dstU, unsigned char *dstV, 
								  												int width, int height) )

#endif

#if MEX_SDK_VER >= 1018
MEX_H_DECLARE_FUNC( bool, 			SetDateTime,				(Mex_DateTime DateTime) )
#endif
#if MEX_SDK_VER >= 1024
MEX_H_DECLARE_FUNC( int32,			LaunchAppExByAdm,					(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam,int32 iAdmId) )
MEX_H_DECLARE_FUNC( void,			FreeAppByAdm,						(int32 iAppId,int32 iAdmId) )
MEX_H_DECLARE_FUNC( int32,			GetAppMaxMemSize,		(const wchar_t*, const wchar_t*, bool) )
MEX_H_DECLARE_FUNC( int32,			GetLeftMemSize,		(E_Mex_MemoryPool) )
MEX_H_DECLARE_FUNC( bool,			IsUsingReservedMem,		(int32) )
MEX_H_DECLARE_FUNC( bool,			IsKernalApp,		(const wchar_t*, const wchar_t*, bool) )
MEX_H_DECLARE_FUNC( bool,			IsAppInternal,		(int32) )
MEX_H_DECLARE_FUNC( bool,			SetWidgetTop,		(int32 iAppId, bool bTop) )
MEX_H_DECLARE_FUNC( bool,			SetWidgetVisiable,		(int32 iAppId, bool bVisiable) )
MEX_H_DECLARE_FUNC( void*,			GetGdiLayer2Buf,		(int32*) )
MEX_H_DECLARE_FUNC( void,			ReturnGdiLayer2Buf,		(void) )
MEX_H_DECLARE_FUNC( bool,			SetAppPriority,		(int32 iAppId, int32 iPriority) )
MEX_H_DECLARE_FUNC( bool,			AllowAcceptKey,		(int32 iAppId, bool bAcccept) )
MEX_H_DECLARE_FUNC( bool,			GetAppFrameBuffer,		(int32 iAppId, uint16 **ppBuffer, int32 * pWidth, int32 * pHeight) )
MEX_H_DECLARE_FUNC( bool,			SetWidgetFullModeMemSize,		(int32 iAppId, uint32 size) )
MEX_H_DECLARE_FUNC( void*,			GetDeskAppImgPro,		(void) )
MEX_H_DECLARE_FUNC( int32,			SetManCurAppId,		(int32) )	
#endif
#if MEX_SDK_VER >= 1028
MEX_H_DECLARE_FUNC( E_Mex_RunMode,	GetRunModeFromAppId, 	(int32) )	
MEX_H_DECLARE_FUNC( int32,			AppIsInMemory,	(int32) )	
#endif

// gui control macro
#if defined(MEX_TARGET_SERVER)
#define GUI_BEGIN(dstBuf, dstW, dstH)			{  Mex_GuiBuffer __currBuffer;							 \
												   __currBuffer.size = sizeof(mex_color) * dstW * dstH;  \
												   __currBuffer.width = dstW;                            \
												   __currBuffer.height = dstH;                           \
												   __currBuffer.clipx1 = 0;                              \
												   __currBuffer.clipy1 = 0;                              \
												   __currBuffer.clipx2 = dstW - 1;                       \
												   __currBuffer.clipy2 = dstH - 1;                       \
												   __currBuffer.buffer = dstBuf;                         \
												   __currBuffer.vcf = MEX_COLOR_FORMAT;                  \
												   __currBuffer = mex_SelectGuiBuffer(&__currBuffer);
#define GUI_END()                                  mex_SelectGuiBuffer(&__currBuffer);                   \
												}
#elif defined(MEX_TARGET_CLIENT)
#define GUI_BEGIN(dstBuf, dstW, dstH)			{  Mex_GuiBuffer __currBuffer;							 \
												   __currBuffer.size = sizeof(mex_color) * dstW * dstH;  \
												   __currBuffer.width = dstW;                            \
												   __currBuffer.height = dstH;                           \
												   __currBuffer.clipx1 = 0;                              \
												   __currBuffer.clipy1 = 0;                              \
												   __currBuffer.clipx2 = dstW - 1;                       \
												   __currBuffer.clipy2 = dstH - 1;                       \
												   __currBuffer.buffer = dstBuf;                         \
												   __currBuffer.vcf = MEX_COLOR_FORMAT;                  \
												   __currBuffer = SelectGuiBuffer(&__currBuffer);       
#define GUI_END()                                  SelectGuiBuffer(&__currBuffer);                       \
												}
#endif
// 原GUI库部分函数 end


#if defined(MEX_TARGET_SERVER)

#if defined(MEX_PLATFORM_MTK) || defined(MEX_PLATFORM_SPREAD)
#define RUN_IN_SRAM( functype, funcname, funcargs, funcsize )	\
					{	\
						int32 _iAlign = ((int32)funcname) & 3;	\
						void *_pSramFunc = mex_smalloc( 600 + funcsize );	\
						if (_pSramFunc != NULL)	\
						{	\
							void *_func = (void*)((int32)_pSramFunc + 600 + _iAlign);	\
							mex_memcpy( (void*)((int32)_pSramFunc + 600), (void*)((int32)funcname - _iAlign), funcsize );	\
							((functype)(_func))##funcargs;	\
							mex_sfree( _pSramFunc );	\
						}	\
						else	\
						{	\
							funcname##funcargs;	\
						}	\
					}
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_SIMULATOR)
#define RUN_IN_SRAM( functype, funcname, funcargs, funcsize )	\
					funcname##funcargs;
#endif

#elif defined(MEX_TARGET_CLIENT)

#if defined(MEX_PLATFORM_MTK) || defined(MEX_PLATFORM_SPREAD)
#define RUN_IN_SRAM( functype, funcname, funcargs, funcsize )	\
					{	\
						int32 _iAlign = ((int32)funcname) & 3;	\
						void *_pSramFunc = smalloc( 600 + funcsize );	\
						if (_pSramFunc != NULL)	\
						{	\
							void *_func = (void*)((int32)_pSramFunc + 600 + _iAlign);	\
							memcpy( (void*)((int32)_pSramFunc + 600), (void*)((int32)funcname - _iAlign), funcsize );	\
							((functype)(_func))##funcargs;	\
							sfree( _pSramFunc );	\
						}	\
						else	\
						{	\
							funcname##funcargs;	\
						}	\
					}
#elif defined(MEX_PLATFORM_MODIS) || defined(MEX_PLATFORM_SIMULATOR)
#define RUN_IN_SRAM( functype, funcname, funcargs, funcsize )	\
					funcname##funcargs;
#endif

#endif


MEX_H_ENUM_FUNC_START_OS
	MEX_H_ENUM_FUNC_OS( ExchangeLibPtrPtr )
	MEX_H_ENUM_FUNC_OS( GetLaunchParam )
	MEX_H_ENUM_FUNC_OS( LaunchAppEx )
	MEX_H_ENUM_FUNC_OS( LaunchApp )
	MEX_H_ENUM_FUNC_OS( RegisterLaunchAppAfterExit )
	MEX_H_ENUM_FUNC_OS( FreeApp )
	MEX_H_ENUM_FUNC_OS( RegisterMsgHandler )
	MEX_H_ENUM_FUNC_OS( RegisterExitFunc )
	MEX_H_ENUM_FUNC_OS( GetMsgHandler )
	MEX_H_ENUM_FUNC_OS( Exit )

	MEX_H_ENUM_FUNC_OS( LoadAppIntoSRam )
	MEX_H_ENUM_FUNC_OS( FreeAppFromSRam )
	MEX_H_ENUM_FUNC_OS( LoadLibIntoSRam )
	MEX_H_ENUM_FUNC_OS( FreeLibFromSRam )

	MEX_H_ENUM_FUNC_OS( GetCurrentApp )
	MEX_H_ENUM_FUNC_OS( SetCurrentApp )
	
	MEX_H_ENUM_FUNC_OS( GetSystemRootDir )
	MEX_H_ENUM_FUNC_OS( GetAppRootDir )
	MEX_H_ENUM_FUNC_OS( GetAppDir )
	
	MEX_H_ENUM_FUNC_OS( GetCurAppDir )
	MEX_H_ENUM_FUNC_OS( GetCurAppFileName )

	MEX_H_ENUM_FUNC_OS( BeginPaint )
	MEX_H_ENUM_FUNC_OS( EndPaint )
	MEX_H_ENUM_FUNC_OS( Invalidate )

	MEX_H_ENUM_FUNC_OS( GetFreeMemory )
	MEX_H_ENUM_FUNC_OS( GetAppAttribute )
	MEX_H_ENUM_FUNC_OS( SetAppAttribute )
	MEX_H_ENUM_FUNC_OS( PauseAppTimer )

	MEX_H_ENUM_FUNC_OS( GetScreenLayerBuffer )
	MEX_H_ENUM_FUNC_OS( DisableDbBufferBlt )
	MEX_H_ENUM_FUNC_OS( GetFullScreenBuffer )
	MEX_H_ENUM_FUNC_OS( GetAppScreenBuffer )
	MEX_H_ENUM_FUNC_OS( RegisterScreenBuffer )
	MEX_H_ENUM_FUNC_OS( UnRegisterScreenBuffer )
	
	MEX_H_ENUM_FUNC_OS( GetAppScreenKeyColor )
	MEX_H_ENUM_FUNC_OS( RegisterScreenKeyColor )
	
	MEX_H_ENUM_FUNC_OS( GetAppScreenImage )

	MEX_H_ENUM_FUNC_OS( SetTimer )
	MEX_H_ENUM_FUNC_OS( KillTimer )
	MEX_H_ENUM_FUNC_OS( SetTimerInterval )

	MEX_H_ENUM_FUNC_OS( gettime )

	MEX_H_ENUM_FUNC_OS( GetNextAppId )
	MEX_H_ENUM_FUNC_OS( GetAppInfoFromId )
	MEX_H_ENUM_FUNC_OS( GetAppInfoFromFile )
	MEX_H_ENUM_FUNC_OS( GetAppInfoFromFile_ext )

	MEX_H_ENUM_FUNC_OS( GetRunMode )
	MEX_H_ENUM_FUNC_OS( SwitchRunMode )
	MEX_H_ENUM_FUNC_OS( GetRegistry )
	MEX_H_ENUM_FUNC_OS( SendMessage )

	MEX_H_ENUM_FUNC_OS( GetPhoneConfig )
	MEX_H_ENUM_FUNC_OS( ConvertSysDef )
	MEX_H_ENUM_FUNC_OS( GetDateTime )

	MEX_H_ENUM_FUNC_OS( fopenw_org )
	MEX_H_ENUM_FUNC_OS( fopenw_ext )
	MEX_H_ENUM_FUNC_OS( fopenw )
	MEX_H_ENUM_FUNC_OS( fopen )
	MEX_H_ENUM_FUNC_OS( fseek )
	MEX_H_ENUM_FUNC_OS( ftell )
	MEX_H_ENUM_FUNC_OS( fclose )	
	MEX_H_ENUM_FUNC_OS( GetFileSize )
	MEX_H_ENUM_FUNC_OS( fread )
	MEX_H_ENUM_FUNC_OS( fwrite )
	MEX_H_ENUM_FUNC_OS( fflush )
	MEX_H_ENUM_FUNC_OS( remove )
	MEX_H_ENUM_FUNC_OS( remove_ext )
	MEX_H_ENUM_FUNC_OS( rename )
	MEX_H_ENUM_FUNC_OS( rename_ext )
	MEX_H_ENUM_FUNC_OS( CreateDir )
	MEX_H_ENUM_FUNC_OS( CreateDir_ext )
	MEX_H_ENUM_FUNC_OS( RemoveDir )
	MEX_H_ENUM_FUNC_OS( RemoveDir_ext )
	MEX_H_ENUM_FUNC_OS( IsDirExist_ext )

	MEX_H_ENUM_FUNC_OS( FindFirst )
	MEX_H_ENUM_FUNC_OS( FindDirFirst )
	MEX_H_ENUM_FUNC_OS( FindNext )
	MEX_H_ENUM_FUNC_OS( FindClose )
	MEX_H_ENUM_FUNC_OS( log )
	MEX_H_ENUM_FUNC_OS( getlogfilter )
	MEX_H_ENUM_FUNC_OS( setlogfilter )

	MEX_H_ENUM_FUNC_OS( StartProfiling )
	MEX_H_ENUM_FUNC_OS( AddProfilingData )

	MEX_H_ENUM_FUNC_OS( GetKeyState )
	MEX_H_ENUM_FUNC_OS( SetKeyState )

	MEX_H_ENUM_FUNC_OS( VMalloc )
	MEX_H_ENUM_FUNC_OS( VFree )
	MEX_H_ENUM_FUNC_OS( VMemcpy )
	MEX_H_ENUM_FUNC_OS( VToMem )
	MEX_H_ENUM_FUNC_OS( VPMalloc )
	MEX_H_ENUM_FUNC_OS( VPFree )

	MEX_H_ENUM_FUNC_OS( SetUseReservedMem )

	MEX_H_ENUM_FUNC_OS( StartSystemTimer )
	MEX_H_ENUM_FUNC_OS( KillSystemTimer )
	MEX_H_ENUM_FUNC_OS( SetSystemTimerInterval )

	MEX_H_ENUM_FUNC_OS( RegisterLaunchOnScreen )
	MEX_H_ENUM_FUNC_OS( RegisterLaunchOnStartup )
	MEX_H_ENUM_FUNC_OS( GetLastFullScreenBuffer )

	MEX_H_ENUM_FUNC_OS( InputBox_Create )
	
	MEX_H_ENUM_FUNC_OS( PaintAllPause )
	MEX_H_ENUM_FUNC_OS( PaintAllResume )

	MEX_H_ENUM_FUNC_OS( zip_uncompress )
	MEX_H_ENUM_FUNC_OS( DisablePaintOpt )

	MEX_H_ENUM_FUNC_OS( smalloc )
	MEX_H_ENUM_FUNC_OS( sfree )
	MEX_H_ENUM_FUNC_OS( NotifyLockSRam )
	MEX_H_ENUM_FUNC_OS( NotifyUnlockSRam )

	MEX_H_ENUM_FUNC_OS( ShutdownPlatform )

	MEX_H_ENUM_FUNC_OS( RegisterLaunchOnFullAppExit )
	MEX_H_ENUM_FUNC_OS( SeparateAbsPath )

	MEX_H_ENUM_FUNC_OS( CreateDir_ext_recur )

	MEX_H_ENUM_FUNC_OS( ExitPlatform )

	MEX_H_ENUM_FUNC_OS( GetAppRegistSizeFromId )
	MEX_H_ENUM_FUNC_OS( GetAppRegistSizeFromFile )

	MEX_H_ENUM_FUNC_OS( GetCurrentTime )
	MEX_H_ENUM_FUNC_OS( UtcSec2DateTime )

	MEX_H_ENUM_FUNC_OS( xmalloc )
	MEX_H_ENUM_FUNC_OS( xfree )

	MEX_H_ENUM_FUNC_OS( DisableQuitControl )

	MEX_H_ENUM_FUNC_OS( GetAppExtraInfoFromId )
	MEX_H_ENUM_FUNC_OS( GetAppExtraInfoFromFile )

	MEX_H_ENUM_FUNC_OS( GetSystemParam )
	MEX_H_ENUM_FUNC_OS( Sleep )

	MEX_H_ENUM_FUNC_OS( RegisterLaunchSystemApp )

	MEX_H_ENUM_FUNC_OS( GetFileAttributes_ext )
	MEX_H_ENUM_FUNC_OS( SetFileAttributes_ext )
	MEX_H_ENUM_FUNC_OS( GetFileAttributes )
	MEX_H_ENUM_FUNC_OS( SetFileAttributes )	

	// 原GUI库部分函数 begin ////////////////////////////////////
	MEX_H_ENUM_FUNC_OS( SelectGuiBuffer )
	MEX_H_ENUM_FUNC_OS( GetBufferClipper )
	MEX_H_ENUM_FUNC_OS( SetBufferClipper )
	MEX_H_ENUM_FUNC_OS( SetBufferSubClipper )

	MEX_H_ENUM_FUNC_OS( GetCharDimension )
	MEX_H_ENUM_FUNC_OS( GetStringDimension )
	MEX_H_ENUM_FUNC_OS( GetStringSpacingDimension )
	MEX_H_ENUM_FUNC_OS( GetStringExDimension )
	MEX_H_ENUM_FUNC_OS( GetStringExSpacingDimension )
	MEX_H_ENUM_FUNC_OS( DrawString )
	MEX_H_ENUM_FUNC_OS( DrawStringSpacing )
	MEX_H_ENUM_FUNC_OS( DrawStringEx )
	MEX_H_ENUM_FUNC_OS( DrawStringExSpacing )
	MEX_H_ENUM_FUNC_OS( DrawChar )
	MEX_H_ENUM_FUNC_OS( GetCharData )

	MEX_H_ENUM_FUNC_OS( DrawPointInternal )
	MEX_H_ENUM_FUNC_OS( DrawLineInternal )
	MEX_H_ENUM_FUNC_OS( DrawFrameRectInternal )
	MEX_H_ENUM_FUNC_OS( DrawFillRectInternal )
	MEX_H_ENUM_FUNC_OS( DrawFrameCircleInternal )
	MEX_H_ENUM_FUNC_OS( DrawFillCircleInternal )

	MEX_H_ENUM_FUNC_OS( GetImageTypeFromMemInternal )
	MEX_H_ENUM_FUNC_OS( GetImageTypeFromFileInternal )
	MEX_H_ENUM_FUNC_OS( GetImageDimensionFromMemInternal )
	MEX_H_ENUM_FUNC_OS( GetImageDimensionFromFileInternal )
	MEX_H_ENUM_FUNC_OS( GetGifImageFrameCountFromMemInternal )
	MEX_H_ENUM_FUNC_OS( GetGifImageFrameCountFromFileInternal )
	MEX_H_ENUM_FUNC_OS( DrawImageFromMemInternal )
	MEX_H_ENUM_FUNC_OS( DrawImageFromFileInternal )
	MEX_H_ENUM_FUNC_OS( DrawImageFromMemFramesInternal )
	MEX_H_ENUM_FUNC_OS( DrawImageFromFileFramesInternal )
	// 原GUI库部分函数 end ////////////////////////////////////

	MEX_H_ENUM_FUNC_OS( GetAppParam )

	MEX_H_ENUM_FUNC_OS( GetAdmFunctions )
	
	MEX_H_ENUM_FUNC_OS( IsAppPermissiveToDo )
	
	MEX_H_ENUM_FUNC_OS( GetActScreenBuffer )

	MEX_H_ENUM_FUNC_OS( UnCompressDataFromMemory )
	MEX_H_ENUM_FUNC_OS( UnCompressDataFromFile )

	MEX_H_ENUM_FUNC_OS( CallManagerFunction )

	MEX_H_ENUM_FUNC_OS( RegisterSimulateKey )

	MEX_H_ENUM_FUNC_OS( StartThreadTimer )
	MEX_H_ENUM_FUNC_OS( KillThreadTimer )
	MEX_H_ENUM_FUNC_OS( SetThreadTimerInterval )

	MEX_H_ENUM_FUNC_OS( TakeMutex )
	MEX_H_ENUM_FUNC_OS( GiveMutex )
	
	MEX_H_ENUM_FUNC_OS( GetAppPosition )
	MEX_H_ENUM_FUNC_OS( SetAppPosition )

	MEX_H_ENUM_FUNC_OS( RegSystemStatusIcon )
	MEX_H_ENUM_FUNC_OS( UnRegSystemStatusIcon )
	MEX_H_ENUM_FUNC_OS( SetSystemStatusIcon )
	MEX_H_ENUM_FUNC_OS( GetSystemStatusIcon )
	MEX_H_ENUM_FUNC_OS( SetSystemStatusIconStatus )

#if MEX_SDK_VER >= 1001
	MEX_H_ENUM_FUNC_OS( GetScreenLayerBufferWithInfo )
#endif

#if MEX_SDK_VER >= 1004
	MEX_H_ENUM_FUNC_OS( tmalloc )
	MEX_H_ENUM_FUNC_OS( tfree )
#endif


#if MEX_SDK_VER >= 1009
	MEX_H_ENUM_FUNC_OS( Fmgr_Create )
	MEX_H_ENUM_FUNC_OS( Fmgr_Cancel )
	MEX_H_ENUM_FUNC_OS( Fmgr_SetFilter )
	MEX_H_ENUM_FUNC_OS( Fmgr_Show )
#endif

#if MEX_SDK_VER >= 1010
	MEX_H_ENUM_FUNC_OS( Pen_Block )
	MEX_H_ENUM_FUNC_OS( Pen_unBlock )
	MEX_H_ENUM_FUNC_OS( KeyPad_Lock )
	MEX_H_ENUM_FUNC_OS( KeyPad_unLock )
	MEX_H_ENUM_FUNC_OS( ReStartKeyPadLock )
#endif

#if MEX_SDK_VER >= 1011
	MEX_H_ENUM_FUNC_OS( DrawImageScaleFromFileInternal )
	MEX_H_ENUM_FUNC_OS( DrawImageScaleFromFileFramesInternal )
#endif
#if MEX_SDK_VER >= 1015
	MEX_H_ENUM_FUNC_OS( SetROCache )
	MEX_H_ENUM_FUNC_OS( ClearROCache )
#endif
#if MEX_SDK_VER >= 1017
	MEX_H_ENUM_FUNC_OS( SetSystemStatusIcon_ext )
	MEX_H_ENUM_FUNC_OS( SetSystemStatusIconStatus_ext )
	MEX_H_ENUM_FUNC_OS( Rgb565ToYuv420 )
#endif

#if MEX_SDK_VER >= 1018
	MEX_H_ENUM_FUNC_OS( SetDateTime )
#endif
#if MEX_SDK_VER >= 1024
	MEX_H_ENUM_FUNC_OS( LaunchAppExByAdm )
	MEX_H_ENUM_FUNC_OS( FreeAppByAdm )
	MEX_H_ENUM_FUNC_OS( GetAppMaxMemSize )
	MEX_H_ENUM_FUNC_OS( GetLeftMemSize )
	MEX_H_ENUM_FUNC_OS( IsUsingReservedMem )
	MEX_H_ENUM_FUNC_OS( IsKernalApp )
	MEX_H_ENUM_FUNC_OS( IsAppInternal )
	MEX_H_ENUM_FUNC_OS( SetWidgetTop )
	MEX_H_ENUM_FUNC_OS( SetWidgetVisiable )
	MEX_H_ENUM_FUNC_OS( GetGdiLayer2Buf )
	MEX_H_ENUM_FUNC_OS( ReturnGdiLayer2Buf )
	MEX_H_ENUM_FUNC_OS( SetAppPriority )
	MEX_H_ENUM_FUNC_OS( AllowAcceptKey )
	MEX_H_ENUM_FUNC_OS( GetAppFrameBuffer )
	MEX_H_ENUM_FUNC_OS( SetWidgetFullModeMemSize )
	MEX_H_ENUM_FUNC_OS( GetDeskAppImgPro )
	MEX_H_ENUM_FUNC_OS( SetManCurAppId )	
#endif
#if MEX_SDK_VER >= 1028
	MEX_H_ENUM_FUNC_OS( GetRunModeFromAppId )	
	MEX_H_ENUM_FUNC_OS( AppIsInMemory )	
#endif

MEX_H_ENUM_FUNC_END_OS


#if defined (MEX_TARGET_CLIENT)
	#define ExchangeLibPtrPtr		MEX_H_GET_FUNC_DEFINE_OS( ExchangeLibPtrPtr )	//(int32 iLibId, int32 iNbFuncsInLib)
	#define GetLaunchParam			MEX_H_GET_FUNC_DEFINE_OS( GetLaunchParam )		//(void)
	#define LaunchAppEx				MEX_H_GET_FUNC_DEFINE_OS( LaunchAppEx )			//(const wchar_t *szAppName, const wchar_t *szFilePath, bool bInternal, bool bInSystemDir, const char *szParam)
	#define LaunchApp				MEX_H_GET_FUNC_DEFINE_OS( LaunchApp )			//(const wchar_t *szAppName, const wchar_t *szFilePath, bool bInternal, bool bInSystemDir)
	#define RegisterLaunchAppAfterExit		MEX_H_GET_FUNC_DEFINE_OS( RegisterLaunchAppAfterExit )		//(bool bEnable, const wchar_t *szAppName, const wchar_t *szFilePath, bool bInternal, bool bInSystemDir)
	#define FreeApp					MEX_H_GET_FUNC_DEFINE_OS( FreeApp )				//(int32 iAppId)
	#define RegisterMsgHandler		MEX_H_GET_FUNC_DEFINE_OS( RegisterMsgHandler )	//(int32 iAppId, MexApp_MsgHandler_Func pFunc)
	#define RegisterExitFunc		MEX_H_GET_FUNC_DEFINE_OS( RegisterExitFunc )	//(int32 iAppId, MexApp_Exit_Func pFunc, bool bAmILib)
	#define GetMsgHandler			MEX_H_GET_FUNC_DEFINE_OS( GetMsgHandler )		//(int32 iAppId)
	#define Exit					MEX_H_GET_FUNC_DEFINE_OS( Exit )				//(void)

	#define LoadAppIntoSRam			MEX_H_GET_FUNC_DEFINE_OS( LoadAppIntoSRam )		//(int32 iAppId)
	#define FreeAppFromSRam			MEX_H_GET_FUNC_DEFINE_OS( FreeAppFromSRam )		//(int32 iAppId)
	#define LoadLibIntoSRam			MEX_H_GET_FUNC_DEFINE_OS( LoadLibIntoSRam )		//(funcptr pLibFuncPtr)
	#define FreeLibFromSRam			MEX_H_GET_FUNC_DEFINE_OS( FreeLibFromSRam )		//(funcptr pLibFuncPtr)
	
	#define GetCurrentApp			MEX_H_GET_FUNC_DEFINE_OS( GetCurrentApp )		//(void)
	#define SetCurrentApp			MEX_H_GET_FUNC_DEFINE_OS( SetCurrentApp )		//(int32 iAppId)
	#define GetSystemRootDir		MEX_H_GET_FUNC_DEFINE_OS( GetSystemRootDir )	//(void)
	#define GetAppRootDir			MEX_H_GET_FUNC_DEFINE_OS( GetAppRootDir )		//(void)
	#define GetAppDir				MEX_H_GET_FUNC_DEFINE_OS( GetAppDir )			//(const wchar_t *szAppFileName)

	#define GetCurAppDir			MEX_H_GET_FUNC_DEFINE_OS( GetCurAppDir )		//(wchar_t* curAppDir)
	#define GetCurAppFileName		MEX_H_GET_FUNC_DEFINE_OS( GetCurAppFileName )	//(wchar_t* curAppFileName)
	
	#define BeginPaint				MEX_H_GET_FUNC_DEFINE_OS( BeginPaint )			//(void)
	#define EndPaint				MEX_H_GET_FUNC_DEFINE_OS( EndPaint )			//(void)
	#define Invalidate				MEX_H_GET_FUNC_DEFINE_OS( Invalidate )			//(void)

	#define GetFreeMemory			MEX_H_GET_FUNC_DEFINE_OS( GetFreeMemory )		//(E_Mex_MemoryPool ePool)
	#define GetAppAttribute			MEX_H_GET_FUNC_DEFINE_OS( GetAppAttribute )		//(int32 iAppId)
	#define SetAppAttribute			MEX_H_GET_FUNC_DEFINE_OS( SetAppAttribute )		//(int32 iAppId, int32 iAttribute)
	#define PauseAppTimer			MEX_H_GET_FUNC_DEFINE_OS( PauseAppTimer )		//(int32 iAppId, int32 iTimerId, bool bPause)

	#define GetScreenLayerBuffer	MEX_H_GET_FUNC_DEFINE_OS( GetScreenLayerBuffer )	//(int32 iLayer)
	#define DisableDbBufferBlt		MEX_H_GET_FUNC_DEFINE_OS( DisableDbBufferBlt )		//(bool bDisable)
	#define GetFullScreenBuffer		MEX_H_GET_FUNC_DEFINE_OS( GetFullScreenBuffer )		//(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight)
	#define GetAppScreenBuffer		MEX_H_GET_FUNC_DEFINE_OS( GetAppScreenBuffer )		//(int32 iAppId, uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight)
	#define RegisterScreenBuffer	MEX_H_GET_FUNC_DEFINE_OS( RegisterScreenBuffer )	//(uint16 *pBuffer, uint16 uWidth, uint16 uHeight)
	#define UnRegisterScreenBuffer	MEX_H_GET_FUNC_DEFINE_OS( UnRegisterScreenBuffer )	//(void)

	#define GetAppScreenKeyColor	MEX_H_GET_FUNC_DEFINE_OS( GetAppScreenKeyColor )	
	#define RegisterScreenKeyColor	MEX_H_GET_FUNC_DEFINE_OS( RegisterScreenKeyColor )	
	
	#define GetAppScreenImage		MEX_H_GET_FUNC_DEFINE_OS( GetAppScreenImage )	

	#define SetTimer				MEX_H_GET_FUNC_DEFINE_OS( SetTimer )				//(int32 iTime, void* pParam)
	#define KillTimer				MEX_H_GET_FUNC_DEFINE_OS( KillTimer )				//(int32 iTimerId)
	#define SetTimerInterval		MEX_H_GET_FUNC_DEFINE_OS( SetTimerInterval )		//(int32 iAppId, int32 iTimerId, int32 iNewTime, int32 *pOldTime)

	#define gettime					MEX_H_GET_FUNC_DEFINE_OS( gettime )					//(void)

	#define GetNextAppId			MEX_H_GET_FUNC_DEFINE_OS( GetNextAppId )			//(int32 iCurAppId)
	#define GetAppInfoFromId		MEX_H_GET_FUNC_DEFINE_OS( GetAppInfoFromId )		//(int32 iCurAppId, int32 *piAppType, int16 *piAppVersion, wchar_t **ppAppName, wchar_t **ppFileName)
	#define GetAppInfoFromFile		MEX_H_GET_FUNC_DEFINE_OS( GetAppInfoFromFile )		//(const wchar_t *szFile, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion)
	#define GetAppInfoFromFile_ext	MEX_H_GET_FUNC_DEFINE_OS( GetAppInfoFromFile_ext )	//(const wchar_t *szFullFileName, int32 *piAppType, int16 *piAppVersion)

	#define GetRunMode				MEX_H_GET_FUNC_DEFINE_OS( GetRunMode )				//(void)
	#define SwitchRunMode			MEX_H_GET_FUNC_DEFINE_OS( SwitchRunMode )			//(E_Mex_RunMode newMode)
	#define GetRegistry				MEX_H_GET_FUNC_DEFINE_OS( GetRegistry )				//(void)
	#define SendMessage				MEX_H_GET_FUNC_DEFINE_OS( SendMessage )				//(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMessage, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3)

	#define GetPhoneConfig			MEX_H_GET_FUNC_DEFINE_OS( GetPhoneConfig )		//(E_Mex_Phone_Config cfg)
	#define ConvertSysDef			MEX_H_GET_FUNC_DEFINE_OS( ConvertSysDef )

	#define GetDateTime				MEX_H_GET_FUNC_DEFINE_OS( GetDateTime )			//(Mex_DateTime *pDt)

	#define fopenw_org				MEX_H_GET_FUNC_DEFINE_OS( fopenw_org )			//(const wchar_t *filename, const wchar_t *mode)
	#define fopenw_ext				MEX_H_GET_FUNC_DEFINE_OS( fopenw_ext )			//(const wchar_t *filename, const wchar_t *mode, const wchar_t *path, const wchar_t *mexFileName, bool bCheckInsideMexOnly)
	#define fopenw					MEX_H_GET_FUNC_DEFINE_OS( fopenw ) 				//(const wchar_t *filename, const wchar_t *mode)
	#define fopen					MEX_H_GET_FUNC_DEFINE_OS( fopen )				//(const char *filename, const char *mode)
	#define fseek					MEX_H_GET_FUNC_DEFINE_OS( fseek )				//(int32* handle, long offset, int32 whence)
	#define ftell					MEX_H_GET_FUNC_DEFINE_OS( ftell )				//(int32* handle)
	#define fclose					MEX_H_GET_FUNC_DEFINE_OS( fclose ) 				//(int32* handle)
	#define GetFileSize				MEX_H_GET_FUNC_DEFINE_OS( GetFileSize )			//(int32* handle)
	#define fread					MEX_H_GET_FUNC_DEFINE_OS( fread )				//(void *ptr, size_t size, size_t nitems, int32* handle)
	#define fwrite					MEX_H_GET_FUNC_DEFINE_OS( fwrite ) 				//(const void *ptr, size_t size, size_t nitems, int32* handle)
	#define fflush					MEX_H_GET_FUNC_DEFINE_OS( fflush ) 				//(int32* handle)
	#define remove					MEX_H_GET_FUNC_DEFINE_OS( remove ) 				//(const wchar_t *file)
	#define remove_ext				MEX_H_GET_FUNC_DEFINE_OS( remove_ext )			//(const wchar_t *file)
	#define rename					MEX_H_GET_FUNC_DEFINE_OS( rename ) 				//(const wchar_t *ofile, const wchar_t *nfile)
	#define rename_ext				MEX_H_GET_FUNC_DEFINE_OS( rename_ext )			//(const wchar_t *ofile, const wchar_t *nfile)

	#define CreateDir				MEX_H_GET_FUNC_DEFINE_OS( CreateDir )			//(const wchar_t *dir) 
	#define CreateDir_ext			MEX_H_GET_FUNC_DEFINE_OS( CreateDir_ext )		//(const wchar_t *dir) 
	#define RemoveDir				MEX_H_GET_FUNC_DEFINE_OS( RemoveDir )			//(const wchar_t *dir) 
	#define RemoveDir_ext			MEX_H_GET_FUNC_DEFINE_OS( RemoveDir_ext )		//(const wchar_t *dir) 
	#define IsDirExist_ext			MEX_H_GET_FUNC_DEFINE_OS( IsDirExist_ext )		//(const wchar_t *dir) 

	#define FindFirst				MEX_H_GET_FUNC_DEFINE_OS( FindFirst )			//(const wchar_t *path, const wchar_t *namePattern, const wchar_t *fileName, uint32 maxLength)
	#define FindDirFirst			MEX_H_GET_FUNC_DEFINE_OS( FindDirFirst )		//(const wchar_t *path, const wchar_t *fileName, uint32 maxLength)
	#define FindNext				MEX_H_GET_FUNC_DEFINE_OS( FindNext )			//(int32 findHandle, const wchar_t *fileName, uint32 maxLength)
	#define FindClose				MEX_H_GET_FUNC_DEFINE_OS( FindClose )			//(int32 findHandle)

	#define log 					MEX_H_GET_FUNC_DEFINE_OS( log )					//(int32 iFilter, const char *pText, ...)
	#define getlogfilter			MEX_H_GET_FUNC_DEFINE_OS( getlogfilter )		//(void)
	#define setlogfilter			MEX_H_GET_FUNC_DEFINE_OS( setlogfilter )		//(int32 iFilter)
	
	#define StartProfiling			MEX_H_GET_FUNC_DEFINE_OS( StartProfiling )		//(bool bRestart, int32 iNbFrames)
	#define AddProfilingData		MEX_H_GET_FUNC_DEFINE_OS( AddProfilingData )	//(bool bIsStart, int16 *piLevel, const char *pText)

	#define GetKeyState				MEX_H_GET_FUNC_DEFINE_OS( GetKeyState )			//(E_Mex_KeyCode keyCode)
	#define SetKeyState				MEX_H_GET_FUNC_DEFINE_OS( SetKeyState )			//(E_Mex_KeyEvent keyEvent, E_Mex_KeyCode keyCode)

	#define VMalloc					MEX_H_GET_FUNC_DEFINE_OS( VMalloc )
	#define VFree					MEX_H_GET_FUNC_DEFINE_OS( VFree )
	#define VMemcpy					MEX_H_GET_FUNC_DEFINE_OS( VMemcpy )
	#define VToMem					MEX_H_GET_FUNC_DEFINE_OS( VToMem )
	#define VPMalloc				MEX_H_GET_FUNC_DEFINE_OS( VPMalloc )
	#define VPFree					MEX_H_GET_FUNC_DEFINE_OS( VPFree )

	#define SetUseReservedMem		MEX_H_GET_FUNC_DEFINE_OS( SetUseReservedMem )

	#define StartSystemTimer 		MEX_H_GET_FUNC_DEFINE_OS( StartSystemTimer ) 			//(int32 iTime, MexFuncPtr func)
	#define KillSystemTimer			MEX_H_GET_FUNC_DEFINE_OS( KillSystemTimer )				//(int32 iTimeId)
	#define SetSystemTimerInterval	MEX_H_GET_FUNC_DEFINE_OS( SetSystemTimerInterval )		//(int32 iTimerId, int32 iNewTime, int32 *pOldTime)

	#define RegisterLaunchOnScreen	MEX_H_GET_FUNC_DEFINE_OS( RegisterLaunchOnScreen )
	#define RegisterLaunchOnStartup	MEX_H_GET_FUNC_DEFINE_OS( RegisterLaunchOnStartup )
	#define GetLastFullScreenBuffer	MEX_H_GET_FUNC_DEFINE_OS( GetLastFullScreenBuffer )		//(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight)

	#define InputBox_Create			MEX_H_GET_FUNC_DEFINE_OS( InputBox_Create ) 

	#define PaintAllPause 			MEX_H_GET_FUNC_DEFINE_OS( PaintAllPause ) 
	#define PaintAllResume 			MEX_H_GET_FUNC_DEFINE_OS( PaintAllResume ) 

	#define zip_uncompress 			MEX_H_GET_FUNC_DEFINE_OS( zip_uncompress ) 
	#define DisablePaintOpt			MEX_H_GET_FUNC_DEFINE_OS( DisablePaintOpt ) 

	#define smalloc					MEX_H_GET_FUNC_DEFINE_OS( smalloc ) 
	#define sfree					MEX_H_GET_FUNC_DEFINE_OS( sfree ) 
	#define NotifyLockSRam			MEX_H_GET_FUNC_DEFINE_OS( NotifyLockSRam ) 
	#define NotifyUnlockSRam		MEX_H_GET_FUNC_DEFINE_OS( NotifyUnlockSRam ) 

	#define ShutdownPlatform		MEX_H_GET_FUNC_DEFINE_OS( ShutdownPlatform ) 

	#define RegisterLaunchOnFullAppExit		MEX_H_GET_FUNC_DEFINE_OS( RegisterLaunchOnFullAppExit ) 
	#define SeparateAbsPath			MEX_H_GET_FUNC_DEFINE_OS( SeparateAbsPath )			//(wchar_t **pathPart, wchar_t **namePart, const wchar_t *path)

	#define CreateDir_ext_recur		MEX_H_GET_FUNC_DEFINE_OS( CreateDir_ext_recur )			//(const wchar_t *szFullDir)

	#define ExitPlatform			MEX_H_GET_FUNC_DEFINE_OS( ExitPlatform )	

	#define GetAppRegistSizeFromId		MEX_H_GET_FUNC_DEFINE_OS( GetAppRegistSizeFromId )	
	#define GetAppRegistSizeFromFile	MEX_H_GET_FUNC_DEFINE_OS( GetAppRegistSizeFromFile )

	#define GetCurrentTime				MEX_H_GET_FUNC_DEFINE_OS( GetCurrentTime )	
	#define UtcSec2DateTime				MEX_H_GET_FUNC_DEFINE_OS( UtcSec2DateTime )	

	#define xmalloc						MEX_H_GET_FUNC_DEFINE_OS( xmalloc )	
	#define xfree						MEX_H_GET_FUNC_DEFINE_OS( xfree )	

	#define DisableQuitControl			MEX_H_GET_FUNC_DEFINE_OS( DisableQuitControl )	

	#define GetAppExtraInfoFromId		MEX_H_GET_FUNC_DEFINE_OS( GetAppExtraInfoFromId )	
	#define GetAppExtraInfoFromFile		MEX_H_GET_FUNC_DEFINE_OS( GetAppExtraInfoFromFile )	

	#define GetSystemParam				MEX_H_GET_FUNC_DEFINE_OS( GetSystemParam )	
	#define Sleep						MEX_H_GET_FUNC_DEFINE_OS( Sleep )	

	#define RegisterLaunchSystemApp		MEX_H_GET_FUNC_DEFINE_OS( RegisterLaunchSystemApp )	

	#define	GetFileAttributes_ext		MEX_H_GET_FUNC_DEFINE_OS( GetFileAttributes_ext )
	#define	SetFileAttributes_ext		MEX_H_GET_FUNC_DEFINE_OS( SetFileAttributes_ext )
	#define GetFileAttributes			MEX_H_GET_FUNC_DEFINE_OS( GetFileAttributes )	
	#define	SetFileAttributes			MEX_H_GET_FUNC_DEFINE_OS( SetFileAttributes )	

	// 原GUI库部分函数 begin ////////////////////////////////////
	#define SelectGuiBuffer					MEX_H_GET_FUNC_DEFINE_OS( SelectGuiBuffer )					// (void *buffer, uint16 width, uint16 height)
	#define GetBufferClipper				MEX_H_GET_FUNC_DEFINE_OS( GetBufferClipper )				// (int16 *x1, int16 *y1, int16 *x2, int16 *y2)
	#define SetBufferClipper				MEX_H_GET_FUNC_DEFINE_OS( SetBufferClipper )				// (int16 x1, int16 y1, int16 x2, int16 y2)
	#define SetBufferSubClipper				MEX_H_GET_FUNC_DEFINE_OS( SetBufferSubClipper )				// (int16 x1, int16 y1, int16 x2, int16 y2)

	#define GetCharDimension				MEX_H_GET_FUNC_DEFINE_OS( GetCharDimension )				// (uint32 ch, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize)
	#define GetStringDimension				MEX_H_GET_FUNC_DEFINE_OS( GetStringDimension )				// (const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize)
	#define GetStringSpacingDimension		MEX_H_GET_FUNC_DEFINE_OS( GetStringSpacingDimension )		// (const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize, int16 charSpace)
	#define GetStringExDimension			MEX_H_GET_FUNC_DEFINE_OS( GetStringExDimension )			// (const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, uint8 fontAttr, uint8 fontSize, bool lineWrap)
	#define GetStringExSpacingDimension		MEX_H_GET_FUNC_DEFINE_OS( GetStringExSpacingDimension )		// (const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, uint8 fontAttr, uint8 fontSize, bool lineWrap, int16 lineSpace, int16 columnSpace)
	#define DrawString						MEX_H_GET_FUNC_DEFINE_OS( DrawString )						// (const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize)
	#define DrawStringSpacing				MEX_H_GET_FUNC_DEFINE_OS( DrawStringSpacing )				// (const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize, int16 charSpace)
	#define DrawStringEx					MEX_H_GET_FUNC_DEFINE_OS( DrawStringEx )					// (const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap)
	#define DrawStringExSpacing				MEX_H_GET_FUNC_DEFINE_OS( DrawStringExSpacing )				// (const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap, int16 lineSpace, int16 columnSpace)
	#define DrawChar						MEX_H_GET_FUNC_DEFINE_OS( DrawChar )						// (uint8 *pCharData, uint32 dataSize, uint16 dataWidth, uint16 dataHeight, int32 x, int32 y, mex_color clr, uint8 fontAttr)
	#define GetCharData						MEX_H_GET_FUNC_DEFINE_OS( GetCharData )						// (int32 ch, uint8 **ppCharData, uint16 *pDataWidth, uint16 *pDataHeight, uint8 fontSize)

	#define DrawPointInternal				MEX_H_GET_FUNC_DEFINE_OS( DrawPointInternal )				// (int32 x, int32 y, mex_color clr)
	#define DrawLineInternal				MEX_H_GET_FUNC_DEFINE_OS( DrawLineInternal )				// (int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr)
	#define DrawFrameRectInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawFrameRectInternal )			// (int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr)
	#define DrawFillRectInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawFillRectInternal )			// (int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr)
	#define DrawFrameCircleInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawFrameCircleInternal )			// (int32 x, int32 y, int32 r, mex_color clr)
	#define DrawFillCircleInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawFillCircleInternal )			// (int32 x, int32 y, int32 r, mex_color clr)

	#define GetImageTypeFromMemInternal				MEX_H_GET_FUNC_DEFINE_OS( GetImageTypeFromMemInternal )				// (uint8 *ptr)
	#define GetImageTypeFromFileInternal			MEX_H_GET_FUNC_DEFINE_OS( GetImageTypeFromFileInternal )			// (const char *ptr)
	#define GetImageDimensionFromMemInternal		MEX_H_GET_FUNC_DEFINE_OS( GetImageDimensionFromMemInternal )		// (uint8 img_type,uint8 *img_ptr,int32 img_size,int32 *width, int32 *height)
	#define GetImageDimensionFromFileInternal		MEX_H_GET_FUNC_DEFINE_OS( GetImageDimensionFromFileInternal )		// (const char *image_name, int32 *width, int32 *height)
	#define GetGifImageFrameCountFromMemInternal	MEX_H_GET_FUNC_DEFINE_OS( GetGifImageFrameCountFromMemInternal )	// (uint8 *GIF_src, uint32 size, int32 *frame_count)
	#define GetGifImageFrameCountFromFileInternal	MEX_H_GET_FUNC_DEFINE_OS( GetGifImageFrameCountFromFileInternal )	// (const char *filename, int32 *frame_count)
	#define DrawImageFromMemInternal				MEX_H_GET_FUNC_DEFINE_OS( DrawImageFromMemInternal )				// (int32 x, int32 y, uint8 *img_ptr, uint8 img_type, int32 img_size)
	#define DrawImageFromFileInternal				MEX_H_GET_FUNC_DEFINE_OS( DrawImageFromFileInternal )				// (int32 x, int32 y, const char *img_name)
	#define DrawImageFromMemFramesInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawImageFromMemFramesInternal )			// (int32 x, int32 y, uint8 *img_ptr, uint8 img_type, int32 img_size, uint32 frame_no)
	#define DrawImageFromFileFramesInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawImageFromFileFramesInternal )			// (int32 x, int32 y, const char *img_name, uint32 frame_no)
	// 原GUI库部分函数 end ////////////////////////////////////
	#define GetAppParam					MEX_H_GET_FUNC_DEFINE_OS( GetAppParam )	

	#define GetAdmFunctions				MEX_H_GET_FUNC_DEFINE_OS( GetAdmFunctions )	
	
	#define IsAppPermissiveToDo 		MEX_H_GET_FUNC_DEFINE_OS( IsAppPermissiveToDo ) 
	
	#define GetActScreenBuffer 			MEX_H_GET_FUNC_DEFINE_OS( GetActScreenBuffer ) 

	#define UnCompressDataFromMemory	MEX_H_GET_FUNC_DEFINE_OS( UnCompressDataFromMemory )		// (uint8 *memData, uint8 *orgData)
	#define UnCompressDataFromFile		MEX_H_GET_FUNC_DEFINE_OS( UnCompressDataFromFile )			// (int32 *file_handle, uint8 *orgData)

	#define CallManagerFunction 		MEX_H_GET_FUNC_DEFINE_OS( CallManagerFunction ) 

	#define RegisterSimulateKey 		MEX_H_GET_FUNC_DEFINE_OS( RegisterSimulateKey ) 

	#define StartThreadTimer 			MEX_H_GET_FUNC_DEFINE_OS( StartThreadTimer ) 
	#define KillThreadTimer 			MEX_H_GET_FUNC_DEFINE_OS( KillThreadTimer ) 
	#define SetThreadTimerInterval 		MEX_H_GET_FUNC_DEFINE_OS( SetThreadTimerInterval ) 

	#define TakeMutex 					MEX_H_GET_FUNC_DEFINE_OS( TakeMutex ) 
	#define GiveMutex 					MEX_H_GET_FUNC_DEFINE_OS( GiveMutex ) 

	#define GetAppPosition				MEX_H_GET_FUNC_DEFINE_OS( GetAppPosition ) 
	#define SetAppPosition				MEX_H_GET_FUNC_DEFINE_OS( SetAppPosition ) 

	#define RegSystemStatusIcon 		MEX_H_GET_FUNC_DEFINE_OS( RegSystemStatusIcon )	
	#define UnRegSystemStatusIcon 		MEX_H_GET_FUNC_DEFINE_OS( UnRegSystemStatusIcon )	
	#define SetSystemStatusIcon 		MEX_H_GET_FUNC_DEFINE_OS( SetSystemStatusIcon )	
	#define GetSystemStatusIcon 		MEX_H_GET_FUNC_DEFINE_OS( GetSystemStatusIcon )	
	#define SetSystemStatusIconStatus 	MEX_H_GET_FUNC_DEFINE_OS( SetSystemStatusIconStatus )

#if MEX_SDK_VER >= 1001
	#define GetScreenLayerBufferWithInfo 	MEX_H_GET_FUNC_DEFINE_OS( GetScreenLayerBufferWithInfo )
#endif

#if MEX_SDK_VER >= 1004
	#define tmalloc 					MEX_H_GET_FUNC_DEFINE_OS( tmalloc )
	#define tfree 						MEX_H_GET_FUNC_DEFINE_OS( tfree )
#endif


#if MEX_SDK_VER >= 1009
	#define Fmgr_Create					MEX_H_GET_FUNC_DEFINE_OS( Fmgr_Create )
	#define Fmgr_Cancel					MEX_H_GET_FUNC_DEFINE_OS( Fmgr_Cancel )					
	#define Fmgr_SetFilter				MEX_H_GET_FUNC_DEFINE_OS( Fmgr_SetFilter )
	#define Fmgr_Show					MEX_H_GET_FUNC_DEFINE_OS( Fmgr_Show )
#endif


#if MEX_SDK_VER >= 1010
	#define Pen_Block					MEX_H_GET_FUNC_DEFINE_OS( Pen_Block )					 
	#define Pen_unBlock					MEX_H_GET_FUNC_DEFINE_OS( Pen_unBlock ) 
	#define KeyPad_Lock					MEX_H_GET_FUNC_DEFINE_OS( KeyPad_Lock ) 
	#define KeyPad_unLock				MEX_H_GET_FUNC_DEFINE_OS( KeyPad_unLock ) 
	#define ReStartKeyPadLock			MEX_H_GET_FUNC_DEFINE_OS( ReStartKeyPadLock )
#endif

#if MEX_SDK_VER >= 1011
	#define DrawImageScaleFromFileInternal			MEX_H_GET_FUNC_DEFINE_OS( DrawImageScaleFromFileInternal )					 
	#define DrawImageScaleFromFileFramesInternal 	MEX_H_GET_FUNC_DEFINE_OS( DrawImageScaleFromFileFramesInternal ) 
#endif

#if MEX_SDK_VER >= 1015
	#define SetROCache					MEX_H_GET_FUNC_DEFINE_OS( SetROCache )					 
	#define ClearROCache 				MEX_H_GET_FUNC_DEFINE_OS( ClearROCache ) 
#endif

#if MEX_SDK_VER >= 1017
	#define	SetSystemStatusIcon_ext			MEX_H_GET_FUNC_DEFINE_OS( SetSystemStatusIcon_ext )
	#define	SetSystemStatusIconStatus_ext 	MEX_H_GET_FUNC_DEFINE_OS( SetSystemStatusIconStatus_ext )
	#define Rgb565ToYuv420					MEX_H_GET_FUNC_DEFINE_OS( Rgb565ToYuv420 )
#endif

#if MEX_SDK_VER >= 1018
	#define	SetDateTime						MEX_H_GET_FUNC_DEFINE_OS( SetDateTime )
#endif
#if MEX_SDK_VER >= 1024
#define LaunchAppExByAdm 						MEX_H_GET_FUNC_DEFINE_OS( LaunchAppExByAdm ) 
#define FreeAppByAdm 							MEX_H_GET_FUNC_DEFINE_OS( FreeAppByAdm ) 
#define GetAppMaxMemSize 						MEX_H_GET_FUNC_DEFINE_OS( GetAppMaxMemSize ) 
#define GetLeftMemSize 							MEX_H_GET_FUNC_DEFINE_OS( GetLeftMemSize ) 
#define IsUsingReservedMem 						MEX_H_GET_FUNC_DEFINE_OS( IsUsingReservedMem )
#define IsKernalApp		 						MEX_H_GET_FUNC_DEFINE_OS( IsKernalApp )
#define IsAppInternal		 					MEX_H_GET_FUNC_DEFINE_OS( IsAppInternal )
#define SetWidgetTop		 					MEX_H_GET_FUNC_DEFINE_OS( SetWidgetTop )
#define SetWidgetVisiable		 				MEX_H_GET_FUNC_DEFINE_OS( SetWidgetVisiable )
#define GetGdiLayer2Buf		 					MEX_H_GET_FUNC_DEFINE_OS( GetGdiLayer2Buf )
#define ReturnGdiLayer2Buf		 				MEX_H_GET_FUNC_DEFINE_OS( ReturnGdiLayer2Buf )
#define SetAppPriority		 					MEX_H_GET_FUNC_DEFINE_OS( SetAppPriority )
#define AllowAcceptKey		 					MEX_H_GET_FUNC_DEFINE_OS( AllowAcceptKey )	
#define GetAppFrameBuffer						MEX_H_GET_FUNC_DEFINE_OS( GetAppFrameBuffer )
#define SetWidgetFullModeMemSize				MEX_H_GET_FUNC_DEFINE_OS( SetWidgetFullModeMemSize )
#define GetDeskAppImgPro						MEX_H_GET_FUNC_DEFINE_OS( GetDeskAppImgPro )
#define SetManCurAppId 							MEX_H_GET_FUNC_DEFINE_OS( SetManCurAppId )
#endif
#if MEX_SDK_VER >= 1028
#define GetRunModeFromAppId 					MEX_H_GET_FUNC_DEFINE_OS( GetRunModeFromAppId )
#define AppIsInMemory 							MEX_H_GET_FUNC_DEFINE_OS( AppIsInMemory )
#endif


#endif

#if defined(MEX_ENABLE_PROFILING)
	#if defined (MEX_TARGET_CLIENT)
		#define PROF_START(txt)		{ int16 __iProfLevel; AddProfilingData(true, &__iProfLevel, txt);
		#define PROF_END()			AddProfilingData(false, &__iProfLevel, NULL); }
	#endif
	#if defined(MEX_TARGET_SERVER)
		#define PROF_START(txt)		{ int16 __iProfLevel; mex_AddProfilingData(true, &__iProfLevel, txt);
		#define PROF_END()			mex_AddProfilingData(false, &__iProfLevel, NULL); }
	#endif
#else
	#define PROF_START(txt)		
	#define PROF_END()			
#endif


//write comments below for VA to display them
#if defined(_MEX_COMMENTS_NEVER_DEFINE_THIS_)

//取得函数库函数列表指针
//参数1：lib库ID
//参数2：该lib中的函数数目（输入）
//返回：函数指针列表的指针
funcptr** ExchangeLibPtrPtr(int32 iLibId, int32 iNbFuncsInLib);	//内部函数

//返回当前应用程序启动时的参数，该参数在LaunchAppEx中有指明
const char* GetLaunchParam(void);

//同LaunchApp，外加启动参数szParam，可以通过GetLaunchParam获取

int32 LaunchAppEx(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam);
int32 LaunchAppExByAdm(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir, const char *szParam, int32 iAdmId );

//加载启动指定应用程序
//参数1：应用名（文件名去掉后缀）
//参数2：文件目录，相对目录，app传基于mexapp下的目录，如L"HelloWorld"，system则传L""
//参数3：文件名,如L"HelloWorld.mex"
//参数4：是否加载到sys内部
//参数5：是否在sys根目录
//返回：应用ID，为NULL则加载失败
int32 LaunchApp(const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir);

bool RegisterLaunchAppAfterExit(bool bEnable, const wchar_t *szAppName, const wchar_t *szFileDir, const wchar_t *szFileName, bool bInternal, bool bInSystemDir);

//销毁指定应用
//参数1：App库ID
//返回：无
void FreeApp(int32 iAppId);		
void FreeAppByAdm(int32 iAppId ,int32 iAdmId);

//应用注册消息回调函数
//参数1：App库ID
//参数2：消息回调函数
//返回：无
void RegisterMsgHandler(int32 iAppId, MexApp_MsgHandler_Func pFunc);		

//应用或lib注册退出函数
//参数1：App库ID
//参数2：退出函数
//参数3：是否为Lib
//返回：无
void RegisterExitFunc(int32 iAppId, MexApp_Exit_Func pFunc, bool bAmILib);		

//参数1：App库ID
//返回：应用消息回调函数
MexApp_MsgHandler_Func GetMsgHandler(int32 iAppId);		

//应用退出
void Exit(void);		

//把程序代码段加载入sram
//参数1：应用id
bool LoadAppIntoSRam(int32 iAppId);

//把程序代码段从sram释放，（Manager调用）
//参数1：应用id
void FreeAppFromSRam(int32 iAppId);

//把lib的代码段加载入sram
//参数1：lib库函数指针
bool LoadLibIntoSRam(funcptr pLibFuncPtr);

//把lib的代码段从sram释放（Manager调用）
//参数1：lib库函数指针
void FreeLibFromSRam(funcptr pLibFuncPtr);

//取得当前活动的App，对于Lib是调用lib函数的应用，对于App是自己
//返回：当前活动的AppId
int32 GetCurrentApp(void);		


//设置当前活动的App
//参数1：活动的AppId
//返回：无
void SetCurrentApp(int32 iAppId);	


//取得平台系统根目录
//参数：无
//返回：平台系统根目录
const wchar_t* GetSystemRootDir(void);		

//取得应用应该所在的目录
//参数：应用名
//返回：真实目录
//例如，szAppFileName == L"HelloWorld.mex"，返回可能是 L"D:\\MexApp\\HelloWorld"
const wchar_t* GetAppDir(const wchar_t *szAppFileName);

//取得平台应用根目录
//参数：无
//返回：平台应用根目录
const wchar_t* GetAppRootDir(void);		


//取得当前活动的App根目录
//参数1：根目录要拷贝的buf，外部申请
//返回：当前活动的App根目录
const wchar_t* GetCurAppDir(wchar_t* curAppDir);		

//取得当前活动的App文件名
//参数1：根文件名要拷贝的buf，外部申请
//返回：当前活动的App文件名
const wchar_t* GetCurAppFileName(wchar_t* curAppFileName);		


//绘制开始，锁定buf
//返回：是否成功（当背光暗时false）
bool BeginPaint(void);		

//绘制结束，解除锁定
//返回：无
void EndPaint(void);		

bool Invalidate(void);

//取得当前剩余内存，分为sys内存，应用内存
//参数1：是否为sys内存
//返回：剩余内存size
int32 GetFreeMemory(E_Mex_MemoryPool ePool);

//取得指定应用的属性
//参数1：应用ID
//返回：应用的属性

//应用属性有：（&）
//	MEXAPP_ATTRIB_SMALL_TINY_ALWAYS_ON_TOP			保持在最前端
//	MEXAPP_ATTRIB_FULL_NEED_BACKUP_BUF					全屏不会每帧都重绘
//	MEXAPP_ATTRIB_FULL_PAINT_ON_BACKGROUND			绘制不会被中断
//	MEXAPP_ATTRIB_SMALL_TINY_CAN_BE_MOVE				浮动模式是否可以被移动
int32 GetAppAttribute(int32 iAppId);		

//设置指定应用的属性
//参数1：应用ID
//参数2：应用的属性
//返回：应用的属性
int32 SetAppAttribute(int32 iAppId int32 iAttribute);		

//设置指定应用的指定线程是否暂停
//参数1：应用ID
//参数2：timer ID
//参数3：是否暂停
//返回：是否成功
bool PauseAppTimer(int32 iAppId, int32 iTimerId, bool bPause);	


//取得系统gdi layer层的指针
//参数：layer id，一般 0 或者 1
//返回：缓存指针，NULL表示没有该层
uint16* GetScreenLayerBuffer(int32 iLayer);

//禁用系统双缓冲优化
//参数：是否禁用
//注：应用程序如果用参数true调用，则在退出前建议用false调用，以免影响其他程序
void DisableDbBufferBlt(bool bDisable);


//取得系统持有的屏幕buf信息
//参数1：buf指针（out）
//参数2：buf宽（out）
//参数3：buf高（out）
//返回：无
void GetFullScreenBuffer(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);		

//取得应用注册的buf信息
//参数1：AppID
//参数2：buf指针（out）
//参数3：buf宽（out）
//参数4：buf高（out）
//返回：无
void GetAppScreenBuffer(int32 iAppId, uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);	

//注册应用的屏幕buf信息
//参数1：buf指针
//参数2：buf宽
//参数3：buf高
//返回：无
void RegisterScreenBuffer(uint16 *pBuffer, uint16 uWidth, uint16 uHeight);		

//删除注册应用的屏幕buf信息
//返回：无
void UnRegisterScreenBuffer(void);		


//开启计时器，返回计时器ID
//参数1：timer时间间隙
//返回：计时器ID
int32 SetTimer(int32 iTime);

//关闭计时器
//参数1：计时器ID
//返回：是否成功
bool KillTimer(int32 iTimerId);		

//修改计时器的计时间隔
//参数1：App ID
//参数2：timerID
//参数3：timer时间间隙
//参数4：timer原始时间间隙指针（out）
//返回：是否成功
bool SetTimerInterval(int32 iAppId, int32 iTimerId, int32 iNewDuration, int32 *pOldDuration);	

//返回系统计时毫秒数
//返回：毫秒数
int32 gettime( void );		

//取得应用运行模式
//返回：应用运行模式，参见E_Mex_RunMode
E_Mex_RunMode GetRunMode(void);		

//切换应用运行模式
//参数：新模式，参见enum E_Mex_RunMode
//返回：切换是否成功
bool SwitchRunMode(E_Mex_RunMode newMode);


//取得注册表指针
//返回：注册表指针
void* GetRegistry(void);		


//往指定应用发送消息
//参数iSrcAppId ：消息源应用ID，SYSTEM为0。
//参数iDstAppId ：消息目标应用ID（比为本应用ID）。
//参数eMsgType ：消息类型，参见enum E_Mex_MessageType。
//参数iMsgData0 ~ iMsgData3：消息数据0-3，根据消息类型有特定意义。
//返回 int32     ：处理结果，0-未处理，1-已处理
int32 SendMessage(int32 iSrcAppId, int32 iDstAppId, E_Mex_MessageType eMessage, int32 iMsgData0, int32 iMsgData1, int32 iMsgData2, int32 iMsgData3);		

//获取下一个AppId
//参数iCurAppId：从此AppId算起，输入0返回第一个AppId
//返回：从CurAppId算起的下一个App的Id
int32 GetNextAppId(int32 iCurAppId);	//内部函数

//通过appid获取App信息
//参数1：应用id
//参数2：应用类型type  (out)
//参数3：应用版本信息(out)
//参数4：应用名称(out)
//参数5：应用目录，相对于mexapp，如"HelloWorld"，system的为""(out)
//参数6：应用文件名，如"HelloWorld.mex"(out)
//返回：获取是否成功，只当iCurAppId是非法值时返回false
bool GetAppInfoFromId(int32 iCurAppId, int32 *piAppType, int16 *piAppVersion, wchar_t **ppAppName, wchar_t **ppFileDir, wchar_t **ppFileName);	//内部函数

//通过文件名获取App信息
//参数1：应用目录，相对于mexapp，如"HelloWorld"，system的为""(in)
//参数2：应用文件名，如"HelloWorld.mex"(in)
//参数3：是否在sys目录中(in)
//参数4：应用类型type  (out)
//参数5：应用版本信息(out)
//返回：获取是否成功，当文件不存在时返回false
bool GetAppInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int32 *piAppType, int16 *piAppVersion);

//通过绝对路径文件名获取App信息
//参数1：应用文件名，如"E:\MexApp\Test\Test.mex"(in)
//参数2：应用类型type  (out)
//参数3：应用版本信息(out)
//返回：获取是否成功，当文件不存在时返回false
bool GetAppInfoFromFile_ext(const wchar_t *szFullFileName, int32 *piAppType, int16 *piAppVersion);

//取得系统配置信息
//参数cfg：信息类型。
//返回：信息数据
int32 GetPhoneConfig(E_Mex_Phone_Config cfg);		


//取得系统时间
//参数pDt：时间信息结构（out），参见。Mex_DateTime
//返回：无
void GetDateTime(Mex_DateTime *pDt);		


//file functions

//可以以绝对路径打开文件的函数
//参数1：文件名绝对路径
//参数2：模式，"	ｒｂ＋"　"ｒｂ"　"ｗｂ"
//返回：文件句柄
int32* fopenw_org(const wchar_t *filename, const wchar_t *mode);

//可以以绝对路径打开文件的函数
//参数1：文件名
//参数2：模式，"	ｒｂ＋"　"ｒｂ"　"ｗｂ"
//参数3：文件夹路径，绝对路径
//参数4：mex文件，传NULL则直接打开path下filename文件
//参数5：是否只从mex文件中读取
//返回：文件句柄
int32* fopenw_ext(const wchar_t *filename, const wchar_t *mode, const wchar_t *path, const wchar_t *mexFileName, bool bCheckInsideMexOnly);		

//打开文件，宽字节路径，支持"rb+"  "rb"  "wb"
//返回：文件句柄
int32* fopenw(const wchar_t *filename, const wchar_t *mode);		

//打开文件，支持"rb+"  "rb"  "wb"
//返回：文件句柄
int32* fopen(const char *filename, const char *mode);	

//文件定位
int32 fseek(int32* handle, long offset, int32 whence);		

//文件当前读取位置
int32 ftell(int32* handle);		

//文件关闭
int32 fclose(int32* handle);		


//取得文件大小
size_t GetFileSize(int32* handle);		

//文件取得
size_t fread(void *ptr, size_t size, size_t nitems, int32* handle);		

//写入文件
size_t fwrite(const void *ptr, size_t size, size_t nitems, int32* handle);		

//flush文件流
int32 fflush(int32* handle);		

//删除文件(应用虚拟路径)
int32 remove(const wchar_t *file);		

//文件重命名(应用虚拟路径)
int32 rename(const wchar_t *ofile, const wchar_t *nfile);		

//创建文件夹(应用虚拟路径)
int32 CreateDir(const wchar_t *dir);		

//删除文件夹(应用虚拟路径)
int32 RemoveDir(const wchar_t *dir);	

//删除文件(绝对路径)
int32 remove_ext(const wchar_t *file);		

//文件重命名(绝对路径)
int32 rename_ext(const wchar_t *ofile, const wchar_t *nfile);		

//创建文件夹(绝对路径)
int32 CreateDir_ext(const wchar_t *dir);		

//删除文件夹(绝对路径)
int32 RemoveDir_ext(const wchar_t *dir);	

//判断目录是否存在(绝对路径)
bool IsDirExist_ext(const wchar_t *dir);

//查找文件夹下的类型文件
//参数1：文件夹路径
//参数2：文件样式
//参数3：要将找到的文件名称保存buf（out），外部申请
//参数4：buf大小
//返回：查找句柄
int32 FindFirst(const wchar_t *path, const wchar_t *namePattern, const wchar_t *fileName, uint32 maxLength);	

//查找文件夹下的文件夹
//参数1：文件夹路径
//参数2：要将找到的文件夹名称保存buf（out），外部申请
//参数3：buf大小
//返回：查找句柄
int32 FindDirFirst(const wchar_t *path, const wchar_t *fileName, uint32 maxLength);	

//查找文件夹下的下一个文件夹
//参数1：查找句柄
//参数2：要将找到的文件夹名称保存buf（out），外部申请
//参数3：buf大小
//返回：是否找到，<0 查找结束
int32 FindNext(int32 findHandle, const wchar_t *fileName, uint32 maxLength);		

//查找文件夹下的下一个文件夹
//参数1：查找句柄
//返回：0 关闭成功
int32 FindClose(int32 findHandle);	


//打印log信息，保存在文件中
//iFilter可为：
//	MEX_LOG_SYSTEM			0x00000001
//	MEX_LOG_MEMORY			0x00000002
//	MEX_LOG_MANAGER		0x00000004
//	MEX_LOG_STDLIB			0x00000008
//	MEX_LOG_OSLIB			0x00000010
//	MEX_LOG_APP0				0x00000100
//	MEX_LOG_APP1				0x00000200
//	MEX_LOG_APP2				0x00000400
//	MEX_LOG_APP3				0x00000800
void log(int32 iFilter, const char *pText, ...);

//取得logiFilter（&）
int32 getlogfilter(void);

//设置logiFilter（&）
void setlogfilter(int32 iFilter);

//做profiling使用的函数
void StartProfiling(bool bRestart, int32 iNbFrames);
void AddProfilingData(bool bIsStart, int16 *piLevel, const char *pText);

//取得按键状态
E_Mex_KeyEvent GetKeyState(E_Mex_KeyCode keyCode);

//设置按键状态
E_Mex_KeyEvent SetKeyState(E_Mex_KeyEvent keyEvent, E_Mex_KeyCode keyCode);	

//申请虚拟内存
void* VMalloc(size_t iSize);

//释放虚拟内存
void VFree(void* pVmPtr);

//从真实内存复制数据到虚拟内存
void* VMemcpy(void* pVmPtr, void *pMemPtr, size_t size);

//把虚拟内存内容复制到真实内存
void* VToMem(void *pMemPtr, void *pVmPtr, size_t size);

//申请和得到固定内存，手机关机也还是存在
void* VPMalloc(const char *szKeyName, size_t iSize, bool *pbAlreadyExist);

//释放虚拟固定内存
void VPFree(void* pVmPtr);

//设置使用保留的外部内存
//返回原来的设置（用于成对调用）
//该程序应当只由manager调用
bool SetUseReservedMem(bool bUse);

//开启系统线程, 供内部库或程序使用
//参数1 : 时间间隔
//参数2 : 回调函数
//返回: 线程编号
int32 StartSystemTimer (int32 iTime, MexFuncPtr func);

//关闭系统线程, 供内部库或程序使用
//参数1 :  线程编号
//返回:是否成功
bool KillSystemTimer (int32 iTimeId);

//更改线程间隔, 供内部库或程序使用
//参数1 :  线程编号
//参数2 : 新事件间隔
//参数3 : 原时间间隔(out)
//返回:是否成功
bool SetSystemTimerInterval (int32 iTimerId, int32 iNewTime, int32 *pOldTime);

//注册自身应用到屏幕切换时自动启动
//参数1：屏幕Id，0-3
bool RegisterLaunchOnScreen(int32 iScreenId);

//注册自身应用到平台启动时自动启动
bool RegisterLaunchOnStartup(void);

//获取上一帧全屏的buffer（manager处理），特殊应用使用
void GetLastFullScreenBuffer(uint16 **ppBuffer, uint16 *puWidth, uint16 *puHeight);
bool RegisterLaunchOnFullAppExit(void);
//调用输入框的函数
//---------------------------------------------
//参数1: 输入框的标题，unicode 码
//参数2: 输入框的类型，见枚举定义
//参数3: 输入框的最大允许输入长度
//参数4: 是否为密码输入框
//参数5: 是否允许切换输入法
//参数6: 输入的字符串存储的buf，不可为NULL
//参数7: 被回调的函数指针
//---------------------------------------------
bool InputBox_Create(  wchar_t* pInputTitle, E_Mex_Input_Type iInputType, uint32 iMaxNum,
								bool bIsPassword, bool bCanChange, 
								uint8 * pContent, Mex_Input_Func pInputFunc )

//解压缩
//参数1: 解压结果指针
//参数2: 解压长度指针
//参数3: 解压源
//参数4: 解压源长度
//返回: 解压结果
int32 zip_uncompress(int8 * pDest, unsigned long* lDestLen, const int8 * pSource, unsigned long lSourceLen);

//暂停系统非阻塞绘图
//参数：帧数
//注：下帧起效
void DisablePaintOpt(int32 iMaxNbFrames);

//申请sram内存
void* smalloc(size_t iSize);

//释放sram内存
void sfree(void *psPtr);

//告诉系统本程序将暂时使用所有的sram空间
bool NotifyLockSRam(void);

//告诉系统本程序sram空间用好了
bool NotifyUnlockSRam(void);

//关闭mex平台
//参数：关闭种类，枚举值
//		E_MPS_HotReboot：热重启
//		E_MPS_ColdReboot：冷重启
//		E_MPS_HotShutdown：热关闭
//		E_MPS_ColdShutdown：冷关闭
//返回值：无
//注：系统会在下一帧重启或者关闭
void ShutdownPlatform(E_Mex_PlatformShutdown eOp);

//取得应用注册的Screen 通透色信息
//参数1: 应用id (in)
//参数2: 通透色色值(out)
//参数3: 是否通透(out)
void GetAppScreenKeyColor(int32 iAppId, uint16 *pKeyColor, bool *pbIsTrans );

//应用注册自己的Screen 通透色信息
//参数1: 通透色色值
//参数2: 是否通透
void RegisterScreenKeyColor(uint16 uKeyColor, bool bIsTrans );

// 【功  能】  分离绝对路径
// 【说  明】  该函数分离绝对路径的路径及名称
// 【注  意】  路径部分及名称部分调用者使用完需要释放
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      pathPart        路径部分
// 【参数 2】      namePart        名称部分
// 【参数 3】      path            绝对路径
// 【返回值】      bool型          是否成功
bool SeparateAbsPath(wchar_t **pathPart, wchar_t **namePart, const wchar_t *path);

//创建绝对路径（自动递归一个个创建）
//参数：绝对路径
//返回：是否成功
bool CreateDir_ext_recur(const wchar_t *szFullDir);


//弹出询问提示框 是否要退出平台 如果选是 则退出平台并回到idle界面
//参数1:是否 完全退出
void ExitPlatform( bool bExitAll );

//取得程序配置的默认大小，用于启动的特效
//参数1: 应用id   (IN)
//参数2: 注册宽(OUT)
//参数3: 注册高(OUT)
//------------------
//返回: 是否成功
bool GetAppRegistSizeFromId(int32 iCurAppId, uint16 *puWidth, uint16 *puHeight );

//取得程序配置的默认大小，用于启动的特效
//参数1: 应用目录   (IN)
//参数2: 应用文件名  (IN)
//参数3: 注册宽(OUT)
//参数4: 注册高(OUT)
//------------------
//返回: 是否成功
//注意：不支持1970年以前的时间
bool GetAppRegistSizeFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, uint16 *puWidth, uint16 *puHeight );

//取得当前系统时间
//------------------
//返回: 从1970年1月1日0点起到当前时间的秒数
uint32 GetCurrentTime(void);

//将秒数转换为Mex_DateTime
//参数1:从1970年1月1日0点起到某时间的秒数  (IN)
//参数2：转换后的日期时间	（out）
//------------------
//返回:
//注意：不支持1970年以前的时间
void UtcSec2DateTime(uint32 utc_sec, Mex_DateTime *pdt);

//从手机申请临时内存
//注：该内存须在同一帧内释放，并且越快越好
void* xmalloc(size_t iSize);

//释放临时内存
void xfree(void* pPtr);

//禁止manager的 "#号" 提示退出当前应用程序 的功能
//参数：是否禁止该功能
//返回：指令是否接受（如果调用的应用程序没有权限，例如是widget程序，则会失败）
bool DisableQuitControl(bool bDisable);

//获取应用程序的extra参数（mpj文件中第二行的10个int16数字），已知定义：
//[0]: 版本号
//[1]: 程序运行后的宽度
//[2]: 程序运行后的高度
//[3]: 应用的唯一Id号
//[4]: 保留
//[5]: 保留
//[6]: 保留
//[7]: 保留
//[8]: 保留
//[9]: 保留
//参数使用同GetAppInfo
bool GetAppExtraInfoFromId(int32 iCurAppId, int16 *ppInfo );
bool GetAppExtraInfoFromFile(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir, int16 *ppInfo );

//获取系统内部参数
int32 GetSystemParam(void);

//休眠一些帧数
void Sleep(int32 iTicks);

//注册启动系统程序（当平台退出时）
//详情请见E_MEX_Entry_System_App枚举，pParam为参数
void RegisterLaunchSystemApp(E_MEX_Entry_System_App eApp, void *pParam, int32 iParamSize);

// 原GUI库部分函数 begin ////////////////////////////////////
/************************************************************************
* 缓存处理函数
************************************************************************/

// 【功  能】  获取缓存裁剪边界
// 【说  明】  该函数获取当前缓存区裁剪边界，若为设置缓存将返回系统默认裁剪边界
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [OUT] x1              裁剪边界左上角x1
// 【参数 2】      [OUT] y1              裁剪边界左上角y1
// 【参数 3】      [OUT] x2              裁剪边界右下角x2
// 【参数 4】      [OUT] y2              裁剪边界右下角y2
// 【返回值】      [OUT] 无
void GetBufferClipper(int16 *x1, int16 *y1, int16 *x2, int16 *y2);

// 【功  能】  设置缓存区裁剪边界
// 【说  明】  该函数设置当前缓存区裁剪边界，系统默认缓存区裁剪边界为（0，0）-->（width - 1， height - 1），所有OS库绘制
//             函数都将以该裁剪边界对字符串/图形/图像进行裁剪。
//             但若当用户调用该函数重新设置缓存区裁剪边界时，OS库绘制函数将以新裁剪边界对字符串/图形/图像进行裁剪
// 【注  意】  正常情况下 x1 <= x2, y1 <= y2。若用户设置的裁剪边界 x1 > x2, y1 > y2，则绘制函数将不会在缓存中绘制任何字符
//             串/图形/图像
//           ※裁剪边界不遵从左上填充原则，及裁剪边界将包含左上角及右下角，裁剪字符串/图形/图像时将以左上角及右下角点为基准
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 2】      [IN]  x1              裁剪边界左上角x1
// 【参数 2】      [IN]  y1              裁剪边界左上角y1
// 【参数 2】      [IN]  x2              裁剪边界右下角x2
// 【参数 2】      [IN]  y2              裁剪边界右下角y2
// 【返回值】      [OUT] 无
void SetBufferClipper(int16 x1, int16 y1, int16 x2, int16 y2);

// 【功  能】  设置缓存区子裁剪边界
// 【说  明】  该函数设置当前缓存区子裁剪边界，该函数将在前裁剪边界基础上继续设置第二层裁剪边界，
//             设置后的裁剪边界为本裁剪边界与原裁剪边界的交集
// 【注  意】※裁剪边界不遵从左上填充原则，即裁剪边界将包含左上角及右下角，裁剪字符串/图形/图像时将以左上角及右下角点为基准
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1              裁剪边界左上角x1
// 【参数 2】      [IN]  y1              裁剪边界左上角y1
// 【参数 3】      [IN]  x2              裁剪边界右下角x2
// 【参数 4】      [IN]  y2              裁剪边界右下角y2
// 【返回值】      [OUT] 无
void SetBufferSubClipper(int16 x1, int16 y1, int16 x2, int16 y2);

// 【功  能】  设置OS绘制缓存
// 【说  明】  该函数设置OS绘制缓存区，用户向指定缓存中绘制字符及图形时，需要预先调用该函数，
//             若不设置OS绘制缓存，则默认将系统主缓存区（即屏幕缓存）视为绘制缓存
// 【注  意】  该函数不需用户调用，该函数将由宏【OS_BEGIN】【OS_END】自动调用
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  buffer          指定缓存
// 【返回值】      [OUT] Mex_GuiBuffer型 设置前缓存
Mex_GuiBuffer SelectGuiBuffer(Mex_GuiBuffer *buffer);

/************************************************************************
* 字符/字符串处理函数
*
* 字体各属性说明
*-----------------------------------------------------------------------
* 字体属性                      说明
* MEX_FONTATTR_NORMAL           正常
* MEX_FONTATTR_BOLD             粗体
* MEX_FONTATTR_ITALIC           斜体1
* MEX_FONTATTR_OBLIQUE          斜体2
* MEX_FONTATTR_UNDERLINE        下划线
* MEX_FONTATTR_STRIKETHROUGH    删除线（未使用）
*-----------------------------------------------------------------------
* 字体大小                      说明
* MEX_FONTSIZE_SMALL            小号
* MEX_FONTSIZE_MEDIUM           中号
* MEX_FONTSIZE_LARGE            大号（未使用）
* MEX_FONTSIZE_SUBLCD           （未使用）
* MEX_FONTSIZE_DIALER           （未使用）
* MEX_FONTSIZE_VIRTUAL_KEYBOARD （未使用）
*-----------------------------------------------------------------------
* 字体位置                      说明（仅函数【DrawStringEx】使用）
* MEX_FONTPOS_DEFAULT           默认
* MEX_FONTPOS_VTOP              竖直上对齐
* MEX_FONTPOS_VMIDDLE           竖直居中
* MEX_FONTPOS_VBOTTOM           竖直下对齐
* MEX_FONTPOS_HLEFT             水平左对齐
* MEX_FONTPOS_HCENTER           水平居中
* MEX_FONTPOS_HRIGHT            水平右对齐
************************************************************************/

// 【功  能】  获取字符尺寸
// 【说  明】  该函数获取指定样式字符的尺寸，字体属性及字体大小参见【字体各属性说明】
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  ch              指定字符
// 【参数 2】      [OUT] width           字符宽度（指针）
// 【参数 3】      [OUT] height          字符高度（指针）
// 【参数 4】      [IN]  fontAttr        字体属性
// 【参数 5】      [IN]  fontSize        字体大小
// 【返回值】      [OUT] 无
void GetCharDimension(uint32 ch, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize);

// 【功  能】  获取字符串尺寸
// 【说  明】  该函数获取指定样式字符串的尺寸，字体属性及字体大小参见【字体各属性说明】
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          指定字符串
// 【参数 2】      [OUT] width           字符宽度（指针）
// 【参数 3】      [OUT] height          字符高度（指针）
// 【参数 4】      [IN]  fontAttr        字体属性
// 【参数 5】      [IN]  fontSize        字体大小
// 【返回值】      [OUT] 无
void GetStringDimension(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize);

// 【功  能】  获取自定义间距字符串尺寸
// 【说  明】  该函数获取指定样式字符串的尺寸，字体属性及字体大小参见【字体各属性说明】
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          指定字符串
// 【参数 2】      [OUT] width           字符宽度（指针）
// 【参数 3】      [OUT] height          字符高度（指针）
// 【参数 4】      [IN]  fontAttr        字体属性
// 【参数 5】      [IN]  fontSize        字体大小
// 【参数 6】      [IN]  charSpace       字符间距
// 【返回值】      [OUT] 无
void GetStringSpacingDimension(const wchar_t *string, int32 *width, int32 *height, uint8 fontAttr, uint8 fontSize, int16 charSpace);

// 【功  能】  获得字符串块尺寸
// 【说  明】  该函数获得指定缓存中指定区域绘制指定样式字符串时字符串块尺寸，默认为屏幕缓存。字体属性及字体大小、
//             位置参见【字体各属性说明】
//             该函数自动处理换行符【\r】及【\n】，【\r】将光标移动到该行开始处，【\n】将光标向下移动一行
//           ①若使用默认位置，则从绘制区域左上角开始绘制字符串，严格遵从换行符功能
//             若不使用默认位置，则换行符效果将变成不同字符串段落的分割，即【\r】将后面段落与前面分开成两个段落，但仍
//             在同一行，而【\n】将后面段落向下移动一行，而实际每一段落显示位置将遵从参数的位置属性
//           ②若开启自动换行，则当字符串绘制超过绘制区域右边界时，将自动折行到下一行。
//             若不开启自动换行，则字符串即使超过绘制区域右边界，也不会自动折行，而是将字符串绘制到不可见的区域
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [OUT] width           内容宽度
// 【参数 3】      [OUT] height          内容高度
// 【参数 4】      [IN]  areaWidth       绘制区域宽度
// 【参数 5】      [IN]  fontAttr        字体属性
// 【参数 6】      [IN]  fontSize        字体大小
// 【参数 7】      [IN]  lineWrap        是否自动换行
// 【返回值】      [OUT] uint16型        行数
uint16 GetStringExDimension(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth, 
							uint8 fontAttr, uint8 fontSize, bool lineWrap);

// 【功  能】  获得自定义行列间距字符串块尺寸
// 【说  明】  该函数获得指定缓存中指定区域绘制指定样式字符串时字符串块尺寸，默认为屏幕缓存。字体属性及字体大小、
//             位置参见【字体各属性说明】
//             该函数自动处理换行符【\r】及【\n】，【\r】将光标移动到该行开始处，【\n】将光标向下移动一行
//           ①若使用默认位置，则从绘制区域左上角开始绘制字符串，严格遵从换行符功能
//             若不使用默认位置，则换行符效果将变成不同字符串段落的分割，即【\r】将后面段落与前面分开成两个段落，但仍
//             在同一行，而【\n】将后面段落向下移动一行，而实际每一段落显示位置将遵从参数的位置属性
//           ②若开启自动换行，则当字符串绘制超过绘制区域右边界时，将自动折行到下一行。
//             若不开启自动换行，则字符串即使超过绘制区域右边界，也不会自动折行，而是将字符串绘制到不可见的区域
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [OUT] width           内容宽度
// 【参数 3】      [OUT] height          内容高度
// 【参数 4】      [IN]  areaWidth       绘制区域宽度
// 【参数 5】      [IN]  fontAttr        字体属性
// 【参数 6】      [IN]  fontSize        字体大小
// 【参数 7】      [IN]  lineWrap        是否自动换行
// 【参数 8】      [IN]  lineSpace       行间距
// 【参数 9】      [IN]  columnSpace     列间距
// 【返回值】      [OUT] uint16型        行数
uint16 GetStringExSpacingDimension(const wchar_t *string, int32 *width, int32 *height, int16 areaWidth,
								   uint8 fontAttr, uint8 fontSize, bool lineWrap, int16 lineSpace, int16 columnSpace);

// 【功  能】  绘制字符串
// 【说  明】  该函数在指定缓存中指定位置绘制指定样式字符串，默认为屏幕缓存。字体属性及字体大小参见【字体各属性说明】
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [IN]  x               指定位置X坐标
// 【参数 3】      [IN]  y               指定位置Y坐标
// 【参数 4】      [IN]  clr             字体颜色
// 【参数 5】      [IN]  fontAttr        字体属性
// 【参数 6】      [IN]  fontSize        字体大小
// 【返回值】      [OUT] int32型         绘制后光标x轴位置
int32 DrawString(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize);

// 【功  能】  绘制指定字符间距的字符串
// 【说  明】  该函数在指定缓存中指定位置绘制指定样式字符串，默认为屏幕缓存。字体属性及字体大小参见【字体各属性说明】
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [IN]  x               指定位置X坐标
// 【参数 3】      [IN]  y               指定位置Y坐标
// 【参数 4】      [IN]  clr             字体颜色
// 【参数 5】      [IN]  fontAttr        字体属性
// 【参数 6】      [IN]  fontSize        字体大小
// 【参数 7】      [IN]  charSpace       字符间距
// 【返回值】      [OUT] int32型         绘制后光标x轴位置
int32 DrawStringSpacing(const wchar_t *string, int32 x, int32 y, mex_color clr, uint8 fontAttr, uint8 fontSize, int16 charSpace);

// 【功  能】  绘制字符串块
// 【说  明】  该函数在指定缓存中指定区域绘制指定样式字符串，默认为屏幕缓存。字体属性及字体大小参见【字体各属性说明】
//             该函数自动处理换行符【\r】及【\n】，【\r】将光标移动到该行开始处，【\n】将光标向下移动一行
//           ①若使用默认位置，则从绘制区域左上角开始绘制字符串，严格遵从换行符功能
//             若不使用默认位置，则换行符效果将变成不同字符串段落的分割，即【\r】将后面段落与前面分开成两个段落，但仍
//             在同一行，而【\n】将后面段落向下移动一行，而实际每一段落显示位置将遵从参数的位置属性
//           ②若开启自动换行，则当字符串绘制超过绘制区域右边界时，将自动折行到下一行。
//             若不开启自动换行，则字符串即使超过绘制区域右边界，也不会自动折行，而是将字符串绘制到不可见的区域
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [IN]  x1              字符串绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              字符串绘制区域左上角y坐标
// 【参数 4】      [IN]  x2              字符串绘制区域右下角x坐标
// 【参数 5】      [IN]  y2              字符串绘制区域右下角y坐标
// 【参数 6】      [IN]  clr             字体颜色
// 【参数 7】      [IN]  fontAttr        字体属性
// 【参数 8】      [IN]  fontSize        字体大小
// 【参数 9】      [IN]  fontPos         字体对齐方式
// 【参数10】      [IN]  lineWrap        是否自动换行
// 【返回值】      [OUT] int32型         绘制后光标x轴位置
int32 DrawStringEx(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, 
				   mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap);

// 【功  能】  绘制字符串块（可指定行间距及列间距）
// 【说  明】  该函数在指定缓存中指定区域绘制指定样式字符串，默认为屏幕缓存。字体属性及字体大小、位置参见【字体各属性说明】
//             该函数自动处理换行符【\r】及【\n】，【\r】将光标移动到该行开始处，【\n】将光标向下移动一行
//           ①若使用默认位置，则从绘制区域左上角开始绘制字符串，严格遵从换行符功能
//             若不使用默认位置，则换行符效果将变成不同字符串段落的分割，即【\r】将后面段落与前面分开成两个段落，但仍
//             在同一行，而【\n】将后面段落向下移动一行，而实际每一段落显示位置将遵从参数的位置属性
//           ②若开启自动换行，则当字符串绘制超过绘制区域右边界时，将自动折行到下一行。
//             若不开启自动换行，则字符串即使超过绘制区域右边界，也不会自动折行，而是将字符串绘制到不可见的区域
// 【注  意】  该函数接收的字符串必须为UCS2编码格式的字符串，若绘制的时英文字符串，可以直接在英文字符串前加上【L】表示，
//             但若绘制的时中文字符，必须使用函数【str2nwcs】或【tstr2wcs】进行转换才可正常显示，不可以直接在中文字符串
//             前加【L】前缀表示，【L】前缀接中文字符串在屏幕上会显示乱码。
// 【测  试】  已测试  T1 
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  string          字符串
// 【参数 2】      [IN]  x1              字符串绘制区域左上角x坐标
// 【参数 3】      [IN]  y1              字符串绘制区域左上角y坐标
// 【参数 4】      [IN]  x2              字符串绘制区域右下角x坐标
// 【参数 5】      [IN]  y2              字符串绘制区域右下角y坐标
// 【参数 6】      [IN]  clr             字体颜色
// 【参数 7】      [IN]  fontAttr        字体属性
// 【参数 8】      [IN]  fontSize        字体大小
// 【参数 9】      [IN]  fontPos         字体对齐方式
// 【参数10】      [IN]  lineWrap        是否自动换行
// 【参数11】      [IN]  lineSpace       行间距
// 【参数12】      [IN]  columnSpace     列间距
// 【返回值】      [OUT] int32型         绘制后光标x轴位置
int32 DrawStringExSpacing(const wchar_t *string, int32 x1, int32 y1, int32 x2, int32 y2, 
						  mex_color clr, uint8 fontAttr, uint8 fontSize, uint8 fontPos, bool lineWrap, int16 lineSpace, int16 columnSpace);

// 【功  能】  绘制字符
// 【说  明】  该函数在指定缓存中指定位置绘制指定样式字符，默认为屏幕缓存。字体属性及字体大小参见【字体各属性说明】
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  pCharData       字符点阵数据
// 【参数 2】      [IN]  dataSize        点阵数据大小
// 【参数 3】      [IN]  dataWidth       点阵数据宽度
// 【参数 4】      [IN]  dataHeight      点阵数据高度
// 【参数 5】      [IN]  x               绘制位置X坐标
// 【参数 6】      [IN]  y               绘制位置Y坐标
// 【参数 7】      [IN]  clr             字符颜色
// 【参数 8】      [IN]  fontAttr        字符属性
// 【返回值】      [OUT] 无
void DrawChar(uint8 *pCharData, uint32 dataSize, uint16 dataWidth, uint16 dataHeight, int32 x, int32 y, mex_color clr, uint8 fontAttr);

// 【功  能】  获取字符点阵数据
// 【说  明】  该函数获取指定大小字符点阵数据。字体大小参见【字体各属性说明】
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  ch              指定字符
// 【参数 2】      [OUT] ppCharData      点阵数据（指针的指针）
// 【参数 3】      [OUT] pDataWidth      点阵数据宽度（指针）
// 【参数 4】      [OUT] pDataHeight     点阵数据高度（指针）
// 【参数 5】      [IN]  fontSize        字体大小
// 【返回值】      [OUT] int32型         点阵数据大小
int32 GetCharData(int32 ch, uint8 **ppCharData, uint16 *pDataWidth, uint16 *pDataHeight, uint8 fontSize);

// 【功  能】  绘制点
// 【说  明】  该函数在指定缓存中绘制指定点，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x               X坐标
// 【参数 2】      [IN]  y               Y坐标
// 【参数 3】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawPointInternal(int32 x, int32 y, mex_color clr);

// 【功  能】  绘制直线
// 【说  明】  该函数在指定缓存中绘制指定直线，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1              左上角X坐标
// 【参数 2】      [IN]  y1              左上角Y坐标
// 【参数 3】      [IN]  x2              右下角X坐标
// 【参数 4】      [IN]  y2              右下角Y坐标
// 【参数 5】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawLineInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// 【功  能】  绘制线框矩形区域
// 【说  明】  该函数在指定缓存中绘制指定线框矩形区域，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1              左上角X坐标
// 【参数 2】      [IN]  y1              左上角Y坐标
// 【参数 3】      [IN]  x2              右下角X坐标
// 【参数 4】      [IN]  y2              右下角Y坐标
// 【参数 5】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawFrameRectInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// 【功  能】  绘制实心矩形区域
// 【说  明】  该函数在指定缓存中绘制指定实心矩形区域，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x1              左上角X坐标
// 【参数 2】      [IN]  y1              左上角Y坐标
// 【参数 3】      [IN]  x2              右下角X坐标
// 【参数 4】      [IN]  y2              右下角Y坐标
// 【参数 5】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawFillRectInternal(int32 x1, int32 y1, int32 x2, int32 y2, mex_color clr);

// 【功  能】  绘制线框圆形区域
// 【说  明】  该函数在指定缓存中绘制指定线框圆形区域，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x               圆形X坐标
// 【参数 2】      [IN]  y               圆形Y坐标
// 【参数 3】      [IN]  r               半径
// 【参数 5】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawFrameCircleInternal(int32 x, int32 y, int32 r, mex_color clr);

// 【功  能】  绘制实心圆形区域
// 【说  明】  该函数在指定缓存中绘制指定实心圆形区域，默认为屏幕缓存。
// 【测  试】  已测试  T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      [IN]  x               圆形X坐标
// 【参数 2】      [IN]  y               圆形Y坐标
// 【参数 3】      [IN]  r               半径
// 【参数 5】      [IN]  clr             颜色
// 【返回值】      [OUT] 无
void DrawFillCircleInternal(int32 x, int32 y, int32 r, mex_color clr);
// 原GUI库部分函数 end ////////////////////////////////////

//获取应用内部参数，内部函数
void GetAppParam(int32 iAppId, void *pResults);

//获取内存池管理函数，内部函数
void GetAdmFunctions(funcptr *pResults);

//取得应用是否允许进行某项操作
//参数1: 应用id   
//参数2: 权限类型
//------------------
//返回: 是否允许
bool IsAppPermissiveToDo( int32 iCurAppId, E_Mex_App_Permission_Type ePermission );


// 【功  能】  解压内存数据
// 【说  明】  该函数解压内存数据
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      memData         内存数据
// 【参数 2】      orgData         原始数据
// 【返回值】      bool型          是否成功
bool UnCompressDataFromMemory(uint8 *memData, uint8 *orgData);

// 【功  能】  解压文件数据
// 【说  明】  该函数解压文件数据
// 【测  试】  已测试 T1
// ---------------------------------------------------------------------------------------------------------------------
// 【参数 1】      file_handle     文件句柄
// 【参数 2】      orgData         原始数据
// 【返回值】      bool型          是否成功
bool UnCompressDataFromFile(int32 *file_handle, uint8 *orgData);


//调用Manager功能，用SendMessage实现，内部将来扩展使用
int32 CallManagerFunction(E_Mex_Mgr_Manager_Func eFunc, int32 iParam0, int32 iParam1, int32 iParam2);

//平台退出并模拟输入按键
//如果同一帧调用了RegisterLaunchSystemApp,则模拟按键输入在启动了的应用上
void RegisterSimulateKey(E_Mex_KeyEvent eKeyEvent, E_Mex_KeyCode eKeyCode);

//多线程相关函数，使用方法同SystemTimer
//func在另一个线程中执行，需小心互斥问题和释放问题
//建议有经验人员使用
int32 StartThreadTimer(int32 iTime, MexFuncPtr func);
bool KillThreadTimer(int32 iTimeId);
bool SetThreadTimerInterval(int32 iTimerId, int32 iNewTime, int32 *pOldTime);

void TakeMutex(int32 iIdx);
void GiveMutex(int32 iIdx);

//取得应用位置信息
//参数1 : 应用id
//参数2 : X坐标(out)
//参数3 : Y坐标(out)
//返回: 是否成功
bool GetAppPosition( int32 iAppId, int16 * pCenterPosX, int16 * pCenterPosY );

//设置应用位置信息，仅对widget有效
//参数1 : 应用id
//参数2 : X坐标
//参数3 : Y坐标
//返回: 是否成功
bool SetAppPosition( int32 iAppId, int16 iCenterPosX, int16 iCenterPosY );

//QQ系统状态栏注册
//参数 iAppid  调用者的Appid
//返回值  如果小于0 说明注册失败
// -1 表示 已经有两个QQ注册了状态栏 目前仅支持两个QQ同时运行
// -2 表示 系统不支持状态栏
//如果不小于0 则是状态栏句柄  以后的每次对状态栏的操作 都需要使用该句柄
int8 RegSystemStatusIcon(int32 iAppid);

//注销对状态栏的注册  
//参数 handle 要注销的状态栏的句柄
//iAppid  调用者的appid  
void UnRegSystemStatusIcon(int8 handle,int32 iAppid);

//更改状态栏状态
//参数 handle 需要更改的状态栏的句柄
//type  更改的状态
//iAppid  调用者的appid 
void SetSystemStatusIcon(int8 handle,E_MEX_QQ_STATUS_TYPE type,int32 iAppid);

//获取状态栏状态
//参数 handle 需要获取的状态栏的句柄

//iAppid  调用者的appid 

//返回值 当前状态栏状态

E_MEX_QQ_STATUS_TYPE GetSystemStatusIcon(int8 handle,int32 iAppid);

//显示或者关闭状态栏图标
//参数 handle 需要更改的系统图标的句柄
//bShow  true为显示 false为关闭
//iAppid 调用者id
void SetSystemStatusIconStatus(int8 handle,bool bShow,int32 iAppid);

//sdk >= 1001可用
//获取其它层layer buffer以及长宽信息
bool GetScreenLayerBufferWithInfo(int32 iLayer, uint16 **ppScrBuf, uint16 *puWidth, uint16 *puHeight);

//sdk >= 1004可用
//申请和释放t内存
void* tmalloc(size_t iSize);
void tfree(void* psPtr);

//sdk >= 1009可用
//初始化文件浏览
//参数sel_type 文件浏览选择类型
//		base_path 根路径
//		pfCB 文件选择结果回调
//返回是否成功

int8 Fmgr_Create(uint32 sel_type, wchar_t* base_path,Fmgr_CallBack pfCB);

//sdk >= 1009可用
//取消文件浏览
//Fmgr_Create 与Fmgr_Cancel需成对出现

void Fmgr_Cancel(void);

//sdk >= 1009可用
//设置文件类型过滤

void Fmgr_SetFilter(E_Mex_Fmgr_Filtertype filter);

//sdk >= 1009可用
//显示文件浏览界面
void Fmgr_Show(void);


void Pen_Block(void);

void Pen_unBlock(void);

void KeyPad_Lock(void);	

void KeyPad_unLock(void); 

void ReStartKeyPadLock(void);


//设置code cache
//参数1 [IN]: start_addr 开始地址
//参数2 [IN]: total_size 代码段大小
//参数3 [OUT]: cache_size 缓冲的大小
//返回: cache_id 缓冲编号,0为无效编号，其它值为有效的cache_id
int32 SetROCache(uint32 start_addr, uint32 total_size, uint32 *cache_size);

//取消code cache
//参数1 : cache_id 缓冲编号
//返回: 是否成功
bool ClearROCache(int32 cache_id);

//将icon id对应的状态栏图标修改的指定的image
void SetSystemStatusIcon_ext(int32 icon_id, int32 image_id);


//控制icon是否显示
void SetSystemStatusIconStatus_ext(int32 icon_id, bool bShow);

// rgb565 转换为420
int Rgb565ToYuv420(unsigned int *srcYUV, 
                              unsigned char *dstY, unsigned char *dstU, unsigned char *dstV, 
                              int width, int height);

//设置当前系统时间 
//注意设置dayIndex 字段无效
bool SetDateTime( Mex_DateTime DateTime );

//通过配置文件取得应用程序使用内存的最大值
//参数1：szFileDir，文件目录名
//参数2：szFileName，文件名
//参数3：bInSystemDir，是否在system目录
//返回值：应用程序使用内存的最大值
int32 GetAppMaxMemSize(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir);

//取得指定内存池剩余的可用内存
//参数1：eMemoryPoolType，哪块内存池
//返回值：指定内存池剩余的可用内存
int32 GetLeftMemSize(E_Mex_MemoryPool eMemoryPoolType);

//判断当前应用程序是否使用第三块内存
//参数1：应用程序Id
//返回值：true，当前应用程序使用第三块内存；false，不使用
bool IsUsingReservedMem(int32 iAppId);

//通过配置文件取得应用程序是不是internal or kernal
//参数1：szFileDir，文件目录名
//参数2：szFileName，文件名
//参数3：bInSystemDir，是否在system目录
//返回值: true，应用程序是
bool IsKernalApp(const wchar_t *szFileDir, const wchar_t *szFileName, bool bInSystemDir);

//当前应用是否是internal应用
bool IsAppInternal(int32 iAppId);

bool SetWidgetTop(bool bEnable, bool bTop);

bool SetWidgetVisiable(int32 iAppId, bool bVisiable);

//void BeginWidgetPaint(void);

//void EndWidgetPaint(void);

void* GetGdiLayer2Buf(int32* pMemSize);

void ReturnGdiLayer2Buf(void);

//设置应用程序的优先级
//参数1：iAppId，应用程序id
//参数2：iPriority，优先级
//返回值: true，设置成功；false，设置失败
bool SetAppPriority( int32 iAppId, int32 iPriority );

//设置widget应用是否接受key事件
//参数1：iAppId，应用程序id
//参数2：bAcccept，true，表明应用需要接受key事件；false，表明应用将不接受key event
//返回值: true，设置成功；false，设置失败
bool AllowAcceptKey( int32 iAppId, bool bAcccept );
bool GetAppFrameBuffer( int32 iAppId, uint16 **ppBuffer, int32 * pWidth, int32 * pHeight );
bool SetWidgetFullModeMemSize( int32 iAppId, uint32 size );
void* GetDeskAppImgPro(void);
int32 	SetManCurAppId(int32 iAppId);
E_Mex_RunMode GetRunModeFromAppId(int32 iAppId);
int32 		  AppIsInMemory(int32 iAppId);

#endif

#endif
